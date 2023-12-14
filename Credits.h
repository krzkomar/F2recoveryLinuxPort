#pragma once

#define CREDITS_COLOR_A		gPalColorCubeRGB[18][15][5]
#define CREDITS_COLOR_B 	gPalColorCubeRGB[13][11][9]
#define CREDITS_COLOR_C		0x4c//gPalColorCubeRGB[17][17][17]
#define CREDITS_FONT_A		103
#define CREDITS_FONT_B		104
#define CREDITS_SPEED		38

void Credits( const char *fname, int WallPaperId, char sel );
