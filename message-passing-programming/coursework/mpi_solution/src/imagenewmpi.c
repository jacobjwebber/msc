/*
 * A solution to the coursework for the WIDTH_P
 * course.  HEIGHTote that this uses the alternative boundary conditions
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

void print_array(float array[WIDTH_P + 2][HEIGHT_P + 2], int rank);

int main(int argc, char **argv)
{
  
  char *filename;
  filename = "../inputs/edgenew512x384.pgm"; // "../inputs/edgenew768x768.pgm";

  int i, j, iter, maxiter;
  real_number val;

  int rank, size;
  int up, down, right, left;

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
  real_number old[WIDTH_P + 2][HEIGHT_P + 2], new[WIDTH_P + 2][HEIGHT_P + 2], 
              edge[WIDTH_P + 2][HEIGHT_P + 2],
      partial_image[WIDTH_P][HEIGHT_P];

  // find neighbors for stenciling.
  up = mp_get_up(cart_comm, rank);
  down = mp_get_down(cart_comm, rank);
  right = mp_get_right(cart_comm, rank);
  left = mp_get_left(cart_comm, rank);

  int coords[2];
  mp_get_coords(&cart_comm, rank, &(coords[0]));

  printf(
      "I am rank %i, coords [%i,%i] up is %i, down %i, left %i, right %i\n",
      rank, coords[0], coords[1], up, down, left, right);

  if (rank == 0)
  {
    printf("Processing %d x %d image\n", WIDTH_P, HEIGHT);
    printf("Number of iterations = %d\n", MAXITER);
  }

  mp_scatter(cart_comm, filename, partial_image, size, rank);
  
  mp_gather_and_write_png(cart_comm, "../outputs/input.pgm", partial_image,
                          size, rank);

  // set edge to be partial image with halos.
  for (i = 1; i < WIDTH_P + 1; i++)
  {
    for (j = 1; j < HEIGHT_P + 1; j++)
    {
      edge[i][j] = partial_image[i - 1][j - 1];
    }
  }

  // set old to be white
  for (i = 0; i < WIDTH_P + 2; i++)
  {
    for (j = 0; j < HEIGHT_P + 2; j++)
    {
      old[i][j] = 255.0;
    }
  }
  // Set fixed boundary conditions on the left and right sides
  for (j = 1; j < HEIGHT_P + 1; j++)
  {
    // compute sawtooth value

    val = boundaryval(j + (HEIGHT_P * (1 - coords[0])), HEIGHT_P);

    old[0][j] = 255.0; // - val;
    old[WIDTH_P + 1][j] = 255.0; // - (1.0 - val);
  }

  // pgmwrite("../outputs/old.pgm", old, WIDTH_P+2, HEIGHT_P+2);
  
  //Create vector type
  MPI_Datatype h_halo, v_halo;
  mp_create_vector_type(&v_halo, &h_halo);

  real_number delta;
  // BEGIN MAIN LOOP
  double t1, t2;
  t1 = MPI_Wtime();
  for (iter = 1; iter <= MAXITER; iter++)
  {
    if (iter % PRINTFREQ == 0 && rank == 0)
    {
      printf("Iteration %d\n", iter);
    }

    mp_halo_swap(cart_comm, &h_halo, &v_halo, old, up, down, right, left, coords);


    for (i = 1; i < WIDTH_P + 1; i++)
    {
      for (j = 1; j < HEIGHT_P + 1; j++)
      {
        new[i][j] = 0.25 * (old[i - 1][j] + old[i + 1][j] + old[i][j - 1] +
                            old[i][j + 1] - edge[i][j]);
      }
    }

    //Calculate max delta every 20 iterations
    if (iter % 20 == 0)
    {
        delta = max_delta(cart_comm, old, new);
        if (delta < SMALL_DELTA)
            break;
    }

    // memory copy
    for (i = 0; i < WIDTH_P + 1; i++)
    {
      for (j = 0; j < HEIGHT_P + 1; j++)
      {
        old[i][j] = new[i][j];
      }
    }
   
  } // END MAIN LOOP
  t2 = MPI_Wtime();


  // write to output local buffer
  for (i = 1; i < WIDTH_P + 1; i++)
  {
    for (j = 1; j < HEIGHT_P + 1; j++)
    {
      partial_image[i - 1][j - 1] = old[i][j];
    }
  }

  mp_gather_and_write_png(cart_comm, "../outputs/output.pgm", partial_image,
                          size, rank);
  if (rank == 0)
  {
      printf("Completed %i iterations in %f seconds\n", iter-1, t2-t1);
  }
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

