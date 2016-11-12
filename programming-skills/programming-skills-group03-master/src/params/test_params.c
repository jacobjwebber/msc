#include <stdio.h>
#include "params.h"


void test1();
void test2();

int main()
{
   
    test1();
    test2();
    
    return 0;
}

void test1()
{
    config_params parameters;
    printf("let's get started\n");
    set_params(&parameters, "file name");

    printf("checking defaults have been passed in\n");
    printf("dt should be 0.4, is %f\n", parameters.dt);
}

void test2()
{
    config_params parameters;
    printf("let's get started\n");
    printf("checking params have been passed in\n");
    set_params(&parameters, "testfile.dat");

    printf("dt should be 99, is %f\n", parameters.dt);
}
