
/***************************************************************/
/* set_params   sets params to params struct                   */
/***************************************************************/

/*========================== Library include files ===========================*/
#include <stdio.h>
#include <stdlib.h>
/*========================== Project include files ===========================*/
#include "params.h"

/*========================== Define Defaults       ===========================*/

#define DEFAULT_r 0.08
#define DEFAULT_a 0.04
#define DEFAULT_b 0.02
#define DEFAULT_m 0.06
#define DEFAULT_k 0.2
#define DEFAULT_l 0.2
#define DEFAULT_dt 0.4
#define DEFAULT_T 100
#define DEFAULT_TA 100

/*====================== Internal function declarations=======================*/

void set_defaults(config_params *parameters); //set defaults for no config file
void dehash(FILE **); //helper function for removing comments from config file

void set_params(config_params *parameters, char* file_name)
{
   FILE *fd; // file
   void dehash(FILE **);

   fd = fopen(file_name, "r");//open file so it can be read

   if (fd == NULL) //case where no config file exists
   {
       printf("No config file found. Using default values \n");
       set_defaults(parameters); //set defaults in this case
       return;
   }
   
   dehash(&fd);
   /*Read in variables from config*/        
   fscanf(fd, "%lf", &(parameters->r));
   dehash(&fd);
   
   fscanf(fd, "%lf", &(parameters->a));
   dehash(&fd);
   
   fscanf(fd, "%lf", &(parameters->b));
   dehash(&fd);
   
   fscanf(fd, "%lf", &(parameters->m));
   dehash(&fd);
   
   fscanf(fd, "%lf", &(parameters->k));
   dehash(&fd);
   
   fscanf(fd, "%lf", &(parameters->l));
   dehash(&fd);
   
   fscanf(fd, "%lf", &(parameters->dt));
   dehash(&fd);
   
   fscanf(fd, "%d", &(parameters->T));
   dehash(&fd);
   
   fscanf(fd, "%d", &(parameters->T_average));
   dehash(&fd);
   
   fclose(fd); //close file
}

void set_defaults(config_params *parameters)
{
    /*Set defaults*/
    parameters->r = DEFAULT_r;
    parameters->a = DEFAULT_a;
    parameters->b = DEFAULT_b;
    parameters->m = DEFAULT_m;
    parameters->k = DEFAULT_k;
    parameters->l = DEFAULT_l;
    parameters->dt = DEFAULT_dt;
    parameters->T = DEFAULT_T;
    parameters->T_average = DEFAULT_TA;
}

void dehash(FILE **fd)
{
    int c;

    if((c =fgetc(*fd)) != '\n') ungetc(c,*fd);

    while(((c=fgetc(*fd)) == ' ')||(c == '#')){

        if(c == '#') while(fgetc(*fd) != '\n');
    }
    ungetc(c,*fd);
}
