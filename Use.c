#include "FrameWork.h"

Obj_t *gUnk123 = NULL;

int UseGetScriptId( Obj_t *obj, int *pScrId )
{
    return ((*pScrId = obj->ScrId) != -1) - 1;
}

int UseCreateScript( Obj_t *Obj, int *ScrId )
{
    int Flags;
    Scpt_t *scr;
    Proto_t *proto;

    *ScrId = -1;
    if( ProtoGetObj( Obj->Pid, &proto ) == -1 ) return -1;
    switch( OBJTYPE( Obj->Pid ) ){
	case 0 ... 3: Flags = proto->AttackMode; break;
	case 4: Flags = proto->Flags; break;
	case 5: Flags = -1; break;
	default: Flags = proto->AttackMode; break;
    }
    if( Flags == -1 ) return -1;
    if( ScptNewScript( ScrId, Flags >> 24 ) == -1 ) return -1;
    if( ScptPtr( *ScrId, &scr ) == -1 ) return -1;
    scr->LocVarId = Flags & 0xFFFFFF;
    if( OBJTYPE( Obj->Pid ) == TYPE_CRIT ) Obj->ScrFNameId = scr->LocVarId;
    if( Flags >> 24 == 1 ){
        scr->HexOrTimer = (Obj->Elevation << 29) | Obj->GridId;
        scr->Radius = 3;
    }
    if( Obj->TimeEv == -1 ) Obj->TimeEv = ScptNewObjId();
    scr->i08 = Obj->TimeEv;
    scr->TimeEv = Obj;
    ScptSetupLocalVars( Flags & 0xFFFFFF, *ScrId );
    return 0;
}

int UseUnk03( Obj_t *obj, int ObjType, int VarPid )
{
    int v6, Id;
    Scpt_t *pScript;

    if( VarPid == -1 ) return -1;
    if( ScptNewScript(&Id, ObjType) == -1 ) return -1;
    if( ScptPtr(Id, &pScript) == -1 ) return -1;
    pScript->LocVarId = VarPid;
    if( ObjType == 1 ){
        pScript->HexOrTimer = (obj->Elevation << 29) | obj->GridId;
        pScript->Radius = 3;
    }
    obj->ScrId = Id;
    v6 = ScptNewObjId();
    obj->TimeEv = v6;
    pScript->i08 = v6;
    pScript->TimeEv = obj;
    ScptSetupLocalVars( VarPid & 0xFFFFFF, Id );
    if( OBJTYPE( obj->Pid ) == 1 ) obj->ScrFNameId = pScript->LocVarId;
    return 0;        
}

int UseUnk02( Obj_t *Critter )
{
    Proto_t *proto;

    if( OBJTYPE( Critter->Pid ) ) return 0;
    if( ProtoGetObj( Critter->Pid, &proto ) == -1 ) return 0;
    if( Critter->Critter.Box.Cnt )
        return Critter->Critter.Box.Box->obj->Critter.State.Reaction;
    else
        return 0;
}

int UseUnk01( Obj_t *Critter, int Reaction )
{
    Proto_t *proto;

    if( OBJTYPE( Critter->Pid ) != TYPE_ITEM ) return -1;
    if( ProtoGetObj( Critter->Pid, &proto ) == -1 ) return -1;
    if( Critter->Critter.Box.Cnt == 0 ) return -1;
    if( Reaction > Critter->Container.Box.Box->obj->Critter.State.Reaction ) return -1;
    Critter->Container.Box.Box->obj->Critter.State.Reaction -= Reaction;
    return Critter->Container.Box.Box->obj->Critter.State.Reaction;
    return 0;
}

void UseLookMsg( Obj_t *crit, Obj_t *obj )
{
    UseLook( crit, obj, IfcMsgOut );
}

void UseLook( Obj_t *crit, Obj_t *obj, void (*OutCb)(char *))
{
    int Override;
    char stmp[ 260 ];
    MsgLine_t msg;
    Scpt_t *scr;
    Proto_t *proto;

    Override = 0;
    if( CritterIsDead( crit ) ) return;
    if( OBJTYPE( obj->ImgId ) == TYPE_TILE ) return;
    if( ProtoGetObj( obj->Pid, &proto ) == -1 ) return;
    if( obj->ScrId != -1 ){
        ScptSetup( obj->ScrId, crit, obj );
        ScptRun( obj->ScrId, SCPT_AEV_LOOK_AT_P_PROC );
        if( ScptPtr( obj->ScrId, &scr ) == -1 ) return;
        Override = scr->OverrideFlag;
    }
    if( !Override ){
        if( OBJTYPE( obj->Pid ) == TYPE_CRIT && CritterIsDead( obj ) )
            msg.Id = 491 + RandMinMax(0, 1);
        else
            msg.Id = 490;                   // look text
        if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ){
            sprintf( stmp, msg.Text, ObjGetName( obj ) );
            OutCb( stmp );
        }
    }
}

int UseExamineMsg( Obj_t *a1, Obj_t *a2)
{
    return UseExamine( a1, a2, IfcMsgOut );
}

