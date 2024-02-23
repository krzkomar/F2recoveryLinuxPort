#pragma once

#define COMBAT_RECOVERY_AP_COST 3
#define CBT_IN_COMBAT	0x01
#define IN_COMBAT	(gCombatStatus  & CBT_IN_COMBAT)

// combat flags:
#define CMBT_DAM_KNOCKED_OUT	0x01
#define CMBT_DAM_KNOCKED_DOWN	0x02
#define CMBT_DAM_CRIP_LEG_LT	0x04
#define CMBT_DAM_CRIP_LEG_RT	0x08
#define CMBT_DAM_CRIP_ARM_LT	0x10
#define CMBT_DAM_CRIP_ARM_RT	0x20
#define CMBT_DAM_BLIND		0x40
#define CMBT_DAM_DEAD		0x80
#define CMBT_DAM_HIT		0x100
#define CMBT_DAM_CRITICAL	0x200
#define CMBT_DAM_ON_FIRE	0x400
#define CMBT_DAM_BYPASS		0x800
#define CMBT_DAM_EXPLODE	0x1000
#define CMBT_DAM_DESTROY	0x2000
#define CMBT_DAM_DROP		0x4000
#define CMBT_DAM_LOSE_TURN	0x8000
#define CMBT_DAM_HIT_SELF	0x10000
#define CMBT_DAM_LOSE_AMMO	0x20000
#define CMBT_DAM_DUD		0x40000
#define CMBT_DAM_HURT_SELF	0x80000
#define CMBT_DAM_RANDOM_HIT	0x100000
#define CMBT_DAM_CRIP_RANDOM	0x200000
#define CMBT_DAM_BACKWASH	0x400000
#define CMBT_DAM_PERFORM_REVERSE	0x800000



typedef struct 
{
    Obj_t *i01;
    Obj_t *i02;
    Obj_t *i03;
    int    i04;
} Combat01_t;

typedef struct
{
    Obj_t *Critter;
    Obj_t *Target;
    int   unk01[ 4 ]; // 1 - hit bonus
    int  unk02;
    int  unk03;
} Combat02_t; // Scpt01_t

typedef struct
{
    int i01;
    int i02;
    int i03;
    int i04;
    int i05;
    int i06;
    int i07;
} Combat03_t;

typedef struct // size of b8
{
    Obj_t *Dude;	// player ?
    int Hand;
    Obj_t *HandEq;
    int i04;
    int DudeDmg;
    int DudeInjuries;
    int AmmoCap;
    int i08;

    Obj_t *Comp;	// company ?
    int TileNo;
    int i11;
    int CompDmg;
    int CompInjuries;
    int CompKnockDown;	// chance to be knocked down
    Obj_t *i15;

    int Count;    
    Obj_t *obj[ 6 ];
    int unk1[ 6 ];
    int Damage[ 6 ];
    int Injuries[ 6 ];
    int KnockDown[ 6 ];
} Combat_t;

extern int gCombatStatus;
extern int gCombatMovePts;
extern int gCombat01;

/*
int gCombat00;
int gCombat01;
int gCombatStatus;
Combat01_t *gCombat03;
Obj_t **gCombat07;
int gCombat08;
int gCombatTacticMode;
int gCombat12;
int gCombatUnk12[ 4 ];
int gCombatBodyParts[ 8 ];
Combat_t gCombat20;
Msg_t gCombatMsg;
Obj_t *gCombatTarget;
int gCombatFocusWin;
int gCombatMapLvl;
int gCombatCritCnt;
Obj_t *gCombat10;
int gCombat05;
int gCombat06;
Obj_t *gCombat19;
int gTargetHighlightLvl;
Obj_t **gCombatCritters;
int gCombat04;
int gCombat21;
int gCombatMovePts;
*/



