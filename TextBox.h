#pragma once 

#define TEXTBOX_MARGIN_H 2
#define TEXTBOX_MARGIN_V 8

int TextBoxFileSelect( char *Title, char **FileList, int Win, void (*cnt)(), int w, int h, int flags );
int TextBoxSelect( char *msg, char **FileList, int n, void (*Cb)(), int a0, int a1, int a7, int a8 );
int TextBoxDialogEdit1( char *text, int MaxChars, char *Label, int Xpos, int Ypos );
int TextBoxUnk10( char *Label, char **TextList, int Lines, int Ypos, int Yposa, int Color, char *a7 );
int TextBoxUnk11( char *a1, int a2, int a3, int a4 );
int TextBoxUnk12( char *a1, int a2, int a3, int a4 );
int TextBoxUnk13( char **a1, int a2, int a3, int a4, int FrameColor );
int TextBoxCreate( char **TextList, int Lines, int Xpos, int Ypos, int FrameColor, int BgColor, VidRect_t *Area );
int TextBoxUnk01( int WidId, VidRect_t *Area, char **lines, int text, int FrameColor, int CanvasColor, WinDlg_t *a7, int Idx );
int TextBoxUnk03( char *a1 );
void TextBoxUnk04();
int TextBoxNew( int WinId, int x0, int y0, int x1, int y1, int ColorFg, int ColorBg );
int TextBoxAddMsg( int a1, int a2, char *a3, int a4, int a5, char **a6, int a7, int a8 );
void TextBoxDelete( int a1 );
int TextBoxRun( WinDlg_t *tb, int TextId );
int TextBoxSearchLine( int chr, char **TextList, int Lines );
int TextBoxListWidth( char **TextList, int Lines );
int TextBoxDialogRun( int a1, char *edx0, int a3, int ecx0, int arg0, int a6, int a5 );
