#include "FrameWork.h"

typedef struct {
    int i01;
    int Width;
} TileLt_t;

int gTileMapAreaDefined = 0;
int gTileUnk62 = 1;
int gTileUnk63 = 1;
int gTileUnk60 = 1;
int gTileSketchMode = 0;
void (*gTileRedrawCb)(VidRect_t *, int ) = TileRenderIso;
int gTileUpdateEnable = 1;
int gTileHexX[ 6 ] = { 16, 32, 16, -16, -32, -16 };
int gTileHexY[ 6 ] = { -12, 0, 12, 12, 0, -12 };

TileLt_t gTileUnk71[ 13 ] = { 
    { -1, 2 },  // -1
    { 78, 2 },  // 80
    { 76, 6 },  // 82
    { 73, 8 },  // 81
    { 71, 10 }, // 81
    { 68, 14 }, // 82
    { 65, 16 }, // 81
    { 63, 18 }, // 81
    { 61, 20 }, // 81
    { 58, 24 }, // 82
    { 55, 26 }, // 81
    { 53, 28 }, // 81
    { 50, 32 }  // 82
};

TileLt_t gTileUnk65[ 13 ] = {
    { 0, 32 },  // 32
    { 48, 32 }, // 80
    { 49, 30 }, // 79
    { 52, 26 }, // 78
    { 55, 24 }, // 79
    { 57, 22 }, // 79
    { 60, 18 }, // 78
    { 63, 16 }, // 79
    { 65, 14 }, // 79
    { 67, 12 }, // 79
    { 70, 8 },  // 78
    { 73, 6 },  // 79
    { 75, 4 }   // 79
};

TileLight_t gTileShade[ 10 ] = {
    { 0x10,   -1,    -0xc9, 0 },
    { 0x30,   -2,    -2,    0 },
    { 0x3C0,  0x0,   0,     0 },
    { 0x3E0,  0x0C7, -1,    0 },
    { 0x400,  0x0C6, 0x0C6, 0 },
    { 0x790,  0x0C8, 0x0C8, 0 },
    { 0x7B0,  0x18F, 0x0C7, 0 },
    { 0x7D0,  0x18E, 0x18E, 0 },
    { 0x0B60, 0x190, 0x190, 0 },
    { 0x0B80, 0x257, 0x18F, 0 }
};

TileColor_t gTileColorTable[ 5 ] = {
    { 2, 3, 0 },
    { 3, 4, 1 },
    { 5, 6, 3 },
    { 6, 7, 4 },
    { 8, 9, 6 }
};

TileColor_t gTileUnk70[ 5 ] = {
    { 0, 3, 1 },
    { 2, 5, 3 },
    { 3, 6, 4 },
    { 5, 8, 6 },
    { 6, 9, 7 }
};

int gTileLight[ 80 * 41 ]; // 82x40 ?
int gTileUnk12[ 2 ][ 6 ];
int gTileUnk06[ 2 ][ 6 ];
char gTileUnk43[ 512 ];
char gTileUnk42[ 512 ];
char gTileRegions[ 32*16 ];
VidRect_t gTileMapArea;
VidRect_t gTileSurfArea;
char gTileUnk41[512];
int gTileGridOffsetY;
int gTileGridOffsetX;
int gTileScrOffsetX;
int gTileScrOffsetY;
void (*gTileMapUpdateCb)(VidRect_t *);
int gTileSurfCentY;
int gTileSurfCentX;
int gTileGridSize;
int gTileMapWidth;
int **gTileGrid;
char *gTileIsoSurface;
int gTileMapHeight;
int gTileSurfHeight;
int gTileGridCentX;
int gTileGridCentY;
int gTileMapSize;
int gTileGridHeight;
unsigned int gTileSurfPitch;
int gTileGridWidth;
int gTileSurfWidth;
int gTileCentIdx;

/*********************************************/

