#include "FrameWork.h"

int gGmouseInited = 0;
int gGmouseHexCursor = 0;
int gGmouseUnk10 = 0;
int gGmouseUnk11 = 0;
int gGmouseMoveAllow = 1;
int gGmouseCursorId = 0;
CachePool_t *gGmouseCursorImg = ART_NULL;
static int gGmouseCursorIds[ 27 ] = { 
    0x10A, 0x10B, 0x10C, 0x10D, 0x10E, 0x10F, 0x110, 0x111, 0x112,
    0x113, 0x114, 0x115, 0x14A, 0x14B, 0x149, 0x148, 0x14C, 0x14E,
    0x14D, 0x14F, 0x117, 0x118, 0x119, 0x125, 0x136, 0x116, 0x127
};
int gGmouseUnk13 = 0;
int gGmouseStay = 0;
int gGmouseRstTime = 0;
ArtFrmHdr_t *gGmouseImgA = NULL;
CachePool_t *gGmouseImgObjA = ART_NULL;
int gGmouseWidthImgA = 0;
int gGmouseHeightImgA = 0;
int gGmouseSizeImgA = 0;
int gGmouseUnk55 = 0;
int gGmouseUnk54 = 0;
char *gGmouseDataImgA = 0;
ArtFrmHdr_t *gGmouseImgB = NULL;
CachePool_t *gGmouseImgObjB = ART_NULL;
int gGmouseWidthImgB = 0;
int gGmouseHeightImgB = 0;
int gGmouseSizeImgB = 0;
int gGmouseMidX  = 0;
int gGmouseMidY  = 0;
char *gGmouseDataImgB = 0;
ArtFrmHdr_t *gGmouseImgC = NULL;
CachePool_t *gGmouseImgObjC = ART_NULL;
int gGmouseWidthImgC = 0;
int gGmouseHeightImgC = 0;
int gGmouseSizeImgC = 0;
char *gGmouseDataImgC = 0;
ArtFrmHdr_t *gGmouseImgD = NULL;
CachePool_t *gGmouseImgObjD = ART_NULL;
int gGmouseWidthImgD = 0;
int gGmouseHeightImgD = 0;
int gGmouseSizeImgD = 0;
char *gGmouseDataImgD = 0;
ArtFrmHdr_t *gGmouseImgE = NULL;
CachePool_t *gGmouseImgObjE = ART_NULL;
int gGmouseWidthImgE = 0;
int gGmouseHeightImgE = 0;
int gGmouseSizeImgE = 0;
char *gGmouseDataImgE = 0;
char gGmouseUnk32 = 0;
char *gGmouseUnk31 = 0;
char gGmouseUnk30 = 0;
short gGmouseUnk29[ 11 ] = { 253, 255, 257, 259, 261, 263, 265, 302, 304, 435, 0 };
int gGmouseUnk23 = 1;
int gGmouseShape = 0;
int gGmouseShapeIds[2] = { 249, 250 };
int gGmouseUnk26[ 5 ]= { 251, 293, 293, 293, 293 };
int gGmouseUnk25[11] = { 293, 293, 293, 293, 6, 7, 9, 10, 11, 12, 13 };
int gGmouseUnk06 = 0;
int gGmouseTime = 0;
int gGmouseRestoreCursor = -1;
int gItemHlShow = 1;
Obj_t *gGmouseObjC = NULL;
int gGmouseUnk75 = 0;
int gGmouseUnk21 = -1;
int gGmouseUnk61[ 10 ];
int gGmouseOldX;
int gGmouseOldY;
Obj_t *gGmouseObjA;
Obj_t *gGmouseObjB;
Obj_t *gGmouseActionObject;

int GmouseInit()
{
    if( gGmouseInited ) return -1;
    if( GmouseInitA() ) return -1;
    gGmouseInited = 1;
    gGmouseHexCursor = 1;
    GmouseLoadCursor( 1 );
    return 0;
}

int GmouseReset()
{
    if( !gGmouseInited ) return -1;
    if( gGmouseUnk13 ){
        GmouseUnk34();
        GmouseSetHexCursorBlank();
        GmouseSetMode(0);
        GmouseIsoEnter();
        gGmouseOldX = -1;
        gGmouseOldY = -1;
        gGmouseStay = 0;
        gGmouseRstTime = TimerGetSysTime();
        GmouseHLcfg();
    } else {
        return -1;
    }
    if( !gGmouseHexCursor ){
        gGmouseCursorId = -1;
        GmouseLoadCursor( 0 );
        GmouseScrollEnable();
        gGmouseHexCursor = 1;
        gGmouseRestoreCursor = -1;
    }
    gGmouseMoveAllow = 1;
    GmouseLoadCursor( 1 );
    gGmouseUnk06 = 0;
    gGmouseTime = 0;
    gGmouseUnk75 = 0;
    return 0;
}

void GmouseClose()
{
    if( !gGmouseInited ) return;    
    MseCursorRedraw();
    MseSetStaticCursor( 0, 0, 0, 0, 0, 0, 0 );
    if( gGmouseUnk13 ){
        GmouseCloseImages();
        gGmouseObjA->Flags &= ~0x400;
        gGmouseObjB->Flags &= ~0x400;
        ObjDestroy( gGmouseObjA, 0 );
        ObjDestroy( gGmouseObjB, 0 );
        gGmouseUnk13 = 0;
    }
    if( gGmouseCursorImg != ART_NULL ) ArtClose( gGmouseCursorImg );
    gGmouseCursorImg = ART_NULL;
    gGmouseHexCursor = 0;
    gGmouseInited = 0;
    gGmouseCursorId = -1;
}

void GmouseSetIsoMode()
{
    if( gGmouseHexCursor ) return;    
    gGmouseCursorId = -1;
    GmouseLoadCursor( 0 );
    gGmouseMoveAllow = 1;
    gGmouseHexCursor = 1;
    gGmouseRestoreCursor = -1;    
}

void GmouseSetIfaceMode( int ScrollEn )
{
    if( !gGmouseHexCursor ) return;    
    GmouseLoadCursor( 0 );
    gGmouseHexCursor = 0;
    gGmouseMoveAllow = ( (ScrollEn & 1) != 0 ) ? 1 : 0;    
}

int GmouseHexCursorMode()
{
    return gGmouseHexCursor;
}

void GmouseScrollEnable()
{
    gGmouseMoveAllow = 1;
}

void GmouseScrollDisable()
{
    gGmouseMoveAllow = 0;
}

int GmouseScrollEnabled()
{
    return gGmouseMoveAllow;
}

void GmouseUnk28( int a1 )
{
    if( a1 == gGmouseUnk11 ) return;    
    gGmouseUnk11 = a1;
    gGmouseUnk75 = 0;
}

int GmouseUnk29()
{
    return gGmouseUnk11;
}

int GmouseReachBorder()
{
    int y, x;

    if( !gGmouseMoveAllow ) return 0;    
    MseGetCursorPosition( &x, &y );
    if( x != gVidMainGeo.lt && x != gVidMainGeo.rt && y != gVidMainGeo.tp && y != gVidMainGeo.bm ) return 0;
    
    switch( gGmouseCursorId ){
        case 4 ... 19: return 1;
    }    
    return 0;
}

#define GMOUSE_SCROLL_VIEWPORT()	\
    MseGetCursorPosition( &MseX, &MseY );\
    PrevCursor = gGmouseCursorId;\
    if( !GmouseViewMove( MseX, MseY ) ){ if( PrevCursor < 4 || PrevCursor > 19 ) gGmouseRestoreCursor = PrevCursor; return; }\
    if( gGmouseRestoreCursor != -1 ){ GmouseLoadCursor( gGmouseRestoreCursor ); gGmouseRestoreCursor = -1; }

