program orbit
    use init
    implicit none


    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !! Program to plot the theoretical and integrated orbit of a particle
    !! output goes to an axx compatible file
    !! 
    !! INPUTS:  
    !! file  : "params.dat" orbit parmeters specified.
    !! 
    !! OUTPUTS: 
    !! file   : "trajectory.ax" theoretical and integrated trajectories.
    !! file   : "energy.ax" total energy.
    !! file   : "potential.ax" total potential energy.
    !! file   : "kinetic.ax" total kinetic energy.
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    ! various parameters
    double precision Mtot
    double precision h2
    double precision R, R2
    double precision theta
    double precision t
    double precision vtan, vrad

    integer count


    ! x and y components for the positions, velocity and accelerations.
    double precision x, y
    double precision vx, vy
    double precision ax, ay


    ! kinetic and potential energies
    double precision Ek, Ep


    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !! SETUP: the following lines of code calculate the initial 
    !! conditions for the system and check that the input parameters
    !! are self consistent. See START below for the actual start of the
    !! integration algorithm.
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    ! Read in initialisation parameters from a file.
    call InitParams


    ! Total mass.
    Mtot = M1 + M2


    ! check initial separation is not too small.
    if (dR.LT.Rp) then
        print *,'Initial separation ',dR,' smaller than pericntric'
        print *,'distance ',Rp,' exiting ...'

        stop
    endif


    ! Now check that for closed orbits the initial separation is not too big.
    if((e.LT.1).AND.(dR.GT.(Rp*(1.0+e)/(1.0-e)))) then
        print *,'Intitial separation ',dR,' is too large!'
        print *,'Maximum allowed value is ',Rp*(1.0+e)/(1.0-e)

        stop
    endif


    ! open the output files.
    open(20, file='trajectory.agr')
    open(30, file='energy.agr')
    open(40, file='potential.agr')
    open(50, file='kinetic.agr')


    ! Plot the theoretical orbit trajectory to unit 20.
    call PlotTheoretical(20)


    ! Calculate the intitial velocity and position of the
    ! particle whose orbit will be integrated.
    if((e.EQ.0.0).OR.(Rp.EQ.dR)) then
        theta = 0.0
    else
        theta = acos((Rp*(e+1.0)/dR -1.0)/e)
    endif

    h2 =  Rp*Mtot*(e + 1.0)
    R  =  h2/(Mtot*(e*cos(theta) + 1.0))
    x  =  R*cos(theta)
    y  = -R*sin(theta)


    ! Write initial positions to trajectory plot
    write(20,100) real(x), real(y)


    ! Calculate the initial velocity of the particle using 
    ! the tangential and radial velocity components.
    vtan = sqrt(Rp*(1.0 + e)*Mtot)/dR
    if(abs(Rp-dR).LT.0.00001) then
        vrad = 0.0
    else
        vrad = sqrt((e-1)*Mtot/Rp+2.0*Mtot/dR-vtan*vtan)
    endif

    vx = (vtan*sin(theta)-vrad*cos(theta))
    vy = (vtan*cos(theta)+vrad*sin(theta))

    t = 0.0


    ! Calculate and Output the Energy of the Particles.
    Ep = -M2*M1/R
    Ek =  0.5*M2*(vx*vx+vy*vy)

    write(30,100) real(t), real(Ep+Ek)
    write(40,100) real(t), real(Ep)
    write(50,100) real(t), real(Ek)


    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !! START: Integrate the particle trajectory.
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    do count = 1, nstep

        ! calculate the acceleration acting on the particle.
        R2 = x*x+y*y
        R  = sqrt(R2)
        ax = -M1*x/(R*R2) 
        ay = -M1*y/(R*R2) 


        ! update the time
        t = t + dt


        ! update the particle positions using the old velocities
        x = x+dt*vx
        y = y+dt*vy


        ! update the velocities.
        vx = vx+dt*ax
        vy = vy+dt*ay


        ! Output if we have evolved 'outint' time steps.
        if(MOD(count,outint).EQ.0) then
            write(20,100) real(x), real(y)


            ! Calculate and Output the Energy of the Particles.
            R  =  sqrt(x*x+y*y)
            Ep = -M1*M2/R
            Ek =  0.5*M2*(vx*vx+vy*vy)

            write(30,100) real(t), real(Ep+Ek)
            write(40,100) real(t), real(Ep)
            write(50,100) real(t), real(Ek)
        endif
    enddo


    ! Write a plus sign at the heavier particle location.
    write(20,*) '&'
    write(20,*) '@s2 symbol 1'
    write(20,*) '@s2 symbol color 1'
    write(20,*) '@s2 symbol fill color 1'
    write(20,*) '@s2 symbol fill pattern 1'
    write(20,100) real(0.0),real(0.0)


    ! Close file and exit.
    close(20)
    close(30)
    close(40)
    close(50)


    100  format(f14.8,1x,f14.8)
end program orbit
