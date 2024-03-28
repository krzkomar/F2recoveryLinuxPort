#include "FrameWork.h"

extern int gDlgPmExchangeMode;

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
Obj_t *gInvUnk84;
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
    { 0x00, 0x001, 0x030, 0x1F3, 0x179 },
    { 0x50, 0x000, 0x071, 0x124, 0x178 },
    { 0x50, 0x000, 0x072, 0x219, 0x178 },
    { 0x50, 0x000, 0x06F, 0x1E0, 0x0B4 },
    { 0x50, 0x122, 0x131, 0x103, 0x0A2 },
};
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
    INV_ACT_INSPECT, 
    INV_ACT_USE, 
    INV_ACT_THROW, 
    INV_ACT_CANCEL 
};

int gInvUnk90[3] = { 
    INV_ACT_INSPECT, 
    INV_ACT_THROW, 
    INV_ACT_CANCEL 
};

int gInvUnk91[3] = { 
    INV_ACT_INSPECT, 
    INV_ACT_USE, 
    INV_ACT_CANCEL 
};

int gInvUnk92[2] = { 
    INV_ACT_INSPECT, 
    INV_ACT_CANCEL 
};

int gInvMenuIconWeaponOwned[4] = { 
    INV_ACT_INSPECT, 
    INV_ACT_UNLOAD, 
    INV_ACT_THROW, 
    INV_ACT_CANCEL 
};

int gInvMenuIconWeapon[3] = { 
    INV_ACT_INSPECT, 
    INV_ACT_UNLOAD, 
    INV_ACT_CANCEL
};

int gInvBarterDiscount = 0;

int gInvUnk12[ 7 ] = { 35, 9, 17, 18, 19, 20, 23 };
int gInvUnk13[ 7 ] = { 7, -1, 24, 25, 26, 27, 30 };
int gInvUnk100[ 4 ] = { 122, 123, 124, 125 };

void InvSetBodyImg();
static void InvEnterRegionCb( int , int RegionNb );
static void InvLeaveRegionCb();
void InvEquip( int sel, int SlotOffset );
void InvActionMenu( int sel, int mode );
int InvPopUpDlg( int mode, Obj_t *Obj, int quantity );
void InvPopUpClose( int Mode );
void InvUpdateStatistics( Obj_t *critter, Obj_t *Item1, Obj_t *Item2 );
int InvPickItem( int sel, Obj_t **PickedItem, Obj_t **BoxObj, Obj_t ***RhandObj );
void InvWearItem( Obj_t *PickedPtr, Obj_t **Slot, Obj_t **ptr, int Pos );
int InvPopUpDlgCreate( int mode, Obj_t *Item );
int InvLoadAmmo( Obj_t *item0, Obj_t *item1, Obj_t **item2, int quantity, int sel );
int InvPickUpBoxQuantity( Obj_t *Critter, Obj_t *Item1, int Quantity, Obj_t **Item2, int quantity );
void InvSetInfoMode( int a1 );
void InvBodyUpdate( int a1 ,int a2 );
Obj_t *InvGetArmorObj( Obj_t *obj );
/*****************************************************************/

void InvSelectCritter( Obj_t *Critter, int Pid )
{
    gInvSelectedPid = Pid;
    gInvSelectedDude = Critter;
}

void InvSelectMain()
{
    gInvSelectedDude = gObjDude;
    gInvSelectedPid = 0x1000000;
}

int InvMsgOpen()
{
    char fname[260];

    if( MessageInit( &gInvMsg ) != 1 ) return -1;
    sprintf( fname, "%s%s", gGamePath, "inventry.msg" );
    if( MessageLoad( &gInvMsg, fname ) != 1 ) return -1;
    return 0;
}

int InvMsgClose()
{
    MessageClose( &gInvMsg );
    return 0;
}

void InvMenu()
{
    int tmp,i, menu,sel;
    Obj_t *armor;
    MsgLine_t MsgLine;
    VidRect_t Area;

    if( IN_COMBAT && CombatUnk05() != gInvSelectedDude ) return;
    if( InvInit() == -1 ) return;
    if( (gCombatStatus & 1) && gInvSelectedDude == gObjDude ){ // access inventory in combat
        tmp = INV_AP_ACCESS_COST - 2 * PerkLvl( gInvSelectedDude, 48 ); // AP cost for acccess to inventory
        if( (tmp > 0) && (gObjDude->Critter.State.CurrentAP < tmp) ){
            MsgLine.Id = 19; // 'You don't have enough action points to use inventory'
            if( MessageGetMsg( &gInvMsg, &MsgLine ) == 1 ) IfcMsgOut( MsgLine.Text );
            for( i = 0; i < 5; i++ ) ArtClose( gInvMseCursor[ i ].Obj );
            if( gInvUnk02 ) GameIfaceDisable( 0 );
            InvMsgClose();
            gInvArt[0].Xpos = 0;
            return;
        }
        if( tmp > 0 ){
            if( tmp > gObjDude->Critter.State.CurrentAP )
                gObjDude->Critter.State.CurrentAP = 0;
            else
                gObjDude->Critter.State.CurrentAP -= tmp; // spent AP
            IfaceRenderAP( gObjDude->Critter.State.CurrentAP, gCombatMovePts );
        }
    }

    armor = InvGetArmorObj( gInvSelectedDude );
    menu = InvMenuCreate( 0 );
    AnimRegClear( gInvSelectedDude );
    InvStatsUpdate();
    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 0 );
    InvSetInfoMode( 0 );
    while( 1 ){
        sel = InpUpdate();
        if( sel == KEY_ESC || gMenuEscape ) break;
        InvBodyUpdate( -1, 0 );
        if( GlobFloatMsgState() == 5 ) break;
	switch( sel ){
    	    case 17: case 24: case 324: SysQuitDlg(); break;
    	    case 327:
        	gInvUnk04[ gInvUnk05 ] = 0;
        	InvBpUpdate( 0, -1, 0 );
        	break;
    	    case 328: // arrow UP
        	if( gInvUnk04[ gInvUnk05 ] <= 0 ) break;
            	gInvUnk04[ gInvUnk05 ]--;
            	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 0);
        	break;        
    	    case 329:
        	gInvUnk04[ gInvUnk05 ] = gInvUnk04[ gInvUnk05 ] - gInvUnk08;
        	if( gInvUnk04[ gInvUnk05 ] < 0 ) gInvUnk04[ gInvUnk05 ] = 0;
        	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 0);
        	break;
    	    case 335:
        	gInvUnk04[ gInvUnk05 ] = gInvBackPack->Box.Cnt - gInvUnk08;
        	if( gInvUnk04[ gInvUnk05 ] < 0 ) gInvUnk04[ gInvUnk05 ] = 0;
        	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 0 );
        	break;                               
    	    case 336: // Arrow DN
        	if( gInvUnk08 + gInvUnk04[ gInvUnk05 ] >= gInvBackPack->Box.Cnt ) break;
        	gInvUnk04[ gInvUnk05 ]++;
        	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 0 );
        	break;
    	    case 337:
        	gInvUnk04[ gInvUnk05 ] = gInvUnk08 + gInvUnk04[ gInvUnk05 ];
        	if( gInvUnk04[ gInvUnk05 ] + gInvUnk08 >= gInvBackPack->Box.Cnt ){
            	    tmp = gInvBackPack->Box.Cnt - gInvUnk08;
            	    gInvUnk04[ gInvUnk05 ] = gInvBackPack->Box.Cnt - gInvUnk08;
            	    if( tmp < 0 ) gInvUnk04[ gInvUnk05 ] = 0;
        	}
        	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 0 );
        	break;                
    	    case 2500: // click on portrait
    		InvUnk46( 2500, 0 ); 
		break;    	
	    case 1000 ... 1008: // left mouse button pressed
    		if( !( MseGetButtons() & MSE_LBUTT ) ) break;
                if( gInvInfoMode == 1 )
                    InvActionMenu( sel, 0 );
                else
                    InvEquip( sel, gInvUnk04[ gInvUnk05 ] );
        	break;
    	    default: // right mouse button pressed
    		if( !(MseGetButtons() & MSE_RBUTT )) break;
                if( gInvInfoMode ){ // toggle mode
                    if( gInvInfoMode == 1 ){
                        InvSetInfoMode( 0 );
                        InvStatsUpdate();
                        WinUpdate( gInvWin );
                    }
                } else {
                    InvSetInfoMode( 1 );
                }
    		break;
	}
    }
    gInvSelectedDude = gInvParty[0];
    InvSetBodyImg();
    if( gInvSelectedDude == gObjDude ){
        ObjSetShape( gInvSelectedDude, gInvBodyImgId, &Area );
        TileUpdateArea( &Area, gInvSelectedDude->Elevation );
    }
    // armor changed ?
    if( (gInvSelectedDude == gObjDude) && (armor != InvGetArmorObj( gInvSelectedDude )) ) IfaceResetAC( 1 );
    InvMenuClose( menu ) ;
    for( i = 0; i != 5; i++ ) ArtClose( gInvMseCursor[ i ].Obj );
    if( gInvUnk02 ) GameIfaceDisable( 0 );
    InvMsgClose();
    gInvArt[ 0 ].Xpos = 0;
    if( gInvSelectedDude == gObjDude ) IfaceHandSlotUpdate( 0, -1, -1 );
}

int InvMenuCreate( unsigned int idx )
{	
    char *img1, *img2, *img3;
    int win,i,bt,j,k;
    CachePool_t *Obj;
    Obj_t *p;

    gInvUnk83 = 0;
    gInvUnk05 = 0;
    gInvUnk04[0] = 0;
    gInvUnk08 = 6;
    gInvBackPack = &gInvSelectedDude->Container;
    gInvParty[0] = gInvSelectedDude;
    if( idx <= 2 ){
        win = WinCreateWindow( 80, 0, gInvArt[ idx ].Width, gInvArt[ idx ].Height, 257, 20 );
        gInvWin = win;
        gInvUnk15 = gInvArt[ idx ].Width + 80;
        gInvUnk16 = gInvArt[ idx ].Height;
        if( (img1 = ArtGetBitmap( ArtMakeId( 6, gInvArt[ idx ].ArtId, 0, 0, 0 ), 0, 0, &Obj ) ) ){
            ScrCopy( img1, gInvArt[ idx ].Width, gInvArt[ idx ].Height, gInvArt[ idx ].Width, WinGetSurface( win ), gInvArt[ idx ].Width );
            ArtClose( Obj );
        }
        gInvInfoCb = IfcMsgOut;
    } 
    if( idx == 3 ){
        if( gInvUnk18 == -1 ) exit( 1 );
        gInvUnk08 = 3;
        gInvWin = WinCreateWindow( 80, 290, 480, 180, 257, 0 );
        gInvUnk15 = 560;
        gInvUnk16 = 470;
        ScrCopy( WinGetSurface( gInvUnk18 ) + 80, 480, 180, gVidMainGeo.rt - gVidMainGeo.lt + 1, WinGetSurface( gInvWin ), 480 ); // background
        gInvInfoCb = GdialogReply;
    }
    if( (idx != 3) && (idx != 2) ){
        for( i = 0; i < gInvUnk08; i++ ){ // mouse regions for items
            bt = WinCreateButton( gInvWin, 44, 48 * (gInvUnk08 - i - 1) + 35, 64, 48, gInvUnk08 - i + 999, -1, gInvUnk08 - i + 999, -1, 0, 0, 0, 0 );
            if( bt != -1 ) WinSetButtonHandler( bt, InvEnterRegionCb, InvLeaveRegionCb, 0, 0 );
        }
    }
    if( idx == 2 ){
        for( j = 0; j < gInvUnk08; j++ ){
            bt = WinCreateButton(gInvWin, 176, 48 * (gInvUnk08 - j - 1) + 37, 64, 48, gInvUnk08 - j + 999, -1, gInvUnk08 - j + 999, -1, 0, 0, 0, 0);
            if( bt != -1 ) WinSetButtonHandler( bt, InvEnterRegionCb, InvLeaveRegionCb, 0, 0 );
        }
        for( k = 277, j = 2005; k != -11; k -= 48, j-- ){
            bt = WinCreateButton( gInvWin, 297, k, 64, 48, j, -1, j, -1, 0, 0, 0, 0 );
            if( bt != -1 ) WinSetButtonHandler( bt, InvEnterRegionCb, InvLeaveRegionCb, 0, 0 );
        }
    } else if( idx == 3 ){
        for( i = 0; i < gInvUnk08; i++ ){
            bt = WinCreateButton( gInvWin, 29,  35 + i*48, 64, 48, 1000+i, -1, 1000+i, -1, 0, 0, 0, 0 );
            if( bt != -1 ) WinSetButtonHandler( bt, InvEnterRegionCb, InvLeaveRegionCb, 0, 0 );
            bt = WinCreateButton( gInvWin, 395, 35 + i*48, 64, 48, 2000+i, -1, 2000+i, -1, 0, 0, 0, 0 );
            if( bt != -1 ) WinSetButtonHandler( bt, InvEnterRegionCb, InvLeaveRegionCb, 0, 0 );
            bt = WinCreateButton( gInvWin, 165, 20 + i*48, 64, 48, 2300+i, -1, 2300+i, -1, 0, 0, 0, 0 );
            if( bt != -1 ) WinSetButtonHandler( bt, InvEnterRegionCb, InvLeaveRegionCb, 0, 0 );
            bt = WinCreateButton( gInvWin, 250, 20 + i*48, 64, 48, 2400+i, -1, 2400+i, -1, 0, 0, 0, 0 );
            if( bt != -1 ) WinSetButtonHandler( bt, InvEnterRegionCb, InvLeaveRegionCb, 0, 0 );                
        }
    }
    if( idx == 0 ){
        bt = WinCreateButton(gInvWin, 245, 286, 90, 61, 1006, -1, 1006, -1, 0, 0, 0, 0);
        if( bt != -1 ) WinSetButtonHandler(bt, InvEnterRegionCb, InvLeaveRegionCb, 0, 0);
        bt = WinCreateButton(gInvWin, 154, 286, 90, 61, 1007, -1, 1007, -1, 0, 0, 0, 0);
        if( bt != -1 ) WinSetButtonHandler(bt, InvEnterRegionCb, InvLeaveRegionCb, 0, 0);
        bt = WinCreateButton(gInvWin, 154, 183, 90, 61, 1008, -1, 1008, -1, 0, 0, 0, 0);
        if( bt != -1 ) WinSetButtonHandler(bt, InvEnterRegionCb, InvLeaveRegionCb, 0, 0);
    }

    memset( gInvUnk20, 0, sizeof( gInvUnk20 ) );    
    if( (img2 = ArtGetBitmap( ArtMakeId( 6, 8, 0, 0, 0 ), 0, 0, &gInvUnk20[ 0 ] )) ){  // exit button
        if( (img1 = ArtGetBitmap( ArtMakeId( 6, 9, 0, 0, 0 ), 0, 0, &gInvUnk20[ 1 ] )) ){
            if( idx == 0 ){
                bt = WinCreateButton( gInvWin, 437, 329, 15, 16, -1, -1, -1, 27, img2, img1, 0, 32 );
                if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
            } else if( idx == 1 ){
                bt = WinCreateButton( gInvWin, 233, 328, 15, 16, -1, -1, -1, 27, img2, img1, 0, 32 );
                if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
            } else if( idx == 2 ){
                bt = WinCreateButton( gInvWin, 476, 331, 15, 16, -1, -1, -1, 27, img2, img1, 0, 32 );
                if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
            }
        }
    }
    if( idx == 3 ){
        img1 = ArtGetBitmap( ArtMakeId( 6, ArtMakeId( 6, 100, 0, 0, 0 ), 0, 0, 0 ), 0, 0, &gInvUnk20[ 2 ] );
        if( img1 ){
            img2 = ArtGetBitmap( ArtMakeId( 6, ArtMakeId( 6, 101, 0, 0, 0 ), 0, 0, 0 ), 0, 0, &gInvUnk20[3] );
            if( img2 ){
                 bt = WinCreateButton( gInvWin, 109, 56, 23, 24, -1, -1, 328, -1, img1, img2, 0, 0 );
                if( bt != -1 ) WinSetClickSound(bt, GSoundPlayPushBt, GSoundPlayReleaseBt);
                bt = WinCreateButton( gInvWin, 342, 56, 23, 24, -1, -1, 397, -1, img1, img2, 0, 0 );
                if( bt != -1 ) WinSetClickSound(bt, GSoundPlayPushBt, GSoundPlayReleaseBt);
            }
        }
    } else {
	// arrow UP
        if( (img1 = ArtGetBitmap( ArtMakeId(6, 49, 0, 0, 0), 0, 0, &gInvUnk20[ 2 ])) ) {
            if( (img2 = ArtGetBitmap( ArtMakeId(6, 50, 0, 0, 0), 0, 0, &gInvUnk20[ 3 ])) ){
                if( (img3 = ArtGetBitmap( ArtMakeId( 6, 53, 0, 0, 0 ), 0, 0, &gInvUnk20[ 4 ] )) ){
                    if( idx != 3 ){
                        gInvUpArrow = WinCreateButton( gInvWin, 128, 39, 22, 23, -1, -1, 328, -1, img1, img2, 0, 0 );
                        if( gInvUpArrow != -1 ){
                            WinSetAnimH( gInvUpArrow, img3, img3, img3 );
                            WinSetClickSound( gInvUpArrow, GSoundPlayPushBt, GSoundPlayReleaseBt );
                            WinDisableWidget( gInvUpArrow );
                        }
                    }
                    if( idx == 2 ){
                        gInvUnk36 = WinCreateButton(gInvWin, 379, 39, 22, 23, -1, -1, 397, -1, img1, img2, 0, 0);
                        if( gInvUnk36 != -1 ){
                            WinSetAnimH( gInvUnk36, img3, img3, img3);
                            WinSetClickSound( gInvUnk36, GSoundPlayPushBt, GSoundPlayReleaseBt);
                            WinDisableWidget( gInvUnk36 );
                        }
                    }
                }
            }
        }
    }
    if( idx == 3 ){
        img1 = ArtGetBitmap( ArtMakeId( 6, ArtMakeId( 6, 93, 0, 0, 0 ), 0, 0, 0 ), 0, 0, &gInvUnk20[ 5 ] );
        if( img1 ){
            img2 = ArtGetBitmap( ArtMakeId( 6, ArtMakeId( 6, 94, 0, 0, 0 ), 0, 0, 0 ), 0, 0, &gInvUnk20[ 6 ] );
            if( img2 ){
                bt = WinCreateButton( gInvWin, 109, 82, 24, 25, -1, -1, 336, -1, img1, img2, 0, 0 );
                if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
                bt = WinCreateButton(gInvWin, 342, 82, 24, 25, -1, -1, 401, -1, img1, img2, 0, 0);
                if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
            }
        }
        WinCreateButton( gInvUnk18, 15, 25, 60, 100, -1, -1, 2500, -1, 0, 0, 0, 0 ); 
        WinCreateButton( gInvUnk18, 560, 25, 60, 100, -1, -1, 2501, -1, 0, 0, 0, 0 );
    } else {
	// arrow DN
        if( (img1 = ArtGetBitmap( ArtMakeId( 6, 51, 0, 0, 0 ), 0, 0, &gInvUnk20[5] ) ) ){
            if( (img2 = ArtGetBitmap( ArtMakeId( 6, 52, 0, 0, 0 ), 0, 0, &gInvUnk20[6] )) ){
                if( (img3 = ArtGetBitmap( ArtMakeId( 6, 54, 0, 0, 0 ), 0, 0, &gInvUnk20[7] ) ) ){
                    gInvDnArrow = WinCreateButton( gInvWin, 128, 62, 22, 23, -1, -1, 336, -1, img1, img2, 0, 0 );
                    WinSetClickSound( gInvDnArrow, GSoundPlayPushBt, GSoundPlayReleaseBt );
                    WinSetAnimH( gInvDnArrow, img3, img3, img3 );
                    WinDisableWidget( gInvDnArrow );
                    if( idx == 2 ){
                        WinCreateButton( gInvWin, 44, 35, 60, 100, -1, -1, 2500, -1, 0, 0, 0, 0 );
                        gInvUnk37 = WinCreateButton( gInvWin, 379, 62, 22, 23, -1, -1, 401, -1, img1, img2, 0, 0 );
                        if( gInvUnk37 != -1 ){
                            WinSetClickSound( gInvUnk37, GSoundPlayPushBt, GSoundPlayReleaseBt );
                            WinSetAnimH( gInvUnk37, img3, img3, img3 );
                            WinDisableWidget( gInvUnk37 );
                        }
                        WinCreateButton( gInvWin, 422, 35, 60, 100, -1, -1, 2501, -1, 0, 0, 0, 0 );
                    } else {
                        WinCreateButton( gInvWin, 176, 37, 60, 100, -1, -1, 2500, -1, 0, 0, 0, 0 ); // portrait
                    }
                }
            }
        }
    }
    if( idx == 3 ){
	if( (img1 = ArtGetBitmap( ArtMakeId( 6, 49, 0, 0, 0 ), 0, 0, &gInvUnk20[ 8 ] )) ){    	    
    	    if( (img2 = ArtGetBitmap( ArtMakeId( 6, 50, 0, 0, 0 ), 0, 0, &gInvUnk20[ 9 ] )) ){
        	bt = WinCreateButton( gInvWin, 128, 113, 22, 23, -1, -1, 329, -1, img1, img2, 0, 0 );
        	if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt);
        	bt = WinCreateButton( gInvWin, 333, 113, 22, 23, -1, -1, 388, -1, img1, img2, 0, 0 );
        	if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    	    }
	}	
	if( (img1 = ArtGetBitmap( ArtMakeId( 6, 51, 0, 0, 0 ), 0, 0, &gInvUnk20[ 10 ] ) ) ){    	    
    	    if( (img2 = ArtGetBitmap( ArtMakeId( 6, 52, 0, 0, 0 ), 0, 0, &gInvUnk20[ 11 ] ) ) ){
        	bt = WinCreateButton( gInvWin, 128, 136, 22, 23, -1, -1, 337, -1, img1, img2, 0, 0 );
        	if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
        	bt = WinCreateButton( gInvWin, 333, 136, 22, 23, -1, -1, 374, -1, img1, img2, 0, 0 );
        	if ( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    	    }
	}
    } else {	
        if( idx == 2 && !gSkillUnk80 ){
    	    if( (img1 = ArtGetBitmap( ArtMakeId( 6, 436, 0, 0, gSkillUnk80 ), 0, 0, &gInvUnk20[ 8 ] ) ) ){
    		if( (img2 = ArtGetBitmap( ArtMakeId( 6, 437, 0, 0, gSkillUnk80 ), 0, 0, &gInvUnk20[ 9 ] )) ){
    		    if( (bt = WinCreateButton( gInvWin, 432, 204, 39, 41, -1, -1, 'A', -1, img1, img2, NULL, 0 )) != -1 ){
    			WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    		    }
    		}
    	    }
        }
    }

    gInvRHandObj = NULL;
    gInvArmorObj = NULL;
    gInvLHandObj = NULL;

    for( i= 0; i < gInvBackPack->Box.Cnt; i++ ){
        p = gInvBackPack->Box.Box[ i ].obj;
        if( p->Flags & 0x1000000 ){
            if( p->Flags & 0x2000000 ) gInvRHandObj = gInvBackPack->Box.Box[ i ].obj;
            gInvLHandObj = gInvBackPack->Box.Box[ i ].obj;
        } else if( p->Flags & 0x2000000 ){
            gInvRHandObj = gInvBackPack->Box.Box[ i ].obj;
        } else if( p->Flags & 0x4000000 ){
            gInvArmorObj = gInvBackPack->Box.Box[ i ].obj;
        }
    }

    if( gInvLHandObj ) ItemUseItem( gInvSelectedDude, gInvLHandObj, 1 );
    if( gInvRHandObj && gInvRHandObj != gInvLHandObj ) ItemUseItem( gInvSelectedDude, gInvRHandObj, 1 );
    if( gInvArmorObj ) ItemUseItem( gInvSelectedDude, gInvArmorObj, 1 );
    InvSetBodyImg();
    k = MapAmbientDisable();
    GmouseSetIfaceMode( 0 );
    return k;
}

