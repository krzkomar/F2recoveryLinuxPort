#include "FrameWork.h"

static char 	*gPalStack[ 16 ];	// internal color stack
Pal8_t 	gPalCurrent[ 256 ];
static char	gPalBright[ 64 ];	// bright color map
static char 	*gPalExtUsed[ 256 ];	// AAF font color
static char	gPalColorUsed[ 256 ];	// mapa uzywanych kolorow
unsigned char 	gPalSemiTransparent[ 256 ][ 256 ]; // used for semitransparent walls
unsigned char 	gPalShades[ 256 ][ 256 ];	   // used for shading objects/tiles
unsigned char 	gPalExt1[ 256 ][ 256 ];
unsigned char	gPalColorCubeRGB[ 32 ][ 32 ][ 32 ];
static int	gPalColorStackPointer;
static int 	gPal_rrr;
static int 	gPal_bbb;
static int 	gPal_ggg;
//static int 	(*gPal10)[32][32][32];
static int 	(*gpPalRGBi)[32][32][32];
static int 	gCnt;  
static char 	(*gpPalRGBb)[32][32][32];
//static int 	gPal01;
//static int 	gPal17;
//static int 	gPal16;
//static int 	gPal14;
static int 	gPalFrac_y;
static int 	gPalFrac_r;
//static char 	(*gPal09)[32][32][32];
//static int 	gPal18;
//static int 	gPal11;
//static int 	gPal15;
static int 	gPal_g5;
static int 	gPal_b5;
static int 	gPal_r5;
static int 	gPalFrac_g;
static int  	(*gPalReadFile)( xFile_t *, void *, int);
static void 	(*gPalCloseFile)( xFile_t * );
static xFile_t 	*(*gPalOpenFile)( const char *, int );

static char *gPalErrorMsg = "color.c: No errors\n";
static double gPalBrightness = 1.0;
static void (*gPalFadeCb)( int ) = NULL;
static void *(*gPalMalloc)( size_t ) = PalAlloc;
static void *(*gPalRealloc)( void *, size_t ) = PalRealloc;
static void (*gPalFree)( void *) = PalFree;
static char *(*gPalGetPathCb)() = NULL;
Pal8_t gPalBase[ 256 ] = { {0x3f,0x3f,0x3f} };

xFile_t *PalOpenFile( const char *path, int mode )
{
    if( gPalOpenFile )
        return gPalOpenFile( path, mode );
    else
        return (xFile_t *)-1;
}

int PalReadFile( xFile_t *fh, void *ptr, int size )
{
    if( gPalReadFile )
        return gPalReadFile( fh, ptr, size );
    else
        return -1;
}

void PalCloseFile( xFile_t *fh )
{
    if( gPalCloseFile ) gPalCloseFile( fh );
}

void PalSetFileManage( xFile_t *(*Open)(const char *, int), int (*Read)(xFile_t *, void *, int), void (*Close)(xFile_t *) )
{
    gPalReadFile = Read;
    gPalCloseFile = Close;
    gPalOpenFile = Open;
}

void *PalAlloc( size_t nsize )
{
    return malloc( nsize );
}

void *PalRealloc( void *ptr, size_t size )
{
    return realloc( ptr, size );
}

void PalFree( void *ptr )
{
    free( ptr );
}

void PalSetPathCb( char *(*Cb)() )
{
    gPalGetPathCb = Cb;
}

char PalExt0GetColor( unsigned char Color, unsigned char Shade )
{
    return gPalSemiTransparent[ Color ][ Shade ];
}

char PalExt1GetColor( unsigned char Color, unsigned char Shade )
{
    return gPalExt1[ Color ][ Shade ];
}

char PalGetColorShade( int Shade, unsigned char Color )
{
    return gPalShades[ Color ][ (Shade >> 9) & 0xff ];
}

char PalGetColorMap( int Color )
{
    return ((int *)gPalColorCubeRGB)[ Color ];
}

int PalGetColor16( unsigned char Color )
{
    return PalConvColor8to16( Color );
}

