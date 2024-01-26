#include "FrameWork.h"

#define EXPORT_ENTRIES	1013
#define RANDOM_ENTRY( exp, id )		{ char *_s; id = 0; for( _s = exp; *_s; _s++ ) id += tolower( *_s ) + 8*id + (id >> 29); id = id % EXPORT_ENTRIES; }
#define FREE_VALSTR( exp ) 		if( (exp->Flags & 0xF7FF) == SCR_STRING ) dbg_free( exp->ValStr ); // 9001 | 9801
#define TYPE_STRING( type )    		( (type) & 0xf7ff ) == SCR_STRING

static Export_t gExportProcBase[ EXPORT_ENTRIES ]; // procedures?
static Export_t gExportVarsBase[ EXPORT_ENTRIES ]; // variables, arguments ?

unsigned int ExportRandomEntry( char *Name )
{
    unsigned int Id;
        
    RANDOM_ENTRY( Name, Id );
    return Id;
}

Export_t *ExportSearchProc( char *Name )
{
    unsigned int Idx, n;

    RANDOM_ENTRY( Name, Idx );
    n = Idx;
    if( gExportProcBase[ Idx ].Itp && !strcasecmp( gExportProcBase[ Idx ].Name, Name ) ) return &gExportProcBase[ Idx ];
    do{
        Idx += 7;
        if( Idx >= EXPORT_ENTRIES ) Idx -= EXPORT_ENTRIES;
        if( gExportProcBase[ Idx ].Itp && !strcasecmp( gExportProcBase[ Idx ].Name, Name ) ) return &gExportProcBase[ Idx ];
    }while( Idx != n );
    return NULL;
}

Export_t *ExportGetEmptyProc( char *name )
{
    unsigned int Idx, n;

    RANDOM_ENTRY( name, Idx );
    n = Idx;
    if( !gExportProcBase[ Idx ].Name[ 0 ] ) return &gExportProcBase[ Idx ];
    do{
        Idx += 7;
        if( Idx >= EXPORT_ENTRIES ) Idx -= EXPORT_ENTRIES;
        if( !gExportProcBase[ Idx ].Name[ 0 ] ) return &gExportProcBase[ Idx ];
    }while( Idx != n );
    return 0;
}

Export_t *ExportSearchVar( char *Name )
{
    unsigned int idx, n;

    RANDOM_ENTRY( Name, idx );
    n = idx;
    if( !strcasecmp( gExportVarsBase[ idx ].Name, Name ) ) return &gExportVarsBase[ idx ];
    do{ 
        if( !gExportVarsBase[ idx ].Name[0] ) break;
	idx += 7;
        if( idx >= EXPORT_ENTRIES ) idx -= EXPORT_ENTRIES;
        if( !strcasecmp( gExportVarsBase[ idx ].Name, Name ) ) return &gExportVarsBase[ idx ];
    } while( idx != n );
    return 0;
}

int ExportGetVar( char *Name, short *pType, char **pStr )
{
    Export_t *exp;
    
    if( !(exp = ExportSearchVar( Name ) ) ){
	*pType = 0;
	*pStr = NULL;
	return 0;
    }
    *pType = exp->Flags;
    *pStr = exp->ValStr;
    return 1;
}

Export_t *ExportGetUndefVar( char *Name )
{
    unsigned int idx, n;

    RANDOM_ENTRY( Name, idx );
    
    n = idx;
    if( !gExportVarsBase[ idx ].Name[ 0 ] ) return &gExportVarsBase[ idx ];
    do{
        idx += 7;
        if( idx >= EXPORT_ENTRIES ) idx -= EXPORT_ENTRIES;
        if( !gExportVarsBase[ idx ].Name[ 0 ] ) return &gExportVarsBase[ idx ];
    }while ( idx != n );
    return NULL;
}

int ExporSetFVar( char *Name, char *FloatStr ) // no xref
{
    Export_t *exp;

    if( !(exp = ExportSearchVar( Name )) ) return 1;
    FREE_VALSTR( exp );
    exp->Flags = SCR_FSTRING;
    exp->ValStr = strdup( FloatStr );
    return 0;
}

int ExportSetVarArg( Intp_t *intp, char *Name, short Type, int ArgIdx )
{
    Export_t *exp;
    char *Arg;

    if( !( exp = ExportSearchVar( Name ) ) ) return 1;
    FREE_VALSTR( exp );
    if( TYPE_STRING( Type ) ){
        if( !intp ) return 0;
        Arg = IntpGetString( intp, Type >> 8, ArgIdx );
        exp->Flags = SCR_FSTRING;
        exp->ValStr = dbg_malloc( strlen( Arg ) + 1 );
        strcpy( exp->ValStr, Arg );        
    } else {
        exp->ValInt = ArgIdx;
        exp->Flags = Type;
    }
    return 0;
}

