#include "FrameWork.h"

static int CacheSortUsageCb( CachePool_t **pp1, CachePool_t **pp2);
static int CacheSortUtilityCb( CachePool_t **p1, CachePool_t **p2 );

int CacheInit( Cache_t *cache, int (*LenCb)(int Id, int *pLen), int (*LoadCb)(int Id, int *Size, void *data), void (*FreeCb)(void *), int CacheSize )
{
    if( !cache ) return 0;
    if( !LenCb ) return 0;
    if( !LoadCb ) return 0;
    if( !FreeCb ) return 0;
    if( !CacheSize ) return 0;
    if( HeapInit( &cache->heap, CacheSize) != 1 ) return 0;
    cache->UsedBytes = 0;
    cache->Count = 0;
    cache->Chunks = 100;
    cache->Used = 0;
    cache->CacheSize = CacheSize;
    cache->GetSize = LenCb;
    cache->Free = FreeCb;
    cache->Load = LoadCb;
    if( !(cache->Pool = (CachePool_t **)Malloc( cache->Chunks * sizeof( CachePool_t * ) )) ) return 0;
    memset( cache->Pool, 0x00, cache->Chunks * sizeof( CachePool_t * ) );
    return 1;
}

void CacheClose( Cache_t *cache )
{
    if( !cache ) return;    
    CacheUnlockAll( cache );
    CacheExpand( cache );
    HeapClose( &cache->heap );
    cache->UsedBytes = 0;
    cache->CacheSize = 0;
    cache->Count = 0;
    cache->Chunks = 0;
    cache->Used = 0;
    if( cache->Pool ) Free( cache->Pool );
    cache->GetSize = NULL;
    cache->Free = NULL;
    cache->Load = NULL;
    cache->Pool = NULL;    
}

int CacheIsFileCached( Cache_t *Cache, int FileId ) // not used
{
    int Idx;
    return Cache && ( CacheBinSearchBlock( Cache, FileId, &Idx ) == 2 );
}

int CacheLoad( Cache_t *cache, int FileId, void **pData, CachePool_t **pBlock )
{
//    static int gCacheUnk01 = 0;
    int r, idx;
    CachePool_t *p;

    if( !cache || !pData || !pBlock ) return 0;
    *pBlock = NULL;

    r = CacheBinSearchBlock( cache, FileId, &idx );
    if( r < 2 ) return 0;
    if( r > 2 ){ // not cached, create new
        if( (r != 3) || ( cache->Count == 0x7FFFFFFF ) ) return 0;
//printf("new blk %i %p\n", idx, cache);

        if( CacheCreateBlock( cache, FileId, &idx ) != 1 ) return 0;
//DD
//printf("-OO->%i\n", idx );
//        gCacheUnk01 %= 4;
//        if( !gCacheUnk01 ) SoundUpdateAll();
    } else { // cached
//printf("cached blk %i %p\n", idx, cache->Pool[ idx ]);
	cache->Pool[ idx ]->Usage++;
    }
    p = cache->Pool[ idx ];
    if( p->Locked == 0 ){ // if not locked
	if( !HeapLockBlock( &cache->heap, p->BlkId, &p->Data ) ) return 0;
    }
    cache->Pool[ idx ]->Locked++;
    cache->Used++;
    cache->Pool[ idx ]->Utility = cache->Used;
    if( cache->Used == -1 ) CacheSortByUtility( cache );
    *pData  = cache->Pool[ idx ]->Data;
    *pBlock = cache->Pool[ idx ];
    return 1;
}

int CacheGetDataById( Cache_t *cache, int BlkIdA, int BlkIdB, void **data, CachePool_t **pBlock ) // not used
{
    int r, Idx;
    CachePool_t *blk;

    if( !cache || !data || !pBlock ) return 0;
    r = CacheBinSearchBlock( cache, BlkIdA, &Idx );
    if( r < 2 ) return 0;
    if( r > 2 ){
        if( r != 3 ) return 0;        
        r = CacheBinSearchBlock( cache, BlkIdB, &Idx );
        if( r < 2 ) return 0;        
        if( r <= 2 ){
            blk = cache->Pool[ Idx ];
            blk->Usage++;
            if( blk->Locked == 0 ){
        	if( !HeapLockBlock( &cache->heap, blk->BlkId, &blk->Data ) ) return 0;
            }
            blk->Locked++;
            cache->Used++;
            blk->Utility = cache->Used;
            if( cache->Used == -1 ) CacheSortByUtility( cache );
            *data = blk->Data;
            *pBlock = blk;
            return 1;                                        
        }
        if( r == 3 ){
            if( CacheLoad( cache, BlkIdA, data, pBlock ) == 1 ) return 1;
            return CacheLoad( cache, BlkIdB, data, pBlock );
        }
        return 0;
    }
    blk = cache->Pool[ Idx ];
    blk->Usage++;
    if( !blk->Locked ){
	if( !HeapLockBlock(&cache->heap, blk->BlkId, &blk->Data) ) return 0;
    }
    blk->Locked++;
    cache->Used++;
    blk->Utility = cache->Used;
    if( cache->Used == -1 ) CacheSortByUtility( cache );
    *data = blk->Data;
    *pBlock = blk;
    return 1;
}

