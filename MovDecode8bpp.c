#define PINT( ptr )	((int *)(ptr))
#define PWRD( ptr )	((short *)(ptr))
#define UCHR( ptr )	((unsigned char *)(ptr))
#define SCHR( ptr )	((signed char *)(ptr))

unsigned char *MovDecodeNibbleFrameD( unsigned char *pdst, unsigned char op, unsigned char *pData, short Width, short Height );

void MovDecodeFrame8bpp( unsigned char *pMap, unsigned char *pData, short Xpos, short Ypos, short Width, short Height )
{
    int i;
    unsigned int NextLine;
    unsigned char *pdst, n;

    gMovMovieXpos = 8 * Xpos;
    gMovUnk02 = 8 * Width;
    gMovMovieYpos = gMovScrSize * 8 * Ypos;
    gMovUnk20 = gMovScrSize * 8 * Height;
    NextLine = gMovScrBufSizeA - 8 * Width;
    pdst = gMovScrPixelsA;
    if( Xpos || Ypos ) pdst = &gMovScrPixelsA[ gMovMovieXpos + gMovSurfWidth * gMovMovieYpos ];
    for( ;Height; Height-- ){
        for( i = (Width / 2); --i >= 0; ){
	    n = *pMap++;
	    pData = MovDecodeNibbleFrameD( pdst, n & 0x0f, pData, Width, Height );
	    pdst += 8;
	    n >>= 4;
	    pData = MovDecodeNibbleFrameD( pdst, n & 0x0f, pData, Width, Height );
	    pdst += 8;
        }
    	pdst += NextLine;	
    }
}

static void Mov8Pattern2x2_7( unsigned char *pdst, unsigned char *pData )
{
    int i;
    unsigned char r0, r1, b0, b1, *p0, *p1, *p2, *p3,*p4,*p5,*p6,*p7, r;
    
    r1 = UCHR( pData )[ 0 ];
    r0 = UCHR( pData )[ 1 ];
    b0 = UCHR( pData )[ 2 ]; 
    b1 = UCHR( pData )[ 3 ];
    p0 = pdst;
    p1 = p0 + gMovSurfWidth;
    p2 = p1 + gMovSurfWidth;
    p3 = p2 + gMovSurfWidth;
    p4 = p3 + gMovSurfWidth;
    p5 = p4 + gMovSurfWidth; 
    p6 = p5 + gMovSurfWidth; 
    p7 = p6 + gMovSurfWidth; 
    for( i = 0; i < 4; i++, b0 >>=1, b1 >>= 1 ){
	r = (b0 & 0x01) ? r0 : r1;
	*p0++ = r; *p0++ = r; *p1++ = r; *p1++ = r;
        r = (b0 & 0x10) ? r0 : r1;
	*p2++ = r; *p2++ = r; *p3++ = r; *p3++ = r;
	r = (b1 & 0x01) ? r0 : r1;
	*p4++ = r; *p4++ = r; *p5++ = r; *p5++ = r;
	r = (b1 & 0x10) ? r0 : r1;
	*p6++ = r; *p6++ = r; *p7++ = r; *p7++ = r;
    }
}

static void Mov8Pattern1x1_7( unsigned char *pdst, unsigned char *pData )
{
    int i, j;
    unsigned char *p, n;
    
    for( i = 0; i < 8; i++ ){
        n = UCHR( pData )[ 2 + i ];
        p = pdst;        	    
        for( j = 0; j < 8; j++, n >>= 1 ){
    	    *p++ = UCHR( pData )[ n & 0x01 ];
        }
        pdst += gMovSurfWidth;
    }

}

