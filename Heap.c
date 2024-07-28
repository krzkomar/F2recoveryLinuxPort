#include "FrameWork.h"

static int HeapStatus( Heap_t *heap, char *str );
static int HeapFreeBlksMerge( Heap_t *heap );
static int HeapBuildMovableList( Heap_t *heap, int *MovableCnt, unsigned int *MaxMerged );
static int HeapRebuildHeap( int idx );
static int HeapOrderRealloc( int Elements );
static int HeapInternalInit();
static int HeapInternalFree();
static int HeapAcqHandle( Heap_t *heap, int *pIdx );
static int HeapInitializeHandles( Heap_t *heap );
static int HeapSortCb( HeapBlk_t **p1, HeapBlk_t **p2 );
static int HeapFindFreeBlock( Heap_t *heap, int BytesSize, HeapBlk_t **blk, char NoAllocFlg );
static int HeapMovableSortCb( HeapMovBlk_t *blk1, HeapMovBlk_t *blk2 );

static HeapBlk_t 	**gHeapFreeBlks = NULL;
static HeapMovBlk_t 	*gHeapMovableList = NULL;
static HeapBlk_t 	**gHeapBlks = NULL;
static int 		*gHeapOrder = NULL;
static int gHeapTotFreeBlk = 0;
static int gHeapMovableEntries = 0;
static int gHeapBlksAlloc;
static int gHeapOrderAlloc;
static int gHeapsTotal = 0;

int HeapInit( Heap_t *heap, unsigned int Size )
{
    if( !heap ) return 0;
    if( !gHeapsTotal ){
	if( !HeapInternalInit() ) return 0;
    }
    memset( heap, 0x00, sizeof( Heap_t ) );
    if( HeapInitializeHandles( heap ) == 1 ){
        Size += Size / 1024;
        if( (heap->Blk = (HeapBlk_t *)Malloc( Size ) ) ){
            heap->TotAllocated = Size;
            heap->TotFreeSize  = heap->TotAllocated - HEAP_PAYLOAD;
            heap->TotFreeBlk   = 1;
            heap->Blk->Guard   = HEAP_FOREHEAD_GUARD;
            heap->Blk->Size    = heap->TotFreeSize;
            heap->Blk->State   = 0;
            heap->Blk->Id      = -1;
            HEAP_SET_MEM_GUARD( heap->Blk->Data, heap->Blk->Size, HEAP_BACKEND_GUARD );
            gHeapsTotal++;
            return 1;
        }
    }
    if( !gHeapsTotal ) HeapInternalFree();
    return 0;
}

int HeapClose( Heap_t *heap )
{
    int i;

    if( !heap ) return 0;    
    for( i = 0; i < heap->TotHandles; i++ ){
        if( (heap->Hdr[ i ].State == HEAP_FREE ) && heap->Hdr[ i ].Blk ) Free( heap->Hdr[ i ].Blk );
    }
    if( heap->Hdr ){
        Free( heap->Hdr );
        heap->Hdr = NULL;
        heap->TotHandles = 0;
    }
    if( heap->Blk ) Free( heap->Blk );
    memset( heap, 0x00, sizeof( Heap_t ) );
    if( !--gHeapsTotal ) HeapInternalFree();
    return 1;    
}

