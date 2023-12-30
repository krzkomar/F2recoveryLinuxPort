#pragma once

typedef struct 
{
  int i01;
  int i02;
} Region01_t;

typedef struct 
{
    char 	Name[ 32 ];
    Region01_t 	*p01;
    int 	i10;
    int 	i11;
    int 	i12;
    int 	i13;
    int 	i14;
    int 	i15;
    int 	i16;
    int 	p01Cnt;
    Intp_t 	*Script;
    int 	i19;
    int 	i20;
    int 	i21;
    int 	i22;
    int 	i23;
    int 	i24;
    int 	i25; //time
    int 	i26;
    int 	i27;
    int 	i28;
    int 	i29;
    int 	Flags;
    int 	(*i31)(void *, int, int i33 );
    int 	(*i32)(void *, int, int i34 );
    int 	i33;
    int 	i34;
    void 	*UserData;
} Region_t;

void RegionUnk01( Region_t *reg );
int RegionUnk02( Region_t *reg, int a2, int a3 );
Region_t *RegionInit( int Cnt );
void RegionAddPoint( Region_t *reg, int a2, int a3 );
void RegionDelete( Region_t *reg );
void RegionAddName( Region_t *reg, char *Name );
const char * RegionGetName( Region_t *reg );
void *RegionGetUserData( Region_t *reg );
void RegionSetUserData( Region_t *reg, void *UserData );
void RegionSetFlags( Region_t *reg, int flags );


