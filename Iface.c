#include "FrameWork.h"

#define INFO_BOX_COLOR_INFO	gPalColorCubeRGB[0][31][0]	// information (positive effects)
#define INFO_BOX_COLOR_WARN	gPalColorCubeRGB[31][0][0]	// warning (negative effects)

static void IfaceRenderHandSlotCb();
static int IfaceIndicatorBoxSortCb( int *p1, int *p2 );
int IfaceIndicatorBoxAddEffect( int Effect );


int gIfcBarSlots[ 6 ];
Iface_t gIfcHandSlot[ 2 ];
CachePool_t *gIfcObj23;
CachePool_t *gIfcObj24;
CachePool_t *gIfcObj21;
int   gIfcIndicatorBoxVisible;
char  *gIfcImg18;
CachePool_t *gIfcObj22;
char  *gIfcCmbtBtDnImg;
char  *gIfcCmbtBtUpImg;
char  *gIfcImg19;
char  *gIfcTurnUpImg;
char  gIfcSlotSurfDn[ 12596 ];
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
int gifcCmbtBt = -1;
CachePool_t *gIfcCmbtBtDnImgObj = 0;
CachePool_t *gIfcCmbtBtUpImgObj = 0;
void *gIfcImg22 = 0;
void *gIfcImg23 = 0;
char *gIfcImg24 = 0;
VidRect_t gIfcUnk17 = { 0x13C, 0x0E, 0x196, 0x13 };
char *gIfcImg21 = NULL;
Iface01_t gIfcIndBox[ 5 ] = {
    { 102, 1, NULL }, // addict
    { 100, 0, NULL }, // Sneak
    { 101, 0, NULL }, // level
    { 103, 1, NULL }, // poisoned
    { 104, 1, NULL }, // radiated
};
int gIfcWin = -1;
int gIfcIndBoxWin = -1;
int gIfcCurrentHP = 0;
int gIfcStateHP = COUNTER_RED;
int gIfcCurrentAC = 0;
int gIfcTimeout = 0;

/**************************************/

int IfaceInit()
{
    char *img;
    CachePool_t *WpObj;

    if( gIfcWin != -1 ) return -1;
    gIfcUnk01 = 1;
    if( (gIfcWin = WinCreateWindow(0, 379, 640, 100, gPalColorCubeRGB[0][0][0], 8)) == -1 ){ IfaceClose(); return -1; }
    if( !(gIfcSurf = WinGetSurface( gIfcWin )) ){ IfaceClose(); return -1; }
    if( !(img = ArtGetBitmap(ArtMakeId(6, 16, 0, 0, 0), 0, 0, &WpObj)) ){ IfaceClose(); return -1; }
    ScrCopy( img, 640, 99, 640, gIfcSurf, 640 );
    ArtClose( WpObj );
    if( !(gIfcImg01 = ArtGetBitmap(ArtMakeId(6, 47, 0, 0, 0), 0, 0, &gIfcObj01)) ){ IfaceClose(); return -1; }        
    if( !(gIfcImg02 = ArtGetBitmap(ArtMakeId(6, 46, 0, 0, 0), 0, 0, &gIfcObj02)) ){ IfaceClose(); return -1; }
    if( (gIfcBt01 = WinCreateButton(gIfcWin, 211, 41, 32, 21, -1, -1, -1, 'i', gIfcImg01, gIfcImg02, 0, 0)) == -1 ){ IfaceClose(); return -1; }
    WinSetClickSound(gIfcBt01, GSoundPlayCheck, GSoundPlayUnCheck);
    if( !(gIfcImg03 = ArtGetBitmap(ArtMakeId(6, 18, 0, 0, 0), 0, 0, &gIfcObj03)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg04 = ArtGetBitmap(ArtMakeId(6, 17, 0, 0, 0), 0, 0, &gIfcObj04)) ){ IfaceClose(); return -1; }
    if( (gIfcBt02 = WinCreateButton(gIfcWin, 210, 62, 34, 34, -1, -1, -1, 'o', gIfcImg03, gIfcImg04, 0, 0)) == -1 ){ IfaceClose(); return -1; }
    WinSetClickSound(gIfcBt02, GSoundPlayCheck, GSoundPlayUnCheck);
    if( !(gIfcImg05 = ArtGetBitmap(ArtMakeId(6, 6, 0, 0, 0), 0, 0, &gIfcObj05)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg06 = ArtGetBitmap(ArtMakeId(6, 7, 0, 0, 0), 0, 0, &gIfcObj06)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg07 = ArtGetBitmap(ArtMakeId(6, 6, 0, 0, 0), 0, 0, &gIfcObj07)) ){ IfaceClose(); return -1; }        
    if( (gIfcBt08 = WinCreateButton(gIfcWin, 523, 6, 22, 21, -1, -1, -1, 's', gIfcImg05, gIfcImg06, 0, 32)) == -1 ){ IfaceClose(); return -1; }
    WinSetDefaultImg(gIfcBt08, gIfcImg07);
    WinSetClickSound(gIfcBt08, GSoundPlayCheck, GSoundPlayUnCheck);        
    if( !(gIfcImg08 = ArtGetBitmap(ArtMakeId(6, 13, 0, 0, 0), 0, 0, &gIfcObj08)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg09 = ArtGetBitmap(ArtMakeId(6, 10, 0, 0, 0), 0, 0, &gIfcObj09)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg10 = ArtGetBitmap(ArtMakeId(6, 13, 0, 0, 0), 0, 0, &gIfcObj10)) ){ IfaceClose(); return -1; }
    if( (gIfcBt03 = WinCreateButton(gIfcWin, 526, 40, 41, 19, -1, -1, -1, KEY_TAB, gIfcImg08, gIfcImg09, 0, 32)) == -1 ){ IfaceClose(); return -1; }
    WinSetDefaultImg(gIfcBt03, gIfcImg10);
    WinSetClickSound(gIfcBt03, GSoundPlayCheck, GSoundPlayUnCheck);
    if( !(gIfcImg11 = ArtGetBitmap(ArtMakeId(6, 59, 0, 0, 0), 0, 0, &gIfcObj11)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg12 = ArtGetBitmap(ArtMakeId(6, 58, 0, 0, 0), 0, 0, &gIfcObj12)) ){ IfaceClose(); return -1; }        
    if( (gIfcBt04 = WinCreateButton(gIfcWin, 526, 78, 41, 19, -1, -1, -1, 'p', gIfcImg11, gIfcImg12, 0, 0)) == -1 ){ IfaceClose(); return -1; }
    WinSetDefaultImg(gIfcBt04, gIfcImg10);
    WinSetClickSound(gIfcBt04, GSoundPlayCheck, GSoundPlayUnCheck);
    if( !(gIfcImg13 = ArtGetBitmap(ArtMakeId(6, 57, 0, 0, 0), 0, 0, &gIfcObj13)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg14 = ArtGetBitmap(ArtMakeId(6, 56, 0, 0, 0), 0, 0, &gIfcObj14)) ){ IfaceClose(); return -1; }
    if( (gIfcBt05 = WinCreateButton(gIfcWin, 526, 59, 41, 19, -1, -1, -1, 'c', gIfcImg13, gIfcImg14, 0, 0)) == -1 ){ IfaceClose(); return -1; }
    WinSetDefaultImg(gIfcBt05, gIfcImg10);
    WinSetClickSound(gIfcBt05, GSoundPlayCheck, GSoundPlayUnCheck);
    if( !(gIfcImgSlotRaised = ArtGetBitmap(ArtMakeId(6, 32, 0, 0, 0), 0, 0, &gIfcObj15)) ){ IfaceClose(); return -1; }
    if( !(gIfcImgSlotPressed = ArtGetBitmap(ArtMakeId(6, 31, 0, 0, 0), 0, 0, &gIfcObj16)) ){ IfaceClose(); return -1; }
    if( !(gIfcImgSlotGrayed = ArtGetBitmap(ArtMakeId(6, 73, 0, 0, 0), 0, 0, &gIfcObj17)) ){ IfaceClose(); return -1; }
    memcpy( gIfcSlotSurfUp, gIfcImgSlotRaised, 12596 );
    memcpy( gIfcSlotSurfDn, gIfcImgSlotPressed, 12596 );
    if( (gIfcBt06 = WinCreateButton(gIfcWin, 267, 26, 188, 67, -1, -1, -1, -20, gIfcSlotSurfUp, gIfcSlotSurfDn, 0, 32)) == -1 ){ IfaceClose(); return -1; }
    WinSetHandler2(gIfcBt06, -1, 110, 0, 0);
    WinSetClickSound(gIfcBt06, (void *)GSoundPlayKnobLt, (void *)GSoundPlayKnobRt);
    if( !(gIfcImg18 = ArtGetBitmap( ArtMakeId(6, 6, 0, 0, 0), 0, 0, &gIfcObj18)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg19 = ArtGetBitmap( ArtMakeId(6, 7, 0, 0, 0), 0, 0, &gIfcObj19)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg20 = ArtGetBitmap( ArtMakeId(6, 6, 0, 0, 0), 0, 0, &gIfcObj20)) ){ IfaceClose(); return -1; }
    if( (gIfcBt07 = WinCreateButton( gIfcWin, 218, 6, 22, 21, -1, -1, -1, 'b', gIfcImg18, gIfcImg19, 0, 32)) == -1 ){ IfaceClose(); return -1; }
    WinSetDefaultImg( gIfcBt07, gIfcImg20 );
    WinSetClickSound( gIfcBt07, GSoundPlayCheck, GSoundPlayUnCheck );
    if( !(gIfcImg21 = ArtGetBitmap( ArtMakeId(6, 82, 0, 0, 0), 0, 0, &gIfcObj21)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg22 = ArtGetBitmap( ArtMakeId(6, 83, 0, 0, 0), 0, 0, &gIfcObj22)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg23 = ArtGetBitmap( ArtMakeId(6, 84, 0, 0, 0), 0, 0, &gIfcObj23)) ){ IfaceClose(); return -1; }
    if( !(gIfcImg24 = ArtGetBitmap( ArtMakeId(6, 85, 0, 0, 0), 0, 0, &gIfcObj24)) ){ IfaceClose(); return -1; }
    ScrCopy( gIfcSurf + 9276, 90, 5, 640, gIfcUnk06, 90 );
    if( IfaceIndicatorBoxInit() == -1 ){ IfaceClose(); return -1; }
    gIfcHandSlotState = 0;
    gIfcHandSlot[ 0 ].obj = ART_NULL;
    gIfcHandSlot[ 1 ].obj = ART_NULL;
    IfcMsgInit();
    gIfcUiState = 1;
    gIfcUnk01 = 0;
    gIfcPanelState = 1;
    return 0;        
}

