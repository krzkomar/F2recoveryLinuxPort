#include "FrameWork.h"

static int   gMainMenuWin = -1;
static char *gWallPaper = NULL;
static char *gImgButtUp = NULL;
static char *gImgButtDn = NULL;
static int   gMainMenuHandlerGuard = 0;
static int   gMainMenuGuard = 0;
static int   gMainMenuTimeout = 120000; // 2min
static char  gMainMenuHotKeys[ 6 ] = {'i','n','l','o','c','e' };
static int   gMainMenuSelCodes[ 6 ] = { 0,1,2,9,5,7 };


int  gMainMenuButtons[6];
int  gMainMenuUpdate;
CachePool_t *gMainMenuButtonUpImg;
CachePool_t *gMainMenuButtonDnImg;
CachePool_t *gMainMenuWallPaper;


/**************************/

int MainMenuCreate()
{
    static char *surf;
    int lw, i, w, color, FontRestore;
    char GameVersionStr[ 32 ];
    MsgLine_t msg, mline;

    if( gMainMenuGuard ) return 0;
    PalLoadFromFile( "color.pal" );
    if( ( gMainMenuWin = WinCreateWindow( 0, 0, MAIN_MENU_WIDTH, MAIN_MENU_HEIGHT, 0, 0x0C ) ) == -1 ){ MainMenuDestroy(); return -1; }
    surf = WinGetSurface( gMainMenuWin );

    gWallPaper = ArtGetBitmap( ArtMakeId( MAIN_MENU_ART_WALLPAPER ) , 0, 0, &gMainMenuWallPaper );
    if( !gWallPaper ){ MainMenuDestroy(); return -1; }
    ScrCopy( gWallPaper, MAIN_MENU_WIDTH, MAIN_MENU_HEIGHT, MAIN_MENU_WIDTH, surf, MAIN_MENU_WIDTH );
    ArtClose( gMainMenuWallPaper );
    FontRestore = FontGetCurrent();
    FontSet( MAIN_MENU_FONT );
    mline.Id = MAIN_MENU_TXT_COPYRIGHT;
    if( MessageGetMsg( &gMessage, &mline ) == 1 ) WinDrawText( gMainMenuWin, mline.Text, 0, 15, 460, MAIN_MENU_COLOR | 0x6000000 );
    FontSet( MAIN_MENU_FONT );
    MainGameVersion( GameVersionStr );
    color = MAIN_MENU_COLOR | 0x6000000;
    WinDrawText( gMainMenuWin, GameVersionStr, 0, 615 - gFont.LineWidth( GameVersionStr ), 460, color );
    if( (gImgButtUp = ArtGetBitmap( ArtMakeId( MAIN_MENU_ART_BUTTUP ), 0, 0, &gMainMenuButtonUpImg ) ) == 0 ){ MainMenuDestroy(); return -1; }
    if( (gImgButtDn = ArtGetBitmap( ArtMakeId( MAIN_MENU_ART_BUTTDN ), 0, 0, &gMainMenuButtonDnImg ) ) == 0 ){ MainMenuDestroy(); return -1; }
    memset( gMainMenuButtons, 0xff, 6 * sizeof( int ) );

    for( i = 0; i < 6; i++ ){
        w = WinCreateButton( gMainMenuWin, 30, 19 + i*41, 26, 26, -1, -1, 1111, gMainMenuHotKeys[ i ], gImgButtUp, gImgButtDn, 0, 0x20 );
        gMainMenuButtons[ i ] = w;
        if( w == -1 ){ MainMenuDestroy(); return -1; }
        WinSetDefaultImg( w, gImgButtUp );
    }
    FontSet( MAIN_MENU_FONT_BUTT );

    for( i = 0; i < 6; i++ ){
        msg.Id = i + MAIN_MENU_TXT_BUTTs;
        if( MessageGetMsg( &gMessage, &msg ) == 1 ){
            lw = gFont.LineWidth( msg.Text );
    	    gFont.Print( 
    		WIN_XY( 126 - lw / 2, 20 + i*41, MAIN_MENU_WIDTH, surf ), 
    		msg.Text, MAIN_MENU_WIDTH - (126 - lw / 2) - 1, MAIN_MENU_WIDTH, MAIN_MENU_COLOR
    	    );
        }
    }
    FontSet( FontRestore );
    gMainMenuGuard = 1;
    gMainMenuUpdate = 1;
    return 0;
}

