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
int gScpIfcUnk08;
int gScpIfcUnk07;

/*************************************************/

void ScpIfc_FillWin3x3( Intp_t *scr ) // 806A
{
    unsigned short Type;
    int Arg;
//    char *s;
//    char *v13;
//    void *p;
//    RECT *v16;
//    int v15, v18, v19;

//    v19 = v18 = 0;
    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to fillwin3x3" )
    IntpGetArg( scr, TYPEH( Type ), Arg );
//    s = IntpMseHandler();
//    p = FillWin5( &v16, &v15 );
//    if( !p ) IntpError( "cannot load 3x3 file '%s'", s );    
//    FillRect( v15, v16, Type );
//    FillWin1( &p->SrcType, v19, FillWin4(), v18, FillWin3(), FillWin2() );
//    dbg_free( p );
}

void ScpIfc_MessageFormat( Intp_t *scr ) // 8073
{
    unsigned short Type3, Type4, Type1, Type2, Type5, Type6;
    int Arg1, Arg2, Arg3, Arg4, Arg5, Arg6;
    char *s;

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
    s = IntpGetArg(scr, TYPEH( Type6 ), Arg6 );
//    if( !FormatMsg( s, Arg5, Arg3, Arg4, Arg2, Arg1) ) IntpError( "Error formatting message\n" );
}

void ScpIfc_Print( Intp_t *scr ) // 8072
{
    unsigned short Type;
    int Arg;

//    FillRect( Idx, vars0, vars0_4 );
    GET_ARGi( scr, Arg, Type );
    Type &= ~0x0800;
    switch( Type ){
	case SCR_STRING: IntpLog( "%s", IntpGetArg( scr, TYPEH( Type ), Arg ) ); break;
	case SCR_FLOAT: IntpLog( "%.5f", Arg ); break;
	case SCR_INT: IntpLog( "%d", Arg ); break;
    }
}

void ScpIfc_SelectFileList( Intp_t *scr ) // 809F
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *str1, str2;

    scr->Flags |= 0x20;

    GET_ARGi( scr, Arg1, Type1 );
    ARG_TYPE( Type1, SCR_STRING, "Error, invalid arg 2 given to selectfilelist" );
    str2 = IntpGetArg( scr, TYPEH( Type1 ), Arg1 );

    GET_ARGi( scr, Arg2, Type2 );
    ARG_TYPE( Type2, SCR_STRING, "Error, invalid arg 1 given to selectfilelist" );
    str1 = IntpGetArg( scr, TYPEH( Type2 ), Arg2 );
/*
    v11 = IntpMseHandler();
    v13 = Unk6015( v11, v12 );
    if( v13 && v19 ){
        v18 = gPalColorCubeRGB[31][31][31] | 0x10000;
        Map1 = LoadMap1( str1, v13, 0, v19, 320 - gFont.LineWidth( str1 ) / 2 - 10, 200, v18 );
        if( Map1 == -1 ){
            IntpPushiA( scr, 0 );
            IntpPushwA( scr, SCR_INT );
        } else {
            IntpPushiA( scr, IntpDbgStr(scr, v13[Map1], v19 ) );
            IntpPushwA( scr, SCR_FSTRING );
        }        
        FileList(v13);
    } else {
        IntpPushiA( scr, 0 );
        IntpPushwA( scr, SCR_INT );
    }
    scr->Flags &= ~0x20;
*/
}

void ScpIfc_TokenizeString( Intp_t *scr ) // 80A0
{
/*
    xx = 0;
    v25 = 0;
    errcall = v4;

    GET_ARGi( scr, Arg1, Type1 );
    ARG_TYPE( Type1, SCR_INT, "Error, invalid arg 3 to tokenize." );

    v29 = v5;
    GET_ARGi( scr, Arg2, Type2 );

    switch( Type2 & ~0x800 ){
	case SCR_INT: if( Arg2 ) IntpError( "Error, invalid arg 2 to tokenize. (only accept 0 for int value)" ); break;
	case SCR_STRING: xx = IntpGetArg( scr, TYPEH( Type2 ), Arg2 ); break;
	default: IntpError( "Error, invalid arg 2 to tokenize. (string)" ); break;
    }

    GET_ARGi( scr, Arg3, Type3 );
    ARG_TYPE( Type3, SCR_STRING, "Error, invalid arg 1 to tokenize." );

    s = IntpGetArg(scr, TYPEH( Type3 ), Arg3 );
    if( !xx ){
        if( *s != v29 ){
            do{
                if( !*s ) break;
                v21 = *++s;
                ++xx;
            } while ( v21 != v29 );
        }
        if( s ){
            p = dbg_calloc(1, (xx + 1), 248);
            strncpy( p, s, xx );
            IntpPushiA( scr, IntpDbgStr( scr, p, xx ) );
	    IntpPushwA( scr, SCR_FSTRING );
	    if( p ) dbg_free( p );
	    return;
        }
        goto LABEL_32;
    }
    v13 = strstr_(s, xx);
    if( !v13 ) goto LABEL_32;
    v14 = &v13[strlen(xx)];
    if( v29 != *v14 ){
        do{
            if( !*v14 ) break;
            v15 = *++v14;
        } while ( v15 != v29 );
    }
    if( *v14 != v29 ){
LABEL_32:
        IntpPushiA( scr, 0 );
	IntpPushwA( scr, SCR_INT );
	if( errcall ) dbg_free( errcall );
	return;
    }
    v16 = v14 + 1;
    v17 = v16;
    if( *v16 != v29 ){
        do{
            if( !*v17 ) break;
            v18 = *++v17;
            ++v25;
        } while ( v18 != v29 );
    }
    p = dbg_calloc( 1, v25 + 1, 230 );
    strncpy( p, v16, v25 );
    IntpPushiA( scr, IntpDbgStr( scr, p, v25 ) );
    IntpPushwA( scr, SCR_FSTRING );
    if( errcall ) dbg_free( p );
    return;
*/
}