void GmouseProcess()
{
    int Time,v6,v7,v8,Action,PrevCursor,MseX,MseY,EntryValue,flg, v32;
    unsigned char color;
    char stmp1[ 16 ], stmp2[8];
    Obj_t *obj;
    VidRect_t v20,v21,Rect,Area4,Area1,Area2,Area3;

    if( !gGmouseInited ) return;

    if( gGmouseCursorId >= 25 ){
        MseUpdate();
        if( !gGmouseMoveAllow ){ GmouseLoadCursor( gGmouseCursorId ); return; }
	GMOUSE_SCROLL_VIEWPORT();
        return;

    }
    if( !gGmouseHexCursor ){
        if( !gGmouseMoveAllow ) return;
	GMOUSE_SCROLL_VIEWPORT();
        return;
    }
    GMOUSE_SCROLL_VIEWPORT();

    if( WinGetPointedWidgetId( MseX, MseY ) != gMapIsoWin ){
        if( !gGmouseCursorId ){
            GmouseUnk03();
            GmouseLoadCursor( 1 );
            if( gGmouseShape >= 2 && (gCombatStatus & 1) == 0 ) GmouseSetMode(0);
        }
        return;
    }
    if( gGmouseCursorId ){
        if( gGmouseCursorId <= 3 || !((gGmouseCursorId < 20 || gGmouseCursorId > 20) && gGmouseCursorId != 23 ) ){
    	    GmouseLoadCursor( 0 );
	    if( (gGmouseObjB->Flags & 0x01) ) GmouseIsoEnter();
	}
    } else {
	if( (gGmouseObjB->Flags & 0x01) ) GmouseIsoEnter();
    }
    if( !GmouseCursorUpdate( MseX, MseY, gCurrentMapLvl, &Area3 ) ) TileUpdateArea( &Area3, gCurrentMapLvl );
    if( (gGmouseObjB->Flags & 0x01) || gGmouseUnk10 ) return;
    Time = TimerGetTime();
    if( MseX == gGmouseOldX && MseY == gGmouseOldY ){
        if( gGmouseStay || TimerDiff( Time, gGmouseRstTime ) < 250 ) return;
        if( gGmouseShape ){
            if( gGmouseShape == 1 ){ // pointed cursor
                gGmouseRstTime = Time;
                gGmouseStay = 1;                
                if( (obj = GmouseGetObject( -1, 1, gCurrentMapLvl )) ){
                    Action = -1;
                    switch( OBJTYPE( obj->ImgId ) ){
                        case TYPE_ITEM:
                            Action = ACTION_USE;
                            if( gItemHlShow == 1 && !ObjSetOutline( obj, 16, &v21 ) ){
                                TileUpdateArea( &v21, gCurrentMapLvl );
                                gGmouseObjC = obj;
                            }
                            break;
                        case TYPE_CRIT:
                            if( obj == gObjDude ){ 
                        	Action = ACTION_TURN;
                            } else if( ObjTalkative( obj ) ){ 
                        	Action = IN_COMBAT ? ACTION_INFO: ACTION_TALK;
                    	    } else {
                        	Action = ( CritterGetInjure( obj->Pid, 0x20 ) ) ? ACTION_INFO : ACTION_USE;
                            }
                            break;
                        case TYPE_SCEN: Action = !ObjOpenable( obj ) ? ACTION_INFO: ACTION_USE; break;
                        case TYPE_WALL: Action = ACTION_INFO; break;
                        default: break;
                    }
                    if( Action != -1 && !GmouseTile( MseX, MseY, Action, gVidMainGeo.rt - gVidMainGeo.lt + 1, gVidMainGeo.bm - gVidMainGeo.tp - 99 ) ){
                        if( ObjSetShape( gGmouseObjB, ArtMakeId( 6, 282, 0, 0, 0 ), &Area4 ) == 0 ) TileUpdateArea( &Area4, gCurrentMapLvl );
                    }
                    if( obj != gGmouseActionObject ){
                        gGmouseActionObject = obj;
                        UseLookMsg( gObjDude, obj );
                    }
                }
            } else if( gGmouseShape == 2 ){                
                if( !(obj = GmouseGetObject( 1, 0, gCurrentMapLvl )) ){
                    obj = GmouseGetObject( -1, 0, gCurrentMapLvl );
                    if( !GmouseUnk43( obj ) ) obj = NULL;
                }
                if( obj ){
                    EntryValue = 0;
                    flg = OBJTYPE( obj->ImgId ) == TYPE_CRIT;
                    CfgGetInteger( &gConfiguration, "preferences", "combat_looks", &EntryValue );
                    if( EntryValue && UseExamineMsg( gObjDude, obj ) == -1 ) UseLookMsg( gObjDude, obj );
                    if( CombatUnk71( obj, &v32 ) ){
                        sprintf( stmp2, "%d%%", v32 );
                        if( flg ){
                            color = ( obj->Critter.State.GroupId ) ? gPalColorCubeRGB[31][31][31] : gPalColorCubeRGB[31][23][15];
                        } else {
                            color = gPalColorCubeRGB[17][17][17];
                        }
                    } else {
                        sprintf( stmp2, " %c ", 'X' );
                        if( flg ){
                            color = ( obj->Critter.State.GroupId ) ? gPalColorCubeRGB[31][0][0] : gPalColorCubeRGB[17][23][17];
                        } else {
                            color = gPalColorCubeRGB[31][15][15];
                        }
                    }
                    if( !GmousePrint( stmp2, color ) ){
                        if( ObjSetShape( gGmouseObjB, ArtMakeId( 6, 284, 0, 0, 0 ), &v20 ) == 0 ) TileUpdateArea( &v20, gCurrentMapLvl );
                    }
                    if( obj != gGmouseActionObject ) gGmouseActionObject = obj;
                } else if( !GmouseSetShapeB( &Area3 ) ){
                    TileUpdateArea( &Area3, gCurrentMapLvl );
                }
                gGmouseRstTime = Time;
                gGmouseStay = 1;
            }
            return;
        }
        if( (v6 = AnimFindTrace( gObjDude, gObjDude->GridId, gGmouseObjB->GridId, NULL, 1 ) ) ){
            if( (gCombatStatus & 1) == 0 ){
                stmp1[0] = '\0';
                color = gPalColorCubeRGB[31][0][0];
            } else {
        	v7 = CritterUnk47( gObjDude, v6 );
        	v8 = ( v7 - gCombatMovePts >= 0 ) ? ( v7 - gCombatMovePts ) : 0;
        	if( v8 <= gObjDude->Critter.State.CurrentAP ){
            	    sprintf( stmp1, "%d", v8 );
            	    color = gPalColorCubeRGB[31][31][31];
        	} else {
    		    sprintf( stmp1, "%c", 'X' );
    		    color = gPalColorCubeRGB[31][0][0];
    		}
    	    }
        } else { // '< X >'
    	    sprintf( stmp1, "%c", 'X' );
    	    color = gPalColorCubeRGB[31][0][0];
        }        
        if( !GmouseText( stmp1, color ) ){ ObjGetRadiusArea( gGmouseObjB, &Rect ); TileUpdateArea( &Rect, 0 ); }
        gGmouseRstTime = Time;
        gGmouseStay = 1;
        gGmouseUnk21 = gGmouseObjB->GridId;
        return;
    }
    gGmouseRstTime = Time;
    gGmouseStay = 0;
    gGmouseOldX = MseX;
    gGmouseOldY = MseY;
    flg = 0;
    if( !gGmouseUnk10 ) GmouseSetShapeA( ArtMakeId( 6, 0, 0, 0, 0 ) );    
    if( !GmouseSetShapeB( &Area1 ) ) flg |= 0x01;
    if( gGmouseObjC ){
        if( !ObjGetRadius( gGmouseObjC, &Area2 ) ) flg |= 0x02;
        gGmouseObjC = 0;
    }
    if( flg < 2 ){
	if( flg == 1 ) TileUpdateArea( &Area1, gCurrentMapLvl );
    } else {
	if( flg <= 2 ){ TileUpdateArea( &Area2, gCurrentMapLvl); return; }
        if( flg == 3 ){ RegionExpand( &Area1, &Area2, &Area1 ); TileUpdateArea( &Area1, gCurrentMapLvl ); }
    }
}

