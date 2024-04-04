#include "FrameWork.h"

#define MSG_DISPL_PIXEL_HEIGHT	60
#define MSG_DISPL_PIXEL_WIDTH	167
#define MSG_DISPL_H		100
#define MSG_DISPL_W		80
#define MSG_FONT_ID		101
#define MSG_TEXT_COLOR		gPalColorCubeRGB[0][31][0]
#define MSG_POSX		23
#define MSG_POSY		24
#define MSG_BUTT_H		30
#define MSG_CLICK_SND		"monitor"
#define MSG_CLICK_TIME		500
#define MSG_INDEX_CHAR		149

int gIfcMsgEnable = 0;
VidRect_t gIfcMsgArea = { 0x17, 0x18, 0x0BD, 0x53 };
int gIfcMsgButtDn = -1;
int gIfcMsgButtUp = -1;

char gIfcMsgBufLines[ MSG_DISPL_H * MSG_DISPL_W ];
char *gIfcMsgSurf;
int gIfcMsgDisplLines;
int gIfcMsgBtEnable;
int gIfcMsgDisplOffset;
int gIfcMsgWidth;
int gIfcMsgLines;
int gIfcMsgLinesCounter;
int gIfcMsgTime;


/*****************************************/

int IfcMsgInit()
{
    CachePool_t *ImgObj;
    ArtFrmHdr_t *art;
    int Font, i;
    char *img;

    if( gIfcMsgEnable ) return 0;

    Font = FontGetCurrent();
    FontSet( MSG_FONT_ID );
    gIfcMsgLines = MSG_DISPL_H;
    gIfcMsgDisplLines = MSG_DISPL_PIXEL_HEIGHT / gFont.ChrHeight();
    gIfcMsgLinesCounter = 0;
    gIfcMsgDisplOffset = 0;
    FontSet( Font );
    if( !( gIfcMsgSurf = Malloc( MSG_DISPL_PIXEL_HEIGHT * MSG_DISPL_PIXEL_WIDTH ) ) ) return -1;    
    if( !(art = ArtLoadImg( ArtMakeId( 6, 16, 0, 0, 0 ), &ImgObj )) ){ Free( gIfcMsgSurf ); return -1; }
    img = ArtGetObjData( art, 0, 0 );
    gIfcMsgWidth = ArtGetObjWidth( art, 0, 0 );
    ScrCopy( WIN_XY( MSG_POSX, MSG_POSY, gIfcMsgWidth, img ), MSG_DISPL_PIXEL_WIDTH, MSG_DISPL_PIXEL_HEIGHT, gIfcMsgWidth, gIfcMsgSurf, MSG_DISPL_PIXEL_WIDTH );
    ArtClose( ImgObj );
    gIfcMsgButtUp = WinCreateButton( gIfcWin, MSG_POSX, MSG_POSY, MSG_DISPL_PIXEL_WIDTH, MSG_BUTT_H, -1, -1, -1, -1, 0, 0, 0, 0 );
    if( gIfcMsgButtUp != -1 ) WinSetButtonHandler( gIfcMsgButtUp, IfcMsgSetCursorA, IfcMsgSetCursorC, IfcMsgButtUp, 0 );
    gIfcMsgButtDn = WinCreateButton( gIfcWin, MSG_POSX, MSG_POSY + MSG_BUTT_H, MSG_DISPL_PIXEL_WIDTH, MSG_BUTT_H, -1, -1, -1, -1, 0, 0, 0, 0 );
    if( gIfcMsgButtDn != -1 ) WinSetButtonHandler( gIfcMsgButtDn, IfcMsgSetCursorB, IfcMsgSetCursorC, IfcMsgButtDn, 0 );
    gIfcMsgBtEnable = 1;
    gIfcMsgEnable = 1;
    for( i = 0; i < gIfcMsgLines; i++ ) gIfcMsgBufLines[ i * MSG_DISPL_W ] = '\0';
    gIfcMsgLinesCounter = 0;
    gIfcMsgDisplOffset = 0;
    IfcMsgUpdate();    
    return 0;
}

void IfcMsgFlush()
{
    int i;

    if( !gIfcMsgEnable ) return;
    for( i = 0; i < gIfcMsgLines; i++ ) gIfcMsgBufLines[ i * MSG_DISPL_W ] = '\0';
    gIfcMsgLinesCounter = 0;
    gIfcMsgDisplOffset = 0;
    IfcMsgUpdate();
}

void IfcMsgClose()
{
    if( !gIfcMsgEnable ) return;    
    Free(gIfcMsgSurf);
    gIfcMsgEnable = 0;    
}

