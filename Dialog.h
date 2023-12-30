#pragma once

typedef struct 
{
    char *Str1;
    union{
	char *Str2;
	int iVal;
    };
    int Flags;
    int i04;
    int i05;
    int Cnt;
    short Font;
    short w01;
} Dialog02_t;

typedef struct _Dialog01_t
{
  char *Str1;
  char *Str2; // reply
  char *Str3;
  Dialog02_t *Option;
  int i05;
  int Cnt;
  short Font;
  short w01;
}  Dialog01_t;

typedef struct 
{
    Intp_t *Itp;	
  Dialog01_t *Dlgs;
  int Cnt1; // reply cnt
  int Cnt2;
  int Cnt3;
  int i06;
  int i07;
} Dialog_t;

/*
int gSayUnk31 = -1;
int gSayUnk100 = 0;
int gSayUnk101 = 0;
char *gSayUnk102 = "fullscreen";
int (*gSayUnk33)(void) = NULL;
int (*gSayUnk32)(void) = NULL;
int gSayUnk13 = 7;
int gSayUnk14 = 7;
int gSayUnk22 = 5;
int gSayUnk26 = 0;
int gSayUnk23 = 0;
int gSayUnk01 = 0;
int gSayUnk02 = 0;
int gSayUnk34 = 2;

Dialog_t gSayUnk103[ 4 ];
char gSayUnk110;
short gSayUnk30;
int gSayUnk05;
int gSayUnk06;
int gSayUnk03;
int gSayUnk04;
int gSayUnk07;
short gSayUnk111;
int gSayUnk10;
int gSayUnk11;
int gSayUnk08;
int gSayUnk09;
int gSayUnk12;
int gSayUnk112;
int gSayUnk113;
int gSayUnk27;
int gSayUnk28;
int gSayUnk24;
int gSayUnk29;
char *gSayLines[1];
int gSayUnk25;
int gSayUnk114;
int gSayUnk115;
int gSayUnk116;
void *gSayUnk117;
void *gSayUnk118;
void *gSayUnk119;
void *gSayUnk120;
char *gSayUnk121;
int gSayUnk16;
int gSayUnk15;
int gSayUnk21;
void *gSayUnk17;
void *gSayUnk18;
void *gSayUnk19;
void *gSayUnk20;
int gSayUnk122;
int (*gSayUnk123)(int, int);
int gSayUnk124;
*/


void DialogUnk01( char *a1, char *a2 );
Dialog01_t *DialogUnk02();
void DialogUnk03( char *a1, char *a2, int a3 );
void DialogUnk04( char *a1, int a2, int a3 );
void DialogUnk10( Dialog02_t *p );
void DialogUnk11();
int  DialogUnk12( int eax0, int a1, int a2, int a3, int Color, char * a6 );
int  DialogUnk13( char *a1, char *edx0, int a3, int a4, int a5 );
int  DialogUnk14();
char *DialogUnk15( char *a1, int a2, int a3 );			// X not used
void DialogUnk16( char *a1, int a2, int *a3, int *a4 );	// X not used
void DialogUnk17( int a1, char **a2, int a3, int a4, int a5, int posX, int a7, int Flags, int Wrap );
void DialogUnk18( int a1, char *a2, int ecx0, int a4, int posX, int a7, int Flags, int Wrap, int a9 );
int  DialogUnk19( int a1, int a2, int a3, int a4, int a5, int Flags );
void DialogUnk20( int a1 );
int  DialogUnk21( int a1 );
void DialogUnk22(); // X internal
void  DialogUnk23( int a1, char *a2, int a3, int a4, int posX, int a7, int Flags, int Wrap, int a9, int ); // X internal
int  DialogUnk24( int eax0 );				// X internal
int  DialogUnk25();					// X not used
int  DialogUnk26( char *a1, char *a2 );			// X not used
int  DialogUnk27();					// X internal
int  DialogUnk28( int a1 );				// X internal
int  DialogUnk29( int a1 );				// X internal
int  DialogStart( Intp_t *itp );			// SayStart;
int  DialogRestartOpt();				// Restart;
int  DialogGotoReply( char *NodeName ); 		// SayGoToReply( "sm_node1" ); go to label
int  DialogSetTitle( char *Title ); 			// sayreplytitle( "Simple dialog" ); // Optional
int  DialogReply( char *NodeName, char *Text ); 	// SayReply( "sm_node1","Hello" ); create reply with ID="sm node1"
int  DialogSetOptionStr( char *Label, char *StrNode );	// SayOption( "Quit", "exit" );
int  DialogSetOptionInt( char *Label, int NodeId );	// SayOption( "Gimme exp", node1 );
int  DialogSetOptionStrA( char *Label, char *StrNode, int a3 ); // X not used
int  DialogSetOptionIntA( char *Label, int NodeId, int a3 );	// X not used
int  DialogMsg( char *a1, char *a2, int Time );			// SayMessage( "MsgFile", Msg )
int  DialogEnd( int a1 );				// SayEnd;
int  DialogUnk32();
int  DialogQuit();				// SayQuit;
int  DialogOptWin( int x0, int y0, int x1, int y1, char *ImgFileName );
int  DialogReplyWin( int x0, int y0, int x1, int y1, char *ImgFileName );
int  DialogBorder( int hor, int vert ); // set hor/vert tabs for windows
int  DialogScrollUp( int a1, int a2, char *a3, char *a4, char *a5, char *a6, int a7 );	// sayscrollup( "", "", "", int,int,int )
int  DialogScrollDn( int a1, int a2, char *a3, char *a4, char *a5, char *a6, int a7 );	// sayscrolldn( "", "", "", int,int,int )
int  DialogSetOptSpacing( int spc ); // gap beetwen options
int  DialogSetOptionColor( float r, float g, float b );
int  DialogSetReplyColor( float r, float g, float b );
int  DialogSetOptionFlags( short justify ); // justify: 0-left, 1-right, 2-center
void DialogFree();
int  DialogGetSayModeLvl();
void DialogUnk34( int (*)(), int (*a2)() );
int  DialogSetOneOptPause( int a1 );
int  DialogGetOneOptPause();





