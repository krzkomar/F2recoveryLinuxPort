#include "FrameWork.h"
/*
#define SCP_GETARG_A( type, val, p ) 	type = IntpPopShortStack( (p)->StackA, &(p)->StackApos ); val = IntpPopIntStack( (p)->StackA, &(p)->StackApos );
#define SCP_GETARG_B( type, val, p )    type = IntpPopwB( (p) ); val = IntpPopIntStack( (p)->StackB, &(p)->StackIdxB );
#define SCP_PUSHARG_A( type, val, p )	IntpPushIntStack( (p)->StackA, &(p)->StackApos, val ); IntpPushwA( (p), type );
#define SCP_PUSHARG_B( type, val, p )  	IntpPushIntStack( (p)->StackB, &(p)->StackIdxB, val ); IntpPushwB( (p), type );
#define SCP_ERRTYPE( cond, str )  if( cond ) IntpError( str )
#define SCP_ERRTYPEF( cond, s, fmt, m... )  if( cond ){ sprintf( s, fmt, ##m ); IntpError( s ); }
#define SCP_FSTRDEREF( type, val, scr ) if( (type) == SCR_FSTRING ) IntpStringDeRef( scr, SCR_FSTRING, val );
#define SCP_GETARG( type, ArgsCnt, scr ) SCP_GETARG_A( type, ArgsCnt, scr ); SCP_FSTRDEREF( type, ArgsCnt, scr );
#define SCP_GETARGF( type, Argi, Argf, scr)   SCP_GETARG( type, Argi, scr ); Argf = *((float *)(&Argi));
*/

int gScpUnk01 = 1;
extern int gSciUnk01;

/*
    name: o_nop()
    code: 0x8000
    ret: void
    dsc: do nothing
*/
void ScpA_Nop( Intp_t *scr )
{    
    /* Empty Function */
}

/*
    name: ?
    code 0x8001
    ret: ?
    dsc: ?
*/
void ScpA_8001( Intp_t *scr ) // push address pointer onto stack
{
printf("==>[%i] => 0x%x\n", scr->InstrPtr, BSWAP32(*(int *)(&scr->IntpData[ scr->InstrPtr ])));
    IntpPushIntStack( scr->StackA, &scr->StackApos, IntpReadBei( scr->IntpData, scr->InstrPtr ) );
    scr->InstrPtr += 4;
    IntpPushwA( scr, scr->Opcode );
}

void ScpA_802B( Intp_t *scr ) // 802B push_base( ArgsCnt ) ?
{
    short type;
    int ArgsCnt;

    SCP_GETARG( type, ArgsCnt, scr )
    SCP_PUSHARG_B( SCR_INT, scr->Base, scr );
    scr->Base = scr->StackApos - 6 * ArgsCnt;
}

void ScpA_PopBase( Intp_t *scr ) // 8029 'pop_base( base )'
{
    short type;
    int base;
    char stmp[ 256 ];

    SCP_GETARG_B( type, base, scr );
    SCP_ERRTYPEF( type != SCR_INT, stmp, "Invalid type given to pop_base: %x", type );
    scr->Base = base;
}

void ScpA_Ret( Intp_t *scr ) // 802A 'Ret( ... )
{
    short Type;
    int val;

    while( scr->StackApos != scr->Base ){
        SCP_GETARG( Type, val, scr );
    }    
}

void ScpA_802C( Intp_t *scr ) // 802C save_stack() ?
{
    scr->SaveStack = scr->StackApos;
}

void ScpA_Dump( Intp_t *scr ) // 802E dump( arg_cnt, ... )
{
    short type;
    int arg_cnt, argn;
    char stmp[ 256 ];

    SCP_GETARG_A( type, arg_cnt, scr );
    SCP_ERRTYPEF( type != SCR_INT, stmp, "Invalid type given to dump, %x", type );
    while( --arg_cnt != -1 ){
	SCP_GETARG( type, argn, scr );
    }
}

void ScpA_Call( Intp_t *scr ) // 8006 'call( procedure, procedure_time )
{
    short type;
    int addr, time, NameIdx;

    SCP_GETARG( type, addr, scr );
    SCP_ERRTYPE( SCRTYPE( type ) != SCR_INT, "Invalid procedure type given to call" );
    SCP_GETARG( type, time, scr );
    SCP_ERRTYPE( SCRTYPE( time ) != SCR_INT, "Invalid time given to call" );
    NameIdx = IntpReadBei( (char *)&scr->ProcTable[ addr ].NameOfst, 4 ) | 0x01;
    time = 1000 * time;
    if( !gSciUnk01 ) time += gIntpGetTime() / gIntpTimeDiv;    
    IntpWriteBew( time >> 16, 	(char *)&scr->ProcTable[ addr ].NameOfst, 8 );
    IntpWriteBew( time,       	(char *)&scr->ProcTable[ addr ].NameOfst, 10 );
    IntpWriteBew( NameIdx >> 16,(char *)&scr->ProcTable[ addr ].NameOfst, 4 );
    IntpWriteBew( NameIdx,     	(char *)&scr->ProcTable[ addr ].NameOfst, 6 );
}

void ScpA_CondCall( Intp_t *scr ) // 8007 cond_call( procedure, procedure_address )
{
    int ProcIdx, addr, NameIdx;
    short type;

    SCP_GETARG( type, ProcIdx, scr );
    SCP_ERRTYPE( SCRTYPE( type ) != SCR_INT, "Invalid procedure type given to conditional call" );
    SCP_GETARG( type, addr, scr );
    SCP_ERRTYPE( SCRTYPE( type ) != SCR_INT, "Invalid address given to conditional call" );
    NameIdx = IntpReadBei( (char *)&scr->ProcTable[ ProcIdx ].NameOfst, 4 ) | 0x02;
    IntpWriteBew( NameIdx >> 16,(char *)&scr->ProcTable[ ProcIdx ].NameOfst, 4 );
    IntpWriteBew( NameIdx,      (char *)&scr->ProcTable[ ProcIdx ].NameOfst, 6 );
    IntpWriteBew( addr >> 16, 	(char *)&scr->ProcTable[ ProcIdx ].NameOfst, 12 );
    IntpWriteBew( addr,       	(char *)&scr->ProcTable[ ProcIdx ].NameOfst, 14 );
}

void ScpA_Wait( Intp_t *scr ) // 8047 'wait( time )'
{
    short type;
    int time;

    SCP_GETARG( type, time, scr );
    SCP_ERRTYPE( SCRTYPE( type ) != SCR_INT, "Invalid type given to wait\n" );
    scr->TimeA = 1000 * gIntpGetTime() / gIntpTimeDiv;
    scr->Time = scr->TimeA + time;
    scr->Func = IntpTimer;
    scr->Flags |= 0x10;
}

void ScpA_Cancel( Intp_t *scr ) // 8048 'cancel( proc )'
{
    short type;
    int ProcOffs;

    SCP_GETARG( type, ProcOffs, scr );
    SCP_ERRTYPE( SCRTYPE( type ) != SCR_INT, "invalid type given to cancel" );
    SCP_ERRTYPE( ProcOffs >= IntpReadBei( (char *)&scr->ProcTable->Count, 0), "Invalid procedure offset given to cancel" );
    IntpWriteBew( 0, (char *)&scr->ProcTable[ ProcOffs ].Flags, 0 );
    IntpWriteBew( 0, (char *)&scr->ProcTable[ ProcOffs ].Flags, 2 );
    IntpWriteBew( 0, (char *)&scr->ProcTable[ ProcOffs ].Delay, 0 );
    IntpWriteBew( 0, (char *)&scr->ProcTable[ ProcOffs ].Delay, 2 );
    IntpWriteBew( 0, (char *)&scr->ProcTable[ ProcOffs ].Delay, 0 );
    IntpWriteBew( 0, (char *)&scr->ProcTable[ ProcOffs ].Delay, 2 );
}

void ScpA_CancelAll( Intp_t *scr ) // 8049 'CancelAll()'
{
    int i, Arg1;
    
    Arg1 = IntpReadBei( (char *)&scr->ProcTable->Count, 0 );
    for( i = 0; i < Arg1; i++ ){
        IntpWriteBew( 0, (char *)&scr->ProcTable[ i ].Flags, 0 );
        IntpWriteBew( 0, (char *)&scr->ProcTable[ i ].Flags, 2 );
        IntpWriteBew( 0, (char *)&scr->ProcTable[ i ].Delay, 0 );
        IntpWriteBew( 0, (char *)&scr->ProcTable[ i ].Delay, 2 );
        IntpWriteBew( 0, (char *)&scr->ProcTable[ i ].CntOfst, 0 );
        IntpWriteBew( 0, (char *)&scr->ProcTable[ i ].CntOfst, 2 );
    }
}

void ScpA_If( Intp_t *scr ) // 802F ifcon( Cond, skip_addr ) -> 'if( Cond ) then begin'
{
    short type;
    int Cond, addr;

    SCP_GETARG( type, Cond, scr );
    if( Cond ){
        SCP_GETARG( type, addr, scr );
    } else {
        SCP_GETARG( type, addr, scr );
        scr->InstrPtr = addr;
    }
}

