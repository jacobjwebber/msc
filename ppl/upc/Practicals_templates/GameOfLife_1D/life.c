
#include <stdio.h>
#include <stdlib.h>
#include <upc.h>

#define BOARD_SIZE 8            // Game of life board = boardSize x boardSize
#define MAXLOOP 10              // Number of update iterations


void findNeighbourCoords(int neighbourCoords[2]);
void haloSwap(shared [BOARD_SIZE/THREADS] int masterBoard[BOARD_SIZE][BOARD_SIZE], int localRows, int neighbourCoords[2], int localBoard[(BOARD_SIZE/THREADS)+2][BOARD_SIZE+2]);


int main(int argc, char *argv)
{

    // ..Data for 8-point stencil
    int neighbourCoords[2];

    // Variables for decomposition
    static int localRows = BOARD_SIZE/THREADS;

    // Varialbes for Game of Life...
    // ..Main arrays
    static shared [BOARD_SIZE/THREADS] int masterBoard[BOARD_SIZE][BOARD_SIZE];
    int localBoard[(BOARD_SIZE/THREADS)+2][BOARD_SIZE+2];
    int neighbours[BOARD_SIZE/THREADS][BOARD_SIZE];
    static shared int active_cells[THREADS];

    int i, j, temp, loop;
    int iStart;

    // !------------------------------------------------------!
    // !                    STAGE ONE                         !
    // !------------------------------------------------------!
    // ! Master image initialises the board and distributes   !
    // ! it among the 2D grid of Images.                      !
    // !------------------------------------------------------!

    // --------------------------
    // 1. Initialises masterBoard
    // --------------------------
    // First, set all the board to 0
    // Each thread initialises it's own local piece
    // The code distributes the board in vertical stripes => affinity of j will select all local columns
    for (i=0; i < BOARD_SIZE; i++, i)
        upc_forall(j=0; j < BOARD_SIZE; j++; j)
            masterBoard[i][j] = 0;

    // Synchronise
    upc_barrier;


    // Master thread
    if (MYTHREAD == 0) {

        // Master thread sets the row and column nearest the centre (boardSize/2) alive

        // Set centre row alive
        i = (BOARD_SIZE/2) - 1;
        for (j=0; j < BOARD_SIZE; j++)
            masterBoard[i][j] = 1;

        // Set centre column alive
        j = (BOARD_SIZE/2) - 1;
        for (i=0; i < BOARD_SIZE; i++)
            masterBoard[i][j] = 1;

        // -------------------------------------
        // 2. Print initial number of live cells
        // -------------------------------------
        temp = 0;
        for (i=0; i < BOARD_SIZE; i++)
            for (j=0; j < BOARD_SIZE; j++)
                if (masterBoard[i][j] != 0)
                    temp++;
        printf("\n[%2d] Initial live count = %d\n\n", MYTHREAD, temp);

    }

    // Synchronisation needed to make sure that master has initialized board
    upc_barrier;

    // TO DO
    // Each thread needs to copy its local shared piece into a local private space
    for()
        for()
            localBoard[][] = masterBoard[][];

    // !------------------------------------------------------!
    // !                    STAGE TWO                         !
    // !------------------------------------------------------!
    // ! Find the row-column coordinates of the 4 nearest     !
    // ! neighbours of each thread.                           !
    // ! Then main update loop with MAXITER iterations.       !
    // ! At each iteration:                                   !
    // ! 1) swap halo with 4 neighbours                       !
    // ! 2) fill neighbours array with the # alive cells      !
    // !    sourrounding each point                           !
    // ! 3) update the board using the values from the        !
    // !    neighbours array                                  !
    // !------------------------------------------------------!

    findNeighbourCoords(neighbourCoords);

    // Main update loop - Perform MAXLOOP updates
    for (loop=0; loop < MAXLOOP; loop++) {

        // TO DO
        // Synchronise to ensure that all threads have
        // completed their last iteration

        // Perform halo swap with neighbours
        haloSwap(masterBoard, localRows, neighbourCoords, localBoard);

        // Count number of neighbours
        for (i=1; i < localRows+1; i++)
            for (j=1; j < BOARD_SIZE+1; j++)
                neighbours[i-1][j-1] = localBoard[i+1][j] +   localBoard[i-1][j] +
                                       localBoard[i][j+1] +   localBoard[i][j-1] +
                                       localBoard[i+1][j+1] + localBoard[i+1][j-1] +
                                       localBoard[i-1][j+1] + localBoard[i-1][j-1];

/*
        // Uncomment to print the board at each iteration
        // Helpful when debuging
        for(i=0; i < THREADS; i++) {
            if ( i == MYTHREAD ) {
                for(iStart=1; iStart < localRows+1; iStart++) {
                    for(j=1; j < BOARD_SIZE+1; j++) {
                        printf("%3d ", localBoard[iStart][j]);
                    }
                    printf("\n");
                }
            }
            upc_barrier;
        }
*/

        // Calculate new generation
        for (i=1; i < localRows+1; i++) {
            for (j=1; j < BOARD_SIZE+1; j++) {
                if (neighbours[i-1][j-1] < 2 || neighbours[i-1][j-1] > 3)
                    localBoard[i][j] = 0;
                else if (neighbours[i-1][j-1] == 3)
                    localBoard[i][j] = 1;
            }
        }

        // Sync needed to ensure that all halo swaps have completed before
        // board is updated
        upc_barrier;


        // TO DO
        // Copy updated values back to shared board
        for()
            for()
                masterBoard[][] = localBoard[][];



        // Sync needed before counting total number of live cells
        upc_barrier;


        // TO DO
        // Prints total number of live cells at each iteration.

    }

    exit(EXIT_SUCCESS);
}


//  !--------------------------------------------!
//  ! Finds the 4 neighbours of each image.      !
//  !--------------------------------------------!
void findNeighbourCoords(int neighbourCoords[2])
{
    int i;

    // Up neighbour
    neighbourCoords[0] = MYTHREAD - 1;

    // Down neighbour
    neighbourCoords[1] = MYTHREAD + 1;

    // Apply periodic boundary conditions for threads at the edge
    if (neighbourCoords[0] < 0)
        neighbourCoords[0] = THREADS - 1;

    if ( neighbourCoords[1] >= THREADS)
        neighbourCoords[1] = 0;
}


//  !--------------------------------------------!
//  ! Get halo data from 4 nearest neighbours of !
//  ! each Thread.                               !
//  ! 1) Get from E & W neighbours.              !
//  ! 2) Sync all to make sure that all procs    !
//  ! have halo from E & W neighbours.           !
//  ! 3) Get from N & S neighbours - get core    !
//  ! data and halo also.                        !
//  !--------------------------------------------!
void haloSwap(shared [BOARD_SIZE/THREADS] int masterBoard[BOARD_SIZE][BOARD_SIZE], int localRows, int neighbourCoords[2], int localBoard[(BOARD_SIZE/THREADS)+2][BOARD_SIZE+2])
{

    // Get Halo elements from North neighbour
    for()

    // Get Halo elements from South neighbour
    for()


    // Fill in elements on the West side
    for()

    // Fill in elements on the East side
    for()

}

