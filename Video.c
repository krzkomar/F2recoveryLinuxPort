#include "FrameWork.h"
#include <SDL.h>

SDL_Rect      gUpdateRect;

int gVidScaleX = 4;
int gVidScaleY = 4;
int gVidUpdateForbid = 0;

SDL_Surface *gSDLSurfaceCur = NULL;
SDL_Surface *gSDLSurfaceMain = NULL;
SDL_Surface *gSDLSurfaceBckp = NULL;
//SDL_Palette *gSDLPalette = NULL;
int gSDLWindow = 0;
SDL_Window *gWindow = NULL;
SDL_Renderer *gSDLRenderer = NULL;
SDL_Rect      gSDLRect;
SDL_Texture  *gSDLTexture = NULL;

SDL_Palette *gSDLPalette = NULL;

void (*gVidColorCb)() = NULL;
int gUseMMX = 0;

// ** 2 zmienne pomiedzy **
//short *gpVidPalx;		// pointer to 16bit palette
Pal8_t gVidPalx[ 256 ];		// 16 bit palette
VidRect_t gVidMainGeo;
// color mask in 16 bit word
int gVidMaskGx;
int gVidMaskRx;
int gVidMaskBx;
// shift bit of 16 bit colors +/- -> position in 16bit word
int gVidColShiftBx;
int gVidColShiftRx;
int gVidColShiftGx;
void(*gVidCopyA)( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int posX, int posY );
void(*gVidCls)();
int gMMXon;

void VidPaletteUpdate( );

void VidSetMMX( int AllowMMX )
{
    static int MMXinit = 0;
    
    if( !MMXinit ){
        gMMXon = 0;//CpuHaveMMX();
        MMXinit = 1;
    }
    if( gMMXon ) gUseMMX = AllowMMX;
}

int VidInit320x200x8bpp()
{
    return VidInit(320, 200, 8);
}

int VidInit320x240x8bpp()
{
    return VidInit( 320, 240, 8 );
}

int VidInit320x400x8bpp()
{
    return VidInit( 320, 400, 8 );
}

int VidInitDummy()
{
    return -1;
}

int VidInit640x480x8bpp()
{

    return VidInit8bpp( 640, 480 );
}

int VidInit640x480x16bpp()
{
    return VidInit( 640, 480, 16 );
}

int VidInit640x400x8bpp()
{
    return VidInit8bpp( 640, 400 );
}

int VidInit800x600x8bpp()
{
    return VidInit8bpp( 800, 600 );
}

int VidInit1024x768x8bpp()
{
    return VidInit8bpp( 1024, 768 );
}

int VidInit1280x1024x8bpp()
{
    return VidInit8bpp( 1280, 1024 );
}

void VidUnkSub1( int a1, int a2 )
{
    VidInit( a1, 0, 8);
}

void VidCls()
{
    if( gVidCls ) gVidCls();
}

void VidSetColorCb( void (*Cb)(void) )
{
    gVidColorCb = Cb;
}

void (*VidGetColorCb())(void)
{
    return gVidColorCb;
}

int VidInit( int Width, int Height, int Bpp )
{
    if( VidCreateMainWindow() == -1) return -1;

    if( VidInitMode( Width, Height, Bpp ) == -1 ) return -1;
    gSDLWindow = 1;
    gVidMainGeo.lt = 0;
    gVidMainGeo.tp = 0;
    gVidMainGeo.rt = Width - 1;
    gVidMainGeo.bm = Height - 1;
    VidSetMMX( 1 );
//    if( Bpp == 8 ){
//        gVidCls = VidClearScreen;
//        gMseBlit = VidCopy8;
//        gMseBlitAlpha = NULL;
//        gVidCopyA = VidCopy8;
//    } else {
	gVidCls = NULL;
        gMseBlit = (void *)VidCopy16;
        gMseBlitAlpha = (void *)VidCopyAlpha16;
        gVidCopyA = VidCopy16A;
//    }
    gUpdateRect.w = Width;
    gUpdateRect.h = Height;
    gUpdateRect.x = 0;
    gUpdateRect.y = 0;    
    return 0;
}

int VidInit8bpp( int ScreenWidth, int ScreenHeight )
{
    return VidInit( ScreenWidth, ScreenHeight, 8 );
}

