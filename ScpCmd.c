#include "FrameWork.h"

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
    SCP_DBG( scr, "NOP" );
    /* Empty Function */
}

/*
    name: ?
    code 0x8001, 0xC001, 0x9001, 0x9801, 
    ret: ?
    dsc: ?
*/
void ScpA_PushConst( Intp_t *scr ) // Fetch INT from Code and push it on to stack A
{
    SCP_DBG_VAR;
    uint32_t addr;

    addr = IntpReadBei( scr->Code, scr->CodePC );
    IntpPushIntStack( scr->StackA, &scr->StackApos, addr );
    IntpPushwA( scr, scr->Opcode ); // copy constant type
    scr->CodePC += 4;
    SCP_DBGA( "  PUSHD( [%x]%x )", scr->Opcode, addr );
}

void ScpA_802B( Intp_t *scr ) // 802B push_base( ArgsCnt ) ?
{
    SCP_DBG_VAR;
    uint16_t type;
    int ArgsCnt;

    SCP_GETARG( type, ArgsCnt, scr );
    SCP_DBGA( "  PUSHA_BASE( [%x]%x base:%d )", type, ArgsCnt, scr->Base );
    SCP_PUSHARG_B( SCR_INT, scr->Base, scr );
    scr->Base = scr->StackApos - 6 * ArgsCnt;
}

void ScpA_PopBase( Intp_t *scr ) // 8029 'pop_base( base )'
{
    SCP_DBG_VAR;
    unsigned short type;
    int base;
    char stmp[ 256 ];

    SCP_GETARG_B( type, base, scr );
    SCP_DBGA( "  POPA_BASE( [%x]%d )", type, base );
    SCP_ERRTYPEF( type != SCR_INT, stmp, "Invalid type given to pop_base: %x", type );
    scr->Base = base;
}

void ScpA_Ret( Intp_t *scr ) // 802A 'Ret( ... )
{
    SCP_DBG_VAR;
    short Type;
    int val;

    SCP_DBGA( "  LEAVE_FRM" );
    while( scr->StackApos != scr->Base ){
        SCP_GETARG( Type, val, scr );
    }    
}

void ScpA_802C( Intp_t *scr ) // 802C save_stack() ?
{
    SCP_DBG_VAR;

    SCP_DBGA( "  ENTER_FRM" );
    scr->SaveStack = scr->StackApos;
}

void ScpA_Dump( Intp_t *scr ) // 802E dump( arg_cnt, ... )
{
    SCP_DBG_VAR;
    short type;
    int arg_cnt, argn;
    char stmp[ 256 ];

    SCP_GETARG_A( type, arg_cnt, scr );
    SCP_DBGA( "DUMP( [%x]%x )", type, arg_cnt );
    SCP_ERRTYPEF( type != SCR_INT, stmp, "Invalid type given to dump, %x", type );
    while( --arg_cnt != -1 ){
	SCP_GETARG( type, argn, scr );
    }
}

void ScpA_Call( Intp_t *scr ) // 8006 'call( procedure, procedure_time )
{
    SCP_DBG_VAR;
    unsigned short type1, type2;
    int addr, time, NameIdx;

    SCP_GETARG( type1, addr, scr );
    SCP_ERRTYPE( SCRTYPE( type1 ) != SCR_INT, "Invalid procedure type given to call" );
    SCP_GETARG( type2, time, scr );
    SCP_DBGA( "CALL(( [%x]%x, [%x]%x )", type2, addr, type1, time );
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
    SCP_DBG_VAR;
    int ProcIdx, addr, NameIdx;
    unsigned short type1, type2;

    SCP_GETARG( type1, ProcIdx, scr );
    SCP_ERRTYPE( SCRTYPE( type1 ) != SCR_INT, "Invalid procedure type given to conditional call" );
    SCP_GETARG( type2, addr, scr );
    SCP_DBGA( "COND_CALL( [%x]%x, [%x]%x )", type2, addr, type1, ProcIdx );
    SCP_ERRTYPE( SCRTYPE( type2 ) != SCR_INT, "Invalid address given to conditional call" );
    NameIdx = IntpReadBei( (char *)&scr->ProcTable[ ProcIdx ].NameOfst, 4 ) | 0x02;
    IntpWriteBew( NameIdx >> 16,(char *)&scr->ProcTable[ ProcIdx ].NameOfst, 4 );
    IntpWriteBew( NameIdx,      (char *)&scr->ProcTable[ ProcIdx ].NameOfst, 6 );
    IntpWriteBew( addr >> 16, 	(char *)&scr->ProcTable[ ProcIdx ].NameOfst, 12 );
    IntpWriteBew( addr,       	(char *)&scr->ProcTable[ ProcIdx ].NameOfst, 14 );
}

void ScpA_Wait( Intp_t *scr ) // 8047 'wait( time )'
{
    SCP_DBG_VAR;
    short type;
    int time;

    SCP_GETARG( type, time, scr );
    SCP_DBGA( "WAIT( [%x]%x )", type, time );
    SCP_ERRTYPE( SCRTYPE( type ) != SCR_INT, "Invalid type given to wait\n" );
    scr->TimeA = 1000 * gIntpGetTime() / gIntpTimeDiv;
    scr->Time = scr->TimeA + time;
    scr->Func = IntpTimer;
    scr->Flags |= 0x10;
}

