#pragma once
#include <zlib.h>

// gz file header 
#define GZ_FILE_FLAGS 	0x00
#define GZ_TIMESTUMP	0x00, 0x00, 0x00, 0x00
#define GZ_COMP_FLAGS	0x00
#define GZ_SYSID	0x03

typedef struct // size of 100
{
    z_stream 	zip;
    int 	Error;
    int 	eof;
    void 	*file;
    void 	*pRdBuffRoot;
    void 	*pWrBuffRoot;
    int 	crc;
    void 	*p20;
    char 	*name;
    int 	RealFileFlag;
    char 	mode;
    int 	pos;
} zFile_t;

zFile_t *zOpenInt( const char *fname, const char *mode, int dbaseId );
zFile_t *zOpenByFileName( const char *fname, const char *mode );
zFile_t *zOpenByDbaseId( int dbaseId, const char *mode );
int zFlush_( zFile_t *stream, int CompLvl, int CompStrategy );
int zGetInt8( zFile_t *stream );
void zReadHdr( zFile_t *stream );
int zFileClose( zFile_t *zfile );
int zread( zFile_t *stream, char *buff, int nsize );
int zgetc( zFile_t *stream );
char *zgets( char *str, int nsize, zFile_t *stream );
int zputblock( zFile_t *str, void *buffer, int nsize );
int zvprintf( zFile_t *fh, const char *fmt, va_list vargs );
int zputc( zFile_t *stream, int chr );
int zputs( zFile_t *stream, char *str );
int zFlushInt( zFile_t *file, int arg );
int zflush( zFile_t *stream, int arg  );
int zseek( zFile_t *stream, int offset, int whence );
int zrewind( zFile_t *stream );
int ztell( zFile_t *stream );
int zfeof( zFile_t *stream );
int zGetInt32( zFile_t *stream );
int zclose( zFile_t *fh );
char **zerrorp();
