#pragma once

#define GSLOG			if( gGSoundDbgLog ) eprintf
#define GSOUND_READLIMIT	0x40000

#define GSND_LOOPED		16
#define GSND_NOT_START_PLAY	10
#define GSND_LIMIT		11

#define GSND_FILEFIND		13
#define GSND_FILEFIND_WITH_COPY		14


/*
char gGSoundProtoFileName[13];
int gGSoundDbgEn;
int gGSoundDbgLog;
int gGSoundMusicOn;
int gGSound_Unk61;
int gGSoundBgUnk01;
int gGSoundSpkOn;
int gGSoundOn;
int gGSoundActiveSfxCnt;
Sound_t *gGsoundBgSound;
Sound_t *gGSoundSpeech;
void *gGSoundUnk02;
void *gGSoundUnk01;
const char gGSoundCodeA[ 5 ];
const char gGSoundCodeB[ 5 ];
int gGSound_unk02;
int gGSound_unk01;
char *gGSoundPath[ 4 ];
int gGSoundMasterVol;
int gGSoundMusicVol;
int gGSoundSpkVol;
int gGSoundSfxVol;
int gGSoundUnk03;
int gGSoundBgPlayTime;
char gGSoundBgFname[ 270 ];
*/

int GSoundNullSub();
int GSoundInit();
void GSoundReset();
int GSoundClose();
void GSoundEnable();
void GSoundDisable();
int GSoundEnabled();
int GSoundSetMasterVolume( unsigned int Volume );
int GSoundGetMasterVolume();
int GSoundSetSfxVolume( unsigned int volume );
int GSoundGetSfxVolume();
void GSoundOff();
void GSoundOn();
int GSoundIsMusicEnabled();
void GSoundSetMusicVolume( int Val );
unsigned int GSoundGetMusicVol();
void GSoundSetMusicVol( unsigned int vol );
void GSoundSetBgUnk19( int val );
int GSoundGetBgUnk19();
int GSoundExBgUnk19( int a1 );
void GSoundBgSetCb( int (*cb)(int) );
int (*GSoundGetBgCb())(int);
void *GSoundBgSetCb_( int (*cb)(int) );
void GSoundUnk25();
int GSoundLoadBg( const char *fname, int a2, int filemode, int loop );
int GSoundSetBg( const char *name, int PlayMode );
int GSoundPlayOn();
void GSoundBgClose();
void GSoundRestartBg( int PlayMode );
void GSoundBgPause();
void GSoundBgUnPause();
void GSoundSpkClose();
void GSoundSpkDbgEn();
int GSoundIsSpkOn();
void GSoundSetSpkVolume( int volume );
int GSoundGetSpkVolume();
void GSoundUnk32( int vol );
void GSoundUnk33( int (*cb)(int) );
void *GSoundUnk34();
void *GSoundUnk35( int (cb)(int) );
int GSoundSpkGetSamples();
int GSoundLoadSpk( const char *a1, int a2, unsigned int a3, unsigned int a4 );
int GSoundSpkDelete();
void GSoundSpkCancel();
void GSoundSpkPause();
void GSoundSpkUnPause();
int GSoundUnk42( char *fname, int a2 );
Sound_t *GSoundLoadAcm( char *FileName, Obj_t *obj );
Sound_t *GSoundUnk43( char *fname, Obj_t *obj, int Vol );
void GSoundSfxDelete( Sound_t *snd );
int GSoundDbgPlayA( void *, void * );
int GSoundDbgPlayB( Sound_t *a1 );
int GSoundDistance( Obj_t *obj );
char *GSoundCharacterFileName( Obj_t *a1, int a2, int a3 );
char *GSoundAmbientFileName( const char *str );
char *GSoundItemFileName( const char *str );
char *GSoundWeaponFileName( int a1,Obj_t *a2, int a3,Obj_t * );
char *GSoundSceneryFileName( int a1, int a2, const char *a3 );
char *GSoundOpenFileName( Obj_t *obj, int a2 );
int GSoundPlayPushBt();
int GSoundPlayReleaseBt();
void GSoundPlayTg();
int GSoundPlayCheck();
int GSoundPlayUnCheck();
void GSoundPlayKnobLt();
int GSoundPlayKnobRt();
int GSoundPlay( char *fname );
void GSoundUpdate();
xFile_t *GSoundFileOpen( char *fname, char mode );
int GSoundFileWrite( xFile_t *fh, char *data, size_t Size );
void GSoundFileClose( xFile_t *stream );
int GSoundFileRead( xFile_t *fh, char *data, size_t Size );
int GSoundFileSeek( xFile_t *fh, int offset, int origin );
int GSoundFileTell( xFile_t *fh);
int GSoundFileLen( xFile_t *fh );
int GSoundSpkCb( int a1, int a2 );
int GSoundBgCb( int a1, int a2 );
void GSoundActiveSfxFire( void *ptr, int OneShot );
int GSoundOpen( Sound_t **sound, unsigned int alg1, unsigned int alg4 );
int GSoundBgFindWithCopy( char *FullPath, const char *fname );
int GSoundBgFind( char *FullPath, const char *fname );
int GSoundSpkFind( char *FullPath, const char *fname );
void GSoundOldMusFileDel();
int GSoundBgPlayback();
int GSoundUpdateSpkVolume();
int GSoundMusicPath( char **FullPath, const char *fname );
void GSoundSfxDbg();
Sound_t *GSoundSfxInit();
int GSoundFileFind( const char *fpath );
int GSoundFileCheck( const char *fname );
int GSoundSetupPaths();
int GSoundMapInit();
int GSoundMapAmbient( Obj_t *, int *arg );




