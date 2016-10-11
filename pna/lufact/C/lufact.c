#include <stdio.h>
#include <math.h>

#define NMAX 100

void matprint(int n, float a[NMAX][NMAX]);
void vecprint(int n, float x[NMAX]);
void matvec(int n, float y[NMAX], float a[NMAX][NMAX], float x[NMAX]);
void matmat(int n, float a[NMAX][NMAX], float b[NMAX][NMAX], float c[NMAX][NMAX]);
void matgen(int n, float a[NMAX][NMAX], float b[NMAX]);


void main(void)
{
  float a[NMAX][NMAX], l[NMAX][NMAX], u[NMAX][NMAX], lu[NMAX][NMAX];
  float x[NMAX], b[NMAX], ax[NMAX];
  float y[NMAX];

  int i, j;

  int n = 4;  

  matgen(n, a, b);

  printf("\na\n\n");
  matprint(n, a);

  printf("\nb\n\n");
  vecprint(n, b);

  printf("\nNeed to perform LU factorisation of A\n");
  printf("Currently setting L=U=0\n");

  /*
   * Set l and u to zero
   */

  for (i=0; i < n; i++)
  {
    for (j=0; j < n; j++)
    {
      l[i][j] = 0.0;
      u[i][j] = 0.0;
    }
  }

  printf("\nl\n\n");
  matprint(n, l);

  printf("\nu\n\n");
  matprint(n, u);

  /*
   * Multiply the L and U factors together
   */

  matmat(n, lu, l, u);

  printf("\nlu\n\n");
  matprint(n, lu);
  
  printf("\nNeed to forward and back substitution to solve Ax=b\n");
  printf("Currently setting x=b\n");

  for (i=0; i < n; i++)
  {
    x[i] = b[i];
  }

  printf("\nx\n\n");
  vecprint(n, x);
  
  /*
   * Set y = A x and print out
   */

  matvec(n, ax, a, x);

  printf("\nAx\n\n");
  vecprint(n, ax);
}


/*  matprint
 *
 *  Prints a matrix of floats to the screen. Matrix is taken to be
 *  of size n x n although it must have been defined as size NMAX x NMAX
 *  where NMAX is a compile-time constant.
 */


void matprint(int n, float a[NMAX][NMAX])
{
  int i, j;

  if (n > 6)
  {
    printf("Matrix too large to print\n");
  }
  else
  {
    for (i=0; i < n; i++)
    {
      for (j=0; j < n; j++)
      {
        printf(" %12.8f", a[i][j]);
      }
      printf("\n");
    }
  }
}


/*  vecprint
 *
 *  Prints a vector of floats to the screen. Vector is taken to be
 *  of size n although it must have been defined as size NMAX
 *  where NMAX is a compile-time constant.
 */


void vecprint(int n, float x[NMAX])
{
  int i;

  if (n > 6)
  {
    printf("Vector too large to print\n");
  }
  else
  {
    for (i=0; i < n; i++)
    {
      printf(" %12.8f\n", x[i]);
    }
  }
}


/*  matvec
 *
 *  Sets y = a.x, where y and x are vectors of floats of length n, and a
 *  is an n x n matrix. All arrays must have been defined as size NMAX
 *  where NMAX is a compile-time constant.
 */

void matvec(int n, float y[NMAX], float a[NMAX][NMAX], float x[NMAX])
{
  int i, j;

  for (i=0; i < n; i++)
  {
    y[i] = 0.0;

    for (j=0; j < n; j++)
    {
      y[i] += a[i][j]*x[j];
    }
  }
}


/*  matmat
 *
 *  Sets a = b.c, where a, b and c are all n x n matrices of floats.
 *  All matrices must have been defined as size NMAX x NMAX where NMAX
 *  is a compile-time constant.
 */


void matmat(int n, float a[NMAX][NMAX], float b[NMAX][NMAX], float c[NMAX][NMAX])
{
  int i, j, k;

  for (i=0; i < n; i++)
  {
    for (j=0; j < n; j++)
    {
      a[i][j] = 0.0;

      for (k=0; k < n; k++)
      {
        a[i][j] += b[i][k]*c[k][j];
      }
    }
  }
}


/*  matgen
 *
 *  Constructs a random matrix of size n x n, and a solution vector b
 *  All arrays must have been defined as size NMAX where NMAX is a
 *  compile-time constant.
 */

void matgen(int n, float a[NMAX][NMAX], float b[NMAX])
{
	int init, i, j;

	init = 1325;
	for (j = 0; j < n; j++) {
		for (i = 0; i < n; i++) {
			init = 3125*init % 65536;
			a[i][j] = (init - 32768.0)/16384.0;
		}
	}
	for (i = 0; i < n; i++) {
          b[i] = 0.0;
	}
	for (j = 0; j < n; j++) {
		for (i = 0; i < n; i++) {
			b[i] = b[i] + a[i][j];
		}
	}
}
