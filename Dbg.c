#include "FrameWork.h"

void DbgInit()
{
    /* Empty */
}

void DbgUnk01( int *a1 )
{
/*
    int v1; // esi
    int v3; // edx
    int i; // ebx

    v1 = gDbgUnk01[0];
    v3 = 0;
    for( i = 0; i >> 2 < 512; i += 4 ){
        *a1++ = gDbgUnk02[v3];
        v3 = (v3 + 1) % v1;
    }
    gDbgUnk01[0] = v1;
*/
}

int DbgUnk02( int a1, char *a2 )
{
/*
    int result; // eax
    _DWORD *v3; // ebp
    int v4; // esi
    int i; // edi

    result = 16 * a1;
    if ( *(int *)((char *)gDbgUnk03 + result) )
    {
        v3 = *(_DWORD **)((char *)&gDbgUnk04 + result);
        v4 = 0;
        for( i = 0; i < 512; i++ ){
            if( gDbgUnk02[v4] != *v3 ) DbgStompErr3(i, a2);
            result = (v4 + 1) / gDbgUnk01[0];
            ++v3;
            v4 = (v4 + 1) % gDbgUnk01[0];
        }
    }
    return result;
*/
}

int DbgUnk03( int a1, char *a2 )
{
/*
    _DWORD *v3; // edi
    int v4; // esi
    int result; // eax
    _DWORD *v6; // [esp+0h] [ebp-1Ch]

    v3 = *(_DWORD **)(DbgUnk04[0] + 4 * a1);
    v4 = 0;
    v6 = v3 + 512;
    do
    {
        if ( gDbgUnk02[v4] != *v3 )
            DbgStompErr1(a1, a2);
        result = (v4 + 1) / gDbgUnk01[0];
        ++v3;
        v4 = (v4 + 1) % gDbgUnk01[0];
    }
    while ( v3 != v6 );
    return result;
*/
}

int DbgUnk05( int a1, char *a2 )
{
/*
    int result; // eax
    int v4; // esi
    int i; // edi
    unsigned int v6; // [esp+0h] [ebp-30h]
    int v7; // [esp+4h] [ebp-2Ch]
    int v8; // [esp+Ch] [ebp-24h]
    _BYTE *v9; // [esp+18h] [ebp-18h]
    _BYTE *v10; // [esp+1Ch] [ebp-14h]

    result = 20 * a1;
    if ( *(int *)((char *)gDbgUnk05 + result) )
    {
        v7 = *(int *)((char *)&gDbgUnk06 + result);
        v4 = 0;
        v8 = *(int *)((char *)&gDbgUnk07 + result);
        v6 = *(unsigned int *)((char *)gDbgUnk08 + result) >> 2;
        while ( 1 )
        {
            result = v4 + v7;
            if ( v4 >= (int)(4 * v6) )
                break;
            if ( *(_DWORD *)result != *(_DWORD *)(v4 + v8) )
                DbgStompErr2(a1, v4, a2);
            v4 += 4;
        }
        if ( gDbgUnk08[5 * a1] != 4 * v6 )
        {
            v9 = (_BYTE *)(v4 + v7);
            v10 = (_BYTE *)(v4 + v8);
            for ( i = v4;
                  i < gDbgUnk08[5 * a1];
                  ++v9 )
            {
                if ( *v10 != *v9 )
                    DbgStompErr2(a1, i, a2);
                result = 20 * a1;
                ++i;
                ++v10;
            }
        }
    }
    return result;
*/
}

void DbgStompErr3( int a1, char *a2 )
{
/*
    int v3; // ecx
    int v4; // edx
    _DWORD *v5; // edx

    eprintf( "TRAPPED A STOMP ERROR:\n" );
    eprintf( "Stomp caught by check on line %d", v3 );
    eprintf( " of module %s.\n", a2);
    eprintf( "Data stomped was in trap %s", *(const char **)(v4 + 4) );
    eprintf( " at address %p.\n", (const void *)(4 * a1 + *v5) );
    eprintf( "See comment in trap.c for suggestions on better" );
    eprintf( " isolating the stomp bug.\n" );
    WinMsgError( "STOMPED!" );
*/
    exit( 1 );
}

void DbgStompErr2( int a1, int a2, char *a3 )
{
/*
    int v5; // ecx
    int v6; // edx
    _DWORD *v7; // edx
    int v8; // edx

    eprintf("TRAPPED A STOMP ERROR:\n");
    eprintf("Stomp caught by check on line %d", v5);
    eprintf(" of module %s.\n", a3);
    eprintf("Data stomped was in trap %s", *(const char **)(v6 + 12));
    eprintf(" at address %p.\n", (const void *)(a2 + *v7));
    eprintf("This is duplicate trap number %d", a1);
    eprintf(" at an internal offset of %d.\n", a2);
    eprintf("Trap size is %d.\n", *(_DWORD *)(v8 + 8));
    eprintf("See comment in trap.c for suggestions on better");
    eprintf(" isolating the stomp bug.\n");
    WinMsgError("STOMPED!");
*/
    exit(1);
}

void DbgStompErr1( int a1, char *a2 )
{
/*
    int v3; // ecx
    const void *v4; // edx

    eprintf("TRAPPED A STOMP ERROR:\n");
    eprintf("Stomp caught by check on line %d", v3);
    eprintf(" of module %s.\n", a2);
    eprintf("Data stomped was in heap trap number %d", a1);
    eprintf(" at address %p.\n", v4);
    eprintf("See comment in trap.c for suggestions on better");
    eprintf(" isolating the stomp bug.\n");
    WinMsgError("STOMPED!");
*/
    exit(1);
}

int DbgUnk06()
{
    return 1;
}

void DbgGameVersion( char *str )
{
    sprintf( str, "FALLOUT II %d.%02d ", 1, 2 );
}

int DbgUnk07( int result, int a2, int a3 )
{
/*
    void *v3; // edi
    const void *v4; // esi

    if ( a3 > a2 )
    {
        v3 = (void *)(result + a2);
        v4 = (const void *)(result + a2 + 1);
        result = a3 - a2;
        qmemcpy(v3, v4, a3 - a2);
    }
    return result;
*/
}



