program ex1b

  use pgmio

  implicit none

  integer, parameter :: nx = 360
  integer, parameter :: ny = 240

  integer, parameter :: numimage = 8

  integer, parameter :: nxlocal = nx
  integer, parameter :: nylocal = ny/numimage

  integer :: image, myimage
  integer :: ix, iy

  double precision :: bigpic(nx, ny)
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

! Need to synchronise to ensure smallpic is initialised on all images
! before the master image reads them.

  sync all

  if (myimage == 1) then  

    do image = 1, numimage

      do ix = 1, nxlocal
        do iy = 1, nylocal

          bigpic(ix, (image-1)*nylocal + iy) = smallpic(ix,iy)[image]

        end do

      end do

    end do

    call pgmwrite('output.pgm', bigpic)
  
  end if

end program ex1b