void ScpA_Cancel( Intp_t *scr ) // 8048 'cancel( proc )'
{
    SCP_DBG_VAR;
    short type;
    int ProcOffs;

    SCP_GETARG( type, ProcOffs, scr );
    SCP_DBGA( "CANCEL( [%x]%x )", type, ProcOffs );
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
    SCP_DBG_VAR;
    int i, Arg1;
    
    SCP_DBGA( "CANCEL_ALL()" );
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

void ScpA_If( Intp_t *scr ) // 802F ifcon( skip_addr, cond ) -> 'if( Cond == 0 ) then begin'
{
    SCP_DBG_VAR;
    uint16_t type[ 2 ];
    int Cond, addr;
    void *ptr;

    SCP_GETARGX( type[ 0 ], Cond, ptr, scr );
    SCP_GETARG( type[ 1 ], addr, scr );
    SCP_DBGA( "IF( [%x]%x == 0 ) GOTO [%x]%x", type[ 0 ], Cond, type[ 1 ], addr );
    if( type[ 0 ] == SCR_PTR ){
	if( !ptr ) scr->CodePC = addr;    
    } else {
	if( !Cond ) scr->CodePC = addr;    
    }
}

void ScpA_WhileDo( Intp_t *scr ) // 8030 while( Cond, SkipAddress ) -> 'while( Cond ) do begin'
{
    SCP_DBG_VAR;
    short type1, type2;
    int Cond, BreakAddr;

    SCP_GETARG( type1, Cond, scr );
    if( !Cond ){
	SCP_GETARG( type2, BreakAddr, scr );
        scr->CodePC = BreakAddr;
	SCP_DBGA( "WHILE( [%x]%x, [%x]%x )", type1, Cond, type2, BreakAddr );
    } else {
	SCP_DBGA( "WHILE( [%x]%x )", type1, Cond );
    }

}

void ScpA_8031( Intp_t *scr ) // 8031 ?( arg1, arg2 )
{
    SCP_DBG_VAR;
    short Type1, Type2;
    int Arg1, Arg2, k;
    char *stack;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    SCP_DBGA( "8031( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    k = IntpReadBei( scr->StackA, 6 * Arg1 + scr->Base );
    if( *(&scr->StackA[6 * Arg1 + 4] + scr->Base) == 0x0198 ) IntpStringDeRef( scr, SCR_FSTRING, k );
    IntpWriteBew( Arg2 >> 16, scr->StackA, 6 * Arg1 + scr->Base );
    IntpWriteBew( Arg2, scr->StackA, 6 * Arg1 + scr->Base + 2 );
    stack = &scr->StackA[ 6 * Arg1 + 4 ] + scr->Base;
    stack[ 0 ] = Type2 >> 16;
    stack[ 1 ] = Type2;
    if( Type2 == SCR_FSTRING ) INTP_STR_REF( scr->Strings + Arg2 )++;
}

void ScpA_Fetch( Intp_t *scr ) // 8032
{
    SCP_DBG_VAR;
    unsigned short Type1;
    int Arg1, k;
    char stmp[264];
    uint8_t *p;
    
    SCP_GETARG( Type1, Arg1, scr );
    SCP_DBGA( "FETCH( [%x]%x )", Type1, Arg1 );
    SCP_ERRTYPEF( Type1 != SCR_INT, stmp, "Invalid type given to fetch, %x", Type1 );
    k = IntpReadBei( scr->StackA, scr->Base + 6 * Arg1 );
    
    p = (uint8_t *)&scr->StackA[ 6 * Arg1 + 4 ];
    SCP_PUSHARG_A( (((uint16_t)p[ scr->Base + 0 ] << 8) | (uint16_t)p[ scr->Base + 1 ]), k, scr );
}

void ScpA_CmpNE( Intp_t *scr ) // 8034 'stack:=p2<>p1'
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    float Arg1f, Arg2f;
    int Arg2,Arg1;
    char *s, stmp[ 80 ];
    void *Arg1p, *Arg2p;

    SCP_GETARGFX( Type1, Arg1, Arg1f, Arg1p, scr );
    SCP_GETARGFX( Type2, Arg2, Arg2f, Arg2p, scr );
    SCP_DBGA( "( [%x]%x != [%x]%x )", Type2, Arg2, Type1, Arg1 );

    switch( Type2 ){
	case SCR_STRING:
	case SCR_FSTRING:
	    switch( Type1 ){
        	case SCR_STRING:
    		case SCR_FSTRING:
    		    s = ( (Type2 & 0x800) != 0 ) ? &scr->Strings[ Arg2 + 4 ] : &scr->StringsConst[ Arg2 + 4 ];
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( s, IntpGetString( scr, Type1 >> 8, Arg1 ) ) );
    		    break;
    		case SCR_FLOAT:
        	    sprintf( stmp, "%.05f", Arg1f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type2 >> 8, Arg2 ) ) );
    		    break;
    		case SCR_INT:
        	    if( Arg1 == 0 ){ IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 ); break; }
        	    sprintf( stmp, "%d", Arg1 );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type2 >> 8 , Arg2 ) ) );
    		    break;
    		case SCR_PTR:
        	    if( !Arg1p ){ IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 ); break; }
        	    sprintf( stmp, "%p", Arg1p );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type2 >> 8 , Arg2 ) ) );
    		    break;
    	    }
    	    break;
	case SCR_FLOAT:
	    switch( Type1 ){
    		case SCR_STRING:
    		case SCR_FSTRING:
    		    sprintf( stmp, "%.05f", Arg2f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) );
    		    break;
    		case SCR_FLOAT:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f != Arg1f );
    		    break;
    		case SCR_INT:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, (float)Arg1 != Arg2f );
    		    break;
    		case SCR_PTR:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f != 0.0 || Arg1p );
    		    break;
    	    }
    	    break;
	case SCR_INT:
	    switch( Type1 ){
		case SCR_STRING:
		case SCR_FSTRING:
    		    if( Arg2 == 0 ){ IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 != 0 ); break; }
    		    sprintf( stmp, "%d", Arg2 );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) );
    		    break;
		case SCR_FLOAT:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, (double)Arg2 != Arg1f );
    		    break;
    		case SCR_INT: 
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 != Arg1 );
    		    break;
    		case SCR_PTR:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 || Arg1p );
    		    break;
	    }
	    break;
	case SCR_PTR:
	    switch( Type1 ){
		case SCR_STRING:
		case SCR_FSTRING:
    		    if( !Arg2p ){ IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 != 0 ); break; }
    		    sprintf( stmp, "%p", Arg2p );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) );
    		    break;
		case SCR_FLOAT:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2p || Arg1f != 0.0 );
    		    break;
    		case SCR_INT: 
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2p || Arg1 );
    		    break;
    		case SCR_PTR:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2p != Arg1p );
    		    break;
	    }
	    break;
    }
    IntpPushwA( scr, SCR_INT );
}

void ScpA_CmpEQ( Intp_t *scr ) // 8033 'stack:= p2==p1'
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    float Arg1f, Arg2f;
    int Arg2,Arg1;
    char *s, stmp[ 80 ];
    void *Arg1p, *Arg2p;

    SCP_GETARGFX( Type1, Arg1, Arg1f, Arg1p, scr );
    SCP_GETARGFX( Type2, Arg2, Arg2f, Arg2p, scr );
    SCP_DBGA( "( [%x]%x == [%x]%x )", Type2, Arg2, Type1, Arg1 );

    switch( Type2 ){
	case SCR_STRING:
	case SCR_FSTRING:
	    switch( Type1 ){
        	case SCR_STRING:
    		case SCR_FSTRING:
    		    s = ( (Type2 & 0x800) != 0 ) ? &scr->Strings[ Arg2 + 4 ] : &scr->StringsConst[ Arg2 + 4 ];
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, !strcmp( s, IntpGetString( scr, Type1 >> 8, Arg1 ) ) );
    		    break;
    		case SCR_FLOAT:
        	    sprintf( stmp, "%.05f", Arg1f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, !strcmp( stmp, IntpGetString( scr, Type2 >> 8, Arg2 ) ) );
    		    break;
    		case SCR_INT:
        	    if( Arg1 == 0 ){ IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 == 0 ); break; }
        	    sprintf( stmp, "%d", Arg1 );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, !strcmp( stmp, IntpGetString( scr, Type2 >> 8 , Arg2 ) ) );
    		    break;
    		case SCR_PTR:
        	    if( !Arg1p ){ IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 == 0 ); break; }
        	    sprintf( stmp, "%p", Arg1p );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, !strcmp( stmp, IntpGetString( scr, Type2 >> 8 , Arg2 ) ) );
    		    break;
    	    }
    	    break;
	case SCR_FLOAT:
	    switch( Type1 ){
    		case SCR_STRING:
    		case SCR_FSTRING:
    		    sprintf( stmp, "%.05f", Arg2f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, !strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) );
    		    break;
    		case SCR_FLOAT:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f == Arg1f );
    		    break;
    		case SCR_INT:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, (float)Arg1 == Arg2f );
    		    break;
    		case SCR_PTR:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f == 0.0 && !Arg1p );
    		    break;
    	    }
    	    break;
	case SCR_INT:
	    switch( Type1 ){
		case SCR_STRING:
		case SCR_FSTRING:
    		    if( Arg2 == 0 ){ IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 == 0 ); break; }
    		    sprintf( stmp, "%d", Arg2 );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, !strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) );
    		    break;
		case SCR_FLOAT:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, (double)Arg2 == Arg1f );
    		    break;
    		case SCR_INT: 
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 == Arg1 );
    		    break;
    		case SCR_PTR:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 == 0 && !Arg1p );
    		    break;
	    }
	    break;
	case SCR_PTR:
	    switch( Type1 ){
		case SCR_STRING:
		case SCR_FSTRING:
    		    if( !Arg2p ){ IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 == 0 ); break; }
    		    sprintf( stmp, "%p", Arg2p );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, !strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) );
    		    break;
		case SCR_FLOAT:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, !Arg2p && Arg1f == 0.0 );
    		    break;
    		case SCR_INT: 
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, !Arg2p && !Arg1 );
    		    break;
    		case SCR_PTR:
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2p == Arg1p );
    		    break;
	    }
	    break;
    }
    IntpPushwA( scr, SCR_INT );
}