int TileInit( int **Grid, int GridW, int GridH, int MapW, int MapH, char *Surface, int SurfW, int SurfH, unsigned int SurfPitch, void (*UpdateCb)(VidRect_t *) )
{
    int n,i,j,k,m;
    char *sysex;

    gTileGridWidth = GridW;
    gTileGrid = Grid;
    gTileMapHeight = MapH;
    gTileGridHeight = GridH;
    gTileMapWidth = MapW;
    gTileMapSize = gTileMapWidth * gTileMapHeight;
    gTileIsoSurface = Surface;
    gTileSurfWidth = SurfW;
    gTileSurfHeight = SurfH;
    gTileSurfPitch = SurfPitch;
    gTileSurfArea.rt = SurfW - 1;
    gTileGridSize = GridH * GridW;
    gTileSurfArea.bm = gTileSurfHeight - 1;
    gTileSurfArea.lt = 0;
    gTileMapUpdateCb = UpdateCb;
    gTileSurfArea.tp = 0;
    gTileSketchMode = 0;

    gTileUnk12[ 0 ][ 0 ] = -1;
    gTileUnk12[ 0 ][ 1 ] = -MapW - 1;
    gTileUnk12[ 0 ][ 2 ] = MapW - 1;
    gTileUnk12[ 0 ][ 3 ] = -1;
    gTileUnk12[ 0 ][ 4 ] = MapW;
    gTileUnk12[ 0 ][ 5 ] = MapW;
    gTileUnk12[ 1 ][ 0 ] = MapW + 1;
    gTileUnk12[ 1 ][ 1 ] = 1;
    gTileUnk12[ 1 ][ 2 ] = 1;
    gTileUnk12[ 1 ][ 3 ] = 1 - MapW;
    gTileUnk12[ 1 ][ 4 ] = -MapW;
    gTileUnk12[ 1 ][ 5 ] = -MapW;

    gTileUnk06[ 0 ][ 0 ] = -1;
    gTileUnk06[ 0 ][ 1 ] = MapW - 1;
    gTileUnk06[ 0 ][ 2 ] = MapW;
    gTileUnk06[ 0 ][ 3 ] = MapW + 1;
    gTileUnk06[ 0 ][ 4 ] = 1;
    gTileUnk06[ 0 ][ 5 ] = -MapW;
    gTileUnk06[ 1 ][ 0 ] = -MapW - 1;
    gTileUnk06[ 1 ][ 1 ] = -1;
    gTileUnk06[ 1 ][ 2 ] = MapW;
    gTileUnk06[ 1 ][ 3 ] = 1;
    gTileUnk06[ 1 ][ 4 ] = 1 - MapW;
    gTileUnk06[ 1 ][ 5 ] = -MapW;

    n = 0;
    for( i = 0; i != 64; i += 16 ){
        for( j = 64; j; j -= 4 ) gTileRegions[ n++ ] = j > i ? 1 : 0;   
        for( j = 0; j != 64; j += 4 ) gTileRegions[ n++ ] = ( j > i ) ? 2 : 0;        
    }

    for( i = 0; i < 8; i++ ){
        for( j = 0; j < 32; j++ ){
            gTileRegions[ n++ ] = 0;
        }
    }
    for( k = 0; k != 64; k += 16 ){
        for( m = 0; m != 64; m += 4 ) gTileRegions[ n++ ] = ( m > k ) ? 0 : 3;
        for( j = 64; j; j -= 4 ){
            gTileRegions[ n++ ] = ( j > k ) ? 0 : 4;            
        }
    }

    ScrFillSolid( gTileUnk41, 32, 16, 32, 0 );
    ScrLine( gTileUnk41, 32, 16, 0,  31, 4,  gPalColorCubeRGB[4][4][4] );
    ScrLine( gTileUnk41, 32, 31, 4,  31, 12, gPalColorCubeRGB[4][4][4] );
    ScrLine( gTileUnk41, 32, 31, 12, 16, 15, gPalColorCubeRGB[4][4][4] );
    ScrLine( gTileUnk41, 32, 0,  12, 16, 15, gPalColorCubeRGB[4][4][4] );
    ScrLine( gTileUnk41, 32, 0,  4,  0,  12, gPalColorCubeRGB[4][4][4] );
    ScrLine( gTileUnk41, 32, 16, 0,  0,  4,  gPalColorCubeRGB[4][4][4] );

    ScrFillSolid( gTileUnk42, 32, 16, 32, 0 );
    ScrLine( gTileUnk42, 32, 16, 0,  31, 4,  gPalColorCubeRGB[0][0][31] );
    ScrLine( gTileUnk42, 32, 31, 4,  31, 12, gPalColorCubeRGB[0][0][31] );
    ScrLine( gTileUnk42, 32, 31, 12, 16, 15, gPalColorCubeRGB[0][0][31] );
    ScrLine( gTileUnk42, 32, 0,  12, 16, 15, gPalColorCubeRGB[0][0][31] );
    ScrLine( gTileUnk42, 32, 0,  4,  0,  12, gPalColorCubeRGB[0][0][31] );
    ScrLine( gTileUnk42, 32, 16, 0,  0,  4,  gPalColorCubeRGB[0][0][31] );

    ScrFillSolid( gTileUnk43, 32, 16, 32, 0 );
    ScrLine( gTileUnk43, 32, 16, 0,  31, 4,  gPalColorCubeRGB[31][0][0] );
    ScrLine( gTileUnk43, 32, 31, 4,  31, 12, gPalColorCubeRGB[31][0][0] );
    ScrLine( gTileUnk43, 32, 31, 12, 16, 15, gPalColorCubeRGB[31][0][0] );
    ScrLine( gTileUnk43, 32, 0,  12, 16, 15, gPalColorCubeRGB[31][0][0] );
    ScrLine( gTileUnk43, 32, 0,  4,  0,  12, gPalColorCubeRGB[31][0][0] );
    ScrLine( gTileUnk43, 32, 16, 0,  0,  4,  gPalColorCubeRGB[31][0][0] );

    for( i = 0; i < 16; i++ ){
        for( j = 31; j > 0 && !gTileUnk43[ i*32 + 31 - j ]; j-- );
        for( k = 0; k < 32 && !gTileUnk43[ i*32 + k ]; k++ );
        ScrLine( gTileUnk43, 32, k, i, j, i, gPalColorCubeRGB[31][0][0] );
    }

    TileSetCenter( gTileMapWidth * (gTileMapHeight / 2) + gTileMapWidth / 2, 2 ); // center of map
    TileMapAreaDefine( SurfW, SurfH, MapW, MapH );
    CfgGetString( &gConfiguration, "system", "executable", &sysex );
    if( !strcasecmp( sysex, "mapper" ) ) gTileRedrawCb = TileRenderIsoMapper;
    return 0;
}

void TileMapAreaDefine( int ScrW, int ScrH, int MapW, int MapH )
{
    gTileMapArea.lt = abs32( MapW - (TileGetPointed( -320, 240 + ScrH ) % MapW) - gTileGridCentX - 1 ) + 6;
    gTileMapArea.tp = abs32( gTileGridCentY - TileGetPointed( -320, -240 ) / MapW ) + 7;
    gTileMapArea.rt = MapW - gTileMapArea.lt - 1;
    gTileMapArea.bm = MapH - gTileMapArea.tp - 1;
    if( (gTileMapArea.lt & 1) == 0 ) gTileMapArea.lt++;
    if( (gTileMapArea.rt & 1) == 0 ) gTileMapArea.lt--;
    gTileMapAreaDefined = 1;
}

int TileUnk03()
{
    return gTileUpdateEnable;
}

void TileUpdateDisable()
{
    gTileUpdateEnable = 0;
}

void TileUpdateEnable()
{
    gTileUpdateEnable = 1;
}

void TileUpdateArea( VidRect_t *Area, int MapLvl )
{
    if( !gTileUpdateEnable ) return;    
    if( MapLvl == gCurrentMapLvl ) gTileRedrawCb( Area, MapLvl );
}

void TileUpdate()
{
    if( gTileUpdateEnable ) gTileRedrawCb( &gTileSurfArea, gCurrentMapLvl );
}

