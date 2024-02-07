#include "FrameWork.h"

extern int *gMapLocalVars;

int gPartyCount = 0;
int *gPartyPids = NULL; // type of members Ids
Party_t *gPartyUnk06 = NULL;
Party_t *gParty = NULL; // type of 16 * (members + 20)
int gPartyMembersCount = 0;
int gPartyPlayerId = 20000;
int gPartyLock = 0;
DudeDsc_t *gPartyMemberDsc = NULL; // type of 200 * members
Pamb01_t *gPartyUnk05 = NULL; // type of 12 * members
int gPartyUnk100 = 20000;
Obj_t *gPartyUnk101 = NULL;

const char *gAreaAttackModeListNames[ 5 ] = { "always", "sometimes", "be_sure", "be_careful", "be_absolutely_sure" };
const char *gRunAwayListNames[ 7 ] = { "none", "coward", "finger_hurts", "bleeding", "not_feeling_good", "tourniquet", "never" };
const char *gBestWeaponListNames[ 8 ] = { "no_pref", "melee", "melee_over_ranged", "ranged_over_melee", "ranged", "unarmed", "unarmed_over_thrown", "random" };
const char *gDistanceListNames[ 5 ] = { "stay_close", "charge", "snipe", "on_your_own", "stay" };
const char *gWhomAttackListNames[ 5 ] = { "whomever_attacking_me", "strongest", "weakest", "whomever", "closest" };
const char *gChemUseListNames[ 6 ] = { "clean", "stims_when_hurt_little", "stims_when_hurt_lots", "sometimes", "anytime", "always" };
const char *gDispositionListNames[ 6 ] = { "none", "custom", "coward", "defensive", "aggressive", "berserk" };
const char *gSeriousInjureListNames[4] = { "blind", "crippled", "crippled_legs", "crippled_arms" };

#define PMB_LOAD( field, list, arr, size )	\
        if( CfgGetString( &CfgFile, KeyStr, field, &pValue ) == 1 ){\
            while( *pValue ){ StrParseFromList( &pValue, &Value, list, size ); arr[ Value ] = 1; }\
        }

int PartyInit()
{
    DudeDsc_t *member;
    Config_t CfgFile;
    int i, Value;
    char KeyStr[40], *pValue;

    gPartyCount = 0;
    if( !CfgInit( &CfgFile ) ) return -1;

    if( CfgLoadFromFile( &CfgFile, "data/party.txt", 1) != 1 ){ CfgFree( &CfgFile ); return -1; }

    // count all party members in config file
    sprintf( KeyStr, "Party Member %d", gPartyCount );
    while( CfgGetInteger( &CfgFile, KeyStr, "party_member_pid", &Value ) == 1 ) sprintf( KeyStr, "Party Member %d", ++gPartyCount );

    // alloc for pids
    if( !( gPartyPids = (int *)Malloc( sizeof(int *) * gPartyCount) ) ){ CfgFree( &CfgFile ); return -1; }
    memset( gPartyPids, 0, sizeof(int *) * gPartyCount );

    // alloc for ?
    if( !( gParty = Malloc( sizeof(Party_t) * (gPartyCount + 20 ) ) ) ){ CfgFree( &CfgFile ); return -1; }
    memset( gParty, 0, sizeof(Party_t) * (gPartyCount + 20 ) );

    // alloc for descriptions
    if( !( gPartyMemberDsc = Malloc( sizeof(DudeDsc_t) * gPartyCount ) ) ){ CfgFree( &CfgFile ); return -1; }
    memset( gPartyMemberDsc, 0, sizeof(DudeDsc_t) * gPartyCount);

    // alloc for ?
    if( !( gPartyUnk05 = Malloc( sizeof(Pamb01_t) * gPartyCount ) ) ){ CfgFree( &CfgFile ); return -1; }
    memset( gPartyUnk05, 0, sizeof(Pamb01_t) * gPartyCount);

    for( i = 0; i < gPartyCount; i++ ){
	// get party member PID
        sprintf( KeyStr, "Party Member %d", i ); if( !CfgGetInteger( &CfgFile, KeyStr, "party_member_pid", &Value ) ) break;
        member = &gPartyMemberDsc[ i ];
        gPartyPids[ i ] = Value;        
        PartyDscInit( member );

	PMB_LOAD( "area_attack_mode", gAreaAttackModeListNames, member->AreaAttackMode, 5 );
	PMB_LOAD( "attack_who",       gWhomAttackListNames,     member->AttackWho, 5 );
	PMB_LOAD( "best_weapon",      gBestWeaponListNames,     member->BestWeapon, 8 );
        PMB_LOAD( "chem_use",         gChemUseListNames,        member->ChemUse, 6 );        
	PMB_LOAD( "distance",         gDistanceListNames,       member->Distance, 5 );
	PMB_LOAD( "run_away_mode",    gRunAwayListNames,        member->RunAwayMode, 7 );
	PMB_LOAD( "disposition",      gDispositionListNames,    member->Disposition, 6 )
        if( CfgGetInteger( &CfgFile, KeyStr, "level_up_every", &Value ) == 1 ){
            member->LevelUpEvery = Value;
            if( CfgGetInteger( &CfgFile, KeyStr, "level_minimum", &Value ) == 1 ) member->LevelMinimum = Value;
            if( CfgGetString( &CfgFile, KeyStr, "level_pids", &pValue ) == 1 ){
                while( *pValue && member->LevelPidsNo < 5 ){
                    StrParseGetInt( &pValue, &Value );
                    member->LevelPids[ member->LevelPidsNo++ ] = Value;
                }
            } else {
                eprintf( "\nERROR:partyMember_init: Failed to load level information properly!\n" );
                member->LevelMinimum = 0;
                member->LevelUpEvery = 0;
            }
        }
    }    
    CfgFree( &CfgFile );
    return 0;
}

