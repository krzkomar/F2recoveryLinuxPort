#include "FrameWork.h"

char gGSoundProtoFileName[13];
int gGSoundDbgEn = 0;
int gGSoundDbgLog = 0; // debug
int gGSoundMusicOn = 0;
int gGSound_Unk61 = 0;
int gGSoundBgUnk01 = 0;
int gGSoundSpkOn = 0;
int gGSoundOn = 0;
int gGSoundActiveSfxCnt = 0;
Sound_t *gGSoundBgSound = NULL;
Sound_t *gGSoundSpeech = NULL;
int (*gGSoundBgCallback)() = NULL;
int (*gGSoundSpkCallback)() = NULL;
const char gGSoundCodeA[ 5 ] = { 'R', 'A', 'O', 'F', 'H' };
const char gGSoundCodeB[ 5 ] = { 'O', 'C', 'L', 'N', 'U' };
/**/
int gGSoundFileMode = -1;
int gGSoundLoopMode = -1;
char *gGSoundPath[ 4 ] = { "sound/sfx/", "sound/music/", "sound/music/", "sound/speech/" };
int gGSoundMasterVol = 0x7fff;
int gGSoundMusicVol = 0x7fff;
int gGSoundSpkVol = 0x7fff;
int gGSoundSfxVol = 0x7fff;
int gGSoundUnk03 = -1;
int gGSoundBgPlayTime = 0;
char gGSoundBgFname[ 270 ];

/*******************************************/

int GSoundNullSub()
{
    return 1;
}

int GSoundInit()
{
    int tmp;

    if( gGSoundDbgEn ){
        GSLOG( "Trying to initialize gsound twice.\n" );
        return -1;
    }
    CfgGetInteger( &gConfiguration, "sound", "initialize", &tmp );
    if( !tmp ) return 0;
    CfgGetInteger( &gConfiguration, "sound", "debug", &gGSoundDbgLog );
    GSLOG( "Initializing sound system..." );
    if( GSoundMusicPath( &gGSoundPath[ 1 ], "music_path1" ) ) return -1;
    if( GSoundMusicPath( &gGSoundPath[ 2 ], "music_path2" ) ) return -1;
    if( strlen( gGSoundPath[ 1 ] ) > 247 || strlen( gGSoundPath[ 2 ] ) > 247 ){
        GSLOG( "Music paths way too long.\n" );
        return -1;
    }
    if( GSoundSetupPaths() ) return -1;
    SoundSetMemMng( (void *)Malloc, (void *)Realloc, (void *)Free );
    if( SoundInit( gGSoundUnk03, 24, 0x800, 22050 ) ){
        GSLOG( "failed!\n" );
        return -1;
    }
    GSLOG( "success.\n" );
    AudiofInit( GSoundNullSub );
    AudioInit( GSoundNullSub );
    CfgGetInteger( &gConfiguration, "sound", "cache_size", &tmp );
    if( tmp >= SIZE_KB( 256 ) ){
        eprintf( "\n!!! Config file needs adustment.  Please remove the " );
        eprintf( "cache_size line and run fallout again.  This will reset " );
        eprintf( "cache_size to the new default, which is expressed in K.\n" );
        return -1;
    }
    if( SfxCacheInit( SIZE_KB( tmp ), gGSoundPath[0] ) && gGSoundDbgLog ) eprintf( "Unable to initialize sound effects cache.\n" );

    if( !SoundInitIO( (void *)GSoundFileOpen, (void *)GSoundFileClose, (void *)GSoundFileWrite, (void *)GSoundFileRead, (void *)GSoundFileSeek, (void *)GSoundFileTell, (void *)GSoundFileLen, NULL ) ){
        InpTaskStart( GSoundUpdate );
        gGSoundDbgEn = 1;
        tmp = 0;
        CfgGetInteger( &gConfiguration, "sound", "sounds", &tmp );
        GSLOG( "Sounds are " );
        if( tmp ){
            if( gGSoundDbgEn ) gGSoundOn = 1;
        } else if( gGSoundDbgLog ){
            eprintf( "not " );
        }
        GSLOG( "on.\n" );
        tmp = 0;
        CfgGetInteger( &gConfiguration, "sound", "music", &tmp );
        GSLOG( "Music is " );

        if( tmp ){
            if( gGSoundDbgEn && !gGSoundMusicOn ){
//                MveSetMusicVolume( lround( gGSoundMusicVol * 0.94 ) );
                gGSoundMusicOn = 1;
                GSoundRestartBg( 12 );
            }
        } else if( gGSoundDbgLog ){
            eprintf("not ");
        }

        GSLOG("on.\n");
        tmp = 0;
        CfgGetInteger( &gConfiguration, "sound", "speech", &tmp );
        GSLOG( "Speech is " );
        if( tmp ){
            if( gGSoundDbgEn && !gGSoundSpkOn ) gGSoundSpkOn = 1;
        } else if( gGSoundDbgLog ){
            eprintf("not ");
        }
        GSLOG("on.\n");
        CfgGetInteger( &gConfiguration, "sound", "master_volume", &gGSoundMasterVol );
        GSoundSetMasterVolume( gGSoundMasterVol );
        CfgGetInteger( &gConfiguration, "sound", "music_volume", &gGSoundMusicVol );
        GSoundSetMusicVolume( gGSoundMusicVol );
        CfgGetInteger( &gConfiguration, "sound", "sndfx_volume", &gGSoundSfxVol );
        GSoundSetSfxVolume( gGSoundSfxVol );
        CfgGetInteger( &gConfiguration, "sound", "speech_volume", &gGSoundSpkVol );
        GSoundSetSpkVolume( gGSoundSpkVol );
        gGSoundBgUnk01 = 0;
        gGSoundBgFname[ 0 ] = 0;
        return 0;
    }
    GSLOG( "Failure setting sound I/O calls.\n" );
    return -1;
}

