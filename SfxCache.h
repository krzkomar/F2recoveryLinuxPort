#pragma once

#define SFX_HANDLERS	4

typedef struct // size of 32
{
    char a; // used flag
    CachePool_t *Blk;
    int Size;
    int Len;
    int e;
    int FilePtr;
    int g;
    char *Data;
    int Channels;
    int Freq;
    int Samples;
} SfxCache_t;

typedef struct // size of 16
{
    char *fname;
    int a;
    int b;
    int c;
} SfxCacheList_t;

/*
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
char *gSfxCacheListPathLen = NULL;
SfxCacheList_t *gSfxCacheList = NULL;
int gSfxCacheSfxCount = 0;

int SfxCacheLen( int fh, int *pLen );
int SfxCacheLoad( int fh, char **pData, char *header );
void SfxCacheFree( void *ptr );
*/

/**********************************************************************/

int SfxCacheInit( unsigned int CacheSize, const char *Name );
void SfxCacheClose();
int SfxCached();
int SfxCacheReAlloc();
int SfxCacheFileOpen( const char *fname, int a2 );
int SfxCacheFileClose( unsigned int Id );
unsigned int SfxCacheFileRead( unsigned int Id, short *data, int size );
int SfxCacheFileWrite();
int SfxCacheFileSeek( int fh, int offset, unsigned int whence );
int SfxCacheFileTell( int fh );
int SfxCacheFileLen( int fh );
int SfxCacheLen( int fh, int *pLen );
int SfxCacheLoad( int fh, int *argb, char *header );
void SfxCacheFree( void *ptr );
int SfxCacheInitHandlers();
void SfxCacheDestroy();
int SfxCacheGetBlock( int *fh, int Size, char *data, CachePool_t *blk );
int SfxCacheGetSize( unsigned int fh );
int SfxCacheLoadAcm( unsigned int fh, short *WaveData, unsigned int nBytes, int *channels, int *freq, int *samples );
unsigned int SfxCacheGetData( int *fh, char *data, unsigned int Size );
int SfxCacheUnk12( int fh );
int SfxCacheLoadSfxList( const char *path, int a2, int a3 );
void SfxCacheUnk14();
int SfxCacheGetFileId( char *ListPath, int *pFileId );
int SfxCacheSfxGetFname( int a1, char **pfname );
int SfxCacheSfxGetArgA( int fh, int *pArg );
int SfxCacheSfxGetArgB( int fh, int *pArg );
int SfxCacheGetIdx( int fh, int *pIdx );
int SfxCacheGetIdxOfs( int fh, int *pId );
void SfxCacheListFree();
int SfxCacheCreateFileList();
int SfxCacheListCopyFnames( char **name );
int SfxCacheLoadEffectList();
int SfxCacheSortList();
//int SfxCacheStrCmp( char **a1, char **a2 );
int SfxCacheReadFile( xFile_t *fh, char *data, int count );
int SfxCacheGetFmt( int fh, int *pChannels, int *pFreq, int *pSamples );