int HeapAllocate( Heap_t *heap, int *pBlkIdx, unsigned int BytesToAlloc, unsigned char NoAllocBlk )
{
    int stat, sz, Idx, Size;
    HeapBlk_t *h, *NewBlock;

    if( heap && pBlkIdx && BytesToAlloc ){
	if( NoAllocBlk != 1 ) NoAllocBlk = 0;
	if( HeapFindFreeBlock( heap, BytesToAlloc, &NewBlock, NoAllocBlk ) == 1 ){
	    stat = NewBlock->State;
	    if( HeapAcqHandle( heap, &Idx ) == 1 ){
		Size = NewBlock->Size;
		if( stat == HEAP_ZERO ){
		    sz = Size - BytesToAlloc;
		    if( sz > HEAP_PAYLOAD + 128 ){
    			NewBlock->Size = BytesToAlloc;
    			Size = BytesToAlloc;
    			HEAP_SET_MEM_GUARD( NewBlock->Data, NewBlock->Size, HEAP_BACKEND_GUARD );
    			h = (HeapBlk_t *)&NewBlock->Data[ NewBlock->Size + HEAP_GUARD_SIZE ];
    			h->State = HEAP_ZERO;
    			h->Id = -1;
    			h->Size = sz - HEAP_PAYLOAD;
    			h->Guard = HEAP_FOREHEAD_GUARD;
    			HEAP_SET_MEM_GUARD( h->Data, h->Size, HEAP_BACKEND_GUARD );
    			heap->TotFreeBlk++;
    			heap->TotFreeSize -= HEAP_PAYLOAD;
		    }
		    NewBlock->Id = Idx;
		    NewBlock->State = HEAP_MOVABLE;
		    heap->Hdr[ Idx ].State = HEAP_MOVABLE;
		    heap->Hdr[ Idx ].Blk = NewBlock;
		    heap->TotFreeBlk--;
		    heap->TotMoveableBlk++;
		    heap->TotFreeSize -= Size;
		    heap->TotMoveableSize += Size;
		    *pBlkIdx = Idx;
		    return 1;
		}
		if( stat == HEAP_FREE ){
    		    NewBlock->Id = Idx;
    		    heap->Hdr[ Idx ].State = HEAP_FREE;
    		    heap->Hdr[ Idx ].Blk = NewBlock;
    		    heap->TotSystemBlk++;
    		    heap->TotSystemSize += Size;
    		    *pBlkIdx = Idx;
    		    return 1;
		}
		heap->Hdr[ Idx ].State = HEAP_ERROR;
		heap->Hdr[ Idx ].Blk = 0;
		eprintf("Heap Error: Unknown block state during allocation.");
	    }
	    eprintf( "Heap Error: Could not acquire handle for new block." );
	    if( stat == HEAP_FREE ) Free( NewBlock );
	}
    }
    eprintf( "Heap Warning: Could not allocate block of %d bytes.", BytesToAlloc );
    return 0;    
}

int HeapDeallocate( Heap_t *BlockPool, int *BlkNum )
{
    int Idx, State;
    HeapBlk_t *Blk;

    if( BlockPool && BlkNum ){
        Idx = *BlkNum;
        State = BlockPool->Hdr[ Idx ].State;
        Blk = BlockPool->Hdr[ Idx ].Blk;
        if( Blk->Guard != HEAP_FOREHEAD_GUARD ){
    	    eprintf( "Heap Error: Bad guard begin detected during deallocate." );
DBG_CRASH;
        }
        if( HEAP_GET_MEM_GUARD( Blk->Data, Blk->Size ) != HEAP_BACKEND_GUARD ){
    	    eprintf( "Heap Error: Bad guard end detected during deallocate." );
DBG_CRASH;
        }
        if( State != Blk->State ) eprintf( "Heap Error: Mismatched block states detected during deallocate." );
        if( !(State & HEAP_LOCKED) ){
            if( State == HEAP_MOVABLE ){
                Blk->Id = -1;
                Blk->State = HEAP_ZERO;
                BlockPool->TotFreeBlk++;
                BlockPool->TotMoveableBlk--;
                BlockPool->TotFreeSize += Blk->Size;
                BlockPool->TotMoveableSize -= Blk->Size;
                BlockPool->Hdr[ Idx ].State = HEAP_ERROR;
                BlockPool->Hdr[ Idx ].Blk = NULL;
                return 1;
            }
            if( State == HEAP_FREE ){
                Free( Blk );
                BlockPool->TotSystemSize -= Blk->Size;
                BlockPool->TotSystemBlk--;
                BlockPool->Hdr[ Idx ].State = HEAP_ERROR;
                BlockPool->Hdr[ Idx ].Blk = NULL;
                return 1;
            }
            eprintf( "Heap Error: Unknown block state during deallocation." );
        }
        eprintf( "Heap Error: Attempt to deallocate locked block." );
    }
    eprintf( "Heap Error: Could not deallocate block." );
    return 0;
}

