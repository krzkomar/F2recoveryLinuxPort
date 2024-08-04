#include "FrameWork.h"

int gOptionUnk01; // poza plikiem
int gCurrentFont;
int gPrefUnk05 = 0;

/*....*/
static Geo_t 	gOptMenuImgSize[ 3 ];
static Msg_t 	gOptPrefMsgBase;
static Geo_t 	gOptPrefImgSize[ 10 ];
static MsgLine_t gOptPrefMsgLine;
static char  	*gOptPrefPix[ 10 ];
static char 	*gOptMenuImgs[ 10 ];
static CachePool_t *gOptPrefImgs[ 10 ];
static int  	gOptTextBaseDelay;
static double 	gOptSetBrightness;
static double 	gOptBrightness;
static double 	gOptSetTextDelay;
static double 	gOptSetMouseSens;
static int 	gOptMouseSensitivity;
static char 	*gOptPrefSurface;
static int 	gOptionUnk15;
static int 	gOptionMenuWindow;
static int 	gPrefWindow;
static char 	*gOptionMenuSurface;
static CachePool_t *gOptionMenuWallpImg[ 3 ];
static char 	*gOptionWallPaperBmp[ 3 ];
static int 	gOptGameDifficulty;
static int 	gOptCombatDifficulty;
static int 	gOptViolenceLvl;
static int 	gOptTargetH1;
static int 	gOptCombatLook;
static int 	gOptCombatMsg;
static int 	gOptCombatTaunts;
static int 	gOptLangFilter;
static int 	gOptRunning;
static int 	gOptSubtitles;
static int 	gOptItemH1;
static int 	gOptCombatSpeed;
static int 	gOptPlayerSpeedUp;
static int 	gOptMasterVolume;
static int	gOptMusicVolume;
static int 	gOptSndFxVolume;
static int	gOptSpeechVolume;
static int 	gOptSetFxVolume;
static int 	gOptSetSubtitles;
static int 	gOptSetLangFilter;
static int 	gOptSetSpeechVolume;
static int 	gOptSetMasterVolume;
static int 	gOptSetPlayerSpeed;
static int 	gOptSetCombatTaunts;
static int 	gOptSetFont;
static int 	gOptSetMusicVolume;
static int 	gOptSetUnk20;
static int 	gOptSetRunning;
static int 	gOptSetCombatSpeed;
static int 	gOptSetUnk18;
static int 	gOptSetItemH1;
static int 	gOptPrefChanged;
static int 	gOptSetCombatMsg;
static int 	gOptSetTargetH1;
static int 	gOptSetCombatDifficulty;
static int 	gOptSetViolenceLvl;
static int 	gOptSetGameDifficulty;
static int 	gOptSetCombatLook;
static int 	gOptSetCombatLook;

const short 	gPrefTitlePosA[ 5 ] = { 0x30, 0x7D, 0xCB, 0x11E, 0x16B };
const short 	gPrefTitlePosB[ 6 ] = { 0x31, 0x74, 0xB5, 0xF7, 0x139, 0x17C };
const short 	gPrefTitlePosC[ 8 ] = { 0x13, 0x5E, 0xA5, 0xD8, 0x10C, 0x13F, 0x171, 0x1A4 };
const short 	gOptPrefLabXofsA[ 4 ] = { 2, 25, 46, 46 };
const short 	gOptPrefLabXofsB[ 3 ] = { 31, 4, 21 };
const int   	gOptPauseImgIds[ 4 ] = { 208, 209, 8, 9 };
const int   	gOptKnobsAposY[ 5 ]  = { 66, 143, 222, 304, 382 };
const short 	gOptKnobPosXY[ 4 ]   = { 10, -4, 10, 31 };
const int   	gOptKnobsBposY[ 6 ]  = { 66, 133, 200, 264, 331, 397 };

int 		gOptionMenuImgIds[ 3 ] = { 220, 222, 221 };
int 		gOptPrefWinPixIds[ 10 ] = { 0xF0, 0x0F1, 0x0F2, 0x0F3, 0x0F4, 0xF5, 0xF6, 0xF7, 0x08, 0x09 };

OptPref_t gOptPrefKnobs[ 19 ] = {
    { 3, 0, 76,  71,  0, 0, { 203, 204, 205, 0   }, 0, 0, "game_difficulty",    0.0, 0.0,     .iVal = &gOptSetGameDifficulty },
    { 3, 0, 76,  149, 0, 0, { 206, 204, 208, 0   }, 0, 0, "combat_difficulty",  0.0, 0.0,     .iVal = &gOptSetCombatDifficulty },
    { 4, 0, 76,  226, 0, 0, { 214, 215, 204, 216 }, 0, 0, "violence_level",     0.0, 0.0,     .iVal = &gOptSetViolenceLvl },
    { 3, 0, 76,  309, 0, 0, { 202, 201, 213, 0   }, 0, 0, "target_highlight",   0.0, 0.0,     .iVal = &gOptSetTargetH1 },
    { 2, 0, 76,  387, 0, 0, { 202, 201, 0,   0   }, 0, 0, "combat_looks",       0.0, 0.0,     .iVal = &gOptSetCombatLook },
    { 2, 0, 299, 74,  0, 0, { 211, 212, 0,   0   }, 0, 0, "combat_messages",    0.0, 0.0,     .iVal = &gOptSetCombatMsg },
    { 2, 0, 299, 141, 0, 0, { 202, 201, 0,   0   }, 0, 0, "combat_taunts",      0.0, 0.0,     .iVal = &gOptSetCombatTaunts },
    { 2, 0, 299, 207, 0, 0, { 202, 201, 0,   0   }, 0, 0, "language_filter",    0.0, 0.0,     .iVal = &gOptSetLangFilter },
    { 2, 0, 299, 271, 0, 0, { 209, 219, 0,   0   }, 0, 0, "running",            0.0, 0.0,     .iVal = &gOptSetRunning },
    { 2, 0, 299, 338, 0, 0, { 202, 201, 0,   0   }, 0, 0, "subtitles",          0.0, 0.0,     .iVal = &gOptSetSubtitles },
    { 2, 0, 299, 404, 0, 0, { 202, 201, 0,   0   }, 0, 0, "item_highlight",     0.0, 0.0,     .iVal = &gOptSetItemH1 },
    { 2, 0, 374, 50,  0, 0, { 207, 210, 0,   0   }, 0, 0, "combat_speed",       0.0, 50.0,    .iVal = &gOptSetCombatSpeed },
    { 3, 0, 374, 125, 0, 0, { 217, 209, 218, 0   }, 0, 0, "text_base_delay",    1.0, 6.0,     .iVal = NULL },
    { 4, 0, 374, 196, 0, 0, { 202, 221, 209, 222 }, 0, 0, "master_volume",      0.0, 32767.0, .iVal = &gOptSetMasterVolume },
    { 4, 0, 374, 247, 0, 0, { 202, 221, 209, 222 }, 0, 0, "music_volume",       0.0, 32767.0, .iVal = &gOptSetMusicVolume },
    { 4, 0, 374, 298, 0, 0, { 202, 221, 209, 222 }, 0, 0, "sndfx_volume",       0.0, 32767.0, .iVal = &gOptSetFxVolume },
    { 4, 0, 374, 349, 0, 0, { 202, 221, 209, 222 }, 0, 0, "speech_volume",      0.0, 32767.0, .iVal = &gOptSetSpeechVolume },
    { 2, 0, 374, 400, 0, 0, { 207, 223, 0,   0   }, 0, 0, "brightness",         1.0, 1.18,    .iVal = NULL },
    { 2, 0, 374, 451, 0, 0, { 207, 218, 0,   0   }, 0, 0, "mouse_sensitivity",  1.0, 2.5,     .iVal = NULL }
};
/***/


int OptPrefUpdate( int WgNo );
void OptPauseGrayScreen( int a1 );

/*********************************************/

int OptMenuInvoke()
{
    return OptMenu( -1 );
}

