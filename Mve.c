#include "FrameWork.h"

VidRect_t gMveWinArea;
VidRect_t gMveArea;
int (*gMveUnk23)(void);
int (*gMveUnk01)();
int (*gMveFadingCb)(void);
int (*gMveUnk24)(void);
char *(*gMveGetSubsFname)(void);
int (*gMvePrepCb)(void);
unsigned int gMveSubsWidth;
int gMveUnk28;
int gMveUnk29;
int gMveXpos;
int gMveYpos;
int gMveScaled;
int (*gMveUnk33)(void);
int gMveUnk34;
int gMveUnk35;
int gMveUnk36;
int gMveUnk37;
MveSubTitle_t *gMveSubtitles;
int gMveStat;
int gMveOpened;
int gMveNotFullScreen; // scaled
int gMveHeight;
int gMvePosOffset;
int (*gMveUnk42)();
//void *gMveSurface;
int gMveWidth;
int (*gMveUnk45)();
unsigned int gMveSubsHeight;
int gMvePlaying = 0;
xFile_t *gMveFile;
unsigned char *gMveScreen;
int gMvePosX;
int gMvePosY;
int gMveAudioEn;
xFile_t *gMveMraFile;
unsigned char *gMveUnk53;
Pal8_t gMveFx[768];
char gMveUnk55[768];
char gMvePal[768];
int gMveUnk57;

int gMveWin = -1;
int gMveSubsFont = -1;
int (*gMveFunc[8])() = {
    MveUnk13,
    MveUnk13,
    MveUnk12,
    MveUnk09,
    MveUnk11,
    MveUnk11,
    MveUnk10,
    MveUnk10
};
void (*gMvePalMod)(Pal8_t *, int first, int last) = (void *)PalModulate;
int gMveSubsR = 31;
int gMveSubsG = 31;
int gMveSubsB = 31;
unsigned int gMveEffectsFlg = 0;
MveFade_t *gMveFade = NULL;

/****************************************************/
void MveUnk02( int (*cb)(void) )
{
    gMveUnk33 = cb;
}

void MveUnk03( int (*cb)(void) )
{
    gMveUnk24 = cb;
}

void MveUnk01( int (*cb1)(void), int (*cb2)() )
{
    gMveUnk01 = cb2;
    gMvePrepCb = cb1;
}

void *MveAlloc( int bytes )
{
    return dbg_malloc( bytes );
}

void MveFree( void *ptr )
{
    dbg_free( ptr );
}

int MveGetData( xFile_t *fh, char *Buffer, int nbytes )
{
    return dbread( Buffer, 1, nbytes, fh ) == nbytes;
}

int MveBlitDirect( char *Surf, int Width, int Height, int PosX, int PosY, int w, int h, int aa, int ab )
{
    int w1, h1;
    VidRect_t RectSrc, RectDst;

    RectSrc.lt = PosX;
    RectSrc.tp = PosY;
    RectSrc.rt = PosX + Width;
    RectSrc.bm = PosY + Height;
    w1 = gMveWinArea.rt - gMveWinArea.lt + 1;
    h1 = gMveWinArea.bm - gMveWinArea.tp + 1;
    if( gMveScaled ){
        if( gMveStat & 0x08 ){
            RectDst.tp = ( h1 - h ) / 2;
            RectDst.lt = ( w1 - 4 * Width / 3 ) / 2;
        } else {
            RectDst.tp = gMvePosY + gMveWinArea.tp;
            RectDst.lt = gMveWinArea.lt + gMvePosX;
        }
        RectDst.rt = 4 * Width / 3 + RectDst.lt;
        RectDst.bm = h + RectDst.tp;
    } else {
        if( gMveStat & 0x08 ){
            RectDst.tp = ( h1 - h ) / 2;
            RectDst.lt = ( w1 - w ) / 2;
        } else {
            RectDst.tp = gMvePosY + gMveWinArea.tp;
            RectDst.lt = gMveWinArea.lt + gMvePosX;
        }
        RectDst.rt = w + RectDst.lt;
        RectDst.bm = h + RectDst.tp;
    }
    gMveXpos = PosX;
    gMveYpos = PosY;
    gMveUnk36 = RectDst.lt;
    gMveUnk37 = RectDst.tp;
    gMveUnk28 = Height;
    gMveUnk35 = RectDst.rt - RectDst.lt;
    gMveUnk29 = Width;
    gMveUnk34 = RectDst.bm - RectDst.tp;
    if( gMveUnk42 ) gMveUnk42( Surf + Width * PosY + PosX, Width, Height, Width, RectDst.lt, RectDst.tp, RectDst.rt - RectDst.lt, RectDst.bm - RectDst.tp );
    VidCopy16( Surf, Width, 0, RectSrc.lt, RectSrc.tp, Width, Height, RectDst.lt, RectDst.tp );
    return 0;
}