int HeapLockBlock( Heap_t *heap, int Idx, void **data )
{
    int stat;
    HeapBlk_t *Blk;

    if( heap ){
        stat = heap->Hdr[ Idx ].State;
        Blk = heap->Hdr[ Idx ].Blk;
        if( Blk->Guard != HEAP_FOREHEAD_GUARD ){
         eprintf("Heap Error: Bad guard begin detected during lock.");
DBG_CRASH;
        }
        if( HEAP_GET_MEM_GUARD( Blk->Data, Blk->Size ) != HEAP_BACKEND_GUARD ){
    	    eprintf("Heap Error: Bad guard end detected during lock.");
DBG_CRASH;
        }
        if( stat != Blk->State ) eprintf("Heap Error: Mismatched block states detected during lock.");
        if( !( stat & HEAP_LOCKED ) ){
            if( stat == HEAP_MOVABLE ){
                Blk->State = HEAP_LOCKED;
                heap->Hdr[ Idx ].State = HEAP_LOCKED;
                heap->TotMoveableBlk--;
                heap->TotLockedBlk++;
                heap->TotMoveableSize -= Blk->Size;
                heap->TotLockedSize += Blk->Size;
                *data = Blk->Data;
                return 1;
            }
            if( stat == HEAP_FREE ){
                Blk->State |= HEAP_LOCKED;
                heap->Hdr[ Idx ].State |= HEAP_LOCKED;
                *data = Blk->Data;
                return 1;
            }
            eprintf("Heap Error: Unknown block state during lock.");
        }
        eprintf("Heap Error: Attempt to lock a previously locked block.");
    }
    eprintf("Heap Error: Could not lock block.");
    return 0;
}

int HeapUnlockBlock( Heap_t *heap, int idx )
{
    int stat; 
    HeapBlk_t *dat;

    if( heap ){
	stat = heap->Hdr[ idx ].State;
	dat = heap->Hdr[ idx ].Blk;
	if( dat->Guard != HEAP_FOREHEAD_GUARD ){
	    eprintf( "Heap Error: Bad guard begin detected during unlock." );
DBG_CRASH;
	}
	if( HEAP_GET_MEM_GUARD( dat->Data, dat->Size ) != HEAP_BACKEND_GUARD ){
	    eprintf( "Heap Error: Bad guard end detected during unlock." );	
DBG_CRASH;
	}
	if( stat != dat->State ) eprintf( "Heap Error: Mismatched block states detected during unlock." );
	if( stat & HEAP_LOCKED ){
	    if( stat & HEAP_FREE ){
    		dat->State = HEAP_FREE;
    		heap->Hdr[ idx ].State = HEAP_FREE;
	    } else {
		dat->State = HEAP_MOVABLE;
		heap->Hdr[ idx ].State = HEAP_MOVABLE;
		heap->TotMoveableBlk++;
		heap->TotLockedBlk--;
    		heap->TotMoveableSize += dat->Size;
		heap->TotLockedSize -= dat->Size;
	    }
	    return 1;
	}
	eprintf("Heap Error: Attempt to unlock a previously unlocked block.");	
    }
    eprintf("Heap Error: Could not unlock block.");
    return 0;
}

int HeapStatus( Heap_t *heap, char *str )
{
    if( !heap || !str ) return 0;
    sprintf( str, 
	"[Heap]\n"
	"Total free blocks: %d\n"
	"Total free size: %d\n"
	"Total moveable blocks: %d\n"
	"Total moveable size: %d\n"
	"Total locked blocks: %d\n"
	"Total locked size: %d\n"
	"Total system blocks: %d\n"
	"Total system size: %d\n"
	"Total handles: %d\n"
	"Total heaps: %d", 
	heap->TotFreeBlk, heap->TotFreeSize, heap->TotMoveableBlk, heap->TotMoveableSize, 
	heap->TotLockedBlk, heap->TotLockedSize, heap->TotSystemBlk, heap->TotSystemSize, 
	heap->TotHandles, gHeapsTotal
    );
    return 1;
}

