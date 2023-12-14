#pragma once

#define PROTO_TEMPLATE 		0x1000000
#define PROTO_1	0x1000000

// Proto_t flags
#define PRFLG_INVISIBLE		0x00000001	// object is not rendered
#define PRFLG_FLAT		0x00000008	// Flat ( rendered first, just after tiles )
#define PRFLG_NO_BLOCK		0x00000010	// NoBlock ( doesn't block the tile )
#define PRFLG_LIGHTED		0x00000020	// object is lighted (by another object) (?)
#define PRFLG_MULTIHEX		0x00000800	// ?
#define PRFLG_NO_HL		0x00001000	// No Highlight ( doesn't highlight the border; used for containers )
#define PRFLG_TR_RED		0x00004000
#define PRFLG_TR_NONE		0x00008000	// opaque
#define PRFLG_TR_WALL		0x00010000
#define PRFLG_TR_GLASS		0x00020000
#define PRFLG_TR_STEAM		0x00040000
#define PRFLG_TR_ENERGY		0x00080000
#define PRFLG_WORN_RHAND	0x01000000
#define PRFLG_WORN_LHAND	0x02000000
#define PRFLG_WORN_ARMOR	0x04000000
#define PRFLG_WALL_TR_END	0x10000000	// WallTransEnd (changes transparency egg logic. Set for walls with 1144 - 1155 pid )
#define PRFLG_LIGHTTHRU		0x20000000
#define PRFLG_SHOOTTHRU		0x80000000

#define PR_OL_NONE		0x00	// no outline
#define PR_OL_RED		0x01	// red outline
#define PR_OL_YEL		0x20	// yellow outline

typedef enum{
    TYPE_ITEM, // 0
    TYPE_CRIT, // 1
    TYPE_SCEN, // 2
    TYPE_WALL, // 3
    TYPE_TILE, // 4
    TYPE_MISC, // 5
    TYPE_IFCE, // 6
    TYPE_INVE, // 7
    TYPE_HEAD, // 8
    TYPE_BCKG, // 9
    TYPE_SKIL, // 10
    TYPE_PROTO_ALL // 11
} ProtoType_e;

enum{
    PR_ITEM_ARMOR = 0,	// 0
    PR_ITEM_CONTAINER,	// 1
    PR_ITEM_DRUG,	// 2
    PR_ITEM_WEAPON,	// 3
    PR_ITEM_AMMO,	// 4
    PR_ITEM_MISC,	// 5
    PR_ITEM_KEY		// 6
};

// extended flags
#define PR_CONTAINER_OPEN	0x008000	// ? container is locked
#define PR_ITEMF_HIDDEN		0x080000	// hidden item
#define PR_ACTF_USE		0x000008	// can be used
#define PR_ACTF_USEON		0x000010	// can be used on anything
#define PR_ACTF_PICKUP		0x000080	// can be picked up
#define PR_WEAPONF_BIG		0x000001	// big gun
#define PR_WEAPONF_2HND		0x000001	// two-handed weapon

// attack modes
enum{
    PR_ATT_NONE,
    PR_ATT_PUNCH,
    PR_ATT_KICK,
    PR_ATT_SWING,
    PR_ATT_THRUST,
    PR_ATT_THROW,
    PR_ATT_FIRE_SINGLE,
    PR_ATT_FIRE_BURST,
    PR_ATT_FLAME
};

// Material IDs
enum{
    PR_MAT_GLASS,
    PR_MAT_METAL,
    PR_MAT_PLASTIC,
    PR_MAT_WOOD,
    PR_MAT_DIRT,
    PR_MAT_STONE,
    PR_MAT_CEMENT,
    PR_MAT_LEATHER
};

// Types of scenery
enum{
    PR_SCN_DOOR,	// 0
    PR_SCN_STAIRS,	// 1
    PR_SCN_ELEVATOR,	// 2
    PR_SCN_LADDER_BOT,	// 3
    PR_SCN_LADDER_TOP,	// 4
    PR_SCN_GENERIC	// 5
};

/*
 Critt.BaseStat[x]:
  18 - subtype -> PR_ITEM_*
  19 - Material ID
  20 - Size
  21 = weight
  22 - cost
*/


typedef struct 
{
    int Pid;
    int TextID; // Normally ObjectID multiplied by 100. This is the line number of the object name & description (TextID+1) in the appropriate message file.
    int ImgId;
    int LtRad;	// light radius
    int LtInt;	// light intensity
    int Flags;	// flags PRFLG_*
    int FlgExt;	// extflags
    int AttackMode;
    Critter_t Critt;
    int i09;
    int AiPacketNum;
    int i11;
    int i12;
    int i13;
    int i14;
    int i15;
    int i16;
    int i17;
} Proto_t;

typedef struct _ProtoObj_t // size of 0x48
{
  Proto_t 		*Objs[ 16 ];
  int 			Count;
  struct _ProtoObj_t 	*next;
} ProtoObj_t;

typedef struct // size of 16
{
    ProtoObj_t *ObjRoot;
    ProtoObj_t *Current;
    int 	Cnt;
    int 	Active;
} ProtoLib_t;

/*******/

typedef struct 
{
  unsigned int i01;
} ProtoItem6_t;

typedef struct 
{
  unsigned int i01;
  unsigned int i02;
  unsigned int i03;
} ProtoItem5_t;


typedef struct 
{
  unsigned int i01;
  unsigned int i02;
  unsigned int i03;
  unsigned int i04;
  unsigned int i05;
  unsigned int i06;
} ProtoItem4_t;

