#include "FrameWork.h"

int gActionUnk06 = 0;
int gActionUnk05 = 0;
int gActionUnk01 = -1;
int gActionUnk02 = -1;
int gActionUnk04[ 7 ] = { 26, 28, 24, 24, 27, 20, 23 };
int gActionUnk03[ 7 ] = { 25, 28, 33, 32, 30, 20, 31 };

// no xref
void ActionCopyPlayer()
{
    Obj_t *tmp;

    if( !(tmp = ActionUnk18( 1 )) ) return;    
    FeatSetBase( gObjDude, FEAT_GENDER , FeatGetVal( tmp, FEAT_GENDER ) );
    gActionUnk01 = tmp->ImgId;
    gObjDude = tmp;
    gActionUnk02 = tmp->Pid;
    tmp->Pid = 0x1000000;    
}

int ActionUnk40( Obj_t *obj, int *a2, int a3, int a4, int a5 )
{
    int i;
    int GridId;

    if( CritterGetInjure( obj->Pid, 0x4000 ) ) return -1;
    if( *a2 == 21 ){
        if( !ArtFileExist( ArtMakeId( 1, obj->ImgId & 0xFFF, *a2, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 ) ) ) *a2 = 20;
    }
    for( i = 1; i <= a3; i++ ){
        if( ObjReach( obj, TileGetTileNumInDir( obj->GridId, a4, i ), obj->Elevation) ) break;
    }
    AnimRegPlaySfx( obj, GSoundCharacterFileName( obj, *a2, 1 ), a5 );
    if( i <= 0 ){
        GridId = obj->GridId;
        AnimRegAnimate( obj, *a2, 0 );
    } else {
        GridId = TileGetTileNumInDir( obj->GridId, a4, i );
        AnimUnk45( obj, GridId, obj->Elevation, *a2, 0 );
    }
    return GridId;
}

int ActionAnimGore( Obj_t *obj, int a2, int a3 )
{
    int n, gore;

    gore = 3;
    CfgGetInteger( &gConfiguration, "preferences", "violence_level", &gore );
    if( gore == 0 ) return a2;
    if( a2 == 20 ){
        n = 34;
    } else {
        if( a2 != 21 ) return a2;
        n = 35;
    }
    if( ArtFileExist( ArtMakeId( 1, obj->ImgId & 0xFFF, n, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 ) ) ) AnimRegAnimate( obj, n, a3 );
    return n;
}

int ActionUnk38( Obj_t *a1, Obj_t *a2, Obj_t *a3, int a4, int a5, int a6 )
{
    int v7,v8,v15,gore,v21,WeaponBase;

    v7 = 15; v8 = 45;
    v21 = 0;
    gore = 3;
    WeaponBase = ItemGetWeaponBase( a1, a3 );
    if( a3 && a3->Pid == 0x9F ){
        v7 = 5; v8 = 15;
        a5 = 45;
        WeaponBase = 2;
    }
    if( a1 == gObjDude && PerkLvl( a1, 101 ) && WeaponBase == 2 ){
        v7 = 1; v8 = 1;
    }
    if( a3 && Item56( a3 ) == 67 ){
	v7 = v7 / 3;
	v8 = v8 / 3;
    }
    CfgGetInteger( &gConfiguration, "preferences", "violence_level", &gore );
    if( CritterGetInjure( a2->Pid, 0x1000 ) ) return ActionUnk37( a2, 31, 2, a6 );    
    if( a1 == gObjDude && TraitSpecActive( TRAIT_BLOODY_MESS ) ) v21 = 1;
    v15 = 20;
    if( (a5 == 16 && WeaponBase == 0 ) || ((a5 == 17 || a5 == 41 || a5 == 42 || a5 == 18) && WeaponBase != 6) ){
        if( gore == 3 && v21 ) v15 = 23;        
    } else if( a5 != 45 || WeaponBase ){
        if( gore > 1 && (v21 || v7 <= a4) ){
            v15 = gActionUnk03[ WeaponBase ];
            if( gore > 2 && (v21 || v8 <= a4) ){
                if( ActionUnk37( a2, v15, 3, a6 ) != v15 ) v15 = gActionUnk04[ WeaponBase ];
            }
        }
    } else {
	if( gore == 3 && (v21 || v8 <= a4) ) v15 = 23;
    }
    if( !a6 && v15 == 20 ) v15 = 21;
    return ActionUnk37( a2, v15, 0, a6 );
}

int ActionUnk37( Obj_t *obj, int a2, int a3, int a4 )
{
    int gore;

    gore = 3;
    CfgGetInteger( &gConfiguration, "preferences", "violence_level", &gore );
    if( gore >= a3 ){
	if( ArtFileExist( ArtMakeId( 1, obj->ImgId & 0xFFF, a2, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 ) ) ) return a2;
    }
    if( a4 ) return 20;
    return ( ArtFileExist( ArtMakeId( 1, obj->ImgId & 0xFFF, 21, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 ) ) ) ? 20 : 21;
}

int ActionUnk36( int a1, Obj_t *a2 )
{
    return UseUnk06( a2 );
}