int TileSetCenter( int MapIdx, int MapLvl )
{
    int xgrid, ygrid, x0, x1, y0, y1;

    if( MapIdx < 0 || MapIdx >= gTileMapSize ) return -1;
    if( (gTileUnk63 & ((MapLvl & 2) == 0 )) != 0 ){
        TileGetScrCoordinates( MapIdx, &x0, &y0 );
        TileGetScrCoordinates( gObjDude->GridId, &x1, &y1 );
        x0 = abs32( x1 - x0 );
        y0 = abs32( y1 - y0 );
        if( (x0 > abs32( x1 - gTileSurfCentX ) || y0 > abs32( y1 - gTileSurfCentY )) && (x0 >= 480 || y0 >= 400) ) return -1;
    }

    if( ( gTileUnk62 & MapLvl & 2 ) == 0 ){
        if( !ObjUnk57( MapIdx, gCurrentMapLvl ) ) return -1;
    }
    xgrid = gTileMapWidth - ( MapIdx % gTileMapWidth ) - 1;
    ygrid = MapIdx / gTileMapWidth;

    if( gTileMapAreaDefined ){
        if( xgrid <= gTileMapArea.lt || xgrid >= gTileMapArea.rt || ygrid <= gTileMapArea.tp || ygrid >= gTileMapArea.bm ) return -1;
    }

    gTileGridCentY = ygrid;
    gTileSurfCentX = (gTileSurfWidth - 32) / 2;
    gTileGridCentX = xgrid;
    gTileSurfCentY = (gTileSurfHeight - 16) / 2;

    if( xgrid & 0x01 ){
        gTileGridCentX = xgrid - 1;
        gTileSurfCentX -= 32;
    }

    gTileGridOffsetX = gTileGridCentX / 2;
    gTileGridOffsetY = gTileGridCentY / 2;
    gTileScrOffsetX = gTileSurfCentX - 16;
    gTileScrOffsetY = gTileSurfCentY - 2;
    if( gTileGridCentY & 0x01 ){
        gTileScrOffsetY -= 12;
        gTileScrOffsetX -= 16;
    }
    gTileCentIdx = MapIdx;
    if( MapLvl & 0x01 ){
        if( gTileUpdateEnable ) gTileRedrawCb( &gTileSurfArea, gCurrentMapLvl );
    }
    return 0;
}

void TileRenderIsoMapper( VidRect_t *Area, int a2 )
{
    VidRect_t tmp;

    if( RegionShrink( Area, &gTileSurfArea, &tmp ) == -1 ) return;    
    ScrFillSolid( gTileIsoSurface + tmp.lt + gTileSurfPitch * tmp.tp, tmp.rt - tmp.lt + 1, tmp.bm - tmp.tp + 1, gTileSurfPitch, 0 );
    TileRenderRoof( &tmp, a2 );
    TileRenderSketch( &tmp, a2 );
    ObjRenderObjects( &tmp, a2 );
    TileRenderRoof( &tmp, a2 );
    ObjRenderCursor( &tmp );
    gTileMapUpdateCb( &tmp );
}

void TileRenderIso( VidRect_t *area, int MapLvl )
{
    VidRect_t tmp;

    if( RegionShrink( area, &gTileSurfArea, &tmp ) == -1 ) return;
    TileRenderFloor( &tmp, MapLvl );
    ObjRenderObjects( &tmp, MapLvl );
    TileRenderRoof( &tmp, MapLvl );
    ObjRenderCursor( &tmp );
    gTileMapUpdateCb( &tmp );
}

void TileUnk09( int result )
{
    gTileUnk60 = 1 - gTileUnk60;
    if( !result ) return;    
    if( gTileUpdateEnable ) gTileRedrawCb( &gTileSurfArea, gCurrentMapLvl );    
}

int TileUnk10()
{
    return gTileUnk60;
}

int TileGetScrCoordinates( int MapIdx, int *pX, int *pY )
{
    int xg, yg;

    if( MapIdx < 0 || MapIdx >= gTileMapSize ) return -1;
    xg = gTileMapWidth - (MapIdx % gTileMapWidth) - 1;
    yg = MapIdx / gTileMapWidth;
    *pX = gTileSurfCentX + 48 * ((xg - gTileGridCentX) / 2) + 16 * (yg - gTileGridCentY);
    *pY = gTileSurfCentY - 12 * ((xg - gTileGridCentX) / 2) + 12 * (yg - gTileGridCentY);
    if( xg & 0x01 ){
        if( xg <= gTileGridCentX ){
            *pX -= 16;
            *pY += 12;
        } else {
            *pX += 32;
        }
    }
    return 0;
}

int TileGetPointed( int x, int y )
{
    int yi,xi,yo,xo,n,b,j,k,yg;

    y -= gTileSurfCentY;
    if( y >= 0 )
        yg = y / 12;
    else
        yg = (y + 1) / 12 - 1;
    k = x - gTileSurfCentX - 16 * yg;
    n = y - 12 * yg;
    if ( k >= 0 ){
	j = k / 64;
    } else {
        j = ( k + 1 ) / 64 - 1;
    }
    yi = j + yg;
    xi = k - (j * 64 );
    b = 2 * j;
    if( xi >= 32 ){
        xi -= 32;
        b++;
    }
    yo = gTileGridCentY + yi;
    xo = gTileGridCentX + b;

    switch( gTileRegions[ 32 * n + xi ] ){ // 32x16
        case 1: yo--; break;
        case 2: xo++; if( (xo & 1) != 0 ) yo--; break;
        case 3: xo--; if( (xo & 1) == 0 ) yo++; break;
        case 4: yo++; break;
    }

    xo = gTileMapWidth - 1 - xo;
    if( xo < 0 || xo >= gTileMapWidth || yo < 0 || yo >= gTileMapHeight ) return -1;
    return gTileMapWidth * yo + xo;    
}

int TileGetDistance( int MapIdx1, int MapIdx2 )
{
    long double dbl;
    int i,v7,sy,sx,pY,pX,v12,v14;

    sy = sx = 0;
    if( MapIdx1 == -1 || MapIdx2 == -1 ) return 9999;
    TileGetScrCoordinates( MapIdx2, &sx, &sy );
    for( i = 0; MapIdx1 != MapIdx2; i++ ){
        TileGetScrCoordinates( MapIdx1, &pX, &pY );
        v14 = sx - pX;
        if( sx == pX ){
            v7 = ( sy - pY < 0 ) ? 0 : 2;
        } else {
            v12 = pY - sy;
            dbl = atan2( pY - sy, v14 );
            v12 = lround( dbl * 180.0 * 0.3183098862851122 );
            v7 = (360 - (v12 + 180) - 90 + (360 - (v12 + 180) - 90 < 0 ? 360 : 0)) / 60;
            if( v7 >= 6 ) v7 = 5;
        }
        MapIdx1 += gTileUnk06[(MapIdx1 % gTileMapWidth) & 1][v7];
    }
    return i;
}

int TileUnk14( int MapIdx1, int MapIdx2 )
{
    int y0,x0,y1,x1;

    TileGetScrCoordinates( MapIdx1, &y0, &x0 );
    TileGetScrCoordinates( MapIdx2, &y1, &x1 );
    x1 -= x0;
    y1 -= y0;
    return (double)y1 <= (double)x1 * -4.0;
}

int TileUnk15( int MapIdx1, int MapIdx2 )
{
    int y0,x0,y1,x1;

    TileGetScrCoordinates( MapIdx1, &y0, &x0 );
    TileGetScrCoordinates( MapIdx2, &y1, &x1 );
    x1 -= x0;
    y1 -= y0;
    return (double)y1 <= (double)x1 * 1.333333333333333;
}

