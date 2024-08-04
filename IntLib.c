#include "FrameWork.h"
//intlib

#define MODE_64bit

int gIntpTaskListCnt = 0;
IntpTask_t gIntpList[ 256 ];
void (**gIntpTasks)(Intp_t *);
Intp_t *gIntpCurScript;
IntpList_t *gIntpQe;
int (*gIntpGetTime)() = TimerGetSysTime;
int gIntpTimeDiv = 1000;
int (*gIntpErrHandler)() = NULL;


void IntpTaskAdd( void (*Task)( Intp_t *) )
{
    int i;

    // search for free slot
    for( i = 0; i < gIntpTaskListCnt; i++ ){
        if( gIntpTasks[ i ] == NULL ) break;
    }
    // no free slots, expand task table
    if( i == gIntpTaskListCnt ){
        if( gIntpTasks ){
            gIntpTasks = dbg_realloc( gIntpTasks, (gIntpTaskListCnt + 1) * sizeof( void *) );
        } else { // create first element
            gIntpTaskListCnt = 0;
            gIntpTasks = dbg_malloc( sizeof( void *) );
        }
        gIntpTaskListCnt++;
    }
    gIntpTasks[ i ] = Task;
}

void IntpTaskDel( void *Ptr )
{
    int i;

    for( i = 0; i < gIntpTaskListCnt; i++ ){
    	if( Ptr == gIntpTasks[ i ] ){
    	    gIntpTasks[ i ] = NULL;
    	    break;
    	}
    }
}

void IntpTaskFire( Intp_t *itp )
{
    int i;

    for( i = 0; i < 256; i++ ){
        if( itp == gIntpList[ i ].scr ) gIntpList[ i ].scr = NULL;
    }

    for( i = 0; i < gIntpTaskListCnt; i++ ){
        if( gIntpTasks[ i ] ) gIntpTasks[ i ]( itp );
    }
}

int IntpGetSysTime()
{
    return TimerGetSysTime();
}

void IntpTimerInit( int (*TimeCb)(), int Resolution )
{
    gIntpTimeDiv = Resolution;
    gIntpGetTime = TimeCb;
}

void IntpNull1()
{
    /* EMPTY */
}

char *IntpMseHandler( char *s )
{
    return gSciMseHandler( s );
}

int IntpNull2()
{
    return 1;
}

int IntpTimer( Intp_t *scr )
{
    return 1000 * (gIntpGetTime)() / gIntpTimeDiv <= scr->Time;
}

void IntpSetErrHandler( int (*cb)() )
{
    gIntpErrHandler = cb;
}

int IntpLog( const char *fmt, ... )
{
    char stmp[ 256 ];
    va_list va;
    int err;

    va_start( va, fmt );
    if( !gIntpErrHandler ) return 0;
    err = vsprintf( stmp, fmt, va );
    eprintf( "%s", stmp );
    return err;
}

char *IntpGetProcName( Intp_t *scr )
{
    IntpProc_t *ProcTable;
    int *p_NameOfst, a, b, pos;
    unsigned int i, cnt;

    ProcTable = scr->ProcTable;
    p_NameOfst = &ProcTable->NameOfst;
    cnt = IntpReadBei( &ProcTable->Count, 0 );
    a = IntpReadBei( p_NameOfst, 16 );
    for( i = 0; i < (cnt - 1); i++ ){
	pos = IntpReadBei( &p_NameOfst[ i*6 ], 0 );
        b = IntpReadBei( &p_NameOfst[ (i+1)*6 ], 16 );
        if( a <= scr->CodePC && b > scr->CodePC ) return (char *)scr->ProcVarNames + pos;
        a = b;
    }
    return "<couldn't find proc>";
}

void IntpError( char *fmt, ... )
{
    char tmp[ 260 ];
    va_list valist;

    va_start( valist, fmt );
    vsprintf( tmp, fmt, valist );
    eprintf( "Error during execution: %s", tmp );
    if( gIntpCurScript ){
        eprintf( "Current script: %s, procedure %s", gIntpCurScript->FileName, IntpGetProcName( gIntpCurScript ) );
    } else {
        eprintf( "No current script" );
    }
    longjmp( gIntpCurScript->EnvSave, 1 ); // throw instruction 
}

