#pragma once

#define OBJ_FLG_NOTREMOVE	0x400	// cause the object cannot be deleted by ObjDelete()

typedef struct 
{
  struct _Obj_t *obj;
  int Quantity;
} ObjStack_t;

typedef struct
{
    int Cnt;	      // 12 occupied box positions
    int Capacity;     // 13 box size
    ObjStack_t *Box;  // 14 box of stacked objects
} ObjBox_t;

typedef struct 
{
    int 	Reaction;
    int 	CurrentAP;
    int 	CombatResult;
    int 	DmgLastTurn;
    int 	AIpackNb;
    int 	GroupId;
    union{
	int 	WhoHitMe;
	struct _Obj_t *WhoHitMeObj; // added
    };
} ObjCritterCond_t;

typedef struct // 0x38
{
    ObjBox_t Box;
    int i04; // 15
    ObjCritterCond_t State;
    int HitPts;
    int Radiated;
    int Poisoned;
} ObjCritter_t;

typedef struct // 0x20
{
    ObjBox_t Box;
    int i04;
    union{
	int Ammo;
	int Charges;
	int KeyCode;
    };
    int AmmoId;
    int Flags;
    int i08;
//    int i09;
//    int i10;
} ObjContainer_t;

typedef struct // 0x28
{
    ObjBox_t Box;
    int i04;
    int DestMapId;
    int DestStartPos;
    int DestMapElev;
    int DestOrientation;
    int i09;
    int i10;
} ObjGrid_t;

typedef struct // 0x28
{
    ObjBox_t Box;
    int i04;
    int i05; // bit0 = 1 - closed door/ bit0 = 0 opened
    int i06;
    int i07;
    int i08;
    int i09;
    int i10;
} ObjScenery_t;

typedef struct _Obj_t // 132
{
    union{
	int ProtoPid;
	int BodyPart;
	int TimeEv;
	struct _Obj_t *obj;
    };
  int GridId;
  int PosX;
  int PosY;
  int Sx;
  int Sy;
  int FrameNo;
  int Orientation;
  int ImgId;
  int Flags; // 0x400 - removable
  int Elevation;	// map level
  union {
    ObjCritter_t  Critter;
    ObjContainer_t Container;// zmienic na item
    ObjScenery_t  Scenery;
    ObjGrid_t  Grid;
  };
  int Pid;
  int CritterIdx;
  int LightRadius;
  int LightIntensity;
  int OutlineColor;
  int ScrId;
  struct _Obj_t *Owner;
  int ScrFNameId;
} Obj_t;

typedef struct _ObjList_t
{
  Obj_t *object;
  struct _ObjList_t *Next;
} ObjList_t;

typedef struct 
{
  int Flags;
  Obj_t *obj;
} ObjTable_t;



enum{
    OBJ_ORI_NE,	// 0
    OBJ_ORI_E,	// 1
    OBJ_ORI_SE,	// 2
    OBJ_ORI_SW,	// 3
    OBJ_ORI_W,	// 4
    OBJ_ORI_NW	// 5
};

#define OBJTYPE( Pid )	((((unsigned int)Pid) >> 24) & 0x0f)
#define OBJIDX( Pid )	(((unsigned int)Pid) & 0xffffff )
/*
    Obj PID format:
    0xAABBCCCC
    AA   - ObjectType:
	    00: items,
	    01: critters
	    02: scenery
	    03: walls
	    04: tiles
	    05: misc
    BB   - Posible part of object ID
    CCCC - ObjectID
*/

extern Obj_t *gObjDude;
extern Obj_t *gObjUnk42;
extern char gObjPalRY[256];
extern char gObjPalBY[256];
extern char *gObjShadeWall;
extern Obj_t *gObjRadius;

//Obj_t *ObjUnk54( Obj_t *, int, int );
//char *ObjGetName( Obj_t *obj );
//char *ObjGetDsc( Obj_t *obj );
//void Unk6012( char *ImgDat, int Width, int Height, int SrcPitch, char *surf, int Xpos, int Ypos, int DstPitch, int Flags, char *Color, char *ColorMap );
//int ObjLightItem( Obj_t *, VidRect_t *);
//int ObjCopy( Obj_t **, int );
//int ObjFSave( xFile_t *fh );
//int ObjLoadDude( xFile_t *fh );
//int ObjOpenable( Obj_t *);
//Obj_t *ObjGetOwner( Obj_t *);
//int ObjSetLight(Obj_t *, int, int, VidRect_t *);
//void ObjCritterListDestroy( void * );
//Obj_t *ObjGetFirst( int, int );
//Obj_t *ObjGetNext();
//Obj_t *ObjUnk46();
//Obj_t *ObjUnk47();