void InvMenuClose( int eax0 )
{
    Scpt01_t v16;
    Combat_t a1;
    Obj_t *v11, *v14;
    int v9,v12,v13,GroupId;

    gInvSelectedDude = gInvParty[0];
    if( gInvLHandObj ){
        gInvLHandObj->Flags = gInvLHandObj->Flags | 0x01000000;
        if( gInvLHandObj == gInvRHandObj ) gInvRHandObj->Flags |= 0x02000000;
        ItemAdd( gInvSelectedDude, gInvLHandObj, 1 );
    }
    if( gInvRHandObj && gInvRHandObj != gInvLHandObj ){
        gInvRHandObj->Flags |= 0x02000000;
        ItemAdd( gInvSelectedDude, gInvRHandObj, 1 );
    }
    if( gInvArmorObj ){
        gInvArmorObj->Flags |= 0x04000000;
        ItemAdd( gInvSelectedDude, gInvArmorObj, 1);
    }
    v9 = 0;
    gInvRHandObj = 0;
    gInvArmorObj = 0;
    gInvLHandObj = 0;
    do{
        ArtClose( gInvUnk20[v9++ ] );
    } while ( v9 != 12 );
    if( eax0 ) MapAmbientEnable();
    WinClose(gInvWin);
    GmouseSetIsoMode();
    if( gInvUnk83 ){
        CombatSetUp(&a1, gObjDude, 0, 4, 3);
        a1.DudeInjuries = 256;
        a1.TileNo = gObjDude->GridId;
        CombatExplosion( &a1, 0, 0, 1 );
        GroupId = gObjDude->Critter.State.GroupId;        
        v11 = 0;
        v13 = 0;
        for( v12 = 0; v12 < a1.Count; v12++, v13++ ){
            v14 = a1.obj[v13];
            if( v14 != gObjDude && GroupId != v14->Critter.State.GroupId && FeatDice(v14, 1, 0, 0) >= 2 ){
                CritterUnk45(a1.obj[v13], gObjDude);
                if( !v11 ) v11 = a1.obj[v13];
            }                
        }            
        if( v11 ){
            if( !IN_COMBAT ){
                v16.obj = v11;
                v16.crit = gObjDude;
                memset(v16.i03, 0, sizeof(v16.i03));
                v16.i08 = 0;
                v16.i07 = 0x7FFFFFFF;
                ScptUnk121(&v16);
            }
        }
        gInvUnk83 = 0;
    }
}

void InvBpUpdate( int BpOffset, int Picked, int mode )
{
    CachePool_t *ImgObj1, *Obj2;
    int Pitch,w,i,Offset,j;
    char *surf;
    char *img;
    ArtFrmHdr_t *img1;
    ObjStack_t *Obj;

    surf = WinGetSurface( gInvWin );
    Offset = 0;
    switch( mode ){
        case 0:
            Pitch = 499;
            if( !(img = ArtGetBitmap( ArtMakeId( 6, 48, 0, 0, 0 ), 0, 0, &Obj2 )) ) break;
            ScrCopy( img + 17509, 64, 48 * gInvUnk08, 499, surf + 17509, 499 );
            ScrCopy( img + 91471, 90, 61, 499, surf + 91471, 499 );
            if( gInvLHandObj && ( gInvLHandObj == gInvRHandObj ) ){
                if( ( img1 = ArtLoadImg( ArtMakeId( 6, 32, 0, 0, 0 ), &ImgObj1 ) ) ){ // only one item cls 
                    w = ArtGetObjWidth( img1, 0, 0 );
                    ScrCopy( ArtGetObjData( img1, 0, 0 ), w, ArtGetObjHeight( img1, 0, 0 ), w, surf + 141868, 499 );
                    ArtClose( ImgObj1 );
                }
            } else {
                ScrCopy( img + 142868, 180, 61, 499, surf + 142868, 499 );
            }
            ArtClose( Obj2 );
            break;
        case 1:
            Pitch = 292;
            if( !(img = ArtGetBitmap( ArtMakeId(6, 113, 0, 0, 0), 0, 0, &Obj2 ) ) ) break;
            ScrCopy( img + 10264, 64, 48 * gInvUnk08, 292, surf + 10264, 292 );
            ArtClose( Obj2 );
            break;
        case 2:
            Pitch = 537;            
            if( !(img = ArtGetBitmap( ArtMakeId( 6, 114, 0, 0, 0 ), 0, 0, &Obj2 ) ) ) break;
            ScrCopy( img + 20045, 64, 48 * gInvUnk08, 537, surf + 20045, 537 );
            ArtClose( Obj2 );
            break;
        case 3:
            Pitch = 480;
            Offset = -20;
            ScrCopy( WinGetSurface( gInvUnk18 ) + 35 * (gVidMainGeo.rt - gVidMainGeo.lt + 1) + 100, 64, 48 * gInvUnk08, gVidMainGeo.rt - gVidMainGeo.lt + 1, (surf + 16820), 480 );
            break;
    }
    if( mode == 0 || mode == 1 || mode == 2 ){ // enable/disable arrows
        if( gInvUpArrow != -1 ){
            if( BpOffset <= 0 )
                WinDisableWidget( gInvUpArrow );
            else
                WinEnableWidget( gInvUpArrow );
        }
        if( gInvDnArrow != -1 ){
            if( gInvBackPack->Box.Cnt - BpOffset <= gInvUnk08 )
                WinDisableWidget( gInvDnArrow );
            else
                WinEnableWidget( gInvDnArrow );
        }
    }

    j = 39 * Pitch;
    for( i = 0; BpOffset + i < gInvBackPack->Box.Cnt && i < gInvUnk08; i++, j += 48 * Pitch ){
        if( mode == 3 ){
            img = surf + 26 + j;
            w = 59;
        } else {
            if( mode == 2 ){
                img = surf + 180 + Pitch * (41 + i*48);
            } else {
                img = surf + 48 + j;
            }
            w = 56;
        }
        Obj = &gInvBackPack->Box.Box[ gInvBackPack->Box.Cnt - (BpOffset + 1 + i) ];
        ArtLoadImgScaled( ItemGetArtId( Obj->obj ), img , w, 40, Pitch );
        if( mode == 2 )
            InvPrintQuantity( Obj->obj, Obj->Quantity, Pitch * ( i*48 + 41 ) + surf + Offset + 180, Pitch, i == Picked );
        else
            InvPrintQuantity( Obj->obj, Obj->Quantity, Pitch * ( i*48 + 39 ) + surf + Offset + 48, Pitch, i == Picked );
    }
    if( mode == 0 ){ //  left/right hand and armor slot
        if( gInvRHandObj ) ArtLoadImgScaled( ItemGetArtId( gInvRHandObj ), surf + 142959, ( gInvRHandObj == gInvLHandObj ) ? 180 : 90, 61, 499 ); // item 2
        if( gInvLHandObj && (gInvLHandObj != gInvRHandObj) ) ArtLoadImgScaled( ItemGetArtId( gInvLHandObj ), surf + 142868, 90, 61, 499); // item 1
        if( gInvArmorObj ) ArtLoadImgScaled( ItemGetArtId( gInvArmorObj ), surf + 91471, 90, 61, 499 ); // armor
    }
    WinUpdate( gInvWin );
}

void InvItemList( int a1, int a2, ObjBox_t *box, int Mode )
{
    char *surf, *bmp;
    int Pitch,i,n,k;
    CachePool_t *ImgObj;

    surf = WinGetSurface( gInvWin );
    if( Mode == 2 ){
        Pitch = 537;
        if( (bmp = ArtGetBitmap( ArtMakeId( 6, 114, 0, 0, 0 ), 0, 0, &ImgObj )) ){
            ScrCopy( bmp + 20166, 64, 48 * gInvUnk08, 537, surf + 20166, 537 );
            ArtClose( ImgObj );
        }
    } else if( Mode == 3 ){
        Pitch = 480;
        ScrCopy( 
    	    WinGetSurface( gInvUnk18 ) + (35 * (gVidMainGeo.rt - gVidMainGeo.lt + 1)) + 475, 
    	    64, 48 * gInvUnk08, gVidMainGeo.rt - gVidMainGeo.lt + 1, surf + 17195, 480 
    	); // Clear Merchant inventory background
    }

    k = a1 + 1;
    n = 0;
    for( i = 0; ( i < gInvUnk08 ) && ( i + a1 < box->Cnt ); k++, i++ ){
        if( Mode == 2 ){ n = Pitch * (i*48 + 41) + 301; } 
        if( Mode == 3 ){ n = Pitch * (i*48 + 39) + 397; }
        ArtLoadImgScaled( ItemGetArtId( box->Box[ box->Cnt - k ].obj ), surf + n, 56, 40, Pitch );
        InvPrintQuantity( box->Box[ box->Cnt - k ].obj, box->Box[ box->Cnt - k ].Quantity, surf + n, Pitch, (i == a2) );
    }

    if( Mode != 2 ) return;
    if( gInvUnk36 != -1 ){
        if( a1 <= 0 )
            WinDisableWidget( gInvUnk36 );
        else
            WinEnableWidget( gInvUnk36 );
    }
    if( gInvUnk37 != -1 ){
        if( box->Cnt - a1 <= gInvUnk08 )
            WinDisableWidget( gInvUnk37 );
        else
            WinEnableWidget( gInvUnk37 );
    }
}

void InvPrintQuantity( Obj_t *obj, int Quantity, char *surf, int Pitch, int Picked )
{
    int Current;
    char stmp[ 24 ];

    Current = FontGetCurrent();
    FontSet( 101 );

    if( ItemGetObjType( obj ) == PR_ITEM_AMMO ){
	Quantity--;
        Quantity *= ItemGetPackQuantity( obj );
        if( !Picked ) Quantity += ItemGetAmmo( obj );
        if( Quantity > 99999 ) Quantity = 99999;
        sprintf( stmp, "x%d", Quantity );
        gFont.Print( surf, stmp, 80, Pitch, gPalColorCubeRGB[31][31][31] );
    } else if( Quantity > 1 ){
        if( Picked ) Quantity--;
        if( Quantity > 99999 ) Quantity = 99999;
        sprintf( stmp, "x%d", Quantity );
        gFont.Print( surf, stmp, 80, Pitch, gPalColorCubeRGB[31][31][31] );
    }

    FontSet( Current );
}

void InvBodyUpdate( int a1 ,int a2 )
{
    int FrameNo, i, ObjWidth, v19[2], Id[2], SrcPitch, pitch, v31, Height;
    ArtFrmHdr_t *v7;
    char *wiper;
    VidRect_t Alpha;
    CachePool_t *ImgObj2,*ImgObj1;
    char *pSrc, *surf;

    FrameNo = 0;
    if( TimerCurrDiff( gInvSysTimer ) < 166 ) return;
    if( ++gInvArmorThumbFrame >= 6 ) gInvArmorThumbFrame = 0;
    Id[0] = gInvBodyImgId;
    Id[1] = a1;
    if( a1 == -1 ){
        v19[0] = gInvArmorThumbFrame; // frame No
        v19[1] = 2;
    } else {
        v19[0] = 3;
        v19[1] = gInvUnk39[ gInvUnk40 ]->Orientation;
    }

    for( i = 0; i < 2; i++ ){
        if( Id[ i ] == -1 ) continue;
        v7 = ArtLoadImg( Id[ i ], &ImgObj1 );
        if( !v7 ) continue;
        if( i == 1 ) FrameNo = ArtGetFpd( v7 ) - 1;
        pSrc = ArtGetObjData( v7, FrameNo, v19[ i ] );
        ObjWidth = ArtGetObjWidth( v7, FrameNo, v19[ i ] );
        SrcPitch = ObjWidth;
        v31 = ( ObjWidth <= 60 ) ? ObjWidth : 60;
        Height = ArtGetObjHeight( v7, FrameNo, v19[ i ] );
        if( Height > 100 ) Height = 100;
        if( a2 == 3 ){
            surf = WinGetSurface( gInvUnk18 );
            pitch = WinGetWidth( gInvUnk18 );
            Alpha.tp = 25;
            Alpha.lt = ( i ) ? 560 : 15;
            Alpha.rt = Alpha.lt + 59;
            Alpha.bm = Alpha.tp + 99;
            wiper = ArtGetBitmap( ArtMakeId(6, ( gInvBarterDiscount ) ? 420 : 111, 0, 0, 0), 0, 0, &ImgObj2);
            if( wiper ) ScrCopy( wiper + Alpha.lt + Alpha.tp * (gVidMainGeo.rt - gVidMainGeo.lt + 1), 60, 100, gVidMainGeo.rt - gVidMainGeo.lt + 1, &surf[Alpha.lt + Alpha.tp * pitch], pitch);
        } else {
            surf = WinGetSurface( gInvWin );
            pitch = WinGetWidth( gInvWin );
            if( i ){
                if( a2 == 2 ){
                    Alpha.tp = 39;
                    Alpha.lt = 426;
                } else {
                    Alpha.tp = 37;
                    Alpha.lt = 297;
                }
            } else if( a2 == 2 ){
                Alpha.tp = 39;
                Alpha.lt = 48;
            } else {
                Alpha.tp = 37;
                Alpha.lt = 176;
            }
            Alpha.rt = Alpha.lt + 59;
            Alpha.bm = Alpha.tp + 99;
            wiper = ArtGetBitmap( ArtMakeId( 6, 114, 0, 0, 0 ), 0, 0, &ImgObj2 );
            if( wiper ) ScrCopy( wiper + 537 * Alpha.tp + Alpha.lt, 60, 100, 537, &surf[ Alpha.lt + Alpha.tp * pitch ], pitch );
        }
        ScrCopyAlpha( pSrc, v31, Height, SrcPitch, &surf[ pitch * ( Alpha.tp + ( 100 - Height ) / 2 ) + Alpha.lt + ( 60 - v31 ) / 2 ], pitch );
        WinAreaUpdate( gInvWin, &Alpha );
        ArtClose( ImgObj1 );
        ArtClose( ImgObj2 );                
    }
    gInvSysTimer = TimerGetSysTime();
}

