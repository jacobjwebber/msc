
#include "parallelSort.h"

int func_cmp(const void *v1, const void *v2);

void quickSortDriver(double arrayToSort[], int array_size)
{
    qsort(arrayToSort, array_size, sizeof(double), func_cmp);
}


// Comparison function to be used with QuickSort in
// order to sort the elements in ascenting order
int func_cmp(const void *v1, const void *v2)
{
    double f1 = *(double *)v1;
    double f2 = *(double *)v2;

    if (f1 < f2)
        return -1;
    else if (f1 > f2)
        return 1;
    else
        return 0;
}