int VidCreateMainWindow()
{
    if( gWindow ) return 0;    
    if( !(gWindow = SDL_CreateWindow( WIN_TITLE, 0, 0, gVidScaleX*640, gVidScaleY*480, 0/*SDL_WINDOW_FULLSCREEN_DESKTOP*/ )) ) return -1;
    return 0;
}

int VidInitMode( int Width, int Height, int Bpp )
{
    int inited;
    Pal8_t *Pal;
    int i;
    SDL_Color PaletteGray[ 256 ];
    SDL_DisplayMode mode;

    if( gSDLWindow ){    
	Pal = VidCreateDimmedPalette();
	VidClose();
        if( !(inited = VidInitMode( Width, Height, Bpp )) ) VidSetPaletteAll( Pal );
	return inited;
    }

    mode.format = (Bpp == 8) ? SDL_PIXELFORMAT_INDEX1LSB : SDL_PIXELFORMAT_RGBA32;
    mode.w = Width;
    mode.h = Height;
    mode.refresh_rate = 0;
    mode.driverdata = NULL;
    gSDLRect.x = 0;
    gSDLRect.y = 0;
    gSDLRect.w = Width * gVidScaleX;
    gSDLRect.h = Height * gVidScaleY;
    if( SDL_SetWindowDisplayMode( gWindow, &mode ) ) return -1;
    if( !(gSDLSurfaceMain = SDL_CreateRGBSurfaceWithFormat( 0, Width, Height, 32, SDL_PIXELFORMAT_RGBA32 )) ){
	eprintf( "<ERROR> Creating SDL main surface error !! '%s'\n", SDL_GetError() );
	return -1;
    }
    if( !(gSDLRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/ ) ) ){
	eprintf( "<ERROR> Creating SDL renderer error !! '%s'\n", SDL_GetError() );
	return -1;
    }
    if( !(gSDLSurfaceCur = SDL_CreateRGBSurface( 0, 640, 480, 8, 0, 0, 0, 0 )) ){
	eprintf( "<ERROR> Creating SDL in game surface error !! '%s'\n", SDL_GetError() );
	return -1;
    }
    if( !(gSDLPalette = SDL_AllocPalette( 256 )) ){
	eprintf( "<ERROR> Creating SDL palette error !! '%s'\n", SDL_GetError() );
	return -1;
    }    
    if( SDL_SetSurfacePalette( gSDLSurfaceCur, gSDLPalette ) < 0 ){
	eprintf( "<ERROR> Creating SDL in game surface error !! '%s'\n", SDL_GetError() );
	return -1;
    }    
    if(!(gSDLTexture = SDL_CreateTexture( gSDLRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, mode.w, mode.h ))){
	eprintf( "<ERROR> Creating SDL texture error !! '%s'\n", SDL_GetError() );
	return -1;
    }

    // create greyscale 8bit default palette
    for( i = 0; i < 256; i++ ){
        PaletteGray[ i ].g = i;
        PaletteGray[ i ].b = i;
        PaletteGray[ i ].r = i;
        PaletteGray[ i ].a = 0;
    }
    SDL_SetPaletteColors( gSDLPalette, PaletteGray, 0, 256 );
    
//    if( Bpp == 8 ){
//	if( !(gSDLPalette = SDL_AllocPalette( 256 )) ) return -1;
//	if( !SDL_SetPaletteColors( gSDLPalette, PaletteGray, 0, 256 ) ) return 0;
//    } else {
//        gSDLPalette = NULL;
//        if( !gSDLSurfaceCur->format ){
    	    gVidMaskRx = gSDLSurfaceCur->format->Rmask;
    	    gVidMaskGx = gSDLSurfaceCur->format->Gmask;
            gVidMaskBx = gSDLSurfaceCur->format->Bmask;
            gVidColShiftRx = gSDLSurfaceCur->format->Rshift;
            gVidColShiftGx = gSDLSurfaceCur->format->Gshift;
            gVidColShiftBx = gSDLSurfaceCur->format->Bshift;
            return 0;
//        }
//    }
    return -1;
}