int InvInit()
{
    int i, j;
    ArtFrmHdr_t *Img;

    if( InvMsgOpen() == -1 ) return -1;    
    gInvUnk02 = GameIfaceStat();
    if( gInvUnk02 ) GameIfaceEnable();
    GmouseUnk03();
    GmouseLoadCursor( 1 );
    for( i = 0; i < 5; i++ ){
        if( !(Img = ArtLoadImg( ArtMakeId(6, gInvUnk42[ i ], 0, 0, 0), &gInvMseCursor[ i ].Obj )) ) break;
        gInvMseCursor[ i ].Art = Img;
        gInvMseCursor[ i ].Data = ArtGetObjData( Img, 0, 0 );
        gInvMseCursor[ i ].Width = ArtGetObjWidth( Img, 0, 0 );
        gInvMseCursor[ i ].Height = ArtGetObjHeight( Img, 0, 0 );
        ArtGetFrameShift( Img, 0, 0, &gInvMseCursor[ i ].CenterX, &gInvMseCursor[ i ].CenterY );
    }
    if( i < 5 ){
        for( j = 0; j < i; j++ ) ArtClose( gInvMseCursor[ j ].Obj );
        if( gInvUnk02 ) GameIfaceDisable( 0 );
        MessageClose( &gInvMsg );
        return -1;
    }
    gInvArt[0].Xpos = 1;
    gInvFocusRegionId = -1;
    return 0;
}

void InvSetInfoMode( int a1 )
{
    gInvInfoMode = a1;
    if( a1 != 1 || gInvFocusRegionId == -1 ){
        MseSetStaticCursor( 
    	    gInvMseCursor[ a1 ].Data, 
    	    gInvMseCursor[ a1 ].Width, 
    	    gInvMseCursor[ a1 ].Height, 
    	    gInvMseCursor[ a1 ].Width, 
    	    gInvMseCursor[ a1 ].CenterX, 
    	    gInvMseCursor[ a1 ].CenterY, 0 
    	);
    } else {
	InvEnterRegionCb( -1, gInvFocusRegionId );
    }
}

static void InvEnterRegionCb( int nu, int RegionNb )
{
    int ymid, xmid, y, x;
    Obj_t *Item;

    ymid = xmid = 0;
    if( gInvInfoMode == 1 ){
        MseGetCursorPosition( &x, &y );
        if( InvPickItem( RegionNb, &Item, NULL, NULL ) ){
            GmouseTile( x, y, 3, gInvUnk15, gInvUnk16 );
            GmouseGetCenter( &xmid, &ymid );
            MseSetStaticCursor( gInvMseCursor[2].Data, gInvMseCursor[2].Width, gInvMseCursor[2].Height, gInvMseCursor[2].Width, xmid, ymid, 0 );
	    if( Item != gInvUnk52 ) UseLook( gInvParty[0], Item, gInvInfoCb );
        } else {
            MseSetStaticCursor(
        	gInvMseCursor[1].Data, 
        	gInvMseCursor[1].Width, 
        	gInvMseCursor[1].Height, 
        	gInvMseCursor[1].Width, 
        	gInvMseCursor[1].CenterX, 
        	gInvMseCursor[1].CenterY, 
        	0
    	    );
        }
        gInvUnk52 = Item;
    }
    gInvFocusRegionId = RegionNb;
}

static void InvLeaveRegionCb()
{
    if( gInvInfoMode == 1 )
        MseSetStaticCursor(
    	    gInvMseCursor[1].Data, 
    	    gInvMseCursor[1].Width, 
    	    gInvMseCursor[1].Height, 
    	    gInvMseCursor[1].Width, 
    	    gInvMseCursor[1].CenterX, 
    	    gInvMseCursor[1].CenterY, 
    	    0
    	);
    gInvFocusRegionId = -1;
}

void InvLight( Obj_t *item )
{
    int lr, radius;
    Obj_t *critter;
    VidRect_t Area;

    if( gObjDude != gInvSelectedDude ) return;    
    if( !item ){
        radius = 4;
        critter = gInvSelectedDude;
    } else {
	lr = item->LightRadius;
	if( lr > 4 ){
            critter = gInvSelectedDude;
            radius = lr;
	} else {
            radius = 4;
            critter = gInvSelectedDude;
	}
    }
    ObjSetLight( critter, radius, 0x10000, &Area );
    TileUpdateArea( &Area, gMapCurrentLvl );    
}

void InvEquip( int sel, int SlotOffset )
{
    int Idx, avail, tmp, Height, Width;
    Obj_t **PickedPtr, *PickedObj, *item;
    VidRect_t SlotArea;
    ObjStack_t *stack;
    CachePool_t *ImgObj;
    ArtFrmHdr_t *Img;
    char *Surface, *bmp;

    PickedPtr = NULL;
    item = NULL;
    Idx = -1;
    if( !(avail = InvPickItem( sel, &PickedObj, NULL, &PickedPtr )) ) return;
    switch( sel ){
	case 1006: // rhand
    	    SlotArea.tp = 286;
    	    SlotArea.lt = 245;
    	    if( gInvSelectedDude == gObjDude && IfaceGetSelectedHand() ) item = PickedObj;
	    break;
	case 1007: // lhand
    	    SlotArea.tp = 286;
    	    SlotArea.lt = 154;
    	    if( gInvSelectedDude == gObjDude && !IfaceGetSelectedHand() ) item = PickedObj;
    	    break;
	case 1008: // armor
    	    SlotArea.tp = 183;
    	    SlotArea.lt = 154;
    	    break;
        default:
	    Idx = sel - 1000;
	    SlotArea.lt = 44;
	    SlotArea.tp = 48 * (sel - 1000) + 35;    
	    break;
    }

    // clear area under cursor    
    if( ( Idx == -1 ) || gInvBackPack->Box.Box[ Idx + SlotOffset ].Quantity <= 1 ){ // clear hand/armor slot
        Surface = WinGetSurface( gInvWin );
        if( gInvRHandObj == gInvLHandObj && PickedObj == gInvLHandObj ){ // two-hand item            
            if( (bmp = ArtGetBitmap( ArtMakeId( 6, 48, 0, 0, 0 ), 0, 0, &ImgObj )) ){
                ScrCopy( bmp + 142868, 180, 61, 499, Surface + 142868, 499 );
                ArtClose( ImgObj );
            }
            SlotArea.tp = 286;
            SlotArea.rt = 333;
            SlotArea.bm = 346;
            SlotArea.lt = 154;
        } else {
            if( Idx == -1 ){
                Height = 61;
                Width = 90;
            } else {
                Height = 48;
                Width = 64;
            }
            if( (bmp = ArtGetBitmap( ArtMakeId( 6, 48, 0, 0, 0 ), 0, 0, &ImgObj )) ){
                ScrCopy( bmp + 499 * SlotArea.tp + SlotArea.lt, Width, Height, 499, &Surface[ 499 * SlotArea.tp + SlotArea.lt ], 499 );
                ArtClose( ImgObj );
            }
            SlotArea.rt = Width + SlotArea.lt - 1;
            SlotArea.bm = Height + SlotArea.tp - 1;
        }
        WinAreaUpdate( gInvWin, &SlotArea );
    } else { // clear/update inventory list
        InvBpUpdate( SlotOffset, Idx, 0 ); // update quantity
    }

    // pickup item
    if( (Img = ArtLoadImg( ItemGetArtId( PickedObj ), &ImgObj )) ){
        Width = ArtGetObjWidth( Img, 0, 0 );
        Height = ArtGetObjHeight( Img, 0, 0 );
        MseSetStaticCursor( ArtGetObjData( Img, 0, 0 ), Width, Height, Width, Width / 2, Height / 2, 0 );
        GSoundPlay( "ipickup1" );
    }
    if( item ) InvLight( NULL );

    // event loop until mouse key release
    do{
        InpUpdate();
        InvBodyUpdate( -1, 0 );
    }while( (MseGetButtons() & 4) );

    // put down -> free mouse image of item
    if( Img ){
        ArtClose( ImgObj );
        GSoundPlay( "iputdown" );
    }

    // put down into inventory list
    if( MseCursorCenterInArea( 124, 35, 188, 48 * gInvUnk08 + 35 ) ){
        MseGetCursorPosition( &Width, &Height );
        tmp = (Height - 39) / 48 + SlotOffset; // insert index
        if( tmp < gInvBackPack->Box.Cnt ){ // on occupied position
            stack = &gInvBackPack->Box.Box[ tmp ];
            if( stack->obj != PickedObj ){ // different objects (on position vs added)
                if( ItemGetObjType( stack->obj ) == PR_ITEM_CONTAINER ){
                    if( !InvPickUpBoxQuantity( gInvBackPack->Box.Box[ tmp ].obj, PickedObj, Idx, PickedPtr, avail ) ) Idx = 0;
                } else {
            	    if( !InvLoadAmmo( gInvBackPack->Box.Box[ tmp ].obj, PickedObj, PickedPtr, avail, sel ) ) Idx = 0;
                }
            }
        }
        if( Idx == -1 ){ // from hand/armor on free position
            *PickedPtr = 0;
            if( ItemAdd( gInvSelectedDude, PickedObj, 1 ) ){
                *PickedPtr = PickedObj;
            } else if( PickedPtr == &gInvArmorObj ){
                InvUpdateStatistics( gInvParty[0], PickedObj, 0 );
            } else if( gInvRHandObj == gInvLHandObj ){
                gInvLHandObj = NULL;
                gInvRHandObj = NULL;
            }
        }
    } else if( MseCursorCenterInArea( 234, 286, 324, 347 ) ){ // Left hand slot
        if( !gInvLHandObj || ( ItemGetObjType( gInvLHandObj ) != PR_ITEM_CONTAINER ) ) goto xx1;
        if( gInvLHandObj != PickedObj ){
    	    InvPickUpBoxQuantity( gInvLHandObj, PickedObj, Idx, PickedPtr, avail );
        } else {
xx1:
    	    if( !gInvLHandObj || InvLoadAmmo( gInvLHandObj, PickedObj, PickedPtr, avail, sel ) ) InvWearItem( PickedObj, &gInvLHandObj, PickedPtr, Idx );
        }
    } else if( MseCursorCenterInArea( 325, 286, 415, 347 ) ){ // Right hand slot
    	if( !gInvRHandObj || ( ItemGetObjType( gInvRHandObj ) != PR_ITEM_CONTAINER ) ) goto xx2;
    	if( gInvRHandObj != PickedObj ){
    	    InvPickUpBoxQuantity( gInvRHandObj, PickedObj, Idx, PickedPtr, avail );
    	} else {
xx2:
    	    if( !gInvRHandObj || InvLoadAmmo( gInvRHandObj, PickedObj, PickedPtr, avail, sel ) ) InvWearItem( PickedObj, &gInvRHandObj, PickedPtr, Idx );
    	}
    } else if( MseCursorCenterInArea( 234, 183, 324, 244 ) ){ // Armor slot
        if( ItemGetObjType( PickedObj ) == PR_ITEM_ARMOR ){
            tmp = 0;
            if( Idx != -1 ) ItemUseItem( gInvSelectedDude, PickedObj, 1 );
            if( gInvArmorObj ){
                if( PickedPtr ){
                    *PickedPtr = gInvArmorObj;
                } else {
                    gInvArmorObj = 0;
                    tmp = ItemAdd( gInvSelectedDude, gInvArmorObj, 1 );
                }
            } else if( PickedPtr ){
                *PickedPtr = gInvArmorObj;
            }
            if( tmp ){
                if( Idx != -1 ) ItemAdd( gInvSelectedDude, PickedObj, 1 );
            } else {
                InvUpdateStatistics( gInvParty[0], gInvArmorObj, PickedObj );
                gInvArmorObj = PickedObj;
            }
        }
    } else if( MseCursorCenterInArea( 256, 37, 316, 137 ) && gInvUnk05 ){ // Body slot
        InvPickUpBoxQuantity( gInvParty[ gInvUnk05 - 1 ], PickedObj, Idx, PickedPtr, avail );
    }

    InvSetBodyImg();
    InvStatsUpdate();
    InvBpUpdate( SlotOffset, -1, 0 ); //
    InvSetInfoMode( 0 );
    if( gInvSelectedDude == gObjDude ){

        if( IfaceGetSelectedHand() )
            item = InvGetRHandObj( gInvSelectedDude );
        else
            item = InvGetLHandObj( gInvSelectedDude );
        if( item ) InvLight( item );
    }
}

void InvWearItem( Obj_t *PickedPtr, Obj_t **Slot, Obj_t **ptr, int Pos )
{
    Obj_t *tmp;

    if( *Slot == 0 ){
	if( ptr != 0 ){
	    if( ptr == &gInvArmorObj) InvUpdateStatistics( gInvParty[0], gInvArmorObj, NULL );
	    *ptr = NULL;
	}
    } else {
	if( ( ItemGetObjType( *Slot ) == PR_ITEM_WEAPON) && (ItemGetObjType( PickedPtr ) == PR_ITEM_AMMO ) ) return;                
	if( ptr != 0 ){
	    if( ( ptr != &gInvArmorObj) || ( ItemGetObjType( *Slot ) == PR_ITEM_ARMOR ) ){
		if( ptr == &gInvArmorObj) InvUpdateStatistics( gInvParty[0], gInvArmorObj, *Slot );
		*ptr = *Slot;
		*Slot = PickedPtr;
		if( Pos != -1 ) ItemUseItem( gInvSelectedDude, PickedPtr, 1 );
		return;
	    }
	}
	if( Pos != -1 ) ItemUseItem( gInvSelectedDude, PickedPtr, 1 );
	tmp = *Slot;
	*Slot = NULL;
	if( ItemAdd( gInvSelectedDude, tmp, 1 ) == 0 ){
	    Pos = -1;
	    if( ptr != NULL ){
		if( ptr == &gInvArmorObj ) InvUpdateStatistics( gInvParty[0], gInvArmorObj, NULL );
		*ptr = NULL;
	    }
	} else {
	    *Slot = tmp;
	    ItemAdd( gInvSelectedDude, PickedPtr, 1 );
	    return;
	}
    }
    *Slot = PickedPtr;
    if( Pos != -1 ) ItemUseItem( gInvSelectedDude, PickedPtr, 1 );
}

void InvUpdateStatistics( Obj_t *critter, Obj_t *NegativeItem, Obj_t *PositiveItem )
{
    int i;

    FeatSetBoost( critter, FEAT_AC, ItemGetAC( PositiveItem ) + FeatGetBoost( critter, FEAT_AC )- ItemGetAC( NegativeItem ) );
    for( i = 0; i < 7; i++){
        FeatSetBoost( critter, 24 + i, ItemGetDmgRes( PositiveItem, i ) + FeatGetBoost( critter, 24 + i ) - ItemGetDmgRes( NegativeItem, i ) ); // damage resistance *
        FeatSetBoost( critter, 17 + i, ItemGetDmgThr( PositiveItem, i ) + FeatGetBoost( critter, 17 + i ) - ItemGetDmgThr( NegativeItem, i ) ); // damage threshold *
    }

    if( PartyMembRdy( critter ) ){
        if( NegativeItem ){
            PerkRemoveEffect( critter, Item79( NegativeItem ) );
        }
        if( PositiveItem ){
            PerkAddEffect( critter, Item79( PositiveItem ) );
        }
    }
}

void InvSetBodyImg()
{
    int ArtId;
    Proto_t *proto;

    ArtId = gArtPrep[0];
    if( (gInvSelectedDude->ImgId & 0xF000000) >> 24 != TYPE_CRIT ){    
	gInvBodyImgId = gInvSelectedDude->ImgId;
	return;
    }
    if( ProtoGetObj( gInvSelectedPid, &proto ) != -1 ) ArtId = proto->ImgId & 0xFFF;
    if( gInvArmorObj ){
        ProtoGetObj( gInvArmorObj->Pid, &proto );
        ArtId = ( FeatGetVal(gInvSelectedDude, 34) == 1 ) ? proto->Critt.BaseStat[ 17 ] : proto->Critt.BaseStat[ 16 ];
        if( ArtId == -1 ) ArtId = gArtPrep[ 0 ];
    }
    if( IfaceGetSelectedHand() ){
        if( gInvRHandObj ){
    	    ProtoGetObj( gInvRHandObj->Pid, &proto );
    	    if( proto->Critt.Type == PR_ITEM_WEAPON ){
		gInvBodyImgId = ArtMakeId( 1, ArtId, 0, proto->Critt.BaseStat[0], 0 );
		return;
	    }
	}
    } else if( gInvLHandObj ){
    	ProtoGetObj( gInvLHandObj->Pid, &proto );
    	if( proto->Critt.Type == PR_ITEM_WEAPON ){
	    gInvBodyImgId = ArtMakeId( 1, ArtId, 0, proto->Critt.BaseStat[0], 0 );
	    return;
	}
    }                
    gInvBodyImgId = ArtMakeId( 1, ArtId, 0, 0, 0 );
}

