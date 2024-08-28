#include "FrameWork.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <SDL.h>

#define pINT( x )	((int *)(x))

int gMovAudioFrameStep;
int gMovSndCapabilities;
int gMovWidth;
int gMov_10;
int gMovFramesDropped;
int gMovSndCompressed;
MveUnk01_t gMovInputData;
MveChunkHdr_t gMovChunkHdr;
int gMovSnd16Bits;
int gMovBufOffset;
int gMovFrameNo;
int gMovHeight;
int gMovHeight01;
void *(*gMovAlloc)(int);
int (*gMovAfterFrameCb)(void);
int gMovPosX;
int gMovPosY;
int gMovAudioFrameSize;
xFile_t *gMovFileHandler;
int gMovChunkOffset;
void (*gMovFree)(void *);
int gMovSndPcmStereo;
MveChunk_t *gMovChunkData;
short gMovAudioFrameBufOffset[ 60 ];
int gMovTimeStep;
int gMovWidth01;
int gMovAudioMsk;
int gMovTimeStampUs;
int (*gMovReadFileCb)(xFile_t *, char *, int);
int gMovAudioFrameNo;
int gMovUnk101;
char gMovUnk102[512];
int gMovScrBufSizeB;
char *gMov_06;
int gMov_03;
int gMovUnk103;
int gMovSurfWidth;
int gMovScrBufSizeA;
int gMovLines;
int (*gMovCb_07)(int, int, int);
Pal8_t gMovPaletteA[ 256 ];
int gMovScrHeight;
int gMveScrWidth;
int *gMovScreen;
int gMov_08;
int gMovScrSize;
int gMovUnk02;
int gMovMovieXpos;
int gMovMovieYpos;
int gMovUnk20;
int gMov16BppMode;
int gMovScrBufSize;
int gMovSurfHeight;
unsigned char *gMovScrPixelsA;
unsigned char *gMovScrPixelsB;
int gMov_05;
int gMov_02[ 12272 ];
int gMovType = 0;
int gMovScaled = 4;

short gMovPcmTable[ 256 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
    42, 43, 47, 51, 56, 61, 66, 72, 79, 86, 94, 102, 112,
    122, 133, 145, 158, 173, 189, 206, 225, 245, 267, 292,
    318, 348, 379, 414, 452, 493, 538, 587, 640, 699, 763,
    832, 908, 991, 1081, 1180, 1288, 1405, 1534, 1673, 1826,
    1993, 2175, 2373, 2590, 2826, 3084, 3365, 3672, 4008,
    4373, 4772, 5208, 5683, 6202, 6767, 7385, 8059, 8794,
    9597, 10472, 11428, 12471, 13609, 14851, 16206, 17685,
    19298, 21060, 22981, 25078, 27367, 29864, 32589, -29973,
    -26728, -23186, -19322, -15105, -10503, -5481, -1, 1,
    1, 5481, 10503, 15105, 19322, 23186, 26728, 29973, -32589,
    -29864, -27367, -25078, -22981, -21060, -19298, -17685,
    -16206, -14851, -13609, -12471, -11428, -10472, -9597,
    -8794, -8059, -7385, -6767, -6202, -5683, -5208, -4772,
    -4373, -4008, -3672, -3365, -3084, -2826, -2590, -2373,
    -2175, -1993, -1826, -1673, -1534, -1405, -1288, -1180,
    -1081, -991, -908, -832, -763, -699, -640, -587, -538,
    -493, -452, -414, -379, -348, -318, -292, -267, -245,
    -225, -206, -189, -173, -158, -145, -133, -122, -112,
    -102, -94, -86, -79, -72, -66, -61, -56, -51, -47, -43,
    -42, -41, -40, -39, -38, -37, -36, -35, -34, -33, -32,
    -31, -30, -29, -28, -27, -26, -25, -24, -23, -22, -21,
    -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10,
    -9, -8, -7, -6, -5, -4, -3, -2, -1
};
int gMovVideoDrv = 0;
int gMovTimerEn = 0;
int gMovTimeUnk01 = 0;
int gMovUnk15 = 0;
Sound_t *gMovSoundDrv = NULL;
short *gMovSoundBuffer = NULL;
int gMovSoundBufferSize = 0; // added
double gMovSoundVol = 1.0;
int gMovAudioPlaying = 0;
int gMovSndPan = 0;
SDL_Surface *gMovSurfaceA = NULL;
SDL_Surface *gMovSurfaceB = NULL;
int (*gMovVideoCopy)( char *, int, int, int, int, int, int, int, int );
int gMovUnk13 = 1;
int gMovUnk12 = 0;
void (*gMovSetPalette)( Pal8_t *Palette, int PalStart, int PalCount );
int gMovSoundStopped = 0;
int gMovRun = 0;
int gMovMemProt = 0;
int gMovEncPosY[ 256 ] = { 0 };