int CacheUnlockBlock( Cache_t *cache, CachePool_t *blk )
{
    int n;

    if( !cache || !blk ) return 0;
    n = blk->Locked;
    if( !n ) return 0;
    blk->Locked = n -1 ;
    if( n == 1 ) HeapUnlockBlock( &cache->heap, blk->BlkId );
    return 1;
}

int CacheRemove( Cache_t *cache, int FileId )
{
    CachePool_t *blk;
    int idx;

    if( !cache ) return 0;
    if( CacheBinSearchBlock( cache, FileId, &idx ) != 2 ) return 0;
    blk = cache->Pool[ idx ];
    if( blk->Locked ) return 0;
    blk->Flags |= 0x01;
    CacheFlush( cache );
    return 1;
}

int CacheExpand( Cache_t *cache )
{
    int i;

    if( !cache ) return 0;
    for( i = 0; i < cache->Count; i++ ){
        if( !cache->Pool[ i ]->Locked ) cache->Pool[ i ]->Flags |= 0x01;
    }
    CacheFlush( cache );
    i = cache->Count + 50;
    if( i < cache->Chunks ) CachePoolAlloc( cache, i );
    return 1;    
}

int CacheGetBytesUsed( Cache_t *cache, int *BytesUsed )
{
    if( !cache || !BytesUsed ) return 0;
    *BytesUsed = cache->UsedBytes;
    return 1;
}

int CacheStats( Cache_t *db, char *str)
{
    if( !db || !str ) return 0;
    sprintf( str, "Cache stats are disabled.%s", "\n" );
    return 1;
}

int CacheGetPool( Cache_t *cache, unsigned int mode, int **ObjList, int *ObjCount )
{
    int i, n;
    CachePool_t *blk;

    if( !cache || !ObjList || !ObjCount ) return 0;
    *ObjCount = 0;
    switch( mode ){
	case 0:
    	    if( !( *ObjList = (int *)Malloc( cache->Count * sizeof( CachePool_t *) ) ) ) return 0;
    	    if( cache->Count > 0 ){
        	for( i = 0; i < cache->Count; i++ ){
DD
//            	    (*ObjList)[ i ] = (int *)cache->Pool[ i ];
        	}
    	    }
    	    *ObjCount = cache->Count;
    	    return 1;
	case 1:
    	    for( i = 0; i < cache->Count; i++ ){
        	if( cache->Pool[ i ]->Locked != 0 ) ( *ObjCount )++;
    	    }
    	    if( !(*ObjList = (int *)Malloc( *ObjCount * sizeof( int )) ) ) return 0;
    	    for( i = n = 0; i < cache->Count; i++ ){
        	blk = cache->Pool[ i ];
        	if( blk->Locked != 0 || n >= *ObjCount ) continue;
        	(*ObjList)[ n++ ] = blk->FileId;
    	    }        
    	    return 1;
	case 2:
	    for( i = 0; i < cache->Count; i++ ){
    		if( cache->Pool[ i ]->Locked == 0 ) ( *ObjCount )++;
	    }
	    if( !(*ObjList = (int *)Malloc( *ObjCount * sizeof( int ) ) ) ) return 0;        
	    for( i = n = 0; i < cache->Count; i++ ){
    		blk = cache->Pool[ i ];
    		if( blk->Locked == 0 || n >= *ObjCount ) continue;
    		(*ObjList)[ n++ ] = blk->FileId;
	    }
	    return 1;
    }
    return 1;
}

int CacheFreeP( void **p)
{
    if( !p || !*p ) return 0;
    Free( *p );
    *p = NULL;
    return 1;
}

