//int gMapperEnable = 0;
//int GameSaveBmp( int width, int height, char *palette, char *pixels );
//int gSysMapperEnable = 0;

#include "FrameWork.h"

char *gGamePath = "game/";
void MouseMgrResetSpeed();
void FadeInit();
int SysOpenDataBase();
int SysOpenDataFiles();
void SysLoadConfiguration( int MapperFlag, int argc, char **argv );
void SysUnloadConfiguration( int SaveFlag );
xFile_t *SysMapperInit();

#define SYS_ERROR( errmsg, fmt, m... ) sprintf( errmsg, fmt, ##m ); WinMsgError( errmsg ); exit(1)

typedef struct // local
{
  int Width;
  int Height;
} Resolutions_t;


static int ( *gSysInitVidModeSel[12])() = {
    VidInit320x200x8bpp, VidInit640x480x8bpp, VidInitDummy, VidInit320x400x8bpp,
    VidInitDummy, VidInit640x400x8bpp, VidInitDummy, VidInit800x600x8bpp,
    VidInitDummy, VidInit1024x768x8bpp, VidInitDummy, VidInit1280x1024x8bpp
};

static Resolutions_t gSysResolutionTable[12] = {
    { 320,  200  }, { 640,  480  }, { 640,  240  }, { 320,  400  }, 
    { 640,  200  }, { 640,  400  }, { 800,  300  }, { 800,  600  }, 
    { 1024, 384  }, { 1024, 768  }, { 1280, 512  }, { 1280, 1024 }
};




int gSysResolWidth;

int gSysResolHeight;
int gSysUnk54;
int gSysFont;

int gSysUnk52;
int gSysUnk53;
int gSysUnk56;
int gSysUnk51;
int gSysUnk55;
int gSysUnk57;

int gSysMasterDataFile;
int gSysCritterDataFile;

int gCfgInit = 0;

Config_t gConfiguration;
char gCfgPath[ 264 ];

void SysHelpDialog()
{
    int Window, IsEnabled, v9;
    char *Surface, *p;
    CachePool_t *Block;

    v9 = MapUnk35();
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
                FadeSetPalette( &gPalBase );
                while( InpUpdate() == -1 && !gMenuEscape );
                while( MseGetButtons() ) InpUpdate();
                FadeSetPalette(gFadePaletteC);
            }
        }
        WinClose( Window );
        PalLoadFromFile( "color.pal" );
        FadeSetPalette( &gPalBase );
    }
    if( IsEnabled ) CycleColorStart();
    GmouseIsoEnter();
    if( v9 ) MapUnk34();
}

void SykInit( int VideoModeSel, int Flags )
{
//    int v2, v5,v6, v12; 
    int i, n, k,j, err;
    char errmsg[ 256 ];
printf("Syk Init<<<<<<<<<<<<<<<<<<<\n");
//    IntLib_TaskListAdd(InitVidTab);
    gSysUnk51 = 0x1f;
    gSysUnk52 = 0x1f;
    gSysUnk53 = 0x1f;
    gSysUnk54 = 0x1f;
    gSysUnk55 = 0x1f;
    gSysUnk56 = 0x2010000;
    gSysUnk57 = 0;
    gSysResolHeight = gSysResolutionTable[ VideoModeSel ].Height;
    gSysResolWidth  = gSysResolutionTable[ VideoModeSel ].Width;
//    for( i = 0; i < 368; ){
//        i += 23;
//        gVidTabUnk02[i] = -1;
//    }
    VidSetMMX(1);
    err = WinInit( gSysInitVidModeSel[ VideoModeSel ], VidClose, Flags );
    switch( err ){
        case 0: break;
        case 1:  SYS_ERROR( errmsg, "Error initializing video mode %dx%d\n", gSysResolWidth, gSysResolHeight );
        case 2:  SYS_ERROR( errmsg, "Not enough memory to initialize video mode\n" );
        case 3:  SYS_ERROR( errmsg, "Couldn't find/load text fonts\n" );
        case 4:  SYS_ERROR( errmsg, "Attempt to initialize window system twice\n" );
        case 5:  SYS_ERROR( errmsg, "Window system not initialized\n" );
        case 6:  SYS_ERROR( errmsg, "Current windows are too big for new resolution\n" );
        case 7:  SYS_ERROR( errmsg, "Error initializing default database.\n" );
        case 8:  SYS_ERROR( errmsg, "Error 8\n" );
        case 9:  SYS_ERROR( errmsg, "Program already running.\n" );
        case 10: SYS_ERROR( errmsg, "Program title not set.\n" );
        case 11: SYS_ERROR( errmsg, "Failure initializing input devices.\n" );
        default: SYS_ERROR( errmsg, "Unknown error code %d\n", err );
    }
    FontSet( 100 );
    gSysFont = 100;
    MouseMgrResetSpeed();
//    MouseMgrSetHandler( ScpHandlerCallback );
    for( j = 0; j < 64; j++ ){
        i = j * 256;
        k = 0;
        n = (j + 1) * 256;
        do{
            i++;
//            gVidTabUnk60[ i ] = k*j*i / 512;
        } while ( i != n );
    }
}


