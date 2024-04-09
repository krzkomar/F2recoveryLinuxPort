#include "FrameWork.h"

// .text
int gWinInit = 0;
int gWinTitle = -1;
WinSys_t gWinSys = { 0 };
char *gWinSurface = NULL;

int gWinPointedWg1 = -1;

//.bss
int  gWinWindowsIds[ WIN_COUNT_MAX ];
Window_t *gWinWindows[ WIN_COUNT_MAX ];
void (*gWinRestoreDisplay)();
int  gWinWindowsCount;
int  gWinWindowsDefaultFlags;
int  gWinUnk01;
int  gWinFillPattern;
int  (*gWinVidModeInitCb)(void);
int  gWinRedrawAreaRun;
Image_t *gWinTexture;
WinRadio_t gWinRadioButtons[ 64 ];

void WinDrawCursor( Window_t *Video, VidRect_t *Area );
int WinRadioClicked( Widget_t *wg );

/***********************************************/

int WinInit( int (*VidModeInit)(), void (*Close)(void), int Flags )
{
    int err;
    char *Pal;
    Window_t *gw;

    if( gWinInit ) return 9; // Program already running
    if( gWinTitle == -1 ) return 10; // Program title not set
    if( gWinSys.Init ) return 4; // Attempt to initialize window system twice
    memset( gWinWindowsIds, 0xff, 50 * 4);
    if( dbLoadData( "", 0 ) == -1 ) return 7; // Error initializing default database
    if( FontInit() == -1 ) return 3; // Couldn't find/load text fonts
    gWinVidModeInitCb = VidModeInit;
    gWinRestoreDisplay = VidClose;
    err = VidModeInit();
    if( (err == -1 || err == 8) && gWinRestoreDisplay ) gWinRestoreDisplay();
    if( err == -1 ) return 1;                               // Error initializing video mode
    if( err == 8 ) return 8;
    if( Flags & 0x01 ){
        gWinSurface = Malloc((gVidMainGeo.bm - gVidMainGeo.tp + 1) * (gVidMainGeo.rt - gVidMainGeo.lt + 1));
        if( !gWinSurface ){
            if( gWinRestoreDisplay )
                gWinRestoreDisplay();
            else
                VidClose();
            return 2; // Not enough memory to initialize video mode
        }
    }
    gWinUnk01 = 0;
    gWinRedrawAreaRun = 0;
    PalSetFileManage( WinOpenFile, WinReadFile, WinCloseFile );
    PalSetMemManage( (void*)Malloc, Realloc, Free );
    if( !PalInit() ){
        if( !(Pal = Malloc( 768 )) ){
            if( gWinRestoreDisplay )
                gWinRestoreDisplay();
            else
                VidClose();
            if( gWinSurface ) Free( gWinSurface );
            return 2; // Not enough memory to initialize video mode
        }
        ScrFillSolid( Pal, 768, 1, 768, 0 );
        PalCreateColors( PalGetCurrent(), Pal );
        Free( Pal );
    }
    ErrorCloseLogTaskAtExit();
    if( InpOpen( Flags & 0x200 ) == -1 ) return 11; // Failure initializing input devices
    MsgInit();
    if( !(gw = gWinWindows[ 0 ] = Malloc( sizeof( Window_t ) )) ){
	if( gWinRestoreDisplay )
    	    gWinRestoreDisplay();
	else
    	    VidClose();
	if( gWinSurface ) Free( gWinSurface );
	return 2; // Not enough memory to initialize video mode
    }
    gw->Surface = NULL;
    gw->Id     = 0;
    gw->Flags  = 0;
    gw->Widget = NULL;
    gw->PointedWg   = NULL;
    gw->Img1   = NULL;
    gw->Dialog = NULL;
    gw->RandX  = 0;
    gw->RandY  = 0;
    gw->Geometry.lt = gVidMainGeo.lt;
    gw->Geometry.tp = gVidMainGeo.tp;
    gw->Geometry.rt = gVidMainGeo.rt;
    gw->Geometry.bm = gVidMainGeo.bm;
    gWinWindowsCount = 1;
    gWinSys.Init = 1;
    gw->Width  = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    gw->Height = gVidMainGeo.bm - gVidMainGeo.tp + 1;
    gWinWindowsIds[ 0 ] = 0;
    gWinTexture = 0;
    gWinFillPattern = 0;
    gWinWindowsDefaultFlags = Flags;
    gWinSys.Colors[ 0 ] = 0x294a;
    gWinSys.Colors[ 1 ] = 0x3def;
    gWinSys.Colors[ 2 ] = 0x2108;
    gWinSys.Colors[ 3 ] = 0x5294;
    gWinSys.Colors[ 4 ] = 0x7feb;
    gWinSys.Colors[ 5 ] = 0x7c00;
    xCleanAdd( WinDestruct );
    return 0;    
}

int WinResize( int (*InitVideo)(void) )
{
    int err, i;
    char *Surf;
    Window_t *Widget;

    if( !gWinSys.Init ) return 5;
    if( InitVideo() == -1 ) return 1;

    err = 0;    
    for( i = 1; i < gWinWindowsCount; i++ ){
        if( gVidMainGeo.rt - gVidMainGeo.lt + 1 < gWinWindows[ i ]->Width || gVidMainGeo.bm - gVidMainGeo.tp + 1 < gWinWindows[ i ]->Height ){
    	    err = 6;
    	    break;
        }
    }
    if( !err && (gWinWindowsDefaultFlags & 0x01) ){
        if( (Surf = Malloc( (gVidMainGeo.bm - gVidMainGeo.tp + 1) * (gVidMainGeo.rt - gVidMainGeo.lt + 1) )) ){
            Free( gWinSurface );
            gWinSurface = Surf;
        } else {
            err = 2;
        }
    }
    if( err )
        gWinVidModeInitCb();
    else
        gWinVidModeInitCb = InitVideo;        
    PalSetColors( PalGetCurrent() );
    gWinWindows[ 0 ]->Geometry.lt = gVidMainGeo.lt;
    gWinWindows[ 0 ]->Geometry.tp = gVidMainGeo.tp;
    gWinWindows[ 0 ]->Geometry.rt = gVidMainGeo.rt;
    gWinWindows[ 0 ]->Geometry.bm = gVidMainGeo.bm;
    gWinWindows[ 0 ]->Width  = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    gWinWindows[ 0 ]->Height = gVidMainGeo.bm - gVidMainGeo.tp + 1;
    Widget = WinGetWindow( 0 );
    if( gWinSys.Init && Widget ) WinRedraw( Widget, &Widget->Geometry, 0 );    
    for( i = 1; i < gWinWindowsCount; i++ ){
        WinSetPosition( gWinWindows[ i ]->Id, gWinWindows[ i ]->Geometry.lt, gWinWindows[ i ]->Geometry.tp );
    }
    return err;
}

int WinReady()
{
    return gWinSys.Init;
}

void WinDestruct()
{
    static int gWinGuard = 0;
    int i;

    if( gWinGuard ) return;    
    gWinGuard = 1;
    if( gWinSys.Init ){
        MsgKill();        
        for( i = gWinWindowsCount - 1; i >= 0; i-- ){
            WinFree( gWinWindows[ i ]->Id );            
        }
        if( gWinTexture ) Free( gWinTexture );
        if( gWinSurface ) Free( gWinSurface );
        if( gWinRestoreDisplay ) gWinRestoreDisplay();
        InpClose();
        RegionsPurge();
        FontFree();
        PalStackZero();
        gWinSys.Init = 0;
        gWinTitle = -1;
    }
    gWinGuard = 0;
}

char WinColorEx( int BgColor ) // added
{
    Pal8_t *p;
    
    p = &gPalBase[ ( BgColor & 0xff ) + 43 ];
    
    return gPalColorCubeRGB[ (int)p->r ][ (int)p->g ][ (int)p->b ];
//    return *(((char *)gPalColorCubeRGB) + ((int *)gPalBase)[ Color + 43 ]);
}

int WinCreateWindow( int Xpos, int Ypos, int Width, int Height, short BgColor, int Flags )
{
    Window_t *wg;
    int WgId, i, n;


    if( !gWinSys.Init || gWinWindowsCount == WIN_COUNT_MAX || Width > VidW || Height > VidH ) return WIN_ERROR;
    wg = Malloc( sizeof( Window_t ) );
    gWinWindows[ gWinWindowsCount ] = wg;
    if( !wg ) return WIN_ERROR;
    if( !( wg->Surface = Malloc( Width * Height ) ) ){ Free( wg ); return WIN_ERROR; }
    for( WgId = 1; WinGetWindow( WgId ); WgId++ );
    wg->Id = WgId;
    if( Flags & WIN_FLG_1 ) Flags |= gWinWindowsDefaultFlags;
    wg->Width  = Width;
    wg->Height = Height;
    wg->Flags  = Flags;
    wg->RandX  = rand() & 0xFFFE;
    wg->RandY  = rand() & 0xFFFE;
    if( BgColor == WIN_BGALPHA ){
        if( !gWinTexture ) BgColor = WIN_PALCOLOR_ALPHA;
    } else if( BgColor && 0xff00 ){
        BgColor = WinColorEx( BgColor );
    }

    wg->Widget = NULL;
    wg->PointedWg = NULL;
    wg->Img1 = NULL;
    wg->Dialog  = NULL;
    wg->Blitter = (void *)ScrCopyAlpha;
    wg->FontColor = BgColor;
    gWinWindowsIds[ WgId ] = gWinWindowsCount;
    gWinWindowsCount++;
    WinDrawFilledRect( WgId, 0, 0, Width, Height, BgColor );
    wg->Flags |= WIN_FLG_CLEAN;

    WinSetPosition( WgId, Xpos, Ypos );
    wg->Flags = Flags;

    if( !( Flags & WIN_FLG_4 ) ){ // win on top ?
        for( i = gWinWindowsCount - 2; i > 0; i-- ){
            if( !(gWinWindows[ i ]->Flags & WIN_FLG_4) ) break;
        }
        if( i != gWinWindowsCount - 2 ){
            n = i + 1;
            for( i = gWinWindowsCount - 1; i > n; i-- ){
                gWinWindows[ i ] = gWinWindows[ i - 1 ];
                gWinWindowsIds[ gWinWindows[ i ]->Id ] = i;
            }
            gWinWindows[ n ] = wg;
            gWinWindowsIds[ WgId ] = n;
        }
    }
    return WgId;
}