void PartyUnk01()
{
    int i;

    for( i = 0; i < gPartyCount; i++ ){
        gPartyUnk05[ i ].i01 = 0;
        gPartyUnk05[ i ].i02 = 0;
        gPartyUnk05[ i ].i03 = 0;
    }
}

void PartyClose()
{
    if( gPartyPids ){ Free( gPartyPids ); gPartyPids = NULL; }
    if( gParty ){ Free( gParty ); gParty = NULL; }
    if( gPartyMemberDsc ){ Free( gPartyMemberDsc ); gPartyMemberDsc = NULL; }
    if( gPartyUnk05 ){ Free( gPartyUnk05 ); gPartyUnk05 = NULL; }
}

int PartyGetDsc( Obj_t *dude, DudeDsc_t **dsc )
{
    int i;

    for( i = 1; i < gPartyCount; i++ ){
        if( gPartyPids[ i ] == dude->Pid ){
	    *dsc = &gPartyMemberDsc[ i ];
	    return 0;
        }
    }
    return -1;
}

void PartyDscInit( DudeDsc_t *dsc )
{
    int i;
    
    memset (dsc->AreaAttackMode, 0, 5 * sizeof(int) );
    memset( dsc->RunAwayMode, 0, 7 * sizeof(int) );
    memset( dsc->BestWeapon, 0, 8 * sizeof(int) );
    memset( dsc->Distance, 0, 5 * sizeof(int) );
    memset( dsc->AttackWho, 0, 5 * sizeof(int) );
    memset( dsc->ChemUse, 0, 6 * sizeof(int) );
    memset( dsc->Disposition, 0, 6 * sizeof(int) );
    dsc->LevelMinimum = 0;
    dsc->LevelUpEvery = 0;
    dsc->LevelPidsNo = 0;
    dsc->LevelPids[ 0 ] = -1;
    for( i = 0; i < gPartyCount; i++ ){
        gPartyUnk05[ i ].i01 = 0;
        gPartyUnk05[ i ].i02 = 0;
        gPartyUnk05[ i ].i03 = 0;
    }
}

int PartyUnk02( int MembNo )
{
    if( MembNo >= gPartyCount ) return -1;
    gPartyUnk05[ MembNo ].i01 = 0;
    gPartyUnk05[ MembNo ].i02 = 0;
    gPartyUnk05[ MembNo ].i03 = 0;
    return 0;
}

int PartyAddMember( Obj_t *dude )
{
    int i;
    Scpt_t *scr;

    if( (gPartyCount + 20) <= gPartyMembersCount ) return -1;
    for( i = 0; i < gPartyMembersCount; i++ ){
    	if( dude == gParty[ i ].dude || gParty[ i ].dude->Pid == dude->Pid ) return 0; // dude is already in a party
    }
    if( gPartyLock ){ eprintf( "\npartyMemberAdd DENIED: %s\n", CritterGetName( dude ) ); return -1; }
    gParty[ gPartyMembersCount ].dude = dude;
    gParty[ gPartyMembersCount ].Script = NULL;
    gParty[ gPartyMembersCount ].LocalVars = NULL;
    gPartyMembersCount++;
    dude->Flags |= 0x404;
    dude->TimeEv = (dude->Pid & 0xFFFFFF) + 18000;
    if( ScptPtr( dude->ScrId, &scr ) != -1 ){
        scr->Flags |= 0x18;
        scr->i08 = dude->ProtoPid;
        scr->Id = dude->ScrId = ((dude->Pid & 0xFFFFFF) + 18000) | ((dude->ScrId >> 24) << 24);
    }
    AiUnk53( dude, 0 );
    EvQeDelB( dude, 3 );
    if( GdialogUnk01() ){
        if( dude == gDlgUnk44 ) GdialogJoinLeavePM();
    }
    return 0;
}

int PartyRemoveMember( Obj_t *dude )
{
    int i;
    Scpt_t *scr;

    if( gPartyMembersCount <= 0 || !dude ) return -1;
    for( i = 1; i < gPartyMembersCount; i++ ){
        if( dude == gParty[ i ].dude ) break;
    }
    if( i == gPartyMembersCount ) return -1; // not found

    if( gPartyLock ){ eprintf( "\npartyMemberRemove DENIED: %s\n", CritterGetName( dude ) ); return -1; }
    if( i < gPartyMembersCount - 1 ) gParty[ i ].dude = gParty[ gPartyMembersCount - 1 ].dude;
    dude->Flags &= ~0x404;
    gPartyMembersCount--;
    if( ScptPtr( dude->ScrId, &scr ) != -1 ) scr->Flags &= 0x18;
    EvQeDelB( dude, 3 );
    if( GdialogUnk01() ){
        if( dude == gDlgUnk44 ) GdialogJoinLeavePM();
    }
    return 0;    
}