short IntpReadBew( char *Base, int Offset )
{
    short tmp = *((short *)(( (char *)Base + Offset )));
    tmp = ((tmp << 8) & 0xff00) | ((tmp >> 8) & 0x00ff);
    return tmp;
}

int IntpReadBei( void *Base, int Offset )
{
    return BSWAP32( *((uint32_t *)((unsigned char *)Base + Offset )) );
}


void *IntpReadBep( void *Base, int Offset )
{
#if ARCH_32
    return IntpReadBei( Base, Offset );
#elif ARCH_64
    uint64_t tmp;
    tmp = (uint64_t)(((unsigned char *)Base)[ Offset + 7 ]);
    tmp |= (uint64_t)(((unsigned char *)Base)[ Offset + 6 ]) << 8;
    tmp |= (uint64_t)(((unsigned char *)Base)[ Offset + 5 ]) << 16;
    tmp |= (uint64_t)(((unsigned char *)Base)[ Offset + 4 ]) << 24;
    tmp |= (uint64_t)(((unsigned char *)Base)[ Offset + 3 ]) << 32;
    tmp |= (uint64_t)(((unsigned char *)Base)[ Offset + 2 ]) << 40;
    tmp |= (uint64_t)(((unsigned char *)Base)[ Offset + 1 ]) << 48;
    tmp |= (uint64_t)(((unsigned char *)Base)[ Offset + 0 ]) << 56;
    return (void *)tmp;
#elif
#error not supported platform
#endif
}

void IntpWriteBew( short wdata, char *Base, int Offset )
{
    Base[ Offset + 1 ] = wdata;
    Base[ Offset + 0 ] = wdata >> 8;
}

void IntpWriteBei( int idata, char *Base, int Offset )
{
    Base[ Offset + 3 ] = idata;
    Base[ Offset + 2 ] = idata >> 8;
    Base[ Offset + 1 ] = idata >> 16;
    Base[ Offset + 0 ] = idata >> 24;
}

void IntpWriteBep( void *pdata, void *Base, int Offset )
{
#if ARCH_32
    IntpWriteBei( idata, Base, Offset );
#elif ARCH_64
    uint64_t tmp = (uint64_t)pdata;
    ((unsigned char *)Base)[ Offset + 7 ] = tmp; tmp >>= 8;
    ((unsigned char *)Base)[ Offset + 6 ] = tmp; tmp >>= 8;
    ((unsigned char *)Base)[ Offset + 5 ] = tmp; tmp >>= 8;
    ((unsigned char *)Base)[ Offset + 4 ] = tmp; tmp >>= 8;
    ((unsigned char *)Base)[ Offset + 3 ] = tmp; tmp >>= 8;
    ((unsigned char *)Base)[ Offset + 2 ] = tmp; tmp >>= 8;
    ((unsigned char *)Base)[ Offset + 1 ] = tmp; tmp >>= 8;
    ((unsigned char *)Base)[ Offset + 0 ] = tmp;
#endif
}

void IntpPushShortStack( char *Base, int *pIdx, short wdata )
{
    if( (*pIdx + 2) >= INTP_STACK_SIZE ) IntpError( "pushShortStack: Stack overflow." );
    IntpWriteBew( wdata, Base, *pIdx );
    (*pIdx) += 2;
}

void IntpPushPtrStack( char *Base, int *pIdx, void *pdata )
{
    if( (*pIdx + sizeof( void * )) >= INTP_STACK_SIZE ) IntpError( "pushPtrStack: Stack overflow." );
    IntpWriteBep( pdata, Base, *pIdx );
    (*pIdx) += sizeof( void *);
}

void *IntpPopPtrStack( char *Base, int *pidx )
{
    if( *pidx < sizeof( void * ) ) IntpError( "\nStack underflow long." );
    *pidx -= sizeof( void *);
    return IntpReadBep( Base, *pidx );
}

void IntpPushIntStack( char *Base, int *pIdx, int idata )
{
    if( (*pIdx + sizeof( void *)) >= INTP_STACK_SIZE ) IntpError( "pushIntStack: Stack overflow." );
    IntpWriteBei( idata, Base, *pIdx );
    (*pIdx) += sizeof( void *);
}

