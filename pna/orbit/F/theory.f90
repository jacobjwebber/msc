subroutine PlotTheoretical(unit)
    use init
    implicit none


    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !!
    !! This routine will plot the theoretical orbit trajectory to
    !! the file connected to unit. 
    !! 
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    double precision h2, Mtot, PI
    double precision thetabeg, thetaend, thetastep, theta, R
    integer unit, thetaloop, nplot


    ! how many points to plot for theoretical trajectory
    parameter (nplot = 500)


    ! set up PI
    PI=4.0*atan(1.0)


    ! calculate the initial angle from which to plot the trajectory.
    if(e.LT.1.0) then

        ! circular and elliptic orbits.
        thetabeg = -PI
        thetaend =  PI

    else if(e.EQ.1.0) then

        ! parabolic orbit.
        thetabeg = -PI+0.1
        thetaend =  PI-0.1

    else

        ! hyperbolic orbit.
        thetabeg = -PI + acos(1.0/e)+0.1
        thetaend =  PI - acos(1.0/e)-0.1

    endif


    ! calculate the starting position.
    Mtot = M1 + M2
    h2   = Rp*Mtot*(e + 1.0)
    R    = h2/(Mtot*(e*cos(thetabeg) + 1.0))


    ! calculate the remainder of the theoretical trajectory
    ! by looping round the angle variable.

    thetastep = (thetaend - thetabeg)/real(nplot-1)

    do thetaloop = 1, nplot
        theta = thetabeg + real(thetaloop-1)*thetastep
        R     = h2/(Mtot*(e*cos(theta) + 1.0))

        write(unit,100) real(R*cos(theta)), real(R*sin(theta))
    enddo

    write(unit,*) '&'

    100  format(f14.8,1x,f14.8)

end subroutine PlotTheoretical
