/*
 * A simple serial solution to the Case Study exercise from the MP
 * course.  Note that this uses the alternative boundary conditions
 * that are appropriate for the assessed coursework.
 */

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "arralloc.h"
#include "pgmio.h"
#include "definitions.h"


float boundaryval(int i, int m);

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
int get_north(MPI_Comm cart_comm, int my_rank)
{
  int north_rank;
  MPI_Cart_shift(cart_comm, 0, 1, &my_rank, &north_rank);
  return north_rank;
}

int get_south(MPI_Comm cart_comm, int my_rank)
{
  int south_rank;
  MPI_Cart_shift(cart_comm, 0, -1, &my_rank, &south_rank);
  return south_rank;
}

int get_east(MPI_Comm cart_comm, int my_rank)
{
  int east_rank;
  MPI_Cart_shift(cart_comm, 1, 1, &my_rank, &east_rank);
  return east_rank;
}

int get_west(MPI_Comm cart_comm, int my_rank)
{
  int west_rank;
  MPI_Cart_shift(cart_comm, 1, -1, &my_rank, &west_rank);
  return west_rank;
}

int mp_gather_and_write_png(float** partial_image, MPI_Comm* cart_comm, char* filename, int rank, int size)
{
  /*Send each local image back and form masterbuf!*/
  if (rank != 0)
  {
    MPI_Send(&(partial_image[0][0]), MP * NP, MPI_FLOAT, 0, 0, *cart_comm);
  }
  else
  {

    float masterbuf[M][N];
    printf("writing image\n");
    int proc, proc_coord[2];
    for (proc = 0; proc < size; proc++)
    {
      if (proc != 0)
      { 
        printf("bang");
        MPI_Recv(partial_image, MP * NP, MPI_FLOAT, proc, 0,
                 *cart_comm, MPI_STATUS_IGNORE);
      }
      MPI_Cart_coords(*cart_comm, proc, 2, proc_coord);
      printf("reassembling image from proc %i", proc);
      int i, j;
      for (i = 0; i < MP; i++)
      {
        for (j = 0; j < NP; j++)
        {
            printf("writing element [%i,%i]\n", i,j);
          masterbuf[(proc_coord[0] * MP) + (i - 1)]
                   [(proc_coord[1] * NP) + (j - 1)] = partial_image[i][j];
        }
      }
    }
 printf("writing image\n");
   printf("\nWriting <%s>\n", filename);
    pgmwrite(filename, masterbuf, M, N);
    return 0;
  }
}

float** return_masterbuf()
{
  float **masterbuf = (float**)arralloc(sizeof(float), 2, M, N);
  return masterbuf;
} 


