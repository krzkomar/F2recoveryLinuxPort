#include "FrameWork.h"

int   gDlgInDialog = 0;
int   gDlgOptionCnt = 0;
int   gDlgReviewCount = 0;
char  *gDlgSurf = NULL;
int   gDlgBoxReply = -1;
int   gDlgBoxOption = -1;
int   gDlgUnk88 = 0;
int   gDlgIndicatorBoxState = 0;
int   gDlgFidgetId = 0;
CachePool_t *gDlgFidgetImgObj = NULL;
ArtFrmHdr_t *gDlgFidgetImg = NULL;
short gDlgBgndIdx = 2;
int   gDlgHeadFrameId = 0;
CachePool_t *gDlgHeadArtObj = NULL;
ArtFrmHdr_t *gDlgHeadFrameImg = NULL;
int   gDlgLipsEn = 0;
int   gDlgUnk09 = 0;
int   gDlgState = 0;
int   gDlgUnk07 = -1;
int   gDlgUnk13 = 0;
int   gDlgUnk18 = 0;
int   gDlgUnk17 = 0;
int   gDlgUnk16 = 0;
Obj_t *gDlgUnk15 = NULL;
Obj_t *gDlgUnk20 = NULL;
Obj_t *gDlgUnk21 = NULL;
int   gDlgBarterModifier = 0;
int   gDlgBackWindow = -1;
int   gDlgMainWin = -1;
VidRect_t gDlgUnk22[ 8 ] = { 
    { 126, 14,  152, 40 },
    { 488, 14,  514, 40 }, 
    { 126, 188, 152, 214 },
    { 488, 188, 514, 214 }, 
    { 152, 14,  488, 24 },
    { 152, 204, 488, 214 }, 
    { 126, 40,  136, 188 },
    { 504, 40,  514, 188 } 
};
int gDlgUnk26 = 1;
int gDlgFidgetAnimate = 0;
int gDlgReplyWin = -1;
int gDlgOptionWin = -1;
int gDlgUnk29 = -1;
int gDlgUnk12 = -1;
int gDlgUnk11 = -1;
char *gDlgPalA = NULL;
char *gDlgPalB = NULL;
int gDlgUnk10 = 0;
int gDlgFidgetDrawFactor = 0;
CachePool_t *gDlgArt[ 8 ] = { ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL };
char *gDlgUnk36 = NULL;
int gDlgUnk37[ 12 ] = { 35, 35, 82, 35, 37, 46, 89, 90, 87, 88, 91, 92 };
Obj_t *gDlgPartyMemberObj = NULL;
int gDlgPmExchangeMode = 0;
int gDlgHeadId = 0;
int gDlgActionSrc = -1;
int gDlgUnk48[ 42 ] = { 0, 3, 1, 1, 3, 1, 1, 1, 7, 8, 7, 3, 1, 8, 1, 7, 7, 6, 6, 2, 2, 2, 2, 4, 4, 5, 5, 2, 2, 2, 2, 2, 6, 2, 2, 5, 8, 2, 2, 2, 2, 8 };
int gDlgHeadFrame = 0;
int gDlgUnk50 = -1;
int gDlgFidgetFinishSpeed = 10000;
const char *gDlgUnk52[ 3 ] = { "Said Good", "Said Neutral", "Said Bad" };
int gDlgPanelHeight = 0;
Gdialog02_t gDlgCmbtCtrlButtons[ 5 ] = {
    { 0x1B6, 0x25, 0x18D, 0x18B, 0x18C, NULL, NULL, NULL, 2098, 4 },
    { 0x1B6, 0x43, 0x18A, 0x188, 0x189, NULL, NULL, NULL, 2103, 3 },
    { 0x1B6, 0x60, 0x196, 0x194, 0x195, NULL, NULL, NULL, 2102, 2 },
    { 0x1B6, 0x7E, 0x190, 0x18E, 0x18F, NULL, NULL, NULL, 2111, 1 },
    { 0x1B6, 0x9C, 0x193, 0x191, 0x192, NULL, NULL, NULL, 2099, 0 },
};

int gDlgTextIds[ 6 ][ 12 ][ 2 ] = {
    { { 100, 0 }, { 101, 1 }, { 102, 2 }, { 103, 3 }, { 104, 4 }, { -1,  0 } },
    { { 200, 0 }, { 201, 1 }, { 202, 2 }, { 203, 3 }, { 204, 4 }, { 205, 5 } },
    { { 300, 0 }, { 301, 1 }, { 302, 2 }, { 303, 3 }, { 304, 4 }, { 305, 5 } },
    { { 400, 0 }, { 401, 1 }, { 402, 2 }, { 403, 3 }, { 404, 4 }, { -1,  0 } },
    { { 500, 0 }, { 501, 1 }, { 502, 2 }, { 503, 3 }, { 504, 4 }, { -1,  0 } },
    { { 600, 0 }, { 601, 1 }, { 602, 2 }, { 603, 3 }, { 604, 4 }, { -1,  0 } }
};

Gdialog02_t gDlgCustomDispButtons[ 6 ] = {
    { 95, 9,   410, 409, -1, NULL, NULL, NULL, 0, 0 },
    { 96, 38,  416, 415, -1, NULL, NULL, NULL, 1, 0 }, 
    { 96, 68,  418, 417, -1, NULL, NULL, NULL, 2, 0 },
    { 96, 98,  414, 413, -1, NULL, NULL, NULL, 3, 0 },
    { 96, 127, 408, 407, -1, NULL, NULL, NULL, 4, 0 },
    { 96, 157, 412, 411, -1, NULL, NULL, NULL, 5, 0 }
};
int gDlgUnk79 = 0;
int gDlgDispositions[ 6 ];
Msg_t gDlgMsg;
char gDlgLumA[ 256 ];
char gDlgLumB[ 256 ];
void *gDlgUnk97[ 8 ];
VidRect_t gDlgBoxWinArea;
VidRect_t gDlgBoxTitleArea;
GdialogReview_t gDlgReviewSlots[ 80 ];
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
int gDlgImg100Width;
char *gDlgImgBtA0;
int gDlgImg101Width;
ArtFrmHdr_t *gDlgImg100Dat;
int gDlgImg101Height;
CachePool_t *gDlgUnk130;
CachePool_t *gDlgUnk131;
ArtFrmHdr_t *gDlgImg101Dat;
int gDlgFontSave;
int gDlgFidgetTimer;
int gDlgMood;
Intp_t *gDlgScript;
int gDlgReplyMsgList;
int gDlgReplyMsgNum;
int gDlgUnk139;
char gDlgReply[ 2700 ];

Gdialog_t gDlgOptions[ 30 ]; 
int gDlgUnk149;
int gDlgFidgetSpeed;
int gDlgFidgetFrame;

/****************************************************************/

int GdialogInit()
{
    return 0;
}

int GdialogClose()
{
    GdialogLipsyncEnd();
    return 0;
}

int GdialogInDialog()
{
    return gDlgInDialog != 0;
}

void GdialogEnter( Obj_t *target, int flg )
{
    MsgLine_t msg;
    Scpt_t *scr;
    int tmp;

    if( !target ){ eprintf( "\nError: gdialogEnter: target was NULL!" ); return; }
    gDlgUnk13 = 0;
    if( IN_COMBAT || target->ScrId == -1 ) return;
    if( (OBJTYPE( target->Pid ) != TYPE_ITEM) && (target->ScrId >> 24 ) != 1 ){
        tmp = ActionTargetAccessible( gObjDude, target );
        if( tmp == -1 ){
            msg.Id = 660; // 'You can't see there.'
            if( MessageGetMsg( &gProtoMessages, &msg ) != 1 ){ eprintf( "\nError: gdialog: Can't find message!" ); return; }
            if( flg )
                IfcMsgOut( msg.Text );
            else
            	eprintf( "%s", msg.Text );
            return;
        }
        if( tmp == -2 ){
            msg.Id = 661; // 'Too far away.'
            if( MessageGetMsg( &gProtoMessages, &msg ) != 1 ){ eprintf( "\nError: gdialog: Can't find message!" ); return; }
            if( flg )
                IfcMsgOut( msg.Text );
            else
            	eprintf( "%s", msg.Text );
            return;
        }
    }
    gDlgUnk12 = gTileCentIdx;
    gDlgBarterModifier = 0;
    gDlgUnk11 = gObjDude->GridId;
    MapAmbientDisable();
    gDlgInDialog = 1;
    gDlgPartyMemberObj = target;
    gDlgPmExchangeMode = PartyMembRdy( target );
    gDlgUnk10 = 1;
//SCP_DBG_EN;
    if( target->ScrId != -1 ) ScptRun( target->ScrId, SCPT_AEV_TALK_P_PROC );
//SCP_DBG_DIS;
    if( ScptPtr( target->ScrId, &scr ) == -1 ){
        GmouseIsoEnter();
        MapAmbientEnable();
        ScptMapUpdate();
        gDlgInDialog = 0;
        return;
    }
    if( scr->OverrideFlag || gDlgUnk09 != 4 ){
        gDlgUnk10 = 0;
        MapAmbientEnable();
        ScptMapUpdate();
        gDlgInDialog = 0;
        return;
    }
    GdialogLipsyncEnd();
    if( gDlgUnk07 == 1 ){
	if( gDlgState == 2 || gDlgState == 8 || gDlgState == 11 ){ GdialogPanelCloseAnimation(); } else // enter PM Trade
	if( gDlgState == 1 ){ GdialogPanelClose(); } else 
	if( gDlgUnk09 == 1 ){ GdialogPanelCloseAnimation(); } else 
	if( gDlgUnk09 == 4 ){ GdialogPanelClose(); }        
	GdialogUnk06();
    }
    gDlgUnk07 = 0;
    gDlgUnk09 = 0;
    if( gDlgUnk11 != gObjDude->GridId ) gDlgUnk12 = gObjDude->GridId;
    if( gDlgUnk13 ) TileUnk49( gDlgUnk12, 2 );
    MapAmbientEnable();
    ScptMapUpdate();
    gDlgInDialog = 0;
    return;    
}

int GdialogFloatingMessages()
{
    GlobVarFloatMsgDec();
    gDlgUnk18 = 1;
    SoundUpdateAll();
    GdialogEnter( gDlgPartyMemberObj, 0 );
    SoundUpdateAll();
    if( gDlgUnk11 != gObjDude->GridId ) gDlgUnk12 = gObjDude->GridId;
    if( gDlgUnk13 ) TileUnk49( gDlgUnk12, 2 );
    GlobVarFloatMsgInc( 2 );
    return GlobVarFloatMsgDec();
}

int GdialogLipsyncStart( char *DirName )
{
    char fname[ 16 ];

    if( !DirName ){ eprintf( "\nGDialog: Bleep!" ); return GSoundPlay( "censor" ); }
    if( ArtGetFilenameFromList( 8, gDlgHeadId & 0xFFF, fname ) == -1 ) return -1;    
    if( LipsyncLoadFile( DirName, fname ) == -1 ) return -1;
    gDlgLipsEn = 1;
    LipsyncSpeechStart();
    eprintf( "Starting lipsynch speech" );
    return 0;
}

void GdialogLipsyncEnd()
{
    if( !gDlgLipsEn ) return;
    eprintf( "Ending lipsynch system" );
    gDlgLipsEn = 0;
    LipsyncClose();
}

int GdialogTaskCb()
{
    InpTaskStart( GdialogAnimationUpdate );
    return 0;
}

int GdialogUnk04()
{
    InpTaskStop( GdialogAnimationUpdate );
    return 0;
}

int GdialogStartConversation( int HeadId, int Mood ) // dialog with Party Member
{
    if( gDlgUnk09 == 1 ) return -1;
    if( gDlgUnk07 == 1 ) return 0;
    AnimReset();    
    gDlgIndicatorBoxState = IfaceIndicatorBoxHide();
    gDlgPmExchangeMode = PartyMembRdy( gDlgPartyMemberObj );
    gDlgFontSave = FontGetCurrent();
    FontSet( 101 );
    DialogReplyWin( 135, 225, 58, 379, 0 );
    DialogSetReplyColor( 0.30000001, 0.30000001, 0.30000001 );
    DialogOptWin( 127, 335, 117, 393, 0 );
    DialogSetOptionColor( 0.2, 0.2, 0.2 );
    DialogSetTitle( 0 );
    DialogUnk34( (void *)GdialogReplyWinCls, (void *)GdialogOptionWinCls );
    GdialogCreateArts();
    CycleColorStop();
    if( gDlgUnk18 ) GmouseSetIfaceMode( 0 );
    GmouseUnk03();
    GmouseLoadCursor( 1 );
    TextFlush();
    if( OBJTYPE( gDlgPartyMemberObj->Pid ) ) TileUnk49( gDlgPartyMemberObj->GridId, 2 );
    gDlgUnk26 = 1;
    GdialogMakeWindow();
    InpTaskStart( GdialogAnimationUpdate );
    GdialogHeadUpdate( HeadId, Mood );
    gDlgUnk07 = 1;
    GmouseScrollDisable();
    if( HeadId == -1 ){ 
	gDlgUnk29 /= 2;
        GSoundSetMusicVol( gDlgUnk29 );        
    } else { // talking head
        gDlgUnk29 = -1;
        GSoundBgClose();
    }
    gDlgUnk13 = 1;
    return 0;
}

int GdialogUnk06()
{
    int Pid_high;

    if( gDlgState == 2 || gDlgState == 8 || gDlgState == 11 ) return -1;
    if( !gDlgUnk07 ) return 0;
    GdialogLipsyncEnd();
    GdialogUnk27();
    InpTaskStop( GdialogAnimationUpdate );
    Pid_high = OBJTYPE( gDlgPartyMemberObj->Pid );
    if( Pid_high ){
        if( gDlgUnk11 != gObjDude->GridId ) gDlgUnk12 = gObjDude->GridId;
        Pid_high = TileUnk49( gDlgUnk12, 2 );
    }
    GdialogCloseWindow( Pid_high );
    FontSet( gDlgFontSave );
    if( gDlgFidgetImg ){
        ArtClose( gDlgFidgetImgObj );
        gDlgFidgetImg = 0;
    }
    if( gDlgHeadArtObj ){
        if( ArtClose( gDlgHeadArtObj ) == -1 ) eprintf( "Failure unlocking lips frame!\n" );
        gDlgHeadArtObj = 0;
        gDlgHeadFrameImg = 0;
        gDlgHeadFrameId = 0;
    }
    PalExtClose( gPalColorCubeRGB[17][17][17] );
    PalExtClose( gPalColorCubeRGB[21][21][11] );
    ArtClose( gDlgUnk120 );
    ArtClose( gDlgUnk131 );
    gDlgUnk07 = 0;
    gDlgUnk09 = 0;
    CycleColorStart();
    if( !GameIfaceStat() ) GmouseScrollEnable();
    if( gDlgUnk29 == -1 ) GSoundRestartBg(11);
    else
        GSoundSetMusicVolume( gDlgUnk29 );
    if( gDlgIndicatorBoxState ) IfaceIndicatorBoxShow();
    gDlgIndicatorBoxState = 0;
    if( gDlgUnk18 ){
        if( !GameIfaceStat() ) GmouseSetIsoMode();
        gDlgUnk18 = 0;
    }
    if( !GameIfaceStat() ) GmouseIsoEnter();
    gDlgUnk13 = 1;
    return 0;
}

