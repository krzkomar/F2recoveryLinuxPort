#include "FrameWork.h"

int gAiPacket = 0;
Ai_t *gAiPackets = NULL;
Obj_t *gAiUnk03 = NULL;
int gAiPacketsCnt = 0;
int gAiUnk04 = 0;
char gAiUnk06[260];
int gAiObjCount;
Obj_t **gAiObjList;
char gAiUnk05[268];
int gAiInjureFlags[ 4 ] = { 0x40, 0x3c, 0x0c, 0x30 };
Obj_t *gAiUnk11 = NULL;
Obj_t *gAiUnk12 = NULL;
int gAiConstPercentRank[ 6 ] = { 0, 25, 40, 60, 75, 100 };
int gAiUnk10[ 45 ] = {
    4, 3, 2, 1, 0, 
    4, 3, 2, 1, 0, 
    2, 0, 0, 0, 0, 
    2, 4, 0, 0, 0, 
    4, 2, 0, 0, 0,
    4, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 3, 0, 0, 0,
    0, 0, 0, 0, 0 
};
int gAiUnk15[5] = { 5, 7, 7, 7, 50000 };
Msg_t gCombatAiMessages;

/*******************************************************/

void AiParseInjureFlags( char *str, int *Flags )
{
    int i, SepPos;
    char sep;

    *Flags = 0;
    StrLwr( str );
    while( *str ){        
        str += strspn(str, " ");
        SepPos = strcspn( str, "," );
        sep = str[ SepPos ];
        str[ SepPos ] = '\0';
        for( i = 0; i < 4; i++ ){    	    
    	    if( !strcmp( str, gSeriousInjureListNames[ i ] ) ){
    		*Flags |= gAiInjureFlags[ i ];
    		break;
    	    }
        }
        if( i == 4 ) eprintf( "Unrecognized flag: %s\n", str );
        str = &str[ SepPos ];
        *str = sep;
        if( sep ) str++;
    }
}

int AiListMatch( char *val, const char **list, int cnt, int *pVal )
{
    int i;

    for( i = 0; i < cnt; list++, i++ ){
        if( !strcasecmp( val, *list ) ) break;
    }
    *pVal = ( i >= cnt ) ? -1 : i;
    return 0;
}

int AiPacketInit( Ai_t *ai )
{
    ai->Name = NULL;
    ai->AreaAttackMode = -1;
    ai->RunAwayMode = -1;
    ai->BestWeapon = -1;
    ai->Distance = -1;
    ai->AttackWho = -1;
    ai->ChemUse = -1;
    ai->ChemDesire[0] = -1;
    ai->ChemDesire[1] = -1;
    ai->ChemDesire[2] = -1;
    ai->Disposition = -1;
    return 0;
}

void AiLoadCfg()
{
    int i;
    char *tmp;
    int err;
    Config_t cfg;

    err = 0;
    if( AiLoadCombatMsg() == -1 ) return;
    gAiPacketsCnt = 0;
    if( !CfgInit( &cfg ) ) return;
    if( CfgLoadFromFile( &cfg, "data/ai.txt", 1 ) != 1 ){ DD CfgFree( &cfg ); return; }
    if( !(gAiPackets = Malloc( cfg.nSect * sizeof( Ai_t ) )) ){ DD CfgFree( &cfg ); return; }
    for( i = 0; i < cfg.nSect; i++ ){
    	AiPacketInit( &gAiPackets[ i ] );
    }                                
    for( i = 0; i < cfg.nSect; i++ ){
	if( !(gAiPackets[ i ].Name = strdup( cfg.Section[ i ].Name )) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "packet_num", 		&gAiPackets[ i ].PacketNum) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "max_dist", 		&gAiPackets[ i ].MaxDist ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "min_to_hit", 		&gAiPackets[ i ].MinToHit ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "min_hp", 		&gAiPackets[ i ].MinHP ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "aggression",  	&gAiPackets[ i ].Aggression ) ) break; 
	if( CfgGetString( &cfg,   cfg.Section[ i ].Name, "hurt_too_much", 	&tmp) == 1 ) AiParseInjureFlags( tmp, &gAiPackets[ i ].HurtTooMuch );
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "secondary_freq", 	&gAiPackets[ i ].SecondaryFreq ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "called_freq", 	&gAiPackets[ i ].CalledFreq ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "font", 		&gAiPackets[ i ].Font ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "color", 		&gAiPackets[ i ].Color ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "outline_color", 	&gAiPackets[ i ].OutlineColor ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "chance", 		&gAiPackets[ i ].Chance ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "run_start", 		&gAiPackets[ i ].RunStart ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "run_end", 		&gAiPackets[ i ].RunEnd ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "move_start", 		&gAiPackets[ i ].MoveStart ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "move_end", 		&gAiPackets[ i ].MoveEnd )) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "attack_start", 	&gAiPackets[ i ].AttackStart ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "attack_end", 		&gAiPackets[ i ].AttackEnd ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "miss_start", 		&gAiPackets[ i ].MissStart   ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "miss_end", 		&gAiPackets[ i ].MissEnd ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_head_start", 	&gAiPackets[ i ].HitHeadStart ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_head_end", 	&gAiPackets[ i ].HitHeadEnd ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_left_arm_start", 	&gAiPackets[ i ].HitLeftArmStart ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_left_arm_end", 	&gAiPackets[ i ].HitLeftArmEnd ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_right_arm_start",	&gAiPackets[ i ].HitRightArmStart ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_right_arm_end", 	&gAiPackets[ i ].HitRightArmEnd ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_torso_start", 	&gAiPackets[ i ].HitTorsoStart ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_torso_end", 	&gAiPackets[ i ].HitTorsoEnd )) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_right_leg_start",	&gAiPackets[ i ].HitRightLegStart ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_right_leg_end", 	&gAiPackets[ i ].HitRightLegEnd ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_left_leg_start", 	&gAiPackets[ i ].HitLeftLegStart ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_left_leg_end", 	&gAiPackets[ i ].HitLeftLegEnd ) ) break;
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_eyes_start", 	&gAiPackets[ i ].HitEyesStart ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_eyes_end", 	&gAiPackets[ i ].HitEyesEnd ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_groin_start", 	&gAiPackets[ i ].HitGroinStart ) ) break; 
	if( !CfgGetInteger( &cfg, cfg.Section[ i ].Name, "hit_groin_end", 	&gAiPackets[ i ].HitGroinEnd ) ) break;
        gAiPackets[ i ].AreaAttackMode++;
        if( CfgGetString( &cfg, cfg.Section[ i ].Name, "area_attack_mode", &tmp ) == 1 ) 
    	    AiListMatch( tmp, gAreaAttackModeListNames, 5, &gAiPackets[ i ].AreaAttackMode );
        else 
            gAiPackets[ i ].AreaAttackMode = -1;
        
        if( CfgGetString( &cfg, cfg.Section[ i ].Name, "run_away_mode", &tmp ) == 1 ){
            AiListMatch( tmp, gRunAwayListNames, 7, &gAiPackets[ i ].RunAwayMode );
            if( gAiPackets[ i ].Distance >= 0 ) gAiPackets[ i ].RunAwayMode--;
        }
        if( CfgGetString( &cfg, cfg.Section[ i ].Name, "best_weapon", &tmp ) == 1 ) AiListMatch( tmp, gBestWeaponListNames, 8, &gAiPackets[ i ].BestWeapon );
        if( CfgGetString( &cfg, cfg.Section[ i ].Name, "distance", &tmp ) == 1 ) AiListMatch( tmp, gDistanceListNames, 5, &gAiPackets[ i ].Distance );
        if( CfgGetString( &cfg, cfg.Section[ i ].Name, "attack_who", &tmp ) == 1 ) AiListMatch( tmp, gWhomAttackListNames, 5, &gAiPackets[ i ].AttackWho );
        if( CfgGetString( &cfg, cfg.Section[ i ].Name, "chem_use", &tmp ) == 1 ) AiListMatch( tmp, gChemUseListNames, 6, &gAiPackets[ i ].ChemUse );
        CfgGetIntParamList( &cfg, cfg.Section[ i ].Name, "chem_primary_desire", gAiPackets[ i ].ChemDesire, 3 );
        if( CfgGetString( &cfg, cfg.Section[ i ].Name, "disposition", &tmp ) == 1 ){
            AiListMatch( tmp, gDispositionListNames, 6, &gAiPackets[ i ].Disposition );
            gAiPackets[ i ].Disposition--;
        }
        gAiPackets[ i ].BodyType = CfgGetString( &cfg, cfg.Section[i].Name, "body_type", &tmp ) == 1 ? strdup( tmp ) : NULL;
        gAiPackets[ i ].GeneralType = CfgGetString( &cfg, cfg.Section[i].Name, "general_type", &tmp ) == 1 ? strdup( tmp ) : NULL;
    } 
    if( i >= cfg.nSect ){
	gAiPacketsCnt = cfg.nSect;
    } else {
        for( i = 0; i < cfg.nSect; i++ ){
    	    if( gAiPackets[ i ].Name ) Free( gAiPackets[ i ].Name );
        }
        Free( gAiPackets );
        eprintf( "Error processing ai.txt" );
        err = -1;
    }
    CfgFree( &cfg );
    if( !err ) gAiUnk04 = 1;
}

int AiReset()
{
    return 0;
}

void AiClose()
{
    int i;
        
    for( i = 0; i < gAiPacketsCnt; i++ ){
        if( gAiPackets[ i  ].Name ){ Free(gAiPackets[ i ].Name ); gAiPackets[ i ].Name = NULL; }
        if( gAiPackets[ i ].GeneralType ){ Free( gAiPackets[ i ].GeneralType ); gAiPackets[ i ].GeneralType = NULL; }
        if( gAiPackets[ i ].BodyType ){ Free( gAiPackets[ i ].BodyType ); gAiPackets[ i ].BodyType = NULL; }
    }
    Free( gAiPackets );
    gAiPacketsCnt = 0;
    gAiUnk04 = 0;
    MessageClose( &gCombatAiMessages );
}

int AiLoad( xFile_t *fh )
{
    int i, Pid;
    Ai_t *Packet;
    Proto_t *proto;

    for( i = 1; i < gPartyCount; i++ ){
        Pid = gPartyPids[ i ];
        if( Pid != -1 && OBJTYPE( Pid ) == 1 ){
            if( ProtoGetObj(Pid, &proto) == -1 ) return -1;
            Packet = AiGetPacketById( proto->AiPacketNum );
            if( !Packet->Disposition ) AiLoadPacket( fh, Packet );
        }
    }
    return 0;
}

