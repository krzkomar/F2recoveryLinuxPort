#include "FrameWork.h"

#define SCREENSAVER_TIME 120000	// 2 minutes
typedef struct 
{
    char Show;		// 1 - on screen, 0 - fired
    int xpos;		// x position
    int ypos;		// y position
    float Speed;	// Speed
    float Move;		// Move counter (frames)
} PipSaver_t;

static int PipQuestSortCb( PipQuests_t *q1, PipQuests_t *q2 );

VidRect_t gPipRegion ={ 0xFE, 0x2E, 0x274, 0x1C8 };
int gPipImgIds[11] = { 0x08, 0x09, 0x52, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0xE2 };

PipQuests_t *gPipQuestList = NULL;
int gPipQuestCount = 0;
PipHoloDisk_t *gPipHoloTapes = NULL;
int gPipHoloTapesCount = 0;
int gPipAlarms = 14;
int gPipUnk04 = 0;
Pip01_t gPipUnk23[ 8 ] = { 
    { 0x01, 0x01, 0x64 }, { 0x02, 0x0E, 0x65 }, { 0x04, 0x01, 0x66 }, { 0x07, 0x04, 0x68 }, 
    { 0x0A, 0x06, 0x67 }, { 0x0A, 0x1F, 0x69 }, { 0x0B, 0x1C, 0x6A }, { 0x0C, 0x19, 0x6B }
};

void (*gPipHandlers[ 6 ])(int) = { PipStatus, PipAutomap, PipArchives, PipAlarmClock, PipAlarmClock, (void *)-1 };

Geo_t gPipGeo[11];
MsgLine_t gPipMsgLine;
Msg_t gPipMsg;
Pip_t gPipLocNames[24];
Msg_t gPipUnk28;
int  gPipCurrentTasks;
char *gPipSurface;
char *gPipImgs[11];
int gPipHoloTapesEnabled;
int gPipMseY1;
int gPipMseX1;
int gPipTime;
int gPipButtons[ 24 ];
int gPipUnk46;
int gPipMseX0;
int gPipMseY0;
int gPipWin;
CachePool_t *gPipImgHan[11];
int gPipUnk33;
int gPipListViewed;
int gPipFontSave;
int gPipFontSave;
int gPipExit;
int gPipUnk40;
int gPipMenuSel;
int gpipAutomapLocations;
int gPipListBase;
int gPipLineRow;
int gPipUnk07;
int gPipMapId;
int gPipArchivesEnabled;
int gPipTermLines;
int gPipUnk12;
int gPipUnk32;
int gPipUnk36;

/*************************************/

void PipMenu( int Clock )
{
    int err, sel;

    if( !WmPipBoyEquipped() ){ // 'You aren't wearing a pipboy'
        DlgBox( MessageGetMessage( &gMessage, &gPipMsgLine, 7000 ), 0, 0, 192, 135, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 1 );
        return;
    }    
    if( (err = PipCreate( Clock )) == -1 ) return;
    MseGetCursorPosition( &gPipMseX0, &gPipMseY0 );
    gPipTime = TimerGetSysTime();
    while( 1 ){
        sel = InpUpdate();
        if( err == 1 ) sel = 504;
        MseGetCursorPosition( &gPipMseX1, &gPipMseY1 );

        if( sel == -1 && gPipMseX1 == gPipMseX0 && gPipMseY1 == gPipMseY0 ){
            if( (TimerGetSysTime() - gPipTime) > SCREENSAVER_TIME ){
                PipScreenSaver();
                gPipTime = TimerGetSysTime();
                MseGetCursorPosition( &gPipMseX0, &gPipMseY0 );
            }
        } else {
            gPipTime = TimerGetSysTime();
            gPipMseX0 = gPipMseX1;
            gPipMseY0 = gPipMseY1;
        }
        if( gMenuEscape || sel == 503 ){ PipClose(); return; }
        switch( sel ){
    	case 17: case 24: case 324: SysQuitDlg(); PipClose(); return;
    	case 13: case 27: case 'P': case 'p': PipClose(); return;
            case 329: gPipHandlers[ gPipMenuSel ]( 1027 ); break;
            case 337: gPipHandlers[ gPipMenuSel ]( 1026 ); break;
            case 390: InpScrShotTake();  break;
            case 500 ... 504: gPipMenuSel = sel - 500; gPipHandlers[ gPipMenuSel ]( 1024 ); break; // menu button selection
            case 505 ... 527: gPipHandlers[ gPipMenuSel ]( sel - 506 ); break;// terminal line selection
            case 528: gPipHandlers[ gPipMenuSel ]( 1025 ); break; // alarm clock 'bell'
        }                
        if( gPipExit ){ PipClose(); return; }
    }
}