void IfaceReset()
{
    IfaceUiEnable();
    if( gIfcWin != -1 && !gIfcPanelState ){
        WinUpdateDirty( gIfcWin );
        gIfcPanelState = 1;
    }
    IfaceIndicatorBoxUpdate();
    IfcMsgFlush();
    if( gIfcIndBoxWin != -1 ){
        WinClose( gIfcIndBoxWin );
        gIfcIndBoxWin = -1;
    }
    gIfcHandSlotState = 0;
    gIfcIndicatorBoxVisible = 1;
}

void IfaceClose()
{
    if( gIfcWin != -1 ){
        IfcMsgClose();
        if( gIfcImg24 ){ ArtClose( gIfcObj24 ); gIfcImg24 = NULL; }
        if( gIfcImg23 ){ ArtClose( gIfcObj23 ); gIfcImg23 = NULL; }
        if( gIfcImg22 ){ ArtClose( gIfcObj22 ); gIfcImg22 = NULL; }
        if( gIfcImg21 ){ ArtClose( gIfcObj21 ); gIfcImg21 = NULL; }
        if( gIfcBt07 != -1 ){ WinImageRemove( gIfcBt07 ); gIfcBt07 = -1; }
        if( gIfcImg20 ){ ArtClose( gIfcObj20 ); gIfcObj20 = NULL; gIfcImg20 = NULL; }
        if( gIfcImg19 ){ ArtClose( gIfcObj19 ); gIfcObj19 = NULL; gIfcImg19 = NULL; }
        if( gIfcImg18 ){ ArtClose( gIfcObj18 ); gIfcObj18 = NULL; gIfcImg18 = NULL; }
        if( gIfcBt06 != -1 ){ WinImageRemove( gIfcBt06 ); gIfcBt06 = -1; }
        if( gIfcImgSlotGrayed ){ ArtClose( gIfcObj17 ); gIfcObj17 = NULL; gIfcImgSlotGrayed = NULL; }
        if( gIfcImgSlotPressed ){ ArtClose( gIfcObj16 ); gIfcObj16 = NULL; gIfcImgSlotPressed = NULL; }
        if( gIfcImgSlotRaised ){ ArtClose( gIfcObj15 ); gIfcObj15 = NULL; gIfcImgSlotRaised = NULL; }
        if( gIfcBt05 != -1 ){ WinImageRemove( gIfcBt05 ); gIfcBt05 = -1; }
        if( gIfcImg14 ){ ArtClose( gIfcObj14 ); gIfcObj14 = NULL; gIfcImg14 = NULL; }
        if( gIfcImg13 ){ ArtClose( gIfcObj13 ); gIfcObj13 = NULL; gIfcImg13 = NULL; }
        if( gIfcBt04 != -1 ){ WinImageRemove( gIfcBt04 ); gIfcBt04 = -1; }
        if( gIfcImg12 ){ ArtClose( gIfcObj12 ); gIfcObj12 = NULL; gIfcImg12 = NULL; }
        if( gIfcImg11 ){ ArtClose( gIfcObj11 ); gIfcObj11 = NULL; gIfcImg11 = NULL; }
        if( gIfcBt03 != -1 ){ WinImageRemove( gIfcBt03 ); gIfcBt03 = -1; }
        if( gIfcImg10 ){ ArtClose( gIfcObj10 ); gIfcObj10 = NULL; gIfcImg10 = NULL; }
        if( gIfcImg09 ){ ArtClose( gIfcObj09 ); gIfcObj09 = NULL; gIfcImg09 = NULL; }
        if( gIfcImg08 ){ ArtClose( gIfcObj08 ); gIfcObj08 = NULL; gIfcImg08 = NULL; }
        if( gIfcBt08 != -1 ){ WinImageRemove( gIfcBt08 ); gIfcBt08 = -1; }
        if( gIfcImg07 ){ ArtClose( gIfcObj07 ); gIfcObj07 = NULL; gIfcImg07 = NULL; }
        if( gIfcImg06 ){ ArtClose( gIfcObj06 ); gIfcObj06 = NULL; gIfcImg06 = NULL; }
        if( gIfcImg05 ){ ArtClose( gIfcObj05 ); gIfcObj05 = NULL; gIfcImg05 = NULL; }
        if( gIfcBt02 != -1 ){ WinImageRemove( gIfcBt02 ); gIfcBt02 = -1; }
        if( gIfcImg04 ){ ArtClose( gIfcObj04 ); gIfcObj04 = NULL; gIfcImg04 = NULL; }
        if( gIfcImg03 ){ ArtClose( gIfcObj03 ); gIfcObj03 = NULL; gIfcImg03 = NULL; }
        if( gIfcBt01 != -1 ){ WinImageRemove( gIfcBt01 ); gIfcBt01 = -1; }
        if( gIfcImg02 ){ ArtClose( gIfcObj02 ); gIfcObj02 = NULL; gIfcImg02 = NULL; }
        if( gIfcImg01 ){ ArtClose( gIfcObj01 ); gIfcObj01 = NULL; gIfcImg01 = NULL; }
        if( gIfcWin != -1 ){ WinClose( gIfcWin ); gIfcWin = -1; }
    }
    IfaceIndicatorBoxClose();
}

int IfacePanelLoad( xFile_t *fh )
{
    int CombatState = 0, HandSlot = 0, PlayerPanel = 0, UiPanel = 0;

    if( gIfcWin == -1 ){
	if( IfaceInit() == -1 ) return -1;    
    }
    if( dbgetBei( fh, &UiPanel ) ) return -1;
    if( dbgetBei( fh, &PlayerPanel ) ) return -1;
    if( dbgetBei( fh, &HandSlot ) ) return -1;
    if( dbgetBei( fh, &CombatState ) ) return -1;
    if( !gIfcUiState ) IfaceUiEnable();
    if( PlayerPanel ){
        if( gIfcWin != -1 && !gIfcPanelState ){
            WinUpdateDirty( gIfcWin );
            gIfcPanelState = 1;
        }
        IfaceIndicatorBoxUpdate();
    } else {
        IfacePanelDisable();
    }
    IfaceRenderHP( 0 );
    IfaceResetAC( 0 );
    gIfcHandSlotState = HandSlot;
    IfaceHandSlotUpdate( 0, -1, -1 );
    if( CombatState != gIfcCombatState ){
        if( CombatState )
            IfaceCombatOpen( 0 );
        else
            IfaceCombatClose( 0 );
    }
    if( !UiPanel ) IfaceUiDisable();
    IfaceIndicatorBoxUpdate();
    WinUpdate( gIfcWin );
    return 0;
}