int AiFSave( xFile_t *fh )
{
    int pid, i;
    Ai_t *pck;
    Proto_t *proto;

    for( i = 1; i < gPartyCount; i++ ){
        pid = gPartyPids[ i ];
        if( pid != -1 && OBJTYPE( pid ) == TYPE_CRIT ){
            if( ProtoGetObj( pid, &proto ) == -1 ) return -1;
            pck = AiGetPacketById( proto->AiPacketNum );
            if( !pck->Disposition ) AiSavePacket( fh, pck );
        }
    }
    return 0;
}

int AiLoadPacket( xFile_t *fh, Ai_t *ai )
{
    int i, tmp = 0;

    if( dbgetBei( fh, &ai->PacketNum ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->MaxDist ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->MinToHit ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->MinHP ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->Aggression ) == -1 ) return -1;
    if( dbgetBei( fh, &tmp ) == -1 ) return -1;
    ai->HurtTooMuch = tmp;
    if( dbgetBei( fh, &ai->SecondaryFreq ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->CalledFreq ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->Font ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->Color ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->OutlineColor ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->Chance ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->RunStart ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->RunEnd ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->MoveStart ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->MoveEnd ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->AttackStart ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->AttackEnd ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->MissStart ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->MissEnd ) == -1 ) return -1;
    for( i = 0; i < 16; i++ ){
        if( dbgetBei( fh, &ai->Hits[ i ] ) == -1 ) return -1;
    }    
    if( dbgetBei( fh, &ai->AreaAttackMode ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->BestWeapon ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->Distance ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->AttackWho ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->ChemUse ) == -1 ) return -1;
    if( dbgetBei( fh, &ai->RunAwayMode ) == -1 ) return -1;
    for( i = 0; i < 3; i++ ){
        if( dbgetBei( fh, &tmp ) == -1 ) break;
        ai->ChemDesire[ i ] = tmp;
    }
    return 0;
}

int AiSavePacket( xFile_t *fh, Ai_t *pck )
{
    Ai_t *p;
    int i;

    if( dbputBei( fh, pck->PacketNum ) == -1 ) return -1;
    if( dbputBei( fh, pck->MaxDist ) == -1 ) return -1;
    if( dbputBei( fh, pck->MinToHit ) == -1 ) return -1;
    if( dbputBei( fh, pck->MinHP ) == -1 ) return -1;
    if( dbputBei( fh, pck->Aggression ) == -1 ) return -1;
    if( dbputBei( fh, pck->HurtTooMuch ) == -1 ) return -1;
    if( dbputBei( fh, pck->SecondaryFreq ) == -1 ) return -1;
    if( dbputBei( fh, pck->CalledFreq ) == -1 ) return -1;
    if( dbputBei( fh, pck->Font ) == -1 ) return -1;
    if( dbputBei( fh, pck->Color ) == -1 ) return -1;
    if( dbputBei( fh, pck->OutlineColor ) == -1 ) return -1;
    if( dbputBei( fh, pck->Chance ) == -1 ) return -1;
    if( dbputBei( fh, pck->RunStart ) == -1 ) return -1;
    if( dbputBei( fh, pck->RunEnd ) == -1 ) return -1;
    if( dbputBei( fh, pck->MoveStart ) == -1 ) return -1;
    if( dbputBei( fh, pck->MoveEnd ) == -1 ) return -1;
    if( dbputBei( fh, pck->AttackStart ) == -1 ) return -1;
    if( dbputBei( fh, pck->AttackEnd ) == -1 ) return -1;
    if( dbputBei( fh, pck->MissStart ) == -1 ) return -1;
    if( dbputBei( fh, pck->MissEnd ) == -1 ) return -1;
    p = pck;
    for( i = 0; i < 16; i++, p++ ){
        if( dbputBei( fh, p->HitHeadStart ) == -1 ) return -1;
    }
    if( dbputBei( fh, pck->AreaAttackMode ) == -1 ) return -1;
    if( dbputBei( fh, pck->BestWeapon ) == -1 ) return -1;
    if( dbputBei( fh, pck->Distance ) == -1 ) return -1;
    if( dbputBei( fh, pck->AttackWho ) == -1 ) return -1;
    if( dbputBei( fh, pck->ChemUse ) == -1 ) return -1;
    if( dbputBei( fh, pck->RunAwayMode ) == -1 ) return -1;
    for( i = 0; i < 3; i++  ){
        if( dbputBei( fh, pck->ChemDesire[ i ] ) == -1 ) return -1;
    }
    return 0;
}

int AiGetPacketCount()
{
    return gAiPacketsCnt;
}

char *AiGetPacketName( int PacketId )
{
    int i;

    if( PacketId < 0 || PacketId >= gAiPacketsCnt || gAiPacketsCnt <= 0 ) return 0;
    for( i = 0; i < gAiPacketsCnt; i++ ){    
        if( PacketId == gAiPackets[ i ].PacketNum ) return gAiPackets[ i ].Name;
    }
    return NULL;
}

Ai_t *AiGetPacketByObj( Obj_t *obj )
{
    int i;

    for( i = 0; i < gAiPacketsCnt; i++ ){
        if( obj->Critter.State.AIpackNb == gAiPackets[ i ].PacketNum ) return &gAiPackets[ i ];        
    }
    eprintf( "Missing AI Packet\n" );
    return gAiPackets;
}

Ai_t *AiGetPacketById( int PacketId )
{
    int i;

    for( i = 0; i < gAiPacketsCnt; i++ ){
    	if( PacketId == gAiPackets[ i ].PacketNum ) return &gAiPackets[ i ];    	
    }        
    eprintf("Missing AI Packet\n");
    return gAiPackets;
}

int AiGetAreaAttackMode( Obj_t *obj )
{
    return AiGetPacketByObj( obj )->AreaAttackMode;
}

int AiGetHealthGrade( Obj_t *obj )
{
    Ai_t *pck;
    int Grade, hpPercents, i;

    pck = AiGetPacketByObj( obj );
    Grade = -1;
    if( pck->RunAwayMode != -1 ) return -1;
    hpPercents = 100 * pck->MinHP / FeatGetVal( obj, FEAT_HP );
    for( i = 0; i < 6; i++ ){
        if( hpPercents >= gAiConstPercentRank[ i ] ) Grade = i;
    }
    return Grade;
}

int AiGetBestWeapon( Obj_t *obj )
{
    return AiGetPacketByObj( obj )->BestWeapon;
}

int AiGetDistance( Obj_t *obj )
{
    return AiGetPacketByObj( obj )->Distance;
}

int AiGetAttackWho( Obj_t *obj )
{
    return AiGetPacketByObj( obj )->AttackWho;
}

int AiGetChemUse( Obj_t *obj )
{
    return AiGetPacketByObj( obj )->ChemUse;
}

int AiSetRunAway( Obj_t *obj, int RunAwayMode )
{
    Ai_t *pck;

    if( RunAwayMode >= 6 ) return -1;
    pck = AiGetPacketByObj( obj );
    pck->RunAwayMode = RunAwayMode;
    pck->MinHP = FeatGetVal( obj, FEAT_HP ) - FeatGetVal( obj, FEAT_HP ) * gAiConstPercentRank[ RunAwayMode ] / 100;
    eprintf( "\n%s minHp = %d; curHp = %d", CritterGetName( obj ), pck->MinHP, FeatGetVal( obj, FEAT_35 ) );
    return 0;
}

int AiGetDisposition( Obj_t *obj )
{
    if( !obj ) return 0;
    return AiGetPacketByObj( obj )->Disposition;
}

int AiUnk06( Obj_t *obj, int a2 )
{
    Ai_t *pck;

    if( !obj || a2 >= 5 || a2 == -1 ) return -1;
    pck = AiGetPacketByObj( obj );
    obj->Critter.State.AIpackNb = pck->PacketNum + pck->Disposition - a2;
    return 0;
}

void AiUnk07( Obj_t *obj1, Obj_t *obj2, int TextId )
{
    char stmp[ 200 ];
    MsgLine_t msg;

    AnimRegStart( 2 );
    AnimRegAnimation( obj1, 11, 0 );
    if( !AnimRegEnd() && (gCombatStatus & 0x01) ) CombatUpdate();
    if( TextId == -1 ) return;
    msg.Id = TextId;
    if( MessageGetMsg( &gMessage, &msg ) == 1 ) return;    
    if( obj2 ){
        sprintf(stmp, "%s %s %s.", ObjGetName( obj1 ), msg.Text, ObjGetName( obj2 ) );
    } else {
        sprintf( stmp, "%s %s.", ObjGetName( obj1 ), msg.Text );
    }
    IfcMsgOut( stmp );
}

