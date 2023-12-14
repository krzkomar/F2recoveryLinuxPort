#pragma once

typedef struct 
{
  int i01;
  int i02;
  int i03;
  int i04;
} TileLight_t;

typedef struct 
{
  int r;
  int g;
  int b;
} TileColor_t;

//int gTileMapAreaDefined = 0;
//int gTileUnk62 = 1;
//int gTileUnk63 = 1;
//int gTileUnk60 = 1;
//int gTileSketchMode = 0;
//void (*gTileRedrawCb)(VidRect_t *, int ) = TileRenderIso;
//int gTileUpdateEnable = 1;
//TileLight_t gTileShade[ 10 ] = { 
//TileColor_t gTileColorTable[ 5 ] = {
//int gTileLightTable[ 80 * 39 ];
//int gTileUnk12[ 2 ][ 6 ];
//int gTileUnk06[ 2 ][ 6 ];
//char gTileUnk43[ 512 ];
//char gTileUnk42[ 512 ];
//char gTileRegions[ 32*16 ];
//VidRect_t gTileMapArea;
//VidRect_t gTileSurfArea;
//char gTileUnk41[512];
//int gTileGridOffsetY;
//int gTileGridOffsetX;
//int gTileScrOffsetX;
//int gTileScrOffsetY;
//void (*gTileMapUpdateCb)(VidRect_t *);
//int gTileSurfCentY;
//int gTileSurfCentX;
//int gTileGridSize;
//int gTileMapWidth;
//int **gTileGrid;
//char *gTileIsoSurface;
//int gTileMapHeight;
//int gTileSurfHeight;
//int gTileGridCentX;
//int gTileGridCentY;
//int gTileMapSize;
//int gTileGridHeight;
//unsigned int gTileSurfPitch;
//int gTileGridWidth;
//int gTileSurfWidth;
extern int gTileCentIdx;
extern int gTileHexX[6];
extern int gTileHexY[6];

int  TileInit( int **Grid, int GridW, int GridH, int MapW, int MapH, char *Surface, int SurfW, int SurfH, unsigned int SurfPitch, void (*UpdateCb)(VidRect_t *) );
void TileMapAreaDefine( int ScrW, int ScrH, int MapW, int MapH );
int  TileUnk03();
void TileUpdateDisable();
void TileUpdateEnable();
void TileUpdateArea( VidRect_t *Area, int MapLvl );
void TileUpdate();
int  TileSetCenter( int MapIdx, int MapLvl );
void TileRenderIsoMapper( VidRect_t *Area, int a2 );
void TileRenderIso( VidRect_t *area, int MapLvl );
void TileUnk09( int result );
int  TileUnk10();
int  TileGetScrCoordinates( int MapIdx, int *pX, int *pY );
int  TileGetPointed( int x, int y );
int  TileGetDistance( int MapIdx1, int MapIdx2 );
int  TileUnk14( int MapIdx1, int MapIdx2 );
int  TileUnk15( int MapIdx1, int MapIdx2 );
int  TileUnk16( int MapIdx, int n, int TilesCnt );
int  TileTurnAt( int MapIdx1, int MapIdx2 );
int  TileUnk18( int a1, int a2, int a3 );
int  TileGridBorder( int MapIdx );
void TileUnk20();
void TileUnk21();
int  TileUnk22();
void TileUnk23();
void TileUnk24();
int  TileUnk25();
void TileGetScrPosition( int GridIdx, int *pXpos, int *pYpos );
int  TileGetScrPositionA( int GridIdx, int *pXpos, int *pYpos );
int  TileGetGridIdx( int x, int y );
int  TileGetGridIdxA( int x, int y );
void TileScrToGridPos( int x, int y, int *pGridX, int *pGridY );
void TileScrToGridPosA( int x, int y, int *pGridX, int *pGridY );
void TileRenderRoof( VidRect_t *Area1, int MapLvl );
void TileRoofCover( int GridX, int GridY, int MapLvl );
void TileRoofVisible( int GridX, int GridY, int MapLvl, int Visible );
void TileRoofUncover( int GridX, int GridY, int MapLvl );
void TileDrawRoof( int ImgId, int Xpos, int Ypos, VidRect_t *Area, int Light );
void TileRenderFloor( VidRect_t *area, int MapLvl );
int  TilePointOpaque( int x, int y, int MapLvl );
void TileUnk39( VidRect_t *a1, int *a2, int *a3, int a4 );
void TileSketchToggle();
void TileSketchOn();
void TileSketchOff();
int  TileSketch();
void TileRenderSketch( VidRect_t *area, int a2 );
void TileSketchIso( int MapIdx, int a2 );
void TileSketchDraw( int GridIdx, int MapLvl, VidRect_t *a3 );
void TileDrawTile( int ArtId, int Xpos, int Ypos, VidRect_t *area );
int  TileUnk48( int MapIdx1, int MapIdx2, int *a3, int cnt );
int  TileUnk49( int a1, int a2 );



