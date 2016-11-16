Module COOmatrix

  Implicit None

  ! this is a small module introducting a COO stored sparse matrix

  type COO
     integer :: nrow      ! number of rows 
     integer :: ncol      ! number of columns
     integer :: nnz       ! number of non-zero-entires

     integer, allocatable, dimension(:) :: rowIndex    ! row index info
     integer, allocatable, dimension(:) :: colIndex    ! col index info
     real(kind=8), allocatable, dimension(:) :: value  ! date entries    
  end type COO

contains

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  Subroutine allocate_COO(A,n)
    type(COO), intent(inout) :: A
    integer, intent(in)            :: n

    A%nnz = n
    allocate(A%rowIndex(n))
    allocate(A%colIndex(n))
    allocate(A%value(n))
  end Subroutine allocate_COO
  
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  Subroutine deallocate_COO(A)
    type(COO), intent(inout) :: A

    A%nnz = 0
    deallocate(A%rowIndex)
    deallocate(A%colIndex)
    deallocate(A%value)
  end Subroutine deallocate_COO


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  Subroutine print_COO(A)
    type(COO), intent(in) :: A

    integer :: row  ! current row
    integer :: col  ! current column
    integer :: idx  ! current data element
    
    real(kind=8), dimension(:), allocatable :: mline 

    idx = 1
    allocate(mline(A%ncol))

    print *," Printing COO-matrix:"
    print *

    do row = 1, A%nrow
       do col = 1, A%ncol
          if ( row.eq.A%rowIndex(idx) .and. col.eq.A%colIndex(idx) ) then
             mline(col) = A%value(idx)
             idx = idx + 1
          else
             mline(col) = 0.0
          endif
       end do       
       print 101, mline
    end do
    
101 Format(10(x,F6.2))
 
    print *
   
    deallocate(mline)
 
 end Subroutine print_COO

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

! Subroutine read_COO(fnum,A)
 Subroutine read_COO(fname ,A)
!   integer, intent(in)      :: fnum
   character*80, intent(in)    :: fname
   type(COO), intent(inout) :: A
   ! read matrix A from unit fnum

   integer :: rdrow, rdcol
   real(kind=8) :: rdval
   integer :: idx
   integer :: fnum

   fnum=15
   
   open(fnum,file=fname)

   read (fnum,*) A%nrow, A%ncol, A%nnz
   read (fnum,*)

   allocate(A%rowIndex(A%nnz))
   allocate(A%colIndex(A%nnz))
   allocate(A%value(A%nnz))
   

   do idx = 1, A%nnz
      read (fnum,*)  A%rowIndex(idx), A%colIndex(idx), A%value(idx)  
   enddo
   
   close(fnum)

 end Subroutine read_COO



end module COOmatrix
