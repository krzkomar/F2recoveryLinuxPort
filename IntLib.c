#include "FrameWork.h"
//intlib

IntpTask_t gIntpList[ 256 ];

int (**gIntpTasks)(Intp_t *) = {0};

int gIntpTaskListCnt = 0;

Intp_t *gIntpCurScript;
IntpList_t *gIntpQe;


int (*gIntpGetTime)() = TimerGetSysTime;
int gIntpTimeDiv = 1000;
int (*gIntpErrHandler)() = NULL;


void IntpTaskAdd( int (*Cb)( Intp_t *) )
{
    int (**restore)(Intp_t *);
    int i;

    restore = gIntpTasks;
    // search for free slot
    for( i = 0; i < gIntpTaskListCnt; i++ ){
        if( gIntpTasks[ i ] == NULL ) break;
    }
    // extend task table
    if( i == gIntpTaskListCnt ){
        if( gIntpTasks ){
            restore = dbg_realloc( gIntpTasks, (gIntpTaskListCnt + 1) * sizeof( void *) );
        } else { // create first element
            gIntpTasks = NULL;
            restore = dbg_malloc( sizeof( void *) );
        }
        gIntpTaskListCnt++;
    }
    restore[ i ] = Cb;
    gIntpTasks = restore;
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

char *IntpMseHandler( char *s)
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
    char stmp[256];
    va_list va;
    int err;

    va_start(va, fmt);
    if( !gIntpErrHandler ) return 0;
    err = vsprintf( stmp, fmt, va );
    eprintf( "%s", stmp );
    return err;
}