const MveXYofs_t gMovEncXYOffstCur[ 256 ] = {
    { 0xF8, 0xF8 },{ 0xF9, 0xF8 },{ 0xFA, 0xF8 },{ 0xFB, 0xF8 },{ 0xFC, 0xF8 },{ 0xFD, 0xF8 },{ 0xFE, 0xF8 },{ 0xFF, 0xF8 },
    { 0x00, 0xF8 },{ 0x01, 0xF8 },{ 0x02, 0xF8 },{ 0x03, 0xF8 },{ 0x04, 0xF8 },{ 0x05, 0xF8 },{ 0x06, 0xF8 },{ 0x07, 0xF8 },
    { 0xF8, 0xF9 },{ 0xF9, 0xF9 },{ 0xFA, 0xF9 },{ 0xFB, 0xF9 },{ 0xFC, 0xF9 },{ 0xFD, 0xF9 },{ 0xFE, 0xF9 },{ 0xFF, 0xF9 },
    { 0x00, 0xF9 },{ 0x01, 0xF9 },{ 0x02, 0xF9 },{ 0x03, 0xF9 },{ 0x04, 0xF9 },{ 0x05, 0xF9 },{ 0x06, 0xF9 },{ 0x07, 0xF9 },
    { 0xF8, 0xFA },{ 0xF9, 0xFA },{ 0xFA, 0xFA },{ 0xFB, 0xFA },{ 0xFC, 0xFA },{ 0xFD, 0xFA },{ 0xFE, 0xFA },{ 0xFF, 0xFA },
    { 0x00, 0xFA },{ 0x01, 0xFA },{ 0x02, 0xFA },{ 0x03, 0xFA },{ 0x04, 0xFA },{ 0x05, 0xFA },{ 0x06, 0xFA },{ 0x07, 0xFA },
    { 0xF8, 0xFB },{ 0xF9, 0xFB },{ 0xFA, 0xFB },{ 0xFB, 0xFB },{ 0xFC, 0xFB },{ 0xFD, 0xFB },{ 0xFE, 0xFB },{ 0xFF, 0xFB },
    { 0x00, 0xFB },{ 0x01, 0xFB },{ 0x02, 0xFB },{ 0x03, 0xFB },{ 0x04, 0xFB },{ 0x05, 0xFB },{ 0x06, 0xFB },{ 0x07, 0xFB },
    { 0xF8, 0xFC },{ 0xF9, 0xFC },{ 0xFA, 0xFC },{ 0xFB, 0xFC },{ 0xFC, 0xFC },{ 0xFD, 0xFC },{ 0xFE, 0xFC },{ 0xFF, 0xFC },
    { 0x00, 0xFC },{ 0x01, 0xFC },{ 0x02, 0xFC },{ 0x03, 0xFC },{ 0x04, 0xFC },{ 0x05, 0xFC },{ 0x06, 0xFC },{ 0x07, 0xFC },
    { 0xF8, 0xFD },{ 0xF9, 0xFD },{ 0xFA, 0xFD },{ 0xFB, 0xFD },{ 0xFC, 0xFD },{ 0xFD, 0xFD },{ 0xFE, 0xFD },{ 0xFF, 0xFD },
    { 0x00, 0xFD },{ 0x01, 0xFD },{ 0x02, 0xFD },{ 0x03, 0xFD },{ 0x04, 0xFD },{ 0x05, 0xFD },{ 0x06, 0xFD },{ 0x07, 0xFD },
    { 0xF8, 0xFE },{ 0xF9, 0xFE },{ 0xFA, 0xFE },{ 0xFB, 0xFE },{ 0xFC, 0xFE },{ 0xFD, 0xFE },{ 0xFE, 0xFE },{ 0xFF, 0xFE },
    { 0x00, 0xFE },{ 0x01, 0xFE },{ 0x02, 0xFE },{ 0x03, 0xFE },{ 0x04, 0xFE },{ 0x05, 0xFE },{ 0x06, 0xFE },{ 0x07, 0xFE },
    { 0xF8, 0xFF },{ 0xF9, 0xFF },{ 0xFA, 0xFF },{ 0xFB, 0xFF },{ 0xFC, 0xFF },{ 0xFD, 0xFF },{ 0xFE, 0xFF },{ 0xFF, 0xFF },
    { 0x00, 0xFF },{ 0x01, 0xFF },{ 0x02, 0xFF },{ 0x03, 0xFF },{ 0x04, 0xFF },{ 0x05, 0xFF },{ 0x06, 0xFF },{ 0x07, 0xFF },
    { 0xF8, 0x00 },{ 0xF9, 0x00 },{ 0xFA, 0x00 },{ 0xFB, 0x00 },{ 0xFC, 0x00 },{ 0xFD, 0x00 },{ 0xFE, 0x00 },{ 0xFF, 0x00 },
    { 0x00, 0x00 },{ 0x01, 0x00 },{ 0x02, 0x00 },{ 0x03, 0x00 },{ 0x04, 0x00 },{ 0x05, 0x00 },{ 0x06, 0x00 },{ 0x07, 0x00 },
    { 0xF8, 0x01 },{ 0xF9, 0x01 },{ 0xFA, 0x01 },{ 0xFB, 0x01 },{ 0xFC, 0x01 },{ 0xFD, 0x01 },{ 0xFE, 0x01 },{ 0xFF, 0x01 },
    { 0x00, 0x01 },{ 0x01, 0x01 },{ 0x02, 0x01 },{ 0x03, 0x01 },{ 0x04, 0x01 },{ 0x05, 0x01 },{ 0x06, 0x01 },{ 0x07, 0x01 },
    { 0xF8, 0x02 },{ 0xF9, 0x02 },{ 0xFA, 0x02 },{ 0xFB, 0x02 },{ 0xFC, 0x02 },{ 0xFD, 0x02 },{ 0xFE, 0x02 },{ 0xFF, 0x02 },
    { 0x00, 0x02 },{ 0x01, 0x02 },{ 0x02, 0x02 },{ 0x03, 0x02 },{ 0x04, 0x02 },{ 0x05, 0x02 },{ 0x06, 0x02 },{ 0x07, 0x02 },
    { 0xF8, 0x03 },{ 0xF9, 0x03 },{ 0xFA, 0x03 },{ 0xFB, 0x03 },{ 0xFC, 0x03 },{ 0xFD, 0x03 },{ 0xFE, 0x03 },{ 0xFF, 0x03 },
    { 0x00, 0x03 },{ 0x01, 0x03 },{ 0x02, 0x03 },{ 0x03, 0x03 },{ 0x04, 0x03 },{ 0x05, 0x03 },{ 0x06, 0x03 },{ 0x07, 0x03 },
    { 0xF8, 0x04 },{ 0xF9, 0x04 },{ 0xFA, 0x04 },{ 0xFB, 0x04 },{ 0xFC, 0x04 },{ 0xFD, 0x04 },{ 0xFE, 0x04 },{ 0xFF, 0x04 },
    { 0x00, 0x04 },{ 0x01, 0x04 },{ 0x02, 0x04 },{ 0x03, 0x04 },{ 0x04, 0x04 },{ 0x05, 0x04 },{ 0x06, 0x04 },{ 0x07, 0x04 },
    { 0xF8, 0x05 },{ 0xF9, 0x05 },{ 0xFA, 0x05 },{ 0xFB, 0x05 },{ 0xFC, 0x05 },{ 0xFD, 0x05 },{ 0xFE, 0x05 },{ 0xFF, 0x05 },
    { 0x00, 0x05 },{ 0x01, 0x05 },{ 0x02, 0x05 },{ 0x03, 0x05 },{ 0x04, 0x05 },{ 0x05, 0x05 },{ 0x06, 0x05 },{ 0x07, 0x05 },
    { 0xF8, 0x06 },{ 0xF9, 0x06 },{ 0xFA, 0x06 },{ 0xFB, 0x06 },{ 0xFC, 0x06 },{ 0xFD, 0x06 },{ 0xFE, 0x06 },{ 0xFF, 0x06 },
    { 0x00, 0x06 },{ 0x01, 0x06 },{ 0x02, 0x06 },{ 0x03, 0x06 },{ 0x04, 0x06 },{ 0x05, 0x06 },{ 0x06, 0x06 },{ 0x07, 0x06 },
    { 0xF8, 0x07 },{ 0xF9, 0x07 },{ 0xFA, 0x07 },{ 0xFB, 0x07 },{ 0xFC, 0x07 },{ 0xFD, 0x07 },{ 0xFE, 0x07 },{ 0xFF, 0x07 },
    { 0x00, 0x07 },{ 0x01, 0x07 },{ 0x02, 0x07 },{ 0x03, 0x07 },{ 0x04, 0x07 },{ 0x05, 0x07 },{ 0x06, 0x07 },{ 0x07, 0x07 },
};

