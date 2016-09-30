/*
 * MPI Debug example program
 *
 * Adrian Jackson, EPCC, 2013
 */


#include <mpi.h>

#define P 8
#define M 192
#define N 360
#define Mp M/P
#define Np N
#define FALSE 0
#define TRUE 1

int main(int argc, char **argv){

  int i,j,k;
  int iterations = 100000;

  int size, rank;

  int count = Np * Mp;

  float old[Mp+2][Np+2], new[Mp+2][Np+2], edge[Mp+2][Np+2], buf[Mp][Np];
  float masterbuf[M][N];

  char in_filename[] = "edge192x360.dat";
  char out_filename[] = "halo_output.pgm";

  int ndims = 1;
  int dims[ndims];
  int periods[ndims];
  int reorder = FALSE;

  int direction = 0;
  int disp = 1;
  int rank_source, rank_sink;

  MPI_Comm mpi_cartesian;

  MPI_Status message_status;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  dims[0] = size;
  periods[0] = TRUE;

  MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &mpi_cartesian);

  MPI_Cart_shift(mpi_cartesian, direction, disp, &rank_source, &rank_sink);


  if(size != P){
    printf("Error: Number of Processors not same as declared in program.\n");
    MPI_Finalize();
    return 0;
  }

  if(rank == 0){
    printf("Starting program with %d processes\n",size);
    datread(in_filename, masterbuf, M, N);
  }

  MPI_Scatter(masterbuf, count, MPI_FLOAT, buf, count, MPI_FLOAT, 0, MPI_COMM_WORLD);
  
  for(i=0;i<Mp+2;i++){
    for(j=0;j<Np+2;j++){
      edge[i][j] = 0.0;
    }
  }

  for(i=1;i<Mp+1;i++){
    for(j=1;j<Np+1;j++){
      edge[i][j] = buf[i-1][j-1];
    }
  }

  for(i=0;i<Mp+2;i++){
    for(j=0;j<Np+2;j++){
      old[i][j] = edge[i][j];
    }
  }

  for(k=0;k<iterations;k++){

    MPI_Ssend(&old[Mp][1], N, MPI_FLOAT, rank_sink, 0, mpi_cartesian);
    MPI_Recv(&old[0][1], N, MPI_FLOAT, rank_source, 0, mpi_cartesian, &message_status);

    /*
    if(rank % 2 == 0){      
      MPI_Ssend(&old[Mp][1], N, MPI_FLOAT, rank_sink, 0, mpi_cartesian);
      MPI_Recv(&old[0][1], N, MPI_FLOAT, rank_source, 0, mpi_cartesian, &message_status);
    }else{
      MPI_Recv(&old[0][1], N, MPI_FLOAT, rank_source, 0, mpi_cartesian, &message_status);
      MPI_Ssend(&old[Mp][1], N, MPI_FLOAT, rank_sink, 0, mpi_cartesian);
    }
    */    

    MPI_Ssend(&old[1][1], N, MPI_FLOAT, rank_source, 0, mpi_cartesian);
    MPI_Recv(&old[Mp+1][1], N, MPI_FLOAT, rank_sink, 0, mpi_cartesian, &message_status);
	      
    /*
    if(rank % 2 == 0){
      MPI_Ssend(&old[1][1], N, MPI_FLOAT, rank_source, 0, mpi_cartesian);
      MPI_Recv(&old[Mp+1][1], N, MPI_FLOAT, rank_sink, 0, mpi_cartesian, &message_status);
    }else{
      MPI_Recv(&old[Mp+1][1], N, MPI_FLOAT, rank_sink, 0, mpi_cartesian, &message_status);
      MPI_Ssend(&old[1][1], N, MPI_FLOAT, rank_source, 0, mpi_cartesian);
    }
    */
      
    for(i=1;i<Mp+1;i++){
      for(j=1;j<Np+1;j++){
	new[i][j] = (0.25) * (old[i-1][j] + old[i+1][j] + old[i][j-1] + old[i][j+1] - edge[i][j]);
      }
    }
    
    for(i=0;i<Mp+2;i++){
      for(j=0;j<Np+2;j++){
	old[i][j] = new[i][j];
      }
    }    
  }


  for(i=1;i<Mp+1;i++){
    for(j=1;j<Np+1;j++){
      buf[i-1][j-1] = old[i][j];
    }
  }  

  MPI_Gather(buf, count, MPI_FLOAT, masterbuf, count, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if(rank == 0){
    pgmwrite(out_filename, masterbuf, M, N);
    printf("Finished program\n");
  }


  MPI_Finalize();

}