void ScpA_WhileDo( Intp_t *scr ) // 8030 while( Cond, SkipAddress ) -> 'while( Cond ) do begin'
{
    short type;
    int Cond, BreakAddr;

    SCP_GETARG( type, Cond, scr );
    if( !Cond ){
	SCP_GETARG( type, BreakAddr, scr );
        scr->InstrPtr = BreakAddr;
    }
}

void ScpA_8031( Intp_t *scr ) // 8031 ?( arg1, arg2 )
{
    short Type1, Type2;
    int Arg1, Arg2, k;
    char *stack;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    k = IntpReadBei( scr->StackA, 6 * Arg1 + scr->Base );
    if( *(&scr->StackA[3 * Arg1 + 2] + scr->Base) == 0x0198 ) IntpStringDeRef( scr, SCR_FSTRING, k );
    IntpWriteBew( Arg2 >> 16, scr->StackA, 6 * Arg1 + scr->Base );
    IntpWriteBew( Arg2, scr->StackA, 6 * Arg1 + scr->Base + 2 );
    stack = &scr->StackA[ 3 * Arg1 + 2 ] + scr->Base;
    stack[ 0 ] = Type2 >> 16;
    stack[ 1 ] = Type2;
    if( Type2 == SCR_FSTRING ) ++scr->Strings->Data[ Arg2 - 2 ];
}

void ScpA_Fetch( Intp_t *scr ) // 8032
{
    unsigned short Type1;
    int Arg1, k;
    char stmp[264];

    SCP_GETARG( Type1, Arg1, scr );
    SCP_ERRTYPEF( Type1 != SCR_INT, stmp, "Invalid type given to fetch, %x", Type1 );
    k = IntpReadBei( scr->StackA, 6 * Arg1 + scr->Base );
    SCP_PUSHARG_A( (*(&scr->StackA[3 * Arg1 + 2] + scr->Base) << 8) | *(&scr->StackA[3 * Arg1 + 2] + scr->Base + 1), k, scr );
}

void ScpA_CmpNE( Intp_t *scr ) // 8034 'stack:=p2<>p1'
{
    unsigned short Type1, Type2;
    float Arg1f, Arg2f;
    int Arg2i,Arg1i;
    char *s, stmp1[80], stmp2[80], stmp3[80], stmp4[80];

    SCP_GETARGF( Type1, Arg1i, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2i, Arg2f, scr );
    if( ( Type2 == SCR_FSTRING ) || ( Type2 == SCR_STRING ) ){ // int
        if( Type1 < SCR_FSTRING ) {
            if( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ) {
            if( Type1 < SCR_FLOAT ) goto LABEL_55;
            if( Type1 == SCR_FLOAT ){
                sprintf( stmp1, "%.05f", Arg1f );
                IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp(stmp1, IntpGetArg( scr, Type2 >>8, Arg2i ) ) != 0);
            } else {
                if( Type1 == SCR_INT ){
            	    if( Arg1f == 0.0 ){
        		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2i != 0 );
        	    } else {
            		sprintf( stmp3, "%d", Arg1i );
            		IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp(stmp3, IntpGetArg(scr, Type2 >> 8, Arg2i )) != 0);
            	    }
                }
            }
        } else {
    	    s = ( (Type2 & 0x800) != 0 ) ? scr->Strings[ Arg2i ].Data : scr->Floats[ Arg2i ].Data;
    	    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( s, IntpGetArg(scr, Type1 >> 8, Arg1i )) != 0 );
        }
        goto LABEL_55;
    }
    if( Type2 < SCR_FSTRING ) goto LABEL_55;

    if( Type2 == SCR_FLOAT ){ // float
        if( Type1 < SCR_FSTRING ){
            if( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ){
    	    if( Type1 == SCR_FLOAT ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f != Arg1f );
            if( Type1 == SCR_INT ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1f != Arg2f );
        } else {
    	    sprintf( stmp2, "%.05f", Arg2f );
    	    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp2, IntpGetArg( scr, Type1 >> 8 , Arg1i ) ) != 0);
    	}
    } else {
	if( Type2 == SCR_INT ){
	    if( Type1 == SCR_FSTRING || ( Type1 == SCR_STRING )){
    		if( Arg2f == 0.0 ){
        	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1i != 0 );
    		} else {
    		    sprintf( stmp4, "%d", Arg2i );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp4, IntpGetArg( scr, Type1 >> 8, Arg1i ) ) != 0 );
    		}
	    } else {
		if( Type1 == SCR_FLOAT ){
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f != Arg1f );
		} else {
    		    if( Type1 == Type2 ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f != Arg1i );
		}
	    }
	}
    }
    goto LABEL_55;
LABEL_55:
    IntpPushwA( scr, SCR_INT );
    return;
}

void ScpA_CmpEQ( Intp_t *scr ) // 8033 'stack:= p2==p1'
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *s, stmp1[80], stmp2[80], stmp3[80], stmp4[80];
    float Arg2f,Idx;

    SCP_GETARGF( Type1, Arg1, Idx, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FSTRING ){
        if( Type2 == SCR_STRING ) goto LABEL_37;
        goto LABEL_55;        
    }
    if( Type2 <= SCR_FSTRING ){
LABEL_37:
        if( Type1 < SCR_FSTRING ) {
            if( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_55;
            if( Type1 <= SCR_FLOAT ){
                sprintf( stmp1, "%.05f", *&Idx );
                IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp(stmp1, IntpGetArg( scr, Type2 >> 8, Arg2f )) == 0 );
            } else {
                if( Type1 != SCR_INT ) goto LABEL_55;
                if( Idx == 0.0 ){
        	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f == 0 );
                } else {
        	    sprintf( stmp3, "%d", Arg1 );
            	    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp(stmp3, IntpGetArg( scr, Type2 >> 8, Arg2f ) ) == 0 );
                }
            }            
            goto LABEL_55;
        }
        s = ( (Type2 & 0x800) != 0 ) ? scr->Strings[ Arg2 ].Data : scr->Floats[ Arg2 ].Data;
        IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( s, IntpGetArg( scr, Type1 >> 8, Idx )) == 0 );
        goto LABEL_55;
    }
    if( Type2 < SCR_FLOAT ) goto LABEL_55;
    if( Type2 <= SCR_FLOAT ){
        if( Type1 < SCR_FSTRING ){
            if( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_55;
            if( Type1 <= SCR_FLOAT ){
                IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f == Idx );
            } else {
                if( Type1 == SCR_INT ) IntpPushIntStack( scr->StackA, &scr->StackApos, Idx == Arg2f );
            }            
            goto LABEL_55;
        }
        sprintf( stmp2, "%.05f", Arg2f );
        IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp2, IntpGetArg( scr, Type1 >> 8, Idx )) == 0 );
        goto LABEL_55;
    }
    if( Type2 != SCR_INT ){
    	IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Type1 < SCR_FSTRING ){
        if( Type1 != SCR_STRING ){
    	    IntpPushwA( scr, SCR_INT );
	    return;
	}
    }
    if( Type1 > SCR_FSTRING ){
	if( Type1 >= SCR_FLOAT ){
    	    if( Type1 == SCR_FLOAT ){
        	IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f == Idx );
    	    } else {
        	if( Type1 == Type2 ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f == Idx );
    	    }        
	}
        IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Arg2f == 0.0 ){
        IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 == 0 );
    } else {
    	sprintf( stmp4, "%d", Arg2 );
	IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp4, IntpGetArg( scr, Type1 >> 8, Arg1 ) ) == 0 );
    }
    IntpPushwA( scr, SCR_INT );
    return;    
LABEL_55:
    IntpPushwA( scr, SCR_INT );
}

