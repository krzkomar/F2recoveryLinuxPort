#include "FrameWork.h"

#define GET_ARGi( scr, Argi, Type )	\
    Type = IntpPopwA( scr );	\
    Argi = IntpPopiA( scr );	\
    if( Type == SCR_FSTRING ) IntpStringDeRef( scr, SCR_FSTRING, Argi );

#define GET_ARGf( scr, Argi, Argf, Type )	\
    Type = IntpPopwA( scr );	\
    Argi = IntpPopiA( scr );	\
    if( Type == SCR_FSTRING ) IntpStringDeRef( scr, SCR_FSTRING, Argi );	\
    Argf = *((float *)&Argi);

#define TYPEH( n )	((n) >> 8)
#define ARG_TYPE( Type, TypeEnum, ErrStr )	if( ( Type & 0xF7FF) != TypeEnum ) IntpError( ErrStr );

Sound_t *gScpIfcSounds[ 32 ];

Pal8_t gScpIfcUnk02[ 256 ];

int gScpIfcUnk01;
int gScpIfcUnk07;

Intp_t *gScpIfcUnk08;

int gScpIfcUnk05;
char gScpIfcUnk03[ 100 ];
char gScpIfcUnk04[ 100 ];

/*************************************************/

void ScpIfc_FillWin3x3( Intp_t *scr )
{
    SCP_DBG_VAR;
    short Type;
    int Arg,SrcH,SrcW;
    char *s, *data;

    GET_ARGi( scr, Arg, Type );
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to fillwin3x3" );
    SCP_DBGA( "FILL_WIN3x3( [%x]%x )", Type, Arg );
    s = IntpMseHandler( IntpGetString( scr, TYPEH( Type ), Arg ) );    
    if( !(data = DataFileGetArt( s, &SrcW, &SrcH ) ) ) IntpError("cannot load 3x3 file '%s'", s );
    WinFillRect( scr->i34 );
    WinBlit( data, SrcW, SrcH, WinFill4(), WinGetCurrentWidth(), WinGetCurrentHeight() );
    dbg_free( data );
}

void ScpIfc_MessageFormat( Intp_t *scr ) // 8073
{
    SCP_DBG_VAR;
    unsigned short Type3, Type4, Type1, Type2, Type5, Type6;
    int Arg1, Arg2, Arg3, Arg4, Arg5, Arg6;

    GET_ARGi( scr, Arg1, Type1 );
    GET_ARGi( scr, Arg2, Type2 );
    GET_ARGi( scr, Arg3, Type3 );
    GET_ARGi( scr, Arg4, Type4 );
    GET_ARGi( scr, Arg5, Type5 );
    GET_ARGi( scr, Arg6, Type6 );

    ARG_TYPE( Type1, SCR_INT, "Invalid arg 6 given to format\n" );
    ARG_TYPE( Type2, SCR_INT, "Invalid arg 5 given to format\n" );
    ARG_TYPE( Type3, SCR_INT, "Invalid arg 4 given to format\n" );
    ARG_TYPE( Type4, SCR_INT, "Invalid arg 3 given to format\n" );
    ARG_TYPE( Type5, SCR_INT, "Invalid arg 2 given to format\n" );
    ARG_TYPE( Type6, SCR_STRING, "Invalid arg 1 given to format\n" );
    SCP_DBGA( 
	"message_format( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type6, Arg6, Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );
    if( !WinFormatMsg( IntpGetString( scr, TYPEH( Type6 ), Arg6 ), Arg5, Arg3, Arg4, Arg2, Arg1 ) ) IntpError( "Error formatting message\n" );
}

void ScpIfc_Print( Intp_t *scr ) // 8072
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    WinFillRect( scr->i34 );
    GET_ARGi( scr, Arg, Type );
    SCP_DBGA( "Print( [%x]%x", Type, Arg );
    Type &= ~0x0800;
    switch( Type ){
	case SCR_STRING: IntpLog( "%s", IntpGetString( scr, TYPEH( Type ), Arg ) ); break;
	case SCR_FLOAT: IntpLog( "%.5f", Arg ); break;
	case SCR_INT: IntpLog( "%d", Arg ); break;
    }
}

void ScpIfc_SelectFileList( Intp_t *scr ) // 809F
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2, selected, Cnt;
    char *title, *mask, **list;

    scr->Flags |= 0x20;

    GET_ARGi( scr, Arg1, Type1 ); // string mask
    ARG_TYPE( Type1, SCR_STRING, "Error, invalid arg 2 given to selectfilelist" );
    mask = IntpGetString( scr, TYPEH( Type1 ), Arg1 );

    GET_ARGi( scr, Arg2, Type2 ); // string title
    ARG_TYPE( Type2, SCR_STRING, "Error, invalid arg 1 given to selectfilelist" );
    title = IntpGetString( scr, TYPEH( Type2 ), Arg2 );
    list = FileListMake( IntpMseHandler( mask ), &Cnt );
    SCP_DBGA( "selectfilelist( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    if( list && Cnt ){
        selected = TextBoxFileSelect( title, list, Cnt, NULL, 320 - gFont.LineWidth( title ) / 2 - 10, 200, gPalColorCubeRGB[31][31][31] | 0x10000 );
        if( selected == -1 ){
            IntpPushiA( scr, 0 );
            IntpPushwA( scr, SCR_INT );
        } else {
            IntpPushiA( scr, IntpAddString( scr, list[ selected ] ) );
            IntpPushwA( scr, SCR_FSTRING );
        }        
        FileListFree( list );
    } else {
        IntpPushiA( scr, 0 );
        IntpPushwA( scr, SCR_INT );
    }
    scr->Flags &= ~0x20;
}

void ScpIfc_TokenizeString( Intp_t *scr ) // 80A0
{
    SCP_DBG_VAR;
    short Type1, Type2, Type3;
    int v25, Arg1, Arg2, Arg3, v29, i;
    char *s, *v13, *v14, *p, *v17, *errcall, *xx;
    
    xx = 0;
    v25 = 0;
    errcall = NULL;

    GET_ARGi( scr, Arg1, Type1 );
    ARG_TYPE( Type1, SCR_INT, "Error, invalid arg 3 to tokenize." );

    v29 = Arg1;
    GET_ARGi( scr, Arg2, Type2 );

    switch( Type2 & ~0x800 ){
	case SCR_INT: if( Arg2 ) IntpError( "Error, invalid arg 2 to tokenize. (only accept 0 for int value)" ); break;
	case SCR_STRING: xx = IntpGetString( scr, TYPEH( Type2 ), Arg2 ); break;
	default: IntpError( "Error, invalid arg 2 to tokenize. (string)" ); break;
    }

    GET_ARGi( scr, Arg3, Type3 );
    ARG_TYPE( Type3, SCR_STRING, "Error, invalid arg 1 to tokenize." );

    SCP_DBGA( "tokenize( [%x]%x, [%x]%x, [%x]%x )", Type3, Arg3, Type2, Arg2, Type1, Arg1 );
    s = IntpGetString( scr, TYPEH( Type3 ), Arg3 );
    if( !xx ){
        for( i = 0;*s != v29 && *s; s++, i++ );
        if( s ){
            p = calloc(1, (i + 1) );
            strncpy( p, s, i );
            IntpPushiA( scr, IntpAddString( scr, p ) );
	    IntpPushwA( scr, SCR_FSTRING );
	    if( p ) dbg_free( p );
	    return;
        }
    } else {
	if( (v13 = strstr( s, xx )) ){	
    	    for( v14 = v13 + strlen( xx ); *v14 != v29 && *v14; v14++ );
	    if( *v14 == v29 ){	    
    		for( v17 = v14 + 1; *v17 != v29 && *v17; v17++, v25++ );
		p = calloc( 1, v25 + 1 );
		strncpy( p, v14 + 1, v25 );
		IntpPushiA( scr, IntpAddString( scr, p ) );
		IntpPushwA( scr, SCR_FSTRING );
		if( errcall ) dbg_free( p );
		return;    
	    }
	}
    }
    IntpPushiA( scr, 0 );
    IntpPushwA( scr, SCR_INT );
    if( errcall ) dbg_free( errcall );
    return;
}

void ScpIfc_PrintRect( Intp_t *scr ) // 8074
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3;
    int Arg1, Arg2, Arg3;
    float Arg3f;
    char *s, stmp[80];

    WinFillRect( scr->i34 );
    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGf( scr, Arg3, Arg3f, Type3 )
    SCP_DBGA( "printrect( [%x]%x, [%x]%x, [%x]%x )", Type3, Arg3, Type2, Arg2, Type1, Arg1 );
    if( (Type1 & 0xF7FF) != SCR_INT || Arg1 > 2 ) IntpError( "Invalid arg 3 given to printrect, expecting int" );
    ARG_TYPE( Type2, SCR_INT, "Invalid arg 2 given to printrect, expecting int" );
    if( (Type3 & 0xF7FF) >= SCR_FLOAT ){
        if( (Type3 & 0xF7FF) > SCR_FLOAT ){
            if( (Type3 & 0xF7FF) == SCR_INT ) sprintf( stmp, "%d", Arg3 );
        } else {
    	    s = stmp;
    	    sprintf( stmp, "%.5f", Arg3f );
        }
    } else {
	if( Type2 == SCR_STRING ) s = IntpGetString( scr, TYPEH( Type2 ), Arg2 );
    }
    if( !WinPrintRect( s, Arg2, Arg1 ) ) IntpError( "Error in printrect" );
}

