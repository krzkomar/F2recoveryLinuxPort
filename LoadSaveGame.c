#include "FrameWork.h"

#define THUMBNAIL_WIDTH 	224
#define THUMBNAIL_HEIGHT 	133
#define THUMBNAIL_SIZE 		(THUMBNAIL_WIDTH * THUMBNAIL_HEIGHT)

//char *MapCityLvlName( int, int );
//char *MapCityName( int );

const int gLsgImgIds[ 9 ] = { 237, 238, 239, 9, 8, 181, 182, 199, 200 };

int	gLsgSelectedSlotIdx = 0;
int 	gLsgUnk01 = 0;
int 	gLsgUnk18 = 0;
int 	gLsgUnk55 = -1;
int 	gLsgUnk56 = 0;
char 	*gLsgMasterPatches = NULL;

int (*gLsgSaveAction[ 27 ])( xFile_t * ) = {
    LsgFSaveNull,		// 0
    LsgSavePlayerPIN,		// 1
    ScptSaveVariables,		// 2
    LsgFSaveMaps,		// 3
    ScptSaveVariables,		// 4
    ObjFSave,			// 5
    CritterSave,		// 6
    CritterSaveKillStats, 	// 7
    SkillSaveSpecials,		// 8
    RandFSave,			// 9
    PerkSave,			// 10
    CombatFSave,		// 11
    AiFSave,			// 12
    FeatSavePoints,		// 13
    ItemFSave,			// 14
    TraitSave,			// 15
    AutomapFSave,		// 16
    OptSaveData,		// 17
    CharEditSaveLvl,		// 18
    WmSaveWmapDatFile,		// 19
    PipFSave,			// 20
    GMovieSave,			// 21
    SkillUsingSave,		// 22
    PartyFSave,			// 23
    EvQeSave,			// 24
    IfacePanelSave,		// 25
    LsgFSaveNull		// 26
};

#define LSG_FUNC_CNT_LOAD 27

int (*gLsgLoadAction[ 27 ])( xFile_t *) = {
    LsgFLoadInit,		// 0 +
    LsgLoadPlayerPIN,		// 1 +
    ScptLoadVariables,		// 2 + GVARS
    LsgSlotMap2Game,		// 3 + Maps
    ScptLoadVarTest,		// 4 + GVARS (test)
    ObjLoadDude,		// 5 + player + inventory
    CritterLoad,		// 6 + player stats
    CritterLoadKillStats,	// 7 + kill stat
    SkillLoadSpecials,		// 8 + Specialized skills selection (Tag skills)
    RandFSave,			// 9 + not used
    PerkLoad,			// 10 + load perks
    CombatLoad,			// 11 + Combat
    AiLoad,			// 12 + AI
    FeatLoadPoints,		// 13 + Exp + Level
    ItemFSave,			// 14 + not used
    TraitLoad,			// 15 + Traits
    AutomapFLoad,		// 16 + Automap ?
    OptLoadData,		// 17 + Preferences
    CharEditLoadLvl,		// 18 + Character window level + outstanding perks
    WmLoadWmapDatFile,		// 19 + world map data
    PipFLoad,			// 20 + not used
    GMovieLoad,			// 21 + iplogo&credits + Video Archives in the Pipboy (0/1)
    SkillUsingLoad,		// 22 + Skill use
    PartyFLoad,			// 23 +? load party
    EvQeLoad,			// 24 +? load event queue
    IfacePanelLoad,		// 25 
    LsgUnk47			// 26
};

int gLsgLoadSaveInProcess = 0;

Geo_t 	gLsgGeo[9];
Msg_t 	gLsgMsg;
LsgSlot_t gLsgSlots[ 10 ];
int 	gLsgSaveFilesCondition[ 10 ];
char 	*gLsgThumbnailP;
char 	*gLsgThumbnailEnd;
MsgLine_t gLsgMsgLine;
int 	gLsgTimer;
int 	gLsgWin;
char 	*gLsgImg[ 9 ];
char 	*gLsgThumbnailBuffer;
char 	gLsgFileDir[300];
char 	gLsgBakFileName[300];
char 	gLsgCurFileName[300];
char 	gLsgMsgPath[300];
char 	*gLsgSurf;
char 	gLsgFileName[300];
xFile_t *gLsgFileHandler;
int 	gLsgError;
int 	gLsgFontSave;
CachePool_t *gLsgArt[ 9 ];

/******************************************************************/

void LsgClean()
{
    gLsgUnk01 = 0;
    gLsgSelectedSlotIdx = 0;
    if( CfgGetString( &gConfiguration, "system", "master_patches", &gLsgMasterPatches ) != 1 ){
        eprintf( "LOADSAVE: Error reading patches config variable! Using default." );
        gLsgMasterPatches[0] = '\0';
    }
    LsgDeleteFiles( "maps/", "sav" );
    LsgDeleteFiles( "proto/critters/", "pro" );
    LsgDeleteFiles( "proto/items/", "pro" );
}

void LsgPurgeFiles()
{
    LsgDeleteFiles( "maps/", "sav" );
    LsgDeleteFiles( "proto/critters/", "pro" );
    LsgDeleteFiles( "proto/items/", "pro" );
}