int GdialogSetBg( int BackgroundIdx )
{
    if( BackgroundIdx != -1 ) gDlgBgndIdx = BackgroundIdx;
    return 0;
}

void GdialogReply( char *str )
{
    int Position = 0;

    if( gDlgReplyWin == -1 ) eprintf( "\nError: Reply window doesn't exist!" );
    gDlgBoxTitleArea.lt = 5;
    gDlgBoxTitleArea.tp = 10;
    gDlgBoxTitleArea.bm = 58;
    gDlgBoxTitleArea.rt = 374;
    GdialogReplyWinCls( gDlgBoxReply );
    GdialogDisplayMsg( WinGetSurface( gDlgBoxReply ), &gDlgBoxTitleArea, str, &Position, gFont.ChrHeight(), 379, DLG_COLOR_5 | 0x2000000, 1 );
    WinMoveTop( gDlgReplyWin );
    WinUpdate( gDlgBoxReply );
}

int GdialogStart()
{
    gDlgReviewCount = 0;
    gDlgOptionCnt = 0;
    return 0;
}

void GdialogFinish()
{
    MseDrawCursor();
    GdialogEnd();
    gDlgOptionCnt = 0;
    gDlgReplyMsgList = -1;
}

int GdialogSayOptionConstNoProc( int MsgList, int MsgNum, int Reaction )
{
    gDlgOptions[ gDlgOptionCnt ].ProcedureTargetId = 0;
    return (GdialogAddOptionByIdx( MsgList, MsgNum, Reaction ) != -1) - 1;
}

int GdialogSayOptionStrNoProc( int MsgList, char *Text, int Reaction )
{
    gDlgOptions[ gDlgOptionCnt ].ProcedureTargetId = 0;
    return (GdialogAddOptionByStr( MsgList, Text, Reaction ) != -1) - 1;
}

int GdialogSayOptionConst( int MsgList, int MsgNum, int ProcedureTarget, int Reaction )
{
    gDlgOptions[ gDlgOptionCnt ].ProcedureTargetId = ProcedureTarget;
    return (GdialogAddOptionByIdx( MsgList, MsgNum, Reaction ) != -1) - 1;
}

int GdialogSayOptionStr( int MsgList, char *Text, int ProcedureTarget, int Reaction )
{
    gDlgOptions[ gDlgOptionCnt ].ProcedureTargetId = ProcedureTarget;
    return (GdialogAddOptionByStr( MsgList, Text, Reaction ) != -1) - 1;
}

int GdialogSayReplyConst( Intp_t *Script, int MsgList, int MsgNum )
{
    GdialogAddReplyToReviewIdx( Script, MsgList, MsgNum );
    gDlgScript = Script;
    gDlgReplyMsgList = MsgList;
    gDlgReplyMsgNum = MsgNum;
    gDlgUnk139 = 0;
    gDlgReply[ 0 ] = '\0';
    gDlgOptionCnt = 0;
    return 0;
}

int GdialogSayReply( Intp_t *Script, char *Text )
{
    GdialogAddReplyToReviewStr( Script, Text );
    gDlgScript = Script;
    gDlgUnk139 = 0;
    gDlgReplyMsgList = -4;
    gDlgReplyMsgNum = -4;
    strcpy( gDlgReply, Text );
    gDlgOptionCnt = 0;
    return 0;
}

int GdialogEnd()
{
    if( gDlgReplyMsgList == -1 ) return 0;
    if( gDlgOptionCnt < 1 ){
        gDlgOptions[ gDlgOptionCnt ].ProcedureTargetId = 0;
        if( GdialogAddOptionByIdx( -1, -1, 50 ) == -1 ){ 
    	    IntpError( "Error setting option." ); 
    	    gDlgOptionCnt = 0; 
    	    return -1;
    	}
    }
    return GdialogMenu();
}

void GdialogJoinLeavePM()
{
    int v0;
DD
    if( gDlgUnk09 != 1 ) return;
    v0 = PartyMembRdy( gDlgPartyMemberObj );
    if( v0 == gDlgPmExchangeMode ) return;
    if( gDlgReplyWin != -1 ) WinUpdateDirty( gDlgReplyWin );
    if( gDlgOptionWin != -1 ) WinUpdateDirty( gDlgOptionWin );
    GdialogPanelCloseAnimation();
    gDlgPmExchangeMode = v0;
    GdialogCreateButtons();
    if( gDlgReplyWin != -1 ) WinMoveTop( gDlgReplyWin );
    if( gDlgOptionWin != -1 ) WinMoveTop( gDlgOptionWin );
}

int GdialogWinUpdate()
{
    if( gDlgReplyWin != -1 ) WinUpdateDirty( gDlgReplyWin );
    if( gDlgOptionWin != -1 ) WinUpdateDirty( gDlgOptionWin );
    return 0;
}

int GdialogWinMoveTop()
{
    if( gDlgReplyWin != -1 ) WinMoveTop( gDlgReplyWin );
    if( gDlgOptionWin != -1 ) WinMoveTop( gDlgOptionWin );
    return 0;
}

int GdialogReplyWinMoveTop()
{
    if( gDlgReplyWin != -1 ) WinMoveTop( gDlgReplyWin );
    return 0;
}

int GdialogAddOptionByIdx( int MsgList, int MsgNum, int Reaction )
{
    if( gDlgOptionCnt >= 30 ){ eprintf( "\nError: dialog: Ran out of options!" ); return -1; }
    gDlgOptions[ gDlgOptionCnt ].Text[ 0 ] = '\0';
    gDlgOptions[ gDlgOptionCnt ].Button = -1;
    gDlgOptions[ gDlgOptionCnt ].MsgPage = MsgList;
    gDlgOptions[ gDlgOptionCnt ].MsgId = MsgNum;
    gDlgOptions[ gDlgOptionCnt ].Reaction = Reaction;
    gDlgOptionCnt++;
    return 0;
}

int GdialogAddOptionByStr( int a1, char *Text, int Reaction )
{
    if( gDlgOptionCnt >= 30 ){ eprintf( "\nError: dialog: Ran out of options!" ); return -1; }
    gDlgOptions[ gDlgOptionCnt ].MsgPage = -4;
    gDlgOptions[ gDlgOptionCnt ].MsgId = -4;
    sprintf( gDlgOptions[ gDlgOptionCnt ].Text, "%c %s", 149, Text );
    gDlgOptions[ gDlgOptionCnt ].Reaction = Reaction;
    gDlgOptions[ gDlgOptionCnt ].Button = -1;
    gDlgOptionCnt++;
    return 0;
}

int GdialogCreateReview( int *pWin )
{
    int i, bt, width;
    char *img, *imgs[ 6 ];

    if( gDlgLipsEn == 1 && SoundIsPlaying( gLipsync.Snd ) ) GdialogLipsyncEnd();
    gDlgUnk114 = FontGetCurrent();
    if( !pWin ) return -1;    
    width = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    if( (*pWin = WinCreateWindow( 0, 0, width, 480, 256, 0x14 )) == -1 ) return -1;    

    if( !(img = ArtGetBitmap( ArtMakeId( 6, 102, 0, 0, 0 ), 0, 0, &gDlgArt[ 6 ] ) ) ){ WinClose( *pWin ); *pWin = -1; return -1; }
    ScrCopy( img, width, 480, width, WinGetSurface( *pWin ), width );
    ArtClose( gDlgArt[ 6 ] );

    gDlgArt[ 6 ] = ART_NULL;
    for( i = 0; i < 6; i++ ){
        if( !(imgs[ i ] = ArtGetBitmap( ArtMakeId( 6, gDlgUnk37[ i + 6 ], 0, 0, 0 ), 0, 0, gDlgArt ) ) ) break;
    }
    if( i < 6 ){ GdialogCloseReview( pWin ); return -1; }    
    if( (bt = WinCreateButton( *pWin, 475, 152, gDlgUnk37[ 0 ], gDlgUnk37[ 3 ], -1, -1, -1, 328, imgs[ 0 ], imgs[ 1 ], 0, 32 ) ) == -1 ){ GdialogCloseReview( pWin ); return -1; }
    WinSetClickSound( bt, GSoundPlayCheck, GSoundPlayUnCheck);
    if( (bt = WinCreateButton( *pWin, 475, 191, gDlgUnk37[ 1 ], gDlgUnk37[ 4 ], -1, -1, -1, 336, imgs[ 2 ], imgs[ 3 ], 0, 32 ) ) == -1 ){ GdialogCloseReview( pWin ); return -1; }
    WinSetClickSound( bt, GSoundPlayCheck, GSoundPlayUnCheck);
    if( (bt = WinCreateButton( *pWin, 499, 398, gDlgUnk37[ 2 ], gDlgUnk37[ 5 ], -1, -1, -1, 27, imgs[ 4 ], imgs[ 5 ], 0, 32 ) ) == -1 ){ GdialogCloseReview( pWin ); return -1; }
    WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    FontSet( 101 );
    WinUpdate( *pWin );
    InpTaskStop( GdialogAnimationUpdate );
    if( !(gDlgUnk36 = ArtGetBitmap( ArtMakeId( 6, 102, 0, 0, 0 ), 0, 0, &gDlgArt[ 7 ] )) ){ GdialogCloseReview( pWin ); return -1; }
    return 0;
}

int GdialogCloseReview( int *pWin )
{
    int i;

    InpTaskStart( GdialogAnimationUpdate );
    for( i = 0; i != 6; i++ ){
        if( gDlgArt[ i ] != ART_NULL ){
            ArtClose( gDlgArt[ i ] );
            gDlgArt[ i ] = ART_NULL;
        }
    }
    if( gDlgArt[ 7 ] != ART_NULL ){
        ArtClose( gDlgArt[ 7 ] );
        gDlgArt[ 7 ] = ART_NULL;
        gDlgUnk36 = 0;
    }
    FontSet( gDlgUnk114 );
    if( !pWin ) return -1;
    WinClose( *pWin );
    *pWin = -1;
    return 0;
}

int GdialogReview()
{
    int pos, win, sel;

    pos = 0;
    if( GdialogCreateReview( &win ) == -1 ){ eprintf( "\nError initializing review window!" ); return -1; }
    GdialogReviewUpdate( win, 0 );
    do{
        switch( ( sel = InpUpdate() ) ){
    	    case 17: case 24: case 324:
    		SysQuitDlg();
    		break;		
    	    case 328:
        	if( --pos < 0 ) pos = 0;
    		GdialogReviewUpdate( win, pos ); 
    		break;
    	    case 336:
        	if( ++pos > (gDlgReviewCount - 1) ) pos = gDlgReviewCount - 1;
    		GdialogReviewUpdate( win, pos ); 
        	break;
        }	
    } while( sel != KEY_ESC && !gMenuEscape );
    if( GdialogCloseReview( &win ) == -1 ) return -1;
    return 0;    
}

void GdialogReviewUpdate( int win, int pos )
{
    VidRect_t area;
    int v4, v16, h, i;
    char *surf, *DialogA, *v13, stmp[60], *v20;

    v4 = 76;
    area.lt = 113;
    area.tp = 76;
    area.rt = 422;
    area.bm = 418;

    h = gFont.ChrHeight() + 2;
    v20 = WinGetSurface(win) + 113;
    if( !v20 ) eprintf( "\nError: gdialog: review: can't find buffer!" );
    v16 = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    surf = WinGetSurface( win );
    ScrCopy(
	(v16 * area.tp + area.lt + gDlgUnk36), 
	gVidMainGeo.rt - gVidMainGeo.lt + 1, 
	area.bm + 15 - area.tp, 
	gVidMainGeo.rt - gVidMainGeo.lt + 1, 
	&surf[ v16 * area.tp + area.lt ], v16
    );    
    for( i = pos; i < gDlgReviewCount; i++ ){
        sprintf( stmp, "%s:", ObjGetName( gDlgPartyMemberObj ) );
        WinDrawText( win, stmp, 180, 88, v4, DLG_COLOR_5 | 0x2000000 );
        area.tp += h;
        if( gDlgReviewSlots[ i ].ReplyMsgList <= -3 )
            DialogA = gDlgReviewSlots[ i ].ReplyText;
        else
            DialogA = ScptGetDialogA( gDlgReviewSlots[ i ].ReplyMsgList, gDlgReviewSlots[ i ].ReplyMsgNum );
        if( !DialogA ){ WinMsgError( "\nGDialog::Error Grabbing text message!" ); exit( 1 ); }
        v4 = GdialogDisplayMsg( v20, &area, DialogA, 0, gFont.ChrHeight(), 640, DLG_COLOR_6 | 0x2000000, 1 );
        if( gDlgReviewSlots[ i ].OptionMsgList != -3 ){
            sprintf( stmp, "%s:", ObjGetName( gObjDude ) );
            WinDrawText( win, stmp, 180, 88, v4, DLG_COLOR_4 | 0x2000000 );
            area.tp += h;
            if( gDlgReviewSlots[ i ].OptionMsgList <= -3 )
                v13 = gDlgReviewSlots[ i ].OptionText;
            else
                v13 = ScptGetDialogA( gDlgReviewSlots[ i ].OptionMsgList, gDlgReviewSlots[ i ].OptionMsgNum );
            if( !v13 ){ WinMsgError( "\nGDialog::Error Grabbing text message!" ); exit( 1 ); }
            v4 = GdialogDisplayMsg( v20, &area, v13, 0, gFont.ChrHeight(), 640, DLG_COLOR_7 | 0x2000000, 1 );
        }
        if( v4 >= 407 ) break;            
    }
    area.lt = 88;
    area.tp = 76;
    area.bm += 14;
    area.rt = 434;
    WinAreaUpdate( win, &area );
}

void GdialogUnk27()
{
    int i;

    for( i = 0; i < gDlgReviewCount; i++ ){
        gDlgReviewSlots[ i ].ReplyMsgNum = 0;
        gDlgReviewSlots[ i ].OptionMsgList = 0;
        gDlgReviewSlots[ i ].OptionMsgNum = 0;
        gDlgReviewSlots[ i ].ReplyMsgList = 0;
        if( gDlgReviewSlots[ i ].ReplyText ){ Free( gDlgReviewSlots[ i ].ReplyText ); gDlgReviewSlots[ i ].ReplyText = NULL; }
    }
    gDlgReviewCount = 0;
}

int GdialogAddReplyToReviewIdx( Intp_t *scr, int MsgList, int MsgNum )
{
    if( gDlgReviewCount >= 80 ){ eprintf( "\nError: Ran out of review slots!" ); return -1; }
    gDlgReviewSlots[ gDlgReviewCount ].ReplyMsgList = MsgList;
    gDlgReviewSlots[ gDlgReviewCount ].ReplyMsgNum = MsgNum;
    gDlgReviewSlots[ gDlgReviewCount ].OptionMsgList = -1;
    gDlgReviewSlots[ gDlgReviewCount ].OptionMsgNum = -1;
    gDlgReviewSlots[ gDlgReviewCount ].OptionMsgList = -3;
    gDlgReviewSlots[ gDlgReviewCount ].OptionMsgNum = -3;
    gDlgReviewCount++;
    return 0;
}

