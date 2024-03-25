#include "FrameWork.h"
// Trigger(Obj_t*, void *) / Free( void *) / Load( fh, int *) / Save( fh, int *) / RunAuto / CbAuto(Obj_t *, int *)
EvFunc_t gEvMethods[ 14 ] = {
    { (void *)Item105, 			(void *)Free, (void *)ItemLoadEffects,   (void *)ItemSaveEffects, 	1, (void *)Item103 }, 	// 0 load/save
    { (void *)CritterUnk43, 		NULL, NULL, 		 NULL, 			1, (void *)CritterUnk44 }, 			// 1
    { (void *)ItemWithdrawal,  		(void *)Free, (void *)ItemWithdrawalLoad,(void *)ItemWithdrawalSave, 	1, (void *)Item109 }, 	// 2 withdrawal money
    { (void *)ScptRunTimedEvent,	(void *)Free, (void *)ScptLoadUnk17,   	 (void *)ScptUnk128, 		1, NULL }, 		// 3 run timed_event script procedure
    { (void *)ScptMidnight, 		NULL, NULL, 		 NULL, 			1, NULL }, 					// 4 run at midnight
    { (void *)CritterPoison, 		NULL, NULL, 		 NULL, 			0, NULL }, 					// 5 Poison
    { (void *)CritterRadUnk02,  	(void *)Free, (void *)CritterLoadUnk01,  (void *)CritterSaveUnk01, 	0, NULL }, 		// 6 Rad
    { (void *)EvQeUnk10, 		NULL, NULL, 		 NULL, 			1, (void *)EvQeUnk10 }, 			// 7 ?
    { (void *)EvQeExplosionFail, 	NULL, NULL, 		 NULL, 			1, (void *)EvQeExplosionSuccess }, 		// 8 Explosion
    { (void *)ItemDeviceUse, 		NULL, NULL, 		 NULL, 			1, (void *)ItemDeviceOffCb }, 			// 9 Device On/Off
    { (void *)CritterUnk40, 		NULL, NULL, 		 NULL, 			1, (void *)CritterUnk41 }, 			// 10 ?
    { (void *)EvQeExplosionOnSelf, 	NULL, NULL, 		 NULL, 			1, (void *)EvQeExplosionSuccess },  		// 11 Inept handling explosives, self detonation
    { (void *)ScptMapUpdateEv, 		NULL, NULL, 		 NULL, 			1, NULL }, 					// 12
    { (void *)GSoundMapAmbient, 	(void *)Free, NULL, 		 NULL, 			1, NULL }				// 13 Sound ambient effect
};

Ev_t *gEvQeCur = NULL;
Ev_t *gEvQueue;

/*******************************************/
void EvQeInit()
{
    gEvQueue = NULL;
}

int EvQeClose()
{
    EvQeFlush();
    return 0;
}

int EvQeLoad( xFile_t *fh )
{
    Obj_t *p, *obj;
    Ev_t *Ev,*Next,*v15,*v16,**pEv,*root;
    int i,Method,ObjId,count,err;

    err = 0;
    if( dbgetBei( fh, &count ) == -1 ) return -1;
    root = gEvQueue;
    pEv = &gEvQueue;
    gEvQueue = NULL;
    for( i = 0; i < count; i++ ){            
        if( !( Ev = Malloc( sizeof( Ev_t ) ) ) ){ err = -1; break;}
        if( dbgetBei( fh, &Ev->TimeEv ) == -1 ){ Free( Ev ); err = -1; break; }
        if( dbgetBei( fh, &Ev->Method ) == -1 ){ Free( Ev ); err = -1; break; }
        if( dbgetBei( fh, &ObjId ) == -1 ){ Free( Ev ); err = -1; break; }
        if( ObjId == -2 ){ // not used
            obj = NULL;
        } else {
            for( p = ObjGetObjectFirst(); p; p = ObjGetObjectNext() ){
                if( ( obj = InvSearchObjByPid( p, ObjId ) ) ) break;
            }
        }
        Ev->Obj = obj;
        if( gEvMethods[ Ev->Method ].Load ){
            if( gEvMethods[ Ev->Method ].Load( fh, &Ev->Ptr ) == -1 ){ Free( Ev ); err = -1; break; }
        } else {
            Ev->Ptr = NULL;
        }
        Ev->Next = NULL;
        *pEv = Ev;
        pEv = &Ev->Next;
    }
    if( err == -1 ){
        while ( gEvQueue ){
            Method = gEvQueue->Method;
            Next = gEvQueue->Next;
            if( gEvMethods[ Method ].Free ) gEvMethods[ Method ].Free( gEvQueue->Ptr );
            Free( gEvQueue );
            gEvQueue = Next;
        }
    }
    pEv = &gEvQueue;
    while( root ){
        while( 1 ){
            v15 = *pEv;
            if( !*pEv ) break;
            if( v15->TimeEv > (unsigned int)root->TimeEv ) break;
            pEv = &v15->Next;
        }
        v16 = root->Next;
        root->Next = *pEv;
        *pEv = root;
        root = v16;
    }
    return err;
}

