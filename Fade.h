#pragma once

#define FADE_TIME_MS	500

extern Pal8_t 	gFadePaletteA[ 256 ];
extern Pal8_t 	gFadePaletteB[ 256 ];
extern Pal8_t 	gFadePaletteC[ 256 ];
extern int 	gFadeSteps;

void FadeInit();
void FadeStep( Pal8_t *pal );
void FadeSetPalette( Pal8_t *pal );
void FadePalMod( Pal8_t *Palette, int FirstColor, int LastColor );






