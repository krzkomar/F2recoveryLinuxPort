#include "FrameWork.h"

int gSoundSfxAcmFile = 0; // !

static int SfxCacheStrCmp( char *key, SfxCacheList_t *list );

int gSfxCacheDebug = 0x7FFFFFFF;
char *gSfxCacheLog = NULL;
SfxCache_t *gSfxCacheHandlers = NULL;
int gSfxCacheDirty = 0;
Cache_t *gSfxCache = NULL;
int gSfxCacheEnable = 0;
int gSfxCacheUnk03 = 1;
int gSfxCacheUnk10 = 0;
int gSfxCacheUnk07 = 0x7FFFFFFF;
char *gSfxCacheListPath = 0;
int gSfxCacheListPathLen = 0;
SfxCacheList_t *gSfxCacheList = NULL;
int gSfxCacheSfxCount = 0;

/**********************************************************************/

int SfxCacheInit( unsigned int CacheSize, const char *Name )
{
    if( CfgGetInteger(&gConfiguration, "sound", "debug_sfxc", &gSfxCacheDebug) != 1 ) gSfxCacheDebug = 1;
    if( CacheSize < 0x40000 ) return -1;
    gSfxCacheLog = strdup( ( Name ) ? Name : "" );
    if( !gSfxCacheLog ) return -1;
    if( SfxCacheLoadSfxList( gSfxCacheLog, gSfxCacheUnk03, gSfxCacheDebug ) ){
        Free( gSfxCacheLog );
        return -1;
    } else if( SfxCacheInitHandlers() ){
        SfxCacheUnk14();
        Free( gSfxCacheLog );
        return -1;
    }        
    if( !(gSfxCache = (Cache_t *)Malloc( sizeof( Cache_t ) )) ){ 
        SfxCacheDestroy();
        SfxCacheUnk14();
        Free( gSfxCacheLog );
        return -1;
    }
    if( CacheInit( gSfxCache, SfxCacheLen, (void *)SfxCacheLoad, SfxCacheFree, CacheSize ) != 1 ){
        Free( gSfxCache );
        SfxCacheDestroy();
        SfxCacheUnk14();
        Free( gSfxCacheLog );
        return -1;
    }
    gSfxCacheEnable = 1;
    return 0;
}

void SfxCacheClose()
{
    if( !gSfxCacheEnable ) return;    
    CacheClose( gSfxCache );
    Free( gSfxCache );
    gSfxCache = NULL;
    SfxCacheDestroy();
    SfxCacheUnk14();
    Free( gSfxCacheLog );
    gSfxCacheEnable = 0;    
}

int SfxCached()
{
    return gSfxCacheEnable;
}

int SfxCacheReAlloc()
{
    if( !gSfxCacheEnable ) return 0;
    return CacheExpand( gSfxCache );
}

int SfxCacheFileOpen( const char *fname, int a2 )
{
    char *stmp;
    int data;
    void *FileData;
    int err, FileId;
    CachePool_t *Obj;

    Obj = NULL;
    FileData = NULL;
    FileId = 0;
    if( gSfxCacheDirty >= 4 ) return -1;
    if( !(stmp = strdup( fname )) ) return -1;
    err = SfxCacheGetFileId( stmp, &FileId );
    Free( stmp );
    if( err ) return -1;
    if( CacheLoad( gSfxCache, FileId, &FileData, &Obj ) != 1 ) return -1;
    if( SfxCacheGetBlock( &data, FileId, FileData, Obj ) ){
	CacheUnlockBlock( gSfxCache, Obj );
	return -1;
    }    
    return data;
}

int SfxCacheFileClose( unsigned int Id )
{
    if( !SfxCacheGetSize( Id ) ) return -1;
    if( CacheUnlockBlock( gSfxCache, gSfxCacheHandlers[ Id ].Blk ) != 1 ) return -1;
    if ( Id <= SFX_HANDLERS ) gSfxCacheHandlers[ Id ].a = 0;
    return 0;
}