int OptMenu( int KeyCode )
{
    int exit_code, key, PreferFlg;

    exit_code = -1;
    if( OptMenuCreate() == -1 ){
	ErrorPrintf("\nOPTION MENU: Error loading option dialog data!\n");
	return -1;
    }
    while( exit_code == -1 ){    
	key = InpUpdate();
	PreferFlg = 0;
	if( KeyCode != -1 ){
    	    exit_code = 1;
    	    key = KeyCode;
	}
	if( key == '\x1B' || key == 504 || gMenuEscape ){
    	    exit_code = 0;
	} else {
	    if( key == '\r' || key == 'O' || key == 'o' || key == 'D' || key == 'd' ){
		GSoundPlay( "ib1p1xx1" );
		exit_code = 0;
	    } else {
		switch( key ){
        	    case 500: case 'S': case 's':
            		if( LsgSaveGameMenu( 1 ) == 1 ) exit_code = 1;                
            		break;
        	    case 501: case 'L': case 'l':
            		if( LsgMenuGameLoad( 1 ) == 1 ) exit_code = 1;                
            	        break;
        	    case '=': case '+':
            		OptBrightInc();
            	        break;
        	    case '-': case '_':
            		OptBrightDec();
            	        break;
        	    case 502:
            		PreferFlg = 1;
            		break;
        	    case 'P': case 'p':
            		GSoundPlay( "ib1p1xx1" );
            	        PreferFlg = 1;
            		break;
		}
	    }
	}
	if( PreferFlg ){
	    OptPrefDialog();
	} else {
	    if( key == 390 ){
    		InpScrShotTake();
	    } else if( key == 17 || key == 24 || key == 324 || key == 503 || key == 'E' || key == 'e' ){
    		SysQuitDlg();
	    }
	}
    }
    OptMenuDestroy();        
    return exit_code;
}

int OptMenuCreate()
{
    CachePool_t **pImgs;
    char *p, *q, stmp[ 128 ];
    int toggle,j,k,m,hh,wcent,wdg,hcent, ypos;

    gCurrentFont = FontGetCurrent();
    gOptionUnk01 = 0;

    if( MessageInit( &gOptPrefMsgBase ) != 1 ) return -1;
    sprintf( stmp, "%s%s", gGamePath, "options.msg" );
    if( MessageLoad( &gOptPrefMsgBase, stmp ) != 1 ) return -1;
    pImgs = gOptionMenuWallpImg;
    for( j = 0; j < 3; j++ ){
        p = ArtLoadBmp( 
    	    ArtMakeId( 6, gOptionMenuImgIds[ j ], 0, 0, 0 ), 
    	    &pImgs[ j ], &gOptMenuImgSize[ j ].Width, &gOptMenuImgSize[ j ].Height 
    	);
        gOptionWallPaperBmp[ j ] = p;
        if( !p ){
            while( --j >= 0 ) ArtClose( gOptionMenuWallpImg[ j ] );
    	    MessageClose( &gOptPrefMsgBase );
            return -1;                
        }
    }

    for( j = toggle = 0; j < 10; j++ ){
        p = (char *)Malloc( gOptMenuImgSize[ 1 ].Height * gOptMenuImgSize[ 1 ].Width + 1024 );
        gOptMenuImgs[ j ] = p;
        if( !p ){
            while( --j >= 0 ) Free( gOptMenuImgs[ j ] );
            for( j = 0; j != 3; j++ ) ArtClose( gOptionMenuWallpImg[ j ] );
            MessageClose( &gOptPrefMsgBase );
            return -1;
        }
        q = ( toggle ^= 0x01 ) ? gOptionWallPaperBmp[ 2 ] : gOptionWallPaperBmp[ 1 ];
        memcpy( p, q, gOptMenuImgSize[ 1 ].Height * gOptMenuImgSize[ 1 ].Width );
    } while( j < 10 );

    gOptionMenuWindow = WinCreateWindow(
	(640 - gOptMenuImgSize[0].Width) / 2, ((480 - gOptMenuImgSize[0].Height) / 2 ) - 60, 
	gOptMenuImgSize[ 0 ].Width, gOptMenuImgSize[ 0 ].Height, 256, 18 
    );
    if( gOptionMenuWindow == -1 ){
        for( k = 0; k != 10; k++ ) Free( gOptMenuImgs[k] );
        for( m = 0; m != 3; m++ ) ArtClose( gOptionMenuWallpImg[ m ] );
        MessageClose( &gOptPrefMsgBase );
        return -1;
    }
    gOptSetUnk20 = MapAmbientDisable();
    gOptionUnk15 = GmouseUnk58();
    if( gOptionUnk15 ) GmouseUnk03();
    GmouseLoadCursor( 1 );
    gOptionMenuSurface = WinGetSurface( gOptionMenuWindow );
    memcpy( gOptionMenuSurface, gOptionWallPaperBmp[ 0 ], gOptMenuImgSize[ 0 ].Width * gOptMenuImgSize[ 0 ].Height );
    FontSet( 103 );

    hh = gFont.ChrHeight();
    hcent = ( ( gOptMenuImgSize[ 1 ].Height - hh) / 2 ) + 1;
    ypos = OPTION_MENU_START_Y;
    for( j = 0; j < 5; j++ ){
        strcpy( stmp, MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, j ) );
        wcent = (gOptMenuImgSize[ 1 ].Width - gFont.LineWidth( stmp ) ) >> 1;
        if( wcent < 0 ) wcent = 0;
        gFont.Print( 
    	    gOptMenuImgs[ 2*j + 0 ] + wcent + gOptMenuImgSize[ 1 ].Width * hcent, stmp,
    	    gOptMenuImgSize[ 1 ].Width, gOptMenuImgSize[ 1 ].Width, OPTION_MENU_COLOR_A
    	);
        gFont.Print( 
    	    gOptMenuImgs[ 2*j + 1 ] + gOptMenuImgSize[ 1 ].Width * hcent + wcent, stmp, 
    	    gOptMenuImgSize[ 1 ].Width, gOptMenuImgSize[ 1 ].Width, OPTION_MENU_COLOR_B
    	);
        wdg = WinCreateButton( gOptionMenuWindow, 13, ypos, 
    	    gOptMenuImgSize[ 1 ].Width, gOptMenuImgSize[ 1 ].Height, -1, -1, -1, j + 500, gOptMenuImgs[2*j + 0], gOptMenuImgs[2*j + 1], 0, 32 
    	);
        if( wdg != -1 ) WinSetClickSound( wdg, (void *)GSoundPlayKnobLt, GSoundPlayKnobRt );
        ypos += gOptMenuImgSize[ 1 ].Height + 3;
    }
    FontSet( 101 );
    WinUpdate( gOptionMenuWindow );
    return 0;
}

void OptMenuDestroy()
{
    int i;

    WinClose( gOptionMenuWindow );
    FontSet( gOptSetFont );
    MessageClose( &gOptPrefMsgBase );
    for( i = 0; i < 10; i++ ) Free( gOptMenuImgs[ i ] );
    for( i = 0; i != 3; i++ ) ArtClose( gOptionMenuWallpImg[ i ] );
    if( gOptionUnk15 ) GmouseIsoEnter();
    if( gOptSetUnk20 ) MapAmbientEnable();
}

