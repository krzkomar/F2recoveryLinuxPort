#include "FrameWork.h"

int gAnimCnt1 = 0;
int gAnimIdx = -1;
int gAnim0 = 0;
int gAnimUnk41 = 0;
int gAnimUnk40 = 0;
int gAnimRunPos = -2;
int gAnimPlayTime = 0;
int gAnimPlaySpeed = 0;
Anim03_t gAnimUnk23[ 24 ];
Anim_t gAnimations[ 32 ];
int gAnimSubIdx;
Obj_t *gAnimAmbientCritter[ 100 ];

/***********************************************/

int AnimInit()
{
    gAnim0 = 1;
    AnimInit_00();
    gAnim0 = 0;
    return 0;
}

void AnimInit_00()
{
    int i;

    i = gAnim0;
    if( !gAnim0 ){
        gAnimIdx = -1;
        gAnimUnk41 = 1;
        for( ; i < 32; i++) AnimEnd( i );
        gAnimUnk41 = 0;
    }
    
    gAnimCnt1 = 0;
    gAnimIdx = -1;
    for( i = 0; i < 32; i++ ){
        gAnimations[ i ].i01 = -1000;
        gAnimations[ i ].Flags = 0x00;
    }

}

void AnimClose()
{
    int i;

    gAnimUnk41 = 1;
    gAnimIdx = -1;
    for( i = 0; i < 32; i++ ) AnimEnd( i );
    gAnimUnk41 = 0;
    gAnimCnt1 = 0;
}

int AnimRegStart( int Type )
{
    int Idx;
printf("Animate reg start: %x\n", Type);
    if( gAnimIdx != -1 || gAnimUnk41 ) return -1;
    if( ( Idx = AnimReserve( Type ) ) == -1 ) return -1;
    gAnimations[ Idx ].Flags |= 0x08;
    if( Type & 0x0002 ) gAnimations[ Idx ].Flags |= 0x04;
    if( Type & 0x0200 ) gAnimations[ Idx ].Flags |= 0x40;
    if( Type & 0x0004 ) gAnimations[ Idx ].Flags |= 0x80;
    gAnimIdx = Idx;
    gAnimSubIdx = 0;    
    return 0;
}

int AnimReserve( int flg )
{
    int Idx, n, i;

    Idx = -1;
    n = 0;
    for( i = 0; i < 32; i++ ){
        if( ( gAnimations[ i ].i01 != -1000 ) || ( gAnimations[ i ].Flags & 0x08 ) || ( gAnimations[ i ].Flags & 0x20 ) ){
            if( !( gAnimations[ i ].Flags & 0x04 ) ) n++;
            continue;
        }
        if( Idx == -1 && ( !(flg & 0x100) || !( gAnimations[ i ].Flags & 0x10 ) ) ) Idx = i;
    }
    if( Idx == -1 ){
        if( flg & 0x02 ) eprintf( "Unable to begin reserved animation!\n" );
        return -1;
    }
    if( !(flg & 0x02) && n >= 20 ) return -1;
    return Idx;
}

int AnimUnk33( int a1 )
{
    if( gAnimIdx == -1 ) return gAnimIdx;
    if( !a1 ) return -1;
    gAnimations[ gAnimIdx ].Flags |= 0x01;
    return 0;
}

int AnimRegClear( Obj_t *obj )
{
    int i, j;

    for( i = 0; i < 32; i++ ){
        if( gAnimations[ i ].i01 == -1000 ) continue;
	for( j = 0; j < gAnimations[ i ].Steps; j++ ){
	    if( !(obj != gAnimations[ i ].i05[ j ].Target.Obj || gAnimations[ i ].i05[ j ].State == 11 ) ){
		if( (gAnimations[ i ].Flags & 1) != 0 ) return -2;
		AnimEnd( i );
		return 0;
	    }    	    
	}
    }
    return -1;
}

int AnimRegEnd()
{
    int tmp;
printf("Animate reg end\n");
    if( gAnimIdx == -1 ) return -1;
    gAnimations[ gAnimIdx ].i01 = 0;
    gAnimations[ gAnimIdx ].Steps = gAnimSubIdx;
    gAnimations[ gAnimIdx ].Step = -1;
    tmp = (gAnimSubIdx & ~0xff) | (gAnimations[ gAnimIdx ].Flags & 0xff);
    gAnimations[ gAnimIdx ].Flags = (gAnimations[ gAnimIdx ].Flags & ~0x000ff ) | ( tmp & 0xF7 );
    gAnimations[ gAnimIdx ].Flags = (gAnimations[ gAnimIdx ].Flags & ~0x0ff00) | (gCombatStatus << 8);
    gAnimations[ gAnimIdx ].i05[0].i07 = 0;
    if( tmp & 0x100 ){
        CombatUnk61();
        gAnimations[ gAnimIdx ].Flags |= 0x02;
    }
    tmp = gAnimIdx;
    gAnimIdx = -1;
    if( ( gAnimations[ tmp ].Flags & 0x10 ) == 0 ) AnimUpdate( tmp, 1 );
    return 0;
}

int AnimTestExist( int Id, CachePool_t **Img )
{
    *Img = NULL;
    if( !ArtLoadImg( Id, Img ) ) return -1;
    ArtClose( *Img );
    *Img = NULL;
    return 0;
}

void AnimStop()
{
    Anim01_t *anim;
    int i;

    if( gAnimIdx == -1 ) return;
    for( i = 0; i < 32; i++ ) gAnimations[ i ].Flags &= ~0x18;    
    for( i = 0; i < gAnimSubIdx; i++ ){
        anim = &gAnimations[ gAnimIdx ].i05[ i ];
        if( anim->ImgObj ) ArtClose( anim->ImgObj );
        if( anim->State == 11 && anim->Callback11 == GSoundDbgPlayA ) GSoundSfxDelete( anim->GpPtr );
    }
    gAnimIdx = -1;    
}

int AnimCancel( Obj_t *obj )
{
    int i, j;

    if( gAnimIdx == -1 || gAnimSubIdx >= 55 ) return -1;
    if( !obj ) return 0;    
    for( i = 0; i < 32; i++ ){
        if( i == gAnimIdx || gAnimations[ i ].i01 == -1000 ) continue;
        for( j = 0; j < gAnimations[ i ].Steps; j++ ){
            if( (obj == gAnimations[ i ].i05[ j ].Target.Obj) && (gAnimations[ i ].i05[ j ].State != 11) ){
                if( (gAnimations[ i ].Flags & 0x40) == 0 ) return -1;
                AnimEnd( i );
            }
        }        
    }    
    return 0;
}

int AnimUnk39( Obj_t *a1 )
{
    int i, j; 

    if( gAnimSubIdx >= 55 || !a1 ) return 0;    
    for( i = 0; i < 32; i++ ){
        if( i == gAnimIdx || gAnimations[ i ].i01 == -1000 ) continue;        
	for( j = 0; j < gAnimations[ i ].Steps; j++ ){
	    if( a1 == gAnimations[ i ].i05[ j ].Target.Obj && gAnimations[ i ].i05[ j ].State != 11 && ( gAnimations[ i ].Steps != 1 || gAnimations[ i ].i05[ j ].Silence ) ) return -1;
	}	    
    }
    return 0;
}

int AnimObjMoveToObj( Obj_t *Object, Obj_t *DstObj, int Ap, int a4 )
{
    Anim01_t *anim;

    if( AnimCancel( Object ) == -1 || !Ap ){ AnimStop(); return -1; }
    if( DstObj->GridId == Object->GridId ){
        if( Object->Elevation == DstObj->Elevation ) return 0;
    }
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 0;
    anim->Silence = 1;
    anim->Target.Obj = Object;
    anim->GpPtr = DstObj;
    anim->Ap.Int = Ap;
    anim->i07 = a4;
    anim->ImgObj = NULL;
    if( ArtLoadImg( ArtMakeId( OBJTYPE( Object->ImgId ), Object->ImgId & 0xFFF, anim->Silence, (Object->ImgId & 0xF000) >> 12, Object->Orientation + 1), &anim->ImgObj ) ){
        ArtClose( anim->ImgObj );
        anim->ImgObj = NULL;
    } else {
        AnimStop();
        return -1;
    }
    gAnimSubIdx++;
    return AnimUnk51( Object, DstObj->GridId );
}

int AnimObjRunToObj( Obj_t *obj1, Obj_t *obj2, int Ap, int a4 )
{
    Anim01_t *anim;
    char stmp[ 100 ];
    MsgLine_t Line;

    if( AnimCancel( obj1 ) == -1 || !Ap ){ AnimStop(); return -1; }
    if( obj2->GridId == obj1->GridId ){
        if( obj1->Elevation == obj2->Elevation ) return 0;
    }
    if( CritterOverload( obj1 ) ){
        if( PartyMembRdy( obj1 ) ){
            if( obj1 == gObjDude ){
                strcpy( stmp, MessageGetMessage( &gMessage, &Line, 8000 ) ); // 'You are overloaded.'
            } else {
                sprintf( stmp, MessageGetMessage( &gMessage, &Line, 8001 ), CritterGetName( obj1 ) ); // '%s is overloaded.'
            }
            IfcMsgOut( stmp );
        }
        return AnimObjMoveToObj( obj1, obj2, Ap, a4 );
    }
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 0;
    anim->Target.Obj = obj1;
    anim->GpPtr = obj2;
    if( OBJTYPE( obj1->ImgId ) == 1 && ( obj1->Critter.State.CombatResult & ( 0x08 | 0x04 )) ){
        anim->Silence = 1;
    } else {
        if( obj1 == gObjDude && CritterUsingSkill( 0 ) && !PerkLvl( gObjDude, PERK_SILENT_RUNNING ) ){
            anim->Silence = 1;
        } else {
    	    anim->Silence = ( !ArtFileExist( ArtMakeId( OBJTYPE( obj1->ImgId ), obj1->ImgId & 0xFFF, 19, 0, obj1->Orientation + 1 ) ) ) ? 1 : 19;
        }
    }
    anim->Ap.Int = Ap;
    anim->i07 = a4;
    anim->ImgObj = NULL;
    if( ArtLoadImg( ArtMakeId( OBJTYPE( obj1->ImgId ), obj1->ImgId & 0xFFF, anim->Silence, (obj1->ImgId & 0xF000) >> 12, obj1->Orientation + 1 ), &anim->ImgObj ) ){
        ArtClose( anim->ImgObj );
        anim->ImgObj = NULL;
    } else {
        AnimStop(); return -1;
    }
    gAnimSubIdx++;
    return AnimUnk51( obj1, obj2->GridId );
}