void ScpIfc_Select( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg, win;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to select" );
    SCP_DBGA( "select( [%x]%x )", Type, Arg );
    win = WinSelect( IntpGetString(scr, TYPEH( Type ), Arg ) );
    if( win == -1 ) IntpError("Error selecing window %s\n", IntpGetString( scr, TYPEH( Type ), Arg ) );
    scr->i34 = win;
    IntpSetErrHandler( WinError );
}

void ScpIfc_Display( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to display" );
    SCP_DBGA( "display( [%x]%x )", Type, Arg );
    WinFillRect( scr->i34 );
    WinUnk10( IntpMseHandler( IntpGetString(scr, TYPEH(Type), Arg ) ) );
}

void ScpIfc_DisplayRaw( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to displayraw" );    
    SCP_DBGA( "displayraw( [%x]%x )", Type, Arg );
    WinFillRect( scr->i34 );    
    WinUnk13( IntpMseHandler( IntpGetString( scr, TYPEH( Type ), Arg ) ) );
}

void ScpIfcPal( Pal8_t *Pal, Pal8_t *PalStop, float fsteps , int Update )
{
    int v3,j,i,a1,v15,steps;
    Pal8_t tmpal[ 256 ];

    a1 = TimerGetSysTime();
    v3 = a1;    
    steps = (int)lround( fsteps );
    v15 = 0;
    if( (Update & 0x7FFFFFFF) ){
        for( j = 0; j < steps; ){
            if( v15 ){
                for( i = 0; i < 256; i++ ){
                    tmpal[ i ].r = Pal[ i ].r - j * (Pal[ i ].r - PalStop[ i ].r) / steps;
                    tmpal[ i ].g = Pal[ i ].g - j * (Pal[ i ].g - PalStop[ i ].g) / steps;
                    tmpal[ i ].b = Pal[ i ].b - j * (Pal[ i ].b - PalStop[ i ].b) / steps;
                }
                PalSetColors( tmpal );
                v3 = a1;
                j += v15;
            }
            if( Update ) InpWinUpdate();
            a1 = TimerGetSysTime();
            v15 = a1 - v3;
        }
    }
    PalSetColors( PalStop );
}

void ScpIfcPalA( Pal8_t *a1, Pal8_t *a2, float a3 )
{
    ScpIfcPal( a1, a2, a3, 1 );
}

int ScpIfcUnk01()
{
    return gScpIfcUnk01;
}

void ScpIfcUnk02( float a1 )
{
    int c;

    c = MseIsCursorClear();
    MseCursorRedraw();
    ScpIfcPal( PalGetCurrent(), gScpIfcUnk02, a1, 1);
    if( !c ) MseDrawCursor();
}

void ScpIfcUnk03( float a1 )
{
    ScpIfcPal( gScpIfcUnk02, gPalBase, a1, 1 );
}

void ScpIfcUnk04( float a1 )
{
    int c;

    c = MseIsCursorClear();
    MseCursorRedraw();
    ScpIfcPal( PalGetCurrent(), gScpIfcUnk02, a1, 0 );
    if( !c ) MseDrawCursor();
}

void ScpIfcUnk05( float a1 )
{
    ScpIfcPal( gScpIfcUnk02, gPalBase, a1, 0 );
}