static void Mov8Pattern4q_8( unsigned char *pdst, unsigned char *pData )
{
    int i, j;
    unsigned char c, *p;

    for( i = 0; i < 2; i++ ){    
	p = pdst;
	for( j = 0; j < 2; j++ ){
	    c = pData[ 2 ]; 
	    p[ 0 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 1 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 2 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 3 ] = pData[ c & 0x01 ]; c >>= 1;
	    p += gMovSurfWidth;
	    p[ 0 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 1 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 2 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 3 ] = pData[ c & 0x01 ];
	    p += gMovSurfWidth;
	    c = pData[ 3 ]; 
	    p[ 0 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 1 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 2 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 3 ] = pData[ c & 0x01 ]; c >>= 1;
	    p += gMovSurfWidth;
	    p[ 0 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 1 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 2 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 3 ] = pData[ c & 0x01 ];
	    p += gMovSurfWidth;
	    pData += 4;
	}
	pdst += 4;
    }
}

static void Mov8PatternVert2_8( unsigned char *pdst, unsigned char *pData )
{
    int i, j;
    unsigned char c, *p;

    for( j = 0; j < 2; j++ ){
	p = pdst;
	for( i = 0; i < 4; i++ ){
	    c = pData[ 2 + i ]; 
	    p[ 0 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 1 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 2 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 3 ] = pData[ c & 0x01 ]; c >>= 1;	
	    p += gMovSurfWidth;
	    p[ 0 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 1 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 2 ] = pData[ c & 0x01 ]; c >>= 1;
	    p[ 3 ] = pData[ c & 0x01 ];
	    p += gMovSurfWidth;
	}
	pData += 6;
	pdst += 4;
    }
}

static void Mov8PatternHoriz2_8( unsigned char *pdst, unsigned char *pData )
{
    int i, j;
    unsigned char c0, c1, c2, c3, *p, *q, *r, *s;

    for( j = 0; j < 2; j++ ){
	c0 = UCHR( pData )[ 2 ]; 
	c1 = UCHR( pData )[ 3 ]; 
	c2 = UCHR( pData )[ 4 ];
	c3 = UCHR( pData )[ 5 ]; 
	p = pdst;
	q = p + gMovSurfWidth;
	r = q + gMovSurfWidth;
	s = r + gMovSurfWidth;
	pdst = s + gMovSurfWidth;
	for( i = 0; i < 8; i++, p++, q++, r++, s++ ){
    	    *p = UCHR( pData )[ c0 & 0x01 ]; c0 >>= 1;
    	    *q = UCHR( pData )[ c1 & 0x01 ]; c1 >>= 1;
    	    *r = UCHR( pData )[ c2 & 0x01 ]; c2 >>= 1; 
    	    *s = UCHR( pData )[ c3 & 0x01 ]; c3 >>= 1; 
	}
	pData += 6;
    }
}

static void Mov8Pattern4Pix1x1_9( unsigned char *pdst, unsigned char *pData )
{
    int i, j;
    short r;
    
    for( i = 0; i < 8; i++ ){
	r = PWRD( pData + 4 + i*2 )[ 0 ];
	for( j = 0; j < 8; j++ ){
	    pdst[ j ] = UCHR( pData )[ r & 0x03 ]; r >>= 2;
	}
	pdst += gMovSurfWidth;
    }
}

static void Mov8Pattern4Pix2x2_9( unsigned char *pdst, unsigned char *pData )
{
    int si, i, j;
    unsigned char *p;

    si = PINT( pData + 4 )[ 0 ];
    for( j = 0; j < 4; j++ ){
	p = pdst;
	for( i = 0; i < 4; i++, p += 2, si >>= 2 ){
	    p[0] = p[1] = UCHR( pData )[ si & 0x03 ];  
	    p[gMovSurfWidth] = p[gMovSurfWidth + 1] = p[0]; 
	}
	pdst += 2*gMovSurfWidth;
    }		    

}

static void Mov8Pattern4Pix2x1_9( unsigned char *pdst, unsigned char *pData )
{
    int si, i, j;
    unsigned char *p;

    si = PINT( pData + 4 )[ 0 ];
    for( j = 0; j < 4; j++ ){
	p = pdst;
	for( i = 0; i < 4; i++, p += 2, si >>= 2 ){
	    p[0] = p[1] = UCHR( pData )[ si  & 0x03 ];  
	}
	pdst += gMovSurfWidth;
    }		    
    si = PINT( pData + 4 )[ 1 ];
    for( j = 0; j < 4; j++ ){
	p = pdst;
	for( i = 0; i < 4; i++, p += 2, si >>= 2 ){
	    p[0] = p[1] = UCHR( pData )[ si & 0x03 ];  
	}
	pdst += gMovSurfWidth;
    }		    		    
}