void GmouseAction( int MseX, int MseY, int MseButt )
{
    Obj_t *obj, *p;
    VidRect_t Area1, Area2;
    unsigned char MenuPositions;
    int ap,tmp,sel,pos,IconMenu[ 6 ],bottom,right;

    if( !gGmouseInited ) return;
    if( gGmouseCursorId >= 25 ) return;
    if( !gGmouseHexCursor ) return;
    if( gGmouseUnk75 && GmouseUnk29() ) return;
    if( !MseCursorCenterInArea( 0, 0, gVidMainGeo.rt - gVidMainGeo.lt, gVidMainGeo.bm - gVidMainGeo.tp - 100 ) ) return;     
    // Click on ISO area
    if( MseButt & MSE_RBUTT ){  // right click
	if( (MseButt & MSE_RBUTT_HOLD) == 0 && (gGmouseObjB->Flags & 1) == 0 ) GmouseCycleMode(); 	
	return; 
    }
    if( (MseButt & MSE_LBHOLD) != 0 ){ // normal click
    	switch( gGmouseShape ){
            case 0: // pointed place to go
                ap = IN_COMBAT ? (gCombatMovePts + gObjDude->Critter.State.CurrentAP) : -1;
                CfgGetInteger( &gConfiguration, "preferences", "running", &tmp );
                if( KeyShiftKeyState() ){
                    if( tmp ){ AnimWalk( ap ); return; }
                } else if( !tmp ){ AnimWalk( ap ); return; }
                AnimRun( ap );
                return;
            case 1: // pointed object
                if( !( obj = GmouseGetObject( -1, 1, gCurrentMapLvl ) ) ) return;
                switch( OBJTYPE( obj->ImgId ) ){
                    case TYPE_ITEM: ActionPickupItem( gObjDude, obj ); break;
                    case TYPE_CRIT:
                        if( obj == gObjDude ){
                            if( !((gObjDude->ImgId & 0xFF0000) >> 16) && !ObjTurnCW( obj, &Area2 ) ) TileUpdateArea( &Area2, obj->Elevation );
                        } else if( ObjTalkative( obj ) ){
                            if( IN_COMBAT ){
                                if( UseExamineMsg( gObjDude, obj ) == -1 ) UseLookMsg( gObjDude,obj );
                            } else {
                                ActionTalk( gObjDude, obj );
                            }
                        } else {
                            ActionUseOnCritter( gObjDude, obj );
                        }
                        break;
                    case TYPE_SCEN:
                        if( ObjOpenable(obj) ){ ActionUseOnScenery( gObjDude, obj ); }
                        else if ( UseExamineMsg( gObjDude, obj ) == -1 ){ UseLookMsg( gObjDude, obj ); }
                        break;
                    case TYPE_WALL:
                        if( UseExamineMsg( gObjDude, obj ) == -1 ) UseLookMsg( gObjDude, obj );
                        break;
                }                    
                return;
            case 2:
                obj = GmouseGetObject( 1, 0, gCurrentMapLvl );
                if( obj ) return;                    
                obj = GmouseGetObject( -1, 0, gCurrentMapLvl );
                if( !GmouseUnk43( obj ) ) return;                        
                CombatStartAttack( obj );
                gGmouseStay = 1;
                gGmouseOldY = MseY;
                gGmouseOldX = MseX;
                gGmouseRstTime = TimerGetSysTime() - 250;                    
                return;
            case 3:
                obj = GmouseGetObject( -1, 1, gCurrentMapLvl );
                if( obj && IfaceGetHandObject( &p ) != -1 ){
                    if( IN_COMBAT ){
                        if( (ap = ItemGetSlotApCost(gObjDude, ( IfaceGetSelectedHand() ) ? 2 : 0, 0)) <= gObjDude->Critter.State.CurrentAP && ActionUseSceneryObject( gObjDude, obj, p ) != -1 ){
                            if( ap > gObjDude->Critter.State.CurrentAP )
                                gObjDude->Critter.State.CurrentAP = 0;
                            else
                                gObjDude->Critter.State.CurrentAP -= ap;
                            IfaceRenderAP( gObjDude->Critter.State.CurrentAP, gCombatMovePts );
                        }
                    } else {
                        ActionUseSceneryObject( gObjDude, obj, p );
                    }
                }
                GmouseLoadCursor( 0 );
                GmouseSetMode( 0 );
                return;
            case 4 ... 10:
                obj = GmouseGetObject( -1, 1, gCurrentMapLvl );
                if( !obj || ActionSkillUse( gObjDude, obj, gGmouseUnk25[ gGmouseShape ] ) != -1 ){
                    GmouseLoadCursor( 0 );
                    GmouseSetMode( 0 );
                }
                return;
        }
        return;
    }
    if( (MseButt & 0x05) != 5 || gGmouseShape != 1 ) return;
    // click & hold
    obj = GmouseGetObject( -1, 1, gCurrentMapLvl );
    if( !obj ) return;
    MenuPositions = 0;
    switch( OBJTYPE( obj->ImgId ) ){
        case TYPE_ITEM:
            MenuPositions = 2; 
            IconMenu[ 0 ] = INV_ACT_USE; 
            IconMenu[ 1 ] = INV_ACT_INSPECT;
            if( ItemGetObjType( obj ) == 1 ){ 
        	MenuPositions = 4; 
        	IconMenu[ 3 ] = INV_ACT_SKILL; 
        	IconMenu[ 2 ] = INV_ACT_TAKE; 
    	    }
	    IconMenu[ MenuPositions++ ] = INV_ACT_CANCEL;
            break;
        case TYPE_CRIT:
            if( obj == gObjDude ){ 
        	MenuPositions = 1; IconMenu[ 0 ] = 4; 
    	    } else {
                if( ObjTalkative( obj ) ){ 
            	    if( !IN_COMBAT ){ 
            		MenuPositions = 1; 
            		IconMenu[ 0 ] = INV_ACT_TALK; 
            	    }
                } else if( !CritterGetInjure( obj->Pid, 0x20 ) ){ 
            	    MenuPositions = 1; 
            	    IconMenu[ 0 ] = INV_ACT_USE; 
            	} 
            	if( ActionUnk03( gObjDude, obj ) ) IconMenu[ MenuPositions++ ] = 9; 
            }
            IconMenu[ MenuPositions++ ] = INV_ACT_INSPECT; 
            IconMenu[ MenuPositions++ ] = INV_ACT_TAKE; 
            IconMenu[ MenuPositions++ ] = INV_ACT_SKILL; 
            IconMenu[ MenuPositions++ ] = INV_ACT_CANCEL;
            break;
        case TYPE_SCEN:
            if( ObjOpenable( obj ) ){ IconMenu[ 0 ] = 6; MenuPositions = 1; } 
    	    IconMenu[ MenuPositions++ ] = INV_ACT_INSPECT; 
    	    IconMenu[ MenuPositions++ ] = INV_ACT_TAKE; 
    	    IconMenu[ MenuPositions++ ] = INV_ACT_SKILL; 
    	    IconMenu[ MenuPositions++ ] = INV_ACT_CANCEL;
            break;
        case TYPE_WALL:
            MenuPositions = 1; 
            IconMenu[ 0 ] = INV_ACT_INSPECT; 
            if( ObjOpenable( obj ) ){ MenuPositions = 2; IconMenu[ 1 ] = INV_ACT_TAKE; } 
            IconMenu[ MenuPositions ] = INV_ACT_CANCEL;
            MenuPositions++;
            break;
    }
    // create icon submenu
    if( GmouseCursorMenuCreate( MseX, MseY, IconMenu, MenuPositions, gVidMainGeo.rt - gVidMainGeo.lt + 1, gVidMainGeo.bm - gVidMainGeo.tp - 99 ) ) return;
    if( ObjSetShape( gGmouseObjB, ArtMakeId( 6, 283, 0, 0, 0 ), &Area1 ) != 0 ) return;
    if( GmouseCursorUpdate( MseX, MseY, gCurrentMapLvl, &Area1 ) ) return;
    TileUpdateArea( &Area1, gCurrentMapLvl );
    MapUnk35();
    tmp = MseY;
    sel = 0;
    while( ( MseGetButtons() & MSE_LBHOLD ) == 0 ){
        InpUpdate();
        if( gMenuEscape ) IconMenu[ sel ] = 0;
        MseGetCursorPosition( &right, &bottom );
        if( abs32( bottom - tmp ) > 10 ){
            pos = sel + (( tmp >= bottom ) ? -1 : 1);
            if( !GmouseMenuIconHL( pos ) ){
        	TileUpdateArea( &Area1, gCurrentMapLvl );
        	sel = pos;
            }
            tmp = bottom;
        }
    }
    MapUnk34();
    gGmouseOldX = MseX;
    gGmouseOldY = MseY;
    gGmouseStay = 0;
    gGmouseRstTime = TimerGetSysTime();
    MseSetCursorPosition( MseX, MseY );
    if( !GmouseSetShapeB( &Area1 ) ) TileUpdateArea( &Area1, gCurrentMapLvl );
    // use selected action
    switch( IconMenu[ sel ] ){
        case INV_ACT_TAKE: InvMenuBackPack( obj ); break;
        case INV_ACT_INSPECT: if( UseExamineMsg( gObjDude, obj ) == -1 ) UseLookMsg( gObjDude, obj ); break;
        case INV_ACT_TURN: if( !ObjTurnCW( obj, &Area1 ) ) TileUpdateArea( &Area1, obj->Elevation ); break;
        case INV_ACT_TALK: ActionTalk( gObjDude, obj ); break;
        case INV_ACT_USE:
            switch( OBJTYPE( obj->ImgId ) ){
        	case TYPE_CRIT: ActionUseOnCritter( gObjDude, obj ); break;
        	case TYPE_SCEN: ActionUseOnScenery( gObjDude, obj ); break;
        	default: ActionPickupItem( gObjDude, obj ); break;
            }
            break;
        case INV_ACT_SKILL:
            switch( SkillDexMenu() ){
                case 1: ActionUseSneak( SKILL_SNEAK ); break;
                case 2: ActionSkillUse( gObjDude, obj, SKILL_LOCKPICK ); break;
                case 3: ActionSkillUse( gObjDude, obj, SKILL_STEAL ); break;
                case 4: ActionSkillUse( gObjDude, obj, SKILL_TRAPS ); break;
                case 5: ActionSkillUse( gObjDude, obj, SKILL_FIRSTAID ); break;
                case 6: ActionSkillUse( gObjDude, obj, SKILL_DOCTOR ); break;
                case 7: ActionSkillUse( gObjDude, obj, SKILL_SCIENCE ); break;
                case 8: ActionSkillUse( gObjDude, obj, SKILL_REPAIR ); break;
            }
            break;
        case INV_ACT_9: ActionUnk02( gObjDude, obj ); break;
    }
}

