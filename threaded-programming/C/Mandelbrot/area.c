#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h> 

# define NPOINTS 2000
# define MAXITER 2000


struct complex{
  double real;
  double imag;
};

int main(){
  int numoutside = 0;
  double area, error, ztemp;
  double start, finish;
  struct complex z, c;

/*
 *   
 *
 *     Outer loops run over npoints, initialise z=c
 *
 *     Inner loop has the iteration z=z*z+c, and threshold test
 */
  int threads;
  int i;
  int thread_loops;
  int thread_id;
  int private_start_index;
  int private_end_index;


  start = omp_get_wtime();  
 
#pragma omp parallel default(none) reduction(+:numoutside) \ 
 private(i,thread_id,private_start_index,private_end_index,c,z,ztemp) \
 shared(threads,thread_loops) 
 {

 threads = omp_get_num_threads();
 thread_loops = NPOINTS/threads;
 thread_id = omp_get_thread_num();
 private_start_index = thread_id*thread_loops;
 private_end_index = private_start_index + thread_loops;

  for (int i=private_start_index; i<private_end_index; i++) {
    for (int j=0; j<NPOINTS; j++) {
      c.real = -2.0+2.5*(double)(i)/(double)(NPOINTS)+1.0e-7;
      c.imag = 1.125*(double)(j)/(double)(NPOINTS)+1.0e-7;
      z=c;
      for (int iter=0; iter<MAXITER; iter++){
	ztemp=(z.real*z.real)-(z.imag*z.imag)+c.real;
	z.imag=z.real*z.imag*2+c.imag; 
	z.real=ztemp; 
	if ((z.real*z.real+z.imag*z.imag)>4.0e0) {
	  numoutside++; 
	  break;
	}
      }
    }
  }
}

  finish = omp_get_wtime();  

/*
 *  Calculate area and error and output the results
 */

      area=2.0*2.5*1.125*(double)(NPOINTS*NPOINTS-numoutside)/(double)(NPOINTS*NPOINTS);
      error=area/(double)NPOINTS;

      printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n",area,error);
      printf("Time = %12.8f seconds\n",finish-start);

  }
