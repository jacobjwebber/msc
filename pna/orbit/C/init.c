/***********************************************************************
*     init.c: read initialisation parameters from an ASCII file provided 
*             by the user.
***********************************************************************/

double	e, Rp, M1, M2, dR, tend, dt;
int	nstep, outint;

#include <stdio.h>
#include <stdlib.h>

void InitParams()
{

 FILE     	*fd;
 void		dehash(FILE **);

 fd = fopen("params.dat","r");
 if(fd == NULL){
   fprintf(stderr,"Could not open file: params.dat\n");
   fprintf(stderr,"Exiting...\n");
   exit(1);
 }

 fscanf(fd,"%lf",&e);    /* eccentricity of orbit. */
 dehash(&fd);

 fscanf(fd,"%lf",&Rp);   /* pericentric distance (closest approach). */
 dehash(&fd);

 fscanf(fd,"%lf",&M1);   /* mass of object 1. */
 dehash(&fd);

 fscanf(fd,"%lf",&M2);   /* mass of object 2. */
 dehash(&fd);

 fscanf(fd,"%lf",&dR);   /* initial separation of the particles. */
 dehash(&fd);

 fscanf(fd,"%lf",&tend); /* end time for the integration. */
 dehash(&fd);

 fscanf(fd,"%lf",&dt);   /* integration time step. */

 nstep = tend/dt; /* Number of integration steps. */

 /* Set output interval so we do about 500 points per plot. */
 outint = (500+nstep-1)/500;

 fclose(fd);

}

void dehash(FILE **fd)
{
        int     c,d;

        /* in case we have any trailing '\n's */
        if((c =fgetc(*fd)) != '\n')  ungetc(c,*fd);

        /* Get rid of white spaces and everything after a hash to the
           end of the  line. */

        while(((c=fgetc(*fd)) == ' ')||(c == '#')){

            /* if hash delete to the end of the line */
            if(c == '#') while(fgetc(*fd) != '\n');

        }

        ungetc(c,*fd);

}/* End func */

