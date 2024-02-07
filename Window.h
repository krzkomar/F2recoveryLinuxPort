#pragma once

// draw text flags
#define WIN_UPDATE		0x1000000

#define WIN_COUNT_MAX		50
#define WIN_ERROR		-1
#define WIN_PALCOLOR_ALPHA      gPalColorCubeRGB[0][0][gWinSys.Colors[0]]
#define WIN_BGALPHA		0x100

// widget flags
#define WIN_FLG_CLEAN	0x08 // if clear screen is dirty
#define WIN_FLG_4	0x04 // on top ?
#define WIN_FLG_1	0x01
#define WIN_FLG_BLIT	0x20 // use blitter callback field to screen copy
#define WIN_FLG_40	0x40

// colors
#define WIN_FRCOL_ALPHA	gPalColorCubeRGB[0][0][ 0 ]
#define WIN_FRCOL_A	gPalColorCubeRGB[0][0][ gWinSys.Colors[1] ]
#define WIN_FRCOL_B	gPalColorCubeRGB[0][0][ gWinSys.Colors[2] ]
#define WIN_FRCOL_C	gPalColorCubeRGB[0][0][ gWinSys.Colors[3] ]
#define WIN_FRCOL_D	gPalColorCubeRGB[0][0][ gWinSys.Colors[4] ]
#define WIN_COLOR_A 	0x294A // RGB16_SET()
#define WIN_COLOR_B 	0x3DEF // RGB16_SET()
#define WIN_COLOR_C 	0x2108 // RGB16_SET()
#define WIN_COLOR_D 	0x5294 // RGB16_SET()
#define WIN_COLOR_E 	0x7FEB // RGB16_SET()
#define WIN_COLOR_F 	0x7C00 // RGB16_SET()

#define WIN_XY( x, y, pitch, pSurface )	((pSurface) + (x) + (y)*(pitch))

typedef struct { // size of 272
    int ArgA;
    int State;    
    void (*Cb)();
    int Count;
    struct _Widget_t *Wgs[ 64 ];
} WinRadio_t;

typedef struct { // size of 36
    VidRect_t Rect;
    int key;
    int Lines;
    char **TextList;
    int FgColor;
    int BgColor;
} WinDlgBox_t;

typedef struct // size of 572
{
    int WinId;		// 4
    VidRect_t	Area;	// 16
    int Cnt;		// 4
    WinDlgBox_t box[ 15 ];    // 36 *15 = 540
    int ColorA;		// 4
    int ColorB;		// 4
} WinDlg_t; // textbox01_t ?


typedef struct 
{
    int		Init;
    int		Colors[ 6 ];
} WinSys_t;

typedef struct _Widget_t // size of 0x7c
{
    int 	Id;
    int 	Flags; // 4,5,6,7
    VidRect_t	area;
    int 	HotKeys[6]; // 3-keycode, 2-keycode
    char 	*AnimFrame[6];
    char 	*CurrentFrame;
    char 	*DefImg;
    void 	(*Cb0)(); // pointed area
    void 	(*Cb1)();
    void 	(*Cb2)(); // left butt pressed
    void 	(*Cb3)(); // left butt released
    void 	(*Cb4)();
    void 	(*Cb5)();
    int 	(*ClickPlayA)();
    int 	(*ClickPlayB)();
    WinRadio_t 	*Radio;
    struct _Widget_t *prev;
    struct _Widget_t *next;
} Widget_t;

typedef struct // size of 0x44
{
    int 	Id;
    int 	Flags;
    VidRect_t 	Geometry;
    int 	Width;
    int 	Height;
    int 	FontColor;
    int 	RandX;
    int 	RandY;
    char 	*Surface;
    Widget_t 	*Widget;
    Widget_t 	*PointedWg; // mouse cursor points this widget
    Widget_t 	*Img1;
    WinDlg_t 	*Dialog;
    void 	(*Blitter)(void *pSrc, int w, int h, int Spitch, void *pDst, int Dpitch);
} Window_t;

extern WinSys_t gWinSys;
extern char *gWinSurface;
//extern char gWinFillPattern[4];
extern int gWinUnk01;
extern int gWinWidgetCount;
extern int gWinWidgetDefaultFlags;
extern int gWinRedrawAllExecFlag;
extern int gWinImg01;
extern Image_t *gWinTexture;
extern int gWinWidgetIds[ WIN_COUNT_MAX ];
extern Window_t *gWinWidget[ WIN_COUNT_MAX ];
extern int gWinGuard;
extern void (*gWinRestoreDisplay)();