int ExportSetVarArgP( Intp_t *intp, char *Name, void *ArgIdx )
{
    Export_t *exp;

    if( !( exp = ExportSearchVar( Name ) ) ) return 1;
    FREE_VALSTR( exp );
    exp->ValPtr = ArgIdx;
    exp->Flags = SCR_PTR;
    return 0;
}


int ExportSetVar( char *Name, short Type, char *Str )
{
    Export_t *exp;

    if( !( exp = ExportSearchVar( Name ) ) ) return 1;
    FREE_VALSTR( exp );
    if( TYPE_STRING( Type ) ){
        exp->Flags = SCR_FSTRING;
        exp->ValStr = dbg_malloc( strlen( Str ) + 1 );
        strcpy( exp->ValStr, Str );
    } else {
        exp->ValStr = Str; // float or int
        exp->Flags = Type;
    }
    return 0;
}

int ExportGetVarArg( Intp_t *intp, char *Name, short *pFlags, int *pArg )
{
    Export_t *exp;
    
    if( !(exp = ExportSearchVar( Name )) ) return 1;
    *pFlags = exp->Flags;
    if( exp->Flags == SCR_PTR ){
	*((void **)pArg) = exp->ValPtr;
	return 0;
    }
    if( TYPE_STRING( exp->Flags ) )
        *pArg = IntpAddString( intp, exp->ValStr ); // string index
    else
        *pArg = exp->ValInt;
    return 0;
}


int ExportCreateIVar( char **pFname, char *Name )
{
    Export_t *exp;
    
    if( (exp = ExportSearchVar( Name )) ){
        if( strcasecmp( exp->Fname, *pFname ) ) return 1;
	FREE_VALSTR( exp );
    } else {        
        if( !(exp = ExportGetUndefVar( Name ) ) ) return 1;
        strncpy( exp->Name, Name, 31 );
        exp->Fname = dbg_malloc( strlen( *pFname ) + 1 );
        strcpy( exp->Fname, *pFname );
    }
    exp->Flags = SCR_INT;
    exp->ValStr = 0;
    return 0;
}

int ExportDeleteProc( Intp_t *itp )
{
    int i;

    for( i = 0; i != EXPORT_ENTRIES; i++ ){
        if( itp == gExportProcBase[ i ].Itp ){
            gExportProcBase[ i ].Name[ 0 ] = '\0';
            gExportProcBase[ i ].Itp = NULL;
        }
    }
    return i;
}

void ExportFlushProc()
{
    IntpTaskAdd( (void *)ExportDeleteProc );
}

void ExportFlushVars()
{
    int i;

    for( i = 0; i != EXPORT_ENTRIES; i++ ){
        if( gExportVarsBase[ i ].Name[ 0 ] ) dbg_free( gExportVarsBase[ i ].Fname );
        if( gExportVarsBase[ i ].Flags == SCR_FSTRING ) dbg_free( gExportVarsBase[ i ].ValStr );
    }
}

Intp_t *ExportGetProcedure( char *ProcName, int *pStr, short *IntCtx )
{
    Export_t *exp;

    exp = ExportSearchProc( ProcName );
    if( !exp || !exp->Itp ) return NULL;
    *pStr = exp->ValInt;
    *IntCtx = exp->Flags;
    return exp->Itp;
}

int ExportSetProcedure( Intp_t *itp, char *ProcName, void *Procedure, short Type )
{
    Export_t *exp1, *exp2;
    
    if( ( exp1 = ExportSearchProc( ProcName ) ) ){
        if( itp != exp1->Itp ) return 1;
    } else {        
        if( !( exp2 = ExportGetEmptyProc( ProcName ) ) )return 1;
        strncpy( exp2->Name, ProcName, 31 );
    }
    exp1->Flags = Type;
    exp1->ValStr = Procedure;
    exp1->Itp = itp;
    return 0;
}

char **ExportCreateListOfVariables( int *pNum )
{
    int Idx, j, cnt;
    char stmp[ 512 ], type[ 100 ], Value[ 100 ], **VarList;

    VarList = NULL;
    for( j = 1; j <= 2; j++ ){
        cnt = 0;
        for( Idx = 0; Idx < EXPORT_ENTRIES; Idx++){
            if( !gExportVarsBase[ Idx ].Name[ 0 ] ) continue;
    	    if( j == 2 ){
                switch( gExportVarsBase[ Idx ].Flags & 0xf7ff ){
                    case SCR_STRING: strcpy( type, "STR" ); sprintf( Value, "'%s'", gExportVarsBase[ Idx ].ValStr ); break;
            	    case SCR_FLOAT: strcpy( type, "FLOAT" ); sprintf( Value, "%f", gExportVarsBase[ Idx ].ValFloat ); break;
            	    case SCR_INT: strcpy( type, "INT" ); sprintf( Value, "%d", gExportVarsBase[ Idx ].ValInt ); break;
            	    default: sprintf( type, "Unkn(%u)", gExportVarsBase[ Idx ].Flags ); sprintf( Value, "%d", gExportVarsBase[ Idx ].ValInt ); break;
                }
                sprintf( stmp, "%s:%s:%s", gExportVarsBase[ Idx ].Name, type, Value );
                VarList[ cnt ] = strdup( stmp );
    	    }
    	    cnt++;
        }
        if( j == 1 ) VarList = dbg_calloc( sizeof( char * ), cnt );
    }
    *pNum = cnt;
    return VarList;
}

