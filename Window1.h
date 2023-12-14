#pragma once

typedef struct 
{
  char Name[1];
  int i02;
  int i03;
  int i04;
  int i05;
  int i06;
  int i07;
  int i08;
  int WinId;
  int i10;
  int i11;
    Region_t **Regions;
  int i13;
  int RegionCnt;
  int i15;
//  Win03_t *i16;
  int Cnt;
  int i18;
  int i19;
  int i20;
  int i21;
  int i22;
  int i23;
} Win02_t;


int WinGetFont();
int WinSetFont( int FontId );
void WinSetTextColorDefault();
int WinGetTextFlags();
int WinSetTextFlags( int Flags );
char WinGetTextColor();
char WinGetColorHL();
void WinSetTextColor( float r, float g, float b );
void WinSetTextColorHL( float r, float g, float b );
int WinUnk97( char *Str, int a2, int a3 );
void WinDlgInsertExec( int (*ptr)(int) );
int WinDlgProcess();
void WinDrawButton( int w, int h, char *surf1, char *surf2, char *surf3 );
void WinDlgClose();
void WinPrintText( int Id, char *Str, int BufferSize, int FieldSize, int MaxHeight, int posX, int posY, int Flags, int Wrap );
char **WinTextWrapCreate( char *Str, int FieldWidth, int LineWidth, int *pLines );
void WinTextWrapDestroy( char **list, int cnt );
int WinUnk07( int a1, char *a2, int a3, int a4, int a5, int a6, int a7, int a8 );
int WinUnk16( char *a1, int a2, int a3, int a4, int a5, int a6 );
int WinGetScreenWidth();
int WinGetScreenHeight();
void WinBlit( char *data, int SrcW, int SrcH, char *surf, int DstW, int DstH );