void WinClose( int WgId )
{
    Window_t *Widget;
    VidRect_t Area;
    int id, i;

    Widget = WinGetWindow( WgId );
    if( !gWinSys.Init || !Widget ) return;
    Area = Widget->Geometry;
    id = gWinWindowsIds[ Widget->Id ];
    WinFree( WgId );
    gWinWindowsCount--;
    gWinWindowsIds[ WgId ] = -1;
    for( i = id; i < gWinWindowsCount; i++ ){
        gWinWindows[ i ] = gWinWindows[ i + 1 ];
        gWinWindowsIds[ gWinWindows[ i ]->Id ] = i;
    }
    if( gWinSys.Init ) WinRedrawArea( &Area, 0 ); // restore area beneath
}

void WinFree( int WinId )
{
    Window_t *win;
    Widget_t *wg, *tmp;

    if( !( win = WinGetWindow( WinId )) ) return;
    if( win->Surface ) Free( win->Surface );
    if( win->Dialog ) Free( win->Dialog );
    wg = win->Widget;
    while( wg ){
	tmp = wg->next;
	WinWidgetsFree( wg );
	wg = tmp;
    }
    Free( win );
}

void WinGetUnk01( int arg )
{
    if( gWinSurface ) gWinUnk01 = arg;
}

void WinDrawFrame( int WinId )
{
    Window_t *win;

    win = WinGetWindow(WinId);
    if( !gWinSys.Init || !win ) return;        
    ScrEnlight( win->Surface + 5, win->Width - 10, 5, win->Width );
    ScrEnlight( win->Surface, 5, win->Height, win->Width );
    ScrEnlight( &win->Surface[ win->Width - 5 ], 5, win->Height, win->Width );
    ScrEnlight( &win->Surface[ win->Width * (win->Height - 5) + 5], win->Width - 10, 5, win->Width );
    ScrRectangle( win->Surface, win->Width, 0, 0, win->Width - 1, win->Height - 1, WIN_FRCOL_ALPHA );
    ScrShadowRectangle( win->Surface, win->Width, 1, 1, win->Width - 2, win->Height - 2, WIN_FRCOL_A, WIN_FRCOL_B );
    ScrShadowRectangle( win->Surface, win->Width, 5, 5, win->Width - 6, win->Height - 6, WIN_FRCOL_B, WIN_FRCOL_A );
}

void WinSetDefaultColors()
{
    if( !gWinSys.Init ) return;
    if( gWinTexture ){
        Free( gWinTexture );
        gWinTexture = NULL;
    }
    gWinSys.Colors[ 0 ] = WIN_COLOR_A;
    gWinSys.Colors[ 1 ] = WIN_COLOR_B;
    gWinSys.Colors[ 2 ] = WIN_COLOR_C;
    gWinSys.Colors[ 3 ] = WIN_COLOR_D;
    gWinSys.Colors[ 4 ] = WIN_COLOR_E;
    gWinSys.Colors[ 5 ] = WIN_COLOR_F;
}

void WinLoadTexture( const char *fname )
{
    xFile_t *fh;
    Image_t *p;
    int i,b, g, r;
    char str[ 80 ];

    if( !gWinSys.Init ) return;    
    if( !( fh = dbOpen( fname, "rt") ) ) return;

    dbgets( str, 79, fh );
    p = gWinTexture;
    str[ strlen( str ) - 1 ] = '\0';
    if( p ) Free( p );
    gWinTexture = WinLoadImage( str );
    for( i = 0; i < 6; i++ ){
        dbgets( str, 79, fh );
        sscanf( str, "%d %d %d", &r, &g, &b );
        gWinSys.Colors[ i ] = RGB16_SET( (32 * r - 1) / 100, (32 * g - 1) / 100, (32 * b - 1) / 100 );
    }
    dbClose( fh );
}

void WinClear( int Pattern )
{
    Window_t *Widget;

    if( !gWinSys.Init ) return;    
    gWinFillPattern = Pattern;
    Widget = WinGetWindow( 0 );
    if( !gWinSys.Init ) return;        
    if( Widget ) WinRedraw( Widget, &Widget->Geometry, 0 );
}

void WinDrawText( int WinId, char *text, int TextWidth, int Xpos, int Ypos, int Color )
{
    Window_t *win;
    Image_t *pos;
    VidRect_t geo;
    int Width, h, _h, _y, _x, FontColor;
    char *Surface;

    win = WinGetWindow(WinId);
    if( !gWinSys.Init || !win ) return;
    if( TextWidth == 0 ){
        TextWidth = ( (Color & 0x40000) ) ? gFont.LineWidthMono( text ) : gFont.LineWidth( text );
    }
    Width = win->Width;
    if( TextWidth + Xpos > Width ){
        if( (Color & 0x4000000) == 0 ) return;
        TextWidth = Width - Xpos;
    }
    h = gFont.ChrHeight();
    Surface = &win->Surface[ Xpos + win->Width * Ypos ];
    if( Ypos + h > win->Height ) return;    
    if( (Color & 0x2000000) == 0 ){
        if( win->FontColor == 0x100 && gWinTexture ){
            _x = Ypos + win->RandY;
            _y = Xpos + win->RandX;
            pos = gWinTexture;
            _h = gFont.ChrHeight();
            ScrFillImg( Surface, TextWidth, _h, Width, pos, _y, _x );
        } else {
            FontColor = win->FontColor;
            _h = gFont.ChrHeight();
            ScrFillSolid(Surface, TextWidth, _h, Width, FontColor);
        }
    }
    if( Color & 0xff00 ){
    	Color = WinColorEx( Color );
    }
    gFont.Print( Surface, text, TextWidth, win->Width, Color );
    if( Color & WIN_UPDATE ) {
        geo.lt = win->Geometry.lt + Xpos;
        geo.tp = win->Geometry.tp + Ypos;
        geo.rt = TextWidth + geo.lt;
        geo.bm = gFont.ChrHeight() + geo.tp;
        WinRedraw(win, &geo, 0);
    }    
}

void WinDrawTextList( int WinId, char **text, int Lines, int UnderLineWidth, int Xpos, int Ypos, int Color )
{
    Window_t *Widget;
    int w;
    char *surface;
    int h, y, pitch, x, i;

    Widget = WinGetWindow(WinId);
    if( !gWinSys.Init || !Widget ) return;
    w = Widget->Width;
    h = gFont.ChrHeight();
    pitch = w * h;
    surface = &Widget->Surface[ Xpos + w * Ypos ];
    x = UnderLineWidth - 1;
    y = h / 2;
    for( i = 0; i < Lines; i++, Ypos += h, surface += pitch, text++ ){
        if( **text ){
    	    WinDrawText( WinId, *text, UnderLineWidth, Xpos, Ypos, Color );
        } else if( UnderLineWidth ){
            ScrLine( surface, w, 0, y, x, y, WIN_FRCOL_B );
            ScrLine( surface, w, 0, y + 1, x, y + 1, WIN_FRCOL_A );
        }        
    }    
}

void WinDrawLine( int WinId, int x0, int y0, int x1, int y1, int Color )
{
    Window_t *win;

    win = WinGetWindow( WinId );
    if( !gWinSys.Init || !win ) return;
    if( Color & 0xff00 ){
    	Color = WinColorEx( Color );
    }
    ScrLine( win->Surface, win->Width, x0, y0, x1, y1, Color );
}

void WinDrawRectangle( int WinId, int x0, int y0, int x1, int y1, int Color )
{
    Window_t *win;
    int tmp;

    win = WinGetWindow( WinId );
    if( !gWinSys.Init && !win ) return;    
    if( Color & 0xff00 ) Color = WinColorEx( Color );
    if( x1 < x0 ){
        tmp = x1;
        x1 = x0;
        x0 = tmp;
    }
    if( y1 < y0 ){
        tmp = y1;
        y1 = y0;
        y0 = tmp;
    }
    ScrRectangle( win->Surface, win->Width, x0, y0, x1, y1, Color );    
}

void WinDrawShadowRect( int WinId, int x0, int y0, int x1, int y1, int ColorUp, int ColorDn )
{
    Window_t *Widget;

    Widget = WinGetWindow( WinId );
    if( !gWinSys.Init || !Widget ) return;    
    if( ColorUp & 0xff00 ){
        ColorUp &= 0xff00;
	ColorUp |= WinColorEx( ColorUp );
    }
    if( ColorDn & 0xff00 ){
        ColorDn &= 0xff00;
	ColorDn |= WinColorEx( ColorDn );
    }
    ScrShadowRectangle( Widget->Surface, Widget->Width, x0, y0, x1, y1, ColorUp, ColorDn );    
}

void WinDrawFilledRect( int WinId, int Xpos, int Ypos,int Width, int Height, int Color )
{
    Window_t *win;

    win = WinGetWindow( WinId );
    if( !gWinSys.Init && !win ) return;    
    if( Color == 0x100 ){
        if( gWinTexture )
            ScrFillImg( &win->Surface[ win->Width * Ypos + Xpos ], Width, Height, win->Width, gWinTexture, Xpos + win->RandX, Ypos + win->RandY );
        else
            Color = WIN_FRCOL_ALPHA;
    } else if ( Color & 0xff00 ){
	Color = WinColorEx( Color );
    }
    if( Color < 0x100 ) ScrFillSolid( &win->Surface[ win->Width * Ypos + Xpos ], Width, Height, win->Width, Color );    
}

