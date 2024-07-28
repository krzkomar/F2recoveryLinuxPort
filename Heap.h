#pragma once
#include <stdint.h>

// heap states
#define HEAP_ZERO		0x00
#define HEAP_MOVABLE		0x01
#define HEAP_LOCKED		0x02
#define	HEAP_FREE		0x04
#define HEAP_ERROR		-1

#define HEAP_GUARD_TYPE		uint32_t
#define HEAP_FOREHEAD_GUARD	0xDEADC0DEu
#define HEAP_BACKEND_GUARD	0xACDCACDCu
//#define HEAP_BACKEND_GUARD	0x12ABCDEFu

#define HEAP_GUARD_SIZE		sizeof( HEAP_GUARD_TYPE )
#define HEAP_BYTE( base, idx )	*((char *)((char*)base + idx ))
#define HEAP_INT( base, idx )	*((int  *)((char*)base + idx ))
#define HEAP_SET_MEM_GUARD( addr, idx, magic ) HEAP_INT( addr, idx ) = magic
//    HEAP_BYTE( addr, idx + 0 ) = magic >> 24; 
//    HEAP_BYTE( addr, idx + 1 ) = magic >> 16; 
//    HEAP_BYTE( addr, idx + 2 ) = magic >> 8; 
//    HEAP_BYTE( addr, idx + 3 ) = magic >> 0; 

#define HEAP_GET_MEM_GUARD( addr, idx )  HEAP_INT( addr, idx )
//    ((HEAP_BYTE( addr, idx + 0 ) << 24) & 0xff000000 ) | 
//    ((HEAP_BYTE( addr, idx + 1 ) << 16) & 0x00ff0000 ) | 
//    ((HEAP_BYTE( addr, idx + 2 ) << 8)  & 0x0000ff00 ) | 
//    ((HEAP_BYTE( addr, idx + 3 ) << 0)  & 0x000000ff )

#define HEAP_PAYLOAD		( sizeof( HeapBlk_t ) + HEAP_GUARD_SIZE ) // 20(28) = (forehead header = 16(20) ) + (backend guard = 4(8))

#define HEAP_HANDLERS	64

typedef struct
{
    HEAP_GUARD_TYPE	Guard;
    int			Size;
    int			State;
    int			Id;
    char		Data[];
} HeapBlk_t;

typedef struct
{
    int		State;
    HeapBlk_t	*Blk;
} HeapHdr_t;

typedef struct
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

typedef struct
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