unsigned int SfxCacheFileRead( unsigned int Id, short *WaveData, int size )
{
    SfxCache_t *cache;

    if( !SfxCacheGetSize( Id ) ) return -1;
    if( !size ) return 0;
    cache = &gSfxCacheHandlers[ Id ];
    if( (cache->Len - cache->FilePtr) <= 0 ) return 0;
    size = ( size <= ( cache->Len - cache->FilePtr )) ? size : ( cache->Len - cache->FilePtr );
    if( gSfxCacheUnk03 ) {
	gSfxCacheHandlers[ Id ].Freq = 0;
	gSfxCacheHandlers[ Id ].Channels = 0;
	gSfxCacheHandlers[ Id ].Samples = 0;
        if( gSfxCacheUnk03 != 1 || SfxCacheLoadAcm( Id, WaveData, size, &gSfxCacheHandlers[ Id ].Channels,  &gSfxCacheHandlers[ Id ].Freq, &gSfxCacheHandlers[ Id ].Samples ) ) return -1;
    } else {
        memcpy( WaveData, gSfxCacheHandlers[ Id ].FilePtr + gSfxCacheHandlers[ Id ].Data, size );
    }
    gSfxCacheHandlers[ Id ].FilePtr += size;
    return size;
}

int SfxCacheFileWrite()
{
    return -1;
}

int SfxCacheFileSeek( int fh, int offset, unsigned int whence )
{
    int start, d, pos;

    if( !SfxCacheGetSize( fh ) ) return -1;
    start = 0;
    switch( whence ){
	case SEEK_SET: break;
	case SEEK_CUR: start = gSfxCacheHandlers[ fh ].FilePtr; break;
	case SEEK_END: start = gSfxCacheHandlers[ fh ].Len; break;
    }
    d = abs( offset );
    if( offset >= 0 ){
        if( d > gSfxCacheHandlers[ fh ].Len - gSfxCacheHandlers[ fh ].FilePtr ) d = gSfxCacheHandlers[ fh ].Len - gSfxCacheHandlers[ fh ].FilePtr;
        pos = start + d;
    } else {
        if( d > start ) return -1;
        pos = start - d;
    }
    gSfxCacheHandlers[ fh ].FilePtr = pos;
    return pos;
}

int SfxCacheFileTell( int fh )
{
    if( !SfxCacheGetSize( fh ) ) return -1;
    return gSfxCacheHandlers[ fh ].FilePtr;    
}

int SfxCacheFileLen( int fh )
{
    if( !SfxCacheGetSize( fh ) ) return 0;
    return gSfxCacheHandlers[ fh ].Len;
}

int SfxCacheLen( int fh, int *pLen )
{
    int len;

    if( SfxCacheSfxGetArgB(fh, &len) ) return -1;
    *pLen = len;
    return 0;
}

int SfxCacheLoad( int fh, int *argb, char *header )
{
    int p;
    char *fname;

    fname = NULL;
    p = 0;
    if( !SfxCacheUnk12( fh ) ) return -1;
    SfxCacheSfxGetArgB( fh, &p );
    SfxCacheSfxGetFname( fh, &fname );
    if( dbReadFile( fname, header ) ){
        Free( fname );
        return -1;
    }
    Free( fname );
    *argb = p;
    return 0;
}

void SfxCacheFree( void *ptr )
{
    Free( ptr );
}

int SfxCacheInitHandlers()
{
    SfxCache_t *p;
    int err, i;

    p = (SfxCache_t *)Malloc( SFX_HANDLERS * sizeof( SfxCache_t ) );
    if( p ){
        for( i = 0; i != SFX_HANDLERS; i++ ) p[ i ].a = 0;        
        err = 0;
        gSfxCacheDirty = 0;
    } else {
        err = -1;
    }
    gSfxCacheHandlers = p;
    return err;
}

void SfxCacheDestroy()
{
    int i;

    if( gSfxCacheDirty ){
        for ( i = 0; i < SFX_HANDLERS; i++ ){
            if( gSfxCacheHandlers[ i ].a ) SfxCacheFileClose( i );
        }
    }
    Free( gSfxCacheHandlers );
}