int OptPauseWindow( int Flg )
{
    char *p, *surf, *text, *ImgData[4];
    unsigned int key;
    char stmp[256];
    Geo_t ImgSize[4];
    CachePool_t *Img[4];
    MsgLine_t v22;
    int i,WinPosY,WinPosX,win,w,Button,ImgIds[4],flg2;

    memcpy( ImgIds, gOptPauseImgIds, 4 * sizeof(int) );
    if( !Flg ){
        gOptSetUnk20 = MapAmbientDisable();
        CycleColorStop();
        if( (flg2 = GmouseUnk58()) ) GmouseUnk03();
    }
    GmouseLoadCursor( 1 );
    OptPauseGrayScreen( Flg );
    for( i = 0; i < 4; i++ ){
        if( !( ImgData[ i ] = ArtLoadBmp( ArtMakeId( 6, ImgIds[ i ], 0, 0, 0 ), &Img[ i ], &ImgSize[ i ].Width, &ImgSize[ i ].Height ) ) ) break;
    }

    if( i < 4 ){
        while( i > -1 ) ArtClose( Img[ --i ] );
        ErrorPrintf("\n** Error loading pause window graphics! **\n");
        return -1;
    }

    if( MessageInit( &gOptPrefMsgBase ) != 1 ) return -1;    
    sprintf( stmp, "%s%s", gGamePath, "options.msg" );
    if( MessageLoad(&gOptPrefMsgBase, stmp) != 1 ) return -1;    
    WinPosY = ( 480 - ImgSize[ 0 ].Height ) >> 1;
    WinPosX = ( 640 - ImgSize[ 0 ].Width ) >> 1;
    if( Flg ){
        WinPosY -= 24;
        WinPosX -= 65;
    } else {
        WinPosY -= 54;
    }

    if( ( win = WinCreateWindow( WinPosX, WinPosY, ImgSize[0].Width, ImgSize[0].Height, 256, 18 ) ) == -1 ){
        for( i = 0; i != 4; i++ ) ArtClose( Img[ i ] );        
        MessageClose(&gOptPrefMsgBase);
        eprintf("** Error opening pause window! **");
        return -1;
    } 
    p = ImgData[ 0 ];
    surf = WinGetSurface( win );
    memcpy( surf, p, ImgSize[ 0 ].Width * ImgSize[ 0 ].Height );
    ScrCopyAlpha( ImgData[1], ImgSize[1].Width, ImgSize[1].Height, ImgSize[1].Width, WIN_XY( 13, 42, ImgSize[ 0 ].Width, surf ), ImgSize[ 0 ].Width );
    gOptSetFont = FontGetCurrent();
    FontSet( 103 );
    text = MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, 300 );
    gFont.Print( WIN_XY( 52, 45, ImgSize[ 0 ].Width, surf ), text, ImgSize[ 0 ].Width, ImgSize[ 0 ].Width, PREF_COLOR_I );
    FontSet( 104 );
    p = stmp;
    text = MessageGetMessage(&gOptPrefMsgBase, &v22, 301);
    strcpy( p, text );
    w = gFont.LineWidth( stmp );
    gFont.Print( WIN_XY( 2 + ((ImgSize[0].Width - w) / 2), 10, ImgSize[0].Width, surf ), stmp, ImgSize[0].Width, ImgSize[0].Width, PREF_COLOR_I );
    Button = WinCreateButton(win, 26, 46, ImgSize[2].Width, ImgSize[2].Height, -1, -1, -1, 504, ImgData[2], ImgData[3], 0, 32);
    if( Button != -1 ) WinSetClickSound( Button, GSoundPlayPushBt, GSoundPlayReleaseBt );
    WinUpdate( win );

    do{
        key = InpUpdate();
        if( key == '=' || key == '+' ) OptBrightInc();
        if( key == '-' || key == '_' ) OptBrightDec();
    } while( key >= -1 && !gMenuEscape );
    if( !Flg ) TileUpdate();
    WinClose( win );
    for( i = 0; i != 4; i++ ) ArtClose( Img[ i ] );    
    MessageClose( &gOptPrefMsgBase );
    if( !Flg ){
        if( flg2 ) GmouseIsoEnter();
        if( gOptSetUnk20 ) MapAmbientEnable();
        CycleColorStart();
        GmouseLoadCursor( 1 );
    }
    FontSet( gOptSetFont );
    return 0;    
}

void OptPauseGrayScreen( int a1 )
{
    if( a1 ){
        MseCursorHide();
    } else {
        MseCursorHide();
        TileUpdate();
        GrGrayMapApply( (unsigned char *)WinGetSurface( gMapIsoWin ), 640, WinGetHeight( gMapIsoWin ), 640 );
        WinUpdate( gMapIsoWin );
    }
    MseCursorShow();
}

void OptPrefDialog()
{
    int sel;
    int Exit;

    sel = OptPrefDialogCreate();

    if( sel == -1 ){
        eprintf( "PREFERENCE MENU: Error loading preference dialog data!" );
        return;
    }
    for( Exit = -1; Exit == -1; ){
            sel = InpUpdate();
            switch( sel ){
                case 504:
                    Exit = 1;
                    break;
                case '\r': case 'P': case 'p':
                    GSoundPlay("ib1p1xx1");
                    Exit = 1;
                    break;
                case 527:
                    OptSetDefault(1);
                    break;
                case 390:
                    InpScrShotTake();
                    break;
                case '=': case '+':
                    OptBrightInc();
                    break;
                case '-': case '_':
                    OptBrightDec();
                    break;
                case 17: case 24: case 324:
                    SysQuitDlg();
                    break;
                default:
                    if( sel == 528 || sel == 27 || gMenuEscape ){
                        OptPrefSave();
                        Exit = 0;
                    } else {
                        if( sel >= 505 && sel <= 524 ) OptPrefHandler( sel );
                    }
                    break;
            }
    }
    OptPrefDialogClose( sel );    
}

int OptPrefDialogCreate()
{
    char *PixData, *Text;
    int j, i, Xpos, Ypos, Id, width, height, g0, g1, g2, g3;

    OptPrefLoad();
    for( i = 0; i < 10; i++ ){
        PixData = ArtLoadBmp( ArtMakeId( 6, gOptPrefWinPixIds[ i ], 0, 0, 0 ), &gOptPrefImgs[ i ], &gOptPrefImgSize[ i ].Width, &gOptPrefImgSize[ i ].Height );
        gOptPrefPix[ i ] = PixData;
        if( !PixData ){ for( j = i; j > -1; j-- ) ArtClose( gOptPrefImgs[ j ] ); return -1; }
    }
    gPrefUnk05 = 0;
    gPrefWindow = WinCreateWindow( 0, 0, 640, 480, 256, 0x12 );

    if( gPrefWindow == -1 ){ for( i = 0; i != 10; i++ ) ArtClose( gOptPrefImgs[ i ] ); return -1; }
    gOptPrefSurface = WinGetSurface( gPrefWindow );
    memcpy( gOptPrefSurface, gOptPrefPix[ 0 ], gOptPrefImgSize[ 0 ].Width * gOptPrefImgSize[ 0 ].Height );

    // page title 'GAME OPTIONS'
    FontSet( PREF_FONT_A );
    Text = MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, 100 );
    gFont.Print( WIN_XY( 74, 10, 640, gOptPrefSurface ), Text, 640, 640, PREF_COLOR_A );

    // 101 .. 105
    FontSet( PREF_FONT_B );
    for( i = 0; i < 5; i++ ){
        Text = MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, 101 + i );
        gFont.Print( WIN_XY( 99 - gFont.LineWidth( Text ) / 2, gPrefTitlePosA[ i ], 640, gOptPrefSurface ), Text, 640, 640, PREF_COLOR_B );
    }
    
    // 106 .. 111
    for( i = 0; i < 6; i++ ){
        Text = MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, 106 + i );
        gFont.Print( WIN_XY( 206, gPrefTitlePosB[ i ], 640, gOptPrefSurface ), Text, 640, 640, PREF_COLOR_C );
    }

    // 112 .. 119
    for( i = 0; i < 8; i++ ){
        Text = MessageGetMessage(&gOptPrefMsgBase, &gOptPrefMsgLine, 112 + i );
        gFont.Print( WIN_XY( 384, gPrefTitlePosC[ i ], 640, gOptPrefSurface ), Text, 640, 640, PREF_COLOR_D );
    }

    // 'DEFAULT'
    Text = MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, 120 );
    gFont.Print( WIN_XY( 43, 449, 640, gOptPrefSurface ), Text, 640, 640, PREF_COLOR_E );

    // 'DONE'
    Text = MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, 4 );
    gFont.Print( WIN_XY( 169, 449, 640, gOptPrefSurface ), Text, 640, 640, PREF_COLOR_F );

    // 'CANCEL'
    Text = MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, 121 );
    gFont.Print( WIN_XY( 283, 449, 640, gOptPrefSurface ), Text, 640, 640, PREF_COLOR_G );
    
    // 'Affect player speed'
    FontSet( PREF_FONT_C );
    Text = MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, 122 );
    gFont.Print( WIN_XY( 405, 72, 640, gOptPrefSurface ), Text, 640, 640, PREF_COLOR_H );

    // knobs & sliders
    for( i = 0; i < 19; i++ ) OptPrefUpdate( i );

    for( i = 0; i < 19; i++ ){
        g0 = g1 = g2 = g3 = -1;
        if( i < 5 ){ // 0..4
            g3 = 505 + i;
            Xpos  = gOptPrefKnobs[ i ].x0;
            Ypos  = gOptPrefKnobs[ i ].y - 4;
            width = gOptPrefKnobs[ i ].x1 - gOptPrefKnobs[ i ].x0;
            height = 48;
        } else { 
            if( i < 11 ){ // 5..10
                g3 = 505 + i;
                Xpos  = gOptPrefKnobs[ i ].x0;
                Ypos  = gOptPrefKnobs[ i ].y - 5;
                width = gOptPrefKnobs[ i ].x1 - gOptPrefKnobs[ i ].x0;
                height = 28;
            } else { // 11 .. 18
                g0 = 526;
                g1 = 526;
                g2 = 505 + i;
                g3 = 526;
                width = 240;
                Xpos = 384;
                Ypos = gOptPrefKnobs[ i ].y - 12;
                height = 23;
            }
        }
        gOptPrefKnobs[ i ].WgId = WinCreateButton( gPrefWindow, Xpos, Ypos, width, height, g0, g1, g2, g3, 0, 0, 0, 0x20 );
    }
    
    gOptSetUnk18 = WinCreateButton( gPrefWindow, 383, 68, gOptPrefImgSize[4].Width, gOptPrefImgSize[5].Height, -1, -1, 524, 524, gOptPrefPix[ 5 ], gOptPrefPix[ 4 ], 0, 0x23 );
    if( gOptSetUnk18 != -1 ) WinButtonSet( gOptSetUnk18, gOptSetPlayerSpeed, 0 );
    WinSetClickSound( gOptSetUnk18, GSoundPlayCheck, GSoundPlayCheck );

    Id = WinCreateButton( gPrefWindow, 23, 450, gOptPrefImgSize[9].Width, gOptPrefImgSize[9].Height, -1, -1, -1, 527, gOptPrefPix[ 8 ], gOptPrefPix[ 9 ], 0, 0x20 );
    if( Id != -1 ) WinSetClickSound( Id, GSoundPlayPushBt, GSoundPlayReleaseBt );

    Id = WinCreateButton( gPrefWindow, 148, 450, gOptPrefImgSize[9].Width, gOptPrefImgSize[9].Height, -1, -1, -1, 504, gOptPrefPix[ 8 ], gOptPrefPix[ 9 ], 0, 0x20 );
    if( Id != -1 ) WinSetClickSound( Id, GSoundPlayPushBt, GSoundPlayReleaseBt );

    Id = WinCreateButton( gPrefWindow, 263, 450, gOptPrefImgSize[9].Width, gOptPrefImgSize[9].Height, -1, -1, -1, 528, gOptPrefPix[ 8 ], gOptPrefPix[ 9 ], 0, 0x20 );
    if( Id != -1 ) WinSetClickSound( Id, GSoundPlayPushBt, GSoundPlayReleaseBt );

    FontSet( PREF_FONT_D );
    WinUpdate( gPrefWindow );
    return 0;
}

