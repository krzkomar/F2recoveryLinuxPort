#include "FrameWork.h"

#define INTP_STACK_SIZE	0x1000

#define SCR_OPCODE	0x8000
#define SCR_STRING	0x9001
#define SCR_FSTRING	0x9801 // const ?
#define SCR_FLOAT	0xA001
#define SCR_INT		0xC001
#define SCRTYPE( n )	((n) & ~0x0800)
#define SCR_PTR		SCR_INT


typedef struct	// size of 24, BE values!
{
    int		Count;
    int		NameOfst;
    int		Flags;
    int		Delay;
    int		CntOfst;
    int		BodyOfst;
    int		ArgsCnt;
} IntpProc_t;

typedef struct // size of 6
{
    short Type;
    int	  Var;
} __attribute__ (( packed )) IntpArg_t;

typedef struct // size of 4
{
    short w01; // size
    short w02; // length
    char  Data[]; // string
} IntpOp_t;

typedef struct // size of 4
{
    short Size;
    short Ref;
    char  String[];
} IntpString_t;



typedef struct _Intp_t
{
    char *FileName;			// *.INT file name
    char *IntpData;			// buffer of *.INT file, start of script initialiation code
    struct _Intp_t *Parent;		//
    struct _Intp_t *ChildProcess;	//
    int InstrPtr;			//
    int Base; 				// base
    int SaveStack;			//
    char *StackA;			//
    char *StackB;			//
    int StackApos;			//
    int StackIdxB;			// instruction stack
    IntpOp_t *Floats;			//
    IntpOp_t *Strings;			//
    char *StringBase;			//
    IntpProc_t *ProcTable;		//
    jmp_buf EnvSave; 			// setjmp()/longjmp() buffer
    int Time;				//
    int TimeA;				//
    int TimeAtExec;			//
    int (*Func)(struct _Intp_t *); 	//
    short Flags;			//
    short Opcode;			//
    int i34;				//
    int i35;				//
} Intp_t;

typedef struct _IntpList_t
{
  Intp_t *Itp;
  struct _IntpList_t *Prev;
  struct _IntpList_t *Next;
} IntpList_t;

typedef struct 
{
  Intp_t *scr;
  int i02;
} IntpTask_t;

extern int (**gIntpTasks)();
extern IntpTask_t gIntpList[ 256 ];
extern int gIntpTaskListCnt;
extern int gIntpTimeDiv;
extern Intp_t *gIntpCurScript;
extern int (*gIntpGetTime)();
extern IntpList_t *gIntpQe;

/*
IntpTask_t gIntpList[ 256 ];
int (**gIntpTasks)(Intp_t *);
int gIntpTaskListCnt;
Intp_t *gIntpCurScript;
IntpList_t *gIntpQe;
int (*gIntpGetTime)();
int gIntpTimeDiv;
int (*gIntpErrHandler)();
*/

void IntpTaskAdd( int (*Cb)( Intp_t *) );
void IntpTaskDel( void *Ptr );
void IntpTaskFire( Intp_t *itp );
int IntpGetSysTime();
void IntpTimerInit( int (*TimeCb)(), int Resolution );
void IntpNull1();
char *IntpMseHandler( char *s);
int IntpNull2();
int IntpTimer( Intp_t *scr );
void IntpSetErrHandler( int (*cb)() );
int IntpLog( const char *fmt, ... );
char *IntpGetProcName( Intp_t *scr );
void IntpError( char *fmt, ... );
short IntpReadBew( char *Base, int Offset );
int IntpReadBei( void *Base, int Offset );
void IntpWriteBew( short wdata, char *Base, int Offset );
void IntpWriteBei( int idata, char *Base, int Offset );
void IntpPushShortStack( char *Base, int *pIdx, short wdata );
void IntpPushPtrStack( char *Base, int *pIdx, void *pdata );
void *IntpPopPtrStack( char *Base, int *pidx );
void IntpPushIntStack( char *Base, int *pIdx, int idata );
int IntpPopIntStack( char *Base, int *pidx );
short IntpPopShortStack( char *Base, int *pIdx );
void IntpUnk02( Intp_t *r, short opcode, int a3 );
void IntpStringDeRef( Intp_t *scr, short type, int Idx );
void IntpPushwA( Intp_t *scr, short wdata );
void IntpPushiA( Intp_t *scr, int idata );
short IntpPopwA( Intp_t *scr );
int IntpPopiA( Intp_t *scr );
void IntpPushwB( Intp_t *scr, short opcode );
void IntpPushiB( Intp_t *scr, int idata );
int IntpPopwB( Intp_t *scr );
int IntpPopiB( Intp_t *scr );
void IntpUnk03( Intp_t *scr );
void IntpTaskRun( Intp_t *scr );
void IntpUnLoad( Intp_t *scr );
Intp_t *IntpLoad( const char *fname );
int IntpGetOpcodeW( Intp_t *scr );
char *IntpGetArg( Intp_t *scr, char Type, int Ref );
char *IntpGetString( Intp_t *scr, int Idx );
void IntpNegArg( Intp_t *scr, char *Str );
void IntpValidate( Intp_t *scr );
void IntpValidateAll();
void IntpMergeString( Intp_t *Itp );
int IntpDbgStr( Intp_t *intp, char *a2, unsigned int Line );
void IntpGetOpcodeI( Intp_t *a1 );