void ScpA_CmpLE( Intp_t *scr ) // 8035 'stack:= p2<=p1'
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg1f, Arg2f;
    char *s, stmp1[80], stmp2[80], stmp3[80], stmp4[80];

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FSTRING ){
        if( Type2 != SCR_STRING ) goto LABEL_55;
        goto LABEL_37;
    }
    if( Type2 <= SCR_FSTRING ){
LABEL_37:
        if( Type1 < SCR_FSTRING ){
            if( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_55;
            if( Type1 <= SCR_FLOAT ){
                sprintf( stmp1, "%.05f", Arg1f );
                IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp1, IntpGetArg( scr, Type2 >> 8, Arg2 ) ) <= 0 );
            } else {
                if( Type1 != SCR_INT ) goto LABEL_55;
                if( Arg1 == 0.0 ){
        	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 <= 0 );
                } else {
            	    sprintf( stmp3, "%d", Arg1 );
            	    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp3, IntpGetArg( scr, Type2 >> 8, Arg2 )) <= 0 );
                }                
            }    	    
    	    goto LABEL_55;
        }
        s = ( (Type2 & 0x800) != 0 ) ? scr->Strings[ Arg2 ].Data :  scr->Floats[Arg2 ].Data;
        IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( s, IntpGetArg( scr, Type1 > 8, Arg1 )) <= 0 );
        goto LABEL_55;
    }
    if( Type2 < SCR_FLOAT ) goto LABEL_55;
    if( Type2 <= SCR_FLOAT ){
        if( Type1 < SCR_FSTRING ){
            if( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_55;
            if( Type1 <= SCR_FLOAT ){
                IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 <= Arg1 );
            } else {
                if( Type1 != SCR_INT ) goto LABEL_55;
                IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 >= Arg2 );
            }	    
	    goto LABEL_55;
        }
        sprintf( stmp2, "%.05f", Arg2f );
    	IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp2, IntpGetArg(scr, Type1 >> 8, Arg1 ) ) <= 0 );
    	goto LABEL_55;
    }
    if( Type2 != SCR_INT ) goto LABEL_55;
    if( Type1 < SCR_FSTRING ){
        if( Type1 != SCR_STRING ) goto LABEL_55;
        goto LABEL_23;
    }
    if( Type1 <= SCR_FSTRING ){
LABEL_23:
        if( Arg2 == 0.0 ){
            IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 <= 0 );
        } else {
    	    sprintf( stmp4, "%d", Arg2 );
    	    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp4, IntpGetArg(scr, Type1 >> 8, Arg1 ) ) <= 0 );
    	}
	IntpPushwA(scr, SCR_INT);
	return;
    }
    if( Type1 >= SCR_FLOAT ){
        if ( Type1 <= SCR_FLOAT ){
            IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f <= Arg1f );
        } else {
            if( Type1 == Type2 ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 <= Arg1 );
        }	
	goto LABEL_55;
    }
LABEL_55:
    IntpPushwA(scr, SCR_INT);
}

void ScpA_CmpGE( Intp_t *scr ) // 8036 'stack:=p2>=p1'
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *s, stmp1[80], stmp2[80], stmp3[80], stmp4[80];
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FSTRING ){
        if( Type2 != SCR_STRING ) goto LABEL_55;
        goto LABEL_37;
    }
    if( Type2 <= SCR_FSTRING ){
LABEL_37:
        if( Type1 < SCR_FSTRING ){
            if( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_55;
            if( Type1 <= SCR_FLOAT ){
                sprintf( stmp1, "%.05f", Arg1f );
                IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp1, IntpGetArg( scr, Type2 >> 8, Arg2f ) ) >= 0 );
            } else {
                if( Type1 != SCR_INT ) goto LABEL_55;
                if( Arg1 == 0.0 ){
        	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f >= 0);
                } else {
            	    sprintf( stmp3, "%d", Arg1 );
            	    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp3, IntpGetArg(scr, Type2 >> 8, Arg2f)) >= 0 );
                }
            }    	    
    	    goto LABEL_55;
        }
        s = ( (Type2 & 0x800) != 0 ) ? scr->Strings[ Arg2 ].Data : scr->Floats[ Arg2 ].Data;
    	IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( s, IntpGetArg(scr, Type1 >> 8, Arg1)) >= 0 );
        goto LABEL_55;
    }
    if( Type2 < SCR_FLOAT ) goto LABEL_55;
    if( Type2 <= SCR_FLOAT ){
        if( Type1 < SCR_FSTRING ){
            if( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_55;
            if( Type1 <= SCR_FLOAT ){
    		IntpPushIntStack( scr->StackA, &scr->StackApos, *&Arg2f >= *&Arg1 );
            } else {
                if( Type1 != SCR_INT ) goto LABEL_55;
    		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 <= *&Arg2f );
            }
            goto LABEL_55;
        }
        sprintf( stmp2, "%.05f", Arg2f );
    	IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp(stmp2, IntpGetArg(scr, Type1 >> 8, Arg1)) >= 0 );
    	goto LABEL_55;
    }
    if( Type2 != SCR_INT ) goto LABEL_55;
    if( Type1 < SCR_FSTRING ){
        if( Type1 == SCR_STRING ) goto LABEL_23;
        goto LABEL_55;        
    }
    if( Type1 <= SCR_FSTRING ){
LABEL_23:
        if( Arg2f == 0.0 ){
            IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 >= 0 );
        } else {
    	    sprintf( stmp4, "%d", Arg2 );
    	    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp(stmp4, IntpGetArg(scr, Type1 >> 8, Arg1)) >= 0 );
    	}
    	goto LABEL_55;
    }
    if( Type1 >= SCR_FLOAT ){
        if( Type1 <= SCR_FLOAT ){
            IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f >= Arg1 );
        } else {
            if( Type1 == Type2 ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f >= Arg1 );
        }    	
        goto LABEL_55;
    }
LABEL_55:
    IntpPushwA( scr, SCR_INT );
}

void ScpA_CmpLS( Intp_t *scr ) // 8037 'stack:=p2<p1'
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *s, stmp1[80], stmp2[80], stmp3[80], stmp4[80];
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FSTRING ){
        if( Type2 == SCR_STRING ) goto LABEL_37;
        goto LABEL_55;        
    }
    if( Type2 <= SCR_FSTRING ){
LABEL_37:
        if( Type1 < SCR_FSTRING ){
            if ( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_55;
            if( Type1 <= SCR_FLOAT ){
                sprintf( stmp1, "%.05f", Arg1f );
                IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp1, IntpGetArg( scr, Type2 >> 8, Arg2 ) ) < 0 );
            } else {
                if( Type1 != SCR_INT ) goto LABEL_55;
                if( Arg1f == 0.0 ){
        	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f < 0);
        	    goto LABEL_55;
                }
                sprintf( stmp3, "%d", Arg1 );
                IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp3, IntpGetArg( scr, Type2 >> 8, Arg2 ) ) < 0 );
            }
    	    goto LABEL_55;
        }
        s = ( (Type2 & 0x800) != 0 ) ? scr->Strings[ Arg2 ].Data : scr->Floats[ Arg2 ].Data;
    	IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( s, IntpGetArg(scr, Type1 >> 8, Arg1) ) < 0 );
        goto LABEL_55;
    }
    if( Type2 < SCR_FLOAT ) goto LABEL_55;
    if( Type2 <= SCR_FLOAT ){
        if( Type1 < SCR_FSTRING ){
            if( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_55;
            if( Type1 <= SCR_FLOAT ){
                IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f < Arg1 );
            } else {
                if( Type1 != SCR_INT ) goto LABEL_55;
                IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 > Arg2f );
            }    	    
            goto LABEL_55;
        }
        sprintf( stmp2, "%.05f", *&Arg2f );
    	IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp2, IntpGetArg( scr, Type1 >> 8, Arg1 ) ) < 0 );
        goto LABEL_55;
    }
    if( Type2 != SCR_INT ) goto LABEL_55;
    if( Type1 < SCR_FSTRING ){
        if( Type1 == SCR_STRING ) goto LABEL_23;
        goto LABEL_55;        
    }
    if( Type1 <= SCR_FSTRING ){
LABEL_23:
        if( Arg2f == 0.0 ){
            IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 < 0 );
            goto LABEL_55;
        }
        sprintf( stmp4, "%d", Arg2 );
    	IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp4, IntpGetArg(scr, Type1 >> 8, Arg1) ) < 0);
        goto LABEL_55;
    }
    if( Type1 >= SCR_FLOAT ){
        if( Type1 <= SCR_FLOAT ){
            IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f < Arg1 );
        } else {
            if( Type1 == Type2 ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f < Arg1 );
        }    	
        goto LABEL_55;
    }
LABEL_55:
    IntpPushwA( scr, SCR_INT );
}
        
void ScpA_CmpGT( Intp_t *scr ) // 8038 'stack:=p2>p1''
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    char *s, stmp1[80], stmp2[80], stmp3[80], stmp4[80];
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FSTRING ){
        if( Type2 == SCR_STRING ) goto LABEL_37;
        goto LABEL_55;        
    }
    if( Type2 <= SCR_FSTRING ){
LABEL_37:
        if( Type1 < SCR_FSTRING ){
            if( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_55;
            if( Type1 <= SCR_FLOAT ){
                sprintf( stmp1, "%.05f", Arg1f );
                IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp1, IntpGetArg( scr, Type2 >> 8, Arg2 ) ) > 0);
            } else {
                if( Type1 != SCR_INT ) goto LABEL_55;
                if( Arg1f == 0.0 ){
        	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 > 0 );
                } else {
            	    sprintf( stmp3, "%d", Arg1 );
            	    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp3, IntpGetArg( scr, Type2 >> 8, Arg2 ) ) > 0);
                }
            }    	    
    	    goto LABEL_55;
        }  
        s = ( (Type2 & 0x800) != 0 ) ? scr->Strings[ Arg2 ].Data : scr->Floats[ Arg2 ].Data;
        IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( s, IntpGetArg( scr, Type1 >> 8, Arg1 ) ) > 0);
        IntpPushwA( scr, SCR_INT );
        return;
    }
    if( Type2 < SCR_FLOAT ) goto LABEL_55;
    if( Type2 <= SCR_FLOAT ){
        if( Type1 < SCR_FSTRING ){
            if( Type1 != SCR_STRING ) goto LABEL_55;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_55;
            if( Type1 <= SCR_FLOAT ){
                IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f > Arg1f );
            } else {
                if( Type1 == SCR_INT ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 < Arg2f );
                IntpPushwA( scr, SCR_INT );
                return;   
            }            
        } else {
    	    sprintf( stmp2, "%.05f", Arg2f );
    	    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp2, IntpGetArg(scr, Type1 >> 8, Arg1 ) ) > 0);
    	    IntpPushwA( scr, SCR_INT );
        }
        return;
    }
    if( Type2 != SCR_INT ){
	IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Type1 < SCR_FSTRING ){
        if( Type1 != SCR_STRING ){
	    IntpPushwA( scr, SCR_INT );
	    return;
        }
    } else {
	if( Type1 > SCR_FSTRING ){
	    if( Type1 >= SCR_FLOAT ){
    		if( Type1 == SCR_FLOAT ){
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f > Arg1f );
    		} else {
        	    if( Type1 == Type2 ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 > Arg1 );
    		}
		IntpPushwA( scr, SCR_INT );
    		return;
	    }
	    IntpPushwA( scr, SCR_INT );
	    return;
	}
    }
    if( Arg2 == 0.0 ){
        IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 > 0 ); IntpPushwA( scr, SCR_INT );
	return;
    }
    sprintf( stmp4, "%d", Arg2 );
    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp4, IntpGetArg( scr, Type1 >> 8, Arg1 ) ) > 0); IntpPushwA( scr, SCR_INT );
    return;