int PalConvColor8to16( unsigned char color8 ) // 8bit color to 16bit 555
{
    unsigned short r,g,b;
    r = ((gPalBase[ color8 ].b >> 1 ) & 0x1f);
    g = ((gPalBase[ color8 ].g >> 1 ) & 0x1f) << 5;
    b = ((gPalBase[ color8 ].r >> 1 ) & 0x1f) << 10; 
    return r | g | b;
}

void PalFadeIn( Pal8_t *PalScale, Pal8_t *PalIn, int Steps )
{
    Pal8_t PalTmp[ 256 ];
    int j, i;

    for( j = 0; j < Steps; j++ ){
        for( i = 0; i < 256; i++ ){
            PalTmp[ i ].r = PalScale[ i ].r - j * (PalScale[ i ].r - PalIn[ i ].r) / Steps;
            PalTmp[ i ].g = PalScale[ i ].g - j * (PalScale[ i ].g - PalIn[ i ].g) / Steps;
            PalTmp[ i ].b = PalScale[ i ].b - j * (PalScale[ i ].b - PalIn[ i ].b) / Steps;
        }
        if( gPalFadeCb && !(j % 128) ) gPalFadeCb( j / 128 );
        PalSetColors( PalTmp );
    }
    PalSetColors( PalIn );
}

void PalSetFadeSndCb( void (*cb)(int) )
{
    gPalFadeCb = cb;
}

void PalSetColors( Pal8_t *Palette )
{
    int i;
    Pal8_t TmpPal[ 256 ];
    for( i = 0; i < 256; i++ ){
        TmpPal[ i ].r = gPalBright[ (int)Palette[ i ].r ];
        TmpPal[ i ].g = gPalBright[ (int)Palette[ i ].g ];
        TmpPal[ i ].b = gPalBright[ (int)Palette[ i ].b ];
    }
    memcpy( gPalCurrent, Palette, sizeof( Pal8_t ) * 256 );
    VidSetPaletteAll( TmpPal );
}

Pal8_t *PalGetCurrent()
{
    return gPalCurrent;
}

void PalModulate( Pal8_t *Palette, unsigned int FirstColor, int LastColor )
{
    Pal8_t NewPal[ 256 ];
    unsigned int colors, col, i;
    unsigned char r,g,b;

    colors = LastColor - FirstColor + 1;
    col = FirstColor;
    Palette += FirstColor;
    for( i = 0; i < colors; i++, col++, Palette++ ){
        NewPal[ col ].r = gPalBright[ (r = Palette->r) & 0x3f ];
        NewPal[ col ].g = gPalBright[ (g = Palette->g) & 0x3f ];
        NewPal[ col ].b = gPalBright[ (b = Palette->b) & 0x3f ];
        gPalCurrent[ col ].r = r;
        gPalCurrent[ col ].g = g;
        gPalCurrent[ col ].b = b;
    }
    VidSetPaletteRange( &NewPal[FirstColor], FirstColor, colors );
}

void PalSetCurrentColorRGB( int PalIdx, unsigned char r, unsigned char g, unsigned char b )
{
    gPalCurrent[ PalIdx ].r = r;
    gPalCurrent[ PalIdx ].g = g;
    gPalCurrent[ PalIdx ].b = b;
    VidSetPaletteIndex( PalIdx, gPalBright[ r ], gPalBright[ b ], gPalBright[ g ]);
}

void PalGetCurrentColorRGB( int PalIdx, char *pR, char *pB, char *pG )
{
    *pR = gPalCurrent[ PalIdx ].r;
    *pG = gPalCurrent[ PalIdx ].g;
    *pB = gPalCurrent[ PalIdx ].b;
}


