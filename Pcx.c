#include "FrameWork.h"

int gPcxUnk02 = 0;
int gPcxUnk01 = 0;

/*******************************************************/

void PcxReadHdr( PcxHdr_t *hdr, xFile_t *fh )
{
    hdr->Magic = dbgetc( fh );
    hdr->Ver = dbgetc( fh );
    hdr->Enc = dbgetc( fh );
    hdr->Bpp = dbgetc( fh );
    dbread( &hdr->Xstart, 2, 1, fh );
    dbread( &hdr->Ystart, 2, 1, fh );
    dbread( &hdr->Xend, 2, 1, fh );
    dbread( &hdr->Yend, 2, 1, fh );
    dbread( &hdr->Hres, 2, 1, fh );
    dbread( &hdr->Vres, 2, 1, fh );
    dbread( hdr->Palette, 32, 1, fh );
    hdr->Rsvd1 = dbgetc( fh );
    hdr->NumBitPlanes = dbgetc( fh );
    dbread( &hdr->BytesPerLine, 2, 1, fh );
    dbread( &hdr->PaletteType, 2, 1, fh );
    dbread( &hdr->HScrSize, 2, 1, fh );
    dbread( &hdr->VScrSize, 2, 1, fh );
    dbread( hdr->Rsv2, 54, 1, fh );
}

int PcxUnk01( char *a1, int size, xFile_t *fh )
{
    unsigned char v3;
    char v4,*v8,v9;
    int v7, v11;

    v3 = gPcxUnk02;
    v4 = gPcxUnk01;
    v7 = 0;
    v11 = 0;
    while( v7 < size ){
        v11 += v3;
        v8 = &a1[ v7 ];
        while( v3 ){
            if( v7 >= size ) break;
            *v8++ = v4;
            v3--;
            v7++;
        }
        gPcxUnk02 = v3;
        gPcxUnk01 = v4;
        if( v3 ){ v11 -= v3; break; }
        v9 = dbgetc( fh );
        if( (v9 & 0xC0) == 0xC0 ){
            gPcxUnk02 = v9 & 0x3F;
            v9 = dbgetc( fh );
            v3 = gPcxUnk02;
        } else {
            v3 = 1;
        }
        v4 = v9;
    }
    gPcxUnk01 = v4;
    gPcxUnk02 = v3;
    return 0;
}

int PcxReadPalette( PcxHdr_t *hdr, Pal8_t *Pal, xFile_t *fh )
{
    int fpos, flen, i;

    if( hdr->Ver != 5 ) return 0; // not 3.0 24bit image
    fpos = dbtell( fh );
    flen = dbFileLength( fh );
    dbseek( fh, flen - 769, SEEK_SET );
    if( dbgetc( fh ) != 12 ){ 
	dbseek( fh, fpos, SEEK_SET ); 
	return 0; 
    }
    for( i = 0; i < 256; i++ ){
        Pal[ i ].r = dbgetc( fh );
        Pal[ i ].g = dbgetc( fh );
        Pal[ i ].b = dbgetc( fh );
    }
    dbseek( fh, fpos, SEEK_SET );
    return 1;
}

char *PcxLoad( char *fname, int *pW, int *pH, Pal8_t *Pal )
{
    PcxHdr_t hdr;
    xFile_t *fh;
    char *p, *buf;
    int size,i,h,w;
    
    if( !(fh = dbOpen( fname, "rb" ) ) ) return NULL;    
    PcxReadHdr( &hdr, fh );
    w = hdr.Xend - hdr.Xstart + 1;
    h = hdr.Yend - hdr.Ystart + 1;
    *pW = w;
    *pH = h;
    size = hdr.NumBitPlanes * hdr.BytesPerLine;
    if( !(buf = dbg_malloc( size * h )) ){ dbClose( fh ); return NULL; }
    p = buf;    
    gPcxUnk02 = 0;
    gPcxUnk01 = 0;
    for( i = 0; i < h; p += w, i++ ) PcxUnk01( p, size, fh );
    PcxReadPalette( &hdr, Pal, fh );
    dbClose( fh );
    return buf;
}

