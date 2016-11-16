package sparse;

import java.text.DecimalFormat;

/** File CSRMatrix.java 2001/01/23

 Modified 2004/02/09 -- updated for 2004 lecture course.

 Written by George Beckett (g.beckett@epcc.ed.ac.uk)

Basic CSR sparse matrix class with simple routines for creating and
manipulating CSR formatted sparse matrices.

Methods implemented include: - 
CSRMatrix(int r, int c, int n)
                - constructor method

	*** Following are to be completed ***
int populateMatrix(int row[], int col[], double data[])
                - populate matrix from COO dataset
int timesv(double[] v, double[] w)
                - perform matrix-vector multiplication
lincg(double b[], double x[], double tol, int maxIter)
                - Conjugate Gradient method
	*** End of methods that need to be completed ***

int getM()      - retrieve number of rows in matrix
int getN()      - retrieve number of columns in matrix
int getNzmax()  - retrieve capacity of matrix
int getNnz()    - retrieve number of nonzero elements
void dispMatrix()
                - display matrix

Private methods: -
int validateCOO(int row[], int col[], double data[])
                - validate COO data
double norm(double v[])
                - compute the L2 norm of a vector.
double normInf(double v[])
                - compute the infinity norm of a vector.
double dotProduct(double v[], double w[])
                - compute dot product of two vectors.
*/

public class CSRMatrix{
    private final int nrow;     // number of rows
    private final int ncol;     // number of columns
    private final int nzmax;    // maximum number elements

    public int[] rowStart;           // row index information
    public int[] colIdx;           // column index information
    public double[] value;         // data entries

    /** public CSRMatrix(int r, int c, int n) - only sparse matrix
	constructor. Inputs r = number of rows, c = number of columns,
	n = maximum number of entries (capacity). Basic error validation
	is performed to check that input arguments are at least logical!

        Failure is indicated by a zero capacity matrix being returned.
    */
    public CSRMatrix(int r, int c, int n){
	int errCode = 0;          // error code

	/* matrix must have at least one row and one column... */
	if((r < 1) || (c < 1)){
	    System.out.println("CSRMatrix(): Row/ column data nonsense.");
	    errCode = 1;
	}
	/* ... and an appropriate number of nonzero entries */
	if((n < 1) || (n > ((long)r)*((long)c))){
	    System.out.println("CSRMatrix(): Capacity data is nonsense.");
	    errCode = 2;
	}

	/* if error validation fails we return an empty matrix of
	   dimension 0x0. */
	if(errCode != 0){
	    nrow = 0;
	    ncol = 0;
	    nzmax = 0;

	    return;
	}

	/* okay to define matrix dimensions/ capacity and allocate memory
	   for contents. Note that the matrix is empty. */
	nrow = r; ncol = c; nzmax = n;

	rowStart = new int[nrow+1];
	colIdx = new int[nzmax];
	value = new double[nzmax];

	return;
    }

    /* ****************************************************************
       public int populateMatrix(int row[], int col[], double data[])

       This method populates the matrix with data entries supplied in
       COO format.

       Parameters (I/O)
       row[]      row index of each element
       col[]      column index of each element
       data[]     contents of matrix locations

       Returns:
       int        zero if successful, non-zero otherwise.

       N.B. It may only be used to populate an empty matrix. Data must be
       supplied, sorted by row and then by column.
    */
    public int populateMatrix(int row[], int col[], double data[]){

	/* Firstly, we check that the matrix is empty ... */
	if(this.getNnz() != 0){
	    System.out.println("populateMatrix: matrix must be empty.");
	    return(1);
	}

	/* ... and that the input data is appropriate. */
	if(validateCOO(row, col, data) != 0){
	    System.out.println("Invalid user data specified.");
	    return(1);
	}

        /* Insert code to copy COO matrix into CSR matrix here */
	System.out.println("*** populateMatrix method is doing " +
			   "nothing. ***");



	return(0);
    }


    /* ****************************************************************
       public int timesv(double[] v, double[] w)

       Performs matrix-vector multiplication A.v and stored answer in
       vector w.

       Parameters (I/O)
       v[]        vector to be multiplied by A
       w[]        vector that will contain the answer, w=A.v

       Returns:
       int        zero if successful, non-zero otherwise

    ****************************************************************/
    public int timesv(double[] v, double[] w){


        /* Insert matrix-vector multiplication code here */
	System.out.println("*** timesv method is doing nothing. ***");





	return 0;
    }


