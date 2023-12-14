#include "FrameWork.h"

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
    { 0x00, 0x001, 0x030, 0x1F3, 0x179 },
    { 0x50, 0x000, 0x071, 0x124, 0x178 },
    { 0x50, 0x000, 0x072, 0x219, 0x178 },
    { 0x50, 0x000, 0x06F, 0x1E0, 0x0B4 },
    { 0x50, 0x122, 0x131, 0x103, 0x0A2 },

//    { 0x8C, 0x050, 0x131, 0x103, 0x0A2 },
//    { 0x8C, 0x050 } 
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

int gInvUnk80 = 0;
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

//    if( (gCombatStatus & 1) && Unk1004() != gInvSelectedDude ) return;
    if( InvInit() == -1 ) return;
    if( (gCombatStatus & 1) && gInvSelectedDude == gObjDude ){ // access inventory in combat
        tmp = INV_AP_ACCESS_COST - 2 * PerkLvl( gInvSelectedDude, 48 ); // AP cost for acccess to inventory
        if( (tmp > 0) && (gObjDude->Critter.State.CurrentAP < tmp) ){
            MsgLine.Id = 19; // 'You don't have enough action points to use inventory'
            if( MessageGetMsg( &gInvMsg, &MsgLine ) == 1 ) IfcMsgOut( MsgLine.Text );
            for( i = 0; i < 5; i++ ) ArtClose( gInvMseCursor[ i ].Obj );
//            if( gInvUnk02 ) GameIfaceDisable();
            InvMsgClose();
            gInvArt[0].Xpos = 0;
            return;
        }
        if( tmp > 0 ){
            if( tmp > gObjDude->Critter.State.CurrentAP )
                gObjDude->Critter.State.CurrentAP = 0;
            else
                gObjDude->Critter.State.CurrentAP -= tmp; // spent AP
//            IfaceUnk12(gObjDude->Critter.State.CurrentAP, gCombatMovePts);
        }
    }

    armor = InvGetArmorObj( gInvSelectedDude );
    menu = InvMenuCreate( 0 );
//    Unk9024( gInvSelectedDude );
    InvStatsUpdate();
    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 0 );
    InvSetInfoMode( 0 );
    while( 1 ){
        sel = InpUpdate();
        if( sel == KEY_ESC || gMenuEscape ) break;
        InvBodyUpdate( -1, 0 );
        if( GlobUnk01() == 5 ) break;
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
DD
        	gInvUnk04[ gInvUnk05 ] = gInvUnk04[ gInvUnk05 ] - gInvUnk08;
        	if( gInvUnk04[ gInvUnk05 ] < 0 ) gInvUnk04[ gInvUnk05 ] = 0;
        	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 0);
        	break;
    	    case 335:
DD
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
DD
        	gInvUnk04[ gInvUnk05 ] = gInvUnk08 + gInvUnk04[ gInvUnk05 ];
        	if( gInvUnk04[ gInvUnk05 ] + gInvUnk08 >= gInvBackPack->Box.Cnt ){
            	    tmp = gInvBackPack->Box.Cnt - gInvUnk08;
            	    gInvUnk04[ gInvUnk05 ] = gInvBackPack->Box.Cnt - gInvUnk08;
            	    if( tmp < 0 ) gInvUnk04[ gInvUnk05 ] = 0;
        	}
        	InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 0 );
        	break;                
    	    case 2500: // click on portrait
DD
//    		InvUnk46( 2500, 0 ); 
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
//        ObjUnk20(g, gInvBodyImgId, v23);
//        TileUpdateArea(v23, g->Elevation);
    }
    // armor changed ?
    if( (gInvSelectedDude == gObjDude) && (armor != InvGetArmorObj( gInvSelectedDude )) ) IfaceResetAC( 1 );
    InvMenuClose( menu ) ;
    for( i = 0; i != 5; i++ ) ArtClose( gInvMseCursor[i].Obj );
//    if( gInvUnk02 ) GameIfaceDisable();
    InvMsgClose();
    gInvArt[ 0 ].Xpos = 0;
//    if( g == gObjDude ) IfaceHandSlotUpdate( 0, -1, -1 );
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
        ScrCopy( WinGetSurface( gInvUnk18 ) + 80, 480, 180, gVidMainGeo.rt - gVidMainGeo.lt + 1, WinGetSurface( gInvWin ), 480 );
