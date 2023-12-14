#pragma once

int  DlgBox( char *Str1, char **List, int Lines, int Xpos, int Ypos, int Color1, char *Str2, int Color2, int Flags );
int  DlgBoxLoad( char *Title, char **pList, char *Selected, int Lines, int Xpos, int Ypos );
int  DlgBoxSave( char *Title, char **pList, char *Selected, int Lines, int Xpos, int Ypos );
void DlgBoxList( char *Surf, char **LineList, int Lines, int LineOfs, int Marked, int Width );


