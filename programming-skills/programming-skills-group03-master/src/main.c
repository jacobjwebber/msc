
/******************************************************************************
 *  main.c This is the main function of the CLiPH simulation, which provides  *
 *  approximation results of propagation of hares and pumas in a designed map *
 *  over a certain time period based on the provided difference functions.    *
 *  The outputs include PPM image files and average of population density     *
 *  and total time of execution.                                              *
 ******************************************************************************/

/*========================== Library include files ===========================*/
#include <stdio.h>
#include <time.h>

/*========================== Project include files ===========================*/
#include "build_results_dir.h"
#include "model/matrix_model_functions.h"
#include "input/input.h"
#include "output/output_functions.h"
#include "params/params.h"

/*============================ External file input ===========================*/
#define default_input "input.txt"


/*====================== Internal function declarations  =====================*/

int main(int argc, char * argv[])
{
  /* start timing */
  clock_t start, end;
  start = clock();
  
  /* get parameters */
  char * config_filename = "params.dat";
  config_params params;
  set_params(&params, config_filename);
  
 
  int NX, NY;

  char * input_filename;
  input_filename = (argc > 1)? argv[1]: default_input; 
  
  /* create directories */
  char * results_dir = get_dir_name();
  build_results_directory(results_dir);
 
  /* initialization */
  int **map = input_map(input_filename, &NX, &NY); 

  double **hares = init_densities(map, NX, NY);
  double **pumas = init_densities(map, NX, NY);
  
  /* simulation and output*/
  if(NX < 20 && NY < 20)
  {
    print_map(map, NX, NY);
    printf("initial populations:\n");  
    print_population(hares, NX, NY, "hares");
    print_population(pumas, NX, NY, "pumas");
    printf("\n");
  }
  
  printf("running simulation...\n");
  update_grid(map, hares, pumas, NX, NY, params, results_dir);


  if(NX < 20 && NY < 20)
  {
    print_map(map, NX, NY);
    printf("final populations:\n");
    print_population(hares, NX, NY, "hares");
    print_population(pumas, NX, NY, "pumas");
  }
  
  /* stop timing */
  end = clock();
  print_time(start, end);
  
  return 0;
}

