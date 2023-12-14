#include "FrameWork.h"


int gRandUnk01[ 32 ];
int gRandUnk02 = 0;
int gRandUnk03;
int gRandUnk04 = 302400; // 3.5 day ?

int RandomInit()
{
    srand( RandGetTime() );
    RandUnk01( RandGet2x() );
    RandTest();
    return 0;
}

int RandFSave()
{
    return 0;
}

int RandUnk05( int a1, int a2, int *a3 )
{
    int v3, r;

    v3 = a1 - RandMinMax( 1, 100 );
    r = RandUnk04( v3, a2 );
    if( a3 ) *a3 = v3;
    return r;
}

int RandUnk04( int a1, int a2 )
{
    if( a1 < 0 ){
        if( (RandUnk03() / 0xD2F00) && RandMinMax( 1, 100 ) <= -a1 / 10 ) return 0;
        return 1;
    }
    if( ( RandUnk03() / 0xD2F00) && RandMinMax( 1, 100 ) <= a1 / 10 + a2 ) return 3;
    return 2;    
}

int RandMinMax( int min, int max )
{
    int r;

    if( min <= max ){
        r = min + RandUnk02( max + 1 - min);
    } else {
        r = max + RandUnk02( min + 1 - max );
    }
    if( r < min || r > max ){
        eprintf("Random number %d is not in range %d to %d", r, min, max );
        return min;
    }
    return r;
}

int RandUnk02( int r )
{
    int a, b, c;
    
    a = 16807 * (gRandUnk03 % 127773) - 2836 * (gRandUnk03 / 127773);
    if( a < 0 ) a += a + 0x7FFFFFFF < 0 ? -2 : 0x7FFFFFFF;
    b = gRandUnk02 & 0x1F;
    c = gRandUnk01[ b ];
    gRandUnk01[ b ] = a;
    gRandUnk02 = c;
    gRandUnk03 = a;
    return c % r;
}

int RandGetB()
{
    srand( TimerGetSysTime() );
    return RandUnk01((rand() + rand()) & 0x7FFFFFF);
}

int RandGetA( int r )
{
    if( r == -1 ) r = ( rand() + rand() ) & 0x7FFFFFF;    
    return RandUnk01( r );
}

unsigned int RandGet2x()
{
    return ( rand() + rand() ) & 0x7FFFFFF;
}

int RandUnk01( int r )
{
    int k, i, a;

    if( r < 1 ) r = 1;
    k = 39;
    for( i = 39; i >= 0; i--, k-- ){
        a = 16807 * (r % 127773) - 2836 * (r / 127773);
        r = a + (a < 0 ? 0x7FFFFFFF : 0);
        if( i < 32 ) gRandUnk01[ k ] = r;
    }
    gRandUnk02 = gRandUnk01[0];
    gRandUnk03 = r;
    return gRandUnk02;
}

int RandGetTime()
{
    return TimerGetSysTime();
}

int RandTest()
{
    int i,r, noise[25];
    double fmt;

    memset( noise, 0x00, 25 * sizeof( int ) );
    for( i = 0;i < 100000; i++ ){
        r = RandMinMax( 1, 25 );
        if( r - 1 < 0 ) eprintf( "I made a negative number %d\n", r - 1 );
        noise[ r - 1 ]++;
    }
    fmt = 0.0;
    for( i = 0; i < 25; i++ ){
        fmt += ((double)noise[ i ] - 4000.0) * ((double)noise[ i ] - 4000.0) / 4000.0;
    }
    eprintf( "Chi squared is %lf, P = %lf at 0.05\n", fmt, 36.42 );
    if( fmt <= 36.42 )
        return eprintf( "Sequence is random, 95%% confidence.\n" );
    else
        return eprintf( "Warning! Sequence is not random, 95%% confidence.\n" );
}

int RandUnk03()
{
    return gRandUnk04;
}

