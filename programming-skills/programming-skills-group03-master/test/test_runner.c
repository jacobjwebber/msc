#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <stdio.h>
#include "params_test.h"
#include "matrix_model_functions_test.h"
#include "input_test.h"
#include "output_test.h"

/********* Test Runner Code **********/

int main()
{

    /* initialise CUnit test registry */
    if ( CUE_SUCCESS != CU_initialize_registry() )
    {
        return CU_get_error();
    }


    params_test_add_suite();
    model_test_add_suite();
    input_test_add_suite();



    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");
    int num_fails = CU_get_number_of_failures();
    CU_cleanup_registry();
    return num_fails;
}
