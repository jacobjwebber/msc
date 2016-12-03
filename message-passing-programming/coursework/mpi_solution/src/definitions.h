
#define WIDTH 512
#define HEIGHT 384

//#define WIDTH 10
//#define HEIGHT 10

#define MAXITER 10000
#define PRINTFREQ 100000


#define DIMX 4
#define DIMY 4

#define P DIMX*DIMY 
#define WIDTH_P WIDTH / DIMX 
#define HEIGHT_P HEIGHT / DIMY

#define SMALL_DELTA 0
#define DELTA_FREQ 20

typedef float real_number;
#define MPI_REALNUMBER MPI_FLOAT
