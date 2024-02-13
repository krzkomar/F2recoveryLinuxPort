#include "FrameWork.h"

int gLipsPhoneme = 0;
int gLipsPrevPhoneme = 0;
int gLipsPosition = 0;
int gLipsNewPhonemeFlag = 1;
Lips_t gLipsync = { 2, 0x5800, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x32, 0x64, 0, 0, 0, {'T','E','S','T'}, {'V','O','C',0}, {'T','X','T',0}, {'L','I','P',0} };
unsigned int gLipsTime = 0;
char gLipsFileDir[ 14 ];
char gLipsFnameBuf[ 50 ];

/********************************************************************/

char *LipsyncGetFName( char *Name, int MaxSize )
{
    strncpy( gLipsFnameBuf, Name, MaxSize );
    return gLipsFnameBuf;
}

void LipsyncSpeechUpdate()
{
    int SpkPos, i, k;

    i = gLipsPosition;
    if( gLipsync.Status & LIPS_ANIMATE ){
        SpkPos = SoundSpeechPosition( gLipsync.Snd );
        for( i = gLipsPosition; ; i++ ){
    	    if( gLipsync.Markers[ i ].Pos >= SpkPos ) break;
    	    gLipsPhoneme = gLipsync.Phonemes[ i ];
            if( i >= gLipsync.MarkersCnt ) break;
        }
        if( i >= gLipsync.MarkersCnt ){
            gLipsPosition = i;
            k = 0;
            if( gLipsync.MarkersCnt <= 5 )
                eprintf( "Error: Too few markers to stop speech!" );
            else
                k = 3;
            i = gLipsPosition;
            if( gLipsync.Markers[ k ].Pos > SpkPos ){
                i = 0;
                gLipsPhoneme = gLipsync.Phonemes[ 0 ];
                if( !(gLipsync.Status & LIPS_NOTSTOP ) ){
                    SoundStop( gLipsync.Snd );
                    gLipsync.Status &= LIPS_CLRMASK;
                }
            }
        }
    }
    if( gLipsPrevPhoneme != gLipsPhoneme ){
        gLipsPrevPhoneme = gLipsPhoneme;
        gLipsNewPhonemeFlag = 1;
    }
    gLipsPosition = i;
    SoundUpdateAll();
}

int LipsyncSpeechStart()
{
    gLipsync.Status |= LIPS_ANIMATE;

    if( SoundSpeechSetOffset( gLipsync.Snd, gLipsync.Position ) ) eprintf( "Failed set of start_offset!\n" );

    for( gLipsPosition = 0; gLipsync.Markers[ gLipsPosition ].Pos < gLipsync.Position; gLipsPosition++ ){
        gLipsPhoneme = gLipsync.Phonemes[ gLipsPosition ];        
    }
DD
printf("*******************************\n* spk vol: %ld %p %i %i\n", lround( GSoundGetSpkVolume() * 0.69 ), gLipsync.Snd, gLipsync.Position, gLipsync.Snd->Volume );
    SoundSetSndVol( gLipsync.Snd, lround( GSoundGetSpkVolume() * 0.69 ) );
    gLipsTime = TimerGetSysTime();
    if( SoundPlayAcm( gLipsync.Snd ) ){
        eprintf( "Failed play!\n" );
        gLipsPosition = 0;
        SoundStop( gLipsync.Snd );
        gLipsync.Status &= LIPS_CLRMASK;
    }
    return 0;
}

int LipsyncRewind()
{
    gLipsPosition = 0;
    SoundStop( gLipsync.Snd );
    gLipsync.Status &= LIPS_CLRMASK;
    return 0;
}

int LipsyncGetByte( char *fname, xFile_t *fh )
{
    return dbgetb( fh, fname );
}

int LipsyncGet( LipsMarker_t *lp, xFile_t *fh )
{
    LipsMarker_t tmp;

    if( dbgetBei( fh, &tmp.Type ) == -1 || dbgetBei( fh, &tmp.Pos ) == -1 ) return -1;
    lp->Type = tmp.Type;
    return 0;
}

