#include "FrameWork.h"

char *gMovieFileList[ 17 ] = {
    "iplogo.mve",
    "intro.mve",
    "elder.mve",
    "vsuit.mve",
    "afailed.mve",
    "adestroy.mve",
    "car.mve",
    "cartucci.mve",
    "timeout.mve",
    "tanker.mve",
    "enclave.mve",
    "derrick.mve",
    "artimer1.mve",
    "artimer2.mve",
    "artimer3.mve",
    "artimer4.mve",
    "credits.mve"
};

char *gMoviePalFileList[ 17 ] = { 
    NULL, "art/cuts/introsub.pal", "art/cuts/eldersub.pal", NULL,
    "art/cuts/artmrsub.pal", NULL, NULL, NULL,
    "art/cuts/artmrsub.pal", NULL, NULL, NULL,
    "art/cuts/artmrsub.pal", "art/cuts/artmrsub.pal", "art/cuts/artmrsub.pal", "art/cuts/artmrsub.pal",
    "art/cuts/crdtssub.pal"
};

int gMovieError = 0;
int gMovieFading = 0;

char gMovieList[ 17 ];
char gMovieSubTitlesPath[ 256 ];

/****************************************************************************/

int GMovieInit()
{
    int Volume = 0;

    if( GSoundIsMusicEnabled() ) Volume = GSoundGetMusicVol();
    MveSetMusicVolume( Volume );
    MveSetupFname( GMovieCreateSubtitlesPath );
    memset( gMovieList, 0, sizeof( gMovieList ) );
    gMovieError = 0;
    gMovieFading = 0;
    return 0;
}

void GMovieClose()
{
    memset( gMovieList, 0, sizeof( gMovieList ) );
    gMovieError = 0;
    gMovieFading = 0;
}

int GMovieLoad( xFile_t *fh )
{
    return ( dbread( gMovieList, 1, 17, fh ) == 17 ) - 1;
    return 0;
}

int GMovieSave( xFile_t *fh )
{
    return ( dbwrite( gMovieList, 1, 17, fh ) == 17) - 1;
}

int GMoviePlay( int MovieId, int Flags )
{
    char butt, FilePath[260], *Language, SubsColor;
    int err, Win, flen, CursorHidden, FileLen, SubTit, MseButt, MseY, MseX, SaveFont, flg;

    gMovieError = 1;
    eprintf( "Playing movie: %s\n", gMovieFileList[ MovieId ] );
    err = 0;
    if( CfgGetString( &gConfiguration, "system", "language", &Language ) != 1 ){
        eprintf( "gmovie_play() - Error: Unable to determine language!" );
        gMovieError = 0;
        return -1;
    }
    if( strcasecmp( Language, "english" ) ){
        sprintf( FilePath, "art/%s/cuts/%s", Language, gMovieFileList[ MovieId ] );
        err = dbCheckFileLength( FilePath, &FileLen ) + 1;
    }
    if( !err ){
	sprintf( FilePath, "art/cuts/%s", gMovieFileList[ MovieId ] );
	if( dbCheckFileLength(FilePath, &FileLen) == -1 ){
    	    eprintf( "gmovie_play() - Error: Unable to open %s", gMovieFileList[ MovieId ] );
    	    gMovieError = 0;
	    return -1;
	}
    }
    if( Flags & MOVIE_FADE_IN ){
        FadeStep( gFadePaletteC );
        gMovieFading = 1;
    }
    if( (Win = WinCreateWindow( 0, 0, 640, 480, 0, 16 )) == -1 ){ gMovieError = 0; return -1; }

    if( Flags & MOVIE_BGSND_CLOSE ){
        GSoundBgClose();
    } else if( Flags & MOVIE_BGSND_PAUSE ){
        GSoundBgPause();
    }
    WinUpdate( Win );
    flg = MVE_4;
    SubTit = 0;
    CfgGetInteger( &gConfiguration, "preferences", "subtitles", &SubTit );
    if( SubTit == 1 ){
        if( dbCheckFileLength( GMovieCreateSubtitlesPath( FilePath ), &flen ) )
            SubTit = 0; // no subtitles found
        else
            flg |= MVE_SUBS;
    }
    MveSetFlags( flg );
    if( SubTit == 1 ){
        PalLoadFromFile( gMoviePalFileList[ MovieId ] ? gMoviePalFileList[ MovieId ] : "art/cuts/subtitle.pal" );
        SubsColor = WinGetTextColor();
        WinSetTextColor( 1.0, 1.0, 1.0 );
        SaveFont = FontGetCurrent();
        WinSetFont( MOVIE_SUBS_FONT );
    }
    if( (CursorHidden = MseCursorHidden()) ){
        GmouseLoadCursor( 0 );
        MseCursorShow();
    }
    while( MseGetButtons() ){ InputPoll(); MseUpdate(); };
    MseCursorHide();
    CycleColorStop();
    MveSetEffects( FilePath );
    VidCls();
    MveStart( Win, FilePath );
    butt = 0;
    while( MveIsPlaying() && !gMenuEscape && ( InpUpdate() == -1 ) ){
        MseGetData( &MseX, &MseY, &MseButt );
        butt |= MseButt;
	if(!( ( (butt & 1) == 0 && (butt & 2) == 0 ) || ((MseButt & 1) != 0 || (MseButt & 2) != 0) ) ) break;
    };
    MveAbort();
    MveEffectsClear();
    MvePlay();
    // finish movie
    FadeSetPalette( gFadePaletteC ); // restore palette
    gMovieList[ MovieId ] = 1;
    CycleColorStart();
    GmouseLoadCursor( 1 );
    if( !CursorHidden ) MseCursorShow();
    if( SubTit == 1 ){
        PalLoadFromFile( "color.pal" );
        WinSetFont( SaveFont );
        WinSetTextColor(
    	    RGB16_GET_R( PalConvColor8to16( SubsColor ) ) * 0.032258064, 
    	    RGB16_GET_G( PalConvColor8to16( SubsColor ) ) * 0.032258064, 
    	    RGB16_GET_B( PalConvColor8to16( SubsColor ) ) * 0.032258064
    	);
    }
    WinClose( Win );
    if( Flags & MOVIE_BGSND_PAUSE ) GSoundBgUnPause();
    if( Flags & MOVIE_FADE_OUT ){
        if( SubTit != 1 ) PalLoadFromFile( "color.pal" );
        FadeStep( gPalBase );
        gMovieFading = 0;
    }
    gMovieError = 0;
    return 0;
}

void GMovieFade()
{
    if( gMovieFading != 1 ) return;
    FadeStep( gPalBase );
    gMovieFading = 0;    
}

int GMovieEnabled( int MovieId )
{
    return gMovieList[ MovieId ] == 1;
}

int GMovieGetError()
{
    return gMovieError;
}

char *GMovieCreateSubtitlesPath( char *Path )
{
    char *s, *language;

    language = 0;
    CfgGetString( &gConfiguration, "system", "language", &language );
    s = strrchr( Path, '/' );
    if( s ) Path = s + 1;

    sprintf( gMovieSubTitlesPath, "text/%s/cuts/%s", language, Path );
    s = strrchr( gMovieSubTitlesPath, '.' );
    if( *s ) *s = '\0';
    strcpy( &gMovieSubTitlesPath[ strlen( gMovieSubTitlesPath ) ], ".sve" );
    return gMovieSubTitlesPath;
}


