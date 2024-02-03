#pragma once 

/*
int gActionUnk06;
int gActionUnk05;
int gActionUnk01;
int gActionUnk02;
int gActionUnk04[ 7 ];
int gActionUnk03[ 7 ];
*/

void ActionCopyPlayer();
int ActionUnk40( Obj_t *obj, int *a2, int a3, int a4, int a5 );
int ActionAnimGore( Obj_t *obj, int a2, int a3 );
int ActionUnk38( Obj_t *a1, Obj_t *a2, Obj_t *a3, int a4, int a5, int a6 );
int ActionUnk37( Obj_t *obj, int a2, int a3, int a4 );
int ActionUnk36( int a1, Obj_t *a2 );
int ActionUnk35( Obj_t *obj1, int dmg, int obj3, Obj_t *a4, int a5, int a6, int a7, int a8, Obj_t *obj4, int a10 );
int ActionUnk34( Obj_t *obj, int a2 );
void ActionUnk33( Combat_t *pObj );
void ActionUnk32( Combat_t *cmbt );
int ActionUnk31( Combat_t *cmbt, int a2, int a3 );
int ActionUnk30( Combat_t *cmbt );
int ActionUnk29( Combat_t *cmbt, int a2 );
void ActionUnk28( Obj_t *obj, int ImgId );
int ActionUnk27( Combat_t *cmbt, int a2 );
int ActionUnk26( Obj_t *a1 );
int ActionReachable( Obj_t *obj1, Obj_t *obj2 );
int ActionLadderClimb( Obj_t *obj1, Obj_t *obj2 );
//int ActionUnk23( Obj_t *obj1, Obj_t *obj2, int a3 );
int ActionUseSceneryObject( Obj_t *obj1, Obj_t *obj2, Obj_t *a3 );
int ActionUseOnScenery( Obj_t *obj1, Obj_t *obj2 );
int ActionPlayerPickup( Obj_t *obj );
int ActionPickupItem( Obj_t *Critter, Obj_t *Obj );
int ActionUseOnCritter( Obj_t *a1, Obj_t *a2 );
int ActionUseSneak( int SkillId );
int ActionUnk20( Obj_t *obj );
int ActionSkillUse( Obj_t *obj1, Obj_t *obj2, int SkillNo );
Obj_t *ActionUnk18( int a1 );
int ActionUnk17();
int ActionUnk16( Obj_t *a1, Obj_t *a2 );
int ActionTurnAt( Obj_t *obj1, Obj_t *obj2 );
int ActionUnk14( Obj_t *obj, int a2 );
int ActionUnk13();
int ActionExplode( int GridPos, int MapLvl, int DmgMin, int DmgMax, Obj_t *Target, int SelfHurt );
void ActionUnk11( Combat_t *cmbt, Obj_t *obj );
void ActionUnk10();
int ActionGetExplosionDmg( int min, int max, Obj_t *obj, int *pVal );
int ActionTalk( Obj_t *Crit1, Obj_t *Crit2 );
int ActionSndACb( Obj_t *Critter, Obj_t *Target );
void ActionTalkToObj( Obj_t *Src, Obj_t *Trg );
void ActionUnk05( int a1, int edx0, int a3, int a4, int a5, int a6, int a7 );
int ActionUnk100( Combat_t *cmbt );
int ActionUnk04( int Min, int Max, Obj_t *obj, int *a4, int a5 );
int ActionCritterTalkTo( Obj_t *a1, Obj_t *a2 );
int ActionUnk02( Obj_t *a1, Obj_t *a2 );
int ActionTargetAccessible( Obj_t *Crit, Obj_t *Target );


