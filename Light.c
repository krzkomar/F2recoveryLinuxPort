#include "FrameWork.h"

#define LIGHT_NIGHT_VISION_MODIFIER 	0x3333
#define LIGHT_MIN_LVL			0x4000
#define LIGHT_MAX_LVL			0x10000
#define LIGHT_DEFAULT			655
#define LIGHT_TILES			200*200
#define LIGHT_LAST_TILE			(LIGHT_TILES - 1)
#define LIGHT_MAP_LEVELS		3
#define LIGHT_MLVL			(LIGHT_MAP_LEVELS - 1)

int32_t gItemGridLight[ LIGHT_MAP_LEVELS ][ LIGHT_TILES ];
int32_t gItemLightLvl = LIGHT_MAX_LVL;

int LightInit()
{
    LightTileResetAll();
    return 0;
}

int LightMapGetLt()
{
    return gItemLightLvl;
}

void LightMapEnlight( int Enlight, int Update ) // not used
{
    LightMapSetLt( gItemLightLvl + Enlight, Update );
}

void LightMapSetLt( int Intensity, int Update )
{
    uint32_t LightLvl, tmp;

    LightLvl = LIGHT_NIGHT_VISION_MODIFIER * PerkLvl( gObjDude, PERK_NIGHT_VISION ) + Intensity;
    if( LightLvl < LIGHT_MIN_LVL ) LightLvl = LIGHT_MIN_LVL;
    if( LightLvl > LIGHT_MAX_LVL ) LightLvl = LIGHT_MAX_LVL;
    tmp = gItemLightLvl;
    gItemLightLvl = LightLvl;
    if( !Update ) return;
    if( tmp != LightLvl ) TileUpdate();
}

void LightMapDimm( int Dimm, int Update ) // not used
{
    LightMapSetLt( gItemLightLvl - Dimm, Update );
}

int LightTileGetLtLimited( unsigned int MapLvl, unsigned int GridPos )
{
    int lt;

    if( MapLvl > LIGHT_MLVL || GridPos > LIGHT_LAST_TILE ) return 0;
    lt = gItemGridLight[ MapLvl ][ GridPos ];
    return ( lt < LIGHT_MAX_LVL ) ? lt : LIGHT_MAX_LVL;
}

int LightTileGetLt( unsigned int MapLvl, unsigned int GridPos )
{
    if( MapLvl > LIGHT_MLVL || GridPos > LIGHT_LAST_TILE ) return 0;
    return gItemGridLight[ MapLvl ][ GridPos ];
}

void LightTileSetLt( unsigned int MapLvl, unsigned int GridPos, int Intensity ) // not used
{
    if( MapLvl > LIGHT_MLVL || GridPos > LIGHT_LAST_TILE ) return;
    gItemGridLight[ MapLvl ][ GridPos ] = Intensity;
}

void LightTileEnlight( unsigned int MapLvl, unsigned int Pos, int Enlight )
{
    if( MapLvl > LIGHT_MLVL || Pos > LIGHT_LAST_TILE ) return;
    gItemGridLight[ MapLvl ][ Pos ] += Enlight;
}

void LightTileDimm( unsigned int MapLvl, unsigned int Pos, int Dimm )
{
    if( MapLvl > LIGHT_MLVL || Pos > LIGHT_LAST_TILE ) return;
    gItemGridLight[ MapLvl ][ Pos ] -= Dimm;
}

void LightTileResetAll()
{
    int lvl, i;

    for( lvl = 0; lvl < LIGHT_MAP_LEVELS; lvl++ ){
        for( i = 0; i < LIGHT_TILES; i++ ){
            gItemGridLight[ lvl ][ i ] = LIGHT_DEFAULT;
        }
    }
}

