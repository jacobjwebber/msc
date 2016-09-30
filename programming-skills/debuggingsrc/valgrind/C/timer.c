#include <stdlib.h>
#include <sys/time.h>

// Timer.c is used to return a time value which can be used to 
// calculate and record timings.  This function was not written by
// me, but was taken from one of the course practical codes.

time_t starttime = 0; 

double Secnds()
{

  struct timeval ts; 

  double t;

  int err; 

  err = gettimeofday(&ts, NULL); 

  t = (double) (ts.tv_sec - starttime)  + (double) ts.tv_usec * 1.0e-6; 
 
  return t; 

}

void Get_Time(double *time){

  struct timeval temp_time;

  gettimeofday(&temp_time, NULL);

  *time = (temp_time.tv_sec + (temp_time.tv_usec * 1.0e-6));

}
