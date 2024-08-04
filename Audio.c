#include "FrameWork.h"

int AudioFileExt( char *fname );

int gAudioCnt;
Audio_t *gAudio;
int ( *gAudioCallback )( char *) = AudioFileExt;

/*******************************************************/

int AudioFileExt( char *fname )
{
    char *s;

    s = strrchr( fname, '.' );
    if( s ) strcpy( s + 1, "raw" );
    return 0;
}

int AudioReadBlk( xFile_t *fh, char *Data, int Count )
{
    return dbread( Data, 1, Count, fh );
}

int AudioOpen( char *Name, char mode )
{
    int k, m, i;
    Audio_t *audio, *NewAudio;
    char fname[80],smode[4];
    xFile_t *fh;

    strcpy( fname, Name );
    k = ( gAudioCallback( fname ) ) ? 2 : 0;
    memset( smode, 0, sizeof( smode ) );
    if( mode & 0x01 ){
        m = 1;
        audio = gAudio;
        smode[ 0 ] = 'w';
    } else {
        audio = gAudio;
        if( mode & 0x02 ){
            memcpy( smode, "w+", 2 );
            m = 2;
        } else {
            smode[ 0 ] = 'r';
            m = 1;
        }
    }
    if( mode & 0x01 ){
        gAudio = audio;
        smode[ m ] = 't';
    } else {
        gAudio = audio;
        if( (mode & 2) != 0 ) smode[ m ] = 'b';
    }
    if( !(fh = dbOpen( fname, smode ) ) ){
        eprintf( "AudioOpen: Couldn't open %s for read", fname );
        return -1;
    }
    NewAudio = gAudio;
    for( i = 0; i < gAudioCnt; i++ ){
        if( !(gAudio[ i ].Flags & 1) )break;
    }
    if( i == gAudioCnt ){
        if( gAudio ){
            NewAudio = dbg_realloc( gAudio, sizeof( Audio_t ) * (gAudioCnt + 1) );
        } else {
            gAudio = NULL;
            NewAudio = dbg_malloc( sizeof( Audio_t ) );
        }
        i = gAudioCnt++;
    }
    gAudio = NewAudio;
    gAudio[ i ].Flags = 0x01;
    gAudio[ i ].File = fh;
    if( k == 2 ){	
        gAudio[ i ].Flags |= 0x02;
        gAudio[ i ].Acm = AcmOpen( (void *)AudioReadBlk, gAudio->File, &gAudio->Channels, &gAudio->Freq, &gAudio->Samples );
        gAudio[ i ].Samples *= 2;
    } else {
        gAudio[ i ].Samples = dbFileLength( gAudio[ i ].File );
    }
    gAudio[ i ].FilePos = 0;
    return i + 1;
}

void AudioClose( int Id )
{
    Audio_t *p;

    dbClose( gAudio[ Id - 1 ].File );
    p = &gAudio[ Id - 1 ];
    if( p->Flags & 2 ) AcmClose( p->Acm );
    memset( &gAudio[Id - 1], 0, sizeof( Audio_t ) );
}

int AudioRead( int Id, void *buffer, unsigned int nsize )
{
    Audio_t *p;
    int cnt = 0;

    p = &gAudio[ Id - 1 ];
    if( p->Flags & 2 )
        cnt = AcmInflate( p->Acm, buffer, nsize );
    else
        cnt = dbread( buffer, 1, nsize, p->File );
    p->FilePos += cnt;
    return cnt;
}

int AudioSeek( int Id, int offset, int whence )
{
    int pos, idx,i;
    char *p;

    switch( whence ){
	case SEEK_SET: pos = offset; break;
	case SEEK_CUR: pos = offset + gAudio[ Id - 1 ].FilePos; break;
	case SEEK_END: pos = offset + gAudio[ Id - 1 ].Samples; break;
	default: return 0;
    }
    idx = Id - 1;
    if( !(gAudio[ idx ].Flags & 0x02) ) return dbseek( gAudio[ idx ].File, offset, whence );
    if( pos <= gAudio[ idx ].FilePos ){
        AcmClose( gAudio[ idx ].Acm );
        dbseek( gAudio[ idx ].File, 0, SEEK_SET );
        gAudio[ idx ].Acm = AcmOpen( (void *)AudioReadBlk, gAudio[ idx ].File, &gAudio[ idx ].Channels, &gAudio[ idx ].Freq, &gAudio[ idx ].Samples );
        gAudio[ idx ].FilePos = 0;
        gAudio[ idx ].Samples *= 2;
        if( pos ){
            p = dbg_malloc( 4096 );
            while( pos > 4096 ){
                pos -= 4096;
                AudioRead( Id, p, 4096 );
            }
            if( pos ) AudioRead( Id, p, pos );
            dbg_free( p );
        }
    } else {
        p = dbg_malloc( 1024 );
        for( i = gAudio[ idx ].FilePos - pos; i > 1024; i -= 1024 ){
            AudioRead( Id, p, 1024 );
        }
        if( i ) AudioRead( Id, p, i );
    }
    return gAudio[ Id - 1 ].FilePos;
}

int AudioLen( int Id )
{
    return gAudio[ Id - 1 ].Samples;
}

int AudioTell( int fh )
{
    return gAudio[ fh - 1 ].FilePos;
}

int AudioWrite()
{
    eprintf( "AudioWrite shouldn't be ever called" );
    return 0;
}

void AudioInit( int ( *Callback )( char *) )
{
    gAudioCallback = Callback;
    gAudio = NULL;
    gAudioCnt = 0;
    SoundInitIO( (void *)AudioOpen, (void *)AudioClose, (void *)AudioWrite, (void *)AudioRead, (void *)AudioSeek, (void *)AudioTell, (void *)AudioLen, (void *)AudioGetFmt );
}

void AudioFree()
{
    if( gAudio ) dbg_free( gAudio );
    gAudioCnt = 0;
    gAudio = NULL;
}

int AudioGetFmt( int Id, int *pChannels, int *pFreq, int *pSamples )
{
    if( Id <= 0 || Id > gAudioCnt ) return 0;
    *pChannels = gAudio[ Id - 1 ].Channels;
    *pFreq     = gAudio[ Id - 1 ].Freq;
    *pSamples  = gAudio[ Id - 1 ].Samples;
    return 1;
}