void ScpIfc_PrintRect( Intp_t *scr ) // 8074
{
    unsigned short Type1, Type2, Type3;
    int Arg1, Arg2, Arg3;
    float Arg3f;
//    char *s;

//    FillRect(*stmp, *&stmp[4], *&stmp[8]);
    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGf( scr, Arg3, Arg3f, Type3 )

    if( (Type1 & 0xF7FF) != SCR_INT || Arg1 > 2 ) IntpError("Invalid arg 3 given to printrect, expecting int");
    if( (Type2 & 0xF7FF) != SCR_INT ) IntpError("Invalid arg 2 given to printrect, expecting int");
//    if( (Type3 & 0xF700) >= SCR_FLOAT ){
//        if( (Type3 & 0xF700) > SCR_FLOAT ){
//            if( (Type3 & 0xF700) == SCR_INT ) sprintf( &stmp[12], "%d", Arg3 );
//        } else {
//    	    s = &stmp[12];
//    	    sprintf( &stmp[12], "%.5f", Arg3f );
//        }
//    } else {
//	if( Type2 == SCR_STRING ) s = IntpGetArg( scr, t1[1], Arg2 );
//    }
//    if( !PrintRect(s, t3, Arg1) ) IntpError("Error in printrect");
}

void ScpIfc_Select( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to select" );
//    x = IntpGetArg(scr, TYPEH( Type ), Arg );
//    v7 = WinSelect( x );
//    if( v7 == -1 ) IntpError("Error selecing window %s\n", IntpGetArg( v6, TYPEH( Type ), Arg ) );
//    v6->i34 = v7;
//    IntpSetErrHandler( IntpErrHandlerCb );
}

void ScpIfc_Display( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to display" );
    IntpGetArg(scr, TYPEH(Type), Arg);
//    FillRect(v10, v11, v12);
//    IntpMseHandler();
//    WinUnk10();
}

void ScpIfc_DisplayRaw( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to displayraw" );
    IntpGetArg(scr, TYPEH( Type ), Arg);
//    FillRect(v10, v11, v12);
//    IntpMseHandler();
//    WinUnk13();
}

void ScpIfcPal( unsigned char *eax0, Pal8_t *a2, float a3, int a4 )
{
/*
    v13 = eax0;
    Palette = a2;
    a1 = TimerGetSysTime();
    v4 = a1;
    v5 = 0;
    v17 = lround(a3);
    v16 = 0;
    if( (v6 & 0x7FFFFFFF) != 0 ){
        while( v5 < v17 ){
            if( v16 ){
                v7 = Palette;
                v8 = v13;
                for( i = 0; i < 768; i++ ){
                    v10 = v5 * (*v8 - v7->r) / v17;
                    v7++;
                    LOBYTE(v10) = *v8++ - v10;
                    v12[ i ] = v10;
                }
                PalSetColors(v12);
                v4 = a1;
                v5 += v16;
            }
            if( a4 ) InpWinUpdate();
            v16 = TimerGetSysTime() - v4;
        }
    }
    PalSetColors( Palette );
*/
}

void ScpIfcPalA( unsigned char *a1, Pal8_t *a2, float a3 )
{
    ScpIfcPal( a1, a2, a3, 1 );
}

int ScpIfcUnk01()
{
//    return gScpIfcUnk01;
}

void ScpIfcUnk02( float a1 )
{
/*
    Pal8_t *v2; // edx
    int v3; // ecx

    MseIsCursorClear();
    MseCursorRedraw();
    ScpIfcPal( PalGetCurrent(), v2, a1, 1);
    if( !v3 ) MseDrawCursor();
*/
}

void ScpIfcUnk03( float a1 )
{
//    ScpIfcPal( &gScpIfcUnk02, &gPalBase, a1, 1 );
}

void ScpIfcUnk04( float a1 )
{
/*
    Pal8_t *v2; // edx
    int v3; // ecx

    MseIsCursorClear();
    MseCursorRedraw();
    ScpIfcPal( PalGetCurrent(), v2, a1, 0);
    if( !v3 ) MseDrawCursor();
*/
}

void ScpIfcUnk05( float a1 )
{
//    ScpIfcPal(&gScpIfcUnk02, &gPalBase, a1, 0);
}

void ScpIfc_FadeIn( Intp_t *scr )
{
    unsigned short Type;
    int Arg, cc;
    float Argf;

    GET_ARGf( scr, Arg, Argf, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid type given to fadein\n" );
    scr->Flags |= 0x20;
//    PalSetColors( &gScpIfcUnk02 );
//    ScpIfcPal( &gScpIfcUnk02, v7, Argf, 1 );
//    gScpIfcUnk01 = 1;
    scr->Flags &= ~0x20;
}

void ScpIfc_FadeOut( Intp_t *scr )
{
    unsigned short Type;
    int Arg, cc;
    float Argf;

    GET_ARGf( scr, Arg, Argf, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid type given to fadein\n" );
    scr->Flags |= 0x20;
    cc = MseIsCursorClear();
    MseCursorRedraw();
//    ScpIfcPal( PalGetCurrent(), v9, Argf, 1 );
    if( !cc ) MseDrawCursor();
//    gScpIfcUnk01 = 0;
    scr->Flags = scr->Flags & ~0x20;
}

int ScpIfcUnk06()
{
//    if( Unk6002() <= 0 ) return Unk6003();
    return 1;
}

void ScpIfc_SettingMovieFlag( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
//    if( !SetMovieFlags( Arg ) ) IntpError( "Error setting movie flags\n" );
}

void ScpIfc_PlayMovie1( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid arg given to playmovie" );
//    strcpy( gScpIfcUnk03, IntpGetArg( scr, TYPEH( Type ), Arg ) );
//    if( !strchr( gScpIfcUnk03, '.' ) ) strcpy( &gScpIfcUnk03 + strlen( gScpIfcUnk03 ), ".mve" );
//    FillRect( v14, v15, v16 );
    scr->Flags |= 0x10;
    scr->Func = ScpIfcUnk06;
    IntpMseHandler();
//    if( !PlayMovie1() ) IntpError( "Error playing movie" );
}

void ScpIfc_PlayMovie( Intp_t *scr )
{
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )
    ARG_TYPE( Type5, SCR_STRING, "Invalid arg given to playmovie" );
//    strcpy( gScpIfcUnk04, IntpGetArg(scr, TYPEH(Type5), Arg5) );
//    if( !strchr( gScpIfcUnk04, '.') ) strcpy( gScpIfcUnk04 + strlen( gScpIfcUnk04 ), ".mve" );
//    FillRect( Arg2, Arg1, Arg4 );
    scr->Func = ScpIfcUnk06;
    scr->Flags |= 0x10;
    IntpMseHandler();
//    if( !PlayMovie(v24, v25, Arg3, v23) ) IntpError( "Error playing movie" );
}

void ScpIfc_MovieStat( Intp_t *scr )
{
//    Unk6001();
//    LOBYTE(scr->Flags) |= 0x40u;
}

void ScpIfc_DeleteRegion( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    if( (Type & 0xF7FF) == SCR_INT && Arg != -1 ) IntpError( "Invalid type given to deleteregion" );
//    FillRect(v12, v13, v14);
//    if( Arg == -1 )
//        s = NULL;
//    else
//        s = IntpGetArg( scr, TYPEH( Type ), Arg );
//    DeleteRegion( s );
}

void ScpIfc_ActivateRegion( Intp_t *scr )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    IntpGetArg( scr, TYPEH( Type2 ), Arg2 );
//    ActivateRegion( v11, Arg1 );
}