void GSoundReset()
{
    if( !gGSoundDbgEn ) return;    
    GSLOG("Resetting sound system...");
    if( gGSoundDbgEn && gGSoundSpkOn && gGSoundSpeech ){
        SoundDelete( gGSoundSpeech );
        gGSoundSpeech = NULL;
    }
    if( gGSound_Unk61 && gGSoundDbgEn && !gGSoundMusicOn ){
//        MveSetMusicVolume( lround( gGSoundMusicVol * 0.94) );
        gGSoundMusicOn = 1;
        GSoundRestartBg( 12 );
    }
    GSoundBgClose();
    gGSoundBgUnk01 = 0;
    SoundDeleteAll();
    SfxCacheReAlloc();
    gGSoundActiveSfxCnt = 0;
    GSLOG("done.\n");
}

int GSoundClose()
{
    if( !gGSoundDbgEn ) return -1;
    InpTaskStop( GSoundUpdate );
    if( gGSoundDbgEn && gGSoundSpkOn ){
        if( gGSoundSpeech ){
            SoundDelete( gGSoundSpeech );
            gGSoundSpeech = NULL;
        }
    }
    GSoundBgClose();
    GSoundOldMusFileDel();
    SoundClose();
    SfxCacheClose();
    AudiofFree();
    AudioFree();
    gGSoundDbgEn = 0;
    return 0;
}

void GSoundEnable()
{
    if( gGSoundDbgEn ) gGSoundOn = 1;
}

void GSoundDisable()
{
    if( gGSoundDbgEn ) gGSoundOn = 0;
}

int GSoundEnabled()
{
    return gGSoundOn;
}

int GSoundSetMasterVolume( unsigned int Volume )
{
    if( !gGSoundDbgEn ) return -1;
    if( Volume >= 32768 ){
        GSLOG("Requested master volume out of range.\n");
        return -1;
    }
    if( gGSound_Unk61 && Volume && GSoundGetMusicVol() ){
        if( gGSoundDbgEn && !gGSoundMusicOn ){
            MveSetMusicVolume( lround( gGSoundMusicVol * 0.94 ) );
            gGSoundMusicOn = 1;
            GSoundRestartBg( 12 );
        }
        gGSound_Unk61 = 0;
    }
    if( SoundSetMasterVol( Volume ) ){ GSLOG("Error setting master sound volume.\n"); return -1; }
    gGSoundMasterVol = Volume;
    if( gGSoundMusicOn && (Volume == 0) ){
        if( gGSoundDbgEn && gGSoundMusicOn ){
            GSoundBgClose();
            MveSetMusicVolume(0);
            gGSoundMusicOn = 0;
        }
        gGSound_Unk61 = 1;
    }
    return 0;        
}

int GSoundGetMasterVolume()
{
    return gGSoundMasterVol;
}

int GSoundSetSfxVolume( unsigned int volume )
{
    if( gGSoundDbgEn && volume <= 0x7FFF ){
        gGSoundSfxVol = volume;
        return 0;
    }
    GSLOG("Error setting sfx volume.\n");
    return -1;
}

int GSoundGetSfxVolume()
{
    return gGSoundSfxVol;
}

void GSoundOff()
{
    if( !gGSoundDbgEn || !gGSoundMusicOn ) return;    
    GSoundBgClose();
    MveSetMusicVolume( 0 );
    gGSoundMusicOn = 0;
}

void GSoundOn()
{
    if( !gGSoundDbgEn || gGSoundMusicOn ) return;        
//    MveSetMusicVolume( lround( gGSoundMusicVol * 0.94 ) );
    gGSoundMusicOn = 1;
    GSoundRestartBg( 12 );
}

int GSoundIsMusicEnabled()
{
    return gGSoundMusicOn;
}

void GSoundSetMusicVolume( int Val )
{
    if( !gGSoundDbgEn ) return;
    if( Val >= 0x8000 ){    
        GSLOG("Requested background volume out of range.\n");
        return;
    }
    gGSoundMusicVol = Val;
    if( gGSound_Unk61 ){
        if( gGSoundDbgEn && !gGSoundMusicOn ){
            MveSetMusicVolume( lround( Val * 0.94 ) );
            gGSoundMusicOn = 1;
            GSoundRestartBg( 12 );
        }
        gGSound_Unk61 = 0;
    }
    if( gGSoundMusicOn ){
        MveSetMusicVolume( lround( Val * 0.94 ) );
    }
    if( gGSoundMusicOn && gGSoundBgSound ){
        SoundSetSndVol( gGSoundBgSound, lround( gGSoundMusicVol * 0.94 ) );
    }
    if( !gGSoundMusicOn ) return;
    if( Val ){
	GSoundGetMasterVolume();
	if( Val ) return;
    }
    if( gGSoundDbgEn && gGSoundMusicOn ){
        GSoundBgClose();
        MveSetMusicVolume( 0 );
        gGSoundMusicOn = 0;
    }
    gGSound_Unk61 = 1;        
}

unsigned int GSoundGetMusicVol()
{
    return gGSoundMusicVol;
}

void GSoundSetMusicVol( unsigned int vol )
{
    GSoundSetMusicVolume( vol );
}

void GSoundSetBgUnk19( int val )
{
    gGSoundBgUnk01 = val;
}

int GSoundGetBgUnk19()
{
    return gGSoundBgUnk01;
}

int GSoundExBgUnk19( int a1 )
{
    int tmp;

    tmp = gGSoundBgUnk01;
    gGSoundBgUnk01 = a1;
    return tmp;
}

void GSoundBgSetCb( int (*cb)(int) )
{
    gGSoundBgCallback = cb;
}

int (*GSoundGetBgCb())(int)
{
    return gGSoundBgCallback;
}

void *GSoundBgSetCb_( int (*cb)(int) )
{
    void *old = gGSoundBgCallback;
    gGSoundBgCallback = cb;
    return old;
}

