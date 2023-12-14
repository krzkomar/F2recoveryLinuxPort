#include "FrameWork.h"

extern int gSysResolWidth;
extern int gSysResolHeight;

int gWinCurrent = -1;

int gWinUnk101 = 0;
int gWinDlgCnt = -1;
int gWinUnk102 = 1;

Win02_t gWinUnk103[ 16 ];
int (**gWinUnk100)( int );

int gWinColorR_HL;
int gWinTextFont;

int gWinColorG;
int gWinColorB;
int gWinTextFlags;
int gWinColorR;
int gWinColorG_HL;
int gWinColorB_HL;

/*************************************************************************/

int WinGetFont()
{
    return gWinTextFont;
}

int WinSetFont( int FontId )
{
    gWinTextFont = FontId;
    FontSet( FontId );
    return 1;
}

void WinSetTextColorDefault()
{
    gWinColorG = 31;
    gWinColorB = 31;
    gWinTextFlags = 0x2010000;
    gWinColorR = 31;
}

int WinGetTextFlags()
{
    return gWinTextFlags;
}

int WinSetTextFlags( int Flags )
{
    gWinTextFlags = Flags;
    return 1;
}

char WinGetTextColor()
{
    return gPalColorCubeRGB[ gWinColorR ][ gWinColorG ][ gWinColorB ];
}

char WinGetColorHL()
{
    return gPalColorCubeRGB[ gWinColorR_HL ][ gWinColorG_HL ][ gWinColorB_HL ];
}

void WinSetTextColor( float r, float g, float b )
{
    gWinColorG = (int)lround( 31.0 * g );
    gWinColorB = (int)lround( 31.0 * b );
    gWinColorR = (int)lround( 31.0 * r );
}

void WinSetTextColorHL( float r, float g, float b )
{
    gWinColorG_HL = (int)lround( 31.0 * g );
    gWinColorB_HL = (int)lround( 31.0 * b );
    gWinColorR_HL = (int)lround( 31.0 * r );
}

int WinUnk97( char *Str, int a2, int a3 )
{
    char *s, *p;
    int t, k, w;

    t = a3;
    s = Str;
    k = 1;
    while( *Str ){
        w = gFont.ChrWidth( *Str ) + t;
        if( *Str == '\n' || w > a2 ){
            while( w > a2 ) w -= gFont.ChrWidth( *Str );
            t = a3;
            k++;
            p = Str + 1;
            if( *Str == '\n' ){
        	Str = p;
            } else {
        	while ( Str != s && *Str != ' ' ) Str--;
        	if( Str == s ) Str = p;
            }
            while( *Str == ' ' ) Str++;
            s = Str;
        } else {
            Str++;
            t += gFont.Distance();;
        }
    }
    return k;
}






void WinDlgInsertExec( int (*ptr)(int) )
{
    int (**p)( int );
    int i;

    p = gWinUnk100;
    for( i = 0; i < gWinUnk101; i++ ){
        if( gWinUnk100[ i ] == NULL ) break;
    }        
    if( i == gWinUnk101 ){
        if( gWinUnk100 ){
            p = dbg_realloc( gWinUnk100, sizeof( void * ) * ( gWinUnk101 + 1 ) );
        } else {
            gWinUnk100 = NULL;
            p = dbg_malloc( sizeof( void * ) );
        }
    }
    p[ gWinUnk101 ] = ptr;
    gWinUnk101++;
    gWinUnk100 = p;
}








int WinDlgProcess()
{
    int sel, i;

    sel = InpUpdate();
    if( sel == 17 || sel == 24 || sel == 324 ) SysQuitDlg();
    if( gMenuEscape ){
        gWinUnk102 = 1 - gWinUnk102;
        return ( gWinUnk102 ) ? -1: KEY_ESC;
    }
    if( gWinUnk101 <= 0 ) return sel;    
    for( i = 0; i < gWinUnk101; i++ ){
        if( gWinUnk100[ i ] == NULL) continue;
        if( gWinUnk100[ i ]( sel ) ) return -1;
    }
    return sel;    
}