void ScpIfc_FadeIn( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;
    float Argf;

    GET_ARGf( scr, Arg, Argf, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid type given to fadein\n" );
    SCP_DBGA( "fadein( [%x]%x )", Type, Arg );
    scr->Flags |= 0x20;
    PalSetColors( gScpIfcUnk02 );
    ScpIfcPal( gScpIfcUnk02, gPalBase, Argf, 1 );
    gScpIfcUnk01 = 1;
    scr->Flags &= ~0x20;
}

void ScpIfc_FadeOut( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg, cc;
    float Argf;

    GET_ARGf( scr, Arg, Argf, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid type given to fadeout\n" );
    SCP_DBGA( "fadeout( [%x]%x )", Type, Arg );
    scr->Flags |= 0x20;
    cc = MseIsCursorClear();
    MseCursorRedraw();
    ScpIfcPal( PalGetCurrent(), gScpIfcUnk02, Argf, 1 );
    if( !cc ) MseDrawCursor();
    gScpIfcUnk01 = 0;
    scr->Flags = scr->Flags & ~0x20;
}

int ScpIfcUnk06()
{
    if( DialogGetSayModeLvl() <= 0 ) return WinPlay();
    return 1;
}

void ScpIfc_SettingMovieFlag( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    SCP_DBGA( "movie_flags( [%x]%x )", Type, Arg );
    if( !WinSetMovieFlags( Arg ) ) IntpError( "Error setting movie flags\n" );
}

void ScpIfc_PlayMovie1( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid arg given to playmovie" );
    SCP_DBGA( "playmovie1( [%x]%x )", Type, Arg );
    strcpy( gScpIfcUnk03, IntpGetString( scr, TYPEH( Type ), Arg ) );
    if( !strchr( gScpIfcUnk03, '.' ) ) strcpy( gScpIfcUnk03 + strlen( gScpIfcUnk03 ), ".mve" );
    WinFillRect( scr->i34 );
    scr->Flags |= 0x10;
    scr->Func = ScpIfcUnk06;
    if( !WinPlayStart( IntpMseHandler( gScpIfcUnk03 ) ) ) IntpError( "Error playing movie" );
}

void ScpIfc_PlayMovie( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )
    ARG_TYPE( Type5, SCR_STRING, "Invalid arg given to playmovie" );
    SCP_DBGA( 
	"playmovie( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );
    strcpy( gScpIfcUnk04, IntpGetString( scr, TYPEH( Type5 ), Arg5 ) );
    if( !strchr( gScpIfcUnk04, '.') ) strcpy( gScpIfcUnk04 + strlen( gScpIfcUnk04 ), ".mve" );
    WinFillRect( scr->i34 );
    scr->Func = ScpIfcUnk06;
    scr->Flags |= 0x10;    
    if( !WinPlayAt( IntpMseHandler( gScpIfcUnk04 ), Arg4, Arg3, Arg2, Arg1 ) ) IntpError( "Error playing movie" );
}

void ScpIfc_MovieStat( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "movie_start()" );
    WinPlayAbort();
    scr->Flags |= 0x40;
}

void ScpIfc_DeleteRegion( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;
    char *s;

    GET_ARGi( scr, Arg, Type )
    SCP_DBGA( "deleteregion( [%x]%x )", Arg, Type );
    if( (Type & 0xF7FF) == SCR_INT && Arg != -1 ) IntpError( "Invalid type given to deleteregion" );
    WinFillRect( scr->i34 );
    if( Arg == -1 )
        s = NULL;
    else
        s = IntpGetString( scr, TYPEH( Type ), Arg );
    WinDeleteRegion( s );
}

void ScpIfc_ActivateRegion( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    SCP_DBGA( "activate_region( [%x]%x, [%x]%x )", Arg2, Type2, Arg1, Type1 );
    WinActivateRegion( IntpGetString( scr, TYPEH( Type2 ), Arg2 ), Arg1 );
}

void ScpIfc_CheckRegion( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid arg 1 given to checkregion();\n" );
    SCP_DBGA( "checkregion( [%x]%x )", Arg, Type );
    IntpPushiA( scr, WinCheckRegion( IntpGetString( scr, TYPEH( Type ), Arg ) ) );
    IntpPushwA( scr, SCR_INT );
}

void ScpIfc_SetRegion( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type, t1, t2;
    int Arg, a1, a2;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid number of elements given to region" );
    if( Arg < 2 ) IntpError( "addregion call without enough points!" );
    SCP_DBGA( "setregion( [%x]%x, ... )", Arg, Type );
    WinFillRect( scr->i34 );
    WinUnk106( Arg / 2 );
    for( ;Arg >= 2; Arg -= 2 ){
	GET_ARGi( scr, a1, t1 )
        ARG_TYPE( t1, SCR_INT, "Invalid y value given to region" );
	GET_ARGi( scr, a2, t2 )
        ARG_TYPE( t2, SCR_INT, "Invalid x value given to region" );
        WinUnk193((a2 * WinGetScreenWidth() + 639) / 640, (a1 * WinGetScreenHeight() + 479) / 480, 1);
    }
    if( Arg ){
        t1 = IntpPopwA( scr );
        a1 = IntpPopiA( scr );
        if( t1 == SCR_FSTRING ) IntpStringDeRef( scr, SCR_FSTRING, a1 );
        if( (t1 & 0xF7FF) != SCR_STRING && t1 == SCR_INT ){
            if( a1 ) IntpError( "Invalid name given to region" );
        }
        WinSetRegionA( IntpGetString( scr, TYPEH( t1 ), a1 ) );
        WinSetRegion();
    } else {
        IntpError( "Unnamed regions not allowed\n" );
        WinSetRegion();
    }
}

void ScpIfc_SetRegionProc( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;
    char *name;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )
    ARG_TYPE( Type1, SCR_INT, "Invalid procedure 4 name given to addregionproc" );
    ARG_TYPE( Type2, SCR_INT, "Invalid procedure 3 name given to addregionproc" );
    ARG_TYPE( Type3, SCR_INT, "Invalid procedure 2 name given to addregionproc" );
    ARG_TYPE( Type4, SCR_INT, "Invalid procedure 1 name given to addregionproc" );
    ARG_TYPE( Type5, SCR_STRING, "Invalid name given to addregionproc" );
    SCP_DBGA( 
	"addregionproc( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );
    name = IntpGetString( scr, TYPEH( Type5 ), Arg5 );
    WinFillRect( scr->i34 );
    if( !WinRegionSetProcedure( name, scr, Arg4, Arg3, Arg2, Arg1 ) ) IntpError( "Error setting procedures to region %s\n", name );
}

void ScpIfc_SetRightButtonProc( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3;
    int Arg1, Arg2, Arg3;
    char *name;

    GET_ARGi( scr, Arg1, Type1 ) // 
    GET_ARGi( scr, Arg2, Type2 ) // 
    GET_ARGi( scr, Arg3, Type3 ) // 

    ARG_TYPE( Type1, SCR_INT, "Invalid procedure 2 name given to addregionrightproc" );
    ARG_TYPE( Type2, SCR_INT, "Invalid procedure 1 name given to addregionrightproc" );
    ARG_TYPE( Type3, SCR_STRING, "Invalid name given to addregionrightproc" );
    SCP_DBGA( 
	"addregionrightproc( [%x]%x, [%x]%x, [%x]%x )", 
	Type3, Arg3, Type2, Arg2, Type1, Arg1
    );
    WinFillRect( scr->i34 );
    name = IntpGetString( scr, TYPEH( Type3 ), Arg3 );
    if( !WinRightButtonSetProcedure( name, scr, Arg2, Arg1 ) ) IntpError("Error setting right button procedures to region %s\n", name );
}

void ScpIfc_CreateWindow( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;

    GET_ARGi( scr, Arg1, Type1 ) // int h
    GET_ARGi( scr, Arg2, Type2 ) // int w
    GET_ARGi( scr, Arg3, Type3 ) // int y
    GET_ARGi( scr, Arg4, Type4 ) // int x
    GET_ARGi( scr, Arg5, Type5 ) // name
    SCP_DBGA( 
	"create_window( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );

    if( WinCreate( 
	IntpGetString( scr, TYPEH( Type5 ), Arg5 ), 
	(Arg4 * WinGetScreenWidth() + 639) / 640, 
	(Arg3 * WinGetScreenHeight() + 479) / 480, 
	(Arg2 * WinGetScreenWidth() + 639) / 640, 
	(Arg1 * WinGetScreenHeight() + 479) / 480,
	gPalColorCubeRGB[0][0][0], 0 ) == -1 
    ) IntpError("Couldn't create window.");
}

void ScpIfc_ResizeWindow( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;

    GET_ARGi( scr, Arg1, Type1 ) // int h
    GET_ARGi( scr, Arg2, Type2 ) // int w
    GET_ARGi( scr, Arg3, Type3 ) // int y
    GET_ARGi( scr, Arg4, Type4 ) // int x
    GET_ARGi( scr, Arg5, Type5 ) // name
    SCP_DBGA( 
	"resize_window( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );

    if( WinRes( 
	IntpGetString( scr, TYPEH( Type5 ), Arg5 ), 
	(Arg4 * WinGetScreenWidth() + 639) / 640, 
	(Arg3 * WinGetScreenHeight() + 479) / 480, 
	(Arg2 * WinGetScreenWidth() + 639) / 640, 
	(Arg1 * WinGetScreenHeight() + 479) / 480 ) == -1 
    ) IntpError("Couldn't resize window.");
}

void ScpIfc_ScaleWindow( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;

    GET_ARGi( scr, Arg1, Type1 ) // int h
    GET_ARGi( scr, Arg2, Type2 ) // int w
    GET_ARGi( scr, Arg3, Type3 ) // int y
    GET_ARGi( scr, Arg4, Type4 ) // int x
    GET_ARGi( scr, Arg5, Type5 ) // name
    SCP_DBGA( 
	"scale_window( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );
    if( WinScale( 
	IntpGetString( scr, TYPEH( Type5 ), Arg5 ), 
	(Arg4 * WinGetScreenWidth() + 639) / 640, 
	(Arg3 * WinGetScreenHeight() + 479) / 480, 
	(Arg2 * WinGetScreenWidth() + 639) / 640, 
	(Arg1 * WinGetScreenHeight() + 479) / 480) == -1 
    ) IntpError( "Couldn't scale window." );
}

void ScpIfc_DeleteWindow( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;
    char *s;

    GET_ARGi( scr, Arg, Type )
    SCP_DBGA( "delete_window( [%x]%x )", Type, Arg );

    s = IntpGetString( scr, Type, Arg );
    if( !WinDelete( s ) ) IntpError( "Error deleting window %s\n", s );
    scr->i34 = WinRemove();
}

void ScpIfc_SayStart( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "say_start" );
    scr->Flags |= 0x20;
    gScpIfcUnk05 = 0;
    if( DialogStart( scr ) ){
        scr->Flags &= ~0x20;
        IntpError("Error starting dialog.");
    }
    scr->Flags &= ~0x20;
}

void ScpIfc_StartDialog( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    SCP_DBGA( "start_dialog( [%x]%x )", Type, Arg );
    scr->Flags |= 0x20;
    gScpIfcUnk05 = Arg;
    if( DialogStart( scr ) ){
        scr->Flags &= ~0x20;
        IntpError( "Error starting dialog." );
    }
    scr->Flags &= ~0x20;
}

void ScpIfc_SetTitle( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;
    char *x;

    x = NULL;
    GET_ARGi( scr, Arg, Type )
    SCP_DBGA( "set_title( [%x]%x )", Type, Arg );
    if( (Type & 0xF7FF) == SCR_STRING ) x = IntpGetString( scr, TYPEH( Type ), Arg );
    if( DialogSetTitle( x ) ) IntpError( "Error setting title." );
}

void ScpIfc_SayGotoReply( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;
    char *x;

    x = NULL;
    GET_ARGi( scr, Arg, Type )
    SCP_DBGA( "say_goto_reply( [%x]%x )", Type, Arg );
    if( (Type & 0xF7FF) == SCR_STRING ) x = IntpGetString( scr, TYPEH( Type ), Arg );
    if( DialogGotoReply( x ) ) IntpError( "Error during goto, couldn't find reply target %s\n", x );
}

void ScpIfc_SayOption( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *x, *k;
    
    scr->Flags |= 0x20;
    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    SCP_DBGA( "say_option( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    if( (Type2 & ~0x800) == SCR_STRING )
        x = IntpGetString( scr, TYPEH( Type2 ), Arg2 );
    else
        x = 0;

    if( (Type1 & ~0x800) == SCR_STRING ){
        k = IntpGetString( scr, TYPEH( Type1 ), Arg1 );
        if( DialogSetOptionStr( x, k ) ){
            scr->Flags &= ~0x20;
            IntpError( "Error setting option." );
        }
    } else {
        if( (Type1 & ~0x800) != SCR_INT ){
            IntpError( "Invalid arg 2 to sayOption" );
        } else {
    	    if( DialogSetOptionInt( x, Arg1 ) ){
        	scr->Flags &= ~0x20;
        	IntpError( "Error setting option." );
    	    }
        }
    }
    scr->Flags &= ~0x20;
}

void ScpIfc_SayReply( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *s, *k;

    scr->Flags |= 0x20;
    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    SCP_DBGA( "say_reply( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    if( (Type2 & ~0x800) == SCR_STRING )
        s = IntpGetString(scr, TYPEH(Type2), Arg2);
    else
        s = 0;
        
    if( (Type1 & ~0x800) == SCR_STRING )
        k = IntpGetString(scr, TYPEH(Type1), Arg1);
    else
        k = 0;
    if( DialogReply( s, k ) ){
        scr->Flags &= ~0x20;
        IntpError( "Error setting option." );
    }
    scr->Flags &= ~0x20;
}

int ScpIfcUnk08( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "???" );
    scr->Flags |= 0x40;
    return DialogGetSayModeLvl() != -1;
}

void ScpIfc_SayEnd( Intp_t *scr )
{
    SCP_DBG_VAR;
    int err;

    SCP_DBGA( "say_end()" );
    scr->Flags |= 0x20;
    err = DialogEnd( gScpIfcUnk05 );
    scr->Flags = scr->Flags & ~0x20;
    if( err == -2 ){
        scr->Func = ScpIfcUnk08;
        scr->Flags |= 0x10;
    }
}

void ScpIfc_unk09( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "???" );
    IntpPushiA( scr, DialogUnk32() );
    IntpPushwA( scr, SCR_INT );
}

void ScpIfc_SayQuit( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "say_quit()" );
    if( DialogQuit() ) IntpError( "Error quitting option." );
}

int ScpIfcGetTimeOut()
{
    return gIfcTimeout;
}

int ScpIfcSetTimeOut( int time )
{
    gIfcTimeout = time;
    return time;
}

void ScpIfc_SayMsgTimeout( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    SCP_DBGA( "sayMsgTimeout( [%x]%x )", Type, Arg );
    ARG_TYPE( Type, 0x4000, "sayMsgTimeout:  invalid var type passed." );
    gIfcTimeout	= Arg;
}

void ScpIfc_SayMessage( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *title, *msg;

    scr->Flags |= 0x20;
    GET_ARGi( scr, Arg1, Type1 ) // message_text    
    GET_ARGi( scr, Arg2, Type2 ) // message title
    SCP_DBGA( "sayMessage( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    if( (Type2 & 0xF7FF) == SCR_STRING )
        title = IntpGetString( scr, TYPEH(Type2), Arg2 );
    else
        title = NULL;

    if( (Type1 & 0xF7FF) == SCR_STRING )
        msg = IntpGetString( scr, TYPEH(Type1), Arg1 );
    else
        msg = NULL;

    if( DialogMsg( title, msg, gIfcTimeout ) ){
        scr->Flags &= ~0x20;
        IntpError( "Error setting option." );
    }
    scr->Flags &= ~0x20;
}

void ScpIfc_GotoXY( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Type1, SCR_INT, "Invalid operand given to gotoxy" );
    ARG_TYPE( Type2, SCR_INT, "Invalid operand given to gotoxy" );
    SCP_DBGA( "gotoXY( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    WinFillRect( scr->i34 );
    WinMoveXY( Arg2, Arg1 );
}

void ScpIfc_AddButtonFlag( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )    
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 2 given to addbuttonflag" );
    ARG_TYPE( Type2, SCR_STRING, "Invalid arg 1 given to addbuttonflag" );
    SCP_DBGA( "AddButtonFlag( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    if( !WinAddButtonFlag( IntpGetString( scr, TYPEH( Type2 ), Arg2 ), Arg1 ) ) IntpError( "Error setting flag on button %s", IntpGetString( scr, TYPEH( Type2 ), Arg2 ) );
}

void ScpIfc_AddRegionFlag( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    
    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )        
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 2 given to addregionflag" );
    ARG_TYPE( Type2, SCR_STRING, "Invalid arg 1 given to addregionflag" );
    SCP_DBGA( "AddRegionFlag( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    if( !WinAddRegionFlag( IntpGetString( scr, TYPEH( Type2 ), Arg2), Arg1 ) ) IntpError("Error setting flag on region %s", IntpGetString( scr, TYPEH( Type2 ), Arg2 ) );
}

void ScpIfc_AddButton( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;

    GET_ARGi( scr, Arg1, Type1 )    
    ARG_TYPE( Type1, SCR_INT, "Invalid height given to addbutton" );
    GET_ARGi( scr, Arg2, Type2 )    
    ARG_TYPE( Type2, SCR_INT, "Invalid width given to addbutton" );
    GET_ARGi( scr, Arg3, Type3 )    
    ARG_TYPE( Type3, SCR_INT, "Invalid y given to addbutton" );
    GET_ARGi( scr, Arg4, Type4 )    
    ARG_TYPE( Type4, SCR_INT, "Invalid x given to addbutton" );
    GET_ARGi( scr, Arg5, Type5 )    
    ARG_TYPE( Type5, SCR_STRING, "Invalid name given to addbutton" );
    SCP_DBGA( 
	"addbutton( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );
    WinFillRect( scr->i34 );
    Arg1 = (Arg1 * WinGetScreenHeight() + 479) / 480;
    Arg2 = (Arg2 * WinGetScreenWidth() + 639) / 640;
    Arg3 = (Arg3 * WinGetScreenHeight() + 479) / 480;
    Arg4 = (Arg4 * WinGetScreenWidth() + 639) / 640;
    WinNewButton( IntpGetString( scr, Type5, Arg5 ), Arg4, Arg3, Arg2, Arg1, 0 );
}

void ScpIfc_AddButtonText( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *name;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Arg1, SCR_STRING, "Invalid text string given to addbuttontext" );
    ARG_TYPE( Arg2, SCR_STRING, "Invalid name given to addbuttontext" );
    SCP_DBGA( "addbuttontext( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    WinFillRect( scr->i34 );
    name = IntpGetString( scr, Type1, Arg2 );
    if( !WinAddButtonText( name, IntpGetString( scr, Type2, Arg1 ) ) ) IntpError( "Error setting text to button %s\n", name );
}

void ScpIfc_AddButtonGFX( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3, Type4;
    int Arg1, Arg2, Arg3, Arg4;
    int r;
    char *name;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    SCP_DBGA( "addbuttongfx( [%x]%x, [%x]%x, [%x]%x, [%x]%x )", Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1 );

    if( (Type3 & 0xF7FF) == SCR_STRING ){ r = 0; goto nn; }
    if( (Type3 & 0xF7FF) != SCR_INT ){ r = 1; goto nn; }
    r = ( !Arg3 ) ? 0 : 1;
nn:
    if( r ) goto vv;
    if( (Type2 & 0xF7FF) == SCR_STRING ){ r = 0; goto kk; }
    if( (Type2 & 0xF7FF) != SCR_INT ){ r = 1; goto kk; }
    r = ( !Arg2 ) ? 0 : 1;
kk:
    if( r ) goto vv;
    if( (Type1 & 0xF7FF) == SCR_STRING ){ r = 0; goto zz; }
    if( (Type1 & 0xF7FF) != SCR_INT ){ r = 1; goto zz; }
    r = ( !Arg1 ) ? 0 : 1;
zz:
    if( r ){
vv:
        IntpError( "Invalid filename given to addbuttongfx" );
    }
    ARG_TYPE( Type4, SCR_STRING, "Invalid name given to addbuttontext" );
    name = IntpGetString( scr, TYPEH( Type4 ), Arg4 );    
    WinFillRect( scr->i34 );
    if( !WinAddButtonGFX( 
	    name, 
	    IntpMseHandler( IntpGetString( scr, TYPEH( Type3 ), Arg3 ) ), 
	    IntpMseHandler( IntpGetString( scr, TYPEH( Type2 ), Arg2 ) ), 
	    IntpMseHandler( IntpGetString( scr, TYPEH( Type1 ), Arg1 ) )
	) 
    ) IntpError( "Error setting graphics to button %s\n", name );
}

void ScpIfc_AddButtonProc( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;
    char *name;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )
    SCP_DBGA( 
	"addbuttonproc( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );
    ARG_TYPE( Type1, SCR_INT, "Invalid procedure 4 name given to addbuttonproc" );
    ARG_TYPE( Type2, SCR_INT, "Invalid procedure 3 name given to addbuttonproc" );
    ARG_TYPE( Type3, SCR_INT, "Invalid procedure 2 name given to addbuttonproc" );
    ARG_TYPE( Type4, SCR_INT, "Invalid procedure 1 name given to addbuttonproc" );
    ARG_TYPE( Type5, SCR_STRING, "Invalid name given to addbuttonproc" );
    WinFillRect( scr->i34 );
    name = IntpGetString( scr, TYPEH( Type5 ), Arg5 );
    if( !WinAddButtonProc( name, scr, Arg4, Arg3, Arg2, Arg1) ) IntpError( "Error setting procedures to button %s\n", name );
}

void ScpIfc_AddButtonRightProc( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;
    char *name;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    SCP_DBGA( "addbuttonproc( [%x]%x, [%x]%x, [%x]%x )", Type3, Arg3, Type2, Arg2, Type1, Arg1 );
    ARG_TYPE( Type1, SCR_INT, "Invalid procedure 2 name given to addbuttonrightproc" );
    ARG_TYPE( Type2, SCR_INT, "Invalid procedure 1 name given to addbuttonrightproc" );
    ARG_TYPE( Type3, SCR_STRING, "Invalid name given to addbuttonrightproc" );
    WinFillRect( scr->i34 );
    name = IntpGetString( scr, TYPEH( Type3 ), Arg3 );
    if( !WinAddButtonRightProc( name, scr, Arg2, Arg1 ) ) IntpError( "Error setting right button procedures to button %s\n", name );
}

void ScpIfc_ShowWin( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "show_win()" );
    WinFillRect( scr->i34 );
    if( gWinList[ gWinCurrent ].WinId != -1 ) WinUpdate( gWinList[ gWinCurrent ].WinId );
}

void ScpIfc_DeleteButton( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1;
    int Arg1;
    char *n;

    GET_ARGi( scr, Arg1, Type1 )
    if( (Type1 & 0xF7FF) == SCR_INT && Arg1 != -1 ) IntpError( "Invalid type given to delete button" );
    SCP_DBGA( "deletebutton( [%x]%x )", Arg1, Type1 );
    WinFillRect( scr->i34 );
    if( (Type1 & ~0x800) == SCR_INT ) {
        if( WinDeleteButton( 0 ) ) return;
    } else {
        n = IntpGetString( scr, TYPEH( Type1 ), Arg1 );
        if( WinDeleteButton( n ) ) return;
    }
    IntpError( "Error deleting button" );
}

void ScpIfc_FillWin( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3;
    float Bcolor, Gcolor, Rcolor;

    GET_ARGi( scr, Bcolor, Type1 )
    GET_ARGi( scr, Gcolor, Type2 )        
    GET_ARGi( scr, Rcolor, Type3 )    
    SCP_DBGA( "fillwin( [%x]%f, [%x]%f, [%x]%f )", Type3, Rcolor, Type2, Gcolor, Type1, Bcolor );
    if( (Type3 & 0xF7FF) != SCR_FLOAT ){
        if( (Type3 & 0xF7FF) == SCR_INT ){
            if( Rcolor == 1 ){
                Rcolor = 1.0;
            } else if( Rcolor ){
                IntpError( "Invalid red value given to fillwin" );
            }
        }
    }
    if( (Type2 & 0xF7FF) != SCR_FLOAT && (Type2 & 0xF7FF) == SCR_INT ){
        if( Gcolor == 1 ){
            Gcolor = 1.0;
        } else if( Gcolor ){
            IntpError( "Invalid green value given to fillwin" );
        }
    }
    if( (Type1 & 0xF7FF) != SCR_FLOAT && (Type1 & 0xF7FF) == SCR_INT ){
        if( Bcolor == 1 ){
            Bcolor = 1.0;
        } else if( Bcolor ){
            IntpError( "Invalid blue value given to fillwin" );
        }
    }
    WinFillRect( scr->i34 );
    WinFill( Rcolor, Gcolor, Bcolor );
}

void ScpIfc_FillRect( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3,Type4,Type5,Type6,Type7;
    int Xpos, Width, Height, Ypos, tmp;
    float Gcolor, Rcolor, Bcolor;

    GET_ARGf( scr, tmp, Bcolor, Type1 )    
    GET_ARGf( scr, tmp, Gcolor, Type2 )    
    GET_ARGf( scr, tmp, Rcolor, Type3 )    
    GET_ARGi( scr, Height, Type4 )    
    GET_ARGi( scr, Width,  Type5 )    
    GET_ARGi( scr, Ypos,   Type6 )    
    GET_ARGi( scr, Xpos,   Type7 )    
    SCP_DBGA( 
	"fillrect( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%f, [%x]%f, [%x]%f )", 
	Type7, Xpos, Type6, Ypos, Type5, Width, Type4, Height, Type3, Rcolor, Type2, Gcolor, Type1, Bcolor
    );

    if( (Type3 & 0xF7FF) != SCR_FLOAT ){
        if( (Type3 & 0xF7FF) == SCR_INT ){
            if ( Rcolor == 1 ){
                Rcolor = 1.0;
            } else if ( Rcolor ){
                IntpError("Invalid red value given to fillrect");
            }
        }
    }
    if( (Type2 & 0xF7FF) != SCR_FLOAT ){
        if( (Type2 & 0xF7FF) == SCR_INT ){
            if ( Gcolor == 1 ){
                Gcolor = 1.0;
            } else if( Gcolor ){
                IntpError( "Invalid green value given to fillrect" );
            }
        }
    }
    if( (Type1 & 0xF7FF) != SCR_FLOAT ){
        if( (Type1 & 0xF7FF) == SCR_INT ){
            if ( Bcolor == 1 ) {
                Bcolor = 1.0;
            } else if( Bcolor ) {
                IntpError( "Invalid blue value given to fillrect" );
            }
        }
    }
    ARG_TYPE( Type7, SCR_INT, "Invalid arg 1 given to fillrect" );
    ARG_TYPE( Type6, SCR_INT, "Invalid arg 2 given to fillrect" );
    ARG_TYPE( Type5, SCR_INT, "Invalid arg 3 given to fillrect" );
    ARG_TYPE( Type4, SCR_INT, "Invalid arg 4 given to fillrect" );
    WinFillRect( scr->i34 );
    WinFillArea( Xpos, Ypos, Width, Height, Rcolor, Gcolor, Bcolor );
}

void ScpIfc_MouseRedraw( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "MouseRedraw()" );
    MseCursorRedraw();
}

void ScpIfc_MouseDrawCursor()
{
    MseDrawCursor();
}

void ScpIfc_MouseShape( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )    
    GET_ARGi( scr, Arg3, Type3 )    
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 3 given to mouseshape" );
    ARG_TYPE( Type2, SCR_INT, "Invalid arg 2 given to mouseshape" );
    ARG_TYPE( Type3, SCR_STRING, "Invalid filename given to mouseshape" );
    SCP_DBGA( "MouseShape( [%x]%x, [%x]%x, [%x]%x )", Type3, Arg3, Type2, Arg2, Type1, Arg1 );
    if( !MouseMgrLoadMou( IntpGetString( scr, TYPEH( Type3 ), Arg3 ), Arg2, Arg1 ) ) IntpError( "Error loading mouse shape." );
}

void ScpIfc_SetGlobalMouseFunc( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "setglobalmousefunc()" );
    IntpError( "setglobalmousefunc not defined" );
}