int GdialogAddReplyToReviewStr( Intp_t *a1, char *Text )
{
    if( gDlgReviewCount >= 80 ){ eprintf( "\nError: Ran out of review slots!" ); return -1; }
    gDlgReviewSlots[ gDlgReviewCount ].ReplyMsgList = -4;
    gDlgReviewSlots[ gDlgReviewCount ].ReplyMsgNum = -4;
    if( gDlgReviewSlots[ gDlgReviewCount ].ReplyText ) Free( gDlgReviewSlots[ gDlgReviewCount ].ReplyText );
    gDlgReviewSlots[ gDlgReviewCount ].ReplyText = Malloc( strlen( Text ) + 1 );
    strcpy( gDlgReviewSlots[ gDlgReviewCount ].ReplyText, Text );
    gDlgReviewSlots[ gDlgReviewCount ].OptionMsgList = -1;
    gDlgReviewSlots[ gDlgReviewCount ].OptionMsgNum = -1;
    gDlgReviewSlots[ gDlgReviewCount ].OptionText = NULL;
    gDlgReviewSlots[ gDlgReviewCount ].OptionMsgList = -3;
    gDlgReviewSlots[ gDlgReviewCount ].OptionMsgNum = -3;
    gDlgReviewCount++;
    return 0;
}

int GdialogAddOptionToReviewIdx( int MsgList, int MsgNum )
{
    int tmp;

    if( gDlgReviewCount >= 80 ){ eprintf( "\nError: Ran out of review slots!" ); return -1; }
    tmp = gDlgReviewCount - 1;
    gDlgReviewSlots[ tmp ].OptionMsgList = MsgList;
    gDlgReviewSlots[ tmp ].OptionMsgNum = MsgNum;
    gDlgReviewSlots[ tmp ].OptionText = NULL;
    return 0;
}

int GdialogAddOptionToReviewStr( char *Text )
{
    int n;
    char *s;

    if( gDlgReviewCount >= 80 ){ eprintf( "\nError: Ran out of review slots!" ); return -1; }
    n = gDlgReviewCount - 1;
    gDlgReviewSlots[ n ].OptionMsgList = -4;
    gDlgReviewSlots[ n ].OptionMsgNum = -4;
    s = Malloc( strlen( Text ) + 1 );
    gDlgReviewSlots[ n ].OptionText = s;
    strcpy( s, Text + 2 );
    return 0;
}

int GdialogUnk30()
{
    int bt;

    if( (gDlgBoxReply = WinCreateWindow( 135, 225, 379, 58, 256, 4 )) == -1 ) return -1;    
    if( (bt = WinCreateButton( gDlgBoxReply, 1, 1, 377, 28, -1, -1, 328, -1, 0, 0, 0, 32 ) ) != -1 ){
        WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
        WinSetButtonHandler( bt, GdialogUnk45, GdialogUnk47, 0, 0 );
        if( (bt = WinCreateButton( gDlgBoxReply, 1, 29, 377, 28, -1, -1, 336, -1, 0, 0, 0, 32 ) ) != -1 ){
            WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
            WinSetButtonHandler( bt, GdialogUnk46, GdialogUnk47, 0, 0 );
            if( (gDlgBoxOption = WinCreateWindow( 127, 335, 393, 117, 256, 4 ) ) != -1 ){
                gDlgImgBtA0 = ArtGetBitmap( ArtMakeId( 6, 96, 0, 0, 0 ), 0, 0, &gDlgUnk115 );
                if( gDlgImgBtA0 ){
                    if( (gDlgImgBtA1 = ArtGetBitmap( ArtMakeId( 6, 95, 0, 0, 0 ), 0, 0, &gDlgUnk130 ) ) ){
                        gDlgUnk149 = FontGetCurrent();
                        FontSet( 101 );
                        return 0;
                    }
                }
                ArtClose( gDlgUnk115 );
                gDlgUnk115 = ART_NULL;
            }
            WinClose( gDlgBoxOption );
            gDlgBoxOption = -1;
        }
    }
    WinClose( gDlgBoxReply );
    gDlgBoxReply = -1;    
    return -1;
}

int GdialogOptionsRemove()
{
    int i;
    
    for( i = 0; i < gDlgOptionCnt; i++ ){
        if( gDlgOptions[ i ].Button == -1 ) continue;
        WinImageRemove( gDlgOptions[ i ].Button );
        gDlgOptions[ i ].Button = -1;
    }
    return 0;
}

int GdialogDestroy()
{
    GdialogOptionsRemove();
    ArtClose( gDlgUnk130 );
    gDlgUnk130 = 0;
    gDlgImgBtA1 = 0;
    ArtClose( gDlgUnk115 );
    gDlgUnk115 = 0;
    gDlgImgBtA0 = 0;
    WinClose( gDlgBoxReply );
    gDlgBoxReply = -1;
    WinClose( gDlgBoxOption );
    gDlgBoxOption = -1;
    FontSet( gDlgUnk149 );
    return 0;
}

void GdialogDisplayMoney()
{
    int font, MoneyAll, w;
    char stmp[20];
    VidRect_t Area;

    Area.lt = 5;
    Area.rt = 70;
    Area.tp = 36;
    Area.bm = gFont.ChrHeight() + 36;
    GdialogRenderA( &Area );
    font = FontGetCurrent();
    FontSet( 101 );
    MoneyAll = ItemGetMoneyAll( gObjDude );
    sprintf( stmp, "$%d", MoneyAll );
    w = gFont.LineWidth( stmp );
    if( w > 60 ) w = 60;
    WinDrawText( gDlgMainWin, stmp, w, 38 - w / 2, 36, DLG_COLOR_5 | 0x7000000 );
    FontSet( font );
}

int GdialogMenu() // dialog with NPC
{
    int v0,v1,sel,v7,Time,Option[11],SysTime,v19;

    v0 = 0;
    v1 = 0;
    v19 = 0;
    Option[10] = 10000;
    if( !gDlgUnk17 ){
        if( GdialogUnk30() == -1 ) return -1;
    }
    Option[0] = 0;
    gDlgUnk17++;
    GdialogCreateDialog();
    if( gDlgUnk139 ){ v19 = 1; gDlgUnk16 = 1; }
    SysTime = TimerGetSysTime();
    while( 1 ){
        sel = InpUpdate();
        if( sel == 17 || sel == 24 || sel == 324 ) SysQuitDlg();
        if( gMenuEscape ) break;
        if( sel == 2 && !MseCursorCenterInArea( 135, 225, 514, 283 ) ){
            if( GmouseGetCursorId() != 1 ) GmouseLoadCursor( 1 );
        } else {
    	    switch( gDlgState ){
        	case 3: // PM Trade
            	    gDlgUnk09 = 4;
            	    InvMenuBarter( gDlgMainWin, gDlgPartyMemberObj, gDlgUnk20, gDlgUnk15, gDlgBarterModifier );
            	    GdialogUnk62();
            	    v7 = gDlgUnk09;
            	    GdialogPanelClose();
            	    gDlgUnk09 = v7;
            	    if( gDlgUnk09 == 4 ){ gDlgState = 1; gDlgUnk09 = 1; }
            	    continue;
        	case 9: // PM Combat Control
            	    gDlgUnk09 = 10;
            	    GdialogCombatControlMenu();
            	    GdialogCombatControlMenuClose();
            	    continue;
        	case 12: // PM Custum disposition
            	    gDlgUnk09 = 13;
            	    GdialogCustomDispMenuProcess();
            	    GdialogCustomDispMenuClose();
            	    continue;
        	case 98: // PM barter
        	    GdialogBarter(); 
        	    break;
    	    }
        }
        if( gDlgUnk16 ){
            Time = TimerGetTime();
            if( v19 ){
                if( TimerDiff( Time, SysTime ) >= Option[10] || sel == ' ' ){
                    v1++;
                    Option[ ++v0 ] = gDlgUnk139;
                    GdialogDisplayReply();
                    SysTime = Time;
                    if( !gDlgUnk139 ) v19 = gDlgUnk139;
                }
            }
            if( sel == 328 ){
                if( v1 > 0 ){
                    v19 = 0;
                    gDlgUnk139 = Option[ --v1 ];
                    GdialogDisplayReply();
                }
            } else if( sel == 336 ){
                if( v1 < v0 ){
                    v19 = 0;
                    gDlgUnk139 = Option[ ++v1 ];
                    GdialogDisplayReply();
                }
                if( gDlgUnk139 ){
                    SysTime = Time;
                    v1++;
                    v19 = 0;
                    Option[ ++v0 ] = gDlgUnk139;
                    GdialogDisplayReply();
                }
            }
        }
        if( sel == -1 ) continue;
        if( sel >= 1200 && sel <= 1250 ){ GdialogFocusOptionLine( sel - 1200 ); continue; }
        if( sel >= 1300 && sel <= 1330 ){ GdialogLeaveOptionLine( sel - 1300 ); continue; }
        if( sel >= '0' && sel <= '9' ){
            if( (sel - '1') >= gDlgOptionCnt ) continue;
            v0 = 0;
            v1 = 0;
            Option[0] = 0;
            gDlgUnk16 = 0;
            if( GdialogUnk35( sel - '1' ) == -1 ) break; // finish dialog
            SysTime = TimerGetSysTime();
            if( gDlgUnk139 ){
                v19 = 1;
                gDlgUnk16 = 1;
            } else {
                v19 = 0;
            }                    
        }
    }
    if( --gDlgUnk17 ) return 0;
    if( GdialogDestroy() == -1 ) return -1;
    return 0;    
}

int GdialogUnk35( int a1 )
{
    int reaction;

    MseCursorRedraw();
    GdialogOptionsRemove();
    if( gDlgOptions[ a1 ].MsgPage == -4 )
        GdialogAddOptionToReviewStr( gDlgOptions[ a1 ].Text );
    else
        GdialogAddOptionToReviewIdx( gDlgOptions[ a1 ].MsgPage, gDlgOptions[ a1 ].MsgId );
    gDlgFidgetAnimate = 0;
    GdialogLipsyncEnd();

    reaction = 0;
    switch( gDlgOptions[ a1 ].Reaction ){
	case 49: reaction = -1; break;
	case 50: reaction = 0; break;
	case 51: reaction = 1; break;
	default: eprintf( "\nError: dialog: Empathy Perk: invalid reaction!" ); break;
    }
    GdialogReplyWinCls( gDlgBoxReply );
    GdialogOptionWinCls( gDlgBoxOption );
    WinUpdate( gDlgBoxReply );
    WinUpdate( gDlgBoxOption );
    GdialogFocusOptionLine( a1 );
    GdialogReaction( reaction );
    gDlgOptionCnt = 0;
    if( gDlgUnk17 < 2 ){
        if( gDlgOptions[ a1 ].ProcedureTargetId ) SciRunProcedure( gDlgScript, gDlgOptions[ a1 ].ProcedureTargetId );
    }
    MseDrawCursor();
    if( !gDlgOptionCnt ) return -1;
    GdialogCreateDialog();
    return 0;
}

void GdialogFocusOptionLine( int LineNo )
{
    Gdialog_t *dlg;
    char color;

    dlg = &gDlgOptions[ LineNo ];
    if( !dlg->Button ) return;    
    gDlgBoxWinArea.tp = dlg->Top;
    gDlgBoxWinArea.lt = 0;
    gDlgBoxWinArea.bm = dlg->Bottom;
    gDlgBoxWinArea.rt = 391;
    GdialogRfshOptionRect( gDlgBoxOption, &gDlgBoxWinArea);
    gDlgBoxWinArea.lt = 5;
    gDlgBoxWinArea.rt = 388;
    color = gPalColorCubeRGB[31][31][11];
    if( PerkLvl( gObjDude, PERK_EMPATHY ) ){
        color = gPalColorCubeRGB[31][31][11];
        switch( dlg->Reaction ){
	    case 49: color = gPalColorCubeRGB[31][0][31]; break;
    	    case 50: break;
	    case 51: color = gPalColorCubeRGB[31][10][10]; break;
    	    default: eprintf( "\nError: dialog: Empathy Perk: invalid reaction!" ); break;
        }
    }
    GdialogDisplayMsg( WinGetSurface( gDlgBoxOption ), &gDlgBoxWinArea, dlg->Text, 0, gFont.ChrHeight(), 393, color, 1 );
    gDlgBoxWinArea.lt = 0;
    gDlgBoxWinArea.rt = 391;
    gDlgBoxWinArea.tp = dlg->Top;
    WinAreaUpdate( gDlgBoxOption, &gDlgBoxWinArea );
}

void GdialogLeaveOptionLine( int LineNo )
{
    Gdialog_t *p;
    int color;

    p = &gDlgOptions[ LineNo ];
    gDlgBoxWinArea.lt = 0;
    gDlgBoxWinArea.tp = p->Top;
    gDlgBoxWinArea.bm = p->Bottom;
    gDlgBoxWinArea.rt = 391;
    GdialogRfshOptionRect( gDlgBoxOption, &gDlgBoxWinArea );

    color = DLG_COLOR_5 | 0x2000000;
    if( PerkLvl( gObjDude, PERK_EMPATHY ) ){
	switch( p->Reaction ){
	    case 49: color = DLG_COLOR_8 | 0x2000000; break;
	    case 50: color = DLG_COLOR_5 | 0x2000000; break;
	    case 51: color = DLG_COLOR_9 | 0x2000000; break;
	    default: eprintf( "\nError: dialog: Empathy Perk: invalid reaction!" ); break;
	}
    }
    gDlgBoxWinArea.lt = 5;
    gDlgBoxWinArea.rt = 388;
    GdialogDisplayMsg( WinGetSurface( gDlgBoxOption ), &gDlgBoxWinArea, p->Text, NULL, gFont.ChrHeight(), 393, color, 1 );
    gDlgBoxWinArea.rt = 391;
    gDlgBoxWinArea.tp = p->Top;
    gDlgBoxWinArea.lt = 0;
    WinAreaUpdate( gDlgBoxOption, &gDlgBoxWinArea );
}

void GdialogDisplayReply()
{
    gDlgBoxTitleArea.lt = 5;
    gDlgBoxTitleArea.tp = 10;
    gDlgBoxTitleArea.rt = 374;
    gDlgBoxTitleArea.bm = 58;
    if( PerkLvl( gObjDude, PERK_EMPATHY ) ); // not implemented
    GdialogReplyWinCls( gDlgBoxReply );
    GdialogDisplayMsg( WinGetSurface( gDlgBoxReply ), &gDlgBoxTitleArea, gDlgReply, &gDlgUnk139, gFont.ChrHeight(), 379, DLG_COLOR_5 | 0x2000000, 1 );
    WinUpdate( gDlgBoxReply );
}