int GmouseLoadCursor( int CursorId )
{
    int Id,err,SysTime, hh, ww, h,frm, w;
    unsigned int t_;
    ArtFrmHdr_t *Img = NULL;
    CachePool_t *CurObj;

    if( !gGmouseInited ) return -1;
    if( (CursorId != 1) && (CursorId == gGmouseCursorId) && (gGmouseCursorId < 25 || gGmouseCursorId >= 27) ) return -1;
    Id = ArtMakeId( 6, gGmouseCursorIds[ CursorId ], 0, 0, 0 );    
    if( !(Img = ArtLoadImg( Id, &CurObj ) ) ) return -1;
    err = 1;
    frm = 0;
    if( CursorId >= 25 ){
        SysTime = TimerGetSysTime();
        if( !( gGmouseObjB->Flags & 0x01) ) GmouseUnk03();
        t_ = 1000 / ArtGetObjSpeed(Img);
        if( TimerDiff(SysTime, gGmouseTime) < t_ ){
            err = 0;
        } else {
            if( ArtGetFpd(Img) <= gGmouseUnk06 ) gGmouseUnk06 = 0;
            frm = gGmouseUnk06;
            gGmouseTime = SysTime;
            gGmouseUnk06++;
        }
    }
    if( !err ) return -1;
    w = ArtGetObjWidth( Img, frm, 0 );
    h = ArtGetObjHeight( Img, frm, 0 );
    ArtGetCenter(Img, 0, &ww, &hh);
    ww = w / 2 - ww;
    hh = h - 1 - hh;

    if( MseSetStaticCursor( ArtGetObjData( Img, frm, 0 ), w, h, w, ww, hh, 0) ) return -1;
    if( gGmouseCursorImg != ART_NULL ) ArtClose( gGmouseCursorImg );
    gGmouseCursorId = CursorId;
    gGmouseCursorImg = CurObj;
    return 0;
}

int GmouseGetCursorId()
{
    return gGmouseCursorId;
}

void GmouseUnk50( int a1 )
{
    gGmouseUnk10 = a1;
}

void GmouseUnk34()
{
    gGmouseUnk23 = 1;
}

void GmouseUnk51()
{
    gGmouseUnk23 = 0;
}

int GmouseUnk52()
{
    return gGmouseUnk23;
}

void GmouseSetMode( int mode )
{
    int flg, MseY, MseX;
    VidRect_t Area1, Area2;

    if( !gGmouseInited ) return;    
    if( !gGmouseUnk23 ) return;    
    if( mode == gGmouseShape ) return; // already set    
    GmouseSetShapeA( CURSOR_BLANK );
    if( ObjSetShape( gGmouseObjB, ArtMakeId( 6, gGmouseShapeIds[ mode ], 0, 0, 0 ), &Area1 ) ) return;    
    MseGetCursorPosition( &MseX, &MseY );
    flg = 0;
    if( !GmouseCursorUpdate( MseX, MseY, gCurrentMapLvl, &Area2 ) ){
        RegionExpand( &Area1, &Area2, &Area1 );
    }
    if( gGmouseShape == 2 ) flg = -1;
    if( mode ){
        if( mode == 2 ) flg = 1;
        if( !gGmouseShape ){
    	    if( !ObjUnk35( gGmouseObjB, &Area2 ) ) RegionExpand( &Area1, &Area2, &Area1 ); // outline on ?
	}
    } else {
	if( !ObjUnk34( gGmouseObjB, &Area2 ) ) RegionExpand( &Area1, &Area2, &Area1 ); // putline off ?
    }
    gGmouseShape = mode;
    gGmouseStay = 0;
    gGmouseRstTime = TimerGetSysTime();
    TileUpdateArea( &Area1, gCurrentMapLvl );
    if( flg == 1 ){
        CombatTargetHighlight();
    } else if( flg == -1 ){
        CombatUnk74();
    }    
}