int ActionUnk35( Obj_t *obj1, int dmg, int obj3, Obj_t *a4, int a5, int a6, int a7, int a8, Obj_t *obj4, int a10 )
{
    Obj_t *v34;
    int result;
    int v18, v19, v21, a2, v39;

    if( CritterGetInjure( obj1->Pid, 0x4000 ) ) a6 = 0;
    a2 = (obj1->ImgId & 0xFF0000) >> 16;
    result = CritterUnk31( obj1 );
    if( !result ){
        if( obj3 >= 0 ){
            if( obj3 & 0x03 ){
                a2 = (a5 == 0) + 20;
                AnimRegPlaySfx( obj1, GSoundCharacterFileName( obj1, a2, 1 ), a10 );
                if( a6 ){
                    result = ActionUnk40( obj1, &a2, a6, a7, 0 );
                } else {
            	    a2 = ActionUnk14( obj1, a2 );
    		    result = AnimRegAnimate( obj1, a2, 0 );
    		}
    		goto LABEL_47;
            }
            if( obj3 & 0x400 ){
                if( ArtFileExist( ArtMakeId( 1, obj1->ImgId & 0xFFF, 33, (obj1->ImgId & 0xF000) >> 12, obj1->Orientation + 1 ) ) ){
                    AnimRegAnimate( obj1, 33, a10 );
                    result = AnimUnk62( obj1, ArtMakeId( 1, obj1->ImgId & 0xFFF, 0, (obj1->ImgId & 0xF000) >> 12, obj1->Orientation + 1 ), -1 );
                    goto LABEL_47;
                }
            }            
            if( a6 ){
                a2 = (a5 == 0) + 20;
                ActionUnk40( obj1, &a2, a6, a7, a10 );
    		result = AnimRegAnimate( obj1, ( a2 == 20 ) ? 37 : 36, -1 );
    		goto LABEL_47;
            }
            if( a5 ){
                AnimRegPlaySfx( obj1, GSoundCharacterFileName( obj1, 14, 0 ), a10 );
                v21 = 14;
            } else {
        	if( !ArtFileExist( ArtMakeId( 1, obj1->ImgId & 0xFFF, 15, (obj1->ImgId & 0xF000) >> 12, obj1->Orientation + 1 ) ) ){
        	    AnimRegPlaySfx( obj1, GSoundCharacterFileName( obj1, 14, 0 ), a10 );
            	    v21 = 14;
        	} else {
            	    AnimRegPlaySfx( obj1, GSoundCharacterFileName( obj1, 15, 0 ), a10 );
            	    v21 = 15;
        	}
            }
    	    result = AnimRegAnimate( obj1, v21, 0 );
    	    goto LABEL_47;
        } else {        
            if( ArtMakeId( 5, 10, 0, 0, 0 ) == obj4->ImgId ){
                a2 = ActionUnk37( obj1, 31, 3, a5 );
            } else if( obj4->Pid == 0x20001EB ){
                a2 = ActionUnk37( obj1, 30, 3, a5 );
            } else if( obj4->ImgId == 0x20001F5 ){
                a2 = ActionUnk37( obj1, a8, 3, a5 );
            } else {
                a2 = ActionUnk38( obj4, obj1, a4, dmg, a8, a5 );
            }
            if( a2 != 33 ){
                if( a6 && (a2 == 21 || a2 == 20) ){
                    ActionUnk40( obj1, &a2, a6, a7, a10 );
                    result = ActionAnimGore( obj1, a2, -1 );
                    a2 = result;
                } else {
                    AnimRegPlaySfx( obj1, GSoundCharacterFileName( obj1, a2, 3 ), a10 );
                    a2 = ActionUnk14( obj1, a2 );
                    result = AnimRegAnimate( obj1, a2, 0 );
                    if( a2 == 21 || a2 == 20 ){
                        result = ActionAnimGore( obj1, a2, -1 );
                        a2 = result;
                    }
                }
                goto LABEL_47;
            }
            if( ArtFileExist( ArtMakeId( 1, obj1->ImgId & 0xFFF, 33, (obj1->ImgId & 0xF000) >> 12, obj1->Orientation + 1 ) ) ){
                AnimRegPlaySfx( obj1, GSoundCharacterFileName( obj1, a2, 0 ), a10 );
                AnimRegAnimate( obj1, a2, 0 );                
                v39 = RandMinMax( 0, 5 );
                for( v18 = RandMinMax( 2, 5 ); v18 > 0; v18-- ){
                    v19 = TileGetTileNumInDir( obj1->GridId, v39, v18 );
                    v34 = NULL;
                    AnimUnk06( obj1, obj1->GridId, v19, 0, &v34, 4 );
                    if( !v34 ){
                	AnimUnk51( obj1, v19 );
                	AnimUnk44( obj1, v19, obj1->Elevation, a2, 0 );
                	break;
                    }
                }
            }
            a2 = 29;
            AnimRegPlaySfx( obj1, GSoundCharacterFileName( obj1, 29, 0 ), -1 );
    	    result = AnimRegAnimate( obj1, a2, 0 );
    	    goto LABEL_47;
        }
    }
    if( obj3 < 0 && !( obj1->Critter.State.CombatResult & 0x80 ) ){
        result = ActionAnimGore( obj1, a2, a10 );
        a2 = result;
        goto LABEL_47;
    }
    return result;
LABEL_47:
    if( a4 ){
        if( obj3 & 0x1000 ){
    	    AnimSetFinish( obj1, a4, UseDropObj, -1 );
    	    AnimUnk62( a4, ArtMakeId( 5, 10, 0, 0, 0 ), 0 );
    	    AnimUnk50( a4, 0, 0 );
    	    AnimRegPlaySfx( a4, GSoundWeaponFileName( 4, a4, 2, obj1 ), 0 );
    	    result = AnimUnk55( a4 );
        } else {
    	    if( obj3 & 0x2000 ){
    		result = AnimSetFinish( obj1, a4, (void *)ActionUnk36, -1 );
    	    } else {
    		if( obj3 & 0x4000 ) result = AnimSetFinish( obj1, a4, UseDropObj, -1 );
    	    }
        }
    }
DD
//    if( obj3 & 0x80 ) return AnimSetFinish( obj1, (void *)a2, (void *)ActionUnk34, -1 ); // !!! scalar to pointer 
    return result;
}

int ActionUnk34( Obj_t *obj, int a2 )
{
    VidRect_t v5, Rect;

    ObjGetRadiusArea( obj, &Rect );
    if( !CritterGetInjure( obj->Pid, 2048 ) ){
        obj->Flags |= 0x10;
        if( !ObjSetPlayer( obj, &v5 ) ) RegionExpand( &Rect, &v5, &Rect );
    }
    if( !ObjUnk35( obj, &v5 ) ) RegionExpand( &Rect, &v5, &Rect );
    if( a2 >= 30 && a2 <= 31 && !CritterGetInjure( obj->Pid, 4096 ) && !CritterGetInjure( obj->Pid, 64 ) ) Item18( obj, obj->GridId );
    TileUpdateArea( &Rect, obj->Elevation );
    return 0;
}

void ActionUnk33( Combat_t *pObj )
{
    int v3, v4;

    if( OBJTYPE( pObj->Comp->ImgId ) != TYPE_CRIT ) return;
    v3 = pObj->Dude->Orientation - pObj->Comp->Orientation;
    if( v3 < 0 ) v3 = -v3;
    v4 = v3 >= 2 && v3 != 5;
    AnimStart( 2 );
    AnimUnk33( 1 );
    ActionUnk35( pObj->Comp, pObj->CompDmg, pObj->CompInjuries, pObj->HandEq, v4, pObj->CompKnockDown, TileTurnAt(pObj->Dude->GridId, pObj->Comp->GridId), Item44(pObj->Dude, pObj->Hand), pObj->Dude, 0 );
    AnimBegin();
}

void ActionUnk32( Combat_t *cmbt )
{
    int i, v6;

    for( i = 0; i < cmbt->Count; i++ ){
        if( OBJTYPE( cmbt->obj[ i ]->ImgId ) != TYPE_CRIT ) continue;        
        v6 = cmbt->Dude->Orientation - cmbt->obj[ i ]->Orientation;
        if( v6 < 0 ) v6 = cmbt->obj[ i ]->Orientation - cmbt->Dude->Orientation;
        v6 = v6 >= 2 && v6 != 5;
        AnimStart( 2 );
        AnimUnk33( 1 );
        ActionUnk35( cmbt->obj[ i ], cmbt->Damage[ i ], cmbt->Injuries[ i ], cmbt->HandEq, v6, cmbt->KnockDown[ i ], TileTurnAt( cmbt->Dude->GridId, cmbt->obj[ i ]->GridId ), Item44( cmbt->Dude, cmbt->Hand ), cmbt->Dude, 0 );
        AnimBegin();
    }
}

int ActionUnk31( Combat_t *cmbt, int a2, int a3 )
{
    int i;

    for( i = 0; i < cmbt->Count; i++ ){
        if( OBJTYPE( cmbt->obj[ i ]->ImgId ) != TYPE_CRIT ) continue;
        AnimUnk68( 2, a3 );
        a3 = 0;
    }
    if( cmbt->DudeInjuries & 0x100 ){
        if( cmbt->Comp ){
            if( OBJTYPE( cmbt->Comp->ImgId ) == TYPE_CRIT ){
                if( cmbt->Dude->ImgId == 0x20001F5 ){
                    a3 = ActionUnk35( cmbt->Comp, cmbt->CompDmg, cmbt->CompInjuries, cmbt->HandEq, 1, cmbt->CompKnockDown, TileTurnAt( cmbt->Dude->GridId, cmbt->Comp->GridId), a2, cmbt->Dude, a3 );
                } else {
                    a3 = ActionUnk35( cmbt->Comp, cmbt->CompDmg, cmbt->CompInjuries, cmbt->HandEq, 1, cmbt->CompKnockDown, TileTurnAt( cmbt->Dude->GridId, cmbt->Comp->GridId ), Item44( cmbt->Dude, cmbt->Hand ), cmbt->Dude, a3 );
                }
            } else {
                TileTurnAt( cmbt->Dude->GridId, cmbt->Comp->GridId );
                a3 = Item44( cmbt->Dude, cmbt->Hand );
            }
        }
        if( (cmbt->DudeInjuries & 0x400000) != 0 ) return ActionUnk35( cmbt->Dude, cmbt->DudeDmg, cmbt->DudeInjuries, cmbt->HandEq, 1, 0, 0, a2, cmbt->Dude, -1 );
    } else if( (cmbt->DudeInjuries & 0x200) || (cmbt->DudeInjuries & 0x400000) ) {
        return ActionUnk35( cmbt->Dude, cmbt->DudeDmg, cmbt->DudeInjuries, cmbt->HandEq, 1, 0, 0, a2, cmbt->Dude, -1 );
    }
    return a3;
}