static void Mov8Pattern4Pix1x2_9( unsigned char *pdst, unsigned char *pData )
{
    int si, i, j;
    unsigned char *p;
    
    si = PINT( pData + 4 )[ 0 ]; // pattern
    for( j = 0; j < 2; j++ ){
	p = pdst;
	for( i = 0; i < 8; i++, p++, si >>= 2 ){
	    p[0] = UCHR( pData )[ si & 0x03 ];  
	    p[gMovSurfWidth] = p[0]; 
	}
	pdst += 2*gMovSurfWidth;
    }		    
    si = PINT( pData + 4 )[ 1 ]; // pattern
    for( j = 0; j < 2; j++ ){
	p = pdst;
	for( i = 0; i < 8; i++, p++, si >>= 2 ){
	    p[0] = UCHR( pData )[ si & 0x03 ];  
	    p[gMovSurfWidth] = p[0]; 
	}
	pdst += 2*gMovSurfWidth;
    }		    
}

static void Mov8Pattern4Pix4q_10( unsigned char *pdst, unsigned char *pData )
{
    int i, j, lo, lt;
    unsigned char *p, r0, r1, r2, r3;

    lo = 4 * gMovSurfWidth;
    lt = lo + 4;
    for( i = 0; i < 4; i++ ){
        r0 = UCHR( pData )[ 4 + i ];  r1 = UCHR( pData )[ 12 + i ];
        r2 = UCHR( pData )[ 20 + i ]; r3 = UCHR( pData )[ 28 + i ];
        p = pdst;
        for( j = 0; j < 4; j++, r0 >>= 2, r1 >>= 2, r2 >>= 2, r3 >>= 2, p++ ){ 
    	    p[ 0  ] = UCHR( pData )[ r0 & 0x03 ];    p[ 4 ] = UCHR( pData + 16)[ r2 & 0x03 ];
	    p[ lo ] = UCHR( pData + 8)[ r1 & 0x03 ]; p[ lt ] = UCHR( pData + 24)[ r3 & 0x03 ];
        }
        pdst += gMovSurfWidth;
    }
}

static void Mov8Pattern4PixVert_10( unsigned char *pdst, unsigned char *pData )
{
    int i;
    unsigned char *p, r;

    p = pdst;
    for( i = 0; i < 8; i++ ){
	r = UCHR( pData )[ 4 + i ];
	p[ 0 ] = UCHR( pData )[ r & 0x03 ]; r >>= 2;
	p[ 1 ] = UCHR( pData )[ r & 0x03 ]; r >>= 2;
	p[ 2 ] = UCHR( pData )[ r & 0x03 ]; r >>= 2;
	p[ 3 ] = UCHR( pData )[ r & 0x03 ];
	p += gMovSurfWidth;
    }
    p = pdst + 4;
    for( i = 0; i < 8; i++ ){
	r = UCHR( pData )[ 16 + i ];
	p[ 0 ] = UCHR( pData + 12 )[ r & 0x03 ]; r >>= 2;
	p[ 1 ] = UCHR( pData + 12 )[ r & 0x03 ]; r >>= 2;
	p[ 2 ] = UCHR( pData + 12 )[ r & 0x03 ]; r >>= 2;
	p[ 3 ] = UCHR( pData + 12 )[ r & 0x03 ];
	p += gMovSurfWidth;
    }
}