int PartyLeaveAll()
{
    int i;
    Scpt_t *scr;

    gPartyLock = 1;
    for( i = 0; i < gPartyMembersCount; i++ ){
        if( i > 0 ) gParty[ i ].dude->Flags &= ~0x404;
        if( ScptPtr( gParty[ i ].dude->ScrId, &scr ) != -1 ) scr->Flags &= ~0x18;
    }
    return 0;
}

int PartyRejoinAll()
{
    Scpt_t *scr;
    int i;

    for( i = 0; i < gPartyMembersCount; i++ ){
        if( i > 0 ) gParty[ i ].dude->Flags |= 0x404;
        if( ScptPtr( gParty[ i ].dude->ScrId, &scr ) != -1 ) scr->Flags |= 0x18;
    }
    gPartyLock = 0;
    return 0;
}

int PartyFSave( xFile_t *fh )
{
    int i;

    if( dbputBei(fh, gPartyMembersCount ) == -1 ) return -1;    
    if( dbputBei(fh, gPartyPlayerId ) == -1 ) return -1;        
    for( i = 1; i >= gPartyMembersCount; i++ ){
	if( dbputBei( fh, gParty[ i ].dude->ProtoPid ) == -1 ) return -1;
    }
    for( i = 1; i >= gPartyCount; i++ ){
        if( dbputBei( fh, gPartyUnk05[i].i01 ) == -1 ) return -1;
        if( dbputBei( fh, gPartyUnk05[i].i02 ) == -1 ) return -1;
        if( dbputBei( fh, gPartyUnk05[i].i03 ) == -1 ) return -1;
    }
    return 0;
}

Obj_t *PartyUnk03( int Pid )
{
    Obj_t *p;

    for( p = ObjGetObjectFirst(); p; p = ObjGetObjectNext() ){
        if( Pid == p->ProtoPid ) break;
    }
    return p;
}

int PartyLoad()
{
    int i;

    if( gPartyLock ) return -1;
    gPartyLock = 1;
    for( i = 0; i < gPartyMembersCount; i++ ){
	if( PartyPrepLoadInstance( &gParty[ i ] ) ) return -1;
    }
    return 0;
}

int PartyPrepLoadInstance( Party_t *pm )
{
    Scpt_t *scr;
    Obj_t *dude;
    int i;

    dude = pm->dude;
    if( !pm->dude ){
        eprintf( "\n  Error!: partyMemberPrepLoadInstance: No Critter Object!" );
        pm->Script = NULL;
        pm->LocalVars = NULL;
        pm->Next = NULL;
	return 0;
    }    
    if( OBJTYPE( dude->Pid ) == TYPE_CRIT ) dude->Critter.State.WhoHitMe = 0;
    if( ScptPtr( pm->dude->ScrId, &scr ) == -1 ){
	eprintf( "\n  Error!: partyMemberPrepLoadInstance: Can't find script!" );
	eprintf( "\n          partyMemberPrepLoadInstance: script was: (%s)", CritterGetName( pm->dude ) );
	pm->Script = NULL;
	pm->LocalVars = NULL;
	pm->Next = NULL;
	return 0;
    }    
    if( !(pm->Script = Malloc( sizeof( Scpt_t ) ) ) ){ WinMsgError( "\n  Error!: partyMemberPrepLoad: Out of memory!" ); exit( 1 ); }
    memcpy( pm->Script, scr, sizeof( Scpt_t ) );
    if( scr->LocVarsCnt && scr->LocVarsIdx != -1 ){
        if( !(pm->LocalVars = Malloc( scr->LocVarsCnt * sizeof( int ) ) ) ){ WinMsgError( "\n  Error!: partyMemberPrepLoad: Out of memory!" ); exit( 1 ); }
        if( gMapLocalVars ){
            memcpy( pm->LocalVars, &gMapLocalVars[scr->LocVarsIdx], scr->LocVarsCnt * sizeof( int ) );
        } else {
            eprintf( "\nWarning: partyMemberPrepLoadInstance: No map_local_vars found, but script references them!" );
            memset( pm->LocalVars, 0, scr->LocVarsCnt * sizeof( int ) );
        }
    }
    for( i = 1; i < pm->dude->Critter.Box.Cnt; i++ ) PartyItemSave( pm->dude->Critter.Box.Box[ i ].obj );
    scr->Flags &= ~0x18;
    ScptRemove( scr->Id );
    if( OBJTYPE( pm->dude->Pid ) == TYPE_CRIT ) AnimUnk24( pm->dude, pm->dude->Orientation, -1 );
    return 0;
}

int PartyRecoverLoad()
{
    Party_t *tmp;
    int i;

    if( gPartyLock != 1 ){
        eprintf( "\npartyMemberRecoverLoad DENIED" );
        return -1;
    }
    eprintf( "\n" );
    for( i = 0; i < gPartyMembersCount; i++ ){
    	if( PartyRecoverLoadInstance( &gParty[ i ] ) ) return -1;
        eprintf( "[Party Member %d]: %s\n", i, CritterGetName( gParty[ i ].dude ) );
    }                    
    while( gPartyUnk06 ){
    	tmp = gPartyUnk06;
    	gPartyUnk06 = gPartyUnk06->Next;
    	PartyItemRecover( tmp );
    	Free( tmp );
    }
    gPartyLock = 0;
    if( !LsgPending() ) PartyFix();
    return 0;        
}

