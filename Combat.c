#include "FrameWork.h"

int gCombat00 = 0;
int gCombat01 = 0;
int gCombatStatus = 2;
Combat01_t *gCombat03 = NULL;
Combat02_t *gCombat07 = NULL;
int gCombat08 = 0;
int gCombatHitPenalty[ 9 ] = { -0x28, -0x1E, -0x1E, 0x00, -0x14, -0x14, -0x3C, -0x1E, 0x00 };
#include "CombatTable.c"
int gCombatTacticMode = 0;
int gCombat12 = 0;
int gCombatUnk100[ 30 ] = {
    0x000000, 0x008000, 0x008000, 0x080002, 0x200000, 
    0x000000, 0x008000, 0x004000, 0x100000, 0x010000, 
    0x000000, 0x020000, 0x004000, 0x100000, 0x002000, 
    0x008000, 0x028000, 0x00C000, 0x100000, 0x009000, 
    0x040000, 0x004000, 0x084000, 0x100000, 0x001000, 
    0x008000, 0x040000, 0x002000, 0x100000, 0x009002
};

int gCombatUnk12[ 4 ] = { 122, 188, 251, 316 };
int gCombatBodyParts[ 8 ] = { 0, 6, 2, 4, 3, 7, 1, 5 };
/**/
Combat_t gCombat20;
Msg_t gCombatMsg;
Obj_t *gCombatTarget;
int gCombatFocusWin;
int gCombatMapLvl;
int gCombatCritCnt;
Obj_t *gCombat10;
int gCombat05;
Obj_t *gCombat06;
/**/
Obj_t *gCombat19;
int gTargetHighlightLvl;
Obj_t **gCombatCritters;
int gCombat04;
int gCombat21;
int gCombatMovePts;
Combat_t gCombatUnk09;
Combat_t gCombatExplosion;

/************************************/
int CombatInit()
{
    char path[ 260 ];

    gCombat00 = 0;
    gCombat01 = 0;
    gCombatCritters = NULL;
    gCombat03 = 0;
    gCombat04 = 0;
    gCombat05 = 0;
    gCombatCritCnt = 0;
    gCombat07 = NULL;
    gCombat08 = 0;
    gCombatStatus = 2;
    gCombatMovePts = 0;
    gCombat10 = 0;
    gCombatTacticMode = 0;
    gObjDude->Critter.State.CurrentAP = FeatGetVal( gObjDude, FEAT_AP );
    gCombat12 = 0;
    if( MessageInit( &gCombatMsg ) != 1 ) return -1;
    sprintf( path, "%s%s", gGamePath, "combat.msg" );
    if( MessageLoad( &gCombatMsg, path ) != 1 ) return -1;
    return 0;
}

int CombatReset()
{
    gCombat00 = 0;
    gCombat01 = 0;
    gCombatCritters = NULL;
    gCombat03 = 0;
    gCombat04 = 0;
    gCombat05 = 0;
    gCombatCritCnt = 0;
    gCombat07 = NULL;
    gCombat08 = 0;
    gCombatStatus = 2;
    gCombatMovePts = 0;
    gCombat10 = 0;
    gObjDude->Critter.State.CurrentAP = FeatGetVal( gObjDude, FEAT_AP );
    return 0;
}

int CombatClose()
{
    MessageClose( &gCombatMsg );
    return 0;
}

int CombatLookForAgressor( int CritIdx, int WhoHitMe, Obj_t **CrittList, int CrittCnt )
{
    Obj_t **Crit;

    Crit = &CrittList[ CritIdx ];
    for(;CritIdx < CrittCnt; CritIdx++, Crit++ ){
        if( WhoHitMe == (*Crit)->CritterIdx ) break;            
    }
    return CritIdx;
}

int CombatLoad( xFile_t *fh )
{
    int j, tmp, i;
    Obj_t *p, *obj;

    if( dbgetBei(fh, &gCombatStatus) == -1 ) return -1; // load combat status
    if( !(gCombatStatus & CBT_IN_COMBAT ) ){
        for( p = ObjGetObjectFirst(); p; p = ObjGetObjectNext() ){
            if( OBJTYPE( p->Pid ) != TYPE_CRIT ) continue;
            if( p->Critter.State.WhoHitMeObj == ART_NULL ) p->Critter.State.WhoHitMeObj = NULL;
        }
        return 0;
    }

    if( dbgetBei( fh, &gCombat00 ) == -1 ) return -1; // ?
    if( dbgetBei( fh, &gCombatMovePts ) == -1 ) return -1; // ?
    if( dbgetBei( fh, &gCombat21 ) == -1 ) return -1; // ?
    if( dbgetBei( fh, &gCombat04 ) == -1 ) return -1; // ?
    if( dbgetBei( fh, &gCombat05 ) == -1 ) return -1; // ?
    if( dbgetBei( fh, &gCombatCritCnt ) == -1 ) return -1; // number of NPC at the current map level + player + party at the beginning of the battle ( and corpses ?)
    if( ObjGetObjList( -1, gCurrentMapLvl, 1, &gCombatCritters ) != gCombatCritCnt ){ // get all critters list
        ObjCritterListDestroy( gCombatCritters );
	return -1;
    }
    if( dbgetBei( fh, &gObjDude->CritterIdx ) == -1 ) return -1;  // Player sequence number
    for( i = 0; i < gCombatCritCnt; i++ ){
        if( gCombatCritters[ i ]->Critter.State.WhoHitMeObj == ART_NULL ){
            gCombatCritters[ i ]->Critter.State.WhoHitMeObj = NULL;
        } else {
            for( j = 0; j < gCombatCritCnt; j++ ){
                if( gCombatCritters[ i ]->Critter.State.WhoHitMe == gCombatCritters[ j ]->CritterIdx ) break;
            }
            gCombatCritters[ i ]->Critter.State.WhoHitMeObj = ( j == gCombatCritCnt ) ? NULL : gCombatCritters[ j ];
        }
    }
    // turn order
    for( i = 0; i < gCombatCritCnt; i++ ){
        if( dbgetBei( fh, &tmp ) == -1 ) return -1;
        for( j = i ;j < gCombatCritCnt; j++ ){
            if( tmp == gCombatCritters[ i ]->CritterIdx ) break;                    
        }            
        if( j == gCombatCritCnt ) return -1;
        obj = gCombatCritters[ i ];
        gCombatCritters[ i ] = gCombatCritters[ j ];
        gCombatCritters[ j ] = obj;
    }
    
    for( i = 0; i < gCombatCritCnt; i++ ){
        gCombatCritters[ i ]->CritterIdx = i;
    }
    if( gCombat03 ) Free( gCombat03 );
    
    if( !(gCombat03 = Malloc( gCombatCritCnt * sizeof( Combat01_t ) )) ) return -1;    
    for( i = 0; i < gCombatCritCnt; i++ ){
        if( dbgetBei( fh, &tmp ) == -1 ) return -1;
        if( tmp == -1 ){
            gCombat03[ i ].i01 = 0;
        } else {
            if( !(gCombat03[ i ].i01 = ObjUnk43( tmp )) ) return -1;
        }
        if( dbgetBei( fh, &tmp ) == -1 ) return -1;
        if( tmp == -1 ){
            gCombat03[ i ].i02 = 0;
        } else {
            if( !(gCombat03[ i ].i02 = ObjUnk43( tmp )) ) return -1;
        }
        if( dbgetBei( fh, &tmp ) == -1 ) return -1;
        if( tmp == -1 ){
            gCombat03[ i ].i03 = 0;
        } else {
            if( !(gCombat03[ i ].i03 = ObjUnk43( tmp )) ) return -1;
        }
        if( dbgetBei( fh, &gCombat03[ i ].i04 ) == -1 ) return -1;
    }
    CombatTarget( gObjDude );
    return 0;
}

int CombatFSave( xFile_t *fh )
{
    int i;

    if( dbputBei( fh, gCombatStatus ) == -1 ) return -1;
    if( !IN_COMBAT== 0 ) return 0;
    if( dbputBei( fh, gCombat00 ) == -1 ) return -1;
    if( dbputBei( fh, gCombatMovePts ) == -1 ) return -1;
    if( dbputBei( fh, gCombat21 ) == -1 ) return -1;
    if( dbputBei( fh, gCombat04 ) == -1 ) return -1;
    if( dbputBei( fh, gCombat05 ) == -1 ) return -1;
    if( dbputBei( fh, gCombatCritCnt ) == -1 ) return -1;
    if( dbputBei( fh, gObjDude->CritterIdx ) == -1 ) return -1;    
    for( i = 0; i < gCombatCritCnt; i++ ){
    	if( dbputBei( fh, gCombatCritters[ i ]->CritterIdx ) == -1 ) return -1;
    }
    if( !gCombat03 ) return -1;
    for( i = 0; i < gCombatCritCnt; i++ ){
        if( dbputBei( fh, gCombat03[ i ].i01 ? gCombat03[ i ].i01->TimeEv : -1 ) == -1 ) return -1;
        if( dbputBei( fh, gCombat03[ i ].i02 ? gCombat03[ i ].i02->TimeEv : -1 ) == -1 ) return -1;
        if( dbputBei( fh, gCombat03[ i ].i03 ? gCombat03[ i ].i03->TimeEv : -1 ) == -1 ) return -1;
        if( dbputBei( fh, gCombat03[ i ].i04 ) == -1 ) return -1;
    }
    return 0;
}

int CombatUnk02( Obj_t *a1, Obj_t *a2, int a3, Obj_t *a4, int *steps )
{
    return CombatUnk03( a1, a2, a3, a4, steps, 0 );
}

int CombatUnk03( Obj_t *Critter, Obj_t *Weapon, int a3, Obj_t *a4, int *a5, Obj_t *a6 )
{
    Obj_t *obj;
    Combat_t cmbt;
    int v19,i,DmgMax,v31,v32,intel,v39,GroupId,v34;

    if( a5 ) *a5 = 0;
    if( Critter->Pid == 0x10001E0 ) return 0;
    intel = FeatGetVal( Critter, FEAT_INTELLIGENCE );
    GroupId = Critter->Critter.State.GroupId;
    v39 = Item69( Weapon, a3 );
    ItemGetWeaponDmg( Weapon, NULL, &DmgMax );
    v34 = ItemGetWeaponBase( Critter, Weapon );
    if( v39 > 0 ){
        if( intel < 5 ){
            v39 -= 5 - intel;
            if( v39 < 0 ) v39 = 0;
        }
        if( a6 ){
            if( ObjGetDistance( a4, a6 ) < v39 ){ eprintf( "Friendly was in the way!" ); return 1; }
        }
        for( i = 0; i < gCombatCritCnt; i++ ){
            obj = gCombatCritters[ i ];
            if( GroupId == obj->Critter.State.GroupId && obj != Critter && a4 != obj && !CritterIsDead( gCombatCritters[ i ] ) ){
                if( ObjGetDistance( a4, obj ) < v39 && obj != obj->Critter.State.WhoHitMeObj ){
                    if( FeatGetVal( obj, FEAT_DMG_RES( v34 ) ) * (DmgMax - FeatGetVal( obj, FEAT_DMG_THR( v34 ) )) / 100 > 0 ) return 1;
                }
            }            
        }            
        if( ObjGetDistance(a4, Critter) <= v39 ){
            if( !a5 ) return 1;
            *a5 = v39 - ObjGetDistance( a4, Critter ) + 1;
        }
    } else {
	v19 = Item45( Weapon, a3 );
	if( v19 != 46 && v19 != 47 ) return 0;
	CombatSetUp( &cmbt, Critter, a4, a3, 3 );
	CombatUnk40( &cmbt, CombatGetHitChance( Critter, Critter->GridId, a4, 3, a3, 1 ), &v31, &v32, v19 );
	if( a6 && cmbt.Count > 0 ){        
    	    for( i = 0; i < cmbt.Count; i++ ){
    		if( a6 == cmbt.obj[ i ]  ){ eprintf( "Friendly was in the way!" ); return 1; }
    	    }
	}
	for( i = 0; i < cmbt.Count; i++ ){
    	    obj = cmbt.obj[ i ];
    	    if( GroupId != obj->Critter.State.GroupId || obj == Critter || a4 == obj  ) continue;
    	    if( CritterIsDead( cmbt.obj[ i ] ) ) continue;
    	    if( obj == obj->Critter.State.WhoHitMeObj ) continue;
    	    if( FeatGetVal( obj, FEAT_DMG_THR( v34 ) ) * (DmgMax - FeatGetVal( obj, FEAT_DMG_RES( v34 ) ) ) / 100 > 0 ) return 1;
	}
    }
    return 0;    
}

int CombatUnk04( Obj_t *a1, Obj_t *a2, Obj_t *a3, Obj_t *a4 )
{
    return CombatUnk03( a1, a4, 2, a2, NULL, a3 );
}

Obj_t *CombatUnk05()
{
    if( IN_COMBAT ) return gCombat06;
    return 0;
}

void CombatUnk06( Obj_t *obj )
{
    obj->Critter.State.CombatResult = 0;
    obj->Critter.State.DmgLastTurn = 0;
}

void CombatUnk07()
{
    int i;

    for( i = 0; i < gCombatCritCnt; i++ ){
        gCombat03[ i ].i01 = 0;
        gCombat03[ i ].i02 = 0;
        gCombat03[ i ].i03 = 0;
        gCombat03[ i ].i04 = 0;
    }
}

void CombatUnk08( int a1, int a2 )
{
    gCombat03[ a2 ].i01 = gCombat03[ a1 ].i01;
    gCombat03[ a2 ].i02 = gCombat03[ a1 ].i02;
    gCombat03[ a2 ].i03 = gCombat03[ a1 ].i03;
    gCombat03[ a2 ].i04 = gCombat03[ a1 ].i04;
}

Obj_t *CombatUnk09( Obj_t *obj )
{
    if( !IN_COMBAT || !obj ) return 0;
    if( obj->CritterIdx == -1 ) return 0;
    return gCombat03[ obj->CritterIdx ].i01;    
}

