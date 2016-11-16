#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//maximum number of threads this code can support.
#define MAX_THREADS 128

#define N 729
#define reps 1000 
#include <omp.h> 

double a[N][N], b[N][N], c[N];
int jmax[N];  


void init1(void);
void init2(void);
void runloop(int); 
void loop1chunk(int, int);
void loop2chunk(int, int);
void valid1(void);
void valid2(void);


int main(int argc, char *argv[]) { 

  double start1,start2,end1,end2;
  int r;

  init1(); 

  start1 = omp_get_wtime(); 

  for (r=0; r<reps; r++){ 
    runloop(1);
  } 

  end1  = omp_get_wtime();  

  valid1(); 

  printf("Total time for %d reps of loop 1 = %f\n",reps, (float)(end1-start1)); 


  init2(); 

  start2 = omp_get_wtime(); 

  for (r=0; r<reps; r++){ 
    runloop(2);
  } 

  end2  = omp_get_wtime(); 

  valid2(); 

  printf("Total time for %d reps of loop 2 = %f\n",reps, (float)(end2-start2)); 

} 

void init1(void){
  int i,j; 

  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      a[i][j] = 0.0; 
      b[i][j] = 3.142*(i+j); 
    }
  }

}

void init2(void){ 
  int i,j, expr; 

  for (i=0; i<N; i++){ 
    expr =  i%( 3*(i/30) + 1); 
    if ( expr == 0) { 
      jmax[i] = N;
    }
    else {
      jmax[i] = 1; 
    }
    c[i] = 0.0;
  }

  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      b[i][j] = (double) (i*j+1) / (double) (N*N); 
    }
  }
 
} 


void runloop(int loopid)  {
  int i; 
  int remaining[MAX_THREADS];
  for (i = 0; i < MAX_THREADS; i++)
  {
      remaining[i] = 0;
  }
#pragma omp parallel default(none) shared(loopid, remaining)
  {
      /*SETUP*/
      /*====================================================*/
    int myid  = omp_get_thread_num();

    int end_index;
    int nthreads = omp_get_num_threads(); 

    if (MAX_THREADS < nthreads)
    {
        printf("TOO MANY THREADS.\n");
        exit (-1);
    }   
    
    int ipt = (int) ceil((double)N/(double)nthreads); 

    /*Set how much work each thread starts with*/
    /*For cache coherency reasons this works better than remaining[myid] = ipt*/
    #pragma omp critical
    {
        for (i = 0; i < nthreads; i++)
        {
            remaining[i] = ipt;
        }
    }

    end_index = ipt * (myid+1);

    if (end_index > N)
        end_index = N;
      
      
      /*Do Work*/
      /*====================================================*/
    int done = 0;
    int chunk_size, lo, hi, temp_max, steal_index;
    temp_max=1;
    while(!done)
    {
        #pragma omp critical
        {

        if(remaining[myid] == 0)
        {
            //use loop to find thread with most work remaining
            temp_max = 0;
            for (i = 0; i < nthreads; i++)
            {
                if(temp_max < remaining[i])
                {
                    temp_max = remaining[i];
                    steal_index = i;
                }
            }

        }
        else
        {
            steal_index = myid;
        }


        chunk_size = (int) ceil((double) remaining[steal_index] /(double) nthreads);
        lo  = end_index - remaining[steal_index];
        hi = lo + chunk_size;


        remaining[steal_index] -= chunk_size;
        } //END CRITICAL

        switch (loopid) { 
           case 1: loop1chunk(lo,hi); break;
           case 2: loop2chunk(lo,hi); break;
        } 
        
        if(temp_max == 0)
            break;

    }
  }
}

void loop1chunk(int lo, int hi) { 
  int i,j; 
  
  for (i=lo; i<hi; i++){ 
    for (j=N-1; j>i; j--){
      a[i][j] += cos(b[i][j]);
    } 
  }

} 



void loop2chunk(int lo, int hi) {
  int i,j,k; 
  double rN2; 

  rN2 = 1.0 / (double) (N*N);  

  for (i=lo; i<hi; i++){ 
    for (j=0; j < jmax[i]; j++){
      for (k=0; k<j; k++){ 
	c[i] += (k+1) * log (b[i][j]) * rN2;
      } 
    }
  }

}

void valid1(void) { 
  int i,j; 
  double suma; 
  
  suma= 0.0; 
  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      suma += a[i][j];
    }
  }
  printf("Loop 1 check: Sum of a is %lf\n", suma);

} 


void valid2(void) { 
  int i; 
  double sumc; 
  
  sumc= 0.0; 
  for (i=0; i<N; i++){ 
    sumc += c[i];
  }
  printf("Loop 2 check: Sum of c is %f\n", sumc);
} 
