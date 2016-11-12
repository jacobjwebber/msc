#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "matrix_model_functions_test.h"
#include "matrix_model_functions.h"
#include "params.h"
#include "arralloc.h"
/********* Initialise before and clean after tests **********/

int init_model_test()
{
    return 0;
}

int clean_model_test()
{
    return 0;
}


/************ Test cases **********************/

/* test for one pass of the model on both populations */
void test_one()
{
  int ** map = init_map(2, 2);
  map[1][1]=1;
  map[1][2]=1;
  map[2][1]=0;
  map[2][2]=0;
  double ** hares = (double **)arralloc(sizeof(double), 2, 4, 4);
  double ** pumas = (double **)arralloc(sizeof(double), 2, 4, 4);
  double ** hares_old = (double **)arralloc(sizeof(double), 2, 4, 4);
  double ** pumas_old = (double **)arralloc(sizeof(double), 2, 4, 4);
  hares[1][1] = 5.0;
  pumas[1][2] = 2.0;
  config_params params;
  set_params(&params, NULL);
  double average_h;
  double average_p;
  update_populations(map, hares, pumas, hares_old, pumas_old , 2, 2, params, 1, &average_h, &average_p);

  CU_ASSERT_DOUBLE_EQUAL(hares[1][1], 4.76, 0.0001);
  CU_ASSERT_DOUBLE_EQUAL(hares[1][2], 0.4, 0.0001);
  CU_ASSERT_DOUBLE_EQUAL(pumas[1][1], 0.16, 0.0001);
  CU_ASSERT_DOUBLE_EQUAL(pumas[1][2], 1.792, 0.0001);
  CU_ASSERT_DOUBLE_EQUAL(average_h, 2.58, 0.0001);
  CU_ASSERT_DOUBLE_EQUAL(average_p, 0.976, 0.0001);
}

void test_two()
{
  int **map = init_map(8, 9);
  CU_ASSERT_PTR_NOT_NULL(map);
}

void test_three()
{
  int correct_range = 1;
  int nx = 5;
  int ny =6;
  int **map = init_map(nx, ny);
  double ** pop = init_densities(map, nx, ny);
  int i, j;
  for(i=1; i<=nx; ++i)
    { 
      for(j=1; j<=ny; ++j)
	{
	  if (pop[i][j]<0.0 || pop[i][j]>5.0)
	    {
	      correct_range=0;
	    }
	}
    }
  int halo_all_zero = 1;
  for(i=0; i<=nx+1; ++i)
    {
      if (pop[i][0]!=0 || pop[i][ny+1]!=0)
	{
	  halo_all_zero = 0;
	}
    }
  for(j=0; j<=ny+1; ++j)
    {
      if(pop[0][j]!=0 || pop[nx+1][j]!=0)
	{
	  halo_all_zero = 0;
	}
    }
  CU_ASSERT(correct_range);
  CU_ASSERT(halo_all_zero);
}

/* add a suite to the registry */
int model_test_add_suite()
{
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite( "model_test_suite", init_model_test, clean_model_test);
    if ( NULL == pSuite )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add tests to suite. */
    if ( (NULL == CU_add_test(pSuite, "map points to a non_null pointer", test_two)) || (NULL == CU_add_test(pSuite, "test basic model for 2x2 case", test_one)) || (NULL == CU_add_test(pSuite, "pop array initializes correctly", test_three)))
          
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return 0;
}
