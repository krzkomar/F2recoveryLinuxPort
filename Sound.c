
#include <SDL.h>
#include <AL/al.h>
#include <AL/alc.h>
#include "FrameWork.h"


static void *SoundMalloc( size_t nsize );
static void SoundFree( void *ptr );
static int SoundUpdate( Sound_t *snd );

Sound_t *gSounds;
SoundFile_t gSoundOpenCb;
int gSoundTimer;
Sound01_t *gSoundSfxRoot;
Sound01_t *gSoundSfxList;

void *(*gSoundMalloc)(size_t) = SoundMalloc;
void *(*gSoundRealloc)(void *, size_t) = realloc;
void (*gSoundFree)(void *) = SoundFree;


const char *gSoundErrorList[ SND_ERR_ALL ] = 
{
    "sound.c: No error",			// 0  SND_ERR_OK
    "sound.c: SOS driver not loaded",		// 1  SND_ERR_SOS_DRVNLOAD
    "sound.c: SOS invalid pointer",		// 2  SND_ERR_SOS_PTR
    "sound.c: SOS detect initialized",		// 3  SND_ERR_SOS_INIT
    "sound.c: SOS fail on file open",		// 4  SND_ERR_SOS_FOPEN
    "sound.c: SOS memory fail",			// 5  SND_ERR_SOS_MEMFAIL
    "sound.c: SOS invalid driver ID",		// 6  SND_ERR_SOS_DRVID
    "sound.c: SOS no driver found",		// 7  SND_ERR_SOS_NODRV
    "sound.c: SOS detection failure",		// 8  SND_ERR_SOS_DETFAIL
    "sound.c: SOS driver loaded",		// 9  SND_ERR_SOS_DRVLOAD
    "sound.c: SOS invalid handle",		// 10 SND_ERR_SOS_INVHANDLE
    "sound.c: SOS no handles",			// 11 SND_ERR_SOS_NOHANDLE
    "sound.c: SOS paused",			// 12 SND_ERR_SOS_PAUSED
    "sound.c: SOS not paused",			// 13 SND_ERR_SOS_NOTPAUSED
    "sound.c: SOS invalid data",		// 14 SND_ERR_SOS_DATA
    "sound.c: SOS drv file fail",		// 15 SND_ERR_SOS_DRV
    "sound.c: SOS invalid port",		// 16 SND_ERR_SOS_PORT
    "sound.c: SOS invalid IRQ",			// 17 SND_ERR_SOS_IRQ
    "sound.c: SOS invalid DMA",			// 18 SND_ERR_SOS_DMA
    "sound.c: SOS invalid DMA IRQ",		// 19 SND_ERR_SOS_DMAIRQ
    "sound.c: no device",			// 20 SND_ERR_NODEV
    "sound.c: not initialized",			// 21 SND_ERR_NOINIT
    "sound.c: no sound",			// 22 SND_ERR_NOSOUND
    "sound.c: function not supported",		// 23 SND_ERR_UNSUP
    "sound.c: no buffers available",		// 24 SND_ERR_NOBUFFER
    "sound.c: file not found",			// 25 SND_ERR_NOFILE
    "sound.c: already playing",			// 26 SND_ERR_ISPLAYING
    "sound.c: not playing",			// 27 SND_ERR_NOTPLAYING
    "sound.c: already paused",			// 28 SND_ERR_ISPAUSED
    "sound.c: not paused",			// 29 SND_ERR_NOTPAUSED
    "sound.c: invalid handle",			// 30 SND_ERR_INVHANDLE
    "sound.c: no memory available",		// 31 SND_ERR_ALLOC
    "sound.c: unknown error"			// 32 SND_ERR_UNK
};


int gSoundErrno;
int gSoundMasterVol = 1;
int gSoundSampleRate;
int gSoundPlayed;
int gSound_unk14;
int gSoundBitFmt;
int gSoundDriverOn;

ALCchar *gSoundAlcDevice = NULL;
ALCcontext *gSoundContext = NULL;
ALuint gSoundSrc[3];

/**********************************************************************************/
static void SoundTimerKill( int Id )
{
    SDL_RemoveTimer( Id );
}

static int SoundSetTimer( int delay_ms, int resolution_ms, void *TimerProc, void *UserPtr )
{    
    return SDL_AddTimer( delay_ms, TimerProc, UserPtr );    
}

static void *SoundMalloc( size_t nsize )
{
    return malloc( nsize );
}

//void *SoundRealloc( void *ptr, size_t size )
//{
//    return realloc( ptr, size );
//}

static void SoundFree( void *ptr )
{
    free( ptr );
}

void SoundSetMemMng( void *(*SoundMalloc)(int), void *(*SoundRealloc)(void *, size_t Size), void (*SoundFree)(void *) )
{
    gSoundRealloc = SoundRealloc;
    gSoundFree    = SoundFree;
    gSoundMalloc  = (void *)SoundMalloc;
}

int SoundFileLen( void *fh )
{
    int p1, p2;

    p1 = ftell( fh );
    p2 = fseek( fh, 0, SEEK_END );
    fseek( fh, p1, SEEK_SET );
    return p2;
}

int SoundFileTell( FILE *fh )
{
    return ftell( fh );
}

int SoundFileWrite( void *ptr, int count, int size, void *fh )
{
    return fwrite( ptr, count, size, fh );
}

