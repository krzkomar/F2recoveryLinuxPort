#pragma once

#define COUNTER_ROTATION_MS	250
#define COUNTER_NORMAL		0
#define COUNTER_YELLOW		120
#define COUNTER_RED		240
#define TEXT_BOX_W		130
#define TEXT_BOX_H		21
#define TEXT_BOX_SIZE		(TEXT_BOX_W * TEXT_BOX_H)

typedef struct 
{
    Obj_t *obj;
    char i02;
    char Usable;	// item is consumable or a device to use
    int i03;
    int i04;
    int i05;
    int ArtId;
} Iface_t;

typedef struct 
{
    int TextId;
    int Color;
    char *Img;
} Iface01_t;

/*
void IfacePanelDisable();
int  gIfcBarSlots[ 6 ];
Iface_t gIfcHandSlot[ 2 ];
CachePool_t *gIfcObj23;
CachePool_t *gIfcObj24;
CachePool_t *gIfcObj21;
int   gIfcUnk31;
char  *gIfcImg18;
CachePool_t *gIfcObj22;
char  *gIfcCmbtBtDnImg;
int   gIfcCmbtBtUpImg;
char  *gIfcImg19;
int   gIfcTurnUpImg;
char  gIfcSlotSurfDn[12596];
char  *gIfcTurnDnImg;
char  *gIfcImg20;
char  *gIfcImg13;
char  *gIfcImgSlotRaised;
char  *gIfcImgSlotGrayed;
char  *gIfcImg09;
char  *gIfcImg11;
char  *gIfcImg14;
char  *gIfcImgSlotPressed;
char  *gIfcImg12;
char  *gIfcImg10;
char  gIfcSlotSurfUp[ 188 * 67 ];
char  *gIfcImg08;
char  *gIfcImg07;
char  *gIfcImg06;
char  *gIfcSurf;
char  *gIfcImg01;
char  *gIfcImg03;
char  *gIfcImg04;
char  *gIfcImg05;
char  *gIfcImg02;
char  gIfcUnk06[452];
int gIfcUnk01 = 0;
int gIfcAnimate = 0;
int gIfcUiState = 0;
int gIfcPanelState = 0;
int gIfcBt01 = -1;
CachePool_t *gIfcObj01 = 0;
CachePool_t *gIfcObj02 = 0;
int gIfcBt02 = -1;
CachePool_t *gIfcObj03 = 0;
CachePool_t *gIfcObj04 = 0;
int gIfcBt08 = -1;
CachePool_t *gIfcObj05 = 0;
CachePool_t *gIfcObj06 = 0;
CachePool_t *gIfcObj07 = 0;
int gIfcBt03 = -1;
CachePool_t *gIfcObj08 = 0;
CachePool_t *gIfcObj09 = 0;
CachePool_t *gIfcObj10 = 0;
int gIfcBt04 = -1;
CachePool_t *gIfcObj11 = 0;
CachePool_t *gIfcObj12 = 0;
int gIfcBt05 = -1;
CachePool_t *gIfcObj13 = 0;
CachePool_t *gIfcObj14 = 0;
int gIfcBt06 = -1;
CachePool_t *gIfcObj15 = 0;
CachePool_t *gIfcObj16 = 0;
CachePool_t *gIfcObj17 = 0;
int gIfcHandSlotState = 0;
VidRect_t gIfcUnk25 = { 0x10B, 0x1A, 0x1C7, 0x5D };
int gIfcBt07 = -1;
CachePool_t *gIfcObj18 = 0;
CachePool_t *gIfcObj19 = 0;
CachePool_t *gIfcObj20 = 0;
int gIfcCombatState = 0;
VidRect_t gIfcUnk10 = { 0x244, 0x26, 0x27D, 0x60 };
int gIfcEndTurnBt = -1;
CachePool_t *gIfcTurnDnImgObj = 0;
CachePool_t *gIfcTurnUpImgObj = 0;
int gIfcUnk14 = -1;
CachePool_t *gIfcCmbtBtDnImgObj = 0;
CachePool_t *gIfcCmbtBtUpImgObj = 0;
void *gIfcImg22 = 0;
void *gIfcImg23 = 0;
char *gIfcImg24 = 0;
VidRect_t gIfcUnk17 = { 0x13C, 0x0E, 0x196, 0x13 };
char *gIfcImg21 = NULL;
Iface01_t gIfcIndBox[ 5 ] = {
int gIfcWin = -1;
int gIfcIndBoxWin = -1;
int gIfcCurrentHP = 0;
int gIfcStateHP = COUNTER_RED;
int gIfcCurrentAC = 0;
*/
extern int gIfcTimeout;


int IfaceInit();
void IfaceReset();
void IfaceClose();
int IfacePanelLoad( xFile_t *fh );
int IfacePanelSave( xFile_t *fh );
void IfacePanelEnable();
void IfacePanelDisable();
int IfaceGetPanelState();
void IfaceUiEnable();
void IfaceUiDisable();
int IfaceGetUiState();
void IfaceUnk09();
void IfaceRenderHP( int mode );
void IfaceResetAC( int Animate );
void IfaceRenderAP( int ap, int extra );
int IfaceGetWeaponDsc( int *Slot, int *Ranged );
int IfaceHandSlotUpdate( int a1, int LhandArg, int RhandArg );
int IfaceToggleItem( int a1 );
int IfaceUnk16( int *a1, int *a2 );
int IfaceUnk17();
void IfaceHandSlotSelect();
int IfaceGetSelectedHand();
int IfaceGetHandObject( Obj_t **obj );
void IfaceAmmoGauge();
void IfaceCombatOpen( int a1 );
void IfaceCombatClose( int a1 );
void IfaceUnk24();
void IfaceUnk25();
void IfaceHandSlotReset();
void IfaceRenderHandSlot();
//static void IfaceRenderHandSlotCb();
void IfaceAnimationEnd();
void IfcAnimateSlot( int a1, int a2 );
int IfaceTurnBtOpen();
void IfaceTurnBtClose();
int IfaceCmbtBtOpen();
void IfaceCmbtBtClose();
void IfaceRenderAmmo( int a1, int a2);
int IfaceUnk36();
void IfaceRenderCounter( int Xpos, int Ypos, int CurrVal, int NewVal, int ColorSel, unsigned int Fps );
void IfaceDestroy();
int IfaceIndicatorBoxInit();
void IfaceIndicatorBoxClose();
void IfaceIndicatorBoxDestroy();
int IfaceIndicatorBoxUpdate();
//static int IfaceSortCb( int *p1, int *p2 );
void IfaceIndicatorBoxRender( int a1 );
int IfaceIndocatorBoxAddEffect( int Effect );
void IfaceIndicatorBoxShow();
void IfaceIndicatorBoxHide();