void IfcMsgOut( char *str )
{
    char IndexStr[2], IndexChar, *Rest, *s;
    int IdxWidth, time, CurrentFont;

    Rest = NULL;
    IndexChar = MSG_INDEX_CHAR;
    if( gIfcMsgEnable == 0 ) return;
    CurrentFont = FontGetCurrent();
    FontSet( 101 );
    IndexStr[ 0 ] = MSG_INDEX_CHAR;
    IndexStr[ 1 ] = '\0';
    IdxWidth = gFont.LineWidth( IndexStr );
    if( !IN_COMBAT ){
	time = TimerGetTime();
	if( TimerDiff( time, gIfcMsgTime ) >= MSG_CLICK_TIME ){
	    gIfcMsgTime = time;
	    GSoundPlay( MSG_CLICK_SND );
	}
    }
    while( 1 ){
	if( gFont.LineWidth( str ) > (167 - gIfcMsgDisplLines - IdxWidth) ){
	    if( (s = strrchr( str, ' ' )) != 0 ){
		if( Rest ) *Rest = ' ';
		Rest = s;    
		if( Rest ) *Rest = '\0';
		continue;
	    }
	    if( IndexChar == 0 ){
		strncpy( gIfcMsgBufLines + gIfcMsgLinesCounter * 80, str, 79 );
	    } else {
		gIfcMsgBufLines[ gIfcMsgLinesCounter * 80 ] = IndexChar;
		strncpy( gIfcMsgBufLines + gIfcMsgLinesCounter * 80 + 1, str, 78 );
	    }
	    gIfcMsgBufLines[ gIfcMsgLinesCounter * 80 + 79 ] = '\0';
	    gIfcMsgLinesCounter = (gIfcMsgLinesCounter + 1) % gIfcMsgLines;
	    break;
	}
	if( IndexChar == 0 ){
	    strncpy( gIfcMsgBufLines + gIfcMsgLinesCounter * 80, str, 79 );
	} else {
	    gIfcMsgBufLines[ gIfcMsgLinesCounter * 80 ] = IndexChar;
    	    IndexChar = 0;
	    IdxWidth = 0;
	    strncpy( gIfcMsgBufLines + gIfcMsgLinesCounter * 80 + 1, str, 78 );
	}
	gIfcMsgBufLines[ gIfcMsgLinesCounter * 80 + 79 ] = '\0';
	gIfcMsgLinesCounter = (gIfcMsgLinesCounter + 1) % gIfcMsgLines;
	if( Rest == 0 ) break;
	str = Rest + 1;
	*Rest = ' ';
	Rest = NULL;
    }
    FontSet( CurrentFont );
    gIfcMsgDisplOffset = gIfcMsgLinesCounter;
    IfcMsgUpdate();
}

void IfcMsgUpdate()
{
    char *surf, *p;
    int fnt, i, n;

    if( !gIfcMsgEnable ) return;            
    if( !(surf = WinGetSurface( gIfcWin )) ) return;
    p = WIN_XY( MSG_POSX, MSG_POSY, gIfcMsgWidth, surf );
    ScrCopy( gIfcMsgSurf, MSG_DISPL_PIXEL_WIDTH, MSG_DISPL_PIXEL_HEIGHT, MSG_DISPL_PIXEL_WIDTH, p, gIfcMsgWidth );
    fnt = FontGetCurrent();
    FontSet( MSG_FONT_ID );
    for( i = 0; i < gIfcMsgDisplLines; i++ ){
	n = ((gIfcMsgLines - gIfcMsgDisplLines + gIfcMsgDisplOffset + i) % gIfcMsgLines) * MSG_DISPL_W;
        gFont.Print( p + i * gIfcMsgWidth * gFont.ChrHeight(), &gIfcMsgBufLines[ n ], MSG_DISPL_PIXEL_WIDTH, gIfcMsgWidth, MSG_TEXT_COLOR );
    }
    WinAreaUpdate( gIfcWin, &gIfcMsgArea );
    FontSet( fnt );
}

void IfcMsgButtUp()
{
    if( (gIfcMsgLines + gIfcMsgDisplOffset - 1) % gIfcMsgLines != gIfcMsgLinesCounter ){
        gIfcMsgDisplOffset = (gIfcMsgLines + gIfcMsgDisplOffset - 1) % gIfcMsgLines;
        IfcMsgUpdate();
    }
}

void IfcMsgButtDn()
{
    if( gIfcMsgDisplOffset != gIfcMsgLinesCounter ){
        gIfcMsgDisplOffset = (gIfcMsgDisplOffset + 1) % gIfcMsgLines;
        IfcMsgUpdate();
    }
}

void IfcMsgSetCursorA()
{
    GmouseLoadCursor( 2 );
}

void IfcMsgSetCursorB()
{
    GmouseLoadCursor( 3 );
}

void IfcMsgSetCursorC()
{
    GmouseLoadCursor( 1 );
}

void IfcMsgButtDisable()
{
    if( !gIfcMsgBtEnable ) return;
    WinDisableWidget( gIfcMsgButtDn );
    WinDisableWidget( gIfcMsgButtUp );
    gIfcMsgBtEnable = 0;
}

void IfcMsgButtEnable()
{
    if( gIfcMsgBtEnable ) return;    
    WinEnableWidget( gIfcMsgButtDn );
    WinEnableWidget( gIfcMsgButtUp );
    gIfcMsgBtEnable = 1;    
}