int IfacePanelSave( xFile_t *fh )
{
    if( gIfcWin == -1 ) return -1;
    if( dbputBei( fh, gIfcUiState ) ) return -1;
    if( dbputBei( fh, gIfcPanelState ) ) return -1;
    if( dbputBei( fh, gIfcHandSlotState ) ) return -1;
    if( dbputBei( fh, gIfcCombatState ) ) return -1;
    return 0;
}

void IfacePanelEnable()
{
    if( gIfcWin != -1 && !gIfcPanelState ){
        WinUpdateDirty( gIfcWin );
        gIfcPanelState = 1;
    }
    IfaceIndicatorBoxUpdate();
}

void IfacePanelDisable()
{
    if( gIfcWin != -1 && gIfcPanelState ){
        IfaceHandSlotUpdate( 0, -1, -1 );
        IfaceRenderHP( 0 );
        IfaceResetAC( 0 );
        WinMoveTop( gIfcWin );
        gIfcPanelState = 0;
    }
    IfaceIndicatorBoxUpdate();
}

int IfaceGetPanelState()
{
    return gIfcPanelState;
}

void IfaceUiEnable()
{
    if( gIfcUiState ) return; 
    WinEnableWidget( gIfcBt01 );
    WinEnableWidget( gIfcBt02 );
    WinEnableWidget( gIfcBt08 );
    WinEnableWidget( gIfcBt03 );
    WinEnableWidget( gIfcBt04 );
    WinEnableWidget( gIfcBt05 );
    if( !gIfcHandSlot[ gIfcHandSlotState ].i02 ) WinEnableWidget( gIfcBt06 );
    WinEnableWidget( gIfcEndTurnBt );
    WinEnableWidget( gifcCmbtBt );
    IfcMsgButtEnable();
    gIfcUiState = 1;    
}

void IfaceUiDisable()
{
    if( !gIfcUiState ) return;    
    IfcMsgButtDisable();
    WinDisableWidget( gIfcBt01 );
    WinDisableWidget( gIfcBt02 );
    WinDisableWidget( gIfcBt08 );
    WinDisableWidget( gIfcBt03 );
    WinDisableWidget( gIfcBt04 );
    WinDisableWidget( gIfcBt05 );
    if( !gIfcHandSlot[ gIfcHandSlotState ].i02 ) WinDisableWidget( gIfcBt06 );
    WinDisableWidget( gIfcEndTurnBt );
    WinDisableWidget( gifcCmbtBt );
    gIfcUiState = 0;    
}

int IfaceGetUiState()
{
    return gIfcUiState;
}

void IfaceUnk09()
{
    if( gIfcWin != -1 ){
        IfaceHandSlotUpdate( 0, -1, -1 );
        IfaceRenderHP( 0 );
        IfaceResetAC( 0 );
        IfaceIndicatorBoxUpdate();
        WinUpdate( gIfcWin );
    }
    IfaceIndicatorBoxUpdate();
}

void IfaceRenderHP( int mode )
{
    int cnt, i, hp[ 3 ], cond[ 2 ], hp100, Pddset, hp50, hp25, CurHp;

    if( gIfcWin != -1 ){
        CurHp = CritterGetHp( gObjDude );
        hp100 = FeatGetVal( gObjDude, 7 );
        hp25 = lround( hp100 * 0.25 );
        hp50 = lround( hp100 * 0.5 );
        if( CurHp >= hp25 ){
            Pddset = ( CurHp >= hp50 ) ? COUNTER_NORMAL : COUNTER_YELLOW;
        } else {
            Pddset = COUNTER_RED;
        }
        cnt = 1;
        hp[ 0 ] = gIfcCurrentHP;
        cond[ 0 ] = gIfcStateHP;
        if( gIfcStateHP != Pddset ){
            if( CurHp >= hp[ 0 ] ){
                if( hp[ 0 ] < hp25 && CurHp >= hp25 ){ hp[ 1 ] = hp25; cond[ 1 ] = COUNTER_YELLOW; cnt = 2; }
                if( hp50 > gIfcCurrentHP && hp50 <= CurHp ){ hp[ 1 ] = hp50; cond[ 1 ] = COUNTER_NORMAL; cnt = 2; }
            } else {
                if( hp[ 0 ] >= hp50 && CurHp < hp50 ){ hp[ 1 ] = hp50; cond[ 1 ] = COUNTER_YELLOW; cnt = 2; }
                if( hp25 <= gIfcCurrentHP && hp25 > CurHp ){ hp[ 1 ] = hp25; cond[ 1 ] = COUNTER_RED; cnt = 2; }
            }
        }
        hp[ cnt ] = CurHp;
        if( mode ){
            for( i = 0; i < cnt; i++ ){
                IfaceRenderCounter( 473, 40, hp[ i ], hp[ i + 1 ], cond[ i ], 250 / (abs32(gIfcCurrentHP - CurHp) + 1) );
            }
        } else {
            IfaceRenderCounter( 473, 40, gIfcCurrentHP, CurHp, Pddset, 0 );
        }
        gIfcCurrentHP = CurHp;
        gIfcStateHP = Pddset;
    }
}

void IfaceResetAC( int Animate )
{
    int ac, time_ms;

    ac = FeatGetVal( gObjDude, FEAT_AC );
    time_ms = 0;
    if( Animate ) time_ms = COUNTER_ROTATION_MS / ( abs32( gIfcCurrentAC - ac ) + 1 );
    IfaceRenderCounter( 473, 75, gIfcCurrentAC, ac, 0, time_ms );
    gIfcCurrentAC = ac;
}

void IfaceRenderAP( int ap, int extra )
{
    char *img;
    int i;

    if( gIfcWin == -1 ) return;
    ScrCopy( gIfcUnk06, 90, 5, 90, gIfcSurf + 14*640 + 316, 640 );
    if( ap == -1 ){
        img = gIfcImg24;
        ap = 10;
        extra = 0; 
    } else {
	img = gIfcImg22;
	if( ap < 0 ) ap = 0;
	if( ap > 10 ) ap = 10;
	if( extra >= 0 ){
    	    if( extra + ap > 10 ){ extra = 10 - ap; }
	} else {
    	    extra = 0;
	}
    }
    for( i = 0; i < ap; i++ ){
        ScrCopy( img, 5, 5, 5, gIfcSurf + i * 9 + 14*640 + 316, 640 );
    }
    for( i = 0; i < extra; i++ ){
        ScrCopy( gIfcImg23, 5, 5, 5, gIfcSurf + 9*ap + i*9 + 14*640 + 316, 640 );
    }
    if( !gIfcUnk01 ) WinAreaUpdate( gIfcWin, &gIfcUnk17 );
}

int IfaceGetWeaponDsc( int *Slot, int *Ranged )
{
    if( gIfcWin == -1 ) return -1;
    *Ranged = 0;
    switch( gIfcHandSlot[ gIfcHandSlotState ].i05 ){
        case 1: *Slot = gIfcHandSlot[ gIfcHandSlotState ].i03; return 0;
        case 2: *Slot = gIfcHandSlot[ gIfcHandSlotState ].i03; *Ranged = 1; return 0;
        case 3: *Slot = gIfcHandSlot[ gIfcHandSlotState ].i04; return 0;
        case 4: *Slot = gIfcHandSlot[ gIfcHandSlotState ].i04; *Ranged = 1; return 0;
    }
    return -1;
}

