
#include "parallelSort.h"


void compareSplit(const int nValues, shared [localListSize] double workingList[], double neighbourList[], int smaller)
{

    double wSpace[localListSize*2];
    int i;

    // Copy wokringList array to 1st half of wSpace
    for (i=0; i < nValues; i++)
        wSpace[i] =workingList[MYTHREAD*nValues+i];

    // Copy neighbourList to 2nd half of wSpace
    for (i=0; i < nValues; i++)
        wSpace[nValues+i] = neighbourList[i];

    // Sort wSpace with sequential quickSort
    quickSortDriver(wSpace, 2*nValues);

    // Keep 1st half of wSpace if smaller
    if (smaller == 1) {
        for (i=0; i < nValues; i++)
            workingList[MYTHREAD*nValues+i] = wSpace[i];
    }
    else {
        for (i=0; i < nValues; i++)
            workingList[MYTHREAD*nValues+i] = wSpace[nValues+i];
    }

}