void ScpIfc_CheckRegion( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid arg 1 given to checkregion();\n" );
    IntpGetArg(scr, TYPEH( Type ), Arg );
//    IntpPushiA( scr, ChkRegion() );
    IntpPushwA( scr, SCR_INT );
}

void ScpIfc_SetRegion( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid number of elements given to region" );
    if ( Arg < 2 ) IntpError( "addregion call without enough points!" );
/*
    FillRect( v29, v30, v31 );
    WinUnk106( Arg >> 1 );
    while( Arg >= 2 ){
	GET_ARGi( scr, a1, t1 )
        if( (t1 & 0xF7FF) != SCR_INT ) IntpError("Invalid y value given to region");
	GET_ARGi( scr, a2, t2 )
        if( (t2 & 0xF7FF) != SCR_INT ) IntpError("Invalid x value given to region");
        v19 = (a1 * GetScreenHight() + 479) / 480;
        DisplResW = GetDisplResW();
        Arg -= 2;
        sub_4BAB68((v21 * DisplResW + 639) / 640, v19, 1);
    }
    if( Arg ){
        t1 = IntpPopwA( scr );
        a1 = IntpPopiA( scr );
        if( t1 == SCR_FSTRING ) IntpStringDeRef(scr, SCR_FSTRING, a1);
        if( (t1 & 0xF7FF) != SCR_STRING && t1 == SCR_INT ){
            if( a1 ) IntpError( "Invalid name given to region" );
        }
        SetRegionA( IntpGetArg( scr, TYPEH( t1 ), a1 ) );
        SetRegionB();
    } else {
        IntpError( "Unnamed regions not allowed\n" );
        SetRegionB();
    }
*/
}

void ScpIfc_SetRegionProc( Intp_t *scr )
{
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;

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
//    x = IntpGetArg(scr, TYPEH(hDC), Arg5);
//    FillRect( Arg1, Arg2, Arg5 );
//    if( !WindowRegionSetProcedure(x, scr, v12, Arg4, Type2, v11) ) IntpError( "Error setting procedures to region %s\n", x );
}

void ScpIfc_SetRightButtonProc( Intp_t *scr )
{
    unsigned short Type1, Type2, Type3;
    int Arg1, Arg2, Arg3;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )

    ARG_TYPE( Type1, SCR_INT, "Invalid procedure 2 name given to addregionrightproc" );
    ARG_TYPE( Type2, SCR_INT, "Invalid procedure 1 name given to addregionrightproc" );
    ARG_TYPE( Type3, SCR_STRING, "Invalid name given to addregionrightproc" );
//    x = IntpGetArg( scr, TYPEH( Type3 ), Arg3 );
//    FillRect( Arg3, Arg1, Arg2 );
//    if( !WindowRightButtonSetProcedure(x, v11, v12, Arg2) ) IntpError("Error setting right button procedures to region %s\n", x);

}

void ScpIfc_CreateWindow( Intp_t *scr )
{
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )

//    s = IntpGetArg(scr, TYPEH( Type5 ), Idx);
//    GetDisplResW();
//    DisplResW = GetDisplResW();
//    v25 = (Arg2 * DisplResW + 639) / v24;
//    v26 = (Arg3 * GetScreenHight() + 479) / 0x1E0u;
//    ScreenHight = GetScreenHight();
//    if( WinCreate( s, v25, v26, (v28 * ScreenHight + 479) / v29, gPalColorCubeRGB[0][0][0], 0) == -1 ) IntpError("Couldn't create window.");
}

void ScpIfc_ResizeWindow( Intp_t *scr )
{
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )

//    s = IntpGetArg(scr, TYPEH( Type5 ), Arg5);
//    GetDisplResW();
//    DisplResW = GetDisplResW();
//    v25 = (Arg2 * DisplResW + 639) / v24;
//    v26 = (Arg3 * GetScreenHight() + 479) / 480u;
//    ScreenHight = GetScreenHight();
//    if( WinRes( s, v25, v26, (v28 * ScreenHight + 479) / v29) == -1 ) IntpError("Couldn't resize window.");
}

void ScpIfc_ScaleWindow( Intp_t *scr )
{
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )

//    s = IntpGetArg(scr, TYPEH( Type5 ), Arg5 );
//    GetDisplResW();
//    DisplResW = GetDisplResW();
//    v25 = (Arg2 * DisplResW + 639) / v24;
//    v26 = (Arg3 * GetScreenHight() + 479) / 480;
//    ScreenHight = GetScreenHight();
//    if( ScaleWin( s, v25, v26, (v28 * ScreenHight + 479) / v29) == -1 ) IntpError( "Couldn't scale window." );
}