int ActionUnk30( Combat_t *cmbt )
{
    int i, n;

    if( AnimClear( cmbt->Dude ) == -2 ) return -1;
    if( AnimClear( cmbt->Comp ) == -2 ) return -1;
    for( i = 0; i < cmbt->Count; i++  ){
	if( AnimClear( cmbt->obj[ i ] ) == -2 ) return -1;
    }    
    n = Item44( cmbt->Dude, cmbt->Hand );
    if( n >= 45 || n == 18 )
        return ActionUnk27( cmbt, n );
    else
        return ActionUnk29( cmbt, n );
}

int ActionUnk29( Combat_t *cmbt, int a2 )
{
    CachePool_t *ImgObj;
    ArtFrmHdr_t *Img;
    int ActionFrame, Frame;
    char stmp[16];

    AnimStart( 2 );
    AnimUnk33( 1 );
    Img = ArtLoadImg( ArtMakeId(1, cmbt->Dude->ImgId & 0xFFF, a2, (cmbt->Dude->ImgId & 0xF000) >> 12, cmbt->Dude->Orientation + 1), &ImgObj );
    if( Img ) ActionFrame = ArtGetActionFrame( Img );
    ArtClose( ImgObj );
    TileGetTileNumInDir( cmbt->Dude->GridId, cmbt->Dude->Orientation, 1 );
    AnimUnk51( cmbt->Dude, cmbt->Comp->GridId );
    if( a2 == 16 || a2 == 17 ){
        strcpy( stmp, GSoundCharacterFileName( cmbt->Dude, a2, 0 ) );
    } else {
        strcpy( stmp, GSoundWeaponFileName( 1, cmbt->HandEq, cmbt->Hand, cmbt->Comp ) );
    }    
    AiCombatTaunts( cmbt->Dude, cmbt, 2, 0 );
    if( cmbt->DudeInjuries & 0x300 ){
        AnimRegPlaySfx( cmbt->Dude, stmp, 0 );
        if( a2 == 16 || a2 == 17 ){
    	    strcpy( stmp, GSoundCharacterFileName( cmbt->Dude, a2, 4 ) );
        } else {
    	    strcpy( stmp, GSoundWeaponFileName( 4, cmbt->HandEq, cmbt->Hand, cmbt->Comp ) );
        }
        AnimRegAnimate( cmbt->Dude, a2, 0 );
        AnimRegPlaySfx( cmbt->Dude, stmp, ActionFrame );
        ActionUnk31( cmbt, a2, 0 );
    } else {
        if( cmbt->Comp->Critter.State.CombatResult & 0x03 ){
            AnimRegPlaySfx( cmbt->Dude, stmp, -1 );
            AnimRegAnimate( cmbt->Dude, a2, 0 );
        } else {
            if( (Img = ArtLoadImg( ArtMakeId( 1, cmbt->Comp->ImgId & 0xFFF, 13, (cmbt->Comp->ImgId & 0xF000) >> 12, cmbt->Comp->Orientation + 1 ), &ImgObj ) ) ){
                Frame = ArtGetActionFrame( Img );
                ArtClose( ImgObj );
                if( Frame <= ActionFrame ){
                    AnimRegPlaySfx( cmbt->Dude, stmp, -1 );
                    AnimRegAnimate( cmbt->Dude, a2, 0 );
                    AnimRegPlaySfx( cmbt->Comp, GSoundCharacterFileName( cmbt->Comp, 13, 0 ), ActionFrame - Frame );
                    AnimRegAnimate( cmbt->Comp, 13, 0 );
                } else {
                    AnimRegPlaySfx( cmbt->Comp, GSoundCharacterFileName( cmbt->Comp, 13, 0 ), -1 );
                    AnimRegAnimate( cmbt->Comp, 13, 0 );
                    AnimRegPlaySfx( cmbt->Dude, stmp, Frame - ActionFrame );
                    AnimRegAnimate( cmbt->Dude, a2, 0 );
                }
            }
        }
    }
    if( cmbt->DudeInjuries & 0x100 ){
        if( !(cmbt->CompInjuries & 0x80) ) AiCombatTaunts( cmbt->Comp, cmbt, 4, -1 );
    } else {
        AiCombatTaunts( cmbt->Comp, cmbt, 3, -1 );
    }    
    if( AnimBegin() == -1 ) return -1;
    ActionUnk32( cmbt );
    return 0;    
}

void ActionUnk28( Obj_t *obj, int ImgId )
{
    VidRect_t area;

    eprintf( "\n[throw_change_fid!]: %d", ImgId );
    ObjSetShape( obj, ImgId, &area );
    TileUpdateArea( &area, gCurrentMapLvl );
}

