#pragma once

#define RECORD_FRAMES_BUFFER_SIZE	4096

typedef struct
{
    int i00;
    int i01;
    int i02;
    int mx;
    int my;
    int mbutt;
} Rec_t;

extern Rec_t *gRec;
extern int gRecFrame;
extern char gRecState;
extern char gRecUnk01;
extern int gRecPlayUnk05;
extern int gRecUnk04;
extern int gRecPlayUnk04;
extern int gRecPlayUnk03;
extern unsigned int gRecTimer;
extern int gRecUnk02;
extern xFile_t *gRecFile;
extern int gRecUnk06;
extern int (*gRecPlayUnk02)(void);
extern int gRecPlayUnk01;
extern int gRecUnk07;
 
extern Rec_t gRecUnk08;

int RecRecInit( const char *fname );
int RecPlayInit( const char *fname, int a2, int (*a3)(void) );
void RecClr();
int RecGetState();
int RecRecord();
int RecAllocBuffer();
int RecFreeBuffer();
int RecReset();
int RecFlushOut();
int RecPlay();
int RecSaveFrame( Rec_t *Frame, xFile_t *fh );
int RecLoadFrame( Rec_t *Frame, xFile_t *fh );