int   WinInit( int (*VidModeInit)(), void (*Close)(), int Flags );
int   WinResize( int (*a1)(void) );
int   WinReady();
void  WinDestruct();
int   WinCreateWindow( int Xpos, int Ypos, int Width, int Height, short BgColor, int Flags );
Widget_t *WinCreateWidget( int WinId, int PosX, int PosY, int Width, int Height, int g0, int g1, int g2, int HotKey, int Flags, char *Frame0, char *Frame1, char *Frame2 );
void  WinClose( int WgId );
void  WinFree( int WinId );
void  WinGetUnk01( int arg );
void  WinDrawFrame( int WinId );
void  WinSetDefaultColors();
void  WinLoadTexture( const char *fname );
void  WinClear( int Pattern );
void  WinDrawText( int WinId, char *text, int TextWidth, int Xpos, int Ypos, int Color );
void  WinDrawTextList( int WinId, char **text, int Lines, int UnderLineWidth, int Xpos, int Ypos, int Color );
void  WinDrawLine( int WinId, int x0, int y0, int x1, int y1, int Color );
void  WinDrawRectangle( int WinId, int x0, int y0, int x1, int y1, int Color );
void  WinDrawShadowRect( int WinId, int x0, int y0, int x1, int y1, int ColorUp, int ColorDn );
void  WinDrawFilledRect( int WinId, int Xpos, int Ypos,int Width, int Height, int Color );
void  WinMoveTop( int WinId );
void  WinUpdateDirty( int WinId );
void  WinSetPosition( int WinId, int Xpos, int Ypos );
void  WinUpdate( int WinId );
void  WinAreaUpdate( int WinId, VidRect_t *Area );
void  WinRedraw( Window_t *win, VidRect_t *Geometry, char *pSurface );
void  WinUpdateArea( VidRect_t *Area );
void  WinRegionUpdate( Window_t *Win, Regions_t **Mse, char *pSurface );
void  WinDragAndMove( int WinId );
void  WinWhipeCursor(char *Surface);
void  WinRedrawArea( VidRect_t *Area, char *Surface );
Window_t *WinGetWindow( int WinId );
char *WinGetSurface( int WinId );
int   WinGetPointedWidgetId( int x, int y );
int   WinGetWidth( int WinId );
int   WinGetHeight( int WinId );
int   WinGetRect( int WinId, VidRect_t *Geometry );
int   WinCursorProcess();
Widget_t *WinLookup( int WgtId, Window_t **win );
int   WinDialogRun( int arg );
Image_t *WinLoadImage(const char *fname );
void  WinSetTitle( const char *Title );
void  WinSetBlitter( int WinId, void (*Blitter)(void *, int, int, int, void *, int));
xFile_t *WinOpenFile( const char *fname, int mode );
void  WinCloseFile( xFile_t *handler );
int   WinReadFile( xFile_t *handler, void *buff, int size );
int   WinMsgError( const char *msg );
int   WinCreateButton( int WinId, int Xpos, int Ypos, int Width, int Height, int ImgId0, int ImgId1, int ImgId2, int HotKey, char *ImgChk, char *ImgUnChk, char *ImgUnk, int Flags);
int   WinAddButton( int WinId, int PosX, int PosY, int g0, int g1, int g2, int HotKey, char *LblText, int Flags );
int   WinSetAnimH( int WdgId, char *Img0, char *Img1, char *Img2 );
int   WinSetAnimL( int WdgId, char *Img0, char *Img1, char *Img2, int Hide );
int   WinSetButtonHandler( int WdgId, void *Cb0, void *Cb1, void *Cb2, void *Cb3 );
int   WinSetHandler2( int WdgId, int ff0, int ff1, void *Cb0, void *Cb1 );
int   WinSetClickSound( int WsgId, int ( *ClickPushCb)(), int ( *ClickReleaseCb)() );
int   WinSetDefaultImg( int WdgId, char *Img );

int   WinButtonState( int WdgId );
int   WinProcess( Window_t *win, int *a2 );
int   WinGetAreaPointedPixel( Widget_t *Wg, VidRect_t *Rect );
int   WinImageGetWinId( int ImageId );
int   WinGetPointed();
int   WinImageRemove( int ImageId ); // WinWidgetFree()
void  WinWidgetsFree( Widget_t *img );
void  WinCloseWithKey( int WdgId, int Key );
int   WinGetEmpty();
int   WinEnableWidget( int WgId );
int   WinDisableWidget( int WgId );
int   WinButtonSet( int Id, int Flg1, int Flg2 );
int   WinRadioButtonAssign( int a1, int *a2, int a3, void *a4);
void  WinRadioButton( int Count, int *Buttons );
int   WinUnk971( Widget_t *a1 );
void  WinDrawWidget( Widget_t *Wdt, Window_t *Win, char *pSrcPic, int Hide, VidRect_t *Position, int Update );
int   WinUnk970( int a1 );