void PalMakeShades( unsigned char Color8 )
{
    short col16;
    int r,k,g,i,b;

    col16 = PalConvColor8to16( Color8 );
    r = RGB16_GET_R( col16 );
    g = RGB16_GET_G( col16 );
    b = RGB16_GET_B( col16 );
    for( i = k = 0; i < 128; i++ ){
        gPalShades[ Color8 ][ 0   + i ] = gPalColorCubeRGB[ (k * r) >> 16 ][ (k * g) >> 16 ][ (k * b) >> 16 ];
        gPalShades[ Color8 ][ 128 + i ] = gPalColorCubeRGB[ r + ((k * (31 - r)) >> 16 )][ g + ((k * (31 - g)) >> 16) ][ b + ((k * (31 - b)) >> 16) ];
        k += 512;
    }
}

void PalCreateShades()
{
    int i;
    
    for( i = 0; i < 256; i++ ){
        if( gPalColorUsed[ i ] )
            PalMakeShades( i );
        else
            memset( gPalShades[ i ], 0, 256 );
    }
}

void PalMakeExt( int Col )
{
    unsigned int i, g, r, b;
    unsigned char ColorShade, max;

    for( i = 0; i < 256; i++ ){
        if( !gPalColorUsed[ Col ] ){
            if( gPalColorUsed[ i ] ){
                gPalExt1[ Col ][ i ] = i;
                gPalSemiTransparent[ Col ][ i ] = i;
                continue;
            }
            gPalExt1[ Col ][ i ] = gPalSemiTransparent[ Col ][ i ] = Col;
            continue;
        }
        if( !gPalColorUsed[ i ] ){
            gPalExt1[ Col ][ i ] = gPalSemiTransparent[ Col ][ i ] = Col;
            continue;
        }
	r = RGB16_GET_R( PalConvColor8to16( Col ) ) + RGB16_GET_R( PalConvColor8to16( i ) );
        g = RGB16_GET_G( PalConvColor8to16( Col ) ) + RGB16_GET_G( PalConvColor8to16( i ) );
        b = RGB16_GET_B( PalConvColor8to16( Col ) ) + RGB16_GET_B( PalConvColor8to16( i ) );
        max = r;
        if( g > max ) max = g;
        if( b > max ) max = b;
        ColorShade = gPalColorCubeRGB[ r ][ g ][ b ];
        if( max > 31 ) ColorShade = PalGetColorShade( lround(((max + -31.0) * 0.0078125 + 1.0) * 65536.0), ColorShade );        
        gPalSemiTransparent[ Col ][ i ] = ColorShade;
        gPalExt1[ Col ][ i ] = gPalColorCubeRGB
    	    [ ( RGB16_GET_R( PalConvColor8to16( Col ) ) * RGB16_GET_R( PalConvColor8to16( i ) ) ) >> 5 ]
    	    [ ( RGB16_GET_G( PalConvColor8to16( Col ) ) * RGB16_GET_G( PalConvColor8to16( i ) ) ) >> 5 ]
    	    [ ( RGB16_GET_B( PalConvColor8to16( Col ) ) * RGB16_GET_B( PalConvColor8to16( i ) ) ) >> 5 ];
    }
}

void PalCreateExt()
{
    int i;

    for( i = 0; i < 256; i++ ) PalMakeExt(i);
}