int CombatUnk10( Obj_t *obj1, Obj_t *obj2 )
{
    if( !IN_COMBAT ) return 0;
    if( !obj1 ) return -1;
    if( obj1->CritterIdx == -1 || obj1 == obj2 ) return -1;
    gCombat03[ obj1->CritterIdx ].i01 = obj2;
    return 0;
}

Obj_t *CombatUnk11( Obj_t *obj )
{
    if( !IN_COMBAT || !obj ) return NULL;
    if( obj->CritterIdx == -1 ) return NULL;
    return gCombat03[ obj->CritterIdx ].i02;
}

int CombatUnk12( Obj_t *obj1, Obj_t *obj2 )
{
    if( !IN_COMBAT ) return 0;    
    if( !obj1 ) return -1;
    if( obj1->CritterIdx == -1 ) return -1;
    if( obj1 == obj2 ) return -1;
    if( CritterIsDead( obj2 ) ) obj2 = NULL;
    gCombat03[ obj1->CritterIdx ].i02 = obj2;
    return 0;
}

Obj_t *CombatUnk13( Obj_t *obj )
{
    if( !IN_COMBAT )  return NULL;
    if( !obj ) return NULL;    
    if( obj->CritterIdx == -1 ) return NULL;
    return gCombat03[ obj->CritterIdx ].i03;
}

int CombatUnk14( Obj_t *obj )
{
    if( !IN_COMBAT ) return 0;
    if( !obj ) return -1;
    if( obj->CritterIdx == -1 ) return -1;
    gCombat03[ obj->CritterIdx ].i03 = NULL;    
    return 0;
}

int CombatUnk15( Obj_t *obj )
{
    if( !IN_COMBAT ) return 0;
    if( !obj ) return -1;
    if( obj->CritterIdx == -1 ) return -1;
    return gCombat03[ obj->CritterIdx ].i04;
}

int CombatUnk16( Obj_t *obj, int arg )
{
    if( !IN_COMBAT ) return 0;    
    if( !obj ) return -1;
    if( obj->CritterIdx == -1 ) return -1;
    gCombat03[ obj->CritterIdx ].i04 = arg;
    return 0;
}

void CombatUnk17( Obj_t *a1 )
{
    Obj_t *v1, *obj;
    int i,id;

    v1 = NULL;
    gCombat00 = 0;
    AnimReset();
    InpTaskStop( AnimAmbient );
    gCombatMapLvl = gCurrentMapLvl;
    if( IN_COMBAT ) return;

    gCombat01 = 0;
    gCombat21 = 0;
    gCombatCritters = NULL;
    gCombatCritCnt = ObjGetObjList( -1, gCombatMapLvl, 1, &gCombatCritters );
    gCombat05 = gCombatCritCnt;
    gCombat04 = 0;
    gCombat03 = (Combat01_t *)Malloc( gCombatCritCnt * sizeof( Combat01_t ) );
    if( gCombat03 ){
        for( i = 0; i < gCombatCritCnt; i++ ){
            gCombat03[ i ].i01 = NULL;
            gCombat03[ i ].i02 = NULL;
            gCombat03[ i ].i03 = NULL;
            gCombat03[ i ].i04 = 0;
        }
        for( i = 0; i < gCombatCritCnt; i++ ){
            obj = gCombatCritters[ i ];
            obj->Critter.State.DmgLastTurn = 0;
            obj->Critter.State.WhoHitMe = 0;
            obj->Critter.State.Reaction &= 0x01;
            obj->Critter.State.CurrentAP = 0;
            obj->CritterIdx = i;
            if( (gCombatStatus & 0x100) != 0 && obj && i != -1 ) gCombat03[ i ].i04 = 0;
            ScptUnk138( obj->ScrId, 0, 0 );
            ScptSetArg( obj->ScrId, 0 );
            if( obj->Pid == 0x1000098 && !CritterIsDead( obj ) ) v1 = obj;
        }
        gCombatStatus |= 0x01;
        TileUpdate();
        GameIfaceDisable( 0 );
        GmouseLoadCursor( 26 );
        gCombat10 = 0;
        CombatTarget( a1 );
        AiUnk23( gCombatCritters, gCombatCritCnt );
        IfaceCombatOpen( 1 );
        GmouseScrollEnable();
        if( v1 && !LsgGetUnk02() ){
            id = ArtMakeId( (v1->ImgId & 0xF000000) >> 24, 100, (v1->ImgId & 0xFF0000u) >> 16, (v1->ImgId & 0xF000) >> 12, (v1->ImgId & 0x70000000) >> 28);
            AnimClear( v1 );
            AnimStart( 2 );
            AnimUnk48( v1, 6, -1 );
            AnimUnk62( v1, id, -1 );
            AnimBegin();
            while( AnimUnk39( v1 ) ) InpWinUpdate();
        }
    }    
}

int CombatTarget( Obj_t *obj )
{
    int i;

    for( i = 0; i < gCombatCritCnt; i++ ) {
        CombatUnk01( gCombatCritters[ i ], 0 );
    }
    CombatSetUp( &gCombat20, obj, 0, 4, 3 );
    gCombat19 = obj;
    AiUnk46( gCombatCritCnt, gCombatCritters );
    gTargetHighlightLvl = 2;
    return CfgGetInteger( &gConfiguration, "preferences", "target_highlight", &gTargetHighlightLvl );
}

void CombatUnk19( int Flg )
{
    int i;
    
    for( i = 0; i < gCombatCritCnt; i++ ){
        CombatUnk01( gCombatCritters[ i ], Flg );
    }
}

void CombatUnk01( Obj_t *obj, int Flg )
{
    int tmp, k, a, b;

    if( OBJTYPE( obj->Pid ) != TYPE_CRIT || obj == gObjDude ) return;
    if( CritterIsDead( obj ) ) return;
    if( CombatBlockedAim( gObjDude, gObjDude->GridId, obj->GridId, obj, 0 ) == 0 ){
	tmp = obj->OutlineColor & 0xFFFFFF;
        if( tmp != 1 && tmp != 8 ){
            ObjUnk35( obj, 0 );
            ObjGetRadius( obj, 0 );
            ObjSetOutline( obj, ( gObjDude->Critter.State.GroupId == obj->Critter.State.GroupId ) ? 8 : 1, 0 );
            if( Flg )
        	ObjUnk34( obj, 0 ); 
    	    else
    		ObjUnk35( obj, 0 );    	    
            return;
        }
        if( obj->OutlineColor && obj->OutlineColor >= 0 ){
            if( !Flg ) ObjUnk35( obj, 0 );
            return;
        }
        if( !Flg ) return;
    } else {
        a = ObjGetDistance( gObjDude, obj );
        b = 5 * FeatGetVal( gObjDude, FEAT_PERCEPTION );
        if( (obj->Flags & 0x20000) != 0 ) b /= 2;
        k = ( a <= b ) ? 1:0;
        if( ( obj->OutlineColor & 0xFFFFFF ) == 0x20 ){
            if( obj->OutlineColor && ( obj->OutlineColor >= 0 ) ){
                if( !Flg ) ObjUnk35( obj, 0 );
                return;
            }
            if( !Flg ) return;
        } else {
            ObjUnk35( obj, 0 );
            ObjGetRadius( obj, 0 );
            if( k ){
        	ObjSetOutline( obj, 32, 0 );
        	if( !Flg ) ObjUnk35( obj, 0 );
            }
            return;
        }
    }
    ObjUnk34( obj, 0 );
}

int CombatUnk20()
{
    Obj_t *obj;
    int i, Id, v14, v15;

    if( !gMenuEscape ){
        for( i = 0; i < gCombat04; i++ ){
            obj = gCombatCritters[ i ];
            if( obj != gObjDude ) AiUnk45( obj, 0 );
        }
    }
    InpTaskStart( AnimAmbient );    
    for( i = 0; i < (gCombat05 + gCombat04); i++ ){
        gCombatCritters[ i ]->Critter.State.DmgLastTurn = 0;
        gCombatCritters[ i ]->Critter.State.Reaction = 0;        
    }
    for( i = 0; i < gCombatCritCnt; i++ ){
        obj = gCombatCritters[ i ];
        obj->Critter.State.CurrentAP = 0;
        ObjGetRadius( obj, 0 );
        obj->Critter.State.WhoHitMe = 0;
        ScptUnk138( obj->ScrId, 0, 0 );
        ScptSetArg( obj->ScrId, 0 );
        if( obj->Pid == 0x1000098 && !CritterIsDead( obj ) && !LsgGetUnk02() ){
            Id = ArtMakeId( OBJTYPE( obj->ImgId ), 99, (obj->ImgId & 0xFF0000u) >> 16, (obj->ImgId & 0xF000) >> 12, (obj->ImgId & 0x70000000) >> 28);
            AnimClear( obj );
            AnimStart( 2 );
            AnimUnk48( obj, 6, -1 );
            AnimUnk62( obj, Id, -1 );
            AnimBegin();
            while( AnimUnk39( obj ) ) InpWinUpdate();
        }            
    }
    TileUpdate();
    IfaceUnk16( &v15, &v14 );
    IfaceHandSlotUpdate( 1, v15, v14 );
    gObjDude->Critter.State.CurrentAP = FeatGetVal( gObjDude, FEAT_AP );
    IfaceRenderAP( 0, 0 );
    if( !gMenuEscape ) CombatEarnExpPts( gCombat21 );
    gCombat21 = 0;
    gCombatStatus = (gCombatStatus & 0xFC) | 2;
    if( gCombatCritCnt ){
        ObjCritterListDestroy( gCombatCritters );
        if( gCombat03 ) Free( gCombat03 );
        gCombat03 = 0;
    }
    gCombatCritCnt = 0;
    AiUnk47();
    GameIfaceEnable();
    GmouseSetMode( 0 );
    IfaceResetAC( 1 );
    if( CritterUnk31( gObjDude ) ){
        if( !CritterIsDead( gObjDude ) && !gCombat10 ){
            EvQeDelB( gObjDude, 1 );
            return CritterUnk43( gObjDude );
        }
        return 0;
    }
    return 0;
}

void CombatUnk21()
{
    CombatUnk20();
    gCombatStatus = 0;
    gCombatTacticMode = 1;
}

void CombatEarnExpPts( int Exp )
{
    char stmp[ 132 ];
    MsgLine_t msg1, msg2;

    if( Exp <= 0 || CritterIsDead( gObjDude ) ) return;    
    FeatLvlUp( Exp );
    msg1.Id = 621; // '%s you earn %d exp. points.'
    if( MessageGetMsg( &gProtoMessages, &msg1 ) != 1 ) return;    
    msg2.Id = 622 + RandMinMax( 0, 3 ); // 'For [defeating/slaying/crushing/victorious] your enemies,'
    if( FeatGetVal( gObjDude, FEAT_35 ) == FeatGetVal( gObjDude, FEAT_HP ) && RandMinMax( 0, 100 ) > 65 ) msg2.Id = 626; // 'For destroying your enemies without taking a scratch,'
    if( MessageGetMsg( &gProtoMessages, &msg2 ) != 1 ) return;
    sprintf( stmp, msg1.Text, msg2.Text, Exp );
    IfcMsgOut( stmp );
}

void CombatUnk23()
{
    Obj_t *obj, *tmp;
    int ap, i;

    AiUnk64( gObjDude );
    for( i = gCombat04; i < (gCombat04 + gCombat05); i++ ){
        obj = gCombatCritters[ i ];
        if( AiUnk51( obj ) ){
            obj->Critter.State.Reaction = 0;
            tmp = gCombatCritters[ i ];
            gCombatCritters[ i ] = gCombatCritters[ gCombat04 ];
            gCombatCritters[ gCombat04 ] = tmp;
            gCombat04++;
            gCombat05--;
            ap = 0; // added -> bug, uninitialized ap value in esi !!!
            if( obj != gObjDude ) ap = FeatGetVal( obj, FEAT_AP );
            if( gCombat07 ) ap += gCombat07->unk01[ 0 ];
            obj->Critter.State.CurrentAP = ap;
            CombatUnk33( obj, 0 );
        }            
    }        
}

int CombatUnk24( Obj_t *obj )
{
    int pe, i;

    pe = FeatGetVal( obj, FEAT_PERCEPTION );
    for( i = 0; i < gCombat04; i++ ){
	if( ObjGetDistance( gCombatCritters[ i ], obj ) <= pe ) return 1;
    }
    return 0;        
}

int CombatUnk25( Obj_t **a1, Obj_t **a2 )
{
    int a, b;

    a = FeatGetVal( *a1, FEAT_SEQ );
    b = FeatGetVal( *a2, FEAT_SEQ );
    if( a > b ) return -1;
    if( a < b ) return 1;
    a = FeatGetVal( *a1, FEAT_LUCK );
    b = FeatGetVal( *a2, FEAT_LUCK );
    if( a > b ) return -1;
    return a < b;
}

int CombatUnk26( Obj_t *a1, Obj_t *a2 )
{
    int i, err;
    Obj_t *e;

    gCombat04 = 0;
    if( a1 ){
	for( i = 0; i < gCombatCritCnt; i++ ){
    	    if( a1 == gCombatCritters[ i ] ){
    		e = gCombatCritters[ i ];
    		gCombatCritters[ i ] = gCombatCritters[ 0 ];
    		gCombatCritters[ 0 ] = e;
    		gCombat04 = 1;
    	        break;
    	    }            
	}
    }

    if( a2 && a2 != a1 ){
	for( i = 0; i < gCombatCritCnt; i++ ){
    	    if( a2 == gCombatCritters[ i ] ){
    		e = gCombatCritters[ i ];
    		gCombatCritters[ i ] = gCombatCritters[ gCombat04 ];
    		gCombatCritters[ gCombat04 ] = e;
    		gCombat04++;
        	break;
	    }
	}
    }
    
    if( a1 != gObjDude && a2 != gObjDude ){        
        for( i = 0; i < gCombatCritCnt; i++ ){            
            if( gCombatCritters[ i ] == gObjDude ){
        	e = gCombatCritters[ i ];
    		gCombatCritters[ i ] = gCombatCritters[ gCombat04 ];
    		gCombatCritters[ gCombat04 ] = e;
    		gCombat04++;
        	break;
            }
        }
    }

    gCombatCritters = gCombatCritters;
    gCombat05 -= gCombat04;
    if( a1 ) err = CritterUnk45( a1, a2 );
    if( a2 ) return CritterUnk45( a2, a1 );
    return err;
}

