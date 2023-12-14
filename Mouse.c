#include "FrameWork.h"

void (*gMseBlitAlpha)( void *SrcData, int pitch, int unk, int SrcX, int SrcY, int Width, int Height, int Xpos, int Ypos, char TransColor );
void (*gMseBlit)( void *src, int pitch, int unk, int SrcX, int SrcY, int Width, int Height, int Xpos, int Ypos );

static char gMseCursorArrow[ 8*8 ] = {
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, // "AAAAAAA "
    0x01, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x01, 0x00, // "A+++++A "
    0x01, 0x0F, 0x0F, 0x0F, 0x0F, 0x01, 0x01, 0x00, // "A++++AA "
    0x01, 0x0F, 0x0F, 0x0F, 0x0F, 0x01, 0x01, 0x00, // "A++++AA "
    0x01, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x01, 0x01, // "A+++++AA"
    0x01, 0x0F, 0x01, 0x01, 0x0F, 0x0F, 0x0F, 0x01, // "A+AA+++A"
    0x01, 0x01, 0x01, 0x01, 0x01, 0x0F, 0x0F, 0x01, // "AAAAA++A"
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01  // "    AAAA"
};

static int 	gMseSampleTime = 0;
static char	*gMseCursorPic = NULL; 		// static cursor data
static char 	*gMseAnimCursorFrame = NULL;	// current displayed frame
static char	*gMseAnimCursorFrames = NULL;   // animated cursor data
static double 	gMseSpeed = 1.0;
static int 	gMseAnimTimer = 0;
static int 	gMseButtonsOld = 0;

static int 	gMseCursorClear;
static int 	gMseOldX;
static int 	gMseHeight;
static int 	gMseOldY;
static int 	gMseButtStatCur;
static int 	gMseY;
static int 	gMseX;
static int 	gMseDisable;
static int 	gMseButtStat;
static int 	gMseAnimRate;
static int 	gMseAnimFrameNo;
static int 	gMseAcq;
static int 	gMsePitch;
static int 	gMseWidth;
static int 	gMseAnimFrameCnt;
static int 	gMseCenterY;
static int 	gMseCenterX;
static int 	gMseTimeEv;
static char 	gMseAlpha;
static int 	gMseRbuttTimer;
static int 	gMseLbuttTimer;


int MseInit()
{
    gMseAcq = 0;
    gMseDisable = 0;
    gMseCursorClear = 1;
    MseCursorInit();
    if( MseSetStaticCursor( NULL, 0, 0, 0, 0, 0, 0 ) == -1 ) return -1;
    if( InputMouseAcquire() != 1 ) return -1;
    gMseAcq = 1;
    gMseOldX = gMseX = gVidMainGeo.rt / 2;
    gMseOldY = gMseY = gVidMainGeo.bm / 2;
    gMseTimeEv = TimerGetSysTime();
    return 0;
}

void MseClose()
{
    InputMouseUnlink();
    if( gMseCursorPic ) Free(gMseCursorPic);
    gMseCursorPic = 0;
    if( gMseAnimCursorFrames ) InpTaskStop( MseUpdateAnimatedCursor );
    gMseAnimCursorFrames = 0;
}

void MseCursorInit()
{
    int i;

    for( i = 0; i < 64; i++ ){
	switch( gMseCursorArrow[ i ] ){
	    case 0x0f: gMseCursorArrow[ i ] = MSE_CURSOR_COLOR_FG; break;
	    case 0x01: gMseCursorArrow[ i ] = MSE_CURSOR_COLOR_BG; break;    
	    default:   gMseCursorArrow[ i ] = MSE_CURSOR_COLOR_ALPHA; break;
	}	
    }
}

void MseGetStaticCursor( char **Data, int *Width, int *Height, int *Pitch, int *Xcenter, int *Ycenter, char *Alpha )
{
    *Data = gMseAnimCursorFrame;
    *Width = gMseWidth;
    *Height = gMseHeight;
    *Pitch = gMsePitch;
    *Xcenter = gMseCenterX;
    *Ycenter = gMseCenterY;
    *Alpha = gMseAlpha;
}

