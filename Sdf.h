#pragma once

typedef struct
{
  char vcrname[13];
  char sdfname[13];
  int i28;
} Sdf_t;

int  SdfOpen( char ***FileList, int *Cnt );
int  SdfClose( char **List );
int  SdfUnk01( char *fname, Sdf_t *sdf );
void SdfUnk02( Sdf_t *sdf );
int  SdfUnk03( char *fname, char *vcrname, Sdf_t *sdf );
void SdfUnk04( Sdf_t *sdf );
void SdfUnk05();
int  SdfUnk06( char *fname, Sdf_t *sdf );
int  SdfUnk07( const char *fname, Sdf_t *sdf );