void WinDrawButton( int w, int h, char *surf1, char *surf2, char *surf3 )
{
    if( surf1 ){
        ScrFillSolid( surf1, w, h, w, gPalColorCubeRGB[0][0][0] );
        ScrFillSolid( &surf1[w + 1], w - 2, h - 2, w, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] );
        ScrLine( surf1, w, 1, 1, w - 2, 1, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf1, w, 2, 2, w - 3, 2, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf1, w, 1, h - 2, w - 2, h - 2, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf1, w, 2, h - 3, w - 3, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf1, w, w - 2, 1, w - 3, 2, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] );
        ScrLine( surf1, w, 1, 2, 1, h - 3, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf1, w, 2, 3, 2, h - 4, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf1, w, w - 2, 2, w - 2, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf1, w, w - 3, 3, w - 3, h - 4, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf1, w, 1, h - 2, 2, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] );
    }
    if( surf2 ){
        ScrFillSolid( surf2, w, h, w, gPalColorCubeRGB[0][0][0]);
        ScrFillSolid( &surf2[w + 1], w - 2, h - 2, w, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] );
        ScrLine( surf2, w, 1, 1, w - 2, 1, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf2, w, 1, 1, 1, h - 2, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
    }
    if( surf3 ){
        ScrFillSolid( surf3, w, h, w, gPalColorCubeRGB[0][0][0] );
        ScrFillSolid( &surf3[w + 1], w - 2, h - 2, w, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] );
        ScrLine( surf3, w, 1, 1, w - 2, 1, gPalColorCubeRGB[31][31][31]);
        ScrLine( surf3, w, 2, 2, w - 3, 2, gPalColorCubeRGB[31][31][31]);
        ScrLine( surf3, w, 1, h - 2, w - 2, h - 2, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf3, w, 2, h - 3, w - 3, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf3, w, w - 2, 1, w - 3, 2, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] ); 
        ScrLine( surf3, w, 1, 2, 1, h - 3, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf3, w, 2, 3, 2, h - 4, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf3, w, w - 2, 2, w - 2, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf3, w, w - 3, 3, w - 3, h - 4, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf3, w, 1, h - 2, 2, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31] ][ 89 ] );
    }
}




void WinDlgClose()
{
    int i;

    for( i = 0; i < 16; i++ ){
//        if( gWinUnk103[ i ].WinId != -1 ) WinDelete( gWinUnk103[ i ].Name );
    }
    if( gWinUnk100 ) dbg_free( gWinUnk100 );
    MouseMgrFree();
    dbFree();
    WinDestruct();
}




void WinPrintText( int Id, char *Str, int BufferSize, int FieldSize, int MaxHeight, int posX, int posY, int Color, int Align )
{
    int Width, h, PitchDst, Len, Height;
    char *ptr, *pDst, *s, *pBitmap, *p;

    Len = BufferSize;
    if( posY + gFont.ChrHeight() > MaxHeight ) return;
    if( BufferSize > 255 ) Len = 255;
    s = dbg_malloc( Len + 1 );
    strncpy( s, Str, Len );
    s[ Len ] = '\0';
    Width = gFont.LineWidth( s );
    h = gFont.ChrHeight();
    Height = h;
    if( !Width || !Height ){
        dbg_free( s );
        return;
    }
    if( Color & FONT_SHADOW ){ Width++; Height++; }
    pBitmap = p = dbg_calloc( Width, Height );
    gFont.Print( p, s, Width, Width, Color );

    switch( Align ){
	case 0: // left align
		if( Width < FieldSize ) FieldSize = Width; break;
	case 1: // right align
        	if( FieldSize < Width ){
            	    pBitmap = p + (Width - FieldSize);
        	} else {
		    posX += FieldSize - Width;
            	    FieldSize = Width;
            	}
	        break;
	case 2: // center align
        	if( FieldSize < Width ){
            	    pBitmap = p + (Width - FieldSize) / 2;
        	} else {
        	    posX = (FieldSize - Width) / 2;
            	    FieldSize = Width;
            	}    
		break;
        default: break;
    }    
    if( (Height + posY) > WinGetHeight( Id ) ) Height = WinGetHeight( Id ) - posY;
    PitchDst = WinGetWidth( Id );
    pDst = WinGetSurface( Id ) + posX;
    if( Color & 0x2000000 )
        ScrCopyAlpha( pBitmap, FieldSize, Height, Width, &pDst[ posY * PitchDst ], PitchDst );
    else
        ScrCopy( pBitmap, FieldSize, Height, Width, &pDst[ posY * PitchDst ], PitchDst );
    dbg_free( s );
    dbg_free( p );
}

