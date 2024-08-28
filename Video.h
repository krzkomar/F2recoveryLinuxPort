#pragma once

typedef struct
{
    int lt; // left
    int tp; // top
    int rt; // right
    int bm; // bottom
} VidRect_t;

extern int gVidUpdateForbid;

#define VidW	( gVidMainGeo.rt - gVidMainGeo.lt + 1 )
#define VidH	( gVidMainGeo.bm - gVidMainGeo.tp + 1 )

#define COLOR_SCALE	4
#define WIN_TITLE	"F2"

extern VidRect_t gVidMainGeo;
extern void (*gVidColorCb)();
extern int gVidScaleX;
extern int gVidScaleY;
//extern void(*gVidCopyA)( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int posX, int posY );
extern void(*gVidCls)();

void VidSetColorCb( void (*Cb)(void) );
void (*VidGetColorCb())(void);

void VidSetMMX( int AllowMMX );
int VidInit320x200x8bpp();
int VidInit320x240x8bpp();
int VidInit320x400x8bpp();
int VidInitDummy();
int VidInit640x480x8bpp();
int VidInit640x480x16bpp();
int VidInit640x400x8bpp();
int VidInit800x600x8bpp();
int VidInit1024x768x8bpp();
int VidInit1280x1024x8bpp();
void VidCls();
int VidInit( int Width, int Height, int Bpp );
int VidInit8bpp( int ScreenWidth, int ScreenHeight );
int VidCreateMainWindow();
int VidInitMode( int Width, int Height, int Bpp );
void VidClose();
void VidSetPaletteIndex( int ColorIndex, char Rr, char Gc, char Bb );
void VidSetPaletteRange( Pal8_t *Pal, unsigned int FirstColor, unsigned int nColors );
int VidGetPaletteColor( int ColorIndex, char *pR, char *pG, char *pB );
void VidSetPaletteAll( Pal8_t *Pal );
Pal8_t *VidCreateDimmedPalette();
void VidCopy( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int DstX, int DstY );
void VidCopyAlpha( char *pSrc, int SrcPich, int Unused, int SrcX, int SrcY, int Width, short Height, int posX, int posY, int Alpha );
void VidClearScreen();
void VidUpdateOpt();
void VidUpdateClr();
void VidUpdate( );
