#pragma once

#define ITEM_CLASS_RANGED	4

typedef struct 
{
  int i01[3]; // Pid,,
  int i04;
  int i05;
  int i06;
  int i07;
} ItemEv01_t;

typedef struct 
{
  int i01;
  int i02;
  int i03;
} ItemEv02_t;

typedef struct 
{
  int Pid;
  int GVarId;
  int i03;
} Item01_t;

//int gItemUnk03[ 9 ]
//int gItemUnk678[ 9 ]
//int gItemClass[ 9 ]
//Item01_t gItemGVar[ 9 ]
//char *gItemUnk02 
//int gItemLightLvl
//Msg_t gItemMsg;
//Obj_t *gItemUnk99;
//Obj_t *gItemUnk98;
//int gItemGVarId;
//int gItemGridLight[ 3*200*200 ];

int ItemInit();
int ItemSubNull0();
void ItemClose();
int ItemFSave( xFile_t *fh );
int Item09( Obj_t *dude1, Obj_t *dude2, int a3 );
int ItemAdd( Obj_t *dude, Obj_t *item, int Quantity );
int ItemUseItem( Obj_t *Obj1, Obj_t *Obj2, int QuantityMax );
int Item11( Obj_t *a1, Obj_t *a2 );
int Item12( Obj_t *a1, Obj_t *a2, Obj_t *a3, int a4, int a5 );
int Item13( Obj_t *a1, Obj_t *a2, Obj_t *a3, int a4 );
int Item14( Obj_t *a1, Obj_t *a2, Obj_t *a3, int a4 );
int Item15( Obj_t *a1, Obj_t *a2 );
int Item16( Obj_t *a1, Obj_t *a2 );
int Item17( Obj_t *a1 );
int Item18( Obj_t *item, int edx0 );
int ItemStack( Obj_t *Container, Obj_t *Item );
char *ItemGetName( Obj_t *Obj );
char *ItemGetDsc( Obj_t *obj );
int ItemGetObjType( Obj_t *obj );
int Item23( Obj_t *obj );
int Item24( Obj_t *obj );
int ItemGetItemWeight( Obj_t *item );
int Item26( Obj_t *obj );
int Item27( Obj_t *obj );
int ItemGetBackPackWeight( Obj_t *obj );
int Item29( Obj_t *obj );
int ItemGetArtId( Obj_t *obj );
Obj_t *ItemGetSlotItem( Obj_t *obj, int a2 );
int ItemGetSlotApCost( Obj_t *obj, int Slot, int a3 );
int Item33( Obj_t *obj, Obj_t *a2 );
int Item34( Obj_t *obj );
Obj_t *Item35( Obj_t *obj, Obj_t *a2, int a3 );
int Item36( Obj_t *obj );
int ItemGetClass( Obj_t *item, int slot );
int ItemGetSkill( Obj_t *obj, int a2 );
int ItemGetHitChance( Obj_t *obj, int a2 );
int ItemGetWeaponDmg( Obj_t *Weapon, int *pMin, int *pMax );
int Item41( Obj_t *obj, int a2 );
int ItemGetWeaponBase( Obj_t *obj1, Obj_t *obj2 );
#define ITEM_WPN_FIRE	2


