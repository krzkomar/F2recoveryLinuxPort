#pragma once

#define MAPFLG_SAV	1	// == 1 - the map is a savegame map (.SAV)
#define MAPFLG_LVL0	2	// == 0 - the map has an elevation at level 0
#define MAPFLG_LVL1	4	// == 0 - the map has an elevation at level 1
#define MAPFLG_LVL2	8	// == 0 - the map has an elevation at level 2

typedef struct 
{
  int 	Version;		// map version
  char 	Name[ 16 ];		// map filename
  int 	StartHexGrid;		// default player position
  int 	MapLvl;			// default map elevation
  int 	PlayerOrientation;	// default player elevation 
  int 	LocVarsCnt;		// number of local variables stored in map
  int 	ScriptId;		// script id for this map
  int 	MapFlags;		// MAPFLG_xx
  int 	Darkness;		// map darkness
  int 	GlobVarsCnt;		// number of global variables stored in map
  int 	MapId;			// map details
  int 	Time;
  int 	Filler[ 44 ];
} Map_t;

typedef struct 
{
  int MapId;
  int Lvl;
  int GridPos;
  int Orientation;
} MapPosition_t;

extern Map_t gMap;
extern Msg_t gMapMsg;
extern int *gMapIsoGrid[ 3 ];
extern int gMapIsoWin;
extern int gMapScriptId;
extern int gMapCurrentLvl;

/**********************************************************/
int  MapIsoInit();
void MapIsoReset();
void MapIsoClose();
void MapMsgInit();
void MapStartA();
int  MapExitMsg();
void MapAmbientEnable();
int  MapAmbientDisable();
int  MapAmbientEnabled();
int  MapSetLvl( unsigned int a1 );
int  MapLvlNotExist( unsigned int MapLvl );
int  MapSetGlobalVar( int VarId , int Val );
int  MapGetGlobalVar( int VarId );
int  MapSetLocalVar( int VarId, int Val );
int  MapGetLocalVar( int VarId );
int  MapAddLocalVars( int VarNum );
void MapSetStart( int a1, int a2, int a3 );
void MapNewScript( int ScriptId );
void MapCurSetFileName( char *fname );
void MapCurGetFileName( char *fname );
char *MapCityLvlName( int MapId, unsigned int lvl );
int  MapAreaCmpByMapId( int MapId1, int MapId2 );
int  MapAreaCmpByEntId( int EntId1, int EntId2 );
char *MapGetCityName( int MapId );
char *MapGetCurrentCityName( int EntranceId );
int  MapGetCurrentMapId();
int  MapMove( int dx, int dy);
char *MapGetFilePath( char *fname );
int  MapSetPlayerPosition( int Lvl, int Pos, int Orientation );
void MapReset();
void MapLoadMapDialog();
int  MapLoadMAP( char *fname );
int  MapOpenById( int MapIdx );
int  MapLoadMapFile( xFile_t *fh );
int  MapLoadSAV( char *fname );
int  MapClearKilled();
int  MapGetAreaByEntrance();
int  MapSetPos( MapPosition_t *p );
int  MapJump();
void MapUnk07();
int  MapSetMapFname();
int  MapMapSave();
int  MapSaving( xFile_t *fh );
int  MapSaveMap( char *fName );
int  MapSavingRandomEncounter( int a1 );
int  MapUnk08();
void MapUpdateArea( VidRect_t *Area );
void MapRedrawIso( VidRect_t *Area );
void MapMapperCb( VidRect_t *Area );
int  MapAllocVars( int num );
void MapFreeGlobalVars();
int  MapLoadVars( xFile_t *fh );
int  MapAllocLocalVars( int num );
void MapFreeLocalVars();
int  MapLoadLocalVars( xFile_t *fh );
void MapUnk01();
void MapIsoGridInit();
void MapResetGrid();
int  MapLoadGrid( xFile_t *fh, int Flags );
int  MapSaveHdr( Map_t *map, xFile_t *fh );
int  MapLoadHdr( Map_t *map, xFile_t *fh );