int ActionUnk27( Combat_t *cmbt, int a2 )
{
    ArtFrmHdr_t *v5;
    CachePool_t *Obj;
    Proto_t *proto;
    Obj_t *a1[ 6 ], *obj, *v48;
    int GridId, WeaponBase, ActionFrame, Flags, i, v36, v43, ImgId, v50, tmp;

    obj = NULL;
    v48 = 0;
    v50 = 0;
    ImgId = -1;
    memset( a1, 0, sizeof( a1 ) );
    AnimStart( 2 );
    AnimUnk33( 1 );
    ProtoGetObj( cmbt->HandEq->Pid, &proto );
    if( (v5 = ArtLoadImg( ArtMakeId(1, cmbt->Dude->ImgId & 0xFFF, a2, (cmbt->Dude->ImgId & 0xF000) >> 12, cmbt->Dude->Orientation + 1), &Obj )) )
        ActionFrame = ArtGetActionFrame( v5 );
    else
        ActionFrame = 0;
    ArtClose( Obj );
    ItemGetRange( cmbt->Dude, cmbt->Hand );
    WeaponBase = ItemGetWeaponBase( cmbt->Dude, cmbt->HandEq );
    TileGetTileNumInDir( cmbt->Dude->GridId, cmbt->Dude->Orientation, 1 );
    AnimUnk51( cmbt->Dude, cmbt->Comp->GridId );
    if( a2 == 18 ){
        if( WeaponBase == 6 || WeaponBase == 3 || WeaponBase == 5 ) v50 = 1;
    } else {
        AnimRegAnimate( cmbt->Dude, 43, -1 );
    }
    AiCombatTaunts( cmbt->Dude, cmbt, 2, 0 );
    if( (cmbt->Dude->ImgId & 0xF000) >> 12 ){
	AnimRegPlaySfx( cmbt->Dude, GSoundWeaponFileName( 1, cmbt->HandEq, cmbt->Hand, cmbt->Comp ), -1 );
    } else {
	AnimRegPlaySfx( cmbt->Dude, GSoundCharacterFileName( cmbt->Dude, a2, 0 ), -1 );
    }
    AnimRegAnimate( cmbt->Dude, a2, 0 );
    if( a2 != 47 ){
        if( ( cmbt->DudeInjuries & 0x100 ) || !( cmbt->DudeInjuries & 0x200 ) ){
            if( ProtoGetObj( Item59( cmbt->HandEq ), &proto ) == -1 || proto->ImgId == -1 ){
                if( !( cmbt->DudeInjuries & 0x100 ) ){
                    if( !( cmbt->Comp->Critter.State.CombatResult & 0x03 ) ){
                        AnimRegAnimate( cmbt->Comp, 13, ActionFrame );
                        goto LABEL_57;
                    }
                }
            } else {
                if( a2 == 18 ){
                    obj = cmbt->HandEq;
                    ImgId = cmbt->HandEq->ImgId;
                    Flags = cmbt->HandEq->Flags;
                    IfaceUnk16( &tmp, &v43 );
                    ItemUseItem( cmbt->Dude, cmbt->HandEq, 1 );
                    v48 = Item35( cmbt->Dude, cmbt->HandEq, Flags & 0x3000000 );
                    ObjSetShape( obj, proto->ImgId, 0 );
                    AiUnk44( cmbt->Dude, cmbt->HandEq );
                    if( gObjDude == cmbt->Dude ){
                        if( !v48 ){
                            if( (Flags & 0x3000000 & 0x1000000) != 0 ){
                                tmp = -1;
                            } else if ( (Flags & 0x3000000 & 0x2000000) != 0 ){
                                v43 = -1;
                            }
                        }
                        IfaceHandSlotUpdate( 0, tmp, v43 );
                    }
                    ObjUnk14( cmbt->HandEq, cmbt->Dude->GridId, cmbt->Dude->Elevation, 0 );
                } else {
                    ObjCreate( &obj, proto->ImgId, -1 );
                }
                ObjUnk33( obj, 0 );
                ObjSetLight( obj, 9, obj->LightIntensity, 0 );
                tmp = CombatUnk37( cmbt->Dude, cmbt->Comp );
                ObjMoveToTile( obj, tmp, cmbt->Dude->Elevation, NULL );
                ObjSetRotation( obj, TileTurnAt( cmbt->Dude->GridId, cmbt->Comp->GridId ), 0 );
                AnimUnk60( obj, 1, ActionFrame );
                AnimRegPlaySfx( obj, GSoundWeaponFileName( 3, cmbt->HandEq, cmbt->Hand, cmbt->Comp ), 0 );
                if( (cmbt->DudeInjuries & 0x100) != 0 ){
                    AnimUnk44( obj, cmbt->Comp->GridId, cmbt->Comp->Elevation, 1, 0 );
                    ActionFrame = AnimUnk06( obj, tmp, cmbt->Comp->GridId, 0, 0, 32 ) - 1;
                    GridId = cmbt->Comp->GridId;
                } else {
                    AnimUnk44( obj, cmbt->TileNo, cmbt->Comp->Elevation, 1, 0 );
                    GridId = cmbt->TileNo;
                    ActionFrame = 0;
                }
                if( v50 || WeaponBase == 6 ){
                    if( (cmbt->DudeInjuries & 0x40000) != 0 ) goto LABEL_58;
                    if( v50 ){
                        if( WeaponBase == 5 ){
                            tmp = 2;
                        } else {
                    	    tmp = ( WeaponBase == 3 ) ? 31 : 29;
                        }
                    } else {
                        tmp = 10;
                    }
                    if( v50 ) AnimUnk62( obj, ImgId, -1 );
                    tmp = ArtMakeId( 5, tmp, 0, 0, 0 );
                    AnimUnk62( obj, tmp, -1 );
                    AnimRegPlaySfx( obj, GSoundWeaponFileName( 4, cmbt->HandEq, cmbt->Hand, cmbt->Comp ), 0 );
                    AnimUnk50( obj, 0, 0 );
                    for( i = 0; i < 6; ++i ){
                        if( ObjCreate( a1 + i, tmp, -1 ) == -1 ) continue;
                        ObjUnk33( a1[ i ], 0 );
                        ObjMoveToTile( a1[ i ], TileGetTileNumInDir( GridId, i, 1 ), obj->Elevation, 0 );
                    	AnimUnk60( a1[ i ], 1, i ? 0 : ( ( WeaponBase == 3 ) ? 4 : 2 ) );
                        AnimUnk50( a1[ i ], 0, 0 );
                    }
                    goto LABEL_57;
                }
                if( a2 != 18 ) AnimUnk55( obj );
            }
            AnimRegPlaySfx( cmbt->HandEq, GSoundWeaponFileName( 4, cmbt->HandEq, cmbt->Hand, cmbt->Comp ), ActionFrame );
LABEL_57:
            ActionFrame = 0;
        }
    }
LABEL_58:
    ActionUnk31( cmbt, a2, ActionFrame );
    if( (cmbt->DudeInjuries & 0x100) == 0 ){
        AiCombatTaunts( cmbt->Comp, cmbt, 3, -1 );
    } else {
	if( !(cmbt->CompInjuries & 0x80 ) ) AiCombatTaunts( cmbt->Comp, cmbt, 4, -1 );	
    }
    if( obj && (v50 || WeaponBase == 6) ){
        AnimUnk55( obj );
    } else if( a2 == 18 && obj ){
        AnimUnk62( obj, ImgId, -1 );
    }
    for( i = 0; i != 6; i++ ){
        if( a1[ i ] ) AnimUnk55( a1[ i ] );
    }
    if( (cmbt->DudeInjuries & 0x83) == 0 ){
        if( a2 == 18 ){
            if( !v48 ){
                AnimUnk62( cmbt->Dude, ArtMakeId( 1, cmbt->Dude->ImgId & 0xFFF, 0, 0, cmbt->Dude->Orientation + 1 ), -1 );
            } else {
        	if( (v36 = Item58( v48 )) ){
            	    AnimUnk63( cmbt->Dude, v36, -1 );
        	} else {
        	    AnimUnk62( cmbt->Dude, ArtMakeId( 1, cmbt->Dude->ImgId & 0xFFF, 0, 0, cmbt->Dude->Orientation + 1 ), -1 );
        	}
            }
        } else {
            AnimRegAnimate( cmbt->Dude, 44, -1 );
        }
    }
    if( AnimBegin() == -1 ){
	eprintf( "Something went wrong with a ranged attack sequence!\n" );
        if( obj && (v50 || WeaponBase == 6 || a2 != 18) ) ObjDestroy( obj, 0 );
        for( i = 0; i != 6; i++ ){
            if( a1[ i ] ) ObjDestroy( a1[ i ], 0 );
        }
        return -1;
    } else {
        ActionUnk32( cmbt );
        return 0;
    }
}

int ActionUnk26( Obj_t *a1 )
{
    return ActionUseSceneryObject( gObjDude, a1, NULL );
}

int ActionReachable( Obj_t *obj1, Obj_t *obj2 )
{
    MsgLine_t msg;

    if( ObjGetDistance( obj1, obj2 ) <= 1 ) return 0;
    if( obj1 == gObjDude ){
        msg.Id = 2000; // misc.msg 'You cannot get there.'
        if( MessageGetMsg( &gMessage, &msg ) == 1 ) IfcMsgOut( msg.Text );
    }
    return -1;
}

int ActionLadderClimb( Obj_t *obj1, Obj_t *obj2 )
{
    int tmp, ap;
    short flgs;

    if( obj1 == gObjDude ) {
        tmp = (gObjDude->ImgId & 0xFF0000) >> 16;
        if( tmp == 1 || tmp == 19 ) AnimClear( gObjDude );
    }
    flgs = IN_COMBAT ? 2 : 1;
    ap = IN_COMBAT ? obj1->Critter.State.CurrentAP : -1;
    if( obj1 == gObjDude ) flgs = 2;
    AnimStart( flgs | 0x04 );
    tmp = TileGetTileNumInDir( obj2->GridId, 2, 1 );
    if( ap == -1 && ObjGetDistance( obj1, obj2 ) >= 5 )
        AnimObjRunToTile( obj1, tmp, obj2->Elevation, -1, 0 );
    else
        AnimObjMoveToTile( obj1, tmp, obj2->Elevation, ap, 0 );
    AnimSetFinish( obj1, obj2, ActionReachable, -1 );
    AnimUnk51( obj1, obj2->GridId );
    AnimSetCallback11( obj1, (AnimU_t)obj2, (void *)UseApUpdate, -1 );
    if( (tmp = (obj1->ImgId & 0xF000) >> 12) ){
        AnimRegPlaySfx( obj1, GSoundCharacterFileName( obj1, 39, 0 ), -1 );
        AnimRegAnimate( obj1, 39, 0 );
    }
    AnimRegPlaySfx( obj1, GSoundCharacterFileName( obj1, 4, 0 ), -1 );
    AnimRegAnimate( obj1, 4, 0 );
    AnimSetCallback11( obj1, (AnimU_t)obj2, (void *)UseObject, -1 );
    if( tmp ) AnimUnk63( obj1, tmp, -1 );
    return AnimBegin();
}