static int HeapInternalInit()
{
    if( !(gHeapFreeBlks = (HeapBlk_t **)Malloc( 128 * sizeof( HeapBlk_t * ) ) ) ) return 0;    
    gHeapTotFreeBlk = 128;    
    if( (gHeapMovableList = (HeapMovBlk_t *)Malloc( 64 * sizeof( HeapMovBlk_t ))) ){
        gHeapMovableEntries = 64;        
        if( (gHeapBlks = (HeapBlk_t **)Malloc( 64 * sizeof(HeapBlk_t *))) ){
            gHeapBlksAlloc = 64;            
            if( (gHeapOrder = (int *)Malloc( 64 * sizeof( int * ))) ){
                gHeapOrderAlloc = 64;
                return 1;
            }
            Free( gHeapBlks );
        }
        Free( gHeapMovableList );
    }
    Free( gHeapFreeBlks );    
    return 0;
}

static int HeapInternalFree()
{
    if( gHeapOrder ){
        Free( gHeapOrder );
        gHeapOrder = NULL;
    }
    gHeapOrderAlloc = 0;
    if( gHeapBlks ){
        Free( gHeapBlks );
        gHeapBlks = NULL;
    }
    gHeapBlksAlloc = 0;
    if( gHeapMovableList ){
        Free( gHeapMovableList );
        gHeapMovableList = NULL;
    }
    gHeapMovableEntries = 0;
    if( gHeapFreeBlks ){
        Free( gHeapFreeBlks );
        gHeapFreeBlks = NULL;
    }
    gHeapTotFreeBlk = 0;
    return 1;
}

static int HeapInitializeHandles( Heap_t *heap )
{
    int i;

    if( !( heap->Hdr = (HeapHdr_t *)Malloc( 64 * sizeof( HeapHdr_t ) ) ) ){
        eprintf( "Heap Error: Could not initialize handles." );
        return 0;
    }
    for( i = 0; i < HEAP_HANDLERS; i++ ){
        heap->Hdr[ i ].State = -1;
        heap->Hdr[ i ].Blk = NULL;
    }
    heap->TotHandles = HEAP_HANDLERS;
    return 1;
}

static int HeapAcqHandle( Heap_t *heap, int *pIdx )
{
    int idx;
    HeapHdr_t *w, *hn, *t, *p;

    w = heap->Hdr;
    for( idx = 0; idx < heap->TotHandles; idx++, w++ ){
    	if( w->State == HEAP_ERROR ){
    	    *pIdx = idx;
    	    return 1;
    	}    	
    }
    if( !(p = (HeapHdr_t *)Realloc(heap->Hdr, (heap->TotHandles + HEAP_HANDLERS) * sizeof( HeapHdr_t ) )) ) return 0;
    heap->Hdr = p;
    hn = &heap->Hdr[ heap->TotHandles ];
    t = hn + HEAP_HANDLERS;
    do{
        hn->State = HEAP_ERROR;
        hn->Blk = NULL;
        hn++;
    } while ( hn != t );
    *pIdx = heap->TotHandles;
    heap->TotHandles += HEAP_HANDLERS;
    return 1;
}

