#include "FrameWork.h"

int gMapperEnable = 0;
int gSysMapperEnable = 0;
int gGameIface = 0;

int GameSysInit( char *ProgName, int flag1, int Font, int Flags, int argc, char **argv )
{
    char stmp[260]; 
    char *lang; 

    if( GmemSetup() == -1 ) return -1;
    SysLoadConfiguration( flag1, argc, argv );
    gMapperEnable = ( flag1 == 1 );
    if( SysOpenDataBase() == -1 ){ SysUnloadConfiguration( 0 ); return -1; }
    WinSetTitle( ProgName );
    WinSetup( 1, Flags );
    FadeInit();
    if( CfgGetString( &gConfiguration, "system", "language", &lang ) == 1 ){
        if( !strcasecmp( lang, "french" ) ){
            KeySetLang( 1 );
        } else if( !strcasecmp( lang, "german" ) ){
            KeySetLang( 2 );
        } else if( !strcasecmp( lang, "italian" ) ){
            KeySetLang( 3 );
        } else if( !strcasecmp( lang, "spanish" ) ){
            KeySetLang( 4 );
        }
    }
    if( !gMapperEnable ) SysMapperInit();
    DbgInit();
    FontMgrInit();
    FontSetup( &gFontInit );
    FontSet( Font );
    InpScrShotInit( 390, SysSaveBmp );
    InpPauseInit( -1, 0 );
    TileUpdateDisable();
    RandomInit();
//    MessageBadWordsInit(); // wieszaja się
    SkillInit();
    FeatStatInit();
    if( PartyInit() ){ eprintf("Failed on partyMember_init\n"); return -1; } eprintf(">partyMember_init\t");
    PerkInit();
    TraitInit();
    ItemInit();
    EvQeInit();
    CritterInit();
    AiLoadCfg();
    InvSelectMain();
    if( GSoundInit() ) eprintf("Sound initialization failed."); 
    eprintf(">gsound_init\t");
    MveInit(); 
    eprintf( ">initMovie\t\t" );
    if( GMovieInit() ){ eprintf("Failed on gmovie_init"); return -1; } 
    eprintf(">gmovie_init\t");
    if( MveFxInit() ){ eprintf("Failed on moviefx_init"); return -1; } 
    eprintf(">moviefx_init\t");
    if( MapIsoInit() ){ eprintf("Failed on iso_init"); return -1; }  
    eprintf(">iso_init\t\t");
    if( GmouseInit() ){ eprintf("Failed on gmouse_init"); return -1; }  
    eprintf(">gmouse_init\t");
    if( ProtoInit() ){ eprintf("Failed on proto_init"); return -1; }
    eprintf(">proto_init\t\t");
    AnimInit(); eprintf(">anim_init\t\t");
    if( ScptInit() ){ eprintf("Failed on scpt_init"); return -1; }
    eprintf(">scpt_init\t\t");
    if( GlobVarLoad() ){ eprintf("Failed on game_load_info"); return -1; }
    eprintf(">game_load_info\t");
    if( ScptGameInit() ){ eprintf("Failed on scr_game_init"); return -1; }
    eprintf(">scr_game_init\t");
    if( WmWorldMapInit() ){ eprintf("Failed on wmWorldMap_init"); return -1; }
    eprintf(">wmWorldMap_init\t");
    CharEditInit(); eprintf(">CharEditInit\t");
    PipInit();  eprintf(">pip_init\t\t");
    LsgClean();
    LsgDeleteMapFiles(); 
    eprintf( ">InitLoadSave\t" );
    if( GdialogInit() ){ eprintf("Failed on gdialog_init"); return -1; }
    eprintf(">gdialog_init\t");
    if( CombatInit() ){ eprintf("Failed on combat_init"); return -1; }
    eprintf(">combat_init\t");
    if( AutomapInit() ){ eprintf("Failed on automap_init"); return -1; }
    eprintf(">automap_init\t");
    if( MessageInit( &gMessage ) != 1 ){ eprintf("Failed on message_init"); return -1; }
    eprintf(">message_init\t");
    sprintf(stmp, "%s%s", gGamePath, "misc.msg");
    if( MessageLoad( &gMessage, stmp ) != 1 ){ eprintf("Failed on message_load"); return -1; }
    eprintf(">message_load\t");
    if( ScptDisable() ){ eprintf("Failed on scr_disable"); return -1; }
    eprintf(">scr_disable\t");
    if( OptMenuInit() ){ eprintf("Failed on init_options_menu"); return -1; }
    eprintf( ">init_options_menu" );
    if( EndGameDeathEndingInit() ){ eprintf("Failed on endgameDeathEndingInit"); return -1; }
    eprintf(">endgameDeathEndingInit");
    return 0;        
}



