#pragma once

enum{
    EV_CHEMUSE_TIMER = 0,
    EV_KNOCKDOWN_TIMER,
    EV_WITHDRAW_TIMER,
    EV_SCRIPT_TIMER,
    EV_MIDNIGHT_TIMER,
    EV_POISON_TIMER,
    EV_RAD_TIMER,
    EV_FLARE_TIMER,
    EV_EXPLOSION_TIMER,
    EV_DEVICE_TIMER,
    EV_SNEAK_TIMER,
    EV_EXPLOSIONONSELF_TIOMER,
    EV_MAPUPDATE_TIMER,
    EV_SNDAMBIENT_TIMER,
};

typedef struct 
{
    int 	i01;
    int 	i02;
    int 	i03;
} EvEntry_t;

typedef struct Ev_t
{
    int 	TimeEv; // in game time of shcheduled event
    int 	Method; // function index
    Obj_t 	*Obj;	// object
    int 	*Ptr;	// argument zwrotny funkcji ładowania
    struct Ev_t *Next;
} Ev_t;

typedef struct 
{
    int 	(*TriggerCb)( Obj_t *, void * );
    void 	(*Free)( void *ptr );
    int  	(*Load)( xFile_t *fh, int **);
    int  	(*Save)( xFile_t *fh, int *);
    int 	RunAuto;
    int 	(*CbAuto)(Obj_t *, int *);
} EvFunc_t;


void EvQeInit();
int  EvQeClose();
int  EvQeLoad( xFile_t *fh );
int  EvQeSave( xFile_t *fh );
int  EvQeSchedule( int GameTime, Obj_t *obj, void *Event, int GridId );
void EvQeRmEvent( Obj_t *obj );
int  EvQeRmEventType( Obj_t *obj, int type );
int  EvQeEnqueued( Obj_t *obj, int Method );
int  EvQeTrig();
void EvQeFlush();
void EvQeRun( int MethodIdx, int (*Cb)(Obj_t *, int *) );
int  EvQeGetTime();
int  EvQeUnk10( Obj_t *obj );
int  EvQeExplosionFail( Obj_t *obj );
int  EvQeExplosionSuccess( Obj_t *obj );
int  EvQeExplode( Obj_t *obj, int a2 ); // use of dynamite and plastic explosives
int  EvQeExplosionOnSelf( Obj_t *Obj );
void EvQeRunAll();
int  EvQeIsEmpty();
int  *EvQeFindPtr( Obj_t *obj, int MethodIdx );
int  *EvQeFindNextPtr( Obj_t *obj, int MethodIdx );
int  EvQeUnk19( Obj_t *obj, int a2 );
int  EvQeUnk20( int a1, int a2, int a3 );
int  EvQeGetReactionInfluence( int, int, int );
int  EvQeUnk22( int a1, int a2 );
int  EvQeUnk23( Obj_t *obj );


