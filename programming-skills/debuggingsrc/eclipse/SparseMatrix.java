import java.text.*;
import java.util.*;
import java.lang.Math.*;

/*
 * Debugging exercise for HPC Architectures Course
 * Adrian Jackson, EPCC, 2013
 * Derived from code from George Beckett, EPCC, 2002
 */

public class SparseMatrix{


	private static final int SUCCESS = 0;
	private static final int FAILURE = 1;

	private static int L = 0;

	private final int nRow;
	private final int nCol;
	private final int nzMax;
	private static int nnz;

	public int rowStart[];
	public int column[];
	public double value[];

	public SparseMatrix(int rows, int cols, int nzmax){
		nRow = rows;
		nCol = cols;
		nzMax =  nzmax;

		rowStart = new int[nRow+1];
		column = new int[nzMax];
		value = new double[nzMax];

		nnz = 0;
	}

	public int getNRows(){
		return(nRow);
	}

	public int getNCols(){
		return(nCol);
	}

	public int getNnz(){
		return(nnz);
	}

	public void setNnz(int value){
		nnz = value;
	}

	public int getNzmax(){
		return(nzMax);
	}

	public int populateMatrix(int row[], int col[], double data[]){

		int previous, j;

		nnz = data.length;

		if(row.length != nnz){
			System.out.println("failed");
			return FAILURE;
		}
		if(col.length != nnz){
			System.out.println("failed");
			return FAILURE;
		}

		for(int i=0; i<nnz; i++){
			if(col[i] < nnz && col[i] >= 0){
				column[i] = col[i];
				value[i] = data[i];
			}else{
				System.out.println("failed");
				return FAILURE;
			}

		}

		j = 0;
		previous = -1;
		rowStart[j] = 0;
		j++;
		for(int i=0; i<nnz-1; i++){
			previous = row[i];
			if(row[i+1]!=previous){
				rowStart[j]=i+1;
				j++;
			}
		}

		rowStart[nRow] = nnz;

		return SUCCESS;

	}


	public int timesv(double v[], double w[]){


		for(int i=0; i<nRow; i++){
			w[i] = 0;
			for(int j=rowStart[i]; j<(rowStart[i+1]); j++){
				w[i] = w[i] + value[j] * v[column[j]];
			}
		}

		return SUCCESS;

	}


	private double L2norm(double v[]){

		double norm = 0.0;
		int iters = v.length;

		for(int i=0; i<iters; i++){
			norm += v[i]*v[i];
		}

		norm = Math.sqrt(norm);

		return norm;


	}


	private double timesvectors(double a[], double b[]){

		double product = 0.0;
		int iters = a.length;

		if(a.length != b.length){
			System.out.println("timesvectors failure");
			return -1;
		}

		for(int i=0; i<iters; i++){
			product += a[i]*b[i];
		}

		return product;

	}


//	****************************************************************
//	BEGINNING OF CREATEMATRIX METHOD
//	****************************************************************