int ActionUseSceneryObject( Obj_t *dude, Obj_t *object, Obj_t *ap )
{
    int v9,CurrentAP,v13,v15,Type;
    Proto_t *proto;
DD
    Type = -1;
    if( OBJTYPE( object->ImgId ) == TYPE_SCEN ){
        if( ProtoGetObj( object->Pid, &proto ) == -1 ) return -1;
        Type = proto->Critt.Type;
    }
    if( (Type == PR_SCN_LADDER_BOT) && (ap == NULL) ) return ActionLadderClimb( dude, object );
    if( dude == gObjDude ){ // player
        v9 = (gObjDude->ImgId & 0xFF0000) >> 16;
        if( v9 == 1 || v9 == 19 ) AnimClear( gObjDude );
    }
    if( IN_COMBAT ){
        v9 = 2;
        CurrentAP = dude->Critter.State.CurrentAP;
    } else {
        v9 = 1;
        CurrentAP = -1;
    }
    if( dude == gObjDude ) v9 = 2;
    AnimStart( v9 );
    if( CurrentAP == -1 && ObjGetDistance( dude, object ) >= 5 )
        AnimObjRunToObj( dude, object, -1, 0 );
    else
        AnimObjMoveToObj( dude, object, CurrentAP, 0 );
    AnimSetFinish( dude, object, (void *)ActionReachable, -1 );
    if( !ap ) AnimSetCallback11(dude, (AnimU_t)object, (void *)UseApUpdate, -1);
    v15 = (dude->ImgId & 0xF000) >> 12;
    if( v15 ){
        AnimRegPlaySfx( dude, GSoundCharacterFileName(dude, 39, 0), -1 );
        AnimRegAnimate( dude, 39, 0 );
    }
    v9 = OBJTYPE( object->ImgId );
    if( v9 == TYPE_CRIT && CritterUnk31( object ) ){
        v13 = 10;
    } else if( v9 == TYPE_SCEN && (proto->FlgExt & 0x01) ) {
        v13 = 10;
    } else {
        v13 = 11;
    }
    if( Type != PR_SCN_STAIRS && !ap ) AnimRegAnimate( dude, v13, -1 );
    if( ap )
        AnimSetCallback12( dude, object, (AnimU_t)ap, (void *)UseObjOnObj, -1 );
    else
        AnimSetCallback11( dude, (AnimU_t)object, (void *)UseObject, -1 );
    if( v15 ) AnimUnk63( dude, v15, -1 );
    return AnimBegin();
}

int ActionUseOnScenery( Obj_t *obj1, Obj_t *obj2 )
{
    return ActionUseSceneryObject( obj1, obj2, NULL );
}

int ActionPlayerPickup( Obj_t *obj )
{
    return ActionPickupItem( gObjDude, obj );
}

int ActionPickupItem( Obj_t *Critter, Obj_t *Item )
{
    ArtFrmHdr_t *Img;
    Proto_t *proto;
    CachePool_t *ImgObj;
    char stmp[ 20 ];
    int ActionFrame, tmp;

    if( OBJTYPE( Item->ImgId ) != TYPE_ITEM ) return -1;
    if( Critter == gObjDude ){
        tmp = (gObjDude->ImgId & 0xFF0000) >> 16;
        if( tmp == 1 || tmp == 19 ) AnimClear( gObjDude );
    }
    if( IN_COMBAT ){
        AnimStart( 2 );
        AnimObjMoveToObj( Critter, Item, Critter->Critter.State.CurrentAP, 0 );
    } else {
	AnimStart( ( Critter == gObjDude ) ? 2 : 1 );
	if( ObjGetDistance( Critter, Item ) < 5 )
    	    AnimObjMoveToObj( Critter, Item, -1, 0 );
	else
	    AnimObjRunToObj( Critter, Item, -1, 0 );	
    }
    AnimSetFinish( Critter, Item, ActionReachable, -1 );
    AnimSetCallback11( Critter, (AnimU_t)Item, (void *)UseApUpdate, -1 );
    ProtoGetObj( Item->Pid, &proto );

    if( proto->Critt.Type != PR_ITEM_CONTAINER || ProtoItemAccessible( Item->Pid ) ){
        AnimRegAnimate( Critter, 10, 0 );
        if( (Img = ArtLoadImg( ArtMakeId(1, Critter->ImgId & 0xFFF, 10, (Critter->ImgId & 0xF000) >> 12, Critter->Orientation + 1 ), &ImgObj ) ) ){
            ActionFrame = ArtGetActionFrame( Img );
            ArtClose( ImgObj );
        } else {
            ActionFrame = -1;
        }
        if( !ArtGetFilenameFromList( OBJTYPE( Item->ImgId ), Item->ImgId & 0xFFF, &stmp[ 4 ] ) ){
            tmp = strlen( &stmp[ 4 ] );
            if( tmp >= 4 ){
                stmp[ tmp ] = '\0';
                AnimRegPlaySfx( Item, &stmp[ 4 ], ActionFrame );
                ActionFrame = 0;
            }
        }
	AnimSetCallback11( Critter, (AnimU_t)Item, (void *)UseUseOn, ActionFrame );
    } else { // container
DD
        if( (tmp = (Critter->ImgId & 0xF000) >> 12) ){
            AnimRegPlaySfx( Critter, GSoundCharacterFileName( Critter, 39, 0 ), -1 );
            AnimRegAnimate( Critter, 39, -1 );
        }
        AnimRegAnimate( Critter, ((proto->Critt.BaseStat[1] & 1) == 0) + 10, 0 );
        if( (Img = ArtLoadImg( ArtMakeId( 1, Critter->ImgId & 0xFFF, ((proto->Critt.BaseStat[1] & 1) == 0) + 10, 0, Critter->Orientation + 1 ), &ImgObj )) ){
            ArtGetActionFrame( Img );
            ArtClose( ImgObj );
        }
        if( Item->FrameNo != 1 ) AnimSetCallback11( Critter, (AnimU_t)Item, (void *)UseContainer, -1 );
        if( tmp ) AnimUnk63( Critter, tmp, -1 );
        if( Item->FrameNo > 1 ) return AnimBegin();
	AnimSetCallback11( Critter, (AnimU_t)Item, ScptUnk113, -1 );
    }
    return AnimBegin();
}

int ActionUseOnCritter( Obj_t *a1, Obj_t *a2 )
{
    int v5;
DD
    if( OBJTYPE( a2->ImgId ) != TYPE_CRIT ) return -1;
    if( a1 == gObjDude ){
        v5 = (gObjDude->ImgId & 0xFF0000) >> 16;
        if ( v5 == 1 || v5 == 19 ) AnimClear( gObjDude );
    }
    if( IN_COMBAT ){
        AnimStart( 2 );
        AnimObjMoveToObj( a1, a2, a1->Critter.State.CurrentAP, 0 );
    } else {
	AnimStart( ( a1 == gObjDude ) ? 2 : 1 );
	if( ObjGetDistance( a1, a2 ) < 5 ){
    	    AnimObjMoveToObj( a1, a2, -1, 0 );
	} else {
	    AnimObjRunToObj( a1, a2, -1, 0 );
	}
    }
    AnimSetFinish( a1, a2, (void *)ActionReachable, -1 );
    AnimSetCallback11( a1, (AnimU_t)a2, (void *)UseApUpdate, -1 );
    AnimSetCallback11(a1, (AnimU_t)a2, (void *)ScptUnk113, -1);
    return AnimBegin();
}