int CacheCreateBlock( Cache_t *Cache, int FileId, int *Idx )
{
    CachePool_t *p;
    double sz;
    int i, AllocSize = 0, flg;
#define RETRIES 10 // 10

    if( !(p = Malloc( sizeof( CachePool_t ) ) ) ) return 0;
    if( CacheBlkClear( p ) != 1 ) return 0;
    if( Cache->GetSize( FileId, &AllocSize ) ) goto Err1;
    if( !CacheUnk07( Cache, AllocSize ) ) goto Err1;
    flg = 0;
    for( i = 0; i < RETRIES; i++ ){
        if( HeapAllocate( &Cache->heap, &p->BlkId, AllocSize, 1 ) == 1 ){
	    flg = 1;
            break;
        }
        sz = lround( AllocSize * 1.25 );
        if( sz >= Cache->CacheSize ) break;
        if( !CacheUnk07( Cache, sz ) ) break;
    }
    if( !flg ){
        CacheExpand( Cache );
        flg = 1;
        if( !HeapAllocate( &Cache->heap, &p->BlkId, AllocSize, 1) && !HeapAllocate( &Cache->heap, &p->BlkId, AllocSize, 0 ) ) flg = 0;
    }
    if( !flg ) goto Err1;
    if( HeapLockBlock( &Cache->heap, p->BlkId, &p->Data ) != 1 ) goto Err2;
//DD
//printf("0CACHE SIZE>>%i\n", AllocSize);
    if( Cache->Load( FileId, &AllocSize, p->Data ) ){
	eprintf("Error: Cache->Load()\n");
	goto Err2; // set size
    }
//printf("1CACHE SIZE>>%i\n", AllocSize);
    HeapUnlockBlock( &Cache->heap, p->BlkId );
    p->Size = AllocSize;
    p->FileId = FileId;
    flg = 1;
    if( *Idx < Cache->Count ){
        if( FileId < Cache->Pool[ *Idx ]->FileId && ( (*Idx == 0) || FileId > Cache->Pool[ *Idx - 1 ]->FileId) ) flg = 0;
    }
    if( flg && CacheBinSearchBlock( Cache, FileId, Idx ) != 3 ) goto Err2;
    if( CacheBlockInsert( Cache, p, *Idx ) != 1 ) goto Err2;
    return 1;
// errors    
Err1:
    if( p->Data ) HeapDeallocate( &Cache->heap, &p->BlkId );
    Free( p );
    return 0;
Err2:
    HeapUnlockBlock( &Cache->heap, p->BlkId );
    goto Err1;
}

int CacheBlockInsert( Cache_t *Cache, CachePool_t *Block, int Idx )
{
    if( (Cache->Chunks - 1) == Cache->Count ){
	if( CachePoolAlloc( Cache, Cache->Chunks + 50 ) != 1 ) return 0;
    }
    memmove( &Cache->Pool[ Idx + 1 ], &Cache->Pool[ Idx ], (Cache->Count - Idx) * sizeof( char *));
    Cache->Pool[ Idx ] = Block;
    Cache->Count++;
    Cache->UsedBytes += Block->Size;
    return 1;
}

int CacheBinSearchBlock( Cache_t *cache, int FileId, int *Idx )
{
    int r = 0, d, Count, b, a;

    d = 0;
    *Idx = 0;
    Count = cache->Count;
    if( Count != 0 ){
	b = Count - 1;
	a = 0;
	while( a <= b ){
	    d = (a + b) / 2;
	    r = cache->Pool[ d ]->FileId;
	    if( r == FileId ){
		*Idx = d;
		return 2;
	    }
	    if( FileId < r )
    		b = d - 1;
	    else
    		a = d + 1;
	}
	if( FileId > r ) d++;
    }
    *Idx = d;
    return 3;
}

int CacheBlkNew( CachePool_t **pBlk )
{
    CachePool_t *p;

    p = (CachePool_t *)Malloc( sizeof( CachePool_t ) );
    *pBlk = p;
    p->FileId = 0;
    p->Size = 0;
    p->Data = 0;
    p->Locked = 0;
    p->Usage = 0;
    p->Flags = 0;
    p->Utility = 0;
    return 1;
}

int CacheBlkClear( CachePool_t *blk )
{
    blk->Size = 0;
    blk->Data = 0;
    blk->Locked = 0;
    blk->Usage = 0;
    blk->Flags = 0;
    blk->Utility = 0;
    blk->FileId = 0;
    return 1;
}

int CacheBlkRem( Cache_t *cache, CachePool_t *blk )
{
    if( blk->Data ) HeapDeallocate( &cache->heap, &blk->BlkId );
    Free( blk );
    return 1;
}