char **WinTextWrapCreate( char *Text, int FieldWidth, int LineWidth, int *pLines )
{
    char *p, *s, **WrapList;
    int Len, Cnt;

    Cnt = 0;
    if( !Text ){ *pLines = 0; return NULL; }
    s = p = Text;
    WrapList = NULL;
    while( *s ){
        LineWidth += gFont.ChrWidth( *s );
        if( (*s == '\n') || (LineWidth > FieldWidth) ){
    	    for( ;LineWidth > FieldWidth; s-- ) LineWidth -= gFont.ChrWidth( *s ); // back to fit field size
            if( *s == '\n' ){
        	s++;
            } else {
        	for( ;(s != p) && (*s != ' '); s-- ); // turn back to first space
        	if( s == p ) s++;
            }
            if( WrapList )
                WrapList = dbg_realloc( WrapList, (Cnt + 1) * sizeof( char *) );
            else
                WrapList = dbg_malloc( sizeof( char *) );
            Len = s - p;
            WrapList[ Cnt ] = dbg_malloc( Len + 1 );
            strncpy( WrapList[ Cnt ], p, Len );
            WrapList[ Cnt ][ Len ] = '\0';
            for( ;*s == ' '; s++ ); // skip spaces
            p = s;
	    Cnt++;;
	    LineWidth = 0;
        } else {
            LineWidth += gFont.Distance();
            s++;
        }
    }
    if( p != s ){
        if( WrapList )
            WrapList = dbg_realloc( WrapList, (Cnt + 2) * sizeof( char *) );
        else
            WrapList = dbg_malloc( sizeof( char *) );
        Len = s - p;        
        WrapList[ Cnt ] = dbg_malloc( Len + 1 );
        strncpy( WrapList[ Cnt ], p, Len );
        WrapList[ Cnt ][ Len ] = '\0';
        Cnt++;
    }
    *pLines = Cnt;
    return WrapList;
}

void WinTextWrapDestroy( char **list, int cnt )
{
    int i;

    if( !list ) return;    
    for( i = 0; i < cnt; i++ ) dbg_free( list[ i ] );
    dbg_free( list );
}

void WinPrintWrapped( int WinId, char *Text, int FieldWidth, unsigned int Height, int posX, int PosY, int Color, unsigned int Flags, int StandOff )
{
    char **s, **list;
    int i,y,n,h;

    h = Height + PosY;
    if( !Text || !FieldWidth ) return;
    s = list = WinTextWrapCreate( Text, FieldWidth, 0, &n );
    for( i = 0; i < n; i++, s++ ){
        WinPrintText( WinId, *s, strlen( *s ), FieldWidth, h, posX, PosY + i * (StandOff + gFont.ChrHeight()), Color, Flags );
    }
    WinTextWrapDestroy( list, n );
}

void WinPrintMessage( int WinId, char *Text, int FieldWidth, int Height, int Xpos, int Ypos, int Color, int Flags )
{
    WinPrintWrapped( WinId, Text, FieldWidth, Height, Xpos, Ypos, Color, Flags, 0 );
}






