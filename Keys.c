#include <SDL.h>
#include "FrameWork.h"


#define KEY_RingBufPop() 	{ if( gKeyRingTail != gKeyRingHead ) gKeyRingTail = ( gKeyRingTail + 1 ) & 0x3F; }

// keypad keys scancodes
#define KEY_SNUM_0	82
#define KEY_SNUM_1	79
#define KEY_SNUM_2	80
#define KEY_SNUM_3	81
#define KEY_SNUM_4	75
#define KEY_SNUM_5	76
#define KEY_SNUM_6	77
#define KEY_SNUM_7	71
#define KEY_SNUM_8	72
#define KEY_SNUM_9	73
#define KEY_SNUM_NUM	69
#define KEY_SNUM_SLASH	181
#define KEY_SNUM_AST	55
#define KEY_SNUM_MINUS	74
#define KEY_SNUM_PLUS	78
#define KEY_SNUM_ENTER	156
#define KEY_SNUM_DEL	83

#define KEY_LSHIFT	42
#define KEY_RSHIFT	54
#define KEY_LALT	56
#define KEY_RALT	184
#define KEY_LCTRL	29
#define KEY_RCTRL	157
#define KEY_SCROLL_LOCK	70
#define KEY_NUM_LOCK	69
#define KEY_CAPS_LOCK	58


int		gKeyInitialized = 0;
int		gKeyHold = 0;
int		gKeyNumpadOff = 0;
int		gKeyNumlockOff = 0;
int		gKeyRingHead = 0;
int		gKeyRingTail = 0;
int		gKeyOffset = 0;
int		gKeyCtrl = 0;
int		(*gKeyInputCb)( void )=  KeyGetLangEnKey;

KeyRaw_t	gKeyRingBuffer[ 64 ];
KeyMap_t 	gKeyLangMap[ 256 ];
char		gKeyStates[ 256 ];
int		gKeyTimeStamp;
KeyRaw_t	gKeyRaw;
int 		gKeyLangId;
int		gKeyPressedCnt;

int KeyInit()
{
    if( gKeyInitialized ) return -1;
    gKeyInitialized = 1;
    gKeyPressedCnt = 0;
    memset( gKeyStates, 0, 256 );
    gKeyRingHead = 0;
    gKeyRingTail = 0;
    InputKeyboardFlush();
    KbdTimesReset();
    KeyGetControlKey();
    KeySetLang( 0 );
    gKeyTimeStamp = TimerGetSysTime();
    return 0;
}

void KeyOff()
{
    if( gKeyInitialized ) gKeyInitialized = 0;
}

void KeyClear()
{
    if( !gKeyInitialized ) return;
    gKeyPressedCnt = 0;
    memset(gKeyStates, 0, 256 );
    gKeyRingHead = 0;
    gKeyRingTail = 0;
    InputKeyboardFlush();
    KbdTimesReset();
    do
        KbdUpdate();
    while( !gKeyPressedCnt );
    if( gKeyInitialized ){
        gKeyPressedCnt = 0;
        memset( gKeyStates, 0, 256 );
        gKeyRingHead = 0;
        gKeyRingTail = 0;
    }
    InputKeyboardFlush();
    KbdTimesReset();
}

void KeyFlush()
{
    if( gKeyInitialized ){
        gKeyPressedCnt = 0;
        memset( gKeyStates, 0, 256 );
        gKeyRingHead = 0;
        gKeyRingTail = 0;
    }
    InputKeyboardFlush();
    KbdTimesReset();
}

int KeyGet()
{
    if( gKeyInitialized ) return gKeyInputCb();
    return -1;
}

void KeySetHold()
{
    gKeyHold = 1;
}

void KeyClrHold()
{
    gKeyHold = 0;
}

int KeyIsHold()
{
    return gKeyHold;
}

void KeyNumpadOff()
{
    gKeyNumpadOff = 1;
}

void KeyNumpadOn()
{
    gKeyNumpadOff = 0;
}

int KeyNumapdStat()
{
    return gKeyNumpadOff;
}

void KeyNumlockDisable()
{
    gKeyNumlockOff = 1;
}

void KeyNumlockEnable()
{
    gKeyNumlockOff = 0;
}

int KeyNumlockStatus()
{
    return gKeyNumlockOff;
}

void KeySetLang( int LangId )
{
    int tmp;

    tmp = gKeyLangId;
    gKeyLangId = LangId;
    switch( LangId ){
        case 0:  gKeyInputCb = KeyGetLangEnKey; KeySetLangEn(); break;
        case 1:  gKeyInputCb = KeyGetLangFrKey; KeySetLangFr(); break;
        case 2:  gKeyInputCb = KeyGetLangGeKey; KeySetLangGe(); break;
        case 3:  gKeyInputCb = KeyGetLangItKey; KeySetLangIt(); break;
        case 4:  gKeyInputCb = KeyGetLangSpKey; KeySetLangSp(); break;
        default: gKeyLangId = tmp; break;
    }
}

int KeyGetLangId()
{
    return gKeyLangId;
}

int KeyFindKey( int KeyCode )
{
    int i;

    for( i = 0; i < 256; i++ ){
        if( 
    	    gKeyLangMap[ i ].a1 == KeyCode || 
    	    gKeyLangMap[ i ].a2 == KeyCode || 
    	    gKeyLangMap[ i ].a3 == KeyCode || 
    	    gKeyLangMap[ i ].a4 == KeyCode || 
    	    gKeyLangMap[ i ].a5 == KeyCode 
    	  )
          return i;
    }
    return -1;
}

unsigned int KeyTime()
{
    return TimerCurrDiff(gKeyTimeStamp);
}

int KeyTimeStamp()
{
    return gKeyTimeStamp = TimerGetSysTime();
}

