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
int UseDropObj( Obj_t *crit, Obj_t *obj );
int UseUnk06( Obj_t *obj );
int UseReadBook( Obj_t *obj );
int UseLightFlare( Obj_t *crit, Obj_t *obj );
int UseRunScript( Obj_t *obj );
int UseSetTimer( Obj_t *obj );
int UseCarRefuel( Obj_t *obj );
int UseFabularItem( Obj_t *crit ); // use item related to fabule "Hardcoded fabular objects!!!"
int UseItem( Obj_t *crit, Obj_t *obj ); // read book, reload, recharge, light flare, run item procedure, set timer in explosives
void UseUseExplosives( Obj_t *a1 );
int UseSlotItem( Obj_t *crit, Obj_t *obj );
int UseUnk16( Obj_t *crit, Obj_t *obj1, Obj_t *obj2 );
unsigned int UseUseHealSkill( Obj_t *crit, Obj_t *obj1, Obj_t *obj2 );
int UseObjOnObj( Obj_t *obj, Obj_t *a2, Obj_t *a3 );
void UseTileUpdate();
int UseApUpdate( Obj_t *obj,  Obj_t *, Obj_t * );
int UseObject( Obj_t *crit, Obj_t *obj, Obj_t * );
int UseLadderDn( Obj_t *crit, Obj_t *obj );
int UseLadderUp( Obj_t *crit, Obj_t *obj );
int UseStairs( Obj_t *crit, Obj_t *obj );
int UseDoorOpen( Obj_t *a1 );
int UseDoorClose( Obj_t *a1 );
int UseDoorLock( Obj_t *obj );
int UseDoor( Obj_t *Crit, Obj_t *obj, int a3 );
int UseContainer( Obj_t *crit, Obj_t *obj );
int UseUseSkill( Obj_t *crit, Obj_t *obj, unsigned int SkillIdx );
int UseObjIsDoor( Obj_t *obj );
int UseObjIsOpenable( Obj_t *obj );
int UseObjLocked( Obj_t *obj );
int UseObjLock( Obj_t *obj );
int UseObjUnlock( Obj_t *obj );
int UseObjIsOpenable1( Obj_t *obj ); // powtorzenie funkcji !
int UseObjInAnimation( Obj_t *obj );
int UseStartAnimation( Obj_t *obj );
int UseObjOpen( Obj_t *obj );
int UseObjClose( Obj_t *obj );
int UseObjIsJammed( Obj_t *obj );
int UseObjJam( Obj_t *obj );
int UseObjUnjam( Obj_t *obj );
int UseUnjamAll();
int UseUnk46( Obj_t *obj, int GridIdx, int lvl, int a4 );
int UseUnk47( Obj_t *obj, int a2, int a3 );
int UseAddItem( int Pid, int Quantity );
int UseEditRenderObj( int Win, int Type, int a3, int Pid, int (*Cb)(Proto_t *) );
int UseEditObjSelector( int Pid, int a2, void (*a3)(int, int ), int (*a4)(Proto_t *) );
int UseEditSelectObj( int Pid, int NotUsed, void (*a3)(int, int ) );
int UseEditObjSel( int *pPid, int PidType, int a3, int Flags );
int UseEditObjView( int WinId, Proto_t **a2, char *surf, int pitch, int pid );



