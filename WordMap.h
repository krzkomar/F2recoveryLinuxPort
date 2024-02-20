#pragma once

typedef struct 
{
  int ArtId;
  int w;
  int h;
  CachePool_t *ImgObj;
  char *ImgData;
} WmImg_t;

typedef struct // size of 44
{
  char 		Name[ 40 ];
  int 		Id;
} WmAmbientSfx_t;

typedef struct // size of 12
{
  int 		Elevation;
  int 		TileNum;
  int 		i03;
} WmStartPoint_t;

typedef struct // size of 584
{
    char 	LookupName[ 40 ];
    int 	i11;
    int 	i13;
    char 	MapFileName[ 40 ];
    char 	BgMusicFname[ 40 ];
    int 	Flags;
    int 	AmbientSfxCnt;
    WmAmbientSfx_t AmbientSfx[ 6 ];
    int 	StartPointsCnt;
    WmStartPoint_t RandomStartPoints[ 15 ];
} Wmap_t;

typedef struct // size of 28
{
  int i01;
  int i02;
  int i03;
  int Id;
  int i05;
  int i06;
  int i07;
} WmEntrance_t;

typedef struct // size of 360
{
    char 	Name[ 40 ];
    int 	Id;
    int		WorldPosX;
    int 	WorldPosY;
    int 	Size;
    int 	StartState;
    int 	LockState[ 2 ];
    int 	TnmapArtIdx;
    int 	TnmapLblArtIdx;
    int 	EntranceCnt;
    WmEntrance_t Entrances[ 10 ];
} WmArea_t;

typedef struct 
{
  int TerrainType;
  int FillDir;
  int Freq[3];
  int EncounterType;
  int Fog;		// 0 - covered, 1 - grayed, other - uncovered
} WmTileDsc_t;

typedef struct
{
  int ArtId;
  CachePool_t *Img;
  char *Pix;
  char WalkMaskName[40];
  char *WorldMask;
  int Difficulty;
  WmTileDsc_t Dsc[6][7];
} WmTile_t;

typedef struct 
{
  int i01; // pid
  int i02; // min  
  int i03; // max
  int Used;
} WmItem_t;

typedef struct 
{
  int ArgType;
  int Operator;
  int Arg1;
  int Arg2;
} WmCond_t;

typedef struct 
{
  int Args;
  WmCond_t Cond[ 3 ];
  int i02[2];
} WmExpr_t;

typedef struct 
{
  char Name[40];
  int i05;
  int RatioFlg;
  int Ratio;
  int Pid;
  int Flags;
  int Distance;
  int TileNum;
  int ItemsCnt;
  WmItem_t Items[10];
  int TeamNum;
  int ScriptId;
  WmExpr_t Expr;
} WmTeam_t;

typedef struct 
{
  char Name[ 40 ];
  int Position; // formation type
  int Spacing;
  int Distance;
  int TeamCnt;
  WmTeam_t Team[ 10 ];
} WmPlayer_t;

typedef struct 
{
  int RandMin;
  int RandMax;
  int i03;
  int Type;
} WmEnc_t;

typedef struct 
{
  int Flags;
  int MapId;
  int SceneryType;
  int Chance;
  int Counter;
  WmExpr_t Expr;
  int EncCnt;
  WmEnc_t Enc[ 6 ];
} WmIdx_t;

typedef struct 
{
  char Name[40];
  int EncIdx;
  int IdxCnt;
  int MapIdxPool[6];
  int i01;
  int TabIdxCnt;
  WmIdx_t EncounterMaps[ 41 ];
} WmEnctr_t;

typedef struct 
{
    char Name[40];
    int i01;
    int MapId;
    int MapName[20];
} WmTerrain_t;