void WinMoveTop( int WinId)
{
    Window_t *win, *v6;
    int id, cnt, i;

    win = WinGetWindow( WinId );
    id = gWinWindowsIds[ win->Id ];
    if( !gWinSys.Init ) return;    

    if( win->Flags & WIN_FLG_CLEAN ){
        win->Flags &= ~WIN_FLG_CLEAN;
        if( id == gWinWindowsCount - 1 ) WinRedraw( win, &win->Geometry, 0 );
    }

    cnt = gWinWindowsCount - 1;
    if( id < gWinWindowsCount - 1 && (win->Flags & 2) == 0 ){
        for( i = id; id < cnt && ((win->Flags & 4) != 0 || (gWinWindows[i + 1]->Flags & 4) == 0); i++ ){
            v6 = gWinWindows[ i + 1 ];
            gWinWindows[ i ] = v6;
            gWinWindowsIds[ v6->Id ] = id++;
        }
        gWinWindows[ id ] = win;
        gWinWindowsIds[ win->Id ] = id;
        WinRedraw( win, &win->Geometry, 0 );
    }    
}

void WinUpdateDirty( int WinId )
{
    Window_t *win;

    win = WinGetWindow( WinId );
    if( !gWinSys.Init || !win ) return;        
    if( win->Flags & WIN_FLG_CLEAN ) return;    
    win->Flags |= WIN_FLG_CLEAN;
    if( gWinSys.Init ) WinRedrawArea( &win->Geometry, 0 );
}

void WinSetPosition( int WinId, int Xpos, int Ypos )
{
    Window_t *win;
    VidRect_t PrevGeo;

    win = WinGetWindow( WinId );
    if( !gWinSys.Init || !win ) return;    
    PrevGeo = win->Geometry;
    if( Xpos < 0 ) Xpos = 0;
    if( Ypos < 0 ) Ypos = 0;
    if( win->Flags & 0x100 ) Xpos += 2;
    if( Xpos + win->Width  - 1 > gVidMainGeo.rt ) Xpos = gVidMainGeo.rt - win->Width  + 1;
    if( Ypos + win->Height - 1 > gVidMainGeo.bm ) Ypos = gVidMainGeo.bm - win->Height + 1;
    if( win->Flags & 0x100 ) Xpos &= 0xFC;
    win->Geometry.lt = Xpos;
    win->Geometry.tp = Ypos;
    win->Geometry.rt = win->Width + Xpos - 1;
    win->Geometry.bm = win->Height + Ypos - 1;
    if( !(win->Flags & WIN_FLG_CLEAN) ){
        WinRedraw( win, &win->Geometry, 0 );
        if( gWinSys.Init ) WinRedrawArea( &PrevGeo, 0 );
    }
}

void WinUpdate( int WinId )
{
    Window_t *win;

    win = WinGetWindow(WinId);
    if( !gWinSys.Init || !win ) return;
    WinRedraw( win, &win->Geometry, 0 );
}

void WinAreaUpdate( int WinId, VidRect_t *Area )
{
    Window_t *win;
    VidRect_t geo;

    win = WinGetWindow(WinId);
    if( !gWinSys.Init || !win ) return;
            
    geo.lt = win->Geometry.lt + Area->lt;
    geo.tp = win->Geometry.tp + Area->tp;
    geo.rt = win->Geometry.lt + Area->rt;
    geo.bm = win->Geometry.tp + Area->bm;
    WinRedraw( win, &geo, 0 );
}

void WinRedraw( Window_t *win, VidRect_t *Geometry, char *pSurface )
{
    int height;
    VidRect_t MouseArea;
    Regions_t *region, *p, *t;
    int w,h,width, dstPitch;
    void *pSrc;

    if( win->Flags & WIN_FLG_CLEAN ) return;    
    if( ( win->Flags & WIN_FLG_BLIT ) && gWinUnk01 && !gWinRedrawAreaRun ){
        WinRedrawArea( Geometry, gWinSurface );
        width  = Geometry->rt - Geometry->lt + 1;
        height = Geometry->bm - Geometry->tp + 1;
        if( MseCursorHidden() || !MseCursorInArea( Geometry->lt, Geometry->tp, Geometry->bm, Geometry->rt ) ){
            WinDrawCursor( win, Geometry );
            gVidCopyA( gWinSurface, width, height, 0, 0, width, height, Geometry->lt, Geometry->tp );
        } else {
            MseCursorShow();
            MseGetCursorSizeRect( &MouseArea );
            for( region = RegionsUnk01( Geometry, &MouseArea ); region; region = region->Prev ){
                WinDrawCursor( win, &region->Area );
                gVidCopyA( 
            	    WIN_XY( region->Area.lt - Geometry->lt, region->Area.tp - Geometry->tp, width, gWinSurface ), width, region->Area.bm - region->Area.tp + 1, 0, 0, 
            	    region->Area.rt - region->Area.lt + 1, region->Area.bm - region->Area.tp + 1, region->Area.lt, region->Area.tp 
            	);
                RegionsPush( region );                
            }
        }
    } else {
        if( !(region = RegionsPop()) ) return;
        region->Prev = NULL;        
        region->Area.lt = ( win->Geometry.lt < Geometry->lt ) ? Geometry->lt : win->Geometry.lt;
        region->Area.tp = ( win->Geometry.tp < Geometry->tp ) ? Geometry->tp : win->Geometry.tp;        
        region->Area.rt = ( win->Geometry.rt > Geometry->rt ) ? Geometry->rt : win->Geometry.rt;        
        region->Area.bm = ( win->Geometry.bm > Geometry->bm ) ? Geometry->bm : win->Geometry.bm;

        if( (region->Area.rt >= region->Area.lt) && (region->Area.bm >= region->Area.tp) ){
    	    if( pSurface ) dstPitch = Geometry->rt - Geometry->lt + 1;
            WinRegionUpdate( win, &region, pSurface );
            if( win->Id ){                
                for( p = region; p; p = p->Prev ){
                    WinDrawCursor( win, &p->Area );
                    if( pSurface ){ // mouse fetch bmp
                	if( gWinUnk01 && (win->Flags & WIN_FLG_BLIT ) ){
                            win->Blitter(
                        	WIN_XY( p->Area.lt - win->Geometry.lt, p->Area.tp - win->Geometry.tp, win->Width, win->Surface ), p->Area.rt - p->Area.lt + 1, p->Area.bm - p->Area.tp + 1, win->Width, 
				WIN_XY( p->Area.lt - Geometry->lt, p->Area.tp - Geometry->tp, dstPitch, pSurface ), dstPitch
                   	    );
                	} else {
                            ScrCopy(
                        	WIN_XY( p->Area.lt - win->Geometry.lt, p->Area.tp - win->Geometry.tp, win->Width, win->Surface ), p->Area.rt - p->Area.lt + 1, p->Area.bm - p->Area.tp + 1, win->Width, 
                        	WIN_XY( p->Area.lt - Geometry->lt, p->Area.tp - Geometry->tp, dstPitch, pSurface ), dstPitch 
                    	    );
                    	}
                    } else if( gWinUnk01 ){
                        w = gVidMainGeo.rt - gVidMainGeo.lt + 1;
                        if( win->Flags & WIN_FLG_BLIT ){
                            win->Blitter(
                        	WIN_XY( p->Area.lt - win->Geometry.lt, p->Area.tp - win->Geometry.tp, win->Width, win->Surface), p->Area.rt - p->Area.lt + 1, p->Area.bm - p->Area.tp + 1, win->Width, 
                        	WIN_XY( p->Area.lt, p->Area.tp, w, gWinSurface ), gVidMainGeo.rt - gVidMainGeo.lt + 1
                    	    );
                        } else {
                            ScrCopy(
                        	WIN_XY( p->Area.lt - win->Geometry.lt, p->Area.tp - win->Geometry.tp, win->Width, win->Surface ), p->Area.rt - p->Area.lt + 1, p->Area.bm - p->Area.tp + 1, win->Width, 
                        	WIN_XY( p->Area.lt, p->Area.tp, w, gWinSurface ), gVidMainGeo.rt - gVidMainGeo.lt + 1
                    	    );
			}
                    } else { // screen draw
                        gVidCopyA(
                    	    WIN_XY( p->Area.lt - win->Geometry.lt, p->Area.tp - win->Geometry.tp, win->Width, win->Surface ), win->Width, p->Area.bm - p->Area.tp + 1, 
                    	    0, 0, p->Area.rt - p->Area.lt + 1, p->Area.bm - p->Area.tp + 1, p->Area.lt, p->Area.tp
                    	);
                    }                        
            	}
    	    } else {
                for( p = region; p; p = p->Prev ){
                    w = p->Area.rt - p->Area.lt + 1;
                    h = p->Area.bm - p->Area.tp + 1;
                    if( (pSrc = Malloc( w * h )) ){
                        ScrFillSolid( pSrc, w, h, w, gWinFillPattern );
                        if( pSurface ){
                            ScrCopy( 
                        	pSrc, w, h, w, 
                        	WIN_XY( p->Area.lt - Geometry->lt, p->Area.tp - Geometry->tp, dstPitch, pSurface ), dstPitch 
                    	    );
                        } else if( gWinUnk01 ){
                            ScrCopy( 
                        	pSrc, w, h, w, 
                        	WIN_XY( p->Area.lt, gVidMainGeo.rt - gVidMainGeo.lt + 1, p->Area.tp, gWinSurface ), gVidMainGeo.rt - gVidMainGeo.lt + 1
                    	    );
                        } else {
                            gVidCopyA( pSrc, w, h, 0, 0, w, h, p->Area.lt, p->Area.tp );
                        }
                        Free( pSrc );
                    }
                }
    	    }            
            for( p = region; p; ){
        	t = p->Prev;
                if( gWinUnk01 && !pSurface ) gVidCopyA(&gWinSurface[p->Area.lt + (gVidMainGeo.rt - gVidMainGeo.lt + 1) * p->Area.tp], gVidMainGeo.rt - gVidMainGeo.lt + 1, p->Area.bm - p->Area.tp + 1, 0, 0, p->Area.rt - p->Area.lt + 1, p->Area.bm - p->Area.tp + 1, p->Area.lt, p->Area.tp);
                RegionsPush( p );
                p = t;
            }
            if( !gWinRedrawAreaRun && !pSurface && !MseCursorHidden() && MseCursorInArea(Geometry->lt, Geometry->tp, Geometry->bm, Geometry->rt) ) MseCursorShow();
        } else {
    	    RegionsPush( region );
        }    
    }    
}

