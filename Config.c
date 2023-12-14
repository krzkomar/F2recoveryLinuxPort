#include "FrameWork.h"

static void *(*gCfgMalloc)(unsigned int nsize) = CfgMallocMem;
static void *(*gCfgRealloc)(void *ptr, size_t size ) = CfgReallocMem;
static void (*gCfgFree)(void *) = CfgFreeMem;

static int CfgInsertEntry( Config_t *cfg, char *SectionName, Config_t *NewEntry );

void *CfgMallocMem( unsigned int nsize )
{
    return malloc( nsize );
}

void *CfgReallocMem( void *ptr, size_t size )
{
    return realloc(ptr, size);
}

void CfgFreeMem( void *ptr )
{
    free( ptr );
}

int CfgInitSection( Config_t *cfg, int SectCount, ConfigMtd_t *Methods, int sectsize )
{
    cfg->RootCount = SectCount;
    cfg->RootAllocated = sectsize;
    cfg->nSect = 0;
    if( Methods ){
        cfg->Methods = *Methods;
    } else {
        cfg->Methods.Write = NULL;
        cfg->Methods.Args[ 0 ] = 0;
        cfg->Methods.Args[ 1 ] = 0;
        cfg->Methods.Read = NULL;
    }
    if( cfg->RootCount ){        
        if( !(cfg->Section = gCfgMalloc( SectCount * sizeof( ConfigTree_t ) )) ) return -1;
    } else {
        cfg->Section = NULL;
    }
    cfg->Value = CFG_ROOT_MARKER;
    return 0;
}

int CfgSectionExpand( Config_t *cfg, int size )
{
    ConfigTree_t *p;

    if( cfg->Value != CFG_ROOT_MARKER ) return -1;
    if( size < cfg->nSect ) return -1;
    if( !(p = gCfgRealloc( cfg->Section, size * sizeof( ConfigTree_t ))) ) return -1;
    cfg->Section = p;
    cfg->RootCount = size;
    return 0;
}

int CfgDelete( Config_t *cfg )
{
    int i;

    if( cfg->Value != CFG_ROOT_MARKER ) return -1;
    for( i = 0; i < cfg->nSect; i++ ){
        if( cfg->Section[ i ].Name ) gCfgFree( cfg->Section[ i ].Name );
        if( cfg->Section[ i ].SubSect ) gCfgFree( cfg->Section[ i ].SubSect );
    }
    if( cfg->Section ) gCfgFree( cfg->Section );
    memset( cfg, 0x00, sizeof( Config_t ) );
    return 0;
}

int CfgLookupSection( Config_t *Cfg, const char *Name, int *Position )
{
    int pos, Hi, Lo, match;

    if( Cfg->Value != CFG_ROOT_MARKER ) return -1;    
    *Position = 0;
    match = 0;

    if( Cfg->nSect == 0 ) return -1;
    Hi = Cfg->nSect - 1;
    Lo = 0;
    while( Hi >= Lo ){
        pos = ( Hi + Lo ) / 2;
        match = strcasecmp( Name, Cfg->Section[ pos ].Name );
        if( match == 0 ) break;
        if( match >= 0 )
            Lo = pos + 1;
        else
            Hi = pos - 1;
    }
    if( match == 0 ) {
        *Position = pos;
        return 0;
    }
    *Position = ( match < 0 ) ? pos : pos + 1;    
    return -1;
}

int CfgFindSection( Config_t *Cmd, const char *Name )
{
    int pos;

    if( Cmd->Value != CFG_ROOT_MARKER ) return -1;
    if( CfgLookupSection( Cmd, Name, &pos ) ) return -1;
    return pos;
}

static int CfgInsertEntry( Config_t *cfg, char *SectionName, Config_t *NewEntry )
{
    int idx, i;
    Config_t *p;
    char *name;

    if( cfg->Value != CFG_ROOT_MARKER ) return -1;
    if( !CfgLookupSection(cfg, SectionName, &idx) ) return -1; // section already exist

    if( cfg->RootCount == cfg->nSect ){
        if( CfgSectionExpand( cfg, 2 * (cfg->RootCount + 1) ) == -1 ) return -1;
    }
    if( !(name = gCfgMalloc( strlen( SectionName ) + 1 )) ) return -1;

    strcpy( name, SectionName );
    if( !NewEntry || !cfg->RootAllocated ){
        p = NULL;
    } else {	
	if( !(p = gCfgMalloc( cfg->RootAllocated )) ){ gCfgFree( name ); return -1; }
    }

    if( p ){
        if( cfg->RootAllocated ) memcpy( p, NewEntry, cfg->RootAllocated );
    }

    for( i = cfg->nSect; i > idx; i-- ){
        memcpy( &cfg->Section[ i ], &cfg->Section[ i - 1 ], sizeof( ConfigTree_t ) );
    }
    cfg->Section[ idx ].Name = name; // key
    cfg->Section[ idx ].SubSect = p; // value
    cfg->nSect++;

    return 0;
}