int WinUnk16( char *a1, int a2, int a3, int a4, int a5, int a6 )
{
    int v8, v12;
    char *Surface;

    v8 = gWinUnk103[ gWinCurrent ].i10 * a5;
    Surface = WinGetSurface( gWinUnk103[ gWinCurrent ].WinId );
//    WinUnk70( &Surface[a4 + v8], a6, v12, gWinUnk103[ gWinCurrent ].i10, a1, a2, a3, a2 );
    return 1;
}

int WinGetScreenWidth()
{
    return gSysResolWidth;
}

int WinGetScreenHeight()
{
    return gSysResolHeight;
}


void WinBlit( char *data, int SrcW, int SrcH, char *surf, int DstW, int DstH )
{
    int i; // ebx
    int v7; // edx
    int v8; // ecx
    int v9; // ebp
    int v10; // edx
    int v11; // ebp
    int v12; // edx
    int v13; // ebp
    int v14; // edx
    int v15; // edx
    int v16; // ebp
    int v17; // edx
    int v18; // ebp
    int v19; // [esp+5Ch] [ebp-4Ch]
    int j; // [esp+68h] [ebp-40h]
    int k; // [esp+6Ch] [ebp-3Ch]
    int v23; // [esp+70h] [ebp-38h]
    char *pSrc; // [esp+74h] [ebp-34h]
    int v27; // [esp+90h] [ebp-18h]

    v23 = SrcW / 3;
    v27 = SrcH / 3;
    v19 = 0;
    for( pSrc = &data[SrcW * (SrcH / 3) + SrcW / 3]; v19 < DstW; v19 += v23 ){
        for( i = 0; i < DstH; i += v27 ){
            v7 = i + v27;
            v8 = ( v23 + v19 < DstW ) ? v23 : (DstW - v19);
            if( v7 >= DstH ) v7 = DstH;
            ScrCopy( pSrc, v8, v7 - i, SrcW, &surf[DstW * i + v19], DstW );
        }
    }
    for( j = 0; j < DstW; j += v23 ){
        v9 = v23 + j;
        v10 = v27;
        if( v23 + j >= DstW ) v9 = DstW;
        if( v27 >= DstH ) v10 = DstH;
        ScrCopy( &data[v23], v9 - j, v10, SrcW, &surf[j], DstW );
        v11 = v23 + j;
        if( v23 + j >= DstW ) v11 = DstW;
        ScrCopy( &data[SrcW * 2 * v27 + v23], v11 - j, v27, SrcW, &surf[DstW * (DstH - v27) + j], DstW );
    }
    for( k = 0; DstH > k; k += v27 ){
        v12 = v27 + k;
        v13 = ( v23 < DstW ) ? v23 : DstW;
        if( v12 >= DstH ) v12 = DstH;
        ScrCopy( &data[SrcW * v27], v13, v12 - k, SrcW, &surf[DstW * k], DstW );
        v14 = v27 + k;
        if( v27 + k >= DstH ) v14 = DstH;
        ScrCopy( &data[2 * v23 + SrcW * v27], v23, v14 - k, SrcW, &surf[DstW * k + DstW - v23], DstW );
    }
    v15 = v27;
    v16 = ( v23 < DstW ) ? v23 : DstW;
    if( v27 >= DstH ) v15 = DstH;
    ScrCopy( data, v16, v15, SrcW, surf, DstW );
    v17 = v27;
    if( v27 >= DstH ) v17 = DstH;
    ScrCopy( &data[2 * v23], v23, v17, SrcW, &surf[DstW - v23], DstW );
    v18 = v23;
    if( v23 >= DstW ) v18 = DstW;
    ScrCopy( &data[SrcW * 2 * v27], v18, v27, SrcW, &surf[DstW * (DstH - v27)], DstW );
    ScrCopy( &data[2 * v23 + SrcW * 2 * v27], v23, v27, SrcW, &surf[DstW * (DstH - v27) + DstW - v23], DstW );
}

int WinMovieUpdate()
{
    MvePlay();
//    MouseMgrAnimate();
//    WinUnk17();
//    return WidgetUnk13();
}