int PipCreate( int a1 )
{
    char stmp[260];
    int win,i,bt,n,w,h,Day,Month;

    gPipUnk04 = MapUnk35();
    CycleColorStop();
    GmouseUnk03();
    IfaceIndicatorBoxHide();
    GmouseLoadCursor( 1 );
    gPipAlarms = 13;
    if( PartyUnk12() > 1 && PartyIsWounded() ) gPipAlarms = 14;
    gPipFontSave = FontGetCurrent();
    FontSet( 101 );
    gPipExit = 0;
    gPipUnk07 = 0;
    gPipLineRow = 0;
    gPipListViewed = 0;
    gPipTermLines = 410 / gFont.ChrHeight() - 1;
    gPipListBase = 0;
    gPipUnk12 = 0;
    if( PipHoloTapesLoad() == -1 ) return -1;
    if( MessageInit( &gPipMsg ) != 1 ) return -1;
    sprintf( stmp, "%s%s", gGamePath, "pipboy.msg" );
    if( MessageLoad( &gPipMsg, stmp ) != 1 ) return -1;
    for( i = 0; i < 11; i++ ){
        if( !(gPipImgs[ i ] = ArtLoadBmp( ArtMakeId( 6, gPipImgIds[ i ], 0, 0, 0 ), &gPipImgHan[ i ], &gPipGeo[ i ].Width, &gPipGeo[ i ].Height )) ) break;
    }
    if( i < 11 ){
        eprintf( "\n** Error loading pipboy graphics! **\n" );
        for( i--; i; i-- ) ArtClose( gPipImgHan[ i ] );
        return -1;
    }
    win = WinCreateWindow( 0, 0, 640, 480, gPalColorCubeRGB[0][0][0], 16 );
    gPipWin = win;
    if( win == -1 ){
        eprintf( "\n** Error opening pipboy window! **\n" );
        for( i = 0; i < 11; i++ ) ArtClose( gPipImgHan[i] );
        return -1;
    }
    gPipSurface = WinGetSurface( win );
    memcpy( gPipSurface, gPipImgs[ 3 ], gPipGeo[ 3 ].Width * gPipGeo[ 3 ].Height );
    PipPrintCounter( ScptGetGameTime(), 4, 155, 17 );
    PipPrintDate();
    // cancel button
    bt = WinCreateButton( gPipWin, 124, 13, gPipGeo[ 8 ].Width, gPipGeo[ 8 ].Height, -1, -1, -1, 504, gPipImgs[ 8 ], gPipImgs[ 7 ], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayCheck, GSoundPlayUnCheck );
    // status/automap/archive buttons
    for( i = 0, n = 500; i < 5; i++ ){ // buttons 500 ... 503
        if( i == 1 ) continue;
        bt = WinCreateButton( gPipWin, 53, 341 + i*27, gPipGeo[ 0 ].Width, gPipGeo[ 0 ].Height, -1, -1, -1, n++, gPipImgs[ 0 ], gPipImgs[ 1 ], 0, 32 );
        if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );        
    }

    if( a1 == 1 ){ // alarm clock as first page
        if( !CritterUnk46() ){
            ScrCopyAlpha( gPipImgs[ 9 ], gPipGeo[ 9 ].Width, gPipGeo[ 9 ].Height, gPipGeo[ 9 ].Width, gPipSurface + 100163, 640 );
            ScptGetGameDate( &Month, &Day, &h );
            for( i = 0; i < 8; i++  ){
                if( gPipUnk23[ i ].w01 != Month ) continue;
                if( (Month & 0xffff0000) | ((gPipUnk23[ i ].w02 & 0xffff) != Day) ) continue;
        	strcpy( stmp, MessageGetMessage(&gPipMsg, &gPipMsgLine, gPipUnk23[ i ].TxtId) );
        	w = (gPipGeo[9].Width / 2) + 323;
        	gFont.Print( &gPipSurface[640 * gPipGeo[9].Height + 111366 + w - (gFont.LineWidth(stmp) / 2 ) ], stmp, 350, 640, gPalColorCubeRGB[0][31][0] );
                break;                                
            }
            WinUpdate( gPipWin );
            GSoundPlay( "iisxxxx1" );
            DlgBox( MessageGetMessage( &gPipMsg, &gPipMsgLine, 215 ), 0, 0, 192, 135, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 1);
            a1 = 0;
        }
    } else {
        ScrCopyAlpha( gPipImgs[9], gPipGeo[9].Width, gPipGeo[9].Height, gPipGeo[9].Width, gPipSurface + 100163, 640 );
        ScptGetGameDate( &Month, &Day, &h );
        for( i = 0; i < 8; i++ ){
            if( gPipUnk23[ i ].w01 != Month ) continue;
            if( (Month & 0xffff0000) | ((gPipUnk23[ i ].w02 & 0xffff) != Day) ) continue;
            strcpy( stmp, MessageGetMessage( &gPipMsg, &gPipMsgLine, gPipUnk23[ i ].TxtId ));
            w = (gPipGeo[9].Width / 2) + 323;
            gFont.Print( &gPipSurface[640 * gPipGeo[9].Height + 111366 + w - (gFont.LineWidth( stmp ) / 2 ) ], stmp, 350, 640, gPalColorCubeRGB[0][31][0] );
            break;
        }
        WinUpdate( gPipWin );
    }
    if( PipQuestsLoad() == -1 ) return -1;
    GSoundPlay( "pipon" );
    WinUpdate( gPipWin );
    return a1;
}

void PipClose()
{
    int i;

    i = 0;
    CfgGetInteger( &gConfiguration, "debug", "show_script_messages", &i );
    if( i ) eprintf( "\nScript <Map Update>" );
    ScptUnk30();
    WinClose( gPipWin );
    MessageClose( &gPipMsg );
    if( gPipHoloTapes ){
        Free( gPipHoloTapes );
        gPipHoloTapes = NULL;
    }
    gPipHoloTapesCount = 0;
    for( i = 0; i < 11; i++ ) ArtClose( gPipImgHan[ i ] );
    PipDeleteLineRegions();
    FontSet( gPipFontSave );
    if( gPipUnk04 ) MapUnk34();
    CycleColorStart();
    IfaceIndicatorBoxShow();
    GmouseLoadCursor(1);
    IfaceUnk09();
    if( gPipQuestList ){
        Free( gPipQuestList );
        gPipQuestList = NULL;
    }
    gPipQuestCount = 0;
    MessageClose( &gPipUnk28 );
    PipInit();
}

void PipInit()
{
    /* Empty function */
}

void PipPrintCounter( int Value, int Digits, int Xpos, int Ypos )
{
    int i, x, p;

    x = Xpos + 9 * (Digits - 1);
    p = 640 * Ypos + x;
    for( i = 0; i < Digits; i++ ){
        ScrCopy( &gPipImgs[ 2 ][ 9 * (Value % 10) ], 9, 17, 360, &gPipSurface[ p ], 640 );
        x -= 9;
        p -= 9;
        Value /= 10;
    }        
}

void PipPrintDate()
{
    int Month, Year, Day;

    ScptGetGameDate( &Month, &Day, &Year );
    PipPrintCounter( Day, 2, 20, 17 );
    ScrCopy( &gPipImgs[ 5 ][ 435 * Month - 435 ], 29, 14, 29, gPipSurface + 11566, 640 );
    PipPrintCounter( Year, 4, 83, 17 );
}

void PipPrintLine( char *str, int Flags, int Color )
{
    int x, TxtColor, w;

    x = 8;
    TxtColor = Color;
    if( Flags & 0x08 ) TxtColor |= FONT_UNDERLINE;
    if( Flags & 0x80 ) x -= 7;
    w = gFont.LineWidth( str );
    if( Flags & 0x02 ){ x = (350 - w) / 2;  } else
    if( Flags & 0x04 ){ x += 175; } else
    if( Flags & 0x10 ){ x += (86 - w) + 16; } else
    if( Flags & 0x20 ){ x += (260 - w); }

    gFont.Print( gPipSurface + 640 * gPipLineRow * gFont.ChrHeight() + 29694 + x, str, 640, 640, TxtColor );
    // score a text line out
    if( (Flags & 0x40) ) ScrLine( gPipSurface, 640, 254 + x, 49 + gPipLineRow * gFont.ChrHeight(), 254 + x + w,   49 + gPipLineRow * gFont.ChrHeight(), Color );
    if( gPipLineRow < gPipTermLines ) gPipLineRow++;
}

void PipBaclLink( int a1 )
{
    if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
    ScrCopy( gPipImgs[ 3 ] + 640*436 + 254, 350, 20, 640, gPipSurface + 640*436 + 254, 640 );
    PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 201 ), 2, a1 );
}

int PipFSave( xFile_t * fh )
{
    return 0;
}

int PipFLoad( xFile_t * fh )
{
    return 0;
}