void KeyCodeInsert( short Code )
{
    Rec_t *p;
    short pressed;
    unsigned short ScanCode;

//printf("==>%x\n", Code);
    if( !gRecState ){
        if( gRecFrame != 4095 ){
    	    p = &gRec[ gRecFrame++ ];
            p->i00 = 2;            
            p->mx = Code;
            p->i01 = gRecPlayUnk05;
            p->i02 = gRecUnk04;
        }
    }

    gKeyTimeStamp = TimerGetSysTime();
    if( Code == 0xe0 ){ // extended code
        gKeyOffset = 128;
    } else {
	if( (Code & 0x80) == 0 ){
    	    pressed = 1;
	} else {
    	    pressed = 0;
    	    Code &= ~0x80;
	}
	ScanCode = gKeyOffset + (Code & 0xff);
	if( pressed && gKeyStates[ ScanCode ] ) pressed = 2;
	if( gKeyStates[ ScanCode ] != pressed ){
	    gKeyStates[ ScanCode ] = pressed;
    	    if( pressed == 1 ){
        	gKeyPressedCnt++;
    	    } else if( !pressed ){
        	gKeyPressedCnt--;
    	    }
	}
	if( pressed ){
    	    gKeyRaw.Code = ScanCode;
    	    gKeyRaw.Flags = 0;
    	    if( ScanCode != KEY_CAPS_LOCK ){
        	if( ScanCode == KEY_NUM_LOCK){
            	    if( !(gKeyStates[ KEY_LCTRL ] || gKeyStates[ KEY_RCTRL ]) ){
                	if( gKeyCtrl & SKEY_NUM_LOCK ) gKeyCtrl &= ~SKEY_NUM_LOCK; else gKeyCtrl |= SKEY_NUM_LOCK;
            	    }
        	} else if( ScanCode == KEY_SCROLL_LOCK ){
            	    if( !(gKeyStates[ KEY_LCTRL ] || gKeyStates[ KEY_RCTRL ]) ){
                	if( gKeyCtrl & SKEY_SCROLL_LOCK ) gKeyCtrl &= ~SKEY_SCROLL_LOCK; else gKeyCtrl |= SKEY_SCROLL_LOCK;
            	    }
        	} else if( (ScanCode == KEY_LSHIFT || ScanCode == KEY_RSHIFT ) && (gKeyCtrl & SKEY_CAPS_LOCK) && gKeyLangId ){
            	    if( gKeyCtrl & SKEY_CAPS_LOCK ) gKeyCtrl &= ~SKEY_CAPS_LOCK; else gKeyCtrl |= SKEY_CAPS_LOCK;
        	}
    	    } else {
    		if( !gKeyStates[ KEY_LCTRL ] && !gKeyStates[ KEY_RCTRL ] ){
        	    if( !(gKeyCtrl & SKEY_CAPS_LOCK) ){
        		gKeyCtrl |= SKEY_CAPS_LOCK;
        	    } else {
        		if( !gKeyLangId ){
            		    if( gKeyCtrl & SKEY_CAPS_LOCK ) gKeyCtrl &= ~SKEY_CAPS_LOCK; else gKeyCtrl |= SKEY_CAPS_LOCK;
        		}
        	    }
    		}
    	    }
    	    if( gKeyCtrl ){
        	if( ( gKeyCtrl & SKEY_NUM_LOCK ) && !gKeyNumlockOff ) gKeyRaw.Flags |= KEYFLAG_NUMLOCK;
        	if( gKeyCtrl & SKEY_CAPS_LOCK ) gKeyRaw.Flags |= KEYFLAG_CAPS;
        	if( gKeyCtrl & SKEY_SCROLL_LOCK ) gKeyRaw.Flags |= KEYFLAG_SCROLL;
    	    }
    	    if( gKeyStates[ KEY_LSHIFT ] ) gKeyRaw.Flags |= KEYFLAG_LSH;
    	    if( gKeyStates[ KEY_RSHIFT ] ) gKeyRaw.Flags |= KEYFLAG_RSH;
    	    if( gKeyStates[ KEY_LALT   ] ) gKeyRaw.Flags |= KEYFLAG_LALT;
    	    if( gKeyStates[ KEY_RALT   ] ) gKeyRaw.Flags |= KEYFLAG_RALT;
    	    if( gKeyStates[ KEY_LCTRL  ] ) gKeyRaw.Flags |= KEYFLAG_LCTR;
    	    if( gKeyStates[ KEY_RCTRL  ] ) gKeyRaw.Flags |= KEYFLAG_RCTR;
    	    if( ((gKeyRingHead + 1) & 0x3F) != gKeyRingTail ){
//printf("=++>%c\n", gKeyRaw.Code);
        	gKeyRingBuffer[ gKeyRingHead++ ] = gKeyRaw;
        	gKeyRingHead &= 0x3F;
    	    }
	}
	gKeyOffset = 0;
    }
    if( gKeyStates[198] ){ //??
        if( gKeyInitialized ){
            gKeyPressedCnt = 0;
            memset( gKeyStates, 0, 256 );
            gKeyRingHead = 0;
            gKeyRingTail = 0;
        }
        InputKeyboardFlush();
        KbdTimesReset();
    }
}

int KeyGetLangEnKey()
{
    int v2;
    char v3;
    int v4;
    int v6;
    int v8;
    int v9;
    int v10;
    KeyRaw_t *tmp;

    if( KeyCheckScanCode( 0, &tmp ) ) return -1;
    if( (tmp->Flags & 1) != 0 ){
        v2 = (gKeyLangId == 1) ? 16 : 30;
        v4 = (gKeyLangId != 1) ? '2' : '\'';
        v8 = gKeyLangId != 1 ? '\x11' : ',';
        v6 = gKeyLangId != 1 ? 16 : '\x1E';
        v3 = tmp->Code;
        v10 = gKeyLangId ? (gKeyLangId != 1 ? (gKeyLangId != 4 ? (gKeyLangId != 3 ? '\x15' : 44 ) : ',') : '\x11' ) : ',';
	v9 = gKeyLangId ? (gKeyLangId != 1 ? (gKeyLangId != 4 ? ( gKeyLangId != 3 ? ',' : '\x15') : '\x15') : '\x15') : '\x15'; 
        if( 
    	    v3 == '0' || v3 == '.' || v3 == ' ' || v3 == '\x12' || v3 == '!' || v3 == '"' || v3 == '#' || v3 == '\x17' || v3 == '$' || v3 == '%' || v3 == '&' || 
    	    v3 == '1' || v3 == '\x18' || v3 == '\x19' || v3 == '\x13' || v3 == '\x1F' || v3 == '\x14' || v3 == '\x16' || v3 == '/' || v3 == '-' || 
    	    v3 == v4 || v3 == v2 || v3 == v8 || v3 == v6 || v3 == v9 || v3 == v10
        )
        {
            if( tmp->Flags & ( 0x08 | 0x10 ) )
                tmp->Flags &= ~( 0x10 | 0x01 | 0x08 );
            else
                tmp->Flags |= 0x08;
        }
    }
    return KeyGetKey();
}

int KeyGetLangFrKey()
{
    /* -- cut -- */
    return KeyGetLangEnKey(); // dummy
}

int KeyGetLangGeKey()
{
    /* -- cut -- */
    return KeyGetLangEnKey(); // dummy
}

int KeyGetLangItKey()
{
    /* -- cut -- */
    return KeyGetLangEnKey(); // dummy
}

int KeyGetLangSpKey()
{
    /* -- cut -- */
    return KeyGetLangEnKey(); // dummy
}


int KeyGetKey()
{
    int KeyCode;
    unsigned char code;
    KeyRaw_t *pScanCode;

    KeyCode = -1;
    if( KeyCheckScanCode(0, &pScanCode) ) return -1;
    code = pScanCode->Code;
//printf("=++>%c\n", code);
//    if( gKeyNumpadOff && ( code == KEY_SNUM_SLASH || code == KEY_SNUM_AST || code == KEY_SNUM_MINUS || code == KEY_SNUM_PLUS || code == KEY_SNUM_ENTER )  ){
//        KEY_RingBufPop();
//        return -1;
//    }
/*
    if( code == KEY_SNUM_0 || code == KEY_SNUM_1 || code == KEY_SNUM_2 || code == KEY_SNUM_3 || 
	code == KEY_SNUM_4 || code == KEY_SNUM_5 || code == KEY_SNUM_6 || code == KEY_SNUM_7 || 
	code == KEY_SNUM_8 || code == KEY_SNUM_9 || code == KEY_SNUM_DEL 
      ){
	if( gKeyNumpadOff ){
	    KEY_RingBufPop();
	    return -1;
	}
    	if( (pScanCode->Flags & 0x60) == 0 && (pScanCode->Flags & 2) != 0 ){
    	    if( (pScanCode->Flags & 0x18) != 0 )
    	        pScanCode->Flags &= 0xE7;
    	    else
                pScanCode->Flags |= 0x08;
    	}	
    }
*/
//    if( pScanCode->Flags & 0x180 ){
        KeyCode = pScanCode->Code; //gKeyLangMap[ (int)pScanCode->Code ].a5;
//    } else {
//        if( (pScanCode->Flags & 0x20) != 0 ){
//            KeyCode = pScanCode->Code; //gKeyLangMap[ (int)pScanCode->Code ].a3;
//        } else if( pScanCode->Flags & 0x40 ){
//            KeyCode = pScanCode->Code; //gKeyLangMap[ (int)pScanCode->Code ].a4;
//        } else if( pScanCode->Flags & 0x18 ){
//            KeyCode = pScanCode->Code; //gKeyLangMap[ (int)pScanCode->Code ].a2;
//        } else {
//            KeyCode = pScanCode->Code; //gKeyLangMap[ (int)pScanCode->Code ].a1;
//        }
//    }

    KEY_RingBufPop();
//printf("==>%c\n", KeyCode);
    return KeyCode;
}

