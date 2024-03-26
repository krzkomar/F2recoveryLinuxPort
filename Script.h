#pragma once

#define SCP_DEBUG

#define SCRT( n )	((((unsigned int)(n)) >> 24) & 0xff)
//#define SCP_DBG( n, m... )

// script action flags
#define SCP_ACT_01		0x01 // combat ...
#define SCP_ACT_02		0x02 // wmunk10()
#define SCP_ACT_WORLDMAP	0x04
#define SCP_ACT_ELEVATOR	0x08
#define SCP_ACT_EXPLOSION	0x10
#define SCP_ACT_TALK		0x20
#define SCP_ACT_COMBAT		0x40 // combat ...
#define SCP_ACT_ENDGAME		0x80
#define SCP_ACT_STEAL_MENU	0x100
#define SCP_ACT_STEAL_ATTEMPT	0x200
#define SCP_ACT_400		0x400 // combat ...

#ifdef SCP_DEBUG
extern int scp_dbg;
#define SCP_DBG_IF( cond )	if( scp_dbg && (cond) )
#define SCP_DBG_ELSE		else
#define SCP_DBG_EN		scp_dbg = 1
#define SCP_DBG_DIS		scp_dbg = 0
#define SCP_DBG( s, n, m... )	if( scp_dbg ){ printf( "SCRIPT{A:%i}['%s':<%x>:%x]>"#n"\n",s->StackApos,s->FileName, s->Opcode & 0xffff, s->CodePC - 2, ##m ); }
#define SCP_DBG_VAR		Intp_t *s_dbg = scr;
#define SCP_DBGA( n, m... )	if( scp_dbg ){ printf( "SCRIPT{A:%i}['%s':<%x>:%x]>"#n"\n",s_dbg->StackApos,s_dbg->FileName, s_dbg->Opcode & 0xffff, s_dbg->CodePC - 2, ##m ); }
#define SCP_DNAME( Idx )	INTP_VNAME( s_dbg, Idx )
#define SCP_DECHO( str )	if( scp_dbg ){ printf( "%s\n", str ); }
#define SCP_DBGP( fmt, m... )	if( scp_dbg ){ printf( fmt, ##m ); }
#else
#define SCP_DBG_IF( cond )
#define SCP_DBG_ELSE
#define SCP_DBG_EN
#define SCP_DBG_DIS
#define SCP_DBG( s, n, m... )
#define SCP_DBG_VAR
#define SCP_DBGA( n, m... )
#define SCP_DNAME( Idx )
#define SCP_DECHO( str )
#define SCP_DBGP( fmt, m... )
#endif

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

// Action Event handler procedure names in scripts
enum{
    SCPT_AEV_NO_P_PROC,			// 0
    SCPT_AEV_START,			// 1
    SCPT_AEV_SPATIAL_P_PROC,		// 2
    SCPT_AEV_DESCRIPTION_P_PROC,	// 3
    SCPT_AEV_PICKUP_P_PROC,		// 4
    SCPT_AEV_DROP_P_PROC,		// 5
    SCPT_AEV_USE_P_PROC,		// 6
    SCPT_AEV_USE_OBJ_ON_P_PROC,		// 7
    SCPT_AEV_USE_SKILL_ON_P_PROC,	// 8
    SCPT_AEV_NONE_X_BAD1,		// 9
    SCPT_AEV_NONE_X_BAD2,		// 10
    SCPT_AEV_TALK_P_PROC,		// 11
    SCPT_AEV_CRITTER_P_PROC,		// 12
    SCPT_AEV_COMBAT_P_PROC,		// 13
    SCPT_AEV_DAMAGE_P_PROC,		// 14
    SCPT_AEV_MAP_ENTER_P_PROC,		// 15
    SCPT_AEV_MAP_EXIT_P_PROC,		// 16
    SCPT_AEV_CREATE_P_PROC,		// 17
    SCPT_AEV_DESTROY_P_PROC,		// 18
    SCPT_AEV_NONE_X_BAD3,		// 19
    SCPT_AEV_NONE_X_BAD4,		// 20
    SCPT_AEV_LOOK_AT_P_PROC,		// 21
    SCPT_AEV_TIMED_EVENT_P_PROC,	// 22
    SCPT_AEV_MAP_UPDATE_P_PROC,		// 23
    SCPT_AEV_PUSH_P_PROC,		// 24
    SCPT_AEV_IS_DROPPING_P_PROC,	// 25
    SCPT_AEV_COMBAT_IS_STARTING_P_PROC,	// 26
    SCPT_AEV_COMBAT_IS_OVER_P_PROC,	// 27
    SCPT_AEV_ALL
};


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
    Intp_t *Intp;
    int i08;
  int LocalVarBase;	// base position for local variables in gMapLocalVars
  int LocVarsCnt;
  int i11; 	// ret value
  int ActionEventId;	// proc id
  int ArgVal;
  Obj_t *TimeEv; // ptr ?
    Obj_t *SourceObj; // source obj
    Obj_t *TargetObj; // target obj
  int i17;	// action being used flag
  int OverrideFlag;	// override
  int i19;
  int i20;
  int i21;
  int ActionEventsIdx[ SCPT_AEV_ALL ];	// prototype procedures 'Action Events'
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
    Obj_t 	*obj; 	// who obj
    Obj_t 	*crit; 	// victim obj
    int 	i03[4];	// 0-?, 1-bonus, 2-0, 3-min dmg
    int 	i07;	// max dmg
    int 	i08;	// boolean attacker_result == traget result
    int 	i09;	// attacker results obj?
    int 	i10;	// target results obj?
} Scpt01_t; 		// attack


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

