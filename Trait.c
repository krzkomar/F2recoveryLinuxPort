#include "FrameWork.h"

Trait_t gTraits[ 16 ] = {
    { 0, 0, 55 },
    { 0, 0, 56 },
    { 0, 0, 57 },
    { 0, 0, 58 },
    { 0, 0, 59 },
    { 0, 0, 60 },
    { 0, 0, 61 },
    { 0, 0, 62 },
    { 0, 0, 63 },
    { 0, 0, 64 },
    { 0, 0, 65 },
    { 0, 0, 66 },
    { 0, 0, 67 },
    { 0, 0, 68 },
    { 0, 0, 69 },
    { 0, 0, 70 },
};

Msg_t gTraitMsg;
int gTraitSpec[ 2 ];

/***************************************/

int TraitInit()
{
    MsgLine_t MsgLine;
    char path[ 260 ];
    int i;

    if( MessageInit( &gTraitMsg ) != 1 ) return -1;
    sprintf( path, "%s%s", gGamePath, "trait.msg" );
    if( MessageLoad( &gTraitMsg, path ) != 1 ) return -1;
    for( i = 0; i < 16; i++ ){
        MsgLine.Id = i + 100;
        if( MessageGetMsg( &gTraitMsg, &MsgLine ) == 1 ) gTraits[ i ].Name = MsgLine.Text;
        MsgLine.Id = i + 200;
        if( MessageGetMsg( &gTraitMsg, &MsgLine ) == 1 ) gTraits[ i ].Dsc = MsgLine.Text;
    }
    memset( gTraitSpec, 0xff, sizeof( gTraitSpec ) );
    return 0;
}

void TraitSpecReset()
{
    memset( gTraitSpec, 0xff, sizeof( gTraitSpec ) );
}

void TraitClose()
{
    MessageClose( &gTraitMsg );
}

int TraitLoad( xFile_t *fh )
{
    return ( dbreadBeiBlk( fh, gTraitSpec, 2 ) != -1 ) - 1;
}

int TraitSave( xFile_t *fh )
{
    return ( dbputBeiBlk( fh, gTraitSpec, 2 ) != -1 ) - 1;
}

void TraitSetSpec( int s1, int s2 )
{
    gTraitSpec[ 1 ] = s2;
    gTraitSpec[ 0 ] = s1;
}

void TraitGetSpec( int *Spec1, int *Spec2 )
{
    *Spec1 = gTraitSpec[ 0 ];
    *Spec2 = gTraitSpec[ 1 ];
}

char *TraitGetName( unsigned int Id )
{
    if( Id < 16 ) return gTraits[ Id ].Name;
    return 0;
}

char *TraitGetDsc( unsigned int Id )
{
    if( Id < 16 ) return gTraits[ Id ].Dsc;
    return 0;
}

int TraitGetId( unsigned int Id )
{
    if( Id < 16 ) return gTraits[ Id ].Id;
    return 0;
}

int TraitSpecActive(  int id )
{
    int i;

    i = 0;
    if( id == gTraitSpec[0] ) return 1;
    while( ++i < 2 ){ if( id == gTraitSpec[i] ) return 1; }
    return 0;
}