const MveXYofs_t gMovEncXYoffst[ 256 ] = {
    { 0x08, 0x00 },{ 0x09, 0x00 },{ 0x0A, 0x00 },{ 0x0B, 0x00 },{ 0x0C, 0x00 },{ 0x0D, 0x00 },{ 0x0E, 0x00 },{ 0x08, 0x01 },
    { 0x09, 0x01 },{ 0x0A, 0x01 },{ 0x0B, 0x01 },{ 0x0C, 0x01 },{ 0x0D, 0x01 },{ 0x0E, 0x01 },{ 0x08, 0x02 },{ 0x09, 0x02 },
    { 0x0A, 0x02 },{ 0x0B, 0x02 },{ 0x0C, 0x02 },{ 0x0D, 0x02 },{ 0x0E, 0x02 },{ 0x08, 0x03 },{ 0x09, 0x03 },{ 0x0A, 0x03 },
    { 0x0B, 0x03 },{ 0x0C, 0x03 },{ 0x0D, 0x03 },{ 0x0E, 0x03 },{ 0x08, 0x04 },{ 0x09, 0x04 },{ 0x0A, 0x04 },{ 0x0B, 0x04 },
    { 0x0C, 0x04 },{ 0x0D, 0x04 },{ 0x0E, 0x04 },{ 0x08, 0x05 },{ 0x09, 0x05 },{ 0x0A, 0x05 },{ 0x0B, 0x05 },{ 0x0C, 0x05 },
    { 0x0D, 0x05 },{ 0x0E, 0x05 },{ 0x08, 0x06 },{ 0x09, 0x06 },{ 0x0A, 0x06 },{ 0x0B, 0x06 },{ 0x0C, 0x06 },{ 0x0D, 0x06 },
    { 0x0E, 0x06 },{ 0x08, 0x07 },{ 0x09, 0x07 },{ 0x0A, 0x07 },{ 0x0B, 0x07 },{ 0x0C, 0x07 },{ 0x0D, 0x07 },{ 0x0E, 0x07 },
    { 0xF2, 0x08 },{ 0xF3, 0x08 },{ 0xF4, 0x08 },{ 0xF5, 0x08 },{ 0xF6, 0x08 },{ 0xF7, 0x08 },{ 0xF8, 0x08 },{ 0xF9, 0x08 },
    { 0xFA, 0x08 },{ 0xFB, 0x08 },{ 0xFC, 0x08 },{ 0xFD, 0x08 },{ 0xFE, 0x08 },{ 0xFF, 0x08 },{ 0x00, 0x08 },{ 0x01, 0x08 },
    { 0x02, 0x08 },{ 0x03, 0x08 },{ 0x04, 0x08 },{ 0x05, 0x08 },{ 0x06, 0x08 },{ 0x07, 0x08 },{ 0x08, 0x08 },{ 0x09, 0x08 },
    { 0x0A, 0x08 },{ 0x0B, 0x08 },{ 0x0C, 0x08 },{ 0x0D, 0x08 },{ 0x0E, 0x08 },{ 0xF2, 0x09 },{ 0xF3, 0x09 },{ 0xF4, 0x09 },
    { 0xF5, 0x09 },{ 0xF6, 0x09 },{ 0xF7, 0x09 },{ 0xF8, 0x09 },{ 0xF9, 0x09 },{ 0xFA, 0x09 },{ 0xFB, 0x09 },{ 0xFC, 0x09 },
    { 0xFD, 0x09 },{ 0xFE, 0x09 },{ 0xFF, 0x09 },{ 0x00, 0x09 },{ 0x01, 0x09 },{ 0x02, 0x09 },{ 0x03, 0x09 },{ 0x04, 0x09 },
    { 0x05, 0x09 },{ 0x06, 0x09 },{ 0x07, 0x09 },{ 0x08, 0x09 },{ 0x09, 0x09 },{ 0x0A, 0x09 },{ 0x0B, 0x09 },{ 0x0C, 0x09 },
    { 0x0D, 0x09 },{ 0x0E, 0x09 },{ 0xF2, 0x0A },{ 0xF3, 0x0A },{ 0xF4, 0x0A },{ 0xF5, 0x0A },{ 0xF6, 0x0A },{ 0xF7, 0x0A },
    { 0xF8, 0x0A },{ 0xF9, 0x0A },{ 0xFA, 0x0A },{ 0xFB, 0x0A },{ 0xFC, 0x0A },{ 0xFD, 0x0A },{ 0xFE, 0x0A },{ 0xFF, 0x0A },
    { 0x00, 0x0A },{ 0x01, 0x0A },{ 0x02, 0x0A },{ 0x03, 0x0A },{ 0x04, 0x0A },{ 0x05, 0x0A },{ 0x06, 0x0A },{ 0x07, 0x0A },
    { 0x08, 0x0A },{ 0x09, 0x0A },{ 0x0A, 0x0A },{ 0x0B, 0x0A },{ 0x0C, 0x0A },{ 0x0D, 0x0A },{ 0x0E, 0x0A },{ 0xF2, 0x0B },
    { 0xF3, 0x0B },{ 0xF4, 0x0B },{ 0xF5, 0x0B },{ 0xF6, 0x0B },{ 0xF7, 0x0B },{ 0xF8, 0x0B },{ 0xF9, 0x0B },{ 0xFA, 0x0B },
    { 0xFB, 0x0B },{ 0xFC, 0x0B },{ 0xFD, 0x0B },{ 0xFE, 0x0B },{ 0xFF, 0x0B },{ 0x00, 0x0B },{ 0x01, 0x0B },{ 0x02, 0x0B },
    { 0x03, 0x0B },{ 0x04, 0x0B },{ 0x05, 0x0B },{ 0x06, 0x0B },{ 0x07, 0x0B },{ 0x08, 0x0B },{ 0x09, 0x0B },{ 0x0A, 0x0B },
    { 0x0B, 0x0B },{ 0x0C, 0x0B },{ 0x0D, 0x0B },{ 0x0E, 0x0B },{ 0xF2, 0x0C },{ 0xF3, 0x0C },{ 0xF4, 0x0C },{ 0xF5, 0x0C },
    { 0xF6, 0x0C },{ 0xF7, 0x0C },{ 0xF8, 0x0C },{ 0xF9, 0x0C },{ 0xFA, 0x0C },{ 0xFB, 0x0C },{ 0xFC, 0x0C },{ 0xFD, 0x0C },
    { 0xFE, 0x0C },{ 0xFF, 0x0C },{ 0x00, 0x0C },{ 0x01, 0x0C },{ 0x02, 0x0C },{ 0x03, 0x0C },{ 0x04, 0x0C },{ 0x05, 0x0C },
    { 0x06, 0x0C },{ 0x07, 0x0C },{ 0x08, 0x0C },{ 0x09, 0x0C },{ 0x0A, 0x0C },{ 0x0B, 0x0C },{ 0x0C, 0x0C },{ 0x0D, 0x0C },
    { 0x0E, 0x0C },{ 0xF2, 0x0D },{ 0xF3, 0x0D },{ 0xF4, 0x0D },{ 0xF5, 0x0D },{ 0xF6, 0x0D },{ 0xF7, 0x0D },{ 0xF8, 0x0D },
    { 0xF9, 0x0D },{ 0xFA, 0x0D },{ 0xFB, 0x0D },{ 0xFC, 0x0D },{ 0xFD, 0x0D },{ 0xFE, 0x0D },{ 0xFF, 0x0D },{ 0x00, 0x0D },
    { 0x01, 0x0D },{ 0x02, 0x0D },{ 0x03, 0x0D },{ 0x04, 0x0D },{ 0x05, 0x0D },{ 0x06, 0x0D },{ 0x07, 0x0D },{ 0x08, 0x0D },
    { 0x09, 0x0D },{ 0x0A, 0x0D },{ 0x0B, 0x0D },{ 0x0C, 0x0D },{ 0x0D, 0x0D },{ 0x0E, 0x0D },{ 0xF2, 0x0E },{ 0xF3, 0x0E },
    { 0xF4, 0x0E },{ 0xF5, 0x0E },{ 0xF6, 0x0E },{ 0xF7, 0x0E },{ 0xF8, 0x0E },{ 0xF9, 0x0E },{ 0xFA, 0x0E },{ 0xFB, 0x0E },
    { 0xFC, 0x0E },{ 0xFD, 0x0E },{ 0xFE, 0x0E },{ 0xFF, 0x0E },{ 0x00, 0x0E },{ 0x01, 0x0E },{ 0x02, 0x0E },{ 0x03, 0x0E },
    { 0x04, 0x0E },{ 0x05, 0x0E },{ 0x06, 0x0E },{ 0x07, 0x0E },{ 0x08, 0x0E },{ 0x09, 0x0E },{ 0x0A, 0x0E },{ 0x0B, 0x0E }
};

void MovVideoCopy( unsigned char *pSrc, unsigned int SrcW, int SrcH, unsigned int SrcX, int SrcY, unsigned int DstW, unsigned int DstH, int DstX, int DstY, int nnn );

