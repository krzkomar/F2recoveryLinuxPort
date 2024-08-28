#include "FrameWork.h"

#define TASK_STOP		0x01
#define INP_BUFF_SIZE		40
#define PAUSEDIALOG_TEXT_COLOR 	gPalColorCubeRGB[31][0][0]
#define PAUSE_ESC_KEY		27
#define SCRSHOT_HKEY		302
#define PAUSE_HKEY		281

static int gInpScrShotHKey;
static int (*gInpScrShotSave)( int Width, int Height, char *Pal, char *BmpData );
static char *gInpScrShotPixBuf;
static KbdEv_t	gKbdEv[ INP_BUFF_SIZE ];
static int gInpMouseX;
static int gInpMouseY;
static int gInpPaused;
static int gInp_01; // unused
static int gInpPauseKey;
static int gInpTail;
static int (*gInpPauseDlgInit)();
static int gInpHead;
static int gInpTaskHalt;
static InpTask_t *gInpTaskList;

int InpOpen( int unused )
{
    if( InputInit() != 1) return -1;
    if( KeyInit() == -1 ) return -1;
    if( MseInit() == -1 ) return -1;
    KbdClose( 1 );
    KbdTrLang();
    KbdTimesReset();

    gInp_01 = unused;
    gInpHead = 0;
    gInpTail = -1;
    gInpMouseX = -1;
    gInpMouseY = -1;
    gInpTaskHalt = 0;
    gInpPaused = 0;
    gInpPauseKey = PAUSE_HKEY;
    gInpPauseDlgInit = InpPauseDialog;
    gInpScrShotSave = 0;//SaveBMP;
    gInpTaskList = NULL;
    gInpScrShotHKey = SCRSHOT_HKEY;
    return 0;
}

void InpClose()
{
    InpTask_t *p, *q;

    KbdCloseA( 0 );
    KbdNull01();
    MseClose();
    KeyOff();
    InputRelease();
    p = gInpTaskList;
    while( p ){
	q = p->Next;
        Free( p );
        p = q;
    }
}

int InpUpdate()
{
    int n;

    VidUpdate();
    InputPoll();
    KbdUpdate();
    if( !gSDLReady ) KbdThreadSync();
    InpWinUpdate();    
    n = InpPopBuffer();
    if( n == -1 ){
	if( MseGetButtons() & ( MSE_LBUTT | MSE_RBUTT | MSE_LBHOLD | MSE_RBHOLD ) ){
    	    MseGetCursorPosition( &gInpMouseX, &gInpMouseY );
    	    return -2;
        }
    }    
    return WinDialogRun( n );
}

void InpGetMouse( int *Xpos, int *Ypos )
{
    *Xpos = gInpMouseX;
    *Ypos = gInpMouseY;
}

void InpWinUpdate()
{
    int key;
    
    InpTaskProceed();
    if( RecRecord() != 3 ) MseUpdate();
    if( ( key = WinCursorProcess() ) == -1 ){
	if( (key = KeyGet()) == -1 ) return;
    }
    InpPushBuffer( key );
}

void InpPushBuffer( int Key )
{
    if( Key == -1 ) return;    
    if( Key == gInpPauseKey ){
        InpPause();
    } else if( Key == gInpScrShotHKey ){
        InpScrShotTake();
    } else if( gInpHead != gInpTail ){
        gKbdEv[ gInpHead ].KbdKey = Key;
        MseGetCursorPosition( &gKbdEv[ gInpHead ].MouseX, &gKbdEv[ gInpHead ].MouseY );
        if( ++gInpHead == INP_BUFF_SIZE ){
            gInpHead = 0;
        } else if( gInpTail == -1 ){
            gInpTail = 0;
        }
    }    
}

int InpPopBuffer()
{
    int tmp, KbdKey;
    unsigned int MouseY;

    tmp = gInpTail;
    KbdKey = -1;
    if( gInpTail != -1 ){
        tmp = gInpTail + 1;
        KbdKey = gKbdEv[ gInpTail ].KbdKey;
        MouseY = gKbdEv[ gInpTail ].MouseY;
        gInpMouseX = gKbdEv[ gInpTail ].MouseX;
        gInpMouseY = MouseY;
        if( gInpTail == (INP_BUFF_SIZE - 1) )
            tmp = 0;
        if( tmp == gInpHead ){
            tmp = -1;
            gInpHead = 0;
        }
    }
    gInpTail = tmp;
    return KbdKey;
}