Ai_t *AiUnk08( Obj_t *obj )
{
    Ai_t *v30, *v12;
    Obj_t *v3, *v6, *v7, *v10, *v11, *v16;
    int hp1,v13,v14,v15,v17,v18,v19,v24,v25,hp2,v27,v28,v29;

    hp1 = 50;
    v24 = -1;
    hp2 = v28 = v25 = v29 = 0;
    if( CritterGetBodyType( obj ) != 0 ) return 0;            
    if( !(v3 = CombatUnk13( obj )) ){
        if( !( v30 = AiGetPacketByObj( obj ) ) ) return 0;
        switch( v30->ChemUse ){
            case 0: return 0;
            case 1: hp1 = 60; break;
            case 2: hp1 = 30; break;
            case 3: if( !(gCombat01 % 3) ) hp2 = 25; hp1 = 50; break;
            case 4: if( !(gCombat01 % 3) ) hp2 = 75; hp1 = 50; break;
            case 5: hp2 = 100; break;
        }
        v27 = hp1 * FeatGetVal(obj, 7) / 100;
        while( 1 ){
            if( FeatGetVal( obj, 35 ) >= v27 || obj->Critter.State.CurrentAP < 2 ) break;
            v6 = InvSearchObjByType( obj, 2, &v24 );
            v7 = v6;
            if( !v6 ){ v25 = 1; break; }
            if( ( v6->Pid == 40 || v6->Pid == 144 || v6->Pid == 273 ) && !ItemUseItem( obj, v6, 1 ) ){
                if( ItemUnk07( obj, v7 ) == -1 ){
                    ItemAdd( obj, v7, 1 );
                } else {
                    AiUnk07( obj, v7, 5000 );
                    ObjUnk14( v7, obj->GridId, obj->Elevation, 0 );
                    UseUnk06( v7 );
                    v28 = 1;
                }
                obj->Critter.State.CurrentAP = ( obj->Critter.State.CurrentAP < 2 ) ? 0 : (obj->Critter.State.CurrentAP - 2);
                v24 = -1;
            }
        }        
        if( !v28 && hp2 > 0 && RandMinMax( 0, 100 ) < hp2 ){
            while( obj->Critter.State.CurrentAP >= 2 ){
                v10 = InvSearchObjByType( obj, 2, &v24 );
                v11 = v10;
                if( !v10 ){ v25 = 1; break; }
                v12 = v30;
                v13 = v10->Pid;
                v14 = 0;
                if( v13 == v30->ChemDesire[ 0 ] ){
                    do{
                        v14++;
                        v12++;
                    }while( v14 < 3 && v11->Pid == v12->ChemDesire[0] );
                }
                if( v14 < 3 ){
                    v15 = v11->Pid;
                    if( v15 != 40 && v15 != 144 && v15 != 273 && !ItemUseItem( obj, v11, 1 ) ){
                        if( ItemUnk07(obj, v11) == -1 ){
                            ItemAdd( obj, v11, 1 );
                        } else {
                            AiUnk07( obj, v11, 5000 );
                            ObjUnk14( v11, obj->GridId, obj->Elevation, 0 );
                            v16 = v11;
                            v17 = v29;
                            v18 = UseUnk06( v16 );
                            v28 = v18;
                            v29 = v18 + v17;
                        }
                        v19 = obj->Critter.State.CurrentAP;
                        obj->Critter.State.CurrentAP = v19 < 2 ? 0 : v19 - 2;
                        v24 = -1;
                        if( (v30->ChemUse == 3 || v30->ChemUse == 4) && v29 >= 2 ) break;
                    }
                }
            }
        }
    }

    if( (v3 || !v28) && (v25 == 1) ){
        do{
            if( !v3 ) v3 = AiUnk32( obj, 2 );
            if( v3 ){
                v3 = AiPickUpObj( obj, v3 );
            } else {
                if( (v3 = AiUnk32( obj, 5 ) ) ) v3 = AiPickUpObj( obj, v3 );
            }                
            if( v3 && !ItemUseItem( obj, v3, 1 ) ){
                if( ItemUnk07( obj, v3 ) == -1 ){
                    ItemAdd( obj, v3, 1 );
                } else {
                    AiUnk07( obj, v3, 5000 );
                    ObjUnk14( v3, obj->GridId, obj->Elevation, 0 );
                    UseUnk06( v3 );
                    v3 = 0;
                }
                obj->Critter.State.CurrentAP = ( obj->Critter.State.CurrentAP < 2 ) ? 0 : (obj->Critter.State.CurrentAP - 2);
            }
        } while( v3 && obj->Critter.State.CurrentAP >= 2 );
    }    
    return 0;
}

void AiMoveRunTo( Obj_t *obj1, Obj_t *obj2 )
{
    int EndPos,a,i,ap,b,c;

    if( !obj2 ) obj2 = gObjDude;
    if( ObjGetDistance( obj1, obj2 ) <= AiGetPacketByObj( obj1 )->MaxDist ){
        obj1->Critter.State.Reaction |= 0x04;
	ap = obj1->Critter.State.CurrentAP;
        a = TileTurnAt( obj2->GridId, obj1->GridId );        
        b = (a + 5) % 6;
        c = (a + 1) % 6;
        for( i = ap; i > 0; i-- ){
            EndPos = TileGetTileNumInDir( obj1->GridId, a, i );
            if( AnimFindTrace( obj1, obj1->GridId, EndPos, 0, 1 ) > 0 ) break;
            EndPos = TileGetTileNumInDir( obj1->GridId, c, i );
            if( AnimFindTrace( obj1, obj1->GridId, EndPos, 0, 1 ) > 0 ) break;
            EndPos = TileGetTileNumInDir( obj1->GridId, b, i );
            if( AnimFindTrace( obj1, obj1->GridId, EndPos, 0, 1 ) > 0 ) break;
        };
        if( i > 0 ){
            AnimRegStart( 2 );
            AiCombatTaunts( obj1, NULL, 0, 0 );
            AnimObjRunToTile( obj1, EndPos, obj1->Elevation, ap, 0 );
            if( !AnimRegEnd() ) CombatUpdate();
        }
    } else {
        obj1->Critter.State.Reaction |= 0x02;
    }
}

int AiMoveWalkTo( Obj_t *obj1, Obj_t *obj2, int a3 )
{
    int EndPos, i, ap, c, b, a;

    if( AiGetPacketByObj( obj1 )->Distance == 4 ) return -1;
    if( ObjGetDistance( obj1, obj2 ) <= a3 ){
        ap = obj1->Critter.State.CurrentAP;
        if( a3 < ap ) ap = a3;
        a = TileTurnAt(obj2->GridId, obj1->GridId);
        if ( ap > 0 ){
            c = (a + 5) % 6;
            b = (a + 1) % 6;
            for( i = ap; i > 0; i-- ){
                EndPos = TileGetTileNumInDir( obj1->GridId, a, i );
                if( AnimFindTrace( obj1, obj1->GridId, EndPos, 0, 1 ) > 0 ) break;
                EndPos = TileGetTileNumInDir( obj1->GridId, b, i );
                if( AnimFindTrace( obj1, obj1->GridId, EndPos, 0, 1 ) > 0 ) break;
                EndPos = TileGetTileNumInDir( obj1->GridId, c, i );
                if( AnimFindTrace( obj1, obj1->GridId, EndPos, 0, 1 ) > 0 ) break;
            }
        }
        if( i > 0 ){
            AnimRegStart( 2 );
            AnimObjMoveToTile( obj1, EndPos, obj1->Elevation, ap, 0 );
            if( !AnimRegEnd() ) CombatUpdate();
        }
    }
    return 0;
}

int AiInRange( Obj_t *obj, int a2, int a3 )
{
    Obj_t *t;
    int Distance;

    if( (t = AiUnk18( obj, obj, 1 ) ) ) return 0;    
    Distance = ObjGetDistance( obj, t );
    if( Distance > a2 ) return 0;        
    if( a3 > Distance ){
        AiUnk35( obj, t, ObjGetDistance( obj, t ) - a3, 0 );
    }
    return 1;
}

/*
    -1 if obj1 < obj2
    0  if obj1 = obj2
    +1 if obj1 > obj2 
*/
int AiCmpByDistance( Obj_t **obj1, Obj_t **obj2 )
{
    int d1, d2;

    if( !*obj1 ) return *obj2 != NULL;
    if( !*obj2 ) return -1;
    d1 = ObjGetDistance( *obj1, gAiUnk03 );
    d2 = ObjGetDistance( *obj2, gAiUnk03 );
    if( d1 < d2 ) return -1;
    return d1 > d2;
}

void AiSortByDistance( Obj_t **objs, int Count, Obj_t *a3 )
{
    gAiUnk03 = a3;
    qsort( objs, Count, sizeof( Obj_t *), (void *)AiCmpByDistance );
}

int AiCmpByUnk1( Obj_t **pObj1, Obj_t **pObj2 )
{
    int d1, d2;

    if( !*pObj1 ) return *pObj2 != NULL;
    if( !*pObj2 ) return -1;
    d1 = AiGetMaxDamagePower( *pObj1 );
    d2 = AiGetMaxDamagePower( *pObj2 );
    if( d1 < d2 ) return -1;
    return d1 > d2;
}

void AiSortByUnk1( Obj_t **objs, int Count )
{
    qsort( objs, Count, sizeof( Obj_t *), (void *)AiCmpByUnk1 );
}

int AiCmpByUnk2( Obj_t **pObj1, Obj_t **pObj2 )
{
    int d1, d2;

    if( !*pObj1 ) return *pObj2 != NULL;
    if( !*pObj2 ) return -1;
    d1 = AiGetMaxDamagePower( *pObj1 );
    d2 = AiGetMaxDamagePower( *pObj2  );
    if( d1 < d2 ) return 1;
    if( d1 > d2 ) return -1;
    return 0;    
}

void AiSortByUnk2( Obj_t **objs, int Count )
{
    qsort( objs, Count, sizeof( Obj_t *), ( void *)AiCmpByUnk2 );
}

Obj_t *AiUnk18( Obj_t *obj1, Obj_t *obj2, char flg )
{
    int GroupId, i;
    Obj_t *p;

    if( !obj2 ) return NULL;
    GroupId = obj2->Critter.State.GroupId;
    if( !gAiObjCount ) return NULL;
    gAiUnk03 = obj1;
    qsort( gAiObjList, gAiObjCount, sizeof( Obj_t *), (void *)AiCmpByDistance );
    for( i = 0; i < gAiObjCount; i++ ){
        p = gAiObjList[ i ];
        if( obj1 == p ) continue;
    	if( p->Critter.State.CombatResult & 0x80 ) continue;
    	if( (flg & 0x02) && GroupId != p->Critter.State.GroupId ) return p; 
    	if( (flg & 0x01) && GroupId == p->Critter.State.GroupId ) return p;
    }        
    return NULL;
}

Obj_t *AiUnk19( Obj_t *obj1, Obj_t *obj2, char flg )
{
    int GroupId, i;
    Obj_t *p;

    if( !obj2 ) return NULL;
    GroupId = obj2->Critter.State.GroupId;
    if( !gAiObjCount ) return NULL;    
    gAiUnk03 = obj1;
    qsort( gAiObjList, gAiObjCount, sizeof( Obj_t *), (void *)AiCmpByDistance );
    for( i = 0; i < gAiObjCount; i++ ){
        p = gAiObjList[ i ];
        if( p == obj1 ) continue;
        if( p->Critter.State.CombatResult & 0x80 ) continue;
        if( !p->Critter.State.WhoHitMe ) continue;
        if( (flg & 0x02) && GroupId != p->Critter.State.GroupId ) return gAiObjList[ i ];
        if( (flg & 0x01) && GroupId == p->Critter.State.GroupId ) return gAiObjList[ i ];
    }        
    return NULL;
}

