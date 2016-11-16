#include "matrix.h"

int main()
{

/* set some parameters */
  const int M = 7; /* grid resultion */
  const int maxIter = 5000;
  const double tol = 1.0e-6;
  /* boundary conditions */
  const double y1 = 0.4;
  const double y2 = 0.6;
  const int p = 2;
  double k;

  /* indices and counters */
  int usedIter;
  int index; /* index of current element */
  int i; /* current i coordinate */
  int j; /* current j coordinate */

  double x,y; /* x and y at current coordinate */
  
  /* temporary store for coefficient data */
  double coeffC, coeffN, coeffE, coeffS, coeffW;

  /* local indices on interior mesh*/ 
  int RC, RN, RE, RS, RW;

  /* The linear sytem */
  int noUnknowns;
  int noNonZeros;

  /* The coefficient matrix */
  CSRmatrix A;

  /* pointers to the vectors */
  double *b, *u;

  FILE *fptr;

  /* starting the execution */

  /* calculate some derived parameters */
  /* k normalises the height of the chimney to 1 */
  k=pow( (2.0/(y2-y1)),(2*p) );
  /* the system size */ 
  noUnknowns = M*M;
  noNonZeros = 5*(M-2)*(M-2) + 16*(M-2) + 12;

  /* allocate the data objects */
  setup_CSR(&A, noUnknowns, noUnknowns, noNonZeros);
  b = (double *)malloc( (sizeof(double))*noUnknowns);
  u = (double *)malloc( (sizeof(double))*noUnknowns);


  /* set the boundary conditions */
  coeffC = 4.0 * (double)(M+1)*(M+1);
  coeffN=coeffE=coeffS=coeffW=-(double)(M+1)*(M+1);

  for(index=0;index<noUnknowns;index++){
    b[index]=0.0;  /* solving a homogenous eqn, i.e. RHS=0 */
                   /* chimney is added by hand later */
    u[index]=0.0;  /* initial trial solution is zero */ 
  }

  /* 
   * Populate the matrix by hand 
   * stepping along each row of the u-field in the x-direction
   * column by column 
   */

  index=0;

  for(j=0;j<M;j++){ /* looping over the rows if the u-field */
    y=(double)(j+1)/(double)(M+1);

    for(i=0;i<M;i++){ /* looping over the rows if the u-field */
      x=(double)(i+1)/(double)(M+1);
 
      /* converting the two-dimensional indices into 
       * one dimensional index */ 
      RC = i + j*M;

      /* Lookup table for ordinal neighbours */
      RN=RC+M;
      RE=RC+1;
      RS=RC-M;
      RW=RC-1;
      /* set the row-start */
      A.rowStart[RC]=index;
      
      /*
       * Now insert coefficients into the matrix, 
       * modifying the allocation for boundary terms
       * 
       * There are nine different cases corresponding to
       * the interior, the sides and corners of the u-field
       */

      /* interior */
      if( (i>0) && (i<(M-1)) && (j>0) && (j<(M-1)) ){
	A.colIndex[index] = RS;
	A.value[index]=coeffS;
	index++;

	A.colIndex[index] = RW;
	A.value[index]=coeffW;
	index++;

	A.colIndex[index] = RC;
	A.value[index]=coeffC;
	index++;

	A.colIndex[index] = RE;
	A.value[index]=coeffE;
	index++;

	A.colIndex[index] = RN;
	A.value[index]=coeffN;
	index++;
      }
      else if( (i==0) && (j>0) && (j<(M-1))){ /* west boundary */
	A.colIndex[index] = RS;
	A.value[index]=coeffS;
	index++;

	A.colIndex[index] = RC;
	A.value[index]=coeffC;
	index++;

	A.colIndex[index] = RE;
	A.value[index]=coeffE;
	index++;

	A.colIndex[index] = RN;
	A.value[index]=coeffN;
	index++;
      }
      else if( (i==(M-1)) && (j>0) && (j<(M-1)) ){ /* East boundary */

	A.colIndex[index] = RS;
	A.value[index]=coeffS;
	index++;

	A.colIndex[index] = RW;
	A.value[index]=coeffW;
	index++;

	A.colIndex[index] = RC;
	A.value[index]=coeffC;
	index++;

	A.colIndex[index] = RN;
	A.value[index]=coeffN;
	index++;

	/* if we need to add the pollution source to the boundary condition */
	if( (y>y1) && (y<y2)){
	  b[RC]= -coeffE * k * pow( ( (y2-y)*(y-y1) ), p);
	}

      }
      else if( (i>0) && (i<(M-1)) && (j==0) ){ /* South boundary */

	A.colIndex[index] = RW;
	A.value[index]=coeffW;
	index++;

	A.colIndex[index] = RC;
	A.value[index]=coeffC;
	index++;

	A.colIndex[index] = RE;
	A.value[index]=coeffE;
	index++;

	A.colIndex[index] = RN;
	A.value[index]=coeffN;
	index++;
      }
      else if( (i>0) && (i<(M-1)) && (j==(M-1)) ){ /* north boundary */
	A.colIndex[index] = RS;
	A.value[index]=coeffS;
	index++;

	A.colIndex[index] = RW;
	A.value[index]=coeffW;
	index++;

	A.colIndex[index] = RC;
	A.value[index]=coeffC;
	index++;

	A.colIndex[index] = RE;
	A.value[index]=coeffE;
	index++;

      }
      else if( (i==0) && (j==0) ){ /* SW corner */
	A.colIndex[index] = RC;
	A.value[index]=coeffC;
	index++;

	A.colIndex[index] = RE;
	A.value[index]=coeffE;
	index++;

	A.colIndex[index] = RN;
	A.value[index]=coeffN;
	index++;
      }
      else if( (i==(M-1)) && (j==0) ){ /* SE corner */
	A.colIndex[index] = RW;
	A.value[index]=coeffW;
	index++;

	A.colIndex[index] = RC;
	A.value[index]=coeffC;
	index++;

	A.colIndex[index] = RN;
	A.value[index]=coeffN;
	index++;
      }
      else if( (i==0) && (j==(M-1)) ){ /* NW corner */
	A.colIndex[index] = RS;
	A.value[index]=coeffS;
	index++;

	A.colIndex[index] = RC;
	A.value[index]=coeffC;
	index++;

	A.colIndex[index] = RE;
	A.value[index]=coeffE;
	index++;
      }
      else if( (i==(M-1)) && (j==(M-1)) ){ /* NE corner */
	A.colIndex[index] = RS;
	A.value[index]=coeffS;
	index++;

	A.colIndex[index] = RW;
	A.value[index]=coeffW;
	index++;

	A.colIndex[index] = RC;
	A.value[index]=coeffC;
	index++;
      }
      else{
	fprintf(stderr,"pollution:Error:Building matrix out of bounds [%d][%d]\n",i,j);
	return 1;
      }

    } /* end loop over i */
  } /* end loop over j */

  /* check the correct number of elements have been allocated */
  if(index==noNonZeros){
    A.rowStart[A.nrow]=index;
  }
  else{
    fprintf(stderr,"pollution:Error:wrong number of elements assigned index=%d nnz=%d\n",index,noNonZeros);
    return 1;
  }

  print_CSR(&A);
  
  linearCG_CSR(&A,b,u,tol,maxIter,&usedIter);

  printf("%s:Solution found in %i iterations\n",__FUNCTION__,usedIter);
  printf("%s:Writing solution to pollution.dat\n",__FUNCTION__);

  fptr=fopen("pollution.dat","w");
  for(j=0;j<=M+1;j++){
    y=(double)j/(double)(M+1);
    for(i=0;i<=M+1;i++){
      x=(double)i/(double)(M+1);
      /* If interior point write u-fields */
      if( (i>0) && (i<M+1) && (j>0) && (j<M+1) ){
	RC=i+(j-1)*M-1;
	  fprintf(fptr,"%10.6f\n",u[RC]);
      }
      else if( i==M+1 ){
	/* east boundary put the source */
	if( (y>y1) && (y<y2) ){
      	  fprintf(fptr,"%10.6f\n",k*pow( (((y-y2)*(y-y1))) , p )); 
	} 
	else{ 
      	  fprintf(fptr,"%10.6f\n",0.0); 
	} 
      } 
      else{ 
	fprintf(fptr,"%10.6f\n",0.0); 
      }
    }
  }
  fclose(fptr);

  printf("%s:Writing associated field file to polluton.general\n",__FUNCTION__);
  fptr=fopen("pollution.general","w");
  fprintf(fptr,"file = pollution.dat\n");
  fprintf(fptr,"grid = %d x %d\n",M+2,M+2);
  fprintf(fptr,"format = ascii\n");
  fprintf(fptr,"interleaving = record\n");
  fprintf(fptr,"majority = row\n");
  fprintf(fptr,"field = field0\n");
  fprintf(fptr,"structure = scalar\n");
  fprintf(fptr,"type = float\n");
  fprintf(fptr,"dependency = positions\n");
  fprintf(fptr,"positions = regular, regular, 0, 1, 0, 1\n");
  fprintf(fptr,"\nend\n");
  fclose(fptr);



  return 0;
}
