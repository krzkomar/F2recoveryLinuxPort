#pragma once

#define MVE_STAT_ERROR	0x01
#define MVE_STAT_ABORT	0x02
#define MVE_STAT_DIRECT	0x04
#define MVE_STAT_8	0x08
#define MVE_STAT_SUBS	0x10

// flags for MveSetFlags()
#define MVE_SCALED	0x01
#define MVE_4		0x04
#define MVE_SUBS	0x08


typedef struct 
{
    int		FirstFrame;
    int 	LastFrame;
    int 	Steps;
    char 	Type;
    Pal8_t 	Color;
    struct MveFade_t *Next;
} MveFade_t;

typedef struct _MveSubTitle_t
{
    char	*Line;
    int		Frame;
    struct _MveSubTitle_t *Next;
} MveSubTitle_t;

typedef struct 
{
  int i01;
  int i02;
  int i03;
  int i04;
  int i05;
  int i06;
  int i07;
  int i08;
  int i09;
  int i10;
  int i11;
  int i12;
  int i13;
  int i14;
  int i15;
  int i16;
  int i17;
  int i18;
  int i19;
  int i20;
  int i21;
  int i22;
  int i23;
  int i24;
  int i25;
  int i26;
  int i27;
} Mve01_t;


void MveUnk02( int (*cb)(void) );
void MveUnk03( int (*cb)(void) );
void MveUnk01( int (*cb1)(void), int (*cb2)() );
void *MveAlloc( int bytes );
void MveFree( void *ptr );
int MveGetData( xFile_t *fh, char *Buffer, int nbytes );
int MveBlitDirect( char *Surf, int Width, int Height, int PosX, int PosY, int w, int h, int aa, int ab );
void MveBlitBuffered( char *a1, int a2, int a3, int a4, int a5, int a6, int a7, int aa, int bb );
void MveUnk05( int (*Cb)() );
void MveUnk06( int (*Cb)());
void MveUnk07( void **pA, int *pB, int *pC, int *pD, int *pE, int *pG, int *pH );
void MveUpdate( int WinId, char *a2, int a3, int a4 );
int MveUnk09( int WinId, char *pSrc, int Width, int Height, int Pitch );
int MveUnk10( int WinId, char *pSrc, int Width, int Height, int Pitch );
int MveUnk11( int WinId, char *pSrc, int Width, int Height, int Pitch );
int MveUnk12( int WinId, char *pSrc, int Width, int Height, int Pitch );
int MveUnk13( int WinId, char *pSrc, int Width, int Height, int Pitch );
void MveSetPal( Pal8_t *a1, int a2, int a3 );
int MveNullHandler();
void MveInit();
void MveFinish( int a1 );
void MveClose();
void MveAbort();
int MveSetFlags( char flags );
void MveUnk19( int result );
void MveUnk21( float r, float g, float b );
void MveSetPaletteCb( int (*modulator)() );
void MveSetFadingCb( int (*Cb)(void) );
int MveOpenMRA( char *fname );
void MveUnk24();
xFile_t *MveOpenMovie( char *fname );
void MveLoadSubtitles( char *fname );
void MvePrintSubs();
int MvePrep( int WinId, char *MovieFile, int (*UserCb)() );
int MveHandler();
int MveStartSubs( int WinId, char *fname, int (*a2)(void) );
int MveStart( int WinId, char *fname );
int MveStartAt( int WinId, char *fname, int xpos, int ypos, int w, int h );
int MvePlayChunk();
void MveSetupFname( char *(*cb)() );
void MveSetMusicVolume( int vol );
void MvePlay();
int MveIsPlaying();
void MveSetupAudio( int Enable );
int MveFxInit();
void MveUnk37();
void MveUnk38();
int MveSetEffects( char *Path );
void MveEffectsClear();
void MveSetFadeEFX( int CurrentFrame );
void MveSetPaletteEFX( Pal8_t *Palette, int FirstColor, int LastColor );
void MveFadeLink( MveFade_t *fade );
void MveFadeClear();