void ScpIfc_DeleteWindow( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
//    x = IntpGetArg( scr, Type, Arg );
//    if( !DelWin(x) ) IntpError( "Error deleting window %s\n", x );
//    scr->i34 = DelWin1();
}

void ScpIfc_SayStart( Intp_t *scr )
{
    scr->Flags |= 0x20;
//    gScpIfcUnk05 = 0;
//    if( SayStart( scr ) ){
//        scr->Flags &= ~0x20;
//        IntpError("Error starting dialog.");
//    }
    scr->Flags &= ~0x20;
}

void ScpIfc_StartDialog( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    scr->Flags |= 0x20;
//    gScpIfcUnk05 = Arg;
//    if( SayStart( scr ) ){
//        scr->Flags &= ~0x20;
//        IntpError( "Error starting dialog." );
//    }
    scr->Flags &= ~0x20;
}

void ScpIfc_SetTitle( Intp_t *scr )
{
    unsigned short Type;
    int Arg;
    char *x;

    x = NULL;
    GET_ARGi( scr, Arg, Type )
    if( Type & ~0x800 == SCR_STRING ) x = IntpGetArg(scr, TYPEH(Type), Arg);
//    if( SetTitle( x ) ) IntpError("Error setting title.");
}

void ScpIfc_SayGotoReply( Intp_t *scr )
{
    unsigned short Type;
    int Arg;
    char *x;

    x = NULL;
    GET_ARGi( scr, Arg, Type )
    if( Type & ~0x800 == SCR_STRING ) x = IntpGetArg(scr, TYPEH(Type), Arg);
//    if( SayGotoReply(x) ) IntpError("Error during goto, couldn't find reply target %s\n", x);
}

void ScpIfc_SayOption( Intp_t *scr )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *x, *k;
    
    scr->Flags |= 0x20;
    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    if( (Type2 & ~0x800) == SCR_STRING )
        x = IntpGetArg( scr, TYPEH( Type2 ), Arg2 );
    else
        x = 0;
/*
    if( (Type1 & ~0x800) == SCR_STRING ){
        k = IntpGetArg(scr, TYPEH(Type1), Arg1);
        if( SetOption(x, k) ){
LABEL_10:
            scr->Flags &= ~0x20;
            IntpError("Error setting option.");
        }
    } else {
        if( (Type1 & ~0x800) != SCR_INT ){
            IntpError("Invalid arg 2 to sayOption");
            goto LABEL_15;
        }
        if( SetOption1(x, Arg1) ) goto LABEL_10;
    }
LABEL_15:
*/
    scr->Flags &= ~0x20;
}

void ScpIfc_SayReply( Intp_t *scr )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *s, *k;

    scr->Flags |= 0x20;
    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    if( (Type2 & ~0x800) == SCR_STRING )
        s = IntpGetArg(scr, TYPEH(Type2), Arg2);
    else
        s = 0;
        
    if( (Type1 & ~0x800) == SCR_STRING )
        k = IntpGetArg(scr, TYPEH(Type1), Arg1);
    else
        k = 0;
//    if( SayReply( s, k ) ){
//        scr->Flags &= ~0x20;
//        IntpError( "Error setting option." );
//    }
    scr->Flags &= ~0x20;
}

int ScpIfcUnk08( Intp_t *scr )
{
//    LOBYTE(scr->Flags) |= 0x40u;
//    return Unk6002() != -1;
}

void ScpIfc_SayEnd( Intp_t *scr )
{
/*
    int err;

    scr->Flags |= 0x20;
    err = SayEnd( gScpIfcUnk05 );
    scr->Flags = scr->Flags & ~0x20;
    if( err == -2 ){
        scr->Func = ScpIfcUnk08;
        scr->Flags |= 0x10;
    }
*/
}

void ScpIfc_unk09( Intp_t *scr )
{
/*
    IntpPushiA( scr, sub_431184() );
    IntpPushwA( scr, SCR_INT );
*/
}

void ScpIfc_SayQuit( Intp_t *scr )
{
//    if( SayQuit() ) IntpError( "Error quitting option." );
}

int ScpIfcUnk09()
{
//    return gScpIfcUnk06;
}

int ScpIfcUnk07( int result )
{
//    gScpIfcUnk06 = result;
//    return result;
}

void ScpIfc_SayMsgTimeout( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, 0x4000, "sayMsgTimeout:  invalid var type passed.");
//    gScpIfcUnk06 = Arg;
}

void ScpIfc_SayMessage( Intp_t *scr )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *s;

    scr->Flags |= 0x20;
    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    if( (Type2 & ~0x08FF) == SCR_STRING )
        s = IntpGetArg(scr, TYPEH(Type2), Arg2);
    else
        s = NULL;

    if( (Type1 & 0xF7FF) == SCR_STRING ) IntpGetArg( scr, TYPEH( Type1 ), Arg1 );

//    if( SayMsg( s, gScpIfcUnk06 ) ){
//        scr->Flags &= ~0x20;
//        IntpError( "Error setting option." );
//    }
    scr->Flags &= ~0x20;
}

void ScpIfc_GotoXY( Intp_t *scr )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    if( (Type1 & 0xF7FF) != SCR_INT || (Type2 & 0xF7FF) != SCR_INT ) IntpError( "Invalid operand given to gotoxy" );
//    FillRect(Type2, Type1, v16);
//    GotoXY(Arg2, Arg1);
}

void ScpIfc_AddButtonFlag( Intp_t *scr )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )    
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 2 given to addbuttonflag" );
    ARG_TYPE( Type2, SCR_STRING, "Invalid arg 1 given to addbuttonflag" );
//    if( !AddButtonFlag( IntpGetArg( scr, TYPEH( Type2 ), Arg2 ), Arg1 ) ) IntpError( "Error setting flag on button %s", IntpGetArg( scr, TYPEH( Type2 ), Arg2 ) );
}

void ScpIfc_AddRegionFlag( Intp_t *scr )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    
    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )        
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 2 given to addregionflag" );
    ARG_TYPE( Type2, 36865, "Invalid arg 1 given to addregionflag" );