int IntpPopIntStack( char *Base, int *pidx )
{
    if( *pidx < sizeof( void *) ) IntpError("\nStack underflow long.");
    (*pidx) -= sizeof( void *);
    return IntpReadBei( Base, *pidx );
}

short IntpPopShortStack( char *Base, int *pIdx )
{    
    if( *pIdx < 2 ) IntpError( "\nStack underflow short." );    
    (*pIdx) -= 2;
    return Base[ *pIdx + 1] | (Base[ *pIdx + 0 ] << 8);
}

void IntpStrRefInc( Intp_t *intp, short opcode, int Idx )
{
    if( opcode == SCR_FSTRING ) INTP_STR_REF( intp->Strings + Idx )++;
}

void IntpStringDeRef( Intp_t *scr, short type, int Idx )
{
    if( type != SCR_FSTRING ) return;
    if( INTP_STR_REF( scr->Strings + Idx ) ){
        INTP_STR_REF( scr->Strings + Idx )--;
    } else {
        eprintf( "Reference count zero for %s!", &scr->Strings[ Idx + 4 ] );
    }
    if( INTP_STR_REF( scr->Strings + Idx ) < 0 ) eprintf( "String ref went negative, this shouldn't ever happen" );
}

void IntpPushwA( Intp_t *scr, short wdata )
{
    IntpPushShortStack( scr->StackA, &scr->StackApos, wdata );
    if( wdata != SCR_FSTRING ) return;
    if( scr->StackApos < 10 )  return;
    INTP_STR_REF( scr->Strings + IntpReadBei( scr->StackA, scr->StackApos - 10 ) )++;
}

void IntpPushiA( Intp_t *scr, int idata )
{
    IntpPushIntStack( scr->StackA, &scr->StackApos, idata );
}

void IntpPushPtrA( Intp_t *scr, void *pdata ) // added
{
    IntpPushPtrStack( scr->StackA, &scr->StackApos, pdata );
}

short IntpPopwA( Intp_t *scr )
{
    return IntpPopShortStack( scr->StackA, &scr->StackApos );
}

int IntpPopiA( Intp_t *scr )
{
    return IntpPopIntStack( scr->StackA, &scr->StackApos );
}

void *IntpPopPtrA( Intp_t *scr )
{
    return IntpPopPtrStack( scr->StackA, &scr->StackApos );
}

void IntpPushwB( Intp_t *scr, short opcode )
{
    IntpPushShortStack( scr->StackB, &scr->StackIdxB, opcode );
    if( opcode != SCR_FSTRING ) return;
    if( scr->StackApos < 10 ) return;
    INTP_STR_REF( &scr->Strings[ IntpReadBei( scr->StackB, scr->StackIdxB - 10 ) ] )++;
}

void IntpPushiB( Intp_t *scr, int idata )
{
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, idata );
}

int IntpPopwB( Intp_t *scr )
{
    short tmp;

    tmp = IntpPopShortStack( scr->StackB, &scr->StackIdxB );
    if( tmp != SCR_FSTRING ) return tmp;
    if( scr->StackApos < 8 ) return tmp;
    IntpStringDeRef( scr, SCR_FSTRING, IntpReadBei( scr->StackB, scr->StackIdxB - 8 ) );
    return tmp;
}

int IntpPopiB( Intp_t *scr )
{
    return IntpPopIntStack( scr->StackB, &scr->StackIdxB );
}

void IntpUnk03( Intp_t *scr )
{
    if( !scr->Parent ) return;
    scr->Parent->Flags &= ~0x120u;
    if( scr == scr->Parent->ChildProcess ) scr->Parent->ChildProcess = NULL;
}

void IntpTaskRun( Intp_t *scr )
{
    if( scr->i35 ) return;    
    IntpTaskFire( scr );
    scr->i35 = 1;    
}

