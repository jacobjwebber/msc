#include <cstdlib>
#include <cmath>

#include "mpi.h"

#include <iostream>

using namespace std;

#define N 840

int main ()
{
  double pi, exactpi;

  double tstart, tstop;

  int i;

  // MPI variables

  MPI::Status status;
  MPI::Intercomm comm;

  int rank, size, source, tag;

  // Other variables

  int istart, istop;
  double partialpi, recvpi;

  cout << "Computing approximation to pi using N = " << N << endl;

  // Initialise MPI and compute number of processes and local rank

  comm = MPI::COMM_WORLD;

  MPI::Init();

  rank = comm.Get_rank();
  size = comm.Get_size();

  cout << "Hello from rank " << rank << endl;

  // Now make sure output only comes from one process

  if (rank == 0) cout << "Running on " << size << " process(es)" << endl;

  tstart = MPI::Wtime();

  partialpi = 0.0;

  //
  // Compute an approximation to pi using a simple series expansion for pi/4
  // Ensure each process computes a separate section of the summation
  // NOTE: here I assume that N is exactly divisible by the number of processes
  //

  istart = N/size * rank + 1;
  istop  = istart + N/size - 1;

  cout << "On rank " << rank << " istart = " << istart
       << ", istop = " << istop << endl;

  for (i=istart; i<=istop; i++)
    {
      partialpi = partialpi +
	          1.0/( 1.0 + pow( (((double) i)-0.5)/((double) N), 2.0) );
    }

  cout << "On rank " << rank << " partialpi = " << partialpi << endl;

  //
  // Compute global value of pi by sending partial values to rank 0 
  // NOTE: this would be more efficiently done using MPI_REDUCE 
  //

  if (rank == 0)
    {
      // Initialise pi to locally computed parial sum

      pi = partialpi;

      // Add in contribution from other processes

      for (source = 1; source < size; source++)
	{
	  // receive partialpi from rank=source and place value in recvpi
	  // all messages are tagged as zero

	  tag = 0;

	  comm.Recv(&recvpi, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
			       tag, status);

	  cout << "rank 0 receiving from rank " << status.Get_source() << endl;

	  // add to running total

	  pi = pi + recvpi;
	}
    }
  else
    {
      // all other processes send their partial value to rank 0

      tag = 0;

      cout << "rank " << rank << " sending to rank 0" << endl;

      comm.Ssend(&partialpi, 1, MPI_DOUBLE, 0, tag);
    }

  pi = pi * 4.0/((double) N);

  exactpi = 4.0*atan(1.0);
  
  tstop = MPI::Wtime();

  if (rank == 0)
    {
      cout << "pi = " << pi
	   << ", % error = " << fabs(100.0*(pi-exactpi)/exactpi) << endl;

      cout << "Time taken was " << (tstop - tstart) << " seconds" << endl;
    }

  MPI::Finalize();

  return 0;
}