int CacheUnlockAll( Cache_t *cache )
{
    int i, n;

    for( i = 0; i < cache->Count; i++ ){
        n = ( cache->Pool[ i ]->Locked != 0 );
    	cache->Pool[ i ]->Locked = 0;
    	if( n ) HeapUnlockBlock( &cache->heap, cache->Pool[ i ]->BlkId );
    }
    return 1;
}

int CacheSortByUtility( Cache_t *cache ) // co sie dzieje z posortowana tablica pool ? memleak ?????
{
    CachePool_t **pool;
    int i;
DD
return 1; // ignore this function
    if( !cache ) return 0;    
    if( !( pool = (CachePool_t **)Malloc( cache->Count * sizeof( CachePool_t ** ) ) ) ) return 0;
    memcpy( pool, cache->Pool, cache->Count * sizeof( CachePool_t ** ) );
    // sort
    qsort( pool, cache->Count, sizeof( CachePool_t ** ), (void *)CacheSortUtilityCb );
    // renumerate utility
    for( i = 0; i < cache->Count; i++ ){
        pool[ i ]->Utility = i;
    }
    cache->Used = i;
    return 1;
}

int CacheUnk07( Cache_t *cache, unsigned int Size )
{
    CachePool_t *t, **d, **p;
    unsigned int CacheSize, extsize, PoolSize;
    int i, Count;

    CacheSize = cache->CacheSize;
    if( Size > CacheSize ) return 0;
    if( CacheSize - cache->UsedBytes >= Size ) return cache->CacheSize - cache->UsedBytes >= Size;
    p = (CachePool_t **)Malloc( cache->Count * sizeof( CachePool_t ** ) );
    if( p ){
	extsize = Size + lround( cache->Count * 0.2 );
	memcpy( p, cache->Pool, cache->Count * sizeof( CachePool_t ** ) );
	qsort( p, cache->Count, sizeof( CachePool_t ** ), (void *)CacheSortUsageCb );
	for( PoolSize = i = 0; i < cache->Count; i++ ){
	    t = p[ i ];
	    if( !t->Locked ){
		if( extsize <= t->Size ){
		    t->Flags |= 0x01;
		    PoolSize = 0;
		    break;
		}
		PoolSize += t->Size;
		if( PoolSize >= extsize ) break;
	    }	
	}
	if( PoolSize ){
	    Count = cache->Count;
    	    if( i == Count ) i--;
	    d = p + i;
    	    for( ;i >= 0; i--, d-- ){
        	if( !d[ i ]->Locked ) d[ i ]->Flags |= 0x01;
	    }
	}
	Free( p );
    }
    CacheFlush( cache );
    return cache->CacheSize - cache->UsedBytes >= Size;
}

void CacheFlush( Cache_t *cache )
{
    int i;
    CachePool_t *p;
    
    for( i = 0; i < cache->Count; i++ ){
        p = cache->Pool[ i ];
        if( p->Flags & 0x01 ){
            if( p->Locked ){
                p->Flags &= ~0x01;
            } else {
                if( p->Data ) HeapDeallocate( &cache->heap, &p->BlkId );
                Free( cache->Pool[ i ] );
                memmove( &cache->Pool[ i ], &cache->Pool[ i + 1 ], (cache->Count - i - 1) * sizeof( CachePool_t * ) );
                cache->Count--;
                cache->UsedBytes = cache->UsedBytes - p->Size;
                i--;
            }
        }            
    }
}

int CachePoolAlloc( Cache_t *cache, int PoolSize )
{
    CachePool_t **p;

    if( PoolSize < cache->Count ) return 0;
    p = (CachePool_t **)Realloc(cache->Pool, PoolSize * sizeof(CachePool_t **) );
    if( !p ) return 0;
    cache->Pool = p;
    cache->Chunks = PoolSize;
    return 1;
}

static int CacheSortUsageCb( CachePool_t **pp1, CachePool_t **pp2)
{
    CachePool_t *p1, *p2;

    p1 = *pp1;
    p2 = *pp2;
    if( p1->Locked && !p2->Locked ) return 1;
    if( p2->Locked && !p1->Locked ) return -1;
    if( p1->Usage < p2->Usage ) return -1;
    if( p1->Usage > p2->Usage ) return 1;
    if( p1->Utility > p2->Utility ) return 1;
    if( p1->Utility < p2->Utility ) return -1;
    return 0;    
}

static int CacheSortUtilityCb( CachePool_t **p1, CachePool_t **p2 )
{
    if( (*p1)->Utility > (*p2)->Utility ) return 1;
    if( (*p1)->Utility < (*p2)->Utility ) return -1;
    return 0;    
}