int LsgSaveGameMenu( int Mode )
{
    int ExtCode, tb,sel,aa,stime,v44,tt,tmp,v42,bottom,right,ee,MoveUpDn,SelectedLine,ff;
    uint32_t SysTime;
    char stmp[260], a1[260], *Str2[3];

    ExtCode = -1;
    SelectedLine = -1;
    Str2[0] = gLsgCurFileName;
    Str2[1] = gLsgFileDir;
    Str2[2] = stmp;
    gLsgError = 0;
    if( CfgGetString( &gConfiguration, "system", "master_patches", &gLsgMasterPatches ) != 1 ){
        eprintf( "LOADSAVE: Error reading patches config variable! Using default." );
        gLsgMasterPatches = "/fallout/cd/data/savegame";
    }
    if( Mode == 2 && gLsgUnk01 ){ // fast save
        sprintf( gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1 );
        strcpy( &gLsgFileName[ strlen( gLsgFileName ) ], "save.dat" );
        gLsgFileHandler = dbOpen( gLsgFileName, "rb" );
        if( gLsgFileHandler ){
            LsgSlotLoad( gLsgSelectedSlotIdx );
            dbClose( gLsgFileHandler );
        }
        gLsgThumbnailEnd = NULL;
        if( (tb = LsgMakeThumbnail()) == 1 ) LsgSaveGame();
        if( gLsgThumbnailEnd ) Free( gLsgThumbnailBuffer );
        GmouseLoadCursor( 1 );
        if( tb == -1 ){
    	    if( MessageInit( &gLsgMsg ) != 1 ) return -1;
    	    sprintf( a1, "%s%s", gGamePath, "lsgame.msg" );
    	    if( MessageLoad( &gLsgMsg, a1 ) != 1 ) return -1;
    	    GSoundPlay( "iisxxxx1" );
    	    strcpy( gLsgBakFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 132 ) );
    	    strcpy( gLsgCurFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 133 ) );
    	    DlgBox( gLsgBakFileName, Str2, 1, 169, 116, COLOR_A0, 0, COLOR_A0, 1 );
    	    MessageClose( &gLsgMsg );
    	    return -1;     
        }           
        return 1;
    }
    gLsgUnk01 = 0;
    if( LsgMenuCreate( Mode == 2 ) == -1 ){ eprintf( "LOADSAVE: ** Error loading save game screen data! **" ); return -1; } 
    if( LsgLoadSlots() == -1 ){
        WinUpdate( gLsgWin );
        GSoundPlay( "iisxxxx1" );
        strcpy( gLsgBakFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 106 ) );
        strcpy( gLsgCurFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 107 ) );
        sprintf( gLsgFileDir, "\"%s/\"", "savegame" );
        strcpy( stmp, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 108 ) );
        DlgBox( gLsgBakFileName, Str2, 2, 169, 116, COLOR_A0, 0, COLOR_A0, 1 );
        LsgClose( 0 );
        return -1;
    }

    tmp = gLsgSaveFilesCondition[ gLsgSelectedSlotIdx ];
    if( tmp == 0 || tmp == 2 || tmp == 3 ){
        ScrCopy( gLsgThumbnailEnd, 223, 132, THUMBNAIL_WIDTH, WIN_XY( 366, 58, 640, gLsgSurf ), 640 );
    } else {
        LsgReadThumbnail( gLsgSelectedSlotIdx );
        ScrCopy( gLsgThumbnailP, 223, 132, THUMBNAIL_WIDTH, WIN_XY( 366, 58, 640, gLsgSurf ), 640 );
    }
    LsgDrawSlotList( 0 );
    LsgDescription( gLsgSelectedSlotIdx );
    WinUpdate( gLsgWin );
    gLsgTimer = 24;
    while( ExtCode == -1 ){ // select slot
        SysTime = TimerGetSysTime();
        sel = InpUpdate();
        aa = 0; MoveUpDn = 0; ee = 0; ff = 0;
        if( gMenuEscape ) ExtCode = 0;
        switch( sel ){
            case KEY_ESC:
            case 501: ExtCode = 0; break; // click Close, or hit Esc
            case 328: if( --gLsgSelectedSlotIdx < 0 ) gLsgSelectedSlotIdx = 0; aa = 1; SelectedLine = -1; break;
            case 336: if( ++gLsgSelectedSlotIdx > 9 ) gLsgSelectedSlotIdx = 9; aa = 1; SelectedLine = -1; break;
            case 327: aa = 1; SelectedLine = -1; gLsgSelectedSlotIdx = gMenuEscape; break;
            case 335: SelectedLine = -1; gLsgSelectedSlotIdx = 9; aa = 1; break;
            case 506: MoveUpDn = 1; break; // click Up Arrow
            case 504: MoveUpDn = 2; break; // click Dn Arrow
            case 502: // click list area
                MseGetCursorPosition( &right, &bottom );
                gLsgSelectedSlotIdx = (bottom - 79) / (3 * gFont.ChrHeight() + 4); // mouse y position to line number
                if( gLsgSelectedSlotIdx < 0 ) gLsgSelectedSlotIdx = 0;
                if( gLsgSelectedSlotIdx > 9 ) gLsgSelectedSlotIdx = 9;
                aa = 1;
                if( gLsgSelectedSlotIdx == SelectedLine ){ sel = 500; GSoundPlay( "ib1p1xx1" ); }
                SelectedLine = gLsgSelectedSlotIdx;
                MoveUpDn = 0;
                break;
            case 17: case 24: case 324: SysQuitDlg(); if( gMenuEscape ) ExtCode = 0; break;
            case 61: case 43: OptBrightInc(); break;
            case 45: case 95: OptBrightDec(); break;
            case KEY_ENTER: sel = 500; break;
        }            
	if( sel == 500 ){ // click Done
            ExtCode = gLsgSaveFilesCondition[ gLsgSelectedSlotIdx ];
            if( ExtCode == 1 ){ // file exist, overwrite
            	if( !DlgBox( MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 131 ), 0, 0, 169, 116, COLOR_A0, 0, COLOR_A0, 16) ) ExtCode = -1;
            } else {
            	ExtCode = 1;
            }
            aa = 1;
            MoveUpDn = 0;
	}
        if( MoveUpDn ){
                v42 = 4;
                SelectedLine = -1;
                do{
                    stime = TimerGetSysTime();
                    v44 = ++ff;
                    if( !ee && (v44 == 1 || ee == 1) && (double)ff > 14.4 ){
                        ee = 1;
                        if( (double)ff > 14.4 && ++v42 > 24 ) v42 = 24;
                        if( MoveUpDn == 1 ){ // Up
                            if( --gLsgSelectedSlotIdx < 0 ) gLsgSelectedSlotIdx = 0;
                        } else if( ++gLsgSelectedSlotIdx > 9 ){
                            gLsgSelectedSlotIdx = 9;
                        }
                        tmp = gLsgSaveFilesCondition[ gLsgSelectedSlotIdx ];
                        if( tmp && tmp != 2 ){
                            LsgReadThumbnail( gLsgSelectedSlotIdx );
                            ScrCopy( gLsgThumbnailP, 223, 132, THUMBNAIL_WIDTH, WIN_XY( 366, 58, 640, gLsgSurf ), 640 );
                        } else {
                            ScrCopy( gLsgThumbnailEnd, 223, 132, THUMBNAIL_WIDTH, WIN_XY( 366, 58, 640, gLsgSurf ), 640 );
                        }
                        LsgDrawSlotList( 0 );
                        LsgDescription( gLsgSelectedSlotIdx );
                        WinUpdate( gLsgWin );
                    }
                    if( (double)ff > 14.4 ){
                	TIMER_WAIT( stime, 1000 / v42 );
                    } else {
                        TIMER_WAIT( stime, 41 );
                    }
                    tt = InpUpdate();
                } while( tt != 505 && tt != 503 );
            continue;
        }
        if( aa ){
            tmp = gLsgSaveFilesCondition[ gLsgSelectedSlotIdx ];
            if( tmp == 0 || tmp == 2 || tmp == 3 ){
                ScrCopy( gLsgThumbnailEnd, 223, 132, THUMBNAIL_WIDTH, WIN_XY( 366, 58, 640, gLsgSurf ), 640 );
            } else {
                LsgReadThumbnail( gLsgSelectedSlotIdx );
                ScrCopy( gLsgThumbnailP, 223, 132, THUMBNAIL_WIDTH, WIN_XY( 366, 58, 640, gLsgSurf ), 640 );
            }
            LsgDescription( gLsgSelectedSlotIdx );
            LsgDrawSlotList( 0 );
        }
        WinUpdate( gLsgWin );
        if( !--gLsgTimer ){
            gLsgTimer = 24;
            SelectedLine = -1;
        }
        TIMER_WAIT( SysTime, 41 );
    }

    if( ExtCode == 1 ){ // selection done
	switch( LsgEditLine( gLsgSelectedSlotIdx ) ){
	    case -1:
                GmouseLoadCursor( 1 );
                GSoundPlay( "iisxxxx1" );
                eprintf( "LOADSAVE: ** Error getting save file comment **" );
                strcpy( gLsgBakFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 132 ) );
                strcpy( gLsgCurFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 133 ) );
                ExtCode = -1;
                DlgBox( gLsgBakFileName, Str2, 1, 169, 116, COLOR_A0, 0, COLOR_A0, 1 );
                break;
    	    case 0:
        	GmouseLoadCursor( 1 );
        	ExtCode = -1;
        	break;
    	    case 1:
		if( LsgSaveGame() == -1 ){
        	    GmouseLoadCursor( 1 );
        	    GSoundPlay( "iisxxxx1" );
        	    strcpy( gLsgBakFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 132 ) );
        	    strcpy( gLsgCurFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 133 ) );
        	    ExtCode = -1;
        	    DlgBox( gLsgBakFileName, Str2, 1, 169, 116, COLOR_A0, 0, COLOR_A0, 1 );
        	    if( LsgLoadSlots() == -1 ){
            	        WinUpdate( gLsgWin );
            		GSoundPlay( "iisxxxx1" );
            		strcpy( gLsgBakFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 106 ) );
            		strcpy( gLsgCurFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 107 ) );
            		sprintf( gLsgFileDir, "\"%s/\"", "savegame");
            		strcpy( stmp, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 108 ) );
            		DlgBox( gLsgBakFileName, Str2, 2, 169, 116, COLOR_A0, 0, COLOR_A0, 1 );
            		LsgClose( 0 );
            		return -1;
        	    }
        	    tmp = gLsgSaveFilesCondition[ gLsgSelectedSlotIdx ];
        	    if( tmp == 0 || tmp == 2 || tmp == 3 ){
            		ScrCopy( gLsgThumbnailEnd, 223, 132, THUMBNAIL_WIDTH, WIN_XY( 366, 58, 640, gLsgSurf ), 640 );
        	    } else {
            		LsgReadThumbnail( gLsgSelectedSlotIdx );
            		ScrCopy( gLsgThumbnailP, 223, 132, THUMBNAIL_WIDTH, WIN_XY( 366, 58, 640, gLsgSurf ), 640 );
        	    }
        	    LsgDrawSlotList( 0 );
        	    LsgDescription( gLsgSelectedSlotIdx );
        	    WinUpdate( gLsgWin );
        	    gLsgTimer = 24;
        	    break;
    	    }
        }
    }
    GmouseLoadCursor( 1 );
    LsgClose( 0 );
    TileUpdate();
    if( Mode == 2 && ExtCode == 1 ) gLsgUnk01 = 1;
    return ExtCode;    
}

int LsgMakeThumbnail()
{
    int n;
    
    if( !(gLsgThumbnailBuffer = Malloc( THUMBNAIL_SIZE )) ) return -1;
    n = GmouseUnk58();
    if( n ) GmouseUnk03();
    MseCursorHide();
    TileUpdate();
    MseCursorShow();
    if( n ) GmouseIsoEnter();
    ScrScaleImg( WinGetSurface( gMapIsoWin ), 640, 380, 640, gLsgThumbnailBuffer, THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, THUMBNAIL_WIDTH );
    gLsgThumbnailEnd = gLsgThumbnailBuffer;
    return 1;
}

