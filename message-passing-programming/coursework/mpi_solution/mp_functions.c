
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "arralloc.h"
#include "pgmio.h"
#include "definitions.h"
#include "mp_functions.h"

int mp_init(int *rank, int *size, MPI_Comm *cart_comm, int argc, char **argv)
{

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, size);

  if (*size != P)
  {
    if (rank == 0)
      printf("ERROR: size = %i, P = %i\n", size, P);

    return -1;
  }
  else
  {

    MPI_Comm old_comm, new_comm;
    int ndims, reorder, ierr;
    int dim_size[2], periods[2];

    old_comm = MPI_COMM_WORLD;
    ndims = 2;
    dim_size[0] = DIMX; // replace with MPI_create_dims if poss
    dim_size[1] = DIMY;
    periods[0] = 0;
    periods[1] = 0;
    reorder = 1;

    ierr =
        MPI_Cart_create(old_comm, ndims, dim_size, periods, reorder, cart_comm);

    MPI_Comm_rank(*cart_comm, rank);

    return ierr;
  }
}

/*Slightly verbose list of functions for finding neighbors*/
int mp_get_north(MPI_Comm cart_comm, int my_rank)
{
  int north_rank;
  MPI_Cart_shift(cart_comm, 0, 1, &my_rank, &north_rank);
  return north_rank;
}

int mp_get_south(MPI_Comm cart_comm, int my_rank)
{
  int south_rank;
  MPI_Cart_shift(cart_comm, 0, -1, &my_rank, &south_rank);
  return south_rank;
}

int mp_get_east(MPI_Comm cart_comm, int my_rank)
{
  int east_rank;
  MPI_Cart_shift(cart_comm, 1, 1, &my_rank, &east_rank);
  return east_rank;
}

int mp_get_west(MPI_Comm cart_comm, int my_rank)
{
  int west_rank;
  MPI_Cart_shift(cart_comm, 1, -1, &my_rank, &west_rank);
  return west_rank;
}

int mp_scatter(MPI_Comm cart_comm, char* filename, int size, int rank, float partial_image[MP][NP])
{
/* Master thread section */
  if (rank == 0)
  {
    printf("Processing %d x %d image\n", M, N);
    printf("Number of iterations = %d\n", MAXITER);
    filename = "inputs/edgenew768x768.pgm";

    float masterbuf[M][N];
    printf("\nReading <%s>\n", filename);
    pgmread(filename, &(masterbuf[0][0]), M, N);
    printf("\n");
    masterbuf[M][N] = 8.0;
    printf("yeah");
    pgmwrite("outputs/intput.pgm", masterbuf, M, N);
 
    /*Distribute bits of the image to different procs*/
    int proc, proc_coord[2];
    for (proc = size - 1; proc >= 0; proc--)
    {
      MPI_Cart_coords(cart_comm, proc, 2, proc_coord);
      printf("sending to %i\n", proc);
      int i, j;
      for (i = 0; i < MP; i++)
      {
        for (j = 0; j < NP; j++)
        {
          partial_image[i][j] = masterbuf[(proc_coord[0] * MP) + i]
                                               [(proc_coord[1] * NP) + (j )];
        }
      }

      if (proc != 0)
      {
        // send to this process
        printf("sending to proc %i\n", proc);
        MPI_Send(&partial_image[0], NP * MP, MPI_FLOAT, proc, 0,
                 cart_comm);
      }
    }
  }
  else
  {
    MPI_Recv(&partial_image[0], NP * MP, MPI_FLOAT, 0, 0, cart_comm,
             MPI_STATUS_IGNORE);
    printf("proc %i recieved partial image from master\n", rank);
  }

} 