//        gInvInfoCb = sub_445448;
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
        if( idx == 2 && !gInvUnk80 ){
    	    if( (img1 = ArtGetBitmap( ArtMakeId( 6, 436, 0, 0, gInvUnk80 ), 0, 0, &gInvUnk20[ 8 ] ) ) ){
    		if( (img2 = ArtGetBitmap( ArtMakeId( 6, 437, 0, 0, gInvUnk80 ), 0, 0, &gInvUnk20[ 9 ] )) ){
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
    k = MapUnk35();
    GmouseSetIfaceMode( 0 );
    return k;
}

void InvMenuClose( int a1 )
{
//    Obj_t *v2; // eax
//    char v3; // dl
//    Obj_t *v4; // ebx
//    Obj_t *v5; // eax
//    Obj_t *v6; // edx
//    Obj_t *v7; // eax
//    Obj_t *v8; // edx
    int v9; // esi
//    CachePool_t *v10; // eax
//    int result; // eax
//    Obj_t *v12; // ebp
//    int v13; // edi
//    int v14; // esi
//    int v15[15]; // [esp+0h] [ebp-FCh] BYREF
//    int v16; // [esp+3Ch] [ebp-C0h]
//    Obj_t *dude[30]; // [esp+40h] [ebp-BCh]
//    int v18[10]; // [esp+B8h] [ebp-44h] BYREF
//    int i05; // [esp+E0h] [ebp-1Ch]

    gInvSelectedDude = gInvParty[0];
    if( gInvLHandObj ){
        gInvLHandObj->Flags |= 0x1000000;
        if( gInvLHandObj == gInvRHandObj ) gInvRHandObj->Flags |= 0x2000000 | 0x1000000;
        ItemAdd( gInvSelectedDude, gInvLHandObj, 1 );
    }
    if( gInvRHandObj && gInvRHandObj != gInvLHandObj ){
        gInvRHandObj->Flags |= 0x2000000;
        ItemAdd( gInvSelectedDude, gInvRHandObj, 1 );
    }
    if( gInvArmorObj ){
        gInvArmorObj->Flags |= 0x4000000;
        ItemAdd( gInvSelectedDude, gInvArmorObj, 1 );
    }
    gInvRHandObj = NULL;
    gInvArmorObj = NULL;
    gInvLHandObj = NULL;
    for( v9 = 0; v9 < 12; v9++ ) ArtClose( gInvUnk20[ v9++ ] );
    if( a1 ) MapUnk34();
    WinClose( gInvWin );
    GmouseSetIsoMode();
    if( !gInvUnk83 ) return 0;
/*
//        sub_422EC4(v15, gObjDude, 4, 0, 3);
//        v15[5] = 256;
//        v15[9] = gObjDude->GridId;
    sub_423C10(v15, 0, 1, 0);
    v12 = 0;
    i05 = gObjDude->Effect.i05;
    result = v16;        
    for( v13 = v14 = 0; v13 < v16; v13++, v14++ ){
        result = dude[v14];
        if ( result != gObjDude && i05 != *(result + 80) ){
            result = FeatDice( result, 1, 0, 0 );
            if( result >= 2 ){
                result = CritterUnk45( dude[v14], gObjDude );
                if( !v12 ) v12 = dude[v14];
            }
        }            
    }
        
    if( v12 ){
        if( (gCombatStatus & 1) == 0 ){
            v18[0] = v12;
            v18[1] = gObjDude;
            memset(&v18[2], 0, 16);
            v18[7] = 0;
            v18[6] = 0x7FFFFFFF;
            result = ScptUnk121(v18);
        }
    }
    gInvUnk83 = 0;
*/    
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

int InvUnk03( int a1, int a2, ObjBox_t *box, int a4 )
{
    char *aa,*surf,*bmp,*surfA;
    int v4,v10,_i,v12,rr,v18,v26,i;
    ObjStack_t *v16;
    CachePool_t *ImgObj;

    surf = WinGetSurface( gInvWin );
    if( a4 == 2 ){
        v4 = 537;
        if( (bmp = ArtGetBitmap( ArtMakeId( 6, 114, 0, 0, 0 ), 0, 0, &ImgObj )) ){
            ScrCopy( bmp + 20166, 64, 48 * gInvUnk08, 537, surf + 20166, 537 );
            ArtClose( ImgObj );
        }
    } else if( a4 == 3 ){
        v4 = 480;
        surfA = WinGetSurface( gInvUnk18 );
        v10 = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        v10 = (v10 * 9)/4 - v10;
        ScrCopy( &surfA[v10 + 475], 64, 48 * gInvUnk08, gVidMainGeo.rt - gVidMainGeo.lt + 1, surf + 17195, 480 );
    }
    _i = 0;
    v26 = 0;
    for( i = a1 + 1; ; i++, _i++, v26 += 48 ){
        rr = _i + a1;
        if( _i + a1 >= box->Cnt || _i >= gInvUnk08 ) break;
        if( a4 == 2 ){
            v12 = v4 * (v26 + 41) + 301;
        } else if( a4 == 3 ){
            v12 = v4 * (v26 + 39) + 397;
        }
        aa = &surf[v12];
        ArtLoadImgScaled( ItemGetArtId( box->Box[ box->Cnt - i ].obj ), aa, 56, 40, v4 );
        v18 = (_i == a2);
        v16 = &box->Box[ box->Cnt - i ];
        InvPrintQuantity( v16->obj, v16->Quantity, aa, v4, v18 );
    }
    if( a4 == 2 ){
        if( gInvUnk36 != -1 ){
            if( a1 <= 0 )
                rr = WinDisableWidget( gInvUnk36 );
            else
                rr = WinEnableWidget( gInvUnk36 );
        }
        if( gInvUnk37 != -1 ){
            if( box->Cnt - a1 <= gInvUnk08 )
                return WinDisableWidget( gInvUnk37 );
            else
                return WinEnableWidget( gInvUnk37 );
        }
    }
    return rr;
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
//    gInvUnk02 = GameIfaceStat();
//    if( gInvUnk02 ) GameIfaceEnable();
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
//        if( gInvUnk02 ) GameIfaceDisable();
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
    TileUpdateArea( &Area, gCurrentMapLvl );    
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
DD
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
//    int ap;
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
//        	InvUnk46( sel, 1 );
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
                    if( gCombatStatus & 1 ){
//                	if( gObjDude->Critter.State.CurrentAP >= 2 && ActionuNk23(gObjDude, a1, (int)gInvBackPack->Box.Box[v13].obj) != -1 ){
//                            ap = gObjDude->Critter.State.CurrentAP;
//                            if( ap < 2 )
//                            	gObjDude->Critter.State.CurrentAP = 0;
//                            else
//                            	gObjDude->Critter.State.CurrentAP = ap - 2;
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
//    if( gInvUnk02 ) GameIfaceDisable();
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

    for( i = 0; i >= Obj->Container.Box.Cnt; i++ ){
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
    Obj_t *obj;

    if( ObjPid == Critter->TimeEv ) return Critter;
    for( i = 0; i < Critter->Critter.Box.Cnt; i++ ){
        if( ObjPid == Critter->Critter.Box.Box[ i ].obj->TimeEv ) return Critter->Critter.Box.Box[ i ].obj;
        if( ItemGetObjType( Critter->Critter.Box.Box[ i ].obj ) != PR_ITEM_CONTAINER ) continue;
        if( ( Critter = InvSearchObjByPid( obj, ObjPid ) ) ) return Critter;
    }    
    return NULL;
}

Obj_t *InvUnk27( Obj_t *a1, int a2 )
{
//    p_Feat = &a1->Feat;
//    if ( a2 >= 0 && a2 < p_Feat->Critter.Box.Cnt )
//        return p_Feat->Critter.Box.Box[a2].obj;
//    else
//        return 0;
}

int InvUnk28( Obj_t *a1, Obj_t *a2, int a3)
{
    return InvUnk29(a1, a2, 1, a3);
}

int InvUnk29( Obj_t *a1, Obj_t *a2, int a3, int a4 )
{
/*
    Obj_t *ArmorObj; // eax
    Obj_t *v8; // ecx
    __int16 v9; // dx
    int v10; // edx
    __int16 v11; // bx
    __int16 v12; // cx
    int Id; // eax
    int result; // eax
    Obj_t *RHandObj; // eax
    int Pid; // edx
    int LtInt; // ebx
    int LtRad; // edx
    int v19; // edx
    int v20; // ebx
    int v21; // ecx
    char *v22; // eax
    int v23; // ebx
    int v24; // [esp-4h] [ebp-2Ch]
    VidRect_t v25; // [esp+0h] [ebp-28h] BYREF
    Proto_t *proto; // [esp+10h] [ebp-18h] BYREF
    int v27; // [esp+14h] [ebp-14h]
    int v28; // [esp+18h] [ebp-10h]

    v27 = a4;
    if ( a3 && !MapUnk21() )
        UNK_A04(2);
    if ( ItemGetObjType((Obj_t *)a2) )
    {
        if ( (Obj_t *)a1 == gObjDude )
            v28 = IfaceGetSelectedHand();
        else
            v28 = 1;
        v24 = *(_DWORD *)(a1 + 28) + 1;
        Item58((Obj_t *)a2);
        v11 = Item45((Obj_t *)a2, v10);
        Id = ArtMakeId(1, *(_WORD *)(a1 + 32) & 0xFFF, v11, v12, v24);
        if ( !ArtFileExist(Id) )
        {
            eprintf(aInvenWieldFail);
            return -1;
        }
        if ( v27 )
        {
            RHandObj = InvGetRHandObj((Obj_t *)a1);
            *(_BYTE *)(a2 + 39) |= 2u;
        }
        else
        {
            RHandObj = InvGetLHandObj((Obj_t *)a1);
            *(_BYTE *)(a2 + 39) |= 1u;
        }
        if ( RHandObj )
        {
            Pid = RHandObj->Pid;
            HIBYTE(RHandObj->Flags) &= 0xFCu;
            if ( Pid == 205 )
            {
                if ( (Obj_t *)a1 == gObjDude )
                {
                    LtInt = 0x10000;
                    LtRad = 4;
                }
                else
                {
                    result = ProtoGetObj(*(_DWORD *)(a1 + 100), &proto);
                    if ( result == -1 )
                        return result;
                    LtInt = proto->LtInt;
                    LtRad = proto->LtRad;
                }
                ObjSetLight((Obj_t *)a1, LtRad, LtInt, &v25);
            }
        }
        if ( *(_DWORD *)(a2 + 100) == 205 )
        {
            v19 = *(_DWORD *)(a2 + 108);
            if ( v19 < *(_DWORD *)(a1 + 108) )
                v19 = *(_DWORD *)(a1 + 108);
            v20 = *(_DWORD *)(a2 + 112);
            if ( v20 < *(_DWORD *)(a1 + 112) )
                v20 = *(_DWORD *)(a1 + 112);
            ObjSetLight((Obj_t *)a1, v19, v20, &v25);
            TileUpdateArea(&v25, gCurrentMapLvl);
        }
        if ( ItemGetObjType((Obj_t *)a2) == 3 )
            LOWORD(v21) = Item58((Obj_t *)a2);
        else
            LOWORD(v21) = 0;
        if ( v28 == v27 )
        {
            if ( (*(_DWORD *)(a1 + 32) & 0xF000) >> 12 && a3 && !MapUnk21() )
            {
                v22 = GSoundProtoFname6(a1, 39, 0);
                sub_41541C((int *)a1, v22, 0);
                sub_4149D0(a1, 0);
            }
            if ( !a3 || MapUnk21() )
            {
                v23 = ArtMakeId(1, *(_DWORD *)(a1 + 32) & 0xFFF, 0, v21, *(_DWORD *)(a1 + 28) + 1);
                Unk9025((Obj_t *)a1, *(_DWORD *)(a1 + 28), v23);
            }
            else if ( v21 )
            {
                sub_415238(a1, v21, -1);
            }
            else
            {
                ArtMakeId(1, *(_DWORD *)(a1 + 32) & 0xFFF, 0, 0, *(_DWORD *)(a1 + 28) + 1);
                sub_41518C(a1);
            }
        }
    }
    else
    {
        ArmorObj = InvGetArmorObj((Obj_t *)a1);
        if ( ArmorObj )
            HIBYTE(ArmorObj->Flags) &= ~4u;
        *(_BYTE *)(a2 + 39) |= 4u;
        if ( FeatGetVal((Obj_t *)a1, 34) == 1 )
            Item81((Obj_t *)a2);
        else
            Item80((Obj_t *)a2);
        if ( (Obj_t *)a1 == gObjDude )
        {
            if ( !MapUnk21() )
            {
                ArtMakeId(1, v9, 0, (*(_DWORD *)(a1 + 32) & 0xF000) >> 12, *(_DWORD *)(a1 + 28) + 1);
                sub_41518C(a1);
            }
        }
        else
        {
            InvUpdateStatistics((Obj_t *)a1, v8, (Obj_t *)a2);
        }
    }
    if ( !a3 || MapUnk21() )
        return 0;
    else
        return UNK_A06();
*/
}

int InvUnk31( Obj_t *a1, int a2 )
{
    return InvUnk30(a1, a2, 1);
}

int InvUnk30( Obj_t *a1, int a2, int a3 )
{
/*
    Obj_t *RHandObj; // eax
    int v5; // edx
    int v6; // ecx
    int v7; // edx
    char *v8; // eax
    __int16 v9; // cx
    int Id; // ebx

    if( a1 == gObjDude ) IfaceGetSelectedHand();
    if ( a2 )
        RHandObj = InvGetRHandObj(a1);
    else
        RHandObj = InvGetLHandObj(a1);
    if ( RHandObj )
        HIBYTE(RHandObj->Flags) &= 0xFCu;
    if ( v6 == v5 && (a1->ImgId & 0xF000) >> 12 )
    {
        if ( a3 && !MapUnk21() )
        {
            UNK_A04(2);
            v8 = GSoundProtoFname6((int)a1, v7, 0);
            sub_41541C(&a1->TimeEv, v8, 0);
            sub_4149D0((int)a1, 0);
            ArtMakeId(1, a1->ImgId & 0xFFF, 0, v9, a1->Orientation + 1);
            sub_41518C((int)a1);
            return UNK_A06();
        }
        Id = ArtMakeId(1, a1->ImgId & 0xFFF, 0, 0, a1->Orientation + 1);
        Unk9025(a1, a1->Orientation, Id);
    }
*/
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
    	        Box = gInvUnk84->Obj;
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
//                	UseUnk05( BoxObj, Item);
                    } else {
                        if( !ItemUseItem( BoxObj, Item, Available - 1 ) ){
                    	    if( !InvPickItem( sel, &obj, &BoxObj, &HandObj ) ){
                        	ItemAdd( BoxObj, Item, Available - 1 );
                    	    } else {
                    		ItemSetMoney( obj, Available );
//                	    	UseUnk05( BoxObj, obj );
                    	    }
                        }
                    }
            	    break;
                }
            } else {
DD
//                if( Item->Pid == PID_DYNAMITE || Item->Pid == PID_PLASTICEXPLOSIVES ){ gInvUnk83 = 1; UseUnk05( BoxObj, Item ); break; }
//                if( Available <= 1 ){ UseUnk05( BoxObj, Item ); break; }
                Available = InvPopUpDlg( 4, Item, Available );
                for( tmp = 0; tmp < Available; tmp++ ){
//                    if( InvPickItem( sel, &Item, &BoxObj, &HandObj ) ) UseUnk05( BoxObj, Item );
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
//                    InvUnk45( sel, mode );
                    break;
                case PR_ITEM_DRUG:
                    if( ItemUnk07( gInvParty[0], Item ) == 1 ){
                        if( HandObj )
                            *HandObj = NULL;
                        else
                            ItemUseItem( BoxObj, Item, 1 );
//                        ObjUnk14( Item, gObjDude->GridId, gObjDude->Elevation, 0 );
//                        UseUnk06( &Item->TimeEv );
                    }
                    IfaceRenderHP( 1 );
                    break;
                case PR_ITEM_WEAPON: case PR_ITEM_MISC:
                    if( !HandObj ) ItemUseItem( BoxObj, Item, 1 );
//                    if( ObjOpenable( Item ) )
//                        tmp = UseUnk13( v20, Item );
//                    else
//                        tmp = UseUnk17( gInvParty[0], gInvParty[0], Item );
                    if( tmp == 1 ){
                        if( HandObj ) *HandObj = NULL;
//                        ObjUnk14( Item, gObjDude->GridId, gObjDude->Elevation, 0 );
//                        UseUnk06( &Item->TimeEv );
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
//    if( mode == 2 || mode == 3 ) InvUnk03( gInvUnk63[ gInvUnk40 ], -1, gInvUnk62, mode );
    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, mode );
//    if( mode == 3 ) InvUnk43( gInvUnk18, gInvUnk84, gInvUnk65, -1 );
    InvSetBodyImg();
    return;
}

int InvMenuSteal( Obj_t *Critter, Obj_t *Obj2 )
{

    int v3; // edi
    ArtFrm_t *img; // eax
    int v5; // edx
    Obj_t *LHandObj; // eax
    Obj_t *RHandObj; // eax
    Obj_t *ArmorObj; // eax
    int num; // ebx MAPDST
    int ii; // eax
    int _i; // esi
    char Obj; // cl
    int id; // eax MAPDST
    char *bmp1; // esi
    char *bmp2; // eax
    int bt; // eax MAPDST
    char *bmp3; // esi
    char *bmp4; // eax
    int v25; // eax
    Obj_t **v26; // edx
    int v27; // ebx
    int sel; // esi MAPDST
    int v30; // ecx
    int v31; // edx
    int v32; // ecx
    int v33; // edx
    Obj_t *BackPackWeight; // eax
    int v35; // edx
    int v36; // ecx
    int v37; // esi
    Obj_t *v38; // eax
    Obj_t *v39; // eax
    int v40; // edx
    int v41; // eax
    int v42; // ecx
    int v43; // esi
    Obj_t *v44; // eax
    int v45; // eax
    int v46; // ebx
    int i; // esi
    CachePool_t *v48; // eax
    Obj_t *v49; // edx
    Obj_t *v50; // eax
    Obj_t *v51; // eax
    int v52; // esi
    signed int Total; // eax
    int j; // esi
    CachePool_t *v55; // eax
    char stmp[200]; // [esp+0h] [ebp-150h] BYREF
    int atmp[4]; // [esp+C8h] [ebp-88h]
    MsgLine_t msg; // [esp+D8h] [ebp-78h] BYREF
    CachePool_t *ImgObj2[4]; // [esp+E8h] [ebp-68h] BYREF
    Obj_t *NewItem; // [esp+F8h] [ebp-58h] BYREF
    Obj_t **ObjTable; // [esp+FCh] [ebp-54h] BYREF
    unsigned int Id; // [esp+100h] [ebp-50h] BYREF
    int v64; // [esp+104h] [ebp-4Ch]
    CachePool_t *ImgObj1; // [esp+108h] [ebp-48h] BYREF
    int k; // [esp+108h] [ebp-48h] FORCED
    ScptCache_t *pScript; // [esp+10Ch] [ebp-44h] BYREF
    ScptCache_t *v68; // [esp+110h] [ebp-40h] BYREF
    Obj_t *item; // [esp+114h] [ebp-3Ch]
    Obj_t *v70; // [esp+118h] [ebp-38h]
    Obj_t *dude2; // [esp+11Ch] [ebp-34h]
    int v72; // [esp+120h] [ebp-30h]
    int v73; // [esp+124h] [ebp-2Ch]
    int v76; // [esp+130h] [ebp-20h]
    int v78; // [esp+138h] [ebp-18h]


    Id = -1;
    item = v70 = dude2 = NewItem = v76 = v78 = v73 = 0;
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

    if( (OBJTYPE( Obj2->ImgId ) == TYPE_ITEM) || ItemGetObjType( Obj2 ) == PR_ITEM_CONTAINER || (Obj2->FrameNo == 0) ){
	if( (img = ArtLoadImg( Obj2->ImgId, &ImgObj1 ) ) ){
	    v5 = ArtGetFpd( img );
	    ArtClose( ImgObj1 );
	    if( v5 > 1 )  return 0;
	}
    }
/*
    if( gInvUnk80 && UseGetScriptId( Obj2, &Id ) != -1 ){
	ScptUnk138( Id, Critter, 0 );
	ScptExecScriptProc( Id );
	if( ScptPtr(Id, (Scpt_t **)&pScript) == -1 || pScript->Script[0].i18 ) return 0;
    }
*/
    if( InvInit() == -1 ) return 0;
    gInvUnk62 = &Obj2->Container;
    gInvUnk40 = 0;
    gInvUnk63[ 0 ] = 0;
    gInvUnk39[ 0 ] = Obj2;
    if( ObjCreate( &NewItem, 0, 467) == -1 ) return 0;
    Item16( Obj2, NewItem );

    if( gInvUnk80 ){
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
    if( !gInvUnk80 && OBJTYPE( Obj2->ImgId ) == TYPE_CRIT ){
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

    InvUnk03( gInvUnk63[gInvUnk40], -1, &gInvUnk62->Box, 2 );
    v27 = 2;
    InvBpUpdate( gInvUnk04[gInvUnk05], -1, 2 );
    InvBodyUpdate( Obj2->ImgId, 2 );
    InvSetInfoMode( 0 );
    v72 = v73 - 1;

    do{
        if( gMenuEscape || v76 ) break;
        sel = InpUpdate();
        if( sel == 17 || sel == 24 || sel == 324 ) SysQuitDlg();
        if( gMenuEscape ) break;        
        if( sel < 336 ){
            if( sel < 328 ){
                if( sel != 65 ) continue;
                if( gInvUnk80 ) continue;
                FeatGetVal( Critter, 12 );
                ItemGetBackPackWeight( Critter );
                BackPackWeight = ItemGetBackPackWeight( Obj2 );
            	if( BackPackWeight <= v35 ){
                    Item15( Obj2, Critter );
                    InvUnk03( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, v36 );
                    v27 = 2;
                    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 2 );
            	} else {
                    v27 = 31;
                    msg.Id = 31;// 'Sorry, you cannot carry that much.'
                    if( MessageGetMsg(&gInvMsg, &msg) == 1 ) {
                        v27 = 0;
//                        DlgBox( msg.Text, 0, 0, 169, 117, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 0 );
                    }
            	}            	    
                continue;                
            } else {
                if( sel == 328 ){
                    v27 = gInvUnk04[ gInvUnk05 ];
                    if( gInvUnk04[ gInvUnk05 ] > 0 ){
                        gInvUnk04[ gInvUnk05 ]--;
                        InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 2 );
                        v27 = 2;
                    }
                    continue;
                }
                if( sel == 329 ){
LABEL_81:
            	    if( v73 ){
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
                        InvUnk03( 0, -1, &Obj2->Container.Box, 2 );
                        v27 = 2;
                        InvBpUpdate( gInvUnk04[gInvUnk05], -1, 2 );
                        InvBodyUpdate( Obj2->ImgId, 2 );
                    }
                    continue;
                }
            }
        } else {
            if( sel <= 336 ){
                v27 = gInvUnk04[gInvUnk05] + gInvUnk08;
                if( v27 < gInvBackPack->Box.Cnt ){
                    v27 = 2;
                    gInvUnk04[ gInvUnk05 ]++;
                    InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 2 );
                }
                continue;
            }
            if( sel < 397 ){
                if( sel == 337 ) goto LABEL_81;
            } else {
                if( sel == 397 ){
                    if( gInvUnk63[gInvUnk40] > 0 ){
                        v27 = gInvUnk62;
                        gInvUnk63[ gInvUnk40 ]--;
                        InvUnk03( gInvUnk63[ gInvUnk40 ], -1, gInvUnk62, 2 );
                        WinUpdate( gInvWin );
                    }
                    continue;
                }
                if( sel >= 401 ){
                    if( sel == 401 ){
                        v27 = gInvUnk62;
                        if( gInvUnk08 + gInvUnk63[gInvUnk40] < gInvUnk62->Box.Cnt ){
                            gInvUnk63[ gInvUnk40 ]++;
                            InvUnk03( gInvUnk63[ gInvUnk40 ], -1, v27, 2 );
                            WinUpdate( gInvWin );
                        }
                        continue;
                    }
                    if( sel >= 2500 && sel <= 2501 ){
                        InvUnk46( sel, 2 );
                        continue;
                    }
                }
            }
        }
        if( ( MseGetButtons() & 0x02 ) && !gInvUnk80 ){
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
//                            gInvUnk60++;
//                            gInvUnk61 += Item24( v44 );
//                            v45 = InvUnk38( gInvUnk62->Box.Box[gInvUnk62->Box.Cnt - (v43 + gInvUnk63[gInvUnk40] + 1)].obj, v43, gInvUnk39[gInvUnk40], 0 );
                            if( v45 == 1 ){
                                v76 = 1;
                            } else if ( v45 == 2 ) {
                                v78 += v3;
                                v3 += 10;
                            }
                            InvUnk03( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 2 );
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
//                        gInvUnk60++;
                        v39 = Item24( v38 );
                        v40 = v37 + gInvUnk04[ gInvUnk05 ];
//                        gInvUnk61 += v39;
//                        v41 = InvUnk38( gInvBackPack->Box.Box[ gInvBackPack->Box.Cnt - (v40 + 1) ].obj, v37, gInvUnk39[ gInvUnk40 ], 1 );
                        if( v41 == 1 ){
                            v76 = 1;
                        } else if ( v41 == 2 ){
                            v78 += v3;
                            v3 += 10;
                        }
                        InvUnk03( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, 2 );
                        v27 = 2;
                        InvBpUpdate( gInvUnk04[ gInvUnk05 ], -1, 2 );
                    }
                    sel = -1;
                    continue;
                }
                InvActionMenu(sel, 2);
            }
LABEL_122:
        } while( sel != KEY_ESC );
    	if( v73 ){
    	    ObjCritterListDestroy( ObjTable );
    	    for( i = 0; i != 4; i++ ) ArtClose( ImgObj2[ i ] );            
    	}
    	if( gInvUnk80 ){
    	    if( item  ){ v27 = 1; item->Flags  |= 0x1000000; ItemAdd( Obj2, item, 1  ); }
    	    if( v70   ){ v27 = 1; v70->Flags   |= 0x2000000; ItemAdd( Obj2, v70, 1   ); }
    	    if( dude2 ){ v27 = 1; dude2->Flags |= 0x4000000; ItemAdd( Obj2, dude2, 1 ); }
    	}
    	Item15( NewItem, Obj2 );
//    	ObjDestroy( NewItem, 0, v27 );
    	if( gInvUnk80 && !v76 ){
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
//    	if( gInvUnk02 ) GameIfaceDisable();
    	InvMsgClose();
    	gInvArt[0].Xpos = 0;
//        if( gInvUnk80 && v76 && gInvUnk60 > 0 && UseGetScriptId( Obj2, &Id ) != -1 ){
//    	    ScptUnk138(Id, Critter, 0);
//    	    ScptExecScriptProc(Id);
//    	    ScptPtr(Id, &v68);
//	}
    	return 0;
}

void InvStealAttempt( Obj_t *Thief, Obj_t *Npc )
{
    if( Thief == Npc ) return;
    gInvUnk80 = OBJTYPE( Thief->Pid ) == TYPE_CRIT && CritterCanTalk( Npc );
//    gInvUnk60 = 0;
//    gInvUnk61 = 0;
    InvMenuSteal( Thief, Npc );
    gInvUnk80 = 0;
//    gInvUnk60 = 0;
//    gInvUnk61 = 0;
}

int InvUnk38( Obj_t *a1, int a2, Obj_t *a3, int a4 )
{
/*
    int Quantity; // esi
    int v6; // edx
    char *Surface; // eax
    int v8; // ecx
    __int16 v9; // dx
    int Id; // eax
    char *bmp; // eax
    int ArtId; // eax
    CachePool_t **v13; // edx
    ArtFrm_t *Img; // eax
    ArtFrm_t *v15; // ecx
    ArtFrmHdr_t *v16; // ecx
    char *ObjData; // eax
    Obj_t *v18; // esi
    Obj_t *v20; // esi
    char v21; // dl
    int v22; // [esp-Ch] [ebp-60h]
    int v23; // [esp-8h] [ebp-5Ch]
    VidRect_t area; // [esp+0h] [ebp-54h] BYREF
    MsgLine_t msg; // [esp+10h] [ebp-44h] BYREF
    CachePool_t *Obj; // [esp+20h] [ebp-34h] BYREF
    int ObjWidth; // [esp+24h] [ebp-30h]
    int ObjHeight; // [esp+28h] [ebp-2Ch]
    char *v29; // [esp+2Ch] [ebp-28h]
    ArtFrm_t *v30; // [esp+30h] [ebp-24h]
    int v31; // [esp+34h] [ebp-20h]
    int v32; // [esp+38h] [ebp-1Ch]
    int v33; // [esp+3Ch] [ebp-18h]
    int v34; // [esp+40h] [ebp-14h]
    int v35; // [esp+44h] [ebp-10h]

    v33 = a4;
    v35 = 0;
    v34 = 1;
    if( a4 ){
        area.tp = 48 * a2 + 37;
        area.lt = 176;
        Quantity = gInvBackPack->Box.Box[ gInvBackPack->Box.Cnt - ( a2 + gInvUnk04[ gInvUnk05 ] + 1 ) ].Quantity;
        if( Quantity > 1 ){
            InvBpUpdate( gInvUnk04[ gInvUnk05 ], a2, 2 );
            v34 = 0;
        }
    } else {
        area.lt = 297;
        area.tp = 48 * a2 + 37;
        Quantity = gInvUnk62->Box.Box[ gInvUnk62->Box.Cnt - ( a2 + gInvUnk63[ gInvUnk40 ] + 1 ) ].Quantity;
        if( Quantity > 1 ){
            InvUnk03( gInvUnk63[ gInvUnk40 ], a2, &gInvUnk62->Box, 2 );
            WinUpdate( gInvWin );
            v34 = v6;
        }
    }
    if( v34 ){
        Surface = WinGetSurface( gInvWin );
        v31 = 64;
        v32 = v8;
        v29 = Surface;
        bmp = ArtGetBitmap( ArtMakeId( 6, v9, 0, 0, 0 ), 0, 0, &Obj );
        if( bmp ){
            ScrCopy( &bmp[ 537 * area.tp + area.lt ], 64, 48, 537, &v29[ 537 * area.tp + area.lt ], 537 );
            ArtClose( Obj );
        }
        area.rt = v31 + area.lt - 1;
        area.bm = v32 + area.tp - 1;
        WinAreaUpdate( gInvWin, &area );
    }
    ArtId = ItemGetArtId( a1 );
    Img = (ArtFrm_t *)ArtLoadImg(ArtId, v13);
    v30 = Img;
    if( Img ){
        ObjWidth = ArtGetObjWidth(Img, 0, 0);
        ObjHeight = ArtGetObjHeight(v15, 0, 0);
        v23 = ObjHeight / 2;
        v22 = ObjWidth / 2;
        ObjData = (char *)ArtGetObjData(v16, 0, 0);
        MseSetStaticCursor(ObjData, ObjWidth, ObjHeight, ObjWidth, v22, v23, 0);
        GSoundPlay("ipickup1");
    }
    do
        InpUpdate();
    while( (MseGetButtons() & 4) != 0 );
    if( v30 ){ ArtClose( Obj ); GSoundPlay( "iputdown" ); }
    if( v33 ) 
        if( !MseCursorCenterInArea( 377, 37, 441, 48 * gInvUnk08 + 37 ) ) goto LABEL_44;
        v18 = (Obj_t *)(Quantity <= 1 ? 1 : InvPopUpDlg(4, a1, Quantity));
        if( v18 == (Obj_t *)-1 ) goto LABEL_44;
        if( gInvUnk80 ){
            if( !SkillUseSteal( gInvSelectedDude, a3, 1, a1 ) ) v35 = 1;
        }
        if( v35 == 1 ) goto LABEL_44;
        if( Item13( gInvSelectedDude, a3, a1, v18 ) != -1 ) {
            v35 = 2;
            InvSetInfoMode( 0 );
            return v35;
        }
        msg.Id = 26;
        if( MessageGetMsg( &gInvMsg, &msg ) != 1 ) goto LABEL_44;
        goto LABEL_43;
    }
    if( !MseCursorCenterInArea(256, 37, 320, 48 * gInvUnk08 + 37) ) goto LABEL_44;
    v20 = (Obj_t *)(Quantity <= 1 ? 1 : InvPopUpDlg(4, a1, Quantity));
    if( v20 == (Obj_t *)-1 ) goto LABEL_44;
    if( gInvUnk80 && !SkillUseSteal(gInvSelectedDude, a3, 0, a1) )  v35 = 1;
    if( v35 == 1 ) goto LABEL_44;
    if( Item13(a3, gInvSelectedDude, a1, v20) ){
        msg.Id = 25;
        if( MessageGetMsg(&gInvMsg, &msg) != 1 ){
LABEL_44:
            InvSetInfoMode(0);
            return v35;
        }
LABEL_43:
        IfcMsgOut(msg.Text);
        InvSetInfoMode(0);
        return v35;
    }
    if( (a1->Flags & 0x2000000) != 0 ) a3->ImgId = ArtMakeId((a3->ImgId & 0xF000000) >> 24, a3->ImgId & 0xFFF, (a3->ImgId & 0xFF0000u) >> 16, 0, a3->Orientation + 1);
    v21 = HIBYTE(a1->Flags) & 0xF8;
    v35 = 2;
    HIBYTE(a1->Flags) = v21;
    InvSetInfoMode(0);
    return v35;
*/
}

int InvStackPrice( Obj_t *dude, Obj_t *a2 )
{
/*
    Obj_t *v3; // eax
    float v4; // edx
    Obj_t *v5; // ecx
    unsigned int v6; // edx
    double *v8; // [esp+0h] [ebp-1Ch]
    double *Total; // [esp+4h] [ebp-18h]
    float MoneyAll; // [esp+Ch] [ebp-10h]
    float v12; // [esp+10h] [ebp-Ch]
    float v13; // [esp+14h] [ebp-8h]
    float v14; // [esp+14h] [ebp-8h]
    float v15; // [esp+14h] [ebp-8h]
    float v16; // [esp+18h] [ebp-4h]

    if( gInvBarterDiscount ) return ItemGetBackPackWeight( gInvUnk65 );
    v3 = Item27( gInvUnk65 );
    v16 = 0;
    MoneyAll = (float)ItemGetMoneyAll( gInvUnk65 );
    if( (dude == gObjDude) && PerkLvl( gObjDude, PERK_MASTER_TRADER ) ) v16 = 25.0; // -25% discount
    v12 = ((double)gInvUnk66 + 100.0 - v16) * 0.01;
    if( v12 < 0.0 ) v12 = 0.01;
    return (int)lround(((((double)SkillGetTotal(a2, SKILL_BARTER)) + 160.0) / (((double )PartyGetBestSkillLvl( SKILL_BARTER )) + 160.0) * ((v3 - MoneyAll) * 2.0)) * v12 + MoneyAll);
*/
}

int InvBarterDo( Obj_t *dude, Obj_t *Item, Obj_t *a3, Obj_t *a4 )
{
/*
    MsgLine_t msg;
    int Reject;

    if( ItemGetBackPackWeight( a3 ) > (FeatGetVal( dude, FEAT_CARRY ) - ItemGetBackPackWeight( dude )) ){
	msg.Id = 31; // 'Sorry, you cannot carry that much.'
	if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) GdialogReply( msg.Text );
	return -1;    
    }
    if( gInvUnk64 ){
	if( ItemGetBackPackWeight( Item ) > (FeatGetVal( a4, 12 ) - ItemGetBackPackWeight( a4 )) ){
    	    msg.Id = 32; // 'Sorry, that's too much to carry.'
    	    if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) GdialogReply( msg.Text );
    	    return -1;
	} 
    }
    if( gInvUnk64 == 0 ){
	Reject = 0;
	if( !Item->Critter.Box.Cnt ){
    	    Reject = 1;
	} else {
	    if( Item34( Item ) && ( Item->Pid != PID_GEIGER || ItemDeviceOff( Item ) == -1 ) ) Reject = 1;
	    if( !Reject ){
    		if( InvStackPrice( dude, a4 ) > Item27( Item ) ) Reject = 1;
	    }
	}
	if( Reject ){
	    msg.Id = 28; // 'No, your offer is not good enough.'
	    if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) GdialogReply( msg.Text );
	    return -1;
	}
    }
    Item15( a3, dude );
    Item15( Item, a4 );
*/
    return 0;
}

void InvUnk41( Obj_t *dude, int quantity, int a3, int a4, Obj_t *a1, Obj_t *a2, int a7 )
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
        InvUnk03( a4, a3, &gInvUnk62->Box, 3 );
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
/*
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
        InvUnk43( gInvUnk18, a1, 0, a3 );
    } else {
        InvUnk43( gInvUnk18, 0, a1, a3 );
    }    
    if( (Img = ArtLoadImg( ItemGetArtId( Item ), ImgObj ) ) ){
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
                msg.Id = 26;
                if( MessageGetMsg( &gInvMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
            }
        }
    } else if( MseCursorCenterInArea( 475, 310, 539, 48 * gInvUnk08 + 310 ) ){
        tmp = quantity <= 1 ? 1 : InvPopUpDlg(4, Item, quantity);
        if( tmp != -1 && Item14(a1, a4, Item, tmp ) == -1 ){
            msg.Id = 25;
            if ( MessageGetMsg(&gInvMsg, &msg) == 1 ) IfcMsgOut(msg.Text);
        }
    }
    InvSetInfoMode(0);
*/
}

void InvUnk43( int win, ObjContainer_t *box, Obj_t *obj, int a4 )
{
/*
    char *surf1; // edi MAPDST
    int const_480; // edx
    char *surf2; // eax
    int v7; // edx
    char *v8; // edi
    int i; // esi
    int ArtId; // eax
    char *v11; // edx
    int v12; // ecx
    ObjStack_t *v13; // eax
    char *v14; // ebx
    int v15; // esi
    Obj_t *BackPackWeight; // eax
    Obj_t *v17; // eax
    int v19; // edi
    char *v20; // eax
    int v21; // edx
    int v22; // esi
    char *v23; // edi
    int v24; // eax
    char *v25; // edx
    int v26; // ecx
    int v27; // eax
    int v28; // esi
    int v29; // eax
    int v30; // eax
    void *v31; // [esp-4h] [ebp-B8h]
    int v32; // [esp+0h] [ebp-B4h]
    int v33; // [esp+0h] [ebp-B4h]
    char stmp[80]; // [esp+4h] [ebp-B0h] BYREF
    VidRect_t area; // [esp+54h] [ebp-60h] BYREF
    MsgLine_t msg1; // [esp+64h] [ebp-50h] BYREF
    MsgLine_t msg2; // [esp+74h] [ebp-40h] BYREF
    int v38; // [esp+84h] [ebp-30h]
    int DestPitch; // [esp+88h] [ebp-2Ch]
    int FontId; // [esp+8Ch] [ebp-28h]
    Obj_t *item; // [esp+98h] [ebp-1Ch]
    int v45; // [esp+A0h] [ebp-14h]

    item = (Obj_t *)box;
    surf1 = WinGetSurface(gInvWin);
    FontId = FontGetCurrent();
    DestPitch = const_480;
    FontSet(101);
    v45 = gFont.ChrHeight() + 48 * gInvUnk08;
    if( item ){
        surf2 = WinGetSurface( win );
        v31 = surf1 + 9769;
        v8 = surf1 + 11689;
        i = 0;
        ScrCopy(&surf2[v7 + 249], 64, v45 + 1, gVidMainGeo.rt - gVidMainGeo.lt + 1, v31, 480);
        while( i + gInvUnk78 < item->Feat.Critter.Box.Cnt && i < gInvUnk08 ){
            ArtId = ItemGetArtId(item->Feat.Critter.Box.Box[item->Feat.Critter.Box.Cnt - (i + gInvUnk78 + 1)].obj);
            ArtLoadImgScaled(ArtId, v11, 56, v12, 480);
            v32 = i == a4;
            v13 = &item->Feat.Critter.Box.Box[item->Feat.Critter.Box.Cnt - (i + gInvUnk78 + 1)];
            v14 = v8;
            v8 += 23040;
            ++i;
            InvPrintQuantity(v13->obj, v13->Quantity, v14, 480, v32);
        }
        v15 = DestPitch * (48 * gInvUnk08 + 24) + 169;
        if( gInvUnk64 ){
            msg1.Id = 30;
            if( MessageGetMsg(&gInvMsg, &msg1) == 1 ){
                BackPackWeight = ItemGetBackPackWeight(item);
                sprintf(stmp, "%s %d", msg1.Text, BackPackWeight);
            }
        } else {
            sprintf(stmp, "$%d", Item27(item));
        }
        gFont.Print(&surf1[v15], stmp, 80, DestPitch, gPalColorCubeRGB[31][31][31]);
        area.lt = 169;
        area.tp = 24;
        area.bm = v45 + 24;
        area.rt = 233;
        WinAreaUpdate(gInvWin, &area);
    }
    if( obj ){
        v19 = 4 * (gVidMainGeo.rt - gVidMainGeo.lt + 1);
        v20 = WinGetSurface(win);
        ScrCopy(&v20[4 * v21 + 334 + 4 * v19], 64, v45 + 1, gVidMainGeo.rt - gVidMainGeo.lt + 1, &surf1[20 * DestPitch + 254], DestPitch);
        v38 = 48 * DestPitch;
        v22 = 0;
        v23 = &surf1[24 * DestPitch + 254];
        while( v22 + gInvUnk77 < obj->Feat.Critter.Box.Cnt && v22 < gInvUnk08 ){
            v33 = DestPitch;
            v24 = ItemGetArtId(obj->Feat.Critter.Box.Box[obj->Feat.Critter.Box.Cnt - (v22 + gInvUnk77 + 1)].obj);
            ArtLoadImgScaled(v24, v25, 56, v26, v33);
            v27 = (int)&obj->Feat.Critter.Box.Box[obj->Feat.Critter.Box.Cnt - (v22 + gInvUnk77 + 1)];
            InvPrintQuantity(*(Obj_t **)v27, *(_DWORD *)(v27 + 4), v23, DestPitch, v22++ == a4);
            v23 += v38;
        }
        v28 = DestPitch * (48 * gInvUnk08 + 24) + 254;
        if( gInvUnk64 ){
            msg2.Id = 30;
            if ( MessageGetMsg( &gInvMsg, &msg2 ) == 1 ){
                sprintf( stmp, "%s %d", msg2.Text, InvStackPrice(gObjDude, gInvUnk39[0] ) );
            }
        } else {
            sprintf( stmp, "$%d", InvStackPrice( gObjDude, gInvUnk39[0] ) );
        }
        gFont.Print(&surf1[v28], stmp, 80, DestPitch, (unsigned __int8)gPalColorCubeRGB[31][31][31]);
        area.lt = 254;
        area.tp = 24;
        area.bm = v45 + 24;
        area.rt = 318;
        WinAreaUpdate(gInvWin, &area);
    }
    FontSet(FontId);
*/
}

int InvMenuBarter(int eax0, Obj_t *a2, Obj_t *a3, Obj_t *a4, int a5 )
{
/*
    int sel; // esi MAPDST
    int rr; // eax
    Obj_t *ArmorObj; // eax
    Obj_t *RHandObj; // eax
    int v11; // ecx
    int Cnt; // ebx
    int v13; // ecx
    int v14; // eax
    unsigned int v15; // eax
    int v17; // ecx
    int v18; // ecx
    int v19; // edx
    int v20; // ecx
    int v21; // edx
    int v22; // ecx
    int v23; // ecx
    ObjContainer_t *v24; // edx
    Window01_t *v25; // eax
    int v26; // esi
    int v27; // ecx
    ObjStack_t *v28; // eax
    int v29; // ecx
    int v30; // esi
    int v31; // ecx
    ObjStack_t *v32; // eax
    int v33; // ecx
    ObjContainer_t *v34; // edx
    Window01_t *v35; // eax
    int v36; // esi
    ObjStack_t *v37; // eax
    int v38; // ecx
    int v39; // esi
    ObjStack_t *v40; // eax
    Obj_t *v44; // eax
    int i; // esi
    CachePool_t *v46; // eax
    int v47; // edx
    MsgLine_t msg; // [esp+0h] [ebp-40h] BYREF
    int v49; // [esp+10h] [ebp-30h] BYREF
    int v50; // [esp+14h] [ebp-2Ch]
    Obj_t *v51; // [esp+18h] [ebp-28h]
    Obj_t *v52; // [esp+1Ch] [ebp-24h] MAPDST
    Obj_t *v53; // [esp+20h] [ebp-20h] MAPDST
    int v54; // [esp+24h] [ebp-1Ch]
    int v55; // [esp+28h] [ebp-18h]
    Obj_t *obj; // [esp+2Ch] [ebp-14h] MAPDST
    Obj_t *a1; // [esp+30h] [ebp-10h]

    obj = a2;
    a1 = a3;
    sel = -1;
    v51 = 0;
    v49 = 0;
    gInvUnk66 = a5;
    rr = InvInit();
    if ( rr != -1 )
    {
        ArmorObj = InvGetArmorObj(obj);
        v53 = ArmorObj;
        if ( ArmorObj )
            ItemUseItem(obj, ArmorObj, 1);
        RHandObj = InvGetRHandObj(obj);
        v52 = RHandObj;
        if ( RHandObj )
            goto LABEL_7;
        if ( !gInvUnk64 )
        {
            RHandObj = InvSearchObjByType(obj, 3, 0);
            v51 = RHandObj;
            if ( RHandObj )
LABEL_7:
                ItemUseItem(obj, RHandObj, 1);
        }
        rr = ObjCreate((Obj_t **)&v49, 0, 467);
        if ( rr != -1 )
        {
            Item16(obj, (Obj_t *)v49);
            gInvBackPack = (ObjContainer_t *)&gInvSelectedDude->Feat;
            gInvUnk65 = a1;
            gInvUnk84 = a4;
            gInvUnk78 = 0;
            gInvUnk77 = 0;
            gInvUnk79 = (ObjContainer_t *)((char *)a4 + 44);
            gInvUnk85 = (ObjContainer_t *)&a1->Feat;
            gInvUnk18 = (int)eax0;
            gInvUnk40 = 0;
            gInvUnk62 = (ObjContainer_t *)&obj->Feat;
            gInvUnk39[0] = obj;
            gInvUnk63[0] = 0;
            v50 = InvMenuCreate(3u);
            InvUnk03(gInvUnk63[gInvUnk40], -1, &gInvUnk62->Box, v11);
            InvBpUpdate(gInvUnk04[0], -1, 3);
            InvBodyUpdate(obj->ImgId, 3);
            Cnt = (int)a1;
            WinUpdate(gInvUnk18);
            InvUnk43((int)eax0, a4, (Obj_t *)Cnt, v13);
            InvSetInfoMode(0);
            v14 = EvQeUnk23(obj);
            v15 = EvQeUnk20(v14);
            if ( v15 )
            {
                if ( v15 <= 1 )
                {
                    v55 = 0;
                }
                else if ( v15 == 2 )
                {
                    v55 = -15;
                }
            }
            else
            {
                v55 = 25;
            }
            v54 = a5 + v55;
            while ( 1 )
            {
                while ( 1 )
                {
                    while ( 1 )
                    {
                        do
                        {
                            if ( sel != 27 && !gMenuEscape )
                            {
                                sel = InpUpdate();
                                if ( sel == 17 || sel == 24 || sel == 324 )
                                    SysQuitDlg();
                                if ( !gMenuEscape )
                                    continue;
                            }
LABEL_108:
                            Item15((Obj_t *)v49, obj);
                            ObjDestroy((Obj_t *)v49, 0, Cnt);
                            if ( v53 )
                            {
                                HIBYTE(v53->Flags) |= 4u;
                                ItemAdd(obj, v53, 1);
                            }
                            if ( v52 )
                            {
                                v44 = obj;
                                HIBYTE(v52->Flags) |= 2u;
                                ItemAdd(v44, v52, 1);
                            }
                            if ( v51 )
                                ItemAdd(obj, v51, 1);
                            InvMenuClose(v50);
                            for ( i = 0;
                                  i != 5;
                                  ++i )
                            {
                                v46 = gInvMseCursor[i].Obj;
                                ArtClose(v46);
                            }
                            if ( gInvUnk02 )
                                GameIfaceDisable();
                            rr = InvMsgClose();
                            gInvArt[0].Xpos = v47;
                            return rr;
                        }
                        while ( sel == -1 );
                        gInvUnk66 = v54;
                        if ( sel == 116 || v55 <= -30 )
                        {
                            Item15(a1, obj);
                            Item15((Obj_t *)a4, gObjDude);
                            GdialogUnk59((int)eax0, (int)a4, sel);
                            goto LABEL_108;
                        }
                        if ( (unsigned int)sel >= 337 )
                            break;
                        if ( (unsigned int)sel < 328 )
                        {
                            if ( sel != 109 )
                                goto LABEL_71;
                            if ( a4[1].Box.Capacity || gInvUnk65->Feat.Critter.Box.Cnt )
                            {
                                Cnt = (int)obj;
                                if ( !InvBarterDo(gInvSelectedDude, (Obj_t *)a4, a1, obj) )
                                {
                                    InvUnk03(gInvUnk63[gInvUnk40], -1, &gInvUnk62->Box, 3);
                                    InvBpUpdate(gInvUnk04[gInvUnk05], -1, 3);
                                    InvUnk43((int)eax0, a4, a1, v22);
                                    Cnt = 27;
                                    msg.Id = 27;// 'OK, That's good trade.'
                                    if ( !gInvUnk64 && MessageGetMsg(&gInvMsg, &msg) == 1 )
                                        GdialogReply(msg.Text);
                                }
                            }
                        }
                        else if ( (unsigned int)sel <= 328 )
                        {
                            v17 = gInvUnk04[gInvUnk05];
                            if ( v17 > 0 )
                            {
                                Cnt = 3;
                                v18 = v17 - 1;
                                gInvUnk04[gInvUnk05] = v18;
                                InvBpUpdate(v18, -1, 3);
                            }
                        }
                        else if ( (unsigned int)sel <= 329 )
                        {
                            if ( gInvUnk78 > 0 )
                            {
                                Cnt = (int)a1;
                                --gInvUnk78;
                                InvUnk43((int)eax0, a4, a1, -1);
                            }
                        }
                        else
                        {
                            if ( sel != 336 )
                                goto LABEL_71;
                            Cnt = gInvUnk04[gInvUnk05] + gInvUnk08;
                            if ( Cnt < gInvBackPack->Box.Cnt )
                            {
                                v19 = gInvUnk04[gInvUnk05] + 1;
                                Cnt = 3;
                                gInvUnk04[gInvUnk05] = v19;
                                InvBpUpdate(v19, -1, 3);
                            }
                        }
                    }
                    if ( (unsigned int)sel > 337 )
                        break;
                    if ( gInvUnk78 + gInvUnk08 < gInvUnk79->Box.Cnt )
                    {
                        Cnt = (int)a1;
                        ++gInvUnk78;
                        InvUnk43((int)eax0, a4, a1, -1);
                    }
                }
                if ( (unsigned int)sel < 397 )
                {
                    if ( (unsigned int)sel < 374 )
                        goto LABEL_71;
                    if ( (unsigned int)sel <= 374 )
                    {
                        Cnt = gInvUnk08;
                        if ( gInvUnk08 + gInvUnk77 < gInvUnk85->Box.Cnt )
                        {
                            Cnt = (int)a1;
                            ++gInvUnk77;
                            InvUnk43((int)eax0, a4, a1, -1);
                        }
                    }
                    else
                    {
                        if ( sel != 388 )
                            goto LABEL_71;
                        if ( gInvUnk77 > 0 )
                        {
                            Cnt = (int)a1;
                            --gInvUnk77;
                            InvUnk43((int)eax0, a4, a1, -1);
                        }
                    }
                }
                else if ( (unsigned int)sel <= 397 )
                {
                    if ( gInvUnk63[gInvUnk40] > 0 )
                    {
                        v20 = gInvUnk63[gInvUnk40] - 1;
                        Cnt = (int)gInvUnk62;
                        gInvUnk63[gInvUnk40] = v20;
                        InvUnk03(v20, -1, (ObjBox_t *)Cnt, 3);
                        WinUpdate(gInvWin);
                    }
                }
                else if ( (unsigned int)sel < 401 )
                {
LABEL_71:
                    if ( (MseGetButtons() & 2) != 0 )
                    {
                        if ( gInvInfoMode )
                            InvSetInfoMode(0);
                        else
                            InvSetInfoMode(1);
                    }
                    else if ( (MseGetButtons() & 1) != 0 )
                    {
                        if ( sel >= 1000 && sel < gInvUnk08 + 1000 )
                        {
                            if ( gInvInfoMode == 1 )
                            {
                                InvActionMenu(sel, 3);
                                Cnt = 0;
                                v24 = a4;
                                v25 = eax0;
                                goto LABEL_82;
                            }
                            v26 = sel - 1000;
                            Cnt = gInvBackPack->Box.Cnt;
                            if ( v26 + gInvUnk04[gInvUnk05] < gInvBackPack->Box.Cnt )
                            {
                                v27 = gInvUnk04[gInvUnk05];
                                v28 = &gInvBackPack->Box.Box[Cnt - (v26 + v27 + 1)];
                                InvUnk41(v28->obj, v28->Quantity, v26, v27, obj, (Obj_t *)a4, 1);
                                InvUnk03(gInvUnk63[gInvUnk40], -1, &gInvUnk62->Box, 3);
                                InvBpUpdate(gInvUnk04[gInvUnk05], -1, 3);
                                v24 = a4;
                                v25 = eax0;
                                Cnt = 0;
LABEL_82:
                                InvUnk43((int)v25, v24, 0, v23);
                            }
                            sel = -1;
                        }
                        if ( sel >= 2000 && sel < gInvUnk08 + 2000 )
                        {
                            if ( gInvInfoMode == 1 )
                            {
                                Cnt = (int)a1;
                                InvActionMenu(sel, 3);
                                goto LABEL_90;
                            }
                            v30 = sel - 2000;
                            Cnt = gInvUnk62->Box.Cnt;
                            if ( v30 + gInvUnk63[gInvUnk40] < gInvUnk62->Box.Cnt )
                            {
                                v31 = gInvUnk63[gInvUnk40];
                                v32 = &gInvUnk62->Box.Box[gInvUnk62->Box.Cnt - (v30 + v31 + 1)];
                                InvUnk41(v32->obj, v32->Quantity, v30, v31, obj, a1, 0);
                                InvUnk03(gInvUnk63[gInvUnk40], -1, &gInvUnk62->Box, 3);
                                InvBpUpdate(gInvUnk04[gInvUnk05], -1, 3);
                                Cnt = (int)a1;
LABEL_90:
                                InvUnk43((int)eax0, 0, (Obj_t *)Cnt, v29);
                            }
                            sel = -1;
                        }
                        if ( sel < 2300 || sel >= gInvUnk08 + 2300 )
                            goto LABEL_100;
                        if ( gInvInfoMode == 1 )
                        {
                            InvActionMenu(sel, 3);
                            Cnt = 0;
                            v34 = a4;
                            v35 = eax0;
LABEL_98:
                            InvUnk43((int)v35, v34, 0, v33);
                            goto LABEL_99;
                        }
                        v36 = sel - 2300;
                        if ( v36 < gInvUnk79->Box.Cnt )
                        {
                            v37 = &gInvUnk79->Box.Box[gInvUnk79->Box.Cnt - (v36 + gInvUnk78 + 1)];
                            InvUnk42(v37->obj, v37->Quantity, v36, obj, (Obj_t *)a4, 1);
                            InvUnk03(gInvUnk63[gInvUnk40], -1, &gInvUnk62->Box, 3);
                            InvBpUpdate(gInvUnk04[gInvUnk05], -1, 3);
                            v34 = a4;
                            v35 = eax0;
                            Cnt = 0;
                            goto LABEL_98;
                        }
LABEL_99:
                        sel = -1;
LABEL_100:
                        if ( sel >= 2400 && sel < gInvUnk08 + 2400 )
                        {
                            if ( gInvInfoMode == 1 )
                            {
                                Cnt = (int)a1;
                                InvActionMenu(sel, 3);
                            }
                            else
                            {
                                v39 = sel - 2400;
                                if ( v39 >= gInvUnk85->Box.Cnt )
                                    goto LABEL_107;
                                v40 = &gInvUnk85->Box.Box[gInvUnk85->Box.Cnt - (v39 + gInvUnk77 + 1)];
                                InvUnk42(v40->obj, v40->Quantity, v39, obj, a1, 0);
                                InvUnk03(gInvUnk63[gInvUnk40], -1, &gInvUnk62->Box, 3);
                                InvBpUpdate(gInvUnk04[gInvUnk05], -1, 3);
                                Cnt = (int)a1;
                            }
                            InvUnk43((int)eax0, 0, (Obj_t *)Cnt, v38);
LABEL_107:
                            sel = -1;
                        }
                    }
                }
                else if ( (unsigned int)sel <= 401 )
                {
                    Cnt = (int)gInvUnk62;
                    if ( gInvUnk08 + gInvUnk63[gInvUnk40] < gInvUnk62->Box.Cnt )
                    {
                        v21 = gInvUnk63[gInvUnk40] + 1;
                        gInvUnk63[gInvUnk40] = v21;
                        InvUnk03(v21, -1, (ObjBox_t *)Cnt, 3);
                        WinUpdate(gInvWin);
                    }
                }
                else
                {
                    if ( (unsigned int)sel < 2500 || (unsigned int)sel > 2501 )
                        goto LABEL_71;
                    InvUnk46(sel, 3);
                }
            }
        }
    }
    return rr;
*/
}

void InvUnk45( int a1, int *a2 )
{
/*
    int v3; // ecx
    int v4; // ecx
    int v5; // ebp
    Obj_t *v6; // eax
    int v7; // edx
    int v8; // ecx
    int v9; // ecx
    int v10; // ebp
    Obj_t *v11; // eax
    int v12; // ecx

    if( a1 >= 2000 ){
        v8 = gInvUnk62->Box.Cnt - (gInvUnk63[gInvUnk40] + a1 - 2000 + 1);
        if( v8 < gInvUnk62->Box.Cnt && gInvUnk40 < 9 && ItemGetObjType(gInvUnk62->Box.Box[v8].obj) == 1 ){
            v10 = gInvUnk40 + 1;
            gInvUnk39[v10] = *(Obj_t **)((char *)&gInvUnk62->Box.Box->obj + v9);
            v11 = gInvUnk39[v10];
            gInvUnk63[v10] = 0;
            gInvUnk40 = v10;
            gInvUnk62 = (ObjContainer_t *)&v11->Feat;
            InvBodyUpdate(v11->ImgId, (int)a2);
            InvUnk03(gInvUnk63[gInvUnk40], -1, &gInvUnk62->Box, v12);
            WinUpdate(gInvWin);
        }
    } else {
        v3 = gInvBackPack->Box.Cnt - (gInvUnk04[gInvUnk05] + a1 - 1000 + 1);
        if( v3 < gInvBackPack->Box.Cnt && gInvUnk05 < 9 && ItemGetObjType(gInvBackPack->Box.Box[v3].obj) == 1 ){
            v5 = gInvUnk05 + 1;
            gInvParty[v5] = *(Obj_t **)((char *)&gInvBackPack->Box.Box->obj + v4);
            gInvUnk05 = v5;
            gInvSelectedDude = gInvParty[v5];
            v6 = gInvSelectedDude;
            gInvUnk04[v5] = 0;
            gInvBackPack = (ObjContainer_t *)&v6->Feat;
            InvSetBodyImg();
            InvBodyUpdate(-1, v7);
            InvBpUpdate(gInvUnk04[gInvUnk05], -1, (int)a2);
        }
    }
*/
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
//        InvUnk03( gInvUnk63[ gInvUnk40 ], -1, &gInvUnk62->Box, mode );
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
            UseUnk06( &item1->TimeEv );
            v16 = 1;
            if( !InvPickItem( sel, &item1, 0, 0 ) ) break;
        }
        if( err != -1 ) v16 = 1;
        if( err ) break;
    }
    if( n != -1 ) ItemAdd( gInvSelectedDude, item0, 1 );
    if( !v16 ) return -1;
    GSoundPlay( GSoundProtoFname3( 0, item0, 2, 0 ) );
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
        if( sel == KEY_ESC ){ InvPopUpClose( mode ); return -1; }
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
DD
/*
    Obj_t *v1; // edx
    int result; // eax
    int v3; // ebx
    int v4; // ecx
    int i; // edx
    int v6; // ecx

    v1 = a1;
    if ( gInvArt[0].Xpos )
        goto LABEL_3;
    result = InvInit();
    if( result != -1 ){
LABEL_3:
        v3 = InvPopUpDlg(5, v1, 180);
        if( !v4 ){
            for ( i = 0; i != 140;  ) ArtClose(*(CachePool_t **)((char *)&gInvMseCursor[0].Obj + i));
            if( gInvUnk02 ) GameIfaceDisable();
            InvMsgClose();
            gInvArt[0].Xpos = v6;
        }
        return v3;
    }
    return result;
*/
}

