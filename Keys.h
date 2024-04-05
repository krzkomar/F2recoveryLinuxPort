#include "FrameWork.h"

#define KEY_SPACE		32
#define KEY_ENTER		13
#define KEY_ESC			27
#define KEY_BS			8
#define KEY_TAB			9

#define SKEY_CAPS_LOCK		0x02
#define SKEY_NUM_LOCK		0x01
#define SKEY_SCROLL_LOCK	0x04

#define KEYFLAG_CAPS		0x01
#define KEYFLAG_NUMLOCK		0x02	
#define KEYFLAG_SCROLL		0x04	
#define KEYFLAG_LSH		0x08
#define KEYFLAG_RSH		0x010
#define KEYFLAG_LALT		0x020
#define KEYFLAG_RALT		0x040
#define KEYFLAG_LCTR		0x080
#define KEYFLAG_RCTR		0x100

/*
typedef struct 
{
    int i00;
    int i01;
    int i02;
    short ScanCode;
    char xx[10];
} Key01_t;
*/
typedef struct 
{
    union{
	InputKbd_t ScanCode;
	int	   iScanCode;
    };
  short a1;
  short a2;
  short a3;
  short a4;
  short a5;
} KeyMap_t;

typedef struct 
{
    char Code;
    short Flags;
} KeyRaw_t;

extern int	gKeyInitialized;
extern int	gKeyHold;
//extern int	gKeyUnk07;
//extern int	gKeyUnk06;
extern int	gKeyRingHead;
extern int	gKeyRingTail;
extern int	gKeyOffset;
extern int	gKeyCtrl;
extern int	(*gKeyInputCb)( void );

extern KeyRaw_t	gKeyRingBuffer[ 64 ];
extern KeyMap_t gKeyLangMap[ 256 ];
extern char	gKeyStates[ 256 ];
extern int	gKeyTimeStamp;
extern KeyRaw_t	gKeyRaw;
extern int 	gKeyLangId;
extern int	gKeyPressedCnt;

int  		KeyInit();
void 		KeyOff();
void 		KeyClear();
void 		KeyFlush();
int  		KeyGet();
void 		KeySetHold();
void 		KeyClrHold();
int  		KeyIsHold();

void 		KeyNumpadOff();
void 		KeyNumpadOn();
int  		KeyNumpadStat();

void 		KeyNumlockDisable();
void 		KeyNumlockEnable();
int  		KeyNumlockStatus();

void 		KeySetLang( int LangId );
int  		KeyGetLangId();
int  		KeyFindKey( int KeyCode );
unsigned int 	KeyTime();
int  		KeyTimeStamp();
void 		KeyCodeInsert( short Code );
int  		KeyGetLangEnKey();
int  		KeyGetLangFrKey();
int  		KeyGetLangGeKey();
int  		KeyGetLangItKey();
int  		KeyGetLangSpKey();
int  		KeyGetKey();
void 		KeySetLangEn();
void 		KeySetLangFr();
void 		KeySetLangGe();
void 		KeySetLangIt();
void 		KeySetLangSp();
void 		KeyGetControlKey();
int  		KeyPushScanCode( KeyRaw_t *ScanCode );
int  		KbdPopScanCode( KeyRaw_t *pScanCode );
int  		KeyCheckScanCode( int ofset, KeyRaw_t **raw);


int KeyShiftKeyState();

