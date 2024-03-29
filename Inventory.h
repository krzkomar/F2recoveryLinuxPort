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
#define INV_ACT_PUSH		9

typedef struct 
{
  int ArtId;
  int Width;
  int Height;
  int Xpos;
  int Ypos;
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

void InvSelectCritter( Obj_t *Critter, int Pid );
void InvSelectMain();
int  InvMsgOpen();
int  InvMsgClose();
void InvMenu();
int  InvMenuCreate( unsigned int idx );
void InvMenuClose( int a1 );
void InvBpUpdate( int BpOffset, int Picked, int mode );
void InvItemList( int a1, int a2, ObjBox_t *box, int a4 );
void InvPrintQuantity( Obj_t *obj, int Quantity, char *surf, int Pitch, int Picked );
void InvBodyUpdate( int a1 ,int a2 );
int  InvInit();
void InvSetInfoMode( int a1 );
void InvLight( Obj_t *item );
void InvEquip( int sel, int SlotOffset );
void InvWearItem( Obj_t *PickedPtr, Obj_t **Slot, Obj_t **ptr, int Pos );
void InvUpdateStatistics( Obj_t *critter, Obj_t *NegativeItem, Obj_t *PositiveItem );
void InvSetBodyImg();
int  InvMenuBackPack( Obj_t *a1 );
// get rhand obj with flag worn
Obj_t *InvGetRHandObj( Obj_t *obj );
// get lhand obj with flag worn
Obj_t *InvGetLHandObj( Obj_t *obj );
// get armor with flag worn
Obj_t *InvGetArmorObj( Obj_t *obj );
int InvIsObjInInventory( Obj_t *obj, int Pid );
Obj_t *InvGetItem( Obj_t *Obj, int Pid );
int  InvGetQuantityTot( Obj_t *obj, int Pid );
void InvReset( Obj_t *crit );
void InvStatsUpdate();
Obj_t *InvSearchObjByType( Obj_t *Obj, int ObjType, int *Position );
Obj_t *InvSearchObjByPid( Obj_t *Obj, int ObjPid );
Obj_t *InvUnk27( Obj_t *a1, int a2 );
int InvUnk28( Obj_t *a1, Obj_t *a2, int a3);
int InvUnk29( Obj_t *a1, Obj_t *a2, int a3, int a4 );
int InvUnwield( Obj_t *a1, int a2 );
int InvUnk30( Obj_t *a1, int a2, int a3 );
// returns current quantity of item in inventory
int  InvPickItem( int sel, Obj_t **PickedItem, Obj_t **BoxObj, Obj_t ***PickPtrObj );
void InvDisplMsg( char *strmsg );
void InvInfo( Obj_t *Critter, Obj_t *Item );
void InvActionMenu( int sel, int mode );
int  InvMenuSteal( Obj_t *Critter, Obj_t *Obj2 );
void InvStealAttempt( Obj_t *Thief, Obj_t *Npc );
int  InvUnk38( Obj_t *a1, int a2, Obj_t *a3, int a4 );
int  InvStackPrice( Obj_t *dude, Obj_t *a2 );
int  InvBarterDo( Obj_t *dude, Obj_t *Item, Obj_t *a3, Obj_t *a4 );
void InvUnk41( Obj_t *dude, int quantity, int a3, int a4, Obj_t *a1, Obj_t *a2, int a7 );
void InvUnk42( Obj_t *Item, int quantity, int a3, Obj_t *a4, Obj_t *a1, int a6 );
void InvBarterStack( int win, Obj_t *box, Obj_t *obj, int a4 );
int  InvMenuBarter(int eax0, Obj_t *a2, Obj_t *a3, Obj_t *a4, int a5 );
void InvUnk45( int a1, int a2 );
void InvUnk46( int sel, int mode );
int  InvPickUpBoxQuantity( Obj_t *Critter, Obj_t *Item1, int Quantity, Obj_t **Item2, int quantity );
int  InvLoadAmmo( Obj_t *item0, Obj_t *item1, Obj_t **item2, int quantity, int sel );
void InvDrawCnt( int Value, int mode );
int  InvPopUpDlg( int mode, Obj_t *Obj, int quantity );
int  InvPopUpDlgCreate( int mode, Obj_t *Item );
void InvPopUpClose( int Mode );
int  InvSetTimer( Obj_t *a1 );