/*
const char *gWmOffOnList[ 2 ] = { "off", "on" };
const char *gWmNoYesList[ 2 ] = { "no", "yes" };
const char *gWmFreqList[ 6 ] = { "none", "rare", "uncommon", "common", "frequent", "forced" };
const char *gWmFillDirList[ 9 ] = { "no_fill", "fill_n", "fill_s", "fill_e", "fill_w", "fill_nw", "fill_ne", "fill_sw", "fill_se" };
const char *gWmWeightList[ 4 ] = { "none", "light", "normal", "heavy" };
WmTerrain_t *gWmTerrainTypes = NULL;
int gWmTerrainTypesCnt = 0;
WmTile_t *gWmTiles = NULL;
int gWmTilesCnt = 0;
int gWmHorizTileCnt = 0;
WmArea_t *gWmAreas = NULL;
int gWmAreasCount = 0;
const char *gWmSizeList[ 3 ] = { "small", "medium", "large" };
Wmap_t *gWmMaps = NULL;
int gWmMapCount = 0;
int gWmMenuWin = -1;
CachePool_t *gWmWallPaper = (void *)-1;
unsigned int gWmWpapW= 0;
int gWmWpapH = 0;
char *gWmSurface = NULL;
void *gWmWpapImg = 0;
int gWmMapOffsetX = 0;
int gWmMapOffsetY = 0;
char *gWmColor01 = NULL;
int gWmUnk121 = 0;
const char *gWmEncTypeList[ 4 ] = { "nothing","ambush","fighting","and" };
const char *gWmOperatorList[ 8 ] = { "_", "==", "!=", "<","> ", " ", "and", "or" };
const char *gWmPositionsList[ 6 ] = { "surrounding", "straight_line", "double_line", "wedge", "cone", "huddle" };
const int gWmUnk125[4] = { 154, 155, 438, 439 };
int gWmKnownLocationsCount = 0;
int *gWmKnownLocations = NULL;
int gWmCityId = -1;
int gWmTimeStamp = 0;
int gWmUnk128 = 0;
int gWmUnk129 = 0;
int gWmUnk130 = 1;
int gWmMseTimer = 0;
int gWmMoveMapAllow = 1;
void *gWmUnk133 = 0;
CachePool_t *gWmUnk134 = ART_NULL;
int gWmUnk135 = 0;
int gWmUnk136 = 0;
char *gWmSfxFname[ 2 ] = { "cricket", "cricket1" };

int gWmFreqValues[ 6 ];
int gWmMapStart;
Config_t *gWmCfg;
Msg_t gWmMsg;
WmEnctr_t *gWmEncounterTable;
int gWmEncounterTableCnt;
WmPlayer_t *gWmPlayers;
int gWmPlayersCnt;

int gWmTimeStamp;
int gWmFontSave;
CachePool_t *gWmUnk79[4];
void *gWmUnk80[4];
int gWmUnk81[4];
int gWmUnk82[4];
int gWmUnk100;
int gWmWidgets[7];
CachePool_t *gWmUnk84[2];
CachePool_t *gWmUnk99[2];
void *gWmUnk83[ 2 ];
void *gWmUnk85[ 2 ];

WmImg_t gWmMenuImg[3];

int gWmUnk109[ 2 ];
//int --gWmUnk108;
int gWmMapDst[ 2 ];
//int --gWmUnk106;
int gWmUnk105[ 2 ];
//int --gWmUnk104;
int gWmUnk103[ 2 ];
//int --gWmUnk102[];
int gWmUnk101[ 10 ];
int gWmGoToLocation;
int    gWmUnk23;
WmTile_t    *gWmTile;
int    gWmUnk22;
int    gWmUnk13;
int    gWmUnk24;
int    gWmUnk25;
int    gWmUnk26;
int    gWmUnk27;
int    gWmUnk28;
int    gWmUnk29;
int    gWmUnk30;
int    gWmUnk31;
int    gWmGoToLocation;
int    gWmAreaId;
int    gWmWorldPosX;
int    gWmWorldPosY;
int    gWmUnk11;
int    gWmUnk12;
int    gWmRandMapId;
int    gWmUnk33;
int    gWmUnk34;
int    gWmUnk35;
int    gWmCurrentArea;
int    gWmGasoline;
CachePool_t *gWmUnk39;
CachePool_t *gWmUnk58;
CachePool_t *gWmUnk60;
CachePool_t *gWmUnk61;
int    gWmUnk62;
int    gWmUnk63;
int    gWmUnk66;
CachePool_t    *gWmUnk67;
int    gWmUnk53;
int    gWmUnk36;
int    gWmUnk37;
int    gWmTravelByCar;
int    gWmCarWidth;
int    gWmCarHeight;
int    gWmCarFrameNo;
void    *gWmLocListImg;
int    gWmLocListHeight;
int    gWmLocListStartLine;
void *gWmImgDat03;
ArtFrmHdr_t    *gWmDialImg;
int    gWmDialWidth;
int    gWmDialHeight;
int    gWmDialFrame;
int 	gWmDialArt;
void    *gWmCarFrameImg;
int    gWmCarFrameWidth;
int    gWmCarFrameHeight;
void    *gWmVtLogoImg;
int    gWmVtLogoWidth;
int    gWmVtLogoHeight;
int    gWmUnk64;
int    gWmUnk65;
void    *gWmUnk68;
void    *gWmUnk69;
int    gWmLocListWidth;
ArtFrmHdr_t  *gWmCarImg;
int    gWmUnk72;
int    gWmUnk71[2];
int    gWmUnk70;
CachePool_t    *gWmUnk74;
CachePool_t    *gWmUnk76;
int    gWmUnk78;
int    gWmUnk73;
void    *gWmMonthImg;
ArtFrmHdr_t    *gWmDigitImg;
CachePool_t    *gWmUnk40;
char    *gWmUnk41;
CachePool_t    *gWmUnk42;
char    *gWmUnk43;
int    gWmUnk44;
int    gWmUnk45;
CachePool_t    *gWmUnk46;
char    *gWmUnk47;
int    gWmGasoline0;
int    gWmUnk48;
CachePool_t    *gWmUnk49;
char   *gWmUnk50;
int    gWmUnk51;
int    gWmGoToLocation;
int    gWmUnk53;
int    gWmUnk54;
int    gWmUnk55;
int    gWmUnk56;
int    gWmUnk57;
int    gWmUnk17;
int    gWmUnk16;

int gMapCurrentMapId; // --
*/

