#include "FrameWork.h"

Pal8_t 	gFadePaletteA[ 256 ];
Pal8_t 	gFadePaletteB[ 256 ];
Pal8_t 	gFadePaletteC[ 256 ];
int 	gFadeSteps;

void FadeInit()
{
    unsigned int time;

    memset( gFadePaletteC, 0x00, sizeof( gFadePaletteC ) );
    memset( gFadePaletteB, 0x3F, sizeof( gFadePaletteB ) );
    memcpy( gFadePaletteA, &gPalBase, sizeof( gFadePaletteA ) );
    time = TimerGetSysTime();
    if( GSoundIsMusicEnabled() || GSoundIsSpkOn() ) PalSetFadeSndCb( SoundUpdateAll );
    PalFadeIn( gFadePaletteA, gFadePaletteA, 60 );
    PalSetFadeSndCb( NULL );
    gFadeSteps = lround( 60.0 / (TimerCurrDiff( time ) * 0.0014) );
    ErrorPrintf( "\nFade time is %u\nFade steps are %d\n", time, gFadeSteps );
}

void FadeStep( Pal8_t *pal )
{
    int en;

    en = CycleColorIsEnabled();
    CycleColorStop();
    if( GSoundIsMusicEnabled() || GSoundIsSpkOn() ) PalSetFadeSndCb( SoundUpdateAll );
    PalFadeIn( gFadePaletteA, pal, gFadeSteps );
    PalSetFadeSndCb( 0 );
    memcpy( gFadePaletteA, pal, sizeof( gFadePaletteA ) );
    if( en ) CycleColorStart();
}

void FadeSetPalette( Pal8_t *pal )
{
    memcpy( gFadePaletteA, pal, sizeof( gFadePaletteA ) );
    PalSetColors( pal );
}

void FadePalMod( Pal8_t *Palette, int FirstColor, int LastColor )
{
    memcpy( &gFadePaletteA[ FirstColor ], Palette, (LastColor - FirstColor + 1) * sizeof( Pal8_t ) );
    PalModulate( Palette, FirstColor, LastColor );
}