int IfaceHandSlotUpdate( int a1, int LhandArg, int RhandArg )
{
    Obj_t *HandObj, *obj;
    int AgPts,PePts,StPts,UnArmPts,tmp;

    if( MapAmbientEnabled() ) a1 = 0;
    if( gIfcWin == -1 ) return -1;
    obj = gIfcHandSlot[ gIfcHandSlotState ].obj;
    
    // left hand parameters
    HandObj = InvGetLHandObj( gObjDude );
    if( HandObj == gIfcHandSlot[ 0 ].obj && gIfcHandSlot[ 0 ].obj ){ // no item change in left hand
        gIfcHandSlot[ 0 ].i02 = Item29( HandObj ); // need ammo flag ??
        gIfcHandSlot[ 0 ].ArtId = ItemGetArtId( gIfcHandSlot[ 0 ].obj );
    } else {
        gIfcHandSlot[ 0 ].obj = HandObj;
        if( HandObj ){
            gIfcHandSlot[ 0 ].i02 = Item29( HandObj );
            gIfcHandSlot[ 0 ].i03 = 0;
            gIfcHandSlot[ 0 ].i04 = 1;
            gIfcHandSlot[ 0 ].Usable = (ItemGetObjType( HandObj ) == PR_ITEM_WEAPON);
            if( LhandArg == -1 ){
                gIfcHandSlot[ 0 ].i05 = ( gIfcHandSlot[ 0 ].Usable ) ? 1 : 0;
            } else {
                gIfcHandSlot[ 0 ].i05 = LhandArg;
            }
            gIfcHandSlot[ 0 ].ArtId = ItemGetArtId( HandObj );
        } else { // unarmed
            gIfcHandSlot[ 0 ].i02 = 0;
            gIfcHandSlot[ 0 ].Usable = 1;
            gIfcHandSlot[ 0 ].i05 = 1;
            gIfcHandSlot[ 0 ].ArtId = -1;
            UnArmPts = SkillGetTotal( gObjDude, SKILL_UNARMED );
            AgPts = FeatGetVal( gObjDude, FEAT_AGILITY );
            StPts = FeatGetVal( gObjDude, FEAT_STAMINA );
            PePts = FeatGetPoints( FEAT_PERCEPTION );
            if( UnArmPts <= 99 || AgPts <= 6 || StPts <= 4 || PePts <= 8 ){
                if ( UnArmPts <= 74 || AgPts <= 5 || StPts <= 4 || PePts <= 5 ){
                    gIfcHandSlot[ 0 ].i03 = ( UnArmPts <= 54 || AgPts <= 5 ) ? 4 : 8;
                } else {
                    gIfcHandSlot[ 0 ].i03 = 9;
                }
            } else {
                gIfcHandSlot[ 0 ].i03 = 10;
            }
            if( UnArmPts <= 129 || AgPts <= 6 || StPts <= 4 || PePts <= 15 ){
                if( UnArmPts <= 114 || AgPts <= 6 || StPts <= 4 || PePts <= 11 ){
                    gIfcHandSlot[ 0 ].i04 = ( UnArmPts <= 74 || AgPts <= 6 || StPts <= 4 || PePts <= 4 ) ? 4 : 11;
                } else {
                    gIfcHandSlot[ 0 ].i04 = 12;
                }
            } else {
                gIfcHandSlot[ 0 ].i04 = 13;
            }
        }
    }

    // right hand parameters
    HandObj = InvGetRHandObj( gObjDude );
    if( HandObj == gIfcHandSlot[ 1 ].obj && gIfcHandSlot[ 1 ].obj ){
        gIfcHandSlot[ 1 ].i02 = Item29( HandObj );
        gIfcHandSlot[ 1 ].ArtId = ItemGetArtId( HandObj );
    } else {
        gIfcHandSlot[ 1 ].obj = HandObj;
        if( HandObj ){
            gIfcHandSlot[ 1 ].i02 = Item29( HandObj );
            gIfcHandSlot[ 1 ].i03 = 2;
            gIfcHandSlot[ 1 ].i04 = 3;
            gIfcHandSlot[ 1 ].Usable = (ItemGetObjType( HandObj ) == PR_ITEM_WEAPON );
            if( RhandArg == -1 ) {
                gIfcHandSlot[ 1 ].i05 = ( gIfcHandSlot[ 1 ].Usable ) ? 1 : 0;
            } else {
                gIfcHandSlot[ 1 ].i05 = RhandArg;
            }
            gIfcHandSlot[ 1 ].ArtId = ItemGetArtId( HandObj );
        } else {
            gIfcHandSlot[ 1 ].i02 = 0;
            gIfcHandSlot[ 1 ].Usable = 1;
            gIfcHandSlot[ 1 ].i05 = 1;
            gIfcHandSlot[ 1 ].ArtId = -1;
            UnArmPts = SkillGetTotal( gObjDude, SKILL_UNARMED );
            AgPts = FeatGetVal( gObjDude, FEAT_AGILITY );
            StPts = FeatGetVal( gObjDude, FEAT_STAMINA );
            PePts = FeatGetPoints( FEAT_PERCEPTION );
            if( UnArmPts <= 79 || AgPts <= 5 || StPts <= 5 || PePts <= 8 ){
                if( UnArmPts <= 59 || AgPts <= 5 || PePts <= 5 ){
                    gIfcHandSlot[ 1 ].i03 = ( UnArmPts <= 39 || AgPts <= 5 ) ? 5 : 14;
                } else {
                    gIfcHandSlot[ 1 ].i03 = 15;
                }
            } else {
                gIfcHandSlot[ 1 ].i03 = 16;
            }
            if( UnArmPts <= 124 || AgPts <= 7 || StPts <= 5 || PePts <= 14 ){
                if ( UnArmPts <= 99 || AgPts <= 6 || StPts <= 5 || PePts <= 11 ){
                    gIfcHandSlot[ 1 ].i04 = ( UnArmPts <= 59 || AgPts <= 6 || StPts <= 5 || PePts <= 5 ) ? 5 : 17;
                } else {
                    gIfcHandSlot[ 1 ].i04 = 18;
                }
            } else {
                gIfcHandSlot[ 1 ].i04 = 19;
            }
        }
    }

    if( !a1 ){
        IfaceRenderHandSlot();
        return 0;
    }
    
    if( gIfcHandSlot[ gIfcHandSlotState ].obj == obj ){
        IfaceRenderHandSlot();
    } else {
        tmp = 0;
        if( gIfcHandSlot[ gIfcHandSlotState ].obj ) {
            if( ItemGetObjType( gIfcHandSlot[ gIfcHandSlotState ].obj ) == PR_ITEM_WEAPON ) tmp = Item58( gIfcHandSlot[ gIfcHandSlotState ].obj );
        }
        IfcAnimateSlot( (gObjDude->ImgId & 0xF000) >> 12, tmp );
    }
    return 0;
}

int IfaceToggleItem( int a1 )
{
    int v1, Mode;

    if( gIfcWin == -1 ) return -1;
    gIfcHandSlotState = 1 - gIfcHandSlotState; // change slot
    if( a1 ){
        v1 = 0;
        if( gIfcHandSlot[ gIfcHandSlotState ].obj && ItemGetObjType( gIfcHandSlot[ gIfcHandSlotState ].obj ) == PR_ITEM_WEAPON ) v1 = Item58( gIfcHandSlot[ gIfcHandSlotState ].obj );
        IfcAnimateSlot( (gObjDude->ImgId & 0xF000) >> 12, v1);
    } else {
        IfaceRenderHandSlot();
    }
    Mode = GmouseGetMode();
    if( Mode == 2 || Mode == 3 ) GmouseSetMode( 0 );
    return 0;
}

int IfaceUnk16( int *a1, int *a2 )
{
    *a1 = gIfcHandSlot[ 0 ].i05;
    *a2 = gIfcHandSlot[ 1 ].i05;
    return 0;
}

int IfaceUnk17()
{
    Iface_t *v0;
    int i05, v2, v7;

    if( gIfcWin == -1 ) return -1;
    v0 = &gIfcHandSlot[ gIfcHandSlotState ];
    i05 = v0->i05;
    if( v0->Usable ){
        v2 = 0;
        do{
            switch( ++v0->i05 ){
                case 1: v2 = 1; break;
                case 2: if( Item62( gObjDude, v0->i03 ) ) v2 = 1; break;
                case 3: if( v0->i04 != 4 && v0->i04 != 5 && ItemGetClass( v0->obj, v0->i04 ) ) v2 = 1; break;
                case 4: if( v0->i04 != 4 && v0->i04 != 5 && ItemGetClass( v0->obj, v0->i04 ) && Item62( gObjDude, v0->i04 ) ) v2 = 1; break;
                case 5: v7 = ItemGetPackQuantity( v0->obj ); if( v7 != ItemGetAmmo( v0->obj ) ) v2 = 1; break;
                case 6: v0->i05 = 0; break;
            }
        }while( !v2 );
    }
    if( i05 != v0->i05 ) IfaceRenderHandSlot();
    return 0;
}