void ScpA_CmpLE( Intp_t *scr ) // 8035 'stack:= p2<=p1'
{
    SCP_DBG_VAR;
    uint16_t Type1, Type2;
    int Arg1, Arg2;
    double Arg2f, Arg1f;
    char stmp[ 80 ], *s;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x <= [%x]%x )", Type2, Arg2, Type1, Arg1 );

    switch( Type2 ){
	case SCR_FSTRING:
	case SCR_STRING:
	    switch( Type1 ){
    		case SCR_STRING:
    		case SCR_FSTRING: // (string)Arg2 >= (string)Arg1
    		    s = ( Type2 & 0x800 ) ? scr->Strings : scr->StringsConst;
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( &s[ Arg2 + 4 ], IntpGetString( scr, Type1 >> 8, Arg1 ) ) <= 0 );
    		    break;
    		case SCR_FLOAT: // (string)Arg2 >= (float)Arg1
        	    sprintf( stmp, "%.05f", Arg1f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( IntpGetString( scr, Type2 >> 8, Arg2 ), stmp ) <= 0 );
    		    break;
    		case SCR_INT: // (string)Arg2 >= (int)Arg1
        	    if( Arg1 == 0 ){
        		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 <= 0 ); // ??
    		    } else {
            		sprintf( stmp, "%d", Arg1 );
        		IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( IntpGetString( scr, Type2 >> 8, Arg2 ), stmp ) <= 0 );
    		    }
    		    break;    	
    	    }
    	    break;    
	case SCR_FLOAT:
	    switch( Type1 ){
    		case SCR_FSTRING:
    		case SCR_STRING: // (float)Arg2 >= (string)Arg1
    		    sprintf( stmp, "%.05f", Arg2f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) <= 0 );
    		    break;
    		case SCR_FLOAT: // (float)Arg2 >= (float)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f <= Arg1f );
    		    break;
    		case SCR_INT: // (float)Arg2 >= (int)Arg1
    	    	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f <= (double)Arg1 );
    		    break;
    	    }
	    break;
	case SCR_INT:
	    switch( Type1 ){
		case SCR_FSTRING: 
		case SCR_STRING: // (int)Arg2 >= (string)Arg1
    		    if( Arg2 == 0 ){
        		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 <= 0 ); // ?
    		    } else {
    			sprintf( stmp, "%d", Arg2 );
    			IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) <= 0 );
    		    }
    		    break;
    		case SCR_FLOAT: // (int)Arg2 >= (float)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, (double)Arg2 <= Arg1f );
    		    break;
    		case SCR_INT: // (int)Arg2 >= (int)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 <= Arg1 );
    		    break;
	    }
	    break;
    }
    IntpPushwA( scr, SCR_INT );
}

void ScpA_CmpGE( Intp_t *scr ) // 8036 'stack:=p2>=p1'
{
    SCP_DBG_VAR;
    uint16_t Type1, Type2;
    int Arg1, Arg2;
    double Arg2f, Arg1f;
    char stmp[ 80 ], *s;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x >= [%x]%x )", Type2, Arg2, Type1, Arg1 );

    switch( Type2 ){
	case SCR_FSTRING:
	case SCR_STRING:
	    switch( Type1 ){
    		case SCR_STRING:
    		case SCR_FSTRING: // (string)Arg2 >= (string)Arg1
    		    s = ( Type2 & 0x800 ) ? scr->Strings : scr->StringsConst;
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( &s[ Arg2 + 4 ], IntpGetString( scr, Type1 >> 8, Arg1 ) ) >= 0 );
    		    break;
    		case SCR_FLOAT: // (string)Arg2 >= (float)Arg1
        	    sprintf( stmp, "%.05f", Arg1f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( IntpGetString( scr, Type2 >> 8, Arg2 ), stmp ) >= 0 );
    		    break;
    		case SCR_INT: // (string)Arg2 >= (int)Arg1
        	    if( Arg1 == 0 ){
        		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 >= 0 ); // ??
    		    } else {
            		sprintf( stmp, "%d", Arg1 );
        		IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( IntpGetString( scr, Type2 >> 8, Arg2 ), stmp ) >= 0 );
    		    }
    		    break;    	
    	    }
    	    break;    
	case SCR_FLOAT:
	    switch( Type1 ){
    		case SCR_FSTRING:
    		case SCR_STRING: // (float)Arg2 >= (string)Arg1
    		    sprintf( stmp, "%.05f", Arg2f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) >= 0 );
    		    break;
    		case SCR_FLOAT: // (float)Arg2 >= (float)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f >= Arg1f );
    		    break;
    		case SCR_INT: // (float)Arg2 >= (int)Arg1
    	    	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f >= (double)Arg1 );
    		    break;
    	    }
	    break;
	case SCR_INT:
	    switch( Type1 ){
		case SCR_FSTRING: 
		case SCR_STRING: // (int)Arg2 >= (string)Arg1
    		    if( Arg2 == 0 ){
        		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 >= 0 ); // ?
    		    } else {
    			sprintf( stmp, "%d", Arg2 );
    			IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) >= 0 );
    		    }
    		    break;
    		case SCR_FLOAT: // (int)Arg2 >= (float)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, (double)Arg2 >= Arg1f );
    		    break;
    		case SCR_INT: // (int)Arg2 >= (int)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 >= Arg1 );
    		    break;
	    }
	    break;
    }
    IntpPushwA( scr, SCR_INT );
}

void ScpA_CmpLS( Intp_t *scr ) // 8037 'stack:=p2<p1'
{
    SCP_DBG_VAR;
    uint16_t Type1, Type2;
    int Arg1, Arg2;
    double Arg2f, Arg1f;
    char stmp[ 80 ], *s;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x < [%x]%x )", Type2, Arg2, Type1, Arg1 );

    switch( Type2 ){
	case SCR_FSTRING:
	case SCR_STRING:
	    switch( Type1 ){
    		case SCR_STRING:
    		case SCR_FSTRING: // (string)Arg2 >= (string)Arg1
    		    s = ( Type2 & 0x800 ) ? scr->Strings : scr->StringsConst;
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( &s[ Arg2 + 4 ], IntpGetString( scr, Type1 >> 8, Arg1 ) ) < 0 );
    		    break;
    		case SCR_FLOAT: // (string)Arg2 >= (float)Arg1
        	    sprintf( stmp, "%.05f", Arg1f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( IntpGetString( scr, Type2 >> 8, Arg2 ), stmp ) < 0 );
    		    break;
    		case SCR_INT: // (string)Arg2 >= (int)Arg1
        	    if( Arg1 == 0 ){
        		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 < 0 ); // ??
    		    } else {
            		sprintf( stmp, "%d", Arg1 );
        		IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( IntpGetString( scr, Type2 >> 8, Arg2 ), stmp ) < 0 );
    		    }
    		    break;    	
    	    }
    	    break;    
	case SCR_FLOAT:
	    switch( Type1 ){
    		case SCR_FSTRING:
    		case SCR_STRING: // (float)Arg2 >= (string)Arg1
    		    sprintf( stmp, "%.05f", Arg2f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) < 0 );
    		    break;
    		case SCR_FLOAT: // (float)Arg2 >= (float)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f < Arg1f );
    		    break;
    		case SCR_INT: // (float)Arg2 >= (int)Arg1
    	    	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f < (double)Arg1 );
    		    break;
    	    }
	    break;
	case SCR_INT:
	    switch( Type1 ){
		case SCR_FSTRING: 
		case SCR_STRING: // (int)Arg2 >= (string)Arg1
    		    if( Arg2 == 0 ){
        		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 < 0 ); // ?
    		    } else {
    			sprintf( stmp, "%d", Arg2 );
    			IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) < 0 );
    		    }
    		    break;
    		case SCR_FLOAT: // (int)Arg2 >= (float)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, (double)Arg2 < Arg1f );
    		    break;
    		case SCR_INT: // (int)Arg2 >= (int)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 < Arg1 );
    		    break;
	    }
	    break;
    }
    IntpPushwA( scr, SCR_INT );
}
        