void PipStatus( int Selected )
{
    int i,j,n,v19,v57,v58,v59,v61;
    char *v28, *v29, v53[ 1024 ], c;
    short WrapList[ 64 ], pWraps;

    if( Selected == 1024 ){
        PipDeleteLineRegions();
        ScrCopy( gPipImgs[ 3 ] + 640*46 + 254, 374, 410, 640, gPipSurface + 640*46 + 254, 640 );
        if( gPipTermLines >= 0 ) gPipLineRow = 0;
        gPipUnk32 = 0;
        gPipUnk33 = -1;
        gPipHoloTapesEnabled = 0;
        gPipArchivesEnabled = 0;
        gPipUnk36 = 0;
        for( i = 0; i < gPipHoloTapesCount; i++ ){
    	    if( gGVals[ gPipHoloTapes[ i ].a ] ){
        	gPipHoloTapesEnabled++;
        	break;
    	    }
        }            
        PipCitiesList( -1 );
        if( !gPipCurrentTasks ) PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 203 ), 0, gPalColorCubeRGB[0][31][0] ); // 'No current tasks.'
        gPipHoloTapesEnabled = PipTapesList( -1 );
        WinAreaUpdate( gPipWin, &gPipRegion );
        PipMakeLineRegions( 2, gPipCurrentTasks + gPipHoloTapesEnabled + 1, 0 );
        WinUpdate( gPipWin );
        return;
    }
    if( !gPipUnk36 && !gPipUnk32 ){
        if( gPipCurrentTasks && gPipMseX1 < 429 ){ // click on Status
            GSoundPlay( "ib1p1xx1" );
            ScrCopy( gPipImgs[ 3 ] + 640*46 + 254, 374, 410, 640, gPipSurface + 640*46 + 254, 640 );
            PipCitiesList( Selected );
            PipTapesList( -1 );
            WinAreaUpdate( gPipWin, &gPipRegion );
            TimerWaitProc( 200 );
            gPipUnk36 = 1;
        } else if( gPipHoloTapesEnabled && gPipHoloTapesEnabled >= Selected && gPipMseX1 > 429 ){ // click on Data
            GSoundPlay( "ib1p1xx1" );
            gPipUnk33 = 0;
            for( i = 0; i < gPipHoloTapesCount; i++ ){
                if( gGVals[ gPipHoloTapes[ i ].a ] <= 0 ) continue;                
                if( Selected - 1 == gPipUnk33 ) break;
                gPipUnk33++;
            }
            gPipUnk33 = i;
            ScrCopy( gPipImgs[ 3 ] + 29694, 374, 410, 640, gPipSurface + 29694, 640 );
            PipTapesList( gPipUnk33 );
            PipCitiesList( -1 );
            WinAreaUpdate( gPipWin, &gPipRegion );
            TimerWaitProc( 200 );
            PipDeleteLineRegions();
            PipHoloList();
            PipMakeLineRegions( 0, 0, 1 );
            gPipUnk32 = 1;
        }
    }
    if( !gPipUnk36 ){
        if( !gPipUnk32 || Selected < 1025 || Selected > 1027 ) return;
        if( ((gPipMseX1 > 459) && (Selected != 1027)) || Selected == 1026 ){
            if( gPipButtons[ 0 ] <= gPipArchivesEnabled ){
                if( Selected != 1026 ){
                    GSoundPlay( "ib1p1xx1" );
                    ScrCopy( gPipImgs[ 3 ] + 279294, 350, 20, 640, gPipSurface + 279294, 640 );
                    if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
                    PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 201 ), 16, gPalColorCubeRGB[0][31][0] );
                    if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
                    PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 214 ), 32, gPalColorCubeRGB[31][31][11] );
                    WinAreaUpdate( gPipWin, &gPipRegion );
                    TimerWaitProc( 200 );
                    PipStatus( 1024 );
                }
            } else {
                GSoundPlay( "ib1p1xx1" );
                ScrCopy( gPipImgs[ 3 ] + 279294, 350, 20, 640, gPipSurface + 279294, 640 );
                if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
                PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 201 ), 16, gPalColorCubeRGB[0][31][0] );
                if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
                PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 200 ), 32, gPalColorCubeRGB[31][31][11] );
                WinAreaUpdate( gPipWin, &gPipRegion );
                TimerWaitProc( 200 );
                gPipArchivesEnabled++;
                PipHoloList();
            }
            return;
        }
        if( Selected == 1027 ){
            GSoundPlay( "ib1p1xx1" );
            ScrCopy( gPipImgs[ 3 ] + 279294, 350, 20, 640, gPipSurface + 279294, 640 );
            if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
            PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 201 ), 16, gPalColorCubeRGB[0][31][0] );
            if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
            PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 200 ), 32, gPalColorCubeRGB[31][31][11] );
            WinAreaUpdate( gPipWin, &gPipRegion );
            TimerWaitProc( 200 );
            if( --gPipArchivesEnabled < 0 ){ PipStatus( 1024 ); return; }
        } else {
            if( gPipMseX1 > 395 ) return;
            GSoundPlay( "ib1p1xx1" );
            ScrCopy( gPipImgs[ 3 ] + 279294, 350, 20, 640, gPipSurface + 279294, 640 );
            if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
            PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 201), 16, gPalColorCubeRGB[0][31][0] );
            if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
            PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 200 ), 32, gPalColorCubeRGB[31][31][11] );
            WinAreaUpdate( gPipWin, &gPipRegion );
            TimerWaitProc( 200 );
            if( gPipArchivesEnabled <= 0 ){ PipStatus( 1024 ); return; }
            gPipArchivesEnabled--;
        }
        PipHoloList();
        return;
    }
    if( Selected == 1025 ){
        GSoundPlay( "ib1p1xx1" );
        PipBaclLink( gPalColorCubeRGB[31][31][11] );
        WinAreaUpdate( gPipWin, &gPipRegion );
        TimerWaitProc( 200 );
        PipStatus( 1024 );
    }
    if( Selected > gPipCurrentTasks ) return;

    GSoundPlay( "ib1p1xx1" );
    for( n = v61 = 0; v61 < gPipQuestCount; v61++ ){
        if( gPipQuestList[ v61 ].d > gGVals[ gPipQuestList[ v61 ].c ] ) continue;
        if( n == Selected - 1 ) break;
        n++;
        for( j = v61; j < gPipQuestCount; j++, v61++ ){
            if( gPipQuestList[ j ].a != gPipQuestList[ j + 1 ].a ) break;
        }
    }
    PipDeleteLineRegions();
    ScrCopy( gPipImgs[3] + 29694, 374, 410, 640, gPipSurface + 29694, 640 );
    if( gPipTermLines >= 0 ) gPipLineRow = 0;
    if( gPipTermLines >= 1 ) gPipLineRow = 1;

    PipMakeLineRegions( 0, 0, 1 );
    sprintf( v53, "%s %s", MessageGetMessage( &gMapMsg, &gPipMsgLine, gPipQuestList[ v61 ].a ), MessageGetMessage( &gPipMsg, &gPipMsgLine, 210 ) );
    PipPrintLine( v53, 8, gPalColorCubeRGB[0][31][0] );
    if( gPipTermLines >= 3 ) gPipLineRow = 3;

    v59 = v61;
    v58 = 1;
    v57 = v61 + 1;
    for( ;v61 < gPipQuestCount; v61++, v57++, v59++ ){
        v19 = gGVals[ gPipQuestList[ v59 ].c ];
        if( v19 >= gPipQuestList[ v59 ].d ){
            sprintf( v53, "%d. %s", v58++, MessageGetMessage( &gPipUnk28, &gPipMsgLine, gPipQuestList[ v59 ].b ) );
            if( WinTextWrap( v53, 350, WrapList, &pWraps ) ){
                eprintf( "\n ** Word wrap error in pipboy! **\n" );
            } else {
                for( i = 0; i < pWraps - 1; i++ ){
                    v28 = v53 + WrapList[ i + 0 ];
                    v29 = v53 + WrapList[ i + 1 ];
                    c = *v29;
                    *v29 = '\0';
                    if( v19 < gPipQuestList[ v59 ].e ){
                        PipPrintLine( v28, 0x00, gPalColorCubeRGB[0][31][0] );
                    } else {
                        PipPrintLine( v28, 0x40, gPalColorCubeRGB[8][19][4] );
                    }                    
                    *v29 = c;
                    gPipLineRow++;
                }
            }
        }
        if( gPipQuestCount - 1 != v61 && gPipQuestList[ v57 ].a != gPipQuestList[ v59 ].a ) break;
    }    
    PipBaclLink( gPalColorCubeRGB[0][31][0] );
    WinAreaUpdate( gPipWin, &gPipRegion );
    gPipUnk36 = 1;
}

