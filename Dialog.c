#include "FrameWork.h"

int gSayModeLvl = -1;
int gSayUnk100 = 0;
int gSayUnk101 = 0;
char *gSayUnk102 = "fullscreen";
int (*gSayUnk33)(void) = NULL;
int (*gSayUnk32)(void) = NULL;
int gSayWinHorizontal = 7;
int gSayWinVertical = 7;
int gSayOptSpacing = 5;
int gSayReplyColors = 0;
int gSayOptColors = 0;
int gSayUnk01 = 0;
int gSayUnk02 = 0;
int gSayOptStat = 2;

Dialog_t gSayDialog[ 4 ];
char gSayUnk110;
short gSayOptFlags;
int gSayOptWinX1;
int gSayOptWinY1;
int gSayOptWinX0;
int gSayOptWinY0;
char *gSayOptWinTitle;
short gSayUnk111;
int gSayReplyWinX1;
int gSayReplyWinY1;
int gSayReplyWinX0;
int gSayReplyWinY0;
char *gSayReplyWinTitle;
int gSayUnk112;
int gSayUnk113;

int gSayReplyColorG;
int gSayReplyColorB;
int gSayOptColorG;
int gSayReplyColorR;
int gSayOptColorB;
int gSayOptColorR;
int gSayUnk114;
int gSayUnk115;
int gSayUnk116;
void *gSayUnk117;
void *gSayUnk118;
void *gSayUnk119;
void *gSayUnk120;
char *gSayTitle;
int gSayUnk16;
int gSayUnk15;
int gSayUnk21;
void *gSayUnk17;
void *gSayUnk18;
void *gSayUnk19;
void *gSayUnk20;
int gSayUnk122;
int (*gSayUnk123)(int, int);
int gSayUnk124;

/***************************************/

void DialogUnk01( char *a1, char *a2 )
{
    int Cnt1;
    char *s;

    Cnt1 = gSayDialog[ gSayModeLvl ].Cnt1;
    gSayDialog[ gSayModeLvl ].Cnt2 = Cnt1;
    if( gSayDialog[ gSayModeLvl ].Dlgs ){
        gSayDialog[ gSayModeLvl ].Cnt1++;
        gSayDialog[ gSayModeLvl ].Dlgs = dbg_realloc( gSayDialog[ gSayModeLvl ].Dlgs, sizeof( Dialog01_t ) * (Cnt1 + 1) );
    } else {
        gSayDialog[ gSayModeLvl ].Dlgs = dbg_malloc( sizeof( Dialog01_t ) );
        gSayDialog[ gSayModeLvl ].Cnt1 = 1;
    }
    if( a1 ){
        s = dbg_malloc( strlen( a1 ) + 1 );
        strcpy( s, a1 );
        gSayDialog[ gSayModeLvl ].Dlgs[ Cnt1 ].Str2 = s;
    } else {
        gSayDialog[ gSayModeLvl ].Dlgs[ Cnt1 ].Str2 = NULL;
        gSayDialog[ gSayModeLvl ].Cnt3 = Cnt1;
    }
    gSayDialog[ gSayModeLvl ].Dlgs[ Cnt1 ].Str3 = NULL;
    if( a2 ){
        s = dbg_malloc( strlen( a2 ) + 1 );
        strcpy( s, a2 );
        gSayDialog[ gSayModeLvl ].Dlgs[ Cnt1 ].Str3 = s;
    }
    gSayDialog[ gSayModeLvl ].Dlgs[ Cnt1 ].Option = NULL;
    gSayDialog[ gSayModeLvl ].Dlgs[ Cnt1 ].Cnt = 0;
    gSayDialog[ gSayModeLvl ].Dlgs[ Cnt1 ].i05 = 0;
    gSayDialog[ gSayModeLvl ].Dlgs[ Cnt1 ].Font = WinGetFont();
    gSayDialog[ gSayModeLvl ].Dlgs[ Cnt1 ].w01 = gSayUnk111;
    gSayDialog[ gSayModeLvl ].Dlgs[ Cnt1 ].Str1 = NULL;
    if( gSayTitle ){
        s = dbg_malloc( strlen( gSayTitle ) + 1 );
        strcpy( s, gSayTitle );
	gSayDialog[ gSayModeLvl ].Dlgs[ Cnt1 ].Str1 = s;
    }
}

Dialog01_t *DialogUnk02()
{
    Dialog01_t *p;

    p = &gSayDialog[ gSayModeLvl ].Dlgs[ gSayDialog[ gSayModeLvl ].Cnt2 ];
    if( p->Option ){
        p->Cnt++;
        p->Option = dbg_realloc( p->Option, p->Cnt * sizeof( Dialog02_t ) );
    } else {
        p->Option = dbg_malloc( sizeof( Dialog02_t ) );
        p->Cnt = 1;
    }
    return p;
}