int TileUnk16( int MapIdx, int Orientation, int TilesCnt )
{
    int i;

    for( i = 0; i < TilesCnt; i++ ){
        if( TileGridBorder( MapIdx ) ) break;
        MapIdx += gTileUnk06[ (MapIdx % gTileMapWidth) & 0x01][ Orientation ];
    }
    return MapIdx;
}

int TileTurnAt( int MapIdx1, int MapIdx2 )
{
    int ori, x0, y1, y0, deg, x1;

    TileGetScrCoordinates( MapIdx1, &x0, &y0 );
    TileGetScrCoordinates( MapIdx2, &x1, &y1 );
    x1 -= x0;
    y1 -= y0;
    if( x1 ){
        deg = (int)lround( atan2( -y1, x1 ) * 180.0 / M_PI ); // radians to degree
        ori = (360 - (deg + 180) - 90 + (360 - (deg + 180) - 90 < 0 ? 360 : 0)) / 60;
        return ( ori >= 6 ) ? 5 : ori;
    } else if( y1 < 0 ) {
        return 0;
    }
    return 2;
}

int TileUnk18( int a1, int a2, int a3 )
{
    int v3, v5, v6, v11, v12, y2, x2, y1, x1, v22, v23, v25, v26, v27, v28;

    v3 = a1;
    v25 = a3;
    if( a3 <= 0 || a1 == a2 ) return a1;    
    TileGetScrCoordinates(a1, &x1, &y1);
    x1 += 16;
    y1 += 8;
    v28 = v3;
    TileGetScrCoordinates(a2, &x2, &y2);
    v5 = x2 + 16 - x1;
    x2 += 16;
    y2 += 8;
    v6 = 0;
    v27 = 2 * abs32(v5);
    if( v5 >= 0 )
        v22 = v5 > 0;
    else
        v22 = -1;
    v26 = 2 * abs32(y2 - y1);
    if( y2 - y1 >= 0 )
        v23 = y2 - y1 > 0;
    else
        v23 = -1;

    if( v27 > v26 ){
        for( v11 = v26 - (v27 / 2 ); ; v11 += v26, x1 += v22 ){
            v12 = TileGetPointed( x1, y1 );
            if( v12 != v28 ){
                if( ++v6 == v25 || TileGridBorder( v12 ) ) break;
                v28 = v12;
            }
            if( v11 >= 0 ){ v11 -= v27; y1 += v23; }            
        }
    } else {
	for( v11 = v27 - (v26 / 2); ;v11 += v27, y1 += v23 ){
    	    v12 = TileGetPointed( x1, y1 );
    	    if( v12 != v28 ){
        	if( ++v6 == v25 || TileGridBorder( v12 ) ) break;
        	v28 = v12;
    	    }
    	    if( v11 >= 0 ){ v11 -= v26; x1 += v22; }        
	}    
    }
    return v12;
}

int TileGridBorder( int MapIdx )
{
    if( MapIdx < 0 || MapIdx >= gTileMapSize ) return 0;
    if( MapIdx < gTileMapWidth ) return 1;
    if( MapIdx >= gTileMapSize - gTileMapWidth ) return 1;
    if( !(MapIdx % gTileMapWidth) ) return 1;
    return MapIdx % gTileMapWidth == gTileMapWidth - 1;
}

void TileUnk20()
{
    gTileUnk62 = 1;
}

void TileUnk21()
{
    gTileUnk62 = 0;
}

int TileUnk22()
{
    return gTileUnk62;
}

void TileUnk23()
{
    gTileUnk63 = 1;
}

void TileUnk24()
{
    gTileUnk63 = 0;
}

int TileUnk25()
{
    return gTileUnk63;
}

void TileGetScrPosition( int GridIdx, int *pXpos, int *pYpos )
{
    int x,y;

    if( GridIdx < 0 || GridIdx >= gTileGridSize ) return;    
    x = gTileGridWidth - (GridIdx % gTileGridWidth) - 1;
    y = GridIdx / gTileGridWidth;
    x -= gTileGridOffsetX;
    y -= gTileGridOffsetY;
    *pXpos = gTileScrOffsetX + 48 * x + 32 * y;
    *pYpos = gTileScrOffsetY - 12 * x + 24 * y;    
}

int TileGetScrPositionA( int GridIdx, int *pXpos, int *pYpos )
{
    int x, y;

    if( GridIdx < 0 || GridIdx >= gTileGridSize ) return -1;
    x = gTileGridWidth - (GridIdx % gTileGridWidth) - 1;
    y = GridIdx / gTileGridWidth;
    x -= gTileGridOffsetX;
    y -= gTileGridOffsetY;
    *pXpos = gTileScrOffsetX + 48 * x + 32 * y;
    *pYpos = gTileScrOffsetY - 12 * x + 24 * y - 96;
    return 0;
}

int TileGetGridIdx( int x, int y )
{
    int xg, yg;

    TileScrToGridPos( x, y, &xg, &yg );
    if( xg < 0 || xg >= gTileGridWidth || yg < 0 || yg >= gTileGridHeight ) return -1;
    return xg + gTileGridWidth * yg;    
}

int TileGetGridIdxA( int x, int y )
{
    int xg, yg;

    TileScrToGridPosA( x, y, &xg, &yg );
    if( xg < 0 || xg >= gTileGridWidth || yg < 0 || yg >= gTileGridHeight ) return -1;
    return xg + gTileGridWidth * yg;    
}

void TileScrToGridPos( int x, int y, int *pGridX, int *pGridY )
{
    *pGridY = 4 * ( y - gTileScrOffsetY - 12 );
    *pGridX = 3 * ( x - gTileScrOffsetX ) - *pGridY;
    *pGridY += x - gTileScrOffsetX;

    *pGridX = ( *pGridX < 0 ) ? ((*pGridX + 1) / 192 - 1) : *pGridX / 192;
    *pGridY = ( *pGridY < 0 ) ? ((*pGridY + 1) / 128 - 1) : *pGridY / 128;

    *pGridY = *pGridY + gTileGridOffsetY;
    *pGridX = gTileGridWidth - ( *pGridX + gTileGridOffsetX ) - 1;
}

