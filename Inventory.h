#pragma once

#define INV_AP_ACCESS_COST 4

#define INV_ACT_CANCEL		0
#define INV_ACT_THROW		1
#define INV_ACT_TAKE		2
#define INV_ACT_INSPECT		3
#define INV_ACT_TURN		4
#define INV_ACT_TALK		5
#define INV_ACT_USE		6
#define INV_ACT_UNLOAD		7
#define INV_ACT_SKILL		8
#define INV_ACT_9		9

typedef struct 
{
  int Xpos;
  int Ypos;
  int ArtId;
  int Width;
  int Height;
} Inv01_t;

typedef struct 
{
  ArtFrmHdr_t *Art;
  char *Data;
  int Width;
  int Height;
  int CenterX;
  int CenterY;
  CachePool_t *Obj;
} Inv02_t;


/*
CachePool_t *gInvQaDlgImgObj[ 8 ];
CachePool_t *gInvUnk20[12];
int gInvUnk63[10];
Msg_t gInvMsg;
Obj_t *gInvUnk39[10];
int gInvUnk04[10];
Obj_t *gInvParty[ 10 ];
int gInvQdlgWin;
int gInvUnk66;
int gInvUnk77;
int gInvUnk78;
ObjContainer_t *gInvUnk79;
Inv02_t gInvMseCursor[5];
ObjContainer_t *gInvUnk84;
void (*gInvInfoCb)(char *);
int gInvFocusRegionId;
int gInvInfoMode;
Obj_t *gInvUnk65;
int gInvUnk40;
ObjContainer_t *gInvUnk85;
int gInvUnk02;
Obj_t *gInvArmorObj;
Obj_t *gInvLHandObj;
int gInvBodyImgId;
ObjContainer_t *gInvBackPack;
int gInvWin;
Obj_t *gInvRHandObj;
int gInvUnk05;
int gInvUnk16;
int gInvUnk15;
ObjContainer_t *gInvUnk62;
int gInvUnk18;
int gInvUnk08 = 6;
Obj_t *gInvSelectedDude = NULL;
int gInvSelectedPid = -1;
Inv01_t gInvArt[ 5 ] = {
int gInvUnk83 = 0;
int gInvUpArrow = -1;
int gInvDnArrow = -1;
int gInvUnk36 = -1;
int gInvUnk37 = -1;
int gInvSysTimer = 0;
int gInvArmorThumbFrame = 0;
int gInvUnk42[5] = { 286, 250, 282, 283, 266 };
Obj_t *gInvUnk52 = NULL;
int gInvMenuIconConsumable[4] = { 
int gInvUnk90[3] = { 
int gInvUnk91[3] = { 
int gInvUnk92[2] = { 
int gInvMenuIconWeaponOwned[4] = { 
int gInvMenuIconWeapon[3] = { 
int gInvUnk80 = 0;
int gInvBarterDiscount = 0;
int gInvUnk12[ 7 ] = { 35, 9, 17, 18, 19, 20, 23 };
int gInvUnk13[ 7 ] = { 7, -1, 24, 25, 26, 27, 30 };
int gInvUnk100[ 4 ] = { 122, 123, 124, 125 };
*/


void InvSelectCritter( Obj_t *Critter, int Pid );
void InvSelectMain();
int InvMsgOpen();
int InvMsgClose();
void InvMenu();
int InvMenuCreate( unsigned int idx );
void InvMenuClose( int a1 );
void InvBpUpdate( int BpOffset, int Picked, int mode );
int InvUnk03( int a1, int a2, ObjBox_t *box, int a4 );
void InvPrintQuantity( Obj_t *obj, int Quantity, char *surf, int Pitch, int Picked );
void InvBodyUpdate( int a1 ,int a2 );
int InvInit();
void InvSetInfoMode( int a1 );
//void InvEnterRegionCb( int nu, int RegionNb );
//void InvLeaveRegionCb();
void InvLight( Obj_t *item );
void InvEquip( int sel, int SlotOffset );
void InvWearItem( Obj_t *PickedPtr, Obj_t **Slot, Obj_t **ptr, int Pos );
void InvUpdateStatistics( Obj_t *critter, Obj_t *NegativeItem, Obj_t *PositiveItem );
void InvSetBodyImg();
int InvMenuBackPack( Obj_t *a1 );
// get rhand obj with flag worn
Obj_t *InvGetRHandObj( Obj_t *obj );
// get lhand obj with flag worn
Obj_t *InvGetLHandObj( Obj_t *obj );
// get armor with flag worn
Obj_t *InvGetArmorObj( Obj_t *obj );
int InvIsObjInInventory( Obj_t *obj, int Pid );
Obj_t *InvGetItem( Obj_t *Obj, int Pid );
int InvGetQuantityTot( Obj_t *obj, int Pid );
void InvReset( Obj_t *crit );
void InvStatsUpdate();
Obj_t *InvSearchObjByType( Obj_t *Obj, int ObjType, int *Position );
Obj_t *InvSearchObjByPid( Obj_t *Obj, int ObjPid );
Obj_t *InvUnk27( Obj_t *a1, int a2 );
int InvUnk28( Obj_t *a1, Obj_t *a2, int a3);
int InvUnk29( Obj_t *a1, Obj_t *a2, int a3, int a4 );
int InvUnk31( Obj_t *a1, int a2 );
int InvUnk30( Obj_t *a1, int a2, int a3 );
// returns current quantity of item in inventory
int InvPickItem( int sel, Obj_t **PickedItem, Obj_t **BoxObj, Obj_t ***PickPtrObj );
void InvDisplMsg( char *strmsg );
void InvInfo( Obj_t *Critter, Obj_t *Item );
void InvActionMenu( int sel, int mode );
int InvMenuSteal( Obj_t *Critter, Obj_t *Obj2 );
void InvStealAttempt( Obj_t *Thief, Obj_t *Npc );
int InvUnk38( Obj_t *a1, int a2, Obj_t *a3, int a4 );
int InvStackPrice( Obj_t *dude, Obj_t *a2 );
int InvBarterDo( Obj_t *dude, Obj_t *Item, Obj_t *a3, Obj_t *a4 );
void InvUnk41( Obj_t *dude, int quantity, int a3, int a4, Obj_t *a1, Obj_t *a2, int a7 );
void InvUnk42( Obj_t *Item, int quantity, int a3, Obj_t *a4, Obj_t *a1, int a6 );
void InvUnk43( int win, Obj_t *box, Obj_t *obj, int a4 );
int InvMenuBarter(int eax0, Obj_t *a2, Obj_t *a3, Obj_t *a4, int a5 );
void InvUnk45( int a1, int a2 );
void InvUnk46( int sel, int mode );
int InvPickUpBoxQuantity( Obj_t *Critter, Obj_t *Item1, int Quantity, Obj_t **Item2, int quantity );
int InvLoadAmmo( Obj_t *item0, Obj_t *item1, Obj_t **item2, int quantity, int sel );
void InvDrawCnt( int Value, int mode );
int InvPopUpDlg( int mode, Obj_t *Obj, int quantity );
int InvPopUpDlgCreate( int mode, Obj_t *Item );
void InvPopUpClose( int Mode );
int InvSetTimer( Obj_t *a1 );


