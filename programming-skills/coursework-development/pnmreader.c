/* Make some islands */

#include <stdio.h>
#include <stdlib.h>

/* to read in a .pnm file created by the gimp. */

#define MAX 4000 
	
int map[MAX][MAX];

int main(int argc, char * argv[])
{
	int x, y = 200;
	int i, j;
	FILE * fp;
	char inFile[20] = "\0";
	char outFile[20] = "\0";
	
	/* should only be max 5 */
	int length = 80;
	char chars[length];	
	char *morechars;
	int num;
	
	
	if (argc == 3)
	{
	  strcpy(inFile, argv[1]);
	  strcpy(outFile, argv[2]);
	}
	else {
	  printf("Usage: pnmreader infile outfile\n"); 
          exit(1); 
	}
	

  /* read back in convert to 0 and 1 */
	if ((fp = fopen(inFile, "r+")) == NULL)
	{
	  printf("Cannot open file %s.\n", inFile);
		exit(1);
	}
	
	i = j = 0;

	/* skip first two lines */ 
        fgets(chars, (length - 1), fp);
        printf(chars); 
        fgets(chars, (length - 1), fp);
        printf(chars); 


	/* next line contain x and y */ 

        morechars = (char *) malloc(length* sizeof(char)); 
	
        fgets(chars, (length - 1), fp);
        printf(chars); 
	x = strtol(chars, &morechars, 0);
	y = strtol(morechars, 0, 0) ; 

	printf("x=%d, y=%d\n", x, y);
	

	while (!feof(fp))
	{
	  fgets(chars, (length - 1), fp);
	  num = strtol(chars, 0, 0);	  
	  
	  /* sea is white is 255, land is black is 0 */
	  if (num < 128) { 

	    map[i][j++] = 1; 
	  }
	  else {
	    map[i][j++] = 0;
	  }
	  // map[i][j++] = num ? 0 : 1;
	  /* move to next row */
	  if (j == x) 
	    {
	      if (++i < y) j = 0;
	      else break;
	    }
	  
	}
	

	
	fclose(fp);
	
	/* write array to file */
	/* Open file for output */
	if ((fp = fopen(outFile, "w")) == NULL)
	{
	  printf("Cannot open file %s.\n", outFile);
		exit(1);
	}
	
        fprintf(fp, "%d %d\n", x, y);
	for (i = 0; i < y; i++)
	{
		for (j = 0; j < x; j++)
		{
			fprintf(fp, "%d ", map[i][j]);
		
		}
		fprintf(fp, "\n");
	}
	
	
	

	return 0;
}
