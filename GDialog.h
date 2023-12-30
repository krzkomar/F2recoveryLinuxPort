#pragma once

#define DLG_COLOR_1	gPalColorCubeRGB[17][17][17]
#define DLG_COLOR_2	gPalColorCubeRGB[21][21][11]
#define DLG_COLOR_3	gPalColorCubeRGB[18][17][3]
#define DLG_COLOR_4	gPalColorCubeRGB[20][20][20]
#define DLG_COLOR_5	gPalColorCubeRGB[0][31][0]
#define DLG_COLOR_6	gPalColorCubeRGB[0][24][0]
#define DLG_COLOR_7	gPalColorCubeRGB[15][15][15]
#define DLG_COLOR_8	gPalColorCubeRGB[0][0][31]
#define DLG_COLOR_9	gPalColorCubeRGB[31][0][0]
#define DLG_COLOR_10	gPalColorCubeRGB[31][31][11]

typedef struct{
    int i01;
    int i02;
    void *i03;
    int i04;
    int i05;
    char *i06;
} Gdialog01_t;

typedef struct 
{
    int Xpos;
    int Ypos;
    int ArtId1;
    int ArtId2;
    int ArtId3;
    CachePool_t *Art1;
    CachePool_t *Art2;
    CachePool_t *Art3;
    int HotKey;
    int Disposition;
} Gdialog02_t;

typedef struct 
{
  int MsgPage;
  int MsgId;
  int Reaction;
    int i04;
  int Button;
  int Top;
  char Text[900];
  int Bottom;
} Gdialog_t;

extern Obj_t *gDlgUnk44;
extern short gDlgUnk46;

/*
int   gDlgUnk06 = 0;
int   gDlgOption = 0;
int   gDlgReviewCount = 0;
char  *gDlgSurf = NULL;
int   gDlgBoxTitle = -1;
int   gDlgBoxWin = -1;
int   gDlgUnk88 = 0;
int   gDlgUnk87 = 0;
int   gDlgUnk86 = 0;
CachePool_t *gDlgUnk85 = 0;
int   gDlgUnk84 = 0;
short gDlgUnk83 = 2;
int   gDlgUnk82 = 0;
CachePool_t *gDlgUnk81 = 0;
int   gDlgUnk80 = 0;
int   gDlgLipsEn = 0;
int   gDlgUnk09 = 0;
int   gDlgUnk08 = 0;
int   gDlgUnk07 = -1;
int   gDlgUnk13 = 0;
int   gDlgUnk18 = 0;
int   gDlgUnk17 = 0;
int   gDlgUnk16 = 0;
Obj_t *gDlgUnk15 = NULL;
Obj_t *gDlgUnk20 = NULL;
Obj_t *gDlgUnk21 = NULL;
int   gDlgUnk14 = 0;
int   gDlgBackWindow = -1;
int   gDlgSurface = -1;
VidRect_t gDlgUnk22[ 8 ] = { 
int gDlgUnk26 = 1;
int gDlgUnk27 = 0;
int gDlgReplyWin = -1;
int gDlgUnk28 = -1;
int gDlgUnk29 = -1;
int gDlgUnk12 = -1;
int gDlgUnk11 = -1;
int *gDlgPalA = 0;
int *gDlgPalB = 0;
int gDlgUnk10 = 0;
int gDlgUnk32 = 0;
CachePool_t *gDlgArt[ 8 ] = { ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL };
int gDlgUnk36 = 0;
int gDlgUnk37 = 35;
int gDlgUnk38 = 35;
int gDlgUnk39 = 82;
int gDlgUnk40 = 35;
int gDlgUnk41 = 37;
int gDlgUnk42 = 46;
int gDlgUnk43[] = { 89, 90, 87, 88, 91, 92 };
Obj_t *gDlgUnk44 = NULL;
int gDlgUnk45 = 0;
short gDlgUnk46 = 0;
int gDlgUnk47 = -1;
int gDlgUnk48[ 42 ] = { 0, 3, 1, 1, 3, 1, 1, 1, 7, 8, 7, 3, 1, 8, 1, 7, 7, 6, 6, 2, 2, 2, 2, 4, 4, 5, 5, 2, 2, 2, 2, 2, 6, 2, 2, 5, 8, 2, 2, 2, 2, 8 };
int gDlgUnk49 = 0;
int gDlgUnk50 = -1;
int gDlgUnk51 = 10000;
const char *gDlgUnk52[ 3 ] = { "Said Good", "Said Neutral", "Said Bad" };
int gDlgUnk53[ 50 ] = { 
int gDlgTextIds[ 6 ][ 12 ][ 2 ] = {
int gDlgUnk71[ 6 ][10] = {
int gDlgUnk79 = 0;
int gDlgUnk105[ 6 ];
Msg_t gDlgMsg;
char gDlgLumA[256];
char gDlgLumB[256];
void *gDlgUnk97[ 8 ];
VidRect_t gDlgBoxWinArea;
VidRect_t gDlgBoxTitleArea;
Gdialog01_t gDlgReviewSlots[ 80 ];
int gDlgUnk112;
int gDlgUnk113;
int gDlgUnk114;
CachePool_t *gDlgUnk115;
int gDlgButtons[ 9 ];
CachePool_t *gDlgUnk120;
CachePool_t *gDlgUnk121;
int gDlgImg100Height;
CachePool_t *gDlgUnk123;
char *gDlgImgBtA1;
char *gDlgImg100Width;
char *gDlgImgBtA0;
int gDlgImg101Width;
ArtFrmHdr_t *gDlgImg100Dat;
int gDlgImg101Height;
CachePool_t *gDlgUnk130;
CachePool_t *gDlgUnk131;
ArtFrmHdr_t *gDlgImg101Dat;
int gDlgUnk133;
int gDlgUnk134;
int gDlgUnk135;
Intp_t *gDlgUnk136;
int gDlgUnk137;
int gDlgUnk138;
int gDlgUnk139;
char gDlgTitleMsg[ 2712 ];
Gdialog_t gDlgOptions[ 30 ]; 
int gDlgUnk149;
int gDlgUnk150;
int gDlgUnk151;
*/