LABEL_55:
    IntpPushwA( scr, SCR_INT );
    return;
}

void ScpA_Add( Intp_t *scr ) // 8039
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;
    unsigned int t2, t1;
    char *s2, *pd, *p, *q;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    t2 = Type2 & 0xF7FF;
    if( t2 < SCR_FLOAT ){
        if( t2 != SCR_STRING ) return;
        if( (Type2 & 0x800) != 0 ){
            s2 = &scr->Strings->Data[ Arg2 ];
        } else if( (Type2 & 0x1000) != 0 ){
            s2 = &scr->Floats->Data[ Arg2 ];
        } else {
            s2 = 0;
        }
        t1 = Type1 & 0xF7FF;
        if( t1 < SCR_FLOAT ){
            if( t1 == SCR_STRING ){
                if( Type1 & 0x800 ){
                    pd = &scr->Strings->Data[ Arg1 ];
                } else if( Type1 & 0x1000 ){
                    pd = &scr->Floats->Data[ Arg1 ];
                } else {
                    pd = 0;
                }
                q = dbg_malloc( strlen( pd ) + 1 );
                strcpy( q, pd );
            }
        } else if ( t1 <= SCR_FLOAT ) {
            q = dbg_malloc( 80 );
            sprintf( q, "%.5f", Arg1f );
        } else if ( t1 == SCR_INT ){
            q = dbg_malloc( 80 );
            sprintf( q, "%d", Arg1 );
        }
        p = dbg_malloc( strlen( s2 ) + 1 + strlen( q ) );
        strcpy( p, s2 );
        strcpy( p + strlen( p ), q );
        IntpPushIntStack( scr->StackA, &scr->StackApos, IntpDbgStr( scr, p, __LINE__ ) ); IntpPushwA( scr, SCR_FSTRING );
        dbg_free( q );
        dbg_free( p );
        return;
    }
    if( t2 > SCR_FLOAT ){
        if( t2 != SCR_INT ) return;
        t1 = Type1 & 0xF7FF;
        if( t1 >= SCR_FLOAT ){
            if( t1 <= SCR_FLOAT ){
                IntpPushIntStack(scr->StackA, &scr->StackApos, Arg2 + Arg1 ); IntpPushwA( scr, SCR_FLOAT );
            } else if( t1 == SCR_INT ){
                if( (Arg1 <= 0 || 0x7FFFFFFF - Arg1 >= Arg2) && (Arg1 >= 0 || (0x80000001 - Arg1) <= Arg2) ){
                    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 + Arg2 ); IntpPushwA( scr, SCR_INT );
                } else {
                    IntpPushIntStack(scr->StackA, &scr->StackApos, Arg2 + Arg1 ); IntpPushwA( scr, SCR_FLOAT );
                }
            }
            return;
        }
        if( t1 != SCR_STRING ) return;
        if( (Type1 & 0x800) != 0 ){
            pd = &scr->Strings->Data[ Arg1 ];
        } else if( (Type1 & 0x1000) != 0 ){
            pd = &scr->Floats->Data[ Arg1 ];
        } else {
            pd = NULL;
        }
        p = dbg_malloc( strlen(pd) + 80 );
        sprintf( p, "%d", Arg2 );
        strcpy( p + strlen( p ), pd );
        IntpPushIntStack( scr->StackA, &scr->StackApos, IntpDbgStr( scr, p, __LINE__ ) ); IntpPushwA( scr, SCR_FSTRING );
        dbg_free( p );
        return;
    }
    t1 = Type1 & 0xF7FF;
    if( t1 < SCR_FLOAT ){
        if( t1 != SCR_STRING ) return;
        if( Type1 & 0x800 ){
            pd = &scr->Strings->Data[ Arg1 ];
        } else if( Type1 & 0x1000  ){
            pd = &scr->Floats->Data[ Arg1 ];
        } else {
            pd = NULL;
        }
        p = dbg_malloc( strlen( pd ) + 80 );
        sprintf( p, "%.5f", Arg2f );
        strcpy( p + strlen( p ), pd );
        IntpPushIntStack(scr->StackA, &scr->StackApos, IntpDbgStr( scr, p, __LINE__ ) ); IntpPushwA( scr, SCR_FSTRING );
        dbg_free( p );
        return;
    }
    if( t1 <= SCR_FLOAT ){
        IntpPushIntStack(scr->StackA, &scr->StackApos, Arg2 + Arg1 ); IntpPushwA(scr, SCR_FLOAT);
    } else if( t1 == SCR_INT ){
        IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 + Arg2 ); IntpPushwA( scr, SCR_FLOAT );
    }
}

void ScpA_Sub( Intp_t *scr ) // 803A
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FLOAT ) return;
    if( Type2 == SCR_FLOAT ){ // float - float/int
        IntpPushIntStack( scr->StackA, &scr->StackApos, ( Type1 == SCR_FLOAT ) ? Arg2f - Arg1f : Arg2f - Arg1 );
        IntpPushwA( scr, SCR_FLOAT );
    } else { // ? - int
        if( Type2 != SCR_INT ) return;
        if( Type1 == SCR_FLOAT ){ // int - float
    	IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 - Arg1f );	IntpPushwA( scr, SCR_FLOAT );
        } else { // int - int
            IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 - Arg1 ); IntpPushwA( scr, SCR_INT );
        }
    }    
}

void ScpA_Mul( Intp_t *scr ) // 803B
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FLOAT ) return;
    if( Type2 == SCR_FLOAT ){
        IntpPushIntStack( scr->StackA, &scr->StackApos, ( Type1 == SCR_FLOAT ) ? Arg2f * Arg1f : Arg1f * Arg2f );
        IntpPushwA( scr, SCR_FLOAT );
    } else { // ? * int
        if( Type2 != SCR_INT ) return;
        if( Type1 == SCR_FLOAT ){
    	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 * Arg1f ); IntpPushwA( scr, SCR_FLOAT );
        } else { // int * int
            IntpPushIntStack(scr->StackA, &scr->StackApos, Arg1f * Arg2 ); IntpPushwA( scr, SCR_INT );
        }
    }            
}

void ScpA_Div( Intp_t *scr ) // 803C
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f, tmp;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FLOAT ) return;
    if( Type2 == SCR_FLOAT ){
        tmp = ( Type1 == SCR_FLOAT ) ? Arg1f : (float)Arg1;
        SCP_ERRTYPE( tmp == 0.0, "Division (DIV) by zero" );
        IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f / tmp );
        IntpPushwA( scr, SCR_FLOAT );
        return;
    }
    if( Type2 != SCR_INT ) return;
    if( Type1 == SCR_FLOAT ){
        SCP_ERRTYPE( Arg1 == 0.0, "Division (DIV) by zero" );
        IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 / Arg1f );
        IntpPushwA( scr, SCR_FLOAT );
    } else {
        SCP_ERRTYPE( Arg1 == 0, "Division (DIV) by zero" );
        IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 / Arg1 );
        IntpPushwA( scr, SCR_INT );
    }    
}

void ScpA_Mod( Intp_t *scr ) // 803D
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    if( Type2 < SCR_FLOAT ) return;
    if( Type2 <= SCR_FLOAT ){
        IntpError( "Trying to MOD a float" );        
    } else {
        if( Type2 != SCR_INT ) return;
        if( Type1 == SCR_FLOAT ){
    	    IntpError( "Trying to MOD with a float" );
        } else {
            if( !Arg1 ) IntpError( "Division (MOD) by zero" );
            IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 % Arg1 );
    	    IntpPushwA( scr, SCR_INT );
    	    return;
        }        
    }
    IntpPushIntStack( scr->StackA, &scr->StackApos, 0 );
    IntpPushwA( scr, SCR_FLOAT );    
}

