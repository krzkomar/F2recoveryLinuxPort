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
    int 	ReplyMsgList;
    int 	ReplyMsgNum;	
    char 	*ReplyText;
    int 	OptionMsgList;
    int 	OptionMsgNum;
    char 	*OptionText;
} GdialogReview_t;

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
    int ProcedureTargetId;
  int Button;
  int Top;
  char Text[900];
  int Bottom;
} Gdialog_t;

extern Obj_t *gDlgPartyMemberObj;
extern int gDlgHeadId;
extern int gDlgActionSrc;
extern int gDlgPmExchangeMode;

int  GdialogInit();
int  GdialogClose();
int  GdialogUnk01();
void GdialogEnter( Obj_t *target, int flg );
int  GdialogFloatingMessages();
int  GdialogLipsyncStart( char *DirName );
void GdialogLipsyncEnd();
int  GdialogUnk03();
int  GdialogUnk04();
int  GdialogStartConversation( int HeadId, int Mood );
int  GdialogUnk06();
int  GdialogSetBg( int BackgroundIdx );
void GdialogReply( char *str );
int  GdialogStart();
void GdialogFinish();
int  GdialogSayOptionConstNoProc( int a1, int a2, int a3 );
int  GdialogSayOptionStrNoProc( int a1, char *a2, int a4 );
int  GdialogSayOptionConst( int a1, int a2, int a3, int a4 );
int  GdialogSayOptionStr( int a1, char *a2, int a3, int a4 );
int  GdialogSayReplyConst( Intp_t *a1, int a2, int a3 );
int  GdialogSayReply( Intp_t *script, char *Text );
int  GdialogEnd();
void GdialogJoinLeavePM();
int  GdialogWinUpdate();
int  GdialogWinMoveTop();
int  GdialogReplyWinMoveTop();
int  GdialogAddOptionByIdx( int Page, int TxtId, int Reaction );
int  GdialogAddOptionByStr( int a1, char *Text, int Reaction );
int  GdialogCreateReview( int *pWin );
int  GdialogCloseReview( int *pWin );
int  GdialogReview();
void GdialogReviewUpdate( int win, int pos );
void GdialogUnk27();
int  GdialogAddReplyToReviewIdx( Intp_t *scr, int MsgList, int MsgNum );
int  GdialogAddReplyToReviewStr( Intp_t *scr, char *Text );
int  GdialogAddOptionToReviewIdx( int MsgList, int MsgNum );
int  GdialogAddOptionToReviewStr( char *Text );
int  GdialogUnk30();
int  GdialogOptionsRemove();
int  GdialogDestroy();
void GdialogDisplayMoney();
int  GdialogMenu();
int  GdialogUnk35( int a1 );
void GdialogFocusOptionLine( int LineNo );
void GdialogLeaveOptionLine( int LineNo );
void GdialogDisplayReply();
void GdialogCreateDialog();
int  GdialogMakeWindow();
void GdialogCloseWindow();
void GdialogHeadUpdate( int HeadId, unsigned int Mood );
void GdialogUnk43();
void GdialogTransition( int a1 );
void GdialogUnk45();
void GdialogUnk46();
int  GdialogUnk47();
void GdialogReplyWinCls( int a1 );
void GdialogOptionWinCls( int a1 );
void GdialogRfshOptionRect( int win, VidRect_t *DrawRect );
void GdialogAnimationUpdate();
void GdialogReaction( int a1 );
int  GdialogPanelSlide( int a1, int a2, char *a3, char *a4, char *a5, int a6, int a7 );
int  GdialogGetLines( char *text, int width );
void GdialogUnk55( char *Surf, VidRect_t *Area, char *Text, int *pStrPos, int Height, int Pitch, int Color );
int  GdialogDisplayMsg( char *Surf, VidRect_t *Area, char *Text, int *pStrPos, int Height, int Pitch, int Color, int Flag );
int  GdialogSetBarterModifier( int percentage );
int  GdialogBarterMenu( int a1 );
void GdialogUnk59();
int  GdialogUnk60();
void GdialogPanelClose();
void GdialogUnk62();
int  GdialogCombatControlMenuCreate();
void GdialogCombatControlMenuClose();
void GdialogCombatControlAttributes();
void GdialogUnk66();
int  GdialogUnk67( Obj_t *a1 );
int  GdialogMenuTrade();
void GdialogCombatControlMenu();
int  GdialogCustomDispMenuCreate();
void GdialogCustomDispMenuClose();
void GdialogCustomDispMenuProcess();
void GdialogUpdateCustomDispMenu();
void GdialogDispositionOptions( char *a1, int a2, int a3, int a4 );
int  GdialogCustomDispMode( int a1 );
int  GdialogSetDisposition( int a1, int a2 );
void GdialogBarter();
int  GdialogCreateButtons();
void GdialogPanelCloseAnimation();
int  GdialogCreateWindow();
int  GdialogRenderReplyBg();
int  GdialogRenderA( VidRect_t *Area );
void GdialogBlit( char *pSrc, int SrcPitch, int SrcHeight, int DstWidth, char *pDst, int PosX, int PosY, int DstPitch, char *Shader );
void GdialogRenderScreen( ArtFrmHdr_t *Img, int FrameNo );
void GdialogCreateArts();
int  GdialogUnk86();



