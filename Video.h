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

//SDL_Surface *gSDLSurfaceCur = NULL;
//SDL_Surface *gSDLSurfaceBckp = NULL;
//SDL_Palette *gSDLPalette = NULL;
//SDL_Window *gSDLWindow = NULL;
//SDL_Window *gWindow = NULL;

extern VidRect_t gVidMainGeo;

extern void (*gVidColorCb)();
extern int gUseMMX;

extern int gVidScaleX;
extern int gVidScaleY;

// 2 zm
//extern short *gpVidPalx;		// pointer to 16bit palette
//extern ABGR_t gVidPalx[ 256 ];		// 16 bit palette
// color mask in 16 bit word
extern int gVidMaskGx;
extern int gVidMaskRx;
extern int gVidMaskBx;
// shift bit of 16 bit colors +/- -> position in 16bit word
extern int gVidColShiftBx;
extern int gVidColShiftRx;
extern int gVidColShiftGx;
extern void(*gVidCopyA)( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int posX, int posY );
extern void(*gVidCls)();
extern int gMMXon;

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
//void VidUnkSub1( int a1, int a2 );
//void VidNullSub2();
//void VidNullSub1();
void VidCls();
void VidSetColorCb( void (*Cb)(void) );
void (*VidGetColorCb())(void);
int VidInit( int Width, int Height, int Bpp );
int VidInit8bpp( int ScreenWidth, int ScreenHeight );
int VidCreateMainWindow();
//void VidGetShiftBits( int ColorMask);
int VidInitMode( int Width, int Height, int Bpp );
void VidClose();
void VidSetPaletteIndex( int ColorIndex, char Rr, char Gc, char Bb );
void VidSetPaletteRange( Pal8_t *Pal, unsigned int FirstColor, unsigned int nColors );
int VidGetPaletteColor( int ColorIndex, char *pR, char *pG, char *pB );
void VidSetPaletteAll( Pal8_t *Pal );
Pal8_t *VidCreateDimmedPalette();
void VidCopy8( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int DstX, int DstY );
void VidCopy16( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int DstX, int DstY );
void VidCopy16A( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int DstX, int DstY );
void VidCopyAlpha16( char *pSrc, int SrcPich, int Unused, int SrcX, int SrcY, int Width, short Height, int posX, int posY, short Alpha );
void VidClearScreen();
void VidUpdateOpt();
void VidUpdateClr();
void VidCopy( void *Buffer, Pal8_t *, int pitch, int x, int y, int w, int h, int bpp );
