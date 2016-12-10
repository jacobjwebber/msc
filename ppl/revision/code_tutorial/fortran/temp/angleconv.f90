program angleconv
    use trig
    implicit none
    real :: ang1, ang2, ang3
    
    write (*, *) "Give me three-o"
    
    read (*, *) ang1, ang2, ang3

    write (*, *) "You gave me ", ang1, ang2, ang3

    ang1 = degtorad(ang1)
    ang2 = degtorad(ang2)
    ang3 = degtorad(ang3)

    write (*, *) "In rads: ", ang1, ang2, ang3
    
    ang1 = radtodeg(ang1)
    ang2 = radtodeg(ang2)
    ang3 = radtodeg(ang3)

    write (*, *) "And back: ", ang1, ang2, ang3

end program
