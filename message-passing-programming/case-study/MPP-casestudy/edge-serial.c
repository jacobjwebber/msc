#include <stdio.h>
#include "pgmio.h"

int main(int argc, char** argv)
{
    int Nx, Ny, i, j;
    pgmsize(argv[1], &Nx, &Ny);

    float image[Ny][Nx], output[Ny][Nx];
    

    pgmread(argv[1], image, Nx, Ny);
    
    pgmwrite("input.pgm", image, Nx, Ny);
    printf("image dimensions are %i x %i\n", Nx, Ny);
    
    //main loop
    
    for (i= 1; i < (Ny -1); i++)
    {
        for (j = 1; j < (Nx- 1); j++)
        {
            output[i][j] = image[i+1][j+1]; //image[i][j-1] + image[i][j+1]
                             //image[i-1][j] + image[i+1][j]
                            //- 2*image[i][j];
        }
    }
    
    pgmwrite("output.pgm", output, Nx, Ny);

    return 0;
}
