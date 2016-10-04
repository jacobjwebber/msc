program FFT_example

  implicit none

  include 'fftw_f77.i'

! dimensions of square picture
  integer, parameter :: n = 320
  integer, parameter :: nx=n, ny=n

! working arrays
  real,    dimension(nx,ny) :: data
  complex, dimension(nx,ny) :: store

! 1D FFTW arrays
  complex, dimension(nx) :: in, out

! FFTW plans
  integer(kind=8) :: plan_in, plan_out

! loop variables
  integer :: i, j

! read in datafile
  call datread('../gibsonsq.dat', data, nx, ny)

! write PGM file
  call pgmwrite('beforeFFT.pgm', data, nx, ny, .FALSE., 255)

! create fftw plans
 
! copy real data in 'store' array

! perform forward FFT in 1st dimension

! perform forward FFT in 2nd dimension

! Alter data in Fourier space here

! cut off high frequencies

! or cut off low frequencies

! write the real and imaginary parts of the fourier array to files
! copy real and imaginary parts of the fourier array to files to
! array named data and write out the intermediate Fourier pgm files
! if the array store has been used, then the following 4 lines will work
  data=real(store)
  call pgmwrite('data_real.pgm', data, nx, ny, .TRUE., 2)
  data=imag(store)
  call pgmwrite('data_imaa.pgm', data, nx, ny, .TRUE., 2)

! perform backward FFT in 1st dimension 

! perform backward FFT in 2nd dimension

! copy real part into 'data' array and normalise

! write PGM file 
  call pgmwrite('afterFFT.pgm', data, nx, ny, .FALSE., 255)

! destroy FFT plans

end program FFT_example





subroutine datread(filename, x, nx, ny)

  implicit none

  character*(*) :: filename
  integer :: nx, ny, nxt, nyt
  real, dimension(nx, ny) :: x

  integer i, j

  integer, parameter :: iounit = 12

  open(unit=iounit, file=filename)

  read(iounit,*) nxt, nyt

  if (nx .ne. nxt .or. ny .ne. nyt) then
    write(*,*) 'datread: size mismatch, (nx,ny) = (', nxt, ',', nyt, &
               ') expected (', nx, ',', ny, ')'
    stop
  end if

  read(iounit,*) ((x(i,j), i=1,nx), j=1,ny)

  close(unit=iounit)

end subroutine datread







!  Routine to write a PGM image file from a 2D floating point array
!  x(nx,ny). Uses unit 10 for IO.

subroutine pgmwrite(filename, x, nx, ny, rescale, imax)

  implicit none

  character*(*) :: filename
  integer :: nx, ny
  logical :: rescale

  real,    dimension(nx, ny) :: x

  real,    dimension(nx, ny) :: tmp
  integer, dimension(nx, ny) :: grey

  real :: tmin, tmax
  real, parameter :: thresh = 255.0

  integer :: imax

  integer, parameter :: iounit = 10

  integer :: i, j

  tmp(:,:) = x(:,:)

!  Find the max and min absolute values of the array

  tmin = minval(abs(tmp(:,:)))
  tmax = maxval(abs(tmp(:,:)))

!  Scale the values appropriately so the lies between 0 and thresh

  if (rescale .and. (tmin .lt. 0 .or. tmax .gt. thresh)) then

   tmp(:,:) = int((thresh*((abs(tmp(:,:)-tmin))/(tmax-tmin))) + 0.5)

  else

   tmp(:,:) = int(abs(tmp(:,:)) + 0.5)

  end if

!  Increase the contrast by boosting the lower values
!  grey(:,:) = thresh * sqrt(tmp(:,:)/thresh)

  grey(:,:) = tmp(:,:)

  open(unit=iounit, file=filename)

  write(iounit,fmt='(''P2''/''# Written by pgmwrite'')')
  write(iounit,*) nx, ny
!  write(iounit,*) int(thresh)
  write(iounit,*) imax
  write(iounit,fmt='(17(i3,'' ''))') ((min(grey(i,j),imax), i=1,nx), j=1,ny)  

  close(unit=iounit)

end subroutine pgmwrite

