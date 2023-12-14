#pragma once

#define FONT_MGR_DATABASE_SIZE 	16
#define FONT_AFF_MAGIC 		0x41414646 // file magic 'AAFF'
#define FONT_AFF_FILE_OFFSET	2060
#define WBE_TO_LE( x )		(x) = (((unsigned short)(x) >> 8) & 0x00ff) | (( (unsigned short)(x) << 8) & 0xff00)

typedef struct { // size of 8
    short	Width;
    short	Height;
    int		Offset;
} FontMgrDsc_t;

typedef struct{ // size of 0x80c 2064
    short	MaxH;		// 0,1
    short	gapH;		// 2,3
    short	SpaceW;		// 4,5
    short	gapW;		// 6,7
    short	MaxW;		// 8,9 [10,11]
    FontMgrDsc_t dsc[ 256 ];	// 12 ... [2048]
    char	*Data;		// 2060
} FontMgr_t;

extern FontObj_t gFontInit;

//FontMgr_t  gFontMgrDataBase[ FONT_MGR_DATABASE_SIZE ];
//int 	   gFontMgrCurId;
//FontMgr_t *gFontMgrCurData;

int FontMgrInit();
void FontMgrClose();
int FontMgrLoad( int FontId );
void FontMgrSelect( int FontId );
int FontMgrHeight();
int FontMgrLineWidth( char *str );
int FontMgrWidth( char Chr );
int FontMgrWidthMono( char *str );
int FontMgrDistance();
int FontMgrPixels( char *str );
int FontMgrMaxWidth();
int FontMgrGetCurId();
void FontMgrPrint( unsigned char *Dst, char *Str, int Pitch, int FieldPixSize, int Color );
int FontMgrGetGapW();
int FontMgrGetSpaceW();


