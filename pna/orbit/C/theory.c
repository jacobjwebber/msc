/***********************************************************************
*   
*  theory.c: This routine will plot the theoretical orbit trajectory to
*            the file connected to the file descriptor fd. 
*     
***********************************************************************/

#include <stdio.h>
#include <math.h>

#define NPLOT (500)    /* number of points to plot for theoretical traj. */

/* global variables defined in init.c - explanation contained therein. */
extern	double	e, Rp, M1, M2, dR, tend, dt;
extern	int	nstep, outint;

void PlotTheoretical(FILE *fd)
{
      double 	h2,Mtot,PI;
      double 	thetabeg,thetaend,thetastep,theta,R;

      /* set up PI */
      PI=4.0*atan(1.0);

      /* calculate the initial angle from which to plot the trajectory. */
      if(e < 1.0){
         /* circular and elliptic orbits. */
         thetabeg = -PI;
         thetaend =  PI;
      }else if(e == 1.0){
         /* parabolic orbit. */
	         thetabeg = -PI+0.1;
	         thetaend =  PI-0.1;
      }else{
         /* hyperbolic orbit. */
         thetabeg = -PI + acos(1.0/e)+0.1;
         thetaend =  PI - acos(1.0/e)-0.1;
      }

      /* calculate the starting position. */
      Mtot     = M1 + M2;
      h2       = Rp*Mtot*(e + 1.0);
      R        = h2/(Mtot*(e*cos(thetabeg) + 1.0));

      /* calculate the remainder of the theoretical trajectory
         by looping round the angle variable. */

      thetastep = (thetaend - thetabeg)/(NPLOT-1.0);


      for(theta= thetabeg;theta <  thetaend;theta += thetastep){
          R = h2/(Mtot*(e*cos(theta) + 1.0));
          fprintf(fd,"%f %f\n",(R*cos(theta)),(R*sin(theta)));
      }

      fprintf(fd,"&\n");

}
