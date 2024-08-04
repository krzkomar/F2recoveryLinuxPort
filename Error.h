#pragma once
#include "FrameWork.h"

//#define eprintf
#define eprintf( fmt, a... )	printf( "{%14s: %4i}\t" fmt "\n",__FILE__,__LINE__, ##a )
//#define ASSERT( cond )		_assert_( cond, ##cond, __LINE__, __FILE_NAME__ )
#define DD	{ static int cnt=0; printf("<!debug %i!> file %s -> function %s() -> line %i\n", cnt++, __FILE_NAME__,__func__, __LINE__ ); }

#define DBG_CRASH	*(char *)0 = 1;
#define DBG_OBJ		((Obj_t *)dbg_ptr)

extern void *dbg_ptr;

int  ErrorCloseLogTaskAtExit();
void ErrorSetOutMGA();
void ErrorSetOutFile( const char *fpath, const char *mode );
void ErrorSetOutVGA();

/*
    Checks value of envirnment variable DEBUGACTIVE
	'mono' - redirects logs to MGA (Hercules)
	'log'  - redirects logs to file 'debug.log'
	'screen' - redirects logs to VGA text mode screen
	'gnw' - redirects logs to in game window system
*/
void ErrorLogSetup();
void ErrorSetOutCb( void (*Cb)(char *str) );
int  ErrorPrintf( const char *fmt, ... );
int  ErrorPuts( char *str );
void ErrorCls();
void ErrorMonoPuts( char *str );
int  ErrorFileOut( const char *str );
void ErrorVgaPuts( char *str );
void ErrorMonoPutc( int chr );
void ErrorMonoScrollUp();
void ErrorCloseLogFile();

char **GetErrMsgPtr();

