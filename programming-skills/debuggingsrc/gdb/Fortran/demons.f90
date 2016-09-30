program demons

  implicit none

  integer, parameter :: ndim = 3
  integer, parameter :: npart = 125
  integer, parameter :: maxloop = 1000
  integer, parameter :: iounit = 10
	
  integer i, j, loop, idim, ipair, pointemp

  double precision, parameter :: lbox = 0.25
  double precision, parameter :: velmax = 0.1
  double precision, parameter :: dt = 0.005, dtsq = dt*dt

  double precision, dimension(ndim, npart) :: pos, pm1, pm2, vel
  double precision, dimension(ndim, npart) :: force

  double precision pe, ke, energy, e0, delta, sumpos

  logical, parameter :: periodic = .true.

  integer :: seed = 666


!  Initialise random initial positions and velocities

  call rinit(seed)

  call initpos(lbox,   pos, ndim, npart)

!  Add a constant offset

  do i = 1, npart

    pos(1,i) = pos(1,i) + 0.037
    pos(2,i) = pos(2,i) - 0.011
    pos(3,i) = pos(3,i) + 0.023

!    pos(1,i) = pos(1,i) + 0.0
!    pos(2,i) = pos(2,i) - 0.0
!    pos(3,i) = pos(3,i) + 0.0

  end do

!  Now bounce any particles off the walls

  do i = 1, npart
    do idim = 1, ndim
    
      if (pos(idim,i) .lt. 0.0) then
    
        if (periodic) then
    
          pos(idim,i) = pos(idim,i) + lbox
    
        else
    
          pos(idim,i) = -pos(idim,i)
    
        end if
    
      end if
    
      if (pos(idim,i) .ge. lbox) then
    
        if (periodic) then
    
          pos(idim,i) = pos(idim,i) - lbox
    
        else
    
          pos(idim,i) = 2.0*lbox - pos(idim,i)

        end if

      end if

    end do
  end do  

  call initvel(velmax, vel, ndim, npart)


!  Set the previous position at time t-1, pm1, appropriately

  do i = 1, npart
    do idim = 1, ndim
      
      pm1(idim,i) = pos(idim,i) - vel(idim,i)*dt

    end do
  end do

! Open file for writing PE

  open(unit=iounit, file='pe.ax')

  do loop = 1, maxloop

    write(*,*) 'Starting loop ', loop

!  Use sum of positions as a debug aid

    sumpos = 0.0

    do i = 1, npart
      do idim = 1, ndim
        sumpos = sumpos + pos(idim,i)
      end do
    end do

    write(*,*) 'loop ', loop, ', sumpos = ', sumpos

!  Compute total force on each particle (and PE at same time)

    call pairforce(periodic, lbox, force, pe, pos, ndim, npart)

  pointemp = 100 * 1000

  write(*,*) 'Force at 100,1000: ', force(pointemp,pointemp)


!  Update rule is x(t+1) = 2.x(t) - x(t-1) + dt*dt*f(t)
!  
!  This comes from: f = ma (assume mass=1) so f = a
!
!  acceleration = d2x/dt2 = (x(t+1) + x(t-1) - 2.x(t)) / (dt*dt)
!
!  ie f = (x(t+1) + x(t-1) - 2.x(t)) / (dt*dt)
!
!  rearrange so that x(t+1) is on LHS gives the above expression
!
!  note that below we are computing x(t) on the LHS so the eqn becomes
!
!  x(t) = 2.x(t-1) - x(t-2) + dt*dt*f(t-1)
!
!  ie pos = 2.pm1 - pm2 + dt*dt*f(pos)


!  First do the part that doesn't involve f

    do i = 1, npart
      do idim = 1, ndim

        pm2(idim,i) = pm1(idim,i)
        pm1(idim,i) = pos(idim,i)
        pos(idim,i) = 2.0*pos(idim,i) - pm2(idim,i) + force(idim,i)*dtsq

      end do
    end do

!  To calculate kinetic energy we need the velocity: KE = 1/2 m v*v
!  get this from v(t) = dx/dt = (x(t+1) - x(t-1))/(2.dt)

    ke = 0.0

    do i = 1, npart
      do idim = 1, ndim

        ke = ke + 0.5*((pos(idim,i) - pm2(idim,i))/(2.0*dt))**2

      end do
    end do

!  Sum the energies

    energy = ke + pe

!  Actually, we have computed the PE and KE for t-1 so subtract one from loop

    write(*,*) 'loop, pe, ke, energy ',loop-1, real(pe), real(ke), real(energy)
    write(iounit,*) loop, pe

