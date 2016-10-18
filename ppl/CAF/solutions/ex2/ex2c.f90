program ex2c

  use pgmio

  implicit none

  integer, parameter :: nx = 360
  integer, parameter :: ny = 240

  integer, parameter :: numimage = 8

  integer, parameter :: nxlocal = nx
  integer, parameter :: nylocal = ny/numimage
 
  integer, parameter :: maxloop = 100000

  integer :: ix, iy, iloop
  integer :: image, myimage, upimage, dnimage

  double precision :: globalsum
  double precision :: delta, deltasq
  double precision :: deltamin = 0.1

  double precision :: bigpic(nx, ny)

  double precision :: oldpic(0:nxlocal+1, 0:nylocal+1)[*]
  double precision :: newpic(0:nxlocal+1, 0:nylocal+1)[*]
  double precision ::   edge(0:nxlocal+1, 0:nylocal+1)[*]

  myimage = this_image()

  if (numimage /= num_images()) then

    if (myimage == 1) then
      write(*,*) 'Error: compiled for ', numimage, & 
                 ' image(s), but running on ', num_images()
    end if

    stop

  end if

  if (myimage == 1)  then

    write(*,*) 'Running on ', numimage, ' image(s)'
    write(*,*)
    write(*,*) 'Stopping when delta is less than ', deltamin
    write(*,*) 'Maximum number of iterations is  ', maxloop
    write(*,*)

  end if

  write(*,*) 'Hello from image ', myimage, ' out of ', numimage

  oldpic(:,:) = 0.0
  newpic(:,:) = 0.0
    edge(:,:) = 0.0

! Need to synchronise to ensure all images have finished initialising
! their local pictures before the master writes to them.

  sync all

! Read the image on the master and copy to oldpic

  if (myimage == 1) then

    call pgmread('edge360x240.pgm', bigpic)

    do image = 1, numimage

      edge  (1:nx, 1:nylocal)[image] = &
      bigpic(1:nx, (image-1)*nylocal+1:image*nylocal)

    end do

  end if

! Overwrite bigpic to ensure that we have to do the output correctly

  bigpic(:,:) = -1.0

! Need to synchronise to ensure the master has completed initialising
! edge on all images before any of the images try to read it.

  sync all

  oldpic(:,:) = edge(:,:)

! Compute indices of neighbouring images

  upimage = myimage + 1
  dnimage = myimage - 1

! Need to synchronise to ensure oldpic has been initialised on all images
! before any images try to access its halos on their neighbours

  sync all

  do iloop = 1, maxloop

! Do halo swap, taking care at the upper and lower picture boundaries

    if (upimage <= numimage) then

      oldpic(1:nxlocal, nylocal+1) = oldpic(1:nxlocal, 1)[upimage]

    end if

    if (dnimage >= 1) then

      oldpic(1:nxlocal, 0) = oldpic(1:nxlocal, nylocal)[dnimage]

    end if

! Now update the local values of newpic

    do ix = 1, nxlocal
      do iy = 1, nylocal


        newpic(ix, iy) = 0.25*(   oldpic(ix-1, iy) + oldpic(ix+1, iy) &
                                + oldpic(ix, iy-1) + oldpic(ix, iy+1) &
                                - edge(ix,iy)                           )

      end do
    end do

! Now compute the delta parameter locally

    deltasq = sum((newpic(1:nx, 1:nylocal)-oldpic(1:nx, 1:nylocal))**2)
    deltasq = deltasq / float(nx*ny)

! Sum up the values across images and take the square root

    deltasq = globalsum(deltasq)
    delta = sqrt(deltasq)

! Exit if convergence criterion is reached

    if (myimage == 1) write(*,*) 'Iteration ', iloop, ' delta = ', delta
    if (delta < deltamin) exit

! Need to synchronise to ensure that all images have finished reading the
! oldpic halo values on this image before overwriting it with newpic

    sync all

! Prepare for next iteration by setting oldpic = newpic

    oldpic(1:nxlocal,1:nylocal) = newpic(1:nxlocal,1:nylocal)

! Need to synchronise to ensure that all images have finished updating
! their oldpic arrays before this image reads any halo data from them at
! the start of the next iteration

    sync all

  end do

! Now write the output on the master image.
! No synchronisation required as due to sync all at the end of each loop

  if (myimage == 1) then  

! Copy newpic back to bigpic

    do image = 1, numimage

      bigpic(1:nx, (image-1)*nylocal+1:image*nylocal) = &
      newpic(1:nx, 1:nylocal)[image]

    end do

    call pgmwrite('output.pgm', bigpic)

  end if

end program ex2c


double precision function globalsum(dval)

  implicit none

  double precision :: dval
  double precision, save :: codval[*]

  integer :: image

  globalsum = 0.0

! Need to synchronise to ensure my codval value has been read
! by other images before it is reset

  sync all

  codval = dval

! Need to synchronise so coarray is set on all images before summation

  sync all

  do image = 1, num_images()

    globalsum = globalsum + codval[image]

  end do

end function globalsum
