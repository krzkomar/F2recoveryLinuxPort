#pragma once

#define CURSOR_BLANK ArtMakeId( 6, 0, 0, 0, 0 )
#define ACTION_INFO	3
#define ACTION_USE	6
#define ACTION_TURN	4
#define ACTION_TALK	5

extern int gGmouseUnk75;
extern Obj_t *gGmouseObjA;
extern Obj_t *gGmouseObjB;

/*
int gGmouseInited = 0;
int gGmouseHexCursor = 0;
int gGmouseUnk10 = 0;
int gGmouseUnk11 = 0;
int gGmouseMoveAllow = 1;
int gGmouseCursorId = 0;
CachePool_t *gGmouseCursorImg = ART_NULL;
static int gGmouseCursorIds[ 27 ] = { 
    0x10A, 0x10B, 0x10C, 0x10D, 0x10E, 0x10F, 0x110, 0x111, 0x112,
    0x113, 0x114, 0x115, 0x14A, 0x14B, 0x149, 0x148, 0x14C, 0x14E,
    0x14D, 0x14F, 0x117, 0x118, 0x119, 0x125, 0x136, 0x116, 0x127
};
int gGmouseUnk13 = 0;
int gGmouseStay = 0;
int gGmouseRstTime = 0;

ArtFrmHdr_t *gGmouseImgA = NULL;
CachePool_t *gGmouseImgObjA = ART_NULL;
int gGmouseWidthImgA = 0;
int gGmouseHeightImgA = 0;
int gGmouseSizeImgA = 0;
int gGmouseUnk55 = 0;
int gGmouseUnk54 = 0;
char *gGmouseDataImgA = 0;

ArtFrmHdr_t *gGmouseImgB = NULL;
CachePool_t *gGmouseImgObjB = ART_NULL;
int gGmouseWidthImgB = 0;
int gGmouseHeightImgB = 0;
int gGmouseSizeImgB = 0;
int gGmouseMidX  = 0;
int gGmouseMidY  = 0;
char *gGmouseDataImgB = 0;

ArtFrmHdr_t *gGmouseImgC = NULL;
CachePool_t *gGmouseImgObjC = ART_NULL;
int gGmouseWidthImgC = 0;
int gGmouseHeightImgC = 0;
int gGmouseSizeImgC = 0;
char *gGmouseDataImgC = 0;

ArtFrmHdr_t *gGmouseImgD = NULL;
CachePool_t *gGmouseImgObjD = ART_NULL;
int gGmouseWidthImgD = 0;
int gGmouseHeightImgD = 0;
int gGmouseSizeImgD = 0;
char *gGmouseDataImgD = 0;

ArtFrmHdr_t *gGmouseImgE = NULL;
CachePool_t *gGmouseImgObjE = ART_NULL;
int gGmouseWidthImgE = 0;
int gGmouseHeightImgE = 0;
int gGmouseSizeImgE = 0;
char *gGmouseDataImgE = 0;


char gGmouseUnk32 = 0;
char *gGmouseUnk31 = 0;
char gGmouseUnk30 = 0;
short gGmouseUnk29[ 11 ] = { 253, 255, 257, 259, 261, 263, 265, 302, 304, 435, 0 };
int gGmouseUnk23 = 1;
int gGmouseShape = 0;

int gGmouseShapeIds[2] = { 249, 250 };
int gGmouseUnk26[ 5 ]= { 251, 293, 293, 293, 293 };
int gGmouseUnk25[11] = { 293, 293, 293, 293, 6, 7, 9, 10, 11, 12, 13 };
int gGmouseUnk06 = 0;
int gGmouseTime = 0;
int gGmouseRestoreCursor = -1;
int gItemHlShow = 1;
Obj_t *gGmouseObjC = NULL;
int gGmouseUnk75 = 0;
int gGmouseUnk21 = -1;

int gGmouseUnk61[ 10 ];
int gGmouseOldX;
int gGmouseOldY;
Obj_t *gGmouseObjA;
Obj_t *gGmouseObjB;
Obj_t *gGmouseActionObject;
*/

int GmouseInit();
int GmouseReset();
void GmouseClose();
void GmouseSetIsoMode();
void GmouseSetIfaceMode( int ScrollEn );
int GmouseHexCursorMode();
void GmouseScrollEnable();
void GmouseScrollDisable();
int GmouseScrollEnabled();
void GmouseUnk28( int a1 );
int GmouseUnk29();
int GmouseReachBorder();
void GmouseProcess();
void GmouseAction( int MseX, int MseY, int MseButt );
int GmouseLoadCursor( int CursorId );
int GmouseGetCursorId();
void GmouseUnk50( int a1 );
void GmouseUnk34();
void GmouseUnk51();
int GmouseUnk52();
void GmouseSetMode( int mode );
int GmouseGetMode();
void GmouseCycleMode();
void GmouseUnk55();
void GmouseSetShapeA( int NewImg );
int GmouseUnk56();
void GmouseSetHexCursorBlank();
void GmouseIsoEnter();
void GmouseUnk03();
int GmouseUnk58();
Obj_t *GmouseGetObject( int ObjType, int Flg, int MapLvl );
int GmouseTile( int x0, int y0, int CursorType, int a4, int a5);
void GmouseGetCenter( int *pXmid, int *pYmid );
int GmouseCursorMenuCreate( int xpos, int ypos, int *IconsArray, int IconsCount, int xmax, int ymax );
int GmouseUnk60( int *a1, int *a2 );
int GmouseMenuIconHL( int a1 );
int GmousePrint( char *str, int Color );
int GmouseText( char *str, int Color );
void GmouseHLcfg();
int GmouseInitA();
int GmouseUnk62();
void GmouseDestroyCursors( );
int GmouseLoadImages();
void GmouseCloseImages();
int GmouseUnk65( int a1, VidRect_t *a2 );
int GmouseSetShapeB( VidRect_t *Area );
int GmouseCursorUpdate( int x, int y, int lvl, VidRect_t *Area );
int GmouseViewMove( int MseX, int MseY );
void GmouseUnk66( Obj_t *result );
int GmouseUnk43( Obj_t *scen );

