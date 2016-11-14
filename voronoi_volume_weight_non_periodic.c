/*

   NAME: 
       VORONOI VOLUME

   DESCRIPTION:
       Create a (weighted) distance field starting from a set of voronoi
          cell nuclei. The program assigns to each pixel the
          distance to the closest nuclei. This gives as an output
          a weighted voronoi distribution.

   COMPILE AS:

       gcc -o voronoi_volume_weight_non_periodic voronoi_volume_weight_non_periodic.c io_files.c -lm

   HISTORY:
       2008-08-08  First  version. 

   Written by: Miguel Angel Aragon Calvo. 
	       miguel@pha.jhu.edu

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "io_files.h"

//===============================================================
//                 MAIN FUNCTION
//===============================================================
int main(int argc, char **argv)
{
  //--- General variables
  int    i,j,k,w;
  int    cont=0;
  int    n_nuclei,n_nuclei_periodic;
  float  boxsize;
  double min_dista = 0.0;
  double half_pix;
  double *x_arr,    *y_arr,    *z_arr,    *w_arr;
  double xg,yg,zg;
  //--- Temporal variables
  double tempD;
  float  tempF;
  //--- Data arrays
  //double *density;
  //--- Grid size
  int    sizeG,sizeX,sizeY,sizeZ;
  float  offsX,offsY,offsZ;
  int datatype=4, gridtype=1; 
  int    n_cell; 
  //--- Filenames
  char   *file_inp, *file_wei, *file_dis, *file_vor;
  FILE   *fp_inp, *fp_wei, *fp_dis, *fp_vor;
  char   buffer[255];
  //--- For header
  int  remaining_bytes = 256-36;
  char dummy_byte=0;

  

  if (argc != 11) {
    printf("------------------------------------------ \n");
    printf("           Voronoi Volume Generator \n");
    printf("\n");
    printf(" Usage:\n");
    printf("     ./voronoi_volume_weight  file_nuclei file_weight sizeG sizeX sizeY sizeY offX offY offZ file_out_suff \n");
    printf("\n");
    printf("        Output: distance field from the seeds and voronoi cell ID \n"); 
    printf("\n");
    printf("\n");
    printf("------------------------------------------ \n");
    exit(0); }

  file_inp   = *++argv;
  file_wei   = *++argv;
  sscanf(*++argv,"%d",&sizeG);
  sscanf(*++argv,"%d",&sizeX);
  sscanf(*++argv,"%d",&sizeY);
  sscanf(*++argv,"%d",&sizeZ);
  sscanf(*++argv,"%f",&offsX);
  sscanf(*++argv,"%f",&offsY);
  sscanf(*++argv,"%f",&offsZ);
  file_dis   = *++argv;

  //==========================
  //--- Read nuclei data
  //==========================  
  //--- Attempt to open file
  sprintf(buffer,"%s",file_inp);
  printf("File '%s'\n",buffer);
  if(!(fp_inp=fopen(buffer,"r"))) { printf("Can't open file '%s'\n",buffer); exit(0); }
  //--- Read boxsize  and number of nuclei
  fread(&boxsize,sizeof(float),1,fp_inp);
  fread(&n_nuclei,sizeof(int),1,fp_inp);
  //--- Working array for the positions
  x_arr = (double *) malloc(sizeof(double) * n_nuclei);
  y_arr = (double *) malloc(sizeof(double) * n_nuclei);
  z_arr = (double *) malloc(sizeof(double) * n_nuclei);

  printf("%d nuclei, boxsize = %f \n",n_nuclei,boxsize);
  //--- Read nuclei positions
  for (i=0 ; i<n_nuclei ; i++){            //--- DTFE format
    fread(&tempF,sizeof(float),1,fp_inp);
    tempD = (double) tempF;
    x_arr[i] = tempD/boxsize;
    printf("%e \n",tempD/boxsize);
    fread(&tempF,sizeof(float),1,fp_inp);
    tempD = (double) tempF;
    y_arr[i] = tempD/boxsize;
    fread(&tempF,sizeof(float),1,fp_inp);
    tempD = (double) tempF;
    z_arr[i] = tempD/boxsize;
  }
  //--- Close input file
  fclose(fp_inp);
  printf("------------------------------\n");
  fflush(stdout);

  //==========================
  //--- Read weights
  //==========================
  sprintf(buffer,"%s",file_wei);
  printf("File '%s'\n",buffer);
  if(!(fp_wei=fopen(buffer,"r"))) { printf("Can't open file '%s'\n",buffer); exit(0); }
  int n_wei;
  fread(&n_wei,sizeof(int),1,fp_wei);

  //--- Check same nuclei as weights
  if (n_nuclei != n_wei) {
    printf("Not same number of nuclei as weights!, stopping \n");
    fflush(stdout);
    exit(0);
  }
  //--- Allocate memory for weights and add periodic buffers
  w_arr    = (double *) malloc(sizeof(double) * n_wei);
  for (i=0 ; i<n_wei ; i++){
    fread(&tempD,sizeof(double),1,fp_wei);
    w_arr[i] = tempD;
    printf("%d, %lf\n",i, w_arr[i]);
  }
  fclose(fp_wei);


  //==========================  
  //--- Write header
  //==========================  
  //--- Distance file
  sprintf(buffer,"%s.ddis",file_dis);
  if(!(fp_dis=fopen(buffer,"w"))) { printf("Can't open file '%s'\n",buffer); exit(0); }
  write_head_variables(fp_dis, datatype, gridtype, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);
  for (i=0; i<remaining_bytes; i++) fwrite(&dummy_byte,sizeof(char),1,fp_dis); 

  //--- Voronoi file
  sprintf(buffer,"%s.ivor",file_dis);
  if(!(fp_vor=fopen(buffer,"w"))) { printf("Can't open file '%s'\n",buffer); exit(0); }
  datatype = 2;
  write_head_variables(fp_vor, datatype, gridtype, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);
  for (i=0; i<remaining_bytes; i++) fwrite(&dummy_byte,sizeof(char),1,fp_vor);

  //==========================  
  //--- Generate Voronoi Volume
  //========================== 
  printf("Generate voronoi volume...\n");
  half_pix = 0.5/sizeG;
  for (i=0 ; i<sizeX ; i++){  
    for (j=0 ; j<sizeY ; j++){
      for (k=0 ; k<sizeZ ; k++){

	//--- Pixel's center position
	xg = i/((double) sizeG) + half_pix;
	yg = j/((double) sizeG) + half_pix;
	zg = k/((double) sizeG) + half_pix;

	//--- Find the smallest off all distances
	int nuc_i;
	for(w=0; w<n_nuclei; w++) {
	  //--- Compute distances and weight them
	  tempD = (pow(xg-x_arr[w],2) + pow(yg-y_arr[w],2) + pow(zg-z_arr[w],2)) / w_arr[w];
	  //--- First time always take value
	  if (w==0) {
	    min_dista = tempD;
	    nuc_i = w;
	  }
	  //--- Get the smallest of the two
	  if (tempD < min_dista){
	    min_dista = tempD;
	    nuc_i = w;
	  }
	}

	//--- Multiply again by weight to recover original unweighted distance
	min_dista *= w_arr[nuc_i];

	//--- Write distance to disk
	fwrite(&min_dista,sizeof(double),1, fp_dis);
	//--- Write seed id to disk
	fwrite(&nuc_i,sizeof(int),1, fp_vor);

      }
    }
    printf("Plane %d \n",i);
    fflush(stdout);
  }
  //--- Close output file
  fclose(fp_dis);
  fclose(fp_vor);

  //--- Free allocated memory
  printf("Free alocated memory...\n");fflush(stdout);
  fflush(stdout);
  free(x_arr);
  free(y_arr);
  free(z_arr);
  free(w_arr);

  printf("End of program\n");
  fflush(stdout);

}