int AnimObjMoveToTile( Obj_t *obj, int TargetPos, int MapLvl, int Ap, int a5 )
{
    Anim01_t *anim;

    if( AnimCancel(obj) == -1 || !Ap ){ AnimStop(); return -1; }
    if( TargetPos == obj->GridId && MapLvl == obj->Elevation ) return 0;
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 1;
    anim->Silence = 1;
    anim->Target.Obj = obj;
    anim->TargetPos = TargetPos;
    anim->Elevation = MapLvl;
    anim->Ap.Int = Ap;
    anim->i07 = a5;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, anim->Silence, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 ),  &anim->ImgObj ) ){ AnimStop(); return -1; }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimObjRunToTile( Obj_t *obj, int GridId, int MapLvl, int Ap, int a5 )
{
    Anim01_t *anim;
    MsgLine_t Line;
    char stmp[ 80 ];

    if( AnimCancel( obj ) == -1 ) return -1;
    if( !Ap ){ AnimStop(); return -1; }
    if( GridId == obj->GridId ){
        if( obj->Elevation == MapLvl ) return 0;
    }

    if( CritterOverload( obj ) ){
        if( PartyMembRdy( obj ) ){
            if ( obj == gObjDude ){
                strcpy( stmp, MessageGetMessage( &gMessage, &Line, 8000 ) ); // 'You are overload'
            } else {
                sprintf( stmp, MessageGetMessage( &gMessage, &Line, 8001 ), CritterGetName( obj ) ); // '%s is overload'
            }
            IfcMsgOut( stmp );
        }
        return AnimObjMoveToTile( obj, GridId, MapLvl, Ap, a5 ); // walk, when overloaded
    }

    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 1;

    if( OBJTYPE( obj->ImgId ) == TYPE_CRIT && (obj->Critter.State.CombatResult & 0x0C) ){
        anim->Silence = 1;
    } else {
	if( obj == gObjDude && CritterUsingSkill( 0 ) && !PerkLvl( gObjDude, PERK_SILENT_RUNNING ) ){
    	    anim->Silence = 1;
	} else {
	    anim->Silence = ( !ArtFileExist( ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, 19, 0, obj->Orientation + 1 ) ) ) ? 1 : 19;
	}
    }

    anim->Target.Obj = obj;
    anim->TargetPos = GridId;
    anim->Elevation = MapLvl;
    anim->Ap.Int = Ap;
    anim->i07 = a5;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, anim->Silence, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 ), &anim->ImgObj ) ){ AnimStop(); return -1; }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk44( Obj_t *obj, int GridIdx, int MapLvl, int a4, int a5 )
{
    Anim01_t *anim;

    if( AnimCancel(obj) == -1 ){ AnimStop(); return -1; }
    if( GridIdx == obj->GridId && MapLvl == obj->Elevation ) return 0;
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 2;
    anim->Target.Obj = obj;
    anim->TargetPos = GridIdx;
    anim->Elevation = MapLvl;
    anim->Silence = a4;
    anim->i07 = a5;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, anim->Silence, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 ), &anim->ImgObj ) ){ AnimStop(); return -1; }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk45( Obj_t *obj, int GridIdx, int MapLvl, int a4, int a5 )
{
    Anim01_t *anim;

    if( AnimCancel( obj ) == -1 ){ AnimStop(); return -1; }
    if( GridIdx == obj->GridId && MapLvl == obj->Elevation ) return 0;
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 3;
    anim->Target.Obj = obj;
    anim->TargetPos = GridIdx;
    anim->Elevation = MapLvl;
    anim->Silence = a4;
    anim->i07 = a5;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, anim->Silence, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1), &anim->ImgObj ) ){ AnimStop(); return -1; }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk46( Obj_t *obj1, Obj_t *obj2, int a3 )
{
    int v6, NewPos, Elevation;
    Anim01_t *anim;

    if( AnimCancel(obj1) == -1 ){ AnimStop(); return -1; }
    if( obj1->Elevation == obj2->Elevation ){
        v6 = 7;
        NewPos = obj2->GridId + 4;
        Elevation = obj2->Elevation + 1;
    } else {
        v6 = 8;
        NewPos = obj2->GridId + 200;
        Elevation = obj2->Elevation;
    }
    if( NewPos == obj1->GridId && Elevation == obj1->Elevation ) return 0;
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 20;
    anim->Target.Obj = obj1;
    anim->TargetPos = NewPos;
    anim->Elevation = Elevation;
    anim->Silence = v6;
    anim->i07 = a3;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( ArtMakeId( OBJTYPE( obj1->ImgId ), obj1->ImgId & 0xFFF, anim->Silence, (obj1->ImgId & 0xF000) >> 12, obj1->Orientation + 1 ), &anim->ImgObj ) ){ AnimStop(); return -1; }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk47( Obj_t *obj, int a2 )
{
    Anim01_t *anim;

    if( AnimCancel(obj) == -1 ){ AnimStop(); return -1; }
    anim = &gAnimations[gAnimIdx].i05[gAnimSubIdx];
    anim->State = 23;
    anim->Silence = 5;
    anim->Target.Obj = obj;
    anim->i07 = a2;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( ArtMakeId( OBJTYPE( obj->ImgId ), anim->Silence, obj->ImgId & 0xFFF, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 ), &anim->ImgObj ) ){
        AnimStop();
        return -1;
    }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimRegAnim( Obj_t *obj, int a2, int a3 )
{
    Anim01_t *anim;

    if( AnimCancel( obj ) == -1 ){ AnimStop(); return -1; }
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 4;
    anim->Target.Obj = obj;
    anim->Silence = a2;
    anim->i07 = a3;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, anim->Silence, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1), &anim->ImgObj) ){ AnimStop(); return -1; }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimRegAnimReverse( Obj_t *a1, int a2, int a3 )
{
    Anim01_t *anim;
DD
    if( AnimCancel( a1 ) == -1 ){ AnimStop(); return -1; }
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 5;
    anim->Target.Obj = a1;
    anim->Silence = a2;
    anim->i07 = a3;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( ArtMakeId( OBJTYPE( a1->ImgId ), a1->ImgId & 0xFFF, anim->Silence, (a1->ImgId & 0xF000) >> 12, a1->Orientation + 1 ), &anim->ImgObj) ){ AnimStop(); return -1; }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk50( Obj_t *obj, int a2, int a3 )
{
    Anim01_t *anim;

    if( AnimCancel( obj ) == -1 ){ AnimStop(); return -1; }
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 6;
    anim->Target.Obj = obj;
    anim->Silence = a2;
    anim->i07 = a3;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, anim->Silence, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 ), &anim->ImgObj ) ){ AnimStop(); return -1; }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk51( Obj_t *obj, int a2 )
{
    Anim01_t *anim;

    if( AnimCancel( obj ) == -1 ){ AnimStop(); return -1; }
    anim = gAnimations[ gAnimIdx ].i05;
    anim[ gAnimSubIdx ].State = 7;
    anim[ gAnimSubIdx ].i07 = -1;
    anim[ gAnimSubIdx ].ImgObj = NULL;
    anim[ gAnimSubIdx ].Target.Obj = obj;
    anim[ gAnimSubIdx ].TargetPos = a2;
    gAnimSubIdx++;
    return 0;
}

int AnimTurnCCW( Obj_t *obj )
{
    Anim01_t *anim;

    if( AnimCancel(obj) == -1 ){ AnimStop(); return -1; }
    anim = gAnimations[ gAnimIdx ].i05;
    anim[ gAnimSubIdx ].State = 8;
    anim[ gAnimSubIdx ].i07 = -1;
    anim[ gAnimSubIdx ].ImgObj = 0;
    anim[ gAnimSubIdx ].Target.Obj = obj;
    gAnimSubIdx++;
    return 0;
}

