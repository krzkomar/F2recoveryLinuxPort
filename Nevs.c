#include "FrameWork.h"

Nevs_t *gNevs;
int gNevsAnyHits;

Nevs_t *NevsAlloc()
{
    Nevs_t *nep;
    int i;

    if( !gNevs ){
        eprintf( "nevs_alloc(): nevs_initonce() not called!" );
        exit( 99 );
    }
    nep = gNevs;
    for( i = 0; i < NEVS_ALLOCATED; i++ ){
        if( !nep->Used ){
            memset( nep, 0, sizeof( Nevs_t ) );
            return nep;
        }
        nep++;
    }
    return 0;
}

void NevsClean( Nevs_t *nep )
{
    nep->Used = 0;
    memset( nep, 0, sizeof( Nevs_t ) );
}

void NevsClose()
{
    if( !gNevs ) return;
    dbg_free( gNevs );
    gNevs = NULL;
}

void NevsDelete( Intp_t *itp )
{
    int i;

    if ( !gNevs ) return;
    for( i = 0; i < NEVS_ALLOCATED; i++ ){
        if( !gNevs[ i ].Used ) continue;
        if( itp != gNevs[ i ].Prog ) continue;
        gNevs[ i ].Used = 0;
        memset( gNevs, 0, sizeof( Nevs_t ) );
    }
}

void NevsInitOnce()
{
    IntpTaskAdd( (void *)NevsDelete );
    if( gNevs ) return;    
    gNevs = dbg_calloc( sizeof( Nevs_t ), NEVS_ALLOCATED );
    if( !gNevs ){
        eprintf( "nevs_initonce(): out of memory" );
        exit( 99 );
    }    
}

Nevs_t *NevsFind( char *name )
{
    int i;

    if( !gNevs ){
        eprintf( "nevs_find(): nevs_initonce() not called!" );
        exit( 99 );
    }
    for( i = 0; i < NEVS_ALLOCATED; i++ ){
        if( gNevs[ i ].Used && !strcasecmp( gNevs[ i ].Name, name ) ) return &gNevs[ i ];
    }
    return NULL;
}

int NevsAddProg( char *name, Intp_t *Prog, int Proc, int Type )
{
    Nevs_t *nep;

    if( !(nep = NevsFind( name ) ) ) nep = NevsAlloc();
    if( !nep ) return 1;
    nep->Used = 1;
    strcpy( nep->Name, name );
    nep->Event = 0;
    nep->Prog = Prog;
    nep->Proc = Proc;
    nep->Flag = Type;
    return 0;
}

int NevsAddCEvent( char *name, void *Event, int Flag )
{
    Nevs_t *p;

    eprintf( "nevs_addCevent( '%s', %p);\n", name, Event );
    p = NevsFind( name );
    if( !p ) p = NevsAlloc();
    if( !p ) return 1;
    p->Used = 1;
    strcpy( p->Name, name );
    p->Prog = NULL;
    p->Proc = 0;
    p->Flag = Flag;
    p->Event = Event;
    return 0;
}

int NevsClearEvents( char *name )
{
    Nevs_t *nep;

    eprintf( "nevs_clearevent( '%s');\n", name );
    if( !(nep = NevsFind( name ) ) ) return 1;
    nep->Used = 0;
    memset( nep, 0, sizeof( Nevs_t ) );
    return 0;
}

int NevsUnk05( char *name, int *a2 )
{
    Nevs_t *nep; // eax MAPDST
//    int *v3; // edx
//    int v4; // ecx

    nep = NevsFind( name );
    if( !nep || !nep->Used || !nep->Prog || !nep->Proc ) return 1;
//    SciUnk08( nep->Prog, nep->Proc, v3, a2, v4 );
    if( !nep->Flag ){
        nep->Used = 0;
        memset( nep, 0, sizeof( Nevs_t ) );
    }
    return 0;
}

int NevsSignal( char *name )
{
    Nevs_t *nep;

    nep = NevsFind( name );
    eprintf( "nevs_signal( '%s');\n", name );
    if( !nep ) return 1;
    eprintf("nep: %p,  used = %u, prog = %p, proc = %d\n", nep, nep->Used, nep->Prog, nep->Proc);
    if( !nep->Used ) return 1;
    if( (!nep->Prog || !nep->Proc) && !nep->Event ) return 1;
    if( nep->i14 ) return 1;
    gNevsAnyHits++;
    nep->i13++;
    return 0;
}

void NevsUpdate()
{
    int i;
    Nevs_t *p;

    if( gNevsAnyHits == 0 ) return;
    eprintf( "nevs_update(): we have anyhits = %u\n", gNevsAnyHits );
    gNevsAnyHits = 0;
    p = gNevs;
    for( i = 0; i < NEVS_ALLOCATED; i++, p++ ){
        if( p && p->Used && (p->Prog && (p->Proc || p->Event)) && !p->i14 ){
            if( p->i13 > 0 ){
                p->i14 = 1;
                p->i13--;
                gNevsAnyHits += p->i13;
                if( p->Event )
                    p->Event( p->Name );
//                else
//                    SciUnk11( p->Prog, p->Proc );
                p->i14 = 0;
                if( !p->Flag ){
                    p->Used = 0;
                    memset( p, 0, sizeof( Nevs_t ) );
                }
            }
        }        
    }    
}