void WinUpdateArea( VidRect_t *Area )
{
    if( !gWinSys.Init ) return;
    WinRedrawArea( Area, NULL );
}

void WinRegionUpdate( Window_t *Win, Regions_t **Mse, char *pSurface )
{
    VidRect_t MseCursorArea;
    Window_t *wg;
    int i;
    for( i = gWinWindowsIds[ Win->Id ] + 1; i < gWinWindowsCount; i++ ){
        if( !*Mse ) break;
        wg = gWinWindows[ i ];
        if( wg->Flags & WIN_FLG_CLEAN ) continue;
        if( gWinUnk01 && (wg->Flags & 0x20 ) ){
    	    if( gWinRedrawAreaRun ) continue;
    	    WinRedraw( wg, &wg->Geometry, NULL );
    	}
    	RegionsUpdate( Mse, &gWinWindows[ i ]->Geometry );
    }
    if( (pSurface == gWinSurface || !pSurface) && !MseCursorHidden() ){
        MseGetCursorSizeRect( &MseCursorArea );
        RegionsUpdate( Mse, &MseCursorArea );
    }
}

void WinDragAndMove( int WinId )
{
    Window_t *Widget;
    int yd, xd, Xmse1,Ymse1, Ymse0, Xmse0;
    Regions_t *region, *prev;
    VidRect_t Rect, *Geo;

    Widget = WinGetWindow(WinId);
    yd = xd = 0;
    if( !gWinSys.Init || !Widget ) return;    
    WinMoveTop(WinId);
    Rect = Widget->Geometry;
    InpTaskProceed();
    if( RecRecord() != 3 ) MseUpdate();
    Geo = &Widget->Geometry;
    while( (MseGetButtons() & ( MSE_LBHOLD | MSE_RBHOLD ) ) == 0 ){
        if( xd || yd ){
            Widget->Geometry.lt += xd;
            Widget->Geometry.tp += yd;
            Widget->Geometry.rt += xd;
            Widget->Geometry.bm += yd;
            WinRedraw( Widget, Geo, 0 );
            region = RegionsUnk01( &Rect, Geo );
            while( region ){
                prev = region->Prev;
                if( gWinSys.Init ) WinRedrawArea( &region->Area, 0 );
                RegionsPush(region);
                region = prev;
            }
            Rect = Widget->Geometry;
        }
        MseGetCursorPosition( &Xmse0, &Ymse0 );
        InpTaskProceed();
        if( RecRecord() != 3 ) MseUpdate();
        Xmse1 = Xmse0;
        Ymse1 = Ymse0;
        MseGetCursorPosition( &Xmse0, &Ymse0 );
        xd = Xmse0 - Xmse1;
        yd = Ymse0 - Ymse1;
        if( xd + Widget->Geometry.lt < gVidMainGeo.lt ) xd = gVidMainGeo.lt - Widget->Geometry.lt;
        if( xd + Widget->Geometry.rt > gVidMainGeo.rt ) xd = gVidMainGeo.rt - Widget->Geometry.rt;
        if( yd + Widget->Geometry.tp < gVidMainGeo.tp ) yd = gVidMainGeo.tp - Widget->Geometry.tp;
        if( yd + Widget->Geometry.bm > gVidMainGeo.bm ) yd = gVidMainGeo.bm - Widget->Geometry.bm;
    }
    if( (Widget->Flags & 0x100) != 0 && (Widget->Geometry.lt & 3) != 0 ) WinSetPosition(Widget->Id, Widget->Geometry.lt, Widget->Geometry.tp);    
}

void WinWhipeCursor( char *Surface )
{
    VidRect_t MouseArea;

    MseGetCursorSizeRect( &MouseArea );
    WinRedrawArea( &MouseArea, Surface );
}

void WinRedrawArea( VidRect_t *Area, char *Surface )
{
    int i;

    gWinRedrawAreaRun = 1;
    for( i = 0; i < gWinWindowsCount; i++ ){
        WinRedraw( gWinWindows[ i ], Area, Surface );
    }
    gWinRedrawAreaRun = 0;
    if( !Surface && !MseCursorHidden() ){
        if( MseCursorInArea( Area->lt, Area->tp, Area->bm, Area->rt ) ) MseCursorShow();
    }
}

Window_t *WinGetWindow( int WinId )
{
    int id;

    if( WinId == -1 ) return 0;
    if( (id = gWinWindowsIds[ WinId ]) == -1 ) return 0;    
    return gWinWindows[ id ];
}

char *WinGetSurface( int WinId )
{
    Window_t *p;

    p = WinGetWindow( WinId );
    if( !gWinSys.Init || !p ) return NULL;
    return p->Surface;        
}

int WinGetPointedWidgetId( int x, int y )
{
    int i;
    Window_t *win;

    for( i = gWinWindowsCount - 1; i >= 1; i-- ){
        win = gWinWindows[ i ];
        if( x >= win->Geometry.lt && x <= win->Geometry.rt && y >= win->Geometry.tp && y <= win->Geometry.bm ) break;
    }
    return gWinWindows[ i ]->Id;
}

int WinGetWidth( int WinId )
{
    Window_t *win;

    win = WinGetWindow( WinId );
    if( !gWinSys.Init || !win ) return -1;
    return win->Width;
}

int WinGetHeight( int WinId )
{
    Window_t *win;

    win = WinGetWindow( WinId );
    if( !gWinSys.Init || !win ) return -1;
    return win->Height;
}

int WinGetRect( int WinId, VidRect_t *Geometry )
{
    Window_t *win;
    
    win = WinGetWindow( WinId );
    if( !gWinSys.Init || !win ) return -1;
    Geometry->lt = win->Geometry.lt;
    Geometry->tp = win->Geometry.tp;
    Geometry->rt = win->Geometry.rt;
    Geometry->bm = win->Geometry.bm;
    return 0;
}

int WinCursorProcess()
{
    int i, HotKey = -1;

    VidUpdateClr();
//    tmp = gVidUpdateForbid;
//    gVidUpdateForbid = 1;
    if( gWinSys.Init && gWinWindowsCount - 1 < 1 ) return -1;

    for( i = gWinWindowsCount - 1; i >= 1; i-- ){
	if( !WinProcess( gWinWindows[ i ], &HotKey ) ) break;
        if( gWinWindows[ i ]->Flags & 0x10 ) break;
    }
//    gVidUpdateForbid = tmp;
    VidUpdateOpt();
    return HotKey;
}

Widget_t *WinLookup( int WgtId, Window_t **win )
{
    Widget_t *wgt;
    int i;

    if( gWinWindowsCount <= 0 ) return 0;    
    for( i = 0; i < gWinWindowsCount; i++ ){        
        for( wgt = gWinWindows[ i ]->Widget; wgt; wgt = wgt->next ){
	    if( WgtId == wgt->Id ){
		if( win ) *win = gWinWindows[ i ];
		return wgt;
	    }	    
        }
    }
    return 0;        
}

int WinDialogRun( int key )
{
    int j, i;
    WinDlg_t *p;

    if( !gWinSys.Init ) return 0;
    for( j = gWinWindowsCount - 1; j >= 1; j-- ){
        if( (p = gWinWindows[ j ]->Dialog) ){
            for( i = 0; i < p->Cnt; i++ ){
                if( key == p->box[ i ].key ){
                    key = TextBoxRun( p, i );
                    break;
                }
            }
        }
	if( gWinWindows[ j ]->Flags & 0x10 ) break;
    }
    return key;
}