void IfaceHandSlotSelect()
{
    int nn, CurrentAP;

    if( gIfcWin == -1 ) return;
    if( gIfcHandSlot[ gIfcHandSlotState ].Usable ){
        if( gIfcHandSlot[ gIfcHandSlotState ].i05 == 5 ){
            if( !IN_COMBAT){ IfaceUnk36(); return; }
            nn = ItemGetSlotApCost( gObjDude, ( gIfcHandSlotState != 0 ) + 6, 0 );
            if( nn <= gObjDude->Critter.State.CurrentAP && !IfaceUnk36() ){
                CurrentAP = gObjDude->Critter.State.CurrentAP;
                if( nn > CurrentAP )
                    gObjDude->Critter.State.CurrentAP = 0;
                else
                    gObjDude->Critter.State.CurrentAP = CurrentAP - nn;
                IfaceRenderAP( gObjDude->Critter.State.CurrentAP, gCombatMovePts );
            }
        } else { // click on weapon
            GmouseLoadCursor( 20 );
            GmouseSetMode( 2 );
            if( !IN_COMBAT ) CombatStart( 0 );
        }
        return;
    }
    if( ProtoDrugUsable( gIfcHandSlot[ gIfcHandSlotState ].obj->Pid ) ){
        GmouseLoadCursor( 23 );
        GmouseSetMode( 3 );
        return;
    }
    if( !ObjOpenable( gIfcHandSlot[ gIfcHandSlotState ].obj ) ) return;            
    if( IN_COMBAT ){
        nn = ItemGetSlotApCost( gObjDude, gIfcHandSlot[ gIfcHandSlotState ].i03, 0 );
        if( nn > gObjDude->Critter.State.CurrentAP ) return;
        UseUnk15( gObjDude, gIfcHandSlot[ gIfcHandSlotState ].obj );
        IfaceHandSlotUpdate( 0, -1, -1 );
        if( nn > gObjDude->Critter.State.CurrentAP )
            gObjDude->Critter.State.CurrentAP = 0;
        else
            gObjDude->Critter.State.CurrentAP -= nn;
        IfaceRenderAP( gObjDude->Critter.State.CurrentAP, gCombatMovePts );
        return;
    }
    UseUnk15( gObjDude, gIfcHandSlot[ gIfcHandSlotState ].obj );
    IfaceHandSlotUpdate( 0, -1, -1 );            
}

int IfaceGetSelectedHand()
{
    return gIfcHandSlotState;
}

int IfaceGetHandObject( Obj_t **obj )
{
    if( gIfcWin == -1 ) return -1;
    *obj = gIfcHandSlot[ gIfcHandSlotState ].obj;
    return 0;
}

void IfaceAmmoGauge()
{
    double Ammo, Cap;

    if( gIfcWin == -1 ) return;    
    if( gIfcHandSlot[ gIfcHandSlotState ].Usable ){
        if( ItemGetPackQuantity( gIfcHandSlot[ gIfcHandSlotState ].obj ) <= 0 ){ IfaceRenderAmmo( 463, 0 ); return; }
        Ammo = ItemGetAmmo( gIfcHandSlot[ gIfcHandSlotState ].obj );
        Cap = ItemGetPackQuantity( gIfcHandSlot[ gIfcHandSlotState ].obj );
    } else {
        if( ItemGetObjType( gIfcHandSlot[ gIfcHandSlotState ].obj) != 5 || ItemCharges(gIfcHandSlot[gIfcHandSlotState].obj) <= 0 ){ IfaceRenderAmmo( 463, 0 ); return; }
        Ammo = ItemGetCharges( gIfcHandSlot[ gIfcHandSlotState ].obj );
        Cap = ItemCharges( gIfcHandSlot[ gIfcHandSlotState ].obj );
    }
    IfaceRenderAmmo( 463, lround( Ammo / Cap * 70.0 ) );
}

void IfaceCombatOpen( int a1 )
{
    ArtFrmHdr_t *Img;
    CachePool_t *ImgObj;
    char *bmp;
    unsigned int speed, time, i, Fpd;

    if( gIfcWin == -1 || gIfcCombatState ) return;        
    if( !(Img = ArtLoadImg( ArtMakeId( 6, 104, gIfcCombatState, 0, gIfcCombatState ), &ImgObj )) ) return;    
    Fpd = ArtGetFpd( Img );
    GSoundPlay( "iciboxx1" );
    if( a1 ){
        speed = 1000 / ArtGetObjSpeed( Img );
        time = 0;        
        for( i = 0; i < Fpd; ){
            if( TimerCurrDiff( time ) >= speed ){
        	if( (bmp = ArtGetObjData( Img, i, 0 ) ) ){
            	    ScrCopy( bmp, 57, 58, 57, WIN_XY( 580, 38, 640, gIfcSurf ), 640 ); // animate open/close "door" 
            	    WinAreaUpdate( gIfcWin, &gIfcUnk10 );
        	}
        	time = TimerGetSysTime();
        	i++;
            }
            GmouseProcess();
        }                    
    } else {
        ScrCopy( ArtGetObjData( Img, Fpd - 1, 0 ), 57, 58, 57, WIN_XY( 580, 38, 640, gIfcSurf ), 640 );
        WinAreaUpdate( gIfcWin, &gIfcUnk10 );
    }
    ArtClose( ImgObj );
    gIfcCombatState = 1;
    IfaceTurnBtOpen();
    IfaceCmbtBtOpen();
    IfaceUnk25();        
}

void IfaceCombatClose( int a1 )
{
    CachePool_t *ImgObj;
    ArtFrmHdr_t *Art;
    unsigned int speed, time, Fpd;
    char *ObjData;

    if( gIfcWin == -1 || !gIfcCombatState ) return;        
    if( !(Art = ArtLoadImg( ArtMakeId( 6, 104, 0, 0, 0 ), &ImgObj ) ) ) return;
    IfaceTurnBtClose();
    IfaceCmbtBtClose();
    GSoundPlay( "icibcxx1" );
    if( a1 ){
        speed = 1000 / ArtGetObjSpeed( Art );
        Fpd = ArtGetFpd( Art );
        while( Fpd != 0 ){
            if( TimerCurrDiff( time ) >= speed ){                
                if( (ObjData = ArtGetObjData( Art, Fpd - 1, 0 )) ){
                    ScrCopy( ObjData, 57, 58, 57, gIfcSurf + 24900, 640 );
                    WinAreaUpdate( gIfcWin, &gIfcUnk10 );
                }
                time = TimerGetSysTime();
                Fpd--;
            }
            GmouseProcess();
        }
    } else {
        ScrCopy( ArtGetObjData( Art, 0, 0 ), 57, 58, 57, gIfcSurf + 24900, 640 );
        WinAreaUpdate( gIfcWin, &gIfcUnk10 );
    }
    ArtClose( ImgObj );
    gIfcCombatState = 0;
}

void IfaceUnk24()
{
    CachePool_t *ImgObj;
    char *img;

    if( !gIfcCombatState ) return;    
    WinEnableWidget( gIfcEndTurnBt );
    WinEnableWidget( gifcCmbtBt );
    if( !(img = ArtGetBitmap( ArtMakeId( 6, 109, 0, 0, 0 ), 0, 0, &ImgObj ) ) ) return;
    GSoundPlay( "icombat2" );
    ScrCopyAlpha( img, 57, 58, 57, gIfcSurf + 24900, 640 );
    WinAreaUpdate( gIfcWin, &gIfcUnk10 );
    ArtClose( ImgObj );            
}

void IfaceUnk25()
{
    CachePool_t *ImgObj;
    char *img;
    
    if( !gIfcCombatState ) return;    
    WinDisableWidget( gIfcEndTurnBt );
    WinDisableWidget( gifcCmbtBt );
    if( !(img = ArtGetBitmap( ArtMakeId( 6, 110, 0, 0, 0 ), 0, 0, &ImgObj )) ) return;
    GSoundPlay( "icombat1" );
    ScrCopyAlpha( img, 57, 58, 57, gIfcSurf + 24900, 640 );
    WinAreaUpdate( gIfcWin, &gIfcUnk10 );
    ArtClose( ImgObj );            
}

void IfaceHandSlotReset()
{
    gIfcHandSlot[ 1 ].obj = ART_NULL;
    gIfcHandSlot[ 0 ].obj = ART_NULL;
}