void KeySetLangEn()
{
    int i;

    for( i = 0; i != 256; i++ ){        
        gKeyLangMap[ i ].iScanCode = -1;
        gKeyLangMap[ i ].a1 = -1;
        gKeyLangMap[ i ].a2 = -1;
        gKeyLangMap[ i ].a3 = -1;
        gKeyLangMap[ i ].a4 = -1;
        gKeyLangMap[ i ].a5 = -1;
    }
    gKeyLangMap[ 1].a1 = 27;  gKeyLangMap[1 ].a2 = 27;  gKeyLangMap[ 1].a3 = 27;  gKeyLangMap[ 1].a4 = 27;  gKeyLangMap[ 1].a5 = 27;
    gKeyLangMap[59].a1 = 315; gKeyLangMap[59].a2 = 340; gKeyLangMap[59].a3 = 360; gKeyLangMap[59].a4 = 360; gKeyLangMap[59].a5 = 350;
    gKeyLangMap[60].a1 = 316; gKeyLangMap[60].a2 = 341; gKeyLangMap[60].a3 = 361; gKeyLangMap[60].a4 = 361; gKeyLangMap[60].a5 = 351;
    gKeyLangMap[61].a1 = 317; gKeyLangMap[61].a2 = 342; gKeyLangMap[61].a3 = 362; gKeyLangMap[61].a4 = 362; gKeyLangMap[61].a5 = 352;
    gKeyLangMap[62].a1 = 318; gKeyLangMap[62].a2 = 343; gKeyLangMap[62].a3 = 363; gKeyLangMap[62].a4 = 363; gKeyLangMap[62].a5 = 353;
    gKeyLangMap[63].a1 = 319; gKeyLangMap[63].a2 = 344; gKeyLangMap[63].a3 = 364; gKeyLangMap[63].a4 = 364; gKeyLangMap[63].a5 = 354;
    gKeyLangMap[64].a1 = 320; gKeyLangMap[64].a2 = 345; gKeyLangMap[64].a3 = 365; gKeyLangMap[64].a4 = 365; gKeyLangMap[64].a5 = 355;
    gKeyLangMap[65].a1 = 321; gKeyLangMap[65].a2 = 346; gKeyLangMap[65].a3 = 366; gKeyLangMap[65].a4 = 366; gKeyLangMap[65].a5 = 356;
    gKeyLangMap[66].a1 = 322; gKeyLangMap[66].a2 = 347; gKeyLangMap[66].a3 = 367; gKeyLangMap[66].a4 = 367; gKeyLangMap[66].a5 = 357;
    gKeyLangMap[67].a1 = 323; gKeyLangMap[67].a2 = 348; gKeyLangMap[67].a3 = 368; gKeyLangMap[67].a4 = 368; gKeyLangMap[67].a5 = 358;
    gKeyLangMap[68].a1 = 324; gKeyLangMap[68].a2 = 349; gKeyLangMap[68].a3 = 369; gKeyLangMap[68].a4 = 369; gKeyLangMap[68].a5 = 359;
    gKeyLangMap[87].a1 = 389; gKeyLangMap[87].a2 = 391; gKeyLangMap[87].a3 = 395; gKeyLangMap[87].a4 = 395; gKeyLangMap[87].a5 = 393;
    gKeyLangMap[88].a1 = 390; gKeyLangMap[88].a2 = 392; gKeyLangMap[88].a3 = 396; gKeyLangMap[88].a4 = 396; gKeyLangMap[88].a5 = 394;

    switch( gKeyLangId ){
        case 0:  i = 41; break;
        case 1:  i = 3;  break;
        case 4:  i = 0;  break;
        case 3:  i = 0;  break;
        default: i = 27; break;
    }
    gKeyLangMap[i].a1 = 96; gKeyLangMap[i].a2 = 126; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1; 
    
    gKeyLangMap[ 2].a1 = 49; gKeyLangMap[ 2].a2 = 33; gKeyLangMap[ 2].a3 = -1; gKeyLangMap[ 2].a4 = -1; gKeyLangMap[ 2].a5 = -1;
    gKeyLangMap[ 3].a1 = 50; gKeyLangMap[ 3].a2 = 64; gKeyLangMap[ 3].a3 = -1; gKeyLangMap[ 3].a4 = -1; gKeyLangMap[ 3].a5 = -1; 
    gKeyLangMap[ 4].a1 = 51; gKeyLangMap[ 4].a2 = 35; gKeyLangMap[ 4].a3 = -1; gKeyLangMap[ 4].a4 = -1; gKeyLangMap[ 4].a5 = -1; 
    gKeyLangMap[ 5].a1 = 52; gKeyLangMap[ 5].a2 = 36; gKeyLangMap[ 5].a3 = -1; gKeyLangMap[ 5].a4 = -1; gKeyLangMap[ 5].a5 = -1; 
    gKeyLangMap[ 6].a1 = 53; gKeyLangMap[ 6].a2 = 37; gKeyLangMap[ 6].a3 = -1; gKeyLangMap[ 6].a4 = -1; gKeyLangMap[ 6].a5 = -1; 
    gKeyLangMap[ 7].a1 = 54; gKeyLangMap[ 7].a2 = 94; gKeyLangMap[ 7].a3 = -1; gKeyLangMap[ 7].a4 = -1; gKeyLangMap[ 7].a5 = -1; 
    gKeyLangMap[ 8].a1 = 55; gKeyLangMap[ 8].a2 = 38; gKeyLangMap[ 8].a3 = -1; gKeyLangMap[ 8].a4 = -1; gKeyLangMap[ 8].a5 = -1;
    gKeyLangMap[ 9].a1 = 56; gKeyLangMap[ 9].a2 = 42; gKeyLangMap[ 9].a3 = -1; gKeyLangMap[ 9].a4 = -1; gKeyLangMap[ 9].a5 = -1; 
    gKeyLangMap[10].a1 = 57; gKeyLangMap[10].a2 = 40; gKeyLangMap[10].a3 = -1; gKeyLangMap[10].a4 = -1; gKeyLangMap[10].a5 = -1; 
    gKeyLangMap[11].a1 = 48; gKeyLangMap[11].a2 = 41; gKeyLangMap[11].a3 = -1; gKeyLangMap[11].a4 = -1; gKeyLangMap[11].a5 = -1;

    i = ( gKeyLangId ) ? (( gKeyLangId == 1 ) ? 7 : 53 ) : 12;    
    gKeyLangMap[i].a1 = 45; gKeyLangMap[i].a2 = 95; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    i = ( gKeyLangId ) ? (( gKeyLangId == 1 ) ? 13 : 11) : 13;    
    gKeyLangMap[i].a1 = 61; gKeyLangMap[i].a2 = 43; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    gKeyLangMap[14].a1 = 8; gKeyLangMap[14].a2 = 8; gKeyLangMap[14].a3 = 8; gKeyLangMap[14].a4 = 8; gKeyLangMap[14].a5 = 127;
    gKeyLangMap[15].a1 = 9; gKeyLangMap[15].a2 = 9; gKeyLangMap[15].a3 = 9; gKeyLangMap[15].a4 = 9; gKeyLangMap[15].a5 = 9;

    i = ( gKeyLangId ) ? (( gKeyLangId == 1 ) ? 30 : 16 ) : 16;    
    gKeyLangMap[i].a1 = 113; gKeyLangMap[i].a2 = 81; gKeyLangMap[i].a3 = 272; gKeyLangMap[i].a4 = 272; gKeyLangMap[i].a5 = 17;

    i = ( gKeyLangId ) ? (( gKeyLangId == 1 ) ? 44 : 17) : 17;    
    gKeyLangMap[i].a1 = 119; gKeyLangMap[i].a2 = 87; gKeyLangMap[i].a3 = 273; gKeyLangMap[i].a4 = 273; gKeyLangMap[i].a5 = 23;

    gKeyLangMap[18].a1 = 101; gKeyLangMap[18].a2 = 69; gKeyLangMap[18].a3 = 274; gKeyLangMap[18].a4 = 274; gKeyLangMap[18].a5 = 5;
    gKeyLangMap[19].a1 = 114; gKeyLangMap[19].a2 = 82; gKeyLangMap[19].a3 = 275; gKeyLangMap[19].a4 = 275; gKeyLangMap[19].a5 = 18;
    gKeyLangMap[20].a1 = 116; gKeyLangMap[20].a2 = 84; gKeyLangMap[20].a3 = 276; gKeyLangMap[20].a4 = 276; gKeyLangMap[20].a5 = 20;

    switch( gKeyLangId ){
        case 0:  i = 21; break;
        case 1:  i = 21; break;
        case 4:  i = 21; break;
        case 3:  i = 21; break;
        default: i = 44;break;
    }
    gKeyLangMap[i].a1 = 121; gKeyLangMap[i].a2 = 89; gKeyLangMap[i].a3 = 277; gKeyLangMap[i].a4 = 277; gKeyLangMap[i].a5 = 25;

    gKeyLangMap[22].a1 = 117; gKeyLangMap[22].a2 = 85; gKeyLangMap[22].a3 = 278; gKeyLangMap[22].a4 = 278; gKeyLangMap[22].a5 = 21;
    gKeyLangMap[23].a1 = 105; gKeyLangMap[23].a2 = 73; gKeyLangMap[23].a3 = 279; gKeyLangMap[23].a4 = 279; gKeyLangMap[23].a5 = 9;
    gKeyLangMap[24].a1 = 111; gKeyLangMap[24].a2 = 79; gKeyLangMap[24].a3 = 280; gKeyLangMap[24].a4 = 280; gKeyLangMap[24].a5 = 15;
    gKeyLangMap[25].a1 = 112; gKeyLangMap[25].a2 = 80; gKeyLangMap[25].a3 = 281; gKeyLangMap[25].a4 = 281; gKeyLangMap[25].a5 = 16;

    switch ( gKeyLangId ){
        case 0:  i = 26;break;
        case 1:  i = 6; break;
        case 4:  i = 26;break;
        case 3:  i = 26;break;
        default: i = 9; break;
    }
    gKeyLangMap[i].a1 = 91; gKeyLangMap[i].a2 = 123; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    switch ( gKeyLangId ){
        case 0: i = 27; break;
        case 1: i = 12; break;
        case 4: i = 27; break;
        case 3: i = 27; break;
        default:i = 10; break;
    }
    gKeyLangMap[i].a1 = 93;  gKeyLangMap[i].a2 = 125; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    switch( gKeyLangId ){
        case 0:  i = 43; break;
        case 1:  i = 9;  break;
        case 4:  i = 41; break;
        case 3:  i = 41; break;
        default: i = 12; break;
    }
    gKeyLangMap[i].a1 = 92; gKeyLangMap[i].a2 = 124; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = 192;
    gKeyLangMap[58].a1 = -1; gKeyLangMap[58].a2 = -1; gKeyLangMap[58].a3 = -1; gKeyLangMap[58].a4 = -1; gKeyLangMap[58].a5 = -1;

    i = ( gKeyLangId == 1 ) ? 16 : 30;
    gKeyLangMap[i].a1 = 97; gKeyLangMap[i].a2 = 65; gKeyLangMap[i].a3 = 286; gKeyLangMap[i].a4 = 286; gKeyLangMap[i].a5 = 1;

    gKeyLangMap[31].a1 = 115; gKeyLangMap[31].a2 = 83;
    strcpy( (char *)&gKeyLangMap[31].a3, "\x1F\x01\x1F\x01\x13" );
    gKeyLangMap[32].a1 = 100; gKeyLangMap[32].a2 = 68;
    memcpy( &gKeyLangMap[32].a3, " \x01 \x01", 4 );
    gKeyLangMap[32].a5 = 4;

    gKeyLangMap[33].a1 = 102; gKeyLangMap[33].a2 = 70; gKeyLangMap[33].a3 = 289; gKeyLangMap[33].a4 = 289; gKeyLangMap[33].a5 = 6;
    gKeyLangMap[34].a1 = 103; gKeyLangMap[34].a2 = 71; gKeyLangMap[34].a3 = 290; gKeyLangMap[34].a4 = 290; gKeyLangMap[34].a5 = 7;
    gKeyLangMap[35].a1 = 104; gKeyLangMap[35].a2 = 72; gKeyLangMap[35].a3 = 291; gKeyLangMap[35].a4 = 291; gKeyLangMap[35].a5 = 8;
    gKeyLangMap[36].a1 = 106; gKeyLangMap[36].a2 = 74; gKeyLangMap[36].a3 = 292; gKeyLangMap[36].a4 = 292; gKeyLangMap[36].a5 = 10;
    gKeyLangMap[37].a1 = 107; gKeyLangMap[37].a2 = 75; gKeyLangMap[37].a3 = 293; gKeyLangMap[37].a4 = 293; gKeyLangMap[37].a5 = 11;
    gKeyLangMap[38].a1 = 108; gKeyLangMap[38].a2 = 76; gKeyLangMap[38].a3 = 294; gKeyLangMap[38].a4 = 294; gKeyLangMap[38].a5 = 12;

    i = ( gKeyLangId ) ? 51 : 39;
    gKeyLangMap[i].a1 = 59; gKeyLangMap[i].a2 = 58; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    if( gKeyLangId ){
        if( gKeyLangId == 1 ){
            i = 4;
        } else if( gKeyLangId == 4 || (i = gKeyLangId, gKeyLangId != 3) ){
            i = 3;
        }
    } else {
        i = 40;
    }
    gKeyLangMap[i].a1 = 39; gKeyLangMap[i].a2 = 34; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    gKeyLangMap[28].a1 = 13; gKeyLangMap[28].a2 = 13; gKeyLangMap[28].a3 = 13; gKeyLangMap[28].a4 = 13; gKeyLangMap[28].a5 = 10;
    gKeyLangMap[42].a1 = -1; gKeyLangMap[42].a2 = -1; gKeyLangMap[42].a3 = -1; gKeyLangMap[42].a4 = -1; gKeyLangMap[42].a5 = -1;
    switch( gKeyLangId ){
        case 0:  i = 44; break;
        case 1:  i = 17; break;
        case 4:  i = 44; break;
        case 3:  i = 44; break;
        default: i = 21; break;
    }
    gKeyLangMap[i].a1 = 122; gKeyLangMap[i].a2 = 90; gKeyLangMap[i].a3 = 300; gKeyLangMap[i].a4 = 300; gKeyLangMap[i].a5 = 26;
        
    gKeyLangMap[45].a1 = 120; gKeyLangMap[45].a2 = 88; gKeyLangMap[45].a3 = 301; gKeyLangMap[45].a4 = 301; gKeyLangMap[45].a5 = 24;
    gKeyLangMap[46].a1 = 99;  gKeyLangMap[46].a2 = 67; gKeyLangMap[46].a3 = 302; gKeyLangMap[46].a4 = 302; gKeyLangMap[46].a5 = 3;
    gKeyLangMap[47].a1 = 118; gKeyLangMap[47].a2 = 86; gKeyLangMap[47].a3 = 303; gKeyLangMap[47].a4 = 303; gKeyLangMap[47].a5 = 22;
    gKeyLangMap[48].a1 = 98;  gKeyLangMap[48].a2 = 66; gKeyLangMap[48].a3 = 304; gKeyLangMap[48].a4 = 304; gKeyLangMap[48].a5 = 2;
    gKeyLangMap[49].a1 = 110; gKeyLangMap[49].a2 = 78; gKeyLangMap[49].a3 = 305; gKeyLangMap[49].a4 = 305; gKeyLangMap[49].a5 = 14;

    i = ( gKeyLangId ) ? ( ( gKeyLangId == 1 ) ? 39 : 50) : 50;
    gKeyLangMap[i].a1 = 109; gKeyLangMap[i].a2 = 77; gKeyLangMap[i].a3 = 306; gKeyLangMap[i].a4 = 306; gKeyLangMap[i].a5 = 13;
    i = ( gKeyLangId ) ? ( ( gKeyLangId == 1 ) ? 50 : 51) : 51;
    gKeyLangMap[i].a1 = 44; gKeyLangMap[i].a2 = 60; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;
    i = ( gKeyLangId ) ? ( ( gKeyLangId == 1 ) ? 51 : 52) : 52;    
    gKeyLangMap[i].a1 = 46; gKeyLangMap[i].a2 = 62; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;
    i = ( gKeyLangId ) ? ( ( gKeyLangId == 1 ) ? 52 : 8) : 53;    
    gKeyLangMap[i].a1 = 47; gKeyLangMap[i].a2 = 63; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;    

    gKeyLangMap[ 54].a1 = -1;  gKeyLangMap[ 54].a2 = -1;  gKeyLangMap[ 54].a3 = -1;  gKeyLangMap[ 54].a4 = -1;  gKeyLangMap[ 54].a5 = -1;
    gKeyLangMap[ 29].a1 = -1;  gKeyLangMap[ 29].a2 = -1;  gKeyLangMap[ 29].a3 = -1;  gKeyLangMap[ 29].a4 = -1;  gKeyLangMap[ 29].a5 = -1;
    gKeyLangMap[ 56].a1 = -1;  gKeyLangMap[ 56].a2 = -1;  gKeyLangMap[ 56].a3 = -1;  gKeyLangMap[ 56].a4 = -1;  gKeyLangMap[ 56].a5 = -1;
    gKeyLangMap[ 57].a1 = 32;  gKeyLangMap[ 57].a2 = 32;  gKeyLangMap[ 57].a3 = 32;  gKeyLangMap[ 57].a4 = 32;  gKeyLangMap[ 57].a5 = 32;
    gKeyLangMap[184].a1 = -1;  gKeyLangMap[184].a2 = -1;  gKeyLangMap[184].a3 = -1;  gKeyLangMap[184].a4 = -1;  gKeyLangMap[184].a5 = -1;
    gKeyLangMap[157].a1 = -1;  gKeyLangMap[157].a2 = -1;  gKeyLangMap[157].a3 = -1;  gKeyLangMap[157].a4 = -1;  gKeyLangMap[157].a5 = -1;
    gKeyLangMap[210].a1 = 338; gKeyLangMap[210].a2 = 338; gKeyLangMap[210].a3 = 418; gKeyLangMap[210].a4 = 418; gKeyLangMap[210].a5 = 402;
    gKeyLangMap[199].a1 = 327; gKeyLangMap[199].a2 = 327; gKeyLangMap[199].a3 = 407; gKeyLangMap[199].a4 = 407; gKeyLangMap[199].a5 = 375;
    gKeyLangMap[201].a1 = 329; gKeyLangMap[201].a2 = 329; gKeyLangMap[201].a3 = 409; gKeyLangMap[201].a4 = 409; gKeyLangMap[201].a5 = 388;
    gKeyLangMap[211].a1 = 339; gKeyLangMap[211].a2 = 339; gKeyLangMap[211].a3 = 419; gKeyLangMap[211].a4 = 419; gKeyLangMap[211].a5 = 403;
    gKeyLangMap[207].a1 = 335; gKeyLangMap[207].a2 = 335; gKeyLangMap[207].a3 = 415; gKeyLangMap[207].a4 = 415; gKeyLangMap[207].a5 = 373;
    gKeyLangMap[209].a1 = 337; gKeyLangMap[209].a2 = 337; gKeyLangMap[209].a3 = 417; gKeyLangMap[209].a4 = 417; gKeyLangMap[209].a5 = 374;
    gKeyLangMap[200].a1 = 328; gKeyLangMap[200].a2 = 328; gKeyLangMap[200].a3 = 408; gKeyLangMap[200].a4 = 408; gKeyLangMap[200].a5 = 397;
    gKeyLangMap[208].a1 = 336; gKeyLangMap[208].a2 = 336; gKeyLangMap[208].a3 = 416; gKeyLangMap[208].a4 = 416; gKeyLangMap[208].a5 = 401;
    gKeyLangMap[203].a1 = 331; gKeyLangMap[203].a2 = 331; gKeyLangMap[203].a3 = 411; gKeyLangMap[203].a4 = 411; gKeyLangMap[203].a5 = 371;
    gKeyLangMap[205].a1 = 333; gKeyLangMap[205].a2 = 333; gKeyLangMap[205].a3 = 413; gKeyLangMap[205].a4 = 413; gKeyLangMap[205].a5 = 372;
    gKeyLangMap[ 69].a1 = -1;  gKeyLangMap[ 69].a2 = -1;  gKeyLangMap[ 69].a3 = -1;  gKeyLangMap[ 69].a4 = -1;  gKeyLangMap[ 69].a5 = -1;
    gKeyLangMap[181].a1 = 47;  gKeyLangMap[181].a2 = 47;  gKeyLangMap[181].a3 = -1;  gKeyLangMap[181].a4 = -1;  gKeyLangMap[181].a5 = 3;
    gKeyLangMap[ 55].a1 = 42;  gKeyLangMap[ 55].a2 = 42;  gKeyLangMap[ 55].a3 = -1;  gKeyLangMap[ 55].a4 = -1;  gKeyLangMap[ 55].a5 = -1;
    gKeyLangMap[ 74].a1 = 45;  gKeyLangMap[ 74].a2 = 45;  gKeyLangMap[ 74].a3 = -1;  gKeyLangMap[ 74].a4 = -1;  gKeyLangMap[ 74].a5 = -1;
    gKeyLangMap[ 71].a1 = 327; gKeyLangMap[ 71].a2 = 55;  gKeyLangMap[ 71].a3 = 407; gKeyLangMap[ 71].a4 = 407; gKeyLangMap[ 71].a5 = 375;
    gKeyLangMap[ 72].a1 = 328; gKeyLangMap[ 72].a2 = 56;  gKeyLangMap[ 72].a3 = 408; gKeyLangMap[ 72].a4 = 408; gKeyLangMap[ 72].a5 = 397;
    gKeyLangMap[ 73].a1 = 329; gKeyLangMap[ 73].a2 = 57;  gKeyLangMap[ 73].a3 = 409; gKeyLangMap[ 73].a4 = 409; gKeyLangMap[ 73].a5 = 388;
    gKeyLangMap[ 78].a1 = 43;  gKeyLangMap[ 78].a2 = 43;  gKeyLangMap[ 78].a3 = -1;  gKeyLangMap[ 78].a4 = -1;  gKeyLangMap[ 78].a5 = -1;
    gKeyLangMap[ 75].a1 = 331; gKeyLangMap[ 75].a2 = 52;  gKeyLangMap[ 75].a3 = 411; gKeyLangMap[ 75].a4 = 411; gKeyLangMap[ 75].a5 = 371;
    gKeyLangMap[ 76].a1 = 332; gKeyLangMap[ 76].a2 = 53;  gKeyLangMap[ 76].a3 = 9999;gKeyLangMap[ 76].a4 = 9999;gKeyLangMap[ 76].a5 = 399;
    gKeyLangMap[ 77].a1 = 333; gKeyLangMap[ 77].a2 = 54;  gKeyLangMap[ 77].a3 = 413; gKeyLangMap[ 77].a4 = 413; gKeyLangMap[ 77].a5 = 372;
    gKeyLangMap[ 79].a1 = 335; gKeyLangMap[ 79].a2 = 49;  gKeyLangMap[ 79].a3 = 415; gKeyLangMap[ 79].a4 = 415; gKeyLangMap[ 79].a5 = 373;
    gKeyLangMap[ 80].a1 = 336; gKeyLangMap[ 80].a2 = 50;  gKeyLangMap[ 80].a3 = 416; gKeyLangMap[ 80].a4 = 416; gKeyLangMap[ 80].a5 = 401;
    gKeyLangMap[ 81].a1 = 337; gKeyLangMap[ 81].a2 = 51;  gKeyLangMap[ 81].a3 = 417; gKeyLangMap[ 81].a4 = 417; gKeyLangMap[ 81].a5 = 374;
    gKeyLangMap[156].a1 = 13;  gKeyLangMap[156].a2 = 13;  gKeyLangMap[156].a3 = -1;  gKeyLangMap[156].a4 = -1;  gKeyLangMap[156].a5 = -1;
    gKeyLangMap[ 82].a1 = 338; gKeyLangMap[ 82].a2 = 48;  gKeyLangMap[ 82].a3 = 418; gKeyLangMap[ 82].a4 = 418; gKeyLangMap[ 82].a5 = 402;
    gKeyLangMap[ 83].a1 = 339; gKeyLangMap[ 83].a2 = 46;  gKeyLangMap[ 83].a3 = -1;  gKeyLangMap[ 83].a4 = 419; gKeyLangMap[ 83].a5 = 403;
}

