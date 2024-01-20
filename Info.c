#include "FrameWork.h"

void InfoLookMsg( Obj_t *crit, Obj_t *obj )
{
    InfoLook( crit, obj, IfcMsgOut );
}

void InfoLook( Obj_t *crit, Obj_t *obj, void (*OutCb)(char *))
{
    int i18; // ebx
    int ScrId; // esi
    char *Name; // eax
    char stmp[260]; // [esp+0h] [ebp-12Ch] BYREF
    MsgLine_t msg; // [esp+104h] [ebp-28h] BYREF
    Scpt_t *scr; // [esp+114h] [ebp-18h] BYREF
    Proto_t *proto; // [esp+118h] [ebp-14h] BYREF

    i18 = 0;
    if( !CritterIsDead( crit ) && OBJTYPE( obj->ImgId ) != TYPE_TILE && ProtoGetObj( obj->Pid, &proto ) != -1 ){
        ScrId = obj->ScrId;
        if( ScrId != -1 ){
            ScptUseObject( obj->ScrId, crit, obj );
//            ScptExecScriptProc( ScrId );
//            if ( ScptPtr(ScrId, &scr) == -1 ) return;
//            i18 = scr->i18;
        }
        if( !i18 ){
            if( OBJTYPE( obj->Pid ) == TYPE_CRIT && CritterIsDead( obj ) )
                msg.Id = 491 + RandMinMax(0, 1);
            else
                msg.Id = 490;                   // look text
            if( MessageGetMsg( &gProtoMessages, &msg ) == 1 ){
                sprintf( stmp, msg.Text, ObjGetName( obj ) );
printf( "Info look: '%s'\n", stmp );
                OutCb( stmp );
            }
        }
    }
}

int InfoExamineMsg( Obj_t *a1, Obj_t *a2)
{
    return InfoExamine( a1, a2, IfcMsgOut );
}

int InfoExamine( Obj_t *critter, Obj_t *obj, void (*OutCb)(char *) )
{
    int i18; // edx
//    int ScrId; // esi
    char *s; // esi
    char *Dsc; // eax MAPDST
    unsigned int type; // eax
    int idx; // esi
    int v15; // edx
    int v17; // ecx MAPDST
    int Val; // edi
    Obj_t *RHandObj; // eax
    Obj_t *obj2; // esi
    unsigned int ObjType; // eax
    int v42; // eax
    char stmp1[260]; // [esp+0h] [ebp-1ACh] BYREF
    char stmp2[80]; // [esp+104h] [ebp-A8h] BYREF
    MsgLine_t msg3; // [esp+154h] [ebp-58h] BYREF
    MsgLine_t msg1; // [esp+164h] [ebp-48h] BYREF
    MsgLine_t msg4; // [esp+174h] [ebp-38h] BYREF
    MsgLine_t msg2; // [esp+184h] [ebp-28h] BYREF
    Scpt_t *script; // [esp+194h] [ebp-18h] BYREF

DD
return 0;
    i18 = 0;
    if( CritterIsDead( critter ) ) return -1;
    if( OBJTYPE( obj->ImgId ) == TYPE_TILE ) return -1;
//    ScrId = obj->ScrId;
//    if( ScrId != -1 ){
//        ScptUseObject( obj->ScrId, critter, obj );
//        ScptExecScriptProc( ScrId );
//        if( ScptPtr( ScrId, &script ) == -1 ) return -1;
//        i18 = script->i18;
//    }
    if( !i18 ){
        Dsc = ObjGetDsc( obj );
	s = Dsc;
        if( Dsc && !strcmp( Dsc, gProtoUnk100[0] ) ) s = NULL;
        if( !s || !*s ){
            msg1.Id = 493; // 'You see nothing out of the ordinary'
            if( MessageGetMsg( &gProtoMessages, &msg1 ) != 1 ) eprintf( "\nError: Can't find msg num!" );
            Dsc = msg1.Text;
            OutCb( Dsc );
        } else {
    	    if( OBJTYPE( obj->Pid ) != TYPE_CRIT || !CritterIsDead( obj ) ){
		Dsc = s;
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
                eprintf( msg1.Text );
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
//            sprintf( stmp1, msg1.Text, 100 * WmUnk88() / 80000 ); // gas gauge
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