// loads *.pal file
int PalLoadFromFile( const char *Path )
{
    xFile_t *stream;
    int i;
    unsigned int tag;
    unsigned char r, g, b;

    if( gPalGetPathCb ) Path = gPalGetPathCb();
    stream = (void *)-1;
    if( gPalOpenFile ) stream = gPalOpenFile( Path, 0x200 );
    if( stream == (void *)-1 ){
        gPalErrorMsg = "color.c: color table not found\n";
	return 0;
    }
    for( i = 0; i < 256; i++ ){
        if( gPalReadFile ) gPalReadFile(stream, &r, 1);
        if( gPalReadFile ) gPalReadFile(stream, &g, 1);
        if( gPalReadFile ) gPalReadFile(stream, &b, 1);
        if( r > 63 || g > 63 || b > 63 ){ // dopuszczlny zakres wartosci RGB to 0..63
            g = b = r = 0;
            gPalColorUsed[ i ] = 0;
        } else {
            gPalColorUsed[ i ] = 1;
        }
        gPalBase[ i ].r = r;
        gPalBase[ i ].g = g;
        gPalBase[ i ].b = b;
    }
    if( gPalReadFile ) gPalReadFile( stream, gPalColorCubeRGB, sizeof( gPalColorCubeRGB ) );
    tag = 0;
    if( gPalReadFile ) gPalReadFile( stream, &tag, 4 );
    if( tag == TAG_NEWC ){ 
        if( gPalReadFile ) gPalReadFile( stream, gPalShades, sizeof( gPalShades ) );
        if( gPalReadFile ) gPalReadFile( stream, gPalSemiTransparent, sizeof( gPalSemiTransparent ) );
	if( gPalReadFile ) gPalReadFile( stream, gPalExt1, sizeof( gPalExt1 ) );
    } else {
        PalCreateShades();
	for( i = 0; i < 256; i++ ) PalMakeExt( i );
    }
    PalExtInit();
    if( gPalCloseFile ) gPalCloseFile(stream);
/*...*/
// 1:1
//for( i = 0; i < 256; i++ ) memset( &gPalShades[i], i, 256 );

    return 1;
}

char *PalGetErrorMsg()
{
    return gPalErrorMsg;
}

void PalLoadFromPtr( Pal8_t *pal )
{
    memcpy( &gPalBase, pal, sizeof( gPalBase ) );
    memset( gPalColorUsed, 1, sizeof( gPalColorUsed ) );
}

void PalSetColorRGB( int ColorIdx, char r, char g, char b )
{
    gPalBase[ ColorIdx ].r = r;
    gPalBase[ ColorIdx ].g = g;
    gPalBase[ ColorIdx ].b = b;
}

void PalGetColorRGB( int ColorIdx, char *r, char *g, char *b )
{
    *r = gPalBase[ ColorIdx ].r;
    *g = gPalBase[ ColorIdx ].g;
    *b = gPalBase[ ColorIdx ].b;
}

/*
    Inicjuje palete Ext:
     0 - jasnosc liniowa
     1..7  - cyklicznie z okresem 7 ?
     8..15 - cyklicznie z okresem 7 ?
*/
void PalInitExt( char *PalStack, int ColorIdx )
{
    int i, v5,v7,v10, v27,v24,v25,k,j;
    int t, n;
    
    v5 = (PalConvColor8to16( ColorIdx ) & 0x7C00) >> 10;
    v7 = (PalConvColor8to16( ColorIdx ) & 0x03E0) >> 5;
    v10 = PalConvColor8to16( ColorIdx ) & 0x001F;
    for( i = 0; i < 256; i++ ) PalStack[ i ] = i; // paleta 0 - przezroczystosc

    v27 = v10;
    v24 = v7;
    v25 = v5;
    for( k = 6, i = 1; i < 8; i++, k-- ){ // paleta->[1..7] k->[6..0]
        for( j = 0; j < 256; j++ ){
    	    t = PalConvColor8to16( j );
            PalStack[ i * 256 + j ] = gPalColorCubeRGB
        	[ (v25 + (( t & 0x7C00) >> 10 ) * k ) / 7 ]
        	[ (v24 + (( t & 0x03E0) >> 5  ) * k ) / 7 ]
        	[ (v27 + (( t & 0x001F) >> 0  ) * k ) / 7 ];
        }
        v27 += v10, v24 += v7, v25 += v5;
    }
    for( k = 0, i = 8; i < 16; i++, k += 0x10000 ){ // paleta 8 .. 15
        n = k / 7 + 0xFFFF;
        for( j = 0; j < 256; j++ ){
            PalStack[ i * 256 + j ] = PalGetColorShade( n, ColorIdx );
        }
    }
}

void PalExtInit()
{
    int i;

    for( i = 0; i < 256; i++ ){
        if( gPalExtUsed[ i ] ) PalInitExt( gPalExtUsed[ i ], i );
    }
}