!  Now bounce any particles off the walls

    do i = 1, npart
      do idim = 1, ndim

        if (pos(idim,i) .lt. 0.0) then

          if (periodic) then

            pos(idim,i) = pos(idim,i) + lbox
            pm1(idim,i) = pm1(idim,i) + lbox
            pm2(idim,i) = pm2(idim,i) + lbox

          else

            pos(idim,i) = -pos(idim,i)
            pm1(idim,i) = -pm1(idim,i)
            pm2(idim,i) = -pm2(idim,i)

          end if

        end if

        if (pos(idim,i) .ge. lbox) then

          if (periodic) then

            pos(idim,i) = pos(idim,i) - lbox
            pm1(idim,i) = pm1(idim,i) - lbox
            pm2(idim,i) = pm2(idim,i) - lbox

          else

            pos(idim,i) = 2.0*lbox - pos(idim,i)
            pm1(idim,i) = 2.0*lbox - pm1(idim,i)
            pm2(idim,i) = 2.0*lbox - pm2(idim,i)

          end if

        end if

      end do
    end do

  end do


  close(unit=iounit)

end program demons


subroutine pairforce(periodic, lbox, force, pe, pos, ndim, npart)

  implicit none

  integer ndim, npart

  double precision, dimension(ndim, npart) :: pos, force
  double precision, dimension(ndim) :: f
  double precision lbox, pe, pepair

  logical :: periodic

  integer ipair, i, j, idim

  ipair = 1   

  pe = 0.0

  force(:,:) = 0.0

!  Only loop over distinct pairs (i,j)

  do i = 1, npart-1
    do j = i+1, npart

      call fij(periodic, lbox, f, pepair, pos(1,i), pos(1,j), ndim)

      do idim = 1, ndim

        force(idim,i) = force(idim,i) - f(idim)
        force(idim,j) = force(idim,j) + f(idim)

      end do

      pe = pe + pepair

    end do
  end do

return
end



subroutine fij(periodic, lbox, f, pe, ipos, jpos, ndim)

!  The force on particle j due to particle i

  implicit none

  integer ndim, i

  double precision, dimension(ndim) :: f, ipos, jpos, r

  double precision :: pe, absr, lbox

  double precision, parameter :: diameter = 0.05, fmax   = 4.0

  logical :: periodic

!  Compute displacement absr between particles, as well as the vector
!  that joins them r(i)

  absr = 0.0

  do i = 1, ndim

    r(i) = jpos(i) - ipos(i)

! Fix up for periodic BCs

    if (periodic) then

! Is the distance round the back of the box shorter?

      if (r(i) .gt. 0.0) then

        if (abs(r(i) - lbox) .lt. abs(r(i)) ) then
          r(i) = r(i) - lbox
        end if

      else

        if (abs(r(i) + lbox) .lt. abs(r(i)) ) then
          r(i) = r(i) + lbox
        end if

      end if

    end if

    absr = absr + r(i)**2

  end do

  absr = sqrt(absr)

  if (absr .lt. diameter) then

!  Particles are touching so there is a repulsive force and associated
!  potential energy

    do i = 1, ndim
      f(i) = fmax*r(i)*(1.0/absr - 1.0/diameter)
    end do

    pe = fmax/(2.0*diameter) * (absr - diameter)**2

  else

!  Particles are not interacting so force and potential energy are zero

    do i = 1, ndim
      f(i) = 0.0
    end do

    pe = 0.0

  end if

  return
  end


  subroutine initpos(lbox, pos, ndim, npart)

  implicit none

  double precision lbox

  integer ndim, npart

  double precision pos(ndim, npart)

  integer i, idim


  call my_random_number(pos, ndim, npart)

!  Scale pos, currently in the range [0.0,1.0), to be [0.0, lbox)

  do i = 1, npart
    do idim = 1, ndim

      pos(idim,i) = pos(idim,i)*lbox

    end do
  end do

end subroutine initpos


subroutine initvel(velmax, vel, ndim, npart)

  implicit none

  double precision velmax

  integer ndim, npart

  double precision vel(ndim, npart)

  integer i, idim


  call my_random_number(vel, ndim, npart)

!  Scale vel, currently in the range [0.0,1.0), to be [-velmax, velmax)

  do i = 1, npart
    do idim = 1, ndim

      vel(idim,i) = velmax*(2.0*vel(idim,i)-1.0)

    end do
  end do

end subroutine initvel

subroutine my_random_number(x, nx, ny)

  implicit none

  integer :: nx, ny, i, j

  real :: uni

  double precision, dimension(nx, ny) :: x

  do j = 1, ny
    do i = 1, nx

      x(i,j) = uni()

    end do
  end do

end subroutine my_random_number