void CombatUnk27()
{
    int k, i;
    Obj_t *obj, *tmp;

    CombatUnk23();
    k = gCombat04;
    for( i = 0; i < gCombat04; i++ ){
        obj = gCombatCritters[ i ];
        if( obj->Critter.State.CombatResult & 0x80 ){
            gCombatCritters[ i ] = gCombatCritters[ k - 1 ];
            gCombatCritters[ k - 1 ] = obj;            
            gCombatCritters[ k - 1 ] = gCombatCritters[ k + gCombat05 - 1 ];
            gCombatCritters[ k + gCombat05 - 1 ] = obj;
            i--;
            k--;
        }
    }
    for( i = 0; i < k; i++ ){
        obj = gCombatCritters[ i ];
        if( obj == gObjDude ) continue;
        if( (obj->Critter.State.CombatResult & 0x01) || obj->Critter.State.Reaction == 2 ){
            obj->Critter.State.Reaction &= ~0x01;
            tmp = gCombatCritters[ i ];
            gCombatCritters[ i ] = gCombatCritters[ k - 1 ];
            gCombatCritters[ k - 1 ] = tmp;
            k--;
            i--;
            gCombat05++;
        }
    }
    if( k ){
	gCombat04 = k;
        qsort( gCombatCritters, gCombat04, sizeof( Obj_t *), (void *)CombatUnk25 );
	k = gCombat04;
    }
    gCombat04 = k;
    ScptTimeCap2( 5 );
}

void CombatProcess()
{
    int i;
    Obj_t *obj;
    MsgLine_t msg;

    if( gCombatMapLvl != gObjDude->Elevation ){
        gCombatStatus |= 0x08;
        AiUnk24();
        return;
    }    
    for( i = 0; i < gCombat04; i++ ){
        obj = gCombatCritters[ i ];
        if( obj == gObjDude ) continue;
        if( gObjDude->Critter.State.GroupId == obj->Critter.State.GroupId ){
//    	    if( !(obj->Critter.State.WhoHitMe) || (obj->Critter.State.GroupId != obj->Critter.State.WhoHitMe->Critter.State.GroupId ) ) continue;
        }
        if( !AiUnk51( gCombatCritters[ i ] ) ){
	    msg.Id = 103; // 'Combat cannot end with nearby hostile creatures.'
	    if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
	    return;
        }
    }    
    for( i = gCombat04; i < (gCombat05 + gCombat04); i++ ){
        obj = gCombatCritters[ i ];
        if( obj == gObjDude ) continue;
        if( gObjDude->Critter.State.GroupId == obj->Critter.State.GroupId ){        
//    	    if( !(obj->Critter.State.WhoHitMe) || ( obj->Critter.State.GroupId != obj->Critter.State.WhoHitMe->Critter.State.GroupId ) ) continue;
        }
    	if( AiUnk52( obj ) ){
            msg.Id = 103; // 'Combat cannot end with nearby hostile creatures.'
            if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) IfcMsgOut( msg.Text ); 
            return;
        }                                    
    }
    gCombatStatus |= 0x08;
    AiUnk24();
    return;
}

void CombatUpdate()
{
    while( gCombat00 > 0 ) InpWinUpdate();
}

int CombatUnk30()
{
    int tmp, sel;

    tmp = 0;
    while( gCombatStatus & 0x02 ){
        if( (gCombatStatus & 8) != 0 || (gObjDude->Critter.State.CombatResult & 0x8081) != 0 || gMenuEscape || gCombatTacticMode ) break;
        sel = InpUpdate();
        if( ActionUnk13() ){
            while( gCombat00 > 0 ) InpWinUpdate();
        }
        if( gObjDude->Critter.State.CurrentAP <= 0 && gCombatMovePts <= 0 ) break;
        if( sel == 32 ) break;
        if( sel == 13 ){
            CombatProcess();
        } else {
            ScptUnk122();
            GameProcess( sel, 1 );
        }
    }
    if( gMenuEscape == 1 ){ tmp = gMenuEscape; gMenuEscape = 0; }
    if( gCombatStatus & 0x08 ){ gCombatStatus &= ~0x08; return -1; }
    if( gMenuEscape || tmp || gCombatTacticMode ) return -1;    
    ScptUnk122();
    return 0;    
}

void CombatUnk31()
{
    gCombatStatus &= ~0x02;
}

void CombatUnk32()
{
    int ap, i;
        
    for( i = 0; i < gCombat04; i++ ){
        ap = FeatGetVal( gCombatCritters[ i ], FEAT_AP );
        if( gCombat07 ) ap += gCombat07->unk01[ 0 ];
        gCombatCritters[ i ]->Critter.State.CurrentAP = ap;
        if( IN_COMBAT && gCombatCritters[ i ] ){
            if( gCombatCritters[ i ]->CritterIdx != -1 ) gCombat03[ gCombatCritters[ i ]->CritterIdx ].i04 = 0;
        }
    }
}

int CombatUnk33( Obj_t *obj, int edx0 )
{
    int i, flg;
    VidRect_t Area;
    Scpt_t *scr;

    gCombat19 = obj;
    flg = 0;
    CombatSetUp( &gCombat20, obj, 0, 4, 3 );
    if( obj->Critter.State.CombatResult & 0x8081 ){
        obj->Critter.State.CombatResult &= ~0x8000;
    } else {
        if( obj == gObjDude ){
            KeyFlush();
            IfaceResetAC( 1 );
            gCombatMovePts = 2 * PerkLvl( gObjDude, PERK_BONUS_MOVE );
            IfaceRenderAP( gObjDude->Critter.State.CurrentAP, gCombatMovePts );
        } else {
            SoundUpdateAll();
        }
        if( obj->ScrId != -1 ){
            ScptUnk138( obj->ScrId, 0, 0 );
            ScptSetArg( obj->ScrId, 4 );
            ScptExecScriptProc( obj->ScrId, 13  );
            if( ScptPtr( obj->ScrId, &scr ) != -1 ) flg = scr->i18;
            if( gMenuEscape == 1 ) return -1;
        }
        if( !flg ){
            if( !edx0 && CritterUnk31( obj ) ) CombatRecovery( obj );
            if( obj == gObjDude ){
                GameIfaceEnable();
                GmouseUnk55();
                if( gCombat07 ) CombatStartAttack( gCombat07->Target );
                if( !edx0 ) gCombatStatus |= 0x02;
                IfaceUnk24();                
                for( i = 0; i < gCombatCritCnt; i++){
                    CombatUnk01( gCombatCritters[ i ], 0 );
                }
                if( gTargetHighlightLvl ) CombatTargetHighlight();
                if( CombatUnk30() == -1 ){
                    GameIfaceDisable( 1 );
                    GmouseLoadCursor( 26 );
                    obj->Critter.State.DmgLastTurn = 0;
                    IfaceUnk25();
                    CombatUnk74();
                    IfaceRenderAP( -1, -1 );
                    IfaceResetAC( 1 );
                    gCombatMovePts = 0;
                    return -1;
                }
            } else {
                if( !ObjUnk34( obj, &Area ) ) TileUpdateArea( &Area, obj->Elevation );
                AiUnk50( obj, ( gCombat07 ) ? gCombat07->Target : NULL );
            }
            while( gCombat00 > 0 ) InpWinUpdate();
            if( obj == gObjDude ){
                GameIfaceDisable( 1 );
                GmouseLoadCursor( 26 );
                IfaceUnk25();
                CombatUnk74();
                IfaceRenderAP( -1, -1 );
                gCombat19 = 0;
                IfaceResetAC( 1 );
                gCombat19 = gObjDude;
            } else if( !ObjUnk35( obj, &Area ) ){
                TileUpdateArea( &Area, obj->Elevation );
            }
        }
    }
    obj->Critter.State.DmgLastTurn = 0;
    if( (gObjDude->Critter.State.CombatResult & 0xff ) < 0 ) return -1;
    if( obj == gObjDude && (gCombatMapLvl != gObjDude->Elevation) ) return -1;
    gCombatMovePts = 0;
    return 0;
}

int CombatUnk34()
{
    int i;

    if( gCombat04 <= 1 ) return 1;        
    for( i = 0; i < gCombat04; i++ ){
        if( gObjDude == gCombatCritters[ i ] ) break;
    }
    if( i == gCombat04 ) return 1;
    for( i = 0; i < gCombat04; i++ ){
        if( gObjDude->Critter.State.GroupId != gCombatCritters[ i ]->Critter.State.GroupId ) break;
        if( gCombatCritters[ i ]->Critter.State.WhoHitMe ){
//            if( gObjDude->Critter.State.GroupId == gCombatCritters[ i ]->Critter.State.WhoHitMe->Critter.State.GroupId ) break;
        }
    }
    return i == gCombat04;
}

void CombatStart( Combat02_t *pObj )
{
/*
    int j, i;

    if( pObj ){
	if( pObj->Critter && gCurrentMapLvl != pObj->Critter->Elevation ) return;
	if( pObj->Target && gCurrentMapLvl != pObj->Target->Elevation ) return;
    }
    CombatUnk17( 0 );
    if( IN_COMBAT ){
        if( CombatUnk33( gObjDude, 1 ) == -1 ){
            j = -1;
        } else {
            for( i = 0; i < gCombat04; i++){
                if( gCombatCritters[ i ] == gObjDude ) break;
            }                        
            j = i + 1;
        }
        gCombat07 = NULL;
    } else {
        if( pObj ){
    	    CombatUnk26( pObj->Critter, pObj->Target );
        } else {
    	    CombatUnk26( NULL, NULL );
        }
        gCombat07 = pObj;
        j = 0;
    }
    do{
        if( j == -1 ) break;
        CombatUnk32();                
        for( ;j < gCombat04; j++ ){
            if( CombatUnk33( gCombatCritters[ j ], 0 ) == -1 ) break;
            if( gCombat10 ) break;
            gCombat07 = NULL;
        }                
        if( j < gCombat04 ) break;
        CombatUnk27();
        j = 0;
        gCombat01++;
    } while( !CombatUnk34() );
    if( gCombatTacticMode ){
        GameIfaceEnable();
        GmouseSetMode( 0 );
    } else {
        GmouseScrollDisable();
        IfaceCombatClose( 1 );
        GmouseScrollEnable();
        CombatUnk20();
        ScptUnk30();
    }
    gCombatTacticMode = 0;
    if( gMenuEscape == 1 ) gMenuEscape = 0;        
*/
}

void CombatSetUp( Combat_t *cmbt, Obj_t *Critt, Obj_t *a3, int Hand, int a5 )
{
    cmbt->Dude = Critt;
    cmbt->Hand = Hand;
    cmbt->i04 = 3;
    cmbt->DudeDmg = 0;
    cmbt->DudeInjuries = 0;
    cmbt->AmmoCap = 0;
    cmbt->i08 = -1;
    cmbt->HandEq = ItemGetSlotItem( Critt, Hand );
    cmbt->Comp = a3;
    cmbt->TileNo = ( a3 ) ? a3->GridId : -1;
    cmbt->CompDmg = 0;
    cmbt->CompInjuries = 0;
    cmbt->CompKnockDown = 0;
    cmbt->Count = 0;
    cmbt->i15 = a3;
    cmbt->i11 = a5;
}

int CombatAttack( Obj_t *Critter, Obj_t *a2, int a3, int a4 )
{
    int ApCost, ranged;

    if( Critter != gObjDude && a3 == 4 && RandMinMax( 1, 4 ) == 1 ){
        if( ArtFileExist( ArtMakeId( 1, ART_IDX( Critter ), 17, ART_TYP( Critter ), ART_GRP( Critter ) ) ) ) a3 = 5;
    }
    CombatSetUp( &gCombat20, Critter, a2, a3, a4 );
    eprintf( "computing attack...\n" );
    if( CombatUnk42( &gCombat20 ) == -1 ) return -1;
    if( gCombat07 ){
        gCombat20.CompDmg += gCombat07->unk01[ 2 ];
        if( gCombat20.CompDmg < gCombat07->unk01[ 3 ] ) gCombat20.CompDmg = gCombat07->unk01[ 3 ];
        if( gCombat20.CompDmg > gCombat07->unk02 ) gCombat20.CompDmg = gCombat07->unk02;
//        if( gCombat07->unk03 ) gCombat20.CompInjuries = gCombat07[1].Target; // ??????
    }
    if( gCombat20.i11 == 3 || gCombat20.i11 == 8 ){
        if( Critter == gObjDude )
            IfaceGetWeaponDsc( &a3, &ranged );
        else
            ranged = 0;
    } else {
        ranged = 1;
    }
    ApCost = ItemGetAPCost( Critter, gCombat20.Hand, ranged );
    eprintf( "sequencing attack...\n" );
    if( ActionUnk30( &gCombat20 ) == -1 ) return -1;
    Critter->Critter.State.CurrentAP = ( ApCost > Critter->Critter.State.CurrentAP ) ? 0 : ( Critter->Critter.State.CurrentAP - ApCost );
    if( Critter == gObjDude ){
        IfaceRenderAP( gObjDude->Critter.State.CurrentAP, gCombatMovePts );
        CritterUnk45( gObjDude, a2 );
    }
    gCombat08 = 1;
    gCombat12 = 1;
    CombatUnk12( Critter, a2 );
    eprintf( "running attack...\n" );
    return 0;            
}

int CombatUnk37( Obj_t *a1, Obj_t *a2 )
{
    return TileGetTileNumInDir( a1->GridId, TileTurnAt( a1->GridId, a2->GridId ), 1 );
}