void IntpUnLoad( Intp_t *scr )
{
    Intp_t *p, *tmp;

    if( scr->Parent ){
        scr->Parent->Flags &= ~0x120;
        if( scr == scr->Parent->ChildProcess ) scr->Parent->ChildProcess = NULL;
    }
    for( p = scr; p->ChildProcess; ){
        if( !p->ChildProcess->i35 ){
            IntpTaskFire( p->ChildProcess );
            p->ChildProcess->i35 = 1;
        }
        p->ChildProcess->Parent = NULL;
        tmp = p;
        p = p->ChildProcess;
        tmp->ChildProcess = NULL;
    }
    if( !scr->i35 ){
        IntpTaskFire( scr );
        scr->i35 = 1;
    }
    if( scr->Strings ) dbg_free( scr->Strings );
    if( scr->Code ) dbg_free( scr->Code );
    if( scr->FileName ) dbg_free( scr->FileName );
    if( scr->StackA ) dbg_free( scr->StackA );
    if( scr->StackB ) dbg_free( scr->StackB );
    dbg_free( scr );
}

Intp_t *IntpLoad( const char *fname )
{
    xFile_t *fh;
    int flen;
    Intp_t *intp;
    char stmp[300];
    char *buff;

    if( !(fh = dbOpen( fname, "rb" ) ) ){
        sprintf( stmp, "Couldn't open %s for read\n", fname );
        IntpError( stmp );
        return 0;
    }
    // read whole file into the buffer
    flen = dbFileLength( fh );
    buff = dbg_malloc( flen );
    dbread( buff, 1, flen, fh );
    dbClose( fh );

    intp = dbg_malloc( sizeof( Intp_t ) );
    memset( intp, 0x00, sizeof( Intp_t ) );
    // set filename
    intp->FileName = dbg_malloc( strlen( fname ) + 1 );
    strcpy( intp->FileName, fname );    
    intp->ChildProcess = NULL;        
    intp->Parent = NULL;
    intp->TimeAtExec = -1;
    intp->StackA = dbg_calloc( 1, INTP_STACK_SIZE );
    intp->i35 = 0;
    intp->SaveStack = -1;
    intp->Base = -1;
    intp->StackB = dbg_calloc( 1, INTP_STACK_SIZE );
    intp->Code = buff;
    intp->ProcTable = (void *)buff + 42; // 42 - fixed size of ScpInit
    intp->ProcVarNames = (char *)intp->ProcTable + 24 * IntpReadBei( (void *)intp->ProcTable, 0 ) + 4; // +4 -> size of string table entry
    intp->StringsConst = (char *)intp->ProcVarNames + IntpReadBei( (void *)intp->ProcVarNames, 0 ) + 4;
    return intp;    
}

int IntpGetOpcodeW( Intp_t *scr )
{
    uint32_t n1, n2;

    n1 = scr->Code[ scr->CodePC + 0 ];
    n2 = scr->Code[ scr->CodePC + 1 ];
    scr->CodePC += 2;
    return (n1 << 8) | n2;
}

char *IntpGetString( Intp_t *scr, char Type, int Idx )
{
    if( Type & 0x08 ) return &scr->Strings[ Idx + 4 ]; // local var string
    if( Type & 0x10 ) return &scr->StringsConst[ Idx + 4 ]; // string
    return NULL;
}

char *IntpGetName( Intp_t *scr, int Idx )
{
    if( scr ) return NULL;
    return &scr->ProcVarNames[ Idx ];
}