void ScpA_CmpGT( Intp_t *scr ) // 8038 'stack:=p2>p1''
{
    SCP_DBG_VAR;
    uint16_t Type1, Type2;
    int Arg1, Arg2;
    double Arg2f, Arg1f;
    char stmp[ 80 ], *s;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x > [%x]%x )", Type2, Arg2, Type1, Arg1 );

    switch( Type2 ){
	case SCR_FSTRING:
	case SCR_STRING:
	    switch( Type1 ){
    		case SCR_STRING:
    		case SCR_FSTRING: // (string)Arg2 >= (string)Arg1
    		    s = ( Type2 & 0x800 ) ? scr->Strings : scr->StringsConst;
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( &s[ Arg2 + 4 ], IntpGetString( scr, Type1 >> 8, Arg1 ) ) > 0 );
    		    break;
    		case SCR_FLOAT: // (string)Arg2 >= (float)Arg1
        	    sprintf( stmp, "%.05f", Arg1f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( IntpGetString( scr, Type2 >> 8, Arg2 ), stmp ) > 0 );
    		    break;
    		case SCR_INT: // (string)Arg2 >= (int)Arg1
        	    if( Arg1 == 0 ){
        		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 > 0 ); // ??
    		    } else {
            		sprintf( stmp, "%d", Arg1 );
        		IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( IntpGetString( scr, Type2 >> 8, Arg2 ), stmp ) > 0 );
    		    }
    		    break;    	
    	    }
    	    break;    
	case SCR_FLOAT:
	    switch( Type1 ){
    		case SCR_FSTRING:
    		case SCR_STRING: // (float)Arg2 >= (string)Arg1
    		    sprintf( stmp, "%.05f", Arg2f );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) > 0 );
    		    break;
    		case SCR_FLOAT: // (float)Arg2 >= (float)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f > Arg1f );
    		    break;
    		case SCR_INT: // (float)Arg2 >= (int)Arg1
    	    	    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f > (double)Arg1 );
    		    break;
    	    }
	    break;
	case SCR_INT:
	    switch( Type1 ){
		case SCR_FSTRING: 
		case SCR_STRING: // (int)Arg2 >= (string)Arg1
    		    if( Arg2 == 0 ){
        		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 > 0 ); // ?
    		    } else {
    			sprintf( stmp, "%d", Arg2 );
    			IntpPushIntStack( scr->StackA, &scr->StackApos, strcmp( stmp, IntpGetString( scr, Type1 >> 8, Arg1 ) ) > 0 );
    		    }
    		    break;
    		case SCR_FLOAT: // (int)Arg2 >= (float)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, (double)Arg2 > Arg1f );
    		    break;
    		case SCR_INT: // (int)Arg2 >= (int)Arg1
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 > Arg1 );
    		    break;
	    }
	    break;
    }
    IntpPushwA( scr, SCR_INT );
}

void ScpA_Add( Intp_t *scr ) // 8039
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;
    unsigned int t2;
    char *s, *pd, *p, *q;
    void *Arg1p, *Arg2p;

    SCP_GETARGFX( Type1, Arg1, Arg1f, Arg1p, scr );
    SCP_GETARGFX( Type2, Arg2, Arg2f, Arg2p, scr );
    SCP_DBGA( "( [%x]%x + [%x]%x )", Type2, Arg2, Type1, Arg1 );
//printf( "( [%x]%x(%p) + [%x]%x(%p) )\n", Type2, Arg2,Arg2p, Type1, Arg1, Arg1p );
    t2 = Type2 & 0xF7FF;
    if( t2 == SCR_STRING ){
    	    s = NULL;    	    
    	    if( Type2 & 0x0800 ){
    		s = &scr->Strings[ Arg2 + 4 ];
    	    } else {
    		if( Type2 & 0x1000 ) s = &scr->StringsConst[ Arg2 + 4 ];
    	    }
	    q = NULL;
	    switch( Type1 & 0xF7FF ){
    		case SCR_FSTRING:
    		case SCR_STRING:
        	    if( Type1 & 0x800 ){
            		pd = &scr->Strings[ Arg1 + 4 ];
        	    } else if( Type1 & 0x1000 ){
            		pd = &scr->StringsConst[ Arg1 + 4 ];
        	    } else {
            		pd = NULL;
        	    }
        	    q = dbg_malloc( strlen( pd ) + 1 );
        	    strcpy( q, pd );    	    
    		    break;
    		case SCR_FLOAT:
    		    q = dbg_malloc( 80 );
    		    sprintf( q, "%.5f", Arg1f );
    		    break;
    		case SCR_INT:
    		    q = dbg_malloc( 80 );
    		    sprintf( q, "%d", Arg1 );
    		    break;
		case SCR_PTR:
    		    IntpPushPtrStack( scr->StackA, &scr->StackApos, NULL ); IntpPushwA( scr, SCR_PTR ); // illegal operations to pointer
    		    break;
    	    }
    	    p = dbg_malloc( strlen( s ) + strlen( q ) + 1 );
    	    strcpy( p, s );
    	    strcpy( p + strlen( p ), q );
    	    IntpPushIntStack( scr->StackA, &scr->StackApos, IntpAddString( scr, p ) ); 
    	    IntpPushwA( scr, SCR_FSTRING );
    	    dbg_free( q );
    	    dbg_free( p );
    	    return;
    }
    if( t2 == SCR_INT ){
    	    switch( Type1 & 0xF7FF ){
        	case SCR_FLOAT:
            	    IntpPushIntStack(scr->StackA, &scr->StackApos, Arg2 + Arg1 ); IntpPushwA( scr, SCR_FLOAT );
        	    break;
        	case SCR_INT:
            	    if( (Arg1 <= 0 || 0x7FFFFFFF - Arg1 >= Arg2) && (Arg1 >= 0 || (0x80000001 - Arg1) <= Arg2) ){
                	IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 + Arg2 ); IntpPushwA( scr, SCR_INT );
            	    } else {
                	IntpPushIntStack(scr->StackA, &scr->StackApos, Arg2 + Arg1 ); IntpPushwA( scr, SCR_FLOAT );
            	    }        	
            	    break;
    		case SCR_FSTRING:
    		case SCR_STRING:
    		    pd = NULL;
    		    if( Type1 & 0x0800 ){
    			 pd = &scr->Strings[ Arg1 + 4 ];
    		    } else {
    			if( Type1 & 0x1000 ) pd = &scr->StringsConst[ Arg1 + 4 ];
    		    }
    		    p = dbg_malloc( strlen( pd ) + 80 );
    		    sprintf( p, "%d", Arg2 );
    		    strcpy( p + strlen( p ), pd );
    		    IntpPushIntStack( scr->StackA, &scr->StackApos, IntpAddString( scr, p ) ); IntpPushwA( scr, SCR_FSTRING );
    		    dbg_free( p );
    	    	    break;
		case SCR_PTR:
    		    IntpPushPtrStack( scr->StackA, &scr->StackApos, NULL ); IntpPushwA( scr, SCR_PTR ); // illegal operations to pointer
    		    break;
    	    }
    	    return;
    }
    if( t2 == SCR_INT ){
	switch( Type1 & 0xF7FF ){
    	    case SCR_FSTRING:
    	    case SCR_STRING:
    		pd = NULL;    	    
    		if( Type1 & 0x0800 ){
    	    	    pd = &scr->Strings[ Arg1 + 4 ];
    		} else {
    		    if( Type1 & 0x1000 ) pd = &scr->StringsConst[ Arg1 + 4 ];
    		}
    		p = dbg_malloc( strlen( pd ) + 80 );
    		sprintf( p, "%.5f", Arg2f );
    		strcpy( p + strlen( p ), pd );
    	        IntpPushIntStack(scr->StackA, &scr->StackApos, IntpAddString( scr, p ) ); IntpPushwA( scr, SCR_FSTRING );
    		dbg_free( p );
    		break;
	    case SCR_FLOAT:
    		IntpPushIntStack(scr->StackA, &scr->StackApos, Arg2 + Arg1f ); IntpPushwA(scr, SCR_FLOAT);
    		break;
	    case SCR_INT:
    		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 + Arg2 ); IntpPushwA( scr, SCR_FLOAT );
    		break;
    	    case SCR_PTR:
    		IntpPushPtrStack( scr->StackA, &scr->StackApos, Arg1p + Arg2 ); IntpPushwA( scr, SCR_PTR );
    	        break;
	}
	return;
    }
    if( t2 == SCR_PTR ){
	switch( Type1 & 0xF7FF ){
    	    case SCR_FSTRING:
    	    case SCR_STRING:
    	    case SCR_PTR:
	    case SCR_FLOAT:
    		IntpPushPtrStack( scr->StackA, &scr->StackApos, NULL ); IntpPushwA( scr, SCR_PTR ); // illegal operations to pointer
    		break;
	    case SCR_INT:
    		IntpPushPtrStack( scr->StackA, &scr->StackApos, Arg1 + Arg2p ); IntpPushwA( scr, SCR_PTR );
    		break;
	}
	return;
    }    
}

