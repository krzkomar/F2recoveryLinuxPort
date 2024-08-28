#include "FrameWork.h"
#include <SDL.h>

// substitute: original used MMX instructions, diffrent src file
void ScrCopyFast( char *Dst, int DstPitch, char *Src, int SrcPitch, short Width, short Height )
{
    int i;

    for( i = 0; i < Height; i++ ){
	memcpy( Dst, Src, Width );
        Src += SrcPitch;
        Dst += DstPitch;
    }
}

/*
void *mmx_memcpy( void *to, const void *from, size_t len )
{
    __asm__ __volatile__ (
        "  movq (%0), %%mm0\n"
        "  movq 8(%0), %%mm1\n"
        "  movq 16(%0), %%mm2\n"
        "  movq 24(%0), %%mm3\n"
        "  movq %%mm0, (%1)\n"
        "  movq %%mm1, 8(%1)\n"
        "  movq %%mm2, 16(%1)\n"
        "  movq %%mm3, 24(%1)\n"
        "  movq 32(%0), %%mm0\n"
        "  movq 40(%0), %%mm1\n"
        "  movq 48(%0), %%mm2\n"
        "  movq 56(%0), %%mm3\n"
        "  movq %%mm0, 32(%1)\n"
        "  movq %%mm1, 40(%1)\n"
        "  movq %%mm2, 48(%1)\n"
        "  movq %%mm3, 56(%1)\n"
    : : "r" (from), "r" (to) : "memory");
}
*/

// substitute: original used MMX instructions, diffrent src file
void ScrCopyAlphaFast( unsigned char *pDst, int DstPitch, unsigned char *pSrc, int SrcPitch, int Width, int Height )
{
    int i,j,k,l;
    unsigned char *pd, *ps;
    unsigned char col;

    k = Width / 8;
    l = Width % 8;    
    for( i = 0; i < Height; i++ ){
	pd = pDst;
	ps = pSrc;
	for( j = 0; j < k*8; j++, pd++, ps++ ){
	    col = *ps;
	    if( col == 0 ) continue;
	    *pd = col;	    
//	    __asm__ __volatile__ (
//	    " movq	(%%rsi), %%mm0\n"
//	    );
	}
	for( j = 0; j < l; j++, pd++, ps++ ){
	    col = *ps;
	    if( col == 0 ) continue;
	    *pd = col;
	}
        pSrc += SrcPitch;
        pDst += DstPitch;
    }
/*					// mm3 = Alpha
cpy:					// do{
    movq    mm0, qword ptr [esi]	// 	mm0 = *esi.q // src
    movq    mm1, qword ptr [edi]	// 	mm1 = *edi.q // dst
    movq    mm2, mm0			// 	mm2 = mm0
    pcmpeqb mm2, mm3			// 	mm2 = pcmeqb( mm0, mm3 ) // jesli wystapilo gdzies Alpha to mm2[ 0..7 ] == 0xff
    lea     esi, [esi+8]		// 	esi += 8;
    movq    mm4, mm2			// 	mm4 = mm2
    pandn   mm2, mm0			// 	mm2 = ~mm2 & mm0
    pand    mm4, mm1			// 	mm4 = mm4 & mm1
    lea     edi, [edi+8]		// 	edi += 8
    por     mm2, mm4			//	mm2 = mm2 | mm4
    dec     ecx				// 	ecx--
    movq    qword ptr [edi-8], mm2	// 	[edi - 8 ] = mm2
    jns     short cpy			// }while( NS )
*/
}


