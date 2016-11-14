#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "io_files.h"


//==========================================
//   READ HEADER
//==========================================
void read_head_variables(FILE *file_pointer, int *datatype, int *gridtype, 
			 int *sizeG, int *sizeX, int *sizeY, int *sizeZ, float *offsX, float *offsY, float *offsZ)
{
  fread(datatype, sizeof(int),   1, file_pointer);
  fread(gridtype, sizeof(int),   1, file_pointer);
  fread(sizeG,     sizeof(int),   1, file_pointer);
  fread(sizeX,     sizeof(int),   1, file_pointer);
  fread(sizeY,     sizeof(int),   1, file_pointer);
  fread(sizeZ,     sizeof(int),   1, file_pointer);
  fread(offsX,     sizeof(float), 1, file_pointer);
  fread(offsY,     sizeof(float), 1, file_pointer);
  fread(offsZ,     sizeof(float), 1, file_pointer);
}

//==========================================
//   WRITE HEADER
//==========================================
void write_head_variables(FILE *file_pointer, int datatype, int gridtype, 
			 int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ)
{
  fwrite(&datatype, sizeof(int),   1, file_pointer);
  fwrite(&gridtype, sizeof(int),   1, file_pointer);
  fwrite(&sizeG,     sizeof(int),   1, file_pointer);
  fwrite(&sizeX,     sizeof(int),   1, file_pointer);
  fwrite(&sizeY,     sizeof(int),   1, file_pointer);
  fwrite(&sizeZ,     sizeof(int),   1, file_pointer);
  fwrite(&offsX,     sizeof(float), 1, file_pointer);
  fwrite(&offsY,     sizeof(float), 1, file_pointer);
  fwrite(&offsZ,     sizeof(float), 1, file_pointer);
}

//==========================================
//   READ HEADER
//==========================================
void read_volume_head(char *fname, int *sizeG, int *sizeX, int *sizeY, int *sizeZ, float *offsX, float *offsY, float *offsZ)
{
  int   i,j,k,w,q, cont;
  FILE  *file_pointer;
  char  buffer[200];
  float temp;
  int   n_cell;
  int   datatype=3,gridtype=0;
  int   remaining_bytes=0;

  //--- Attempt to open file
  sprintf(buffer,"%s",fname);
  if(!(file_pointer=fopen(buffer,"r"))) { printf("Can't open file '%s'\n",buffer); exit(0); }

  //--- Read header
  read_head_variables(file_pointer, &datatype, &gridtype, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);
  printf("Head:  %d %d %d \t [%d %d %d] \t %f %f %f\n",datatype,gridtype,*sizeG,*sizeX,*sizeY,*sizeZ,*offsX,*offsY,*offsZ ); fflush(stdout);

  //--- Fill rest of header with zeroes
  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++) fread(&dummy_byte,sizeof(char),1,file_pointer);

}


//==========================================
//
//==========================================
void read_volume_float(char *fname, float  *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ)
{
  int   i,j,k,w,q, cont;
  FILE  *file_pointer;
  char  buffer[200];
  float temp;
  int   datatype=3,gridtype=0;
  int   remaining_bytes=0;

  //--- Attempt to open file
  sprintf(buffer,"%s",fname);
  if(!(file_pointer=fopen(buffer,"r"))) { printf("Can't open file '%s'\n",buffer); exit(0); }

  //--- Read header
  read_head_variables(file_pointer, &datatype, &gridtype, &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);

  //--- Fill rest of header with zeroes
  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++) fread(&dummy_byte,sizeof(char),1,file_pointer);

  //--- Read density from file
  for(i=0; i<sizeX*sizeY*sizeZ; i++) {
    fread(&temp, sizeof(float), 1, file_pointer);
    data[i] = temp;
  }
  fclose(file_pointer);

}

//==========================================
//
//==========================================
void read_volume_double(char *fname, double *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ)
{
  int    i,j,k,w,q, cont;
  FILE   *file_pointer;
  char   buffer[200];
  double temp;
  int    datatype=3,gridtype=0;
  int    remaining_bytes=0;

  //--- Attempt to open file
  sprintf(buffer,"%s",fname);
  if(!(file_pointer=fopen(buffer,"r"))) { printf("Can't open file '%s'\n",buffer); exit(0); }

  //--- Read header
  read_head_variables(file_pointer, &datatype, &gridtype, &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);

  //--- Fill rest of header with zeroes
  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++) fread(&dummy_byte,sizeof(char),1,file_pointer);

  //--- Read density from file
  for(i=0; i<sizeX*sizeY*sizeZ; i++) {
    fread(&temp, sizeof(double), 1, file_pointer);
    data[i] = temp;
  }
  fclose(file_pointer);

}