void ExportFreeList( char **List, int cnt )
{
    int i;

    if( !List ) return;
    for( i = 0; i < cnt; i++ ) dbg_free( List[ i ] );
    dbg_free( List );
}

void ExportUndefVars()
{
    int i;

    for( i = 0; i < EXPORT_ENTRIES; i++ ){
        if( !gExportVarsBase[ i ].Name[ 0 ] ) continue;        
        if( (gExportVarsBase[ i ].Flags & ~0x800) == SCR_STRING && gExportVarsBase[ i ].ValStr ) dbg_free( gExportVarsBase[ i ].ValStr );
        gExportVarsBase[ i ].Name[ 0 ] = '\0';
        if( gExportVarsBase[ i ].Fname ){
            dbg_free( gExportVarsBase[ i ].Fname );
            gExportVarsBase[ i ].Fname = NULL;
        }
        gExportVarsBase[ i ].Flags = 0;        
    }
}

void ExportVarsLoad ( xFile_t *fh )
{
    Export_t *p;
    int i, Flags, v13, nsize, buffer;

    dbtell( fh );
    dbread( &buffer, 4, 1, fh );    
    p = dbg_malloc( sizeof( Export_t ) );
    for( i = 0; i < buffer; i++ ){
        dbread( &v13, 4, 1, fh );
        WinDlgProcess();
        memset( p, 0, sizeof( Export_t ) );
        dbread( &p->Flags, 2, 1, fh );
        Flags = p->Flags & ~0x800;
        if( Flags < 0xA001 && Flags == 0x9001 ){
            dbread( &nsize, 4, 1, fh );
            p->ValStr = dbg_malloc( nsize );
    	    dbread( p->ValStr, nsize, 1, fh );
        } else {
    	    dbread( p->ValStr, 4, 1, fh );
        }
        dbread( &nsize, 4, 1, fh );
        dbread( p, nsize, 1, fh );
        dbread( &nsize, 4, 1, fh );
        p->Fname = dbg_malloc( nsize );
        dbread( p->Fname, nsize, 1, fh );
        ExportCreateIVar( &p->Fname, p->Name );
        ExportSetVar( p->Name, p->Flags, p->ValStr );
        dbg_free( p->Fname );
        if( (p->Flags & ~0x800) == 0x9001 ) dbg_free( p->ValStr );
    }
    dbg_free( p );
}

void ExportVarsSave( xFile_t *fh )
{
    int Flags, tmp, cnt, i, j;

    for( j = 1; j <= 2; j++ ){
        cnt = 0;
        for( i = 0; i < 1013; i++ ){
            if( gExportVarsBase[ i ].Name[0] ){
        	if( j == 2 ){
            	    dbwrite( &i, 4, 1, fh );
            	    dbwrite( &gExportVarsBase[ i ].Flags, 2, 1, fh );
        	}
        	Flags = gExportVarsBase[ i ].Flags & ~0x800;
        	if( Flags < 0xA001 ){
            	    if( Flags == 0x9001 ){
                	tmp = strlen( gExportVarsBase[ i ].ValStr ) + 1;
                	if( j == 2 ){
                    	    dbwrite( &tmp, 4, 1, fh );
                	    dbwrite( gExportVarsBase[ i ].ValStr, tmp, 1, fh );
                	}
            	    } else {
            		if( j == 2 ) dbwrite( &gExportVarsBase[ i ].ValStr, 4, 1, fh );
            	    }
        	} else {
        	    if( Flags == 0xA001 ){
            		if( j == 2 ) dbwrite( &gExportVarsBase[ i ].ValStr, 4, 1, fh );
        	    } else {
        		if( Flags == 0xc001 ){
        		    if( j == 2 ) dbwrite( &gExportVarsBase[ i ].ValStr, 4, 1u, fh );
        		} else {
        		    if( j == 2 ) dbwrite( &gExportVarsBase[ i ].ValStr, 4, 1, fh );
        		}
    		    }
        	}
        	if( j == 2 ){
            	    tmp = strlen( gExportVarsBase[ i ].Name ) + 1;
            	    dbwrite( &tmp, 4, 1, fh );
            	    dbwrite( &gExportVarsBase[ i ], tmp, 1, fh );
            	    tmp = strlen( gExportVarsBase[ i ].Fname ) + 1;
            	    dbwrite( &tmp, 4, 1, fh );
            	    dbwrite( gExportVarsBase[ i ].Fname, tmp, 1, fh );
        	}
        	cnt++;
            }            
        }
        if( j == 1 ) dbwrite( &cnt, 4, 1, fh );
    }    
}