void KeySetLangFr()
{
    int i;

    KeySetLangEn();
    gKeyLangMap[41].a1 = 178;gKeyLangMap[41].a2 = -1;  gKeyLangMap[41].a3 = -1; gKeyLangMap[41].a4 = -1; gKeyLangMap[41].a5 = -1;
    gKeyLangMap[ 2].a1 = 38; gKeyLangMap[ 2].a2 = 49;  gKeyLangMap[ 2].a3 = -1; gKeyLangMap[ 2].a4 = -1; gKeyLangMap[ 2].a5 = -1;
    gKeyLangMap[ 3].a1 = 233;gKeyLangMap[ 3].a2 = 50;  gKeyLangMap[ 3].a3 = -1; gKeyLangMap[ 3].a4 = 152;gKeyLangMap[ 3].a5 = -1;
    gKeyLangMap[ 4].a1 = 34; gKeyLangMap[ 4].a2 = 51;  gKeyLangMap[ 4].a3 = -1; gKeyLangMap[ 4].a4 = 35; gKeyLangMap[ 4].a5 = -1;
    gKeyLangMap[ 5].a1 = 39; gKeyLangMap[ 5].a2 = 52;  gKeyLangMap[ 5].a3 = -1; gKeyLangMap[ 5].a4 = 123;gKeyLangMap[ 5].a5 = -1;
    gKeyLangMap[ 6].a1 = 40; gKeyLangMap[ 6].a2 = 53;  gKeyLangMap[ 6].a3 = -1; gKeyLangMap[ 6].a4 = 91; gKeyLangMap[ 6].a5 = -1;
    gKeyLangMap[ 7].a1 = 150;gKeyLangMap[ 7].a2 = 54;  gKeyLangMap[ 7].a3 = -1; gKeyLangMap[ 7].a4 = 166;gKeyLangMap[ 7].a5 = -1;
    gKeyLangMap[ 8].a1 = 232;gKeyLangMap[ 8].a2 = 55;  gKeyLangMap[ 8].a3 = -1; gKeyLangMap[ 8].a4 = 96; gKeyLangMap[ 8].a5 = -1;
    gKeyLangMap[ 9].a1 = 95; gKeyLangMap[ 9].a2 = 56;  gKeyLangMap[ 9].a3 = -1; gKeyLangMap[ 9].a4 = 92; gKeyLangMap[ 9].a5 = -1;
    gKeyLangMap[10].a1 = 231;gKeyLangMap[10].a2 = 57;  gKeyLangMap[10].a3 = -1; gKeyLangMap[10].a4 = 136;gKeyLangMap[10].a5 = -1;
    gKeyLangMap[11].a1 = 224;gKeyLangMap[11].a2 = 48;  gKeyLangMap[11].a3 = -1; gKeyLangMap[11].a4 = 64; gKeyLangMap[11].a5 = -1;
    gKeyLangMap[12].a1 = 41; gKeyLangMap[12].a2 = 176; gKeyLangMap[12].a3 = -1; gKeyLangMap[12].a4 = 93; gKeyLangMap[12].a5 = -1;

    i = ( gKeyLangId ) ? (( gKeyLangId == 1 ) ? 13 : 11) : 13;    
    gKeyLangMap[i].a1 = 61; gKeyLangMap[i].a2 = 43; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = 125; gKeyLangMap[i].a5 = -1;

    gKeyLangMap[26].a1 = 136; gKeyLangMap[26].a2 = 168; gKeyLangMap[26].a3 = -1; gKeyLangMap[26].a4 = -1;  gKeyLangMap[26].a5 = -1;
    gKeyLangMap[27].a1 = 36;  gKeyLangMap[27].a2 = 163; gKeyLangMap[27].a3 = -1; gKeyLangMap[27].a4 = 164; gKeyLangMap[27].a5 = -1;
    gKeyLangMap[40].a1 = 249; gKeyLangMap[40].a2 = 37;  gKeyLangMap[40].a3 = -1; gKeyLangMap[40].a4 = -1;  gKeyLangMap[40].a5 = -1;
    gKeyLangMap[43].a1 = 42;  gKeyLangMap[43].a2 = 181; gKeyLangMap[43].a3 = -1; gKeyLangMap[43].a4 = -1;  gKeyLangMap[43].a5 = -1;
    gKeyLangMap[86].a1 = 60;  gKeyLangMap[86].a2 = 62;  gKeyLangMap[86].a3 = -1; gKeyLangMap[86].a4 = -1;  gKeyLangMap[86].a5 = -1;

    i = ( gKeyLangId ) ? (( gKeyLangId == 1 ) ? 50 : 51) : 51;    
    gKeyLangMap[i].a1 = 44; gKeyLangMap[i].a2 = 63; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    i = ( gKeyLangId ) ? 51 : 39;
    gKeyLangMap[i].a1 = 59; gKeyLangMap[i].a2 = 46; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    i = ( gKeyLangId ) ? 52 : 39;
    gKeyLangMap[i].a1 = 58; gKeyLangMap[i].a2 = 47; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;
    gKeyLangMap[53].a1 = 33; gKeyLangMap[53].a2 = 167; gKeyLangMap[53].a3 = -1; gKeyLangMap[53].a4 = -1; gKeyLangMap[53].a5 = -1;
}

