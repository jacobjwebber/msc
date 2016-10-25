program shmem

  implicit none

  include 'shmem.fh'

  integer :: nproc = 1, procnum = 0, i, nbuf, ierr

!
!  Declare the row and column arrays
! 
!  This is the Fortran 90 version:

  integer, allocatable, dimension(:) :: rowbuf, colbuf

!  However to use OpenSHMEM we must use Cray Pointers (to perform the 
!  shared memory allocate statement). Cray Pointers are like C pointers
!  for Fortran. You will need the following declaration instead:

!  integer, dimension(*) :: rowbuf, colbuf
!  pointer(ptr_rowbuf, rowbuf)
!  pointer(ptr_colbuf, colbuf)


!
!  Start up the parallel code and record nproc, the total number of CPUS,
!  and procnum, the identifier for this CPU in the range [0,nproc-1]
!

!  ...

  if (procnum .eq. 0) write(*,*) 'Running OpenSHMEM program on ', nproc, ' CPUs'

!
!  Size of buffers is equal to the number of processors
!

  nbuf = nproc

! You must replace these allocations with OpenSHMEM allocations,
! you will need to use the Cray pointer equivalents as arguments

  allocate(rowbuf(nbuf))
  allocate(colbuf(nbuf))

!
!  Initialise the row buffer appropriately on each CPU
!  and set the column buffer to -1
!

!  ...

!
!  Print out the initial values
!

  write(*,*) 'Before: procnum = ', procnum, &
              ', rowbuf = (', ((rowbuf(i), ', '), i=1,nbuf-1), rowbuf(nbuf), ')'

  write(*,*) 'Before: procnum = ', procnum, &
              ', colbuf = (', ((colbuf(i), ', '), i=1,nbuf-1), colbuf(nbuf), ')'

!
!  Now must publish relevant buffer and transfer data using puts or gets.
!  Remember to put synchronisation in the appropriate places
!

!  ...

!
!  Print out the final values
!

  write(*,*) 'After : procnum = ', procnum, &
             ', rowbuf = (', ((rowbuf(i), ', '), i=1,nbuf-1), rowbuf(nbuf), ')'



  write(*,*) 'After : procnum = ', procnum, &
              ', colbuf = (', ((colbuf(i), ', '), i=1,nbuf-1), colbuf(nbuf), ')'


!
!  Close down the parallel program
!

!  ...

end program shmem