void IfaceRenderHandSlot()
{
    CachePool_t *ImgObj;
    ArtFrmHdr_t *Img;
    Iface_t *v0;
    unsigned int v13;
    int v2,i03,v9,v29,v39,h,v45,v46,v55,Id,w,ap;
    char *dat;

    if( gIfcWin == -1 ) return;
    WinEnableWidget( gIfcBt06 );
    v0 = &gIfcHandSlot[ gIfcHandSlotState ];
    ap = -1;
    if( !v0->i02 ){
        memcpy( gIfcSlotSurfUp, gIfcImgSlotRaised, 12596 );
        memcpy( gIfcSlotSurfDn, gIfcImgSlotPressed, 12596 );
        if( !v0->Usable ){
            if( ProtoDrugUsable( v0->obj->Pid ) ){
                v2 = ArtMakeId( 6, 294, 0, 0, 0 );
            } else if ( ObjOpenable( v0->obj ) ){
                v2 = ArtMakeId( 6, 292, 0, 0, 0 );
            } else {
                v2 = -1;
            }
            if( v2 != -1 ){                
                if( (Img = ArtLoadImg( v2, &ImgObj )) ){
                    w = ArtGetObjWidth( Img, 0, 0 );
                    h = ArtGetObjHeight( Img, 0, 0 );
                    dat = ArtGetObjData( Img, 0, 0 );
                    ScrCopyAlpha( dat, w, h, w, &gIfcSlotSurfUp[181 - w + 1316], 188 );
                    ObjRenderNormal( dat, w, h, w, gIfcSlotSurfDn, 181 - w + 1, 5, 188, 59641 );
                    ArtClose( ImgObj );
                }
                ap = ItemGetSlotApCost( gObjDude, v0->i03, 0 );
            }
        } else {
    	    v2 = -1; i03 = -1; Id = -1;
    	    switch( v0->i05 - 1 ){
        	case 0: i03 = v0->i03; break;
        	case 1: Id = ArtMakeId( 6, 288, 0, 0, 0 ); i03 = v0->i03; break;
        	case 2: i03 = v0->i04; break;
        	case 3: Id = ArtMakeId( 6, 288, 0, 0, 0 ); i03 = v0->i04; break;
        	case 4: ap = ItemGetSlotApCost( gObjDude, ( gIfcHandSlotState != 0 ) + 6, 0 ); v2 = ArtMakeId( 6, 291, 0, 0, 0 ); break;
    	    }
    	    if( Id != -1 ){            
        	if( (Img = ArtLoadImg( Id, &ImgObj )) ){
            	    w = ArtGetObjWidth( Img, 0, 0 );
            	    h = ArtGetObjHeight( Img, 0, 0 );
            	    dat = ArtGetObjData( Img, 0, 0 );
            	    v9 = 60 - h - 2;
            	    ScrCopyAlpha( dat, w, h, w, &gIfcSlotSurfUp[ 188 * (60 - h) + 181 - w ], 188 );
            	    if( v9 < 0 ){ v9 = 0; h -= 2; }
            	    ObjRenderNormal( dat, w, h, w, gIfcSlotSurfDn, 181 - w + 1, v9, 188, 59641 );
            	    ArtClose( ImgObj );
        	}
    	    }
    	    if( i03 != -1 ){
        	ap = ItemGetAPCost( gObjDude, i03, ( Id == -1 ) ? 0 : 1 );
        	v13 = Item44( gObjDude, i03 );
        	if( v13 >= 0x29 ){
            	    if( v13 <= 0x29 ){
                	v2 = ArtMakeId( 6, 45, 0, 0, 0 );
            	    } else if( v13 < 0x2D ){
                	if( v13 == 42 ) v2 = ArtMakeId( 6, 44, 0, 0, 0 );
            	    } else if( v13 <= 0x2D ){
                	v2 = ArtMakeId( 6, 43, 0, 0, 0 );
            	    } else {
                	if( v13 < 0x30 ) v2 = ArtMakeId( 6, 40, 0, 0, 0 );
            	    }
        	} else {
        	    if( v13 >= 17 ){
            		if( v13 <= 17 ){
                	    switch( i03 ){
                    		case 14: v2 = ArtMakeId(6, 430, 0, 0, 0); break;
                    		case 15: v2 = ArtMakeId(6, 431, 0, 0, 0); break;
                    		case 16: v2 = ArtMakeId(6, 429, 0, 0, 0); break;
                    		case 17: v2 = ArtMakeId(6, 426, 0, 0, 0); break;
                    		case 18: v2 = ArtMakeId(6, 427, 0, 0, 0); break;
                    		case 19: v2 = ArtMakeId(6, 422, 0, 0, 0); break;
                    		default: v2 = ArtMakeId(6, 41, 0, 0, 0);  break;
                	    }
            		} else {
                	    if( v13 == 18 ){
                		v2 = ArtMakeId( 6, 117, 0, 0, 0 );
                	    }
            		}                
        	    } else {
        		if( v13 == 16 ){
            		    switch( i03 ){
                		case 8:  v2 = ArtMakeId(6, 432, 0, 0, 0); break;
                		case 9:  v2 = ArtMakeId(6, 425, 0, 0, 0); break;
                		case 10: v2 = ArtMakeId(6, 428, 0, 0, 0); break;
                		case 11: v2 = ArtMakeId(6, 421, 0, 0, 0); break;
            	    		case 12: v2 = ArtMakeId(6, 423, 0, 0, 0); break;
            	    		case 13: v2 = ArtMakeId(6, 424, 0, 0, 0); break;
                		default: v2 = ArtMakeId(6, 42, 0, 0, 0); break;
            		    }
        		}
        	    }
        	}
    	    }
    	    if( v2 != -1 ){
        	if( (Img = ArtLoadImg( v2, &ImgObj )) ){
            	    w = ArtGetObjWidth( Img, 0, 0 );
            	    h = ArtGetObjHeight( Img, 0, 0 );
            	    dat = ArtGetObjData( Img, 0, 0 );
            	    ScrCopyAlpha( dat, w, h, w, &gIfcSlotSurfUp[181 - w + 1316], 188 );
            	    ObjRenderNormal( dat, w, h, w, gIfcSlotSurfDn, 181 - w + 1, 5, 188, 59641 );
            	    ArtClose( ImgObj );
        	}
    	    }
	}
        if( ap <= 9 ){ // AP xx
            if( (Img = ArtLoadImg( ArtMakeId(6, 289, 0, 0, 0), &ImgObj )) ){
                w = ArtGetObjWidth( Img, 0, 0 );
                h = ArtGetObjHeight( Img, 0, 0 );
                dat = ArtGetObjData( Img, 0, 0 );
                v29 = 60 - h - 2;
                ScrCopyAlpha( dat, w, h, w, &gIfcSlotSurfUp[188 * (60 - h) + 7], 188 ); // AP
                if( v29 < 0 ){
                    v29 = 0;
                    h -= 2;
                }
                ObjRenderNormal( dat, w, h, w, gIfcSlotSurfDn, 7 + 1, v29, 188, 59641 );
                v55 = w + 7;
                ArtClose( ImgObj );

                if( (Img = ArtLoadImg( ArtMakeId( 6, 290, 0, 0, 0 ), &ImgObj ) ) ){
                    w = ArtGetObjWidth( Img, 0, 0 );
                    h = ArtGetObjHeight( Img, 0, 0 );
                    dat = ArtGetObjData( Img, 0, 0 ) + 10 * ap;
                    v39 = 60 - h - 2;
                    ScrCopyAlpha( dat, 10, h, w, &gIfcSlotSurfUp[ 188 * (60 - h) + 7 + v55 ], 188 ); // xx
                    if( v39 < 0 ){
                        v39 = 0;
                        h -= 2;
                    }
                    ObjRenderNormal( dat, 10, h, w, gIfcSlotSurfDn, v55 + 7 + 1, v39, 188, 59641 );
                    ArtClose( ImgObj );
                }
            }
        }
    } else {
	memcpy( gIfcSlotSurfUp, gIfcImgSlotGrayed, 12596 );
	memcpy( gIfcSlotSurfDn, gIfcImgSlotGrayed, 12596 );
    }
    if( v0->ArtId != -1 ){
        if( (Img = ArtLoadImg( v0->ArtId, &ImgObj )) ){ // pressed hand obj
            w = ArtGetObjWidth( Img, 0, 0 );
            h = ArtGetObjHeight( Img, 0, 0 );
            dat = ArtGetObjData( Img, 0, 0 );
            v45 = (188 - w) / 2;
            v46 = (67 - h) / 2 - 2;
            ScrCopyAlpha( dat, w, h, w, &gIfcSlotSurfUp[ 188 * ((67 - h) / 2) + v45 ], 188 );
            if( v46 < 0 ){  v46 = 0; h -= 2; }
            ObjRenderNormal( dat, w, h, w, gIfcSlotSurfDn, v45 + 1, v46, 188, 63571 );
            ArtClose( ImgObj );
        }
    }
    if( !gIfcUnk01 ){
        IfaceAmmoGauge();
        WinAreaUpdate( gIfcWin, &gIfcUnk25 );
        if( v0->i02 )
            WinDisableWidget( gIfcBt06 );
        else
            WinEnableWidget( gIfcBt06 );
    }
}

static void IfaceRenderHandSlotCb()
{
    IfaceRenderHandSlot();
}

void IfaceAnimationEnd()
{
    gIfcAnimate = 0;
}