int EvQeSave( xFile_t *fh )
{
    Ev_t *p;
    int i, tmp;
    
    for( i = 0, p = gEvQueue; p; i++,  p = p->Next );
    
    if( dbputBei( fh, i ) != -1 ) return -1;
    for( p = gEvQueue; p; p = p->Next ){
        tmp = ( p->Ptr ) ? *p->Ptr : -2;
        if( dbputBei( fh, p->TimeEv ) == -1 ) return -1;
        if( dbputBei( fh, p->Method ) == -1 ) return -1;
        if( dbputBei( fh, tmp ) == -1 )  return -1;
        if( gEvMethods[ p->Method ].Save ){
            if( gEvMethods[ p->Method ].Save( fh, p->Ptr ) == -1 ) return -1;
        }        
    }
    return 0;
}

int EvQeSchedule( int GameTime, Obj_t *object, void *Ptr, int Method )
{    
    int InGameTime;
    Ev_t **p_Next, *q, *ev;

    ev = Malloc( sizeof( Ev_t ) );
    if( !ev ) return -1;
    InGameTime = ScptGetGameDekaSeconds();
    ev->Method = Method;
    ev->Obj = object;
    ev->TimeEv = InGameTime + GameTime;
    if( object ) object->Flags |= 0x20000000;
    ev->Ptr = Ptr;
    p_Next = &gEvQueue;
    if( gEvQueue ){
        do{
            q = *p_Next;
            if( ev->TimeEv < (*p_Next)->TimeEv ) break;
            p_Next = &q->Next;
        }while( q->Next );
    }
    ev->Next = *p_Next;
    *p_Next = ev;
    return 0;
}

void EvQeDelA( Obj_t *obj )
{
    Ev_t *qe;
    Ev_t **p_Next;
    Ev_t *p;

    qe = gEvQueue;
    p_Next = &gEvQueue;
    while( qe ){
        if( obj == qe->Obj ){
            p = qe;
            *p_Next = qe->Next;
            if( gEvMethods[ qe->Method ].Free ) gEvMethods[ qe->Method ].Free( qe->Ptr );
            Free( p );
        } else {
            p_Next = &qe->Next;
            qe = qe->Next;
        }
    }
}

int EvQeDelB( Obj_t *A, int B )
{
    Ev_t *qe;
    Ev_t **p_Next;
    Ev_t *v6;

    qe = gEvQueue;
    p_Next = &gEvQueue;
    while( qe ){
        if( A == qe->Obj && B == qe->Method ){
            v6 = qe;
            *p_Next = qe->Next;
            if( gEvMethods[qe->Method].Free ) gEvMethods[ qe->Method ].Free( qe->Ptr );
            Free( v6 );
        } else {
            p_Next = &qe->Next;
            qe = qe->Next;
        }
    }
    return 0;
}

int EvQeEnqueued( Obj_t *obj, int MethodId )
{
    Ev_t *qe;
    
    for( qe = gEvQueue; qe; qe = qe->Next ){
	if( obj == qe->Obj && MethodId == qe->Method ) return 1;
    }
    return 0;
}

int EvQeTrig()
{
    unsigned int time;
    int err = 0;
    Ev_t *eq;

    time = ScptGetGameDekaSeconds();
    while( gEvQueue ){
        eq = gEvQueue;
        if( time < gEvQueue->TimeEv || err ) break; // not time passed or error occured
        gEvQueue = gEvQueue->Next; // event fired, take next
        err = gEvMethods[ eq->Method ].TriggerCb( eq->Obj, eq->Ptr ); // Call method
        if( gEvMethods[ eq->Method ].Free ) gEvMethods[ eq->Method ].Free( eq->Ptr );
        Free( eq );
    }
    return err;
}

void EvQeFlush()
{
    Ev_t *eq, *tmp;

    eq = gEvQueue;
    while( eq ){
        tmp = eq->Next;
        if( gEvMethods[ eq->Method ].Free ) gEvMethods[ eq->Method ].Free( eq->Ptr );
        Free( eq );
        eq = tmp;
    }
    gEvQueue = NULL;
}

void EvQeRun( int MethodIdx, int (*Cb)(Obj_t *, int *) )
{
    Ev_t *p;
    Ev_t **p_Next;
    Ev_t *cur;

    p = gEvQueue;
    p_Next = &gEvQueue;
    while( p ){
        if( MethodIdx == p->Method ){
            cur = p;
            p = p->Next;
            *p_Next = p;
            if( Cb && Cb( cur->Obj, cur->Ptr ) != 1 ){
                *p_Next = cur;
                p_Next = &cur->Next;
            } else {
                if( gEvMethods[ cur->Method ].Free ) gEvMethods[ cur->Method ].Free( cur->Ptr );
                Free( cur );
            }
        } else {
            p_Next = &p->Next;
            p = p->Next;
        }
    }
}

