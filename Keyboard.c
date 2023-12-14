#include "FrameWork.h"

static char 		gKbdScanLang[ 256 ];
static KbdTimer_t 	gKbdTimer[ 256 ];

static void 		(*gKbdCallbackRun)( void ) = NULL;
static void 		(*gKbdCallbackInit)( int ) = NULL;
static int  		gKbdTimeThs1 = 50;
static int  		gKbdTimeThs0 = 500;  
static int  		gKbdState = 0;

void KbdSetTimeThs1( int time )
{
    gKbdTimeThs1 = time;
}

int KbdGetTimeThs1()
{
    return gKbdTimeThs1;
}

void KbdSetTimeThs0( int time )
{
    gKbdTimeThs0 = time;
}

int KbdGetTimeThs0()
{
    return gKbdTimeThs0;
}

void gKbdCallbackInitSet( void (*cb)(int) )
{
    gKbdCallbackInit = cb;
}

void (*KbdInitCb())(int)
{
    return gKbdCallbackInit;
}

void KbdRunCbSet( void (*cb)(void) )
{
    gKbdCallbackRun = cb;
}

void (*KbdRunCb())(void)
{
    return gKbdCallbackRun;
}

void KbdInit()
{
    int i;

    KbdCloseA( 1 );
    InputMouseAcquire();
    KbdTrLang();
    for( i = 0; i != 256; i++ ){
        gKbdTimer[ i ].time = -1;
        gKbdTimer[ i ].counter = 0;
    }
}