void GameReset()
{
    TileUpdateDisable();
    RandFSave();
    SkillClear();
    FeatResetPoints();
    PerkZero();
    TraitSpecReset();
    EvQeClose();
    AnimInit_00();
    LsgDeleteMapFiles();
    CritterReset();
    AiReset();
    InvSelectMain();
    GSoundReset();
    MveAbort();
    MveUnk37();
    GMovieClose();
    MapIsoReset();
    GmouseReset();
    ProtoFreePremade();
    ScptUnk01();
    GlobVarLoad();
    ScptGameReset();
    WmUnk108();
    PartyUnk01();
    CharEditInit();
    PipInit();
    LsgPurgeFiles();
    GdialogClose();
    CombatReset();
    gMenuEscape = 0;
    AutomapReset();
    OptMenuInit();
}

void GameExit()
{
DD
/*
    eprintf( "\nGame Exit\n" );
    TileUpdateDisable();
    MessageClose( &gMessage );
    CombatClose();
    GdialogClose();
    ScptReset();
    gGValCount = 0;
    if( gGVals ){
        Free( gGVals );
        gGVals = NULL;
    }
    ScptClose();
    AnimClose();
    ProtoClose();
    GmouseClose();
    MapIsoClose();
    MveUnk38();
    MveClose();
    GSoundClose();
    AiClose();
    CritterClose();
    ItemClose();
    EvQeClose();
    PerkClose();
    FeatStatClose();
    SkillMsgClose();
    TraitClose();
    RandFSave();
    MessageBadWordsClear();
    AutomapClose();
    WmClose();
    PartyClose();
    EndGameClose();
    FontMgrClose();
    DbgInit();
    WinDlgClose();
    dbFree();
    SysUnloadConfiguration(1);
*/
}