int InvMenuBackPack( Obj_t *a1 )
{
    int WinId;
    int sel;
    int v13;
    int i;

    if( InvInit() == -1 ) return -1;
    WinId = InvMenuCreate( 1 );
    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 1 );
    InvSetInfoMode( 0 );
    while( !gMenuEscape ){
        InvBodyUpdate( -1, 1 );
        sel = InpUpdate();
        if( sel == KEY_ESC ) break;
        switch( sel ){
    	    case 328:
        	if( gInvUnk04[ gInvUnk05 ] > 0 ){
            	    gInvUnk04[ gInvUnk05 ]--;
            	    InvBpUpdate( gInvUnk04[ gInvUnk05 ] - 1, -1, 1 );
        	}
        	continue;            
    	    case 329:
        	gInvUnk04[ gInvUnk05 ] -= gInvUnk08;
        	if( gInvUnk04[ gInvUnk05 ] < 0 ) gInvUnk04[ gInvUnk05 ] = 0;
        	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 1 );
        	continue;            
    	    case 327:
        	gInvUnk04[ gInvUnk05 ] = 0;
        	InvBpUpdate( 0, -1, 1 );
        	continue;            
    	    case 335:
        	gInvUnk04[ gInvUnk05 ] = gInvBackPack->Box.Cnt - gInvUnk08;
        	if( gInvUnk04[ gInvUnk05 ] < 0 ) gInvUnk04[ gInvUnk05 ] = 0;
        	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 1 );
        	continue;            
    	    case 336:
        	if( gInvUnk04[ gInvUnk05 ] + gInvUnk08 < gInvBackPack->Box.Cnt ){
            	    gInvUnk04[ gInvUnk05 ] = gInvUnk04[ gInvUnk05 ] + 1;
            	    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 1 );
        	}
        	continue;            
    	    case 337:
        	gInvUnk04[ gInvUnk05 ] = gInvUnk08 + gInvUnk04[ gInvUnk05 ];
        	if( gInvUnk04[ gInvUnk05 ] + gInvUnk08 >= gInvBackPack->Box.Cnt ){
            	    gInvUnk04[ gInvUnk05 ] = gInvBackPack->Box.Cnt - gInvUnk08;
            	    if( gInvUnk04[ gInvUnk05 ] < 0 ) gInvUnk04[ gInvUnk05 ] = 0;
        	}
        	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 1 );
        	continue;
    	    case 2500:
        	InvUnk46( sel, 1 );
        	continue;
    	    default:
        	if( MseGetButtons() & 0x02 ){
        	    InvSetInfoMode( gInvInfoMode ? 0:1 );
        	    continue;
        	}        	
        	if( !(MseGetButtons() & 0x01) || sel < 1000 || sel >= (gInvUnk08 + 1000) ) continue;
        	if( gInvInfoMode == 1 ){
            	    InvActionMenu( sel, gInvInfoMode );
            	    continue;
        	}
            	v13 = gInvBackPack->Box.Cnt - (gInvUnk04[gInvUnk05] + sel - 1000 + 1);
            	if( v13 >= gInvBackPack->Box.Cnt ){
                    sel = -1;
            	} else {
                    if( IN_COMBAT ){
//                	if( gObjDude->Critter.State.CurrentAP >= 2 && ActionUnk23(gObjDude, a1, (int)gInvBackPack->Box.Box[v13].obj) != -1 ){
//                            if( gObjDude->Critter.State.CurrentAP < 2 )
//                            	gObjDude->Critter.State.CurrentAP = 0;
//                            else
//                            	gObjDude->Critter.State.CurrentAP -= 2;
//                            IfaceUnk12( gObjDude->Critter.State.CurrentAP, gCombatMovePts );
//                    	}
            	    } else {
//                    	ActionUnk23( gObjDude, a1, gInvBackPack->Box.Box[ v13 ].obj );
                    }
                    sel = KEY_ESC;
            	}
    	}
    }
    InvMenuClose( WinId );
    for( i = 0; i < 5; i++ ) ArtClose( gInvMseCursor[ i ].Obj );
    if( gInvUnk02 ) GameIfaceDisable( 0 );
    gInvArt[0].Xpos = 0;
    return InvMsgClose();
}

// get rhand obj with flag worn
Obj_t *InvGetRHandObj( Obj_t *obj )
{
    int i;

    if( gInvRHandObj && obj == gInvSelectedDude ) return gInvRHandObj;
    for( i = 0; i < obj->Container.Box.Cnt; i++ ){
        if( obj->Critter.Box.Box[ i ].obj->Flags & PRFLG_WORN_LHAND ) return obj->Critter.Box.Box[ i ].obj;
    }
    return NULL;    
}

// get lhand obj with flag worn
Obj_t *InvGetLHandObj( Obj_t *obj )
{
    int i;

    if( gInvLHandObj && obj == gInvSelectedDude ) return gInvLHandObj;
    for( i = 0; i < obj->Container.Box.Cnt; i++ ){
        if( obj->Critter.Box.Box[ i ].obj->Flags & PRFLG_WORN_RHAND ) return obj->Critter.Box.Box[ i ].obj;
    }
    return NULL;
}

// get armor with flag worn
Obj_t *InvGetArmorObj( Obj_t *obj )
{
    int i;

    if( gInvArmorObj && obj == gInvSelectedDude ) return gInvArmorObj;
    for( i = 0; i < obj->Container.Box.Cnt; i++ ){
        if( obj->Critter.Box.Box[ i ].obj->Flags & PRFLG_WORN_ARMOR ) return obj->Critter.Box.Box[ i ].obj;
    }
    return NULL;
}

int InvIsObjInInventory( Obj_t *obj, int Pid )
{
    int i;
    Obj_t *p;

    for( i = 0; i < obj->Container.Box.Cnt; i++ ){
        p = obj->Container.Box.Box[ i ].obj;
        if( Pid == p->Pid ) return 1;
        if( InvIsObjInInventory( p, Pid ) ) return 1;
    }    
    return 0;
}

Obj_t *InvGetItem( Obj_t *Obj, int Pid )
{
    int i;
    Obj_t *item;

    for( i = 0; i < Obj->Container.Box.Cnt; i++ ){
        item = Obj->Container.Box.Box[ i ].obj;
        if( Pid == item->Pid ) return item;
        if( (item = InvGetItem( item, Pid ) ) ) return item;
    }
    return NULL;    
}

int InvGetQuantityTot( Obj_t *obj, int Pid )
{
    int Quantity;
    int i;

    Quantity = 0;
    if( !obj ) return 0;
    for( i = 0; i < obj->Critter.Box.Cnt; i++ ){
        if( Pid == obj->Critter.Box.Box[ i ].obj->Pid ) Quantity += obj->Critter.Box.Box[ i ].Quantity;
        Quantity += InvGetQuantityTot( obj->Critter.Box.Box[ i ].obj, Pid );
    }
    return Quantity;
}

void InvReset( Obj_t *crit )
{
    if( crit ) crit->Container.Box.Cnt = 0;
    gInvLHandObj = NULL;
    gInvRHandObj = NULL;
    gInvArmorObj = NULL;
    InvSetBodyImg();
}

void InvStatsUpdate()
{
    char *Bitmap;
    MsgLine_t msg1, msg2;
    Obj_t *item[2];
    CachePool_t *Obj;
    char *surf, stmp[80];
    int v9,i,v16,v21,v25,type,Class,DmgBoost,Rng,v32,DmgMax,DmgMin,FontId,v69,color,ItemSlot[2],v57[7],FeatId[7];

    memcpy( FeatId, gInvUnk12, sizeof( FeatId ) );
    item[ 0 ] = gInvLHandObj;
    ItemSlot[ 0 ] = 0;

    memcpy( v57, gInvUnk13, sizeof( v57 ) );
    item[ 1 ] = gInvRHandObj;
    ItemSlot[ 1 ] = 2;

    FontId = FontGetCurrent();
    v69 = 0;
    FontSet( 101 );

    surf = WinGetSurface( gInvWin );
    Bitmap = ArtGetBitmap( ArtMakeId( 6, 48, 0, 0, 0 ), 0, 0, &Obj ); // info screen cls
    if( Bitmap ) ScrCopy( Bitmap + 22253, 152, 188, 499, surf + 22253, 499 );
    ArtClose( Obj );
    gFont.Print( surf + 22253, CritterGetName( gInvParty[0] ), 80, 499, gPalColorCubeRGB[0][31][0] );

    v9 = 998 * gFont.ChrHeight() + 22253;
    ScrLine( surf, 499, 297, 3 * gFont.ChrHeight() / 2 + 44, 440, 3 * gFont.ChrHeight() / 2 + 44, gPalColorCubeRGB[0][31][0] );

    for( i = 0; i < 7; i++, v9 += 499 * gFont.ChrHeight(), v69++ ){
        msg1.Id = v69;
        if( MessageGetMsg( &gInvMsg, &msg1 ) == 1 ) gFont.Print( surf + v9, msg1.Text, 80, 499, gPalColorCubeRGB[0][31][0] );
        sprintf( stmp, "%d", FeatGetVal( gInvParty[0], i ) );
        gFont.Print( surf + v9 + 24, stmp, 80, 499, gPalColorCubeRGB[0][31][0] );        
    }
    v16 = v9 - 499 * i * gFont.ChrHeight();
    for( i = 0; i < 7; i++, v69++, v16 += 499 * gFont.ChrHeight() ){
        msg1.Id = v69;
        if( MessageGetMsg( &gInvMsg, &msg1 ) == 1 ) gFont.Print( surf + v16 + 40, msg1.Text, 80, 499, gPalColorCubeRGB[0][31][0] );
        if( v57[ i ] == -1 ){
            sprintf( stmp, "   %d", FeatGetVal(gInvParty[0], FeatId[ i ] ) );
        } else {
            sprintf( stmp, "%d/%d%c", FeatGetVal( gInvParty[0], FeatId[ i ] ), FeatGetVal( gInvParty[0], v57[ i ] ), i ? '%' : 0 );
        }
        gFont.Print( surf + v16 + 104, stmp, 80, 499, gPalColorCubeRGB[0][31][0] );        
    }

    ScrLine( surf, 499, 297, 18 * gFont.ChrHeight() / 2 + 48, 440, 18 * gFont.ChrHeight() / 2 + 48, gPalColorCubeRGB[0][31][0] );
    ScrLine( surf, 499, 297, 26 * gFont.ChrHeight() / 2 + 48, 440, 26 * gFont.ChrHeight() / 2 + 48, gPalColorCubeRGB[0][31][0] );
    v21 = 499 * gFont.ChrHeight() + v16;
    for( i = 0; i < 2; i++ ){
        if( item[ i ] ){
            gFont.Print( surf + v21, ItemGetName( item[ i ] ), 140, 499, gPalColorCubeRGB[0][31][0] ); // item name
            v25 = 499 * gFont.ChrHeight() + v21;
            type = ItemGetObjType( item[ i ] );
            if( type == PR_ITEM_WEAPON ){
        	Rng = ItemGetRange( gInvParty[0], ItemSlot[ i ] );
                ItemGetWeaponDmg( item[ i ], &DmgMin, &DmgMax );
                Class = ItemGetClass( item[ i ], ItemSlot[ i ] );
                msg2.Id = 16;
                msg1.Id = 15;
                stmp[ 0 ] = '\0';
                DmgBoost = ( Class == 2 || Class == 1 ) ? FeatGetVal( gInvParty[0], 11 ) : 0;
                if( MessageGetMsg( &gInvMsg, &msg1 ) == 1 ){
                    if( Class == 4 || Rng > 1 ){ // ranged weapon
                        if( MessageGetMsg( &gInvMsg, &msg2 ) == 1 ) sprintf( stmp, "%s %d-%d   %s %d", msg1.Text, DmgMin, DmgMax + DmgBoost, msg2.Text, Rng );// Dmg/Rng
                    } else {
                        sprintf( stmp, "%s %d-%d", msg1.Text, DmgMin, DmgMax + DmgBoost ); // Dmg
                    }
                    gFont.Print( surf + v25, stmp, 140, 499, gPalColorCubeRGB[0][31][0] );
                }

                v25 += 499 * gFont.ChrHeight();
                if( ItemGetPackQuantity( item[ i ] ) <= 0 ){
                    v21 = 998 * gFont.ChrHeight() + v25;
                    continue;
                }
                msg1.Id = 17;
                stmp[0] = 0;
                v32 = ItemGetAmmoId( item[i] );
                if( MessageGetMsg( &gInvMsg, &msg1 ) == 1 ){
                    if( v32 == -1 ){ // no ammo
                        gFont.Print( surf + v25, stmp, 140, 499, gPalColorCubeRGB[0][31][0] );
                        v21 = 998 * gFont.ChrHeight() + v25;
                	continue;
                    }
                    if( ItemGetAmmo( item[ i ] ) ){
                        sprintf( stmp, "%s %d/%d %s", msg1.Text, ItemGetAmmo( item[ i ] ), ItemGetPackQuantity( item[ i ] ), ProtoGetObjName( v32 ) ); // Ammo ammo name
                        gFont.Print( surf + v25, stmp, 140, 499, gPalColorCubeRGB[0][31][0] );
                        v21 = 998 * gFont.ChrHeight() + v25;
                        continue;
                    }
                }
                sprintf( stmp, "%s %d/%d", msg1.Text, ItemGetAmmo( item[ i ] ), ItemGetPackQuantity( item[ i ] ) );
                gFont.Print( surf + v25, stmp, 140, 499, gPalColorCubeRGB[0][31][0] );
                v21 = 998 * gFont.ChrHeight() + v25;
                continue;
            }
            if( type == 0 ){ // not worn
                msg1.Id = 18;
                if( MessageGetMsg(&gInvMsg, &msg1) == 1 ) gFont.Print( &surf[v25], msg1.Text, 120, 499, gPalColorCubeRGB[0][31][0] );
            }
        } else { // No item
            stmp[ 0 ] = '\0';
            msg1.Id = 14;
            if( MessageGetMsg( &gInvMsg, &msg1 ) == 1 ) gFont.Print( surf + v21, msg1.Text, 120, 499, gPalColorCubeRGB[0][31][0] );
            v25 = 499 * gFont.ChrHeight() + v21;
            msg1.Id = 24;
            if( MessageGetMsg( &gInvMsg, &msg1 ) == 1 ) sprintf( stmp, "%s 1-%d", msg1.Text, FeatGetVal( gInvParty[0], 11 ) + 2 );
            gFont.Print( surf + v25, stmp, 120, 499, gPalColorCubeRGB[0][31][0] );
        }
        v21 = 1497 * gFont.ChrHeight() + v25;
    }
    msg1.Id = 20;
    if( MessageGetMsg( &gInvMsg, &msg1 ) == 1 ){
        color = gPalColorCubeRGB[0][31][0];
        if( ( gInvParty[0]->Pid >> 24 ) == 1 ){
            sprintf( stmp, "%s %d/%d", msg1.Text, ItemGetBackPackWeight( gInvParty[0] ), FeatGetVal( gInvParty[0], FEAT_CARRY ) ); // Total Wt
            if( CritterOverload( gInvParty[0] ) ) color = gPalColorCubeRGB[31][0][0];
            gFont.Print( surf + v21 + 15, stmp, 120, 499, color );
        } else {
            sprintf( stmp, "%s %d", msg1.Text, ItemGetBackPackWeight( gInvParty[0] ) );
            gFont.Print( surf + v21 + 30, stmp, 80, 499, gPalColorCubeRGB[0][31][0] );
        }
    }
    FontSet( FontId );
}

Obj_t *InvSearchObjByType( Obj_t *a1, int ItemType, int *InvPosition )
{
    int tmp = -1;

    if( !InvPosition ) InvPosition = &tmp;
    (*InvPosition)++;
    if( *InvPosition >= a1->Critter.Box.Cnt ) return NULL;
    if( ItemType == -1 ) return a1->Critter.Box.Box[ *InvPosition ].obj;
    for( ;*InvPosition < a1->Critter.Box.Cnt; (*InvPosition)++ ){
        if( ItemGetObjType( a1->Critter.Box.Box[ *InvPosition ].obj ) == ItemType ) return a1->Critter.Box.Box[ *InvPosition ].obj;
    }
    return NULL;
}

Obj_t *InvSearchObjByPid( Obj_t *Critter, int ObjPid )
{
    int i;

    if( ObjPid == Critter->TimeEv ) return Critter;
    for( i = 0; i < Critter->Critter.Box.Cnt; i++ ){
        if( ObjPid == Critter->Critter.Box.Box[ i ].obj->TimeEv ) return Critter->Critter.Box.Box[ i ].obj;
        if( ItemGetObjType( Critter->Critter.Box.Box[ i ].obj ) != PR_ITEM_CONTAINER ) continue;
        if( ( Critter = InvSearchObjByPid( Critter, ObjPid ) ) ) return Critter;
    }    
    return NULL;
}

Obj_t *InvUnk27( Obj_t *a1, int a2 )
{
    if( a2 < 0 || a2 >= a1->Critter.Box.Cnt ) return 0;
    return a1->Critter.Box.Box[ a2 ].obj;        
}

int InvUnk28( Obj_t *a1, Obj_t *a2, int a3 )
{
    return InvUnk29( a1, a2, a3, 1 );
}

