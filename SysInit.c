#include "FrameWork.h"

//int gMapperEnable = 0;
//int GameSaveBmp( int width, int height, char *palette, char *pixels );
//int gSysMapperEnable = 0;


char *gGamePath = "game/";
void MouseMgrResetSpeed();
void FadeInit();
int SysOpenDataBase();
int SysOpenDataFiles();
void SysLoadConfiguration( int MapperFlag, int argc, char **argv );
void SysUnloadConfiguration( int SaveFlag );
int gSysMasterDataFile;
int gSysCritterDataFile;
int gCfgInit = 0;
Config_t gConfiguration;
char gCfgPath[ 264 ];


/*******************************************************/

void SysDbl2Str( char *str, double dbl )
{
    char stmp[ 20 ];

    sprintf( stmp, "%f", dbl );
    IfcMsgOut( stmp );
}


int SysSaveBmp( int width, int height, char *pix, char *pal )
{
    MsgLine_t Line;

    if( InpSaveBMP( width, height, pix, pal ) ){
        Line.Id = 8;
        if( MessageGetMsg( &gMessage, &Line ) == 1 ) IfcMsgOut( Line.Text );
        return -1;
    } else {
        Line.Id = 3;
        if( MessageGetMsg( &gMessage, &Line ) == 1 ) IfcMsgOut( Line.Text );
        return 0;
    }
}

void SysGlobVarFree()
{
    gGValCount = 0;
    if( gGVals ){
        Free( gGVals );
        gGVals = NULL;
    }
}

void SysHelpDialog()
{
    int Window, IsEnabled, v9;
    char *Surface, *p;
    CachePool_t *Block;

    v9 = MapAmbientDisable();
    GmouseUnk03();
    GmouseLoadCursor( 0 );
    IsEnabled = CycleColorIsEnabled();
    CycleColorStop();
    Window = WinCreateWindow( 0, 0, 640, 480, 0, 12 );
    if( Window != -1 ){
        Surface = WinGetSurface( Window );
        if( Surface ){
            if( (p = ArtGetBitmap( ArtMakeId( 6, 297, 0, 0, 0 ), 0, 0, &Block )) ){
                FadeSetPalette( gFadePaletteC );
                ScrCopy( p, 640, 480, 640, Surface, 640 );
                ArtClose( Block );
                WinMoveTop( Window );
                PalLoadFromFile( "art/intrface/helpscrn.pal" );
                FadeSetPalette( gPalBase );
                while( InpUpdate() == -1 && !gMenuEscape );
                while( MseGetButtons() ) InpUpdate();
                FadeSetPalette(gFadePaletteC);
            }
        }
        WinClose( Window );
        PalLoadFromFile( "color.pal" );
        FadeSetPalette( gPalBase );
    }
    if( IsEnabled ) CycleColorStart();
    GmouseIsoEnter();
    if( v9 ) MapAmbientEnable();
}

void SysQuitDlg()
{
    int k, e, IsCursorClear, CursorId, CursorUpdate;
    MsgLine_t msg;

    if( (k = MapAmbientDisable()) )
        e = GmouseUnk58();
    else
        e = 0;
    if( e ) GmouseUnk03();
    IsCursorClear = MseCursorHidden();
    CursorUpdate = IsCursorClear == 0;
    if( CursorUpdate ) MseCursorShow();
    CursorId = GmouseGetCursorId();
    GmouseLoadCursor( 1 );
    msg.Id = 0; // 'Are you sure you want to quit?'
    if( MessageGetMsg( &gMessage, &msg ) == 1 && 
	DlgBox( msg.Text, 0, 0, 169, 117, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 16 ) == 1 
      ) gMenuEscape = 2;
    GmouseLoadCursor( CursorId );
    if( !CursorUpdate ) MseCursorHide();
    if( e ) GmouseIsoEnter();
    if( k ) MapAmbientEnable();
}