Image_t *WinLoadImage( const char *fname )
{
    Image_t *Img;
    xFile_t *fh;
    int x, chr, i, y, k, tmp[4];
    short w,h;    
    char *ImgData; char Colors[257];

    x = 0;

    if( !(fh = dbOpen( fname, "rb") ) ) return NULL;

    // looking for marker 'BMHD'
    for( i = 0; i < 4; i++ ){ // odczyt pierwszych 4 bajtow
        if( (tmp[ i ] = dbgetc( fh )) == -1 ){ dbClose( fh ); return NULL; }
    }
    do {
        tmp[ 0 ] = tmp[ 1 ];
        tmp[ 1 ] = tmp[ 2 ];
        tmp[ 2 ] = tmp[ 3 ];
        tmp[ 3 ] = dbgetc(fh);
	if( tmp[ 3 ] == -1 ){ dbClose( fh ); return 0; }
    }while( !(tmp[ 0 ] == 'B' && tmp[ 1 ] == 'M' && tmp[ 2 ] == 'H' && tmp[ 3 ] == 'D') );
    
    // ignore next 4 bytes
    for( i = 0; i < 4; i++ ){
        if( (tmp[i] = dbgetc(fh)) == -1 ){ dbClose( fh ); return NULL; }
    }

    tmp[ 0 ] = dbgetc( fh );
    tmp[ 1 ] = dbgetc( fh );
    w = (tmp[ 0 ] << 8) + tmp[ 1 ];

    tmp[ 0 ] = dbgetc( fh );    
    tmp[ 1 ] = dbgetc( fh );
    h = (tmp[0] << 8) + tmp[ 1 ];
    
    if( !(Img = Malloc( w * h + 4 )) ) return NULL;

    Img->Width  = w;
    Img->Height = h;
    
    // skip next 7 bytes
    for( i = 0; i < 7; i++ ){
        tmp[0] = dbgetc(fh);
        if( tmp[0] == -1 ){ dbClose(fh); return 0; }
    }

    // looking for marker 'CMAP'
    do {
        tmp[ 0 ] = tmp[ 1 ];
        tmp[ 1 ] = tmp[ 2 ];
        tmp[ 2 ] = tmp[ 3 ];
        tmp[ 3 ] = dbgetc( fh );
        if( tmp[ 3 ] == -1 ){ dbClose( fh ); Free( Img ); return 0; }
    }while( !(tmp[ 0 ] == 'C' && tmp[ 1 ] == 'M' && tmp[ 2 ] == 'A' && tmp[ 3 ] == 'P') );
    
    // ignore next 4 bytes
    for( i = 0; i < 4; i++ ){
        if( (tmp[ i ] = dbgetc(fh)) == -1 ){ dbClose( fh ); Free( Img ); return 0; }
    }
    // read palette data
    for( i = 0; i < 256; i++ ){
        if( ( tmp[ 0 ] = dbgetc( fh ) ) == -1 ) break;
        if( ( tmp[ 1 ] = dbgetc( fh ) ) == -1 ) break;
        if( ( tmp[ 2 ] = dbgetc( fh ) ) == -1 ) break;
        Colors[ i ] = gPalColorCubeRGB[ tmp[ 0 ] >> 3 ][ tmp[ 1 ] >> 3 ][ tmp[ 2 ] >> 3 ];
    }
    if( i < 256 ){ dbClose( fh ); Free( Img ); return 0; }

    // looking for marker 'BODY'
    do{
        tmp[ 0 ] = tmp[ 1 ];
        tmp[ 1 ] = tmp[ 2 ];
        tmp[ 2 ] = tmp[ 3 ];
        tmp[ 3 ] = dbgetc( fh );
        if( tmp[ 3 ] == -1 ){ dbClose( fh ); Free( Img ); return 0; }
    }while ( (tmp[ 0 ] != 'B' || tmp[ 1 ] != 'O' || tmp[ 2 ] != 'D' || tmp[ 3 ] != 'Y') );

    // read next 4 bytes    
    for( i = 0; i < 4; i++ ){        
        if( ( tmp[ i ] = dbgetc( fh ) ) == -1 ){ dbClose( fh ); Free( Img ); return 0; }
    }
    
    ImgData = Img->Data;
    if( tmp[ 3 ] == 0 ){
	// read pixels
        for( i = 0; i < h * w; i++ ){
            if( ( tmp[ 0 ] = dbgetc( fh ) ) == -1 ) break;
            *ImgData++ = Colors[ tmp[ 0 ] + 1 ];
        }
	dbClose( fh );
	return Img;        
    }
    while( (chr = dbgetc(fh)) != -1 ){
        if( chr >= 128 ) {            
            if( (tmp[ 0 ] = dbgetc( fh ) ) == -1 ) break;
            for( k = 0; k <= (256 - chr); k++ ){
                if( x < w && y < h ) *ImgData++ = Colors[ tmp[ 0 ] + 1 ];
                if( ++x == w ){
                    x = 0;
                    y++;
                }
            }
        } else {            
            for( i = 0; chr >= i; i++ ){
                if( (tmp[ 0 ] = dbgetc( fh ) ) == -1 ) break;
                if( (x < w) && (y < h) ) *ImgData++ = Colors[ tmp[0] + 1 ];
                if( ++x == w ){
            	    x = 0;
            	    y++;
                }
            }
            if( i <= chr ) break;
        }
    }    
    dbClose( fh );
    return Img;
}

void WinSetTitle( const char *Title )
{
    if( !Title ) return;    
    printf( "<dummy> Set title: \"%s\"\n", Title );
    gWinTitle = 1;
}

void  WinSetBlitter( int WinId, void (*Blitter)(void *, int, int, int, void *, int))
{
    Window_t *Widget;

    Widget = WinGetWindow( WinId );
    if( !gWinSys.Init || !Widget || !(Widget->Flags & 0x20) ) return;    
    Widget->Blitter = ( Blitter ) ? Blitter : (void *)ScrCopyAlpha;    
}

xFile_t *WinOpenFile( const char *fname, int mode )
{
    xFile_t *f;
    char tmp[16];

    memset( tmp, 0x00, 4 );
    if( mode & 0x01 ){
        tmp[ 0 ] = 'w';
    } else {
        if( mode & 0x10 ){
            tmp[ 0 ] = 'a';
            if( mode & 0x100 ){
                tmp[ 1 ] = 't';
            } else {
		if( mode & 0x200 ) tmp[1] = 'b';
	    }
	    if( !(f = dbOpen( fname, tmp )) ) return (xFile_t *)(-1);
	    return f;
        } else {
    	    tmp[0] = 'r';
        }
    }
    if( mode & 0x100 ){
        tmp[1] = 't';
    } else {
	if( mode & 0x200 ) tmp[1] = 'b';
    }
    if( !(f = dbOpen( fname, tmp )) ) return (xFile_t *)(-1);
    return f;
}

void WinCloseFile( xFile_t *handler )
{
    dbClose( handler );
}

int WinReadFile( xFile_t *handler, void *buff, int size )
{
    return dbread( buff, 1, size, handler );
}

int WinMsgError( const char *msg )
{
    char cmd[ 1024 ];

    printf( "<CRITICAL> Windowed Sys Error:%s\n", msg );
DBG_CRASH
    if( strlen( msg ) > 950 ) return 1;
    // not an elegant way, but it works and is simple, and works before initializing sdl window
    sprintf( cmd, "xmessage -center -font -adobe-*-*-r-*--0-200-*-*-p-*-*-* -center \"%s\"", msg );
    if( !fork() ){
	close( 1 );
	close( 2 );
	system( cmd );
	exit( 0 );
    }    
    return 1;
}

int WinCreateButton( int WinId, int Xpos, int Ypos, int Width, int Height, int HotKey4, int HotKey3, int HotKey2, int HotKey1, char *ImgChk, char *ImgUnChk, char *ImgUnk, int Flags)
{
    Window_t *win;
    Widget_t *wgt;

    win = WinGetWindow( WinId );
    if( !gWinSys.Init || !win ) return -1;
    if( !ImgChk && (ImgUnChk || ImgUnk) ) return -1;
    wgt = WinCreateWidget( WinId, Xpos, Ypos, Width, Height, HotKey4, HotKey3, HotKey2, HotKey1, Flags | 0x10000, ImgChk, ImgUnChk, ImgUnk );
    if( !wgt ) return -1;
    WinDrawWidget( wgt, win, wgt->AnimFrame[0], 0, 0, 0 );
    return wgt->Id;
}

int WinAddButton( int WinId, int PosX, int PosY, int g0, int g1, int g2, int HotKey, char *LblText, int Flags )
{
    Window_t *Parent;
    Widget_t *wg;
    int w, h;
    char *NormalSurf, *PressedSurf;

    Parent = WinGetWindow( WinId );
    if( !gWinSys.Init || !Parent ) return -1;
    w = gFont.LineWidth( LblText ) + 16;
    h = gFont.ChrHeight() + 6;    

    if( !(NormalSurf = Malloc( h * w )) ) return -1;
    if( !(PressedSurf = Malloc( h * w )) ){ Free( NormalSurf ); return -1; }
    
    if( Parent->FontColor == 256 && gWinTexture ){
        ScrFillImg( NormalSurf, w, h, w, gWinTexture, PosX + Parent->RandX + 1, PosY + Parent->RandY + 1 );
        ScrFillImg( PressedSurf, w, h, w, gWinTexture, PosX + Parent->RandX, PosY + Parent->RandY );
    } else {
        ScrFillSolid( NormalSurf, w, h, w, Parent->FontColor );
        ScrFillSolid( PressedSurf, w, h, w, Parent->FontColor );
    }
    // normal state image
    ScrEnlight( NormalSurf, w, h, w);
    gFont.Print( WIN_XY( 8, 3, w, NormalSurf ), LblText, w, w, WIN_FRCOL_C | 0x10000);
    ScrShadowRectangle( NormalSurf, w, 2, 2, w - 3, h - 3, WIN_FRCOL_A, WIN_FRCOL_B );
    ScrShadowRectangle( NormalSurf, w, 1, 1, w - 2, h - 2, WIN_FRCOL_A, WIN_FRCOL_B );
    ScrRectangle( NormalSurf, w, 0, 0, w - 1, h - 1, WIN_FRCOL_ALPHA );
    // pressed state image
    gFont.Print( WIN_XY( 9, 4, w, PressedSurf ), LblText, w, w, WIN_FRCOL_C | 0x10000);
    ScrShadowRectangle( PressedSurf, w, 2, 2, w - 3, h - 3, WIN_FRCOL_B, WIN_FRCOL_A );
    ScrShadowRectangle( PressedSurf, w, 1, 1, w - 2, h - 2, WIN_FRCOL_B, WIN_FRCOL_A );
    ScrRectangle( PressedSurf, w, 0, 0, w - 1, h - 1, WIN_FRCOL_ALPHA );

    if( !(wg = WinCreateWidget( WinId, PosX, PosY, w, h, g0, g1, g2, HotKey, Flags, NormalSurf, PressedSurf, NULL )) ){
        Free( NormalSurf );
        Free( PressedSurf );
        return -1;
    }
    WinDrawWidget( wg, Parent, wg->AnimFrame[0], 0, 0, 0 );
    return wg->Id;
}

