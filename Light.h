#pragma once

#define LIGHT_DEFAULT	655

int  LightInit();
int  LightMapGetLt();
void LightMapEnlight( int LightModifier, int Update );
void LightMapSetLt( int Intensity, int Update );
void LightMapDimm( int Darkening, int Update );
int  LightTileGetLtLimited( unsigned int MapLvl, unsigned int GridPos );
int  LightTileGetLt( unsigned int MapLvl, unsigned int GridPos );
void LightTileSetLt( unsigned int MapLvl, unsigned int GridPos, int Intensity );
void LightTileEnlight( unsigned int MapLvl, unsigned int Pos, int Val );
void LightTileDimm( unsigned int MapLvl, unsigned int Pos, int Val );
void LightTileResetAll();


