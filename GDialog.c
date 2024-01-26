#include "FrameWork.h"

int   gDlgUnk06 = 0;
int   gDlgOption = 0;
int   gDlgReviewCount = 0;
char  *gDlgSurf = NULL;
int   gDlgBoxTitle = -1;
int   gDlgBoxWin = -1;
int   gDlgUnk88 = 0;
int   gDlgUnk87 = 0;
int   gDlgUnk86 = 0;
CachePool_t *gDlgUnk85 = NULL;
ArtFrmHdr_t *gDlgUnk84 = NULL;
short gDlgUnk83 = 2;
int   gDlgUnk82 = 0;
CachePool_t *gDlgUnk81 = NULL;
ArtFrmHdr_t *gDlgUnk80 = NULL;
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
int   gDlgBarterModifier = 0;
int   gDlgBackWindow = -1;
int   gDlgSurface = -1;
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
int gDlgUnk27 = 0;
int gDlgReplyWin = -1;
int gDlgUnk28 = -1;
int gDlgUnk29 = -1;
int gDlgUnk12 = -1;
int gDlgUnk11 = -1;
char *gDlgPalA = NULL;
char *gDlgPalB = NULL;
int gDlgUnk10 = 0;
int gDlgUnk32 = 0;
CachePool_t *gDlgArt[ 8 ] = { ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL, ART_NULL };
char *gDlgUnk36 = NULL;
int gDlgUnk37[ 12 ] = { 35, 35, 82, 35, 37, 46, 89, 90, 87, 88, 91, 92 };
Obj_t *gDlgUnk44 = NULL;
int gDlgUnk45 = 0;
short gDlgUnk46 = 0;
int gDlgUnk47 = -1;
int gDlgUnk48[ 42 ] = { 0, 3, 1, 1, 3, 1, 1, 1, 7, 8, 7, 3, 1, 8, 1, 7, 7, 6, 6, 2, 2, 2, 2, 4, 4, 5, 5, 2, 2, 2, 2, 2, 6, 2, 2, 5, 8, 2, 2, 2, 2, 8 };
int gDlgUnk49 = 0;
int gDlgUnk50 = -1;
int gDlgUnk51 = 10000;
const char *gDlgUnk52[ 3 ] = { "Said Good", "Said Neutral", "Said Bad" };
int gDlgUnk53 = 0;
Gdialog02_t gDlgUnk54[ 5 ] = {
    { 0x1B6, 0x25, 0x18D, 0x18B, 0x18C, NULL, NULL, NULL, 0x832, 4 },
    { 0x1B6, 0x43, 0x18A, 0x188, 0x189, NULL, NULL, NULL, 0x837, 3 },
    { 0x1B6, 0x60, 0x196, 0x194, 0x195, NULL, NULL, NULL, 0x836, 2 },
    { 0x1B6, 0x7E, 0x190, 0x18E, 0x18F, NULL, NULL, NULL, 0x83F, 1 },
    { 0x1B6, 0x9C, 0x193, 0x191, 0x192, NULL, NULL, NULL, 0x833, 0 },
};

int gDlgTextIds[ 6 ][ 12 ][ 2 ] = {
    { { 100, 0 }, { 101, 1 }, { 102, 2 }, { 103, 3 }, { 104, 4 }, { -1,  0 } },
    { { 200, 0 }, { 201, 1 }, { 202, 2 }, { 203, 3 }, { 204, 4 }, { 205, 5 } },
    { { 300, 0 }, { 301, 1 }, { 302, 2 }, { 303, 3 }, { 304, 4 }, { 305, 5 } },
    { { 400, 0 }, { 401, 1 }, { 402, 2 }, { 403, 3 }, { 404, 4 }, { -1,  0 } },
    { { 500, 0 }, { 501, 1 }, { 502, 2 }, { 503, 3 }, { 504, 4 }, { -1,  0 } },
    { { 600, 0 }, { 601, 1 }, { 602, 2 }, { 603, 3 }, { 604, 4 }, { -1,  0 } }
};

Gdialog02_t gDlgUnk71[ 6 ] = {
    { 95, 9,   410, 409, -1, NULL, NULL, NULL, 0, 0 },
    { 96, 38,  416, 415, -1, NULL, NULL, NULL, 1, 0 }, 
    { 96, 68,  418, 417, -1, NULL, NULL, NULL, 2, 0 },
    { 96, 98,  414, 413, -1, NULL, NULL, NULL, 3, 0 },
    { 96, 127, 408, 407, -1, NULL, NULL, NULL, 4, 0 },
    { 96, 157, 412, 411, -1, NULL, NULL, NULL, 5, 0 }
};
int gDlgUnk79 = 0;
int gDlgUnk105[ 6 ];
Msg_t gDlgMsg;
char gDlgLumA[ 256 ];
char gDlgLumB[ 256 ];
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
int gDlgImg100Width;
char *gDlgImgBtA0;
int gDlgImg101Width;
ArtFrmHdr_t *gDlgImg100Dat;
int gDlgImg101Height;
CachePool_t *gDlgUnk130;
CachePool_t *gDlgUnk131;
ArtFrmHdr_t *gDlgImg101Dat;
int gDlgFontSave;
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

