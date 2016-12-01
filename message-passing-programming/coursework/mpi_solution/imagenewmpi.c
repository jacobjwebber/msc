/*
 * A solution to the coursework for the MP
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
#include "mp_functions.h"

real_number boundaryval(int i, int m);

void print_array(float array[MP+2][NP+2], int rank);

int main(int argc, char **argv)
{
  printf("intitialising message passing \n");
  
  int i, j, iter, maxiter;
  char *filename;
  real_number val;

  int rank, size;
  int north, south, east, west;

  MPI_Comm cart_comm;
  int initialised_error = mp_init(&rank, &size, &cart_comm, argc, argv);

  if (initialised_error)
  {
    printf("error initialising message passing\n");
    MPI_Abort(MPI_COMM_WORLD, initialised_error);
    exit(-1);
  }
  
  if (rank == 0)
      printf("message passing initialised\n");
  real_number old[MP+2][NP+2], new[MP+2][NP+2], edge[MP+2][NP+2],
        partial_image[MP][NP];
  
  //find neighbors for stenciling.
  north = mp_get_north(cart_comm, rank);
  south = mp_get_south(cart_comm, rank);
  east = mp_get_east(cart_comm, rank);
  west = mp_get_west(cart_comm, rank);
  
  int coords[2];
  mp_get_coords(&cart_comm, rank, &(coords[0]));
  
  printf(
      "I am rank %i, coords [%i,%i] north is %i, south %i, west %i, east %i\n",
      rank, coords[0], coords[1], north, south, west, east);
  
  if (rank ==0)
  {
    printf("Processing %d x %d image\n", M, N);
    printf("Number of iterations = %d\n", MAXITER);
  }

  filename = "../inputs/edgenew768x768.pgm";

    for (i=0; i < MP+2; i++)
    {
      for (j= 0; j < NP+2; j++)
      {
        edge[i][j] = rank;
      }
    }
    mp_scatter(cart_comm, filename , partial_image, size, rank);
    if (rank == 1)
      pgmwrite("../outputs/outputs_partial1.pgm", partial_image, MP, NP);
 

    mp_gather_and_write_png(cart_comm, "../outputs/input.pgm", partial_image, size, rank);

  
  //set edge to be partial image with halos.
  for (i = 1; i < MP + 1; i++)
  {
    for (j = 1; j < NP + 1; j++)
    {
      edge[i][j] = partial_image[i - 1][j - 1];
    }
  }
 
     
  //set old to be white
  printf("whiting out old %i \n", rank);
  for (i = 0; i < MP + 2; i++)
  {
    for (j = 0; j < NP + 2; j++)
    {
      old[i][j] = 255.0;
    }
  }
  
  // Set fixed boundary conditions on the left and right sides
  for (j = 1; j < NP + 1; j++)
  {
     //compute sawtooth value 

    val = boundaryval(j+ (NP*(1-coords[0]) ), NP);

    old[0][j] = 255.0 - val;
    old[MP + 1][j] = 255.0 - (1.0 - val);
  }
    
  //pgmwrite("../outputs/old.pgm", old, MP+2, NP+2);
  
  //MAIN LOOP 
  for (iter = 1; iter <= MAXITER; iter++)
  {
    if (iter % PRINTFREQ == 0 && rank == 0)
    {
      printf("Iteration %d\n", iter);
    }
    
    //mp_halo_swap(cart_comm, old, north, south, east, west, coords);
    
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
  } //END MAIN LOOP

//  if (rank ==0)
//    print_array(old,rank);
  
  
  printf("\nProc %i Finished %d iterations\n", rank, iter - 1);
  
  //write to output local buffer
  for (i = 1; i < MP + 1; i++)
  {
    for (j = 1; j < NP + 1; j++)
    {
      partial_image[i - 1][j - 1] = old[i][j];
    }
  }

  if (rank == 0)
  {
      pgmwrite("../outputs/lalala.pgm", partial_image, MP, NP);
  }
  
  printf("partial image [00,00]from proc %i = %f \n", partial_image[0][0], rank);
  mp_gather_and_write_png(cart_comm, "../outputs/output.pgm", partial_image, size, rank);
  
  MPI_Finalize();
}

real_number boundaryval(int i, int m)
{
  real_number val;

  val = 2.0 * ((real_number)(i - 1)) / ((real_number)(m - 1));
  if (i >= m / 2 + 1)
    val = 2.0 - val;

  return val;
}

void print_array(float array[MP+2][NP+2], int rank)
{
    int i, j;
    for (i=0; i < MP+2; i++)
    {
        for (j=0; j < NP+2; j++)
        {
            printf("%.1f ", array[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

