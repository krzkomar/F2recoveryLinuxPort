#pragma once

#include "FrameWork.h"

#define LIPMAGIC	0x5800

#define LIPS_NOTSTOP	0x01
#define LIPS_ANIMATE	0x02
#define LIPS_CLRMASK	~( LIPS_NOTSTOP | LIPS_ANIMATE )

typedef struct
{
    int Type;
    int Pos;
}  LipsMarker_t;

typedef struct 
{
    int 	Version;
    int 	Magic;
    int 	Status;
    Sound_t 	*Snd;
    int		Unk11;
    void 	*Unk12;
    char 	*Phonemes;
    int 	Unk14;
    int 	Position;
    int 	PhonemesCnt;
    int 	Unk16;
    int 	MarkersCnt;
    LipsMarker_t *Markers;
    int 	MarkerSize;
    int 	i15;
    int 	i16;
    int 	i17;
    int 	i18;
    int 	i19;
    int 	i20;
    char 	SpkId[8];
    char 	SndExt[4];
    char 	TextExt[4];
    char 	LipExt[4];
    char 	i26[ 260 ];
} Lips_t;

extern Lips_t gLipsync;
extern int gLipsNewPhonemeFlag;
extern int gLipsPhoneme;
/*
char gLipsUnk03;
int gLipsPosition;
unsigned int gLipsTime;
char gLipsFileDir[ 14 ];
char gLipsFnameBuf[ 50 ];
*/

/********************************************************************/

char *LipsyncGetFName( char *Name, int MaxSize );
void LipsyncSpeechUpdate();
int LipsyncSpeechStart();
int LipsyncRewind();
int LipsyncGetByte( char *fname, xFile_t *fh );
int LipsyncGet( LipsMarker_t *lp, xFile_t *fh );
int LipsyncLoadDataOldFmt( Lips_t *lp, xFile_t *fh );
int LipsyncLoadFile( char *a1, char *DirName );
int LipsyncMakeSpeech();
int LipsyncClose();