int WinSetAnimH( int WdgId, char *PointedWg, char *Img1, char *Img2 )
{
    Widget_t *wdg;

    if( !gWinSys.Init ) return -1;
    if( !(wdg = WinLookup(WdgId, 0)) ) return -1;
    wdg->AnimFrame[ 4 ] = Img1;
    wdg->AnimFrame[ 5 ] = Img2;
    wdg->AnimFrame[ 3 ] = PointedWg;
    return 0;
}

int WinSetAnimL( int WdgId, char *PointedWg, char *Img1, char *Img2, int Hide )
{
    Widget_t *wdg;
    Window_t *win;

    if( !gWinSys.Init || (!PointedWg && (Img1 || Img2)) ) return -1;
    if( !(wdg = WinLookup(WdgId, &win)) )return -1;
    if( (wdg->Flags & 0x10000) == 0 ) return -1;
    if( wdg->CurrentFrame == wdg->AnimFrame[0] ){
        wdg->CurrentFrame = PointedWg;
    } else if ( wdg->CurrentFrame == wdg->AnimFrame[1] ){
        wdg->CurrentFrame = Img1;
    } else if ( wdg->CurrentFrame == wdg->AnimFrame[2] ){
        wdg->CurrentFrame = Img2;
    }
    wdg->AnimFrame[ 0 ] = PointedWg;
    wdg->AnimFrame[ 1 ] = Img1;
    wdg->AnimFrame[ 2 ] = Img2;
    WinDrawWidget( wdg, win, wdg->CurrentFrame, Hide, 0, 0 );
    return 0;
}

int WinSetButtonHandler( int WdgId, void *Cb0, void *Cb1, void *Cb2, void *Cb3 )
{
    Widget_t *wdg;

    if( !gWinSys.Init ) return -1;
    if( !(wdg = WinLookup(WdgId, 0)) ) return -1;
    wdg->Cb1 = Cb1;
    wdg->Cb2 = Cb2;
    wdg->Cb0 = Cb0;
    wdg->Cb3 = Cb3;
    return 0;
}

int WinSetHandler2( int WdgId, int ff0, int ff1, void *Cb0, void *Cb1 )
{
    Widget_t *wdg;

    if( !gWinSys.Init ) return -1;
    if( !(wdg = WinLookup( WdgId, 0 )) ) return -1;
    wdg->HotKeys[5] = ff1;
    wdg->Cb4 = Cb0;
    wdg->HotKeys[4] = ff0;
    wdg->Cb5 = Cb1;
    if( ff0 == -1 && ff1 == -1 && !Cb0 && !Cb1 )
        wdg->Flags &= ~0x080000;
    else
        wdg->Flags |= 0x080000;
    return 0;
}

int WinSetClickSound( int WsgId, int ( *ClickPushCb)(), int ( *ClickReleaseCb)() )
{
    Widget_t *wdg;

    if( !gWinSys.Init ) return -1;
    if( !( wdg = WinLookup( WsgId, 0 )) ) return -1;
    wdg->ClickPlayB = ClickReleaseCb;
    wdg->ClickPlayA = ClickPushCb;
    return 0;
}

int WinSetDefaultImg( int WdgId, char *Img )
{
    Widget_t *wdg;

    if( !gWinSys.Init ) return -1;    
    if( !(wdg = WinLookup( WdgId, 0 )) ) return -1;
    wdg->DefImg = Img;
    return 0;
}

Widget_t *WinCreateWidget( int WinId, int PosX, int PosY, int Width, int Height, int HotKey3, int HotKey2, int HotKey1, int HotKey0, int Flags, char *Frame0, char *Frame1, char *Frame2 )
{
    Widget_t *p;
    Window_t *win;
    int WgId;

    if( !(win = WinGetWindow( WinId )) ) return NULL;    
    if( PosX < 0 || PosY < 0 || (Width + PosX) > WinGetWidth( WinId ) || (PosY + Height) > WinGetHeight( WinId ) ) return NULL;    
    if( !(p = Malloc( sizeof( Widget_t ) )) ) return NULL;    
    if( (Flags & 1) == 0 ){
        if( (Flags & 2) != 0 ) Flags &= ~0x02;
        if( (Flags & 4) != 0 ) Flags &= ~0x04;
    }
    for( WgId = 1; WinLookup( WgId, 0 ); WgId++ ); // find free Id for widget
    p->HotKeys[4] = -1;
    p->HotKeys[5] = -1;
    p->AnimFrame[3] = NULL;
    p->AnimFrame[4] = NULL;
    p->AnimFrame[5] = NULL;
    p->CurrentFrame = NULL;
    p->DefImg = 0;
    p->Cb0 = NULL;
    p->Cb1 = NULL;
    p->Cb2 = NULL;
    p->Cb3 = NULL;
    p->Cb4 = NULL;
    p->Cb5 = NULL;
    p->ClickPlayA = NULL;
    p->ClickPlayB = NULL;
    p->Radio = NULL;
    p->area.lt = PosX;
    p->area.tp = PosY;
    p->area.rt = Width + PosX - 1;
    p->area.bm = Height + PosY - 1;
    p->Id = WgId;
    p->Flags = Flags;
    p->HotKeys[0] = HotKey3;
    p->HotKeys[1] = HotKey2;
    p->HotKeys[2] = HotKey1;
    p->HotKeys[3] = HotKey0;
    p->AnimFrame[0] = Frame0;
    p->AnimFrame[1] = Frame1;
    p->AnimFrame[2] = Frame2;
    p->prev = NULL;
    // link at the end of list
    p->next = win->Widget;
    if( win->Widget ) win->Widget->prev = p;
    win->Widget = p;    
    return p;
}

int WinButtonState( int WdgId )
{
    Widget_t *wdg;

    if( !gWinSys.Init ) return 0;
    if( !(wdg = WinLookup( WdgId, 0 )) ) return 0;
    return (wdg->Flags & 0x01) && (wdg->Flags & 0x20000);
}