void MveBlitBuffered( char *Surf, int Width, int Height, int PosX, int PosY, int w, int h, int aa, int ab )
{
    if( gMveWin == -1 ) return;
    gMveUnk29 = Width;
    gMveUnk28 = Width;
    gMveUnk35 = w;
    gMveUnk34 = h;
    gMveUnk36 = PosX;
    gMveUnk37 = PosY;
    gMveXpos = PosX;
    gMveYpos = PosY;
//    if( gMveUnk42 ) gMveUnk42( dsc_16 + PosY * v10 + PosX, Width, Height, Width, gMveArea.lt, gMveArea.tp, Surf, Width );
    if( gMveUnk45 ){
//	gMveUnk45( dsc_76, Width );
    } else {
//        if( !gMveFunc[ gMveNotFullScreen + 2 * gMveScaled + 4 * gMveOpened ]( gMveWin, dsc_16 + PosY * v10 + PosX, Width, Height, dsc_76 ) ) return;
//        if( gMveUnk33 ) gMveUnk33();
        WinAreaUpdate( gMveWin, &gMveArea );
    }
}

void MveUnk05( int (*Cb)() )
{
    gMveUnk45 = Cb;
}

void MveUnk06( int (*Cb)())
{
    gMveUnk42 = Cb;
}

void MveUnk07( void **pA, int *pB, int *pC, int *pD, int *pE, int *pG, int *pH )
{
    if( !pA ) return;        
    *pB = gMveUnk29;
    *pC = gMveUnk28;
    *pD = gMveUnk36;
    *pE = gMveUnk37;
    *pG = gMveUnk35;
    *pH = gMveUnk34;
    *pA = NULL;            
}

void MveUpdate( int WinId, char *a2, int a3, int a4 )
{
    VidRect_t Area;

    if( !gMveFunc[ 4 * gMveOpened + 2 * gMveScaled + gMveNotFullScreen ]( WinId, a2, a3, a4, a3 ) ) return;
    Area.lt = gMvePosX;
    Area.tp = gMvePosY;
    Area.rt = a3 + gMvePosX;
    Area.bm = a3 + gMvePosY;
    WinAreaUpdate( WinId, &Area );
}

int MveUnk09( int WinId, char *pSrc, int Width, int Height, int Pitch )
{
    int i,j,w;
    char *p;

    w = WinGetWidth( WinId );
    p = WinGetSurface( WinId ) + gMvePosY * w + gMvePosX;
    if( 4 * Width / 3 > gMveWidth ){ gMveStat |= MVE_STAT_ERROR; return 0; }
    while( --Height != -1 ){
        for( i = 0; i < Width / 3; i++ ){
    	    *p++ = *pSrc++;
    	    *p++ = *pSrc++;
    	    *p++ = *pSrc++;
    	    *p++ = *pSrc;
        }        
        for( j = 3 * i; j < Width; j++ ) *p++ = *pSrc++;
        pSrc += Pitch - Width;
        p += w - gMveWidth;        
    }
    return 1;
}

