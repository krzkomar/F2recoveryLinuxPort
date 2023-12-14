#include "FrameWork.h"

static unsigned int gMemAllocatedBlocksTotal = 0;
static unsigned int gMemMaxAllocatedBytesTotal = 0;
static unsigned int gMemAllocatedBytesTotal = 0;
static unsigned int gMemMaxAllocatedBlocksTotal = 0;
static void *(*gReallocCb)(void *ptr, size_t size) = MemRealloc;
static void *(*gMallocCb)(size_t nsize) = MemMalloc;
static void (*gFreeCb)(void *ptr) = MemFree;

/*******************************************************************/

void *Malloc( unsigned int size )
{
    return gMallocCb( size );
}

void *MemMalloc( size_t nsize )
{
    return malloc( nsize );
/*
    MemInt_t *p;
    
    if( !nsize ) return NULL;
    if( !( p = malloc( nsize + sizeof( MemInt_t ) + sizeof( unsigned int ) ) ) ) return NULL;
    p->Size = nsize;
    p->Guard = MEMORY_BEGIN_GUARD;
    gMemAllocatedBlocksTotal++;
    *((int *)&( (MemInt_t *)p)->Data[ nsize - sizeof( unsigned int ) ]) = MEMORY_END_GUARD;
    if( gMemAllocatedBlocksTotal > gMemMaxAllocatedBlocksTotal ) gMemMaxAllocatedBlocksTotal = gMemAllocatedBlocksTotal;
    gMemAllocatedBytesTotal += nsize;
    if( gMemAllocatedBytesTotal > gMemMaxAllocatedBytesTotal ) gMemMaxAllocatedBytesTotal = gMemAllocatedBytesTotal;
    return p->Data;
*/
}

void *Realloc( void *ptr, size_t size )
{
    return gReallocCb(ptr, size);
}

void *MemRealloc( void *ptr , size_t size )
{
    return realloc( ptr, size );
/*
    MemInt_t *p, *new_ptr;
    int k;
    
    if( !ptr ) return gMallocCb( size );
    p = (MemInt_t *)(ptr - sizeof( MemInt_t ) );
    k = p->Size;
    gMemAllocatedBytesTotal -= k;
    MemCheckBlock( p );

    if( size ) size += sizeof( MemInt_t ) + sizeof( unsigned int );
    if( !(new_ptr = realloc( p, size )) ){
	if( size ){
    	    gMemAllocatedBytesTotal += k;
    	    eprintf( "(%s,%u): ", "Memory.c", 195 );
    	    eprintf( "Realloc failure.\n" );
	} else {
    	    gMemAllocatedBlocksTotal--;
	}
	return NULL;
    }
    gMemAllocatedBytesTotal += size;
    if( gMemAllocatedBytesTotal > gMemMaxAllocatedBytesTotal ) gMemMaxAllocatedBytesTotal = gMemAllocatedBytesTotal;
    new_ptr->Size = size;
    new_ptr->Guard = MEMORY_BEGIN_GUARD;
    *((int *)&( new_ptr)->Data[ size - sizeof( unsigned int ) ]) = MEMORY_END_GUARD;
    return new_ptr->Data;
*/
}

void Free( void *ptr )
{
    gFreeCb( ptr );
}

void MemFree( void *ptr )
{
    free( ptr );
/*
    MemInt_t *p;

    if( !ptr ) return;
    p = (MemInt_t *)(ptr - sizeof( MemInt_t ) );
    MemCheckBlock( p );
    gMemAllocatedBytesTotal -= p->Size;
    free( p );
    gMemAllocatedBlocksTotal--;    
*/
}

void MemStat()
{
    if( gMallocCb != MemMalloc ) return;    
    printf( "Current memory allocated: %6d blocks, %9u bytes total\n", gMemAllocatedBlocksTotal, gMemAllocatedBytesTotal );
    printf( "Max memory allocated:     %6d blocks, %9u bytes total\n", gMemMaxAllocatedBlocksTotal, gMemMaxAllocatedBytesTotal );
}

void MemReassign( void *(*pMalloc)(size_t nsize), void *(*pRealloc)(void *ptr, size_t size), void (*pFree)(void *ptr) )
{
//    if( gWindowSystem.Init ) return;
    gReallocCb = pRealloc;
    gFreeCb = pFree;
    gMallocCb = pMalloc;
}

void *MemSetBlock( MemInt_t *ptr, int size )
{
    ptr->Size = size;
    ptr->Guard = MEMORY_BEGIN_GUARD;
    *((int *)&( ptr->Data[ size - sizeof( unsigned int ) ])) = MEMORY_END_GUARD;
    return ptr->Data;    
}

void MemCheckBlock( MemInt_t *ptr )
{
    if( ptr->Guard != MEMORY_BEGIN_GUARD ) eprintf( "Memory header stomped.\n" );
    if( *((int *)&( ptr->Data[ ptr->Size - sizeof( unsigned int ) ])) != MEMORY_END_GUARD ) eprintf( "Memory footer stomped.\n" );
}

