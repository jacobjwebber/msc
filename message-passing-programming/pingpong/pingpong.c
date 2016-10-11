#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define ARRAY_SIZE 20
#define N 1000
int main(void)
{
  MPI_Init(NULL, NULL);
 
  int rank;
  int size_of_world;
  int namelen;
  char procname[MPI_MAX_PROCESSOR_NAME];
  double time;
  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size_of_world);
  MPI_Get_processor_name(procname, &namelen);

  MPI_Status status;

  printf("Hello World! I am rank %i out of %i and my name is %s\n", 
            rank, 
            size_of_world,
            procname);
 
  int array[ARRAY_SIZE] = { 0 };
  int i;
  time = MPI_Wtime();
  for (i = 0; i < N; i++)
  {
    if(rank == 0)
    {
        //send(array) to rank 1;
        MPI_Ssend(&array, ARRAY_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
        //receive(array) from rank 1;
        MPI_Recv(&array, ARRAY_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        
    }
    if(rank == 1)
    {
        //receive(array) from rank 0;
        MPI_Recv(&array, ARRAY_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        
        //send(array) to rank 0;
        MPI_Ssend(&array, ARRAY_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);        
     }
  }
  
  
  //calculate band width
  // total data = sizeof(array) * N * 2
  // total time = time
  time  = MPI_Wtime()- time;
  if (rank == 0)
  {
  float bandwidth = (sizeof(array) * N * 2.0) / time;

  printf("Total Bandwidth is %d bytes/s with a message size of %zu bytes\n", bandwidth, sizeof(array));
  }


  MPI_Finalize();
}