void  OptPrefHandler( int WgCode )
{
    char *Message;
    double SlScale,NewVal;
    int *pVal;

    int x,y,i,color,n,SlPos,time_ms,mse_y,mse_x,WgNo,dx;
    int flg,Spk,Sfx,Step;
    
    MseGetCursorPosition( &mse_x, &mse_y );
    WgNo = WgCode - 505;
    flg = 0;
    if( WgNo < 5 ){
        x = gOptPrefKnobs[ WgNo ].x;
        y = gOptPrefKnobs[ WgNo ].y;
        if( sqrt((mse_x - (x + 23)) * (mse_x - (x + 23)) + (mse_y - (y + 21)) * (mse_y - (y + 21))) > 16.0 ){
            if( y < mse_y ){
                y = gOptKnobPosXY[0] + y;
                if( (y <= mse_y) && (y + gFont.ChrHeight() >= mse_y) ){
                    if( (gOptPrefKnobs[ WgNo ].x0 > mse_x) || ( gOptPrefKnobs[ WgNo ].x < mse_x ) ){
                        if( (gOptPrefKnobs[ WgNo ].Positions >= 3) && ((gOptPrefKnobs[ WgNo ].x + 46) <= mse_x) && ( gOptPrefKnobs[ WgNo ].x1 >= mse_x ) ){
                            pVal = gOptPrefKnobs[ WgNo ].iVal;
                            n = *pVal;
                            *pVal = 2;
                            flg = 1;
                            gOptPrefKnobs[ WgNo ].Dir = ( n > 2 );
                        }
                    } else {
                        *gOptPrefKnobs[ WgNo ].iVal = 0;
                        gOptPrefKnobs[ WgNo ].Dir = 0;
                        flg = 1;
                    }
                }
            } else if( (x + 9 <= mse_x) && (x + 37 >= mse_x) ){
                n = *gOptPrefKnobs[ WgNo ].iVal;
                *gOptPrefKnobs[ WgNo ].iVal = 1;
                gOptPrefKnobs[ WgNo ].Dir = ( n ) ? 1 : 0;
                flg = 1;
            }
            if( gOptPrefKnobs[ WgNo ].Positions == 4 ){
                n = gOptPrefKnobs[ WgNo ].y + gOptPrefLabXofsB[ 0 ];
                if( (n <= mse_y) && (n + 2 * gFont.ChrHeight() >= mse_y) && ((gOptPrefKnobs[ WgNo ].x + 46) <= mse_x) && gOptPrefKnobs[ WgNo ].x1 >= mse_x ){
                    *gOptPrefKnobs[ WgNo ].iVal = 3;
                    flg = 1;
                    gOptPrefKnobs[ WgNo ].Dir = 1;
                }
            }
        } else {
    	    if( gOptPrefKnobs[ WgNo ].Dir ){
        	if( *gOptPrefKnobs[ WgNo ].iVal ){
    		    (*gOptPrefKnobs[ WgNo ].iVal)--;
        	} else {
        	    gOptPrefKnobs[ WgNo ].Dir = 0;
    		    ( *gOptPrefKnobs[ WgNo ].iVal )++;
    		}
    	    } else {
        	if( ( gOptPrefKnobs[ WgNo ].Positions - 1 ) == *gOptPrefKnobs[ WgNo ].iVal ){
            	    gOptPrefKnobs[ WgNo ].Dir = 1;
    		    (*gOptPrefKnobs[ WgNo ].iVal)--;
        	} else {
    		    ( *gOptPrefKnobs[ WgNo ].iVal )++;
    		}
    	    }
    	    flg = 1;
        }
        if( flg ){
            GSoundPlay( "ib3p1xx1" );
            TimerWait( 0x46 );
            GSoundPlay( "ib3lu1x1" );
            OptPrefUpdate( WgNo );
            WinUpdate( gPrefWindow );
            gOptPrefChanged = 1;
            return;
        }
	gOptPrefChanged = 1;
	return;
    }

    if( WgNo < 11 ){
        y = gOptPrefKnobs[ WgNo ].y;
        x = gOptPrefKnobs[ WgNo ].x + 11;
        if( sqrt( (mse_x - x) * (mse_x - x) + (mse_y - (y + 12)) * (mse_y - (y + 12)) ) <= 10.0 ){
            flg = 1;
            *gOptPrefKnobs[ WgNo ].iVal = ( *gOptPrefKnobs[ WgNo ].iVal ^ 0x01) & 0x01;
        } else {
    	    y = y - 5;
    	    if( (y <= mse_y) && (y + gFont.ChrHeight() + 2) >= mse_y ){
        	if( ( gOptPrefKnobs[ WgNo ].x0 <= mse_x ) && ( gOptPrefKnobs[ WgNo ].x >= mse_x ) ){
            	    *gOptPrefKnobs[ WgNo ].iVal = (WgNo == 5);
            	    flg = 1;
        	}
        	if( ( (gOptPrefKnobs[ WgNo ].x + 22) <= mse_x) && ( gOptPrefKnobs[ WgNo ].x1 >= mse_x ) ){
            	    *gOptPrefKnobs[ WgNo ].iVal = ( WgNo == 5 ) ? 0 : 1;
            	    flg = 1;
        	}
	    }
        }
        if( flg ){
            GSoundPlay("ib2p1xx1");
            TimerWait( 0x46 );
            GSoundPlay("ib2lu1x1");
            OptPrefUpdate( WgNo );
            WinUpdate( gPrefWindow );
            gOptPrefChanged = 1;
            return;
        }
	gOptPrefChanged = 1;
	return;
    }
    if( WgNo < 19 ){
        GSoundPlay( "ib1p1xx1" );
        Sfx = Spk = Step = 0;
        if( WgNo == 12 ){
            NewVal = 6.0 - gOptSetTextDelay + 1.0;
        } else if( WgNo > 16 ) {
            if( WgNo == 17 ){
                NewVal = gOptSetBrightness;
            } else {
                NewVal = gOptSetMouseSens;
            }
        } else {
            NewVal = *gOptPrefKnobs[ WgNo ].dVal;
        }
        n = 640 * gOptPrefKnobs[ WgNo ].y + 384;
        SlScale = 219.0 / ( gOptPrefKnobs[ WgNo ].dMax - gOptPrefKnobs[ WgNo ].dMin );
        SlPos = lround( (NewVal - gOptPrefKnobs[ WgNo ].dMin) * SlScale + 384.0 );
        ScrCopy( gOptPrefPix[ 0 ] + n, 240, 12, 640, &gOptPrefSurface[ n ], 640 ); // clear
        ScrCopyAlpha( gOptPrefPix[7], 21, 12, 21, &gOptPrefSurface[ 640 * gOptPrefKnobs[ WgNo ].y + SlPos ], 640 );
        WinUpdate( gPrefWindow );

        while( 1 ){
            InpUpdate();
            time_ms = TimerGetSysTime();
            MseGetCursorPosition( &mse_x, &mse_y );
            if( MseGetButtons() & 0x10 ) break;            
            if( (SlPos + 14) < mse_x ){
                SlPos = mse_x - 6;
                if( SlPos > 603 ) SlPos = 603;
            } else {
                if( (SlPos + 6) > mse_x ){
                    SlPos = mse_x - 6;
                    if( SlPos < 384 ) SlPos = 384;
                }
            }
            NewVal = (SlPos - 384) / SlScale + gOptPrefKnobs[ WgNo ].dMin;
            if( NewVal < gOptPrefKnobs[ WgNo ].dMin ) NewVal = gOptPrefKnobs[ WgNo ].dMin;
            if( NewVal > gOptPrefKnobs[ WgNo ].dMax ) NewVal = gOptPrefKnobs[ WgNo ].dMax;
            switch( WgNo - 11 ){
                case 0:
                    *gOptPrefKnobs[ WgNo ].iVal = lround( NewVal );
                    break;
                case 1:
                    gOptSetTextDelay = 6.0 - NewVal + 1.0;
                    break;
                case 2:
                    *gOptPrefKnobs[ WgNo ].iVal = lround( NewVal );
                    GSoundSetMasterVolume( gOptSetMasterVolume );
                    Step = 1;
                    break;
                case 3:
                    *gOptPrefKnobs[ WgNo ].iVal = lround( NewVal );
                    Step = 1;
                    GSoundSetMusicVolume( gOptSetMusicVolume );
                    break;
                case 4:
                    *gOptPrefKnobs[ WgNo ].iVal = lround( NewVal );
                    GSoundSetSfxVolume( gOptSetFxVolume );
                    Step = 1;
                    if( Sfx ){
                        Sfx -= 1;
                    } else {
                        GSoundPlay( "butin1" );
                        Sfx = 7;
                    }
                    break;
                case 5:
                    *gOptPrefKnobs[ WgNo ].iVal = lround( NewVal );
                    Step = 15;
                    GSoundSetSpkVolume(gOptSetSpeechVolume);
                    if( Spk ){
                        Spk -= 15;
                    } else {
//                        OptPrefUnk05( "narrator/options", 12, 0xF, 0xD );
                        Spk = 40;
                    }
                    break;
                case 6:
                    gOptSetBrightness = NewVal;
                    PalSetBrightness( NewVal );
                    break;
                case 7:
                    gOptSetMouseSens = NewVal;
                    break;
                default:
                    break;
            }

            if( Step ){
                n = 640 * (gOptPrefKnobs[ WgNo ].y - 12) + 384;
                ScrCopy( gOptPrefPix[ 0 ] + n, 240, 24, 640, gOptPrefSurface + n, 640 );
                for( i = 0; i < 4; i++ ){
                    Message = MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, gOptPrefKnobs[ WgNo ].MsgId[ i ] );
                    color = PREF_COLOR_I;
                    switch( i ){
                        case 0:
                            dx = 384;
                            if( NewVal == gOptPrefKnobs[ WgNo ].dMin ) color = PREF_COLOR_J;
                            break;
                        case 1:
                            dx = lround( ( gFont.LineWidth( Message ) / 2 ) + 444.0 + -8.0 );
                            break;
                        case 2:
                            dx = lround( 564.0 - gFont.LineWidth( Message ) - 4.0 );
                            break;
                        case 3:
                            dx = 624 - gFont.LineWidth( Message );
                            break;
                    }
                    gFont.Print( WIN_XY( dx, gOptPrefKnobs[ WgNo ].y - 12, 640, gOptPrefSurface ), Message, 640, 640, color );
                } while ( i < 4 );
            } else {
                n = 640 * gOptPrefKnobs[ WgNo ].y + 384;
                ScrCopy( gOptPrefPix[ 0 ] + n, 240, 12, 640, &gOptPrefSurface[ n ], 640 );
            }
            ScrCopyAlpha( gOptPrefPix[7], 21, 12, 21, &gOptPrefSurface[640 * gOptPrefKnobs[ WgNo ].y + SlPos ], 640 );
            WinUpdate( gPrefWindow );
            while( TimerCurrDiff( time_ms ) < 35 );
        }
        GSoundPlay( "ib1lu1x1" );
        OptPrefUpdate( WgNo );
        WinUpdate( gPrefWindow );
        gOptPrefChanged = 1;
        return;        
    }
    if( WgNo != 19 ) return;
    gOptSetPlayerSpeed = (gOptSetPlayerSpeed ^ 0x01) & 0x01;
    gOptPrefChanged = 1;
    return;
}