void ScpIfc_DisplayGFX( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3,Type4,Type5;
    int Arg1,Arg2,Arg3,Arg4,Arg5;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )    
    GET_ARGi( scr, Arg3, Type3 )    
    GET_ARGi( scr, Arg4, Type4 )    
    GET_ARGi( scr, Arg5, Type5 )        
    SCP_DBGA( 
	"DisplayGFX( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );

    WinDisplayGFX( IntpMseHandler( IntpGetString( scr, Type5, Arg5 ) ), Arg4, Arg3, Arg2, Arg1 );
}

void ScpIfc_LoadPaletteTable( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to loadpalettetable" );
    SCP_DBGA( "loadpalettetable( [%x]%x )", Arg, Type );
    if( !PalLoadFromFile( IntpGetString( scr, TYPEH( Type ), Arg ) ) ) IntpError( PalGetErrorMsg() );    
}

void ScpIfc_AddNamedEvent( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Type1, SCR_STRING, "Invalid type given to addnamedevent" );
    SCP_DBGA( "addnamedevent( [%x]%x, [%x]%x )", Arg2, Type2, Arg1, Type1 );
    NevsAddProg( IntpGetString( scr, TYPEH( Type1 ), Arg2 ), scr, Arg1, 0 );
}

void ScpIfc_AddNamedHandler( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Type1, SCR_STRING, "Invalid type given to addnamedhandler" );
    SCP_DBGA( "addnamedhandler( [%x]%x, [%x]%x )", Arg2, Type2, Arg1, Type1 );
    NevsAddProg( IntpGetString( scr, TYPEH( Type1 ), Arg2 ), scr, Arg1, 1 );
}

