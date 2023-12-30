#pragma once

#define SCP_GETARG_A( type, val, p ) 	type = IntpPopShortStack( (p)->StackA, &(p)->StackApos ); val = IntpPopIntStack( (p)->StackA, &(p)->StackApos );
#define SCP_GETARG_B( type, val, p )    type = IntpPopwB( (p) ); val = IntpPopIntStack( (p)->StackB, &(p)->StackIdxB );
#define SCP_PUSHARG_A( type, val, p )	IntpPushIntStack( (p)->StackA, &(p)->StackApos, val ); IntpPushwA( (p), type );
#define SCP_PUSHARG_B( type, val, p )  	IntpPushIntStack( (p)->StackB, &(p)->StackIdxB, val ); IntpPushwB( (p), type );
#define SCP_ERRTYPE( cond, str )  if( cond ) IntpError( str )
#define SCP_ERRTYPEF( cond, s, fmt, m... )  if( cond ){ sprintf( s, fmt, ##m ); IntpError( s ); }
#define SCP_FSTRDEREF( type, val, scr ) if( (type) == SCR_FSTRING ) IntpStringDeRef( scr, SCR_FSTRING, val );
#define SCP_GETARG( type, ArgsCnt, scr ) SCP_GETARG_A( type, ArgsCnt, scr ); SCP_FSTRDEREF( type, ArgsCnt, scr );
#define SCP_GETARGF( type, Argi, Argf, scr)   SCP_GETARG( type, Argi, scr ); Argf = *((float *)(&Argi));

//int gScpUnk01;
extern int gSciUnk01;
extern int gScpUnk01;
/*
    name: o_nop()
    code: 0x8000
    ret: void
    dsc: do nothing
*/
void ScpA_Nop( Intp_t *scr );

