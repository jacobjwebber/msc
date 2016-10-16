program ex1b

  use pgmio

  implicit none

  integer, parameter :: nx = 360
  integer, parameter :: ny = 240

  integer, parameter :: numimage = 8

  integer, parameter :: nxlocal = nx
  integer, parameter :: nylocal = ny/numimage

  integer :: myimage

  double precision :: bigpic(nx, ny)

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

! In this incomplete template, just write picture out

  if (myimage == 1) then  

    call pgmwrite('output.pgm', bigpic)
  
  end if

end program ex1b
