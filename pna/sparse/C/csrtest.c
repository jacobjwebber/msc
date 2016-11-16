#include "matrix.h"

int main()
{
  
  /* The matrices */
  COOmatrix A;
  CSRmatrix B;
  CSRmatrix apples;
  double b[2];
  double u[2];

  /* pointers to the test vectors */
  double *v, *w1, *w2;
  FILE *fp;

  char fileName[100];
  int dataset;
  int i,j;

  double vnorm;


  printf("csrtest:Which dataset (1/2/3)? ");
  scanf("%d",&dataset);
  printf("\n");
  if( (dataset<1) || (dataset>3) ){
    fprintf(stderr,"csrtest:Error:Input out of range [%d]\n",dataset);
    return 1;
  }

  sprintf(fileName,"../COOInput/random_matrix%d.dat",dataset);
  printf("csrtest:Reading COO data%s\n",fileName);

  read_COO(fileName, &A); 

  print_COO(&A);

  printf("csrtest:Converting to CSR format \n");

  setup_CSR(&B,A.nrow,A.ncol,A.nnz);

  populate_CSR(&B,&A);

  print_CSR(&B);

  /* now allocate the vectors */
  /* v is column vector */
  v = (double *)malloc( (sizeof(double))*B.ncol);
  /* w1 and w2 are row vectors */
  w1 = (double *)malloc( (sizeof(double))*B.nrow);
  w2 = (double *)malloc( (sizeof(double))*B.nrow);

  sprintf(fileName,"../COOInput/vector%d.dat",dataset);
  printf("csrtest:Reading vector data%s\n",fileName);  

  fp=fopen(fileName,"r");
  for(i=0;i<B.nrow;i++){
    fscanf(fp,"%lf %lf",&v[i],&w1[i]);
  }
  fclose(fp);

  printf("csrtest:calling vector-matrix multiply\n");

  vector_matrix(&B, v, w2);

  for(i=0;i<B.nrow;i++){
    printf("%g %g\n",w1[i],w2[i]);
  }

  vnorm=vector_norm(w1,B.nrow);
  printf("csrtest:L2 norm of w1 = %g\n",vnorm);

  vnorm=vector_norm(w2,B.nrow);
  printf("csrtest:L2 norm of w2 = %g\n",vnorm);
  
  return 0;
}


