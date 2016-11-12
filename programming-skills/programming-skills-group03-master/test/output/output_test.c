#include <stdio.h>
#include <time.h>
#include "../../src/output/output_functions.h"
#include "../../src/arralloc/arralloc.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

/********* Initialise before and clean after tests **********/

int init_output_test()
{
    return 0;
}

int clean_output_test()
{
    return 0;
}


/************  Test cases ( 2 )  **********************/


int test_if_ppm_generated(double t, char type[10], char * time_stamp)
{
	
    int nx = 1500;
    int ny = 2000;
    double **population = arralloc(sizeof(double), 2, ny +2, nx+ 2);
    int i,j;    
    for ( j = 0; j < ny; j++) {
        for ( i = 0; i < nx; i++) {
            population[j][i] = (i + j);
        }
    }

    ppm(population, nx, ny, t, type, time_stamp );
	
    char filename[50];
    sprintf(filename,"%s/%s_PPM/%.2fs_%s_density.ppm", time_stamp, type, t, type);

    FILE *fp = fopen(filename, "r");

    if(fp==NULL)
    {
	return 1;
    }
    else
    {
	fclose(fp);
	return 0;
	   
    }

}


int test_if_timefile_generated()
{
    clock_t a = 50;
    clock_t b = 10000;
    print_time(a, b);

    FILE *fp = fopen("total_time.txt", "r");

    if(fp == NULL)
    {
	return 1;
    }
    else
    {
	fclose(fp);
	return 0;   
    }
}

void Test1()
{
	CU_ASSERT(test_if_ppm_generated(3.4, "hares", "dir"))
}


void Test2()
{
	CU_ASSERT(test_if_timefile_generated())
}


/* add a suite to the registry */
int output_test_add_suite()
{
	CU_pSuite pSuite = NULL;

	pSuite = CU_add_suite( "output_test_suite", init_output_test, clean_output_test );
	
	if (NULL == pSuite)
	{
		CU_cleanup_registry();
        	return CU_get_error();	
	}


	if ( (NULL == CU_add_test(pSuite, "ppm files have been generated", Test1)) ||
	    (NULL == CU_add_test(pSuite, "total_time.txt is generated to record time", Test2)) ) 
	{
		CU_cleanup_registry();
        	return CU_get_error(); 
	}
	
	return 0;
}
 