int ItemWeaponTwoHanded( Obj_t *obj );
int Item44( Obj_t *obj, int a2 );
int Item45( Obj_t *obj, int a2 );
int ItemGetPackQuantity( Obj_t *obj );
int ItemGetAmmo( Obj_t *obj );
int Item48( Obj_t *obj );
void ItemSetCharges( Obj_t *Obj, int Charges );
int Item50( Obj_t *Obj1, Obj_t *Obj2 );
int ItemUnk01( Obj_t *obj1, Obj_t *obj2 );
int Item51( Obj_t *obj1, Obj_t *obj2 );
int ItemGetRange( Obj_t *Obj, int Slot );
int ItemGetAPCost( Obj_t *obj, int Slot, int a3 );
int Item54( Obj_t *obj );
int Item55( Obj_t *obj );
int Item56( Obj_t *obj );
int Item57( Obj_t *obj );
int Item58( Obj_t *obj );
int Item59( Obj_t *obj );
int ItemGetAmmoId( Obj_t *obj );
int Item61( Obj_t *obj );
int Item62( Obj_t *obj, int a2 );
int ItemCharged( Obj_t *obj );
Obj_t *ItemUnload( Obj_t *obj );
int Item65( Obj_t *obj );
int Item66( Obj_t *obj );
int Item67( Obj_t *obj, int *n );
int Item68( Obj_t *obj );
int Item69( Obj_t *obj, int a2 );
int Item70();
int Item71();
int Item72( Obj_t *obj );
int Item73( Obj_t *obj );
int Item74( Obj_t *obj );
int Item75( Obj_t *obj );
int ItemGetAC( Obj_t *obj );
int ItemGetDmgRes( Obj_t *obj, int Idx );
int ItemGetDmgThr( Obj_t *obj, int idx );
int Item79( Obj_t *obj );
int Item80( Obj_t *obj );
int Item81( Obj_t *obj );
int ItemCharges( Obj_t *obj );
int ItemGetCharges( Obj_t *obj );
int ItemUseCharges( Obj_t *obj, int Charges );
int Item85( Obj_t *obj );
int Item86( Obj_t *obj );
int Item87( Obj_t *obj );
int ItemRecharge( Obj_t *Obj1, Obj_t *Obj2 );
int ItemMotSensCharge( Obj_t *obj );
int ItemDeviceUse( Obj_t *obj );
int Item90( Obj_t *obj );
int ItemDeviceOn( Obj_t *obj );
int ItemDeviceOff( Obj_t *obj );
int ItemDeviceOffCb( Obj_t *obj );
int ItemStealthBoyOn( Obj_t *Obj );
int ItemStealthBoyOff( Obj_t *Obj1, Obj_t *Obj2 );
int Item94( Obj_t *Obj );
int Item95( Obj_t *obj );
int ItemGetAcMod( Obj_t *Obj );
int ItemGetDrMod( Obj_t *Obj ); // damage resistance
int ItemGetDmgMinMod( Obj_t *Obj );
int ItemGetDmgMaxMod( Obj_t *Obj );
int Item100( Obj_t *Obj );
int Item101( Obj_t *Critter, Obj_t *Obj2, int duration, int *a4, int *a5 );
void ItemChemUse( Obj_t *obj, int *stats, int a3, int *a4 );
int Item102( Obj_t *Obj, int Pid );
int ItemUnk07( Obj_t *Obj1, Obj_t *Obj2 );
int Item103( Obj_t *obj, ItemEv01_t *eff );
int Item105( Obj_t *Obj, ItemEv01_t *eff );
int ItemLoadEffects( xFile_t *fh, void **a2 );
int ItemSaveEffects( xFile_t *fh, int *a2 );
int ItemSchedEffect( Obj_t *obj, int a2, int RecoveryTime, int EffectId, int a5 );
int Item109( Obj_t *obj, EvEntry_t *effect );
int Item110( Obj_t *obj, EvEntry_t *Effect );
int ItemWithdrawal( Obj_t *obj, EvEntry_t *effect );
int ItemWithdrawalLoad( xFile_t *fh, EvEntry_t **wd );
int ItemWithdrawalSave( xFile_t *fh, EvEntry_t *event );
int ItemWithdrawalStart( Obj_t *Obj, int EffectId, int a3 );
void ItemWithdrawalEnd( Obj_t *Obj, int EffectId );
int ItemGetGVarId( int Pid );
void ItemGVarSet( int ObjPid );
void ItemGVarClr( int ObjPid );
int ItemGVarDefined( int Pid );
int ItemGetMoneyAll( Obj_t *obj );
int ItemBarter( Obj_t *obj, int Value );
int ItemGetMoney( Obj_t *Obj );
int ItemSetMoney( Obj_t *obj, int Value );
int ItemGridLightInit();
int ItemMapGetLight();
void ItemMapModifyLight( int LightModifier, int TilesUpdate );
void ItemMapSetLight( int Intensity, int TilesUpdateFlg );
int ItemGridGetLightA( int MapLvl, int GridPos );
int ItemGridGetLight( unsigned int MapLvl, unsigned int GridPos );
void ItemGridSetLight( int MapLvl, int GridPos, int Intensity );
void ItemGridLightInc( unsigned int MapLvl, unsigned int Pos, int Val );
void ItemGridLightDec( unsigned int MapLvl, unsigned int Pos, int Val );
void ItemGridLightReset();