/****************************************************************************/

void MovSetupMemory( void *(*Alloc)(int), void ( *Free)(void *))
{
    gMovAlloc = Alloc;
    gMovFree = Free;
}

void MovSetInput( int (*Cb)(xFile_t *, char *, int ))
{
    gMovReadFileCb = Cb;
}

void MovSetP( MveUnk01_t *p, int size, void *ptr )
{
    if( !ptr ) return;
    MovFreeP( p );
    p->data = (MveHdr_t *)ptr;
    p->i00 = size;
    p->i01 = 0;
}

void MovFreeP( MveUnk01_t *p )
{
    if( p->i01 && gMovFree ){
        gMovFree( p->data );
        p->i01 = 0;
    }
    p->i00 = 0;
}

void MovSetSoundDrv( void *sndDrv )
{
//    gMovSoundDrv = sndDrv;
}

void MovSetSndVolume( double Vol )
{
    gMovSoundVol = (10000.0 + Vol)/10000.0;
    SoundSetVolume( 3, gMovSoundVol );
}

void MovSetAspect( int Width, int Height, int Lines, int a4, int *Screen, unsigned int a6, unsigned int a7, int (*Cb)(int, int, int), int a9 )
{
    gMovWidth = Width;
    gMovHeight = Height;
    gMovWidth01 = Width;
    gMovHeight01 = Height;
    gMovLines = Lines;
    if( gMovType & 0x04 ) gMovLines = 2 * Lines;
    gMov_02[ 0 ] = a4;
    gMov_03 = a6;
    gMovScreen = Screen;
    gMov_05 = a7;
    gMov_06 = (char *)Screen + a6;
    gMovCb_07 = Cb;
    gMov_08 = ( a7 ) ? (a6 / a7) : 1;
    gMovUnk13 = 0;
    gMov_10 = a9;
}

void MovSetupBlitter( int (*Cb)())
{
    gMovVideoCopy = Cb;
}

void MovNullHandlerCb()
{
    /* Empty */
}

void MovSetupPalette( void (*pal)(Pal8_t *, int first, int cnt) )
{
    gMovSetPalette = pal;
}

int MveNullSub1()
{
    return 0;
}

void MovSetupVideo( int videoDrv )
{
    gMovVideoDrv = videoDrv;
}

void MovSetupAfterFrameCb( int (*cb)(void) )
{
    gMovAfterFrameCb = cb;
}

void MovSetType( int Type )
{
    gMovType = Type;
}

void MovSetScale( int a1 )
{
    gMovScaled = ( a1 == 3 ) ? 3 : 4;
}

void MovGetFrameNo( int *Frame, int *Dropped )
{
    *Frame = gMovFrameNo;
    *Dropped = gMovFramesDropped;
}

int MovPrep( xFile_t *fh, int PosX, int PosY, char AudioBitNo )
{
    MovProtectMemArea();
    if( !gMovVideoDrv ) return -11;
    gMovPosX = PosX;
    gMovPosY = PosY;
    gMovAudioMsk = 1 << AudioBitNo;
    if( !gMovAudioMsk ) gMovAudioMsk = 1;
    if( !MovLoadHdr( fh ) ){ MovStop(); return -8; }
    gMovChunkData = (MveChunk_t *)MovLoadChunk();
    gMovChunkOffset = 0;
    if( !gMovChunkData ){ MovStop(); return -2; }
    gMovRun = 1;
    gMovSoundStopped = 0;
    gMovFrameNo = 0;
    gMovFramesDropped = 0;
    return 0;
}

int MovLoadHdr( xFile_t *fh ) // load main header and first chunk header
{
    MveHdr_t *hdr;

    gMovFileHandler = fh;    
    if( !(hdr = (MveHdr_t *)MovLoadData( 30 )) ) return 0;
    if( memcmp( hdr, "Interplay MVE File\x1a", 20 ) ) return 0;
    if( ((~hdr->w1 - hdr->w2) != -0x1234) || (hdr->w1 != 0x100) || (hdr->w0 != 0x1A) ) return 0;
    gMovChunkHdr = hdr->Chunk;
    return 1;
}

char *MovLoadData( unsigned int size )
{
    char *p;

    p = MovReAlloc( &gMovInputData, size );
    if( !p ) return NULL;
    return !gMovReadFileCb( gMovFileHandler, p, size ) ? NULL : p;
}

char *MovReAlloc( MveUnk01_t *p, unsigned int size )
{
    char *q;

    if( p->i00 >= size ) return p->data->Magic;
    if( !gMovAlloc ) return 0;
    MovFreeP( p );    
    if( !(q = gMovAlloc( size + 100 )) ) return 0;
    MovSetP( p, size + 100, q );
    p->i01 = 1;
    return p->data->Magic;
}

char *MovLoadChunk()
{
    char *ChunkData;

    ChunkData = MovLoadData( gMovChunkHdr.Size + 4 ); // 4 -> size of next chunk header
    if( !ChunkData ) return NULL;
    gMovChunkHdr = *(MveChunkHdr_t *)( &ChunkData[ gMovChunkHdr.Size ] ); // set next chunk header
    return ChunkData;
}

void MovProtectMemArea()
{
    if( gMovMemProt ) return;
    gMovMemProt = 1;
}

void MovSndMute()
{
    if( !gMovSoundStopped ){
        MovSndPause();
        gMovSoundStopped = 1;
    }
    MovTimeUpdate();
}

int MovTimeUpdate()
{
    if( !gMovTimerEn ) return 0;
    while( ( gMovTimeStampUs + 1000 * TimerGetSysTime() ) < 0 );
    gMovTimeStampUs += gMovTimeStep;
    return 1;
}

void MovSndPause()
{
//    if( gMovSoundBuffer ) gMovSoundBuffer->lpVtbl->Stop(gMovSoundBuffer);
}

#define MOV_ERROR( err ) { MovStop(); return err; }
#define TT(a,n)	printf( "Code[%2i]: \""a"\"\n", n );
//#define TT(a,n)	