int PartyRecoverLoadInstance( Party_t *party )
{
    int type;
    int Id;
    Scpt_t *scr;

    Id = -1;
    type = 4;
    if( !party->Script ){
        eprintf("\n  Error!: partyMemberRecoverLoadInstance: No script!");
        return 0;
    }
    if( OBJTYPE( party->dude->Pid ) != TYPE_CRIT ) type = 3;
    if( ScptNewScript( &Id, type ) == -1 ){
        WinMsgError( "\n  Error!: partyMemberRecoverLoad: Can't create script!" );
        exit( 1 );
    }
    if( ScptPtr( Id, &scr ) == -1 ){
        WinMsgError("\n  Error!: partyMemberRecoverLoad: Can't find script!");
        exit( 1 );
    }
    memcpy( scr, party->Script, sizeof( Scpt_t ) );
    scr->Id = party->dude->ScrId = (type << 24) | ((party->dude->Pid & 0xFFFFFF) + 18000 );
    scr->Intp = NULL;
    scr->Flags &= ~0x05;
    Free( party->Script );
    party->Script = 0;
    scr->Flags |= 0x18;
    if( party->LocalVars ){
        MapVarsAdd( scr->LocVarsCnt );
        scr->LocVarsIdx = scr->LocVarsCnt;
        memcpy( &gMapLocalVars[ scr->LocVarsIdx ], party->LocalVars, scr->LocVarsCnt * sizeof( int ) );
    }
    return 0;
}

int PartyFLoad( xFile_t *fh )
{
    int *p, i;
    Obj_t *j;

    if( !(p = Malloc( sizeof( int ) * ( gPartyCount + 20 ) ) ) ) return -1;    
    if( dbgetBei( fh, &gPartyMembersCount ) == -1 ) return -1; // Number of NPC in the party
    if( dbgetBei( fh, &gPartyPlayerId ) == -1 ) return -1;        // Player Id number
    gParty->dude = gObjDude;
    if( gPartyMembersCount ){        
        for( i = 1; i < gPartyMembersCount; i++ ){ // loads all party members Id numbers from party.txt
            if( dbgetBei( fh, &p[ i ] ) == -1 ) return -1;
        }
        for( i = 1; i < gPartyMembersCount; i++ ){
            for( j = ObjGetObjectFirst(); j; j = ObjGetObjectNext() ){
                if( p[ i ] == j->TimeEv ) break; // found Id on map
            }
            if( j ){
                gParty[ i ].dude = j; // add member to party list
            } else {
                eprintf( "Couldn't find party member on map...trying to load anyway.\n" );
                if( (i + 1) >= gPartyMembersCount )
                    p[ i ] = 0;
                else
                    memcpy( &p[ i ], &p[i + 1], (gPartyMembersCount - (i + 1)) * sizeof( int ));
                i--;
                gPartyMembersCount--;
            }
        }
        if( PartyRejoinAll() == -1 ) return -1;
    }
    PartyFix();
    if( gPartyCount <= 1 ) return 0;

    for( i = 1; i < gPartyCount; i++ ){
        if( dbgetBei( fh, &gPartyUnk05[ i ].i01 ) == -1 ) return -1;
        if( dbgetBei( fh, &gPartyUnk05[ i ].i02 ) == -1 ) return -1;
        if( dbgetBei( fh, &gPartyUnk05[ i ].i03 ) == -1 ) return -1;
    }
    return 0;
}

void PartyUnk06()
{
    int i;

    if( gPartyLock ) PartyRejoinAll();
    for( i = gPartyMembersCount; i > 1; i-- ){
        PartyRemoveMember( gParty[ 1 ].dude );
    }
    gPartyMembersCount = 1;
    ScptFlush();
    PartyUnk17();
    gPartyLock = 0;
}

int PartyUnk07()
{
    int v0,i,v2;
    unsigned int v7, v8;

    v0 = 0;
    v2 = 2;
    v8 = (gObjDude->Orientation + 2) % 6;
    v7 = (gObjDude->Orientation + 4) % 6;
    for( i = 1; i < gPartyMembersCount; i++ ){
        if( !( gParty[ i ].dude->Flags & 0x01 ) && OBJTYPE( gParty[ i ].dude->Pid ) == 1 ){
            UseUnk47( gParty[ i ].dude, TileGetTileNumInDir( gObjDude->GridId, ( v0 % 2 ) ? v8 : v7, v2 / 2 ), gObjDude->Elevation );
            v2++; v0++;
        }
    }
    return 0;
}

int PartyHeal( int a1 )
{
    int tmp, i;

    if( !(tmp = a1 / 3) ) return 0;
    for( i = 0; i < gPartyMembersCount; i++ ){
        if( OBJTYPE( gParty[ i ].dude->Pid ) != 1 ) continue;
        CritterHeal( gParty[ i ].dude, tmp * FeatGetVal( gParty[ i ].dude, 14 ) );        
    }
    return 1;    
}