void ScpA_LogAnd( Intp_t *scr ) // 803E
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FSTRING ){
        if( Type2 != SCR_STRING ) goto LABEL_60;
        if( Type1 < SCR_FSTRING ){
            if( Type1 != SCR_STRING ) goto LABEL_60;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_60;
            if( Type1 > SCR_FLOAT ){
                if( Type1 == SCR_INT ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1f != 0 );
            } else {
    		IntpPushIntStack( scr->StackA, &scr->StackApos, (Arg1f != 0.0) );
    	    }
    	    goto LABEL_60;
        }
        IntpPushIntStack( scr->StackA, &scr->StackApos, 1 );
        goto LABEL_60;
    }
    if( Type2 <= SCR_FSTRING ){
        if( Type1 < SCR_FSTRING ){
            if( Type1 != SCR_STRING ) goto LABEL_60;
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 < SCR_FLOAT ) goto LABEL_60;
            if( Type1 > SCR_FLOAT ){
                if( Type1 != SCR_INT ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1f != 0 );
            } else {
    		IntpPushIntStack( scr->StackA, &scr->StackApos, (Arg1f != 0.0) );
    	    }
    	    goto LABEL_60;
        }
        IntpPushIntStack( scr->StackA, &scr->StackApos, 1 );
        goto LABEL_60;
    }
    if( Type2 < SCR_FLOAT ) goto LABEL_60;
    if( Type2 == SCR_FLOAT ){
        if( Type1 < SCR_FSTRING ){ // float AND str
            if( Type1 == SCR_STRING ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f != 0 );
	    IntpPushwA( scr, SCR_INT );
	    return;
        }
        if( Type1 == SCR_FSTRING ){ // float AND fstr
    	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f != 0 );
	    IntpPushwA( scr, SCR_INT );
	    return;
        }
        if( Type1 < SCR_FLOAT ) goto LABEL_60;
        if( Type1 == SCR_FLOAT ){ // float AND float
            IntpPushIntStack( scr->StackA, &scr->StackApos, (Arg2f != 0.0) && (Arg1f != 0.0) );
        } else { // float AND int
            if( Type1 == SCR_INT ) IntpPushIntStack( scr->StackA, &scr->StackApos, (Arg2f != 0.0 )  && Arg1 );
        }        
	IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Type2 != SCR_INT ) goto LABEL_60;
    if( Type1 < SCR_FSTRING ){ // int AND str
        if( Type1 == SCR_STRING ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 != 0 );
	IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Type1 == SCR_FSTRING ){ // int AND fstr
        IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 != 0 );
	IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Type1 >= SCR_FLOAT ){
        if( Type1 == SCR_FLOAT ){ // int AND float
            IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 && (Arg1f != 0.0) );
        } else { // int AND int
            if( Type1 == SCR_INT ) IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 && Arg1 );
        }        
	IntpPushwA( scr, SCR_INT );
	return;
    }
LABEL_60:
    IntpPushwA( scr, SCR_INT );
    return;
}

void ScpA_LogOr( Intp_t *scr ) // 803F ||( p1 , p2 )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FSTRING ){
        if( Type2 == SCR_STRING ){
    	    if( Type1 < SCR_FSTRING ){
        	if( Type1 == SCR_STRING ) IntpPushIntStack( scr->StackA, &scr->StackApos, 1 );
    	    } else if( Type1 > SCR_FSTRING ){
        	if( Type1 >= SCR_FLOAT ){
        	    if( Type1 == SCR_FLOAT || Type1 == SCR_INT ) IntpPushIntStack( scr->StackA, &scr->StackApos,  1 );
        	}
    	    } else {
    		IntpPushIntStack( scr->StackA, &scr->StackApos, 1 );
    	    }
        }
	IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Type2 <= SCR_FSTRING ){
        if( Type1 < SCR_FSTRING ){
            if( Type1 == SCR_STRING ) IntpPushIntStack( scr->StackA, &scr->StackApos, 1 );
        } else if( Type1 > SCR_FSTRING ){
            if( Type1 >= SCR_FLOAT ){
        	if( Type1 == SCR_FLOAT || Type1 == SCR_INT ) IntpPushIntStack( scr->StackA, &scr->StackApos,  1 );
            }
        } else {
    	    IntpPushIntStack( scr->StackA, &scr->StackApos, 1 );
        }
	IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Type2 < SCR_FLOAT ){
	IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Type2 > SCR_FLOAT ){
	if( Type2 == SCR_INT ){
	    if( Type1 < SCR_FSTRING ){
    		if( Type1 == SCR_STRING ) IntpPushIntStack( scr->StackA, &scr->StackApos, 1 );
		IntpPushwA( scr, SCR_INT );
		return;
	    }
	    if( Type1 <= SCR_FSTRING ){
		IntpPushIntStack( scr->StackA, &scr->StackApos, 1 );
		IntpPushwA( scr, SCR_INT );
		return;
	    }
	    if( Type1 >= SCR_FLOAT ){
    		if( Type1 <= SCR_FLOAT ){
        	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f || (Arg1f != 0.0) );
    		} else {
    		    if( Type1 == SCR_INT ){
        		if( Arg2f ){
        		    IntpPushIntStack( scr->StackA, &scr->StackApos, 1 );
        		} else {
        		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1f ? 1:0 );
        		}            
        	    }
    		}        
	    }
	}
	IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Type1 < SCR_FSTRING ){
        if( Type1 == SCR_STRING ) IntpPushIntStack( scr->StackA, &scr->StackApos, 1 );
	IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Type1 == SCR_FSTRING ){
        IntpPushIntStack( scr->StackA, &scr->StackApos, 1 );
	IntpPushwA( scr, SCR_INT );
	return;
    }
    if( Type1 >= SCR_FLOAT ){
        if( Type1 == SCR_FLOAT ){
    	    IntpPushIntStack( scr->StackA, &scr->StackApos, (Arg2f != 0.0) || (Arg1f != 0.0 ) ); IntpPushwA( scr, SCR_INT );
        } else {
    	    if( Type1 == SCR_INT ){
    		if( Arg2f != 0.0 ) Arg1f = 1;                    
    		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1f ? 1:0 );
    	    }
        }
    }
    IntpPushwA( scr, SCR_INT );
    return;
}

void ScpA_LogNot( Intp_t *scr ) // 8045 !( arg )
{
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_PUSHARG_A( SCR_INT, Arg == 0, scr );
}

void ScpA_Neg( Intp_t *scr ) // 8046 -( arg )
{
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_PUSHARG_A( SCR_INT, -Arg, scr );
}

void ScpA_BinNot( Intp_t *scr ) // 8043 ~( arg )
{
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_PUSHARG_A( SCR_INT, ~Arg, scr );
}

void ScpA_Floor( Intp_t *scr ) // 8044 floor( float arg )
{
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    if( Type < SCR_FLOAT ){
        SCP_ERRTYPE( Type == SCR_STRING, "Invalid arg given to floor()" );
    } else if( Type == SCR_FLOAT ){
        Type = SCR_INT;
        Arg = lround( *((float *)&Arg ) );
    }
    SCP_PUSHARG_A( Type, Arg, scr );
}

void ScpA_BinAnd( Intp_t *scr ) // 8040 &( p1, p2 )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FLOAT ) return;
    if( Type2 == SCR_FLOAT ){
        if( Type1 == SCR_FLOAT ){
    	    SCP_PUSHARG_A( SCR_INT, lround( Arg2f ) & lround( Arg1f ), scr );
        } else {
    	    SCP_PUSHARG_A( SCR_INT, Arg1 & lround( Arg2f ), scr );
        }	
    } else {
        if( Type2 != SCR_INT ) return;
        if( Type1 == SCR_FLOAT ){
    	    SCP_PUSHARG_A( SCR_INT, lround( Arg1f ) & Arg2, scr );
        } else {
    	    SCP_PUSHARG_A( SCR_INT, Arg1 & Arg2, scr );
        }	
    }        
}

void ScpA_BinOr( Intp_t *scr ) // 8041 |( p1, p2 )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FLOAT ) return;    
    if( Type2 == SCR_FLOAT ){
        if( Type1 == SCR_FLOAT ){
    	    SCP_PUSHARG_A( SCR_INT, lround( Arg1f ) | lround( Arg2f ), scr );
        } else {
    	    SCP_PUSHARG_A( SCR_INT, Arg1 | lround( Arg2f ), scr );
        }        
    } else {
        if( Type2 != SCR_INT ) return;
        if( Type1 == SCR_FLOAT ){
    	    SCP_PUSHARG_A( SCR_INT, lround( Arg1f ) | Arg2, scr );
        } else {
    	    SCP_PUSHARG_A( SCR_INT, Arg1 | Arg2, scr );
        }        
    }        
}

