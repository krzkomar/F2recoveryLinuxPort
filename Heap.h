#pragma once

// heap states
#define HEAP_ZERO		0x00
#define HEAP_MOVABLE		0x01
#define HEAP_LOCKED		0x02
#define	HEAP_FREE		0x04
#define HEAP_ERROR		-1

#define HEAP_FOREHEAD_GUARD	0xDEADC0DEu
#define HEAP_BACKEND_GUARD	0xACDCACDCu
#define HEAP_MEM_GUARD( addr )  *((unsigned int *)(addr))
#define HEAP_PAYLOAD		( sizeof( HeapBlk_t ) + sizeof( int ) ) // 20 = (forehead header = 16 ) + (backend guard = 4)

#define HEAP_HANDLERS	64

typedef struct // size of 16
{
    unsigned int Guard; //0
    int		Size;  //1
    int		State; //2
    int		Id;    //3
    char	Data[];
} HeapBlk_t;

typedef struct // size of 8
{
    int		State;
    HeapBlk_t	*Blk;
} HeapHdr_t;

typedef struct // size of 48
{
    int		TotAllocated;
    int 	TotFreeBlk;
    int 	TotMoveableBlk;
    int 	TotLockedBlk;
    int 	TotSystemBlk;
    int 	TotHandles;
    int 	TotFreeSize;
    int 	TotMoveableSize;
    int 	TotLockedSize;
    int 	TotSystemSize;
    HeapHdr_t 	*Hdr;
    HeapBlk_t 	*Blk;
} Heap_t;

typedef struct // size of 16
{
  HeapBlk_t *Block;
  int TotMerged;
  int Stat1Merged;
  int Size;
} HeapMovBlk_t;

int HeapInit( Heap_t *heap, unsigned int Size );
int HeapClose( Heap_t *heap );
int HeapAllocate( Heap_t *heap, int *pBlkIdx, unsigned int BytesToAlloc, unsigned char NoAllocBlk );
int HeapDeallocate( Heap_t *BlockPool, int *BlkNum );
int HeapLockBlock( Heap_t *heap, int Idx, void **data );
int HeapUnlockBlock( Heap_t *heap, int idx );

// internal -> to move to static 
int HeapValidate( Heap_t *heap );
int HeapStatus( Heap_t *heap, char *str );
int HeapFreeHandlers( Heap_t *heap );
int HeapHandlerSetFree( Heap_t *Heap, int Idx );
int HeapHandlersSetFreeRange( HeapHdr_t *Handler, int Idx );
int HeapFindFreeBlock( Heap_t *heap, int BytesSize, HeapBlk_t **blk, char NoAllocFlg );
int HeapFreeBlksMerge( Heap_t *heap );
int HeapSortFree( Heap_t *heap );
int HeapBuildMovableList( Heap_t *heap, int *MovableCnt, unsigned int *MaxMerged );
int HeapSortMovable( unsigned int Total );
int HeapRebuildHeap( int idx );
int HeapSort( unsigned int Total );
int HeapReallocUnk02( unsigned int Elements );