int SoundFileRead( void *ptr, int count, int size, void *fh )
{
    return fread( ptr, count, size, fh );
}

void *SoundFileOpen( char *fname, char *mode )
{
    return fopen(fname, mode);
}

int SoundFileSeek( void *fh, int offset, int whence )
{
    return fseek( fh, offset, whence );
}

void SoundFileClose( void *fh )
{
    fclose( fh );
}

const char *SoundErrorToStr( int ErrCode )
{
    if( ErrCode == -1 ) ErrCode = gSoundErrno;
    if( ErrCode >= SND_ERR_ALL ) ErrCode = SND_ERR_UNK;
    return gSoundErrorList[ ErrCode ];
}

int SoundInit( int rr, int BitFmt, int BuffSize, int SampleRate )
{    
    static ALfloat ListenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };

    gSoundSampleRate = SampleRate;
    if( !(gSoundAlcDevice = (ALCchar *)alcOpenDevice( NULL )) ) return 1;
    if( !(gSoundContext = alcCreateContext( (ALCdevice *)gSoundAlcDevice, NULL ))) return 1;
    if( !alcMakeContextCurrent( gSoundContext ) ) return 1;
    alListener3f( AL_POSITION, 0, 0, 1.0 );
    alListener3f( AL_VELOCITY, 0, 0, 0 );
    alListenerfv( AL_ORIENTATION, ListenerOri );
    alGenSources( (ALuint)3, (ALuint *)&gSoundSrc );

    // background, looped
    alSourcef( gSoundSrc[0], AL_PITCH, 1 );
    alSourcef( gSoundSrc[0], AL_GAIN, 1 );    
    alSource3f( gSoundSrc[0], AL_POSITION, 0, 0, 0 );
    alSource3f( gSoundSrc[0], AL_VELOCITY, 0, 0, 0 );
    alSourcei( gSoundSrc[0], AL_LOOPING, AL_TRUE );

    // effects
    alSourcef( gSoundSrc[1], AL_PITCH, 1 );
    alSourcef( gSoundSrc[1], AL_GAIN, 1 );    
    alSource3f( gSoundSrc[1], AL_POSITION, 0, 0, 0 );
    alSource3f( gSoundSrc[1], AL_VELOCITY, 0, 0, 0 );
    alSourcei( gSoundSrc[1], AL_LOOPING, AL_FALSE );

    // speech
    alSourcef( gSoundSrc[2], AL_PITCH, 1 );
    alSourcef( gSoundSrc[2], AL_GAIN, 1 );    
    alSource3f( gSoundSrc[2], AL_POSITION, 0, 0, 0 );
    alSource3f( gSoundSrc[2], AL_VELOCITY, 0, 0, 0 );
    alSourcei( gSoundSrc[2], AL_LOOPING, AL_FALSE );

    SoundSetMasterVol( 32767 );

    gSoundErrno = 0;
    gSoundDriverOn = 1;
    return 0;
}

void SoundClose()
{
    Sound_t *p;
    Sound01_t *sfx;

    if( gSounds ){
        do{        
    	    p = gSounds->Prev;
            SoundDelete( gSounds );
            gSounds = p;
        }while ( p );
    }
    if( gSoundTimer != -1 ) SoundTimerClear( &gSoundTimer );
    for( sfx = gSoundSfxList; sfx; sfx = sfx->Next ){
        gSoundSfxList = sfx;
        gSoundFree( sfx );
    }
    gSoundSfxList = NULL;
    if( gSoundAlcDevice ){
	alDeleteSources( 2, gSoundSrc );
	alcDestroyContext( gSoundContext );
	alcCloseDevice( (ALCdevice *)gSoundAlcDevice );
	gSoundAlcDevice = NULL;
    }
    gSoundErrno = 0;
    gSoundDriverOn = 0;
}

Sound_t *SoundCreate( int FlagB, int FlagC )
{
    Sound_t *NewSnd;

    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return NULL; }
    if(!(NewSnd = (Sound_t *)gSoundMalloc( sizeof( Sound_t ) )) ){ gSoundErrno = SND_ERR_ALLOC; return NULL; }
    memset( NewSnd, 0x00, sizeof( Sound_t ) );
    memcpy( &NewSnd->File, &gSoundOpenCb, sizeof( SoundFile_t ) );
    NewSnd->File.Handler = -1;
    NewSnd->Channels = 1;
    NewSnd->Freq = gSoundSampleRate;
    NewSnd->FlagsC = FlagC | 0x02;
    NewSnd->FlagsB = FlagB;
    NewSnd->PlayPos = 0;
    NewSnd->State = 0;    
    if( NewSnd->FlagsB & 0x10 ) {
        NewSnd->LoopCount = -1;
        NewSnd->FlagsC |= SND_LOOPING;
    } else {
        NewSnd->LoopCount = 0;
    }
    NewSnd->i22 = -1;
    NewSnd->i23 = 1;
    NewSnd->Volume = 32767;
    NewSnd->Next = NULL;
    NewSnd->FilePos = 0;
    // OpenAL
    alGenBuffers( 1, &NewSnd->BuffId );
    NewSnd->BuffAllocated = 1;
    NewSnd->Prev = gSounds;
    if( gSounds ) gSounds->Next = NewSnd;
    gSounds = NewSnd;
    return NewSnd;    
}

