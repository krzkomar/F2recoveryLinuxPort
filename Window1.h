#pragma once

typedef struct 
{
  int BtId;
  int Width;
  int Height;
  int Xpos;
  int Ypos;
  int Flags;
  int WgFlags;
  char Name[32];  
 Intp_t *Script;
  char *DownPix;
  char *UppPix;
  char *OverPix;
  void *i20;
  char *ImgData;
  int ProcButtPress;
  int ProcButtRelease;
  int ProcButtOn;
  int ProcButtOff;
  int ProcRtButtOn;
  int ProcRtButtOff;
  int (*i28)(int, int, void*);
  int (*i29)(); 
  int i30; // arg dla i28 ?
  int i31; // arg dla i29 ?
} Win03_t;


typedef struct 
{
    char	Name[ 32 ];
    int		WinId;
    int 	Width;
    int 	Height;
    Region_t 	**Regions;
    int 	RegionId;
    int 	RegionCnt;
    int 	Region;
    Win03_t 	*Buttons;
    int 	Cnt;
    int		TextWidth;
    int 	TextHeight;
    int 	TextColor;
    int 	TextFlags;
    float 	ScaleX;
    float 	ScaleY;
} Win02_t;

extern Win02_t gWinList[ 16 ];
extern int gWinCurrent;

/*
int gWinUnk203[ 16 ];
int gWinUnk209[ 64 * 256 ];

int (**gWinUnk100)( int );
int (*gWinUnk109)( int, char *, int * );
int (*gWinUnk202)( int, char * );
int gWinResortWidth;
int (*gWinUnk110)( int, char *, char *, int, int );
int (*gWinUnk108)( int id, char *);
int gWinResortHeight;
int gWinColorR_HL;
int gWinTextFont;
int (*gWinUnk115)( int, char *, int * );
int (*gWinUnk116)( int, char *, int * );
int (*gWinUnk117)( int, char *, int * );
int gWinColorG;
int gWinColorB;
int gWinTextFlags;
int gWinColorR;
int gWinColorG_HL;
int gWinColorB_HL;
int gWinUnk200 = 250;
int gWinUnk104 = 1;
int gWinUnk201 = -1;

static int ( *gWinInitVidModeSel[12])() = {
static Resolutions_t gWinResolutionTable[12] = {
int gWinUnk101 = 0;
int gWinDlgCnt = -1;
int gWinUnk102 = 1;
*/

/*************************************************************************/