int OptPrefUpdate( int WgNo )
{
    double k,u,dblVal;
    unsigned int i;
    int Bpos[6], Apos[7],x, n;
    char *p, stmp[128];

    FontSet( PREF_FONT_E );
    if( WgNo < 5 ) {
	memcpy( Apos, gOptKnobsAposY, 5 * sizeof( int ) );
	n = WIN_XY( 23, Apos[ WgNo ], 640 , 0);
	ScrCopy( gOptPrefPix[ 0 ] + n, 160, 54, 640, gOptPrefSurface + n, 640 ); // clear

	for( i = 0; i < gOptPrefKnobs[ WgNo ].Positions; i++ ){
    	    strcpy( stmp, MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, gOptPrefKnobs[ WgNo ].MsgId[ i ] ) );
    	    switch( i ){
    		case 0:     		    
    		    x = gOptPrefKnobs[ WgNo ].x + gOptPrefLabXofsA[ i ] - gFont.LineWidth( stmp );
    		    gOptPrefKnobs[ WgNo ].x0 = x;
    		    break;
    		case 1:
            	    x = gOptPrefLabXofsA[ i ] + gOptPrefKnobs[ WgNo ].x - gFont.LineWidth( stmp ) / 2;
        	    gOptPrefKnobs[ WgNo ].x1 = x + gFont.LineWidth( stmp );
		    break;
		case 2: case 3:
            	    x = gOptPrefLabXofsA[ i ] + gOptPrefKnobs[ WgNo ].x;
        	    gOptPrefKnobs[ WgNo ].x1 = x + gFont.LineWidth( stmp );
    	    }    	    
    	    if( (p = strchr( stmp, ' ')) ){ // space
        	*p = '\0';
        	gFont.Print( &gOptPrefSurface[ 640 * gOptKnobPosXY[ i ] + 640 * gOptPrefKnobs[ WgNo ].y + x ], stmp, 640, 640, PREF_COLOR_I );
        	gFont.Print( WIN_XY( x + 5, gOptPrefKnobs[ WgNo ].y + gOptKnobPosXY[ i ] + gFont.ChrHeight(), 640, gOptPrefSurface ), p + 1, 640, 640, PREF_COLOR_I );
    	    } else {
        	gFont.Print( WIN_XY( x, gOptKnobPosXY[ i ] + gOptPrefKnobs[ WgNo ].y, 640, gOptPrefSurface ), stmp, 640, 640, PREF_COLOR_I );
    	    }
	}
	n = *gOptPrefKnobs[ WgNo ].iVal;
	if( n >= 4 ){ eprintf( "OPTION MENU: Invalid column 1 value!" ); return 1; }
	ScrCopyAlpha( gOptPrefPix[ 2 ] + 46 * 47 * n, 46, 47, 46, WIN_XY( gOptPrefKnobs[ WgNo ].x, gOptPrefKnobs[ WgNo ].y, 640, gOptPrefSurface), 640 );
	return 1;
    }
    if( WgNo < 11 ){
        memcpy( Bpos, gOptKnobsBposY, sizeof( Bpos ) );
        n = WIN_XY( 251, Bpos[ WgNo - 5 ], 640, 0 );
        ScrCopy( gOptPrefPix[ 0 ] + n, 113, 34, 640, gOptPrefSurface + n, 640 ); // clear
        for( i = 0; i < 2; i++ ){
            p = MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, gOptPrefKnobs[ WgNo ].MsgId[ i ] );
            if( i == 0 ){
                x = gOptPrefKnobs[ WgNo ].x0 = gOptPrefKnobs[ WgNo ].x + gOptPrefLabXofsB[ 1 ] - gFont.LineWidth( p );
            } else {            
        	x = gOptPrefLabXofsB[ 2 ] + gOptPrefKnobs[ WgNo ].x;
                gOptPrefKnobs[ WgNo ].x1 = x + gFont.LineWidth( p );
            }
            gFont.Print( WIN_XY( x, gOptPrefKnobs[ WgNo ].y - 5, 640, gOptPrefSurface ), p, 640, 640, PREF_COLOR_I );
        }
	n = *gOptPrefKnobs[ WgNo ].iVal;
        if( WgNo == 5 ) n = ( n ^ 0x01) & 0x01;
        ScrCopyAlpha( gOptPrefPix[ 3 ] + 550 * n, 22, 25, 22, WIN_XY( gOptPrefKnobs[ WgNo ].x, gOptPrefKnobs[ WgNo ].y, 640, gOptPrefSurface ), 640 );
        return 1;        
    }
    if( WgNo >= 19 ) return 0;            

    n = 640 * (gOptPrefKnobs[ WgNo ].y - 12) + 384;
    ScrCopy( gOptPrefPix[ 0 ] + n, 240, 24, 640, gOptPrefSurface + n, 640 ); // clear screen area

    switch( WgNo ){
        case 11:
            n = *gOptPrefKnobs[ WgNo ].iVal;
            if( n < 0 ) n = 0;
            if( n > 50 ) n = 50;
            n = lround( (n - gOptPrefKnobs[ WgNo ].dMin) * (219.0 / (gOptPrefKnobs[ WgNo ].dMax - gOptPrefKnobs[ WgNo ].dMin ) ) + 384.0 );
            ScrCopyAlpha( gOptPrefPix[ 1 ], 21, 12, 21, WIN_XY( n, gOptPrefKnobs[ WgNo ].y, 640, gOptPrefSurface ), 640 );
            break;
        case 12:
            if( gOptSetTextDelay < 1.0 ) gOptSetTextDelay = 1.0;
            if( gOptSetTextDelay > 6.0 ) gOptSetTextDelay = 6.0;
            n = lround( (6.0 - gOptSetTextDelay) * 43.8 + 384.0 );
            ScrCopyAlpha( gOptPrefPix[ 1 ], 21, 12, 21, WIN_XY( n, gOptPrefKnobs[ WgNo ].y, 640, gOptPrefSurface ), 640 );
            dblVal = (gOptSetTextDelay + -1.0) * 0.2 * 2.0;
            if( dblVal < 0.0 ) dblVal = 0.0;
            if( dblVal > 2.0 ) dblVal = 2.0;
            TextSetBaseDelay( gOptSetTextDelay );
            TextSetLineDelay( dblVal );
            break;
        case 13: case 14: case 15: case 16:
            k = lround( gOptPrefKnobs[ WgNo ].dMin);
            u = lround( gOptPrefKnobs[ WgNo ].dMax);
            n = *gOptPrefKnobs[ WgNo ].iVal;
            if ( n < k ) n = k;
            if( n > u ) n = u;
            n = lround( (n - gOptPrefKnobs[ WgNo ].dMin) * (219.0 / (gOptPrefKnobs[ WgNo ].dMax - gOptPrefKnobs[ WgNo ].dMin)) + 384.0 );
            ScrCopyAlpha( gOptPrefPix[ 1 ], 21, 12, 21, WIN_XY( n, gOptPrefKnobs[ WgNo ].y, 640, gOptPrefSurface ), 640 );
            switch( WgNo ){
                case 13: GSoundSetMasterVolume( gOptSetMasterVolume ); break;
                case 14: GSoundSetMusicVolume( gOptSetMusicVolume ); break;
                case 15: GSoundSetSfxVolume( gOptSetFxVolume ); break;
                case 16: GSoundSetSpkVolume( gOptSetSpeechVolume ); break;
            }
            break;
        case 17:
            if( gOptSetBrightness < 1.0 ) gOptSetBrightness = 1.0;                        
            if( gOptSetBrightness > 1.18 ) gOptSetBrightness = 1.18;
            n = lround((gOptSetBrightness - gOptPrefKnobs[WgNo].dMin) * (219.0 / (gOptPrefKnobs[WgNo].dMax - gOptPrefKnobs[WgNo].dMin)) + 384.0);
            ScrCopyAlpha( gOptPrefPix[ 1 ], 21, 12, 21, WIN_XY( n, gOptPrefKnobs[ WgNo ].y, 640, gOptPrefSurface ), 640 );
            PalSetBrightness( gOptSetBrightness );
            break;
        case 18:
            if( gOptSetMouseSens < 1.0 ) gOptSetMouseSens = 1.0;
            if( gOptSetMouseSens > 2.5 ) gOptSetMouseSens = 2.5;
            n = lround( (gOptSetMouseSens - gOptPrefKnobs[WgNo].dMin) * (219.0 / (gOptPrefKnobs[ WgNo ].dMax - gOptPrefKnobs[ WgNo ].dMin )) + 384.0 );
            ScrCopyAlpha( gOptPrefPix[ 1 ], 21, 12, 21, WIN_XY( n, gOptPrefKnobs[ WgNo ].y, 640, gOptPrefSurface ), 640 );
            MseSetSpeed( gOptSetMouseSens );
            break;
    }

    for( i = 0; i < gOptPrefKnobs[WgNo].Positions; i++ ){
        p = MessageGetMessage( &gOptPrefMsgBase, &gOptPrefMsgLine, gOptPrefKnobs[ WgNo ].MsgId[ i ] );
        n = PREF_COLOR_I;
        switch( i ){
            case 0:
                x = 384;
                if( ( WgNo > 12 ) && ( WgNo < 17 ) && ( *gOptPrefKnobs[ WgNo ].dVal == gOptPrefKnobs[ WgNo ].dMin ) ) n = PREF_COLOR_J;
                break;
            case 1:
                switch( gOptPrefKnobs[ WgNo ].Positions ){
        	    case 2: x = 624 - gFont.LineWidth( p ); break;
        	    case 3: x = 504 - gFont.LineWidth( p ) / 2 - 2; break;
        	    default: x = lround( (gFont.LineWidth( p ) / 2) + 444.0 - 8.0 ); break;
                }
                break;
            case 2:
                if( gOptPrefKnobs[ WgNo ].Positions == 3 )
        	    x = 624 - gFont.LineWidth( p );
                else 
        	    x = lround( 564.0 - gFont.LineWidth( p ) - 4.0 );
                break;
            case 3:
                x = 624 - gFont.LineWidth( p );
                break;
        }
        gFont.Print( WIN_XY( x, gOptPrefKnobs[ WgNo ].y - 12, 640, gOptPrefSurface ), p, 640, 640, n );
    }
    return 1;        
}