int LsgMenuGameLoad( unsigned int arg )
{
    int Exit,win,SysTime,sel,ListMoveFlag,n,v40,v41,bottom,right,mode,v87,v88,v89,v90;
    unsigned int v39, Time;
    char stmp[260], stmp2[260], *Str2[3];

    Exit = -1;
    v89 = -1;
    Str2[0] = gLsgCurFileName;
    Str2[1] = gLsgFileDir;
    Str2[2] = stmp;
    gLsgError = 0;

    if( CfgGetString( &gConfiguration, "system", "master_patches", &gLsgMasterPatches) != 1 ){
        eprintf( "LOADSAVE: Error reading patches config variable! Using default." );
        gLsgMasterPatches = "/fallout/cd/data/savegame";
    }

    if( arg == 2 && gLsgUnk01 ){ // fast load ?
        if( (win = WinCreateWindow( 0, 0, 640, 480, 256, 18 )) != -1 ){
            ScrFillSolid( WinGetSurface( win ), 640, 480, 640, gPalColorCubeRGB[0][0][0] );
            WinUpdate( win );
        }
        if( LsgLoad( gLsgSelectedSlotIdx ) == -1 ){
    	    if( MessageInit( &gLsgMsg ) != 1 ) return -1;
    	    sprintf( stmp2, "%s%s", gGamePath, "lsgame.msg" );
    	    if( MessageLoad( &gLsgMsg, stmp2 ) != 1 ) return -1;
    	    if( win != -1 ) WinClose( win );
    	    GmouseLoadCursor( 1 );
    	    GSoundPlay( "iisxxxx1" );
    	    strcpy( gLsgBakFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 134 ) );
    	    strcpy( gLsgCurFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 135 ) );
    	    DlgBox( gLsgBakFileName, Str2, 1, 169, 116, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 1 );
    	    MessageClose( &gLsgMsg );
    	    MapReset();
    	    gMenuEscape = 2;
    	    return -1;
        }
        if( win != -1 ) WinClose( win );
        GmouseLoadCursor( 1 );
        return 1;        
    }
    gLsgUnk01 = 0;
    mode = 3;
    if( arg ){
        if( arg <= 1 ){
            mode = 2;
        } else if( arg == 2 ){
            mode = 4;
        }
    }
    if( LsgMenuCreate( mode ) == -1 ){ eprintf( "LOADSAVE: ** Error loading save game screen data! **" ); return -1; } 
    if( LsgLoadSlots() == -1 ){
        GmouseLoadCursor( 1 );
        WinUpdate( gLsgWin );
        GSoundPlay( "iisxxxx1" );
        strcpy( gLsgBakFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 106 ) );
        strcpy( gLsgCurFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 107 ) );
        sprintf( gLsgFileDir, "\"%s/\"", "savegame" );
        strcpy( stmp, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 108 ) );
        DlgBox( gLsgBakFileName, Str2, 2, 169, 116, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 1 );
        LsgClose( mode );
        return -1;
    }

    n = gLsgSaveFilesCondition[ gLsgSelectedSlotIdx ];
    if( !n || n == 2 || n == 3 ){
        ScrCopy( gLsgImg[2], gLsgGeo[2].Width, gLsgGeo[2].Height, gLsgGeo[2].Width, gLsgSurf + 25300, 640 );
    } else {
        LsgReadThumbnail( gLsgSelectedSlotIdx );
        ScrCopy( gLsgThumbnailP, 223, 132, THUMBNAIL_WIDTH, gLsgSurf + 37486, 640 );
    }
    LsgDrawSlotList( 2 );
    LsgDescription( gLsgSelectedSlotIdx );
    WinUpdate( gLsgWin );
    gLsgTimer = 24;
    while( Exit == -1 ){
        while( Exit == -1 ){
            SysTime = TimerGetSysTime();
            sel = InpUpdate();
            ListMoveFlag = v88 = v87 = v90 = 0;
            if( gMenuEscape ){ Exit = 0; sel = -1; }
            switch( sel ){
                case 27 : case 501: Exit = 0; break;
                case 328: if( --gLsgSelectedSlotIdx < 0 ) gLsgSelectedSlotIdx = 0; v89 = -1; ListMoveFlag = 1; break;
                case 336: if( ++gLsgSelectedSlotIdx > 9 ) gLsgSelectedSlotIdx = 9; v89 = -1; ListMoveFlag = 1; break;
                case 327: gLsgSelectedSlotIdx = 0; v89 = -1; ListMoveFlag = 1; break;
                case 335: gLsgSelectedSlotIdx = 9; v89 = -1; ListMoveFlag = 1; break;
                case 506: v88 = 1; break;
                case 504: v88 = 2; break;
                case 502: // list area
                    MseGetCursorPosition( &right, &bottom );
                    n = (bottom - 79) / (3 * gFont.ChrHeight() + 4);
                    if( n < 0 ) n = 0;
                    if( n > 9 ) n = 9;
                    gLsgSelectedSlotIdx = n;
                    if( gLsgSelectedSlotIdx == v89 ) GSoundPlay( "ib1p1xx1" );
                    ListMoveFlag = 1;
                    v88 = 0;
                    v89 = gLsgSelectedSlotIdx;
                    break;
                case 17: case 24: case 324: SysQuitDlg(); if( gMenuEscape ) Exit = 0; break;
                case '=':  case '+': OptBrightInc(); break;
                case '-':  case '_': OptBrightDec(); break;
                case '\r': case 500:
            	    Exit = gLsgSaveFilesCondition[ gLsgSelectedSlotIdx ] ? 1: -1;
            	    ListMoveFlag = 1;
            	    v88 = 0;
            	    break;
            }                    
            if( v88 ){
                v39 = 4;
                v89 = -1;
                do{
                    v40 = TimerGetSysTime();
                    v41 = v90 + 1;
                    Time = v40;
                    v90++;
                    if( !v87 && ( v41 == 1 || v87 == 1 ) && (double)v90 > 14.4 ){
                        v87 = 1;
                        if( ((double)v90 > 14.4) && (++v39 > 24) ) v39 = 24;
                        if( v88 == 1 ){
                            if( --gLsgSelectedSlotIdx < 0 ) gLsgSelectedSlotIdx = 0;
                        } else if( ++gLsgSelectedSlotIdx > 9 ){
                            gLsgSelectedSlotIdx = 9;
                        }
                        n = gLsgSaveFilesCondition[gLsgSelectedSlotIdx];
                        if( !n || n == 2 || n == 3 ){
                            ScrCopy( gLsgImg[2], gLsgGeo[2].Width, gLsgGeo[2].Height, gLsgGeo[2].Width, gLsgSurf + 25300, 640 );
                        } else {
                            LsgReadThumbnail( gLsgSelectedSlotIdx );
                            ScrCopy( gLsgImg[0] + 25300, gLsgGeo[2].Width, gLsgGeo[2].Height, 640, gLsgSurf + 25300, 640 );
                            ScrCopy( gLsgThumbnailP, 223, 132, THUMBNAIL_WIDTH, gLsgSurf + 37486, 640 );
                        }
                        LsgDrawSlotList( 2 );
                        LsgDescription( gLsgSelectedSlotIdx );
                        WinUpdate( gLsgWin );
                    }
                    if( (double)v90 > 14.4 ){
			TIMER_WAIT( Time, (1000 / v39) );
                    } else {
			TIMER_WAIT( Time, 41 );
                    }
                    n = InpUpdate();
                } while( n != 505 && n != 503 );
            } else {
                if( ListMoveFlag ){
                    n = gLsgSaveFilesCondition[gLsgSelectedSlotIdx];
                    if( !n || n == 2 || n == 3 ){
                        ScrCopy( gLsgImg[2], gLsgGeo[2].Width, gLsgGeo[2].Height, gLsgGeo[2].Width, gLsgSurf + 25300, 640 );
                    } else {
                        LsgReadThumbnail( gLsgSelectedSlotIdx );
                        ScrCopy( gLsgImg[0] + 25300, gLsgGeo[2].Width, gLsgGeo[2].Height, 640, gLsgSurf + 25300, 640 );
                        ScrCopy( gLsgThumbnailP, 223, 132, THUMBNAIL_WIDTH, gLsgSurf + 37486, 640 );
                    }
                    LsgDescription( gLsgSelectedSlotIdx );
                    LsgDrawSlotList( 2 );
                }
                WinUpdate( gLsgWin );
                if( !--gLsgTimer ){
                    gLsgTimer = 24;
                    v89 = -1;
                }
		TIMER_WAIT( SysTime, 41 );
            }
        }
        if( Exit == 1 ){
            switch( gLsgSaveFilesCondition[ gLsgSelectedSlotIdx ] ){
        	case 3:
            	    GSoundPlay( "iisxxxx1" );
            	    strcpy( gLsgBakFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 134 ) );
            	    strcpy( gLsgCurFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 136 ) );
            	    strcpy( gLsgFileDir, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 135 ) );
            	    Exit = -1;
            	    DlgBox( gLsgBakFileName, Str2, 2, 169, 116, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 1 );
            	    break;
            case 2:
            	    GSoundPlay( "iisxxxx1" );
            	    strcpy( gLsgBakFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 134 ) );
            	    strcpy( gLsgCurFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 135 ) );
            	    Exit = -1;
            	    DlgBox( gLsgBakFileName, Str2, 1, 169, 116, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 1 );
            	    break;
            default:
        	    if( LsgLoad( gLsgSelectedSlotIdx ) != -1 ) break;
            	    GmouseLoadCursor( 1 );
            	    GSoundPlay( "iisxxxx1" );
            	    strcpy( gLsgBakFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 134 ) );
            	    strcpy( gLsgCurFileName, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 135 ) );
            	    Exit = -1;
            	    DlgBox(gLsgBakFileName, Str2, 1, 169, 116, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 1);
            	    MapReset();
            	    gMenuEscape = 2;        	
            }
        }
    }
    LsgClose( (arg == 0) + 2 );
    if( (arg == 2) && (Exit == 1) ) gLsgUnk01 = 1;
    return Exit;            
}

