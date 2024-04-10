#include "FrameWork.h"

typedef struct
{
    char *Name;
    int Id;    
    int Stat;
} ChrEditKillStat_t;

typedef struct 
{
  int Id;
  char *Name;
} PerksList_t;

int gChrEditKarma1[ 38 ] = {
    47, 0, 48, 2, 49, 1, 50, 4, 51, 5, 52, 3, 53, 8, 54,
    6, 55, 7, 56, 13, 57, 10, 59, 11, 61, 14, 63, 17, 64,
    19, 65, 18, 66, 25, 294, 9, 308, 20
};

PerksList_t gChrEditPerksAvail[119];

int gChrEditTmpPerks;
char gChrEditNewPerkFlag;
Critter_t gChrEditSaveCritter;
int gChrEditPerkDisplOffset;
char *gChrEditPerkDlgSurf;
int gChrEditUnk46;
int gChrEditPerkWin;
char *gChrEditPerkImg;
int gChrEditUnk65;

int gChrEditUnk47;
int gChrEditTmpLvl;
int gChrEditCurLvl;

char gChrEditSaveName[32];
int gChrEditSaveHp;

int gChrEditKarmaIds1[ 8 ] = { 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x128, 0x127 };
int gChrEditKarmaIds2[ 8 ] = { 0x8E, 0x7E, 0x8C, 0x90, 0x91, 0x34, 0x88,  0x95  };

short gChrEditUnk127[ 10 ] = { 9, 8, 12, 11, 24, 32, 31, 13, 14, 15 };

short gChrEditUnk128[ 10 ] = { 18, 19, 20, 21, 22, 23, 83, 24, 25, 26 };

char gChrEditUnk30[ 260 ];


unsigned int gChrEditStatPosY[7] = { 37, 70, 103, 136, 169, 202, 235 };

int gChrEditImgIds[ 50 ] = {
    170, 175, 176, 181, 182, 183, 184, 185, 186, 187, 188,
    189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
    200, 8,   9,   204, 205, 206, 207, 208, 209, 210, 211, 
    212, 213, 214, 122, 123, 124, 125, 219, 220, 221, 222, 
    178, 179, 180, 38,  215, 216
};

char gChrEditImgCpyMap[ 50 ] = {
    0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0
};

int gCharEditDigBase[ 7 ] = { 1000000, 100000, 10000, 1000, 100, 10, 1 };

int gChrEditSkillList[ 19 ];
int gChrEditSkillSpecial[ 5 ];

int gChrEditUnk38;
int gChrEditFolderOffset;
int gChrEditUnk09;

int gChrEditUnspentSkillPt;

int    gChrEditFoldKarmaOffs = -1;
int    gChrEditFoldPerkOffs = -1;
int    gChrEditFoldKillsOffs = -1;
int    gChrEditFoldArrBtUp = -1;
int    gChrEditFoldArrBtDn = -1;

int gChrEditUnk39;
int gChrEditUnk40;
int gChrEditUnk41;

int gChrEditUnk70;

int gChrEditUnspentChrPts;
int gChrEditUnk79;
int gChrEditOptionTraits[3];
int gChrEditOptionalTraitsCnt;


int gChrEditUnspentChrPts = 0;
KarmaMsg_t *gCharEditKarma = NULL;
int gChrEditMsgKarmaCount = 0;

int gCharEditGenreCount = 0;
GenreMsg_t *gCharEditGenre = NULL;


int gChrEditTagSkill[ 4 ] = { 3, 4, 5 ,7 };
int gChrEditUnk18 = 0;
int gChrEditTime;
int gChrEditSelected = 0;
int gChrEditFolderType = 0;

int gCharEditMseX; 
int gCharEditMseY;

int gChrEditFontSave;
int gChrTagSkillUnspent;
int gChrEditUnk13;
int gChrEditUnk14;
int gChrEditOptTraits[4];
int gChrEditSavePerks[119]; // unk53,54,55,56,57
int gChrEditUnk17;
int gChrEditUnk18;
char gChrEditUnk20[48];


int gChrTagSkillUnspent = 0;
int gChrEditSkillSelect = 0;
int gChrEditSkillSelPosY = 0;
int gChrEditTaggedSkillsCnt = 0;
Geo_t gChrEditImgGeo[50];
CachePool_t *gChrEditImg[50];

char *gChrEditImgPixMaps[50];

char *gChrEditPix[50];
char *gChrEditSurface;
MsgLine_t gChrEditList;

int gChrEditSkillPlus;
int gChrEditSkillMinus;
int gChrEditUnk02[7];

int gChrEditUnk06[7];

char gChrEditUnk19[48];
int gChrEditUnk58[18];

int gChrEditUnk59[8];
int gChrEditUnk60[8];

int gChrEditWin;
Msg_t gChrEditMsg;
char *gChrEditWpPix;
CachePool_t *gChrEditWpImg;
int gChrEditMenuMode = 0;
int gCharEditTime;

int gChrEditDscImgId;
char *gChrEditDscParams;
char *gChrEditDscTitle;
char *gChrEditDscDsc;


/**********************************************************/

#define CHRED_STATES_A( MsgId, Id, cond ) \
    y += gFont.ChrHeight() + 3;\
    strcpy( str, MessageGetMessage( &gChrEditMsg, &gChrEditList, MsgId ) );\
    if( gChrEditSelected == Id ){\
	col = ( cond ) ? CHREDT_COL_LVL_C : CHREDT_COL_LVL_A;\
    } else {\
	col = ( cond ) ? CHREDT_COL_LVL_D : CHREDT_COL_LVL_B;\
    }\
    gFont.Print( gChrEditSurface + 640 * y + 194, str, 640, 640, col );

#define CHRED_STATES_B( yoffs, Sel, MsgId, Fmt, ObjId, ColB )	\
    y += gFont.ChrHeight() + yoffs;\
    col = ( gChrEditSelected == Sel ) ? CHREDT_COL_LVL_A : CHREDT_COL_LVL_B;\
    strcpy( str, MessageGetMessage( &gChrEditMsg, &gChrEditList, MsgId ) ); \
    gFont.Print( gChrEditSurface + 640 * y + 194, str, 640, 640, col );\
    sprintf( str, Fmt, FeatGetVal( gObjDude, ObjId ) ); \
    gFont.Print( gChrEditSurface + 640 * y + 288, str, 640, 640, ColB );

static int CharEditPerkSortCb( PerksList_t *s1, PerksList_t *s2 );
static int CharEditSortGenreCb( GenreMsg_t *gen1, GenreMsg_t *gen2 );
static int CharEditKarmaSortCb( KarmaMsg_t *kr1, KarmaMsg_t *kr2 );


int CharEditMenu( int mode )
{
    int ExCode, sel, st, done;
    char stmp2[128], stmp1[128], *pstr = stmp2;    

    ExCode = -1;
    gChrEditMenuMode = mode;
    CharEditStoreObj();

    if( CharEditMenuCreate() == -1 ){ eprintf( "\n ** Error loading character editor data! **\n" ); return -1; }

    if( (gChrEditMenuMode == 0) && CharEditLvlUp() ){
        FeatStatsRecalculate( gObjDude );
        CharEditShowTraits();
        CharEditShowSkills( 0 );
        CharEditShowStats( 7, 0, 0 );
        CharEditShowParams();
        CharEditDscUpdate( );
    }
    while( ExCode == -1 ){
        gCharEditTime = TimerGetSysTime();
        sel = InpUpdate();
        done = 0;
        if( sel == 500 ) done = 1;
        if( sel == '\r' || sel == 'd' || sel == 'D' ){ done = 1; GSoundPlay( "ib1p1xx1" ); }
        if( done ){
            if( gChrEditMenuMode != 1 ) { ExCode = 0; WinUpdate( gChrEditWin ); continue; }
            if( gChrEditUnspentChrPts ){ // 'You must use all character points before starting the game!' 
                GSoundPlay( "iisxxxx1" );
                strcpy( stmp1, MessageGetMessage( &gChrEditMsg, &gChrEditList, 118 ) );
                strcpy( stmp2, MessageGetMessage( &gChrEditMsg, &gChrEditList, 119 ) );
                done = 1; ExCode = -1;
                DlgBox( stmp1, &pstr, 1, 192, 126, CHREDT_COL_LVL_E, NULL, CHREDT_COL_LVL_E, 0 );
                WinUpdate( gChrEditWin );
                continue;
            }
            if( gChrEditTaggedSkillsCnt > 0 ){ // 'You must select all tag skills before starting the game!' 
                GSoundPlay( "iisxxxx1" );
                strcpy( stmp1, MessageGetMessage(&gChrEditMsg, &gChrEditList, 142));
                strcpy( stmp2, MessageGetMessage(&gChrEditMsg, &gChrEditList, 143));
                done = 1; ExCode = -1;
                DlgBox( stmp1, &pstr, 1, 192, 126, CHREDT_COL_LVL_E, NULL, CHREDT_COL_LVL_E, 0 );
                WinUpdate( gChrEditWin );
                continue;
            }
            if( CharEditStatOverFlow() ){ // 'All stats must be between 1 and 10 before starting the game!' 
                GSoundPlay( "iisxxxx1" );
                strcpy( stmp1, MessageGetMessage( &gChrEditMsg, &gChrEditList, 157 ) );
                strcpy( stmp2, MessageGetMessage( &gChrEditMsg, &gChrEditList, 158 ) );
                done = 1; ExCode = -1;
                DlgBox( stmp1, &pstr, 1, 192, 126, CHREDT_COL_LVL_E, NULL, CHREDT_COL_LVL_E, 0 );
                WinUpdate( gChrEditWin );
                continue;
            } else {
                if( !strcasecmp( CritterGetName( gObjDude ), "None") ){ // 'Warning: You haven't changed your player name. Use this character anyway?'
            	    GSoundPlay( "iisxxxx1" );
            	    strcpy( stmp1, MessageGetMessage( &gChrEditMsg, &gChrEditList, 160) );
            	    strcpy( stmp2, MessageGetMessage( &gChrEditMsg, &gChrEditList, 161 ) );
            	    done = 1;
            	    ExCode = ( DlgBox(stmp1, &pstr, 1, 192, 126, CHREDT_COL_LVL_E, NULL, CHREDT_COL_LVL_E, 16) ) ? 0 : -1;
                } else {
            	    ExCode = 0;
		}
                WinUpdate( gChrEditWin );
            }
        } else if( sel == 17 || sel == 24 || sel == 324 ){
            SysQuitDlg();
            WinUpdate( gChrEditWin );
        } else if( sel == 27 || sel == 502 || sel == 99 || sel == 67 || gMenuEscape ){ // Cancel
            ExCode = 1;
            WinUpdate( gChrEditWin );
        } else if( gChrEditMenuMode == 1 && (sel == 519 || sel == 65 || sel == 97) ){ // character Age button
            CharEditDialogAge();
            WinUpdate( gChrEditWin );
        } else if( gChrEditMenuMode == 1 && (sel == 517 || sel == 78 || sel == 110) ){ // character Name button
            CharEditDialogName();
            WinUpdate( gChrEditWin );
        } else if( gChrEditMenuMode == 1 && (sel == 520 || sel == 83 || sel == 115) ){ // character Gender button
            CharEditDialogGender();
            WinUpdate( gChrEditWin );
        } else if( gChrEditMenuMode == 1 && sel >= 503 && sel < 517 ){ // character stat modify +/-
            CharEditStatModify( sel );
            WinUpdate( gChrEditWin );
        } else if( (sel == 501) || ( (gChrEditMenuMode == 1) && ((sel == 79 || sel == 111) || !gChrEditMenuMode) && (sel == 80 || sel == 112) ) ){ // print/options
            CharEditPrint();
            WinUpdate( gChrEditWin );
        } else if( sel < 525 || sel > 534 ){
            switch( sel ){
                case 535:   CharEditFolderSelect();   WinUpdate( gChrEditWin ); continue; // Folder select
                case 17000: CharEditFolderUpDn( -1 ); WinUpdate( gChrEditWin ); continue; // Folder arrow Up
                case 17001: CharEditFolderUpDn( 1  ); WinUpdate( gChrEditWin ); continue; // Folder arrow Dn
                case 521: case 523: CharEditSkillUpDn( sel );  WinUpdate( gChrEditWin ); continue; // +/- skills
	    }
    	    st = gChrEditSelected;
            switch( sel ){
                case 331: case '-': case 'J':
            	    if( gChrEditSelected > 6 ){
                	if( gChrEditSelected >= '=' && gChrEditSelected < 'O' ){
                    	    if( gChrEditMenuMode != 1 ) 
                    		CharEditSkillUpDn( sel );
                    	    else
                    		WinUnk970( gChrEditUnk02[gChrEditSelected] );
                	} else {
                	    if( gChrEditSelected >= 82 && gChrEditSelected < 98 && gChrEditMenuMode == 1 ) WinUnk970( gChrEditPerksAvail[ (gChrEditSelected + 172)/2 ].Id );
                	}
            	    } else {
            		if( gChrEditMenuMode == 1 ) WinUnk970( gChrEditUnk02[ gChrEditSelected ] );
            	    }
            	    WinUpdate( gChrEditWin );
                    continue;
                case 333: case '+': case 'N':
                    if( gChrEditSelected > 6 ){
                        if( gChrEditSelected < 61 || gChrEditSelected >= 79 ){
                            if( gChrEditSelected >= 82 && gChrEditSelected < 98 && gChrEditMenuMode == 1 ) WinUnk970( gChrEditPerksAvail[ (gChrEditSelected + 172)/2 ].Id );
                        } else {
                            if( gChrEditMenuMode != 1 ) 
                        	CharEditSkillUpDn( sel );
                    	    else
                            	WinUnk970( gChrEditPerksAvail[ (gChrEditSelected + 237)/2 ].Id );
                        }
                    } else {
                        if( gChrEditMenuMode == 1 ) WinUnk970( gChrEditUnk06[ gChrEditSelected ] );
                    }
                    WinUpdate( gChrEditWin );
                    continue;
                case 9:
            	    if( gChrEditSelected <= 6 ){
                    	gChrEditSelected = ( gChrEditMenuMode == 1 ) ? 82 : 7;
                    	break;
                    }
                    if( gChrEditSelected >= 7 && gChrEditSelected < 9 ){
                    	if( gChrEditMenuMode != 1 ){
                    	    gChrEditSelected = 10;
                    	    gChrEditFolderType = 0;
                    	} else {
                    	    gChrEditSelected = 82;
                    	}
                    	break;
                    }
                    if( gChrEditSelected < 10 || gChrEditSelected >= 43 ){
                	if( gChrEditSelected < 82 || gChrEditSelected >= 98 ){
                    	    if( gChrEditSelected < 43 || gChrEditSelected >= 51 ){
                    		if( gChrEditSelected < 51 || gChrEditSelected >= 61 ){
                            	    gChrEditSelected = 0;
                    		} else {
                            	    gChrEditSelected = 61;
                            	    gChrEditSkillSelect = 0;
                    		}
                    	    } else {
                    		gChrEditSelected = 51;
                    	    }
                    	    break;
                	}
                    } else {
                        if( !gChrEditFolderType ){
                            gChrEditFolderType = 1;
                            gChrEditSelected = 10;
                            break;
                        }
                        if( gChrEditFolderType == 1 ){
                            gChrEditSelected = 10;
                            gChrEditFolderType = 2;
                            break;
                        }
                    }
            	    gChrEditSelected = 43;
            	    break;
                case 328:
                    if( gChrEditSelected < 10 || gChrEditSelected >= 43 ){
                        if( gChrEditSelected ){
                            switch( gChrEditSelected ){
                                case 7:  gChrEditSelected = 9; break;
                                case 43: gChrEditSelected = 50; break;
                                case 51: gChrEditSelected = 60; break;
                                case 61: gChrEditSelected = 78; break;
                                case 82: gChrEditSelected = 97; break;
                                default: gChrEditSelected--; break;
                            }
                        } else {
                            gChrEditSelected = 6;
                        }
                        if( gChrEditSelected >= '=' && gChrEditSelected < 'O' ) gChrEditSkillSelect = gChrEditSelected - '=';
                        break;
                    }
                    if( gChrEditSelected == 10 ){
                        if( gChrEditFolderOffset <= 0 ){ WinUpdate( gChrEditWin ); continue; }
                        CharEditFolderUpDn(-1);
                        st = gChrEditSelected;
                    }
                    gChrEditSelected = st - 1;
                    CharEditShowFolder();
                    CharEditDscUpdate();
                    WinUpdate( gChrEditWin );
                    continue;
                case 336:
                    if( gChrEditSelected < 10 || gChrEditSelected >= 43 ){
                        switch( gChrEditSelected ){
                            case 6 : gChrEditSelected = 0;  break;
                            case 9 : gChrEditSelected = 7;  break;
                            case 50: gChrEditSelected = 43; break;
                            case 60: gChrEditSelected = 51; break;
                            case 78: gChrEditSelected = 61; break;
                            case 97: gChrEditSelected = 82; break;
                            default: gChrEditSelected++; break;
                        }
                        if( gChrEditSelected >= 61 && gChrEditSelected < 79 ) gChrEditSkillSelect = gChrEditSelected - 61;
                        break;
                    } else {
                        if( (gChrEditSelected - 10) >= gChrEditUnk38 - gChrEditFolderOffset ){ WinUpdate( gChrEditWin ); continue; }
                        if( (gChrEditSelected - 10) == gChrEditUnk09 - 1 ) CharEditFolderUpDn(1);
                        gChrEditSelected++;
                        CharEditShowFolder();
                    }
                    CharEditDscUpdate();
                    WinUpdate( gChrEditWin );
                    continue;
		case 536 ... 554: //  edit mode tag Skills select
                    if( gChrEditMenuMode == 1 ) CharEditTagSkillChoose( sel - 536 );
                    WinUpdate( gChrEditWin ); 
                    continue;
                case 555 ... 571: // edit mode spec Traits select
                    if( gChrEditMenuMode == 1 ) CharEditSpecTraitChoose( sel - 555 ); 
                    WinUpdate( gChrEditWin );
            	    continue;
                default:
                    if( sel == 390 ) InpScrShotTake();
                    WinUpdate( gChrEditWin );			                                
                    continue;
            }
            done = 0;
            CharEditShowStats( 7, 0, 0 );
            CharEditShowTraits();
            CharEditShowSkills( 0 );
            CharEditShowLevelInfo();
            CharEditShowFolder();
            CharEditShowParams();
        } else {
            CharEditAreaSelect( sel );
            WinUpdate( gChrEditWin );
        }
    }
    if( !ExCode && mode == 1 ){
        ProtoDudeImgInit();
        FadeStep(gFadePaletteC);
    }
    CharEditClose();
    if( ExCode == 1 ) CharEditRestoreObj();
    if( CritterUsingSkill( 3 ) ) CritterUnk36( 3 );
    IfaceRenderHP( 0 );
    return ExCode;
}

