#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "params.h"

/*
 * Initial_Periodic_C/demons.c: 
 */

// unic.c
float uni(void);
void rinit(int ijkl);

// timer.c
double Secnds(void);
void Get_Time(double *);

// particle data structure
typedef struct{
  double x;
  double y;
  double z;
  int id;
}particle;

// velocity data structure
typedef struct{
  double x;
  double y;
  double z;
}velocity;

// force data structure
typedef struct{
  double x;
  double y;
  double z;
}force_struct;


void initpos(double, particle[]);
void initvel(double, velocity[]);
double pairforce(force_struct[], particle[], int, double);
double fij(force_struct[], particle, particle, int, double);


int main(){

  // The handle for the file that the potential energy is 
  // outputted to (i.e. pe.ax).  File output only done 
  // if selected at compile time.
  FILE *output;

  // Loop indices for various loops
  int i, j;

  // Loop variable for the main simulation loop
  int loop;

  // Number of pairs of particles
  int ipair;

  // Seed for the random number generator.  Used to generate
  // the initial particle positions and velocity
  int seed = 666;

  // periodic defines whether the simulation has periodic 
  // physical boundaries or not
  int periodic = FALSE;

  // Below are various simulation parameters
  // Size of the physical box the simulation occurs in
  double lbox = 1.0;
  // velmax is the maximum particle velocity
  double velmax = 0.1;
  // dt is the value of a single time step
  double dt = 0.005;
  double dtsq = dt*dt;

  // These hold local instances of global simulation values, 
  //calculated throughout the simulation
  double pe, ke, energy, sumpos;

  // The following variable are used for timing
  double total_start, total_stop;
  double calc_start, calc_stop;
  double calc_total = 0.0;
  double time_total = 0.0;

  // now is where the particles are stored
  particle now[NPART];
  // old1 and old2 hold previous copies of now
  particle old1[NPART];
  particle old2[NPART];

  // The velocity arrays are only used in the initialisation of the simulation
  velocity vel[NPART];

  // force is used in the pairforce function, to compile the force on each interacting
  // particle
  force_struct force[NPART];

  // Initialise the random number generator
  rinit(seed);

  // Create and initialise the particles
  initpos(lbox, now);
  // Initialise the velocities for the above particles
  initvel(velmax, vel);

  // Used the velocities that were created above to 
  // calculate the state of the simulation one time step ago
  for(i=0; i<NPART; i++){
    old1[i].x = (now[i].x - ((vel[i].x)*dt));
    old1[i].y = (now[i].y - ((vel[i].y)*dt));
    old1[i].z = (now[i].z - ((vel[i].z)*dt));
  }

  // Create handle for file output
  output = fopen("pe.ax", "w");

  // Get a time for the start of the main program loop
  Get_Time(&total_start);

  // Start the main simulation loop. MAX_LOOP is defined in params.h.
  // loop starts from 1 because the simulation is always running one step
  // behind the current loop
  for(loop=1; loop<=MAXLOOP; loop++){

#ifdef IO
    // Output progress
    printf("Starting loop %d\n", loop);
#endif

    // Initialise sumpos to 0.0
    sumpos = 0.0;

    // Now calculate the current sumpos.  The sum of positions
    // is used as a global check of the current simulation state
    for(i=0; i<NPART; i++){
      sumpos += now[i].x;
      sumpos += now[i].y;
      sumpos += now[i].z;
    }
    
#ifdef IO
    // Output the new global sum of particle positions
    printf("Loop %d, Sumpos = %f\n", loop, sumpos);
#endif    

    // Get a start time for the main calculations in the simulation
    Get_Time(&calc_start);

    // Calculate the forces and potential energy between the particles.
    // This is the main simulation calculation.
    pe = pairforce(force, now, periodic, lbox);

//  printf("Velocity on particle 10000: %lf %lf %lf\n", vel[10000].x, vel[10000].y, vel[10000].z);

    // Update the particles positions with the forces calculated above.
    // Record the particle positions for the last two time steps.
    // This data is used to calculate the current positions and the
    // kinetic energy of the system.
    for(i=0; i<NPART; i++){
      old2[i].x = old1[i].x;
      old2[i].y = old1[i].y;
      old2[i].z = old1[i].z;
      
      old1[i].x = now[i].x;
      old1[i].y = now[i].y;
      old1[i].z = now[i].z;
      
      now[i].x = 2.0*now[i].x - old2[i].x + force[i].x*dtsq;
      now[i].y = 2.0*now[i].y - old2[i].y + force[i].y*dtsq;
      now[i].z = 2.0*now[i].z - old2[i].z + force[i].z*dtsq;
    }
 
    // Set the current kinetic energy to zero.
    ke = 0.0;

    // Calculate the simulations kinetic energy
    for(i=0; i<NPART; i++){
      ke += 0.5 * (((now[i].x - old2[i].x)/(2.0*dt))*
		   ((now[i].x - old2[i].x)/(2.0*dt)));
      ke += 0.5 * (((now[i].y - old2[i].y)/(2.0*dt))*
		   ((now[i].y - old2[i].y)/(2.0*dt)));
      ke += 0.5 * (((now[i].z - old2[i].z)/(2.0*dt))*
		   ((now[i].z - old2[i].z)/(2.0*dt)));
    }

    // Calculate the total energy in the system.  Whether this
    // is conserved throughout the simulation is a good indicator
    // of whether the simulation is correct or not.
    energy = ke + pe;

    // Get an end time for the main calculations
    Get_Time(&calc_stop);

    // Work out the total time for main calculations for this loop
    calc_total += calc_stop - calc_start;

#ifdef IO
    // Output the current state of the system.
    printf("loop, pe, ke, energy %d %f %f %f\n", loop-1, pe, ke, energy); 

    // Output the potential energy to file
    fprintf(output, "%d %.16f\n", loop, pe);
#endif

    // Check the particles to find ones that have moved outside
    // the simulation box.  If they have then adjust the particles;
    // int periodic == FALSE then bounce them of the side of the box.
    // If periodic == TRUE then adjust their positions so they wrap 
    // round the box.  To maintain the correctness of the simulation,
    // this has to be done retrospectively to the last two time steps.
    for(i=0; i<NPART; i++){

      // Check whether the particles have moved below the box
      if(now[i].x < 0.0){
	if(periodic==TRUE){
	  now[i].x = now[i].x + lbox;
	  old1[i].x = old1[i].x + lbox;
	  old2[i].x = old2[i].x + lbox;
	}else{
	  now[i].x = -now[i].x;
	  old1[i].x = -old1[i].x;
	  old2[i].x = -old2[i].x;
	}
      }
      // Check whether the particles have moved below the box
      if(now[i].y < 0.0){
	if(periodic==TRUE){
	  now[i].y = now[i].y + lbox;
	  old1[i].y = old1[i].y + lbox;
	  old2[i].y = old2[i].y + lbox;
	}else{
	  now[i].y = -now[i].y;
	  old1[i].y = -old1[i].y;
	  old2[i].y = -old2[i].y;
	}
      }
      // Check whether the particles have moved below the box
      if(now[i].z < 0.0){
	if(periodic==TRUE){
	  now[i].z = now[i].z + lbox;
	  old1[i].z = old1[i].z + lbox;
	  old2[i].z = old2[i].z + lbox;
	}else{
	  now[i].z = -now[i].z;
	  old1[i].z = -old1[i].z;
	  old2[i].z = -old2[i].z;
	}
      }
      
      // Check whether the particles have moved above the box
      if(now[i].x >= lbox){
	if(periodic==TRUE){
	  now[i].x = now[i].x - lbox;
	  old1[i].x = old1[i].x - lbox;
	  old2[i].x = old2[i].x - lbox;
	}else{
	  now[i].x = 2.0*lbox - now[i].x;
	  old1[i].x = 2.0*lbox - old1[i].x;
	  old2[i].x = 2.0*lbox - old2[i].x;
	}
      }
      // Check whether the particles have moved above the box
      if(now[i].y >= lbox){
	if(periodic==TRUE){
	  now[i].y = now[i].y - lbox;
	  old1[i].y = old1[i].y - lbox;
	  old2[i].y = old2[i].y - lbox;
	}else{
	  now[i].y = 2.0*lbox - now[i].y;
	  old1[i].y = 2.0*lbox - old1[i].y;
	  old2[i].y = 2.0*lbox - old2[i].y;
	}
      }
      // Check whether the particles have moved above the box
      if(now[i].z >= lbox){
	if(periodic==TRUE){
	  now[i].z = now[i].z - lbox;
	  old1[i].z = old1[i].z - lbox;
	  old2[i].z = old2[i].z - lbox;
	}else{
	  now[i].z = 2.0*lbox - now[i].z;
	  old1[i].z = 2.0*lbox - old1[i].z;
	  old2[i].z = 2.0*lbox - old2[i].z;
	}
      }
	
    }
    
  }


  // Get a time for the end of the main program loop
  Get_Time(&total_stop);
  // Work out the total time for the main program loop
  time_total = total_stop - total_start;

#ifdef TIMING_OUTPUT
  // Output the timing data
  printf("\n");
  printf("Simulation Times:\n");
  printf("Overall Simulation Time for %d loops is %f\n", MAXLOOP, time_total);
  printf("Single loop Time: %f\n", calc_total/MAXLOOP);
  printf("\n");
#endif

  // Close the output file handle
  fclose(output);

  return 0;
}