int HeapFindFreeBlock( Heap_t *heap, int BytesSize, HeapBlk_t **blk, char NoAllocFlg )
{
    HeapBlk_t *pb, *bm, *bw, *bk, *BlkA, *BlkB, *pblk;
    HeapMovBlk_t *mvblk;
    unsigned int mv2;
    int MovableNum,n,MvBlk,fb,i,w,k,mrg,nn,rk,j,kk,ii,Size;
    char stmp[512];

    if( (HeapFreeBlksMerge( heap ) == 1) && (BytesSize <= heap->TotFreeSize) ){
	if( heap->TotFreeBlk > 1 ) qsort( gHeapFreeBlks, heap->TotFreeBlk, sizeof( HeapBlk_t * ), (void *)HeapSortCb );
	k = gHeapFreeBlks[ heap->TotFreeBlk - 1 ]->Size;
	if( BytesSize <= k ){        
    	    for( fb = i = 0; fb < heap->TotFreeBlk; i++, fb++ ){
    		pb = (HeapBlk_t *)gHeapFreeBlks[ i ];
        	if( pb->Size >= BytesSize ){ *blk = pb; return 1; }
    	    }        
	}
	if( HeapBuildMovableList( heap, &MovableNum, &mv2 ) == 1 ){
	    if( HeapOrderRealloc( mv2 ) ){
		qsort( gHeapMovableList, MovableNum, sizeof( HeapMovBlk_t ), (void *)HeapMovableSortCb );
		for( n = 0; n < MovableNum; n++ ){
    		    MvBlk = gHeapMovableList[ n ].Size + HEAP_PAYLOAD * ( gHeapMovableList[ n ].TotMerged - 1 );
    		    if( MvBlk >= BytesSize && HeapRebuildHeap( n ) == 1 ){
        		qsort( gHeapBlks, gHeapMovableList[ n ].Stat1Merged, sizeof( HeapBlk_t *), (void *)HeapSortCb );
        		for( w = nn = 0; nn < ( mrg = gHeapMovableList[ n ].Stat1Merged ); nn++ ){
            		    bk = gHeapBlks[ nn ];
            		    if( k >= bk->Size ){                    
                		for( rk = 0; rk < heap->TotFreeBlk; rk++ ){
                    		    bm = gHeapFreeBlks[ rk ];
                    		    if( bm->Size < bk->Size ) continue;
                    		    bw = gHeapMovableList[ n ].Block;
                    		    if( !((bm >= bw) && bm < (HeapBlk_t *)&bw[ MvBlk + HEAP_GUARD_SIZE ] ) ) continue;
                    		    for( j = 0; j < w && ( rk != gHeapOrder[ j ] ); j++ );
                    		    if( j == w ){
                    			gHeapOrder[ w++ ] = rk;
                    		        break;
                    		    }
                		}
            		    }                
        		}
        		if( w == mrg ){
			    for( kk = ii = 0; ii < gHeapMovableList[ n ].Stat1Merged; ii++ ){
    				BlkB = gHeapBlks[ ii ];
    				if( k < BlkB->Size ) continue;
    				BlkA = gHeapFreeBlks[ gHeapOrder[ kk ] ];
    				memcpy( BlkA, BlkB, BlkB->Size + HEAP_PAYLOAD );
    				heap->Hdr[ BlkA->Id ].Blk = BlkA;
    				Size = BlkA->Size - BlkB->Size;
    				if( Size ){
        			    if( Size < 148 ) {
            				BlkA->Size += Size;
            			        HEAP_SET_MEM_GUARD( BlkA, BlkA->Size + HEAP_GUARD_SIZE, HEAP_BACKEND_GUARD );
            			        heap->TotFreeSize -= Size;
            			        heap->TotMoveableSize = Size + heap->TotMoveableSize;
        			    } else {
            				pblk = (HeapBlk_t *)&BlkA->Data[ BlkA->Size + HEAP_GUARD_SIZE ];
            			        pblk->State = 0;
            			        pblk->Id = -1;
            			        pblk->Size = Size - HEAP_PAYLOAD;
            			        pblk->Guard = HEAP_FOREHEAD_GUARD;
            			        HEAP_SET_MEM_GUARD( pblk->Data, pblk->Size, HEAP_BACKEND_GUARD );
            			        heap->TotFreeBlk++;
            			        heap->TotFreeSize = heap->TotFreeSize - HEAP_PAYLOAD;
        			    }
    				}
    				kk++;
			    }
			    mvblk = &gHeapMovableList[ n ];
			    heap->TotFreeBlk -= mvblk->TotMerged - 1;
			    heap->TotFreeSize += HEAP_PAYLOAD * ( mvblk->TotMerged - 1 );
			    *blk = mvblk->Block;
			    (*blk)->Guard = HEAP_FOREHEAD_GUARD;
			    (*blk)->Size = MvBlk;
			    (*blk)->State = HEAP_ZERO;
			    (*blk)->Id = -1;
			    HEAP_SET_MEM_GUARD( (*blk)->Data,(*blk)->Size, HEAP_BACKEND_GUARD );
			    return 1;        		
        		}
    		    }        
		}
	    }
	}
    }
    if( HeapStatus( heap, stmp ) ) eprintf( "\n%s", stmp );
    if( !NoAllocFlg ){
        eprintf("Allocating block from system memory...");
        pblk = (HeapBlk_t *)Malloc(BytesSize + HEAP_PAYLOAD);
        *blk = pblk;
        if( pblk ){
            pblk->Guard = HEAP_FOREHEAD_GUARD;
            (*blk)->Size = BytesSize;
            (*blk)->State = HEAP_FREE;
            HEAP_SET_MEM_GUARD( (*blk)->Data, (*blk)->Size, HEAP_BACKEND_GUARD );
            return 1;
        }
        eprintf("fatal error: internal_malloc() failed in heap_find_free_block()!");
    }
    return 0;
}

