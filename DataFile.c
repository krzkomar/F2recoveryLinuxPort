#include "FrameWork.h"

char *(*gDataFileUnk02)() = NULL;
char *(*gDataFileUnk01)( char *) = DataFileUnk01;

Pal8_t gDataFileUnk03[ 256 ];

/***********************************/

char *DataFileUnk01( char *fname )
{
    /* EMPTY */
    return fname;
}

void DataFileUnk02( char *(*a1)(char *) )
{
    gDataFileUnk01 = a1;
}

void DataFileUnk03( char *(*a1)() )
{
    gDataFileUnk02 = a1;
}

void DataFileUnk04( char *surf, Pal8_t *pal, int w, int h )
{
    int i;
    char rgb[ 256 ];

    rgb[ 0 ] = 0;
    for( i = 1; i != 256; i++ ){
        rgb[ i ] = gPalColorCubeRGB[ pal[ i ].r >> 3 ][ pal[ i ].g >> 3 ][ pal[ i ].b >> 3 ];
    }    
    for( i = 0; i < (w * h); i++ ) surf[ i ] = rgb[ (unsigned int)((unsigned char *)surf)[ i ] ];
}

void DataFileUnk05( char *surf, Pal8_t *pal, int w, int h )
{
    int i;
    char rgb[ 256 ];

    rgb[ 0 ] = 0;
    for( i = 1; i != 256; i++ ){
        rgb[ i ] = gPalColorCubeRGB[ pal[ i ].r >> 1 ][ pal[ i ].g >> 1 ][ pal[ i ].b >> 1 ];
    }    
    for( i = 0; i < (w * h); i++ ) surf[ i ] = rgb[ (unsigned int)((unsigned char *)surf)[ i ] ];
}

char *DataFileUnk06( char *fname, int *pW, int *pH )
{
    char *s;

    fname = gDataFileUnk01( fname );
    s = strrchr( fname, '.' );
    if( s ){
        if( !strcasecmp( s + 1, "pcx" ) ) return PcxLoad( fname, pW, pH, gDataFileUnk03 );
        if( gDataFileUnk02 ) return gDataFileUnk02();
    } else if( gDataFileUnk02 ){
        return gDataFileUnk02();
    }
    return NULL;
}

char *DataFileGetArt( char *fname, int *pW, int *pH )
{
    char *p;

    p = DataFileUnk06( fname, pW, pH );
    if( p ) DataFileUnk04( p, gDataFileUnk03, *pW, *pH );
    return p;
}

Pal8_t *DataFileUnk08( char *fname )
{
    char *p;
    int h, w;

    if( !(p = DataFileUnk06( fname, &w, &h ) ) ) return NULL;
    dbg_free( p );
    return gDataFileUnk03;
}

void DataFileUnk09(char *a1, int *a2, int *a3 )
{
    int v3,j,i,v13;
    char *v6,*v7,*v8,*p;

    v3 = *a2;
    v13 = *a3;
    
    p = (char *)dbg_malloc( *a2 * *a3 );
    v6 = p;
    v7 = a1;
    for( j = 0;j < v13; j++, v7 += v3 ){
        if( !*v7 ) break;        
        v8 = &a1[ v3 * j ];
        for( i = 0; i < v3; v6++, i++, v8++ ){
            if( !*v8 ) break;
            *v6 = *v8;            
        }
    }
    *a2 = i;
    *a3 = j;
    memcpy( a1, p, j * i );
    dbg_free( p );
}

Pal8_t *DataFileUnk10()
{
    return gDataFileUnk03;
}

char *DataFileUnk11( char *fname, int *pSize )
{
    xFile_t *fh;
    int len;
    char *buf;

    if( !(fh = dbOpen( gDataFileUnk01( fname ), "rb" ) ) ) return 0;
    len = dbFileLength( fh );    
    if( !(buf = dbg_malloc( len )) ){ *pSize = 0; return 0; }
    dbread( buf, 1, len, fh );
    dbClose( fh );
    *pSize = len;
    return buf;
}