int WinGetFont();
int WinSetFont( int FontId );
void WinSetTextColorDefault();
int WinGetTextFlags();
int WinSetTextFlags( int Flags );
char WinGetTextColor();
char WinGetColorHL();
int WinSetTextColor( float r, float g, float b );
int WinSetTextColorHL( float r, float g, float b );
int WinUnk97( char *Str, int a2, int a3 );
int WinUnk47( int a1, int a2, int a3, int a4 );
int WinMouseUpdate( int a1, int x, int y, int butt );
int WinMouseRfsh();
int WinMouseHandle();
void WinDlgInsertExec( int (*ptr)(int) );
void WinUnk18( Region_t *reg, int idx );
void WinUnk19( Region_t *reg, int idx );
int WinActivateRegion( char *RegionName, int RegionProc );
int WinDlgProcess();
void WinUnk23( int a1 );
void WinUnk231( int BtId, int Idx );
void WinUnk24( int a1 );
void WinUnk25( int a1 );
void WinUnk28( int a1 );
int WinUnk26( int a1 );
int WinUnk27( int a1, int a2 );
int WinUnk29( int a1 );
void WinDrawButton( int w, int h, char *surf1, char *surf2, char *surf3 );
int WinSetAnim( Widget_t *wg );
int WinUpdateCurrent();
int WinMoveOnTopCurrent();
int WinUpdateCurrentRegion( int lt, int tp, int rt, int bm );
int WinUpdateRegionByIdx( int Idx, int lt, int tp, int rt, int bm );
int WinGetCurrentWidth();
int WinGetCurrentHeight();
int WinGetCurrentPosX();
int WinGetCurentPosY();
int WinCurrentPosWithin( int xpos, int ypos );
int WinGetCurrentArea( VidRect_t *Area );
int WinGetCurrentId();
int WinGetCurrentWin();
int WinGetId( unsigned int Id );
int WinDelete( char *name );
int WinRes( char *a1, int ecx0, int a3, int Width, int Height );
int WinScale( char *Name, int xpos, int ypos, int Width, int Height );
int WinCreate( char *Name, int Xpos, int Ypos, int Width, int Height, int Color, int Flags );
int WinError( char *a1 );
int WinMoveXY( int a1, int a2 );
int WinFillRect( int Idx );
int WinUnk31( char *a1 );
int WinUnk32( char *a1 );
char *WinFill4();
Win02_t *WinUnk33();
int WinSelect( char *a1 );
int WinRemove();
void WinPrintText( int Id, char *Str, int BufferSize, int FieldSize, int MaxHeight, int posX, int posY, int Color, int Align );
char **WinTextWrapCreate( char *Text, int FieldWidth, int LineWidth, int *pLines );
void WinTextWrapDestroy( char **list, int cnt );
void WinPrintWrapped( int WinId, char *Text, int FieldWidth, unsigned int Height, int posX, int PosY, int Color, unsigned int Flags, int StandOff );
void WinPrintMessage( int WinId, char *Text, int FieldWidth, int Height, int Xpos, int Ypos, int Color, int Flags );
int WinPrintRect( char *Text, int height, unsigned int Flags );
int WinFormatMsg( char *Text, int posX, int posY, int TextWidth, int Height, unsigned int Flags );
int WinPrintA( char *Text, int posX, int posY, int TextWidth, unsigned int Height, unsigned int Flags, int Color );
int WinUnk11( char *Text, int TextWidth, int Xpos, int Ypos, int Color );
int WinUnk180( char *Text, int TextWidth, int Xpos, int Ypos, int Color, int FontId );
void WinUnk12( char *data, int Width, int Height, int Pitch );
void WinUnk10( char *fname );
void WinUnk13( char *fname );
int WinUnk181( char *a1, char *a2 );
int WinUnk182( char *fname );
int WinDisplayGFX( char *fname, int a2, int a3, int a4, int a5 );
int WinUnk183( char *fname, int a2, int a3, int a4, int a5 );
int WinUnk15( char *pSrc, int SrcPitch, int a3, int Xpos, int Ypos, int Width, int Height );
int WinUnk184( char *pSrc, int SrcPitch, int a3, int Xpos, int Ypos, int Width, int Height );
int WinUnk16( char *pSrc, int SrcPitch, int a3, int Xpos, int Ypos, int Width, int Height );
int WinGetScreenWidth();
int WinGetScreenHeight();
int WinClean( Intp_t *cmp );
void WinSetup( int VideoModeSel, int Flags );
void WinDlgClose();
int WinDeleteButton( char *a1 );
void WinUnk185( char *Name, int a2 );
int WinUnk186( char *Name );
int WinAddButtonFlag( char *Name, int Flags );
void WinUnk187( void (*Cb1)(), int (*Cb2)(), int (*Cb3)() );
int WinNewButton( char *Name, int xpos, int ypos, int Width, int Height, int Flags );
int WinAddButtonGFX( char *Name, char *DownPixName, char *UppPixName, char *OverPixName );
int WinUnk103( char *a1, void *a2 );
int WinUnk104( char *Name, char *a2, char *a3, char *a4, int a5, int a6, int a7 );
int WinAddButtonProc( char *Name, Intp_t *script, int ProcButtOn, int ProcButtOff, int ProcButtPress, int ProcButtRelease );
int WinAddButtonRightProc( char *Name, Intp_t *Scr, int ProcRtButtOn, int ProcRtButtOff );
int WinUnk190( char *Name, int (*a2)(int, int, int), int a3 );
int WinUnk191( char *Name, int (*a2)(), int a3 );
int WinAddButtonText( char *Name, char *Text );
int WinUnk105( char *Name, char *Text, int bx, int by, int ax, int ay );
int WinFill( float fr, float fg, float fb );
int WinFillArea( int xpos, int ypos, int width, int height, float r, float g, float b );
void WinSetRegion();
void *WinUnk195( char *Name );
void WinUnk196( char *Name, void *pUser );
int WinCheckRegion( char *Name );
int WinUnk106( int RegIdx );
int WinUnk193( int a1, int a2, int a3 );
int WinUnk197( int a1, int a2, int a3, int a4, int a5 );
int WinUnk198( char *Name, int (*Cb)(void *, int, int), int CbArg );
int WinUnk199( char *Name, int (*Cb)(void *, int, int), int CbArg );
int WinRegionSetProcedure( char *Name, Intp_t *Script, int ProcOn, int ProcOff, int ProcPress, int ProcRelease );
int WinRightButtonSetProcedure( char *Name, Intp_t *Script, int a3, int a4 );
int WinAddRegionFlag( char *Name, int Flags );
int WinSetRegionA( char *a1 );
int WinDeleteRegion( char *Name );
void WinRun();
int WinPlay();
int WinSetMovieFlags( char a1 );
int WinPlayStart( char *a1 );
int WinPlayAt( char *eax0, int a1, int a2, int a3, int a5 );
void WinPlayAbort();
void WinUnk200( char *pDst, int DstPitch, int a3, char *pSrc, int SrcPitch, int a6 );
void WinUnk70( char *a1, int a2, int a3, int a4, char *pSrc, int Width, int a7, int SrcPitch );
int WinResizeArt( char *a1, int a2, int a3, char *a4, int a5, int a6 );
void WinUnk201( char *a1, int a2, int a3, int a4, unsigned char *a5, unsigned char *a6, char *a7, int a8 );
int WinUnk202( char *a1, int a2, char *a3, char *a4, char *a5, int a6, int *a7 );
void WinBlit( char *data, int SrcW, int SrcH, char *surf, int DstW, int DstH );
void WinUnk203();
void WinUnk204();
void WinUnk205( int a1 );
int WinUnk206( int a1, int a2, int a3, int a4, int Wrap, int Flags, char a7 );
int WinDrawTextA( int Idx, char *str );
int WinLabelMouseDefine( int MsgId );
int WinUnk207( int a1 );
int WinUnk208( int a1, int a2, int a3, int a4, unsigned char a5 );
int WinUnk209( int a1, char *a2, int a3, void *a4 );
int WinUnk210( int Idx );
int WinTextWrap( char *Str, int LineWidth, short *pOffs, short *pLines );