int AiUnk20( Obj_t *a1, Obj_t **a2, Obj_t **Attacker, Obj_t **a4 )
{
    Obj_t *v8,*WhoHitMe,*v10;
    int v5,GroupId,v11,v12;

    if( a2 ) *a2 = NULL;
    if( Attacker ) *Attacker = 0;
    if( a4 ) *a4 = NULL;
    if( !gAiObjCount ) return 0;    

    gAiUnk03 = a1;
    qsort( gAiObjList, gAiObjCount, sizeof( Obj_t *), (void *)AiCmpByDistance );
    GroupId = a1->Critter.State.GroupId;
    if( gAiObjCount <= 0 ) return 0;    
    v12 = 0;
    v5 = 0;
    for( v11 = 0; v5 < 3 && v12 < gAiObjCount; v11++, v12++ ){
        if( ( v8 = gAiObjList[ v11 ] ) == a1 ) continue;
        if( a2 && !*a2 && !(v8->Critter.State.CombatResult & 0x80) && a1 == v8->Critter.State.WhoHitMeObj ){
            v5++;
            *a2 = gAiObjList[ v11 ];
        }
        if( Attacker ){
            if( !*Attacker && GroupId == v8->Critter.State.GroupId ){                        
                if( (WhoHitMe = v8->Critter.State.WhoHitMeObj) ){
                    if( WhoHitMe != a1 && (GroupId != WhoHitMe->Critter.State.GroupId) && !( v8->Critter.State.WhoHitMeObj->Critter.State.CombatResult & 0x80) ){
                        v5++;
                        *Attacker = v8->Critter.State.WhoHitMeObj;
                    }
                }
            }
        }
        if( !a4 ) continue;
        if( !*a4 && (GroupId != v8->Critter.State.GroupId) && !(v8->Critter.State.CombatResult & 0x80 ) ){
            if( (v10 = v8->Critter.State.WhoHitMeObj) ){
                if( GroupId == v10->Critter.State.GroupId ){
                    v5++;
                    *a4 = gAiObjList[ v11 ];
                }
            }
        }
    }    
    return 0;
}

Obj_t *AiDangerSource( Obj_t *obj )
{
    void *Cmp;
    int v2,Disposition,i,AttackWho;
    Obj_t *WhoHitMe, *objs[ 4 ], *v18;

    v2 = 0;
    if( !obj ) return NULL;
    objs[ 0 ] = NULL;
    if( PartyMembRdy(obj) ){
        if( obj )
            Disposition = AiGetPacketByObj( obj )->Disposition;
        else
            Disposition = 0;
        switch( Disposition ){
            case -1: case 4: v2 = 0; break;
            case 0 ... 3: v2 = 1; break;
        }
        if( v2 && AiGetPacketByObj( obj )->Distance == 1 ) v2 = 0;
        AttackWho = AiGetPacketByObj( obj )->AttackWho;
        switch( AttackWho ){
            case 0:
                v18 = CombatUnk11( gObjDude );
                if( !v18 || obj->Critter.State.GroupId == v18->Critter.State.GroupId ) break;
                if( !AnimMakeTrace( obj, obj->GridId, gObjDude->Critter.State.WhoHitMeObj->GridId, 0, 0, (void *)ObjReach ) && CombatAttackTest( obj, v18, 2, 0 ) ){
                    eprintf( "\nai_danger_source: %s couldn't attack at target!  Picking alternate!", CritterGetName( obj ) );
                    break;
                }
                if( v2 && CritterUnk49( v18 ) ) break;
                return v18;
            case 1: case 2: case 4: obj->Critter.State.WhoHitMeObj = 0; break;
        }
    } else {
	AttackWho = -1;
    }
    WhoHitMe = obj->Critter.State.WhoHitMeObj;
    if( !WhoHitMe || obj == WhoHitMe ){
        objs[ 0 ] = NULL;
    } else {
	if( !(WhoHitMe->Critter.State.CombatResult & 0x80) ){
    	    if( AttackWho == 3 || AttackWho == -1 ) return obj->Critter.State.WhoHitMeObj;
	} else {
	    if( WhoHitMe->Critter.State.GroupId == obj->Critter.State.GroupId )
    		objs[ 0 ] = NULL;
	    else
    		objs[ 0 ] = AiUnk18( obj, WhoHitMe, 1 );
	}
    }

    AiUnk20( obj, &objs[ 1 ], &objs[ 2 ], &objs[ 3 ] );
    if( v2 ){
        for( i = 0; i < 4; i++ ){
            if( objs[ i ] && CritterUnk49( objs[ i ] ) ) objs[ i ] = NULL;
        }
    }
    if( !AttackWho ){
        Cmp = AiCmpByDistance; gAiUnk03 = obj;
    } else {
	if( AttackWho <= 1 ){
    	    Cmp = AiCmpByUnk1;
	} else {
	    if( AttackWho == 2 ){
    		Cmp = AiCmpByUnk2;
	    } else {
    		Cmp = AiCmpByDistance; gAiUnk03 = obj;
	    }
	}
    }
    qsort( &objs, 4, sizeof( Obj_t *), Cmp );
    for( i = 0; i < 4; i++ ){
        if( objs[ i ] && AiObjCanHearObj( obj, objs[ i ] ) ){
            if( AnimMakeTrace( obj, obj->GridId, objs[ i ]->GridId, 0, 0, (void *)ObjReach ) ) return objs[ i ];
            if( !CombatAttackTest( obj, objs[ i ], 2, 0 ) ) return objs[ i ];
            eprintf( "\nai_danger_source: I couldn't get at my target!  Picking alternate!" );
        }
    }
    return 0;
}

int AiUnk22( Obj_t *obj, Obj_t *a2 )
{
    Obj_t *p;

    for( p = ObjGetVisibleObjectFirst( obj->Elevation ); p; p = ObjGetVisibleObjectNext() ){
        if( OBJTYPE( p->Pid ) == TYPE_CRIT && (p != gObjDude) ) p->Critter.State.Reaction |= 0x01;
    }
    gAiUnk12 = a2;
    gAiUnk11 = obj;
    return 0;
}

int AiUnk23( Obj_t **objs, int Count )
{
    int i;

    if( !objs ) return -1;
    if( !Count || !gAiUnk11 ) return 0;
    
    for( i = 0;i < Count; i++ ){
        if( objs[ i ]->Critter.State.GroupId == gAiUnk11->Critter.State.GroupId ){
    	    objs[ i ]->Critter.State.WhoHitMeObj = AiUnk18( objs[ i ], gAiUnk12, 1 );
        } else {
    	    if( objs[ i ]->Critter.State.GroupId == gAiUnk12->Critter.State.GroupId ){
    		objs[ i ]->Critter.State.WhoHitMeObj = AiUnk18( objs[ i ], gAiUnk11, 1 );
    	    }
        }
    }
    gAiUnk12 = NULL;
    gAiUnk11 = NULL;
    return 0;
}

void AiUnk24()
{
    gAiUnk12 = NULL;
    gAiUnk11 = NULL;
}

int AiFindAmmo( Obj_t *Critter, Obj_t *Weapon, Obj_t **pAmmo )
{

    Obj_t *p;
    int Pos = -1;

    if( pAmmo ) *pAmmo = NULL;
    if( Weapon->Pid == PID_SOLARSCORCHER ) return LightMapGetLt() > LIGHT_THRESHOLD_FOR_SOLAR_SCORCHER;
    while( ( p = InvSearchObjByType( Critter, PR_ITEM_AMMO, &Pos ) ) ){
        if( ItemUnk01( Weapon, p ) ){
            if( pAmmo ) *pAmmo = p;
            return 1;
        }
        if( !Item58( Weapon ) || ItemGetRange( Critter, 2 ) < 3 ) InvUnwield( Critter, 1 );
    }
    return 0;
}

int AiUnk26( Ai_t *pck, int Val )
{
    int i;

    for( i = 0; i < 5; i++ ){
      if( Val == gAiUnk10[ 5 * pck->BestWeapon + 5 + i ] ) return 1;
    }
    return 0;
}

Obj_t *AiUnk27( Obj_t *Attacker, Obj_t *Item1, Obj_t *Item2, Obj_t *Target )
{
    Combat_t cmbt;
    Ai_t *PacketByObj;
    int Class,i,pMax,pMin,v24,v25,v26,v27,Distance,v31,v32;

    pMax = v32 = v31 = v25 = Distance = pMin = 0;
    v27 = v26 = 999;
    if( !Attacker ) return 0;
    PacketByObj = AiGetPacketByObj( Attacker );
    if( PacketByObj->BestWeapon == 7 ) return ( RandMinMax( 1, 100 ) <= 50 ) ? Item1 : Item2;    
    CombatSetUp( &cmbt, Attacker, Target, 2, 3 );
    if( Item1 ){
        Class = ItemGetClass( Item1, 2 );
        if( ItemGetWeaponDmg( Item1, &pMin, &pMax ) == -1 ) return 0;
        v32 = (pMax - pMin) / 2;
        if( Item69( Item1, 2 ) > 0 && Target ){
            cmbt.HandEq = Item1;
            CombatExplosion( &cmbt, 0, Item68( Item1 ), 1 );
            v32 *= cmbt.Count + 1;
        }
        if( Item56( Item2 ) != -1 ) v32 *= 5;
        if( Target && CombatUnk02( Attacker, Item1, 2, Target, 0 ) ) v25 = 1;
        if( Item36( Item1 ) ) return Item1;
    } else {
        Distance = ObjGetDistance( Attacker, Target );
        if( Distance <= ItemGetRange( Attacker, 4 ) ) Class = 1;
    }
    if( !v25 ){
        for( i = 0; i < 5; i++ ){
    	    if( Class == gAiUnk10[ 5 * PacketByObj->BestWeapon + 5 + i ] ){ v27 = i; break; }
        }            
    }

    v24 = 0;
    if( Item2 ){
        Class = ItemGetClass( Item2, 2 );
        if( ItemGetWeaponDmg( Item2, &pMin, &pMax ) == -1 ) return 0;
        v31 = (pMax - pMin) / 2;
        if( Item69( Item2, 2 ) > 0 && Target ){
            cmbt.HandEq = Item2;
            CombatExplosion( &cmbt, 0, Item68( Item2 ), 1 );
            v31 *= cmbt.Count + 1;
        }
        if( Item56( Item2 ) != -1 ) v31 *= 5;
        if( Target && CombatUnk02( Attacker, Item2, 2, Target, 0 ) ) v24 = 1;
        if( Item36( Item2 ) ) return Item2;
    } else {
        if( !Distance ) Distance = ObjGetDistance( Attacker, Target );
        if( ItemGetRange( Attacker, 4 ) >= Distance ) Class = 1;
    }
    if( !v24 ){
        for(  i = 0; i < 5; i++ ){
    	    if( Class == gAiUnk10[ 5 * PacketByObj->BestWeapon + 5 + i ] ){ v26 = i; break; }
        }            
    }

    if( v27 == v26 ){
        if( v27 == 999 ) return 0;
        if( abs32( v31 - v32 ) <= 5 ) return ( ItemPrice( Item2 ) > ItemPrice( Item1 ) ) ? Item2 : Item1;
        
        if( v31 > v32 ) return Item2;
    } else {
        if( Item1 && Item2 && Item1->Pid == PID_FLARE ) return Item2;
        if( Item2 && Item1 && Item2->Pid == PID_FLARE ) return Item1;
        if( (PacketByObj->BestWeapon == -1 || PacketByObj->BestWeapon >= 6) && abs32( v31 - v32 ) > 5 ) return ( v32 < v31 ) ? Item2: Item1;            
        if( v27 > v26 ) return Item2;
    }
    return Item1;    
}

