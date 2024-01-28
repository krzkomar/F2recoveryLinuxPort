#pragma once

#define INTP_STACK_SIZE	0x1000

#define SCR_OPCODE	0x8000		// 1000
#define SCR_STRING	0x9001		// 1001 stala
#define SCR_FSTRING	0x9801 		// 1001	zmienna lancuchowa
#define SCR_FLOAT	0xA001		// 1010
#define SCR_INT		0xC001		// 1100
#define SCRTYPE( n )	((n) & ~0x0800)	//
#define SCR_PTR		0xE001		// ** added type **

// Intp_t.Flags
#define SCR_FEXIT	0x01
//#define SCR_		0x02
#define SCR_FERROR	0x04
//#define SCR_		0x08
#define SCR_FEXEC	0x10
#define SCR_FPROC_RUN	0x20
//#define SCR_		0x40
#define SCR_FCRITICAL	0x80
#define SCR_FATTACH	0x100

#define INTP_VNAME( scr, Idx )	(&((char *)scr->ProcVarNames)[ Idx ])

#define INTP_P_TIMED		0x01
#define INTP_P_CONDITIONAL	0x02
#define INTP_P_IMPORT		0x04
#define INTP_P_EXPORT		0x08
#define INTP_P_CRITICAL		0x10

typedef struct	// size of 24, BE values!
{
    int		Count;		// 0
    int		NameOfst;	// 4
    int		Flags; 		// 8 INTP_P_* flags
    int		Delay;		// 12
    int		CntOfst;	// 16
    int		BodyOfst;	// 20
    int		ArgsCnt;	// 24
} IntpProc_t;

typedef struct // size of 6
{
    short Type;
    int	  Var;
} __attribute__ (( packed )) IntpArg_t;

//typedef struct // size of 4
//{
//    short w01; // size
//    short w02; // length
//    char  Data[]; // string
//} IntpOp_t;

typedef struct // size of 4
{
    short Size;
    short Ref;
    char  String[];
} IntpString_t;

/*
typedef struct // size of 4
{
    uint16_t 	Len;		// string length, have to be even
    union{
	uint16_t Wrd;    	// 
	char     Str[ 2 ];	// 
    };
    char Data[];
} IntpPos_t;

typedef struct // size of 8
{
    uint32_t 	TotalSize;
    IntpPos_t	Entry; 
} IntpStr_t;
*/

#define INTP_STR_TOT( str )		(((uint32_t *)(str))[0])
#define INTP_STR_LEN( str )		(((uint16_t *)(str))[0])
#define INTP_STR_REF( str )		(((uint16_t *)(str))[1])

typedef struct _Intp_t
{
    char *FileName;			// *.INT file name
    char *Code;				// program data 
    struct _Intp_t *Parent;		//
    struct _Intp_t *ChildProcess;	//
    int CodePC;				// program counter
    int Base; 				// base
    int SaveStack;			//
    char *StackA;			// data stack
    char *StackB;			// address stack
    int StackApos;			//
    int StackIdxB;			// instruction stack
    char *StringsConst;			// pointer to string constans
    char *Strings;			// pointer to message strings list
    char *ProcVarNames;			// pointer to procedures and vars names
    IntpProc_t *ProcTable;		//
    jmp_buf EnvSave; 			// setjmp()/longjmp() buffer
    int Time;				//
    int TimeA;				//
    int TimeAtExec;			//
    int (*Func)(struct _Intp_t *); 	//
    uint16_t Flags;			// 0 - exit, 7- critical, .. ?
    uint16_t Opcode;			//
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

extern void (**gIntpTasks)( Intp_t *);
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

void IntpTaskAdd( void (*Task)( Intp_t *) );
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
void *IntpReadBep( void *Base, int Offset );
void IntpWriteBew( short wdata, char *Base, int Offset );
void IntpWriteBei( int idata, char *Base, int Offset );
void IntpWriteBep( void *pdata, void *Base, int Offset );
void IntpPushShortStack( char *Base, int *pIdx, short wdata );
void IntpPushPtrStack( char *Base, int *pIdx, void *pdata );
void *IntpPopPtrStack( char *Base, int *pidx );
void IntpPushIntStack( char *Base, int *pIdx, int idata );
int IntpPopIntStack( char *Base, int *pidx );
short IntpPopShortStack( char *Base, int *pIdx );
void IntpStrRefInc( Intp_t *r, short opcode, int a3 );
void IntpStringDeRef( Intp_t *scr, short type, int Idx );
void IntpPushwA( Intp_t *scr, short wdata );
void IntpPushiA( Intp_t *scr, int idata );
void IntpPushPtrA( Intp_t *scr, void *pdata ); // added
short IntpPopwA( Intp_t *scr );
int IntpPopiA( Intp_t *scr );
void *IntpPopPtrA( Intp_t *scr ); // added
void IntpPushwB( Intp_t *scr, short opcode );
void IntpPushiB( Intp_t *scr, int idata );
int IntpPopwB( Intp_t *scr );
int IntpPopiB( Intp_t *scr );
void IntpUnk03( Intp_t *scr );
void IntpTaskRun( Intp_t *scr );
void IntpUnLoad( Intp_t *scr );
Intp_t *IntpLoad( const char *fname );
int IntpGetOpcodeW( Intp_t *scr );
char *IntpGetString( Intp_t *scr, char Type, int Ref );
char *IntpGetName( Intp_t *scr, int Idx );
void IntpNegArg( Intp_t *scr, char *Str );
void IntpValidate( Intp_t *scr );
void IntpValidateAll();
void IntpMergeString( Intp_t *Itp );
int IntpAddString( Intp_t *intp, char *Msg );
void IntpGetOpcodeI( Intp_t *intp );


