#include <stdio.h>
#include "pgmio.h"

int main(int argc, char** argv)
{
    int Nx, Ny, i, j;
    pgmsize(argv[1], &Nx, &Ny);

    float image[Ny][Nx], output[Ny][Nx];

    pgmread(argv[1], image, Nx, Ny);
    
    printf("image dimensions are %i x %i\n", Nx, Ny);
    
    //main loop
    
    for (j= 1; j < (Ny -1); j++)
    {
        for (i = 1; i < (Nx- 1); i++)
        {
            output[j][i] = image[j-1][i] + image[j+1][i]
                            + image[j][i-1] + image[j][i+1]
                            - 4*image[j][i];
        }
    }
    
    pgmwrite("output.pgm", output, Ny, Nx);

    return 0;
}