int CombatUnk38( Combat_t *cmbt )
{
    Obj_t *a5;
    int GridId,v5,v6,v12;

    v12 = 1;
    GridId = cmbt->Dude->GridId;
    v5 = TileUnk18( cmbt->Dude->GridId, cmbt->Comp->GridId, ItemGetRange( cmbt->Dude, cmbt->Hand ) );
    a5 = cmbt->Dude;
    while( a5 && GridId != v5 ){
        AnimUnk07( cmbt->Dude, GridId, v5, 0, &a5, 32, (void *)ObjUnk55 );            
        if( a5 && a5->Flags >= 0 ){
            if( (a5->ImgId & 0xF000000) >> 24 != 1 ){ v12 = 2; break; }
            if( a5 != cmbt->Comp ){
                v6 = CombatGetHitChance( cmbt->Dude, cmbt->Dude->GridId, a5, cmbt->i11, cmbt->Hand, 1 ) / 3;
                if( CritterIsDead( a5 ) ) v6 = 5;
                if( RandMinMax( 1, 100 ) <= v6 ){ v12 = 2; break; }
            }
            GridId = a5->GridId;
        }
    }    
    cmbt->i11 = 3;
    if( v12 < 2 ) return 0;
    if( !a5 || a5->Flags < 0 ) return 0;
    cmbt->Comp = a5;
    cmbt->TileNo = a5->GridId;
    cmbt->DudeInjuries |= 0x100;
    cmbt->i11 = 3;
    CombatSetDmgChance( cmbt, 1, 2 );
    return 1;
}

int CombatUnk39( Combat_t *cmbt, int a2, int a3, int a4 )
{
    Obj_t *Dude, *obj;
    int GridId,HitChance,i,v17,a2a;

    obj = cmbt->Dude;
    Dude = cmbt->Dude;
    v17 = 0;
    GridId = Dude->GridId;
    while( obj ){
        if( (a3 <= 0 && a4 != 47) || GridId == a2 || cmbt->Count >= 6 ) break;
        AnimUnk07( cmbt->Dude, GridId, a2, 0, &obj, 32, (void *)ObjUnk55 );
        if( obj ){
            if( OBJTYPE( obj->ImgId ) != TYPE_CRIT ) break;
            HitChance = CombatGetHitChance( cmbt->Dude, cmbt->Dude->GridId, obj, 3, cmbt->Hand, 1 );
            if( a4 == 47 ) a3 = 1;
            a2a = 0;
            while( RandMinMax( 1, 100 ) <= HitChance && a3 > 0 ){
                a3--;
                a2a++;
            }
            if( a2a ){
                if( obj == cmbt->Comp ){
                    v17 += a2a;
                } else {
                    for( i = 0; i < cmbt->Count; i++ ){
                        if( obj == cmbt->obj[i] ) break;
                    }                    
                    cmbt->unk1[ i ] = 3;
                    cmbt->obj[ i ] = obj;
                    CombatSetUp( &gCombatUnk09, cmbt->Dude, obj, cmbt->Hand, 3 );
                    gCombatUnk09.DudeInjuries |= 0x100;
                    CombatSetDmgChance( &gCombatUnk09, a2a ,2 );
                    if( i == cmbt->Count ){
                        cmbt->Damage[ i ] = gCombatUnk09.CompDmg;
                        cmbt->Injuries[ i ] = gCombatUnk09.CompInjuries;
                        cmbt->KnockDown[ i ] = gCombatUnk09.CompKnockDown;
                        cmbt->Count++;
                    } else if( a4 == 46 ){
                        cmbt->Damage[ i ] += gCombatUnk09.CompDmg;
                        cmbt->Injuries[ i ] |= gCombatUnk09.CompInjuries;
                        cmbt->KnockDown[ i ] += gCombatUnk09.CompKnockDown;
                    }
                }
            }
            GridId = obj->GridId;
        }
    }
    if( a4 == 47 ) return 0;
    return v17;
}

int CombatUnk40( Combat_t *cmbt, int a2, int *a3, int *pAmmoCharges, int a5 )
{
    int AmmoCharges,v10,v14,v15,Range,v18,GridId,v22,v27,v28,ebx0,v30,v31;

    *a3 = 0;
    AmmoCharges = ItemGetAmmo( cmbt->HandEq );
    v10 = Item57( cmbt->HandEq );
    if( v10 < AmmoCharges ) AmmoCharges = v10;
    *pAmmoCharges = AmmoCharges;
    v30 = RandUnk05( a2, FeatGetVal( cmbt->Dude, FEAT_CRIT ), 0 );
    if( v30 == 0 ) return 0;
    if( v30 == 3 ) a2 += 20;
    if( a5 == 46 ){
        v31 = AmmoCharges / 3;
        if( !(AmmoCharges / 3) ) v31 = 1;
        ebx0 = AmmoCharges / 3;
        v28 = AmmoCharges - v31 - AmmoCharges / 3;
        v14 = v31 / 2;
        if( !(v31 / 2) ){
            v14 = 1;
            v31--;
        }
    } else {
        ebx0 = v14 = v31 = v28 = 1;
    }
    for( v15 = 0; v15 < v14; v15++ ){
        if( RandUnk05( a2, 0, 0 ) >= 2 ) ++*a3;
    }
    if( !*a3 && CombatUnk38( cmbt ) ) *a3 = 1;
    Range = ItemGetRange( cmbt->Dude, cmbt->Hand );
    v18 = TileUnk18( cmbt->Dude->GridId, cmbt->Comp->GridId, Range );
    *a3 += CombatUnk39( cmbt, v18, v31 - *a3, a5 );
    if( ObjGetDistance( cmbt->Dude, cmbt->Comp ) <= 3 )
        GridId = TileUnk18( cmbt->Dude->GridId, cmbt->Comp->GridId, 3 );
    else
        GridId = cmbt->Comp->GridId;
    v27 = TileTurnAt( GridId, cmbt->Dude->GridId );
    v22 = TileUnk18( cmbt->Dude->GridId, TileGetTileNumInDir( GridId, (v27 + 1) % 6, 1 ), Range );
    *a3 += CombatUnk39( cmbt, v22, ebx0, a5 );
    *a3 = CombatUnk39( cmbt, TileUnk18( cmbt->Dude->GridId, TileGetTileNumInDir( GridId, (v27 + 5) % 6, 1 ), Range ), v28, a5 ) + *a3;
    if( v30 == 1 && ( *a3 > 0 || cmbt->Count > 0 ) ){
        v30 = 2;
    } else if( v30 >= 2 && !*a3 && !cmbt->Count ){
        v30 = 1;
    }
    return v30;
}

void CombatUnk41( Combat_t *cmbt )
{
    Obj_t *aux;
    
    if( Item56( cmbt->HandEq ) != PID_FLOWER ) return;
    if( RandMinMax( 1, 100 ) > (FeatGetVal( cmbt->Dude, FEAT_STAMINA ) - 8) ) return;
    aux = NULL;
    if( cmbt->Comp != gObjDude ) aux = ItemGetSlotItem( cmbt->Comp, 2 );
    if( !( CombatUnk45( cmbt->Comp, aux ) & 0x01 ) ) cmbt->CompInjuries |= 0x01;
}

int CombatUnk42( Combat_t *cmbt )
{
    Obj_t *a1;
    int v2,Class,v9,Hand,v15,a4,a3,Distance,a2,a5,v31,Range, WeaponBase;

    a3 = a4 = 1;
    v31 = 2;
    v2 = 0;
    Range = ItemGetRange( cmbt->Dude, cmbt->Hand );
    Distance = ObjGetDistance( cmbt->Dude, cmbt->Comp );
    if( Range < Distance ) return -1;
    a5 = Item44( cmbt->Dude, cmbt->Hand );
    a2 = CombatGetHitChance( cmbt->Dude, cmbt->Dude->GridId, cmbt->Comp, cmbt->i11, cmbt->Hand, 1 );
    WeaponBase = ItemGetWeaponBase( cmbt->Dude, cmbt->HandEq );
    if( a5 == 18 && (WeaponBase == 6 || WeaponBase == 3 || WeaponBase == 5) ) v2 = 1;
    if( cmbt->i11 == 8 ) cmbt->i11 = 3;
    Class = ItemGetClass( cmbt->HandEq, cmbt->Hand );
    if( a5 == 46 || a5 == 47 ){
        v9 = CombatUnk40( cmbt, a2, &a3, &a4, a5 );
    } else {
        v9 = RandUnk05( a2, FeatGetVal( cmbt->Dude, FEAT_CRIT ) - gCombatHitPenalty[ cmbt->i11 ], 0 );
    }
    if( v9 == 1 && (TraitSpecActive( TRAIT_JINXED ) || PerkLvl( gObjDude, PERK_JINXED )) && RandMinMax( 0, 1 ) ) v9 = 0;
    if( (Class == 2 || Class == 1) && v9 == 2 && cmbt->Dude == gObjDude ){
        if( PerkLvl( cmbt->Dude, PERK_SLAYER ) ) v9 = 3;
        if( PerkLvl( gObjDude, PERK_SILENT_DEATH ) && !ActionUnk16( gObjDude, cmbt->Comp ) && CritterUsingSkill( 0 ) && gObjDude != cmbt->Comp->Critter.State.WhoHitMeObj ) v31 = 4;
        Hand = cmbt->Hand;
        if( Hand == 16 || Hand == 9 ){
            if( RandMinMax( 1, 100 ) <= 5 ){
        	v9 = 3;
            } else {
        	if( 
        	( cmbt->Hand == 18 || cmbt->Hand == 11 ) && 
        	( ( RandMinMax( 1, 100 ) <= 10 || cmbt->Hand == 10) && ( ( RandMinMax( 1, 100 ) <= 15 || cmbt->Hand == 12 ) && 
        	  ( (RandMinMax( 1, 100 ) <= 20 || cmbt->Hand == 13) && ( ( RandMinMax(1, 100) <= 40 || cmbt->Hand == 19) && RandMinMax(1, 100) <= 50)))) )
            	    v9 = 3;
            }
        }
    }
    if( Class == 4 ){
        cmbt->AmmoCap = a4;
        if( v9 == 2 && cmbt->Dude == gObjDude ){
            if( PerkLvl( cmbt->Dude, PERK_SNIPER ) ){
                if( RandMinMax( 1, 10 ) <= FeatGetVal( gObjDude, FEAT_LUCK ) ) v9 = 3;
            }
        }
    } else if( ItemGetPackQuantity( cmbt->HandEq ) > 0 ){
        cmbt->AmmoCap = 1;
    }
    if( Item67( cmbt->HandEq, &cmbt->AmmoCap ) == -1 ) return -1;    
    switch( v9 ){
        case 0: CombatInjure( cmbt ); break;
        case 1: if( Class == 4 || Class == 3 ) CombatUnk38( cmbt ); break;
        case 2 ... 3:
            if( v9 == 3 ) v31 = CombatUnk44( cmbt );
            cmbt->DudeInjuries |= 0x100;
            CombatUnk41( cmbt );
            CombatSetDmgChance( cmbt, a3, v31 ); 
            break;
    }
    if( (Class == 4 || Class == 3) && !( cmbt->DudeInjuries & 0x300 ) ){
        if( v2 ){
            v15 = RandMinMax( 1, Distance / 2 );
            if( !v15 ) v15 = 1;
            cmbt->TileNo = TileGetTileNumInDir( cmbt->Comp->GridId, RandMinMax( 0, 5 ), v15 );
        } else {
            cmbt->TileNo = TileUnk18( cmbt->Dude->GridId, cmbt->Comp->GridId, Range );
        }
        a1 = cmbt->Comp;
        AnimUnk07( a1, cmbt->Comp->GridId, cmbt->TileNo, 0, &a1, 32, (void *)ObjUnk55 );
        if( !a1 || a1 == cmbt->Comp )
            a1 = ObjReach( 0, cmbt->TileNo, cmbt->Comp->Elevation );
        else
            cmbt->TileNo = a1->GridId;
        if( a1 && a1->Flags >= 0 ) {
            cmbt->DudeInjuries |= 0x100;
            cmbt->Comp = a1;
            CombatSetDmgChance( cmbt, 1, 2 );
        }
    }
    if( WeaponBase == 6 || v2 ){
        if( ( cmbt->DudeInjuries & 0x100 ) || !(cmbt->DudeInjuries & 0x200) ){
    	    CombatExplosion( cmbt, 0, v2, 0 );
    	    CombatKillUpdate( cmbt );
    	    return 0;
        }
    }
    if( cmbt->DudeInjuries & 0x1000 ) CombatExplosion( cmbt, 1, v2, 0 );
    CombatKillUpdate( cmbt );
    return 0;        
}

void CombatExplosion( Combat_t *cmbt, int a2, int ecx0, int a4 )
{
    Obj_t *obj, *Dude;
    int v5,GridId,i,a3,a1,v22,v23,v24,v25;

    v5 = -1;
    if( a2 ){
        Dude = cmbt->Dude;
	GridId = ( cmbt->Dude ) ? cmbt->Dude->GridId : cmbt->TileNo;
    } else {
        if( (cmbt->DudeInjuries & 0x100) != 0 ){
            Dude = cmbt->Comp;
            GridId = ( cmbt->Comp ) ? cmbt->Comp->GridId : cmbt->TileNo;
        } else {
    	    Dude = NULL;
	    GridId = cmbt->TileNo;
	}
    }    
    a1 = GridId;
    a3 = cmbt->Dude->Elevation;
    v25 = 0;
    v22 = GridId;
    if( GridId == -1 ){
        eprintf("\nError: compute_explosion_on_extras: Called with bad target/tileNum");
    } else {
        while( 1 ){
    	    if( cmbt->Count >= 6 ) break;
            if( !v25 ) goto LABEL_45;
            if( v5 == -1 ){
        	if( !(++v23 % v25) && ++v24 == 6 ) v24 = 0;
            } else {
        	v5 = TileGetTileNumInDir( v5, v24, 1 );
        	if( v5 != v22 ){
            	    if( !(++v23 % v25) && ++v24 == 6 ) v24 = 0;
        	} else {
            	    v25++;
            	    if( !ecx0 ) goto LABEL_19;
LABEL_45:
            	    if( Item70() < v25 ){
                	v5 = -1; v22 = -1; v24 = 2; v23 = 0;
            	    } else {
LABEL_19:	
                	if( ecx0 ){
                	    v5 = TileGetTileNumInDir( v22, 0, 1 );
                	} else {
                	    if( Item71() >= v25 )
                    		v5 = TileGetTileNumInDir( v22, 0, 1 );
                	    else
                    		v5 = -1;
            		}
            		v22 = v5; v24 = 2; v23 = 0;
            	    }
        	}
            }
            if( v5 == -1 ) break;
            obj = ObjReach( Dude, v5, a3 );
            if( obj && OBJTYPE( obj->ImgId ) == TYPE_CRIT && !(obj->Critter.State.CombatResult & 0x80) && obj->Flags >= 0 && !CombatBlockedAim(obj, obj->GridId, a1, 0, 0) ){
                if( obj == cmbt->Dude ){
                    cmbt->DudeInjuries &= ~0x100;
                    CombatSetDmgChance( cmbt, 1, 2 );
                    cmbt->DudeInjuries |= cmbt->DudeInjuries & 0x100;
                    cmbt->DudeInjuries |= 0x400000;
                } else {
                        for( i = 0; i < cmbt->Count; i++ ){
                            if( obj == cmbt->obj[ i ] ) break;
                        }
                    if( i == cmbt->Count ){
                        cmbt->unk1[ i ] = 3;
                        cmbt->obj[ i ] = obj;
                        CombatSetUp( &gCombatExplosion, cmbt->Dude, obj, cmbt->Hand, 3 );
                        if( !a4 ){
                            gCombatExplosion.DudeInjuries |= 0x100;
                            CombatSetDmgChance( &gCombatExplosion, 1, 2 );
                        }
                        cmbt->Damage[ i ] = gCombatExplosion.CompDmg;
                        cmbt->Injuries[ i ] = gCombatExplosion.CompInjuries;
                        cmbt->KnockDown[ i ] = gCombatExplosion.CompKnockDown;
                        cmbt->Count++;
                    }
                }
            }
        }
    }
}

