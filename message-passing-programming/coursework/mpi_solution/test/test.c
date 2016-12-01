#include <stdio.h>
#include <mpi.h>

#define NP 10
#define MP 10

#define MPI_REALNUMBER MPI_FLOAT
typedef float real_number;

int mp_halo_swap(MPI_Comm cart_comm, real_number old[MP+2][NP+2], 
                int north, int south, int east, int west, int coords[2]);

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

int main(int argc, char **argv)
{

    
	MPI_Init(&argc, &argv);
    MPI_Comm cart_comm, old_comm;
    int ndims, reorder, ierr, rank;
    int dim_size[2], periods[2];

    old_comm = MPI_COMM_WORLD;
    ndims = 2;
    dim_size[0] = 2; // replace with MPI_create_dims if poss
    dim_size[1] = 1;
    periods[0] = 1;
    periods[1] = 1;
    reorder = 1;

    ierr =
        MPI_Cart_create(old_comm, ndims, dim_size, periods, reorder, &cart_comm);

    MPI_Comm_rank(cart_comm, &rank);
    
    float array[MP+2][NP+2];
    int i, j;
    for (i=0; i < MP+2; i++)
    {
    	for (j= 0; j < NP+2; j++)
    	{
    		array[i][j] = rank;
    	}
    }

	if(rank==1)
	{
    	print_array(array, rank);
	}
	int coords[2];
	int north, south, east, west;

	north = mp_get_north(cart_comm, rank);
	south = mp_get_south(cart_comm, rank);
	east = mp_get_east(cart_comm, rank);
	west = mp_get_west(cart_comm, rank);

	
    MPI_Cart_coords(cart_comm, rank, 2, coords);
	
	mp_halo_swap(cart_comm, array, north,south,east,west, coords);

	if(rank==1)
	{
    	print_array(array, rank);
	}
    MPI_Finalize();
    return 0;
}


int mp_halo_swap(MPI_Comm cart_comm, real_number old[MP+2][NP+2], 
                int north, int south, int east, int west, int coords[2])
{    
    
    real_number recv_north_buf[NP], recv_south_buf[NP], send_north_buf[MP],
      send_south_buf[MP];
    
    int i,j;
    for (i = 0; i < MP; i++)
    {
      recv_north_buf[i] = 0;
      recv_south_buf[i] = 0;
      send_north_buf[i] = old[i + 1][1];
      send_south_buf[i] = old[i + 1][NP];
    }
 
    // Send edge halos
    if (coords[1] % 2 == 0)
    {
      MPI_Send(&old[1][1], NP, MPI_REALNUMBER, west, 0, cart_comm);
      MPI_Recv(&old[0][1], NP, MPI_REALNUMBER, west, 0, cart_comm,
               MPI_STATUS_IGNORE);

      MPI_Send(&old[MP][1], NP, MPI_REALNUMBER, east, 0, cart_comm);
      MPI_Recv(&old[MP + 1][1], NP, MPI_REALNUMBER, east, 0, cart_comm,
               MPI_STATUS_IGNORE);
    }
    else
    {
      MPI_Recv(&old[MP + 1][1], NP, MPI_REALNUMBER, east, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&old[MP][1], NP, MPI_REALNUMBER, east, 0, cart_comm);

      MPI_Recv(&old[0][1], NP, MPI_REALNUMBER, west, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&old[1][1], NP, MPI_REALNUMBER, west, 0, cart_comm);
    }

    if (coords[0] % 2 == 0)
    {
      MPI_Send(&(send_north_buf[0]), MP, MPI_REALNUMBER, north, 0, cart_comm);
      MPI_Recv(&(recv_north_buf[0]), MP, MPI_REALNUMBER, north, 0, cart_comm,
               MPI_STATUS_IGNORE);

      MPI_Send(&(send_south_buf[0]), MP, MPI_REALNUMBER, south, 0, cart_comm);
      MPI_Recv(&(recv_south_buf[0]), MP, MPI_REALNUMBER, south, 0, cart_comm,
               MPI_STATUS_IGNORE);
    }
    else
    {
      MPI_Recv(&(recv_south_buf[0]), MP, MPI_REALNUMBER, south, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&(send_south_buf[0]), MP, MPI_REALNUMBER, south, 0, cart_comm);

      MPI_Recv(&(recv_north_buf[0]), MP, MPI_REALNUMBER, north, 0, cart_comm,
               MPI_STATUS_IGNORE);
      MPI_Send(&(send_north_buf[0]), MP, MPI_REALNUMBER, north, 0, cart_comm);
    }

    for (i = 0; i < MP; i++)
    {
      old[i + 1][NP + 1] = recv_south_buf[i];
      old[i + 1][1] = recv_north_buf[i];
    }
    int rank;
    MPI_Comm_rank(cart_comm, &rank);
    //printf("rank %i [0][1] = %f", rank, old[0][1]);
    //printf("rank %i [MP][1] = %f", rank, old[MP][1]);
    return 0;
}


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
