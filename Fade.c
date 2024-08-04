#include "FrameWork.h"

Pal8_t 	gFadePaletteA[ 256 ];
Pal8_t 	gFadePaletteB[ 256 ];
Pal8_t 	gFadePaletteC[ 256 ];
int 	gFadeSteps;

void FadeInit()
{
    unsigned int t0, time;

    memset( gFadePaletteC, 0x00, sizeof( gFadePaletteC ) );
    memset( gFadePaletteB, 0x3F, sizeof( gFadePaletteB ) );
    memcpy( gFadePaletteA, &gPalBase, sizeof( gFadePaletteA ) );
    // calibration
    t0 = TimerGetSysTime();
    if( GSoundIsMusicEnabled() || GSoundIsSpkOn() ) PalSetFadeSndCb( SoundUpdateAll );
    PalFadeIn( gFadePaletteA, gFadePaletteA, 60 );
    PalSetFadeSndCb( NULL );
    time = TimerCurrDiff( t0 );
    gFadeSteps = lround( FADE_TIME_MS * time / 60.0 ); // lround( 60.0 / (time * 0.0014) );
    eprintf( "Fade time is %ums,\tFade steps are %d.", time, gFadeSteps );
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