int GmouseGetMode()
{
    return gGmouseShape;
}

void GmouseCycleMode()
{
    int mode;
    Obj_t *obj;

    mode = (gGmouseShape + 1) % 3;
    if( IN_COMBAT ){
        if( !IfaceGetHandObject( &obj ) && obj && ( ItemGetObjType( obj ) != PR_ITEM_WEAPON) && (mode == 2) ) mode = 0;
    } else if( mode == 2 ){
        mode = 0;
    }
    GmouseSetMode( mode );
}

void GmouseUnk55()
{
    gGmouseOldX = -1;
    gGmouseOldY = -1;
    gGmouseStay = 0;
    gGmouseRstTime = 0;
    GmouseProcess();
}

void GmouseSetShapeA( int NewImg )
{
    int flg;
    VidRect_t Area1, Area2;

    if( !gGmouseInited ) return;
    if( !ArtFileExist( NewImg ) ) return;
    if( NewImg == gGmouseObjA->ImgId ) return;
    if( gGmouseUnk10 ){
        flg = 0;
        if( gGmouseObjA->ImgId != -1 ){ ObjGetRadiusArea( gGmouseObjA, &Area1 ); flg = 0x01; }
        if( !ObjSetShape( gGmouseObjA, NewImg, &Area2 ) ){ flg |= 0x02; }
        if( gGmouseObjB->Flags & 0x01 ) return;
        switch( flg ){
            case 1: TileUpdateArea( &Area1, gCurrentMapLvl ); break;
            case 2: TileUpdateArea( &Area2, gCurrentMapLvl ); break;
            case 3: RegionExpand( &Area1, &Area2, &Area1 ); break;
        }
    } else {
	ObjSetShape( gGmouseObjA, NewImg, NULL );
    }
}

int GmouseUnk56()
{
    if( gGmouseInited ) return gGmouseObjA->ImgId;
    return -2;
}

void GmouseSetHexCursorBlank()
{
    GmouseSetShapeA( CURSOR_BLANK );
}

void GmouseIsoEnter()
{
    VidRect_t Area1, Area2, Area3;
    int flg;

    if( !gGmouseInited ) return;    
    flg = 0;
    if( ObjUnk32( gGmouseObjA, &Area2 ) == 0 ) flg = 0x01;
    if( ObjUnk32( gGmouseObjB, &Area3 ) == 0 ) flg |= 0x02;
    if( gGmouseShape && !ObjUnk35( gGmouseObjB, &Area1 ) ){
        if( flg & 0x02 )
            RegionExpand( &Area3, &Area1, &Area3 );
        else
            Area3 = Area1;
    }
    if( !GmouseSetShapeB( &Area1 ) ){
        if( flg & 0x02 ){
            RegionExpand( &Area3, &Area1, &Area3 );
        } else {
            flg |= 0x02;
            Area3 = Area1;
        }
    }
    if( !(flg & 0x02) ){
        if( flg == 1 ){
	    TileUpdateArea( &Area2, gCurrentMapLvl );
	}
    } else {
        if( flg != 3 ){
	    TileUpdateArea(  &Area3, gCurrentMapLvl );
        } else {
    	    RegionExpand( &Area2, &Area3, &Area2 );
	    TileUpdateArea( &Area2, gCurrentMapLvl );
	}
    }
    gGmouseStay = 0;
    gGmouseRstTime = TimerGetSysTime() - 250;    
}

void GmouseUnk03()
{
    unsigned int flg;
    VidRect_t Area1, Area2;

    if( !gGmouseInited ) return;    
    flg = 0;
    if( !ObjUnk33( gGmouseObjA, &Area2 ) ) flg = 0x01;
    if( !ObjUnk33( gGmouseObjB, &Area1 ) ) flg |= 0x02;
    switch( flg ){    
        case 2: TileUpdateArea( & Area1, gCurrentMapLvl ); break;
        case 3: RegionExpand( &Area2, &Area1, &Area2 ); TileUpdateArea( &Area2, gCurrentMapLvl ); break;
	case 1: TileUpdateArea( &Area2, gCurrentMapLvl ); break;
    }
}

int GmouseUnk58()
{
    return (gGmouseObjB->Flags & 0x01) == 0;
}

Obj_t *GmouseGetObject( int ObjType, int Flg, int MapLvl )
{
    ObjTable_t *v8, *stack;
    Obj_t *obj;
    int cnt,i,y,x,flg;

    flg = 0;
    obj = NULL;
    MseGetCursorPosition( &x, &y );
    if( ObjType == -1 && TilePointOpaque( x, y, MapLvl ) && !ObjInteraction( gObjRadius, x, y ) ) flg = 1;
    if( flg ) return NULL;
    cnt = ObjTabCreate( x, y, MapLvl, ObjType, &stack );
    v8 = &stack[ cnt - 1 ];
    for( i = cnt - 1; i >= 0; i--, v8-- ){
        if( !Flg && gObjDude == v8->obj ) continue;        
        obj = v8->obj;
        if( (v8->Flags & 0x01) && !(v8->Flags & 0x04) && (OBJTYPE( obj->ImgId ) != TYPE_CRIT || (obj->Scenery.i07 & 0x81) == 0) ) break;
    }
    if( cnt ) ObjTabDestroy( &stack );
    return obj;
}

int GmouseTile( int x0, int y0, int CursorType, int a4, int a5)
{
    ArtFrmHdr_t *Img1, *Img2;
    int v14,v18,ImgHeight2,ImgHeight1,ImgWidth1,v44,ImgWidth2;
    char *v15;
    CachePool_t *ImgObj2, *ImgObj1;
    void *ImgPix2, *pDst, *ImgPix1;

    if( !(Img1 = ArtLoadImg( ArtMakeId( 6, gGmouseUnk29[ CursorType ], 0, 0, 0 ), &ImgObj1 ) ) ) return -1;
    if( !(Img2 = ArtLoadImg( ArtMakeId( 6, gGmouseShapeIds[1], 0, 0, 0 ), &ImgObj2 ) ) ){ ArtClose( ImgObj1 ); return 0; }
    ImgPix1 = ArtGetObjData( Img2, 0, 0 );
    ImgWidth1 = ArtGetObjWidth( Img2, 0, 0 );
    ImgHeight1 = ArtGetObjHeight( Img2, 0, 0 );

    ImgPix2 = ArtGetObjData( Img1, 0, 0 );
    ImgWidth2 = ArtGetObjWidth( Img1, 0, 0 );
    ImgHeight2 = ArtGetObjHeight( Img1, 0, 0 );

    v14 = x0 + ImgWidth2 + ImgWidth1 - 1;
    v44 = y0 + ImgHeight2 - 1;
    v15 = gGmouseDataImgB;
    pDst = gGmouseDataImgB;

    gGmouseMidX = gGmouseMidY = 0;
    gGmouseImgB->PixShiftX[ 0 ] = gGmouseWidthImgB / 2;
    gGmouseImgB->PixShiftY[ 0 ] = gGmouseHeightImgB - 1;
    v18 = v44 - a5 + 2;
    if( v14 < a4 ){
        pDst = &v15[ ImgWidth1 ];
        if( a5 <= v44 ){
            gGmouseMidY = v18;
            gGmouseImgB->PixShiftY[ 0 ] -= v18;
            v15 += gGmouseWidthImgB * v18;
        }
    } else {
        ArtClose( ImgObj2 );
        ImgPix1 = ArtGetObjData( ArtLoadImg( ArtMakeId( 6, 285, 0, 0, 0 ), &ImgObj2 ), 0, 0 );
        v15 += ImgWidth2;
        gGmouseImgB->PixShiftX[ 0 ] = -gGmouseImgB->PixShiftX[ 0 ];
        gGmouseMidX += ImgWidth2 + ImgWidth1;
        if( v44 >= a5 ){
            gGmouseMidY += v18;
            gGmouseImgB->PixShiftY[ 0 ] = gGmouseImgB->PixShiftY[ 0 ] - v18;
            v15 += gGmouseWidthImgB * v18;
        }
    }
    memset( gGmouseDataImgB, 0, gGmouseSizeImgB );
    ScrCopy( ImgPix1, ImgWidth1, ImgHeight1, ImgWidth1, v15, gGmouseWidthImgB );
    ScrCopy( ImgPix2, ImgWidth2, ImgHeight2, ImgWidth2, pDst, gGmouseWidthImgB );
    ArtClose( ImgObj2 );
    ArtClose( ImgObj1 );
    return 0;
}

