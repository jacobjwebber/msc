#include <stdio.h>
#include <upc.h>

int main()
{
    printf("I am thread %i and there are %i threads\n", MYTHREAD, THREADS);
    return 0;
}