void OptPrefDialogClose()
{
    int i;

    if( gOptPrefChanged ){
        OptSaveConfig( 1 );
        OptRealize();
        CombatUnk75();
    }
    WinClose( gPrefWindow );
    for( i = 0; i != 10; i++ ) ArtClose( gOptPrefImgs[ i ] );
}

int OptMenuInit()
{
    int i;

    for( i = 0; i != 11; i++ ) gOptPrefKnobs[ i ].Dir = 0;
    OptLoad();
    GrMakeGrayMap( 0, 255 );
    return 0;
}

void OptBrightInc()
{
    gOptSetBrightness = 1.0;
    CfgGetFloat( &gConfiguration, "preferences", "brightness", &gOptSetBrightness );
    if( gOptSetBrightness >= 1.18 ) return;
    gOptSetBrightness = gOptSetBrightness + 0.01125;
    if( gOptSetBrightness >= 1.0 ){
        if( gOptSetBrightness > 1.18 ) gOptSetBrightness = 1.18;
    } else {
        gOptSetBrightness = 1.0;
    }
    PalSetBrightness( gOptSetBrightness );
    CfgSetDouble( &gConfiguration, "preferences", "brightness", gOptSetBrightness );
    SysSaveConfiguration();
}

void OptBrightDec()
{
    gOptSetBrightness = 1.0;
    CfgGetFloat( &gConfiguration, "preferences", "brightness", &gOptSetBrightness );
    if ( gOptSetBrightness <= 1.0 ) return;
    gOptSetBrightness = gOptSetBrightness - 0.01125;
    if( gOptSetBrightness >= 1.0 ){
        if( gOptSetBrightness > 1.18 ) gOptSetBrightness = 1.18;
    } else {
        gOptSetBrightness = 1.0;
    }
    PalSetBrightness( gOptSetBrightness );
    CfgSetDouble( &gConfiguration, "preferences", "brightness", gOptSetBrightness );
    SysSaveConfiguration();
}