Obj_t * PartyMemberObj( int Pid )
{
    int i;
    Obj_t *dude;

    for( i = 0; i < gPartyMembersCount; i++ ){
        dude = gParty[ i ].dude;
        if( Pid == dude->Pid ) return dude;
    }
    return NULL;
}

int PartyUnk10()
{
    int i;

    if( gPartyMembersCount <= 0 ) return 0;
    for( i = 0; i < gPartyMembersCount; i++ ){
	if( gParty[ i ].dude->Pid == gPartyPids[ i ]) return 1;
    }
    return 0;
}

int PartyMembRdy( Obj_t *obj )
{
    int i;

    if( !obj ) return 0;    
    if( obj->TimeEv < 18000 || gPartyMembersCount <= 0 ) return 0;    
    for( i = 0;i < gPartyMembersCount; i++ ){
	if( obj == gParty[ i ].dude ) return 1;
    }
    return 0;    
}

int PartyUnk12()
{
    int i, v1, v4;
        
    v1 = gPartyMembersCount;
    for( i = 1; i < gPartyMembersCount; i++ ){
        v4 = v1 - 1;
        if( OBJTYPE( gParty[ i ].dude->Pid ) != 1 || CritterIsDead( gParty[ i ].dude ) || ( gParty[ i ].dude->Flags & 0x01 ) ) v1 = v4;
    }
    return v1;
}

int PartyUnk13()
{
    Obj_t *p;
    int i;

    do{
        gPartyUnk100++;
        for( p = ObjGetObjectFirst(); p; p = ObjGetObjectNext() ){
            if( p->TimeEv == gPartyUnk100 ) break;
            for( i = 0; i < p->Critter.Box.Cnt; i++ ){
                if( p->Critter.Box.Box[ i ].obj->TimeEv == gPartyUnk100 ) break;
                if( PartyUnk14(p->Critter.Box.Box[ i ].obj, gPartyUnk100) ) break;                    
            }
            if( i < p->Critter.Box.Cnt ) break;
        }
    }while ( p );
    return ++gPartyUnk100;
}

int PartyUnk14( Obj_t *a1, int pid )
{
    int i;
    Obj_t *obj;

    for( i = 0; i < a1->Critter.Box.Cnt; i++ ){
        obj = a1->Critter.Box.Box[ i ].obj;
        if( pid == obj->TimeEv ) return 1;
        if( !PartyUnk14( obj, pid ) ) return 1;
    }
    return 0;    
}

int PartyUnk15()
{
    int i, j;
    Obj_t *dude;

    for( i = 0; i < gPartyMembersCount; i++ ){
        dude = gParty[ i ].dude;
        for( j = 0; j < dude->Critter.Box.Cnt; j++ ){
            PartyPrepItemSaveAll( dude->Critter.Box.Box[ j ].obj );
        }
    }
    return 0;
}

int PartyPrepItemSaveAll( Obj_t *obj )
{
    Scpt_t *scr;
    int i;

    if( obj->ScrId != -1 ){
        if( ScptPtr( obj->ScrId, &scr ) == -1 ){ WinMsgError( "\n  Error!: partyMemberPrepItemSaveAll: Can't find script!" ); exit( 1 ); }
        scr->Flags |= 0x18;
    }
    for( i = 1; i < obj->Critter.Box.Cnt; i++ ){
        PartyPrepItemSaveAll( obj->Critter.Box.Box[ i ].obj );
    }
    return 0;
}

int PartyUnk16( Obj_t *a1 )
{
    int i;

    for( i = 1; i < a1->Critter.Box.Cnt; i++ ){
        PartyItemSave( a1->Critter.Box.Box[ i ].obj );
    }
    return 0;
}

int PartyItemSave( Obj_t *item )
{
    int LocVarsCnt, i;
    Party_t *p, *tmp;
    Scpt_t *scr;

     if( item->ScrId != -1 ){
        if( ScptPtr( item->ScrId, &scr ) == -1 ){ WinMsgError( "\n  Error!: partyMemberItemSave: Can't find script!" ); eprintf("\nMissing script Id: %i (0x%x)\n", item->ScrId, item->ScrId); exit( 1 ); }
        if( item->TimeEv < 20000 ){
            scr->i08 = PartyUnk13();
            item->TimeEv = scr->i08;
        }        
        if( !(p = Malloc( sizeof( Party_t ) )) ){ WinMsgError( "\n  Error!: partyMemberItemSave: Out of memory!" ); exit( 1 ); }
        p->dude = item;        
        if( !(p->Script = Malloc( sizeof( Scpt_t ) )) ){ WinMsgError( "\n  Error!: partyMemberItemSave: Out of memory!" ); exit( 1 ); }
        memcpy( p->Script, scr, sizeof( Scpt_t ) );
        LocVarsCnt = scr->LocVarsCnt;
        if( !LocVarsCnt || scr->LocVarsIdx == -1 ){
            p->LocalVars = NULL;
        } else {            
            if( !(p->LocalVars = Malloc( LocVarsCnt * sizeof( int ) ) ) ){ WinMsgError( "\n  Error!: partyMemberItemSave: Out of memory!" ); exit( 1 ); }
            memcpy( p->LocalVars, &gMapLocalVars[ scr->LocVarsIdx ], scr->LocVarsCnt * sizeof( int ) );
        }
        tmp = gPartyUnk06;
        gPartyUnk06 = p;
        p->Next = tmp;
    }
    for( i = 0; i < item->Critter.Box.Cnt; i++ ) PartyItemSave( item->Critter.Box.Box[ i ].obj );
    return 0;
}

