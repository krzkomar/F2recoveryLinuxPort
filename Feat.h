#pragma once

#define FEAT_PTS_LEVELCAP	99
#define FEAT_PTS_PER_LEVEL( lvl ) 1000 * (( lvl & 0x01 ) ? ((lvl / 2) * lvl) : ((lvl / 2) * (lvl - 1)))
#define FEAT_DMG_THR( x )	(FEAT_17 + x)
#define FEAT_DMG_RES( x )	(FEAT_24 + x)

// threshold/resistance
enum{
    FEAT_DMG_NORMAL,	// 0
    FEAT_DMG_LASER,	// 1
    FEAT_DMG_FIRE,	// 2
    FEAT_DMG_PLASMA,	// 3
    FEAT_DMG_ELECTRIC,	// 4
    FEAT_DMG_EMP,	// 5
    FEAT_DMG_EXPLOSIVE	// 6
};

// stats
enum{
    // base
    FEAT_STAMINA,	// 0 stamina
    FEAT_PERCEPTION,	// 1 perception
    FEAT_ENDURANCE,	// 2 endurance
    FEAT_CHARISMA,	// 3 charisma
    FEAT_INTELLIGENCE,	// 4 intelligence
    FEAT_AGILITY,	// 5 agility
    FEAT_LUCK,		// 6 luck
//
    FEAT_HP,		// 7 hit points
    FEAT_AP,		// 8 action points
    FEAT_AC,		// 9 armor class
    FEAT_10,		// 10 unused ( unarmed damage )
    FEAT_MELEE,		// 11 melee damage
    FEAT_CARRY,		// 12 carry weight
    FEAT_SEQ,		// 13 sequence
    FEAT_HEALING,	// 14 healing rate
    FEAT_CRIT,		// 15 critical chance
    FEAT_16,		// 16 critical hit table roll modifier
    FEAT_17,		// 17 damage threshold  (normal)
    FEAT_18,		// 18 damage threshold  (laser)
    FEAT_19,		// 19 damage threshold  (fire)
    FEAT_20,		// 20 damage threshold  (plasma)
    FEAT_21,		// 21 damage threshold  (electrical)
    FEAT_22,		// 22 damage threshold  (EMP)
    FEAT_23,		// 23 damage threshold  (explosive)
    FEAT_24,		// 24 damage resistance (normal)
    FEAT_25,		// 25 damage resistance (laser)
    FEAT_26,		// 26 damage resistance (fire)
    FEAT_27,		// 27 damage resistance (plasma)
    FEAT_28,		// 28 damage resistance (electrical)
    FEAT_29,		// 29 damage resistance (EMP)
    FEAT_30,		// 30 damage resistance (explosive)
    FEAT_RADRES,	// 31 radiation resistamce
    FEAT_PSNRES,	// 32 poison resistance
    FEAT_33,		// 33 players starting age
    FEAT_GENDER,	// 34 gender 0 - male, 1 - female
    FEAT_35,		// 35 FEAT_CURHP current HP
    FEAT_36,		// 36 FEAT_POISONED
    FEAT_37,		// 37 FEAT_RADIATED
    // unspent skill points, level, experience, reputation, karma
    FEAT_PTS = 38,
    FEAT_PTS_SKILL = 0, // 0 38 unspent skill points
    FEAT_PTS_LVL,	// 1 39
    FEAT_PTS_EXP,	// 2 40
    FEAT_PTS_REP,	// 3 41
    FEAT_PTS_KARMA	// 4 42
};

// condition
enum{
    FEAT_POISONED = 312,	// 312 poisoned
    FEAT_RADIATED,		// 313 radiated
    FEAT_EYEDMG,		// 314 eye damage
    FEAT_CRIP_RA,		// 315 crippled right arm
    FEAT_CRIP_LA,		// 316 crippled left arm
    FEAT_CRIP_RL,		// 317 crippled right leg
    FEAT_CRIP_LL,		// 318 crippled left leg
};

enum{
    FEAT_PT_SP,		// 0 unspent skill points
    FEAT_PT_LV,		// 1 current level
    FEAT_PT_XP,		// 2 experience points
    FEAT_PT_RP,		// 3 reputation points
    FEAT_PT_KM,		// 4 karma points
    FEAT_PT_5,		// 5
    FEAT_PT_6,		// 6
    FEAT_PT_7,		// 7
    FEAT_PT_8,		// 8
    FEAT_PT_9 		// 9 
};

typedef struct 
{
    char *Name;
    char *Description;
    int Id;
    int Min;
    int Max;
    int Value;
} Feat_t;

//Feat_t gFeats[ 43 ];
//Msg_t gFeatMsgStat;
//char *gFeatComment[ 10 ];
//int gFeatPoints[ 10 ];

int  FeatStatInit();
void FeatResetPoints();
int  FeatStatClose();
int  FeatLoadPoints( xFile_t *fh );
int  FeatSavePoints( xFile_t *fh );
int  FeatGetVal( Obj_t *dude, int FeatId );
int  FeatGetTotal( Obj_t *dude, unsigned int Id );
int  FeatGetBase( Obj_t *dude, int Id );
int  FeatGetBoost( Obj_t *dude, int Id );
int  FeatSetBase( Obj_t *dude, int Id, int NewVal );
int  FeatIncVal( Obj_t *dude, int Id );
int  FeatDecVal( Obj_t *dude, int Id );
int  FeatSetBoost( Obj_t *dude, int Id, int NewVal );
void FeatReset( Critter_t *p );
void FeatStatsRecalculate( Obj_t *dude );
char *FeatGetName( unsigned int feat );
char *FeatGetDsc( unsigned int feat );
char *FeatGetComments( unsigned int FeatNo );
int  FeatGetPoints( unsigned int Feat );
int  FeatSetPoints( unsigned int Id, int NewVal );
void FeatPtsReset();
int  FeatNextLvlPts();
int  FeatPtsPerLvl( int lvl );
char *FeatGetPtsName( unsigned int Id );
char *FeatGetPtsDsc( unsigned int Id );
int  FeatGetId( int idx );
int  FeatDice( Obj_t *dude, int Id, int Ofs, int *pVal );
int  FeatLvlUp( int pts );
int  FeatLvlUpStats( int pts, int LvlUpAllow );
int  FeatLvlDn( int pts );


