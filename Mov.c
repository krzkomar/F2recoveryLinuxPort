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
int gMovSoundVol = 0;
int gMovAudioPlaying = 0;
int gMovSndPan = 0;
SDL_Surface *gMovSurfaceA = NULL;
SDL_Surface *gMovSurfaceB = NULL;
int (*gMovVideoCopy)( char *, int, int, int, int, int, int, int, int );
int gMovUnk13 = 1;
int gMovUnk12 = 0;
int (*gMovSetPalette)( Pal8_t *Palette, int PalStart, int PalCount );
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
    gMovSoundVol = Vol;
//    if( gMovSoundBuffer )  gMovSoundBuffer->lpVtbl->SetVolume( gMovSoundBuffer, LODWORD(Vol) );
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
    gMovSetPalette = (void *)pal;
}

/*
int MovUnk01()
{
    return MovUnk03( 167, 420, 350, 370, 0, 0, 0, 0 );
}

int MovUnk03( char a1, int a2, int a3, unsigned int a4, char a5, char a6, int a7, char a8 )
{
    Mov01_t mv;
    char v10, v11;

    mv.M1 = a2 - 2;
    mv.M2 = ((unsigned __int16)(a2 - 2) >> 8) & 1 | (a4 >> 2) & 0x80 | (a4 >> 6) & 4 | ((unsigned int)(a3 - 1) >> 3) & 0x40 | ((unsigned int)(a2 - 2) >> 4) & 0x20 | ((unsigned int)(a3 - 1 + a7) >> 5) & 8 | ((unsigned int)(a3 - 1) >> 7) & 2;
    mv.M3 = a6 | (a5 << 7) | ((unsigned int)(a3 - 1 + a7) >> 4) & 0x20;
    mv.M4 = a4;
    mv.M5 = (a4 + 2) & 0xF;
    mv.M6 = a3 - 1;
    v10 = a3 - 1 + a7;
    v11 = a2 - a8 - 2;
    return MovProtUnk20(a1, &mv);
}
*/

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
            	    MovDecodeOldFrame( &Chunk->Op5.w7, Chunk->Op5.w2, Chunk->Op5.FlipBack, Chunk->Op5.w4, Chunk->Op5.w5 );
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
            	    MovSetPalette( (char *)&Chunk->Pal.Flags, Chunk->Pal.PalStart, Chunk->Pal.PalCount );
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
    memcpy( &gMovPaletteA[ Start ], Pal, 3 * Count );
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

void MovProtUnk03( short *a1, int a2, int a3, int a4, int a5 )
{
/*
    unsigned int result; // eax
    short *v9; // ebx
    char v11; // ch
    char v12; // cl
    char v25; // ch
    char v26; // cl
    char *v66; // [esp+18h] [ebp-4h]
    int v5; // ecx
    int *s; // edi MAPDST
    int v8; // edx
    int *p; // esi MAPDST
    int *v22; // ebx
    int v24; // esi
    int v43; // ecx
    int *v60; // [esp-8h] [ebp-24h]
    int *v61; // [esp-8h] [ebp-24h]
    int v62; // [esp-4h] [ebp-20h]
    int v63; // [esp-4h] [ebp-20h]
    int v64; // [esp+10h] [ebp-Ch]
    int v65; // [esp+14h] [ebp-8h]

    v64 = gMovScrPixelsB - gMovScrPixelsA;
    gMovMovieXpos = 16 * a2;
    gMovUnk02 = 16 * a4;
    gMovMovieYpos = gMovScrSize * 8 * a3;
    gMovUnk20 = gMovScrSize * 8 * a5;
    v65 = gMovScrBufSizeA - 16 * a4;
    v66 = gMovScrPixelsA;
    if( a2 || a3 ) v66 = gMovScrPixelsA + gMovMovieXpos + gMovSurfWidth * gMovMovieYpos;
    result = 2 * gMovScrSize * a5 * a4;
    s = v66;
    v8 = gMovScrBufSize;
    v9 = a1;
    p = &a1[ result / 2 ];
    v5 = gMovScrSize;
    for( ;v62 != 1; v62-- ){
        v62 = v5;
        v60 = s;        
        for( v11 = a5; v11; v11-- ){
            v12 = a4;
            for( ;v12; v9++, v12-- ){
                if( *v9 ){
                    s += 4;
                } else {
                    *s = *p++; *++s = *p++; *++s = *p++; *++s = *p++; s = (int *)((char *)s + v8 + 4);
                    *s = *p++; *++s = *p++; *++s = *p++; *++s = *p++; s = (int *)((char *)s + v8 + 4);
                    *s = *p++; *++s = *p++; *++s = *p++; *++s = *p++; s = (int *)((char *)s + v8 + 4);
                    *s = *p++; *++s = *p++; *++s = *p++; *++s = *p++; s = (int *)((char *)s + v8 + 4);
                    *s = *p++; *++s = *p++; *++s = *p++; *++s = *p++; s = (int *)((char *)s + v8 + 4);
                    *s = *p++; *++s = *p++; *++s = *p++; *++s = *p++; s = (int *)((char *)s + v8 + 4);
                    *s = *p++; *++s = *p++; *++s = *p++; *++s = *p++; s = (int *)((char *)s + v8 + 4);
                    *s = *p++; *++s = *p++; *++s = *p++; *++s = *p++; s = (int *)((char *)s - gMovScrBufSizeB + 4);
                }                
            }            
            s = (int *)((char *)s + v65);            
        }        
        s = (int *)((char *)v60 + gMovSurfWidth);
    }
    v22 = (int *)&v9[result / 0xFFFFFFFE];
    s = (int *)v66;
    LOBYTE(v5) = gMovScrSize;
    v24 = 0;
    do{
        v63 = v5;
        v61 = s;
        v25 = a5;
        do{
            v26 = a4;
            do{
                LOWORD(v24) = *(short *)v22 | v24;
                if( (__int16)v24 <= 0 ){
                    if( (v24 & 0x8000u) == 0 ){
                        s += 4;
                        goto LABEL_19;
                    }
LABEL_35:
                    p = (int *)((char *)s + 2 * v24 + v64 - 98304); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                    p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                    p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                    p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                    p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                    p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                    p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                    p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p; *++s = p[1];
                    s = (int *)((char *)s - gMovScrBufSizeB + 4);
                    v24 = 0;
                    while( 1 ){
                        v22 = (int *)((char *)v22 + 2);
                        if( !--v26 ){
                            s = (int *)((char *)s + v65);
                            if( !--v25 ){
                                v43 = v63;
                                s = (int *)((char *)v61 + gMovSurfWidth);
                                if( (_BYTE)v63 == 1 ) return result;
                                LOBYTE(v43) = v63 - 1;
                                v63 = v43;
                                v61 = (int *)((char *)v61 + gMovSurfWidth);
                                v25 = a5;
                            }
                            v26 = a4;
                        }
                        LOWORD(v24) = *(short *)v22 | v24;
                        if( (v24 & 0x8000u) != 0 ) goto LABEL_35;
                        if( (__int16)v24 > 0 ) break;
                        s += 4;
                    }
                }
                p = (int *)((char *)s + 2 * v24 - 0x8000);
                *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p++; *++s = *p;
                p = (int *)((char *)p + v8 + 4); s = (int *)((char *)s + v8 + 4); *s = *p++; *++s = *p++; *++s = *p; *++s = p[1];
                s = (int *)((char *)s - gMovScrBufSizeB + 4);
                v24 = 0;
LABEL_19:
                v22 = (int *)((char *)v22 + 2);
                v26--;
            } while ( v26 );
            s = (int *)((char *)s + v65);
            v25--;
        }while ( v25 );
        v5 = v63;
        s = (int *)((char *)v61 + gMovSurfWidth);
        LOBYTE(v5) = v63 - 1;
    }
    while ( (_BYTE)v63 != 1 );
    return result;
*/
}

