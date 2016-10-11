import java.io.*;
import java.text.*;

public class lufact
{
    static final int NMAX = 100;

    static NumberFormat Nf;

    public static void main (String argv[]) throws IOException
    {
	float [][] a = new float[NMAX][NMAX];
	float [][] l = new float[NMAX][NMAX];
	float [][] u = new float[NMAX][NMAX];
	float [][] lu = new float[NMAX][NMAX];
	float [] x = new float[NMAX];
	float [] b = new float[NMAX];
	float [] ax = new float [NMAX];
	float [] y = new float [NMAX];
	
	int i, j;
	
	int n = 4;  

	Nf = NumberFormat.getInstance();
	Nf.setMaximumFractionDigits(8);
	Nf.setMinimumFractionDigits(8);

	matgen(n, a, b);
	
	System.out.println("\na\n");
	matprint(n, a);
	
	System.out.println("\nb\n");
	vecprint(n, b);
	
	System.out.println("\nNeed to perform LU factorisation of A");
	System.out.println("Currently setting L=U=0\n");
	
	/*
	 * Set l and u to zero
	 */
	
	for (i=0; i < n; i++)
	    {
		for (j=0; j < n; j++)
		    {
			l[i][j] = 0.0f;
			u[i][j] = 0.0f;
		    }
	    }
	
	System.out.println("\nl\n");
	matprint(n, l);
	
	System.out.println("\nu\n");
	matprint(n, u);
	
	/*
	 * Multiply the L and U factors together
	 */
	
	matmat(n, lu, l, u);
	
	System.out.println("\nlu\n");
	matprint(n, lu);
	
	System.out.println("\nNeed to forward and back substitution to solve Ax=b");
	System.out.println("Currently setting x=b\n");
	
	for (i=0; i < n; i++)
	    {
		x[i] = b[i];
	    }
	
	System.out.println("\nx\n\n");
	vecprint(n, x);
	
	/*
	 * Set y = A x and print out
	 */
	
	matvec(n, ax, a, x);
	
	System.out.println("\nAx\n");
	vecprint(n, ax);
    }
    
    
    /*  matprint
     *
     *  Prints a matrix of floats to the screen. Matrix is taken to be
     *  of size n x n although it must have been defined as size NMAX x NMAX
     *  where NMAX is a compile-time constant.
     */
    
    
    static void matprint(int n, float [][] a)
    {
	int i, j;
	
	if (n > 6)
	    {
		System.out.println("Matrix too large to print\n");
	    }
	else
	    {
		for (i=0; i < n; i++)
		    {
			for (j=0; j < n; j++)
			    {
				System.out.print(Nf.format(a[i][j])+"\t");
			    }
			System.out.println();
		    }
	    }
    }
    
    
    /*  vecprint
     *
     *  Prints a vector of floats to the screen. Vector is taken to be
     *  of size n although it must have been defined as size NMAX
     *  where NMAX is a compile-time constant.
     */
    
    
    static void vecprint(int n, float [] x)
    {
	int i;
	
	if (n > 6)
	    {
		System.out.println("Vector too large to print\n");
	    }
	else
	    {
		for (i=0; i < n; i++)
		    {
			System.out.println(Nf.format(x[i])+"\t");
		    }
	    }
    }
    
    
    /*  matvec
     *
     *  Sets y = a.x, where y and x are vectors of floats of length n, and a
     *  is an n x n matrix. All arrays must have been defined as size NMAX
     *  where NMAX is a compile-time constant.
     */
    
    static void matvec(int n, float [] y, float [][] a, float [] x)
    {
	int i, j;
	
	for (i=0; i < n; i++)
	    {
		y[i] = 0.0f;
		
		for (j=0; j < n; j++)
		    {
			y[i] += a[i][j]*x[j];
		    }
	    }
    }
    
    
    /*  matmat
     *
     *  Sets a = b.c, where a, b and c are all n x n matrices of floats.
     *  All matrices must have been defined as size NMAX x NMAX where NMAX
     *  is a compile-time constant.
     */
    
    
    static void matmat(int n, float [][] a, float [][] b, float [][] c)
    {
	int i, j, k;
	
	for (i=0; i < n; i++)
	    {
		for (j=0; j < n; j++)
		    {
			a[i][j] = 0.0f;
			
			for (k=0; k < n; k++)
			    {
				a[i][j] += b[i][k]*c[k][j];
			    }
		    }
	    }
    }
    
    
    /*  matgen
     *
     *  Constructs a random matrix of size n x n, and a solution vector b
     *  All arrays must have been defined as size NMAX where NMAX is a
     *  compile-time constant.
     */
    
    static void matgen(int n, float [][] a, float [] b)
    {
	int init, i, j;
	
	init = 1325;
	for (j = 0; j < n; j++) {
	    for (i = 0; i < n; i++) {
		init = 3125*init % 65536;
		a[i][j] = (init - 32768.0f)/16384.0f;
	    }
	}
	for (i = 0; i < n; i++) {
	    b[i] = 0.0f;
	}
	for (j = 0; j < n; j++) {
	    for (i = 0; i < n; i++) {
		b[i] = b[i] + a[i][j];
	    }
	}
    }
    
}