int AnimTurnCW( Obj_t *a1 )
{
    Anim01_t *anim;

    if( AnimCancel(a1) == -1 ){ AnimStop(); return -1; }
    anim = gAnimations[gAnimIdx].i05;
    anim[ gAnimSubIdx ].State = 9;
    anim[ gAnimSubIdx ].i07 = -1;
    anim[ gAnimSubIdx ].ImgObj = 0;
    anim[ gAnimSubIdx ].Target.Obj = a1;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk54( Obj_t *a1 )
{
    Anim01_t *anim;

    if( AnimCancel(a1) == -1 ){ AnimStop(); return -1; }
    anim = gAnimations[gAnimIdx].i05;
    anim[ gAnimSubIdx ].State = 10;
    anim[ gAnimSubIdx ].i07 = -1;
    anim[ gAnimSubIdx ].ImgObj = 0;
    anim[ gAnimSubIdx ].i10 = 0;
    anim[ gAnimSubIdx ].Target.Obj = a1;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk55( Obj_t *a1 )
{
    Anim01_t *anim;

    if( AnimCancel(a1) == -1 ){ AnimStop(); return -1; }
    anim = gAnimations[gAnimIdx].i05;
    anim[ gAnimSubIdx ].State = 10;
    anim[ gAnimSubIdx ].i07 = -1;
    anim[ gAnimSubIdx ].ImgObj = 0;
    anim[ gAnimSubIdx ].i10 = 1;
    anim[ gAnimSubIdx ].Target.Obj = a1;
    gAnimSubIdx++;
    return 0;
}

int AnimSetCallback11( Obj_t *a1, AnimU_t a2, int (*Cb)(void *, void *), int a4 )
{
    Anim01_t *anim;

    if( AnimCancel( NULL ) == -1 || !Cb ){ AnimStop(); return -1; }
    anim = gAnimations[ gAnimIdx ].i05;
    anim[ gAnimSubIdx ].State = 11;
    anim[ gAnimSubIdx ].i10 = 0;
    anim[ gAnimSubIdx ].ImgObj = NULL;
    anim[ gAnimSubIdx ].Target = a2;
    anim[ gAnimSubIdx ].GpPtr = a1;
    anim[ gAnimSubIdx ].Callback11 = (void *)Cb;
    anim[ gAnimSubIdx ].i07 = a4;
    gAnimSubIdx++;
    return 0;
}

int AnimSetCallback12( Obj_t *Critter, Obj_t *Target, AnimU_t Ap, int (*Cb)(Obj_t *, Obj_t *, int), int a5 )
{
    Anim01_t *anim;

    if( AnimCancel( NULL ) == -1 || !Cb ){ AnimStop(); return -1; }
    anim = gAnimations[ gAnimIdx ].i05;
    anim[ gAnimSubIdx ].State = 12;
    anim[ gAnimSubIdx ].i10 = 0;
    anim[ gAnimSubIdx ].ImgObj = NULL;
    anim[ gAnimSubIdx ].Target.Obj = Target;
    anim[ gAnimSubIdx ].GpPtr = Critter;
    anim[ gAnimSubIdx ].Callback12 = Cb;
    anim[ gAnimSubIdx ].Ap = Ap;
    anim[ gAnimSubIdx ].i07 = a5;
    gAnimSubIdx++;
    return 0;
}

int AnimSetFinish( void *a1, Obj_t *a2, int (*a3)(Obj_t *,Obj_t *), int a4 )
{
    Anim01_t *i05;

    if( AnimCancel( NULL ) == -1 || !a3 ){ AnimStop(); return -1; }
    i05 = gAnimations[ gAnimIdx ].i05;
    i05[ gAnimSubIdx ].State = 11;
    i05[ gAnimSubIdx ].i10 = 1;
    i05[ gAnimSubIdx ].ImgObj = NULL;
    i05[ gAnimSubIdx ].Target.Obj = a2;
    i05[ gAnimSubIdx ].GpPtr = a1;
    i05[ gAnimSubIdx ].Callback11 = (void *)a3;
    i05[ gAnimSubIdx ].i07 = a4;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk59( Obj_t *a1, int a2, int a3 ) // no xref
{
    Anim01_t *i05;

    if( AnimCancel(a1) == -1 ){ AnimStop(); return -1; }
    i05 = gAnimations[ gAnimIdx ].i05;
    i05[ gAnimSubIdx ].State = 14;
    i05[ gAnimSubIdx ].ImgObj = NULL;
    i05[ gAnimSubIdx ].Target.Obj = a1;
    i05[ gAnimSubIdx ].i10 = a2;
    i05[ gAnimSubIdx ].i07 = a3;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk60( Obj_t *a1, int a2, int a3 )
{
    Anim01_t *i05;

    if( AnimCancel(a1) == -1 ){ AnimStop(); return -1; }
    i05 = gAnimations[ gAnimIdx ].i05;
    i05[ gAnimSubIdx ].State = 15;
    i05[ gAnimSubIdx ].ImgObj = 0;
    i05[ gAnimSubIdx ].Target.Obj = a1;
    i05[ gAnimSubIdx ].i10 = a2;
    i05[ gAnimSubIdx ].i07 = a3;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk61( Obj_t *a1, int a2 )
{
    Anim01_t *i05;

    if( AnimCancel( a1 ) == -1 ){ AnimStop(); return -1; }
    i05 = gAnimations[ gAnimIdx ].i05;
    i05[ gAnimSubIdx ].State = 16;
    i05[ gAnimSubIdx ].ImgObj = NULL;
    i05[ gAnimSubIdx ].Target.Obj = a1;
    i05[ gAnimSubIdx ].i07 = a2;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk62( Obj_t *a1, int a2, int a3 )
{
    Anim01_t *anim;

    if( AnimCancel(a1) == -1 ){ AnimStop(); return -1; }
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 17;
    anim->Target.Obj = a1;
    anim->TargetPos = a2; // ???
    anim->i07 = a3;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( a2, &anim->ImgObj ) ){ AnimStop(); return -1; }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk63( Obj_t *a1, int a2, int a3 )
{
    Anim01_t *anim;

    if( AnimRegPlaySfx( a1, GSoundCharacterFileName( a1, 38, a2 ), a3 ) == -1 ) return -1;
    if( AnimCancel(a1) == -1 ){ AnimStop(); return -1; }
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 18;
    anim->Silence = 38;
    anim->i07 = 0;
    anim->Target.Obj = a1;
    anim->TargetPos = a2;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( ArtMakeId( OBJTYPE( a1->ImgId ), a1->ImgId & 0xFFF, 38, a2, a1->Orientation + 1), &anim->ImgObj ) ){ AnimStop(); return -1; }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimChangeItemStart( Obj_t *obj, int Radius, int a3 ) // light animation ?
{
    Anim01_t *anim;

    if( AnimCancel( obj ) == -1 ){ AnimStop(); return -1; }
    anim = gAnimations[ gAnimIdx ].i05;
    anim[ gAnimSubIdx ].State = 19;
    anim[ gAnimSubIdx ].ImgObj = 0;
    anim[ gAnimSubIdx ].Target.Obj = obj;
    anim[ gAnimSubIdx ].TargetPos = Radius;
    anim[ gAnimSubIdx ].i07 = a3;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk65( Obj_t *obj, int TargetPos, int a3 )
{
    Anim01_t *anim;

    if( AnimCancel( obj ) == -1 ){ AnimStop(); return -1; }
    anim = gAnimations[ gAnimIdx ].i05;
    anim[ gAnimSubIdx ].State = 24;
    anim[ gAnimSubIdx ].ImgObj = 0;
    anim[ gAnimSubIdx ].Target.Obj = obj;
    anim[ gAnimSubIdx ].TargetPos = TargetPos;
    anim[ gAnimSubIdx ].i07 = a3;
    gAnimSubIdx++;
    return 0;
}

int AnimRegPlaySfx( Obj_t *a1, char *a2, int a3 )
{
    Anim01_t *anim;

    if( AnimCancel( a1 ) == -1 ){ AnimStop(); return -1; }
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 11;
    anim->Target.Obj = a1;
    if( a2 ){
        if( (anim->GpPtr = GSoundUnk43( a2, a1, GSoundDistance( a1 ) )) )
            anim->Callback11 = GSoundDbgPlayA;
        else
            anim->State = 28;
    } else {
        anim->State = 28;
    }
    anim->ImgObj = NULL;
    anim->i07 = a3;
    gAnimSubIdx++;
    return 0;
}

int AnimRegAnimateForever( Obj_t *who, int animId, int delay )
{
    Anim01_t *anim;
DD
printf("*** Anim Reg animate: Id=%i, Delay=%i\n", animId, delay );
    if( AnimCancel( who ) == -1 ){ AnimStop(); return -1; }
    anim = &gAnimations[ gAnimIdx ].i05[ gAnimSubIdx ];
    anim->State = 25;
    anim->Target.Obj = who;
    anim->Silence = animId;
    anim->i07 = delay;
    anim->ImgObj = NULL;
    if( !ArtLoadImg( ArtMakeId( OBJTYPE( who->ImgId ), who->ImgId & 0xFFF, anim->Silence, (who->ImgId & 0xF000) >> 12, who->Orientation + 1), &anim->ImgObj ) ){ AnimStop(); return -1; }
    ArtClose( anim->ImgObj );
    anim->ImgObj = NULL;
    gAnimSubIdx++;
    return 0;
}

int AnimUnk68( int a1, int a2 )
{
    int Ap;
    Anim01_t *anim;

    if( AnimCancel( 0 ) == -1 ){ AnimStop(); return -1; }
    a1 |= 0x100;
    if( (Ap = AnimReserve( a1 )) == -1 ) return -1;        
    gAnimations[ Ap ].Flags = 16;
    anim = gAnimations[ gAnimIdx ].i05;
    anim[ gAnimSubIdx ].Target.Obj = 0;
    anim[ gAnimSubIdx ].State = 26;
    anim[ gAnimSubIdx ].ImgObj = 0;
    anim[ gAnimSubIdx ].Ap.Int = Ap;
    anim[ gAnimSubIdx ].i07 = a2;
    gAnimSubIdx++;
    return 0;
}

int AnimStateMachine( int AnimIdx )
{
    int n, tmp, err;
    Anim01_t *anim;
    VidRect_t v18, Area, Area1;

    if( AnimIdx == -1 ) return -1;
    if( gAnimations[ AnimIdx ].i01 == -1000 ) return -1;
    do{
        n = gAnimations[ AnimIdx ].i01;
        if( n >= gAnimations[ AnimIdx ].Steps ) return 0;
        if( n > gAnimations[ AnimIdx ].Step ){
            tmp = gAnimations[ AnimIdx ].i05[ n ].i07;
            if( tmp < 0 ) return 0;
            if( tmp > 0 ){
                gAnimations[ AnimIdx ].i05[ n ].i07--;
                return 0;
            }
        }
        anim = &gAnimations[ AnimIdx ].i05[ gAnimations[ AnimIdx ].i01++ ];
        switch( anim->State ){
            case 0: err = AnimUnk08( anim->Target.Obj, anim->GpPtr, anim->Ap.Int, anim->Silence, AnimIdx ); break;
            case 1: err = AnimUnk10( anim->Target.Obj, anim->TargetPos, anim->Elevation, anim->Ap.Int, anim->Silence, AnimIdx ); break;
            case 2: err = AnimUnk12( anim->Target.Obj, anim->TargetPos, anim->Elevation, anim->Silence, AnimIdx, 0x00 ); break;
            case 3: err = AnimUnk12( anim->Target.Obj, anim->TargetPos, anim->Elevation, anim->Silence, AnimIdx, 0x10 ); break;
            case 4: err = AnimUnk17( anim->Target.Obj, anim->Silence, AnimIdx, 0x00 ); break;
            case 5: err = AnimUnk17( anim->Target.Obj, anim->Silence, AnimIdx, 0x01 ); break; //!
            case 6: 
        	err = AnimUnk17( anim->Target.Obj, anim->Silence, AnimIdx, 0x40 ); 
        	if( err == -1 ){
                    if( !ObjUnk33( anim->Target.Obj, &Area1 ) ) TileUpdateArea( &Area1, anim->Target.Obj->Elevation );
                    if( AnimIdx != -1 ) AnimUpdate( AnimIdx, 0 );
                    err = 0;
                }
                break;
            case 7:
                if( !CritterUnk31( anim->Target.Obj ) ) AnimUnk24( anim->Target.Obj, TileTurnAt( anim->Target.Obj->GridId, anim->TargetPos ), -1);
                AnimUpdate( AnimIdx, 0 );
                err = 0;
                break;
            case 8: err = AnimUnk27( anim->Target.Obj, 1, AnimIdx ); break; // ?
            case 9: err = AnimUnk27( anim->Target.Obj, -1, AnimIdx ); break; // ?
            case 10:
                if( !ObjUnk33( anim->Target.Obj, &v18 ) ) TileUpdateArea( &v18, anim->Target.Obj->Elevation );
                if( AnimIdx != -1 ) AnimUpdate( AnimIdx, 0 );
                err = 0;
                break;
            case 11:
                if( !(err = anim->Callback11( anim->GpPtr, anim->Target.Obj ) ) ) err = AnimUpdate( AnimIdx, 0 );
                break;
            case 12:
                if( !(err = anim->Callback12( anim->GpPtr, anim->Target.Obj, anim->Ap.Int )) ) err = AnimUpdate( AnimIdx, 0 ); 
                break;
            case 14:
                if( anim->i10 == 0x20 ) {
                    if( !ObjLightedOn( anim->Target.Obj, &Area ) ) TileUpdateArea( &Area, anim->Target.Obj->Elevation );
                } else {
                    if( anim->i10 != 1 ){
                        anim->Target.Obj->Flags |= anim->i10;
                    } else {
                	if( !ObjUnk33( anim->Target.Obj, &Area ) ) TileUpdateArea( &Area, anim->Target.Obj->Elevation );
                    }
                }                
                err = AnimUpdate( AnimIdx, 0 ); 
                break;
            case 15:
                if( anim->i10 == 0x20 ){
                    if( !ObjLightedOff(anim->Target.Obj, &Area) ) TileUpdateArea( &Area, anim->Target.Obj->Elevation );
                } else {
                    if( anim->i10 != 1 ) {
                        anim->Target.Obj->Flags &= ~anim->i10;
                    } else {
                	if( !ObjUnk32( anim->Target.Obj, &Area ) ) TileUpdateArea( &Area, anim->Target.Obj->Elevation );
                    }
                }                
                err = AnimUpdate( AnimIdx, 0 ); 
                break;
            case 16: if( !ObjSetPlayer( anim->Target.Obj, &Area ) ) TileUpdateArea( &Area, anim->Target.Obj->Elevation );
        	err = AnimUpdate( AnimIdx, 0 ); 
        	break;
            case 17: err = AnimUnk29( anim->Target.Obj, AnimIdx, anim->TargetPos); break;
            case 18: err = AnimUnk17( anim->Target.Obj, 38, AnimIdx, anim->TargetPos ); break;
            case 19: // light animation
                ObjSetLight( anim->Target.Obj, anim->TargetPos, anim->Target.Obj->LightIntensity, &Area ); // obj, radius, intensity, area
                TileUpdateArea( &Area, anim->Target.Obj->Elevation );
                err = AnimUpdate( AnimIdx, 0 );
                break;
            case 20: err = AnimUnk13( anim->Target.Obj, anim->TargetPos, anim->Elevation, anim->Silence, AnimIdx );  break;
            case 23: err = AnimUnk14( anim->Target.Obj, anim->Silence, AnimIdx ); break;
            case 24:
                if( anim->TargetPos ){
                    if( !ObjUnk34( anim->Target.Obj, &Area ) ) TileUpdateArea( &Area, anim->Target.Obj->Elevation );            	    
                } else {
            	    if( !ObjUnk35( anim->Target.Obj, &Area ) ) TileUpdateArea( &Area, anim->Target.Obj->Elevation );
                }                
                err = AnimUpdate( AnimIdx, 0 ); 
                break;
            case 25: 
DD 
        	err = AnimUnk17( anim->Target.Obj, anim->Silence, AnimIdx, 0x80 ); break;
            case 26:
                gAnimations[ anim->Ap.Int ].Flags &= ~0x10;
                if( (err = AnimUpdate( anim->Ap.Int, 1 ) ) != -1 ) err = AnimUpdate( AnimIdx, 0 );
                break;
            case 28: err = AnimUpdate( AnimIdx, 0 ); break;
            default: err = -1; break;
        }
	if( err == -1 ) AnimEnd( AnimIdx );
    }while( gAnimations[ AnimIdx ].i01 != -1000 );
    return -1;
}

int AnimUpdate( int AnimIdx, int Flg )
{
    if( AnimIdx == -1 || gAnimations[ AnimIdx ].i01 == -1000 ) return -1;
    gAnimations[ AnimIdx ].Step++;
    if( gAnimations[ AnimIdx ].Step == gAnimations[ AnimIdx ].Steps ) return AnimEnd( AnimIdx );
    if( Flg ) return AnimStateMachine( AnimIdx );
    return 0;
}

int AnimEnd( int idx )
{
    VidRect_t Area;
    Anim01_t *p;
    int i, j, tmp;

    if( idx == -1 || gAnimations[ idx ].i01 == -1000 ) return -1;
    for( i = 0; i < gAnimCnt1; i++  ) if( idx == gAnimUnk23[ i ].AnimIdx ) gAnimUnk23[ i ].Stat = -1000;
    
     for( i = 0; i < gAnimations[ idx ].Steps; i++ ){
        if( gAnimations[ idx ].i05[ i ].State != 10 ) continue;
        if( i >= gAnimations[ idx ].Step && !(gAnimations[ idx ].i05[ i ].i10 & 1 ) ) continue;
        ObjDestroy( gAnimations[ idx ].i05[ i ].Target.Obj, &Area );
        TileUpdateArea( &Area, gAnimations[ idx ].i05[ i ].Target.Obj->Elevation );                
    }
    p = gAnimations[ idx ].i05;
    for( i = 0; i < gAnimations[ idx ].Steps; i++, p++ ){
        if( p->ImgObj ) ArtClose( p->ImgObj );
        if( p->State == 11 || p->State == 12 ){
            if( i < gAnimations[ idx ].i01 ) continue;            
            if( p->i10 & 1 ){
                p->Callback11( p->GpPtr, p->Target.Obj );
            } else if( p->State == 11 && p->Callback11 == GSoundDbgPlayA ){
                GSoundSfxDelete( p->GpPtr );
            }            
	    continue;
        } 
        if( p->State == 26 ) continue;
        if( OBJTYPE( p->Target.Obj->ImgId ) != TYPE_CRIT ) continue;
        for( j = 0; j < i; j++ ){
            if( p->Target.Obj != gAnimations[ idx ].i05[ j ].Target.Obj ) continue;
            tmp = gAnimations[ idx ].i05[ j ].State;
            if( tmp != 11 && tmp != 12 ) break;
        }
        if( j != i ) continue;
        for( j = 0; j < gAnimations[ idx ].Step; j++ ){
            if( gAnimations[ idx ].i05[ j ].State == 10 && p->Target.Obj == gAnimations[ idx ].i05[ j ].Target.Obj ) break;
        }                        

        if( j != gAnimations[ idx ].Step ) continue;
        for( j = 0; j < gAnimCnt1; j++ ){
    	    if( p->Target.Obj == gAnimUnk23[ j ].Obj ){ gAnimUnk23[ j ].Stat = -1000; break; }
        }                            
        if( (gAnimations[ idx ].Flags & 0xff) >= 0 && !CritterUnk31( p->Target.Obj ) ) AnimUnk24( p->Target.Obj, p->Target.Obj->Orientation, -1);
    }
    gAnimations[ idx ].Step = -1;
    gAnimations[ idx ].i01 = -1000;
    if( gAnimations[ idx ].Flags & 2 ) CombatUnk62();
    gAnimations[ idx ].Flags = ( gAnimUnk40 ) ? 0x20 : 0;
    return 0;
}

int AnimUnk71( Obj_t *a1, Obj_t *a2 )
{
    Proto_t *proto;

    if( a1 == gObjDude ){
	if( !ObjUnk42( a2 ) ) return 0;    
    }
    if( OBJTYPE( a1->ImgId ) != TYPE_CRIT ) return 0;    
    if( OBJTYPE( a2->ImgId ) != TYPE_SCEN ) return 0;    
    if( CritterGetBodyType( a1 ) != 2 ) return 0;        
    if( ProtoGetObj( a1->Pid, &proto ) == -1 ) return 0;                    
    if( proto->Critt.Type ) return 0;
    return !UseObjLocked( a2 ) && CritterGetGender( a1 ) != 15;
}

int AnimFindTrace( Obj_t *obj, int BeginPos, int EndPos, unsigned char *Trace, int NotApproach )
{
    return AnimMakeTrace( obj, BeginPos, EndPos, Trace, NotApproach, ObjReach );
}

#define MAPGRID_CLR()		memset( MapGrid, 0x00, 5000 )
#define MAPGRID_SET( pos )	MapGrid[ pos / 8 ] |= (1 << ( pos & 0x07 ) )
#define MAPGRID_TEST( pos )	( MapGrid[ pos / 8 ] & (1 << ( pos & 7 ) ) )
int AnimMakeTrace( Obj_t *obj, int BeginPos, int EndPos, unsigned char *Trace, int NotApproach, Obj_t *(*AccessibleCb)(Obj_t *, int, int))
{
    static unsigned char MapGrid[ 5000 ];
    static Anim02_t gAnimUnk01[ 2000 ];
    static Anim02_t gAnimUnk08[ 2000 ];
    unsigned int v50;
    int v11,v12,v19,v22,Pos,v39,v40,v41,i02,v43,v44;
    int pY,pX,NotCombatFlg,Gender,CritterFlg,v55,k,i;
    Anim02_t *v13, *v14;
    Obj_t *v21, *j;
    char c;

    CritterFlg = 0;
    if( NotApproach && AccessibleCb( obj, EndPos, obj->Elevation ) ) return 0; // destination is not accessible

    if( OBJTYPE( obj->Pid ) == TYPE_CRIT ){ CritterFlg = 1; Gender = CritterGetGender( obj ); }
    NotCombatFlg = !IN_COMBAT;
    MAPGRID_CLR();

    gAnimUnk01[ 0 ].GridIdx = BeginPos;
    gAnimUnk01[ 0 ].i02 = -1;
    gAnimUnk01[ 0 ].Orientation = 0;
    gAnimUnk01[ 0 ].Distance = AnimDistance( BeginPos, EndPos );
    gAnimUnk01[ 0 ].i05 = 0;

    MAPGRID_SET( BeginPos );

    for( i = 1; i < 2000; i++ ) gAnimUnk01[ i ].GridIdx = -1;
    TileGetScrCoordinates( EndPos, &pX, &pY );

    v50 = 0;
    v55 = 1;
    while( v55 ){            
            i = -1;
            v13 = NULL;
	    v14 = gAnimUnk01;
            for( v11 = v12 = 0; v11 < v55; v12++, v14++ ){
                if( v14->GridIdx == -1 ) continue;
                v11++;
                if( i == -1 || ((v14->Distance + v14->i05) < (v13->i05 + v13->Distance)) ){
                    v13 = v14;
                    i = v12;
                } 
            }
            Pos = gAnimUnk01[ i ].GridIdx;
            v39 = gAnimUnk01[ i ].i02;
            v40 = gAnimUnk01[ i ].Orientation;
            v41 = gAnimUnk01[ i ].Distance;            
            i02 = gAnimUnk01[ i ].i05;
            gAnimUnk01[ i ].GridIdx = -1;
	    v55--;
            if( Pos == EndPos ){
    		if( v55 == 0 ) v55 = 1;
    		break;
            }
            gAnimUnk08[ v50 ].GridIdx = Pos;
            gAnimUnk08[ v50 ].i02 = v39;
            gAnimUnk08[ v50 ].Orientation = v40;
            gAnimUnk08[ v50 ].Distance = v41;
            gAnimUnk08[ v50 ].i05 = i02;
            v50++;
            if( v50 == 2000 ) return 0;
            for( i = 0; i < 6; i++ ){ // kierunki
                v19 = TileGetTileNumInDir( Pos, i, 1 );
                if( MAPGRID_TEST( v19 )) continue;
                if( v19 != EndPos ){
            	    if( (v21 = AccessibleCb( obj, v19, obj->Elevation ) ) ){
            		if( !AnimUnk71( obj, v21 ) ) continue;
            	    }
                }
                for( v22 = 0; gAnimUnk01[ v22 ].GridIdx != -1; v22++ );
                if( ++v55 == 2000 ) return 0;
                TileGetScrCoordinates( v19, &v44, &v43 );
                MAPGRID_SET( v19 );
                gAnimUnk01[ v22 ].GridIdx = v19;
                gAnimUnk01[ v22 ].i02 = Pos;
                gAnimUnk01[ v22 ].Orientation = i;
                gAnimUnk01[ v22 ].Distance = AnimDiagonal( v44, v43, pX, pY );
                gAnimUnk01[ v22 ].i05 = i02 + 50;
                if( NotCombatFlg && v40 != i ) gAnimUnk01[ v22 ].i05 += 10;
                if( CritterFlg ){
                    for( j = ObjGetFirst( obj->Elevation, gAnimUnk01[ v22 ].GridIdx ); j; j = ObjGetNext() ){
                        if( j->Pid >= 0x20003D9 && j->Pid <= 0x20003DC ) break;
                    }            
                    if( j ) gAnimUnk01[ v22 ].i05 += ( Gender == 15 ) ? 100 : 400;                            
                }
            }
    }
    if( !v55 ) return 0;

    for( k = 0; k < 800; k++ ){
        if( Pos == BeginPos ) break;
        if( Trace ) Trace[ k ] = v40;
        for( i = 0; gAnimUnk08[ i ].GridIdx != v39; i++ );
        Pos = gAnimUnk08[ i ].GridIdx;
        v39 = gAnimUnk08[ i ].i02;
        v40 = gAnimUnk08[ i ].Orientation;
    }
    // swap trace table
    if( Trace ){        
        for( i = 0; i < (k / 2); i++ ){
            c = Trace[ k - i - 1 ];
            Trace[ k - i - 1 ] = Trace[ i ];
            Trace[ i ] = c;
        }
    }
    return k;
}

int AnimDiagonal( int x0, int y0, int x1, int y1 )
{
    int dx, dy, min;

    dx = x1 - x0; if( dx < 0 ) dx = -dx; // |dx|
    dy = y1 - y0; if( dy < 0 ) dy = -dy; // |dy|
    min = ( dx > dy ) ? dy : dx;
    return dx + dy - min / 2;
}

int AnimDistance( int BeginPos, int EndPos )
{
    int x0, y0, x1, y1;

    TileGetScrCoordinates( BeginPos, &x0, &y0 );
    TileGetScrCoordinates( EndPos, &x1, &y1 );
    return AnimDiagonal( x0, y0, x1, y1 );
}

int AnimUnk06( Obj_t *obj, int GridPos1, int GridPos2, Anim04_t *t, Obj_t **pRet, int flg )
{
    return AnimUnk07( obj, GridPos1, GridPos2, t, pRet, flg, ObjReach );
}

int AnimUnk07( Obj_t *obj, int GridPos1, int GridPos2, Anim04_t *t, Obj_t **pRet, int a6, Obj_t *(*Cb)( Obj_t *, int, int ) )
{
    Obj_t *obj1, *v32,*v31,*v23;
    Anim04_t *v41, *v42;
    int v11,v12,v13,v14,v15,v17,*v18,v33,v34,v35;
    int v36,v38,v39,v40,v43,v26,v27,pY,pX; 
    int MapIdx, Pointed, *p_GridPos;

    if( pRet ){
        obj1 = Cb(obj, GridPos1, obj->Elevation);
        if( obj1 ){
            if( obj1 != *pRet && (a6 != 32 || obj1->Flags >= 0) ){
                *pRet = obj1;
                return 0;
            }
        }
    }
    TileGetScrCoordinates( GridPos1, &pX, &pY );
    pX += 16;
    pY += 8;
    v38 = GridPos1;
    TileGetScrCoordinates( GridPos2, &v27, &v26 );
    v43 = 0;
    v27 += 16;
    v26 += 8;
    v36 = 2 * abs32( v27 - pX );
    if( v27 - pX >= 0 )
        v11 = v27 - pX > 0;
    else
        v11 = -1;
    v33 = v11;
    v35 = 2 * abs32( v26 - pY );
    if( v26 - pY >= 0 )
        v12 = v26 - pY > 0;
    else
        v12 = -1;
    v13 = pX;
    v34 = v12;
    v14 = pY;
    if( v36 <= v35 ){
        v42 = &t[v43];
        v39 = v36 - (v35 >> 1);
        while( 1 ){
            Pointed = TileGetPointed(v13, v14);
            v17++;
            MapIdx = Pointed;
            if( v17 == a6 ){
                if( v43 >= 200 ) return 0;
                if( t ){
                    v42->GridPos = Pointed;
                    v42->i02 = obj->Elevation;
                    TileGetScrCoordinates(Pointed, &pX, &pY);
                    p_GridPos = &v42->GridPos;
                    v42->i03 = v13 - pX - 16;
                    p_GridPos[3] = v14 - pY - 8;
                }
                v17 = 0;
                v42++; v43++;
            }
            if( v14 == v26 ){ if( pRet ) *pRet = 0; break; }
            if( v39 >= 0 ){
                v13 += v33;
                v39 -= v35;
            }
            v14 += v34;
            v39 += v36;
            if( MapIdx != v38 ){
                if( pRet ){
                    v23 = Cb(obj, MapIdx, obj->Elevation);
                    v31 = v23;
                    if( v23 ){
                        if( v23 != *pRet && (a6 != 32 || v23->Flags >= 0) ){ *pRet = v31; break; }
                    }
                }
                v38 = MapIdx;
            }
        }        
    } else {
        v41 = &t[v43];
        v40 = v35 - (v36 >> 1);
        while( 1 ){
            v15 = TileGetPointed(v13, v14);
            v17++;
            MapIdx = v15;
            if( v17 == a6 ){
                if( v43 >= 200 ) return 0;
                if( t ){
                    v41->GridPos = v15;
                    v41->i02 = obj->Elevation;
                    TileGetScrCoordinates(v15, &pX, &pY);
                    v18 = &v41->GridPos;
                    v41->i03 = v13 - pX - 16;
                    v18[3] = v14 - pY - 8;
                }
                v17 = 0;
                v41++; v43++;
            }
            if( v13 == v27 ){ if( pRet ) *pRet = 0; break; }
            if( v40 >= 0 ){ v14 += v34; v40 -= v36; }
            v13 += v33;
            v40 += v35;
            if( MapIdx != v38 ){
                if( pRet ){                    
                    if( (v32 = Cb( obj, MapIdx, obj->Elevation )) ){
                        if( v32 != *pRet && (a6 != 32 || v32->Flags >= 0) ){ *pRet = v32; break; }
                    }
                }
                v38 = MapIdx;
            }
        }        
    }
    if( v17 ){
        if( v43 >= 200 ) return 0;
        if( t ){
            t[ v43 ].GridPos = MapIdx;
            t[ v43 ].i02 = obj->Elevation;
            TileGetScrCoordinates( MapIdx, &pX, &pY );
            t[ v43 ].i03 = v13 - pX - 16;
            t[ v43 ].i04 = v14 - pY - 8;
        }
        v43++;
    } else if( v43 > 0 && t ){
        t[ v43 - 1 ].i02 = obj->Elevation;
    }
    return v43;
}

int AnimUnk08( Obj_t *a1, Obj_t *a2, int a3, int a4, int a5 )
{
    int v8,dAp,flg;

    flg = a2->Flags & 1;
    a2->Flags |= 0x01;
    v8 = AnimCreateTrace( a1, a2->GridId, a2->Elevation, -1, a4, 0, a5 );
    if( !flg ) a2->Flags &= ~0x01;
    if( v8 == -1 ) return -1;
    dAp = ((a1->Flags & 0x800) != 0) + 1;
    gAnimUnk23[ v8 ].Ap -= dAp;
    if( gAnimUnk23[ v8 ].Ap <= 0 ){
        gAnimUnk23[ v8 ].Stat = -1000;
        AnimUpdate( a5, 0 );
    }
    if( dAp ) gAnimUnk23[ v8 ].GridPos = TileGetTileNumInDir( a2->GridId, gAnimUnk23[ v8 ].Path[ dAp + gAnimUnk23[ v8 ].Ap ], 1 );
    if( dAp == 2 ) gAnimUnk23[ v8 ].GridPos = TileGetTileNumInDir( gAnimUnk23[ v8 ].GridPos, gAnimUnk23[ v8 ].Path[ gAnimUnk23[ v8 ].Ap ], 1 );
    if( a3 != -1 && a3 < gAnimUnk23[ v8 ].Ap ) gAnimUnk23[ v8 ].Ap = a3;
    return 0;
}

int AnimUnk09( Obj_t *obj, int GridId, int a3, int a4, int a5, Anim04_t *a6, Obj_t **pObj )
{
    Anim04_t *v40, *v41;
    int v10,v11,v12,v13,v16,v32,v33,v34,v35,v37;
    int v38,v39,v42,v27,v28,pY,pX,GridIdx;

    TileGetScrCoordinates(GridId, &pX, &pY);
    if( a3 > a5 ) a3 = a5;
    v37 = GridId;
    pX += 16;
    pY += 8;
    v42 = 0;
    TileGetScrCoordinates(a4, &v28, &v27);
    v28 += 16;
    v27 += 8;
    if( pObj ) *pObj = NULL;
    v35 = 2 * abs32(v28 - pX);
    if( v28 - pX >= 0 )
        v10 = v28 - pX > 0;
    else
        v10 = -1;
    v32 = v10;
    v34 = 2 * abs32(v27 - pY);
    if( v27 - pY >= 0 )
        v11 = v27 - pY > 0;
    else
        v11 = -1;
    v12 = pX;
    v33 = v11;
    v13 = pY;
    if( v35 > v34 ){
        v40 = &a6[ v42 ];
        v38 = v34 - (v35 >> 1);
        while( 1 ){
            GridIdx = TileGetPointed(v12, v13);
            v16++;
            if( v16 == 16 ){
                if( v42 >= 200 ) return 0;
                if( a6 ){
                    v40->GridPos = GridIdx;
                    v40->i02 = a3;
                    TileGetScrCoordinates( GridIdx, &pX, &pY );
                    v40->i03 = v12 - pX - 16;
                    v40->i04 = v13 - pY - 8;
                }
                v16 = 0;
                v40++;
                v42++;
            }
            if( v12 == v28 ) break;
            if( v38 >= 0 ){
                v13 += v33;
                v38 -= v35;
            }
            v12 += v32;
            v38 += v34;
            if( GridIdx != v37 ){
                if( pObj ){
                    if( (*pObj = ObjReach(obj, GridIdx, obj->Elevation ) ) ) break;
                }
                v37 = GridIdx;
            }
        }
    } else {
	v41 = &a6[v42];
	v39 = v35 - (v34 >> 1);
	while( 1 ){
	    GridIdx = TileGetPointed( v12, v13 );
    	    v16++;
    	    if( v16 == 16 ){
        	if( v42 >= 200 ) return 0;
        	if( a6 ){
            	    v41->GridPos = GridIdx;
            	    v41->i02 = a3;
            	    TileGetScrCoordinates( GridIdx, &pX, &pY );
            	    v41->i03 = v12 - pX - 16;
            	    v41->i04 = v13 - pY - 8;
        	}
        	v16 = 0;
        	v41++;
        	v42++;
    	    }
    	    if( v13 == v27 ) break;
    	    if( v39 >= 0 ){
        	v12 += v32;
        	v39 -= v34;
	    }
	    v13 += v33;
    	    v39 += v35;
    	    if( GridIdx != v37 ){
        	if( pObj ){
            	    if( (*pObj = ObjReach( obj, GridIdx, obj->Elevation ) ) ) break;
        	}
        	v37 = GridIdx;
    	    }
	}
    }
    
    if( v16 ){
        if( v42 >= 200 ) return 0;
        if( a6 ){
            a6[ v42 ].GridPos = GridIdx;
            a6[ v42 ].i02 = a5;
            TileGetScrCoordinates( GridIdx, &pX, &pY );
            a6[ v42 ].i03 = v12 - pX - 16;
            a6[ v42 ].i04 = v13 - pY - 8;
        }
        v42++;
    } else if( v42 > 0 && a6 ){
        a6[ v42 - 1 ].i02 = a5;
    }
    return v42;
}

int AnimUnk10( Obj_t *obj, unsigned int MapIdx, int MapLvl, int Ap, int Silence, int AnimIdx )
{
    int k;

    if( (k = AnimCreateTrace( obj, MapIdx, MapLvl, -1, Silence, 0, AnimIdx )) == -1 ) return -1;    
    if( !ObjReach( obj, MapIdx, MapLvl ) ) return 0;        
    if( --gAnimUnk23[ k ].Ap <= 0 ){ // reached destination
        gAnimUnk23[ k ].Stat = -1000;
        AnimUpdate( AnimIdx, 0 );
    }
    gAnimUnk23[ k ].GridPos = TileGetTileNumInDir( MapIdx, gAnimUnk23[ k ].Path[ gAnimUnk23[ k ].Ap ], 1 ); // stop position
    if( Ap != -1 && Ap < gAnimUnk23[ k ].Ap ) gAnimUnk23[ k ].Ap = Ap;    
    return 0;
}

int AnimCreateTrace( Obj_t *obj, int DstIdx, int MapLvl, int Ap, int a5, int a6, int AnimIdx )
{
    if( gAnimCnt1 == 24 ) return -1;
    gAnimUnk23[ gAnimCnt1 ].Obj = obj;
    gAnimUnk23[ gAnimCnt1 ].Flags = ( a6 ) ? 0x20 : 0;
    gAnimUnk23[ gAnimCnt1 ].Stat = -2000;
    gAnimUnk23[ gAnimCnt1 ].ArtId = ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, a5, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 );
    gAnimUnk23[ gAnimCnt1 ].Time = 0;
    gAnimUnk23[ gAnimCnt1 ].Speed = AnimGetSpeed( obj, gAnimUnk23[ gAnimCnt1 ].ArtId );
    gAnimUnk23[ gAnimCnt1 ].GridPos = DstIdx;
    gAnimUnk23[ gAnimCnt1 ].AnimIdx = AnimIdx;
    gAnimUnk23[ gAnimCnt1 ].i04 = a5;
    gAnimUnk23[ gAnimCnt1 ].Ap = AnimFindTrace( obj, obj->GridId, DstIdx, gAnimUnk23[ gAnimCnt1 ].Path, a6 );
    if( gAnimUnk23[ gAnimCnt1 ].Ap ){
        if( Ap != -1 && gAnimUnk23[ gAnimCnt1 ].Ap > Ap ) gAnimUnk23[ gAnimCnt1 ].Ap = Ap;
	return gAnimCnt1++;
    }
    gAnimUnk23[ gAnimCnt1 ].Stat = -1000;
    return -1;    
}

int AnimUnk12( Obj_t *obj, int a2, int a3, int a4, int a5, int Flags )
{
    int n;

    if( gAnimCnt1 == 24 ) return -1;
    gAnimUnk23[ gAnimCnt1 ].Obj = obj;
    gAnimUnk23[ gAnimCnt1 ].Flags = Flags | 0x02;
    if( a4 == -1 ){
        gAnimUnk23[ gAnimCnt1 ].ArtId = obj->ImgId;
        gAnimUnk23[ gAnimCnt1 ].Flags |= 0x04;
    } else {
        gAnimUnk23[ gAnimCnt1 ].ArtId = ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, a4, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1);
    }
    gAnimUnk23[ gAnimCnt1 ].Stat = -2000;
    gAnimUnk23[ gAnimCnt1 ].Time = 0;
    gAnimUnk23[ gAnimCnt1 ].Speed = AnimGetSpeed( obj, gAnimUnk23[ gAnimCnt1 ].ArtId );
    gAnimUnk23[ gAnimCnt1 ].AnimIdx = a5;
    if( OBJTYPE( obj->ImgId ) == 1 ){
        n = ( ( obj->ImgId ) >> 16 == 2 ) ? 16 : 4;
    } else {
        n = 32;
    }
    if( !( gAnimUnk23[ gAnimCnt1 ].Ap = AnimUnk06( obj, obj->GridId, a2, gAnimUnk23[ gAnimCnt1 ].Tab, 0, n ) ) ){
        gAnimUnk23[ gAnimCnt1 ].Stat = -1000;
        return -1;
    }
    gAnimCnt1++;
    return 0;
}

int AnimUnk13( Obj_t *obj, int a2, int a3, int a4, int a5 )
{
    Anim03_t *v8;

    if( gAnimCnt1 == 24 ) return -1;
    v8 = &gAnimUnk23[ gAnimCnt1 ];
    v8->Flags = 2;
    v8->Obj = obj;
    if( a4 == -1 ){
        v8->ArtId = obj->ImgId;
        v8->Flags |= 0x04;
    } else {
        v8->ArtId = ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, a4, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1);
    }
    v8->Stat = -2000;
    v8->Time = 0;
    v8->Speed = AnimGetSpeed( obj, v8->ArtId );
    v8->AnimIdx = a5;    
    if( !(v8->Ap = AnimUnk09( obj, obj->GridId, obj->Elevation, a2, a3, gAnimUnk23[ gAnimCnt1 ].Tab, 0 ) ) ){ v8->Stat = -1000; return -1; }
    gAnimCnt1++;
    return 0;
}

int AnimUnk14( Obj_t *obj, int a2, int a3 )
{
    Anim03_t *v6;

    if( gAnimCnt1 == 24 ) return -1;
    if( AnimUnk30( obj->GridId, obj->Elevation ) == obj->Elevation ) return -1;
    v6 = &gAnimUnk23[ gAnimCnt1 ];
    v6->Flags = 2;
    v6->Obj = obj;
    if( a2 == -1 ){
        v6->Flags |= 0x04;
        v6->ArtId = obj->ImgId;
    } else {
        v6->ArtId = ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, a2, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 );
    }
    v6->Stat = -2000;
    v6->Time = 0;
    v6->Speed = AnimGetSpeed( obj, v6->ArtId );
    v6->AnimIdx = a3;    
    if( !(v6->Ap = AnimUnk07( obj, obj->GridId, obj->GridId, v6->Tab, 0, 16, ObjReach )) ){ v6->Stat = -1000; return -1; } 
    gAnimCnt1++;
    return 0;
}

void AnimMovement( int AnimIdx )
{
    Obj_t *obj1,*obj2;
    ArtFrmHdr_t *Img;
    VidRect_t Area3, Area12;
    CachePool_t *ImgObj;
    int v7, y, v10, v16, CurrentAP, x, dir;

    obj1 = gAnimUnk23[ AnimIdx ].Obj;
    if( gAnimUnk23[ AnimIdx ].Stat == -2000 ){
        ObjMoveToTile( gAnimUnk23[ AnimIdx ].Obj, obj1->GridId, obj1->Elevation, &Area12 );
        ObjSetFrame( obj1, 0, &Area3 );
        RegionExpand( &Area12, &Area3, &Area12 );
        ObjSetRotation( obj1, gAnimUnk23[ AnimIdx ].Path[ 0 ], &Area3 );
        RegionExpand( &Area12, &Area3, &Area12 );
        ObjSetShape( obj1, ArtMakeId( OBJTYPE( obj1->ImgId ), obj1->ImgId & 0xFFF, gAnimUnk23[ AnimIdx ].i04, (obj1->ImgId & 0xF000) >> 12, obj1->Orientation + 1 ), &Area3 );
        RegionExpand( &Area12, &Area3, &Area12 );
        gAnimUnk23[ AnimIdx ].Stat = 0;
    } else {
        ObjIncFrame( gAnimUnk23[ AnimIdx ].Obj, &Area12 ); // movement animation advance
    }    
    if( (Img = ArtLoadImg( obj1->ImgId, &ImgObj )) ){
        ArtGetFrameShift( Img, obj1->FrameNo, obj1->Orientation, &x, &y ); // get displacement to next frame
        ArtClose( ImgObj );
    } else {
        x = y = 0;
    }

    ObjMove( obj1, x, y, &Area3 ); // update position
    RegionExpand( &Area12, &Area3, &Area12 );

    dir = gAnimUnk23[ AnimIdx ].Path[ gAnimUnk23[ AnimIdx ].Stat ] & 0x07;
    y = gTileHexY[ dir ];
    x = gTileHexX[ dir ];
    // outside hex area?
    if( (x < 0 && obj1->PosX <= x) || (x > 0 && x <= obj1->PosX ) || (y < 0 && obj1->PosY <= y) || (y > 0 && y <= obj1->PosY) ){
        x = obj1->PosX - x;
        y = obj1->PosY - y;
        v10 = TileGetTileNumInDir( obj1->GridId, dir, 1 );        
        if( (obj2 = ObjReach( obj1, v10, obj1->Elevation )) ){
            if( AnimUnk71( obj1, obj2 ) ){
                UseDoor( obj1, obj2, 0 ); // doorway
            } else {
                if( (gAnimUnk23[ AnimIdx ].Ap = AnimFindTrace( obj1, obj1->GridId, gAnimUnk23[ AnimIdx ].GridPos, (unsigned char *)gAnimUnk23[ AnimIdx ].Path, 1 ) ) ){
                    ObjMoveToTile( obj1, obj1->GridId, obj1->Elevation, &Area3 );
                    RegionExpand( &Area12, &Area3, &Area12 );
                    ObjSetFrame( obj1, 0, &Area3 );
                    RegionExpand( &Area12, &Area3, &Area12 );
                    ObjSetRotation( obj1, gAnimUnk23[ AnimIdx ].Path[ 0 ], &Area3 );
                    RegionExpand( &Area12, &Area3, &Area12 );
                    gAnimUnk23[ AnimIdx ].Stat = 0;
                } else {
                    gAnimUnk23[ AnimIdx ].Stat = -1000;
                }
                v10 = -1;
            }
        }
        if( v10 != -1 ){
            ObjMoveToTile( obj1, v10, obj1->Elevation, &Area3 );
            RegionExpand( &Area12, &Area3, &Area12 );
            v16 = 0;
            if( IN_COMBAT && OBJTYPE( obj1->ImgId ) == TYPE_CRIT ){
                v7 = CritterUnk47( obj1, 1 );
                if( gCombatMovePts < v7 ){
                    CurrentAP = obj1->Critter.State.CurrentAP;
                    v7 -= gCombatMovePts;
                    gCombatMovePts = 0;
                    obj1->Critter.State.CurrentAP = ( v7 > CurrentAP ) ? 0 : ( CurrentAP - v7 );
                } else {
                    gCombatMovePts -= v7;
                }
                if( obj1 == gObjDude ) IfaceRenderAP( gObjDude->Critter.State.CurrentAP, gCombatMovePts );
                v16 = gCombatMovePts + obj1->Critter.State.CurrentAP <= 0;
            }
            gAnimUnk23[ AnimIdx ].Stat++;
            if( gAnimUnk23[ AnimIdx ].Stat == gAnimUnk23[ AnimIdx ].Ap || v16 ){
                gAnimUnk23[ AnimIdx ].Stat = -1000;
            } else {
                ObjSetRotation( obj1, gAnimUnk23[ AnimIdx ].Path[ gAnimUnk23[ AnimIdx ].Stat ], &Area3 );
                RegionExpand( &Area12, &Area3, &Area12 );
                ObjMove( obj1, x, y, &Area3 );
                RegionExpand( &Area12, &Area3, &Area12 );
            }
        }
    }
    TileUpdateArea( &Area12, gAnimUnk23[ AnimIdx ].Obj->Elevation );
    if( gAnimUnk23[ AnimIdx ].Stat == -1000 ) AnimUpdate( gAnimUnk23[ AnimIdx ].AnimIdx, 1 );
}

void AnimMoveReverse( int AnimIdx )
{
    Obj_t *obj;
    ArtFrmHdr_t *Img;
    VidRect_t Area1, Area2;
    CachePool_t *ImgObj;
    Anim04_t *t;    
    int fpd;
DD
    obj = gAnimUnk23[ AnimIdx ].Obj;
    if( gAnimUnk23[ AnimIdx ].Stat == -2000 ){
        ObjSetShape( gAnimUnk23[ AnimIdx ].Obj, gAnimUnk23[ AnimIdx ].ArtId, &Area1 );
        gAnimUnk23[ AnimIdx ].Stat = 0;
    } else {
        ObjGetRefreshArea(gAnimUnk23[ AnimIdx ].Obj, &Area1 );
    }    
    if( (Img = ArtLoadImg( obj->ImgId, &ImgObj )) ){
        fpd = ArtGetFpd( Img ) - 1;
        ArtClose( ImgObj );
        if( !(gAnimUnk23[ AnimIdx ].Flags & 0x04) && ( !( gAnimUnk23[ AnimIdx ].Flags & 0x10 ) || ( fpd > obj->FrameNo ) ) ){
            ObjIncFrame( obj, &Area2 );
            RegionExpand( &Area1, &Area2, &Area1 );
        }
        if( gAnimUnk23[ AnimIdx ].Stat < gAnimUnk23[ AnimIdx ].Ap ){
            t = &gAnimUnk23[ AnimIdx ].Tab[ gAnimUnk23[ AnimIdx ].Stat ];
            ObjMoveToTile( obj, t->GridPos, t->i02, &Area2 );
            RegionExpand( &Area1, &Area2, &Area1 );
            ObjMove( obj, t->i03, t->i04, &Area2 );
            RegionExpand( &Area1, &Area2, &Area1 );
            gAnimUnk23[ AnimIdx ].Stat++;
        }
        if( gAnimUnk23[ AnimIdx ].Stat == gAnimUnk23[ AnimIdx ].Ap && ((gAnimUnk23[ AnimIdx ].Flags & 0x10) == 0 || obj->FrameNo == fpd) ) gAnimUnk23[ AnimIdx ].Stat = -1000;
        TileUpdateArea( &Area1, gAnimUnk23[ AnimIdx ].Obj->Elevation );
        if( gAnimUnk23[ AnimIdx ].Stat == -1000 ) AnimUpdate( gAnimUnk23[ AnimIdx ].AnimIdx, 1 );
    }
}

int AnimUnk17( Obj_t *obj, int a2, int AnimIdx, int a4 )
{
    int Id;

    if( gAnimCnt1 == 24 ) return -1;
    if( a2 == 38 ){
        gAnimUnk23[ gAnimCnt1 ].Flags = 0;
        Id = ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, 38, a4, obj->Orientation + 1 );
    } else {
        Id = ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, a2, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 );
        gAnimUnk23[ gAnimCnt1 ].Flags = a4;
    }
    if( !ArtFileExist( Id ) ) return -1;
    gAnimUnk23[ gAnimCnt1 ].Obj = obj;
    gAnimUnk23[ gAnimCnt1 ].ArtId = Id;
    gAnimUnk23[ gAnimCnt1 ].AnimIdx = AnimIdx;
    gAnimUnk23[ gAnimCnt1 ].Time = 0;
    gAnimUnk23[ gAnimCnt1 ].Speed = AnimGetSpeed(obj, gAnimUnk23[gAnimCnt1].ArtId);
    gAnimUnk23[ gAnimCnt1 ].Stat = 0;
    gAnimUnk23[ gAnimCnt1 ].Ap = 0;
    gAnimCnt1++;
    return 0;
}

// task callback, in game animations process
void AnimProcess()
{
    int ypos, xpos, xoffs, yoffs, i, SysTime, AnimIdx, j, ImgId, n;
    ArtFrmHdr_t *Img;
    VidRect_t Area2, AreaOut, Area1;
    CachePool_t *ImgObj;
    Obj_t *obj;

    if( !gAnimCnt1 ) return;
    gAnimUnk40 = 1;    
    for( i = 0; i < gAnimCnt1; i++ ){
        if( gAnimUnk23[ i ].Stat == -1000 ) continue;
	// test if time passed for given animation
        SysTime = TimerGetSysTime();
        if( (unsigned int)TimerDiff( SysTime, gAnimUnk23[ i ].Time ) < gAnimUnk23[ i ].Speed ) continue;
        gAnimUnk23[ i ].Time = SysTime; // update time marker
	// *** do animation step ***
        obj = gAnimUnk23[ i ].Obj;
        AnimIdx = gAnimUnk23[ i ].AnimIdx;
        if( AnimStateMachine( AnimIdx ) == -1 ) continue;
        if( gAnimUnk23[ i ].Ap > 0 ){
            if( gAnimUnk23[ i ].Flags & 0x02 ){
                AnimMoveReverse( i );
    	    } else {
    		n = obj->GridId;
                AnimMovement( i );
                if( n != obj->GridId ) ScptUnk24( obj, obj->GridId, obj->Elevation );
            }
            continue;
        }
        if( gAnimUnk23[ i ].Stat == 0 ){
            for( j = 0; j < gAnimCnt1; j++ ){
                if( obj == gAnimUnk23[ j ].Obj && gAnimUnk23[ j ].Stat == -2000 ){
                    gAnimUnk23[ j ].Stat = -1000;
                    AnimUpdate( gAnimUnk23[ j ].AnimIdx, 1 );
                }
            }
            gAnimUnk23[ i ].Stat = -2000;
        }
        ObjGetRefreshArea( obj, &Area1 );
        ImgId = obj->ImgId;
        if( ImgId != gAnimUnk23[ i ].ArtId ){
            if( (Img = ArtLoadImg( ImgId, &ImgObj )) ){
        	ArtGetCenter( Img, obj->Orientation, &xpos, &ypos );
    		ArtClose( ImgObj );
            } else {
            	xpos = ypos = 0;
    	    }
            ObjSetShape( obj, gAnimUnk23[ i ].ArtId, &Area2 );
            RegionExpand( &Area1, &Area2, &Area1 );    	    
    	    if( (Img = ArtLoadImg( obj->ImgId, &ImgObj )) ){
            	if( gAnimUnk23[ i ].Flags & 0x01  ){
            	    n = ArtGetFpd( Img ) - 1;
            	} else {
            	    n = 0;
            	}
            	ObjSetFrame( obj, n, &Area2 );
            	RegionExpand( &Area1, &Area2, &Area1 );
                ArtGetFrameShift( Img, obj->FrameNo, obj->Orientation, &xoffs, &yoffs );
                ObjMove( obj, xpos + xoffs, ypos + yoffs, &Area2 );
                RegionExpand( &Area1, &Area2, &Area1 );
                ArtClose( ImgObj );                    
            	TileUpdateArea( &Area1, gMapCurrentLvl );
            	continue;
    	    }
    	    ObjSetFrame( obj, 0, &Area2 );
    	    RegionExpand( &Area1, &Area2, &Area1 );
            TileUpdateArea( &Area1, gMapCurrentLvl );
            continue;
        }
        if( !(gAnimUnk23[ i ].Flags & 0x01) ){
            Img = ArtLoadImg( obj->ImgId, &ImgObj );
            if( !Img ){
        	TileUpdateArea( &Area1, gMapCurrentLvl );
        	continue;
            }
            if( ( gAnimUnk23[ i ].Flags & 0x80 ) || ((ArtGetFpd( Img ) - 1) != obj->FrameNo) ){
            	ObjIncFrame( obj, &Area2 );
            	RegionExpand( &Area1, &Area2, &Area1 );
            	ArtGetFrameShift( Img, obj->FrameNo, obj->Orientation, &xpos, &ypos );
            	ObjMove( obj, xpos, ypos, &Area2 );
            	RegionExpand( &Area1, &Area2, &Area1 );
            	ArtClose( ImgObj );
            	TileUpdateArea( &Area1, gMapCurrentLvl );
            } else {
                gAnimUnk23[ i ].Stat = -1000;
                ArtClose( ImgObj );
                if( (gAnimUnk23[ i ].Flags & 0x40 ) && !ObjUnk33( obj, &AreaOut ) ) TileUpdateArea( &AreaOut, obj->Elevation );
                AnimUpdate( gAnimUnk23[ i ].AnimIdx, 1 );                    
            }
    	    continue;
        }
	if( (gAnimUnk23[ i ].Flags & 0x80) || obj->FrameNo ){
            if( ( Img = ArtLoadImg( obj->ImgId, &ImgObj ) ) ){
                ArtGetFrameShift( Img, obj->FrameNo, obj->Orientation, &xpos, &ypos );
                ArtClose( ImgObj );
            }
            ObjDecFrame( obj, &Area2 );
            RegionExpand( &Area1, &Area2, &Area1 );
            ObjMove( obj, -xpos, -ypos, &Area2 );
            RegionExpand( &Area1, &Area2, &Area1 );
            TileUpdateArea( &Area1, gMapCurrentLvl );
        } else {
            gAnimUnk23[ i ].Stat = -1000;
    	    AnimUpdate( gAnimUnk23[ i ].AnimIdx, 1 );
        }
    }
    gAnimUnk40 = 0;
    AnimClean();    
}

void AnimClean()
{
    int i, j;

    for( i = 0; i != 32; i++ ){
        if( gAnimations[ i ].Flags & 0x20 ) gAnimations[ i ].Flags = 0;
    }    
    for( i = 0; i < gAnimCnt1; i++){
        if( gAnimUnk23[ i ].Stat != -1000 ) continue;
        for( j = i + 1; j < gAnimCnt1; j++ ){
            if( gAnimUnk23[ j ].Stat != -1000 ) break;
        }                
        if( j == gAnimCnt1 ) break;
        if( i != j ){
            memcpy( &gAnimUnk23[ i ], &gAnimUnk23[ j ], sizeof( Anim03_t ) );
            gAnimUnk23[ j ].Stat = -1000;
            gAnimUnk23[ j ].Flags = 0;
        }
    }
    gAnimCnt1 = i;
}

int AnimGetTarget( int *Ap )
{
    int GridIdx,EntryValue,MseY,MseX;

    MseGetCursorPosition( &MseX, &MseY );    
    if( (GridIdx = TileGetPointed( MseX, MseY )) == -1 ) return -1;
    if( IN_COMBAT ){
        if( *Ap != -1 && (gKeyStates[29] || gKeyStates[157]) ){
            IfaceGetWeaponDsc( &MseY, &MseY );
            *Ap -= ItemGetSlotApCost( gObjDude, MseY, MseY );
            if( *Ap <= 0 ) return -1; // not enough Action points
        }
    } else {
        CfgGetInteger( &gConfiguration, "system", "interrupt_walk", &EntryValue );
        if( EntryValue ) AnimRegClear( gObjDude );
    }
    return GridIdx;
}

int AnimWalk( int Ap )
{
    int GridPos;

    if( (GridPos = AnimGetTarget( &Ap )) == -1 ) return -1;
    if( GridPos == gAnimRunPos ) return AnimRun( Ap );        
    gAnimRunPos = GridPos;
    AnimRegStart( 2 );
    AnimObjMoveToTile( gObjDude, GridPos, gObjDude->Elevation, Ap, 0 );
    return AnimRegEnd();        
}

int AnimRun( int Ap )
{
    int TargetGridId;
    
    if( (TargetGridId = AnimGetTarget( &Ap )) == -1 ) return -1;
    if( !PerkLvl( gObjDude, PERK_SILENT_RUNNING ) ) CritterUnk36( 0 );
    AnimRegStart( 2 );
    AnimObjRunToTile( gObjDude, TargetGridId, gObjDude->Elevation, Ap, 0 );
    return AnimRegEnd();    
}

// task callback, fidgets animation
void AnimAmbient()
{
    VidRect_t Area;
    Obj_t *obj;
    char fname[40];
    int ObjCount, Time;
    int v7;
    int Speed;

    if( gMenuEscape || IN_COMBAT ) return;
    if( RecGetState() != 2 ) return;
    if( gObjDude->Flags & 1 ) return;   
    Time = TimerGetTime();

    if( TimerDiff( Time, gAnimPlayTime ) <= gAnimPlaySpeed ) return;    
    gAnimPlayTime = Time;
    ObjCount = 0;
    for( obj = ObjGetVisibleObjectFirst( gObjDude->Elevation ); obj; obj = ObjGetVisibleObjectNext() ){
        if( ObjCount >= 100 ) break;
        if( (obj->Flags & 0x01) || (OBJTYPE( obj->ImgId ) != TYPE_CRIT) || ((obj->ImgId & 0xFF0000) >> 16) ) continue;
        if( CritterIsDead( obj ) ) continue;
        ObjGetRefreshArea( obj, &Area );
        if( !RegionShrink( &Area, &gVidMainGeo, &Area ) && ( gMap.MapId != 97 || obj->Pid != 0x10000FA ) ) gAnimAmbientCritter[ ObjCount++ ] = obj;
    }
    Speed = 7;
    if( ObjCount ){
        obj = gAnimAmbientCritter[ RandMinMax( 0, ObjCount - 1 ) ];
        AnimRegStart( 513 );
        v7 = 0;
        if( obj == gObjDude ){
    	    v7 = 1;
        } else {
    	    *fname = '\0';
    	    ArtGetFilenameFromList( 1, obj->ImgId & 0xFFF, fname );
    	    if( *fname == 'm' || *fname == 'M' ){
        	if( ObjGetDistance( obj, gObjDude ) <= 2 * FeatGetVal( gObjDude, FEAT_PERCEPTION ) ) v7 = 1;
    	    }
        }
        if( v7 ) AnimRegPlaySfx( obj, GSoundCharacterFileName( obj, 0, 0 ), 0 );        
        AnimRegAnim( obj, 0, 0 );
        AnimRegEnd();
        Speed = 20 / ObjCount;
        if( 20 / ObjCount < 1 ){ 
    	    gAnimPlaySpeed = RandMinMax( 0, 3000 ) + 1000; 
    	    return; 
    	}
    }
    if( Speed > 7 ) Speed = 7;
    gAnimPlaySpeed = RandMinMax( 0, 3000 ) + 1000 * Speed;
}

void AnimUnk24( Obj_t *obj, unsigned int Orientation, int ArtId )
{
    CachePool_t *ImgObj;
    ArtFrmHdr_t *Art;
    VidRect_t Area2, v20;
    int n,Ypos,i,Xoffs,Yoffs,Fpd,Xpos;

    ObjSetRotation( obj, Orientation, &Area2 );
    n = (obj->ImgId & 0xF000) >> 12;
    Xpos = Ypos = 0;
    if( n ){
        if( ArtId == -1 ){
            Art = ArtLoadImg( ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, 38, n, obj->Orientation + 1), &ImgObj );
            if( Art ){
                Fpd = ArtGetFpd( Art );
                for( i = 0; i < Fpd; i++ ){
                    ArtGetFrameShift( Art, i, obj->Orientation, &Xoffs, &Yoffs );
                    Ypos += Yoffs;
                    Xpos += Xoffs;
                }
                ArtClose( ImgObj );
                if( (Art = ArtLoadImg( ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, 0, 0, obj->Orientation + 1 ), &ImgObj )) ){
                    if( !ArtGetCenter( Art, obj->Orientation, &Xoffs, &Yoffs ) ){
                        Xpos += Xoffs;
                        Ypos += Yoffs;
                    }
                    ArtClose( ImgObj );
                }
            }
        }
    }
    if( ArtId == -1 )
        ArtId = ArtMakeId( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, ( (obj->ImgId & 0xFF0000) >> 16 == 33 ) ? 33 : 0, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 );
    ObjSetShape( obj, ArtId, &v20 );
    RegionExpand( &Area2, &v20, &Area2 );
    ObjMoveToTile( obj, obj->GridId, obj->Elevation, &v20 );
    RegionExpand( &Area2, &v20, &Area2 );
    ObjSetFrame( obj, 0, &v20 );
    RegionExpand( &Area2, &v20, &Area2 );
    ObjMove( obj, Xpos, Ypos, &v20 );
    RegionExpand( &Area2, &v20, &Area2 );
    TileUpdateArea( &Area2, obj->Elevation );
}