void PipCitiesList( int Selected )
{
    int i;

    if( gPipTermLines >= 0 ) gPipLineRow = 0;
    PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 202 ), (( gPipHoloTapesEnabled ) ? 16 : 2) | 0x08, gPalColorCubeRGB[0][31][0] );
    if( gPipTermLines >= 2 ) gPipLineRow = 2;
    gPipCurrentTasks = 0;
    for( i = 0; i < gPipQuestCount; i++ ){
        if( gPipQuestList[ i ].d > gGVals[ gPipQuestList[ i ].c ] ) continue;        
        PipPrintLine( 
    	    MessageGetMessage( &gMapMsg, &gPipMsgLine, gPipQuestList[i].a ), 0, 
    	    ((gPipLineRow - 1) / 2 == (Selected - 1)) ? gPalColorCubeRGB[31][31][11] : gPalColorCubeRGB[0][31][0] 
    	);
        gPipLineRow++;
        gPipCurrentTasks++;
        for( ;i < gPipQuestCount; i++ ){
            if( gPipQuestList[ i ].a != gPipQuestList[ i + 1 ].a ) break;            
        }        
    }        
}

void PipHoloList()
{
    int v2, c, v6, v9, v11, i;
    char *s, stmp[ 40 ];
    PipHoloDisk_t *v5;

    ScrCopy( gPipImgs[3] + 29694, 374, 410, 640, gPipSurface + 29694, 640 );
    if( gPipTermLines >= 0 ) gPipLineRow = 0;
    gPipButtons[ 0 ] = 0;
    c = gPipHoloTapes[ gPipUnk33 ].c;
    v2 = 0;
    while( 1 ){
	if( c >= gPipHoloTapes[ gPipUnk33 ].c + 500 ) break;
        if( !strcmp( MessageGetMessage( &gPipMsg, &gPipMsgLine, c++), "**END-DISK**" ) ) break;
        if( ++v2 >= 35 ){ v2 = 0; gPipButtons[ 0 ]++; }
    }
    if( c >= gPipHoloTapes[ gPipUnk33 ].c + 500 ) eprintf( "\nPIPBOY: #1 Holodisk text end not found!\n" );
    v5 = &gPipHoloTapes[ gPipUnk33 ];
    v6 = v5->c;
    if( gPipArchivesEnabled ){        
        for( v9 = v11 = 0; v6 < gPipHoloTapes[ gPipUnk33 ].c + 500; v6++ ){
            if( !strcmp( MessageGetMessage( &gPipMsg, &gPipMsgLine, v6 ), "**END-DISK**") ){
                eprintf( "\nPIPBOY: Premature page end in holodisk page search!\n" );
                break;
            }
            v11++;
            if( v11 >= 35 && ++v9 >= gPipArchivesEnabled ) break;            
        }
        if( ++v6 >= gPipHoloTapes[gPipUnk33].c + 500 ) eprintf( "\nPIPBOY: #2 Holodisk text end not found!\n" );
    } else {
        PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, v5->b), 10, gPalColorCubeRGB[0][31][0] );
    }
    if( gPipButtons[ 0 ] ){ // page counter 'x of n'
        sprintf( stmp, "%d %s %d", gPipArchivesEnabled + 1, MessageGetMessage( &gPipMsg, &gPipMsgLine, 212 ), gPipButtons[ 0 ] + 1 );
        gFont.Print( &gPipSurface[ 604 - gFont.LineWidth( stmp ) + 30696 ], stmp, 350, 640, gPalColorCubeRGB[0][31][0] );
    }
    if( gPipTermLines >= 3 ) gPipLineRow = 3;
    for( i = 0; i < 35; i++ ){
        s = MessageGetMessage( &gPipMsg, &gPipMsgLine, v6 + i );
        if( !strcmp( s, "**END-DISK**" ) ) break;
        if( !strcmp( s, "**END-PAR**") )
            gPipLineRow++;
        else
            PipPrintLine( s, 128, gPalColorCubeRGB[0][31][0] );
    }
    if( gPipButtons[ 0 ] <= gPipArchivesEnabled ){
        if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
        PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 201 ), 16, gPalColorCubeRGB[0][31][0] );
        if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
	PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 214 ), 32, gPalColorCubeRGB[0][31][0] );
    } else {
        if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
        PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 201 ), 16, gPalColorCubeRGB[0][31][0] );
        if( gPipTermLines >= 0 ) gPipLineRow = gPipTermLines;
	PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 200 ), 32, gPalColorCubeRGB[0][31][0] );
    }
    WinUpdate( gPipWin );
}

int PipTapesList( int Selected )
{
    int i, n;

    if( gPipTermLines >= 2 ) gPipLineRow = 2;    
    for( i = n = 0; i < gPipHoloTapesCount; i++ ){
        if( gGVals[ gPipHoloTapes[ i ].a ] == 0 ) continue;
        PipPrintLine( 
    	    MessageGetMessage( &gPipMsg, &gPipMsgLine, gPipHoloTapes[ i ].b ), 0x04, 
    	    ( (gPipLineRow - 2) >> 1 == Selected ) ? gPalColorCubeRGB[31][31][11] :  gPalColorCubeRGB[0][31][0]
        );
        n++;
        gPipLineRow++;
    }
    if( n ){
        if( gPipTermLines >= 0 ) gPipLineRow = 0;
        PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 211 ), 0x28, gPalColorCubeRGB[0][31][0] );
    }
    return n;
}

int PipLocationSortCb( Pip_t *p1, Pip_t *p2 )
{
    return strcmp( p1->Name, p2->Name );
}