int MveUnk10( int WinId, char *pSrc, int Width, int Height, int Pitch )
{
    gMveStat |= MVE_STAT_ERROR;
    return 0;
}

int MveUnk11( int WinId, char *pSrc, int Width, int Height, int Pitch )
{
    int w;

    w = WinGetWidth( WinId );
    WinUnk201( pSrc, Width, Height, Pitch, gMveScreen, gMveUnk53, WinGetSurface( WinId ) + gMvePosY * w + gMvePosX, w );
    return 1;
}

int MveUnk12( int WinId, char *pSrc, int Width, int Height, int Pitch )
{
    char *Surface;
    int n;

    if( Width != 3 * WinGetWidth( WinId ) / 4 ){ gMveStat |= MVE_STAT_ERROR; return 0; }
    Surface = WinGetSurface( WinId );
    while ( --Height != -1 ){
        n = Width / 3;
        while( --n != -1 ){
            *Surface++ = *pSrc++;
            *Surface++ = *pSrc++;
            *Surface++ = *pSrc++;
            *Surface++ = *pSrc;
        }
        pSrc += Pitch - Width;
    }
    return 1;
}

int MveUnk13( int WinId, char *pSrc, int Width, int Height, int Pitch )
{
//    int w;

//    w = WinGetWidth( WinId );
//    WinUnk70( WinGetSurface( WinId ) + w * gMvePosY + gMvePosX , gMveWidth, gMveHeight, w, pSrc, Width, Height, Pitch );
    return 1;
}

void MveSetPal( Pal8_t *pal, int first, int cnt )
{
    if( cnt ) gMvePalMod( pal, first, first + cnt - 1 );
}

int MveNullHandler()
{
    return 0;
}

void MveInit()
{
    Sound_t *snd;
    
    MovSetupMemory( MveAlloc, MveFree );
    snd = SoundCreate( 0, 0 );
    MovSetSoundDrv( snd );
    gMveAudioEn = (snd != NULL);
    MovSetupVideo( 1 ); // !!
    MovSetupPalette( MveSetPal );
    MovSetAspect( 640, 480, 480, 0, NULL, 0, 0, NULL, 0 );
    MovSetInput( MveGetData );
}

void MveFinish( int StopPlay )
{
    MveSubTitle_t *p;
    char *Surface;
    int Width,Dropped,dropped;

    if( !gMvePlaying ) return;
    if( gMveUnk01 ) gMveUnk01();
    
    MovGetFrameNo( &dropped, &Dropped );
    eprintf( "Frames %d, dropped %d\n", dropped, Dropped );
    if( StopPlay ) MovStop();
    MovClose();
    dbClose( gMveFile );
    if( gMveScreen ){
        Width = WinGetWidth( gMveWin );
        Surface = WinGetSurface( gMveWin );
        ScrCopy( gMveScreen, gMveWidth, gMveHeight, gMveWidth, &Surface[ gMvePosY * Width + gMvePosX ], Width );
        WinAreaUpdate( gMveWin, &gMveArea );
    }
    if( gMveMraFile ){
        dbClose( gMveMraFile );
        gMveMraFile = NULL;
    }
    if( gMveUnk53 ){
        dbg_free( gMveUnk53 );
        gMveUnk53 = NULL;
    }
    if( gMveScreen ){
        dbg_free( gMveScreen );
        gMveScreen = NULL;
    }
    while( gMveSubtitles ){
        dbg_free( gMveSubtitles->Line );
        p = gMveSubtitles->Next;
        dbg_free( gMveSubtitles );
        gMveSubtitles = p;
    }
    gMvePlaying = 0;
    gMveNotFullScreen = 0;
    gMveScaled = 0;
    gMveOpened = 0;
    gMveStat = 0;
    gMveWin = -1;    
}

void MveClose()
{
    MveFinish( 1 );
}

