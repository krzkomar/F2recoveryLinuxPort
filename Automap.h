#pragma once

typedef struct 
{
  char c01;
  int i01;
  int tab[480];
} Automap_t;

int AutomapInit();
void AutomapReset();
void AutomapClose();
int AutomapFLoad( xFile_t *fh );
int AutomapFSave( xFile_t *fh );
int AutomapUnk04( int a1 );
void AutomapScanner( int a1, int MotionFlg );
void AutomapRender( int win, int MapLvl, char *a3, char Flags );
int AutomapRenderMinimap( int WinId, int MapId, int MapLvl );
int AutomapSave();
int AutomapSaveEntryData( xFile_t *fh );
int AutomapCreateMinimap( int MapId, int MapLvl );
int AutomapWriteHdr( xFile_t *fh );
int AutomapReadHdr( xFile_t *fh );
void AutomapMakeMinimap( int a1 );
int AutomapCreateDatabase();
int AutomapUnk17( int a1, int a2 );
int AutomapFileCopy( xFile_t *fh1, xFile_t *fh2 , unsigned int a3 );
int AutomapLoadDB( Automap_t **pAutomap );