void PartyItemRecover( Party_t *party )
{
    Scpt_t *Script;
    int *LocalVars;
    int Id;
    Scpt_t *scr;

    Id = -1;
    if( ScptNewScript( &Id, 3 ) == -1 ){
        WinMsgError( "\n  Error!: partyMemberItemRecover: Can't create script!" );
        exit( 1 );
    }
    if( ScptPtr( Id, &scr ) == -1 ){
        WinMsgError( "\n  Error!: partyMemberItemRecover: Can't find script!" );
        exit( 1 );
    }
    memcpy( scr, party->Script, sizeof( Scpt_t ) );
    party->dude->ScrId = gPartyPlayerId | 0x3000000;
    scr->Id = gPartyPlayerId | 0x3000000;
    scr->Intp = NULL;
    scr->Flags &= ~0x1D;
    Script = party->Script;
    gPartyPlayerId++;
    Free( Script );
    LocalVars = party->LocalVars;
    party->Script = 0;
    if( LocalVars ){
        MapVarsAdd( scr->LocVarsCnt );
        scr->LocVarsIdx = scr->LocVarsCnt;
        memcpy( &gMapLocalVars[ scr->LocVarsIdx ], party->LocalVars, scr->LocVarsCnt * sizeof(int) );
    }
}

int PartyUnk17()
{
    Party_t *p;
    Scpt_t *Script;

    while( gPartyUnk06 ){
        p = gPartyUnk06;
        Script = gPartyUnk06->Script;
        gPartyUnk06 = gPartyUnk06->Next;
        if( Script ) Free( Script );
        if( p->LocalVars ) Free( p->LocalVars );
        Free( p );
    }
    gPartyPlayerId = 20000;
    return 0;
}

int PartyBestSkill( Obj_t *obj )
{
    int BestSkill, Lvl, i, Total;

    BestSkill = Lvl = 0;
    if( !obj ) return 0;
    if( OBJTYPE( obj->Pid ) != TYPE_CRIT ) return 0;
    for( i = 0; i < 18; i++ ){
        Total = SkillGetTotal( obj, i );
        if( Total > Lvl ){
            BestSkill = i;
            Lvl = Total;
        }
    }
    return BestSkill;
}

Obj_t *PartyGetBestSkilled( unsigned int Skill )
{
    Obj_t *memb;
    int v2, i, Total;

    v2 = 0;    
    memb = NULL;
    for( i = 0; i < gPartyMembersCount; i++ ){
        if( ( gParty[ i ].dude->Flags & 0x01 ) || OBJTYPE( gParty[ i ].dude->Pid ) != TYPE_CRIT ) continue;        
        Total = SkillGetTotal( gParty[ i ].dude, Skill );
        if( Total > v2 ){
	    v2 = Total;
    	    memb = gParty[ i ].dude;
        }             
    }
    return memb;
}

int PartyGetBestSkillLvl( int SkillId )
{
    Obj_t *dude;
    int i, SkillLvl, Total;

    SkillLvl = 0;
    for( i = 0; i < gPartyMembersCount; i++ ){
        dude = gParty[ i ].dude;
        if( !(dude->Flags & 0x01) && OBJTYPE( dude->Pid ) == 1 ){
            Total = SkillGetTotal( dude, SkillId );
            if( Total > SkillLvl ) SkillLvl = Total;
        }
    }
    return SkillLvl;
}

int PartyFix()
{
    Obj_t *obj,*dude, *v10;
    Scpt_t *scr;
    int v2,TotCrittCnt,v18,i;

    eprintf( "\n\n\n[Party Members]:" );
    v18 = 0;
    TotCrittCnt = 0;
    for( obj = ObjGetObjectFirst(); obj; obj = ObjGetObjectNext() ){
	// filter all joinable objects
        v2 = 0;
        if( OBJTYPE( obj->Pid ) == TYPE_CRIT ) TotCrittCnt++;
        for( i = 1; i < gPartyCount; i++ ){ // check if object on the map is joinable
    	    if( obj->Pid == gPartyPids[ i ] ){ v2 = 1; break; }
        }            
        if( !v2 ) continue; // object is not a member of the party
        eprintf( "\n   PM: %s", CritterGetName( obj ) );
	// 
        if( obj->ScrId == -1 ){
            v18 = 1;
        } else {
            dude = NULL;
            for( i = 0; i < gPartyMembersCount; i++ ){
                dude = gParty[ i ].dude;
                if( obj->Pid == dude->Pid ){
        	    if( obj != dude ){
            		if( dude->ScrId == obj->ScrId ) obj->ScrId = -1;
            		v18 = 1;
        	    }
                    break;
                }
            }
        }
        if( !v18 ) continue;
    	v18 = 0;
    	v10 = NULL;
    	for( i = 0; i < gPartyMembersCount; i++ ){
            v10 = gParty[ i ].dude;
            if( obj->Pid == v10->Pid ) break;
    	}
    	if( obj == v10 ){
    	    eprintf( "\nError: Attempting to destroy evil obj doppleganger FAILED!" );
    	    continue;
    	}
    	eprintf( "\nDestroying evil obj doppleganger!" );
    	if( obj->ScrId == -1 ){
    	    if( EvQeDelB( obj, 3 ) == -1 ) eprintf( "\nERROR Removing Timed Events on FIX remove!!\n" );
    	} else {
    	    ScptRemove( obj->ScrId );
    	    obj->ScrId = -1;
    	}
    	ObjDestroy( obj, NULL );
    	TotCrittCnt = 0;
    }
    for( i = 0; i < gPartyMembersCount; i++ ){
        if( ScptPtr( gParty[ i ].dude->ScrId, &scr ) == -1 )
            eprintf( "\nError: Failed to fix party member obj scripts!" );
        else
            scr->TimeEv = gParty[ i ].dude;
    }
    eprintf( "\nTotal Critter Count: %d\n\n", TotCrittCnt );
    return 0;
}

