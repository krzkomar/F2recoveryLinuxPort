#include "FrameWork.h"

unsigned int gTimerTime;
unsigned int gThrTimeInit;

void TimerThrTimeInit()
{
    gThrTimeInit = TimerGetSysTime();
}

int TimerThrGetTime()
{
    return (TimerGetSysTime() - gThrTimeInit) / 10;
}

int TimerGetSysTime()
{
    static long long ZeroAdj = 0, tmp;
    struct timeval ts;
//    struct timespec ts;
//    clock_gettime( CLOCK_MONOTONIC, &ts );
//    return (1000 * ts.tv_sec + ts.tv_nsec / 1000000);
    
    gettimeofday( &ts, 0 );
    tmp = (long long)(ts.tv_sec * 1000 + (ts.tv_usec / 1000 ) );
    if( ZeroAdj == 0 ) ZeroAdj = tmp;    
    return tmp - ZeroAdj;
}

void TimerWaitProc( unsigned int time_ms )
{
    unsigned int t0, t1, dt;

    t0 = TimerGetSysTime();
    t1 = TimerGetSysTime();
    dt = ( t0 > t1 ) ? 0x7FFFFFFF : (t1 - t0);
    while( dt < time_ms ){
//	TaskUnk01( time_ms );
	t1 = TimerGetSysTime();
	dt = ( t0 > t1 ) ? 0x7FFFFFFF : (t1 - t0);
    }
}

void TimerWait( unsigned int time_ms )
{
    unsigned int t0,t1,dt;
    
    t0 = TimerGetSysTime();
    do{
        t1 = TimerGetSysTime();
        dt = ( t0 > t1 ) ? 0x7FFFFFFF : (t1 - t0);
    }while ( dt < time_ms );
}

unsigned int TimerCurrDiff( unsigned int t0_ms )
{
    unsigned int t;

    t = TimerGetSysTime();
    return ( t0_ms > t ) ? 0x7FFFFFFF : (t - t0_ms);
}

int TimerDiff( unsigned int t1, unsigned int t0 )
{
    return ( t0 > t1 ) ? 0x7FFFFFFF : (t1 - t0);
}

unsigned int TimerGetTime()
{
    return gTimerTime;
}

