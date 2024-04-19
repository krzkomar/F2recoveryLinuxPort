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
    int i05;		//
    int ArtId;
} Iface_t;

typedef struct 
{
    int TextId;
    int Color;
    char *Img;
} Iface01_t;

extern int gIfcTimeout;

int  IfaceInit();
void IfaceReset();
void IfaceClose();
int  IfacePanelLoad( xFile_t *fh );
int  IfacePanelSave( xFile_t *fh );
void IfacePanelEnable();
void IfacePanelDisable();
int  IfaceGetPanelState();
void IfaceUiEnable();
void IfaceUiDisable();
int  IfaceGetUiState();
void IfaceUnk09();
void IfaceRenderHP( int mode );
void IfaceRenderAC( int Animate );
void IfaceRenderAP( int ap, int extra );
int  IfaceGetWeaponDsc( int *Slot, int *Ranged );
int  IfaceHandSlotUpdate( int a1, int LhandArg, int RhandArg );
int  IfaceToggleItem( int a1 );
int  IfaceUnk16( int *a1, int *a2 );
int  IfaceUnk17();
void IfaceHandSlotSelect();
int  IfaceGetSelectedHand();
int  IfaceGetHandObject( Obj_t **obj );
void IfaceAmmoGauge();
void IfaceCombatOpen( int a1 );
void IfaceCombatClose( int a1 );
void IfaceUnk24();
void IfaceUnk25();
void IfaceHandSlotReset();
void IfaceRenderHandSlot();
void IfaceAnimationEnd();
void IfcAnimateSlot( int a1, int a2 );
int  IfaceTurnBtOpen();
void IfaceTurnBtClose();
int  IfaceCmbtBtOpen();
void IfaceCmbtBtClose();
void IfaceRenderAmmo( int a1, int a2);
int  IfaceUnk36();
void IfaceRenderCounter( int Xpos, int Ypos, int CurrVal, int NewVal, int ColorSel, unsigned int Fps );
void IfaceDestroy();
int  IfaceIndicatorBoxInit();
void IfaceIndicatorBoxClose();
void IfaceIndicatorBoxDestroy();
int  IfaceIndicatorBoxUpdate();
void IfaceIndicatorBoxRender( int a1 );
int  IfaceIndocatorBoxAddEffect( int Effect );
void IfaceIndicatorBoxShow();
int  IfaceIndicatorBoxHide();


