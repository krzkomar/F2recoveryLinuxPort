#include <SDL.h>
#include "FrameWork.h"

int gMainUnk03;
int gMainUnk05 = 0;
char **gMainFileList = NULL;
int gMainUnk06 = 0;
int gMainUnk07 = 0;
int gMainUnk08 = 0;
int gMainUnk01 = 0;

static void MainUnk07();
extern void *gCharEditKarma;

void MainGameVersion( char *str )
{
    sprintf( str, "FALLOUT II %d.%02d ", 1, 2 );
}

void MainGame( int argc, char **argv )
{
    char stmp[16];
    int ExitFlg, MenuSelection, lang, Window, zm;

    if( MainInit( argc, argv ) != 1 ) return;
//GMoviePlay( 2, 4 );
//    GMoviePlay( 0, MOVIE_FADE_IN );
//    GMoviePlay( 1, 0  );
//    GMoviePlay( 16, 0 );
//return;
    if( !MainMenuCreate() ){
        zm = 1;
        ExitFlg = 0;
        do{
    	    KeyFlush();
            GSoundSetBg( "07desert", 11 );
            MainMenuFadeInit( 1 );
            MseDrawCursor();
            MenuSelection = MainMenuHandle();
            MseCursorRedraw();
            CfgGetInteger( &gConfiguration, "preferences", "language_filter", &lang );
            switch( MenuSelection ){
                case -1:
                case 7: // EXIT
            	    ExitFlg = 1;
                    MainMenuUpdate( 1 );
                    MainMenuDestroy();
                    GSoundBgClose();
            	    break;
                case 0:  // INTRO
                    MainMenuUpdate( 1 );
                    GMoviePlay( 1, 8 );
                    GMoviePlay( 16, 0 );
                    break;
                case 1: // NEW GAME
                    MainMenuUpdate(1);
                    MainMenuDestroy();
                    if( NgNewGame() == 2 ){
//                        GMoviePlay( 2, 4 );
                        RandGetA( -1 );
                        strcpy( stmp, "artemple.map" );
                        MainGameCreate( stmp );
                        MainGameLoop();
                        FadeStep( gFadePaletteB );
                        ObjUnk33( gObjDude, 0 );
                        MapExitMsg();
                        GameReset();
                        if( gMainUnk08 ){
                            MainUnk05();
                            gMainUnk08 = 0;
                        }
                    }
                    MainMenuCreate();
                    break;
                case 2: // Load Game
                    Window = WinCreateWindow(0, 0, 640, 480, gPalColorCubeRGB[0][0][0], 20);
                    MainMenuUpdate( 1 );
                    MainMenuDestroy();
                    GSoundBgClose();
                    gMenuEscape = 0;
                    gObjDude->Flags &= ~0x08;
                    gMainUnk08 = 0;
                    ObjUnk32( gObjDude, 0 );
                    MseCursorRedraw();
                    MapMsgInit();
                    GmouseLoadCursor( 0 );
                    MseDrawCursor();
                    PalLoadFromFile( "color.pal" );
                    FadeStep( gPalBase );
		    switch( LsgMenuGameLoad( 0 ) ){
			case 0: break;
			case -1: ErrorPrintf("\n ** Error running LoadGame()! **\n"); break;
			default: 
                    	    WinClose( Window );
			    Window = -1;
                    	    MainGameLoop();
                    	    break;
		    }		    
                    FadeStep( gFadePaletteB );
                    if( Window != -1 ) WinClose( Window );
                    ObjUnk33( gObjDude, 0 );
                    MapExitMsg();
                    GameReset();
                    if( gMainUnk08 ){
                        MainUnk05();
                        gMainUnk08 = 0;
                    }
                    MainMenuCreate();
                    break;
                case 4:
                    ErrorPrintf("Main menu timed-out\n");
                case 3:
                    MainUnk04();
                    break;
                case 5: // Credits
                    MainMenuUpdate( 1 );
//                    Credits( "credits.txt", -1, 1 );
                    break;
                case 6:
                    if( !zm ){
                	MainMenuUpdate( 1 );
                        Credits( "quotes.txt", -1, 1 );
                    }
                    break;
                case 8:
                    MainSelectRecordMap();
                    break;
                case 9:                 // Option Menu
                    MainMenuUpdate( 0 );
                    MseDrawCursor();
                    OptMenu( 'p' );
                    GmouseLoadCursor( 1 );
                    MseDrawCursor();
                    MainMenuFadeInit( 0 );
                    break;
                default:
                    break;
            }
        } while( !ExitFlg );
    }
    GSoundBgClose();
//    GameCleanFiles();
    GameExit();
}

int MainInit( int argc, char **argv )
{
    if( GameSysInit( GAME_NAME, 0, 0, 0, argc, argv ) == -1 ) return 0;
    if( gMainFileList ) MainFilesClose();
    if( !SdfOpen( &gMainFileList, &gMainUnk06) ) gMainUnk07 = 0;
    return 1;
}

int MainReset()
{
    GameReset();
    return 1;
}