void GdialogCreateDialog()
{
    Gdialog_t *p;
    MsgLine_t msg;
    char *Dialog,*Text, *s, *Surface;
    int bm,Ypos,w,PerkEmpathy,LineErr,MsgColor,i;

    gDlgBoxTitleArea.lt = 5;
    gDlgBoxTitleArea.tp = 10;
    gDlgBoxTitleArea.rt = 374;
    gDlgBoxTitleArea.bm = 58;
    gDlgBoxWinArea.lt = 5;
    gDlgBoxWinArea.tp = 5;
    gDlgBoxWinArea.rt = 388;
    gDlgBoxWinArea.bm = 112;
    GdialogReplyWinCls( gDlgBoxReply ); // reply window
    GdialogOptionWinCls( gDlgBoxOption );
    LineErr = PerkEmpathy = 0;
    if( gDlgReplyMsgList > 0 ){
        Dialog = ScptGetDialog( gDlgReplyMsgList, gDlgReplyMsgNum, 1 );
        if( !Dialog ){ WinMsgError( "\nGDialog::Error Grabbing text message!" ); exit( 1 ); }
        strncpy( gDlgReply, Dialog, 899 );
        gDlgReply[ 899 ] = '\0';
    }
    GdialogDisplayReply();
    MsgColor = DLG_COLOR_5 | 0x2000000;
    if( PerkLvl( gObjDude, PERK_EMPATHY ) ) PerkEmpathy = 1;
    w = gDlgBoxWinArea.rt - gDlgBoxWinArea.lt - 4;    
    if( gDlgOptionCnt > 0 ){        
        for( i = 0; i < gDlgOptionCnt; i++ ){
    	    p = &gDlgOptions[ i ];
            if( PerkEmpathy ){
        	switch( p->Reaction ){
        	    case 49: MsgColor = DLG_COLOR_8 | 0x2000000; break;
        	    case 50: MsgColor = DLG_COLOR_5 | 0x2000000; break;
        	    case 51: MsgColor = DLG_COLOR_9 | 0x2000000; break;
        	    default: eprintf( "\nError: dialog: Empathy Perk: invalid reaction!" );
        	}
            }   
            if( p->MsgPage < 0 ){
                if( p->MsgPage == -1 ){
                    if( i == 0 ){
                        msg.Id = 655; // 'Go on'
                        if( FeatGetVal( gObjDude, FEAT_INTELLIGENCE) < 4 ) msg.Id++; // 'Err...'
                        if( MessageGetMsg( &gProtoMessages, &msg ) != 1 ){ eprintf( "\nError...can't find message!" ); return; }
                        Text = msg.Text;
                    } else {
                        Text = " ";
                    }
                    strcpy( p->Text, Text );
                } else if( p->MsgPage == -2 ){
                    msg.Id = 650; // '[Done]'
                    if( MessageGetMsg( &gProtoMessages, &msg ) != 1 ){ eprintf( "\nError...can't find message!" ); return; }
                    sprintf( p->Text, "%c %s", 149, msg.Text );
                }
            } else {
                if( !(s = ScptGetDialog( p->MsgPage, p->MsgId, 0 ) ) ){ WinMsgError( "\nGDialog::Error Grabbing text message!" ); exit( 1 ); }
                sprintf( p->Text, "%c ", 149 );
                strncat( p->Text, s, 897 );
            }
            bm = GdialogGetLines( p->Text, gDlgBoxWinArea.rt - gDlgBoxWinArea.lt) * gFont.ChrHeight() + gDlgBoxWinArea.tp + 2;
            if( bm >= gDlgBoxWinArea.bm ){
                if( LineErr )
                    eprintf( "Error: couldn't make button because it went below the window.\n" );
                else
                    LineErr = 1;
            } else {
                Ypos = gDlgBoxWinArea.tp;
                p->Bottom = bm;
                p->Top = Ypos;
                if( !i ) Ypos = 0;
                Surface = WinGetSurface( gDlgBoxOption );
                GdialogDisplayMsg( Surface, &gDlgBoxWinArea, p->Text, NULL, gFont.ChrHeight(), 393, MsgColor, 1 );
                gDlgBoxWinArea.tp += 2;
                if( (p->Button = WinCreateButton( gDlgBoxOption, 2, Ypos, w, gDlgBoxWinArea.tp - Ypos - 4, 1200 + i,  1300 + i, -1, '1' + i, 0, 0, 0, 0 )) == -1 )
                    eprintf( "\nError: Can't create button!" );
                else
                    WinSetClickSound( p->Button, GSoundPlayPushBt, GSoundPlayReleaseBt );
            }            
        }
    }
    GdialogDisplayMoney();
    WinUpdate( gDlgBoxReply );
    WinUpdate( gDlgBoxOption );
}

int GdialogMakeWindow()
{
    int i, n, w;
    char *surf;

    gDlgUnk09 = 1;    
    gDlgBackWindow = WinCreateWindow( 0, 0, gVidMainGeo.rt - gVidMainGeo.lt + 1, 480, 256, 2 );
    GdialogRenderReplyBg(); // upper dialog iface background
    surf = WinGetSurface( gDlgBackWindow );
    for( i = 0; i < 8; i++){
        SoundUpdateAll();
        if( !( gDlgUnk97[ i ] = Malloc( ( gDlgUnk22[ i ].bm - gDlgUnk22[ i ].tp ) * ( gDlgUnk22[ i ].rt - gDlgUnk22[ i ].lt ) ) ) ) return -1;
        w = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        n = gDlgUnk22[ i ].rt - gDlgUnk22[ i ].lt;
        ScrCopy( &surf[ gDlgUnk22[ i ].tp * w + gDlgUnk22[ i ].lt ], n, gDlgUnk22[ i ].bm - gDlgUnk22[ i ].tp, w, gDlgUnk97[ i ], n ); // 'screen' frame
    }
    GdialogCreateButtons();
    surf = WinGetSurface( gDlgBackWindow ) + 14 * ( gVidMainGeo.rt - gVidMainGeo.lt + 1 );
    gDlgSurf = surf + 126;
    if( gDlgSurf == NULL ) GdialogCloseWindow();
    return 0;
}

void GdialogCloseWindow()
{
    int i;

    if( gDlgMainWin != -1 ) gDlgSurf = 0;
    if( gDlgUnk09 == 1 ){
        GdialogPanelCloseAnimation();
    } else if( gDlgUnk09 == 4 ){
        GdialogPanelClose();
    }
    if( gDlgBackWindow != -1 ){
        WinClose(gDlgBackWindow);
        gDlgBackWindow = -1;
    }
    for( i = 0; i != 8; i++ ) Free( gDlgUnk97[ i ] );
}

void GdialogHeadUpdate( int HeadId, unsigned int Mood )
{
    int Frame, HeadsInfo, Random;
    unsigned int fidget;
    char stmp[ 200 ];

    Frame = 10;
    gDlgFidgetFrame = 0;
    if( HeadId == -1 ){
        gDlgFidgetId = -1;
        gDlgFidgetImg = 0;
        gDlgFidgetImgObj = ART_NULL;
        gDlgMood = -1;
        gDlgFidgetSpeed = 0;
        gDlgFidgetTimer = 0;
        GdialogRenderScreen( 0, 0 );
        gDlgHeadFrameId = 0;
        gDlgHeadArtObj = NULL;
        gDlgHeadFrameImg = NULL;
        return;
    }

    if( Mood == 1 ) Frame = 9;
    if( Mood == 7 ) Frame = 11;

    if( gDlgHeadFrameId && (Frame != gDlgHeadFrame) ){ // last frame ?
        if( ArtClose( gDlgHeadArtObj ) == -1 ) eprintf( "failure unlocking lips frame!\n" );
        gDlgHeadArtObj = NULL;
        gDlgHeadFrameImg = NULL;
        gDlgHeadFrameId = 0;
    }
    if( gDlgHeadFrameId == 0 ){ //
        gDlgHeadFrame = Frame;
        gDlgHeadFrameId = ArtMakeId( 8, HeadId, Frame, 0, 0 );
        gDlgHeadFrameImg = ArtLoadImg( gDlgHeadFrameId, &gDlgHeadArtObj );
        if( !gDlgHeadFrameImg ){
            eprintf( "failure!\n" );
            CacheStats( &gArtCacheDb, stmp );
            eprintf( "%s", stmp );
        }
    }
    if( !(HeadsInfo = ArtGetHeadsInfo( ArtMakeId( 8, HeadId, Mood, 0, 0 ) ) ) ){
	eprintf( "\tError - No available fidgets for given frame id\n" );
        return;
    }
    Random = RandMinMax( 1, 100 ) + gDlgFidgetDrawFactor / 2;
    fidget = HeadsInfo;
    if( HeadsInfo < 2 ){
        if( HeadsInfo == 0 ){
	    if( Random < 52 )
    		fidget = 1;
	    else 
		fidget = ( Random < 77 ) ? 2 : 3;
	}
    } else {
        if( HeadsInfo == 2 ){
            fidget = ( Random < 68 ) ? 1 : 2;
        } else {
    	    if( HeadsInfo == 3 ) gDlgFidgetDrawFactor = 0;
	    if( Random < 52 )
    		fidget = 1;
	    else 
		fidget = ( Random < 77 ) ? 2 : 3;
	}
    }
    eprintf( "Choosing fidget %d out of %d\n", fidget, HeadsInfo );
    if( gDlgFidgetImg && ArtClose( gDlgFidgetImgObj ) == -1 ) eprintf( "failure!\n" );
    gDlgFidgetId = ArtMakeId( 8, HeadId, Mood, fidget, 0 );
    gDlgFidgetFrame = 0;
    gDlgFidgetImg = ArtLoadImg( gDlgFidgetId, &gDlgFidgetImgObj );
    if( !gDlgFidgetImg ){
        eprintf( "failure!\n" );
        CacheStats( &gArtCacheDb, stmp );
        eprintf( "%s", stmp );
    }
    gDlgFidgetTimer = 0;
    gDlgMood = Mood;
    gDlgFidgetSpeed = 1000 / ArtGetObjSpeed( gDlgFidgetImg );
}

void GdialogUnk43()
{
    if( gDlgFidgetImg && gDlgMainWin != -1 ){
        eprintf("Waiting for fidget to complete...\n");
        while( ArtGetFpd( gDlgFidgetImg ) > gDlgFidgetFrame ){
            if( TimerCurrDiff( gDlgFidgetTimer ) >= gDlgFidgetSpeed ){
                GdialogRenderScreen( gDlgFidgetImg, gDlgFidgetFrame );
                gDlgFidgetTimer = TimerGetSysTime();
                gDlgFidgetFrame++;
            }
        }
        gDlgFidgetFrame = 0;
    }
}

void GdialogTransition( int a1 )
{
    CachePool_t *ImgObj;
    ArtFrmHdr_t *Img;
    unsigned int v7, v9;
    int v8;

    if ( gDlgFidgetImg && gDlgMainWin == -1 ) return;
    MseCursorRedraw();
    eprintf( "Starting transition...\n" );
    GdialogUnk43();
    if( gDlgFidgetImg ){
        if( ArtClose( gDlgFidgetImgObj ) == -1 ) eprintf( "\tError unlocking fidget in transition func..." );
        gDlgFidgetImg = 0;
    }
    if( !(Img = ArtLoadImg( ArtMakeId( 8, gDlgHeadId, a1, 0, 0 ), &ImgObj ) ) ) eprintf( "\tError locking transition...\n" );
    v7 = 1000 / ArtGetObjSpeed( Img );  
    v9 = 0;  
    for( v8 = 0; v8 < ArtGetFpd( Img ); v8++ ){
        if( TimerCurrDiff( v9 ) >= v7 ){
            GdialogRenderScreen( Img, v8 );
            v9 = TimerGetSysTime();            
        }
    }
    if( ArtClose( ImgObj ) == -1 ) eprintf( "\tError unlocking transition...\n" );
    eprintf( "Finished transition...\n" );
    MseDrawCursor();
}

void GdialogUnk45()
{
    if( !gDlgUnk16 ) return;
    GmouseLoadCursor( 2 );
}

void GdialogUnk46()
{
    if( !gDlgUnk16 ) return;
    GmouseLoadCursor( 3 );
}

int GdialogUnk47()
{
    return GmouseLoadCursor( 1 );
}

void GdialogReplyWinCls( int Win )
{
    int Width,w;
    char *Surface;

    gDlgReplyWin = Win;
    if( Win == -1 ){ eprintf( "\nError: demo_copy_title: win invalid!" ); return; }
    Width = WinGetWidth( Win );
    if( Width < 1 ){ eprintf("\nError: demo_copy_title: width invalid!"); return; }
    if( (w = WinGetHeight( Win )) < 1 ){ eprintf( "\nError: demo_copy_title: length invalid!" ); return; }
    if( gDlgBackWindow == -1 ){ eprintf( "\nError: demo_copy_title: dialogueBackWindow wasn't created!" ); return; }
    Surface = WinGetSurface( gDlgBackWindow );
    if( !Surface ){ eprintf( "\nError: demo_copy_title: couldn't get buffer!" ); return; }
    ScrCopy( Surface + 144135, Width, w, 640, WinGetSurface( Win ), Width );    
}

void GdialogOptionWinCls( int Win ) // cls option area
{
    int Height, w;
    char *src;
    VidRect_t Area;

    gDlgOptionWin = Win;
    if( Win == -1 ){ eprintf( "\nError: demo_copy_options: win invalid!" ); return; }
    if( ( w = WinGetWidth( Win ) ) < 1 ){ eprintf( "\nError: demo_copy_options: width invalid!" ); return; }
    Height = WinGetHeight( Win );
    if( Height < 1 ){ eprintf("\nError: demo_copy_options: length invalid!"); return; }
    if( gDlgBackWindow == -1 ){ eprintf( "\nError: demo_copy_options: dialogueBackWindow wasn't created!"); return; }
    WinGetRect( gDlgMainWin, &Area );
    if( !(src = WinGetSurface( gDlgMainWin )) ){ eprintf("\nError: demo_copy_options: couldn't get buffer!"); return; }
    ScrCopy( &src[ 640 * ( 335 - Area.tp ) + 127 ], w, Height, 640, WinGetSurface( Win ), w );
}

void GdialogRfshOptionRect( int win, VidRect_t *DrawRect )
{
    VidRect_t Area;
    char *Surface;
    int Width;

    if( !DrawRect ){ eprintf( "\nError: gDialogRefreshOptionsRect: drawRect NULL!" ); return; }
    if( win == -1 ){ eprintf("\nError: gDialogRefreshOptionsRect: win invalid!"); return; }
    if( gDlgBackWindow == -1 ){ eprintf("\nError: gDialogRefreshOptionsRect: dialogueBackWindow wasn't created!"); return; }
    WinGetRect( gDlgMainWin, &Area );
    Surface = WinGetSurface( gDlgMainWin );
    if( !Surface ){ eprintf( "\nError: gDialogRefreshOptionsRect: couldn't get buffer!" );  return; }
    if( DrawRect->tp >= DrawRect->bm ) { eprintf( "\nError: gDialogRefreshOptionsRect: Invalid Rect (too many options)!" ); return; }
    if( DrawRect->lt >= DrawRect->rt ){ eprintf( "\nError: gDialogRefreshOptionsRect: Invalid Rect (too many options)!" ); return; }
    Width = WinGetWidth( win );
    ScrCopy( 
        &Surface[640 * (335 - Area.tp) + 127] + 640 * DrawRect->tp + DrawRect->lt, 
        DrawRect->rt - DrawRect->lt, DrawRect->bm - DrawRect->tp, 
        640, WinGetSurface( win ) + Width * DrawRect->tp + DrawRect->lt, Width 
    );
}

