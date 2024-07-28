#pragma once

#define LIGHT_THRESHOLD_FOR_SOLAR_SCORCHER	62259

// predefinied disposition type
enum{
    AI_DISP_CUSTOM,	// 0
    AI_DISP_COWARD,	// 1
    AI_DISP_DEFENSIVE,	// 2
    AI_DISP_AGGRESIVE,	// 3
    AI_DISP_BERSERK	// 4
};
// dispositions
enum{
    AI_DISP_ATTACK,	// 0
    AI_DISP_RUNAWAY,	// 1
    AI_DISP_BESTWEAPON,	// 2
    AI_DISP_DISTANCE,	// 3
    AI_DISP_ATTACKWHO,	// 4
    AI_DISP_CHEMUSE	// 5
};

typedef struct
{
    char *Name;
    int PacketNum;
    int MaxDist;
    int MinToHit;
    int MinHP;
    int Aggression;
    int HurtTooMuch;
    int SecondaryFreq;
    int CalledFreq;
    int Font;
    int Color;
    int OutlineColor;
    int Chance;
    int RunStart;
    int RunEnd;
    int MoveStart;
    int MoveEnd;
    int AttackStart;
    int AttackEnd;
    int MissStart;
    int MissEnd;
    union{
	struct{
	    int HitHeadStart;
	    int HitHeadEnd;
	    
	    int HitLeftArmStart;
	    int HitLeftArmEnd;
	    
	    int HitRightArmStart;
	    int HitRightArmEnd;
	    
	    int HitTorsoStart;
	    int HitTorsoEnd;
	};
	struct {
	    int Start;
	    int End;
	} HitTable[ 8 ];
	int Hits[16];
    };
    int HitRightLegStart;
    int HitRightLegEnd;
    int HitLeftLegStart;
    int HitLeftLegEnd;
    int HitEyesStart;
    int HitEyesEnd;
    int HitGroinStart;
    int HitGroinEnd;

    int AreaAttackMode;
    int RunAwayMode;
    int BestWeapon;
    int Distance;
    int AttackWho;
    int ChemUse;
    int ChemDesire[3];
    int Disposition;
    char *BodyType;
    char *GeneralType;
} Ai_t;

// Combat_t * to cut
typedef struct {
    Obj_t *Obj;
    int	  Slot;
    int i03;
    int i04;
    int i05;
    int i06;
    int i07;
    int i08;
    Obj_t *Obj1;
    int i10;
    int i11;
} Ai01_t;

typedef struct 
{
  Obj_t *Attacker;
    Obj_t *Unk;
  Obj_t *Enemies[ 100 ];
  int EnemyFirePower[ 100 ];
  int Cnt;
  int GroupId;
  int AttackerFirePower;
  int AttackerPos;
  int *Positions;
  int Unk02;
  int Intelligence;
} Ai02_t;