void OptLoad()
{
    OptSetDefault( 0 );
    CfgGetInteger(&gConfiguration, "preferences", "game_difficulty",   &gOptSetGameDifficulty );
    CfgGetInteger(&gConfiguration, "preferences", "combat_difficulty", &gOptSetCombatDifficulty );
    CfgGetInteger(&gConfiguration, "preferences", "violence_level",    &gOptSetViolenceLvl );
    CfgGetInteger(&gConfiguration, "preferences", "target_highlight",  &gOptSetTargetH1 );
    CfgGetInteger(&gConfiguration, "preferences", "combat_messages",   &gOptSetCombatMsg );
    CfgGetInteger(&gConfiguration, "preferences", "combat_looks",      &gOptSetCombatLook );
    CfgGetInteger(&gConfiguration, "preferences", "combat_taunts",     &gOptSetCombatTaunts );
    CfgGetInteger(&gConfiguration, "preferences", "language_filter",   &gOptSetLangFilter );
    CfgGetInteger(&gConfiguration, "preferences", "running",           &gOptSetRunning );
    CfgGetInteger(&gConfiguration, "preferences", "subtitles",         &gOptSetSubtitles );
    CfgGetInteger(&gConfiguration, "preferences", "item_highlight",    &gOptSetItemH1 );
    CfgGetInteger(&gConfiguration, "preferences", "combat_speed",      &gOptSetCombatSpeed );
    CfgGetFloat(  &gConfiguration, "preferences", "text_base_delay",   &gOptSetTextDelay );
    CfgGetInteger(&gConfiguration, "preferences", "player_speedup",    &gOptSetPlayerSpeed );
    CfgGetInteger(&gConfiguration, "sound",       "master_volume",     &gOptSetMasterVolume );
    CfgGetInteger(&gConfiguration, "sound",       "music_volume",      &gOptSetMusicVolume );
    CfgGetInteger(&gConfiguration, "sound",       "sndfx_volume",      &gOptSetFxVolume );
    CfgGetInteger(&gConfiguration, "sound",       "speech_volume",     &gOptSetSpeechVolume );
    CfgGetFloat(  &gConfiguration, "preferences", "brightness",        &gOptSetBrightness );
    CfgGetFloat(  &gConfiguration, "preferences", "mouse_sensitivity", &gOptSetMouseSens );
    OptRealize();
}

void OptSaveConfig( int ModifyFileFlg )
{
    double tmp;

    CfgSetInteger(&gConfiguration, "preferences", "game_difficulty", gOptSetGameDifficulty);
    CfgSetInteger(&gConfiguration, "preferences", "combat_difficulty", gOptSetCombatDifficulty);
    CfgSetInteger(&gConfiguration, "preferences", "violence_level", gOptSetViolenceLvl);
    CfgSetInteger(&gConfiguration, "preferences", "target_highlight", gOptSetTargetH1);
    CfgSetInteger(&gConfiguration, "preferences", "combat_messages", gOptSetCombatMsg);
    CfgSetInteger(&gConfiguration, "preferences", "combat_looks", gOptSetCombatLook);
    CfgSetInteger(&gConfiguration, "preferences", "combat_taunts", gOptSetCombatTaunts);
    CfgSetInteger(&gConfiguration, "preferences", "language_filter", gOptSetLangFilter);
    CfgSetInteger(&gConfiguration, "preferences", "running", gOptSetRunning);
    CfgSetInteger(&gConfiguration, "preferences", "subtitles", gOptSetSubtitles);
    CfgSetInteger(&gConfiguration, "preferences", "item_highlight", gOptSetItemH1);
    CfgSetInteger(&gConfiguration, "preferences", "combat_speed", gOptSetCombatSpeed);
    CfgSetDouble(&gConfiguration, "preferences", "text_base_delay", gOptSetTextDelay);
    tmp = (gOptSetTextDelay -1.0 ) * 0.2 * 2.0;
    if( tmp < 0.0 ) tmp = 0.0;    
    if( tmp > 2.0 ) tmp = 2.0;    
    CfgSetDouble(&gConfiguration, "preferences", "text_line_delay", tmp);
    CfgSetInteger(&gConfiguration, "preferences", "player_speedup", gOptSetPlayerSpeed);
    CfgSetInteger(&gConfiguration, "sound", "master_volume", gOptSetMasterVolume);
    CfgSetInteger(&gConfiguration, "sound", "music_volume", gOptSetMusicVolume);
    CfgSetInteger(&gConfiguration, "sound", "sndfx_volume", gOptSetFxVolume);
    CfgSetInteger(&gConfiguration, "sound", "speech_volume", gOptSetSpeechVolume);
    CfgSetDouble(&gConfiguration, "preferences", "brightness", gOptSetBrightness);
    CfgSetDouble(&gConfiguration, "preferences", "mouse_sensitivity", gOptSetMouseSens);

    if( ModifyFileFlg ) SysSaveConfiguration();
}

void OptSetDefault( int UpdateFlg )
{
    int i;

    gOptSetCombatDifficulty = 1;
    gOptSetViolenceLvl = 3;
    gOptSetTargetH1 = 2;
    gOptSetCombatMsg = 1;
    gOptSetCombatLook = 0;
    gOptSetCombatTaunts = 1;
    gOptSetRunning = 0;
    gOptSetSubtitles = 0;
    gOptSetItemH1 = 1;
    gOptSetCombatSpeed = 0;
    gOptSetPlayerSpeed = 0;
    gOptSetTextDelay = 3.5;
    gOptSetBrightness = 1.0;
    gOptSetMouseSens = 1.0;
    gOptSetGameDifficulty = 1;
    gOptSetLangFilter = 0;
    gOptSetMasterVolume = 22281;
    gOptSetMusicVolume = 22281;
    gOptSetFxVolume = 22281;
    gOptSetSpeechVolume = 22281;
    if( !UpdateFlg ) return;    

    for( i = 0; i < 19; i++ ) OptPrefUpdate( i );

    WinButtonSet( gOptSetUnk18, gOptSetPlayerSpeed, 0 );
    WinUpdate( gPrefWindow );
    gOptPrefChanged = 1;    
}