void GSoundUnk25()
{
    if( gSoundDriverOn ){
        if( !gGSoundBgSound ) gSoundErrno = 22;
    } else {
        gSoundErrno = 21;
    }
}

int GSoundLoadBg( const char *fname, int ModeC, int ModeA, int ModeB )
{
    char stmp[ 300 ];
    int err;

    gGSoundFileMode = ModeA;
    gGSoundLoopMode = ModeB;
    strcpy( gGSoundBgFname, fname );
    if( !gGSoundDbgEn || !gGSoundMusicOn ) return -1;
    GSLOG( "Loading background sound file %s%s...", fname, ".acm" );
    GSoundBgClose();

    if( GSoundOpen( &gGSoundBgSound, ModeA, ModeB ) ){
        GSLOG( "failed because sound could not be allocated.\n" );
        gGSoundBgSound = NULL;
        return -1;
    }
    
    if( SoundSetFileIO( gGSoundBgSound, AudiofOpen, AudiofRead, AudiofClose, NULL, AudiofSeek, GSoundFileWrite, AudiofLen, AudiofGetFmt ) ){
        GSLOG( "failed because file IO could not be set for compression.\n" );
        SoundDelete( gGSoundBgSound );
        gGSoundBgSound = NULL;
        return -1;
    }

    if( SoundSetChannel( gGSoundBgSound, SND_CHANNEL_BG ) ){
	GSLOG( "failed because the channel could not be set.\n" );
	SoundDelete( gGSoundBgSound );
	gGSoundBgSound = NULL;
	return -1;
    }    

    switch( ModeA ){
        case GSND_FILEFIND:           err = GSoundBgFind( stmp, fname ); break;
        case GSND_FILEFIND_WITH_COPY: err = GSoundBgFindWithCopy( stmp, fname ); break;
        default: err = 0; return -1;
    }
    
    if( err ){ GSLOG( "failed because the file could not be found.\n" ); goto Err; }
    if( ( ModeB == GSND_LOOPED ) && SoundRepeat( gGSoundBgSound, SND_INFINITY ) ){ GSLOG( "failed because looping could not be set.\n" ); goto Err; }
    if( SoundSetCallback( gGSoundBgSound, (void *)GSoundBgCb, NULL ) ){ GSLOG( "soundSetCallback failed for background sound\n" ); goto Err; }
    if( ModeC == GSND_LIMIT ) { 
	if( SoundSetBuffSize( gGSoundBgSound, GSOUND_READLIMIT ) ){ GSLOG( "unable to set read limit " ); goto Err; }
    }
    if( SoundLoadFile( gGSoundBgSound, stmp ) ){ GSLOG( "failed on call to soundLoad.\n" ); goto Err; }
    if( ModeC != GSND_LIMIT ){
	if( SoundSetBuffSize( gGSoundBgSound, GSOUND_READLIMIT ) ){ GSLOG( "unable to set read limit " ); goto Err; }
    }
    if( ModeC == GSND_NOT_START_PLAY ) return 0;
    if( GSoundBgPlayback() ){  GSLOG( "failed starting to play.\n" ); goto Err; }
    GSLOG( "succeeded.\n" );
    return 0;

Err:
    SoundDelete( gGSoundBgSound ); gGSoundBgSound = 0;
    return -1;
}

int GSoundSetBg( const char *name, int PlayMode )
{
    return GSoundLoadBg( name, PlayMode, GSND_FILEFIND_WITH_COPY, GSND_LOOPED );
}

int GSoundPlayOn()
{
    if( !gGSoundDbgEn || !gGSoundMusicOn || !gGSoundBgSound ) return -1;
    if( SoundIsPlaying( gGSoundBgSound ) ) return -1;
    if( SoundIsPaused( gGSoundBgSound ) ) return -1;
    if( SoundWasPlayed( gGSoundBgSound ) ) return -1;
    if( !GSoundBgPlayback() ){
	SoundDelete( gGSoundBgSound );
	gGSoundBgSound = NULL;    
	return -1;
    }
    return 0;
}

void GSoundBgClose()
{
    if( !gGSoundDbgEn || !gGSoundMusicOn || !gGSoundBgSound ) return;
    if( gGSoundBgUnk01 ){
        if( !SoundSetPlayback( gGSoundBgSound, 2000, 0 ) ){
            gGSoundBgSound = NULL;
            return;
        }
    }
    SoundDelete( gGSoundBgSound );
    gGSoundBgSound = NULL;
}

void GSoundRestartBg( int PlayMode )
{
    if( !gGSoundBgFname[0] ) return;
    if( GSoundLoadBg( gGSoundBgFname, PlayMode, gGSoundFileMode, gGSoundLoopMode ) ){ GSLOG(" background restart failed " ); }            
}

void GSoundBgPause( )
{
    if( gGSoundBgSound ) SoundPause( gGSoundBgSound );
}

void GSoundBgUnPause()
{
    if( gGSoundBgSound ) SoundUnPause( gGSoundBgSound );
}

void GSoundSpkClose()
{
    if( !gGSoundDbgEn || !gGSoundSpkOn ) return;
    if( gGSoundDbgEn && gGSoundSpkOn ){
        if( gGSoundSpeech ){
            SoundDelete( gGSoundSpeech );
            gGSoundSpeech = NULL;
        }
    }
    gGSoundSpkOn = 0;
}

void GSoundSpkDbgEn()
{
    if( gGSoundDbgEn && !gGSoundSpkOn )  gGSoundSpkOn = 1;    
}

int GSoundIsSpkOn()
{
    return gGSoundSpkOn;
}