void ScpA_BinXor( Intp_t *scr ) // 8042  = ^( a, b )
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    if( Type2 < SCR_FLOAT ) return;
    if( Type2 <= SCR_FLOAT ){
        if( Type1 == SCR_FLOAT ){
    	    SCP_PUSHARG_A( SCR_INT, lround( Arg2f ) ^ lround( Arg1f ), scr );
        } else {
    	    SCP_PUSHARG_A( SCR_INT, Arg1 ^ lround( Arg2f ), scr );
        }        
    } else {
        if( Type2 != SCR_INT ) return;
        if( Type1 == SCR_FLOAT ){
    	    SCP_PUSHARG_A( SCR_INT, lround( Arg1f ) ^ Arg2, scr );
        } else {
    	    SCP_PUSHARG_A( SCR_INT, Arg1 ^ Arg2, scr );
        }        
    }    
}

void ScpA_Swap( Intp_t *scr ) // 8019 <->( p1, p2 )
{
    int Arg1, Arg2;
    short Type1, Type2;

    Type1 = IntpPopwB( scr ); Arg1 = IntpPopIntStack( scr->StackB, &scr->StackIdxB );
    Type2 = IntpPopwB( scr ); Arg2 = IntpPopIntStack( scr->StackB, &scr->StackIdxB );
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, Arg1 ); IntpPushwB( scr, Type1 );
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, Arg2 ); IntpPushwB( scr, Type2 );
}

void ScpA_EndCritical( Intp_t *scr ) // 8003, 804B
{
    scr->Flags &= ~0x80;
}

void ScpA_StartCritical( Intp_t *scr ) // 8002, 804A O_CRITICAL_START
{
    scr->Flags |= 0x80;
}

void ScpA_Jump( Intp_t *scr ) // 8004 jmp()
{
    unsigned short Type;
    char stmp[256];
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    Type &= ~0x800;
    SCP_ERRTYPEF( Type != SCR_INT, stmp, "Invalid type given to jmp, %x", Type );
    scr->InstrPtr = Arg;
}

void ScpA_CallExt( Intp_t *scr ) // 8005 call( adres, arguments_cnt, )
{
    unsigned short Type;
    char b, *ExtName, stmp[256], *s;
    int Arg,i,ArgL, tmp, ArgC_got;
    short TypeL, ArgC_expected, v9;    
    Intp_t LocalScr;
    char *ArgV;

    SCP_GETARG( Type, Arg, scr );
    SCP_ERRTYPE( SCRTYPE( Type ) != SCR_INT, "Invalid address given to call" );
    // get procedure name
    b = IntpReadBei( &scr->ProcTable[ Arg ].NameOfst, 4 );
    if( !(b & 0x04) ){
        scr->InstrPtr = IntpReadBei( &scr->ProcTable[ Arg ].NameOfst, 16 );
        if( b & 0x10 ) scr->Flags |= 0x80;
        return;
    }
    ExtName = scr->StringBase + IntpReadBei( &scr->ProcTable[ Arg ].NameOfst, 0 );
    SCP_ERRTYPEF( !ExportGetProcedure( ExtName, &ArgV, &ArgC_expected ), "External procedure %s not found", ExtName );

    SCP_GETARG( v9, ArgC_got, scr );
    SCP_ERRTYPEF( SCRTYPE( v9 ) != SCR_INT || ArgC_got != ArgC_expected, stmp, "Wrong number of arguments to external procedure %s.Expecting %d, got %d.", ExtName, ArgC_expected, ArgC_got );
    // save context
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, scr->InstrPtr ); IntpPushwB( scr, SCR_INT );
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, scr->Flags );    IntpPushwB( scr, SCR_INT );
    IntpPushPtrStack( scr->StackB, &scr->StackIdxB, scr->Func );     IntpPushwB( scr, SCR_PTR );
    IntpPushPtrStack( scr->StackB, &scr->StackIdxB, scr );           IntpPushwB( scr, SCR_PTR );
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, 36 );            IntpPushwB( scr, SCR_INT );
    IntpPushIntStack( scr->StackA, &scr->StackApos, scr->Flags );    IntpPushwA( scr, SCR_INT );
    IntpPushPtrStack( scr->StackA, &scr->StackApos, scr->Func );     IntpPushwA( scr, SCR_PTR );
    IntpPushIntStack( scr->StackA, &scr->StackApos, scr->i34 );      IntpPushwA( scr, SCR_INT );

    scr->i34 = scr->i34;
    LocalScr.StackApos = LocalScr.StackIdxB = 0;
    while( --i != -1 ){ // copy arguments
        SCP_GETARG( TypeL, ArgL, scr );
        SCP_PUSHARG_A( TypeL, ArgL,  &LocalScr );
    }    
    for( i = 0; i < ArgC_expected; i++ ){
        SCP_GETARG( TypeL, ArgL, &LocalScr );
        if( SCRTYPE( TypeL ) == SCR_STRING ){
            if( TypeL & 0x800 ){
                s = &scr->Strings->Data[ ArgL ];
            } else if( TypeL & 0x1000 ){
                s = &scr->Floats->Data[ ArgL ];
            } else {
                s = NULL;
            }
            IntpPushIntStack( scr->StackA, &scr->StackApos, IntpDbgStr( scr, s, (tmp & 0x00ff) | (TypeL & 0x00ff) ) ); IntpPushwA( scr, SCR_FSTRING );
        } else {
            IntpPushIntStack( scr->StackA, &scr->StackApos, ArgL ); IntpPushwA( scr, TypeL );
        }
    }

    IntpPushIntStack( scr->StackA, &scr->StackApos, ArgC_expected ); IntpPushwA( scr, SCR_INT );
    scr->Flags |= 0x20;
    scr->Flags = 0;
    i = IntpReadBei( &scr->ProcTable[ 3 * Arg ].NameOfst, 4 );
    scr->InstrPtr = ArgV;
    if( (i & 0x10) || (scr->Flags & 0x80) ) scr->Flags |= 0x80;
}

void ScpA_801F( Intp_t *scr ) // 801F ?( arg1, arg2, arg3 )
{

    unsigned short Type1, Type2, Type3;
    int Arg1, Arg3;
    void *Arg2;

    SCP_GETARG( Type1, Arg1, scr ); scr->i34 = Arg1; // window

    Type2 = IntpPopShortStack( scr->StackA, &scr->StackApos);
    if( Type2 == 0x9801 ){
	Arg1 = IntpPopIntStack( scr->StackA, &scr->StackApos);
	IntpStringDeRef( scr, 0x9801, Arg1 );
	scr->Func = NULL; // ???
    } else {
	Arg2 = IntpPopPtrStack( scr->StackA, &scr->StackApos);
	scr->Func = Arg2;
    }    
    SCP_GETARG( Type3, Arg3, scr ); scr->Flags = Arg3;
}

void ScpA_RetA( Intp_t *scr ) // 801C
{
    IntpPopwB( scr );
    scr->InstrPtr = IntpPopIntStack( scr->StackB, &scr->StackIdxB );
}

void ScpA_RetB( Intp_t *scr ) // 801D
{
    IntpPopwB( scr );
    scr->InstrPtr = IntpPopIntStack( scr->StackB, &scr->StackIdxB );
    scr->Flags |= 0x40;
}

void ScpA_8020( Intp_t *scr ) // 8020 O_POP_FLAGS_RETURN
{
    ScpA_801F( scr );
    IntpPopwB( scr ); scr->InstrPtr = IntpPopiB( scr );
}

void ScpA_8021( Intp_t *scr ) // 8021 O_POP_FLAGS_EXIT
{
    ScpA_801F( scr );
    IntpPopwB( scr ); scr->InstrPtr = IntpPopiB( scr );
    scr->Flags |= 0x40;
}

void ScpA_8025( Intp_t *scr ) // 8025 O_POP_FLAGS_RETURN_VAL_EXIT
{
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    ScpA_801F( scr );
    IntpPopwB( scr ); scr->InstrPtr = IntpPopiB( scr );
    scr->Flags |= 0x40;
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg ); IntpPushwA( scr, Type );
}

void ScpA_8026( Intp_t *scr ) // 8026 O_POP_FLAGS_RETURN_VAL_EXIT_EXTERN
{
    int Arg;
    Intp_t *p;
    unsigned short Type;

    SCP_GETARG( Type, Arg, scr );
    ScpA_801F( scr );
    IntpPopwB( scr ); p = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Func = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Flags = IntpPopIntStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); scr->InstrPtr = IntpPopiB( scr );
    scr->Flags |= 0x40;
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg );
    IntpPushwA( scr, Type );
}

void ScpA_8022( Intp_t *scr ) // 8022 O_POP_FLAGS_RETURN_EXTERN
{
    Intp_t *p;

    ScpA_801F( scr );
    IntpPopwB( scr ); p = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Func = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Flags = IntpPopIntStack( scr->StackB, &scr->StackIdxB ); IntpPopwB( scr );
    scr->InstrPtr = IntpPopiB( scr );
}