int InvUnk29( Obj_t *a1, Obj_t *a2, int a3, int a4 )
{
    VidRect_t Area;
    Proto_t *proto;
    Obj_t *RHandObj;
    Obj_t *ArmorObj;
    int LtInt;
    int LtRad;
    int LightRadius;
    int LightIntensity;
    int tmp;
    int SelectedHand;

    if( a4 && !MapAmbientEnabled() ) AnimRegStart( 2 );
    if( ItemGetObjType(a2) ){
        SelectedHand = ( a1 == gObjDude ) ? IfaceGetSelectedHand() : 1;        
        if( !ArtFileExist( ArtMakeId( 1, a1->ImgId & 0xFFF, Item45( a2, 2 ), Item58( a2 ), a1->Orientation + 1 ) ) ){ 
    	    eprintf( "\ninven_wield failed!  ERROR ERROR ERROR!" ); 
    	    return -1; 
    	}
        if( a3 ){
            RHandObj = InvGetRHandObj( a1 );
            a2->Flags |= 0x02000000;
        } else {
            RHandObj = InvGetLHandObj( a1 );
            a2->Flags |= 0x01000000;
        }
        if( RHandObj ){
            RHandObj->Flags &= 0xFCFFFFFF;
            if( RHandObj->Pid == PID_LIGHTEDFLARE ){
                if( a1 == gObjDude ){
                    LtInt = 0x10000;
                    LtRad = 4;
                } else {
                    if( ProtoGetObj( a1->Pid, &proto ) == -1 ) return -1;
                    LtInt = proto->LtInt;
                    LtRad = proto->LtRad;
                }
                ObjSetLight( a1, LtRad, LtInt, &Area );
            }
        }
        if( a2->Pid == PID_LIGHTEDFLARE ){
            LightRadius = a2->LightRadius;
            if( LightRadius < a1->LightRadius ) LightRadius = a1->LightRadius;
            LightIntensity = a2->LightIntensity;
            if( LightIntensity < a1->LightIntensity ) LightIntensity = a1->LightIntensity;
            ObjSetLight( a1, LightRadius, LightIntensity, &Area );
            TileUpdateArea( &Area, gMapCurrentLvl );
        }
        tmp = ( ItemGetObjType(a2) == 3 ) ? Item58( a2 ) : 0;
        if( SelectedHand == a3 ){
            if( (a1->ImgId & 0xF000) >> 12 && a4 && !MapAmbientEnabled() ){
                AnimRegPlaySfx( a1, GSoundCharacterFileName( a1, 39, 0 ), 0 );
                AnimRegAnimation( a1, 39, 0 );
            }
            if( !a4 || MapAmbientEnabled() ){
                AnimUnk24( a1, a1->Orientation, ArtMakeId( 1, a1->ImgId & 0xFFF, 0, tmp, a1->Orientation + 1 ) );
            } else if( tmp ){
                AnimUnk63( a1, tmp, -1 );
            } else {
                AnimUnk62( a1, ArtMakeId( 1, a1->ImgId & 0xFFF, 0, 0, a1->Orientation + 1 ), -1 );
            }
        }
    } else {
        ArmorObj = InvGetArmorObj(a1);
        if( ArmorObj ) ArmorObj->Flags &= ~0x4000000;
        a2->Flags |= 0x04000000;
        tmp = ( FeatGetVal( a1, FEAT_GENDER ) == 1 ) ? Item81( a2 ) : Item80( a2 );
        if( a1 == gObjDude ){
            if( !MapAmbientEnabled() ){
                AnimUnk62( a1, ArtMakeId(1, tmp, 0, (a1->ImgId & 0xF000) >> 12, a1->Orientation + 1), 0 );
            }
        } else {
            InvUpdateStatistics( a1, ArmorObj, a2 );
        }
    }
    if( !a4 || MapAmbientEnabled() ) return 0;
    return AnimRegEnd();
}

int InvUnwield( Obj_t *a1, int a2 )
{
    return InvUnk30(a1, a2, 1);
}

int InvUnk30( Obj_t *a1, int Lhand, int a3 )
{
    Obj_t *HandObj;
    int hand;

    hand = 1;
    if( a1 == gObjDude ) hand = IfaceGetSelectedHand();
    HandObj = ( Lhand ) ? InvGetRHandObj( a1 ) : InvGetLHandObj( a1 );
    if( HandObj ) HandObj->Flags &= ~0x03000000;
    if( hand == Lhand && (a1->ImgId & 0xF000) >> 12 ){
        if( a3 && !MapAmbientEnabled() ){
            AnimRegStart( 2 );
            AnimRegPlaySfx( a1, GSoundCharacterFileName( a1, 39, 0 ), 0 );
            AnimRegAnimation( a1, 39, 0 );
            AnimUnk62( a1, ArtMakeId( 1, a1->ImgId & 0xFFF, 0, 0, a1->Orientation + 1 ), -1 );
            return AnimRegEnd();
        }
        AnimUnk24( a1, a1->Orientation, ArtMakeId( 1, a1->ImgId & 0xFFF, 0, 0, a1->Orientation + 1 ) );
    }
    return 0;
}

// returns current quantity of item in inventory
int InvPickItem( int sel, Obj_t **PickedItem, Obj_t **BoxObj, Obj_t ***PickPtrObj )
{
    int Quantity, ItemIdx;
    Obj_t **PickPtr, *Box, *picked;
    ObjStack_t *stack;

    Quantity = 0;
    switch( sel ){
        case 1006:
            PickPtr = &gInvRHandObj;
            Box = gInvParty[0];
            picked = gInvRHandObj;
            break;        
        case 1007:
            PickPtr = &gInvLHandObj;
            Box = gInvParty[0];
            picked = gInvLHandObj;
            break;
        case 1008:
            PickPtr = &gInvArmorObj;
            Box = gInvParty[0];
            picked = gInvArmorObj;
            break;
	default:
	    PickPtr = NULL;
	    Box = NULL;
	    picked = NULL;
	    if( sel < 2000 ){
    		ItemIdx = gInvUnk04[ gInvUnk05 ] + sel - 1000;
    		if( ItemIdx >= gInvBackPack->Box.Cnt ) break;
    		stack = &gInvBackPack->Box.Box[ gInvBackPack->Box.Cnt - ItemIdx - 1 ];
    		picked = stack->obj;
    		Box = gInvParty[ gInvUnk05 ];
    		Quantity = stack->Quantity;
    		break;
	    }
	    if( sel < 2300 ){
    		ItemIdx = gInvUnk63[gInvUnk40] + sel - 2000;
    		if( ItemIdx >= gInvUnk62->Box.Cnt ) break;
    		stack = &gInvUnk62->Box.Box[ gInvUnk62->Box.Cnt - ItemIdx -1 ];
    		picked = stack->obj;
    		Box = gInvUnk39[ gInvUnk40 ];
    		Quantity = stack->Quantity;
    		break;
	    }
	    if( sel >= 2400 ){
    		ItemIdx = sel - 2400 + gInvUnk77;
    		if( ItemIdx >= gInvUnk85->Box.Cnt ) break;
    		stack = &gInvUnk85->Box.Box[ gInvUnk85->Box.Cnt - ItemIdx - 1 ];
    		Box = gInvUnk65;
    		picked = stack->obj;
    		Quantity = stack->Quantity;
    		break;
	    }
	    ItemIdx = sel - 2300 + gInvUnk78;
	    if( ItemIdx < gInvUnk79->Box.Cnt ){
    		stack = gInvUnk79->Box.Box;
    		ItemIdx = gInvUnk79->Box.Cnt - ItemIdx - 1;
    	        Box = gInvUnk84;
    		picked = stack[ ItemIdx ].obj;
    		Quantity = stack[ ItemIdx ].Quantity;
	    }
	    break;
    }
    if( PickPtrObj ) *PickPtrObj = PickPtr;
    if( PickedItem ) *PickedItem = picked;
    if( BoxObj ) *BoxObj = Box;
    if( !Quantity ) return ( picked != 0 );
    return Quantity;
}

void InvDisplMsg( char *strmsg )
{
    char *s, *surf, *tmp;
    int FontId;

    FontId = FontGetCurrent();
    FontSet( 101 );
    s = NULL;
    surf = WinGetSurface( gInvWin ) + 22253;
    if( strmsg ){
        while( *strmsg ){
            if( ++gInvArt[0].Ypos > 17 ){ eprintf( "\nError: inven_display_msg: out of bound!" ); return; }
            if( gFont.LineWidth(strmsg) > 152 ){
                s = strmsg + 1;
                if( strmsg[1] ){
                    do{ if( *s == ' ' ) break; } while ( *++s );
                }
                if( !*s ){
                    gFont.Print( &surf[gInvArt[0].Ypos * 499 * gFont.ChrHeight() ], strmsg, 152, 499, gPalColorCubeRGB[0][31][0] );
                    return;
                }
                tmp = s + 1;
                while( 1 ){
                    while( *tmp && *tmp != ' ' ) tmp++;
                    if( !*tmp ){                        
                	tmp = NULL;
                	break;
                    } else {
                    	*tmp = '\0';
                    	if( gFont.LineWidth( strmsg ) >= 152 ){
                	    *tmp = ' ';
                	    tmp = NULL;
                	    break;
                    	} else {
                    	    s = tmp;
                    	    *tmp++ = ' ';
                    	}
                    }
                }
                if( *s == ' ' ) *s = '\0';
            }
            if( gFont.LineWidth( strmsg ) > 152 ){ eprintf( "\nError: inven_display_msg: word too long!" ); return; }
            gFont.Print( &surf[ gInvArt[0].Ypos * 499 * gFont.ChrHeight() ], strmsg, 152, 499, gPalColorCubeRGB[0][31][0] );
            if( s ){
                strmsg = s + 1;
                if( !*s ) *s = ' ';
                s = NULL;
            } else {
                strmsg = NULL;
            }
            if( !strmsg ) break;
        }
    }
    FontSet( FontId );
}

void InvInfo( Obj_t *Critter, Obj_t *Item )
{
    char *surf, *img;
    int h, ItemWeight;
    char stmp[ 40 ];
    MsgLine_t msg;
    CachePool_t *ImgObj;
    int FontId;

    FontId = FontGetCurrent();
    FontSet( 101 );
    surf = WinGetSurface(gInvWin);
    if( (img = ArtGetBitmap( ArtMakeId( 6, 48, 0, 0, 0 ), 0, 0, &ImgObj ) ) ) ScrCopy( img + 22253, 152, 188, 499, surf + 22253, 499 );
    ArtClose( ImgObj );
    gInvArt[ 0 ].Ypos = 0;
    InvDisplMsg( ObjGetName( Item ) );
    gInvArt[ 0 ].Ypos++;
    gFont.ChrHeight();
    h = gFont.ChrHeight();
    ScrLine( surf, 499, 297, 3 * h / 2 + 49, 440, 3 * h / 2 + 49, gPalColorCubeRGB[0][31][0] );
    UseExamine( Critter, Item, InvDisplMsg );
    if( (ItemWeight = ItemGetItemWeight( Item ) ) ){
        msg.Id = 540; // 'It weighs %d pounds.'
        if( ItemWeight == 1 ) msg.Id = 541; // 'ItWeighs %d pound.'
        if( MessageGetMsg( &gProtoMessages, &msg ) != 1 ) eprintf( "\nError: Couldn't find message!" );
        sprintf( stmp, msg.Text, ItemWeight );
        InvDisplMsg( stmp );
    }
    FontSet( FontId );
}

void InvActionMenu( int sel, int mode )
{
    int tmp, x, y,msebt,MseY,SelectedAction,CenterX,CenterY,MseX,Height,ObjType,IconsCount,*IconsTable,Available, bt;
    char *Surface, *Bitmap, *surf;
    VidRect_t area;
    Obj_t *BoxObj, *obj, **HandObj, *Item;
    CachePool_t *ImgObj;

    if( !( Available = InvPickItem( sel, &Item, &BoxObj, &HandObj ) ) ) return;

    ObjType = ItemGetObjType( Item );
    do{
        InpUpdate();
        if( !mode ) InvBodyUpdate( -1, 0 );
        msebt = MseGetButtons();
        if( msebt & 0x10 ){ // click left button
            if( mode )
                UseLook( gInvParty[ 0 ], Item, gInvInfoCb );
            else
                InvInfo( gInvParty[ 0 ], Item ); // display information about item in inventory
            WinUpdate( gInvWin );
            return;
        }
    }while( (msebt & 0x05) != 0x05 );
    // press and hold left button
    InvSetInfoMode( 4 );

    surf = WinGetSurface( gInvWin );
    MseGetCursorPosition( &MseX, &MseY );

    // select menu configuration
    if( (ObjType == PR_ITEM_WEAPON) && ItemCharged( Item ) ){ // possible unload weapon
        if( mode && ObjGetOwner( Item ) != gObjDude ){
            IconsTable = gInvMenuIconWeapon; IconsCount = 3;
        } else {
            IconsTable = gInvMenuIconWeaponOwned; IconsCount = 4;
        }
    } else if( mode ){
        if( ObjGetOwner( Item ) != gObjDude ){
            if( ObjType == PR_ITEM_CONTAINER ){
                IconsTable = gInvUnk91; IconsCount = 3;
            } else {
                IconsTable = gInvUnk92; IconsCount = 2;
            }
        } else {
    	    if( ObjType == PR_ITEM_CONTAINER ){
        	IconsTable = gInvMenuIconConsumable; IconsCount = 4;
    	    } else {
    		IconsTable = gInvUnk90; IconsCount = 3;
    	    }
        }
    } else if( ObjType == PR_ITEM_CONTAINER && HandObj ){
	IconsTable = gInvUnk90; IconsCount = 3;
    } else if( ObjOpenable( Item ) || ProtoDrugUsable( Item->Pid ) ){  // use chem or device
        IconsTable = gInvMenuIconConsumable; IconsCount = 4;
    } else {
        IconsTable = gInvUnk90; IconsCount = 3;
    }

    GmouseCursorMenuCreate( 
	MseX, MseY, IconsTable, IconsCount, 
	gInvArt[ mode ].Width + gInvArt[ mode + 1 ].Xpos, 
	gInvArt[ mode ].Height + gInvArt[ mode + 1 ].Ypos 
    );
    ArtGetCenter( gInvMseCursor[ 3 ].Art, 0, &CenterX, &CenterY );

    area.lt = MseX - gInvArt[ mode + 1 ].Xpos - gInvMseCursor[ 3 ].Width / 2 + CenterX;
    area.tp = MseY - gInvArt[ mode + 1 ].Ypos - gInvMseCursor[ 3 ].Height + CenterY + 1;
    area.rt = gInvMseCursor[ 3 ].Width  + area.lt - 1;
    area.bm = gInvMseCursor[ 3 ].Height + area.tp - 1;

    Height = gInvMseCursor[3].Height;
    if( area.tp + gInvMseCursor[3].Height > gInvArt[ mode ].Height ) Height = gInvArt[ mode ].Height - area.tp;
    bt = WinCreateButton( gInvWin, area.lt, area.tp, gInvMseCursor[3].Width, Height, -1, -1, -1, -1, gInvMseCursor[3].Data, gInvMseCursor[3].Data, 0, 0x20 );

    WinAreaUpdate( gInvWin, &area );

    SelectedAction = 0;
    tmp = MseY;

    while( (MseGetButtons() & 0x10) == 0 ){ // cursor menu selection
        InpUpdate();
        if( !mode ) InvBodyUpdate( -1, 0 );
        MseGetCursorPosition( &x, &y );
        if( y - tmp > 10 || tmp - y > 10 ){
            if( y >= tmp || SelectedAction <= 0 ){
                if( tmp < y && SelectedAction < (IconsCount - 1) ) SelectedAction++;
            } else {
                SelectedAction--;
            }
            GmouseMenuIconHL( SelectedAction );
            WinAreaUpdate( gInvWin, &area );
            tmp = y;
        }
    }
    WinImageRemove( bt );
    // restore screen under icon menu 
    if( mode == 3 ){
        Surface = WinGetSurface( gInvUnk18 );
        tmp = gInvArt[ mode ].Width * area.tp + area.lt;
        ScrCopy( &Surface[ area.lt + 80 + tmp ], gInvMseCursor[ 3 ].Width, Height, gVidMainGeo.rt - gVidMainGeo.lt + 1, &surf[ tmp ], gInvArt[ mode ].Width );
    } else {
        if( (Bitmap = ArtGetBitmap( ArtMakeId( 6, gInvArt[ mode ].ArtId, 0, 0, 0 ), 0, 0, &ImgObj )) ){
            tmp = gInvArt[ mode ].Width * area.tp + area.lt;
            ScrCopy( Bitmap + tmp, gInvMseCursor[3].Width, Height, gInvArt[ mode ].Width, &surf[ tmp ], gInvArt[ mode ].Width );
            ArtClose( ImgObj );
        }
    }
    MseSetCursorPosition( MseX, MseY ); // restore cursor position
    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, mode );

    switch( IconsTable[ SelectedAction ] ){ // selected action
        case INV_ACT_THROW: // throwout item
            if( HandObj ){
                if( HandObj == &gInvArmorObj ) InvUpdateStatistics( gInvParty[0], Item, 0 );
                ItemAdd( BoxObj, Item, 1 );
                Available = 1;
                *HandObj = NULL;
            }
            if( Item->Pid == PID_MONEY ){
                Available = ( Available <= 1 ) ? 1 : InvPopUpDlg( 4, Item, Available );
                if( Available > 0 ){
                    if( Available == 1 ){
                        ItemSetMoney( Item, 1 );
                	UseDropObj( BoxObj, Item);
                    } else {
                        if( !ItemUseItem( BoxObj, Item, Available - 1 ) ){
                    	    if( !InvPickItem( sel, &obj, &BoxObj, &HandObj ) ){
                        	ItemAdd( BoxObj, Item, Available - 1 );
                    	    } else {
                    		ItemSetMoney( obj, Available );
                	    	UseDropObj( BoxObj, obj );
                    	    }
                        }
                    }
            	    break;
                }
            } else {
                if( Item->Pid == PID_DYNAMITE || Item->Pid == PID_PLASTICEXPLOSIVES ){ gInvUnk83 = 1; UseDropObj( BoxObj, Item ); break; }
                if( Available <= 1 ){ UseDropObj( BoxObj, Item ); break; }
                Available = InvPopUpDlg( 4, Item, Available );
                for( tmp = 0; tmp < Available; tmp++ ){
                    if( InvPickItem( sel, &Item, &BoxObj, &HandObj ) ) UseDropObj( BoxObj, Item );
                }
            }
            break;
        case INV_ACT_INSPECT:
            if( mode )
                UseExamine( gInvParty[0], Item, gInvInfoCb );
            else
                InvInfo( gInvParty[0], Item );
            break;
        case INV_ACT_USE:
            switch( ObjType ){
                case PR_ITEM_CONTAINER:
                    InvUnk45( sel, mode );
                    break;
                case PR_ITEM_DRUG:
                    if( ItemUnk07( gInvParty[0], Item ) == 1 ){
                        if( HandObj )
                            *HandObj = NULL;
                        else
                            ItemUseItem( BoxObj, Item, 1 );
                        ObjUnk14( Item, gObjDude->GridId, gObjDude->Elevation, 0 );
                        UseUnk06( Item );
                    }
                    IfaceRenderHP( 1 );
                    break;
                case PR_ITEM_WEAPON: case PR_ITEM_MISC:
                    if( !HandObj ) ItemUseItem( BoxObj, Item, 1 );
                    if( ObjOpenable( Item ) )
                        tmp = UseUnk13( gInvParty[0], Item );
                    else
                        tmp = UseUseHealSkill( gInvParty[0], gInvParty[0], Item );
                    if( tmp == 1 ){
                        if( HandObj ) *HandObj = NULL;
                        ObjUnk14( Item, gObjDude->GridId, gObjDude->Elevation, 0 );
                        UseUnk06( Item );
                    } else if( !HandObj ){
            		ItemAdd( BoxObj, Item, 1 );
        		break;
                    }
                    break;
            }
            break;
        case INV_ACT_UNLOAD:
            if( !HandObj ) ItemUseItem( BoxObj, Item, 1 );
            while( 1 ){
                if( !(obj = ItemUnload( Item )) ) break;
                ObjLightItem( obj, NULL );
                ItemAdd( BoxObj, obj, 1 );
            }
            if( !HandObj ) ItemAdd( BoxObj, Item, 1 );
            break;
    }

    InvSetInfoMode( 1 );
    if( !mode && IconsTable[ SelectedAction ] != 3 ) InvStatsUpdate();
    if( mode == 2 || mode == 3 ) InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, mode );
    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, mode );
    if( mode == 3 ) InvBarterStack( gInvUnk18, gInvUnk84, gInvUnk65, -1 );
    InvSetBodyImg();
    return;
}