void GSoundSetSpkVolume( int volume )
{
    if( !gGSoundDbgEn ) return;    
    if( volume < 32768 ){
        gGSoundSpkVol = volume;
        if( gGSoundSpkOn && gGSoundSpeech ){
            SoundSetSndVol( gGSoundSpeech, lround( volume * 0.69 ) );
            return;
        }
    } else if( gGSoundDbgLog ){
        eprintf("Requested speech volume out of range.\n");
    }
}

int GSoundGetSpkVolume()
{
    return gGSoundSpkVol;
}

void GSoundUnk32( int vol )
{
    GSoundSetSpkVolume( vol );
}

void GSoundUnk33( int (*cb)(int) )
{
    gGSoundSpkCallback = (void *)cb;
}

void *GSoundUnk34()
{
    return gGSoundSpkCallback;
}

void *GSoundUnk35( int (cb)(int) )
{
    void *cbr;

    cbr = gGSoundSpkCallback;
    gGSoundSpkCallback = (void *)cb;
    return cbr;
}

int GSoundSpkGetSamples()
{
    int spl = 0;

    if( !gSoundDriverOn ){  gSoundErrno = 21; return 0; }
    if( !gGSoundSpeech ){ gSoundErrno = 22; return 0; }
    if( gGSoundSpeech->Bps == 0 ) return 0;
    spl = gGSoundSpeech->FileLen / gGSoundSpeech->Bps; // bytes per second
    if( gGSoundSpeech->FileLen % gGSoundSpeech->Bps ) spl++;

    return spl;
}

int GSoundLoadSpk( const char *fname, int PlayMode, unsigned int ModeB, unsigned int ModeA )
{
    char stmp[ 300 ];

    if( !gGSoundDbgEn || !gGSoundSpkOn ) return -1;
    GSLOG( "Loading speech sound file %s%s...", fname, ".acm" );
    if( gGSoundDbgEn && gGSoundSpkOn && gGSoundSpeech ){
        SoundDelete( gGSoundSpeech );
        gGSoundSpeech = NULL;
    }
    if( GSoundOpen( &gGSoundSpeech, ModeA, ModeB ) ){
        GSLOG( "failed because sound could not be allocated.\n" );
        gGSoundSpeech = NULL;
        return -1;
    }
    if( !SoundSetFileIO( gGSoundSpeech, AudioOpen, AudioRead, AudioClose, NULL, AudioSeek, GSoundFileWrite, AudioLen, AudioGetFmt ) ){
	GSLOG( "failed because file IO could not be set for compression.\n" );
	SoundDelete( gGSoundSpeech );
	gGSoundSpeech = NULL;
	return -1;
    }

    if( GSoundSpkFind( stmp, fname ) ){ GSLOG( "failed because the file could not be found.\n" ); goto err; }
    if( (ModeB == GSND_LOOPED ) && SoundRepeat( gGSoundSpeech, SND_INFINITY ) ){ GSLOG( "failed because looping could not be set.\n" ); goto err; }
    if( SoundSetCallback( gGSoundSpeech, (void *)GSoundSpkCb, 0 ) ){ GSLOG( "soundSetCallback failed for speech sound\n" ); goto err; }
    if( PlayMode == GSND_LIMIT ){
	if( SoundSetBuffSize( gGSoundSpeech, GSOUND_READLIMIT ) ){ GSLOG( "unable to set read limit " ); goto err; }
    }
    if( !SoundLoadFile(gGSoundSpeech, stmp) ){ GSLOG( "failed on call to soundLoad.\n" ); goto err; }    
    if( PlayMode != GSND_LIMIT ){
	if( SoundSetBuffSize( gGSoundSpeech, GSOUND_READLIMIT ) ){ GSLOG( "unable to set read limit " ); goto err; }
    }
    if( PlayMode == GSND_NOT_START_PLAY ) return 0;
    if( GSoundUpdateSpkVolume() ){ GSLOG( "failed starting to play.\n" ); goto err; }
    GSLOG( "succeeded.\n" ); 
    return 0; 

err:
    SoundDelete( gGSoundSpeech );
    gGSoundSpeech = NULL;
    return -1;
}

int GSoundSpkDelete()
{
    int err;

    if( !gGSoundDbgEn || !gGSoundSpkOn || !gGSoundSpeech ) return -1;
    if( SoundIsPlaying( gGSoundSpeech ) ) return -1;
    if( SoundIsPaused( gGSoundSpeech ) ) return -1;
    if( SoundWasPlayed( gGSoundSpeech ) ) return -1;
    if( (err = GSoundUpdateSpkVolume() ) ){
        SoundDelete( gGSoundSpeech );
        err = -1;
        gGSoundSpeech = NULL;
    }
    return err;
}

void GSoundSpkCancel()
{
    if( !gGSoundDbgEn || !gGSoundSpkOn || !gGSoundSpeech ) return;
    SoundDelete( gGSoundSpeech );
    gGSoundSpeech = NULL;
}

void GSoundSpkPause()
{
    if( gGSoundSpeech ) SoundPause( gGSoundSpeech );
}

void GSoundSpkUnPause()
{
    if( gGSoundSpeech ) SoundUnPause( gGSoundSpeech );
}

int GSoundUnk42( char *fname, int a2 )
{
    Sound_t *snd;

    if( !gGSoundDbgEn || !gGSoundOn ) return -1;
    if( !(snd = GSoundUnk43( fname, 0, a2 )) ) return -1;
    if( !gGSoundDbgEn || !gGSoundOn ) return -1;
    SoundPlayAcm( snd );
    return 0;
}