int MseSetStaticCursor( char *Data, int Width, int Height, int Pitch, int Xcenter, int Ycenter, char Alpha )
{
    VidRect_t MouseArea;
    char *p;
    char *pic;
    int clean;

    p = Data;
    if( !Data ){
        p = gMseCursorArrow;
        Width = 8;
        Height = 8;
        Pitch = 8;
        Xcenter = 1;
        Ycenter = 1;
        Alpha = MSE_CURSOR_COLOR_ALPHA;
    }
    clean = gMseCursorClear;
    if( !clean && gMseAcq ){ // cursor is dirty
        gMseCursorClear = 1;
        MseGetCursorSizeRect( &MouseArea );
        WinUpdateArea( &MouseArea );
    }
    if( Width != gMseWidth || Height != gMseHeight ){ // realloc memory if picture size differs
        if( !( pic = Malloc( Width * Height ) ) ){
            if( clean )return -1;
            MseDrawCursor();            
        }
        if( gMseCursorPic ) Free( gMseCursorPic ); // free old picture
        gMseCursorPic = pic;
    }
    gMseWidth = Width;
    gMseHeight = Height;
    gMsePitch = Pitch;
    gMseAnimCursorFrame = p;
    gMseAlpha = Alpha;
    if( gMseAnimCursorFrames ) InpTaskStop( MseUpdateAnimatedCursor ); // stop animating cursor
    gMseAnimCursorFrames = 0;
    gMseX += gMseCenterX - Xcenter;
    gMseCenterX = Xcenter;
    gMseY += gMseCenterY - Ycenter;
    gMseCenterY = Ycenter;
    MseCursorClipMain();
    if( !clean ) MseDrawCursor();
    gMseOldX = gMseX;
    gMseOldY = gMseY;
    return 0;
}

int MseGetAnimatedCursor( char **FramesData, int *Frames, int *Width, int *Height, int *Xcenter, int *Ycenter, int *Alpha, int *Rate )
{
    if( !gMseAnimCursorFrames ) return -1;
    *FramesData = gMseAnimCursorFrames;
    *Frames  = gMseAnimFrameCnt;
    *Width   = gMseWidth;
    *Height  = gMseHeight;
    *Xcenter = gMseCenterX;
    *Ycenter = gMseCenterY;
    *Alpha   = gMseAlpha;
    *Rate    = gMseAnimRate;
    return 0;
}

int MseSetAnimatedCursor( char *pFramesData, int Frames, int FrameStart, int Width, int Height, int Xcenter, int Ycenter, char Alpha, int Rate_ms )
{
    int err;

    err = MseSetStaticCursor(&pFramesData[Height * Width * FrameStart], Width, Height, Width, Xcenter, Ycenter, Alpha);
    if( err != -1 ){
        gMseAnimCursorFrames = pFramesData;
        gMseAnimFrameCnt = Frames;
        gMseAnimRate = Rate_ms;
        gMseAnimFrameNo = FrameStart;
        InpTaskStart( MseUpdateAnimatedCursor );
        return 0;
    }
    return err;
}

void MseUpdateAnimatedCursor()
{
    if( TimerCurrDiff( gMseAnimTimer ) < gMseAnimRate ) return;
    gMseAnimTimer = TimerGetSysTime();
    if( ++gMseAnimFrameNo == gMseAnimFrameCnt ) gMseAnimFrameNo = 0;
    gMseAnimCursorFrame = &gMseAnimCursorFrames[ gMseWidth * gMseHeight * gMseAnimFrameNo ];
    if( !gMseCursorClear ) MseDrawCursor();
}