int ActionUseSneak( int SkillId )
{
DD
    if( SkillId != SKILL_SNEAK ) return -1;
    AnimClear( gObjDude );
    CritterUnk38( 0 );
    return 0;
}

#define YOU_CANNOT_DO_THAT_IN_COMBAT( obj )	\
                if( obj == gObjDude ){\
                    msg.Id = 902; \
                    if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );\
                }\
                return -1;

int ActionUnk20( Obj_t *obj )
{
    MsgLine_t msg;

    YOU_CANNOT_DO_THAT_IN_COMBAT( obj )
}


int ActionSkillUse( Obj_t *dude, Obj_t *target, int SkillNo )
{
    Obj_t *v4,*v8;
    ArtFrmHdr_t *Img;
    MsgLine_t msg;
    VidRect_t Area;
    CachePool_t *ImgObj;
    int v11;
printf("=Target=>0x%x %i\n", target->Pid, target->Pid );
    v4 = gObjDude;
    switch( SkillNo ){
        case SKILL_FIRSTAID:
        case SKILL_DOCTOR:
            if( IN_COMBAT ){
        	YOU_CANNOT_DO_THAT_IN_COMBAT( dude )
            } else {
                if( OBJTYPE( target->Pid ) == TYPE_CRIT ) break;
                return -1;
            }
        case SKILL_SNEAK:
            CritterUnk38( 0 );
            return 0;
        case SKILL_LOCKPICK:
            if( IN_COMBAT ){
        	YOU_CANNOT_DO_THAT_IN_COMBAT( dude )
            } else {
                v11 = OBJTYPE( target->Pid );
                if( !v11 || v11 == 2 ) break;
                return -1;
            }
        case SKILL_STEAL:
            if( IN_COMBAT ){
        	YOU_CANNOT_DO_THAT_IN_COMBAT( dude )
            } else if( OBJTYPE( target->Pid ) < 2 ){
                if( target != dude ) break;
                return -1;
            } else {
                return -1;
            }
        case SKILL_TRAPS:
            if( IN_COMBAT ){
        	YOU_CANNOT_DO_THAT_IN_COMBAT( dude )
            } else {
                if( OBJTYPE( target->Pid ) != 1 ) break;
                return -1;
            }
        case SKILL_SCIENCE: 
        case SKILL_REPAIR:
            if( IN_COMBAT ){
        	YOU_CANNOT_DO_THAT_IN_COMBAT( dude )
            } else {
                if( OBJTYPE( target->Pid ) != TYPE_CRIT || CritterGetGender( target ) == 10 || ( CritterGetGender( target ) == 5 && SkillNo == 12 ) ) break;
                return -1;
            }
        default: eprintf( "\nskill_use: invalid skill used." ); break;
    }
    if( dude == gObjDude ){
        v8 = PartyGetBestSkilled( SkillNo );
	if( v8 == gObjDude ) v8 = NULL;
	if( SkillNo == SKILL_STEAL ) v8 = NULL;
	if( v8 ){
	    if( PartyBestSkill( v8 ) != SkillNo ){
    		v8 = 0;
	    } else {
    		v11 = (v8->ImgId & 0xFF0000) >> 16;
		v4 = v8;
		if( v11 == 1 || v11 == 19 ){
    		    AnimClear( v8 );
		} else if( v11 ){
    		    v4 = gObjDude;
    	    	    v8 = NULL;
		}
	    }
	}
	if( v8 ){
	    v11 = ObjGetDistance( gObjDude, target ) <= 1;
    	    if( !TextFloatMessage( v8, SkillUseTryWantMsg( v11 ), 101, gPalColorCubeRGB[31][31][11], gPalColorCubeRGB[0][0][0], &Area) ) TileUpdateArea( &Area, gCurrentMapLvl );
    	    if( v11 ){
        	v4 = gObjDude;
    		v8 = NULL;
    	    }
	}
	if( !v8 ){
    	    v11 = (v4->ImgId & 0xFF0000) >> 16;
    	    if( v11 == 1 || v11 == 19 ) AnimClear(v4);
	}
    }
    if( IN_COMBAT ){
        AnimStart( 2 );
        AnimObjMoveToObj( v4, target, v4->Critter.State.CurrentAP, 0 );
    } else { 
        AnimStart( ( dude == gObjDude ) ? 2 : 1 );
        if( target != gObjDude ){
            if( ObjGetDistance( v4, target ) >= 5 ){
            	AnimObjRunToObj( v4, target, -1, 0 );
            } else {
            	AnimObjMoveToObj( v4, target, -1, 0 );
            }
        }
    }
    AnimSetFinish( v4, target, ActionReachable, -1 );
    v11 = ( OBJTYPE( target->ImgId ) == TYPE_CRIT && CritterUnk31( target ) ) ? 10 : 11;
    if( (Img = ArtLoadImg( ArtMakeId( 1, v4->ImgId & 0xFFF, v11, 0, v4->Orientation + 1 ), &ImgObj ) ) ){
        ArtGetActionFrame( Img );
        ArtClose( ImgObj );
    }
    AnimRegAnimate( v4, v11, -1 );
    AnimSetCallback12( v4, target, (AnimU_t)SkillNo, (void *)UseUseSkill, -1 );
    return AnimBegin();
}

Obj_t *ActionUnk18( int a1 )
{
    Obj_t *Object;
    char Buttons;
    int v4;

    Object = 0;
    do
        InpUpdate();
    while( MseGetButtons() & 0x04 );
    if( a1 != -1 && a1 != 1 ) a1 = 0;
    GmouseLoadCursor( 21 );
    GmouseUnk03();
    v4 = KEY_ESC;
    do{
	if( InpUpdate() == -2 ){
	    Buttons = MseGetButtons();
	    if( (Buttons & 0x10) == 0 ){
    		if( Buttons & 0x02 ){
    		    v4 = 0;
    		    break;
    		}
	    } else {
		Object = GmouseGetObject( a1, 0, gCurrentMapLvl );
		v4 = 0;
		break;
	    }
	}
    } while( !gMenuEscape );    
    GmouseLoadCursor( 1 );
    GmouseIsoEnter();
    return ( v4 == KEY_ESC ) ? NULL : Object;
}

int ActionUnk17() // no xref
{
    VidRect_t area;
    int sel;

    while( (sel = InpUpdate()) != -2 ){
        if( sel == 372 ){
            if( ++gActionUnk05 > 5 ) gActionUnk05 = 0;
            ObjSetRotation( gGmouseObjA, gActionUnk05, &area );
            TileUpdateArea( &area, gGmouseObjA->Elevation );
        }
        if( sel == 371 ){
            if( --gActionUnk05 == -1 ) gActionUnk05 = 5;
            ObjSetRotation( gGmouseObjA, gActionUnk05, &area );
            TileUpdateArea( &area, gGmouseObjA->Elevation );
        }
        if( sel == 329 || sel == 337 ){
            MapSetLvl( ( sel == 329 ) ? gCurrentMapLvl + 1 : gCurrentMapLvl - 1 );
            area.tp = 62;
            area.rt = 50;
            area.bm = 88;
            area.lt = 30;
        }
        if( gMenuEscape ) return -1;
    }
    if( !( MseGetButtons() & 0x01 ) ) return -1;
    if( !MseCursorCenterInArea( 0, 0, gVidMainGeo.rt - gVidMainGeo.lt, gVidMainGeo.bm - gVidMainGeo.tp - 100 ) ) return -1;
    MseGetCursorPosition( &area.lt, &area.tp );
    if( (sel = TileGetPointed( area.lt, area.tp )) == -1 ) return -1;
    return sel;    
}

int ActionUnk16( Obj_t *a1, Obj_t *a2 )
{
    int v2;

    v2 = a1->Orientation - a2->Orientation;
    if( v2 < 0 ) v2 = -v2;
    return v2 >= 2 && v2 != 5;
}