void PipAutomap( int sel )
{
    if( sel == 1024 ){
        PipDeleteLineRegions();
        ScrCopy( gPipImgs[ 3 ] + 29694, 374, 410, 640, gPipSurface + 29694, 640 );
        if( gPipTermLines >= 0 )gPipLineRow = 0;
        PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 205 ), 10, gPalColorCubeRGB[0][31][0] );
        gpipAutomapLocations = PipLocationList( -1 );
        PipMakeLineRegions( 2, gpipAutomapLocations, 0 );
        WinAreaUpdate( gPipWin, &gPipRegion );
        gPipUnk40 = 0;
        return;
    }
    if( gPipUnk40 ){
        if( sel == 1025 || sel <= -1 ){
            PipAutomap( 1024 );
            GSoundPlay( "ib1p1xx1" );
        }
        if( sel <= (gpipAutomapLocations + 3) && sel >= 1 ){
            GSoundPlay( "ib1p1xx1" );
            PipGetLocations( sel );
            AutomapRenderMinimap( gPipWin, gPipLocNames[ sel - 1 ].MapId, gPipLocNames[ sel - 1 ].MapLvl );
            WinAreaUpdate( gPipWin, &gPipRegion );
        }
        return;
    }
    if( sel <= gpipAutomapLocations && sel > 0 ){
        GSoundPlay( "ib1p1xx1" );
        PipDeleteLineRegions();
        PipLocationList( sel );
        WinAreaUpdate( gPipWin, &gPipRegion );
        gPipMapId = gPipLocNames[ sel - 1 ].MapLvl;
        gpipAutomapLocations = PipGetLocations( 1 );
        PipMakeLineRegions( 0, gpipAutomapLocations + 2, 1 );
        AutomapRenderMinimap( gPipWin, gPipLocNames[ 0 ].MapId, gPipLocNames[ 0 ].MapLvl );
        WinAreaUpdate( gPipWin, &gPipRegion );
        gPipUnk40 = 1;
    }
}

int PipGetLocations( int sel )
{
    int i, n, Id, lvl;
    Automap_t *automap;

    if( AutomapLoadDB( &automap ) == -1 ) return -1;
    n = 0;
    for( lvl = 0; lvl < 3; lvl++ ){
        if( automap->tab[ 3*gPipMapId + lvl ] <= 0 ) continue;
        gPipLocNames[ n ].MapLvl = lvl;
        gPipLocNames[ n ].Name = MapCityLvlName( gPipMapId, lvl );
        gPipLocNames[ n ].MapId = gPipMapId;
        n++;
    }
    for( Id = 0; Id < WmGetMapCount(); Id++ ){
        if( Id == gPipMapId ) continue;
        if( MapAreaCmpByEntId( gPipMapId, Id ) == -1 ) continue;
        for( lvl = 0; lvl < 3; lvl++ ){
            if( automap->tab[ 3*Id + lvl ] <= 0 ) continue;
            gPipLocNames[ n ].MapLvl = lvl;
            gPipLocNames[ n ].Name = MapCityLvlName( Id, lvl );
            gPipLocNames[ n ].MapId = Id;
            n++;
        }                    
    }    
    ScrCopy( gPipImgs[ 3 ] + 640*46 + 254, 374, 410, 640, gPipSurface + 640*46 + 254, 640 );
    // title
    if( gPipTermLines >= 0 ) gPipLineRow = 0;
    PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 205 ), 10, gPalColorCubeRGB[0][31][0] );
    // city name
    if( gPipTermLines >= 2 ) gPipLineRow = 2;
    PipPrintLine( MapGetCurrentCityName( gPipMapId ), 2, gPalColorCubeRGB[0][31][0] );
    // city locations
    if( gPipTermLines >= 4 ) gPipLineRow = 4;
    sel = 2 * (sel - 1);
    for( i = 0; i < n; i++, gPipLineRow++ ){
        PipPrintLine( gPipLocNames[ i ].Name, 0, ( gPipLineRow - 4 == sel ) ? gPalColorCubeRGB[31][31][11] : gPalColorCubeRGB[0][31][0] );
    }
    PipBaclLink( gPalColorCubeRGB[0][31][0] );
    return n;    
}

int PipLocationList( int a1 )
{
    int n,lvl,i,j;
    Automap_t *automap;

    if( AutomapLoadDB( &automap ) == -1 ) return -1;
    n = 0;        
    for( j = 0; j < WmGetMapCount(); j++ ){
        for( lvl = 0; lvl < 3; lvl++ ){
            if( automap->tab[ 3*j + lvl ] > 0 && !AutomapUnk04( j ) ) break;
        }
        if( lvl >= 3 ) continue;
        for( i = 0; i < n; i++ ){
            if( MapAreaCmpByMapId( j, gPipLocNames[ i ].MapLvl ) ) break;
        }
        if( i == n ){
            gPipLocNames[ n ].MapLvl = j;
            gPipLocNames[ n ].Name = MapGetCityName( j );
            n++;
        }

    }
    if( !n ) return 0;
    if( n > 1 ) qsort( &gPipLocNames, n, sizeof( Pip_t ), (void *)PipLocationSortCb );
    ScrCopy( gPipImgs[ 3 ] + 29694, 374, 410, 640, gPipSurface + 29694, 640 );
    if( gPipTermLines >= 0 ) gPipLineRow = 0;
    PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 205 ), 10, gPalColorCubeRGB[0][31][0] );
    if( gPipTermLines >= 2 ) gPipLineRow = 2;            
    for( i = 0; i < n; i++ ){
        PipPrintLine( gPipLocNames[ i ].Name, 0, ( gPipLineRow - 1 == a1 ) ? gPalColorCubeRGB[31][31][11] : gPalColorCubeRGB[0][31][0] );
        gPipLineRow++;
    }
    return n;
}

void PipArchives( int Selected )
{
    int i;

    if( Selected == 1024 ){
        PipDeleteLineRegions();
        gPipArchivesEnabled = PipArchiveList( -1 );
        PipMakeLineRegions( 2, gPipArchivesEnabled, 0 );
        return;
    }
    if( Selected > gPipArchivesEnabled || Selected < 0 ) return;    
    GSoundPlay( "ib1p1xx1" );
    PipArchiveList( Selected );
    for( i = 2; i < 16; i++ ){ // skip two first movies, and the last one ( InterPlay Logo, Intro, Credits )
        if( GMovieEnabled( i ) && (--Selected <= 0) ) break;
    }    
    if( i <= 17 )
        GMoviePlay( i, 11 );
    else
        eprintf( "\n ** Selected movie not found in list! **\n" );

    FontSet( 101 );
    gPipTime = TimerGetSysTime();
    PipArchiveList( -1 );
}