/*
int gObjUnk52[ 6 ][ 36 ];
char gObjLight[ 2*6*8*18 ];
VidRect_t gObjUnk16;
Obj_t *gObjUnk27[100];
VidRect_t gObjViewPortArea;
ObjList_t *gObjGridObjects[ 200*200 ];
char gObjPalRY[256];
char gObjPalBY[256];
int gObjUnk17;
char *gObjIsoSurface;
int gObjUnk14;
Obj_t *gObjRadius; // light player object 
int gObjIsoPitch;
int gObjUnk13;

char gObjUnk79;
char gObjUnk80;
int gObjUnk81[4999];
char gObjUnk01[5003];
int gObjUnk20 = 0;
int gObjGridWinWidth = 0;
int gObjGridWinHeight = 0;
unsigned int gObjGridWinSize = 0;
int *gObjUnk11[ 2 ] = {NULL,NULL};
int *gObjViewPort[ 2 ] = {NULL,NULL};
int *gObjViewPortAxisY = NULL;
int *gObjViewPortAxisX = NULL;
ObjList_t **gObjRenderList = NULL;
int gObjUnk28 = 0;
ObjList_t *gObjOffGridObjs = NULL;
int gObjUnk12 = 0;
int gObjUnk46 = 0;
int gObjUnk45 = 0;
ObjList_t *gObjUnk44 = 0;
int *gObjArtTable = NULL;
unsigned int gObjCnt = 0;
int gObjUnk41 = 0;
int gObjUnk40 = -1;
VidRect_t gObjUnk39[ 9 ] = { 
int gObjUnk38[ 36 ] = {
int gObjViolLvl = -1;
int gObjPlayerMapX = -1;
int gObjPlayerMapY = -1;
int gObjPlayerMapLvl = -1;
int gObjUnk36 = 1;
int *gObjColorA = 0;
int *gObjColorB = 0;
int *gObjColorC = 0;
int *gObjColorD = 0;
int gObjColorE = 0;
int gObjUnk42 = 0;
int gObjUnk43 = 0;
int gObjUnk37[4] = { 1, 0, 3, 5 };
int gObjUnk21;
int ObjSortA( int *a1, int *a2 );
int ObjSortB( int *a1, int *a2 );
int ObjBoxDelete( ObjContainer_t *box );
int ObjDelete( ObjList_t *obl1, ObjList_t *obl2 );
int ObjArtSort( int *Art1, int *Art2 );
*/
    