int MovStep()
{
    MveChunk_t *Chunk;
    MveOpcode_t OpcodeHdr;
    unsigned short ChunkOffset;
    unsigned int tmp1, TrueColor,Count,tmp2, flags;
    unsigned char *SndData, *SkipMap, Version, *Map, opcode;

    ChunkOffset = gMovChunkOffset;
    Chunk = gMovChunkData;
    if( !gMovRun ) return -10;
    if( gMovSoundStopped ) gMovSoundStopped = 0;    
    while( 1 ){
        SkipMap = NULL;
        Map = NULL;
        if( !Chunk ) MOV_ERROR( -2 )
	while( 1 ){
    	    Chunk = (MveChunk_t *)((char *)Chunk + ChunkOffset);
    	    OpcodeHdr = Chunk->Opcode; //!
    	    Chunk = (MveChunk_t *)((char *)Chunk + 4);
    	    ChunkOffset = OpcodeHdr.Offset; // !!
    	    Version = OpcodeHdr.Version;
    	    opcode = OpcodeHdr.Opcode;
    	    if( opcode == 1 ){ // End of Chunk - terminate a chunk and fetch and decode next chunk
    		TT( "End of chunk", OpcodeHdr.Opcode )
        	ChunkOffset = 0;
        	Chunk = (MveChunk_t *)MovLoadChunk();
        	break;        
    	    }
    	    switch( opcode ){
        	case 0: TT( "*** End of stream ***", OpcodeHdr.Opcode ) // End of Stream - 
        	    return -1;
        	case 2: // Create Timer - time frame lap
        	    TT( "Create Timer", OpcodeHdr.Opcode )
            	    if( !MovCreateTimer( Chunk->Tim.Rate, Chunk->Tim.SubDivision ) ) MOV_ERROR( MOV_ERR_TIMER )
            	    break;                
        	case 3: // Initialize Audio Buffers
        	    TT( "Init Audio buffers", OpcodeHdr.Opcode )            	    
            	    if( !MovSndSetup( 
            		Chunk->Snd.W0, 			// ?
            		Chunk->Snd.MinBufferLength, 	// minimal buffer length in bytes for output buffer to allocate
            		Chunk->Snd.Flags & 0x01, 	// 0 - mono, 1 - stereo
            		Chunk->Snd.SampleRate, 		// frequency in Hz
            		(Chunk->Snd.Flags & 0x02) >> 1, // 0 - 8bit, 1 - 16bit
            		( OpcodeHdr.Version ) ? ((Chunk->Snd.Flags & 0x04) >> 2) : 0 ) // 0 - uncompressed, 1 - compressed
            	    ) MOV_ERROR( MOV_ERR_SND )
            	    break;                
        	case 4: // Start/Stop audio
        	    TT( "Start/Stop audio", OpcodeHdr.Opcode )
            	    MovSndStartStop();
            	    break;
        	case 5: // Initialize Video Buffers
        	    TT( "Initialize video buffers", OpcodeHdr.Opcode )
            	    TrueColor = ( OpcodeHdr.Version >= 2 ) ? Chunk->Vid.TrueColor : 0;
            	    Count = 1;
            	    if( Version ) Count = Chunk->Vid.Count;
            	    if( !MovVideoBufferInit( Chunk->Vid.Width, Chunk->Vid.Height, Count, TrueColor ) ) MOV_ERROR( MOV_ERR_VID )
            	    tmp1 = ( 4 * gMovSurfWidth / gMovScaled ) & 0xFFFFFFF0;
            	    if( gMov16BppMode ) tmp1 /= 2;
            	    tmp2 = ( gMovPosX > 0 ) ? gMovPosX : 0;
            	    if( tmp1 + tmp2 > gMovWidth ) MOV_ERROR( MOV_ERR_RES ) // horizontal out of the screen
            	    tmp2 = ( gMovPosY > 0 ) ? gMovPosY : 0;
            	    if( ( gMovSurfHeight + tmp2 ) > gMovHeight ) MOV_ERROR( MOV_ERR_RES ) // vertical out of the screen
            	    if( gMov16BppMode && !gMov_10 ) MOV_ERROR( MOV_ERR_RES )
            	    break;
        	case 6: // Opcode format 0x06 video stream
        	    TT( "Unknown", OpcodeHdr.Opcode )
            	    if( Chunk->Op5.w6 & 0x01 ) MovSwapSurfaces();
            	    if( !MovFlipScreen() ) MOV_ERROR( MOV_ERR_VIDIO )
//            	    MovDecodeOldFrame( &Chunk->Op5.w7, Chunk->Op5.w2, Chunk->Op5.FlipBack, Chunk->Op5.w4, Chunk->Op5.w5 );
            	    MovUnlockSurfaces();
            	    break;
        	case 7: // Send Buffer to Display
        	    TT( "Send buffer to display", OpcodeHdr.Opcode )
            	    flags = 0;
            	    gMovFrameNo++;
            	    if( OpcodeHdr.Version ) flags = Chunk->Pal.Flags;
            	    if( !Chunk->Pal.PalCount || SkipMap || gMov_10 )
                	MovSetPalette8A( Chunk->Pal.PalStart, Chunk->Pal.PalCount );
            	    else
                	MovSetPalette8B( Chunk->Pal.PalStart, Chunk->Pal.PalCount );
            	    if( !gMovTimeUnk01 || Chunk->Pal.PalCount ){
                	MovVideoUpdate( gMovPosX, gMovPosY, flags );
            	    } else {
                	gMovUnk15 = 1;
                	gMovFramesDropped++;
        	    }
            	    if( Chunk->Pal.PalCount && !SkipMap && !gMov_10 ) MovSetPalette8A( Chunk->Pal.PalStart, Chunk->Pal.PalCount );
            	    gMovChunkData = Chunk;
            	    gMovChunkOffset = ChunkOffset;
            	    return 0;
        	case 8: // Audio Frame - Data
        	case 9: // Audio Frame - Silence
        	    TT( "Audio frame", OpcodeHdr.Opcode );
            	    if( Chunk->Audio.StrmMsk & gMovAudioMsk ){ // language selection
                	SndData = (unsigned char *)Chunk->Audio.Data;
                	if( opcode == 9 ) SndData = NULL; // Silence
                	MovAudioFrame( (char *)SndData, Chunk->Audio.StrmLen );
            	    }
            	    break;
        	case 10: // Initialize Video Mode
        	    TT( "Initialize video mode", OpcodeHdr.Opcode )
            	    if( !gMovUnk13 ) break;
            	    tmp2 = Chunk->VidMode.Flags;
            	    if( gMovType && !(gMovType & 0x04) ) tmp2 |= 0x8000u;
            	    if( tmp2 != gMovUnk12 && !MveNullSub1() ) MOV_ERROR( MOV_ERR_GMODE )
            	    gMovUnk13 = 1;
            	    gMovUnk12 = tmp2;
            	    break;
        	case 11: // Create Gradient
        	    TT( "Create gradient", OpcodeHdr.Opcode )
            	    MovCreateGradient( Chunk->Grad.RBbase, Chunk->Grad.RB_Rnum, Chunk->Grad.RGbase, Chunk->Grad.RG_Rnum, Chunk->Grad.RG_Gnum, Chunk->Grad.Data );
            	    break;
        	case 12: // Set Palette
        	    TT( "Set palette", OpcodeHdr.Opcode )
            	    MovSetPalette( &Chunk->Palette.PalData, Chunk->Pal.PalStart, Chunk->Pal.PalCount );
            	    break;
        	case 13: // Set Palette Entries Compressed
        	    TT( "Set palette entries compressed", OpcodeHdr.Opcode )
            	    MovSetPaletteCompressedP((char *)Chunk);
            	    break;
        	case 14: // Skip Map
        	    TT( "Unknown", OpcodeHdr.Opcode )
            	    SkipMap = (unsigned char *)Chunk;
            	    break;
        	case 15: // Set Decoding Map
        	    TT( "Set coding map", OpcodeHdr.Opcode )
            	    Map = (unsigned char *)Chunk;
            	    break;
        	case 16: // Storing Video Data (old format )
        	    TT( "Unknown", OpcodeHdr.Opcode )
            	    if( Chunk->Run.w4 & 0x01 ) MovSwapSurfaces();
            	    if( !MovFlipScreen() ) MOV_ERROR( MOV_ERR_VIDIO )
            	    MovDecodeFrame10( (short *)SkipMap, (short *)Map, (char *)&Chunk->Run.data, Chunk->Run.w0, Chunk->Run.w1, Chunk->Run.w2, Chunk->Run.w3 );
            	    MovUnlockSurfaces();
            	    break;
        	case 17: // Video Data ( new format )
        	    TT( "Video data", OpcodeHdr.Opcode )
            	    if( OpcodeHdr.Version < 3 ) MOV_ERROR( MOV_ERR_FILE )
            	    if( Chunk->Run.w4 & 0x01 ) MovSwapSurfaces();
            	    if( gMov16BppMode ){
                	if( gMovType ) MOV_ERROR( MOV_ERR_FILE ) // Invalid movie file
                	if( !MovFlipScreen() ) MOV_ERROR( MOV_ERR_VIDIO )
                	MovDecodeFrame16bpp( (char *)Map, (char *)&Chunk->Run.data, Chunk->Run.w0, Chunk->Run.w1, Chunk->Run.w2, Chunk->Run.w3 );
                	MovUnlockSurfaces();
                	break;
            	    }
            	    if( !MovFlipScreen() ) MOV_ERROR( MOV_ERR_VIDIO )
            	    MovDecodeFrame8bpp( Map, (unsigned char *)&Chunk->Run.data, Chunk->Run.w0, Chunk->Run.w1, Chunk->Run.w2, Chunk->Run.w3 );            	    
            	    MovUnlockSurfaces();
            	    break;
        	default: TT( "Unknown", OpcodeHdr.Opcode ) break;
    	    }
        }
    }
}