void MveAbort()
{
    if( gMvePlaying ) gMveStat |= MVE_STAT_ABORT;
}

int MveSetFlags( char flags )
{
    if( flags & MVE_4 ){
        gMveStat |= ( MVE_STAT_8 | MVE_STAT_DIRECT );
    } else {
        gMveStat &= ~MVE_STAT_8;
        if( flags & MVE_STAT_ABORT )
            gMveStat |= MVE_STAT_DIRECT;
        else
            gMveStat &= ~MVE_STAT_DIRECT;
    }
    if( flags & MVE_SCALED ){
        gMveScaled = 1;
        if( gMveStat & MVE_STAT_DIRECT ) MovSetScale( 3 );
    } else {
        gMveScaled = 0;
        if( gMveStat & MVE_STAT_DIRECT )
            MovSetScale( 4 );
        else
            gMveStat &= ~MVE_STAT_8;
    }
    if( flags & MVE_SUBS )
        gMveStat |= MVE_STAT_SUBS;
    else
        gMveStat &= ~MVE_STAT_SUBS;
    return 0;
}

void MveUnk19( int result )
{
    gMveSubsFont = result;
}

void MveUnk21( float r, float g, float b )
{
    gMveSubsG = lround(31.0 * r);
    gMveSubsB = lround(31.0 * g);
    gMveSubsR = lround(31.0 * b);
}

void MveSetPaletteCb( int (*modulator)() )
{
    if( modulator )
        gMvePalMod = (void *)modulator;
    else
        gMvePalMod = (void *)PalModulate;
}

void MveSetFadingCb( int (*Cb)(void) )
{
    gMveFadingCb = Cb;
}

int MveOpenMRA( char *fname )
{
    xFile_t *fh;
    int buff[3];

    fh = dbOpen( fname, "rb" );
    gMveMraFile = fh;
    if( !fh ) return 1;
    buff[0] = dbgetBei( fh, buff );
    if( buff[0] == 0x4D524130 ){ // 'MRA0'
        dbgetBei( gMveMraFile, buff );
        gMveOpened = 1;
	return 0;
    }
    dbClose( gMveMraFile );
    gMveMraFile = 0;
    return 1;
}

void MveSurfaceFree()
{
}

xFile_t *MveOpenMovie( char *fname )
{
    gMveFile = dbOpen( fname, "rb" );
    if( !gMveFile ){
        if( !gMveUnk24 ){
            eprintf( "Couldn't find movie file %s\n", fname );
            return NULL;
        }
        while( !gMveFile && gMveUnk24() ) gMveFile = dbOpen( fname, "rb" );
    }
    return gMveFile;
}

void MveLoadSubtitles( char *fname )
{
    xFile_t *fh;
    MveSubTitle_t *p, *NewSub;
    char *s, stmp[ 260 ];
    int SubTitlesCnt;

    gMveSubsWidth = WinGetScreenWidth();
    gMveSubsHeight = gFont.ChrHeight() + 4;
    if( gMveGetSubsFname ) fname = gMveGetSubsFname();
    strcpy( stmp, fname );
    eprintf( "Opening subtitle file %s\n", stmp );    
    if( !(fh = dbOpen( stmp, "r" ) ) ){
        eprintf( "Couldn't open subtitle file %s\n", stmp );
        gMveStat &= ~MVE_STAT_SUBS;
        return;
    }
    p = NULL;
    SubTitlesCnt = 0;
    while( !dbfeof( fh ) ){
        stmp[ 0 ] = '\0';
        dbgets( stmp, 259, fh );
        if( stmp[ 0 ] == '\0' ) break;
        NewSub = dbg_malloc( sizeof( MveSubTitle_t ) );
	NewSub->Next = NULL;
        SubTitlesCnt++;
        if( (s = strchr( stmp, '\n' ) ) ) *s = '\0';        
        if( (s = strchr( stmp, '\r' ) ) ) *s = '\0';        
        if( !(s = strchr( stmp, ':' ) ) ){
            eprintf( "subtitle: couldn't parse %s\n", stmp );
            continue;
        }
        *s = '\0';
        NewSub->Frame = strtol( stmp, NULL, 0 );
        NewSub->Line = strdup( s + 1 );
        if( !p )
            gMveSubtitles = NewSub;
        else
            p->Next = NewSub;
        p = NewSub;
    }
    dbClose( fh );
    eprintf( "Read %d subtitles\n", SubTitlesCnt );
}

