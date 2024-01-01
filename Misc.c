#include "FrameWork.h"

int gMenuEscape = 0;
int gCurrentMapLvl = 0;
int gCurrentMapId = 0;
char **gDlgLines;
int gLangFilter = -1;

const char IsTable[ 256 ] = {
//  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F    
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,

    0x01, 0x0A, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48,
    0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
    0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x0C, 0x0C, 0x0C, 0x0C,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int gSDLReady = 0;
int gLanguageId;

char *StrDup( char *str )
{
    char *p;
    
    p = Malloc( strlen( str ) + 1);
    strcpy( p, str );
    return p;
}

char *StrUpr( char *s )
{
    char *p = s;
    for( ;*p; p++ ) *p = toupper( *p );
    return s;
}

char *StrLwr( char *s )
{
    char *p = s;
    for( ;*p; p++ ) *p = tolower( *p );
    return s;
}

int abs32( int x )
{
    return (x < 0) ? -x:x;
}

int FileListSort( char **a1, char **a2 )
{
    return strcmp( *a1, *a2 );
}

char **FileListMake( char *path, int *pCnt )
{
    int cnt;
    char **list;

    cnt = dbGetFileList( path, &list );
    *pCnt = cnt;
    if( !cnt ) return NULL;
    qsort( list, cnt, sizeof( char *), (void *)FileListSort );
    return list;
}

void FileListFree( char **FileList )
{
    dbDelFileList( FileList );
}




// dummies