void TileScrToGridPosA( int x, int y, int *pGridX, int *pGridY )
{
    *pGridY = 4 * (y + 96 - gTileScrOffsetY - 12);
    *pGridX = 3 * (x - gTileScrOffsetX) - *pGridY;
    *pGridY += x - gTileScrOffsetX;

    *pGridX = ( *pGridX < 0 ) ? (( *pGridX + 1 ) / 192 - 1) : *pGridX / 192;
    *pGridY = ( *pGridY < 0 ) ? (( *pGridY + 1 ) / 128 - 1) : *pGridY / 128;

    *pGridY = *pGridY + gTileGridOffsetY;
    *pGridX = gTileGridWidth - ( *pGridX + gTileGridOffsetX ) - 1;
}

void TileRenderRoof( VidRect_t *Area1, int MapLvl )
{
    int i,GridIdx,Id,y1,x1,y0,x0,tmp,Ypos,Xpos,light;
    unsigned int ObjId;

    if( !gTileUnk60 ) return;    
    TileScrToGridPosA( Area1->lt, Area1->tp, &tmp, &y0 );
    TileScrToGridPosA( Area1->rt, Area1->tp, &x0, &tmp );
    TileScrToGridPosA( Area1->lt, Area1->bm, &x1, &tmp );
    TileScrToGridPosA( Area1->rt, Area1->bm, &tmp, &y1 );
    if( x0 < 0 ) x0 = 0;
    if( x0 >= gTileGridWidth ) x0 = gTileGridWidth - 1;
    if( y0 < 0 ) y0 = 0;
    if( x0 >= gTileGridHeight ) y0 = gTileGridHeight - 1;
    light = ItemMapGetLight();
    tmp = gTileGridWidth * y0;
    for( ;y0 <= y1; y0++, tmp += gTileGridWidth ){
        for( i = x0; i <= x1; i++ ){
            GridIdx = i + tmp;
            ObjId = gTileGrid[ MapLvl ][ GridIdx ] >> 16;            
            if( ObjId & 0x1000 ) continue;
            Id = ArtMakeId( 4, ObjId & 0xFFF, 0, 0, 0 );
            if( Id == ArtMakeId( 4, 1, 0, 0, 0 ) ) continue;
            TileGetScrPositionA( GridIdx, &Xpos, &Ypos );
            TileDrawRoof( Id, Xpos, Ypos, Area1, light );
        }        
    }
}

void TileRoofCover( int GridX, int GridY, int lvl )
{
    int *pTile, Flags;

    while( 1 ){
        if( GridX < 0 || GridX >= gTileGridWidth ) break;
        if( GridY < 0 || GridY >= gTileGridHeight ) break;
        pTile = &gTileGrid[ lvl ][ GridX + GridY * gTileGridWidth ];
        Flags = *pTile >> 16;
        if( ArtMakeId( 4, 1, 0, 0, 0 ) == ArtMakeId( 4, Flags & 0xFFF, 0, 0, 0 ) ) break;
        if( !(Flags & 0x1000) ) break;
        *pTile &= ~0x10000000;
        TileRoofCover( GridX - 1, GridY, lvl );
        TileRoofCover( GridX + 1, GridY, lvl );
        TileRoofCover( GridX, GridY++ - 1, lvl );
    }
}

void TileRoofVisible( int GridX, int GridY, int lvl, int Visible )
{
    if( Visible )
        TileRoofCover( GridX, GridY, lvl );
    else
        TileRoofUncover( GridX, GridY, lvl );
}

void TileRoofUncover( int GridX, int GridY, int lvl )
{
    int *pTile, Flags;

    while( 1 ){
        if( GridX < 0 || GridX >= gTileGridWidth ) break;
        if( GridY < 0 || GridY >= gTileGridHeight ) break;
        pTile = &gTileGrid[ lvl ][ GridX + GridY * gTileGridWidth ];
        Flags = *pTile >> 16;
        if( ArtMakeId( 4, 1, 0, 0, 0 ) == ArtMakeId( 4, Flags & 0xFFF, 0, 0, 0 ) ) break;
        if( Flags & 0x3000 ) break;
        *pTile |= 0x10000000;
        TileRoofUncover( GridX - 1, GridY, lvl );
        TileRoofUncover( GridX + 1, GridY, lvl );
        TileRoofUncover( GridX, GridY - 1, lvl );
        GridY++;
    }
}

void TileDrawRoof( int ImgId, int Xpos, int Ypos, VidRect_t *a4, int light )
{
    ArtFrmHdr_t *Art1;
    int ObjWidth;
    int ObjHeight;
    ArtFrmHdr_t *Art2;
    int i;
    VidRect_t reg[4];
    VidRect_t AreaOut;
    VidRect_t b;
    VidRect_t tmp;
    CachePool_t *Obj;
    CachePool_t *Block;
    int RadH;
    int RadW;
    int SrcW;
    int SrcH;
    char *TilePix;
    char *p, *q;
    int w, h;
    int lt,rt,bm,tp;

    if( !( Art1 = ArtLoadImg( ImgId, &Obj ) ) ) return;
    ObjWidth = ArtGetObjWidth( Art1, 0, 0 );
    ObjHeight = ArtGetObjHeight( Art1, 0, 0 );
    tmp.lt = Xpos;
    tmp.tp = Ypos;
    tmp.rt = ObjWidth - 1 + Xpos;
    tmp.bm = ObjHeight - 1 + Ypos;
    if( RegionShrink( &tmp, a4, &tmp ) ){ ArtClose( Obj ); return; }
    TilePix = ArtGetObjData( Art1, 0, 0 ) + tmp.lt - Xpos + ObjWidth * ( tmp.tp - Ypos );
    SrcW = tmp.rt - tmp.lt + 1;
    SrcH = tmp.bm - tmp.tp + 1;
    if( !(Art2 = ArtLoadImg( gObjRadius->ImgId, &Block )) ){ ArtClose( Obj ); return; }    
    RadW = ArtGetObjWidth( Art2, 0, 0 );
    RadH = ArtGetObjHeight( Art2, 0, 0 );
    TileGetScrCoordinates( gObjRadius->GridId, &b.lt, &b.tp );
    b.lt = b.lt + Art2->PixShiftX[ 0 ] + 16;
    b.tp = b.tp + Art2->PixShiftY[ 0 ] + 8;
    b.lt = b.lt + gObjRadius->PosX;    
    b.bm = b.tp + gObjRadius->PosY;
    b.lt = b.lt - RadW / 2;    
    b.tp = b.bm - RadH + 1;
    b.rt = b.lt + RadW - 1;
    gObjRadius->Sx = b.lt;
    gObjRadius->Sy = b.tp;
    if( RegionShrink( &b, &tmp, &AreaOut ) ){
        ObjRenderNormal( TilePix, SrcW, SrcH, ObjWidth, gTileIsoSurface, tmp.lt, tmp.tp, gTileSurfPitch, light );
	ArtClose( Block );
	ArtClose( Obj );
	return;
    }

    // -- areas sorrounded object --
    // upper area
    reg[0].lt = tmp.lt;
    reg[0].tp = tmp.tp;
    reg[0].rt = tmp.rt;
    reg[0].bm = AreaOut.tp - 1;
    // left area
    reg[1].lt = tmp.lt;
    reg[1].tp = AreaOut.tp;
    reg[1].rt = AreaOut.lt - 1;
    reg[1].bm = AreaOut.bm;
    // right area
    reg[2].lt = AreaOut.rt + 1;
    reg[2].tp = AreaOut.tp;
    reg[2].rt = tmp.rt;
    reg[2].bm = AreaOut.bm;
    // bottom area
    reg[3].lt = tmp.lt;
    reg[3].tp = AreaOut.bm + 1;
    reg[3].rt = tmp.rt;        
    reg[3].bm = tmp.bm;

    for( i = 0; i < 4; i++){
        lt = reg[ i ].lt; rt = reg[ i ].rt;
        bm = reg[ i ].bm; tp = reg[ i ].tp;
        if( lt <= rt && tp <= bm ){
            ObjRenderNormal( &TilePix[ ObjWidth * (tp - tmp.tp) + lt - tmp.lt ], rt - lt + 1, bm - tp + 1, ObjWidth, gTileIsoSurface, lt, tp, gTileSurfPitch, light );
        }                        
    }

    // object itself
    w = AreaOut.rt - AreaOut.lt + 1;
    h = AreaOut.bm - AreaOut.tp + 1;
    p = &TilePix[ ObjWidth * (AreaOut.tp - tmp.tp) + AreaOut.lt - tmp.lt ];
    q = ArtGetObjData( Art2, 0, 0 );
    ObjRenderMix( p, w, h, ObjWidth, &gTileIsoSurface[gTileSurfPitch * AreaOut.tp + AreaOut.lt], 
    	    gTileSurfPitch, q + (AreaOut.tp - b.tp) * RadW + AreaOut.lt - b.lt, RadW, light );

    ArtClose( Block );
    ArtClose( Obj );
}

