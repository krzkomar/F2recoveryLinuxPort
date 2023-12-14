#pragma once 

#define MEMORY_BEGIN_GUARD	0xFEEDFACE
#define MEMORY_END_GUARD	0xBEEFCAFE

#define dbg_malloc( x )	Malloc( x )
#define dbg_realloc( x, n )	Realloc( x, n )
#define dbg_calloc( x, n )	calloc( x, n )
#define dbg_free( x )	Free( x )

typedef struct // size of 8
{
    unsigned int Size;		// +0
    unsigned int Guard; 	// +4    
    char	 Data[];	// +8
} MemInt_t;

void *Malloc( unsigned int size );
void *Realloc( void *ptr, size_t size );
void Free( void *ptr );
void MemStat();	// 

void *MemMalloc( size_t nsize );
void *MemRealloc( void *ptr, size_t size );
void MemFree( void *ptr );
void MemReassign( void *(*pMalloc)(size_t nsize), void *(*pRealloc)(void *ptr, size_t size), void (*pFree)(void *ptr) );
void *MemSetBlock( MemInt_t *ptr, int size );
void MemCheckBlock( MemInt_t *ptr );


