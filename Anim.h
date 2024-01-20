/*
int gAnimCnt1 = 0;
int gAnimIdx = -1;
int gAnim0 = 0;
int gAnimUnk41 = 0;
int gAnimUnk40 = 0;


Anim03_t gAnimUnk23[ 24 ];
Anim02_t gAnimUnk08[ 2000 ];
Anim_t gAnimations[ 32 ];
char gAnimUnk37[ 5000 ];
Anim02_t gAnimUnk01[ 2000 ];
int gAnimSubIdx;
Obj_t *gAnimUnk100[ 100 ];
*/

#include "FrameWork.h"
typedef union
{
    void  *Ptr;
    Obj_t *Obj;    
    int   Int;
} AnimU_t;


typedef struct 
{
    int State;			// machine state
    AnimU_t Target;		//     
    void *GpPtr;		// general purpose pointer, holds obj_t or sound_t
    int TargetPos;		// 
    int Elevation;		// 
    int Silence;		// 1 - noisy, 19 - stealth
    int i07;			// 
    int (*Callback11)(void *, void *);	// 
    int (*Callback12)(Obj_t *, Obj_t *, int );	// 
    int i10;			// 
    AnimU_t Ap; 		// action points/object/skill no
    CachePool_t *ImgObj;	// 
} Anim01_t;

typedef struct
{
  int GridIdx;
  int i02;
  char Orientation;
  int Distance;
  int i05;
}  Anim02_t;

typedef struct 
{
    int GridPos; // gridpos | orientation ?
    int i02; // map lvl ?
    int i03; // x
    int i04; // y
} Anim04_t;

typedef struct 
{
    int Flags;
    Obj_t *Obj;
    int ArtId;
    int i04;
    int Time;
    int Speed;
    int AnimIdx;
    int Ap;
    int Stat;
    int GridPos;
    union{
	Anim04_t Tab[ 200 ];
	unsigned char Path[ 800 ];
    };
} Anim03_t;

typedef struct // size of 0xa60 2656
{
  int i01;
  int Step;
  int Steps;	// count of used Anim01_t 'Steps'
  int Flags;
  Anim01_t i05[ 55 ];
} Anim_t;

/****************************/

