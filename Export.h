#pragma once

typedef struct 
{
    char Name[ 32 ];	// variable name
    union{
	char *Fname;	// file name
	Intp_t *Itp;	// interpreter script
    };
    uint16_t Flags;	// variable type -> !0 - interrupt context
    union{
	char *ValStr;	
	float ValFloat;
	int   ValInt;
	void  *ValPtr;
    };
} Export_t;

unsigned int ExportRandomEntry( char *Name );
Export_t *ExportSearchProc( char *Name );
Export_t *ExportGetEmptyProc( char *name );
Export_t *ExportSearchVar( char *Name );
int ExportGetVar( char *Name, short *pType, char **pStr );
Export_t *ExportUndefVar( char *Name );
int ExporSetFVar( char *Name, char *FloatStr );
int ExportSetVarArg( Intp_t *intp, char *Name, short Type, int Val );
int ExportSetVarArgP( Intp_t *intp, char *Name, void *Val );
int ExportSetVar( char *Name, short Type, char *Str );
int ExportGetVarArg( Intp_t *intp, char *Name, uint16_t *pFlags, int *pArg );
int ExportCreateIVar( char **a1, char *Name );
int ExportDeleteProc( Intp_t *Itp );
void ExportFlushProc();
void ExportFlushVars();
Intp_t *ExportGetProcedure( char *Name, int *pStr, short *IntCtx );
int ExportSetProcedure( Intp_t *itp, char *Name, void *Procedure, short Type );
char **ExportCreateListOfVariables( int *pNum );
void ExportFreeList( char **List, int cnt );
void ExportUndefVars();
void ExportVarsLoad ( xFile_t *fh );
void ExportVarsSave( xFile_t *fh );