int CombatUnk44( Combat_t *cmbt )
{
    Obj_t *Comp, *SlotItem;
    const Combat03_t *v11, *v14;
    int rand, sel;

    Comp = cmbt->Comp;
    SlotItem = 0;
    if( Comp && CritterGetInjure( Comp->Pid, 0x400 ) ) return 2;
    if( cmbt->Comp && OBJTYPE( cmbt->Comp->Pid ) != TYPE_CRIT ) return 2;
    cmbt->DudeInjuries |= CMBT_CRIT_HIT;
    rand = RandMinMax( 1, 100 ) + FeatGetVal( cmbt->Dude, FEAT_16 ); // critical hit table roll modifier
    sel = 0;
    if( rand > 20 ) sel = 1;
    if( rand > 45 ) sel = 2;
    if( rand > 70 ) sel = 3;
    if( rand > 90 ) sel = 4;
    if( rand > 100 ) sel = 5;

    if( cmbt->Comp == gObjDude ){
        v11 = &gCombatUnk102[ 6 * cmbt->i11 ];
    } else {
        v11 = &gCombatUnk101[ 6 * ( 9 * CritterGetGender( cmbt->Comp ) + cmbt->i11 ) ];
     }
    v14 = &v11[ sel ];
    cmbt->CompInjuries |= v14->i02;
    if( v14->i03 == -1 || FeatDice( cmbt->Comp, v14->i03, v14->i04, 0 ) > 1 ){
        cmbt->i08 = v14->i06;
    } else {
        cmbt->CompInjuries |= v14->i05;
        cmbt->i08 = v14->i07;
    }
    if( cmbt->CompInjuries & CMBT_DEAL_INJURE ){
        cmbt->CompInjuries &= ~CMBT_DEAL_INJURE;
        switch( RandMinMax( 0, 3 ) ){
            case 0: cmbt->CompInjuries |= 0x04; break;
            case 1: cmbt->CompInjuries |= 0x08; break;
            case 2: cmbt->CompInjuries |= 0x10; break;
            case 3: cmbt->CompInjuries |= 0x20; break;
        }
    }
    if( Item56( cmbt->HandEq ) == 117 ) cmbt->CompInjuries |= 0x01;
    if( cmbt->Comp != gObjDude ) SlotItem = ItemGetSlotItem( cmbt->Comp, 2 );
    cmbt->CompInjuries = ~CombatUnk45( cmbt->Comp, SlotItem ) & cmbt->CompInjuries;
    return v14->i01;
}

int CombatUnk45( Obj_t *obj, Obj_t *Item )
{
    int flg;

    flg = 0;
    if( obj && OBJTYPE( obj->Pid ) == TYPE_CRIT && CritterGetInjure( obj->Pid, CRITT_CRIP_RARM ) ) flg = 0x4000;
    if( Item && Item36( Item ) ) flg |= 0x4000;
    return flg;
}

void CombatInjure( Combat_t *cmbt )
{
    int WpnClass, tmpA, tmp;

    cmbt->DudeInjuries &= ~0x100;
    if( cmbt->Dude && CritterGetInjure( cmbt->Dude->Pid, 0x400 ) ) return;    
    if( gObjDude == cmbt->Dude ){
	if( (ScptGetGameDekaSeconds() / 864000) < 6 ) return;    
    }
    WpnClass = ItemGetClass( cmbt->HandEq, cmbt->Hand );
    tmpA = Item55( cmbt->HandEq );
    if( tmpA == -1 ) tmpA = 0;
    tmp = RandMinMax( 1, 100 ) - 5 * ( FeatGetVal( cmbt->Dude, FEAT_LUCK ) - 5 );
    if( tmp > 20 ){
        if( tmp > 50 ){
            if( tmp > 75 )
                tmp = tmp > 95 ? 4 : 3;
            else
                tmp = 2;
        } else {
            tmp = 1;
        }
    } else {
        tmp = 0;
    }
    tmp = 5 * tmpA + tmp; // tmp: 0..4
    if( !gCombatUnk100[ tmp ] ) return;
    cmbt->DudeInjuries |= CMBT_CRIT_HIT;
    cmbt->DudeInjuries |= gCombatUnk100[ tmp ];
    cmbt->DudeInjuries &= ~CombatUnk45( cmbt->Dude, cmbt->HandEq );
    if( cmbt->DudeInjuries & 0x10000 ){
        if( WpnClass == 4 ){
            CombatSetDmgChance( cmbt, cmbt->AmmoCap, 2 );
        } else {
	    CombatSetDmgChance( cmbt, 1, 2 );
	}
    } else {
	if( cmbt->DudeInjuries & 0x1000 ) CombatSetDmgChance( cmbt, 1, 2 );    
    }
    if( cmbt->DudeInjuries & CMBT_BONUS_DMG ) cmbt->DudeDmg += RandMinMax( 1, 5 );    
    if( cmbt->DudeInjuries & CMBT_LOST_AP ) cmbt->Dude->Critter.State.CurrentAP = 0;
    if( cmbt->DudeInjuries & CMBT_RELOAD_AMMO ){
        if( WpnClass == 4 ){
            cmbt->AmmoCap = ItemGetAmmo( cmbt->HandEq );
        } else {
            cmbt->DudeInjuries &= ~CMBT_RELOAD_AMMO;
        }
    }
    if( cmbt->DudeInjuries & CMBT_DEAL_INJURE ){ // deal injure
        cmbt->DudeInjuries &= ~CMBT_DEAL_INJURE;
         switch( RandMinMax( 0, 3 ) ){ // random part
            case 0: cmbt->DudeInjuries |= 0x04; break;
            case 1: cmbt->DudeInjuries |= 0x08; break;
            case 2: cmbt->DudeInjuries |= 0x10; break;
            case 3: cmbt->DudeInjuries |= 0x20; break;
        }
    }
    if( !(cmbt->DudeInjuries & 0x100000) ) return;    
    if( ( cmbt->Comp = AiDrawOponent( cmbt ) ) ){
        cmbt->DudeInjuries |= CMBT_TARGET;
        cmbt->i11 = 3;
        cmbt->DudeInjuries &= ~CMBT_CRIT_HIT;
        if( WpnClass == 4 ){
            CombatSetDmgChance( cmbt, cmbt->AmmoCap, 2 );
        } else {
            CombatSetDmgChance( cmbt, 1, 2 );
        }        
    } else {
        cmbt->Comp = cmbt->i15;
    }
    if( cmbt->Comp ) cmbt->TileNo = cmbt->Comp->GridId;
}

void CombatDrawInjure( int *Injure )
{
    *Injure &= ~0x200000;
    switch( RandMinMax( 0, 3 ) ){
        case 0: *Injure |= 0x04; break;
        case 1: *Injure |= 0x08; break;
        case 2: *Injure |= 0x10; break;
        case 3: *Injure |= 0x20; break;
    }
}

int CombatDetermineHitObstacled( Obj_t *Attacker, Obj_t *Target, int Penalty, int Slot )
{
    return CombatGetHitChance( Attacker, Attacker->GridId, Target, Penalty, Slot, 1 );
}

int CombatDetermineHit( Obj_t *Attacker, Obj_t *Target, int Penalty, int Slot )
{
    return CombatGetHitChance( Attacker, Attacker->GridId, Target, Penalty, Slot, 0 );
}

int CombatDetermineHitDist( Obj_t *Attacker, int GridId, Obj_t *Target, int Penalty, int Slot )
{
    return CombatGetHitChance( Attacker, GridId, Target, Penalty, Slot, 1 );
}

int CombatGetHitChance( Obj_t *Attacker, int arg2, Obj_t *Target, int Penalty, int Slot, int Obstacle )
{
    int HitChance, LightIntensity, tmpA, flg, tmpB, LivingTarget, pe, tmp;
    Obj_t *SlotItem;

    HitChance = flg = tmpA = tmpB = LivingTarget = 0;
    SlotItem = ItemGetSlotItem( Attacker, Slot );
    if( Target ) LivingTarget = OBJTYPE( Target->ImgId ) == TYPE_CRIT;
    if( !SlotItem ){ // unarmed
        HitChance = SkillGetTotal( Attacker, SKILL_UNARMED );
    } else {
	if( Slot >= 4 ){
    	    if( Slot <= 5 || !( Slot < 8 || Slot > 19 ) ) HitChance = SkillGetTotal( Attacker, SKILL_UNARMED );
	} else {
	    HitChance = ItemGetHitChance( Attacker, Slot );
	    tmp = ItemGetClass( SlotItem, Slot );
	    if( tmp == 4 || tmp == 3 ){
		flg = 1;
		switch( Item56( SlotItem ) ){
		    case 58: tmp = 4;break;
		    case 64: tmp = 5; tmpB = 8; break;
		    default: tmp = 2; break;
		}
    		pe = FeatGetVal( Attacker, FEAT_PERCEPTION );
    		if( Target )
        	    tmpA = ObjUnk60( Attacker, arg2, Target, Target->GridId );
    		else
        	    tmpA = 0;
    		if( tmpA >= tmpB )
        	    tmpA -= ( Attacker == gObjDude ) ? tmp * (pe - 2) : pe * tmp;
    		else
        	    tmpA += tmpB;
    		if( (-2 * pe) > tmpA ) tmpA = -2 * pe;
    		if( Attacker == gObjDude ) tmpA -= 2 * PerkLvl( gObjDude, PERK_SHARPSHOOTER ); // +2 PE
    		if( tmpA >= 0 ){
        	    if( Attacker->Critter.State.CombatResult & 0x40 )
            		tmpA *= -12;
        	    else
            		tmpA *= -4;
    		} else {
        	    tmpA *= -4;
    		}
    		if( Obstacle || tmpA > 0 ) HitChance += tmpA;
    		tmp = 0;
    		if( Target && Obstacle ) CombatBlockedAim( Attacker, arg2, Target->GridId, Target, &tmp );
    		HitChance -= 10 * tmp; // obstacle
	    }
	    if( Attacker == gObjDude && TraitSpecActive( TRAIT_ONE_HANDER ) ){ // -40% chance to hit with 2h weapon, but +20% to hit with 1h weapon
    		HitChance += ( ItemWeaponTwoHanded( SlotItem ) ) ? -40 : 20;
	    }
    	    tmpA = Item54( SlotItem ) - FeatGetVal( Attacker, FEAT_STAMINA );
	    if( Attacker == gObjDude && PerkLvl( gObjDude, PERK_WEAPON_HANDLING ) ) tmpA -= 3; // effective +3 for weapon minimum strength
    	    if( tmpA > 0 ) HitChance -= 20 * tmpA;
	    if( Item56( SlotItem ) == 59 ) HitChance += 20;
	}
    }
    if( LivingTarget && Target ){
        tmp = Item72( SlotItem ) + FeatGetVal( Target, FEAT_AC );
        if( tmp < 0 ) tmp = 0;
        HitChance -= tmp;
    }
    HitChance += (( flg ) ? gCombatHitPenalty[ Penalty ] : (gCombatHitPenalty[ Penalty ] / 2));
    if( Target && (Target->Flags & 0x800) ) HitChance += 15;
    if( Attacker == gObjDude ){ // add light factor for player
        if( Target ){
            LightIntensity = ObjGetLightIntensity( Target );
            if( Item56( SlotItem ) == 66 ) LightIntensity = 0x10000;
        } else {
            LightIntensity = 0;
        }
        if( LightIntensity > 0x6666 ){
            if( LightIntensity > 0x9999 ){
                if( LightIntensity <= 0xcccc ) HitChance -= 10;
            } else {
                HitChance -= 25;
            }
        } else {
            HitChance -= 40;
        }
    }
    if( gCombat07 ) HitChance += gCombat07->unk01[ 1 ];
    if( Attacker->Critter.State.CombatResult & 0x40 ) HitChance -= 25;
    if( LivingTarget && Target && ( Target->Grid.DestMapElev & 0x03 ) ) HitChance += 40;
    if( Attacker->Critter.State.GroupId != gObjDude->Critter.State.GroupId ){ // enemy
        tmp = 1; // normal
        CfgGetInteger( &gConfiguration, "preferences", "combat_difficulty", &tmp );
        if( tmp == 2 ) HitChance += 20; // hard
        if( tmp == 0 ) HitChance -= 20; // easy
    }
    if( HitChance > 95 ) HitChance = 95; // keep 5% uncertainty
    if( HitChance < -100 ) eprintf( "Whoa! Bad skill value in determine_to_hit!\n" );
    return HitChance;
}

