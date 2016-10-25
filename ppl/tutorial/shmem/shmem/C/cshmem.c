#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "shmem.h"

void bufprint(char *label, int procnum, char *bufname, int *buf, int nbuf);

int main(void)
{
  int nproc = 1, procnum = 0, i, nbuf;

/*
 * Declare the row and column arrays
 */

  int *rowbuf, *colbuf;

/*
 * Start up the parallel code and record nproc, the total number of CPUS,
 * and procnum, the identifier for this CPU in the range [0,nproc-1]
 */

  shmem_init();
  procnum = shmem_my_pe();
  nproc = shmem_n_pes();


  if (0 == procnum) printf("Running OpenSHEM program on %d CPUs\n", nproc);

/*
 * Size of buffers is equal to the number of processors
 */

  nbuf = nproc;

  rowbuf = (int *) malloc(nbuf*sizeof(int));
  colbuf = (int *) shmalloc(nbuf*sizeof(int));

/*
 * Initialise the row buffer appropriately on each CPU
 * and set the column buffer to -1
 */

  /* ... */
  for (i = 0; i < nproc; i++)
  {
      rowbuf[i] = i + procnum * nproc;
      colbuf[i] = -1;
  }

  shmem_barrier_all();

/*
 * Print out the initial values
 */

  bufprint("Before", procnum, "rowbuf", rowbuf, nbuf);
  bufprint("Before", procnum, "colbuf", colbuf, nbuf);

/*
 * Now must publish relevant buffer and transfer data using puts or gets.
 * Remember to put synchronisation in the appropriate places
 */


  /* ... */
  for(i=0; i < nproc; i++)
  {
      shmem_int_put(&colbuf[procnum], &rowbuf[i], 1, i);
  }

/*
 * Print out the final values
 */

  bufprint("After ", procnum, "rowbuf", rowbuf, nbuf);
  bufprint("After ", procnum, "colbuf", colbuf, nbuf);


/*
 * Close down the parallel program
 */

  shmem_finalize();

  return(0);
}

void bufprint(char *label, int procnum, char *bufname, int *buf, int nbuf)
{
  int i;

  printf("%s: procnum = %d, %s = [", label, procnum, bufname);

  for (i=0; i < nbuf-1; i++)
  {
    printf("%2d, ", buf[i]);
  }

  printf("%2d]\n", buf[nbuf-1]);
}