int InvMenuSteal( Obj_t *Critter, Obj_t *Obj2 )
{
    MsgLine_t msg;
    CachePool_t *ImgObj2[4], *ImgObj1;
    ScptCache_t *pScript;
    Scpt_t *v68;
    ArtFrmHdr_t *img;
    Obj_t *LHandObj,*RHandObj,*ArmorObj,*v38,*v44,*NewItem,**ObjTable,*item,*v70,*dude2;
    char *bmp1,*bmp2,*bmp3,*bmp4,stmp[200];
    int v3,v5,num,ii,_i,bt,v25,v27,sel,BackPackWeight,v35,v37,v39,v40,v41,v43,v45,i,j,atmp[4],Id,v64,k,v72,v73,v76,v78;

    Id = -1;
    item = v70 = dude2 = 0;
    NewItem = 0;
    v76 = v78 = v73 = 0;
    v3 = 10;
    atmp[ 0 ] = gInvUnk100[ 0 ];
    atmp[ 1 ] = gInvUnk100[ 1 ];
    atmp[ 2 ] = gInvUnk100[ 2 ];
    atmp[ 3 ] = gInvUnk100[ 3 ];

    if( Critter != gInvSelectedDude ) return 0;
    if( (OBJTYPE( Obj2->ImgId ) == TYPE_CRIT) && CritterGetInjure( Obj2->Pid, 0x20 ) ){
        msg.Id = 50; // 'You can't find anything to take from that.'
        if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
        return 0;
    }

    if( OBJTYPE( Obj2->ImgId ) == 0 ){
	if( ItemGetObjType( Obj2 ) == 1 ){
	    if( Obj2->FrameNo == 0 ){
		if( (img = ArtLoadImg( Obj2->ImgId, &ImgObj1 )) ){
		    v5 = ArtGetFpd( img );
	    	    ArtClose( ImgObj1 );
		    if( v5 > 1 ) return 0; 
		}
	    }   
	}
    }
    if( gSkillUnk80 && UseGetScriptId( Obj2, &Id ) != -1 ){
	ScptSetup( Id, Critter, 0 );
	ScptRun( Id, SCPT_AEV_PICKUP_P_PROC );
	if( ScptPtr(Id, (Scpt_t **)&pScript) == -1 || pScript->Script[0].OverrideFlag ) return 0;
    }
    if( InvInit() == -1 ) return 0;
    gInvUnk62 = &Obj2->Container;
    gInvUnk40 = 0;
    gInvUnk63[ 0 ] = 0;
    gInvUnk39[ 0 ] = Obj2;
    if( ObjCreate( &NewItem, 0, 467) == -1 ) return 0;
    Item16( Obj2, NewItem );
    if( gSkillUnk80 ){
        LHandObj = InvGetLHandObj( Obj2 );
        item = LHandObj;
        if( LHandObj ) ItemUseItem( Obj2, LHandObj, 1 );
        RHandObj = InvGetRHandObj( Obj2 );
        v70 = RHandObj;
	if( RHandObj ) ItemUseItem( Obj2, RHandObj, 1 );
        ArmorObj = (Obj_t *)InvGetArmorObj( Obj2 );
        dude2 = ArmorObj;
        if( ArmorObj ) ItemUseItem( Obj2, ArmorObj, 1 );
    }
    v64 = InvMenuCreate( 2 );
    if( !gSkillUnk80 && OBJTYPE( Obj2->ImgId ) == TYPE_CRIT ){
	num = ObjGetObjList( Obj2->GridId, Obj2->Elevation, 1, &ObjTable );
	ii = k = 0;
	_i = num;
    	for( ii = 0; ii < num; ii++ ){
    	    if( ( ObjTable[ ii ]->Critter.State.CombatResult & (CRITT_EYE_DMG | 0x01 ) ) ){
        	k++;
    	    } else {
            	ObjTable[ --ii ] = ObjTable[ --_i ];
            	ObjTable[ _i ] = ObjTable[ ii ];
            	num--;
    	    }
    	}

	v73 = k;
	if( k == 1 ){
    	    ObjCritterListDestroy( ObjTable );
	    v73 = 0;
	}
	if( v73 > 1 ){
    	    memset( ImgObj2, -1, 4 * sizeof(void *) );                            
    	    if( (bmp1 = ArtGetBitmap( ArtMakeId( 6, atmp[0], 0, 0, 0 ), 0, 0, ImgObj2 )) ){                                
        	if( (bmp2 = ArtGetBitmap( ArtMakeId(6, atmp[1], 0, 0, 0), 0, 0, &ImgObj2[1])) ){
            	    bt = WinCreateButton( gInvWin, 436, 162, 20, 18, -1, -1, 329, -1, bmp1, bmp2, 0, 0 );
            	    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
            	}
    	    }
    	    if( (bmp3 = ArtGetBitmap( ArtMakeId(6, atmp[2], 0, 0, 0), 0, 0, &ImgObj2[2])) ){
        	if( (bmp4 = ArtGetBitmap( ArtMakeId(6, atmp[3], 0, 0, 0), 0, 0, &ImgObj2[3] ) ) ){
            	    bt = WinCreateButton( gInvWin, 456, 162, 20, 18, -1, -1, 337, -1, bmp3, bmp4, 0, 0 );
            	    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
            	}
    	    }    	    
            for( v25 = k = 0; v25 < v73; v25++ ){
            	if( Obj2 == ObjTable[ v25 ]){
            	    k = v25;
            	    break;
            	}            	    
            }    	    
	}
    }

    InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 2 );
    v27 = 2;
    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 2 );
    InvBodyUpdate( Obj2->ImgId, 2 );
    InvSetInfoMode( 0 );
    v72 = v73 - 1;
    do{
        if( gMenuEscape || v76 ) break;
        sel = InpUpdate();
        if( gMenuEscape ) break;
        switch( sel ){
    	    case 17: case 24: case 324: 
    		SysQuitDlg();
    		continue;
    	    case 'A':
        	if( gSkillUnk80 ) continue;
            	v35 = FeatGetVal( Critter, 12 );
            	v35 -= ItemGetBackPackWeight( Critter );
            	BackPackWeight = ItemGetBackPackWeight( Obj2 );
        	if( BackPackWeight <= v35 ){
            	    ItemMoveObjInvToObj( Obj2, Critter );
            	    InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 2 );
            	    v27 = 2;
            	    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 2 );
        	} else {
            	    v27 = 31;
            	    msg.Id = 31;// 'Sorry, you cannot carry that much.'
            	    if( MessageGetMsg(&gInvMsg, &msg) == 1 ) {
                	v27 = 0;
                	DlgBox( msg.Text, 0, 0, 169, 117, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 0 );
        	    }
        	}            	    
        	continue;
    	    case 328:
        	v27 = gInvUnk04[ gInvUnk05 ];
        	if( gInvUnk04[ gInvUnk05 ] <= 0 ) continue;
            	gInvUnk04[ gInvUnk05 ]--;
            	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 2 );
            	v27 = 2;
        	continue;
    	    case 337:
    	    case 329:
    		if( !v73 )continue;
            	if( sel == 329 ){
            	    k = ( k ) ? (k - 1) : v72;
            	} else if( k == v72 ) {
            	    k = 0;
            	} else {
            	    k++;
            	}
            	Obj2 = ObjTable[ k ];
            	gInvUnk62 = &Obj2->Container;
            	gInvUnk39[ 0 ] = Obj2;
            	gInvUnk40 = 0;
            	gInvUnk63[ 0 ] = 0;
        	InvItemList( 0, -1, &Obj2->Container.Box, 2 );
            	v27 = 2;
            	InvBpUpdate( gInvUnk04[gInvUnk05], -1, 2 );
            	InvBodyUpdate( Obj2->ImgId, 2 );
        	continue;        
    	    case 336:
        	v27 = gInvUnk04[gInvUnk05] + gInvUnk08;
        	if( v27 >= gInvBackPack->Box.Cnt ) continue;
            	v27 = 2;
            	gInvUnk04[ gInvUnk05 ]++;
            	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 2 );
        	continue;
    	    case 397:
        	if( gInvUnk63[gInvUnk40] <= 0 ) continue;
            	gInvUnk63[ gInvUnk40 ]--;
            	InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 2 );
            	WinUpdate( gInvWin );
        	continue;
    	    case 401:
        	if( gInvUnk08 + gInvUnk63[gInvUnk40] >= gInvUnk62->Box.Cnt ) continue;
            	gInvUnk63[ gInvUnk40 ]++;
            	InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 2 );
            	WinUpdate( gInvWin );
        	continue;
    	    case 2500 ... 2501:
        	InvUnk46( sel, 2 );
        	continue;
        }
        if( ( MseGetButtons() & 0x02 ) && !gSkillUnk80 ){
            InvSetInfoMode( gInvInfoMode ? 0:1 );
            continue;
        }
        if( (MseGetButtons() & 0x01) ){
            if( sel < 1000 || sel >= gInvUnk08 + 1000 ){
                if( sel < 2000 || sel >= gInvUnk08 + 2000 ) continue;
                    if( gInvInfoMode != 1 ){
                        v43 = sel - 2000;
                        if( v43 + gInvUnk63[gInvUnk40] < gInvUnk62->Box.Cnt ){
                            v44 = gInvParty[gInvUnk05];
                            gSkillUnk60++;
                            gSkillUnk61 += Item24( v44 );
                            v45 = InvUnk38( gInvUnk62->Box.Box[gInvUnk62->Box.Cnt - (v43 + gInvUnk63[gInvUnk40] + 1)].obj, v43, gInvUnk39[gInvUnk40], 0 );
                            if( v45 == 1 ){
                                v76 = 1;
                            } else if ( v45 == 2 ) {
                                v78 += v3;
                                v3 += 10;
                            }
                            InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 2 );
                            v27 = 2;
                            InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 2 );
                        }
                        sel = -1;
                        continue;
                    }
                } else if( gInvInfoMode != 1 ){
                    v37 = sel - 1000;
                    v27 = v37 + gInvUnk04[gInvUnk05];
                    if( v27 < gInvBackPack->Box.Cnt ){
                        v38 = gInvParty[ gInvUnk05 ];
                        gSkillUnk60++;
                        v39 = Item24( v38 );
                        v40 = v37 + gInvUnk04[ gInvUnk05 ];
                        gSkillUnk61 += v39;
                        v41 = InvUnk38( gInvBackPack->Box.Box[ gInvBackPack->Box.Cnt - (v40 + 1) ].obj, v37, gInvUnk39[ gInvUnk40 ], 1 );
                        if( v41 == 1 ){
                            v76 = 1;
                        } else if ( v41 == 2 ){
                            v78 += v3;
                            v3 += 10;
                        }
                        InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 2 );
                        v27 = 2;
                        InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 2 );
                    }
                    sel = -1;
                    continue;
                }
                InvActionMenu(sel, 2);
            }
        } while( sel != KEY_ESC );
    	if( v73 ){
    	    ObjCritterListDestroy( ObjTable );
    	    for( i = 0; i != 4; i++ ) ArtClose( ImgObj2[ i ] );            
    	}
    	if( gSkillUnk80 ){
    	    if( item  ){ v27 = 1; item->Flags  |= 0x1000000; ItemAdd( Obj2, item, 1  ); }
    	    if( v70   ){ v27 = 1; v70->Flags   |= 0x2000000; ItemAdd( Obj2, v70, 1   ); }
    	    if( dude2 ){ v27 = 1; dude2->Flags |= 0x4000000; ItemAdd( Obj2, dude2, 1 ); }
    	}
    	ItemMoveObjInvToObj( NewItem, Obj2 );
    	ObjDestroy( NewItem, 0 );
    	if( gSkillUnk80 && !v76 ){
    	    if( v78 > 0 && !PartyMembRdy(Obj2) ){
                if( 300 - SkillGetTotal( Critter, 10 ) < v78 ) v78 = 300 - SkillGetTotal( Critter, 10 );
                eprintf( "\n[[[%d]]]", 300 - SkillGetTotal( Critter, 10 ) );
                msg.Id = 29;// 'You gain %d experience points for succesfully using Steal skill.'
                if( MessageGetMsg( &gInvMsg, &msg ) == 1 ){
            	    sprintf( stmp, msg.Text, v78 );
            	    IfcMsgOut( stmp );
                }
                FeatLvlUp( v78 );
    	    }            
    	}
    	InvMenuClose( v64 );
    	for( j = 0; j != 5; j++ ) ArtClose( gInvMseCursor[ j ].Obj );        
    	if( gInvUnk02 ) GameIfaceDisable( 0 );
    	InvMsgClose();
    	gInvArt[0].Xpos = 0;
        if( gSkillUnk80 && v76 && gSkillUnk60 > 0 && UseGetScriptId( Obj2, &Id ) != -1 ){
    	    ScptSetup( Id, Critter, 0 );
    	    ScptRun( Id, SCPT_AEV_PICKUP_P_PROC );
    	    ScptPtr( Id, &v68 );
	}
    	return 0;
}

void InvStealAttempt( Obj_t *Thief, Obj_t *Npc )
{
    if( Thief == Npc ) return;
    gSkillUnk80 = OBJTYPE( Thief->Pid ) == TYPE_CRIT && CritterCanTalk( Npc );
    gSkillUnk60 = 0;
    gSkillUnk61 = 0;
    InvMenuSteal( Thief, Npc );
    gSkillUnk80 = 0;
    gSkillUnk60 = 0;
    gSkillUnk61 = 0;
}

int InvUnk38( Obj_t *a1, int a2, Obj_t *a3, int a4 )
{
    VidRect_t area;
    MsgLine_t msg;
    CachePool_t *Obj;
    ArtFrmHdr_t *Img;
    char *Surface, *bmp;
    int Quantity, n, ObjWidth, ObjHeight, flg, err;

    err = 0;
    flg = 1;
    if( a4 ){
        area.tp = 48 * a2 + 37;
        area.lt = 176;
        Quantity = gInvBackPack->Box.Box[ gInvBackPack->Box.Cnt - (a2 + gInvUnk04[ gInvUnk05 ] + 1 ) ].Quantity;
        if( Quantity > 1 ){
            InvBpUpdate( gInvUnk04[ gInvUnk05 ], a2, 2 );
            flg = 0;
        }
    } else {
        area.lt = 297;
        area.tp = 48 * a2 + 37;
        Quantity = gInvUnk62->Box.Box[gInvUnk62->Box.Cnt - ( a2 + gInvUnk63[ gInvUnk40 ] + 1 ) ].Quantity;
        if( Quantity > 1 ){
            InvItemList( gInvUnk63[ gInvUnk40 ], a2, &gInvUnk62->Box, 2 );
            WinUpdate( gInvWin );
            flg = 0;
        }
    }
    if( flg ){
        Surface = WinGetSurface(gInvWin);
        bmp = ArtGetBitmap( ArtMakeId( 6, 114, 0, 0, 0 ), 0, 0, &Obj );
        if( bmp ){
            ScrCopy( &bmp[ 537 * area.tp + area.lt ], 64, 48, 537, &Surface[ 537 * area.tp + area.lt ], 537 );
            ArtClose( Obj );
        }
        area.rt = 64 + area.lt - 1;
        area.bm = 48 + area.tp - 1;
        WinAreaUpdate( gInvWin, &area );
    }    
    if( (Img = ArtLoadImg( ItemGetArtId( a1 ), &Obj ) ) ){
        ObjWidth = ArtGetObjWidth( Img, 0, 0 );
        ObjHeight = ArtGetObjHeight( Img, 0, 0 );
        MseSetStaticCursor( ArtGetObjData( Img, 0, 0 ), ObjWidth, ObjHeight, ObjWidth, ObjWidth / 2, ObjHeight / 2, 0 );
        GSoundPlay( "ipickup1" );
    }
    do
        InpUpdate();
    while( MseGetButtons() & 0x04 );
    if( Img ){
        ArtClose( Obj );
        GSoundPlay( "iputdown" );
    }
    if( a4 ){
        if( MseCursorCenterInArea( 377, 37, 441, 48 * gInvUnk08 + 37 ) ){
    	    n = Quantity <= 1 ? 1 : InvPopUpDlg( 4, a1, Quantity );
    	    if( n != -1 ){
    		if( gSkillUnk80 ){
        	    if( !SkillUseSteal( gInvSelectedDude, a3, a1, 1 ) ) err = 1;
    		}
    		if( err != 1 ){
    		    if( Item13( gInvSelectedDude, a3, a1, n ) != -1 ){ InvSetInfoMode( 0 ); return 2; }
    		    msg.Id = 26; // 'There is no space left for that item.'
    		    if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
    		}
    	    }
        }
    } else {
	if( MseCursorCenterInArea( 256, 37, 320, 48 * gInvUnk08 + 37 ) ){
	    n = ( Quantity <= 1 ) ? 1 : InvPopUpDlg( 4, a1, Quantity );
	    if( n != -1 ){
		if( gSkillUnk80 && !SkillUseSteal( gInvSelectedDude, a3, a1, 0 ) ) err = 1;
		if( err != 1 ){
		    if( !Item13( a3, gInvSelectedDude, a1, n ) ){
			if( a1->Flags & 0x2000000 ) a3->ImgId = ArtMakeId( OBJTYPE( a3->ImgId ), a3->ImgId & 0xFFF, (a3->ImgId & 0xFF0000u) >> 16, 0, a3->Orientation + 1);
			a1->Flags &= 0x07000000;
			InvSetInfoMode( 0 );
			return 2;
		    }
	    	    msg.Id = 25; // 'You cannot pick that up. You are at your maximum wieght capacity.'
		    if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
		}
	    }
	}
    }
    InvSetInfoMode( 0 );
    return err;    
}