void SoundLoad( Sound_t *Snd )
{
    unsigned int flen,ReadOutBytes,nsize;
    char *FileBuffer;

    flen = Snd->File.Length( Snd->File.Handler );
    Snd->FileLen = flen;
    nsize = flen;

    if( Snd->FlagsB & 0x02 ){
        if( !(Snd->FlagsC & SND_LOOPING) ) Snd->FlagsC |= 0x120;
	/*??*/
    } else {
        Snd->FlagsB &= ~0x03;
        Snd->FlagsB |= 0x01;
    }
    FileBuffer = (char *)gSoundMalloc( nsize );    
    ReadOutBytes = Snd->File.Read( Snd->File.Handler, FileBuffer, nsize );

    if( ReadOutBytes != nsize ){
        if( !(Snd->FlagsC & SND_LOOPING ) || (Snd->FlagsC & 0x100) ){
            memset( &FileBuffer[ReadOutBytes], 0, nsize - ReadOutBytes );
        } else {
            memcpy( &FileBuffer[ ReadOutBytes ], FileBuffer, nsize - ReadOutBytes );
        }
    }    

    if( Snd->File.GetFmt ) Snd->File.GetFmt( Snd->File.Handler, &Snd->Channels, &Snd->Freq, &Snd->Samples );
    SoundLoadBuffer( Snd, FileBuffer, nsize );
    gSoundFree( FileBuffer );
    if( Snd->FlagsB & 0x01 ){
        Snd->File.Close( Snd->File.Handler );
        Snd->File.Handler = -1;
    }
}

int SoundLoadFile( Sound_t *Snd, char *Name )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !Snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    if( (Snd->File.Handler = Snd->File.Open( Name, 512 )) == -1 ){ gSoundErrno = SND_ERR_NOFILE; return SND_ERR_NOFILE; }
    SoundLoad( Snd );
    return 0;
}

int SoundRewind( Sound_t *snd, int Pos )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    if( snd->FlagsB & 2 ){
        snd->File.Seek( snd->File.Handler, 0, Pos );
        snd->i28 = 0;
        snd->BufSize = 0;
        snd->PlayPos = 0;
        snd->CurPos = 0;
        SoundLoad( snd );
    } else {
        snd->CurPos = 0;
    }
    snd->State &= ~SND_PLAYED;
    gSoundErrno = 0;
    return 0;
}

int SoundLoadBufferAl( Sound_t *Snd, char *Wave, int Size )
{
    alBufferData( Snd->BuffId, (Snd->Channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, Wave, Size, Snd->Freq );
    gSoundErrno = 0;        
    return 0;
}

int SoundLoadBuffer( Sound_t *Snd, char *Buffer, int Size )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !Snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    return SoundLoadBufferAl( Snd, Buffer, Size );
}

int SoundPlayAcm( Sound_t *snd )
{
    ALuint src;

    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    if( gSoundDriverOn ){
        if( snd->State & SND_PLAYED ) SoundRewind( snd, snd->CurPos );
    } else {
        gSoundErrno = SND_ERR_NOINIT;
    }
    src = gSoundSrc[ snd->FlagsC & SND_LOOPING ? 0:1 ];
    alSourcei( src, AL_BUFFER, snd->BuffId );
    snd->SrcId = src;
    SoundSetSndVol( snd, snd->Volume );

    alSourcePlay( src );    

    snd->State |= SND_PLAYING;
    gSoundPlayed++;
    gSoundErrno = 0;
    return 0;
}


int SoundStop( Sound_t *Snd )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !Snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    if( !gSoundDriverOn ){
        gSoundErrno = SND_ERR_NOINIT;
        return SND_ERR_NOINIT;
    }
    if( !(Snd->State & SND_PLAYING) ){ gSoundErrno = SND_ERR_NOTPLAYING; return SND_ERR_NOTPLAYING; }
    alSourceStop( Snd->SrcId );
    gSoundPlayed--;
    Snd->State &= ~SND_PLAYING;
    gSoundErrno = 0;
    return 0;
}

int SoundDelete( Sound_t *Snd )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }    
    if( !Snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    if( Snd->File.Handler != -1 ){
        Snd->File.Close( Snd->File.Handler );
        Snd->File.Handler = -1;
    }
    SoundDestruct( Snd, Snd->File.Handler );
    gSoundErrno = 0;
    return 0;        
}

int SoundPlayed()
{
    return gSoundPlayed;
}