void KeySetLangGe()
{
    int i;

    KeySetLangEn();
    gKeyLangMap[41].a1 = 136; gKeyLangMap[41].a2 = 186; gKeyLangMap[41].a3 = -1; gKeyLangMap[41].a4 = -1; gKeyLangMap[41].a5 = -1;
    gKeyLangMap[3].a1 = 50;   gKeyLangMap[3].a2 = 34;   gKeyLangMap[3].a3 = -1;  gKeyLangMap[3].a4 = 178; gKeyLangMap[3].a5 = -1;
    gKeyLangMap[4].a1 = 51;   gKeyLangMap[4].a2 = 167;  gKeyLangMap[4].a3 = -1;  gKeyLangMap[4].a4 = 179; gKeyLangMap[4].a5 = -1;
    gKeyLangMap[7].a1 = 54;   gKeyLangMap[7].a2 = 38;   gKeyLangMap[7].a3 = -1;  gKeyLangMap[7].a4 = -1;  gKeyLangMap[7].a5 = -1;
    gKeyLangMap[8].a1 = 55;   gKeyLangMap[8].a2 = 166;  gKeyLangMap[8].a3 = -1;  gKeyLangMap[8].a4 = 123; gKeyLangMap[8].a5 = -1;
    gKeyLangMap[9].a1 = 56;   gKeyLangMap[9].a2 = 40;   gKeyLangMap[9].a3 = -1;  gKeyLangMap[9].a4 = 91;  gKeyLangMap[9].a5 = -1;
    gKeyLangMap[10].a1 = 57;  gKeyLangMap[10].a2 = 41;  gKeyLangMap[10].a3 = -1; gKeyLangMap[10].a4 = 93; gKeyLangMap[10].a5 = -1;
    gKeyLangMap[11].a1 = 48;  gKeyLangMap[11].a2 = 61;  gKeyLangMap[11].a3 = -1; gKeyLangMap[11].a4 = 125;gKeyLangMap[11].a5 = -1;
    gKeyLangMap[12].a1 = 223; gKeyLangMap[12].a2 = 63;  gKeyLangMap[12].a3 = -1; gKeyLangMap[12].a4 = 92; gKeyLangMap[12].a5 = -1;
    gKeyLangMap[13].a1 = 180; gKeyLangMap[13].a2 = 96;  gKeyLangMap[13].a3 = -1; gKeyLangMap[13].a4 = -1; gKeyLangMap[13].a5 = -1;

    i = ( gKeyLangId == 1 ) ? 30 : 16;
    gKeyLangMap[i].a1 = 113; gKeyLangMap[i].a2 = 81; gKeyLangMap[i].a3 = 272; gKeyLangMap[i].a4 = 64; gKeyLangMap[i].a5 = 17;
    gKeyLangMap[26].a1 = 252; gKeyLangMap[26].a2 = 220; gKeyLangMap[26].a3 = -1; gKeyLangMap[26].a4 = -1; gKeyLangMap[26].a5 = -1;

    i = ( gKeyLangId ) ? (( gKeyLangId == 1 ) ? 13 : 27 ) : 13;
    gKeyLangMap[i].a1 = 43; gKeyLangMap[i].a2 = 42; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = 152; gKeyLangMap[i].a5 = -1;

    gKeyLangMap[39].a1 = 246; gKeyLangMap[39].a2 = 214; gKeyLangMap[39].a3 = -1; gKeyLangMap[39].a4 = -1;  gKeyLangMap[39].a5 = -1;
    gKeyLangMap[40].a1 = 228; gKeyLangMap[40].a2 = 196; gKeyLangMap[40].a3 = -1; gKeyLangMap[40].a4 = -1;  gKeyLangMap[40].a5 = -1;
    gKeyLangMap[43].a1 = 35;  gKeyLangMap[43].a2 = 39;  gKeyLangMap[43].a3 = -1; gKeyLangMap[43].a4 = -1;  gKeyLangMap[43].a5 = -1;
    gKeyLangMap[86].a1 = 60;  gKeyLangMap[86].a2 = 62;  gKeyLangMap[86].a3 = -1; gKeyLangMap[86].a4 = 166; gKeyLangMap[86].a5 = -1;

    i = ( gKeyLangId == 1 ) ? 39 : 50;
    gKeyLangMap[i].a1 = 109; gKeyLangMap[i].a2 = 77; gKeyLangMap[i].a3 = 306; gKeyLangMap[i].a4 = 181; gKeyLangMap[i].a5 = 13;

    i = ( gKeyLangId == 1 ) ? 50 : 51;
    gKeyLangMap[i].a1 = 44; gKeyLangMap[i].a2 = 59; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    i = ( gKeyLangId ) ? (( gKeyLangId == 1 ) ? 51 : 52 ) : 52;    
    gKeyLangMap[i].a1 = 46; gKeyLangMap[i].a2 = 58; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    i = ( gKeyLangId ) ? (( gKeyLangId == 1 ) ? 7 : 53 ) : 12;    
    gKeyLangMap[i].a1 = 150; gKeyLangMap[i].a2 = 151; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    gKeyLangMap[181].a1 = 247; gKeyLangMap[181].a2 = 247; gKeyLangMap[181].a3 = -1; gKeyLangMap[181].a4 = -1; gKeyLangMap[181].a5 = 3;
    gKeyLangMap[55].a1 = 215;  gKeyLangMap[55].a2 = 215;  gKeyLangMap[55].a3 = -1;  gKeyLangMap[55].a4 = -1;  gKeyLangMap[55].a5 = -1;
    gKeyLangMap[83].a1 = 339;  gKeyLangMap[83].a2 = 44;   gKeyLangMap[83].a3 = -1;  gKeyLangMap[83].a4 = 419; gKeyLangMap[83].a5 = 403;
}

