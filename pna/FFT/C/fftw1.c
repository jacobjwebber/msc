/*****************************************************************************
 *
 * FFTW exercise 1.
 *
 * Manipulate an image in the Fourier domain.
 * To compile type:
 *    $ make fftw1
 *
 * Output (.pgm) files can be viewed with xv.
 *
 *****************************************************************************/

#include <stdio.h>
#include <math.h>

/*
 * The FFTW package offers:
 *   sfttw.h   ... for 'float' precission
 *   dfttw.h   ... for 'double' precission
 * as well as
 *   srffw.h   ... for single precission real transforms
 *   drffw.h   ...     double
 */

#include <sfftw.h>

static const fftw_real zero = 0.0;

/*
 * The ('sqaure') image is NX by NY in size.
 * Filtering should remove rectangular regions determined by
 * NLOWER_CUT and NUPPER_CUT.
 */

#define NX 320
#define NY 320

#define NLOWER_CUT 9
#define NUPPER_CUT 309


void filter_low_frequencies(fftw_complex array2d[NX][NY]);
void filter_high_frequencies(fftw_complex array2d[NX][NY]);


int main (int argc, char **argv)
{
  char *imagefile = "../gibsonsq.dat";

  fftw_real    data[NX][NY];
  fftw_complex store[NX][NY];

  fftw_complex in[NY];
  fftw_complex out[NY];

  fftw_plan plan_forward;
  fftw_plan plan_backward;

  int i, j;

  /*
   * Read in the original image data, and write it out as a pgm.
   */

  datread(imagefile, data, NX, NY);

  pgmwrite("beforeFFT.pgm", data, NX, NY, 0, 255);
  
  for (i=0; i < NX; i++)
  {
      for (j=0; j < NY; j++)
      {
          store[i][j].re = data[i][j];
          store[i][j].im = 0;
      }
  }

  

  /*
   * Create FFTW plans.
   */

  plan_forward = fftw_create_plan(NX, FFTW_FORWARD, FFTW_ESTIMATE);



  if (!plan_forward || !plan_backward) {
    fprintf(stderr, "Failed to initialise plans!\n");
    exit(-1);
  }

  /*
   * Perform the forward FFT in the 1st dimension.
   */
  for (j = 0; j < NY; j++)
  {


    for (i = 0; i < NX; i++)
    {
        in[i] = store[i][j];
    }
    
    fftw_one(plan_forward, in, out);

    for (i = 0; i < NX; i++)
    {
        store[i][j] = out[i];
    }
    

  }




  /*
   * Perform the forward FFT in the 2nd dimension.
   */

for (i = 0; i < NY; i++)
{


  for (j = 0; j < NX; j++)
  {
      in[j] = store[i][j];
  }
  
  fftw_one(plan_forward, in, out);

  for (j = 0; j < NX; j++)
  {
      store[i][j] = out[j];
  }
  

}






  /*
   * Write the real and imaginary parts of the image in Fourier space
   * to separate files.
   */





  pgmwrite("data_real.pgm", data, NX, NY, 1, 2);






  pgmwrite("data_imag.pgm", data, NX, NY, 1, 2);


  /*
   * Operate on the image in Fourier space; either:
   *
   *  1) cut off high frequencies, or
   *  2) cut off low frequencies
   *
   */

  filter_high_frequencies(store);

  /* filter_low_frequencies(store); */

  /*
   * Perform backward FFT in the 1st dimension.
   */


  





  /*
   * Perform backward FFT in the 2nd dimension.
   */





  /*
   * Store the final image.
   */







  pgmwrite("finalFFT.pgm", data, NX, NY, 0, 255);

  /*
   * Clean up.
   */

  



  return 0;
}


/*****************************************************************************
 *
 *  filter_low_frequencies
 *
 *    i.e., mask out the 'corners' in the Fourier domain.
 *
 *****************************************************************************/

void filter_low_frequencies(fftw_complex array[NX][NY]) {

  int i, j;








  return;
}

/*****************************************************************************
 *
 *  filter_high_frequencies
 *
 *    i.e., mask out a 'cross' in the Fourier domain.
 *
 *****************************************************************************/

void filter_high_frequencies(fftw_complex array[NX][NY]) {

  int i, j;






  return;
}