char *PalExtOpen( unsigned char Color )
{
    int idx;
    char *p;

    idx = Color;
    if( !gPalExtUsed[ idx ] ){
        p = gPalMalloc( 256 * 16 + sizeof(int) );
        *((int *)p) = 1;
        gPalExtUsed[ idx ] = p + sizeof(int);
        PalInitExt( gPalExtUsed[ idx ], Color );
    }
    (*((int *)(gPalExtUsed[ idx ] - 4)))++;
    return gPalExtUsed[ idx ];
}

void PalExtClose( unsigned char Color )
{
    char *p;
    int z, idx = Color;
    
    if( !(p = gPalExtUsed[ idx ]) ) return;
    z = --(*((int *)(gPalExtUsed[ idx ] - 4)));
    if( !z ){
	gPalFree( gPalExtUsed[ idx ] - sizeof(int) );
	gPalExtUsed[ idx ] = NULL;
    }
}

void PalSetMemManage( void *(*Malloc_)(size_t), void *(*Realloc_)(void *, size_t), void (*Free_)(void *))
{
    gPalRealloc = Realloc_;
    gPalFree = Free_;
    gPalMalloc = Malloc_;
}

void PalSetBrightness( double bright )
{
    double c;
    int i;

    gPalBrightness = bright;
    for( i = 0; i < 64; i++ ){
        c = pow( i, gPalBrightness );
        if( c > 63.0 ) c = 63.0;
        if( c < 0.0 )  c = 0.0;
        gPalBright[ i ] = lround( c );
    }
    PalSetColors( gPalCurrent );
}

double PalGetBrightness()
{
    return gPalBrightness;
}

int PalIsColorUsed ( unsigned char ColrIdx )
{
    return gPalColorUsed[ ColrIdx ];
}

int PalCreateColors( Pal8_t *pal, char *UnusedColors )
{
    int (*piColorCube)[32][32][32], i, PalFrac_b;
    unsigned char b, g, r;
    
    if( !(piColorCube = malloc( 32 * 32 * 32 * sizeof( int ) )) ) return 1;
    for( i = 0; i < 32*32*32*sizeof( int ); i++ ) ((int *)piColorCube)[ i ] = -1;
    memset( gPalColorCubeRGB, 0, sizeof( gPalColorCubeRGB ) );
    for( gCnt = 0; gCnt < 256; gCnt++ ){
        if( UnusedColors ){
            if( UnusedColors[ gCnt ] ) gPalColorUsed[ gCnt ] = 0;
        } else {
            r = pal[ gCnt ].r;
            g = pal[ gCnt ].g;
            b = pal[ gCnt ].b;
            gPal_r5 = r / 8;
            gPal_g5 = g / 8;
            gPal_b5 = b / 8;
            gPalFrac_r = ( r % 8 ) - ( 8 / 2 ); // <-4 .. 0 .. +3>
            gPalFrac_g = ( g % 8 ) - ( 8 / 2 );
            PalFrac_b  = ( b % 8 ) - ( 8 / 2 );

            gPalFrac_y = PalFrac_b * PalFrac_b + gPalFrac_g * gPalFrac_g / 36 + gPalFrac_r * gPalFrac_r / 9; // bb^2 + 0,027*gg^2 + 0,11*rr^2  luminance?

            gPal_rrr = 2 * (((r / 8) + 1) * 64 - 8 * r);
            gPal_ggg = 2 * (((g / 8) + 1) * 64 - 8 * g);
            gPal_bbb = 2 * (((b / 8) + 1) * 64 - 8 * b);

            gpPalRGBi = (int (*)[32][32][32])&piColorCube[ r / 8 ][ g / 8 ][ b / 8 ];
            gpPalRGBb = (char (*)[32][32][32])&gPalColorCubeRGB[ r / 8 ][ g / 8 ][ b / 8 ];
            PalGenerateRGBcube();
            gPalBase[ gCnt ].r = r / 4;
            gPalBase[ gCnt ].g = g / 4;
            gPalBase[ gCnt ].b = b / 4;
            gPalColorUsed[ gCnt ] = 1;
        }
    }
    free( piColorCube );
    PalCreateShades();
    for( i = 0; i < 256; i++ ) PalMakeExt( i );
    PalExtInit();
    return 0;
}

