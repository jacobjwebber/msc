program ex3a

  use pgmio

  implicit none

  integer, parameter :: nx = 360
  integer, parameter :: ny = 240

  integer, parameter :: numimagex = 4
  integer, parameter :: numimagey = 2

  integer, parameter :: maxloop = 1000

  integer, parameter :: numimage  = numimagex*numimagey

  integer, parameter :: nxlocal = nx/numimagex
  integer, parameter :: nylocal = ny/numimagey

  integer :: ix, iy, iloop
  integer :: image, myimage, imagex, imagey

  integer :: myimagex, myimagey, upimagex, dnimagex, upimagey, dnimagey

  integer, dimension(numimage) :: imagestartx, imagestopx
  integer, dimension(numimage) :: imagestarty, imagestopy

  integer, dimension(numimage) :: imagecoordx, imagecoordy

  double precision :: bigpic(nx, ny)

  double precision :: oldpic(0:nxlocal+1, 0:nylocal+1)[numimagex,*]
  double precision :: newpic(0:nxlocal+1, 0:nylocal+1)[numimagex,*]
  double precision :: edge  (0:nxlocal+1, 0:nylocal+1)[numimagex,*]


  myimage = this_image()

  if (numimage /= num_images()) then

    if (myimage == 1) then
      write(*,*) 'Error: compiled for ', numimage, & 
                 ' image(s), but running on ', num_images()
    end if

    stop

  end if

! Initialise the lookup tables for image coordinates and start and stop
! x and y indices for each image

  do imagex = 1, numimagex
    do imagey = 1, numimagey

      image = image_index(oldpic, (/ imagex, imagey /))

      imagecoordx(image) = imagex
      imagecoordy(image) = imagey

      imagestartx(image) = (imagex-1)*nxlocal + 1
      imagestopx (image) = imagestartx(image) + nxlocal - 1

      imagestarty(image) = (imagey-1)*nylocal + 1
      imagestopy (image) = imagestarty(image) + nylocal - 1

    end do
  end do

! Store image indices of this image and its four neighbours

  myimagex = imagecoordx(myimage)
  myimagey = imagecoordy(myimage)

  upimagex = myimagex + 1
  dnimagex = myimagex - 1

  upimagey = myimagey + 1
  dnimagey = myimagey - 1

! Write out lookup tables for debugging purposes

  if (myimage == 1) then

    write(*,*) 'Running on ', numimage, ' image(s) in ', numimagex, &
                 ' x ', numimagey, ' grid'
    write(*,*)

    do image = 1, numimage

      write(*,*) 'Image ', image, ' is at location (',               &
                 imagecoordx(image), ', ', imagecoordy(image),     &
                 ', index range is (',                                 &
                 imagestartx(image), ':', imagestopx(image), ', ', &
                 imagestarty(image), ':', imagestopy(image), ')'

    end do

    write(*,*)
    write(*,*) 'Performing ', maxloop, ' iterations'
    write(*,*)

  end if

  oldpic(:,:) = 0.0
  newpic(:,:) = 0.0
  edge  (:,:) = 0.0

  if (myimage == 1) then

    call pgmread('edge360x240.pgm', bigpic)

    edge(1:nxlocal, 1:nylocal) = bigpic(1:nxlocal, 1:nylocal)

  end if

! Overwrite bigpic to ensure that we have to do the output correctly

  bigpic(:,:) = -1.0

! initialise

  oldpic(1:nxlocal, 1:nylocal) = edge(1:nxlocal, 1:nylocal)

  do iloop = 1, maxloop

! Do halo swap, taking care at the upper and lower picture boundaries in x and y

! ... <insert halo-swap code here> ...

! Now update the local values of newpic (using array notation for compactness)

    newpic(1:nxlocal, 1:nylocal) = 0.25*(oldpic(0:nxlocal-1, 1:nylocal  ) +  &
                                         oldpic(2:nxlocal+1, 1:nylocal  ) +  &
                                         oldpic(1:nxlocal  , 0:nylocal-1) +  &
                                         oldpic(1:nxlocal  , 2:nylocal+1) -  &
                                         edge  (1:nxlocal  , 1:nylocal  )      )

! Prepare for next iteration by setting oldpic = newpic

    oldpic(1:nxlocal, 1:nylocal) = newpic(1:nxlocal, 1:nylocal)

  end do

! Now write the output on the master image.

  if (myimage == 1) then  

    bigpic(1:nxlocal, 1:nylocal) = oldpic(1:nxlocal, 1:nylocal)

    call pgmwrite('output.pgm', bigpic)

  end if

end program ex3a
