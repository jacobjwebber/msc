#include <stdio.h>
#include "parallelSort.h"

void compareSplit(const int nValues, shared [localListSize] double workingList[], double neighbourList[], int smaller);
void quickSortDriver(double arrayToSort[], int array_size);

int main(int argc, char *argv)
{

    int oddPhaseNeighbour, evenPhaseNeighbour;
    double masterList[totalListSize];
    double neighbourList[localListSize];

    int iter, recvNeighbour, i;
    int smaller; // This variable is going to be used as boolean

    // TO DO
    // Declare a shared array with localListSize elements for the distributed list


    // !------------------------------------------------------!
    // !                    STAGE ONE                         !
    // !------------------------------------------------------!
    // ! Master thread generates the list to be sorted and    !
    // ! scatters the list among the other thread threads.     !
    // !------------------------------------------------------!

    // Check that numThreads matches the number of executing 
    // threads
    if (numThreads != THREADS) {
        if (MYTHREAD == 0)
            printf("(ERROR: Value of THREADS %3d does not match number of executing threads, %3d)", numThreads, THREADS);
        // All exit
        exit(EXIT_FAILURE);
    }

    if (MYTHREAD == 0) {

        // TO DO:
        // Master thread initialises his masterList with 
        // random values using the initialiseArray routine.
        //

        // Print initial list
        printf("\nList before sorting: ");

	// TO DO:
	// print list

        printf("\n\n");
    }

    // Check that totalListSize divides evenly by numThreads
    // (want exactly totalListSize/numThreads elements on each thread)
    if (totalListSize%numThreads != 0) {
        // Master prints error message
        if (MYTHREAD == 0) {
            printf("\nERROR: Number of elements to sort %d doesn't divide evenly by total number of numThreads %d", totalListSize, numThreads);
        }
        exit(EXIT_FAILURE);
    }

    // TO DO:
    // Master scatters locaListSize elements of masterList to each of the
    // other threads.
    // First need to synchronise to ensure that other threads have allocated
    // arrays before scattering.

    if (MYTHREAD == 0) {

    }

    // Synchronise again to ensure that each thread has received its portion
    // of the list.


    // !------------------------------------------------------!
    // !                    STAGE TWO                         !
    // !------------------------------------------------------!
    // ! Each thread sorts their local array with sequential  !
    // ! quicksort.                                           !
    // ! The threads also work out which threads they need to !
    // ! swap their list with during each phase of the        !
    // ! algorithm.                                           !
    // !------------------------------------------------------!


    // Sort the local array with sequential quicksort
    quickSortDriver((double *)&workingList[MYTHREAD*localListSize], localListSize);


    // TO DO
    // Work out the thread index which MYTHREAD needs
    // to swap during the odd and the even phase of
    // the algorithm -- implement function at the
    // end of the source file
    oddEvenNeighbours(MYTHREAD, numThreads, &oddPhaseNeighbour, &evenPhaseNeighbour);

    // !----------------------------------------------------------!
    // !                   STAGE THREE                            !
    // !----------------------------------------------------------!
    // ! Main loop of odd-even sorting algorithm.                 !
    // ! numThreads iterations of each thread sending workingList !
    // ! to their odd or even neighbour and then performing a     !
    // ! compare-split operation with their own data and the      !
    // ! data received.                                           !
    // !----------------------------------------------------------!


    // nThreads iterations required
    for (iter=1; iter <= THREADS; iter++) {

        // TO DO:
        //
        // IF iteration is odd 
        //    copy from odd-phase neighbour and call compareSplit
        // ELSE IF iteration is even
        //    copy from even-phase neighbour and call compareSplit
        //
        // HINT: Two syncs. needed.
        //
        // Prototype for compareSplit:
        // compareSplit(int localListSize, double workingList[], double neighbourList[], int smaller)

        // Check if odd or even phase
        if (iter%2 == 0) {

        }
        else {

        }

        // TO DO:
	// Perform a compare-split with workingList and neighbourList

    }

    // !------------------------------------------------------!
    // !                    STAGE FOUR                        !
    // !------------------------------------------------------!
    // ! Gather the sorted data to the master thread and print !
    // ! the sorted list.                                     !
    // ! Also free all allocated space 
    // !------------------------------------------------------!

    // Sync before gathering to ensure that each thread has
    // finished it's sorting iterations
    upc_barrier;

    // TO DO:
    // Master gathers localListSize elements from each thread
    // and puts it in correct part of its masterList array
    if (MYTHREAD == 0) {


        // Print initial list
        printf("\nList after sorting: ");

        printf("\n\n");

    }

}


//  !--------------------------------------------!
//  ! Initialises the array to be sorted.        !
//  ! Uses the rand() intrinsic routine.         !
//  ! Values are between 0 and 1000. Change the  !
//  ! min and max variables for a different range!
//  ! -------------------------------------------!
void initialiseArray(double initArray[], int nInitArray)
{

    int min = 0, max = 1000;
    int i;

    // Seed RNG with the current time
    srand( time(NULL) );

    for (i=0; i < nInitArray; i++) {
        initArray[i] = (max - min) * (rand()/(double)RAND_MAX) + min;
    }
}


//  !---------------------------------------------!
//  ! Determines which thread to communicate with !
//  ! during each phase of the algorithm.         !
//  !---------------------------------------------!
void oddEvenNeighbours(int myID, int nThreads, int *oddNeigh, int *evenNeigh)
{

    // TO DO
    // Determine index of thread to swap data with 
    // during the odd and the even phase of
    // the algorithm.
    //
    // IF myID even 
    //    oddPhaseNeighbour = myID - 1
    //    evenPhaseNeighbour = myID + 1 
    // ELSE IF myID odd 
    //    oddPhaseNeighbour = myID + 1
    //    evenPhaseNeighbour = myID - 1


    // Set neighbours for boundary processors...
    // If oddNeigh or evenNeigh are < 0 or THREADS+1 then set to -1
    // This will allow easier control statements in compare-split section


}