void ScpA_Sub( Intp_t *scr ) // 803A
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x - [%x]%x )", Type2, Arg2, Type1, Arg1 );
    switch( Type2 ){
	case SCR_FLOAT: // float - float/int
    	    IntpPushIntStack( scr->StackA, &scr->StackApos, ( Type1 == SCR_FLOAT ) ? Arg2f - Arg1f : Arg2f - Arg1 );
    	    IntpPushwA( scr, SCR_FLOAT );
    	    break;
	case SCR_INT:
	    if( Type1 == SCR_FLOAT ){ // int - float
    		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 - Arg1f );	
    		IntpPushwA( scr, SCR_FLOAT );
	    } else { // int - int
    		IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 - Arg1 ); 
    		IntpPushwA( scr, SCR_INT );
	    }
	    break;
    }
}

void ScpA_Mul( Intp_t *scr ) // 803B
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x * [%x]%x )", Type2, Arg2, Type1, Arg1 );
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
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f, tmp;
    
    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x / [%x]%x )", Type2, Arg2, Type1, Arg1 );

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
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    SCP_DBGA( "( [%x]%x %% [%x]%x )", Type2, Arg2, Type1, Arg1 );
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
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x && [%x]%x )", Type2, Arg2, Type1, Arg1 );
    
    switch( Type2 ){
	case SCR_STRING :
	case SCR_FSTRING:
	    switch( Type1 ){
    		case SCR_STRING:
    		case SCR_FSTRING: IntpPushIntStack( scr->StackA, &scr->StackApos, 1 ); break;
    		case SCR_FLOAT: IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1f != 0.0 ); break;
    		case SCR_INT: IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 != 0 ); break;
    	    }
    	    break;    
	case SCR_FLOAT:
	    switch( Type1 ){
    		case SCR_STRING:
    		case SCR_FSTRING: IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f != 0 ); break;
    		case SCR_FLOAT:   IntpPushIntStack( scr->StackA, &scr->StackApos, (Arg2f != 0.0) && (Arg1f != 0.0) ); break;
        	case SCR_INT:     IntpPushIntStack( scr->StackA, &scr->StackApos, (Arg2f != 0.0 ) && Arg1 ); break;
	    }        
    	    break;        
	case SCR_INT:
	    switch( Type1 ){
    		case SCR_STRING: 
		case SCR_FSTRING: IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 != 0 ); break;
    		case SCR_FLOAT:   IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 && (Arg1f != 0.0) ); break;
    		case SCR_INT:     IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 && Arg1 ); break;
	    }
	    break;
    }
    IntpPushwA( scr, SCR_INT );
    return;
}

void ScpA_LogOr( Intp_t *scr ) // 803F ||( p1 , p2 )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x || [%x]%x )", Type2, Arg2, Type1, Arg1 );
    
    switch( Type2 ){
	case SCR_STRING :
	case SCR_FSTRING:
	    switch( Type1 ){
    		case SCR_STRING:
    		case SCR_FSTRING: IntpPushIntStack( scr->StackA, &scr->StackApos, 1 ); break;
    		case SCR_FLOAT: IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1f != 0.0 ); break;
    		case SCR_INT: IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 != 0 ); break;
    	    }
    	    break;    
	case SCR_FLOAT:
	    switch( Type1 ){
    		case SCR_STRING:
    		case SCR_FSTRING: IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2f != 0 ); break;
    		case SCR_FLOAT:   IntpPushIntStack( scr->StackA, &scr->StackApos, (Arg2f != 0.0) || (Arg1f != 0.0) ); break;
        	case SCR_INT:     IntpPushIntStack( scr->StackA, &scr->StackApos, (Arg2f != 0.0 ) || Arg1 ); break;
	    }        
    	    break;        
	case SCR_INT:
	    switch( Type1 ){
    		case SCR_STRING: 
		case SCR_FSTRING: IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 != 0 ); break;
    		case SCR_FLOAT:   IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 || (Arg1f != 0.0) ); break;
    		case SCR_INT:     IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 || Arg1 ); break;
	    }
	    break;
    }
    IntpPushwA( scr, SCR_INT );
    return;

}

void ScpA_LogNot( Intp_t *scr ) // 8045 !( arg )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "!( [%x]%x )", Type, Arg );
    SCP_PUSHARG_A( SCR_INT, Arg == 0, scr );
}

void ScpA_Neg( Intp_t *scr ) // 8046 -( arg )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "-( [%x]%x )", Type, Arg );
    SCP_PUSHARG_A( SCR_INT, -Arg, scr );
}

void ScpA_BinNot( Intp_t *scr ) // 8043 ~( arg )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "~( [%x]%x )", Type, Arg );
    SCP_PUSHARG_A( SCR_INT, ~Arg, scr );
}

void ScpA_Floor( Intp_t *scr ) // 8044 floor( float arg )
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "FLOOR( [%x]%x )", Type, Arg );
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
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x & [%x]%x )", Type2, Arg2, Type1, Arg1 );
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
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x | [%x]%x )", Type2, Arg2, Type1, Arg1 );
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
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;
    float Arg2f, Arg1f;

    SCP_GETARGF( Type1, Arg1, Arg1f, scr );
    SCP_GETARGF( Type2, Arg2, Arg2f, scr );
    SCP_DBGA( "( [%x]%x ^ [%x]%x )", Type2, Arg2, Type1, Arg1 );
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
    SCP_DBG_VAR;
    int Arg1, Arg2;
    uint16_t Type1, Type2;

    Type1 = IntpPopwB( scr ); Arg1 = IntpPopIntStack( scr->StackB, &scr->StackIdxB );
    Type2 = IntpPopwB( scr ); Arg2 = IntpPopIntStack( scr->StackB, &scr->StackIdxB );

    SCP_DBGA( "  SWAPA: [%x]%d <-> [%x]%d", Type2, Arg2, Type1, Arg1 );
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, Arg1 ); IntpPushwB( scr, Type1 );
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, Arg2 ); IntpPushwB( scr, Type2 );
}

void ScpA_EndCritical( Intp_t *scr ) // 8003, 804B
{
    SCP_DBG( scr, "CRITICAL END" );
    scr->Flags &= ~SCR_FCRITICAL;
}

void ScpA_StartCritical( Intp_t *scr ) // 8002, 804A O_CRITICAL_START
{
    SCP_DBG( scr, "CRITICAL START" );
    scr->Flags |= SCR_FCRITICAL;
}

void ScpA_Jump( Intp_t *scr ) // 8004 jmp()
{
    SCP_DBG_VAR;
    unsigned short Type;
    char stmp[256];
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "JUMP [%x]%x", Type, Arg );
    SCP_DECHO( "\n" );
    Type &= ~0x800;
    SCP_ERRTYPEF( Type != SCR_INT, stmp, "Invalid type given to jmp, %x", Type );
    scr->CodePC = Arg;
}