void ScpIfc_ClearNamed( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to clearnamed" );
    SCP_DBGA( "clearnamed( [%x]%x )", Arg, Type );
    NevsClearEvents( IntpGetString(scr, TYPEH( Type ), Arg ) );
}

void ScpIfc_SignalNamed( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to signalnamed" );
    SCP_DBGA( "signalnamed( [%x]%x )", Arg, Type );
    NevsSignal( IntpGetString( scr, TYPEH( Type ), Arg ) );
}

void ScpIfc_AddKey( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 2 given to addkey" );
    ARG_TYPE( Type2, SCR_INT,  "Invalid arg 1 given to addkey" );
    SCP_DBGA( "addkey( [%x]%x, [%x]%x )", Arg2, Type2, Arg1, Type1 );
    if( Arg2 == -1 ){
        gScpIfcUnk07 = Arg1;
        gScpIfcUnk08 = scr;
    } else {
        if( Arg2 > 0xFF ) IntpError("Key out of range");
        gIntpList[ Arg2 ].scr = scr;
        gIntpList[ Arg2 ].i02 = Arg1;
    }
}

void ScpIfc_DeleteKey( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to deletekey" );
    SCP_DBGA( "deletekey( [%x]%x )", Arg, Type );
    if( Arg == -1 ){
        gScpIfcUnk07 = 0;
        gScpIfcUnk08 = 0;
    } else {
        if( Arg > 0xFF ) IntpError( "Key out of range" );
        gIntpList[ Arg ].scr = 0;
        gIntpList[ Arg ].i02 = 0;
    }
}

void ScpIfc_RefreshMouse( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to refreshmouse" );
    SCP_DBGA( "refreshmouse( [%x]%x )", Arg, Type );
    if( !WinMouseRfsh() ){ if( Arg ) SciUnk11( scr, Arg ); }
}

void ScpIfc_SetFont( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    if( (Type & 0xF7FF) != SCR_INT ) IntpError("Invalid arg 1 given to setfont");
    SCP_DBGA( "setfont( [%x]%x )", Arg, Type );
    if( !WinSetFont( Arg ) ) IntpError( "Error setting font" );
}

void ScpIfc_SetTextFlags( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to setflags" );
    SCP_DBGA( "setflags( [%x]%x )", Arg, Type );
    if( !WinSetTextFlags( Arg ) ) IntpError( "Error setting text flags" );
}