void AiParseInjureFlags( char *str, int *Flags );
int AiListMatch( char *val, const char **list, int cnt, int *pVal );
int AiPacketInit( Ai_t *ai );
void AiLoadCfg();
int AiReset();
void AiClose();
int AiLoad( xFile_t *fh );
int AiFSave( xFile_t *fh );
int AiLoadPacket( xFile_t *fh, Ai_t *ai );
int AiSavePacket( xFile_t *fh, Ai_t *pck );
int AiGetPacketCount();
char *AiGetPacketName( int PacketId );
Ai_t *AiGetPacketByObj( Obj_t *obj );
Ai_t *AiGetPacketById( int PacketId );
int AiGetAreaAttackMode( Obj_t *obj );
int AiGetHealthGrade( Obj_t *obj );
int AiGetBestWeapon( Obj_t *obj );
int AiGetDistance( Obj_t *obj );
int AiGetAttackWho( Obj_t *obj );
int AiGetChemUse( Obj_t *obj );
int AiSetRunAway( Obj_t *obj, int RunAwayMode );
int AiGetDisposition( Obj_t *obj );
int AiUnk06( Obj_t *obj, int a2 );
void AiUnk07( Obj_t *obj1, Obj_t *obj2, int TextId );
Ai_t *AiUnk08( Obj_t *obj );
void AiMoveRunTo( Obj_t *obj1, Obj_t *obj2 );
int AiMoveWalkTo( Obj_t *obj1, Obj_t *obj2, int a3 );
int AiInRange( Obj_t *obj, int a2, int a3 );
/*
    -1 if obj1 < obj2
    0  if obj1 = obj2
    +1 if obj1 > obj2 
*/
int AiCmpByDistance( Obj_t **obj1, Obj_t **obj2 );
void AiSortByDistance( Obj_t **objs, int Count, Obj_t *a3 );
int AiCmpByUnk1( Obj_t **pObj1, Obj_t **pObj2 );
void AiSortByUnk1( Obj_t **objs, int Count );
int AiCmpByUnk2( Obj_t **pObj1, Obj_t **pObj2 );
void AiSortByUnk2( Obj_t **objs, int Count );
Obj_t *AiUnk18( Obj_t *obj1, Obj_t *obj2, char flg );
Obj_t *AiUnk19( Obj_t *obj1, Obj_t *obj2, char flg );
int AiUnk20( Obj_t *a1, Obj_t **a2, Obj_t **Attacker, Obj_t **a4 );
Obj_t *AiDangerSource( Obj_t *obj );
int AiUnk22( Obj_t *obj, Obj_t *a2 );
int AiUnk23( Obj_t **objs, int Count );
void AiUnk24();
int AiFindAmmo( Obj_t *Critter, Obj_t *Weapon, Obj_t **pAmmo );
int AiUnk26( Ai_t *pck, int Val );
Obj_t *AiUnk27( Obj_t *eax0, Obj_t *a2, Obj_t *a3, Obj_t *ebx0 );
int AiUnk28( Obj_t *obj1, Obj_t *Weapon );
Obj_t *AiUnk29( Obj_t *obj1, int a2, Obj_t *a3 );
Obj_t *AiFindBestArmor( Obj_t *obj );
int AiUnk31( Obj_t *Critter, Obj_t *a2 );
Obj_t *AiUnk32( Obj_t *obj, int a2 );
Obj_t *AiPickUpObj( Obj_t *Critter, Obj_t *Item );
int AiUnk34( Obj_t *Critter, Obj_t *ItemObj, Obj_t *Target );
int AiUnk35( Obj_t *a1, Obj_t *a2, int a3, int a4 );
int AiUnk36( Obj_t *a1, Obj_t *a2, int a3 );
int AiUnk37( Obj_t *obj, Obj_t *pe, int *pIdx );
int AiUnk38( Ai02_t *grp, int idx );
int AiUnk39( Obj_t *a1, Obj_t *a2, Obj_t *a3, int a4, int *n );
int AiUnk40( Obj_t *a1, int *a2, Obj_t **a3, Obj_t *a4 );
int AiUnk41( Obj_t *a1, Obj_t *a2, int a3 );
int AiUnk42( Obj_t *a1, Obj_t *a2, int a3 );
int AiUnk43( Obj_t *a1, Obj_t *edx0 );
int AiLightFlare( Obj_t *a1, Obj_t *a2 );
void AiUnk45( Obj_t *a1, int a2 );
void AiUnk46( int Count, Obj_t **p );
void AiUnk47();
int AiUnk48( Obj_t *obj, Obj_t *a2 );
int AiUnk49( Ai_t *pck );
Obj_t *AiUnk50( Obj_t *a1, Obj_t *a2 );
int AiUnk51( Obj_t *obj );
int AiUnk52( Obj_t *obj );
int AiUnk53( Obj_t *obj, int Group );
int AiUnk54( Obj_t *obj, int PackNb );
int AiCombatTaunts( Obj_t *Critter, Combat_t *Combat, int ReactionType, int a4 );
int AiMumble( Obj_t *obj, int type );
Obj_t *AiDrawOponent( Combat_t *a1 );
int AiGetMaxDamagePower( Obj_t *obj );
int AiUnk59( Obj_t *obj, Obj_t *a2 );
int AiObjCanHearObj( Obj_t *Rogue, Obj_t *Target );
int AiLoadCombatMsg();
int AiUnk61();
void AiUnk62();
void AiUnk63( Obj_t *obj );
void AiUnk64( Obj_t *obj );
void AiUnk65( Obj_t *obj );

 
 