void KeySetLangIt()
{
    int i;

    KeySetLangEn();
    gKeyLangMap[41].a1 = 92;  gKeyLangMap[41].a2 = 124;  gKeyLangMap[41].a3 = -1;  gKeyLangMap[41].a4 = -1;  gKeyLangMap[41].a5 = -1;
    gKeyLangMap[86].a1 = 60;  gKeyLangMap[86].a2 = 62;   gKeyLangMap[86].a3 = -1;  gKeyLangMap[86].a4 = -1;  gKeyLangMap[86].a5 = -1;
    gKeyLangMap[2].a1 = 49;   gKeyLangMap[2].a2 = 33;    gKeyLangMap[2].a3 = -1;   gKeyLangMap[2].a4 = -1;   gKeyLangMap[2].a5 = -1;
    gKeyLangMap[3].a1 = 50;   gKeyLangMap[3].a2 = 34;    gKeyLangMap[3].a3 = -1;   gKeyLangMap[3].a4 = -1;   gKeyLangMap[3].a5 = -1;
    gKeyLangMap[4].a1 = 51;   gKeyLangMap[4].a2 = 163;   gKeyLangMap[4].a3 = -1;   gKeyLangMap[4].a4 = -1;   gKeyLangMap[4].a5 = -1;
    gKeyLangMap[7].a1 = 54;   gKeyLangMap[7].a2 = 38;    gKeyLangMap[7].a3 = -1;   gKeyLangMap[7].a4 = -1;   gKeyLangMap[7].a5 = -1;
    gKeyLangMap[8].a1 = 55;   gKeyLangMap[8].a2 = 47;    gKeyLangMap[8].a3 = -1;   gKeyLangMap[8].a4 = -1;   gKeyLangMap[8].a5 = -1;
    gKeyLangMap[9].a1 = 56;   gKeyLangMap[9].a2 = 40;    gKeyLangMap[9].a3 = -1;   gKeyLangMap[9].a4 = -1;   gKeyLangMap[9].a5 = -1;
    gKeyLangMap[10].a1 = 57;  gKeyLangMap[10].a2 = 41;   gKeyLangMap[10].a3 = -1;  gKeyLangMap[10].a4 = -1;  gKeyLangMap[10].a5 = -1;
    gKeyLangMap[11].a1 = 48;  gKeyLangMap[11].a2 = 61;   gKeyLangMap[11].a3 = -1;  gKeyLangMap[11].a4 = -1;  gKeyLangMap[11].a5 = -1;
    gKeyLangMap[12].a1 = 39;  gKeyLangMap[12].a2 = 63;   gKeyLangMap[12].a3 = -1;  gKeyLangMap[12].a4 = -1;  gKeyLangMap[12].a5 = -1;
    gKeyLangMap[13].a1 = 236; gKeyLangMap[13].a2 = 94;   gKeyLangMap[13].a3 = -1;  gKeyLangMap[13].a4 = -1;  gKeyLangMap[13].a5 = -1;
    gKeyLangMap[26].a1 = 232; gKeyLangMap[26].a2 = 233;  gKeyLangMap[26].a3 = -1;  gKeyLangMap[26].a4 = 91;  gKeyLangMap[26].a5 = -1;
    gKeyLangMap[27].a1 = 43;  gKeyLangMap[27].a2 = 42;   gKeyLangMap[27].a3 = -1;  gKeyLangMap[27].a4 = 93;  gKeyLangMap[27].a5 = -1;
    gKeyLangMap[39].a1 = 242; gKeyLangMap[39].a2 = 231;  gKeyLangMap[39].a3 = -1;  gKeyLangMap[39].a4 = 64;  gKeyLangMap[39].a5 = -1;
    gKeyLangMap[40].a1 = 224; gKeyLangMap[40].a2 = 176;  gKeyLangMap[40].a3 = -1;  gKeyLangMap[40].a4 = 35;  gKeyLangMap[40].a5 = -1;
    gKeyLangMap[43].a1 = 249; gKeyLangMap[43].a2 = 167;  gKeyLangMap[43].a3 = -1;  gKeyLangMap[43].a4 = -1;  gKeyLangMap[43].a5 = -1;

    i = ( gKeyLangId == 1 ) ? 50 : 51;
    gKeyLangMap[i].a1 = 44; gKeyLangMap[i].a2 = 59; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;
    i = ( gKeyLangId == 1 ) ? 51 : 52;
    gKeyLangMap[i].a1 = 46; gKeyLangMap[i].a2 = 58; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;
    i = ( gKeyLangId ) ? (( gKeyLangId == 1 ) ? 7 : 53 ) : 12;    
    gKeyLangMap[i].a1 = 45; gKeyLangMap[i].a2 = 95; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;
}