void ScrLine( char *Surface, int Pitch, int x0, int y0, int x1, int y1, int Color )
{
    int y; int rt; int w_cent; int h_cent; int dpitch; int cc; int j; int i; int dx; int dy;
    char *pStart; char *pStop; char *pr; char *pl;

    y = y0;
    rt = x1;
    if( x0 == x1 ){ // vertical line
        if( y0 > y1 ){ y = y1; y1 = y0; } // swap y's
        pStart = Surface + Pitch * y + x0;
        pStop = Surface + Pitch * y1 + x1;
        for( ;pStop >= pStart; pStart += Pitch, pStop -= Pitch ) *pStop = *pStart = Color;        
	return;
    } 
    if( x0 > x1 ){ x1 = x0; x0 = rt; y = y1; y1 = y0; }
    pStart = Surface +x0 + Pitch * y;
    pStop = Surface + x1 + Pitch * y1;
    if( y == y1 ){ // horizontal line
        memset( pStart, Color, pStop - pStart ); 
        return;
    } 
    // skewed line    
    dx = x1 - x0;  
    w_cent = x0 + ((x1 - x0) / 2); 
    if( y <= y1 ){
        dy = y1 - y; 
        dpitch = Pitch; 
        h_cent = w_cent + (((y1 - y) >> 1) + y) * Pitch;
    } else {
        dy = y - y1; 
        dpitch = -Pitch;
        h_cent = w_cent + (y - ((y - y1) >> 1)) * Pitch;  
    }
    pl = pr = Surface + h_cent; 
    if( dx > dy ) {
        cc = dy - (dx >> 1);
        for( j = dx >> 2; j; j--, cc += dy ){
            *pStart = *pStop = *pr = *pl = Color;
            if( cc >= 0 ){
                pr += dpitch; pStop -= dpitch; pl -= dpitch; pStart += dpitch; 
                cc -= dx;
            }
            pr++; pStop--; pl--; pStart++;
        }
    } else {
        cc = dx - (dy >> 1);
        for( i = dy >> 2; i; i--, cc += dx ){
            *pStart = *pStop = *pr = *pl = Color;
            if( cc >= 0 ){
                pr++; pStop--; pl--; pStart++;
                cc -= dy;
            }
            pr += dpitch; pStop -= dpitch; pl -= dpitch; pStart += dpitch;            
        }
    }
}

void ScrRectangle( char *Surface, int Pitch, int x0, int y0, int x1, int y1, int Color )
{
    ScrLine(Surface, Pitch, x0, y0, x1, y0, Color);
    ScrLine(Surface, Pitch, x0, y1, x1, y1, Color);
    ScrLine(Surface, Pitch, x0, y0, x0, y1, Color);
    ScrLine(Surface, Pitch, x1, y0, x1, y1, Color);
}

void ScrShadowRectangle( char *Surface, int Pitch, int x0, int y0, int x1, int y1, int ColorUp, int ColorDn )
{
    ScrLine(Surface, Pitch, x0, y0, x1, y0, ColorUp);
    ScrLine(Surface, Pitch, x0, y1, x1, y1, ColorDn);
    ScrLine(Surface, Pitch, x0, y0, x0, y1, ColorUp);
    ScrLine(Surface, Pitch, x1, y0, x1, y1, ColorDn);
}

void ScrScaleImg( char *Src, int SrcWidth, int SrcHeight, int SrcPitch, char *Dst, int DstWidth, int DstHeight, int DstPitch )
{
    int v8, ratioY, n, yr1, y, ratioX, ScaledY, x, t, w, kk, j, xr1, xr;
    char e, *s, *p;

    ratioY = (DstHeight << 16) / SrcHeight;
    ratioX = (DstWidth << 16) / SrcWidth;    
    yr1 = (DstHeight << 16) / SrcHeight;
    for( y = n = 0; y < SrcHeight; y++, n += SrcPitch, yr1 += ratioY ){
        ScaledY = (ratioY * y) >> 16;
        t = yr1 >> 16;
        xr1 = ratioX;
        p = Src + n;
        for( xr = x = 0; x < SrcWidth; xr += ratioX, xr1 += ratioX, p++, x++ ){
            v8 = (ratioY * y) >> 16;
            w = xr1 >> 16;
            kk = xr >> 16;
            e = *p;
            if( ScaledY >= t ) continue;
            s = Dst + DstPitch * ScaledY;
            do{
                j = xr >> 16;
                s += kk;
                if( kk < w ){
                    do{
                        j++;
                        *s++ = e;
                    }while ( j < w );
                }
                v8++;
                s += DstPitch;
            }while ( v8 < t );
        }            
    }
}