// Pairforce calculates the forces on all the pairs of particles
// in the simulation
double pairforce(force_struct force[NPART], particle now[], int periodic, double lbox){

  int i, j, k;

  // These hold the potential energy calculated in this function
  // call
  double pe, pepair;

  // This is used as a force temporary to get results from fij
  // The whole force array could be passed to fij each time
  // but this would be inefficent
  force_struct f[1];

  // Set the potential energy to zero
  pe = 0.0;

  // Initialise the force for this iteration to zero
  for(i=0; i<NPART; i++){
    force[i].x = 0.0;
    force[i].y = 0.0;
    force[i].z = 0.0;
  }

  // Iterate through all the pairs of particles in the system
  // (unique pairs; so can't have i,j and j,i).
  for(i=0; i<NPART-1; i++){
    for(j=i+1; j<NPART; j++){

      // For each pair of particles, call fij
      pepair = fij(f, now[i], now[j], periodic, lbox);
      
      // Update the force each particle in the pair has upon it
      force[i].x = force[i].x - f[0].x;
      force[i].y = force[i].y - f[0].y;
      force[i].z = force[i].z - f[0].z;
      
      force[j].x = force[j].x + f[0].x;
      force[j].y = force[j].y + f[0].y;
      force[j].z = force[j].z + f[0].z;
      
      // Update the potential energy of the system
      pe += pepair;
    }
  }

  return pe;

}