typedef struct 
{
  unsigned int i01;
  unsigned int i02;
  unsigned int i03;
  unsigned int i04;
  unsigned int i05;
  unsigned int i06;
  unsigned int i07;
  unsigned int i08;
  unsigned int i09;
  unsigned int i10;
  unsigned int i11;
  unsigned int i12;
  unsigned int i13;
  unsigned int i14;
  unsigned int i15;
  unsigned int i16;
  char b17;
} ProtoItem3_t;


typedef struct 
{
  unsigned int i01;
  unsigned int i02;
  unsigned int i03;
  int i04[3];
  unsigned int i05;
  int i06[3];
  unsigned int i07;
  int i08[3];
  unsigned int i09;
  unsigned int i10;
  unsigned int i11;
} ProtoItem2_t;

typedef struct 
{
  unsigned int i01;
  unsigned int i02;
} ProtoItem1_t;

typedef struct 
{
  unsigned int i01;
  int i02[7];
  int i03[7];
  int i04;
  unsigned int i05;
  unsigned int i06;
  int i07;
  int i08;
  int i09;
  int i10;
  int i11;
  int i12;
} ProtoItem0_t;

typedef union 
{
  ProtoItem0_t Type0;
  ProtoItem1_t Type1;
  ProtoItem2_t Type2;
  ProtoItem3_t Type3;
  ProtoItem4_t Type4;
  ProtoItem5_t Type5;
  ProtoItem6_t Type6;
} ProtoItemsData_t;

/****/

typedef struct 
{
  unsigned int i01;
  unsigned int i02;
} ProtoSceneryData_t;


//ProtoLib_t gProtoObjLib[ 11 ];
//int gProtoAllocSize[ 11 ];
//int gProtoUnk02;
//Proto_t gProtoDude;
//int gProtoImgId;
//Obj_t *gObjDude;
//Proto_t gProtos;
//char *gProtoFilePath;
//int gProtoVirgin1;
//int gProtoError;
//char *gProtoPerkNames[120];
//char *gProtoStatNames[40];
//Msg_t gProtoMsg[ 6 ];
//char *gProtoUnk105[2];
//char *gProtoUnk102[6];
extern Msg_t gProtoMessages;
//char *gProtoUnk106[8];
extern char *gProtoUnk100[4];
//char *gProtoUnk101[7];
//char *gProtoUnk103[7];
//char *gProtoUnk104[19];

int ProtoUnk10( char *path, unsigned int Pid );
//int ProtoUnk09( unsigned int Pid, char *a2 );
//int ProtoUnk08( unsigned int Pid, char *a2, int *a3 );
int ProtoGetFName( unsigned int Idx, char *fname );
int ProtoGetTypeSize( ProtoType_e Type );
int ProtoOpenable( unsigned int Pid );
int ProtoDrugUsable( unsigned int Pid );
int ProtoUnk05( unsigned int Pid );
int ProtoItemAccessible( unsigned int Pid );
char *ProtoGetMsg( int Pid, int MsgId );
char *ProtoGetObjName( unsigned int Pid );
char *ProtoGetDudeDsc( int Pid );
int ProtoInitItem( Proto_t *dude, int Pid );
int ProtoUnk02( Proto_t *dude, ProtoType_e Type );
int ProtoInitCritter( Proto_t *proto, int ImgPid );
void ProtoDudeEffectReset( Obj_t *dude );
int ProtoLoadB( Obj_t *dude, xFile_t *fh );
int ProtoSaveB( Obj_t *dude, xFile_t *fh );
int ProtoLoadObj( Obj_t *dude, xFile_t *fh );
int ProtoLoadA( xFile_t *fh, Obj_t *dude, int a3 );
int ProtoSaveA( xFile_t *fh, Obj_t *dude, int a3 );
int ProtoSaveDude( Obj_t *dude, xFile_t *fh );
int ProtoEffectApply( Obj_t *dude );
int ProtoEffectInit( Obj_t *dude );
void ProtoDudeImgInit();
int ProtoDudeInit( char *fname );
int ProtoInitScenery( Proto_t *proto, int Pid );
int ProtoUnk01( Proto_t *proto, ProtoType_e Type );
int ProtoInitWall( Proto_t *proto, int Pid );
int ProtoInitTile( Proto_t *proto, int Pid );
int ProtoInitMisc( Proto_t *proto, int Pid );
int ProtoDuplicate( unsigned int Pid1, unsigned int Pid2 );
int ProtoTestFlg( Proto_t *proto, int flg );
int ProtoDataMember( unsigned int Pid, unsigned int MembId, void **DataMember );
int ProtoInit();
int ProtoFreePremade();
void ProtoClose();
int ProtoObjCount();
int ProtoLoadItems( ProtoItemsData_t *Data, int Type, xFile_t *fh );
int ProtoLoadScenery( int *Data, int Type, xFile_t *fh );
int ProtoLoadPrototype( Proto_t *Pt, xFile_t *fh);
int ProtoSaveItems( ProtoItemsData_t *it, int Type, xFile_t *fh );
int ProtoSaveScenery( ProtoSceneryData_t *sc, int Type, xFile_t *fh );
int ProtoSaveCritter( Proto_t *pt, xFile_t *fh );
void ProtoSaveCritterFile( unsigned int Pid );
int ProtoLoadCritterFile( unsigned int Pid, Proto_t **Pt );
int ProtoAlloc( unsigned int Type, void **pObj );
int ProtoCreate( int *pPid, ProtoType_e Type );
unsigned int ProtoRemove( unsigned int Pid );
void ProtoFreeBlock( int Type );
int ProtoFreeCategory( int Cat );
void ProtoResetTypes();
int ProtoGetObj( int Pid, Proto_t **pObj );
int ProtoInc( int Type );
int ProtoDec( int Type );
int ProtoActive( int Type );
int ProtoFindArtMatch( int Pid );
int ProtoReset();