void GmouseGetCenter( int *pXmid, int *pYmid )
{
    *pXmid = gGmouseMidX;
    *pYmid = gGmouseMidY;
}

int GmouseCursorMenuCreate( int xpos, int ypos, int *IconsArray, int IconsCount, int xmax, int ymax )
{
    Sound_t *Acm;
    CachePool_t *ImgObjs[ 10 ], *CurObj;
    ArtFrmHdr_t *Imgs[ 10 ], *ImgCur;
    int i, Height2, var_4, Width2, Height1, Width1, var_1, var_6;
    char *p, *q, *pDst;

    gGmouseUnk31 = NULL;
    gGmouseUnk30 = 0;
    gGmouseUnk32 = 0;
    if( !IconsArray || !IconsCount || IconsCount >= 10 ) return -1;
    Imgs[ 0 ] = ArtLoadImg( ArtMakeId( 6, gGmouseUnk29[ IconsArray[ 0 ] ] - 1, 0, 0, 0 ), &ImgObjs[ 0 ] );
    if( !Imgs[0] ) return -1;
    for( i = 1; i < IconsCount; i++ ){
        if( !( Imgs[ i ] = ArtLoadImg( ArtMakeId( 6, gGmouseUnk29[ IconsArray[ i ] ], 0, 0, 0 ), &ImgObjs[ i ] ) ) ){
            while( 1 ){
                i--;
                if( i < 0 ) break;
                ArtClose( ImgObjs[ i ] );
            }
            return -1;
        }
    }
    if( !( ImgCur = ArtLoadImg( ArtMakeId( 6, gGmouseShapeIds[1], 0, 0, 0 ), &CurObj ) ) ) return -1;
    Width1 = ArtGetObjWidth( ImgCur, 0, 0 );
    Height1 = ArtGetObjHeight( ImgCur, 0, 0 );
    Width2 = ArtGetObjWidth( Imgs[ 0 ], 0, 0 );
    Height2 = ArtGetObjHeight( Imgs[ 0 ], 0, 0 );
    var_1 = IconsCount * Height2 + ypos - 1;
    gGmouseUnk55 = 0;
    gGmouseUnk54 = 0;
    gGmouseImgA->PixShiftX[ 0 ] = gGmouseWidthImgA / 2;
    p = gGmouseDataImgA;
    gGmouseImgA->PixShiftY[ 0 ] = gGmouseHeightImgA - 1;
    pDst = gGmouseDataImgA;
    var_4 = var_1 - ymax + 2;
    if( (Width2 + Width1 + xpos - 1) < xmax ){
        q = (char *)ArtGetObjData( ImgCur, 0, 0 );
        pDst = gGmouseDataImgA + Width1;
        if( ymax <= var_1 ){
            gGmouseUnk54 += var_4;
            p += gGmouseWidthImgA * var_4;
            gGmouseImgA->PixShiftY[0] -= var_4;
        }
    } else {
        ArtClose( CurObj );
        q = (char *)ArtGetObjData( ArtLoadImg( ArtMakeId( 6, 285, 0, 0, 0 ), &CurObj ), 0, 0 );
        p += Width2;
        gGmouseImgA->PixShiftX[ 0 ] = -gGmouseImgA->PixShiftX[ 0 ];
        gGmouseUnk55 = Width2 + Width1 + gGmouseUnk55;
        if( var_1 >= ymax ){
            gGmouseUnk54 = var_4 + gGmouseUnk54;
            gGmouseImgA->PixShiftY[ 0 ] -= var_4;
            p += gGmouseWidthImgA * var_4;
        }
    }
    memset( gGmouseDataImgA, 0, gGmouseSizeImgA );
    ScrCopy( q, Width1, Height1, Width1, p, gGmouseWidthImgB );
    p = pDst;
    var_6 = Height2 * gGmouseWidthImgA;
    for( i = 0; i < IconsCount; i++, p += var_6 ){
        ScrCopy( ArtGetObjData( Imgs[ i ], 0, 0 ), Width2, Height2, Width2, p, gGmouseWidthImgA );	
    }
    ArtClose( CurObj );
    for( i = 0; i < IconsCount; i++  ) ArtClose( ImgObjs[ i ] );    
    memcpy( gGmouseUnk61, IconsArray, IconsCount * sizeof( int ) );
    gGmouseUnk32 = IconsCount;
    gGmouseUnk31 = pDst;
    if( (Acm = GSoundLoadAcm( "iaccuxx1", 0 )) ) GSoundDbgPlayB( Acm );
    return 0;
}

int GmouseUnk60( int *a1, int *a2 )
{
    *a1 = gGmouseUnk55;
    *a2 = gGmouseUnk54;
    return 0;
}

int GmouseMenuIconHL( int a1 )
{
    ArtFrmHdr_t *Img;
    int ObjWidth, ObjHeight;
    CachePool_t *ImgObj;

    if( a1 < 0 ) return -1;
    if( gGmouseUnk32 <= a1 ) return -1;
    if( !(Img = ArtLoadImg( ArtMakeId( 6, gGmouseUnk29[ gGmouseUnk61[ (int)gGmouseUnk30 ] ], 0, 0, 0 ), &ImgObj )) ) return -1;
    ObjWidth = ArtGetObjWidth( Img, 0, 0 );
    ObjHeight = ArtGetObjHeight( Img, 0, 0 );
    ScrCopy( ArtGetObjData( Img, 0, 0 ), ObjWidth, ObjHeight, ObjWidth, gGmouseWidthImgA * ObjHeight * gGmouseUnk30 + gGmouseUnk31, gGmouseWidthImgA );
    ArtClose( ImgObj );
    
    if( !(Img = ArtLoadImg( ArtMakeId( 6, gGmouseUnk29[gGmouseUnk61[a1]] - 1, 0, 0, 0 ), &ImgObj )) ) return -1;
    ScrCopy( ArtGetObjData( Img, 0, 0), ObjWidth, ObjHeight, ObjWidth, gGmouseUnk31 + gGmouseWidthImgA * ObjHeight * a1, gGmouseWidthImgA );
    ArtClose( ImgObj );
    gGmouseUnk30 = a1;
    return 0;
}

int GmousePrint( char *str, int Color ) 
{
    CachePool_t *ImgObj;
    ArtFrmHdr_t *art;
    int w, SaveFont;

    if( !( art = ArtLoadImg( ArtMakeId( 6, gGmouseUnk26[ 0 ], 0, 0, 0 ), &ImgObj ) ) ) return -1;
    memset( gGmouseDataImgC, 0, gGmouseSizeImgC );
    w = ArtGetObjWidth( art, 0, 0 );
    ScrCopy( ArtGetObjData( art, 0, 0 ), w, ArtGetObjHeight( art, 0, 0 ), w, gGmouseDataImgC, gGmouseWidthImgC );

    SaveFont = FontGetCurrent();
    FontSet( 101 );
    gFont.Print( gGmouseDataImgC + gGmouseWidthImgC + w + 1, str, gGmouseWidthImgC - w, gGmouseWidthImgC, Color );
    ScrEdgeHighlight( gGmouseDataImgC + w, gGmouseWidthImgC - w, gGmouseHeightImgC, gGmouseWidthImgC, gPalColorCubeRGB[0][0][0] );
    FontSet( SaveFont );
    ArtClose( ImgObj );
    return 0;
}