void KeySetLangSp()
{
    int i;

    KeySetLangEn();
    gKeyLangMap[ 2].a1 = 49;  gKeyLangMap[ 2].a2 = 33;  gKeyLangMap[ 2].a3 = -1; gKeyLangMap[ 2].a4 = 124; gKeyLangMap[ 2].a5 = -1;
    gKeyLangMap[ 3].a1 = 50;  gKeyLangMap[ 3].a2 = 34;  gKeyLangMap[ 3].a3 = -1; gKeyLangMap[ 3].a4 = 64;  gKeyLangMap[ 3].a5 = -1;
    gKeyLangMap[ 4].a1 = 51;  gKeyLangMap[ 4].a2 = 149; gKeyLangMap[ 4].a3 = -1; gKeyLangMap[ 4].a4 = 35;  gKeyLangMap[ 4].a5 = -1;
    gKeyLangMap[ 7].a1 = 54;  gKeyLangMap[ 7].a2 = 38;  gKeyLangMap[ 7].a3 = -1; gKeyLangMap[ 7].a4 = 172; gKeyLangMap[ 7].a5 = -1;
    gKeyLangMap[ 8].a1 = 55;  gKeyLangMap[ 8].a2 = 47;  gKeyLangMap[ 8].a3 = -1; gKeyLangMap[ 8].a4 = -1;  gKeyLangMap[ 8].a5 = -1;
    gKeyLangMap[ 9].a1 = 56;  gKeyLangMap[ 9].a2 = 40;  gKeyLangMap[ 9].a3 = -1; gKeyLangMap[ 9].a4 = -1;  gKeyLangMap[ 9].a5 = -1;
    gKeyLangMap[10].a1 = 57;  gKeyLangMap[10].a2 = 41;  gKeyLangMap[10].a3 = -1; gKeyLangMap[10].a4 = -1;  gKeyLangMap[10].a5 = -1;
    gKeyLangMap[11].a1 = 48;  gKeyLangMap[11].a2 = 61;  gKeyLangMap[11].a3 = -1; gKeyLangMap[11].a4 = -1;  gKeyLangMap[11].a5 = -1;
    gKeyLangMap[12].a1 = 146; gKeyLangMap[12].a2 = 63;  gKeyLangMap[12].a3 = -1; gKeyLangMap[12].a4 = -1;  gKeyLangMap[12].a5 = -1;
    gKeyLangMap[13].a1 = 161; gKeyLangMap[13].a2 = 191; gKeyLangMap[13].a3 = -1; gKeyLangMap[13].a4 = -1;  gKeyLangMap[13].a5 = -1;
    gKeyLangMap[41].a1 = 176; gKeyLangMap[41].a2 = 170; gKeyLangMap[41].a3 = -1; gKeyLangMap[41].a4 = 92;  gKeyLangMap[41].a5 = -1;
    gKeyLangMap[26].a1 = 96;  gKeyLangMap[26].a2 = 94;  gKeyLangMap[26].a3 = -1; gKeyLangMap[26].a4 = 91;  gKeyLangMap[26].a5 = -1;
    gKeyLangMap[27].a1 = 43;  gKeyLangMap[27].a2 = 42;  gKeyLangMap[27].a3 = -1; gKeyLangMap[27].a4 = 93;  gKeyLangMap[27].a5 = -1;
    gKeyLangMap[86].a1 = 60;  gKeyLangMap[86].a2 = 62;  gKeyLangMap[86].a3 = -1; gKeyLangMap[86].a4 = -1;  gKeyLangMap[86].a5 = -1;
    gKeyLangMap[39].a1 = 241; gKeyLangMap[39].a2 = 209; gKeyLangMap[39].a3 = -1; gKeyLangMap[39].a4 = -1;  gKeyLangMap[39].a5 = -1;
    gKeyLangMap[40].a1 = 168; gKeyLangMap[40].a2 = 180; gKeyLangMap[40].a3 = -1; gKeyLangMap[40].a4 = 123; gKeyLangMap[40].a5 = -1;
    gKeyLangMap[43].a1 = 231; gKeyLangMap[43].a2 = 199; gKeyLangMap[43].a3 = -1; gKeyLangMap[43].a4 = 125; gKeyLangMap[43].a5 = -1;

    i = ( gKeyLangId == 1 ) ? 50 : 51;
    gKeyLangMap[i].a1 = 44; gKeyLangMap[i].a2 = 59; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    i = ( gKeyLangId == 1 ) ? 51 : 52;
    gKeyLangMap[i].a1 = 46; gKeyLangMap[i].a2 = 58; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;

    i = ( gKeyLangId ) ? (( gKeyLangId == 1 ) ? 7 : 53 ) : 12;
    gKeyLangMap[i].a1 = 45; gKeyLangMap[i].a2 = 95; gKeyLangMap[i].a3 = -1; gKeyLangMap[i].a4 = -1; gKeyLangMap[i].a5 = -1;
}

