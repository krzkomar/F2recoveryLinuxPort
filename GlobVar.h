#pragma once

extern int *gGVals;
extern int gGlobUnk01;
//int gGValCount;


int GlobVarGet( int GlobIdx );
int GlobVarSet( int Idx, int Val );
int GlobVarLoad();
int GlobVarLoadFile( char *fname, char *SecName, int *pCount, int **pData );
int GlobUnk01();
int GlobUnk02(unsigned int );
int GlobUnk03();