int PipArchiveList( int sel )
{
    int i, n;

    ScrCopy( gPipImgs[ 3 ] + 640*46 + 254, 374, 410, 640, gPipSurface + 640*46 + 254, 640 );
    if( gPipTermLines >= 0 ) gPipLineRow = 0;
    PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 206 ), 10, gPalColorCubeRGB[0][31][0] );
    if( gPipTermLines >= 2 ) gPipLineRow = 2;    
    sel--;    
    for( i = 2, n = 0; i < 16; i++ ){ // skip two first movies, and the last one ( InterPlay Logo, Intro, Credits )
        if( !GMovieEnabled( i ) ) continue;
        PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 500 + i ), 0, ( n == sel ) ? gPalColorCubeRGB[31][31][11] : gPalColorCubeRGB[0][31][0] );
	n++;
        gPipLineRow++;
    }
    WinAreaUpdate( gPipWin, &gPipRegion );
    return n;
}

void PipAlarmClock( int Select )
{
    int hours, minutes;

    if( Select == 1024 ){
        if( CritterUnk46() ){
            PipDeleteLineRegions();
            PipWaitList( 0 );
            PipMakeLineRegions( 5, gPipAlarms, 0 );
        } else {
            GSoundPlay( "iisxxxx1" );
            DlgBox( MessageGetMessage( &gPipMsg, &gPipMsgLine, 215 ), 0, 0, 192, 135, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 1 );
        }
        return;
    }
    if( Select < 4 && Select > 17 ) return;    
    GSoundPlay( "ib1p1xx1" );
    PipWaitList( Select - 3 );
    switch( Select - 4 ){
        case 0:  PipWaitGameTime( 0, 10, 0 ); break;
        case 1:  PipWaitGameTime( 0, 30, 0 ); break;
        case 2 ... 7: PipWaitGameTime( Select - 5, 0, 0 ); break;
        case 8:  PipGameTimeDiff( &hours, &minutes, 8 );  PipWaitGameTime( hours, minutes, 0 ); break;
        case 9:  PipGameTimeDiff( &hours, &minutes, 12 ); PipWaitGameTime( hours, minutes, 0 ); break;
        case 10: PipGameTimeDiff( &hours, &minutes, 18 ); PipWaitGameTime( hours, minutes, 0 ); break;
        case 11: PipGameTimeDiff( &hours, &minutes, 0 ); if( !PipWaitGameTime( hours, minutes, 0 ) ) PipPrintCounter( 0, 4, 155, 17 ); WinUpdate( gPipWin ); break;
        case 12: PipWaitGameTime( 12, 0, 0 ); break;
        case 13: PipWaitGameTime( 0, 0, 13 ); break; // rest until party is healed
    }
    GSoundPlay( "ib2lu1x1" );
    PipWaitList( 0 );
}

void PipWaitList( int Selected )
{
    int i, TxtId;

    ScrCopy( gPipImgs[ 3 ] + 29694, 374, 410, 640, gPipSurface + 29694, 640 ); // clear "terminal" screen
    if( gPipTermLines >= 0 ) gPipLineRow = 0;
    PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, 300 ), 10, gPalColorCubeRGB[0][31][0] ); // title
    if( gPipTermLines >= 5 ) gPipLineRow = 5;
    PipHpPoints();
    TxtId = 302;
    for( i = 1; i < gPipAlarms + 1; i++, gPipLineRow++ ){
        PipPrintLine( MessageGetMessage( &gPipMsg, &gPipMsgLine, TxtId++ ), 0, ( i == Selected ) ? gPalColorCubeRGB[31][31][11] : gPalColorCubeRGB[0][31][0] );
    }
    WinAreaUpdate( gPipWin, &gPipRegion );
}

void PipHpPoints()
{
    char stmp[ 100 ];

    ScrCopy( gPipImgs[ 3 ] + 42494, 350, 10, 640, gPipSurface + 42494, 640 );
    sprintf( stmp, "%s %d/%d", MessageGetMessage( &gPipMsg, &gPipMsgLine, 301 ), CritterGetHp( gObjDude ), FeatGetVal( gObjDude, 7 ) );
    gFont.Print( &gPipSurface[((350 - gFont.LineWidth( stmp )) / 2) + 42494], stmp + 4, 640, 640, gPalColorCubeRGB[0][31][0] );
}

void PipTerminalCls()
{
    PipDeleteLineRegions();
    ScrCopy( gPipImgs[3] + 29694, 374, 410, 640, gPipSurface + 29694, 640 );
    if( gPipTermLines >= 0 ) gPipLineRow = 0;
}

void PipMakeLineRegions( int Offset, int Viewed, int NavFlag )
{
    int h, y, i;

    FontSet( 101 );
    h = gFont.ChrHeight();
    gPipListBase = Offset;
    gPipListViewed = Viewed;
    y = h * Offset + 46;
    if( Viewed ){        
        for( i = Offset; gPipListViewed + gPipListBase > i && i < 22; y += 2 * h, i++ ){
            gPipButtons[ i + 1 ] = WinCreateButton( gPipWin, 254, y, 350, h, -1, -1, -1, 505 + i, 0, 0, 0, 32 );
        }
    }
    if( NavFlag == 1 ) gPipButtons[ 23 ] = WinCreateButton( gPipWin, 254, h * gPipTermLines + 46, 350, h, -1, -1, -1, 528, 0, 0, 0, 32 );
}

void PipDeleteLineRegions()
{
    int i;

    if( gPipListViewed ){        
        for( i = gPipListBase; i < gPipListViewed + gPipListBase; i++ ){
            WinImageRemove( gPipButtons[ i + 1 ] );
        }
    }
    if( gPipUnk12 ) WinImageRemove( gPipUnk46 );
    gPipListViewed = 0;
    gPipUnk12 = 0;
}

