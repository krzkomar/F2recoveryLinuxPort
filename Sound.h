#pragma once

#define SND_INFINITY	-1

// flags C
#define SND_LOOPING	0x20

// flags B

// States
#define SND_PLAYED	0x01
#define SND_PLAYING	0x02
#define	SND_QUEUED	0x04
#define SND_PAUSED	0x08


// channels
#define SND_CHANNEL_BG	3

enum {
    SND_ERR_OK = 0,
    SND_ERR_SOS_DRVNLOAD,
    SND_ERR_SOS_PTR,
    SND_ERR_SOS_INIT,
    SND_ERR_SOS_FOPEN,
    SND_ERR_SOS_MEMFAIL,
    SND_ERR_SOS_DRVID,
    SND_ERR_SOS_NODRV,
    SND_ERR_SOS_DETFAIL,
    SND_ERR_SOS_DRVLOAD,
    SND_ERR_SOS_INVHANDLE,
    SND_ERR_SOS_NOHANDLE,
    SND_ERR_SOS_PAUSED,
    SND_ERR_SOS_NOTPAUSED,
    SND_ERR_SOS_DATA,
    SND_ERR_SOS_DRV,
    SND_ERR_SOS_PORT,
    SND_ERR_SOS_IRQ,
    SND_ERR_SOS_DMA,
    SND_ERR_SOS_DMAIRQ,
    SND_ERR_NODEV,
    SND_ERR_NOINIT,
    SND_ERR_NOSOUND,
    SND_ERR_UNSUP,
    SND_ERR_NOBUFFER,
    SND_ERR_NOFILE,
    SND_ERR_ISPLAYING,
    SND_ERR_NOTPLAYING,
    SND_ERR_ISPAUSED,
    SND_ERR_NOTPAUSED,
    SND_ERR_INVHANDLE,
    SND_ERR_ALLOC,
    SND_ERR_UNK,
    SND_ERR_ALL
};

typedef struct 
{
  int i00;
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
  int i28;
} Sound02_t;


typedef struct 
{
    int (*GetFmt)( int fh, int *pChannels, int *pFreq, int *pSamples );
    int (*Open)(char *Name, int mode);
    void (*Close)(int);
    int (*Read)(int handler, char *buffer, int nsize);
    void *Write;
    int (*Seek)(int, int, int);
    int (*Tell)(int fh);
    int (*Length)(int fh);
    int Handler;
} SoundFile_t;

typedef struct _Sound_t
{
    SoundFile_t File;
// --OpenAL
    int BuffAllocated;
    unsigned int BuffId;
    int Channels;
    int Freq;
    int Samples;
    unsigned int SrcId;
// --
    int State;
    int FlagsB;
    int FlagsC;
  int  CurPos;
  int Volume;
  int LoopCount; // -1 - infinity
  int FilePos;
  int i22;
  int i23;
  int FileLen;
  int PlayPos;
  int i26;
  int OutBufferSize;
  int i28;
  int BufSize;
  int Bps;
  int InBufferSize;
  int i32;
  void *pCallbackArg;
  void (*Callback)(void *, int);
//  void *UserPtr;
//  void (*UserCb)();
 struct _Sound_t *Prev;
 struct _Sound_t *Next;
} Sound_t;

typedef struct _Sound01_t // size of 32
{
  Sound_t *Sound;
  int i01;
  int i02;
  int i03;
  int i04;
  int i05;
  struct _Sound01_t *Prev;
  struct _Sound01_t *Next;
} Sound01_t;

typedef struct
{
    int Freq;
    int Channels;
    int BuffIdx;
    int Bits;
    unsigned int SourceId;
} SoundStr_t;


extern void *(*gSoundMalloc)(size_t);
extern void *(*gSoundRealloc)(void *, size_t);
extern void (*gSoundFree)(void *);
extern const char *gSoundErrorList[ 33 ];
extern int gSoundErrno;
extern int gSoundVolScale;
extern int gSoundBuffer;
extern int gSoundSampleRate;
extern int gSoundPlayed;
extern int gSound_unk14;
extern int gSoundBufferSize;
extern int gSoundBitFmt;
extern int gSoundDriverOn;
//extern int gSounds;
extern int DirectSound;

