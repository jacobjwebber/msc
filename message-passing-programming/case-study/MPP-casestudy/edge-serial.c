#include <stdio.h>
#include "pgmio.h"

#define N 1000

int main(int argc, char** argv)
{
    int Nx, Ny, i, j,k;
    pgmsize(argv[1], &Nx, &Ny);

    float image[Nx][Ny], output[Nx][Ny], inverse[Nx][Ny];
    

    pgmread(argv[1], image, Nx, Ny);
    
    pgmwrite("input.pgm", image, Nx, Ny);
    printf("image dimensions are %i x %i\n", Nx, Ny);
    
    
    //add halo to original image.
    

    float image_bigger[Nx+2][Ny+2];

    for (i= 0; i < (Nx+2); i++)
    {
        for(j = 0; j < (Ny+2); j++)
        {
            if(j== 0 | i == 0 | j == Ny+1 | i == Nx+1)
                image_bigger[i][j] = 255;
            else
                image_bigger[i][j] = image[i-1][j-1];
        }
    }
    
    //main loop
    
    for (i= 0; i < (Nx+1); i++)
    {
        for (j = 0; j < (Ny+1); j++)
        {
                output[i][j] =  image_bigger[i][j-1] + image_bigger[i][j+1]
                           +  image_bigger[i-1][j] + image_bigger[i+1][j]
                           - 4*image_bigger[i][j];
        } 
    }
    
    pgmwrite("output.pgm", output, Nx, Ny);


    // build another large array for carrying out inverse operation.
    for (i= 0; i < (Nx+2); i++)
    {
        for(j = 0; j < (Ny+2); j++)
        {
            if(j== 0 | i == 0 | j == Ny+1 | i == Nx+1)
                image_bigger[i][j] = 255;
            else
                image_bigger[i][j] = output[i-1][j-1];
        }
    }

    for (k = 0; k < N; k++)
    {

        for (i= 0; i < (Nx+1); i++)
        {
            for (j = 0; j < (Ny+1); j++)
            {
                inverse[i][j] =  (image_bigger[i][j-1] + image_bigger[i][j+1]
                           +  image_bigger[i-1][j] + image_bigger[i+1][j]
                           - image_bigger[i][j])*0.25;
            } 
        }   
    }

    pgmwrite("inversed.pgm", inverse, Nx, Ny);

 

    return 0;
}