int PipWaitGameTime( int Hour, int Min, char heal )
{
    unsigned int EvT;
    double v4,v5,Alarm,v58, tres;
    int err,SysTime,Hp,HpPts,Wound,timea,time,Sec10,i;

    GmouseLoadCursor( 26 );
    err = 0;
    if( heal ){
        if( heal == 12 || heal == 13 ){
            Hp = CritterGetHp( gObjDude );
            HpPts = FeatGetVal( gObjDude, FEAT_HP );
            if( Hp == HpPts && (heal != 13 || !PartyIsWounded()) ){
		GmouseLoadCursor( 1 );
		return 0;
            }
            time = lround( (float)(HpPts - Hp) / FeatGetVal( gObjDude, FEAT_HEALING ) * 3.0);
            while( time && !err ){
                if( time <= 24 ){
                    err = PipWaitGameTime( time, 0, 0 );
                    time = 0;
                } else {
                    err = PipWaitGameTime( 24, 0, 0 );
                    time -= 24;
                }
            }
            Wound = FeatGetVal( gObjDude, FEAT_HP ) - CritterGetHp( gObjDude );
            if( heal == 13 ){
                Hp = PartyMostWound();
                if( Hp > Wound ) Wound = Hp;
            }
            while( !err && Wound ){
                Wound = FeatGetVal( gObjDude, FEAT_HP ) - CritterGetHp( gObjDude );
                if( heal == 13 ){
                    Hp = PartyMostWound();
                    if( Hp > Wound ) Wound = Hp;
                }
                err = PipWaitGameTime( 3, 0, 0 );
            }
        }
    } else {
        v4 = (double)(60 * Hour + Min);
        v5 = v4 * 0.0006944444444444445 * 3.5 + 0.25;
        v58 = (double)Min / v4 * v5;
        if( Min ){
            Sec10 = ScptGetGameDekaSeconds();            
            Alarm = v58 * 20.0;
            tres = Alarm * (Min * 600.0) + Sec10;
            for( i = 0; i < lround( Alarm ) && !err; i++ ){
                SysTime = TimerGetSysTime();
                timea = lround( i / tres );
                EvT = EvQeGetTime();
                if( timea >= EvT ){
                    ScptSetGameTime( EvT + 1 );                    
                    if( EvQeTrig() ){ err = 1;  eprintf( "PIPBOY: Returning from Queue trigger...\n" ); gPipExit = 1; break; }
                    if( gMenuEscape ) err = 1;
                }
                if( !err ){
                    ScptSetGameTime( timea );
                    if( InpUpdate() == KEY_ESC || gMenuEscape ) err = 1;
                    PipPrintCounter( ScptGetGameTime(), 4, 155, 17 );
                    PipPrintDate();
                    WinUpdate( gPipWin );
                    while( TimerCurrDiff( SysTime ) < 50 );
                }                
            }
        
            if( !err ){
                ScptSetGameTime( Sec10 + 600 * Min );
                if( PipAddHealth( Min ) ){
                    PartyHeal( 3 );
                    CritterGetHp( gObjDude );
                    FeatGetVal( gObjDude, FEAT_HP );
                }
            }
            PipPrintCounter( ScptGetGameTime(), 4, 155, 17 );
            PipPrintDate();
            PipHpPoints();
            WinUpdate( gPipWin );
        }
        if( Hour && !err ){
            Sec10 = ScptGetGameDekaSeconds();            
            Alarm = (v5 - v58) * 20.0;
            tres = Alarm * (Hour * 36000.0) + Sec10;
            for( i = 0; i < lround( Alarm ) && !err; i++ ){
                SysTime = TimerGetSysTime();
                if( InpUpdate() == KEY_ESC || gMenuEscape ) err = 1;
                time = lround( i / tres );
                EvT = EvQeGetTime();
                if( !err && time >= EvT ){
                    ScptSetGameTime( EvT + 1 );
                    if( EvQeTrig() ){ err = 1; eprintf( "PIPBOY: Returning from Queue trigger...\n" ); gPipExit = 1; break; }
                    if( gMenuEscape ) err = 1;
                }
                if( !err ){
                    ScptSetGameTime( time );
                    if( PipAddHealth( lround( (60 * Hour) / Alarm ) ) ){
                        PartyHeal( 3 );
                        CritterGetHp( gObjDude );
                        FeatGetVal( gObjDude, FEAT_HP );
                    }
                    PipPrintCounter( ScptGetGameTime(), 4, 155, 17 );
                    PipPrintDate();
                    PipHpPoints();
                    WinUpdate( gPipWin );
                    while( TimerCurrDiff( SysTime ) < 50 );
                }                
            }
            if( !err ) ScptSetGameTime( Sec10 + 36000 * Hour );
            PipPrintCounter( ScptGetGameTime(), 4, 155, 17 );
            PipPrintDate();
            PipHpPoints();
            WinUpdate( gPipWin );
        }
    }    
    if( ScptGetGameDekaSeconds() > EvQeGetTime() ){
        if( EvQeTrig() ){ err = 1; eprintf( "PIPBOY: Returning from Queue trigger...\n" ); gPipExit = 1; }
    }
    PipPrintCounter( ScptGetGameTime(), 4, 155, 17 );
    PipPrintDate();
    WinUpdate( gPipWin );
    GmouseLoadCursor(1);
    return err;
}

int PipAddHealth( int a1 )
{
    gPipUnk07 += a1;
    if( gPipUnk07 < 180 ) return 0;
    eprintf( "\n health added!\n" );
    gPipUnk07 = 0;
    return 1;
}

int PipHealed()
{
    PartyHeal( 3 );
    return CritterGetHp( gObjDude ) == FeatGetVal( gObjDude, FEAT_HP );
}

void PipGameTimeDiff( int *hours, int *minutes, int TillHour )
{
    int GameTime, tmp;

    GameTime = ScptGetGameTime();
    *hours = GameTime / 100;
    *minutes = GameTime % 100;
    if( TillHour != *hours || *minutes ){
        tmp = TillHour - *hours;
        *hours = tmp;
        if( tmp < 0 ){
            *hours = tmp + 24;
            if( *minutes ){
                *hours = tmp + 23;
                *minutes = 60 - *minutes;
            }
        } else if( *minutes ){
            *hours = tmp - 1;
            *minutes = 60 - *minutes;
            if( *hours < 0 ) *hours = 23;
        }
    } else {
        *hours = 24;
    }
}

