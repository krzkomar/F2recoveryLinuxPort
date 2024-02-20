#pragma once

#define MAPFLG_SAV	1
#define MAPFLG_LVL0	2
#define MAPFLG_LVL1	4
#define MAPFLG_LVL2	8

typedef struct 
{
  int Version;
  char Name[16];
  int StartHexGrid;
  int MapLvl;
  int PlayerOrientation;
  int LocVarsCnt;
  int ScriptId;
  int MapFlags;
  int Darkness;
  int VarsCnt;
  int MapId;
  int Time;
  int Filler[44];
} Map_t;

typedef struct 
{
  int MapId;
  int Frame;
  int PosY;
  int Orientation;
} Map01_t;

extern Map_t gMap;
extern Msg_t gMapMsg;
extern int *gMapIsoGrid[ 3 ];
extern int gMapIsoWin;
extern int gMapScriptId;

/*
void (*gMapRedrawIsoCb)( VidRect_t * ) = MapRedrawIso; 
int gMapGridFlags[ 3 ] = { 2, 4, 8 };
int gMapSysTime = 0;
int gMapUnk38 = 0;
int gMapIsoUnk03 = 0;
int gMapIsoUnk04 = -1;
int gMapIsoUnk05 = 0;

int *gMapLocalVars = NULL;
int *gMapVars = NULL;
int gMapLocalVarsCnt = 0;
int gMapVarsCnt = 0;
char *gMapName = "";
int gMapUnk43 = 0;
int gMapUnk03 = -1;
char gMapCurrentFName[ 16 ];
int gMapUnk33;
int gMapUnk35;
int gMapUnk34;
int gMapIsoGridData[ 30000 ]; // 3 * 100 * 100
Map01_t gMapCurrentPos;
VidRect_t gMapIsoGeo;
Msg_t gMapMsg;
char *gMapIsoSurf;
Map_t gMap;
int *gMapIsoGrid[ 3 ];
int  gMapIsoWin;
char gMapCityName[ 40 ];
char gMapFileName[ 260 ];
*/

/**********************************************************/
int MapIsoInit();
void MapIsoReset();
void MapIsoClose();
void MapMsgInit();
void MapStartA();
int MapExitMsg();
void MapAmbientEnable();
int MapAmbientDisable();
int MapUnk21();
int MapSetLvl( unsigned int a1 );
int MapUnk23( unsigned int a1 );
int MapSetVar( int VarId , int Val );
int MapGetVar( int VarId );
int MapSetLocalVar( int VarId, int Val );
int MapGetLocalVar( int VarId );
int MapVarsAdd( int VarNum );
void MapSetStart( int a1, int a2, int a3 );
void MapNewScript( int ScriptId );
void MapCurSetFileName( char *fname );
void MapCurGetFileName( char *fname );
char *MapCityLvlName( int MapId, unsigned int lvl );
int MapAreaCmpByMapId( int MapId1, int MapId2 );
int MapAreaCmpByEntId( int EntId1, int EntId2 );
char *MapGetCityName( int MapId );
char *MapGetCurrentCityName( int EntranceId );
int MapGetCurrentMapId();
int MapMove( int dx, int dy);
char *MapGetFilePath( char *fname );
int MapUnk10( int a1, int a2, int a3 );
void MapUnk09();
void MapLoadMapDialog();
int MapLoadMAP( char *fname );
int MapOpenById( int MapIdx );
int MapLoadMapFile( xFile_t *fh );
int MapLoadSAV( char *fname );
int MapClearKilled();
int MapGetAreaByEntrance();
int MapSetPos( Map01_t *p );
int MapJump();
void MapUnk07();
int MapSetMapFname();
int MapMapSave();
int MapSaving( xFile_t *fh );
int MapSaveMap( char *fName );
int MapSavingRandomEncounter( int a1 );
int MapUnk08();
void MapUpdateArea( VidRect_t *Area );
void MapRedrawIso( VidRect_t *Area );
void MapMapperCb( VidRect_t *Area );
int MapAllocVars( int num );
void MapFreeVars();
int MapLoadVars( xFile_t *fh );
int MapAllocLocalVars( int num );
void MapFreeLocalVars();
int MapLoadLocalVars( xFile_t *fh );
void MapUnk01();
void MapIsoGridInit();
void MapResetGrid();
int MapLoadGrid( xFile_t *fh, int Flags );
int MapSaveHdr( Map_t *map, xFile_t *fh );
int MapLoadHdr( Map_t *map, xFile_t *fh );

