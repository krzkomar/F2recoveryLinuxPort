#pragma once

#define NEVS_ALLOCATED	40

typedef struct 
{
  int Used;
  char Name[ 32 ];
 Intp_t *Prog;
  int Proc;
  int Flag;	// 0 - event, 1- handler
  int i13;
  int i14;
    void (*Event)( char *Name );
  int i16;
} Nevs_t;

//Nevs_t *gNevs;
//int gNevsAnyHits;

Nevs_t *NevsAlloc();
void NevsClean( Nevs_t *nep );
void NevsClose();
void NevsDelete( Intp_t *itp );
void NevsInitOnce();
Nevs_t *NevsFind( char *name );
int NevsAddProg( char *name, Intp_t *Prog, int Proc, int Flag );
int NevsAddCEvent( char *name, void *Event, int Flag );
int NevsClearEvents( char *name );
int NevsUnk05( char *name, int *a2 );
int NevsSignal( char *name );
void NevsUpdate();


