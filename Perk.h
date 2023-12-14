#pragma once

// id 101 + perk.msg 
#define PERK_AWARENESS			0
#define PERK_BONUS_HTH_ATTACK		1
#define PERK_BONUS_HTH_DMG		2
#define PERK_BONUS_MOVE			3
#define PERK_BONUS_RANGED_DMG		4
#define PERK_BONUS_RATE_FIRE		5
#define PERK_ERLIER_SEQ			6
#define PERK_FASTER_HEALING		7
#define PERK_MORE_CRITICALS		8
#define PERK_NIGHT_VISION		9
#define PERK_PRESENCE			10
#define PERK_RAD_RESISTANCE		11
#define PERK_TOUGHTNESS			12
#define PERK_STRONG_BACK		13
#define PERK_SHARPSHOOTER		14
#define PERK_SILENT_RUNNING		15
#define PERK_SURVIVALIST		16
#define PERK_MASTER_TRADER		17
#define PERK_EDUCATED			18
#define PERK_HEALER			19
#define PERK_FORTUNE_FINDER		20
#define PERK_BETTER_CRITICALS		21
#define PERK_EMPATHY			22
#define PERK_SLAYER			23
#define PERK_SNIPER			24
#define PERK_SILENT_DEATH		25
#define PERK_ACTION_BOY			26
#define PERK_MENTAL_BLOCK		27
#define PERK_LIFEGIVER			28
#define PERK_DODGER			29
#define PERK_SNAKEATER			30
#define PERK_MRFIXIT			31
#define PERK_MEDIC			32
#define PERK_MASTER_THIEF		33
#define PERK_SPEAKER			34
#define PERK_HEAVE_HO			35
#define PERK_FRIENDLY_FOE		36
#define PERK_PICKPOCKET			37
#define PERK_GHOST			38
#define PERK_CULT_PERSONALITY		39
#define PERK_SCROUNGER			40
#define PERK_EXPLORER			41
#define PERK_FLOWER_CHILD		42
#define PERK_PATHFINDER			43
#define PERK_ANIMAL_FRIEND		44
#define PERK_SCOUT			45
#define PERK_MYSTERIOUS_STRANGER	46
#define PERK_RANGER			47
#define PERK_QUICK_POCKETS		48
#define PERK_SMOOTH_TALKER		49
#define PERK_SWIFT_LEARNER		50
#define PERK_TAG			51
#define PERK_MUTATE			52
#define PERK_NUKACOLA_RELIANCE		53
#define PERK_BUFFOUT_RELIANCE		54
#define PERK_MENTATS_RELIANCE		55
#define PERK_PSYCHO_RELIANCE		56
#define PERK_RADAWAY_RELIANCE		57
#define PERK_WEAPON_LONG_RANGE		58
#define PERK_WEAPON_ACCURATE		59
#define PERK_WEAPON_PENETRATE		60
#define PERK_WEAPON_KNOCKBACK		61
#define PERK_POWERED_ARMOR		62
#define PERK_COMBAT_ARMOR		63
#define PERK_WEAPON_SCOPE_RANGE		64
#define PERK_WEAPON_FAST_RELOAD		65
#define PERK_WEAPON_NIGHT_SIGHT		66
#define PERK_WEAPON_FLAMEBOY		67
#define PERK_ARMOR_ADVANCED_I		68
#define PERK_ARMOR_ADVANCED_II		69
#define PERK_JET_RELIANCE		70
#define PERK_TRAGIC_RELIANCE		71
#define PERK_ARMOR_CHARISMA		72
#define PERK_GECKO_SKINNING		73
#define PERK_DERMAL_IMPACT_ARMOR	74
#define PERK_DERMAL_IMPACT_ASSLT_ENCH	75
#define PERK_PHOENIX_ARMOR_IMPLANTS	76
#define PERK_PHOENIX_ASSAULT_ENCH	77
#define PERK_VAULT_CITY_INOCULATIONS	78
#define PERK_ADRENALINE_RUSH		79
#define PERK_CAUTIOUS_NATURE		80
#define PERK_COMPREHENSION		81
#define PERK_DEMOLITION_EXPERT		82
#define PERK_GAMBLER			83
#define PERK_GAIN_STRENGHT		84
#define PERK_GAIN_PERCEPTION		85
#define PERK_GAIN_ENDURANCE		86
#define PERK_GAIN_CHARISMA		87
#define PERK_GAIN_INTELLIGENCE		88
#define PERK_GAIN_AGILITY		89
#define PERK_GAIN_LUCK			90
#define PERK_HARMLESS			91
#define PERK_HERE_AND_NOW		92
#define PERK_HTH_EVADE			93
#define PERK_KAMA_SUTRA_MASTER		94
#define PERK_KARMA_BEACON		95
#define PERK_LIGHT_STEP			96
#define PERK_LIVING_ANATOMY		97
#define PERK_MAGNETIC_PERSONALITY	98
#define PERK_NEGOTIATOR			99
#define PERK_PACK_RAT			100
#define PERK_PYROMANIAC			101
#define PERK_QUICK_RECOVERY		102
#define PERK_SALESMAN			103
#define PERK_STONEWELL			104
#define PERK_THIEF			105
#define PERK_WEAPON_HANDLING		106
#define PERK_VAULT_CITY_TRAINING	107
#define PERK_ALCOHOL_RAISED_HP1		108
#define PERK_ALCOHOL_RAISED_HP2		109
#define PERK_ALCOHOL_LOWERED_HP1	110
#define PERK_ALCOHOL_LOWERED_HP2	111
#define PERK_AUTODOC_RAISED_HP1		112
#define PERK_AUTODOC_RAISED_HP2		113
#define PERK_AUTODOC_LOWERED_HP1	114
#define PERK_AUTODOC_LOWERED_HP2	115
#define PERK_EXPERT_EXCREMENT_EXPEDITOR	116
#define PERK_WEAPON_ENHANCED_KNOCKOUT	117
#define PERK_JINXED			118



extern int gPartyCount;
extern int *gPartyPids;

typedef struct 
{
  char *Name;
  char *Dsc;
  int ImgId;
  int MaxLvl;
  int Pe;
  int i06;
  int i07;
  int i08;
  int i09;
  int i10;
  int i11;
  int i12;
  int FeatsMod[ 7 ];
} Perk_t;

//Perk_t gPerk[ 119 ];
//Msg_t gMsgPerk;
//int *gPerkLvlData;
//int gPerkUnk01;

void PerkInit();
int  PerkZero();
void PerkClose();
int  PerkLoad( xFile_t *fh );
int  PerkSave( xFile_t *fh );
int *PerkGetLevelData( Obj_t *dude );
int  PerkLvlUpAvail( Obj_t *dude, unsigned int PerkIdx );
void PerkReset();
int  PerkLvlInc( Obj_t *dude, unsigned int PerkIdx );
int  PerkLvlUp( Obj_t *dude, unsigned int SkillIdx );
int  PerkLvlDec( Obj_t *dude, unsigned int PerkIdx );
int  PerkGetAvail( Obj_t *dude, int *Avail );
int  PerkLvl( Obj_t *dude, unsigned int PerkIdx );
char *PerkGetName( unsigned int PerkIdx );
char *PerkGetDsc( unsigned int PerkIdx );
int  PerkGetImgId( unsigned int PerkIdx );
void PerkAddEffect( Obj_t *dude, int PerkIdx );
void PerkRemoveEffect( Obj_t *dude, unsigned int PerkIdx );
int  PerkGrantPts( Obj_t *dude, int lvl );

