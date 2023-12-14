#pragma once 

typedef struct
{
    int i01;
    int i02;
    int i03;
    int i04;
    int i05;
    union{
	int   itg;
	char *str;
	float flt;
    };
    void (*i07)(int);
    void (*i08)(int);
} WidgetProc_t;

typedef struct 
{
  int id;
  int i01;
  int InputLength;
  int CursorPos;
  int StrLen;
  char *EditedString;
  char *Str01;
  int StrLen01;
  int i08;
  void (*i09)( char *, void * );
  void (*Validator)();
    void *i05;
} WidgetMsg_t;

typedef struct 
{
  int Id;
  int Pending;
  int Xpos;
  int Ypos;
  int StrField;
  int FontHeight;
  int Wrap;
  int ColorBg;
  int ColorFg;
  int Font;
} Window01_t;

typedef struct{
    char *data;
    char *i02;
    int  WinId;
    VidRect_t Area;
    int i04;
    int i05;
    int i06;
} Widget02_t;

/*
int gWidgetEditorRun = 0;
WidgetMsg_t gWidgetEditor;
int gWidgetUnk08;
int gWidgetEditValidFlg;
int gWidgetUnk07;
WidgetProc_t **gWidgetUnk10;
Widget02_t gWidgetUnk13;
char gWidgetEditorString;
int gWidgetUnk09;
WidgetMsg_t *gWidgetEvQe;
int gWidgetEvQeSize;
Window01_t *gWidgets;
int gWidgetUnk12;
int gWidgetTableSize;
int gWidgetUnk02;
int gWidgetUnk01;
int gWidgetUnk03;
*/

void WidgetEditDrawCursor( WidgetMsg_t *msg );
void WidgetUnk02( int a1, int a2 );
void WidgetDrawMsg( int idx, WidgetMsg_t *msg );
void WidgetEditorStart( int WgId, char *Str, int ValidatorFlg, int BufSize, int AutoUpdate );
void WidgetEditorUpdate();
void WindgetEditorEnd();
int WidgetUnk01( int a1, char *a2, int a3, void( *a4)() );
void WidgetUnk03( int a1 );
int WidgetEvProcess( int Id );
int WidgetEvExe( int Idx );
int WidgetUnk04( int MsgId, void (*cb)(char *, void * ), void *ptr );
int WidgetMsgNew( int MsgId, int posX, int Field, int posY, int Font, int Wrap, int Flags, char Color );
int WidgetDrawText1( int Idx, char *str );
int WidgetDrawText0( int MsgId, char *Str, int StrBufLen );
int WidgetLabelMouseDefine( int MsgId );
int WidgetUnk06( int a1 );
int WidgetUnk07( int Idx ); 
int WidgetUnk08( int Idx, int Font, int Wrap, int ColorBg, int ColorFg );
int WidgetUnk09( int Id );
int WidgetUnk10();
int WidgetUnk11( int WinId, char *Text, int Ypos, int Color );
void WidgetWinRegisterUpdate( WidgetProc_t *proc );
int WidgetUnk12();
int WidgetUnk13();
int WidgetUnk05( int a1, int a2, int a3, void (*a4)(int), void (*a5)(int), int a6, int a7, int a8 );
int WidgetUnk14( int idx );
void WidgetUnk15();
void WidgetUnk16();
void WidgetUnk17();
void WidgetUnk18();
void WidgetUnk19();
void WidgetUnk20( int a1, int a2, int a3 );
void WidgetUnk21( float a1, float a2, int a3, int a4 );
void WidgetUnk22( float a1, int a2, int a3 );
void WidgetUnk23( int WinId, char *fname1, char *fname2, int a4, int lt, int tp );
void WidgetUnk24( int *a1, int *a2, int *a3, int *a4, int a5 );
void WidgetUnk25( int result, int a2, int a3, int a4, int a5 );