int SfxCacheGetBlock( int *fh, int Size, char *data, CachePool_t *blk )
{
    SfxCache_t *h;
    int err, i;

    h = gSfxCacheHandlers;
    if( gSfxCacheDirty < SFX_HANDLERS ){
        for( i = 0; i < SFX_HANDLERS; i++ ){ // find free handler
            if( gSfxCacheHandlers[ i ].a == 0 ) break;
        }
        if( i == SFX_HANDLERS ) return -1;
        h = &gSfxCacheHandlers[ i ];
        h->a = 1;
        gSfxCacheHandlers = h;
        h->Blk = blk;
        h->Size = Size;
        SfxCacheSfxGetArgA( Size, &h->Len );
        SfxCacheSfxGetArgB( Size, &h->e );
        h->FilePtr = 0;
        h->g = 0;
        h = gSfxCacheHandlers;
        h->Data = data;
        err = 0;
        *fh = i;
    } else {
        err = -1;
    }
    gSfxCacheHandlers = h;
    return err;
}

int SfxCacheGetSize( unsigned int fh )
{
    SfxCache_t *h;

    if( fh >= SFX_HANDLERS ) return 0;
    h = &gSfxCacheHandlers[ fh ];
    if( !h->a ) return 0;
    if( h->Len >= h->FilePtr ) return SfxCacheUnk12( h->Size );
    return 0;
}

int SfxCacheGetFmt( int fh, int *pChannels, int *pFreq, int *pSamples )
{
    SfxCache_t *h;

    if( (fh >= SFX_HANDLERS) || (fh < 0) ) return 0;
    h = &gSfxCacheHandlers[ fh ];
    if( !h->a ) return 0;
    *pChannels = h->Channels;
    *pFreq = h->Freq;
    *pSamples = h->Samples;
    return 1;
}

int SfxCacheLoadAcm( unsigned int fh, short *WaveData, unsigned int nBytes, int *pChannels, int *pFreq, int *pSamples )
{
    SfxCache_t *h;
    Acm_t *acm;
    void *buf;
    int pos, k;

    if( !SfxCacheGetSize( fh ) )return -1;
    h = &gSfxCacheHandlers[ fh ];
    h->g = 0;
    acm = AcmOpen( (void *)SfxCacheGetData, &fh, pChannels, pFreq, pSamples );
    if( (pos = h->FilePtr) ){
	if( !(buf = Malloc( pos )) ){
	    AcmClose( acm );
	    return -1;
	}
	k = AcmInflate( acm, buf, h->FilePtr );
	Free( buf );
	if( k != h->FilePtr ){
	    AcmClose( acm );
	    return -1;
	}
    }
    k = AcmInflate( acm, WaveData, nBytes );
    AcmClose( acm );
    if( k != nBytes ) return -1;
    return 0;                
}

unsigned int SfxCacheGetData( int *fh, char *data, unsigned int Size )
{
    unsigned int n;
    SfxCache_t *h;
    char *k;

    if( !Size ) return 0;
    h = &gSfxCacheHandlers[ *fh ];
    n = h->e - h->g;
    k = &h->Data[ h->g ];
    if( Size <= n ) n = Size;
    memcpy( data, k, n );
    h->g += n;
    return n;
}

int SfxCacheUnk12( int fh )
{
    return SfxCacheGetIdx( fh, 0 ) == 0;
}