static void Mov8Pattern4PixHoriz_10( unsigned char *pdst, unsigned char *pData )
{
    unsigned char *p, c0, c1;
    int i, j;

    p = pdst;    
    for( j = 0; j < 8; j += 2  ){
	c0 = pData[ 4 + j ];
	c1 = pData[ 5 + j ];
	for( i = 0; i < 4; i++, c0 >>= 2, c1 >>= 2 ){
    	    p[ i ] = pData[ c0 & 0x03 ]; 
    	    p[ i + 4 ] = pData[ c1 & 0x03 ]; 
	}
	p += gMovSurfWidth;
    }
    pData += 12;
    for( j = 0; j < 8; j += 2  ){
	c0 = pData[ 4 + j ];
	c1 = pData[ 5 + j ];
	for( i = 0; i < 4; i++, c0 >>= 2, c1 >>= 2 ){
    	    p[ i ] = pData[ c0 & 0x03 ];
    	    p[ i + 4 ] = pData[ c1 & 0x03 ];
	}
	p += gMovSurfWidth;
    }
}

static void Mov8CopyRaw1x1_11( unsigned char *pdst, unsigned char *pData )
{
    int i;

    for( i = 0; i < 16; i += 2, pdst += gMovSurfWidth ){
    	PINT( pdst )[ 0 ] = PINT( pData )[ i ]; PINT( pdst )[ 1 ] = PINT( pData )[ i + 1 ]; 
    }
}

static void Mov8CopyRaw2x2_12( unsigned char *pdst, unsigned char *pData )
{
    int a, b;

    UCHR( &a )[ 0 ] = pData[ 0 ]; UCHR( &a )[ 1 ] = pData[ 0 ];
    UCHR( &a )[ 2 ] = pData[ 1 ]; UCHR( &a )[ 3 ] = pData[ 1 ];
    UCHR( &b )[ 0 ] = pData[ 2 ]; UCHR( &b )[ 1 ] = pData[ 2 ];
    UCHR( &b )[ 2 ] = pData[ 3 ]; UCHR( &b )[ 3 ] = pData[ 3 ];
    PINT( pdst )[ 0 ] = a; PINT( pdst )[ 1 ] = b; pdst += gMovSurfWidth;
    PINT( pdst )[ 0 ] = a; PINT( pdst )[ 1 ] = b; pdst += gMovSurfWidth;
    UCHR( &a )[ 0 ] = pData[ 4 ]; UCHR( &a )[ 1 ] = pData[ 4 ];
    UCHR( &a )[ 2 ] = pData[ 5 ]; UCHR( &a )[ 3 ] = pData[ 5 ];
    UCHR( &b )[ 0 ] = pData[ 6 ]; UCHR( &b )[ 1 ] = pData[ 6 ];
    UCHR( &b )[ 2 ] = pData[ 7 ]; UCHR( &b )[ 3 ] = pData[ 7 ];
    PINT( pdst )[ 0 ] = a; PINT( pdst )[ 1 ] = b; pdst += gMovSurfWidth;
    PINT( pdst )[ 0 ] = a; PINT( pdst )[ 1 ] = b; pdst += gMovSurfWidth;
    UCHR( &a )[ 0 ] = pData[ 8 ]; UCHR( &a )[ 1 ] = pData[ 8 ];
    UCHR( &a )[ 2 ] = pData[ 9 ]; UCHR( &a )[ 3 ] = pData[ 9 ];
    UCHR( &b )[ 0 ] = pData[ 10 ]; UCHR( &b )[ 1 ] = pData[ 10 ];
    UCHR( &b )[ 2 ] = pData[ 11 ]; UCHR( &b )[ 3 ] = pData[ 11 ];
    PINT( pdst )[ 0 ] = a; PINT( pdst )[ 1 ] = b; pdst += gMovSurfWidth;
    PINT( pdst )[ 0 ] = a; PINT( pdst )[ 1 ] = b; pdst += gMovSurfWidth;
    UCHR( &a )[ 0 ] = pData[ 12 ]; UCHR( &a )[ 1 ] = pData[ 12 ];
    UCHR( &a )[ 2 ] = pData[ 13 ]; UCHR( &a )[ 3 ] = pData[ 13 ];
    UCHR( &b )[ 0 ] = pData[ 14 ]; UCHR( &b )[ 1 ] = pData[ 14 ];
    UCHR( &b )[ 2 ] = pData[ 15 ]; UCHR( &b )[ 3 ] = pData[ 15 ];
    PINT( pdst )[ 0 ] = a; PINT( pdst )[ 1 ] = b; pdst += gMovSurfWidth;
    PINT( pdst )[ 0 ] = a; PINT( pdst )[ 1 ] = b; pdst += gMovSurfWidth;
}

