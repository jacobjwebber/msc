program lufact

  implicit none

  integer, parameter :: nmax = 100

  real, dimension(nmax,nmax) :: a, l, u, lu
  real, dimension(nmax)   :: x, b, ax

  integer :: i, j

  integer :: n = 4

  call matgen(a,nmax,n,b)

  write(*,*)
  write(*,*) 'a'
  write(*,*)
  call matprint(a, nmax, n)

  write(*,*)
  write(*,*) 'b'
  write(*,*)
  call vecprint(b,n)

  write(*,*)
  write(*,*) 'Need to perform LU factorisation of A'
  write(*,*) 'Currently setting L=U=0'

!
! Set l and u to zero
!

  do i = 1, n
    do j = 1, n
      l(i,j) = 0.0
      u(i,j) = 0.0
    end do
  end do

  write(*,*)
  write(*,*) 'l'
  write(*,*)
  call matprint(l, nmax, n)


  write(*,*)
  write(*,*) 'u'
  write(*,*)
  call matprint(u, nmax, n)

!
!  Multiply the L and U factors together
!

  call matmat(lu, l, u, nmax, n)

  write(*,*)
  write(*,*) 'lu'
  write(*,*)
  call matprint(lu, nmax, n)


  write(*,*)
  write(*,*) 'Need to forward and back substitution to solve Ax=b'
  write(*,*) 'Currently setting x=b'

  do i = 1, n
    x(i) = b(i)
  end do


  write(*,*)
  write(*,*) 'x'
  write(*,*)
  call vecprint(x,n)

!
! Set y = A x and print out
!

  call matvec(ax, a, nmax, x, n)

  write(*,*)
  write(*,*) 'Ax'
  write(*,*)
  call vecprint(ax,n)

end program lufact


!  matprint
!
!  Prints a matrix of reals to the screen. Matrix is taken to be
!  of size n x n although it must have been defined with a leading
!  dimension of lda.


subroutine matprint(a, lda, n)

  implicit none

  integer :: lda, n, i, j

  real, dimension(lda,n) :: a

  if (n .gt. 6) then
    write(*,*) 'Matrix too large to print'
  else

    do i = 1, n
      write(*,10) (a(i,j), j=1,n)
10    format(10(1x, f12.8))
    end do

  end if

end subroutine matprint


!  vecprint
!
!  Prints a vector containing n reals to the screen.

subroutine vecprint(x, n)

  implicit none

  integer :: n, i

  real, dimension(n) :: x

  if (n .gt. 6) then
    write(*,*) 'Vector too large to print'
  else

    do i = 1, n
      write(*,*) x(i)
10    format(1x, f12.8)
    end do

  end if

end subroutine vecprint

!  matvec
!
!  Sets y = a.x, where y and x are vectors of reals of length n, and a
!  is an n x n matrix. Matrix must have been defined with a leading
!  dimension of lda.


subroutine matvec(y, a, lda, x, n)

  implicit none

  integer :: lda, n, i, j

  real, dimension(n)     :: x, y
  real, dimension(lda,n) :: a

  do i = 1, n
    y(i) = 0.0

    do j = 1, n
      y(i) = y(i) + a(i,j)*x(j)
    end do

  end do

end subroutine matvec


!  matmat
!
!  Sets a = b.c, where a, b and c are all n x n matrices of reals.
!  All matrices must have been defined with a leading dimension lda.

subroutine matmat(a, b, c, lda, n)

  implicit none

  integer :: lda, n, j

  real, dimension(lda,n) :: a, b, c

  do j = 1, n

    call matvec(a(1,j), b, lda, c(1,j), n)

  end do

end subroutine matmat


!  matgen
!
!  Constructs a random matrix of size n x n, and a solution vector b
!  Matrix must have been defined with a leading dimension lda.


subroutine matgen(a,lda,n,b)

  implicit none

  integer                :: lda, n
  real, dimension(n)     :: b
  real, dimension(lda,n) :: a

  integer :: i, j, init

  init = 1325;

  do j = 1, n
    do i = 1, n

       init = mod(3125*init, 65536)
       a(i,j) = (init - 32768.0)/16384.0

    end do
  end do

  b(:) = sum(a, dim=2)

end subroutine matgen
