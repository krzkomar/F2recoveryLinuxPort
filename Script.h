#pragma once

#define SCRT( n )	((((unsigned int)(n)) >> 24) & 0xff)
enum{
    SCR_TYPE_SYS,	// 0 system
    SCR_TYPE_SPT,	// 1 spatial
    SCR_TYPE_TIM,	// 2 timer
    SCR_TYPE_ITM,	// 3 item
    SCR_TYPE_CRT,	// 4 critter
    SCR_TYPES
};

// flags
#define	SCR_NOTREMOVE	0x10
#define	SCR_02		0x02
#define	SCR_01		0x01 // script loaded ?
#define	SCR_04		0x04
#define	SCR_08		0x08

extern int gGValCount;
#define SCPT_UNSETID	0xCCCCCCCC
#define MAX_SCPTID	32000
#define GYEAR		(365 * 24 * 60 * 60 * 10)

typedef struct 
{
  int Id;
  int i01;
  int HexOrTimer; // spatial Hex or timer
  int Radius;	  // spatial radius
  int Flags;
  int LocVarId;
    Intp_t *i07;
    int i08;
  int LocVarsIdx;
  int LocVarsCnt;
  int i11;
  int i12;
  int ArgVal;
  Obj_t *TimeEv; // ptr ?
    Obj_t *crit;
    Obj_t *item;
  int i17;
  int i18;
  int i19;
  int i20;
  int i21;
  int PprocIdx[ 28 ];
  int i23;
  int i24;
  int i25;
  int i26;
  int i27;
  int i28;
  int i29;
} Scpt_t;


typedef struct 
{
    Obj_t *obj;
    Obj_t *crit;
    char i03[16];
    int i07;
    int i08;
    int i09;
    int i10;
} Scpt01_t;


typedef struct _ScptCache_t
{
  Scpt_t 		Script[ 16 ];
  int 			ScptUsed;
  struct _ScptCache_t 	*Next;
} ScptCache_t;

typedef struct {
    ScptCache_t 	*First;
    ScptCache_t 	*Current;
    int 		Blocks;//pages
    int 		NextId;
} ScptBook_t;

typedef struct 
{
    char fname[ 16 ];	// script file name '*.int'
    int  LocalVars;
} ScptVars_t;

/*
int gScptUnk02;
int gScptUnk15[ 10 ];
int gScptUnk18;

Msg_t gScptMsgBook[ 1450 ];
Msg_t gScptMsg;
char gScptTimeStr[ 7 ];

char gScptFileName[ 20 ];

int gScptUnk01 = 1;
int gScptUnk13 = 0;
int gScptUnk17 = 0;
char gScptUnk22[6] = {0};
int gScptUnk24 = 0;
int gScptUnk23 = 0;


char *gScptTypeName[ SCR_TYPES ] = { "s_system", "s_spatial", "s_time", "s_item", "s_critter" };

int gScptCount = 0;

int gScptUnk10 = 0;
ScptCache_t *gScptUnk50 = NULL;
int gScptUnk51 = 0;
int gScptUnk52 = 1;

ScptBook_t gScrScripts[ SCR_TYPES ] = { NULL };
char *gScptPath = "scripts/";

int gScptEnable = 0;
int gScptUnk07 = 0;
int gScptUnk03 = 0;
unsigned int gScptInGameDekaSeconds = 302400;


ScptVars_t *gScptLocVarTable = NULL;
int gScptFiles = 0;
int gScptObjIds = 4;

int gScptUnk14 = -1;
int *gScptLocalVars = NULL;

int gScptLocalVarsCnt = 0;

Scpt_t *ScptUnk21();

//char *ScptGetDialog( int MsgPage, int MsgId, int a3 );

int gScptUnk109 = 0;
int gScptUnk110 = 0;
int gScptUnk111 = 0;
int gScptUnk113 = 0;
int gScptUnk112 = 0;
int gScptCalendarDays[ 13 ] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };

int gScptUnk02;
int gScptUnk15[10];
int gScptUnk114[10];
int gScptUnk115;
int gScptUnk118;
int gScptUnk121;
int *gScptUnk122;
int gScptUnk116;
int gScptUnk117;
int gScptUnk117;
Obj_t *gScptUnk102;
Obj_t *gScptUnk100;
Obj_t *gScptUnk101;
Obj_t *gScptUnk119;
Obj_t *gScptUnk120;
Msg_t gScptMsgBook[1450];
Msg_t gScptMsg;
char gScptTimeStr[7];
char gScptUnk104[21];
int gScptUnk107;
int gScptUnk108;
char gScptFileName[20];


#define P_PROC_COUNT	28
const char *gScptP_proc[ P_PROC_COUNT ] = {
    "no_p_proc",
    "start",
    "spatial_p_proc",
    "description_p_proc",
    "pickup_p_proc",
    "drop_p_proc",
    "use_p_proc",
    "use_obj_on_p_proc",
    "use_skill_on_p_proc",
    "none_x_bad",
    "none_x_bad",
    "talk_p_proc",
    "critter_p_proc",
    "combat_p_proc",
    "damage_p_proc",
    "map_enter_p_proc",
    "map_exit_p_proc",
    "create_p_proc",
    "destroy_p_proc",
    "none_x_bad",
    "none_x_bad",
    "look_at_p_proc",
    "timed_event_p_proc",
    "map_update_p_proc",
    "push_p_proc",
    "is_dropping_p_proc",
    "combat_is_starting_p_proc",
    "combat_is_over_p_proc"
};
*/

