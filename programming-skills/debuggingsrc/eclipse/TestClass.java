import java.text.*;
import java.util.*;

/*
 * Debugging exercise for HPC Architectures Course
 * Adrian Jackson, EPCC, 2013
 * Derived from code from George Beckett, EPCC, 2002
 */

public class TestClass{

	private static final int SUCCESS = 0;
	private static final int FAILURE = 1;

	public static void main(String args[]){

		int noRows;
		int nnz;
		COOMatrix M = null;
		SparseMatrix Q = null;
		Lock Qlock = new Lock();
		
		noRows = 30;
		M = new COOMatrix(noRows, noRows);
		double x[] = new double[noRows];
		double f[] = new double[noRows];
		double w[] = new double[noRows];

		nnz = Q.createMatrix(noRows, M, x, f);

		Q = new SparseMatrix(noRows,noRows,nnz);

		M.dispMatrix();

		try {
			Qlock.lock();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Q.populateMatrix(M.rowIdx, M.colIdx, M.value);
		//Qlock.unlock();

		
		try {
			Qlock.lock();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Q.dispMatrix();
		//Qlock.unlock();	
		
		try {
			Qlock.lock();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Q.timesv(x,w);
		//Qlock.unlock();
		
		for(int i = 0; i<noRows; i++){
			if(w[i] != f[i]){
				System.out.println("timesv failure");
				return;
			}
		}
		System.out.println("timesv test passed");

	}


}
