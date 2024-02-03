#include "FrameWork.h"

int *gGVals;
int gGValCount;

int gGlobFloatMsg = 0;

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
DD
    if( Idx < 0 || Idx >= gGValCount ){
DD
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

    InvSelectMain();
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

int GlobFloatMsgState()
{
    return gGlobFloatMsg;
}

int GlobVarFloatMsgInc( unsigned int a1 )
{
    switch( a1 ){
	case 0: a1 = 1; break;
	case 2: a1 = 3; break;
	case 4: a1 = 5; break;
    }
    if( gGlobFloatMsg == 4 && a1 == 5 ) return -1;
    gGlobFloatMsg = a1;
    return 0;
}

int GlobVarFloatMsgDec()
{
    switch( gGlobFloatMsg ){
        case 1: gGlobFloatMsg = 0; break;
        case 3: gGlobFloatMsg = 2; break;
        case 5: gGlobFloatMsg = 4; break;
        default: break;
    }
    return 0;
}