void MainKill()
{
    GSoundBgClose();
    if( gMainFileList ) SdfClose( gMainFileList );
    gMainFileList = NULL;
    gMainUnk06 = 0;
    gMainUnk07 = 0;
    GameExit();
}

int MainGameCreate( char *MapName )
{
    int win;

    gMenuEscape = 0;
    gMainUnk08 = 0;
    gObjDude->Flags &= ~0x08;
    ObjUnk32( gObjDude, 0 );
    MseCursorRedraw();
    win = WinCreateWindow(0, 0, 640, 480, gPalColorCubeRGB[0][0][0], 20);
    WinUpdate( win );
    PalLoadFromFile( "color.pal" );
    FadeStep( gPalBase );
    MapMsgInit();
    GmouseLoadCursor( 0 );
    MseDrawCursor();
    MapLoadMAP( MapName );
    WmStartMapMusic();
    FadeStep( gFadePaletteB );
    WinClose( win );
    PalLoadFromFile( "color.pal" );
    FadeStep( gPalBase );
    return 0;
}

int MainUnk01()
{
    gMenuEscape = 0;
    gMainUnk08 = 0;
    gObjDude->Flags &= ~0x08;
    ObjUnk32( gObjDude, 0 );
    MseCursorRedraw();
    MapMsgInit();
    GmouseLoadCursor( 0 );
    MseDrawCursor();
    return 0;
}

int MainUnk02()
{
    ObjUnk33( gObjDude, 0 );
    return MapExitMsg();
}

void MainGameLoop()
{
    int IsCursorClear, sel, crs;

    IsCursorClear = MseIsCursorClear();
    crs = (IsCursorClear == 0);
    if( IsCursorClear ) MseDrawCursor();
    gMainUnk05 = 0;
    ScptEnable();
    while( !gMenuEscape ){
        sel = InpUpdate();
        GameProcess( sel, 0 );
//        ScptProcess();
        MapJump();
        if( gMainUnk05 ) gMainUnk05 = 0;
        if( gObjDude->Critter.State.CombatResult & 0x81 ){
            EndGameSetupDeathEnding(0);
            gMainUnk08 = 1;
            gMenuEscape = 2;
        }
    }
    ScptDisable();
    if( !crs ) MseCursorRedraw();
}

int MainUnk03()
{
    if( gMainFileList ){
        SdfClose( gMainFileList );
        gMainUnk06 = 0;
        gMainUnk07 = 0;
        gMainFileList = 0;
    }
    if( SdfOpen( &gMainFileList, &gMainUnk06 ) ) return 0;
    gMainUnk07 = 0;
    return 1;
}

void MainFilesClose()
{
    if( gMainFileList ) SdfClose( gMainFileList );
    gMainUnk06 = 0;
    gMainUnk07 = 0;
    gMainFileList = 0;
}

void MainSelectRecordMap()
{
    VidRect_t Area;
    int cnt, err, MapNo;
    Sdf_t sdf;
    char stmp[16], **FileList;

    cnt = dbGetFileList( "maps/*.map", &FileList );
    err = 0;
    if( cnt ){
        MapNo = TextBoxFileSelect( "Select Map", FileList, cnt, NULL, 80, 80, 0x10104 );
        if( MapNo != -1 ){
            stmp[0] = 0;
            if( !TextBoxDialogEdit1( stmp, 11, "Enter name for recording (8 characters max, no extension):", 100, 100 ) ){
                memset( &sdf, 0x00, sizeof( Sdf_t ) );
                if( !SdfUnk03( stmp, FileList[ MapNo ], &sdf ) ) err = 1;
            }
        }
        dbDelFileList( FileList );
    }
    if( !err ) return;
    MainMenuUpdate( 1 );
    MainMenuDestroy();
    GSoundBgClose();
    RandGetA( 0xBEEFFEED );
    GameReset();
    ProtoDudeInit( "premade/combat.gcd" );
    MainGameCreate( sdf.sdfname );
    SdfUnk04( &sdf );
    FadeStep( gFadePaletteB );
    ObjUnk33( gObjDude, &Area );
    MapExitMsg();
    GameReset();
    MainMenuCreate();
    if( gMainFileList ) MainFilesClose();
    if( !SdfOpen( &gMainFileList, &gMainUnk06 ) ) gMainUnk07 = 0;
}

void MainUnk04()
{
    VidRect_t Area;
    Sdf_t sdf;

    if( gMainUnk01 || gMainUnk06 <= 0 ){
        MainMenuUpdate( 1 );
        GMoviePlay( 1, 8 );
    } else {
        if( !SdfUnk01( gMainFileList[ gMainUnk07 ], &sdf ) ){
            MainMenuUpdate( 1 );
            MainMenuDestroy();
            GSoundBgClose();
            RandGetA( 0xBEEFFEED );
            GameReset();
            ProtoDudeInit( "premade/combat.gcd" );
            MainGameCreate( sdf.sdfname );
            SdfUnk02( &sdf );
            FadeStep( gFadePaletteB );
            ObjUnk33( gObjDude, &Area );
            MapExitMsg();
            GameReset();
            MainMenuCreate();
        }
        if( ++gMainUnk07 >= gMainUnk06 ) gMainUnk07 = 0;
    }
    gMainUnk01 = 1 - gMainUnk01;
}