int GmouseText( char *str, int Color ) // cursor text '','X', 'AP_num'
{
    int Font;

    memset( gGmouseDataImgE, 0, gGmouseWidthImgE * gGmouseHeightImgE );
    if( *str == '\0' ) return 0;
    Font = FontGetCurrent();
    FontSet( 101 );
    gFont.Print( 
	gGmouseDataImgE + (gGmouseWidthImgE - gFont.LineWidth( str ) ) / 2 + ((gGmouseHeightImgE - gFont.ChrHeight()) / 2) * gGmouseWidthImgE, 
	str, gGmouseWidthImgE, gGmouseWidthImgE, Color 
    );
    ScrEdgeHighlight( gGmouseDataImgE, gGmouseWidthImgE, gGmouseHeightImgE, gGmouseWidthImgE, gPalColorCubeRGB[0][0][0]);
    FontSet( Font );
    GmouseSetShapeA( ArtMakeId( 6, 1, 0, 0, 0 ) );    
    return 0;
}

void GmouseHLcfg()
{
    int tmp;
    if( CfgGetInteger( &gConfiguration, "preferences", "item_highlight", &tmp ) == 1 ) gItemHlShow = tmp;
}

int GmouseInitA()
{
    VidRect_t tmp;
    int x, y;

    if( gGmouseUnk13 ) return -1;
    if( ObjCreate( &gGmouseObjA, CURSOR_BLANK, -1 ) ) return -1;
    if( ObjCreate( &gGmouseObjB, ArtMakeId( 6, 1, 0, 0, 0 ), -1 ) ) return -1;
    if( ObjSetOutline( gGmouseObjB, 0x40000002, NULL ) ) return -1;
    if( GmouseLoadImages() ) return -1;
    gGmouseObjA->Flags |= 0x00000004;
    gGmouseObjA->Flags |= 0x20000000; //PRFLG_LIGHTTHROU
    gGmouseObjA->Flags |= 0x00000400;
    gGmouseObjA->Flags |= 0x80000000;
    gGmouseObjA->Flags |= 0x00000010;

    gGmouseObjB->Flags |= 0x00000400;
    gGmouseObjB->Flags |= 0x00000004;
    gGmouseObjB->Flags |= 0x20000000; //PRFLG_LIGHTTHROU
    gGmouseObjB->Flags |= 0x80000000;
    gGmouseObjB->Flags |= 0x00000010;
    ObjSetPlayer( gGmouseObjB, 0 );
    MseGetCursorPosition( &x, &y );
    GmouseCursorUpdate( x, y, gCurrentMapLvl, &tmp );
    gGmouseUnk13 = 1;
    GmouseHLcfg();
    return 0;
}

int GmouseUnk62()
{
    if( !gGmouseUnk13 ) return -1;
    gGmouseUnk23 = 1;
    GmouseSetShapeA( CURSOR_BLANK );
    GmouseSetMode( 0 );
    GmouseIsoEnter();
    gGmouseOldX = -1;
    gGmouseOldY = -1;
    gGmouseStay = 0;
    gGmouseRstTime = TimerGetSysTime();
    GmouseHLcfg();
    return 0;
}

void GmouseDestroyCursors( )
{
    if( !gGmouseUnk13 ) return;
    GmouseCloseImages();
    gGmouseObjA->Flags &= ~0x400;
    gGmouseObjB->Flags &= ~0x400;
    ObjDestroy( gGmouseObjA, NULL );
    ObjDestroy( gGmouseObjB, NULL );
    gGmouseUnk13 = 0;
}

int GmouseLoadImages()
{
    if( (gGmouseImgA = ArtLoadImg( ArtMakeId(6, 283, 0, 0, 0), &gGmouseImgObjA )) ){
        if( (gGmouseImgB = ArtLoadImg( ArtMakeId(6, 282, 0, 0, 0), &gGmouseImgObjB )) ){
            if( (gGmouseImgC = ArtLoadImg(ArtMakeId(6, 284, 0, 0, 0), &gGmouseImgObjC )) ){
                if( (gGmouseImgD = ArtLoadImg( CURSOR_BLANK, &gGmouseImgObjD )) ){
                    if( ( gGmouseImgE = ArtLoadImg( ArtMakeId(6, 1, 0, 0, 0), &gGmouseImgObjE ) ) ){
                        gGmouseWidthImgA = ArtGetObjWidth( gGmouseImgA, 0, 0);
                        gGmouseHeightImgA = ArtGetObjHeight( gGmouseImgA, 0, 0);
                        gGmouseSizeImgA = gGmouseWidthImgA * gGmouseHeightImgA;
                        gGmouseDataImgA = (char *)ArtGetObjData( gGmouseImgA, 0, 0);

                        gGmouseWidthImgB = ArtGetObjWidth( gGmouseImgB, 0, 0);
                        gGmouseHeightImgB = ArtGetObjHeight( gGmouseImgB, 0, 0);
                        gGmouseSizeImgB = gGmouseWidthImgB * gGmouseHeightImgB;                        
                        gGmouseDataImgB = (char *)ArtGetObjData( gGmouseImgB, 0, 0);

                        gGmouseWidthImgC = ArtGetObjWidth( gGmouseImgC, 0, 0);
                        gGmouseHeightImgC = ArtGetObjHeight( gGmouseImgC, 0, 0);
                        gGmouseSizeImgC = gGmouseWidthImgC * gGmouseHeightImgC;                        
                        gGmouseDataImgC = (char *)ArtGetObjData( gGmouseImgC, 0, 0);

                        gGmouseWidthImgD = ArtGetObjWidth( gGmouseImgD, 0, 0);
                        gGmouseHeightImgD = ArtGetObjHeight( gGmouseImgD, 0, 0);
                        gGmouseSizeImgD = gGmouseWidthImgD * gGmouseHeightImgD;                        
                        gGmouseDataImgD = (char *)ArtGetObjData( gGmouseImgD, 0, 0);

                        gGmouseWidthImgE = ArtGetObjWidth( gGmouseImgE, 0, 0);
                        gGmouseHeightImgE = ArtGetObjHeight( gGmouseImgE, 0, 0);
                        gGmouseSizeImgE = gGmouseWidthImgE * gGmouseHeightImgE;                        
                        gGmouseDataImgE = (char *)ArtGetObjData( gGmouseImgE, 0, 0);
                        return 0;
                    }
                    ArtClose(gGmouseImgObjD);
                    gGmouseImgObjD = ART_NULL;
                    gGmouseImgD = NULL;
                }
                ArtClose( gGmouseImgObjC );
                gGmouseImgObjC = ART_NULL;
                gGmouseImgC = NULL;
            }
            ArtClose( gGmouseImgObjB );
            gGmouseImgObjB = ART_NULL;
            gGmouseImgB = NULL;
        }
        ArtClose( gGmouseImgObjA );
        gGmouseImgA = NULL;
        gGmouseImgObjA = ART_NULL;
    }
    return -1;
}