void KbdTrLang()
{
    gKbdScanLang[  2] = 2;
    gKbdScanLang[  4] = 4;
    gKbdScanLang[  6] = 6;
    gKbdScanLang[  1] = 1;
    gKbdScanLang[  3] = 3;
    gKbdScanLang[  5] = 5;
    gKbdScanLang[  7] = 7;
    gKbdScanLang[  8] = 8;
    gKbdScanLang[ 10] = 10;
    gKbdScanLang[  9] = 9;
    gKbdScanLang[ 11] = 11;
    gKbdScanLang[ 12] = ( gLanguageId ) ? (( gLanguageId == 1 ) ? 7 : '5') : 12;    
    gKbdScanLang[ 14] = 14;
    gKbdScanLang[ 13] = ( gLanguageId ) ? (( gLanguageId == 1 ) ? 13 : 11 ) : 13;    
    gKbdScanLang[ 15] = 15;
    gKbdScanLang[ 16] = ( gLanguageId == 1 ) ? 30 : 16;
    gKbdScanLang[ 17] = ( gLanguageId == 1 ) ? ',' : 17;
    gKbdScanLang[ 18] = 18;
    gKbdScanLang[ 20] = 20;
    gKbdScanLang[ 19] = 19;
    switch( gLanguageId ){
        case 0:  gKbdScanLang[21] = 21; break;
        case 1:  gKbdScanLang[21] = '\x15'; break;
        case 4:  gKbdScanLang[21] = '\x15'; break;
        case 3:  gKbdScanLang[21] = '\x15'; break;
        default: gKbdScanLang[21] = ','; break;
    }
    gKbdScanLang[23] = 23;
    gKbdScanLang[25] = 25;
    gKbdScanLang[22] = 22;
    gKbdScanLang[24] = 24;
    switch( gLanguageId ){
	case 0: gKbdScanLang[26] = 26; break;
        case 1: gKbdScanLang[26] = 6;  break;
        case 4: gKbdScanLang[26] = 26; break;
        case 3: gKbdScanLang[26] = 26; break;
        default: gKbdScanLang[26] = 9; break;
    }
    switch ( gLanguageId ){
        case 0: gKbdScanLang[27] = 27; break;
        case 1: gKbdScanLang[27] = 12; break;
        case 4: gKbdScanLang[27] = 27; break;
        case 3: gKbdScanLang[27] = 27; break;
        default: gKbdScanLang[27] = 10; break;
    }
    gKbdScanLang[28] = 28;
    gKbdScanLang[29] = 29;
    gKbdScanLang[30] = ( gLanguageId == 1 ) ? '\x10' : '\x1E';
    gKbdScanLang[31] = '\x1F';
    gKbdScanLang[34] = '"';
    gKbdScanLang[36] = '$';
    gKbdScanLang[32] = ' ';
    gKbdScanLang[35] = '#';
    gKbdScanLang[37] = '%';
    gKbdScanLang[38] = '&';
    gKbdScanLang[33] = '!';
    gKbdScanLang[39] = ( gLanguageId ) ? '3' : '\'';
    switch( gLanguageId ){
	case 0: gKbdScanLang[40] = '('; break;
	case 1: gKbdScanLang[40] = 5; break;
	case 3: gKbdScanLang[40] = 12; break;
	case 4: gKbdScanLang[40] = 12; break;
	default: gKbdScanLang[40] = 13; break;
    }
    switch( gLanguageId ){
	case 0: gKbdScanLang[41] = ')'; break;
	case 1: gKbdScanLang[41] = 3; break;
	case 3: gKbdScanLang[41] = 0; break;
	case 4: gKbdScanLang[41] = 0; break;
	default:gKbdScanLang[41] = 27; break;
    }
    gKbdScanLang[42] = '*';
    switch( gLanguageId ){
        case 0:  gKbdScanLang[43] = '+'; break;
        case 1:  gKbdScanLang[43] = '\t'; break;
        case 4:  gKbdScanLang[43] = ')'; break;
        case 3:  gKbdScanLang[43] = ')'; break;
        default: gKbdScanLang[43] = 12; break;
    }
    switch( gLanguageId ){
	case 0: gKbdScanLang[44] = ','; break;
        case 1: gKbdScanLang[44] = 17;  break;
        case 4: gKbdScanLang[44] = ','; break;
        case 3: gKbdScanLang[44] = ','; break;
        default: gKbdScanLang[44] = 21; break;
    }
    memcpy( &gKbdScanLang[45], "-./01", 5 );
    gKbdScanLang[50] = ( gLanguageId == 1 ) ? '\'' : '2';
    gKbdScanLang[51] = ( gLanguageId == 1 ) ? '2' : '3';
    gKbdScanLang[52] = ( gLanguageId == 1 ) ? '3' : '4';    
    gKbdScanLang[53] = ( gLanguageId ) ? (( gLanguageId == 1 ) ? '4' : 8) : '5';
    memcpy( &gKbdScanLang[54], "6789:;<=>?@ABCDEFGHIJKLMNOPQRS", 30 );
    memcpy( &gKbdScanLang[87], "WX", 2 );
    gKbdScanLang[100] = -1;
    gKbdScanLang[101] = -1;
    gKbdScanLang[102] = -1;
    gKbdScanLang[112] = -1;
    gKbdScanLang[121] = -1;
    gKbdScanLang[123] = -1;
    gKbdScanLang[125] = -1;
    gKbdScanLang[141] = -1;
    gKbdScanLang[144] = -1;
    gKbdScanLang[145] = -1;
    gKbdScanLang[146] = -1;
    gKbdScanLang[147] = -1;
    gKbdScanLang[148] = -1;
    gKbdScanLang[149] = -1;
    gKbdScanLang[150] = -1;
    gKbdScanLang[151] = -1;
    gKbdScanLang[184] = 0xB8;
    gKbdScanLang[181] = 0xB5;
    gKbdScanLang[156] = 0x9C;
    gKbdScanLang[157] = 0x9D;
    gKbdScanLang[179] = -1;
    gKbdScanLang[199] = 0xC7;
    gKbdScanLang[183] = 0x54;
    gKbdScanLang[200] = 0xC8;
    gKbdScanLang[203] = 0xCB;
    gKbdScanLang[209] = 0xD1;
    gKbdScanLang[207] = 207;
    gKbdScanLang[201] = 201;
    gKbdScanLang[205] = 205;
    gKbdScanLang[210] = 210;
    gKbdScanLang[211] = 0xD3;
    gKbdScanLang[208] = 0xD0;
    gKbdScanLang[219] = -1;
    gKbdScanLang[220] = -1;
    gKbdScanLang[221] = -1;
}

