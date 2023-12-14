#include "Error.h"

#define TEXT_MODE_BASE_MONO_ADDR	0xB0000;
#define TEXT_MODE_BASE_VGA_ADDR		0xB8000

static FILE *gErrorLogFileHandler = NULL;
//static int gErrorCursorXpos = 0;
//static int gErrorCursorYpos = 0;
static int (*gErrorOutput)(const char *) = NULL;

int ErrorCloseLogTaskAtExit()
{
//!!    return AtExit( ErrorCloseLogFile );
return 0;
}

void ErrorSetOutMGA()
{
    if( (void *)gErrorOutput == (void *)ErrorMonoPuts ) return;
    if( gErrorLogFileHandler ){
        fclose( gErrorLogFileHandler );
        gErrorLogFileHandler = NULL;
    }
    gErrorOutput = (void *)ErrorMonoPuts;
    ErrorCls();
}

void ErrorSetOutFile( const char *fpath, const char *mode )
{
    if( (*mode == 'w' || *mode == 'a') && mode[1] == 't' ){
        if( gErrorLogFileHandler ) fclose( gErrorLogFileHandler );
        gErrorLogFileHandler = fopen( fpath, mode );
        gErrorOutput = (void *)ErrorFileOut;
    }
}

void ErrorSetOutVGA()
{
    if( (void *)gErrorOutput == (void *)ErrorVgaPuts ) return;
    if( gErrorLogFileHandler ){
        fclose(gErrorLogFileHandler);
        gErrorLogFileHandler = NULL;
    }
    gErrorOutput = (void *)ErrorVgaPuts;
}

/*
    Checks value of envirnment variable DEBUGACTIVE
	'mono' - redirects logs to MGA (Hercules)
	'log'  - redirects logs to file 'debug.log'
	'screen' - redirects logs to VGA text mode screen
	'gnw' - redirects logs to in game window system
*/
void ErrorLogSetup()
{
    const char *env;
    char *penv;
    unsigned int i;

    env = getenv( "DEBUGACTIVE" );
    if( !env ) return;
    if( !(penv = Malloc( strlen( env ) + 1 )) ) return;
    strcpy( penv, env );
    for( i = 0; i < strlen( penv ); i++ ) penv[ i ] = tolower( penv[ i ] );
    if( !strcmp( penv, "mono" ) ){
	ErrorSetOutMGA();
    } else if( !strcmp( penv, "log" ) ){
        ErrorSetOutFile( "wt", "debug.log" );
    } else if( !strcmp( penv, "screen" ) ){
	ErrorSetOutVGA();
    } else if( !strcmp( penv, "gnw" ) ){
//!!        if( gErrorOutput != WidgetPutsLog ){
            if( gErrorLogFileHandler ){
                fclose(gErrorLogFileHandler);
                gErrorLogFileHandler = NULL;
            }
//!!            gErrorOutput = WidgetPutsLog;
//!!        }
    }
    Free( penv );
}

void ErrorSetOutCb( void (*Cb)(char *str) )
{
    if( Cb == (void *)gErrorOutput ) return;    
    if( gErrorLogFileHandler ){
        fclose( gErrorLogFileHandler );
        gErrorLogFileHandler = NULL;
    }
    gErrorOutput = (void *)Cb;    
}

int ErrorPrintf( const char *fmt, ... )
{
    char str[256];
    va_list vl, va;

    va_start( va, fmt );
    if( !gErrorOutput ) return -1;
    va_copy( vl, va );
    vsprintf( str, fmt, vl );
    return gErrorOutput( str );
}

int ErrorPuts( char *str )
{
    if( gErrorOutput )
        return gErrorOutput( str );
    else
        return -1;
}

void ErrorCls()
{
/*
    char *p;
    int i, j;

    p = NULL;
    if( gErrorOutput == ErrorMonoPuts ){
        p = TEXT_MODE_BASE_MONO_ADDR;
    } else if( gErrorOutput == ErrorVgaPuts ){
        p = TEXT_MODE_BASE_VGA_ADDR;
    }
    if( !p ) return;
    for( i = 0; i < 25; i++ ){
	for( j = 0; j < 80; j++ ){
            *p++ = ' ';
            *p++ = 7;
        }
    }
    gErrorCursorYpos = 0;
    gErrorCursorXpos = 0;
*/
}

void ErrorMonoPuts( char *str )
{
    if( (void *)gErrorOutput != (void *)ErrorMonoPuts ) return;
    while( *str ) ErrorMonoPutc( *str++ );        
}

int ErrorFileOut( const char *str )
{
    if( (void *)gErrorOutput == (void *)ErrorFileOut && (!gErrorLogFileHandler || fprintf( gErrorLogFileHandler, "%s", str ) < 0 || fflush( gErrorLogFileHandler ) == -1) )
        return -1;
    else
        return 0;
}

void ErrorVgaPuts( char *str )
{
    if( (void *)gErrorOutput != (void *)ErrorVgaPuts ) return;
    puts( str );
}

void ErrorMonoPutc( int chr )
{
/*
    char *addr;
    switch( chr ){
        case '\a': // BELL
            printf( "\a" );
            return;
        case 8: // BS
            if( gErrorCursorXpos > 0 ){
                addr = TEXT_MODE_BASE_MONO_ADDR + 2 * ( (gErrorCursorXpos - 1) + 80 * gErrorCursorYpos );
                addr[ 0 ] = ' ';
                addr[ 1 ] = 7;
                gErrorCursorXpos--;
            }
            return;
        case 9: // TAB
            do
                ErrorMonoPutc( ' ' );
            while( (gErrorCursorXpos - 1) % 4 );
            return;
        case 10: // LF
    	    break;
        case 13: // CR
            gErrorCursorXpos = 0;
            return;
        default:
            addr = TEXT_MODE_BASE_MONO_ADDR + 2 * ( gErrorCursorXpos + 80 * gErrorCursorYpos );
            addr[ -1 ] = (char)chr;
            addr[ 0 ]  = 7;
            gErrorCursorXpos++;
            if( gErrorCursorXpos > 79 ) break;
            return;
    }
    gErrorCursorXpos = 0;    
    if( ++gErrorCursorYpos > 24 ){
        gErrorCursorYpos = 24;
        ErrorMonoScrollUp();
    }
*/
}

void ErrorMonoScrollUp()
{
/*
    char *addr;
    int i, j;

    addr = TEXT_MODE_BASE_MONO_ADDR;
    for( i = 0; i < 24; i++ ){
        for( j = 0; j < 160; j++, addr++ ){
            addr[ 0 ] = addr[ 160 ];
        }
    }
    for ( i = 0; i < 80; i++ ){
        *addr++ = ' ';
        *addr++ = 7;
    }
*/
}

void ErrorCloseLogFile()
{
    if( gErrorLogFileHandler ) fclose( gErrorLogFileHandler );
}

void _assert_( int ErrCond, const char *name, int line, const char *file )
{
    if( ErrCond ) return;
    printf( "Assertion failed: %s, file %s, line %d\n", name, file, line);
//    gCb_01();
    exit( EXIT_FAILURE );
}

char *last_err = "";

char **GetErrMsgPtr()
{
    return &last_err;
}