int ActionTurnAt( Obj_t *obj1, Obj_t *obj2 )
{
    int k;

    k = obj1->Orientation - TileTurnAt( obj1->GridId, obj2->GridId );
    if( k < 0 ) k = -k;
    return k <= 1 || k == 5;
}

int ActionUnk14( Obj_t *obj, int a2 )
{
    int i;

    if( a2 == 21 ){
        i = 1;
        do{
            if( ObjReach( obj, TileGetTileNumInDir(obj->GridId, obj->Orientation, i ), obj->Elevation ) ){
        	a2 = 20;
        	break;
            }
        }while( ++i <= 2 );
    } else if( a2 == 20 ){
        i = 1;
        do{
            if( ObjReach( obj, TileGetTileNumInDir( obj->GridId, (obj->Orientation + 3) % 6, i ), obj->Elevation ) ){
    		a2 = 21;
        	break;
            }
        } while( ++i <= 2 );
    }
    if( a2 == 21 ){
        if( !ArtFileExist( ArtMakeId(1, obj->ImgId & 0xFFF, 21, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1) ) ) return 20;
    }
    return a2;
}

int ActionUnk13()
{
    return gActionUnk06 != 0;
}

int ActionExplode( int GridPos, int MapLvl, int DmgMin, int DmgMax, Obj_t *Target, int SelfHurt )
{
    Combat_t *cmbt;
    Obj_t *obj[ 6 ], *TmpObj, *p;
    int j, i;

    if( SelfHurt && gActionUnk06 ) return -2;
    if( !(cmbt = Malloc( sizeof( Combat_t ) )) ) return -1;
    if( ObjCreate( &TmpObj, ArtMakeId( 5, 10, 0, 0, 0 ), -1 ) == -1 ){ Free( cmbt ); return -1; }
    ObjUnk33( TmpObj, 0 );
    TmpObj->Flags |= 0x04;
    ObjMoveToTile( TmpObj, GridPos, MapLvl, 0 );
    for( i = 0; i < 6; i++ ){
        if( ObjCreate( &obj[ i ], ArtMakeId( 5, 10, 0, 0, 0 ), -1 ) == -1 ) break;
        ObjUnk33( obj[ i ], 0 );
        obj[ i ]->Flags |= 0x04;
        ObjMoveToTile( obj[ i ], TileGetTileNumInDir( GridPos, i, 1 ), MapLvl, 0 );
    }
    if( i >= 6 ){
	p = ObjReach( 0, GridPos, MapLvl );
	if( p && (( p->ImgId ) != 1 || ( p->Critter.State.CombatResult < 0) ) ) p = NULL;
	CombatSetUp( cmbt, TmpObj, p, 4, 3 );
	cmbt->TileNo = GridPos;
	cmbt->DudeInjuries = 256;
	GameIfaceDisable( 1 );
	if( p ){
    	    if( AnimClear( p ) == -2 ) eprintf( "Cannot clear target's animation for action_explode!\n" );
    	    cmbt->CompDmg = ActionGetExplosionDmg( DmgMin, DmgMax, p, &cmbt->CompKnockDown );
	}
	CombatExplosion( cmbt, 0, 0, 1 );
	for( i = 0; i < cmbt->Count; i++ ){
    	    if( AnimClear( cmbt->obj[ i ] ) == -2 ) eprintf( "Cannot clear extra's animation for action_explode!\n" );
    	    cmbt->Damage[ i ] = ActionGetExplosionDmg( DmgMin, DmgMax, cmbt->obj[ i ], &cmbt->KnockDown[ i ] );
	}
        CombatKillUpdate( cmbt );
        if( !SelfHurt ){
            if( p && (cmbt->CompInjuries & 0x80) ) CritterKill( p, -1, 0 );
            for( i = 0; i < cmbt->Count; i++ ){
                if( cmbt->Injuries[ i ] & 0x80 ) CritterKill( cmbt->obj[ i ], -1, 0 );
            }
            ActionUnk11( cmbt, Target );
        }
        gActionUnk06 = 1;
        AnimStart( 2 );
        AnimUnk33( 1 );
        AnimRegPlaySfx( TmpObj, "whn1xxx1", 0 );
        AnimUnk60( TmpObj, 1, 0 );
        AnimUnk50( TmpObj, 0, 0 );        
        ActionUnk31( cmbt, 0, 1 );
        for( i = 0; i != 6; i++ ){
            AnimUnk60( obj[ i ], 1, 0 );
            AnimUnk50( obj[ i ], 0, 0 );
        }
        AnimSetFinish( TmpObj, 0, (void *)CombatUnk77, -1 );
        AnimUnk55( TmpObj );
        for( i = 0; i != 6; i++ ){
            AnimUnk55( obj[ i ] );
        }
        AnimSetFinish( cmbt, Target, (void *)ActionUnk11, -1 );
        AnimSetFinish( 0, 0, (void *)ActionUnk10, -1 );
        if( AnimBegin() == -1 ){            
            gActionUnk06 = 0;
            ObjDestroy( TmpObj, 0 );
            for( i = 0; i != 6; i++ ){
                ObjDestroy( obj[ i ], NULL );
            }
            Free( cmbt );
            GameIfaceEnable();
            return -1;
        } else {
            ActionUnk32( cmbt );
            return 0;
        }
    } else {
        for( j = 0; j < i; j++ ){
            ObjDestroy( obj[ j ], 0 );
        }
        ObjDestroy( TmpObj, 0 );
        Free( cmbt );
        return -1;
    }    
}

void ActionUnk11( Combat_t *cmbt, Obj_t *obj )
{
    Obj_t *p;
    int i, DeadPool[ 6 ], dead, Exp;
    Scpt01_t v17;

    p = NULL;
    dead = ( cmbt->Comp ) ? ( cmbt->Comp->Critter.State.CombatResult & 0x80 ) : 0;    
    for( i = 0; i < cmbt->Count; i++ ){
        DeadPool[ i ] = cmbt->obj[ i ]->Critter.State.CombatResult & 0x80;
    }
    CombatKillUpdate( cmbt );
    CombatHitInfo( cmbt );
    Exp = 0;
    CombatUnk54( cmbt, 0 );
    if( obj ){
        if( cmbt->Comp && obj != cmbt->Comp ){
            if( !( cmbt->Comp->Critter.State.CombatResult & 0x80 ) ){
                CritterUnk45( cmbt->Comp, obj );
		p = cmbt->Comp;
            } else if( obj == gObjDude && !dead ){
                Exp = CritterUnk27( cmbt->Comp );
            }
        }        
        for( i = 0; i < cmbt->Count; i++ ){
            if( obj == cmbt->obj[ i ] ) continue;
            if( !( cmbt->obj[ i ]->Critter.State.CombatResult & 0x80 ) ){
                CritterUnk45( cmbt->obj[ i ], obj );
                if( !p ) p = cmbt->obj[ i ];
            } else if( obj == gObjDude && !DeadPool[ i ] ){
                Exp += CritterUnk27( cmbt->obj[ i ] );
            }
        }
        if( p && !IN_COMBAT ){
            v17.crit = obj;
            v17.obj = p;
            memset( &v17.i03, 0, 16 );
            v17.i07 = 0x7FFFFFFF;
            v17.i08 = 0;
            ScptUnk121( &v17 );
        }
    }
    Free( cmbt );
    GameIfaceEnable();
    if( obj == gObjDude ) CombatEarnExpPts( Exp );
}

void ActionUnk10()
{
    gActionUnk06 = 0;
}

int ActionGetExplosionDmg( int min, int max, Obj_t *obj, int *pVal )
{
    int dmg;

    dmg = RandMinMax( min, max ) - FeatGetVal( obj, 23 ); // damage threshold explosive
    if( dmg > 0 ) dmg -= FeatGetVal( obj, 30 ) * dmg / 100; // damage resistance explosive
    if( dmg <= 0 ) dmg = 0;
    if( pVal && (obj->Flags & 0x800) == 0 ) *pVal = dmg / 10;
    return dmg;
}