int CfgDeleteSection( Config_t *cfg, char *SectionName )
{
    int i, idx;

    if( cfg->Value != CFG_ROOT_MARKER ) return -1;
    if( CfgLookupSection( cfg, SectionName, &idx ) == -1 ) return -1;
    gCfgFree( cfg->Section[ idx ].Name );
    if( cfg->Section[ idx ].SubSect ) gCfgFree( cfg->Section[ idx ].SubSect );
    cfg->nSect--;
    for( i = idx; i < cfg->nSect; i++ ){
        memcpy( &cfg->Section[ i ], &cfg->Section[ i + 1 ], sizeof( cfg->Section[ i ] ) );
    }
    return 0;
}

int CfgAttachBranch( Config_t *cfg, Config_t *branch )
{
    int i;

    if( branch->Value != CFG_ROOT_MARKER ) return -1;
    if( CfgInitSection( cfg, branch->RootCount, &branch->Methods, branch->RootAllocated ) ) return 0;
    if( branch->nSect <= 0 ) return 0;
    for( i = 0; i < branch->nSect; i++ ){
        if( CfgInsertEntry( cfg, branch->Section[i].Name, branch->Section[i].SubSect) == -1 ) break;
    }
    return 0;
}

int CfgUnk01( Config_t *cfg, const char *SecName, int Position )
{
    unsigned int len;
    char *Name;
    int pos;

    pos = Position;
    if( cfg->Value != CFG_ROOT_MARKER ) return -1;
    if( Position == -1 )
	 CfgLookupSection( cfg, SecName, &pos );
    else
        pos = Position + 1;
    if( pos >= cfg->nSect ) return -1; // pozycja spoza zakresu

    len = strlen( SecName ) + 1;
    Name = cfg->Section[ pos ].Name;
    if( strncasecmp( Name, SecName, len - 1) ) return -1;
/*
    r = Name + v8;
    end_s = Name + len - 1;
    if( r[ 0 ] == 0 || r[ 0 ] == ' '){
	if( r[1] == '(' || Name[ len - 1 ] == ' ' ){ 
	    if( Name[ len ] == '[' || end_s[ 0 ] == '/' ){ 
		if( end_s[ 1 ] == 'T' && end_s[ 2 ] == 'L' ) return pos;
    	    }
        }
    }
*/
    return -1;
}


int CfgGetFileInt( FILE *fh, int *Val )
{
    int tmp, a;
    
    if( (tmp = fgetc(fh)) == -1 ) return -1;
    if( (a = fgetc(fh)) == -1 ) return -1;
    tmp = (tmp << 8) | (a & 0xff);
    if( (a = fgetc(fh)) == -1 ) return -1;
    tmp = (tmp << 8) | (a & 0xff);
    if( (a = fgetc(fh)) == -1 ) return -1;
    tmp = (tmp << 8) | (a & 0xff);
    *Val = tmp;
    return 0;
}

int CfgRootLoad( FILE *fh, Config_t *cfg )
{
    int tmp;

    if ( CfgGetFileInt(fh, &tmp) ) return -1;
    cfg->nSect = tmp;
    if ( CfgGetFileInt(fh, &tmp) ) return -1;
    cfg->RootCount = tmp;
    if ( CfgGetFileInt(fh, &tmp) ) return -1;
    cfg->RootAllocated = tmp;
    if ( CfgGetFileInt(fh, &tmp) ) return -1;
    cfg->iDat = tmp;
    return 0;
}