/*
    name: ?
    code 0x8001
    ret: ?
    dsc: ?
*/
void ScpA_8001( Intp_t *scr ); // push address pointer onto stack
void ScpA_802B( Intp_t *scr ); // 802B push_base( ArgsCnt ) ?
void ScpA_PopBase( Intp_t *scr ); // 8029 'pop_base( base )'
void ScpA_Ret( Intp_t *scr ); // 802A 'Ret( ... )
void ScpA_802C( Intp_t *scr ); // 802C save_stack() ?
void ScpA_Dump( Intp_t *scr ); // 802E dump( arg_cnt, ... )
void ScpA_Call( Intp_t *scr ); // 8006 'call( procedure, procedure_time )
void ScpA_CondCall( Intp_t *scr ); // 8007 cond_call( procedure, procedure_address )
void ScpA_Wait( Intp_t *scr ); // 8047 'wait( time )'
void ScpA_Cancel( Intp_t *scr ); // 8048 'cancel( proc )'
void ScpA_CancelAll( Intp_t *scr ); // 8049 'CancelAll()'
void ScpA_If( Intp_t *scr ); // 802F ifcon( Cond, skip_addr ) -> 'if( Cond ) then begin'
void ScpA_WhileDo( Intp_t *scr ); // 8030 while( Cond, SkipAddress ) -> 'while( Cond ) do begin'
void ScpA_8031( Intp_t *scr ); // 8031 ?( arg1, arg2 )
void ScpA_Fetch( Intp_t *scr ); // 8032
void ScpA_CmpNE( Intp_t *scr ); // 8034 'stack:=p2<>p1'
void ScpA_CmpEQ( Intp_t *scr ); // 8033 'stack:= p2==p1'
void ScpA_CmpLE( Intp_t *scr ); // 8035 'stack:= p2<=p1'
void ScpA_CmpGE( Intp_t *scr ); // 8036 'stack:=p2>=p1'
void ScpA_CmpLS( Intp_t *scr ); // 8037 'stack:=p2<p1'
void ScpA_CmpGT( Intp_t *scr ); // 8038 'stack:=p2>p1''
void ScpA_Add( Intp_t *scr ); // 8039
void ScpA_Sub( Intp_t *scr ); // 803A
void ScpA_Mul( Intp_t *scr ); // 803B
void ScpA_Div( Intp_t *scr ); // 803C
void ScpA_Mod( Intp_t *scr ); // 803D
void ScpA_LogAnd( Intp_t *scr ); // 803E
void ScpA_LogOr( Intp_t *scr ); // 803F ||( p1 , p2 )
void ScpA_LogNot( Intp_t *scr ); // 8045 !( arg )
void ScpA_Neg( Intp_t *scr ); // 8046 -( arg )
void ScpA_BinNot( Intp_t *scr ); // 8043 ~( arg )
void ScpA_Floor( Intp_t *scr ); // 8044 floor( float arg )
void ScpA_BinAnd( Intp_t *scr ); // 8040 &( p1, p2 )
void ScpA_BinOr( Intp_t *scr ); // 8041 |( p1, p2 )
void ScpA_BinXor( Intp_t *scr ); // 8042  = ^( a, b )
void ScpA_Swap( Intp_t *scr ); // 8019 <->( p1, p2 )
void ScpA_EndCritical( Intp_t *scr ); // 8003, 804B
void ScpA_StartCritical( Intp_t *scr ); // 8002, 804A O_CRITICAL_START
void ScpA_Jump( Intp_t *scr ); // 8004 jmp()
void ScpA_CallExt( Intp_t *scr ); // 8005 call( adres, arguments_cnt, )
void ScpA_801F( Intp_t *scr ); // 801F ?( arg1, arg2, arg3 )
void ScpA_RetA( Intp_t *scr ); // 801C
void ScpA_RetB( Intp_t *scr ); // 801D
void ScpA_8020( Intp_t *scr ); // 8020 O_POP_FLAGS_RETURN
void ScpA_8021( Intp_t *scr ); // 8021 O_POP_FLAGS_EXIT
void ScpA_8025( Intp_t *scr ); // 8025 O_POP_FLAGS_RETURN_VAL_EXIT
void ScpA_8026( Intp_t *scr ); // 8026 O_POP_FLAGS_RETURN_VAL_EXIT_EXTERN
void ScpA_8022( Intp_t *scr ); // 8022 O_POP_FLAGS_RETURN_EXTERN
void ScpA_8023( Intp_t *scr ); // 8023 O_POP_FLAGS_EXIT_EXTERN
void ScpA_8024( Intp_t *scr ); // 8024 O_POP_FLAGS_RETURN_VAL_EXTERN
void ScpA_801E( Intp_t *scr ); // 801E
void ScpA_800C( Intp_t *scr ); // 800C
void ScpA_800D( Intp_t *scr ); // 800D O_D_TO_A
void ScpA_8010( Intp_t *scr ); // 8010 O_EXIT_PROG
void ScpA_8011( Intp_t *scr ); // 8011
void ScpA_8012( Intp_t *scr ); // 8012
void ScpA_8013( Intp_t *scr ); // 8013
void ScpA_8018( Intp_t *scr ); // 8018
void ScpA_FetchAddr( Intp_t *scr ); // 802D
void ScpA_801A( Intp_t *scr ); // 801A O_POP
void ScpA_801B( Intp_t *scr ); // 801B
void ScpA_SetExportedVar( Intp_t *scr ); // 8015
void ScpA_GetExportedVar( Intp_t *scr ); // 8014
void ScpA_ExportProcedure( Intp_t *scr ); // 8017
void ScpA_ExportVariable( Intp_t *scr ); // 8016
void ScpA_KillProcedure( Intp_t *scr ); // 800E
void ScpA_Detach( Intp_t *scr ); // 800F
void ScpA_CallStart( Intp_t *scr ); // 8008 callstart()
void ScpA_Spawn( Intp_t *scr ); // 800A
void ScpA_Fork( Intp_t *scr ); // 800B fork()
void ScpA_8009( Intp_t *scr ); // 8009
void ScpA_8027( Intp_t *scr ); // 8027 ?( arg1, arg2 )
void ScpA_LookupStringProc( Intp_t *scr ); // 8028
void ScpAInitCommands();