void GmouseCloseImages()
{
    if( gGmouseImgObjD != ART_NULL ) ArtClose( gGmouseImgObjD );
    gGmouseImgD = NULL;
    gGmouseImgObjD = ART_NULL;
    if( gGmouseImgObjE != ART_NULL ) ArtClose( gGmouseImgObjE );
    gGmouseImgE = NULL;
    gGmouseImgObjE = ART_NULL;
    if( gGmouseImgObjC != ART_NULL ) ArtClose( gGmouseImgObjC );
    gGmouseImgC = NULL;
    gGmouseImgObjC = ART_NULL;
    if( gGmouseImgObjA != ART_NULL ) ArtClose( gGmouseImgObjA );
    gGmouseImgA = NULL;
    gGmouseImgObjA = ART_NULL;
    if( gGmouseImgObjB != ART_NULL ) ArtClose( gGmouseImgObjB );
    gGmouseImgB = NULL;
    gGmouseDataImgB = NULL;
    gGmouseWidthImgB = 0;
    gGmouseHeightImgB = 0;
    gGmouseSizeImgB = 0;
    gGmouseImgObjB = ART_NULL;
}

int GmouseUnk65( int a1, VidRect_t *a2 )
{
    return (ObjSetShape( gGmouseObjB, a1, a2 ) == 0 ) - 1;
}

int GmouseSetShapeB( VidRect_t *Area )
{
    int Id;

    if( (Id = ArtMakeId( 6, gGmouseShapeIds[ gGmouseShape ], 0, 0, 0 ) ) == gGmouseObjB->ImgId ) return -1;
    return ( ObjSetShape( gGmouseObjB, Id, Area ) == 0 ) - 1;
}

int GmouseCursorUpdate( int x, int y, int lvl, VidRect_t *Area )
{
    ArtFrmHdr_t *Img, *v13, *Data;
    VidRect_t Area1, Rect, Area2, AreaOut;
    CachePool_t *ImgObj, *Block;
    char *pValue;
    int Pointed, GridIdx, pY, pX, v27, a3, v30, pXshift, flg, err, a2, v38, GridPos, v40, xoffs, yoffs;

    err = -1;
    if( gGmouseUnk10 ){
        a2 = 0; v38 = 0; v40 = 0;
        if( OBJTYPE( gGmouseObjA->ImgId ) == TYPE_TILE ){
            GridIdx = TileGetGridIdx( x, y );
            GridPos = GridIdx;
            if( GridIdx != -1 ){
                GridPos = 200 * (2 * (GridIdx / 100) + 1) + 2 * (GridIdx % 100) + 1;
                v38 = 13; a2 = -8;
                CfgGetString( &gConfiguration, "system", "executable", &pValue );
                if( !strcasecmp( pValue, "mapper" ) && TileUnk10() && ( gObjDude->Flags & 1 ) ) v38 = -83;
            }
        } else {
            GridPos = TileGetPointed( x, y );
        }
        if( GridPos == -1 ) return -1;
        if( !ObjMoveToTile( gGmouseObjA, GridPos, lvl, &AreaOut ) ){
            if( (a2 || v38) && !ObjMove( gGmouseObjA, a2, v38, &Area2 ) ) RegionExpand( &AreaOut, &Area2, &AreaOut );
            v40 = 1;
        }
        if( gGmouseShape ){
            a3 = 0; v27 = 0;
            v13 = ArtLoadImg( gGmouseObjB->ImgId, &Block );
            Data = v13;
            if( v13 ){
                ArtGetCenter( v13, 0, &a3, &v27 );
                ArtGetFrameShift( Data, 0, 0, &pXshift, &v30 );
                a3 += pXshift;
                v27 += v30;
                ArtClose( Block );
            }
             if( !ObjPutCursor( gGmouseObjB, x + a3, y + v27, lvl, &Area2 ) ){
        	if( v40 ){
            	    RegionExpand( &AreaOut, &Area2, &AreaOut );
        	} else {
        	    AreaOut = Area2;
        	    v40 = 1;
        	}
            }
        } else {
            if( !ObjMoveToTile( gGmouseObjB, GridPos, lvl, &Area2 ) ){
        	if( v40 ){
        	    RegionExpand( &AreaOut, &Area2, &AreaOut );
        	} else {
        	    v40 = 1;
        	    AreaOut = Area2;
        	}
            }
        }
        if( v40 ){
            err = 0;
            memcpy( Area, &Area, sizeof( VidRect_t ) );
        }
        return err;
    }
    if( gGmouseShape ){ // arrow cursor, linear movement
        pX = pY = 0;        
        if( (Img = ArtLoadImg( gGmouseObjB->ImgId, &ImgObj )) ){
            ArtGetCenter( Img, 0, &pX, &pY );
            ArtGetFrameShift( Img, 0, 0, &xoffs, &yoffs );
            pX += xoffs;
            pY += yoffs;
            ArtClose( ImgObj );
        }
        ObjPutCursor( gGmouseObjB, x + pX, y + pY, lvl, Area );
    } else { // hex cursor, hex jump movement
	flg = 0;
	if( (Pointed = TileGetPointed( x, y )) == -1 ) return 0;    
	if( !TileGetScrCoordinates( Pointed, &pX, &pY ) ){ // erase previous cursor
	    if( !ObjPutCursor( gGmouseObjA, pX + 16, pY + 15, 0, &Rect ) ) flg = 1;
	}
	if( ObjMoveToTile( gGmouseObjB, Pointed, lvl, &Area1 ) ) return 0;    
	if( flg )
    	    RegionExpand( &Rect, &Area1, &Rect );
	else
    	    Rect = Area1;    
	memcpy( Area, &Rect, sizeof( VidRect_t ) );
    }
    return 0;
}

int GmouseViewMove( int MseX, int MseY )
{
    int dir, CursorShape, allow, dx, dy;

    if( !gGmouseMoveAllow ) return -1;
    dir = dx = dy = 0;
    if( MseX <= gVidMainGeo.lt ) dir = 0x01;	// LT
    if( MseX >= gVidMainGeo.rt ) dir |= 0x02;	// RT
    if( MseY <= gVidMainGeo.tp ) dir |= 0x04;	// UP
    if( MseY >= gVidMainGeo.bm ) dir |= 0x08;	// DN

    switch( dir ){
        case 1:  dx = -1; dy = 0;  CursorShape = 11; break; // LT
        case 2:  dx = 1;  dy = 0;  CursorShape = 7;  break; // RT
        case 4:  dx = 0;  dy = -1; CursorShape = 5;  break; // UP
        case 5:  dx = -1; dy = -1; CursorShape = 4;  break; // LT, UP
        case 6:  dx = 1;  dy = -1; CursorShape = 6;  break; // RT, UP
        case 8:  dx = 0;  dy = 1;  CursorShape = 9;  break; // DN
        case 9:  dx = -1; dy = 1;  CursorShape = 10; break; // LT, DN
        case 10: dx = 1;  dy = 1;  CursorShape = 8;  break; // RT, DN
        default:  break;
    }
    if( !(dx | dy) ) return -1;
    if( !gGmouseUnk11 || gGmouseUnk75 ){
        allow = MapMove( dx, dy );
        if( allow == 0 ){ GmouseLoadCursor( CursorShape ); return 0; } // move possible cursor
        if( allow == -1 ){ GmouseLoadCursor( CursorShape + 8 ); return 0; } // move disabled cursor
    }    
    return 0;
}

void GmouseUnk66( Obj_t *result )
{
    VidRect_t rect;

    if( !gGmouseObjC || result != gGmouseObjC ) return;
    if( !ObjGetRadius( result, &rect ) ) TileUpdateArea( &rect, gCurrentMapLvl );
    gGmouseObjC = NULL;    
}

int GmouseUnk43( Obj_t *scen )
{
    Proto_t *proto;

    return scen && (OBJTYPE( scen->Pid ) == TYPE_SCEN) && (ProtoGetObj( scen->Pid, &proto ) != -1) && !proto->Critt.Type;
}