int LipsyncLoadDataOldFmt( Lips_t *lp, xFile_t *fh )
{
    int z1, z2, z3, z4;

    if( dbgetBei( fh, &lp->Version ) == -1 ) return -1; 
    if( dbgetBei( fh, &lp->Magic ) == -1 ) return -1; 
    if( dbgetBei( fh, &lp->Status ) == -1 ) return -1;
    if( dbgetBei( fh, &z4 ) == -1 ) return -1;
    if( dbgetBei( fh, &lp->Unk11 ) != -1 ) return -1;
    if( dbgetBei( fh, &z3 ) == -1 ) return -1; 
    if( dbgetBei( fh, &z2 ) == -1 ) return -1;
    if( dbgetBei( fh, &lp->Unk14 ) == -1 ) return -1; 
    if( dbgetBei( fh, &lp->Position ) == -1 ) return -1;
    if( dbgetBei( fh, &lp->PhonemesCnt ) == -1 ) return -1;
    if( dbgetBei( fh, &lp->Unk16 ) == -1 ) return -1;
    if( dbgetBei( fh, &lp->MarkersCnt ) == -1 ) return -1;
    if( dbgetBei( fh, &z1 ) == -1 ) return -1;
    if( dbgetBei( fh, &lp->MarkerSize ) == -1 ) return -1;
    if( dbgetBei( fh, &lp->i15 ) == -1 ) return -1; 
    if( dbgetBei( fh, &lp->i16 ) == -1 ) return -1; 
    if( dbgetBei( fh, &lp->i17 ) == -1 ) return -1; 
    if( dbgetBei( fh, &lp->i18 ) == -1 ) return -1; 
    if( dbgetBei( fh, &lp->i19 ) == -1 ) return -1; 
    if( dbgetBei( fh, &lp->i20 ) == -1 ) return -1;
    if( dbreadByteBlk( fh, lp->SpkId, 8 ) == -1 ) return -1; 
    if( dbreadByteBlk( fh, lp->SndExt, 4 ) == -1 ) return -1; 
    if( dbreadByteBlk( fh, lp->TextExt, 4 ) == -1 ) return -1; 
    if( dbreadByteBlk( fh, lp->LipExt, 4 ) == -1 ) return -1;
    if( dbreadByteBlk( fh, lp->i26, 260 ) == -1 ) return -1;
    lp->Snd = (void *)((long)z4);
    lp->Unk12 = (void *)((long)z3);
    lp->Phonemes = (void *)((long)z2);
    lp->Markers = (void *)((long)z1);
    return 0;
}

int LipsyncLoadFile( char *a1, char *DirName )
{
    xFile_t *fh;
    char *s, fpath[ 260 ], stmp[ 16 ];
    int i, tmp;

    strcpy( fpath, "SOUND/SPEECH/" );
    strcpy( gLipsFileDir, DirName );
    strcpy( &fpath[ strlen( fpath ) ], gLipsFileDir );
    strcpy( &fpath[ strlen( fpath ) ], "/" );
    if( (s = strchr( fpath, '.') ) ) *s = 0;
    strcpy( stmp, a1 );    
    if( (s = strchr( stmp, '.') ) ) *s = 0;
    strcpy( gLipsync.SpkId, stmp );
    strcpy( &fpath[strlen(fpath)], LipsyncGetFName( gLipsync.SpkId, 8 ) );
    strcpy( &fpath[ strlen( fpath ) ], "." );
    strcpy( &fpath[ strlen( fpath ) ], gLipsync.LipExt );
    LipsyncClose();
    if( (fh = dbOpen( fpath, "rb" ) ) ){
	if( dbgetBei( fh, &gLipsync.Version ) ) return -1;
	if( gLipsync.Version == 1 ){
    	    eprintf( "\nLoading old save-file version (1)" );
    	    if( dbseek( fh, 0, 0 ) ) return -1;
    	    if( LipsyncLoadDataOldFmt( &gLipsync, fh ) ) return -1;
	} else {
	    if( gLipsync.Version != 2 ){
    		eprintf( "\nError: Lips file WRONG version: %s!", fpath );
	    } else {
		eprintf( "\nLoading current save-file version (2)" );
		if( dbgetBei( fh, &gLipsync.Magic ) ) return -1;
		if( dbgetBei( fh, &gLipsync.Status ) ) return -1;
		if( dbgetBei( fh, &gLipsync.Unk11 ) ) return -1;
    		if( dbgetBei( fh, &gLipsync.Unk14 ) ) return -1;
		if( dbgetBei( fh, &gLipsync.PhonemesCnt ) ) return -1;
		if( dbgetBei( fh, &gLipsync.Unk16 ) ) return -1;
		if( dbgetBei( fh, &gLipsync.MarkersCnt ) ) return -1;
		if( dbreadByteBlk( fh, gLipsync.SpkId, 8 ) == -1 ) return -1;    
		if( dbreadByteBlk( fh, gLipsync.SndExt, 4 ) == -1 ) return -1;
	    }
	}
    }    
    if( !(gLipsync.Phonemes = Malloc( gLipsync.PhonemesCnt )) ){ eprintf( "Out of memory in lips_load_file.\n" ); return -1; }
    if( fh ){
        for( i = 0; i < gLipsync.PhonemesCnt; i++ ){
            if( dbgetb( fh, &gLipsync.Phonemes[ i ] ) ){ eprintf( "lips_load_file: Error reading phoneme type.\n" ); return -1; }                        
        }
        for( i = 0; i < gLipsync.PhonemesCnt; i++ ){
            if( gLipsync.Phonemes[ i ] >= 42 ) eprintf( "\nLoad error: Speech phoneme %d is invalid (%d)!", i, gLipsync.Phonemes[ i ] );
        }
    }
    if( !(gLipsync.Markers = Malloc( gLipsync.MarkersCnt * sizeof( LipsMarker_t ) )) ){ eprintf( "Out of memory in lips_load_file.\n" ); return -1; }
    if( fh ){
	for( i = 0; i < gLipsync.MarkersCnt; i++ ){
    	    if( dbgetBei( fh, &tmp ) == -1 || dbgetBei( fh, &gLipsync.Markers[ i ].Pos ) == -1 ){
        	eprintf( "lips_load_file: Error reading marker type.\n" ); return -1;
    	    } else {
        	gLipsync.Markers[ i ].Type = tmp;
    	    }
	}
	if( gLipsync.Markers[ 0 ].Type >= 2 )  eprintf( "\nLoad error: Speech marker 0 is invalid (%d)!", gLipsync.Markers[ 0 ].Type );
	if( gLipsync.Markers[ 0 ].Pos != 0 ) eprintf( "\nLoad error: Speech marker 0 has invalid position(%d)!", gLipsync.Markers[ 0 ].Pos );	
        for( i = 1; i < gLipsync.MarkersCnt; i++ ){
            if( gLipsync.Markers[ i ].Type >= 2 ) eprintf( "\nLoad error: Speech marker %d is invalid (%d)!", i, gLipsync.Markers[ i ].Type );
            if( gLipsync.Markers[ i ].Pos < gLipsync.Markers[ i - 1 ].Pos ) eprintf( "\nLoad error: Speech marker %d has invalid position(%d)!", i, gLipsync.Markers[ i ].Pos );
        }
    }
    if( fh ) dbClose( fh );
    gLipsync.i15 = 0;
    gLipsync.MarkerSize = 0;
    gLipsync.i19 = 0;
    gLipsync.Position = 0;
    gLipsync.i16 = 50;
    gLipsync.i17 = 100;
    if( gLipsync.Version == 1 ) gLipsync.Magic = LIPMAGIC;
    strcpy( gLipsync.SndExt, "ACM" );
    strcpy( gLipsync.TextExt,"TXT" );
    strcpy( gLipsync.LipExt, "LIP" );
    LipsyncMakeSpeech();
    gLipsPosition = 0;
    gLipsPhoneme = gLipsync.Phonemes[ 0 ];
    return 0;
}

