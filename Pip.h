#pragma once

typedef struct 
{
  short w01;
  short w02;
  short TxtId;
} Pip01_t;

typedef struct 
{
  int a;
  int b;
  int c;
} PipHoloDisk_t;

typedef struct
{
  int a;
  int b;
  int c;
  int d;
  int e;
} PipQuests_t;

typedef struct 
{
  char *Name;
  short MapLvl;
  short MapId;
} Pip_t;

/*
VidRect_t gPipRegion
int gPipImgIds[11]
PipQuests_t *gPipQuestList
int gPipQuestCount
PipHoloDisk_t *gPipHoloTapes
int gPipHoloTapesCount
int gPipUnk05
int gPipUnk04
Pip01_t gPipUnk23[ 8 ]
void PipStatus( int a1 );
void PipAutomap( int a1 );
void PipArchives( int a1 );
void PipAlarmClock( int a1 );
void (*gPipHandlers[ 6 ])(int)
Geo_t gPipGeo[11];
MsgLine_t gPipMsgLine;
Msg_t gPipMsg;
Pip_t gPipLocNames[24];
Msg_t gPipUnk28;
int  gPipCurrentTasks;
char *gPipSurface;
char *gPipImgs[11];
int gPipHoloTapesEnabled;
int gPipMseY1;
int gPipMseX1;
int gPipTime;
int gPipButtons[ 24 ];
int gPipUnk46;
int gPipMseX0;
int gPipMseY0;
int gPipWin;
CachePool_t *gPipImgHan[11];
int gPipUnk33;
int gPipListViewed;
int gPipFontSave;
int gPipFontSave;
int gPipExit;
int gPipUnk40;
int gPipMenuSel;
int gpipAutomapLocations;
int gPipListBase;
int gPipLineRow;
int gPipUnk07;
int gPipMapId;
int gPipArchivesEnabled;
int gPipTermLines;
int gPipUnk12;
int gPipUnk32;
int gPipUnk36;
*/

void PipMenu( int Clock );
int PipCreate( int a1 );
void PipClose();
void PipInit();
void PipPrintCounter( int Value, int Digits, int Xpos, int Ypos );
void PipPrintDate();
void PipPrintLine( char *str, int Flags, int Color );
void PipBaclLink( int a1 );
int PipFSave( xFile_t * fh );
int PipFLoad( xFile_t * fh );
void PipStatus( int Selected );
void PipCitiesList( int Selected );
void PipHoloList();
int PipTapesList( int Selected );
int PipLocationSortCb( Pip_t *p1, Pip_t *p2 );
void PipAutomap( int a1 );
int PipGetLocations( int sel );
int PipLocationList( int a1 );
void PipArchives( int Selected );
int PipArchiveList( int sel );
void PipAlarmClock( int Select );
void PipWaitList( int Selected );
void PipHpPoints();
void PipTerminalCls();
void PipMakeLineRegions( int Offset, int Viewed, int Flag );
void PipDeleteLineRegions();
int  PipWaitGameTime( int Hour, int Min, char heal );
int PipAddHealth( int a1 );
int PipHealed();
void PipGameTimeDiff( int *hours, int *minutes, int TillHour );
int PipScreenSaver();
int PipQuestsLoad();
void PipDeleteQuestList();
//static int PipQuestSortCb( PipQuests_t *q1, PipQuests_t *q2 );
int PipHoloTapesLoad();
void PipDeleteHoloList();