int TraitSpecBonus( int Id )
{
    int r, i, k;

    r = 0;
    switch( Id ){
        case 0:
            i = 0;
            if( gTraitSpec[0] == 15 ){ k = 1; goto LABEL_8; }
            while( ++i < 2 ){ if( gTraitSpec[i] == 15 ){ k = 1; goto LABEL_8; } }
            k = 0;
LABEL_8:
            if( k ) r = 1;
            i = 0;
            if( gTraitSpec[0] == 1 ){
LABEL_11:
                k = 1;
            } else {
                while( ++i < 2 ) {
                    if( gTraitSpec[i] == 1 ) goto LABEL_11;
                }
                k = 0;
            }
            if( !k ) return r;
            return r + 2;
        case 1:
            i = 0;
            if( gTraitSpec[0] == 15 ){ k = 1; goto LABEL_24; }
            while( ++i < 2 ){ if( gTraitSpec[i] == 15 ){ k = 1; goto LABEL_24; } }
            k = 0;
LABEL_24:
            if( !k ) return r;
            return 1;
        case 2:
            i = 0;
            if( gTraitSpec[0] == 15 ){ k = 1; goto LABEL_32; }
            while ( ++i < 2 ){ if( gTraitSpec[i] == 15 ){ k = 1; goto LABEL_32; } }
            k = 0;
LABEL_32:
            if( !k ) return r;
            return 1;
        case 3:
            i = 0;
            if( gTraitSpec[0] == 15 ){ k = 1; goto LABEL_40; }
            while( ++i < 2 ){ if( gTraitSpec[i] == 15 ){ k = 1; goto LABEL_40; } }
            k = 0;
LABEL_40:
            if( !k ) return r;
            return 1;
        case 4:
            i = 0;
            if( gTraitSpec[0] == 15 ){ k = 1; goto LABEL_48; }
            while( ++i < 2 ){ if( gTraitSpec[i] == 15 ){ k = 1; goto LABEL_48; } }
            k = 0;
LABEL_48:
            if( !k ) return r;
            return 1;
        case 5:
            i = 0;
            if( gTraitSpec[0] == 15 ){ k = 1; goto LABEL_56; }
            while( ++i < 2 ){ if( gTraitSpec[i] == 15 ){ k = 1; goto LABEL_56; } }
            k = 0;
LABEL_56:
            if( k ) r = 1;
            i = 0;
            if( gTraitSpec[0] == 2 ){
LABEL_59:
                k = 1;
            } else {
                while( ++i < 2 ){
                    if( gTraitSpec[i] == 2 ) goto LABEL_59;
                }
                k = 0;
            }
            if( !k ) return r;
            return r + 1;
        case 6:
            i = 0;
            if( gTraitSpec[0] == 15 ){ k = 1; goto LABEL_72; }
            while( ++i < 2 ){ if( gTraitSpec[i] == 15 ){ k = 1; goto LABEL_72; } }
            k = 0;
LABEL_72:
            if( !k ) return r;
            return 1;
        case 8:
            i = 0;
            if( gTraitSpec[0] == 1 ){k = 1; goto LABEL_80;}
            while ( ++i < 2 ){ if( gTraitSpec[i] == 1 ){ k = 1; goto LABEL_80; } }
            k = 0;
LABEL_80:
            if( !k ) return r;
            return -2;
        case 9:
            i = 0;
            if( gTraitSpec[0] == 5 ) goto LABEL_83;
            while( ++i < 2 ){
                if( gTraitSpec[i] == 5 ){
LABEL_83:
                    k = 1;
LABEL_88:
                    if( !k ) return r;
                    Id = 9;
                    return -FeatGetBase(gObjDude, Id);
                }
            }
            k = 0;
            goto LABEL_88;
        case 11:
            i = 0;
            if( gTraitSpec[0] == 6 ){ k = 1; goto LABEL_96; }
            while( ++i < 2 ){ if( gTraitSpec[i] == 6 ){ k = 1; goto LABEL_96; } }
            k = 0;
LABEL_96:
            if( !k ) return r;
            return 4;
        case 12:
            i = 0;
            if( gTraitSpec[0] == 2 ){ k = 1; goto LABEL_104; }
            while( ++i < 2 ){ if( gTraitSpec[i] == 2 ){ k = 1; goto LABEL_104; } }
            k = 0;
LABEL_104:
            if( !k ) return r;
            return -10 * FeatGetBase(gObjDude, 0);
        case 13:
            i = 0;
            if( gTraitSpec[0] == 5 ){ k = 1; goto LABEL_112; }
            while( ++i < 2 ){ if( gTraitSpec[i] == 5 ){ k = 1; goto LABEL_112; } }
            k = 0;
LABEL_112:
            if( !k ) return r;
            return 5;
        case 14:
            i = 0;
            if( !gTraitSpec[0] ){ k = 1; goto LABEL_120; }
            while( ++i < 2 ){ if( !gTraitSpec[i] ){ k = 1; goto LABEL_120; } }
            k = 0;
LABEL_120:
            if( !k ) return r;
            return 2;
        case 15:
            i = 0;
            if( gTraitSpec[0] == 4 ){ k = 1; goto LABEL_128; }
            while( ++i < 2 ){ if( gTraitSpec[i] == 4 ){ k = 1; goto LABEL_128; } }
            k = 0;
LABEL_128:
            if( !k ) return r;
            return 10;
        case 16:
            i = 0;
            if( gTraitSpec[0] == 6 ){ k = 1; goto LABEL_136; }
            while( ++i < 2 ){ if( gTraitSpec[i] == 6 ){ k = 1; goto LABEL_136; } }
            k = 0;
LABEL_136:
            if( !k ) return r;
            return -30;
        case 31:
            i = 0;
            if( !gTraitSpec[0] ) {k = 1; goto LABEL_144;}
            while( ++i < 2 ){ if( !gTraitSpec[i] ) k = 1; goto LABEL_144; }
            k = 0;
LABEL_144:
            if( k ) return -FeatGetBase(gObjDude, Id);
            return r;
        case 32: i = 0; if( !gTraitSpec[0] ) goto LABEL_147; break; 
        default:
            return r;
    }
    while( ++i < 2 ){
        if( !gTraitSpec[i] ){
LABEL_147:
            k = 1; goto LABEL_152;
        }
    }
    k = 0;
LABEL_152:
    if( k ) return -FeatGetBase( gObjDude, Id );
    return r;
}

int TraitSpecBoost( unsigned int Id )
{
    int r,i,k;

    i = r = 0;
    if( gTraitSpec[0] == 15 ){
LABEL_2:
        k = 1;
    } else {
        while( ++i < 2 ){ if( gTraitSpec[i] == 15 ) goto LABEL_2; }
        k = 0;
    }
    if( k ) r = -10;
    i = 0;
    if( gTraitSpec[0] == 10 ){
LABEL_10:
        k = 1;
    } else {
        while( ++i < 2 ){ if( gTraitSpec[i] == 10 ) goto LABEL_10; }
        k = 0;
    }
    if( k ){
        if( Id < 6 ){
            r -= 10;
        } else if( (Id <= 7) || ((Id >= 14) && (Id <= 15)) ) {
            r += 15;
        }
    }
    return r;
}