void IfcAnimateSlot( int a1, int a2 )
{
    int tmp;
    Obj_t *obj;

    gIfcAnimate = 1;
    AnimRegClear( gObjDude );
    AnimRegStart( 2 );
    AnimChangeItemStart( gObjDude, 4, 0 );
    if( a1 ){
        AnimRegPlaySfx( gObjDude, GSoundCharacterFileName( gObjDude, 39, 0 ), 0 );
        AnimRegAnimation( gObjDude, 39, 0 );
    }
    AnimSetFinish( 0, 0, (void *)IfaceRenderHandSlotCb, -1 );
    obj = gIfcHandSlot[ gIfcHandSlotState ].obj;
    if( obj && obj->LightRadius > 4 ) AnimChangeItemStart( gObjDude, obj->LightRadius, 0 );
    if( a2 ){
        AnimUnk63( gObjDude, a2, -1 );
    } else {
        AnimUnk62( gObjDude, ArtMakeId( 1, gObjDude->ImgId & 0xFFF, 0, 0, gObjDude->Orientation + 1 ), -1 );
    }
    AnimSetFinish( 0, 0, (void *)IfaceAnimationEnd, -1 );
    if( AnimRegEnd() != -1 ){
	tmp = gIfcUiState;
        IfaceUiDisable();
        GmouseSetIfaceMode( 0 );
        GmouseLoadCursor( 26 );
        // wait for end of animation
        while( gIfcAnimate && !gMenuEscape ){
            InpUpdate();
        }
        GmouseLoadCursor( 0 );
        GmouseSetIsoMode();
        if( tmp ) IfaceUiEnable();
    }
}

int IfaceTurnBtOpen()
{
    if( gIfcWin == -1 ) return -1;
    if( !gIfcCombatState ) return -1;
    if( !( gIfcTurnDnImg = ArtGetBitmap( ArtMakeId( 6, 105, 0, 0, 0 ), 0, 0, &gIfcTurnDnImgObj ) ) ) return -1;
    if( !( gIfcTurnUpImg = ArtGetBitmap( ArtMakeId( 6, 106, 0, 0, 0 ), 0, 0, &gIfcTurnUpImgObj ) ) ) return -1;
    if( ( gIfcEndTurnBt = WinCreateButton(gIfcWin, 590, 43, 38, 22, -1, -1, -1, ' ', gIfcTurnDnImg, gIfcTurnUpImg, 0, 0 ) ) == -1 ) return -1;
    WinSetAnimH( gIfcEndTurnBt, gIfcTurnDnImg, gIfcTurnDnImg, gIfcTurnDnImg );
    WinSetClickSound( gIfcEndTurnBt, GSoundPlayCheck, GSoundPlayUnCheck );
    return 0;
}

void IfaceTurnBtClose()
{
    if( gIfcWin == -1 ) return;
    if( gIfcEndTurnBt != -1 ){
        WinImageRemove( gIfcEndTurnBt );
        gIfcEndTurnBt = -1;
    }
    if( gIfcTurnUpImg ){
        ArtClose( gIfcTurnUpImgObj );
        gIfcTurnUpImgObj = NULL;
        gIfcTurnUpImg = NULL;
    }
    if( gIfcTurnDnImg ){
        ArtClose( gIfcTurnDnImgObj );
        gIfcTurnDnImgObj = NULL;
        gIfcTurnDnImg = NULL;
    }
}

int IfaceCmbtBtOpen()
{
    if( gIfcWin == -1 ) return -1;
    if( !gIfcCombatState ) return -1;
    if( !(gIfcCmbtBtDnImg = ArtGetBitmap( ArtMakeId( 6, 107, 0, 0, 0 ), 0, 0, &gIfcCmbtBtDnImgObj ) ) ) return -1;
    if( !(gIfcCmbtBtUpImg = ArtGetBitmap( ArtMakeId( 6, 108, 0, 0, 0 ), 0, 0, &gIfcCmbtBtUpImgObj ) ) ) return -1;
    if( (gifcCmbtBt = WinCreateButton( gIfcWin, 590, 65, 38, 22, -1, -1, -1, 13, gIfcCmbtBtDnImg, gIfcCmbtBtUpImg, 0, 0 ) ) == -1 ) return -1;
    WinSetAnimH( gifcCmbtBt, gIfcCmbtBtDnImg, gIfcCmbtBtDnImg, gIfcCmbtBtDnImg );
    WinSetClickSound( gifcCmbtBt, GSoundPlayCheck, GSoundPlayUnCheck );
    return 0;
}

void IfaceCmbtBtClose()
{
    if( gIfcWin == -1 ) return;
    if( gifcCmbtBt != -1 ){
        WinImageRemove( gifcCmbtBt );
        gifcCmbtBt = -1;
    }
    if( gIfcCmbtBtUpImg ){
        ArtClose( gIfcCmbtBtUpImgObj );
        gIfcCmbtBtUpImgObj = NULL;
        gIfcCmbtBtUpImg = NULL;
    }
    if( gIfcCmbtBtDnImg ){
        ArtClose( gIfcCmbtBtDnImgObj );
        gIfcCmbtBtDnImgObj = NULL;
        gIfcCmbtBtDnImg = NULL;
    }
}

void IfaceRenderAmmo( int a1, int a2)
{
    int v3;
    char *i;
    VidRect_t Area;

    if( (a2 & 1) != 0 ) a2--;
    v3 = 70;
    for( i = gIfcSurf + a1 + 26 * 640; v3 > a2; i += 640 ){
        --v3;
        *i = 14;
    }
    for( ;a2 > 0; a2 -= 2 ){
        *i = 196;
        *(i + 640) = 14;
        i += 2*640;
    }
    if( !gIfcUnk01 ){
        Area.lt = a1;
        Area.tp = 26;
        Area.bm = 96;
        Area.rt = a1 + 1;
        WinAreaUpdate( gIfcWin, &Area );
    }
}

int IfaceUnk36()
{
    int err;

    if( gIfcWin == -1 ) return -1;
    err = 0;
    while( Item50( gObjDude, gIfcHandSlot[ gIfcHandSlotState ].obj ) != -1 ) err = 1;
    IfaceUnk17();
    IfaceHandSlotUpdate( 0, -1, -1 );
    if( !err ) return -1;
    GSoundPlay( GSoundWeaponFileName( 0, gIfcHandSlot[ gIfcHandSlotState ].obj, 2, 0 ) );
    return 0;
}

#define COUNTER_UPDATE( area, time )   MseUpdate(); GmouseProcess(); TimerWait( time ); WinAreaUpdate( gIfcWin, area )
void IfaceRenderCounter( int Xpos, int Ypos, int CurrVal, int NewVal, int ColorSel, unsigned int Fps )
{
    VidRect_t Area;
    char *surf, *pDigImg;
    int dig2,dig1,dig0,val,Sign,dir;

    dig2 = dig1 = 0;
    // limit
    if( NewVal > 999 ) NewVal = 999; 
    if( NewVal < -999 ) NewVal = -999;
    pDigImg = gIfcImg21 + ColorSel;
    surf = gIfcSurf + 640 * Ypos + Xpos;
    if( gIfcUnk01 || !Fps ){
        val = abs32( NewVal );
        Sign = ( NewVal >= 0 ) ? 1 : -1;
    } else {
        val = CurrVal;
        Sign = ( CurrVal >= 0 ) ? 1 : -1;
    }
    dig0 = val % 10;
    if( val > 9 ){
        dig1 = val / 10 % 10;
        if( val > 99 ) dig2 = val / 100;
    }
    // render current counter value ( or new one if fps=0 )
    ScrCopy( pDigImg + 9 * dig2, 9, 17, 360, surf + 6, 640  ); 	// 100' digit
    ScrCopy( pDigImg + 9 * dig1, 9, 17, 360, surf + 15, 640 );	// 10' digit
    ScrCopy( pDigImg + 9 * dig0, 9, 17, 360, surf + 24, 640 ); 	// 1' digit
    ScrCopy( pDigImg + (( Sign >= 0 ) ? 114 : 108), 6, 17, 360, surf, 640 ); // sign
    if( gIfcUnk01 ) return; // no update
    Area.lt = Xpos;
    Area.tp = Ypos;
    Area.rt = Xpos + 33;
    Area.bm = Ypos + 17;
    WinAreaUpdate( gIfcWin, &Area );
    if( Fps == 0 ) return; // no animate

    // start counter animation
    dir = (CurrVal >= 0 ? 1 : -1) * ((NewVal - CurrVal) >= 0 ? 1 : -1);
    while( CurrVal != NewVal ){
        if( !(dig2 | dig1 | dig0) ) dir = 1;
        ScrCopy( pDigImg + 90, 9, 17, 360, surf + 24, 640 );
        COUNTER_UPDATE( &Area, Fps );
        dig0 += dir;
        if( dig0 >= 10 ){
            ScrCopy(pDigImg + 90, 9, 17, 360, surf + 15, 640);
    	    COUNTER_UPDATE( &Area, Fps );
            dig1 += dir;
            dig0 -= 10 * dir;
            if( dig1 == 10 || dig1 == -1 ){
                ScrCopy( pDigImg + 90, 9, 17, 360, surf + 6, 640 );
    		COUNTER_UPDATE( &Area, Fps );
                dig2 += dir;
                dig1 -= 10 * dir;
                if( dig2 == 10 || dig2 == -1 ) dig2 -= 10 * dir;
                ScrCopy( pDigImg + 99, 9, 17, 360, surf + 6, 640 );
    		COUNTER_UPDATE( &Area, Fps );
            }
            ScrCopy( pDigImg + 99, 9, 17, 360, surf + 15, 640 );
            TimerWait( Fps );
            WinAreaUpdate( gIfcWin, &Area );
        }
        ScrCopy( pDigImg + 99, 9, 17, 360, surf + 24, 640 );
    	COUNTER_UPDATE( &Area, Fps );
        CurrVal += dir;
        ScrCopy( (pDigImg + 9 * dig2), 9, 17, 360, surf + 6, 640 );
        ScrCopy( (pDigImg + 9 * dig1), 9, 17, 360, surf + 15, 640 );
        ScrCopy( (pDigImg + 9 * dig0), 9, 17, 360, surf + 24, 640 );
        ScrCopy( ( CurrVal >= 0 ) ? pDigImg + 114 : pDigImg + 108, 6, 17, 360, surf, 640 );
    	COUNTER_UPDATE( &Area, Fps );
    }
}

