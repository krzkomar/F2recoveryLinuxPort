#pragma once

void InfoLookMsg( Obj_t *crit, Obj_t *obj );
void InfoLook( Obj_t *obj, Obj_t *, void (*OutCb)(char *) );
int InfoExamineMsg( Obj_t *a1, Obj_t *a2);
int InfoExamine( Obj_t *critter, Obj_t *obj, void (*OutCb)(char *) );

