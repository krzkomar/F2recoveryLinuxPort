#pragma once

//Audio_t *gAudioF;
//int gAudioFCnt;

int  AudiofGetFmt( int Id, int *pChannels, int *pFreq, int *pSamples );
int  AudiofExt( char *fname );
int  AudiofReadFile( FILE *file, void *Buffer, int Size );
int  AudiofOpen( char *Name, int mode );
void AudiofClose( int Id );
int  AudiofRead( int Id, void *buffer, int nsize );
int  AudiofSeek( int Id, int Offset, int whence );
int  AudiofLen( int Id );
int  AudiofTell( int Id );
int  AudiofWrite();
void AudiofInit( int (*Callback)(char *fname) );
void AudiofFree();






