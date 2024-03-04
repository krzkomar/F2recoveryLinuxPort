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
int WmFindMatchForEncounterType( char *EncTypeName, int *pIdx );
int WmFindMatchForTerrainType( char *TerrainTypeName, int *pIdx );
int WmParseItems( char **pLine, WmItem_t *item, int *idx, const char *separator );
int WmParseItemState( char *line, WmItem_t *item );
int WmParseExprCond( char **pLine, char *Expr, WmExpr_t *Cond );
int WmParseGetExprArgs( char **pLine, char *a2, int *pType, int *pOperator, int *pVal, int *ArgRt );
int WmParseExprGetOperator( char **pLine, int *OpId );
int WmAreaInit();
int WmLookupMapByName( char *Name, int *Idx );
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