void GdialogAnimationUpdate()
{
    switch( gDlgState ){
        case 2:
            gDlgUnk50 = -1; 
            gDlgState = 3; 
            GdialogPanelCloseAnimation(); 
            GdialogUnk60(); 
            break;
        case 1:
            gDlgUnk50 = -1;
            gDlgState = 0;
            GdialogPanelClose();
            GdialogCreateButtons();
            if( gDlgReplyWin != -1 ) WinMoveTop( gDlgReplyWin );
            if( gDlgOptionWin != -1 ) WinMoveTop( gDlgOptionWin );
            break;
        case 8:
            gDlgUnk50 = -1;
            gDlgState = 9;
            GdialogPanelCloseAnimation();
            GdialogCombatControlMenuCreate();
            break;
        case 11:
            gDlgUnk50 = -1;
            gDlgState = 12;
            GdialogPanelCloseAnimation();
            GdialogCustomDispMenuCreate();
            break;
    }
    if( gDlgFidgetImg == NULL ) return;
    if( gDlgLipsEn == 1 ){
        LipsyncSpeechUpdate();
        if( gLipsNewPhonemeFlag ){ // update head Art
            GdialogRenderScreen( gDlgHeadFrameImg, gDlgUnk48[ gLipsPhoneme ] );
            gLipsNewPhonemeFlag = 0;
        }
        if( !SoundIsPlaying( gLipsync.Snd ) ){
            GdialogLipsyncEnd();
            GdialogRenderScreen( gDlgHeadFrameImg, 0 );
            gDlgFidgetAnimate = 1; // set head to it default state of animation (fidget ending frame)
            gDlgFidgetDrawFactor = 3;
            gDlgFidgetFrame = 0;
        }
        return;
    }
    if( gDlgFidgetAnimate ){ // finish fidget animation
        if( TimerCurrDiff( gDlgFidgetTimer ) >= gDlgFidgetFinishSpeed ){
            gDlgFidgetAnimate = 0;
            gDlgFidgetDrawFactor += gDlgFidgetFinishSpeed / 1000;
            gDlgFidgetFinishSpeed = 1000 * (RandMinMax(0, 3) + 4);
            GdialogHeadUpdate( gDlgFidgetId & 0xFFF, (gDlgFidgetId & 0xFF0000) >> 16 );
        }
        return;
    }
    if( TimerCurrDiff( gDlgFidgetTimer ) >= gDlgFidgetSpeed ){ // fidget animation
        if( ArtGetFpd( gDlgFidgetImg ) <= gDlgFidgetFrame ){
            GdialogRenderScreen( gDlgFidgetImg, 0 );
            gDlgFidgetAnimate = 1;
        } else {
            GdialogRenderScreen( gDlgFidgetImg, gDlgFidgetFrame );
            gDlgFidgetTimer = TimerGetSysTime();
            gDlgFidgetFrame++;
        }
    }
}

void GdialogReaction( int a1 )
{
    int v4, v6;

    eprintf( "Dialogue Reaction: " );
    if( a1+1 < 3 ) eprintf( "%s\n", gDlgUnk52[ a1 ] );
    gDlgFidgetDrawFactor = 0;
    if( a1 >= -50 ){
        if( a1 != -1 ) return;
        if( gDlgMood < 4 ){
            if( gDlgMood != 1 ) return;
            v4 = 0;
            v6 = 1;
        } else if( gDlgMood <= 4 ){
            v4 = 3;
            v6 = 1;
        } else {
            if( gDlgMood != 7 ) return;
            v4 = 6;
            v6 = 4;
        }
    } else {
	if( a1 >= -50 || a1 == 0 || a1 != 1 ) return;    
	if( gDlgMood < 4 ){
            if( gDlgMood == 1 ){
                GdialogTransition( 2 );
                GdialogHeadUpdate( gDlgHeadId, 4 );
            }
            return;
        }
        if( gDlgMood <= 4 ){
            v4 = 5;
        } else {
            if( gDlgMood != 7 ) return;
            v4 = 8;
        }
        v6 = 7;
    }
    GdialogTransition( v4 );
    GdialogHeadUpdate( gDlgHeadId, v6 );    
}

int GdialogPanelSlide( int a1, int a2, char *a3, char *a4, char *a5, int a6, int a7 )
{
    unsigned int result;
    char *pSrc, *v9;
    int v7,SysTime,v12,v13,v17,v18;
    VidRect_t v14;

    v7 = a6;
    pSrc = a3;
    v9 = a4;
    result = a6 / 10;
    if( a2 == 1 ){
        v14.rt = gVidMainGeo.rt - gVidMainGeo.lt;
        v14.bm = a6 - 1;
        v18 = a6 / 10;
        v14.lt = 0;
        if( a7 == -1 ){
            v18 = 0;
            v14.tp = 10;
        } else {
            v14.tp = 10 * result;
            result *= 10 * ( gVidMainGeo.rt - gVidMainGeo.lt + 1 );
            v7 = a6 - v14.tp;
            v9 = &a4[ result ];
        }
        for( ;v18 >= 0; v18-- ){
            SoundUpdateAll();
            ScrCopy(pSrc, gVidMainGeo.rt - gVidMainGeo.lt + 1, v7, gVidMainGeo.rt - gVidMainGeo.lt + 1, v9, gVidMainGeo.rt - gVidMainGeo.lt + 1);
            v14.tp -= 10;
            WinAreaUpdate( a1, &v14 );
            v7 += 10;
            v9 += -10 * (gVidMainGeo.rt - gVidMainGeo.lt) - 10;
            SysTime = TimerGetSysTime();
            while( TimerCurrDiff( SysTime ) < 33 );
        }
    } else {
        v14.rt = gVidMainGeo.rt - gVidMainGeo.lt;
        v14.bm = a6 - 1;
        v14.lt = 0;
        v14.tp = 0;        
        for( v17 = a6 / 10; v17 > 0; v17-- ){
            SoundUpdateAll();
            ScrCopy( a5, gVidMainGeo.rt - gVidMainGeo.lt + 1, 10, gVidMainGeo.rt - gVidMainGeo.lt + 1, v9, gVidMainGeo.rt - gVidMainGeo.lt + 1);
            v12 = 10 * (gVidMainGeo.rt - gVidMainGeo.lt + 1);
            v9 += v12;
            v7 -= 10;
            a5 += v12;
            ScrCopy( pSrc, gVidMainGeo.rt - gVidMainGeo.lt + 1, v7, gVidMainGeo.rt - gVidMainGeo.lt + 1, v9, gVidMainGeo.rt - gVidMainGeo.lt + 1 );
            WinAreaUpdate( a1, &v14 );
            v14.tp += 10;
            for( v13 = TimerGetSysTime(); TimerCurrDiff( v13 ) < 33; );
        }
    }
    return result;
}

int GdialogGetLines( char *text, int width )
{
    int n, i;
    
    i = 0;
    for( n = gFont.LineWidth( text ); n > 0; n -= width, i++ );
    return i;
}

void GdialogUnk55( char *surf, VidRect_t *area, char *text, int *pStrPos, int Height, int Pitch, int Color )
{
    GdialogDisplayMsg( surf, area, text, pStrPos, Height, Pitch, Color, 1 );
}

int GdialogDisplayMsg( char *Surf, VidRect_t *Area, char *Text, int *pPosition, int Height, int Pitch, int Color, int Flag )
{
    char *s, *str, *p;
    int Width, h;

    s = NULL;
    str = ( pPosition ) ? &Text[ *pPosition ] : Text;
    Width = Area->rt - Area->lt;
    while( str && *str ){
    	if( gFont.LineWidth(str) > Width ){
	    for( s = str + 1; *s && *s != ' '; s++ );
	    if( !*s ){
		if( Flag ){
    		    h = gFont.ChrHeight();
    		    if( Area->bm - h < Area->tp ) return Area->tp;
    		    if( Flag != 1 || str == Text )
        		gFont.Print( &Surf[Pitch * Area->tp + 10], str, Width, Pitch, Color );
    		    else
        		gFont.Print( &Surf[Pitch * Area->tp], str, Width, Pitch, Color );
		}
		if( pPosition ) *pPosition += strlen( str ) + 1;
		Area->tp += Height;
		return Area->tp;
	    }
	    p = s + 1;
	    while( p ){
    		while( *p && *p != ' ' ) p++;
    		if( !*p ){
    		    p = NULL;
    		} else {
    		    *p = '\0';
        	    if( gFont.LineWidth( str ) >= Width ){
        		*p = ' ';
        		p = NULL;
        	    } else {
            		s = p;
            		*p++ = ' ';
        	    }
    		}
	    }
	    if( *s == ' ' ) *s = '\0';
    	}
    	if( gFont.LineWidth(str) > Width ){ eprintf( "\nError: display_msg: word too long!" ); return Area->tp; }
    	if( Flag ){
    	    h = gFont.ChrHeight();
    	    if( Area->bm - h < Area->tp ){
            	if( s && !*s ) *s = ' ';
            	return Area->tp;
    	    }
    	    if( Flag != 1 || str == Text )
            	gFont.Print( &Surf[ Pitch * Area->tp + 10 ], str, Width, Pitch, Color );
    	    else
        	gFont.Print( &Surf[ Pitch * Area->tp ], str, Width, Pitch, Color );
    	}
    	if( pPosition && s ) *pPosition += strlen( str ) + 1;
    	Area->tp += Height;
    	if( s ){
    	    str = s + 1;
    	    if( !*s ) *s = ' ';
    	    s = NULL;
    	} else {
    	    str = NULL;
    	}
    }
    if( pPosition ) *pPosition = 0;
    return Area->tp;    
    
}

int GdialogSetBarterModifier( int mod )
{
    gDlgBarterModifier = mod;
    return 0;
}

int GdialogBarterMenu( int BarterMod )
{
    if( !gDlgInDialog ) return -1;
    gDlgBarterModifier = BarterMod;
    GdialogBarter();
    gDlgUnk09 = 4;
    gDlgState = 2;
    return 0;
}

void GdialogUnk59()
{
    DialogQuit();
    DialogFree();
    SciUnk19();
    WinRun();
    gDlgUnk09 = 1;
    gDlgState = 1;
}