int MovCreateTimer( int Rate, unsigned int Division )
{
    int t;

    t = -((Division >> 1) + Rate * Division);
    if( gMovTimerEn && (gMovTimeStep == t) ) return 1;
    MovTimeUpdate();
    gMovTimeStep = -((Division >> 1) + Rate * Division );
    MovTimeStamp( t );
    return 1;
}

int MovTimeStamp( int time )
{
    gMovTimerEn = 1;
    gMovTimeStampUs = -1000 * TimerGetSysTime() + time;
    return gMovTimeStampUs;
}

SoundStr_t *sstr;

int MovSndSetup( short a1, unsigned int BuffSize, short Stereo, short Freq, unsigned short Bits16, int Compressed )
{
    int tmp;
    
//    if( !gMovSoundDrv ) return 1;
    gMovSoundDrv = SoundCreate( 0, 0 );
    MovWait();
    gMovAudioPlaying = 0;
    MovSoundStop();
    gMovSndPcmStereo = Stereo;
    gMovSnd16Bits = Bits16;
    gMovSndCompressed = Compressed;
    gMovSoundDrv->Freq = Freq;
    gMovSoundDrv->Channels = 2 - ( Stereo == 0 );
    tmp = (BuffSize + (BuffSize / 2)) & ~0x03;
    if( gMovSoundBufferSize < tmp ){
	gMovSoundBuffer = dbg_realloc( gMovSoundBuffer, tmp );
	gMovSoundBufferSize = tmp;
    }    
    memset( gMovSoundBuffer, 0 , gMovSoundBufferSize );
//    gMovSoundStopped = 0;
    gMovAudioFrameNo = 0;
    gMovAudioFrameStep = 0;
    gMovBufOffset = 0;
    sstr = SoundStrCreate( Freq, Stereo, Bits16 );
    return 1;
}

void MovWait()
{
    if( !gMovTimerEn ) return;
    while( (gMovTimeStampUs + 1000 * TimerGetSysTime()) < 0 );
}

void MovSoundStop()
{
    if( !gMovSoundBuffer ) return;
    SoundStrPlay( sstr, 0 );
    SoundDestruct( gMovSoundDrv, 0 );
    gMovSoundDrv = NULL;
    dbg_free( gMovSoundBuffer );
    gMovSoundBuffer = NULL;
    gMovSoundBufferSize = 0;
    gMovAudioPlaying = 0;
    SoundStrClose( sstr );
}

void MovSndStartStop()
{
    gMovTimeUnk01 = MovTimerWaitMs(gMovTimeStep >> 2) > -gMovTimeStep >> 1 && !gMovUnk15;
    if( gMovAudioPlaying == 0 ){
	SoundStrPlay( sstr, 1 );
	gMovAudioPlaying = 1;
    }
}

int MovTimerWaitMs( int time )
{
    int result, k;

    if( !gMovTimerEn ) return 0;
    k = gMovTimeStampUs + time;
    do
        result = k + 1000 * TimerGetSysTime();
    while( result < 0 );
    gMovTimeStampUs += gMovTimeStep;
    return result;
}

void MovAudioFrame( char *SndData, short Size )
{
    gMovAudioFrameSize = Size;
    if( !gMovSoundBuffer ) return;    
    MovAudioDecode( gMovSoundBuffer, SndData, Size );
    gMovBufOffset += Size;
    SoundStrQueue( sstr, gMovSoundBuffer, Size );    
}

void MovAudioDecode( short *pOutBuffer, char *pRaw, unsigned int Size )
{
    if( pRaw == NULL ){
        memset( pOutBuffer, gMovSnd16Bits == 0 ? 0x80 : 0, Size );
        return;
    }
    if( !gMovSndCompressed ){
        memcpy( pOutBuffer, pRaw, Size );
	return;
    }
    if( gMovSndPcmStereo )
	MovPCMdecodeStereo( pOutBuffer, (unsigned char *)pRaw + 4, Size, (short *)pRaw );
    else 
	MovPCMdecodeMono( pOutBuffer, (unsigned char *)pRaw + 2, Size, (short *)pRaw );
}

int MovVideoBufferInit( short Width, short Height, int Size, int Mode16bpp )
{
    if( gMovSurfaceA ){
	SDL_FreeSurface( gMovSurfaceA );
        gMovSurfaceA = NULL;
    }
    if( gMovSurfaceB ){
	SDL_FreeSurface( gMovSurfaceB );
        gMovSurfaceB = NULL;
    }
    gMveScrWidth = Width;
    gMovScrHeight = Height;
    gMovScrSize = Size;
    gMovSurfWidth = 8 * Width;
    gMovSurfHeight = 8 * Height * Size;
    if( gMovType ) gMovSurfHeight /= 2;
//    if( Mode16bpp ){} else { }
    gMovSurfaceA = SDL_CreateRGBSurface( 0, gMovSurfWidth, gMovSurfHeight, 8, 0, 0, 0, 0 );
    gMovSurfaceB = SDL_CreateRGBSurface( 0, gMovSurfWidth, gMovSurfHeight, 8, 0, 0, 0, 0 );


    gMov16BppMode = Mode16bpp;
    gMovScrBufSize = Size * gMovSurfWidth - 8;
    if( gMov16BppMode ){
        gMovSurfWidth *= 2;
        gMovScrBufSize *= 2;
    }
    gMovScrBufSizeA = 8 * Size * gMovSurfWidth;
    gMovScrBufSizeB = 7 * Size * gMovSurfWidth;
    MovEncPitchInit();
    return 1;
}

int MovFlipScreen()
{
    if( !gMovSurfaceA || !gMovSurfaceB ) return 0;
    SDL_LockSurface( gMovSurfaceA );
    gMovScrPixelsA = gMovSurfaceA->pixels;
    gMovScrPixelsB = gMovSurfaceB->pixels;
    SDL_LockSurface( gMovSurfaceA );
    return 1;    
}

void MovUnlockSurfaces()
{
    SDL_UnlockSurface( gMovSurfaceA );
    SDL_UnlockSurface( gMovSurfaceB );
}

void MovSwapSurfaces()
{
    SDL_Surface *tmp;

    tmp = gMovSurfaceB;
    gMovSurfaceB = gMovSurfaceA;
    gMovSurfaceA = tmp;
}