int AnimUnk25( Obj_t *obj )
{
    int n;

    AnimRegStart( 2 );
    AnimRegAnim( obj, ( (obj->ImgId & 0xFF0000) >> 16 == 20 ) ? 37 : 36, 0 );
    n = AnimRegEnd();
    obj->Scenery.i07 &= ~0x02;
    return n;
}

int AnimUnk26( Obj_t *obj, int MapIdx, int a2 )
{
    if( !CritterUnk31( obj ) ){
        AnimUnk24( obj, TileTurnAt( obj->GridId, MapIdx ), -1 );
    }
    AnimUpdate( a2, 0 );
    return 0;
}

int AnimUnk27( Obj_t *obj, int turn, int AnimIdx )
{
    int ori;

    if( !CritterUnk31( obj ) ){
        ori = obj->Orientation + turn;
        if( ori >= 0 ) {
            if( ori >= 6 ) ori = 0;
        } else {
            ori = 5;
        }
        AnimUnk24( obj, ori, -1 );
    }
    AnimUpdate( AnimIdx, 0 );
    return 0;
}

int AnimUnk28( Obj_t *obj, int AnimIdx )
{
    VidRect_t Area;

    if( !ObjUnk33( obj, &Area ) ) TileUpdateArea( &Area, obj->Elevation );
    if( AnimIdx != -1 ) AnimUpdate( AnimIdx, 0 );
    return 0;
}

