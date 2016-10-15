#include <stdio.h>
#include <math.h>
#include <omp.h>

#define N 100000000

int main()
{

    double array[N];
    double time = 0;
    double sum =0;
    int sum_len = 10;
    int i, j, k;
    int jmax = log10(N);
    int repetitions;
    
    for (j = 0; j < jmax; j++)
    {
        sum_len = 10 * pow(10,j);
        
        repetitions = N / sum_len;
        
        time = omp_get_wtime();
        for (k = 0; k < repetitions; k++)
        {
            for (i = N-1; i > N-1-sum_len; i--)
            {
                sum += array[i];
            }
        }
        
        time = omp_get_wtime() - time;
        printf("time for summing %i elements is %lf \n", sum_len, time);
    
    }
}