void DialogUnk03( char *a1, char *a2, int a3 )
{
    Dialog01_t *p;
    char *s;
    int idx;

    p = DialogUnk02();
    idx = p->Cnt - 1;
    p->Option[ idx ].Flags = 2;
    if( a1 ){
        s = dbg_malloc( strlen( a1 ) + 1 );
        strcpy( s, a1 );
        p->Option[ idx ].Str1 = s;
    } else {
        p->Option[ idx ].Str1 = NULL;
    }
    if( a2 ){
        s = dbg_malloc( strlen( a2 ) + 1 );
        strcpy( s, a2 );
        p->Option[ idx ].Str2 = s;
    } else {
        p->Option[ idx ].Str2 = NULL;
    }
    p->Option[ idx ].Font = WinGetFont();
    p->Option[ idx ].w01 = gSayOptFlags;
    p->Option[ idx ].Cnt = a3;
}

void DialogUnk04( char *a1, int Val, int a3 )
{
    Dialog01_t *p;
    char *s;
    int idx;

    p = DialogUnk02();
    idx = p->Cnt - 1;
    p->Option[ idx ].Flags = 1;
    if( a1 ){
        s = dbg_malloc( strlen( a1 ) + 1 );
        strcpy( s, a1 );
        p->Option[ idx ].Str1 = s;
    } else {
        p->Option[ idx ].Str1 = NULL;
    }
    p->Option[ idx ].iVal = Val;
    p->Option[ idx ].Font = WinGetFont();
    p->Option[ idx ].w01 = gSayOptFlags;
    p->Option[ idx ].Cnt = a3;
}

void DialogUnk10( Dialog02_t *p )
{
    if( p->Str1 ) dbg_free( p->Str1 );
    if( p->Flags == 2 ){
        if( p->Str2 ) dbg_free( p->Str2 );
    }
}

void DialogUnk11()
{
    int i, j;
    Dialog01_t *p;

    for( i = 0; i < gSayDialog[ gSayModeLvl ].Cnt1; i++ ){
        if( gSayDialog[ gSayModeLvl ].Dlgs[ i ].Option ){            
            for( j = 0; j < p->Cnt; j++ ){
                p = &gSayDialog[ gSayModeLvl ].Dlgs[ i ];
                DialogUnk10( &p->Option[ j ] );
            }
            dbg_free( p->Option );
        }
        p = &gSayDialog[ gSayModeLvl ].Dlgs[ i ];
        if( p->Str3 ) dbg_free( p->Str3 );
        p = &gSayDialog[ gSayModeLvl ].Dlgs[ i ];
        if( p->Str2 ) dbg_free( p->Str2 );
        p = &gSayDialog[ gSayModeLvl ].Dlgs[ i ];
        if( p->Str1 ) dbg_free( p->Str1 );        
    }
    if( gSayDialog[ gSayModeLvl ].Dlgs ) dbg_free( gSayDialog[ gSayModeLvl ].Dlgs );
}

int DialogUnk12( int x, int y, int w, int h, int Color, char *fname )
{
    int win, pW, pH;
    char *data;

    win = WinCreateWindow( x, y, w, h, Color, 0x25 );
    if( !fname ){
        WinDrawFilledRect( win, 0, 0, w, h, Color );
        if( !gSayReplyColors ) WinDrawFrame( win );
        return win;
    }
    if( (data = DataFileGetArt( fname, &pW, &pH )) ){
        WinBlit( data, pW, pH, WinGetSurface( win ), WinGetWidth( win ), WinGetHeight( win ) );
        dbg_free( data );
        return win;
    }
    WinDrawFilledRect( win, 0, 0, w, h, Color );
    if( !gSayReplyColors ) WinDrawFrame( win );
    return win;
}

