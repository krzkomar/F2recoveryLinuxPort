#pragma once

// field Type:
enum{
    M_UNSPEC,	// 0
    M_STATIC,	// 1
    M_ANIMATED,	// 2
};

typedef struct 
{
  char *Data;
  int CentX;
  int CentY;
  int Width;
  int Height;
} MseMgrStatic_t;

typedef struct 
{
    char **Data;
    char **bmp;
    int *CentX;
    int *CentY;
    int Width;
    int Height;
    float Fps;
    int Time;
    int Align;
    unsigned char cnt1;
    unsigned char cnt2;
    unsigned char cnt3;
} MseMgrAnimated_t;

typedef union{
    MseMgrStatic_t stat;
    MseMgrAnimated_t anim;
} MseMgrCursor_t;


typedef struct // size of 844
{
    MseMgrCursor_t *Cursor;
    int Type;
    Pal8_t Pal[ 256 ];
    int i01;
    char Name1[ 32 ];	// #1 filename
    char Name2[ 32 ];	// #2 filename
} MseMgr_t;

char *MouseMgrHandlerDefaultCb( char *fname );
int MouseMgrTimeResDefaultCb();
int MouseMgrGetTime();
int MouseMgrSetCursor( char *Data, int w, int h, int pitch, int x0, int y0, char alpha );
void MouseMgrSetHandler( char *(*HandlerCb)( char *) );
void MouseMgrSetupTime( int (*TimeResCb)(void), int (*TimeGetCb)(void) );
void MouseMgrDelCache( MseMgr_t *mse );
void MouseMgrFlushCache();
int MouseMgrInsertCache( MseMgrCursor_t **Cursor, int Type, Pal8_t *Palette, char *Name );
MseMgr_t *MouseMgrGetCache( char *fname, Pal8_t **pPal, int *pCentX, int *pCentY, int *pWidth, int *pHeight, int *Type );
void MouseMgrResetSpeed();
void MouseMgrFree();
void MouseMgrAnimate();
int MouseMgrLoadAnim( char *fname, int StartFrame );
int MouseMgrLoadMou( char *fname, int CentX, int CentY );
int MouseMgrLoadCursor( char *fname );
void MouseMgrCursorUpdate();
void MouseMgrUnk10();
void MouseMgrRedraw();
void MouseMgrDrawCursor();