void MovVideoUpdate( int Xpos, int Ypos, short flags )
{
    unsigned int ofst;

    ofst = (((gMovSurfWidth / gMovScaled - 12) *4) & ~0x0f) + 12;
    gMovUnk103 = gMovSurfWidth - gMovScaled * (ofst / 4);
    if( Xpos < 0 ) Xpos = (gMovWidth - (gMov16BppMode ? (ofst / 2) : ofst )) / 2;
    if( gMov16BppMode ) Xpos *= 2;
    Xpos &= ~0x03;
    if( Ypos < 0 ) Ypos = (gMovHeight - (( gMovType & 0x04 ) ? (2 * gMovSurfHeight) : gMovSurfHeight)) / 2;
    if( gMovType & 0x04 ) Ypos /= 2;
    if( flags ){
	MovVideoCopy( gMovScrPixelsA, gMovSurfWidth, gMovSurfHeight, gMovMovieXpos, gMovMovieYpos, gMovUnk02, gMovUnk20, Xpos, Ypos, flags );
    } else {
	if( gMovScaled == 4 ) 
	    gMovVideoCopy( (char *)gMovScrPixelsA, gMovSurfWidth, gMovSurfHeight, gMovMovieXpos, gMovMovieYpos, gMovUnk02, gMovUnk20, Xpos, Ypos );
	else
	    gMovVideoCopy( (char *)gMovScrPixelsA, gMovSurfWidth, gMovSurfHeight, 0, gMovMovieYpos, ofst, gMovUnk20, Xpos, Ypos );
    }
}

void MovSetPalette8A( short Start, short Count )
{
    if( !gMov16BppMode ) gMovSetPalette( gMovPaletteA, Start, Count );
}

void MovSetPalette8B( short PalStart, short PalCount )
{
    if( !gMov16BppMode ) gMovSetPalette( gMovPaletteA, PalStart, PalCount ); // changed
}

void MovCreateGradient( char BaseA, char a2, char a3, char BaseB, char a5, char a6 )
{
    int i, j;

    for( j = 0; j < a2; j++, BaseA += a3 ){
        for( i = 0; i < a3; i++ ){
            gMovPaletteA[ BaseA + i ].r = (j * 63) / (a2 - 1);
            gMovPaletteA[ BaseA + i ].g = 0;
            gMovPaletteA[ BaseA + i ].b = 5 * ((i * 63) / (a3 - 1)) / 8;
        }            
    }        
    for( j = 0; j < a5; j++, BaseB += a6 ){
        for( i = 0; i < a6; i++ ){
            gMovPaletteA[ BaseB + i ].r = 0;
            gMovPaletteA[ BaseB + i ].g = (j * 63) / (a5 - 1);
            gMovPaletteA[ BaseB + i ].b = 5 * (i * 63 / (a6 - 1)) / 8;
        }            
    }
}

void MovSetPalette( char *Pal, short Start, short Count )
{
    int i, *p;

    p = (void *)&gMovPaletteA[ Start ];
    for( i = 0; i < Count; i++, Pal += 3 ){
	*p++ = *((int *)Pal);
    }
}

void MovStop()
{
    if( !gMovRun ) return;
    MovTimeUpdate();
    MovTimerStop();
    MovSoundStop();
    gMovRun = 0;
}

void MovTimerStop()
{
    gMovTimerEn = 0;
}

int MovPlay( xFile_t *stream, int a2, int a3, char a4 )
{
    int err;

    err = MovPrep( stream, a2, a3, a4 );
    while( !err ){
        if( (err = MovStep()) ) break;
        while ( !err ){
            if( (err = gMovAfterFrameCb() ) != -1 ) break;
            MovSndMute();
        }
    }
    MovStop();
    return err == -1 ? 0 : err;
}

int MovPlayB( xFile_t *a1, int a2, int a3, char a4 )
{
    int err;

    err = MovPrep( a1, a2, a3, a4 );
    while( !err ){
        if( ( err = MovStep() ) ) break;
        while( !err ){
            if( ( err = gMovAfterFrameCb() ) != -1 ) break;
            MovSndMute();
        }
    }
    return err == -1 ? 0 : err;
}

void MovClose()
{
    MovStop();
    MovUnk52();
    MovVideoClose();
}

void MovUnk52()
{
    MovFreeP(&gMovInputData);
}

void MovVideoClose()
{
    if( gMovSurfaceA ) SDL_FreeSurface( gMovSurfaceA );
    gMovSurfaceA = NULL;
    if( gMovSurfaceB ) SDL_FreeSurface( gMovSurfaceB );
    gMovSurfaceB = NULL;
}

const char *MovGetError( int ErrNo )
{
    char *ErrMsg[14];

    ErrMsg[0] = "Movie completed normally";
    ErrMsg[1] = "Movie completed normally";
    ErrMsg[2] = "File I/O error or Unable to allocate I/O buffers";
    ErrMsg[3] = "Unable to create timer";
    ErrMsg[4] = "Unable to allocate sound buffers";
    ErrMsg[5] = "Unable to allocate video buffers";
    ErrMsg[6] = "Insufficient screen resolution for movie";
    ErrMsg[7] = "Unable to setup graphics mode used by movie";
    ErrMsg[8] = "Invalid movie file";
    ErrMsg[9] = "Incorrect screen color mode";
    ErrMsg[10] = "StepMovie() without PrepMovie()";
    ErrMsg[11] = "Unable to initialize DirectDraw";
    ErrMsg[12] = "DirectDraw Surface Lost";
    ErrMsg[13] = "Unknown movie error code";
    if( ErrNo >= 2 ) ErrNo = 2;
    if( ErrNo <= MOV_ERR_UNK ) ErrNo = MOV_ERR_UNK;
    return ErrMsg[ -ErrNo ];
}

#include "MovOld.c"

void MovPCMdecodeMono( short *OutBuf, unsigned char *pInNBuf, int nbytes, short *FirstSample )
{
    int Sample, i;

    Sample = FirstSample[ 0 ];
    for( i = 0; i < nbytes; i++ ){
        Sample += gMovPcmTable[ *pInNBuf++ ];
        *OutBuf++ = Sample;
    }
}

void MovPCMdecodeStereo( short *OutBuf, unsigned char *pInBuf, int nbytes, short *FirstSample )
{
    int  LastR, LastL, i;

    LastR = FirstSample[ 0 ];
    LastL = FirstSample[ 1 ];
    for( i = 0; i < nbytes; i++ ){
        LastR += gMovPcmTable[ *pInBuf++ ];
        *OutBuf++ = LastR;
        LastL += gMovPcmTable[ *pInBuf++ ];
        *OutBuf++ = LastL;
    }
}

#include "MovDecode10.c"

void MovEncPitchInit()
{
    int *p, k, i;

    p = gMovEncPosY;
    k = 0;    
    for( i = 0; i < 128; i++ ){
        *p++ = k;
        k += gMovSurfWidth;
    }

    k = -128 * gMovSurfWidth;
    for( i = 0; i < 128; i++ ){
        *p++ = k;
        k += gMovSurfWidth;
    }
}

#include "MovDecode8bpp.c"
#include "MovDecode16bpp.c"

void MovProtUnk25( short a1, int a2, int a3, int *a4 )
{
    int *v4;

    a4[ 0 ] = a1; pINT( a4 + a2 )[ 0 ] = a1; a4[ 1 ] = a3; pINT( a4 + a2 )[ 1 ] = a3; a4[ 2 ] = a3; pINT( a4 + a2 )[ 2 ] = a3; a4[ 3 ] = a3; pINT( a4 + a2 )[ 3 ] = a3;
    v4 = pINT( a4 + 2 * a2 );
    v4[ 0 ] = a3; pINT( v4 + a2 )[ 0 ] = a3; v4[ 1 ] = a3; pINT( v4 + a2 )[ 1 ] = a3; v4[ 2 ] = a3; pINT( v4 + a2 )[ 2 ] = a3; v4[ 3 ] = a3; pINT( v4 + a2 )[ 3 ] = a3;
    v4 = pINT( v4 + 2 * a2 );
    v4[ 0 ] = a3; pINT( v4 + a2 )[ 0 ] = a3; v4[ 1 ] = a3; pINT( v4 + a2 )[ 1 ] = a3; v4[ 2 ] = a3; pINT( v4 + a2 )[ 2 ] = a3; v4[ 3 ] = a3; pINT( v4 + a2 )[ 3 ] = a3;
    v4 = pINT( v4 + 2 * a2 );
    v4[ 0 ] = a3; pINT( v4 + a2 )[ 0 ] = a3; v4[ 1 ] = a3; pINT( v4 + a2 )[ 1 ] = a3; v4[ 2 ] = a3; pINT( v4 + a2 )[ 2 ] = a3; v4[ 3 ] = a3; pINT( v4 + a2 )[ 3 ] = a3;
}

