#pragma once

#define MSE_LBUTT		0x01
#define MSE_RBUTT		0x02
#define MSE_LBUTT_HOLD 		0x04
#define MSE_RBUTT_HOLD 		0x08
#define MSE_LBHOLD 		0x10
#define MSE_RBHOLD 		0x20

#define MSE_CURSOR_COLOR_FG 	gPalColorCubeRGB[31][31][31]
#define MSE_CURSOR_COLOR_BG     gPalColorCubeRGB[8][8][8]
#define MSE_CURSOR_COLOR_ALPHA  gPalColorCubeRGB[0][0][0]

#define MSE_BUTT_HTIME   250	// time in ms to detect press & hold button

int 	MseInit();
void 	MseClose();
void 	MseCursorInit();
void 	MseGetStaticCursor( char **Data, int *Width, int *Height, int *Pitch, int *Xcenter, int *Ycenter, char *Alpha );
int 	MseSetStaticCursor( char *Data, int Width, int Height, int Pitch, int Xcenter, int Ycenter, char Alpha );
int 	MseGetAnimatedCursor( char **FramesData, int *Frames, int *Width, int *Height, int *Xcenter, int *Ycenter, int *Alpha, int *Rate );
int 	MseSetAnimatedCursor( char *pFramesData, int Frames, int FrameStart, int Width, int Height, int Xcenter, int Ycenter, char Alpha, int Rate_ms );
void 	MseUpdateAnimatedCursor();
void 	MseCursorShow();
void 	MseCursorHide();
void 	MseUpdate();
void 	MseProcess( int x, int y, int buttons );
int 	MseCursorInArea( int left, int top, int bottom, int right );
int 	MseCursorCenterInArea( int left, int top, int right, int bottom );
void 	MseGetCursorSizeRect( VidRect_t *p );
void 	MseGetCursorPosition( int *Xpos, int *Ypos );
void 	MseSetCursorPosition( int x, int y );
void 	MseCursorClipMain();
int 	MseGetButtons();
int 	MseCursorHidden();
void 	MseGetCursorSize( int *Width, int *Height );
void 	MseCursorMoveCenter( int x, int y );
int 	MseIsAcquired();
void 	MseGetData( int *posX, int *posY, int *buttons );
void 	MseDisable();
void 	MseEnable();
int 	MseEnabled();
void 	MseSetSpeed( double val );
double 	MseGetSpeed();
int 	MseGetTime();
void 	MseTimeReset();