void VidClose()
{
    if( !gSDLWindow ) return;
    if( gSDLSurfaceCur ){
        SDL_FreeSurface( gSDLSurfaceCur );
        gSDLSurfaceCur = NULL;
        gSDLSurfaceBckp = NULL;
    }
    if( gSDLPalette ){
        SDL_FreePalette( gSDLPalette );
        gSDLPalette = NULL;
    }
    gSDLWindow = 0;
}

void VidSetPaletteIndex( int ColorIndex, char Rr, char Gg, char Bb )
{
    Gg *= COLOR_SCALE;
    Rr *= COLOR_SCALE;
    Bb *= COLOR_SCALE;
    gVidPalx[ ColorIndex ].r = Rr;
    gVidPalx[ ColorIndex ].g = Gg;
    gVidPalx[ ColorIndex ].b = Bb;
    VidPaletteUpdate( );
    WinUpdateArea( &gVidMainGeo );

    if( gVidColorCb ) gVidColorCb();
}

void VidSetPaletteRange( Pal8_t *Pal, unsigned int FirstColor, unsigned int nColors )
{
    int i;
    
    for( i = FirstColor; i < (FirstColor + nColors); i++ ){
        gVidPalx[ i ].r = COLOR_SCALE * Pal[ i ].r;
        gVidPalx[ i ].g = COLOR_SCALE * Pal[ i ].g;
        gVidPalx[ i ].b = COLOR_SCALE * Pal[ i ].b;
    }

    VidPaletteUpdate( );
    WinUpdateArea( &gVidMainGeo );
    if( gVidColorCb ) gVidColorCb();
}

int VidGetPaletteColor( int ColorIndex, char *pR, char *pG, char *pB )
{
    if( ColorIndex < 0 || ColorIndex >= gSDLPalette->ncolors ) return -1;
    *pR = gVidPalx[ ColorIndex ].r >> 2;
    *pG = gVidPalx[ ColorIndex ].r >> 2;
    *pB = gVidPalx[ ColorIndex ].r >> 2;
    return 0;
}

void VidSetPaletteAll( Pal8_t *Pal )
{
    int i;

    for( i = 0; i < 256; i++, Pal++ ){
        gVidPalx[ i ].r = COLOR_SCALE * Pal->r;
        gVidPalx[ i ].g = COLOR_SCALE * Pal->g;
        gVidPalx[ i ].b = COLOR_SCALE * Pal->b;
//        gVidPalx[ i ].a = COLOR_SCALE;
    }
    VidPaletteUpdate( );
//    WinUpdateArea( &gVidMainGeo );
    VidUpdate( 0, 0, 640, 480 );
    if( gVidColorCb ) gVidColorCb();
}

Pal8_t *VidCreateDimmedPalette()
{
    int i;
    static Pal8_t DimPal8[ 256 ];

    for( i = 0; i < 256; i++ ){
        DimPal8[ i ].g = gSDLPalette->colors[ i ].r / COLOR_SCALE;
        DimPal8[ i ].b = gSDLPalette->colors[ i ].g / COLOR_SCALE;
        DimPal8[ i ].r = gSDLPalette->colors[ i ].b / COLOR_SCALE;
    }
    return DimPal8;
}

void VidUpdate( int x, int y, int Width, int Height )
{        
    if( gVidUpdateForbid ){
	if( Width > gUpdateRect.w ) gUpdateRect.w = Width;
	if( Height > gUpdateRect.h ) gUpdateRect.h = Height;
	if( x > gUpdateRect.x ) gUpdateRect.x = x;
	if( y > gUpdateRect.y ) gUpdateRect.y = y;
	return;
    }

    SDL_BlitSurface( gSDLSurfaceCur, NULL, gSDLSurfaceMain, NULL );
    SDL_UpdateTexture( gSDLTexture, NULL, gSDLSurfaceMain->pixels, gSDLSurfaceMain->pitch );
    SDL_RenderCopy( gSDLRenderer, gSDLTexture, NULL, NULL );
    SDL_RenderPresent( gSDLRenderer );
}


