/*Functions for message passing*/
#include <mpi.h>

float** return_masterbuf();
int mp_get_coords(MPI_Comm* cart_comm, int rank, int* coord);
int mp_gather_and_write_png(float** partial_image, MPI_Comm* cart_comm, char* filename, int rank, int size);
int mp_get_west(MPI_Comm cart_comm, int my_rank);
int mp_get_east(MPI_Comm cart_comm, int my_rank);
int mp_get_south(MPI_Comm cart_comm, int my_rank);
int mp_get_north(MPI_Comm cart_comm, int my_rank);
int mp_init(int *rank, int *size, MPI_Comm *cart_comm, int argc, char **argv);