int CfgLoadData( FILE *fh, Config_t *cfg, int args )
{
    int i, n;
    char **s;

    if( cfg->Value != CFG_ROOT_MARKER ) return -1;    
    for( i = 0; i < cfg->nSect; i++ ){
        if( cfg->Section[ i ].Name ) gCfgFree( cfg->Section[ i ].Name );
        if( cfg->Section[ i ].SubSect ) gCfgFree( cfg->Section[ i ].SubSect );
    }
    if( cfg->Section ) gCfgFree(cfg->Section);
    if( CfgRootLoad( fh, cfg ) ) return -1;
    cfg->Section = NULL;
    if( cfg->RootCount <= 0 ) return 0;
    if( !(cfg->Section = gCfgMalloc( cfg->RootCount * sizeof( ConfigTree_t ) )) ) return -1;
    for( i = 0; i < cfg->nSect; i++ ){
        cfg->Section[i].Name = 0;
        cfg->Section[i].SubSect = 0;
    }
    for( i = 0; i < cfg->nSect; i++ ){
        if( (n = fgetc( fh )) == -1 ) return -1;
        cfg->Section[ i ].Name = gCfgMalloc( n + 1 );
        s = &cfg->Section[ i ].Name;
        if( !*s || !fgets( *s, n + 1, fh ) ) return -1;
        if( cfg->RootAllocated ){
            cfg->Section[i].SubSect = gCfgMalloc( cfg->RootAllocated );
            if( !cfg->Section ) return -1;
            if( cfg->Methods.Read ){
                if( cfg->Methods.Read(fh, cfg->Section[i].SubSect, cfg->RootAllocated, args ) ) return -1;
            } else if( fread( cfg->Section[i].SubSect, cfg->RootAllocated, 1, fh ) != 1 ){
                return -1;
            }
        }
    }
    return 0;
}

int CfgPutFileInt( FILE *fh, int n )
{
    if( fputc(n, fh) == -1 || fputc(n >> 8, fh) == -1 || fputc(n >> 16, fh) == -1 || fputc(n >> 24, fh) == -1 )
        return -1;
    else
        return 0;
}

int CfgSaveRoot( FILE *fh, Config_t *cfg )
{
    if( CfgPutFileInt( fh, cfg->nSect ) ) return -1;
    if( CfgPutFileInt( fh, cfg->RootCount ) ) return -1;
    if( CfgPutFileInt( fh, cfg->RootAllocated ) ) return -1;
    if( CfgPutFileInt( fh, cfg->iDat ) )return -1;
    return 0;
}

int CfgSaveData( FILE *fh, Config_t *cfg, int arg )
{
    int i;

    if( cfg->Value != CFG_ROOT_MARKER || CfgSaveRoot(fh, cfg) ) return -1;
    if( cfg->nSect <= 0 ) return 0;
    for( i = 0; i < cfg->nSect; i++ ){
        if( fputc(strlen(cfg->Section[ i ].Name), fh) == -1 ) break;
        if( fputs(cfg->Section[i].Name, fh) == -1 ) break;
        if( cfg->Methods.Write ){
            if( cfg->RootAllocated && cfg->Methods.Write(fh, cfg->Section[ i ].SubSect, cfg->RootAllocated, arg ) ) return -1;
        } else {
            if( cfg->RootAllocated && fwrite( cfg->Section[ i ].SubSect, cfg->RootAllocated, 1, fh ) != 1 ) return -1;
        }
    }

    return 0;
}

void CfgSetMemMng( void *(*malloc_)(unsigned int nsize), void *(*realloc_)(void *ptr, size_t size ), void (*free_)(void *) )
{
    if( malloc_ && realloc_ && free_ ){
        gCfgMalloc = malloc_;
    }else{
        free_ = CfgFreeMem;
        gCfgMalloc = CfgMallocMem;
        realloc_ = CfgReallocMem;
    }
    gCfgFree = free_;
    gCfgRealloc = realloc_;
}

/* inny plik */

int CfgInit( Config_t *cfg )
{
    return cfg && !CfgInitSection( cfg, 10, NULL, 10 * sizeof( void * ) );
}

void CfgFree( Config_t *Cfg )
{
    int i, j;
    Config_t *SubSect;

    if( !Cfg ) return;
    for( i = 0; i < Cfg->nSect; i++ ){
        SubSect = Cfg->Section[ i ].SubSect;        
        for( j = 0; j < SubSect->nSect; j++ ){
            Free( SubSect->Section[ j ].SubSect->Value );
            SubSect->Section[ j ].SubSect = NULL;
        }
        CfgDelete( SubSect );
    }
    CfgDelete( Cfg );
}

int CfgCmdLineParser( Config_t *cfg, int Argc, char **Argv )
{
    char *SectName, *p, Value[260], KeyName[260], *st;
    int i;

    if( !cfg ) return 0;
    for( i = 0; i < Argc; i++ ){
        if( (SectName = strchr( *Argv, '[' )) ){
            SectName++;
            p = strchr( SectName, ']' );
            st = p;
            if( p ){
                *p = '\0';
                if( CfgFetchKeyValuePair( p + 1, KeyName, Value ) == 1 ){
                    if( !CfgSetString( cfg, SectName, KeyName, Value ) ){
                        *p = ']';
                        return 0;
                    }
                }
                *st = ']';
            }
        }
        Argv++;
    }
    return 1;
}