int ScptGetGameDekaSeconds();
void ScptGetGameDate( int *pMonth, int *pDay, int *pGYear);
int ScptGetGameTime();
char *ScptGetGameTimeStr();
void ScptSetGameTime( int time );
void ScptSetSeconds( int sec );
void ScptTimeAdvance( int dsec );
void ScptTimeCap2( int dsec );
int ScptClockInit();
int ScptUnk146();
int ScptPlayMovieEv( int *pMovieId, int WinId );
int ScptUnk144();
int ScptNewObjId();
Obj_t *ScptUnk142( int a1 );
int ScptUnk141( Intp_t *a1, int a2 );
Obj_t *ScptUnk140( Intp_t *a1 );
char *ScptUnk139( int a1 );
int ScptUnk138( int Pids, Obj_t *critter, Obj_t *item );
int ScptSetArg( int ScriptId, int ArgVal );
int ScptUnk136( int a1, int a2 );
Intp_t *ScptLoad( char *fname );
void ScptTaskCb( int arg );
int ScptUnk133();
void ScptUnk132();
void ScptUnk131( int a1, int a2 );
int ScptUnk130( int a1, Scpt_t *a2 );
int ScptUnk129( int a1, int a2 );
int ScptUnk128( xFile_t *a1, int *a2 );
int ScptLoadUnk17( xFile_t *fh, int **ptr );
int ScptUnk126( int Unused, Obj_t *obj );
void ScptUnk125();
void ScptUnk124( Scpt_t *a1 );
void ScptProcess();
void ScptUnk122();
int ScptUnk121( Scpt01_t *a1 );
int ScptUnk120( Scpt01_t *scr );
int ScptUnk119();
int ScptUnk118();
int ScptRequestElevator( Scpt_t *a1, int Reaction );
void ScptUnk116( unsigned int a1, int a2, int a3, int a4 );
void ScptUnk115( Obj_t *a1 );
void ScptUnk114();
int ScptUnk113( void *a1, void *a2 );
void ScptUnk112( Obj_t *a1, Obj_t *a2 );
void ScptUnk111( char *a1 );
int ScptExecScriptProc( int ScriptId, int arg );
void ScptIndexPproc( Scpt_t *scr );
int ScptUnk108( int Pid, int idx );
int ScptAppendFileToList( char *fname );
int ScptIsFileOnListA( char *fname );
int ScptIsFileOnList( char *fname, int *pLine);
int ScptLoadFileList();
void ScpVarNamesFree();
int ScptUnk102( int a1, int Pid );
int ScptGetScriptFname( int ScriptFileIdx, char *stmp );
int ScptSetDudeScript();
int ScptClearDudeScript();
int ScptInit();
int ScptUnk01();
int ScptGameInit();
int ScptGameReset();
int ScptClose();
int ScptMsgFree();
int ScptReset();
int ScptEnable();
int ScptDisable();
void ScptEnableUnk07();
void ScptDisableUnk07();
int ScptSaveVariables( xFile_t *fh );
int ScptLoadVariables( xFile_t *fh );
int ScptLoadVarTest( xFile_t *fh );
int ScptBookReset();
int ScptSaveScpt( Scpt_t *scr, xFile_t *fh );
int ScptSavePage( ScptCache_t *scr, xFile_t *fh );
int ScptSaveScript( xFile_t *fh );
int ScptLoadScpt( Scpt_t *scp, xFile_t *fh );
int ScptLoadScriptPage( ScptCache_t *dat, xFile_t *fh );
int ScptLoadScript( xFile_t *fh );
int ScptPtr( int Pid, Scpt_t **pScript );
int ScptCreateId( int Category );
int ScptGetFreeId( int Category );
int ScptNewScript( int *pNewId, int Category );
int ScptUnk17( int *pPid, int a2, int a3 );
int ScptRemoveLocalVars( Scpt_t *Scr );
int ScptRemove( int Pid );
int ScptFlush();
int ScptDeleteAll();
Scpt_t *ScptUnk20( int a1 );
Scpt_t *ScptUnk21();
void ScptUnk22();
void ScptUnk23();
int ScptUnk24( Obj_t *a1, int a2, int a3 );
int ScptUnk25( int a1, int a2, int a3 );
int ScptGetUsage( int Type );
int ScptCountUsage( int Type );
int ScptLoadAllScripts();
void ScptUnk29();
void ScptUnk30();
void ScptExecMapUpdateScripts( int a1 );
void ScptUnk32();
void ScptPrintScriptUsage();
int ScptGetMsgStr( int MsgIdx, Msg_t **Msg );
char *ScptGetDialogA( int a1, int a2 );
char *ScptGetDialog( int MsgPage, int MsgId, int SpkFlg );
int ScptGetLocVar( int ScrId, int VarIdx, int *pValue );
int ScptSetLocVar( int ScrId, int VarIdx, int Value );
int ScptUnk39();
int ScptUnk40( Obj_t *a1, int edx0,int a3, int a4 );



