/*********************************************************************/
/*                  build results directory                          */
/*********************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char * timestamp()
{
  time_t t = time(NULL);
  return asctime(localtime(&t));
}

char * get_dir_name()
{
  char ts[30];
  strcpy(ts, timestamp());

  int i=0;
  while(ts[i] != '\0')
    {
      if(ts[i] == ' ')
	{
	  ts[i] = '_';
	}
      else if(ts[i] == '\n')
	{
	  ts[i] = '\0';
	  break;
	}
      ++i;
    }

  char * name = (char*)malloc(strlen(ts)+strlen("results-")+1);
  strcpy(name, "results-");
  strcat(name, ts);
  return name;
}

void build_results_directory(char * dirname)
{
  mkdir(dirname, 0777);
  chdir(dirname);
  mkdir("puma_PPM", 0777);
  mkdir("hare_PPM", 0777);
  chdir("..");
 }