int CfgGetString( Config_t *Cfg, const char *SectionName, const char *KeyName, char **pStr )
{
    int pos;
    Config_t *Sect;

    if( ( !Cfg ) || ( !SectionName ) || ( !KeyName ) || ( !pStr ) ) return 0;
    pos = CfgFindSection(Cfg, SectionName);
    if( pos == -1 ) return 0;
    Sect = Cfg->Section[ pos ].SubSect;
    pos = CfgFindSection( Sect, KeyName );
    if( pos == -1 ) return 0;
    *pStr = Sect->Section[ pos ].SubSect->Value;
    return 1;
}

int CfgSetString( Config_t *Cfg, char *SectionName, char *KeyName, char *sValue )
{
    Config_t *SubSect;
    int idx;
    char *Value;

    if( !Cfg || !SectionName || !KeyName || !sValue ) return 0;
    idx = CfgFindSection( Cfg, SectionName );
    if( idx == -1 ){
        if( CfgSetEntry( Cfg, SectionName ) == -1 ) return 0;
        idx = CfgFindSection( Cfg, SectionName );
    }

    SubSect = Cfg->Section[ idx ].SubSect;
    idx = CfgFindSection( SubSect, KeyName );
    if( idx != -1 ){ // entry already exist -> delete it
        Free( SubSect->Section[ idx ].SubSect->Value );
        SubSect->Section[ idx ].SubSect = NULL;
        CfgDeleteSection( SubSect, KeyName );
    }    
    if(( Value = StrDup( sValue ))){ // create new entry
        if( CfgInsertEntry( SubSect, KeyName, (Config_t *)&Value ) != -1 ) return 1;
        Free( Value );
    }
    return 0;
}

int CfgGetInteger( Config_t *Cfg, const char *SectionName, const char *EntryName, int *iValue )
{
    char *tmp;

    if( !EntryName || CfgGetString( Cfg, SectionName, EntryName, &tmp ) != 1 ) return 0;
    *iValue = strtol( tmp, NULL, 10 );
    return 1;
}

int CfgGetIntParamList( Config_t *Cfg, const char *SectionName, char *EntryName, int *ValueList, int Parameters )
{
    int *value;
    char *p, tmp[256], *q;

    value = ValueList;
    if( !ValueList || Parameters < 2 ) return 0;
    if( !CfgGetString( Cfg, SectionName, EntryName, &q ) ) return 0;
    q = strncpy( tmp, q, 255 );
    while( 1 ){
        p = strchr( q, ',' );
        if( !p ) break;
        if( --Parameters == -1 ) break;
        *p = '\0';
        *value++ = strtol( q, NULL, 10 );
        q = p + 1;
    }
    if( Parameters > 1 ) return 0;
    *value = strtol( q, NULL, 10 );
    return 1;    
}

int CfgSetInteger( Config_t *Cfg, char *SectionName, char *KeyName, int iValue )
{
    char tmp[ 32 ];

    sprintf( tmp, "%d", iValue );
    return CfgSetString( Cfg, SectionName, KeyName, tmp );
}

int CfgSetIntParamList( Config_t *cfg, char *SectionName, char *KeyName, int *ValueList, int Count )
{
    char Line[512], Str[16];

    *Line = '\0';
    while( --Count != -1 ){
	sprintf( Str, "%d", *ValueList++ );
        strcpy( Line + strlen( Line ), Str );
        if( Count ) strcpy( Line + strlen( Line ), "," );
    }
    return CfgSetString( cfg, SectionName, KeyName, Line ) != 0;
}

int CfgLoadFromFile( Config_t *Cfg, const char *CfgPath, int DatabaseFlag )
{
    xFile_t *dbh;
    FILE *fh;
    char buffer[256];

    if( !Cfg || !CfgPath ) return 0;

    if( DatabaseFlag == 1 ){
        if( (dbh = dbOpen( CfgPath, "rt" ) ) ){
            while( dbgets( buffer, 256, dbh ) ) CfgParser( Cfg, buffer );
            dbClose( dbh );
        }
    } else {
        if( ( fh = fopen( CfgPath, "rt" ) ) ){
            while( fgets( buffer, 256, fh ) ) CfgParser( Cfg, buffer );;
            fclose( fh );
        }
    }
    return 1;
}