void SoundProcess( Sound_t *Snd )
{
/*
    char c; int n;
    unsigned int FileLen; unsigned int InBufferSize; unsigned int v6; unsigned int i28; unsigned int v8; unsigned int OutBufferSize;
    int err; int v11; int v12; int i23; int v14; int v15; unsigned int i22;char *InBuffer; int v18; int v19; int LoopCount;
    int FilePos; void *Handler; char Data1; int v24; int v25;  int v26; unsigned int v27; int v28; int v29; int v30; int v31;
    int v32; int v33; char *v34; int v35; int v36; char *v37; char *v38; int WritePos; unsigned int PlayPos; int v41; int v42;
    char *v43;  int a1;  int num;

    if( Snd->Data1 < 0 && Snd->GetCurrentPosition(Snd->DxBuf, &PlayPos, (LPDWORD)&WritePos) ) return;
    Snd->PlayPos += ( PlayPos < Snd->BufSize ) ? (PlayPos + Snd->Bps * Snd->InBufferSize - Snd->BufSize) : ( PlayPos - Snd->BufSize );
    c = BYTE1(Snd->.Data1);
    if( (c & 1) != 0 ){
            if( Snd->.Data4 & 0x20 ){
                if( c & 2 ) Snd->.Data1 |= 0x80u;
            } else if( Snd->FileLen <= Snd->PlayPos ){
                Snd->Data1 |= 0x280u;
            }
    }
    Snd->BufSize = PlayPos;
    if( Snd->FileLen < Snd->PlayPos ){
            do{
                n = Snd->PlayPos - Snd->FileLen;
                FileLen = Snd->FileLen;
                Snd->PlayPos = n;
            }while( n > FileLen );
    }
    InBufferSize = Snd->InBufferSize;
    v41 = PlayPos / InBufferSize;
    v6 = PlayPos / InBufferSize;
    i28 = Snd->i28;
    if( i28 == PlayPos / InBufferSize ) return;
    v42 = ( i28 >= PlayPos / InBufferSize ) ? (v6 + Snd->Bps - Snd->i28) : ( v6 - Snd->i28 );
    v8 = Snd->InBufferSize;
    OutBufferSize = Snd->OutBufferSize;
    if( v8 * v42 >= OutBufferSize ) v42 = (OutBufferSize + v8 - 1) / v8;
    if( v42 < Snd->i23 ) return;
    err = Snd->Lock(Snd->DxBuf, Snd->InBufferSize * Snd->i28, Snd->InBufferSize * v42, (LPVOID *)&v38, (LPDWORD)&v36, (LPVOID *)&v37, (LPDWORD)&v35, 0);
    if( err ) return;
    v11 = Snd->InBufferSize;
    v43 = v38;
    v14 = v36;
LABEL_28:

    a1 = v14;
    while( --v42 != -1 ){
	if( Snd->Data1 & 0x200 ){
    	    memset(Snd->InBuffer, 0, Snd->InBufferSize);
    	    break;
        }
        if( Snd->i22 == -1 ){
    	    v18 = Snd->InBufferSize;
        } else {
            v15 = Snd->File.Tell( Snd->File.Handler );
            v18 = ( v15 + Snd->InBufferSize <= Snd->i22 ) ? Snd->InBufferSize : (Snd->i22 - v15);
        }
        num = Snd->File.Read( Snd->File.Handler, Snd->InBuffer, v18);
        if( num < Snd->InBufferSize ){
            if( (Snd->Data1 & 0x20) && !(Snd->Data1 & 0x100) ){
                while( num < Snd->InBufferSize ){
                    if( Snd->LoopCount == -1 ){
                        Snd->File.Seek( Snd->File.Handler, Snd->FilePos, SEEK_SET );
                        if( Snd->Callback ) Snd->Callback( Snd->pCallbackArg, 1024 );                                	
                    } else {
                        if( Snd->LoopCount <= 0 ){
                            Snd->i22 = -1;
                            Snd->FilePos = 0;
                            Snd->LoopCount = 0;
                            Snd->Data1 = Snd->Data1 & ~0x20;
                            num += Snd->File.Read( Snd->File.Handler, &Snd->InBuffer[num], Snd->InBufferSize - num );
                            break;
                        }
                        Snd->LoopCount--;
                        Snd->File.Seek( Snd->File.Handler, Snd->FilePos, 0);
                        if( Snd->Callback ) Snd->Callback( Snd->pCallbackArg, 1024 );
                    }                 
                    if( Snd->i22 == -1 ){
                	v28 = Snd->InBufferSize - num;
                    } else {
                        v24 = Snd->File.Tell( Snd->File.Handler );
                        v28 = ( Snd->InBufferSize + v24 + v26 <= Snd->i22 ) ? (Snd->InBufferSize - num) : (Snd->i22 - v24 - num);
                    }
                    v29 = Snd->File.Read( Snd->File.Handler, &Snd->InBuffer[num], v28 );
                    num += v29;
                    if( v29 < v30 ) break;
                }
            } else {
        	memset( &Snd->InBuffer[num], 0, Snd->InBufferSize - num );
                Snd->Data1 |= 0x200;
                num = Snd->InBufferSize;
            }
        }
        if( num > a1 ){
            if( a1 ) memcpy(v43, Snd->InBuffer, a1);
            if( v37 ){
                v32 = num - a1;
                v33 = num;
                memcpy(v37, &Snd->InBuffer[a1], num - a1);
                v43 = &v37[v33 - a1];
                v14 = v35 - v32;
                goto LABEL_28;
            }
            eprintf("Hm, no second write pointer, but buffer not big enough, this shouldn't happen\n");
        } else {
            v34 = v43;
            memcpy(v43, Snd->InBuffer, num);
            v43 = &v34[num];
            a1 -= num;
        }
    }
    Snd->Unlock(Snd->DxBuf, v38, v36, v37, v35);
    Snd->i28 = v41;
*/
}