int GdialogUnk60()
{
    ArtFrmHdr_t *Img;
    char *ObjData,*Surface;
    int w,h;
    CachePool_t *ImgObj;
DD
    gDlgUnk09 = 4;    
    if( !( Img = ArtLoadImg( ( gDlgPmExchangeMode ) ? ArtMakeId( 6, 420, 0, 0, 0 ) : ArtMakeId( 6, 111, gDlgPmExchangeMode, 0, 0 ), &ImgObj ) ) ) return -1;
    if( ( ObjData = ArtGetObjData( Img, 0, 0 ) ) ){
        gDlgPanelHeight = ArtGetObjHeight( Img, 0, 0 );
        h = 480 - gDlgPanelHeight;            
        if( ( gDlgMainWin = WinCreateWindow( 0, h, gVidMainGeo.rt - gVidMainGeo.lt + 1, gDlgPanelHeight, 256, 2 ) ) != -1 ){
            Surface = WinGetSurface( gDlgMainWin );
            w = gVidMainGeo.rt - gVidMainGeo.lt + 1;
            ScrCopy( WinGetSurface( gDlgBackWindow ) + w * h, w, gDlgPanelHeight, w, Surface, w ); // background, under panel
            GdialogPanelSlide( gDlgMainWin, 1, ObjData, Surface, 0, gDlgPanelHeight, 0 ); // Panel Look in Trade mode
            ArtClose( ImgObj );
            if( (gDlgButtons[ 0 ] = WinCreateButton( gDlgMainWin, 41, 163, 14, 14, -1, -1, -1, 109, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) != -1 ){ // 'Trade' button
                WinSetClickSound( gDlgButtons[ 0 ], GSoundPlayCheck, GSoundPlayUnCheck );
                if( (gDlgButtons[ 1 ] = WinCreateButton( gDlgMainWin, 584, 162, 14, 14, -1, -1, -1, 116, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) != -1 ){ // 'Talk' button
                    WinSetClickSound( gDlgButtons[ 1 ], GSoundPlayCheck, GSoundPlayUnCheck );
                    if( ObjCreate( &gDlgUnk15, -1, -1 ) != -1 ){
                        gDlgUnk15->Flags |= 0x01;
                        if( ObjCreate( &gDlgUnk20, -1, -1 ) != -1 ){
                            gDlgUnk20->Flags |= 0x01;
                            if( ObjCreate( &gDlgUnk21, gDlgPartyMemberObj->ImgId, -1 ) != -1 ){
                                gDlgUnk21->ScrId = -1;
                                gDlgUnk21->Flags |= 0x05;
                                return 0;
                            }
                            ObjDestroy( gDlgUnk20, 0 );
                        }
                        ObjDestroy( gDlgUnk15, 0 );
                    }
                    WinImageRemove( gDlgButtons[ 1 ] );
                    gDlgButtons[ 1 ] = -1;
                }
                WinImageRemove( gDlgButtons[ 0 ] );
                gDlgButtons[ 0 ] = -1;
            }
            WinClose( gDlgMainWin );
            gDlgMainWin = -1;
        }
    }
    ArtClose( ImgObj );
    return -1;
}

void GdialogPanelClose()
{
    int i;
    char *Surface,*Bitmap;
    CachePool_t *ImgObj;

    if( gDlgMainWin == -1 ) return;
    ObjDestroy( gDlgUnk21, 0 );
    ObjDestroy( gDlgUnk20, 0 );
    ObjDestroy( gDlgUnk15, 0 );
    for ( i = 0; i != 9;  i++ ){
        WinImageRemove( gDlgButtons[ i ] );
        gDlgButtons[ i ] = -1;
    }
    Surface = WinGetSurface( gDlgBackWindow );    
    if( (Bitmap = ArtGetBitmap( ( gDlgPmExchangeMode ) ? ArtMakeId( 6, 420, 0, 0, 0 ) : ArtMakeId( 6, 111, 0, 0, gDlgPmExchangeMode ), 0, 0, &ImgObj )) ){
        GdialogPanelSlide( gDlgMainWin, 0, Bitmap, WinGetSurface( gDlgMainWin ), &Surface[ (gVidMainGeo.rt - gVidMainGeo.lt + 1) * (480 - gDlgPanelHeight) ], gDlgPanelHeight, 0 );
        ArtClose( ImgObj );
    }
    WinClose( gDlgMainWin );
    gDlgMainWin = -1;
    AiUnk45( gDlgPartyMemberObj, 0 );
}

void GdialogUnk62()
{
    int i;

    for( i = 0; i < gDlgUnk15->Critter.Box.Cnt; i++ ){
        Item14( gDlgUnk15, gObjDude, gDlgUnk15->Critter.Box.Box->obj, Item33( gDlgUnk15, gDlgUnk15->Critter.Box.Box->obj ) );
    }
    for( i = 0; i < gDlgUnk20->Critter.Box.Cnt; i++ ){
        Item14( gDlgUnk20, gDlgPartyMemberObj, gDlgUnk20->Critter.Box.Box->obj, Item33( gDlgUnk20, gDlgUnk20->Critter.Box.Box->obj ) );
    }
    if( gDlgUnk21 ){
        for( i = 0; i < gDlgUnk21->Critter.Box.Cnt; i++ ){
            Item14( gDlgUnk21, gDlgPartyMemberObj, gDlgUnk21->Critter.Box.Box->obj, Item33( gDlgUnk21, gDlgUnk21->Critter.Box.Box->obj ) );
        }
    }
}

int GdialogCombatControlMenuCreate()
{
    ArtFrmHdr_t *Img,*v18,*v23,*v26;
    CachePool_t *ImgObj;
    char *ObjData,*Surface,*v9,*v27,*a8,*a9;    
    int ObjHeight,v8,Disposition,v31,zz,a3,i;

    if( !( Img = ArtLoadImg( ArtMakeId( 6, 390, 0, 0, 0 ), &ImgObj ) ) ) return -1;
    if( !( ObjData = ArtGetObjData( Img, 0, 0 ) ) ) goto err;
    ObjHeight = ArtGetObjHeight( Img, 0, 0 );
    gDlgPanelHeight = ObjHeight;    
    if( (gDlgMainWin = WinCreateWindow(0, 480 - ObjHeight, gVidMainGeo.rt - gVidMainGeo.lt + 1, ObjHeight, 256, 2)) == -1 ) goto err;
    Surface = WinGetSurface( gDlgMainWin );
    v31 = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    v8 = v31 * (480 - ObjHeight);
    v9 = WinGetSurface( gDlgBackWindow );
    ScrCopy( &v9[v8], v31, ObjHeight, v31, Surface, v31 );
    GdialogPanelSlide( gDlgMainWin, 1, ObjData, Surface, 0, ObjHeight, 0 );
    ArtClose( ImgObj );            
    if( ( gDlgButtons[ 0 ] = WinCreateButton( gDlgMainWin, 593, 41, 14, 14, -1, -1, -1, 27, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) == -1 ) goto err;
    WinSetClickSound( gDlgButtons[ 0 ], GSoundPlayCheck, GSoundPlayUnCheck );                
    if( ( gDlgButtons[ 1 ] = WinCreateButton( gDlgMainWin, 593, 97, 14, 14, -1, -1, -1, 100, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) == -1 ) goto err;
    WinSetClickSound( gDlgButtons[ 1 ], GSoundPlayCheck, GSoundPlayUnCheck);                    
    if( ( gDlgButtons[ 2 ] = WinCreateButton( gDlgMainWin, 236, 15, 14, 14, -1, -1, -1, 119, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) == -1 ) goto err;
    WinSetClickSound( gDlgButtons[ 2 ], GSoundPlayCheck, GSoundPlayUnCheck );
    if( ( gDlgButtons[ 3 ] = WinCreateButton( gDlgMainWin, 235, 46, 14, 14, -1, -1, -1, 97, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) == -1 ) goto err;
    WinSetClickSound( gDlgButtons[ 3 ], GSoundPlayCheck, GSoundPlayUnCheck );
    gDlgUnk113 = 4;
    for( i = 0; i < 5; i++  ){ // disposition list
        if( !(v18 = ArtLoadImg( ArtMakeId( 6, gDlgCmbtCtrlButtons[ i ].ArtId1, 0, 0, 0 ), &gDlgCmbtCtrlButtons[ i ].Art1 ) ) ) goto err;
        zz = ArtGetObjWidth( v18, 0, 0 );
        a3 = ArtGetObjHeight( v18, 0, 0 );
        a8 = ArtGetObjData( v18, 0, 0 );
        if( !(v23 = ArtLoadImg( ArtMakeId( 6, gDlgCmbtCtrlButtons[ i ].ArtId2, 0, 0, 0 ), &gDlgCmbtCtrlButtons[ i ].Art2 ) ) ) goto err;
        a9 = ArtGetObjData( v23, 0, 0 );                                
        if( !(v26 = ArtLoadImg( ArtMakeId( 6, gDlgCmbtCtrlButtons[ i ].ArtId3, 0, 0, 0 ), &gDlgCmbtCtrlButtons[ i ].Art3 ) ) ) goto err;
        v27 = ArtGetObjData( v26, 0, 0 );
	gDlgButtons[ i + 4 ] = WinCreateButton( gDlgMainWin, gDlgCmbtCtrlButtons[ i ].Xpos, gDlgCmbtCtrlButtons[ i ].Ypos, zz, a3, -1, -1, gDlgCmbtCtrlButtons[ i ].HotKey, -1, a8, a9, 0, 37 );
        if( ( gDlgButtons[ i + 4 ]  ) == -1 ) break;
        WinSetAnimH( gDlgButtons[ i + 4 ], v27, v27, v27 );
        WinSetClickSound( gDlgButtons[ i + 4 ], GSoundPlayCheck, GSoundPlayUnCheck );
        if( !PartyInDisposition( gDlgPartyMemberObj, gDlgCmbtCtrlButtons[ i ].Disposition ) ) WinDisableWidget( gDlgButtons[ i + 4 ] );
    }                                                        
    WinRadioButton( 5, &gDlgButtons[ gDlgUnk113 ] );
    Disposition = AiGetDisposition( gDlgPartyMemberObj );
    WinButtonSet( gDlgButtons[ gDlgUnk113 + 4 - Disposition ], 1, 0 );
    GdialogCombatControlAttributes();
    gDlgUnk09 = 10;
    WinUpdate( gDlgMainWin );
    return 0;
err:
    GdialogCombatControlMenuClose();
    return -1;
}

void GdialogCombatControlMenuClose()
{
    CachePool_t *ImgObj;
    char *surf, *img;
    int i;

    if( gDlgMainWin == -1 ) return;    
    for( i = 0; i != 9; i++ ){
        WinImageRemove( gDlgButtons[ i ] );
        gDlgButtons[ i ] = -1;
    }
    for( i = 0; i != 5; i++ ){
        if( gDlgCmbtCtrlButtons[ i ].Art1 ){
            ArtClose( gDlgCmbtCtrlButtons[ i ].Art1 );
            gDlgCmbtCtrlButtons[ i ].Art1 = NULL;
        }
        if( gDlgCmbtCtrlButtons[ i ].Art2 ){
            ArtClose( gDlgCmbtCtrlButtons[ i ].Art2 );
            gDlgCmbtCtrlButtons[ i ].Art2 = NULL;
        }
        if( gDlgCmbtCtrlButtons[ i ].Art3 ){
            ArtClose( gDlgCmbtCtrlButtons[ i ].Art3 );
            gDlgCmbtCtrlButtons[ i ].Art3 = NULL;
        }
    }
    surf = WinGetSurface( gDlgBackWindow );    
    if( (img = ArtGetBitmap( ArtMakeId( 6, 390, 0, 0, 0 ), 0, 0, &ImgObj )) ){
        GdialogPanelSlide( gDlgMainWin, 0, img, WinGetSurface( gDlgMainWin ), &surf[ (gVidMainGeo.rt - gVidMainGeo.lt + 1) * (480 - gDlgPanelHeight) ], gDlgPanelHeight, 0 );
        ArtClose( ImgObj );
    }
    WinClose( gDlgMainWin );
    gDlgMainWin = -1;    
}

void GdialogCombatControlAttributes()
{
    int Width,ObjWidth,v20,v22,v29,FontId;
    char *ObjData,stmp[256],*surf;
    ArtFrmHdr_t *img, *v18;
    Obj_t *RHandObj, *ArmorObj;
    MsgLine_t Line;
    CachePool_t *Obj;

    FontId = FontGetCurrent();
    FontSet( 101 );
    surf = WinGetSurface( gDlgMainWin );
    Width = WinGetWidth( gDlgMainWin );
    if( (img = ArtLoadImg( ArtMakeId( 6, 390, 0, 0, 0 ), &Obj )) ){
        ObjWidth = ArtGetObjWidth( img, 0, 0 );
        ObjData = ArtGetObjData( img, 0, 0 );
        ScrCopy( &ObjData[ 20 * ObjWidth + 112 ], 110, gFont.ChrHeight(), ObjWidth, &surf[ 20 * Width + 112 ], Width );
        ScrCopy( &ObjData[ 49 * ObjWidth + 112 ], 110, gFont.ChrHeight(), ObjWidth, &surf[ 49 * Width + 112 ], Width );
        ScrCopy( &ObjData[ 84 * ObjWidth + 8 ], 70, 98, ObjWidth, &surf[ 84 * Width + 8 ], Width );
        ScrCopy( &ObjData[ 80 * ObjWidth + 232 ], 132, 106, ObjWidth, &surf[ 80 * Width + 232 ], Width );
        ArtClose( Obj );
    }
    RHandObj = InvGetRHandObj(gDlgPartyMemberObj);
    sprintf( stmp, "%s", ( RHandObj ) ? ItemGetName( RHandObj ) : MessageGetMessage( &gProtoMessages, &Line, 10 ) );
    gFont.Print( &surf[ 20 * Width + 112 ], stmp, 110, Width, DLG_COLOR_5 );
    ArmorObj = InvGetArmorObj( gDlgPartyMemberObj );
    sprintf( stmp, "%s", ( ArmorObj ) ? ItemGetName( ArmorObj ) : MessageGetMessage( &gProtoMessages, &Line, 10 ) );
    gFont.Print( &surf[ 49 * Width + 112 ], stmp, 110, Width, DLG_COLOR_5 );
    if( (v18 = ArtLoadImg( ArtMakeId( OBJTYPE( gDlgPartyMemberObj->ImgId ), gDlgPartyMemberObj->ImgId & 0xFFF, 0, (gDlgPartyMemberObj->ImgId & 0xF000) >> 12, 3 ), &Obj )) ){
        v20 = ArtGetObjWidth( v18, 0, 3 );
        v22 = ArtGetObjHeight( v18, 0, 3 );
        ScrCopyAlpha( ArtGetObjData( v18, 0, 3 ), v20, v22, v20, &surf[ Width * (132 - v22 / 2) + 39 - v20 / 2], Width );
        ArtClose( Obj );
    }
    // attributes
    sprintf( stmp, "%d/%d", FeatGetVal(gDlgPartyMemberObj, 35), FeatGetVal( gDlgPartyMemberObj, FEAT_HP ) );
    gFont.Print( &surf[ 96 * Width + 240 ], stmp, 115, Width, DLG_COLOR_5 );

    sprintf( stmp, "%s", SkillGetName( PartyBestSkill( gDlgPartyMemberObj ) ) );
    gFont.Print( &surf[ 113 * Width + 240 ], stmp, 115, Width, DLG_COLOR_5 );

    v29 = DLG_COLOR_5;
    sprintf( stmp, "%d/%d ", ItemGetBackPackWeight( gDlgPartyMemberObj ), FeatGetVal( gDlgPartyMemberObj, FEAT_CARRY ) );
    if( CritterOverload( gDlgPartyMemberObj ) ) v29 = DLG_COLOR_9;
    gFont.Print( &surf[ 131 * Width + 240 ], stmp, 115, Width, v29 );

    sprintf( stmp, "%d", FeatGetVal( gDlgPartyMemberObj, FEAT_MELEE ) );
    gFont.Print( &surf[ 148 * Width + 240 ], stmp, 115, Width, DLG_COLOR_5 );

    sprintf( stmp, "%d/%d ", ( IN_COMBAT ) ? gDlgPartyMemberObj->Critter.State.CurrentAP : FeatGetVal( gDlgPartyMemberObj, FEAT_AP ), FeatGetVal( gDlgPartyMemberObj, FEAT_AP ) );
    gFont.Print( &surf[ 167 * Width + 240 ], stmp, 115, Width, DLG_COLOR_5 );

    FontSet( FontId );
    WinUpdate( gDlgMainWin );
}

void GdialogUnk66()
{
    gDlgState = 8;
    gDlgUnk09 = 10;
    if( gDlgReplyWin != -1 ) WinUpdateDirty( gDlgReplyWin );
    if( gDlgOptionWin != -1 ) WinUpdateDirty( gDlgOptionWin );
}

int GdialogUnk67( Obj_t *a1 )
{
    const int Pids[ 3 ] = { 0x1000088, 0x1000156, 0x1000180 };
    int i;
    for( i = 0; i < 3; i++ ){
        if( a1->Pid == Pids[ i ] ) return RandMinMax( 0, 1 ) + 677;
    }
    return RandMinMax( 0, 4 ) + 670;
}

int GdialogMenuTrade()
{
    MsgLine_t msg;
    Proto_t *proto;

    if( OBJTYPE( gDlgPartyMemberObj->Pid ) != TYPE_CRIT ) return 1;
    ProtoGetObj( gDlgPartyMemberObj->Pid, &proto );
    if( (proto->Critt.Type & 2) != 0 ) return 1;
    msg.Id = 903;
    if( gDlgPmExchangeMode ) msg.Id = 913;
    if( MessageGetMsg( &gProtoMessages, &msg ) == 1 )
        GdialogReply( msg.Text );
    else
        eprintf( "\nError: gdialog: Can't find message!" );
    return 0;
}

void GdialogCombatControlMenu()
{
    int ext,n;
    Obj_t *obj;
    MsgLine_t msg;

    ext = 0;
    while( !ext && !gMenuEscape){
    	switch( (n = InpUpdate()) ){
    	    case -1: break;
    	    case 17: case 24: case 324: SysQuitDlg(); break;
    	    case KEY_ESC: gDlgState = 1; gDlgUnk09 = 1; return;
    	    case 'a': // use best armor
    		if( gDlgPartyMemberObj->Pid != 0x10000A1 ){                
        	    if( (obj = AiFindBestArmor( gDlgPartyMemberObj ) ) ) InvUnk28( gDlgPartyMemberObj, obj, 0 );
    		}
    		GdialogReply( MessageGetMessage( &gProtoMessages, &msg, GdialogUnk67( gDlgPartyMemberObj ) ) );
    		GdialogCombatControlAttributes();
    		break;
	    case 'd': // Trade
		if( !GdialogMenuTrade() ) break;
		gDlgState = 2; 
		gDlgUnk09 = 4; 
		return;
    	    case -2: // disposition
    		if( !MseCursorCenterInArea( 441, 451, 540, 470 ) ) break;
    		AiUnk06( gDlgPartyMemberObj, AI_DISP_CUSTOM );
    		gDlgUnk09 = 13; 
    		gDlgState = 11; 
    		ext = 1;
    		break;
    	    case 'w': // best weapon
        	InvUnwield( gDlgPartyMemberObj, 1 );
        	if( (obj = AiUnk29( gDlgPartyMemberObj, 0, 0 )) ){ InvUnk28( gDlgPartyMemberObj, obj, 1 ); AiUnk45( gDlgPartyMemberObj, 0 ); }
    		GdialogReply( MessageGetMessage( &gProtoMessages, &msg, GdialogUnk67( gDlgPartyMemberObj ) ) );
    		GdialogCombatControlAttributes();
    		break;
    	    case 2098: AiUnk06( gDlgPartyMemberObj, AI_DISP_BERSERK ); break;
    	    case 2099: AiUnk06( gDlgPartyMemberObj, AI_DISP_CUSTOM ); gDlgUnk09 = 13; gDlgState = 11; ext = 1; break;
    	    case 2102: AiUnk06( gDlgPartyMemberObj, AI_DISP_DEFENSIVE ); break;
    	    case 2103: AiUnk06( gDlgPartyMemberObj, AI_DISP_AGGRESIVE ); break;
    	    case 2111: AiUnk06( gDlgPartyMemberObj, AI_DISP_COWARD ); break;        
        }
    }
}

int GdialogCustomDispMenuCreate()
{
    ArtFrmHdr_t *Img, *Img1, *Img2;
    CachePool_t *ImgObj;
    char *ObjData, *Surface, *p;
    int ObjHeight, i, w;

    if( MessageInit( &gDlgMsg ) != 1 ) return -1;
    if( MessageLoad( &gDlgMsg, "game/custom.msg" ) != 1 ) return -1;    
    if( !(Img = ArtLoadImg( ArtMakeId( 6, 391, 0, 0, 0 ), &ImgObj )) ) return -1;
    ObjData = ArtGetObjData( Img, 0, 0 );
    if( !ObjData ){ GdialogCustomDispMenuClose(); return -1; }
    ObjHeight = ArtGetObjHeight( Img, 0, 0 );
    gDlgPanelHeight = ObjHeight;
    w = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    if( (gDlgMainWin = WinCreateWindow( 0, 480 - ObjHeight, w, ObjHeight, 256, 2 ) ) == -1 ){ GdialogCustomDispMenuClose(); return -1; }
    Surface = WinGetSurface( gDlgMainWin );
    p = WinGetSurface( gDlgBackWindow );
    ScrCopy( &p[ w * (480 - ObjHeight)], w, ObjHeight, w, Surface, w );
    GdialogPanelSlide( gDlgMainWin, 1, ObjData, Surface, 0, ObjHeight, 0 );
    ArtClose( ImgObj );
    if( (gDlgButtons[ 0 ] = WinCreateButton( gDlgMainWin, 593, 101, 14, 14, -1, -1, -1, 13, gDlgImgBtA0, gDlgImgBtA1, 0, 32 )) == -1 ){ GdialogCustomDispMenuClose(); return -1; }
    WinSetClickSound( gDlgButtons[ 0 ], GSoundPlayCheck, GSoundPlayUnCheck );
    gDlgUnk112 = 1;                
    for( i = 0; i < 6; i++ ){
        if( !(Img1 = ArtLoadImg( ArtMakeId( 6, gDlgCustomDispButtons[ i ].ArtId1, 0, 0, 0 ), &gDlgCustomDispButtons[ i ].Art1 ) ) ) break;
        if( !(Img2 = ArtLoadImg( ArtMakeId( 6, gDlgCustomDispButtons[ i ].ArtId2, 0, 0, 0 ), &gDlgCustomDispButtons[ i ].Art2 ) ) ) break;
        gDlgButtons[ i + 1 ] = WinCreateButton( gDlgMainWin, gDlgCustomDispButtons[ i ].Xpos, gDlgCustomDispButtons[ i ].Ypos, ArtGetObjWidth( Img1, 0, 0 ), ArtGetObjHeight( Img1, 0, 0 ), -1, -1, -1, gDlgCustomDispButtons[ i ].HotKey, ArtGetObjData( Img1, 0, 0 ), ArtGetObjData( Img2, 0, 0 ), 0, 32 );
        if( gDlgButtons[ i + 1 ] == -1 ) break;
        WinSetClickSound( gDlgButtons[ i + 1 ], GSoundPlayCheck, GSoundPlayUnCheck );
    }
    gDlgDispositions[ 0 ] = AiGetAreaAttackMode( gDlgPartyMemberObj );
    gDlgDispositions[ 1 ] = AiGetHealthGrade( gDlgPartyMemberObj );
    gDlgDispositions[ 2 ] = AiGetBestWeapon( gDlgPartyMemberObj );
    gDlgDispositions[ 3 ] = AiGetDistance( gDlgPartyMemberObj );
    gDlgDispositions[ 4 ] = AiGetAttackWho( gDlgPartyMemberObj );
    gDlgDispositions[ 5 ] = AiGetChemUse( gDlgPartyMemberObj );
    gDlgUnk09 = 13;
    GdialogUpdateCustomDispMenu();
    return 0;    
}

void GdialogCustomDispMenuClose()
{
    int i, tmp;
    CachePool_t *ImgObj;
    char *surf, *Img;

    if( gDlgMainWin == -1 ) return;
    for( i = 0; i != 9; i++ ){
        WinImageRemove( gDlgButtons[ i ] );
        gDlgButtons[ i ] = -1;
    }
    for( i = 0; i != 6; i++ ){
        if( gDlgCustomDispButtons[ i ].Art1 ){ ArtClose( gDlgCustomDispButtons[ i ].Art1 ); gDlgCustomDispButtons[ i ].Art1 = NULL; }
        if( gDlgCustomDispButtons[ i ].Art2 ){ ArtClose( gDlgCustomDispButtons[ i ].Art2 ); gDlgCustomDispButtons[ i ].Art2 = NULL; }
        if( gDlgCustomDispButtons[ i ].Art3 ){ ArtClose( gDlgCustomDispButtons[ i ].Art3 ); gDlgCustomDispButtons[ i ].Art3 = NULL; }
    }
    tmp = (gVidMainGeo.rt - gVidMainGeo.lt + 1) * (480 - gDlgPanelHeight);
    surf = WinGetSurface( gDlgBackWindow );
    if( ( Img = ArtGetBitmap( ArtMakeId( 6, 391, 0, 0, 0 ), 0, 0, &ImgObj ) ) ){
        GdialogPanelSlide( gDlgMainWin, tmp, Img, WinGetSurface( gDlgMainWin ), &surf[ tmp ], gDlgPanelHeight, 0 );
        ArtClose( ImgObj );
    }
    WinClose( gDlgMainWin );
    gDlgMainWin = -1;
    MessageClose( &gDlgMsg );
}

void GdialogCustomDispMenuProcess()
{
    int sel;

    do{
        sel = InpUpdate();
        if( sel == -1 ) continue;
    	if( sel == KEY_ENTER || sel == KEY_ESC ){
    	    gDlgState = 8;
    	    gDlgUnk09 = 10;
    	    break;
    	}
	if( sel == 17 || sel == 24 || sel == 324 ) SysQuitDlg();
    	if( sel <= 5 ){
if( sel < 0 ) continue;
            GdialogCustomDispMode( sel );
            GdialogUpdateCustomDispMenu();
    	}
    }while( !gMenuEscape );
}

void GdialogUpdateCustomDispMenu()
{
    char *surf;
    int w, ow, oh, FontId;
    ArtFrmHdr_t *img;
    MsgLine_t fmt; 
    CachePool_t *ImgObj;

    FontId = FontGetCurrent();
    FontSet( 101 );
    surf = WinGetSurface( gDlgMainWin );
    w = WinGetWidth( gDlgMainWin );
    if( !( img = ArtLoadImg( ArtMakeId( 6, 391, 0, 0, 0 ), &ImgObj ) ) ) return;
    oh = ArtGetObjHeight( img, 0, 0 );
    ow = ArtGetObjWidth( img, 0, 0 );
    ScrCopy( ArtGetObjData( img, 0, 0 ), ow, oh, ow, surf, gVidMainGeo.rt - gVidMainGeo.lt + 1 );
    ArtClose( ImgObj );
    if( gDlgDispositions[ 0 ] == -1 ){
        gFont.Print( &surf[ 20 * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, 99 ), 248, w, DLG_COLOR_5 );
    } else {
        eprintf( "\nburst: %d", gDlgDispositions[ 0 ] );
        gFont.Print( &surf[ 20 * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[0][ gDlgDispositions[0] ][0] ), 248, w, DLG_COLOR_5 );
    }        
    gFont.Print( &surf[ 48  * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[1][ gDlgDispositions[1] ][0]), 248, w, DLG_COLOR_5 );
    gFont.Print( &surf[ 78  * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[2][ gDlgDispositions[2] ][0]), 248, w, DLG_COLOR_5 );
    gFont.Print( &surf[ 108 * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[3][ gDlgDispositions[3] ][0]), 248, w, DLG_COLOR_5 );
    gFont.Print( &surf[ 137 * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[4][ gDlgDispositions[4] ][0]), 248, w, DLG_COLOR_5 );
    gFont.Print( &surf[ 166 * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[5][ gDlgDispositions[5] ][0]), 248, w, DLG_COLOR_5 );
    WinUpdate( gDlgMainWin );
    FontSet( FontId );
}

void GdialogDispositionOptions( char *surf, int Pitch, int DispositionType, int HighLight )
{
    int i, state = 0, color, FieldLen;
    MsgLine_t msg;

    FontSet( 101 );    
    FieldLen = Pitch - 84;
    for( i = 0; i < 6; i++ ){
        if( gDlgTextIds[ DispositionType ][ i ][ 0 ] == -1 ) continue;
        switch( DispositionType ){
            case 0: state = PartyInAttackMode( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ i ][ 1 ] ); break;
            case 1: state = PartyInRunAwayMode( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ i ][ 1 ] ); break;
            case 2: state = PartyInBestWeapon( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ i ][ 1 ] ); break;
            case 3: state = PartyInDistance( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ i ][ 1 ] ); break;
            case 4: state = PartyInAttackWho( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ i ][ 1 ] ); break;
            case 5: state = PartyInChemUse( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ i ][ 1 ] ); break;
        }
        if( state ){
            color = ( i == HighLight ) ? DLG_COLOR_10 : DLG_COLOR_5;
        } else {
            color = DLG_COLOR_7;
        }
        gFont.Print( (surf + Pitch * (i * gFont.ChrHeight() + 42) + 42), MessageGetMessage( &gDlgMsg, &msg, gDlgTextIds[ DispositionType ][ i ][ 0 ] ), FieldLen, Pitch, color );
    }    
}