void ScpA_CallExt( Intp_t *scr ) // 8005 call( adres ) or call( ?, adres )
{
    SCP_DBG_VAR;
    unsigned short Type, Type1;
    char *ExtName, stmp[256], *s;
    int Arg,i,ArgL, ArgC_got, flags;
    short TypeL, ArgC_expected;    
    Intp_t LocalScr;
    int ArgV;

    SCP_GETARG( Type, Arg, scr );
    SCP_ERRTYPE( SCRTYPE( Type ) != SCR_INT, "Invalid address given to call" );
    // get procedure name
    flags = IntpReadBei( (char *)scr->ProcTable + 24 * Arg + 8, 0 );
    if( !(flags & INTP_P_IMPORT )){ // not imported procedure
	SCP_DBGA( "CALL( [%x]%x )", Type, Arg );
        scr->CodePC = IntpReadBei( (char *)scr->ProcTable + 24 * Arg + 20, 0 ); // BodyOffst
        if( flags & INTP_P_CRITICAL ) scr->Flags |= SCR_FCRITICAL;
        return;
    }
    // Imported procedure
    flags = IntpReadBei( (char *)scr->ProcTable + 24 * Arg + 4, 0 );
    ExtName = scr->ProcVarNames + flags;
DD
printf("=>%p, %x\n", ExtName, flags );
printf("==>%s\n", ExtName );
    SCP_ERRTYPEF( !ExportGetProcedure( ExtName, &ArgV, &ArgC_expected ), "External procedure %s not found", ExtName );
    SCP_GETARG( Type1, ArgC_got, scr );
    SCP_ERRTYPEF( SCRTYPE( Type1 ) != SCR_INT || ArgC_got != ArgC_expected, stmp, "Wrong number of arguments to external procedure %s.Expecting %d, got %d.", ExtName, ArgC_expected, ArgC_got );
    SCP_DBGA( "CALL_EXT( [%x]%x, %s )", Type1, ArgC_got, ExtName );
    // save context
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, scr->CodePC ); IntpPushwB( scr, SCR_INT );
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
                s = &scr->Strings[ ArgL + 4 ];
            } else if( TypeL & 0x1000 ){
                s = &scr->StringsConst[ ArgL + 4 ];
            } else {
                s = NULL;
            }
            IntpPushIntStack( scr->StackA, &scr->StackApos, IntpAddString( scr, s ) ); IntpPushwA( scr, SCR_FSTRING );
        } else {
            IntpPushIntStack( scr->StackA, &scr->StackApos, ArgL ); IntpPushwA( scr, TypeL );
        }
    }

    IntpPushIntStack( scr->StackA, &scr->StackApos, ArgC_expected ); IntpPushwA( scr, SCR_INT );
    scr->Flags |= 0x20;
    scr->Flags = 0;
    i = IntpReadBei( &scr->ProcTable[ 6 * Arg ].NameOfst, 4 );
    scr->CodePC = ArgV;
printf("==>%x\n", ArgV);
    if( (i & 0x10) || (scr->Flags & SCR_FCRITICAL) ) scr->Flags |= SCR_FCRITICAL;
}

void ScpA_ExitProc( Intp_t *scr ) // 801F ?( arg1, arg2, arg3 )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2, Type3;
    int Arg1, Arg3;

    SCP_GETARG( Type1, Arg1, scr ); scr->i34 = Arg1; // window
    Type2 = IntpPopShortStack( scr->StackA, &scr->StackApos);
    if( Type2 == SCR_FSTRING ){
	Arg1 = IntpPopIntStack( scr->StackA, &scr->StackApos);
	IntpStringDeRef( scr, SCR_FSTRING, Arg1 );
	scr->Func = NULL;
	SCP_DBGA( "EXIT_PROC( [%x]%x )", Type2, Arg1 );
    } else {
	scr->Func = IntpPopPtrStack( scr->StackA, &scr->StackApos);
	SCP_DBGA( "EXIT_PROC( [%x]%p )", Type2, scr->Func );
    }    
    SCP_GETARG( Type3, Arg3, scr ); scr->Flags = Arg3;
    SCP_DECHO( "---------------------------------------------------\n\n" );
}

void ScpA_RetA( Intp_t *scr ) // 801C
{
    SCP_DBG_VAR;

    SCP_DBGA( "RET" );
    IntpPopwB( scr );
    scr->CodePC = IntpPopIntStack( scr->StackB, &scr->StackIdxB );
    SCP_DECHO( "\n" );
}

void ScpA_RetB( Intp_t *scr ) // 801D
{
    SCP_DBG( scr, "RET F" );
    IntpPopwB( scr );
    scr->CodePC = IntpPopIntStack( scr->StackB, &scr->StackIdxB );
    scr->Flags |= 0x40;
}

void ScpA_8020( Intp_t *scr ) // 8020 O_POP_FLAGS_RETURN
{
    SCP_DBG( scr, "O_POP_FLAGS_RETURN()" );
    ScpA_ExitProc( scr );
    IntpPopwB( scr ); 
    scr->CodePC = IntpPopiB( scr );
}

void ScpA_8021( Intp_t *scr ) // 8021 O_POP_FLAGS_EXIT
{
    SCP_DBG( scr, "O_POP_FLAGS_EXIT()" );
    ScpA_ExitProc( scr );
    IntpPopwB( scr ); scr->CodePC = IntpPopiB( scr );
    scr->Flags |= 0x40;
}

void ScpA_8025( Intp_t *scr ) // 8025 O_POP_FLAGS_RETURN_VAL_EXIT
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "O_POP_FLAGS_RETURN_VAL_EXIT( [%x]%x )", Type, Arg );
    ScpA_ExitProc( scr );
    IntpPopwB( scr ); scr->CodePC = IntpPopiB( scr );
    scr->Flags |= 0x40;
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg ); IntpPushwA( scr, Type );
}

void ScpA_8026( Intp_t *scr ) // 8026 O_POP_FLAGS_RETURN_VAL_EXIT_EXTERN
{
    SCP_DBG_VAR;
    int Arg;
    Intp_t *p;
    unsigned short Type;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "O_POP_FLAGS_RETURN_VAL_EXIT_EXTERN( [%x]%x )", Type, Arg );
    ScpA_ExitProc( scr );
    IntpPopwB( scr ); p = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Func = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Flags = IntpPopIntStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); scr->CodePC = IntpPopiB( scr );
    scr->Flags |= 0x40;
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg );
    IntpPushwA( scr, Type );
}

void ScpA_8022( Intp_t *scr ) // 8022 O_POP_FLAGS_RETURN_EXTERN
{
    Intp_t *p;

    SCP_DBG( scr, "O_POP_FLAGS_RETURN_EXTERN()" );
    ScpA_ExitProc( scr );
    IntpPopwB( scr ); p = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Func = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Flags = IntpPopIntStack( scr->StackB, &scr->StackIdxB ); IntpPopwB( scr );
    scr->CodePC = IntpPopiB( scr );
}