void PartyUnk21()
{
    int i;

    for( i = 1; i < gPartyCount; i++ ){
        if( gPartyPids[ i ] != -1 ) ProtoSaveCritterFile( gPartyPids[ i ] );
    }
}

int PartyInDisposition( Obj_t *obj, int a2 )
{
    DudeDsc_t *dsc;

    if( !obj ) return 0;
    if( OBJTYPE( obj->Pid ) != 1 ) return 0;
    if( a2 > 5 ) return 0;
    if( a2 == -1 ) return 0;
    return ( PartyGetDsc( obj, &dsc ) != -1 && dsc->Disposition[ a2 + 1 ] != 0 );
}

int PartyInAttackMode( Obj_t *obj, int a2 )
{
    DudeDsc_t *dsc;

    if( !obj ) return 0;
    if( OBJTYPE( obj->Pid ) != 1 ) return 0;
    if( a2 >= 5 ) return 0;
    return ( PartyGetDsc( obj, &dsc ) != -1 && dsc->AreaAttackMode[ a2 ] != 0);
}

int PartyInRunAwayMode( Obj_t *obj, int a2 )
{
    DudeDsc_t *dsc;

    if( !obj ) return 0;
    if( OBJTYPE( obj->Pid ) != 1 ) return 0;
    if( a2 >= 7 ) return 0;
    return (PartyGetDsc( obj, &dsc ) != -1 && dsc->RunAwayMode[ a2 + 1 ] != 0 );
}

int PartyInBestWeapon( Obj_t *obj, int a2 )
{
    DudeDsc_t *dsc;

    if( !obj ) return 0;
    if( OBJTYPE(obj->Pid) != 1 ) return 0;
    if( a2 >= 8 ) return 0;
    return ( PartyGetDsc( obj, &dsc ) != -1 && dsc->BestWeapon[ a2 ] != 0 );
}

int PartyInDistance( Obj_t *obj, int a2 )
{
    DudeDsc_t *dsc;

    if( !obj ) return 0;
    if( OBJTYPE(obj->Pid) != 1 ) return 0;
    if( a2 >= 5 ) return 0;
    return ( PartyGetDsc( obj, &dsc ) != -1 && dsc->Distance[a2] != 0 );
}

int PartyInAttackWho( Obj_t *obj, int a2 )
{
    DudeDsc_t *dsc;

    if( !obj ) return 0;
    if( OBJTYPE(obj->Pid) != 1 ) return 0;
    if( a2 >= 5 ) return 0;
    return ( PartyGetDsc( obj, &dsc ) != -1 && dsc->AttackWho[ a2 ] != 0 );
}

int PartyInChemUse( Obj_t *obj, int a2 )
{
    DudeDsc_t *dsc;

    if( !obj ) return 0;    
    if( OBJTYPE( obj->Pid ) != TYPE_CRIT ) return 0;    
    if( a2 >= 6 ) return 0;
    return (PartyGetDsc(obj, &dsc) != -1 && dsc->ChemUse[ a2 ] != 0 );
}

