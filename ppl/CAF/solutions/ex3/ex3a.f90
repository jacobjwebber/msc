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

! Need to synchronise to ensure all images have finished initialising
! their local pictures before the master writes to them.

  sync all

  if (myimage == 1) then

    call pgmread('edge360x240.pgm', bigpic)

    do image = 1, numimage

      edge(1:nxlocal, 1:nylocal)[imagecoordx(image), imagecoordy(image)] = &
        bigpic(imagestartx(image):imagestopx(image), &
               imagestarty(image):imagestopy(image)   )

    end do

  end if

! Need to synchronise to ensure the master has completed initialising
! edge on all images before any of the images try to read it.

  sync all

! initialise

  oldpic(1:nxlocal, 1:nylocal) = edge(1:nxlocal, 1:nylocal)

! Need to synchronise to ensure oldpic has been initialised on all images
! before any images try to access its halos on their neighbours

  sync all

  do iloop = 1, maxloop

! Do halo swap, taking care at the upper and lower picture boundaries in x and y

    if (upimagex <= numimagex) then

      oldpic(nxlocal+1, 1:nylocal) = &
      oldpic(1, 1:nylocal)[upimagex, myimagey]

    end if

    if (dnimagex >= 1) then

      oldpic(0, 1:nylocal) = &
      oldpic(nxlocal, 1:nylocal)[dnimagex, myimagey]

    end if

    if (upimagey <= numimagey) then

      oldpic(1:nxlocal, nylocal+1) = &
      oldpic(1:nxlocal, 1)[myimagex, upimagey]

    end if

    if (dnimagey >= 1) then

      oldpic(1:nxlocal, 0) = &
      oldpic(1:nxlocal, nylocal)[myimagex, dnimagey]

    end if

! Now update the local values of newpic (using array notation for compactness)

    newpic(1:nxlocal, 1:nylocal) = 0.25*(oldpic(0:nxlocal-1, 1:nylocal  ) +  &
                                         oldpic(2:nxlocal+1, 1:nylocal  ) +  &
                                         oldpic(1:nxlocal  , 0:nylocal-1) +  &
                                         oldpic(1:nxlocal  , 2:nylocal+1) -  &
                                         edge  (1:nxlocal  , 1:nylocal  )      )

! Need to synchronise to ensure that all images have finished reading the
! oldpic halo values on this image before overwriting it with newpic

    sync all

! Prepare for next iteration by setting oldpic = newpic

    oldpic(1:nxlocal, 1:nylocal) = newpic(1:nxlocal, 1:nylocal)

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

      bigpic(imagestartx(image):imagestopx(image),       &
             imagestarty(image):imagestopy(image)   )  = &
      oldpic(1:nxlocal,1:nylocal)[imagecoordx(image), imagecoordy(image)]

    end do

    call pgmwrite('output.pgm', bigpic)

  end if

end program ex3a
