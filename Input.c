#include <SDL.h>
#include "FrameWork.h"

static SDL_Event InEvents;
static SDL_Event *InEvent = NULL;

int InputPoll() // added
{
    int tmp;
    if( !InEvent ) return 0;
    tmp = SDL_PollEvent( InEvent );
    if( InEvent->type == SDL_QUIT ){
	SDL_ShowCursor( SDL_ENABLE );
	SDL_Quit();
	printf( "Tymczasowa ewakuacja.\n " );
	exit(0);
    }
    return tmp;
}

void *InputGetEvent() // added
{
    return InEvent;
}

int InputInit()
{
    InEvent = &InEvents;
    return 1;
}

int InputRelease()
{
    InEvent = NULL;
    return 0;
}

int InputMouseAcquire()
{
    return 1;
}

void InputMouseUnlink()
{

}

int InputMouseGetData( InputMouse_t *mse )
{
    int bt;    

    if( !InEvent ) return 0;
    bt = SDL_GetMouseState( &mse->x, &mse->y );    
    mse->x = mse->x / gVidScaleX;
    mse->y = mse->y / gVidScaleY;
    mse->BtL = ( bt & SDL_BUTTON_LMASK ) ? 1 : 0;
    mse->BtR = ( bt & SDL_BUTTON_RMASK ) ? 1 : 0;
    return 1;
}

int InputKeyboardAcquire()
{
    return 0;
}

int InputKeyboardUnlink()
{
    return 0;
}

int InputKeyboardFlush()
{
    return 0;
}

int InputKeyboardGetData( InputKbd_t *kbd )
{
    char key = 0;

    if( !InEvent || !kbd ) return 0;    
    switch( InEvent->type ){
	case SDL_KEYDOWN: key = 1; break;
	case SDL_KEYUP: key = -1; break;
	default: key = 0;
    }    
    if( key == 0 ) return 0;
    kbd->ScanCode = InEvent->key.keysym.sym;
    kbd->Pressed  = key > 0 ? 1:0;
    InputPoll();
//printf("RawKey:%i\n", kbd->ScanCode);
    return 1;    
}

int InputCreateMouse()
{
    return 0;
}

void InputCloseMouse()
{
}

int InputCreateKeyboard()
{
    return 0;
}

void InputCloseKeyboard()
{
}