int GdialogCustomDispMode( int DispositionType )
{
    MsgLine_t msg;
    CachePool_t *Obj;
    ArtFrmHdr_t *Data;
    int state, ObjWidth,sel,SysTime,bottom,right,FontId,CentY,CentX,Height,win,x0,y1,x1,y0,ExitFlg,SelectedLine;
    char *ObjData, *surf;
    unsigned int SelLine, PrevTime;

    FontId = FontGetCurrent();
    ExitFlg = PrevTime = 0;    
    if( !( Data = ArtLoadImg( ArtMakeId( 6, 419, 0, 0, 0 ), &Obj ) ) ) return -1;
    ObjWidth = ArtGetObjWidth( Data, 0, 0 );
    Height = ArtGetObjHeight( Data, 0, 0 );
    CentX = (640 - ObjWidth) / 2;
    CentY = (480 - Height) / 2;    
    if( ( win = WinCreateWindow( CentX, CentY, ObjWidth, Height, 256, 20 ) ) == -1 ){ ArtClose( Obj ); return -1; }
    surf = WinGetSurface( win );
    ObjData = ArtGetObjData( Data, 0, 0 );
    ScrCopy( ObjData, ObjWidth,  Height, ObjWidth, surf, ObjWidth );
    ArtClose( Obj );
    if( WinCreateButton( win, 70, 164, 14, 14, -1, -1, -1, 13, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) == -1 ){ WinClose( win ); return -1; }
    if( WinCreateButton( win, 176, 163, 14, 14, -1, -1, -1, 27, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) == -1 ){ WinClose( win ); return -1; }    
    FontSet( 103 );
    gFont.Print( ( surf + 15  * ObjWidth + 40  ), MessageGetMessage( &gDlgMsg, &msg, DispositionType ), ObjWidth, ObjWidth, DLG_COLOR_3 );
    gFont.Print( ( surf + 163 * ObjWidth + 88  ), MessageGetMessage( &gDlgMsg, &msg, 10 ), ObjWidth, ObjWidth, DLG_COLOR_3 );
    gFont.Print( ( surf + 162 * ObjWidth + 193 ), MessageGetMessage( &gDlgMsg, &msg, 11 ), ObjWidth, ObjWidth, DLG_COLOR_3 );
    SelectedLine = gDlgDispositions[ DispositionType ];
    GdialogDispositionOptions( surf, ObjWidth, DispositionType, SelectedLine );
    WinUpdate( win );
    x0 = CentX + 42;
    y0 = CentY + 42;
    x1 = CentX + ObjWidth - 42;
    y1 = CentY + Height - 42;
    do{
        sel = InpUpdate();
        if( sel == -1 ) continue;
        if( sel == 17 || sel == 24 || sel == 324 ) SysQuitDlg();
        if( gMenuEscape ) break;
        if( sel >= 13 ){
            if( sel == KEY_ENTER ){
                ExitFlg = 1;
                gDlgDispositions[ DispositionType ] = SelectedLine;
                GdialogSetDisposition( DispositionType, gDlgTextIds[ DispositionType ][ SelectedLine ][ 1 ] );
                continue;
            }
            if( sel == KEY_ESC ){ ExitFlg = 1; continue; }
            continue;
        }
        if( sel != -2 ) continue;
        if( (MseGetButtons() & 0x10) == 0 ) continue;
        if( !MseCursorCenterInArea( x0, y0, x1, y1 ) ) continue;
        MseGetCursorPosition( &right, &bottom );
        SelLine = (bottom - y0) / gFont.ChrHeight();
        if( SelLine >= 6 ) continue;
        SysTime = TimerGetSysTime();

        if( SelLine == SelectedLine ){
            if( TimerDiff( SysTime, PrevTime ) < 250 ){
                gDlgDispositions[ DispositionType ] = SelLine;
                ExitFlg = 1;
                GdialogSetDisposition( DispositionType, gDlgTextIds[ DispositionType ][ SelLine ][ 1 ] );
                continue;
            }
        } else {
            if( gDlgTextIds[ DispositionType ][ SelLine ][ 0 ] != -1 ){
                switch( DispositionType ){
                    case 0: state = PartyInAttackMode( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ SelLine ][ 1 ] ); break;
                    case 1: state = PartyInRunAwayMode( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ SelLine ][ 1 ] ); break;
                    case 2: state = PartyInBestWeapon( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ SelLine ][ 1 ] ); break;
                    case 3: state = PartyInDistance( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ SelLine ][ 1 ] ); break;
                    case 4: state = PartyInAttackWho( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ SelLine ][ 1 ] ); break;
                    case 5: state = PartyInChemUse( gDlgPartyMemberObj, gDlgTextIds[ DispositionType ][ SelLine ][ 1 ] ); break;
                    default: state = 0;
                }
                if( state ){ // select line
                    SelectedLine = SelLine;
                    GdialogDispositionOptions( surf, ObjWidth, DispositionType, SelLine );
                    WinUpdate( win );
                }
            }
        }
        PrevTime = SysTime;
    }while( !ExitFlg );
    WinClose( win );
    FontSet( FontId );
    return 0;
}