int ActionTalk( Obj_t *Crit1, Obj_t *Crit2 )
{
    int tmp;

    if( Crit1 != gObjDude || OBJTYPE( Crit2->ImgId ) != TYPE_CRIT ) return -1;
    tmp = (gObjDude->ImgId & 0xFF0000) >> 16;
    if( tmp == 1 || tmp == 19 ) AnimClear( gObjDude );
    if( IN_COMBAT ){
        AnimStart( 2 );
        AnimObjMoveToObj( Crit1, Crit2, Crit1->Critter.State.CurrentAP, 0 );
    } else {
        tmp = ( Crit1 == gObjDude ) ? 2 : 1;
        AnimStart( tmp );
        if( ObjGetDistance( Crit1, Crit2 ) >= 9 || CombatBlockedAim( Crit1, Crit1->GridId, Crit2->GridId, Crit2, 0 ) ) AnimObjRunToObj( Crit1, Crit2, -1, 0 );
    }
    AnimSetFinish( Crit1, Crit2, (void *)ActionSndACb, -1 );
    AnimSetCallback11( Crit1, (AnimU_t)Crit2, (void *)ActionSndBCb, -1 );
    return AnimBegin();
}

int ActionSndACb( Obj_t *Critter, Obj_t *Target )
{
    MsgLine_t msg;

    if( !CombatBlockedAim( Critter, Critter->GridId, Target->GridId, Target, 0 ) && ObjGetDistance( Critter, Target ) < 9 ) return 0;
    if( Critter == gObjDude ){
        msg.Id = 2000; // 'You cannot get there.'
        if( MessageGetMsg( &gMessage, &msg ) == 1 ) IfcMsgOut( msg.Text );
    }
    return -1;
}

void ActionSndBCb( Obj_t *a1 )
{
    ScptUnk115( a1 );
}

void ActionUnk05( int a1, int edx0, int a3, int a4, int a5, int a6, int a7 )
{
    Combat_t *cmbt;
    Obj_t *TragetObj, *TmpObj;

    cmbt = Malloc( sizeof( Combat_t ) );
    if( !cmbt ) return;
    if( ObjCreate( &TmpObj, 0x20001F5, -1 ) == -1 ){ Free( cmbt ); return; }
    ObjUnk33( TmpObj, 0 );
    TmpObj->Flags |= 0x04;
    ObjMoveToTile( TmpObj, a1, edx0, 0 );
    TragetObj = ObjReach( 0, a1, edx0 );
    CombatSetUp( cmbt, TmpObj, TragetObj, 4, 3 );
    cmbt->TileNo = a1;
    cmbt->DudeInjuries = 256;
    GameIfaceDisable( 1 );
    if( TragetObj ){
        AnimClear( TragetObj );
        if( a7 )
            cmbt->CompDmg = a4;
        else
            cmbt->CompDmg = ActionUnk04( a3, a4, TragetObj, &cmbt->CompKnockDown, a5 );
    }
    CombatKillUpdate( cmbt );
    if( a6 ){
        AnimStart( 2 );
        AnimRegPlaySfx( TmpObj, "whc1xxx1", 0 );
        ActionUnk31( cmbt, gActionUnk03[a5], 0 );
        AnimSetFinish( cmbt, 0, (void *)ActionUnk100, 0 );
        AnimUnk55( TmpObj );
        if( AnimBegin() != -1 ) return;
        ObjDestroy( TmpObj, 0 );
        Free( cmbt );
    } else {
        if( TragetObj ){
            if( cmbt->CompInjuries & 0x80 ) CritterKill( TragetObj, -1, 1 );
        }
        CombatHitInfo( cmbt );
        CombatUnk54( cmbt, 0 );
        Free( cmbt );
        GameIfaceEnable();
        ObjDestroy( TmpObj, 0 );
    }
    GameIfaceEnable();    
}

int ActionUnk100( Combat_t *cmbt )
{
    CombatHitInfo( cmbt );
    CombatUnk54( cmbt, 0 );
    Free( cmbt );
    GameIfaceEnable();
    return 0;
}

int ActionUnk04( int Min, int Max, Obj_t *obj, int *a4, int a5 )
{
    int tmp;

    if( CritterGetInjure( obj->Pid, 0x4000 ) ) a4 = NULL;
    tmp = RandMinMax( Min, Max ) - FeatGetVal( obj, a5 + 17 );
    if( tmp > 0 ) tmp -= FeatGetVal( obj, a5 + 24 ) * tmp / 100;
    if( tmp <= 0 ) tmp = 0;
    if( a4 && (obj->Flags & 0x800) == 0 && a5 != 4 ) *a4 = tmp / 10;
    return tmp;
}

int ActionUnk03( Obj_t *a1, Obj_t *a2 )
{
//    int GroupId;
    ObjCritterCond_t *p_State;
    int WhoHitMe;
DD
    if( OBJTYPE( a2->ImgId ) != TYPE_CRIT ) return 0;
    if( a1 == a2 ) return 0;
    if( !CritterCanTalk( a2 ) ) return 0;
    if( ActionFindPath( a1, a2 ) ) return 0;
    if( !ScptUnk108( a2->ScrId, 24 ) ) return 0;
    if( !IN_COMBAT ) return 1;
//    GroupId = a2->Critter.State.GroupId;
    p_State = &a2->Critter.State;
//    if( GroupId == a1->Critter.State.GroupId && a1 == p_State->WhoHitMe ) return 0;
    if( !(WhoHitMe = p_State->WhoHitMe) ) return 1;
//    return WhoHitMe->Critter.State.GroupId != a1->Critter.State.GroupId;    
return 0;
}

int ActionUnk02( Obj_t *a1, Obj_t *a2 )
{
    int v6;
    int v8;
    int i18;
    int Id;
    Scpt_t *scr;

    if( !ActionUnk03( a1, a2 ) ) return -1;
    if( UseGetScriptId( a2, &Id ) != -1 ){
        ScptSetup( Id, a1, a2 );
        ScptRun( Id, SCPT_AEV_PUSH_P_PROC );
        if( ScptPtr( Id, &scr ) != -1 ) i18 = scr->OverrideFlag;
        if( i18 ) return -1;
    }
    v6 = TileTurnAt( a1->GridId, a2->GridId );
    v8 = TileGetTileNumInDir( i18, v6, 1 );
    if( ObjReach(a2, v8, a2->Elevation ) ){
        v8 = TileGetTileNumInDir( i18, (v6 + 1) % 6, 1 );
        if( ObjReach( a2, v8, a2->Elevation ) ){
            v8 = TileGetTileNumInDir(i18, (v6 + 5) % 6, 1);
            if( ObjReach(a2, v8, a2->Elevation) ){
                v8 = TileGetTileNumInDir(i18, (v6 + 2) % 6, 1);
                if( ObjReach(a2, v8, a2->Elevation) ){
                    v8 = TileGetTileNumInDir(i18, (v6 + 4) % 6, 1);
                    if( ObjReach(a2, v8, a2->Elevation) ){
                        v8 = TileGetTileNumInDir(i18, (v6 + 3) % 6, 1);
                        if( ObjReach( a2, v8, a2->Elevation ) ) return -1;
                    }
                }
            }
        }
    }
    AnimStart(2);
    AnimUnk51(a2, v8);
    AnimObjMoveToTile(a2, v8, a2->Elevation, i18, 0);
    return AnimBegin();
}

int ActionFindPath( Obj_t *Crit, Obj_t *Target )
{
    if( !AnimMakeTrace( Crit, Crit->GridId, Target->GridId, 0, 0, (void *)ObjUnk58 ) ) return -1;
    if( TileGetDistance( Crit->GridId, Target->GridId ) <= 12 ) return 0;
    return -2;
}