    /* ****************************************************************
       lincg(double b[], double x[], double tol, int maxIter){

       Computes CG iterative solution of the matrix system A.x = b. To
       achieve convergence it is normally necessary that A is a
       symmetric positive definite matrix. Algorithm is as described
       in Numerical Recipes in C.

       Parameters (I/O)
       b          righthand vector (I)
       x          initial guess/ final solution (I/O)
       tol        L_2 convergence tolerance
       maxIter    maximum number of CG iterations allowed

       Returns
       int        zero if convergence occured, non-zero otherwise
    ****************************************************************/
    public int lincg(double b[], double x[], double tol,
		     int maxIter){

	System.out.println("*** lincg method compute Conjugate " 
	                   + "Gradient solution of a CSR linear system.");
	System.out.println("*** currently does nothing.");

	return 0;


    }







    /* YOU DO NOT NEED TO MAKE MODIFICATIONS BEYOND THIS POINT */







    /** Accessor function which returns the number of rows in the matrix
     */
    public int getM(){
	return(nrow);
    }

    /** Accessor function which returns the number of columns in the
	matrix */
    public int getN(){
	return(ncol);
    }

    /** Accessor function which returns the capacity of the matrix */
    public int getNzmax(){
	return(nzmax);
    }

    /** Accessor function which returns the number of nonzeros in the
	matrix */
    public int getNnz(){
	return(rowStart[nrow]);
    }

    /** Function which displays matrix on standard output.
     */
    public void dispMatrix(){
	DecimalFormat shortFormat = new DecimalFormat(" #0.000;-#0.000");

	int row;  // current row
	int col;  // current column
	int idx;  // current data element

	// Step through the rows one at a time.
	for(row = 0; row < nrow; row++){
	    /* Step along columns */
	    col = 0;

	    /* if (row,col) is empty print zero and move on... */
	    for(idx = rowStart[row]; idx < rowStart[row+1]; idx++){
		while(col < colIdx[idx]){
		    String entry = shortFormat.format(0.0);
		    System.out.print(" " + entry + " ");
		    col++;
		}

		/* ... until we find a nonzero entry. */
		String entry = shortFormat.format(value[idx]);
		System.out.print(" " + entry + " ");
		col++;
	    }
    
	    /* Then fill in any trailing zeros on this row before
	       advancing to the next.
	    */
	    while(col < ncol){
		String entry = shortFormat.format(0.0);
		System.out.print(" " + entry + " ");
		col++;
	    }
      
	    /* Advance print position to new line. */
	    System.out.println("");
	}

	return;
    }

    /*  ****************************************************************
	Small internal method to check that a set of input data is
	valid. 
    */
    private int validateCOO(int row[], int col[], double data[]){
	int noEntries;

	/* Check that the input data is appropriate. */
	noEntries = row.length;

	if((col.length !=  noEntries) | (data.length != noEntries)){
	    System.out.println("populateMatrix: inconsistent data supplied.");
	    return(1);
	}

	if(this.nzmax < noEntries){
	    System.out.println("populateMatrix: insufficient capacity.");
	    return(1);
	}

	/* Now, check that the input data is sorted - this routine also
	   eliminates duplicate index pairs, or out-of-bounds entries. */
	for(int i=0; i<noEntries-1; i++){
	    /* Check that index pair is valid */
	    if((row[i] < 0) || (row[i] >= this.nrow) || (col[i] < 0) ||
	       (col[i] >= this.ncol)){
		System.out.println("populateMatrix: index out of bounds.");
		return(2);
	    }

	    /* Check that it is locally sorted. */
	    if((row[i] > row[i+1]) ||
	       ((row[i] == row[i+1]) && (col[i] >= col[i+1]))){
		System.out.println("populateMatrix: data not sorted.");
		return(3);
	    }
	}

	return(0);
    }


    private static double norm(double v[]){
	double ans = 0.0;

	for(int j=0; j<v.length; j++)
	    ans += v[j]*v[j];

	return(Math.sqrt(ans));
    }


    private static double normInf(double v[]){
      double ans=Math.abs(v[0]);

      for(int j=1; j< v.length; j++){
	if(Math.abs(v[j]) > ans)
	  ans=Math.abs(v[j]);
      }

      return(ans);
    }

	
    private static double dotProduct(double v[], double w[]){
	double ans = 0.0;

	if(v.length != w.length){
	    System.out.println("dotProduct(): vectors must be of " +
			       "same length.");
	    return(0.0);
	}

	for(int j=0; j<v.length; j++)
	    ans += v[j]*w[j];

	return(ans);
    }
}