int UseExamine( Obj_t *critter, Obj_t *obj, void (*OutCb)(char *) )
{
    int i18;
    char *v10;
    char *Dsc;
    unsigned int type;
    int idx;
    int v15;
    int v17;
    int Val;
    Obj_t *RHandObj;
    Obj_t *obj2;
    unsigned int ObjType;
    int v42;
    char stmp1[260];
    char stmp2[80];
    MsgLine_t msg3;
    MsgLine_t msg1;
    MsgLine_t msg4;
    MsgLine_t msg2;
    Scpt_t *script;

DD
    i18 = 0;
    if( CritterIsDead( critter ) ) return -1;
    if( OBJTYPE( obj->ImgId ) == TYPE_TILE ) return -1;
    if( obj->ScrId != -1 ){
        ScptSetup( obj->ScrId, critter, obj );
        ScptRun( obj->ScrId, SCPT_AEV_DESCRIPTION_P_PROC );
        if( ScptPtr( obj->ScrId, &script ) == -1 ) return -1;
        i18 = script->OverrideFlag;
    }
    if( !i18 ){
        Dsc = ObjGetDsc( obj );
	v10 = Dsc;
        if( Dsc && !strcmp( Dsc, gProtoUnk100[0] ) ) v10 = NULL;
        if( !v10 || !*v10 ){
            msg1.Id = 493; // 'YouSee nothing out of the ordinary'
            if( MessageGetMsg( &gProtoMessages, &msg1 ) != 1 ) eprintf( "\nError: Can't find msg num!" );
            Dsc = msg1.Text;
            OutCb( Dsc );
        } else {
    	    if( OBJTYPE( obj->Pid ) != TYPE_CRIT || !CritterIsDead( obj ) ){
		Dsc = v10;
        	OutCb( Dsc );
    	    }
        }
    }
    if( !critter || critter != gObjDude ) return 0;

    type = OBJTYPE( obj->Pid );
    if( type == TYPE_CRIT ){
        if( (obj == gObjDude) || !PerkLvl( gObjDude, 0 ) || CritterIsDead( obj ) ){
            idx = 0;
            if( CritterUnk30( obj ) ) idx = -2;
            v17 = FeatGetVal( obj, 35 );
            v15 = FeatGetVal( obj, 7 );
            if( v17 <= 0 || CritterIsDead( obj ) ){
                v17 = 0;
            } else if( v17 == v15 ){
                v17 = 4;
            } else {
                Val = FeatGetVal( obj, 7 );
                v17 /= (Val + 1);
            }
            msg1.Id = v17 + 500; // Examine text 'dead/almost dead,seve ....
            if( MessageGetMsg( &gProtoMessages, &msg1 ) != 1 ){ eprintf( "\nError: Can't find msg num!" ); exit( 1 ); }
            if( v17 > 4 ){
                msg1.Id = 550; // examine text error msg
                if( MessageGetMsg( &gProtoMessages, &msg1) != 1 ){ eprintf( "\nError: Can't find msg num!" ); exit( 1 ); }
                eprintf( "%s", msg1.Text );
                return 0;
            }
            if( obj == gObjDude ){
                msg2.Id = idx + 520;
                if( MessageGetMsg( &gProtoMessages, &msg2 ) != 1 ){ eprintf( "\nError: Can't find msg num!" ); exit( 1 ); }
                sprintf( stmp1, msg2.Text, msg1.Text );
            } else {
                msg2.Id = idx + 521; // examine text
                if( MessageGetMsg( &gProtoMessages, &msg2 ) != 1 ){ eprintf( "\nError: Can't find msg num!" ); exit( 1 ); }
                msg3.Id = FeatGetVal(obj, 34) + 522;
                if( MessageGetMsg( &gProtoMessages, &msg3 ) != 1 ){ eprintf( "\nError: Can't find msg num!" ); exit( 1 ); }
                sprintf( stmp1, msg3.Text, msg1.Text );
            }
        } else {
            msg3.Id = ( CritterGetBodyType(obj) ) ? 537 : (FeatGetVal( obj, 34 ) + 535);
            RHandObj = InvGetRHandObj( obj );
            obj2 = RHandObj;
            if( RHandObj && ItemGetObjType( RHandObj ) != PR_ITEM_WEAPON ) obj2 = NULL;
            if( MessageGetMsg( &gProtoMessages, &msg3 ) != 1 ){ eprintf( "\nError: Can't find msg num!" ); exit( 1 ); }
            if( obj2 ){
                msg4.Id = 546;                  // examine wielding
                if( Item48( obj2 ) ) msg4.Id++;
                if( MessageGetMsg( &gProtoMessages, &msg4 ) != 1 ){ eprintf( "\nError: Can't find msg num!" ); exit( 1 ); }
                sprintf( stmp2, "%s%s", msg3.Text, msg4.Text );
                if( Item48( obj2 ) ){
                    sprintf( stmp1, stmp2, FeatGetVal( obj, 35 ), 
                	FeatGetVal( obj, 7 ), ObjGetName( obj2 ), ItemGetAmmo( obj2 ), ItemGetPackQuantity( obj2 ), ProtoGetObjName( ItemGetAmmoId( obj2 ) ) );
                } else {
                    sprintf( stmp1, stmp2, FeatGetVal( obj, 35 ), FeatGetVal( obj, 7 ), ObjGetName( obj2 ) );
                }
            } else {
                msg4.Id = 545;
                if( CritterUnk30( obj ) ) msg4.Id--;
                if( MessageGetMsg( &gProtoMessages, &msg4 ) != 1 ){ eprintf( "\nError: Can't find msg num!" ); exit( 1 ); }
                sprintf( stmp1, msg3.Text, FeatGetVal( obj, 35 ), FeatGetVal( obj, 7 ) );
                strcpy( &stmp1[ strlen( stmp1 ) ], msg4.Text );
            }
        }
        if( CritterUnk30(obj) ){
            msg3.Id = ( FeatGetVal( obj, 35 ) >= FeatGetVal( obj, 7 ) ) + 530;           // examine woman
            if( obj == gObjDude ) msg3.Id += 2;
            if( MessageGetMsg( &gProtoMessages, &msg3 ) != 1 ){ eprintf( "\nError: Can't find msg num!" ); exit( 1 ); }
            strcpy( &stmp1[ strlen( stmp1 ) ], msg3.Text );
        }
        OutCb( stmp1 );
        return 0;
    }
    if( type ){
        if( type != TYPE_SCEN || obj->Pid != 0x20003f1 ) return 0;
        v42 = GlobVarGet( 18 );
        msg1.Id = 549; // examine car
        if( !v42 ) msg1.Id = 548;
        if( MessageGetMsg((Msg_t *)&gProtoMessages, &msg1) != 1 ) eprintf( "\nError: Couldn't find message!" );
        if( v42 ){
            sprintf( stmp1, msg1.Text, 100 * WmCarFuelGauge() / 80000 ); // gas gauge
        } else {
            strcpy( stmp1, msg1.Text );
        }
        OutCb( stmp1 );
        return 0;
    }
    ObjType = ItemGetObjType(obj);
    if( ObjType == PR_ITEM_WEAPON ){
        if( Item48( obj ) ){
            msg1.Id = 526;                      // it has ...
            if( MessageGetMsg( &gProtoMessages, &msg1 ) != 1 ) exit( 1 );
            sprintf(stmp1, msg1.Text, ItemGetAmmo( obj ), ItemGetPackQuantity( obj ), ProtoGetObjName( ItemGetAmmoId( obj ) ) );
            OutCb(stmp1);
        }
        return 0;
    }
    if( ObjType != PR_ITEM_AMMO ) return 0;
    msg1.Id = 510;
    if( MessageGetMsg( &gProtoMessages, &msg1 ) != 1 ) exit( 1 );
    sprintf( stmp1, msg1.Text, ItemGetAcMod( obj ) );
    OutCb( stmp1 );
    msg1.Id++;
    if( MessageGetMsg( &gProtoMessages, &msg1 ) != 1 ) exit( 1 );
    sprintf( stmp1, msg1.Text, ItemGetDrMod( obj ) );
    OutCb( stmp1 );
    msg1.Id++;// 512
    if( MessageGetMsg( &gProtoMessages, &msg1 ) != 1 )exit( 1 );
    sprintf( stmp1, msg1.Text, ItemGetDmgMinMod( obj ), ItemGetDmgMaxMod( obj ) );
    OutCb( stmp1 );
    return 0;
}

int UseUseOn( Obj_t *crit, Obj_t *obj )
{
    int i18, Money, v10;
    MsgLine_t msg;
    VidRect_t Area;
    Scpt_t *scr;
DD
    i18 = 0;
    if( obj->ScrId != -1 ){
        ScptSetup( obj->ScrId, crit, obj );
        ScptRun( obj->ScrId, SCPT_AEV_PICKUP_P_PROC );
        if( ScptPtr( obj->ScrId, &scr ) == -1 ) return -1;
        i18 = scr->OverrideFlag;
    }
    if( !i18 ){
        if( obj->Pid == PID_MONEY ){
            Money = ItemGetMoney( obj );
            if( Money <= 0 ) Money = 1;
            v10 = Item09( crit, obj, Money );
            if( !v10 ) ItemSetMoney( obj, 0 );
        } else {
            v10 = Item09( crit, obj, 1 );
        }
        if( v10 ){
            msg.Id = 905; //  Use on messages
            if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
        } else {
            ObjLightItem( obj, &Area );
            TileUpdateArea( &Area, obj->Elevation );
        }
    }
    return 0;
}

int UseUnk04( Obj_t *a1, Obj_t *a2)
{
    int v5, v9, v16;
    VidRect_t Rect;
    Proto_t *proto;

    v5 = 1;
    if( InvGetRHandObj( a1 ) == a2 ){
        if( a1 != gObjDude || IfaceGetSelectedHand() ){
            v9 = 2;
            ObjSetShape( a1, ArtMakeId( 1, a1->ImgId & 0xFFF, ( a1->ImgId & 0xFF0000 ) >> 16, 0, a1->Orientation ), &Rect );
        }
    } else {
        if( InvGetLHandObj( a1 ) == a2 ){
            if( a1 == gObjDude && !IfaceGetSelectedHand() ){
                v9 = 2;
        	ObjSetShape( a1, ArtMakeId( 1, a1->ImgId & 0xFFF, ( a1->ImgId & 0xFF0000 ) >> 16, 0, a1->Orientation ), &Rect );
            }
        } else {
            if( InvGetArmorObj( a1 ) == a2 && a1 == gObjDude ){
                if ( ProtoGetObj(0x1000000, &proto) != -1 ) v5 = proto->ImgId & 0xFFF;
                v9 = 3;
        	ObjSetShape( a1, ArtMakeId( 1, v5, ( a1->ImgId & 0xFF0000 ) >> 16, ( a1->ImgId & 0xF000 ) >> 12, a1->Orientation ), &Rect );
            }
        }
    }
    v16 = ItemUseItem( a1, a2, 1 );
    if( v9 ){
        if( v9 >= 2 ) TileUpdateArea( &Rect, a1->Elevation );
        if( v9 <= 2 && a1 == gObjDude ) IfaceHandSlotUpdate( 0, -1, -1 );
    }
    return v16;
}

