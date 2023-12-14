#pragma once

#define THUMBNAIL_WIDTH 	224
#define THUMBNAIL_HEIGHT 	133
#define THUMBNAIL_SIZE 		(THUMBNAIL_WIDTH * THUMBNAIL_HEIGHT)

typedef struct // size of 136
{
    char 	Magic[ 24 ];
    short	VerMinor;
    short 	VerMajor;
    char 	VerRelease;
    char 	PlayerName[ 32 ];
    char 	SaveName[ 30 ];
    short 	RtcDay;
    short 	RtcMonth;
    short 	RtcYear;
    int 	RtcTime;
    short 	GMonth;
    short 	GDay;
    short 	GYear;
    int 	GTime;
    short 	MapLvl;
    short 	MapNum;
    char 	MapFname[ 16 ];
} LsgSlot_t;

//const int gLsgImgIds[ 9 ] = { 237, 238, 239, 9, 8, 181, 182, 199, 200 };
//int	gLsgSelectedSlotIdx = 0;
//int 	gLsgUnk01 = 0;
//int 	gLsgUnk18 = 0;
//int 	gLsgUnk55 = -1;
//int 	gLsgUnk56 = 0;
//char 	*gLsgMasterPatches = NULL;
//int (*gLsgSaveAction[ 27 ])( xFile_t * ) = {
//int (*gLsgLoadAction[ 27 ])( xFile_t *) = {
//int gLsgUnk02 = 0;
//Geo_t 	gLsgGeo[9];
//Msg_t 	gLsgMsg;
//LsgSlot_t gLsgSlots[ 10 ];
//int 	gLsgSaveFilesCondition[ 10 ];
//char 	*gLsgUnk15;
//char 	*gLsgThumbnail;
////MsgLine_t gLsgMsgLine;
//int 	gLsgUnk09;
//int 	gLsgWin;
//char 	*gLsgImg[ 9 ];
//char 	*gLsgThumbnail;
//char 	gLsgUnk38[260];
//char 	gLsgBakFileName[300];
//char 	gLsgCurFileName[300];
//char 	gLsgMsgPath[260];
//char 	*gLsgSurf;
//char 	gLsgFileName[260];
//xFile_t *gLsgFileHandler;
//int 	gLsgError;
//int 	gLsgFontSave;
//CachePool_t *gLsgArt[ 9 ];

void LsgClean();
void LsgPurgeFiles();
int LsgSaveGameMenu( int Mode );
int LsgMakeThumbnail();
int LsgMenuGameLoad( unsigned int arg );
// mode: 0- save game, 1- pick a quick save slot, 2,3 - load game, 4 - pick a quick load slot
int LsgMenuCreate( unsigned int mode );
void LsgClose( int Mode );
int LsgSaveGame();
int LsgGetUnk02();
int LsgLoad( int SlotNo );
void LsgGetTime( short *pDay, short *pMon, short *a4, int a3 );
int LsgSaveHdr( int SlotNo );
int LsgSlotLoad( int SlotNo );
int LsgLoadSlots();
void LsgDrawSlotList( int a1 );
void LsgDescription( int SlotNo );
int LsgReadThumbnail( int Idx );
int LsgEditLine( int SlotNo );
int LsgEditLineHandler( int WinId, int CodeKey1, int CodeKey2, char *str, int FieldLen, int Width, int Height, int ColorA, int ColorB, int flags );
int LsgFSaveNull( xFile_t *fh );
int LsgFLoadInit( xFile_t *fh );
int LsgUnk47( xFile_t *fh );
int LsgFSaveMaps( xFile_t *fh1 );
int LsgSlotMap2Game( xFile_t *fh );
int LsgReadFname( char *str, xFile_t *fh );
int LsgFileCopy( char *fname1, char *fname2 );
int LsgDeleteMapFiles();
int LsgDeleteFiles( char *path, char *Ext );
int LsgDeleteFile( char *a1, char *a2 );
int LsgBackup();
int LsgBackupRestore();
int LsgFLoadUnkA( xFile_t *fh );
int LsgFSaveUnkA( xFile_t *fh );
int LsgEraseBadSlot();

