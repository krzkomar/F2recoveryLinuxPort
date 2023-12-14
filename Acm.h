#pragma once

#define ACM_SIGNATURE 0x32897

typedef struct
{
    short R;
    short L;
} AcmAmp_t;

typedef struct
{
  int (*Cb)(void *arg, char *buf, int nsize );
  void *File;
  char *Buf1;
  int Buf1Size;
  char *pBitBlk;
  int BitBlkAvail;
  int BitBlock;
  int Bits;
  int RecursLvl;
  int Cols;
  int Rows;
  int OutAvail;
  int *WrapBuf;
  int *block;
  int StepSubCnt;
  int StepBytes;
  int nChannels;
  int Frequency;
  int ReadAvail;
  int *ReadPtr;
  int ReadCnt;
} Acm_t;


Acm_t *AcmOpen( int (*ReadCb)( void *file, void *data, int size), void *file, int *pnChannels, int *pFreq, int *pnSamples );
int    AcmInflate( Acm_t *acm, short *data, int nbytes );
void   AcmClose( Acm_t *acm );
int AcmUnk01( Acm_t *acm, int Column );