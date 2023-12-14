#pragma once

#define ENLIGHT_COLOR 147

typedef struct
{
    short Width;
    short Height;
    char Data[];
} Image_t;

typedef union
{
    struct{
	char a;
	char b;
	char g;
	char r;
    };
    int	argb;
} ABGR_t;


// substitute: original used MMX instructions, diffrent src file
void ScrCopyFast( char *Dst, int DstPitch, char *Src, int SrcPitch, short Width, short Height );
void ScrCopyAlphaFast( unsigned char *pDst, int DstPitch, unsigned char *pSrc, int SrcPitch, int Width, int Height );

void ScrLine( char *Surface, int Pitch, int x0, int y0, int x1, int y1, int Color );
void ScrRectangle( char *Surface, int Pitch, int x0, int y0, int x1, int y1, int Color );
void ScrShadowRectangle( char *Surface, int Pitch, int x0, int y0, int x1, int y1, int ColorUp, int ColorDn );
void ScrScaleImg( char *Src, int SrcWidth, int SrcHeight, int SrcPitch, char *Dst, int DstWidth, int DstHeight, int DstPitch );
void ScrScaleImgAlpha( char *Src, int SrcWidth, int SrcHeight, int SrcPitch, char *Dst, int DstWidth, int DstHeight, int DstPitch );
void ScrCopy( void *pSrc, int Width, int Height, int SrcPitch, void *pDst, int DestPitch );
void ScrCopyAlpha( void *pSrc, int Width, int Height, int Spitch, void *pDst, int Dpitch );
void ScrFillSolid( char *pDst, int Width, int Height, int Pitch, int Color );
void ScrFillImg( char *Surface, int Width, int Height, int Pitch, Image_t *Img, int Xpos, int Ypos );
void ScrEnlight( char *pDst, unsigned int Width, int Height, unsigned int Pitch );
void ScrColorXchg( char *Scr, int Width, int Height, int Pitch, char ColorA, char ColorB );
void ScrEdgeHighlight( char *Scr, int Width, int Height, unsigned int Pitch, char Color );