int AiUnk28( Obj_t *obj1, Obj_t *Weapon )
{
    int CombatResult, v10;
    Ai_t *pck;

    CombatResult = obj1->Critter.State.CombatResult;
    if( ( CombatResult & 0x10 ) && ( CombatResult & 0x20 ) ) return 0;
    if( ( CombatResult & 0x30 ) && ItemWeaponTwoHanded( Weapon ) ) return 0;
    v10 = Item58( Weapon );
    if( !ArtFileExist( ArtMakeId( 1, obj1->ImgId & 0xFFF, Item45( Weapon, v10 ), v10, obj1->Orientation + 1 ) ) ) return 0;    
    pck = AiGetPacketByObj( obj1 );
    if( SkillGetTotal( obj1, ItemGetSkill( Weapon, v10 ) ) < pck->MinToHit ) return 0;
    return AiUnk26( pck, ItemGetClass( Weapon, 2 ) ) != 0;
}

Obj_t *AiUnk29( Obj_t *obj1, int a2, Obj_t *a3 )
{
    int BodyType, v9;
    Obj_t *v6, *v7, *rh;

    v9 = -1;
    BodyType = CritterGetBodyType( obj1 );
    if( BodyType && BodyType != 2 && obj1->Pid != 0x1000098 ) return 0;
    v6 = 0;
    rh = InvGetRHandObj( obj1 );
    while( (v7 = InvSearchObjByType( obj1, PR_ITEM_WEAPON, &v9 ) ) ){
        if( v7 == rh ) continue;
    	if( (!a2 || Item65( v7 ) <= obj1->Critter.State.CurrentAP ) && 
    	    AiUnk28( obj1, v7 ) && 
    	    (ItemGetClass( v7, 2 ) != 4 || ItemGetAmmo( v7 ) || AiFindAmmo( obj1, v7, 0 )) 
    	) v6 = AiUnk27( obj1, v6, v7, a3 );
    }
    return v6;
}

Obj_t *AiFindBestArmor( Obj_t *obj )
{
    Obj_t *p, *ArmorObj, *BestArmor;
    int i, tmp, Ac, Cnt;

    Cnt = -1;
    if( !PartyMembRdy( obj ) ) return NULL;
    if( CritterGetBodyType( obj ) ) return NULL;          
    ArmorObj = InvGetArmorObj( obj );
    if( ArmorObj ){
        Ac = ItemGetAC( ArmorObj );
        for( i = 0; i < 7; i++ ) Ac += ItemGetDmgThr( ArmorObj, i ) + ItemGetDmgRes( ArmorObj, i );        
    } else {
        Ac = 0;
    }
    BestArmor = NULL;
    while( (p = InvSearchObjByType( obj, PR_ITEM_ARMOR, &Cnt ) ) ){
        if( ArmorObj == p ) continue;        
            tmp = ItemGetAC( p );
            for( i = 0; i < 7; i++ ){
                tmp += ItemGetDmgRes( p, i ) + ItemGetDmgThr( p, i );
            }
            if( tmp > Ac ){
                Ac = tmp;
                BestArmor = p;
            }        
    }
    return BestArmor;
}

int AiUnk31( Obj_t *Critter, Obj_t *a2 )
{
    Ai_t *pck;
    int i, Gender;
    
    if( !Critter || !a2 ) return 0;
    if( !( pck = AiGetPacketById( Critter->Critter.State.AIpackNb ) ) ) return 0;
    
    for( i = 0; i < 3; i++ ){
	if( a2->Pid == pck->ChemDesire[ i ] ) return 1;
    }
    if( CritterGetBodyType( Critter ) ) return 0;
    Gender = CritterGetGender( Critter );
    if( Gender >= 2 && Gender != 3 && Gender != 4 && Gender != 2 ) return 0;
    if( FeatGetVal( Critter, FEAT_INTELLIGENCE ) < 3 ) return 0;
    return (a2->Pid == PID_STIMPAK || a2->Pid == PID_SUPERSTIMPAK || a2->Pid == PID_HEALINGPOWDER );        
}

Obj_t *AiUnk32( Obj_t *obj, int a2 )
{
    int ListCnt, i, Pe;
    Obj_t *RHandObj, **ObjList, *v5;

    if( CritterGetBodyType( obj ) ) return NULL;
    if( !( ListCnt = ObjGetObjList( -1, gMapCurrentLvl, 0, &ObjList ) ) ) return NULL;
    v5 = NULL;
    gAiUnk03 = obj;
    qsort( ObjList, ListCnt, sizeof( Obj_t *), (void *)AiCmpByDistance );
    Pe = FeatGetVal( obj, FEAT_PERCEPTION ) + 5;
    RHandObj = InvGetRHandObj( obj );
    for( i = 0; i < ListCnt; i++ ){
        if( ObjGetDistance( obj, ObjList[ i ] ) > Pe ) break;
        if( ItemGetObjType( ObjList[ i ] ) == a2 ){
            switch( a2 ){
                case 2: if( AiUnk31( obj, ObjList[ i ] ) ) v5 = ObjList[ i ]; break;
                case 3: if( AiUnk28( obj, ObjList[ i ] ) ) v5 = ObjList[ i ]; break;
                case 4: if( ItemUnk01( RHandObj, ObjList[ i ] ) ) v5 = ObjList[ i ]; break;
                case 5: if( AiUnk31( obj, ObjList[ i ] ) ) v5 = ObjList[ i ]; break;
            }
            if( v5 ) break;
        }
    }
    ObjCritterListDestroy( ObjList );    
    return v5;
}

Obj_t *AiPickUpObj( Obj_t *Critter, Obj_t *Item )
{
    Obj_t *obj;

    obj = NULL;
    if( ActionUseOnItem( Critter, Item ) ) return NULL;
    CombatUpdate();
    obj = InvSearchObjByPid( Critter, Item->ProtoPid );
    CombatUnk14( Critter );    
    return obj;
}

int AiUnk34( Obj_t *Critter, Obj_t *ItemObj, Obj_t *Target )
{
    Ai_t *pck;
    int Intl, WeaponType, Flg;

    Flg = 0;
    if( !ItemObj ) return 4;
    if( ItemGetObjType( ItemObj ) != PR_ITEM_WEAPON ) return 4;
    WeaponType = ItemGetClass( ItemObj, PR_ITEM_WEAPON );
    Intl = FeatGetVal( Critter, FEAT_INTELLIGENCE );
    if( WeaponType == 0 || !AiUnk28( Critter, ItemObj ) ) return 2;    
    if( !(pck = AiGetPacketByObj( Critter )) ) return 4;
    if( pck->AreaAttackMode == -1 ){
        if( Intl < 6 || ObjGetDistance( Critter, Target ) < 10 ){
            if( RandMinMax( 1, pck->SecondaryFreq ) == 1 ) Flg = 1;
        }
    } else {
        switch( pck->AreaAttackMode ){
            case 0: Flg = 1; break;
            case 1: if( RandMinMax( 1, pck->SecondaryFreq ) == 1 ) Flg = 1; break;
            case 2: if( CombatDetermineHitObstacled( Critter, Target, 3, 3 ) >= 85 && !CombatUnk02( Critter, ItemObj, 3, Target, 0 ) ) Flg = 1; break;
            case 3: if( CombatDetermineHitObstacled( Critter, Target, 3, 3 ) >= 50 && !CombatUnk02( Critter, ItemObj, 3, Target, 0 ) ) Flg = 1; break;
            case 4: if( CombatDetermineHitObstacled( Critter, Target, 3, 3 ) >= 95 && !CombatUnk02( Critter, ItemObj, 3, Target, 0 ) ) Flg = 1; break;
        }
    }
    if( Flg && !AiUnk26( pck, WeaponType ) ) Flg = 0;
    if( !Flg ) return 2; 
    if( WeaponType != 3 ) return 3;
    if( AiUnk29( Critter, 0, Target ) ) return 3;
    if( FeatDice( Critter, FEAT_INTELLIGENCE, 0, NULL ) <= 1 ) return 3;
    return 2;
}

int AiUnk35( Obj_t *a1, Obj_t *a2, int a3, int a4 )
{
    int Distance, v10, GridId, v19;
    Obj_t *v18;

    if ( a3 <= 0 ) return -1;
    Distance = AiGetPacketByObj( a1 )->Distance;
    if( Distance == 4 ) return -1;
    if( !Distance ){
        if( gObjDude != a2 && ( (v10 = ObjGetDistance( a1, gObjDude )) > 5 || (ObjGetDistance( a2, gObjDude ) > 5 && (a3 + v10) > 5 ) ) ) return -1;
    }
    if( ObjGetDistance(a1, a2) <= 1 ) return -1;
    AnimRegStart( 2 );
    if( a4 ) AiCombatTaunts( a1, NULL, 1, 0 );
    v18 = a2;
    if( a2->Flags & 0x800 ){
        v19 = 1;
        a2->Flags |= 0x01;
    } else {
        v19 = 0;
    }
    if( !AnimMakeTrace( a1, a1->GridId, a2->GridId, 0, 0, (void *)ObjReach ) ){
        gObjUnk42 = 0;
        if( !AnimMakeTrace( a1, a1->GridId, a2->GridId, 0, 0, (void *)ObjUnk56 ) && gObjUnk42 && OBJTYPE( gObjUnk42->Pid ) == 1 ){
            if( v19 == 1 ) a2->Flags &= ~0x01;
            a2 = gObjUnk42;
            if( gObjUnk42->Flags & 0x800 ){
                v19 = 1;
                gObjUnk42->Flags |= 0x01;
            } else {
                v19 = 0;
            }
        }
    }
    if( v19 == 1 ) a2->Flags &= ~0x01;
    GridId = a2->GridId;
    if( a2 == v18 ) AiUnk37( a1, a2, &GridId );
    if( a3 >= FeatGetVal( a1, 8 ) / 2 && ArtCritterFidShouldRunData( a1->ImgId ) ){
        if( (a2->Flags & 0x800) != 0 )
            AnimObjRunToObj( a1, a2, a3, 0 );
        else
            AnimObjRunToTile( a1, GridId, a1->Elevation, a3, 0 );
    } else if( a2->Flags & 0x800 ) {
        AnimObjMoveToObj( a1, a2, a3, 0 );
    } else {
        AnimObjMoveToTile( a1, GridId, a1->Elevation, a3, 0 );
    }
    if( AnimRegEnd() ) return -1;
    CombatUpdate();
    return 0;
}