Sound_t *GSoundLoadAcm( char *FileName, Obj_t *pArg )
{
    Sound_t *Snd;
    char c;
    char stmp[ 260 ];

    if( !gGSoundDbgEn || !gGSoundOn ) return 0;
    GSLOG( "Loading sound file %s%s...", FileName, ".acm" );
    if( gGSoundActiveSfxCnt >= SFX_HANDLERS ){
        GSLOG( "failed because there are already %d active effects.\n", gGSoundActiveSfxCnt );
        return NULL;
    }
    if( !(Snd = GSoundSfxInit()) ){
        GSLOG( "failed.\n" );
        return NULL;
    }
    gGSoundActiveSfxCnt++;
    sprintf( stmp, "%s%s%s", gGSoundPath[ 0 ], FileName, ".acm" );
    if( !SoundLoadFile( Snd, stmp ) ){
        GSLOG( "succeeded.\n" );
        return Snd;
    }
    if( !( !pArg || (OBJTYPE( pArg->ImgId ) != TYPE_CRIT) || ((*FileName != 'H') && (*FileName != 'N')) ) ){
    c = FileName[ 1 ];
    if( c == 'A' || c == 'F' || c == 'M' ){
	c = FileName[1];
	if( c == 'A' ){
    	    c = FeatGetVal( pArg, FEAT_GENDER ) ? 'F' : 'M';
	}
	sprintf( stmp, "%sH%cXXXX%s%s", gGSoundPath[0], c, FileName + 6, ".acm" );
	GSLOG( "trying %s ", &stmp[ strlen( gGSoundPath[ 0 ] ) ] );
	if( !SoundLoadFile(Snd, stmp) ){
	    GSLOG( "succeeded (with alias).\n" );    
	    return Snd;
	}
	if( c == 'F' ){
    	    sprintf( stmp, "%sHMXXXX%s%s", gGSoundPath[0], FileName + 6, ".acm" );
    	    GSLOG("trying %s ", &stmp[ strlen( gGSoundPath[ 0 ] ) ] );
    	    if( !SoundLoadFile(Snd, stmp) ){
        	GSLOG( "succeeded (with male alias).\n" );
        	return Snd;
    	    }
	}
    }
    }
    if( ( FileName[0] == 'M' && FileName[1] == 'A' && FileName[2] == 'L' && FileName[3] == 'I' && FileName[4] == 'E' && FileName[5] == 'U' ) ||
    	( FileName[0] == 'M' && FileName[1] == 'A' && FileName[2] == 'M' && FileName[3] == 'T' && FileName[4] == 'N' && FileName[5] == '2' ) )
    {
            sprintf( stmp, "%sMAMTNT%s%s", gGSoundPath[0], FileName + 6, ".acm" );
            GSLOG("trying %s ", &stmp[ strlen( gGSoundPath[0] ) ]);
            if( !SoundLoadFile(Snd, stmp) ){
                GSLOG( "succeeded (with alias).\n" );
                return Snd;
            }
    }
    gGSoundActiveSfxCnt--;
    SoundDelete( Snd );
    GSLOG( "failed.\n" );
    return 0;
}

Sound_t *GSoundUnk43( char *fname, Obj_t *obj, int Vol )
{
    Sound_t *Acm;

    Acm = GSoundLoadAcm( fname, obj );
    if( Acm ) SoundSetSndVol( Acm, Vol * gGSoundSfxVol / 0x7FFF );
    return Acm;
}


void GSoundSfxDelete( Sound_t *snd )
{
    if( !gGSoundDbgEn || !gGSoundOn ) return;
    if( SoundIsPlaying( snd ) ){ GSLOG( "Trying to manually delete a sound effect after it has started playing.\n" ); return; }
    if( SoundDelete( snd ) ){ GSLOG( "Unable to delete sound effect -- active effect counter may get out of sync.\n" ); return; }
    gGSoundActiveSfxCnt--;
}

int GSoundDbgPlayA( void *snd, void * )
{
    if( gGSoundDbgEn && gGSoundOn && snd ) SoundPlayAcm( snd );
    return 0;
}

int GSoundDbgPlayB( Sound_t *snd )
{
    if( !gGSoundDbgEn || !gGSoundOn || !snd ) return -1;
    SoundPlayAcm( snd );
    return 0;
}

int GSoundDistance( Obj_t *obj )
{
    VidRect_t v11, area, Rect;
    Obj_t *Owner;
    int type, distance, pe;

    if( !obj ) return 0x7FFF;
    type = OBJTYPE( obj->ImgId );
    if( type != TYPE_CRIT && type != TYPE_SCEN && type != TYPE_ITEM ) return 0x7FFF;
    if( !(Owner = ObjGetOwner( obj )) ) Owner = obj;
    ObjGetRefreshArea( Owner, &Rect );
    WinGetRect( gMapIsoWin, &area );
    if( RegionShrink( &Rect, &area, &v11 ) != -1 ) return 0x7FFF;
    distance = ObjGetDistance( Owner, gObjDude );
    pe = FeatGetVal( gObjDude, FEAT_PERCEPTION );
    if( distance <= pe ) return 0x7FFF;    
    if( distance < 2 * pe ) return 0x7FFF - 0x5554 * ( distance - pe ) / pe;
    return 0x2AAA;
}

char *GSoundCharacterFileName( Obj_t *obj, int a2, int a3 )
{
    char fname[ 8 ], Ext[ 3 ], id2;

    if( ArtGetFilenameFromList( OBJTYPE( obj->ImgId ), obj->ImgId & 0xFFF, fname ) == -1 ) return 0;
    if( a2 == 38 ){
        if( ArtMakeFnameMark( 38, a3, &id2, Ext ) == -1 ) return 0;
    } else {
	if( ArtMakeFnameMark( a2, (obj->ImgId & 0xF000) >> 12, &id2, Ext ) == -1 ) return 0;
    }
    if( a2 != 21 && a2 != 20 ){
        if( (a2 == 16 || a2 == 17) && a3 == 4 ) Ext[ 0 ] = 'Z';
    } else {
	if( a3 != 2 ){
	    Ext[ 0 ] = 'Y';    
	} else {
    	    if( a3 == 3 ) Ext[ 0 ] = 'Z';
	}
    }
    sprintf( gGSoundProtoFileName, "%s%c%c", fname, Ext[ 0 ], id2 );
    StrUpr( gGSoundProtoFileName );
    return gGSoundProtoFileName;
}

