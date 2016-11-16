import java.io.FileOutputStream;
import java.io.PrintStream;
import java.text.DecimalFormat;

import sparse.CSRMatrix;

public class pollutionModel{
    /* Small class which will solve the two-dimensional polution problem
       represented by a convection-diffusion equation using upwind
       finite differences. Note that we make the slightly unrealistic
       assumption that the boundary conditions are Dirichlet, rather
       than Neumann.

       The final solution is written to disk along with an appropriate
       AVS Field file, for use with the "pollution.dat" AVS Application
       file.

       Written by George Beckett
       Created Thursday, 21st August 2003.
       Modified February 2004 -- for Numerical Algorithms module.
    */

    public static void main(String[] args){
	/* Start by declaring variables, */
	final int M=7;         // grid resolution;

	final int maxIter=5000; // Maximum no. BiCGSTAB iterations
	final double tol=1.0e-6;// Relative BiCGSTAB tolerance

	final int noUnknowns;   // number of unknowns
        final int noNonzeros;   // number of nonzeros in sparse matrix

	/* Boundary conditions -- see exercise sheet for notation */
	final double y1=0.4;  
	final double y2=0.6;  
        final double p=2;
  	    // N.B. Parameter k is chosen such that height of "chimney"
	    // is 1.

        final double k = Math.pow(2.0/(y2-y1), 2.0*p);

	/* Other variables */
	int index;             // index of current element being
	int i;                 // current i coordinate being processed
        int j;                 // current j coordinate being processed

	double x;  // x and y data at current coordinate.
	double y;  //

	double coeffC;         //
	double coeffN;         //
	double coeffE;         // temporary stores for coefficient
	double coeffS;         // data
	double coeffW;         //

	int RC;  //
	int RN;  //
	int RE;  // local index on interior mesh
	int RS;  //
	int RW;  //

	DecimalFormat SixDP = new DecimalFormat("##00.000000");

	/* Initialisation of linear system */
	noUnknowns = M*M;
	noNonzeros = 5*(M-2)*(M-2) + 16*(M-2) + 12;
	CSRMatrix A = new CSRMatrix(noUnknowns, noUnknowns, 
					noNonzeros);

	double [] b = new double[noUnknowns]; // righthand side

	double [] u = new double[noUnknowns]; // solution vector

	/* In this example, we will populate the matrix by hand,
	   stepping along each row of the mesh (x-direction), column
	   by column. */
	index = 0;

	for(j=0; j<M; j++){
	    for(i=0; i<M; i++){
		x = ((double) i+1.0)/((double) M+1);
		y = ((double) j+1.0)/((double) M+1);

		RC = i+j*M;
		RN = i+(j+1)*M;
		RE = (i+1)+j*M;
		RS = i+(j-1)*M;
		RW = (i-1)+j*M;

		coeffC = 4.0*(M+1)*(M+1);
		coeffN = -(M+1)*(M+1);
		coeffE = -(M+1)*(M+1);
		coeffS = -(M+1)*(M+1);
		coeffW = -(M+1)*(M+1);

		/* Compute righthand side, */
		b[RC] = 0.0;

		/* and allocate rowStart entry. */
		A.rowStart[RC] = index;

		/* Now insert coefficients into the matrix, modifying
		   the allocation for boundary terms - there are nine
		   different cases corresponding to the interior, the
		   sides and the corners of the solution domain. Local
		   quantities are referenced using the compass
		   notation. */
		if(i>0 && i<(M-1) && j>0 && j<(M-1)){
		    // interior of the domain
		    A.colIdx[index] = RS;
		    A.value[index] = coeffS;
		    index++;

		    A.colIdx[index] = RW;
		    A.value[index] = coeffW;
		    index++;

		    A.colIdx[index] = RC;
		    A.value[index] = coeffC;
		    index++;

		    A.colIdx[index] = RE;
		    A.value[index] = coeffE;
		    index++;

		    A.colIdx[index] = RN;
		    A.value[index] = coeffN;
		    index++;
		}
		else if(i==0 && j>0 && j<(M-1)){
		    // west boundary
		    A.colIdx[index] = RS;
		    A.value[index] = coeffS;
		    index++;

		    A.colIdx[index] = RC;
		    A.value[index] = coeffC;
		    index++;

		    A.colIdx[index] = RE;
		    A.value[index] = coeffE;
		    index++;

		    A.colIdx[index] = RN;
		    A.value[index] = coeffN;
		    index++;
		}
		else if(i==(M-1) && j>0 && j<(M-1)){
		    // east boundary
		    A.colIdx[index] = RS;
		    A.value[index] = coeffS;
		    index++;

		    A.colIdx[index] = RW;
		    A.value[index] = coeffW;
		    index++;

		    A.colIdx[index] = RC;
		    A.value[index] = coeffC;
		    index++;

		    A.colIdx[index] = RN;
		    A.value[index] = coeffN;
		    index++;

		    // Check to see if we are at the pollution source
		    if((y > y1) && (y < y2))
			b[RC] = -coeffE*(k*Math.pow((y2-y),p)*
					 Math.pow((y-y1),p)); 
		}
		else if(i>0 && i<(M-1) && j==0){
		    // south boundary
		    A.colIdx[index] = RW;
		    A.value[index] = coeffW;
		    index++;

		    A.colIdx[index] = RC;
		    A.value[index] = coeffC;
		    index++;

		    A.colIdx[index] = RE;
		    A.value[index] = coeffE;
		    index++;

		    A.colIdx[index] = RN;
		    A.value[index] = coeffN;
		    index++;
		}
		else if(i>0 && i<(M-1) && j==(M-1)){
		    // north boundary
		    A.colIdx[index] = RS;
		    A.value[index] = coeffS;
		    index++;

		    A.colIdx[index] = RW;
		    A.value[index] = coeffW;
		    index++;

		    A.colIdx[index] = RC;
		    A.value[index] = coeffC;
		    index++;

		    A.colIdx[index] = RE;
		    A.value[index] = coeffE;
		    index++;
		}
		else if(i==0 && j==0){
		    // South-west corner
		    A.colIdx[index] = RC;
		    A.value[index] = coeffC;
		    index++;

		    A.colIdx[index] = RE;
		    A.value[index] = coeffE;
		    index++;

		    A.colIdx[index] = RN;
		    A.value[index] = coeffN;
		    index++;
		}
		else if(i==(M-1) && j==0){
		    // South-east corner
		    A.colIdx[index] = RW;
		    A.value[index] = coeffW;
		    index++;

		    A.colIdx[index] = RC;
		    A.value[index] = coeffC;
		    index++;

		    A.colIdx[index] = RN;
		    A.value[index] = coeffN;
		    index++;
		}
		else if(i==0 && j==(M-1)){
		    // North-west corner
		    A.colIdx[index] = RS;
		    A.value[index] = coeffS;
		    index++;

		    A.colIdx[index] = RC;
		    A.value[index] = coeffC;
		    index++;

		    A.colIdx[index] = RE;
		    A.value[index] = coeffE;
		    index++;
		}
		else if(i==(M-1) && j==(M-1)){
		    // North-east corner
		    A.colIdx[index] = RS;
		    A.value[index] = coeffS;
		    index++;

		    A.colIdx[index] = RW;
		    A.value[index] = coeffW;
		    index++;

		    A.colIdx[index] = RC;
		    A.value[index] = coeffC;
		    index++;
		}
		else{
		    // This case should never occur!
		    System.out.println("Error in coordinate allocation.");
		}
	    }
	}

	/* Check that the correct no. elements have been
	 * allocated. */
	if(index == noNonzeros){
	    RC = M*M;
	    A.rowStart[RC] = index;
	}
	else{
	    System.out.println("index = " + index + " nnz = " 
			       + noNonzeros);
	    System.out.println("Error - incorrect no. entries.");
	    return;
	}

	/* initial solution will be the zero vector, */
	for(i=0; i<noUnknowns; i++)
	    u[i] = 0.0;

	/* ... and then away we go! */
	A.lincg(b, u, tol, maxIter);

	

        // Save final solution to a file

        System.out.println("Writing solution to pollution.dat");

        try{
            FileOutputStream outputStream = 
                new FileOutputStream("pollution.dat");
            PrintStream myOut = new PrintStream(outputStream);
            
            for (j=0; j<(M+2); j++){
                for(i=0; i<(M+2); i++){
		    x = ((double) i)/((double) M + 1.0);
		    y = ((double) j)/((double) M + 1.0);
		    
		    if(i>0 && i<(M+1) && j>0 && j<(M+1)){
			RC = (i-1)+(j-1)*M;
			myOut.println(SixDP.format(u[RC]));
		    }
		    else  if(i == M+1){
			// east boundary with inflow
			if((y > y1) && (y < y2))
			    myOut.println(SixDP.format(k*Math.pow((y2-y),p)*Math.pow((y-y1),p)));
			else
			    myOut.println(SixDP.format(0.0));
		    }
		    else
			myOut.println(SixDP.format(0.0));
                }
            }

            myOut.close();
        }
        catch(Exception e){
            System.err.println("Error writing solution data to file.");
        }

        // Then write a field file for AVS Express.

        System.out.println("Writing associated field file to pollution.general");

	try{
            FileOutputStream outputStream = 
                new FileOutputStream("pollution.general");
            PrintStream myOut = new PrintStream(outputStream);
	    myOut.println("file = pollution.dat");
	    myOut.println("grid = " + (M+2) + " x " + (M+2));
	    myOut.println("format = ascii");
	    myOut.println("interleaving = record");
	    myOut.println("majority = row");
	    myOut.println("field = field0");
	    myOut.println("structure = scalar");
	    myOut.println("type = float");
	    myOut.println("dependency = positions");
	    myOut.println("positions = regular, regular, 0, 1, 0, 1");
	    myOut.println("");
	    myOut.println("end");

	    myOut.close();
	}
	catch(Exception e){
            System.err.println("Error writing dx general file.");
        }

	return;
    }

}