int CharEditMenuCreate()
{
    int i, win, bt, y, ht1, ht2, m, err;
    char stmp0[512], stmp1[512], stmp2[512], *p;

    gChrEditFontSave = FontGetCurrent();
    gChrTagSkillUnspent = 0;
    gChrEditUnk13 = 0;
    gChrEditUnk14 = -1;
    gChrEditOptTraits[0] = -1;
    gChrEditSavePerks[0] = 0;
    gChrEditUnk17 = 0;
    gChrEditUnk18 = 1;
    gChrEditUnk19[0] = 0;
    gChrEditUnk20[0] = 0;

    FontSet( 101 );
    gChrEditSkillSelPosY = gChrEditSkillSelect * ( gFont.ChrHeight() + 1 ) + 27;
    SkillGetSpecials( gChrEditTagSkill, 4 );
    for( i = 0; i < 4; i++ ){
        if( gChrEditTagSkill[ 3 - i ] != -1 ) break;
    }
    if( gChrEditMenuMode == 1 ) i--;
    gChrEditTaggedSkillsCnt = i; // tagged skills

    TraitGetSpec( &gChrEditOptionTraits[0], &gChrEditOptionTraits[1] );
    for( i = 0; i < 2; i++ ){
        if( gChrEditOptionTraits[ 1 - i ] != -1 ) break;
    }
    gChrEditOptionalTraitsCnt = i; // optioned traits

    if( gChrEditMenuMode == 0 ) gChrEditUnk13 = MapAmbientDisable();

    CycleColorStop();
    GmouseLoadCursor( 1 );
    if( MessageInit( &gChrEditMsg ) != 1 ) return -1;
    sprintf( stmp1, "%s%s", gGamePath, "editor.msg" );
    if( MessageLoad( &gChrEditMsg, stmp1 ) != 1 ) return -1;
    gChrEditWpPix = ArtLoadBmp( ArtMakeId( 6, ( gChrEditMenuMode == 1 ) ? 169 : 177, 0, 0, 0 ), &gChrEditWpImg, &gChrEditImgGeo[0].Width, &gChrEditImgGeo[0].Height );
    if( !gChrEditWpPix ){ MessageClose( &gChrEditMsg ); return -1; }
    if( CharEditLoadKarma() == -1 ) return -1;
    if( CharEditLoadGenre() == -1 ) return -1;
    SoundUpdateAll();

    for( i = 0; i < 50; i++ ){
        if( !(gChrEditPix[ i ] = ArtLoadBmp( ArtMakeId( 6, gChrEditImgIds[ i ], 0, 0, 0 ), &gChrEditImg[i], &gChrEditImgGeo[i].Width, &gChrEditImgGeo[i].Height ) ) ) break;
    }
    if( i < 50 ){
	for( i--; i; i-- ){
            ArtClose( gChrEditImg[ i ] );
        }
        ArtClose( gChrEditWpImg );
        MessageClose( &gChrEditMsg );
        if( gChrEditUnk13 ) MapAmbientEnable();
        CycleColorStart(); GmouseLoadCursor( 1 ); return -1;
    }
    SoundUpdateAll();
    err = 0;
    for( i = 0; i < 50; i++ ){
        if( err ) break;
        if( gChrEditImgCpyMap[ i ] ){
            p = Malloc( gChrEditImgGeo[ i ].Height * gChrEditImgGeo[ i ].Width );
            gChrEditImgPixMaps[ i ] = p;
            if( p )
                memcpy( p, gChrEditPix[ i ], gChrEditImgGeo[ i ].Width * gChrEditImgGeo[ i ].Height );
            else
                err = 1;
        } else {
            gChrEditImgPixMaps[ i ] = (void *)-1;
        }
    }

    if( err ){
        for( i--; i; i-- ){
    	    if( gChrEditImgCpyMap[ i ] ) Free( gChrEditImgPixMaps[ i ] );
    	    ArtClose( gChrEditImg[ i ] );
        }
        ArtClose( gChrEditWpImg );
        MessageClose( &gChrEditMsg );
        if( gChrEditUnk13 ) MapAmbientEnable();
        CycleColorStart(); GmouseLoadCursor( 1 ); return -1;
    }

    win = WinCreateWindow(0, 0, 640, 480, 256, 18);

    if( (gChrEditWin = win) == -1 ){
        for( i = 0; i < 50; i++ ){
            ArtClose( gChrEditImg[ i ] );
    	    if( gChrEditImgCpyMap[ i ] ) Free( gChrEditImgPixMaps[ i ] );
        }
        ArtClose( gChrEditWpImg );
        MessageClose( &gChrEditMsg );
        if( gChrEditUnk13 ) MapAmbientEnable();
        CycleColorStart(); GmouseLoadCursor( 1 ); return -1;
    }
    gChrEditSurface = WinGetSurface( win );
    memcpy( gChrEditSurface, gChrEditWpPix, 307200 );

    if( gChrEditMenuMode ){
        FontSet( 103 );
        gFont.Print(gChrEditSurface + 183054, MessageGetMessage(&gChrEditMsg, &gChrEditList, 116 ), 640, 640, CHREDT_COL_SELECT );
        CharEditCounterDisplay(126, 282, 0, gChrEditUnspentChrPts, 0, gChrEditWin );
        gFont.Print(gChrEditSurface + 290923, MessageGetMessage(&gChrEditMsg, &gChrEditList, 101), 640, 640, CHREDT_COL_SELECT );
        gFont.Print(gChrEditSurface + 208692, MessageGetMessage(&gChrEditMsg, &gChrEditList, 139), 640, 640, CHREDT_COL_SELECT );
        CharEditCounterDisplay( 522, 228, 0, gChrEditUnk79, 0, gChrEditWin );
        gFont.Print(gChrEditSurface + 149542, MessageGetMessage(&gChrEditMsg, &gChrEditList, 138), 640, 640, CHREDT_COL_SELECT );
        CharEditCounterDisplay(522, 228, 0, gChrEditTaggedSkillsCnt, 0, gChrEditWin);
    } else {
        FontSet( 103 );
        strcpy( stmp1, MessageGetMessage( &gChrEditMsg, &gChrEditList, 109 ) ); // 'Perks'
        strcpy( stmp0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 110 ) ); // 'Karma'
        strcpy( stmp2, MessageGetMessage( &gChrEditMsg, &gChrEditList, 111 ) ); // 'Kills'
	// Perk
        gFont.Print( (gChrEditImgPixMaps[46] + 5 * gChrEditImgGeo[46].Width + 61  - gFont.LineWidth( stmp1 ) / 2 ), stmp1, gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Width, CHREDT_COL_SELECT );
        gFont.Print( (gChrEditImgPixMaps[46] + 6 * gChrEditImgGeo[46].Width + 159 - gFont.LineWidth( stmp0 ) / 2 ), stmp0, gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Width, CHREDT_COL_DIMMED );
        gFont.Print( (gChrEditImgPixMaps[46] + 6 * gChrEditImgGeo[46].Width + 257 - gFont.LineWidth( stmp2 ) / 2 ), stmp2, gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Width, CHREDT_COL_DIMMED );
	// Karma
        gFont.Print( (gChrEditImgPixMaps[44] + 6 * gChrEditImgGeo[46].Width + 61  - gFont.LineWidth( stmp1 ) / 2 ), stmp1, gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Width, CHREDT_COL_DIMMED );
        gFont.Print( (gChrEditImgPixMaps[44] + 5 * gChrEditImgGeo[46].Width + 159 - gFont.LineWidth( stmp0 ) / 2 ), stmp0, gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Width, CHREDT_COL_SELECT );
        gFont.Print( (gChrEditImgPixMaps[44] + 6 * gChrEditImgGeo[46].Width + 257 - gFont.LineWidth( stmp2 ) / 2 ), stmp2, gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Width, CHREDT_COL_DIMMED );
	// Kills
        gFont.Print( (gChrEditImgPixMaps[45] + 6 * gChrEditImgGeo[46].Width + 61  - gFont.LineWidth( stmp1 ) / 2 ), stmp1, gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Width, CHREDT_COL_DIMMED );
        gFont.Print( (gChrEditImgPixMaps[45] + 6 * gChrEditImgGeo[46].Width + 159 - gFont.LineWidth( stmp0 ) / 2 ), stmp0, gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Width, CHREDT_COL_DIMMED );
        gFont.Print( (gChrEditImgPixMaps[45] + 5 * gChrEditImgGeo[46].Width + 257 - gFont.LineWidth( stmp2 ) / 2 ), stmp2, gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Width, CHREDT_COL_SELECT );
        CharEditShowFolder();
        FontSet( 103 );
        gFont.Print( gChrEditSurface + 290923, MessageGetMessage( &gChrEditMsg, &gChrEditList, 103 ), 640, 640, CHREDT_COL_SELECT ); // print
        CharEditShowLevelInfo();
        CharEditFolderArrows();
    }

    FontSet( 103 );
    gFont.Print( gChrEditSurface + 291131, MessageGetMessage( &gChrEditMsg, &gChrEditList, 102 ), 640, 640, CHREDT_COL_SELECT ); // cancel
    gFont.Print( gChrEditSurface + 291035, MessageGetMessage( &gChrEditMsg, &gChrEditList, 100 ), 640, 640, CHREDT_COL_SELECT ); // done

    CharEditShowStats( 7, 0, 0 );
    CharEditShowParams();

    if( gChrEditMenuMode == 0 ){
        gChrEditSkillPlus  = WinCreateButton( gChrEditWin, 614, 20, gChrEditImgGeo[16].Width, gChrEditImgGeo[16].Height, -1, 522, 521, 522, gChrEditPix[15], gChrEditPix[16], 0, 96 );
        gChrEditSkillMinus = WinCreateButton( gChrEditWin, 614, gChrEditImgGeo[14].Height + 19, gChrEditImgGeo[14].Width, gChrEditImgGeo[13].Height, -1, 524, 523, 524, gChrEditPix[13], gChrEditPix[14], 0, 96 );
        WinSetClickSound( gChrEditSkillPlus, GSoundPlayPushBt, 0);
        WinSetClickSound( gChrEditSkillMinus, GSoundPlayPushBt, 0 );
    }
    CharEditShowSkills( 0 );
    CharEditDscUpdate();
    SoundUpdateAll();
    CharEditShowHeroName();
    CharEditShowHeroAge();
    CharEditShowHeroGender();

    if( gChrEditMenuMode == 1 ){
        y = 26;
        bt = WinCreateButton( gChrEditWin, 9, 0, gChrEditImgGeo[6].Width, gChrEditImgGeo[6].Height, -1, -1, -1, 517, gChrEditImgPixMaps[7], gChrEditImgPixMaps[6], 0, 32 );
        if( bt != -1 ){
            WinSetDefaultImg( bt, gChrEditPix[ 5 ] );
            WinSetClickSound( bt, (void *)GSoundPlayKnobLt, 0 );
        }
        bt = WinCreateButton( gChrEditWin, gChrEditImgGeo[6].Width + 9, 0, gChrEditImgGeo[25].Width, gChrEditImgGeo[25].Height, -1, -1, -1, 519, (char *)gChrEditImgPixMaps[2], (char *)gChrEditImgPixMaps[25], 0, 32);
        if( bt != -1 ){
            WinSetDefaultImg( bt, gChrEditPix[ 1 ] );
            WinSetClickSound( bt, (void *)GSoundPlayKnobLt, 0 );
        }
        bt = WinCreateButton( gChrEditWin, gChrEditImgGeo[25].Width + gChrEditImgGeo[6].Width + 9, 0, gChrEditImgGeo[11].Width, gChrEditImgGeo[11].Height, -1, -1, -1, 520, (char *)gChrEditImgPixMaps[10], (char *)gChrEditImgPixMaps[11], 0, 32);
        if( bt != -1 ){
            WinSetDefaultImg( bt, gChrEditPix[ 9 ] );
            WinSetClickSound( bt, (void *)GSoundPlayKnobLt, 0 );
        }        
        for( i = 0; i < 18; i++, y += gChrEditImgGeo[49].Height ){
            bt = WinCreateButton( gChrEditWin, 347, y, gChrEditImgGeo[49].Width, gChrEditImgGeo[49].Height, -1, -1, -1, 536 + i, gChrEditPix[48], gChrEditPix[49], 0, 32);
            gChrEditUnk58[ i ] = bt;
            if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, 0 );
        }        
        y = 352;
        for( i = 0; i < 8; i++, y += gChrEditImgGeo[49].Height + 2 ){ // traits left side
            bt = WinCreateButton( gChrEditWin, 23, y, gChrEditImgGeo[49].Width, gChrEditImgGeo[49].Height, -1, -1, -1, 555 + i, gChrEditPix[48], gChrEditPix[49], 0, 32 );
            gChrEditUnk59[ i ] = bt;
            if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, 0 );            
        }
        y = 352;
        for( i = 0; i < 8; i++, y += gChrEditImgGeo[49].Height + 2 ){ // traits right side
            bt = WinCreateButton(gChrEditWin, 298, y, gChrEditImgGeo[49].Width, gChrEditImgGeo[49].Height, -1, -1, -1, 563 + i, gChrEditPix[48], gChrEditPix[49], 0, 32);
            gChrEditUnk60[ i ] = bt;
            if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, 0 );            
        }
        CharEditShowTraits();
    } else {
        ScrCopyAlpha( gChrEditImgPixMaps[7], gChrEditImgGeo[6].Width, gChrEditImgGeo[6].Height, gChrEditImgGeo[6].Width, gChrEditSurface + 9, 640 );
        ScrCopyAlpha( gChrEditImgPixMaps[2], gChrEditImgGeo[25].Width, gChrEditImgGeo[25].Height, gChrEditImgGeo[25].Width, &gChrEditSurface[gChrEditImgGeo[6].Width + 9], 640 );
        ScrCopyAlpha( gChrEditImgPixMaps[10], gChrEditImgGeo[11].Width, gChrEditImgGeo[11].Height, gChrEditImgGeo[11].Width, &gChrEditSurface[gChrEditImgGeo[25].Width + 9 + gChrEditImgGeo[6].Width], 640 );
        bt = WinCreateButton( gChrEditWin, 11, 327, gChrEditImgGeo[8].Width, gChrEditImgGeo[8].Height, -1, -1, -1, 535, 0, 0, 0, 32 );
        if( bt != -1 ) WinSetDefaultImg(bt, gChrEditPix[8]);
    }

    if( gChrEditMenuMode == 1 ){
        ht1 = 510; ht2 = 503;
        for( m = 0; m != 7; m++, ht1++, ht2++ ){
            bt = WinCreateButton(gChrEditWin, 149, gChrEditStatPosY[m], gChrEditImgGeo[16].Width, gChrEditImgGeo[16].Height, -1, 518, ht2, 518, gChrEditPix[15], gChrEditPix[16], 0, 32);
            gChrEditUnk06[m] = bt;
            if( bt != -1 ) WinSetClickSound(bt, GSoundPlayPushBt, 0);
            bt = WinCreateButton(gChrEditWin, 149, gChrEditImgGeo[16].Height + gChrEditStatPosY[m] - 1, gChrEditImgGeo[14].Width, gChrEditImgGeo[14].Height, -1, 518, ht1, 518, gChrEditPix[13], gChrEditPix[14], 0, 32);
            gChrEditUnk02[m] = bt;
            if( bt != -1 ) WinSetClickSound(bt, GSoundPlayPushBt, 0);            
        }
    }
    CharEditAreasDefine();
    SoundUpdateAll();
    bt = WinCreateButton(gChrEditWin, 343, 454, gChrEditImgGeo[23].Width, gChrEditImgGeo[23].Height, -1, -1, -1, 501, gChrEditPix[23], gChrEditPix[24], 0, 32);
    if( bt != -1 ) WinSetClickSound(bt, GSoundPlayPushBt, GSoundPlayReleaseBt);
    bt = WinCreateButton(gChrEditWin, 552, 454, gChrEditImgGeo[23].Width, gChrEditImgGeo[23].Height, -1, -1, -1, 502, gChrEditPix[23], gChrEditPix[24], 0, 32);
    if( bt != -1 ) WinSetClickSound(bt, GSoundPlayPushBt, GSoundPlayReleaseBt);
    bt = WinCreateButton(gChrEditWin, 455, 454, gChrEditImgGeo[23].Width, gChrEditImgGeo[23].Height, -1, -1, -1, 500, gChrEditPix[23], gChrEditPix[24], 0, 32);
    if( bt != -1 )WinSetClickSound(bt, GSoundPlayPushBt, GSoundPlayReleaseBt);
    WinUpdate( gChrEditWin );
    IfaceIndicatorBoxHide();
    return 0;
}

void CharEditClose()
{
    int i;

    if( gChrEditFoldArrBtUp != -1 ){ WinImageRemove( gChrEditFoldArrBtUp ); gChrEditFoldArrBtUp = -1; }
    if( gChrEditFoldArrBtDn != -1 ){ WinImageRemove( gChrEditFoldArrBtDn ); gChrEditFoldArrBtDn = -1; }
    WinClose(gChrEditWin);
    for( i = 0; i < 50; i++ ){
        ArtClose( gChrEditImg[ i ] );
        if( gChrEditImgCpyMap[ i ] ) Free( gChrEditImgPixMaps[ i ] );
    }
    ArtClose( gChrEditWpImg );
    if( gCharEditGenre ){ Free( gCharEditGenre ); gCharEditGenre = NULL; }
    gCharEditGenreCount = 0;
    if( gCharEditKarma ){ Free( gCharEditKarma ); gCharEditKarma = NULL; }
    gChrEditMsgKarmaCount = 0;
    MessageClose( &gChrEditMsg );
    IfaceUnk09();
    if( gChrEditUnk13 ) MapAmbientEnable();
    CycleColorStart();
    GmouseLoadCursor( 1 );
    FontSet( gChrEditFontSave );
    if( gChrEditMenuMode == 1 ){
        SkillSetSpecials( gChrEditTagSkill, 3 );
        TraitSetSpec( gChrEditOptionTraits[0], gChrEditOptionTraits[1] );
        gChrEditSelected = 0;
        CritterHeal( gObjDude, 1000 );
    }
    IfaceIndicatorBoxShow();
}

void CharEditClean()
{
    if( gChrEditUnk13 ) MapAmbientEnable();
    CycleColorStart();
    GmouseLoadCursor(1);
}

void CharEditInit()
{
    int i;

    gChrEditSelected = 0;
    gChrEditSkillSelect = 0;
    gChrEditSkillSelPosY = 27;
    gChrEditNewPerkFlag = 0;
    gChrEditFolderType = 0;
    for( i = 0; i < 2; i++ ){ 
        gChrEditOptionTraits[ i ] = -1;
        gChrEditOptTraits[ i ] = -1;
    }
    gChrEditUnspentChrPts = 5;
    gChrEditCurLvl = 1;
}

int CharEditLineEditor( int WinId, int KeyDone, char *Text, int MaxLen, int PosX, int PosY, int FgColor, int BgColor, char a9 )
{
    char stmp[ 256 ], *pDst, *Surface;
    int Width, k, sel, CursorWidth, Height, Blink, BlinkCnt, RetCode;

    RetCode = 1;
    CursorWidth = gFont.LineWidth( "_" ) - 4;
    BlinkCnt = 3;
    Width = WinGetWidth( WinId );
    Height = gFont.ChrHeight();
    Blink = 0;
    Surface = WinGetSurface( WinId );
    if( MaxLen > 255 ) MaxLen = 255;
    strcpy( stmp, Text );
    k = strlen( stmp );
    stmp[ k ] = ' ';
    stmp[ k + 1 ] = '\0';

    pDst = Surface + PosX + Width * PosY;
    ScrFillSolid( pDst, gFont.LineWidth( stmp ), Height, Width, BgColor ); // clear
    gFont.Print( pDst, stmp, Width, Width, FgColor );
    WinUpdate( WinId );

    while( RetCode == 1 ){
            gChrEditTime = TimerGetSysTime();
            sel = InpUpdate();
            if( sel == KeyDone ){
                RetCode = 0;
            } else if( sel == KEY_ENTER ){
                GSoundPlay( "ib1p1xx1" );
                RetCode = 0;
            } else if( sel == KEY_ESC || gMenuEscape ){
                RetCode = -1;
            } else {
                if( (sel == 339 || sel == 8) && k >= 1 ){ // delete character
                    ScrFillSolid( pDst, gFont.LineWidth(stmp), Height, Width, BgColor );
                    stmp[ k ] = '\0';
                    stmp[ k - 1 ] = ' ';
                    gFont.Print( pDst, stmp, Width, Width, FgColor );
                    k--;
                    WinUpdate( WinId );
                } else if( (sel >= ' ') && (sel <= 'z') && (k < MaxLen) ){
                    if( (a9 & 0x01) && !CharEditStrChrMark( sel ) ) break;
                    ScrFillSolid( pDst, gFont.LineWidth( stmp ), Height, Width, BgColor );// clear
                    stmp[ k ] = sel;
                    stmp[ k + 1 ] = ' ';
                    stmp[ k + 2 ] = '\0';
                    gFont.Print( pDst, stmp, Width, Width, FgColor );
                    k++;
                    WinUpdate( WinId );                    
                }                
            }
            if( --BlinkCnt <= 0 ){ // draw blinking cursor
                BlinkCnt = 3;
                ScrFillSolid( pDst + gFont.LineWidth( stmp ) - CursorWidth, CursorWidth, Height - 2, Width, ( Blink ) ? BgColor : FgColor );
                Blink = !Blink;
            }
            WinUpdate( WinId );
            TIMER_WAIT( gChrEditTime, 41 );
    }
    if( !RetCode || k > 0 ){
        stmp[ k ] = '\0';
        strcpy( Text, stmp );
    }
    return 0;
}

int CharEditStrChrMark( int chr )
{
    unsigned int i;
    char stmp[28];

    strcpy( stmp, "#@!$`'~^&()-_=[]{}" );
    if( (IsTable[ chr + 1 ] & 0xE0) ) return 1;
    for( i = 0; i < strlen( stmp ); i++ ){
        if( stmp[ i ] == chr ) return 1;
    }
    return 0;
}

char *CharEditFnameChgExt( char *StrA, char *Src, char *StrB )
{
    char *s, *p;

    s = StrA;
    while( *Src && (*Src != '.') ) *StrA++ = *Src++;
    
    *StrA = '.';
    for( p = StrA + 1; *StrB; p++ ) *p = *StrB++;
    
    *p = '\0';
    return s;
}

void CharEditShowFolder()
{
    if( gChrEditMenuMode == 1 ) return;    
    ScrCopy( gChrEditWpPix + 360 * 640 + 34, 280, 120, 640, gChrEditSurface + 360 * 640 + 34, 640 );
    FontSet( 101 );
    
    switch( gChrEditFolderType ){
	case 0: // perks
	    ScrCopy( gChrEditImgPixMaps[46], gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Height, gChrEditImgGeo[46].Width, gChrEditSurface + 327*640 + 11 , 640 );
	    CharEditShowPerks();
	    break;
	case 1: // karma
    	    ScrCopy( gChrEditImgPixMaps[44], gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Height, gChrEditImgGeo[46].Width, gChrEditSurface + 327*640 + 11, 640 );
    	    CharEditShowKarma();
	    break;
	case 2: //kills
    	    ScrCopy( gChrEditImgPixMaps[45], gChrEditImgGeo[46].Width, gChrEditImgGeo[46].Height, gChrEditImgGeo[46].Width, gChrEditSurface + 327*640 + 11, 640 );
    	    gChrEditUnk70 = CharEditShowKills();
    	    break;
    	default: eprintf("\n ** Unknown folder type! **\n"); break;

    }
}

void CharEditShowPerks()
{
    int i, Selected = 0, j, lvl; 
    char stmp[128];
    
    CharEditFoldArrowsHandle();

    if( gChrEditOptionTraits[ 0 ] != -1 ){
        if( CharEditPerksFoldTitle( MessageGetMessage( &gChrEditMsg, &gChrEditList, 156 ) ) ){
            gChrEditDscImgId = 54;
            gChrEditDscTitle = MessageGetMessage( &gChrEditMsg, &gChrEditList, 146 );
            gChrEditDscParams = 0;
            gChrEditDscDsc = MessageGetMessage( &gChrEditMsg, &gChrEditList, 147 );
	    Selected = 1;
        }
        if( gChrEditOptionTraits[0] != -1 ){
            if( CharEditFoldLine( TraitGetName( gChrEditOptionTraits[0] ) ) ){
                gChrEditDscImgId = TraitGetId( gChrEditOptionTraits[0] );
                gChrEditDscTitle = TraitGetName( gChrEditOptionTraits[0] );
                gChrEditDscParams = 0;
                gChrEditDscDsc = TraitGetDsc( gChrEditOptionTraits[0] );
		Selected = 1;
            }
        }
        if( gChrEditOptionTraits[ 1 ] != -1 ){
            if( CharEditFoldLine( TraitGetName( gChrEditOptionTraits[1] ) ) ){
                gChrEditDscImgId = TraitGetId( gChrEditOptionTraits[1] );
                gChrEditDscTitle = TraitGetName( gChrEditOptionTraits[1] );
                gChrEditDscParams = 0;
                gChrEditDscDsc = TraitGetDsc( gChrEditOptionTraits[1] );
		Selected = 1;
            }
        }
    }

    for( i = 0; i < 119; i++ ){
        if( PerkLvl( gObjDude, i ) ) break;
    }
    if( i < 119 ){ // any perks ?
        CharEditPerksFoldTitle( MessageGetMessage( &gChrEditMsg, &gChrEditList, 109 ) );
        for( j = 0; j < 119; j++ ){
            if( !(lvl = PerkLvl( gObjDude, j )) ) continue;            
            if( lvl <= 1 ){
                strcpy( stmp, PerkGetName( j ) );
            } else {
                sprintf( stmp, "%s (%d)", PerkGetName( j ), lvl );
            }
            if( CharEditFoldLine( stmp ) ){
                gChrEditDscImgId = PerkGetImgId( j );
                gChrEditDscTitle = PerkGetName( j );
                gChrEditDscParams = 0;
                gChrEditDscDsc = PerkGetDsc( j );
                Selected = 1;
            }
        }
    }
    if( !Selected ){ // no perk selected, use default
        gChrEditDscImgId = 71;
        gChrEditDscTitle = MessageGetMessage( &gChrEditMsg, &gChrEditList, 124 );
        gChrEditDscParams = 0;
        gChrEditDscDsc = MessageGetMessage( &gChrEditMsg, &gChrEditList, 127 );
    }
}

int CharEditStrCmp( char **pStr1, char **pStr2 )
{
    return strcasecmp( *pStr1, *pStr2 );
}