int SfxCacheLoadSfxList( const char *path, int FileFrm, int a3 )
{
    int err;
    xFile_t *fh;
    int i;
    char p[256];

    memset( p, 0x00, 255 );
    gSfxCacheUnk07 = a3;
    gSoundSfxAcmFile = FileFrm;
    gSfxCacheSfxCount = 0;
    if( !( gSfxCacheListPath = strdup( path ) ) ) return 1;    
    gSfxCacheListPathLen = strlen( gSfxCacheListPath );
    if( !strlen( path ) || path[ strlen( path ) - 1 ] == '/' )
        sprintf( p, "%sSNDLIST.LST", path );
    else
        sprintf( p, "%s/SNDLIST.LST", path );
    
    if( ( fh = dbOpen( p, "rt" ) ) ){
        dbgets( p, 255, fh );
        gSfxCacheSfxCount = strtol( p, NULL, 10 );
        gSfxCacheList = Malloc( sizeof( SfxCacheList_t ) * gSfxCacheSfxCount );
        for( i = 0; i < gSfxCacheSfxCount; i++ ){
            dbgets( p, 255, fh ); p[ strlen( p ) - 1 ] = '\0'; gSfxCacheList[ i ].fname = strdup( p );
            dbgets( p, 255, fh ); gSfxCacheList[ i ].a = strtol( p, NULL, 10 );
            dbgets( p, 255, fh ); gSfxCacheList[ i ].b = strtol( p, NULL, 10 );
            dbgets( p, 255, fh ); gSfxCacheList[ i ].c = strtol( p, NULL, 10 );
        }
        dbClose( fh );
        eprintf( "Reading SNDLIST.LST Sound FX Count: %d", gSfxCacheSfxCount );
    } else {
        if( (err = SfxCacheCreateFileList()) ){
            Free( gSfxCacheListPath );
            return err;
        }
        if( (err = SfxCacheLoadEffectList()) ){
            SfxCacheListFree();
            Free( gSfxCacheListPath );
            return err;
        }
        if( gSfxCacheSfxCount != 1 ) qsort( gSfxCacheList, gSfxCacheSfxCount, 16, (void *)SfxCacheStrCmp );
        if( ( fh = dbOpen( p, "wt" ) ) ){
            dbPrintf( fh, "%d\n", gSfxCacheSfxCount );
            for( i = 0; i < gSfxCacheSfxCount; i++ ){
                    dbPrintf( fh, "%s\n", gSfxCacheList[ i ].fname );
                    dbPrintf( fh, "%d\n", gSfxCacheList[ i ].a );
                    dbPrintf( fh, "%d\n", gSfxCacheList[ i ].b );
                    dbPrintf( fh, "%d\n", gSfxCacheList[ i ].c );
            }
            dbClose( fh );
        } else {
            eprintf( "SFXLIST: Can't open file for write %s", p );
        }
    }
    gSfxCacheUnk10 = 1;
    return 0;
}

void SfxCacheUnk14()
{
    if ( !gSfxCacheUnk10 ) return;
    SfxCacheListFree();
    Free( gSfxCacheListPath );
    gSfxCacheUnk10 = 0;
}

int SfxCacheGetFileId( char *ListPath, int *pFileId )
{
    SfxCacheList_t *p;
    int idx;

    if( strncasecmp( gSfxCacheListPath, ListPath, gSfxCacheListPathLen ) ) return 1;
    
    p = bsearch( ListPath + gSfxCacheListPathLen, gSfxCacheList, gSfxCacheSfxCount, sizeof( SfxCacheList_t ), (void *)SfxCacheStrCmp );
    if( !p ) return 1;
    idx = ((unsigned long long)p - (unsigned long long)gSfxCacheList) / sizeof( SfxCacheList_t );
    if( idx >= gSfxCacheSfxCount ) return 1;
    if( idx < 0 ) return 1;
    *pFileId = 2 * (idx + 1);
    return 0;
}

int SfxCacheSfxGetFname( int a1, char **pfname )
{
    int Idx;
    char *p;

    if( SfxCacheGetIdx( a1, &Idx ) ) return 0;
    p = Malloc( strlen( gSfxCacheListPath ) + 1 + strlen( gSfxCacheList[ Idx ].fname ) );
    if( !p ) return 1;
    strcpy( p, gSfxCacheListPath );
    strcpy( &p[ strlen( p ) ], gSfxCacheList[ Idx ].fname );
    *pfname = p;
    return 0;
}

int SfxCacheSfxGetArgA( int fh, int *pArg )
{
    int err, Idx; 

    if( (err = SfxCacheGetIdx( fh, &Idx )) ) return err;    
    *pArg = gSfxCacheList[ Idx ].a;
    return 0;    
}

int SfxCacheSfxGetArgB( int fh, int *pArg )
{
    int err, Idx;

    if( (err = SfxCacheGetIdx( fh, &Idx ) ) ) return err;
    *pArg = gSfxCacheList[ Idx ].b;
    return 0;
}

int SfxCacheGetIdx( int fh, int *pIdx )
{
    int n;

    if( fh <= 0  ) return 2;
    if( (fh & 1) ) return 2;
    n = (fh / 2) - 1;
    if( n >= gSfxCacheSfxCount ) return 2;
    if( pIdx ) *pIdx = n;
    return 0;
}