void MovProtUnk04( short *a1, short *a2, char *a3, int a4, int a5, int a6, int a7 )
{
/*
    int v7; // ecx
    int *s; // edi MAPDST
    int v9; // edx
    short *v11; // ebx
    short v12; // ax
    char v13; // ch
    char v14; // cl
    bool v15; // zf
    bool v16; // cc
    short *v17; // eax
    int *p; // esi MAPDST
    short *v28; // ebx
    short result; // ax
    int v30; // esi
    char v31; // ch
    char v32; // cl
    bool v33; // zf
    short *v34; // eax
    bool v51; // zf
    int v52; // ecx
    short *v53; // eax
    int *v70; // [esp-8h] [ebp-24h]
    int *v71; // [esp-8h] [ebp-24h]
    int v72; // [esp-4h] [ebp-20h]
    int v73; // [esp-4h] [ebp-20h]
    short *v74; // [esp+Ch] [ebp-10h]
    short *v75; // [esp+Ch] [ebp-10h]
    int v76; // [esp+10h] [ebp-Ch]
    int v77; // [esp+14h] [ebp-8h]
    int *v78; // [esp+18h] [ebp-4h]

    v76 = (_BYTE *)gMovScrPixelsB - (_BYTE *)gMovScrPixelsA;
    gMovMovieXpos = 16 * a4;
    gMovUnk02 = 16 * a6;
    gMovMovieYpos = (unsigned __int8)gMovScrSize * 8 * a5;
    gMovUnk20 = (unsigned __int8)gMovScrSize * 8 * a7;
    v77 = gMovScrBufSizeA - 16 * a6;
    v78 = (int *)gMovScrPixelsA;
    if ( a4 || a5 )
        v78 = (int *)((char *)gMovScrPixelsA + gMovMovieXpos + gMovSurfWidth * gMovMovieYpos);
    s = v78;
    v9 = gMovScrBufSize;
    v11 = a2;
    v74 = a1;
    v12 = 0;
    LOBYTE(v7) = gMovScrSize;
    do
    {
        v72 = v7;
        v70 = s;
        v13 = a7;
        do
        {
            v14 = a6;
            do
            {
                while ( 1 )
                {
                    v15 = 2 * v12 == 0;
                    v16 = __CFADD__(v12, v12) || v15;
                    v12 *= 2;
                    if ( !v16 )
                        goto LABEL_11;
                    if ( !v15 )
                        break;
                    v17 = v74++;
                    v12 = *v17;
                }
                if ( *v11 )
                {
                    ++v11;
LABEL_11:
                    s += 4;
                    goto LABEL_12;
                }
                *s = *a3;
                p = a3 + 1;
                *++s = *p++;
                *++s = *p++;
                *++s = *p++;
                s = (int *)((char *)s + v9 + 4);
                *s = *p++;
                *++s = *p++;
                *++s = *p++;
                *++s = *p++;
                s = (int *)((char *)s + v9 + 4);
                *s = *p++;
                *++s = *p++;
                *++s = *p++;
                *++s = *p++;
                s = (int *)((char *)s + v9 + 4);
                *s = *p++;
                *++s = *p++;
                *++s = *p++;
                *++s = *p++;
                s = (int *)((char *)s + v9 + 4);
                *s = *p++;
                *++s = *p++;
                *++s = *p++;
                *++s = *p++;
                s = (int *)((char *)s + v9 + 4);
                *s = *p++;
                *++s = *p++;
                *++s = *p++;
                *++s = *p++;
                s = (int *)((char *)s + v9 + 4);
                *s = *p++;
                *++s = *p++;
                *++s = *p++;
                *++s = *p++;
                s = (int *)((char *)s + v9 + 4);
                *s = *p++;
                *++s = *p++;
                *++s = *p++;
                *++s = *p;
                a3 = p + 1;
                s = (int *)((char *)s - gMovScrBufSizeB + 4);
                ++v11;
LABEL_12:
                --v14;
            }
            while ( v14 );
            s = (int *)((char *)s + v77);
            --v13;
        }
        while ( v13 );
        v7 = v72;
        s = (int *)((char *)v70 + gMovSurfWidth);
        LOBYTE(v7) = v72 - 1;
    }
    while ( (_BYTE)v72 != 1 );
    s = v78;
    v28 = a2;
    v75 = a1;
    result = 0;
    LOBYTE(v7) = gMovScrSize;
    v30 = 0;
LABEL_18:
    v73 = v7;
    v71 = s;
    v31 = a7;
    while ( 2 )
    {
        v32 = a6;
LABEL_20:
        while ( 2 )
        {
            v33 = 2 * result == 0;
            v16 = __CFADD__(result, result) || v33;
            result *= 2;
            if ( !v16 )
            {
LABEL_25:
                s += 4;
                goto LABEL_26;
            }
            if ( v33 )
            {
                v34 = v75++;
                result = *v34;
                continue;
            }
            break;
        }
        LOWORD(v30) = *v28 | v30;
        if ( (short)v30 > 0 )
        {
LABEL_32:
            p = (int *)((char *)s + 2 * v30 - 0x8000);
            *s = *p++;
            *++s = *p++;
            *++s = *p++;
            *++s = *p;
            p = (int *)((char *)p + v9 + 4);
            s = (int *)((char *)s + v9 + 4);
            *s = *p++;
            *++s = *p++;
            *++s = *p++;
            *++s = *p;
            p = (int *)((char *)p + v9 + 4);
            s = (int *)((char *)s + v9 + 4);
            *s = *p++;
            *++s = *p++;
            *++s = *p++;
            *++s = *p;
            p = (int *)((char *)p + v9 + 4);
            s = (int *)((char *)s + v9 + 4);
            *s = *p++;
            *++s = *p++;
            *++s = *p++;
            *++s = *p;
            p = (int *)((char *)p + v9 + 4);
            s = (int *)((char *)s + v9 + 4);
            *s = *p++;
            *++s = *p++;
            *++s = *p++;
            *++s = *p;
            p = (int *)((char *)p + v9 + 4);
            s = (int *)((char *)s + v9 + 4);
            *s = *p++;
            *++s = *p++;
            *++s = *p++;
            *++s = *p;
            p = (int *)((char *)p + v9 + 4);
            s = (int *)((char *)s + v9 + 4);
            *s = *p++;
            *++s = *p++;
            *++s = *p++;
            *++s = *p;
            p = (int *)((char *)p + v9 + 4);
            s = (int *)((char *)s + v9 + 4);
            *s = *p++;
            *++s = *p++;
            *++s = *p;
            *++s = p[1];
            s = (int *)((char *)s - gMovScrBufSizeB + 4);
            v30 = 0;
            ++v28;
LABEL_26:
            if ( !--v32 )
            {
                s = (int *)((char *)s + v77);
                if ( --v31 )
                    continue;
                v7 = v73;
                s = (int *)((char *)v71 + gMovSurfWidth);
                LOBYTE(v7) = v73 - 1;
                if ( (_BYTE)v73 == 1 )
                    return result;
                goto LABEL_18;
            }
            goto LABEL_20;
        }
        break;
    }
    if ( (v30 & 0x8000u) == 0 )
    {
        ++v28;
        goto LABEL_25;
    }
LABEL_47:
    p = (int *)((char *)s + 2 * v30 + v76 - 98304);
    *s = *p++;
    *++s = *p++;
    *++s = *p++;
    *++s = *p;
    p = (int *)((char *)p + v9 + 4);
    s = (int *)((char *)s + v9 + 4);
    *s = *p++;
    *++s = *p++;
    *++s = *p++;
    *++s = *p;
    p = (int *)((char *)p + v9 + 4);
    s = (int *)((char *)s + v9 + 4);
    *s = *p++;
    *++s = *p++;
    *++s = *p++;
    *++s = *p;
    p = (int *)((char *)p + v9 + 4);
    s = (int *)((char *)s + v9 + 4);
    *s = *p++;
    *++s = *p++;
    *++s = *p++;
    *++s = *p;
    p = (int *)((char *)p + v9 + 4);
    s = (int *)((char *)s + v9 + 4);
    *s = *p++;
    *++s = *p++;
    *++s = *p++;
    *++s = *p;
    p = (int *)((char *)p + v9 + 4);
    s = (int *)((char *)s + v9 + 4);
    *s = *p++;
    *++s = *p++;
    *++s = *p++;
    *++s = *p;
    p = (int *)((char *)p + v9 + 4);
    s = (int *)((char *)s + v9 + 4);
    *s = *p++;
    *++s = *p++;
    *++s = *p++;
    *++s = *p;
    p = (int *)((char *)p + v9 + 4);
    s = (int *)((char *)s + v9 + 4);
    *s = *p++;
    *++s = *p++;
    *++s = *p;
    *++s = p[1];
    s = (int *)((char *)s - gMovScrBufSizeB + 4);
    ++v28;
    v30 = 0;
    while ( --v32 )
    {
        while ( 1 )
        {
LABEL_35:
            v51 = 2 * result == 0;
            v16 = __CFADD__(result, result) || v51;
            result *= 2;
            if ( !v16 )
                goto LABEL_40;
            if ( !v51 )
                break;
            v53 = v75++;
            result = *v53;
        }
        LOWORD(v30) = *v28 | v30;
        if ( (v30 & 0x8000u) != 0 )
            goto LABEL_47;
        if ( (short)v30 > 0 )
            goto LABEL_32;
        ++v28;
LABEL_40:
        s += 4;
    }
    s = (int *)((char *)s + v77);
    if ( --v31 )
    {
LABEL_34:
        v32 = a6;
        goto LABEL_35;
    }
    v52 = v73;
    s = (int *)((char *)v71 + gMovSurfWidth);
    if ( (_BYTE)v73 != 1 )
    {
        LOBYTE(v52) = v73 - 1;
        v73 = v52;
        v71 = (int *)((char *)v71 + gMovSurfWidth);
        v31 = a7;
        goto LABEL_34;
    }
    return result;
*/
}

