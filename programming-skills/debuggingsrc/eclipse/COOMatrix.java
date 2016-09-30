import java.text.*;
import java.util.*;

/*
 * Debugging exercise for HPC Architectures Course
 * Adrian Jackson, EPCC, 2013
 * Derived from code from George Beckett, EPCC, 2002
 */

public class COOMatrix{
	private final int nrow;       // number of rows
	private final int ncol;       // number of columns
	private int nnz;              // number of nonzero elements
	public int rowIdx[]=null;     // row index information
	public int colIdx[]=null;     // column index information
	public double value[]=null; // data entries

	public int GetM(){
		return(nrow);
	}

	public int GetN(){
		return(ncol);
	}

	public int GetNnz(){
		return(nnz);
	}

	public COOMatrix(int r, int c){
		int errCode = 0;          // error code

		/* matrix must have at least one row and one column... */
		if((r < 1) || (c < 1)){
			System.out.println("SparseMatrix(): Row/ column data nonsense.");
			errCode = 1;
		}

		/* if error validation fails we return an empty matrix of
	   dimension 0x0. */
		if(errCode != 0){
			nrow = 0;
			ncol = 0;
			nnz = 0;

			return;
		}

		nrow = r;
		ncol = c;
		nnz = 0;

		return;
	}

	public int allocateSpace(int n){
		/* only empty matrices may be allocated space. */
		if(nnz != 0)
			return 0;

		nnz = n;
		rowIdx =  new int[n];
		colIdx =  new int[n];
		value = new double[n];

		return n;
	}

	public void dispMatrix(){
		DecimalFormat shortFormat = new DecimalFormat("00.00");

		for(int i=0; i<nnz; i++)
			System.out.println(this.rowIdx[i] + "\t" + this.colIdx[i] +
					"\t" + shortFormat.format(this.value[i]));

		/* Advance print position by one line. */
		System.out.println("");

		return;
	}

}
