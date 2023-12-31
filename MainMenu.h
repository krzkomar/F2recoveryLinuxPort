#pragma once

#define MAIN_MENU_COLOR 		gPalColorCubeRGB[0x14][0x13][3]
#define MAIN_MENU_ART_WALLPAPER 	6, 140, 0, 0, 0
#define MAIN_MENU_FONT			100
#define MAIN_MENU_WIDTH			640
#define MAIN_MENU_HEIGHT		480
#define MAIN_MENU_TXT_COPYRIGHT		20
#define MAIN_MENU_ART_BUTTUP		6, 299, 0, 0, 0
#define MAIN_MENU_ART_BUTTDN		6, 300, 0, 0, 0
#define MAIN_MENU_TXT_BUTTs		9
#define MAIN_MENU_FONT_BUTT		104
#define MAIN_MENU_POSITIONS		6

/*
int  gMainMenuButtons[6];
int  gMainMenuUpdate;
CachePool_t *gMainMenuButtonUpImg;
CachePool_t *gMainMenuButtonDnImg;
CachePool_t *gMainMenuWallPaper;
*/

int MainMenuCreate();
void MainMenuDestroy();
void MainMenuUpdate( int Fade );
void MainMenuFadeInit( int Fade );
int MainMenuIsDirty();
void MainMenuSetTimeout( int sec );
unsigned int MainMenuGetTimeout();
int MainMenuHandle();
int MainMenuKill();