int LipsyncMakeSpeech()
{
    char fname[ 260 ];
DD
    if( gLipsync.Unk12 ){
        Free( gLipsync.Unk12 );
        gLipsync.Unk12 = NULL;
    }
    sprintf( fname, "%s%s/%s.%s", "SOUND/SPEECH/", gLipsFileDir, LipsyncGetFName( gLipsync.SpkId, 8 ), "ACM" );
    if( gLipsync.Snd ){
        SoundDelete( gLipsync.Snd );
        gLipsync.Snd = NULL;
    }
    gLipsync.Snd = SoundCreate( 1, 8 );
    if( !gLipsync.Snd ){ eprintf( "\nsoundAllocate falied in lips_make_speech!" ); return -1; }
    if( SoundSetFileIO( gLipsync.Snd, AudioOpen, AudioRead, AudioClose, NULL, AudioSeek, NULL, AudioLen, AudioGetFmt ) ){ eprintf( "Ack!" ); eprintf( "Error!" ); }
    if( SoundLoadFile( gLipsync.Snd, fname ) ){
        SoundDelete( gLipsync.Snd );
        gLipsync.Snd = NULL;
        eprintf( "lips_make_speech: soundLoad failed with path " );
        eprintf( "%s -- file probably doesn't exist.\n", fname );
        return -1;
    }
printf("=>freq:%i, ch:%i, samples:%i\n", gLipsync.Snd->Freq, gLipsync.Snd->Channels, gLipsync.Snd->Samples );
    gLipsync.MarkerSize = sizeof( LipsMarker_t ) * (gLipsync.Unk14 / gLipsync.MarkersCnt );
    return 0;    
}

int LipsyncClose()
{
    if( gLipsync.Unk12 ){
        Free( gLipsync.Unk12 );
        gLipsync.Unk12 = NULL;
    }
    if( gLipsync.Snd ){
        gLipsPosition = 0;
        SoundStop( gLipsync.Snd );
        gLipsync.Status &= LIPS_CLRMASK;
        SoundDelete( gLipsync.Snd );
        gLipsync.Snd = NULL;
    }
    if( gLipsync.Phonemes ){
        Free( gLipsync.Phonemes );
        gLipsync.Phonemes = NULL;
    }
    if( gLipsync.Markers ){
        Free( gLipsync.Markers );
        gLipsync.Markers = NULL;
    }
    return 0;
}