void SoundSetMemMng( void *(*SoundMalloc)(int), void *(*SoundRealloc)(void *, size_t Size), void (*SoundFree)(void *) );
int SoundFileLen( void *fh );
int SoundFileTell( FILE *fh );
int SoundFileWrite( void *ptr, int count, int size, void *fh );
int SoundFileRead( void *ptr, int count, int size, void *fh );
void *SoundFileOpen( char *fname, char *mode );
int SoundFileSeek( void *fh, int offset, int whence );
void SoundFileClose( void *fh );
const char *SoundErrorToStr( int ErrCode );
void SoundPlayP( Sound_t *Snd );
int SoundInit( int BufferSize, int BitFmt, int BuffSize, int SampleRate );
void SoundClose();
Sound_t *SoundCreate( int Ds3dAlgo1, int Ds3dAlgo4 );
int SoundUnk21( Sound_t *snd, int bps, int bufsize );
void SoundLoad( Sound_t *Snd );
int SoundLoadFile( Sound_t *Snd, char *Name );
int SoundRewind( Sound_t *snd, int a2 );
int SoundLoadBufferAl( Sound_t *Snd, char *Buff, int Size );
int SoundLoadBuffer( Sound_t *Snd, char *Buffer, int Size );
int SoundPlayAcm( Sound_t *snd );
int SoundStop( Sound_t *Snd );
int SoundDelete( Sound_t *Snd );
int SoundPlayed();
int SoundUnkSub_48( Sound_t *snd, int a2, int a3 );
int SoundIsPlaying( Sound_t *snd );
int SoundWasPlayed( Sound_t *snd );
int SoundIsQueued( Sound_t *snd );
int SoundIsPaused( Sound_t *snd );
int SoundUnk26( Sound_t *snd, int a2 );
int SoundResume( Sound_t *snd, int a2 );
int SoundUnk28( Sound_t *snd, int a2 );
int SoundUnk29( Sound_t *snd );
//int SoundUnk30( char *(*cb)(char *) );
int SoundRepeat( Sound_t *snd, short Count );
int SoundVolConvert( unsigned int Vol );
int SoundSetSndVol( Sound_t *Snd, int Volume );
int SoundGetVolume( Sound_t *snd );
//int SoundSetPan( Sound_t *snd, short pan );
int SoundSetCallback( Sound_t *Snd, void (*Cb)(int, int), void *pArg );
int SoundSetChannel( Sound_t *snd, unsigned int a2 );
int SoundSetBuffSize( Sound_t *snd, int size );
int SoundPause( Sound_t *snd );
int SoundUnPause( Sound_t *snd );
int SoundSetFileIO( Sound_t *Snd, void *OpenCb, void *ReadCb, void *CloseCb, void *WriteCb, void *SeekCb, void *TellCb, void *LengthCb, void *GetFmtCb );
void SoundDestruct( Sound_t *Snd, int fh );
int SoundErrorClear();
int SoundUnk40( Sound_t *snd, int a2, int a3, int a4 );
int SoundSetMasterVol( unsigned int volume );
void SoundUnk41( int timer, int msg, void *cb, short dw1, short dw2);
int SoundUnk42( int Delay, int *a2, int *a3 );
void SoundTimerClear( int *timer );
int SoundSpeechUnk01( Sound_t *snd );
int SoundSpeechPlay( Sound_t *snd, int a2);
int SoundSetBps( Sound_t *snd, int a2 );
void SoundSfxUnlink( Sound01_t *fx );
void SoundBckgTimerCb();
int SoundFadePause( Sound_t *snd, int a2, int a3, int a4 );
int SoundSetPlayback( Sound_t *snd, int a2, int a3 );
int SoundFadePause1( Sound_t *snd, int a2, int a3 );
void SoundLink( Sound_t *snd );
void SoundDeleteAll();
void SoundCloseAll();
void SoundUpdateAll();
int  SoundInitIO( 
	int (*open)(char *Name, int mode), 
	void (*close)(int), 
	int (*write)(char *buff,int cnt, int nsize, int fh), 
	int (*read)(char *buff,int cnt, int nsize, int fh), 
	int (*seek)(int, int, int), int (*tell)(int fh), int (*len)(int fh),
	int (GetInstance)(int fh, int *ch, int *freq, int *sampl )
    );
void SoundPauseAll();
void SoundPauseFadeAll( int a1, int a2);
void SoundUnk51();
//void SoundUnk52( Sound_t *snd, void *a2, int a3 );
SoundStr_t *SoundStrCreate( int, int, int );
int SoundStrQueue( SoundStr_t *str, void *buff, int size );
void SoundStrClose( SoundStr_t *str );
void SoundStrPlay( SoundStr_t *str, int );




