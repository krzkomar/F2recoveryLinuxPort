#pragma once

#define TIMER_WAIT( TimeStart, time_ms ) while( TimerCurrDiff( TimeStart ) < 4 );
//#define TIMER_WAIT( TimeStart, time_ms ) while( TimerCurrDiff( TimeStart ) < time_ms );
//#define TIMER_WAIT( TimeStart, time_ms ) usleep( 41*100);

extern unsigned int gTimerTime;

void TimerThrTimeInit();
int TimerThrGetTime();
int TimerGetSysTime();
void TimerWaitProc( unsigned int time_ms );
void TimerWait( unsigned int time_ms );
unsigned int TimerCurrDiff( unsigned int t0_ms );
int TimerDiff( unsigned int t1, unsigned int t0 );
unsigned int TimerGetTime();



