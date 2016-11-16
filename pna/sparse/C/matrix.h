#include <stdio.h>
#include <stddef.h>
#include <malloc.h>
#include <math.h>

/* typdef for csrmatrix */
typedef struct
{
  int nrow;
  int ncol;
  int nzmax;

  int *rowStart;
  int *colIndex;

  double *value;
}
  CSRmatrix;

typedef struct
{
  int nrow;   /* size of matrix */
  int ncol;
  /* number of non-zero entries, and thus size of vectors */
  int nnz; 


  int *rowIndex;
  int *colIndex;
  double *value;
  /* pointers to the vectors */
}
  COOmatrix;
  
void read_COO(char *fileName, COOmatrix *coo);

void print_COO(COOmatrix *coo);

void setup_CSR(CSRmatrix *csr, int r, int c, int nmax);

void populate_CSR(CSRmatrix *csr, COOmatrix *coo);

void print_CSR(CSRmatrix *csr);

void vector_matrix(CSRmatrix *A, double *v, double *b);

double vector_norm(double *v, int len);

double scalar_prod(double *a, double *b, int len);

void linearCG_CSR(CSRmatrix *M, double *b, double *x, 
		 double tol, int maxIter, int *usedIter);


