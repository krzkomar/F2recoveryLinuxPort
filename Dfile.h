#pragma once
#include <zlib.h>

typedef struct // size of 20
{
    char		*FileName;	// 0,1,2,3 	filename
    char		Compressed;	// 4,[allign:5,6,7] 1 - compressed/ 0 - decompressed
    unsigned int	RealSize;	// 8,9,10,11 	Size of the file without compression
    unsigned int	PackedSize;	// 12,13,14,15  Size of the compressed file
    unsigned int	Location;    	// 16,17,18,19  Address/Location of the file
} dVfile_t;

typedef struct _dFile_t // size of 44
{
 struct  _dFileBase_t	*Dbase;		// 0,1,2,3     baza danych
  dVfile_t 		*Vfile;		// 4,5,6,7     uchwyt pliku wirtualnego
  char 			Flags;		// 8,[ align: 9,10,11 ]
  FILE 			*fhandler;	// 12,13,14,15 uchwyt pliku bazy danych
  z_streamp 		pData;		// 16,17,18,19 
  char 			*buff1024;	// 20,21,22,23 ?
  int 			FirstChr;	// 24,25,26,27 
  int 			UngetChr;	// 28,29,30,31
  int 			DataPos;	// 32,33,34,35
  int 			CurPos;		// 36,37,38,39
  struct _dFile_t 	*next;		// 40,41,42,43 
} dFile_t;


typedef struct _dFileBase_t // size of 20
{
    char 		*ArchName;	// 0,1,2,3		Archive file name
    unsigned int 	TreeSize;	// 4 [allign:5,6,7]	tree block size
    unsigned int 	FilesTot;	// 8,910,11		files total count
    dVfile_t 		*DirTree;	// 12,13,14,15		list of all files
    dFile_t 		*OpenedList;	// 16,17,18,19		opened file list
} dFileBase_t;

typedef struct 
{
  char 		Entry[260];
  char 		Path[260];
  int 		i520;
} dDirEnt_t;

dFileBase_t *dOpenArchive( const char *filename );
int dCloseArchive( dFileBase_t *dbase );
int dGetFile( dFileBase_t *dbase, dDirEnt_t *obj, char *path );
int dReadDir( dFileBase_t *db, dDirEnt_t *de);
unsigned int dFilesTotal( dFile_t *stream );
int dclose( dFile_t *stream );
int dflush( dFile_t *stream );
dFile_t *dopen( dFileBase_t *dbase, const char *fname, const char *mode );
dFile_t *dopenstrm( const char *filename, const char *mode, dFile_t *stream );
void dsetbuf( dFile_t *stream, void *buf);
int dunlock( dFile_t *stream );
int dvprintf( dFileBase_t *stream, const char *format, va_list vl);
int dgetc( dFile_t *stream );
char *dgets( char *s, int n, dFile_t *stream );
int dputc( int c, dFile_t *stream );
int dputs( char *s, dFile_t *stream );
int dungetc( int chr, dFile_t *stream );
int dread( void *ptr, unsigned int blksize, int blknum, dFile_t *stream );
int dwrite( void *buffer, size_t size, size_t count, dFile_t *stream );
int dtellp( dFile_t *stream, int *pos );
int dseek( dFile_t *stream, int offset, int whence );
int dseekset( dFile_t *stream, unsigned int *pos );
int dtell( dFile_t *stream );
int drewind( dFile_t *stream );
void d_unk01( dFile_t *stream );
int dfeof( dFile_t *stream );
int derror( dFile_t *stream );
void *dLookingUpFile( dFileBase_t *dbase, const char *path );
dFile_t * dopen_int( dFileBase_t *dbase, const char *fname, const char *mode, dFile_t *NewNode );
int dReadChar( dFile_t *stream );
int dReadCompressed( dFile_t *stream, char *buffer, size_t size );