int PipScreenSaver()
{
    PipSaver_t Bombs[ 17 ];
    char *SaveScreen;
    unsigned int time;
    int i,a,h,w,xpos,sx,RunFlg,sy,c,n,ypos;
    float b;

    RunFlg = 1;
    n = 50;
    MseGetCursorPosition( &gPipMseX0, &gPipMseY0 );
    for( i = 0; i < 16; i++) Bombs[ i ].Show = 0;
    GmouseSetIfaceMode( 0 );
    if( !( SaveScreen = Malloc( 154364 ) ) ) return -1;
    ScrCopy( gPipSurface + 640*46 + 254, 374, 410, 640, SaveScreen, 374 ); // store current screen
    ScrCopy( gPipImgs[ 3 ] + 640*46 + 254, 374, 410, 640, gPipSurface + 640*46 + 254, 640 ); // cls terminal area
    while( RunFlg ){
        time = TimerGetSysTime();
	 // exit at key stroke or mouse movement
        MseGetCursorPosition( &gPipMseX1, &gPipMseY1 );
        if( InpUpdate() != -1 || gPipMseX0 != gPipMseX1 || gPipMseY0 != gPipMseY1 ) RunFlg = 0;
	// create new Bomb
        if( FLOAT( RandMinMax( 0, 32767 ) ) < 3047.3311 ){            
            for( i = 0; i < 16 && Bombs[ i ].Show; i++ );
            if( i < 16 ){
        	a = gPipGeo[ 10 ].Height / 4;
		b = 350 - gPipGeo[ 10 ].Width / 4 + 406 - gPipGeo[ 10 ].Height / 4;
                c = lround( RandMinMax( 0, 0x7FFF ) * 0.000030518509 * b );
                if( 410 - a >= c ){
                    Bombs[ i ].xpos = 602;
                    Bombs[ i ].ypos = 48 + c;
                } else {
                    Bombs[ i ].xpos = c - (410 - a) + 254 + gPipGeo[ 10 ].Width / 4;
                    Bombs[ i ].ypos = 46 - gPipGeo[ 10 ].Height + 2;
                }
                Bombs[ i ].Show = 1;
                Bombs[ i ].Move = 0.0;
                Bombs[ i ].Speed = RandMinMax( 0, 0x7FFF ) * 0.00008392590105899228 + 0.15;
            }
        }
        if( n == 0 ) ScrCopy( gPipImgs[ 3 ] + 29694, 374, 410, 640, gPipSurface + 29694, 640 ); 
        for( i = 0; i < 16; i++ ){
            if( Bombs[ i ].Show != 1 ) continue;
            h = gPipGeo[ 10 ].Height;
            w = gPipGeo[ 10 ].Width;            
            xpos = Bombs[ i ].xpos;
            ypos = Bombs[ i ].ypos;
            sy = sx = 0;
	    // clip horizontal            
            if( xpos >= 254 ){
                if( xpos + gPipGeo[ 10 ].Width >= 604 ){
                    w = 604 - xpos; // cut
                    if( w < 1 ) Bombs[ i ].Show = 0; // out of screen RIGHT clip
                }
            } else {
                sx = 254 - xpos;
                if( sx >= gPipGeo[ 10 ].Width ) Bombs[ i ].Show = 0; // out of screen LEFT clip
                xpos = 254;
                w = gPipGeo[ 10 ].Width - sx;
            }
            // clip vertical
            if( Bombs[ i ].ypos >= 46 ){
                if( gPipGeo[ 10 ].Height + Bombs[ i ].ypos >= 452 ){
                    h = 452 - Bombs[ i ].ypos;
                    if( h < 1 ) Bombs[ i ].Show = 0; // out of screen BOTTOM clip
                }
            } else {
                sy = 46 - Bombs[ i ].ypos;
                if( gPipGeo[ 10 ].Height + Bombs[ i ].ypos < 46 ) Bombs[ i ].Show = 0;  // out of screen UP clip
                ypos = 46;
                h = gPipGeo[ 10 ].Height - sy;
            }
	    // display
            if( Bombs[ i ].Show == 1 && !n ) ScrCopyAlpha( &gPipImgs[ 10 ][ gPipGeo[ 10 ].Width * sy + sx ], w, h, gPipGeo[ 10 ].Width, &gPipSurface[ 640 * ypos + xpos ], 640 );
            Bombs[ i ].Move += Bombs[ i ].Speed; // move speed
            if( Bombs[ i ].Move >= 1.0 ){
                Bombs[ i ].xpos = lround( Bombs[ i ].xpos - Bombs[ i ].Move );
                Bombs[ i ].ypos = lround( Bombs[ i ].ypos + Bombs[ i ].Move );
                Bombs[ i ].Move = 0.0; // reset speed counter
            }            
        }
        if( n ){
            n--;
        } else {
            WinAreaUpdate( gPipWin, &gPipRegion );
            while( TimerCurrDiff( time ) < 50 );
        }
    }
    ScrCopy( SaveScreen, 374, 410, 374, gPipSurface + 29694, 640 );
    Free( SaveScreen );
    WinAreaUpdate( gPipWin, &gPipRegion );
    GmouseSetIsoMode();
    return 0;
}

int PipQuestsLoad()
{
    PipQuests_t *p;
    xFile_t *fh;
    char *s, *tmp, Buff[256], delim[4];
    int a, b, c, d, e, err;

    err = 0;
    strcpy( delim, " \t," );
    if( gPipQuestList ){
        Free( gPipQuestList );
        gPipQuestList = NULL;
    }
    gPipQuestCount = 0;
    MessageClose( &gPipUnk28 );
    if( MessageInit( &gPipUnk28 ) != 1 ) return -1;
    if( MessageLoad( &gPipUnk28, "game/quests.msg" ) != 1 ) return -1;
    if( !(fh = dbOpen( "data/quests.txt", "rt" ) ) ) return -1;
    while( dbgets( Buff, 256, fh ) ){
        for( s = Buff; IsTable[ *s + 1 ] & 0x02; s++ );
        if( *s == '#' ) continue;
        if( !(tmp = strtok( s, delim ) ) ) continue;
        a = strtol( tmp, 0, 10 );
        if( !(tmp = strtok( 0, delim ) ) ) continue;
        b = strtol( tmp, 0, 10 );
        if( !(tmp = strtok( 0, delim ) ) ) continue;
        c = strtol( tmp, 0, 10 );
        if( !( tmp = strtok( 0, delim ) ) ) continue;
        d = strtol( tmp, 0, 10 );
        if( !( tmp = strtok( 0, delim ) ) ) continue;
        e = strtol( tmp, 0, 10 );
        if( !(p = Realloc( gPipQuestList, sizeof( PipQuests_t ) * (gPipQuestCount + 1))) ){ err = -1; break; }
        gPipQuestList = p;
        p += gPipQuestCount;
        p->a = a;
        p->b = b;
        p->c = c;
        p->d = d;
        p->e = e;
        gPipQuestCount++;
    }
    if( !err ) qsort( gPipQuestList, gPipQuestCount, sizeof( PipQuests_t ), (void *)PipQuestSortCb );
    dbClose( fh );
    return err;
}

void PipDeleteQuestList()
{
    if( gPipQuestList ){
        Free( gPipQuestList );
        gPipQuestList = NULL;
    }
    gPipQuestCount = 0;
    MessageClose(&gPipUnk28);
}

static int PipQuestSortCb( PipQuests_t *q1, PipQuests_t *q2 )
{
    return q1->a - q2->a;
}

int PipHoloTapesLoad()
{
    PipHoloDisk_t *p;
    xFile_t *fh;
    int err, a, b, c;
    char *s, *tmp, Buf[ 256 ], delim[ 4 ];

    err = 0;
    strcpy( delim, " \t," );
    if( gPipHoloTapes ){
        Free(gPipHoloTapes);
        gPipHoloTapes = NULL;
    }
    gPipHoloTapesCount = 0;    
    if( !(fh = dbOpen( "data/holodisk.txt", "rt" ) ) ) return -1;
    while( dbgets( Buf, 256, fh ) ){
        for( s = Buf; IsTable[ ( *s + 1 ) ] & 0x02; s++ );
        if( *s == '#' ) continue;
        if( !( tmp = strtok( s, delim ) ) ) continue;            
        a = strtol( tmp, 0, 10 );
        if( !( tmp = strtok( 0, delim ) ) ) continue;                
        b = strtol( tmp, 0, 10 );
        if( !( tmp = strtok( 0, delim ) ) ) continue;
        c = strtol( tmp, 0, 10 );
        if( !(p = Realloc( gPipHoloTapes, sizeof( PipHoloDisk_t ) * (gPipHoloTapesCount + 1))) ){ err = -1; break; }
        p[ gPipHoloTapesCount ].a = a;
        p[ gPipHoloTapesCount ].b = b;
        p[ gPipHoloTapesCount ].c = c;
        gPipHoloTapes = p;
        gPipHoloTapesCount++;
    }
    dbClose( fh );
    return err;
}

void PipDeleteHoloList()
{
    if( gPipHoloTapes ){
        Free( gPipHoloTapes );
        gPipHoloTapes = NULL;
    }
    gPipHoloTapesCount = 0;
}