void InpFlushBuffer()
{
    gInpTail = -1;
    gInpHead = 0;
}

void InpTaskProceed()
{
    int time;
    InpTask_t **prev, *p, *t;

    if( gInpPaused || gInpTaskHalt ) return;    
    time = TimerGetSysTime();
    prev = &gInpTaskList;
    p = gInpTaskList;
    gTimerTime = time;
    while( p ){
        t = p->Next;
        if( p->Flags & TASK_STOP ){
            *prev = t;
            Free( p );
        } else {
            p->Task(); // Run
            prev = &p->Next;
        }
        p = t;
    }    
}

InpTask_t *InpTaskStart( void (*CallBack)() )
{
    InpTask_t *NewCall, *p;
    // test if already in queue
    for( NewCall = gInpTaskList; NewCall; NewCall = NewCall->Next ){
        if( CallBack == NewCall->Task ){
    	    if( NewCall->Flags & TASK_STOP ) NewCall->Flags &= ~TASK_STOP; // run a task again
    	    return NewCall;
        }        
    }
    // not existed, create new one
    NewCall = Malloc( sizeof( InpTask_t ) );
    if( NewCall ) {
        NewCall->Flags = 0;
        NewCall->Task = CallBack;
        p = gInpTaskList;
        gInpTaskList = NewCall;
        NewCall->Next = p;
    }    
    return NewCall;
}

void InpTaskStop( void (*CallBack)() )
{
    InpTask_t *task;
    
    for( task = gInpTaskList; task; task = task->Next ){
        if( CallBack == task->Task ){
    	    task->Flags |= TASK_STOP;
    	    break;
        }        
    }
}

void InpTaskHalt()
{
    gInpTaskHalt = 0;
}

void InpTaskUnHalt()
{
    gInpTaskHalt = 1;
}

void InpPause()
{
    int WinId;
    
    if( gInpPaused ) return;
    gInpPaused = 1;
    WinId = gInpPauseDlgInit();
    while( InpUpdate() != PAUSE_ESC_KEY );
    gInpPaused = 0;
    WinClose( WinId );
}

int InpPauseDialog()
{
    int w, h, win = 0;
    char *Surface;

    w = gFont.LineWidth("Paused") + 32;
    h = 3 * gFont.ChrHeight() + 16;
    win = WinCreateWindow( (gVidMainGeo.rt - gVidMainGeo.lt + 1 - w) / 2, (gVidMainGeo.bm - gVidMainGeo.tp + 1 - h) / 2, w, h, 256, 20 );
    if( win == -1 ) return -1;
    WinDrawFrame( win );
    Surface = WinGetSurface(win);
    gFont.Print( &Surface[ 8 * w + 16 ], "Paused", w, w, PAUSEDIALOG_TEXT_COLOR );
    h -= 8 - gFont.ChrHeight();
    w -= gFont.LineWidth( "Done" );
    WinAddButton( win, ((w - 16) / 2), h - 6, -1, -1, -1, PAUSE_ESC_KEY, "Done", 0 );
    WinUpdate( win );    

    return win;
}

void InpPauseInit( int PauseHKey, int (*PauseCb)(void) )
{
    gInpPauseKey = PauseHKey;
    if( PauseCb == NULL ) PauseCb = InpPauseDialog;
    gInpPauseDlgInit = PauseCb;
}

void InpScrShotTake()
{
/*
    int Width;
    void (*SaveA)( void *, int, int, int, int, int, int, int, int );
    void (*SaveB)( void *, int, int, int, int, int, int, int, int );
    void (*SaveC)( void *, int, int, int, int, int, int, int, int, char );
    int Height;
    Width = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    gInpScrShotPixBuf = Malloc((gVidMainGeo.bm - gVidMainGeo.tp + 1) * Width);
    if( !gInpScrShotPixBuf ) return;    
    SaveA = gVidCopyA;
    SaveB = VidCopy;
    SaveC = gMseBlitAlpha;        
    
    gVidCopyA     = InpScrShotBlit;
    gMseBlit      = InpScrShotBlit;
    gMseBlitAlpha = NULL;

    WinUpdateArea( &gVidMainGeo );

    gMseBlitAlpha = SaveC;
    gMseBlit      = SaveB;
    gVidCopyA     = SaveA;

    gInpScrShotSave( Width, Height, gInpScrShotPixBuf, PalGetCurrent() );
    Free( gInpScrShotPixBuf );
*/
DD
}