int UseDropObj( Obj_t *crit, Obj_t *obj )
{
    int i18;
    Obj_t *Owner;
    VidRect_t v11;
    Scpt_t *scr;

    i18 = 0;
    if( !obj ) return -1;
    if( crit->ScrId != -1 ){
        ScptSetup( crit->ScrId, obj, 0 );
        ScptRun( crit->ScrId, SCPT_AEV_IS_DROPPING_P_PROC );
        if( ScptPtr( crit->ScrId, &scr ) == -1 ) return -1;
        i18 = scr->OverrideFlag;
    }
    if( !i18 ){
        if( obj->ScrId != -1 ){
            ScptSetup( obj->ScrId, crit, obj );
            ScptRun( obj->ScrId, SCPT_AEV_DROP_P_PROC );
            if( ScptPtr( obj->ScrId, &scr ) == -1 ) return -1;
            i18 = scr->OverrideFlag;
        }
        if( !i18 && !UseUnk04( crit, obj ) ){
            if( !(Owner = ObjGetOwner( crit ) ) ) Owner = crit;
            ObjUnk14(obj, Owner->GridId, Owner->Elevation, &v11);
            TileUpdateArea( &v11, Owner->Elevation );
        }
    }
    return 0;
}

int UseUnk06( Obj_t *obj )
{
    VidRect_t rect;
DD
printf(">>%p\n", obj );
    if( !obj ) return -1;
DD
    if( obj->Owner ) UseUnk04( obj->Owner, obj );
DD
    EvQeRmEvent( obj );
DD
    ObjDestroy( obj, &rect );
DD
    if( !obj->Owner ) TileUpdateArea( &rect, obj->Elevation );
DD
    return 0;
}

int UseReadBook( Obj_t *obj )
{
    int TxtId, SkillIdx, SkillPts, i;
    MsgLine_t msg;

    TxtId = -1;
    switch( obj->Pid ){
	case PID_BOOKBIGSCI:  
	    TxtId = 802; SkillIdx = SKILL_SCIENCE; break; // 'You learn new science information.'
	case PID_BOOKELECTRO:  
	    TxtId = 803; SkillIdx = SKILL_REPAIR; break; // 'You learn a lot about repairing broken electronics.'
	case PID_BOOKFIRSTAID:  
	    TxtId = 804; SkillIdx = SKILL_FIRSTAID;  break; // 'You learn new ways to heal injury.'
	case PID_BOOK_SCOUT:  
	    TxtId = 806; SkillIdx = SKILL_OUTDOORSMAN; break; // 'You learn a lot about wilderness survival'
	case PID_GUNSANDBULLETS: 
	    TxtId = 805; SkillIdx = SKILL_SMALLGUNS; break; // 'You learn how to handle your guns better'
	default: return -1;
    }

    if( IN_COMBAT ){
        msg.Id = 902; // 'You cannot do that in combat '
        if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
        return 0;
    }
    SkillPts = (100 - SkillGetTotal( gObjDude, SkillIdx ) ) / 10;
    if( SkillPts <= 0 ){
        TxtId = 801; //'You learn nothing new.'
    } else {
        if( PerkLvl( gObjDude, PERK_COMPREHENSION ) ) SkillPts = 150 * SkillPts / 100;
        for( i = 0; i < SkillPts; i++ ){
            SkillInc( gObjDude, SkillIdx );
        }
    }
    FadeStep( gFadePaletteC );
    ScptTimeCap2( 3600 * (11 - FeatGetVal( gObjDude, FEAT_INTELLIGENCE ) ) );
    ScptMapUpdate();
    FadeStep( gPalBase );
    msg.Id = 800; // 'You read the book.'
    if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
    msg.Id = TxtId;
    if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
    return 1;
}