int DialogUnk13( char *a1, char *edx0, int a3, int a4, int a5 )
{
    int DisplResW,v7,v9,v13,v14,color,v16,a6,v25,v27;

    DisplResW = WinGetScreenWidth();
    v9 = WinGetScreenHeight();
    v7 = v25 = 0;
    a6 = WinGetTextColor() + WinGetTextFlags();
    if( !edx0 && !a1 ) return 0;
    v27 = ( gSayReplyWinX1 ) ? gSayReplyWinX1 : (300 * DisplResW / 640);
    if( gSayReplyWinY1 ){
        v7 = gSayReplyWinY1;
        if( a1 ) v25 = 2;
    } else {
        if( edx0 ) v7 = WinUnk97( edx0, v27 - 2 * gSayWinHorizontal, gSayReplyWinY1 ) * gFont.ChrHeight() + 2 * gSayWinVertical;
        if( a1 ){
            v25 = 2;
            v7 += 2 * gFont.ChrHeight();
        }
    }
    if( !v7 ) return 0;
    if( gSayReplyWinY0 ){
        v13 = gSayReplyWinY0;
    } else if( a3 == -1 ){
        v13 = v9 / 2 - v7 / 2;
        if( v13 < 0 ) v13 = 0;
    } else {
        v13 = v9 * a3 / 480;
    }
    if( gSayReplyWinX0 ){
        v14 = gSayReplyWinX0;
    } else {
        v14 = DisplResW / 2 - v27 / 2;
        if( v14 < 0 ) v14 = 0;
    }
    if( gSayReplyColors )
        color = gPalColorCubeRGB[0][0][ gSayReplyColorB | (gSayReplyColorR << 10) | (32 * gSayReplyColorG) ];
    else
        color = gPalColorCubeRGB[8][8][8];
    v16 = DialogUnk12( v14, v13, v27, v7, color, gSayReplyWinTitle );
    if( gSayUnk33 ) gSayUnk33();
    if( a1 ) WinDrawText( v16, a1, 0, ( v27 - gFont.LineWidth( a1 ) ) / 2, gSayWinVertical, a6 );
    WinPrintMessage( v16, edx0, v7, v27 - 2 * gSayWinHorizontal, gSayWinHorizontal, gSayWinVertical + v25 * gFont.ChrHeight(), a6, a5 );
    if( a4 ){
        WinMoveTop( v16 );
        WinUpdate( v16 );
    }
    return v16;
}

int DialogUnk14()
{
    if( gSayModeLvl == -1 ) return gSayModeLvl;
    gSayUnk101 = gSayDialog[ gSayModeLvl ].Cnt3;
    DialogUnk11();
    if( gSayTitle ){ dbg_free( gSayTitle ); gSayTitle = NULL; }
    gSayModeLvl--;
    return 0;
}

char *DialogUnk15( char *a1, int a2, int a3 )
{
    int v5,v6,v7;
    char *s;

    v5 = strlen( a1 );
    v7 = gFont.Distance();
    for( ; a3 > 0; a3 -= v7 + v6 ){
        if( v5 <= 0 ) break;
        v6 = gFont.ChrWidth( a1[ v5 ] );
    }
    for( s = &a1[ v5 ]; v5 && *s != a2; s-- ) v5--;    
    return &a1[ v5 ];
}

void DialogUnk16( char *a1, int a2, int *a3, int *a4 )
{
    WinTextWrapCreate( a1, a2, 0, a3 );
    *a4 = *a3;
}

void DialogUnk17( int WinId, char **Text, int Lines, int FieldSize, int h, int x, int y, int Flags, int Wrap )
{
    int i;

    for( i = 0; i < Lines; i++ ){
        WinPrintText( WinId, Text[ i ], strlen( Text[ i ] ), FieldSize, h + y, x, y + i * gFont.ChrHeight(), Flags, Wrap );
    }
}

void DialogUnk18( int a1, char *a2, int ecx0, int a4, int posX, int a7, int Flags, int Wrap, int a9 )
{
    char **Text;
    int a3;

    Text = WinTextWrapCreate( a2, ecx0, 0, &a3 );
    DialogUnk17( a1, Text, a3, ecx0, a4, posX, a7, Flags, Wrap );
    WinTextWrapDestroy( Text, a3 );
}