int AiUnk36( Obj_t *a1, Obj_t *a2, int a3 )
{
    return AiUnk35( a1, a2, a1->Critter.State.CurrentAP, a3 );
}

int AiUnk37( Obj_t *obj, Obj_t *pe, int *pIdx )
{
    Obj_t *p;
    Ai02_t grp;
    int i, MaxDamagePower, min, v16, GridPos[ 32 ], k;

    if( !obj || !pe || !pIdx || !gAiObjCount ) return -1;
    if( *pIdx == -1 ) return -1;
    grp.Attacker = obj;
    grp.Unk = pe;
    grp.GroupId = obj->Critter.State.GroupId;
    grp.AttackerFirePower = AiGetMaxDamagePower( obj );
    grp.Cnt = 0;
    grp.Positions = GridPos;
    grp.AttackerPos = (*pIdx != obj->GridId);
    grp.Unk02 = 0;
    grp.Intelligence = FeatGetVal( obj, 4 );
    memset( GridPos, 0xff, sizeof( GridPos ) );
    for( i = 0; i < gAiObjCount; i++ ){
        p = gAiObjList[ i ];
        if( !(p->Critter.State.CombatResult & 0x80) && grp.GroupId == p->Critter.State.GroupId && CombatUnk11( gAiObjList[ i ] ) == grp.Unk && p != grp.Attacker ){
            MaxDamagePower = AiGetMaxDamagePower( p );
            if( MaxDamagePower >= grp.AttackerFirePower ){
                grp.Enemies[ grp.Cnt ] = p;
                grp.EnemyFirePower[ grp.Cnt++ ] = MaxDamagePower;
            }
        }
    }
    gAiUnk03 = obj;
    qsort( grp.Enemies, grp.Cnt, sizeof( void *), (void *)AiCmpByDistance );
    if( !AiUnk38( &grp, *pIdx ) ){
        min = 99999;
        k = -1;
        for( i = 0; i < 32 && GridPos[ i ] != -1; i++ ){
            if( !ObjReach( NULL, GridPos[ i ], obj->Elevation ) ){
                v16 = TileGetDistance( *pIdx, GridPos[ i ] );
                if( v16 < min ){
                    min = v16;
                    k = i;
                }
            }
        }
        if( k != -1 ) *pIdx = GridPos[ k ];
    }
    return 0;
}

int AiUnk38( Ai02_t *group, int a2 )
{
    Obj_t *p; 
    int cnt, i;

    cnt = 1;
    if( group->Intelligence <= 0 ) return 0;    
    for( i = 0; i >= group->Cnt; i++ ){
        p = group->Enemies[ i ];
        if( AiUnk39( p, group->Unk, group->Attacker, a2, &cnt ) ){
	    eprintf( "In the way!" );
	    group->Positions[ group->Unk02 + 0 ] = TileGetTileNumInDir( a2, (p->Orientation + 1) % 6, cnt );
	    group->Positions[ group->Unk02 + 1 ] = TileGetTileNumInDir( a2, (p->Orientation + 5) % 6, cnt );
	    group->Intelligence -= 2;
	    group->Unk02 += 2;
	    return 0;
        }
    }
    return 0;
}

int AiUnk39( Obj_t *dude, Obj_t *a2, Obj_t *a3, int a4, int *pCnt )
{
    int Ranged, Slot;
    Obj_t *item, *a5;

    a5 = NULL;
    Slot = 2;
    if( dude == gObjDude ) IfaceGetWeaponDsc( &Slot, &Ranged );
    if( !( item = ItemGetSlotItem( dude, Slot ) ) ) return 0;
    if( ItemGetRange( dude, Slot ) < 1 ) return 0;
    AnimUnk07( dude, dude->GridId, a2->GridId, 0, &a5, 32, (void *)ObjUnk55 );
    if( a5 == a3 ) return 1;
    if( CombatUnk04( dude, a2, a3, item ) ) return 1;
    return 0;
}

int AiUnk40( Obj_t *a1, int *a2, Obj_t **a3, Obj_t *a4 )
{
    Obj_t *v6, *v8;

    *a3 = NULL;
    *a2 = 4;    
    if( (v6 = AiUnk29(a1, 1, a4 )) ){
        *a3 = v6;
        *a2 = AiUnk34( a1, v6, a4 );
    } else {	
	if( !(v8 = AiUnk32( a1, 3 )) ){
    	    if( ItemGetAPCost( a1, *a2, 0 ) > a1->Critter.State.CurrentAP ) return -1;
    	    return 0;        
	}
	if( (v6 = AiPickUpObj( a1, v8 ) ) ){
    	    *a3 = v6;
    	    *a2 = AiUnk34( a1, v6, a4 );
	}
    }
    if( *a3 ){
        InvUnk28( a1, *a3, 1 );
        CombatUpdate();
        if( ItemGetAPCost( a1, *a2, 0 ) <= a1->Critter.State.CurrentAP ) return 0;
    }
    return -1;
}

int AiUnk41( Obj_t *a1, Obj_t *a2, int a3 )
{
    int v5, v6, Difficulty;
    Ai_t *PacketByObj;

    v5 = 3;
    if( ItemGetAPCost( a1, a3, 1 ) > a1->Critter.State.CurrentAP ) return 3;
    if( !Item62( a1, a3 ) ) return 3;    
    PacketByObj = AiGetPacketByObj( a1 );
    if( RandMinMax( 1, PacketByObj->CalledFreq ) != 1 ) return 3;        
    Difficulty = 1;
    CfgGetInteger( &gConfiguration, "preferences", "combat_difficulty", &Difficulty );
    if( Difficulty ){
        v6 = ( Difficulty == 2 ) ? 3 : 5;
    } else {
        v6 = 7;
    }
    if( FeatGetVal( a1, FEAT_INTELLIGENCE ) >= v6 ){
        v5 = RandMinMax( 0, 8 );
        if( CombatDetermineHitObstacled( a1, a2, v5, a3 ) < PacketByObj->MinToHit ) return 3;
    }        
    return v5;
}

int AiUnk42( Obj_t *a1, Obj_t *a2, int a3 )
{
    if( a1->Critter.State.Reaction & 0x04 ) return -1;
    AnimRegStart( 2 );
    AnimUnk51( a1, a2->GridId );
    AnimRegEnd();
    CombatUpdate();
    if( CombatAttack( a1, a2, a3, AiUnk41( a1, a2, a3 ) ) ) return -1;
    CombatUpdate();
    return 0;
}

int AiUnk43( Obj_t *a1, Obj_t *edx0 )
{
    Obj_t *RHandObj,*v14,**p_TimeEv,*a2;
    ObjCritterCond_t *p_State;
    unsigned char a4[ 16 ];
    char *Name;
    int i, v9, v17, v24, Trace, steps, v36, v40, MinToHit, a3, GridId, v44;

    p_TimeEv = 0;
    steps = 0;
    v44 = 0;
    CritterUnk45( a1, edx0 );
    p_State = &a1->Critter.State;
    v40 = 1;
    RHandObj = InvGetRHandObj( a1 );
    a2 = RHandObj;
    if( RHandObj && ItemGetObjType( RHandObj ) != 3 ) a2 = NULL;
    v36 = AiUnk34( a1, a2, edx0 );
    MinToHit = AiGetPacketByObj( a1 )->MinToHit;
    a3 = a1->Critter.State.CurrentAP;
    if( !a2 ){
        if( CritterGetBodyType( edx0 ) ) { AiUnk40( a1, &v36, &a2, edx0 );goto LABEL_9; }
        if( (edx0->ImgId & 0xF000 ) >> 12 ) { AiUnk40( a1, &v36, &a2, edx0 );goto LABEL_9; }
        if ( !ArtFileExist( ArtMakeId( 1, a1->ImgId & 0xFFF, 16, 0, a1->Orientation + 1 ) ) ){ AiUnk40( a1, &v36, &a2, edx0 );goto LABEL_9; }
    }
    if( CombatUnk02( a1, a2, 2, edx0, &steps ) ) AiUnk40( a1, &v36, &a2, edx0 );
LABEL_9:
    for( i = 0; i < 10 && !(p_State->CombatResult & 0x8081 ); i++ ){
        switch( CombatAttackTest( a1, edx0, v36, 0 ) ){
            case 0:
                v24 = CombatDetermineHitObstacled( a1, edx0, 8, v36 );
                if( steps ){
                    if( AiMoveWalkTo( a1, edx0, steps ) == -1 ) return -1;
                }
                if( v24 < MinToHit ){
                    if( CombatDetermineHit( a1, edx0, 8, v36 ) < MinToHit ){
                        Name = CritterGetName( a1 );
                        eprintf( "%s: FLEEING: Can't possibly Hit Target!", Name );
                        AiMoveRunTo( a1, edx0 );
                        return 0;
                    }
                    if( a3 > 0 ){
                	Trace = AnimMakeTrace(a1, a1->GridId, edx0->GridId, a4, 0, (void *)ObjReach );
                	if( !Trace ){
                	    v44 = a3;
                	} else {
                    	    GridId = a1->GridId;
                    	    if( Trace < a3 ) a3 = Trace;
                    	    for( i = 0; i < a3; i++, v44++ ){
                        	GridId = TileGetTileNumInDir( GridId, a4[ i ], 1 );
                        	if( CombatDetermineHitDist( a1, GridId, edx0, 8, v36 ) >= MinToHit ) return 0;
                    	    }
                    	    if( i == a3 ) v44 = a3;
                	}
                    }
                    if( AiUnk35( a1, edx0, v44, v40 ) == -1 ){
                        eprintf( "%s: FLEEING: Can't possibly get closer to Target!", CritterGetName( a1 ) );
                        AiMoveRunTo( a1, edx0 );
                        return 0;
                    } else {
                        v40 = 0;
                        if( AiUnk42( a1, edx0, v36 ) != -1 && ItemGetAPCost( a1, v36, 0 ) <= a1->Critter.State.CurrentAP ) break;
                        return -1;
                    }
                }
                if( AiUnk42( a1, edx0, v36 ) != -1 && ItemGetAPCost( a1, v36, 0 ) <= a1->Critter.State.CurrentAP ) break;
                return -1;
            case 1:
                if( AiFindAmmo( a1, a2, p_TimeEv ) ){
                    v9 = Item51( a2, *p_TimeEv );
                    if( !v9 && p_TimeEv ) UseUnk06( *p_TimeEv );
                    if( v9 != -1 ){
                        GSoundUnk42( GSoundWeaponFileName( 0, a2, v36, 0 ), GSoundDistance( a1 ) );
                        AiUnk07( a1, a2, 5002 );
                        a1->Critter.State.CurrentAP = ( a1->Critter.State.CurrentAP >= 2 ) ? ( a1->Critter.State.CurrentAP - 2 ) : 0;
                        break;
                    }
                } else {
                    v14 = AiUnk32( a1, 4 );
                    p_TimeEv = &v14;
                    if( *p_TimeEv ){
                        v14 = AiPickUpObj( a1, *p_TimeEv );
                        p_TimeEv = &v14;
                        if( *p_TimeEv ){
                            if( !( v17 = Item51( a2, *p_TimeEv ) ) ) UseUnk06( *p_TimeEv );
                            if( v17 != -1 ){
                                GSoundUnk42( GSoundWeaponFileName( 0, a2, v36, 0 ), GSoundDistance( a1 ) );
                                AiUnk07( a1, a2, 5002 );
                                a1->Critter.State.CurrentAP = ( a1->Critter.State.CurrentAP >= 2 ) ? (a1->Critter.State.CurrentAP - 2) : 0;
                            }
                        }
                    } else {
                        GSoundUnk42( GSoundWeaponFileName( 2, a2, v36, 0 ), GSoundDistance( a1 ) );
                        AiUnk07( a1, a2, 5001 );
                        if( !InvUnwield( a1, 1 ) ) CombatUpdate();
                        AiUnk40( a1, &v36, &a2, edx0 );
                    }
                }
                break;
            case 2:
                if( CombatDetermineHit( a1, edx0, 8, v36 ) < MinToHit ){
                    eprintf( "%s: FLEEING: Can't possibly Hit Target!", CritterGetName( a1 ) );
                    AiMoveRunTo( a1, edx0 );
                    return 0;
                }
                if( a2 ){
                    if( AiUnk35( a1, edx0, a3, v40 ) == -1 ) return -1; 
                    v40 = 0;
                } else if( AiUnk40( a1, &v36, &a2, edx0 ) == -1 || !a2 ){
                    if( AiUnk35( a1, edx0, a1->Critter.State.CurrentAP, v40 ) == -1 ) return -1; 
                    v40 = 0;
                }
                break;
            case 3: case 6: case 7: 
        	if( AiUnk40( a1, &v36, &a2, edx0 ) == -1 ) return -1; 
        	break;
            case 5: 
        	if( AiUnk35( a1, edx0, a1->Critter.State.CurrentAP, v40 ) == -1 ) return -1; 
        	v40 = 0; 
        	break;
        }
    }
    return -1;
}