void ScpA_8023( Intp_t *scr ) // 8023 O_POP_FLAGS_EXIT_EXTERN
{
    Intp_t *p;

    ScpA_801F( scr );
    IntpPopwB( scr ); p = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Func = IntpPopPtrStack(scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Flags = IntpPopIntStack(scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); scr->InstrPtr = IntpPopiB( scr );
    scr->Flags |= 0x40;
}

void ScpA_8024( Intp_t *scr ) // 8024 O_POP_FLAGS_RETURN_VAL_EXTERN
{
    Intp_t *p;
    char *s;
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    ScpA_801F( scr );
    IntpPopwB( scr ); p = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Func = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Flags = IntpPopIntStack( scr->StackB, &scr->StackIdxB );
    if( SCRTYPE( Type ) == SCR_STRING ){
        if( Type & 0x800 ){
            s = &scr->Strings->Data[ Arg ];
        } else if( Type & 0x1000 ){
            s = &scr->Floats->Data[ Arg ];
        } else {
            s = NULL;
        }
        IntpPushIntStack( p->StackA, &p->StackApos, IntpDbgStr( p, s, &scr->StackIdxB ) ); IntpPushwA( p, SCR_FSTRING );
    } else {
        IntpPushIntStack( p->StackA, &p->StackApos, Arg ); IntpPushwA( p, Type );
    }
    if( p->Flags & 0x80 ) scr->Flags &= ~0x80;
    IntpPopwB( scr ); scr->InstrPtr = IntpPopiB( scr );
    IntpPopwB( p ); p->InstrPtr = IntpPopiB( p );
}

void ScpA_801E( Intp_t *scr ) // 801E
{
    IntpPopwB( scr ); IntpPopIntStack( scr->StackB, &scr->StackIdxB );
}

void ScpA_800C( Intp_t *scr ) // 800C
{
    unsigned short Type;
    int Arg;

    SCP_GETARG_B( Type, Arg, scr );
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg );
    IntpPushwA( scr, Type );
}

void ScpA_800D( Intp_t *scr ) // 800D O_D_TO_A
{
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, Arg ); IntpPushwB( scr, Type );
}

void ScpA_8010( Intp_t *scr ) // 8010 O_EXIT_PROG
{
    scr->Flags |= 0x01;
}

void ScpA_8011( Intp_t *scr ) // 8011
{
    scr->Flags |= 0x08;
}

void ScpA_8012( Intp_t *scr ) // 8012
{
    unsigned short Type;
    int Arg, k;

    SCP_GETARG( Type, Arg, scr );
    k = *( &scr->StackA[ 3 * Arg + 2 ] + scr->SaveStack );
    k = ((k & 0x00ff) << 8) | ( (k & 0xff00) >> 8 );
    SCP_PUSHARG_A( k, IntpReadBei( scr->StackA, 3 * Arg * 2 + scr->SaveStack ), scr );
}

void ScpA_8013( Intp_t *scr ) // 8013
{
    int Arg1,Arg2;
    unsigned short Type1, Type2;
    
    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    if( scr->StackA[3 * Arg1 + 2 + scr->SaveStack] == 0x0198 ) IntpStringDeRef( scr, SCR_FSTRING, IntpReadBei(scr->StackA, 6 * Arg1 + scr->SaveStack) );
    IntpWriteBew( Arg2 >> 16, scr->StackA, 6 * Arg1 + scr->SaveStack );
    IntpWriteBew( Arg2, scr->StackA, 6 * Arg1 + scr->SaveStack + 2 );
    scr->StackA[ 3 * Arg1 + 2 + scr->SaveStack + 0 ] = Type2 >> 24;
    scr->StackA[ 3 * Arg1 + 2 + scr->SaveStack + 1 ] = Type2;
    if( Type2 == SCR_FSTRING ) ++scr->Strings->Data[ Arg2 - 2 ];
}

void ScpA_8018( Intp_t *scr ) // 8018
{
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 ); IntpPushwA( scr, Type1 );
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 ); IntpPushwA( scr, Type2 );
}

void ScpA_FetchAddr( Intp_t *scr ) // 802D
{
    unsigned short Type;
    int Arg;
    char stmp[ 260 ];

    SCP_GETARG( Type, Arg, scr );
    SCP_ERRTYPEF( Type != SCR_INT, stmp, "Invalid type given to fetch_proc_address, %x", Type );
    IntpPushIntStack( scr->StackA, &scr->StackApos, IntpReadBei( (char *)&scr->ProcTable[ Arg ].NameOfst, 16 ) ); IntpPushwA( scr, SCR_INT );
}

void ScpA_801A( Intp_t *scr ) // 801A O_POP
{
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
}

void ScpA_801B( Intp_t *scr ) // 801B
{
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg ); IntpPushwA( scr, Type );
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg ); IntpPushwA( scr, Type );
}

void ScpA_SetExportedVar( Intp_t *scr ) // 8015
{
    unsigned short Type1, Type2;
    char stmp[ 256 ];
    int Arg1, Arg2;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    if( ExportSetVarArg( scr, (char *)scr->StringBase + Arg1, Type2, Arg2 ) ){
        sprintf( stmp, "External variable %s does not exist\n", IntpGetString( scr, Arg1 ) );
        IntpError( stmp );
    }
}

void ScpA_GetExportedVar( Intp_t *scr ) // 8014
{
    short Type;
    char stmp[ 256 ];
    int Arg;
    int tmp;

    SCP_GETARG( Type, Arg, scr );
    if( ExportGetVarArg( scr, (char *)scr->StringBase + Arg, &Type, &tmp ) ){
        sprintf( stmp, "External variable %s does not exist\n", IntpGetString( scr, Arg ) );
        IntpError( stmp );
    }
    IntpPushPtrStack( scr->StackA, &scr->StackApos, tmp ); 
    IntpPushwA( scr, Type );
}

void ScpA_ExportProcedure( Intp_t *scr ) // 8017
{
    unsigned short Type1, Type2;
    char stmp[ 256 ], *tmp;
    int Arg1, Arg2;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    tmp = &scr->StringBase[ IntpReadBei( &scr->ProcTable[ 6 * Arg1 ].NameOfst, 0 ) ];
    SCP_ERRTYPEF( ExportSetProcedure( scr, tmp, IntpReadBei( &scr->ProcTable[ 6 * Arg1 ].NameOfst, 16 ), Arg2 ),stmp, "Error exporting procedure %s", tmp );
}

void ScpA_ExportVariable( Intp_t *scr ) // 8016
{
    unsigned short Type;
    char stmp[ 256 ];
    int Arg;
    
    SCP_GETARG( Type, Arg, scr );
    if( ExportCreateIVar( &scr->FileName, (char *)scr->StringBase + Arg ) ){
        sprintf( stmp, "External variable %s already exists", IntpGetString( scr, Arg ) );
        IntpError( stmp );
    }
}

void ScpA_KillProcedure( Intp_t *scr ) // 800E
{
    scr->Flags |= 0x01;
    if( scr->Parent && (scr->Parent->Flags & 0x100) != 0 ) scr->Parent->Flags &= ~0x0100;
    if( scr->i35 ) return;
    IntpTaskFire( scr );
    scr->i35 = 1;
}

void ScpA_Detach( Intp_t *scr ) // 800F
{
    if( !scr->Parent ) return;
    scr->Parent->Flags &= ~0x0020;
    scr->Parent->Flags &= ~0x0100;
    if( scr == scr->Parent->ChildProcess ) scr->Parent->ChildProcess = NULL;    
}

void ScpA_CallStart( Intp_t *scr ) // 8008 callstart()
{
    Intp_t *LocScr;
    unsigned short Type;
    char stmp[ 256 ], *v10;
    int Arg;

    SCP_ERRTYPE( scr->ChildProcess, "Error, already have a child process\n" );
    SCP_GETARG( Type, Arg, scr );
    SCP_ERRTYPE( SCRTYPE( Type ) != SCR_STRING , "Invalid type given to callstart" );
    scr->Flags |= 0x0020;
    if( Type & 0x800 ){
        v10 = &scr->Strings->Data[ Arg ];
    } else if( Type & 0x1000 ){
        v10 = &scr->Floats->Data[ Arg ];
    } else {
        v10 = 0;
    }    
    LocScr = IntpLoad( IntpMseHandler( v10 ) );
    if( LocScr ){
        SciItpEqAdd( LocScr );
        SciOpcodeExec( LocScr, 24 );
    }
    scr->ChildProcess = LocScr;
    SCP_ERRTYPEF( !LocScr, stmp, "Error spawning child %s", IntpGetArg( scr, Type >> 8, Arg ) );
    scr->ChildProcess->Parent = scr;
    scr->ChildProcess->i34 = scr->i34;
}