void ScpA_8023( Intp_t *scr ) // 8023 O_POP_FLAGS_EXIT_EXTERN
{
    Intp_t *p;

    SCP_DBG( scr, "O_POP_FLAGS_EXIT_EXTERN()" );
    ScpA_ExitProc( scr );
    IntpPopwB( scr ); p = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Func = IntpPopPtrStack(scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Flags = IntpPopIntStack(scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); scr->CodePC = IntpPopiB( scr );
    scr->Flags |= 0x40;
}

void ScpA_8024( Intp_t *scr ) // 8024 O_POP_FLAGS_RETURN_VAL_EXTERN
{
    SCP_DBG_VAR;
    Intp_t *p;
    char *s;
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "O_POP_FLAGS_RETURN_VAL_EXTERN( [%x]%x )", Type, Arg );
    ScpA_ExitProc( scr );
    IntpPopwB( scr ); p = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Func = IntpPopPtrStack( scr->StackB, &scr->StackIdxB );
    IntpPopwB( scr ); p->Flags = IntpPopIntStack( scr->StackB, &scr->StackIdxB );
    if( SCRTYPE( Type ) == SCR_STRING ){
        if( Type & 0x800 ){
            s = &scr->Strings[ Arg + 4 ];
        } else if( Type & 0x1000 ){
            s = &scr->StringsConst[ Arg + 4 ];
        } else {
            s = NULL;
        }
        IntpPushIntStack( p->StackA, &p->StackApos, IntpAddString( p, s ) ); 
	IntpPushwA( p, SCR_FSTRING );
    } else {
        IntpPushIntStack( p->StackA, &p->StackApos, Arg ); 
        IntpPushwA( p, Type );
    }
    if( p->Flags & 0x80 ) scr->Flags &= ~0x80;
    IntpPopwB( scr ); scr->CodePC = IntpPopiB( scr );
    IntpPopwB( p ); p->CodePC = IntpPopiB( p );
}

void ScpA_801E( Intp_t *scr ) // 801E
{
    SCP_DBG( scr, "?801E?" );
    IntpPopwB( scr ); IntpPopIntStack( scr->StackB, &scr->StackIdxB );
}

void ScpA_800C( Intp_t *scr ) // 800C
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    SCP_GETARG_B( Type, Arg, scr );
    SCP_DBGA( "  ADR To DAT -> [%x]%x", Type, Arg );
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg ); IntpPushwA( scr, Type );
}

void ScpA_800D( Intp_t *scr ) // 800D O_D_TO_A
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "  DAT To ADR -> [%x]%x", Type, Arg );
    IntpPushIntStack( scr->StackB, &scr->StackIdxB, Arg ); IntpPushwB( scr, Type );
}

void ScpA_8010( Intp_t *scr ) // 8010 O_EXIT_PROG
{
    SCP_DBG( scr, "O_EXIT_PROG()" );
    scr->Flags |= SCR_FEXIT;
    SCP_DECHO( "--->" );
}

void ScpA_8011( Intp_t *scr ) // 8011
{
    SCP_DBG( scr, "?8011?()" );
    scr->Flags |= 0x08;
}

void ScpA_8012( Intp_t *scr ) // 8012 GetLocVar() ?
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg, val;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "?8012?( [%x]%x )",Type, Arg );
    Type = *( (uint16_t *)&scr->StackA[ 6 * Arg + 4 + scr->SaveStack ] );
    Type = ((Type & 0x00ff) << 8) | ( (Type & 0xff00) >> 8 );
    val = IntpReadBei( scr->StackA, 6 * Arg + scr->SaveStack );
    SCP_PUSHARG_A( Type, val, scr );
}

void ScpA_8013( Intp_t *scr ) // 8013 SetLocVar( )?
{
    SCP_DBG_VAR;
    int Arg1,Arg2, tmp;
    unsigned short Type1, Type2;
    
    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    SCP_DBGA( "?8013?( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );

    tmp = IntpReadBei( scr->StackA, 6 * Arg1 + scr->SaveStack );
    if( *(uint16_t *)&scr->StackA[ 6 * Arg1 + 4 + scr->SaveStack ] == 0x0198 ) IntpStringDeRef( scr, SCR_FSTRING, tmp );

    IntpWriteBew( Arg2 >> 8, scr->StackA, 6 * Arg1 + scr->SaveStack );
    IntpWriteBew( Arg2, scr->StackA, 6 * Arg1 + scr->SaveStack + 2 );
    scr->StackA[ 6 * Arg1 + 4 + scr->SaveStack + 0 ] = Type2 >> 8;
    scr->StackA[ 6 * Arg1 + 4 + scr->SaveStack + 1 ] = Type2;
    if( Type2 == SCR_FSTRING ) (*(uint16_t *)&scr->Strings[ Arg2 + 2 ] )++;

}

void ScpA_8018( Intp_t *scr ) // 8018
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    int Arg1, Arg2;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    SCP_DBGA( "?8018?( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg1 ); IntpPushwA( scr, Type1 );
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg2 ); IntpPushwA( scr, Type2 );
}

void ScpA_FetchAddr( Intp_t *scr ) // 802D
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;
    char stmp[ 260 ];

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "FETCH_PROC_ADDRESS( [%x]%x )", Type, Arg );
    SCP_ERRTYPEF( Type != SCR_INT, stmp, "Invalid type given to fetch_proc_address, %x", Type );
    IntpPushIntStack( scr->StackA, &scr->StackApos, IntpReadBei( (char *)&scr->ProcTable[ Arg ].NameOfst, 16 ) ); IntpPushwA( scr, SCR_INT );
}

void ScpA_801A( Intp_t *scr ) // 801A O_POP
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "O_POP( [%x]%x )", Type, Arg );
}

void ScpA_801B( Intp_t *scr ) // 801B
{
    SCP_DBG_VAR;
    unsigned short Type;
    int Arg;

    SCP_GETARG( Type, Arg, scr );    
    SCP_DBGA( "?801B?( [%x]%x )", Type, Arg );
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg ); IntpPushwA( scr, Type );
    IntpPushIntStack( scr->StackA, &scr->StackApos, Arg ); IntpPushwA( scr, Type );
}

void ScpA_SetExportedVar( Intp_t *scr ) // 8015
{
    SCP_DBG_VAR;
    uint16_t Type1, Type2;
    char stmp[ 256 ];
    int Arg1, Arg2;
    void *ptr;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARGX( Type2, Arg2, ptr, scr );
    if( Type2 != SCR_PTR ){
	SCP_DBGA( "SET_EXPORTED_VAR: %s = {[%x]%x}", SCP_DNAME( Arg1 ), Type2, Arg2 );
	if( ExportSetVarArg( scr, (char *)scr->ProcVarNames + Arg1, Type2, Arg2 ) ){
    	    sprintf( stmp, "External variable %s does not exist\n", IntpGetName( scr, Arg1 ) );
    	    IntpError( stmp );
	}
    } else {
	SCP_DBGA( "SET_EXPORTED_VAR: %s = %p", SCP_DNAME( Arg1 ), ptr );
	if( ExportSetVarArgP( scr, (char *)scr->ProcVarNames + Arg1, ptr ) ){
    	    sprintf( stmp, "External variable %s does not exist\n", IntpGetName( scr, Arg1 ) );
    	    IntpError( stmp );
	}
    }
}

void ScpA_GetExportedVar( Intp_t *scr ) // 8014
{
    SCP_DBG_VAR;
    short Type;
    char stmp[ 256 ];
    int Arg, tmp[ sizeof( void *) / 4 ];

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "GET_EXPORTED_VAR( %s )", SCP_DNAME( Arg ) );
    if( ExportGetVarArg( scr, (char *)scr->ProcVarNames + Arg, &Type, tmp ) ){
        sprintf( stmp, "External variable %s does not exist\n", IntpGetName( scr, Arg ) );
        IntpError( stmp );
    }
    if( Type == SCR_PTR )
	IntpPushPtrStack( scr->StackA, &scr->StackApos, *((void **)tmp) ); 		
    else
	IntpPushIntStack( scr->StackA, &scr->StackApos, *tmp ); 
    IntpPushwA( scr, Type );
}

void ScpA_ExportProcedure( Intp_t *scr ) // 8017
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    char stmp[ 256 ], *tmp;
    int Arg1, Arg2;
    void *n;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    SCP_DBGA( "EXPORT_PROCEDURE: [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    tmp = &scr->ProcVarNames[ IntpReadBei( &scr->ProcTable[ 6 * Arg1 ].NameOfst, 0 ) ];
    n = IntpReadBep( &scr->ProcTable[ 6 * Arg1 ].NameOfst, 16 );
    SCP_ERRTYPEF( ExportSetProcedure( scr, tmp,  n, Arg2 ),stmp, "Error exporting procedure %s", tmp );
}