void CombatSetDmgChance( Combat_t *cmbt, int a2, int a3 )
{
    Obj_t *obj;
    int *pDamage, *pInjure, *pKnock;
    int DmgType, v14, v15, i, n, tmp, Bonus, Difficulty, DmgThr, DmgRes;

    if( cmbt->DudeInjuries & 0x100 ){
        pDamage = &cmbt->CompDmg;
        obj = cmbt->Comp;
        pInjure = &cmbt->CompInjuries;
        pKnock = &cmbt->CompKnockDown;
    } else {
        pDamage = &cmbt->DudeDmg;
        obj = cmbt->Dude;
        pKnock = NULL;
        pInjure = &cmbt->DudeInjuries;
    }
    *pDamage = 0;
    if( OBJTYPE( obj->ImgId ) != TYPE_CRIT ) return;
    DmgType = ItemGetWeaponBase( cmbt->Dude, cmbt->HandEq );
    DmgThr = FeatGetVal( obj, FEAT_DMG_THR( DmgType ) );
    DmgRes = FeatGetVal( obj, FEAT_DMG_RES( DmgType ) );
    if( !(*pInjure & 0x800) || DmgType == FEAT_DMG_EMP ){
        if( Item56( cmbt->HandEq ) == 60 ){
    	    DmgThr = PERCENT( 20, DmgThr );
        } else {
    	    if( cmbt->Hand == 12 || cmbt->Hand == 13 || cmbt->Hand == 18 || cmbt->Hand == 19 ) DmgThr = PERCENT( 20, DmgThr );
        }
        if( gObjDude == cmbt->Dude && TraitSpecActive( TRAIT_FINESSE ) ) DmgRes += 30;
    } else {
        DmgThr = PERCENT( 20, DmgThr );
        DmgRes = PERCENT( 20, DmgRes );
    }
    Bonus = 0;
    if( gObjDude == cmbt->Dude && ItemGetClass( cmbt->HandEq, cmbt->Hand ) == ITEM_CLASS_RANGED ) Bonus = 2 * PerkLvl( gObjDude, PERK_BONUS_RANGED_DMG );            
    Difficulty = 100;
    if( cmbt->Dude->Critter.State.GroupId != gObjDude->Critter.State.GroupId ){
        tmp = 1;
        CfgGetInteger( &gConfiguration, "preferences", "combat_difficulty", &tmp );
        if( tmp == 0 ) Difficulty = 75;  // -25%
        if( tmp == 2 ) Difficulty = 125; // +25%
    }

    DmgRes = Item73( cmbt->HandEq ) + DmgRes;
    if( DmgRes < 0 ) DmgRes = 0;
    if( DmgRes > 100 ) DmgRes = 100;
    
    v15 = Item74( cmbt->HandEq );
    v14 = Item75( cmbt->HandEq );

    for( i = 0; i < a2; i++ ){
        tmp = v15 * ( Item41( cmbt->Dude, cmbt->Hand ) + Bonus );
        if( v14 ) tmp /= v14;
        tmp = PERCENT( tmp / 2, Difficulty ) - DmgThr;
        if( tmp > 0 ) tmp -= PERCENT( tmp, DmgRes );        
        if( tmp > 0 ) *pDamage += tmp;
    }

    if( cmbt->Dude == gObjDude ){
        if( PerkLvl( cmbt->Dude, PERK_LIVING_ANATOMY ) ){
            tmp = CritterGetGender( cmbt->Comp );
            if( tmp != 10 && tmp != 16 ) *pDamage += 5; // +5 pts of damage to every living creatures
        }
        if( PerkLvl( gObjDude, PERK_PYROMANIAC ) && ItemGetWeaponBase( cmbt->Dude, cmbt->HandEq ) == ITEM_WPN_FIRE ) *pDamage += 5; // +5 dmg with fire-based weapons
    }

    if( !pKnock || ( obj->Flags & 0x800 ) ) return;
    if( DmgType != FEAT_DMG_EXPLOSIVE && cmbt->HandEq ) {
	if( ItemGetClass( cmbt->HandEq, cmbt->Hand ) != 2 ) return;
    }
    if( OBJTYPE( obj->Pid ) != TYPE_CRIT ) return;
    if( CritterGetInjure( obj->Pid, 0x4000 ) ) return;    
    tmp = 1; n = 0;
    if( obj == gObjDude ){
        if( PerkLvl( obj, PERK_STONEWELL ) ){
            n = 1;
            if( RandMinMax( 0, 100 ) < 50 ) tmp = 0;
        }
    }
    if( tmp ){
        *pKnock = *pDamage / ( ( Item56( cmbt->HandEq ) == 61 ) ? 5 : 10 );
        if( n ) *pKnock /= 2;
    }
}

void CombatKillUpdate( Combat_t *cmbt )
{
    int i;

    CombatKillCheck( cmbt->Dude, cmbt->DudeDmg, &cmbt->DudeInjuries );
    CombatKillCheck( cmbt->Comp, cmbt->CompDmg, &cmbt->CompInjuries );
    for( i = 0; i < cmbt->Count; i++ ){
        CombatKillCheck( cmbt->obj[ i ], cmbt->Damage[ i ], &cmbt->Injuries[ i ] );
    }
}

void CombatUnk54( Combat_t *cmbt, int a2 )
{
    Obj_t *obj;
    int v5, i, v20;

    v20 = 0;
    if( cmbt->Dude && OBJTYPE( cmbt->Dude->ImgId ) == TYPE_CRIT ) v20 = 1;
    v5 = (cmbt->Comp != cmbt->i15);
    if( v20 && !(cmbt->Dude->Critter.State.CombatResult & CMBT_KILLED ) ){
        CombatUnk56( cmbt->Dude, cmbt->DudeInjuries );
        CombatDealDamage( cmbt->Dude, cmbt->DudeDmg, a2, cmbt->Comp != cmbt->i15, cmbt->Dude );
    }
    if( cmbt->i15 && cmbt->i15 != cmbt->Comp ) AiUnk63( cmbt->i15 );
    v20 = cmbt->Comp && OBJTYPE( cmbt->Comp->ImgId ) == TYPE_CRIT;
    if( !v20 && !v5 && (!PartyMembRdy( cmbt->Comp ) || !PartyMembRdy( cmbt->Dude ) ) ){
        if( cmbt->Comp && cmbt->Comp->ScrId != -1 ){
            ScptSetArg( cmbt->Comp->ScrId, cmbt->DudeDmg );
            ScptUnk138( cmbt->Comp->ScrId, cmbt->Dude, cmbt->HandEq );
            ScptExecScriptProc( cmbt->Comp->ScrId, 14 );
        }
    }
    if( cmbt->Comp && OBJTYPE( cmbt->Comp->ImgId ) == TYPE_CRIT && !( cmbt->Comp->Grid.DestMapElev & 0x80 ) ){
        CombatUnk56( cmbt->Comp, cmbt->CompInjuries );
        if( v20 ){
            if( cmbt->Comp->Grid.DestMapElev & 0x81 ){
                if( !v5 || cmbt->Comp != gObjDude ) CritterUnk45( cmbt->Comp, cmbt->Dude );
            } else if( cmbt->Comp == cmbt->i15 || cmbt->Comp->Critter.State.GroupId != cmbt->Dude->Critter.State.GroupId ){
                AiUnk59( cmbt->Comp, cmbt->Dude );
            }
        }
        ScptUnk138( cmbt->Comp->ScrId, cmbt->Dude, cmbt->HandEq );
        CombatDealDamage( cmbt->Comp, cmbt->CompDmg, a2, cmbt->Comp != cmbt->i15, cmbt->Dude );
        if( v20 ) AiUnk63( cmbt->Comp );
        if( cmbt->CompDmg >= 0 && ( cmbt->DudeInjuries & CMBT_TARGET) ){
            ScptUnk138( cmbt->Dude->ScrId, 0, cmbt->Comp );
            ScptSetArg( cmbt->Dude->ScrId, 2 );
            ScptExecScriptProc( cmbt->Dude->ScrId, 13 );
        }
    }
    for( i = 0; i < cmbt->Count; i++ ){
	obj = cmbt->obj[ i ];
        if( OBJTYPE( obj->ImgId ) != TYPE_CRIT || ( obj->Critter.State.CombatResult & CMBT_KILLED ) ) continue;
        CombatUnk56( obj, cmbt->Injuries[ i ] );
        if( v20 ){
            if( obj->Critter.State.CombatResult & 0x81 ){
                CritterUnk45( obj, cmbt->Dude );
            } else if( obj->Critter.State.GroupId != cmbt->Dude->Critter.State.GroupId ){
                AiUnk59( obj, cmbt->Dude );
            }
        }
        ScptUnk138( cmbt->obj[ i ]->ScrId, cmbt->Dude, cmbt->HandEq );
        CombatDealDamage( cmbt->obj[ i ], cmbt->Damage[ i ], a2, cmbt->Comp != cmbt->i15, cmbt->Dude );
        AiUnk63( cmbt->obj[ i ] );
        if( cmbt->Damage[ i ] >= 0 && ( cmbt->DudeInjuries & CMBT_TARGET ) ){
            ScptUnk138( cmbt->Dude->ScrId, 0, cmbt->obj[ i ] );
            ScptSetArg( cmbt->Dude->ScrId, 2 );
            ScptExecScriptProc( cmbt->Dude->ScrId, 13 );
        }                    
    }
}

void CombatKillCheck( Obj_t *Critter, int damage, int *flags )
{
    if( Critter && CritterGetInjure( Critter->Pid, 1024 ) ) return; 
    if( Critter && OBJTYPE( Critter->Pid ) != TYPE_CRIT ) return;
    if( damage <= 0 ) return;
    if( ( CritterGetHp( Critter ) - damage ) <= 0 ) *flags |= 0x80;
}

void CombatUnk56( Obj_t *obj, int a2 )
{
    int v4, v5;

    if( !obj || OBJTYPE( obj->ImgId ) != TYPE_CRIT || CritterGetInjure( obj->Pid, 1024 ) || OBJTYPE( obj->Pid ) != TYPE_CRIT ) return;    
    if( !(a2 & 0x80) ){
        if( a2 & 0x01 ){
            EvQeSchedule( 10 * (35 - 3 * FeatGetVal( obj, FEAT_ENDURANCE ) ), obj, 0, 1 );
        }
    } else {
        EvQeDelA( obj );
    }
    if( obj == gObjDude && ( a2 & 0x30 ) ){
        obj->Critter.State.CombatResult |= a2 & 0x80FF;
        IfaceUnk16( &v5, &v4 );
        IfaceHandSlotUpdate( 1, v5, v4 );
    } else {
        obj->Critter.State.CombatResult |= a2 & 0x80FF;
    }    
}

void CombatDealDamage( Obj_t *obj, int damage, int UpdateHpFlag, int a4, Obj_t *result )
{
    Scpt_t *scr;
    ObjCritterCond_t *state;
    Obj_t *attacker;
    int flg;

    if( !obj ) return;
    if( OBJTYPE( obj->ImgId ) != TYPE_CRIT ) return;
    if( CritterGetInjure( obj->Pid, 1024 ) ) return;
    if( damage <= 0 ) return;    
    state = &obj->Critter.State;
    CritterHeal( obj, -damage );              
    if( obj == gObjDude ) IfaceRenderHP( UpdateHpFlag );
    state->DmgLastTurn += damage;
    if( !a4 && (!PartyMembRdy( obj ) || !PartyMembRdy( result ) ) ){
        ScptSetArg( obj->ScrId, damage );
        ScptExecScriptProc( obj->ScrId, 14 );
    }
    if( state->CombatResult & 0x80 ){ // dead condition
        ScptUnk138( obj->ScrId, state->WhoHitMeObj, 0 );
        ScptExecScriptProc( obj->ScrId, 18 );
        Item17( obj );
        if( obj != gObjDude ){
            attacker = state->WhoHitMeObj;
            if( (attacker == gObjDude) || (attacker && (attacker->Critter.State.GroupId == gObjDude->Critter.State.GroupId)) ){
                flg = 0;
                if( ScptPtr( obj->ScrId, &scr ) != -1 ) flg = scr->i18;
                if( !flg ){
                    gCombat21 += CritterUnk27( obj );
                    CritterKillStatInc( CritterGetGender( obj ) );
                }
            }
        }
        if( obj->ScrId != -1 ){ ScptRemove( obj->ScrId ); obj->ScrId = -1; }
        PartyRemoveMember( obj );
    }
}

