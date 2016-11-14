/*

   NAME: 
       VORONOI VOLUME


   DESCRIPTION:
       Create a distance field starting from a set of voronoi
          cell nuclei. The program assigns to each pixel the
          distance to the closest nuclei. This gives as an output
          a voronoi distribution.


   COMPILE AS:

       gcc -o voronoi_volume voronoi_volume.c io_files.c -lm


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
  int     i,j,k,w;
  int     cont=0;
  int     count=0;
  int     n_pixels;
  int     n_nuclei,n_nuclei_periodic;
  float   boxsize;
  double  min_dista = 0.0;
  //--- Normalized Voronoi
  double  second_min_dista = 0.0;
  int     flag_normalize   = 0;
  double  half_pix;
  double *x_nuclei, *y_nuclei, *z_nuclei;
  double *x_arr, *y_arr, *z_arr;
  double  xg,yg,zg;
  //--- Temporal variables
  double  tempD;
  float   tempF;
  //--- Data arrays
  double *densi=NULL;
  int    *water=NULL;
  //--- Grid size
  int     sizeG,sizeX,sizeY,sizeZ;
  float   offsX=0,offsY=0,offsZ=0;
  int     datatype=4, gridtype=1; 
  int     n_cell; 
  //--- Filenames
  char   *file_inp, *file_out;
  FILE   *fp_inp, *fp_out;
  char    buffer[255];

  if (argc != 5) {
    printf("------------------------------------------ \n");
    printf("           Voronoi Volume Generator \n");
    printf("\n");
    printf(" Usage:\n");
    printf("     ./voronoi_volume_hierarchy  file_nuclei [normalization] N_grid  file_out \n");
    printf("\n");
    printf("------------------------------------------ \n");
    exit(0); }

  file_inp   = *++argv;
  sscanf(*++argv,"%d",&flag_normalize);
  sscanf(*++argv,"%d",&sizeG);
  file_out   = *++argv;
  
  //--- Only squared boxes for now...
  sizeX = sizeG;
  sizeY = sizeG;
  sizeZ = sizeG;
  n_pixels = sizeX*sizeY*sizeZ;

  //==========================
  //--- Allocate memory for working arrays
  //==========================  
  printf("Allocating memory...\n"); fflush(stdout);
  densi = (double *) malloc(sizeof(double)*sizeX*sizeY*sizeZ); 
  water = (int *)    malloc(sizeof(int)   *sizeX*sizeY*sizeZ); 

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
  n_nuclei_periodic = n_nuclei*27;
  //--- Allocate memory for nuclei positions, take into account periodic replication
  x_nuclei = (double *) malloc(sizeof(double) * n_nuclei * 27);
  y_nuclei = (double *) malloc(sizeof(double) * n_nuclei * 27);
  z_nuclei = (double *) malloc(sizeof(double) * n_nuclei * 27);

  //--- Working array for the positions
  x_arr = (double *) malloc(sizeof(double) * n_nuclei);
  y_arr = (double *) malloc(sizeof(double) * n_nuclei);
  z_arr = (double *) malloc(sizeof(double) * n_nuclei);


  printf("%d nuclei, boxsize = %f \n",n_nuclei,boxsize);
  //--- Read nuclei positions
  for (i=0 ; i<n_nuclei ; i++){            //--- DTFE format
    fread(&tempF,sizeof(float),1,fp_inp);
    tempD = (float) tempF;
    x_arr[i] = tempD/boxsize;
    printf("%e \n",tempD/boxsize);
    fread(&tempF,sizeof(float),1,fp_inp);
    tempD = (float) tempF;
    y_arr[i] = tempD/boxsize;
    fread(&tempF,sizeof(float),1,fp_inp);
    tempD = (float) tempF;
    z_arr[i] = tempD/boxsize;
  }
  //--- Close input file
  fclose(fp_inp);


  printf("------------------------------\n");
  fflush(stdout);


  //==========================
  //--- Replicate nuclei
  //==========================
  printf("Replicate nuclei...\n");
  cont = 0;
  for (i=0 ; i<=2 ; i++){
    for (j=0 ; j<=2 ; j++){
      for (k=0 ; k<=2 ; k++){
	//--- Concatenate arrays
	for (w=0 ; w<n_nuclei ; w++) {
	  x_nuclei[cont + w] = x_arr[w] + i-1;
          y_nuclei[cont + w] = y_arr[w] + j-1;
          z_nuclei[cont + w] = z_arr[w] + k-1;
	}
	cont = cont+n_nuclei;
      }
    }
  }


  //==========================  
  //--- Write header
  //==========================  
  //--- Attempt to open file
  sprintf(buffer,"%s",file_out);
  if(!(fp_out=fopen(buffer,"w"))) { printf("Can't open file '%s'\n",buffer); exit(0); }
  write_head_variables(fp_out, datatype, gridtype, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);

  //--- Fill rest of header with zeroes
  int  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++) fwrite(&dummy_byte,sizeof(char),1,fp_out); 

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

	//--- Find the smallest off all distances. Initialize minimum to first distance
	min_dista = pow(xg-x_nuclei[0],2) + pow(yg-y_nuclei[0],2) + pow(zg-z_nuclei[0],2);
	for(w=0; w<n_nuclei_periodic; w++) 
	  {
	    tempD = pow(xg-x_nuclei[w],2) + pow(yg-y_nuclei[w],2) + pow(zg-z_nuclei[w],2);
	    //--- Get the smallest of the two
	    if (tempD < min_dista) min_dista = tempD;
	  }

	//--- Find the SECOND smallest off all distances. Initialize minimum to first distance
	second_min_dista = pow(xg-x_nuclei[0],2) + pow(yg-y_nuclei[0],2) + pow(zg-z_nuclei[0],2);
	for(w=0; w<n_nuclei_periodic; w++) 
	  {
	    tempD = pow(xg-x_nuclei[w],2) + pow(yg-y_nuclei[w],2) + pow(zg-z_nuclei[w],2);
	    //--- Get the second closest point
	    if ( (tempD < second_min_dista) && (tempD > min_dista)) second_min_dista = tempD;
	  }

	tempD = min_dista/second_min_dista;

	//--- Write distance to disk
	fwrite(&tempD,sizeof(double),1, fp_out);

      }
    }
    printf("Plane %d \n",i);
    fflush(stdout);
  }

  //--- Close output file
  fclose(fp_inp);


  //--- Free allocated memory
  printf("Free alocated memory...\n"); fflush(stdout);
  
  free(densi);
  free(water);
  free(x_nuclei);  
  free(y_nuclei);
  free(z_nuclei);
  free(x_arr);
  free(y_arr);
  free(z_arr);

  printf("End of program\n"); fflush(stdout);
  

}