int SfxCacheGetIdxOfs( int fh, int *pId )
{
    if( fh >= gSfxCacheSfxCount ) return 1;
    if( fh < 0 ) return 1;
    *pId = 2 * fh + 2;
    return 0;
}

void SfxCacheListFree()
{
    int i;
    char *fname;

    if( (gSfxCacheSfxCount < 0) || !gSfxCacheList ) return;    
    for( i = 0; i < gSfxCacheSfxCount; i++ ){
        fname = gSfxCacheList[ i ].fname;
        if( fname ) Free( fname );
    }
    Free( gSfxCacheList );
    gSfxCacheList = NULL;
    gSfxCacheSfxCount = 0;
}

int SfxCacheCreateFileList()
{
    char *p, *s, **list;
    int n, err;

    if( gSoundSfxAcmFile ){
        if( gSoundSfxAcmFile != 1 ) return 1;
        s = "*.ACM";
    } else {
        s = "*.SND";
    }
    p = Malloc( strlen( gSfxCacheListPath ) + 1 + strlen( s ) );
    if( !p ) return 1;
    strcpy( p, gSfxCacheListPath );
    strcpy( p + strlen( p ), s );    
    gSfxCacheSfxCount = dbGetFileList( p, &list );
    Free( p );
    
    if( gSfxCacheSfxCount > 10000 ){
        dbDelFileList( list );
        return 1;
    }
    if( gSfxCacheSfxCount <= 0 ) return 1;
    n = sizeof( SfxCacheList_t ) * gSfxCacheSfxCount;
    gSfxCacheList = Malloc( n );
    if( !gSfxCacheList ){
        dbDelFileList( list );
        return 1;
    }
    memset( gSfxCacheList, 0, n );
    err = SfxCacheListCopyFnames( list );
    dbDelFileList( list );
    if( err ){
	SfxCacheListFree();
	return err;
    }
    return 0;
}

int SfxCacheListCopyFnames( char **name )
{
    int i;
    char *s;

    if( gSfxCacheSfxCount <= 0 ) return 0;
    for( i = 0; i < gSfxCacheSfxCount; i++ ){
        if( !(s = strdup( name[ i ] ) ) ) break;
        gSfxCacheList[ i ].fname = s;
    }
    SfxCacheListFree();
    return 1;
}

int SfxCacheLoadEffectList()
{
    int i, flen, err;
    char *p, *n;
    xFile_t *fh;
    Acm_t *acm;
    int Channels, Freq, Bytes;

    n = Malloc( gSfxCacheListPathLen + 13 );
    if( !n ) return 1;
    strcpy( n, gSfxCacheListPath );
    p = n + gSfxCacheListPathLen;
    err = 0;
    for( i = 0; i < gSfxCacheSfxCount; i++ ){
        strcpy( p, gSfxCacheList[ i ].fname );
        if( dbCheckFileLength( n, &flen ) ){ err = 1; break;}
        if( flen <= 0 ){ err = 1; break;}
        gSfxCacheList[ i ].b = flen;
        if( gSoundSfxAcmFile ){
            if( gSoundSfxAcmFile != 1 ){ err = 1; break;}
            if( !(fh = dbOpen( n, "rb" )) ){ err = 1; break;}
            acm = AcmOpen( (void *)SfxCacheReadFile, fh, &Channels, &Freq, &Bytes );
            gSfxCacheList[ i ].a = 2 * Bytes;
            AcmClose( acm );
            if( dbClose( fh ) ){ err = 1; break;}
        } else {
            gSfxCacheList[ i ].a = flen;
        }
    }
    Free( n );
    return err;
}

int SfxCacheSortList()
{
    if( gSfxCacheSfxCount != 1 ) qsort(gSfxCacheList, gSfxCacheSfxCount, sizeof( SfxCacheList_t ), (void *)SfxCacheStrCmp );
    return 0;
}

static int SfxCacheStrCmp( char *key, SfxCacheList_t *list )
{
    return strcasecmp( key, list->fname );
}

int SfxCacheReadFile( xFile_t *fh, char *data, int count )
{
    return dbread( data, 1u, count, fh );
}