void MseDrawCursor()
{
    char *MouseBmp;
    int dstIdx; int j; int srcIdx; int SrcX; int Width; int SrcY; int Height; int srcLine; int i;
    char pixel;

    MouseBmp = gMseCursorPic;
    if( gMseAcq ){
        if( !gMseBlitAlpha || !gMseCursorClear ){
            WinWhipeCursor( gMseCursorPic ); // whipes current cursor
            MouseBmp = gMseCursorPic;
            // update transparency to cursor frame
            for( i = srcLine = dstIdx = 0; i < gMseHeight; i++, srcLine += gMsePitch ){
                srcIdx = srcLine;
                for( j = 0; j < gMseWidth; j++, srcIdx++, dstIdx++ ){
                    pixel = gMseAnimCursorFrame[ srcIdx ];
                    if( pixel != gMseAlpha ) MouseBmp[ dstIdx ] = pixel;
                }                
            }
        }

	// left button
        if( gMseX >= gVidMainGeo.lt ){
            if( gMseWidth + gMseX - 1 <= gVidMainGeo.rt ){
                Width = gMseWidth;
                SrcX = 0;
            } else {
                SrcX = 0;
                Width = gVidMainGeo.rt - gMseX + 1;
            }
        } else {
            SrcX = gVidMainGeo.lt - gMseX;
            Width = gMseWidth - (gVidMainGeo.lt - gMseX);
        }

	// right button
        if( gMseY >= gVidMainGeo.tp ){
            Height = gMseHeight;
            SrcY = 0;
            if( gMseHeight + gMseY - 1 > gVidMainGeo.bm ) Height = gVidMainGeo.bm - gMseY + 1;
        } else {
            SrcY = gVidMainGeo.tp - gMseY;
            Height = gMseHeight - (gVidMainGeo.tp - gMseY);
        }
        gMseCursorPic = MouseBmp;
        
        if( gMseBlitAlpha && gMseCursorClear )
            gMseBlitAlpha( gMseAnimCursorFrame, gMsePitch, gMseHeight, SrcX, SrcY, Width, Height, SrcX + gMseX, SrcY + gMseY, gMseAlpha );
        else
            gMseBlit( gMseCursorPic, gMseWidth, gMseHeight, SrcX, SrcY, Width, Height, SrcX + gMseX, SrcY + gMseY );
        MouseBmp = gMseCursorPic;
        gMseCursorClear = 0;
    }
    gMseCursorPic = MouseBmp;
}

void MseCursorRedraw()
{
    VidRect_t tmp;

    if( !gMseAcq || gMseCursorClear ) return;        

    tmp.lt = gMseX;
    tmp.tp = gMseY;
    tmp.rt = gMseWidth + gMseX - 1;
    tmp.bm = gMseHeight + gMseY - 1;
    gMseCursorClear = 1; // dirty flag
    WinUpdateArea( &tmp );
}

void MseUpdate()
{
    InputMouse_t mstat;
    int x_axis, y_axis;
    char buttons;

    if( !gMseAcq || gMseCursorClear || gMseDisable ) return;
    if( InputMouseGetData( &mstat ) == 1 ){
        x_axis = mstat.x;
        y_axis = mstat.y;
        if( mstat.BtL == 1 ) buttons = MSE_LBUTT;
        if( mstat.BtR == 1 ) buttons |= MSE_RBUTT;
    } else {
        x_axis = y_axis = buttons = 0;
    }
    y_axis = lround( gMseSpeed * y_axis );
    x_axis = lround( gMseSpeed * x_axis );
    if( gRecState == 1 ){
DD
//        if( (gRecPlayUnk04 & 4) != 0 && buttons || (gRecPlayUnk04 & 2) != 0 && (x_axis || y_axis) ){
//            gRecPlayUnk03 = 2;
//            RecClr();
//            return;
//        }
//        buttons = gMseButtonsOld;
//	y_axis = 0;
//	x_axis = 0;
    }
    MseProcess( x_axis, y_axis, buttons );
}