int HeapFreeBlksMerge( Heap_t *heap )
{
    int FreeBlk,all,i;
    void *p;
    HeapBlk_t *blk, *q;

    if( (FreeBlk = heap->TotFreeBlk) == 0  ) return 0;
     
    // reallocate free blocks table if it's less than number of free blocks
    if( FreeBlk > gHeapTotFreeBlk ){
        if( !( p = Realloc( gHeapFreeBlks, FreeBlk * sizeof( HeapBlk_t *) ) ) ) return 0;
        gHeapFreeBlks = (HeapBlk_t **)p;
        gHeapTotFreeBlk = FreeBlk;
    }
    all = heap->TotMoveableBlk + heap->TotFreeBlk + heap->TotLockedBlk;
    if( all == 0 ) return FreeBlk; // no blocks available

    blk = heap->Blk;
    for( i = 0; ( i < heap->TotFreeBlk ) && all; all--, blk = (HeapBlk_t *)((char *)blk + blk->Size + HEAP_PAYLOAD) ){
        if( blk->State != HEAP_ZERO ) continue; // free block        
        for( ;all > 1; all-- ){
            q = (HeapBlk_t *)(&blk->Data[ blk->Size ] + HEAP_GUARD_SIZE ); // +4 -> HEAP_BACKEND_GUARD (4 bytes)
            if( q->State ) break; // occupied block
            blk->Size += q->Size + HEAP_PAYLOAD;
            heap->TotFreeBlk--;
            heap->TotFreeSize += HEAP_PAYLOAD;
        }            
        gHeapFreeBlks[ i++ ] = blk;
    }            
    return 1;
}

static int HeapSortCb( HeapBlk_t **p1, HeapBlk_t **p2 )
{
    return (*p1)->Size - (*p2)->Size;
}

int HeapBuildMovableList( Heap_t *heap, int *MovableCnt, unsigned int *MaxMerged )
{
    HeapMovBlk_t *mb;
    unsigned int mvb;
    HeapMovBlk_t *p;
    HeapBlk_t *blk;
    int total;
    unsigned int State;
    HeapMovBlk_t *mv;
    HeapBlk_t *dat;
    unsigned int k;
    unsigned int max;
    unsigned int idx;
    int i;

    mb = gHeapMovableList;
    mvb = heap->TotMoveableBlk + heap->TotFreeBlk;
    if( mvb <= 2 ){ eprintf( "<[couldn't build moveable list]>\n" ); return 0; }
    if( mvb > gHeapMovableEntries ){
        p = Realloc(gHeapMovableList, 16 * mvb);
        if( !p ) return 0;
        mb = p;
        gHeapMovableEntries = mvb;
    }
    blk = heap->Blk;
    idx = 0;
    max = 0;
    total = heap->TotMoveableBlk + heap->TotFreeBlk + heap->TotLockedBlk;
    i = 0;
    for( ;total; total--, blk = (HeapBlk_t *)( &blk->Data[ blk->Size ] + HEAP_GUARD_SIZE ) ){
        if( idx >= mvb ) break;
        State = blk->State;
        if( State <= 1 ){
            mv = &mb[i];
            mv->TotMerged = 1;
            mv->Stat1Merged = 0;
            mv->Block = blk;
            mv->Size = blk->Size;
            if( State == 1 ) mv->Stat1Merged = 1;
            while( --total ){
                dat = (HeapBlk_t *)&blk->Data[blk->Size];
                k = dat->Id;
                blk = (HeapBlk_t *)&dat->Size;
                if( k > 1 ) break;
                mv = &mb[i];
                ++mv->TotMerged;
                mv->Size += blk->Size;
                if( k == 1 ) mv->Stat1Merged++;
            }
            if( mb[i].TotMerged > max ) max = mb[i].TotMerged;
            idx++;
            i++;
            if( !total ) break;
        }        
    }
    *MovableCnt = idx;
    gHeapMovableList = mb;
    *MaxMerged = max;
    return 1;
}

