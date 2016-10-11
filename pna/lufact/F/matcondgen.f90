subroutine matcondgen(cond,a,lda,n,b)

! Generates 'a' so that it has a given condition number
! Sets eigenvalues spaced linearly from 1.0/cond to 1.0

  implicit none

  real                    :: cond
  real, dimension(lda, n) :: a
  real, dimension(n)      :: b
  integer                 :: lda, n

  real, dimension(n,n)  :: t
  real                  :: tmp
  integer               :: i, j, info
  integer, dimension(n) :: ipiv

  real :: det

  a(:,:) = 0.0

  det = 1.0

  do i = 1, n
    a(i,i) = (1.0 + (cond-1.0)*float(i-1)/float(n-1))/cond
    det = det*a(i,i)
  end do

  call matgen(t,n,n,b)

! set a = a.t

  do i = 1, n
    tmp = a(i,i)
    do j = 1, n
      a(i,j) = tmp*t(i,j)
    end do
  end do

! now set a = t^-1. a, ie solve t.a = a

  call sgetrf(n, n, t, n, ipiv, info)
  call sgetrs('N', n, n, t, n, ipiv, a, lda, info)

! set b appropriately so solution is x(i) = 1.0 for all i

  b(:) = sum(a, dim=2)

end subroutine matcondgen