void CombatHitInfo( Combat_t *cmbt )
{
    Obj_t *Dude, *SlotItem, *obj;
    MsgLine_t msg;
    const char *DotStr, *Name;
    char *BodyPartName, stmp1[ 280 ], stmp2[ 20 ];
    int tmp, TextBase, EntryValue, i;

    Dude = cmbt->Dude;
    DotStr = ".";
    if( Dude == gObjDude ){
        SlotItem = ItemGetSlotItem( Dude, cmbt->Hand );
        tmp = Item54( SlotItem ) - 1;
        PerkLvl( gObjDude, PERK_WEAPON_HANDLING );
        if( SlotItem ){
            if( tmp > FeatGetVal( gObjDude, FEAT_STAMINA ) ){
                msg.Id = 107; // 'You are not strong enough to use this weapon properly'
                if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
            }
        }
    }
    obj = ( cmbt->DudeInjuries & CMBT_TARGET ) ? cmbt->Comp : cmbt->Dude;
    Name = DotStr;
    stmp2[ 0 ] = '\0';
    TextBase = 600;
    msg.Id = ( FeatGetVal( gObjDude, FEAT_GENDER ) ) ? 556 : 506; // 'You'
    if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) strcpy( stmp2, msg.Text );
    if( obj == gObjDude ){
        Name = stmp2;
        TextBase = ( FeatGetVal( gObjDude, FEAT_GENDER ) ) ? 550 : 500; // ''
    } else if( obj ){
        Name = ObjGetName( obj );
        TextBase = ( FeatGetVal( obj, FEAT_GENDER ) == 1 ) ? 700 : 600; // ''
    }
    if( cmbt->Comp ){
        if( cmbt->i15 ){
            if( cmbt->Comp != cmbt->i15 && (cmbt->DudeInjuries & CMBT_TARGET) ){
                stmp1[ 0 ] = '\0';
                if( OBJTYPE( cmbt->Comp->ImgId ) == TYPE_CRIT ){
                    if( cmbt->i15 == gObjDude ){
                        msg.Id = TextBase + 8;
                        if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) sprintf( stmp1, msg.Text, Name );
                    } else {
                        msg.Id = TextBase + 9;
                        if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) sprintf( stmp1, msg.Text, Name, ObjGetName( cmbt->i15 ) );
                    }
                } else {
                    if( cmbt->Dude == gObjDude ){
                        msg.Id = ( FeatGetVal( cmbt->Dude, FEAT_GENDER ) ) ? 565 : 515; // '%s missed'
                        if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) sprintf( stmp1, msg.Text, stmp2 );
                    } else {
                        msg.Id = ( FeatGetVal( cmbt->Dude, FEAT_GENDER ) ) ? 715 : 615; // '%s missed'
                        if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) sprintf( stmp1, msg.Text, ObjGetName( cmbt->Dude ) );
                    }
                    strcpy( &stmp1[strlen( stmp1 ) ], DotStr );
                }
                IfcMsgOut( stmp1 );
            }
        }
    }
    if( cmbt->DudeInjuries & CMBT_TARGET ){
        if( cmbt->Comp ){
            if( !(cmbt->Comp->Critter.State.CombatResult & CMBT_KILLED ) ){
                stmp1[ 0 ] = '\0';
                if( OBJTYPE( cmbt->Comp->ImgId ) == TYPE_CRIT ){
                    if( cmbt->i11 == 3 ){ // not focused hit
                        if( cmbt->DudeInjuries & CMBT_CRIT_HIT ){
                            if( cmbt->CompDmg ){
                                msg.Id = TextBase + ( ( cmbt->CompDmg == 1 ) ? 24 : 20 ); // "%s were crittically hit for %d hit points"
                            } else {
                                msg.Id = TextBase + 28; // "%s was critically hit for no damage"
                            }
                            if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ){
                                if( cmbt->CompDmg <= 1 )
                                    sprintf( stmp1, msg.Text, Name );
                                else
                                    sprintf( stmp1, msg.Text, Name, cmbt->CompDmg );
                            }
                        } else {
                            CombatGetHitInfoText( stmp1, cmbt->Comp, cmbt->CompDmg );
                        }
                    } else { // focused hit
                        if( (BodyPartName = CombatGetBodyPartName( cmbt->Comp, cmbt->i11 ) ) ){
                            if( cmbt->DudeInjuries & CMBT_CRIT_HIT ){ // "%s was critically in %s for %d hit points"
                                if( cmbt->CompDmg )
                                    msg.Id = (cmbt->CompDmg == 1) ? TextBase + 21 : TextBase + 11;
                                else
                                    msg.Id = TextBase + 25; 
                            } else { // "%s were hit in %s for %d hit points"
                                if( cmbt->CompDmg )
                                    msg.Id = (cmbt->CompDmg == 1) ? TextBase + 22 : TextBase + 12;
                                else
                                    msg.Id = TextBase + 26;
                            }
                            if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ){
                                if( cmbt->CompDmg <= 1 )
                                    sprintf( stmp1, msg.Text, Name, BodyPartName );
                                else
                                    sprintf( stmp1, msg.Text, Name, BodyPartName, cmbt->CompDmg );
                            }
                        }
                    }
                    EntryValue = 1;
                    CfgGetInteger( &gConfiguration, "preferences", "combat_messages", &EntryValue );
                    if( EntryValue == 1 && ( cmbt->DudeInjuries & CMBT_CRIT_HIT ) && cmbt->i08 != -1 ){
                        msg.Id = cmbt->i08; 
                        if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) strcpy( &stmp1[ strlen( stmp1 ) ], msg.Text );
                        if( cmbt->CompInjuries & CMBT_KILLED ){
                            strcpy( &stmp1[ strlen( stmp1 ) ], DotStr );
                            IfcMsgOut( stmp1 );
                            stmp1[ 0 ] = '\0'; // 'were killed' -- 'was killed'
                            msg.Id = ( cmbt->Comp == gObjDude ) ? ( FeatGetVal( obj, FEAT_GENDER ) ? 257 : 207) : ( FeatGetVal( cmbt->Comp, FEAT_GENDER ) ? 407 : 307 );
                            if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) sprintf( stmp1, "%s %s", Name, msg.Text );
                        }
                    } else {
                        CombatGetFlagsName( stmp1, cmbt->CompInjuries, cmbt->Comp );
                    }
                    strcpy( &stmp1[ strlen( stmp1 ) ], DotStr );
                    IfcMsgOut( stmp1 );
                }
            }
        }
    }
    if( cmbt->Dude && !( cmbt->Dude->Critter.State.CombatResult & CMBT_KILLED ) ){
        if( !(cmbt->DudeInjuries & CMBT_TARGET) ){
            if( cmbt->DudeInjuries & CMBT_CRIT_HIT ){
                if ( cmbt->DudeDmg ){ // '%s critically missed and took %d hit point(s)'
                    msg.Id = TextBase + (( cmbt->DudeDmg == 1 ) ? 33 : 34);
                } else {
                    msg.Id = TextBase + 14; // '%s crittically missed'
                }
            } else {
                msg.Id = TextBase + 15; // '%s missed'
            }
            if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ){
                if( cmbt->DudeDmg <= 1 )
                    sprintf( stmp1, msg.Text, Name );
                else
                    sprintf( stmp1, msg.Text, Name, cmbt->DudeDmg );
            }
            CombatGetFlagsName( stmp1, cmbt->DudeInjuries, cmbt->Dude );
            strcpy( &stmp1[ strlen( stmp1 ) ], DotStr );
            IfcMsgOut( stmp1 );
        }
        if( ( cmbt->DudeInjuries & CMBT_TARGET || !(cmbt->DudeInjuries & CMBT_KILLED) ) && (cmbt->DudeDmg > 0 ) ){
            CombatGetHitInfoText( stmp1, cmbt->Dude, cmbt->DudeDmg );
            CombatGetFlagsName( stmp1, cmbt->DudeInjuries, cmbt->Dude );
            strcpy( &stmp1[ strlen( stmp1 ) ], DotStr );
            IfcMsgOut( stmp1 );
        }
    }
    for( i = 0; i < cmbt->Count; i++){
        if( cmbt->obj[ i ]->Critter.State.CombatResult & 0x80 ) continue;
        CombatGetHitInfoText( stmp1, cmbt->obj[ i ], cmbt->Damage[ i ] );
        CombatGetFlagsName( stmp1, cmbt->Injuries[ i ], cmbt->obj[ i ] );
        strcpy( &stmp1[ strlen( stmp1 ) ], DotStr );
        IfcMsgOut( stmp1 );
    }
}

void CombatGetHitInfoText( char *str, Obj_t *Critter, int DmgPts )
{
    int BaseIdx;
    char *Name, stmp[ 20 ];
    MsgLine_t msg;

    if( Critter == gObjDude ){
        stmp[ 0 ] = '\0';
        BaseIdx = ( FeatGetVal( gObjDude, FEAT_GENDER ) ) ? 550 : 500; // miscellaneus combat messages
        msg.Id = BaseIdx + 6; // 'You'
        if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) strcpy( stmp, msg.Text );        
        Name = stmp;
	if( !DmgPts )
    	    msg.Id = BaseIdx + 27; // '%s were hit for no damage'
	else 
	    msg.Id = BaseIdx + ( ( DmgPts == 1 ) ? 23 : 13 ); // '%s were hit for 1 hit point' : '%s were hit %d hit points'
    } else {
        Name = ObjGetName( Critter );
        if( FeatGetVal( Critter, FEAT_GENDER ) == 1 ){
            if( !DmgPts )
                msg.Id = 727; // '%s was hit for no damage'
            else
		msg.Id = ( DmgPts == 1 ) ? 723 : 713; // '%s was hit 1 hit point' : '%s was hit for %d hit points'
        } else {
	    if( !DmgPts )
    		msg.Id = 627; // '%s was hit for no damage'
	    else 
		msg.Id = ( DmgPts == 1 ) ? 623 : 613; // '%s was hit 1 hit point' : '%s was hit for %d hit points'
    	}
    }
    if( MessageGetMsg( &gCombatMsg, &msg ) != 1 ) return;    
    if( DmgPts > 1 )
	sprintf( str, msg.Text, Name, DmgPts );
    else
	sprintf( str, msg.Text, Name );    
}

void CombatGetFlagsName( char *str, int CombatFlags, Obj_t *Critter )
{
    MsgLine_t msg;
    int Mask, i, BaseIdx, Cnt, tmp[ 32 ];

    if( Critter == gObjDude ){
        BaseIdx = ( FeatGetVal( Critter, FEAT_GENDER ) ) ? 250 : 200; // female/male player combat flags strings
    } else {
        BaseIdx = ( FeatGetVal( Critter, FEAT_GENDER ) ) ? 400 : 300; // female/male combat flags strings
    }
    if( !CombatFlags ) return;
    if( CombatFlags & 0x80 ){        
        msg.Id = 108;
        if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) strcpy( &str[ strlen( str ) ], msg.Text );
        msg.Id = BaseIdx + 7; // 'were killed'
        if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) strcpy( &str[ strlen( str ) ], msg.Text );
	return;
    }
    Cnt = 0;
    for( i = 0, Mask = 1; i < 32; i++, Mask <<= 1 ){
        if( Mask == 0x200 || Mask== 0x100 || !( Mask & CombatFlags ) ) continue;
        tmp[ Cnt++ ] = i;
    }
    if( Cnt == 0 ) return;    
    Cnt--;
    for( i = 0; i < Cnt; i++ ){
        strcpy( &str[ strlen( str ) ], ", " );
        msg.Id = tmp[ i ] + BaseIdx;
        if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) strcpy( &str[ strlen( str ) ], msg.Text );
    }
    msg.Id = 108; // 'and'
    if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) strcpy( &str[ strlen( str ) ], msg.Text );
    msg.Id = tmp[ Cnt ] + BaseIdx;
    if( (MessageGetMsg( &gCombatMsg, &msg ) ) == 1 ) strcpy( &str[ strlen( str ) ], msg.Text );    
}

void CombatUnk61()
{
    gCombat00++;
    if( gCombat00 != 1 ) return;
    if( gObjDude != gCombat20.Dude ) return;    
    GameIfaceDisable( 1 );
    GmouseLoadCursor( 26 );
    if( gTargetHighlightLvl == 2 ) CombatUnk74();    
}

void CombatUnk62()
{
    Obj_t *SlotItem, *Dude;
    int Ammo;

    gCombat00--;
    if( gCombat00 != 0 ) return;
    if( gObjDude == gCombat20.Dude ) GameIfaceEnable();
    if( !gCombat12 ) return;    
    gCombat12 = 0;
    SlotItem = ItemGetSlotItem(gCombat20.Dude, gCombat20.Hand);
    if( SlotItem ){
        if( ItemGetPackQuantity( SlotItem ) > 0 ){
            Ammo = ItemGetAmmo( SlotItem );
            ItemSetCharges( SlotItem, Ammo - gCombat20.AmmoCap );
            if ( gCombat20.Dude == gObjDude )
                IfaceAmmoGauge();
        }
    }
    if( gCombat08 ){
        CombatHitInfo( &gCombat20 );
        gCombat08 = 0;
    }
    CombatUnk54(&gCombat20, 1);
    Dude = gCombat20.Dude;
    if( gCombat20.Dude == gObjDude && gTargetHighlightLvl == 2 ) CombatTargetHighlight();
    if( ScptUnk39() && (gObjDude->Grid.DestMapElev & 1) != 0 ){
        if ( Dude->Critter.State.GroupId == gObjDude->Critter.State.GroupId )
            gCombat10 = gObjDude->Critter.State.WhoHitMeObj;
        else
            gCombat10 = Dude;
    }
    CombatSetUp( &gCombat20, gCombat20.Dude, 0, 4, 3 );
    if( (Dude->Critter.State.CombatResult & 0x03) && !(Dude->Critter.State.CombatResult & 0x8081) ) CombatRecovery( Dude );
}

void CombatRecovery( Obj_t *dude )
{
    int ApCost;

    ApCost = COMBAT_RECOVERY_AP_COST;
    if( (dude == gObjDude) && PerkLvl( dude, PERK_QUICK_RECOVERY ) ) ApCost = 1;
    dude->Critter.State.CurrentAP = ( ApCost > dude->Critter.State.CurrentAP ) ? 0 : dude->Critter.State.CurrentAP - ApCost;
    if( dude == gObjDude ) IfaceRenderAP( gObjDude->Critter.State.CurrentAP, gCombatMovePts );
    AnimUnk25( dude );
    while( gCombat00 > 0 ) InpWinUpdate();
}

void CombatUnk64( char *a1, int a2, int a3 )
{
    char *bmp;
    CachePool_t *ImgObj;

    if( !(bmp = ArtGetBitmap( ArtMakeId( 6, 82, 0, 0, 0 ), 0, 0, &ImgObj ) ) ) return;    
    if( a3 >= 0 ) {
        ScrCopy( &bmp[ 9 * ( a3 % 10 ) ], 9, 17, 360, a1 + 9, a2 );
        ScrCopy( &bmp[ 9 * ( a3 / 10 ) ], 9, 17, 360, a1, a2 );
    } else {
        ScrCopy( bmp + 108, 6, 17, 360, a1, a2 );
        ScrCopy( bmp + 108, 6, 17, 360, a1 + 9, a2 );
    }
    ArtClose( ImgObj );
}

char *CombatGetBodyPartName( Obj_t *Critter, int BodyPartId )
{
    MsgLine_t msg;

    msg.Id = 10 * ArtGetAnonAlias( Critter->ImgId & 0xFFF ) + 1000 + BodyPartId;
    if ( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) return msg.Text;
    return 0;
}

void CombatUnk66( int a1, int a2 )
{
    CombatFocusPrintLabel( a2, gPalColorCubeRGB[0][31][0] );
}

