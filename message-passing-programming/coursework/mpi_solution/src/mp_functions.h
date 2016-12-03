/*Functions for message passing*/
#include <mpi.h>

int mp_init(int *rank, int *size, MPI_Comm *cart_comm, int argc, char **argv);
int mp_get_coords(MPI_Comm* cart_comm, int rank, int* coord);

int mp_gather_and_write_png(MPI_Comm cart_comm, char* filename, 
                    real_number partial_image[WIDTH_P][HEIGHT_P], 
                    int size, int rank);

int mp_scatter(MPI_Comm cart_comm, char* filename, 
            real_number partial_image[WIDTH_P][HEIGHT_P], 
            int size, int rank);

int mp_get_left(MPI_Comm cart_comm, int my_rank);
int mp_get_right(MPI_Comm cart_comm, int my_rank);
int mp_get_down(MPI_Comm cart_comm, int my_rank);
int mp_get_up(MPI_Comm cart_comm, int my_rank);

int mp_create_vector_type(MPI_Datatype *h_halo, MPI_Datatype *v_halo);

int mp_halo_swap(MPI_Comm cart_comm, MPI_Datatype *h_halo, MPI_Datatype *v_halo,
                    real_number old[WIDTH_P+2][HEIGHT_P+2], int north, int south, 
                    int east, int west, int coords[2]);

real_number max_delta(MPI_Comm cart_comm, real_number old[WIDTH_P+2][HEIGHT_P+2], 
                      real_number new[WIDTH_P+2][HEIGHT_P+2]);
