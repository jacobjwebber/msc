import java.text.DecimalFormat;
import java.io.FileReader;
import java.io.StreamTokenizer;
import sparse.COOMatrix;
import sparse.CSRMatrix;

public class CSRTest{

    public static void main(String[] args){



	DecimalFormat shortFormat = new DecimalFormat(" 000.000;-000.000");

	// Create a new COO matrix, based on the above dimensions ...
	COOMatrix A = new COOMatrix();

	String fileStem="../COOInput/";
	String fileName;

	// Check the stdin 
	if(args.length!=1){
	    System.out.println("main:<usage> <filename=1/2/3>");
	    return;
	}
	
	fileName=fileStem + "random_matrix" + args[0] + ".dat";
	int noNonzeros=A.readCOO(fileName);

	// ... and check that the reading went okay.
	int N=A.getN();
	if(A.getM() != N){
	    System.out.println("Error Reading COO matrix A.");
	    return;
	}
	
        /* Display the matrix, created. */
        System.out.println("COO matrix is");
        A.dispMatrix();
	System.out.println(" ");

        /* Then, generate place-holder for CSR version of the matrix. */
        CSRMatrix B = new CSRMatrix(N, N, noNonzeros);

	// ... and check that the creation went okay.
	if(0 == B.getNzmax()){
	    System.out.println("Error creating CSR matrix B.");
	    return;
	}

	// Then, populate CSR matrix from content of COO matrix.
        B.populateMatrix(A.rowIdx, A.colIdx, A.value);

	// We display the CSR matrix for validation purposes.
	System.out.println("CSR matrix is");
	B.dispMatrix();
	System.out.println(" ");

	// Create three test vectors.
	double v[] = new double[N];  // Vectors associated with
	double w1[] = new double[N]; // matrix vector multiplication.
	double w2[] = new double[N]; //
	/* read in v and w1 */

	fileName=fileStem + "vector" + args[0] + ".dat";
	try{
	    FileReader fileReader = new FileReader(fileName); 
	    StreamTokenizer stream = new StreamTokenizer(fileReader);
	    stream.parseNumbers();
	    for(int i=0;i<N;i++){
		stream.nextToken();
		v[i]=stream.nval;
		stream.nextToken();
		w1[i]=stream.nval;
	    }
	}
	catch(Exception e){
	    System.err.println("CSRTest:Error:" + e.getMessage());
	}

        // Then, we perform the matrix-vector multiplication
        B.timesv(v, w2);

        // Finally, output the answers from the multiplications to check
        // that they agree.

        System.out.println("    w1       w2");
        for(int i=0; i<N; i++)
	    System.out.println(shortFormat.format(w1[i]) + " " 
			       + shortFormat.format(w2[i])); 
 
	return;
    }



}