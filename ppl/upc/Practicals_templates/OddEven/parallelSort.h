#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <upc.h>

#define totalListSize 40  // Size of large array to sort
#define numThreads 8
#define localListSize totalListSize/numThreads

void initialiseArray(double initArray[], int nInitArray);
void oddEvenNeighbours(int myID, int nThreads, int *oddNeigh, int *evenNeigh);
void compareSplit(const int nValues, shared [localListSize] double workingList[], double neighbourList[], int smaller);
void quickSortDriver(double arrayToSort[], int array_size);