int ObjInit( char *a1, int Width, int Height, int a4 );
void ObjReset();
void ObjClose();
int ObjLoadObj( Obj_t *obj, xFile_t *fh );
int ObjLoadMapObjs( xFile_t *fh );
int ObjLoadFile( xFile_t *fh );
void ObjLookForAgressor();
void ObjLoad( Obj_t *dude );
int ObjSave( Obj_t *Obj, xFile_t *fh );
int ObjSaveUnk07( xFile_t *fh );
void ObjRenderObjects( VidRect_t *Area, int MapLvl );
void ObjRenderCursor( VidRect_t *Area );
void ObjUnk10( int result, int a2, int a3, unsigned int a4, int a7, int *a6, int a9 );
int ObjCreate( Obj_t **obj, int ArtId, int Pid );
int ObjCopy( Obj_t **pObj, int Pid );
int ObjUnk13( ObjStack_t *stk, Obj_t *item );
int ObjUnk14( Obj_t *obj, unsigned int GridId, int MapLvl, VidRect_t *Area );
int ObjLightItem( Obj_t *Obj, VidRect_t *Area );
int ObjMove( Obj_t *Obj, int dx, int dy, VidRect_t *Area ); 
int ObjPutCursor( Obj_t *Obj, int Xpos, int Ypos, int MapLvl, VidRect_t *Area );
int ObjMoveToTile( Obj_t *obj, unsigned int GridPos, int MapLvl, VidRect_t *pLightArea );
int ObjUnk19();
int ObjSetShape( Obj_t *obj, int ImgId, VidRect_t *Area );
int ObjSetFrame( Obj_t *Crit, int Frame, VidRect_t *Area );
int ObjIncFrame( Obj_t *obj, VidRect_t *Area );
int ObjDecFrame( Obj_t *obj, VidRect_t *Area );
int ObjSetRotation( Obj_t *obj, unsigned int Orientation, VidRect_t *Area );
int ObjTurnCW( Obj_t *obj, VidRect_t *Area );
int ObjTurnCCW( Obj_t *obj, VidRect_t *Area );
void ObjLightGrid();
int ObjSetLight( Obj_t *obj, int LightRadius, int LightIntensity, VidRect_t *Area );
int ObjGetLightIntensity( Obj_t *obj );
int ObjLightedOn( Obj_t *obj, VidRect_t *Area );
int ObjLightedOff( Obj_t *obj, VidRect_t *rect );
int ObjUnk32( Obj_t *obj, VidRect_t *Area );
int ObjUnk33( Obj_t *obj, VidRect_t *Area );
int ObjUnk34( Obj_t *obj, VidRect_t *Area );
int ObjUnk35( Obj_t *obj, VidRect_t *area );
int ObjSetPlayer( Obj_t *obj, VidRect_t *RadArea ); // zla nazwa ?
void ObjUnk37( int Update );
int ObjDestroy( Obj_t *obj, VidRect_t *radius );
int ObjBoxDelete( ObjContainer_t *box );
int ObjOpenable( Obj_t *obj );
int ObjTalkative( Obj_t *obj );
int ObjUnk42( Obj_t *ScenObj );
Obj_t *ObjUnk43( int Id );
Obj_t *ObjGetOwner( Obj_t *obj );
void ObjClear();
Obj_t *ObjGetObjectFirst();
Obj_t *ObjGetObjectNext();
Obj_t *ObjGetVisibleObjectFirst( int MapLvl );
Obj_t *ObjGetVisibleObjectNext();
Obj_t *ObjGetFirst( int MapLvl, int GridIdx );
Obj_t *ObjGetNext();
void ObjGetRadiusArea( Obj_t *obj, VidRect_t *Rect );
int ObjUnk53( int GridIdx, int MapLvl );
Obj_t *ObjReach( Obj_t *obj, int GridIdx, int MapLvl );
Obj_t *ObjUnk55( unsigned int GridIdx, Obj_t *obj, int MapLvl );
Obj_t *ObjUnk56( unsigned int GridIdx, Obj_t *obj, int MapLvl );
int ObjUnk57( int MapIdx, int MapLvl );
Obj_t *ObjUnk58( Obj_t *obj, int GridIdx, int MapLvl );
int ObjGetDistance( Obj_t *crit, Obj_t *obj );
int ObjUnk60( Obj_t *crit, int MapIdx1, Obj_t *obj, int MapIdx2 );
int ObjGetObjList( int GridPos, int MapLvl, int ObjType , Obj_t ***ObjTable );
void ObjCritterListDestroy(void *n );
void ObjUnk63( char *pSrc, int Width, int Height, int SrcPitch, char *pDst, int Xpos, int Ypos, int DstPitch, char *Color, char *ColorMap );
void ObjRenderHatch( char *pSrc, int Width, int Height, int SrcPitch, char *pDst, int Xpos, int Ypos, int DstPitch );
void ObjRenderNormal( char *pSrc, int SrcW, int SrcH, int SrcPitch, char *pDst, int x, int y, int DstPitch, int Shade );
void ObjRenderSemiTr( char *ImgDat, int Width, int Height, int SrcPitch, char *surf, int Xpos, int Ypos, int DstPitch, int Flags, char *Color, char *ColorMap );
void ObjRenderMix( char *Img1, int Width, int Height, int SrcPitch, char *surf, int DstPitch, char *Img2, int Img2Pitch, int inten );
void ObjUnk68( int MapLvl, int HlColor, VidRect_t *Area );
void ObjUnk69( int MapLvl, VidRect_t *Area );
void ObjUnk70( int a1, VidRect_t *a2 );
int ObjSetOutline( Obj_t *obj, int HlColor, VidRect_t *RadiusArea );
int ObjGetRadius( Obj_t *obj, VidRect_t *Area );
void ObjUnk73();
int ObjInteraction( Obj_t *obj, int Xpos, int Ypos );
int ObjTabCreate( int Xpos, int Ypos, int MapLvl, int Type, ObjTable_t **pTable );
void ObjTabDestroy( ObjTable_t **tab );
void ObjUnk77( int a1 );
void ObjUnk78();
void ObjUnk79();
char *ObjGetName( Obj_t *obj );
char *ObjGetDsc( Obj_t *obj );
void ObjUnk80( int flg );
int ObjUnk81();
int ObjViewPortInit();
void ObjFree();
int ObjUnk84();
int ObjSortA( int *a1, int *a2 );
int ObjSortB( int *a1, int *a2 );
void ObjUnk87();
int ObjAllocRenderList();
void ObjUnk89();
void ObjUnk90();
int ObjColorInit();
void ObjUnk91();
void ObjUnk92();
int ObjSaveDude( xFile_t *fh, Obj_t *dude );
int ObjLoadItems( xFile_t *fh, Obj_t **stack, int Lvl );
int ObjFSave( xFile_t *fh );
int ObjLoadDude( xFile_t *fh );
int ObjNewObj( Obj_t **stack );
void ObjUnk95( void **a1 );
int ObjAllocList( ObjList_t **list );
void ObjFreeList( ObjList_t **list );
int ObjFindInList( Obj_t *obj, ObjList_t **list_cur, ObjList_t **list_prev );
void ObjAddObject( ObjList_t *NewObj );
int ObjDelete( ObjList_t *obl1, ObjList_t *obl2 );
int ObjAddObjToList( ObjList_t *pList, int GridPos, int MapLvl, VidRect_t *Area );
int ObjLight( Obj_t *obj, int Dark, VidRect_t *a3);
void ObjRenderHexCursor( Obj_t *obj, VidRect_t *area );
void ObjRender( Obj_t *obj, VidRect_t *Area, int Shade );
void ObjGetArtFileId( int *Id );
int ObjArtSort( int *Art1, int *Art2 );

// inny plik .....
void Unk6012( char *ImgDat, int Width, int Height, int SrcPitch, char *surf, int Xpos, int Ypos, int DstPitch, int Flags, char *Color, char *ColorMap );


