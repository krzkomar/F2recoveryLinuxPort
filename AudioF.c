#include "FrameWork.h"

Audio_t *gAudioF;
int gAudioFCnt;

int (*gAudioFcallback)(char *) = AudiofExt;

/***********************************/

int AudiofExt( char *fname )
{
    char *p;

    p = strchr( fname, '.' );
    if( p ) strcpy(p + 1, "raw");
    return 0;
}

int AudiofReadFile( FILE *file, void *Buffer, int Size )
{
    return fread( Buffer, 1, Size, file );
}

int AudiofOpen( char *Name, int mode )
{
    int type,k,i;
    char stmp[80], smode[4];
    Audio_t *p, *q;
    FILE *f;

    strcpy( stmp, Name );
    type = gAudioFcallback( stmp ) ? AUDIO_COMPRESSED : AUDIO_RAW;
    memset( smode, 0x00, sizeof( smode ) );
    if( mode & 1 ){
        k = 1;
        p = gAudioF;
        smode[0] = 'w';
    } else {
        p = gAudioF;
        if( mode & 2 ){
            memcpy( smode, "w+", 2 );
            k = 2;
        } else {
            smode[ 0 ] = 'r';
            k = 1;
        }
    }
    if( mode & 1 ){
        gAudioF = p;
        smode[ k ] = 't';
    } else {
        gAudioF = p;
        if( mode & 2 ) smode[ k ] = 'b';
    }
    if( !(f = fopen( stmp, smode )) ) return -1;
    p = gAudioF;
    for( i = 0; i < gAudioFCnt; i++ ){
        if( !(gAudioF[ i ].Flags & 1) ) break;
    }        
    if( i == gAudioFCnt ){
        if( gAudioF ){
            p = dbg_realloc( gAudioF, sizeof( Audio_t ) * ( gAudioFCnt + 1 ) );
        } else {
            gAudioF = NULL;
            p = dbg_malloc( sizeof( Audio_t ) );
        }
        i = gAudioFCnt++;
    }
    q = &p[ i ];
    q->Flags = 1;
    q->rFile = f;
    gAudioF = p;
    if( type == AUDIO_COMPRESSED ){
        q->Flags = q->Flags | 2;
        gAudioF[ i ].Acm = AcmOpen( (void *)AudiofReadFile, q->File, &q->Channels, &q->Freq, &q->Samples );
        gAudioF[ i ].Samples *= 2;
    } else {
        gAudioF[ i ].Samples = filelength( q->rFile );
    }
    gAudioF[ i ].FilePos = 0;
    return i + 1;
}

void AudiofClose( int Id )
{
    Audio_t *p;

    if( Id == 0 ) return;
    
    fclose( gAudioF[ Id - 1 ].rFile );
    p = &gAudioF[ Id - 1 ];
    if( p->Flags & 0x02 ) AcmClose( p->Acm );
    memset( p, 0x00, sizeof( Audio_t ) );
}

int AudiofRead( int Id, void *buffer, int nsize )
{
    Audio_t *p;
    unsigned int r = 0;

    p = &gAudioF[Id - 1];
    if( p->Flags & 0x02 )
        r = AcmInflate( p->Acm, buffer, nsize );
    else
        r = fread( buffer, 1, nsize, p->rFile );
    gAudioF[ Id - 1 ].FilePos += r;
    return r;
}

int AudiofSeek( int Id, int Offset, int whence )
{
    int pos, i;
    void *p;

    switch( whence ){
	case SEEK_SET: pos = Offset; break;
	case SEEK_CUR: pos = Offset + gAudioF[Id - 1].FilePos; break;
	case SEEK_END: pos = Offset + gAudioF[Id - 1].Samples; break;
	default: return 0;
    }
    
    if( !(gAudioF[Id - 1].Flags & 0x02) ) return fseek( gAudioF[Id - 1].rFile, Offset, whence );
    if( pos <= gAudioF[ Id - 1 ].FilePos ){
        AcmClose( gAudioF[ Id - 1 ].Acm );
        fseek( gAudioF[ Id - 1 ].rFile, 0, 0 );
        gAudioF[ Id - 1 ].Acm = AcmOpen( (void *)AudiofReadFile, gAudioF[Id - 1].File, &gAudioF[Id - 1].Channels, &gAudioF[Id - 1].Freq, &gAudioF[Id - 1].Samples );
        gAudioF[ Id - 1 ].FilePos = 0;
        gAudioF[ Id - 1 ].Samples *= 2;
        p = dbg_malloc( 4096 );
        for( ;pos > 4096; p -= 4096 ){
            AudiofRead( Id, p, 4096 );
        }
        if( pos ) AudiofRead( Id, p, pos );
        dbg_free( p );
    } else {
        p = dbg_malloc( 1024 );
        for( i = gAudioF[ Id - 1 ].FilePos - pos; i > 1024; i -= 1024 ){
            AudiofRead( Id, p, 1024 );
        }
        if( i ) AudiofRead( Id, p, i );
    }
    return gAudioF[ Id - 1 ].FilePos;
}

int AudiofLen( int Id )
{
    return gAudioF[ Id - 1 ].Samples;
}

int AudiofTell( int Id )
{
    return gAudioF[ Id - 1 ].FilePos;
}

int AudiofWrite()
{
    eprintf( "AudiofWrite shouldn't be ever called\n" );
    return 0;
}

void AudiofInit( int (*Callback)(char *fname) )
{
    gAudioFcallback = Callback;
    gAudioF = NULL;
    gAudioFCnt = 0;
    SoundInitIO( (void *)AudiofOpen, (void *)AudiofClose, (void *)AudiofWrite, (void *)AudiofRead, (void *)AudiofSeek, (void *)AudiofTell, (void *)AudiofLen, (void *)AudiofGetFmt );
}

void AudiofFree()
{
    if( gAudioF ) dbg_free( gAudioF );
    gAudioFCnt = 0;
    gAudioF = NULL;
}

int AudiofGetFmt( int Id, int *pChannels, int *pFreq, int *pSamples )
{
    if( Id <= 0 || Id > gAudioFCnt ) return 0;
    *pChannels = gAudioF[ Id - 1 ].Channels;
    *pFreq     = gAudioF[ Id - 1 ].Freq;
    *pSamples  = gAudioF[ Id - 1 ].Samples;
    return 1;
}