char *GSoundAmbientFileName( const char *str )
{
    sprintf( gGSoundProtoFileName, "A%6s%1d", str, 1 );
    StrUpr( gGSoundProtoFileName );
    return gGSoundProtoFileName;
}

char *GSoundItemFileName( const char *str )
{
    sprintf( gGSoundProtoFileName, "N%6s%1d", str, 1 );
    StrUpr( gGSoundProtoFileName );
    return gGSoundProtoFileName;
}

char *GSoundWeaponFileName( int a1, Obj_t *obj, int a3, Obj_t *a4 )
{
    int d, Type;
    Proto_t *proto;
    char c0, c1, c2;

    c2 = Item61( obj );
    c1 = gGSoundCodeA[ a1 ];
    if( a1 && a1 != 2 ){
        d = ( a3 == 2 || a3 == 4 || !a3 ) ? 1 : 2;
    } else {
        d = 1;
    }
    Type = ItemGetWeaponBase( 0, obj );
    if( c1 != 'H' || !a4 || Type == 6 || Type == 3 || Type == 5 ){
        c0 = 'X';
    } else {
	switch( OBJTYPE( a4->ImgId ) ){
	    case 0: ProtoGetObj( a4->Pid, &proto ); Type = proto->Critt.BaseStat[ 18 ]; break;
	    case 2: ProtoGetObj( a4->Pid, &proto ); Type = proto->Critt.BaseStat[ 2 ]; break;
	    case 3: ProtoGetObj( a4->Pid, &proto ); Type = proto->Critt.Type; break;
    	    default: Type = -1; break;
	}	
	switch( Type ){
    	    case 0 ... 2: c0 = 'M'; break;
    	    case 3: c0 = 'W'; break;
    	    case 4 ... 6: c0 = 'S'; break;
    	    default: c0 = 'F'; break;
	}
    }
    sprintf( gGSoundProtoFileName, "W%c%c%1d%cXX%1d", c1, c2, d, c0, 1 );
    StrUpr( gGSoundProtoFileName );
    return gGSoundProtoFileName;
}

char *GSoundSceneryFileName( int a1, int a2, const char *a3 )
{
    sprintf( gGSoundProtoFileName, "S%c%c%4s%1d", ( a1 ? 'P' :'A'), gGSoundCodeB[ a2 ], a3, 1 );
    StrUpr( gGSoundProtoFileName );
    return gGSoundProtoFileName;
}

char *GSoundOpenFileName( Obj_t *obj, int Idx )
{
    Proto_t *proto;

    if( OBJTYPE( obj->ImgId ) == TYPE_SCEN ){ // doors
        sprintf( gGSoundProtoFileName, "S%cDOORS%c", gGSoundCodeB[ Idx ], ( ProtoGetObj(obj->Pid, &proto) == -1 ) ? 'A' : proto->Critt.BaseStat[ 4 ] );
    } else { // containers
        ProtoGetObj( obj->Pid, &proto );
        sprintf( gGSoundProtoFileName, "I%cCNTNR%c", gGSoundCodeB[ Idx ], proto->Critt.BaseStat[ 23 ] );
    }
    StrUpr( gGSoundProtoFileName );
    return gGSoundProtoFileName;
}

int GSoundPlayPushBt()
{
    return GSoundPlay( "ib1p1xx1" );
}

int GSoundPlayReleaseBt()
{
    return GSoundPlay( "ib1lu1x1" );
}

void GSoundPlayTg()
{
    GSoundPlay( "toggle" );
}

int GSoundPlayCheck()
{
    return GSoundPlay( "ib2p1xx1" );
}

int GSoundPlayUnCheck()
{
    return GSoundPlay( "ib2lu1x1" );
}

void GSoundPlayKnobLt()
{
    GSoundPlay( "ib3p1xx1" );
}

int GSoundPlayKnobRt()
{
    return GSoundPlay( "ib3lu1x1" );
}

int GSoundPlay( char *fname )
{
    Sound_t *AcmFile;
    if( !gGSoundDbgEn || !gGSoundOn ) return -1;
    if( !(AcmFile = GSoundLoadAcm( fname, 0 )) ) return -1;
    if( !gGSoundDbgEn || !gGSoundOn ) return -1;
    SoundPlayAcm( AcmFile );
    return 0;
}

void GSoundUpdate()
{
    SoundUpdateAll();
}

xFile_t *GSoundFileOpen( char *fname, char mode )
{
    if ( (mode & 2) == 0 ) return (void *)-1;
    return dbOpen( fname, "rb" );
}

int GSoundFileWrite( xFile_t *fh, char *data, size_t Size )
{
    return -1;
}

void GSoundFileClose( xFile_t *stream )
{
    if( stream == (void *)-1 ) return;
    dbClose( stream );
}

int GSoundFileRead( xFile_t *fh, char *data, size_t Size )
{
    if( fh == (void *)-1 ) return -1;
    return dbread( data, 1, Size, fh );    
}

int GSoundFileSeek( xFile_t *fh, int offset, int origin )
{
    if( fh == (void *)-1 ) return -1;
    if( dbseek( fh, offset, origin ) ) return -1;
    return dbtell( fh );
}

int GSoundFileTell( xFile_t *fh)
{
    if ( fh == (void *)-1 ) return -1;
    return dbtell( fh );
}

int GSoundFileLen( xFile_t *fh )
{
    if ( fh == (void *)-1 ) return -1;
    return dbFileLength( fh );
}

int GSoundSpkCb( int a1, int a2 )
{
    if( a2 != 1 ) return a1;
    gGSoundSpeech = NULL;
    if( gGSoundSpkCallback ) return gGSoundSpkCallback();    
    return a1;
}