int UseLightFlare( Obj_t *crit, Obj_t *obj )
{
    MsgLine_t msg;

    if( obj->Pid != PID_FLARE ) return -1;
    if( obj->Flags & 0x2000 ){
        if( crit == gObjDude ){
            msg.Id = 588; // 'The flare is already lit.'
            if ( MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
        }
    } else {
        if( crit == gObjDude ){
            msg.Id = 587; // 'You light the flare.'
            if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
        }
        obj->Pid = PID_LIGHTEDFLARE;
        ObjSetLight( obj, 8, 0x10000, 0 );
        EvQeSchedule( 72000, obj, NULL, EV_FLARE_TIMER );
    }
    return 0;
}

int UseRunScript( Obj_t *obj )
{
    Scpt_t *scr;

    if( obj->ScrId == -1 ) return -1;
    ScptSetup( obj->ScrId, gObjDude, obj );
    ScptRun( obj->ScrId, SCPT_AEV_USE_P_PROC );
    if( ScptPtr( obj->ScrId, &scr ) == -1 ) return -1;
    return 0;    
}

int UseSetTimer( Obj_t *obj )
{
    int TimeSet, time, skill;
    MsgLine_t msg;

    if( 
	obj->Pid != PID_DYNAMITE1 && 
	obj->Pid != PID_EXPL_PLASTIC && 
	obj->Pid != PID_DYNAMITE && 
	obj->Pid != PID_PLASTICEXPLOSIVES
      ) return -1;
    if( obj->Flags & OBJ_FLG_TIMERTICKS ){
        msg.Id = 590; // 'The timer is already ticking!'
        if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
        return 2;
    } 
    // set up timer
    TimeSet = InvSetTimer( obj );
    if( TimeSet == -1 ) return 2;    
    // schedule event
    msg.Id = 589; // 'You set the timer.'
    if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
    if( obj->Pid == PID_DYNAMITE1 ){
        obj->Pid = PID_DYNAMITE;
    } else if( obj->Pid == PID_EXPL_PLASTIC ){
        obj->Pid = PID_PLASTICEXPLOSIVES;
    }
    TimeSet = 10 * TimeSet;
    if( PerkLvl( gObjDude, PERK_DEMOLITION_EXPERT ) )
        time = 2;
    else
        time = SkillUse( gObjDude, SKILL_TRAPS, 0, 0 );
    if( time ){
        if( time == 1 ){
            skill = 11;
            time = TimeSet / 2;
        } else {
            skill = 8;
            time = TimeSet;
        }
    } else {
        skill = 11;
    }
    EvQeSchedule( time, obj, NULL, skill );        
    return 2;
}

int UseCarRefuel( Obj_t *obj )
{
    int n, TxtId;
    MsgLine_t msg;

    memset( &msg, 0, sizeof( msg ) );
    if( obj->Pid == PID_SMALLENERGYCELL ){
        n = 16000;
    } else {
        if( obj->Pid != PID_MICROFUSIONCELL ) return -1;
        n = 40000;
    }
    if( WmCarFuelGauge() >= 80000 ){
        TxtId = 596; // 'The car is already full of power.'
    } else {
        if( WmCarRefuel( (ItemGetAmmo( obj ) * n) / ItemGetPackQuantity( obj ) ) == -1 ) return -1;
        TxtId = 595; // 'You charge the car with more power.'
    }
    IfcMsgOut( MessageGetMessage( &gProtoMessages, &msg, TxtId ) );
    return 1;
}

int UseUseMisc( Obj_t *crit )
{
    int ScrId;
    Scpt_t *scr;

    if( !crit ) return -1;
    if( crit->Pid != PID_RAMIREZBOXCLOSED && // 5 jets
	crit->Pid != PID_RAIDERSMAP &&  // add location
	crit->Pid != PID_CATSPAWISS5 && // increase energy weapon
	crit->Pid != PID_PIPBOYLINGUALENCHANCER && // increase speech skill 
	crit->Pid != PID_SURVEYMAP && // add location
	crit->Pid != PID_PIPBOYMEDENCHANCER // doctor skill +10%
    ) return -1;    
    if( ( ScrId = crit->ScrId ) == -1 ) return 1;
    ScptSetup( crit->ScrId, gObjDude, crit );
    ScptRun( ScrId, SCPT_AEV_USE_P_PROC );
    if( ScptPtr( ScrId, &scr ) != -1 ) return -1;
    return 1;    
}

int UseUnk13( Obj_t *crit, Obj_t *obj )
{
    MsgLine_t msg;
    int result;

    switch( ItemGetObjType( obj ) ){
        case 2: return -1;
        case 3: case 5:
            if( (result = UseReadBook( obj )) == -1 ){                
                if( (result = UseLightFlare( crit, obj )) ){                    
                    if( (result = UseUseMisc( obj )) == -1 ){                        
                        if( (result = UseRunScript( obj )) ){                            
                            if( (result = UseSetTimer( obj )) ){
                                if( result != 2 ){
                                    if ( !Item87( obj ) ) break;
                                    if( (result = ItemRecharge( crit, obj )) ) break;
                                }
                            }
                        }
                    }
                }
            }
            return result;
    }
    msg.Id = 582;
    if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
    return -1;
}

void UseUseExplosives( Obj_t *obj )
{
    int i, GroupId;
    Obj_t *v2, *v5;
    Combat_t cmbt;
    Scpt01_t scr;

    if( obj->Pid == PID_DYNAMITE || obj->Pid == PID_PLASTICEXPLOSIVES ) return;
    CombatSetUp( &cmbt, gObjDude, 0, 4, 3 );
    cmbt.DudeInjuries = 256;
    cmbt.TileNo = gObjDude->GridId;
    CombatExplosion( &cmbt, 0, 0, 1 );
    v2 = NULL;
    GroupId = gObjDude->Critter.State.GroupId;
    for( i = 0; i < cmbt.Count; i++ ){
        v5 = cmbt.obj[ i ];
        if ( v5 != gObjDude && GroupId != v5->Critter.State.GroupId && FeatDice( v5, 1, 0, 0 ) >= 2 ) {
            CritterUnk45( cmbt.obj[ i ], gObjDude );
            if( !v2 ) v2 = cmbt.obj[ i ];
        }
    }
    if( !v2 || IN_COMBAT ) return;
    scr.obj = v2;
    scr.crit = gObjDude;
    memset( &scr.i03, 0, 16 );
    scr.i07 = 99999;
    scr.i08 = 0;
    ScptUnk121( &scr );
}

int UseUnk15( Obj_t *crit, Obj_t *obj )
{
    Obj_t *Owner, *v8;
    VidRect_t Rect;
    int lh, rh, v14, v3;

    v3 = UseUnk13( crit, obj );
    if( v3 == 1 || v3 == 2 ){        
        if( (Owner = ObjGetOwner( obj )) ){
            ItemUseItem( Owner, obj, 1 );
            v14 = obj->Flags & 0x3000000;
            v8 = Item35( Owner, obj, v14 );
            if( Owner == gObjDude ){ // drop down item
                IfaceUnk16( &lh, &rh );
                if( !v8 ){
                    if( v14 & 0x1000000 ){
                        lh = -1;
                    } else {
                        if( !(v14 & 0x2000000) ) lh = -1;
                        rh = -1;
                    }
                }
                IfaceHandSlotUpdate( 0, lh, rh );
            }
        }
        if( v3 == 1 ){
            UseUnk06( obj );
        } else if( v3 == 2 && Owner ){
            ObjUnk14( obj, Owner->GridId, Owner->Elevation, &Rect );
            TileUpdateArea( &Rect, Owner->Elevation );
            UseUseExplosives( obj );
        }
        v3 = 0;
    }
    ScptMapUpdate();
    return v3;
}

int UseUnk16( Obj_t *crit, Obj_t *obj1, Obj_t *obj2 )
{
    int tmp;
    char stmp[ 100 ];
    MsgLine_t msg;

    switch( ItemGetObjType( obj2 ) ){
        case PR_ITEM_DRUG:
            if( OBJTYPE( obj1->Pid ) != TYPE_CRIT ){
                if( crit != gObjDude ) return -1;
                msg.Id = 582; // 'That does nothing.'
                if( MessageGetMsg( &gProtoMessages, &msg ) != 1 ) return -1;
                IfcMsgOut( msg.Text );
                return -1;
            }
            if( CritterIsDead( obj1 ) ){
                msg.Id = RandMinMax( 0, 3 ) + 583; // 'To your dismay, you realize that it is already dead.', 'Alas, you are too late.', ..
                if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
                return -1;
            }
            tmp = ItemUnk07( obj1, obj2 );
            if( crit == gObjDude && obj1 != gObjDude ){
                msg.Id = (obj1 != gObjDude ) + 580; // 'You use the %s'
                if( MessageGetMsg( &gProtoMessages, &msg ) != 1 ) return -1;
                sprintf( stmp, msg.Text, ObjGetName( obj2 ), ObjGetName( obj1 ) );
                IfcMsgOut( stmp );
            }
            if( obj1 == gObjDude ) IfaceRenderHP( 1 );
            return tmp;
        case PR_ITEM_WEAPON: case PR_ITEM_MISC:
            if( UseLightFlare( crit, obj2 ) ) break;
            return 0;
        case PR_ITEM_AMMO:
            if( UseCarRefuel( obj2 ) != 1 ) break;
            return 0;
        default:
    }
    msg.Id = 582; // 'That does nothing.'
    if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ){
        sprintf( stmp, "%s", msg.Text );
        IfcMsgOut( stmp );
    }
    return -1;
}