extern const char *gScptP_proc[ 28 ];
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
int ScptMidnightEv(Obj_t *obj, int * );
int ScptPlayMovieEv( int *pMovieId, int WinId );
int ScptMapUpdateEv();
int ScptNewObjId();
Obj_t *ScptUnk142( int a1 );
int ScptGetActionSource( Intp_t *a1 );
Obj_t *ScptGetSelfObj( Intp_t *a1 );
char *ScptUnk139( int a1 );
/*
    Set source and target objects for script execution
*/
int ScptSetup( int Pids, Obj_t *SourceObj, Obj_t *TargetObj );
int ScptSetArg( int ScriptId, int ArgVal );
int ScptUnk136( int a1, int a2 );
Intp_t *ScptLoad( char *fname );
void ScptTaskCb( int arg );
int ScptUnk133();
void ScptUnk132();
void ScptUnk131( int a1, int a2 );
int ScptUnk130( int a1, Scpt_t *a2 );
int ScptAddTimerEvent( int, int a1, int a2 );
int ScptSaveTimedEv( xFile_t *a1, int *a2 );
int ScptLoadTimedEv( xFile_t *fh, int **ptr );
int ScptRunTimedEv( Obj_t *obj, int *Ptr );
void ScptUnk125();
void ScptUnk124( Scpt_t *a1 );
void ScptActionExec();
void ScptUnk122();
int ScptUnk121( Scpt01_t *a1 );
int ScptUnk120( Scpt01_t *scr );
int ScptUnk119();
int ScptWorldMap();
int ScptRequestElevator( Obj_t *a1, int Reaction );
void ScptExplosion( unsigned int tilenum, int a2, int a3, int a4 );
void ScptTalkTo( Obj_t *a1 );
void ScptSlideShow();
int ScptUnk113( void *a1, void *a2 );
void ScptUnk112( Obj_t *a1, Obj_t *a2 );
void ScptUnk111( char *a1 );
int ScptRun( int ScriptId, int ProcId );
void ScptIndexPproc( Scpt_t *scr );
int ScptEventHandled( int Pid, int idx );
int ScptAppendFileToList( char *fname );
int ScptIsFileOnListA( char *fname );
int ScptIsFileOnList( char *fname, int *pLine);
int ScptLoadFileList();
void ScpVarNamesFree();
int ScptSetupLocalVars( int a1, int Pid );
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
void ScptAmbientEnable();
void ScptAmbientDisable();
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
void ScptMapEnter();
void ScptMapUpdate();
void ScptExecMapUpdateScripts( int a1 );
void ScptMapExit();
void ScptPrintScriptUsage();
int ScptGetMsgStr( int MsgIdx, Msg_t **Msg );
char *ScptGetDialogA( int a1, int a2 );
char *ScptGetDialog( int MsgPage, int MsgId, int SpkFlg );
int ScptGetLocVar( int ScrId, int VarIdx, int *pValue );
int ScptSetLocVar( int ScrId, int VarIdx, int Value );
int ScptCombat();
int ScptAreaDamage( Obj_t *a1, int edx0,int a3, int a4 );



