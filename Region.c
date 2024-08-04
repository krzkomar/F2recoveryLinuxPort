#include "FrameWork.h"

void RegionUnk01( Region_t *reg )
{
    int i01; // ecx
    int v2; // edx
    int v3; // ebx
    int i02; // edi
    int v5; // esi
    Region01_t *p01; // eax
    int v9; // [esp+4h] [ebp-20h]
    int v10; // [esp+8h] [ebp-1Ch]

    i01 = 0x7FFFFFFF;
    v2 = 0x80000001;    
    i02 = 0x7FFFFFFF;
    v5 = 0x80000001;
    v9 = 0;
    v10 = 0;
    p01 = reg->p01;
    for( v3 = 0; v3 < reg->i16; v3++, p01++ ){
        if( i01 >= p01->i01 ) i01 = p01->i01;
        if( i02 >= p01->i02 ) i02 = p01->i02;
        if( v2 <= p01->i01 ) v2 = p01->i01;
        if( v5 <= p01->i02 ) v5 = p01->i02;
        v9 += p01->i01;
        v10 += p01->i02;
    }
    reg->i11 = i02;
    reg->i12 = v2;
    reg->i13 = v5;
    reg->i10 = i01;
    if( v3 ){
        reg->i14 = v9 / v3;
        reg->i15 = v10 / v3;
    }
}

int RegionUnk02( Region_t *reg, int a2, int a3 )
{
    Region01_t *p01; // edi
    int i; // ebp
    Region01_t *v7; // ebx
    Region01_t *v8; // ecx
    int v10; // edx
    int v14; // [esp+Ch] [ebp-20h]
    int v15; // [esp+10h] [ebp-1Ch]
    int v16; // [esp+14h] [ebp-18h]

    if( !reg || a2 < reg->i10 || a2 > reg->i12 || a3 < reg->i11 || a3 > reg->i13 ) return 0;
    p01 = reg->p01;
    if( a2 >= p01->i01 ){
        v15 = (a3 >= p01->i02) + 1;
    } else if( a3 >= p01->i02 ){
        v15 = 3;
    } else {
        v15 = 0;
    }
    v16 = 0;
    v7 = reg->p01 + 1;
    for( i = 0; i < reg->i16; i++ ){
        v8 = v7;
        if( v7->i01 <= a2 ){
            v14 = (a3 >= v7->i02) + 1;
        } else if( a3 >= v7->i02 ){
            v14 = 3;
        } else {
            v14 = 0;
        }
        v10 = v14 - v15;
        switch( v10 ){
            case -3: v10 = 1; break;
            case -2: case 2:
                if( (double)a2 < (double)v7->i01 - (double)(p01->i01 - v7->i01) / (double)(p01->i02 - v7->i02) * (double)(v7->i02 - a3) ) v10 = -v10;
                break;
            case 3: v10 = -1; break;
        }
        v7++;
        v15 = v14;        
        v16 += v10;
        p01 = v8;
    }
    return v16 == 4 || v16 == -4;
}

Region_t *RegionInit( int Cnt )
{
    Region_t *p;

    p = dbg_malloc( sizeof(Region_t) );
    memset( p, 0, sizeof( Region_t ) );
    if( Cnt ){
        p->p01 = (Region01_t *)dbg_malloc( sizeof( Region01_t ) * ( Cnt + 1 ) );
        p->p01Cnt = Cnt + 1;
    } else {
        p->p01Cnt = 0;
        p->p01 = NULL;
    }
    p->Flags = 0;
    p->i11 = 0x80000001;
    p->i13 = 0x7FFFFFFF;
    p->i22 = 0;
    p->i24 = 0;
    p->i26 = 0;
    p->i27 = 0;
    p->Name[ 0 ] = '\0';
    p->i28 = 0;
    p->i29 = 0;
    p->i25 = 0;
    p->i31 = 0;
    p->i32 = 0;
    p->i33 = 0;
    p->i34 = 0;
    p->i16 = 0;
    p->i10 = p->i11;
    p->i12 = p->i13;
    p->i21 = p->i22;
    p->i20 = p->i22;
    p->i19 = p->i22;
    p->i23 = p->i24;
    return p;
}

void RegionAddPoint( Region_t *reg, int a2, int a3 )
{
    if( !reg ){
	eprintf( "regionAddPoint(): null region ptr" );
	return;
    }
    if( reg->p01 ){
        if( reg->p01Cnt - 1 == reg->i16 ){
            reg->p01Cnt++;
            reg->p01 = dbg_realloc( reg->p01, sizeof( Region01_t ) * reg->p01Cnt );
        }
    } else {
        reg->p01Cnt = 2;
        reg->i16 = 0;
        reg->p01 = dbg_malloc( 2 * sizeof( Region01_t ) );
    }
    reg->p01[ reg->i16 ].i01 = a2;
    reg->p01[ reg->i16 ].i02 = a3;
    reg->i16++;
    reg->p01[ reg->i16 ] = reg->p01[ 0 ];
}

void RegionDelete( Region_t *reg )
{
    if( reg ){
        if( reg->p01 ) dbg_free( reg->p01 );
        dbg_free( reg );
    } else {
        eprintf( "regionDelete(): null region ptr" );
    }
}

void RegionAddName( Region_t *reg, char *Name )
{
    if( reg ){
        if( Name )
            strncpy( reg->Name, Name, 31 );
        else
            reg->Name[ 0 ] = '\0';
    } else {
        eprintf( "regionAddName(): null region ptr" );
    }
}

const char * RegionGetName( Region_t *reg )
{
    if( !reg ){
        eprintf( "regionGetName(): null region ptr" );
        return "<null>";
    }
    return reg->Name;
}

void *RegionGetUserData( Region_t *reg )
{
    if( reg ) return reg->UserData;
    eprintf( "regionGetUserData(): null region ptr" );
    return NULL;
}

void RegionSetUserData( Region_t *reg, void *UserData )
{
    if( reg )
        reg->UserData = UserData;
    else
        eprintf( "regionSetUserData(): null region ptr" );
}

void RegionSetFlags( Region_t *reg, int flags )
{
    reg->Flags |= flags;
}