int SysOpenDataBase()
{
    int Data, i, hashing;
    char fname[260], *fpatchesname, *fdataname;

    hashing = 0;
    fdataname = fpatchesname = NULL;
    CfgGetInteger( &gConfiguration, "system", "hashing", &hashing );
    CfgGetString( &gConfiguration, "system", "master_dat", &fdataname );
    if( !*fdataname )  fdataname = NULL;
    CfgGetString( &gConfiguration, "system", "master_patches", &fpatchesname );
    if( !*fpatchesname ) fpatchesname = NULL;
    gSysMasterDataFile = dbLoadData( fpatchesname, fdataname );
    if( gSysMasterDataFile == -1 ){
        WinMsgError( "Could not find the master datafile. Please make sure the FALLOUT CD is in the drive "
    		     "and that you are running FALLOUT from the directory you installed it to." );
        return -1;
    }
    CfgGetString( &gConfiguration, "system", "critter_dat", &fdataname );
    if( !*fdataname ) fdataname = NULL;
    CfgGetString( &gConfiguration, "system", "critter_patches", &fpatchesname );
    if( !*fpatchesname ) fpatchesname = NULL;
    gSysCritterDataFile = dbLoadData( fpatchesname, fdataname );
    if( gSysCritterDataFile == -1 ){
        WinMsgError( "Could not find the critter datafile. Please make sure the FALLOUT CD is in the drive "
    		     "and that you are running FALLOUT from the directory you installed it to." );
        return -1;
    }
    Data = -1;
    for( i = 0; i < 1000; i++ ){
        if( Data != -1 ) break;
        sprintf( fname, "patch%03d.dat", i );
        if( !access( fname, F_OK ) ) Data = dbLoadData( 0, fname );
    }
    return 0;
}

void SysSplash()
{
/*
    Pal8_t *pal;    
    xFile_t *fh;
    char stmp[64], fname[64], *Language, *art;
    int EntryValue, i;

    Language = NULL;
    EntryValue = 0;    
    CfgGetInteger( &gConfiguration, "system", "splash", &EntryValue );
    if( CfgGetString( &gConfiguration, "system", "language", &Language ) != 1 || !strcasecmp( Language, "english" ) )
        strcpy( stmp, "art/splash/" );
    else
        sprintf( stmp, "art/%s/splash/", Language );

    for( i = 0; i < 10; i++ ){
        sprintf( fname, "%ssplash%d.rix", stmp, EntryValue );
        if(( fh = dbOpen( fname, "rb" ) )) break;
        if( ++EntryValue >= 10 ) EntryValue = 0;
    }
    if( !fh ) return;    
    if( (pal = Malloc( 768 )) ){
        art = Malloc( 307200 );
        if( art ){
            FadeSetPalette( gFadePaletteC );
            dbseek( fh, 10, 0 );
            dbread( pal, 1, 768, fh );
            dbread( art, 1, 307200, fh );
            dbClose( fh );
            gVidCopyA( art, 640, 480, 0, 0, 640, 480, 0, 0 );
            FadeStep( pal );
            Free( art );
        }
        Free( pal );
    }
    CfgSetInteger( &gConfiguration, "system", "splash", EntryValue + 1 );
*/
}

