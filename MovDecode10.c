//#define pINT( p )	((int *)(p))
#define pCHR( p )	((char *)(p))

int static inline MovGetBit10( short **pSkip, unsigned short *tmp )
{
    short *p = *pSkip, bit;
    
    while( 1 ){
        bit = *tmp & 0x8000;
        *tmp <<= 1;
        if( *tmp ) break;
        *tmp = *p++;
    }
    *pSkip = p;
    return bit;
}

void static inline MovCopyBlock10( unsigned char **pDst, unsigned char *src, int DstPitch, int SrcPitch )
{
    unsigned char *dst = *pDst;

    pINT( dst )[ 0 ] = pINT( src )[ 0 ]; pINT( dst )[ 1 ] = pINT( src )[ 1 ]; src += SrcPitch + 8; dst += DstPitch + 8;
    pINT( dst )[ 0 ] = pINT( src )[ 0 ]; pINT( dst )[ 1 ] = pINT( src )[ 1 ]; src += SrcPitch + 8; dst += DstPitch + 8;
    pINT( dst )[ 0 ] = pINT( src )[ 0 ]; pINT( dst )[ 1 ] = pINT( src )[ 1 ]; src += SrcPitch + 8; dst += DstPitch + 8;
    pINT( dst )[ 0 ] = pINT( src )[ 0 ]; pINT( dst )[ 1 ] = pINT( src )[ 1 ]; src += SrcPitch + 8; dst += DstPitch + 8;
    pINT( dst )[ 0 ] = pINT( src )[ 0 ]; pINT( dst )[ 1 ] = pINT( src )[ 1 ]; src += SrcPitch + 8; dst += DstPitch + 8;
    pINT( dst )[ 0 ] = pINT( src )[ 0 ]; pINT( dst )[ 1 ] = pINT( src )[ 1 ]; src += SrcPitch + 8; dst += DstPitch + 8;
    pINT( dst )[ 0 ] = pINT( src )[ 0 ]; pINT( dst )[ 1 ] = pINT( src )[ 1 ]; src += SrcPitch + 8; dst += DstPitch + 8;
    pINT( dst )[ 0 ] = pINT( src )[ 0 ]; pINT( dst )[ 1 ] = pINT( src )[ 1 ];
    *pDst = dst - gMovScrBufSizeB + 8;
}

void MovDecodeFrame10( short *pSkip, short *pMap, char *pData, short Xpos, short Ypos, short Width, short Height )
{
    unsigned short tmp, offset;
    int page,bit,NextLine;
    short *pm,*skip;
    unsigned char y, x, *Surface, *pScrn, *NextPage;

    if( gMov16BppMode ) return MovProtUnk04(pSkip, pMap, pData, Xpos, Ypos, Width, Height);
    gMovMovieXpos = 8 * Xpos;
    gMovUnk02 = 8 * Width;
    gMovMovieYpos = (unsigned char)gMovScrSize * 8 * Ypos;
    gMovUnk20 = (unsigned char)gMovScrSize * 8 * Height;
    NextLine = gMovScrBufSizeA - 8 * Width;
    Surface = gMovScrPixelsA;
    if( Xpos || Ypos ) Surface = &gMovScrPixelsA[gMovMovieXpos + gMovSurfWidth * gMovMovieYpos];
    pScrn = Surface;
    pm = pMap;
    skip = pSkip;
    tmp = 0;
    // draw blocks from input stream
    for( page = gMovScrSize; page; page--, pScrn = NextPage + gMovSurfWidth ){
        NextPage = pScrn;        
        for( y = Height; y; y--, pScrn += NextLine ){
            for( x = Width; x; x-- ){
        	bit = MovGetBit10( &skip, &tmp );
                if( bit == 0 ){ // skip block -> block is unchanged
            	    pScrn += 8;            	    
                    continue;
                }
            	if( *pm++ != 0 ){ // skip block -> block from previous or current frame
            	    pScrn += 8;
            	    continue;
            	}
            	MovCopyBlock10( &pScrn, (unsigned char *)pData, gMovScrBufSize, 0 );
            	pData += 64;
            }            
        }        
    }
    // draw blocks by copying from frame
    pScrn = Surface;
    skip = pSkip;
    tmp = 0;
    offset = 0;
    for( page = gMovScrSize; page; page--, pScrn = NextPage + gMovSurfWidth ){
	NextPage = pScrn;    
	for( y = Height; y; y--, pScrn = pScrn + NextLine ){
    	    for( x = Width; x; x-- ){
        	bit = MovGetBit10( &skip, &tmp );
        	if( bit == 0 ){ // skip block -> block is unchanged
            	    pScrn += 8;            	    
            	    continue;
        	}
    		offset = *pMap++;
		if( offset == 0 ){ // skip block -> block from input stream
    		    pScrn += 8;
    		    continue;
    		}
    		if( offset & 0x8000 ){ // previous frame
		    MovCopyBlock10( &pScrn, &gMovScrPixelsB[ (pScrn - gMovScrPixelsA) + (offset - 0xc000) ], gMovScrBufSize, gMovScrBufSize );
		} else { // current frame
    		    MovCopyBlock10( &pScrn, (unsigned char *)&pCHR( pScrn )[ offset - 0x4000 ], gMovScrBufSize, gMovScrBufSize );
    		}
    	    }
	}	
    }
    memcpy( gMovScrPixelsB, gMovScrPixelsA, 64 * Width * Height ); // added
}