void MvePrintSubs( )
{
    MveSubTitle_t *Next;
    VidRect_t NewArea;
    int Current, ypos, Dropped, Frame;

    if( gMveSubtitles && (gMveStat & MVE_STAT_SUBS) == 0 ) return;    
    ypos = gMveUnk34 + gMveUnk37 + ( 480 - gMveUnk37 - gMveUnk34 - gFont.ChrHeight() ) / 2;
    if( (ypos + gMveSubsHeight) > WinGetScreenHeight() ) gMveSubsHeight = WinGetScreenHeight() - ypos;
    MovGetFrameNo( &Frame, &Dropped );
    while( gMveSubtitles ){
        if( Frame < gMveSubtitles->Frame ) break;
        Next = gMveSubtitles->Next;
        WinDrawFilledRect( gMveWin, 0, ypos, gMveSubsWidth, gMveSubsHeight, 0 ); // clear text box
        if( gMveSubsFont != -1 ){
            Current = FontGetCurrent();
            FontSet( gMveSubsFont );
        }
        WinPrintMessage( gMveWin, gMveSubtitles->Line, gMveSubsWidth, gMveSubsHeight, 0, ypos, gPalColorCubeRGB[ gMveSubsR ][ gMveSubsG ][ gMveSubsB ] | 0x2000000, 2 );
        NewArea.lt = 0;
        NewArea.tp = ypos;
        NewArea.rt = gMveSubsWidth;
        NewArea.bm = ypos + gMveSubsHeight;
        WinAreaUpdate( gMveWin, &NewArea );
        dbg_free( gMveSubtitles->Line );
        dbg_free( gMveSubtitles );
        gMveSubtitles = Next;
        if( gMveSubsFont != -1 ) FontSet( Current );
    }
}

int MvePrep( int WinId, char *MovieFile, int (*AfterFrameCb)() )
{
    int w, pword;
    short unk1;

    if( gMvePlaying ) return 1;
    MveSurfaceFree();
    if( !(gMveFile = MveOpenMovie( MovieFile ) ) ) return 1;
    gMveWin = WinId;
    gMvePlaying = 1; // enable playing in background
    gMveStat &= ~MVE_STAT_ERROR; // clear error
    if( gMveStat & MVE_STAT_SUBS ) MveLoadSubtitles( MovieFile );
    if( gMveStat & MVE_STAT_DIRECT ){
        eprintf( "Direct " );
        WinGetRect( gMveWin, &gMveWinArea );
        eprintf( "Playing at (%d, %d)  ", gMvePosX + gMveWinArea.lt, gMvePosY + gMveWinArea.tp );
        MovSetupAfterFrameCb( AfterFrameCb );
        MovSetupBlitter( MveBlitDirect );
	MovPrep( gMveFile, gMvePosX + gMveWinArea.lt, gMvePosY + gMveWinArea.tp, 0 );
    } else {
        eprintf( "Buffered " );
        MovSetupAfterFrameCb( AfterFrameCb );
        MovSetupBlitter( (void *)MveBlitBuffered );
	MovPrep( gMveFile, 0, 0, 0 );
    }
    eprintf( "%s", ( gMveScaled ) ? "scaled" : "not scaled" );
    if( gMvePrepCb ) gMvePrepCb();
    if( gMveMraFile ){
        dbgetBei( gMveMraFile, &pword );
        dbgetBew( gMveMraFile, &unk1 );
        dbgetBew( gMveMraFile, &unk1 );
        gMveUnk53 = dbg_malloc( pword );
        gMveScreen = dbg_malloc( gMveHeight * gMveWidth  );
        w = WinGetWidth( gMveWin );
        ScrCopy( WinGetSurface( gMveWin ) + gMvePosY * w + gMvePosX, gMveWidth, gMveHeight, w, gMveScreen, gMveWidth );
    }
    gMveArea.lt = gMvePosX;
    gMveArea.tp = gMvePosY;
    gMveArea.rt = gMveWidth + gMvePosX;
    gMveArea.bm = gMveHeight + gMvePosY;
    return 0;
}

