#include "matrix.h"

void read_COO(char *fileName, COOmatrix *coo){
  FILE *fptr;
  int i;

  /* Open the file and read the size of the matrix and 
   * number of non-zero elements */
  fptr=fopen(fileName,"r");
  fscanf(fptr,"%d %d %d",&(coo->nrow),&(coo->ncol),&(coo->nnz)); 


  /* allocate memory for the vectors of length nnz */
  coo->rowIndex = (int *)malloc( (sizeof(int)*coo->nnz) );
  coo->colIndex = (int *)malloc( (sizeof(int)*coo->nnz) );
  coo->value = (double *)malloc( (sizeof(double)*coo->nnz) );

  /* loop over nnz reading row, col and value*/
  for(i=0;i<coo->nnz;i++){ 
    fscanf(fptr,"%d %d %lf",&(coo->rowIndex[i]),
	   &(coo->colIndex[i]),
	   &(coo->value[i])); 


  /* c counts from zero to n-1, so decrement the row and
   * column index by 1 */
    coo->rowIndex[i]--;
    coo->colIndex[i]--;

  }


  fclose(fptr);

}

void print_COO(COOmatrix *coo){
  int rowCounter, colCounter,valueCounter;

  valueCounter=0;

  for(rowCounter=0;rowCounter<coo->nrow;rowCounter++){
    for(colCounter=0;colCounter<coo->ncol;colCounter++){
      if( (rowCounter==coo->rowIndex[valueCounter]) &&
	  (colCounter==coo->colIndex[valueCounter]) ){
	printf("%g ",coo->value[valueCounter]);
	valueCounter++;
      }
      else{
	printf("0.0 ");
      }
    }
    printf("\n");
  }
}


