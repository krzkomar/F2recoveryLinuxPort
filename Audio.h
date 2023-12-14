#pragma once

#define AUDIO_RAW		0
#define AUDIO_COMPRESSED 	2

typedef struct // size of 28
{
  char Flags;
    union{
	 xFile_t *File;
	 FILE *rFile;
    };
  Acm_t *Acm;
  int Samples;
  int Freq;
  int Channels;
  int FilePos;
}  Audio_t;

int  AudioGetFmt( int Id, int *pChannels, int *pFreq, int *pSamples );
int  AudioFileExt( char *fname );
int  AudioReadBlk( xFile_t *fh, char *Data, int Count );
int  AudioOpen( char *Name, char mode );
void AudioClose( int Id );
int  AudioRead( int Id, void *buffer, unsigned int nsize );
int  AudioSeek( int Id, int offset, int whence );
int  AudioLen( int Id );
int  AudioTell( int fh );
int  AudioWrite();
void AudioInit( int ( *Callback )( char *) );
void AudioFree();







