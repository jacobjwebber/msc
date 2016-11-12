/******************************************************************************
 * Input        Interface functions to read input map from an input file.     *
 * input_map()  Set grid dimension, grid values, and return a pointer for the *
 * map matrix.                                                                *
 ******************************************************************************/

/*========================== Library include files ===========================*/
#include <stdio.h>
#include <stdlib.h>
/*========================== Project include files ===========================*/
#include "../arralloc/arralloc.h"


int ** input_map(char * input_filename, int * nx, int * ny)
{
    //open file so it can be read
    FILE * input;
    input = fopen(input_filename, "r");

    //in case file does not exist, terminate process
    if (input == NULL)
    {
        fprintf(stderr, "Error: input file does not exist. Exiting...\n");
        exit(1);
    }

    //read first line and store first two-values as NX and NY (grid dimension)
    fscanf(input, "%d %d\n", nx, ny);

    //in case NX and/or NY is not positive integers, terminate process
    if (*nx <= 0 || *ny <= 0)
    {
        fprintf(stderr, "Error: grid dimensions must be positive integers. Exiting...\n");
        exit(1);
    }

    //initialise grid using arraloc library and allocate halos region
    int ** grid = (int **)arralloc(sizeof(int), 2, (*nx)+2, (*ny)+2);

    int i, j;
  
    //read every values in input file and store them as grid values
    for(j=(*ny); j>=1; --j)
    {
        for(i=1; i<=*nx; ++i)
	{
	    if (fscanf(input,"%d", &grid[i][j]) < 1) //in case there is not enough grid values for specified dimensions, teminate process
	    {
	        fprintf(stderr, "Not enough grid values for specified dimensions\n");
	        exit(1);
	    }
	    if (grid[i][j] != 0 && grid[i][j] != 1) //in case grid value is not 0 or 1, teminate process
	    {
	        fprintf(stderr, "Grid values must be either 0 or 1. Found a value of %d. Exiting...\n", grid[i][j]);
	        exit(1);
	    }
	}
    }

    //close the file and return the grid pointer
    fclose(input);
    return grid;
}
