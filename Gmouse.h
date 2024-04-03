#pragma once

// cursor shapes
#define CURSOR_CLOCK	26

#define CURSOR_BLANK ArtMakeId( 6, 0, 0, 0, 0 )
#define ACTION_INFO	3
#define ACTION_USE	6
#define ACTION_TURN	4
#define ACTION_TALK	5

extern int gGmouseUnk75;
extern Obj_t *gGmouseObjA;
extern Obj_t *gGmouseObjB;

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

