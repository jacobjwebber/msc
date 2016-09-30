#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int main(void)
{
  MPI_Init(NULL, NULL);
 
  int rank;
  int size_of_world;
  int namelen;
  char procname[MPI_MAX_PROCESSOR_NAME];

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size_of_world);
  MPI_Get_processor_name(procname, &namelen);

  printf("Hello World! I am rank %i out of %i and my name is %s\n", 
            rank, 
            size_of_world,
            procname);
  
  MPI_Finalize();
}
