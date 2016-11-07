
#include <stdio.h>
#include <stdlib.h>
#include <upc.h>

#define BOARD_SIZE 8            // Game of life board = boardSize x boardSize
#define MAXLOOP 10              // Number of update iterations
#define NDIMS 2
#define NDIRS 4

// Change this for different processor grid:
#define N_THREAD_ROW 4
#define N_THREAD_COL 2


typedef struct {
    int localBoard[BOARD_SIZE/N_THREAD_ROW+2][BOARD_SIZE/N_THREAD_COL+2];
} grid_cell;

const int N = 0, S = 1, E = 2, W = 3;   // Indices Up, Down, Left & Right


void findNeighbourCoords(int myCoords[2], int neighbourCoords[NDIMS][NDIRS]);
void haloSwap(shared grid_cell grid_2D_array[N_THREAD_ROW][N_THREAD_COL], int localRows, int localCols, int neighbourCoords[NDIMS][NDIRS], int myCoords[2]);


int main(int argc, char *argv)
{
    // Varialbes for parallel environment..
    // ..to store row-column coordinates of each thread
    int myCoords[2];

    // ..Data for 8-point stencil
    int neighbourCoords[NDIMS][NDIRS];

    // Variables for decomposition
    static int localRows = BOARD_SIZE/N_THREAD_ROW;
    static int localCols = BOARD_SIZE/N_THREAD_COL;

    // Varialbes for Game of Life...
    // ..Main arrays
    int masterBoard[BOARD_SIZE][BOARD_SIZE];
    static shared grid_cell grid_2D_array[N_THREAD_ROW][N_THREAD_COL];
    static shared int active_cells[THREADS];
    int neighbours[BOARD_SIZE/N_THREAD_ROW][BOARD_SIZE/N_THREAD_COL];

    int i, j, temp, loop;
    int iStartRow, iEndRow, iStartCol, iEndCol, coordi, coordj;

    // !------------------------------------------------------!
    // !                    STAGE ONE                         !
    // !------------------------------------------------------!
    // ! Master image initialises the board and distributes   !
    // ! it among the 2D grid of Images.                      !
    // !------------------------------------------------------!


    // Master thread
    if (MYTHREAD == 0) {

        // --------------------------
        // 1. Initialises masterBoard
        // --------------------------

        // First, set all the board to 0
        for (i=0; i < BOARD_SIZE; i++)
            for (j=0; j < BOARD_SIZE; j++)
                masterBoard[i][j] = 0;

        // Then set the row and column nearest the centre (boardSize/2) alive

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

        // TO DO
        // -----------------------------------------------------------------
        // 3. Copies a chunk of masterBoard to core section of grid_2D_array
        // -----------------------------------------------------------------
        for () {
            for () {
                for () {
                    for () {
                        // Put from master to thread at [i,j]
                        // Don't forget the halos
                        grid_2D_array[][].localBoard[][] = masterBoard[][];
                    }
                }
            }
        }
    }

    // Synchronisation needed to make sure that master has
    // finished masterBoard copy
    upc_barrier;

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

    myCoords[0] = MYTHREAD/N_THREAD_COL;
    myCoords[1] = MYTHREAD%N_THREAD_COL;

    findNeighbourCoords(myCoords, neighbourCoords);

    upc_barrier;

    // Main update loop - Perform MAXLOOP updates
    for (loop=0; loop < MAXLOOP; loop++) {

        // Synchronise to ensure that all threads have
        // completed their last iteration
        upc_barrier;

        // Perform halo swap with neighbours
        haloSwap(grid_2D_array, localRows, localCols, neighbourCoords, myCoords);

        // Count number of neighbours
        for (i=1; i < localRows+1; i++)
            for (j=1; j < localCols+1; j++)
                neighbours[i-1][j-1] =
                    grid_2D_array[myCoords[0]][myCoords[1]].localBoard[i+1][j] +
                    grid_2D_array[myCoords[0]][myCoords[1]].localBoard[i-1][j] +
                    grid_2D_array[myCoords[0]][myCoords[1]].localBoard[i][j+1] +
                    grid_2D_array[myCoords[0]][myCoords[1]].localBoard[i][j-1] +
                    grid_2D_array[myCoords[0]][myCoords[1]].localBoard[i+1][j+1] +
                    grid_2D_array[myCoords[0]][myCoords[1]].localBoard[i+1][j-1] +
                    grid_2D_array[myCoords[0]][myCoords[1]].localBoard[i-1][j+1] +
                    grid_2D_array[myCoords[0]][myCoords[1]].localBoard[i-1][j-1];


        // sync needed to ensure that all halo swaps have completed before
        // board is updated
        upc_barrier;

        // Calculate new generation
        for (i=1; i < localRows+1; i++) {
            for (j=1; j < localCols+1; j++) {
                if (neighbours[i-1][j-1] < 2 || neighbours[i-1][j-1] > 3)
                    grid_2D_array[myCoords[0]][myCoords[1]].localBoard[i][j] = 0;
                else if (neighbours[i-1][j-1] == 3)
                    grid_2D_array[myCoords[0]][myCoords[1]].localBoard[i][j] = 1;
            }
        }

        // Sync needed before counting total number of live cells
        upc_barrier;

       // TO DO:
       // Prints total number of live cells at each iteration.

    }

    exit(EXIT_SUCCESS);
}


//  !--------------------------------------------!
//  ! Finds the 4 neighbours of each image.      !
//  !--------------------------------------------!
void findNeighbourCoords(int myCoords[2], int neighbourCoords[NDIMS][NDIRS])
{
    int i;

    // North neighbour
    neighbourCoords[0][N] = myCoords[0] - 1;
    neighbourCoords[1][N] = myCoords[1];

    // South neighbour
    neighbourCoords[0][S] = myCoords[0] + 1;
    neighbourCoords[1][S] = myCoords[1];

    // East neighbour
    neighbourCoords[0][E] = myCoords[0];
    neighbourCoords[1][E] = myCoords[1] + 1;

    // West neighbour
    neighbourCoords[0][W] = myCoords[0];
    neighbourCoords[1][W] = myCoords[1] - 1;

    // Apply periodic boundary conditions for images at edge
    for (i=0; i < NDIRS; i++) {
        if (neighbourCoords[0][i] >= N_THREAD_ROW)
            neighbourCoords[0][i] = 0;
        else if (neighbourCoords[0][i] < 0)
            neighbourCoords[0][i] = N_THREAD_ROW - 1;

        if ( neighbourCoords[1][i] >= N_THREAD_COL)
            neighbourCoords[1][i] = 0;
        else if (neighbourCoords[1][i] < 0)
            neighbourCoords[1][i] = N_THREAD_COL - 1;
    }

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
void haloSwap(shared grid_cell grid_2D_array[N_THREAD_ROW][N_THREAD_COL], int localRows, int localCols, int neighbourCoords[NDIMS][NDIRS], int myCoords[2])
{

    int i;

    // Get Halo with localRows elements for right column from East neighbour
    for () {

    }

    // Get Halo with localRows elements for left column from West neighbour
    for () {

    }

    // Synchronise to ensure that images have received halo data before getting
    // from N & S neighbours
    upc_barrier;

    // Get Halo with localCols+2 elements for top row from North neighbour
    // (includes halo data from NE & NW neighbours)
    for () {

    }

    // Get Halo with localCols+2 elements for bottom row from South neighbour
    // (includes halo data from SE & SW neighbours)
    for () {

    }


}


