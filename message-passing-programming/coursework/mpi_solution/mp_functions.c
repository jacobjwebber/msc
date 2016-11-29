
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


int mp_get_coords(MPI_Comm* cart_comm, int rank, int* coord)
{
  MPI_Cart_coords(*cart_comm, rank, 2, coord);
  return 0;
}