void VidCopy( void *buff, Pal8_t *pal, int pitch, int x, int y, int w, int h, int bpp )
{
    int i, j;
    char *pp, *tmp;
    Pal8_t *p;
    unsigned char rr;

//SDL_BlitScaled()
    SDL_LockSurface( gSDLSurfaceMain );
    if( bpp == 8 ){
	pp = tmp = gSDLSurfaceMain->pixels;
	for( j = 0; j < h; j++ ){
	    tmp = pp;
	    for( i = 0; i < w; i++, tmp += 4, buff++ ){
		p = &pal[ *(unsigned char*)buff ];
		if( p == 0 ) continue;
		tmp[ 0 ] = p->b;
		tmp[ 1 ] = p->g;
		tmp[ 2 ] = p->r;
		tmp[ 3 ] = 0xff;
	    }
	    pp += pitch * 4;
	}
    }

    if( bpp == 16 ){
	pp = tmp = gSDLSurfaceMain->pixels;
	for( j = 0; j < h; j++ ){
	    tmp = pp;
	    for( i = 0; i < w; i++, tmp +=4, buff += 2 ){
		rr = *((unsigned short *)buff);
		tmp[ 0 ] = ((rr) & 0x1f) << 3;
		tmp[ 1 ] = ((rr >> 5) & 0x1f) << 3;
		tmp[ 2 ] = ((rr >> 10) & 0x1f) << 3;
		tmp[ 3 ] = 0xff;
	    }
	    pp += pitch * 4;
	}
    }        
    
    SDL_UnlockSurface( gSDLSurfaceMain );

    SDL_UpdateTexture( gSDLTexture, NULL, gSDLSurfaceMain->pixels, gSDLSurfaceMain->pitch );
    SDL_RenderCopy( gSDLRenderer, gSDLTexture, NULL, NULL );
    SDL_RenderPresent( gSDLRenderer );    
}

int VidUpdateAll()
{
    VidUpdate( 0, 0, gSDLSurfaceCur->w, gSDLSurfaceCur->h );
    return 0;
}

void VidCopy8( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int DstX, int DstY )
{
    if( !gSDLReady ) return;
    VidCopy16( pSrc, SrcPitch, Height, SrcX, SrcY, Width, Height, DstX, DstY );
}

void VidCopy16( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int posX, int posY )
{
    char *Screen;
    char *psrc;

    if( !gSDLReady ) return;
    Screen = gSDLSurfaceCur->pixels + gSDLSurfaceCur->pitch * posY + posX;
    psrc = pSrc + SrcPitch * SrcY + SrcX;
    for( ;Height; psrc += SrcPitch, Height--, Screen += gSDLSurfaceCur->pitch ){
        memcpy( Screen, psrc, Width );
    }
    VidUpdate( posX, posY, Width, Height );
}

void VidCopy16A( char *pSrc, int SrcW, int SrcH, int SrcX, int SrcY, int Width, short Height, int posX, int posY )
{
    VidCopy16( pSrc, SrcW, SrcH, SrcX, SrcY, Width, Height, posX, posY );
}

void VidCopyAlpha16( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int posX, int posY, short Alpha )
{
    char *s;
    char *Screen;
    char *psrc, *ps;
    int i;

    if( !gSDLReady ) return;
    Screen = gSDLSurfaceCur->pixels + gSDLSurfaceCur->pitch * posY + posX;
    psrc = pSrc + SrcX + SrcPitch * SrcY;
    for( ;Height; psrc += SrcPitch, Height--, Screen += gSDLSurfaceCur->pitch ){
	ps = psrc;
	s = Screen;
	for( i = 0; i < Width; i++, s++, ps++ ){
	    if( *ps ) *s = *ps;
	}
    }
    VidUpdate( posX, posY, Width, Height );
}

void VidClearScreen()
{
    SDL_RenderClear( gSDLRenderer );
}

void VidPaletteUpdate( )
{
    static SDL_Color tmp[ 256 ];
    int i;

    for( i = 0; i < 256; i++){
	tmp[i].r = gVidPalx[ i ].r;
	tmp[i].g = gVidPalx[ i ].g;
	tmp[i].b = gVidPalx[ i ].b; 
	tmp[i].a = 0;
    }
    SDL_SetPaletteColors( gSDLPalette, tmp, 0, 256 );
}

void VidUpdateClr()
{
    gUpdateRect.w = 0;
    gUpdateRect.h = 0;
    gUpdateRect.x = 0;
    gUpdateRect.y = 0;        
}

void VidUpdateOpt()
{
    VidUpdate( gUpdateRect.x, gUpdateRect.y, gUpdateRect.w, gUpdateRect.h );
}

