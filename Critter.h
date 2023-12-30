#pragma once

// Critter_t flags:
#define CRITT_SNEAK	0x01
#define CRITT_RADIATED	0x02
#define CRITT_LEVEL	0x08
#define CRITT_ADDICT	0x10

// combat result flag
#define CRITT_EYE_DMG		0x80
#define CRITT_CRIP_RARM		0x20 // 0x40
#define CRITT_CRIP_LARM		0x10
#define CRITT_CRIP_RLEG		0x08
#define CRITT_CRIP_LLEG		0x04

/*
typedef struct {
    int Ac;		// 0 armor class
    int DmgMin;		// 1 dmg min for weapon and pack quantity for ammo
    int DmgMax;		// 2 dmg max for weapon
    int Base;		// 3 weapon base 
    int RangeR;		// 4 right hand range
    int RangeL;		// 5 left hand range
    int i07;
    int i08;
    int ApR;		// 8 Ap cost for right hand
    int ApL;		// 9 Ap cost for left hand
    int i11;
    int i12;
    int i13;
    int i14;
    int i15;
    int Pack;		// 15 pack quantity
    int i17;		// 16
    int SubType;	// 17
    int MaterialId;	// 18
    int Size;		// 19
    int Weight;		// 20
    int Cost;		// 21
    int ArtId;		// 22
    int i24;
    int i25;
    int i26;
    int i27;
    int i28;
    int i29;
    int i30;
    int i31;
    int i32;
    int i33;
    int i34;
    int i35;
} ItemStat_t;
*/

typedef struct 
{
    union{
	 int Type;
	 int ScrId;
    };
//    union{
	int BaseStat[ 35 ];
//	ItemStat_t Item;
//    };
  int BoostStat[35];
  int Skills[18];
  int ProtoID;
  int i91;
  int Gender;
  int TypeNameID;
} Critter_t;


//Msg_t gCrMsg;
//char gCrName[ 32 ];
//int  gCrUnk02;
//int  gCrKillStats[ 19 ];
//char *gCrName;
//int gCrUnk09;

int  CritterInit();
void CritterReset();
void CritterClose();
int  CritterLoad( xFile_t *fh );
int  CritterSave( xFile_t *fh );
void CritterDuplicate( Critter_t *p1, Critter_t *p2 );
char *CritterGetName( Obj_t *dude );
int  CritterSetName( char *name );
void CritterNameInit();
int  CritterGetHp( Obj_t *dude );
void CritterHeal( Obj_t *dude, int damage );
int  CritterPoisoned( Obj_t *dude );
void CritterPoisonInc( Obj_t *dude, int val );
int  CritterPoison( Obj_t *dude, MsgLine_t *MsgLine );
int  CritterRadiated( Obj_t *dude );
void CritterRadInc( Obj_t *dude, int Val );
void CritterRadSetDose( Obj_t *dude );
void CritterRadUnk01( int *a1 );
void CritterUnk11( Obj_t *dude, int *a2 );
void CritterRadApply( Obj_t *dude, int a2, int a3);
void CritterRadUnk02( Obj_t *dude, int *a2 );
int  CritterLoadUnk01( xFile_t *fh, int **dat );
int  CritterSaveUnk01( xFile_t *fh, int *dat );
int  CritterUnk03( Obj_t *dude );
int  CritterKillStatReset();
void CritterKillStatInc( int idx );
int  CritterGetKillStat( int idx );
int  CritterLoadKillStats( xFile_t *fh );
int  CritterSaveKillStats( xFile_t *fh );
int  CritterGetGender( Obj_t *dude );
char *CritterKillTypeName( int TxtId );
char *CritterGetKillTypeDsc( int TxtId );
int  CritterHit1( Obj_t *dude, int a2 );
int  CritterUnk26( int n );
int  CritterKill( Obj_t *dude, int a2 );
int  CritterUnk27( Obj_t *dude );
int  CritterCanTalk( Obj_t *dude );
int  CritterIsDead( Obj_t *dude);
int  CritterUnk30( Obj_t *dude );
int  CritterUnk31( Obj_t *dude );
int  CritterGetBodyType( Obj_t *pObj );
int  CritterLoadA( Critter_t *cr, char *fname );
int  CritterLoadAllStats( char *fname );
int  CritterLoadFile( xFile_t *fh, Critter_t *Data );
int  CritterSaveStats( Critter_t *cr, char *fname );
int  CritterSaveAllStats( char *fname );
int  CritterSaveFile( xFile_t *fh, Critter_t *cr );
void CritterUnk36( char a1 );
void CritterUnk37( char a1 );
void CritterUnk38( char a1 );
int  CritterUnk39( char Effect );
int  CritterUnk40();
void CritterUnk41();
int  CritterUnk42();
int  CritterUnk43( Obj_t *dude );
void CritterUnk44( Obj_t *dude );
int  CritterUnk45( Obj_t *dude1, Obj_t *dude2 );
int  CritterUnk46();
int  CritterUnk47( Obj_t *dude, int a2 );
int  CritterOverload( Obj_t *dude );
int  CritterUnk49( Obj_t *dude );
int  CritterGetInjure( int Pid, int mask );
int  CritterSetInjure( Obj_t *dude, int mask );
int  CritterClrInjure( Obj_t *dude, int mask );
int  CritterToggleInjure( Obj_t *dude, int mask);