// fij calculates the force on a pair of particles
double fij(force_struct f[], particle ipos, particle jpos, int periodic, double lbox){

  int i;

  double rx, ry, rz;
  // This is the diameter of an individual particle
  double diameter = 0.05;
  double fmax = 4.0;
  double potential;
  // this is absolute distance between the two particles
  double absr = 0.0;

  rx = jpos.x - ipos.x;
  ry = jpos.y - ipos.y;
  rz = jpos.z - ipos.z;

  // If the simulation is periodic then find the 
  // shortest distance between the two partices
  if(periodic==TRUE){

    if(rx > 0.0){
      if(fabs(rx - lbox) < fabs(rx)){
	rx = rx - lbox;
      }
    }else{
      if(fabs(rx + lbox) < fabs(rx)){
	rx = rx + lbox;
      }
    }

    if(ry > 0.0){
      if(fabs(ry - lbox) < fabs(ry)){
	ry = ry - lbox;
      }
    }else{
      if(fabs(ry + lbox) < fabs(ry)){
	ry = ry + lbox;
      }
    }
    
    if(rz > 0.0){
      if(fabs(rz - lbox) < fabs(rz)){
	rz = rz - lbox;
      }
    }else{
      if(fabs(rz + lbox) < fabs(rz)){
	rz = rz + lbox;
      }
    }

  }

  // Calculate the absolute position between the pair of 
  // particles
  absr = rx*rx + ry*ry + rz*rz;

  absr = sqrt(absr);

  // If the pair are close enough that they interact...
  if (absr < diameter){

    // ...then calculate the force between the two particles
    f[0].x = fmax*rx*(1.0/absr - 1.0/diameter);
    f[0].y = fmax*ry*(1.0/absr - 1.0/diameter);
    f[0].z = fmax*rz*(1.0/absr - 1.0/diameter);

    // and calculate the potential energy based on the distance
    // between them
    potential = fmax/(2.0*diameter) * ((absr - diameter)*(absr - diameter));

  }else{

    // If they aren't close enough to interact then set the force
    // between them to zero...
    f[0].x = 0.0;
    f[0].y = 0.0;
    f[0].z = 0.0;
    
    // ...and the potential energy to zero
    potential = 0.0;
    
  }
  
  return potential;
}

// Initialise the particles and their positions at
// the beginning of the simulation
void initpos(double lbox, particle now[]){

  int i;

  // Create NPART many particles
  for(i=0; i<NPART; i++){
    // Create a unique id for each particle
    now[i].id = i;
    // And get a random position for the particle
    now[i].x = ((double)uni())*lbox;
    now[i].y = ((double)uni())*lbox;
    now[i].z = ((double)uni())*lbox;
  }


}

// Create random velocities for the particles in the
// simulation
void initvel(double velmax, velocity vel[]){

  int i;

  // For all the particles in the simulation
  for(i=0; i<NPART; i++){

    // Get a random velocity for the particle
    vel[i].x = velmax*(2.0*((double)uni())-1.0);
    vel[i].y = velmax*(2.0*((double)uni())-1.0);
    vel[i].z = velmax*(2.0*((double)uni())-1.0);
  }    
}