void ScpIfc_SetTextColor( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;
    float Arg3f, Arg2f, Arg1f;

    GET_ARGf( scr, Arg1, Arg1f, Type1 )
    GET_ARGf( scr, Arg2, Arg2f, Type2 )
    GET_ARGf( scr, Arg3, Arg3f, Type3 )
    SCP_DBGA( "set_text_color( [%x]%x, [%x]%x, [%x]%x )", Type3, Arg3, Type2, Arg2, Type1, Arg1 );
    if( (( SCRTYPE( Type3 ) == SCR_INT ) && ( Arg3f != 0.0 )) || 
	(( SCRTYPE( Type2 ) == SCR_INT ) && ( Arg2f != 0.0 )) || 
	(( SCRTYPE( Type1 ) == SCR_INT ) && ( Arg1f != 0.0 ))
	) IntpError("Invalid type given to settextcolor" );
    if( !WinSetTextColor(Arg3f, Arg2f, Arg1f) ) IntpError("Error setting text color");
}

void ScpIfc_SayOptionColor( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;
    float Arg3f, Arg2f, Arg1f;

    GET_ARGf( scr, Arg1, Arg1f, Type1 )
    GET_ARGf( scr, Arg2, Arg2f, Type2 )
    GET_ARGf( scr, Arg3, Arg3f, Type3 )
    SCP_DBGA( "sayoptioncolor( [%x]%x, [%x]%x, [%x]%x )", Type3, Arg3, Type2, Arg2, Type1, Arg1 );
    if( ( ( SCRTYPE( Type3 ) == SCR_INT ) && ( Arg3f != 0.0 ) ) || 
	( ( SCRTYPE( Type2 ) == SCR_INT ) && ( Arg2f != 0.0 ) )  || 
	( ( SCRTYPE( Type1 ) == SCR_INT ) && ( Arg1f != 0.0 ) )
	) IntpError( "Invalid type given to sayoptioncolor" );
    if( DialogSetOptionColor( Arg3f, Arg2f, Arg1f ) ) IntpError( "Error setting option color" );
}

void ScpIfc_SayReplytColor( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;
    float Arg3f, Arg2f, Arg1f;

    GET_ARGf( scr, Arg1, Arg1f, Type1 )
    GET_ARGf( scr, Arg2, Arg2f, Type2 )
    GET_ARGf( scr, Arg3, Arg3f, Type3 )
    SCP_DBGA( "sayreplycolor( [%x]%x, [%x]%x, [%x]%x )", Type3, Arg3, Type2, Arg2, Type1, Arg1 );
    if( ( ( SCRTYPE( Type3 ) == SCR_INT ) && ( Arg3f != 0.0 ) ) || 
	( ( SCRTYPE( Type2 ) == SCR_INT ) && ( Arg2f != 0.0 ) ) || 
	( ( SCRTYPE( Type1 ) == SCR_INT ) && ( Arg1f != 0.0 ) ) 
	) IntpError( "Invalid type given to sayreplycolor" );
    if( DialogSetReplyColor( Arg3f, Arg2f, Arg1f ) ) IntpError("Error setting reply color");
}

void ScpIfc_SetTextHighlightColor( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;
    float Arg3f, Arg2f, Arg1f;

    GET_ARGf( scr, Arg1, Arg1f, Type1 )
    GET_ARGf( scr, Arg2, Arg2f, Type2 )
    GET_ARGf( scr, Arg3, Arg3f, Type3 )
    SCP_DBGA( "sethighlightcolor( [%x]%x, [%x]%x, [%x]%x )", Type3, Arg3, Type2, Arg2, Type1, Arg1 );
    if( ( ( SCRTYPE( Type3 ) == SCR_INT ) && ( Arg3f != 0.0 ) ) || 
	( ( SCRTYPE( Type2 ) == SCR_INT ) && ( Arg2f != 0.0 ) ) || 
	( ( SCRTYPE( Type1 ) == SCR_INT ) && ( Arg1f != 0.0 ) )
      ) IntpError( "Invalid type given to sethighlightcolor" );
    if( !WinSetTextColorHL( Arg3f, Arg2f, Arg1f ) ) IntpError( "Error setting text highlight color" );
}

void ScpIfc_SayReplyWindow( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3,Type4,Type5;
    int Arg1,Arg2,Arg3,Arg4,Arg5;
    char *s;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )    
    SCP_DBGA( 
	"sayreplywindow( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );

    s = NULL;
    if( SCRTYPE( Type1 ) == SCR_STRING ){        
        s = strdup( IntpMseHandler( IntpGetString( scr, TYPEH(Type1), Arg1 ) ) );
    } else if( SCRTYPE( Type1 ) != SCR_INT || Arg1 ){
        IntpError( "Invalid arg 5 given to sayreplywindow" );
    }
    if( DialogReplyWin( Arg5, Arg4, Arg2, Arg3, s ) ) IntpError( "Error setting reply window" );
}

void ScpIfc_SayReplyFlags( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to sayreplyflags" );
    SCP_DBGA( "sayreplyflags( [%x]%x )", Arg, Type );
    if( !DialogSetOptionFlags( Arg ) ) IntpError( "Error setting reply flags" );
}

void ScpIfc_SayOptionFlags( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to sayoptionflags" );
    SCP_DBGA( "sayoptionflags( [%x]%x )", Arg, Type );
    if( !DialogSetOptionFlags( Arg ) ) IntpError( "Error setting option flags" );
}

void ScpIfc_SayOptionWindow( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3,Type4,Type5;
    int Arg1,Arg2,Arg3,Arg4,Arg5;
    char *s;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )
    SCP_DBGA( 
	"sayoptionwindow( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );

    s = NULL;
    if( SCRTYPE( Type1 ) == SCR_STRING ){        
        s = strdup( IntpMseHandler( IntpGetString( scr, TYPEH(Type1), Arg1 ) ) );
    } else if( Type5 != SCR_INT || Arg1 ){
        IntpError( "Invalid arg 5 given to sayoptionwindow" );
    }
    if( DialogOptWin( Arg5, Arg4, Arg2, Arg3, s ) ) IntpError( "Error setting option window" );
}

void ScpIfc_SayBorder( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2;
    int Arg1,Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 2 given to sayborder" );
    ARG_TYPE( Type2, SCR_INT, "Invalid arg 1 given to sayborder" );
    SCP_DBGA( "sayborder( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );

    if( DialogBorder( Arg2, Arg1 ) ) IntpError( "Error setting dialog border" );
}

void ScpIfc_SayScrollUp( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3,Type4,Type5,Type6;
    int Arg1,Arg2,Arg3,Arg4,Arg5,Arg6;
    char *s1,*s2,*s3,*s4;

    s1 = s2 = s3 = s4 = NULL;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )
    GET_ARGi( scr, Arg6, Type6 )
    SCP_DBGA( 
	"sayscrollup( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type6, Arg6,Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );

    if( SCRTYPE( Type1 ) == SCR_INT ){
        if( Arg1 != -1 && Arg1 ) IntpError( "Invalid arg 4 given to sayscrollup" );
        if( Arg1 == -1 ) Arg1 = 1;
    } else {
        if( SCRTYPE( Type1 ) != SCR_STRING ) IntpError( "Invalid arg 4 given to sayscrollup" );
    }
    if( SCRTYPE( Type2 ) == SCR_INT && Arg2 ) IntpError( "Invalid arg 3 given to sayscrollup" );
    if( SCRTYPE( Type3 ) == SCR_INT && Arg3 ) IntpError( "Invalid arg 2 given to sayscrollup" );
    if( SCRTYPE( Type4 ) == SCR_INT && Arg4 ) IntpError( "Invalid arg 1 given to sayscrollup" );
        if( SCRTYPE( Type4 ) == SCR_STRING ){ s1 = strdup( IntpMseHandler( IntpGetString( scr, TYPEH( Type4 ), Arg4 ) ) ); }
    if( SCRTYPE( Type3 ) == SCR_STRING ){ s2 = strdup( IntpMseHandler( IntpGetString( scr, TYPEH( Type3 ), Arg3 ) ) ); }
    if( SCRTYPE( Type2 ) == SCR_STRING ){ s3 = strdup( IntpMseHandler( IntpGetString( scr, TYPEH( Type2 ), Arg2 ) ) ); }
    if( SCRTYPE( Type1 ) == SCR_STRING ){ s4 = strdup( IntpMseHandler( IntpGetString( scr, TYPEH( Type1 ), Arg1 ) ) ); }
    if( DialogScrollUp( Arg6, Arg5, s1, s2, s3, s4, Arg1 ) ) IntpError( "Error setting scroll up" );
}

void ScpIfc_SayScrollDn( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1,Type2,Type3,Type4,Type5,Type6;
    int Arg1,Arg2,Arg3,Arg4,Arg5,Arg6;
    char *s1,*s2,*s3,*s4;

    s1 = s2 = s3 = s4 = NULL;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )
    GET_ARGi( scr, Arg6, Type6 )
    SCP_DBGA( 
	"sayscrolldn( [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x, [%x]%x )", 
	Type6, Arg6,Type5, Arg5, Type4, Arg4, Type3, Arg3, Type2, Arg2, Type1, Arg1
    );

    if( SCRTYPE( Type1 ) == SCR_INT ){
        if( Arg1 != -1 && Arg1 ) IntpError( "Invalid arg 4 given to sayscrolldn" );
        if( Arg1 == -1 ) Arg1 = 1;
    } else {
        if( (Type1 & 0xF7FF) != SCR_STRING ) IntpError( "Invalid arg 4 given to sayscrolldn" );
    }
    if( SCRTYPE( Type2 ) == SCR_INT && Arg2 ) IntpError( "Invalid arg 3 given to sayscrolldown" );
    if( SCRTYPE( Type3 ) == SCR_INT && Arg3 ) IntpError( "Invalid arg 2 given to sayscrolldown" );
    if( SCRTYPE( Type4 ) == SCR_INT && Arg4 ) IntpError( "Invalid arg 1 given to sayscrolldown" );
    if( SCRTYPE( Type4 ) == SCR_STRING ){ s1 = strdup( IntpMseHandler( IntpGetString( scr, TYPEH( Type4 ), Arg4 ) ) ); }
    if( SCRTYPE( Type3 ) == SCR_STRING ){ s2 = strdup( IntpMseHandler( IntpGetString( scr, TYPEH( Type3 ), Arg3 ) ) ); }
    if( SCRTYPE( Type2 ) == SCR_STRING ){ s3 = strdup( IntpMseHandler( IntpGetString( scr, TYPEH( Type2 ), Arg2 ) ) ); }
    if( SCRTYPE( Type1 ) == SCR_STRING ){ s4 = strdup( IntpMseHandler( IntpGetString( scr, TYPEH( Type1 ), Arg1 ) ) ); }
    if( DialogScrollDn( Arg6, Arg5, s1, s2, s3, s4, Arg1 ) )  IntpError("Error setting scroll down");
}

