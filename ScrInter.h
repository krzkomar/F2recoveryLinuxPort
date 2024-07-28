#pragma once

#define MAX_OPCODES  342

extern char *(*gSciMseHandler)();

//int gSciUnk04 = 10;
//int (*gScpOpcodeTable[ MAX_OPCODES ])( Intp_t * );
//int gSciUnk02;
//int gSciUnk01;
//int gSciUnk03;
/**************************************************/

void SciUnk01();
void SciUnk02( int a1 );
void SciOpcodeExec( Intp_t *Scp, int Steps );
void SciUnk03( Intp_t *scr, int Idx, int Arg );
void SciUnk04( Intp_t *scr, int Pos, int Arg );
void SciUnk05( Intp_t *Scr1, Intp_t *Scr2, char *Pos, int Arg );
void SciUnk06( Intp_t *scr1, Intp_t *scr2, char *Pos, int Arg );
void SciUnk07( Intp_t *a1, int a2, char *a3 );
void SciUnk08( Intp_t *scr, int a2, int *a3, char *a4, int a5 );
/* not used function 
void SciUnk09( Intp_t *scr, int a2, int a3, char *ebx0 );
void SciUnk10( Intp_t *scr, int a2, int a3, unsigned int ecx0 );
*/
void SciUnk11( Intp_t *scr, int a2 );
/*
    if found returns procedure index, else -1
*/
int SciLookupProcedure( Intp_t *itp, const char *ProcName );
void SciRunProcedure( Intp_t *itp, int ProcIdx );
void SciUnk14();
void SciUnk15( IntpList_t *a1 );
void SciItpEqAdd( Intp_t *itp );
void SciUnk17();
void SciUnk18( int val );
void SciUnk19();
void SciUnk20();
void SciAddOpcode( short Opcode, void (*Func)( Intp_t * ) );
void SciSetMseHandler( char *(*Cb)(void) );
void SciTimeUpdate();
void SciUnk25();
int SciUnk26();
//void SciUnk27( char *fpath, Intp_t *a2, int cnt );
//void SciUnk28();

