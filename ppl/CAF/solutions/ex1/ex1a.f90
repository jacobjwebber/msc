program ex1a

  implicit none

  integer :: myimage, numimage

  numimage = num_images()
  myimage  = this_image()

  if (myimage == 1) write(*,*) 'Running on ', numimage, ' image(s)'

  write(*,*) 'Hello from image ', myimage, ' out of ', numimage

end program ex1a