int mp_gather_and_write_png(float partial_image[MP][NP], MPI_Comm* cart_comm, char* filename, int rank, int size)
{
  //Send each local image back and form masterbuf!
  if (rank != 0)
  {
    MPI_Send(&(partial_image[0][0]), MP * NP, MPI_FLOAT, 0, 0, *cart_comm);
  }
  else
  {
	float masterbuf[M][N];
    int i, j;
    int proc, proc_coord[2];

    //first, write proc 0's elements to masterbuf
    MPI_Cart_coords(*cart_comm, 0, 2, proc_coord);
    for (i = 0; i < MP; i++)
    {
      for (j = 0; j < NP; j++)
      {
        masterbuf[(proc_coord[0] * MP) + (i - 1)]
                 [(proc_coord[1] * NP) + (j - 1)] = partial_image[i][j];
      }
    }

    printf("writing image\n");
    float receive_buf[MP*NP];
    for (proc = 0; proc < size; proc++)
    {
      if (proc != 0)
      { 
        MPI_Recv(&(receive_buf[0]), MP * NP, MPI_FLOAT, proc, 0,
                 *cart_comm, MPI_STATUS_IGNORE);
      }
      MPI_Cart_coords(*cart_comm, proc, 2, proc_coord);
      printf("reassembling image from proc %i\n", proc);
      int i, j;
      for (i = 0; i < MP; i++)
      {
        for (j = 0; j < NP; j++)
        {
          masterbuf[(proc_coord[0] * MP) + (i - 1)]
                   [(proc_coord[1] * NP) + (j - 1)] = receive_buf[i*MP + j];
        }
      }
    }
    printf("\nWriting <%s>\n", filename);
    pgmwrite(filename, masterbuf, M, N);
    return 0;
  }
}

/*float** return_masterbuf()
{
  float **masterbuf = (float**)arralloc(sizeof(float), 2, M, N);
  return masterbuf;
} 
*/

int mp_get_coords(MPI_Comm* cart_comm, int rank, int* coord)
{
  MPI_Cart_coords(*cart_comm, rank, 2, coord);
  return 0;
}


int mp_halo_swap(MPI_Comm cart_comm, float old[MP+2][NP+2], 
                int north, int south, int east, int west, int coords[2])
{    

 
    float recv_north_buf[NP], recv_south_buf[NP], send_north_buf[MP],
      send_south_buf[MP];

    int i,j;
    for (i = 0; i < MP; i++)
    {
      send_north_buf[i] = old[i + 1][1];
      send_south_buf[i] = old[i + 1][NP];
    }
 
    // Send edge halos
    if (coords[1] % 2 == 0)
    {
      MPI_Send(&old[1][1], NP, MPI_FLOAT, west, 0, cart_comm);
      MPI_Recv(&old[0][1], NP, MPI_FLOAT, west, 0, cart_comm,
               MPI_STATUS_IGNORE);

      MPI_Send(&old[MP][1], NP, MPI_FLOAT, east, 0, cart_comm);
      MPI_Recv(&old[MP + 1][1], NP, MPI_FLOAT, east, 0, cart_comm,
               MPI_STATUS_IGNORE);
    }
    else
    {
      MPI_Recv(&old[MP + 1][1], NP, MPI_FLOAT, east, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&old[MP][1], NP, MPI_FLOAT, east, 0, cart_comm);

      MPI_Recv(&old[0][1], NP, MPI_FLOAT, west, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&old[1][1], NP, MPI_FLOAT, west, 0, cart_comm);
    }

    if (coords[0] % 2 == 0)
    {
      MPI_Send(&send_north_buf[0], MP, MPI_FLOAT, north, 0, cart_comm);
      MPI_Recv(&recv_north_buf[0], MP, MPI_FLOAT, north, 0, cart_comm,
               MPI_STATUS_IGNORE);

      MPI_Send(&send_south_buf[0], MP, MPI_FLOAT, south, 0, cart_comm);
      MPI_Recv(&recv_south_buf[0], MP, MPI_FLOAT, south, 0, cart_comm,
               MPI_STATUS_IGNORE);
    }
    else
    {
      MPI_Recv(&recv_south_buf[0], MP, MPI_FLOAT, south, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&send_south_buf[0], MP, MPI_FLOAT, south, 0, cart_comm);

      MPI_Recv(&recv_north_buf[0], MP, MPI_FLOAT, north, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&send_north_buf[0], MP, MPI_FLOAT, north, 0, cart_comm);
    }

    for (i = 0; i < MP; i++)
    {
      old[i + 1][NP + 1] = recv_south_buf[i];
      old[i + 1][1] = recv_north_buf[i];
    }
} 
