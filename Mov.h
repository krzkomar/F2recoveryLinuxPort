#pragma once

#define MOV_ERR_OK	0
#define MOV_ERR_OK_	-1
#define MOV_ERR_IO	-2
#define MOV_ERR_TIMER	-3
#define MOV_ERR_SND	-4
#define MOV_ERR_VID	-5
#define MOV_ERR_RES	-6
#define MOV_ERR_GMODE	-7
#define MOV_ERR_FILE	-8
#define MOV_ERR_COLOR	-9
#define MOV_ERR_STEP	-10
#define MOV_ERR_VIDINIT	-11
#define MOV_ERR_VIDIO	-12
#define MOV_ERR_UNK	-13

//#define REALMODE

typedef struct 
{
  char x;
  char y;
} MveXYofs_t;

typedef struct 
{
  unsigned short Size;
  unsigned short Type;
} MveChunkHdr_t;

typedef struct 
{
  char Magic[20];
  short w0;
  unsigned short w1;
  unsigned short w2;
  MveChunkHdr_t Chunk;
} MveHdr_t;

typedef struct
{
  MveHdr_t *data;
  int i00;
  int i01;
}  MveUnk01_t;

typedef struct 
{
  short Offset;
  char Opcode;
  char Version;
} MveOpcode_t;

typedef struct 
{
  int Rate;
  short SubDivision;
} MveOpTimer_t;

typedef struct 
{
  short W0;
  short Flags;
  short SampleRate;
  short MinBufferLength;
} MveOpAudio_t;

typedef struct 
{
  short Width;
  short Height;
  short Count;
  short TrueColor;
} MveOpVideo_t;

typedef struct 
{
  short w0;
  short w1;
  short w2;
  short FlipBack;
  short w4;
  short w5;
  short w6;
  short w7;
} MveOpUnk5_t;

typedef struct 
{
  unsigned short PalStart;
  unsigned short PalCount;
  unsigned short Flags;
} MveOpPal_t;

typedef struct 
{
  short SeqIdx;
  short StrmMsk;
  short StrmLen;
  char Data[];
} MveOpSnd_t;

typedef struct 
{
  short Xres;
  short Yres;
  short Flags;
} MveOpVidMode_t;

typedef struct 
{
  char RBbase;
  char RB_Rnum;
  char RGbase;
  char RG_Rnum;
  char RG_Gnum;
  char Data;
} MveOpGrad_t;

typedef struct 
{
  short PalStart;
  short PalCount;
  char PalData;
} MveOpPalet_t;

typedef struct 
{
  int i00;
  short w0;
  short w1;
  short w2;
  short w3;
  short w4;
  char data;
} MveOpRun_t;


typedef union 
{
  MveOpcode_t Opcode;
  MveOpTimer_t Tim;
  MveOpAudio_t Snd;
  MveOpVideo_t Vid;
  MveOpUnk5_t Op5;
  MveOpPal_t Pal;
  MveOpSnd_t Audio;
  MveOpVidMode_t VidMode;
  MveOpGrad_t Grad;
  MveOpPalet_t Palette;
  MveOpRun_t Run;
} MveChunk_t;

typedef struct 
{
    xFile_t 	*i01;
    int (*ReadFile)(xFile_t *, char *, int );
    MveUnk01_t *Input;
    int 	i04;
    int 	i05;
    xFile_t 	*file2;
    MveChunkHdr_t *ChunkHdr;
    char 	*data;
    int 	Idx;
    void 	*SurfA;
    void 	*SurfB;
    int 	i12;
    void	*ScrLockA;
    void 	*ScrLockB;
    char 	ScrW;
    char 	ScrH;
    char 	ScrSize;
    int 	Color;
    int 	SurfW;
    int 	SurfH;
    int 	BufSizeA;
    int 	BufSizeB;
    int 	BufSizeC;
} Mov02_t;

typedef struct
{
  int i01;
  short w02;
  char M1;
  char M2;
  char co4;
  char M3;
  char c06;
  char i05;
  int i06;
  char M4;
  char M5;
  char M6;
} Mov01_t;

