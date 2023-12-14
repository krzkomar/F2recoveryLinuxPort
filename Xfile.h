#pragma once
#include "Dfile.h"
#include "Zfile.h"

#define XDB_DBASE	2
#define XDB_FILE	1
#define XDB_DIRECTORY	0

#define	FILE_RAW	0
#define	FILE_DB		1
#define	FILE_ZIP	2

enum{
    ERRXF_OK,		// 0
    ERRXF_NOFILE,	// 1
    ERRXF_ARGLIST,	// 2
    ERRXF_EXECFMT,	// 3
    ERRXF_FILENO,	// 4
    ERRXF_MEMORY,	// 5
    ERRXF_ACCESS,	// 6
    ERRXF_FILE_EXIST,	// 7
    ERRXF_XDEVLINK,	// 8
    ERRXF_ARG,		// 9
    ERRXF_FTOVF,	// 10
    ERRXF_OPENOVF,	// 11
    ERRXF_NOSPACE,	// 12
    ERRXF_ARGTOLARGE,	// 13
    ERRXF_RESTOLARGE,	// 14
    ERRXF_RESOURCE,	// 15
};


typedef struct // size of 8
{
    char	SrcType;	// +0
    union{
	void	*file;		// +4,5,6,7
	FILE	*rfile;	
	dFile_t	*dfile;
	zFile_t	*zfile;
    };
} xFile_t;

typedef struct _xList_t // size of 16
{
  char 			*Path;
  dFileBase_t 		*Dbase;
  int 			Flags;
  struct _xList_t 	*next;
} xList_t;

typedef struct _xPath_t // size of 12
{
  int 			nsize;
  char 			**FullPathList;
  struct _xPath_t 	*next;
} xPath_t;

typedef struct 
{
  char fpath[ 260 ];
  char flags;
  xPath_t *List;
} xFileSpec_t;

typedef struct 
{
  void 			*dir;
  struct dirent 	*entry;
} xDir_t;

extern int gxListGuard;

const char *xstrerror( int ErrNo );
void 	xclose_int( xFile_t *stream );
int 	xflush( xFile_t *stream );
xFile_t *xopen( const char *filename, const char *mode );
void 	xclose( const char *filename, const char *mode, xFile_t *stream );
void 	xsetbuf0( xFile_t *stream, void *buf );
void 	xsetbuf1( xFile_t *stream, char *bufffer, size_t size, int mode );
int 	xprintf( xFile_t *stream, char *format, ... );
int 	xscanf( xFile_t *stream, char *format, ... );
int 	xvprintf( xFile_t *stream, const char *fmt, va_list vl );
int 	xgetc_int( xFile_t *stream );
char 	*xgets_int( char *str, int num, xFile_t *stream );
int 	xputc_int( int c, xFile_t *stream );
int 	xputs_int( char *s, xFile_t *stream );
int 	xread_int( void *buffer, unsigned int size, unsigned int count, xFile_t *stream );
int 	xwrite_int( void *buffer, size_t size, size_t count, xFile_t *stream );
int 	xseek_int( xFile_t *stream, int offset, int origin );
int 	xtell_int( xFile_t *stream );
void 	xrewind_int( xFile_t *stream );
int 	xfeof_int( xFile_t *stream );
int 	xfilelength_int( xFile_t *stream );
void 	xLoadFileList( char *PathList );
void 	xEnumerate( int (*enumfunc)( char *path ));
int 	xOpenList( const char *path );
int 	xRemFromList( const char *path );
int 	xMakeDirList( char *filespec, int (*enumfunc)(xFileSpec_t *), xPath_t *pList );
int 	xLoadPathList( char *filespec, xPath_t *list );
void 	xUnloadFileList( xPath_t *list );
int 	xCreatePath( const char *path );
int 	xRemovePath( const char *path );
void 	xListDestroy();
int	filelength( FILE *file );
void	xCleanProcess();
int	xCleanAdd( void (*CleanFunc)() );

// extra
void	xSplitPath( const char *path, char *dir, char *fname, char *ext ); // removed drive from MS splitpath()
int	xSetCurrentDir( const  char *dir );
int	xDirCreate( const  char *dir );
int	xFileRemove( const char *fname );