int MveHandler()
{
    MvePrintSubs();
    if( gMveUnk23 ) gMveUnk23();
    return InpUpdate() != -1;
}

int MveStartSubs( int WinId, char *fname, int (*a2)(void) ) // no xref
{
    VidRect_t pArea;

    MveSurfaceFree();
    eprintf( "runToEnd\n" );
    gMveFile = MveOpenMovie( fname );
    if( !gMveFile ) return 1;
    gMveWin = WinId;
    gMvePlaying = 1;
    gMveStat &= ~MVE_STAT_ERROR;
    if( gMveStat & MVE_STAT_SUBS ) MveLoadSubtitles( fname );
    gMveUnk23 = a2;
    MovSetupAfterFrameCb( MveHandler );
    MovSetupBlitter( MveBlitDirect );
    gMvePosX = 0;
    gMvePosY = 0;
    gMvePosOffset = 0;
    gMveWidth = WinGetWidth( WinId );
    gMveArea.lt = gMvePosX;
    gMveArea.tp = gMvePosY;
    gMveArea.rt = gMveWidth + gMvePosX;
    gMveArea.bm = gMvePosY + gMveHeight;
    gMveHeight = WinGetHeight( WinId );
    gMveNotFullScreen = 0;
    InpTaskUnHalt();
    WinGetRect( gMveWin, &pArea );
    MovPlay( gMveFile, gMvePosX + gMveWinArea.lt, gMvePosY + gMveWinArea.tp, 0 );
    MveFinish( 0 );
    InpTaskHalt();
    return 0;
}

int MveStart( int WinId, char *fname )
{
    if( gMvePlaying ) return 1;
    gMvePosX = 0;
    gMvePosY = 0;
    gMvePosOffset = 0;
    gMveWidth  = WinGetWidth( WinId );
    gMveHeight = WinGetHeight( WinId );
    gMveNotFullScreen = 0;
    return MvePrep( WinId, fname, MveNullHandler );
}

int MveStartAt( int WinId, char *fname, int xpos, int ypos, int w, int h )
{
    if( gMvePlaying ) return 1;
    gMvePosX = xpos;
    gMvePosY = ypos;
    gMvePosOffset = xpos + ypos * WinGetWidth( WinId );
    gMveWidth = w;
    gMveHeight = h;
    gMveNotFullScreen = 1;
    return MvePrep( WinId, fname, MveNullHandler );
}

int MvePlayChunk()
{
    unsigned int ChunkSize;
    int err;

    if( gMveMraFile ){
        dbgetBei( gMveMraFile, (int *)&ChunkSize );
        dbread( gMveUnk53, 1, ChunkSize, gMveMraFile );
    }
    if( (err = MovStep()) == -1 ) return -1;
    MvePrintSubs();    
    return err;
}

void MveSetupFname( char *(*cb)() )
{
    gMveGetSubsFname = cb;
}

void MveSetMusicVolume( int vol )
{
    if( !gMveAudioEn ) return;
    MovSetSndVolume( SoundVolConvert( vol ) );
}