int GdialogUnk01()
{
    return gDlgUnk06 != 0;
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
        tmp = ActionFindPath( gObjDude, target );
        if( tmp == -1 ){
            msg.Id = 660;
            if( MessageGetMsg( &gProtoMessages, &msg ) != 1 ){ eprintf( "\nError: gdialog: Can't find message!" ); return; }
            if( flg )
                IfcMsgOut( msg.Text );
            else
            	eprintf( "%s", msg.Text );
            return;
        }
        if( tmp == -2 ){
            msg.Id = 661;
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
    MapUnk35();
    gDlgUnk06 = 1;
    gDlgUnk44 = target;
    gDlgUnk45 = PartyMembRdy( target );
    gDlgUnk10 = 1;
    if( target->ScrId != -1 ) ScptRun( target->ScrId, SCPT_AEV_TALK_P_PROC );
    if( ScptPtr(target->ScrId, &scr) == -1 ){
        GmouseIsoEnter();
        MapUnk34();
        ScptUnk30();
        gDlgUnk06 = 0;
        return;
    }
    if( scr->OverrideFlag || gDlgUnk09 != 4 ){
        gDlgUnk10 = 0;
        MapUnk34();
        ScptUnk30();
        gDlgUnk06 = 0;
        return;
    }
    GdialogLipsyncEnd();
    if( gDlgUnk07 == 1 ){
	if( gDlgUnk08 == 2 || gDlgUnk08 == 8 || gDlgUnk08 == 11 ){
    	    GdialogUnk79();
	} else {
	    if( gDlgUnk08 == gDlgUnk07 ){
		GdialogUnk61();
	    } else {
    		if( gDlgUnk09 == gDlgUnk07 ){
        	    GdialogUnk79();
    		} else {
    		    if( gDlgUnk09 == 4 ){
			GdialogUnk61();
    		    }
    		}
    	    }
        }
	GdialogUnk06();
    }
    gDlgUnk07 = 0;
    gDlgUnk09 = 0;
    if( gDlgUnk11 != gObjDude->GridId ) gDlgUnk12 = gObjDude->GridId;
    if( gDlgUnk13 ) TileUnk49( gDlgUnk12, 2 );
    MapUnk34();
    ScptUnk30();
    gDlgUnk06 = 0;
    return;    
}

int GdialogUnk02()
{
    GlobVarUnk03();
    gDlgUnk18 = 1;
    SoundUpdateAll();
    GdialogEnter( gDlgUnk44, 0 );
    SoundUpdateAll();
    if( gDlgUnk11 != gObjDude->GridId ) gDlgUnk12 = gObjDude->GridId;
    if( gDlgUnk13 ) TileUnk49( gDlgUnk12, 2 );
    GlobVarUnk02();
    return GlobVarUnk03();
}

int GdialogLipsyncStart( char *DirName )
{
    char fname[ 16 ];

    if( !DirName ){ eprintf( "\nGDialog: Bleep!" ); return GSoundPlay( "censor" ); }
    if( ArtGetFilenameFromList( 8, gDlgUnk46 & 0xFFF, fname ) == -1 ) return -1;    
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

int GdialogUnk03()
{
    InpTaskStart( GdialogUnk51 );
    return 0;
}

int GdialogUnk04()
{
    InpTaskStop( GdialogUnk51 );
    return 0;
}

int GdialogUnk05( int a1, int a2 )
{
    if( gDlgUnk09 == 1 ) return -1;
    if( gDlgUnk07 == 1 ) return 0;
    AnimReset();    
//    gDlgUnk87 = IfaceUnk47();
    gDlgUnk45 = PartyMembRdy( gDlgUnk44 );
    gDlgFontSave = FontGetCurrent();
    FontSet( 101 );
    DialogReplyWin( 135, 225, 58, 379, 0 );
    DialogSetReplyColor( 0.30000001, 0.30000001, 0.30000001 );
    DialogOptWin( 127, 335, 117, 393, 0 );
    DialogSetOptionColor( 0.2, 0.2, 0.2 );
    DialogSetTitle( 0 );
    DialogUnk34( (void *)GdialogDemoCopyTitle, (void *)GdialogDemoCopyOptions );
    GdialogUnk85();
    CycleColorStop();
    if( gDlgUnk18 ) GmouseSetIfaceMode( 0 );
    GmouseUnk03();
    GmouseLoadCursor(1);
    TextFlush();
    if( OBJTYPE( gDlgUnk44->Pid ) ) TileUnk49( gDlgUnk44->GridId, 2 );
    gDlgUnk26 = 1;
    GdialogUnk40();
    InpTaskStart( GdialogUnk51 );
    GdialogUnk42( a1, a2 );
    gDlgUnk07 = 1;
    GmouseScrollDisable();
    if( a1 == -1 ){
	gDlgUnk29 = gDlgUnk29 / 2;
        GSoundSetMusicVol( gDlgUnk29 );        
    } else {
        gDlgUnk29 = -1;
        GSoundBgClose();
    }
    gDlgUnk13 = 1;
    return 0;
}

int GdialogUnk06()
{
    int Pid_high;

    if( gDlgUnk08 == 2 || gDlgUnk08 == 8 || gDlgUnk08 == 11 ) return -1;
    if( !gDlgUnk07 ) return 0;
    GdialogLipsyncEnd();
    GdialogUnk27();
    InpTaskStop( GdialogUnk51 );
    Pid_high = OBJTYPE( gDlgUnk44->Pid );
    if( Pid_high ){
        if( gDlgUnk11 != gObjDude->GridId ) gDlgUnk12 = gObjDude->GridId;
        Pid_high = TileUnk49( gDlgUnk12, 2 );
    }
    GdialogUnk41( Pid_high );
    FontSet( gDlgFontSave );
    if( gDlgUnk84 ){
        ArtClose( gDlgUnk85 );
        gDlgUnk84 = 0;
    }
    if( gDlgUnk81 ){
        if( ArtClose( gDlgUnk81 ) == -1 ) eprintf( "Failure unlocking lips frame!\n" );
        gDlgUnk81 = 0;
        gDlgUnk80 = 0;
        gDlgUnk82 = 0;
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
    if( gDlgUnk87 ) IfaceIndicatorBoxShow();
    gDlgUnk87 = 0;
    if( gDlgUnk18 ){
        if( !GameIfaceStat() ) GmouseSetIsoMode();
        gDlgUnk18 = 0;
    }
    if( !GameIfaceStat() ) GmouseIsoEnter();
    gDlgUnk13 = 1;
    return 0;
}

int GdialogUnk07( int a1 )
{
    if( a1 != -1 ) gDlgUnk83 = a1;
    return 0;
}

void GdialogReply( char *str )
{
    int Position = 0;
        
    if( gDlgReplyWin == -1 ) eprintf( "\nError: Reply window doesn't exist!" );
    gDlgBoxTitleArea.lt = 5;
    gDlgBoxTitleArea.tp = 10;
    gDlgBoxTitleArea.rt = 58;
    gDlgBoxTitleArea.bm = 374;
    GdialogDemoCopyTitle( gDlgBoxTitle );
    GdialogDisplayMsg( WinGetSurface( gDlgBoxTitle ), &gDlgBoxTitleArea, str, &Position, gFont.ChrHeight(), 379, DLG_COLOR_5 | 0x2000000, 1 );
    WinMoveTop( gDlgReplyWin );
    WinUpdate( gDlgBoxTitle );
}

void GdialogStart()
{
    gDlgReviewCount = 0;
    gDlgOption = 0;
}

void GdialogUnk09()
{
    MseDrawCursor();
    GdialogEnd();
    gDlgOption = 0;
    gDlgUnk137 = -1;
}

int GdialogUnk10( int a1, int a2, int a3 )
{
    gDlgOptions[ gDlgOption ].i04 = 0;
    return (GdialogUnk21( a1, a2, a3 ) != -1) - 1;
}

int GdialogUnk11( int a1, char *a2, int a4 )
{
    gDlgOptions[ gDlgOption ].i04 = 0;
    return (GdialogUnk22( a1, a2, a4 ) != -1) - 1;
}

int GdialogUnk12( int a1, int a2, int a3, int a4 )
{
    gDlgOptions[ gDlgOption ].i04 = a3;
    return (GdialogUnk21( a1, a2, a4 ) != -1) - 1;
}

int GdialogUnk13( int a1, char *a2, int a3, int a4 )
{
    gDlgOptions[ gDlgOption ].i04 = a3;
    return (GdialogUnk22( a1, a2, a4 ) != -1) - 1;
}

int GdialogUnk14( Intp_t *a1, int a2, int a3 )
{
    GdialogUnk28( a1, a2, a3 );
    gDlgUnk136 = a1;
    gDlgUnk137 = a2;
    gDlgUnk138 = a3;
    gDlgUnk139 = 0;
    gDlgTitleMsg[ 0 ] = 0;
    gDlgOption = 0;
    return 0;
}

int GdialogUnk15( Intp_t *a1, char *a2 )
{
    GdialogReviewAdd1( a1, a2 );
    gDlgUnk136 = a1;
    gDlgUnk139 = 0;
    gDlgUnk137 = -4;
    gDlgUnk138 = -4;
    strcpy( gDlgTitleMsg, a2 );
    gDlgOption = 0;
    return 0;
}

int GdialogEnd()
{
    int v0;

    v0 = 0;
    if( gDlgUnk137 == -1 ) return 0;
    if( gDlgOption < 1 ){
        gDlgOptions[ gDlgOption ].i04 = 0;
        if( GdialogUnk21( -1, -1, 50 ) == -1 ){ IntpError( "Error setting option." ); v0 = -1; }
    }
    if( v0 != -1 ) v0 = GdialogUnk34();
    gDlgOption = 0;
    return v0;
}

void GdialogUnk17()
{
    int v0;

    if( gDlgUnk09 != 1 ) return;
    v0 = PartyMembRdy( gDlgUnk44 );
    if( v0 == gDlgUnk45 ) return;
    if( gDlgReplyWin != -1 ) WinUpdateDirty( gDlgReplyWin );
    if( gDlgUnk28 != -1 ) WinUpdateDirty( gDlgUnk28 );
    GdialogUnk79();
    gDlgUnk45 = v0;
    GdialogUnk78();
    if( gDlgReplyWin != -1 ) WinMoveTop( gDlgReplyWin );
    if( gDlgUnk28 != -1 ) WinMoveTop( gDlgUnk28 );
}

int GdialogUnk18()
{
    if( gDlgReplyWin != -1 ) WinUpdateDirty( gDlgReplyWin );
    if( gDlgUnk28 != -1 ) WinUpdateDirty( gDlgUnk28 );
    return 0;
}

int GdialogUnk19()
{
    if( gDlgReplyWin != -1 ) WinMoveTop( gDlgReplyWin );
    if( gDlgUnk28 != -1 ) WinMoveTop( gDlgUnk28 );
    return 0;
}

int GdialogUnk20()
{
    if( gDlgReplyWin != -1 ) WinMoveTop( gDlgReplyWin );
    return 0;
}

int GdialogUnk21( int Page, int TxtId, int Reaction )
{
    if( gDlgOption >= 30 ){ eprintf( "\nError: dialog: Ran out of options!" ); return -1; }
    gDlgOptions[ gDlgOption ].Text[ 0 ] = '\0';
    gDlgOptions[ gDlgOption ].Button = -1;
    gDlgOptions[ gDlgOption ].MsgPage = Page;
    gDlgOptions[ gDlgOption ].MsgId = TxtId;
    gDlgOptions[ gDlgOption ].Reaction = Reaction;
    gDlgOption++;
    return 0;
}

int GdialogUnk22( int a1, char *Text, int Reaction )
{
    if( gDlgOption < 30 ){ eprintf( "\nError: dialog: Ran out of options!" ); return -1; }
    gDlgOptions[ gDlgOption ].MsgPage = -4;
    gDlgOptions[ gDlgOption ].MsgId = -4;
    sprintf( gDlgOptions[ gDlgOption ].Text, "%c %s", 149, Text );
    gDlgOptions[ gDlgOption ].Reaction = Reaction;
    gDlgOptions[ gDlgOption ].Button = -1;
    gDlgOption++;
    return 0;
}

int GdialogCreateReview( int *pWin )
{
    int win, i, bt, w;
    char *surf,* img, *imgs[ 6 ];

    if( gDlgLipsEn == 1 && SoundIsPlaying( gLipsync.Snd ) ) GdialogLipsyncEnd();
    gDlgUnk114 = FontGetCurrent();
    if( !pWin ) return -1;
    win = WinCreateWindow( 0, 0, gVidMainGeo.rt - gVidMainGeo.lt + 1, 480, 256, 20 );
    *pWin = win;
    if( win == -1 ) return -1;    
    if( (img = ArtGetBitmap( ArtMakeId( 6, 102, 0, 0, 0 ), 0, 0, &gDlgArt[ 6 ] ) ) ){ WinClose( *pWin ); *pWin = -1; return -1; }    
    w = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    surf = WinGetSurface( *pWin );
    ScrCopy( img, gVidMainGeo.rt - gVidMainGeo.lt + 1, 480, gVidMainGeo.rt - gVidMainGeo.lt + 1, surf, w );
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
    InpTaskStop( GdialogUnk51 );
    gDlgUnk36 = ArtGetBitmap( ArtMakeId( 6, 102, 0, 0, 0 ), 0, 0, &gDlgArt[ 7 ] );
    if( !gDlgUnk36 ){ GdialogCloseReview( pWin ); return -1; }
    return 0;
}

int GdialogCloseReview( int *pWin )
{
    int i;

    InpTaskStart( GdialogUnk51 );
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
    int pos, sel, win;

    pos = 0;
    if( GdialogCreateReview( &win ) == -1 ){
        eprintf( "\nError initializing review window!" );
        return -1;
    }
    GdialogUnk26( win, 0 );
    while( 1 ){
        sel = InpUpdate();
        if( sel == 17 || sel == 24 || sel == 324 ) SysQuitDlg();
        if( gMenuEscape || sel == 27 ) break;
        if( sel == 328 ){
            if( --pos < 0 ){ pos = 0; continue; }
        } else if( sel == 336 ){
            if( ++pos > (gDlgReviewCount - 1) ){
        	pos = gDlgReviewCount - 1;
        	continue;
            }
        }
	GdialogUnk26( win, pos );
    }
    if( GdialogCloseReview( &win ) == -1 ) return -1;
    return 0;    
}

void GdialogUnk26( int win, int pos )
{
    int v4; // ebx
    char *surf; // eax
    char *DialogA; // ebx
    char *v13; // ebx
    int v16; // [esp-4h] [ebp-70h]
    char stmp[60]; // [esp+0h] [ebp-6Ch] BYREF
    VidRect_t area; // [esp+3Ch] [ebp-30h] BYREF
    int h; // [esp+4Ch] [ebp-20h]
    char *v20; // [esp+50h] [ebp-1Ch]
    int i; // [esp+54h] [ebp-18h]

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
	&surf[v16 * area.tp + area.lt], v16
    );    
    for( i = pos; i < gDlgReviewCount; i++ ){
        sprintf( stmp, "%s:", ObjGetName( gDlgUnk44 ) );
        WinDrawText( win, stmp, 180, 88, v4, DLG_COLOR_5 | 0x2000000 );
        area.tp += h;
        if( gDlgReviewSlots[ i ].i01 <= -3 )
            DialogA = gDlgReviewSlots[ i ].i03;
        else
            DialogA = ScptGetDialogA( gDlgReviewSlots[ i ].i01, gDlgReviewSlots[ i ].i02 );
        if( !DialogA ){ WinMsgError( "\nGDialog::Error Grabbing text message!" ); exit( 1 ); }
        v4 = GdialogDisplayMsg( v20, &area, DialogA, 0, gFont.ChrHeight(), 640, DLG_COLOR_6 | 0x2000000, 1 );
        if( gDlgReviewSlots[ i ].i04 != -3 ){
            sprintf( stmp, "%s:", ObjGetName( gObjDude ) );
            WinDrawText( win, stmp, 180, 88, v4, DLG_COLOR_4 | 0x2000000 );
            area.tp += h;
            if( gDlgReviewSlots[ i ].i04 <= -3 )
                v13 = gDlgReviewSlots[ i ].i06;
            else
                v13 = ScptGetDialogA( gDlgReviewSlots[ i ].i04, gDlgReviewSlots[ i ].i05 );
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
        gDlgReviewSlots[ i ].i02 = 0;
        gDlgReviewSlots[ i ].i04 = 0;
        gDlgReviewSlots[ i ].i05 = 0;
        gDlgReviewSlots[ i ].i01 = 0;
        if( gDlgReviewSlots[ i ].i03 ){ Free( gDlgReviewSlots[ i ].i03 ); gDlgReviewSlots[ i ].i03 = NULL; }
    }
    gDlgReviewCount = 0;
}

int GdialogUnk28( Intp_t *a1, int a2, int a3 )
{
    if( gDlgReviewCount >= 80 ){ eprintf( "\nError: Ran out of review slots!" ); return -1; }
    gDlgReviewSlots[ gDlgReviewCount ].i01 = a2;
    gDlgReviewSlots[ gDlgReviewCount ].i02 = a3;
    gDlgReviewSlots[ gDlgReviewCount ].i04 = -1;
    gDlgReviewSlots[ gDlgReviewCount ].i05 = -1;
    gDlgReviewSlots[ gDlgReviewCount ].i04 = -3;
    gDlgReviewSlots[ gDlgReviewCount ].i05 = -3;
    gDlgReviewCount++;
    return 0;
}

int GdialogReviewAdd1( Intp_t *a1, char *Text )
{
    if( gDlgReviewCount >= 80 ){ eprintf( "\nError: Ran out of review slots!" ); return -1; }
    gDlgReviewSlots[ gDlgReviewCount ].i01 = -4;
    gDlgReviewSlots[ gDlgReviewCount ].i02 = -4;
    if( gDlgReviewSlots[ gDlgReviewCount ].i03 ) Free( gDlgReviewSlots[ gDlgReviewCount ].i03 );
    gDlgReviewSlots[ gDlgReviewCount ].i03 = Malloc( strlen( Text ) + 1 );
    strcpy( gDlgReviewSlots[ gDlgReviewCount ].i03, Text );
    gDlgReviewSlots[ gDlgReviewCount ].i04 = -1;
    gDlgReviewSlots[ gDlgReviewCount ].i05 = -1;
    gDlgReviewSlots[ gDlgReviewCount ].i06 = 0;
    gDlgReviewSlots[ gDlgReviewCount ].i04 = -3;
    gDlgReviewSlots[ gDlgReviewCount ].i05 = -3;
    gDlgReviewCount++;
    return 0;
}

int GdialogReviewAdd2( int a1, int a2 )
{
    int tmp;

    if( gDlgReviewCount >= 80 ){ eprintf( "\nError: Ran out of review slots!" ); return -1; }
    tmp = gDlgReviewCount - 1;
    gDlgReviewSlots[ tmp ].i04 = a1;
    gDlgReviewSlots[ tmp ].i05 = a2;
    gDlgReviewSlots[ tmp ].i06 = 0;
    return 0;
}

int GdialogReviewAdd3( char *Text )
{
    int n;
    char *s;

    if( gDlgReviewCount >= 80 ){ eprintf( "\nError: Ran out of review slots!" ); return -1; }
    n = gDlgReviewCount - 1;
    gDlgReviewSlots[ n ].i04 = -4;
    gDlgReviewSlots[ n ].i05 = -4;
    s = Malloc( strlen( Text ) + 1 );
    gDlgReviewSlots[ n ].i06 = s;
    strcpy( s, Text + 2 );
    return 0;
}

int GdialogUnk30()
{
    int bt;

    if( (gDlgBoxTitle = WinCreateWindow( 135, 225, 379, 58, 256, 4 )) == -1 ) return -1;    
    if( (bt = WinCreateButton( gDlgBoxTitle, 1, 1, 377, 28, -1, -1, 328, -1, 0, 0, 0, 32 ) ) != -1 ){
        WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
        WinSetButtonHandler( bt, GdialogUnk45, GdialogUnk47, 0, 0 );
        if( (bt = WinCreateButton( gDlgBoxTitle, 1, 29, 377, 28, -1, -1, 336, -1, 0, 0, 0, 32 ) ) != -1 ){
            WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
            WinSetButtonHandler( bt, GdialogUnk46, GdialogUnk47, 0, 0 );
            if( (gDlgBoxWin = WinCreateWindow( 127, 335, 393, 117, 256, 4 ) ) != -1 ){
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
            WinClose( gDlgBoxWin );
            gDlgBoxWin = -1;
        }
    }
    WinClose( gDlgBoxTitle );
    gDlgBoxTitle = -1;    
    return -1;
}

int GdialogUnk31()
{
    int i;
    
    for( i = 0; i < gDlgOption; i++ ){
        if( gDlgOptions[ i ].Button == -1 ) continue;
        WinImageRemove( gDlgOptions[ i ].Button );
        gDlgOptions[ i ].Button = -1;
    }
    return 0;
}

int GdialogUnk32()
{
    GdialogUnk31();
    ArtClose( gDlgUnk130 );
    gDlgUnk130 = 0;
    gDlgImgBtA1 = 0;
    ArtClose( gDlgUnk115 );
    gDlgUnk115 = 0;
    gDlgImgBtA0 = 0;
    WinClose( gDlgBoxTitle );
    gDlgBoxTitle = -1;
    WinClose( gDlgBoxWin );
    gDlgBoxWin = -1;
    FontSet( gDlgUnk149 );
    return 0;
}

void GdialogUnk33()
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
    WinDrawText( gDlgSurface, stmp, w, 38 - w / 2, 36, DLG_COLOR_5 | 0x7000000 );
    FontSet( font );
}

int GdialogUnk34() // xxxxx
{
    unsigned int v10;
    int v0,v1,v3,v5,v7,Time,v9,v12,v13,v14,v17[11],SysTime,v19;

    v0 = 0;
    v1 = 0;
    v19 = 0;
    v17[10] = 10000;
    if( !gDlgUnk17 ){
        if( GdialogUnk30() == -1 ) return -1;
    }
    v17[0] = 0;
    gDlgUnk17++;
    GdialogCreateDialog();
    if( gDlgUnk139 ){ v19 = 1; gDlgUnk16 = 1; }
    SysTime = TimerGetSysTime();
    while( 1 ){
        v3 = InpUpdate();
        v5 = v3;
        if( v3 == 17 || v3 == 24 || v3 == 324 ) SysQuitDlg();
        if( gMenuEscape ) break;
        if( v5 == 2 && !MseCursorCenterInArea( 135, 225, 514, 283 ) ){
            if( GmouseGetCursorId() != 1 ) GmouseLoadCursor( 1 );
            goto LABEL_24;
        }
        switch( gDlgUnk08 ){
            case 3:
                gDlgUnk09 = 4;
                InvMenuBarter( gDlgSurface, gDlgUnk44, gDlgUnk20, gDlgUnk15, gDlgBarterModifier );
                GdialogUnk62();
                v7 = gDlgUnk09;
                GdialogUnk61();
                gDlgUnk09 = v7;
                if( gDlgUnk09 == 4 ){ gDlgUnk08 = 1; gDlgUnk09 = 1; }
                break;
            case 9:
                gDlgUnk09 = 10;
                GdialogUnk69();
                GdialogUnk64();
                break;
            case 12:
                gDlgUnk09 = 13;
                GdialogUnk72();
                GdialogUnk71();
                break;
            default:
                if( v5 == 98 ) GdialogBarter();
LABEL_24:
                if( gDlgUnk16 ){
                    Time = TimerGetTime();
                    v9 = Time;
                    if( v19 ){
                        v10 = TimerDiff(Time, SysTime);
                        if( v10 >= v17[10] || v5 == 32 ){
                            ++v0;
                            ++v1;
                            v17[v0] = gDlgUnk139;
                            GdialogDisplayTitle();
                            SysTime = v9;
                            if ( !gDlgUnk139 ) v19 = gDlgUnk139;
                        }
                    }
                    if( v5 == 328 ){
                        if( v1 > 0 ){
                            v12 = v17[--v1];
                            gDlgUnk139 = v12;
                            v19 = 0;
                            GdialogDisplayTitle();
                        }
                    } else if( v5 == 336 ){
                        if( v1 < v0 ){
                            v13 = v17[++v1];
                            gDlgUnk139 = v13;
                            v19 = 0;
                            GdialogDisplayTitle();
                        }
                        if( gDlgUnk139 ){
                            SysTime = v9;
                            ++v1;
                            v17[++v0] = gDlgUnk139;
                            v19 = 0;
                            GdialogDisplayTitle();
                        }
                    }
                }
                if( v5 != -1 ){
                    if( v5 < 1200 || v5 > 1250 ){
                        if( v5 < 1300 || v5 > 1330 ){
                            if( v5 >= 48 && v5 <= 57 ){
                                v14 = v5 - 49;
                                if ( v14 < gDlgOption ){
                                    v0 = 0;
                                    v1 = 0;
                                    v17[0] = 0;
                                    gDlgUnk16 = 0;
                                    if( GdialogUnk35(v14) != -1 ){
                                	SysTime = TimerGetSysTime();
                                	if( gDlgUnk139 ){
                                    	    v19 = 1;
                                    	    gDlgUnk16 = 1;
                                	} else {
                                    	    v19 = 0;
                                	}
                                    }
                                }
                            }
                        } else {
                            GdialogDisplayLine( v5 - 1300 );
                        }
                    } else {
                        GdialogDisplayOption( v5 - 1200 );
                    }
                }
                goto LABEL_53;
        }
    }
LABEL_53:
    if( --gDlgUnk17 ) return 0;
    if( GdialogUnk32() == -1 ) return -1;
    return 0;    
}

int GdialogUnk35( int a1 )
{
    int reaction;
        
    MseCursorRedraw();
    GdialogUnk31();
    if( gDlgOptions[ a1 ].MsgPage == -4 )
        GdialogReviewAdd3( gDlgOptions[ a1 ].Text );
    else
        GdialogReviewAdd2( gDlgOptions[ a1 ].MsgPage, gDlgOptions[ a1 ].MsgId );
    gDlgUnk27 = 0;
    GdialogLipsyncEnd();

    reaction = 0;
    switch( gDlgOptions[ a1 ].Reaction ){
	case 49: reaction = -1; break;
	case 50: reaction = 0; break;
	case 51: reaction = 1; break;
	default: eprintf( "\nError: dialog: Empathy Perk: invalid reaction!" ); break;
    }
    GdialogDemoCopyTitle( gDlgBoxTitle );
    GdialogDemoCopyOptions( gDlgBoxWin );
    WinUpdate( gDlgBoxTitle );
    WinUpdate( gDlgBoxWin );
    GdialogDisplayOption( a1 );
    GdialogReaction( reaction );
    gDlgOption = 0;
    if( gDlgUnk17 < 2 ){
        if( gDlgOptions[ a1 ].i04 ) SciRunProcedure( gDlgUnk136, gDlgOptions[ a1 ].i04 );
    }
    MseDrawCursor();
    if( !gDlgOption ) return -1;
    GdialogCreateDialog();
    return 0;
}

void GdialogDisplayOption( int LineNo )
{
    Gdialog_t *p;
    int color;

    p = &gDlgOptions[ LineNo ];
    if( !p->Button ) return;
    gDlgBoxWinArea.tp = p->Top;
    gDlgBoxWinArea.lt = 0;
    gDlgBoxWinArea.bm = p->Bottom;
    gDlgBoxWinArea.rt = 391;
    GdialogRfshOptionRect( gDlgBoxWin, &gDlgBoxWinArea );
    gDlgBoxWinArea.lt = 5;
    gDlgBoxWinArea.rt = 388;
    color = DLG_COLOR_5 | 0x2000000;
    if( PerkLvl( gObjDude, PERK_EMPATHY ) ){
	switch( p->Reaction ){
	    case 49: color = DLG_COLOR_8 | 0x2000000; break;
	    case 50: color = DLG_COLOR_5 | 0x2000000; break;
	    case 51: color = DLG_COLOR_9 | 0x2000000; break;
	    default: eprintf( "\nError: dialog: Empathy Perk: invalid reaction!" ); break;
	}
    }
    GdialogDisplayMsg( WinGetSurface( gDlgBoxWin ), &gDlgBoxWinArea, p->Text, NULL, gFont.ChrHeight(), 393, color, 1 );
    gDlgBoxWinArea.lt = 0;
    gDlgBoxWinArea.rt = 391;
    gDlgBoxWinArea.tp = p->Top;
    WinAreaUpdate( gDlgBoxWin, &gDlgBoxWinArea ); 
}

void GdialogDisplayLine( int LineNo )
{
    Gdialog_t *p;
    int color;

    p = &gDlgOptions[ LineNo ];
    gDlgBoxWinArea.lt = 0;
    gDlgBoxWinArea.tp = p->Top;
    gDlgBoxWinArea.bm = p->Bottom;
    gDlgBoxWinArea.rt = 391;
    GdialogRfshOptionRect( gDlgBoxWin, &gDlgBoxWinArea );
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
    GdialogDisplayMsg( WinGetSurface( gDlgBoxWin ), &gDlgBoxWinArea, p->Text, NULL, gFont.ChrHeight(), 393, color, 1 );
    gDlgBoxWinArea.rt = 391;
    gDlgBoxWinArea.tp = p->Top;
    gDlgBoxWinArea.lt = 0;
    WinAreaUpdate( gDlgBoxWin, &gDlgBoxWinArea );
}

void GdialogDisplayTitle()
{
    gDlgBoxTitleArea.lt = 5;
    gDlgBoxTitleArea.tp = 10;
    gDlgBoxTitleArea.rt = 374;
    gDlgBoxTitleArea.bm = 58;
    if( PerkLvl( gObjDude, PERK_EMPATHY ) ); // not implemented
    GdialogDemoCopyTitle( gDlgBoxTitle );
    GdialogDisplayMsg( WinGetSurface( gDlgBoxTitle ), &gDlgBoxTitleArea, gDlgTitleMsg, &gDlgUnk139, gFont.ChrHeight(), 379, DLG_COLOR_5 | 0x2000000, 1 );
    WinUpdate( gDlgBoxTitle );
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
    GdialogDemoCopyTitle( gDlgBoxTitle );
    GdialogDemoCopyOptions( gDlgBoxWin );
    LineErr = PerkEmpathy = 0;
    if( gDlgUnk137 > 0 ){
        Dialog = ScptGetDialog( gDlgUnk137, gDlgUnk138, 1 );
        if( !Dialog ){ WinMsgError( "\nGDialog::Error Grabbing text message!" ); exit( 1 ); }
        strncpy( gDlgTitleMsg, Dialog, 899 );
        gDlgTitleMsg[ 899 ] = '\0';
    }
    GdialogDisplayTitle();
    MsgColor = DLG_COLOR_5 | 0x2000000;
    if( PerkLvl( gObjDude, PERK_EMPATHY ) ) PerkEmpathy = 1;
    w = gDlgBoxWinArea.rt - gDlgBoxWinArea.lt - 4;    
    if( gDlgOption > 0 ){        
        for( i = 0; i < gDlgOption; i++ ){
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
                    if( i <= 0 ){
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
                Surface = WinGetSurface( gDlgBoxWin );
                GdialogDisplayMsg( Surface, &gDlgBoxWinArea, p->Text, NULL, gFont.ChrHeight(), 393, MsgColor, 1 );
                gDlgBoxWinArea.tp += 2;
                if( (p->Button = WinCreateButton( gDlgBoxWin, 2, Ypos, w, gDlgBoxWinArea.tp - Ypos - 4, 1200 + i,  1300 + i, -1, '1' + i, 0, 0, 0, 0 )) == -1 )
                    eprintf( "\nError: Can't create button!" );
                else
                    WinSetClickSound( p->Button, GSoundPlayPushBt, GSoundPlayReleaseBt );
            }            
        }
    }
    GdialogUnk33();
    WinUpdate( gDlgBoxTitle );
    WinUpdate( gDlgBoxWin );
}

int GdialogUnk40()
{
    int i, n, w;
    char *surf;

    gDlgUnk09 = 1;    
    gDlgBackWindow = WinCreateWindow( 0, 0, gVidMainGeo.rt - gVidMainGeo.lt + 1, 480, 256, 2 );
    GdialogRenderB();
    surf = WinGetSurface( gDlgBackWindow );
    for( i = 0; i < 8; i++){
        SoundUpdateAll();
        if( !( gDlgUnk97[ i ] = Malloc( ( gDlgUnk22[ i ].bm - gDlgUnk22[ i ].tp ) * ( gDlgUnk22[ i ].rt - gDlgUnk22[ i ].lt ) ) ) ) return -1;
        w = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        n = gDlgUnk22[ i ].rt - gDlgUnk22[ i ].lt;
        ScrCopy( &surf[ gDlgUnk22[ i ].tp * w + gDlgUnk22[ i ].lt ], n, gDlgUnk22[ i ].bm - gDlgUnk22[ i ].tp, w, gDlgUnk97[ i ], n );
    }
    GdialogUnk78();
    gDlgSurf = WinGetSurface( gDlgBackWindow ) + 14 * ( gVidMainGeo.rt - gVidMainGeo.lt + 1 ) + 126;
    if( !((gDlgSurf) == 0) ) GdialogUnk41();
    return 0;
}

void GdialogUnk41()
{
    int i;

    if( gDlgSurface != -1 ) gDlgSurf = 0;
    if( gDlgUnk09 == 1 ){
        GdialogUnk79();
    } else if( gDlgUnk09 == 4 ){
        GdialogUnk61();
    }
    if( gDlgBackWindow != -1 ){
        WinClose(gDlgBackWindow);
        gDlgBackWindow = -1;
    }
    for( i = 0; i != 8; i++ ) Free( gDlgUnk97[ i ] );
}

void GdialogUnk42( int a1, unsigned int a2)
{
    short v2;
    int v4, HeadsInfo, Random;
    unsigned int fidget;
    char v12[ 200 ], v13[ 220 ];

    v2 = a1;
    v4 = 10;
    gDlgUnk151 = 0;
    if( a1 == -1 ){
        gDlgUnk86 = -1;
        gDlgUnk84 = 0;
        gDlgUnk85 = ART_NULL;
        gDlgUnk135 = -1;
        gDlgUnk150 = 0;
        gDlgUnk134 = 0;
        GdialogRenderHead( 0, 0 );
        gDlgUnk82 = 0;
        gDlgUnk81 = NULL;
        gDlgUnk80 = 0;
        return;
    }
    if( a2 < 4 ){
        if( a2 == 1 ) v4 = 9;
    } else if( a2 > 4 && a2 == 7 ){
        v4 = 11;
    }
    if( gDlgUnk82 && v4 != gDlgUnk49 ){
        if( ArtClose( gDlgUnk81 ) == -1 ) eprintf( "failure unlocking lips frame!\n" );
        gDlgUnk81 = 0;
        gDlgUnk80 = 0;
        gDlgUnk82 = 0;
    }
    if( !gDlgUnk82 ){
        gDlgUnk49 = v4;
        gDlgUnk82 = ArtMakeId( 8, v2, v4, 0, gDlgUnk82 );
        gDlgUnk80 = ArtLoadImg( gDlgUnk82, &gDlgUnk81 );
        if( !gDlgUnk80 ){
            eprintf( "failure!\n" );
            CacheStats( &gArtCacheDb, v13 );
            eprintf( "%s", v13 );
        }
    }
    if( !(HeadsInfo = ArtGetHeadsInfo( ArtMakeId(8, v2, a2, 0, 0) )) ){
	eprintf( "\tError - No available fidgets for given frame id\n" );
        return;
    }
    Random = RandMinMax(1, 100) + gDlgUnk32 / 2;
    fidget = HeadsInfo;
    if( HeadsInfo < 2 ){
        if( HeadsInfo != 1 ){
	    if( Random < 52 )
    		fidget = 1;
	    else 
		fidget = ( Random < 77 ) ? 2 : 3;
	}
    } else {
        if( HeadsInfo <= 2 ){
            fidget = ( Random < 68 ) ? 1:2;
        } else {
    	    if( HeadsInfo == 3 ) gDlgUnk32 = 0;
	    if( Random < 52 )
    		fidget = 1;
	    else 
		fidget = ( Random < 77 ) ? 2 : 3;
	}
    }
    eprintf( "Choosing fidget %d out of %d\n", fidget, HeadsInfo );
    if( gDlgUnk84 && ArtClose( gDlgUnk85 ) == -1 ) eprintf( "failure!\n" );
    gDlgUnk86 = ArtMakeId(8, v2, a2, fidget, 0);
    gDlgUnk151 = 0;
    gDlgUnk84 = ArtLoadImg(gDlgUnk86, &gDlgUnk85);
    if( !gDlgUnk84 ){
        eprintf( "failure!\n" );
        CacheStats( &gArtCacheDb, v12 );
        eprintf( "%s", v12 );
    }
    gDlgUnk134 = 0;
    gDlgUnk135 = a2;
    gDlgUnk150 = 1000 / ArtGetObjSpeed( gDlgUnk84 );
}

void GdialogUnk43()
{
    if( gDlgUnk84 && gDlgSurface != -1 ){
        eprintf("Waiting for fidget to complete...\n");
        while ( ArtGetFpd(gDlgUnk84) > gDlgUnk151 ){
            if ( TimerCurrDiff(gDlgUnk134) >= gDlgUnk150 ){
                GdialogRenderHead(gDlgUnk84, gDlgUnk151);
                gDlgUnk134 = TimerGetSysTime();
                gDlgUnk151++;
            }
        }
        gDlgUnk151 = 0;
    }
}

void GdialogTransition( int a1 )
{
    ArtFrmHdr_t *Img; // ecx
    unsigned int v7; // esi
    int v8; // ebx
    unsigned int v9; // edx
    CachePool_t *ImgObj; // [esp+0h] [ebp-18h] BYREF

    if ( gDlgUnk84 && gDlgSurface == -1 ) return;
    MseCursorRedraw();
    eprintf( "Starting transition...\n" );
    GdialogUnk43();
    if( gDlgUnk84 ){
        if( ArtClose( gDlgUnk85 ) == -1 ) eprintf( "\tError unlocking fidget in transition func..." );
        gDlgUnk84 = 0;
    }
    if( !(Img = ArtLoadImg( ArtMakeId( 8, gDlgUnk46, a1, 0, 0 ), &ImgObj ) ) ) eprintf( "\tError locking transition...\n" );
    v7 = 1000 / ArtGetObjSpeed( Img );  
    v9 = 0;  
    for( v8 = 0; v8 < ArtGetFpd( Img ); v8++ ){
        if( TimerCurrDiff( v9 ) >= v7 ){
            GdialogRenderHead( Img, v8 );
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

void GdialogDemoCopyTitle( int a1 )
{
    int Width; // ebx
    char *Surface; // eax
    char *v6; // eax
    int v7; // ecx

    gDlgReplyWin = a1;
    if( a1 == -1 ){
        eprintf("\nError: demo_copy_title: win invalid!");
    } else {
        Width = WinGetWidth( a1 );
        if( Width >= 1 ){
            if( (v7 = WinGetHeight( a1 )) >= 1 ){
                if( gDlgBackWindow == -1 ){
                    eprintf("\nError: demo_copy_title: dialogueBackWindow wasn't created!");
                } else {
                    Surface = WinGetSurface(gDlgBackWindow);
                    if( Surface ){
                        v6 = WinGetSurface( a1 );
                        ScrCopy( Surface + 144135, Width, v7, 640, v6, Width );
                    } else {
                        eprintf("\nError: demo_copy_title: couldn't get buffer!");
                    }
                }
            } else {
                eprintf("\nError: demo_copy_title: length invalid!");
            }
        } else {
            eprintf("\nError: demo_copy_title: width invalid!");
        }
    }
}

void GdialogDemoCopyOptions( int a1 )
{
    int Height; // esi
    int v5; // ecx
    char *Surface; // eax
    VidRect_t v10; // [esp-4h] [ebp-24h] BYREF

    gDlgUnk28 = a1;
    if( a1 == -1 ){
        eprintf("\nError: demo_copy_options: win invalid!");
    } else if( (v5 = WinGetWidth( a1 )) >= 1 ){
        Height = WinGetHeight( a1 );
        if( Height >= 1 ){
            if( gDlgBackWindow == -1 ){
                eprintf("\nError: demo_copy_options: dialogueBackWindow wasn't created!");
            } else {
                WinGetRect( gDlgSurface, &v10 );
                if( WinGetSurface( gDlgSurface ) ){
                    Surface = WinGetSurface( a1 );
                    ScrCopy( &Surface[ 640 * ( 335 - v10.tp ) + 127 ], v5, Height, 640, Surface, v5 );
                } else {
                    eprintf("\nError: demo_copy_options: couldn't get buffer!");
                }
            }
        } else {
            eprintf("\nError: demo_copy_options: length invalid!");
        }
    } else {
        eprintf("\nError: demo_copy_options: width invalid!");
    }
}

void GdialogRfshOptionRect( int win, VidRect_t *DrawRect )
{
    char *Surface; // eax
    char *v5; // esi
    int bm; // ebp
    int tp; // edx
    int v8; // ebp
    int v9; // edi
    char *v10; // esi
    char *v12; // eax
    int v13; // edx
    VidRect_t v14; // [esp+0h] [ebp-28h] BYREF
    int Width; // [esp+10h] [ebp-18h]

    if( DrawRect ){
        if ( win == -1 ){
            eprintf("\nError: gDialogRefreshOptionsRect: win invalid!");
        } else if ( gDlgBackWindow == -1 ){
            eprintf("\nError: gDialogRefreshOptionsRect: dialogueBackWindow wasn't created!");
        } else {
            WinGetRect(gDlgSurface, &v14);
            Surface = WinGetSurface(gDlgSurface);
            if( Surface ){
                if( DrawRect->tp < DrawRect->bm ){
                    if ( DrawRect->lt < DrawRect->rt ){
                        v5 = &Surface[640 * (335 - v14.tp) + 127];
                        bm = DrawRect->bm;
                        tp = DrawRect->tp;
                        Width = DrawRect->rt - DrawRect->lt;
                        v8 = bm - tp;
                        v13 = WinGetWidth( win );
                        v9 =  v13 * DrawRect->tp;
                        v10 = &v5[640 * DrawRect->tp + DrawRect->lt];
                        v12 = WinGetSurface( win );
                        ScrCopy( v10, Width, v8, 640, &v12[v9 + DrawRect->lt], v13 );
                    } else {
                        eprintf( "\nError: gDialogRefreshOptionsRect: Invalid Rect (too many options)!" );
                    }
                } else {
                    eprintf( "\nError: gDialogRefreshOptionsRect: Invalid Rect (too many options)!" );
                }
            } else {
                eprintf( "\nError: gDialogRefreshOptionsRect: couldn't get buffer!" );
            }
        }
    } else {
        eprintf( "\nError: gDialogRefreshOptionsRect: drawRect NULL!" );
    }
}

void GdialogUnk51( int a1 )
{
    switch( gDlgUnk08 ){
        case 2:
            gDlgUnk50 = -1; 
            gDlgUnk08 = 3; 
            GdialogUnk79(); 
            GdialogUnk60(); 
            break;
        case 1:
            gDlgUnk50 = -1;
            gDlgUnk08 = 0;
            GdialogUnk61();
            GdialogUnk78();
            if( gDlgReplyWin != -1 ) WinMoveTop( gDlgReplyWin );
            if( gDlgUnk28 != -1 ) WinMoveTop( gDlgUnk28 );
            break;
        case 8:
            gDlgUnk50 = -1;
            gDlgUnk08 = 9;
            GdialogUnk79();
            GdialogUnk63();
            break;
        case 11:
            gDlgUnk50 = -1;
            gDlgUnk08 = 12;
            GdialogUnk79();
            GdialogUnk70();
            break;
    }
    if( gDlgUnk84 ){
        if( gDlgLipsEn == 1 ){
            LipsyncSpeachStop();
            if( gLipsUnk04 ){
                GdialogRenderHead( gDlgUnk80, gDlgUnk48[ (int)gLipsPhoneme ] );
                gLipsUnk04 = 0;
            }
            if( !SoundIsPlaying( gLipsync.Snd ) ){
                GdialogLipsyncEnd();
                GdialogRenderHead( gDlgUnk80, 0 );
                gDlgUnk27 = 1;
                gDlgUnk32 = 3;
                gDlgUnk151 = 0;
            }
        }
        else if( gDlgUnk27 ){
            if( TimerCurrDiff(gDlgUnk134) >= gDlgUnk51 ){
                gDlgUnk27 = 0;
                gDlgUnk32 += gDlgUnk51 / 0x3E8u;
                gDlgUnk51 = 1000 * (RandMinMax(0, 3) + 4);
                GdialogUnk42(gDlgUnk86 & 0xFFF, (gDlgUnk86 & 0xFF0000) >> 16);
            }
        } else if( TimerCurrDiff(gDlgUnk134) >= gDlgUnk150 ){
            if( ArtGetFpd(gDlgUnk84) <= gDlgUnk151 ){
                GdialogRenderHead(gDlgUnk84, 0);
                gDlgUnk27 = 1;
            } else {
                GdialogRenderHead(gDlgUnk84, gDlgUnk151);
                gDlgUnk134 = TimerGetSysTime();
                gDlgUnk151++;
            }
        }
    }
}

void GdialogReaction( int a1 )
{
    int v1, v4, v6;

    v1 = a1 + 1;
    eprintf( "Dialogue Reaction: " );
    if( v1 < 3 ) eprintf( "%s\n", gDlgUnk52[ v1 ] );
    gDlgUnk32 = 0;
    if( a1 >= -50 ){
        if( a1 != -1 ) return;
        if( gDlgUnk135 < 4 ){
            if( gDlgUnk135 != 1 ) return;
            v4 = 0;
            v6 = 1;
        } else if( gDlgUnk135 <= 4 ){
            v4 = 3;
            v6 = 1;
        } else {
            if( gDlgUnk135 != 7 ) return;
            v4 = 6;
            v6 = 4;
        }
    } else {
	if( a1 >= -50 || a1 == 0 || a1 != 1 ) return;    
	if( gDlgUnk135 < 4 ){
            if( gDlgUnk135 == 1 ){
                GdialogTransition( 2 );
                GdialogUnk42( gDlgUnk46, 4 );
            }
            return;
        }
        if( gDlgUnk135 <= 4 ){
            v4 = 5;
        } else {
            if( gDlgUnk135 != 7 ) return;
            v4 = 8;
        }
        v6 = 7;
    }
    GdialogTransition( v4 );
    GdialogUnk42( gDlgUnk46, v6 );    
}

int GdialogUnk53( int a1, int a2, char *a3, char *a4, char *a5, int a6, int a7 )
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
    int Width, flg, h;

    s = NULL;
    str = ( pPosition ) ? &Text[ *pPosition ] : Text;
    Width = Area->rt - Area->lt;
    while( str ){
        if( *str == '\0' ) break;;
        if( gFont.LineWidth( str ) > Width ){	    
    	    for( s = str + 1; *s && *s != ' '; s++ );
	    if( *s ){
    		p = s + 1;
    		while( 1 ){
            	    for( ; *p && *p != ' '; p++ );
            	    if( *p ){
            		*p = '\0';
            		if( gFont.LineWidth( str ) >= Width ){
        		    p = NULL;
        		    flg = 1;
            		} else {
            		    s = p;
            		    *p++ = ' ';
            		    flg = (p == NULL);
            		}
            		if( flg ){ 
    			    if( *s == ' ' ) *s = '\0';
    			    break;
            		}
        	    }
        	    *p = ' ';
        	}
	    } else {
		if( Flag ){
    		    h = gFont.ChrHeight();
    		    if( Area->bm - h < Area->tp ) return Area->tp;
    		    if( Flag != 1 || str == Text )
    			gFont.Print( &Surf[ Pitch * Area->tp + 10 ], str, Width, Pitch, Color );
    		    else
        		gFont.Print( &Surf[ Pitch * Area->tp ], str, Width, Pitch, Color );
		}
		if( pPosition ) *pPosition += strlen( str ) + 1;
		Area->tp = Height + Area->tp;
		return Area->tp;
	    }
        }
        if( gFont.LineWidth( str ) > Width ){ eprintf( "\nError: display_msg: word too long!" ); return Area->tp; }
        if( Flag ){
            h = gFont.ChrHeight();
            if( Area->bm - h < Area->tp ){
                if( s && !*s ){ *s = ' '; return Area->tp; }
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
    if( !gDlgUnk06 ) return -1;
    gDlgBarterModifier = BarterMod;
    GdialogBarter();
    gDlgUnk09 = 4;
    gDlgUnk08 = 2;
    return 0;
}

void GdialogUnk59()
{
    DialogQuit();
    DialogFree();
    SciUnk19();
    WinRun();
    gDlgUnk09 = 1;
    gDlgUnk08 = 1;
}

int GdialogUnk60()
{
    ArtFrmHdr_t *Img;
    char *ObjData,*Surface,*v7;
    int ObjHeight,v15,h;
    CachePool_t *ImgObj;

    gDlgUnk09 = 4;    
    if( !( Img = ArtLoadImg( ( gDlgUnk45 ) ? ArtMakeId( 6, 420, 0, 0, 0 ) : ArtMakeId( 6, 111, gDlgUnk45, 0, 0 ), &ImgObj ) ) ) return -1;
    if( ( ObjData = ArtGetObjData( Img, 0, 0 ) ) ){
            ObjHeight = ArtGetObjHeight( Img, 0, 0 );
            gDlgUnk53 = ObjHeight;
            h = 480 - ObjHeight;            
            if( ( gDlgSurface = WinCreateWindow( 0, h, gVidMainGeo.rt - gVidMainGeo.lt + 1, ObjHeight, 256, 2 ) ) != -1 ){
                Surface = WinGetSurface( gDlgSurface );
                v15 = gVidMainGeo.rt - gVidMainGeo.lt + 1;
                v7 = WinGetSurface( gDlgBackWindow );
                ScrCopy( &v7[ v15 * h ], v15, ObjHeight, v15, Surface, v15 );
                GdialogUnk53( gDlgSurface, 1, ObjData, Surface, 0, ObjHeight, 0 );
                ArtClose( ImgObj );
                if( (gDlgButtons[ 0 ] = WinCreateButton( gDlgSurface, 41, 163, 14, 14, -1, -1, -1, 109, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) != -1 ){
                    WinSetClickSound( gDlgButtons[ 0 ], GSoundPlayCheck, GSoundPlayUnCheck );
                    if( (gDlgButtons[ 1 ] = WinCreateButton( gDlgSurface, 584, 162, 14, 14, -1, -1, -1, 116, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) != -1 ){
                        WinSetClickSound( gDlgButtons[ 1 ], GSoundPlayCheck, GSoundPlayUnCheck );
                        if( ObjCreate( &gDlgUnk15, -1, -1 ) != -1 ){
                            gDlgUnk15->Flags |= 0x01;
                            if( ObjCreate( &gDlgUnk20, -1, -1 ) != -1 ){
                                gDlgUnk20->Flags |= 0x01;
                                if( ObjCreate( &gDlgUnk21, gDlgUnk44->ImgId, -1 ) != -1 ){
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
                WinClose( gDlgSurface );
                gDlgSurface = -1;
            }
    }
    ArtClose( ImgObj );
    return -1;
}

void GdialogUnk61()
{
    int i;
    char *Surface,*Bitmap;
    CachePool_t *ImgObj;

    if( gDlgSurface == -1 ) return;
    ObjDestroy(gDlgUnk21, 0);
    ObjDestroy(gDlgUnk20, 0);
    ObjDestroy(gDlgUnk15, 0);
    for ( i = 0; i != 9;  i++ ){
        WinImageRemove( gDlgButtons[ i ] );
        gDlgButtons[ i ] = -1;
    }
    Surface = WinGetSurface( gDlgBackWindow );    
    if( (Bitmap = ArtGetBitmap( ( gDlgUnk45 ) ? ArtMakeId( 6, 420, 0, 0, 0 ) : ArtMakeId( 6, 111, 0, 0, gDlgUnk45 ), 0, 0, &ImgObj )) ){
        GdialogUnk53( gDlgSurface, 0, Bitmap, WinGetSurface( gDlgSurface ), &Surface[ (gVidMainGeo.rt - gVidMainGeo.lt + 1) * (480 - gDlgUnk53) ], gDlgUnk53, 0 );
        ArtClose( ImgObj );
    }
    WinClose( gDlgSurface );
    gDlgSurface = -1;
    AiUnk45( gDlgUnk44, 0 );
}

int GdialogUnk62()
{
    int i, n;

    for( i = 0; i < gDlgUnk15->Critter.Box.Cnt; i++ ){
        Item14( gDlgUnk15, gObjDude, gDlgUnk15->Critter.Box.Box->obj, Item33( gDlgUnk15, gDlgUnk15->Critter.Box.Box->obj ) );
    }
    n = gDlgUnk20->Critter.Box.Cnt;
    for( i = 0; i < gDlgUnk20->Critter.Box.Cnt; i++ ){
        n = Item14( gDlgUnk20, gDlgUnk44, gDlgUnk20->Critter.Box.Box->obj, Item33( gDlgUnk20, gDlgUnk20->Critter.Box.Box->obj ) );
    }
    n = gDlgUnk21->Critter.Box.Cnt;
    if( gDlgUnk21 ){
        for( i = 0; i < gDlgUnk21->Critter.Box.Cnt; i++ ){
            n = Item14( gDlgUnk21, gDlgUnk44, gDlgUnk21->Critter.Box.Box->obj, Item33( gDlgUnk21, gDlgUnk21->Critter.Box.Box->obj ) );
        }
    }
    return n;
}

int GdialogUnk63()
{
    ArtFrmHdr_t *Img,*v18,*v23,*v26;
    CachePool_t *ImgObj;
    char *ObjData,*Surface,*v9,*v27,*a8,*a9;    
    int ObjHeight,Window,v8,Disposition,v31,zz,a3,i;
    
    if( !(Img = ArtLoadImg( ArtMakeId(6, 390, 0, 0, 0), &ImgObj )) ) return -1;
    ObjData = ArtGetObjData( Img, 0, 0 );
    if( !ObjData ) goto err;
    ObjHeight = ArtGetObjHeight( Img, 0, 0);
    gDlgUnk53 = ObjHeight;
    Window = WinCreateWindow(0, 480 - ObjHeight, gVidMainGeo.rt - gVidMainGeo.lt + 1, ObjHeight, 256, 2);
    gDlgSurface = Window;
    if( Window == -1 ) goto err;
    Surface = WinGetSurface( Window );
    v31 = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    v8 = v31 * (480 - ObjHeight);
    v9 = WinGetSurface( gDlgBackWindow );
    ScrCopy( &v9[v8], v31, ObjHeight, v31, Surface, v31 );
    GdialogUnk53( gDlgSurface, 1, ObjData, Surface, 0, ObjHeight, 0 );
    ArtClose( ImgObj );            
    if( ( gDlgButtons[ 0 ] = WinCreateButton( gDlgSurface, 593, 41, 14, 14, -1, -1, -1, 27, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) == -1 ) goto err;
    WinSetClickSound( gDlgButtons[ 0 ], GSoundPlayCheck, GSoundPlayUnCheck );                
    if( ( gDlgButtons[ 1 ] = WinCreateButton( gDlgSurface, 593, 97, 14, 14, -1, -1, -1, 100, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) == -1 ) goto err;
    WinSetClickSound( gDlgButtons[ 1 ], GSoundPlayCheck, GSoundPlayUnCheck);                    
    if( ( gDlgButtons[ 2 ] = WinCreateButton( gDlgSurface, 236, 15, 14, 14, -1, -1, -1, 119, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) == -1 ) goto err;
    WinSetClickSound( gDlgButtons[ 2 ], GSoundPlayCheck, GSoundPlayUnCheck );
    if( ( gDlgButtons[ 3 ] = WinCreateButton( gDlgSurface, 235, 46, 14, 14, -1, -1, -1, 97, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) == -1 ) goto err;
    WinSetClickSound( gDlgButtons[ 3 ], GSoundPlayCheck, GSoundPlayUnCheck );
    gDlgUnk113 = 4;
    for( i = 0; i < 5; i++  ){
        if( !(v18 = ArtLoadImg( ArtMakeId( 6, gDlgUnk54[ i ].ArtId1, 0, 0, 0 ), &gDlgUnk54[ i ].Art1 ) ) ) goto err;
        zz = ArtGetObjWidth( v18, 0, 0 );
        a3 = ArtGetObjHeight( v18, 0, 0 );
        a8 = ArtGetObjData( v18, 0, 0 );
        if( !(v23 = ArtLoadImg( ArtMakeId( 6, gDlgUnk54[ i ].ArtId2, 0, 0, 0 ), &gDlgUnk54[ i ].Art2 ) ) ) goto err;
        a9 = ArtGetObjData( v23, 0, 0 );                                
        if( !(v26 = ArtLoadImg( ArtMakeId( 6, gDlgUnk54[ i ].ArtId3, 0, 0, 0 ), &gDlgUnk54[ i ].Art3 ) ) ) goto err;
        v27 = ArtGetObjData( v26, 0, 0 );
	gDlgButtons[ i + 4 ] = WinCreateButton( gDlgSurface, gDlgUnk54[ i ].Xpos, gDlgUnk54[ i ].Ypos, zz, a3, -1, -1, gDlgUnk54[ i ].HotKey, -1, a8, a9, 0, 37 );
        if( ( gDlgButtons[ i + 4 ]  ) == -1 ) break;
        WinSetAnimH( gDlgButtons[ i + 4 ], v27, v27, v27 );
        WinSetClickSound( gDlgButtons[ i + 4 ], GSoundPlayCheck, GSoundPlayUnCheck );
        if( !PartyInDisposition( gDlgUnk44, gDlgUnk54[ i ].Disposition ) ) WinDisableWidget( gDlgButtons[ i + 4 ] );
    }                                                        
    WinRadioButton( 5, &gDlgButtons[ gDlgUnk113 ] );
    Disposition = AiGetDisposition( gDlgUnk44 );
    WinButtonSet( gDlgButtons[ gDlgUnk113 + 4 - Disposition ], 1, 0 );
    GdialogUnk65();
    gDlgUnk09 = 10;
    WinUpdate( gDlgSurface );
    return 0;
err:
    GdialogUnk64();
    return -1;
}

void GdialogUnk64()
{
    CachePool_t *ImgObj;
    char *surf, *img;
    int i;

    if( gDlgSurface == -1 ) return;    
    for( i = 0; i != 9; i++ ){
        WinImageRemove( gDlgButtons[ i ] );
        gDlgButtons[ i ] = -1;
    }
    for( i = 0; i != 5; i++ ){
        if( gDlgUnk54[ i ].Art1 ){
            ArtClose( gDlgUnk54[ i ].Art1 );
            gDlgUnk54[ i ].Art1 = NULL;
        }
        if( gDlgUnk54[ i ].Art2 ){
            ArtClose( gDlgUnk54[ i ].Art2 );
            gDlgUnk54[ i ].Art2 = NULL;
        }
        if( gDlgUnk54[ i ].Art3 ){
            ArtClose( gDlgUnk54[ i ].Art3 );
            gDlgUnk54[ i ].Art3 = NULL;
        }
    }
    surf = WinGetSurface( gDlgBackWindow );    
    if( (img = ArtGetBitmap( ArtMakeId( 6, 390, 0, 0, 0 ), 0, 0, &ImgObj )) ){
        GdialogUnk53( gDlgSurface, 0, img, WinGetSurface( gDlgSurface ), &surf[ (gVidMainGeo.rt - gVidMainGeo.lt + 1) * (480 - gDlgUnk53) ], gDlgUnk53, 0 );
        ArtClose( ImgObj );
    }
    WinClose( gDlgSurface );
    gDlgSurface = -1;    
}

void GdialogUnk65()
{
    int Width,ObjWidth,v20,v22,v29,FontId;
    char *ObjData,stmp[256],*surf;
    ArtFrmHdr_t *img, *v18;
    Obj_t *RHandObj, *ArmorObj;
    MsgLine_t Line;
    CachePool_t *Obj;

    FontId = FontGetCurrent();
    FontSet( 101 );
    surf = WinGetSurface( gDlgSurface );
    Width = WinGetWidth( gDlgSurface );
    if( (img = ArtLoadImg( ArtMakeId( 6, 390, 0, 0, 0 ), &Obj )) ){
        ObjWidth = ArtGetObjWidth( img, 0, 0 );
        ObjData = ArtGetObjData( img, 0, 0 );
        ScrCopy( &ObjData[ 20 * ObjWidth + 112 ], 110, gFont.ChrHeight(), ObjWidth, &surf[ 20 * Width + 112 ], Width );
        ScrCopy( &ObjData[ 49 * ObjWidth + 112 ], 110, gFont.ChrHeight(), ObjWidth, &surf[ 49 * Width + 112 ], Width );
        ScrCopy( &ObjData[ 84 * ObjWidth + 8 ], 70, 98, ObjWidth, &surf[ 84 * Width + 8 ], Width );
        ScrCopy( &ObjData[ 80 * ObjWidth + 232 ], 132, 106, ObjWidth, &surf[ 80 * Width + 232 ], Width );
        ArtClose( Obj );
    }
    RHandObj = InvGetRHandObj(gDlgUnk44);
    sprintf( stmp, "%s", ( RHandObj ) ? ItemGetName( RHandObj ) : MessageGetMessage( &gProtoMessages, &Line, 10 ) );
    gFont.Print( &surf[ 20 * Width + 112 ], stmp, 110, Width, DLG_COLOR_5 );
    ArmorObj = InvGetArmorObj( gDlgUnk44 );
    sprintf( stmp, "%s", ( ArmorObj ) ? ItemGetName( ArmorObj ) : MessageGetMessage( &gProtoMessages, &Line, 10 ) );
    gFont.Print( &surf[ 49 * Width + 112 ], stmp, 110, Width, DLG_COLOR_5 );
    if( (v18 = ArtLoadImg( ArtMakeId( OBJTYPE( gDlgUnk44->ImgId ), gDlgUnk44->ImgId & 0xFFF, 0, (gDlgUnk44->ImgId & 0xF000) >> 12, 3 ), &Obj )) ){
        v20 = ArtGetObjWidth( v18, 0, 3 );
        v22 = ArtGetObjHeight( v18, 0, 3 );
        ScrCopyAlpha( ArtGetObjData( v18, 0, 3 ), v20, v22, v20, &surf[ Width * (132 - v22 / 2) + 39 - v20 / 2], Width );
        ArtClose( Obj );
    }
    sprintf( stmp, "%d/%d", FeatGetVal(gDlgUnk44, 35), FeatGetVal( gDlgUnk44, 7 ) );
    gFont.Print( &surf[ 96 * Width + 240 ], stmp, 115, Width, DLG_COLOR_5 );
    sprintf( stmp, "%s", SkillGetName( PartyBestSkill( gDlgUnk44 ) ) );
    gFont.Print( &surf[ 113 * Width + 240 ], stmp, 115, Width, DLG_COLOR_5 );
    sprintf( stmp, "%d/%d ", ItemGetBackPackWeight( gDlgUnk44 ), FeatGetVal( gDlgUnk44, 12 ) );
    if( CritterOverload( gDlgUnk44 ) ) v29 = DLG_COLOR_9;
    gFont.Print( &surf[ 131 * Width + 240 ], stmp, 115, Width, v29 );
    sprintf( stmp, "%d", FeatGetVal(gDlgUnk44, 11) );
    gFont.Print( &surf[ 148 * Width + 240 ], stmp, 115, Width, DLG_COLOR_5 );
    sprintf( stmp, "%d/%d ", ( IN_COMBAT ) ? gDlgUnk44->Critter.State.CurrentAP : FeatGetVal( gDlgUnk44, 8 ), FeatGetVal( gDlgUnk44, 8 ) );
    gFont.Print( &surf[ 167 * Width + 240 ], stmp, 115, Width, DLG_COLOR_5 );
    FontSet( FontId );
    WinUpdate( gDlgSurface );
}

void GdialogUnk66()
{
    gDlgUnk08 = 8;
    gDlgUnk09 = 10;
    if( gDlgReplyWin != -1 ) WinUpdateDirty( gDlgReplyWin );
    if( gDlgUnk28 != -1 ) WinUpdateDirty( gDlgUnk28 );
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

int GdialogUnk68()
{
    MsgLine_t msg;
    Proto_t *proto;

    if( OBJTYPE( gDlgUnk44->Pid ) != TYPE_CRIT ) return 1;
    ProtoGetObj( gDlgUnk44->Pid, &proto );
    if( (proto->Critt.Type & 2) != 0 ) return 1;
    msg.Id = 903;
    if( gDlgUnk45 ) msg.Id = 913;
    if( MessageGetMsg( &gProtoMessages, &msg ) == 1 )
        GdialogReply( msg.Text );
    else
        eprintf( "\nError: gdialog: Can't find message!" );
    return 0;
}

void GdialogUnk69()
{
    int ext;
    Obj_t *obj;
    MsgLine_t msg;

    ext = 0;
    while( !ext && !gMenuEscape){
    	switch( InpUpdate() ){
    	    case -1: break;
    	    case 17: case 24: case 324: SysQuitDlg(); break;
    	    case 27: gDlgUnk08 = 1; gDlgUnk09 = 1; return;
    	    case 'a':
    		if( gDlgUnk44->Pid != 0x10000A1 ){                
        	    if( (obj = AiFindBestArmor( gDlgUnk44 ) ) ) InvUnk28( gDlgUnk44, obj, 0 );
    		}
    		GdialogReply( MessageGetMessage( &gProtoMessages, &msg, GdialogUnk67( gDlgUnk44 ) ) );
    		GdialogUnk65();
    		break;
	    case 'd':
		if( !GdialogUnk68() ) break;
		gDlgUnk08 = 2; 
		gDlgUnk09 = 4; 
		return;
    	    case -2:
    		if( MseCursorCenterInArea( 441, 451, 540, 470 ) ){ AiUnk06( gDlgUnk44, 0 ); gDlgUnk09 = 13; gDlgUnk08 = 11; ext = 1; }
    		break;
    	    case 'w':
        	InvUnwield( gDlgUnk44, 1 );
        	if( (obj = AiUnk29( gDlgUnk44, 0, 0 )) ){ InvUnk28( gDlgUnk44, obj, 1 ); AiUnk45( gDlgUnk44, 0 ); }
    		GdialogReply( MessageGetMessage( &gProtoMessages, &msg, GdialogUnk67( gDlgUnk44 ) ) );
    		GdialogUnk65();
    		break;
    	    case 2098: AiUnk06( gDlgUnk44, 4 ); break;
    	    case 2099: AiUnk06( gDlgUnk44, 0 ); gDlgUnk09 = 13; gDlgUnk08 = 11; ext = 1; break;
    	    case 2102: AiUnk06( gDlgUnk44, 2 ); break;
    	    case 2103: AiUnk06( gDlgUnk44, 3 ); break;
    	    case 2111: AiUnk06( gDlgUnk44, 1 ); break;        
        }
    }
}

int GdialogUnk70()
{
    ArtFrmHdr_t *Img, *Img1, *Img2;
    CachePool_t *ImgObj;
    char *ObjData, *Surface, *p;
    int ObjHeight, i, w;

    if( MessageInit( &gDlgMsg ) != 1 ) return -1;
    if( MessageLoad( &gDlgMsg, "game/custom.msg" ) != 1 ) return -1;    
    if( !(Img = ArtLoadImg( ArtMakeId( 6, 391, 0, 0, 0 ), &ImgObj )) ) return -1;
    ObjData = ArtGetObjData( Img, 0, 0 );
    if( ObjData ){
        ObjHeight = ArtGetObjHeight( Img, 0, 0 );
        gDlgUnk53 = ObjHeight;
        w = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        if( (gDlgSurface = WinCreateWindow( 0, 480 - ObjHeight, w, ObjHeight, 256, 2 ) ) != -1 ){
            Surface = WinGetSurface( gDlgSurface );
            p = WinGetSurface( gDlgBackWindow );
            ScrCopy( &p[ w * (480 - ObjHeight)], w, ObjHeight, w, Surface, w );
            GdialogUnk53( gDlgSurface, 1, ObjData, Surface, 0, ObjHeight, 0 );
            ArtClose( ImgObj );
            if( (gDlgButtons[ 0 ] = WinCreateButton( gDlgSurface, 593, 101, 14, 14, -1, -1, -1, 13, gDlgImgBtA0, gDlgImgBtA1, 0, 32 )) != -1 ){
                WinSetClickSound( gDlgButtons[ 0 ], GSoundPlayCheck, GSoundPlayUnCheck );
                gDlgUnk112 = 1;                
                for( i = 0; i < 6; i++ ){
                    if( !(Img1 = ArtLoadImg( ArtMakeId( 6, gDlgUnk71[ i ].ArtId1, 0, 0, 0 ), &gDlgUnk71[ i ].Art1 ) ) ) break;
                    if( !(Img2 = ArtLoadImg( ArtMakeId( 6, gDlgUnk71[ i ].ArtId2, 0, 0, 0 ), &gDlgUnk71[ i ].Art2 ) ) ) break;
                    gDlgButtons[ i + 1 ] = WinCreateButton( gDlgSurface, gDlgUnk71[ i ].Xpos, gDlgUnk71[ i ].Ypos, ArtGetObjWidth( Img1, 0, 0 ), ArtGetObjHeight( Img1, 0, 0 ), -1, -1, -1, gDlgUnk71[ i ].HotKey, ArtGetObjData( Img1, 0, 0 ), ArtGetObjData( Img2, 0, 0 ), 0, 32 );
                    if( gDlgButtons[ i + 1 ] == -1 ) break;
                    WinSetClickSound( gDlgButtons[ i + 1 ], GSoundPlayCheck, GSoundPlayUnCheck );
                }
                gDlgUnk105[ 0 ] = AiGetAreaAttackMode( gDlgUnk44 );
                gDlgUnk105[ 1 ] = AiGetHealthGrade( gDlgUnk44 );
                gDlgUnk105[ 2 ] = AiGetBestWeapon( gDlgUnk44 );
                gDlgUnk105[ 3 ] = AiGetDistance( gDlgUnk44 );
                gDlgUnk105[ 4 ] = AiGetAttackWho( gDlgUnk44 );
                gDlgUnk105[ 5 ] = AiGetChemUse( gDlgUnk44 );
                gDlgUnk09 = 13;
                GdialogUnk73();
                return 0;
            }
        }
    }
    GdialogUnk71();
    return -1;
}

void GdialogUnk71()
{
    int i, tmp;
    CachePool_t *ImgObj;
    char *surf, *Img;

    if( gDlgSurface == -1 ) return;
    for( i = 0; i != 9; i++ ){
        WinImageRemove( gDlgButtons[ i ] );
        gDlgButtons[ i ] = -1;
    }
    for( i = 0; i != 6; i++ ){
        if( gDlgUnk71[ i ].Art1 ){ ArtClose( gDlgUnk71[ i ].Art1 ); gDlgUnk71[ i ].Art1 = NULL; }
        if( gDlgUnk71[ i ].Art2 ){ ArtClose( gDlgUnk71[ i ].Art2 ); gDlgUnk71[ i ].Art2 = NULL; }
        if( gDlgUnk71[ i ].Art3 ){ ArtClose( gDlgUnk71[ i ].Art3 ); gDlgUnk71[ i ].Art3 = NULL; }
    }
    tmp = (gVidMainGeo.rt - gVidMainGeo.lt + 1) * (480 - gDlgUnk53);
    surf = WinGetSurface( gDlgBackWindow );
    if( ( Img = ArtGetBitmap( ArtMakeId( 6, 391, 0, 0, 0 ), 0, 0, &ImgObj ) ) ){
        GdialogUnk53( gDlgSurface, tmp, Img, WinGetSurface( gDlgSurface ), &surf[ tmp ], gDlgUnk53, 0 );
        ArtClose( ImgObj );
    }
    WinClose( gDlgSurface );
    gDlgSurface = -1;
    MessageClose( &gDlgMsg );
}

void GdialogUnk72()
{
    int sel;

    while( 1 ){
        sel = InpUpdate();
        if( sel == -1 ) continue;
	if( sel == 17 || sel == 24 || sel == 324 ) SysQuitDlg();
	if( gMenuEscape ) break;
    	if( sel < 13 ){
    	    if( sel <= 5 ){
            	GdialogUnk75( sel );
            	GdialogUnk73();
    	    }
    	} else if( sel <= KEY_ENTER || sel == KEY_ESC ){
    	    gDlgUnk08 = 8;
    	    gDlgUnk09 = 10;
    	    break;
    	}
    }
}

void GdialogUnk73()
{
    char *surf;
    int w, ow, oh, FontId;
    ArtFrmHdr_t *img;
    MsgLine_t fmt; 
    CachePool_t *ImgObj;

    FontId = FontGetCurrent();
    FontSet( 101 );
    surf = WinGetSurface( gDlgSurface );
    w = WinGetWidth( gDlgSurface );
    if( !( img = ArtLoadImg( ArtMakeId( 6, 391, 0, 0, 0 ), &ImgObj ) ) ) return;
    oh = ArtGetObjHeight( img, 0, 0 );
    ow = ArtGetObjWidth( img, 0, 0 );
    ScrCopy( ArtGetObjData( img, 0, 0 ), ow, oh, ow, surf, gVidMainGeo.rt - gVidMainGeo.lt + 1 );
    ArtClose( ImgObj );
    if( gDlgUnk105[ 0 ] == -1 ){
        gFont.Print( &surf[ 20 * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, 99 ), 248, w, DLG_COLOR_5 );
    } else {
        eprintf( "\nburst: %d", gDlgUnk105[ 0 ] );
        gFont.Print( &surf[ 20 * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[0][ gDlgUnk105[0] ][0] ), 248, w, DLG_COLOR_5 );
    }        
    gFont.Print( &surf[ 48  * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[1][ gDlgUnk105[1] ][0]), 248, w, DLG_COLOR_5 );
    gFont.Print( &surf[ 78  * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[2][ gDlgUnk105[2] ][0]), 248, w, DLG_COLOR_5 );
    gFont.Print( &surf[ 108 * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[3][ gDlgUnk105[3] ][0]), 248, w, DLG_COLOR_5 );
    gFont.Print( &surf[ 137 * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[4][ gDlgUnk105[4] ][0]), 248, w, DLG_COLOR_5 );
    gFont.Print( &surf[ 166 * w + 232 ], MessageGetMessage( &gDlgMsg, &fmt, gDlgTextIds[5][ gDlgUnk105[5] ][0]), 248, w, DLG_COLOR_5 );
    WinUpdate( gDlgSurface );
    FontSet( FontId );
}

void GdialogUnk74( char *surf, int a2, int a3, int a4 )
{
    int i;
    int state = 0;
    int color;
    int FieldLen;
    MsgLine_t msg;

    FontSet( 101 );    
    FieldLen = a2 - 84;
    for( i = 0; i < 6; i++ ){
        if( gDlgTextIds[ a3 ][ i ][ 0 ] == -1 ) continue;
        if ( a3 <= 5 ){
            switch( a3 ){
                case 0: state = PartyInAttackMode( gDlgUnk44, gDlgTextIds[ a3 ][ i ][ 1 ] ); break;
                case 1: state = PartyInRunAwayMode( gDlgUnk44, gDlgTextIds[ a3 ][ i ][ 1 ] ); break;
                case 2: state = PartyInBestWeapon( gDlgUnk44, gDlgTextIds[ a3 ][ i ][ 1 ] ); break;
                case 3: state = PartyInDistance( gDlgUnk44, gDlgTextIds[ a3 ][ i ][ 1 ] ); break;
                case 4: state = PartyInAttackWho( gDlgUnk44, gDlgTextIds[ a3 ][ i ][ 1 ] ); break;
                case 5: state = PartyInChemUse( gDlgUnk44, gDlgTextIds[ a3 ][ i ][ 1 ] ); break;
            }
        }
        if( state ){
            color = ( i == a4 ) ? DLG_COLOR_10 : DLG_COLOR_5;
        } else {
            color = DLG_COLOR_7;
        }
//        a3 = FieldLen;//?
        gFont.Print( (surf + a2 * (i * gFont.ChrHeight() + 42) + 42), MessageGetMessage( &gDlgMsg, &msg, gDlgTextIds[ a3 ][ i ][ 0 ] ), FieldLen, a2, color );
    }    
}

int GdialogUnk75( int a1 )
{
    MsgLine_t msg;
    CachePool_t *Obj;
    ArtFrmHdr_t *Data;
    int state;
    char *ObjData, *surf;
    unsigned int Lines, PrevTime;
    int ObjWidth,sel,SysTime,bottom,right,FontId,CentY,CentX,Height,win,x0,y1,x1,y0,ExitFlg,v56;

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
    gFont.Print( ( surf + 15  * ObjWidth + 40  ), MessageGetMessage( &gDlgMsg, &msg, a1 ), ObjWidth, ObjWidth, DLG_COLOR_3 );
    gFont.Print( ( surf + 163 * ObjWidth + 88  ), MessageGetMessage( &gDlgMsg, &msg, 10 ), ObjWidth, ObjWidth, DLG_COLOR_3 );
    gFont.Print( ( surf + 162 * ObjWidth + 193 ), MessageGetMessage( &gDlgMsg, &msg, 11 ), ObjWidth, ObjWidth, DLG_COLOR_3 );
    v56 = gDlgUnk105[ a1 ];
    GdialogUnk74( surf, ObjWidth, a1, v56 );
    WinUpdate( win );
    y0 = CentY + 42;
    y1 = CentY + Height - 42;
    x0 = CentX + 42;
    x1 = CentX + ObjWidth - 42;
    do{
        sel = InpUpdate();
        if( sel == -1 ) continue;
        if( sel == 17 || sel == 24 || sel == 324 ) SysQuitDlg();
        if( gMenuEscape ) break;
        if( sel >= 13 ){
            if( sel == KEY_ENTER ){
                ExitFlg = 1;
                gDlgUnk105[ x1 ] = v56;
                GdialogUnk76( a1, gDlgTextIds[ a1 ][ v56 ][ 1 ] );
            } else if( sel == KEY_ESC ){
                ExitFlg = 1;
            }
            continue;
        }
        if( sel != -2 ) continue;
        if( (MseGetButtons() & 0x10) == 0 ) continue;
        if( !MseCursorCenterInArea( x0, y0, x1, y1 ) ) continue;
        MseGetCursorPosition( &right, &bottom );
        Lines = y0 / gFont.ChrHeight();
        if( Lines >= 6 ) continue;
        SysTime = TimerGetSysTime();
        if( Lines == v56 ){
            if( TimerDiff( SysTime, PrevTime ) < 250 ){
                gDlgUnk105[ x1 ] = Lines;
                ExitFlg = 1;
                GdialogUnk76( a1, gDlgTextIds[ a1 ][ Lines ][ 1 ] );
                continue;
            }
        } else {
            if( gDlgTextIds[ a1 ][ Lines ][ 0 ] != -1 ){
                if( a1 <= 5 ){
                    switch( a1 ){
                        case 0: state = PartyInAttackMode( gDlgUnk44, gDlgTextIds[ a1 ][ Lines ][ 1 ] ); break;
                        case 1: state = PartyInRunAwayMode( gDlgUnk44, gDlgTextIds[ a1 ][ Lines ][ 1 ] ); break;
                        case 2: state = PartyInBestWeapon( gDlgUnk44, gDlgTextIds[ a1 ][ Lines ][ 1 ] ); break;
                        case 3: state = PartyInDistance( gDlgUnk44, gDlgTextIds[ a1 ][ Lines ][ 1 ] ); break;
                        case 4: state = PartyInAttackWho( gDlgUnk44, gDlgTextIds[ a1 ][ Lines ][ 1 ] ); break;
                        case 5: state = PartyInChemUse( gDlgUnk44, gDlgTextIds[ a1 ][ Lines ][ 1 ] ); break;
                        default: state = 0;
                    }
                }
                if( state ){
                    v56 = Lines;
                    GdialogUnk74( surf, ObjWidth, a1, Lines );
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

int GdialogUnk76( int a1, int a2 )
{
    switch( a1 ){
        case 0:
            if( a2 >= 5 ) return -1;
            AiGetPacketByObj( gDlgUnk44 )->AreaAttackMode = a2;
            return 0;
        case 1: return AiSetRunAway( gDlgUnk44, a2 );
        case 2: 
            if( a2 >= 8 ) return -1;
            AiGetPacketByObj( gDlgUnk44 )->BestWeapon = a2;
            return 0;
        case 3:
            if( a2 >= 5 ) return -1;
            AiGetPacketByObj( gDlgUnk44 )->Distance = a2;
            return 0;
        case 4:
            if( a2 >= 5 ) return -1;
            AiGetPacketByObj( gDlgUnk44 )->AttackWho = a2;
            return 0;
        case 5:
            if( a2 >= 6 ) return -1;
            AiGetPacketByObj( gDlgUnk44 )->ChemUse = a2;
            return 0;
    }
    return -1;
}

void GdialogBarter()
{
    MsgLine_t msg;
    Proto_t *proto;
    Scpt_t *scr;

    if( OBJTYPE( gDlgUnk44->Pid ) != 1 ) return;
    if( ScptPtr( gDlgUnk44->ScrId, &scr ) == -1 ) return;    
    if( OBJTYPE( gDlgUnk44->Pid ) == 1 ){ // ??
	ProtoGetObj( gDlgUnk44->Pid, &proto );
	if( !( proto->Critt.Type & 0x02 ) ){
    	    msg.Id = 903; // 'This person will not barter with you'
    	    if( gDlgUnk45 ) msg.Id = 913; // 'This critter can't carry anything.'
    	    if( MessageGetMsg( &gProtoMessages, &msg ) == 1 )
        	GdialogReply( msg.Text );
    	    else
        	eprintf( "\nError: gdialog: Can't find message!" );
    	    return;
	}    
    }
    if( gDlgLipsEn == 1 && SoundIsPlaying( gLipsync.Snd ) ) GdialogLipsyncEnd();
    gDlgUnk08 = 2;
    gDlgUnk09 = 4;
    if( gDlgReplyWin != -1 ) WinUpdateDirty( gDlgReplyWin );
    if( gDlgUnk28 != -1 ) WinUpdateDirty( gDlgUnk28 );
}

int GdialogUnk78()
{
    CachePool_t *ImgObj;
    ArtFrmHdr_t *Img;
    char *ObjData, *Surface, *v8, *Bitmap, *v14;
    int v0, ObjHeight, i, w, h;

    v0 = 0;
    if( gDlgUnk88 ) return -1;    
    for( i = 0; i < 9; i++ ) gDlgButtons[ i ] = -1;
    if( !(Img = ArtLoadImg( gDlgUnk45 ? ArtMakeId( 6, 389, 0, 0, 0 ) : ArtMakeId( 6, 99, gDlgUnk45, 0, 0 ), &ImgObj )) ) return -1;    
    ObjData = ArtGetObjData( Img, 0, 0 );
    if( ObjData ){
        ObjHeight = ArtGetObjHeight( Img, 0, 0 );
	gDlgUnk53 = ObjHeight;
	h = 480 - ObjHeight;
        gDlgSurface = WinCreateWindow( 0, h, gVidMainGeo.rt - gVidMainGeo.lt + 1, ObjHeight, 256, 2 );
        if( gDlgSurface != -1 ){
            Surface = WinGetSurface( gDlgSurface );
            w = gVidMainGeo.rt - gVidMainGeo.lt + 1;
            v8 = WinGetSurface( gDlgBackWindow );
            ScrCopy( &v8[ w * h ], w, ObjHeight, w, Surface, w );
            if( gDlgUnk10 ){
                WinUpdate( gDlgBackWindow );
                GdialogUnk53( gDlgSurface, 1, ObjData, Surface, NULL, ObjHeight, -1 );
                gDlgUnk10 = 0;
            } else {
                GdialogUnk53( gDlgSurface, 1, ObjData, Surface, NULL, ObjHeight, -1 );
            }
            ArtClose( ImgObj );
            if( (gDlgButtons[ 0 ] = WinCreateButton( gDlgSurface, 593, 41, 14, 14, -1, -1, -1, -1, gDlgImgBtA0, gDlgImgBtA1, 0, 32 )) != -1 ){
                    WinSetButtonHandler( gDlgButtons[ 0 ], 0, 0, 0, GdialogBarter );
                    WinSetClickSound( gDlgButtons[ 0 ], GSoundPlayCheck, GSoundPlayUnCheck );
                    if( (Bitmap = ArtGetBitmap( ArtMakeId( 6, 97, 0, 0, 0 ), 0, 0, &gDlgUnk121 ) ) ){
                        if( (v14 = ArtGetBitmap( ArtMakeId( 6, 98, 0, 0, 0 ), 0, 0, &gDlgUnk123 ) ) ){
                            if( (gDlgButtons[ 1 ] = WinCreateButton( gDlgSurface, 13, 154, 51, 29, -1, -1, -1, -1, Bitmap, v14, 0, 0 )) != -1 ){
                                WinSetButtonHandler( gDlgButtons[ 1 ], 0, 0, 0, GdialogReview );
                                WinSetClickSound( gDlgButtons[ 1 ], GSoundPlayPushBt, GSoundPlayReleaseBt );
                                if( !gDlgUnk45 ){ gDlgUnk88 = 1; return 0; }
                                if( (gDlgButtons[ 2 ] = WinCreateButton( gDlgSurface, 593, 116, 14, 14, -1, -1, -1, -1, gDlgImgBtA0, gDlgImgBtA1, 0, 32 ) ) != -1 ){
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
                            v0 = -1; // ???
                            ArtClose( gDlgUnk123 );
                        }
                        ArtClose( gDlgUnk121 );
                    }
                    WinImageRemove( gDlgButtons[ v0 ] );
                    gDlgButtons[ v0 ] = -1;
                }
                WinClose( gDlgSurface );
                gDlgSurface = -1;
        }
    }
    ArtClose( ImgObj );        
    return -1;
}

void GdialogUnk79()
{
    int i, Id;
    CachePool_t *ImgObj;
    char *surf, *Bitmap;

    if( gDlgSurface == -1 ) return;
    for( i = 0; i != 9; i++ ){
        WinImageRemove( gDlgButtons[ i ] );
        gDlgButtons[ i ] = -1;
    }
    ArtClose( gDlgUnk123 );
    ArtClose( gDlgUnk121 );
    surf = WinGetSurface( gDlgBackWindow );
    if( gDlgUnk45 )
        Id = ArtMakeId( 6, 389, 0, 0, 0 );
    else
        Id = ArtMakeId( 6, 99, 0, 0, gDlgUnk45 );    
    if( !( Bitmap = ArtGetBitmap( Id, 0, 0, &ImgObj ) ) ) return;
    GdialogUnk53( gDlgSurface, 0, Bitmap, WinGetSurface( gDlgSurface ), &surf[ (gVidMainGeo.rt - gVidMainGeo.lt + 1) * (480 - gDlgUnk53) ], gDlgUnk53, 0 );
    ArtClose( ImgObj );
    WinClose( gDlgSurface );
    gDlgUnk88 = 0;
    gDlgSurface = -1;
}

int GdialogCreateWindow()
{
    gDlgBackWindow = WinCreateWindow( 0, 0, gVidMainGeo.rt - gVidMainGeo.lt + 1, 480, 256, 2 );
    return ( gDlgBackWindow != -1 ) - 1;
}

int GdialogRenderB()
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

    if( gDlgUnk45 )
        Id = ArtMakeId( 6, 389, 0, 0, 0 );
    else
        Id = ArtMakeId( 6, 99, 0, 0, gDlgUnk45 );
    if( !( img = ArtGetBitmap( Id, 0, 0, &ImgObj ) ) ) return -1;
    pos = 640 * Area->tp + Area->lt;
    surf = WinGetSurface( gDlgSurface );
    ScrCopy( &img[ pos ], Area->rt - Area->lt, Area->bm - Area->tp, gVidMainGeo.rt - gVidMainGeo.lt + 1, &surf[ pos ], gVidMainGeo.rt - gVidMainGeo.lt + 1 );
    ArtClose( ImgObj );
    WinAreaUpdate( gDlgSurface, Area );
    return 0;
}

void GdialogBlit( char *pSrc, int SrcPitch, int SrcHeight, int DstWidth, char *pDst, int PosX, int PosY, int DstPitch, char *Shader )
{
    char *dst, c;
    int i, SrcDiff, DstDiff;

    dst = &pDst[ PosX + DstPitch * PosY ];
    DstDiff = DstPitch - SrcPitch;
    SrcDiff = DstWidth - SrcPitch;
    for( ; --SrcHeight != -1; pSrc += SrcDiff, dst += DstDiff ){
        for( i = 0; i < SrcPitch; i++, pSrc++, dst++ ){
            c = *pSrc;
            if( c ) c = (256 - *pSrc) >> 4;
            *dst = Shader[ 256 * c + *dst ];
        }        
    }
}

void GdialogRenderHead( ArtFrmHdr_t *Img, int FrameNo )
{
    VidRect_t Rect;
    CachePool_t *ImgObj;
    ArtFrmHdr_t *img;
    char *p, *surf, *pSrc;
    int w,pitch,tmp,i,ShiftY,ShiftX,CentY,CentX, h;

    if( gDlgSurface == -1 ) return;    
    if( Img ){
        if( !FrameNo ) gDlgUnk79 = 0;
        if( !(img = ArtLoadImg( ArtMakeId( 9, gDlgUnk83, 0, 0, 0 ), &ImgObj ) ) ) eprintf( "\tError locking background in display..." );        
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
    } else {
        if( gDlgUnk26 == 1 ){
            gDlgUnk26 = 0;
            TileUpdate();
        }
        pSrc = WinGetSurface( gMapIsoWin );
	pitch = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        ScrCopy( &pSrc[ pitch * ((gVidMainGeo.bm - gVidMainGeo.tp - 331) / 2) + (pitch - 388) / 2], 388, 200, pitch, gDlgSurf, pitch );
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
        ScrCopyAlpha( gDlgUnk97[ i ], tmp, gDlgUnk22[ i ].bm - gDlgUnk22[ i ].tp, tmp, &surf[ gDlgUnk22[ i ].tp * pitch + gDlgUnk22[ i ].lt ], pitch );
    }        
    WinAreaUpdate( gDlgBackWindow, &Rect );
}

void GdialogUnk85()
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


