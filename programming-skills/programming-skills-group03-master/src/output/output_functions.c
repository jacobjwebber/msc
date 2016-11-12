
/******************************************************************************
 * colour_pop() Return value of colour based on population density	      *
 * ppm()	Output population density matrix in a colorful image          *
 * print_time() Output total time of execution                                *
 ******************************************************************************/

/*========================== Library include files ===========================*/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*========================== Project include files ============================*/
#include "output_functions.h"

/******************************************************************************
 *  ~colour_pop. Provide the value of color in ppm files based on the value   *
 *  of population density. The relationship between colour and population     *
 *  density is                                                     	      *
 *                 colour = 255 - 255 *sqrt(1-(population/5)^2)               *
 *  usage:   colour_pop(8.52)                                                 *
 *                                                       		      *
 *  ~ppm.  Generate the image files of population density matrix in portable  *
 *  pixmap format(Plain PPM). If density grows up, colour becomes darker.     *
 *  usage: ppm = (population, 1500, 2000, 3.40, "hares", "directory");	      *
 *                                                                            *
 *  ~print_time.  Print the total operation time of the simulaiton and put    *
 *  results in a text file.                               	 	      *
 *  usage: print_time( 30, 7000)

 ******************************************************************************/



/*====================== Internal function declarations=======================*/

int colour_pop(double pop)
{
  int limit = 5;   /* beyond the limit, color is black */
  if (pop>= limit)
    {
      return 255;
    }
  double colour = 255.0 - 255.0*sqrt(1-(pop*pop)/(limit*limit));
  int col_int = (int)colour;
  return col_int;
}



/*-----------------------------------------------------------------------*/


void ppm(double **population, int nx, int ny, double t, char type[10], char * time_stamp)
{
    /* name image file with time and species */
    char filename[50];
    sprintf(filename,"%s/%s_PPM/%.2fs_%s_density.ppm", time_stamp, type, t, type);

    FILE *fp = fopen(filename, "wb");
    int red,green,blue;
    
    /* write content of ppm file */
    fprintf(fp, "P3\n#density.ppm\n%d %d\n255\n", nx, ny);
    int i, j;    
    for (j=ny; j >= 1; --j)
    {
        for (i = 1; i <= nx; ++i)
        {
	  red = colour_pop(population[i][j]);
	  green = red;
	  blue =   red;
            
	  fprintf(fp, "%d %d %d ",red, green, blue);
        }
        fprintf(fp,"\n");
    }
    
    fclose(fp);
}



/*-----------------------------------------------------------------------*/


int print_time(clock_t start, clock_t end)
{
     /* calculate total time and output */
    double duration;
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    
    FILE *fp = fopen("total_time.txt", "aw");
    
    fprintf(fp, "\nThe total time of the code is %.2fs", duration);
    
    fclose(fp);
    return 0;
}