void MvePlay()
{
    int frame, drop;
    
    if( !gMvePlaying ) return;    
    if( gMveStat & MVE_STAT_ABORT ){
        eprintf( "Movie aborted\n" );
        MveFinish( 1 );
        return;
    } else if( gMveStat & MVE_STAT_ERROR ){
        eprintf( "Movie error\n" );
        MveFinish( 1 );
        return;
    }        
    if( MvePlayChunk() == -1 ){
        MveFinish( 1 );
    } else if( gMveFadingCb ){
        MovGetFrameNo( &frame, &drop );
        gMveFadingCb();
    }        
}

int MveIsPlaying()
{
    return gMvePlaying;
}

void MveSetupAudio( int Enable )
{
    if( Enable ){
        gMveAudioEn = 1;
//        MovSetSoundDrv( DirectSound );
    } else {
        gMveAudioEn = 0;
//        MovSetSoundDrv( 0 );
    }
}


int MveFxInit()
{
    if( gMveEffectsFlg ) return -1;
    memset( gMveFx, 0, sizeof( gMveFx ) );
    gMveEffectsFlg = 1;
    return 0;
}

void MveUnk37()
{
    if( !gMveEffectsFlg ) return;
    MveSetFadingCb( NULL );
    MveSetPaletteCb( NULL );
    MveFadeClear();
    gMveUnk57 = 0;
    memset( gMveFx, 0, sizeof( gMveFx ) );
}

void MveUnk38()
{
    if( !gMveEffectsFlg ) return;
    MveSetFadingCb( NULL );
    MveSetPaletteCb( NULL );
    MveFadeClear();
    gMveUnk57 = 0;
    memset( gMveFx, 0, sizeof( gMveFx ) );
    gMveEffectsFlg = 0;
}

int MveSetEffects( char *Path )
{
    MveFade_t *FadeNew, *tmp;
    Config_t Cfg;
    int FadeColor[ 3 ];
    char *s, CfgPath[ 260 ], FrameNumber[4], *FadeTypeName;
    int *EffectTable,IntParamList,i,FirstFrame,TotalEfx,FadeType,FadeSteps,err;

    err = -1;
    if( !gMveEffectsFlg ) return -1;    
    MveSetFadingCb( NULL );
    MveSetPaletteCb( NULL );
    MveFadeClear();
    gMveUnk57 = 0;
    memset( gMveFx, 0, sizeof( gMveFx ) );
    if( !Path ) return -1;
    if( !CfgInit( &Cfg ) ) return -1;    
    strcpy( CfgPath, Path );
    if( (s = strchr( CfgPath, '.' )) ) *s = '\0';
    strcpy( &CfgPath[ strlen( CfgPath ) ], ".cfg" );
    if( CfgLoadFromFile( &Cfg, CfgPath, 1 ) == 1 && CfgGetInteger( &Cfg, "info", "total_effects", &TotalEfx ) == 1 && TotalEfx > 0 ){
        EffectTable = Malloc( sizeof( int ) * TotalEfx );
        if( EffectTable ){
            if( TotalEfx >= 2 )
                IntParamList = CfgGetIntParamList( &Cfg, "info", "effect_frames", EffectTable, TotalEfx );
            else
                IntParamList = CfgGetInteger( &Cfg, "info", "effect_frames", EffectTable );
            if( IntParamList == 1 ){
                for( i = 0; i < TotalEfx; i++ ){
                    sprintf( FrameNumber, "%i", EffectTable[ i ] );
                    if( CfgGetString( &Cfg, FrameNumber, "fade_type", &FadeTypeName ) != 1 ) continue;
                    FadeType = 0;
                    if( !strcasecmp( FadeTypeName, "in" ) ){
                        FadeType = 1;
                    } else {
                        if( !strcasecmp( FadeTypeName, "out" ) ) FadeType = 2;
                    }
                    if( !FadeType ) continue;                    
                    if( CfgGetIntParamList( &Cfg, FrameNumber, "fade_color", (int *)&FadeColor, 3 ) != 1 ) continue;
                    if( CfgGetInteger( &Cfg, FrameNumber, "fade_steps", &FadeSteps ) != 1 ) continue;
                    if( !(FadeNew = Malloc( sizeof( MveFade_t ) )) ) continue;
                    memset( FadeNew, 0, sizeof( MveFade_t ) );
                    FadeNew->FirstFrame = EffectTable[ i ];
                    FadeNew->LastFrame = FadeSteps + EffectTable[ i ] - 1;
                    FadeNew->Steps = FadeSteps;
                    FadeNew->Type = FadeType;
                    FadeNew->Color.r = FadeColor[0];
                    FadeNew->Color.g = FadeColor[1];
                    FirstFrame = FadeNew->FirstFrame;
                    FadeNew->Color.b = FadeColor[2];
                    if( FirstFrame <= 1 ) gMveUnk57 = 1;
                    tmp = gMveFade;
                    gMveFade = FadeNew;
                    FadeNew->Next = (void *)tmp;
                }
                if( i ){
                    MveSetFadingCb( (void *)MveSetFadeEFX );
                    MveSetPaletteCb( (void *)MveSetPaletteEFX );
                    err = 0;
                }
            }
            Free( EffectTable );
        }
    }
    CfgFree( &Cfg );
    return err;
}