static int SoundUpdate( Sound_t *snd )
{
    unsigned int state;
    
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    if( !(snd->State & SND_PLAYING ) ){ gSoundErrno = SND_ERR_NOTPLAYING;return SND_ERR_NOTPLAYING; }
    if( snd->State & SND_PAUSED ){ gSoundErrno = SND_ERR_NOTPLAYING;return SND_ERR_NOTPLAYING; }    
    if( snd->State & SND_PLAYED ){ gSoundErrno = SND_ERR_UNK; return SND_ERR_UNK; }

    alGetSourcei( snd->SrcId, AL_SOURCE_STATE, (ALint *)&state );

    if( snd->FlagsC && ( state == AL_PLAYING ) ) {
        if( (snd->State & SND_PAUSED) == 0 && (snd->FlagsB & 2 ) ) SoundProcess( snd );
    } else if( (snd->State & SND_PAUSED) == 0 ){
        if( snd->Callback ){
            snd->Callback( snd->pCallbackArg, 1 );
            snd->Callback = NULL;
        }
        if( snd->FlagsB & 0x04 ){
            snd->Callback = NULL;
            SoundDelete( snd );
        } else {
            snd->State |= SND_PLAYED;
            if( snd->State & SND_PLAYING ) gSoundPlayed--;
            SoundStop( snd );
            snd->State &= ~SND_PLAYING;
        }
    }
    gSoundErrno = 0;
    return 0;
}

int SoundIsPlaying( Sound_t *snd )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return 0; }    
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return 0; }        
    return snd->State & SND_PLAYING;
}

int SoundWasPlayed( Sound_t *snd )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return 0; }    
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return 0; }        
    return snd->State & SND_PLAYED;        
}

int SoundIsQueued( Sound_t *snd )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return 0; }    
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return 0; }        
    return snd->State & SND_QUEUED;        
}

int SoundIsPaused( Sound_t *snd )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return 0; }    
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return 0; }
    return snd->State & SND_PAUSED;    
}

int SoundUnk26( Sound_t *snd, int a2 )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return 0; }    
    if( !snd ) { gSoundErrno = SND_ERR_NOSOUND; return 0; }
    return a2 & snd->FlagsC;    
}

int SoundResume( Sound_t *snd, int a2 )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return 0; }    
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return 0; }
    return a2 & snd->FlagsB;    
}
/*
int SoundUnk28( Sound_t *snd, int a2 )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return 0; }
    snd->i32 = a2;
    gSoundErrno = SND_ERR_NOSOUND;
    return SND_ERR_NOSOUND;
}
*/
/*
int SoundUnk29( Sound_t *snd )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return 0; }    
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return 0; }        
    return snd->i32;            
}
*/
/*
int SoundUnk30( char *(*cb)(char *) )
{
    gSoundNameTr = cb;
    gSoundErrno = 0;
    return 0;
}
*/
int SoundRepeat( Sound_t *snd, short Count )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }    
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    if( Count ){
        snd->FlagsC |= SND_LOOPING;
        snd->LoopCount = Count;
    } else {
        snd->LoopCount = 0;
        snd->i22 = -1;
        snd->FilePos = 0;
        snd->FlagsC &= ~0x21;
    }
    gSoundErrno = 0;
    return 0;
}

double fyl2x( double a, double b )
{
    return b * log2( a );
}

int SoundVolConvert( unsigned int Volume )
{
    double vol;

    if( Volume > 0x7FFF ) Volume = 0x7FFF;
    if( !Volume ) return (int)lround( -10000.0 );
    vol = fyl2x( 32767.0 / (double)(int)Volume, 0.69314718055994528623 ) * -1000.0 / fyl2x( 2.0, 0.69314718055994528623 );
    if( vol >= -10000.0 ){
        if( vol > 0.0 ) vol = 0.0;
        return (int)lround(vol);
    } else {
        return (int)lround(-10000.0);
    }
}

int SoundSetSndVol( Sound_t *Snd, int Volume )
{
    double vol;

    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !Snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    Snd->Volume = Volume;

    vol = gSoundMasterVol * Volume / (32767.0 * 32767.0);
    alSourcef( Snd->SrcId, AL_GAIN, vol );

    gSoundErrno = 0;
    return gSoundErrno;
}

int SoundGetVolume( Sound_t *snd )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }    
    if ( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    return snd->Volume;    
}

int SoundSetCallback( Sound_t *Snd, void (*Cb)(int, int), void *pArg )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }    
    if( !Snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    Snd->Callback = (void *)Cb;
    Snd->pCallbackArg = pArg;
    gSoundErrno = 0;
    return 0;
}

int SoundSetChannel( Sound_t *snd, unsigned int Stereo )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }    
    if( Stereo == 3 ) snd->Channels = 2;    
    gSoundErrno = 0;
    return 0;    
}

int SoundSetBuffSize( Sound_t *snd, int size )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    snd->OutBufferSize = size;
    gSoundErrno = 0;
    return 0;
}

int SoundPause( Sound_t *snd )
{
    int err=0, flg;
    
    if( gSoundDriverOn ){
        if( !snd ) gSoundErrno = SND_ERR_NOSOUND;
        if( gSoundDriverOn ){
            if( snd ){
                flg = (snd->State & SND_PLAYING) == 0;
            } else { gSoundErrno = SND_ERR_NOSOUND; flg = 1; }
        } else { gSoundErrno = SND_ERR_NOINIT; flg = 1; }
    	if( flg ){ 
    	    err = SND_ERR_NOTPLAYING; gSoundErrno = SND_ERR_NOTPLAYING;
        } else {
            if( gSoundDriverOn ){
                if( snd ){
                    flg = (snd->State & SND_PAUSED) == 0;
                } else { gSoundErrno = SND_ERR_NOSOUND; flg = 1; }
            } else { gSoundErrno = SND_ERR_NOINIT; flg = 1; }
            if( flg ){
                snd->State |= SND_PAUSED;
                return SoundStop( snd );
            } else { err = SND_ERR_ISPAUSED; gSoundErrno = SND_ERR_ISPAUSED; }
        }
    } else { err = SND_ERR_NOINIT; gSoundErrno = SND_ERR_NOINIT; }
    return err;
}