int PalApplyColors( Pal8_t *SrcPal, char *UnusedColors )
{    
    if( PalCreateColors( SrcPal, UnusedColors ) ) return 1;
    PalSetColors( gPalBase );
    return 0;
}

void PalGenerateRGBcube()
{
/*
    int i, v1, v2, v3, v4, v7, v8;
    static int gPal08;

    gPalFrac_r = gPalFrac_y;
    gPal10 = gpPalRGBi;
    gPal09 = gpPalRGBb;
    v2 = 1;
    gPal08 = gPal_rrr;
    for( i = gPal_r5; i < 32; i++ ){
        if( PalUnk06( v2 ) ){
            v4 = gPal08;
        } else {
            v4 = gPal08;
            if( v3 ) break;
        }        
        gPal10 += 32*32 * sizeof(int);
        gPal09 += 32*32;
        v2 = 0;
        gPalFrac_r += v4;
        gPal08 = v1 + v4;
    }
    gPalFrac_r = gPalFrac_y - (gPal_rrr - v1);
    gPal10 = gpPalRGBi - 32*32 * sizeof(int);
    gPal09 = gpPalRGBb - 32*32;
    gPal08 = gPal_rrr - v1;
    v2 = 1;
    for( i = gPal_r5 - 1; i >= 0; i-- ){
        if( !PalUnk06( v2 ) && v7 ) break;        
        gPal10 -= 32*32 * sizeof(int);
        gPal09 -= 32*32;
        gPal08 -= 2 * 64;
        gPalFrac_r -= gPal08;
        v2 = 0;
    }
*/
}

//int PalUnk06( int InitFlag )
//{
/*
    static int varA;
    static int varB;
    static int varC;
    int sa, sb, sc,sd;    
    int jj, GoNext, ii, n, k, i; 

    if( InitFlag ){
        varA = gPal_g5;
        varB = 32 - 1;
        varC = gPal_ggg;
    }    
    sa = gPalFrac_g = gPalFrac_r;    
    sb = gPal14 = gPal10;
    sc = gPal18 = gPal09;
    GoNext = 0;    
    ii = 1;    
    sd = varC;
    for( jj = varA ;jj <= varB; jj++ ){
	if( PalUnk07( ii ) ){
            n = sd;
            if( !GoNext ){
                if( jj > varA ){
                    gPal10 = sb;
                    varC = sd;
                    gPal09 = sc;
                    varA = jj;
                    gPalFrac_r = sa;
                }
                GoNext = 1;
            }
        } else {
            n = sd;
            if( GoNext ) break;
        }        
        gPal14 += 4 * 32;
        sb += 4 * 32;
        gPal18 += 32;
        sc += 32;
        ii = 0;
        gPalFrac_g += n;
        sa += n;
        sd = 2 * 64 + n;
    }
    gPal14 = gPal10 - 4 * 32;
    gPal18 = gPal09 - 32;
    sb = gPal10 - 4 * 32;
    sc = gPal09 - 32;
    gPalFrac_g = gPalFrac_r - (varC - 2 * 64);
    sa = gPalFrac_r - (varC - 2 * 64);
    sd = varC - 2 * 64;
    i = 1
    for( k = varA - 1; k >= 0; k-- ){
        if( PalUnk07( i ) ){
            if( !GoNext ){
                GoNext = 1;
                gPal10 = sb;
                varA = k;
                gPal09 = sc;
                varC = sd;
                gPalFrac_r = sa;
            }
        } else if( GoNext ){
            return GoNext;
        }        
        gPal14 -= 4 * 32;
        sb -= 4 * 32;
        sc -= 32;
        gPal18 -= 32;
        gPalFrac_g -= sd;
        sd -= 2 * 64;
        sa -= sd;
        i = 0
    }
    return GoNext;
*/
//}

