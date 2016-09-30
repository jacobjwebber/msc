#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){
    printf("Before parellel region: there are %i threads\n", omp_get_num_threads());
#pragma omp parallel
    {
#pragma omp critical 
     {
      printf("hello from thread %d\n",omp_get_thread_num());
     }
    }
      printf("After parellel region: there are %i threads\n", omp_get_num_threads());
}
