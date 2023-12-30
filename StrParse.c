#include "FrameWork.h"

int StrParseGetInt( char **str, int *Value )
{
    char *s, *p, *nextpos, c;
    int pos, comma;

    if( *str == NULL ) return -1;    
    s = *str;
    StrLwr(*str);
    pos = strspn(s, " ");
    p = &s[pos];
    comma = strcspn(p, ",");
    nextpos = &(*str)[pos + comma];
    *str = nextpos;
    if( *nextpos ) *str = nextpos + 1;
    if( comma ){
        c = p[ comma ];
        p[ comma ] = '\0';
    }
    *Value = strtol( p, NULL, 10 );
    if( comma ) p[comma] = c;
    return 0;
}

int StrParseGetStr( char **pStr, char *dst, int nsize )
{
    char *s, *p, tmp;
    int comm, n;

    if( *pStr == NULL ) return 0;
    s = *pStr;
    dst[ nsize - 1 ] = '\0';
    StrLwr( *pStr );
    n = strspn( s, " " );
    comm = strcspn((const char *)pStr + n, ",");
    p = &(*pStr)[n + comm];
    *pStr = p;
    if( *p ) *pStr = p + 1;
    if( comm ){
        tmp = s[comm];
        s[comm] = '\0';
    }
    strncpy( dst, s, nsize );
    if( comm ) s[comm] = tmp;
    return 0;
}

int StrParseFromList( char **pStr, int *ElementFoundIdx, const char **MatchList, int MatchElements )
{
    int n, idx, i;
    char *s, *p, tmp;

    if( *pStr == NULL) return 0;    
    s = *pStr;
    StrLwr( *pStr );
    n = strspn( s, " " );
    s += n; // ignore trailing spaces
    idx = strcspn( s, "," );
    p = &(*pStr)[ n + idx ];
    *pStr = p;
    if( *p ) *pStr = p + 1;
    if( idx ){
        tmp = s[ idx ];
        s[ idx ] = '\0';
    }
    for( i = 0; i < MatchElements; i++ ){
        if( !strcasecmp( s, MatchList[ i ] ) ) break;
    }
    if( idx ) s[ idx ] = tmp;
    if( i >= MatchElements ){
        eprintf( "\nstrParseStrFromList Error: Couldn't find match for string: %s!", s );
        *ElementFoundIdx = -1;
        return -1;
    }
    *ElementFoundIdx = i;    
    return 0;
}

int StrParseFromFunc( char **pStr, int *ElementFoundIdx, int (*Func)(int *, char *) )
{
    int err, n, comma;
    char *s, *p, tmp;

    if( !*pStr ) return 0;
    s = *pStr;
    StrLwr(*pStr);
    n = strspn(s, " ");
    s += n;
    comma = strcspn(s, ",");
    p = &(*pStr)[n + comma];
    *pStr = p;
    if( *p ) *pStr = p + 1;
    if( comma ){
        tmp = s[comma];
        s[comma] = '\0';
    }
    err = Func( ElementFoundIdx, s );
    if( comma ) s[ comma ] = tmp;
    if( err ){
        eprintf( "\nstrParseStrFromFunc Error: Couldn't find match for string: %s!", s );
        err = -1;
        *ElementFoundIdx = -1;
    }
    return err;
}

int StrParseGetPrefixedInt( char **pStr, char *MatchStr, int *Val, char *Prefix )
{
    char *s, p, tmp;
    int comma,pos,found;

    s = *pStr;
    found = 0;
    if( !*pStr ) return 0;
    if( *s == '\0') return -1;    
    StrLwr( s );
    if( *s == ',' ){ s++; (*pStr)++; }
    s += strspn(s, " ");
    comma = strcspn(s, ",");
    tmp = s[ comma ];
    s[ comma ] = '\0';
    pos = strcspn(s, Prefix);
    p = s[ pos ];
    s[ pos ] = '\0';
    if ( !strcmp(s, MatchStr) ){
        *pStr = s + 1;
        found = 1;
        *Val = strtol( &s[ pos + 1 ], NULL, 10 );
    }
    s[ pos ] = p;
    s[ comma ] = tmp;
    if( found ) return 0;    
    return -1;
}

int StrParseGetPrefixedStrInt( char **pStr, char *Dst, int *Val, char *Prefix )
{
    char *str, *ComPos, *v19, *v20, tmp1, tmp2;
    int PrefPos, v14;

    str = *pStr;
    if( *pStr == NULL ) return 0;
    str = *pStr;
    if( !*str ) return -1;
    StrLwr(str);
    if( *str == ',' ){ str++; (*pStr)++; }
    str += strspn(str, " ");
    v14 = strcspn(str, ",");
    ComPos = str + v14;
    tmp1 = *ComPos;
    *ComPos = '\0';
    PrefPos = strcspn(str, Prefix);
    v14++;
    tmp2 = str[ PrefPos ];
    v19 = &str[ PrefPos ];
    str[ PrefPos ] = '\0';
    v20 = &str[ PrefPos + 1 ];
    strcpy( Dst, str );
    *pStr += v14;
    *Val = strtol( v20, NULL, 10 );
    *v19 = tmp2;
    *ComPos = tmp1;
    return 0;
}

/******************************************************************************/
// inny plik


int StriCmp( const char *Str1, const char *Str2 )
{
    unsigned char s1, s2;

    while ( 1 ){
        s1 = *Str1;
        s2 = *Str2;
        if( *Str1 >= 'A' && *Str1 <= 'Z' ) s1 += ' ';
        if( s2 >= 'A' && s2 <= 'Z' ) s2 += ' ';
        if( s1 != s2 || !s2 ) break;
        Str1++;
        Str2++;
    }
    return s1 - s2;
}