void MainMenuDestroy()
{
    int i;

    if( !gMainMenuGuard ) return;    
    for( i = 0; i < 6; i++ ){
        if( gMainMenuButtons[ i ] != -1 ) continue;
        WinImageRemove( gMainMenuButtons[ i ] );
        gMainMenuButtons[ i ] = -1;
    }
    if( gImgButtDn ){
        ArtClose( gMainMenuButtonDnImg );
        gMainMenuButtonDnImg = NULL;
        gImgButtDn = NULL;
    }
    if( gImgButtUp ){
        ArtClose( gMainMenuButtonUpImg );
        gMainMenuButtonUpImg = NULL;
        gImgButtUp = NULL;
    }
    if( gMainMenuWin != -1 ) WinClose( gMainMenuWin );
    gMainMenuGuard = 0;
}

void MainMenuUpdate( int Fade )
{
    if( !gMainMenuGuard || gMainMenuUpdate ) return;    
    SoundUpdateAll();
    if( Fade ){
        FadeStep( gFadePaletteC );
        SoundUpdateAll();
    }
    WinUpdateDirty( gMainMenuWin );
    gMainMenuUpdate = 1;    
}

void MainMenuFadeInit( int Fade )
{
    if( !gMainMenuGuard || !gMainMenuUpdate ) return;
    
    WinMoveTop(gMainMenuWin);
    if( Fade ){
        PalLoadFromFile( "color.pal" );
        FadeStep( gPalBase );
    }
    gMainMenuUpdate = 0;    
}

int MainMenuIsDirty()
{
    return gMainMenuGuard && (gMainMenuUpdate == 0);
}

void MainMenuSetTimeout( int sec )
{
    gMainMenuTimeout = 60000 * sec;
}

unsigned int MainMenuGetTimeout()
{
    return gMainMenuTimeout / 60000;
}

int MainMenuHandle()
{
    int SelCode,IsCursorClear, CursorClean,time, key, i;

    gMainMenuHandlerGuard = 1;
    IsCursorClear = MseIsCursorClear();
    CursorClean = (IsCursorClear == 0);
    if( IsCursorClear ) MseDrawCursor();
    time = TimerGetSysTime();

    for( SelCode = -1; SelCode == -1; ){
        key = InpUpdate();
        for( i = 0; i < MAIN_MENU_POSITIONS; i++ ){
            if( (key == gMainMenuHotKeys[ i ]) || key == toupper( gMainMenuHotKeys[ i ] ) ){
    		GSoundPlay( "nmselec1" );
    		SelCode = gMainMenuSelCodes[ i ];
    		if( i == 1 && (gKeyStates['6'] || gKeyStates['*']) ) SelCode = 6;
        	break;
            }
        }
        if( SelCode != -1 ) continue;
    	switch( key ){
    	    case 18: SelCode = 8; continue;
    	    case KEY_ESC: SelCode = 7; GSoundPlay( "nmselec1" ); continue;
//    	    case '+': case '=': OptionsBrightInc(); continue;
//    	    case '-': case '_': OptionsBrightDec(); continue;
    	    case 'D': case 'd': SelCode = 3; continue;
    	    case INP_SELECTED:
        	if( !(MseGetButtons() & MSE_LBUTT_HOLD) ) GSoundPlay( "nmselec0" );
        	continue;
    	}  
    	if( gMenuEscape == 3 ){
    	    SelCode = 7;
    	    GSoundPlay("nmselec1");
    	    continue;
    	}
        if( gMenuEscape == 2 ){
            gMenuEscape = 0;
        } else if ( TimerCurrDiff(time) >= gMainMenuTimeout ){
            SelCode = 4;
        }        
    }
    if( !CursorClean ) MseCursorRedraw();
    gMainMenuHandlerGuard = 0;
    return SelCode;
}

int MainMenuKill()
{
    MainMenuDestroy();
    return -1;
}


