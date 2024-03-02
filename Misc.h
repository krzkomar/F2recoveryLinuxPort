#pragma once
#include <stdint.h>

#define SIZE_MB( n )	((n) << 20)
#define SIZE_KB( n )	((n) << 10)
// converts be to le and le to be
#define BSWAP32( n )	__builtin_bswap32( n )
#define PERCENT( percents, x )	((percents) * (x) / 100)

#define ARCH_64 UINTPTR_MAX == 0xffffffffffffffff
#define ARCH_32 UINTPTR_MAX == 0xffffffff

#define DI			if( gDbg )
#define DF_ON			gDbg = 1;
#define DF_OFF  		gDbg = 0;
#define DF( fmt, args... ) 	if( gDbg ){ DD printf( fmt, ##args ); }
extern int gDbg;

typedef union{
    int i;
    float f;
} CoatseFloat_u;

#define FLOAT( x )	((CoatseFloat_u)x).f


enum{
    DIR_NE,	// 0
    DIR_E,	// 1
    DIR_SE,	// 2
    DIR_SW,	// 3
    DIR_W,	// 4
    DIR_NW	// 5
};


typedef struct {
    int Width;
    int Height;
} Geo_t;

extern int gLangFilter;
extern int gMenuEscape;
extern const char IsTable[ 256 ];
extern int gSDLReady;
extern int gLanguageId;
extern Config_t gConfiguration;
extern int gCurrentMapId;

extern char **gDlgLines;
extern int gIfcWin;

char *StrDup( char *str );
char *StrUpr( char *str );
char *StrLwr( char *str );
int abs32( int );

extern char *gGamePath;

char **FileListMake( char *path, int *pCnt );
void FileListFree( char **FileList );
int FileRename( char *, char * );

void SysQuitDlg();