unsigned int CharEditShowKills()
{
    ChrEditKillStat_t Stats[ 19 ];
    int flg, i, cnt, stat; 

    CharEditFoldArrowsHandle();
    flg = 0;
    cnt = 0;
    for( i = 0; i < 19; i++ ){
        stat = CritterGetKillStat( i );
        if( stat <= 0 ) continue;
        Stats[ cnt ].Name = CritterKillTypeName( i );
        Stats[ cnt ].Id = i;        
        Stats[ cnt ].Stat = stat;
        cnt++;

    }
    if( cnt ){
        qsort( &Stats, cnt, sizeof( ChrEditKillStat_t ), (void *)CharEditStrCmp );
        for( i = 0; i < cnt; i++ ){
            if( CharEditFoldKillsLine( Stats[ i ].Name, Stats[ i ].Stat ) ){
                gChrEditDscImgId = 46;
                gChrEditDscTitle = gChrEditUnk30;
                gChrEditDscParams = NULL;
                gChrEditDscDsc = CritterGetKillTypeDsc( Stats[ i ].Id );
                sprintf( gChrEditUnk30, "%s %s", Stats[ i ].Name, MessageGetMessage( &gChrEditMsg, &gChrEditList, 126 ) );
                flg = 1;
            }
        }
    }
    if( !flg ){
        gChrEditDscImgId = 46;
        gChrEditDscTitle = MessageGetMessage( &gChrEditMsg, &gChrEditList, 126 );
        gChrEditDscParams = NULL;
        gChrEditDscDsc = MessageGetMessage( &gChrEditMsg, &gChrEditList, 129 );
    }
    return cnt;
}

void CharEditCounterDisplay( int Xpos, int Ypos, int flg, int a4, int a5, int WinId )
{
    VidRect_t v2;
    int Width, p, v13,y;
    void *pDst;
    char *Surface;

    Width = WinGetWidth(WinId);
    Surface = WinGetSurface(WinId);
    v2.tp = Ypos;
    v2.rt = Xpos + 28;
    y = 0;
    v2.bm = Ypos + 24;
    v2.lt = Xpos;
    if( flg & 2 )  y = gChrEditImgGeo[ 0 ].Width / 2;
    if( a4 <= 99 && a5 <= 99 && a4 >= 0 && a5 >= 0 ){
        v13 = a4 % 10;
        pDst = &Surface[Width * Ypos + 14 + Xpos];
        if( flg & 1 ){
            if( a5 % 10 != v13 ){
                gChrEditTime = TimerGetSysTime();
                ScrCopy( &gChrEditPix[0][y + 154], 14, 24, gChrEditImgGeo[0].Width, pDst, Width );
                WinAreaUpdate( WinId, &v2 );
                while( TimerCurrDiff( gChrEditTime ) < 0x7B );
            }
            p = Width * Ypos;
            ScrCopy( &gChrEditPix[0][14 * (a4 % 10) + y], 14, 24, gChrEditImgGeo[0].Width, &Surface[Width * Ypos + 14 + Xpos], Width );
            WinAreaUpdate( WinId, &v2 );
            if ( a4 / 10 != a5 / 10 ){
                gChrEditTime = TimerGetSysTime();
                ScrCopy( &gChrEditPix[0][y + 154], 14, 24, gChrEditImgGeo[0].Width, &Surface[Xpos + p], Width );
                WinAreaUpdate( WinId, &v2 );
                while( TimerCurrDiff(gChrEditTime) < 0x7B );
            }
            ScrCopy( &gChrEditPix[0][14 * (a4 / 10) + y], 14, 24, gChrEditImgGeo[0].Width, &Surface[Xpos + Ypos * Width], Width );
            WinAreaUpdate( WinId, &v2 );
        } else {
            ScrCopy( &gChrEditPix[0][14 * (a4 / 10) + y], 14, 24, gChrEditImgGeo[0].Width, &Surface[Xpos + Width * Ypos], Width );
            ScrCopy( &gChrEditPix[0][14 * v13 + y], 14, 24, gChrEditImgGeo[0].Width, pDst, Width );
        }
    } else {
        ScrCopy( &gChrEditPix[0][y + 126], 14, 24, gChrEditImgGeo[0].Width, &Surface[Width * Ypos + Xpos], Width );
        ScrCopy( &gChrEditPix[0][y + 126], 14, 24, gChrEditImgGeo[0].Width, &Surface[Width * Ypos + 14 + Xpos], Width );
    }
}

void CharEditShowLevelInfo()
{
    int y, val, Idx;
    char *str, tmp[16], stmp[136];

    if( gChrEditMenuMode == 1 ) return;
    FontSet( 101 );
    ScrCopy( gChrEditWpPix + 179232, 124, 32, 640, gChrEditSurface + 179232, 640 );
    sprintf( stmp, "%s %d", MessageGetMessage( &gChrEditMsg, &gChrEditList, 113 ), FeatGetPoints( FEAT_PT_LV ) ); // current level
    gFont.Print( gChrEditSurface + 179232, stmp, 640, 640, (gChrEditSelected == 7) ? CHREDT_COL_LVL_A : CHREDT_COL_LVL_B );
    y = gFont.ChrHeight() + 281;
    sprintf( &stmp[8], "%s %s", MessageGetMessage( &gChrEditMsg, &gChrEditList, 114 ), CharEditValToStr( FeatGetPoints( FEAT_PT_XP ), tmp ) ); // current exp
    gFont.Print( &gChrEditSurface[640 * y + 32], &stmp[8], 640, 640, (gChrEditSelected == 8) ? CHREDT_COL_LVL_A : CHREDT_COL_LVL_B );
    y += gFont.ChrHeight() + 1;	
    val = FeatNextLvlPts();
    Idx = 115;
    if( val == -1 ) {
        str = "------";
    } else {
        if( val > 999999 ) Idx = 175;
        str = CharEditValToStr( val, tmp );
    }
    sprintf( &stmp[4], "%s %s", MessageGetMessage( &gChrEditMsg, &gChrEditList, Idx ), str ); // exp for next level
    gFont.Print( &gChrEditSurface[640 * y + 32], &stmp[4], 640, 640, (gChrEditSelected == 9) ? CHREDT_COL_LVL_A : CHREDT_COL_LVL_B );
}

void CharEditShowStats( int Id, int a1, int a2 )
{
    int color, aa, y, pos;

    while( 1 ){
        FontSet( 101 );
        if( Id != 7 ) break;
        CharEditShowStats( 0, 0, 0 );
        CharEditShowStats( 1, 0, 0 );
        CharEditShowStats( 2, 0, 0 );
        CharEditShowStats( 3, 0, 0 );
        CharEditShowStats( 4, 0, 0 );
        CharEditShowStats( 5, 0, 0 );
        Id = 6;
        a2 = 0;
    }
    color = ( Id == gChrEditSelected ) ? CHREDT_COL_LVL_A : CHREDT_COL_LVL_B;
    pos  = 103 + 128 * (5 * (gChrEditStatPosY[ Id ] + 8));
    if( a1 == 1 ){
        if( gChrEditMenuMode == 1 ){
            aa = FeatGetTotal( gObjDude, Id ) + FeatGetBoost( gObjDude, Id );
            CharEditCounterDisplay( 58, gChrEditStatPosY[ Id ], ( aa > 10 ) ? 3 : 1, aa, a2, gChrEditWin );	    	    
            ScrCopy( gChrEditWpPix + pos, 40, gFont.ChrHeight(), 640, gChrEditSurface + 640 * gChrEditStatPosY[ Id ] + 5223, 640 );
            aa = FeatGetVal( gObjDude, Id ) + 199;
            if( aa > 210 ) aa = 210;
	    gFont.Print( gChrEditSurface + pos, MessageGetMessage(&gChrEditMsg, &gChrEditList, aa), 640, 640, color );
        } else {
    	    y = gChrEditStatPosY[ Id ];
    	    CharEditCounterDisplay( 58, y, 1, FeatGetVal( gObjDude, Id ), a2, gChrEditWin );	    
    	    ScrCopy( gChrEditWpPix + pos , 40, gFont.ChrHeight(), 640, &gChrEditSurface[ 640 * y + 5223 ], 640 );
    	    aa = FeatGetVal( gObjDude, Id );
    	    if( aa > 10 ) aa = 10;
	    gFont.Print( gChrEditSurface + pos, FeatGetComments( aa ), 640, 640, color );
	}
    } else if( a1 == 0 ){
	if( gChrEditMenuMode != 1 ){
	    y = gChrEditStatPosY[ Id ];
    	    CharEditCounterDisplay( 58, y, 0, FeatGetVal( gObjDude, Id ), 0, gChrEditWin );
    	    ScrCopy( gChrEditWpPix + pos, 40, gFont.ChrHeight(), 640, &gChrEditSurface[ 640 * y + 5223 ], 640 );
    	    aa = FeatGetVal( gObjDude, Id );
    	    if( aa > 10 ) aa = 10;
	    gFont.Print( gChrEditSurface + pos, FeatGetComments( aa ), 640, 640, color );
	} else {
	    aa = FeatGetTotal( gObjDude, Id ) + FeatGetBoost( gObjDude, Id );
	    CharEditCounterDisplay( 58, gChrEditStatPosY[ Id ], ( aa > 10 ) ? 2 : 0, aa, 0, gChrEditWin );
	    ScrCopy( gChrEditWpPix + pos , 40, gFont.ChrHeight(), 640, &gChrEditSurface[ 640 * gChrEditStatPosY[ Id ] + 5223 ], 640 );
    	    aa = FeatGetVal( gObjDude, Id ) + 199;
	    if( aa > 210 ) aa = 210;
	    gFont.Print( gChrEditSurface + pos , MessageGetMessage(&gChrEditMsg, &gChrEditList, aa), 640, 640, color );
	}
    }
}

void CharEditShowHeroGender()
{
    int Width,n;
    char stmp[56];

    FontSet( 103 );
    strcpy( stmp, MessageGetMessage( &gChrEditMsg, &gChrEditList, FeatGetVal( gObjDude, 34 ) + 107 ) );
    Width = gChrEditImgGeo[11].Width;
    memcpy( gChrEditImgPixMaps[11], gChrEditPix[11], Width * gChrEditImgGeo[11].Height );
    memcpy( gChrEditImgPixMaps[10], gChrEditPix[10], Width * gChrEditImgGeo[10].Height );
    n = 6 * Width + ( Width / 2 ) - (gFont.LineWidth(stmp) / 2 );
    gFont.Print( gChrEditImgPixMaps[11] + n, stmp, Width, Width, CHREDT_COL_DIMMED );
    gFont.Print( gChrEditImgPixMaps[10] + n, stmp, Width, Width, CHREDT_COL_SELECT );
}

void CharEditShowHeroAge()
{
    int Width, v3, v4;
    char stmp[56];

    FontSet( 103 );
    sprintf( stmp, "%s %d", MessageGetMessage( &gChrEditMsg, &gChrEditList, 104 ), FeatGetVal( gObjDude, 33 ) );
    Width = gChrEditImgGeo[25].Width;
    v3 = (Width >> 1) + 1 - (gFont.LineWidth(stmp) >> 1);
    memcpy( gChrEditImgPixMaps[25], gChrEditPix[25], Width * gChrEditImgGeo[25].Height );
    memcpy( gChrEditImgPixMaps[2], gChrEditPix[2], Width * gChrEditImgGeo[25].Height );
    v4 = 6 * Width + v3;
    gFont.Print(v4 + gChrEditImgPixMaps[25], stmp, Width, Width, CHREDT_COL_DIMMED );
    gFont.Print(v4 + gChrEditImgPixMaps[2], stmp, Width, Width, CHREDT_COL_SELECT );
}

void CharEditShowHeroName()
{
    char c;
    int i,flg,Width, a;
    char stmp1[64], stmp2[88];

    FontSet(103);
    strcpy( stmp1, CritterGetName( gObjDude ) );
    if( gFont.LineWidth(stmp1) > 100 ){
        i = 0;
        flg = 0;
        do {
            c = stmp1[i];
            stmp2[i] = c;
            stmp2[i + 1] = 0;
            if( c == ' ' ) flg = 1;
            if( gFont.LineWidth(stmp2) > 100 ) break;
            i++;
        }while ( i < 40 );
        stmp1[i] = 0;
        if( flg ){
            c = stmp1[--i];
            if( c != ' ' ){
                do
                    stmp1[i--] = 0;
                while ( i > 1 && stmp1[i] != ' ' );
            }
            stmp1[i] = 0;
        }
    }
    memcpy( gChrEditImgPixMaps[6], gChrEditPix[6], gChrEditImgGeo[6].Height * gChrEditImgGeo[6].Width );
    memcpy( gChrEditImgPixMaps[7], gChrEditPix[7], gChrEditImgGeo[7].Width * gChrEditImgGeo[7].Height );
    Width = gChrEditImgGeo[6].Width;
    a = (Width / 2) + 3 - (gFont.LineWidth(stmp1) / 2) + 6 * Width;
    gFont.Print( gChrEditImgPixMaps[6] + a, stmp1, Width, Width, CHREDT_COL_DIMMED );
    gFont.Print( gChrEditImgPixMaps[7] + a, stmp1, Width, Width, CHREDT_COL_SELECT );
}

void CharEditShowParams()
{
    int HpPts, y, HpLvl, col;
    char str[512];

    FontSet( 101 );
    ScrCopy( gChrEditWpPix + 29634, 118, 108, 640, gChrEditSurface + 29634, 640 );
    col = ( gChrEditSelected == 43 ) ? CHREDT_COL_LVL_A : CHREDT_COL_LVL_B;
    if( gChrEditMenuMode == 1 ){
        HpLvl = FeatGetVal( gObjDude, FEAT_HP );
        HpPts = HpLvl;
    } else {
        HpLvl = FeatGetVal( gObjDude, 0 );
        HpPts = CritterGetHp( gObjDude );
    }
    // hit points
    sprintf( str, "%s", MessageGetMessage( &gChrEditMsg, &gChrEditList, 300 ) );
    gFont.Print( gChrEditSurface + 29634, str, 640, 640, col );
    sprintf( str, "%d/%d", HpPts, HpLvl );
    gFont.Print( gChrEditSurface + 29440 + 263, str, 640, 640, col );

    y = 46;
    CHRED_STATES_A( 312, 44, CritterPoisoned( gObjDude ) <= 0 )	// poisoned
    CHRED_STATES_A( 313, 45, CritterRadiated( gObjDude ) <= 0 )	// radiated
    CHRED_STATES_A( 314, 46, ( gObjDude->Critter.State.CombatResult & 0x80 ) == 0 ) // eye damage
    CHRED_STATES_A( 315, 47, ( gObjDude->Critter.State.CombatResult & 0x40 ) == 0 ) // crippled right arm
    CHRED_STATES_A( 316, 48, ( gObjDude->Critter.State.CombatResult & 0x10 ) == 0 ) // crippled left arm
    CHRED_STATES_A( 317, 49, ( gObjDude->Critter.State.CombatResult & 0x08 ) == 0 ) // crippled right leg    
    CHRED_STATES_A( 318, 50, ( gObjDude->Critter.State.CombatResult & 0x04 ) == 0 ) // crippled left leg

    ScrCopy( gChrEditWpPix + 640 * 182 + 194, 116, 130, 640, gChrEditSurface + 640 * 182 + 194, 640 );
    
    y = -9; 
    CHRED_STATES_B( 182, 51, 302, "%i",   9,  col ); // armor class
    CHRED_STATES_B( 3,   52, 301, "%i",   8,  col ); // action points
    CHRED_STATES_B( 3,   53, 311, "%i",   12, CritterOverload( gObjDude ) ? CHREDT_COL_LVL_F : col ); // carry weight
    CHRED_STATES_B( 3,   54, 304, "%i",   11, col ); // melee damage
    CHRED_STATES_B( 3,   55, 305, "%d%%", 24, col ); // damage resistance
    CHRED_STATES_B( 3,   56, 306, "%d%%", 32, col ); // poison resistance
    CHRED_STATES_B( 3,   57, 307, "%d%%", 31, col ); // radiation resistance
    CHRED_STATES_B( 3,   58, 308, "%i",   13, col ); // sequence
    CHRED_STATES_B( 3,   59, 309, "%i",   14, col ); // healing rate
    CHRED_STATES_B( 3,   60, 310, "%d%%", 15, col ); // critical chance
}

void CharEditShowSkills( int SkillLock )
{
    int i, y,col, aa = -1;
    char stmp[32];

    if( gChrEditSelected >= 61 && gChrEditSelected < 79 ) aa = gChrEditSelected - 61;
    if( !gChrEditMenuMode && !SkillLock ){
        WinImageRemove( gChrEditSkillPlus );
        WinImageRemove( gChrEditSkillMinus );
        gChrEditSkillMinus = -1;
        gChrEditSkillPlus = -1;
    }
    ScrCopy( gChrEditWpPix + 370, 270, 252, 640, gChrEditSurface + 370, 640 );
    FontSet( 103 );
    gFont.Print( gChrEditSurface + 5*640 + 380, MessageGetMessage( &gChrEditMsg, &gChrEditList, 117 ), 640, 640, CHREDT_COL_SELECT ); // 'Skills'
    if( gChrEditMenuMode ){
        gFont.Print(gChrEditSurface + 149542, MessageGetMessage( &gChrEditMsg, &gChrEditList, 138 ), 640, 640, CHREDT_COL_SELECT );
        if( (SkillLock == 2) && !gChrEditUnk18 ){
            CharEditCounterDisplay( 522, 228, 1, gChrEditTaggedSkillsCnt, gChrTagSkillUnspent, gChrEditWin );
        } else {
            CharEditCounterDisplay( 522, 228, 0, gChrEditTaggedSkillsCnt, 0, gChrEditWin );
            gChrEditUnk18 = 0;
        }
    } else {
        gFont.Print( gChrEditSurface + 640 * 233 + 400 , MessageGetMessage( &gChrEditMsg, &gChrEditList, 112 ), 640, 640, CHREDT_COL_SELECT );
        CharEditCounterDisplay( 522, 228, 0, FeatGetPoints(0), 0, gChrEditWin );
    }

    SkillSetSpecials( gChrEditTagSkill, 4 );
    FontSet( 101 );
    y = 27;
    for( i = 0; i < 18; i++, y += gFont.ChrHeight() + 1 ){
        if( i == aa ){
            col = ( i == gChrEditTagSkill[0] || i == gChrEditTagSkill[1] || i == gChrEditTagSkill[2] || i == gChrEditTagSkill[3] ) ? CHREDT_COL_LVL_G : CHREDT_COL_LVL_A; 
        } else {
    	    col = ( i == gChrEditTagSkill[0] || i == gChrEditTagSkill[1] || i == gChrEditTagSkill[2] || i == gChrEditTagSkill[3] ) ? CHREDT_COL_LVL_H : CHREDT_COL_LVL_B;    	    
    	}
        gFont.Print( gChrEditSurface + 640 * y + 380, SkillGetName( i ), 640, 640, col );
        sprintf( stmp, "%d%%", SkillGetTotal( gObjDude, i ) );
        gFont.Print( gChrEditSurface + 640 * y + 573, stmp, 640, 640, col );        
    }
    // slider
    if( gChrEditMenuMode ) return;    
    y = gChrEditSkillSelect * (gFont.ChrHeight() + 1);
    gChrEditSkillSelPosY = y + 27;
    ScrCopyAlpha( gChrEditPix[12], gChrEditImgGeo[12].Width, gChrEditImgGeo[12].Height, gChrEditImgGeo[12].Width, &gChrEditSurface[640 * y + 10832], 640 );
    if( SkillLock ) return;    
    if( gChrEditSkillPlus == -1 ){
        gChrEditSkillPlus = WinCreateButton( gChrEditWin, 614, gChrEditSkillSelPosY - 7, gChrEditImgGeo[16].Width, gChrEditImgGeo[16].Height, gChrEditSkillPlus, 522, 521, 522, gChrEditPix[15], gChrEditPix[16], 0, 96 );
        WinSetClickSound( gChrEditSkillPlus, GSoundPlayPushBt, 0 );
    }
    if( gChrEditSkillMinus == -1 ){
        gChrEditSkillMinus = WinCreateButton( gChrEditWin, 614, gChrEditImgGeo[14].Height + gChrEditSkillSelPosY + 4 - 12, gChrEditImgGeo[14].Width, gChrEditImgGeo[13].Height, gChrEditSkillMinus, 524, 523, 524, gChrEditPix[13], gChrEditPix[14], 0, 96 );
        WinSetClickSound( gChrEditSkillMinus, GSoundPlayPushBt, 0 );
    }    
}

void CharEditDscUpdate()
{
    unsigned int bb;
    char *s, *p, str[152];

    if( gChrEditSelected > 0x61 ) return;    
    ScrCopy( gChrEditWpPix + 640 * 267 + 345, 277, 170, 640, gChrEditSurface + 640 * 267 + 345, 640 );
    bb = gChrEditSelected;
    if( gChrEditSelected <= 6 ){ // stats
        if( gChrEditSelected > 6 ) bb = 6;                        
        CharEditCaption( FeatGetId( bb ), FeatGetName( bb ), 0, FeatGetDsc( bb ) );
        return;
    }
    if( gChrEditSelected < 7 || gChrEditSelected >= 10 ){ // folder
        if( (gChrEditSelected >= 10) && (gChrEditSelected < 43 || gChrEditSelected >= 82) && (gChrEditSelected < 98) ){
            CharEditCaption( gChrEditDscImgId, gChrEditDscTitle, gChrEditDscParams, gChrEditDscDsc ); // kill stat
            return;
	}
        if( gChrEditSelected < 43 || gChrEditSelected >= 51 ){
            if( gChrEditSelected < 51 || gChrEditSelected >= 61 ){
                if( gChrEditSelected < 61 || gChrEditSelected >= 79 ){ // ??
                    switch( gChrEditSelected ){
                        case 79:
                            if( gChrEditMenuMode ){
                                p = MessageGetMessage( &gChrEditMsg, &gChrEditList, 145 );
                                s = MessageGetMessage( &gChrEditMsg, &gChrEditList, 144 );
                            } else {
                                p = MessageGetMessage( &gChrEditMsg, &gChrEditList, 131 );
                                s = MessageGetMessage( &gChrEditMsg, &gChrEditList, 130 );
                            }
                            CharEditCaption( 27, s, 0, p );
                            break;
                        case 80: CharEditCaption( 27, MessageGetMessage( &gChrEditMsg, &gChrEditList, 150 ), 0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 151 ) ); return;
                        case 81: CharEditCaption( 54, MessageGetMessage( &gChrEditMsg, &gChrEditList, 146 ), 0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 147 ) ); break;
                    }
                } else { // Skills
                    bb = gChrEditSelected - 61;
                    if( bb > 17 ) bb = 17;
                    sprintf( str, "%s %d%% %s", MessageGetMessage( &gChrEditMsg, &gChrEditList, 137 ), SkillGetLvl( bb ), SkillGetAttr( bb ) );
                    CharEditCaption( SkillGetImgId( bb ), SkillGetName( bb ), str, SkillGetDsc( bb ) );
                }
            } else { // abilities
                bb = gChrEditSelected - 51;
                if( bb > 9 ) bb = 9;
                CharEditCaption( gChrEditUnk128[ bb ], FeatGetName( bb ), 0, FeatGetDsc( gChrEditUnk127[ bb ] ) );
            }
            return;
        }
        switch( gChrEditSelected ){ // damages
            case 43: CharEditCaption( FeatGetId( 7 ), MessageGetMessage( &gChrEditMsg, &gChrEditList, 300 ), 0, FeatGetDsc( 7 ) ); break; // HP
            case 44: CharEditCaption( 11, MessageGetMessage( &gChrEditMsg, &gChrEditList, 312 ), 0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 400 ) ); break;
            case 45: CharEditCaption( 12, MessageGetMessage( &gChrEditMsg, &gChrEditList, 313 ), 0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 401 ) ); break;
            case 46: CharEditCaption( 13, MessageGetMessage( &gChrEditMsg, &gChrEditList, 314 ), 0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 402 ) ); break;
            case 47: CharEditCaption( 14, MessageGetMessage( &gChrEditMsg, &gChrEditList, 315 ), 0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 403 ) ); break;
            case 48: CharEditCaption( 15, MessageGetMessage( &gChrEditMsg, &gChrEditList, 316 ), 0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 404 ) ); break;
            case 49: CharEditCaption( 16, MessageGetMessage( &gChrEditMsg, &gChrEditList, 317 ), 0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 405 ) ); break;
            case 50: CharEditCaption( 17, MessageGetMessage( &gChrEditMsg, &gChrEditList, 318 ), 0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 406 ) ); break;
        }
        return;
    } else if( (gChrEditMenuMode == 1) && (gChrEditSelected == 7) ){ // character points
        CharEditCaption( 7, MessageGetMessage( &gChrEditMsg, &gChrEditList, 120 ), 0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 121 ) );        
    } else if( gChrEditSelected == 7 ){
        CharEditCaption( 7, FeatGetPtsName( 1 ), 0, FeatGetPtsDsc( 1 ) );
    } else if( (gChrEditSelected - 7) <= 1 ){
        CharEditCaption( 8, FeatGetPtsName( 2 ), 0, FeatGetPtsDsc( 2 ) );
    } else if( gChrEditSelected == 9 ){
        CharEditCaption( 9, MessageGetMessage( &gChrEditMsg, &gChrEditList, 122 ), 0, MessageGetMessage( &gChrEditMsg, &gChrEditList, 123 ) );
    }    
}