void CombatUnk67( int a1, int a2 )
{
    CombatFocusPrintLabel( a2, gPalColorCubeRGB[31][0][0] );
}

void CombatFocusPrintLabel( int BodyPart, int Color )
{
    static int gCombatUnk13[ 4 ] = { 0x8000, 0x100000, 0x2000, 0x9400 };
    char *txt;

    if( BodyPart >= 4 ){        
        if( (txt = CombatGetBodyPartName( gCombatTarget, gCombatBodyParts[ BodyPart ] )) ){
            WinDrawText( gCombatFocusWin, txt, 0, 431 - gFont.LineWidth( txt ), gCombatUnk13[ BodyPart ] - 86, Color | 0x3000000 );
        }
    } else {
        txt = CombatGetBodyPartName( gCombatTarget, gCombatBodyParts[ BodyPart ] );
        if( txt ) WinDrawText( gCombatFocusWin, txt, 0, 74, gCombatUnk12[ BodyPart ] - 86, Color | 0x3000000 );
    }
}

int CombatFocusMenu( Obj_t *TargetObj, int *BodyPart, int Slot )
{
    char *Surface, *Img1, *Img2;
    int bt, i, IfaceStat, sel, FontId;
    CachePool_t *ImgObj1, *ImgObj2;

    *BodyPart = 3;
    if( !TargetObj || OBJTYPE( TargetObj->Pid ) != TYPE_CRIT ) return 0;
    gCombatTarget = TargetObj;    
    if( (gCombatFocusWin = WinCreateWindow( 68, 20, 504, 309, gPalColorCubeRGB[0][0][0], 16 ) ) == -1 ) return -1;
    Surface = WinGetSurface( gCombatFocusWin );

    Img1 = ArtGetBitmap( ArtMakeId( 6, 118, 0, 0, 0 ), 0, 0, &ImgObj1 );
    if( !Img1 ){ WinClose( gCombatFocusWin ); return -1; }
    ScrCopy( Img1, 504, 309, 504, Surface, 504 );
    ArtClose( ImgObj1 );

    if( (Img1 = ArtGetBitmap( ArtMakeId( 1, TargetObj->ImgId & 0xFFF, 64, 0, 0 ), 0, 0, &ImgObj1 ) ) ){
        ScrCopy( Img1, 170, 225, 170, Surface + 15792, 504 );
        ArtClose( ImgObj1 );
    }

    if( !( Img1 = ArtGetBitmap( ArtMakeId( 6, 8, 0, 0, 0 ), 0, 0, &ImgObj1 ) ) ){ WinClose( gCombatFocusWin ); return -1; }
    if( !( Img2 = ArtGetBitmap( ArtMakeId( 6, 9, 0, 0, 0 ), 0, 0, &ImgObj2 )) ){ ArtClose( ImgObj1 ); WinClose( gCombatFocusWin ); return -1; }

    bt = WinCreateButton( gCombatFocusWin, 210, 268, 15, 16, -1, -1, -1, 27, Img1, Img2, 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    FontId = FontGetCurrent();    
    FontSet( 101 );
    for( i = 0; i < 4; i++ ){
        CombatUnk64( &Surface[ 504 * gCombatUnk12[ i ] - 43311 ], 504, CombatDetermineHitObstacled( gObjDude, TargetObj, gCombatBodyParts[ i ], Slot ) );
        WinSetButtonHandler( WinCreateButton( gCombatFocusWin, 33, gCombatUnk12[ i ] - 90, 128, 20, i, i, -1, i, 0, 0, 0, 0 ), CombatUnk67, CombatUnk66, 0, 0 );
        CombatFocusPrintLabel( i, gPalColorCubeRGB[0][31][0] );

        CombatUnk64( &Surface[ 504 * gCombatUnk12[ i ] - 42891 ], 504, CombatDetermineHitObstacled( gObjDude, TargetObj, gCombatBodyParts[ 4 + i ], Slot ) );
        WinSetButtonHandler( WinCreateButton( gCombatFocusWin, 341, gCombatUnk12[ i ] - 90, 128, 20, 4 + i, 4 + i, -1, 4 + i, 0, 0, 0, 0 ), CombatUnk67, CombatUnk66, 0, 0 );        
        CombatFocusPrintLabel( 4 + i, gPalColorCubeRGB[0][31][0] );
    }
    WinUpdate( gCombatFocusWin );

    // event loop    
    if( (IfaceStat = GameIfaceStat()) ) GameIfaceEnable();
    GmouseSetIfaceMode( 0 );
    GmouseLoadCursor( 1 );
    do
        sel = InpUpdate();
    while( sel != KEY_ESC && sel >= 8 && !gMenuEscape ); // 0 .. 7 
    GmouseSetIsoMode();
    if( IfaceStat ) GameIfaceDisable( 0 );

    FontSet( FontId );
    ArtClose( ImgObj1 );
    ArtClose( ImgObj2 );
    WinClose( gCombatFocusWin );
    if( sel == KEY_ESC ) return -1;
    *BodyPart = gCombatBodyParts[ sel ];
    GSoundPlay( "icsxxxx1" );    
    return 0;
}

int CombatAttackTest( Obj_t *attacker, Obj_t *target, int Slot, int ShotValue )
{
    Obj_t *SlotItem;
    int CombatResult, WeaponType, Range, TargPos;

    Range = 1;
    TargPos = -1;
    if( target ){
        TargPos = target->GridId;
        Range = ObjGetDistance( attacker, target );
        if( target->Critter.State.CombatResult & CRITT_EYE_DMG ) return 4;
    }    
    if( (SlotItem = ItemGetSlotItem( attacker, Slot )) ){
	CombatResult = attacker->Critter.State.CombatResult;
	if( ( CombatResult & CRITT_CRIP_LARM ) && (CombatResult & CRITT_CRIP_RARM ) ) return 7; // both arms cripled
	if( ( CombatResult & ( CRITT_CRIP_LARM | CRITT_CRIP_RARM ) ) && ItemWeaponTwoHanded( SlotItem ) ) return 6; // two-handed weapon and one arm cripled
    }
    if( ItemGetAPCost( attacker, Slot, ShotValue ) > attacker->Critter.State.CurrentAP ) return 3; // not enogh AP
    if( ItemGetRange( attacker, Slot ) < Range ) return 2; // out of range
    WeaponType = ItemGetClass( SlotItem, Slot );
    if( ItemGetPackQuantity( SlotItem ) > 0 && !ItemGetAmmo( SlotItem ) ) return 1; // out of ammo 
    if( (WeaponType == 4 || WeaponType == 3 || ItemGetRange( attacker, Slot ) > 1 ) && 
		CombatBlockedAim( attacker, attacker->GridId, TargPos, target, 0 ) ) return 5; // blocked aim
    return 0;
}

int CombatUnk71( Obj_t *obj, int *a2 )
{
    int ShotVal;
    int Slot;

    if( IfaceGetWeaponDsc( &Slot, &ShotVal ) == -1 ) return 0;
    if( CombatAttackTest( gObjDude, obj, Slot, ShotVal ) ) return 0;
    *a2 = CombatGetHitChance( gObjDude, gObjDude->GridId, obj, 8, Slot, 1 );
    return 1;
}

void CombatStartAttack( Obj_t *Target )
{
    int HandSlot, ShotValue, BodyPart;
    char stmp[80];
    Combat02_t v6;
    MsgLine_t msg;

    if( !Target || !(gCombatStatus & 0x02) || IfaceGetWeaponDsc( &HandSlot, &ShotValue ) == -1 ) return;    
    switch( CombatAttackTest( gObjDude, Target, HandSlot, ShotValue ) ){
        case 1:
            msg.Id = 101; // 'Out of Ammo'
            if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
            GSoundPlay( GSoundProtoFname3( 2, ItemGetSlotItem( gObjDude, HandSlot ), HandSlot, 0 ) );
            return;
        case 2:
            msg.Id = 102; // 'Target out of range.'
            if( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
            return;
        case 3:
            ItemGetSlotItem( gObjDude, HandSlot ); // ??
            msg.Id = 100; // 'You need %d action points.'
            if( MessageGetMsg( &gCombatMsg, &msg ) != 1 ) return;
            sprintf( stmp, msg.Text, ItemGetAPCost( gObjDude, HandSlot, ShotValue ) );
            IfcMsgOut( stmp );
            return;
        case 4:
            return;
        case 5:
            msg.Id = 104; // 'Your aim is blocked'
            if ( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
            return;
        case 6:
            msg.Id = 106; // 'You cannot use two-handed weapon with a cripled arm.'
            if ( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
            return;
        case 7:
            msg.Id = 105; // 'You cannot use weapons with both arms cripled.'
            if ( MessageGetMsg( &gCombatMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
            return;
        default:
            if( !IN_COMBAT ){
                v6.Critter = gObjDude;
                v6.Target = Target;
                memset( v6.unk01, 0, sizeof( v6.unk01 ) );
                v6.unk02 = 0x7FFFFFFF;
                v6.unk03 = 0;
                CombatStart( &v6 );
                return;
            }
            if( !ShotValue ){
                CombatAttack( gObjDude, Target, HandSlot, 8 );
                return;
            }
            if( ShotValue != 1 ) eprintf( "Bad called shot value %d\n", ShotValue );
            if( CombatFocusMenu( Target, &BodyPart, HandSlot ) != -1 ){
                CombatAttack( gObjDude, Target, HandSlot, BodyPart );
                return;
            }
            break;
    }    
}

void CombatTargetHighlight()
{
    int i, cnt, hl;
    Obj_t **ObjList, *obj;

    hl = 2;
    CfgGetInteger( &gConfiguration, "preferences", "target_highlight", &hl );
    if( !hl || GmouseGetMode() != 2 ) return;    
    if( (gCombatStatus & 1) != 0 ){
        for( i = 0; i < gCombatCritCnt; i++ ){
            CombatUnk01( gCombatCritters[ i ], 1 );
        }
    } else {
        cnt = ObjGetObjList( -1, gCurrentMapLvl, 1, &ObjList );
        for( i = 0; i < cnt; i++ ){
            obj = ObjList[ i ];
            if( obj != gObjDude && !(obj->Critter.State.CombatResult & 0x80 ) ) CombatUnk01( obj, 1 );
        }
        if( cnt ) ObjCritterListDestroy( ObjList );
    }
    for( i = 0; i < gCombatCritCnt; i++ ){
        CombatUnk01( gCombatCritters[ i ], 1 );
    }
    TileUpdate();    
}

void CombatUnk74( )
{
    int i, cnt;
    Obj_t **ObjList;

    if( IN_COMBAT ){            
        for( i = 0; i < gCombatCritCnt; i++ ){
            ObjUnk35( gCombatCritters[ i ], 0 );
        }
    } else {
        cnt = ObjGetObjList(-1, gCurrentMapLvl, 1, &ObjList );
        for( i = 0; i < cnt; i++ ){
            ObjUnk35( ObjList[ i ], 0 );
            ObjGetRadius( ObjList[ i ], 0 );
        }
        if( cnt ) ObjCritterListDestroy( ObjList );
    }
    TileUpdate();
}

void CombatUnk75()
{
    int Hilight;

    Hilight = 2;
    CfgGetInteger( &gConfiguration, "preferences", "target_highlight", &Hilight );
    if( (Hilight != gTargetHighlightLvl) && IN_COMBAT ){
        if( Hilight ){
            if( !gTargetHighlightLvl ) CombatTargetHighlight();
        } else {
            CombatUnk74();
        }
    }
    gTargetHighlightLvl = Hilight;
}

int CombatBlockedAim( Obj_t *obj1, int GridPos1, int GridPos2, Obj_t *obj2, int *arg0 )
{
    if( arg0 ) *arg0 = 0;
    while( obj1 && (GridPos1 != GridPos2) ){
        AnimUnk07( obj1, GridPos1, GridPos2, 0, &obj1, 32, (void *)ObjUnk55 );
        if( obj1 ){
            if( ( OBJTYPE( obj1->ImgId ) != TYPE_CRIT ) && ( obj2 != obj1 ) ) return 1;
            if( arg0 ){
                if( obj2 != obj1 ){
                    if( obj2 ){
                        if( (obj2->Critter.State.CombatResult & 0xff) >= 0 ){
                            (*arg0)++;
                            if( obj2->Flags & 0x800 ) (*arg0)++;
                        }
                    }
                }
            }
            if( (obj1->Flags & 0x800) != 0 ){
                GridPos1 = TileGetTileNumInDir( GridPos1, TileTurnAt( GridPos1, GridPos2 ), 1 );
            } else {
                GridPos1 = obj1->GridId;
            }
        }
    }
    return 0;
}

int CombatGetGroupId()
{
    if( !(gObjDude->Critter.State.CombatResult & 0x80) && gCombat10 ) return gCombat10->Critter.State.GroupId;
    return -1;
}

int CombatUnk77( Obj_t *obj )
{
    ScptUnk40( obj, obj->GridId, Item71(), obj->Elevation );
    return 0;
}

void CombatUnk79( Obj_t *obj )
{
    int i, j;

    if( !IN_COMBAT ) return;
    for( i = 0; i < gCombatCritCnt; i++ ){
	if( obj != gCombatCritters[ i ] ) continue;	
	for( j = i; j < gCombatCritCnt - 1; j++ ){
    	    gCombatCritters[ j ] = gCombatCritters[ j + 1 ];
    	    CombatUnk08( j + 1, j );
	}
	gCombatCritCnt--;
	gCombatCritters[ gCombatCritCnt ] = obj;
	if( i >= gCombat04 ){
    	    if( i < gCombat05 + gCombat04 ) gCombat05--;
	} else {
    	    gCombat04--;
	}
	obj->Critter.State.CurrentAP = 0;
	ObjGetRadius( obj, 0 );
	obj->Critter.State.WhoHitMe = 0;
	AiUnk65( obj );        
	return;	
    }    
}

void CombatKillPoison( Obj_t *obj, char *text )
{
    if( obj == gObjDude ) return;
    IfcMsgOut( text );
    ScptExecScriptProc( obj->ScrId, 18 );
    CritterKill( obj, -1, 1 );
}



