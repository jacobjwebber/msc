module CSRmatrix

  Implicit None

  ! basic module intoducing a CSR stored sparse matrix with simple routines 
  ! for creating and maniplating CSR formatted sparse matrices

  type CSR
     integer :: nrow      ! number of rows 
     integer :: ncol      ! number of columns
     integer :: nzmax     ! maximum number of non-zero-entires

     integer, allocatable, dimension(:) :: rowStart    ! row index info
     integer, allocatable, dimension(:) :: colIndex    ! col index info
     real(kind=8), allocatable, dimension(:) :: value  ! data entries    
  end type CSR



contains

  Subroutine Setup_CSR(M, r, c, nmax)

    ! setup work for CSR-matrix M: 
    ! put in the dimensions and allocate memory to store data

    type(CSR), intent(inout) :: M    ! matrix to be set up
    integer,  intent(in)     :: r    ! number of rows
    integer,  intent(in)     :: c    ! number of columns
    integer,  intent(in)     :: nmax ! maximum number of non-zero entries (capacity)

    M%nrow  = r
    M%ncol  = c
    M%nzmax = nmax

    allocate(M%rowStart( M%nrow + 1))
    allocate(M%colIndex( M%nzmax ))
    allocate(M%value( M%nzmax ))

    M%rowStart = 0
    M%colIndex = 0
    M%value    = 0.0d0
  end Subroutine Setup_CSR

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  Subroutine populate_CSR(M, rowvec, colvec, entries, n)
    ! populate the CSR-matrix M with data supplied in COO format
    type(CSR), intent(inout)               :: M       ! CSR matrix 
    integer, dimension(n), intent(in)      :: rowvec  ! vector row indices
    integer, dimension(n), intent(in)      :: colvec  ! vector column indices
    real(kind=8), dimension(n), intent(in) :: entries ! values nonzero entries
    integer, intent(in)                    :: n       ! number nonzero entries

    print *,"populating CSR matrix"
    print *,"Subroutine populate_CSR needs to be implemented"
    print *

  end Subroutine populate_CSR


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  Subroutine times_vector_CSR(M, x, y)
    ! This routine calculates y = M*x for CSR-matrix m and vectors x and y
    type(csr), intent(in)       :: M      
    real(kind=8), dimension(M%ncol), intent(in)   :: x
    real(kind=8), dimension(M%nrow), intent(out)  :: y

    print *
    print *,"Subroutine times_vector_CSR needs to be implemeted"
    print *

  end Subroutine times_vector_CSR

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  Subroutine lincg_CSR(M, b, x, tol, maxIter, usedIter)
    ! Conjugate Gradient solver to solve M.x =b for x
    ! M has to be symmetric and positive definite

    type(csr), intent(in) :: M
    real(kind=8), dimension(M%nrow), intent(in)    :: b
    real(kind=8), dimension(M%ncol), intent(inout) :: x

    real(kind=8), intent(in)  ::  tol       ! L2 convergence tolerance
    integer, intent(in)       ::  maxIter   ! maximum number of iterations

    integer, intent(out) :: usedIter  ! number of iterations used,
                                                ! -1 if no convergence

    print *
    print *, "Subroutine lincg_CSR needs implementing"
    print *

    usedIter = -1

  end Subroutine lincg_CSR


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  Subroutine print_CSR(M)
    ! print the contents of CSR-matrix M to screen
    type(csr), intent(in) :: M
    
    integer :: row
    integer :: idx

    real(kind=8), dimension(:), allocatable :: mline

    allocate(mline(M%ncol))

    print *, " Printing CSR-matrix:"
    print *


    do row = 1, M%nrow
       mline = 0.0
       do idx = M%rowStart(row), M%rowstart(row+1)-1
          mline(M%colIndex(idx)) = M%value(idx)
       end do
       print 101, mline
    end do

101 Format(10(x,F6.2))
    print *
  
  end Subroutine print_CSR

  
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

! Some helper functions
  Function norm(v)
    ! calculate the norm of v
    real(kind=8), dimension(:), intent(in) :: v
    real(kind=8) :: norm
    integer      :: i, leng

    norm = 0.0d0
    do i=1, size(v)
       norm = norm+ v(i)**2
    end do
    norm = sqrt(norm)
  end Function norm

  Function dot_prod(v,u)
    ! calculate scalar product of v and u
    real(kind=8), dimension(:), intent(in) :: v, u
    real(kind=8) :: dot_prod
    integer      :: i, leng
    
    dot_prod = 0.0d0
    do i= 1, size(v)
       dot_prod = dot_prod + v(i)*u(i)
    end do
  end Function dot_prod

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  Subroutine deallocate_CSR(M)
    type(CSR), intent(inout) :: M   ! matrix to be deallocated

    M%nrow  = 0
    M%ncol  = 0
    M%nzmax = 0

    deallocate(M%rowStart)
    deallocate(M%colIndex)
    deallocate(M%value)

  end Subroutine deallocate_CSR


end module CSRmatrix
