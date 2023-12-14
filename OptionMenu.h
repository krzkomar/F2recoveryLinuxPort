#pragma once

#define LIMIT( val, min, max ) if( val < min ) val = min; if( val > max ) val = max;   
#define OPTION_MENU_COLOR_A	gPalColorCubeRGB[0x12][0x11][3]
#define OPTION_MENU_COLOR_B	gPalColorCubeRGB[0xE][0xC][3]
#define PREF_COLOR_A		gPalColorCubeRGB[0x12][0x11][3]
#define PREF_COLOR_B 		gPalColorCubeRGB[18][17][3]
#define PREF_COLOR_C 		gPalColorCubeRGB[0x12][0x11][3]
#define PREF_COLOR_D 		gPalColorCubeRGB[0x12][0x11][3]
#define PREF_COLOR_E 		gPalColorCubeRGB[0x12][0x11][3]
#define PREF_COLOR_F 		gPalColorCubeRGB[0x12][0x11][3]
#define PREF_COLOR_G 		gPalColorCubeRGB[0x12][0x11][3]
#define PREF_COLOR_H 		gPalColorCubeRGB[0x12][0x11][3]
#define PREF_COLOR_I		gPalColorCubeRGB[18][17][3]
#define PREF_COLOR_J            gPalColorCubeRGB[11][9][2];
#define PREF_FONT_A 		104
#define PREF_FONT_B 		103
#define PREF_FONT_C		101
#define PREF_FONT_D 		101
#define PREF_FONT_E 		101
#define OPTION_MENU_START_Y	17

typedef struct // size of 76
{
    short       Positions;
    short	Dir;
    short	x;
    short	y;
    short	x0;
    short	x1;
    short	MsgId[ 4 ];
    short	WgId;
    short	i05;
    char	Name[32];
    double	dMin;
    double	dMax;
    union{
	int  	*iVal;
	double	*dVal;
    };
} OptPref_t;

typedef struct // size of 4
{
    short w01;
    short w02;
} OptUnk01_t;

/*
extern int gOptionUnk01; // poza plikiem
extern int gCurrentFont;
extern char *gOptionMenuSurface;
extern int gPrefWindow;

extern Geo_t gOptionMenuImgSize[ 3 ];
extern Msg_t gPreferencesMsgBase;
extern int gOptMenuUnk01;
extern Geo_t gPrefImgSize[ 10 ];
*/
/*
extern int gOptPrefH1;
extern int gOptPrefW2;
extern int gOptPrefH2;
extern MsgLine_t gOptPrefMsgLine;
extern char  *gOptPrefPix[ 10 ];
extern void *gOptMenuImgs[ 10 ];
extern CachePool_t *gOptPrefImgs[ 10 ];
extern int  gOptTextBaseDelay;
extern double gOptSetBrightness;
extern double gOptBrightness;
extern double gOptSetTextDelay;
extern double gOptSetMouseSens;
extern int gOptMouseSensitivity;
extern char *gOptPrefSurface;
extern int gOptionUnk15;
extern int gOptionMenuWindow;
extern CachePool_t *gOptionMenuWallpImg[ 3 ];
extern char *gOptionWallPaperBmp[ 3 ];
*/
/*
const short gPrefTitlePosA[ 5 ]
const short gPrefTitlePosB[ 6 ]
const short gPrefTitlePosC[ 8 ]
const short gOptPrefLabXofsA[ 4 ]
const short gOptPrefLabXofsB[ 3 ]
const int   gOptPauseImgIds[ 4 ]
const int   gOptKnobsAposY[ 5 ]
const short gOptKnobPosXY[ 4 ]
const int   gOptKnobsBposY[ 6 ]
int gPrefUnk05
//OptUnk01_t gOptions[ 11 ]
int gOptionMenuImgIds[ 3 ]
int gOptPrefWinPixIds[ 10 ]
OptPref_t gOptPrefKnobs[ 19 ]
*/

int  OptMenuInvoke();
int  OptMenu( int KeyCode );
int  OptMenuCreate();
void OptMenuDestroy();
int  OptPauseWindow( int a1 );
void OptPauseUnk01( int a1 );
void OptPrefDialog();
int  OptPrefDialogCreate();
void OptPrefHandler( int WgCode );
int  OptPrefUpdate( int WgNo );
void OptPrefDialogClose();
int  OptMenuInit();
void OptBrightInc();
void OptBrightDec();
void OptLoad();
void OptSaveConfig( int ModifyFileFlg );
void OptSetDefault( int UpdateFlg );
void OptPrefLoad();
void OptPrefSave();
void OptRealize();
int  OptSaveData( xFile_t *fh );
int  OptLoadData( xFile_t *fh );