int DialogUnk19( int Win, int a2, int WinW, int WinH, int a5, int Flags )
{
    char *Art, **Lines;
    int Cnt3,hy,ArtH,ArtW,LinesCnt,Font,i,DisplLines,Height;
    
    Cnt3 = gSayDialog[gSayModeLvl].Cnt3;
    if( gSayOptWinTitle ){        
        if( (Art = DataFileGetArt( gSayOptWinTitle, &ArtW, &ArtH )) ){
            WinBlit( Art, ArtW, ArtH, WinGetSurface( Win ), WinGetWidth( Win ), WinGetHeight( Win ) );
            dbg_free( Art );
        } else {
    	    if( gSayOptColors ){
        	WinDrawFilledRect( Win, 0, 0, WinW, WinH, gPalColorCubeRGB[ gSayOptColorR ][ gSayOptColorG ][ gSayOptColorB ] );
    	    } else {
    		WinDrawFilledRect( Win, 0, 0, WinW, WinH, gPalColorCubeRGB[8][8][8] );
    		WinDrawFrame( Win );
    	    }
        }
    } else {
        if( gSayOptColors ){
            WinDrawFilledRect( Win, 0, 0, WinW, WinH, gPalColorCubeRGB[ gSayOptColorR ][ gSayOptColorG ][ gSayOptColorB ] );
        } else {
    	    WinDrawFilledRect( Win, 0, 0, WinW, WinH, gPalColorCubeRGB[8][8][8] );
    	    WinDrawFrame( Win );
        }
    }
    if( gSayUnk32 ) gSayUnk32();
    Font = WinGetFont();
    DisplLines = 0;
    hy = gSayOptSpacing;
    for( i = a2; i < a5; i++ ){
        WinSetFont( gSayDialog[ gSayModeLvl ].Dlgs[ Cnt3 ].Option[ i ].Font );
        Lines = WinTextWrapCreate( gSayDialog[ gSayModeLvl ].Dlgs[ Cnt3 ].Option[ i ].Str1, WinW - 2 * gSayWinHorizontal, 0, &LinesCnt );
        Height = gFont.ChrHeight() * LinesCnt;
        if( gSayWinVertical + Height <= (WinH - gSayWinVertical) ){
            DialogUnk17( Win, Lines, LinesCnt, WinW - 2 * gSayWinHorizontal, WinH - 2 * gSayWinVertical, gSayWinHorizontal, gSayWinVertical, Flags, gSayDialog[ gSayModeLvl ].Dlgs[ Cnt3 ].Option[ i ].w01 );
            DisplLines++;
        }
        WinTextWrapDestroy( Lines, LinesCnt );
        gSayDialog[ gSayModeLvl ].Dlgs[ Cnt3 ].Option[ i ].i04 = gSayWinVertical;
        gSayDialog[ gSayModeLvl ].Dlgs[ Cnt3 ].Option[ i ].i05 = Height;        
        hy += Height + gSayOptSpacing;
    }
    WinSetFont( Font );
    return DisplLines;
}

void DialogUnk20( int a1 )
{
    Dialog01_t *v1;

    gSayUnk113 = -1;
    gSayUnk112 = 0;
    if( !gSayUnk124 && !gSayUnk122 ){
        v1 = &gSayDialog[gSayModeLvl].Dlgs[a1];
        gSayUnk113 = DialogUnk13(v1->Str1, v1->Str3, 50, 0, v1->w01);
        WinMoveTop(gSayUnk113);
        WinUpdate(gSayUnk113);
        MouseMgrCursorUpdate();
        MouseMgrDrawCursor();
    }
}

int DialogUnk21( int a1 )
{
    int v2, right;

    if( gSayUnk112 == 2 ) return MveIsPlaying() == 0;
    if( gSayUnk112 == 3 ) return 1;
    if( !a1 ) return 1;
    if( gSayUnk113 == -1 ) return 1;
    if( (MseGetButtons() & 0x10) != 0 ){
        MseGetCursorPosition( &right, &v2 );
        if( WinGetPointedWidgetId( right, v2 ) == gSayUnk113 ) return 1;
    }
    return 0;
}

void DialogUnk22()
{
    if( gSayUnk112 == 2 ) return;
    if( gSayUnk112 == 1 ){
        if( !(gSayOptStat & 0x02) && gSayUnk113 != -1 ){
            WinClose( gSayUnk113 );
            gSayUnk113 = -1;
        }
    } else if( gSayUnk112 != 3 && gSayUnk113 != -1 ){
        WinClose( gSayUnk113 );
        gSayUnk113 = -1;
    }
}

void DialogUnk23( int WinId, char *Name, int FontId, int w, int h, int x, int y, int Wrap, int a9, int a9a )
{
    int SaveFont;
    VidRect_t Area;

    SaveFont = WinGetFont();
    WinSetFont( FontId );
    DialogUnk18( WinId, Name, w, h, x, y, Wrap, a9, a9a );
    Area.lt = x;
    Area.tp = y;
    Area.rt = x + w;
    Area.bm = y + h;
    WinAreaUpdate( WinId, &Area );
    WinSetFont( SaveFont );
}