int InvStackPrice( Obj_t *dude, Obj_t *Merchant )
{
    double PartyBarterLvl, MerchantBarterLvl;
    float MoneyAll,v12, v14, v15, discount;

    if( gDlgPmExchangeMode ) return ItemGetBackPackWeight( gInvUnk65 );
    discount = 0.0;
    MoneyAll = ItemGetMoneyAll( gInvUnk65 );
    v14 = ItemOffert( gInvUnk65 ) - MoneyAll;
    if( dude == gObjDude && PerkLvl( gObjDude, PERK_MASTER_TRADER ) ) discount = 25.0;
    PartyBarterLvl = PartyGetBestSkillLvl( SKILL_BARTER );
    MerchantBarterLvl = SkillGetTotal( Merchant, SKILL_BARTER );
    v12 = ((double)gInvUnk66 + 100.0 - discount) * 0.01;
    v15 = ((double)(int)MerchantBarterLvl + 160.0) / ((double)(int)PartyBarterLvl + 160.0) * (v14 * 2.0);
    if( v12 < 0.0 ) v12 = 0.0099999998;
    return lround( v15 * v12 + MoneyAll );
}

int InvBarterDo( Obj_t *Customer, Obj_t *CustomerStack, Obj_t *Merchant, Obj_t *MerchantStack )
{
    MsgLine_t msg;
    int Reject;

    // test overweight
    if( ItemGetBackPackWeight( MerchantStack ) > (FeatGetVal( Customer, FEAT_CARRY ) - ItemGetBackPackWeight( Customer )) ){
	msg.Id = 31; // 'Sorry, you cannot carry that much.'
	if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) GdialogReply( msg.Text );
	return -1;    
    }
    if( gDlgPmExchangeMode ){
	if( ItemGetBackPackWeight( CustomerStack ) > ( FeatGetVal( Merchant, FEAT_CARRY ) - ItemGetBackPackWeight( Merchant ) ) ){
    	    msg.Id = 32; // 'Sorry, that's too much to carry.'
    	    if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) GdialogReply( msg.Text );
    	    return -1;
	} 
    }
    if( gDlgPmExchangeMode == 0 ){
	Reject = 0;
	if( !CustomerStack->Critter.Box.Cnt ){
    	    Reject = 1;
	} else {
	    if( Item34( CustomerStack ) && ( CustomerStack->Pid != PID_GEIGER || ItemDeviceOff( CustomerStack ) == -1 ) ) Reject = 1;
	    if( !Reject ){
    		if( InvStackPrice( Customer, Merchant ) > ItemOffert( CustomerStack ) ) Reject = 1;
	    }
	}
	if( Reject ){
	    msg.Id = 28; // 'No, your offer is not good enough.'
	    if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) GdialogReply( msg.Text );
	    return -1;
	}
    }
    ItemMoveObjInvToObj( MerchantStack, Customer );
    ItemMoveObjInvToObj( CustomerStack, Merchant );
    return 0;
}

void InvMoveObj( Obj_t *dude, int quantity, int a3, int a4, Obj_t *a1, Obj_t *a2, int a7 )
{
    char *surf1, *surf2;
    int tmp, h;
    ArtFrmHdr_t *Img;
    VidRect_t area;
    MsgLine_t msg;
    CachePool_t *ImgObj;

    if( a7 ){
        area.tp = 48 * a3 + 34;
        area.lt = 23;
    } else {
        area.tp = 48 * a3 + 31;
        area.lt = 395;
    }
    if( quantity <= 1 ){
        surf1 = WinGetSurface( gInvWin );
        surf2 = WinGetSurface( gInvUnk18 );
        tmp = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        ScrCopy( surf2 + area.lt + 80 + area.tp * tmp, 64, 48, tmp, surf1 + 480 * area.tp + area.lt, 480 );
        area.rt = area.lt + 63;
        area.bm = area.tp + 47;
        WinAreaUpdate( gInvWin, &area );
    } else if( a7 ){
        InvBpUpdate( a4, a3, 3 );
    } else {
        InvItemList( a4, a3, &gInvUnk62->Box, 3 );
    }
    if( (Img = ArtLoadImg( ItemGetArtId( dude ), &ImgObj ) ) ){
        tmp = ArtGetObjWidth( Img, 0, 0 );
        h = ArtGetObjHeight( Img, 0, 0 );
        MseSetStaticCursor( ArtGetObjData( Img, 0, 0 ), tmp, h, tmp, tmp / 2, h / 2, 0 );
        GSoundPlay( "ipickup1" );
    }
    do
        InpUpdate();
    while( (MseGetButtons() & 0x04) );
    if( Img ){
        ArtClose( ImgObj );
        GSoundPlay( "iputdown" );
    }
    if( a7 ){
        if( MseCursorCenterInArea( 245, 310, 309, 48 * gInvUnk08 + 310 ) ){
            tmp = ( quantity <= 1 ) ? 1 : InvPopUpDlg( 4, dude, quantity );
            if( (tmp != -1) && ( Item14( gInvSelectedDude, a2, dude, tmp ) == -1 ) ){
                msg.Id = 26; // 'There is no space left for that item.'
                if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
            }
        }
    } else if( MseCursorCenterInArea( 330, 310, 394, 48 * gInvUnk08 + 310 ) ){
        tmp = (quantity <= 1) ? 1 : InvPopUpDlg( 4, dude, quantity );
        if( tmp != -1 && Item14( a1, a2, dude, tmp ) == -1 ){
            msg.Id = 25; // 'You cannot picj that up. You are at your maximum weight capacity.'
            if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
        }
    }
    InvSetInfoMode( 0 );
}

void InvUnk42( Obj_t *Item, int quantity, int a3, Obj_t *a4, Obj_t *a1, int a6 )
{
    char *Surface, *v9;
    ArtFrmHdr_t *Img;
    int w,tmp,h;
    MsgLine_t msg;
    VidRect_t area;
    CachePool_t *ImgObj;

    area.tp = 48 * a3 + 24;
    area.lt = ( a6 ) ? 169 : 254;
    if( quantity <= 1 ){
        Surface = WinGetSurface(gInvWin);
        v9 = WinGetSurface(gInvUnk18);
        tmp = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        ScrCopy( v9 + area.lt + 80 + area.tp * tmp , 64, 48, tmp, Surface + 480 * area.tp + area.lt, 480 );
        area.rt = area.lt + 63;
        area.bm = area.tp + 47;
        WinAreaUpdate( gInvWin, &area );
    } else if( a6 ){
        InvBarterStack( gInvUnk18, a1, 0, a3 );
    } else {
        InvBarterStack( gInvUnk18, 0, a1, a3 );
    }    
    if( (Img = ArtLoadImg( ItemGetArtId( Item ), &ImgObj ) ) ){
        w = ArtGetObjWidth( Img, 0, 0 );
        h = ArtGetObjHeight( Img, 0, 0 );
        MseSetStaticCursor( ArtGetObjData( Img, 0, 0 ), w, h, w, w / 2, h / 2, 0 );
        GSoundPlay( "ipickup1" );
    }
    do
        InpUpdate();
    while( (MseGetButtons() & 4) != 0 );
    if( Img ){
        ArtClose( ImgObj );
        GSoundPlay( "iputdown" );
    }
    if( a6 ){
        if( MseCursorCenterInArea( 80, 310, 144, 48 * gInvUnk08 + 310 ) ){
            tmp = quantity <= 1 ? 1 : InvPopUpDlg( 4, Item, quantity );
            if( tmp != -1 && Item14( a1, gInvSelectedDude, Item, tmp ) == -1 ){
                msg.Id = 26; // 'There is no space left for that item.'
                if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
            }
        }
    } else if( MseCursorCenterInArea( 475, 310, 539, 48 * gInvUnk08 + 310 ) ){
        tmp = quantity <= 1 ? 1 : InvPopUpDlg(4, Item, quantity);
        if( tmp != -1 && Item14(a1, a4, Item, tmp ) == -1 ){
            msg.Id = 25; // 'You cannot pick that up. You are at your maximum weight capacity.'
            if ( MessageGetMsg( &gInvMsg, &msg ) == 1 ) IfcMsgOut(msg.Text);
        }
    }
    InvSetInfoMode( 0 );
}

void InvBarterStack( int win, Obj_t *CustomerStack, Obj_t *MerchantStack, int a4 )
{
    ObjStack_t *v13, *v27;
    VidRect_t area;
    MsgLine_t msg1, msg2;
    char stmp[80], *surf1, *surf2, *v8, *Surface;
    void *v31;
    int v7,i,v15,v24,v38,DestPitch,FontId,v45;

    surf1 = WinGetSurface( gInvWin );
    FontId = FontGetCurrent();
    DestPitch = 480;
    FontSet( 101 );
    v45 = gFont.ChrHeight() + 48 * gInvUnk08;
    if( CustomerStack ){ // Customer
        surf2 = WinGetSurface( win );
        v31 = surf1 + 9769;
        v7 = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        ScrCopy( &surf2[ 20 * v7 + 249 ], 64, v45 + 1, v7, v31, 480 );
        v8 = surf1 + 11689;        

        for( i = 0; i + gInvUnk78 < CustomerStack->Critter.Box.Cnt && i < gInvUnk08; ){
            ArtLoadImgScaled( ItemGetArtId( CustomerStack->Critter.Box.Box[ CustomerStack->Critter.Box.Cnt - ( i + gInvUnk78 + 1 ) ].obj ), v8, 56, 40, 480);
            v13 = &CustomerStack->Critter.Box.Box[ CustomerStack->Critter.Box.Cnt - ( i + gInvUnk78 + 1 ) ];
            InvPrintQuantity( v13->obj, v13->Quantity, v8, 480, i == a4 );
            v8 += 23040;
            i++;
        }

        v15 = DestPitch * (48 * gInvUnk08 + 24) + 169;
        if( gDlgPmExchangeMode ){
            msg1.Id = 30;
            if( MessageGetMsg( &gInvMsg, &msg1 ) == 1 ) sprintf( stmp, "%s %d", msg1.Text, ItemGetBackPackWeight( CustomerStack ) );            
        } else {
            sprintf( stmp, "$%d", ItemOffert( CustomerStack ) );
        }
        gFont.Print( &surf1[v15], stmp, 80, DestPitch, gPalColorCubeRGB[31][31][31] );
        area.lt = 169;
        area.tp = 24;
        area.bm = v45 + 24;
        area.rt = 233;
        WinAreaUpdate( gInvWin, &area );
    }
    if( MerchantStack ){ // Merchant side
	v7 = ( gVidMainGeo.rt - gVidMainGeo.lt + 1 );
        Surface = WinGetSurface( win );
        ScrCopy( &Surface[ 20 * v7 + 334], 64, v45 + 1, v7, &surf1[ 20 * DestPitch + 254 ], DestPitch );
        v38 = 48 * DestPitch;        
        v8 = &surf1[ 24 * DestPitch + 254 ];
        for( i = 0; i + gInvUnk77 < MerchantStack->Critter.Box.Cnt && i < gInvUnk08; ){
            v24 = ItemGetArtId( MerchantStack->Critter.Box.Box[ MerchantStack->Critter.Box.Cnt - ( i + gInvUnk77 + 1 ) ].obj );
            ArtLoadImgScaled( v24, v8, 56, 40, DestPitch );
            v27 = &MerchantStack->Critter.Box.Box[ MerchantStack->Critter.Box.Cnt - ( i + gInvUnk77 + 1 ) ];
            InvPrintQuantity( v27->obj, v27->Quantity, v8, DestPitch, i++ == a4 );
            v8 += v38;
        }
        if( gDlgPmExchangeMode ){
            msg2.Id = 30;
            if( MessageGetMsg( &gInvMsg, &msg2 ) == 1 ){
                sprintf( stmp, "%s %d", msg2.Text, InvStackPrice( gObjDude, gInvUnk39[ 0 ] ) );
            }
        } else {
            sprintf( stmp, "$%d", InvStackPrice( gObjDude, gInvUnk39[ 0 ] ) );
        }
        gFont.Print( &surf1[ DestPitch * (48 * gInvUnk08 + 24) + 254 ], stmp, 80, DestPitch, gPalColorCubeRGB[31][31][31] );
        area.lt = 254;
        area.tp = 24;
        area.bm = v45 + 24;
        area.rt = 318;
        WinAreaUpdate( gInvWin, &area );
    }
    FontSet( FontId );
}

int InvMenuBarter( int WinId, Obj_t *Merchant, Obj_t *MerchantStackObj, Obj_t *DudeStackObj, int BarterModifier )
{
    Obj_t *ArmorObj,*v53,*WeaponObj,*RhandObj;
    ObjStack_t *stk;
    MsgLine_t msg;
    int k,n,i,sel,v54,v58,v59;

    sel = -1;
    WeaponObj = NULL;
    v53 = NULL;
    gInvUnk66 = BarterModifier;
    if( InvInit() == -1 ) return -1;

    ArmorObj = InvGetArmorObj( Merchant );
    if( ArmorObj ) ItemUseItem( Merchant, ArmorObj, 1 );

    if( (RhandObj = InvGetRHandObj( Merchant )) ){
	ItemUseItem( Merchant, RhandObj, 1 );
    } else {
	if( !gDlgPmExchangeMode ){
    	    if( (WeaponObj = InvSearchObjByType( Merchant, PR_ITEM_WEAPON, 0 )) ) ItemUseItem( Merchant, WeaponObj, 1 );
	}
    }

    if( ObjCreate( &v53, 0, 467 ) == -1 ) return -1;
    Item16( Merchant, v53 );
    gInvBackPack = &gInvSelectedDude->Container;
    gInvUnk65 = MerchantStackObj;
    gInvUnk84 = DudeStackObj;
    gInvUnk78 = 0;
    gInvUnk77 = 0;
    gInvUnk79 = &DudeStackObj->Container;
    gInvUnk85 = &MerchantStackObj->Container;
    gInvUnk18 = WinId;
    gInvUnk40 = 0;
    gInvUnk62 = &Merchant->Container;
    gInvUnk39[0] = Merchant;
    gInvUnk63[0] = 0;
    v54 = InvMenuCreate( 3 );
    InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 3 );
    InvBpUpdate( gInvUnk04[0], -1, 3 );
    InvBodyUpdate( Merchant->ImgId, 3 );
    WinUpdate( gInvUnk18 );
    InvBarterStack( WinId, DudeStackObj, MerchantStackObj, -1 );
    InvSetInfoMode( 0 );
    switch( EvQeUnk20( EvQeUnk23( Merchant ), 0, 0 ) ){
	case 0: v59 = 25; break;
	case 1: v59 = 0; break;
	case 2: v59 = -15; break;
    }
    v58 = BarterModifier + v59;
    while( 1 ){
        if( sel == KEY_ESC || gMenuEscape ) break;
        sel = InpUpdate();
        if( sel == 17 || sel == 24 || sel == 324 ) SysQuitDlg();
        if( gMenuEscape ) break;
        if( sel == -1 ) continue;
        gInvUnk66 = v58;
        if( sel == 't' || v59 <= -30 ){
            ItemMoveObjInvToObj( MerchantStackObj, Merchant );
            ItemMoveObjInvToObj( DudeStackObj, gObjDude );
            GdialogUnk59();
            break;
        }
        switch( sel ){
    	    case 328:
        	if( gInvUnk04[ gInvUnk05 ] > 0 ){ gInvUnk04[ gInvUnk05 ]--; InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 3 ); }
        	continue;
    	    case 329:
        	if( gInvUnk78 > 0 ){ gInvUnk78--; InvBarterStack( WinId, DudeStackObj, MerchantStackObj, -1 ); }
        	continue;
    	    case 336:
    		if( gInvUnk04[ gInvUnk05 ] + gInvUnk08 < gInvBackPack->Box.Cnt ){ gInvUnk04[ gInvUnk05 ]++; InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 3 ); }
    		continue;
            case 109: // 'Trade' button
        	if( (DudeStackObj->Critter.Box.Cnt == 0) && (gInvUnk65->Critter.Box.Cnt == 0) ) continue;
        	if( InvBarterDo( gInvSelectedDude, DudeStackObj, Merchant, MerchantStackObj ) ) continue;
            	InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 3 );
            	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 3 );
            	InvBarterStack( WinId, DudeStackObj, MerchantStackObj, -1 );
            	msg.Id = 27;    // 'OK, That's good trade.'
            	if( !gDlgPmExchangeMode && MessageGetMsg( &gInvMsg, &msg ) == 1 ) GdialogReply( msg.Text );
        	continue;
            case 337:
    		if( gInvUnk78 + gInvUnk08 < gInvUnk79->Box.Cnt ){ gInvUnk78++; InvBarterStack( WinId, DudeStackObj, MerchantStackObj, -1 ); }
    		continue;       
            case 374:
        	if( gInvUnk08 + gInvUnk77 < gInvUnk85->Box.Cnt ){ gInvUnk77++; InvBarterStack( WinId, DudeStackObj, MerchantStackObj, -1 ); }
        	continue;
            case 388:
        	if( gInvUnk77 > 0 ){ gInvUnk77--; InvBarterStack( WinId, DudeStackObj, MerchantStackObj, -1 ); }
        	continue;
            case 397:
        	if( gInvUnk63[ gInvUnk40 ] <= 0 ) continue;
            	gInvUnk63[ gInvUnk40 ]--;
            	InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 3 );
            	WinUpdate( gInvWin );
        	continue;
            case 401:
        	if( gInvUnk08 + gInvUnk63[gInvUnk40] >= gInvUnk62->Box.Cnt ) continue;
            	gInvUnk63[ gInvUnk40 ]++;
            	InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 3 );
            	WinUpdate( gInvWin );
                continue;
            case 2500 ... 2501: 
        	InvUnk46( sel, 3 ); 
        	continue;
	}

        if( MseGetButtons() & 0x02 ){
            InvSetInfoMode( gInvInfoMode ? 0 : 1 );
            continue;
        }
        if( !(MseGetButtons() & 0x01) ) continue;        
        if( sel >= 1000 && sel < gInvUnk08 + 1000 ){
            if( gInvInfoMode == 1 ){
                InvActionMenu( sel, 3 );
                InvBarterStack( WinId, DudeStackObj, 0, -1 );
            } else { // dude inventory
        	n = sel - 1000;
        	if( n + gInvUnk04[ gInvUnk05 ] < gInvBackPack->Box.Cnt ){
            	    k = gInvUnk04[ gInvUnk05 ];
            	    stk = &gInvBackPack->Box.Box[ gInvBackPack->Box.Cnt - (n + k + 1) ];
                    InvMoveObj( stk->obj, stk->Quantity, n, k, Merchant, DudeStackObj, 1 );
                    InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 3 );
                    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 3 );
                    InvBarterStack( WinId, DudeStackObj, 0, -1 );
        	}
            }
            sel = -1;
        }
        if( sel >= 2000 && sel < gInvUnk08 + 2000 ){
            if( gInvInfoMode == 1 ){
                InvActionMenu( sel, 3 );
                InvBarterStack( WinId, 0, MerchantStackObj, -1 );
            } else { // merchant inventory
            	n = sel - 2000;
            	if( n + gInvUnk63[ gInvUnk40 ] < gInvUnk62->Box.Cnt ){
            	    k = gInvUnk63[ gInvUnk40 ];
                    stk = &gInvUnk62->Box.Box[ gInvUnk62->Box.Cnt - (n + k + 1) ];
                    InvMoveObj( stk->obj, stk->Quantity, n, k, Merchant, MerchantStackObj, 0 );
                    InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 3 );
                    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 3 );
                    InvBarterStack( WinId, 0, MerchantStackObj, -1 );
            	}
            }
            sel = -1;
        }
        if( !(sel < 2300 || sel >= 2300 + gInvUnk08 ) ){
    	    if( gInvInfoMode == 1 ){
        	InvActionMenu( sel, 3 );
            	InvBarterStack( WinId, DudeStackObj, 0, -1 );
    	    } else {
        	n = sel - 2300;
        	if( n < gInvUnk79->Box.Cnt ){
            	    stk = &gInvUnk79->Box.Box[ gInvUnk79->Box.Cnt - (n + gInvUnk78 + 1) ];
            	    InvUnk42( stk->obj, stk->Quantity, n, Merchant, DudeStackObj, 1 );
        	    InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 3 );
        	    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 3 );
            	    InvBarterStack( WinId, DudeStackObj, 0, -1 );
        	}
            }
            sel = -1;
        }
        if( sel >= 2400 && sel < gInvUnk08 + 2400 ){
            if( gInvInfoMode == 1 ){
                InvActionMenu( sel, 3 );
                InvBarterStack( WinId, 0, MerchantStackObj, -1 );
            } else {
                n = sel - 2400;
                if( n < gInvUnk85->Box.Cnt ){
                    stk = &gInvUnk85->Box.Box[ gInvUnk85->Box.Cnt - (n + gInvUnk77 + 1) ];
                    InvUnk42( stk->obj, stk->Quantity, n, Merchant, MerchantStackObj, 0 );
                    InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 3 );
                    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 3 );
            	    InvBarterStack( WinId, 0, MerchantStackObj, -1 );
                }
            }                            
            sel = -1;
        }        
    }
    ItemMoveObjInvToObj( v53, Merchant );
    ObjDestroy( v53, 0 );
    if( ArmorObj ){ ArmorObj->Flags |= 0x04000000; ItemAdd( Merchant, ArmorObj, 1 ); }
    if( RhandObj ){ RhandObj->Flags |= 0x02000000; ItemAdd( Merchant, RhandObj, 1 ); }
    if( WeaponObj ) ItemAdd( Merchant, WeaponObj, 1 );
    InvMenuClose( v54 );
    for( i = 0; i != 5; i++ ) ArtClose( gInvMseCursor[ i ].Obj );
    if( gInvUnk02 ) GameIfaceDisable( 0 );
    n = InvMsgClose();
    gInvArt[ 0 ].Xpos = 0;
    return n;
}

