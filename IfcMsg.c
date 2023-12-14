#include "FrameWork.h"

int gIfcMsgEnable = 0;
VidRect_t gIfcMsgArea = { 0x17, 0x18, 0x0BD, 0x53 };
int gIfcMsgButtDn = -1;
int gIfcMsgButtUp = -1;

char gIfcMsgBufLines[ 100 * 80 ];
char *gIfcMsgSurf;
int gIfcMsgDisplLines;
int gIfcMsgBtEnable;
int gIfcMsgUnk06;
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
    FontSet( 101 );
    gIfcMsgLines = 100;
    gIfcMsgDisplLines = 60 / gFont.ChrHeight();
    gIfcMsgLinesCounter = 0;
    gIfcMsgUnk06 = 0;
    FontSet( Font );
    if( !( gIfcMsgSurf = Malloc( 10020 ) ) ) return -1;    
    if( !(art = ArtLoadImg( ArtMakeId( 6, 16, 0, 0, 0 ), &ImgObj )) ){ Free( gIfcMsgSurf ); return -1; }
    img = ArtGetObjData( art, 0, 0 );
    gIfcMsgWidth = ArtGetObjWidth( art, 0, 0 );
    ScrCopy( &img[24 * gIfcMsgWidth + 23], 167, 60, gIfcMsgWidth, gIfcMsgSurf, 167 );
    ArtClose( ImgObj );
    gIfcMsgButtUp = WinCreateButton( gIfcWin, 23, 24, 167, 30, -1, -1, -1, -1, 0, 0, 0, 0 );
    if( gIfcMsgButtUp != -1 ) WinSetButtonHandler( gIfcMsgButtUp, IfcMsgSetCursorA, IfcMsgSetCursorC, IfcMsgButtUp, 0 );
    gIfcMsgButtDn = WinCreateButton( gIfcWin, 23, 54, 167, 30, -1, -1, -1, -1, 0, 0, 0, 0 );
    if( gIfcMsgButtDn != -1 ) WinSetButtonHandler( gIfcMsgButtDn, IfcMsgSetCursorB, IfcMsgSetCursorC, IfcMsgButtDn, 0 );
    gIfcMsgBtEnable = 1;
    gIfcMsgEnable = 1;
    for( i = 0; i < gIfcMsgLines; i++ ) gIfcMsgBufLines[ i*80 ] = '\0';
    gIfcMsgLinesCounter = 0;
    gIfcMsgUnk06 = 0;
    IfcMsgUpdate();    
    return 0;
}

void IfcMsgFlush()
{
    int i;

    if( !gIfcMsgEnable ) return;
    for( i = 0; i < gIfcMsgLines; i++ ) gIfcMsgBufLines[ i*80 ] = '\0';
    gIfcMsgLinesCounter = 0;
    gIfcMsgUnk06 = 0;
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
    int Font, Time, len;
    char *v1, *s, v19;

    v1 = 0;
    v19 = 149;
    if( !gIfcMsgEnable ) return;
    Font = FontGetCurrent();
    FontSet( 101 );
    gFont.LineWidth( "*" );
    if( (gCombatStatus & 1) == 0 ){
        Time = TimerGetTime();
        if( TimerDiff(Time, gIfcMsgTime) >= 500 ){
            gIfcMsgTime = Time;
            GSoundPlay( "monitor" );
        }
    }
    while( 1 ){
        if( gFont.LineWidth( str ) > (167 - gIfcMsgDisplLines) ){
    	    if( !(s = strchr( str, ' ' )) ){
		s = &gIfcMsgBufLines[ 80 * gIfcMsgLinesCounter ];
		if( v19 ){
    		    s++;
    		    gIfcMsgBufLines[ 80 * gIfcMsgLinesCounter ] = v19;
    		    len = 78;
		} else {
    		    len = 79;
		}
		strncpy( s, str, len );
		gIfcMsgBufLines[ 80 * gIfcMsgLinesCounter + 79 ] = '\0';
		gIfcMsgLinesCounter = (gIfcMsgLinesCounter + 1) % gIfcMsgLines;
    		break;
    	    }
    	    if( v1 ) *v1 = ' ';
    	    v1 = s;
    	    *s = '\0';
        } else {
    	    s = &gIfcMsgBufLines[ 80 * gIfcMsgLinesCounter ];
    	    if( v19 ){
        	s++;
        	gIfcMsgBufLines[ 80 * gIfcMsgLinesCounter ] = v19;
        	v19 = 0;
        	strncpy( s, str, 78 );
    	    } else {
        	strncpy( s, str, 79 );
    	    }            
    	    gIfcMsgBufLines[ 80 * gIfcMsgLinesCounter + 79 ] = 0;
    	    gIfcMsgLinesCounter = (gIfcMsgLinesCounter + 1) % gIfcMsgLines;
    	    if( !v1 ) break;
    	    str = v1 + 1;
    	    *v1 = ' ';
    	    v1 = 0;
        }
    }
    FontSet( Font );
    gIfcMsgUnk06 = gIfcMsgLinesCounter;
    IfcMsgUpdate();    
}

void IfcMsgUpdate()
{
    char *surf, *p;
    int fnt, i, n;

    if ( !gIfcMsgEnable ) return;            
    if( !(surf = WinGetSurface( gIfcWin )) ) return;
    p = surf + 24 * gIfcMsgWidth + 23;
    ScrCopy( gIfcMsgSurf, 167, 60, 167, p, gIfcMsgWidth );
    fnt = FontGetCurrent();
    FontSet( 101 );
    for( i = 0; i < gIfcMsgDisplLines; i++ ){
	n = ((gIfcMsgLines - gIfcMsgDisplLines + gIfcMsgUnk06 + i) % gIfcMsgLines) * 80;
        gFont.Print( p + i * gIfcMsgWidth * gFont.ChrHeight(), &gIfcMsgBufLines[ n ], 167, gIfcMsgWidth, gPalColorCubeRGB[0][31][0] );
    }
    WinAreaUpdate( gIfcWin, &gIfcMsgArea );
    FontSet( fnt );
}

void IfcMsgButtUp()
{
    if ( (gIfcMsgLines + gIfcMsgUnk06 - 1) % gIfcMsgLines != gIfcMsgLinesCounter )
    {
        gIfcMsgUnk06 = (gIfcMsgLines + gIfcMsgUnk06 - 1) % gIfcMsgLines;
        IfcMsgUpdate();
    }
}

void IfcMsgButtDn()
{
    if ( gIfcMsgUnk06 != gIfcMsgLinesCounter )
    {
        gIfcMsgUnk06 = (gIfcMsgUnk06 + 1) % gIfcMsgLines;
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