int GSoundBgCb( int a1, int a2 )
{
    if( a2 != 1 ) return a1;
    gGSoundBgSound = NULL;
    if( gGSoundBgCallback ) return gGSoundBgCallback();
    return a1;
}

void GSoundActiveSfxFire( void *ptr, int OneShot )
{
    if( OneShot == 1 ) gGSoundActiveSfxCnt--;
}

int GSoundOpen( Sound_t **sound, unsigned int ModeA, unsigned int ModeB )
{
    Sound_t *snd;
    int FlagsB, FlagsA;

    switch( ModeA ){
	case 13: FlagsA = 0x01; break;
	case 14: FlagsA = 0x02; break;
	default: FlagsA = 0x08 | 0x02; break;
    }
    
    switch( ModeB ){
	case 15: FlagsB = 0x04; break;
	case 16: FlagsB = 0x20 | 0x08 | 0x02; break;
	default: FlagsB = 0x00; break;
    }

    if( !(snd = SoundCreate( FlagsA, FlagsB )) ) return -1;
    *sound = snd;
    return 0;
}

int GSoundBgFindWithCopy( char *FullPath, const char *fname )
{
    FILE *fh1, *fh2;
    unsigned int len, bytes;
    char *p;
    void *buf;
    char stmpA[ 260 ], stmpB[ 260 ];

    len = strlen(".acm") + strlen( fname );
    if( ( (len + strlen( gGSoundPath[ 1 ] ) ) > 260) ||  ( (strlen( gGSoundPath[ 2 ] ) + len) > 260 ) ){
        GSLOG( "Full background path too long.\n" );
        return -1;
    }
    GSLOG( " finding background sound " );
    sprintf( stmpA, "%s%s%s", gGSoundPath[ 1 ], fname, ".acm" );
    if( GSoundFileFind( stmpA ) ){
        p = stmpA;
    } else {
	GSLOG( "by copy " );
	GSoundOldMusFileDel();
	sprintf( stmpB, "%s%s%s", gGSoundPath[ 2 ], fname, ".acm" );
	if( !( fh1 = fopen( stmpB, "rb" ) ) ){
    	    GSLOG( "Unable to find music file %s to copy down.\n", fname );
    	    return -1;
	}
	if( !(fh2 = fopen( stmpA, "wb" ) ) ){
    	    GSLOG( "Unable to open music file %s for copying to.\n", fname );
    	    fclose( fh1 );
    	    return -1;
	}
	if( !(buf = Malloc( 8192 ) ) ){
    	    GSLOG( "Out of memory in gsound_background_find_with_copy.\n" );
    	    fclose( fh2 );
    	    fclose( fh1 );
    	    return -1;
	}
	while( !feof( fh1 ) ){
	    if( ( bytes = fread( buf, 1, 0x2000, fh1 ) ) ){
		if( fwrite( buf, 1, bytes, fh2 ) != bytes ){
		    GSLOG( "Background sound file copy failed on write -- " );
    	    	    GSLOG( "likely out of disc space.\n" );
		    Free( buf );
	    	    fclose( fh1 );
		    fclose( fh2 );
		    xFileRemove( stmpA );
		    return -1;    
		}
	    }
	}   
	Free( buf );
	fclose( fh1 );
	fclose( fh2 );
	strcpy( gGSoundBgFname, fname );
    }
    strncpy( FullPath, p, 260 );
    FullPath[ 260 ] = 0;
    return 0;
}

int GSoundBgFind( char *FullPath, const char *fname )
{
    unsigned int len;
    char stmp[300];


    len = strlen( ".acm" ) + strlen( fname );
    if( (len + strlen( gGSoundPath[ 1 ] )) > 260 || (strlen( gGSoundPath[ 2 ] ) + len > 260) ){
	GSLOG( "Full background path too long.\n" );
	return -1;
    }
    GSLOG( " finding background sound " );
    sprintf( stmp, "%s%s%s", gGSoundPath[ 1 ], fname, ".acm" );
    if( !GSoundFileFind( stmp ) ){
	GSLOG( "in 2nd path " );
	sprintf( stmp, "%s%s%s", gGSoundPath[ 2 ], fname, ".acm" );
	if( !GSoundFileFind( stmp ) ){
	    GSLOG( "-- find failed " );
	    return -1;
	}
    }
    strncpy( FullPath, stmp, 260 );
    FullPath[ 260 ] = '\0';

    return 0;
}

int GSoundSpkFind( char *FullPath, const char *fname )
{
    char DirPath[ 260 ];
    int len;

    if( strlen( gGSoundPath[ 3 ] ) + strlen( ".acm" ) + strlen( fname ) > 260 ){
	GSLOG( "Full background path too long.\n" );
	return -1;
    }
    GSLOG( " finding speech sound " );
    sprintf( DirPath, "%s%s%s", gGSoundPath[ 3 ], fname, ".acm" );
    if( !dbCheckFileLength( DirPath, &len ) ){
        strncpy( FullPath, DirPath, 260 );
        FullPath[ 260 ] = 0;
        return 0;
    }
    GSLOG( "-- find failed " );
    return -1;
}

void GSoundOldMusFileDel()
{
    char stmp[ 300 ];

    if( !gGSoundBgFname[0] ) return;    
    sprintf( stmp, "%s%s%s", gGSoundPath[ 1 ], gGSoundBgFname, ".acm" );
    if( xFileRemove( stmp ) ){
        GSLOG( "Deleting old music file failed.\n" );
    }
    gGSoundBgFname[ 0 ] = 0;
}

int GSoundBgPlayback()
{
    GSLOG( " playing " );
    if( gGSoundBgUnk01 ){
	SoundSetSndVol( gGSoundBgSound, 1 );
	if( !SoundSetPlayback( gGSoundBgSound, 2000, lround( gGSoundMusicVol * 0.94 ) ) ) return 0;
    }
    SoundSetSndVol( gGSoundBgSound, lround( gGSoundMusicVol * 0.94 ) );
    if( SoundPlayAcm( gGSoundBgSound ) ){
        GSLOG( "Unable to play background sound.\n" );
        return -1;
    }
    return 0;
}

