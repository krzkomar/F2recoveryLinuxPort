#pragma once

#define SKILL_MSG_FILE	"skill.msg"

enum{
    SKILL_SMALLGUNS,	// 0
    SKILL_BIGGUNS,	// 1
    SKILL_ENERGYWP,	// 2
    SKILL_UNARMED,	// 3
    SKILL_MELEE,	// 4
    SKILL_THROWING,	// 5
    SKILL_FIRSTAID,	// 6
    SKILL_DOCTOR,	// 7
    SKILL_SNEAK,	// 8
    SKILL_LOCKPICK,	// 9
    SKILL_STEAL,	// 10
    SKILL_TRAPS,	// 11
    SKILL_SCIENCE,	// 12
    SKILL_REPAIR,	// 13
    SKILL_SPEECH,	// 14
    SKILL_BARTER,	// 15
    SKILL_GAMBLING,	// 16
    SKILL_OUTDOORSMAN,	// 17
    SKILL_NUM
};

typedef struct
{
    char *Name;	// name text
    char *Dsc;  // description text
    char *Attr; // attribute text
    int i04;
    int i05;
    int i06;
    int i07;
    int i08;
    int i09;
    int i10;
    int i11;
} Skill_t;

//Skill_t gSkills[ 18 ];
//int gSkillMenuUnk01;
//int gSkillMenuIds[ 6 ];
//int gSkillMenuIdx[ 8 ];
//const int gSkillDoctorInjuresMask[5];
//int gSkillUsage[ 18 ][3]; // 12*18
//int gSkillSpecials[ 4 ];
//Msg_t gSkillMsg;
//Geo_t gSkillMenuGeo[12];
//char *gSkillMenuSurfaces[16];
//Msg_t gSkillMenuMsg;
//MsgLine_t gSkillMenuMsgLine;
//char *gSkillMenuPix[6];
//CachePool_t *gSkillMenuImgs[6];
//int gSkillMenuWin;
//char *gSkillMenuSurf;
//int gSkillMenuFontSave;


int  SkillInit();
int  SkillClear();
int  SkillMsgClose();
int  SkillLoadSpecials( xFile_t *fname );
int  SkillSaveSpecials( xFile_t *fh );
void SkillReset( Critter_t *cr );
void SkillSetSpecials( int *set, int n );
void SkillGetSpecials( int *res, int num );
int  SkillIsSpecial( int SkilIdx );
int  SkillGetTotal( Obj_t *dude, unsigned int SkillIdx );
int  SkillGetLvl( unsigned int SkillIdx );
int  SkillGetVal( Obj_t *dude, unsigned int SkillIdx );
int  SkillLvlUp( Obj_t *dude, unsigned int SkillIdx );
int  SkillInc( Obj_t *dude, unsigned int SkillIdx );
int  SkillPtsCost( int pts );
int  SkillLvlDn( Obj_t *dude, unsigned int SkillIdx );
int  SkillDec( Obj_t *dude, unsigned int SkillIdx );
int  SkillUse( Obj_t *dude, unsigned int SkillIdx, int *a3, int a4 );
int  SkillUseOnDude( Obj_t *dude1, Obj_t *dude2, int a3, unsigned int SkillIdx, int a5, int *a6 );
char *SkillGetName( unsigned int SkillNo );
char *SkillGetDsc( unsigned int SkillNo );
char *SkillGetAttr( unsigned int SkillNo );
int  SkillGetImgId( unsigned int SkillNo );
void SkillHoningAward( Obj_t *dude,int SkillIdx, int a3, int a4 );
int  SkillUseHeal( Obj_t *dude1, Obj_t *dude2, int a3, unsigned int SkillIdx );
int  SkillUseSteal( Obj_t *dude1, Obj_t *dude2, int a3, Obj_t *Company);
int  SkillGetDifficulty( int SkillIdx );
int  SkillUsingTired( int SkillIdx );
int  SkillUsing( int SkillIdx );
int  SkillUsingClear();
int  SkillUsingSave( xFile_t *fh );
int  SkillUsingLoad( xFile_t *fh );
char *SkillUseTryWantMsg( int Want );
int  SkillDexMenu();
int  SkillMenuOpen();
void SkillMenuClose();