int AiUnk44( Obj_t *a1, Obj_t *a2 )
{
    if( a2 && FeatGetVal( a1, 4 ) >= 3 && a2->Pid == 79 && LightMapGetLt() < 55705 ) UseUnk13( a1, a2 );
    return 0;
}

void AiUnk45( Obj_t *a1, int a2 )
{
    Obj_t *RhObj, *v13;
    int v9;

    if( !(RhObj = InvGetRHandObj( a1 )) ) return;    
    if( ItemGetAmmo( RhObj ) >= ItemGetPackQuantity( RhObj ) ) return;
    if( !AiFindAmmo( a1, RhObj, &v13 ) ) return;        
    if( !(v9 = Item51( RhObj, v13 )) ) UseUnk06( v13 );
    if( v9 == -1 ) return;
    if( !PartyMembRdy( a1 ) ) return;    
    GSoundUnk42( GSoundWeaponFileName( 0, RhObj, 2, 0 ), GSoundDistance( a1 ) );
    if( a2 ) AiUnk07( a1, RhObj, 5002 );        
}

void AiUnk46( int Count, Obj_t **p )
{
    gAiObjCount = Count;
    if( !Count ) return;
    gAiObjList = (Obj_t **)Malloc( Count * sizeof( Obj_t * ) );
    if( gAiObjList ){
        memcpy( gAiObjList, p, Count * sizeof( Obj_t * ) );
    } else {
        gAiObjCount = 0;
    }    
}

void AiUnk47()
{
    if( gAiObjCount ) Free( gAiObjList );
    gAiObjCount = 0;
}

int AiUnk48( Obj_t *obj, Obj_t *a2 )
{
    int Distance, k, n;

    k = -1;
    if( obj->Critter.State.CurrentAP <= 0 ) return -1;
    Distance = AiGetPacketByObj( obj )->Distance;
    if( a2 && ( a2->Critter.State.CombatResult & 0x80 ) ) a2 = 0;
    if( Distance ){
        if( Distance == 1 ){
            if( a2 ) AiUnk35( obj, a2, obj->Critter.State.CurrentAP, 1 );
        } else if( Distance == 2 && a2 && ObjGetDistance( obj, a2 ) < 10 ){
            AiMoveWalkTo( ( obj == a2->Critter.State.WhoHitMeObj ) ? a2->Critter.State.WhoHitMeObj : obj, a2, 10 );
        }
    } else {
        if( gObjDude != obj->Critter.State.WhoHitMeObj ){
            n = ObjGetDistance( obj, gObjDude );
            if( n > 5 ) AiUnk35( obj, gObjDude, n - 5, 0 );
        }
    }
    k = obj->GridId;
    if( !AiUnk37( obj, a2, &k ) && k != obj->GridId ){
        AnimRegStart( 2 );
        AnimObjMoveToTile( obj, k, obj->Elevation, obj->Critter.State.CurrentAP, 0 );
        if( AnimRegEnd() ) return -1;
        CombatUpdate();
    }
    return 0;
}

int AiUnk49( Ai_t *pck )
{
    if( !pck ) return 0;    
    if( pck->RunAwayMode < -1 || pck->RunAwayMode >= 6 ) return 0;    
    return ( pck->RunAwayMode == -1 ) ? pck->MinHP : gAiConstPercentRank[ pck->RunAwayMode ];
}

Obj_t *AiUnk50( Obj_t *a1, Obj_t *a2 )
{
    ObjCritterCond_t *p_State;
    Ai_t *pck;
    Obj_t *v10;
    Obj_t *WhoHitMe;
    int v25;
    int v20;

    p_State = &a1->Critter.State;
    pck = AiGetPacketByObj( a1 );
    v25 = AiUnk49( pck );
    if( pck->RunAwayMode != -1 ){
        eprintf( "\n%s minHp = %d; curHp = %d", CritterGetName( a1 ), FeatGetVal( a1, 7 ) - (FeatGetVal( a1, 7 ) * v25 / 100), FeatGetVal( a1, 35 ) );
    }
    if( (p_State->Reaction & 0x04) || (pck->HurtTooMuch & p_State->CombatResult) || FeatGetVal( a1, 35 ) < pck->MinHP ){
        eprintf( "%s: FLEEING: I'm Hurt!", CritterGetName( a1 ) );
        AiMoveRunTo( a1, a2 );
    } else {
        if( AiUnk08( a1 ) ){
            eprintf( "%s: FLEEING: I need DRUGS!", CritterGetName( a1 ) );
            AiMoveRunTo( a1, a2 );
        } else {
            if( !a2 ) a2 = AiDangerSource( a1 );
            AiUnk48( a1, a2 );
            if( a2 ) AiUnk43( a1, a2 );
        }
        if( a2 && !(a2->Critter.State.CombatResult & 0x80 ) && a1->Critter.State.CurrentAP && ObjGetDistance( a1, a2 ) > pck->MaxDist ){            
            if( (v10 = CombatUnk09( a1 )) ){
                AiMoveWalkTo( a1, v10, 10 );
                CombatUnk10( a1, 0 );
            } else {
                if( !AiInRange( a1, 2 * FeatGetVal( a1, 1 ), 5 ) ) p_State->Reaction |= 2u;
            }
        }
        if( !a2 && !PartyMembRdy( a1 ) ){
            WhoHitMe = p_State->WhoHitMeObj;
            if( WhoHitMe ){
                if( !( WhoHitMe->Critter.State.CombatResult & 0x80) && p_State->DmgLastTurn > 0 ){
                    if( (v10 = CombatUnk09( a1 ) ) ){
                        AiMoveWalkTo( a1, v10, 10 );
                        CombatUnk10( a1, 0 );
                    } else {
                        eprintf( "%s: FLEEING: Somebody is shooting at me that I can't see!", CritterGetName( a1 ) );
                        AiMoveRunTo(a1, 0);
                    }
                }
            }
        }        
        if( (v10 = CombatUnk09( a1 )) ){
            AiMoveWalkTo( a1, v10, 10 );
            if( ObjGetDistance( a1, v10 ) >= 10 ) CombatUnk10( a1, 0 );
        }
        if( a1->Critter.State.GroupId ){
            v10 = AiUnk19( a1, a1, 1 );
            v20 = 5;
        } else {
            v10 = gObjDude;
            if( PartyMembRdy( a1 ) ) v20 = gAiUnk15[ AiGetPacketByObj( a1 )->Distance ];            
        }
        if( !a2 ){
            if( v10 ){
                if( ObjGetDistance( a1, v10 ) > v20 ){
                    AiUnk35( a1, v10, ObjGetDistance( a1, v10 ) - v20, 0 );
                    return 0;
                }
            }
        }
        if( a1->Critter.State.CurrentAP > 0 ){
            eprintf( "\n>>>NOTE: %s had extra AP's to use!<<<", CritterGetName( a1 ) );
            AiUnk48( a1, a2 );
        }
    }
    return a2;
}

int AiUnk51( Obj_t *obj )
{
    InpWinUpdate();
    if( obj->Flags & 0x01 ) return 0;
    if( obj->Elevation != gObjDude->Elevation ) return 0;
    if( obj->Critter.State.CombatResult & 0x81 ) return 0;
    if( obj->Critter.State.DmgLastTurn > 0 ) return 1;
    if( obj->ScrId != -1 ){
        ScptSetup( obj->ScrId, NULL, NULL );
        ScptSetArg( obj->ScrId, 5 );
        ScptRun( obj->ScrId, SCPT_AEV_COMBAT_P_PROC );
    }
    if( ( obj->Critter.State.Reaction & 0x01 ) != 0 ) return 1;
    if( ( obj->Critter.State.Reaction & 0x02 ) != 0 ) return 0;
    if( ( obj->Critter.State.Reaction & 0x04 ) != 0 )return 0;
    if( AiDangerSource( obj ) ) return 1;
    return 0;
}

int AiUnk52( Obj_t *obj )
{
    Obj_t *dang;

    InpWinUpdate();
    if( obj->Grid.DestMapId & 0x02 ) return 1;
    if( obj->Critter.State.CombatResult & 0x81 ) return 1;
    if( obj->Grid.DestMapId & 0x04 ) return 1;
    dang = AiDangerSource( obj );
    return !dang || !AiObjCanHearObj( obj, dang );
}

