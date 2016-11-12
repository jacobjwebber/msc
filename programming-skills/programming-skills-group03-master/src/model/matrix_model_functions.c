/********************************************************************************************************/
/*         functions which initialize map arrays and population arrays, calculate updates using the     */
/*           model for each timestep, and call the update fuction while outputting                      */ 
/*            PPM files and average population densities at the specif\ied intervals.                   */
/********************************************************************************************************/

/*========================== Library include files ===========================*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "arralloc.h"
#include "params.h"
#include "output_functions.h"

#define FINAL_TIME 500.0


/********************************************************************************************************/

void print_map(int **map, int nx, int ny)
{
    int i, j;
    
    printf("Map:\n");
    for(j=ny; j>=1; --j)
    {
        for(i=1; i<=nx; ++i)
        {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
    printf("========================================================\n");
}



/********************************************************************************************************/

void print_population(double **map, int nx, int ny, char *name)
{
    int i, j;
    printf("Population map of %s\n", name);
    for(j=ny; j>=1; --j)
    {
        for(i=1; i<=nx; ++i)
        {
            printf("%f ", map[i][j]);
        }
        printf("\n");
    }
    printf("========================================================\n");
}



/********************************************************************************************************/
/*            initializes an array to represent the map, and sets halo region to zeio.                  */
/********************************************************************************************************/
int **init_map(int nx, int ny)
{
    int **map = arralloc(sizeof(int), 2, nx+2, ny+2);
    int i;
    printf("Initialising grid map:\n");
    for(i=0; i<=nx+1; ++i)
    {
      map[i][0]=0;
      map[i][ny+1]=0;
    }
    for(i=0; i<=ny+1; ++i)
      {
	map[0][i]=0;
	map[nx+1][i]=0;
      }
    return map;
}



/********************************************************************************************************/
/*                    set halo region to zero for array of doubles                                      */ 
/********************************************************************************************************/

void set_halo_zero(double ** array, int nx, int ny)
{
  int i;
  for(i=0; i<=nx+1; ++i)
    {
      array[i][0]=0;
      array[i][ny+1]=0;
    }
  for(i=0; i<=ny+1; ++i)
    {
      array[0][i]=0;
      array[nx+1][i]=0;
    }
}



/********************************************************************************************************/
/*          declares and initialises array for population density. densities randomly generated between */
/*          0.0 and 5.0 where the corresponding map grid is land.                                       */
/********************************************************************************************************/

double **init_densities(int **map, int nx, int ny)
{
    double **population = arralloc(sizeof(double), 2, nx+2, ny+2);
    printf("Initialising a population:\n");
    int i, j;
    static int val = 1;
    srand((unsigned int)(time(NULL) + val));
    val += 1;
    for(i=1; i<=nx; ++i)
    {
        for(j=1; j<=ny; ++j)
        {
            if (map[i][j]){
                population[i][j] = 5*(double)rand()/(double)RAND_MAX;
            }
        }
    }
    set_halo_zero(population, nx, ny); 
    return population;
}



/********************************************************************************************************/

double sum_of_surround_population(double **population, int i, int j)
{
    int shift;
    double sum = 0;
    
    for(shift=-1; shift<=1; shift+=2)
    {
        sum += population[i][j + shift] + population[i + shift][j];
    }
    return sum;
}



/********************************************************************************************************/

int sum_of_surround_land(int **map, int i, int j)
{
    int shift;
    int sum = 0;
    
    for(shift=-1; shift<=1; shift+=2)
    {
        sum += map[i][j + shift] + map[i + shift][j];
    }
    
    return sum;
}



/********************************************************************************************************/
/*       updates poppulation density arrays according model for one time step (dt).                     */
/*       calculates average population densities if passed compute_average_flag.                        */
/********************************************************************************************************/

void update_populations(int ** map, double ** hares, double ** pumas, double ** hares_old, double ** pumas_old, int nx, int ny, config_params params, int compute_averages_flag, double * average_hares, double * average_pumas)
{
  int i, j;
  set_halo_zero(hares_old, nx, ny);
  set_halo_zero(pumas_old, nx, ny);

  for (i=1; i<=nx; ++i)
    {
      for(j=1; j<=ny; ++j)
        {
          hares_old[i][j] = hares[i][j];
          pumas_old[i][j] = pumas[i][j];
        }
    }
  
  double new_pop;
  double sum_hares = 0;
  double sum_pumas = 0;
  int land_points = 0;

  for (i=1; i<=nx; ++i)
    {
      for(j=1; j<=ny; ++j)
        {
          if (map[i][j] == 1)
            {
              new_pop = sum_of_surround_population(hares_old, i, j) - sum_of_surround_land(map, i, j)*hares_old[i][j];
              new_pop = params.k*new_pop + params.r*hares_old[i][j] - params.a*pumas_old[i][j]*hares_old[i][j];
              hares[i][j] += params.dt*new_pop;
		   
              new_pop = sum_of_surround_population(pumas_old, i, j) - sum_of_surround_land(map, i, j)*pumas_old[i][j];
              new_pop = params.l*new_pop + params.b*hares_old[i][j]*pumas_old[i][j] - params.m*pumas_old[i][j];
              pumas[i][j] += params.dt*new_pop;

              hares[i][j] = (hares[i][j] >= 0.0) ? hares[i][j]: 0.0;
              pumas[i][j] = (pumas[i][j] >= 0.0) ? pumas[i][j]: 0.0;

              if (compute_averages_flag)
                {
                  sum_hares += hares[i][j];
                  sum_pumas += pumas[i][j];
                  ++land_points;
                }
            }
        }
    }
  land_points = (land_points >= 1) ? land_points : 1;
  if (compute_averages_flag)
    {
      *average_hares = sum_hares/(double)land_points;
      *average_pumas = sum_pumas/(double)land_points;
    }
}



/********************************************************************************************************/

void set_file_object(FILE ** fp, char * dir_name, char * filename)
{
  char folder[100];
  strcpy(folder, dir_name);
  strcat(folder, filename);
  *fp = fopen(folder, "w+");
}



/***********************************************************************************************************/
/*  calls update_population for the specified number of timesteps. calls PPM routines and prints averages  */
/***********************************************************************************************************/

void update_grid(int **map, double **hares, double **pumas, int nx, int ny, config_params params, char * dir_name)
{
  int iterations = 0;
  int compute_average_switch = 0;

  double average_hares, average_pumas;

  double **hares_old = arralloc(sizeof(double), 2, nx+2, ny+2);
  double **pumas_old = arralloc(sizeof(double), 2, nx+2, ny+2);
    
  FILE * fp_ave;
  
  set_file_object(&fp_ave, dir_name, "/average_population_densities");
    
  if(fp_ave == NULL)
    {
      fprintf(stderr, "abort: unable to open file to print average densities\n");
      exit(1);
    } 

  if (params.T_average > 0)
    {
      fprintf(fp_ave, "Time\tHare_average\tPumas_average\n");
    }

  while (iterations < FINAL_TIME/params.dt)
    {
      if ((iterations+1)%params.T_average == 0 && params.T_average > 0)
        {
          compute_average_switch = 1;
        }

      update_populations(map, hares, pumas, hares_old, pumas_old, nx, ny, params, compute_average_switch, &average_hares, &average_pumas);

        iterations += 1;

        if (iterations % params.T == 0 && params.T>0)
        {
	  ppm(hares, nx, ny, iterations * params.dt, "hare", dir_name);
	  ppm(pumas, nx, ny, iterations * params.dt, "puma", dir_name);
        }
        
        if (compute_average_switch)
        {
	  fprintf(fp_ave, "%g\t%.3f\t%.3f\n", iterations*params.dt, average_hares, average_pumas);
           compute_average_switch = 0;
        }
    }
  fclose(fp_ave);
}