void TileRenderFloor( VidRect_t *area, int MapLvl )
{
    int i,y1,x1,y0,x0,tmp,j, tx, ty;
    int tile, id, GridIdx;

    TileScrToGridPos( area->lt, area->tp, &tmp, &y0 );
    TileScrToGridPos( area->rt, area->tp, &x0, &tmp );
    TileScrToGridPos( area->lt, area->bm, &x1, &tmp );
    TileScrToGridPos( area->rt, area->bm, &tmp, &y1 );
    if( x0 < 0 ) x0 = 0;
    if( x0 >= gTileGridWidth ) x0 = gTileGridWidth - 1;
    if( y0 < 0 )  y0 = 0;
    if( y0 >= gTileGridHeight ) y0 = gTileGridHeight - 1;
    ItemMapGetLight();
    for( j = y0; j <= y1; j++ ){
        for( i = x0; i <= x1; i++ ){
    	    GridIdx = gTileGridWidth * j + i;
            tile = gTileGrid[ MapLvl ][ GridIdx ];
            if( tile & 0x1000 ) continue;
            TileGetScrPosition( GridIdx, &tx, &ty );
	    id = ArtMakeId( 4, tile & 0xFFF, 0, 0, 0 );
            TileDrawTile( id, tx, ty, area );
        }            
    }
}

int TilePointOpaque( int x, int y, int MapLvl )
{
    ArtFrmHdr_t *img;
    CachePool_t *ImgObj;
    unsigned int Tile, Flags;
    int yg,xg,lvl,k,idx;
    char *dat;

    k = 0;
    if( !gTileUnk60 ) return 0;    
    TileScrToGridPosA( x, y, &xg, &yg );
    lvl = MapLvl;
    idx = gTileGridWidth * yg + xg;
    Tile = gTileGrid[ lvl ][ idx ];
    Flags = Tile >> 16;
    if( ArtMakeId( 4, 1, 0, 0, 0 ) == ArtMakeId( 4, Flags & 0xFFF, 0, 0, 0 ) ) return 0;    
    if( Flags & 0x1000 ) return 0; // object is unvisible
    if( !(img = ArtLoadImg( ArtMakeId( 4, Flags & 0xFFF, 0, 0, 0 ), &ImgObj ) ) ) return 0;
    if( (dat = ArtGetObjData( img, 0, 0 ) ) ){
        TileGetScrPositionA( idx, &xg, &yg );
        if( dat[ (y - yg) * ArtGetObjWidth( img, 0, 0 ) + (x - xg) ] ) k = 1;
    }
    ArtClose( ImgObj );
    return k;
}

void TileUnk39( VidRect_t *Region, int *a2, int *a3, int a4 )
{
    VidRect_t area, AreaOut;
    int idx,x,y1,x1,y0,tmp,x0,y;

    if( a4 <= 0 ) return;
    TileScrToGridPos( Region->lt, Region->tp, &tmp, &y0 );
    TileScrToGridPos( Region->rt, Region->tp, &x0, &tmp );
    TileScrToGridPos( Region->lt, Region->bm, &x1, &tmp );
    TileScrToGridPos( Region->rt, Region->bm, &tmp, &y1 );
    if( x0 < 0 ) x0 = 0;
    if( x0 >= gTileGridWidth ) x0 = gTileGridWidth - 1;
    if( y0 < 0 ) y0 = 0;
    if( y0 >= gTileGridHeight ) y0 = gTileGridHeight - 1;    
    tmp = gTileGridWidth * y0;
    idx = 0;
    for( y = y0; y <= y1; y++, tmp += gTileGridWidth ){
    	for( x = x0; x <= x1; x++ ){
    	    TileGetScrPosition( x + tmp, &area.lt, &area.tp );
    	    area.rt = area.lt + 79;
    	    area.bm = area.tp + 35;
    	    if( !RegionShrink( &area, Region, &AreaOut ) && area.lt == AreaOut.lt && area.tp == AreaOut.tp && area.rt == AreaOut.rt && area.bm == AreaOut.bm ){
        	a3[ idx++ ] = x + tmp;
        	if( idx == a4 ) break;
    	    }
    	}                    
    }
}

void TileSketchToggle()
{
    gTileSketchMode = 1 - gTileSketchMode;
}

void TileSketchOn()
{
    gTileSketchMode = 1;
}

