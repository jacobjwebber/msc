module init
    implicit none

    ! Exported params
    double precision e, Rp, M1, M2, dR, tend, dt
    integer nstep, outint


contains


    subroutine initParams

        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !! read initialisation parameters from an ASCII file provided by the 
        !! user.
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


        open(10, file='params.dat', status='OLD')

        ! eccentricity of the orbit.
        read(10,*) e

        ! pericentric distance (closest approach)
        read(10,*) Rp

        ! mass of object 1
        read(10,*) M1

        ! mass of object 2
        read(10,*) M2

        ! initial separation of the particles
        read(10,*) dR

        ! end time for the integration
        read(10,*) tend

        ! integration time step
        read(10,*) dt

        ! number of integration steps
        nstep = tend/dt

        ! set output interval so we do about 500 points per plot
        outint = (500+nstep-1)/500

        close(10)

    end subroutine initParams
end module init