void InpScrShotBlit( void *Screen, int SrcPitch, int Unk, int SrcX, int SrcY, int Width, int Height, int Xpos, int Ypos )
{
    ScrCopy(Screen + SrcX + SrcPitch * SrcY, Width, Height, SrcPitch, &gInpScrShotPixBuf[Xpos + (gVidMainGeo.rt - gVidMainGeo.lt + 1) * Ypos], gVidMainGeo.rt - gVidMainGeo.lt + 1);
}

int InpSaveBMP( int Width, int Height, char *BitmapData, char *PaletteData )
{
    FILE *fh;
    char *p;
    char fname[16];
    int tmp, i, buffer;
    char a,r,g,b;

    for( i = 0; i < 100000; i++ ){
        sprintf( fname, "scr%.5d.bmp", i );
        fh = fopen( fname, "rb" );
        if( !fh ) break;
        fclose( fh );
    }
    if( i == 100000 ) return -1;
    fh = fopen(fname, "wb");
    if( !fh ) return -1;
    tmp = 0x4D42;                               // 'BM'
    fwrite(&tmp, 2u, 1u, fh);
    buffer = Height * Width + 1078;             // size of BMP file in bytes
    fwrite(&buffer, 4u, 1u, fh);
    tmp = 0;
    fwrite(&tmp, 2u, 1u, fh);                   // Rsvd = 0
    tmp = 0;
    fwrite(&tmp, 2u, 1u, fh);                   // Rsvd = 0
    tmp = 1078;
    fwrite(&buffer, 4u, 1u, fh);                // offset data = 1078
    tmp = 40;
    fwrite(&buffer, 4u, 1u, fh);                // / Size of DiB header = 40
    tmp = Width;
    fwrite(&tmp, 4u, 1u, fh);                   // Bitmap width in pixels
    tmp = Height;
    fwrite(&tmp, 4u, 1u, fh);                   // Bitmap heiight in pixells
    tmp = 1;
    fwrite(&tmp, 2u, 1u, fh);                   // color planes = 1
    tmp = 8;
    fwrite(&tmp, 2u, 1u, fh);                   // bits per pixel = 8
    tmp = 0;
    fwrite(&tmp, 4u, 1u, fh);                // Compression method = 0 (none)
    fwrite(&tmp, 4u, 1u, fh);                // image_size = 0 (dummy for BI_RGB)
    fwrite(&tmp, 4u, 1u, fh);                   // horizontal  resolution pix/m
    fwrite(&tmp, 4u, 1u, fh);                   // vertical resolution pix/m
    fwrite(&tmp, 4u, 1u, fh);                // number of colors in the color palette, or 0 to default 2^n
    fwrite(&tmp, 4u, 1u, fh);                // every color is important
    // zapis palety kolorow
    for(i = 0; i < 256; i++, PaletteData += 3 ){
        a = 0;
        r = 4 * PaletteData[0];
        g = 4 * PaletteData[1];
        b = 4 * PaletteData[2];
        fwrite( &b, 1u, 1u, fh );
        fwrite( &g, 1u, 1u, fh );
        fwrite( &r, 1u, 1u, fh );
        fwrite( &a, 1u, 1u, fh );
    }
    // zapis danych obrazu
    p = &BitmapData[i * Width];
    for( i = Height - 1; i >= 0; i-- ){
        fwrite(p, 1u, Width, fh);
        p += -Width;
    }
    fflush( fh );
    fclose( fh );
    return 0;
}

void InpScrShotInit( int hk, int (*bmp)(int Width, int Height, char *PaletteData, char *BitmapData))
{
    gInpScrShotHKey = hk;
    if( !bmp ) bmp = InpSaveBMP;
    gInpScrShotSave = bmp;
}


