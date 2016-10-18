program ex2a

  use pgmio

  implicit none

  integer, parameter :: nx = 360
  integer, parameter :: ny = 240

  integer, parameter :: numimage = 8

  integer, parameter :: nxlocal = nx
  integer, parameter :: nylocal = ny/numimage

  integer :: myimage

  double precision :: bigpic(nx, ny)

  double precision :: oldpic(0:nxlocal+1, 0:nylocal+1)[*]
  double precision :: newpic(0:nxlocal+1, 0:nylocal+1)[*]

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

  oldpic(:,:) = 0.0
  newpic(:,:) = 0.0 

! Read the image on the master and copy to oldpic

  if (myimage == 1) then

    call pgmread('picture360x240.pgm', bigpic)

! The following is only correct on a single image - you need to change this

    oldpic(1:nx, 1:nylocal) = bigpic(1:nx, 1:nylocal)

  end if

! Overwrite bigpic to ensure that we have to do the output correctly

  bigpic(:,:) = -1.0

! This is where we should do the actual calculation of newpic as the
! second derivative (ie the edges) of oldpic. In this template, simply
! set newpic = oldpic.

  newpic(1:nx, 1:nylocal) = oldpic(1:nx, 1:nylocal)

! Now write the output on the master image.

  if (myimage == 1) then  

! Copy newpic back to bigpic

    bigpic(1:nx, 1:nylocal) = newpic(1:nx, 1:nylocal)

    call pgmwrite('output.pgm', bigpic)

  end if

end program ex2a