int GSoundUpdateSpkVolume()
{
    GSLOG( " playing " );
    SoundSetSndVol( gGSoundSpeech, lround( gGSoundSpkVol * 0.69 ) );
    if( SoundPlayAcm( gGSoundSpeech ) ){
        GSLOG( "Unable to play speech sound.\n" );
        return -1;
    }
    return 0;
}

int GSoundMusicPath( char **FullPath, const char *fname )
{
    unsigned int len;
    char *buff;

    CfgGetString( &gConfiguration, "sound", fname, FullPath );
    len = strlen( *FullPath ) + 1;
    if( (*FullPath)[ len - 2 ] == '/' ) return 0;
    if( !(buff = (char *)Malloc( len - 1 + 2 )) ){
        GSLOG( "Out of memory in gsound_get_music_path.\n" );
        return -1;
    }
    strcpy( buff, *FullPath );
    strcpy( &buff[ strlen( buff ) ], "/" );
    if( CfgSetString( &gConfiguration, "sound", (void *)fname, buff ) != 1 ){
        GSLOG( "config_set_string failed in gsound_music_path.\n" );
        return -1;
    }
    if( CfgGetString( &gConfiguration, "sound", (void *)fname, FullPath ) != 1 ){
	GSLOG( "config_get_string failed in gsound_music_path.\n" );
	return -1;
    }
    Free( buff );
    return 0;    
}

void GSoundSfxDbg()
{
    if( gGSoundActiveSfxCnt >= 5 ){
        GSLOG("WARNING: %d active effects.\n", gGSoundActiveSfxCnt );
    }
}

Sound_t *GSoundSfxInit()
{
    Sound_t *Snd;
    int err;

    if( !( Snd = SoundCreate( 0x04 | 0x01, 0x08 | 0x02 ) ) ){
	GSLOG( " Can't allocate sound for effect. " );
	GSLOG( "soundCreate returned: %d, %s\n", 0, SoundErrorToStr( 0 ) );
	return NULL;
    }

    if( SfxCached() ){
        err = SoundSetFileIO( Snd, SfxCacheFileOpen, SfxCacheFileRead, SfxCacheFileClose, SfxCacheFileWrite, SfxCacheFileSeek, SfxCacheFileTell, SfxCacheFileLen, SfxCacheGetFmt );
    }else{
	err = SoundSetFileIO( Snd, AudioOpen, AudioRead, AudioClose, NULL, AudioSeek, GSoundFileWrite, AudioLen, AudioGetFmt );
    }
    if( err ){
        GSLOG( "Can't set file IO on sound effect.\n" );
        GSLOG( "soundSetFileIO returned: %d, %s\n", err, SoundErrorToStr( err ) );
    } else {
        if( (err = SoundSetCallback( Snd, (void *)GSoundActiveSfxFire, 0 ) ) ){
    	    GSLOG( "failed because the callback could not be set.\n" );
    	    GSLOG( "soundSetCallback returned: %d, %s\n", err, SoundErrorToStr( err ) );        
        } else {
    	    SoundSetSndVol( Snd, gGSoundSfxVol );
    	    return Snd; // success
	}
    }
    SoundDelete( Snd );
    return NULL;
}

int GSoundFileFind( const char *fpath )
{
    FILE *fh;
    if( !(fh = fopen( fpath, "rb" ) ) ) return 0;
    fclose( fh );
    return 1;
}

int GSoundFileCheck( const char *fname )
{
    int len;

    return dbCheckFileLength( fname, &len ) == 0;
}

int GSoundSetupPaths()
{
    char DirName[66], str[276], *p;

    strcpy( str, gGSoundPath[ 1 ] );
    if( *gGSoundPath[ 1 ] == '/' ) 
	strcpy( DirName, "/" );
    else
        DirName[ 0 ] = '\0';
    for( p = strtok( str, "/" ); p; p = strtok( NULL, "/" ) ){
        strcpy( &DirName[ strlen( DirName ) ], p );
        if( xDirCreate( DirName ) ){
    	    if( errno != EEXIST ){            
        	GSLOG( "gsound_setup_paths mkdir error: %s\n", xstrerror( errno ) );
            }
        }
        strcpy( &DirName[ strlen( DirName ) ], "/" );
    }
    return 0;
}

int GSoundMapInit()
{
    return GSoundMapAmbient(0, 0);
}

int GSoundMapAmbient( int nu, int *arg)
{
    int k, *p, time;
    char *SfxFileName = NULL;

    k = -1;
    EvQeRun( 13, 0 );
    if( arg ){
        k = *arg;
    } else if( WmUnk92() > 0 ){
        k = WmUnk93();
    }
    if( !( p = Malloc( sizeof( int * ) ) ) ) return -1;
    if( !gMap.Name[ 0 ] ) return 0;
    if( WmUnk92() > 0 ){
	*p = WmUnk93();
 	if( EvQeSchedule( 10 * RandMinMax( 15, 20 ), 0, p, 13 ) == -1 ) return -1;
    }
    if( IN_COMBAT ) k = -1;
    if( k == -1 ) return 0;
    if( WmGetSfxFileName( k, &SfxFileName ) ) return 0;
    time = TimerGetTime();
    if( TimerDiff( time, gGSoundBgPlayTime ) >= 5000 ){
        if( GSoundPlay( SfxFileName ) == -1 )
            eprintf( "\nGsound: playing ambient map sfx: %s.  FAILED", SfxFileName );
        else
            eprintf( "\nGsound: playing ambient map sfx: %s", SfxFileName );
    }
    gGSoundBgPlayTime = time;
    return 0;
}