//==========================================
//
//==========================================
void read_volume_ushort(char *fname, unsigned short int *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ){
  int                 i;
  FILE                *file_pointer;
  unsigned short int  temp;
  char                buffer[200];
  int                 datatype=1;   //--- Unsigned short integer
  int                 gridtype=0;   //--- Rectangular grid
  int                 remaining_bytes=0;

  //--- Attempt to open file
  sprintf(buffer,"%s",fname);
  if(!(file_pointer=fopen(buffer,"r"))) { printf("Can't open file '%s'\n",buffer); exit(0); }

  //--- Read header
  read_head_variables(file_pointer, &datatype, &gridtype, &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);

  //--- Fill rest of header with zeroes
  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++) fread(&dummy_byte,sizeof(char),1,file_pointer); 

  for (i=0; i<sizeX*sizeY*sizeZ; i++) {
    fread(&temp,sizeof(unsigned short int),1, file_pointer);
    data[i] = temp;
  }
  fclose(file_pointer);

}


//==========================================
//
//==========================================
void read_volume_char(char *fname, char *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ){
  int  i;
  FILE *file_pointer;
  char temp;
  char buffer[200];
  int  datatype=0;   //--- char
  int  gridtype=0;   //--- Rectangular grid
  int  remaining_bytes=0;


  //--- Attempt to open file
  sprintf(buffer,"%s",fname);
  if(!(file_pointer=fopen(buffer,"r"))) { printf("Can't open file '%s'\n",buffer); exit(0); }

  //--- Read header
  read_head_variables(file_pointer, &datatype, &gridtype, &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);

  //--- Fill rest of header with zeroes
  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++) fread(&dummy_byte,sizeof(char),1,file_pointer); 

  for (i=0; i<sizeX*sizeY*sizeZ; i++) {
    fread(&temp,sizeof(char),1, file_pointer);
    data[i] = temp;
  }
  fclose(file_pointer);

}


//==========================================
//
//==========================================
void write_volume_float(char *fname, float *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ){

  int    i;
  FILE   *file_pointer;
  float  temp;
  char   buffer[200];
  int    datatype=3,gridtype=0;
  int    remaining_bytes=0;

  //--- Attempt to open file
  sprintf(buffer,"%s",fname);
  if(!(file_pointer=fopen(buffer,"w"))) { printf("Can't open file '%s'\n",buffer); exit(0); }

  //--- Write header
  write_head_variables(file_pointer, datatype, gridtype, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);

  //--- Fill rest of header with zeroes
  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++) fwrite(&dummy_byte,sizeof(char),1,file_pointer); 

  for (i=0; i<sizeX*sizeY*sizeZ; i++) {
    temp = data[i];
    fwrite(&temp,sizeof(float),1, file_pointer);
  }
  fclose(file_pointer);

}




//==========================================
//
//==========================================
void write_volume_ushort(char *fname, unsigned short int *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ){

  int                 i;
  FILE                *file_pointer;
  unsigned short int  temp;
  char                buffer[200];
  int                 datatype=1;   //--- Unsigned short integer
  int                 gridtype=0;   //--- Rectangular grid
  int                 remaining_bytes=0;

  //--- Attempt to open file
  sprintf(buffer,"%s",fname);
  if(!(file_pointer=fopen(buffer,"w"))) { printf("Can't open file '%s'\n",buffer); exit(0); }

  //--- Write header
  write_head_variables(file_pointer, datatype, gridtype, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);

  //--- Fill rest of header with zeroes
  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++) fwrite(&dummy_byte,sizeof(char),1,file_pointer); 

  //--- Total number of cells
  for (i=0; i<sizeX*sizeY*sizeZ; i++) {
    temp = data[i];
    fwrite(&temp,sizeof(unsigned short int),1, file_pointer);
    //if(fmod((float)i,10000.0) == 0) {printf("%ld\n",i); fflush(stdout);}
  }
  fclose(file_pointer);

}



//==========================================
//
//==========================================
void write_volume_char(char *fname, char *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ){
  int  i;
  FILE *file_pointer;
  char temp;
  char buffer[200];
  int  datatype=0;   //--- char
  int  gridtype=0;   //--- Rectangular grid
  int  remaining_bytes=0;

  //--- Attempt to open file
  sprintf(buffer,"%s",fname);
  if(!(file_pointer=fopen(buffer,"w"))) { printf("Can't open file '%s'\n",buffer); exit(0); }

  //--- Write header
  write_head_variables(file_pointer, datatype, gridtype, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);

  //--- Fill rest of header with zeroes
  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++) fwrite(&dummy_byte,sizeof(char),1,file_pointer); 

  for (i=0; i<sizeX*sizeY*sizeZ; i++) {
    temp = data[i];
    fwrite(&temp,sizeof(char),1, file_pointer);
  }
  fclose(file_pointer);

}