int WmWorldMapInit();
int WmInitVars1();
int WmInitVars2();
int WmClose();
int WmUnk108();
int WmSaveWmapDatFile( xFile_t *fh );
int WmLoadWmapDatFile( xFile_t *fh );
int WmSaveWmapDat();
int WmLoadWmapDat();
int WmLoadWmapTxt();
int WmLoadEncounterTable( Config_t *cfg, char *, char *SectionName );
int WmLoadEncounters( WmIdx_t *EncIdx, char *line );
int WmParseEnc( WmIdx_t *xx, char **pline );
int WmParseEncPlayer( char *line, int *PlayerId );
int WmGetPlayerId( char *PlayerName, int *PlayerId );
int WmParseAddEncounter( char *line, int *PlayerId );
int WmParsePlayerState( WmTeam_t *p, char **pLine );
int WmParseExprZero( WmExpr_t *cond );
int WmSetTerrainTypes( Config_t *cfg, char *Line );
int WmSetRandTerrMap( Config_t *cfg, WmTerrain_t *terr );
int WmSetTileDsc( WmTile_t *tile, int a2, int a3, char *text );
int WmFindMatchForEncounterType( int *pIdx, char *EncTypeName );
int WmFindMatchForTerrainType( int *pIdx, char *TerrainTypeName );
int WmParseItems( char **pLine, WmItem_t *item, int *idx, const char *separator );
int WmParseItemState( char *line, WmItem_t *item );
int WmParseExprCond( char **pLine, char *Expr, WmExpr_t *Cond );
int WmParseGetExprArgs( char **pLine, char *a2, int *pType, int *pOperator, int *pVal, int *ArgRt );
int WmParseExprGetOperator( char **pLine, int *OpId );
int WmAreaInit();
int WmLookupMapByName( int *Idx, char *Name );
int WmMapZero( Wmap_t *map );
int WmMapInit();
int WmGetMapCount();
int WmGetMapFileName( int Idx, char *FileName );
int WmGetMapIdxByFileName( char *FileName );
int WmIsMapSaved( int Idx );
int WmIsCurrentMapMapSaved();
int WmIsCurrentMapDeadBodyAge();
int WmCanRestHere( int Place );
int WmPipBoyEquipped();
void WmUnk41( int MapIdx );
int WmGetEntranceIdx( int AreaId, int EntranceId, int *pEntranceIdx );
int WmUnk43( int AreaId, int EntranceId, int Unk, int *pIdx );
int WmFindAreaByEntrance( int EntranceId, int *pIdx );
int WmUnk45( int MapId, int a2, int Val );
int WmMenu();
int WmMenu1( int a1 );
int WmUnk47();
int WmUnk48();
int WmRandomEncounter();
int WmGetCurTile();
int WmGetTile( int x, int y, WmTileDsc_t **pTile );
int WmDrawEncounter();
int WmSetupRandomEncounter();
int WmSetupCritter( int eax0, Obj_t **edx0, int a3 );
int WmSetupCritterObjs( WmPlayer_t *player );
int WmSetupRndNextTileNum( WmPlayer_t *player, WmTeam_t *team, int *pTileNum, int TileNum );
int WmUnk53( int TileNum );
int WmCompare( WmExpr_t *expr, int *var );
int WmScrCmp( int Arg1, int Operator, int Arg2 );
int WmWait( int a1 );
int WmLoadWorldMask( int Idx );
int WmIsAreaAccessible( int wx ,int wy );
int WmMapTravel( int a1, int a2 );
void WmMoveStep();
void WmListScrollUpDn( int Scroll );
void WmRedrawWidgets();
int WmMenuCreate();
int WmMenuDestroy();
int WmMoveMap( int dy, int dx, int hor, int vert, int *pAllow, int a6 );
void WmMouseHandle( int a1 );
int WmUnk68( int a1, int a2, int a3, int a4, int a5, int a6 );
int WmUnk69( int wX, int wY );
int WmUnk70( int wx, int wy, int scout );
int WmUnk71( int a1, int a2, int *a3 );
int WmLoadTileImg( int TileNo );
int WmDrawMapScreen();
void WmCalendar( int Update );
int WmGetArea( int PosX, int PosY, int *AreaNum );
void WmLocationCaption( WmArea_t *Area, WmImg_t *img, char *surf, int Xpos, int Ypos );
void WmDrawGrayed( char *surf, int w, int h, int pitch );
void WmWarFog( WmTile_t *Tiles, int WmX, int WmY, int ScrX, int ScrY, int width );
int WmPositionIndicator();
int WmUnk80();
int WmGetMiscMsg( WmArea_t *a1, char *a2 );
void WmGetLocationName( int CityId, char *CityName );
int WmIsAreaKnown( int AreaId );
int WmLocKarma( int AreaId );
int WmUnk03( int EntranceId );
int WmUnk04( int LocationId, int a2 );
int WmUnk05( int LocationId, int a2, int a3 );
int WmSetPos( int Location, int WmX, int WmY );
int WmGetWorldPos( int *pWmX, int *pWmY );
int WmGetCurrentLocation( int *pLocId );
void WmClearTerrain( int Terrain );
int WmUnk10();
int WmTownMap( int *a1 );
int WmCreateTownMenu();
int WmGetCityEntryName();
int WmCloseTownMenu();
int WmUnk86( int n );
int WmCarRefuel( int fuel );
int WmCarFuelGauge();
int WmCarNoFuel();
int WmUnk90();
int WmUnk91();
int WmUnk92();
int WmUnk93();
int WmGetSfxFileName( int NameId, char **pName );
int WmDrawIface( int Update );
void WmDrawGasGauge();
int WmRenderLocationNameList();
int WmMakeKnownLocationList( int **pNameIds, int *pCount );
int WmSortNamesCb( int *idx1, int *idx2 );
void WmDrawDial( int UpdateFlag );
int WmGetAssignId( int *pIdx );
int WmStartMapMusic();
int WmSetMapMusic( int MapId, const char *MusicFname );
int WmFindAreaByEntranceId( int Id, int *pIdx );
int WmSetArea( int AreaId );

