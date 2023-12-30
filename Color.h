#pragma once

#define TAG_NEWC 		0x4E455743 // 'NEWC'
#define PAL_COLOR_STACK_DEEP 	16
#define COLOR_MASK	0x00ff

#define RGB16_GET_R( col16 ) (((col16) & 0x7C00) >> 10)
#define RGB16_GET_G( col16 ) (((col16) & 0x3E0) >> 5)
#define RGB16_GET_B( col16 ) ((col16) & 0x1F)
#define RGB16_SET( r, g, b ) ( ((r) << 10) | ((g) << 5) | (b) )

// Colors
#define COLOR_ALPHA	gPalColorCubeRGB[0][0][0]
#define COLOR_BLACK	gPalColorCubeRGB[0][0][0]

#define COLOR( r, g, b ) gPalColorCubeRGB[ r ][ g ][ b ]

typedef struct // size of 3
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} __attribute__((packed)) Pal8_t;

extern unsigned char	gPalSemiTransparent[256][256];
extern unsigned char	gPalExt1[256][256];
extern unsigned char	gPalShades[256][256];
extern unsigned char	gPalColorCubeRGB[32][32][32];
extern Pal8_t gPalBase[ 256 ];
extern Pal8_t gPalCurrent[256];

xFile_t *PalOpenFile( const char *path, int mode );
int PalReadFile( xFile_t *fh, void *ptr, int size );
void PalCloseFile( xFile_t *fh );
void PalSetFileManage( xFile_t *(*Open)(const char *, int), int (*Read_)(xFile_t *, void *, int), void (*Close)(xFile_t *) );
void *PalAlloc( size_t nsize );
void *PalRealloc( void *ptr, size_t size );
void PalFree( void *ptr );
void PalSetPathCb( char *(*Cb)() );
char PalExt0GetColor( unsigned char Color, unsigned char Shade );
char PalExt1GetColor( unsigned char Color, unsigned char Shade );
char PalGetColorShade( int Shade, unsigned char Color );
char PalGetColorMap( int Color );
int  PalGetColor16( unsigned char Color );
int  PalConvColor8to16( unsigned char color8 ); // 8bit color to 16bit 555
void PalFadeIn( Pal8_t *PalScale, Pal8_t *PalIn, int Steps );
void PalSetFadeSndCb( void (*cb)(int) );
void PalSetColors( Pal8_t *Palette );
Pal8_t *PalGetCurrent();
void PalModulate( Pal8_t *Palette, unsigned int FirstColor, int LastColor );
void PalSetCurrentColorRGB( int PalIdx, unsigned char r, unsigned char g, unsigned char b );
void PalGetCurrentColorRGB( int PalIdx, char *pR, char *pB, char *pG );
void PalMakeShades( unsigned char Color8 );
void PalCreateShades();
void PalMakeExt( int Col );
void PalCreateExt();
// loads *.pal file
int PalLoadFromFile( const char *Path );
char *PalGetErrorMsg();
void PalLoadFromPtr( Pal8_t *pal );
void PalSetColorRGB( int ColorIdx, char r, char g, char b );
void PalGetColorRGB( int ColorIdx, char *r, char *g, char *b );
/*
    Inicjuje palete Ext:
     0 - jasnosc liniowa
     1..7  - cyklicznie z okresem 7 ?
     8..15 - cyklicznie z okresem 7 ?
*/
void PalInitExt( char *PalStack, int ColorIdx );
void PalExtInit();
char *PalExtOpen( unsigned char Color );
void PalExtClose( unsigned char Color );
void PalSetMemManage( void *( *Malloc_)(size_t), void *(*Realloc_)(void *, size_t), void (*Free_)(void *));
void PalSetBrightness( double bright );
double PalGetBrightness();
int PalIsColorUsed ( unsigned char ColrIdx );
int PalCreateColors( Pal8_t *pal, char *UnusedColors );
int PalApplyColors( Pal8_t *SrcPal, char *UnusedColors );
void PalGenerateRGBcube();
int PalUnk06( int InitFlag );
int PalUnk07( int InitFlag );
void PalResetCubeRGB( int *cube, int dim );
// push current colors on internal stack
int PalColorPush();
// pop current colors from internal stack
int PalColorPop();
int PalInit();
void PalStackZero();
Pal8_t *PalGetBase();

// inny plik
unsigned char Unk8003MaxRGB( unsigned char );