int CharEditDialogName()
{
    int w29; int win; int bt; int h35; int w35;     
    char Name[64]; char stmp[64]; 
    char *Surface, *Field;

    w29 = gChrEditImgGeo[29].Width;
    w35 = gChrEditImgGeo[35].Width;
    h35 = gChrEditImgGeo[35].Height;

    win = WinCreateWindow( 17, 0, gChrEditImgGeo[29].Width, gChrEditImgGeo[29].Height, 256, 18 );
    if( win == -1 ) return -1;
    
    Surface = WinGetSurface( win );
    memcpy( Surface, gChrEditPix[ 29 ], gChrEditImgGeo[29].Width * gChrEditImgGeo[29].Height );

    Field = &Surface[13 * w29 + 13];
    ScrCopy( gChrEditPix[35], w35, h35, w35, Field, w29 );
    ScrCopyAlpha( gChrEditPix[30], gChrEditImgGeo[30].Width, gChrEditImgGeo[30].Height, gChrEditImgGeo[30].Width, &Surface[40 * w29 + 13], w29 );

    FontSet( 103 );
    gFont.Print( &Surface[44 * w29 + 50], MessageGetMessage( &gChrEditMsg, &gChrEditList, 100 ), w29, w29, gPalColorCubeRGB[18][17][3] );
    bt = WinCreateButton( win, 26, 44, gChrEditImgGeo[23].Width, gChrEditImgGeo[23].Height, -1, -1, -1, 500, gChrEditPix[23], gChrEditPix[24], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    WinUpdate( win );
    FontSet( 101 );
    strcpy( stmp, CritterGetName( gObjDude ) );
    if( !strcmp( stmp, "None" ) ) stmp[0] = '\0';
    strcpy( Name, stmp );
    if( CharEditLineEditor( win, 500, Name, 11, 23, 19, gPalColorCubeRGB[0][31][0], 100, 0) == -1 ){
        FontSet( 101 );
        ScrCopy( gChrEditPix[35], w35, h35, w35, Field, w29 );
    } else {
        if( Name[0] ){
            CritterSetName( Name );
            CharEditShowHeroName();
            WinClose( win );
            return 0;
        }
        FontSet(101);
        ScrCopy( gChrEditPix[35], w35, h35, w35, Field, w29 );
    }
    CharEditPrintName( Surface,  w29 );
    strcpy( Name, stmp );
    WinClose( win );
    return 0;
}

void CharEditPrintName( char *surf, int Width )
{
    char s[84];
    
    memset( s, 0, 64 );
    FontSet( 101 );
    strcpy( s, CritterGetName( gObjDude ) );
    gFont.Print( &surf[19 *  Width + 21], s,  Width,  Width, gPalColorCubeRGB[0][31][0] );
}

int CharEditDialogAge()
{
    char *p, *surf, FlgA;
    int h,win,bt,sel,Update,n,m,k,Val,Height,Width,DstPitch,Cnt;

    DstPitch = gChrEditImgGeo[ 29 ].Width;
    Width = gChrEditImgGeo[ 26 ].Width;
    h = gChrEditImgGeo[ 29 ].Height;
    Height = gChrEditImgGeo[ 26 ].Height;
    Val = FeatGetVal( gObjDude, 33 );
    if( (win = WinCreateWindow( gChrEditImgGeo[6].Width + 9, 0, DstPitch, h, 256, 18 ) ) == -1 ) return win;
    p = gChrEditPix[29];
    surf = WinGetSurface( win );
    memcpy( surf, p, DstPitch * h );
    ScrCopyAlpha( gChrEditPix[ 26 ], Width, Height, Width, &surf[7 * DstPitch + 8], DstPitch );
    ScrCopyAlpha( gChrEditPix[ 30 ], gChrEditImgGeo[ 30 ].Width, gChrEditImgGeo[ 30 ].Height, gChrEditImgGeo[ 30 ].Width, surf + 40 * DstPitch + 13, DstPitch );
    FontSet( 103 );
    gFont.Print( surf + 44 * DstPitch + 50, MessageGetMessage( &gChrEditMsg, &gChrEditList, 100 ), DstPitch, DstPitch, gPalColorCubeRGB[18][17][3] );
    FontSet( 101 );
    CharEditCounterDisplay( 55, 10, 0, FeatGetVal( gObjDude, 33 ), 0, win );
    bt = WinCreateButton( win, 26, 44, gChrEditImgGeo[ 23 ].Width, gChrEditImgGeo[ 23 ].Height, -1, -1, -1, 500, gChrEditPix[ 23 ], gChrEditPix[ 24 ], 0, 32 );
    if( bt != -1 ) WinSetClickSound(bt, GSoundPlayPushBt, GSoundPlayReleaseBt);
    bt = WinCreateButton( win, 105, 13, gChrEditImgGeo[ 37 ].Width, gChrEditImgGeo[ 37 ].Height, -1, 503, 501, 503, gChrEditPix[ 38 ], gChrEditPix[ 39 ], 0, 32 );
    if( bt != -1 ) WinSetClickSound(bt, GSoundPlayCheck, 0);
    bt = WinCreateButton( win, 19, 13, gChrEditImgGeo[ 39 ].Width, gChrEditImgGeo[ 39 ].Height, -1, 504, 502, 504, gChrEditPix[ 36 ], gChrEditPix[ 37 ], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayCheck, 0 );

    while( 1 ){
        gChrEditTime = TimerGetSysTime();
        sel = InpUpdate();
        Cnt = Update = FlgA = 0;

        if( sel == 27 || gMenuEscape ){
            FeatSetBase( gObjDude, 33, Val );
            CharEditShowHeroAge(); CharEditShowStats( 7, 0, 0 ); CharEditShowParams(); WinUpdate( gChrEditWin ); WinUpdate( win );
            WinClose( win );
            return 0;
        }
        switch( sel ){
    	    case 13:
		GSoundPlay( "ib1p1xx1" );
    	    case 500:
    		 WinClose( win );
    		 return 0;
            case 501:
                if( FeatGetVal( gObjDude, 33 ) >= 35 ) continue;
                Update = 1;
                break;
            case 333: case '+': case 'N':
                if( (k = FeatGetVal( gObjDude, 33 )) >= 35 ) continue;
                n = FeatIncVal( gObjDude, 33 ) >= 0;
                CharEditCounterDisplay( 55, 10, n, FeatGetVal( gObjDude, 33 ), k, win );
                if( n == 1 ){ CharEditShowHeroAge(); CharEditShowStats( 7, 0, 0 ); CharEditShowParams(); WinUpdate( gChrEditWin ); WinUpdate( win ); }
                break;
            case 502:
                if( FeatGetVal( gObjDude, 33 ) <= 16 ) continue;
                Update = -1;
                break;
            case 331: case '-': case 'J':
                if( (k = FeatGetVal( gObjDude, 33 )) <= 16 ) continue;
                n = FeatDecVal( gObjDude, 33 ) >= 0;
                CharEditCounterDisplay( 55, 10, n, FeatGetVal( gObjDude, 33 ), k, win );
                if( n == 1 ){ CharEditShowHeroAge(); CharEditShowStats( 7, 0, 0 ); CharEditShowParams(); WinUpdate( gChrEditWin ); WinUpdate( win ); } 
                break;
        }

        if( Update == 0 ){ WinUpdate( win ); TIMER_WAIT( gChrEditTime, 41 ); usleep( 41*100 ); continue; }
        gChrEditUnk65 = 4;
        do{
            gChrEditTime = TimerGetSysTime();
            m = 1;
            k = FeatGetVal( gObjDude, 33 );
            Cnt++;
            if( ( FlgA == 0 && (Cnt) == 1 ) || ( FlgA == 1 && Cnt > 14.4 ) ){
        	FlgA = 1;
        	if( Cnt > 14.4 && (++gChrEditUnk65 > 24) ) gChrEditUnk65 = 24;
        	k = FeatGetVal( gObjDude, 33 );
        	if( Update == 1 ){
            	    if( k < 35 ) n = FeatIncVal( gObjDude, 33 );                        
        	} else {
        	    if( k >= 16 ) n = FeatDecVal( gObjDude, 33 );
        	}
        	if( n < 0 ) m = 0; // error
        	CharEditCounterDisplay( 55, 10, m, FeatGetVal( gObjDude, 33 ), k, win );
    		if( m == 1 ){
            	    CharEditShowHeroAge();
            	    CharEditShowStats( 7, 0, 0 );
            	    CharEditShowParams();
            	    WinUpdate( gChrEditWin );
            	    WinUpdate( win );
        	}	
            }
            if( Cnt > 14.4 ){
                while( TimerCurrDiff( gChrEditTime ) < 1000 / gChrEditUnk65 );
            } else {
                while( TimerCurrDiff(gChrEditTime) < 41 );
            }
            k = InpUpdate();                        
        }while( (k != 503) && (k != 504) && !gMenuEscape );
    }
}

int CharEditDialogGender()
{
    char *p, *surf;
    int win, h, DoneBt, Val, sel, GenBt[2], DstPitch;

    DstPitch = gChrEditImgGeo[29].Width;
    h = gChrEditImgGeo[29].Height;
    win = WinCreateWindow( gChrEditImgGeo[25].Width + gChrEditImgGeo[6].Width + 9, 0, gChrEditImgGeo[29].Width, gChrEditImgGeo[29].Height, 256, 18 );
    if( win == -1 ) return -1;    
    p = gChrEditPix[ 29 ];
    surf = WinGetSurface( win );
    memcpy( surf, p, DstPitch * h );
    ScrCopyAlpha( gChrEditPix[30], gChrEditImgGeo[30].Width, gChrEditImgGeo[30].Height, gChrEditImgGeo[30].Width, &surf[44 * DstPitch + 15], DstPitch );
    FontSet( 103 );

    gFont.Print( &surf[48 * DstPitch + 52], MessageGetMessage( &gChrEditMsg, &gChrEditList, 100 ), DstPitch, DstPitch, gPalColorCubeRGB[18][17][3] );
    DoneBt = WinCreateButton( win, 28, 48, gChrEditImgGeo[23].Width, gChrEditImgGeo[23].Height, -1, -1, -1, 500, gChrEditPix[23], gChrEditPix[24], 0, 32 );
    if( DoneBt != -1 ) WinSetClickSound( DoneBt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    GenBt[0] = WinCreateButton( win, 22, 2, gChrEditImgGeo[34].Width, gChrEditImgGeo[34].Height, -1, -1, 501, -1, gChrEditPix[33], gChrEditPix[34], 0, 39 );
    if( GenBt[0] != -1 ) WinSetClickSound( GenBt[0], GSoundPlayPushBt, 0 );
    GenBt[1] = WinCreateButton( win, 71, 3, gChrEditImgGeo[32].Width, gChrEditImgGeo[32].Height, -1, -1, 502, -1, gChrEditPix[31], gChrEditPix[32], 0, 39 );
    if( GenBt[1] != -1 ){
        WinRadioButton( 2, GenBt );
        WinSetClickSound( GenBt[1], GSoundPlayPushBt, 0 );
    }
    Val = FeatGetVal( gObjDude, 34 );
    WinButtonSet( GenBt[ Val != 0 ], 1, 0 );
    while( 1 ){
        gChrEditTime = TimerGetSysTime();
        if( (sel = InpUpdate()) == 27 || gMenuEscape ){
            FeatSetBase( gObjDude, 34, Val );
            CharEditShowStats( 7, 0, 0 );
            CharEditShowParams();
            WinUpdate( gChrEditWin );
            CharEditShowHeroGender();
            WinClose( win );
            return 0;
        }
        switch ( sel ){
    	    case 13:
        	GSoundPlay("ib1p1xx1");
    	    case 500:
        	CharEditShowHeroGender();
        	WinClose( win );
        	return 0;
            case 331: case 333:
                Val = WinButtonState( GenBt[ 0 ] );
                WinButtonSet( GenBt[ 0 ], 1 - Val, 1 );
                WinButtonSet( GenBt[ 1 ], Val, 1 );
                break;
            case 501:
                FeatSetBase( gObjDude, 34, 0 );
                CharEditShowStats( 7, 0, 0 );
                CharEditShowParams();
                WinUpdate( gChrEditWin );
                break;
            case 502:
                FeatSetBase( gObjDude, 34, 1 );
                CharEditShowStats( 7, 0, 0 );
                CharEditShowParams();
                WinUpdate( gChrEditWin );
                break;
        }
        WinUpdate( win );
        TIMER_WAIT( gChrEditTime, 41 );
    }    
    return 0;
}

void CharEditStatModify( int Sel )
{
    int IdA, PrevVal, IdB, Val, Cnt, flg;

    Cnt = 0;
    gChrEditUnk65 = 4;
    PrevVal = gChrEditUnspentChrPts;
    if( gChrEditMenuMode == 0 ) return;    
    IdA = Sel - 503;
    IdB = Sel - 510;
    flg = -1;
    do{
        gChrEditTime = TimerGetSysTime();
        if( Cnt <= 19.2 ) Cnt++;
        if( (Cnt == 1) || (Cnt > 19.2) ){
            if( (Cnt > 19.2) && (++gChrEditUnk65) > 24 ) gChrEditUnk65 = 24;
            flg = 1;
            if( Sel >= 510 ){
                if( FeatDecVal( gObjDude, IdB ) >= 0 )
                    gChrEditUnspentChrPts++;
                else
                    flg = 0;
                CharEditShowStats( IdB, flg, FeatGetVal( gObjDude, IdB ) );
                CharEditCounterDisplay( 126, 282, flg, gChrEditUnspentChrPts, PrevVal, gChrEditWin );
                FeatStatsRecalculate( gObjDude );
                CharEditShowParams();
                CharEditShowSkills( 0 );
                gChrEditSelected = Sel - 510;
            } else {
                if( gChrEditUnspentChrPts <= 0 ){
        	    flg = 0;
                } else {
            	    Val = FeatGetBoost( gObjDude, IdA ) + FeatGetTotal( gObjDude, IdA );
            	    if( (Val >= 10) || FeatIncVal( gObjDude, IdA ) < 0 ){
        		flg = 0;
            	    } else {
                	gChrEditUnspentChrPts--;
        	    }
        	}
                CharEditShowStats( IdA, flg, Val );
                CharEditCounterDisplay( 126, 282, flg, gChrEditUnspentChrPts, PrevVal, gChrEditWin );
                FeatStatsRecalculate( gObjDude );
                CharEditShowParams();
                CharEditShowSkills( 0 );
                gChrEditSelected = Sel - 503;
            }
            WinUpdate( gChrEditWin );
        }
        if( Cnt >= 19.2 ){
            while( TimerCurrDiff( gChrEditTime ) < (1000 / gChrEditUnk65) );
        } else {
            while( TimerCurrDiff( gChrEditTime ) < 41 );
        }
    }while( InpUpdate() != 518 && flg );
    CharEditDscUpdate();    
}

int CharEditPrint()
{
    int ExCode,v68,a2,bt,v243,win;
//    xPath_t *v84;
//    char **FileList;
//    char stmp1[512];
//    char stmp2[512];
    char stmp[512];
//    char v236[512];
    char *Frame0[10], *Surface;
//    char *Str2[2];
    int err,i,k,n,ww;
//    int v131;
    
//    Str2[0] = stmp2;
//    Str2[1] = v236;
    a2 = 17;
    if( gChrEditMenuMode ){
        if( (win = WinCreateWindow(238, 90, gChrEditImgGeo[41].Width, gChrEditImgGeo[41].Height, 256, 18)) == -1 ) return -1;
        Surface = WinGetSurface( win );
        memcpy( Surface, gChrEditPix[41], gChrEditImgGeo[41].Width * gChrEditImgGeo[41].Height );
        FontSet( 103 );
        v243 = gChrEditImgGeo[43].Height + 3;
        ww = 7 * gChrEditImgGeo[43].Width;        
        n = gChrEditImgGeo[43].Height * gChrEditImgGeo[43].Width;
        for( i = err = 0; i < 5; i++, a2 += v243 ){
            if( !(Frame0[i + 5] = Malloc( n )) ){ err = 1; break; }
            if( !(Frame0[i + 0] = Malloc( n )) ){ err = 2; break; }
            memcpy( Frame0[i + 5], gChrEditPix[43], n );
            memcpy( Frame0[i + 0], gChrEditPix[42], n );
            strcpy( stmp, MessageGetMessage( &gChrEditMsg, &gChrEditList, 600 + i ) );
            k = ww + (gChrEditImgGeo[43].Width / 2) - (gFont.LineWidth( stmp ) >> 1);
            gFont.Print( &Frame0[i + 0][ k ], stmp, gChrEditImgGeo[43].Width, gChrEditImgGeo[43].Width, gPalColorCubeRGB[18][17][3] );
            gFont.Print( &Frame0[i + 5][ k ], stmp, gChrEditImgGeo[43].Width, gChrEditImgGeo[43].Width, gPalColorCubeRGB[14][12][3] );
            bt = WinCreateButton( win, 13, a2, gChrEditImgGeo[43].Width, gChrEditImgGeo[43].Height, -1, -1, -1, 500 + i, Frame0[i + 0], Frame0[i + 5], 0, 32 );
            if( bt != -1 ) WinSetClickSound( bt, (void *)GSoundPlayKnobLt, 0 );
        }
        if( err ){
            if( err == 2 ) Free( Frame0[ i + 5 ] );
            for( ; i > 0; i-- ){ Free( Frame0[ i + 0 ] ); Free( Frame0[ i + 5 ] ); }
            return -1;
        }
        FontSet( 101 );

        ExCode = 0;
        while( 1 ){
            v68 = InpUpdate();
            if( gMenuEscape ){ ExCode = 2; }
            else if( v68 == 504 ){ ExCode = 2; }
            else if( v68 == 13 || v68 == 100 || v68 == 68 ){ ExCode = 2; GSoundPlay("ib1p1xx1"); }
            else if( v68 == 27 ){ ExCode = 2; }
            else if( v68 == 500 || v68 == 115 || v68 == 83 ){ // Save
                stmp[0] = '\0';
                strcpy( &stmp[ strlen( stmp ) ], "*." );
                strcpy( &stmp[ strlen( stmp ) ], "GCD" );
//                if( (v84 = dbGetFileList( stmp, &FileList )) != dbFAIL ){
//                    strcpy( stmp1, MessageGetMessage( &gChrEditMsg, &gChrEditList, 617 ) );
//                    strcpy( stmp, MessageGetMessage( &gChrEditMsg, &gChrEditList, 600 ) );
//                    if( !DlgBoxSave( stmp, FileList, stmp1, v84->nsize, 168, 80 ) ){
/*
                        strcpy( &stmp1[ strlen( stmp1 ) ], "." );
                        strcpy( &stmp1[ strlen( stmp1 ) ], "GCD" );
                        stmp[0] = '\0';
                        strcpy( &stmp[ strlen( stmp ) ], stmp1 );
                        if( CharEditFileExist( stmp ) != 1 ){
                    	    ExCode = 1;
                        } else {
                    	    sprintf( stmp, "%s %s", StrUpr( v100 ), MessageGetMessage( &gChrEditMsg, &gChrEditList, 609 ) );
                    	    strcpy( stmp2, MessageGetMessage( &gChrEditMsg, v103, 610 ) );
                    	    ExCode = ( DlgBox(stmp, Str2, 1, 169, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 16) ) ? 1 : 0;
			}
                        if( ExCode ){
                            SkillSetSpecials( gChrEditTagSkill, 4 );
                            TraitSetSpec( gChrEditOptionTraits[ 0 ], gChrEditOptionTraits[ 1 ] );
                            stmp[ 0 ] = '\0';
                            strcpy( &stmp[ strlen( stmp ) ], stmp1 );
                            if( CritterSaveAllStats( stmp ) ){
                                GSoundPlay( "iisxxxx1" );
                                sprintf( stmp, "%s%s!", MessageGetMessage( &gChrEditMsg, &gChrEditList, 611), StrUpr( var_a80 ) );
                                ExCode = 0;
                                DlgBox( stmp, NULL, 0, 169, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 1 );
                            } else {
                                sprintf( stmp, "%s%s", StrUpr( var_a80 ), MessageGetMessage( &gChrEditMsg, &gChrEditList, 607 ) );
                                ExCode = 1;
                                DlgBox( stmp, NULL, 0, 169, 126, gPalColorCubeRGB[0][31][0], NULL, gPalColorCubeRGB[0][31][0], 1 );
                            }
                        }
*/
//                    }
//                    dbDelFileList( FileList );
//                } else {
//            	    GSoundPlay( "iisxxxx1" );
//            	    ExCode = 0;
//            	    DlgBox( MessageGetMessage( &gChrEditMsg, &gChrEditList, 615 ), NULL, 0, 169, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 0);
//                }
            } else if( v68 == 501 || v68 == 108 || v68 == 76 ){ // Load
                    stmp[0] = '\0';
                    strcpy( &stmp[ strlen( stmp ) ], "*." );
                    strcpy( &stmp[ strlen( stmp ) ], "GCD" );
//                    if( (v84 = dbGetFileList( stmp, &FileList ) ) != dbFAIL ){
//                        v131 = DlgBoxLoad( MessageGetMessage( &gChrEditMsg, &gChrEditList, 601 ), FileList, stmp, v84->nsize, 168, 80 );
//                        if( v131 == -1 ){ dbDelFileList( FileList ); return -1; }
/*
                        if( !v131 ){
                            stmp[0] = '\0';
                            strcpy( &stmp[strlen(stmp)], v237 );
                            ProtoReset();
                            if( CritterLoadAllStats( stmp ) ){
                                CharEditRestoreObj();
                                gChrEditUnspentChrPts = v136;
                                CritterHeal( gObjDude, 1000 );
                                GSoundPlay( "iisxxxx1" );
                                strcpy( stmp, MessageGetMessage((Msg_t **)&gChrEditMsg, v138, 612) );
                                strcpy( &stmp[strlen(stmp)], v237 );
                                strcpy( &stmp[strlen(stmp)], "!" );
                                ExCode = 0;
                                DlgBox( stmp, NULL, 0, 169, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 0 );
                            } else {
                                FeatStatsRecalculate( gObjDude );
                                FeatPtsReset();
                                for( i = 0; i < 35; i++ ){ FeatSetBoost(gObjDude, i, 0); }
                                PerkZero();
                                FeatStatsRecalculate( gObjDude );
                                SkillGetSpecials( gChrEditTagSkill, v154 );
                                for( i = 0; i < 4; i++){
                                    if( gChrEditTagSkill[ 3 - i ] != -1 ) break;
                                }
                                if( gChrEditMenuMode == 1 ) i--;
                                gChrEditTaggedSkillsCnt = i;
                                TraitGetSpec(gChrEditOptionTraits, &gChrEditOptionTraits[1]);
                                for( i = 0; i < 2; i++){
                                    if( gChrEditOptionTraits[ 1 - i ] != -1 ) break;
                                }
                                gChrEditOptionalTraitsCnt = i;
                                FeatStatsRecalculate( gObjDude );
                                ExCode = 1;
                                CritterHeal(gObjDude, 1000);
                            }
                            CharEditUnk57();
                        }
                        dbDelFileList( FileList );
*/
//                } else {
//            	    GSoundPlay("iisxxxx1");
//                strcpy( stmp, MessageGetMessage((Msg_t **)&gChrEditMsg, v124, 615) );
//		}
            } else if( v68 != 502 && v68 != 112 && v68 != 80 ){
/*
                        if( v68 == 503 || v68 == 101 || v68 == 69 ){
                            strcpy( stmp2, MessageGetMessage((Msg_t **)&gChrEditMsg, &gChrEditList, 605) );
                            strcpy( v236, MessageGetMessage((Msg_t **)&gChrEditMsg, &gChrEditList, 606) );
                            if( DlgBox( 0, Str2, 2, 169, 126, gPalColorCubeRGB[0][31][0], NULL, gPalColorCubeRGB[0][31][0], 16) ){
                                ProtoReset();
                                SkillGetSpecials(gChrEditTagSkill, 4);
                                for( i = 0;i < 4; i++){
                                    if( gChrEditTagSkill[ 3 - i ] != -1 ) break;
                                }
                                if( gChrEditMenuMode == 1 ) i--;
                                gChrEditTaggedSkillsCnt = i;
                                TraitGetSpec(gChrEditOptionTraits, &gChrEditOptionTraits[1]);
                                for( i =0; i < 2; i++){
                                    if( gChrEditOptionTraits[ 1 - i ] != -1 ) break;
                                }
                                gChrEditOptionalTraitsCnt = i;
                                FeatStatsRecalculate(gObjDude);
                                CharEditUnk57();
                            }
                        }
                        goto LABEL_233;
                    }
                    stmp[0] = 0;
                    strcpy( &stmp[strlen(stmp)], "*." );
                    strcpy( &stmp[strlen(stmp)], &off_501724 );
                    if( dbGetFileList(stmp, FileList) != -1 ){
                        strcpy( stmp1, MessageGetMessage((Msg_t **)&gChrEditMsg, &gChrEditList, 616) );
                        strcpy( stmp, MessageGetMessage((Msg_t **)&gChrEditMsg, &gChrEditList, 602) );
                        if( !DlgBoxSave( stmp, FileList[0], v175, stmp1, 168, 80, 0 ) ){
                            strcpy( &stmp1[strlen(stmp1)], &dword_501728 );
                            strcpy( &stmp1[strlen(stmp1)], &off_501724 );
                            stmp[0] = 0;
                            strcpy( &stmp[strlen(stmp)], stmp1 );
                            if( CharEditFileExist(stmp) != 1 ){
                        	ExCode = 1;
                            } else {
                        	sprintf(stmp, "%s %s", StrUpr(v191), MessageGetMessage( &gChrEditMsg, &gChrEditList, 609 ) );
                        	strcpy( stmp2, MessageGetMessage( &gChrEditMsg, v194, 610) );
                        	ExCode = DlgBox(stmp, Str2, 1, 169, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 16) ? 1:0;
                            }
                            if( ExCode ){
                                stmp[0] = 0;
                                strcpy( &stmp[strlen(stmp)], stmp1 );
                                if( !CharEditPrintToFile(stmp) ){
                                    stmp[0] = 0;
                                    strcpy( &stmp[strlen(stmp)], stmp1 );
                                    if( CharEditPrintToFile(stmp) ){
                                        GSoundPlay("iisxxxx1");
                                        sprintf(stmp, "%s%s%s", MessageGetMessage( &gChrEditMsg, &gChrEditList, 611), StrUpr(v208), "!");
                                        ExCode = 0;
                                        DlgBox(stmp, NULL, 0, 169, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[0][31][0], 1);
                                    } else {
                                        sprintf(stmp, "%s%s", StrUpr(v206), MessageGetMessage( &gChrEditMsg, &gChrEditList, 607 ) );
                                        ExCode = 1;
                                        DlgBox(stmp, NULL, 0, 169, 126, gPalColorCubeRGB[0][31][0], NULL, gPalColorCubeRGB[0][31][0], 0);
                                    }
                                }
                            }
                        }
                        goto LABEL_161;
                    }
                    GSoundPlay("iisxxxx1");
		    strcpy( stmp, MessageGetMessage( &gChrEditMsg, v166, 615) );
                }
                ExCode = 0;
                DlgBox(stmp, NULL, 0, 169, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 0);
*/
            }
//LABEL_233:
            WinUpdate( win );
            if( ExCode ){
                WinClose( win );
                for( i = 0; i < 5; i++){
                    Free( Frame0[i + 0] );
                    Free( Frame0[i + 5] );
                }
                return 0;
            }
        }
    }
/*
    stmp[0] = 0;
    strcpy( &stmp[strlen(stmp)], "*." );
    strcpy( &stmp[strlen(stmp)], off_501724 );
    if( dbGetFileList(stmp, FileList) == -1 ){
        GSoundPlay("iisxxxx1");
        strcpy( stmp, MessageGetMessage( &gChrEditMsg, v9, 615) );
        DlgBox( stmp, NULL, 0, 169, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 0);
        return 0;
    }
    strcpy( stmp1, MessageGetMessage( &gChrEditMsg, &gChrEditList, 616) );
    strcpy( stmp, MessageGetMessage( &gChrEditMsg, &gChrEditList, 602) );
    if( !DlgBoxSave( stmp, FileList[0], v18, stmp1, 168, 80, 0) ){
        strcpy( &stmp1[strlen(stmp1)], &dword_501728 );
        strcpy( &stmp1[strlen(stmp1)], off_501724 );
        stmp[0] = 0;
        strcpy( &stmp[strlen(stmp)], stmp1 );
        if( CharEditFileExist(stmp) != 1 ){
         err = 1;
        } else {
    	    sprintf(stmp, "%s %s", StrUpr(v34), MessageGetMessage(&gChrEditMsg, &gChrEditList, 609));
    	    strcpy( stmp2, MessageGetMessage( &gChrEditMsg, v37, 610 ) );
    	    err = DlgBox( stmp, Str2, 1, 169, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 16);
    	    if( err ) err = 1;
    	}
        if( err ){
            stmp[0] = 0;
            strcpy( &stmp[strlen(stmp)], stmp1 );
            if( CharEditPrintToFile(stmp) ){
                GSoundPlay("iisxxxx1");
                sprintf( stmp, "%s%s%s", MessageGetMessage( &gChrEditMsg, &gChrEditList, 611), StrUpr(v46), "!" );
                DlgBox( stmp, NULL, 0, 169, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 1 );
            }
        }
    }
    dbDelFileList( FileList );
*/
    return 0;
}

int CharEditFileExist( const char *fname )
{
    xFile_t * fh;
    
    if( !(fh = dbOpen(fname, "rb")) ) return 0;
    dbClose( fh );
    return 1;
}

int CharEditPrintToFile( char *fname )
{
/*
    xFile_t *fh;
    int n;        
    char stmp1[256], stmp2[256], stmp3[256];
    
    if( !(fh = dbOpen( fname, "wt" )) ) return -1;
    dbputs( str, fh );
    dbputs( str, fh );
    strcpy( stmp2, MessageGetMessage( &gChrEditMsg, &gChrEditList, 620 ) );
    stmp1[0] = '\0'; n = ((80 - strlen(stmp2)) >> 1) - 2; if( n > 0 ){ for( s = &stmp1[strlen(stmp1)]; --n != -1; s++ ) *s = ' '; *s = '\0'; }
    strcpy( &stmp1[strlen(stmp1)], stmp2 );
    strcpy( &stmp1[strlen(stmp1)], str );
    dbputs(stmp1, fh);
    strcpy( stmp2, MessageGetMessage( &gChrEditMsg, &gChrEditList, 621 ) );
    stmp1[0] = '\0'; n = ((80 - strlen(stmp2)) >> 1) - 2; if( n > 0 ){ for( s = &stmp1[strlen(stmp1)]; --n != -1; s++ ) *s = ' '; *s = '\0'; }
    strcpy( &stmp1[strlen(stmp1)], stmp2 );
    strcpy( &stmp1[strlen(stmp1)], str );
    dbputs(stmp1, fh);
    sub_4A3338(&v192, &v191, &v190);
    sprintf( stmp2, "%.2d %s %d  %.4d %s", v191, MessageGetMessage( &gChrEditMsg, &gChrEditList, v192 + 499), v190, sub_4A33C8(), MessageGetMessage( &gChrEditMsg, &gChrEditList, 622));
    stmp1[0] = '\0'; n = ((80 - strlen(stmp2)) >> 1) - 2; if( n > 0 ){ for( s = &stmp1[strlen(stmp1)]; --n != -1; s++ ) *s = ' '; *s = '\0'; }
    strcpy( &stmp1[strlen(stmp1)], stmp2 );
    strcpy( &stmp1[strlen(stmp1)], str );
    dbputs( stmp1, fh );
    dbputs( str, fh );
    sprintf( stmp1, "%s %s", MessageGetMessage( &gChrEditMsg, &gChrEditList, 642), CritterGetName(gObjDude) );

    if( (n = 27 - strlen(stmp1) - 2) > 0 ){
        stmp2[0] = '\0'; if( n > 0 ){ for( s = &stmp2[strlen(stmp2)]; --n != -1; s++ ) *s = ' '; *s = '\0'; }
        strcpy( &stmp1[strlen(stmp1)], stmp2 );
    }
    sprintf( stmp2, "%s%s %d", stmp1, MessageGetMessage( &gChrEditMsg, &gChrEditList, 643), FeatGetVal(gObjDude, 33) );
    sprintf( stmp3, "%s%s %s", stmp2, MessageGetMessage( &gChrEditMsg, &gChrEditList, 644), MessageGetMessage( &gChrEditMsg, &gChrEditList, FeatGetVal(gObjDude, 34) + 645) );
    dbputs( stmp3, fh ); dbputs( str, fh );

    sprintf( stmp1, "%s %.2d %s %s ", 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 647), FeatGetPoints(1u), 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 648), CharEditValToStr( FeatGetPoints(2), v59) );

    n = 12 - ((strlen(stmp2) + 1) - 1);
    if( n > 0 ){        
        stmp2[0] = 0;
        if( n > 0 ){ for( s = &stmp2[ strlen( stmp2 ) ]; --n != -1; s++ ) *s = ' '; *s = '\0'; }
	strcpy( &stmp1[strlen(stmp1)], stmp2 );
    }
    sprintf( stmp2, "%s%s %s", stmp1, MessageGetMessage( &gChrEditMsg, &gChrEditList, 649), CharEditValToStr( FeatNextLvlPts(), stmp3) );
    dbputs( stmp2, fh ); dbputs( str, fh ); dbputs( str, fh );

    sprintf( stmp1, "%s\n", MessageGetMessage( &gChrEditMsg, &gChrEditList, 623 ) );
    dbputs( stmp1, fh );

    sprintf( stmp1, "%s %.2d %s %.3d/%.3d %s %.2d", 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 624), FeatGetVal(gObjDude, 0), 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 625), CritterGetHp(gObjDude), FeatGetVal(gObjDude, 7), 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 626), FeatGetVal(gObjDude, 0) );
    dbputs(stmp1, fh); dbputs(str, fh);

    sprintf(stmp1, "%s %.2d %s %.3d %s %.2d", 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 627), FeatGetVal(gObjDude, 1), 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 628), FeatGetVal(gObjDude, 9), 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 629), FeatGetVal(gObjDude, 14) );
    dbputs( stmp1, fh ); dbputs( str, fh );

    sprintf(stmp1, "%s %.2d %s %.2d %s %.3d%%", 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 630), FeatGetVal(gObjDude, 2), 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 631), FeatGetVal(gObjDude, 8), 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 632), FeatGetVal(gObjDude, 15) );
    dbputs( stmp1, fh); dbputs( str, fh);
    
    sprintf(stmp1, "%s %.2d %s %.2d %s %.3d lbs.", 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 633), FeatGetVal( gObjDude, 3 ), 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 634), FeatGetVal( gObjDude, 11 ), 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 635), FeatGetVal( gObjDude, 12 ) );
    dbputs( stmp1, fh ); dbputs( str, fh );

    sprintf(stmp1, "%s %.2d %s %.3d%%", 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 636), FeatGetVal(gObjDude, 4), 
	MessageGetMessage( &gChrEditMsg, &gChrEditList, 637), FeatGetVal(gObjDude, 24) );
    dbputs(stmp1, fh); dbputs(str, fh);

    sprintf( stmp1, "%s %.2d %s %.3d%%", MessageGetMessage( &gChrEditMsg, &gChrEditList, 638), FeatGetVal(gObjDude, 5), MessageGetMessage(&gChrEditMsg, &gChrEditList, 639), FeatGetVal(gObjDude, 31) );
    dbputs( stmp1, fh ); dbputs( str, fh );

    sprintf( stmp1, "%s %.2d %s %.3d%%", MessageGetMessage( &gChrEditMsg, &gChrEditList, 640), FeatGetVal(gObjDude, 6), MessageGetMessage( &gChrEditMsg, &gChrEditList, 641), FeatGetVal(gObjDude, 32) );
    dbputs( stmp1, fh ); dbputs( str, fh ); dbputs( str, fh ); dbputs( str, fh );

    if( gChrEditOptionTraits[0] != -1 ){
        sprintf( stmp1, "%s\n", MessageGetMessage( &gChrEditMsg, &gChrEditList, 650 ) );
        dbputs( stmp1, fh );
        if( gChrEditOptionTraits[0] != -1 ){
            sprintf( stmp1, "  %s", TraitGetName( gChrEditOptionTraits[0] ) );
            dbputs( stmp1, fh );
            dbputs( str, fh );
        }
        if( gChrEditOptionTraits[1] != -1 ){
            sprintf( stmp1, "  %s", TraitGetName( gChrEditOptionTraits[1] ) );
            dbputs( stmp1, (xFile_t *)fh );
            dbputs( str, fh );
        }
    }
    for( i = 0; i < 119; i++ ){
        if( PerkLvl(gObjDude, i) ) break;
    }
    if( i < 119 ){
        sprintf( stmp1, "%s\n", MessageGetMessage( &gChrEditMsg, &gChrEditList, 651 ) );
        dbputs( stmp1, fh );
        for( i = 0; i < 119; i++ ){
            n = PerkLvl(gObjDude, i);
            if( n ){
                if( n <= 1 ){
                    sprintf( stmp1, "  %s", PerkGetName(i) );
                } else {
                    sprintf( stmp1, "  %s (%d)", PerkGetName( i ), n );
                }
                dbputs( stmp1, fh );
                dbputs( str, fh );
            }
        }
    }
    dbputs( str, fh );
    sprintf( stmp1, "%s\n", MessageGetMessage((Msg_t **)&gChrEditMsg, &gChrEditList, 652) );
    dbputs( stmp1, fh );
    for( i = 0; i < gChrEditMsgKarmaCount; i++ ){
        if( (n = gCharEditKarma[ i ].Val) ){
            if( gGVals[ n ] ){
                sprintf( stmp1, "  %s", MessageGetMessage( &gChrEditMsg, &gChrEditList, gCharEditKarma[ i ].TitleId ) );
                dbputs( stmp1, fh );
                dbputs( str, fh );
            }
        } else if( gCharEditGenreCount > 0 ){                
            for( j = 0; *gGVals < gCharEditGenre[ j ].i01; j++ ){
                if( (j+1) >= gCharEditGenreCount ) continue;
            }
            sprintf( stmp3, "%d", *gGVals);
            sprintf( stmp1, "  %s: %s (%s)", 
        	    MessageGetMessage( &gChrEditMsg, &gChrEditList, 125), stmp3, 
        	    MessageGetMessage(&gChrEditMsg, &gChrEditList, gCharEditGenre[ j ].i02) );
            dbputs( stmp1, fh );
            dbputs( str, fh );
        }
    }
    for( flg = i = 0; i != 19; i++ ){
        if( Unk8002GetLocationKarma( gChrEditKarma1[1] + i ){
            if( !flg ){
                dbputs( str, fh );
                sprintf( stmp1, "%s\n", MessageGetMessage( &gChrEditMsg, &gChrEditList, 657 ) );
                flg = 1;
                dbputs( stmp1, fh );
            }
            Unk8001GetLocationName( &gChrEditKarma1[1] + i, i );
            n = gGVals[ gChrEditKarma1 + i ];
            if( n < 30 ){
                if( n < 15 ){
                    if( n <= 0 ){
                        if( n ){
                            if( n < -15 ){
                                if ( n < -30 )
                                    n = 2006;
                                else
                                    n = 2005;
                            } else { n = 2004; }
                        } else { n = 2003; }
                    } else { n = 2002; }
                } else { n = 2001; }
            } else { n = 2000; }
            sprintf( stmp1, "  %s: %s", v188, MessageGetMessage( &gChrEditMsg, &gChrEditList, n ) );
            dbputs( stmp1, fh );
            dbputs( str, fh );
        }        
    }
    for( flg = i = 0; i < 8; i++ ){
        if( gGVals[ gChrEditKarmaIds1[ i ] ] ){
            if( !flg ){
                dbputs( str, fh );
                sprintf( stmp1, "%s\n", MessageGetMessage( &gChrEditMsg, &gChrEditList, 656 ) );
                flg = 1;
                dbputs( stmp1, fh );
            }
            sprintf( stmp1, "  %s", MessageGetMessage( &gChrEditMsg, &gChrEditList, 1004 + i ) );
            dbputs( stmp1, fh );
            dbputs( str, fh );
        }
     }
    dbputs(str, fh);
    sprintf( stmp1, "%s\n", MessageGetMessage( &gChrEditMsg, &gChrEditList, 653 ) );
    dbputs( stmp1, fh );        
    for( i = 0; i < 18; i++ ){
        sprintf(stmp1, "%s ", SkillGetName( i ) );
        n = 16 - strlen(stmp1);
        if( n > 0 ){ for( s = &stmp1[strlen(stmp1)]; --n != -1; s++ ) *s = '.'; *s = '\0'; }
        flg = 0;
        for( j = 0; j < 19; j++ ){
            if( CritterGetKillStat( j ) <= 0 ) continue;
            sprintf( stmp2, "%s ", CritterKillTypeName( j ) );
            if( v102 > 0 ){
                kk = &stmp2[ strlen( stmp2 ) ];
                for( v102 = 16 - strlen(stmp2); --v102 != -1; ++kk ) *kk = '.';                    
                *kk = '\0';
            }
            v180 = v101;
            Total = SkillGetTotal( gObjDude, i );
            flg = 1;
            j++;
            sprintf(stmp3, "  %s %.3d%%        %s %.3d\n", stmp1, Total, stmp2, v180 );
            break;
        }        
        if( !flg ) sprintf( stmp3, "  %s %.3d%%\n", stmp1, SkillGetTotal( gObjDude, i ) );
        dbputs( stmp3, fh );
    }
    dbputs( str, fh );
    dbputs( str, fh );
    sprintf( stmp1, "%s\n", MessageGetMessage( &gChrEditMsg, &gChrEditList, 654 ) );
    dbputs( stmp1, fh);
    p_Effect = &gObjDude->Critter;    
    for( i = 0; i < p_Effect->Cnt; i += 3 ){
        stmp1[0] = '\0';        
        for( j = i; j < i + 3; j++ ){
            if( j >= p_Effect->Cnt ) break;
            sprintf( stmp2, "  %sx %s", CharEditValToStr( p_Effect->Ptr + j + 4, stmp3), ObjGetName( p_Effect->Ptr + j ) );
            n = 25 - strlen( stmp2 );
            if( n < 0 ) n = 0;
	    strcpy( &stmp1[ strlen( stmp1 ) ], CharEditStrClear( stmp2, n ) );
        }
	strcpy( &stmp1[ strlen( stmp1 ) ], str );
        dbputs( stmp1, fh );        
    }
    dbputs( str, fh );
    sprintf( stmp1, "%s %d lbs.", MessageGetMessage( &gChrEditMsg, &gChrEditList, 655 ), ItemGetBackPackWeight( gObjDude ) );
    dbputs( stmp1, fh );
    dbputs( str, fh );
    dbputs( str, fh );
    dbputs( str, fh );
    dbClose( fh );
*/
    return 0;
}

void CharEditUnk57()
{
    gChrEditSelected = 0;
    gChrEditSkillSelect = 0;
    gChrEditSkillSelPosY = 27;
    gChrEditFolderType = 0;
    if( gChrEditMenuMode ){
        CharEditCounterDisplay(126, 282, 0, gChrEditUnspentChrPts, 0, gChrEditWin);
    } else {
        CharEditShowFolder();
        CharEditShowLevelInfo();
    }
    CharEditShowHeroName();
    CharEditShowHeroAge();
    CharEditShowHeroGender();
    CharEditShowTraits();
    CharEditShowSkills(0);
    CharEditShowStats(7, 0, 0);
    CharEditShowParams();
    CharEditDscUpdate();
    WinUpdate(gChrEditWin);
}

void CharEditStrClear( char *str, int len )
{
    char *s;

    if( len <= 0 ) return;
    for( s = str + strlen( str ); --len != -1; s++ ) *s = ' ';
    *s = '\0';
}

void CharEditStrFillDot( char *str, int len )
{
    char *s;

    if( len <= 0 ) return;
    for( s = str + strlen( str ); --len != -1; s++ ) *s = '.';
    *s = '\0';
}

int CharEditAreasDefine()
{
    WinCreateButton( gChrEditWin, 19, 38, 125, 227, -1, -1, 525, -1, 0, 0, 0, 0 );
    WinCreateButton( gChrEditWin, 28, 280, 124, 32, -1, -1, 526, -1, 0, 0, 0, 0 );
    if( gChrEditMenuMode ){
        WinCreateButton( gChrEditWin, 52, 324, 169, 20, -1, -1, 533, -1, 0, 0, 0, 0 );
        WinCreateButton( gChrEditWin, 47, 353, 245, 100, -1, -1, 534, -1, 0, 0, 0, 0 );
    } else {
        WinCreateButton( gChrEditWin, 28, 363, 283, 105, -1, -1, 527, -1, 0, 0, 0, 0 );
    }
    WinCreateButton( gChrEditWin, 191, 41, 122, 110, -1, -1, 528, -1, 0, 0, 0, 0 );
    WinCreateButton( gChrEditWin, 191, 175, 122, 135, -1, -1, 529, -1, 0, 0, 0, 0 );
    WinCreateButton( gChrEditWin, 376, 5, 223, 20, -1, -1, 530, -1, 0, 0, 0, 0 );
    WinCreateButton( gChrEditWin, 370, 27, 223, 195, -1, -1, 531, -1, 0, 0, 0, 0 );
    return WinCreateButton( gChrEditWin, 396, 228, 171, 25, -1, -1, 532, -1, 0, 0, 0, 0 );
}

int CharEditCharReset()
{
    signed int i;

    FeatStatsRecalculate( gObjDude );
    FeatPtsReset();
    for( i = 0; i < 35; i++ ) FeatSetBoost( gObjDude, i, 0 );
    PerkZero();
    FeatStatsRecalculate( gObjDude );
    return 1;
}

void CharEditStoreObj()
{
    Proto_t *proto;
    int i;

    ProtoGetObj( gObjDude->Pid, &proto );
    CritterDuplicate( &gChrEditSaveCritter, &proto->Critt );
    gChrEditSaveHp = CritterGetHp( gObjDude );
    strncpy( gChrEditSaveName, CritterGetName( gObjDude ), 32 );
    gChrEditTmpLvl = gChrEditCurLvl;
    for( i = 0; i < 119; i++ ){
        gChrEditSavePerks[ i ] = PerkLvl( gObjDude, i );
    }
    gChrEditTmpPerks = gChrEditNewPerkFlag;
    gChrEditUnspentSkillPt = FeatGetPoints( FEAT_PT_SP );
    SkillGetSpecials( gChrEditSkillSpecial, 4 );
    TraitGetSpec( &gChrEditOptTraits[ 1 ], &gChrEditOptTraits[ 2 ] );
    if( !gChrEditMenuMode ){
        for( i = 0; i < 18; i++ ){
            gChrEditSkillList[ i ] = SkillGetTotal( gObjDude, i );
        }
    }
}

void CharEditRestoreObj()
{
    int i;
    Proto_t *proto;

    CharEditPerksApply();
    ProtoGetObj( gObjDude->Pid, &proto );

    CritterDuplicate( &proto->Critt, &gChrEditSaveCritter );
    CritterSetName( gChrEditSaveName );

    gChrEditCurLvl = gChrEditTmpLvl;
    gChrEditNewPerkFlag = gChrEditTmpPerks;
    FeatSetPoints( 0, gChrEditUnspentSkillPt );
    SkillSetSpecials( gChrEditSkillSpecial, 4 );
    TraitSetSpec( gChrEditOptTraits[1], gChrEditOptTraits[2] );
    SkillGetSpecials( gChrEditTagSkill, 4 );
    for( i = 0; i < 4 && (gChrEditTagSkill[ 3 - i ] == -1); i++ );
    if( gChrEditMenuMode == 1 ) i--;
    gChrEditTaggedSkillsCnt = i;
    TraitGetSpec( gChrEditOptionTraits, &gChrEditOptionTraits[ 1 ] );
    for( i = 0; i < 2 && (gChrEditOptionTraits[ i ] == -1); i++ );
    gChrEditOptionalTraitsCnt = i;
    FeatStatsRecalculate( gObjDude );
    CritterHeal( gObjDude, gChrEditSaveHp - CritterGetHp( gObjDude ) );
}

char *CharEditValToStr( int Val, char *str )
{
    char tmp[64];
    int tb[7];
    char *dst;
    int Value, i, flg;

    dst = str;
    memcpy( tb, gCharEditDigBase, 7 * sizeof( int ) );
    if( Val == 0 ){
        str[0] = '0';
        str[1] = '\0';
	return dst;
    }
    str[0] = '\0';
    flg = 0;
    for( i = 0; i < 7; i++ ){
        Value = Val / tb[ i ];
        if( Value > 0 || flg ){
    	    sprintf( tmp, "%i", Value );
            strcpy( dst + strlen( dst ), tmp );
            flg = 1;
            Val -= tb[ i ] * Value;
            if( (i == 0) || i == 3 ) strcpy( &dst[ strlen( dst ) ], "," );            
        }
    }
    return dst;
}

int CharEditCaption( int ImgId, char *Title, char *Param, char *Dsc )
{
    int LineWidth, i, j, Height, Width, h, y;
    char *surf;
    char tmp, *s;
    short LinOffs[64], Lines;
    CachePool_t *Img;

    if( !(surf = ArtLoadBmp( ArtMakeId( 10, ImgId, 0, 0, 0 ), &Img, &Width, &Height )) ) return -1;
    ScrCopy( surf, Width, Height, Width, gChrEditSurface + 640 * 309 + 484, 640 );

    LineWidth = 150;
    for( i = 0; i < Height; i++, surf += Width ){
        for( j = 0; j < Width; j++ ){
            if( (GrMaxRGB( surf[ j ] ) < 2 ) && (LineWidth >=  j) ) LineWidth = j;
        }
    }
    LineWidth -= 8;
    if( LineWidth < 0 ) LineWidth = 0;

    FontSet( 102 );
    gFont.Print( gChrEditSurface + 640 * 272 + 348, Title, 640, 640, CHREDT_COL_LVL_I );

    h = gFont.ChrHeight();
    if( Param ){
	i = gFont.LineWidth( Title ) + 356;
        FontSet( 101 );
        gFont.Print( gChrEditSurface + 640 * (h - gFont.ChrHeight() + 268) + i, Param, 640, 640, CHREDT_COL_LVL_I );
    }
    WinDrawLine( gChrEditWin, 348, h + 272, 613, h + 272, CHREDT_COL_LVL_I );
    WinDrawLine( gChrEditWin, 348, h + 273, 613, h + 273, CHREDT_COL_LVL_I );

    // description
    FontSet( 101 );
    if( WinTextWrap( Dsc, LineWidth + 136, LinOffs, &Lines ) ) return -1;
    y = 315;
    h = gFont.ChrHeight() + 1;
    for( i = 0; i < Lines - 1; i++, y += h ){
        s = Dsc + LinOffs[ i + 1 ]; // next line
        tmp = *s;
        *s = '\0';
        gFont.Print( gChrEditSurface + 640 * y + 348, Dsc + LinOffs[ i ], 640, 640, CHREDT_COL_LVL_I );
        *s = tmp;
    }
    if( (ImgId != gChrEditOptTraits[0] || strcmp( gChrEditUnk20, Title ) ) && gChrEditUnk17 ) GSoundPlay( "isdxxxx1" );
    strcpy( gChrEditUnk20, Title );
    gChrEditOptTraits[0] = ImgId;
    gChrEditUnk17 = 1;
    ArtClose( Img );
    return 0;
}

int CharEditFindPerk( int PerkId )
{
    int i;

    for( i = 0; i < 119; i++ ){
        if( PerkLvl(gObjDude, i) && i == PerkId ) break;
    }
    if( i < 119 ) return i;
    eprintf( "\n ** Perk not found in translate! **\n" );
    return -1;
}

void CharEditFolderSelect()
{
    MseGetCursorPosition( &gCharEditMseX, &gCharEditMseY );
    GSoundPlay( "ib3p1xx1" );
    if( gCharEditMseX > 110 ){
        if( gCharEditMseX >= 208 ){
            gChrEditSelected = 41;
            gChrEditFolderType = 2;
            CharEditShowFolder();
            CharEditDscUpdate();
            return;
        }
        gChrEditSelected = 42;
        gChrEditFolderType = 1;
    } else {
        gChrEditSelected = 40;
        gChrEditFolderType = 0;
    }
    CharEditShowFolder();
    CharEditDscUpdate();
}

void CharEditAreaSelect( int sel )
{
    int v3; int v4; int v5; int v6; int v7; int v8;int v10;int v12;int i;
    float v11; float v13;
    double v15;

    MseGetCursorPosition( &gCharEditMseX, &gCharEditMseY );

    switch( sel ){
        case 525: // Stats
            for( i = 0; i < 7; i++ ){
                v15 = 5.0 - i * 0.25;
                if( (gChrEditStatPosY[ i ] - v15 <= gCharEditMseY) && ( ( gChrEditStatPosY[i] + 22 ) + v15 >= gCharEditMseY ) ) break;
            }
            if( i < 7 ) gChrEditSelected = i;
            break;
        case 526: // level
            if( gChrEditMenuMode == 1 ){
                gChrEditSelected = 7;
            } else {
                v3 = gCharEditMseY - 280;
                if( v3 < 0 ) v3 = 0;
                gChrEditSelected = v3 / 10 + 7;
            }
            break;
        case 527: // folder line
            if( !gChrEditMenuMode ){
                FontSet(101);
                v4 = gFont.ChrHeight();
                v5 = gCharEditMseY - 364;
                if( v5 < 0 ) v5 = 0;
                gChrEditSelected = v5 / (v4 + 1) + 10;
            }
            break;
        case 528: // damages
            v6 = gCharEditMseY - 41;
            if( v6 < 0 ) v6 = 0;
            gChrEditSelected = v6 / 13 + 43;
            break;
        case 529: // abilities AC,HP, ....
            v7 = gCharEditMseY - 175;
            if( v7 < 0 ) v7 = 0;
            gChrEditSelected = v7 / 13 + 51;
            break;
        case 530:
            gChrEditSelected = 80;
            break;
        case 531: // skills
            v12 = gCharEditMseY - 27;
            if( v12 < 0 ) v12 = 0;
            gChrEditSkillSelect = lround( v12 * 0.092307694 );
            if( gChrEditSkillSelect >= 18 ) gChrEditSkillSelect = 17;
            gChrEditSelected = gChrEditSkillSelect + 61;
            break;
        case 532:
            gChrEditSelected = 79;
            break;
        case 533:
            gChrEditSelected = 81;
            break;
        case 534:
            FontSet(101);
            v10 = gFont.ChrHeight() + 3;
            v8 = 0;
            v13 = 353.0;
            do{
                if( ( (v13 + -4.0) <= gCharEditMseY ) && ( gCharEditMseY <= (gFont.ChrHeight() + v13) ) ) break;
                v8++;
                v11 = v10 + 0.56;
                v13 = v13 + v11;
            } while ( v8 < 8 );
            if( v8 >= 0 ){
                if( v8 > 7 ) v8 = 7;
            } else {
                v8 = 0;
            }
            gChrEditSelected = v8 + 82;
            if( gCharEditMseX >= 169 ) gChrEditSelected += 8;
            break;
        default:
            break;
    }
    CharEditShowStats(7, 0, 0);
    CharEditShowTraits();
    CharEditShowSkills(0);
    CharEditShowLevelInfo();
    CharEditShowFolder();
    CharEditShowParams();
    CharEditDscUpdate();
}

void CharEditSkillUpDn( unsigned int sel )
{
    int Ex, k, flg, pts, cnt;
    char stmp3[64], stmp1[64], stmp2[64], *lst[2];    

    if( gChrEditMenuMode ) return;
        flg = 0;
        pts = FeatGetPoints( 0 );
        cnt = 0;
        lst[0] = stmp2;
        Ex = 0;
        lst[1] = stmp1;
        gChrEditUnk65 = 4;
        if( sel == 333 || sel == '+' || sel == 'N' ){ flg = 1; sel = 521; } else 
    	if( sel == 331 || sel == '-' || sel == 'J' ){ flg = 1; sel = 523; }
        while( 1 ){
            gChrEditTime = TimerGetSysTime();
            if( cnt <= 19.2 ) cnt++;
    	    if( cnt == 1 || cnt > 19.2 ){
        	if( cnt > 19.2 && ++gChrEditUnk65 > 24 ) gChrEditUnk65 = 24;
        	Ex = 1;
        	if( sel >= 521 ){
            	    if( sel > 521 ){
                	if( sel == 523 ){
                	    if( SkillGetTotal( gObjDude, gChrEditSkillSelect ) <= gChrEditSkillList[gChrEditSkillSelect + 1] || SkillLvlDn(gObjDude, gChrEditSkillSelect) == -2 ) Ex = 0;
                	    if( !Ex ){
                		GSoundPlay( "iisxxxx1" );
                		sprintf( stmp3, "%s:", SkillGetName( gChrEditSkillSelect ) );
                		strcpy( stmp2, MessageGetMessage( &gChrEditMsg, &gChrEditList, 134 ) );
                		strcpy( stmp1, MessageGetMessage( &gChrEditMsg, &gChrEditList, 135 ) );
                		DlgBox( stmp3, lst, 2, 192, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 1 );
                		Ex = -1;
                	    }
                	}
            	    } else {
            		if( FeatGetPoints(0) < 1 ){
                	    GSoundPlay( "iisxxxx1" );
                	    strcpy( stmp3, MessageGetMessage( &gChrEditMsg, &gChrEditList, 136 ) );
                	    DlgBox( stmp3, NULL, 0, 192, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 1 );
                	    Ex = -1;
            		} else {            		    
            		    if( SkillLvlUp(gObjDude, gChrEditSkillSelect) == -3 ){
                		GSoundPlay( "iisxxxx1" );
                		sprintf( stmp3, "%s:", SkillGetName( gChrEditSkillSelect ) );
                		strcpy( stmp2, MessageGetMessage( &gChrEditMsg, &gChrEditList, 132 ) );
                		strcpy( stmp1, MessageGetMessage( &gChrEditMsg, &gChrEditList, 133 ) );
                		DlgBox( stmp3, lst, 2, 192, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 1 );
                		Ex = -1;
            		    }
        		}
        	    }
        	}
        	gChrEditSelected = gChrEditSkillSelect + 61;
        	CharEditDscUpdate();
        	CharEditShowSkills(1);
        	if( Ex == 1 ){
        	    CharEditCounterDisplay( 522, 228, 1, FeatGetPoints( 0 ), pts, gChrEditWin );
        	} else {
        	    CharEditCounterDisplay( 522, 228, 0, FeatGetPoints( 0 ), pts, gChrEditWin );
        	}
        	WinUpdate( gChrEditWin );
	    }
            if( !flg ){
                pts = FeatGetPoints( 0 );
                if( cnt >= 19.2 ){
                    while( TimerCurrDiff( gChrEditTime ) < 1000u / gChrEditUnk65 );
                } else {
                    while( TimerCurrDiff( gChrEditTime ) < 41 );
                }
                k = InpUpdate();
                if( k != 524 && k != 522 && Ex != -1 ) continue;
            }
            return;
        }
    
}

int CharEditTagSkillsAvail()
{
    int i;
    
    for( i = 0; i < 3; i++ ){
        if( gChrEditTagSkill[ 3 - i ] != -1 ) break;
    }
    if( gChrEditMenuMode == 1 ) i--;
    return i;
}

void CharEditTagSkillChoose( int Sel )
{
    char stmp1[128], stmp2[128], *pstr= stmp2;
    MsgLine_t MsgLine;
    int i;

    for( i = 0; i < 4; i++ ){
        if( gChrEditTagSkill[ 3 - i ] != -1 ) break;
    }
    if( gChrEditMenuMode == 1 ) i--;
    gChrTagSkillUnspent = i;

    if( Sel == gChrEditTagSkill[0] || Sel == gChrEditTagSkill[1] || Sel == gChrEditTagSkill[2] || Sel == gChrEditTagSkill[3] ){ // uncheck tag
        if( Sel == gChrEditTagSkill[0] ){
            gChrEditTagSkill[ 0 ] = gChrEditTagSkill[ 1 ];
            gChrEditTagSkill[ 1 ] = gChrEditTagSkill[ 2 ];
        } else if( Sel == gChrEditTagSkill[ 1 ] ){
    	    gChrEditTagSkill[ 1 ] = gChrEditTagSkill[ 2 ];
        }
        gChrEditTagSkill[ 2 ] = -1;
    } else if( gChrEditTaggedSkillsCnt <= 0 ){ // max tags reached 
        GSoundPlay( "iisxxxx1" );
        strcpy( stmp1, MessageGetMessage( &gChrEditMsg, &MsgLine, 140 ) );
        strcpy( stmp2, MessageGetMessage( &gChrEditMsg, &gChrEditList, 141 ) );
	DlgBox( stmp1, &pstr, 1, 192, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 0 );
    } else { // set tag
        for( i = 0; gChrEditTagSkill[ i ] != -1 && i < 3; i++ );
        gChrEditTagSkill[ i ] = Sel;
    }

    for( i = 0; i < 4; i++ ){
        if( gChrEditTagSkill[ 3 - i ] != -1 ) break;
    }
    if( gChrEditMenuMode == 1 ) i--;
    gChrEditTaggedSkillsCnt = i;

    gChrEditSelected = 61 + Sel;
    CharEditShowStats( 7, 0, 0 );
    CharEditShowParams( );
    CharEditShowSkills( 2 );
    CharEditDscUpdate();
    WinUpdate( gChrEditWin );
}

void CharEditShowTraits()
{
    unsigned int id;
    int i, col;
    float ff, ypos;

    id = -1;
    if( gChrEditMenuMode != 1 ) return;    
    if( gChrEditSelected >= 82 && gChrEditSelected < 98 ) id = gChrEditSelected - 82;
    ScrCopy( gChrEditWpPix + 225967, 245, 100, 640, gChrEditSurface + 225967, 640 );
    FontSet( 101 );
    ypos = 353.0;
    ff = (double)(gFont.ChrHeight() + 3) + 0.56;
    TraitSetSpec( gChrEditOptionTraits[0], gChrEditOptionTraits[1] );
        
    for( i = 0; i < 8; i++, ypos += ff ){
        if( i == id ){
	    col = CHREDT_COL_LVL_A;
            if( i == gChrEditOptionTraits[0] || i == gChrEditOptionTraits[1]) col = CHREDT_COL_LVL_G;
            gChrEditDscImgId = TraitGetId(i);
            gChrEditDscTitle = TraitGetName(i);
            gChrEditDscParams = 0;
            gChrEditDscDsc = TraitGetDsc(i);            
        } else if( i == gChrEditOptionTraits[0] || i == gChrEditOptionTraits[1] ){
            col = CHREDT_COL_LVL_H;
        } else {
            col = CHREDT_COL_LVL_B;
        }        
        gFont.Print( &gChrEditSurface[640 * lround(ypos) + 47], TraitGetName( i ), 640, 640, col );        
    }
    ypos = 353.0;
    for( i = 8; i < 16; i++, ypos += ff ){
        if( i == id ){
	    col = CHREDT_COL_LVL_A;
            if( i == gChrEditOptionTraits[0] || i == gChrEditOptionTraits[1]) col = CHREDT_COL_LVL_G;
            gChrEditDscImgId = TraitGetId(i);
            gChrEditDscTitle = TraitGetName(i);
            gChrEditDscParams = 0;
            gChrEditDscDsc = TraitGetDsc(i);
        } else if( i == gChrEditOptionTraits[0] || i == gChrEditOptionTraits[1] ){
            col = CHREDT_COL_LVL_H;
        } else {
            col = CHREDT_COL_LVL_B;
        }
        gFont.Print( &gChrEditSurface[640 * lround( ypos ) + 199], TraitGetName( i ), 640, 640, col );        
    }
}

int CharEditOptTraitsAvail()
{
    int i;

    for( i = 0; i < 2; i++ ){
        if( gChrEditOptionTraits[ 1 - i ] != -1 ) break;
    }
    return i;
}

void CharEditSpecTraitChoose( int Sel )
{
    char stmp1[128], stmp2[128], *pstr = stmp2;
    MsgLine_t MsgLine;
    int i;

    if( Sel == gChrEditOptionTraits[ 0 ] || Sel == gChrEditOptionTraits[ 1 ] ){ // uncheck tag
        if( Sel == gChrEditOptionTraits[ 0 ] ) gChrEditOptionTraits[ 0 ] = gChrEditOptionTraits[ 1 ];
        gChrEditOptionTraits[ 1 ] = -1;
    } else if( gChrEditOptionalTraitsCnt <= 0 ){ // tag max
        GSoundPlay( "iisxxxx1" );
        strcpy( stmp1, MessageGetMessage( &gChrEditMsg, &MsgLine, 148 ) );
        strcpy( stmp2, MessageGetMessage( &gChrEditMsg, &gChrEditList, 149 ) );
        DlgBox( stmp1, &pstr, 1, 192, 126, gPalColorCubeRGB[31][18][8], NULL, gPalColorCubeRGB[31][18][8], 0 );
    } else {
        for( i = 0; i < 2 && gChrEditOptionTraits[ i ] != -1; i++ );
        gChrEditOptionTraits[ i ] = Sel;
    }
    for( i = 0; i < 2; i++ ){
        if( gChrEditOptionTraits[ 1 - i ] != -1 ) break;
    }
    gChrEditOptionalTraitsCnt = i;
    gChrEditSelected = 82 + Sel;
    CharEditShowTraits();
    CharEditShowSkills( 0 );
    FeatStatsRecalculate( gObjDude );
    CharEditCounterDisplay( 126, 282, 0, gChrEditUnspentChrPts, 0, gChrEditWin );
    CharEditShowStats( 7, 0, 0 );
    CharEditShowParams();
    CharEditDscUpdate();
    WinUpdate( gChrEditWin );
}

char *CharEditShowKarma()
{
    int Selected, Lvl, ImgId, NameId, i, First; 
    char stmp[256], CityName[40], sNum[32];

    CharEditFoldArrowsHandle();
    // Karma
    Selected = 0;
    for( i = 0; i < gChrEditMsgKarmaCount; i++ ){
        if( (Lvl = gCharEditKarma[ i ].Val) ){
            if( gGVals[ Lvl ] ){
                if( CharEditFoldLine( MessageGetMessage( &gChrEditMsg, &gChrEditList, gCharEditKarma[ i ].TitleId ) ) ){
                    gChrEditDscImgId = gCharEditKarma[ i ].ImgId;
                    gChrEditDscTitle = MessageGetMessage( &gChrEditMsg, &gChrEditList, gCharEditKarma[ i ].TitleId );
                    gChrEditDscParams = 0;
                    gChrEditDscDsc = MessageGetMessage( &gChrEditMsg, &gChrEditList, gCharEditKarma[ i ].DscId );
                    Selected = 1;
                }
            }
	} else if( gCharEditGenreCount > 0 ){
	    for( i = 0; i < gCharEditGenreCount; i++ ){
            	if( *gGVals >= gCharEditGenre[ i ].i01 ){
            	    sprintf( sNum, "%d", *gGVals );
            	    sprintf( stmp, "%s: %s (%s)", 
            		    MessageGetMessage( &gChrEditMsg, &gChrEditList, 125 ), sNum, 
            		    MessageGetMessage( &gChrEditMsg, &gChrEditList, gCharEditGenre[ i ].i02 )
            	    );
            	    if( CharEditFoldLine( stmp ) ){
                	gChrEditDscImgId = gCharEditKarma[ i ].ImgId;
                	gChrEditDscTitle = MessageGetMessage( &gChrEditMsg, &gChrEditList, 125 );
                	gChrEditDscParams = 0;
            		Selected = 1;
                	gChrEditDscDsc = MessageGetMessage( &gChrEditMsg, &gChrEditList, gCharEditKarma[ i ].DscId );
            	    }
            	    break;
        	}
            }
        }
    } 

    // Reputation    
    First = 0;
    for( i = 0; i < 19; i++ ){
        if( WmIsAreaKnown( gChrEditKarma1[ i + 1 ] ) == 1 ){
            if( !First ){
                if( CharEditPerksFoldTitle( MessageGetMessage( &gChrEditMsg, &gChrEditList, 4000 ) ) ){
                    gChrEditDscImgId = 48;
                    gChrEditDscTitle = MessageGetMessage( &gChrEditMsg, &gChrEditList, 4000 );
                    gChrEditDscParams = 0;
                    Selected = 1;
                    gChrEditDscDsc = MessageGetMessage( &gChrEditMsg, &gChrEditList, 4100 );
                }
                First = 1;
            }
            WmGetLocationName( gChrEditKarma1[ i + 1 ], CityName );
            Lvl = gGVals[ gChrEditKarma1[ i ] ];
            if( Lvl >= 30 ){ ImgId = 135; NameId = 2000; }
            else if( Lvl >= 15 ){ ImgId = 137; NameId = 2001; } 
            else if( Lvl > 0 ){ ImgId = 137; NameId = 2002; }
            else if( Lvl == 0 ){ ImgId = 141; NameId = 2003; }
            else if( Lvl >= -15 ){ ImgId = 153; NameId = 2004; }
            else if( Lvl < -30 ){ ImgId = 150; NameId = 2006; }
            else { ImgId = 153; NameId = 2005; }    	    
            sprintf( stmp, "%s: %s", CityName, MessageGetMessage( &gChrEditMsg, &gChrEditList, NameId ) );
            if( CharEditFoldLine( stmp ) ){
                gChrEditDscImgId = ImgId;
                gChrEditDscTitle = MessageGetMessage( &gChrEditMsg, &gChrEditList, NameId );
                gChrEditDscParams = 0;
                gChrEditDscDsc = MessageGetMessage( &gChrEditMsg, &gChrEditList, NameId + 100 );
                Selected = 1;
            }
        }
    }
    // Reliance
    First = 0;
    for( i = 0; i < 8; i++ ){
        if( gGVals[ gChrEditKarmaIds1[ i ] ] ){
            if( !First ){
                if( CharEditPerksFoldTitle( MessageGetMessage( &gChrEditMsg, &gChrEditList, 4001 ) ) ){
                    gChrEditDscImgId = 53;
                    gChrEditDscTitle = MessageGetMessage( &gChrEditMsg, &gChrEditList, 4001 );
                    gChrEditDscParams = 0;
                    gChrEditDscDsc = MessageGetMessage( &gChrEditMsg, &gChrEditList, 4101 );
                    Selected = 1;
                }
                First = 1;
            }
            if( CharEditFoldLine( MessageGetMessage( &gChrEditMsg, &gChrEditList, 1004 + i ) ) ){
                gChrEditDscImgId = gChrEditKarmaIds2[ i ];
                gChrEditDscTitle = MessageGetMessage( &gChrEditMsg, &gChrEditList, 1004 + i );
                gChrEditDscParams = 0;
                gChrEditDscDsc = MessageGetMessage( &gChrEditMsg, &gChrEditList, 1104 + i );
                Selected = 1;
            }
        }
    }
    if( !Selected ){
        gChrEditDscImgId = 47;
        gChrEditDscTitle = MessageGetMessage( &gChrEditMsg, &gChrEditList, 125 );
        gChrEditDscParams = 0;
        gChrEditDscDsc = MessageGetMessage( &gChrEditMsg, &gChrEditList, 128 );
    }

    return 0;
}

int CharEditSaveLvl( xFile_t *fh )
{
    if( dbputBei( fh, gChrEditCurLvl ) != -1 ){
        if( dbputb( fh, gChrEditNewPerkFlag ) != -1 ) return 0;
    }
    return -1;
}

int CharEditLoadLvl( xFile_t *fh )
{

    if( dbgetBei( fh, &gChrEditCurLvl ) != -1 ){
        if( dbgetb( fh, &gChrEditNewPerkFlag ) != -1 ) return 0;
    }
    return -1;
}

void CharEditStartChar()
{
    gChrEditUnspentChrPts = 5;
    gChrEditCurLvl = 1;
}

int CharEditLvlUp()
{
    int points, ex, SkillLvl, i, perks, j;

    points = FeatGetPoints( FEAT_PT_LV );
    if( (points != gChrEditCurLvl) && (points <= FEAT_PTS_LEVELCAP) ){
	for( i = gChrEditCurLvl + 1; i <= points; i++ ){
    	    SkillLvl = 5 * TraitSpecActive( TRAIT_SKILLED ) + 2 * PerkLvl( gObjDude, PERK_EDUCATED ) + 2 * FeatGetTotal( gObjDude, FEAT_INTELLIGENCE ) + FeatGetPoints( FEAT_PT_SP ) + 5;
    	    if( TraitSpecActive( TRAIT_GIFTED ) ){
        	SkillLvl -= 5;
    	        if( SkillLvl < 0 ) SkillLvl = 0;
    	    }        
    	    if( SkillLvl > 99 ) SkillLvl = 99;
    	    FeatSetPoints( 0, SkillLvl );
    	    for( j = 0, perks = 0; j < 119; j++ ){
        	if( PerkLvl(gObjDude, j) <= 0) continue;
        	if( ++perks >= 37 ) break;
    	    }
    	    if( (perks < 37) && !(i % (( TraitSpecActive( TRAIT_SKILLED ) != 0) + 3)) ) gChrEditNewPerkFlag = 1;
	}
    }

    if( gChrEditNewPerkFlag ){
        gChrEditFolderType = 0;
        CharEditShowFolder();
        WinUpdate( gChrEditWin );
        if( (ex = CharEditPerkDialog() ) < 0 ){
            if( ex == -1 ){ eprintf( "\n *** Error running perks dialog! ***\n" ); return -1; }
        } else if( ex <= 0 ){
            CharEditShowFolder();
        } else if( ex == 1 ){
            CharEditShowFolder();
            gChrEditNewPerkFlag = 0;
        }
    }    
    gChrEditCurLvl = points;
    return 1;
}

void CharEditPerkListCb()
{
    int lvl, id;
    char *p, stmp[40];

    ScrCopy( gChrEditPerkImg + 280, 293, 230, 573, gChrEditPerkDlgSurf + 280, 573 );
    CharEditPerkDlgList();
    id = gChrEditPerksAvail[ gChrEditPerkDisplOffset + gChrEditUnk46 ].Id;
    if( (lvl = PerkLvl( gObjDude, id ) ) ){
        sprintf( stmp, "(%d)", lvl );
        p = stmp;
    } else {
        p = NULL;
    }
    CharEditDscPanel( PerkGetImgId( id ), PerkGetName( id ), p, PerkGetDsc( id ) );
    WinUpdate( gChrEditPerkWin );
}

int CharEditPerkDialog()
{
    CachePool_t *Img;
    int bt, v15, lvl, tmp[2], idx, err;
    char *Params, stmp[32];

    gChrEditPerkDisplOffset = 0;
    gChrEditUnk46 = 0;
    gChrEditUnk14 = -1;
    gChrEditUnk19[0] = 0;
    gChrEditSavePerks[0] = 0;
    gChrEditPerkImg = ArtLoadBmp( ArtMakeId( 6, 86, 0, 0, 0 ), &Img, &tmp[0], &tmp[1] );
    if( !gChrEditPerkImg ){ 
	eprintf( "\n *** Error running perks dialog window ***\n" ); return -1; 
    }
    if( (gChrEditPerkWin = WinCreateWindow( 33, 91, 573, 230, 256, 18 ) ) == -1 ){
	eprintf( "\n *** Error running perks dialog window ***\n" ); ArtClose( Img ); return -1;
    }
    gChrEditPerkDlgSurf = WinGetSurface( gChrEditPerkWin ); 
    memcpy( gChrEditPerkDlgSurf, gChrEditPerkImg, tmp[0] * tmp[1] ); 
    bt = WinCreateButton( gChrEditPerkWin, 48, 186, gChrEditImgGeo[23].Width, gChrEditImgGeo[23].Height, -1, -1, -1, 500, gChrEditPix[23], gChrEditPix[24], 0, 32 );
    if( bt != -1 ) WinSetClickSound(bt, GSoundPlayPushBt, GSoundPlayReleaseBt); 
    bt = WinCreateButton( gChrEditPerkWin, 153, 186, gChrEditImgGeo[23].Width, gChrEditImgGeo[23].Height, -1, -1, -1, 502, gChrEditPix[23], gChrEditPix[24], 0, 32 );
    if( bt != -1 ) WinSetClickSound(bt, GSoundPlayPushBt, GSoundPlayReleaseBt); 
    bt = WinCreateButton( gChrEditPerkWin, 25, 46, gChrEditImgGeo[22].Width, gChrEditImgGeo[22].Height, -1, 574, 572, 574, gChrEditPix[21], gChrEditPix[22], 0, 32 );
    if( bt != -1 ) WinSetClickSound(bt, GSoundPlayPushBt, 0); 
    bt = WinCreateButton( gChrEditPerkWin, 25, gChrEditImgGeo[22].Height + 47, gChrEditImgGeo[22].Width, gChrEditImgGeo[22].Height, -1, 575, 573, 575, gChrEditPix[3], gChrEditPix[4], 0, 32 );
    if( bt != -1 ) WinSetClickSound(bt, GSoundPlayPushBt, 0);
    WinCreateButton( gChrEditPerkWin, 45, 43, 192, 129, -1, -1, -1, 501, 0, 0, 0, 32 ); 
    FontSet( 103 );
    gFont.Print( gChrEditPerkDlgSurf + 9217,   MessageGetMessage( &gChrEditMsg, &gChrEditList, 152 ), tmp[0], tmp[0], CHREDT_COL_SELECT ); 
    gFont.Print( gChrEditPerkDlgSurf + 106647, MessageGetMessage( &gChrEditMsg, &gChrEditList, 100 ), tmp[0], tmp[0], CHREDT_COL_SELECT );
    gFont.Print( gChrEditPerkDlgSurf + 106749, MessageGetMessage( &gChrEditMsg, &gChrEditList, 102 ), tmp[0], tmp[0], CHREDT_COL_SELECT ); 
    v15 = CharEditPerkDlgList(); 
    idx = gChrEditPerksAvail[ gChrEditPerkDisplOffset + gChrEditUnk46 ].Id;
    if( (lvl = PerkLvl( gObjDude, idx )) ){
        sprintf( stmp, "(%d)", lvl );
        Params = stmp;
    } else {
        Params = NULL;
    }
    CharEditDscPanel( PerkGetImgId( idx ), PerkGetName( idx ), Params, PerkGetDsc( idx ) ); 
    WinUpdate( gChrEditPerkWin ); 

    err = CharEditListMenu( v15, CharEditPerkListCb );
    if( err == 1 && PerkLvlInc( gObjDude, idx) == -1 ){ eprintf( "\n*** Unable to add perk! ***\n" ); err = 2; }
    if( err & 0x01 ){
        if( !PerkLvl( gObjDude, 51 ) || gChrEditSavePerks[52] ){
    	    if( PerkLvl(gObjDude, 52 ) && !gChrEditSavePerks[53] ){
        	if( !CharEditLoseTrait() ){ 
        	    err = gChrEditSavePerks[53]; 
        	    PerkLvlDec( gObjDude, 52 ); 
        	}
            } else if( PerkLvl( gObjDude, 28 ) == gChrEditSavePerks[29] ){
                if( PerkLvl( gObjDude, 18 ) != gChrEditSavePerks[19] ) FeatSetPoints( 0, FeatGetPoints( 0 ) + 2 );             	    
            } else { 
            	FeatSetBoost( gObjDude, 7, FeatGetBoost( gObjDude, 7 ) + 4 ); 
            	CritterHeal( gObjDude, 4 ); 
            }    	    
        } else if( !CharEditPickPerk() ){ 
    	    err = gChrEditSavePerks[52]; 
    	    PerkLvlDec( gObjDude, 51 ); 
    	}
    }
    CharEditShowSkills( 0 ); 
    CharEditShowStats( 7, 0, 0 ); 
    CharEditShowLevelInfo(); 
    CharEditShowParams(); 
    CharEditShowFolder(); 
    CharEditDscUpdate(); 
    WinUpdate( gChrEditWin );
    ArtClose( Img ); 
    WinClose( gChrEditPerkWin );
    return err;        
}

int CharEditListMenu( int Avail, void (*Cb)() )
{
    int v3, sel, v12, i, v14, ex, v20;

    FontSet( 101 );
    v3 = Avail - 11;
    ex = 0;
    gChrEditUnk47 = -2;
    do{
        sel = InpUpdate();
        v20 = 0;
        if( sel == 502 || sel == 27 || gMenuEscape ){ ex = 2; break; }
        switch( sel ){
            case 500: ex = 1; break;
            case KEY_ENTER: GSoundPlay( "ib1p1xx1" ); ex = 1; break;
            case 501:
                MseGetCursorPosition( &gCharEditMseX, &gCharEditMseY );
                gChrEditUnk46 = (gCharEditMseY - 134) / (gFont.ChrHeight() + 2);
                if( gChrEditUnk46 >= 0 ){
                    if( gChrEditUnk46 > Avail ) gChrEditUnk46 = Avail;
                } else {
                    gChrEditUnk46 = 0;
                }
                if( gChrEditUnk46 == gChrEditUnk47 ){
                    GSoundPlay("ib1p1xx1");
                    ex = 1;
                }
                gChrEditUnk47 = gChrEditUnk46;
                Cb();
                break;
            case 328:
                gChrEditUnk47 = -2;
                if( --gChrEditPerkDisplOffset < 0 ){
                    gChrEditPerkDisplOffset = 0;
                    if( --gChrEditUnk46 < 0 ) gChrEditUnk46 = 0;
                }
                Cb();
                continue;
            case 329:
                gChrEditUnk47 = -2;
                for( i = 0; i < 11; i++ ){
                    if( --gChrEditPerkDisplOffset < 0 ){
                        gChrEditPerkDisplOffset = 0;
                        if( --gChrEditUnk46 < 0 ) gChrEditUnk46 = 0;
                    }
                }
                Cb();
                continue;
            case 336:
                v12 = gChrEditUnk46 + 1;
                gChrEditUnk47 = -2;
                if( Avail <= 11 ){
                    gChrEditUnk46++;
                    if( v12 > Avail ) gChrEditUnk46 = Avail;
                } else if( v3 < ++gChrEditPerkDisplOffset ){
                    gChrEditUnk46++;
                    gChrEditPerkDisplOffset = Avail - 11;
                    if( v12 > 10 ) gChrEditUnk46 = 10;
                }
                Cb();
                continue;
            case 337:
                gChrEditUnk47 = -2;
                for( i = 0; i < 11; i++ ){
                    v14 = gChrEditUnk46 + 1;
                    if( Avail <= 11 ){
                        gChrEditUnk46++;
                        if( v14 > Avail ) gChrEditUnk46 = Avail;
                    } else if( v3 < ++gChrEditPerkDisplOffset ){
                        gChrEditUnk46++;
                        gChrEditPerkDisplOffset = Avail - 11;
                        if( v14 > 10 ) gChrEditUnk46 = 10;
                    }
                }
                Cb();
                continue;
            case 572:
                gChrEditUnk65 = 4;
                gChrEditUnk47 = -2;
                do{
                    gChrEditTime = TimerGetSysTime();
                    if( v20 <= 14.4 ) v20++;
                    if( v20 == 1 || v20 > 14.4 ){
                        if( v20 > 14.4 && (++gChrEditUnk65) > 24 ) gChrEditUnk65 = 24;
                        if( --gChrEditPerkDisplOffset < 0 ){
                            gChrEditPerkDisplOffset = 0;
                            if( --gChrEditUnk46 < 0 ) gChrEditUnk46 = 0;
                        }
                        Cb();
                    }
                    if( v20 >= 14.4 ){
                        while ( TimerCurrDiff(gChrEditTime) < 1000 / gChrEditUnk65 );
                    } else {
                        while( TimerCurrDiff(gChrEditTime) < 41 );
                    }
                }while( InpUpdate() != 574 );
                break;
            case 573:
                gChrEditUnk47 = -2;
                gChrEditUnk65 = 4;
                if( Avail <= 11 ){
                    do{
                        gChrEditTime = TimerGetSysTime();
                        if( v20 <= 14.4 ) v20++;
                        if( v20 == 1 || v20 > 14.4 ){
                            if( v20 > 14.4 && (++gChrEditUnk65) > 24 ) gChrEditUnk65 = 24;
                            if( Avail < ++gChrEditUnk46 ) gChrEditUnk46 = Avail;
                            Cb();
                        }
                        if( v20 >= 14.4 ){
                            while( TimerCurrDiff( gChrEditTime ) < 1000 / gChrEditUnk65 );
                        } else {
                            while( TimerCurrDiff( gChrEditTime ) < 41 );
                        }
                    }while( InpUpdate() != 575 );
                } else {
                    do {
                        gChrEditTime = TimerGetSysTime();
                        if( v20 <= 14.4 ) v20++;
                        if( v20 == 1 || v20 > 14.4 ){
                            if( v20 > 14.4 && (++gChrEditUnk65) > 24 ) gChrEditUnk65 = 24;
                            if( v3 < ++gChrEditPerkDisplOffset ){
                                gChrEditPerkDisplOffset = Avail - 11;
                                if( ++gChrEditUnk46 > 10 ) gChrEditUnk46 = 10;
                            }
                            Cb();
                        }
                        if( v20 >= 14.4 ){
                            while( TimerCurrDiff( gChrEditTime ) < 1000 / gChrEditUnk65 );
                        } else {
                            while( TimerCurrDiff( gChrEditTime ) < 41 );
                        }
                    }while( InpUpdate() != 575 );
                }
                break;
            case 327:
                gChrEditPerkDisplOffset = 0;
                gChrEditUnk46 = 0;
                gChrEditUnk47 = -2;
                Cb();
                break;
            case 335:
                gChrEditUnk47 = -2;
                if( Avail <= 11 ){
            	    gChrEditUnk46 = Avail;                        
                } else {
            	    gChrEditPerkDisplOffset = Avail - 11;
            	    gChrEditUnk46 = 10;
                }
                Cb();
                break;
            default:
                if( TimerCurrDiff( gChrEditTime ) > 700 ){
                    gChrEditTime = TimerGetSysTime();
                    gChrEditUnk47 = -2;
                }
                break;
        }
    }while( !ex );
    return ex;
}

int CharEditPerkDlgList()
{
    int i, lvl, col, Avail, Marked, h, DisplLines, y, Perks[119]; 
    char stmp[256]; 

    ScrCopy( gChrEditPerkImg + 24684, 192, 129, 573, gChrEditPerkDlgSurf + 24684, 573 ); // clear
    FontSet( 101 );

    h = gFont.ChrHeight() + 2;
    Avail = PerkGetAvail( gObjDude, Perks );
    if( !Avail ) return 0;    
    for( i = 0; i < 119; i++ ){
        gChrEditPerksAvail[ i ].Name = NULL;
        gChrEditPerksAvail[ i ].Id = 0;
    }
    for( i = 0; i < Avail; i++ ){
        gChrEditPerksAvail[ i ].Name = PerkGetName( Perks[ i ] );
        gChrEditPerksAvail[ i ].Id = Perks[ i ];
    }
    qsort( gChrEditPerksAvail, Avail, sizeof( PerksList_t ), (void *)CharEditPerkSortCb );
    DisplLines = Avail - gChrEditPerkDisplOffset;
    if( DisplLines > 11 ) DisplLines = 11;
    DisplLines += gChrEditPerkDisplOffset;
    Marked = gChrEditPerkDisplOffset + gChrEditUnk46;    
    y = 43;
    for( i = gChrEditPerkDisplOffset; i < DisplLines; i++, y += h ){
	col = ( i == Marked ) ? CHREDT_COL_LVL_A : CHREDT_COL_LVL_B;
        gFont.Print( gChrEditPerkDlgSurf + 573 * y + 45, gChrEditPerksAvail[ i ].Name, 573, 573, col );
        if( ( lvl = PerkLvl( gObjDude, gChrEditPerksAvail[ i ].Id ) ) ){
            sprintf( stmp, "(%d)", lvl & 0xff );
            gFont.Print( gChrEditPerkDlgSurf + 573 * y + 207, stmp, 573, 573, col );
        }        
    }
    return Avail;
}

void CharEditLoseTraitCb()
{
    ScrCopy( gChrEditPerkImg + 280, 293, 230, 573, gChrEditPerkDlgSurf + 280, 573 );
    CharEditLoseTraitUpdate( gChrEditUnk79 );
    CharEditDscPanel( TraitGetId( gChrEditPerksAvail[ 0 ].Id ), gChrEditPerksAvail[ 0 ].Name, NULL, TraitGetDsc( gChrEditPerksAvail[ 0 ].Id ) );
    WinUpdate( gChrEditPerkWin );
}

int CharEditLoseTrait()
{
    int sel, i, n;

    sel = 1;
    gChrEditUnk14 = -1;
    gChrEditUnk19[0] = 0;
    gChrEditSavePerks[0] = 0;
    for( i = 0; i < 2; i++ ){
        if( gChrEditOptionTraits[ 1 - i] != -1 ) break;
    }
    gChrEditOptionalTraitsCnt = 2 - i;
    if( gChrEditOptionalTraitsCnt >= 1 ){
        FontSet( 103 );
        ScrCopy( gChrEditPerkImg + 8071, 206, gFont.ChrHeight() + 2, 573, gChrEditPerkDlgSurf + 8644, 573 );
        gFont.Print( gChrEditPerkDlgSurf + 9217, MessageGetMessage( &gChrEditMsg, &gChrEditList, 154 ), 573, 573, gPalColorCubeRGB[18][17][3] );
        gChrEditUnk79 = gChrEditUnk46 = 0;
        gChrEditPerkDisplOffset = 0;
        CharEditLoseTraitCb();
        sel = CharEditListMenu( gChrEditOptionalTraitsCnt, CharEditLoseTraitCb );
        if( sel == 1 ){
            if( !gChrEditUnk46 ){
                if( gChrEditOptionalTraitsCnt == 1 ){
                    gChrEditOptionTraits[ 0 ] = -1;
                    gChrEditOptionTraits[ 1 ] = -1;
                } else {
            	    if( gChrEditPerksAvail[ 0 ].Id == gChrEditOptionTraits[ 0 ] ){
                	gChrEditOptionTraits[ 0 ] = gChrEditOptionTraits[ 1 ];
                	gChrEditOptionTraits[ 1 ] = -1;
            	    } else {
            		gChrEditOptionTraits[ 1 ] = -1;
            	    }
                }
            } else {
        	if( gChrEditPerksAvail[ 0 ].Id == gChrEditOptionTraits[ 0 ] ){
            	    gChrEditOptionTraits[ 1 ] = -1;
        	} else {
        	    gChrEditOptionTraits[ 0 ] = gChrEditOptionTraits[ 1 ];
        	    gChrEditOptionTraits[ 1 ] = -1;
        	}
            }
        } else {
            sel = 0;
        }
    }
    if( sel ){
        FontSet( 103 );
        ScrCopy( gChrEditPerkImg + 8071, 206, gFont.ChrHeight() + 2, 573, gChrEditPerkDlgSurf + 8644, 573 );
        gFont.Print( gChrEditPerkDlgSurf + 9217, MessageGetMessage( &gChrEditMsg, &gChrEditList, 153), 573, 573, gPalColorCubeRGB[18][17][3] );
        gChrEditUnk46 = gChrEditPerkDisplOffset = 0;
        gChrEditUnk79 = 1;
        CharEditLoseTraitCb();
        n = 16 - gChrEditOptionalTraitsCnt;
        if( n > 16 ) n = 16;
        sel = CharEditListMenu( n, CharEditLoseTraitCb );
        if( sel == 1 ){
            if( gChrEditOptionalTraitsCnt ){
                gChrEditOptionTraits[ 1 ] = gChrEditPerksAvail[ gChrEditUnk46 + gChrEditPerkDisplOffset ].Id;
            } else {
                gChrEditOptionTraits[ 1 ] = -1;
                gChrEditOptionTraits[ 0 ] = gChrEditPerksAvail[ gChrEditUnk46 + gChrEditPerkDisplOffset ].Id;
            }
            TraitSetSpec( gChrEditOptionTraits[ 0 ], gChrEditOptionTraits[ 1 ] );
        } else {
            sel = 0;
        }
    }
    if( !sel ) memcpy( gChrEditOptionTraits, &gChrEditOptTraits[1], 12 );
    return sel;
}

void CharEditPickPerkCb()
{
    int Idx;

    ScrCopy( gChrEditPerkImg + 280, 293, 230, 573, gChrEditPerkDlgSurf + 280, 573 );
    CharEditPickPerkUpdate();
    Idx = gChrEditPerkDisplOffset  + gChrEditUnk46;
    CharEditDscPanel( SkillGetImgId( gChrEditPerksAvail[ Idx ].Id ), gChrEditPerksAvail[ Idx ].Name, 0, SkillGetDsc( gChrEditPerksAvail[ Idx ].Id ) );
    WinUpdate( gChrEditPerkWin );
}

int CharEditPickPerk()
{
    FontSet( 103 );
    ScrCopy( gChrEditPerkImg + 8071, 206, gFont.ChrHeight() + 2, 573, gChrEditPerkDlgSurf + 8644, 573 );
    gFont.Print( gChrEditPerkDlgSurf + 9217, MessageGetMessage( &gChrEditMsg, &gChrEditList, 155 ), 573, 573, gPalColorCubeRGB[18][17][3] );
    gChrEditUnk46 = 0;
    gChrEditPerkDisplOffset = 0;
    gChrEditUnk14 = -1;
    gChrEditUnk19[ 0 ] = 0;
    gChrEditSavePerks[ 0 ] = 0;
    CharEditPickPerkCb();
    if( CharEditListMenu( gChrEditUnk79, CharEditPickPerkCb ) == 1 ){
        gChrEditTagSkill[ 3 ] = gChrEditPerksAvail[ 0 ].Id;
        SkillSetSpecials( gChrEditTagSkill, 4 );
    } else {
        memcpy( gChrEditTagSkill, gChrEditSkillSpecial, sizeof( gChrEditTagSkill ) );
        SkillSetSpecials( gChrEditSkillSpecial, 4 );
    }
    return 0;
}

void CharEditPickPerkUpdate()
{
    int y, Mark, h, i;

    Mark = gChrEditUnk46 + gChrEditPerkDisplOffset;
    y = 43;
    ScrCopy( gChrEditPerkImg + 24684, 192, 129, 573, gChrEditPerkDlgSurf + 24684, 573 );
    FontSet( 101 );
    gChrEditUnk79 = 0;
    h = gFont.ChrHeight() + 2;
    for( i = 0; i < 18; i++ ){
        if( i != gChrEditTagSkill[ 0 ] && i != gChrEditTagSkill[ 1 ] && i != gChrEditTagSkill[ 2 ] && i != gChrEditTagSkill[ 3 ] ){
            gChrEditPerksAvail[ gChrEditUnk79 ].Id = i;
            gChrEditPerksAvail[ gChrEditUnk79 ].Name = SkillGetName( i );
            gChrEditUnk79++;
        }
    }
    qsort( gChrEditPerksAvail, gChrEditUnk79, sizeof( PerksList_t ), (void *)CharEditPerkSortCb );
    for( i = gChrEditPerkDisplOffset; i < (gChrEditPerkDisplOffset + 11); i++, y += h ){
        gFont.Print( gChrEditPerkDlgSurf + 573 * y + 45, gChrEditPerksAvail[ i ].Name, 573, 573, ( i == Mark ) ? gPalColorCubeRGB[31][31][11] : gPalColorCubeRGB[0][31][0] );
    }
}

int CharEditLoseTraitUpdate( int a1 )
{
    int y, i, k, Mark, h;

    ScrCopy( gChrEditPerkImg + 24684, 192, 129, 573, gChrEditPerkDlgSurf + 24684, 573 );
    FontSet( 101 );
    y = 43;
    h = gFont.ChrHeight() + 2;
    if( a1 ){
        Mark = gChrEditUnk46 + gChrEditPerkDisplOffset;
        for( i = k = 0; i < 16; i++ ){
            if( (i != gChrEditOptTraits[ 1 ]) && (i != gChrEditOptTraits[ 2 ]) ){
                gChrEditPerksAvail[ k ].Id = i;
                gChrEditPerksAvail[ k ].Name = TraitGetName( i );
                k++;
            }
        }
        qsort( gChrEditPerksAvail, k, sizeof( PerksList_t ), (void *)CharEditPerkSortCb );
        for( i = gChrEditPerkDisplOffset; i < gChrEditPerkDisplOffset + 11; i++, y += h ){
            gFont.Print( gChrEditPerkDlgSurf + 573 * y + 45, gChrEditPerksAvail[ i ].Name, 573, 573, 
        		( i == Mark ) ? gPalColorCubeRGB[31][31][11] : gPalColorCubeRGB[0][31][0] );
        }
    } else {
        gChrEditPerksAvail[ 0 ].Id = gChrEditOptionTraits[ 0 ];
        gChrEditPerksAvail[ 0 ].Name = TraitGetName( gChrEditOptionTraits[ 0 ] );
        gChrEditPerksAvail[ 1 ].Id = gChrEditOptionTraits[ 1 ];
        gChrEditPerksAvail[ 1 ].Name = TraitGetName( gChrEditOptionTraits[ 1 ] );
        if( gChrEditOptionalTraitsCnt > 1 ) qsort( gChrEditPerksAvail, gChrEditOptionalTraitsCnt, sizeof( PerksList_t ), (void *)CharEditPerkSortCb );
        
        for( i = 0; i < gChrEditOptionalTraitsCnt; i++, y += h ){
            gFont.Print( gChrEditPerkDlgSurf + 573 * y + 45, gChrEditPerksAvail[ i ].Name, 573, 573, 
        	    ( i == gChrEditUnk46 ) ? gPalColorCubeRGB[31][31][11] : gPalColorCubeRGB[0][31][0] );            
        }        
    }
    return 0;
}

static int CharEditPerkSortCb( PerksList_t *s1, PerksList_t *s2 )
{
    return strcmp( s1->Name, s2->Name );
}

int CharEditDscPanel( int ImgId, char *Name, char *Param, char *Dsc )
{
    CachePool_t *Img;
    char *bmp,*p,*s1,*s2,tmp;
    short Lines, Offsets[ 64 ];
    int MaxLen, i, j, h, Height, Width;

    bmp = ArtLoadBmp( ArtMakeId( 10, ImgId, 0, 0, 0 ), &Img, &Width, &Height );
    if( !bmp ) return -1;
    ScrCopy( bmp, Width, Height, Width, gChrEditPerkDlgSurf + 37085, 573 );
    MaxLen = 150;
    p = bmp;
    for( i = 0; i < Height; i++, p += Width ){
        for( j = 0; j < Width; j++ ){
            if( ( GrMaxRGB( p[ j ] ) < 2 ) && ( MaxLen >= j ) ) MaxLen = j;
        }
    }
    MaxLen -= 8;
    if( MaxLen < 0 ) MaxLen = 0;
    FontSet( 102 );
    h = gFont.ChrHeight();
    gFont.Print( gChrEditPerkDlgSurf + 15751, Name, 573, 573, COLOR_BLACK );
    if( Param ){
        FontSet( 101 );
        gFont.Print( gChrEditPerkDlgSurf + 573 * ( h + 23 - gFont.ChrHeight() ) + gFont.LineWidth( Name ) + 288, Param, 573, 573, COLOR_BLACK );
    }
    WinDrawLine( gChrEditPerkWin, 280, h + 27, 545, h + 27, COLOR_BLACK );
    WinDrawLine( gChrEditPerkWin, 280, h + 28, 545, h + 28, COLOR_BLACK );
    FontSet( 101 );

    h = gFont.ChrHeight() + 1;
    if( WinTextWrap( Dsc, MaxLen + 133, Offsets, &Lines) ) return -1;
    j = 70;
    for( i = 0; i < (Lines - 1); i++, j += h ){
        s1 = &Dsc[ Offsets[ i + 0 ] ];
        s2 = &Dsc[ Offsets[ i + 1 ] ];
        tmp = *s2;
        *s2 = '\0';
        gFont.Print( &gChrEditPerkDlgSurf[ 573 * j + 280 ], s1, 573, 573, COLOR_BLACK );        
        *s2 = tmp;
    }
    if( (ImgId != gChrEditUnk14 || strcmp(gChrEditUnk19, Name)) && gChrEditSavePerks[0] ) GSoundPlay( "isdxxxx1" );
    strcpy( gChrEditUnk19, Name );
    gChrEditUnk14 = ImgId;
    gChrEditSavePerks[ 0 ] = 1;
    ArtClose( Img );
    return 0;
}

void CharEditPerksSave()
{
    int i;
    for( i = 0; i < 119; i++ ){
        gChrEditSavePerks[ i ] = PerkLvl( gObjDude, i );
    }
}

void CharEditPerksApply()
{
    int i;
/* !!!! */
DD
return;
    for( i = 0; i < 119; i++ ){
        while ( 1 ){
            if( PerkLvl( gObjDude, i ) <= gChrEditSavePerks[ i ] ) break;
            PerkLvlDec( gObjDude, i );
        }
    }
    for( i = 0; i < 119; i++ ){
        while( 1 ){
            if( PerkLvl( gObjDude, i ) >= gChrEditSavePerks[ i ] ) break;
            PerkLvlInc( gObjDude, i );
        }
    }
}

int CharEditPerkLimit()
{
    int i, cnt = 0;

    for( i = 0; i < 119; i++ ){
        if( (PerkLvl( gObjDude, i ) > 0) && ++cnt >= 37 ) break;
    }
    return cnt;
}

int CharEditStatOverFlow()
{
    int i;

    for( i = 0; i <= 6; i++ ){
        if( FeatGetTotal( gObjDude, i ) + FeatGetBoost( gObjDude, i ) > 10 ) return 1;
    }
    return 0;
}

void CharEditFolderArrows()
{
    int prev;

    prev = gChrEditFoldArrBtDn;
    gChrEditFoldKarmaOffs = 0;
    gChrEditFoldPerkOffs = 0;
    gChrEditFoldKillsOffs = 0;
    if( gChrEditFoldArrBtDn == -1 ){
        gChrEditFoldArrBtDn = WinCreateButton( gChrEditWin, 317, 364, gChrEditImgGeo[22].Width, gChrEditImgGeo[22].Height, gChrEditFoldArrBtDn, gChrEditFoldArrBtDn, gChrEditFoldArrBtDn, 17000, gChrEditPix[21], gChrEditPix[22], 0, 32 );
        WinSetClickSound( gChrEditFoldArrBtDn, GSoundPlayPushBt, 0 );
        if( prev == gChrEditFoldArrBtDn )return;
    }
    if( gChrEditFoldArrBtUp != -1 ) return;
    gChrEditFoldArrBtUp = WinCreateButton(gChrEditWin, 317, gChrEditImgGeo[22].Height + 365, gChrEditImgGeo[4].Width, gChrEditImgGeo[4].Height, gChrEditFoldArrBtUp, gChrEditFoldArrBtUp, gChrEditFoldArrBtUp, 17001, gChrEditPix[3], gChrEditPix[4], 0, 32);
    WinSetClickSound(gChrEditFoldArrBtUp, GSoundPlayPushBt, 0);
    if( gChrEditFoldArrBtUp == -1 ) WinImageRemove( gChrEditFoldArrBtDn );
}

void CharEditFoldArrDelete()
{
    if( gChrEditFoldArrBtUp != -1 ){ WinImageRemove( gChrEditFoldArrBtUp ); gChrEditFoldArrBtUp = -1; }
    if( gChrEditFoldArrBtDn != -1 ){ WinImageRemove( gChrEditFoldArrBtDn ); gChrEditFoldArrBtDn = -1; }
}

void CharEditFolderUpDn( int dir )
{
    int *p;

    switch( gChrEditFolderType ){
	case 0:  p = &gChrEditFoldPerkOffs; break;
	case 1:  p = &gChrEditFoldKarmaOffs; break;
	default: p = &gChrEditFoldKillsOffs; break;
    }

    if( dir >= 0 ){
        if( gChrEditUnk09 + gChrEditFolderOffset <= gChrEditUnk38 ){
            gChrEditFolderOffset++;
            if( gChrEditSelected >= 10 && gChrEditSelected < 43 && gChrEditSelected != 10 ) gChrEditSelected--;
        }
    } else if( gChrEditFolderOffset > 0 ){
        gChrEditFolderOffset--;
        if( gChrEditSelected >= 10 && gChrEditSelected < 43 && gChrEditUnk09 + 9 > gChrEditSelected ) gChrEditSelected++;
    }
    *p = gChrEditFolderOffset;
    CharEditShowFolder();
    if( gChrEditSelected >= 10 && gChrEditSelected < 43 ){
        ScrCopy( gChrEditWpPix + 171225, 277, 170, 640, gChrEditSurface + 171225, 640 );
        CharEditCaption( gChrEditDscImgId, gChrEditDscTitle, gChrEditDscParams, gChrEditDscDsc );
    }
}

void CharEditFoldArrowsHandle()
{
    gChrEditUnk38 = 0;
    gChrEditUnk41 = 364;
    gChrEditUnk09 = 9;
    gChrEditUnk39 = gFont.ChrHeight() + 1;
    gChrEditUnk40 = ( gChrEditSelected < 10 || gChrEditSelected >= 43 ) ? -1 : gChrEditSelected - 10;
    switch( gChrEditFolderType ){
	case 0: gChrEditFolderOffset = gChrEditFoldPerkOffs; break;
	case 1: gChrEditFolderOffset = gChrEditFoldKarmaOffs; break;
	case 2: gChrEditFolderOffset = gChrEditFoldKillsOffs; break;
    }
}

int CharEditPerksFoldTitle( char *str )
{
    int w, y;
    int v8, v9, v7;

    v9 = 0;
    if( gChrEditUnk09 + gChrEditFolderOffset <= gChrEditUnk38 ) return 0;
    if( gChrEditUnk38 >= gChrEditFolderOffset ){
        if( gChrEditUnk38 - gChrEditFolderOffset == gChrEditUnk40 ) v9 = 1;
        w = 280;
        y = gFont.ChrHeight() / 2 + gChrEditUnk41;
        if( str ){            
    	    v7 = gFont.Distance();
            v8 = v7 + gFont.LineWidth( str );
            w = (280 - v8) / 2;
            gFont.Print( &gChrEditSurface[640 * gChrEditUnk41 + 34 + w + v7 ], str, 640, 640, gPalColorCubeRGB[0][31][0] );
            WinDrawLine( gChrEditWin, v8 + w + 34, y, v8 + 2*w + 34, y, gPalColorCubeRGB[0][31][0] );
        }
        WinDrawLine( gChrEditWin, 34, y, w + 34, y, gPalColorCubeRGB[0][31][0] );
        gChrEditUnk41 += gChrEditUnk39;
    }
    gChrEditUnk38++;
    return v9;
}

int CharEditFoldLine( char *Str )
{
    int selected, col;

    selected = 0;
    if( gChrEditUnk09 + gChrEditFolderOffset <= gChrEditUnk38 ) return 0;
    if( gChrEditUnk38 >= gChrEditFolderOffset ){
        if( gChrEditUnk38 - gChrEditFolderOffset == gChrEditUnk40 ){
            selected = 1;
            col = gPalColorCubeRGB[31][31][11];
        } else {
            col = gPalColorCubeRGB[0][31][0];
        }
        gFont.Print( &gChrEditSurface[640 * gChrEditUnk41 + 34], Str, 640, 640, col );
        gChrEditUnk41 += gChrEditUnk39;
    }
    gChrEditUnk38++;
    return selected;
}

int CharEditFoldKillsLine( char *Name, int Val )
{
    int col, Selected, y, var_20;
    char stmp[128];

    Selected = 0;
    if( gChrEditUnk09 + gChrEditFolderOffset <= gChrEditUnk38 ) return 0;
    if( gChrEditUnk38 >= gChrEditFolderOffset ){
        if( gChrEditUnk38 - gChrEditFolderOffset == gChrEditUnk40 ){
            col = gPalColorCubeRGB[31][31][11];
            Selected = 1;
        } else {
            col = gPalColorCubeRGB[0][31][0];
        }
        sprintf( stmp, "%d", Val );
        gFont.Print( &gChrEditSurface[ 640 * gChrEditUnk41 + 34 ], Name, 640, 640, col );
        y = gFont.ChrHeight() / 2 + gChrEditUnk41;
        var_20 = 314 - gFont.LineWidth( stmp ) ;
        WinDrawLine( gChrEditWin, 34 + gFont.LineWidth( Name ), y, var_20, y, col );
        gFont.Print( &gChrEditSurface[ 640 * gChrEditUnk41 + var_20 ], stmp, 640, 640, col );
        gChrEditUnk41 += gChrEditUnk39;
    }
    gChrEditUnk38++;
    return Selected;
}

int CharEditLoadKarma()
{
    xFile_t *fh;
    char *s, *p, line[256], delim[4];
    KarmaMsg_t *kp, *k;
    int arg1, arg2, arg3, arg4, err;

    err = 0;
    strcpy( delim, " \t," );

    if( gCharEditKarma ){
        Free( gCharEditKarma );
        gCharEditKarma = NULL;
    }
    gChrEditMsgKarmaCount = 0;
    if( !( fh = dbOpen( "data/karmavar.txt", "rt" ) ) ) return -1;
    while( dbgets( line, 256, fh ) ){
        for( s = line; IsTable[ (int)*s ] & 0x02; s++ );
        if( *s == '#' ) continue;
        if( !( p = strtok( s, delim ) ) ) continue;
        arg1 = strtol( p, NULL, 10 );                
        if( !( p = strtok( NULL, delim ) ) ) continue;
        arg2 = strtol( p, NULL, 10  );                    
        if( !( p = strtok( NULL, delim ) ) ) continue;
        arg3 = strtol( p, NULL, 10  );
        if( !(p = strtok( NULL, delim ) ) ) continue;
        arg4 = strtol( p, NULL, 10  );
        kp = (KarmaMsg_t *)Realloc( gCharEditKarma, sizeof( KarmaMsg_t ) * ( gChrEditMsgKarmaCount + 1 ) );
        if( !kp ){ err = -1; break; }
        k = &kp[ gChrEditMsgKarmaCount ];
        k->Val = arg1;
        k->ImgId = arg2;
        k->TitleId = arg3;
        k->DscId = arg4;
        gCharEditKarma = kp;
        gChrEditMsgKarmaCount++;
    }
    if( !err ) qsort( gCharEditKarma, gChrEditMsgKarmaCount, sizeof( KarmaMsg_t ), (void *)CharEditKarmaSortCb );
    dbClose( fh );
    return err;
}

void CharEditFreeKarma()
{
    if( gCharEditKarma ){
        Free(gCharEditKarma);
        gCharEditKarma = NULL;
    }
    gChrEditMsgKarmaCount = 0;
}

static int CharEditKarmaSortCb( KarmaMsg_t *kr1, KarmaMsg_t *kr2 )
{
    return kr1->Val - kr2->Val;
}

int CharEditLoadGenre()
{
    xFile_t *fh;
    GenreMsg_t *gp, *g;
    char *s, *p, line[256], delim[4];
    int arg1, arg2, err;

    err = 0;
    strcpy( delim, " \t," );
    if( gCharEditGenre ) {
        Free( gCharEditGenre );
        gCharEditGenre = NULL;
    }
    gCharEditGenreCount = 0;
    fh = dbOpen( "data/genrep.txt", "rt" );
    if( !fh ) return -1;
    while( dbgets( line, 256, fh ) ){
        for( s = line; IsTable[ (int)*s ] & 0x02; s++ );
        if( *s == '#' ) continue;
        if( !(p = strtok( s, delim ) ) ) continue;
        arg1 = (*p == '-') ? -strtol( ++p, NULL, 10 ) : strtol( p, NULL, 10 );
        if( !(p = strtok( 0, delim )) ) continue;
        arg2 = strtol( p, NULL, 10 );
        gp = (GenreMsg_t *)Realloc(gCharEditGenre, sizeof( GenreMsg_t ) * ( gCharEditGenreCount + 1 ) );
        if( !gp ){ err = -1; break; }
        gCharEditGenre = gp;
        g = &gp[ gCharEditGenreCount ];
        g->i01 = arg1;
        g->i02 = arg2;
        gCharEditGenreCount++;
    }
    if( !err ) qsort( gCharEditGenre, gCharEditGenreCount, sizeof( GenreMsg_t ), (void *)CharEditSortGenreCb );
    dbClose( fh );
    return err;
}

void CharEditFreeGenre()
{
    if( gCharEditGenre ){
        Free( gCharEditGenre );
        gCharEditGenre = NULL;
    }
    gCharEditGenreCount = 0;
}

static int CharEditSortGenreCb( GenreMsg_t *gen1, GenreMsg_t *gen2 )
{
    return gen2->i01 - gen1->i01;
}

