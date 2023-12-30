#include "FrameWork.h"

char *ObjGetName( Obj_t * );

Premade_t gPremadeCharacter[ 3 ] = {
    { "premade/combat",   0x0C9, "VID 208-197-88-125" },
    { "premade/stealth",  0x0CA, "VID 208-206-49-229" },
    { "premade/diplomat", 0x0CB, "VID 208-206-49-227" },
};

#define NG_FEAT_A( s, x, y, y_ofs, wdt, Id )	\
    y += y_ofs;\
    sprintf( s, "%s %02d", FeatGetName( Id ), Val = FeatGetVal( gObjDude, Id ) );\
    w = gFont.LineWidth( s );\
    gFont.Print( gNgSurface + wdt * y - w + x, s, w, wdt, NG_CAPCOLOR );\
    sprintf( s, "  %s", FeatGetComments( Val ) );\
    gFont.Print( gNgSurface + wdt * y + x, s, gFont.LineWidth( s ), wdt, NG_CAPCOLOR );

#define NG_FEAT_B( s, x, y, y_ofs, wdt, fmt, args... )	\
    y += y_ofs;\
    w = gFont.LineWidth( str );\
    gFont.Print( gNgSurface + wdt * y - w + x, str, w, wdt, NG_CAPCOLOR );\
    sprintf( str, fmt, ##args );\
    gFont.Print( gNgSurface + wdt * y + x, str, gFont.LineWidth( str ), wdt, NG_CAPCOLOR );

char *gNgImg[12];

int  gNgWin = -1;
char *gNgSurface = NULL;
void  *gNgUnk01 = NULL;
int  gNgBt1 = -1;
CachePool_t *gNgObjImg1 = NULL;
CachePool_t *gNgObjImg2 = NULL;
int  gNgBt2 = -1;
CachePool_t *gNgObj1 = NULL;
CachePool_t *gNgObj2 = NULL;
int  gNgBt6 = -1;
CachePool_t *gNgObj3 = NULL;
CachePool_t *gNgObj4 = NULL;
int  gNgBt3 = -1;
CachePool_t *gNgObj5 = NULL;
CachePool_t *gNgObj6 = NULL;
int  gNgBt4 = -1;
CachePool_t *gNgObj7 = NULL;
CachePool_t *gNgObj8 = NULL;
int  gNgBt5 = -1;
CachePool_t *gNgObj9 = NULL;
CachePool_t *gNgObj10= NULL;

int gNgDudeSel = 0;
int gNgDudePremadesCount = NG_DUDE_PREMADES;

int NgNewGame()
{
    int a, v1, sel;
    int b, c;

    a = 0;
    v1 = 0;
    b = 0;
    if( NgCharMenuCreate() != 1 ) return 0;    
    if( MseIsCursorClear() ) MseDrawCursor();
    PalLoadFromFile( "color.pal" );
    FadeStep( &gPalBase );
    if( !v1 ){
        do{
            if( gMenuEscape ) break;
            sel = InpUpdate();
            if( sel < 'T' ){
        	if( sel >= '-' ){
        	    if( sel <= '-' ){ OptBrightDec(); continue; }
        	    if( sel < 'B' ){ if( sel == '=' ) OptBrightInc(); continue; }
        	    if( sel <= 'B' ){
            		a = 3;            	    
            		b = 1;
            		continue;
        	    }
        	    if( sel <= 'C' ){
            		ProtoReset( a );
            		if( !CharEditMenu( 1 ) ){ a = 2; b = 1; continue; }
        		NgDudeInit();
        		continue;
        	    }
        	    if( sel == 'M' ){
        		if( !CharEditMenu( 1 ) ){ a = 2; b = 1; continue; }
        		NgDudeInit();
        	    }
        	    continue;
        	}
        	if( sel < 24 ){
        	    if( sel == 17 ) SysQuitDlg();
        	    continue;
        	}
        	if( sel == 24 ){ SysQuitDlg(); continue; }
        	if( sel >= KEY_ESC ){
            	    if( sel > KEY_ESC ){
            		if( sel == '+' ) OptBrightInc();
        	    } else {
            		a = 3;
            		b = 1;
            	    }
        	}
    	    } else {
        	if( sel == 0x54 ){ a = 2; b = 1; continue; }
        	if( sel < 0x74 ){
            	    if( sel < 'b' ){ if( sel == '_' ) OptBrightDec(); continue; }
            	    if( sel <= 'b' ){
            		a = 3;
            		b = 1;
            		continue;
            	    }
            	    if( sel > 'c' ){
            		if( sel == 'm' ){ // modify character
        		    if( !CharEditMenu(1) ){ a = 2; b = 1; continue; }
        		    NgDudeInit();
            		}
            	    } else { // == 99 -> create new
            		ProtoReset();
            		if( !CharEditMenu(1) ){ a = 2; b = 1; continue; }
        		NgDudeInit();
        	    }
        	    continue;
        	}
        	if( sel <= 't' ){ a = 2; b = 1; continue; }
        	if( sel < 333 ){
            	    if( sel >= 324 ){
                	if( sel <= 324 ){ SysQuitDlg(); continue; }
                	if( sel != 331 ) continue;
                	GSoundPlay("ib2p1xx1");
                	if( --gNgDudeSel < 0 ) gNgDudeSel = gNgDudePremadesCount - 1;
        		NgDudeInit();
        		continue;
            	    }
        	} else {
            	    if( sel <= 333 ){
                	GSoundPlay("ib2p1xx1");
                	if( ++gNgDudeSel >= gNgDudePremadesCount ) gNgDudeSel = 0;
        		NgDudeInit();
        		continue;
            	    }
            	    if( sel >= 500 ){
            		if( sel <= 500 ){
                    	    if( --gNgDudeSel < 0 ) gNgDudeSel = gNgDudePremadesCount - 1;
                	} else {
                    	    if( sel != 501 ) continue;
                    	    if( ++gNgDudeSel >= gNgDudePremadesCount ) gNgDudeSel = 0;
                	}
        		NgDudeInit();
            	    }
        	}
    	    }
	} while ( !b );
    }
    FadeStep( gFadePaletteC );
    NgClose();
    if( !c ) MseCursorRedraw();    
    return a;
}

int NgCharMenuCreate()
{
    ArtFrm_t *img;
    CachePool_t *ArtIdx;

    if( gNgWin != -1 ) return 0;
    if( (gNgWin = WinCreateWindow(0, 0, 640, 480, gPalColorCubeRGB[0][0][0], 0)) == -1 ){ NgClose(); return 0; }
    if( !(gNgSurface = WinGetSurface( gNgWin )) ){ NgClose(); return 0; }

    if( !(img = ArtGetBitmap( ArtMakeId( NG_WALLPAPER ), 0, 0, &ArtIdx)) ){ NgClose(); return 0; }
    ScrCopy( img, 640, 480, 640, gNgSurface, 640 );
    if( !(gNgUnk01 = Malloc( 560 * 300 ))){ NgClose(); return 0; }
    ScrCopy( &img->Data[ 19228 ], 560, 300, 640, gNgUnk01, 560 );  
    ArtClose( ArtIdx );

    // arrow left
    if( !(gNgImg[10] = ArtGetBitmap( ArtMakeId( NG_IMG_BT1A ), 0, 0, &gNgObjImg1)) ){ NgClose(); return 0; }    
    if( !(gNgImg[11] = ArtGetBitmap( ArtMakeId( NG_IMG_BT1B ), 0, 0, &gNgObjImg2)) ){ NgClose(); return 0; }
    if( (gNgBt1 = WinCreateButton( gNgWin, 292, 320, 20, 18, -1, -1, -1, 500, gNgImg[10], gNgImg[11], 0, 0)) == -1 ){ NgClose(); return 0; }
    WinSetClickSound( gNgBt1, GSoundPlayCheck, GSoundPlayUnCheck );

    // arrow right
    if( !(gNgImg[9] = ArtGetBitmap( ArtMakeId( NG_IMG_BT2A ), 0, 0, &gNgObj1)) ){ NgClose(); return 0; }
    if( !(gNgImg[8] = ArtGetBitmap( ArtMakeId( NG_IMG_BT2B ), 0, 0, &gNgObj2)) ){ NgClose(); return 0; }
    if( (gNgBt2 = WinCreateButton( gNgWin, 318, 320, 20, 18, -1, -1, -1, 501, gNgImg[ 9 ], gNgImg[ 8 ], 0, 0 )) == -1 ){ NgClose(); return 0; }
    WinSetClickSound( gNgBt2, GSoundPlayCheck, GSoundPlayUnCheck );

    // take
    if( !(gNgImg[0] = ArtGetBitmap( ArtMakeId( NG_IMG_BT3A ), 0, 0, &gNgObj3)) ){ NgClose(); return 0; }
    if( !(gNgImg[7] = ArtGetBitmap( ArtMakeId( NG_IMG_BT3B ), 0, 0, &gNgObj4)) ){ NgClose(); return 0; }
    if( (gNgBt6 = WinCreateButton( gNgWin, 81, 323, 15, 16, -1, -1, -1, 116, gNgImg[0], gNgImg[7], 0, 32 ) ) == -1 ){ NgClose(); return 0; }
    WinSetClickSound( gNgBt6, GSoundPlayPushBt, GSoundPlayReleaseBt );

    // modify
    if( !(gNgImg[4] = ArtGetBitmap( ArtMakeId( NG_IMG_BT4A ), 0, 0, &gNgObj5)) ){ NgClose(); return 0; }
    if( !(gNgImg[1] = ArtGetBitmap( ArtMakeId( NG_IMG_BT4B ), 0, 0, &gNgObj6)) ){ NgClose(); return 0; }
    if( (gNgBt3 = WinCreateButton( gNgWin, 435, 320, 15, 16, -1, -1, -1, 109, gNgImg[4], gNgImg[1], 0, 32 ) ) == -1 ){ NgClose(); return 0; }
    WinSetClickSound( gNgBt3, GSoundPlayPushBt, GSoundPlayReleaseBt );

    // create character
    if( !(gNgImg[3] = ArtGetBitmap( ArtMakeId( NG_IMG_BT5A ), 0, 0, &gNgObj7)) ){ NgClose(); return 0; }
    if( !(gNgImg[6] = ArtGetBitmap( ArtMakeId( NG_IMG_BT5B ), 0, 0, &gNgObj8)) ){ NgClose(); return 0; }
    if( (gNgBt4 = WinCreateButton( gNgWin, 80, 425, 15, 16, -1, -1, -1, 99, gNgImg[3], gNgImg[6], 0, 32)) == -1 ){ NgClose(); return 0; }
    WinSetClickSound( gNgBt4, GSoundPlayPushBt, GSoundPlayReleaseBt );

    // back
    if( !(gNgImg[2] = ArtGetBitmap( ArtMakeId( NG_IMG_BT6A ), 0, 0, &gNgObj9)) ){ NgClose(); return 0; }
    if( !(gNgImg[5] = ArtGetBitmap( ArtMakeId( NG_IMG_BT6B ), 0, 0, &gNgObj10)) ){ NgClose(); return 0; }
    if( (gNgBt5 = WinCreateButton( gNgWin, 461, 425, 15, 16, -1, -1, -1, 27, gNgImg[2], gNgImg[5], 0, 32)) == -1 ){ NgClose(); return 0; }
    WinSetClickSound( gNgBt5, GSoundPlayPushBt, GSoundPlayReleaseBt );

    gNgDudeSel = 0;
    WinUpdate( gNgWin );
    if( !NgDudeInit() ){ NgClose(); return 0; }
    return 1;
}

void NgClose()
{
    if( gNgWin == -1 ) return;    
    if( gNgBt1 != -1 ){ WinImageRemove(gNgBt1); gNgBt1 = 0; }
    if( gNgImg[11] ){ ArtClose(gNgObjImg2); gNgObjImg2 = NULL; gNgImg[11] = NULL; }
    if( gNgImg[10] ){ ArtClose(gNgObjImg1); gNgObjImg1 = NULL; gNgImg[10] = NULL; }
    if( gNgBt2 != -1 ){ WinImageRemove(gNgBt2); gNgBt2 = -1; }
    if( gNgImg[8] ){ ArtClose(gNgObj2); gNgObj2 = NULL; gNgImg[8] = NULL; }
    if( gNgImg[9] ){ ArtClose(gNgObj1); gNgObj1 = NULL; gNgImg[9] = NULL; }
    if( gNgBt6 != -1 ){ WinImageRemove(gNgBt6); gNgBt6 = -1; }
    if( gNgImg[7] ){ ArtClose(gNgObj4); gNgObj4 = NULL; gNgImg[7] = NULL; }
    if( gNgImg[0] ){ ArtClose(gNgObj3); gNgObj3 = NULL; gNgImg[0] = NULL; }
    if( gNgBt3 != -1 ){ WinImageRemove(gNgBt3); gNgBt3 = -1; }
    if( gNgImg[1] ){ ArtClose(gNgObj6); gNgObj6 = NULL; gNgImg[1] = NULL; }
    if( gNgImg[4] ){ ArtClose(gNgObj5); gNgObj5 = NULL; gNgImg[4] = NULL; }
    if( gNgBt4 != -1 ){ WinImageRemove(gNgBt4); gNgBt4 = -1; }
    if( gNgImg[6] ){ ArtClose(gNgObj8); gNgObj8 = NULL; gNgImg[6] = NULL; }
    if( gNgImg[3] ){ ArtClose(gNgObj7); gNgObj7 = NULL; gNgImg[3] = NULL; }
    if( gNgBt5 != -1 ){ WinImageRemove(gNgBt5); gNgBt5 = -1; }
    if( gNgImg[5] ){ ArtClose(gNgObj10); gNgObj10 = NULL; gNgImg[5] = NULL; }
    if( gNgImg[2] ){ ArtClose(gNgObj9); gNgObj9 = NULL; gNgImg[2] = NULL; }
    if( gNgUnk01 ){ Free(gNgUnk01); gNgUnk01 = NULL; }
    if( gNgWin != -1 ){ WinClose(gNgWin); gNgWin = -1; }    
}

int NgDudeInit()
{
    int err = 0;
    Premade_t tmp;

    tmp.label[16] = 0;
    sprintf( tmp.path, "%s.gcd", gPremadeCharacter[ gNgDudeSel ].path );
    if( ProtoDudeInit( tmp.path ) == -1 ){
        eprintf( "\n ** Error in dude init! **\n" );
    } else {
        ScrCopy( gNgUnk01, 560, 300, 560, gNgSurface + 30*640 + 40, 640 );
        if( (NgLoadDudeShot() == 1) && ( NgFeats() == 1 )) err = NgCaption();        
        WinUpdate( gNgWin );
    }
    return err;
}

int NgLoadDudeShot()
{
    int err, Width, Height;
    ArtFrm_t *Img;
    char *pix;
    CachePool_t *ImgObj;

    err = 0;
    if( !(Img = ArtLoadImg( ArtMakeId( 6, gPremadeCharacter[ gNgDudeSel ].ImgId, 0, 0, 0 ), &ImgObj )) ) return 0;
    if( (pix = ArtGetObjData( Img, 0, 0 ) ) ){
        Width = ArtGetObjWidth( Img, 0, 0 );
        Height = ArtGetObjHeight( Img, 0, 0 );
        err = 1;
        ScrCopyAlpha( pix, Width, Height, Width, gNgSurface + 14747, 640);
    }
    ArtClose( ImgObj );    
    return err;
}

int NgFeats()
{
    int y, w, Val, FontId, hspace, Combat[3], Special1, Special2;
    char str[ 80 ];
    MsgLine_t msglist;

    FontId = FontGetCurrent();
    FontSet( 101 );
    hspace = gFont.ChrHeight();
    strcpy( str, ObjGetName( gObjDude ) );
    y = hspace + 40;
    gFont.Print( gNgSurface - gFont.LineWidth( str ) / 2 + 40*640 + 318, str, 160, 640, NG_CAPCOLOR );

    NG_FEAT_A( str, 362, y, 2*hspace + 6, 640, 0 );
    NG_FEAT_A( str, 362, y, hspace, 640, 1 );
    NG_FEAT_A( str, 362, y, hspace, 640, 2 );
    NG_FEAT_A( str, 362, y, hspace, 640, 3 );
    NG_FEAT_A( str, 362, y, hspace, 640, 4 );
    NG_FEAT_A( str, 362, y, hspace, 640, 5 );
    NG_FEAT_A( str, 362, y, hspace, 640, 6 );

    msglist.Id = 16; 
    if( MessageGetMsg( &gMessage, &msglist ) == 1 ) strcpy( str, msglist.Text );

    // hit points
    NG_FEAT_B( str, 379, y, 2*hspace, 640, " %d/%d", CritterGetHp( gObjDude ), FeatGetVal( gObjDude, FEAT_HP ) );

    // armor class
    strcpy( str, FeatGetName( 9 ) );
    NG_FEAT_B( str, 379, y, hspace, 640, " %d", FeatGetVal( gObjDude, 9 ) );

    // action points
    msglist.Id = 15; 
    if( MessageGetMsg( &gMessage, &msglist) == 1 ) strcpy( str, msglist.Text );
    NG_FEAT_B( str, 379, y, hspace, 640, " %d", FeatGetVal( gObjDude, 8 ) );

    // meele damage
    strcpy( str, FeatGetName( 11 ) );
    NG_FEAT_B( str, 379, y, hspace, 640, " %d", FeatGetVal( gObjDude, 11 )  );

    SkillGetSpecials( Combat, 3 );
    TraitGetSpec( &Special1, &Special2 );
    strcpy( str, SkillGetName( 0 ) );
    NG_FEAT_B( str, 379, y, 2*hspace, 640, " %d%%", SkillGetTotal( gObjDude, Combat[ 0 ] ) );

    strcpy( str, SkillGetName( 1 ) );
    NG_FEAT_B( str, 379, y, hspace, 640, " %d%%", SkillGetTotal( gObjDude, Combat[ 1 ] ) );

    strcpy( str, SkillGetName( 2 )  );
    NG_FEAT_B( str, 379, y, hspace, 640, " %d%%", SkillGetTotal( gObjDude, Combat[ 2 ] ) );

    y += hspace;
    strcpy( str, TraitGetName( Special1 ) );
    w = gFont.LineWidth( str );
    gFont.Print( gNgSurface + 640 * y - w + 379, str, w, 640, NG_CAPCOLOR );
    y += hspace;
    strcpy( str, TraitGetName( Special2 ) );
    w = gFont.LineWidth( str );
    gFont.Print( gNgSurface + 640 * y - w + 379, str, w, 640, NG_CAPCOLOR );

    FontSet( FontId );
    return 1;
}

int NgCaption()
{
    xFile_t *fh;
    int y, h, FontId;
    char *p, str[256], stmp[40];

    FontId = FontGetCurrent();
    FontSet( 101 );
    sprintf( stmp, "%s.bio", gPremadeCharacter[ gNgDudeSel ].path );
    if( (fh = dbOpen( stmp, "rt" ) ) ){
        y = 40;
        h = gFont.ChrHeight();
        while( dbgets( str, 256, fh ) && (y < 260) ){
            p = gNgSurface + 640 * y + 438;
            y += h;
            gFont.Print( p, str, 202, 640, NG_CAPCOLOR );
        }
        dbClose( fh );
    }
    FontSet( FontId );
    return 1;
}



