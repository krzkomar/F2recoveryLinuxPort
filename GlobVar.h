#pragma once

extern int *gGVals;
extern int gGlobFloatMsg;
//int gGValCount;


int GlobVarGet( int GlobIdx );
int GlobVarSet( int Idx, int Val );
int GlobVarLoad();
int GlobVarLoadFile( char *fname, char *SecName, int *pCount, int **pData );
int GlobFloatMsgState();
int GlobUnk02(unsigned int );
int GlobUnk03();
int GlobVarFloatMsgDec();
int GlobVarFloatMsgInc();