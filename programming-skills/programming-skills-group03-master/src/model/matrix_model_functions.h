#include "../params/params.h"
int **init_map(int nx, int ny);
double **init_densities( int **map,  int nx, int ny);
void print_map(int **map, int nx, int ny);
void print_population(double **map, int nx, int ny, char *name);
int sum_of_surround_land(int **map, int i, int j);
double sum_of_surround_population(double **population, int i, int j);
void update_grid(int **map, double **hares, double **pumas, int nx, int ny, config_params params, char * dir_name);
void update_populations(int ** map, double ** hares, double ** pumas, double ** hares_old, double ** pumas_old, int nx, int ny, config_params params, int compute_averages_flag, double * average_hares, double * average_pumas);