void OptPrefLoad()
{
    gOptGameDifficulty   = gOptSetGameDifficulty;
    gOptCombatDifficulty = gOptSetCombatDifficulty;
    gOptViolenceLvl      = gOptSetViolenceLvl;
    gOptTargetH1         = gOptSetTargetH1;
    gOptCombatLook       = gOptSetCombatLook;
    gOptCombatMsg        = gOptSetCombatMsg;
    gOptCombatTaunts     = gOptSetCombatTaunts;
    gOptLangFilter       = gOptSetLangFilter;
    gOptRunning          = gOptSetRunning;
    gOptSubtitles        = gOptSetSubtitles;
    gOptItemH1           = gOptSetItemH1;
    gOptCombatSpeed      = gOptSetCombatSpeed;
    gOptPlayerSpeedUp    = gOptSetPlayerSpeed;
    gOptMasterVolume     = gOptSetMasterVolume;
    gOptTextBaseDelay    = gOptSetTextDelay;
    gOptMusicVolume      = gOptSetMusicVolume;
    gOptBrightness       = gOptSetBrightness;
    gOptSndFxVolume      = gOptSetFxVolume;
    gOptMouseSensitivity = gOptSetMouseSens;
    gOptSpeechVolume     = gOptSetSpeechVolume;
}

void OptPrefSave()
{
    gOptSetGameDifficulty = gOptGameDifficulty;
    gOptSetCombatDifficulty = gOptCombatDifficulty;
    gOptSetViolenceLvl = gOptViolenceLvl;
    gOptSetTargetH1 = gOptTargetH1;
    gOptSetCombatLook = gOptCombatLook;
    gOptSetCombatMsg = gOptCombatMsg;
    gOptSetCombatTaunts = gOptCombatTaunts;
    gOptSetLangFilter = gOptLangFilter;
    gOptSetRunning = gOptRunning;
    gOptSetSubtitles = gOptSubtitles;
    gOptSetItemH1 = gOptItemH1;
    gOptSetCombatSpeed = gOptCombatSpeed;
    gOptSetPlayerSpeed = gOptPlayerSpeedUp;
    gOptSetMasterVolume = gOptMasterVolume;
    gOptSetTextDelay = gOptTextBaseDelay;
    gOptSetMusicVolume = gOptMusicVolume;
    gOptSetBrightness = gOptBrightness;
    gOptSetFxVolume = gOptSndFxVolume;
    gOptSetMouseSens = gOptMouseSensitivity;
    gOptSetSpeechVolume = gOptSpeechVolume;
    OptRealize();
}

void OptRealize()
{
    double TxtDelay;
    
    LIMIT( gOptSetGameDifficulty, 0, 2 );
    LIMIT( gOptSetCombatDifficulty, 0, 2 );
    LIMIT( gOptSetViolenceLvl, 0, 3 );
    LIMIT( gOptSetTargetH1, 0, 2 );
    LIMIT( gOptSetCombatMsg, 0, 1 );
    LIMIT( gOptSetCombatLook, 0, 1 );
    LIMIT( gOptSetCombatTaunts, 0, 1 );
    LIMIT( gOptSetLangFilter, 0, 1 );
    LIMIT( gOptSetRunning, 0, 1 );
    LIMIT( gOptSetSubtitles, 0, 1 );
    LIMIT( gOptSetItemH1, 0, 1 );
    LIMIT( gOptSetCombatSpeed, 0, 50 );
    LIMIT( gOptSetPlayerSpeed, 0, 1 );
    LIMIT( gOptSetTextDelay, 1.0, 6.0 );
    LIMIT( gOptSetMasterVolume, 0,0x7FFF );
    LIMIT( gOptSetMusicVolume, 0, 0x7FFF );
    LIMIT( gOptSetFxVolume, 0, 0x7FFF );
    LIMIT( gOptSetSpeechVolume, 0, 0x7FFF );
    LIMIT( gOptSetBrightness, 1.0, 1.18 );
    LIMIT( gOptSetMouseSens, 1.0, 2.5 );

    TextSetBaseDelay( gOptSetTextDelay );
    GmouseHLcfg();
    TxtDelay = (gOptSetTextDelay - 1.0 ) * 0.2 * 2.0;
    LIMIT( TxtDelay, 0.0, 2.0 );
    TextSetLineDelay( TxtDelay );
    AiUnk62();
    ScptMsgFree();
    GSoundSetMasterVolume( gOptSetMasterVolume );
    GSoundSetMusicVolume( gOptSetMusicVolume );
    GSoundSetSfxVolume( gOptSetFxVolume );
    GSoundSetSpkVolume( gOptSetSpeechVolume );
    MseSetSpeed( gOptSetMouseSens );
    PalSetBrightness( gOptSetBrightness );
}

int OptSaveData( xFile_t *fh )
{
    if( 
	dbputBei( fh, gOptSetGameDifficulty) == -1 || 
	dbputBei( fh, gOptSetCombatDifficulty) == -1 || 
	dbputBei( fh, gOptSetViolenceLvl) == -1 || 
	dbputBei( fh, gOptSetTargetH1) == -1 || 
	dbputBei( fh, gOptSetCombatLook) == -1 || 
	dbputBei( fh, gOptSetCombatMsg) == -1 || 
	dbputBei( fh, gOptSetCombatTaunts) == -1 || 
	dbputBei( fh, gOptSetLangFilter) == -1 || 
	dbputBei( fh, gOptSetRunning) == -1 || 
	dbputBei( fh, gOptSetSubtitles) == -1 || 
	dbputBei( fh, gOptSetItemH1) == -1 || 
	dbputBei( fh, gOptSetCombatSpeed) == -1 || 
	dbputBei( fh, gOptSetPlayerSpeed) == -1 || 
        dbputBed( fh, gOptSetTextDelay ) == -1 || 
	dbputBei( fh, gOptSetMasterVolume) == -1 || 
	dbputBei( fh, gOptSetMusicVolume) == -1 || 
	dbputBei( fh, gOptSetFxVolume) == -1 || 
	dbputBei( fh, gOptSetSpeechVolume) == -1 || 
	dbputBed( fh, gOptSetBrightness ) == -1 || 
	dbputBed( fh, gOptSetMouseSens ) == -1 
    ){
	return 0;
    }
    ErrorPrintf( "\nOPTION MENU: Error save option data!\n" );
    return -1;
}

int OptLoadData( xFile_t *fh )
{
    int err = -1, mse, brt, TextDelay;

    OptSetDefault(0);
    if( 
	dbgetBei( fh, &gOptSetGameDifficulty ) == -1 || 
	dbgetBei( fh, &gOptSetCombatDifficulty ) == -1 || 
	dbgetBei( fh, &gOptSetViolenceLvl ) == -1 || 
	dbgetBei( fh, &gOptSetTargetH1 ) == -1 || 
	dbgetBei( fh, &gOptSetCombatLook ) == -1 || 
	dbgetBei( fh, &gOptSetCombatMsg ) == -1 || 
	dbgetBei( fh, &gOptSetCombatTaunts ) == -1 || 
	dbgetBei( fh, &gOptSetLangFilter ) == -1 || 
	dbgetBei( fh, &gOptSetRunning ) == -1 || 
	dbgetBei( fh, &gOptSetSubtitles ) == -1 || 
	dbgetBei( fh, &gOptSetItemH1 ) == -1 || 
	dbgetBei( fh, &gOptSetCombatSpeed ) == -1 || 
	dbgetBei( fh, &gOptSetPlayerSpeed ) == -1 || 
	dbgetBei( fh, &TextDelay ) == -1 || 
	dbgetBei( fh, &gOptSetMasterVolume ) == -1 || 
	dbgetBei( fh, &gOptSetMusicVolume ) == -1 || 
	dbgetBei( fh, &gOptSetFxVolume ) == -1 || 
	dbgetBei( fh, &gOptSetSpeechVolume ) == -1 || 
	dbgetBei( fh, &brt ) == -1 || 
	dbgetBei( fh, &mse ) == -1 
    ) err = 0;
    gOptSetBrightness = *(float *)&brt;
    gOptSetMouseSens  = *(float *)&mse;
    gOptSetTextDelay  = *(float *)&TextDelay;
    if( err ){
        OptRealize();
        OptSaveConfig( 0 );
        return 0;
    }
    ErrorPrintf( "\nOPTION MENU: Error loading option data!, using defaults.\n" );
    OptSetDefault( 0 );
    OptRealize();
    OptSaveConfig( 0 );
    return -1;
}

