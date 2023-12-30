#include "FrameWork.h"

int *gGVals;
int gGValCount;

int gGlobUnk01 = 0;

int GlobVarGet( int GlobIdx )
{
    if( GlobIdx < 0 || GlobIdx >= gGValCount ){
	eprintf( "ERROR: attempt to reference global var out of range: %d", GlobIdx );
	return 0;
    }
    return gGVals[ GlobIdx ];
}

int GlobVarSet( int Idx, int Val )
{
    if( Idx < 0 || Idx >= gGValCount ){
        eprintf( "ERROR: attempt to reference global var out of range: %d", Idx );
        return -1;
    }
    gGVals[ Idx ] = Val;
    return 0;
}

int GlobVarLoad()
{
     return GlobVarLoadFile( "data/vault13.gam", "GAME_GLOBAL_VARS:", &gGValCount, &gGVals );
}

int GlobVarLoadFile( char *fname, char *SecName, int *pCount, int **pData )
{
    xFile_t *fh;
    char *p;
    char buf[ 260 ];

//    ScpUnk200();
    if( !(fh = dbOpen( fname, "rt" )) ) return -1;
    if( *pCount ){
        Free( *pData );
        *pData = NULL;
        *pCount = 0;
    }
    if( SecName ){
        while( dbgets( buf, 258, fh ) && strncmp( buf, SecName, 16 ) );
    }

    while( dbgets( buf, 258, fh ) ){
        if( buf[0] != '\n' && (buf[0] != '/' || buf[1] != '/') ){
            if( ( p = strchr( buf, ';' ) ) ) *p = '\0';
            (*pCount)++;
            *pData = (int *)Realloc( *pData, sizeof( int ) * *pCount );
            if( !(*pData) ) exit( 1 );
            if( (p = strchr( buf, '=' )) )
                sscanf( p + 1, "%d", &(*pData)[ *pCount - 1 ] );
            else
                (*pData)[ *pCount - 1 ] = 0;
        }
    }
    dbClose( fh );
    return 0;
}

int GlobUnk01()
{
    return gGlobUnk01;
}

int GlobVarUnk02( unsigned int a1 )
{
    if( a1 < 2 ){
        if( !a1 ) a1 = 1;
    } else if( a1 <= 2 ){
        a1 = 3;
    } else if( a1 == 4 ){
        a1 = 5;
    }
    if( gGlobUnk01 == 4 && a1 == 5 ) return -1;
    gGlobUnk01 = a1;
    return 0;
}

int GlobVarUnk03()
{
    switch( gGlobUnk01 ){
        case 1: gGlobUnk01 = 0; break;
        case 3: gGlobUnk01 = 2; break;
        case 5: gGlobUnk01 = 4; break;
        default: break;
    }
    return 0;
}
