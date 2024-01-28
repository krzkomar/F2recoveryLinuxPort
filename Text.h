#include "FrameWork.h"

typedef struct 
{
    int Flags;
    Obj_t *obj;
    int Time;
    int LinesCnt;
    int offsx;
    int offsy;
    int GridPos;
    int x;
    int y;
    int w;
    int h;
    char *bmp;
} Text_t;

/*
int gTextCount;
int gTextBaseDelay;
int gTextLineDelay;
Text_t *gTextList[ 20 ];
int gTextWidth;
int gTextHeight; 
char *gTextSurface;
int gTextEvEnable;
int gTextEnable;
*/

int TextInit( char *Surface, int Width, int Height );
int TextFlush();
void TextClose();
void TextEvDisable();
void TextEvEnable();
int TextEvStatus();
void TextSetBaseDelay( double val );
void TextSetLineDelay( double var );
int TextFloatMessage( Obj_t *obj, char *Text, int Font, int ColorA, int ColorB, VidRect_t *Area );
void TextRender( VidRect_t *area );
int TextCount();
void TextUnk09( Text_t *text );
void TextUnk10( Obj_t *obj );


