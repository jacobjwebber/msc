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
    periods[0] = 1;
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

int mp_scatter(MPI_Comm cart_comm, char* filename, real_number partial_image[MP][NP], int size, int rank)
{
// Master thread section 
  if (rank == 0)
  {
    printf("MP SCATTER\n\n");
    real_number masterbuf[M][N];
    printf("\nReading <%s>poo\n/", filename);
    pgmread(filename, &(masterbuf[0][0]), M, N);
    printf("\n");
    masterbuf[M][N] = 8.0;
    printf("yeah");
    pgmwrite("../outputs/input_unmodified.pgm", masterbuf, M, N);
 
    //Distribute bits of the image to different procs
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
        MPI_Send(&partial_image[0], NP * MP, MPI_REALNUMBER, proc, 0,
                 cart_comm);
      }
    }
  }
  else
  {
      //other threads
    MPI_Recv(&(partial_image[0][0]), NP * MP, MPI_REALNUMBER, 0, 0, cart_comm,
             MPI_STATUS_IGNORE);
    printf("proc %i recieved partial image from master\n", rank);   
  }
 } 

int mp_gather_and_write_png(MPI_Comm cart_comm, char* filename, real_number partial_image[MP][NP], int size, int rank)
{
   if (rank != 0)
   {
       MPI_Send(&(partial_image[0][0]), NP*MP, MPI_REALNUMBER, 0,0, cart_comm);
   }
   else
   {
    printf("MP GATHER AND WRITE \n\n");
    real_number masterbuf[M][N], recv_buf[MP][NP];


    int proc, proc_coord[2];
    
    MPI_Cart_coords(cart_comm, 0, 2, proc_coord);
    int i, j;
    for (i = 0; i < MP; i++)
    {
      for (j = 0; j < NP; j++)
      {
         masterbuf[(proc_coord[0] * MP) + i]
                  [(proc_coord[1] * NP) + (j )] = partial_image[i][j]; 
      }
    }
    
    for (proc = 1; proc < size; proc++)
    {
      printf("Receiving from:  %i\n", proc);
      if (proc != 0)
      {
        MPI_Recv(&(recv_buf[0][0]), NP * MP, MPI_REALNUMBER, proc, 0, cart_comm,
                 MPI_STATUS_IGNORE);
      }
      
      MPI_Cart_coords(cart_comm, proc, 2, proc_coord);
      for (i = 0; i < MP; i++)
      {
        for (j = 0; j < NP; j++)
        {
           masterbuf[(proc_coord[0] * MP) + i]
                    [(proc_coord[1] * NP) + (j )] = recv_buf[i][j]; 
        }
      }
    }
    pgmwrite(filename, masterbuf, M, N);
   }
   return 0;
}

int mp_get_coords(MPI_Comm* cart_comm, int rank, int* coord)
{
  MPI_Cart_coords(*cart_comm, rank, 2, coord);
  return 0;
}


int mp_halo_swap(MPI_Comm cart_comm, real_number old[MP+2][NP+2], 
                int north, int south, int east, int west, int coords[2])
{    
    
    real_number recv_east_buf[NP], recv_west_buf[NP], send_east_buf[MP],
      send_west_buf[MP];
    
    int i,j;
    for (i = 0; i < MP; i++)
    {
      recv_east_buf[i] = old[i + 1][1];
      recv_west_buf[i] = old;
      send_east_buf[i] = old[i + 1][1];
      send_west_buf[i] = old[i + 1][NP];
    }
 
    // Send edge halos

    //up-down
    if (DIMY == DIMY)
    {
        if (coords[0] % 2 == 0)
        {
            MPI_Send(&old[MP][1], NP, MPI_REALNUMBER, east, 0, cart_comm);
            MPI_Recv(&old[MP + 1][1], NP, MPI_REALNUMBER, east, 0, cart_comm,
                    MPI_STATUS_IGNORE);

            MPI_Send(&old[1][1], NP, MPI_REALNUMBER, west, 0, cart_comm);
            MPI_Recv(&old[0][1], NP, MPI_REALNUMBER, west, 0, cart_comm,
                    MPI_STATUS_IGNORE);
        }
        else
        {
            MPI_Recv(&old[MP + 1][1], NP, MPI_REALNUMBER, west, 0, cart_comm,
                    MPI_STATUS_IGNORE);
            MPI_Send(&old[MP][1], NP, MPI_REALNUMBER, west, 0, cart_comm);

            MPI_Recv(&old[0][1], NP, MPI_REALNUMBER, east, 0, cart_comm,
                    MPI_STATUS_IGNORE);
            MPI_Send(&old[1][1], NP, MPI_REALNUMBER, east, 0, cart_comm);
        }
    }
    //left-right
    if (north == south)
    {
      for (i=1; i < M+1; i++)
      {
        old[i][0]   = old[i][N];
        old[i][N+1] = old[i][1];
      }
    }
    else
    {

        if (coords[1] % 2 == 0)
        {
            MPI_Send(&(send_east_buf[0]), MP, MPI_REALNUMBER, east, 0, cart_comm);
            MPI_Recv(&(recv_east_buf[0]), MP, MPI_REALNUMBER, east, 0, cart_comm,
                     MPI_STATUS_IGNORE);

            MPI_Send(&(send_west_buf[0]), MP, MPI_REALNUMBER, west, 0, cart_comm);
            MPI_Recv(&(recv_west_buf[0]), MP, MPI_REALNUMBER, west, 0, cart_comm,
                     MPI_STATUS_IGNORE);
        }
        else
        {
            MPI_Recv(&(recv_west_buf[0]), MP, MPI_REALNUMBER, west, 0, cart_comm,
                    MPI_STATUS_IGNORE);
            MPI_Send(&(send_west_buf[0]), MP, MPI_REALNUMBER, west, 0, cart_comm);

            MPI_Recv(&(recv_west_buf[0]), MP, MPI_REALNUMBER, east, 0, cart_comm,
                    MPI_STATUS_IGNORE);
            
            MPI_Send(&(send_west_buf[0]), MP, MPI_REALNUMBER, east, 0, cart_comm);
        }
    
        for (i = 0; i < MP; i++)
        {
            old[i + 1][NP + 1] = recv_west_buf[i];
            old[i + 1][0] = recv_east_buf[i];
        }
    }
    return 0;
} 
