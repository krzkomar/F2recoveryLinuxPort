#pragma once

typedef struct {
  int i01;	//
  int i02;	//
  int i03;	//
  int i04;	//
  int i05;	// 
  int i06;	// 
  int i07;	// 
} EndGame01_t;

typedef struct {
  int GVarId;	//0 4  gvar variable controlling slide
  int GVarVal;	//4 4  gvalue 
  int ArtNum;	//8 4  art number - line number in art/intrface/intrface.lst of the image to use for slide
  char Narrator[ 12 ];	//12 12  narrator file - base filename
  int PanDir;	//24 4
} EndGame02_t;


typedef struct 
{
    int GVarId;		// gvar id
    int GVarVal;	// gvar value 
    int AreaKnown;	// worldAreaKnown to get this one as an option
    int AreaNotKnown;	// worldAreaNotKnown to get this one as an option
    int MinLvl;		// minLevel to get this one as an option
    int OccurChance;	// % chance of a given line to occur
    char FileName[ 16 ];// narrator file
    int Dir;		// direction to pan the image -1 -> r2l,  1 -> l2r
} EndGame_t;

void EndGameRun();
void EndGameUnk01();
void EndGameUnk02();
int  EndGameUnk03();
void EndGameUnk04();
void EndGameUnk05( int a1, char *a2 );
void EndGameUnk06( int a1, char *a2 );
void EndGameUnk07( char *eax0 );
void EndGameUnk08();
void EndGameUnk09();
void EndGameUnk10( int eax0, int a2 );
void EndGameUnk11();
int  EndGameUnk12( char *fname );
void EndGameUnk13();
void EndGameUnk14();
void EndGameUnk15();
void EndGameUnk16();
int  EndGameUnk17();
void EndGameUnk18();
int  EndGameDeathEndingInit();
void EndGameClose();
void EndGameSetupDeathEnding( int a1 );
int  EndGameChance( int *a1 );
char *EndGameSetNarratorFile();

