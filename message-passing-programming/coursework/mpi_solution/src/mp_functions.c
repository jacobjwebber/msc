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
int mp_get_up(MPI_Comm cart_comm, int my_rank)
{
  int up_rank;
  MPI_Cart_shift(cart_comm, 1, 1, &my_rank, &up_rank);
  return up_rank;
}

int mp_get_down(MPI_Comm cart_comm, int my_rank)
{
  int down_rank;
  MPI_Cart_shift(cart_comm, 1, -1, &my_rank, &down_rank);
  return down_rank;
}

int mp_get_right(MPI_Comm cart_comm, int my_rank)
{
  int right_rank;
  MPI_Cart_shift(cart_comm, 0, 1, &my_rank, &right_rank);
  return right_rank;
}

int mp_get_left(MPI_Comm cart_comm, int my_rank)
{
  int left_rank;
  MPI_Cart_shift(cart_comm, 0, -1, &my_rank, &left_rank);
  return left_rank;
}

int mp_scatter(MPI_Comm cart_comm, char* filename, 
			   real_number partial_image[WIDTH_P][HEIGHT_P], int size, int rank)
{
// Master thread section 
  if (rank == 0)
  {
    real_number masterbuf[WIDTH][HEIGHT];
    printf("\nReading <%s>\n", filename);
    pgmread(filename, &(masterbuf[0][0]), WIDTH, HEIGHT);
    printf("\n");
    masterbuf[WIDTH][HEIGHT] = 8.0;
    pgmwrite("../outputs/input_unmodified.pgm", masterbuf, WIDTH, HEIGHT);
 
    //Distribute bits of the image to different procs
    int proc, proc_coord[2];
    for (proc = size - 1; proc >= 0; proc--)
    {
      MPI_Cart_coords(cart_comm, proc, 2, proc_coord);
      int i, j;
      for (i = 0; i < WIDTH_P; i++)
      {
        for (j = 0; j < HEIGHT_P; j++)
        {
          partial_image[i][j] = masterbuf[(proc_coord[0] * WIDTH_P) + i]
                                         [(proc_coord[1] * HEIGHT_P) + (j )];
        }
      }

      if (proc != 0)
      {
        // send to this process
        MPI_Send(&partial_image[0], HEIGHT_P * WIDTH_P, MPI_REALNUMBER, proc, 0,
                 cart_comm);
      }
    }
  }
  else
  {
      //other threads
    MPI_Recv(&(partial_image[0][0]), HEIGHT_P * WIDTH_P, MPI_REALNUMBER, 0, 0, cart_comm,
             MPI_STATUS_IGNORE);
  }
 } 

int mp_gather_and_write_png(MPI_Comm cart_comm, char* filename, 
							real_number partial_image[WIDTH_P][HEIGHT_P], 
							int size, int rank)
{
   if (rank != 0)
   {
       MPI_Send(&(partial_image[0][0]), HEIGHT_P*WIDTH_P, MPI_REALNUMBER, 0,0, cart_comm);
   }
   else
   {
    real_number masterbuf[WIDTH][HEIGHT], recv_buf[WIDTH_P][HEIGHT_P];


    int proc, proc_coord[2];
    
    MPI_Cart_coords(cart_comm, 0, 2, proc_coord);
    int i, j;
    for (i = 0; i < WIDTH_P; i++)
    {
      for (j = 0; j < HEIGHT_P; j++)
      {
         masterbuf[(proc_coord[0] * WIDTH_P) + i]
                  [(proc_coord[1] * HEIGHT_P) + (j )] = partial_image[i][j]; 
      }
    }
    
    for (proc = 1; proc < size; proc++)
    {
      if (proc != 0)
      {
        MPI_Recv(&(recv_buf[0][0]), HEIGHT_P * WIDTH_P, MPI_REALNUMBER, proc, 0, cart_comm,
                 MPI_STATUS_IGNORE);
      }
      
      MPI_Cart_coords(cart_comm, proc, 2, proc_coord);
      for (i = 0; i < WIDTH_P; i++)
      {
        for (j = 0; j < HEIGHT_P; j++)
        {
           masterbuf[(proc_coord[0] * WIDTH_P) + i]
                    [(proc_coord[1] * HEIGHT_P) + (j )] = recv_buf[i][j]; 
        }
      }
    }
    pgmwrite(filename, masterbuf, WIDTH, HEIGHT);
    printf("finished gather\n");
   }
   return 0;
}

int mp_get_coords(MPI_Comm* cart_comm, int rank, int* coord)
{
  MPI_Cart_coords(*cart_comm, rank, 2, coord);
  return 0;
}

int mp_create_vector_type(MPI_Datatype *v_halo, MPI_Datatype *h_halo)
{

    MPI_Type_vector(1, HEIGHT_P, HEIGHT_P, MPI_REALNUMBER, v_halo);
    MPI_Type_vector(WIDTH_P, 1, HEIGHT_P+2, MPI_REALNUMBER, h_halo);
    MPI_Type_commit(v_halo);
    MPI_Type_commit(h_halo);
}


int mp_halo_swap(MPI_Comm cart_comm, MPI_Datatype *h_halo, MPI_Datatype *v_halo, 
                 real_number old[WIDTH_P+2][HEIGHT_P+2], 
                 int up, int down, int right, int left, int coords[2])
{    
    MPI_Status status;
    MPI_Request request1, request2, request3, request4;
    
    //printf("sending... %i %i\n", left, right);
    MPI_Issend(&old[WIDTH_P][1], 1, *v_halo, right, 1, cart_comm, &request1);
    MPI_Issend(&old[1][1], 1, *v_halo, left, 2, cart_comm, &request2);

    MPI_Issend(&old[1][1], 1, *h_halo, up, 3, cart_comm, &request3);
    MPI_Issend(&old[1][HEIGHT_P],1, *h_halo, down, 4, cart_comm, &request4);
 
 
    //printf("reveice... %i %i\n", left, right);
    MPI_Irecv(&old[0][1], 1, *v_halo, left, 1, cart_comm, &request1);
    MPI_Irecv(&old[WIDTH_P+1][1], 1, *v_halo, right, 2, cart_comm, &request2);
    
    MPI_Irecv(&old[1][HEIGHT_P+1], 1, *h_halo, down, 3, cart_comm, &request3);
    MPI_Irecv(&old[1][0], 1, *h_halo, up, 4, cart_comm, &request4);
    
    //Optionally perform calculations that do not depend on halos here.

    MPI_Wait(&request1, &status);
    MPI_Wait(&request2, &status);
    MPI_Wait(&request3, &status);
    MPI_Wait(&request4, &status);

    return 0;
}

real_number max_delta(MPI_Comm cart_comm, real_number old[WIDTH_P+2][HEIGHT_P+2], 
                     real_number new[WIDTH_P+2][HEIGHT_P+2])
{
    real_number delta,  local_max, global_max;
    int i,j;
    local_max =0;
    for (i = 0; i < HEIGHT_P+1; i++)
    {
        for(j = 0; j < HEIGHT_P+1; j++)
        {
            delta = old[i][j] - new[i][j];
            if (delta < 0)
            {
                delta = -delta;
            }

            if ( delta > local_max )
            {
                local_max= delta;
            }

        }
    }
    MPI_Allreduce(&local_max, &global_max, 1, MPI_REALNUMBER, MPI_MAX, cart_comm);
    return global_max;
}