//    if( !AddRegionFlag( IntpGetArg( scr, TYPEH( Type2 ), Arg2), Arg1 ) ) IntpError("Error setting flag on region %s", IntpGetArg( scr, TYPEH( Type2 ), Arg2 ) );
}

void ScpIfc_AddButton( Intp_t *scr )
{
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
/*
    FillRect(Arg4, v32, Arg5);
    ScreenHight = GetScreenHight();
    y = (v27 * ScreenHight + 479) / 0x1E0u;
    GetDisplResW();
    x = (Arg3 * GetScreenHight() + 479) / 0x1E0u;
    GetDisplResW();
    WindowAddButton( IntpGetArg(scr, Type5, Arg5), x, y, 0);
*/
}

void ScpIfc_AddButtonTextString( Intp_t *scr )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Arg1, SCR_STRING, "Invalid text string given to addbuttontext" );
    ARG_TYPE( Arg2, SCR_STRING, "Invalid name given to addbuttontext" );
//    FillRect( Arg1, Arg2, v20);
//    IntpGetArg( scr, Type1, Arg1 );
//    if( !sub_4BA34C( IntpGetArg( scr, Type2, Arg2 ) ) ) IntpError( "Error setting text to button %s\n", IntpGetArg( scr, Type2, Arg2 ) );
}

void ScpIfc_AddButtonText( Intp_t *scr )
{
    unsigned short Type1, Type2, Type3, Type4;
    int Arg1, Arg2, Arg3, Arg4;
    int r,x;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )

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

    x = IntpGetArg( scr, TYPEH( Type4 ), Arg4 );
    IntpGetArg( scr, TYPEH( Type3 ), Arg3 );
//    v26 = IntpMseHandler();
    IntpGetArg( scr, TYPEH( Type2 ), Arg2 );
//    v22 = IntpMseHandler();
    IntpGetArg( scr, TYPEH( Type1 ), Arg1 );
//    v24 = IntpMseHandler();
//    FillRect(v26, Arg4, Arg2);
//    if( !WindowAddButtonText(x, v25, v24, v22) ) IntpError("Error setting graphics to button %s\n", x);
}

void ScpIfc_AddButtonProc( Intp_t *scr )
{
    unsigned short Type1, Type2, Type3, Type4, Type5;
    int Arg1, Arg2, Arg3, Arg4, Arg5;
    char *n;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )

    ARG_TYPE( Type1, SCR_INT, "Invalid procedure 4 name given to addbuttonproc" );
    ARG_TYPE( Type2, SCR_INT, "Invalid procedure 3 name given to addbuttonproc" );
    ARG_TYPE( Type3, SCR_INT, "Invalid procedure 2 name given to addbuttonproc" );
    ARG_TYPE( Type4, SCR_INT, "Invalid procedure 1 name given to addbuttonproc" );
    ARG_TYPE( Type5, SCR_STRING, "Invalid name given to addbuttonproc" );
    n = IntpGetArg( scr, TYPEH( Type5 ), Arg5 );
//    FillRect( Arg1, Arg2, Arg5 );
//    if( !sub_4BA11C(n, scr, v12, Arg4, Type2, v11) ) IntpError( "Error setting procedures to button %s\n", Arg );
}

void ScpIfc_AddButtonRightProc( Intp_t *scr )
{
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;
    char *n;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    ARG_TYPE( Type1, SCR_INT, "Invalid procedure 2 name given to addbuttonrightproc" );
    ARG_TYPE( Type2, SCR_INT, "Invalid procedure 1 name given to addbuttonrightproc" );
    ARG_TYPE( Type3, SCR_STRING, "Invalid name given to addbuttonrightproc" );
    n = IntpGetArg( scr, TYPEH( Type3 ), Arg3 );
//    FillRect(Arg3, Arg1, Type2);
//    if( !AddButtRt( n, v11, v12, Arg2) ) IntpError( "Error setting right button procedures to button %s\n", n );
}

void ScpIfc_ShowWin( Intp_t *scr )
{
//    FillRect(scr->i34);
//    if( VidTab01[gScrIdx].i00 != -1 ) WinUpdate(VidTab01[gScrIdx].i00);
}

void ScpIfc_DeleteButton( Intp_t *scr )
{
    unsigned short Type1;
    int Arg1;
    char *n;

    GET_ARGi( scr, Arg1, Type1 )
    if( (Type1 & 0xF7FF) == SCR_INT && Arg1 != -1 ) IntpError( "Invalid type given to delete button" );

//    FillRect( scr->i34 );
    if( (Type1 & ~0x800) == SCR_INT ) {
//        if( WindowDeleteButton( 0 ) ) return;
    } else {
        n = IntpGetArg( scr, TYPEH( Type1 ), Arg1 );
//        if( WindowDeleteButton( n ) ) return;
    }
    IntpError( "Error deleting button" );
}

void ScpIfc_FillWin( Intp_t *scr )
{
    unsigned short Type1, Type2, Type3;
    int Arg1, Arg2, Arg3;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )        
    GET_ARGi( scr, Arg3, Type3 )    
    if( (Type3 & 0xF7FF) != SCR_FLOAT ){
        if( (Type3 & 0xF7FF) == SCR_INT ){
            if( Arg3 == 1 ){
                Arg3 = 0x3F800000;
            } else if( Arg3 ){
                IntpError( "Invalid red value given to fillwin" );
            }
        }
    }
    if( (Type2 & 0xF7FF) != SCR_FLOAT && (Type2 & 0xF7FF) == SCR_INT ){
        if( Arg2 == 1 ){
            Arg2 = 0x3F800000;
        } else if( Arg2 ){
            IntpError( "Invalid green value given to fillwin" );
        }
    }
    if( (Type1 & 0xF7FF) != SCR_FLOAT && (Type1 & 0xF7FF) == SCR_INT ){
        if( Arg1 == 1 ){
            Arg1 = 0x3F800000;
        } else if( Arg1 ){
            IntpError( "Invalid blue value given to fillwin" );
        }
    }
//    FillRect( scr->i34 );
//    FillWin( v18, Arg3, Arg2, Arg1 );
}

