#include "FrameWork.h"

MsgPos_t gMsgPos[ MSG_WINDOW_SLOTS ];
int gMsgTextPosX;
int gMsgH;
MsgWin_t gMsgWin[ MSG_WINDOW_SLOTS ];
int gMsgTime;
int gMsgXalign;
int gMsgTextPosY;
int gMsgCurrent;
int gMsgWinCount;

void MsgInit()
{
    int i, y;

    gMsgTime = 3000;
    gMsgWinCount = 0;
    gMsgCurrent = -1;
    gMsgXalign = gVidMainGeo.rt / 2;
    if( gVidMainGeo.bm >= 479 ){
        gMsgTextPosY = 16;
        gMsgTextPosX = 16;
    } else {
        gMsgTextPosY = 10;
        gMsgTextPosX = 10;
    }
    gMsgH = 2 * gMsgTextPosY + gFont.ChrHeight();
    y = gVidMainGeo.bm / 4;
    for( i = 0; i < MSG_WINDOW_SLOTS; i++ ){
        gMsgPos[ i ].PosY = y * i;
        gMsgPos[ i ].Used = 0;
    }
}

void MsgSetTime( int time_ms )
{
    gMsgTime = time_ms;
}

void MsgKill()
{
    InpTaskStop( MsgKillAll );
    while( gMsgCurrent != -1 ) MsgKillSingle();
}

int MsgCreate( char *Text, int Col )
{
    int winy, w, i, win, Button, winx, h;

    if( !gWinSys.Init ) return -1;
    if( gMsgCurrent == gMsgWinCount ) return -1;
    w = gFont.LineWidth(Text) + 2 * gMsgTextPosX;
    winx = gMsgXalign - (w >> 1);
    h = gMsgH;
    for( i = 0; i < MSG_WINDOW_SLOTS; i++  ) {
	if( gMsgPos[ i ].Used == 0 ){
	    winy = gMsgPos[ i].PosY;
	    gMsgPos[ i ].Used = 1;
	    break;
	}
    }       
    if( i == MSG_WINDOW_SLOTS ) return -1;
    win = WinCreateWindow( winx, winy, w, h, MSG_WINDOW_BG_COLOR, 4 );
    WinDrawFrame( win );
    WinDrawText( win, Text, 0, gMsgTextPosX, gMsgTextPosY, Col );
    Button = WinCreateButton( win, 0, 0, w, h, -1, -1, -1, -1, 0, 0, 0, 0 );
    WinSetButtonHandler( Button, 0, 0, 0, MsgButtonHandler );
    WinUpdate( win );
    gMsgWin[ gMsgWinCount ].WinId = win;
    gMsgWin[ gMsgWinCount ].time = TimerGetSysTime();
    gMsgWin[ gMsgWinCount ].id = i;
    gMsgWinCount++;
    if( gMsgWinCount == MSG_WINDOW_USED ){
        gMsgWinCount = 0;
    } else if( gMsgCurrent == -1 ){
        gMsgCurrent = 0;
        InpTaskStart( MsgKillAll );
    }
    return 0;
}

void MsgKillAll()
{
    static int Guard = 0;
     
    if( Guard ) return;
    
    Guard = 1;
    while( gMsgCurrent != -1 ){
        if( TimerCurrDiff( gMsgWin[ gMsgCurrent ].time ) < gMsgTime ) break;
        MsgKillSingle();
    }
    Guard = 0;
}

void MsgKillSingle()
{
    int a;

    a = gMsgCurrent;
    if( gMsgCurrent != -1 ){
        WinClose( gMsgWin[ gMsgCurrent ].WinId );
        a = gMsgCurrent + 1;
        gMsgPos[ gMsgWin[gMsgCurrent].id + 1 ].Used = 0;
        if( a == MSG_WINDOW_SLOTS ) a = 0;
        if( a == gMsgWinCount ){
            gMsgWinCount = 0;
            gMsgCurrent = -1;
            InpTaskStop( MsgKillAll );
            a = gMsgCurrent;
        }
    }
    gMsgCurrent = a;
}

void MsgClose( int MsgIdx )
{
    int i, k;

    if( gMsgCurrent == -1 || !MsgUnk01( MsgIdx ) ) return;
    WinClose( gMsgWin[ MsgIdx ].WinId );
    gMsgPos[ gMsgWin[ MsgIdx ].id ].Used = 0;
    
    for( i = MsgIdx; i != gMsgCurrent; ){
        k = i - 1;
        if( k < 0 ) k = MSG_WINDOW_USED;
        memcpy( &gMsgWin[ i ], &gMsgWin[ k ], sizeof( MsgWin_t ) );
        i = k;
    }

    if( ++gMsgCurrent == MSG_WINDOW_SLOTS ) gMsgCurrent = 0;
    if( gMsgWinCount != gMsgCurrent ) return;
    gMsgWinCount = 0;
    gMsgCurrent = -1;
    InpTaskStop( MsgKillAll );
}

void MsgButtonHandler( int WgId, int MsgId, void *p )
{
    int win;

    win = WinImageGetWinId( WgId );
    if( gMsgCurrent != -1 ){
        while( win != gMsgWin[ MsgId ].WinId ){
            if( ++MsgId == MSG_WINDOW_SLOTS ) MsgId = 0;
            if( MsgId == gMsgCurrent || !MsgUnk01( MsgId ) )return;
        }
        MsgClose( MsgId );
    }
}

int MsgUnk01( int MsgIdx )
{
    if( gMsgCurrent == gMsgWinCount ) return 1;    
    if( gMsgCurrent >= gMsgWinCount ){
        if( MsgIdx >= gMsgWinCount && MsgIdx < gMsgCurrent ) return 0;
    } else {
	if( MsgIdx >= gMsgWinCount || MsgIdx < gMsgCurrent ) return 0;
    }
    return 1;    
}