// 0- save game, 1- pick a quick save slot, 2,3 - load game, 4 - pick a quick load slot
int LsgMenuCreate( unsigned int mode )
{
    int i, bt;

    gLsgFontSave = FontGetCurrent();
    FontSet( 103 );
    gLsgUnk18 = 0;
    if( MessageInit( &gLsgMsg ) != 1 ) return -1;
    sprintf( gLsgMsgPath, "%s%s", gGamePath, "lsgame.msg" );
    if( MessageLoad( &gLsgMsg, gLsgMsgPath ) != 1 ) return -1;
    if( !( gLsgThumbnailBuffer = Malloc( 61632 ) ) ){ MessageClose( &gLsgMsg ); FontSet( gLsgFontSave ); return -1; }

    gLsgThumbnailP = gLsgThumbnailBuffer;
    gLsgThumbnailEnd = gLsgThumbnailBuffer + THUMBNAIL_SIZE;
    if( mode != 3 ) gLsgUnk18 = MapAmbientDisable();
    CycleColorStop();
    GmouseLoadCursor(1);
    if( mode <= 1 ){ // save
        if( (i = GmouseUnk58()) ) GmouseUnk03();
        MseCursorHide();
        TileUpdate();
        MseCursorShow();
        if( i ) GmouseIsoEnter();
        ScrScaleImg( WinGetSurface( gMapIsoWin ), 640, 380, 640, gLsgThumbnailEnd, THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, THUMBNAIL_WIDTH );
    }
    for( i = 0; i < 9; i++ ){
        if( !(gLsgImg[ i ] = ArtLoadBmp( ArtMakeId( 6, gLsgImgIds[ i ], 0, 0, 0 ), &gLsgArt[ i ], &gLsgGeo[ i ].Width, &gLsgGeo[ i ].Height ) ) ) break;
    }
    if( i < 9 ){
        while( --i ) ArtClose( gLsgArt[ i ] );        
        Free( gLsgThumbnailBuffer );
        MessageClose( &gLsgMsg );
        FontSet( gLsgFontSave );
        if( mode != 3 && gLsgUnk18 ) MapAmbientEnable();
        CycleColorStart();
        GmouseLoadCursor( 1 );
        return -1;
    }
    if( (gLsgWin = WinCreateWindow( 0, 0, 640, 480, 256, 20 )) == -1 ){
        Free( gLsgThumbnailBuffer );
        MessageClose( &gLsgMsg );
        FontSet( gLsgFontSave );
        if( mode != 3 && gLsgUnk18 ) MapAmbientEnable();
        CycleColorStart();
        GmouseLoadCursor( 1 );
        return -1;
    }
    gLsgSurf = WinGetSurface( gLsgWin );
    memcpy( gLsgSurf, gLsgImg[ 0 ], gLsgGeo[ 0 ].Width * gLsgGeo[ 0 ].Height );
    switch( mode ){
        case 0: i = 102; break;
        case 1: i = 103; break;
        case 2: case 3: i = 100; break;
        case 4: i = 101; break;
    }
    gFont.Print( WIN_XY( 48,  27,  640, gLsgSurf ), MessageGetMessage( &gLsgMsg, &gLsgMsgLine, i   ), 640, 640, gPalColorCubeRGB[18][17][3] );
    gFont.Print( WIN_XY( 410, 348, 640, gLsgSurf ), MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 104 ), 640, 640, gPalColorCubeRGB[18][17][3] );
    gFont.Print( WIN_XY( 515, 348, 640, gLsgSurf ), MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 105 ), 640, 640, gPalColorCubeRGB[18][17][3] );

    bt = WinCreateButton( gLsgWin, 391, 349, gLsgGeo[ 3 ].Width, gLsgGeo[ 3 ].Height, -1, -1, -1, 500, gLsgImg[4], gLsgImg[3], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    bt = WinCreateButton( gLsgWin, 495, 349, gLsgGeo[ 3 ].Width, gLsgGeo[ 3 ].Height, -1, -1, -1, 501, gLsgImg[4], gLsgImg[3], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    bt = WinCreateButton( gLsgWin, 35, 58, gLsgGeo[ 8 ].Width, gLsgGeo[ 8 ].Height, -1, 505, 506, 505, gLsgImg[7], gLsgImg[8], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    bt = WinCreateButton( gLsgWin, 35, gLsgGeo[ 8 ].Height + 58, gLsgGeo[ 6 ].Width, gLsgGeo[ 6 ].Height, -1, 503, 504, 503, gLsgImg[5], gLsgImg[6], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    WinCreateButton( gLsgWin, 55, 87, 230, 353, -1, -1, -1, 502, 0, 0, 0, 32 );
    FontSet( 101 );
    return 0;
}

void LsgClose( int Mode )
{
    int i;

    WinClose( gLsgWin );
    FontSet( gLsgFontSave );
    MessageClose( &gLsgMsg );
    for( i = 0; i < 9; i++)  ArtClose( gLsgArt[ i ] );
    Free( gLsgThumbnailBuffer );
    if( Mode != 3 && gLsgUnk18 ) MapAmbientEnable();
    CycleColorStart();
    GmouseLoadCursor(1);
}

int LsgSaveGame()
{
    int i, PrevPos;
    char *tmp;

    eprintf( "********** [ SAVE ] **********" );
    gLsgError = 0;
    gLsgUnk55 = -1;
    GmouseLoadCursor( 25 );
    GSoundBgPause();
    sprintf( gLsgFileName, "%s/%s", gLsgMasterPatches, "savegame" );
    xDirCreate( gLsgFileName );
    sprintf( gLsgFileName, "%s/%s/%s%.2d", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    xDirCreate( gLsgFileName );

    strcpy( &gLsgFileName[ strlen( gLsgFileName ) ], "/proto" );
    xDirCreate( gLsgFileName );

    tmp = gLsgFileName + strlen( gLsgFileName );

    strcpy( tmp, "/critters" );
    xDirCreate( gLsgFileName );
    
    strcpy( tmp, "/items" );
    xDirCreate( gLsgFileName );

    if( LsgBackup() == -1 ) eprintf( "LOADSAVE: Warning, can't backup save file!" );
    sprintf( gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    strcpy( &gLsgFileName[ strlen( gLsgFileName ) ], "save.dat" );
    eprintf( "\tLOADSAVE: Save name: %s", gLsgFileName );

    gLsgFileHandler = dbOpen( gLsgFileName, "wb" );
    if( !gLsgFileHandler ){
        eprintf("\tLOADSAVE: ** Error opening save game for writing! **");
        LsgBackupRestore();
        sprintf( gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1 );
        LsgDeleteFiles( gLsgFileName, "bak" );
        PartyRejoinAll();
        GSoundBgUnPause();
        return -1;
    }
    PrevPos = dbtell( gLsgFileHandler );
    if( LsgSaveHdr( gLsgSelectedSlotIdx ) == -1 ){
        eprintf( "\tLOADSAVE: ** Error writing save game header! **" );
        eprintf( "LOADSAVE: Save file header size written: %d bytes.", dbtell( gLsgFileHandler ) - PrevPos );
        dbClose( gLsgFileHandler );
        LsgBackupRestore();
        sprintf( gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1 );
        LsgDeleteFiles( gLsgFileName, "bak" );
        PartyRejoinAll();
        GSoundBgUnPause();
        return -1;
    }
    eprintf( "LOADSAVE: Save file header size written: %d bytes.", dbtell( gLsgFileHandler ) - PrevPos );
    for( i = 0; i < 27; i++ ){
        PrevPos = dbtell( gLsgFileHandler );
        if( gLsgSaveAction[ i ]( gLsgFileHandler ) == -1 ){
            eprintf( "\tLOADSAVE: ** Error writing save function #%d data! **", i );
            dbClose( gLsgFileHandler );
    	    LsgBackupRestore();
            sprintf( gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1 );
            LsgDeleteFiles( gLsgFileName, "bak" );
            PartyRejoinAll();
            GSoundBgUnPause();
            return -1;
        }
        eprintf( "LOADSAVE: Save function #%d data size written: %d bytes.", i, dbtell( gLsgFileHandler ) - PrevPos );
    }
    eprintf( "LOADSAVE: Total save data written: %ld bytes.", (long int)dbtell( gLsgFileHandler ) );
    dbClose( gLsgFileHandler );
    sprintf( gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    LsgDeleteFiles( gLsgFileName, "bak" );
    gLsgMsgLine.Id = 140;
    if( MessageGetMsg( &gLsgMsg, &gLsgMsgLine ) == 1 )
        IfcMsgOut( gLsgMsgLine.Text );
    else
        eprintf( "Error: Couldn't find LoadSave Message!" );
    GSoundBgUnPause();

    return 0;        
}

int LsgPending()
{
    return gLsgLoadSaveInProcess;
}

int LsgLoad( int SlotNo )
{
    int i, PrevPos;

    gLsgLoadSaveInProcess = 1;
    if( IN_COMBAT ){
        IfaceCombatClose( 0 );
        CombatUnk21();
        GmouseLoadCursor( 25 );
    }
    sprintf( gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    strcpy( &gLsgFileName[ strlen( gLsgFileName ) ], "save.dat" );
    eprintf( "LOADSAVE: Load name: %s", gLsgSlots[ SlotNo ].SaveName );
    gLsgFileHandler = dbOpen(gLsgFileName, "rb");
    if( !gLsgFileHandler ){
        eprintf( "LOADSAVE: ** Error opening load game file for reading! **" );
        gLsgLoadSaveInProcess = 0;
        return -1;
    }
    PrevPos = dbtell( gLsgFileHandler );
    if( LsgSlotLoad( SlotNo ) == -1 ){
        eprintf( "LOADSAVE: ** Error reading save game header! **" );
        dbClose( gLsgFileHandler );
        GameReset();
        gLsgLoadSaveInProcess = 0;
        return -1;
    }
    eprintf( "LOADSAVE: Load file header size read: %d bytes.", dbtell( gLsgFileHandler ) - PrevPos );
    for( i  = 0; i < LSG_FUNC_CNT_LOAD; i++ ){
        PrevPos = dbtell( gLsgFileHandler );
        if( gLsgLoadAction[ i ]( gLsgFileHandler ) == -1 ){
            eprintf( "LOADSAVE: ** Error reading load function #%d data! **", i );
            eprintf( "LOADSAVE: Load function #%d data size read: %d bytes.", i, dbtell( gLsgFileHandler ) - PrevPos );
            dbClose( gLsgFileHandler );
            GameReset();
            gLsgLoadSaveInProcess = 0;
            return -1;
        }
        eprintf( "LOADSAVE: Load function #%d data size read: %d bytes.", i, dbtell( gLsgFileHandler ) - PrevPos );
    }
    eprintf( "LOADSAVE: Total load data read: %ld bytes.", (long int)dbtell( gLsgFileHandler ) );
    dbClose( gLsgFileHandler );
    sprintf( gLsgMsgPath, "%s/", "maps" );
    LsgDeleteFiles( gLsgMsgPath, "bak" );
    ProtoDudeImgInit();
    gLsgMsgLine.Id = 141;
    if( MessageGetMsg( &gLsgMsg, &gLsgMsgLine ) == 1 )
        IfcMsgOut( gLsgMsgLine.Text );
    else
        eprintf( "Error: Couldn't find LoadSave Message!" );
    gLsgLoadSaveInProcess = 0;    
    return 0;
}

void LsgGetTime( short *pDay, short *pMon, short *a4, int a3 )
{
/* cut */
}

int LsgSaveHdr( int SlotNo )
{
    time_t t;
    struct tm tim;
    char stmp[128];
    int n, Gday, Gyear;
    unsigned short tmp[3];

    gLsgError = 4;
    strncpy( gLsgSlots[ SlotNo ].Magic, "FALLOUT SAVE FILE", 24 );
    if( dbwrite( &gLsgSlots[ SlotNo ].Magic, 24, 1, gLsgFileHandler ) != 1 ) return -1;
    tmp[ 0 ] = 1; tmp[ 1 ] = 2;
    gLsgSlots[ SlotNo ].VerMinor = 1;
    gLsgSlots[ SlotNo ].VerMajor = 2;
    if( dbputBewBlk( gLsgFileHandler, tmp, 2 ) == -1 ) return -1;    
    gLsgSlots[ SlotNo ].VerRelease = 'R';
    if( dbputb( gLsgFileHandler, 'R' ) == -1 ) return -1;    
    strncpy( gLsgSlots[ SlotNo ].PlayerName, CritterGetName( gObjDude ), 32 );
    if( dbwrite( gLsgSlots[ SlotNo ].PlayerName, 32, 1, gLsgFileHandler ) != 1 ) return -1;
    if( dbwrite( gLsgSlots[ SlotNo ].SaveName, 30, 1, gLsgFileHandler ) != 1 ) return -1;
    // RTC time
    t = time( NULL );
    tim = *localtime( &t );
    tmp[ 0 ] = tim.tm_mday;
    tmp[ 1 ] = tim.tm_mon + 1;
    tmp[ 2 ] = tim.tm_year + 1900;
    n = tim.tm_min + tim.tm_hour;
    gLsgSlots[ SlotNo ].RtcMonth = tmp[ 0 ];
    gLsgSlots[ SlotNo ].RtcDay   = tmp[ 1 ];
    gLsgSlots[ SlotNo ].RtcYear  = tmp[ 2 ];
    gLsgSlots[ SlotNo ].RtcTime  = n;
    if( dbputBewBlk( gLsgFileHandler, tmp, 3 ) == -1 ) return -1; // RTC day/month/year
    if( dbputLei( gLsgFileHandler, n ) == -1 ) return -1; // RTC time houres + minutes
    // In game time
    ScptGetGameDate( &n, &Gday, &Gyear );
    tmp[ 0 ] = n;
    tmp[ 1 ] = Gday;
    tmp[ 3 ] = Gyear;
    n = ScptGetGameDekaSeconds();
    gLsgSlots[ SlotNo ].GTime = n;                            
    if( dbputBewBlk( gLsgFileHandler, tmp, 3 ) == -1 ) return -1;
    if( dbputLei( gLsgFileHandler, n ) == -1 ) return -1;
    // map level
    gLsgSlots[ SlotNo ].MapLvl = gMapCurrentLvl;
    if( dbputBew( gLsgFileHandler, gMapCurrentLvl ) == -1 ) return -1;
    // map number
    n = MapGetCurrentMapId();
    gLsgSlots[ SlotNo ].MapNum = n;
    if( dbputBew( gLsgFileHandler, n ) == -1 )  return -1;
    // map name
    strcpy( stmp, gMap.Name );
    strncpy( gLsgSlots[ SlotNo ].MapFname, CharEditFnameChgExt( gLsgMsgPath, stmp, "sav" ), 16 );
    if( dbwrite( gLsgSlots[ SlotNo ].MapFname, 16, 1, gLsgFileHandler ) != 1 ) return -1;
    // thumbnail
    if( dbwrite( gLsgThumbnailEnd, THUMBNAIL_SIZE, 1, gLsgFileHandler ) != 1 ) return -1;                                                
    memset( stmp, 0, sizeof( stmp ) );
    if( dbwrite( stmp, 1, 128, gLsgFileHandler ) != 128 ) return -1;
    gLsgError = 0;
    return 0;
}

int LsgSlotLoad( int SlotNo )
{
    LsgSlot_t *slot;
    short tmp[3];

    gLsgError = 3;
    slot = &gLsgSlots[ SlotNo ];
    if( dbread( slot, 1, 24, gLsgFileHandler ) != 24 ) return -1;
    if( strncmp( slot->Magic, "FALLOUT SAVE FILE", 18 ) ){        
        eprintf( "LOADSAVE: ** Invalid save file on load! **" );
        gLsgError = 2;
        return -1;
    }
    if( dbreadBewBlk( gLsgFileHandler, tmp, 2 ) == -1 ) return -1;
    slot->VerMinor = tmp[ 0 ];
    slot->VerMajor = tmp[ 1 ];            
    if( dbgetb( gLsgFileHandler, &slot->VerRelease ) == -1 ) return -1;
    if( slot->VerMinor != 1 || slot->VerMajor != 2 || slot->VerRelease != 'R' ){
        gLsgError = 1;
        eprintf( "LOADSAVE: Load slot #%d Version: %d.%d%c", 
    		SlotNo, slot->VerMinor, slot->VerMajor, slot->VerRelease );
        return -1;
    }
    if( dbread( slot->PlayerName, 32, 1, gLsgFileHandler ) != 1 ) return -1;
    if( dbread( slot->SaveName, 30, 1, gLsgFileHandler ) != 1 ) return -1;
    if( dbreadBewBlk( gLsgFileHandler, tmp, 3 ) == -1 ) return -1;
    slot->RtcDay   = tmp[ 0 ];
    slot->RtcMonth = tmp[ 1 ];
    slot->RtcYear  = tmp[ 2 ];
    if( dbgetBei( gLsgFileHandler, &slot->RtcTime ) == -1 ) return -1;
    if( dbreadBewBlk( gLsgFileHandler, tmp, 3 ) == -1 ) return -1;
    slot->GMonth = tmp[ 0 ];
    slot->GDay   = tmp[ 1 ];
    slot->GYear  = tmp[ 2 ];
    if( dbgetBei( gLsgFileHandler, &slot->GTime  ) == -1 ) return -1;
    if( dbgetBew( gLsgFileHandler, &slot->MapLvl ) == -1 ) return -1;
    if( dbgetBew( gLsgFileHandler, &slot->MapNum ) == -1 ) return -1;
    if( dbread( slot->MapFname, 1, 16, gLsgFileHandler) != 16 ) return -1;
    if( dbseek( gLsgFileHandler, THUMBNAIL_SIZE, 1 ) ) return -1;
    if( dbseek( gLsgFileHandler, 128, 1 ) ) return -1;
    gLsgError = 0;                                                                            
    return 0;
}

int LsgLoadSlots()
{
    int i, FileLen;

    for( i = 0; i < 10; i++ ){
        sprintf( gLsgMsgPath, "%s/%s%.2d/%s", "savegame", "slot", i + 1, "save.dat" );
        if( dbCheckFileLength( gLsgMsgPath, &FileLen ) ){
            gLsgSaveFilesCondition[ i ] = 0; // file not exist
            continue;
        }
        if( !( gLsgFileHandler = dbOpen(gLsgMsgPath, "rb" ) ) ){
            eprintf( "LOADSAVE: ** Error opening save game for reading! **" );
            return -1;
        }
        if( LsgSlotLoad( i ) == -1 ){
            if( gLsgError == 1 ){
                eprintf( "LOADSAVE: ** save file #%d is an older version! **", gLsgSelectedSlotIdx );
                gLsgSaveFilesCondition[ i ] = 3;
            } else {
                eprintf( "LOADSAVE: ** Save file #%d corrupt! **", i );
                gLsgSaveFilesCondition[ i ] = 2;
            }
        } else {
            gLsgSaveFilesCondition[ i ] = 1;
        }
        dbClose( gLsgFileHandler );
    }
    return 10;
}

void LsgDrawSlotList( int a1 )
{
    int y,h, i, flg;
    unsigned char col;

    ScrFillSolid( gLsgSurf + 55735, 230, 353, 640, *(gLsgSurf + 55095) );
    flg = (a1 != 0);
    y = 87;
    for( i = 0; i < 10; i++ ){
        col = ( i == gLsgSelectedSlotIdx ) ? gPalColorCubeRGB[31][31][11] : gPalColorCubeRGB[0][31][0];
        sprintf( gLsgMsgPath, "[   %s %.2d:   ]", MessageGetMessage( &gLsgMsg, &gLsgMsgLine, flg + 109 ), i + 1 );
        gFont.Print( gLsgSurf + 640 * y + 55, gLsgMsgPath, 640, 640, col );
        h = gFont.ChrHeight() + y;
        switch( gLsgSaveFilesCondition[ i ] ){
            case 0: sprintf( gLsgMsgPath, "       %s", MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 111 ) ); break;
            case 1: strcpy( gLsgMsgPath, gLsgSlots[ i ].SaveName ); break;
            case 2: sprintf( gLsgMsgPath, "%s", MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 112 ) ); col = gPalColorCubeRGB[31][18][8]; break;
            case 3: sprintf( gLsgMsgPath, " %s", MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 113 ) ); col = gPalColorCubeRGB[31][18][8]; break;
        }
        gFont.Print( gLsgSurf + 640 * h + 55, gLsgMsgPath, 640, 640, col );
        y = 2 * gFont.ChrHeight() + 4 + h;
    }
}

void LsgDescription( int SlotNo )
{
    int h, t, y, i;
    char *s, c;
    short WrapLines[64], WrapLinesNo;
    unsigned char col;

    col = gPalColorCubeRGB[0][31][0];
    ScrCopy( gLsgImg[ 0 ] + 162956, 164, 60, 640, gLsgSurf + 162956, 640 );
    switch( gLsgSaveFilesCondition[ SlotNo ] ){
        case 0:
            gFont.Print( gLsgSurf + 168084, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 114 ), 640, 640, col );
            break;
        case 1:
            h = gFont.ChrHeight();
            gFont.Print( gLsgSurf + 162956, gLsgSlots[ SlotNo ].PlayerName, 640, 640, col );
            t = gLsgSlots[ SlotNo ].GTime / 600;
            sprintf( gLsgMsgPath, "%.2d %s %.4d   %.4d", 
        	gLsgSlots[ SlotNo ].GDay, 
        	MessageGetMessage( &gLsgMsg, &gLsgMsgLine, gLsgSlots[ SlotNo ].GMonth + 116 ), 
        	gLsgSlots[ SlotNo ].GYear, 
        	4 * (25 * (t / 60 % 24)) + (t % 60) 
    	    );
            gFont.Print( gLsgSurf + 640 * (h + 256) + 397, gLsgMsgPath, 640, 640, col );
            sprintf( gLsgMsgPath, "%s %s", MapGetCityName( gLsgSlots[ SlotNo ].MapNum ), MapCityLvlName( gLsgSlots[ SlotNo ].MapNum, gLsgSlots[ SlotNo ].MapLvl ) );
            if( !WinTextWrap( gLsgMsgPath, 164, WrapLines, &WrapLinesNo ) ){                
        	y = 2 * h + 3 + 256;
                for( i = 0; WrapLinesNo - 1 > i; y += h + 2, i++ ){
                    s = &gLsgMsgPath[ WrapLines[ i + 1 ] ];
                    c = *s;
                    *s = '\0';
                    gFont.Print( gLsgSurf + 640 * y + 399 + i * 14, &gLsgMsgPath[ WrapLines[ i ] ], 164, 640, col );
                    *s = c;                    
                }
            }
            break;
        case 2:
            gFont.Print( gLsgSurf + 168084, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 115 ), 640, 640, gPalColorCubeRGB[31][18][8] );
            break;
        case 3:
            gFont.Print( gLsgSurf + 168080, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 116 ), 640, 640, gPalColorCubeRGB[31][18][8] );
            break;
    }
}

int LsgReadThumbnail( int Idx )
{
    int cc;
    xFile_t *fh;

    cc = gLsgSaveFilesCondition[ gLsgSelectedSlotIdx ];
    if( cc && cc != 2 && cc != 3 ){
        sprintf( gLsgMsgPath, "%s/%s%.2d/%s", "savegame", "slot", gLsgSelectedSlotIdx + 1, "save.dat" );
        eprintf( " Filename %s", gLsgMsgPath );
        if( !(fh = dbOpen( gLsgMsgPath, "rb" ) ) ){
            eprintf( "LOADSAVE: ** (A) Error reading thumbnail #%d! **", Idx );
            return -1;
        }
        if( dbseek( fh, 131, 0 ) ){
            eprintf( "LOADSAVE: ** (B) Error reading thumbnail #%d! **", Idx );
            dbClose( fh );
            return -1;
        }
        if( dbread( gLsgThumbnailP, THUMBNAIL_SIZE, 1, fh ) != 1 ){
            eprintf( "LOADSAVE: ** (C) Error reading thumbnail #%d! **", Idx );
            dbClose( fh );
            return -1;
        }
        dbClose( fh );
    }
    return 0;
}

int LsgEditLine( int SlotNo )
{
    int win, w, bt, err;
    char stmp[260], *surf;

    if( (win = WinCreateWindow( 169, 116, gLsgGeo[1].Width, gLsgGeo[1].Height, 256, 20 )) == -1 ) return -1;
    surf = WinGetSurface( win );
    memcpy( surf, gLsgImg[1], gLsgGeo[1].Height * gLsgGeo[1].Width );
    FontSet(103);
    w = gLsgGeo[1].Width;
    gFont.Print( surf + 57 * gLsgGeo[1].Width + 56, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 104), w, w, gPalColorCubeRGB[18][17][3] );
    w = gLsgGeo[1].Width;
    gFont.Print( surf + 57 * gLsgGeo[1].Width + 181, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 105), w, w, gPalColorCubeRGB[18][17][3] );
    strcpy( stmp, MessageGetMessage( &gLsgMsg, &gLsgMsgLine, 130 ) );
    w = gLsgGeo[1].Width;
    gFont.Print( surf + ( gLsgGeo[1].Width - gFont.LineWidth( stmp ) ) / 2 + 7 * gLsgGeo[1].Width, stmp, w, w, gPalColorCubeRGB[18][17][3] );

    FontSet( 101 );
    bt = WinCreateButton( win, 34, 58, gLsgGeo[3].Width, gLsgGeo[3].Height, -1, -1, -1, 507, gLsgImg[4], gLsgImg[3], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt);
    bt = WinCreateButton( win, 160, 58, gLsgGeo[3].Width, gLsgGeo[3].Height, -1, -1, -1, 508, gLsgImg[4], gLsgImg[3], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    WinUpdate( win );
    if( gLsgSaveFilesCondition[ gLsgSelectedSlotIdx ] == 1 ) 
        strncpy( stmp, gLsgSlots[ SlotNo ].SaveName, 30 );
    else
        memset( stmp, 0, 30 );
    if( LsgEditLineHandler( win, 507, 508, stmp, 29, 24, 35, gPalColorCubeRGB[0][31][0], *WIN_XY( 24, 35, gLsgGeo[1].Width, gLsgImg[1] ), 0 ) ){
        err = 0;
    } else {
        strncpy( gLsgSlots[ SlotNo ].SaveName, stmp, 30 );
        err = 1;
        gLsgSlots[ SlotNo ].SaveName[ 29 ] = '\0';
    }
    WinClose( win );
    return err;
}