int AnimInit();
void AnimInit_00();
void AnimClose();
int AnimStart( int Type );
int AnimReserve( int flg );
int AnimUnk33( int a1 );
int AnimClear( Obj_t *obj );
int AnimBegin();
int AnimTestExist( int Id, CachePool_t **Img );
void AnimStop();
int AnimCancel( Obj_t *obj );
int AnimUnk39( Obj_t *a1 );
int AnimObjMoveToObj( Obj_t *Object, Obj_t *DstObj, int Ap, int a4 );
int AnimObjRunToObj( Obj_t *obj1, Obj_t *obj2, int Ap, int a4 );
int AnimObjMoveToTile( Obj_t *obj, int TargetPos, int MapLvl, int Ap, int a5 );
int AnimObjRunToTile( Obj_t *obj, int GridId, int MapLvl, int Ap, int a5 );
int AnimUnk44( Obj_t *obj, int GridIdx, int MapLvl, int a4, int a5 );
int AnimUnk45( Obj_t *obj, int GridIdx, int MapLvl, int a4, int a5 );
int AnimUnk46( Obj_t *obj1, Obj_t *obj2, int a3 );
int AnimUnk47( Obj_t *obj, int a2 );
int AnimUnk48( Obj_t *obj, int a2, int a3 );
int AnimRegAnimReverse( Obj_t *a1, int a2, int a3 );
int AnimRegAnim( Obj_t *obj, int a2, int a3 );
int AnimUnk51( Obj_t *obj, int a2 );
int AnimTurnCCW( Obj_t *obj );
int AnimTurnCW( Obj_t *a1 );
int AnimUnk54( Obj_t *a1 );
int AnimUnk55( Obj_t *a1 );
int AnimSetCallback11( Obj_t *a1, AnimU_t a2, int (*a3)(void *, void *), int a4 );
int AnimSetCallback12( Obj_t *a1, Obj_t *a2, AnimU_t Ap, int (*Callback)(Obj_t *, Obj_t *, int), int a5 );
int AnimSetFinish( void *a1, Obj_t *a2, int (*a3)(Obj_t *,Obj_t *), int a4 );
int AnimUnk59( Obj_t *a1, int a2, int a3 );
int AnimUnk60( Obj_t *a1, int a2, int a3 );
int AnimUnk61( Obj_t *a1, int a2 );
int AnimUnk62( Obj_t *a1, int a2, int a3 );
int AnimUnk63( Obj_t *a1, int a2, int a3 );
int AnimChangeItemStart( Obj_t *obj, int TargetPos, int a3 );
int AnimUnk65( Obj_t *obj, int TargetPos, int a3 );
int AnimRegPlaySfx( Obj_t *who, char *file, int delay );
int AnimRegAnimate( Obj_t *what, int anim, int delay );
int AnimUnk68( int a1, int a2 );
int AnimStateMachine( int AnimIdx );
int AnimUnk70( int AnimIdx, int Flg );
int AnimEnd( int idx );
int AnimUnk71( Obj_t *a1, Obj_t *a2 );
int AnimFindTrace( Obj_t *obj, int BeginPos, int EndPos, unsigned char *Trace, int NotApproach );
int AnimMakeTrace( Obj_t *obj, int BeginPos, int EndPos, unsigned char *Trace, int NotApproach, Obj_t *(*AccessibleCb)(Obj_t *, int, int));
int AnimDiagonal( int x0, int y0, int x1, int y1 );
int AnimDistance( int BeginPos, int EndPos );
int AnimUnk06( Obj_t *obj, int GridPos1, int GridPos2, Anim04_t *t, Obj_t **pRet, int flg );
int AnimUnk07( Obj_t *obj, int GridPos1, int GridPos2, Anim04_t *t, Obj_t **pRet, int a6, Obj_t *(*Cb)( Obj_t *, int, int ) );
int AnimUnk08( Obj_t *a1, Obj_t *a2, int a3, int a4, int a5 );
int AnimUnk09( Obj_t *obj, int GridId, int a3, int a4, int a5, Anim04_t *a6, Obj_t **pObj );
int AnimUnk10( Obj_t *obj, unsigned int MapIdx, int MapLvl, int Ap, int Silence, int AnimIdx );
int AnimCreateTrace( Obj_t *obj, int DstIdx, int MapLvl, int Ap, int a5, int a6, int AnimIdx );
int AnimUnk12( Obj_t *obj, int a2, int a3, int a4, int a5, int Flags );
int AnimUnk13( Obj_t *obj, int a2, int a3, int a4, int a5 );
int AnimUnk14( Obj_t *obj, int a2, int a3 );
void AnimMovement( int AnimIdx );
void AnimUnk16( int AnimIdx );
int AnimUnk17( Obj_t *obj, int a2, int AnimIdx, int a4 );
// task callback, in game animations process
void AnimProcess();
void AnimClean();
int AnimGetTarget( int *Ap );
int AnimWalk( int Ap );
int AnimRun( int Ap );
// task callback, ambient animations
void AnimAmbient();
void AnimUnk24( Obj_t *obj, unsigned int Orientation, int ArtId );
int AnimUnk25( Obj_t *obj );
int AnimUnk26( Obj_t *obj, int MapIdx, int a2 );
int AnimUnk27( Obj_t *obj, int turn, int AnimIdx );
int AnimUnk28( Obj_t *obj, int AnimIdx );
int AnimUnk29( Obj_t *obj, int AnimIdx, int a3 );
void AnimReset();
int AnimUnk30( int GridIdx, int Cnt );
unsigned int AnimGetSpeed( Obj_t *dude, int ImgId );