/*
int gMovAudioFrameStep;
int gMovSndCapabilities;
int gMovWidth;
int gMov_10;
int gMovFramesDropped;
int gMovSndCompressed;
MveUnk01_t gMovInputData;
MveChunkHdr_t gMovChunkHdr;
int gMovSnd16Bits;
int gMovBufOffset;
int gMovFrameNo;
int gMovHeight;
int gMovHeight01;
//Pal8_t gMovPaletteB[ 256 ];
void *(*gMovAlloc)(int);
int (*gMovAfterFrameCb)(void);
int gMovPosX;
int gMovPosY;
int gMovAudioFrameSize;
xFile_t *gMovFileHandler;
int gMovChunkOffset;
void (*gMovFree)(void *);
int gMovSndPcmStereo;
MveChunk_t *gMovChunkData;
short gMovAudioFrameBufOffset[ 60 ];
int gMovTimeStep;
int gMovWidth01;
int gMovAudioMsk;
int gMovTimeStampUs;
int (*gMovReadFileCb)(xFile_t *, char *, int);
int gMovAudioFrameNo;
int gMovUnk101;
char gMovUnk102[512];
int gMovScrBufSizeB;
int gMov_06;
int gMov_03;
int gMovUnk103;
int gMovSurfPitchA;
int gMovScrBufSizeA;
int gMovLines;
int (*gMovCb_07)(int, int);
Pal8_t gMovPaletteA[ 256 ];
int gMovScrHeight;
int gMveScrWidth;
int *gMov_04;
int gMov_08;
int gMovScrSize;
int gMovUnk02;
int gMovMovieXpos;
int gMovMovieYpos;
int gMovUnk20;
int gMov16BppMode;
int gMovScrBufSize;
int gMovSurfHeight;
unsigned char *gMovScrPixelsA;
unsigned char *gMovScrPixelsB;
int gMov_05;
int gMov_02[ 12272 ];
int gMovType = 0;
int gMovScaled = 4;
short gMovPcmTable[ 256 ];
void *gMovVideoDrv = NULL;
int gMovTimerEn = 0;
int gMovTimeUnk01 = 0;
int gMovUnk15 = 0;
Sound_t *gMovSoundDrv = NULL;
short *gMovSoundBuffer = NULL;
int gMovSoundBufferSize = 0; // added
int gMovSoundVol = 0;
int gMovAudioPlaying = 0;
int gMovSndPan = 0;
SDL_Surface *gMovSurfaceA = NULL;
SDL_Surface *gMovSurfaceB = NULL;
int (*gMovUnk14)( int, int, int, int, int, int, int, int, int );
//void (*gMovUnk14)() = MovNullHandlerCb;
int gMovUnk13 = 1;
int gMovUnk12 = 0;
int (*gMovSetPalette)( Pal8_t *Palette, int PalStart, int PalCount );
//void (*gMovSetPalette)() = MovProtUnk18;
int gMovSoundStopped = 0;
int gMovRun = 0;
int gMovMemProt = 0;
int gMovEncPosY[ 256 ] = { 0 };

const MveXYofs_t gMovEncXYOffstCur[ 256 ];
const MveXYofs_t gMovEncXYoffst[ 256 ];
*/