int LsgEditLineHandler( int WinId, int CodeKey1, int CodeKey2, char *str, int FieldLen, int Xpos, int Ypos, int ColorA, int ColorB, int flags )
{
    char stmp[260], *surf;
    int CursW, WinW, CursorPos, SysTime, sel, Blink, Cnt, ExtCode, BlinkCnt, FontH;

    CursW = gFont.LineWidth("_") - 4;
    ExtCode = 1;
    BlinkCnt = 3;
    Blink = 0;
    WinW = WinGetWidth( WinId );
    FontH = gFont.ChrHeight();
    Cnt = 0;
    surf = WinGetSurface( WinId );
    if( FieldLen > 255 ) FieldLen = 255;
    strncpy( stmp, str, FieldLen );
    CursorPos = strlen( stmp );
    stmp[ CursorPos + 1 ] = '\0';
    stmp[ CursorPos + 0 ] = ' ';    
    surf += WinW * Ypos + Xpos;    
    ScrFillSolid( surf, gFont.LineWidth( stmp ), FontH, WinW, ColorB );
    gFont.Print( surf, stmp, WinW, WinW, ColorA );
    WinUpdate( WinId );
    while( ExtCode == 1 ){
        SysTime = TimerGetSysTime();
        sel = InpUpdate();
        if( sel >= 0 ) Cnt++;
        if( sel == CodeKey1 || sel == KEY_ENTER ){
            ExtCode = 0;
        } else if( sel == KEY_ESC || sel == CodeKey2 ){
            ExtCode = -1;
        } else {
            if( (sel == 339 || sel == 8) && CursorPos >= 1 ){
                ScrFillSolid( surf, gFont.LineWidth( stmp ), FontH, WinW, ColorB );
                if( Cnt == 1 ) CursorPos = 1;
                stmp[ CursorPos - 0 ] = '\0';
                stmp[ CursorPos - 1 ] = ' ';
                gFont.Print( surf, stmp, WinW, WinW, ColorA );
                CursorPos--;
                WinUpdate( WinId );
            } else if( sel >= ' ' && sel <= 'z' && CursorPos < FieldLen ){
                if( (flags & 0x01) && !CharEditStrChrMark( sel ) ) break;
                ScrFillSolid( surf, gFont.LineWidth( stmp ), FontH, WinW, ColorB );
                stmp[ CursorPos + 0 ] = sel;
                stmp[ CursorPos + 1 ] = ' ';
                stmp[ CursorPos + 2 ] = 0;
                gFont.Print( surf, stmp, WinW, WinW, ColorA );
                CursorPos++;
                WinUpdate( WinId );
            }                
        }
        if( --BlinkCnt <= 0 ){
            BlinkCnt = 3;
            ScrFillSolid( surf + gFont.LineWidth( stmp ) - CursW, CursW, FontH - 2, WinW, Blink ? ColorB : ColorA );
            Blink = !Blink;
            WinUpdate( WinId );
        }
        TIMER_WAIT( SysTime, 41 );
    }    
    if( !ExtCode ){
        stmp[ CursorPos ] = '\0';
        strcpy( str, stmp );
    }
    return ExtCode;
}

