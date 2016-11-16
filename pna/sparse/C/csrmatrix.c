#include "matrix.h"

#undef DEBUG

void setup_CSR(CSRmatrix *csr, int r, int c, int nmax) 
{
  /* 
   * csr is the matrix to be setup
   * r is number of rows
   * c is number of columns 
   * nmax is max number of non-zero entries 
   */
  int i;

  csr->nrow=r;
  csr->ncol=c;
  csr->nzmax=nmax;

  csr->rowStart = (int *)malloc( (sizeof(int)*csr->nrow+1));
  csr->colIndex = (int *)malloc( (sizeof(int)*csr->nzmax));
  csr->value = (double *)malloc( (sizeof(double)*csr->nzmax));

  /* set the vectors to zero */
  for(i=0;i<csr->nrow;i++){
    csr->rowStart[i]=0; 
  }
  for(i=0;i<csr->ncol;i++){
    csr->colIndex[i]=0; 
  }
  for(i=0;i<csr->nzmax;i++){
    csr->value[i]=0.0;
  }

}

void populate_CSR(CSRmatrix *csr, COOmatrix *coo){
   /* this needs to be implemented */

  printf("populate_CSR:Function needs to be implemented\n");

}

void print_CSR(CSRmatrix *csr){
  int count=0;
  int row,col;
  printf("populate_CSR:Printing matrix\n");
  for(row=0;row<csr->nrow;row++){
    for(col=0;col<csr->ncol;col++){
      if( (csr->colIndex[count]==col) &&
	  (count<csr->rowStart[row+1])){
	printf("%g ",csr->value[count]);
	count++;
      }
      else{
	printf("0.0 ");
      }
    }
    printf("\n");
  }
}

void vector_matrix(CSRmatrix *A, double *v, double *b)
{
  /* evaluates A.v=b  */
  printf("vector_matrix:Function needs to be implemented\n");
}

double vector_norm(double *v, int len)
{
  int i;

  double norm=0.0;
  
  /* sum the contributions */
  for(i=0;i<len;i++){
    norm+=v[i]*v[i];
  }

  /* sqrt */
  norm=sqrt(norm);
  return norm;
}

double dot_prod(double *a, double *b, int len)
{
  double result=0.0;
  int i;
  
  for(i=0;i<len;i++){
    result+=a[i]*b[i];
  }
  return result;
}
  
void linearCG_CSR(CSRmatrix *M, double *b, double *x, 
		  double tol, int maxIter, int *usedIter){
  /* 
   *  Conjugate Gradient solver to solve
   *  M.x = b
   *  M has to be symmetric and positive definite
   *  b is row vector 
   *  x is column vector. x should be an initial guess
   *  tol is L2 convergence tolerance
   *  maxIter, usedIter (does what it says on the tin )
   */
  
  printf("linearCG_CSR:Function needs implementing\n");

  *usedIter = -1;

}
