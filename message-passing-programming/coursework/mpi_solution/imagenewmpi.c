/*
 * A simple serial solution to the Case Study exercise from the MP
 * course.  Note that this uses the alternative boundary conditions
 * that are appropriate for the assessed coursework.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include "pgmio.h"

#define M 192
#define N 128

#define MAXITER   1500
#define PRINTFREQ  200

#define P 4

#define MP M/2
#define NP N/2

float boundaryval(int i, int m);

int mp_init(int* rank, int* size, MPI_Comm* cart_comm, int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, size);

    if (*size != P)
    {
        if (rank==0) printf("ERROR: size = %i, P = %i\n", size, P);
        
        return -1;
    }
    else
    {
    
    MPI_Comm  old_comm, new_comm;
    int ndims, reorder, ierr;
    int dim_size[2], periods[2];

    old_comm = MPI_COMM_WORLD;
    ndims = 2;
    dim_size[0] = 2; //replace with MPI_create_dims if poss
    dim_size[1] = 2;
    periods[0] = 0;
    periods[1] = 1;
    reorder = 1;
          
    ierr =  MPI_Cart_create(old_comm,ndims,dim_size,
                       periods,reorder,cart_comm);

    MPI_Comm_rank(*cart_comm, rank);
    
    return ierr;
    }

}

int get_north(MPI_Comm cart_comm, int my_rank)
{
    int north_rank;
    MPI_Cart_shift(cart_comm, 1, 1, &my_rank, &north_rank);
    return north_rank;
}


int get_south(MPI_Comm cart_comm, int my_rank)
{
    int south_rank;
    MPI_Cart_shift(cart_comm, 1, -1, &my_rank, &south_rank);
    return south_rank;
}

int get_east(MPI_Comm cart_comm, int my_rank)
{
    int east_rank;
    MPI_Cart_shift(cart_comm, 0, 1, &my_rank, &east_rank);
    return east_rank;
}


int get_west(MPI_Comm cart_comm, int my_rank)
{
    int west_rank;
    MPI_Cart_shift(cart_comm, 0, -1, &my_rank, &west_rank);
    return west_rank;
}

int main (int argc, char **argv)
{
  float old[M+2][N+2], new[M+2][N+2], edge[M+2][N+2];

  float masterbuf[M][N];

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
      MPI_Finalize();
      exit(-1);
  }
  
  north_rank = get_north(cart_comm, rank);
  south_rank = get_south(cart_comm, rank);
  east_rank = get_east(cart_comm, rank);
  west_rank = get_west(cart_comm, rank);
  
  int coords[2];

  MPI_Cart_coords(cart_comm, rank, 2, coords);
  printf("I am rank %i, coords [%i,%i] north is %i, south %i, west %i, east %i\n", 
                 rank, coords[0], coords[1], north_rank, south_rank, west_rank, east_rank);


  /* Master thread section */
  if (rank ==0)
  {
    printf("Processing %d x %d image\n", M, N);
    printf("Number of iterations = %d\n", MAXITER);
        filename = "edgenew192x128.pgm";

    printf("\nReading <%s>\n", filename);
    pgmread(filename, masterbuf, M, N);
    printf("\n");


    for (i=1;i<M+1;i++)
    {
      for (j=1;j<N+1;j++)
	  {
	    edge[i][j]=masterbuf[i-1][j-1];
	  }
    }

  for (i=0; i<M+2;i++)
    {
      for (j=0;j<N+2;j++)
	{
	  old[i][j]=255.0;
	}
    }

  /* Set fixed boundary conditions on the left and right sides */

  for (j=1; j < N+1; j++)
    {
      /* compute sawtooth value */
     
      val = boundaryval(j, N);

      old[0][j]   = 255.0*val;
      old[M+1][j] = 255.0*(1.0-val);
    }
  


  for (iter=1;iter<=MAXITER; iter++)
    {
      if(iter%PRINTFREQ==0)
	{
	  printf("Iteration %d\n", iter);
	}

    
      /* Implement periodic boundary conditions on top and bottom sides */

      for (i=1; i < M+1; i++)
	{
	  old[i][0]   = old[i][N];
	  old[i][N+1] = old[i][1];
	}

      for (i=1;i<M+1;i++)
	{
	  for (j=1;j<N+1;j++)
	    {
	      new[i][j]=0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]
			      - edge[i][j]);
	    }
	}
	
      for (i=1;i<M+1;i++)
	{
	  for (j=1;j<N+1;j++)
	    {
	      old[i][j]=new[i][j];
	    }
	}
    }

  printf("\nFinished %d iterations\n", iter-1);

  for (i=1;i<M+1;i++)
    {
      for (j=1;j<N+1;j++)
	{
	  masterbuf[i-1][j-1]=old[i][j];
	}
    }

  filename="imagenew192x128.pgm";
  printf("\nWriting <%s>\n", filename); 
  pgmwrite(filename, masterbuf, M, N);

  }
  MPI_Finalize();
} 

float boundaryval(int i, int m)
{
  float val;

  val = 2.0*((float)(i-1))/((float)(m-1));
  if (i >= m/2+1) val = 2.0-val;
  
  return val;
}