int WinProcess( Window_t *Win, int *HotKey )
{
    Window_t *WgE;
    Widget_t *WgA, *WgB, *WgD, *wg, *wgt;
    int MseButt;
    char *a, Flags;
    VidRect_t Rect;
    void (*ExCb)(int, int);

    WgA = Win->PointedWg;
    wg = Win->Widget;
    ExCb = NULL;
    if( Win->Flags & WIN_FLG_CLEAN ) return -1;
    if( WgA ){
        Rect = WgA->area;
        Rect.lt += Win->Geometry.lt;
        Rect.tp += Win->Geometry.tp;
        Rect.rt += Win->Geometry.lt;
        Rect.bm += Win->Geometry.tp;
    } else {
        WgB = Win->Img1;
        if( WgB ){
            Rect = WgB->area;
            Rect.lt += Win->Geometry.lt;
            Rect.tp += Win->Geometry.tp;
            Rect.rt += Win->Geometry.lt;
            Rect.bm += Win->Geometry.tp;
        }
    }
    *HotKey = -1;
    if( MseCursorCenterInArea( Win->Geometry.lt, Win->Geometry.tp, Win->Geometry.rt, Win->Geometry.bm ) ){
        MseButt = MseGetButtons();
        if( (Win->Flags & 0x40) || !(MseButt & MSE_LBUTT) ){
            if( !MseButt ) Win->Img1 = NULL;
        } else {
            WinMoveTop( Win->Id );
        }
        if( WgA ){
            if( !WinGetAreaPointedPixel(WgA, &Rect) ){
                if( (WgA->Flags & 8) == 0 ) *HotKey = WgA->HotKeys[1];
                if( (WgA->Flags & 1) != 0 && (WgA->Flags & 0x20000) != 0 ){
                    a = WgA->AnimFrame[1];
                    WinDrawWidget(WgA, Win, a, 1, 0, 1);
                } else {
                    a = WgA->AnimFrame[0];
                    WinDrawWidget(WgA, Win, a, 1, 0, 1);
                }
                Win->PointedWg = 0;
                Flags = WgA->Flags;
                gWinPointedWg1 = Win->Id;
                if( (Flags & 8) == 0 ){
                    if( WgA->Cb1 ){
                        WgA->Cb1(WgA->Id, *HotKey, a);
                        if( (WgA->Flags & 0x40) == 0 ){ *HotKey = -1; return 0; }
                    }
                }
                return 0;
            }
            wg = WgA;
        } else {
            WgD = Win->Img1;
            if( WgD && WinGetAreaPointedPixel(Win->Img1, &Rect) ){
                WgD = Win->Img1;
                if( (WgD->Flags & 8) == 0 ) *HotKey = WgD->HotKeys[0];
                if( (WgD->Flags & 1) && (WgD->Flags & 0x20000) ){
                    a = WgD->AnimFrame[1];
                    WinDrawWidget(WgD, Win, a, 1, 0, 1);
                } else {
                    a = WgD->AnimFrame[0];
                    WinDrawWidget(WgD, Win, a, 1, 0, 1);
                }
                Win->PointedWg = WgD;
                gWinPointedWg1 = Win->Id;
                if( (WgD->Flags & 8) == 0 ){
                    if( WgD->Cb0 ){
                        WgD->Cb0(WgD->Id, *HotKey, a);
                        if( (WgD->Flags & 0x40) == 0 ){ *HotKey = -1; return 0; }
                    }
                }
                return 0;
            }
        }
        if( gWinPointedWg1 != -1 ){
            if( gWinPointedWg1 != Win->Id ){
                WgE = WinGetWindow( gWinPointedWg1 );
                if( WgE ){
                    gWinPointedWg1 = -1;
                    if( WgE->PointedWg ){
                        if( (WgE->PointedWg->Flags & 8) == 0 ) *HotKey = WgE->PointedWg->HotKeys[1];
                        if( (WgE->PointedWg->Flags & 1) && (WgE->PointedWg->Flags & 0x20000) != 0 ){
                            a = WgE->PointedWg->AnimFrame[1];
                            WinDrawWidget(WgE->PointedWg, WgE, a, 1, 0, 1);
                        } else {
                            a = WgE->PointedWg->AnimFrame[0];
                            WinDrawWidget(WgE->PointedWg, WgE, a, 1, 0, 1);
                        }
                        WgD = WgE->PointedWg;
                        WgE->Img1 = 0;
                        WgE->PointedWg = NULL;
                        if( (WgD->Flags & 8) == 0 ){
                            if( WgD->Cb1 ){
                                WgD->Cb1( WgD->Id, *HotKey, a );
                                if( (WgD->Flags & 0x40) == 0 ){ *HotKey = -1; return 0; }
                            }
                        }
                        return 0;
                    }
                }
            }
        }
        for( ; wg;wg = wg->next ){
            if( (wg->Flags & 8) == 0 ){
                Rect = wg->area;
                Rect.lt += Win->Geometry.lt;
                Rect.tp += Win->Geometry.tp;
                Rect.rt += Win->Geometry.lt;
                Rect.bm += Win->Geometry.tp;
                if( !WinGetAreaPointedPixel(wg, &Rect) ) continue;
    		if( !(wg->Flags & 0x08) )
    		{
        	    if( (MseButt & ( MSE_LBUTT | MSE_RBUTT )) ){ // button pressed right or left
            		if( (MseButt & MSE_RBUTT ) && (wg->Flags & 0x80000) == 0 ){ wg = NULL; break;  } // right mouse button pressed
            		if( (wg != Win->PointedWg) && (wg != Win->Img1) ) break;
            		Win->Img1 = Win->PointedWg = wg;
            		if( wg->Flags & 0x01 ){
                	    if( wg->Flags & 0x02 ){
                    		if( (wg->Flags & 0x20000) ){
                        	    if( !(wg->Flags & 0x04) ){
                            		if( wg->Radio ) wg->Radio->State--;
                            		if( MseButt & 1 ){ *HotKey = wg->HotKeys[3]; ExCb = wg->Cb3; } else { *HotKey = wg->HotKeys[5]; ExCb = wg->Cb5; }
                            		wg->Flags &= ~0x20000;
                        	    }
                    		} else {
                        	    if( WinRadioClicked( wg ) == -1 ){ wg = NULL; break; }
                        	    if( MseButt & 1 ){ *HotKey = wg->HotKeys[2]; ExCb = wg->Cb2; } else { *HotKey = wg->HotKeys[4]; ExCb = wg->Cb4; }
                        	    wg->Flags |= 0x20000;
                    		}
                	    }
            		} else {
                	    if( WinRadioClicked( wg ) == -1 ){ wg = NULL; break; }
                	    if( MseButt & MSE_LBUTT ){ *HotKey = wg->HotKeys[2]; ExCb = wg->Cb2; } else { *HotKey = wg->HotKeys[4]; ExCb = wg->Cb4; }
            		}
            		WinDrawWidget(wg, Win, wg->AnimFrame[1], 1, 0, 1);
            		break;
        	    }
        	    if( (wg == Win->Img1) && (MseButt & (MSE_LBHOLD | MSE_RBHOLD) ) ){ // click pointed widget
        		wgt = Win->Img1;
            		Win->Img1 = NULL;
            		Win->PointedWg = wgt;
            		if( wgt->Flags & 1 ){
                	    if( (wgt->Flags & 2) == 0 ){
                    		if( wgt->Flags & 0x20000 ){
                        	    if( (wgt->Flags & 4) == 0 ){
                            		if( wgt->Radio ) wg->Radio->State--;
                            		if( MseButt & 0x10 ){ *HotKey = wg->HotKeys[3]; ExCb = wg->Cb3; } else { *HotKey = wg->HotKeys[5]; ExCb = wg->Cb5; }
                            		wg->Flags &= ~0x20000;
                        	    }
                    		} else {
                        	    if( WinRadioClicked(wgt) == -1 ){
                            		WinDrawWidget(wgt, Win, wgt->AnimFrame[0], 1, 0, 1);
                            		break;
                        	    }
                        	    if( wgt->Flags & 0x10 ){ *HotKey = wgt->HotKeys[2]; ExCb = Win->Img1->Cb2; } else { *HotKey = wgt->HotKeys[4]; ExCb = wgt->Cb4; }
                        	    wg->Flags |= 0x20000;
                    		}
                	    }
            		} else {
                	    if( wgt->Flags & 0x20000 ){
                    		if( wgt->Radio ) wgt->Radio->State--;
                	    }
                	    if( MseButt & MSE_LBHOLD ){ 
                		*HotKey = wg->HotKeys[ 3 ]; 
                		ExCb = wg->Cb3; 
                	    } else { 
                		*HotKey = wg->HotKeys[ 5 ];
                		ExCb = wg->Cb5; 
                	    }
            		}
            		WinDrawWidget( wg, Win, wg->AnimFrame[ wg->AnimFrame[ 2 ] ? 2:0 ], 1, 0, 1 );
            		break;
        	    }
    		}
		// set focus on widget
    		if( !Win->PointedWg && !MseButt ){
        	    Win->PointedWg = wg;
        	    if( !(wg->Flags & 0x08) ){ *HotKey = wg->HotKeys[0]; ExCb = wg->Cb0; }
        	    WinDrawWidget( wg, Win, wg->AnimFrame[ 2 ], 1, 0, 1 );
    		}
    		break;
	    } 
        }
	if( wg ){
    	    if( (wg->Flags & 0x10) && (MseButt & (MSE_LBUTT | MSE_RBUTT) ) && !(MseButt & ( MSE_LBUTT_HOLD | MSE_RBUTT_HOLD )) ){
        	WinDragAndMove( Win->Id );
        	WinDrawWidget(wg, Win, wg->AnimFrame[0], 1, 0, 1);
    	    }
	} else if( (Win->Flags & 0x80) && (MseButt & (MSE_LBUTT | MSE_RBUTT)) != 0 && (MseButt & ( MSE_LBUTT_HOLD | MSE_RBUTT_HOLD )) == 0 ){
    	    WinDragAndMove( Win->Id );
	}
	gWinPointedWg1 = Win->Id;
	if( wg ){
    	    if( ExCb ){
        	ExCb( wg->Id, *HotKey );
        	if( (wg->Flags & 0x40) == 0 ) *HotKey = -1;
    	    }
	}
	return 0;
    }
    if( WgA ){
	*HotKey = WgA->HotKeys[1];
	if( (WgA->Flags & 1) != 0 && (WgA->Flags & 0x20000) != 0 ){
	    a = WgA->AnimFrame[1];
            WinDrawWidget(WgA, Win, a, 1, 0, 1);
	} else {
    	    a = WgA->AnimFrame[0];
    	    WinDrawWidget(WgA, Win, a, 1, 0, 1);
	}
    	Win->PointedWg = NULL;
    }
    if( *HotKey != -1 ){
    	gWinPointedWg1 = Win->Id;
    	if( (WgA->Flags & 0x08) ){ *HotKey = -1; return 0; }
    	if( !WgA->Cb1 ) return 0;
    	WgA->Cb1( WgA->Id, *HotKey, a );
    	if( !(WgA->Flags & 0x40) ){ *HotKey = -1; return 0; }
    }
    if( WgA && (WgA->Flags & 8) == 0 ){
    	if( WgA->Cb1 ) WgA->Cb1(WgA->Id, *HotKey, a );
    }
    return -1;
}

int WinGetAreaPointedPixel( Widget_t *Wg, VidRect_t *Rect )
{
    int r;
    int y, x;

    if( !(r = MseCursorCenterInArea( Rect->lt, Rect->tp, Rect->rt, Rect->bm )) ) return 0;    
    if( !Wg->DefImg ) return r;
    MseGetCursorPosition( &x, &y );
    x -= Rect->lt;
    y -= Rect->tp;
    return Wg->DefImg[ x + y * ( Wg->area.rt - Wg->area.lt + 1) ];
}

int WinImageGetWinId( int ImageId )
{
    Window_t *win;

    if( !gWinSys.Init ) return -1;
    if( WinLookup( ImageId, &win ) ) return win->Id;
    return -1;
}

int WinGetPointed()
{
    return gWinPointedWg1;
}

int WinImageRemove( int ImageId )
{
    Widget_t *pImg;
    Widget_t *prev;
    Widget_t *next;
    Window_t *win;

    if( !gWinSys.Init ) return -1;
    if( !(pImg = WinLookup(ImageId, &win)) ) return -1;
    prev = pImg->prev;
    if( prev )
        prev->next = pImg->next;
    else
        win->Widget = pImg->next;
    next = pImg->next;
    if( next ) next->prev = pImg->prev;
    WinDrawFilledRect(win->Id, pImg->area.lt, pImg->area.tp, pImg->area.rt - pImg->area.lt + 1, pImg->area.bm - pImg->area.tp + 1, win->FontColor);
    if( pImg == win->PointedWg ) win->PointedWg = NULL;
    if( pImg == win->Img1 ) win->Img1 = NULL;
    WinWidgetsFree( pImg );
    return 0;
}

void WinWidgetsFree( Widget_t *wdg )
{
    int i, j;

    if( (wdg->Flags & 0x10000) == 0 ){
        if( wdg->AnimFrame[ 0 ] ) Free( wdg->AnimFrame[ 0 ] );
        if( wdg->AnimFrame[ 1 ] ) Free( wdg->AnimFrame[ 1 ] );
        if( wdg->AnimFrame[ 2 ] ) Free( wdg->AnimFrame[ 2 ] );
        if( wdg->AnimFrame[ 3 ] ) Free( wdg->AnimFrame[ 3 ] );
        if( wdg->AnimFrame[ 4 ] ) Free( wdg->AnimFrame[ 4 ] );
        if( wdg->AnimFrame[ 5 ] ) Free( wdg->AnimFrame[ 5 ] );
    }

    if( wdg->Radio ){
        for( i = 0; i < wdg->Radio->Count; i++  ){
            if( wdg != wdg->Radio->Wgs[ i ] ) continue;
            for( j = i; j < wdg->Radio->Count; j++ ){
                wdg->Radio->Wgs[ j ] = wdg->Radio->Wgs[ j + 1 ];
            }
            wdg->Radio->Count--;
            break;
        }
    }
    Free( wdg );
}