void MovSetupMemory( void *(*Alloc)(int), void ( *Free)(void *));
void MovSetInput( int (*Cb)(xFile_t *, char *, int ));
void MovSetP( MveUnk01_t *p, int size, void *ptr );
void MovFreeP( MveUnk01_t *p );
void MovSetSoundDrv( void *sndDrv );
void MovSetSndVolume( double Vol );
void MovSetAspect( int Width, int Height, int Lines, int a4, int *a5, unsigned int a6, unsigned int a7, int (*Cb)(int, int, int), int a9 );
void MovSetupBlitter( int (*Cb)());
void MovNullHandlerCb();
void MovSetupPalette( void (*pal)(Pal8_t *, int first, int cnt) );
int MovUnk01();
int MovUnk03( char a1, int a2, int a3, unsigned int a4, char a5, char a6, int a7, char a8 );
int MveNullSub1();
void MovSetupVideo( int videoDrv );
void MovSetupAfterFrameCb( int (*cb)(void) );
void MovSetType( int Type );
void MovSetScale( int a1 );
void MovGetFrameNo( int *Frame, int *Dropped );
int MovPrep( xFile_t *fh, int PosX, int PosY, char AudioBitNo );
int MovLoadHdr( xFile_t *fh ); // load main header and first chunk header
char *MovLoadData( unsigned int size );
char *MovReAlloc( MveUnk01_t *p, unsigned int size );
char *MovLoadChunk();
void MovProtectMemArea();
void MovSndMute();
int MovTimeUpdate();
void MovSndPause();
int MovStep();
int MovCreateTimer( int Rate, unsigned int Division );
int MovTimeStamp( int time );
int MovSndSetup( short a1, unsigned int BuffSize, short Stereo, short Freq, unsigned short Bits16, int Compressed );
void MovWait();
void MovSoundStop();
void MovSndStartStop();
int MovTimerWaitMs( int time );
void MovAudioFrame( char *SndData, short Size );
void MovAudioDecode( short *pOutBuffer, char *pRaw, unsigned int Size );
int MovVideoBufferInit( short Width, short Height, int Size, int Mode16bpp );
int MovFlipScreen();
void MovUnlockSurfaces();
void MovSwapSurfaces();
void MovVideoUpdate( int Xpos, int Ypos, short flags );
void MovSetPalette8A( short Start, short Count );
void MovSetPalette8B( short PalStart, short PalCount );
void MovCreateGradient( char BaseA, char a2, char a3, char BaseB, char a5, char a6 );
void MovSetPalette( char *Pal, short Start, short Count );
void MovStop();
void MovTimerStop();
int MovPlay( xFile_t *stream, int a2, int a3, char a4 );
int MovPlayB( xFile_t *a1, int a2, int a3, char a4 );
void MovClose();
void MovUnk52();
void MovVideoClose();
const char *MovGetError( int ErrNo );
void MovPCMdecodeMono( short *OutBuf, unsigned char *pInNBuf, int nbytes, short *FirstSample );
void MovPCMdecodeStereo( short *OutBuf, unsigned char *pInBuf, int nbytes, short *FirstSample );
void MovProtUnk03( short *a1, int a2, int a3, int a4, int a5 );
void MovProtUnk04( short *a1, short *a2, char *a3, int a4, int a5, int a6, int a7 );
void MovDecodeOldFrame( short *a1, short a2, short a3, short nbytes, short a5 );
void MovEncPitchInit();
void MovProtUnk24( int a1, short a2, int *a3, int a4 );
void MovProtUnk25( short a1, int a2, int a3, int *a4 );
void MovProtUnk11( char *pSrc, unsigned int SrcW, int SrcH, unsigned int SrcX, int SrcY, unsigned int DstW, unsigned int DstH, int DstX, int DstY, int nnn );
void MovProtUnk26( int a1, unsigned int a2, int a3, unsigned int a4, int a5, unsigned int a6, unsigned int a7, int a8, int a9, int a10 );
void MovProtUnk27( int a1, int a2, int a3, int a4, int a5, short *a6, int a7, int a8 );
void MovProtUnk12( int result, int a2, char *a3, char *a4 );
void MovProtUnk33( short a1, short *a2, int a3, char *a4, char *a5 );
void MovProtUnk13( unsigned int result, int a2, char *a3, char *a4 );
void MovProtUnk14( int result, int a2, int a3 );
void MovProtUnk28( int a1, int a2, int a3, int a4, int a5, short *a6, int a7, int a8 );
void MovProtUnk15( int result, int a2, char *a3, char *a4 );
void MovProtUnk32( short a1, short *a2, int a3, char *a4, char *a5 );
void MovProtUnk16( unsigned int Size, int a2, char *pDst, char *pSrc );
void MovProtUnk17( int ww, char *a2, int a3 );
void MovSetPaletteCompressedP( char *pData );
#ifdef REALMODE
void MovProtUnk18( char *a1, unsigned int a2, unsigned int a3 );
int MovProtUnk29( int a1 );
void MovProtUnk30( int a1, char a2, unsigned char a3 );
void MovProtUnk31( char *a1 );
void MovProtUnk20( char a1, Mov01_t *p );
void MovProtUnk21( int a1 );
void MovProtUnk22( unsigned int a1 );
#endif

void MovDecodeFrame16bpp( char *pMap, char *pData, short Xpos, short Ypos, short Width, short Height );
void MovDecodeFrame8bpp( unsigned char *pMap, unsigned char *pData, short Xpos, short Ypos, short Width, short Height );
void MovDecodeFrame10( short *pSkip, short *pMap, char *pData, short Xpos, short Ypos, short Width, short Height );