void MseProcess( int x, int y, int buttons)
{
    static int 	PrevButtStat;
    static int  OldX = -1, OldY = -1;
    int usave;
    VidRect_t rect;

    if( !gMseAcq || gMseCursorClear ) return;

    if( ( x != OldX) || ( y != OldY ) || (buttons != gMseButtonsOld) ){
/*
        if( gRecState == 0 ){
            if( gRecFrame == 4095 ) RecFlushOut();
            gRec[ gRecFrame ]->i00 = 3;
            gRec[ gRecFrame ]->mx = x;
            gRec[ gRecFrame ]->my = y;
            gRec[ gRecFrame ]->mbutt = buttons;
            gRec[ gRecFrame ]->i01 = gRecPlayUnk05;
            gRec[ gRecFrame ]->i02 = gRecUnk04;
            gRecFrame++;
        }
*/
    } else if( !gMseButtonsOld ){
        if( !gMseSampleTime ){
            gMseTimeEv = TimerGetSysTime();
            gMseSampleTime = 1;
        }
        gMseButtonsOld = 0;
        gMseButtStatCur = 0;
        gMseButtStat = 0;
        return;
    }
    gMseSampleTime = 0;
    gMseButtonsOld = buttons;
    PrevButtStat = gMseButtStat;
    gMseButtStat = 0;
    // left button
    if( PrevButtStat & ( MSE_LBUTT | MSE_LBUTT_HOLD ) ){
        if( !( buttons & MSE_LBUTT ) ){
            gMseButtStat = MSE_LBHOLD;
        } else {
    	    gMseButtStat = MSE_LBUTT_HOLD;
    	    if( TimerCurrDiff( gMseLbuttTimer ) > MSE_BUTT_HTIME ){
    		gMseButtStat |= MSE_LBUTT;
		gMseLbuttTimer = TimerGetSysTime();
	    }
	}
    } else if( buttons & MSE_LBUTT ){
    	gMseButtStat = MSE_LBUTT;
	gMseLbuttTimer = TimerGetSysTime();
    }

    // right button
    if( PrevButtStat & ( MSE_RBUTT | MSE_RBUTT_HOLD ) ){
        if( ( buttons & MSE_RBUTT ) == 0 ) {
            gMseButtStat |= MSE_RBHOLD;
        } else {
            gMseButtStat |= MSE_RBUTT_HOLD;
            if( TimerCurrDiff( gMseRbuttTimer ) > MSE_BUTT_HTIME ){
    		gMseButtStat |= MSE_RBUTT;
    		gMseRbuttTimer = TimerGetSysTime();
            }
        }
    } else {
	if( buttons & MSE_RBUTT ){
    	    gMseButtStat |= MSE_RBUTT;
    	    gMseRbuttTimer = TimerGetSysTime();
	}
    }
    gMseButtStatCur = gMseButtStat;
    // redraw cursor if position changed
    if( (x != OldX) || (y != OldY) ){
	usave = gVidUpdateForbid;
	gVidUpdateForbid = 1;
        rect.lt = gMseX; // previous mouse position
        rect.tp = gMseY;
        rect.rt = gMseWidth  + gMseX - 1;
        rect.bm = gMseHeight + gMseY - 1;
        gMseX = x; // update mouse position
        gMseY = y;
        MseCursorClipMain();
        WinUpdateArea( &rect ); // clear cursor
	gVidUpdateForbid = usave;
        MseDrawCursor(); // draw cursor at new position
        gMseOldX = gMseX;
        gMseOldY = gMseY;
        OldX = x; 
        OldY = y;
    }
}

// return 1 if mouse cursor is in area
int MseCursorInArea( int left, int top, int bottom, int right )
{
    return gMseAcq && 
	( ( gMseY + gMseHeight ) >  top    ) && 
	(   gMseX                <= right  ) && 
	( ( gMseX + gMseWidth )  >  left   ) && 
	(   gMseY                <= bottom );    
}

// return 1 if mouse cursor center is in area
int MseCursorCenterInArea( int left, int top, int right, int bottom )
{
    return gMseAcq && 
	( ( gMseY + gMseCenterY ) >= top    ) && 
	( ( gMseX + gMseCenterX ) <= right  ) && 
	( ( gMseX + gMseCenterX ) >= left   ) && 
	( ( gMseY + gMseCenterY ) <= bottom );
}

