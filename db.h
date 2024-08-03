#pragma once
#define dbFAIL		((void *)-1)

int dbLoadData( char *patchfilename, char *datafilename );
void dbFree();
int dbCheckFileLength( const char *filename, int *pLen );
int dbReadFile( const char *filename, void *buf );
int dbClose( xFile_t *stream );
xFile_t *dbOpen( const char *fname, const char *mode );
int dbPrintf( xFile_t *stream, const char *fmt, ... );
int dbPuts( xFile_t *stream, char *str );
int dbgetc( xFile_t *stream );
char *dbgets( char *str, int num, xFile_t *xstream );
int dbputc( int chr, xFile_t *stream );
int dbputs( char *str, xFile_t *stream );
int dbread( void *buffer, size_t size, size_t count, xFile_t *stream );
int dbwrite( void *buffer, size_t size, size_t count, xFile_t *stream );
int dbseek( xFile_t *stream, int offset, int origin );
int dbtell( xFile_t *stream );
void dbrewind( xFile_t *stream );
int dbfeof( xFile_t *stream );
int dbgetb( xFile_t *stream, char *byte );
int dbgetBew( xFile_t *stream, short *pword );
int dbgetBei( xFile_t *stream, int *pint);
int dbputb( xFile_t *stream, int bdata );
int dbputBew( xFile_t *stream, short wdata );
int dbputBei( xFile_t *stream, int idata );
int dbputLei( xFile_t *stream, int idata );
int dbputBed( xFile_t *stream, unsigned int data );
int dbreadByteBlk( xFile_t *stream, char *buff, int size );
int dbreadBewBlk( xFile_t *stream, short *pwdata, int size );
int dbreadBeiBlk( xFile_t *stream, int *pidata, int size );
int dbputbBlk( xFile_t *stream, char *pbdata, int size );
int dbputBewBlk( xFile_t *stream, short *pwdata, int size );
int dbputBeiBlk( xFile_t *stream, int *pidata, int size );
int dbputLeiBlk( xFile_t *stream, int *pidata, int size );
int dbGetFileList( char *filepath, char ***FileList );
void dbDelFileList( char **pList );
int dbFileLength( xFile_t *stream );
void dbSetRWFunc( void (*cb)(), unsigned int threshold );

int dbNull();
void dbNull1();
void dbNull2();