int SoundUnPause( Sound_t *snd )
{
    int flg;

    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !snd ) gSoundErrno = SND_ERR_NOSOUND;
    if( gSoundDriverOn ){
        flg = snd->State & SND_PLAYING;
    } else { gSoundErrno = SND_ERR_NOINIT; flg = 0; }
    if( flg ){ gSoundErrno = SND_ERR_NOTPAUSED; return SND_ERR_NOTPAUSED; }
    if( gSoundDriverOn ){
        if( snd ){
            flg = snd->State & SND_PAUSED;
        } else {
            flg = 0;
            gSoundErrno = SND_ERR_NOSOUND;
        }
    } else {
        gSoundErrno = SND_ERR_NOINIT;
    }
    if( !flg ){ gSoundErrno = SND_ERR_NOTPAUSED; return SND_ERR_NOTPAUSED; }
    snd->State &= ~SND_PAUSED;
    snd->CurPos = 0;
    return SoundPlayAcm( snd );                
}

int SoundSetFileIO( Sound_t *Snd, void *OpenCb, void *ReadCb, void *CloseCb, void *WriteCb, void *SeekCb, void *TellCb, void *LengthCb, void *GetFmtCb )
{
    if( !gSoundDriverOn ) { gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }    
    if( !Snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
    if( OpenCb )   Snd->File.Open = OpenCb;
    if( CloseCb )  Snd->File.Close = CloseCb;
    if( ReadCb )   Snd->File.Read = ReadCb;
    if( WriteCb )  Snd->File.Write = WriteCb;
    if( SeekCb )   Snd->File.Seek = SeekCb;
    if( TellCb )   Snd->File.Tell = TellCb;
    if( LengthCb ) Snd->File.Length = LengthCb;
    if( GetFmtCb ) Snd->File.GetFmt = GetFmtCb;
    gSoundErrno = 0; 
    return 0;
}

void SoundDestruct( Sound_t *Snd, int fh )
{
    Sound01_t *p;

    if( Snd->State & SND_QUEUED ){
	for( p = gSoundSfxRoot; p; p = p->Next ){
    	    if( Snd == p->Sound ){
        	SoundSfxUnlink( p );
    		break;
    	    }        	
	}
    }
    if( gSoundDriverOn ){
	if( (Snd->State & SND_PLAYING) ) SoundStop( Snd );
    } else {
        gSoundErrno = SND_ERR_NOINIT;
    }

    if( Snd->Callback ){
        Snd->Callback( Snd->pCallbackArg, 1 );
        Snd->Callback = NULL;
    }
    if( Snd->BuffAllocated ){
	alDeleteBuffers( 1, &Snd->BuffId );
	Snd->BuffAllocated = 0;
    }    
    
//    if( Snd->p36 ) Snd->p36( fh, Snd );
    
    if( Snd->Prev ) Snd->Prev->Next = Snd->Next;
    if( Snd->Next )
        Snd->Next->Prev = Snd->Prev;
    else
        gSounds = Snd->Prev;
    gSoundFree( Snd );
}

int SoundErrorClear()
{
    gSoundErrno = 0;
    return 0;
}

int SoundSetMasterVol( unsigned int volume )
{
    Sound_t *p;

    if( volume >= 32768 ){ gSoundErrno = SND_ERR_UNK; return SND_ERR_UNK; }
        
    gSoundMasterVol = volume;
    for( p = gSounds; p; p = p->Prev ){
        SoundSetSndVol( p, p->Volume );
    }
    gSoundErrno = 0;
    return 0;    
}

void SoundTimerCb( int timer, int msg, void *cb, short dw1, short dw2 )
{
DD
//    if( cb ) ((void (__cdecl *)(void (__stdcall *)(int, int, __int16, __int16)))cb)(cb);
}


void SoundTimerClear( int *timer )
{
    if( *timer == -1 ) return;
    SoundTimerKill( *timer );
    *timer = -1;    
}

int SoundSpeechPosition( Sound_t *snd ) // returns current play position
{
    int32_t offset;
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }    
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND;return SND_ERR_NOSOUND;}        

    alGetSourcei( snd->SrcId, AL_SAMPLE_OFFSET, &offset );

    return offset;

//    if( !(snd->FlagsB & 0x02) ) 
//    return snd->CurPos;    
//    if( snd->CurPos <= snd->BufSize ) return snd->CurPos + snd->PlayPos + snd->Bps * snd->InBufferSize - snd->BufSize;    
//    return snd->CurPos + snd->PlayPos - snd->BufSize;
}

int SoundSpeechSetOffset( Sound_t *snd, int Pos )
{
//    unsigned int ReadOut;
//    unsigned int Frac;

    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }
//    if( !(snd->FlagsB & 0x02) ){
	snd->CurPos = Pos;
        gSoundErrno = 0;
	SoundUpdate( snd );
        return 0;
