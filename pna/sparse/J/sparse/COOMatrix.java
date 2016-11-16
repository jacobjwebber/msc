package sparse;

import java.text.DecimalFormat;
import java.io.FileReader;
import java.io.StreamTokenizer;

/* Very small class of COO stored sparse matrices.
 */
public class COOMatrix{
    private int nrow;       // number of rows
    private int ncol;       // number of columns
    private int nnz;              // number of nonzero elements
    public int rowIdx[];          // row index information
    public int colIdx[];          // column index information
    public double value[];        // data entries

    public int getM(){
	return(nrow);
    }

    public int getN(){
	return(ncol);
    }

    public int getNnz(){
	return(nnz);
    }

    public COOMatrix(){
	// return an entry matrix
	nrow = 0;
	ncol = 0;
	nnz = 0;
	
	return;
    }

    public int readCOO(String fileName){

	try{
	    FileReader fileReader = new FileReader(fileName); 
	    StreamTokenizer stream = new StreamTokenizer(fileReader);
	    stream.parseNumbers();
	    stream.nextToken();
	    nrow=(int)stream.nval;
	    stream.nextToken();
	    ncol=(int)stream.nval;
	    stream.nextToken();
	    nnz=(int)stream.nval;

	    // allocate the space 
	    allocateSpace();


	    /* loop over the file and assign the data */
	    for(int i=0;i<nnz;i++){

		// java counts from 0 to n-1
		// and files count from 1 to n
		// hence decrement value by 1

		stream.nextToken();
		rowIdx[i]=(int)stream.nval-1;

		stream.nextToken();
		colIdx[i]=(int)stream.nval-1;

		stream.nextToken();
		value[i]=stream.nval;

	    }

	    fileReader.close();
	}
	catch(Exception e){
	    System.err.println("COOMatrix.readCOO:Error:" + e.getMessage());
	}
	return nnz;
    }


    public void allocateSpace(){

	rowIdx =  new int[nnz];
	colIdx =  new int[nnz];
	value = new double[nnz];

	return;
    }

    public void dispMatrix(){
	DecimalFormat shortFormat = new DecimalFormat(" #0.000;-#0.000");
	int row;    // current row
	int col;    // current column
	int idx=0;  // current data element

	for(row=0; row < nrow; row++){
	    col = 0;
	    
	    while(rowIdx[idx] == row){
		/* if (row,col) is empty print zero and move on... */
		while(col < colIdx[idx]){
		    String entry = shortFormat.format(0.0);
		    System.out.print(" " + entry + " ");
		    col++;
		}

		/* ... until we find a nonzero entry. */
		String entry = shortFormat.format(value[idx]);
		System.out.print(" " + entry + " ");
		col++;
		idx++;

		if(idx == nnz) break;
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
	    if(idx == nnz) break;
	}
            
	return;
    }
	
}