/************************************/
int CombatInit();
int CombatReset();
int CombatClose();
int CombatLookForAgressor( int CritIdx, int WhoHitMe, Obj_t **CrittList, int CrittCnt );
int CombatLoad( xFile_t *fh );
int CombatFSave( xFile_t *fh );
int CombatUnk02( Obj_t *a1, Obj_t *a2, int a4, Obj_t *a3, int *pSteps );
int CombatUnk03( Obj_t *a1, Obj_t *a2, int a3, Obj_t *a4, int *pSteps, Obj_t *a6 );
int CombatUnk04( Obj_t *a1, Obj_t *a2, Obj_t *a3, Obj_t *a4 );
Obj_t *CombatUnk05();
void CombatUnk06( Obj_t *obj );
void CombatUnk07();
void CombatUnk08( int a1, int a2 );
Obj_t *CombatUnk09( Obj_t *obj );
int CombatUnk10( Obj_t *obj1, Obj_t *obj2 );
Obj_t *CombatUnk11( Obj_t *obj );
int CombatStopAttack( Obj_t *obj1, Obj_t *obj2 );
Obj_t *CombatUnk13( Obj_t *obj );
int CombatUnk14( Obj_t *obj );
int CombatUnk15( Obj_t *obj );
int CombatUnk16( Obj_t *obj, int arg );
void CombatUnk17( Obj_t *a1 );
int CombatTarget( Obj_t *obj );
void CombatUnk19( int Flg );
void CombatUnk01( Obj_t *obj, int Flg );
int CombatUnk20();
void CombatUnk21();
void CombatEarnExpPts( int Exp );
void CombatUnk23();
int CombatUnk24( Obj_t *obj );
int CombatUnk25( Obj_t **a1, Obj_t **a2 );
int CombatUnk26( Obj_t *a1, Obj_t *a2 );
void CombatUnk27();
void CombatProcess();
void CombatUpdate();
int CombatUnk30();
void CombatUnk31();
void CombatUnk32();
int CombatUnk33( Obj_t *obj, int edx0 );
int CombatUnk34();
void CombatStart( Combat02_t *pObj );
void CombatSetUp( Combat_t *cmbt, Obj_t *Critt, Obj_t *a3, int Hand, int a5 );
int CombatAttack( Obj_t *a1, Obj_t *a2, int a3, int a4 );
int CombatUnk37( Obj_t *a1, Obj_t *a2 );
int CombatUnk38( Combat_t *a1 );
int CombatUnk39( Combat_t *a1, int a2, int ebx0, int a4 );
int CombatUnk40( Combat_t *a1, int a2, int *a3, int *a4, int a5 );
void CombatUnk41( Combat_t *a1 );
int CombatUnk42( Combat_t *eax0 );
void CombatExplosion( Combat_t *eax0, int a2, int ecx0, int a4 );
int CombatUnk44( Combat_t *a1 );
int CombatUnk45( Obj_t *obj, Obj_t *a2 );
void CombatInjure( Combat_t *a1 );
void CombatDrawInjure( int *a1 );
int CombatDetermineHitObstacled( Obj_t *a1, Obj_t *a2, int a3, int a4 );
int CombatDetermineHit( Obj_t *a1, Obj_t *a2, int a3, int a4 );
int CombatDetermineHitDist( Obj_t *a1, int a2, Obj_t *a3, int a4, int a5 );
int CombatGetHitChance( Obj_t *eax0, int edx0, Obj_t *a3, int a4, int a5, int a6 );
void CombatSetDmgChance( Combat_t *cmbt, int, int );
void CombatKillUpdate( Combat_t *cmbt );
void CombatUnk54( Combat_t *a1, int a2 );
void CombatKillCheck( Obj_t *result, int a2, int *a3 );
void CombatUnk56( Obj_t *obj, int a2 );
void CombatUnk57( Obj_t *a1, int a2, int a3, int a4, Obj_t *result );
void CombatUnk58( Combat_t *a1 );
void CombatGetHitInfoText( char *str, Obj_t *Critter, int DmgPts );
void CombatGetFlagsName( char *str, int CombatFlags, Obj_t *Critter );
void CombatUnk61();
void CombatUnk62();
void CombatRecovery( Obj_t *dude );
void CombatUnk64( char *a1, int a2, int a3 );
char *CombatGetBodyPartName( Obj_t *Critter, int BodyPartId );
void CombatUnk66( int a1, int a2 );
void CombatUnk67( int a1, int a2 );
void CombatFocusPrintLabel( int BodyPart, int Color );
int CombatFocusMenu( Obj_t *TargetObj, int *BodyPart, int Slot );
int CombatAttackTest( Obj_t *attacker, Obj_t *target, int Slot, int ShotValue );
int CombatUnk71( Obj_t *obj, int *a2 );
void CombatStartAttack( Obj_t *Target );
void CombatTargetHighlight();
void CombatUnk74( );
void CombatUnk75();
int CombatBlockedAim( Obj_t *obj1, int GridPos1, int GridPos2, Obj_t *obj2, int *arg0 );
int CombatGetGroupId();
int CombatUnk77( Obj_t *obj );
void CombatUnk79( Obj_t *obj );
void CombatKillPoison( Obj_t *obj, char *text );
void CombatDealDamage( Obj_t *Attacker, int dmg, int, int, Obj_t *);
void CombatHitInfo( Combat_t *);