void KeyGetControlKey()
{
    int len = 0;
    const unsigned char  *keystates;

    keystates = SDL_GetKeyboardState( &len );
    if( len <= 0 ) return;
    if( keystates[ SDL_SCANCODE_CAPSLOCK ]     ) gKeyCtrl |= KEY_CAPS_LOCK;
    if( keystates[ SDL_SCANCODE_NUMLOCKCLEAR ] ) gKeyCtrl |= KEY_NUM_LOCK;
    if( keystates[ SDL_SCANCODE_SCROLLLOCK ]   ) gKeyCtrl |= KEY_SCROLL_LOCK;
}

int KeyPushScanCode( KeyRaw_t *ScanCode )
{
    if( ((gKeyRingHead + 1) & 0x3F) == gKeyRingTail ) return -1; // buffer full
    gKeyRingBuffer[ gKeyRingHead ] = *ScanCode;
    gKeyRingHead = (gKeyRingHead + 1 ) & 0x3F;
    return 0;
}

int KeyPopScanCode( KeyRaw_t *pScanCode )
{
    if( gKeyRingTail == gKeyRingHead ) return -1;
    if( pScanCode ) *pScanCode = gKeyRingBuffer[ gKeyRingTail ];
    gKeyRingTail = ( gKeyRingTail + 1 ) & 0x3F;
    return 0;    
}

int KeyCheckScanCode( int ofset, KeyRaw_t **raw)
{
    int KeyFlg;
    int nKey;

    KeyFlg = -1;
    if( gKeyRingTail != gKeyRingHead ){
        nKey = ( gKeyRingHead <= gKeyRingTail ) ? ( gKeyRingHead + 64 - gKeyRingTail - 1 ):( gKeyRingHead - gKeyRingTail - 1 );
        if( ofset <= nKey ){
            KeyFlg = 0;
            *raw = &gKeyRingBuffer[ (gKeyRingTail + ofset) & 0x3F ];
        }
    }
    return KeyFlg;
}


/* extra */

int KeyShiftKeyState()
{
    char *key = SDL_GetKeyboardState( NULL );
    int flg = 0;

    if( !key ) return 0;
//    SDL_PumpEvents();
    flg |= key[ SDL_SCANCODE_LSHIFT ] ? 1 : 0;
    flg |= key[ SDL_SCANCODE_RSHIFT ] ? 2 : 0;
    return flg;
}