int SysSetMemMng()
{
    CfgSetMemMng( Malloc, Realloc, Free );
//    SetMemMng2(MallocCb1, ReallocCb1, FreeCb1);
    return 0;
}


int GameSaveBmp( int width, int height, char *palette, char *pixels )
{
    MsgLine_t Line;

    if( InpSaveBMP( width, height, pixels, palette ) ){
        Line.Id = 8;
//        if( MessageGetMsg( &gMessage, &Line ) == 1 ) IfcMsgOut();
        return -1;
    } else {
        Line.Id = 3;
//        if( MessageGetMsg( &gMessage, &Line ) == 1 ) IfcMsgOut();
        return 0;
    }
}

void SysQuitDlg()
{
    int k, e, IsCursorClear, CursorId, CursorUpdate;
    MsgLine_t msg;

    if( (k = MapUnk35()) )
        e = GmouseUnk58();
    else
        e = 0;
    if( e ) GmouseUnk03();
    IsCursorClear = MseIsCursorClear();
    CursorUpdate = IsCursorClear == 0;
    if( CursorUpdate ) MseDrawCursor();
    CursorId = GmouseGetCursorId();
    GmouseLoadCursor( 1 );
    msg.Id = 0; // 'Are you sure you want to quit?'
    if( MessageGetMsg( &gMessage, &msg ) == 1 && 
	DlgBox( msg.Text, 0, 0, 169, 117, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 16 ) == 1 
      ) gMenuEscape = 2;
    GmouseLoadCursor( CursorId );
    if( !CursorUpdate ) MseCursorRedraw();
    if( e ) GmouseIsoEnter();
    if( k ) MapUnk34();
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

xFile_t *SysMapperInit()
{
/*
    int v0;
    xFile_t *result;
    int v2;
    void *p;
    void *g;
    int v5;
    xFile_t *v6;
    xFile_t *v7;
    char stmp[64];
    char v9[64];
    char pValue[4];
    int EntryValue;
    void *stream;

    EntryValue = 0;
    *pValue = 0;
    CfgGetInteger(&gConfiguration, "system", "splash", &EntryValue);
    v0 = 0;
    result = CfgGetString(&gConfiguration, "system", "language", pValue);
    if ( result == 1 && (result = strcasecmp(*pValue, "english")) != 0 )
        result = sprintf(stmp, "art/%s/splash/", *pValue );
    else
        strcpy( stmp, "art/splash/" );
    while( v0 < 10 ){
        sprintf(v9, "%ssplash%d.rix", stmp, EntryValue);
        result = dbOpen(v9, aRb_15);
        stream = result;
        if ( result )
            break;
        ++v0;
        if ( ++EntryValue >= 10 )
            EntryValue = v2;
    }
    if( stream ) return result;        
    if( (g = Malloc( 0x300 )) ){        
        if( (p = Malloc( 0x4B000 )) ){
            SetPalette( gPaletteRGB );
            dbseek( v6, v5, 0);
            dbread( g, 1, 0x300, v7 );
            dbread( p, 1, 0x4B000, stream );
            dbClose(stream);
            gVidCopyA( p, 640, 480, 0, 0, 640, 480, 0, 0 );
            FadeInOut( g );
            Free( p );
        }
        Free(g);
    }
    return CfgSetInteger( &gConfiguration, "system", "splash", EntryValue + 1 );
*/
DD
return NULL;
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
    if( (s = strchr(*argv, '/')) ) {
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





/***************************************************************************************************/