unsigned int UseUseHealSkill( Obj_t *crit, Obj_t *obj1, Obj_t *obj2 )
{
    int MsgId, v7;
    unsigned int SkillIdx;
    MsgLine_t msg;
    Scpt_t *scr;

    MsgId = -1;
    v7 = 0;
    SkillIdx = -1;
    if( obj2->Pid == PID_DOCTORSBAG ){
        MsgId = 900; // "The supplies in the Doctor's Bag run out."
        v7 = 20;
        SkillIdx = SKILL_DOCTOR;
    } else {
	switch( obj2->Pid ) {
    	    case PID_PARAMEDICSBAG: MsgId = 910; v7 = 40; SkillIdx = SKILL_DOCTOR;   break; // "The supplies in the Paramedic's Bag run out."
    	    case PID_FIRSTAIDKIT:   MsgId = 901; v7 = 20; SkillIdx = SKILL_FIRSTAID; break; // "The supplies in the First Aid Kit run out."
    	    case PID_FIELDMEDICFAK: MsgId = 911; v7 = 40; SkillIdx = SKILL_FIRSTAID; break; // "The supplies in the Field Medic First Aid Kit run out."
	}    
    }
    if( SkillIdx == -1 ){
        if( obj2->ScrId == -1 ){
            if( obj1->ScrId == -1 ) return UseUnk16( crit, obj1, obj2 );
            ScptSetup( obj1->ScrId, crit, obj2 );
            ScptRun( obj1->ScrId, SCPT_AEV_USE_OBJ_ON_P_PROC );
            if( ScptPtr( obj1->ScrId, &scr ) == -1 ) return -1;
            if( !scr->OverrideFlag ) return UseUnk16( crit, obj1, obj2 );
        } else {
            ScptSetup( obj2->ScrId, crit, obj1 );
            ScptRun( obj2->ScrId, SCPT_AEV_USE_OBJ_ON_P_PROC );
            if( ScptPtr( obj2->ScrId, &scr ) == -1 ) return -1;
            if( !scr->i11 ){
                if( obj1->ScrId == -1 ) return UseUnk16( crit, obj1, obj2 );
                ScptSetup( obj1->ScrId, crit, obj2 );
                ScptRun( obj1->ScrId, SCPT_AEV_USE_OBJ_ON_P_PROC );
                if( ScptPtr( obj1->ScrId, &scr ) == -1 ) return -1;
                if( !scr->OverrideFlag ) return UseUnk16( crit, obj1, obj2 );
            }
        }
        return scr->i11;
    }
    if( (gCombatStatus & 1) != 0 ){
        msg.Id = 902; // "You cannot do that in combat."
        if( crit == gObjDude && MessageGetMsg( &gProtoMessages, &msg) == 1 ) IfcMsgOut( msg.Text );
        return -1;
    }
    if( SkillAttempt( crit, obj1, SkillIdx, v7 ) || RandMinMax( 1, 10 ) == 1 ){
        msg.Id = MsgId;
        if( crit == gObjDude && MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
        return 1;
    }
    return 0;
}

int UseObjOnObj( Obj_t *obj, Obj_t *a2, Obj_t *a3 )
{
    Obj_t *v7;
    int v5, n, t, v8;
DD
    if( (n = UseUseHealSkill( obj, a2, a3 )) == 1 ){
        if( obj ){
            v5 = a2->Flags & 0x3000000;
            ItemUseItem( obj, a2, 1 );
            v7 = Item35( obj, a2, v5 );
            if( obj == gObjDude ) IfaceUnk16( &t, &v8 );
            if( !v7 ){
                if( (v5 & 0x1000000) != 0 ){
                    t = -1;
                } else {
                    if( (v5 & 0x2000000) == 0 ) t = -1;
                    v8 = -1;
                }
            }
            IfaceHandSlotUpdate( 0, t, v8 );
        }
        UseUnk06( a2 );
        n = 0;
    }
    ScptMapUpdate();
    return n;
}

void UseTileUpdate()
{
    ObjLightGrid();
    TileUpdate();
}

int UseApUpdate( Obj_t *obj, Obj_t *obj1, Obj_t *obj2 )
{
    MsgLine_t msg;

    if( !IN_COMBAT ) return 0;
    if( obj->Critter.State.CurrentAP >= 3 ){
        obj->Critter.State.CurrentAP -= 3;
        if( obj == gObjDude ) IfaceRenderAP( gObjDude->Critter.State.CurrentAP, gCombatMovePts );
        return 0;
    }
    msg.Id = 700; // 'You don't have enough action points.'
    if( obj == gObjDude && MessageGetMsg( &gProtoMessages, &msg ) == 1 ) IfcMsgOut( msg.Text );
    return -1;
}

int UseObject( Obj_t *crit, Obj_t *obj, Obj_t *objn )
{
    int override, type;
    char stmp[ 260 ];
    MsgLine_t msg;
    Scpt_t *scr;
    Proto_t *proto;

    type = OBJTYPE( obj->ImgId );
    if( crit == gObjDude ){
        if( type != TYPE_SCEN ) return -1;
    }
    if( type != TYPE_SCEN ) return 0;    

    if( ProtoGetObj( obj->Pid, &proto ) == -1 ) return -1;
    if( OBJTYPE( obj->Pid ) == TYPE_SCEN ){
	if( proto->Critt.Type == PR_SCN_DOOR ) return UseDoor( crit, obj, 0 );
    }

    override = 0;
    if( obj->ScrId != -1 ){
        ScptSetup( obj->ScrId, crit, obj );
        ScptRun( obj->ScrId, SCPT_AEV_USE_P_PROC );
        if( ScptPtr( obj->ScrId, &scr ) == -1 ) return -1;
        override = scr->OverrideFlag;
    }
    if( !override && OBJTYPE( obj->Pid ) == TYPE_SCEN ){ // no script
        switch( proto->Critt.Type ){
    	    case PR_SCN_LADDER_TOP: if( !UseLadderDn( crit, obj ) ) override = 1; break;
    	    case PR_SCN_LADDER_BOT: if( !UseLadderUp( crit, obj ) ) override = 1; break;
    	    case PR_SCN_STAIRS: DD if( !UseStairs( crit, obj ) ) override = 1; break;
        }
    }
    // print info
    if( !override && crit == gObjDude ){
        msg.Id = 480; // 'You see: %s.'
        if( MessageGetMsg( &gProtoMessages, &msg ) != 1 ) return -1;
        sprintf( stmp, msg.Text, ObjGetName( obj ) );
        IfcMsgOut( stmp );
    }
    ScptMapUpdate();
    return 0;
}

int UseLadderDn( Obj_t *crit, Obj_t *obj )
{
    int id, pos, lvl;
    VidRect_t rect;
    MapPosition_t Map;

    id = obj->Grid.DestStartPos;
    if( id == -1 ) return -1;
    pos = id & 0x3FFFFFF;
    lvl = (id & 0xE0000000) >> 29;
    if( obj->Grid.DestMapId ){
        memset( &Map, 0, sizeof( Map ) );
        Map.MapId = obj->Grid.DestMapId;
        Map.GridPos = pos;
        Map.Lvl = lvl;
        Map.Orientation = (id & 0x1C000000) >> 26;
        MapSetPos( &Map );
        WmUnk45( Map.MapId, Map.Lvl, 1 );
    } else {
        if( ObjMoveToTile( crit, pos, lvl, &rect ) == -1 ) return -1;
        TileUpdateArea( &rect, gMapCurrentLvl );
    }
    return 0;
}

int UseLadderUp( Obj_t *crit, Obj_t *obj )
{
    int id, pos, lvl;
    VidRect_t Rect;
    MapPosition_t Map;

    id = obj->Grid.DestStartPos;
    if( id == -1 ) return -1;
    pos = id & 0x3FFFFFF;
    lvl = (id & 0xE0000000) >> 29;
    if( obj->Grid.DestMapId ){
        memset( &Map, 0, sizeof( Map ) );
        Map.MapId = obj->Grid.DestMapId;
        Map.GridPos = pos;
        Map.Lvl = lvl;
        Map.Orientation = (id & 0x1C000000) >> 26;
        MapSetPos( &Map );
        WmUnk45( Map.MapId, Map.Lvl, 1 );
    } else {
        if( ObjMoveToTile( crit, pos, lvl, &Rect ) == -1 ) return -1;
        TileUpdateArea( &Rect, gMapCurrentLvl );
    }
    return 0;
}

int UseStairs( Obj_t *crit, Obj_t *obj )
{
    int id, pos, lvl;
    VidRect_t Rect;
    MapPosition_t ptr;

    id = obj->Grid.DestStartPos;
    if( id == -1 ) return -1;
    pos = id & 0x3FFFFFF;
    lvl = (id & 0xE0000000) >> 29;
    if( obj->Critter.State.Reaction > 0 ){
        memset( &ptr, 0, sizeof( ptr ) );
        ptr.MapId = obj->Critter.State.Reaction;
        ptr.GridPos = pos;
        ptr.Lvl = lvl;
        ptr.Orientation = (id & 0x1C000000) >> 26;
        MapSetPos( &ptr );
        WmUnk45( ptr.MapId, ptr.Lvl, 1 );
    } else {
        if( ObjMoveToTile( crit, pos, lvl, &Rect ) == -1 ) return -1;
        TileUpdateArea( &Rect, gMapCurrentLvl );
    }
    return 0;
}

int UseDoorOpen( Obj_t *door )
{
    door->Scenery.i05 |= 0x01;
    return 0;
}

int UseDoorClose( Obj_t *door )
{
    door->Scenery.i05 &= ~0x01;
    return 0;
}

int UseDoorLock( Obj_t *door )
{
    ArtFrmHdr_t *Art;
    CachePool_t *ImgObj;
    VidRect_t Area1, Area2;
    int i, dy, dx, fr;

    if( !(door->Scenery.i05 & 0x01) ){ // close
        door->Flags &= ~0xA0000010;
        ObjLightGrid();
        TileUpdate();
        if( !door->FrameNo ) return 0;
        if( !(Art = ArtLoadImg( door->ImgId, &ImgObj ) ) ) return -1;
        ObjGetRefreshArea( door, &Area1 );        
    	for(i = door->FrameNo - 1 ; i >= 1; i-- ){
            ArtGetFrameShift( Art, i, door->Orientation, &dx, &dy );
            ObjMove( door, -dx, -dy, &Area2 );
        }        
        ObjSetFrame( door, 0, &Area2 );
        RegionExpand( &Area1, &Area2, &Area1 );
        TileUpdateArea( &Area1, gMapCurrentLvl );
        ArtClose( ImgObj );
        return 0;
    }
    // open
    door->Flags |= 0xA0000010;
    ObjLightGrid();
    TileUpdate();
    if( !( Art = ArtLoadImg( door->ImgId, &ImgObj ) ) ) return -1;
    fr = ArtGetFpd( Art ) - 1;
    if( fr == door->FrameNo ){ ArtClose( ImgObj ); return 0; }
    ObjGetRefreshArea( door, &Area1 );
    for( i = door->FrameNo + 1; i <= fr; i++ ){
        ArtGetFrameShift( Art, i, door->Orientation, &dx, &dy );
        ObjMove( door, dx, dy, &Area1 );
    }
    ObjSetFrame( door, fr, &Area2 );
    RegionExpand( &Area1, &Area2, &Area1 );
    TileUpdateArea( &Area1, gMapCurrentLvl );
    ArtClose( ImgObj );
    return 0;
}

int UseDoor( Obj_t *Crit, Obj_t *obj, int a3 )
{
    int v6;
    int v10;
    int i;
    MsgLine_t msg;
    Scpt_t *a2;
    int v18;

    v6 = 0;
    if( UseObjLocked( obj ) ) GSoundPlay( GSoundOpenFileName( obj, 2 ) );
    if( obj->ScrId != -1 ){
        ScptSetup( obj->ScrId, Crit, obj );
        ScptRun( obj->ScrId, SCPT_AEV_USE_P_PROC );
        if( ScptPtr( obj->ScrId, &a2 ) == -1 ) return -1;
        v6 = a2->OverrideFlag;
    }
    if( v6 ) return 0;

    if( obj->FrameNo ){
        if( ObjReach( 0, obj->GridId, obj->Elevation ) ){
            IfcMsgOut( MessageGetMessage( &gProtoMessages, &msg, 597 ) ); // 'The doorway seems to be blocked.'
            return -1;
        }
	// close door
        v10 = (a3 == 0) - 1;
        v18 = -1;
        i = 1;
    } else {
	// open door
        if( obj->Scenery.i05 & 0x01 ) return -1;
        v10 = (a3 != 0) + 1;
        v18 = 1;
        i = 0;
    }
    AnimRegStart( 2 );
    for(; i != v10; i += v18 ){
        if( i ){ // close door
            if( !a3 ) AnimSetCallback11( obj, (AnimU_t)obj, (void *)UseDoorClose, -1 );
            AnimRegPlaySfx( obj, GSoundOpenFileName( obj, 1 ), -1 );
            AnimRegAnimReverse( obj, 0, 0 );
        } else { // open door
            if( !a3 ) AnimSetCallback11(obj, (AnimU_t)obj, (void *)UseDoorOpen, -1);
            AnimRegPlaySfx( obj, GSoundOpenFileName( obj, 0 ), -1 );
            AnimRegAnimation( obj, 0, 0);
        }
    }
    AnimSetFinish( obj, obj, (void *)UseDoorLock, -1 );
    AnimRegEnd();
    return 0;
}

int UseContainer( Obj_t *dude, Obj_t *obj )
{
    int i18, ScrId;
    char stmp[260];
    MsgLine_t msg;
    Proto_t *proto;
    Scpt_t *res;

    if( OBJTYPE( obj->ImgId ) != TYPE_ITEM ) return -1;
    if( ProtoGetObj( obj->Pid, &proto ) == -1 ) return -1;
    if( proto->Critt.Type != 1 ) return -1;
    if( UseObjLocked( obj ) ){
	GSoundPlay( GSoundOpenFileName( obj, 2 ) );
	if( dude != gObjDude ) return -1;
    	msg.Id = 487; // 'It is locked.'
    	if( MessageGetMsg( &gProtoMessages, &msg ) != 1 ) return -1;
    	IfcMsgOut( msg.Text );
	return -1;
    }

    i18 = 0;
    ScrId = obj->ScrId;
    if( (ScrId != -1) - 1 != -1 ){
        ScptSetup( obj->ScrId, dude, obj );
        ScptRun( ScrId, SCPT_AEV_USE_P_PROC );
        if( ScptPtr( ScrId, &res ) == -1 ) return -1;
        i18 = res->OverrideFlag;
    }
    if( i18 ) return -1;
    AnimRegStart( 2 );
    if( obj->FrameNo ){
        AnimRegPlaySfx( obj, GSoundOpenFileName( obj, 1 ), 0 );
        AnimRegAnimReverse( obj, 0, 0 );
    } else {
        AnimRegPlaySfx( obj, GSoundOpenFileName( obj, 0 ), 0 );
        AnimRegAnimation( obj, 0, 0 );
    }
    AnimRegEnd();
    if( dude == gObjDude ){
        msg.Id = (obj->FrameNo != 0) + 485; // 'You search the %s.' / 'You close the %s.'
        if( MessageGetMsg( &gProtoMessages, &msg ) != 1 ) return -1;
        sprintf( stmp, msg.Text, ObjGetName( obj ) );
        IfcMsgOut( stmp );
    }
    return 0;
}

int UseUseSkill( Obj_t *crit, Obj_t *obj, unsigned int SkillIdx )
{
    int i18;
    MsgLine_t msg;
    Scpt_t *scr;
    Proto_t *proto;

    i18 = 0;
    if( UseObjIsJammed( obj ) ){
        if( crit == gObjDude ){
            msg.Id = 2001; // 'The lock is jammed.'
            if( MessageGetMsg( &gMessage, &msg ) == 1 ) IfcMsgOut( msg.Text );
        }
        return -1;
    }
    if( ProtoGetObj( obj->Pid, &proto ) == -1 ) return -1;
    if( obj->ScrId != -1 ){
        ScptSetup( obj->ScrId, crit, obj );
        ScptUnk136( obj->ScrId, SkillIdx );
        ScptRun( obj->ScrId, SCPT_AEV_USE_SKILL_ON_P_PROC );
        if( ScptPtr( obj->ScrId, &scr ) == -1 ) return -1;
        i18 = scr->OverrideFlag;
    }
    if( !i18 ) SkillAttempt( crit, obj, SkillIdx, 0 );
    return 0;    
}

int UseObjIsDoor( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 0;
    return ProtoGetObj( obj->Pid, &proto ) != -1 && ( proto->Critt.Type == PR_SCN_DOOR );
}

int UseObjIsOpenable( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 0;    
    if( ProtoGetObj( obj->Pid, &proto ) == -1 ) return 0;
    switch( OBJTYPE( obj->Pid ) ){
	case TYPE_SCEN: return ( proto->Critt.Type == PR_SCN_DOOR ) ? 1:0;
	case TYPE_ITEM: return ( proto->Critt.Type == PR_ITEM_CONTAINER ) ? 1:0;
    }
    return 0;
}

int UseObjLocked( Obj_t *obj )
{
    int k;

    if( !obj ) return 0;    
    switch( OBJTYPE( obj->Pid ) ){
        case TYPE_ITEM: k = obj->Container.i04; break;
        case TYPE_SCEN: k = obj->Scenery.i05; break;
	default: return 0;
    }
    return k & 0x2000000;
}

int UseObjLock( Obj_t *obj )
{
    if( !obj ) return -1;
    switch( OBJTYPE( obj->Pid ) ){
	case TYPE_ITEM: obj->Container.i04 |= 0x2000000; return 0;
	case TYPE_SCEN: obj->Scenery.i05 |= 0x2000000; return 0;
    }
    return -1;
}

int UseObjUnlock( Obj_t *obj )
{
    if( !obj ) return -1;
    switch( OBJTYPE( obj->Pid ) ){
	case TYPE_ITEM: obj->Container.i04 &= ~0x2000000; return 0;
	case TYPE_SCEN: obj->Scenery.i05 &= ~0x2000000; return 0;
    }
    return -1;
}

int UseObjIsOpenable1( Obj_t *obj ) // powtorzenie funkcji !
{
    Proto_t *proto;

    if( !obj ) return 0;    
    if( ProtoGetObj( obj->Pid, &proto ) == -1 ) return 0;
        
    switch( OBJTYPE( obj->Pid ) ){
	case TYPE_ITEM: return ( proto->Critt.Type == PR_SCN_DOOR ) ? 1 : 0;
	case TYPE_SCEN: return ( proto->Critt.Type == PR_ITEM_CONTAINER ) ? 1 : 0;
    }
    return 0;
}

int UseObjInAnimation( Obj_t *obj )
{
    return obj->FrameNo != 0;
}

int UseStartAnimation( Obj_t *obj )
{
    if( !obj ) return -1;
    if( !UseObjIsOpenable1( obj ) ) return -1;
    if( UseObjLocked( obj ) ) return -1;
    UseObjUnjam( obj );
    AnimRegStart( 2 );
    if( obj->FrameNo ){
        AnimSetCallback11( obj, (AnimU_t)obj, (void *)UseDoorClose, -1 );
        AnimRegPlaySfx( obj, GSoundOpenFileName( obj, 1 ), -1 );
        AnimRegAnimReverse( obj, 0, 0);
    } else {
        AnimSetCallback11( obj, (AnimU_t)obj, (void *)UseDoorOpen, -1 );
        AnimRegPlaySfx( obj, GSoundOpenFileName( obj, 0 ), -1 );
        AnimRegAnimation( obj, 0, 0 );
    }
    AnimSetFinish( obj, obj, (void *)UseDoorLock, -1 );
    AnimRegEnd();
    return 0;
}

int UseObjOpen( Obj_t *obj )
{
    if( obj->FrameNo == 0 ) UseStartAnimation( obj );
    return 0;
}

int UseObjClose( Obj_t *obj )
{
    if( obj->FrameNo != 0 ) UseStartAnimation( obj );
    return 0;
}

int UseObjIsJammed( Obj_t *obj )
{
    if( !UseObjIsOpenable( obj ) ) return 0;    
    switch( OBJTYPE( obj->Pid ) ){
	case 0: return ( obj->Container.i04 & 0x4000000 ) != 0 ? 1:0;
	case 2: return ( obj->Scenery.i05 & 0x4000000 ) != 0 ? 1:0;
    }
    return 0;
}

int UseObjJam( Obj_t *obj )
{
    if( !UseObjIsOpenable( obj ) ) return -1;
    switch( OBJTYPE( obj->Pid ) ){
	case TYPE_ITEM: obj->Scenery.i05 |= 0x4000000; break;
	case TYPE_SCEN: obj->Container.i04 |= 0x4000000; break;
    }
    return 0;
}

int UseObjUnjam( Obj_t *obj )
{
    if( !UseObjIsOpenable( obj ) ) return -1;
    switch( OBJTYPE( obj->Pid ) ){
	case TYPE_ITEM: obj->Scenery.i05 &= ~0x4000000; break;
	case TYPE_SCEN: obj->Container.i04 &= ~0x4000000; break;
    }
    return 0;
}

int UseUnjamAll()
{
    Obj_t *p;

    for( p = ObjGetObjectFirst(); p; p = ObjGetObjectNext() ){
        UseObjUnjam( p );
    }
    return 0;
}

int UseUnk46( Obj_t *obj, int GridIdx, int lvl, int a4 )
{
    int v6, i, j, v10, v18;
    VidRect_t Area2, Area1;    

    if( GridIdx == -1 ) return -1;
    if( ObjReach( NULL, GridIdx, lvl ) ){
        v6 = a4;
	v18 = 0;
        if( a4 < 1 ) v6 = 1;
        for( ;v6 < 7; v6++ ){
            if( ++v18 > 100 ) break;
            for( i = 0; i < 6; i++ ){
                GridIdx = TileGetTileNumInDir( GridIdx, i, v6 );
                if(!ObjReach( 0, GridIdx, lvl ) && v6 > 1 && AnimFindTrace( gObjDude, gObjDude->GridId, GridIdx, 0, 0 ) ) break;
            }
            if( i < 6 ) break;            
        }        
        if( a4 != 1 && v6 > a4 + 2 ){
            for ( j = 0; j < 6; j++ ){
                v10 = TileGetTileNumInDir( GridIdx, j, 1 );
                if( !ObjReach( NULL, v10, lvl ) ) break;
            }
            if( j < 6 ) GridIdx = v10;
        }
    }
    ObjUnk32( obj, &Area2 );
    if( ObjMoveToTile( obj, GridIdx, lvl, &Area1 ) != -1 ){
        RegionExpand( &Area1, &Area2, &Area1 );
        if( lvl == gMapCurrentLvl ) TileUpdateArea( &Area1, lvl );
    }
    return 0;
}

int UseUnk47( Obj_t *obj, int MapPos, int MapLvl )
{
    int i, err, GridPos;

    if( !obj || (MapPos == -1) ) return -1;
    err = WmUnk53( MapPos );
    GridPos = MapPos;
    if( !err ){
        GridPos = gObjDude->GridId;
        for( i = 1; i <= 100; i++ ){
            GridPos = TileGetTileNumInDir( GridPos, i % 6, 1 );
            if( WmUnk53( GridPos ) ) break;
            if( TileGetDistance( gObjDude->GridId, GridPos ) > 8 ){ GridPos = MapPos; break; }
        }
    }
    ObjUnk32( obj, 0 );
    ObjMoveToTile( obj, GridPos, MapLvl, NULL );
    return 0;
}

int UseAddItem( int Pid, int Quantity ) // no X
{
    Obj_t *Item;
    Proto_t *proto;

    if( !gUnk123 ) return -1;
    if( ProtoGetObj( Pid, &proto ) == -1 ) return 0;    
    ObjCreate( &Item, proto->ImgId, Pid );
    ObjMoveToTile( Item, 0, 0, NULL );
    if( ItemAdd( gUnk123, Item, Quantity ) )
        MsgCreate( "Error adding obj to critter!", gPalColorCubeRGB[31][31][11] );
    else
        ObjLightItem( Item, 0 );    
    return 0;
}

int UseEditRenderObj( int Win, int Type, int a3, int Pid, int (*Cb)(Proto_t *) )
{
    VidRect_t Area;
    Proto_t *proto;
    char *surf;
    int x, y, Id, idx, pos;

    idx = 0;
    for( y = 10; y < 370; y += 90 ){
	Id = Pid + idx;
        for( x = 10, pos = 4400; x < 300; x += 80, pos += 35200 ){
            Area.lt = y;
            Area.tp = x;
            Area.rt = y + 80;
            Area.bm = x + 60;
            surf = WinGetSurface( Win );
            ScrFillSolid( surf + pos + y, 80, 60, 440, gPalColorCubeRGB[0][0][31] );
            Area.lt = 5 + y;
            Area.tp = 65 + x;
            Area.rt = 85 + y;
            Area.bm = gFont.ChrHeight() + Area.tp;
            if( ProtoGetObj( Id | (Type << 24), &proto ) == -1 ){
                WinAreaUpdate( Win, &Area );
                WinDrawFilledRect( Win, Area.lt, Area.tp, 80, gFont.ChrHeight(), gPalColorCubeRGB[15][15][15] );
            } else {
                ArtLoadImgScaled( ( Cb ) ? Cb( proto ) : proto->ImgId, surf + pos + y, 80, 60, 440 );
                WinAreaUpdate( Win, &Area );
                WinDrawFilledRect( Win, Area.lt, Area.tp, 80, gFont.ChrHeight(), gPalColorCubeRGB[15][15][15] );
                WinDrawText( Win, ProtoGetObjName( proto->Pid ), 80, Area.lt, Area.tp, gPalColorCubeRGB[31][31][11] | 0x10000 );
                Id++; idx++;
            }            
        }        
    }    
    WinUpdate( Win );
    return 0;
}

int UseEditObjSelector( int Pid, int a2, void (*a3)(int, int), int (*a4)(Proto_t *) )
{
    unsigned int v14;
    char a1[ 40 ], *pDst;
    int i,v8,Button,idx,sel,v18,na,v22,xx,win,v30,a6;

    if( (win = WinCreateWindow( 60, 40, 440, 380, gPalColorCubeRGB[10][10][10], 20 )) == -1 ) return -1;    
    WinDrawFrame( win );
    WinAddButton( win, 345, 350, -1, -1, 91, -1, "<<", 0 );
    WinAddButton( win, 375, 350, -1, -1, 93, -1, ">>", 0 );
    WinAddButton( win, 10, 340, -1, -1, -1, 124, "Done", 0 );
    pDst = WinGetSurface( win );
    v30 = 0;    
    for( xx = 0,v22 = 90; xx != 360; v22 += 90, xx += 90 ){
        na = v22;            
        a6 = v30 + 160;
        for( i = 0, v8 = 0;i != 320; i += 80, v8 += 80, v30++, a6++ ){
            ScrRectangle( pDst, 440, 9 + xx, 9 + i, na, 70 + v8, gPalColorCubeRGB[0][0][0] );
            if( (Button = WinCreateButton( win, 9 + xx, 9 + i, 81, 61, -1, -1, a6, -1, 0, 0, 0, 0 )) != -1 ) WinSetClickSound( Button, GSoundPlayPushBt, GSoundPlayReleaseBt );                
        }            
    }
    WinUpdate( win );
    UseEditRenderObj( win, Pid, a2, 1, a4 );
    idx = ProtoActive( Pid ) - 16;
    while( 1 ){
        sel = InpUpdate();
        switch( sel ){
    	    case 27: case '|': case 13: WinClose( win ); return ( sel == 27 ) ? -1 : 0;
    	    case 160 ... 199:
        	    v14 = Pid + sel - 160; na = 1;
        	    na = 1;
        	    if( gKeyStates[ 29 ] ){
            		sprintf( a1, "How many: %s?", ProtoGetObjName( v14 | (Pid << 24) ) ); 
//			TextBoxInput( &na, 1, 32000, 0, a1, 100, 100 );
        	    } else if( gKeyStates[ 56 ] ){
            		na = 10;
        	    }
        	    a3( v14 | (Pid << 24), na );
    		    break;
    	    case ']':
        	    v18 = Pid; 
        	    Pid += 16; 
        	    if( idx <= Pid ) Pid = idx;
        	    if( v18 != Pid ) UseEditRenderObj( win, Pid, xx, Pid, a4 );
    		    break;
    	    case 327:
        	    if( Pid != 1 ){ Pid = 1; UseEditRenderObj( win, Pid, xx, 1, a4 ); }
    		    break;
    	    case 335:
    		    if( idx > Pid ){ Pid = idx; UseEditRenderObj( win, Pid, xx, idx, a4 ); }                
    		    break;
    	    case '[':
        	    v18 = Pid;
        	    Pid -= 16;
        	    if( Pid <= 0 ) Pid = 1;
        	    if( v18 != Pid ) UseEditRenderObj( win, Pid, xx, Pid, a4 );
    		    break;
    	}
    }    
}

int UseEditSelectObj( int Pid, int NotUsed, void (*a3)(int, int ) )
{
    return UseEditObjSelector( Pid, NotUsed, a3, NULL );
}

int UseEditObjSel( int *pPid, int PidType, int a3, int Flags )
{
    Proto_t *proto;
    int PosY,WinId,k,sel,Height;
    char *Surface, *surf1;

    Height = 240;
    if( a3 || ProtoGetObj( *pPid, &proto ) == -1 ) *pPid = (PidType << 24) | 0x01;
    if( ProtoGetObj( *pPid, &proto ) == -1 ) return -1;
    if( (WinId = WinCreateWindow( 360, 140, 160, Height, gPalColorCubeRGB[10][10][10], 20 )) == -1 ) return -1;
    WinDrawFrame( WinId );
    WinAddButton( WinId, 45, 150, -1, -1, 91, -1, "<<", 0 );
    WinAddButton( WinId, 75, 150, -1, -1, 93, -1, ">>", 0 );
    Surface = WinGetSurface( WinId );
    surf1 = Surface + 4030;
    ScrRectangle( Surface + 3869, 160, 0, 0, 101, 101, gPalColorCubeRGB[0][0][0] );
    UseEditObjView( WinId, &proto, surf1, 160, 0 );
    do
        k = UseEditObjView( WinId, &proto, surf1, 160, 1 );
    while( !ProtoTestFlg( proto, Flags ) && !k );
    WinAddButton( WinId, 110, Height - 65, -1, -1, -1, 110, "None", 0 );
    PosY = Height - 40;
    WinAddButton( WinId, 10, Height - 40, -1, -1, -1, 124, "Done", 0 );
    WinAddButton( WinId, 100, PosY, -1, -1, -1, 27, "Cancel", 0 );

    WinUpdate( WinId );
    do{
        sel = InpUpdate();
        if( sel == KEY_ESC || sel == '|' ) break;
        if( sel == KEY_ENTER ){ sel = '|'; break; }
        switch( sel ){
    	    case '[':
        	do
            	    k = UseEditObjView(WinId, &proto, surf1, 160, -1);
        	while ( !ProtoTestFlg(proto, Flags) && !k );
        	if( !ProtoTestFlg( proto, Flags ) ){
            	    do
                	k = UseEditObjView( WinId, &proto, surf1, 160, 1 );
            	    while( !ProtoTestFlg( proto, Flags ) && !k );
        	}
    		break; 
    	    case ']':
        	do
            	    k = UseEditObjView( WinId, &proto, surf1, 160, 1 );
        	while ( !ProtoTestFlg( proto, Flags ) && !k );
        	if( !ProtoTestFlg( proto, Flags ) ){
            	    do
                	k = UseEditObjView(WinId, &proto, surf1, 160, -1);
            	    while ( !ProtoTestFlg( proto, Flags ) && !k );
        	}
    		break;
    	    case 'n': proto->Pid = -1; break;
        }
    }while( sel != 'n' );
    WinClose( WinId );
    if( sel == KEY_ESC ) return -1;
    *pPid = proto->Pid;
    return 0;
}

int UseEditObjView( int WinId, Proto_t **a2, char *surf, int pitch, int pid )
{
    char *s, fname[ 16 ];
    VidRect_t Area;
    Proto_t *proto;
    unsigned int Idx;

    proto = *a2;
    Idx = pid + (proto->Pid & 0xFFFFFF);
    if( Idx == 0 ) return -1;
    if( Idx >= ProtoActive( OBJTYPE( proto->Pid ) ) ) return -1;
    Idx = Idx | (OBJTYPE( proto->Pid ) != 0 );
    if( ProtoGetObj(Idx, &proto) == -1 ) return 0;    
    *a2 = proto;
    ScrFillSolid( surf, 100, 100, pitch, gPalColorCubeRGB[0][0][31] );
    ArtLoadImgScaled( proto->ImgId, surf, 100, 100, pitch );
    Area.lt = pitch - 130;
    Area.bm = 125;
    Area.rt = pitch - 30;
    Area.tp = 25;
    WinAreaUpdate( WinId, &Area );
    if( ProtoGetFName( Idx, fname ) != -1 ){            
        if( (s = strchr( fname, '.' ) ) ) *s = '\0';
        WinDrawText( WinId, fname, 80, pitch - 110, 130, gPalColorCubeRGB[31][31][11] | 0x10000 );
        Area.lt = pitch - 130;
        Area.tp = 130;
        Area.rt = pitch - 50;
        Area.bm = gFont.ChrHeight() + 130;
        WinAreaUpdate( WinId, &Area );
    }
    Area.lt = pitch - 150;
    Area.tp = 140;
    Area.rt = pitch - 35;
    Area.bm = 140 + gFont.ChrHeight();
    WinDrawFilledRect( WinId, Area.lt, Area.tp, 80, gFont.ChrHeight(), gPalColorCubeRGB[15][15][15] );
    WinDrawText( WinId, ProtoGetObjName(proto->Pid), 110, Area.lt, Area.tp, gPalColorCubeRGB[31][31][11] | 0x10000 );
    WinAreaUpdate( WinId, &Area );
    return 0;
}


