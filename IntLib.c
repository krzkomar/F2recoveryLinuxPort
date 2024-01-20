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
    eprintf( "\nError during execution: %s\n", tmp );
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
    return BSWAP32( *((int *)((char *)Base + Offset )) );
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
    if( (*pIdx + 4) >= INTP_STACK_SIZE ) IntpError( "pushIntStack: Stack overflow." );
    IntpWriteBei( idata, Base, *pIdx );
    (*pIdx) += 4;
}

int IntpPopIntStack( char *Base, int *pidx )
{
    if( *pidx < 4 ) IntpError("\nStack underflow long.");
    (*pidx) -= 4;
    return IntpReadBei( Base, *pidx );
}

short IntpPopShortStack( char *Base, int *pIdx )
{    
    if( *pIdx < 2 ) IntpError( "\nStack underflow short." );    
    (*pIdx) -= 2;
    return Base[ *pIdx + 1] | (Base[ *pIdx + 0 ] << 8);
}

void IntpUnk02( Intp_t *r, short opcode, int a3 )
{
    if( opcode == SCR_FSTRING ) r->FString->Data[ a3 - 2 ]++;
}

void IntpStringDeRef( Intp_t *scr, short type, int Idx )
{
    if( type != SCR_FSTRING ) return;
    if( scr->FString->Data[ Idx - 1 ] ) // short
        scr->FString->Data[ Idx - 2 ] = scr->FString->Data[ Idx - 1 ] - 1;
    else
        eprintf( "Reference count zero for %s!\n", &scr->FString->Data[ Idx ] );
    if( scr->FString->Data[ Idx - 2 ] < 0 ) eprintf( "String ref went negative, this shouldn't ever happen\n" );
}

void IntpPushwA( Intp_t *scr, short wdata )
{
    IntpPushShortStack( scr->StackA, &scr->StackApos, wdata );
    if( wdata != SCR_FSTRING ) return;
    if( scr->StackApos < 6 )  return;
    scr->FString->Data[ IntpReadBei( scr->StackA, scr->StackApos - 6 ) - 2 ]++;
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
    if( scr->StackApos < 6 ) return;
    scr->FString->Data[ IntpReadBei( scr->StackB, scr->StackIdxB - 6 ) - 2 ]++;
}

void IntpPushiB( Intp_t *scr, int idata )
{
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, idata );
}

int IntpPopwB( Intp_t *scr )
{
    short tmp;

    tmp = IntpPopShortStack( scr->StackB, &scr->StackIdxB );
    if( tmp == SCR_FSTRING && scr->StackApos >= 4 ){
        IntpStringDeRef( scr, SCR_FSTRING, IntpReadBei( scr->StackB, scr->StackIdxB - 4 ) );
    }
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
    if( scr->FString ) dbg_free( scr->FString );
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
    char stmp[260];
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
    intp->FileName = dbg_malloc( strlen( fname ) );
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
    int n1, n2;

    n1 = scr->Code[ scr->CodePC + 0 ];
    n2 = scr->Code[ scr->CodePC + 1 ];
    scr->CodePC += 2;
    return (n1 << 8) | n2;
}

char *IntpGetArg( Intp_t *scr, char Type, int Ref )
{
    if( Type & 0x08 ) return &scr->FString->Data[ Ref ]; // number string
    if( Type & 0x10 ) return &scr->StringsConst[ Ref + 4 ]; // string
    return NULL;
}

char *IntpGetString( Intp_t *scr, int Idx )
{
    if( scr ) return NULL;
    return &scr->ProcVarNames[ Idx ];
}

void IntpNegArg( Intp_t *scr, char *Str )
{
    Str[ scr->FString->w01 ] = -Str[ scr->FString->w01 ];
    Str[ scr->FString->w02 ] = 0;
}