void ScpIfc_FillRect( Intp_t *scr )
{
    unsigned short Type1,Type2,Type3,Type4,Type5,Type6,Type7;
    int Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7;

    Intp_t *locscr;
    int Bcolor;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )    
    GET_ARGi( scr, Arg3, Type3 )    
    GET_ARGi( scr, Arg4, Type4 )    
    GET_ARGi( scr, Arg5, Type5 )    
    GET_ARGi( scr, Arg6, Type6 )    
    GET_ARGi( scr, Arg7, Type7 )    

    if( (Type3 & 0xF7FF) != SCR_FLOAT ){
        if( (Type3 & 0xF7FF) == SCR_INT ){
            if ( Arg3 == 1 ){
                Arg3 = 0x3F800000;
            } else if ( Arg3 ){
                IntpError("Invalid red value given to fillrect");
            }
        }
    }
    if( (Type2 & 0xF7FF) != SCR_FLOAT ){
        if( (Type2 & 0xF7FF) == SCR_INT ){
            if ( Arg2 == 1 ){
                Arg2 = 0x3F800000;
            } else if( Arg2 ){
                IntpError( "Invalid green value given to fillrect" );
            }
        }
    }
    if( (Type1 & 0xF7FF) != SCR_FLOAT ){
        if( (Type1 & 0xF7FF) == SCR_INT ){
            if ( Bcolor == 1 ) {
                Bcolor = 0x3F800000;
            } else if( Bcolor ) {
                IntpError( "Invalid blue value given to fillrect" );
            }
        }
    }
    ARG_TYPE( Type7, SCR_INT, "Invalid arg 1 given to fillrect" );
    ARG_TYPE( Type6, SCR_INT, "Invalid arg 2 given to fillrect" );
    ARG_TYPE( Type5, SCR_INT, "Invalid arg 3 given to fillrect" );
    ARG_TYPE( Type4, SCR_INT, "Invalid arg 4 given to fillrect" );
//    FillRect( locscr->i34 );
//    Int_FillRect( Arg4, Arg6, Arg3, Arg2, Bcolor );
}

void ScpIfc_MouseRedraw( Intp_t *scr )
{
    MseCursorRedraw();
}

void ScpIfc_MouseDrawCursor()
{
    MseDrawCursor();
}

void ScpIfc_MouseShape( Intp_t *scr )
{
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )    
    GET_ARGi( scr, Arg3, Type3 )    
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 3 given to mouseshape" );
    ARG_TYPE( Type2, SCR_INT, "Invalid arg 2 given to mouseshape" );
    ARG_TYPE( Type3, SCR_STRING, "Invalid filename given to mouseshape" );
    IntpGetArg( scr, TYPEH( Type3 ), Arg3 );
//    if( !MouseMgrUnk02( Arg2, Arg1 ) ) IntpError( "Error loading mouse shape." );
}

void ScpIfc_SetGlobalMouseFunc( Intp_t *scr )
{
    IntpError( "setglobalmousefunc not defined" );
}

void ScpIfc_DisplayGFX( Intp_t *scr )
{
    unsigned short Type1,Type2,Type3,Type4,Type5;
    int Arg1,Arg2,Arg3,Arg4,Arg5;

    GET_ARGi( scr, Arg1, Type1 )    
    GET_ARGi( scr, Arg2, Type2 )    
    GET_ARGi( scr, Arg3, Type3 )    
    GET_ARGi( scr, Arg4, Type4 )    
    GET_ARGi( scr, Arg5, Type5 )    
    IntpGetArg( scr, Type5, Arg5 );
    IntpMseHandler();
//    WinDisplayGFX( Arg3, Arg1 );
}

void ScpIfc_LoadPaletteTable( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to loadpalettetable" );
    if( !PalLoadFromFile( IntpGetArg( scr, TYPEH( Type ), Arg ) ) ) IntpError( PalGetErrorMsg() );    
}

void ScpIfc_AddNamedEvent( Intp_t *scr )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Type1, SCR_STRING, "Invalid type given to addnamedevent" );
    NevsAddProg( IntpGetArg( scr, TYPEH( Type1 ), Arg2 ), scr, Arg1, 0 );
}

void ScpIfc_AddNamedHandler( Intp_t *scr )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Type1, SCR_STRING, "Invalid type given to addnamedhandler" );
    NevsAddProg( IntpGetArg( scr, TYPEH( Type1 ), Arg2 ), scr, Arg1, 1 );
}

void ScpIfc_ClearNamed( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to clearnamed" );
    NevsClearEvents( IntpGetArg(scr, TYPEH( Type ), Arg ) );
}

void ScpIfc_SignalNamed( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_STRING, "Invalid type given to signalnamed" );
    NevsSignal( IntpGetArg( scr, TYPEH( Type ), Arg ) );
}

void ScpIfc_AddKey( Intp_t *scr )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 2 given to addkey" );
    ARG_TYPE( Type2, SCR_INT,  "Invalid arg 1 given to addkey" );
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
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to deletekey" );

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
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to refreshmouse" );
//    if( !RefreshMouse() ){ if( Arg ) SciUnk11( scr, Arg ); }
}

void ScpIfc_SetFont( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    if( (Type & 0xF7FF) != SCR_INT ) IntpError("Invalid arg 1 given to setfont");
//    if( !SetFont( Arg ) ) IntpError( "Error setting font" );
}

void ScpIfc_SetTextFlags( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to setflags" );
//    if( !SetTextFlags( Arg ) ) IntpError( "Error setting text flags" );
}

void ScpIfc_SetTextColor( Intp_t *scr )
{
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;
    float Arg3f, Arg2f, Arg1f;

    GET_ARGf( scr, Arg1, Arg1f, Type1 )
    GET_ARGf( scr, Arg2, Arg2f, Type2 )
    GET_ARGf( scr, Arg3, Arg3f, Type3 )
    if( SCRTYPE( Type3 ) == SCR_INT && Arg3f != 0.0 || 
	SCRTYPE( Type2 ) == SCR_INT && Arg2f != 0.0 || 
	SCRTYPE( Type1 ) == SCR_INT && Arg1f != 0.0 
	) IntpError("Invalid type given to settextcolor" );
//    if( !SetTextColor(Arg3f, Arg2f, Arg1f) ) IntpError("Error setting text color");
}

