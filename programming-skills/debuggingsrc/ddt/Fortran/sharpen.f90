! MPI Debug example program
!
! Adrian Jackson, EPCC, 2013

program sharpen

  use mpi

  implicit none

  integer :: i,j,k
  integer, parameter :: iterations = 100000
  integer, parameter :: P = 8
  integer, parameter :: M = 192
  integer, parameter :: N = 360
  integer, parameter :: Mp = M/P
  integer, parameter :: Np = N
  integer, parameter :: FALSE = 0
  integer, parameter :: TRUE = 1

  integer :: comm, size, rank, ierr
  integer :: nx, ny, xpix, ypix
  integer :: count = Np * Mp

  integer, parameter :: direction = 0
  integer, parameter :: disp = 1
  integer, parameter :: ndims = 1
  integer, dimension(ndims) :: dims, periods
  integer :: reorder

  integer :: nn, nm
  
  integer :: mpi_cartesian
  integer :: rank_source, rank_sink
  integer,dimension(MPI_STATUS_SIZE) :: message_status

  double precision, dimension(Np+2,Mp+2) :: old, new, edge
  double precision, dimension(Np,Mp) :: buf
  double precision, dimension(N,M) :: masterbuf

  integer, parameter :: maxlen = 32
  character*(maxlen) :: in_filename, out_filename

  comm = MPI_COMM_WORLD

  call MPI_Init(ierr)

  call MPI_Comm_size(comm, size, ierr)
  call MPI_Comm_rank(comm, rank, ierr)

  reorder = FALSE
  dims(1) = size
  periods(1) = TRUE

  in_filename = 'edge192x360.dat'
  out_filename = 'halo_output.pgm'

  call MPI_Cart_create(comm, ndims, dims, periods, reorder, mpi_cartesian, ierr)
  call MPI_Cart_shift(mpi_cartesian, direction, disp, rank_source, rank_sink, ierr)

  if(size .ne. P) then
     write(*,*) 'Error: Number of Processors not the same as declared in the program.\n'
     call MPI_Finalize(ierr)
     stop

  end if

  if(rank .eq. 0) then
     write(*,*) 'Starting the program with ',size,' processes'
     call pgmread(in_filename, masterbuf, N, M, nn, nm) 
  end if

  call MPI_Scatter(masterbuf, count, MPI_DOUBLE_PRECISION, buf, count, MPI_DOUBlE_PRECISION, 0, comm, ierr)

  edge = 0.0


  edge(2:NP+1,2:MP+1) = buf(1:NP,1:MP)

  old = edge

  do k=0,iterations
     
     call MPI_Ssend(old(2,Mp+1), N, MPI_DOUBLE_PRECISION, rank_sink, 0, mpi_cartesian, ierr)
     call MPI_Recv(old(2,1), N, MPI_DOUBLE_PRECISION, rank_source, 0, mpi_cartesian, message_status, ierr)

!     if(mod(rank,2) .eq. 0) then
!        call MPI_Ssend(old(2,Mp+1), N, MPI_DOUBLE_PRECISION, rank_sink, 0, mpi_cartesian, ierr)
!        call MPI_Recv(old(2,1), N, MPI_DOUBLE_PRECISION, rank_source, 0, mpi_cartesian, message_status, ierr)        
!     else
!        call MPI_Recv(old(2,1), N, MPI_DOUBLE_PRECISION, rank_source, 0, mpi_cartesian, message_status, ierr)        
!        call MPI_Ssend(old(2,Mp+1), N, MPI_DOUBLE_PRECISION, rank_sink, 0, mpi_cartesian, ierr)
!     end if

     call MPI_Ssend(old(2,2), N, MPI_DOUBLE_PRECISION, rank_sink, 0, mpi_cartesian, ierr)
     call MPI_Recv(old(2,Mp+2), N, MPI_DOUBLE_PRECISION, rank_source, 0, mpi_cartesian, message_status, ierr)

!     if(mod(rank,2) .eq. 0) then
!        call MPI_Ssend(old(2,2), N, MPI_DOUBLE_PRECISION, rank_sink, 0, mpi_cartesian, ierr)
!        call MPI_Recv(old(2,Mp+2), N, MPI_DOUBLE_PRECISION, rank_source, 0, mpi_cartesian, message_status, ierr)
!     else
!        call MPI_Recv(old(2,Mp+2), N, MPI_DOUBLE_PRECISION, rank_source, 0, mpi_cartesian, message_status, ierr)
!        call MPI_Ssend(old(2,2), N, MPI_DOUBLE_PRECISION, rank_sink, 0, mpi_cartesian, ierr)
!     end if

     do j=2,Mp+1
        do i=2,Np+1  
           new(i,j) = (0.25) * (old(i-1,j) + old(i+1,j) + old(i,j-1) + old(i,j+1) - edge(i,j))
        end do
     end do

     old = new

  end do

  buf(1:NP,1:MP) = old(2:NP+1,2:MP+1)

  call MPI_Gather(buf, count, MPI_DOUBLE_PRECISION, masterbuf, count, MPI_DOUBLE_PRECISION, 0, comm, ierr)

  if(rank .eq. 0) then
     call pgmwrite(out_filename, masterbuf, M, N)
     write(*,*) 'Finished the program'
  end if

  call MPI_Finalize(ierr)

end program sharpen