void IntpNegArg( Intp_t *scr, char *Str )
{
    INTP_STR_LEN( scr->Strings ) = -INTP_STR_LEN( scr->Strings );
    INTP_STR_REF( scr->Strings ) = 0;
}
/*
void IntpValidate( Intp_t *intp )
{    
    char *p;
    short ofst;

    if( !intp->Strings ) return;    
    for( p = intp->Strings + 4; INTP_STR_LEN( p ) != 0x8000; p += ofst + 4 ){
        ofst = INTP_STR_LEN( p );
        if( ofst < 0 ){
            ofst = -ofst;
            if( INTP_STR_REF( p ) ) eprintf( "Warning, reference count on invalid string is not zero!" );
        }
        if( p > intp->Strings + INTP_STR_LEN( p ) + 4  ){
            SciUnk28();
            eprintf( "Program ? string table trashed, got length ?\nnext ?, table end is ?\n"
//        	"Program %s string table trashed, got length %d(%x)\nnext %d(%x), table end is %d(%x)", 
//        	v7, v7, p - scr->Strings - 4, p - scr->Strings - 4, v8, v8, v9
    	    );
            exit( 1 );
        }
    }    
}

void IntpValidateAll()
{
    IntpList_t *p;
    
    for( p = gIntpQe; p; p = p->Prev ) IntpValidate( p->Itp );    
}
*/
void IntpMergeString( Intp_t *intp )
{
    short len, ofst;
    int tmp;
    char *p;

    if( !intp->Strings ) return;        
    for( p = intp->Strings + 4; INTP_STR_LEN( p ) != 0x8000; p += ofst + 4 ){
        ofst = INTP_STR_LEN( p );
        if( ofst <= 0 ){
            ofst = -ofst;
            len = INTP_STR_LEN( p + ofst + 4 );
            if( len == (short)0x8000 ) continue; // end marker
            if( len >= 0 ) continue;
            tmp = 4 - len;
            if( tmp + ofst >= (0x8000 - 2) ){
                eprintf( "merged string would be too long, size %d %d", tmp, ofst );
            } else {
                ofst += tmp;
                INTP_STR_LEN( p ) += len - 4;
            }
        } else if( INTP_STR_REF( p ) == 0 ){
    	    INTP_STR_REF( p ) = 0;
    	    INTP_STR_LEN( p ) = -ofst; // flag it as merged
        }        
    }
}

int IntpAddString( Intp_t *intp, char *msg )
{
    char *p;
    int len; 
    short ofst;

    len = strlen( msg ) + 1;
    if( !intp ) return 0;
    if( len & 0x01 ) len++; // adjust to even size
    if( !intp->Strings ){ // first element
        intp->Strings = dbg_malloc( 8 );
        INTP_STR_LEN( intp->Strings + 4 ) = 0x8000;
        INTP_STR_REF( intp->Strings + 4 ) = 1;
        INTP_STR_TOT( intp->Strings ) = 0;
    } else {
        for( p = intp->Strings + 4; INTP_STR_LEN( p ) != 0x8000; p += ofst + 4 ){
            ofst = INTP_STR_LEN( p );
            if( ofst < 0 ){  // 0x8000 is set, -> string is merged
                ofst = -ofst; // make it positive
                if( ofst <= len ) continue; // not fit, go next
                // replace string
                if( ofst - len <= 4 ){
                    INTP_STR_LEN( p ) = ofst;
                } else {
                    INTP_STR_REF( p + len + 4 ) = 0;
                    INTP_STR_LEN( p + len + 4 ) = (len - ofst) + 4;
                    INTP_STR_LEN( p ) = len;
                }
                INTP_STR_REF( p ) = 0;
                strcpy( p + 4, msg );
                p[ len + 3 ] = 0;
                return p - intp->Strings;
            } else { // check if string is already in list
                if( INTP_STR_LEN( p ) != len ) continue;
                if( !strcmp( msg, p + 4 ) ) return p - intp->Strings; // return just an index
            }            
        }        
    }
    // extend strings
    intp->Strings = dbg_realloc( intp->Strings, INTP_STR_TOT( intp->Strings ) + len + 8 + 4 ); // 
    p = intp->Strings + INTP_STR_TOT( intp->Strings ) + 4; // pointer to end of current string list
    // check end guard
    if( INTP_STR_LEN( p ) != 0x8000 ) IntpError( "Internal consistancy error, string table mangled" );
    INTP_STR_TOT( intp->Strings ) += len + 4; // update total size
    // -- create new entry --
    // start header, 4 bytes
    INTP_STR_REF( p ) = 0;
    INTP_STR_LEN( p ) = len;
    // copy data
    strcpy( p + 4, msg );
    p[ len + 3 ] = 0;
    // end guard
    INTP_STR_LEN( p + len + 4 ) = 0x8000;
    INTP_STR_REF( p + len + 4 ) = 1;
    return p - intp->Strings; // return offset
}

void IntpGetOpcodeI( Intp_t *itp )
{
    IntpReadBei( itp->Code, itp->CodePC );
    itp->CodePC += 4;
    ScpA_Nop( itp );
}


