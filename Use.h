#pragma once

int UseGetScriptId( Obj_t *obj, int *pScrId );
int UseCreateScript( Obj_t *ObjPid, int *ScrId );
int UseUnk03( Obj_t *obj, int ObjType, int VarPid );
int UseUnk02( Obj_t *Critter );
int UseUnk01( Obj_t *Critter, int Reaction );
void UseLookMsg( Obj_t *crit, Obj_t *obj );
void UseLook( Obj_t *crit, Obj_t *obj, void (*OutCb)(char *));
int UseExamineMsg( Obj_t *a1, Obj_t *a2);
int UseExamine( Obj_t *critter, Obj_t *obj, void (*OutCb)(char *) );
int UseUseOn( Obj_t *crit, Obj_t *obj );
int UseUnk04( Obj_t *a1, Obj_t *a2);
int UseUnk05( Obj_t *crit, Obj_t *obj );
int UseUnk06( Obj_t *obj );
int UseReadBook( Obj_t *obj );
int UseLightFlare( Obj_t *crit, Obj_t *obj );
int UseRunScript( Obj_t *obj );
int UseSetTimer( Obj_t *obj );
int UseCarRefuel( Obj_t *obj );
int UseUseMisc( Obj_t *crit );
int UseUnk13( Obj_t *crit, Obj_t *obj );
void UseUseExplosives( Obj_t *a1 );
int UseUnk15( Obj_t *crit, Obj_t *obj );
int UseUnk16( Obj_t *crit, Obj_t *obj1, Obj_t *obj2 );
unsigned int UseUseHealSkill( Obj_t *crit, Obj_t *obj1, Obj_t *obj2 );
int UseUnk18( Obj_t *obj, Obj_t *a2, Obj_t *a3 );
void UseTileUpdate();
int UseApUpdate( Obj_t *obj,  Obj_t *, Obj_t * );
int UseUnk21( Obj_t *crit, Obj_t *obj, Obj_t * );
int UseUnk22( Obj_t *crit, Obj_t *obj );
int UseUnk23( Obj_t *crit, Obj_t *obj );
int UseUnk24( Obj_t *crit, Obj_t *obj );
int UseUnk25();
int UseUnk26( Sound_t *a1 );
int UseUnk27( Sound_t *a1 );
int UseUnk28( Obj_t *obj );
int UseDoor( Obj_t *Crit, Obj_t *obj, int a3 );
int UseSearch( Obj_t *crit, Obj_t *obj );
int UseUseSkill( Obj_t *crit, Obj_t *obj, unsigned int SkillIdx );
int UseObjIsDoor( Obj_t *obj );
int UseObjIsOpenable( Obj_t *obj );
int UseObjLocked( Obj_t *obj );
int UseObjLock( Obj_t *obj );
int UseObjUnlock( Obj_t *obj );
int UseObjIsOpenable1( Obj_t *obj ); // powtorzenie funkcji !
int UseObjInAnimation( Obj_t *obj );
int UseStartAnimation( Obj_t *obj );
int UseRunAnimation1( Obj_t *obj );
int UseRunAnimation2( Obj_t *obj );
int UseObjIsJammed( Obj_t *obj );
int UseObjJam( Obj_t *obj );
int UseObjUnjam( Obj_t *obj );
int UseUnjamAll();
int UseUnk46( Obj_t *obj, int GridIdx, int lvl, int a4 );
int UseUnk47( Obj_t *obj, int a2, int a3 );
int UseAddItem( int Pid, int Quantity );
// Not Used functions 
// UseUnk49( ... );
int UseEditRenderObj( int Win, int Type, int a3, int Pid, int (*Cb)(Proto_t *) );
int UseEditObjSelector( int Pid, int a2, void (*a3)(int, int ), int (*a4)(Proto_t *) );
int UseEditSelectObj( int Pid, int NotUsed, void (*a3)(int, int ) );
int UseEditObjSel( int *pPid, int PidType, int a3, int Flags );
int UseEditObjView( int WinId, Proto_t **a2, char *surf, int pitch, int pid );