void PartyLvlUp()
{
    Pamb01_t *v5;
    VidRect_t v19;
    MsgLine_t Line;
    DudeDsc_t *dsc;
    char stmp[260];
    int i,LvlMod,v0,j;

    v0 = -1;
    for( i = 1; i < gPartyMembersCount; i++ ){
        if( PartyGetDsc( gParty[ i ].dude, &dsc ) == -1 ) break;
        if( OBJTYPE( gParty[ i ].dude->Pid ) != 1 ) continue;
        eprintf( "\npartyMemberIncLevels: %s", CritterGetName( gParty[ i ].dude ) );
        if( !dsc->LevelUpEvery ) continue;
        for( j = 1; j < gPartyCount; j++ ){
            if( gPartyPids[ j ] == gParty[ i ].dude->Pid ) v0 = j;
        }
        if( v0 == -1 ) continue;
        v5 = &gPartyUnk05[ v0 ];
        if( FeatGetPoints( FEAT_PTS_LVL ) < dsc->LevelMinimum || v5->i01 >= dsc->LevelPidsNo ) continue;                    
        LvlMod = (v5->i02 % dsc->LevelUpEvery);
        v5->i02++;
        eprintf( "\npm: levelMod: %d, Lvl: %d, Early: %d, Every: %d", LvlMod, v5->i02, v5->i03, dsc->LevelUpEvery );
        if( LvlMod || !v5->i03 ){
    	    if( v5->i03 ) continue;
    	    if( LvlMod ){
        	if( RandMinMax( 0, 100 ) > 100 * LvlMod / dsc->LevelUpEvery ) continue;
    	    }
    	    v5->i01++;
    	    if( LvlMod ) v5->i03 = 1;
    	    if( PartyCopyStats( gParty[ i ].dude, dsc->LevelPids[ v5->i01 - 1 ] ) == -1 ) return;                                    
    	    sprintf( stmp, MessageGetMessage( &gMessage, &Line, 9000 ), CritterGetName( gParty[ i ].dude ) );
    	    IfcMsgOut( stmp );
    	    eprintf( "%s", stmp );
    	    Line.Id = v5->i01 - 1 + 10 * v0 + 9000;
    	    if( MessageGetMsg( &gMessage, &Line ) == 1 ){
        	sprintf( stmp, Line.Text, CritterGetName( gParty[ i ].dude ) );
        	TextFloatMessage( gParty[ i ].dude, stmp, 101, gPalColorCubeRGB[31][31][31], gPalColorCubeRGB[0][0][0], &v19 );
        	TileUpdateArea( &v19, gParty[ i ].dude->Elevation );
    	    }                                                                                
        } else {
            v5->i03 = 0;
        }
    }
}

int PartyCopyStats( Obj_t *ToObj, int From )
{
    Obj_t *RHandObj, *ArmorObj;
    Proto_t *proto1, *proto2;
    int i;

    if( !ToObj || From == -1 ) return -1;
    if( ProtoGetObj( ToObj->Pid, &proto2 ) == -1 ) return -1;
    if( ProtoGetObj( From, &proto1 ) == -1 ) return -1;
    RHandObj = InvGetRHandObj( ToObj );
    InvUnk30( ToObj, 1, 0 );
    ArmorObj = InvGetArmorObj( ToObj );
    InvUpdateStatistics( ToObj, ArmorObj, 0 );
    ItemUseItem( ToObj, ArmorObj, 1 );
    CritterHeal( ToObj, FeatGetVal( ToObj, FEAT_HP ) );
    for( i = 0; i != 32; i++ ) proto2->Critt.BaseStat[ i ] = proto1->Critt.BaseStat[ i ];
    for( i = 0; i != 32; i++ ) proto2->Critt.BoostStat[ i ] = proto1->Critt.BoostStat[ i ];
    for( i = 0; i != 18; i++ ) proto2->Critt.Skills[ i ] = proto1->Critt.Skills[ i ];
    ToObj->Critter.HitPts = FeatGetVal( ToObj, FEAT_HP );
    if( ArmorObj ){
        ItemAdd( ToObj, ArmorObj, 1 );
        InvUnk28( ToObj, ArmorObj, 0 );
    }
    if( RHandObj ) InvUnk29( ToObj, RHandObj, 0, 0 );
    return 0;
}

int PartyUnk30( Obj_t *obj )
{
    int i, tmp = -1;
    
    if( !obj ) return 0;
    for( i = 1; i < gPartyCount; i++ ){
        if( gPartyPids[ i ] == obj->Pid ) tmp = i;
    }    
    return ( tmp == -1 ) ? 0 : gPartyUnk05[ tmp ].i01;
}

int PartyIsWounded()
{
    Obj_t *dude;
    int i;

    if( gPartyMembersCount <= 1 ) return 0;
    for( i = 1; i < gPartyMembersCount; i++ ){
        dude = gParty[ i ].dude;
        if( OBJTYPE( dude->Pid ) == 1 && !CritterIsDead( gParty[ i ].dude ) && !( dude->Flags & 0x01 ) && CritterGetGender( dude ) != 10 ){
            if( CritterGetHp( dude ) < FeatGetVal( dude, FEAT_HP ) ) return 1;
        }
    }
    return 0;
}

int PartyMostWound()
{
    Obj_t *dude;
    int MostWound, i, wounds;

    MostWound = 0;
    for( i = 1; i < gPartyMembersCount; i++ ){
        dude = gParty[ i ].dude;
        if( OBJTYPE( dude->Pid ) == TYPE_CRIT && !CritterIsDead( gParty[ i ].dude ) && !( dude->Flags & 0x01 ) && CritterGetGender( dude ) != 10 ){
            wounds = FeatGetVal( dude, FEAT_HP ) - CritterGetHp( dude );
            if( wounds > 0 && MostWound < wounds ) MostWound = wounds;
        }            
    }
    return MostWound;
}

int PartyUnk33( Obj_t *a1 )
{
    gPartyUnk101 = a1;
    EvQeRun( 0, (void *)PartyUnk34 );
    return ( !( a1->Critter.State.CombatResult & 0x80 ) ) - 1;
}

int PartyUnk34( Obj_t *obj, ItemEv01_t *a2 )
{
    if( obj != gPartyUnk101 ) return 0;
    Item105( obj, a2 );
    return 1;
}

int PartyUnk35( xFile_t *fh )
{
    short tmp;

    dbread( &tmp, 2, 1, fh );
    return tmp;
}


