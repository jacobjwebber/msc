program ex1c

  use pgmio

  implicit none

  integer, parameter :: nx = 360
  integer, parameter :: ny = 240

  integer, parameter :: numimage = 8

  integer, parameter :: nxlocal = nx
  integer, parameter :: nylocal = ny/numimage

  integer :: image, myimage
  integer :: ix, iy

  double precision :: bigpic(nx, ny)[*]
  double precision :: smallpic(nxlocal, nylocal)[*]

  myimage = this_image()

  if (numimage /= num_images()) then

    if (myimage == 1) then
      write(*,*) 'Error: compiled for ', numimage, & 
                 ' image(s), but running on ', num_images()
    end if

    stop

  end if

  if (myimage == 1)  write(*,*) 'Running on ', numimage, ' image(s)'

  write(*,*) 'Hello from image ', myimage, ' out of ', numimage

  bigpic(:,:) = -1.0

  smallpic(:,:) = myimage

! Need to synchronise to ensure bigpic is initialised on the master
! before any of the other images write to it.

  sync all

  do ix = 1, nxlocal
    do iy = 1, nylocal
  
      bigpic(ix, (myimage-1)*nylocal + iy)[1] = smallpic(ix,iy)

    end do
  end do

! Need to synchronise to ensure all images have finished writing
! to bigpic on the master before the master saves it.

  sync all

  if (myimage == 1) call pgmwrite('output.pgm', bigpic)
  
end program ex1c