void MseGetCursorSizeRect( VidRect_t *p )
{
    p->lt = gMseX;
    p->tp = gMseY;
    p->rt = gMseWidth  + gMseX - 1;
    p->bm = gMseHeight + gMseY - 1;
}

void MseGetCursorPosition( int *Xpos, int *Ypos )
{
    *Xpos = gMseCenterX + gMseX;
    *Ypos = gMseCenterY + gMseY;
}

void MseSetCursorPosition( int x, int y )
{
    gMseX    = x - gMseCenterX;
    gMseY    = y - gMseCenterY;
    gMseOldY = y - gMseCenterY;
    gMseOldX = x - gMseCenterX;
    MseCursorClipMain();
}

void MseCursorClipMain()
{
    if( (gMseCenterX + gMseX) < gVidMainGeo.lt ){
        gMseX = gVidMainGeo.lt - gMseCenterX;
    } else {
        if( (gMseCenterX + gMseX) > gVidMainGeo.rt ){
	    gMseX = gVidMainGeo.rt - gMseCenterX;
        }
    }    
    if( (gMseCenterY + gMseY) < gVidMainGeo.tp ) {
        gMseY = gVidMainGeo.tp - gMseCenterY;
    } else {
        if( (gMseCenterY + gMseY) > gVidMainGeo.bm ){
	    gMseY = gVidMainGeo.bm - gMseCenterY;
        }
    }    
}

int MseGetButtons()
{
    return gMseButtStat;
}

int MseIsCursorClear()
{
    return gMseCursorClear;
}

void MseGetCursorSize( int *Width, int *Height )
{
    *Width  = gMseCenterX;
    *Height = gMseCenterY;
}

void MseCursorMoveCenter( int x, int y )
{
    VidRect_t rect;
    int clean, a;

    clean = gMseCursorClear;
    if( !gMseCursorClear && gMseAcq && !gMseCursorClear ){
        gMseCursorClear = 1;
        MseGetCursorSizeRect( &rect );
        WinUpdateArea( &rect );
    }

    gMseX += gMseCenterX - x;
    gMseOldX = gMseX;
    gMseCenterX = x;

    gMseY += gMseCenterY - y;
    gMseOldY = gMseY;
    gMseCenterY = y;

    if( !clean ) MseDrawCursor();
}

int MseIsAcquired()
{
    return gMseAcq;
}

void MseGetData( int *posX, int *posY, int *buttons )
{
    InputMouse_t mstat;

    if( InputMouseGetData( &mstat ) != 1 ){
        mstat.y = 0;
        mstat.x = 0;
        mstat.BtL = ( gMseButtStat & MSE_LBUTT ) ? 1:0;
        mstat.BtR = ( gMseButtStat & MSE_RBUTT ) ? 1:0;
    }
    gMseButtStatCur = 0;
    gMseOldX += mstat.x;
    gMseOldY += mstat.y;
    if( mstat.BtL == 1 ) gMseButtStatCur = MSE_LBUTT;
    if( mstat.BtR == 1 ) gMseButtStatCur |= MSE_RBUTT;
    *posX = gMseOldX;
    *posY = gMseOldY;
    *buttons = gMseButtStatCur;
}

void MseDisable()
{
    gMseDisable = 1;
}

void MseEnable()
{
    gMseDisable = 0;
}

int MseEnabled()
{
    return gMseDisable;
}

void MseSetSpeed( double val )
{
    if( (val > 0.0) && (val < 2.0) ) gMseSpeed = val;
}

double MseGetSpeed()
{
    return gMseSpeed;
}

int MseGetTime()
{
    if( gMseSampleTime ){
        if( gMseAcq && !gMseCursorClear && !gMseDisable ) return TimerCurrDiff( gMseTimeEv );
        gMseSampleTime = 0;
    }
    return 0;
}

void MseTimeReset()
{
    if( gMseSampleTime ) gMseSampleTime = 0;
}

