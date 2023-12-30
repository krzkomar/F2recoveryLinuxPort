#pragma once

int StrParseGetInt( char **str, int *Value );
int StrParseGetStr( char **pStr, char *dst, int nsize );
int StrParseFromList( char **pStr, int *ElementFoundIdx, const char **MatchList, int MatchElements );
int StrParseFromFunc( char **pStr, int *ElementFoundIdx, int (*Func)(int *, char *) );
int StrParseGetPrefixedInt( char **pStr, char *MatchStr, int *Val, char *Prefix );
int StrParseGetPrefixedStrInt( char **pStr, char *Dst, int *Val, char *Prefix );

int StriCmp( const char *s1, const char *s2 );