void KbdClose( int Acquire )
{
    KbdCloseA( Acquire );
    if( Acquire )
        InputMouseAcquire();
    else
        InputMouseUnlink();
}

int KbdNull01()
{
    return 0;
}

int KbdCloseA( int arg )
{
    if( arg == gKbdState ) return 0;
    if( !arg ){
        InputKeyboardUnlink();
        KeyFlush();
        gKbdState = arg;
        return 0;
    }
    if( !InputKeyboardAcquire() ) return -1;
    KeyFlush();
    gKbdState = arg;
    return 0;
}

void KbdUpdate()
{
    unsigned int a, b, i;
    unsigned int time_ms;
    InputKbd_t kdata;

    if( !gSDLReady || KeyIsHold() ) return;
    while( InputKeyboardGetData( &kdata ) == 1 ) KbdKeyStore( &kdata );
    time_ms = TimerGetSysTime();
    for( i = 0; i < 256; i++ ){
        if( gKbdTimer[ i ].time != -1 ){
            a = ( gKbdTimer[ i ].time > time_ms ) ? 0x7FFFFFFF : time_ms - gKbdTimer[ i ].time;
            b = gKbdTimer[ i ].counter ? gKbdTimeThs1 : gKbdTimeThs0;
            if( a > b ){
                kdata.Pressed = 1;
                kdata.ScanCode = i;
                KbdKeyStore( &kdata );
                gKbdTimer[ i ].time = time_ms;
                gKbdTimer[ i ].counter++;
            }
        }
    }
}

void KbdTimesReset()
{
    int i;

    for( i = 0; i != 256; i++  ){
        gKbdTimer[ i ].time = -1;
        gKbdTimer[ i ].counter = 0;
    }
}

void KbdKeyStore( InputKbd_t *kdata )
{
    short ScanCode;
    int flg;

    flg = 0;
    if( kdata->ScanCode >= 0xC7 ){ //199 Home
        if( kdata->ScanCode > 0xC9 ){//201 PgUp
    	    flg = 1;
	} else {
            if( kdata->ScanCode < 0xCD ){//205 Arrow Rt
                if( kdata->ScanCode == 0xCB ) flg = 1;//203 Arrow Lt
            } else if( kdata->ScanCode <= 0xCD || !(kdata->ScanCode < 0xCF || kdata->ScanCode > 0xD3 ) ){ //205,207,211 Arrow Rt, End, Del
        	flg = 1;
            }
        }
    } else {
	if( kdata->ScanCode < 0xB5 ){ //181 ?
	    if( kdata->ScanCode >= 0x9C && kdata->ScanCode <= 0x9D ) flg = 1; // 156, 157 ?, RCtrl
	} else if ( kdata->ScanCode <= 0xB5 || kdata->ScanCode == 0xb8 ){ //181, 184 ?, RAlt
    	    flg = 1;
	}
    }

    ScanCode = kdata->ScanCode;//gKbdScanLang[ (int)kdata->ScanCode ];
//printf("->%i --> %i\n", kdata->ScanCode, ScanCode );
    if( gRecUnk01 == 1 ){
        if( gRecPlayUnk04 & 0x01 ){
            gRecPlayUnk03 = 2;
            RecClr();
        }
	return;
    }
    if( flg ){
        KeyCodeInsert( 0xE0 );
        ScanCode -= 0x80;
    }
    if( kdata->Pressed ){
        gKbdTimer[ (int)kdata->ScanCode ].time = TimerGetSysTime();
        gKbdTimer[ (int)kdata->ScanCode ].counter = 0;
    } else {
        ScanCode |= 0x80;
        gKbdTimer[ (int)kdata->ScanCode ].time = -1;
    }
    KeyCodeInsert( ScanCode );
}

void KbdThreadSync()
{
    if( gKbdCallbackInit ) gKbdCallbackInit( 0 );
    while( !gSDLReady ){
        KbdUpdate();
        if( gKbdCallbackRun ) gKbdCallbackRun();
    }
    if( gKbdCallbackInit ) gKbdCallbackInit( 1 );
}