int DialogUnk24( int eax0 )
{
    Dialog02_t *p;
    int Cnt3,Font,v11,i,DisplResW,ScreenHight,v34,sel,PointedWidgetId,v38,v55,MseY,MseX,v60,dx,dy,v64,a3;
    int ColorA,zz,Cnt,v70,win;

    v64 = v70 = gSayUnk113 = -1;
    Cnt3 = gSayDialog[ gSayModeLvl ].Cnt3;
    Cnt = gSayDialog[ gSayModeLvl ].Dlgs[ Cnt3 ].Cnt;
    gSayUnk100 = ( Cnt ) ? eax0 : -1;
    DialogUnk20( Cnt3 );
    if( Cnt <= 1 ){
        if( gSayUnk122 ) return gSayUnk100;
        eprintf( "reply has only one option, waiting for dialog to end\n" );
        do
            WinDlgProcess();
        while( !gSayUnk01 && !DialogUnk21(1) );
        eprintf( "Ended\n" );
        while( WinDlgProcess() != -1 );
        DialogUnk22();
        gSayUnk01 = 0;
        if( Cnt ) gSayDialog[gSayModeLvl].Dlgs[ Cnt3 ].i05 = gSayUnk100;
        return gSayUnk100;
    }
    WinDlgProcess();
    eprintf( "waiting for reply to end\n" );
    while( !gSayUnk01 ){
        if( DialogUnk21( Cnt <= 1 ) ) break;
        WinDlgProcess();
    }
    eprintf( "reply ended\n" );
    zz = ( gSayOptWinX1 ) ? gSayOptWinX1 : ( 300 * WinGetScreenWidth() / 640 );
    Font = WinGetFont();
    v11 = gSayWinVertical;
    for( i = 0; i < Cnt; i++ ){
        WinSetFont( gSayDialog[ gSayModeLvl ].Dlgs[ Cnt3 ].Option[ i ].Font );
        v11 += gSayOptSpacing + WinUnk97( gSayDialog[ gSayModeLvl ].Dlgs[ Cnt3 ].Option[ i ].Str1, zz - 2 * gSayWinHorizontal, 0 ) * gFont.ChrHeight();        
    }
    WinSetFont( Font );
    a3 = ( gSayOptWinY1 ) ? gSayOptWinY1 : (gSayWinVertical - gSayOptSpacing + v11);
    dx = gSayOptWinX0;
    if( !gSayOptWinX0 ){
        DisplResW = WinGetScreenWidth();
        dx = (DisplResW - zz) / 2;
    }
    dy = ( gSayOptWinY0 ) ? gSayOptWinY0 : (320 * WinGetScreenHeight() / 480);
    ScreenHight = WinGetScreenHeight();
    if( ( dy + a3 ) > ScreenHight ) dy = WinGetScreenHeight() - a3;
    ColorA = WinGetTextFlags() + WinGetTextColor();    
    v60 = WinGetTextFlags() + WinGetColorHL();
    if( gSayOptColors )
        win = DialogUnk12( dx, dy, zz, a3, gPalColorCubeRGB[ gSayOptColorR ][ gSayOptColorG ] [gSayOptColorB ], gSayOptWinTitle );
    else
        win = DialogUnk12( dx, dy, zz, a3, gPalColorCubeRGB[8][8][8], gSayOptWinTitle );
    WinCreateButton( win, 0, 0, zz, a3, -1, -1, -1, 256, 0, 0, 0, 0 );
    v55 = DialogUnk19( win, gSayUnk100, zz, a3, Cnt, ColorA );
    WinMoveTop( win );
    WinUpdate( win );
    MouseMgrCursorUpdate();
    MouseMgrDrawCursor();
    v34 = Cnt3;
    while( v64 == -1 ){
        sel = WinDlgProcess();
        if( gSayUnk01 || sel == KEY_ESC ){ v64 = -1; break; }
        MseGetCursorPosition( &MseX, &MseY );
        PointedWidgetId = WinGetPointedWidgetId( MseX, MseY );
        if( PointedWidgetId != win && v70 != -1 ){
            p = &gSayDialog[ gSayModeLvl ].Dlgs[ v34 ].Option[ v70 ];
            DialogUnk23( win, p->Str1, p->Font, zz - 2 * gSayWinHorizontal, p->i05, gSayWinHorizontal, p->i04, ColorA, p->w01, 1 );
            v70 = -1;
        }
        MseX -= dx;
        MseY -= dy;
        if( PointedWidgetId == win ){
	    v38 = gSayWinVertical;
    	    for( i = gSayUnk100; i < Cnt; i++ ){
            	p = &gSayDialog[ gSayModeLvl ].Dlgs[ v34 ].Option[ i ];
            	if( (p->i05 + v38) > (a3 - gSayWinVertical) ){ i = Cnt; break; }
            	v38 += gSayOptSpacing + p->i05;
            	if( p->i04 <= MseY && p->i04 + p->i05 >= MseY ) break;                    
    	    }
    	    if( i == Cnt ){
        	if( v70 != -1 ){
            	    p = &gSayDialog[ gSayModeLvl ].Dlgs[ v34 ].Option[ v70 ];
            	    v70 = -1;
            	    DialogUnk23( win, p->Str1, p->Font, zz - 2 * gSayWinHorizontal, p->i05, gSayWinHorizontal, p->i04, ColorA, p->w01, 1 );
        	}
		continue;
    	    }
    	    if( i != v70 ){
            	if( v70 != -1 ){
            	    p = &gSayDialog[ gSayModeLvl ].Dlgs[ v34 ].Option[ v70 ];
            	    DialogUnk23( win, p->Str1, p->Font, zz - 2 * gSayWinHorizontal, p->i05, gSayWinHorizontal, p->i04, ColorA, p->w01, 1 );
            	}
            	v70 = i;
            	p = &gSayDialog[ gSayModeLvl ].Dlgs[ v34 ].Option[ i ];
            	DialogUnk23( win, p->Str1, p->Font, zz - 2 * gSayWinHorizontal, p->i05, gSayWinHorizontal, p->i04, v60, p->w01, 1 );
    	    }
        }
    	switch( sel ){
    	    case 256: 
    		v64 = i; 
    		while( !gSayUnk01 && WinDlgProcess() != -1 )
    		break;
    	    case 512:
        	if( --gSayUnk100 < 0 ) gSayUnk100 = 0;
        	v55 = DialogUnk19( win, gSayUnk100, zz, a3, Cnt, ColorA );
        	v70 = -1;
        	WinMoveTop( win );
        	WinUpdate( win );
    		break;
    	    case 1024:
        	if( (gSayUnk100 + 1 + v55) > Cnt ) break;
        	gSayUnk100++;
        	if( (gSayUnk100 + 1) >= Cnt ) gSayUnk100 = Cnt;
        	v55 = DialogUnk19( win, gSayUnk100, zz, a3, Cnt, ColorA );
        	v70 = -1;
        	WinMoveTop( win );
        	WinUpdate( win );
        	break;
    	}
    }
    DialogUnk22();
    WinClose( win );
    gSayUnk01 = 0;
    gSayDialog[ gSayModeLvl ].Dlgs[ Cnt3 ].i05 = gSayUnk100;
    gSayUnk110 = 0;
    while( WinDlgProcess() != -1 );
    return v64;
}

