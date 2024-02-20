#pragma once

typedef struct // size of 200
{
    int	AreaAttackMode[5];
    int	RunAwayMode[7];
    int BestWeapon[8];
    int Distance[5];
    int AttackWho[5];
    int ChemUse[6];
    int Disposition[6];
    int LevelMinimum;
    int LevelUpEvery;
    int LevelPidsNo;
    int LevelPids[5];
} DudeDsc_t;

typedef struct // size of 12
{
    int i01;
    int i02;
    int i03;
} Pamb01_t;

typedef struct _Party_t // size of 16
{
    Obj_t *dude;
    Scpt_t *Script; // 224 in size
    int *LocalVars;
    struct _Party_t *Next;    
} Party_t;

extern const char *gSeriousInjureListNames[ 4 ];
extern const char *gAreaAttackModeListNames[5];
extern const char *gRunAwayListNames[];
extern const char *gBestWeaponListNames[8];
extern const char *gDistanceListNames[5];
extern const char *gChemUseListNames[];
extern const char *gWhomAttackListNames[];
extern const char *gDispositionListNames[6];

/****************************************************/
/*
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
*/

int PartyInit();
void PartyUnk01();
void PartyClose();
int PartyGetDsc( Obj_t *dude, DudeDsc_t **dsc );
void PartyDscInit( DudeDsc_t *dsc );
int PartyUnk02( int MembNo );
int PartyAddMember( Obj_t *dude );
int PartyRemoveMember( Obj_t *dude );
int PartyLeaveAll();
int PartyRejoinAll();
int PartyFSave( xFile_t *fh );
Obj_t *PartyUnk03( int Pid );
int PartyLoad();
int PartyPrepLoadInstance( Party_t *pm );
int PartyRecoverLoad();
int PartyRecoverLoadInstance( Party_t *party );
int PartyFLoad( xFile_t *fh );
void PartyUnk06();
int PartyUnk07();
int PartyHeal( int a1 );
Obj_t * PartyMemberObj( int Pid );
int PartyUnk10();
int PartyMembRdy( Obj_t *obj );
int PartyUnk12();
int PartyUnk13();
int PartyUnk14( Obj_t *a1, int pid );
int PartySaveBox();
int PartyPrepItemSaveAll( Obj_t *obj );
int PartyUnk16( Obj_t *a1 );
int PartyItemSave( Obj_t *item );
void PartyItemRecover( Party_t *party );
int PartyUnk17();
int PartyBestSkill( Obj_t *obj );
Obj_t *PartyGetBestSkilled( unsigned int Skill );
int PartyGetBestSkillLvl( int SkillId );
int PartyFix();
void PartySave();
int PartyInDisposition( Obj_t *obj, int a2 );
int PartyInAttackMode( Obj_t *obj, int a2 );
int PartyInRunAwayMode( Obj_t *obj, int a2 );
int PartyInBestWeapon( Obj_t *obj, int a2 );
int PartyInDistance( Obj_t *obj, int a2 );
int PartyInAttackWho( Obj_t *obj, int a2 );
int PartyInChemUse( Obj_t *obj, int a2 );
void PartyLvlUp();
int PartyCopyStats( Obj_t *ToObj, int From );
int PartyUnk30( Obj_t *obj );
int PartyIsWounded();
int PartyMostWound();
int PartyUnk33( Obj_t *a1 );
int PartyUnk34( Obj_t *obj, ItemEv01_t *a2 );
int PartyUnk35( xFile_t *fh );