void IfaceDestroy()
{
    IfaceClose();
}

int IfaceIndicatorBoxInit()
{
    int Current, color, i, FontId, w, h;
    char *Img, stmp[ 1024 ];
    CachePool_t *ImgObj;
    MsgLine_t Line;
    Msg_t msg;

    i = 0;
    Current = FontGetCurrent();
    FontId = Current;
    if( gIfcIndBoxWin != -1 ) return 0; // already initialized
    // load messages
    if( MessageInit( &msg ) != 1 ){ eprintf( "\nINTRFACE: Error indicator box messages! **\n" ); return -1; }
    sprintf( stmp, "%s%s", gGamePath, "intrface.msg" );
    if( MessageLoad( &msg, stmp ) != 1 ){ eprintf( "\nINTRFACE: Error indicator box messages! **\n" ); return -1; }
    // load window decoration
    if( !(Img = ArtLoadBmp( ArtMakeId( 6, 126, 0, 0, 0 ), &ImgObj, &w, &h ) ) ){
        eprintf( "\nINTRFACE: Error initializing indicator box graphics! **\n" );
        MessageClose( &msg );
        return -1;
    }
    // initialize sufrfaces for text
    for( i = 0; i < 5; i++ ){
        if( !(gIfcIndBox[ i ].Img = Malloc( TEXT_BOX_SIZE ) ) ){
	    eprintf( "\nINTRFACE: Error initializing indicator box graphics! **\n" );
	    while( 1 ){
    		i--;
    		if( !i ) break;
    		Free( gIfcIndBox[ i ].Img );
	    }
	    MessageClose( &msg );
	    ArtClose( ImgObj );
	    return -1;
        }
    }
    // generate texts
    FontSet( 101 );
    for( i = 0; i < 5; i++){
	switch( gIfcIndBox[ i ].Color ){
	    case 0:  color = INFO_BOX_COLOR_INFO; break;
	    default: color = INFO_BOX_COLOR_WARN; break;
	}
        strcpy( stmp, MessageGetMessage( &msg, &Line, gIfcIndBox[ i ].TextId ) );
        memcpy( gIfcIndBox[ i ].Img, Img, TEXT_BOX_SIZE );
        gFont.Print( gIfcIndBox[ i ].Img + (TEXT_BOX_W - gFont.LineWidth( stmp ) ) / 2 + TEXT_BOX_W * (24 - gFont.ChrHeight()) / 2, stmp, TEXT_BOX_W, TEXT_BOX_W, color );
    }

    gIfcIndicatorBoxVisible = 1;
    IfaceIndicatorBoxUpdate();
    MessageClose( &msg );
    ArtClose( ImgObj );
    FontSet( FontId );
    return 0;
}

void IfaceIndicatorBoxClose()
{
    int i;

    if( gIfcIndBoxWin != -1 ){
        WinClose( gIfcIndBoxWin );
        gIfcIndBoxWin = -1;
    }
    for( i = 0; i != 5; i++ ){
        if ( gIfcIndBox[ i ].Img ){
            Free( gIfcIndBox[ i ].Img );
            gIfcIndBox[ i ].Img = NULL;
        }
    }
}

void IfaceIndicatorBoxDestroy()
{
    if( gIfcIndBoxWin != -1 ){
        WinClose( gIfcIndBoxWin );
        gIfcIndBoxWin = -1;
    }
    gIfcIndicatorBoxVisible = 1;
}

int IfaceIndicatorBoxUpdate()
{
    int k, i;

    k = 0;
    if( gIfcWin == -1 || !gIfcIndicatorBoxVisible || gIfcPanelState ){
        if( gIfcIndBoxWin != -1 ){
            WinClose( gIfcIndBoxWin );
            gIfcIndBoxWin = -1;
        }
        return 0;
    }
    for( i = 0; i < 6; i++ ) gIfcBarSlots[ i ] = -1;
    if( CritterUsingSkill( 0 ) ) k = IfaceIndicatorBoxAddEffect( 1 );
    if( CritterUsingSkill( 3 ) ) k += IfaceIndicatorBoxAddEffect( 2 );
    if( CritterUsingSkill( 4 ) ) k += IfaceIndicatorBoxAddEffect( 0 );
    if( CritterPoisoned( gObjDude ) > 0 ) k += IfaceIndicatorBoxAddEffect( 3 );
    if( CritterRadiated( gObjDude ) > 65 )  k += IfaceIndicatorBoxAddEffect( 4 );
    if( k > 1 ) qsort( gIfcBarSlots, k, sizeof( int ), (void *)IfaceIndicatorBoxSortCb );
    if( gIfcIndBoxWin != -1 ){ WinClose( gIfcIndBoxWin ); gIfcIndBoxWin = -1; }
    if( k == 0 ) return 0;    
    gIfcIndBoxWin = WinCreateWindow( 0, 358, 127 * k, TEXT_BOX_H, gPalColorCubeRGB[0][0][0], 0 );
    IfaceIndicatorBoxRender( k );
    WinUpdate( gIfcIndBoxWin );    
    return k;
}

static int IfaceIndicatorBoxSortCb( int *p1, int *p2 )
{
    if( *p1 == *p2 ) return 0;
    if( *p1 <= *p2 ) return -1;
    return 1;
}

void IfaceIndicatorBoxRender( int cnt )
{
    int Width,ofs,xpos, i;
    char *surf;

    Width = WinGetWidth( gIfcIndBoxWin );
    surf = WinGetSurface( gIfcIndBoxWin );
    if( gIfcIndBoxWin == -1 ) return;    
    ofs = 3;
    xpos = 0;
    for( i = 0; i < cnt; i++ ){
        ScrCopyAlpha( gIfcIndBox[ gIfcBarSlots[ i ] ].Img + ofs, TEXT_BOX_W - ofs, TEXT_BOX_H, TEXT_BOX_W, &surf[ xpos ], Width );
        ofs = 0;
        xpos = i*130 - 3 * (i + 2);
    }            
}

int IfaceIndicatorBoxAddEffect( int Effect )
{
    int i;

    for( i = 0; i < 5 && gIfcBarSlots[ i ] != -1; i++ );
    if( i >= 5 ){
        eprintf( "\nINTRFACE: no free bar box slots!\n" );
        return 0;
    }
    gIfcBarSlots[ i ] = Effect;    
    return 1;
}

void IfaceIndicatorBoxShow()
{
    gIfcIndicatorBoxVisible = 1;
    IfaceIndicatorBoxUpdate();
}

int IfaceIndicatorBoxHide()
{
    int tmp;
    
    tmp = gIfcIndicatorBoxVisible;
    gIfcIndicatorBoxVisible = 0;
    IfaceIndicatorBoxUpdate();
    return tmp;
}

