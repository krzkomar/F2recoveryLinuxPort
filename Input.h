#include "FrameWork.h"

#define INPUT_SCAN_CODE_BUFFER_SIZE	32

typedef struct 
{
  int x;
  int y;
  char BtL;
  char BtR;
} InputMouse_t;

typedef struct 
{
  unsigned char ScanCode;
  char Pressed;
} InputKbd_t;

int  InputPoll(); 	// poll event
void *InputGetEvent();  // get pointer to event
int  InputInit();
int  InputRelease();
int  InputMouseAcquire();
void InputMouseUnlink();
int  InputMouseGetData( InputMouse_t *mse );
int  InputKeyboardAcquire();
int  InputKeyboardUnlink();
int  InputKeyboardFlush();
int  InputKeyboardGetData( InputKbd_t *kbd );
int  InputCreateMouse();
void InputCloseMouse();
int  InputCreateKeyboard();
void InputCloseKeyboard();
