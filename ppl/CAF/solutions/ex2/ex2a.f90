program ex2a

  use pgmio

  implicit none

  integer, parameter :: nx = 360
  integer, parameter :: ny = 240

  integer, parameter :: numimage = 8

  integer, parameter :: nxlocal = nx
  integer, parameter :: nylocal = ny/numimage

  integer :: ix, iy
  integer :: image, myimage, upimage, dnimage

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

! Need to synchronise to ensure all images have finished initialising
! their local pictures before the master writes to them.

  sync all

! Read the image on the master and copy to oldpic

  if (myimage == 1) then

    call pgmread('picture360x240.pgm', bigpic)

    do image = 1, numimage

      oldpic(1:nx, 1:nylocal)[image] = &
      bigpic(1:nx, (image-1)*nylocal+1:image*nylocal)

    end do

  end if

! Overwrite bigpic to ensure that we have to do the output correctly

  bigpic(:,:) = -1.0

! Need to synchronise to ensure the master has completed initialising
! oldpic on all images before any of the images try to read the halos.

  sync all

! Compute indices of neighbouring images

  upimage = myimage + 1
  dnimage = myimage - 1

! Do halo swap, taking care at the upper and lower picture boundaries

  if (upimage <= numimage) then

    oldpic(1:nxlocal, nylocal+1) = oldpic(1:nxlocal, 1)[upimage]

  end if

  if (dnimage >= 1) then

    oldpic(1:nxlocal, 0) = oldpic(1:nxlocal, nylocal)[dnimage]

  end if

! Now compute the edges and place the answer in newpic

  do ix = 1, nxlocal
    do iy = 1, nylocal

      newpic(ix, iy) =   oldpic(ix-1, iy) + oldpic(ix+1, iy) &
                       + oldpic(ix, iy-1) + oldpic(ix, iy+1) &
                       - 4.0 * oldpic(ix,iy)

    end do
  end do

! Now write the output on the master image.
!
! Need to synchronise to ensure calculation of newpic is complete on
! all images before the master reads the values.

  sync all

  if (myimage == 1) then  

! Copy newpic back to bigpic

    do image = 1, numimage

      bigpic(1:nx, (image-1)*nylocal+1:image*nylocal) = &
      newpic(1:nx, 1:nylocal)[image]

    end do

    call pgmwrite('output.pgm', bigpic)

  end if

end program ex2a