void MveEffectsClear()
{
    MveFade_t *p, *n;

    if( !gMveEffectsFlg ) return;
    MveSetFadingCb( NULL );
    MveSetPaletteCb( NULL );
    p = gMveFade;
    while ( p ){
    	n = (void *)p->Next;
        Free( p );
        p = n;
    }
    gMveFade = NULL;
    gMveUnk57 = 0;
    memset( gMveFx, 0, sizeof( gMveFx ) );
}

void MveSetFadeEFX( int CurrentFrame )
{
    Pal8_t NewPal[ 256 ];
    MveFade_t *p;
    int n, i;
    
    for( p = gMveFade; p; p = (void *)p->Next ){
        if( CurrentFrame >= p->FirstFrame && CurrentFrame <= p->LastFrame){
    	    n = CurrentFrame - p->FirstFrame + 1;
    	    if( p->Type == 1 ){
    		for( i = 0; i < 256; i++ ){
        	    NewPal[ i ].g = p->Color.r - n * ( p->Color.r - gMveFx[ i ].r ) / p->Steps;
        	    NewPal[ i ].b = p->Color.g - n * ( p->Color.g - gMveFx[ i ].g ) / p->Steps;
        	    NewPal[ i ].r = p->Color.b - n * ( p->Color.b - gMveFx[ i ].b ) / p->Steps;
    		}
    	    } else {
    		for( i = 0; i < 256; i++ ){
        	    NewPal[ i ].g = gMveFx[ i ].r - n * ( gMveFx[ i ].r - p->Color.r ) / p->Steps;
        	    NewPal[ i ].b = gMveFx[ i ].g - n * ( gMveFx[ i ].g - p->Color.g ) / p->Steps;
        	    NewPal[ i ].r = gMveFx[ i ].b - n * ( gMveFx[ i ].b - p->Color.b ) / p->Steps;
    		}
    	    }
    	    FadeSetPalette( NewPal );
            break;
        }        
    }
    gMveUnk57 = (p != NULL);
}

void MveSetPaletteEFX( Pal8_t *Palette, int FirstColor, int LastColor )
{
    memcpy( &gMveFx[ FirstColor ], Palette, sizeof( Pal8_t ) * (LastColor - FirstColor + 1 ) );
    if( !gMveUnk57 ) FadePalMod( Palette, FirstColor, LastColor );
}

void MveFadeLink( MveFade_t *fade )
{
    MveFade_t *p;

    p = gMveFade;
    gMveFade = fade;
    fade->Next = (void *)p;
}

void MveFadeClear()
{
    MveFade_t *p, *n;

    p = gMveFade;
    while( p ){
	n = (void *)p->Next;
        Free( p );
        p = n;
    };
    gMveFade = NULL;
}