static void Mov8CopyRaw4x4_13( unsigned char *pdst, unsigned char *pData )
{
    int a, b, i;

    UCHR( &a )[ 0 ] = pData[ 0 ]; UCHR( &a )[ 1 ] = pData[ 0 ]; UCHR( &a )[ 2 ] = pData[ 0 ]; UCHR( &a )[ 3 ] = pData[ 0 ];
    UCHR( &b )[ 0 ] = pData[ 1 ]; UCHR( &b )[ 1 ] = pData[ 1 ]; UCHR( &b )[ 2 ] = pData[ 1 ]; UCHR( &b )[ 3 ] = pData[ 1 ];
    for( i = 0; i < 4; i++, pdst += gMovSurfWidth ){
	PINT( pdst )[ 0 ] = a; PINT( pdst )[ 1 ] = b; 
    }

    UCHR( &a )[ 0 ] = pData[ 2 ]; UCHR( &a )[ 1 ] = pData[ 2 ]; UCHR( &a )[ 2 ] = pData[ 2 ]; UCHR( &a )[ 3 ] = pData[ 2 ];
    UCHR( &b )[ 0 ] = pData[ 3 ]; UCHR( &b )[ 1 ] = pData[ 3 ]; UCHR( &b )[ 2 ] = pData[ 3 ]; UCHR( &b )[ 3 ] = pData[ 3 ];
    for( i = 0; i < 4; i++, pdst += gMovSurfWidth ){
	PINT( pdst )[ 0 ] = a; PINT( pdst )[ 1 ] = b; 
    }    
}

static void Mov8Solid_14( unsigned char *pdst, unsigned char *pData )
{
    int i;

    for( i = 0; i < 8; i++, pdst += gMovSurfWidth ) memset( pdst, pData[ 0 ], 8 );    
}

static void Mov8Dith_15( unsigned char *pdst, unsigned char *pData )
{
    int tmp, i, n;

    UCHR( &tmp )[ 0 ] = pData[ 1 ];
    UCHR( &tmp )[ 1 ] = pData[ 0 ];
    PWRD( &tmp )[ 1 ] = PWRD( &tmp )[ 0 ];
    UCHR( &n )[ 0 ] = pData[ 0 ];
    UCHR( &n )[ 1 ] = pData[ 1 ];
    PWRD( &n )[ 1 ] = PWRD( &n )[ 0 ];
    for( i = 0; i < 4; i++ ){
	PINT( pdst )[ 0 ] = tmp; PINT( pdst )[ 1 ] = tmp; pdst += gMovSurfWidth;
	PINT( pdst )[ 0 ] = n; PINT( pdst )[ 1 ] = n; pdst += gMovSurfWidth;	
    }
}

static void Mov8CopyFrame( unsigned char *pdst, unsigned char *psrc )
{
    int i;
    for( i = 0; i < 8; i++, pdst += gMovSurfWidth, psrc += gMovSurfWidth ){
    	((int *)pdst)[ 0 ] = ((int *)psrc)[ 0 ]; ((int *)pdst)[ 1 ] = ((int *)psrc)[ 1 ]; 
    }	    
}