void TileSketchOff()
{
    gTileSketchMode = 0;
}

int TileSketch()
{
    return gTileSketchMode;
}

void TileRenderSketch( VidRect_t *area, int a2 )
{
    int i, j;

    if( !gTileSketchMode ) return;

    for( i = area->tp - 12; i < area->bm + 12; i += 6 ){
	for( j = area->lt - 32; j < area->rt + 32; j += 16 ){
            TileSketchDraw( TileGetPointed( j, i ), a2, area );
        }
    }    
}

void TileSketchIso( int MapIdx, int a2 )
{
    VidRect_t rect;

    TileGetScrCoordinates( MapIdx, &rect.lt, &rect.tp );
    rect.rt = rect.lt + 31;
    rect.bm = rect.tp + 15;
    if( RegionShrink( &rect, &gTileSurfArea, &rect ) == -1 ) return;
    TileSketchDraw( MapIdx, a2, &rect );
    gTileMapUpdateCb( &rect );
}

void TileSketchDraw( int GridIdx, int MapLvl, VidRect_t *a3 )
{
    int v5;
    int v6;
    int v7;
    VidRect_t Alpha;
    int w;
    int v10;
    
    if( GridIdx == -1 ) return;    
    TileGetScrCoordinates( GridIdx, &v10, &w );
    Alpha.lt = v10;
    Alpha.tp = w;
    Alpha.rt = v10 + 31;
    Alpha.bm = w + 15;
    if( RegionShrink( &Alpha, a3, &Alpha ) == -1 ) return;    
    if( ObjReach( 0, GridIdx, MapLvl ) ){
        ScrCopyAlpha( // hex na czerwono
    	    &gTileUnk43[ 32 * (Alpha.tp - w) + Alpha.lt - v10 ], 
    	    Alpha.rt - Alpha.lt + 1, Alpha.bm - Alpha.tp + 1, 32, &gTileIsoSurface[ gTileSurfPitch * Alpha.tp + Alpha.lt ], gTileSurfPitch
    	);
    } else {
        v5 = ObjUnk53(GridIdx, MapLvl);
        v6 = gTileSurfPitch * Alpha.tp;
        v7 = 32 * (Alpha.tp - w);
        if( v5 ) // obwiednia hexa na niebiesko
            ScrCopyAlpha( &gTileUnk42[ v7 + Alpha.lt - v10 ], 
        	Alpha.rt - Alpha.lt + 1, Alpha.bm - Alpha.tp + 1, 32, &gTileIsoSurface[ v6 + Alpha.lt ], gTileSurfPitch
    	    );
        else
            ObjUnk63( &gTileUnk41[v7 + Alpha.lt - v10 ], 
        	Alpha.rt - Alpha.lt + 1, Alpha.bm - Alpha.tp + 1, 32, &gTileIsoSurface[ v6 + Alpha.lt ], 0, 0, gTileSurfPitch, gObjShadeWall, gObjPalBY
    	    );
    }        
}

void TileDrawTile( int ArtId, int Xpos, int Ypos, VidRect_t *area )
{
    int lt,tp,TileH,v10,v11,v12,v13,v14,Light,v19,v28,v31,v32,*v33,v37,j,v73,v74;
    int Height,Width,yg,xg,TileW,dtile,dlight,tmp;
    unsigned int *pLight,dscr,i;
    TileColor_t *v25;
    char *pScr;
    unsigned char *pTile;
    CachePool_t *ImgObj;
    ArtFrmHdr_t *Image;


    if( ArtIsHidden( OBJTYPE( ArtId ) ) ) return;
    if( !(Image = ArtLoadImg( ArtId, &ImgObj )) ) return;
    lt = area->lt;
    tp = area->tp;
    v74 = area->rt - area->lt + 1;
    v73 = area->bm - area->tp + 1;

    if( area->lt < 0 ) lt = 0;
    if( tp < 0 ) tp = 0;
    if( lt + v74 > gTileSurfWidth ) v74 = gTileSurfWidth - lt;
    if( tp + v73 > gTileSurfHeight ) v73 = gTileSurfHeight - tp;
    if( Xpos >= gTileSurfWidth || Xpos > area->rt || Ypos >= gTileSurfHeight || Ypos > area->bm ){ ArtClose( ImgObj ); return; }
    TileW = ArtGetObjWidth( Image, 0, 0 );
    TileH = ArtGetObjHeight( Image, 0, 0 );
    if( lt < Xpos ){
        xg = 0;
        v11 = v74 + lt;
        Width = TileW + Xpos <= v11 ? TileW : v11 - Xpos;
    } else {
        v10 = Xpos;
        Xpos = lt;
        xg = lt - v10;
        Width = TileW - (lt - v10);
        if( Width > v74 ) Width = v74;
    }
    v12 = Ypos;
    if( tp < Ypos ){
        v13 = v73 + tp;
        yg = 0;
        Height = Ypos + TileH <= v13 ? TileH : v13 - Ypos;
    } else {
        Ypos = tp;
        yg = tp - v12;
        Height = TileH - (tp - v12);
        if( Height > v73 ) Height = v73;
    }
    if( Width <= 0 || Height <= 0 ){ ArtClose( ImgObj ); return; }
    v14 = TileGetPointed( Xpos, Ypos + 13 );
    if( v14 != -1 ){
        Light = ItemMapGetLight();        
        v19 = v14 & 0x01;
        for( i = 0; i != 10; v19++, i++ ){
    	    tmp = ItemGridGetLightA( gCurrentMapLvl, v14 + gTileShade[ v19 ].i02 );
            gTileShade[ i ].i04 = (tmp <= Light ) ? Light : tmp;
        }
        for( i = 0; i < 9; i++ ){
            if( gTileShade[ i + 1 ].i04 != gTileShade[ i ].i04 ) break; 
        }
        if( i == 9 ){ // no light effect
            ObjRenderNormal( ArtGetObjData( Image, 0, 0 ) + yg * TileW + xg, Width, Height, TileW, gTileIsoSurface, Xpos, Ypos, gTileSurfPitch, gTileShade[ 0 ].i04 );
            ArtClose( ImgObj ); 
            return;
        }
        // render light source 'shadows'
        // upper triangle
        for( v25 = gTileColorTable; v25 != &gTileColorTable[ 5 ]; v25++ ){    	    
            v28 = gTileShade[ v25->b ].i04;
            v31 = ( gTileShade[ v25->g ].i04 - gTileShade[ v25->r ].i04 ) / 32;
            v32 = ( gTileShade[ v25->r ].i04 - gTileShade[ v25->b ].i04 ) / 13;
            v33 = &gTileLight[ gTileShade[ v25->b ].i01 ];
            if( v31 ){
                if( v32 ){ // v31 && v32
                    for( i = 0; i < 13; i++, v28 += v32 ){
                        v37 = v28;
                        v33 += gTileUnk71[ i ].i01;
                        for( j = 0; j < gTileUnk71[ i ].Width; v37 += v31, j++ ) *v33++ = v37;
                }
                } else {
                    for( i = 0; i != 13; i++ ){
                        v37 = v28;
                        v33 += gTileUnk71[ i ].i01;
                        for( j = 0; j < gTileUnk71[ i ].Width; v37 += v31, j++ ) *v33++ = v37;
                    }
                }
            } else if( v32 ){
                 for( i = 0; i != 13; i++, v28 += v32 ){
                    v33 += gTileUnk71[ i ].i01;
                    for( j = 0; j < gTileUnk71[ i ].Width; j++ ) *v33++ = v28;
                }
            } else {
                for( i = 0; i != 13; i++ ){
                    v33 += gTileUnk71[ i ].i01;
                    for( j = 0; j < gTileUnk71[ i ].Width; j++, v33++ ) *v33 = v28;
                }
            } 
        }
        // lower triangle
        for( v25 = gTileUnk70; v25 != &gTileUnk70[ 5 ]; v25++ ){
            v28 = gTileShade[ v25->r ].i04;
            v31 = ( gTileShade[ v25->b ].i04 - v28 ) / 32;
            v32 = ( gTileShade[ v25->g ].i04 - v28 ) / 13;
            v33 = &gTileLight[ gTileShade[ v25->r ].i01 ];
            if( v31 ){
                if( v32 ){
                    for( i = 0; i != 13; i++, v28 += v32 ){
                        v37 = v28;
                        v33 += gTileUnk65[ i ].i01;
                        for( j = 0; j < gTileUnk65[ i ].Width; v37 += v31, j++ ) *v33++ = v37;
                    }
                } else {
                    for( i = 0; i != 13; i++ ){
                        v37 = v28;
                        v33 += gTileUnk65[ i ].i01;
                        for( j = 0; j < gTileUnk65[ i ].Width; v37 += v31, j++ ) *v33++ = v37;
                    }
                }
            } else if( v32 ){
                for( i = 0; i != 13; i++, v28 += v32 ){
                    v33 += gTileUnk65[ i ].i01;
                    for( j = 0; j < gTileUnk65[ i ].Width; j++ ) *v33++ = v28;
                }
            } else {
                for( i = 0; i != 13; i++ ){
                    v33 += gTileUnk65[ i ].i01;
                    for( j = 0; j < gTileUnk65[ i ].Width; j++) *v33++ = v28;
                }
            }
        }

    }
    // blit tile to window surface
    pScr = Ypos * gTileSurfPitch + gTileIsoSurface + Xpos;
    pTile = (unsigned char *)ArtGetObjData( Image, 0, 0 ) + yg * TileW + xg;

    pLight = (unsigned int *)&gTileLight[ 80 * yg + xg + 160 ];

    dtile = TileW - Width;
    dscr = gTileSurfPitch - Width;
    dlight = 80 - Width;
    while( --Height != -1 ){
        for( i = 0; i < Width; i++, pTile++,  pScr++, pLight++ ){
    	    if( *pTile == 0 ) continue; // transparency 
            *pScr = gPalShades[ *pTile ][ ( *pLight >> 9) & 0xff ];
        }
        pScr += dscr;
        pLight += dlight;
        pTile += dtile;
    }
    ArtClose( ImgObj );
}