int GameProcess( int sel, int a2 )
{
    Msg_t message;
    MsgLine_t msg;
    int tmp, mx, my, month, day, year;
    char stmp1[128], stmp2[128], stmp[100], Version[32];

    if( gGlobFloatMsg == 5 ) GdialogFloatingMessages();
    if( sel == -1 ) return 0; // no event
    if( sel == -2 ){
        tmp = MseGetButtons();
        MseGetCursorPosition( &mx, &my );
        if( tmp & MSE_LBUTT ){
            if( !(tmp & MSE_LBUTT_HOLD) ){            
        	if( mx == gVidMainGeo.lt || mx == gVidMainGeo.rt || my == gVidMainGeo.tp || my == gVidMainGeo.bm ) gGmouseUnk75 = 1;
            }
        } else if( tmp & MSE_LBHOLD ){
    	    gGmouseUnk75 = 0;
        }
        GmouseAction( mx, my, tmp ); // click on isometry
        return 0;
    }
    if( GmouseReachBorder() ) return 0;                

    switch( sel ){
	case  -20: if( IfaceGetUiState() ){ IfaceHandSlotSelect();  } break;
	case  9:  if( IfaceGetUiState() && !gKeyStates[56] && !gKeyStates[184] ){ GSoundPlay( "ib1p1xx1" ); AutomapScanner( 1, 0 ); } break; // [TAB] Automap
	case  12: GSoundPlay( "ib1p1xx1" ); if( LsgMenuGameLoad( 1 ) == -1 ){ eprintf( "\n ** Error calling LoadGame()! **\n" ); } break; // [CTRL-L]
	case  16: GSoundPlay( "ib1p1xx1" ); OptPauseWindow( 0 ); break; // [Ctrl-P] Pause
	case  17: case 24: GSoundPlay( "ib1p1xx1" ); SysQuitDlg(); break;
	case  19: GSoundPlay( "ib1p1xx1" ); if( LsgSaveGameMenu( 1 ) == -1 ) eprintf( "\n ** Error calling SaveGame()! **\n" ); break; // [CTRL-S]
	case  22: GSoundPlay( "ib1p1xx1" ); MainGameVersion( Version ); IfcMsgOut( Version ); IfcMsgOut( "Dec 11 1998 16:54:30" ); break; // 
	case '+': case '=': OptBrightInc(); break; // Increase Brightness
	case '-': case '_': OptBrightDec(); break; // Decrease Brightness
	case '/': case '?': // Show Date & Time
        	GSoundPlay( "ib1p1xx1" );
        	ScptGetGameDate( &month, &day, &year );
        	if( MessageInit( &message ) != 1 ) break;
        	sprintf( stmp1, "%s%s", gGamePath, "editor.msg" );
        	if( MessageLoad( &message, stmp1 ) == 1 ){
            	    msg.Id = 499 + month; // month names
            	    if( MessageGetMsg( &message, &msg ) == 1 ){
                	sprintf( stmp, "%s: %d/%d %s", msg.Text, day, year, ScptGetGameTimeStr() );
                	IfcMsgOut( stmp );
            	    }
        	}
        	MessageClose( &message );
        	break;                            
	case '1': case '!': if( IfaceGetUiState() ){ GSoundPlay( "ib1p1xx1" ); ActionUseSneak( SKILL_SNEAK ); } break; // use sneak
	case '2': case '@': if( IfaceGetUiState() ){ GSoundPlay( "ib1p1xx1" ); GmouseLoadCursor( 23 ); GmouseSetMode( 6 ); } break; // use use Lockpick
	case '3': case '#': if( IfaceGetUiState() ){ GSoundPlay( "ib1p1xx1" ); GmouseLoadCursor( 23 ); GmouseSetMode( 7 ); } break; // use Steal
	case '4': case '$': if( IfaceGetUiState() ){ GSoundPlay( "ib1p1xx1" ); GmouseLoadCursor( 23 ); GmouseSetMode( 8 ); } break; // use Traps
	case '5': case '%': if( IfaceGetUiState() ){ GSoundPlay( "ib1p1xx1" ); GmouseLoadCursor( 23 ); GmouseSetMode( 4 ); } break; // use First Aid
	case '6': case '^': if( IfaceGetUiState() ){ GSoundPlay( "ib1p1xx1" ); GmouseLoadCursor( 23 ); GmouseSetMode( 5 ); } break; // use Doctor
	case '7': case '&': if( IfaceGetUiState() ){ GSoundPlay( "ib1p1xx1" ); GmouseLoadCursor( 23 ); GmouseSetMode( 9 ); } break; // use Science
	case '8': case '*': if( IfaceGetUiState() ){ GSoundPlay( "ib1p1xx1" ); GmouseLoadCursor( 23 ); GmouseSetMode( 10); } break; // use Repair
	case '<': case ',': if( !AnimStart(2) ){ AnimTurnCCW( gObjDude ); AnimBegin(); } break; // rotate CCW
	case '>': case '.': if( !AnimStart(2) ){ AnimTurnCW( gObjDude ); AnimBegin(); } break; // rotate CW
	case 'A': case 'a': if( IfaceGetUiState() && a2 != 1 ){ CombatStart( NULL );  } break; // Attack
	case 'B': case 'b': if( IfaceGetUiState() ){ GSoundPlay("ib1p1xx1"); IfaceToggleItem( 1 ); } break; // toggle active items
	case 'C': case 'c': // character screen
		if( !IfaceGetUiState() ) break;    
    		GSoundPlay( "ib1p1xx1" );
    		tmp = MapAmbientDisable();
    		CharEditMenu( 0 );
    		if( tmp ) MapAmbientEnable();	    
		break;    
	case 'I': case 'i': if( IfaceGetUiState() ){ GSoundPlay( "ib1p1xx1" ); InvMenu(); } break; // inventory
	case 'M': case 'm': GmouseCycleMode(); break; // toggle mouse mode
	case 'N': case 'n': if( IfaceGetUiState() ){ GSoundPlay( "ib1p1xx1" ); IfaceUnk17(); } break; // toggle item mode
	case 'O': case 'o': case  KEY_ESC: if( IfaceGetUiState() ){ GSoundPlay( "ib1p1xx1" ); OptMenuInvoke(); } break; // options
	case 'P': case 'p': // PIPBoy 2000
    		if( !IfaceGetUiState() ) break;    
    		if( a2 ){
    		    GSoundPlay( "iisxxxx1" );
    		    strcpy( stmp2, MessageGetMessage( &gMessage, &msg, 7 ) );
    		    DlgBox( stmp2, 0, 0, 192, 116, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 0 );
    		} else {
    		    GSoundPlay( "ib1p1xx1" );
    		    PipMenu( 0 );
    		}        
    		break;    
	case 'S': case 's': // Skilldex
                if( !IfaceGetUiState() ) break;                
                GSoundPlay( "ib1p1xx1" );
                tmp = -1;
                switch( SkillDexMenu() ){
                    case -1: eprintf( "\n ** Error calling skilldex_select()! ** \n" ); break;
                    case 1:  ActionUseSneak( SKILL_SNEAK ); break;
                    case 2:  tmp = 6; break;
                    case 3:  tmp = 7; break;
                    case 4:  tmp = 8; break;
                    case 5:  tmp = 4; break;
                    case 6:  tmp = 5; break;
                    case 7:  tmp = 9; break;
                    case 8:  tmp = 10; break;
                    default: break;
                }
                if( tmp != -1 ){ GmouseLoadCursor( 23 ); GmouseSetMode( tmp ); }                
                break;
	case 'Z': case 'z': // PIPBoy Clock
    		if( !IfaceGetUiState() ) break;    
        	if( a2 ){
            	    GSoundPlay( "iisxxxx1" );
            	    strcpy( stmp1, MessageGetMessage( &gMessage, &msg, 7 ) );
            	    DlgBox( stmp1, 0, 0, 192, 116, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 0 );
        	} else {
            	    GSoundPlay( "ib1p1xx1" );
            	    PipMenu( 1 );
        	}        
    		break;    
	case 315: GSoundPlay( "ib1p1xx1" ); SysHelpDialog(); break;
	case 316: GSoundSetMasterVolume( GSoundGetMasterVolume() - 2047 ); break; // F2 volume Down
	case 317: GSoundSetMasterVolume( GSoundGetMasterVolume() + 2047 ); break; // F3 volume Up
	case 318: GSoundPlay( "ib1p1xx1" ); if( LsgSaveGameMenu(1) == -1 ) eprintf( "\n ** Error calling SaveGame()! **\n" ); break;
	case 319: 
    	    GSoundPlay( "ib1p1xx1" );
    	    if( LsgMenuGameLoad( 1 ) == -1 ){ eprintf( "\n ** Error calling LoadGame()! **\n" ); break; }
    	    break;    
	case 320: // fast save game
    	    GSoundPlay( "ib1p1xx1" );
    	    if( ( tmp = LsgSaveGameMenu( 2 ) ) >= -1 ){
        	if( tmp <= -1 ){ eprintf( "\n ** Error calling SaveGame()! **\n" ); break; }
    		if( tmp == 1 ){ IfcMsgOut( MessageGetMessage( &gMessage, &msg, 5 ) ); break; }
	    }
    	    break;    
	case 321: // fast load game
    	    GSoundPlay( "ib1p1xx1" );
    	    if( ( tmp = LsgMenuGameLoad( 2 ) ) >= -1 ){
        	if( tmp <= -1 ){ eprintf("\n ** Error calling LoadGame()! **\n"); break; }
    		if( tmp == 1 ){ IfcMsgOut( MessageGetMessage( &gMessage, &msg, 4 ) ); break; }
    	    }
    	    break;    
	case 324: GSoundPlay( "ib1p1xx1" ); SysQuitDlg(); break;
	case 327:
    	    if( gCurrentMapLvl != gObjDude->Elevation ) MapSetLvl( gObjDude->Elevation );
    	    if( gSysMapperEnable )
        	TileSetCenter( gObjDude->GridId, 1 );
    	    else
        	TileUnk49( gObjDude->GridId, 2 );
    	    break;    
	case 328: case 331: case 333: case 336: MapMove( 0, -1 ); break;
	default: break;
    }
    return 0;
}

void GameIfaceDisable( int IfaceMode )
{
    if( gGameIface ) return;    
    GmouseUnk03();
    GmouseSetIfaceMode(IfaceMode);
    KeySetHold();
    IfaceUiDisable();
    gGameIface = 1;
}

void GameIfaceEnable()
{
    if( !gGameIface ) return;    
    IfaceUiEnable();
    KeyClrHold();
    KeyFlush();
    GmouseSetIsoMode();
    GmouseIsoEnter();
    gGameIface = 0;    
}

int GameIfaceStat()
{
    return gGameIface;
}