void MovDecodeOldFrame( short *a1, short a2, short a3, short nbytes, short a5 )
{
/*
    char zz; // cl
    unsigned int result; // eax
    int rr; // edx
    short *pp; // ebx
    short *tt; // esi
    char bbA; // ch
    char i; // cl MAPDST
    char *aa; // edi MAPDST
    char *bb; // esi MAPDST
    short *kk; // ebx
    char j; // ch
    int qq; // ecx
    int *aaA; // [esp-8h] [ebp-24h]
    char *ee; // [esp-8h] [ebp-24h]
    char zzA; // [esp-4h] [ebp-20h]
    int ww; // [esp-4h] [ebp-20h]
    int dd; // [esp+10h] [ebp-Ch]
    int hh; // [esp+14h] [ebp-8h]
    char *xx; // [esp+18h] [ebp-4h]

    if( gMov16BppMode ) return MovProtUnk03(a1, a2, a3, nbytes, a5);
    dd = (_BYTE *)gMovScrPixelsB - (_BYTE *)gMovScrPixelsA;
    gMovMovieXpos = 8 * a2;
    gMovUnk02 = 8 * nbytes;
    gMovMovieYpos = (unsigned __int8)gMovScrSize * 8 * a3;
    gMovUnk20 = (unsigned __int8)gMovScrSize * 8 * a5;
    hh = gMovScrBufSizeA - 8 * nbytes;
    xx = (char *)gMovScrPixelsA;
    if ( a2 || a3 ) xx = (char *)gMovScrPixelsA + gMovMovieXpos + gMovSurfWidth * gMovMovieYpos;
    result = 2 * (unsigned __int8)gMovScrSize * a5 * nbytes;
    aa = xx;
    rr = gMovScrBufSize;
    pp = a1;
    tt = &a1[result / 2];
    zz = gMovScrSize;
    do{
        *(int *)&zzA = *(int *)&zz;
        aaA = (int *)aa;
        bbA = a5;
        do{
            i = nbytes;
            do{
                if( *pp ){
                    aa += 8;
                } else {
                    *(int *)aa = *(int *)tt; bb = (char *)(tt + 2); aa += 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; tt = (short *)(bb + 4); aa = &aa[-gMovScrBufSizeB + 4];
                }
                pp++;
                i--;
            }while ( i );
            aa += hh;
            bbA--;
        } while ( bbA );
        *(int *)&zz = *(int *)&zzA;
        aa = (char *)aaA + gMovSurfWidth;
        zz = zzA - 1;
    } while ( zzA != 1 );
    kk = &pp[result / 0xFFFFFFFE];
    aa = xx;
    zz = gMovScrSize;
    bb = 0;
    do{
        ww = *(int *)&zz;
        ee = aa;
        j = a5;
        do{
            i = nbytes;
            do{
                LOWORD(bb) = *kk | (unsigned short)bb;
                if( (short)bb <= 0 ){
                    if( (short)bb >= 0 ){
                        aa += 8;
                        goto LABEL_21;
                    }
LABEL_37:
                    bb = &bb[(int)aa - 0xC000 + dd]; 
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                    *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                    *(int *)aa = *(int *)bb; aa += 4;
                    *(int *)aa = *((int *)bb + 1); aa = &aa[-gMovScrBufSizeB + 4];
                    bb = 0;
                    while( 1 ){
                        kk++;
                        if( !--i ){
                            aa += hh;
                            if( !--j ){
                                qq = ww;
                                aa = &ee[gMovSurfWidth];
                                if( (_BYTE)ww == 1 ) return result;
                                LOBYTE(qq) = ww - 1;
                                ww = qq;
                                ee += gMovSurfWidth;
                                j = a5;
                            }
                            i = nbytes;
                        }
                        LOWORD(bb) = *kk | (unsigned short)bb;
                        if( (short)bb < 0 ) goto LABEL_37;
                        if( (short)bb > 0 ) break;
                        aa += 8;
                    }
                }
                bb = &bb[(int)aa - 0x4000]; 
                *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                *(int *)aa = *(int *)bb; bb += 4; aa += 4;
                *(int *)aa = *(int *)bb; bb += rr + 4; aa += rr + 4;
                *(int *)aa = *(int *)bb; aa += 4;
                *(int *)aa = *((int *)bb + 1); aa = &aa[-gMovScrBufSizeB + 4];
                bb = 0;
LABEL_21:
                kk++;
                i--;
            }while ( i );
            aa += hh;
            j--;
        }while ( j );
        *(int *)&zz = ww;
        aa = &ee[gMovSurfWidth];
        zz = ww - 1;
    }
    while ( (_BYTE)ww != 1 );
    return result;
*/
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

void MovProtUnk24( int a1, short a2, int *a3, int a4 )
{
/*
    int v4,*v5,*v6,*v7,v8;

    v4 = a1 << 16;
    LOWORD(v4) = a2; a3[ 0 ] = v4; LOWORD(v4) = a2; v4 <<= 16; LOWORD(v4) = a2; a3[ 1 ] = v4;

    v5 = pINT( a3 + a4 );
    LOWORD(v4) = a2; v4 <<= 16; LOWORD(v4) = a2; *v5 = v4; LOWORD(v4) = a2; v4 <<= 16; LOWORD(v4) = a2; v5[1] = v4;

    v6 = pINT( v5 + a4 );
    LOWORD(v4) = a2; v4 <<= 16; LOWORD(v4) = a2; *v6 = v4; LOWORD(v4) = a2; v4 <<= 16; LOWORD(v4) = a2; v6[1] = v4;

    v7 = pINT( v6 + a4 );
    LOWORD(v4) = a2; v4 <<= 16; LOWORD(v4) = a2; *v7 = v4; LOWORD(v4) = a2; v8 = v4 << 16; LOWORD(v8) = a2; v7[1] = v8;
*/
}

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

void MovVideoCopy( char *pSrc, unsigned int SrcW, int SrcH, unsigned int SrcX, int SrcY, unsigned int DstW, unsigned int DstH, int DstX, int DstY, int nnn )
{
    char *dst,*src, *v20, *v28, *v22,*v26, *v24;
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

void MovProtUnk26( int a1, unsigned int a2, int a3, unsigned int a4, int a5, unsigned int a6, unsigned int a7, int a8, int a9, int a10 )
{
/*
    int v10; // ecx
    int v11; // eax
    unsigned int v12; // eax
    unsigned __int64 v13; // rax
    int *v14; // edi
    _BYTE *v15; // esi
    unsigned int v16; // ebx
    unsigned int v17; // ecx
    _BYTE *v18; // esi
    int v19; // eax
    int *v20; // edi
    unsigned int v21; // ecx
    int v22; // ecx
    int *v23; // edi
    unsigned int v24; // ecx
    unsigned int v26; // [esp-4h] [ebp-24h]
    int v27; // [esp-4h] [ebp-24h]
    int v28; // [esp-4h] [ebp-24h]
    int v29; // [esp+Ch] [ebp-14h]
    unsigned int v30; // [esp+10h] [ebp-10h]
    int v31; // [esp+14h] [ebp-Ch]
    unsigned int v32; // [esp+18h] [ebp-8h]
    unsigned int v33; // [esp+1Ch] [ebp-4h]
    _BYTE *v34; // [esp+28h] [ebp+8h]
    int v35; // [esp+44h] [ebp+24h]
    unsigned int v36; // [esp+48h] [ebp+28h]

    v32 = a6 >> 1;
    v31 = a2 - a6;
    v11 = gMovLines;
    if ( a10 )
        v11 = 2 * gMovLines;
    v30 = v11;
    v29 = v11 - a6 - a6;
    v34 = (_BYTE *)(a4 + a2 * a5 + a1);
    v35 = a4 + a8;
    v12 = a2 >> 1;
    if ( a10 && a4 >= v12 )
        v35 -= v12;
    v36 = a5 + a9;
    if ( gMovCb_07 )
    {
        v18 = v34;
        v19 = v36 * v30;
        if ( (a10 & 1) != 0 )
            v19 += gMovLines;
        v33 = (v35 + v35 + v19) / (unsigned int)gMov_05;
        v20 = (int *)(gMovScreen + (v35 + v35 + v19) % (unsigned int)gMov_05);
        gMovCb_07(v10, v33);
        do
        {
            LODWORD(v13) = (v29 + gMov_06 - (int)v20) / v30;
            HIDWORD(v13) = (v29 + gMov_06 - (int)v20) % v30;
            if ( a7 < (unsigned int)v13 )
                LODWORD(v13) = a7;
            if ( (int)v13 )
            {
                a7 -= v13;
                do
                {
                    v21 = a6 >> 1;
                    v27 = v13;
                    LODWORD(v13) = 0;
                    do
                    {
                        LOBYTE(v13) = *v18;
                        v18 += 2;
                        WORD2(v13) = *(short *)&gMovUnk102[2 * v13];
                        LOBYTE(v13) = *(v18 - 1);
                        HIDWORD(v13) <<= 16;
                        WORD2(v13) = *(short *)&gMovUnk102[2 * v13];
                        HIDWORD(v13) = __ROL4__(HIDWORD(v13), 16);
                        *v20++ = HIDWORD(v13);
                        --v21;
                    }
                    while ( v21 );
                    v18 += v31;
                    v20 = (int *)((char *)v20 + v29);
                    LODWORD(v13) = v27 - 1;
                }
                while ( v27 != 1 );
            }
            LODWORD(v13) = a7 | v13;
            if ( !(int)v13 )
                break;
            v22 = (gMov_06 - (int)v20) >> 2;
            if ( v22 < 0 )
                v22 = 0;
            v28 = v22;
            if ( v22 )
            {
                LODWORD(v13) = 0;
                do
                {
                    LOBYTE(v13) = *v18;
                    v18 += 2;
                    WORD2(v13) = *(short *)&gMovUnk102[2 * v13];
                    LOBYTE(v13) = *(v18 - 1);
                    HIDWORD(v13) <<= 16;
                    WORD2(v13) = *(short *)&gMovUnk102[2 * v13];
                    HIDWORD(v13) = __ROL4__(HIDWORD(v13), 16);
                    *v20++ = HIDWORD(v13);
                    --v22;
                }
                while ( v22 );
            }
            v33 += gMov_08;
            v23 = (int *)((char *)v20 - gMov_03);
            gMovCb_07(v22, v33);
            LOBYTE(v13) = v28;
            v24 = v32 - v28;
            if ( v32 != v28 )
            {
                LODWORD(v13) = 0;
                do
                {
                    LOBYTE(v13) = *v18;
                    v18 += 2;
                    WORD2(v13) = *(short *)&gMovUnk102[2 * v13];
                    LOBYTE(v13) = *(v18 - 1);
                    HIDWORD(v13) <<= 16;
                    WORD2(v13) = *(short *)&gMovUnk102[2 * v13];
                    HIDWORD(v13) = __ROL4__(HIDWORD(v13), 16);
                    *v23++ = HIDWORD(v13);
                    --v24;
                }
                while ( v24 );
            }
            v18 += v31;
            v20 = (int *)((char *)v23 + v29);
            --a7;
        }
        while ( a7 );
    }
    else
    {
        v13 = v30 * (unsigned __int64)v36;
        v14 = (int *)(v35 + v35 + v13 + gMovScreen);
        if ( (a10 & 1) != 0 )
            v14 = (int *)((char *)v14 + gMovLines);
        v15 = v34;
        v16 = a7;
        do
        {
            v17 = a6 >> 1;
            v26 = v16;
            LODWORD(v13) = 0;
            do
            {
                LOBYTE(v13) = *v15;
                v15 += 2;
                WORD2(v13) = *(short *)&gMovUnk102[2 * v13];
                LOBYTE(v13) = *(v15 - 1);
                HIDWORD(v13) <<= 16;
                WORD2(v13) = *(short *)&gMovUnk102[2 * v13];
                HIDWORD(v13) = __ROL4__(HIDWORD(v13), 16);
                *v14++ = HIDWORD(v13);
                --v17;
            }
            while ( v17 );
            v15 += v31;
            v14 = (int *)((char *)v14 + v29);
            --v16;
        }
        while ( v26 != 1 );
    }
    return v13;
*/
}

void MovProtUnk27( int a1, int a2, int a3, int a4, int a5, short *a6, int a7, int a8 )
{
/*
    int v8; // ecx
    char *v9; // esi
    char *v10; // esi
    int v12; // edx
    int v13; // ecx
    char *v14; // edi
    int v15; // ecx
    int v16; // eax
    bool v17; // cf
    bool v18; // zf
    bool v19; // cc
    int v20; // ecx
    int v21; // [esp-10h] [ebp-48h]
    int v22; // [esp-Ch] [ebp-44h]
    char *v23; // [esp-8h] [ebp-40h]
    int v24; // [esp-4h] [ebp-3Ch]
    int v25; // [esp+20h] [ebp-18h]
    int v26; // [esp+2Ch] [ebp-Ch]
    int savedregs; // [esp+38h] [ebp+0h] BYREF
    unsigned int v28; // [esp+58h] [ebp+20h]

    v25 = 8 * (unsigned __int8)gMovScrSize * gMovSurfWidth - 8 * a4;
    v26 = (unsigned __int8)gMovScrSize * gMovLines;
    if ( a1 )
        v9 = (char *)gMovScrPixelsB;
    else
        v9 = (char *)gMovScrPixelsA;
    HIWORD(v12) = ((unsigned int)gMovSurfWidth * (unsigned __int64)(unsigned int)(8 * a3)) >> 48;
    v10 = &v9[8 * a2 + gMovSurfWidth * 8 * a3];
    v28 = a7 & 0xFFFFFFFC;
    LOWORD(v12) = 0;
    LOBYTE(v8) = gMovScrSize;
    do
    {
        v24 = v8;
        v23 = v10;
        v13 = a5;
        v21 = v12;
        v14 = (char *)(gMovScreen + (v28 + a8 * gMovLines) % gMov_05);
        if ( gMovCb_07 )
            gMovCb_07(a5, (v28 + a8 * gMovLines) / gMov_05);
        v12 = v21;
        do
        {
            v22 = v13;
            if ( (unsigned int)&v14[8 * a4 - 1 + 7 * v26] >= gMov_06 )
            {
                MovProtUnk33(v12, a6, (int)&savedregs, v14, v10);
                return;
            }
            v15 = a4;
            v16 = 0;
            while ( 1 )
            {
                while ( 1 )
                {
                    while ( 1 )
                    {
                        v17 = __CFADD__((short)v12, (short)v12);
                        LOWORD(v12) = 2 * v12;
                        if ( (short)v12 )
                            break;
                        LOWORD(v12) = *a6++;
                    }
                    if ( v17 )
                        break;
                    v10 += 8;
                    v14 += 8;
                    if ( !--v15 )
                        goto LABEL_10;
                }
LABEL_22:
                v16 += 2;
                if ( !--v15 )
                    break;
                while ( 1 )
                {
                    v18 = 2 * (short)v12 == 0;
                    v19 = __CFADD__((short)v12, (short)v12) || v18;
                    LOWORD(v12) = 2 * v12;
                    if ( !v19 )
                        break;
                    if ( !v18 )
                        goto LABEL_22;
                    LOWORD(v12) = *a6++;
                }
                MovProtUnk12(v16, (int)&savedregs, v14, v10);
                v10 += 8;
                v14 += 8;
                v16 = 0;
                v15 = v20 - 1;
                if ( !v15 )
                    goto LABEL_10;
            }
            MovProtUnk12(v16, (int)&savedregs, v14, v10);
LABEL_10:
            v10 += v25;
            v14 += 7 * v26 + v26 - 8 * a4;
            v13 = v22 - 1;
        }
        while ( v22 != 1 );
        v8 = v24;
        v10 = &v23[gMovSurfWidth];
        ++a8;
        LOBYTE(v8) = v24 - 1;
    }
    while ( (_BYTE)v24 != 1 );
*/
}

void MovProtUnk12( int result, int a2, char *a3, char *a4 )
{
/*
    int v4; // ebx
    int v5; // edx
    char *v6; // esi
    char *v7; // edi
    char *v8; // esi
    char *v9; // edi
    char *v10; // esi
    char *v11; // edi
    char *v12; // esi
    char *v13; // edi
    char *v14; // esi
    char *v15; // edi
    char *v16; // esi
    char *v17; // edi

    v4 = *(int *)(a2 - 8) - 4 * result;
    v5 = *(int *)(a2 - 12) - 4 * result;
    qmemcpy(a3, a4, 4 * result);
    v6 = &a4[4 * result + v4];
    v7 = &a3[4 * result + v5];
    qmemcpy(v7, v6, 4 * result);
    v8 = &v6[4 * result + v4];
    v9 = &v7[4 * result + v5];
    qmemcpy(v9, v8, 4 * result);
    v10 = &v8[4 * result + v4];
    v11 = &v9[4 * result + v5];
    qmemcpy(v11, v10, 4 * result);
    v12 = &v10[4 * result + v4];
    v13 = &v11[4 * result + v5];
    qmemcpy(v13, v12, 4 * result);
    v14 = &v12[4 * result + v4];
    v15 = &v13[4 * result + v5];
    qmemcpy(v15, v14, 4 * result);
    v16 = &v14[4 * result + v4];
    v17 = &v15[4 * result + v5];
    qmemcpy(v17, v16, 4 * result);
    qmemcpy(&v17[4 * result + v5], &v16[4 * result + v4], 4 * result);
    return result;
*/
}

void MovProtUnk33( short a1, short *a2, int a3, char *a4, char *a5 )
{
/*
    unsigned int v6; // eax
    char *v11; // esi
    char *v20; // [esp-Ch] [ebp-10h]
    char *v21; // [esp-8h] [ebp-Ch]
    int v5; // ecx
    int v10; // ecx
    int v12; // edi
    int v15; // ecx
    int v16; // eax
    int v17; // cf
    int v18; // zf
    int v7; // cf
    int v8; // zf
    int v9; // cc
    int v19; // ecx
    short v22; // [esp-4h] [ebp-8h]
    short *v23; // [esp+0h] [ebp-4h]
    short v13; // dx
    short *v14; // ebx

    v22 = a1;
    v21 = a5;
    v20 = a4;
    v5 = *(int *)(a3 + 20);
    v6 = 0;
    while( 1 ){
        while( 1 ){
            while( 1 ){
                v7 = __CFADD__(a1, a1);
                a1 *= 2;
                if( a1 ) break;
                a1 = *a2++;
            }
            if( v7 ) break;
            a5 += 8;
            a4 += 8;
            if( !--v5 ) goto LABEL_13;
        }
LABEL_10:
        v6 += 2;
        if( !--v5 ) break;
        while( 1 ){
            v8 = 2 * a1 == 0;
            v9 = __CFADD__(a1, a1) || v8;
            a1 *= 2;
            if( !v9 ) break;
            if( !v8 ) goto LABEL_10;
            a1 = *a2++;
        }
        MovProtUnk13(v6, a3, a4, a5);
        a5 += 8;
        a4 += 8;
        v6 = 0;
        v5 = v10 - 1;
        if( !v5 ) goto LABEL_13;
    }
    MovProtUnk13(v6, a3, a4, a5);
LABEL_13:
    v11 = v21;
    *(int *)(a3 - 40) += gMov_08;
    v12 = (int)&v20[-gMov_03];
    if( gMovCb_07 ) gMovCb_07(v5, *(int *)(a3 - 40));
    v13 = v22;
    v14 = v23;
    v15 = *(int *)(a3 + 20);
    v16 = 0;
    while( 1 ){
        while( 1 ){
            while( 1 ){
                v17 = __CFADD__(v13, v13);
                v13 *= 2;
                if( v13 ) break;
                v13 = *v14++;
            }
            if( v17 ) break;
            v11 += 8;
            v12 += 8;
            if( !--v15 ) goto LABEL_27;
        }
LABEL_24:
        v16 += 2;
        if( !--v15 ) break;
        while( 1 ){
            v18 = 2 * v13 == 0;
            v9 = __CFADD__(v13, v13) || v18;
            v13 *= 2;
            if( !v9 ) break;
            if( !v18 ) goto LABEL_24;
            v13 = *v14++;
        }
        MovProtUnk14(v16, a3, v12);
        v11 += 8;
        v12 += 8;
        v16 = 0;
        v15 = v19 - 1;
        if( !v15 ) goto LABEL_27;
    }
    MovProtUnk14(v16, a3, v12);
LABEL_27:
    JUMPOUT(0x4FC40D);
*/
}

void MovProtUnk13( unsigned int result, int a2, char *a3, char *a4 )
{
/*
    unsigned int v4, v5;

    v4 = *(int *)(a2 - 8) - 4 * result;
    v5 = *(int *)(a2 - 12) - 4 * result;
    *(int *)(a2 - 44) = gMov_06 - 4 * result;
    if( (int)&a3[-*(int *)(a2 - 44)] >= 0 ) goto LABEL_10;
    memcpy(a3, a4, 4 * result); a4 += 4 * result + v4; a3 += 4 * result + v5;
    if( (int)&a3[-*(int *)(a2 - 44)] >= 0 ) goto LABEL_10;
    memcpy(a3, a4, 4 * result); a4 += 4 * result + v4; a3 += 4 * result + v5;
    if( (int)&a3[-*(int *)(a2 - 44)] >= 0 ) goto LABEL_10;
    memcpy(a3, a4, 4 * result); a4 += 4 * result + v4; a3 += 4 * result + v5;
    if( (int)&a3[-*(int *)(a2 - 44)] >= 0 ) goto LABEL_10;
    memcpy(a3, a4, 4 * result); a4 += 4 * result + v4; a3 += 4 * result + v5;
    if( (int)&a3[-*(int *)(a2 - 44)] >= 0 ) goto LABEL_10;
    memcpy(a3, a4, 4 * result); a4 += 4 * result + v4; a3 += 4 * result + v5;
    if( (int)&a3[-*(int *)(a2 - 44)] >= 0 ) goto LABEL_10;
    memcpy(a3, a4, 4 * result); a4 += 4 * result + v4; a3 += 4 * result + v5;
    if( (int)&a3[-*(int *)(a2 - 44)] >= 0 ) goto LABEL_10;
    memcpy(a3, a4, 4 * result); a4 += 4 * result + v4; a3 += 4 * result + v5;
    if( (int)&a3[-*(int *)(a2 - 44)] >= 0 ) goto LABEL_10;
    memcpy(a3, a4, 4 * result);
    return;
LABEL_10:
    if( gMov_06 - (int)a3 < 0 ) return;
    memcpy(a3, a4, 4 * ((unsigned int)(gMov_06 - (int)a3) >> 2));
*/
}

void MovProtUnk14( int result, int a2, int a3 )
{
/*
    int v3; // edx
    int v4; // edi
    int v5; // edi
    int v6; // edi
    int v7; // edi
    int v8; // edi
    int v9; // edi
    int v10; // edi
    int v11; // edi

    *(int *)(a2 - 44) = -4 * result;
    v3 = *(int *)(a2 - 12);
    v4 = a3 - gMovScreen;
    if ( v4 < 0 && *(int *)(a2 - 44) - v4 >= 0 )
    {
        v5 = v3 + v4;
        if ( v5 < 0 && *(int *)(a2 - 44) - v5 >= 0 )
        {
            v6 = v3 + v5;
            if ( v6 < 0 && *(int *)(a2 - 44) - v6 >= 0 )
            {
                v7 = v3 + v6;
                if ( v7 < 0 && *(int *)(a2 - 44) - v7 >= 0 )
                {
                    v8 = v3 + v7;
                    if ( v8 < 0 && *(int *)(a2 - 44) - v8 >= 0 )
                    {
                        v9 = v3 + v8;
                        if ( v9 < 0 && *(int *)(a2 - 44) - v9 >= 0 )
                        {
                            v10 = v3 + v9;
                            if ( v10 < 0 && *(int *)(a2 - 44) - v10 >= 0 )
                            {
                                v11 = v3 + v10;
                                if ( v11 < 0 && *(int *)(a2 - 44) - v11 >= 0 )
                                    result *= 4;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
*/
}

void MovProtUnk28( int a1, int a2, int a3, int a4, int a5, short *a6, int a7, int a8 )
{
/*
    int v8; // ecx
    char *v9; // esi
    char *v10; // esi
    int v12; // edx
    int v13; // ecx
    char *v14; // edi
    int v15; // ecx
    int v16; // eax
    bool v17; // cf
    bool v18; // zf
    bool v19; // cc
    int v20; // ecx
    int v21; // [esp-10h] [ebp-48h]
    int v22; // [esp-Ch] [ebp-44h]
    char *v23; // [esp-8h] [ebp-40h]
    int v24; // [esp-4h] [ebp-3Ch]
    int v25; // [esp+20h] [ebp-18h]
    int v26; // [esp+2Ch] [ebp-Ch]
    int savedregs; // [esp+38h] [ebp+0h] BYREF
    unsigned int v28; // [esp+58h] [ebp+20h]

    v25 = 8 * (unsigned __int8)gMovScrSize * gMovSurfWidth - 16 * a4;
    v26 = (unsigned __int8)gMovScrSize * gMovLines;
    if( a1 )
        v9 = (char *)gMovScrPixelsB;
    else
        v9 = (char *)gMovScrPixelsA;
    HIWORD(v12) = ((unsigned int)gMovSurfWidth * (unsigned __int64)(unsigned int)(8 * a3)) >> 48;
    v10 = &v9[16 * a2 + gMovSurfWidth * 8 * a3];
    v28 = a7 & 0xFFFFFFFC;
    LOWORD(v12) = 0;
    LOBYTE(v8) = gMovScrSize;
    do{
        v24 = v8;
        v23 = v10;
        v13 = a5;
        v21 = v12;
        v14 = (char *)(gMovScreen + (v28 + a8 * gMovLines) % gMov_05);
        if( gMovCb_07 ) gMovCb_07(a5, (v28 + a8 * gMovLines) / gMov_05);
        v12 = v21;
        do{
            v22 = v13;
            if( (unsigned int)&v14[16 * a4 - 1 + 7 * v26] >= gMov_06 ){
                MovProtUnk32(v12, a6, (int)&savedregs, v14, v10);
                return;
            }
            v15 = a4;
            v16 = 0;
            while( 1 ){
                while( 1 ){
                    while( 1 ){
                        v17 = __CFADD__((short)v12, (short)v12);
                        LOWORD(v12) = 2 * v12;
                        if( (short)v12 ) break;
                        LOWORD(v12) = *a6++;
                    }
                    if( v17 ) break;
                    v10 += 16;
                    v14 += 16;
                    if( !--v15 ) goto LABEL_10;
                }
LABEL_22:
                v16 += 4;
                if( !--v15 ) break;
                while( 1 ){
                    v18 = 2 * (short)v12 == 0;
                    v19 = __CFADD__((short)v12, (short)v12) || v18;
                    LOWORD(v12) = 2 * v12;
                    if( !v19 ) break;
                    if( !v18 ) goto LABEL_22;
                    LOWORD(v12) = *a6++;
                }
                MovProtUnk15(v16, (int)&savedregs, v14, v10);
                v10 += 16;
                v14 += 16;
                v16 = 0;
                v15 = v20 - 1;
                if( !v15 ) goto LABEL_10;
            }
            MovProtUnk15(v16, (int)&savedregs, v14, v10);
LABEL_10:
            v10 += v25;
            v14 += 7 * v26 + v26 - 16 * a4;
            v13 = v22 - 1;
        }
        while ( v22 != 1 );
        v8 = v24;
        v10 = &v23[gMovSurfWidth];
        ++a8;
        LOBYTE(v8) = v24 - 1;
    }
    while ( (_BYTE)v24 != 1 );
*/
}

void MovProtUnk15( int result, int a2, char *a3, char *a4 )
{
/*
    int v4, v5;
    char *v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17;

    v4 = a2[ -2 ] - 4 * result; v5 = a2[ -3 ] - 4 * result; memcpy(a3, a4, 4 * result);
    v6 = &a4[4 * result + v4]; v7 = &a3[4 * result + v5]; memcpy(v7, v6, 4 * result);
    v8 = &v6[4 * result + v4]; v9 = &v7[4 * result + v5]; memcpy(v9, v8, 4 * result);
    v10 = &v8[4 * result + v4]; v11 = &v9[4 * result + v5]; memcpy(v11, v10, 4 * result);
    v12 = &v10[4 * result + v4]; v13 = &v11[4 * result + v5]; memcpy(v13, v12, 4 * result);
    v14 = &v12[4 * result + v4]; v15 = &v13[4 * result + v5]; memcpy(v15, v14, 4 * result);
    v16 = &v14[4 * result + v4]; v17 = &v15[4 * result + v5]; memcpy( v17, v16, 4 * result );
    memcpy( &v17[4 * result + v5], &v16[4 * result + v4], 4 * result );
*/
}

void MovProtUnk32( short a1, short *a2, int a3, char *a4, char *a5 )
{
/*
    int v5; // ecx
    unsigned int v6; // eax
    bool v7; // cf
    bool v8; // zf
    bool v9; // cc
    int v10; // ecx
    char *v11; // esi
    int v12; // edi
    short v13; // dx
    short *v14; // ebx
    int v15; // ecx
    int v16; // eax
    bool v17; // cf
    bool v18; // zf
    int v19; // ecx
    char *v20; // [esp-Ch] [ebp-10h]
    char *v21; // [esp-8h] [ebp-Ch]
    short v22; // [esp-4h] [ebp-8h]
    short *v23; // [esp+0h] [ebp-4h]

    v22 = a1;
    v21 = a5;
    v20 = a4;
    v5 = *(int *)(a3 + 20);
    v6 = 0;
    while( 1 ){
        while( 1 ){
            while( 1 ){
                v7 = __CFADD__(a1, a1);
                a1 *= 2;
                if( a1 ) break;
                a1 = *a2++;
            }
            if( v7 ) break;
            a5 += 16;
            a4 += 16;
            if( !--v5 ) goto LABEL_13;
        }
LABEL_10:
        v6 += 4;
        if( !--v5 ) break;
        while( 1 ){
            v8 = 2 * a1 == 0;
            v9 = __CFADD__(a1, a1) || v8;
            a1 *= 2;
            if( !v9 ) break;
            if( !v8 ) goto LABEL_10;
            a1 = *a2++;
        }
        MovProtUnk16(v6, a3, a4, a5);
        a5 += 16;
        a4 += 16;
        v6 = 0;
        v5 = v10 - 1;
        if( !v5 ) goto LABEL_13;
    }
    MovProtUnk16(v6, a3, a4, a5);
LABEL_13:
    v11 = v21;
    *(int *)(a3 - 40) += gMov_08;
    v12 = (int)&v20[-gMov_03];
    if( gMovCb_07 ) gMovCb_07(v5, *(int *)(a3 - 40));
    v13 = v22;
    v14 = v23;
    v15 = *(int *)(a3 + 20);
    v16 = 0;
    while ( 1 ){
        while ( 1 ){
            while( 1 ){
                v17 = __CFADD__(v13, v13);
                v13 *= 2;
                if( v13 ) break;
                v13 = *v14++;
            }
            if( v17 ) break;
            v11 += 16;
            v12 += 16;
            if( !--v15 ) goto LABEL_27;
        }
LABEL_24:
        v16 += 4;
        if( !--v15 ) break;
        while( 1 ){
            v18 = 2 * v13 == 0;
            v9 = __CFADD__(v13, v13) || v18;
            v13 *= 2;
            if( !v9 ) break;
            if( !v18 ) goto LABEL_24;
            v13 = *v14++;
        }
        MovProtUnk17(v16, a3, v12);
        v11 += 16;
        v12 += 16;
        v16 = 0;
        v15 = v19 - 1;
        if( !v15 ) goto LABEL_27;
    }
    MovProtUnk17(v16, a3, v12);
LABEL_27:
    JUMPOUT(0x4FC860);
*/
}

void MovProtUnk16( unsigned int Size, int a2, char *pDst, char *pSrc )
{
/*
    unsigned int v4,v5,v6;

    Size = 4 * Size;
    v5 = Size + a2[ -3 ] - Size;
    v4 = Size + a2[ -2 ] - Size;
    a2[ -11 ] = gMov_06 - Size;
    if( pDst - a2[ -11 ] < 0 ){
	memcpy( pDst, pSrc, Size ); pSrc += v4; pDst += v5;
	if( pDst - a2[ -11 ] < 0 ){
	    memcpy( pDst, pSrc, Size ); pSrc += v4; pDst += v5;
	    if( pDst - a2[ -11 ] < 0 ){
		memcpy( pDst, pSrc, Size ); pSrc += v4; pDst += v5;
		if( pDst - a2[ -11 ] < 0 ){
		    memcpy( pDst, pSrc, Size ); pSrc += v4; pDst += v5;
		    if( pDst - a2[ -11 ] < 0 ){
			memcpy( pDst, pSrc, Size ); pSrc += v4; pDst += v5;
			if( pDst - a2[ -11 ] < 0 ){
			    memcpy( pDst, pSrc, Size ); pSrc += v4; pDst += v5;
			    if( pDst - a2[ -11 ] < 0 ){
				memcpy( pDst, pSrc, Size ); pSrc += v4; pDst += v5;
				if( pDst - a2[ -11 ] < 0 ){
		    		    memcpy( pDst, pSrc, Size );
				    return;
				}
			    }
			}
		    }
		}
	    }
	}
    }
    if( gMov_06 - pDst >= 0 ) memcpy( pDst, pSrc, gMov_06 - pDst );
*/
}

void MovProtUnk17( int ww, char *a2, int a3 )
{
/*
    int n;

    a2[ -11 ] = -4 * ww;
    n = a2[ -3 ] - gMovScreen;
    if( n >= 0 || a2[ -11 ] - n < 0 ) return ww; 
    n += a2[ -3 ];
    if( n >= 0 || a2[ -11 ] - n < 0 ) return ww; 
    n += a2[ -3 ];
    if( n >= 0 || a2[ -11 ] - n < 0 ) return ww; 
    n += a2[ -3 ];
    if( n >= 0 || a2[ -11 ] - n < 0 ) return ww; 
    n += a2[ -3 ];
    if( n >= 0 || a2[ -11 ] - n < 0 ) return ww; 
    n += a2[ -3 ];
    if( n >= 0 || a2[ -11 ] - n < 0 ) return ww; 
    n += a2[ -3 ];
    if( n >= 0 || a2[ -11 ] - n < 0 ) return ww; 
    n += a2[ -3 ];
    if( n >= 0 || a2[ -11 ] - n < 0 ) return ww;
    return ww *= 4;    
*/
}

#ifdef REALMODE
void MovProtUnk18( char *a1, unsigned int a2, unsigned int a3 )
{
    unsigned int v4;

    v4 = a3;
    if( a2 >= 256 ) return;
    if( a3 + a2 > 0x100 ) v4 = 256 - a2;
    outbyte( 0x3C8, a2 );
    outbytestring( 0x3C9, a1[ 3 * a2 ], 3 * v4 );
}

#endif

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

#ifdef REALMODE

int MovProtUnk29( int a1 )
{
    int result;

    result = a1;
    asm{ int     10h; - VIDEO - }
    return result;
}

void MovProtUnk30( int a1, char a2, unsigned char a3 )
{
    int i;
    
    outword( 0x3C4, 0x04 | (a2 << 8) );
    while( (inbyte( 0x3DA ) & 0x08 ) );
    while( !(inbyte( 0x3DA ) & 0x08 ) );
    disable();
    outword( 0x3C4, 0x100 );
    outbyte( 0x3C2, a3 );
    outword( 0x3C4, 0x300 );
    outword( 0x3D4, 0x311 );
    for( i = 0; i < 24; i++ ) outword( 0x3D4, i | a1[ i ] << 8 );    
    enable();
}

void MovProtUnk31( char *a1 )
{
    int i;

    for( i = 0; i < 24; i++ ){
        outbyte( 0x3d4, i );
        a1[ i ] = inbyte( 0x3d5 );
    }
}

void MovProtUnk20( char a1, Mov01_t *p )
{
    while( (inbyte( 0x3DA ) & 0x08) );
    while( !(inbyte( 0x3DA ) & 0x08) );
    disable();
    outword( 0x3C4, 0x100 );
    outbyte( 0x3C2, a1 & 0xC0 | inbyte( 0x3CC ) & 0x3F );
    outword( 0x3C4, 0x300 );
    outbyte( 0x3D4, 0x11 );
    outword( 0x3D4, 17 | ( inbyte( 0x3D5 ) << 8) & ~0x8000 );
    outbyte( 0x3D4, 0x03 );
    outword( 0x3D4, 0x03 | ( inbyte( 0x3D5 ) << 8) | 0x800 );
    outword( 0x3D4, 0x06 |(p->M1 << 8) );
    outbyte( 0x3D4, 0x07 );
    outword( 0x3D4, 0x07 | (p->M2 << 8) | (inbyte( 0x3D5 ) << 8) & 0x1000 );
    outbyte( 0x3D4, 0x09 );
    outword( 0x3D4, 0x09 | (p->M3 << 8) | (inbyte( 0x3D5 ) << 8) & 0x4000 );
    outword( 0x3D4, 0x10 | p->M4 << 8 );
    outbyte( 0x3D4, 0x11 );
    outword( 0x3D4, 0x11 | (p->M5 << 8) | ( inbyte( 0x3D5 ) << 8 ) & 0x7000 | 0x8000 );
    outword( 0x3D4, 18 | ( p->M6 << 8 ) );
    outword( 0x3D4, 21 | ( p[1].i01 >> 8 ) );
    outword( 0x3D4, 22 | ( p[1].i01 >> 16 ) );
    enable();
}

void MovProtUnk21( int a1 )
{
    if( a1 ){
        while( !(inbyte( 0x3DA ) & 0x08 ) );
    } else {
        while( inbyte( 0x3DA ) & 0x08 );
    }
}

void MovProtUnk22( unsigned int a1 )
{
    while( !(inbyte( 0x3DA ) & 0x08 ) );
    while( !( inbyte( 0x3DA ) & 0x08 ) );
    outbyte( 0x3D4, 7 );
    outword( 0x3D4, 7 | ((a1 >> 4) & 0x10) | (inbyte( 0x3D5 ) & 0xEF) );
    outbyte( 0x3D4, 9 );
    outword( 0x3D4, 9 | ((a1 >> 3) & 0x40) | HIBYTE( inbyte( 0x3D5 ) ) & 0xBF );
    outword( 0x3D4, 24 | (a1 << 16) );
}

#endif