int mp_get_coords(int rank, MPI_Comm cart_comm)
{

int main(int argc, char **argv)
{
  printf("intitialising message passing \n");
  
  int i, j, iter, maxiter;
  char *filename;
  float val;

  int rank, size;
  int north_rank, south_rank, east_rank, west_rank;

  MPI_Comm cart_comm;
  int initialised_error = mp_init(&rank, &size, &cart_comm, argc, argv);

  if (initialised_error)
  {
    printf("error initialising message passing\n");
    MPI_Abort(MPI_COMM_WORLD, -127);
    exit(-1);
  }

  float **old = (float**)arralloc(sizeof(float), 2, MP+2, NP+2);
  float **new = (float**)arralloc(sizeof(float), 2, MP+2, NP+2);
  float **edge = (float**)arralloc(sizeof(float), 2, MP+2, NP+2);
  float **partial_image = (float**)arralloc(sizeof(float), 2, MP, NP);
 
  float **masterbuf;
  if (rank==0)
  {
    masterbuf = return_masterbuf(); 
  }

  north_rank = get_north(cart_comm, rank);
  south_rank = get_south(cart_comm, rank);
  east_rank = get_east(cart_comm, rank);
  west_rank = get_west(cart_comm, rank);
  printf("lol\n");
  int coords[2];

  MPI_Cart_coords(cart_comm, rank, 2, coords);
  
  printf(
      "I am rank %i, coords [%i,%i] north is %i, south %i, west %i, east %i\n",
      rank, coords[0], coords[1], north_rank, south_rank, west_rank, east_rank);

  for (i = 0; i < MP + 2; i++)
  {
    for (j = 0; j < NP + 2; j++)
    {
      old[i][j] = 255.0;
    }
  }

  /* Master thread section */
  if (rank == 0)
  {
    printf("Processing %d x %d image\n", M, N);
    printf("Number of iterations = %d\n", MAXITER);
    filename = "inputs/edgenew768x768.pgm";

    printf("\nReading <%s>\n", filename);
    pgmread(filename, &(masterbuf[0][0]), M, N);
    printf("\n");

    /*Distribute bits of the image to different procs*/
    int proc, proc_coord[2];
    for (proc = size - 1; proc >= 0; proc--)
    {
      MPI_Cart_coords(cart_comm, proc, 2, proc_coord);

      //partial_image[0][0] = masterbuf[280][280];
      for (i = 0; i < MP; i++)
      {
        for (j = 0; j < NP; j++)
        {
          //printf("hello %i %i \n", (proc_coord[0] * MP) + (i ), (proc_coord[1] * NP) + (j) );
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
    if (rank == 2)
    {
      //printf("writing test file 3\n");
      //pgmwrite("test3.pgm", partial_image, MP, NP);
    }
  }
  
  mp_gather_and_write_png(partial_image, &cart_comm, "output/input.png", rank, size);


  //set old to be white
  printf("whiting out old %i \n", rank);
  for (i = 0; i < MP + 2; i++)
  {
    for (j = 0; j < NP + 2; j++)
    {
      old[i][j] = 255.0;
    }
  }

    if (rank == 0)
    {
        printf("writing test file\n");
        pgmwrite("outputs/test5.pgm", old, MP+1, NP+1);
    }
 
  /* Set fixed boundary conditions on the left and right sides */

  for (j = 1; j < NP + 1; j++)
  {
     //compute sawtooth value 

    val = boundaryval(j+ (NP*(1-coords[1]) ), NP);

    old[0][j] = 255.0 * val;
    old[MP + 1][j] = 255.0 * (1.0 - val);
  }
 
  float recv_north_buf[NP], recv_south_buf[NP], send_north_buf[MP],
      send_south_buf[MP];

 
  for (iter = 1; iter <= MAXITER; iter++)
  {
    if (iter % PRINTFREQ == 0 && rank == 0)
    {
      printf("Iteration %d\n", iter);
    }
    
    for (i = 0; i < MP; i++)
    {
      send_north_buf[i] = old[i + 1][1];
      send_south_buf[i] = old[i + 1][NP];
    }

    if (iter==1 && rank == 0)
    {
        printf("writing test file\n");
        pgmwrite("outputs/test6.pgm", old, MP+1, NP+1);
    }
 
  /*  // Send edge halos
    if (coords[1] % 2 == 0)
    {
      MPI_Send(&old[1][1], NP, MPI_FLOAT, west_rank, 0, cart_comm);
      MPI_Recv(&old[0][1], NP, MPI_FLOAT, west_rank, 0, cart_comm,
               MPI_STATUS_IGNORE);

      MPI_Send(&old[MP][1], NP, MPI_FLOAT, east_rank, 0, cart_comm);
      MPI_Recv(&old[MP + 1][1], NP, MPI_FLOAT, east_rank, 0, cart_comm,
               MPI_STATUS_IGNORE);
    }
    else
    {
      MPI_Recv(&old[MP + 1][1], NP, MPI_FLOAT, east_rank, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&old[MP][1], NP, MPI_FLOAT, east_rank, 0, cart_comm);

      MPI_Recv(&old[0][1], NP, MPI_FLOAT, west_rank, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&old[1][1], NP, MPI_FLOAT, west_rank, 0, cart_comm);
    }

    if (coords[0] % 2 == 0)
    {
      MPI_Send(&send_north_buf[0], MP, MPI_FLOAT, north_rank, 0, cart_comm);
      MPI_Recv(&recv_north_buf[0], MP, MPI_FLOAT, north_rank, 0, cart_comm,
               MPI_STATUS_IGNORE);

      MPI_Send(&send_south_buf[0], MP, MPI_FLOAT, south_rank, 0, cart_comm);
      MPI_Recv(&recv_south_buf[0], MP, MPI_FLOAT, south_rank, 0, cart_comm,
               MPI_STATUS_IGNORE);
    }
    else
    {
      MPI_Recv(&recv_south_buf[0], MP, MPI_FLOAT, south_rank, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&send_south_buf[0], MP, MPI_FLOAT, south_rank, 0, cart_comm);

      MPI_Recv(&recv_north_buf[0], MP, MPI_FLOAT, north_rank, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&send_north_buf[0], MP, MPI_FLOAT, north_rank, 0, cart_comm);
    }

    for (i = 0; i < MP; i++)
    {
      old[i + 1][NP + 1] = recv_south_buf[i];
      old[i + 1][1] = recv_north_buf[i];
    }

    if (iter==1 && rank == 0)
    {
        printf("writing test file\n");
        pgmwrite("outputs/test7.pgm", old, MP+1, NP+1);
    }
 
*/
    for (i = 1; i < MP + 1; i++)
    {
      for (j = 1; j < NP + 1; j++)
      {
        edge[i][j] = partial_image[i - 1][j - 1];
      }
    }

    for (i = 1; i < MP + 1; i++)
    {
      for (j = 1; j < NP + 1; j++)
      {
        new[i][j] = 0.25 * (old[i - 1][j] + old[i + 1][j] +
                                  old[i][j - 1] + old[i][j + 1] -
                                  edge[i][j]);
      }
    }

    // memory copy
    for (i = 1; i < MP + 1; i++)
    {
      for (j = 1; j < NP + 1; j++)
      {
        old[i][j] = new[i][j];
      }
    }
  }

  printf("\nFinished %d iterations\n", iter - 1);

  for (i = 1; i < MP + 1; i++)
  {
    for (j = 1; j < NP + 1; j++)
    {
      partial_image[i - 1][j - 1] = old[i][j];
    }
  }

  /*Send each local image back and form masterbuf!*/
  if (rank != 0)
  {
    MPI_Send(&partial_image[0], MP * NP, MPI_FLOAT, 0, 0, cart_comm);
  }
  else
  {
    int proc, proc_coord[2];
    for (proc = 0; proc < size; proc++)
    {
      if (proc != 0)
      {
        MPI_Recv(&partial_image[0], MP * NP, MPI_FLOAT, proc, 0,
                 cart_comm, MPI_STATUS_IGNORE);
      }

      MPI_Cart_coords(cart_comm, proc, 2, proc_coord);
      printf("reassembling image from proc %i", proc);
      for (i = 0; i < MP; i++)
      {
        for (j = 0; j < NP; j++)
        {
          masterbuf[(proc_coord[0] * MP) + (i - 1)]
                   [(proc_coord[1] * NP) + (j - 1)] = partial_image[i][j];
        }
      }
    }
    filename = "outputs/output.pgm";
    printf("\nWriting <%s>\n", filename);
    pgmwrite(filename, masterbuf, M, N);
  }

//  mp_gather_and_write_png(partial_image, &cart_comm, "output.png", rank, size);

  MPI_Finalize();
}

float boundaryval(int i, int m)
{
  float val;

  val = 2.0 * ((float)(i - 1)) / ((float)(m - 1));
  if (i >= m / 2 + 1)
    val = 2.0 - val;

  return val;
}
