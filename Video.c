#include "FrameWork.h"
#include <SDL.h>

SDL_Surface  *gSDLSurfaceCur = NULL;
SDL_Surface  *gSDLSurfaceMain = NULL;
SDL_Surface  *gSDLSurfaceBckp = NULL;
SDL_Window   *gWindow = NULL;
SDL_Renderer *gSDLRenderer = NULL;
SDL_Rect      gSDLRect;
SDL_Texture  *gSDLTexture = NULL;
SDL_Palette  *gSDLPalette = NULL;

int gVidScaleX = 4;
int gVidScaleY = 4;
int gSDLWindow = 0;
int gVidUpdateForbid = 0;
Pal8_t gVidPalx[ 256 ];
VidRect_t gVidMainGeo;
int gVidUpdate = 1;

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
    VidClearScreen();
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
    int inited, i;
    Pal8_t *Pal;
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
	eprintf( "<ERROR> Creating SDL main surface error !! '%s'", SDL_GetError() );
	return -1;
    }
    if( !(gSDLRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/ ) ) ){
	eprintf( "<ERROR> Creating SDL renderer error !! '%s'", SDL_GetError() );
	return -1;
    }
    if( !(gSDLSurfaceCur = SDL_CreateRGBSurface( 0, 640, 480, 8, 0, 0, 0, 0 )) ){
	eprintf( "<ERROR> Creating SDL in game surface error !! '%s'", SDL_GetError() );
	return -1;
    }
    if( !(gSDLPalette = SDL_AllocPalette( 256 )) ){
	eprintf( "<ERROR> Creating SDL palette error !! '%s'", SDL_GetError() );
	return -1;
    }    
    if( SDL_SetSurfacePalette( gSDLSurfaceCur, gSDLPalette ) < 0 ){
	eprintf( "<ERROR> Creating SDL in game surface error !! '%s'", SDL_GetError() );
	return -1;
    }    
    if(!(gSDLTexture = SDL_CreateTexture( gSDLRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, mode.w, mode.h ))){
	eprintf( "<ERROR> Creating SDL texture error !! '%s'", SDL_GetError() );
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
    return 0;
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

void VidPaletteUpdate( )
{
    SDL_SetPaletteColors( gSDLPalette, (void *)gVidPalx, 0, 256 );
    gVidUpdate = 1;
//    VidUpdate();
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
}

void VidSetPaletteRange( Pal8_t *Pal, unsigned int FirstColor, unsigned int nColors )
{
    unsigned int i, *d;

    d = (void *	)&gVidPalx[ FirstColor ];
    for( i = 0; i < nColors; i++, Pal++ ){
	*d++ = (*(unsigned int *)Pal << 2) & 0xfcfcfcfc; 
    }
    VidPaletteUpdate( );
    WinUpdateArea( &gVidMainGeo );
}

void VidSetPaletteAll( Pal8_t *Pal )
{
    VidSetPaletteRange( Pal, 0, 256 );
}

Pal8_t *VidCreateDimmedPalette()
{
    int i;
    static Pal8_t DimPal8[ 256 ];

    for( i = 0; i < 256; i++ ){
        DimPal8[ i ].g = gSDLPalette->colors[ i ].r >> 2;
        DimPal8[ i ].b = gSDLPalette->colors[ i ].g >> 2;
        DimPal8[ i ].r = gSDLPalette->colors[ i ].b >> 2;
    }
    return DimPal8;
}

void VidCopyInt( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int posX, int posY, int Alpha, int AlphaMode )
{
    SDL_Surface tmp;
    SDL_Rect sr, dr;

    if( Width == 0 || Height == 0 || !gSDLReady ) return;
    memcpy( &tmp, gSDLSurfaceCur, sizeof( tmp ) );
    tmp.pixels = pSrc;
    tmp.pitch = SrcPitch;
    tmp.w = Width;
    tmp.h = Height;
    sr.w = SrcPitch;
    sr.h = Height;
    sr.x = SrcX;
    sr.y = SrcY;
    dr.w = Width;
    dr.h = Height;
    dr.x = posX;
    dr.y = posY;
    SDL_SetColorKey( &tmp, AlphaMode ? SDL_TRUE : SDL_FALSE, Alpha );
    SDL_BlitSurface( &tmp, &sr, gSDLSurfaceCur, &dr );
    gVidUpdate = 1;
//    VidUpdate();
}

void VidCopyAlpha( char *pSrc, int SrcPitch, int Unused, int SrcX, int SrcY, int Width, short Height, int posX, int posY, int Alpha )
{
    VidCopyInt( pSrc, SrcPitch, Height, SrcX, SrcY, Width, Height, posX, posY, Alpha, 1 );
}

void VidCopy( char *pSrc, int SrcW, int SrcH, int SrcX, int SrcY, int Width, short Height, int posX, int posY )
{
    VidCopyInt( pSrc, SrcW, SrcH, SrcX, SrcY, Width, Height, posX, posY, 0, 0 );
}

void VidClearScreen()
{
    SDL_FillRect( gSDLSurfaceCur, NULL, 0 );
    gVidUpdate = 1;
//    VidUpdate();
}

// no optimalization!, refreshes whole screen
void VidUpdate()
{        
    if( !gVidUpdate ) return;
    SDL_BlitSurface( gSDLSurfaceCur, NULL, gSDLSurfaceMain, NULL ); // transform paletized surface to RGB
    SDL_UpdateTexture( gSDLTexture, NULL, gSDLSurfaceMain->pixels, gSDLSurfaceMain->pitch );
    SDL_RenderCopy( gSDLRenderer, gSDLTexture, NULL, NULL );
    SDL_RenderPresent( gSDLRenderer );
    gVidUpdate = 0;
}