//    }
//DD
//    Frac = Pos / snd->InBufferSize % snd->Bps;
//    snd->CurPos = Frac * snd->InBufferSize + Pos % snd->InBufferSize;
/*
    snd->File.Seek( snd->File.Handler, Frac * snd->InBufferSize, SEEK_SET );
    ReadOut = snd->File.Read( snd->File.Handler, snd->InBuffer, snd->InBufferSize );
    if( ReadOut < snd->InBufferSize ){
        if( snd->FlagsB & 0x02 ){
            snd->File.Seek( snd->File.Handler, 0, SEEK_SET );
            snd->File.Read( snd->File.Handler, snd->InBuffer + ReadOut, snd->InBufferSize - ReadOut );
        } else {
            memset( &snd->InBuffer[ ReadOut ], 0, snd->InBufferSize - ReadOut );
	}
    }
    snd->PlayPos = Pos;
    snd->i28 = (++Frac) < snd->Bps ? Frac : 0;
    SoundUpdate( snd );
*/
//    gSoundErrno = 0;
//    return 0;
}

int SoundSetBps( Sound_t *snd, int Bps )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND; }    
    if( Bps >= 1 ){ 
        snd->Bps = ( Bps <= snd->Bps ) ? Bps : snd->Bps;
    } else {
        snd->Bps = 1;
    }
    gSoundErrno = 0;
    return 0;    
}

void SoundSfxUnlink( Sound01_t *fx )
{
    Sound01_t *p;

    if( !fx && !fx->Sound && !(fx->Sound->State & SND_QUEUED) ) return;
    if( fx->Prev )
        fx->Prev->Next = fx->Next;
    else
        gSoundSfxRoot = fx->Next;
    if( fx->Next ) fx->Next->Prev = fx->Prev;
    fx->Sound->State &= ~SND_QUEUED;
    p = gSoundSfxList;
    fx->Sound = NULL;
    gSoundSfxList = fx;
    fx->Next = p;
}

void SoundBckgTimerCb()
{
    Sound01_t *p;
    
    for( p = gSoundSfxRoot; p; p = p->Next ){
        if( (p->i04 > p->i02) || ((p->i01 + p->i04) < p->i02) ){
    	    if( (p->i04 < p->i02) || ((p->i01 + p->i04) > p->i02) ){
    		p->i04 += p->i01;
        	SoundSetSndVol( p->Sound, p->i04 );
        	continue;
    	    }
        }
        if( !p->i02 ){
            if( p->i05 ){
                SoundPause( p->Sound );
                SoundSetSndVol( p->Sound, p->i03 );
            } else {
                if( p->Sound->FlagsB & 0x04  ){
                    SoundDelete( p->Sound );
                } else {
                    SoundStop( p->Sound );
                    p->i03 = p->i02;
                    p->i04 = p->i02;
                    p->i01 = 0;
                    SoundSetSndVol( p->Sound, p->i02 );
                }
            }
        }
        SoundSfxUnlink( p );        
    }
    if( !gSoundSfxRoot && gSoundTimer != -1 ){
        SoundTimerKill( gSoundTimer );
        gSoundTimer = -1;
    }
}

int SoundFadePause( Sound_t *snd, int a2, int Vol, int a4 )
{
    Sound01_t *p, *tmp;

    p = NULL;
    if( !gSound_unk14 ){ gSoundErrno = SND_ERR_NOINIT; return SND_ERR_NOINIT; }
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return SND_ERR_NOSOUND;  }
    if( snd->State & SND_QUEUED ){
        for( p = gSoundSfxRoot; p; p = p->Next ){
            if( snd == p->Sound ) break;
        }
    }
    if( !p ){ // create new sound
        if( gSoundSfxList ){
            p = gSoundSfxList;
            gSoundSfxList = gSoundSfxList->Next;
        } else {
            p = (Sound01_t *)gSoundMalloc( sizeof( Sound01_t ) );
        }
        if( p ) {
            if( gSoundSfxRoot ) gSoundSfxRoot->Prev = p;
            p->Prev = NULL;
            tmp = gSoundSfxRoot;
            p->Sound = snd;
            gSoundSfxRoot = p;
            p->Next = tmp;
        }
    }
    if( !p ){ gSoundErrno = SND_ERR_ALLOC; return SND_ERR_ALLOC; }
    p->i02 = Vol;
    p->i03 = p->i04 = SoundGetVolume( snd );
    p->i05 = a4;
    p->i01 = 8 * ( 125 * (Vol - p->i03) ) / (40 * a2);
    snd->State |= SND_QUEUED;
    if( gSoundDriverOn ){
	if( snd->State & SND_PLAYING ) SoundPlayAcm( snd );
    } else {
        gSoundErrno = SND_ERR_NOINIT;
    }
    if( gSoundTimer == -1 ){
	gSoundTimer = SoundSetTimer( 40, 10, SoundTimerCb, SoundBckgTimerCb );
	if( !gSoundTimer ){ gSoundErrno = SND_ERR_UNK; return SND_ERR_UNK; }
    }
    gSoundErrno = 0;
    return 0;
}

int SoundSetPlayback( Sound_t *snd, int time, int volume )
{
    return SoundFadePause( snd, time, volume, 0 );
}

int SoundFadePause1( Sound_t *snd, int a2, int Vol )
{
    return SoundFadePause(snd, a2, Vol, 1 );
}

void SoundLink( Sound_t *snd )
{
    snd->Next = NULL;
    snd->Prev = gSounds;
    if( gSounds ) gSounds->Next = snd;
    gSounds = snd;
}