void ScpIfc_SaySetSpacing( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to saysetspacing" );
    SCP_DBGA( "saysetspacing( [%x]%x )", Arg, Type );
    if( DialogSetOptSpacing( Arg ) ) IntpError( "Error setting option spacing" );
}

void ScpIfc_Restart( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "restart_option()" );
    if( DialogRestartOpt() ) IntpError( "Error restarting option" );
}

void ScpIfcSoundCb( int *p, int a2 )
{
    if( a2 == 1 ) *p = 0;
}

int ScpIfcSoundDelete( int Idx )
{
    if(Idx == -1 ) return 1;
    if( (Idx & 0xA0000000) == 0 ) return 0;
    Idx = Idx & 0x5FFFFFFF;
    if( !gScpIfcSounds[ Idx ] ) return 0;
    if( SoundIsPlaying( gScpIfcSounds[ Idx ] ) ) SoundStop( gScpIfcSounds[ Idx ] );
    SoundDelete( gScpIfcSounds[ Idx ] );
    gScpIfcSounds[ Idx ] = NULL;
    return 1;
}

void ScpIfcDeleteSounds()
{
    int i;

    for( i = 0; i < 32; i++ ){
        if( gScpIfcSounds[ i ] ) ScpIfcSoundDelete( i | 0xA0000000 );
    }
}

unsigned int ScpIfcSounPlay( short Flags, char *fname )
{
    int a,b,i;
    Sound_t *snd;
    unsigned int err;

    a = 1;
    b = 0;
    if( Flags & 0x01 )
        b = 32;
    else
        a = 5;
    b |= ( Flags & 0x02 ) ? 0x08 : 0x10;
    if( Flags & 0x100 ) a = (a & 0xFC) | 0x01;
    if( Flags & 0x200 ) a = (a & 0xFC) | 0x02;
    for( i = 0; i < 32 && gScpIfcSounds[ i ]; i++ );
    if( i == 32 ) return -1;    
    if( !(snd = gScpIfcSounds[ i ] = SoundCreate( a, b )) ) return -1;
    SoundSetCallback( snd, (void *)ScpIfcSoundCb, &gScpIfcSounds[ i ] );
    if( Flags & 0x0001 ) SoundRepeat( gScpIfcSounds[ i ], 0xFFFF );
    if( Flags & 0x1000 ) SoundSetChannel( gScpIfcSounds[ i ], 2 );
    if( Flags & 0x2000 ) SoundSetChannel( gScpIfcSounds[ i ], 3 );

    if( SoundLoadFile( gScpIfcSounds[ i ], fname ) ){ SoundDelete( gScpIfcSounds[ i ] ); gScpIfcSounds[ i ] = 0; return -1; }
    err = SoundPlayAcm( gScpIfcSounds[ i ] );
    if( err < 0x19 ){
        if( err < 0x15 ){
            if( err == 20 ){ eprintf( "soundPlay error: %s\n", "SOUND_NO_DEVICE" ); SoundDelete( gScpIfcSounds[ i ] ); gScpIfcSounds[ i ] = 0; return -1; }
            return err | 0xA0000000;
        }
        if( err > 0x15 ){
            if( err >= 0x17 ){
                if( err <= 0x17 )
                    eprintf( "soundPlay error: %s\n", "SOUND_FUNC_NOT_SUPPORTED" );
                else
                    eprintf( "soundPlay error: %s\n", "SOUND_NO_BUFFERS_AVAILABLE" );
            } else {
        	eprintf( "soundPlay error: %s\n", "SOUND_NO_SOUND" );
            }
        } else {
    	    eprintf( "soundPlay error: %s\n", "SOUND_NOT_INITIALIZED" );
	}
        SoundDelete( gScpIfcSounds[ i ] );
        gScpIfcSounds[ i ] = 0;        
        return -1;
    }
    if( err <= 0x19 ){ eprintf( "soundPlay error: %s\n", "SOUND_FILE_NOT_FOUND" ); SoundDelete( gScpIfcSounds[ i ] ); gScpIfcSounds[ i ] = 0; return -1; }
    if( err >= 0x1D ){
        if( err <= 0x1D ){ eprintf( "soundPlay error: %s\n", "SOUND_NOT_PAUSED" ); SoundDelete( gScpIfcSounds[ i ] ); gScpIfcSounds[ i ] = 0; return -1; }
        if( err < 0x1F ){ eprintf( "soundPlay error: %s\n", "SOUND_INVALID_HANDLE" ); SoundDelete( gScpIfcSounds[ i ] ); gScpIfcSounds[ i ] = 0; return -1; }
        if( err <= 0x1F ){ eprintf( "soundPlay error: %s\n", "SOUND_NO_MEMORY" ); SoundDelete( gScpIfcSounds[ i ] ); gScpIfcSounds[ i ] = 0; return -1; }
        if( err == 32 ){ eprintf( "soundPlay error: %s\n", "SOUND_ERROR" ); SoundDelete( gScpIfcSounds[ i ] ); gScpIfcSounds[ i ] = 0; return -1; }
        return err | 0xA0000000;
    }
    if( err < 0x1B ){ eprintf( "soundPlay error: %s\n", "SOUND_ALREADY_PLAYING" ); SoundDelete( gScpIfcSounds[ i ] ); gScpIfcSounds[ i ] = 0; return -1; } else 
    if( err <= 0x1B ){ eprintf( "soundPlay error: %s\n", "SOUND_NOT_PLAYING" ); SoundDelete( gScpIfcSounds[ i ] ); gScpIfcSounds[ i ] = 0; return -1; } else 
    { eprintf( "soundPlay error: %s\n", "SOUND_ALREADY_PAUSED" ); SoundDelete( gScpIfcSounds[ i ] ); gScpIfcSounds[ i ] = 0; return -1; }
    return 0;
}

int ScpIfcSoundPause( int Idx )
{
    if( Idx == -1 ) return 1;
    if( !(Idx & 0xA0000000) ) return 0;
    Idx = Idx & 0x5FFFFFFF;
    if( gScpIfcSounds[ Idx ] ) return 0;
    if( SoundResume( gScpIfcSounds[ Idx ], 1 ) ) return SoundStop( gScpIfcSounds[ Idx ] ) == 0;
    return SoundPause( gScpIfcSounds[ Idx ] ) == 0;
}

int ScpIfcSoundRewind( int Idx )
{
    if( Idx == -1 ) return 1;
    if( !(Idx & 0xA0000000) ) return 0;
    Idx = Idx & 0x5FFFFFFF;
    if( !gScpIfcSounds[ Idx ] ) return 0;
    if( !SoundIsPlaying( gScpIfcSounds[ Idx ] ) ) return 1;
    SoundStop( gScpIfcSounds[ Idx ] );
    return SoundPlayAcm( gScpIfcSounds[ Idx ] ) == 0;
}

int ScpIfcSounResume( int Idx )
{
    if( Idx == -1 ) return 1;
    if( !(Idx & 0xA0000000) ) return 0;
    Idx = Idx & 0x5FFFFFFF;
    if( !gScpIfcSounds[ Idx ] ) return 0;
    if( SoundResume( gScpIfcSounds[ Idx ], 1 ) ) return SoundPlayAcm( gScpIfcSounds[ Idx ] ) == 0;
    return SoundUnPause( gScpIfcSounds[ Idx ] ) == 0;
}