void IntpValidate( Intp_t *scr )
{
/*
    IntpOp_t *Strings, i, *p;
    char *v7;
    int ref, v8, v9;

    Strings = scr->FString;
    if( !Strings ) return;    
    p = (IntpOp_t *)FString->Data;
    for( i = *Strings; p->w01 != 0x8000;  ){
        ref = p->w01;
        if( ref < 0 ){
            ref = -ref;
            if( p->w02 ) eprintf( "Warning, reference count on invalid string is not zero!\n" );
        }
        p = (IntpOp_t *)(p->Data + ref);
        if( p > &scr->Strings->Data[ i->w01 ] ){
//            SciUnk28();
//            eprintf( 
//        	"Program %s string table trashed, got length %d(%x)\nnext %d(%x), table end is %d(%x)\n", 
//        	v7, v7, p - scr->FString - 4, p - scr->FString - 4, v8, v8, v9
//    	    );
//            exit( 1 );
        }
    }    
*/
}

void IntpValidateAll()
{
    IntpList_t *p;
    
    for( p = gIntpQe; p; p = p->Prev ) IntpValidate( p->Itp );    
}

void IntpMergeString( Intp_t *Itp )
{
    short *opcode, Len2;
    int Len1, tmp;
    
    if( !Itp->FString ) return;        
    for( opcode = (short *)Itp->FString->Data; *opcode != 0x8000; opcode += Len1 + 4 ){
        Len1 = opcode[ 0 ];
        if( Len1 <= 0 ){
            Len1 = -Len1; // abs
            Len2 = opcode[ Len1 + 4 ];
            if( Len2 == 0x8000 ) continue; // end marker
            if( Len2 >= 0 ) continue;
            tmp = 4 - Len2;
            if( tmp + Len1 >= (0x8000 - 2) ){
                eprintf( "merged string would be too long, size %d %d\n", tmp, Len1 );
            } else {
                Len1 += tmp;
                opcode[ 0 ] += Len2 - 4;
            }
        } else if( opcode[ 1 ] == 0 ){
            opcode[ 0 ] = -Len1;
        }        
    }
}

int IntpDbgStr( Intp_t *intp, char *a2, unsigned int Line )
{
    int v27;
    short *v8, *v20;
    
    v27 = strlen( a2 ) + 1;
    if( !intp ) return 0;
    if( v27 & 0x01 ) v27++;
    if( intp->FString ){
        for( v8 = (short *)intp->FString->Data; v8[ 0 ] != SCR_OPCODE; v8 += Line + 4 ){
            Line = v8[ 0 ];
            if( Line >= 0 ){
                if( Line == v27 ){
                    if( !strcmp( a2, (char *)v8 + 4 ) ) return v8 - (short *)intp->FString->Data + 2;
                }
            } else {
                Line = -Line; // abs()
                if( Line > v27 ){
                    if( Line - v27 <= 4 ){
                        v8[ 0 ] = Line;
                    } else {
                        v8[ v27 + 6 ] = 0;
                        v8[ v27 + 4 ] = 4 - Line - v27;
                        v8[ 0 ] = v27;
                    }
                    v8[ 1 ] = 0;
                    strcpy( (char *)v8 + 4, a2 );
                    v8[ v27 + 3 ] = 0;
                    return v8 - (short *)intp->FString->Data + 2;
                }
            }            
        }        
    } else {
        intp->FString = dbg_malloc( 8 );
        ((IntpArg_t *)intp->FString->Data)->Type = SCR_OPCODE;
        ((IntpArg_t *)intp->FString->Data)->Var = 1;
        intp->FString->w01 = 0;
    }
    intp->FString = dbg_realloc( intp->FString, v27 + intp->FString->w01 + 8 + 4 );
    v20 = (short *)&intp->FString->Data[ intp->FString->w01 ];
    if( v20[ 0 ] != SCR_OPCODE ) IntpError( "Internal consistancy error, string table mangled" );
    intp->FString->w01 += v27 + 4;
    v20[ 1 ] = 0;
    v20[ 0 ] = v27;
    strcpy( (char *)v20 + 4, a2 );
    *((char *)&v20[ v27 + 3 ] ) = 0;
    v20[ v27 + 2 ] = SCR_OPCODE;
    v20[ v27 + 3 ] = 1;
    return v20 - (short *)intp->FString->Data + 2;
}

void IntpGetOpcodeI( Intp_t *itp )
{
    IntpReadBei( itp->Code, itp->CodePC );
    itp->CodePC += 4;
    ScpA_Nop( itp );
}