static int HeapMovableSortCb( HeapMovBlk_t *blk1, HeapMovBlk_t *blk2 )
{
    return blk1->Size - blk2->Size;
}

int HeapRebuildHeap( int idx )
{
    HeapBlk_t **p, *blk;
    unsigned int i, n, w;

    n = gHeapMovableList[ idx ].Stat1Merged;
    if( n > gHeapBlksAlloc ){
        if( !(p = Realloc( gHeapBlks, n * sizeof( HeapBlk_t * ) ) ) ) return 0;
        gHeapBlks = p;
        gHeapBlksAlloc = n;
    }
    blk = gHeapMovableList[ idx ].Block;
    for( i = w = 0; i < gHeapMovableList[ idx ].TotMerged; i++, blk = (HeapBlk_t *)( &blk->Data[ blk->Size ] + HEAP_GUARD_SIZE ) ){
        if( blk->State != 1 ) continue;
        gHeapBlks[ w++ ] = blk;
    }
    return (w == n);
}

int HeapReallocOrder( unsigned int Elements )
{
    int *p;

    if( Elements > gHeapOrderAlloc ){
        p = Realloc( gHeapOrder, Elements * sizeof( int * ) );
        if( !p ) return 0;
        gHeapOrderAlloc = Elements;
        gHeapOrder = p;
    }
    return 1;
}

static int HeapOrderRealloc( int Elements ) // inlined
{
    void *p;
    if( Elements <= gHeapOrderAlloc ){
	return 1;
    } else {
	if( (p = Realloc( gHeapOrder, Elements * sizeof( int * ))) ){
	    gHeapOrderAlloc = Elements;
	    gHeapOrder = p;
	    return 1;
	}
    }
    return 0;
}

#if __COMPILE_JUNK__
static int HeapValidate( Heap_t *heap );
static int HeapFreeHandlers( Heap_t *heap );
static int HeapHandlerSetFree( Heap_t *Heap, int Idx );
static int HeapHandlersSetFreeRange( HeapHdr_t *Handler, int Idx );
static int HeapSortFree( Heap_t *heap );
static int HeapSortMovable( unsigned int Total );
static int HeapSort( unsigned int Total );

static int HeapSort( unsigned int Total )
{
    qsort( gHeapBlks, Total, sizeof( HeapBlk_t * ), (void *)HeapSortCb );
    return 1;
}

static int HeapSortMovable( unsigned int Total )
{
    qsort( gHeapMovableList, Total, sizeof( HeapBlk_t * ), (void *)HeapMovableSortCb );
    return 1;
}

static int HeapSortFree( Heap_t *heap )
{
    if( heap->TotFreeBlk > 1 ) qsort( gHeapFreeBlks, heap->TotFreeBlk, sizeof( HeapBlk_t * ), (void *)HeapSortCb );
    return 1;
}

static int HeapHandlerSetFree( Heap_t *Heap, int Idx )
{
    Heap->Hdr[ Idx ].State = HEAP_ERROR;
    Heap->Hdr[ Idx ].Blk = 0;
    return 1;
}

static int HeapHandlersSetFreeRange( HeapHdr_t *Handler, int Idx )
{
    HeapHdr_t *p;

    if( Idx ){
        p = &Handler[ Idx ];
        do{
            Handler->State = HEAP_ERROR;
            Handler->Blk = 0;
            Handler++;
        } while ( Handler < p );
    }
    return 1;
}

