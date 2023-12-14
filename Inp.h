#pragma once

#define INP_SELECTED	1111

typedef struct _Task_t
{
  char 		 Flags;
  void 		 (*Task)();
  struct _Task_t *Next;
} InpTask_t;

typedef struct
{
    int KbdKey;
    int MouseX;
    int MouseY;
} KbdEv_t;

int  	InpOpen( int unused );
void 	InpClose();
int  	InpUpdate();
void 	InpGetMouse( int *Xpos, int *Ypos );
void 	InpWinUpdate();
void 	InpPushBuffer( int Key );
int  	InpPopBuffer();
void 	InpFlushBuffer();
void 	InpTaskProceed();
InpTask_t *InpTaskStart( void (*CallBack)() );
void 	InpTaskStop( void (*CallBack)() );
void 	InpTaskHalt();
void 	InpTaskUnHalt();
void 	InpPause();
int  	InpPauseDialog();
void 	InpPauseInit( int PauseHKey, int (*PauseCb)(void) );
void 	InpScrShotBlit( void *Screen, int SrcPitch, int Unk, int SrcX, int SrcY, int Width, int Height, int Xpos, int Ypos );
void	InpScrShotTake();
void	InpScrShotInit( int hk, int (*bmp)(int Width, int Height, char *PaletteData, char *BitmapData) );