int DialogUnk25()
{
    return 0;
}

int DialogUnk26( char *a1, char *a2 )
{
    char stmp[28];

    if( !*a2 ) return 0;
    strcpy( stmp, a1 );
    strcpy( &stmp[strlen(stmp)], a2 + 5 );
    return 0;
}

int DialogUnk27()
{
    int Cnt3, n, Font;

    Cnt3 = gSayDialog[ gSayModeLvl ].Cnt3;
    gSayUnk124 = 0;
    gSayUnk122 = 0;
    if( Cnt3 < 0 ) return -1;
    Font = WinGetFont();
    WinSetFont( gSayDialog[ gSayModeLvl ].Dlgs[ Cnt3 ].Font );
    n = DialogUnk24( gSayDialog[ gSayModeLvl ].Dlgs[ Cnt3 ].i05 );
    WinSetFont( Font );
    return n;
}

int DialogUnk28( int a1 )
{
    int Flags,i, v12;
    char *Str2;
    Dialog01_t *v7;    

    Flags = gSayDialog[ gSayModeLvl ].Dlgs[ gSayDialog[ gSayModeLvl ].Cnt3 ].Option[ a1 ].Flags;
    if( gSayUnk123 ) gSayUnk123( a1, a1 );
    if( !Flags ) return 1;
    if( Flags > 1 ){
        if( Flags == 2 ){            
            if( (Str2 = gSayDialog[ gSayModeLvl ].Dlgs[ gSayDialog[ gSayModeLvl ].Cnt3 ].Option[ a1 ].Str2) ){
                for( i = 0; i < gSayDialog[ gSayModeLvl ].Cnt1; i++ ){
                    v7 = &gSayDialog[ gSayModeLvl ].Dlgs[ i ];
                    if( v7->Str2 && !strcasecmp( v7->Str2, Str2 ) ){ gSayDialog[ gSayModeLvl ].Cnt3 = i; return 0; }                    
                }
            }
        }
        return 1;
    }
    gSayDialog[ gSayModeLvl ].i07++;
    v12 = gSayDialog[ gSayModeLvl ].Cnt3;
    SciRunProcedure( gSayDialog[ gSayModeLvl ].Itp, gSayDialog[ gSayModeLvl ].Dlgs[ gSayDialog[ gSayModeLvl ].Cnt3 ].Option[ a1 ].iVal );
    gSayDialog[ gSayModeLvl ].i07--;
    return v12 == gSayDialog[ gSayModeLvl ].Cnt3;
}

int DialogUnk29( int a1 )
{
    int v1, v2;

    gSayUnk02++;
    while( gSayModeLvl != -1 ){
        if( (v2 = DialogUnk27()) == v1 ) break;
        v1 = DialogUnk28(v2);
        if( v1 ) break;
    }
    gSayUnk02--;
    return v1;
}

