#pragma once

#define MSG_WINDOW_BG_COLOR 257
#define MSG_WINDOW_SLOTS	5
#define MSG_WINDOW_USED		4

typedef struct
{
    int	Used;
    int	PosY;
} MsgPos_t;

typedef struct
{
  int time;
  int WinId;
  int id;
} MsgWin_t;

/*
MsgPos_t gMsgPos[ MSG_WINDOW_SLOTS ];
int gMsgTextPosX;
int gMsgH;
MsgWin_t gMsgWin[ MSG_WINDOW_SLOTS ];
int gMsgTime;
int gMsgXalign;
int gMsgTextPosY;
int gMsgCurrent;
int gMsgWinCount;
*/

void MsgInit();
void MsgSetTime( int time_ms );
void MsgKill();
int MsgCreate( char *Text, int Col );
void MsgKillAll();
void MsgKillSingle();
void MsgClose( int MsgIdx );
void MsgButtonHandler( int WgId, int MsgId, void *p );
int MsgUnk01( int MsgIdx );