int LsgFSaveNull( xFile_t *fh )
{
    return 0;
}

int LsgFLoadInit( xFile_t *fh )
{
    GameReset();
    GmouseLoadCursor( 25 );
    gMap.Name[ 0 ] = '\0';
    ScptSetGameTime( gLsgSlots[ gLsgSelectedSlotIdx ].GTime );
    return 0;
}

int LsgUnk47( xFile_t *fh )
{
    WmStartMapMusic();
    CritterSetName( gLsgSlots[gLsgSelectedSlotIdx].PlayerName );
    IfaceUnk09();
    IfaceIndicatorBoxUpdate();
    TileUpdate();
    if( IN_COMBAT ) ScptUnk121( 0 );
    return 0;
}

int LsgFSaveMaps( xFile_t *fh1 )
{
    int i, n, t;
    char **FileList, stmp[30], *s;
    xFile_t *fh2;
DD
    if( PartyLeaveAll() == -1 ) return -1;
    if( MapSavingRandomEncounter( 0 ) == -1 ) return -1;
    if( gPartyCount > 1 ){
DD
	for( i = 1; i < gPartyCount; i++ ){
    	    if( (t = gPartyPids[ i ]) == -2 ) continue;
    	    if( ProtoGetFName( gPartyPids[ i ], stmp ) ) continue;
    	    s = (t >> 24) == 1 ? "proto/critters" : "proto/items";
    	    sprintf( gLsgBakFileName, "%s/%s/%s", gLsgMasterPatches, s, stmp );
    	    sprintf( gLsgCurFileName, "%s/%s/%s%.2d/%s/%s", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1, s, stmp );
    	    if( FileDeflate( gLsgBakFileName, gLsgCurFileName ) == -1 ){
exit( -1 );
    	     return -1;

    	    }
	}
    }
    sprintf( gLsgBakFileName, "%s/*.%s", "maps", "sav" );
    if( (n = dbGetFileList( gLsgBakFileName, &FileList ) ) == -1 ) return -1;
    if( dbputBei( fh1, n ) == -1 ){ dbDelFileList( FileList ); return -1; }
    if( !n ){ dbDelFileList( FileList ); return -1; }
    sprintf( gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    if( LsgDeleteFiles( gLsgFileName, "sav" ) == -1 ){ dbDelFileList( FileList ); return -1; }
    sprintf( gLsgFileName, "%s/%s/%s%.2d/", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    strcpy( gLsgFileName + strlen( gLsgFileName ), CharEditFnameChgExt(gLsgBakFileName, "automap.db", "sav" ) );
    xFileRemove( gLsgFileName );
    for( i = 0; i < n; i++ ){
	if( dbwrite( FileList[ i ], strlen( FileList[ i ] ) + 1, 1, fh1 ) != 1 ){ dbDelFileList( FileList ); return -1; }
        sprintf( gLsgBakFileName, "%s/%s/%s", gLsgMasterPatches, "maps", FileList[ i ] );
        sprintf( gLsgCurFileName, "%s/%s/%s%.2d/%s", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1, FileList[ i ] );
        if( FileDeflate( gLsgBakFileName, gLsgCurFileName ) == -1 ) break;
    }
    dbDelFileList( FileList );    
    sprintf( gLsgCurFileName, "%s/%s/%s%.2d/%s", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1, CharEditFnameChgExt( gLsgBakFileName, "automap.db", "sav" ) );
    sprintf( gLsgBakFileName, "%s/%s/%s", gLsgMasterPatches, "maps", "automap.db" );
    if( FileDeflate( gLsgBakFileName, gLsgCurFileName) == -1 ) return -1;
    sprintf( gLsgBakFileName, "%s/%s", "maps", "automap.db" );
    if( !(fh2 = dbOpen( gLsgBakFileName, "rb" ) ) ) return -1;
    if( (n = dbFileLength( fh2 ) ) == -1 ){ dbClose( fh2 ); return -1; }
    dbClose( fh2 );
    if( dbputBei( fh1, n ) == -1 ) return -1;
    if( PartyRejoinAll() == -1 ) return -1;
    return 0;            
}

int LsgSlotMap2Game( xFile_t *fh )
{
    char stmp1[200], stmp2[260];
    int i, n;

    eprintf( "LOADSAVE: in SlotMap2Game" );
    if( dbgetBei( fh, &n ) == -1 ){ eprintf( "LOADSAVE: returning 1" ); return -1; }
    if( !n ){ eprintf( "LOADSAVE: returning 2" ); return -1; }
    sprintf( gLsgBakFileName, "%s/", "proto/critters" );
    if( LsgDeleteFiles( gLsgBakFileName, "pro" ) == -1 ){ eprintf( "LOADSAVE: returning 3" ); return -1; }
    sprintf( gLsgBakFileName, "%s/", "proto/items" );
    if( LsgDeleteFiles( gLsgBakFileName, "pro" ) == -1 ){ eprintf( "LOADSAVE: returning 4" ); return -1; }
    sprintf( gLsgBakFileName, "%s/", "maps" );
    if( LsgDeleteFiles( gLsgBakFileName, "sav" ) == -1 ){ eprintf( "LOADSAVE: returning 5" ); return -1; }
    sprintf( gLsgBakFileName, "%s/%s/%s", gLsgMasterPatches, "maps", "automap.db" );
    xFileRemove( gLsgBakFileName );                    
    for( i = 1; i < gPartyCount; i++ ){
	if( gPartyPids[ i ] == -2 ) continue;
        if( ProtoGetFName( gPartyPids[ i ], stmp1 ) ) continue;
        sprintf( gLsgBakFileName, "%s/%s/%s", gLsgMasterPatches, ( gPartyPids[ i ] >> 24) == 1 ? "proto/critters" : "proto/items", stmp1 );
        sprintf( gLsgCurFileName, "%s/%s/%s%.2d/%s/%s", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1, 
    		    ( gPartyPids[ i ] >> 24) == 1 ? "proto/critters" : "proto/items", stmp1 );
        if( FileInflateB( gLsgCurFileName, gLsgBakFileName ) == -1 ){ eprintf( "LOADSAVE: returning 6" ); return -1; }
    }                    
    for( i = 0; i < n; i++ ){
        if( LsgReadFname( stmp2, fh ) == -1 ) return -1;
        sprintf( gLsgBakFileName, "%s/%s/%s%.2d/%s", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1, stmp2 );
        sprintf( gLsgCurFileName, "%s/%s/%s", gLsgMasterPatches, "maps", stmp2 );
        if( FileInflateB( gLsgBakFileName, gLsgCurFileName ) == -1 ){ eprintf( "LOADSAVE: returning 7" ); return -1; }
    }
    sprintf( gLsgBakFileName, "%s/%s/%s%.2d/%s", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1, CharEditFnameChgExt( gLsgCurFileName, "automap.db", "sav" ) );
    sprintf( gLsgCurFileName, "%s/%s/%s", gLsgMasterPatches, "maps", "automap.db" );
    if( FileInflateA( gLsgBakFileName, gLsgCurFileName ) == -1 ){ eprintf( "LOADSAVE: returning 8" ); return -1; }
    sprintf( gLsgCurFileName, "%s/%s", "maps", "automap.db" );
    if( dbgetBei( fh, &i ) == -1 ){ eprintf( "LOADSAVE: returning 9" ); return -1; }
    if( MapLoadSAV( gLsgSlots[ gLsgSelectedSlotIdx ].MapFname ) == -1 ){ eprintf( "LOADSAVE: returning 13" ); return -1; }
    return 0;        
}

int LsgReadFname( char *str, xFile_t *fh )
{
    int c, i;

    i = 14;
    while( (c = dbgetc( fh )) != -1 ){
        i--;
        *str++ = tolower( c );
        if( i == -1 || c == '\0' ) return i ? 0 : 1;        
    }
    return 0;
}

int LsgFileCopy( char *fname1, char *fname2 )
{
    xFile_t *fh1, *fh2;
    int fsize, err, i, blocks, rest;
    void *buf;

    err = 0;    
    if( !( fh1 = dbOpen( fname1, "rb" ) ) ) return -1;
    if( (fsize = dbFileLength( fh1 ) ) == -1 ) return -1;
    if( !(fh2 = dbOpen( fname2, "wb" ) ) ){ dbClose( fh1 ); return -1; }
    if( !(buf = Malloc(65535)) ){ dbClose( fh1 ); dbClose( fh2 ); return -1; }
    blocks = fsize / 0xFFFF;
    for( i = 0; i < blocks; i++ ){
        if( dbread( buf, 0xFFFF, 1, fh1 ) != 1 ) break;
        if( dbwrite( buf, 0xFFFF, 1, fh2 ) != 1 ) break;
    }    
    if( i < blocks ){
	 err = -1;
    } else {
	rest = fsize % 0xFFFF;
	if( rest ){
    	    if( dbread( buf, rest, 1, fh1 ) != 1 ) err = -1;
	    if( !err ){ 
		if( dbwrite(buf, rest, 1, fh2 ) != 1 ) err = -1;
	    }
	}
    }
    dbClose( fh1 );
    dbClose( fh2 );
    Free( buf );
    return err;    
}

int LsgDeleteMapFiles()
{
    char tmp[ 260 ];
    sprintf( tmp, "%s/", "maps" );
    return LsgDeleteFiles( tmp, "sav" );
}

int LsgDeleteFiles( char *path, char *Extension )
{
    int i;
    char fpath[ 300 ];
    char **FileList;

    sprintf( fpath, "%s*.%s", path, Extension );
    i = dbGetFileList( fpath, &FileList );
    while( --i != -1 ){
        sprintf( fpath, "%s/%s%s", gLsgMasterPatches, path, FileList[ i ] );
        xFileRemove( fpath );
    }
    dbDelFileList( FileList );
    return 0;
}

int LsgDeleteFile( char *a1, char *a2 )
{
    char stmp[260];
    sprintf( stmp, "%s/%s%s", gLsgMasterPatches, a1, a2 );
    return ( xFileRemove( stmp ) == 0) - 1;
}

int LsgBackup()
{
    xFile_t *fh;
    char **FileList;
    int i, files;

    eprintf( "LOADSAVE: Backing up save slot files.." );
    sprintf( gLsgFileName, "%s/%s/%s%.2d/", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    strcpy( gLsgBakFileName, gLsgFileName );
    strcpy( gLsgBakFileName + strlen( gLsgBakFileName ), "save.dat" );
    CharEditFnameChgExt( gLsgCurFileName, gLsgBakFileName, "bak" );
    if( (fh = dbOpen( gLsgBakFileName, "rb") ) ){
        dbClose( fh );
        if( FileRename( gLsgBakFileName, gLsgCurFileName ) ) return -1;
    }
    sprintf( gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    sprintf( gLsgBakFileName, "%.250s*.%.3s", gLsgFileName, "sav" );
    if( (files = dbGetFileList( gLsgBakFileName, &FileList ) ) == -1 ) return -1;
    gLsgUnk55 = files;
    sprintf( gLsgFileName, "%s/%s/%s%.2d/", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    for( i = gLsgUnk55; --i != -1; ){
        strcpy( gLsgBakFileName, gLsgFileName );
        strcpy( gLsgBakFileName + strlen( gLsgBakFileName ), FileList[ i ] );
        CharEditFnameChgExt( gLsgCurFileName, gLsgBakFileName, "bak" );
        if( FileRename( gLsgBakFileName, gLsgCurFileName ) ){ dbDelFileList( FileList ); return -1; }
    }
    dbDelFileList( FileList );
    eprintf( "LOADSAVE: %d map files backed up.", files );
    sprintf( gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    sprintf( gLsgBakFileName, "%.250s/%.12s", gLsgFileName, CharEditFnameChgExt( gLsgFileDir, "automap.db", "sav" ) );
    sprintf( gLsgCurFileName, "%.250s/%.12s", gLsgFileName, CharEditFnameChgExt( gLsgFileDir, "automap.db", "bak") );
    gLsgUnk56 = 0;        
    if( (fh = dbOpen( gLsgBakFileName, "rb" ) ) ){
        dbClose( fh );
        if( LsgFileCopy( gLsgBakFileName, gLsgCurFileName ) == -1 ) return -1;
        gLsgUnk56 = 1;
    }
    return 0;
}

int LsgBackupRestore()
{
    char *s, **FileList;
    int files, i;

    eprintf( "LOADSAVE: Restoring save file backup..." );
    LsgEraseBadSlot();
    sprintf(gLsgFileName, "%s/%s/%s%.2d/", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1);
    strcpy( gLsgBakFileName, gLsgFileName );
    strcpy( gLsgBakFileName + strlen( gLsgBakFileName ), "save.dat" );
    CharEditFnameChgExt( gLsgCurFileName, gLsgBakFileName, "bak" );
    xFileRemove( gLsgBakFileName );
    if( FileRename(gLsgCurFileName, gLsgBakFileName) ){ LsgEraseBadSlot(); return -1; }
    sprintf(gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1);
    sprintf(gLsgBakFileName, "%.250s*.%.3s", gLsgFileName, "bak");
    files = dbGetFileList( gLsgBakFileName, &FileList );
    if( files == -1 ) return -1;        
    if( files != gLsgUnk55 ){ LsgEraseBadSlot(); return -1; }
    s = gLsgBakFileName;
    i = gLsgUnk55;
    sprintf( gLsgFileName, "%s/%s/%s%.2d/", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    while( --i != -1 ){
        strcpy( s, gLsgFileName );
        strcpy( s + strlen( s ), FileList[ i ] );
        CharEditFnameChgExt( gLsgCurFileName, s, "sav" );
        xFileRemove( gLsgCurFileName );
        if( FileRename( s, gLsgCurFileName ) ){ LsgEraseBadSlot(); return -1; }
    }
    dbDelFileList( FileList );
    if( !gLsgUnk56 ) return 0;
    sprintf( gLsgFileName, "%s/%s/%s%.2d/", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    strcpy( gLsgBakFileName, gLsgFileName );
    strcpy( &gLsgBakFileName[ strlen( gLsgBakFileName ) ], CharEditFnameChgExt( gLsgFileDir, "automap.db", "bak" ) );
    strcpy( gLsgCurFileName, gLsgFileName );
    strcpy( &gLsgCurFileName[ strlen( gLsgCurFileName ) ], CharEditFnameChgExt( gLsgFileDir, "automap.db", "sav" ) );
    if( FileRename( gLsgBakFileName, gLsgCurFileName ) ){  LsgEraseBadSlot();  return -1; }
    return 0;            
}

int LsgLoadPlayerPIN( xFile_t *fh )
{
    int tmp;

    if( dbgetBei(fh, &tmp) == -1 ) return -1;
    gObjDude->Pin = tmp;
    return 0;    
}

int LsgSavePlayerPIN( xFile_t *fh )
{
    return ( dbputBei( fh, gObjDude->Pin ) != -1 ) - 1;
}

int LsgEraseBadSlot()
{
    char *s, **FileList;
    int files;

    eprintf( "LOADSAVE: Erasing save(bad) slot..." );
    sprintf( gLsgFileName, "%s/%s/%s%.2d/", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    strcpy( gLsgBakFileName, gLsgFileName );
    strcpy( gLsgBakFileName + strlen( gLsgBakFileName ), "save.dat" );
    xFileRemove( gLsgBakFileName );
    sprintf( gLsgFileName, "%s/%s%.2d/", "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    sprintf( gLsgBakFileName, "%.250s*.%.3s", gLsgFileName, "sav" );
    if( (files = dbGetFileList( gLsgBakFileName, &FileList ) ) == -1 ) files = 0;
    s = gLsgBakFileName;
    sprintf( gLsgFileName, "%s/%s/%s%.2d/", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    while( --files != -1 ){
        strcpy( s, gLsgFileName );
        strcpy( s + strlen( s ), FileList[ files ] );
        xFileRemove( s );
    }
    dbDelFileList( FileList );
    sprintf( gLsgFileName, "%s/%s/%s%.2d/", gLsgMasterPatches, "savegame", "slot", gLsgSelectedSlotIdx + 1 );
    strcpy( gLsgBakFileName, gLsgFileName );
    strcpy( gLsgBakFileName + strlen( gLsgBakFileName ), CharEditFnameChgExt( gLsgCurFileName, "automap.db", "sav" ) );
    xFileRemove( gLsgBakFileName );
    return 0;
}

