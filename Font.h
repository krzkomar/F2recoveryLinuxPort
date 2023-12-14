#pragma once

// flags for print (or-ed on color)
#define FONT_SHADOW	0x10000
#define FONT_UNDERLINE	0x20000
#define FONT_MONOSPACE	0x40000


typedef struct // size of 8
{
    int width;		// 0,1,2,3
    int offset;		// 4,5,6,7
} FontInfo_t;

typedef struct  // size of 20
{
  int count;		// 0,1,2,3
  int height;		// 4,5,6,7
  int distance;		// 8,9,10,11
  FontInfo_t *fontinfo;// 12,13,14,15
  unsigned char *data;	// 16,17,18,19
} FontHdr_t;

typedef struct FontObj_t // size of 44
{
    int dbMin;			// 0,1,2,3
    int dbMax;			// 4,5,6,7
    void (*Select)(int FontId);	// 8,9,10,11
    void (*Print)(char *Dst, char *Str, int FieldPixSize, int Pitch, int Color );	// 12,13,14,15
    int (*Height)();			// 16,17,18,19
    int (*LineWidth)(char *str);	// 20,21,22,23
    int (*Width)(char Chr );		// 24,25,26,27
    int (*WidthMono)(char *str);	// 28,29,30,31
    int (*Distance)();	// 32,33,34,35
    int (*Pixels)(char *Str);	// 36,37,38,39
    int (*MaxWidth)();	// 40,41,42,43
} FontObj_t;

typedef struct // size of 40
{
    int FontId;
    int Fonts;
    void (*Print)(char *Dst, char *Str, int Pitch, int FieldPixSize, int Color );
    int (*ChrHeight)();
    int (*LineWidth)(char *Str );
    int (*ChrWidth)( char Chr );
    int (*LineWidthMono)(char *Str );
    int (*Distance)();
    int (*Pixels)(char *Str);
    int (*MaxWidth)();
} Font_t;


extern Font_t gFont;
//FontHdr_t gSelectedFontData[ 10 ];

int  FontInit();
void FontFree();
int  FontLoadFromFile( int FontFileIdx );
int  FontSetup( FontObj_t *p );
int  FontUnk01( int Id );
void FontDataSelect( int FontId );
int  FontGetCurrent();
void FontSet( int FontId );
int  FontLookup( int n, FontObj_t **p );
void FontPrint( char *Dst, char *Str, int DstPitch, int FieldPixSize, int Color );
int  FontGetHeight();
int  FontGetLineWidth( char *str );
int  FontCharGetWidth( char chr );
int  FontGetLineWidthMono( char *str );
int  FontGetDistance();
int  FontLinePixels( char *str );
int  FontGetMaxWidth();

