#include <stdio.h>

int main()
{

    int array[10][10];
    int i,j;

    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            array[i][j] = 10*i + j;
        }
    }

    int k=0;
    for (k=0;k<10;k++)
    {
        for (j=0;j<10;j++)
        {
            printf("%i\t",array[k][j]);
        }
        printf("\n");
    }

    return 0;
}