void ScpIfc_SoundPlay( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 2 given to soundplay" );
    ARG_TYPE( Type2, SCR_STRING,  "Invalid arg 1 given to soundplay" );    
    SCP_DBGA( "soundplay( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    IntpPushiA( scr, ScpIfcSounPlay( Arg1, IntpMseHandler( IntpGetString( scr, TYPEH( Type2 ), Arg2 ) ) ) );
    IntpPushwA( scr, SCR_INT );
}

void ScpIfc_SoundPause( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to soundpause" );
    SCP_DBGA( "soundpause( [%x]%x )", Arg, Type );
    ScpIfcSoundPause( Arg );
}

void ScpIfc_SoundResume( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to soundresume" );
    SCP_DBGA( "soundresume( [%x]%x )", Arg, Type );
    ScpIfcSounResume( Arg );
}

void ScpIfc_SoundStop( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to soundstop" );
    SCP_DBGA( "soundstop( [%x]%x )", Arg, Type );
    ScpIfcSoundPause( Arg );
}

void ScpIfc_SoundRewind( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to soundrewind" );
    SCP_DBGA( "soundrewind( [%x]%x )", Arg, Type );
    ScpIfcSoundRewind( Arg );
}

void ScpIfc_SoundDelete( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to sounddelete" );
    SCP_DBGA( "sounddelete( [%x]%x )", Arg, Type );
    ScpIfcSoundDelete( Arg );
}

void ScpIfc_SetOneOptPause( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "SetOneOptPause: invalid arg passed (non-integer)." );
    SCP_DBGA( "SetOneOptPause( [%x]%x )", Arg, Type );
    if( Arg ){
        if( !( DialogGetOneOptPause() & 0x08 ) ) return;
    } else if( DialogGetOneOptPause() & 0x08 ){
        return;
    }
    DialogSetOneOptPause( 0x08 );
}

int ScpIfcNevsUpdate()
{
    NevsUpdate();
    return 0;
}

int ScpUnk02( unsigned int a1 )
{
    if( a1 > 0xFF ) return 0;
    if( gScpIfcUnk08 ){
        if( gScpIfcUnk07 ) SciUnk11(gScpIfcUnk08, gScpIfcUnk07);
        return 1;
    }
    if( !gIntpList[ a1 ].scr ) return 0;
    if( gIntpList[ a1 ].i02 ) SciUnk11( gIntpList[ a1 ].scr, gIntpList[ a1 ].i02 );
    return 1;
}

void ScpIfcInit()
{
    WinDlgInsertExec( (void *)ScpUnk02 );
    SciAddOpcode( 0x806A, ScpIfc_FillWin3x3 );
    SciAddOpcode( 0x808C, ScpIfc_DeleteButton );
    SciAddOpcode( 0x8086, ScpIfc_AddButton );
    SciAddOpcode( 0x8088, ScpIfc_AddButtonFlag );
    SciAddOpcode( 0x8087, ScpIfc_AddButtonText );
    SciAddOpcode( 0x8089, ScpIfc_AddButtonGFX );
    SciAddOpcode( 0x808A, ScpIfc_AddButtonProc );
    SciAddOpcode( 0x808B, ScpIfc_AddButtonRightProc );
    SciAddOpcode( 0x8067, ScpIfc_ShowWin );
    SciAddOpcode( 0x8068, ScpIfc_FillWin );
    SciAddOpcode( 0x8069, ScpIfc_FillRect );
    SciAddOpcode( 0x8072, ScpIfc_Print );
    SciAddOpcode( 0x8073, ScpIfc_MessageFormat );
    SciAddOpcode( 0x8074, ScpIfc_PrintRect );
    SciAddOpcode( 0x8075, ScpIfc_SetFont );
    SciAddOpcode( 0x8076, ScpIfc_SetTextFlags );
    SciAddOpcode( 0x8077, ScpIfc_SetTextColor );
    SciAddOpcode( 0x8078, ScpIfc_SetTextHighlightColor );
    SciAddOpcode( 0x8064, ScpIfc_Select );
    SciAddOpcode( 0x806B, ScpIfc_Display );
    SciAddOpcode( 0x806D, ScpIfc_DisplayRaw );
    SciAddOpcode( 0x806C, ScpIfc_DisplayGFX );
    SciAddOpcode( 0x806F, ScpIfc_FadeIn );
    SciAddOpcode( 0x8070, ScpIfc_FadeOut );
    SciAddOpcode( 0x807A, ScpIfc_PlayMovie1 );
    SciAddOpcode( 0x807B, ScpIfc_SettingMovieFlag );
    SciAddOpcode( 0x807C, ScpIfc_PlayMovie );
    SciAddOpcode( 0x8079, ScpIfc_MovieStat );
    SciAddOpcode( 0x807F, ScpIfc_SetRegion );
    SciAddOpcode( 0x8080, ScpIfc_AddRegionFlag );
    SciAddOpcode( 0x8081, ScpIfc_SetRegionProc );
    SciAddOpcode( 0x8082, ScpIfc_SetRightButtonProc );
    SciAddOpcode( 0x8083, ScpIfc_DeleteRegion );
    SciAddOpcode( 0x8084, ScpIfc_ActivateRegion );
    SciAddOpcode( 0x8085, ScpIfc_CheckRegion );
    SciAddOpcode( 0x8062, ScpIfc_CreateWindow );
    SciAddOpcode( 0x8063, ScpIfc_DeleteWindow );
    SciAddOpcode( 0x8065, ScpIfc_ResizeWindow );
    SciAddOpcode( 0x8066, ScpIfc_ScaleWindow );
    SciAddOpcode( 0x804E, ScpIfc_SayStart );
    SciAddOpcode( 0x804F, ScpIfc_StartDialog );
    SciAddOpcode( 0x8050, ScpIfc_SetTitle );
    SciAddOpcode( 0x8051, ScpIfc_SayGotoReply );
    SciAddOpcode( 0x8053, ScpIfc_SayOption );
    SciAddOpcode( 0x8052, ScpIfc_SayReply );
    SciAddOpcode( 0x804D, ScpIfc_SayEnd );
    SciAddOpcode( 0x804C, ScpIfc_SayQuit );
    SciAddOpcode( 0x8054, ScpIfc_SayMessage );
    SciAddOpcode( 0x8055, ScpIfc_SayReplyWindow );
    SciAddOpcode( 0x8056, ScpIfc_SayOptionWindow );
    SciAddOpcode( 0x805F, ScpIfc_SayReplyFlags );
    SciAddOpcode( 0x8060, ScpIfc_SayOptionFlags );
    SciAddOpcode( 0x8057, ScpIfc_SayBorder );
    SciAddOpcode( 0x8058, ScpIfc_SayScrollUp );
    SciAddOpcode( 0x8059, ScpIfc_SayScrollDn );
    SciAddOpcode( 0x805A, ScpIfc_SaySetSpacing );
    SciAddOpcode( 0x805B, ScpIfc_SayOptionColor );
    SciAddOpcode( 0x805C, ScpIfc_SayReplytColor );
    SciAddOpcode( 0x805D, ScpIfc_Restart );
    SciAddOpcode( 0x805E, ScpIfc_unk09 );
    SciAddOpcode( 0x8061, ScpIfc_SayMsgTimeout );
    SciAddOpcode( 0x8071, ScpIfc_GotoXY );
    SciAddOpcode( 0x808D, ScpIfc_MouseRedraw );
    SciAddOpcode( 0x808E, ScpIfc_MouseDrawCursor );
    SciAddOpcode( 0x8090, ScpIfc_RefreshMouse );
    SciAddOpcode( 0x808F, ScpIfc_MouseShape );
    SciAddOpcode( 0x8091, ScpIfc_SetGlobalMouseFunc );
    SciAddOpcode( 0x806E, ScpIfc_LoadPaletteTable );
    SciAddOpcode( 0x8092, ScpIfc_AddNamedEvent );
    SciAddOpcode( 0x8093, ScpIfc_AddNamedHandler );
    SciAddOpcode( 0x8094, ScpIfc_ClearNamed );
    SciAddOpcode( 0x8095, ScpIfc_SignalNamed );
    SciAddOpcode( 0x8096, ScpIfc_AddKey );
    SciAddOpcode( 0x8097, ScpIfc_DeleteKey );
    SciAddOpcode( 0x8098, ScpIfc_SoundPlay );
    SciAddOpcode( 0x8099, ScpIfc_SoundPause );
    SciAddOpcode( 0x809A, ScpIfc_SoundResume );
    SciAddOpcode( 0x809B, ScpIfc_SoundStop );
    SciAddOpcode( 0x809C, ScpIfc_SoundRewind );
    SciAddOpcode( 0x809D, ScpIfc_SoundDelete );
    SciAddOpcode( 0x809E, ScpIfc_SetOneOptPause );
    SciAddOpcode( 0x809F, ScpIfc_SelectFileList );
    SciAddOpcode( 0x80A0, ScpIfc_TokenizeString );
    NevsInitOnce( );
    ScpGameSetup();
}