void ScpA_Spawn( Intp_t *scr ) // 800A
{
    Intp_t *LocScr;
    unsigned short Type;
    char stmp[ 256 ], *v10;
    int Arg;

    SCP_ERRTYPE( scr->ChildProcess, "Error, already have a child process\n" );
    SCP_GETARG( Type, Arg, scr );
    SCP_ERRTYPE( SCRTYPE( Type ) != SCR_STRING, "Invalid type given to spawn" );
    scr->Flags |= 0x0100;
    if( (Type & 0x800) != 0 ){
        v10 = &scr->Strings->Data[Arg];
    } else if ( (Type & 0x1000) != 0 ){
        v10 = &scr->Floats->Data[Arg];
    } else {
        v10 = 0;
    }
    LocScr = IntpLoad( IntpMseHandler( v10 ) );
    if( LocScr ){
        SciItpEqAdd( LocScr );
        SciOpcodeExec( LocScr, 24 );
    }
    scr->ChildProcess = LocScr;
    SCP_ERRTYPEF( !LocScr, stmp, "Error spawning child %s", IntpGetArg( scr, Type >> 8, Arg ) );
    scr->ChildProcess->Parent = scr;
    scr->ChildProcess->i34 = scr->i34;
    if( scr->Flags & 0x80 ){
        scr->ChildProcess->Flags |= 0x80;
        SciOpcodeExec( scr->ChildProcess, -1 );
    }
}

void ScpA_Fork( Intp_t *scr ) // 800B fork()
{
    Intp_t *LocScr;
    short Type;
    char stmp[ 256 ], *v6;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    if( (Type & 0x800) != 0 ){
        v6 = &scr->Strings->Data[Arg];
    } else if ( (Type & 0x1000) != 0 ){
        v6 = &scr->Floats->Data[Arg];
    } else {
        v6 = 0;
    }    
    LocScr = IntpLoad( IntpMseHandler( v6 ) );
    if( LocScr ){
        SciItpEqAdd( LocScr );
        SciOpcodeExec( LocScr, 24 );
    }
    SCP_ERRTYPEF( !LocScr, stmp, "couldn't fork script '%s'", IntpGetArg( scr, Type >> 8, Arg ) );
    LocScr->i34 = scr->i34;
}

void ScpA_8009( Intp_t *scr ) // 8009
{
    Intp_t *Parent;
    
    Parent = scr->Parent;
    ScpA_Fork( scr );
    if( Parent ){
        scr->Parent = Parent;
        Parent->ChildProcess = scr;
    }
    scr->ChildProcess = NULL;
    scr->Parent = NULL;
    scr->Flags |= 0x01;
    if( scr->Parent && scr->Parent->Flags & 0x0100 ) scr->Parent->Flags &= ~0x100;    
    IntpTaskRun( scr );
}

void ScpA_8027( Intp_t *scr ) // 8027 ?( arg1, arg2 )
{
    unsigned short Type1, Type2;
    char stmp[ 256 ];
    int Arg1, Arg2;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    if( IntpReadBei( &scr->ProcTable->NameOfst + 6 * Arg2, 20 ) != Arg1 ){
        sprintf( stmp, "Wrong number of args to procedure %s\n", IntpGetString( scr, IntpReadBei( &scr->ProcTable->NameOfst + 6 * Arg2, 0 ) ) );
        IntpError( stmp );
    }
}

void ScpA_LookupStringProc( Intp_t *scr ) // 8028
{
    unsigned short Type;
    char stmp[ 256 ], *s, *a;
    int Arg, i, cnt;
    void *p;

    SCP_GETARG( Type, Arg, scr );
    SCP_ERRTYPE( SCRTYPE( Type ) != SCR_STRING, "Wrong type given to lookup_string_proc\n" );

    if( Type & 0x800 ){
        s = &scr->Strings->Data[ Arg ];
    } else if( Type & 0x1000 ){
        s = &scr->Floats->Data[ Arg ];
    } else {
        s = NULL;
    }

    cnt = IntpReadBei( scr->ProcTable, 0 ) - 1;
    p = &scr->ProcTable[ 1 ];
    for( i = 0; i < cnt; i++, p += 6 ){
	a = scr ? &scr->StringBase[ IntpReadBei( p, 0 ) ] : NULL;
        if( !strcasecmp( a, s ) ){
	    IntpPushIntStack( scr->StackA, &scr->StackApos, i ); IntpPushwA( scr, SCR_INT );
            return;
        }        
    }
    sprintf( stmp, "Couldn't find string procedure %s\n", s );
    IntpError( stmp );
}

void ScpAInitCommands()
{
    gScpUnk01 = 1;
    SciAddOpcode( 0x8005, ScpA_CallExt );
    SciAddOpcode( 0x800E, ScpA_KillProcedure );
    SciAddOpcode( 0x800F, ScpA_Detach );
    SciAddOpcode( 0x800A, ScpA_Spawn );
    SciAddOpcode( 0x8008, ScpA_CallStart );
    SciAddOpcode( 0x8009, ScpA_8009 );
    SciAddOpcode( 0x800B, ScpA_Fork );
    SciAddOpcode( 0x8000, ScpA_Nop );
    SciAddOpcode( 0x8001, ScpA_8001 );
    SciAddOpcode( 0x8002, ScpA_StartCritical );
    SciAddOpcode( 0x8003, ScpA_EndCritical );
    SciAddOpcode( 0x8004, ScpA_Jump );
    SciAddOpcode( 0x8006, ScpA_Call );
    SciAddOpcode( 0x8007, ScpA_CondCall );
    SciAddOpcode( 0x800C, ScpA_800C );
    SciAddOpcode( 0x800D, ScpA_800D );
    SciAddOpcode( 0x8010, ScpA_8010 );
    SciAddOpcode( 0x8011, ScpA_8011 );
    SciAddOpcode( 0x8012, ScpA_8012 );
    SciAddOpcode( 0x8013, ScpA_8013 );
    SciAddOpcode( 0x8014, ScpA_GetExportedVar );
    SciAddOpcode( 0x8015, ScpA_SetExportedVar );
    SciAddOpcode( 0x8016, ScpA_ExportVariable );
    SciAddOpcode( 0x8017, ScpA_ExportProcedure );
    SciAddOpcode( 0x8018, ScpA_8018 );
    SciAddOpcode( 0x8019, ScpA_Swap );
    SciAddOpcode( 0x801A, ScpA_801A );
    SciAddOpcode( 0x801B, ScpA_801B );
    SciAddOpcode( 0x801C, ScpA_RetA );
    SciAddOpcode( 0x801D, ScpA_RetB );
    SciAddOpcode( 0x801E, ScpA_801E );
    SciAddOpcode( 0x801F, ScpA_801F );
    SciAddOpcode( 0x8021, ScpA_8021 );
    SciAddOpcode( 0x8020, ScpA_8020 );
    SciAddOpcode( 0x8022, ScpA_8022 );
    SciAddOpcode( 0x8024, ScpA_8024 );
    SciAddOpcode( 0x8023, ScpA_8023 );
    SciAddOpcode( 0x8025, ScpA_8025 );
    SciAddOpcode( 0x8026, ScpA_8026 );
    SciAddOpcode( 0x8029, ScpA_PopBase );
    SciAddOpcode( 0x802A, ScpA_Ret );
    SciAddOpcode( 0x802B, ScpA_802B );
    SciAddOpcode( 0x802C, ScpA_802C );
    SciAddOpcode( 0x802D, ScpA_FetchAddr );
    SciAddOpcode( 0x802E, ScpA_Dump );
    SciAddOpcode( 0x802F, ScpA_If );
    SciAddOpcode( 0x8030, ScpA_WhileDo );
    SciAddOpcode( 0x8031, ScpA_8031 );
    SciAddOpcode( 0x8032, ScpA_Fetch );
    SciAddOpcode( 0x8033, ScpA_CmpEQ );
    SciAddOpcode( 0x8034, ScpA_CmpNE );
    SciAddOpcode( 0x8035, ScpA_CmpLE );
    SciAddOpcode( 0x8036, ScpA_CmpGE );
    SciAddOpcode( 0x8037, ScpA_CmpLS );
    SciAddOpcode( 0x8038, ScpA_CmpGT );
    SciAddOpcode( 0x8039, ScpA_Add );
    SciAddOpcode( 0x803A, ScpA_Sub );
    SciAddOpcode( 0x803B, ScpA_Mul );
    SciAddOpcode( 0x803C, ScpA_Div );
    SciAddOpcode( 0x803D, ScpA_Mod );
    SciAddOpcode( 0x803E, ScpA_LogAnd );
    SciAddOpcode( 0x803F, ScpA_LogOr );
    SciAddOpcode( 0x8040, ScpA_BinAnd );
    SciAddOpcode( 0x8041, ScpA_BinOr );
    SciAddOpcode( 0x8042, ScpA_BinXor );
    SciAddOpcode( 0x8043, ScpA_BinNot );
    SciAddOpcode( 0x8044, ScpA_Floor );
    SciAddOpcode( 0x8045, ScpA_LogNot );
    SciAddOpcode( 0x8046, ScpA_Neg );
    SciAddOpcode( 0x8047, ScpA_Wait );
    SciAddOpcode( 0x8048, ScpA_Cancel );
    SciAddOpcode( 0x8049, ScpA_CancelAll );
    SciAddOpcode( 0x804A, ScpA_StartCritical );
    SciAddOpcode( 0x804B, ScpA_EndCritical );
    SciAddOpcode( 0x8027, ScpA_8027 );
    SciAddOpcode( 0x8028, ScpA_LookupStringProc );
    ScpIfcInit( );
    ExportFlushProc( );
}