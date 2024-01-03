#include "FrameWork.h"

static xFile_t *gCreditsFh;
static int gCreditsColorB;
static int gCreditsFontB;
static int gCreditsFontA;
static int gCreditsColorA;

static int CreditsText( char *text, int *Font, int *Color );

void Credits( const char *fname, int WallPaperId, char sel )
{
    ArtFrmHdr_t *ImgData;
    char *ObjData;
    CachePool_t *Img;
    char stmp[260], text[12], *buf,*TextScr,*pSrc,*pTxt,*scr,*Surface,*p,*pDst;
    int CursorClr,w,h,TxtWidth,time,color,FontId,FastExit,FontSave,cc,num,y,win;

    FontSave = FontGetCurrent();
    PalLoadFromFile( "color.pal" );
    if( sel & 1 ){
        gCreditsColorA = CREDITS_COLOR_A;
        gCreditsFontA  = CREDITS_FONT_A;
        gCreditsColorB = CREDITS_COLOR_B;
        gCreditsFontB  = CREDITS_FONT_B;
    } else {
        gCreditsColorA = CREDITS_COLOR_B;
        gCreditsFontA  = CREDITS_FONT_B;
        gCreditsColorB = CREDITS_COLOR_A;
        gCreditsFontB  = CREDITS_FONT_A;
    }
    SoundUpdateAll();
    if( MessageMakeFilePath(stmp, fname) == 1 ){
        gCreditsFh = dbOpen(stmp, "rt");
        if( gCreditsFh ){
            SoundUpdateAll();
            CycleColorStop();
            GmouseLoadCursor( 0 );
            CursorClr = MseIsCursorClear();
            cc = CursorClr == 0;
            if( CursorClr ) MseDrawCursor();
            win = WinCreateWindow( 0, 0, 640, 480, COLOR_ALPHA, 0x14 );
            SoundUpdateAll();
            if( win != -1 ){
                Surface = WinGetSurface( win );
                if( Surface ){
                    scr = Malloc( 640*480 );
                    if( scr ){
                        SoundUpdateAll();
                        memset( scr, COLOR_ALPHA, 640*480 );
                        if( WallPaperId != -1 ){
                            ImgData = ArtLoadImg( WallPaperId, &Img );
                            if( ImgData ){
                                w = ArtGetObjWidth( ImgData, 0, 0 );
                                h = ArtGetObjHeight( ImgData, 0, 0 );
                                ObjData = ArtGetObjData( ImgData, 0, 0 );
                                ScrCopy( ObjData, w, h, w, WIN_XY( 640 - w, ( 480 - h ) / 2, 640, scr ), 640 );
                                ArtClose( Img );
                            }
                        }
                        if( (buf = Malloc( 640*480 )) ){
                            memset( buf, 0, 640*480 );
                            FontSet( gCreditsFontB );
                            h = gFont.ChrHeight();
                            FontSet( gCreditsFontA );
                            w = gFont.ChrHeight();
                            y = w + (h >= w ? h - w : 0);
                            num = 640 * y;
                            TextScr = Malloc( 640 * y );
                            if( TextScr ){
                                ScrCopy( scr, 640, 480, 640, Surface, 640 ); // copy wallpaper on screen
                                WinUpdate( win );
                                time = 0;
                                FadeStep( gPalBase );
                                FastExit = 0;
                                pSrc = buf + 640;
                                pDst = buf + 640*479; // last line
                                while( CreditsText( text, &FontId, &color ) ){
                                    FontSet( FontId );
                                    TxtWidth = gFont.LineWidth( text );
                                    if( TxtWidth >= 640 ) continue;
                                    memset( TextScr, 0, num );
                                    gFont.Print( TextScr, text, 640, 640, color );
                                    if( y == 0 ) continue;
                                    p = TextScr;
                                    pTxt = pDst + (640 - TxtWidth) / 2; // center text
                                    for( ; (void *)p < (void *)(TextScr + 640 * y); p += 640 ){ // petla
                                        if( InpUpdate() != -1 ){
                                            FastExit = 1;
                                            break;
                                        }
                                        memmove( buf, pSrc, 640*479 ); 
                                        memcpy( pTxt, p, TxtWidth ); // copy text line to last line
                                        ScrCopy( scr, 640, 480, 640, Surface, 640 ); // copy wallpaper
                                        ScrCopyAlpha( buf, 640, 480, 640, Surface, 640 );
                                        while( TimerCurrDiff( time ) < CREDITS_SPEED );
                                        time = TimerGetSysTime();
                                        WinUpdate( win );                                                
                                    }                                            
                                    if( FastExit ) break;
                                }
                                if( !FastExit ){ // fast scroll at the end
                                    pDst = buf + 640 * 479;
                                    pSrc = buf + 640;
                                    do{
                                        if( InpUpdate() != -1 ) break;
                                        memmove( buf, pSrc, 640 * 479 );
                                        memset( pDst, 0, 640 );
                                        ScrCopy( scr, 640, 480, 640, Surface, 640 );
                                        ScrCopyAlpha( buf, 640, 480, 640, Surface, 640 );
                                        while( TimerCurrDiff( time ) < 38 );
                                        time = TimerGetSysTime();
                                        WinUpdate( win );
                                    }while( ++FastExit < 480 );
                                }
                                Free( TextScr );
                            }
                            Free( buf );
                        }
                        Free( scr );
                    }
                }
                SoundUpdateAll();
                FadeStep( gFadePaletteC );
                SoundUpdateAll();
                WinClose( win );
            }
            if( !cc ) MseCursorRedraw();
            GmouseLoadCursor(1);
            CycleColorStart();
            dbClose( gCreditsFh );
        }
    }
    FontSet(FontSave);
}

static int CreditsText( char *text, int *Font, int *Color )
{
    char stmp[ 256 ], *p;
    p = stmp;
    while( dbgets( stmp, 256, gCreditsFh ) ){
	switch( stmp[ 0 ] ){
	    case '@' : *Font  = gCreditsFontB; *Color = gCreditsColorA; p++; break;
	    case '#' : *Font  = gCreditsFontA; *Color = CREDITS_COLOR_C; p++; break;
	    case ';' : continue;
	    default: *Font  = gCreditsFontA; *Color = gCreditsColorB; break;
	}
	strncpy( text, p, 256 );
	return 1;
    }
    return 0;
}