int CfgSaveFile( Config_t *Cfg, const char *CfgPath, int Mode )
{
    int err, i, j;
    xFile_t *rFile;
    FILE *File;
    Config_t *sect;

    err = 0;
    if( !Cfg || !CfgPath ) return 0;
    if( Mode != 1 ){                    
        if( (File = fopen(CfgPath, "wt")) ){ // Write in text mode
            for( j = 0; j < Cfg->nSect; j++ ){
                fprintf( File, "[%s]\n", Cfg->Section[ j ].Name ); // Write section name.
                sect = Cfg->Section[ j ].SubSect;                    
                for( i = 0; i < sect->nSect; i++ ){
                    fprintf( File, "%s=%s\n", sect->Section[ i ].Name, sect->Section[ i ].SubSect->Value );
                }
                fprintf(File, "\n");                    
            }
            err = 1;
            fclose(File);
        }
        return err;
    }        
    rFile = dbOpen( CfgPath, "wt" );
    if( !rFile ) return err;
    for( j = 0; j < Cfg->nSect; j++ ){
        dbPrintf( rFile, "[%s]\n", Cfg->Section[ j ].Name);
        sect = Cfg->Section[ j ].SubSect;            
        for( i = 0; i < sect->nSect; i++ ){
            dbPrintf( rFile, "%s=%s\n", sect->Section[ i ].Name, sect->Section[ i ].SubSect->Value );
        }
        dbPrintf( rFile, "\n" );
    }
    dbClose( rFile );
    return 1;
}

int CfgParser( Config_t *Cfg, char *input )
{
    static char SectName[ 256 ] = "unknown"; // STATIC !!
    char *s, *sec_start;
    int err;
    char value[ 256 ];
    char Key[ 272 ];

    if( (s = strchr( input, ';')) ) *s = '\0';  // postaw znak konca w miejscu ';'
    if( (s = strchr( input, '\r')) ) *s = '\0';
    if( (s = strchr( input, '\n')) ) *s = '\0';
    if( (s = strchr( input, '[')) ){

	sec_start = ++s;
	if( (s = strchr( s, ']')) ){  // nazwa sekcji znaleziona
    	    *s = '\0';                              // koniec nazwy sekcji
    	    strcpy( SectName, sec_start );
    	    return CfgValidateSectionName( SectName );
	}
    }
    err = CfgFetchKeyValuePair( input, Key, value );
    if( err == 1 ) return CfgSetString( Cfg, SectName, Key, value );
    return err;
}

int CfgFetchKeyValuePair( char *input, char *key_name, char *value )
{
    char *s;

    if( !input || !key_name || !value ) return 0;
    s = strchr( input, '=' );
    if( !s ) return 0;
    *s = '\0';
    strcpy( key_name, input );
    strcpy( value, s + 1 );
    *s = '=';
    CfgValidateSectionName( key_name );
    CfgValidateSectionName( value );
    return 1;
}

int CfgSetEntry( Config_t *cfg, char *SectName )
{
    Config_t tmp;
    
    if( !cfg || !SectName ) return 0;
    if( CfgFindSection( cfg, SectName ) != -1 ) return 1;
    if( CfgInitSection( &tmp, 10, 0, sizeof( char *) ) == -1) return 0;

    if( CfgInsertEntry( cfg, SectName, &tmp ) == -1) return 0;
    return 1;
}

int CfgValidateSectionName( char *Str )
{
    char *str;
    unsigned int len, i, j;
    char *s, c;

    str = Str;
    if( !Str ) return 0;    
    len = strlen(Str) + 1;
    if( len == 1 ) return 1;    
    i = len - 1;
    s = &str[len - 1];
    do{
        --i;
        c = *--s;
    }while( (IsTable[ (int)(c + 1) ] & 2) != 0 && i );
    s = str;
    str[i + 1] = '\0';
    j = 0;
    while( (IsTable[ (int)(*s + 1) ] & 2) != 0 ){
        ++j;
        ++s;
    }
    if( j ) memmove( str, s, len - j );
    return 1;
}

int CfgGetFloat( Config_t *cfg, char *SectName, char *KeyName, double *Val )
{
    char *value;

    if( !Val || CfgGetString(cfg, SectName, KeyName, &value ) != 1 ) return 0;
    *Val = strtod( value, NULL );
    return 1;
}

int CfgSetDouble( Config_t *Cfg, char *SectionName, char *KeyName, double dValue )
{
    char tmp[ 44 ];

    sprintf( tmp, "%.6f", dValue );
    return CfgSetString( Cfg, SectionName, KeyName, tmp );
}

