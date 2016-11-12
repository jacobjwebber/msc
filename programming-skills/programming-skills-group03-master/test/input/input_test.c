#include <stdio.h>
#include "input_test.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

/********* Initialise before and clean after tests **********/

int init_input_test()
{
    return 0;
}

int clean_input_test()
{
    return 0;
}


/************ Test cases **********************/

int test_read_file(char * input_filename, int * nx, int * ny)
{
    FILE * input;
    input = fopen(input_filename, "r");

    if (input == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int test_array_dimension(char * input_filename, int * nx, int * ny)
{
    FILE * input;
    input = fopen(input_filename, "r");

    if (input == NULL)
    {
        return 0;
    }

    fscanf(input, "%d %d\n", nx, ny);

    if (*nx <= 0 || *ny <= 0)
    {
        return 0;
    }

    return 1;
}

void test_input_file_not_found()
{
    int NX, NY;
    char * input_file = "notfound.txt";

    CU_ASSERT_EQUAL(test_read_file(input_file, &NX, &NY), 0);
}

void test_input_file_found()
{
    int NX, NY;
    char * input_file = "test/test_input.dat";

    CU_ASSERT_EQUAL(test_read_file(input_file, &NX, &NY), 1);
}

void test_array_dimension_not_positive_integer()
{
    int NX, NY;
    char * input_file = "test/test_input_false.dat";

    CU_ASSERT_EQUAL(test_array_dimension(input_file, &NX, &NY), 0);
}

void test_array_dimension_positive_integer()
{
    int NX, NY;
    char * input_file = "test/test_input.dat";

    int **map = input_map(input_file, &NX, &NY);

    CU_ASSERT_EQUAL(NX, 3);
    CU_ASSERT_EQUAL(NY, 3);
}

void test_init_map_success()
{
    int NX, NY;
    char * input_file = "test/test_input.dat";

    int **map = input_map(input_file, &NX, &NY);

    CU_ASSERT_EQUAL(NX, 3);
    CU_ASSERT_EQUAL(NY, 3);
    CU_ASSERT_EQUAL(map[1][1], 1);
    CU_ASSERT_EQUAL(map[1][2], 0);
    CU_ASSERT_EQUAL(map[1][3], 0);
    CU_ASSERT_EQUAL(map[2][1], 0);
    CU_ASSERT_EQUAL(map[2][2], 0);
    CU_ASSERT_EQUAL(map[2][3], 1);
    CU_ASSERT_EQUAL(map[3][1], 1);
    CU_ASSERT_EQUAL(map[3][2], 1);
    CU_ASSERT_EQUAL(map[3][3], 0);
}


/* add a suite to the registry */
int input_test_add_suite()
{
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite( "input_test_suite", init_input_test, clean_input_test);
    if ( NULL == pSuite )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add tests to suite. */
    if ( (NULL == CU_add_test(pSuite, "Test input file is not found",  test_input_file_not_found)) ||
         (NULL == CU_add_test(pSuite, "Test input file is found", test_input_file_found)) ||
         (NULL == CU_add_test(pSuite, "Test array dimension is not positive integer", test_array_dimension_not_positive_integer)) ||
         (NULL == CU_add_test(pSuite, "Test array dimension is positive integer", test_array_dimension_positive_integer)) ||
         (NULL == CU_add_test(pSuite, "Test map initialisation is successful", test_init_map_success))
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return 0;
}