int TileUnk48( int MapIdx1, int MapIdx2, int *a3, int cnt )
{
    int v66,v77,v88,v99,*v10,v11,v12,v16,y1,x1,y0,x0,v23,v24,v27,v28,v30,v31;

    if( cnt <= 1 ) return 0;
    TileGetScrCoordinates(MapIdx1, &x0, &y0);
    x0 += 16; y0 += 8;
    TileGetScrCoordinates(MapIdx2, &x1, &y1);
    x1 += 16; y1 += 8;
    *a3 = MapIdx1;
    v66 = 1;
    v28 = 2 * abs32(x1 - x0);
    if( x1 - x0 >= 0 )
        v77 = x1 - x0 > 0;
    else
        v77 = -1;
    v24 = v77;
    v27 = 2 * abs32(y1 - y0);
    if( y1 - y0 >= 0 )
        v88 = y1 - y0 > 0;
    else
        v88 = -1;
    v99 = x0;
    v23 = v88;
    v10 = a3 + 1;
    v11 = y0;
    if( v28 <= v27 ){
        v16 = v28 - (v27 >> 1);
        while( 1 ){
            v31 = v16;
            *v10 = TileGetPointed(v99, v11);
            if( *v10 == MapIdx2 ) break;
            if( *v10 != *(v10 - 1) && (v66 == 1 || *v10 != *(v10 - 2)) ){
                v66++; v10++;
                if( v66 == cnt ) return cnt;
            }
            if( v11 == y1 ) return v66;
            if( v31 >= 0 ){ v99 += v24; v31 -= v27; }
            v16 = v28 + v31; v11 += v23;
        }
    } else {
        v12 = v27 - (v28 >> 1);
        while( 1 ){
            v30 = v12;
            *v10 = TileGetPointed( v99, v11 );
            if( *v10 == MapIdx2 ) break;
            if( *v10 != *(v10 - 1) && (v66 == 1 || *v10 != *(v10 - 2)) ){
                v66++;v10++;
                if( v66 == cnt ) return cnt;
            }
            if( v99 == x1 ) return v66;
            if( v30 >= 0 ){ v11 += v23; v30 -= v28; }
            v12 = v27 + v30; v99 += v24;
        }
    }
    return v66 + 1;
}

int TileUnk49( int a1, int a2 )
{
    int i, n, err;
    int tmp[200];

    if( gTileCentIdx == a1 ) return -1;    
    if( !(n = TileUnk48( gTileCentIdx, a1, tmp, 200 )) ) return -1;
    err = 0;
    for( i = 1; i < n; i++){
        if( TileSetCenter( tmp[ i ], 0 ) == -1 ) break;
    }
    if( (a2 & 0x01) && (i != n) ){
        err = -1;
        TileSetCenter( gTileCentIdx, 0 );
    }
    if( (a2 & 2) != 0 && gTileUpdateEnable ) gTileRedrawCb( &gTileSurfArea, gCurrentMapLvl );        
    return err;
}