void ScpIfc_SayOptionColor( Intp_t *scr )
{
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;
    float Arg3f, Arg2f, Arg1f;

    GET_ARGf( scr, Arg1, Arg1f, Type1 )
    GET_ARGf( scr, Arg2, Arg2f, Type2 )
    GET_ARGf( scr, Arg3, Arg3f, Type3 )
    if( SCRTYPE( Type3 ) == SCR_INT && Arg3f != 0.0 || 
	SCRTYPE( Type2 ) == SCR_INT && Arg2f != 0.0 || 
	SCRTYPE( Type1 ) == SCR_INT && Arg1f != 0.0 
	) IntpError( "Invalid type given to sayoptioncolor" );
//    if( SetOptionColor( Arg3f, Arg2f, Arg1f ) ) IntpError( "Error setting option color" );
}

void ScpIfc_SayReplytColor( Intp_t *scr )
{
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;
    float Arg3f, Arg2f, Arg1f;

    GET_ARGf( scr, Arg1, Arg1f, Type1 )
    GET_ARGf( scr, Arg2, Arg2f, Type2 )
    GET_ARGf( scr, Arg3, Arg3f, Type3 )
    if( SCRTYPE( Type3 ) == SCR_INT && Arg3f != 0.0 || 
	SCRTYPE( Type2 ) == SCR_INT && Arg2f != 0.0 || 
	SCRTYPE( Type1 ) == SCR_INT && Arg1f != 0.0 
	) IntpError( "Invalid type given to sayreplycolor" );
//    if( SetRepplyColor(Arg3f, Arg2f, Arg1f) ) IntpError("Error setting reply color");
}

void ScpIfc_SetTextHighlightColor( Intp_t *scr )
{
    unsigned short Type1,Type2,Type3;
    int Arg1,Arg2,Arg3;
    float Arg3f, Arg2f, Arg1f;

    GET_ARGf( scr, Arg1, Arg1f, Type1 )
    GET_ARGf( scr, Arg2, Arg2f, Type2 )
    GET_ARGf( scr, Arg3, Arg3f, Type3 )
    if( SCRTYPE( Type3 ) == SCR_INT && Arg3f != 0.0 || 
	SCRTYPE( Type2 ) == SCR_INT && Arg2f != 0.0 || 
	SCRTYPE( Type1 ) == SCR_INT && Arg1f != 0.0 
	) IntpError( "Invalid type given to sethighlightcolor" );
//    if( !SetTextHlColor( Arg3f, Arg2f, Arg1f ) ) IntpError( "Error setting text highlight color" );
}

void ScpIfc_SayReplyWindow( Intp_t *scr )
{
    unsigned short Type1,Type2,Type3,Type4,Type5;
    int Arg1,Arg2,Arg3,Arg4,Arg5;
    char *s;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )    
    s = NULL;
    if( SCRTYPE( Type1 ) == SCR_STRING ){
        IntpGetArg( scr, TYPEH(Type1), Arg1 );
        s = strdup( IntpMseHandler() );
    } else if( SCRTYPE( Type1 ) != SCR_INT || Arg1 ){
        IntpError( "Invalid arg 5 given to sayreplywindow" );
    }
//    if ( SetReplyWindow( Arg5, Arg4, Arg2, Arg3, s ) ) IntpError( "Error setting reply window" );
}

void ScpIfc_SayReplyFlags( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to sayreplyflags" );
//    if( !SetOptionFlags( Arg ) ) IntpError( "Error setting reply flags" );
}

void ScpIfc_SayOptionFlags( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to sayoptionflags" );
//    if( !SetOptionFlags( Arg ) ) IntpError( "Error setting option flags" );
}

void ScpIfc_SayOptionWindow( Intp_t *scr )
{
    unsigned short Type1,Type2,Type3,Type4,Type5;
    int Arg1,Arg2,Arg3,Arg4,Arg5;
    char *s;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    GET_ARGi( scr, Arg3, Type3 )
    GET_ARGi( scr, Arg4, Type4 )
    GET_ARGi( scr, Arg5, Type5 )
    s = NULL;
    if( SCRTYPE( Type1 ) == SCR_STRING ){
        IntpGetArg( scr, TYPEH(Type1), Arg1 );
        s = strdup( IntpMseHandler() );
    } else if( Type5 != SCR_INT || Arg1 ){
        IntpError( "Invalid arg 5 given to sayoptionwindow" );
    }
//    if( SayOptWin( Arg5, Arg4, Arg2, Arg3, s ) ) IntpError( "Error setting option window" );
}

void ScpIfc_SayBorder( Intp_t *scr )
{
    unsigned short Type1,Type2;
    int Arg1,Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 2 given to sayborder" );
    ARG_TYPE( Type2, SCR_INT, "Invalid arg 1 given to sayborder" );
//    if( SayBorder( Arg2, Arg1 ) ) IntpError( "Error setting dialog border" );
}

void ScpIfc_SayScrollUp( Intp_t *scr )
{
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
    if( SCRTYPE( Type1 ) == SCR_INT ){
        if( Arg1 != -1 && Arg1 ) IntpError( "Invalid arg 4 given to sayscrollup" );
        if( Arg1 == -1 ) Arg1 = 1;
    } else {
        if( SCRTYPE( Type1 ) != SCR_STRING ) IntpError( "Invalid arg 4 given to sayscrollup" );
    }
    if( SCRTYPE( Type2 ) == SCR_INT && Arg2 ) IntpError( "Invalid arg 3 given to sayscrollup" );
    if( SCRTYPE( Type3 ) == SCR_INT && Arg3 ) IntpError( "Invalid arg 2 given to sayscrollup" );
    if( SCRTYPE( Type4 ) == SCR_INT && Arg4 ) IntpError( "Invalid arg 1 given to sayscrollup" );
    if( SCRTYPE( Type4 ) == SCR_STRING ){ IntpGetArg(scr, TYPEH(Type4), Arg4); s1 = strdup( IntpMseHandler() ); }
    if( SCRTYPE( Type3 ) == SCR_STRING ){ IntpGetArg(scr, TYPEH(Type3), Arg3); s2 = strdup( IntpMseHandler() ); }
    if( SCRTYPE( Type2 ) == SCR_STRING ){ IntpGetArg(scr, TYPEH(Type2), Arg2); s3 = strdup( IntpMseHandler() ); }
    if( SCRTYPE( Type1 ) == SCR_STRING ){ IntpGetArg(scr, TYPEH(Type1), Arg1); s4 = strdup( IntpMseHandler() ); }
//    if( DlgScrollUp( Arg6, Arg5, s1, s2, s3, s4, Arg1 ) ) IntpError( "Error setting scroll up" );
}