void ScrScaleImgAlpha( char *Src, int SrcWidth, int SrcHeight, int SrcPitch, char *Dst, int DstWidth, int DstHeight, int DstPitch )
{
    unsigned int xr1, xr, yr1;
    int kk, i, j, ScaledY, ratioX, ratioY, n, w, x, t, y;
    char *p, e, *s;

    ratioY = (DstHeight << 16) / SrcHeight;	// ratioY = h1/h2 * 65536
    ratioX = (DstWidth << 16) / SrcWidth;	// ratioX = w1/w2 * 65536
    yr1 = ratioY;
    for( n = y = 0; y < SrcHeight; y++, n += SrcPitch, yr1 += ratioY ){
        ScaledY = (ratioY * y) >> 16;	// ScaledY = h1/h2 * y
        t = yr1 >> 16; 		// t = int( h1 / h2 )
        xr1 = ratioX;
        p = Src + n;
        for( xr = x = 0; x < SrcWidth; x++, xr += ratioX, xr1 += ratioX, p++ ){
            w = xr1 >> 16;
	    kk = xr >> 16;
            if( (e = *p) == 0 ) continue; // color alpha
            if( ScaledY >= t ) continue;
            i = (ratioY * y) >> 16;
            s = Dst + DstPitch * ScaledY;
            do{
                j = xr >> 16;
                s += kk;
                if( kk < w ){
                    do{
                        j++;
                        *s++ = e;
                    }while( j < w );
                }
                i++;
                s += DstPitch;
            }while ( i < t );            
        }        
    }
}

void ScrCopy( void *pSrc, int Width, int Height, int SrcPitch, void *pDst, int DestPitch )
{
    ScrCopyFast( pDst, DestPitch, pSrc, SrcPitch, Width, Height );
}

void ScrCopyAlpha( void *pSrc, int Width, int Height, int Spitch, void *pDst, int Dpitch )
{
    ScrCopyAlphaFast( pDst, Dpitch, pSrc, Spitch, Width, Height );
}

void ScrFillSolid( char *pDst, int Width, int Height, int Pitch, int Color )
{
    int i;

    for( i = 0; i < Height; i++, pDst += Pitch ){
        memset( pDst, Color, Width );        
    }
}

void ScrFillImg( char *Surface, int Width, int Height, int Pitch, Image_t *Img, int Xpos, int Ypos )
{
    int xx, i, yy, cnt, k, j;
    char *src;

    xx = Xpos % (Img->Width >> 16);
    yy = Ypos % (Img->Height >> 16);
    src = Img->Data + yy * (Img->Width >> 16);

    for( i = 0; i < Height; i++ ){        
	k = xx;    
        for( j = 0; j < Width; j += cnt ){
            cnt = (Img->Width >> 16) - k;
            if( cnt > Width - j ) cnt = Width - j;
            memcpy( Surface + j, src + k, cnt );
	    k += cnt;
            if( k == Img->Width >> 16 ) k ^= Img->Width >> 16;
        }        
        yy++;
        Surface += Pitch;
        if( Img->Height >> 16 == yy ){
            src = Img->Data;
            yy = 0;
        } else {
            src += Img->Width >> 16;
        }
    }
}

void ScrEnlight( char *pDst, unsigned int Width, int Height, unsigned int Pitch )
{
    unsigned int i,j,width;

    width = Pitch - Width;
    for( i = 0; i < Height; i++, pDst += width ){
        for( j = 0; j < Width; pDst++, j++ ){
            *pDst = gPalShades[ (int)*pDst ][ ENLIGHT_COLOR ];
        }
    }
}

void ScrColorXchg( char *Scr, int Width, int Height, int Pitch, char ColorA, char ColorB )
{
    int i,j,n;
    
    n = Pitch - Width;
    for( i = 0; i < Height; i++, Scr += n ){
        for( j = 0; j < Width; j++, Scr++ ){
            if( *Scr == ColorA ){
                *Scr = ColorB;
            } else if( *Scr == ColorB ){
                *Scr = ColorA;
            }            
        }
    }
}

void ScrEdgeHighlight( char *Scr, int Width, int Height, unsigned int Pitch, char Color )
{
    unsigned int j, i, h;
    char Toggle,*s;

    s = Scr + Pitch;
    h = Height - 2;
    for( i = 0; i < h; s += Pitch - Width, i++ ){
        Toggle = 1;
        for( j = 0; j < Width; j++, s++ ){
            if( *s && Toggle ){
                *(s - 1) = Color;
                Toggle = 0;
            } else if( (*s == 0) && !Toggle ){
                *s = Color;
                Toggle = 1;
            }                
        }            
    }
    for( i = 0; i < Width; i++ ){
        Toggle = 1;
        s = Scr + i;
        for( j = 0; j < Height; j++, s += Pitch ){
            if( *s && Toggle ){
//                s[ -Pitch ] = Color;
                Toggle = 0;
            } else if( (*s == 0) && !Toggle ){
                *s = Color;
                Toggle = 1;
            }
        }
    }
}