void InvUnk45( int a1, int a2 )
{
    int idx;

    if( a1 >= 2000 ){
        idx = gInvUnk62->Box.Cnt - (gInvUnk63[ gInvUnk40 ] + a1 - 2000 + 1);
        if( idx < gInvUnk62->Box.Cnt && gInvUnk40 < 9 && ItemGetObjType( gInvUnk62->Box.Box[ idx ].obj ) == 1 ){
            gInvUnk40++;
            gInvUnk39[ gInvUnk40 ] = gInvUnk62->Box.Box[ idx ].obj;
            gInvUnk63[ gInvUnk40 ] = 0;            
            gInvUnk62 = &gInvUnk39[ gInvUnk40 ]->Container;
            InvBodyUpdate( gInvUnk39[ gInvUnk40 ]->ImgId, a2 );
            InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, a2 );
            WinUpdate( gInvWin );
        }
    } else {
        idx = gInvBackPack->Box.Cnt - (gInvUnk04[gInvUnk05] + a1 - 1000 + 1);
        if( idx < gInvBackPack->Box.Cnt && gInvUnk05 < 9 && ItemGetObjType(gInvBackPack->Box.Box[idx].obj) == 1 ){
            gInvUnk05++;
            gInvParty[ gInvUnk05 ] = gInvBackPack->Box.Box[ idx ].obj;
            gInvSelectedDude = gInvParty[ gInvUnk05 ];
            gInvUnk04[ gInvUnk05 ] = 0;
            gInvBackPack = &gInvSelectedDude->Container;
            InvSetBodyImg();
            InvBodyUpdate( -1, a2 );
            InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, a2 );
        }
    }
}

void InvUnk46( int sel, int mode )
{
    Obj_t *obj;

    if( sel < 2500 ) return;    
    if( sel == 2500 ){
        if( gInvUnk05 > 0 ){
            gInvSelectedDude = gInvParty[ --gInvUnk05 ];
            gInvBackPack = &gInvSelectedDude->Container;
            InvSetBodyImg();
            InvBodyUpdate( -1, mode );
            InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, mode );
        }
    } else if( sel == 2501 && gInvUnk40 > 0 ){
        obj = gInvUnk39[ --gInvUnk40 ];
        gInvUnk62 = &obj->Container;
        InvBodyUpdate( obj->ImgId, mode );
        InvItemList( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, mode );
        WinUpdate( gInvWin );
    }    
}

int InvPickUpBoxQuantity( Obj_t *Critter, Obj_t *Item1, int Quantity, Obj_t **Item2, int quantity )
{
    int err;

    if( quantity <= 1 )
        Quantity = 1;
    else
        Quantity = InvPopUpDlg( 4, Item1, quantity );
    if( Quantity == -1 ) return -1;
    if( Quantity != -1 ){
	if( ItemUseItem( gInvSelectedDude, Item1, Quantity ) != -1 ) return -1;
    }
    if( (err = Item09( Critter, Item1, Quantity )) ){
        if( Quantity != -1 ) Item09( gInvSelectedDude, Item1, Quantity );
    } else if( Item2 ){
        if( Item2 == &gInvArmorObj ) InvUpdateStatistics( gInvParty[0], gInvArmorObj, 0 );
        *Item2 = NULL;
    }
    return err;
}

int InvLoadAmmo( Obj_t *item0, Obj_t *item1, Obj_t **item2, int quantity, int sel )
{
    int ObjType,i,err,n,v16;

    ObjType = ItemGetObjType( item0 );
    v16 = 0;
    if( ObjType != PR_ITEM_WEAPON ) return -1;
    if( ItemGetObjType( item1 ) != PR_ITEM_AMMO ) return -1;
    if( !ItemUnk01( item0, item1 ) ) return -1;

    if( quantity <= 1 )
        quantity = 1;
    else
        quantity = InvPopUpDlg( 4, item1, quantity );
    if( quantity == -1 ) return -1;

    n = ItemUseItem( gInvSelectedDude, item0, 1 );
    for( i = 0; i < quantity; i++ ){
        if( !( err = Item51( item0, item1 ) ) ){
            if( item2 ) *item2 = NULL;
            UseUnk06( item1 );
            v16 = 1;
            if( !InvPickItem( sel, &item1, 0, 0 ) ) break;
        }
        if( err != -1 ) v16 = 1;
        if( err ) break;
    }
    if( n != -1 ) ItemAdd( gInvSelectedDude, item0, 1 );
    if( !v16 ) return -1;
    GSoundPlay( GSoundWeaponFileName( 0, item0, 2, 0 ) );
    return 0;
}

void InvDrawCnt( int Value, int mode )
{
    char *img;
    int w, i, Number[ 5 ];
    char *surf, *p;
    VidRect_t Area;
    CachePool_t *ImgObj;
    
    if( !(img = ArtGetBitmap( ArtMakeId( 6, 170, 0, 0, 0 ), 0, 0, &ImgObj ) ) ) return;
    if( mode == 4 ){ // quantity
        Area.lt = 125;
        Area.tp = 45;
        Area.bm = 69;
        Area.rt = 195;
        w = WinGetWidth( gInvQdlgWin );
        surf = WinGetSurface( gInvQdlgWin );
        p = &surf[ 45 * w + 125 ];
        Number[ 4 ] = Value % 10; 
        Number[ 3 ] = Value / 10 % 10;
        Number[ 2 ] = Value / 100 % 10;
        Number[ 1 ] = Value / 1000 % 10;
        Number[ 0 ] = Value / 10000 % 10;
        for( i = 0; i < 5; i++ ){
            ScrCopy( img + 14 * Number[ i ], 14, 24, 336, p + 14 * i, w );
        }
    } else { // time
        Area.lt = 133;
        Area.tp = 64;
        Area.rt = 189;
        Area.bm = 88;
        w = WinGetWidth( gInvQdlgWin );
        p = &WinGetSurface( gInvQdlgWin )[ 64 * w + 133 ];
        ScrCopy( img + 14 * (Value / 60), 14, 24, 336, p, w );
        ScrCopy( img + 14 * (Value % 60 / 10), 14, 24, 336, p + 28, w );
        ScrCopy( img + 14 * (Value % 10), 14, 24, 336, p + 42, w );
    }
    ArtClose( ImgObj );
    WinAreaUpdate( gInvQdlgWin, &Area );
}

int InvPopUpDlg( int mode, Obj_t *Obj, int quantity )
{
    int Chng, Result, sel, Speed, min;

    InvPopUpDlgCreate( mode, Obj );
    Chng = 0;
    if( mode == 4 ){
        Result = 1;
        min = 1;
        if( quantity > 99999 ) quantity = 99999;
    } else {
        Result = 60;
        min = 10;
    }
    InvDrawCnt( Result, mode );
    while( 1 ){
        sel = InpUpdate();
        if( sel == KEY_ESC ){ InvPopUpClose( mode ); DD return -1; }
        if( sel == KEY_ENTER ){
    	    if( (Result >= min) && (Result <= quantity) && ( mode != 5 || !( Result % 10 ) ) ) break;
    	    GSoundPlay( "iisxxxx1" );
    	    continue;
        }
        if( sel == 5000 ){ // all
    	    Result = quantity;
    	    InvDrawCnt( quantity, mode );
    	    Chng = 0;
    	    continue;
        }
        if( sel == 6000 ){ // counter UP
            Chng = 0;
            if( Result < quantity ){
                if( mode == 4 ){
                    if( (MseGetButtons() & 4) != 0 ){
                        TimerGetSysTime();
                        Speed = 100;
                        while( (MseGetButtons() & 4) != 0 ){
                            if( Result < quantity ) Result++;
                            InvDrawCnt(Result, mode);
                            InpUpdate();
                            if( Speed > 1 ) TimerWaitProc( --Speed );
                        }
                    } else if( Result < quantity ){
                        Result++;
                    }
                } else {
                    Result += 10;
                }
		InvDrawCnt( Result, mode );
            }
	    continue;
        }
        if( sel == 7000 ){ // counter Dn
            Chng = 0;
            if( Result > min ){
                if( mode == 4 ){
                    if( MseGetButtons() & 4 ){
                        TimerGetSysTime();
                        Speed = 100;
                        while( MseGetButtons() & 4 ){
                            if( Result > min ) Result--;
                            InvDrawCnt( Result, mode );
                            InpUpdate();
                            if( Speed > 1 ) TimerWaitProc( --Speed );
                        }
                    } else if( Result > min ){
                        Result--;
                    }
                } else {
                    Result -= 10;
                }
		InvDrawCnt( Result, mode );
            }
	    continue;
        }
        if( mode == 4 ){
            if( sel >= '0' && sel <= '9' ){
                if( !Chng ) Result = 0;
                Result = 10 * Result % 100000 + sel - '0';
                Chng = 1;
		InvDrawCnt( Result, 4 );
                continue;
            }
            if( sel == KEY_BS ){
                if( !Chng ) Result = 0;
                Chng = 1;
                Result /= 10;
		InvDrawCnt( Result / 10, 4 );
                continue;
            }
        }        
    }
    GSoundPlay( "ib1p1xx1" );
    InvPopUpClose( mode );
    return Result;
}

int InvPopUpDlgCreate( int mode, Obj_t *Item )
{
    int Width, w, buth, butw, bt, pos, FontId;
    char *Img1, *Img2;
    MsgLine_t msg;
    CachePool_t *ImgObj;
    void *surf;

    FontId = FontGetCurrent();
    FontSet( 103 );
    memset( gInvQaDlgImgObj, 0, 8 * sizeof( void * ) );
    Width = gInvArt[ mode ].Width;
    gInvQdlgWin = WinCreateWindow( gInvArt[ mode + 1 ].Xpos, gInvArt[mode + 1].Ypos, Width, gInvArt[ mode ].Height, 257, 20 );
    surf = WinGetSurface( gInvQdlgWin );    
    if( ( Img1 = ArtGetBitmap( ArtMakeId( 6, gInvArt[ mode ].ArtId, 0, 0, 0 ), 0, 0, &ImgObj ) ) ){
        ScrCopy( Img1, Width, gInvArt[ mode ].Height, Width, surf, Width );
        ArtClose( ImgObj );
    }
    if( mode == 4 ){
        msg.Id = 21; // 'MOVE ITEMS'
        if( MessageGetMsg( &gInvMsg, &msg ) == 1 ){
            w = gFont.LineWidth( msg.Text );
            gFont.Print( surf + Width * 9 + (Width - w) / 2, msg.Text, 200, Width, gPalColorCubeRGB[20][19][3] );
        }
    } else if ( mode == 5 ){
        msg.Id = 23; // 'SET TIMER'
        if( MessageGetMsg( &gInvMsg, &msg ) == 1 ){
            w = gFont.LineWidth( msg.Text );
            gFont.Print( surf + Width * 9 + ( Width - w ) / 2, msg.Text, 200, Width, gPalColorCubeRGB[20][19][3] );
        }        
        if( ( Img1 = ArtGetBitmap( ArtMakeId( 6, 306, 0, 0, 0 ), 0, 0, &ImgObj ) ) ){
            ScrCopy( Img1, 105, 81, 105, surf + 34 * Width + 113, Width );
            ArtClose( ImgObj );
        }
    }
    ArtLoadImgScaled( ItemGetArtId( Item ), surf + 46 * Width + 16, 90, 61, Width );
    if( mode == 4 ){
        buth = 46; butw = 200;
    } else {
        buth = 64; butw = 194;
    }    
    if( (Img1 = ArtGetBitmap( ArtMakeId( 6, 193, 0, 0, 0 ), 0, 0, gInvQaDlgImgObj ) ) ){        
        if( (Img2 = ArtGetBitmap( ArtMakeId( 6, 194, 0, 0, 0 ), 0, 0, &gInvQaDlgImgObj[ 1 ] ) ) ){
            bt = WinCreateButton( gInvQdlgWin, butw, buth, 16, 12, -1, -1, 6000, -1, Img1, Img2, 0, 32 );
            if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
        }
    }    
    if( (Img1 = ArtGetBitmap( ArtMakeId( 6, 191, 0, 0, 0 ), 0, 0, &gInvQaDlgImgObj[ 2 ] ) ) ){        
        if( (Img2 = ArtGetBitmap( ArtMakeId( 6, 192, 0, 0, 0 ), 0, 0, &gInvQaDlgImgObj[ 3 ] ) ) ){
            bt = WinCreateButton( gInvQdlgWin, butw, buth + 12, 17, 12, -1, -1, 7000, -1, Img1, Img2, 0, 32 );
            if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
        }
    }    
    if( (Img1 = ArtGetBitmap( ArtMakeId(6, 8, 0, 0, 0), 0, 0, &gInvQaDlgImgObj[ 4 ] ) ) ){        
        if( (Img2 = ArtGetBitmap( ArtMakeId( 6, 9, 0, 0, 0 ), 0, 0, &gInvQaDlgImgObj[ 5 ] ) ) ){
            bt = WinCreateButton( gInvQdlgWin, 98, 128, 15, 16, -1, -1, -1, 13, Img1, Img2, 0, 32 );
            if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
            bt = WinCreateButton( gInvQdlgWin, 148, 128, 15, 16, -1, -1, -1, 27, Img1, Img2, 0, 32 );
            if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
        }
    }
    if( mode == 4 ){
        if( ( Img1 = ArtGetBitmap( ArtMakeId( 6, 307, 0, 0, 0 ), 0, 0, &gInvQaDlgImgObj[ 6 ] ) ) ){
            if( (Img2 = ArtGetBitmap( ArtMakeId( 6, 308, 0, 0, 0 ), 0, 0, &gInvQaDlgImgObj[ 7 ] ) ) ){
                msg.Id = 22; // 'ALL'
                if ( MessageGetMsg( &gInvMsg, &msg ) == 1 ){
                    w = gFont.LineWidth( msg.Text );
                    pos = (94 - w) / 2 + 376;
                    gFont.Print( &Img1[ pos ], msg.Text, 200, 94, gPalColorCubeRGB[20][19][3] );
                    gFont.Print( &Img2[ pos ], msg.Text, 200, 94, gPalColorCubeRGB[18][17][1] );
                }
                bt = WinCreateButton( gInvQdlgWin, 120, 80, 94, 33, -1, -1, -1, 5000, Img1, Img2, 0, 32 ); // all button
                if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
            }
        }
    }
    WinUpdate( gInvQdlgWin );
    InvSetInfoMode( 1 );
    FontSet( FontId );
    return 0;
}

void InvPopUpClose( int Mode )
{
    int Cnt, i;

    Cnt = ( Mode == 4 ) ? 8 : 6;
    for( i = 0; i < Cnt; i++ ) ArtClose( gInvQaDlgImgObj[ i ] );
    WinClose( gInvQdlgWin );
}

int InvSetTimer( Obj_t *a1 )
{
    int err, i, n;

    if( !( n = gInvArt[ 0 ].Xpos ) ){
	if( InvInit() == -1 ) return -1;
    }
    err = InvPopUpDlg( 5, a1, 180 );
    if( n ) return err;
    for( i = 0; i != 5; i++ ) ArtClose( gInvMseCursor[ i ].Obj );
    if( gInvUnk02 ) GameIfaceDisable( 0 );
    InvMsgClose();
    gInvArt[ 0 ].Xpos = 0;    
    return err;
}