	/* int createMatrix(int noRows, COOMatrix M, double v[], double w[])
	 *
	 * Class method to create a (nearly) random sparse matrix M in COO
	 * format and associated vectors v[] and w[] such that M.v=w.
	 *
	 * Parameters (I/O)
	 * noRows    - number of rows/ columns in matrix (I)
	 * M         - empty COO matrix of dimension noRows by noRows
	 * v[noRows] - preallocated for (noRows) doubles, returns
	 *             the vector v on exit (O)
	 * w[noRows] - preallocated for (noRows) doubles, returns
	 *             the vector w on exit (O)
	 *
	 * return value of method is number on nonzeros allocated if
	 * successful or zero otherwise.
	 */
	public static int createMatrix(int noRows, COOMatrix M,
			double v[], double w[]){
		int i;                  // 
		int j;                  // loop counters
		int k;                  //

		boolean duplicateEntry; // Flag for duplicate column indices
		boolean colSorted;      // Flag for column data sorting

		int[] nzRow=null; // stores the number of nonzero elements in
		// each row
		int noNonzeros;   // number of nonzeros in matrix

		int index;        // used as indexes for COO format
		int idxStore;     //

		int currentCol;   // candidate column index
		double entry;     // current entry being computed

		/* First task is to work out how many nonzeros will be in the
		 * final matrix. This is done on a row-by-row basis, with the
		 * restriction that there must be at least one nonzero entries
		 * in each row.
		 */
		nzRow = new int[noRows];
		noNonzeros = 0;

		for(i=0; i<noRows; i++){
			nzRow[i] = 1 + (int)((noRows-1)*Math.random()+0.5);
			noNonzeros += nzRow[i];
		}

		/* Now we can allocate space for the COOMatrix. */
		M.allocateSpace(noNonzeros);

		/* Now, it is time to populate the matrix. Again, we work on a
		 * row-by-row basis. In each row, we compute random column
		 * entries one at a time, checking that we have not duplicated
		 * a previous entry. There is probably a better way to do
		 * this! 
		 */
		index = 0; // Will store the location of the current element
		// in the matrix.
		currentCol = 0;

		for(i=0; i<noRows; i++){
			idxStore = index; // store index of start of current row.

			for(j=0; j<nzRow[i]; j++){
				duplicateEntry = true;

				while(duplicateEntry == true){
					/* create candidate column index */
					currentCol = (int) (noRows*Math.random());

					/* check for duplicate entry */
					duplicateEntry=false;

					for(k=idxStore; k<index; k++){
						if(M.colIdx[k] == currentCol){
							duplicateEntry = true;
							break;
						}
					}
				}

				/* Create entry, check that it is not too close to
				 * zero, and store it */
				entry = 20.0*Math.random()-10.0;

				if(Math.abs(entry) < 1.0e-4)
					entry = entry+1.0;

				M.rowIdx[index] = i;
				M.colIdx[index] = currentCol;
				M.value[index]  = entry;
				index++;

				/* ... and move on to next column entry */
			}

			/* Now, we have to re-order the column entries. This is
			 * done using a very crude sorting algorithm, and could be
			 * improved. 
			 */
			colSorted = false;

			while(colSorted==false){
				colSorted = true;

				for(j=idxStore; j<index-1; j++){
					/* if this and next column are out of order,
					 * exchange them - note that we use currentCol and
					 * entry as temporary stores to save on memory.
					 */
					if(M.colIdx[j] > M.colIdx[j+1]){
						colSorted = false;

						currentCol = M.colIdx[j+1];
						M.colIdx[j+1] = M.colIdx[j];
						M.colIdx[j] = currentCol;

						entry = M.value[j+1];
						M.value[j+1] = M.value[j];
						M.value[j] = entry;

						break;
					}
				}
			}

			/* Also create corresponding entry in v[].
			 */
			v[i] = 20.0*Math.random()-10.0;
		}

		/* Check that the correct number of entries have been
		 * allocated. If not, then there is a programming error!
		 */
		if(index != noNonzeros){
			System.out.println("index = " + index + " nnz = " + 
					noNonzeros);
			System.out.println("createMatrix: error allocating matrix");
			return 0;
		}

		/* Finally, we have to create the righthand side vector.
		 */
		for(i=0; i<noRows; i++)
			w[i] = 0.0;

		for(j=0; j<noNonzeros; j++)
			w[M.rowIdx[j]] += M.value[j]*v[M.colIdx[j]];

		/* and we are done.
		 */

		return noNonzeros;
	}

//	****************************************************************
//	END OF CREATEMATRIX METHOD
//	****************************************************************


//	****************************************************************
//	BEGINNING OF FLUIDFLOW
//	****************************************************************