char *IntpGetProcName( Intp_t *scr )
{
    int *v3,v4,v6,i,cnt;

    cnt = IntpReadBei( (void *)scr->ProcTable, 0 );
    v3 = &scr->ProcTable->NameOfst;
    v4 = IntpReadBei( (void *)v3, 16 );
    for( i = 1; i < cnt; i++ ){
        v6 = IntpReadBei((void *)v3 + 6, 16);
        if( v4 <= scr->InstrPtr && v6 > scr->InstrPtr ) return scr->StringBase + IntpReadBei( (void *)v3, 0 );
        v3 += 6;
        v4 = v6;
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

void IntpPushShortStack( char *Base, int *pIdx, short wdata )
{
    if( (*pIdx + 2) >= INTP_STACK_SIZE ) IntpError("pushShortStack: Stack overflow.");
    Base[ *pIdx + 1 ] = wdata;
    Base[ *pIdx + 0 ] = (wdata >> 8);
    (*pIdx) += 2;
}

void IntpPushPtrStack( char *Base, int *pIdx, void *pdata )
{
    if( (*pIdx + sizeof( void * )) >= INTP_STACK_SIZE ) IntpError( "pushPtrStack: Stack overflow." );
    if( sizeof( void *) == 8 ){
	IntpWriteBew( (long)pdata >> 48, Base, *pIdx + 0 );
	IntpWriteBew( (long)pdata >> 32, Base, *pIdx + 2 );
	IntpWriteBew( (long)pdata >> 16, Base, *pIdx + 4 );
	IntpWriteBew( (long)pdata, Base, *pIdx + 6 );
    } else {
	IntpWriteBew( (long)pdata >> 16, Base, *pIdx + 0 );
	IntpWriteBew( (long)pdata, Base, *pIdx + 2 );
    }
    (*pIdx) += sizeof( void *);
}

void *IntpPopPtrStack( char *Base, int *pidx )
{
    if( *pidx < sizeof( void * ) ) IntpError( "\nStack underflow long." );
    *pidx -= sizeof( void *);
    if( sizeof( void *) == 8 ){
	unsigned long tmp;
	tmp = (unsigned long)IntpReadBei( Base, *pidx + 4 );
	tmp |= (unsigned long)IntpReadBei( Base, *pidx ) << 32;
	return (void *)tmp;
    } else {
	IntpReadBei( Base, *pidx );    
    }
}

void IntpPushIntStack( char *Base, int *pIdx, int idata )
{
    if( (*pIdx + 4) >= INTP_STACK_SIZE ) IntpError( "pushIntStack: Stack overflow." );
    IntpWriteBew( idata >> 16, Base, *pIdx );
    IntpWriteBew( idata, Base, *pIdx + 2 );
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
    if( opcode == SCR_FSTRING ) r->Strings->Data[ a3 - 2 ]++;
}

void IntpStringDeRef( Intp_t *scr, short type, int Idx )
{
    if( type != SCR_FSTRING ) return;
    if( scr->Strings->Data[ Idx - 1 ] ) // short
        scr->Strings->Data[ Idx - 2 ] = scr->Strings->Data[ Idx - 1 ] - 1;
    else
        eprintf( "Reference count zero for %s!\n", &scr->Strings->Data[ Idx ] );
    if( scr->Strings->Data[ Idx - 2 ] < 0 ) eprintf( "String ref went negative, this shouldn't ever happen\n" );
}

void IntpPushwA( Intp_t *scr, short wdata )
{
    IntpPushShortStack( scr->StackA, &scr->StackApos, wdata );
    if( wdata != SCR_FSTRING ) return;
    if( scr->StackApos < 6 )  return;
    scr->Strings->Data[ IntpReadBei( scr->StackA, scr->StackApos - 6 ) - 2 ]++;
}

void IntpPushiA( Intp_t *scr, int idata )
{
    IntpPushIntStack( scr->StackA, &scr->StackApos, idata );
}

short IntpPopwA( Intp_t *scr )
{
    return IntpPopShortStack( scr->StackA, &scr->StackApos );
}

int IntpPopiA( Intp_t *scr )
{
    return IntpPopIntStack( scr->StackA, &scr->StackApos );
}

void IntpPushwB( Intp_t *scr, short opcode )
{
    IntpPushShortStack(scr->StackB, &scr->StackIdxB, opcode);
    if( opcode != SCR_FSTRING ) return;
    if( scr->StackApos < 6 ) return;
    scr->Strings->Data[ IntpReadBei( scr->StackB, scr->StackIdxB - 6 ) - 2 ]++;
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
    if( scr->Strings ) dbg_free( scr->Strings );
    if( scr->IntpData ) dbg_free( scr->IntpData );
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
    intp->IntpData = buff;
    intp->ProcTable = (void *)buff + 42; // 42 - fixed size of ScpInit
    intp->StringBase = (char *)(intp->ProcTable + 24 * IntpReadBei( (void *)intp->ProcTable, 0 ) + 4);
    intp->Floats = (IntpOp_t *)(intp->StringBase + IntpReadBei( (void *)intp->StringBase, 0 ) + 4);
    return intp;    
}

int IntpGetOpcodeW( Intp_t *scr )
{
    int n1, n2;

    n1 = scr->IntpData[ scr->InstrPtr + 0 ];
    n2 = scr->IntpData[ scr->InstrPtr + 1 ];
    scr->InstrPtr += 2;
    return (n1 << 8) | n2;
}

char *IntpGetArg( Intp_t *scr, char Type, int Ref )
{
    if( Type & 0x08 ) return &scr->Strings->Data[ Ref ];
    if( Type & 0x10 ) return &scr->Floats->Data[ Ref ];
    return NULL;
}

char *IntpGetString( Intp_t *scr, int Idx )
{
    if( scr ) return NULL;
    return &scr->StringBase[ Idx ];
}

void IntpNegArg( Intp_t *scr, char *Str )
{
    Str[ scr->Strings->w01 ] = -Str[ scr->Strings->w01 ];
    Str[ scr->Strings->w02 ] = 0;
}

void IntpValidate( Intp_t *scr )
{
/*
    IntpOp_t *Strings; // edx
    IntpOp_t i; // ecx
    IntpOp_t *p; // esi
    char *v7; // edx    short *Data; // ebx
    int ref; // edx
    int v8; // ecx
    int v9; // [esp+0h] [ebp-18h]

    Strings = scr->Strings;
    if( !Strings ) return;    
    p = (short *)Strings->Data;
    for( i = *Strings; p->w01 != 0x8000;  ){
        ref = p[ 0 ];
        if( ref < 0 ){
            ref = -ref;
            if( p->w02 ) eprintf( "Warning, reference count on invalid string is not zero!\n" );
        }
        p += &p->Data[ ref ];
        if( p > &scr->Strings->Data[ i ] ){
            SciUnk28();
            eprintf( 
        	"Program %s string table trashed, got length %d(%x)\nnext %d(%x), table end is %d(%x)\n", 
        	v7, v7, p - scr->Strings - 4, p - scr->Strings - 4, v8, v8, v9
    	    );
            exit( 1 );
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
    
    if( !Itp->Strings ) return;        
    for( opcode = (short *)Itp->Strings->Data; *opcode != 0x8000; opcode += Len1 + 4 ){
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
    if( intp->Strings ){
        for( v8 = (short *)intp->Strings->Data; v8[ 0 ] != 0x8000; v8 += Line + 4 ){
            Line = v8[ 0 ];
            if( Line >= 0 ){
                if( Line == v27 ){
                    if( !strcmp( a2, (char *)v8 + 4 ) ) return v8 - (short *)intp->Strings->Data + 2;
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
                    return v8 - (short *)intp->Strings->Data + 2;
                }
            }            
        }        
    } else {
        intp->Strings = dbg_malloc( 8 );
        ((short *)intp->Strings->Data)[ 0 ] = 0x8000;
        intp->Strings->Data[ 2 ] = 1;
        intp->Strings->w01 = 0;
    }
    intp->Strings = dbg_realloc( intp->Strings, v27 + intp->Strings->w01 + 8 + 4 );
    v20 = (short *)&intp->Strings->Data[ intp->Strings->w01 ];
    if( v20[ 0 ] != 0x8000 ) IntpError( "Internal consistancy error, string table mangled" );
    intp->Strings->w01 += v27 + 4;
    v20[ 1 ] = 0;
    v20[ 0 ] = v27;
    strcpy( (char *)v20 + 4, a2 );
    *((char *)&v20[ v27 + 3 ] ) = 0;
    v20[ v27 + 2 ] = 0x8000;
    v20[ v27 + 3 ] = 1;
    return v20 - (short *)intp->Strings->Data + 2;
}

void IntpGetOpcodeI( Intp_t *a1 )
{
    int InstrPtr;

    InstrPtr = a1->InstrPtr;
    a1->InstrPtr = InstrPtr + 4;
    IntpReadBei(a1->IntpData, InstrPtr);
    ScpA_Nop( a1 );
}