void MovVideoCopy( unsigned char *pSrc, unsigned int SrcW, int SrcH, unsigned int SrcX, int SrcY, unsigned int DstW, unsigned int DstH, int DstX, int DstY, int nnn )
{
    char *dst, *v28, *v22,*v26, *v24;
    unsigned char *src, *v20;
    int v21,v27,DstPitch,SrcPitch,x,y, i, j;
    unsigned int v12,v23,w,v35;

    SrcPitch = ( gMovScaled == 4 ) ? (SrcW - DstW) : gMovUnk103;
    w = gMovLines;
    if( nnn ) w = 2 * gMovLines;
    DstPitch = w - DstW;
    src = pSrc + SrcW * SrcY + SrcX;
    x = SrcX + DstX;
    v12 = SrcW / 2;
    if( nnn && ( SrcX >= v12 ) ) x -= v12;
    y = SrcY + DstY;
    if( gMovCb_07 ){
        v20 = src;
        v21 = y * w;
        if( nnn & 0x01 ) v21 += gMovLines;
        v35 = (x + v21) / gMov_05;
        v22 = (char *)gMovScreen + (x + v21) % gMov_05;
        gMovCb_07( v35, v35, gMov_02[0] );
        for( ; DstH; DstH-- ){
            v23 = (w + gMov_06 - v22 - DstW ) / w;
            if( DstH < v23 ) v23 = DstH;
            if( v23 ){
                DstH -= v23;
                if( gMovScaled == 3 ){
                    v24 = v22 - 2;
                    for( j = 0; j < v23; j++ ){
                        for( i = 0; i < DstW /16; i++ ){
                            pINT( v24 )[ 2 ] = pINT( v20 + 0 )[ i + 0 ]; pINT( v24 )[ 3 ] = pINT( v20 + 3 )[ i + 1 ]; v24 += 16;
                            pINT( v24 )[ 0 ] = pINT( v20 + 6 )[ i + 2 ]; pINT( v24 )[ 1 ] = pINT( v20 + 9 )[ i + 3 ]; v20 += 3;
                        }
                        pINT( v24 )[ 2 ] = pINT( v20 + 0 )[ 0 ]; pINT( v24 )[ 3 ] = pINT( v20 + 3 )[ 0 ]; v26 = v24 + 3;
                        pINT( v26 )[ 1 ] = pINT( v20 + 6 )[ 0 ];
                        v20 += SrcPitch + 9;
                        v24 = v26 + DstPitch;
                    }
                    v22 = v24 + 8;
                } else {
                    for( ; i < v23; i++ ) memcpy( v22, v20, DstW ); 
                    v20 += DstW + SrcPitch; v22 += DstW + DstPitch;
                }
            }
            if( !(DstH | v23) ) break;
            v27 = gMov_06 - v22;
            if( v27 < 0 ) v27 = 0;
            if( gMovScaled == 3 ){
                for( i = 0; i < v27; i++ ) pINT( v22 )[ i ] = pINT( v20 + i*3 )[ 0 ];
            } else {
                memcpy( v22, v20, 4 * v27 ); v20 += v27*4; v22 += v27*4;
            }
            v35 += gMov_08;
            v28 = v22 - gMov_03;
            gMovCb_07( gMov_08, v35, gMov_02[ 0 ] );
	    j =  DstW / 4 - v27;
            if( gMovScaled == 3 ){
                for( i = 0; i < j; i++ ) pINT( v28 )[ i ] = pINT( v20 + i*3 )[ 0 ];
            } else {
                memcpy( v28, v20, DstW - 4*v27 ); v20 += j * 4; v28 += j * 4;
            }
            v20 += SrcPitch;
            v22 = v28 + DstPitch;            
        }
    } else {
        dst = (char *)gMovScreen + w * y + x;
        if( nnn & 0x01 ) dst += gMovLines;
        if( gMovScaled == 3 ){
            dst -= 8;
            for( j = DstH; j; j--, src += SrcPitch + 9, dst += DstPitch + 12 ){
                for( i = DstW / 16; i; i--, src += 12, dst += 16 ){
                    pINT( dst )[ 2 ] = pINT( src + 0 )[ 0 ];
                    pINT( dst )[ 3 ] = pINT( src + 3 )[ 0 ];                    
                    pINT( dst )[ 0 ] = pINT( src + 6 )[ 0 ];
                    pINT( dst )[ 1 ] = pINT( src + 9 )[ 0 ];                    
                }
                pINT( dst )[ 2 ] = pINT( src + 0 )[ 0 ];
                pINT( dst )[ 3 ] = pINT( src + 3 )[ 0 ];
                pINT( dst + 12 )[ 1 ] = pINT( src + 6 )[ 0 ];                
            }
        } else {
            for( i = DstH; i; i-- ){ // blit 1:1
                memcpy( dst, src, DstW );
                src += DstW + SrcPitch;
                dst += DstW + DstPitch;
            }
        }
    }
}

void MovSetPaletteCompressedP( char *pData )
{
    Pal8_t *p;
    int i;
    char dat;

    p = gMovPaletteA;
    for( i = 0; i < 32; i++, p += 8 ){
        dat = *pData++;
        if( !dat ) continue;
        if( dat & 0x01 ){ p[ 0 ].r = pData[ 0 ]; p[ 0 ].g = pData[ 1 ]; p[ 0 ].b = pData[ 2 ]; pData += 3; }
    	if( dat & 0x02 ){ p[ 1 ].r = pData[ 0 ]; p[ 1 ].g = pData[ 1 ]; p[ 1 ].b = pData[ 2 ]; pData += 3; }
    	if( dat & 0x04 ){ p[ 2 ].r = pData[ 0 ]; p[ 2 ].g = pData[ 1 ]; p[ 2 ].b = pData[ 2 ]; pData += 3; }
    	if( dat & 0x08 ){ p[ 3 ].r = pData[ 0 ]; p[ 3 ].g = pData[ 1 ]; p[ 3 ].b = pData[ 2 ]; pData += 3; }
    	if( dat & 0x10 ){ p[ 4 ].r = pData[ 0 ]; p[ 4 ].g = pData[ 1 ]; p[ 4 ].b = pData[ 2 ]; pData += 3; }
    	if( dat & 0x20 ){ p[ 5 ].r = pData[ 0 ]; p[ 5 ].g = pData[ 1 ]; p[ 5 ].b = pData[ 2 ]; pData += 3; }
    	if( dat & 0x40 ){ p[ 6 ].r = pData[ 0 ]; p[ 6 ].g = pData[ 1 ]; p[ 6 ].b = pData[ 2 ]; pData += 3; }
    	if( dat & 0x80 ){ p[ 7 ].r = pData[ 0 ]; p[ 7 ].g = pData[ 1 ]; p[ 7 ].b = pData[ 2 ]; pData += 3; }
    }
}