int AnimUnk29( Obj_t *obj, int AnimIdx, int a3 )
{
    VidRect_t v7, v8;

    if( (a3 & 0xFF0000) >> 16 ) {
        ObjSetShape( obj, a3, &v8 );
        ObjSetFrame( obj, 0, &v7 );
        RegionExpand( &v8, &v7, &v8 );
        TileUpdateArea( &v8, obj->Elevation );
    } else {
        AnimUnk24( obj, obj->Orientation, a3 );
    }
    AnimUpdate( AnimIdx, 0 );
    return 0;
}

void AnimReset()
{
    int i;

    gAnimUnk41 = 1;
    gAnimIdx = -1;
    for( i = 0; i < 32; i++ ) AnimEnd( i );
    gAnimUnk41 = 0;
    gAnimCnt1 = 0;
}

int AnimUnk30( int GridIdx, int Cnt )
{
    int x, y, i;

    for( i = Cnt; i > 0; i-- ){
        TileGetScrCoordinates( GridIdx, &x, &y );
        if( ArtMakeId( 4, gMapIsoGrid[ i ][ TileGetGridIdx( x + 2, y + 8 ) ] & 0xFFF, 0, 0, 0 ) != ArtMakeId( 4, 1, 0, 0, 0 ) ) break;
    }    
    return i;
}

unsigned int AnimGetSpeed( Obj_t *dude, int ImgId )
{
    ArtFrmHdr_t *Img;
    unsigned int speed;
    int player_speed, combat_speed;	
    CachePool_t *ImgObj;

    combat_speed = player_speed = 0;    
    if( (Img = ArtLoadImg( ImgId, &ImgObj )) ){
        speed = ArtGetObjSpeed( Img );
        ArtClose( ImgObj );
    } else {
        speed = 10;
    }
    if( IN_COMBAT && (ImgId & 0xFF0000) >> 16 == 1 ){
        CfgGetInteger( &gConfiguration, "preferences", "player_speedup", &player_speed );
        if( dude != gObjDude || player_speed == 1 ){
            CfgGetInteger( &gConfiguration, "preferences", "combat_speed", &combat_speed );
            speed += combat_speed;
        }
    }
    return 1000 / speed;
}