//int PalUnk07( int InitFlag )
//{
/*
    static int varA;
    static int varB;
    static int varC;

    unsigned int k, n;
    unsigned int *p, *q;
    int GoNext;

    if( InitFlag ){
        varA = gPal_b5;
        varB = 32 - 1;
        varC = gPal_bbb;
    }

    k = varC;
    n = gPalFrac_g;
    p = gPal14;
    q = gPal18;    
    GoNext = 0;
    for( i = varA; i <= varB; i++, p++, q++ ){
        if( n < *p ){
    	    if( i > varA ){
        	gPal14 = p;
        	gPal18 = q;
        	gPalFrac_g = n;
        	varC = k;
        	varA = i;
    	    }
    	    GoNext = 1;
    	    break;
        }        
        n += k;
        k += 2 * 64
    }
    for( ;i <= varB && n < *p; i++ ){
        *p++ = n;
        *q++ = gCnt;        
        n += k;
        k += 2 * 64;
    }

    n = varC - 2 * 64;
    k = gPalFrac_g - varC - 2 * 64;
    p = gPal18 - 1;
    q = gPal14 - 4;
    if( GoNext == 0 ){
        for( i = varA - 1; i >= 0; q--, p--, i-- ){
            if( k < *q ){
                gPal14 = q;
                gPal18 = p;
                gPalFrac_g = k;
                varC = n;
                varA = i;
                GoNext = 1;
                break;
            }            
            n -= 2 * 64;
            k -= n;
        }
    }
    for( ;i >= 0 && k < *q; i-- ){        
        n -= 2 * 64;
        k -= n;
        *q-- = k;
        *p-- = gCnt;
    }
    return GoNext;
*/
//}

void PalResetCubeRGB( int *cube, int dim )
{
    int i;

    for( i = 0; i < dim*dim*dim; i++ ) ((int *)cube)[ i ] = -1;
}

// push current colors on internal stack
int PalColorPush()
{
    char *p;

    if( gPalColorStackPointer >= PAL_COLOR_STACK_DEEP ){
        gPalErrorMsg = "color.c: colorpalettestack overflow\n";
	return 0;
    }
    p = malloc( 0x8400 );
    gPalStack[ gPalColorStackPointer ] = p;
    memcpy( p, gPalColorUsed, 0x100 );
    memcpy( gPalStack[ gPalColorStackPointer ] + 0x100, &gPalBase, 0x300 );
    memcpy( gPalStack[ gPalColorStackPointer ] + 0x400, gPalColorCubeRGB, 32*32*32 );
    gPalColorStackPointer++;
    return 1;
}

// pop current colors from internal stack
int PalColorPop()
{
    int i;

    if( gPalColorStackPointer == 0 ){
        gPalErrorMsg = "color.c: colorpalettestack underflow";
	return 0;
    }
    gPalColorStackPointer--;
    memcpy( gPalColorUsed, gPalStack[ gPalColorStackPointer ], sizeof( gPalColorUsed ) );
    memcpy( &gPalBase,  gPalStack[ gPalColorStackPointer ] + 0x100, sizeof( gPalBase ) );
    memcpy( gPalColorCubeRGB, gPalStack[ gPalColorStackPointer ] + 0x400, 32*32*32 );
    free( gPalStack[ gPalColorStackPointer ] );
    gPalStack[ gPalColorStackPointer ] = NULL;
    PalCreateShades();
    for( i = 0; i < 256; i++ ) PalMakeExt( i );
    PalExtInit();
    return 1;
}

int PalInit()
{
    static int gPaletteInit = 0;
    if( gPaletteInit ) return 1;
    gPaletteInit = 1;
    PalSetBrightness( 1.0 );
    if( PalLoadFromFile("color.pal") != 1 ) return 0;
    PalSetColors( gPalBase );
    return 1;
}

void PalStackZero()
{
    int i;

    for( i = 0; i < 256; i++ ) PalExtClose( i );
    for( i = 0; i < gPalColorStackPointer; i++ ) free( gPalStack[ i ] );
    gPalColorStackPointer = 0;
}

Pal8_t *PalGetBase()
{
    return gPalBase;
}

