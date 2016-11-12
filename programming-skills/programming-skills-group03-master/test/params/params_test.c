#include <stdio.h>
#include "params.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define FP_PRECISION 0.00001
/********* Initialise before and clean after tests **********/

int init_params_test()
{
    return 0;
}

int clean_params_test()
{
    return 0;
}


/************ Test cases **********************/


void test_defaults_passed_when_file_not_found()
{
    config_params parameters;
    set_params(&parameters, "file name");


    CU_ASSERT_DOUBLE_EQUAL(parameters.r, 0.08, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.a, 0.04, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.b, 0.02, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.m, 0.06, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.k, 0.20, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.l, 0.20, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.dt, 0.4, FP_PRECISION);
    CU_ASSERT_EQUAL(parameters.T, 100);
    CU_ASSERT_EQUAL(parameters.T_average, 100);
}

void test_config_file_read_correctly()
{
    config_params parameters;
    set_params(&parameters, "test/params2.dat");


    CU_ASSERT_DOUBLE_EQUAL(parameters.r, 0.08, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.a, 0.08, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.b, 0.04, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.m, 0.12, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.k, 0.40, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.l, 0.40, FP_PRECISION);
    CU_ASSERT_DOUBLE_EQUAL(parameters.dt, 0.8, FP_PRECISION);
    CU_ASSERT_EQUAL(parameters.T, 20);
    CU_ASSERT_EQUAL(parameters.T_average, 200);
}




/* add a suite to the registry */
int params_test_add_suite()
{
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite( "params_test_suite", init_params_test, clean_params_test);
    if ( NULL == pSuite )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add tests to suite. */
    if ( (NULL == CU_add_test(pSuite, "Test defaults are set when config not found",  test_defaults_passed_when_file_not_found)) ||
         (NULL == CU_add_test(pSuite, "test_config_file_read_correctly", test_config_file_read_correctly))
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return 0;
}