void ScpA_ExportVariable( Intp_t *scr ) // 8016
{
    SCP_DBG_VAR;
    unsigned short Type;
    char stmp[ 256 ];
    int Arg;
    
    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "EXPORT_VARIABLE: %s", SCP_DNAME( Arg ) );
    if( ExportCreateIVar( &scr->FileName, (char *)scr->ProcVarNames + Arg ) ){
        sprintf( stmp, "External variable %s already exists", IntpGetName( scr, Arg ) );
        IntpError( stmp );
    }
}

void ScpA_KillProcedure( Intp_t *scr ) // 800E
{
    SCP_DBG( scr, "KILL_PROCEDURE()" );
    scr->Flags |= SCR_FEXIT;
    if( scr->Parent && (scr->Parent->Flags & 0x100) != 0 ) scr->Parent->Flags &= ~SCR_FATTACH;
    if( scr->i35 ) return;
    IntpTaskFire( scr );
    scr->i35 = 1;
}

void ScpA_Detach( Intp_t *scr ) // 800F
{
    SCP_DBG( scr, "DETACH()" );
    if( !scr->Parent ) return;
    scr->Parent->Flags &= ~SCR_FPROC_RUN;
    scr->Parent->Flags &= ~SCR_FATTACH;
    if( scr == scr->Parent->ChildProcess ) scr->Parent->ChildProcess = NULL;    
}

void ScpA_CallStart( Intp_t *scr ) // 8008 callstart()
{
    SCP_DBG_VAR;
    Intp_t *LocScr;
    unsigned short Type;
    char stmp[ 256 ], *v10;
    int Arg;

    SCP_ERRTYPE( scr->ChildProcess, "Error, already have a child process\n" );
    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "CALL_START( [%x]%x)", Type, Arg );
    SCP_ERRTYPE( SCRTYPE( Type ) != SCR_STRING , "Invalid type given to callstart" );
    scr->Flags |= SCR_FPROC_RUN;
    if( Type & 0x800 ){
        v10 = &scr->Strings[ Arg + 4 ];
    } else if( Type & 0x1000 ){
        v10 = &scr->StringsConst[ Arg + 4 ];
    } else {
        v10 = 0;
    }    
    LocScr = IntpLoad( IntpMseHandler( v10 ) );
    if( LocScr ){
        SciItpEqAdd( LocScr );
        SciOpcodeExec( LocScr, 24 );
    }
    scr->ChildProcess = LocScr;
    SCP_ERRTYPEF( !LocScr, stmp, "Error spawning child %s", IntpGetString( scr, Type >> 8, Arg ) );
    scr->ChildProcess->Parent = scr;
    scr->ChildProcess->i34 = scr->i34;
}

void ScpA_Spawn( Intp_t *scr ) // 800A
{
    SCP_DBG_VAR;
    Intp_t *LocScr;
    unsigned short Type;
    char stmp[ 256 ], *v10;
    int Arg;

    SCP_ERRTYPE( scr->ChildProcess, "Error, already have a child process\n" );
    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "SPAWN( [%x]%x )", Type, Arg );
    SCP_ERRTYPE( SCRTYPE( Type ) != SCR_STRING, "Invalid type given to spawn" );
    scr->Flags |= 0x0100;
    if( (Type & 0x800) != 0 ){
        v10 = &scr->Strings[ Arg + 4 ];
    } else if ( (Type & 0x1000) != 0 ){
        v10 = &scr->StringsConst[ Arg + 4 ];
    } else {
        v10 = 0;
    }
    LocScr = IntpLoad( IntpMseHandler( v10 ) );
    if( LocScr ){
        SciItpEqAdd( LocScr );
        SciOpcodeExec( LocScr, 24 );
    }
    scr->ChildProcess = LocScr;
    SCP_ERRTYPEF( !LocScr, stmp, "Error spawning child %s", IntpGetString( scr, Type >> 8, Arg ) );
    scr->ChildProcess->Parent = scr;
    scr->ChildProcess->i34 = scr->i34;
    if( scr->Flags & SCR_FCRITICAL ){
        scr->ChildProcess->Flags |= 0x80;
        SciOpcodeExec( scr->ChildProcess, -1 );
    }
}

void ScpA_Fork( Intp_t *scr ) // 800B fork()
{
    SCP_DBG_VAR;
    Intp_t *LocScr;
    short Type;
    char stmp[ 256 ], *v6;
    int Arg;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA("FORK( [%x]%x )",Type, Arg );
    if( (Type & 0x800) != 0 ){
        v6 = &scr->StringsConst[Arg + 4];
    } else if ( (Type & 0x1000) != 0 ){
        v6 = &scr->StringsConst[ Arg + 4 ];
    } else {
        v6 = 0;
    }    
    LocScr = IntpLoad( IntpMseHandler( v6 ) );
    if( LocScr ){
        SciItpEqAdd( LocScr );
        SciOpcodeExec( LocScr, 24 );
    }
    SCP_ERRTYPEF( !LocScr, stmp, "couldn't fork script '%s'", IntpGetString( scr, Type >> 8, Arg ) );
    LocScr->i34 = scr->i34;
}

void ScpA_8009( Intp_t *scr ) // 8009
{
    Intp_t *Parent;
    
    Parent = scr->Parent;
    SCP_DBG( scr, "?8009?");    
    ScpA_Fork( scr );
    if( Parent ){
        scr->Parent = Parent;
        Parent->ChildProcess = scr;
    }
    scr->ChildProcess = NULL;
    scr->Parent = NULL;
    scr->Flags |= SCR_FEXIT;
    if( scr->Parent && scr->Parent->Flags & 0x0100 ) scr->Parent->Flags &= ~0x100;    
    IntpTaskRun( scr );
}

void ScpA_8027( Intp_t *scr ) // 8027 ?( arg1, arg2 )
{
    SCP_DBG_VAR;
    unsigned short Type1, Type2;
    char stmp[ 256 ];
    int Arg1, Arg2;

    SCP_GETARG( Type1, Arg1, scr );
    SCP_GETARG( Type2, Arg2, scr );
    SCP_DBGA( "PROCEDURE[?]( [%x]%x, [%x]%x )", Type2, Arg2, Type1, Arg1 );
    if( IntpReadBei( &scr->ProcTable->NameOfst + 6 * Arg2, 20 ) != Arg1 ){
        sprintf( stmp, "Wrong number of args to procedure %s\n", IntpGetName( scr, IntpReadBei( &scr->ProcTable->NameOfst + 6 * Arg2, 0 ) ) );
        IntpError( stmp );
    }
}

void ScpA_LookupStringProc( Intp_t *scr ) // 8028
{
    SCP_DBG_VAR;
    unsigned short Type;
    char stmp[ 256 ], *s, *a;
    int Arg, i, cnt;
    void *p;

    SCP_GETARG( Type, Arg, scr );
    SCP_DBGA( "LOOKUP_STRING_PROC( [%x]%x )", Type, Arg );
    SCP_ERRTYPE( SCRTYPE( Type ) != SCR_STRING, "Wrong type given to lookup_string_proc\n" );

    if( Type & 0x800 ){
        s = &scr->Strings[ Arg + 4 ];
    } else if( Type & 0x1000 ){
        s = &scr->StringsConst[ Arg + 4 ];
    } else {
        s = NULL;
    }

    cnt = IntpReadBei( scr->ProcTable, 0 ) - 1;
    p = &scr->ProcTable[ 1 ];
    for( i = 0; i < cnt; i++, p += 6 ){
	a = scr ? &scr->ProcVarNames[ IntpReadBei( p, 0 ) ] : NULL;
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
    SciAddOpcode( 0x8001, ScpA_PushConst );
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
    SciAddOpcode( 0x801F, ScpA_ExitProc );
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