void ScpIfc_SayScrollDn( Intp_t *scr )
{
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

    if( SCRTYPE( Type1 ) == SCR_INT ){
        if( Arg1 != -1 && Arg1 ) IntpError("Invalid arg 4 given to sayscrolldn");
        if( Arg1 == -1 ) Arg1 = 1;
    } else {
        if( (Type1 & 0xF7FF) != SCR_STRING ) IntpError( "Invalid arg 4 given to sayscrolldn" );
    }
    if( SCRTYPE( Type2 ) == SCR_INT && Arg2 ) IntpError( "Invalid arg 3 given to sayscrolldown" );
    if( SCRTYPE( Type3 ) == SCR_INT && Arg3 ) IntpError( "Invalid arg 2 given to sayscrolldown" );
    if( SCRTYPE( Type4 ) == SCR_INT && Arg4 ) IntpError( "Invalid arg 1 given to sayscrolldown" );
    if( SCRTYPE( Type4 ) == SCR_STRING ){ IntpGetArg( scr, TYPEH( Type4 ), Arg4 ); s1 = strdup( IntpMseHandler() ); }
    if( SCRTYPE( Type3 ) == SCR_STRING ){ IntpGetArg( scr, TYPEH( Type3 ), Arg3 ); s2 = strdup( IntpMseHandler() ); }
    if( SCRTYPE( Type2 ) == SCR_STRING ){ IntpGetArg( scr, TYPEH( Type2 ), Arg2 ); s3 = strdup( IntpMseHandler() ); }
    if( SCRTYPE( Type1 ) == SCR_STRING ){ IntpGetArg( scr, TYPEH( Type1 ), Arg1 ); s4 = strdup( IntpMseHandler() ); }
//    if( DlgScrollDn( Arg6, Arg5, s1, s2, s3, s4, Arg1 ) )  IntpError("Error setting scroll down");
}

void ScpIfc_SaySetSpacing( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to saysetspacing" );
//    if( SetOptSpacing( Arg ) ) IntpError( "Error setting option spacing" );
}

void ScpIfc_Restart( Intp_t *scr )
{
//    if ( RestartOpt() ) IntpError( "Error restarting option" );
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

int ScpIfcDeleteSounds()
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
    SoundSetCallback( snd, ScpIfcSoundCb, &gScpIfcSounds[ i ] );
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
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    GET_ARGi( scr, Arg1, Type1 )
    GET_ARGi( scr, Arg2, Type2 )
    ARG_TYPE( Type1, SCR_INT, "Invalid arg 2 given to soundplay" );
    ARG_TYPE( Type2, SCR_STRING,  "Invalid arg 1 given to soundplay" );
    IntpGetArg( scr, TYPEH( Type2 ), Arg2 );
    IntpPushiA( scr, ScpIfcSounPlay( Arg1, IntpMseHandler() ) );
    IntpPushwA( scr, SCR_INT );
}

void ScpIfc_SoundPause( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to soundpause" );
    ScpIfcSoundPause( Arg );
}

void ScpIfc_SoundResume( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to soundresume" );
    ScpIfcSounResume( Arg );
}

void ScpIfc_SoundStop( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to soundstop" );
    ScpIfcSoundPause(Arg);
}

void ScpIfc_SoundRewind( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to soundrewind" );
    ScpIfcSoundRewind( Arg );
}

void ScpIfc_SoundDelete( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "Invalid arg 1 given to sounddelete" );
    ScpIfcSoundDelete( Arg );
}

void ScpIfc_SetOneOptPause( Intp_t *scr )
{
    unsigned short Type;
    int Arg;

    GET_ARGi( scr, Arg, Type )
    ARG_TYPE( Type, SCR_INT, "SetOneOptPause: invalid arg passed (non-integer)." );
    if( Arg ){
//        if( (SetOneOptPause1() & 8) == 0 ) return;
//    } else if( (SetOneOptPause1() & 8) != 0 ){
//        return;
    }
//    SetOneOptPause2(8);
}

int ScpIfcNevsUpdate()
{
    NevsUpdate();
    return 0;
}

int ScpUnk02( unsigned int a1 )
{
    unsigned int v2;

    if( a1 > 0xFF ) return 0;
    if( gScpIfcUnk08 ){
//        if( gScpIfcUnk07 ) SciUnk11(gScpIfcUnk08, gScpIfcUnk07);
        return 1;
    }
    if( !gIntpList[ a1 ].scr ) return 0;
//    if( gIntpList[ a1 ].i02 ) SciUnk11( gIntpList[ a1 ].scr, gIntpList[ a1 ].i02 );
    return 1;
}

void ScpIfcInit()
{
//    ScpFillWin3x3( ScpUnk02 );
    SciAddOpcode( 0x806A, ScpIfc_FillWin3x3 );
    SciAddOpcode( 0x808C, ScpIfc_DeleteButton );
    SciAddOpcode( 0x8086, ScpIfc_AddButton );
    SciAddOpcode( 0x8088, ScpIfc_AddButtonFlag );
    SciAddOpcode( 0x8087, ScpIfc_AddButtonTextString );
    SciAddOpcode( 0x8089, ScpIfc_AddButtonText );
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
//    ScpSetupGameCmdC(  );
}