int DialogStart( Intp_t *itp )
{
    if( gSayModeLvl == 3 ) return 1;
    gSayModeLvl++;
    gSayDialog[ gSayModeLvl ].Itp = itp;
    gSayDialog[ gSayModeLvl ].Cnt2 = -1;
    gSayDialog[ gSayModeLvl ].Cnt3 = -1;
    gSayDialog[ gSayModeLvl ].i06 = 1;
    gSayDialog[ gSayModeLvl ].i07 = 1;
    gSayDialog[ gSayModeLvl ].Dlgs = 0;
    gSayDialog[ gSayModeLvl ].Cnt1 = 0;
    gSayUnk100 = 0;
    gSayUnk101 = 0;
    return 0;
}

int DialogRestartOpt()
{
    if( gSayModeLvl == -1 ) return 1;
    gSayDialog[ gSayModeLvl ].Cnt3 = 0;
    return 0;
}

int DialogGotoReply( char *NodeName )
{
    int i;
    Dialog01_t *v5;

    if( gSayModeLvl == -1 ) return 1;
    if( NodeName ){
        for( i = 0; i < gSayDialog[ gSayModeLvl ].Cnt1; i++ ){
            v5 = &gSayDialog[ gSayModeLvl ].Dlgs[ i ];
            if( v5->Str2 && !strcasecmp( v5->Str2, NodeName ) ){
        	gSayDialog[ gSayModeLvl ].Cnt3 = i;
                return 0;
            }
        }
        return 1;
    }
    if( gSayModeLvl == -1 ) return 1;
    gSayDialog[ gSayModeLvl ].Cnt3 = 0;
    return 0;
}

int DialogSetTitle( char *Title )
{
    if( gSayTitle ) dbg_free( gSayTitle );
    if( Title ){
        gSayTitle = dbg_malloc( strlen( Title ) + 1 );
        strcpy( gSayTitle, Title );
    } else {
        gSayTitle = NULL;
    }
    return 0;
}

int DialogReply( char *NodeName, char *Text )
{
    DialogUnk01( NodeName, Text );
    return 0;
}

int DialogSetOptionStr( char *Label, char *NodeName )
{
    if( gSayDialog[ gSayModeLvl ].Cnt2 == -1 ) return 1;
    DialogUnk03( Label, NodeName, 0 );
    return 0;
}

int DialogSetOptionInt( char *Label, int NodeId )
{
    if( gSayDialog[ gSayModeLvl ].Cnt2 == -1 ) return 1;
    DialogUnk04( Label, NodeId, 0 );
    return 0;
}

int DialogSetOptionStrA( char *Label, char *NodeName, int a3 )
{
    if( gSayDialog[ gSayModeLvl ].Cnt2 == -1 ) return 1;
    DialogUnk03( Label, NodeName, a3 );
    return 0;
}

int DialogSetOptionIntA( char *Label, int NodeId, int a3 )
{
    if( gSayDialog[ gSayModeLvl ].Cnt2 == -1 ) return 1;
    DialogUnk04( Label, NodeId, a3 );
    return 0;
}

int DialogMsg( char *a1, char *a2, int Time )
{
    int SysTime,v7,v10,right;
    short v4;
    int WinId;

    WinGetUnk01(1);
    SysTime = TimerGetSysTime();
    v4 = 0;
    WinId = DialogUnk13( a1, a2, -1, 1, gSayUnk111 );
    if( WinId ){
        v7 = 1000;
        while( 1 ){
            WinDlgProcess();
            if( a2 ){
                if( (TimerGetSysTime() - SysTime) / v7 > Time ){
    		    v4 = 1;
            	    break;
                }
            }
            if( (MseGetButtons() & 0x10) != 0 ){
                MseGetCursorPosition( &right, &v10 );
                if( WinGetPointedWidgetId( right, v10 ) == WinId ) break;
            }
        }
        while( WinDlgProcess() != -1 && !v4 );
        WinClose( WinId );
    }
    WinGetUnk01( 0 );
    return 0;
}

int DialogEnd( int Pid )
{
    int IsCursorClear, v5, Cnt, v8;

    IsCursorClear = MseIsCursorClear();
    MouseMgrRedraw();
    if( gSayDialog[ gSayModeLvl ].i07 != 1 ){ TextBoxUnk03( "Error, can't call sayEnd inside of option target procedure" ); return -1; }
    if( Pid != -1 ){
        v5 = (Pid >> 24) & 0xff;
        if( v5 >= gSayDialog[gSayModeLvl].Cnt1 ){ DialogUnk14(); return -1; }        
        if( (Cnt = gSayDialog[ gSayModeLvl ].Dlgs[ v5 ].Cnt) ){
            if( Pid >= Cnt ){ DialogUnk14(); return -1; }
        }
        gSayDialog[ gSayModeLvl ].Cnt3 = v5;
        gSayDialog[ gSayModeLvl ].Dlgs[v5].i05 = Pid;
    }
    DialogUnk29( 7 * v5 );
    if( DialogUnk14() ) v8 = -1;
    if( IsCursorClear ) MouseMgrRedraw();
    return v8;
}