int EvQeGetTime()
{
    if( gEvQueue ) return gEvQueue->TimeEv;
    return 0;
}

int EvQeUnk10( Obj_t *a1 )
{
    UseUnk06( a1 );
    return 1;
}

int EvQeExplosionFail( Obj_t *Explosives )
{
    return EvQeExplode( Explosives, 1 );
}

int EvQeExplosionSuccess( Obj_t *Explosives )
{
    return EvQeExplode( Explosives, 0 );
}

int EvQeExplode( Obj_t *obj, int OnSelf )
{
    Obj_t *Owner;
    int Elevation, GridId, DmgMax, DmgMin;

    Owner = ObjGetOwner( obj );
    if( Owner ){
        GridId = Owner->GridId;
        Elevation = Owner->Elevation;
    } else {
        Elevation = obj->Elevation;
        GridId = obj->GridId;
    }
    if( obj->Pid == PID_DYNAMITE1 || obj->Pid == PID_DYNAMITE ){ // dynamite
        DmgMax = 50;
        DmgMin = 30;
    } else { // plastic explosives
        DmgMax = 80;
        DmgMin = 40;
    }
    if( gObjDude && PerkLvl( gObjDude, PERK_DEMOLITION_EXPERT ) ){
        DmgMax += 10;
        DmgMin += 10;
    }
    if( ActionExplode( GridId, Elevation, DmgMin, DmgMax, gObjDude, OnSelf ) == -2 )
        EvQeSchedule( 50, obj, 0, 8 );
    else
        UseUnk06( obj );
    return 1;
}

int EvQeExplosionOnSelf( Obj_t *Obj )
{
    MsgLine_t msg;

    msg.Id = 4000; // due to your inept handling, the explosive detonates prematurely
    if( MessageGetMsg( &gMessage, &msg ) == 1 ) IfcMsgOut( msg.Text );
    return EvQeExplode( Obj, 1 );
}

void EvQeRunAll()
{
    int i;
    
    for( i = 0; i < 14; i++ ){
        if ( gEvMethods[ i ].RunAuto ) EvQeRun( i, gEvMethods[ i ].CbAuto );
    }
}

int EvQeIsEmpty()
{
    return gEvQueue == NULL;
}

int *EvQeFindPtr( Obj_t *obj, int MethodIdx )
{
    Ev_t *qe;
    
    for( qe = gEvQueue; qe; qe = qe->Next ){
    	if( obj == qe->Obj && MethodIdx == qe->Method ){
	    gEvQeCur = qe;
	    return qe->Ptr;
    	}        
    }
    gEvQeCur = NULL;
    return NULL;
}

int *EvQeFindNextPtr( Obj_t *obj, int MethodIdx )
{
    Ev_t *p;

    p = gEvQeCur;
    if( gEvQeCur ){    
	for( p = gEvQeCur->Next; p; p = p->Next ){
    	    if( obj == p->Obj && MethodIdx == p->Method ){
		gEvQeCur = p;
		return p->Ptr;
    	    }        
	}
    }
    gEvQeCur = NULL;
    return NULL;
}

int EvQeUnk19( Obj_t *obj, int a2 )
{
    ScptSetLocVar(obj->ScrId, 0, a2);
    return 0;
}

int EvQeUnk20( int a1 )
{
    if( a1 <= -75 ) return 0;//EvQeGetReactionInfluence( );
    if( a1 <= -50 ) return 0;
    if( a1 <= -25 ) return 0;
    if( a1 <= -10 ) return 0;
    if( a1 > 10 ) return 2;
    return 1;
}

int EvQeGetReactionInfluence( int a, int b, int c )
{
    return 0;
}

int EvQeUnk22( int a1, int a2 )
{
    int v2;

    if( a2 > -75 ){
        if( a2 <= -50 ){ ScptSetLocVar( a1, 1, -3 ); return EvQeUnk20( a2 ); }
        if( a2 <= -25 ){ ScptSetLocVar( a1, 1, -2 ); return EvQeUnk20( a2 ); }
        if( a2 <= -10 ){ ScptSetLocVar( a1, 1, -1 ); return EvQeUnk20( a2 ); }
        if( a2 <= 10  ){ ScptSetLocVar( a1, 1, 0  ); return EvQeUnk20( a2 ); }
        if( a2 <= 25  ){ ScptSetLocVar( a1, 1, 1  ); return EvQeUnk20( a2 ); }
        if( a2 <= 50  ){ ScptSetLocVar( a1, 1, 2  ); return EvQeUnk20( a2 ); }
        if( a2 <= 75  ){ ScptSetLocVar( a1, 1, 3  ); return EvQeUnk20( a2 ); }
        v2 = 4;
    } else {
        v2 = -4;
    }
    ScptSetLocVar( a1, 1, v2 );
    return EvQeUnk20( a2 );
}

int EvQeUnk23( Obj_t *obj )
{
    int tmp;

    if( ScptGetLocVar( obj->ScrId, 0, &tmp ) != -1 ) return -1;
    return tmp;    
}