void SoundDeleteAll()
{
    Sound_t *tmp;
    
    while( gSounds ){
	tmp = gSounds->Prev;
        SoundDelete( gSounds );
        gSounds = tmp;
    }    
}

void SoundCloseAll()
{
    Sound_t *tmp;

    while( gSounds ){
	tmp = gSounds->Prev;
        SoundDelete( gSounds );
        gSounds = tmp;
    }
    
    if( gSoundTimer != -1 ){
        SoundTimerKill( gSoundTimer );
        gSoundTimer = -1;
    }
}

void SoundUpdateAll()
{
    Sound_t *p;

    for( p = gSounds; p; p = p->Prev ){
        SoundUpdate( p );
    }
}

int SoundInitIO( 
	int (*open)(char *Name, int mode), 
	void (*close)(int), 
	int (*write)(char *buff,int cnt, int nsize, int fh), 
	int (*read)(char *buff,int cnt, int nsize, int fh), 
	int (*seek)(int , int, int), int (*tell)(int fh), int (*len)(int fh),
	int (GetFmt)( int, int *, int *, int * )
    )
{
    if( open )  gSoundOpenCb.Open = (void *)open;
    if( close ) gSoundOpenCb.Close = (void *)close;
    if( read )  gSoundOpenCb.Read = (void *)read;
    if( write ) gSoundOpenCb.Write = (void *)write;
    if( seek )  gSoundOpenCb.Seek = (void *)seek;
    if( tell )  gSoundOpenCb.Tell = (void *)tell;
    if( len )   gSoundOpenCb.Length = (void *)len;
    if( GetFmt ) gSoundOpenCb.GetFmt = (void *)GetFmt;
    gSoundErrno = 0;
    return 0;
}

void SoundPauseAll()
{
    Sound_t *p;
    int err;

    for( p = gSounds; p; p = p->Prev ){
        if( SoundPause( p ) ){
            err = gSoundErrno;
            if( gSoundErrno >= SND_ERR_ALL ) err = SND_ERR_UNK;
            eprintf( "soundPauseAll: %s\n", gSoundErrorList[ err ] );
        }
    }
}

void SoundPauseFadeAll( int a1, int a2 )
{
    Sound_t *p;
    int err;
    
    for( p = gSounds; p; p = p->Prev ){        
        if( SoundFadePause( p, a1, a2, 1 ) ){
            err = gSoundErrno;
            if( gSoundErrno >= SND_ERR_ALL ) err = SND_ERR_UNK;
            eprintf( "soundPauseFadeAll: %s\n", gSoundErrorList[ err ] );
        }        
    }
}

void SoundUnk51()
{
    Sound_t *p, *q;

    p = gSounds;
    while( p ){
	q = p->Prev;
        SoundUnPause( p );
        p = q;
    }
}
/*
void SoundUnk52( Sound_t *snd, void *Cb, int UserPtr )
{
    if( !gSoundDriverOn ){ gSoundErrno = SND_ERR_NOINIT; return; }
    if( !snd ){ gSoundErrno = SND_ERR_NOSOUND; return; }
    snd->UserCb = Cb;
    snd->UserPtr = UserPtr;
    gSoundErrno = 0;
}
*/

// ***** added for stream playing in movies substituting DXSOUND by OpenAl *****
SoundStr_t *SoundStrCreate( int Freq, int Stereo, int bits )
{
    SoundStr_t *str;
    
    str = dbg_malloc( sizeof( SoundStr_t ) );
    str->Channels = 2 - ( Stereo == 0 );
    str->Bits = bits;
    str->Freq = Freq;
    str->BuffIdx = 0;
    str->SourceId = gSoundSrc[ 1 ];
    return str;
}

int SoundStrQueue( SoundStr_t *snd, void *data, int Size )
{
    ALint pcs, buff = 0, qed = 0, fmt;
     
    if( snd == NULL ) return -1;
    alGetSourcei( snd->SourceId, AL_BUFFERS_PROCESSED, &pcs ); // retrieve the number of processed buffers
    alGetSourcei( snd->SourceId, AL_BUFFERS_QUEUED, &qed ); // retrieve the number of processed buffers
    if( pcs ){
	alSourceUnqueueBuffers( snd->SourceId, 1, (ALuint *)&buff ); // unqueue 1 buffer
    } else {
	alGenBuffers( 1, (ALuint *)&buff ); // create buffer    
    }
    fmt = snd->Bits ? ((snd->Channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16) : ( (snd->Channels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8 );
    alBufferData( buff, fmt, data, Size, snd->Freq ); // load data to buffer
    alSourceQueueBuffers( snd->SourceId, 1, (ALuint *)&buff ); // queue buffer
    return 0;
}

void SoundStrClose( SoundStr_t *snd )
{
    int buff, pcs;

    if( snd == NULL ) return;
    alSourceStop( snd->SourceId );        
    alGetSourcei( snd->SourceId, AL_BUFFERS_PROCESSED, &pcs ); // retrieve the number of processed buffers    
    for( ;pcs; pcs-- ){
	alSourceUnqueueBuffers( snd->SourceId, 1, (ALuint *)&buff );
	alDeleteBuffers( 1, (ALuint *)&buff );
    }
    dbg_free( snd );
}

void SoundStrPlay( SoundStr_t *snd, int en )
{    
    if( en )
	alSourcePlay( snd->SourceId );    
    else
	alSourceStop( snd->SourceId );    
}