int DialogUnk32()
{
    return gSayUnk100 + ( gSayUnk101 << 16 );
}

int DialogQuit()
{
    if( gSayUnk02 )
        gSayUnk01 = 1;
    else
        DialogUnk14();
    return 0;
}

int DialogOptWin( int x0, int y0, int x1, int y1, char *title )
{
    gSayOptWinX0 = x0;
    gSayOptWinY0 = y0;
    gSayOptWinX1 = x1;
    gSayOptWinY1 = y1;
    gSayOptWinTitle = title;
    return 0;
}

int DialogReplyWin( int x0, int y0, int x1, int y1, char *title )
{
    gSayReplyWinX0 = x0;
    gSayReplyWinY0 = y0;
    gSayReplyWinX1 = x1;
    gSayReplyWinY1 = y1;
    gSayReplyWinTitle = title;
    return 0;
}

int DialogBorder( int Width, int Height )
{
    gSayWinHorizontal = Width;
    gSayWinVertical = Height;
    return 0;
}

int DialogScrollUp( int a1, int a2, char *a3, char *a4, char *a5, char *a6, int a7 )
{
    gSayUnk16 = a1;
    gSayUnk15 = a2;
    if( gSayUnk17 ) dbg_free( gSayUnk17 );
    gSayUnk17 = a3;
    if( gSayUnk18 ) dbg_free( gSayUnk18 );
    gSayUnk18 = a4;
    if( gSayUnk19 ) dbg_free( gSayUnk19 );
    gSayUnk19 = a5;
    if( gSayUnk20 ) dbg_free( gSayUnk20 );
    gSayUnk20 = a6;
    gSayUnk21 = a7;
    return 0;
}

int DialogScrollDn( int a1, int a2, char *a3, char *a4, char *a5, char *a6, int a7 )
{
    gSayUnk114 = a1;
    gSayUnk115 = a2;
    if( gSayUnk117 ) dbg_free( gSayUnk117 );
    gSayUnk117 = a3;
    if( gSayUnk118 ) dbg_free( gSayUnk118 );
    gSayUnk118 = a4;
    if( gSayUnk119 ) dbg_free( gSayUnk119 );
    gSayUnk119 = a5;
    if( gSayUnk120 ) dbg_free( gSayUnk120 );
    gSayUnk120 = a6;
    gSayUnk116 = a7;
    return 0;
}

int DialogSetOptSpacing( int spc )
{
    gSayOptSpacing = spc;
    return 0;
}

int DialogSetOptionColor( float r, float g, float b )
{
    gSayOptColors = 1;    
    gSayOptColorR = lround( r * 31.0 );
    gSayOptColorG = lround( g * 31.0 );    
    gSayOptColorB = lround( b * 31.0 );
    return 1;
}

int DialogSetReplyColor( float r, float g, float b )
{
    gSayReplyColors = 1;    
    gSayReplyColorR = lround( r * 31.0 );
    gSayReplyColorG = lround( g * 31.0 );
    gSayReplyColorB = lround( b * 31.0 );
    return 1;
}

int DialogSetOptionFlags( short flags )
{
    gSayOptFlags = flags;
    return 1;
}

void DialogFree()
{
    if( gSayUnk17 ) dbg_free( gSayUnk17 );
    if( gSayUnk18 ) dbg_free( gSayUnk18 );
    if( gSayUnk19 ) dbg_free( gSayUnk19 );
    if( gSayUnk20 ) dbg_free( gSayUnk20 );
    if( gSayUnk117 ) dbg_free( gSayUnk117 );
    if( gSayUnk118 ) dbg_free( gSayUnk118 );
    if( gSayUnk119 ) dbg_free( gSayUnk119 );
    if( gSayUnk120 ) dbg_free( gSayUnk120 );
}

int DialogGetSayModeLvl()
{
    return gSayModeLvl;
}

void DialogUnk34( int (*a1)(), int (*a2)() )
{
    gSayUnk32 = a2;
    gSayUnk33 = a1;
}

int DialogSetOneOptPause( int stat )
{
    if( (stat & gSayOptStat) == stat )
        gSayOptStat &= ~stat;
    else
        gSayOptStat |= stat;
    return gSayOptStat;
}

int DialogGetOneOptPause()
{
    return gSayOptStat;
}