static int HeapFreeHandlers( Heap_t *heap )
{
    if( !heap->Hdr ) return 0;
    Free( heap->Hdr );
    heap->Hdr = NULL;
    heap->TotHandles = 0;
    return 1;
}

static int HeapValidate( Heap_t *heap )
{
    HeapBlk_t *Blk;
    HeapHdr_t *handler;
    int MovBlks, LockBlks, allocated, stat, Handles, Total;
    int SysBlks, LockSize, MovSize, FreeBlks, FreeSize, SysSize;

    eprintf( "Validating heap...\n" );
    FreeSize = FreeBlks = MovSize = LockSize = SysSize = SysBlks = MovBlks = LockBlks = 0;
    Total = heap->TotMoveableBlk + heap->TotFreeBlk + heap->TotLockedBlk;
    Blk = heap->Blk;    
    for( allocated = 0; allocated < Total; allocated++ ){
        if( Blk->Guard != HEAP_FOREHEAD_GUARD ){
            eprintf("Bad guard begin detected during validate.\n");
            break;
        }
        if( HEAP_GET_MEM_GUARD( Blk->Data, Blk->Size ) != HEAP_BACKEND_GUARD ){
            eprintf("Bad guard end detected during validate.\n");
            break;            
        }
        stat = Blk->State;
        if( stat ){
            if( stat == HEAP_MOVABLE ){
                MovBlks++;
                MovSize += Blk->Size;
            } else if( stat == HEAP_LOCKED ){
                LockBlks++;
                LockSize += Blk->Size;
            }
        } else {
            FreeBlks++;
            FreeSize += Blk->Size;
        }
        Blk = (HeapBlk_t *)((char *)Blk + Blk->Size + HEAP_PAYLOAD);
        if( allocated != Total - 1 && Blk > (HeapBlk_t *)((char *)heap->Blk + heap->TotAllocated) ){
    	    eprintf("Ran off end of heap during validate!\n");
    	    break;    	    
        }            
    }
    if( allocated != Total ) return 0;
    if( FreeBlks != heap->TotFreeBlk      ){ eprintf( "Invalid number of free blocks.\n" ); return 0; }
    if( FreeSize != heap->TotFreeSize     ){ eprintf( "Invalid size of free blocks.\n" ); return 0; }
    if( MovBlks  != heap->TotMoveableBlk  ){ eprintf( "Invalid number of moveable blocks.\n" ); return 0; }
    if( MovSize  != heap->TotMoveableSize ){ eprintf( "Invalid size of moveable blocks.\n" ); return 0; }
    if( LockBlks != heap->TotLockedBlk    ){ eprintf( "Invalid number of locked blocks.\n" ); return 0; }
    if( LockSize != heap->TotLockedSize   ){ eprintf( "Invalid size of locked blocks.\n" ); return 0; }
    eprintf( "Heap is O.K.\n" );
    handler = heap->Hdr;
    for( Handles = 0; Handles < heap->TotHandles; Handles++, handler++ ){
        if( (handler->State != HEAP_ERROR) && (handler->State & HEAP_FREE) == 0 ) continue;
        SysBlks++;
	if( handler->Blk == NULL ) break;
        SysSize += handler->Blk->Size;
        if( handler->Blk->Guard != HEAP_FOREHEAD_GUARD ){
            eprintf( "Bad guard begin detected in system block during validate.\n" );
            break;
        }
        if( HEAP_GET_MEM_GUARD( handler->Blk->Data, handler->Blk->Size ) != HEAP_BACKEND_GUARD ){
	    eprintf( "Bad guard end detected in system block during validate.\n" );
	    break;
        }
    }
    if( Handles != heap->TotHandles ) return 0;
    if( SysBlks != heap->TotSystemBlk  ){ eprintf( "Invalid number of system blocks.\n" ); return 0; }
    if( SysSize != heap->TotSystemSize ){ eprintf( "Invalid size of system blocks.\n" ); return 0; }
    return 1;                                                                                            
}

#endif