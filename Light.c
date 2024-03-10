#include <stdint.h>
#include "Obj.h"
#include "Light.h"
#include "Perk.h"
#include "Tile.h"

#define LIGHT_NIGHT_VISION_MODIFIER 	0x3333
#define LIGHT_MIN_LVL			0x4000
#define LIGHT_MAX_LVL			0x10000
#define LIGHT_DEFAULT			655
#define LIGHT_TILES			200*200
#define LIGHT_LAST_TILE			(LIGHT_TILES - 1)
#define LIGHT_MAP_LEVELS		3
#define LIGHT_MLVL			(LIGHT_MAP_LEVELS - 1)

int32_t gLightTiles[ LIGHT_MAP_LEVELS ][ LIGHT_TILES ];
int32_t gLightMap = LIGHT_MAX_LVL;

int LightInit()
{
    LightTileResetAll();
    return 0;
}

int LightMapGetLt()
{
    return gLightMap;
}

void LightMapEnlight( int Enlight, int Update ) // not used
{
    LightMapSetLt( gLightMap + Enlight, Update );
}

void LightMapSetLt( int Intensity, int Update )
{
    uint32_t LightLvl, tmp;

    LightLvl = LIGHT_NIGHT_VISION_MODIFIER * PerkLvl( gObjDude, PERK_NIGHT_VISION ) + Intensity;
    if( LightLvl < LIGHT_MIN_LVL ) LightLvl = LIGHT_MIN_LVL;
    if( LightLvl > LIGHT_MAX_LVL ) LightLvl = LIGHT_MAX_LVL;
    tmp = gLightMap;
    gLightMap = LightLvl;
    if( !Update ) return;
    if( tmp != LightLvl ) TileUpdate();
}

void LightMapDimm( int Dimm, int Update ) // not used
{
    LightMapSetLt( gLightMap - Dimm, Update );
}

int LightTileGetLtLimited( unsigned int MapLvl, unsigned int GridPos )
{
    int lt;

    if( MapLvl > LIGHT_MLVL || GridPos > LIGHT_LAST_TILE ) return 0;
    lt = gLightTiles[ MapLvl ][ GridPos ];
    return ( lt < LIGHT_MAX_LVL ) ? lt : LIGHT_MAX_LVL;
}

int LightTileGetLt( unsigned int MapLvl, unsigned int GridPos )
{
    if( MapLvl > LIGHT_MLVL || GridPos > LIGHT_LAST_TILE ) return 0;
    return gLightTiles[ MapLvl ][ GridPos ];
}

void LightTileSetLt( unsigned int MapLvl, unsigned int GridPos, int Intensity ) // not used
{
    if( MapLvl > LIGHT_MLVL || GridPos > LIGHT_LAST_TILE ) return;
    gLightTiles[ MapLvl ][ GridPos ] = Intensity;
}

void LightTileEnlight( unsigned int MapLvl, unsigned int Pos, int Enlight )
{
    if( MapLvl > LIGHT_MLVL || Pos > LIGHT_LAST_TILE ) return;
    gLightTiles[ MapLvl ][ Pos ] += Enlight;
}

void LightTileDimm( unsigned int MapLvl, unsigned int Pos, int Dimm )
{
    if( MapLvl > LIGHT_MLVL || Pos > LIGHT_LAST_TILE ) return;
    gLightTiles[ MapLvl ][ Pos ] -= Dimm;
}

void LightTileResetAll()
{
    int lvl, i;
//int x, y;
    for( lvl = 0; lvl < LIGHT_MAP_LEVELS; lvl++ ){
        for( i = 0; i < LIGHT_TILES; i++ ){
            gLightTiles[ lvl ][ i ] = LIGHT_DEFAULT;
        }
//	for( y = 0; y < 200; y++){
//	    for( x = 0; x < 200; x++ ){
//		gLightTiles[ lvl ][ y *200 + x ] = ((y & 1) ^ ( x & 1)) ? 20000:40000;
//		gLightTiles[ lvl ][ y *200 + x ] = ( x & 1) ? 20000:40000;
//	    }
//	}
    }
}