	/* int fluidFlow(int L, COOMatrix A, double b[],
	 *               double B, double W, double H)
	 *
	 * Class method to create a finite difference system for the
	 * solution of the `Forced fluid in a cavity' problem.
	 *
	 * Parameters (I/O)
	 * L               - number of grid points in each direction (I)
	 * A               - empty COO matrix of dimension (L-2)^2 by
	 *                   (L-2)^2. (I/O)
	 * b               - preallocated double array of length (L-2)^2
	 *                   (I/O) 
	 * B               - domain parameter: distance from x=0 of
	 *                   outflow (I)
	 * W               - domain parameter: width of inflow/ outflow
	 *                   (I) 
	 * H               - domain parameter: distance from y=0 of inflow
	 *                   (I)
	 *
	 * return value of method is number of nonzeros in COO matrix or
	 * zero if the allocation fails.
	 */

	public static int fluidFlow(int L, COOMatrix A, double b[]){
		final int N;           // number of mesh elements
		// in each direction

		int i;                 // Loop counters
		int j;                 //

		int noUnknowns;        // number of rows in matrix
		int noNonzeros;        // number of nonzeros in matrix
		int index;             // current element being processed

		final double B=0.2;    //
		final double W=0.1;    // domain dimensions data, c.f. practical
		final double H=0.7;    //

		double x;              // x and y data at current coordinate.
		double y;              //

		double coeffC;         //
		double coeffN;         // temporary stores for coefficient
		double coeffE;         // data
		double coeffS;         //
		double coeffW;         //

		int RC;                //
		int RN;                //
		int RE;                // local index on interior mesh
		int RS;                //
		int RW;                //

		/* we compute these to save time later */
		N = L-1;
		noUnknowns = (N-1)*(N-1);
		noNonzeros = 5*(N-3)*(N-3) + 16*(N-3) + 12;

		/* allocate space for nonzeros in the matrix */
		A.allocateSpace(noNonzeros);

		/* Now, it is time to populate the matrix. Again, we work on a
		 * row-by-row basis. In each row, we compute random column
		 * entries one at a time, checking that we have not duplicated
		 * a previous entry. There is probably a better way to do
		 * this! 
		 */
		index = 0; // Will store the location of the current element
		// in the matrix.
		for(j=0; j<N-1; j++){
			for(i=0; i<N-1; i++){
				x = ((double) i+1.0)/((double) N);
				y = ((double) j+1.0)/((double) N);

				RC = i+j*(N-1);
				RN = i+(j+1)*(N-1);
				RE = (i+1)+j*(N-1);
				RS = i+(j-1)*(N-1);
				RW = (i-1)+j*(N-1);

				coeffC = 4.0*N*N;
				coeffN = -N*N;
				coeffE = -N*N;
				coeffS = -N*N;
				coeffW = -N*N;

				/* Compute righthand side, */
				b[RC] = 0.0;

				/* Now insert coefficients into the matrix, modifying
		   the allocation for boundary terms - there are nine
		   different cases corresponding to the interior, the
		   sides and the corners of the solution domain. Local
		   quantities are referenced using the compass
		   notation. */
				if(i>0 && i<(N-2) && j>0 && j<(N-2)){
					// interior of the domain
					A.rowIdx[index] = RC;
					A.colIdx[index] = RS;
					A.value[index] = coeffS;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RW;
					A.value[index] = coeffW;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RC;
					A.value[index] = coeffC;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RE;
					A.value[index] = coeffE;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RN;
					A.value[index] = coeffN;
					index++;
				}
				else if(i==0 && j>0 && j<(N-2)){
					// west boundary
					A.rowIdx[index] = RC;
					A.colIdx[index] = RS;
					A.value[index] = coeffS;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RC;
					A.value[index] = coeffC;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RE;
					A.value[index] = coeffE;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RN;
					A.value[index] = coeffN;
					index++;
				}
				else if(i==(N-2) && j>0 && j<(N-2)){
					// east boundary
					A.rowIdx[index] = RC;
					A.colIdx[index] = RS;
					A.value[index] = coeffS;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RW;
					A.value[index] = coeffW;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RC;
					A.value[index] = coeffC;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RN;
					A.value[index] = coeffN;
					index++;

					// Check to see if we are at the inflow
					if(y<=H)
						b[RC] = -coeffE*W;
					else if(y<(H+W))
						b[RC] = -coeffE*(-y+(H+W));
				}
				else if(i>0 && i<(N-2) && j==0){
					// south boundary
					A.rowIdx[index] = RC;
					A.colIdx[index] = RW;
					A.value[index] = coeffW;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RC;
					A.value[index] = coeffC;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RE;
					A.value[index] = coeffE;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RN;
					A.value[index] = coeffN;
					index++;

					// Check to see if we are at the outflow
					if(x>(B+W))
						b[RC] = -coeffS*W;
					else if(x>B)
						b[RC] = -coeffS*(x-B);
				}
				else if(i>0 && i<(N-2) && j==(N-2)){
					// north boundary
					A.rowIdx[index] = RC;
					A.colIdx[index] = RS;
					A.value[index] = coeffS;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RW;
					A.value[index] = coeffW;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RC;
					A.value[index] = coeffC;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RE;
					A.value[index] = coeffE;
					index++;
				}
				else if(i==0 && j==0){
					// South-west corner
					A.rowIdx[index] = RC;
					A.colIdx[index] = RC;
					A.value[index] = coeffC;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RE;
					A.value[index] = coeffE;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RN;
					A.value[index] = coeffN;
					index++;
				}
				else if(i==(N-2) && j==0){
					// South-east corner
					A.rowIdx[index] = RC;
					A.colIdx[index] = RW;
					A.value[index] = coeffW;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RC;
					A.value[index] = coeffC;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RN;
					A.value[index] = coeffN;
					index++;

					b[RC] = -(coeffE+coeffS)*W;
				}
				else if(i==0 && j==(N-2)){
					// North-west corner
					A.rowIdx[index] = RC;
					A.colIdx[index] = RS;
					A.value[index] = coeffS;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RC;
					A.value[index] = coeffC;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RE;
					A.value[index] = coeffE;
					index++;
				}
				else if(i==(N-2) && j==(N-2)){
					// North-east corner
					A.rowIdx[index] = RC;
					A.colIdx[index] = RS;
					A.value[index] = coeffS;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RW;
					A.value[index] = coeffW;
					index++;

					A.rowIdx[index] = RC;
					A.colIdx[index] = RC;
					A.value[index] = coeffC;
					index++;
				}
				else{
					// This case should never occur!
					System.out.println("Error in coordinate allocation.");
					return 0;
				}
			}
		}

		/* Check that the correct no. elements have been
		 * allocated. This also, should never occur */
		if(index != noNonzeros){
			System.out.println("index = " + index + " nnz = " + noNonzeros);
			System.out.println("Error - incorrect no. entries.");
			return 0;
		}

		return(noNonzeros);
	}

//	****************************************************************
//	END OF FLUIDFLOW METHOD
//	****************************************************************

//	****************************************************************
//	BEGINNING OF DISPLAY MATRIX METHOD
//	****************************************************************

	/** Function which displays matrix on standard output.
	 */
	public void dispMatrix(){
		DecimalFormat shortFormat = new DecimalFormat("##0.00E0");

		int row;  // current row
		int col;  // current column
		int idx;  // current data element

		// Step through the rows one at a time.
		for(row = 0; row < nRow; row++){
			/* Step along columns */
			col = 0;

			/* if (row,col) is empty print zero and move on... */
			for(idx = rowStart[row]; idx < rowStart[row+1]; idx++){
				while(col < column[idx]){
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
			while(col < nCol){
				String entry = shortFormat.format(0.0);
				System.out.print(" " + entry + " ");
				col++;
			}

			/* Advance print position to new line. */
			System.out.println("");
		}

		return;
	}

//	****************************************************************
//	END OF DISPLAY MATRIX METHOD


}



