#pragma once

typedef struct // size of 128
{
    unsigned char 	Magic;
    unsigned char 	Ver;
    unsigned char 	Enc;
    unsigned char 	Bpp;
    unsigned short 	Xstart;
    unsigned short 	Ystart;
    unsigned short 	Xend;
    unsigned short 	Yend;
    unsigned short 	Hres;
    unsigned short 	Vres;
    unsigned char 	Palette[ 48 ];
    unsigned char 	Rsvd1;
    unsigned char 	NumBitPlanes;
    unsigned short	BytesPerLine;
    unsigned short 	PaletteType;
    unsigned short 	HScrSize;
    unsigned short 	VScrSize;
    unsigned char 	Rsv2[ 54 ];
} PcxHdr_t;

void PcxReadHdr( PcxHdr_t *hdr, xFile_t *fh );
int PcxUnk01( char *a1, int size, xFile_t *fh );
int PcxReadPalette( PcxHdr_t *hdr, Pal8_t *Pal, xFile_t *fh );
char *PcxLoad( char *fname, int *pW, int *pH, Pal8_t *Pal );

