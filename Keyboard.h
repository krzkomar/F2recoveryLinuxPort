#pragma once

typedef struct 
{
    unsigned int time;
    unsigned int counter;
} KbdTimer_t;

void KbdSetTimeThs1( int time );
int  KbdGetTimeThs1();
void KbdSetTimeThs0( int time );
int  KbdGetTimeThs0();
void gKbdCallbackInitSet( void (*cb)(int) );
void (*KbdInitCb())(int);
void KbdRunCbSet( void (*cb)(void) );
void (*KbdRunCb())(void);
void KbdInit();
void KbdTrLang();
void KbdClose( int Acquire );
int  KbdNull01();
int  KbdCloseA( int arg );
void KbdUpdate();
void KbdTimesReset();
void KbdKeyStore( InputKbd_t *kdata );
void KbdThreadSync();