int AiUnk53( Obj_t *obj, int Group )
{
    Obj_t *WhoHitMe;
    int GroupId, cf;
    VidRect_t Area;

    if( OBJTYPE( obj->Pid ) != TYPE_CRIT ) return 0;    
    WhoHitMe = obj->Critter.State.WhoHitMeObj;
    obj->Critter.State.GroupId = Group;
    if( WhoHitMe == (void *)-1 ){
        CritterUnk45( obj, 0 );
        eprintf( "\nError: CombatData found with invalid who_hit_me!" );
        return -1;
    }
    if( WhoHitMe ){
        GroupId = WhoHitMe->Critter.State.GroupId;
        if( Group == GroupId ) CritterUnk45( obj, 0 );
    }
    CombatStopAttack( obj, 0 );
    if( IN_COMBAT ){
        cf = obj->OutlineColor && obj->OutlineColor >= 0;
        ObjClrOutline( obj, NULL );
        ObjSetOutline( obj, ( obj->Critter.State.GroupId == gObjDude->Critter.State.GroupId ) ? 8 : 1, 0 );
        if( cf ){
            ObjUnk34( obj, &Area );
            TileUpdateArea( &Area, obj->Elevation );
        }
    }    
    return 0;
}

int AiUnk54( Obj_t *obj, int PackNb )
{
    Proto_t *proto;

    if( OBJTYPE( obj->Pid ) != TYPE_CRIT ) return -1;
    obj->Critter.State.AIpackNb = PackNb;
    if( PartyUnk10() ){
        if( ProtoGetObj( obj->Pid, &proto ) == -1 ) return -1;
        proto->AiPacketNum = PackNb;
    }
    return 0;
}

int AiCombatTaunts( Obj_t *Critter, Combat_t *Combat, int ReactionType, int a4 )
{
    int RunStart, RunEnd, Taunts;
    Ai_t *pck;
    MsgLine_t fmt;
    char *s;    

    Taunts = 1;
    if( OBJTYPE( Critter->ImgId ) != TYPE_CRIT ) return -1;
    CfgGetInteger( &gConfiguration, "preferences", "combat_taunts", &Taunts );
    if( !Taunts ) return -1; // combat_taunts = 0, exit if taunts disabled    
    if( Critter == gObjDude ) return -1; // critter is player 
    if( Critter->Critter.State.CombatResult & 0x81 ) return -1; // player is blind or dead ?
    
    pck = AiGetPacketByObj( Critter );
    eprintf( "%s is using %s packet with a %d%% chance to taunt\n", ObjGetName( Critter ), pck->Name, pck->Chance );
    if( RandMinMax( 1, 100 ) > pck->Chance ) return -1; // roll failed

    switch( ReactionType ){
        case 0: RunStart = pck->RunStart; RunEnd = pck->RunEnd; s = gAiUnk05; break;
        case 1: RunStart = pck->MoveStart; RunEnd = pck->MoveEnd; s = gAiUnk05; break;
        case 2: RunStart = pck->AttackStart; RunEnd = pck->AttackEnd; s = gAiUnk05; break;
        case 3: RunStart = pck->MissStart; RunEnd = pck->MissEnd; s = gAiUnk06; break;
        default:
            RunStart = pck->HitTable[ Combat->i11 ].Start;
            RunEnd = pck->HitTable[ Combat->i11 ].End;
            s = gAiUnk06;
            break;
    }
    if( RunEnd < RunStart ) return -1;
    fmt.Id = RandMinMax( RunStart, RunEnd );
    if( MessageGetMsg( &gCombatAiMessages, &fmt ) != 1 ){
        eprintf( "\nERROR: combatai_msg: Couldn't find message # %d for %s", fmt.Id, CritterGetName( Critter ) );
        return -1;
    }
    eprintf( "%s said message %d\n", ObjGetName( Critter ), fmt.Id );
    strncpy( s, fmt.Text, 259 );
    return AnimSetCallback11( Critter, (AnimU_t)ReactionType, (void *)AiMumble, a4 ); // !!! scalar to pointer !!!
}

int AiMumble( Obj_t *obj, char *Text )
{
    Ai_t *p;
    VidRect_t Area;

    if( TextCount() > 0 ) return 0;    
    p = AiGetPacketByObj( obj );
    if( !TextFloatMessage( obj, Text, p->Font, p->Color, p->OutlineColor, &Area ) ) TileUpdateArea( &Area, obj->Elevation );
    return 0;
}

Obj_t *AiDrawOponent( Combat_t *cmd )
{
    int cnt, i;

    ItemGetRange( cmd->Dude, cmd->Hand );
    if( !gAiObjCount ) return 0;
    cnt = RandMinMax( 0, gAiObjCount - 1 );
    i = cnt;
    do{
        if( gAiObjList[ i ] != cmd->Dude && gAiObjList[ i ] != cmd->Comp ){
	    if( ActionTurnAt( cmd->Dude, gAiObjList[ i ] ) && !CombatAttackTest( cmd->Dude, gAiObjList[ i ], cmd->Hand, 0 ) ) return gAiObjList[ i ];    
        }
        if( ++i == gAiObjCount ) i = 0;
    }while( i != cnt );
    return 0;
}

int AiGetMaxDamagePower( Obj_t *obj )
{
    int dmg, max, min;
    Obj_t *RHandObj, *LHandObj;

    if( !obj ) return 0;    
    if( OBJTYPE( obj->ImgId ) != TYPE_CRIT ) return 0;
    if( obj->Critter.State.CombatResult & ( CRITT_EYE_DMG | CRITT_SNEAK ) ) return 0;    
    dmg = FeatGetVal( obj, FEAT_MELEE );
    RHandObj = InvGetRHandObj( obj );
    if( RHandObj && ItemGetObjType( RHandObj ) == PR_ITEM_WEAPON && ItemGetWeaponDmg( RHandObj, &min, &max ) != -1 && dmg < max ) dmg = max;
    LHandObj = InvGetLHandObj( obj );
    if( LHandObj && ItemGetObjType( LHandObj ) == PR_ITEM_WEAPON && ItemGetWeaponDmg( LHandObj, &min, &max ) != -1 && dmg < max ) dmg = max;
    return dmg + FeatGetVal( obj, FEAT_AC );
}

int AiUnk59( Obj_t *obj, Obj_t *a2 )
{
    int AttackerDmg, ObjDmg;
    
    if( obj->Critter.State.WhoHitMe ){
        ObjDmg = AiGetMaxDamagePower( a2 );
        AttackerDmg = AiGetMaxDamagePower( obj->Critter.State.WhoHitMeObj );
        if( ObjDmg <= AttackerDmg ) return AttackerDmg;
    }
    return CritterUnk45( obj, a2 );
}

int AiObjCanHearObj( Obj_t *Rogue, Obj_t *Target )
{
    int Distance, Perception, SneakSkill, Check;

    if( !Target ) return 0;
    Distance = ObjGetDistance( Target, Rogue );
    Perception = FeatGetVal( Rogue, FEAT_PERCEPTION );
    SneakSkill = SkillGetTotal( Target, SKILL_SNEAK );
    if( ActionTurnAt( Rogue, Target ) ){
        Check = 5 * Perception;
        if( Target->Flags & PRFLG_TR_GLASS ) Check /= 2;
        if( Target == gObjDude ){
            if( CritterUnk42() ){
                Check = Check / 4;
                if( SneakSkill > 120 ) Check--;
            } else if( CritterUsingSkill(0) ){
                Check = 2 * Check / 3;
            }
        } 
        if( Distance <= Check ) return 1;
    }
    Check = IN_COMBAT ? 2 * Perception : Perception;
    if( Target == gObjDude ){
        if( CritterUnk42() ){
            Check = Check / 4;
            if( SneakSkill > 120 ) Check--;
        } else if( CritterUsingSkill( 0 ) ){
            Check = 2 * Check / 3;
        }
    }
    return Distance <= Check;
}

int AiLoadCombatMsg()
{
    char stmp[260];
    int filt = 0;

    if( MessageInit( &gCombatAiMessages ) != 1 ) return -1;
    sprintf( stmp, "%s%s", gGamePath, "combatai.msg" );
    if( MessageLoad( &gCombatAiMessages, stmp ) != 1 ) return -1;
    CfgGetInteger( &gConfiguration, "preferences", "language_filter", &filt );
    if( filt == 1 ) MessageLangFilter( &gCombatAiMessages );
    return 0;
}

int AiUnk61()
{
    return ( MessageClose( &gCombatAiMessages ) == 1 ) - 1;
}

void AiUnk62()
{
    int tmp;

    tmp = 0;
    CfgGetInteger( &gConfiguration, "preferences", "language_filter", &tmp );
    if( tmp == gLangFilter ) return;    
    gLangFilter = tmp;
    if( tmp == 1 ){
        MessageLangFilter( &gCombatAiMessages );
    } else if( MessageClose( &gCombatAiMessages ) == 1 ){
        AiLoadCombatMsg();
    }    
}

void AiUnk63( Obj_t *obj )
{
    Obj_t *p;
    int i;

    for( i = 0; i < gAiObjCount; i++ ){
        p = gAiObjList[ i ];
        if( p->Critter.State.Reaction & 0x01 ) continue;
        if( !AiObjCanHearObj( gAiObjList[ i ], obj ) ) continue;        
        p->Critter.State.Reaction |= 0x01;
        if( ( obj->Critter.State.CombatResult & 0x80 ) && !AiObjCanHearObj( p, obj->Critter.State.WhoHitMeObj ) ){
            eprintf( "\nSomebody Died and I don't know why!  Run!!!" );
            CombatUnk10( p, obj );
        }                
    }
}

void AiUnk64( Obj_t *obj )
{
    int i;
    Obj_t *p;

    for( i = 0; i < gAiObjCount; i++ ){
        p = gAiObjList[ i ];
        if( !(p->Critter.State.Reaction & 0x01) && obj->Critter.State.GroupId == p->Critter.State.GroupId ){
            if( AiObjCanHearObj( gAiObjList[ i ], obj ) ) p->Critter.State.Reaction |= 0x01;
        }
    }
}

void AiUnk65( Obj_t *obj )
{
    Obj_t *tmp;
    int i;

    for( i = 0; i < gAiObjCount; i++ ){
        if( obj == gAiObjList[ i ] ){
    	    gAiObjCount--;
    	    tmp = gAiObjList[ i ];
    	    gAiObjList[ i ] = gAiObjList[ gAiObjCount ];
    	    gAiObjList[ gAiObjCount ] = tmp;
    	    break;
        }
    }
}

 