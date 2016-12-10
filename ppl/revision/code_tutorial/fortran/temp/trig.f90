module trig
    implicit none

    real :: PI = 4.0 * atan(1.0);

    contains

    real function degtorad(degs)
        real :: degs
        real :: output
        degtorad = (degs/360) * 2 * PI
    end function degtorad

    real function radtodeg(rads)
        real :: rads
        real :: output
        radtodeg = (rads / (2*PI) ) * 360
    end function radtodeg

end module
