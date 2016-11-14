/*



*/

void read_head_variables(FILE *file_pointer, int *datatype, int *gridtype, 
			 int *sizeG, int *sizeX, int *sizeY, int *sizeZ, float *offsX, float *offsY, float *offsZ);
void write_head_variables(FILE *file_pointer, int datatype, int gridtype, 
			  int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ);
void read_volume_head(char *fname, int *sizeG, int *sizeX, int *sizeY, int *sizeZ, float *offsX, float *offsY, float *offsZ);


void read_volume_float(  char *fname, float              *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ);
void read_volume_double( char *fname, double             *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ);
void read_volume_ushort( char *fname, unsigned short int *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ);
void read_volume_char(   char *fname, char               *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ);
void write_volume_ushort(char *fname, unsigned short int *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ);
void write_volume_float( char *fname, float              *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ);
void write_volume_char(  char *fname, char               *data, int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ);