void MainUnk05()
{
    unsigned int v19, v20;
    short v23[64], tmp;
    CachePool_t *Obj;
    int IsCursorClear,Window,i,sel,subs,clb,WinId;
    char *Surface,*v5,*scrn,*bmp,v22[512];

    subs = 0;
    ArtExpandCache();
    CycleColorStop();
    GmouseLoadCursor( 0 );
    IsCursorClear = MseIsCursorClear();
    clb = (IsCursorClear == 0);
    if( IsCursorClear ) MseDrawCursor();
    Window = WinCreateWindow( 0, 0, 640, 480, 0, 4 );
    WinId = Window;
    if( Window != -1 ){
        Surface = WinGetSurface(Window);
        if( Surface ){
            if( (bmp = ArtGetBitmap( ArtMakeId( 6, 309, 0, 0, 0 ), 0, 0, &Obj ) ) ){
                while( MseGetButtons() ) InpUpdate();
                KeyFlush();
                InpFlushBuffer();
                ScrCopy( bmp, 640, 480, 640, Surface, 640 );
                ArtClose( Obj );
                v5 = EndGameSetNarratorFile();
                CfgGetInteger( &gConfiguration, "preferences", "subtitles", &subs );
                if( subs ){
                    if( !MainUnk08( v5, v22 ) ){
                        tmp = 0;
                        eprintf( "\n((ShowDeath)): %s\n", v22 );
                        if( !MainUnk06( v22, 560, v23, &tmp ) ){
                            scrn = &Surface[ 640 * ( 480 - tmp * gFont.ChrHeight() ) - 5120 ];
                            ScrFillSolid( scrn - 602, 564, tmp * gFont.ChrHeight() + 2, 640, 0 );
                            scrn += 40;
                            for( i = 0; i < tmp; i++, scrn += 640 * gFont.ChrHeight() ){
                                gFont.Print( scrn, &v22[ v23[ i ] ], 560, 640, gPalColorCubeRGB[31][31][31] );                                
                            }
                        }
                    }
                }
                WinUpdate( WinId );
                PalLoadFromFile( "art/intrface/death.pal" );
                FadeStep( gPalBase );
                gMainUnk03 = 0;
                GSoundUnk33( (void *)MainUnk07 );
                v19 = ( GSoundLoadSpk( v5, 10, 15, 14 ) == -1 ) ? 3000 : -1;
                GSoundSpkDelete();
                v20 = TimerGetSysTime();
                do
                    sel = InpUpdate();
                while( sel == -1 && !gMainUnk03 && TimerCurrDiff( v20 ) < v19 );
                GSoundUnk33( 0 );
                GSoundSpkCancel();
                while( MseGetButtons() ) InpUpdate();
                if( sel == -1 ) TimerWaitProc( 500 );
                FadeStep( gFadePaletteC );
                PalLoadFromFile( "color.pal" );
            }
        }
        WinClose( WinId );
    }
    if( !clb ) MseCursorRedraw();
    GmouseLoadCursor( 1 );
    CycleColorStart();
}

static void MainUnk07()
{
    gMainUnk03 = 1;
}

int MainUnk08( char *a1, char *path )
{
    char *s, *lang, stmp[ 260 ];
    xFile_t *fh;
    int c;

    lang = NULL;    
    if( !( s = strchr( a1, '/' ) ) ) return -1;
    if( CfgGetString( &gConfiguration, "system", "language", &lang ) != 1 ){
        eprintf( "MAIN: Error grabing language for ending. Defaulting to english.\n" );
        lang = "english";
    }
    sprintf( stmp, "text/%s/cuts/%s%s", lang, s + 1, ".TXT" );
    s = path;
    if( !( fh = dbOpen( stmp, "rt" ) ) ) return -1;
    while( (c = dbgetc( fh ) ) != -1 ){
        if( c == '\n' ) c = ' ';
        *s++ = c;
    }
    dbClose( fh );
    *s = '\0';
    return 0;
}

int MainUnk06( char *str, int LineWidth, short *pOffs, short *pLines )
{
    char *s;
    int i;
    short *offs;

    while( 1 ){
        s = strchr( str, ':' );
        if( !s || ((s - 1) < str) ) break;
        s[ 0 ] = ' ';
        s[ -1 ] = ' ';
    }
    if( WinTextWrap( str, LineWidth, pOffs, pLines ) == -1 ) return -1;
    offs = pOffs + 1;
    for( i = 1; i < (*pLines - 1); i++, offs++ ){
        for( s = str + *offs; *s != ' '; --*offs ){
            if( --s < str ) break;
        }
        if( s ){
            *s = '\0';
            ++*offs;
        }        
    }
    return 0;    
}

int main( int argc, char **argv )
{
    TimerThrTimeInit();
    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER );
    SDL_ShowCursor( SDL_DISABLE );
    gSDLReady = 1;
    MainGame( argc, argv );    
    CfgFree( &gConfiguration );
    SDL_ShowCursor( SDL_ENABLE );
    SDL_Quit();
    MemStat();
    printf( "Bye\n" );
    return 0;
}

