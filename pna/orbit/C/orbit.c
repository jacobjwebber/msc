/***********************************************************************
*     orbit.c: Program to plot the theoretical and integrated orbit of a 
*     particle. Output goes to an agr compatible file.
*
*     INPUTS:  
*     file  : "params.dat" orbit parmeters specified.
*
*     OUTPUTS: 
*     file   : "trajectory.agr" theoretical and integrated trajectories.
*     file   : "energy.agr" total energy.
*     file   : "potential.agr" total potential energy.
*     file   : "kinetic.agr" total kinetic energy.
***********************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* global variables defined in init.c - explanation contained therein. */
extern double 	e, Rp, M1, M2, dR, tend, dt; 
extern int	nstep, outint;

main(int argc,char **argv)
{
      double 	Mtot;	        /* total particle mass */
      double 	h2;		/* angular momentum, eccentricty */
      double 	R, R2;             /* particle separations */
      double 	theta;             /* angle */
      double 	t;                 /* time variables */
      double 	vtan, vrad;        /* tangential and radial velocities */
      double 	x, y;        /* particle positions */
      double 	vx, vy;      /* particle velocities */
      double 	ax, ay;      /* particle accelerations */
      double 	Ek, Ep;	  /* kinetic and potential energies */

      int    	count;             /* counters */

      FILE   	*fd_traj,*fd_pot,*fd_ene,*fd_ke;
      void   	InitParams(void );
      void	PlotTheoretical(FILE *);


/**********************************************************************
*     SETUP: the following lines of code calculate the initial 
*     conditions for the system and check that the input parameters
*     are self consistent. See START below for the actual start of the
*     integration algorithm.
***********************************************************************/

     /* Read in initialisation parameters from a file. */

      InitParams();

      Mtot = M1 + M2;  /* Total Mass */

     /* check initial separation is not too small. */

      if (dR < Rp){
	 fprintf(stderr,"Initial separation %f smaller than pericntric",dR);
         fprintf(stderr," distance %f exiting ...\n",Rp);
         exit(1);
      }

   /* Now check that for closed orbits the initial separation is not too big.*/

      if((e < 1)&&(dR > (Rp*(1.0+e)/(1.0-e)))){
         fprintf(stderr,"Intitial separation %f is too large!\n",dR);
         fprintf(stderr,"Maximum allowed value is  %f.\n",Rp*(1.0+e)/(1.0-e));
         exit(1);
      }

    /* open the output files. */

      fd_traj = fopen("trajectory.agr","w");
      if(fd_traj == NULL){
            fprintf(stderr,"Could not open trajectory output file.\n");
            exit(1);
      }
      fd_ene  = fopen("energy.agr","w");
      if(fd_ene == NULL){
            fprintf(stderr,"Could not open energy output file.\n");
            exit(1);
      }
      fd_pot  = fopen("potential.agr","w");
      if(fd_pot == NULL){
            fprintf(stderr,"Could not open potential energy output file.\n");
            exit(1);
      }
      fd_ke   = fopen("kinetic.agr","w");
      if(fd_ke == NULL){
            fprintf(stderr,"Could not open kinetic energy output file.\n");
            exit(1);
      }

     /* Plot the theoretical orbit trajectory. */
      PlotTheoretical(fd_traj);

    /*  Calculate the intitial velocity and position of the
        particle whose orbit will be integrated. */

      if((e == 0.0)||(Rp == dR)){
         theta = 0.0;
      }else{
         theta = acos((Rp*(e+1.0)/dR -1.0)/e);
      }

      h2 =  Rp*Mtot*(e + 1.0);
      R  =  h2/(Mtot*(e*cos(theta) + 1.0));
      x  =  R*cos(theta);
      y  = -R*sin(theta);

      /* Write initial positions to trajectory plot. */
      fprintf(fd_traj,"%f %f\n", x, y);

      /* Calculate the initial velocity of the particle using 
         the tangential and radial velocity components. */

      vtan     =   sqrt(Rp*(1.0 + e)*Mtot)/dR;
      if(fabs(Rp-dR) < 0.00001){
          vrad = 0.0;
      }else{
          vrad = sqrt((e-1)*Mtot/Rp+2.0*Mtot/dR-vtan*vtan);
      }

      vx =   (vtan*sin(theta)-vrad*cos(theta));
      vy =   (vtan*cos(theta)+vrad*sin(theta));
      t = 0.0;

      /* Calculate and Output the Energy of the Particles. */

      Ep       = -M2*M1/R;
      Ek       =  0.5*M2*(vx*vx+vy*vy);

      fprintf(fd_ene,"%f %f\n",t,(Ep+Ek));
      fprintf(fd_pot,"%f %f\n",t,Ep);
      fprintf(fd_ke,"%f %f\n",t,Ek);

/**********************************************
*     START: Integrate the particle trajectory.
**********************************************/

      for(count=0;count<nstep;count++){
         /* calculate the acceleration acting on the particle. */
         R2  =  x*x+y*y;
         R   =  sqrt(R2);
         ax  =  -M1*x/(R*R2);
         ay  =  -M1*y/(R*R2);

         /* update the time. */
         t = t + dt;

         /* update the particle positions using the old velocities. */
         x   =  x+dt*vx;
         y   =  y+dt*vy;

         /* update the velocities. */
         vx  =  vx+dt*ax;
         vy  =  vy+dt*ay;

         /* Output if we have evolved 'outint' time steps. */
         if(!(count%outint)){ 
 	  fprintf(fd_traj,"%f %f\n",x,y);


         /*  Calculate and Output the Energy of the Particles. */
           R        =  sqrt(x*x+y*y);
           Ep       = -M1*M2/R;
           Ek       =  0.5*M2*(vx*vx+vy*vy);

           fprintf(fd_ene,"%f %f\n",t,(Ep+Ek));
           fprintf(fd_pot,"%f %f\n",t,Ep);
           fprintf(fd_ke,"%f %f\n",t,Ek);

         }

      }

      /* plot a plus sign where the stationary body lies. */
      fprintf(fd_traj,"\n&\n@s2 symbol 1\n@s2 symbol color 1\n@s2 symbol fill color 1\n@s2 symbol fill pattern 1\n");
      fprintf(fd_traj,"0.0 0.0\n");

      /* Close file and exit. */
      fclose(fd_traj);
      fclose(fd_ene);
      fclose(fd_ke);
      fclose(fd_pot);

}