int  GdialogInit();
int  GdialogClose();
int  GdialogUnk01();
void GdialogEnter( Obj_t *target, int flg );
int  GdialogUnk02();
int  GdialogLipsyncStart( char *DirName );
void GdialogLipsyncEnd();
int  GdialogUnk03();
int  GdialogUnk04();
int  GdialogUnk05( int a1, int a2 );
int  GdialogUnk06();
int  GdialogUnk07( int a1 );
void GdialogReply( char *str );
void GdialogUnk08();
void GdialogUnk09();
int  GdialogUnk10( int a1, int a2, int a3 );
int  GdialogUnk11( int a1, char *a2, int a3, int a4 );
int  GdialogUnk12( int a1, int a2, int a3, int a4 );
int  GdialogUnk13( int a1, char *a2, int a3, int a4 );
int  GdialogUnk14( Intp_t *a1, int a2, int a3 );
int  GdialogUnk15( Intp_t *a1, char *a2 );
int  GdialogUnk16();
void GdialogUnk17();
int  GdialogUnk18();
int  GdialogUnk19();
int  GdialogUnk20();
int  GdialogUnk21( int Page, int TxtId, int Reaction );
int  GdialogUnk22( int a1, char *Text, int Reaction );
int  GdialogCreateReview( int *pWin );
int  GdialogCloseReview( int *pWin );
int  GdialogReview();
void GdialogUnk26( int win, int pos );
void GdialogUnk27();
int  GdialogUnk28( Intp_t *a1, int a2, int a3 );
int  GdialogReviewAdd1( Intp_t *a1, char *Text );
int  GdialogReviewAdd2( int a1, int a2 );
int  GdialogReviewAdd3( char *Text );
int  GdialogUnk30();
int  GdialogUnk31();
int  GdialogUnk32();
void GdialogUnk33();
int  GdialogUnk34();
int  GdialogUnk35( int a1 );
void GdialogDisplayOption( int LineNo );
void GdialogDisplayLine( int LineNo );
void GdialogDisplayTitle();
void GdialogCreateDialog();
int  GdialogUnk40();
void GdialogUnk41();
void GdialogUnk42( int a1, unsigned int a2);
void GdialogUnk43();
void GdialogTransition( int a1 );
void GdialogUnk45();
void GdialogUnk46();
int  GdialogUnk47();
void GdialogDemoCopyTitle( int a1 );
void GdialogDemoCopyOptions( int a1 );
void GdialogRfshOptionRect( int win, VidRect_t *DrawRect );
void GdialogUnk51( int a1 );
void GdialogReaction( int a1 );
int  GdialogUnk53( int a1, int a2, char *a3, char *a4, char *a5, int a6, int a7 );
int  GdialogGetLines( char *text, int width );
void GdialogUnk55( char *Surf, VidRect_t *Area, char *Text, int *pStrPos, int Height, int Pitch, int Color );
int  GdialogDisplayMsg( char *Surf, VidRect_t *Area, char *Text, int *pStrPos, int Height, int Pitch, int Color, int Flag );
int  GdialogUnk57( int a1 );
int  GdialogUnk58( int a1 );
void GdialogUnk59();
int  GdialogUnk60();
void GdialogUnk61();
int  GdialogUnk62();
int  GdialogUnk63();
void GdialogUnk64();
void GdialogUnk65();
void GdialogUnk66();
int  GdialogUnk67( Obj_t *a1 );
int  GdialogUnk68();
void GdialogUnk69();
int  GdialogUnk70();
void GdialogUnk71();
void GdialogUnk72();
void GdialogUnk73();
void GdialogUnk74( char *a1, int a2, int a3, int a4 );
int  GdialogUnk75( int a1 );
int  GdialogUnk76( int a1, int a2 );
void GdialogBarter();
int  GdialogUnk78();
void GdialogUnk79();
int  GdialogCreateWindow();
int  GdialogRenderB();
int  GdialogRenderA( VidRect_t *Area );
void GdialogBlit( char *pSrc, int SrcPitch, int SrcHeight, int DstWidth, char *pDst, int PosX, int PosY, int DstPitch, char *Shader );
void GdialogRenderHead( ArtFrmHdr_t *Img, int FrameNo );
void GdialogUnk85();
int  GdialogUnk86();



