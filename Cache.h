#pragma once

typedef struct // size of 32
{
    int		FileId;
    int		Size;
    void	*Data;
    int		Locked;
    int		Usage;
    short	Flags;
    int		Utility;
    int		BlkId;
} CachePool_t;

typedef struct 
{
    int		UsedBytes;
    int		CacheSize;
    int		Count;
    int		Chunks;
    int		Used;
    CachePool_t **Pool;
    int		(*GetSize)(int FileId, int *pLen);
    int		(*Load)( int FileId, int *Size, void *Data );
    void	(*Free)(void *);
    Heap_t heap;
} Cache_t;

int CacheInit( Cache_t *cache, int (*LenCb)(int Id, int *pLen), int (*LoadCb)(int Id, int *Size, void *Data), void (*FreeCb)(void *), int CacheSize );
void CacheClose( Cache_t *cache );
int CacheIsFileCached( Cache_t *Cache, int FileId );
int CacheLoad( Cache_t *cache, int FileId, void **pData, CachePool_t **pBlock );
int CacheGetDataById( Cache_t *cache, int BlkIdA, int BlkIdB, void **data, CachePool_t **pBlock );
int CacheUnlockBlock( Cache_t *cache, CachePool_t *blk );
int CacheRemove( Cache_t *cache, int FileId );
int CacheExpand( Cache_t *cache );
int CacheGetBytesUsed( Cache_t *cache, int *BytesUsed );
int CacheStats( Cache_t *db, char *str);
int CacheGetPool( Cache_t *cache, unsigned int mode, int **ObjList, int *ObjCount );
int CacheFreeP( void **p);
int CacheCreateBlock( Cache_t *Cache, int FileId, int *Idx );
int CacheBlockInsert( Cache_t *Cache, CachePool_t *Block, int Idx );
int CacheBinSearchBlock( Cache_t *cache, int FileId, int *Idx );
int CacheBlkNew( CachePool_t **pBlk );
int CacheBlkClear( CachePool_t *blk );
int CacheBlkRem( Cache_t *cache, CachePool_t *blk );
int CacheUnlockAll( Cache_t *cache );
int CacheSortByUtility( Cache_t *cache );
int CacheUnk07( Cache_t *cache, unsigned int Size );
void CacheFlush( Cache_t *cache );
int CachePoolAlloc( Cache_t *cache, int PoolSize );