void WinCloseWithKey( int WdgId, int Key )
{
    Window_t *win;

    if( WinLookup(WdgId, &win) ){
        WinClose(win->Id);
        InpPushBuffer(Key);
    }
}

int WinGetEmpty()
{
    int i;

    for( i = 1; WinLookup(i, 0); i++ );
    return i;
}

int WinEnableWidget( int WgId )
{
    Widget_t *wg;
    Window_t *Win;

    if( !gWinSys.Init ) return -1;
    if( !(wg = WinLookup( WgId, &Win )) ) return -1;
    if( wg->Flags & 0x08 ){
        wg->Flags &= ~0x08;
        WinDrawWidget( wg, Win, wg->CurrentFrame, 1, 0, 0 );
    }
    return 0;
}

int WinDisableWidget( int WgId )
{
    Widget_t *wg;
    Window_t *Win;

    if( !gWinSys.Init ) return -1;
    if( !(wg = WinLookup( WgId, &Win )) ) return -1;
    if( !(wg->Flags & 0x08) ){
        wg->Flags |= 0x08;
//        WinDrawWidget( wg, Win, wg->CurrentFrame, 1, 0, 0);
//        if( wg == Win->PointedWg ){
//            if( Win->PointedWg->HotKeys[1] != -1 ) InpPushBuffer( Win->PointedWg->HotKeys[1] );
//            Win->PointedWg = NULL;
//        }
    }
    return 0;
}

int WinButtonSet( int Id, int Flg1, int Flg2 )
{
    int Key;
    Widget_t *wg;
    Window_t *win;

    Key = -1;
    if( !gWinSys.Init ) return -1;
    if( !(wg = WinLookup( Id, &win )) ) return -1;
    if( (wg->Flags & 1) != 0 ){
        if( wg->Flags & 0x20000 ){
            if( !Flg1 ){
                wg->Flags &= ~0x20000;
                if( !(Flg2 & 0x02) ) WinDrawWidget( wg, win, wg->AnimFrame[0], 1, 0, 0 );
                Key = wg->HotKeys[ 3 ];
                if ( wg->Radio ) wg->Radio->State--;
            }
        } else if( Flg1 ){
    	    wg->Flags |= 0x20000;
            if( !( Flg2 & 0x02 ) ) WinDrawWidget( wg, win, wg->AnimFrame[1], 1, 0, 0 );
            Key = wg->HotKeys[ 2 ];
	    if( wg->Radio ) wg->Radio->State++;
        }
        if( (Key != -1) && (Flg2 & 0x01) ) InpPushBuffer( Key );
    }
    return 0;
}

int WinRadioButtonAssign( int Num, int *Buttons, int ArgA, void *Cb )
{
    int i,j;
    Widget_t *wg;

    if( !gWinSys.Init || Num > 64 ) return -1;
    // looking for free slot
    for( i = 0; gWinRadioButtons[ i ].Count && i < 64; i++ );
    gWinRadioButtons[ i ].State = 0;

    for( j = 0; j < Num; j++ ){
        if( !(wg = WinLookup( Buttons[ j ], 0 ) ) ) return -1;
        gWinRadioButtons[ i ].Wgs[ j ] = wg;
        wg->Radio = &gWinRadioButtons[ i ];
        if( (wg->Flags & 0x20000) != 0 ) gWinRadioButtons[ i ].State++;
    }            
    gWinRadioButtons[ i ].Count = Num;
    gWinRadioButtons[ i ].ArgA = ArgA;
    gWinRadioButtons[ i ].Cb = Cb;
    return 0;    
}

void WinRadioButton( int Count, int *Buttons )
{
    Widget_t *wg;
    int i;

    if( gWinSys.Init && WinRadioButtonAssign( Count, Buttons, 1, 0 ) == -1 ) return;
    
    wg = WinLookup( Buttons[ 0 ], 0 );
    for( i = 0; i < wg->Radio->Count; i++ ){
	wg->Radio->Wgs[ i ]->Flags |= 0x40000;
    }
}

int WinRadioClicked( Widget_t *wg )
{
    int i, WgtId;
    Widget_t *wge;    
    Window_t *win;

    if( !wg->Radio ) return 0;
    if( (wg->Flags & 0x40000) != 0 ){
        if( wg->Radio->State > 0 ){
            for( i = 0; i < wg->Radio->Count; i++ ){
                wge = wg->Radio->Wgs[ i ];
                if( wge->Flags & 0x20000 ){
                    WgtId = wge->Id;
                    wge->Flags &= ~0x20000;
                    WinLookup( WgtId, &win );
                    WinDrawWidget( wge, win, wge->AnimFrame[0], 1, 0, 1 );
                    if( wge->Cb3 ) wge->Cb3();
                }
            }
        }
    } else {
	if( wg->Radio->State >= wg->Radio->ArgA ){
	    if( wg->Radio->Cb ) wg->Radio->Cb();
	    return -1;
	}
    }
    if( !(wg->Flags & 0x20000) ) wg->Radio->State++;
    return 0;
}

void WinDrawWidget( Widget_t *Wdt, Window_t *Win, char *pSrcPic, int Hide, VidRect_t *Position, int Update )
{
    char *PrevFrame;
    int lt, tp, SrcPitch;
    VidRect_t area, pos;

    PrevFrame = 0;
    if( pSrcPic ){
        pos = Wdt->area;
        pos.lt += Win->Geometry.lt;
        pos.tp += Win->Geometry.tp;
        pos.rt += Win->Geometry.lt;
        pos.bm += Win->Geometry.tp;
        if( Position ){
            if( RegionShrink(&pos, Position, &pos) == -1 ) return;
            lt = Win->Geometry.lt;
            tp = Win->Geometry.tp;
            area.lt = pos.lt - lt;
            area.tp = pos.tp - tp;
            area.rt = pos.rt - lt;
            area.bm = pos.bm - tp;
        } else {
            area = Wdt->area;
        }
        if( pSrcPic == Wdt->AnimFrame[0] && (Wdt->Flags & 0x20000) ) pSrcPic = Wdt->AnimFrame[1];
        if( Wdt->Flags & 8 ){
            if( pSrcPic == Wdt->AnimFrame[0] ){
                pSrcPic = Wdt->AnimFrame[3];
            } else if( pSrcPic == Wdt->AnimFrame[1] ){
                pSrcPic = Wdt->AnimFrame[4];
            } else if( pSrcPic == Wdt->AnimFrame[2] ){
                pSrcPic = Wdt->AnimFrame[5];
            }
        } else if( pSrcPic == Wdt->AnimFrame[3] ){
            pSrcPic = Wdt->AnimFrame[0];
        } else if( pSrcPic == Wdt->AnimFrame[4] ){
            pSrcPic = Wdt->AnimFrame[1];
        } else if( pSrcPic == Wdt->AnimFrame[5] ){
            pSrcPic = Wdt->AnimFrame[2];
        }
        if( pSrcPic ){
            if( !Hide ){
                SrcPitch = Wdt->area.rt - Wdt->area.lt + 1;
                if( Wdt->Flags & 0x20 )
                    ScrCopyAlpha(&pSrcPic[(area.tp - Wdt->area.tp) * SrcPitch + area.lt - Wdt->area.lt], area.rt - area.lt + 1, area.bm - area.tp + 1, SrcPitch, &Win->Surface[area.lt + Win->Width * area.tp], Win->Width );
                else
                    ScrCopy(&pSrcPic[(area.tp - Wdt->area.tp) * SrcPitch + area.lt - Wdt->area.lt], area.rt - area.lt + 1, area.bm - area.tp + 1, SrcPitch, &Win->Surface[area.lt + Win->Width * area.tp], Win->Width);
            }
            PrevFrame = Wdt->CurrentFrame;
            Wdt->CurrentFrame = pSrcPic;
            if( Hide ) WinRedraw( Win, &pos, 0 );
        }
    }
    if( Update && PrevFrame != pSrcPic ){
        if( pSrcPic == Wdt->AnimFrame[1] && Wdt->ClickPlayA ){
            Wdt->ClickPlayA();
        } else if( pSrcPic == Wdt->AnimFrame[0] ){
            if( Wdt->ClickPlayB ) Wdt->ClickPlayB();
        }
    }
}

void WinDrawCursor( Window_t *Video, VidRect_t *Area )
{
    Widget_t *p;

    for( p = Video->Widget; p; p = p->next ){
        if( !p->next ) break;
    }
    while( p ){
        WinDrawWidget( p, Video, p->CurrentFrame, 0, Area, 0 );
        p = p->prev;
    }
}

int WinUnk970( int WgId )
{
    Widget_t *wg;
    Window_t *win;

    if( !gWinSys.Init ) return -1;
    if( !(wg = WinLookup( WgId, &win )) ) return -1;

    WinDrawWidget( wg, win, wg->AnimFrame[1], 1, 0, 1 );
    if( wg->Cb2 ){
        wg->Cb2();
        if( wg->Flags & 0x40 ) InpPushBuffer( wg->HotKeys[2] );        
    } else {
        if( wg->HotKeys[2] != -1 ) InpPushBuffer( wg->HotKeys[2] );        
    }

    WinDrawWidget( wg, win, wg->AnimFrame[0], 1, 0, 1 );
    if( wg->Cb3 ){
        wg->Cb3();
        if( wg->Flags & 0x40 ) InpPushBuffer( wg->HotKeys[3] );        
    } else { 
	if( wg->HotKeys[3] != -1 ) InpPushBuffer( wg->HotKeys[3] );
    }

    return 0;
}