void SysLoadConfiguration( int MapperFlag, int argc, char **argv )
{
    char *s;

    if( gCfgInit || !CfgInit( &gConfiguration ) ) return;    

    CfgSetString(&gConfiguration, "system", "executable", "game");
    CfgSetString(&gConfiguration, "system", "master_dat", "master.dat");
    CfgSetString(&gConfiguration, "system", "master_patches", "data");
    CfgSetString(&gConfiguration, "system", "critter_dat", "critter.dat");
    CfgSetString(&gConfiguration, "system", "critter_patches", "data");
    CfgSetString(&gConfiguration, "system", "language", "english");
    CfgSetInteger(&gConfiguration, "system", "scroll_lock", 0);
    CfgSetInteger(&gConfiguration, "system", "interrupt_walk", 1);
    CfgSetInteger(&gConfiguration, "system", "art_cache_size", 8);
    CfgSetInteger(&gConfiguration, "system", "color_cycling", 1);
    CfgSetInteger(&gConfiguration, "system", "hashing", 1);
    CfgSetInteger(&gConfiguration, "system", "splash", 0);
    CfgSetInteger(&gConfiguration, "system", "free_space", 20480);
    CfgSetInteger(&gConfiguration, "preferences", "game_difficulty", 1);
    CfgSetInteger(&gConfiguration, "preferences", "combat_difficulty", 1);
    CfgSetInteger(&gConfiguration, "preferences", "violence_level", 3);
    CfgSetInteger(&gConfiguration, "preferences", "target_highlight", 2);
    CfgSetInteger(&gConfiguration, "preferences", "item_highlight", 1);
    CfgSetInteger(&gConfiguration, "preferences", "combat_looks", 0);
    CfgSetInteger(&gConfiguration, "preferences", "combat_messages", 1);
    CfgSetInteger(&gConfiguration, "preferences", "combat_taunts", 1);
    CfgSetInteger(&gConfiguration, "preferences", "language_filter", 0);
    CfgSetInteger(&gConfiguration, "preferences", "running", 0);
    CfgSetInteger(&gConfiguration, "preferences", "subtitles", 0);
    CfgSetInteger(&gConfiguration, "preferences", "combat_speed", 0);
    CfgSetInteger(&gConfiguration, "preferences", "player_speedup", 0);
    CfgSetDouble(&gConfiguration, "preferences", "text_base_delay", 3.5);
    CfgSetDouble(&gConfiguration, "preferences", "text_line_delay", 1.399993896484375);
    CfgSetDouble(&gConfiguration, "preferences", "brightness", 1.0);
    CfgSetDouble(&gConfiguration, "preferences", "mouse_sensitivity", 1.0);
    CfgSetInteger(&gConfiguration, "sound", "initialize", 1);
    CfgSetInteger(&gConfiguration, "sound", "device", -1);
    CfgSetInteger(&gConfiguration, "sound", "port", -1);
    CfgSetInteger(&gConfiguration, "sound", "irq", -1);
    CfgSetInteger(&gConfiguration, "sound", "dma", -1);
    CfgSetInteger(&gConfiguration, "sound", "sounds", 1);
    CfgSetInteger(&gConfiguration, "sound", "music", 1);
    CfgSetInteger(&gConfiguration, "sound", "speech", 1);
    CfgSetInteger(&gConfiguration, "sound", "master_volume", 22281);
    CfgSetInteger(&gConfiguration, "sound", "music_volume", 22281);
    CfgSetInteger(&gConfiguration, "sound", "sndfx_volume", 22281);
    CfgSetInteger(&gConfiguration, "sound", "speech_volume", 22281);
    CfgSetInteger(&gConfiguration, "sound", "cache_size", 448);
    CfgSetString(&gConfiguration, "sound", "music_path1", "sound/music/");
    CfgSetString(&gConfiguration, "sound", "music_path2", "sound/music/");
    CfgSetString(&gConfiguration, "debug", "mode", "environment");
    CfgSetInteger(&gConfiguration, "debug", "show_tile_num", 0);
    CfgSetInteger(&gConfiguration, "debug", "show_script_messages", 0);
    CfgSetInteger(&gConfiguration, "debug", "show_load_info", 0);
    CfgSetInteger(&gConfiguration, "debug", "output_map_data_info", 0);
    if( MapperFlag == 1 ){
        CfgSetString(&gConfiguration, "system", "executable", "mapper");
        CfgSetInteger(&gConfiguration, "mapper", "override_librarian", 0);
        CfgSetInteger(&gConfiguration, "mapper", "librarian", 0);
        CfgSetInteger(&gConfiguration, "mapper", "use_art_not_protos", 0);
        CfgSetInteger(&gConfiguration, "mapper", "rebuild_protos", 0);
        CfgSetInteger(&gConfiguration, "mapper", "fix_map_objects", 0);
        CfgSetInteger(&gConfiguration, "mapper", "fix_map_inventory", 0);
        CfgSetInteger(&gConfiguration, "mapper", "ignore_rebuild_errors", 0);
        CfgSetInteger(&gConfiguration, "mapper", "show_pid_numbers", 0);
        CfgSetInteger(&gConfiguration, "mapper", "save_text_maps", 0);
        CfgSetInteger(&gConfiguration, "mapper", "run_mapper_as_game", 0);
        CfgSetInteger(&gConfiguration, "mapper", "default_f8_as_game", 1);
        CfgSetInteger(&gConfiguration, "mapper", "sort_script_list", 0);
    }    
    if( (s = strrchr(*argv, '/')) ) {
        *s = '\0';
        sprintf( gCfgPath, "%s/%s", *argv, "fallout2.cfg" );
        *s = '/';
    } else {
        strcpy( gCfgPath, "fallout2.cfg" );
    }
    CfgLoadFromFile( &gConfiguration, gCfgPath, 0 );
    CfgCmdLineParser( &gConfiguration, argc, argv );
    gCfgInit = 1;
}

void SysSaveConfiguration()
{
    if( gCfgInit ) CfgSaveFile( &gConfiguration, gCfgPath, 0 );
}

void SysUnloadConfiguration( int SaveFlag )
{
    if( !gCfgInit ) return;
    if( SaveFlag ) CfgSaveFile( &gConfiguration, gCfgPath, 0 );
    CfgFree( &gConfiguration );
    gCfgInit = 0;    
}

void SysError( const char *str, int a2 )
{
/*
    int v5; // edx MAPDST
    int v6; // ecx
    int v7; // edx MAPDST
    int v8; // ecx
    char stmp[264]; // [esp+0h] [ebp-108h] BYREF

    eprintf("\n");
    eprintf(str, v5, a2, v7);
    WinDestruct();
    printf("\n\n\n\n\n   ");
    printf(str, v5, a2, v6);
    printf("\n\n\n\n\n");
    sprintf(stmp, str, v7, a2, v8);
    WinMsgError(stmp);
*/
    exit(1);
}

/***************************************************************************************************/



