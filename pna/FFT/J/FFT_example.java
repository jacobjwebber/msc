import jfftw.complex.Plan;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;

public class FFT_example{

  public static void main(String[] args){

    // Dimensions of square picture
    final int n = 320;

    // Working Arrays
    double[][] data = new double[n][n];
    double[][] store = new double[n*2][n];

    // 1D FFTW Arrays
    double[] in = new double[n*2];
    double[] out = new double[n*2];

    // FFTW plans
    Plan plan_fw, plan_bw;

    // Read in data file
    datread("../gibsonsq.dat", data, n, n);

    // Write PGM file
    pgmwrite("beforeFFT.pgm", data, n, n, false, 255);

    // Create FFTW Plans

    // Copy data into the complex 'store' array

    // Perform forward FFT in 1st dimension

    // Perform forward FFT in 2nd dimension

    // Alter data in Fourier space
    // by cutting off high frequencies or low frequencies

    // Write the real and imaginary parts of the fourier array to files
    // This relies on the data being copied back into 'store'

    // First extract the real part
    for (int i = 0; i < n; i++){
      for (int j = 0; j < n; j++){
        data[i][j]=store[i*2][j];
      }
    }

    pgmwrite("data_real.pgm", data, n, n, true, 2);

    // Then the imaginary part
    for (int i = 0; i < n; i++){
      for (int j = 0; j < n; j++){
        data[i][j]=store[i*2 + 1][j];
      }
    }

    pgmwrite("data_imag.pgm", data, n, n, true, 2);

    // Perform backward FFT in 1st dimension

    // Perform backward FFT in 2nd dimension

    // Copy real part into 'data' array and normalise

    // Write PGM file
    pgmwrite("afterFFT.pgm", data, n, n, false, 255);

  }

  public static void datread(String filename, double[][] x, int nx, int ny){

    try{
      BufferedReader in = new BufferedReader(new FileReader(filename));

      String line = in.readLine();

      StringTokenizer st = new StringTokenizer(line);

      int nxt = Integer.valueOf(st.nextToken()).intValue();
      int nyt = Integer.valueOf(st.nextToken()).intValue();

      if (nxt != nx || nyt != ny){
        System.err.println("datread: size mismatch, (nx,ny) = ("+nxt+","+nyt+") expected ("+nx+","+ny+")");
        System.exit(1);
      } 

      int i = 0;
      int j = 0;
      line = in.readLine(); 
      while (line != null){
        st = new StringTokenizer(line);
        while (st.hasMoreTokens()){
          x[i][j] = Integer.valueOf(st.nextToken()).intValue();
          i++;
          if ( i == nx ){
            i = 0;
            j++;
          }
        }
        line = in.readLine();
      } 

      in.close();

    }catch(Exception e){
      System.err.println("Exception occurred while reading file: "+filename);
      System.err.println(e);
      System.exit(1);
    }
  }

  public static void pgmwrite(String filename, double[][] x, int nx, int ny, boolean rescale, int imax){
    double[][] tmp = new double[nx][ny];
    int[][] grey = new int[nx][ny];
    final double thresh = 255.0;

    double tmin = Float.MAX_VALUE;
    double tmax = Float.MIN_VALUE;

    for (int i = 0; i < nx; i++){
      for (int j = 0; j < ny; j++){
        tmp[i][j] = x[i][j];
        double abs = Math.abs(tmp[i][j]);
        if (abs < tmin) tmin = abs;
        if (abs > tmax) tmax = abs;
      }
    }

    if (rescale && (tmin < 0 || tmax > thresh)){
      for (int i = 0; i < nx; i++){
        for (int j = 0; j < ny; j++){
          tmp[i][j] = (int)((thresh*((Math.abs(tmp[i][j] - tmin))/(tmax-tmin))) + 0.5);
        }
      }
    }else{
      for (int i = 0; i < nx; i++){
        for (int j = 0; j < ny; j++){
          tmp[i][j] = (int)(Math.abs(tmp[i][j]) + 0.5);
        }
      }
    }

   for (int i = 0; i < nx; i++){
     for (int j = 0; j < ny; j++){
       grey[i][j] = (int)tmp[i][j];
     }
   }

   try{

    PrintWriter out = new PrintWriter(new FileOutputStream(filename));

    out.println("P2");
    out.println("# Written by pgmwrite");
    out.println(nx + " " + ny);
    out.println(imax);
    for (int j = 0; j < ny; j++){
      for (int i = 0; i < nx; i++){
        out.print(Math.min(grey[i][j], imax));
        out.print(" ");
      }
      out.println();
    }
    out.close();

    }catch(Exception e){
      System.err.println("Exception occurred while writing file: "+filename);
      System.err.println(e);
      System.exit(1);
    }
  }
}