int GdialogSetDisposition( int Type, int Mode )
{
    switch( Type ){
        case AI_DISP_ATTACK:
            if( Mode >= 5 ) return -1;
            AiGetPacketByObj( gDlgPartyMemberObj )->AreaAttackMode = Mode;
            return 0;
        case AI_DISP_RUNAWAY: return AiSetRunAway( gDlgPartyMemberObj, Mode );
        case AI_DISP_BESTWEAPON: 
            if( Mode >= 8 ) return -1;
            AiGetPacketByObj( gDlgPartyMemberObj )->BestWeapon = Mode;
            return 0;
        case AI_DISP_DISTANCE:
            if( Mode >= 5 ) return -1;
            AiGetPacketByObj( gDlgPartyMemberObj )->Distance = Mode;
            return 0;
        case AI_DISP_ATTACKWHO:
            if( Mode >= 5 ) return -1;
            AiGetPacketByObj( gDlgPartyMemberObj )->AttackWho = Mode;
            return 0;
        case AI_DISP_CHEMUSE:
            if( Mode >= 6 ) return -1;
            AiGetPacketByObj( gDlgPartyMemberObj )->ChemUse = Mode;
            return 0;
    }
    return -1;
}

void GdialogBarter()
{
    MsgLine_t msg;
    Proto_t *proto;
    Scpt_t *scr;

    if( OBJTYPE( gDlgPartyMemberObj->Pid ) != 1 ) return;
    if( ScptPtr( gDlgPartyMemberObj->ScrId, &scr ) == -1 ) return;    
    if( OBJTYPE( gDlgPartyMemberObj->Pid ) == 1 ){ 
	ProtoGetObj( gDlgPartyMemberObj->Pid, &proto );
	if( !( proto->Critt.Type & 0x02 ) ){
    	    msg.Id = 903; // 'This person will not barter with you'
    	    if( gDlgPmExchangeMode ) msg.Id = 913; // 'This critter can't carry anything.'
    	    if( MessageGetMsg( &gProtoMessages, &msg ) == 1 )
        	GdialogReply( msg.Text );
    	    else
        	eprintf( "\nError: gdialog: Can't find message!" );
    	    return;
	}    
    }
    if( gDlgLipsEn == 1 && SoundIsPlaying( gLipsync.Snd ) ) GdialogLipsyncEnd();
    gDlgState = 2; // enter barter
    gDlgUnk09 = 4;
    if( gDlgReplyWin != -1 ) WinUpdateDirty( gDlgReplyWin );
    if( gDlgOptionWin != -1 ) WinUpdateDirty( gDlgOptionWin );
}

int GdialogCreateButtons()
{
    CachePool_t *ImgObj;
    ArtFrmHdr_t *Img;
    char *ObjData, *Surface, *v8, *Bitmap, *v14;
    int ObjHeight, i, w, h;

    if( gDlgUnk88 ) return -1;    
    for( i = 0; i < 9; i++ ) gDlgButtons[ i ] = -1;
    if( !(Img = ArtLoadImg( gDlgPmExchangeMode ? ArtMakeId( 6, 389, 0, 0, 0 ) : ArtMakeId( 6, 99, gDlgPmExchangeMode, 0, 0 ), &ImgObj )) ) return -1;    
    ObjData = ArtGetObjData( Img, 0, 0 );
    if( ObjData ){
        ObjHeight = ArtGetObjHeight( Img, 0, 0 );
	gDlgPanelHeight = ObjHeight;
	h = 480 - ObjHeight;
        gDlgMainWin = WinCreateWindow( 0, h, gVidMainGeo.rt - gVidMainGeo.lt + 1, ObjHeight, 256, 2 );
        if( gDlgMainWin != -1 ){
            Surface = WinGetSurface( gDlgMainWin );
            w = gVidMainGeo.rt - gVidMainGeo.lt + 1;
            v8 = WinGetSurface( gDlgBackWindow );
            ScrCopy( &v8[ w * h ], w, ObjHeight, w, Surface, w );
            if( gDlgUnk10 ){
                WinUpdate( gDlgBackWindow );
                GdialogPanelSlide( gDlgMainWin, 1, ObjData, Surface, NULL, ObjHeight, -1 ); // render lower dialog iface
                gDlgUnk10 = 0;
            } else {
                GdialogPanelSlide( gDlgMainWin, 1, ObjData, Surface, NULL, ObjHeight, -1 );
            }
            ArtClose( ImgObj );
            if( (gDlgButtons[ 0 ] = WinCreateButton( gDlgMainWin, 593, 41, 14, 14, -1, -1, -1, -1, gDlgImgBtA0, gDlgImgBtA1, 0, 32 )) != -1 ){
                    WinSetButtonHandler( gDlgButtons[ 0 ], 0, 0, 0, GdialogBarter ); // handle Barter button
                    WinSetClickSound( gDlgButtons[ 0 ], GSoundPlayCheck, GSoundPlayUnCheck );
                    if( (Bitmap = ArtGetBitmap( ArtMakeId( 6, 97, 0, 0, 0 ), 0, 0, &gDlgUnk121 ) ) ){
                        if( (v14 = ArtGetBitmap( ArtMakeId( 6, 98, 0, 0, 0 ), 0, 0, &gDlgUnk123 ) ) ){
                            if( (gDlgButtons[ 1 ] = WinCreateButton( gDlgMainWin, 13, 154, 51, 29, -1, -1, -1, -1, Bitmap, v14, 0, 0 )) != -1 ){
                                WinSetButtonHandler( gDlgButtons[ 1 ], 0, 0, 0, GdialogReview ); // handle Review button
                                WinSetClickSound( gDlgButtons[ 1 ], GSoundPlayPushBt, GSoundPlayReleaseBt );
                                if( !gDlgPmExchangeMode ){ gDlgUnk88 = 1; return 0; }
                                if( (gDlgButtons[ 2 ] = WinCreateButton( gDlgMainWin, 593, 116, 14, 14, -1, -1, -1, -1, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) != -1 ){
                                    WinSetButtonHandler( gDlgButtons[ 2 ], 0, 0, 0, GdialogUnk66 );
                                    WinSetClickSound( gDlgButtons[ 2 ], GSoundPlayCheck, GSoundPlayUnCheck );
                                    gDlgUnk88 = 1;
                                    return 0;
                                }
                                WinImageRemove( gDlgButtons[ 1 ] );
                                gDlgButtons[ 1 ] = -1;
                            }
                            WinImageRemove( gDlgButtons[ 0 ] );
                            gDlgButtons[ 0 ] = -1;
                            ArtClose( gDlgUnk123 );
                        }
                        ArtClose( gDlgUnk121 );
                    }
                    WinImageRemove( gDlgButtons[ 0 ] );
                    gDlgButtons[ 0 ] = -1;
                }
                WinClose( gDlgMainWin );
                gDlgMainWin = -1;
        }
    }
    ArtClose( ImgObj );        
    return -1;
}

void GdialogPanelCloseAnimation()
{
    int i, Id;
    CachePool_t *ImgObj;
    char *surf, *Bitmap;

    if( gDlgMainWin == -1 ) return;
    for( i = 0; i != 9; i++ ){
        WinImageRemove( gDlgButtons[ i ] );
        gDlgButtons[ i ] = -1;
    }
    ArtClose( gDlgUnk123 );
    ArtClose( gDlgUnk121 );
    surf = WinGetSurface( gDlgBackWindow );
    if( gDlgPmExchangeMode )
        Id = ArtMakeId( 6, 389, 0, 0, 0 );
    else
        Id = ArtMakeId( 6, 99, 0, 0, gDlgPmExchangeMode );    
    if( !( Bitmap = ArtGetBitmap( Id, 0, 0, &ImgObj ) ) ) return;
    GdialogPanelSlide( gDlgMainWin, 0, Bitmap, WinGetSurface( gDlgMainWin ), &surf[ (gVidMainGeo.rt - gVidMainGeo.lt + 1) * (480 - gDlgPanelHeight) ], gDlgPanelHeight, 0 );
    ArtClose( ImgObj );
    WinClose( gDlgMainWin );
    gDlgUnk88 = 0;
    gDlgMainWin = -1;
}

int GdialogCreateWindow()
{
    gDlgBackWindow = WinCreateWindow( 0, 0, gVidMainGeo.rt - gVidMainGeo.lt + 1, 480, 256, 2 );
    return ( gDlgBackWindow != -1 ) - 1;
}

int GdialogRenderReplyBg()
{
    char *img;
    int w;
    CachePool_t *ImgObj;

    if( !(img = ArtGetBitmap( ArtMakeId( 6, 103, 0, 0, 0 ), 0, 0, &ImgObj ) ) ) return -1;
    w = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    ScrCopy( img, w, 480, w, WinGetSurface( gDlgBackWindow ), w );
    ArtClose( ImgObj );
    if( !gDlgUnk10 ) WinUpdate( gDlgBackWindow );
    return 0;
}

int GdialogRenderA( VidRect_t *Area )
{
    int Id, pos;
    CachePool_t *ImgObj;
    char *surf, *img;

    if( gDlgPmExchangeMode )
        Id = ArtMakeId( 6, 389, 0, 0, 0 );
    else
        Id = ArtMakeId( 6, 99, 0, 0, 0 );
    if( !( img = ArtGetBitmap( Id, 0, 0, &ImgObj ) ) ) return -1;
    pos = 640 * Area->tp + Area->lt;
    surf = WinGetSurface( gDlgMainWin );
    ScrCopy( &img[ pos ], Area->rt - Area->lt, Area->bm - Area->tp, gVidMainGeo.rt - gVidMainGeo.lt + 1, &surf[ pos ], gVidMainGeo.rt - gVidMainGeo.lt + 1 );
    ArtClose( ImgObj );
    WinAreaUpdate( gDlgMainWin, Area );
    return 0;
}

void GdialogBlit( char *pSrc, int SrcPitch, int SrcHeight, int DstWidth, char *pDst, int PosX, int PosY, int DstPitch, char *Shader )
{
    unsigned char *dst, c;
    int i, SrcDiff, DstDiff;

    dst = (unsigned char *)&pDst[ PosX + DstPitch * PosY ];
    DstDiff = DstPitch - SrcPitch;
    SrcDiff = DstWidth - SrcPitch;
    for( ; --SrcHeight != -1; pSrc += SrcDiff, dst += DstDiff ){
        for( i = 0; i < SrcPitch; i++, pSrc++, dst++ ){
            c = *pSrc;
            if( c ) c = (256 - *(unsigned char *)pSrc) >> 4;
            *dst = Shader[ *dst + 256 *c ];
        }        
    }
}

void GdialogRenderScreen( ArtFrmHdr_t *Img, int FrameNo )
{
    VidRect_t Rect;
    CachePool_t *ImgObj;
    ArtFrmHdr_t *img;
    char *p, *surf, *pSrc;
    int w,pitch,tmp,i,ShiftY,ShiftX,CentY,CentX, h;

    if( gDlgMainWin == -1 ) return;    
    if( Img ){ // render head
        if( !FrameNo ) gDlgUnk79 = 0;
        if( !(img = ArtLoadImg( ArtMakeId( 9, gDlgBgndIdx, 0, 0, 0 ), &ImgObj ) ) ) eprintf( "\tError locking background in display..." );        
        if( ( p = ArtGetObjData( img, 0, 0 ) ) )
            ScrCopy( p, 388, 200, 388, gDlgSurf, gVidMainGeo.rt - gVidMainGeo.lt + 1 );
        else
            eprintf( "\tError getting background data in display...\n" );
        ArtClose( ImgObj );
        w = ArtGetObjWidth( Img, FrameNo, 0 );
        h = ArtGetObjHeight( Img, FrameNo, 0 );
        pSrc = ArtGetObjData( Img, FrameNo, 0 );
        ArtGetCenter( Img, 0, &CentX, &CentY );
        ArtGetFrameShift( Img, FrameNo, 0, &ShiftX, &ShiftY );
        gDlgUnk79 += ShiftX;
        CentX += ShiftX + gDlgUnk79;
        if( pSrc ){
            pitch = gVidMainGeo.rt - gVidMainGeo.lt + 1;
            tmp = CentX + (200 - h) * pitch + (388 - w) / 2;
            if( tmp + pitch * CentY > 0 ) tmp += pitch * CentY;
            ScrCopyAlpha( pSrc, w, h, w, gDlgSurf + tmp, pitch );
        } else {
            eprintf( "\tError getting head data in display...\n" );
        }
    } else { // render miniature
        if( gDlgUnk26 == 1 ){
            gDlgUnk26 = 0;
            TileUpdate();
        }
        pSrc = WinGetSurface( gMapIsoWin );
	pitch = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        ScrCopy( &pSrc[ pitch * ((gVidMainGeo.bm - gVidMainGeo.tp - 331) / 2) + (pitch - 388) / 2], 388, 200, pitch, gDlgSurf, pitch ); // render miniature
    }
    Rect.lt = 126;
    Rect.tp = 14;
    Rect.rt = 514;
    Rect.bm = 214;
    surf = WinGetSurface( gDlgBackWindow );
    GdialogBlit( ArtGetObjData( gDlgImg101Dat, 0, 0 ), gDlgImg101Width, gDlgImg101Height, gDlgImg101Width, surf, 426, 15, gVidMainGeo.rt - gVidMainGeo.lt + 1, gDlgPalA );
    GdialogBlit( ArtGetObjData( gDlgImg100Dat, 0, 0 ), gDlgImg100Width, gDlgImg100Height, gDlgImg100Width, surf, 129, 214 - gDlgImg100Height - 2, gVidMainGeo.rt - gVidMainGeo.lt + 1, gDlgPalB );
    for( i = 0; i != 8; i++ ){
	pitch = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        tmp = gDlgUnk22[ i ].rt - gDlgUnk22[ i ].lt;
        ScrCopyAlpha( gDlgUnk97[ i ], tmp, gDlgUnk22[ i ].bm - gDlgUnk22[ i ].tp, tmp, &surf[ gDlgUnk22[ i ].tp * pitch + gDlgUnk22[ i ].lt ], pitch ); // render screen frame
    }        
    WinAreaUpdate( gDlgBackWindow, &Rect );
}

void GdialogCreateArts()
{
    unsigned char r,g,b;
    int i;

    for( i = 0; i < 256; i++ ){
        r = RGB16_GET_R( PalConvColor8to16( i ) );
        g = RGB16_GET_G( PalConvColor8to16( i ) );
        b = RGB16_GET_B( PalConvColor8to16( i ) );
        gDlgLumA[ i ] = (2 * b + r + 2 * g) / 40;
        gDlgLumB[ i ] = (b + r + g) / 40;
    }
    gDlgLumA[ 0 ] = 0;
    gDlgLumB[ 0 ] = 0;
    gDlgPalA = PalExtOpen( DLG_COLOR_1 );
    gDlgPalB = PalExtOpen( DLG_COLOR_2 );
    gDlgImg101Dat = ArtLoadImg( ArtMakeId( 6, 115, 0, 0, 0 ), &gDlgUnk120 );
    gDlgImg101Width = ArtGetObjWidth( gDlgImg101Dat, 0, 0 );
    gDlgImg101Height = ArtGetObjHeight( gDlgImg101Dat, 0, 0 );
    gDlgImg100Dat = ArtLoadImg( ArtMakeId( 6, 116, 0, 0, 0 ), &gDlgUnk131 );
    gDlgImg100Width = ArtGetObjWidth( gDlgImg100Dat, 0, 0 );
    gDlgImg100Height = ArtGetObjHeight( gDlgImg100Dat, 0, 0 );
}

int GdialogUnk86()
{
    PalExtClose( DLG_COLOR_1 );
    PalExtClose( DLG_COLOR_2 );
    ArtClose( gDlgUnk120 );
    return ArtClose( gDlgUnk131 );
}