unsigned char *MovDecodeNibbleFrameD( unsigned char *pdst, unsigned char op, unsigned char *pData, short Width, short Height )
{
    MveXYofs_t rel;

    switch( op ){
        case 0: // block is copied from current frame
            Mov8CopyFrame( pdst, gMovScrPixelsB + ( pdst - gMovScrPixelsA ) );
            break;
        case 1: // block is unmodified
            break;
        case 2: // block is copied from nearby 1
            rel = gMovEncXYoffst[ pData[ 0 ] ];
	    Mov8CopyFrame( pdst, pdst + gMovEncPosY[ (int)((unsigned char)rel.y) ] + rel.x );
    	    pData++;
            break;
        case 3: // block is copied from nearby 2
            rel = gMovEncXYoffst[ pData[ 0 ] ];
	    Mov8CopyFrame( pdst, pdst + gMovEncPosY[ (int)((unsigned char)(-rel.y)) ] - rel.x );
    	    pData++;
            break;
        case 4: // block is copied from nearby 3
            rel = gMovEncXYOffstCur[ pData[ 0 ] ];
	    Mov8CopyFrame( pdst, gMovScrPixelsB + (pdst - gMovScrPixelsA) + gMovEncPosY[ (int)((unsigned char)rel.y) ] + rel.x );
	    pData++;
            break;
        case 5: // block is copied from nearby 4
            rel = *(MveXYofs_t *)pData;
	    Mov8CopyFrame( pdst, gMovScrPixelsB + ( pdst - gMovScrPixelsA ) + gMovEncPosY[ (int)((unsigned char)rel.y) ] + rel.x );
	    pData += 2;
            break;
        case 6: break; // not used
        case 7:
            if( UCHR( pData )[ 0 ] > UCHR( pData )[ 1 ] ){ // P0 > P1
        	Mov8Pattern2x2_7( pdst, pData );
		pData += 4;
            } else { // P0 <= P1
        	Mov8Pattern1x1_7( pdst, pData );
		pData += 10;
            }
            break;
        case 8: // 
            if( UCHR( pData )[ 0 ] > UCHR( pData )[ 1 ] ){ // P0 > P1
                if( UCHR( pData )[ 6 ] > UCHR( pData )[ 7 ] ){ // P2 > P3
            	    Mov8PatternHoriz2_8( pdst, pData );
                } else { // P2 <= P3
            	    Mov8PatternVert2_8( pdst, pData );
		}
		pData += 12;
            } else { // P0 <= P1
        	Mov8Pattern4q_8( pdst, pData );
		pData += 16;
            }
            break;
        case 9:
            if( UCHR( pData )[ 0 ] > UCHR( pData )[ 1 ] ){ // P0 > P1
                if( UCHR( pData )[ 2 ]> UCHR( pData )[ 3 ] ){ // P0 > P1 && P2 > P3
            	    Mov8Pattern4Pix1x2_9( pdst, pData );
                } else { // P0 > P1 && P2 <= P3
            	    Mov8Pattern4Pix2x1_9( pdst, pData );
                }
		pData += 12;
            } else { // P0 <= P1
                if( UCHR( pData )[ 2 ] > UCHR( pData )[ 3 ] ){ // P0 <= P1 && P2 > P3
            	    Mov8Pattern4Pix2x2_9( pdst, pData );
		    pData += 8;
                } else { // P0 <= P1 && P2 <= P3
        	    Mov8Pattern4Pix1x1_9( pdst, pData );
		    pData += 20;
                }
            }
            break;
        case 0xA:
            if( UCHR( pData )[ 0 ] > UCHR( pData )[ 1 ] ){ // P0 > P1
                if( UCHR( pData )[ 12 ] > UCHR( pData )[ 13 ] ){ // P0 > P1 && P4 > P5 
            	    Mov8Pattern4PixHoriz_10( pdst, pData );
                } else { // P0 > P1 && P4 <= P5 
            	    Mov8Pattern4PixVert_10( pdst, pData );
                }
		pData += 24;
            } else { // P0 <= P1
		Mov8Pattern4Pix4q_10( pdst, pData );
		pData += 32;
            }
            break;
        case 0xB: // raw pixel data 
    	    Mov8CopyRaw1x1_11( pdst, pData );
            pData += 64;
            break;
        case 0xC: // raw pixel data 2x2    
    	    Mov8CopyRaw2x2_12( pdst, pData );
	    pData += 16;
            break;
        case 0xD: // raw pixel data 4x4 
	    Mov8CopyRaw4x4_13( pdst, pData );
	    pData += 4;
            break;
        case 0xE: // solid block
	    Mov8Solid_14( pdst, pData );
	    pData++;
	    break;
        case 0xF: // dithered block
    	    Mov8Dith_15( pdst, pData );
	    pData +=2;
            break;
    }
    return pData;
}

