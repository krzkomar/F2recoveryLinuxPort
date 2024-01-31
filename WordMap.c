#include "FrameWork.h"

#define WM_GET_IMG( img_id, ArtId, w, h, obj, data, n )	\
        ArtId = ArtMakeId( 6, img_id, 0, 0, 0 );        \
        if( !(n = ArtLoadImg( ArtId, &obj ) ) ) return -1;\
        w = ArtGetObjWidth( n, 0, 0 );\
        h = ArtGetObjHeight( n, 0, 0 );\
        ArtClose( obj );\
        obj = (void *)-1;\
        data = ArtGetBitmap( ArtId, 0, 0, &obj );\
        if( !data ) return -1;

const char *gWmOffOnList[ 2 ] = { "off", "on" };
const char *gWmNoYesList[ 2 ] = { "no", "yes" };
const char *gWmFreqList[ 6 ] = { "none", "rare", "uncommon", "common", "frequent", "forced" };
const char *gWmFillDirList[ 9 ] = { "no_fill", "fill_n", "fill_s", "fill_e", "fill_w", "fill_nw", "fill_ne", "fill_sw", "fill_se" };
const char *gWmWeightList[ 4 ] = { "none", "light", "normal", "heavy" };
WmTerrain_t *gWmTerrainTypes = NULL;
int gWmTerrainTypesCnt = 0;
WmTile_t *gWmTiles = NULL;
int gWmTilesCnt = 0;
int gWmHorizTileCnt = 0;
WmArea_t *gWmAreas = NULL;
int gWmAreasCount = 0;
const char *gWmSizeList[ 3 ] = { "small", "medium", "large" };
Wmap_t *gWmMaps = NULL;
int gWmMapCount = 0;
int gWmMenuWin = -1;
CachePool_t *gWmWallPaper = (void *)-1;
unsigned int gWmWpapW= 0;
int gWmWpapH = 0;
char *gWmSurface = NULL;
void *gWmWpapImg = 0;
int gWmMapOffsetX = 0;
int gWmMapOffsetY = 0;
char *gWmColor01 = NULL;
int gWmUnk121 = 0;
const char *gWmEncTypeList[ 4 ] = { "nothing","ambush","fighting","and" };
const char *gWmOperatorList[ 8 ] = { "_", "==", "!=", "<","> ", " ", "and", "or" };
const char *gWmPositionsList[ 6 ] = { "surrounding", "straight_line", "double_line", "wedge", "cone", "huddle" };
const int gWmUnk125[4] = { 154, 155, 438, 439 };
int gWmKnownLocationsCount = 0;
int *gWmKnownLocations = NULL;
int gWmCityId = -1;
int gWmTimeStamp = 0;
int gWmUnk128 = 0;
int gWmUnk129 = 0;
int gWmUnk130 = 1;
int gWmMseTimer = 0;
int gWmMoveMapAllow = 1;
void *gWmUnk133 = 0;
CachePool_t *gWmUnk134 = ART_NULL;
int gWmUnk135 = 0;
int gWmUnk136 = 0;
char *gWmSfxFname[ 2 ] = { "cricket", "cricket1" };
int gWmFreqValues[ 6 ];
int gWmMapStart;
Config_t *gWmCfg;
Msg_t gWmMsg;


WmPlayer_t *gWmPlayers;

int gWmTimeStamp;
int gWmFontSave;
CachePool_t *gWmUnk79[4];
void *gWmUnk80[4];
int gWmUnk81[4];
int gWmUnk82[4];
int gWmUnk100;
int gWmWidgets[7];
CachePool_t *gWmUnk84[2];
CachePool_t *gWmUnk99[2];
void *gWmUnk83[ 2 ];
void *gWmUnk85[ 2 ];


int gWmUnk109[ 2 ];
int gWmMapDst[ 2 ];
int gWmUnk105[ 2 ];
int gWmUnk103[ 2 ];
int gWmUnk101[ 10 ];
int gWmGoToLocation;
int    gWmUnk23;
WmTileDsc_t    *gWmTile;
int    gWmUnk22;
int    gWmUnk13;
int    gWmSteps;
int    gWmUnk25;
int    gWmUnk26;
int    gWmUnk27;
int    gWmUnk28;
int    gWmUnk29;
int    gWmUnk30;
int    gWmUnk31;
int    gWmGoToLocation;
int    gWmAreaId;
int    gWmWorldPosX;
int    gWmWorldPosY;
int    gWmUnk11;
int    gWmUnk12;
int    gWmRandMapId;
int    gWmEncounterType;
int    gWmUnk34;
int    gWmUnk35;
int    gWmCurrentArea;
int    gWmGasoline;
CachePool_t *gWmUnk39;
CachePool_t *gWmUnk58;
CachePool_t *gWmUnk60;
CachePool_t *gWmUnk61;
CachePool_t *gWmUnk62;
CachePool_t *gWmUnk63;
CachePool_t *gWmUnk66;
CachePool_t *gWmUnk67;
int    gWmUnk53;
int    gWmUnk36;
int    gWmUnk37;
int    gWmTravelByCar;
int    gWmCarWidth;
int    gWmCarHeight;
int    gWmCarFrameNo;
void    *gWmLocListImg;
int    gWmLocListHeight;
int    gWmLocListStartLine;
void *gWmImgDat03;
ArtFrmHdr_t *gWmDialImg;
int    gWmDialWidth;
int    gWmDialHeight;
int    gWmDialFrame;
void    *gWmCarFrameImg;
int    gWmCarFrameWidth;
int    gWmCarFrameHeight;
void    *gWmVtLogoImg;
int    gWmVtLogoWidth;
int    gWmVtLogoHeight;
int    gWmUnk64;
int    gWmUnk65;
void    *gWmUnk68;
void    *gWmUnk69;
int    gWmLocListWidth;
ArtFrmHdr_t  *gWmCarImg;
int    gWmUnk72;
int    gWmUnk71[2];
int    gWmUnk70;
CachePool_t    *gWmUnk74;
CachePool_t    *gWmUnk76;
int    gWmUnk78;
int    gWmUnk73;
void    *gWmMonthImg;
ArtFrmHdr_t    *gWmDigitImg;
CachePool_t    *gWmUnk40;
char    *gWmUnk41;
CachePool_t    *gWmUnk42;
char    *gWmUnk43;
int    gWmUnk44;
int    gWmUnk45;
CachePool_t    *gWmUnk46;
char    *gWmUnk47;
int    gWmGasoline0;
int    gWmUnk48;
CachePool_t    *gWmUnk49;
char   *gWmUnk50;
int    gWmUnk51;
int    gWmGoToLocation;
int    gWmUnk53;
int    gWmUnk54;
int    gWmUnk55;
int    gWmUnk56;
int    gWmUnk57;
int    gWmUnk17;
int    gWmUnk16;

int gMapCurrentMapId; // --

WmImg_t gWmMenuImg[3];
int gWmUnk94[ 14 ];
WmEnctr_t *gWmEncounterTable;
int gWmPlayersCnt;
int gWmEncounterTableCnt;

/*****************************************************************/

int WmWorldMapInit()
{
    char stmp[ 260 ];

    if( WmInitVars1() == -1 ) return -1;
    if( MessageInit( &gWmMsg ) != 1 ) return -1;    
    sprintf( stmp, "%s%s", gGamePath, "worldmap.msg" );
    if( MessageLoad( &gWmMsg, stmp ) != 1 ) return -1;
    if( WmLoadWmapTxt() == -1 ) return -1;    
    gWmUnk16 = 350 * gWmHorizTileCnt - 450;
    gWmUnk17 = 300 * (gWmTilesCnt / gWmHorizTileCnt) - 443;
    gWmColor01 = PalExtOpen( gPalColorCubeRGB[0][31][0] );
    WmUnk69( gWmWorldPosX, gWmWorldPosY );
    WmSaveWmapDat();
    return 0;    
}

int WmInitVars1()
{
    int i;

    gWmUnk23 = 0;
    gWmAreaId = -1;
    gWmWorldPosX = 173;
    gWmWorldPosY = 122;
    gWmTile = NULL;
    gWmUnk22 = 0;
    gWmUnk13 = 0;
    gWmUnk11 = -1;
    gWmUnk12 = -1;
    gWmSteps = 0;
    gWmUnk25 = 0;
    gWmUnk26 = 0;
    gWmUnk27 = 0;
    gWmUnk28 = 0;
    gWmUnk29 = 0;
    gWmUnk30 = 0;
    gWmUnk31 = 0;
    gWmRandMapId = -1;
    gWmEncounterType = -1;
    gWmUnk34 = -1;
    gWmUnk35 = -1;
    gWmUnk36 = 0;
    gWmUnk37 = 0;
    gWmTravelByCar = 0;
    gWmCurrentArea = -1;
    gWmGasoline = 80000;
    gWmUnk39 = ART_NULL;
    gWmCarWidth = 0;
    gWmCarHeight = 0;
    gWmCarFrameNo = 0;
    gWmUnk40 = ART_NULL;
    gWmUnk41 = 0;
    gWmUnk42 = ART_NULL;
    gWmUnk43 = 0;
    gWmUnk44 = 0;
    gWmUnk45 = 0;
    gWmUnk46 = ART_NULL;
    gWmUnk47 = 0;
    gWmGasoline0 = 0;
    gWmUnk48 = 0;
    gWmUnk49 = ART_NULL;
    gWmUnk50 = NULL; // start indeksu ?
    gWmUnk51 = 0;
    gWmGoToLocation = 0;
    gWmUnk53 = 0;
    gWmUnk54 = 0;
    gWmCarImg = 0;
    for( i = 0; i < 4; i++ ){
        gWmUnk79[ i ] = ART_NULL;
        gWmUnk80[ i ] = NULL;
        gWmUnk81[ i ] = 0;
        gWmUnk82[ i ] = 0;
    }
    gWmUnk17 = 0;
    gWmUnk58 = ART_NULL;
    gWmLocListImg = 0;
    gWmLocListWidth = 0;
    gWmLocListHeight = 0;
    gWmLocListStartLine = 0;
    gWmUnk60 = ART_NULL;
    gWmImgDat03 = 0;
    gWmUnk61 = ART_NULL;
    gWmDialImg = 0;
    gWmDialWidth = 0;
    gWmDialHeight = 0;
    gWmDialFrame = 0;
    gWmUnk62 = ART_NULL;
    gWmCarFrameImg = 0;
    gWmCarFrameWidth = 0;
    gWmCarFrameHeight = 0;
    gWmUnk63 = ART_NULL;
    gWmVtLogoImg = 0;
    gWmVtLogoWidth = 0;
    gWmVtLogoHeight = 0;
    gWmUnk64 = 0;
    gWmUnk65 = 0;
    gWmUnk66 = ART_NULL;
    gWmUnk67 = ART_NULL;
    gWmUnk68 = 0;
    gWmUnk69 = 0;
    gWmUnk16 = 0;
    for( i = 0; i < 2; i++ ){
        gWmUnk99[ i ] = ART_NULL;
        gWmUnk84[ i ] = ART_NULL;
        gWmUnk83[ i ] = NULL;
        gWmUnk85[ i ] = NULL;
    }
    gWmUnk72 = 0;
    gWmUnk73 = 0;
    gWmUnk70 = 0;
    gWmUnk74 = ART_NULL;
    gWmMonthImg = 0;
    gWmUnk76 = ART_NULL;
    gWmDigitImg = 0;
    gWmUnk78 = 0;
    return 0;
}

int WmInitVars2()
{
    int i;

    gWmUnk23 = 0;
    gWmTile = NULL;
    gWmUnk22 = 0;
    gWmUnk13 = 0;
    gWmSteps = 0;
    gWmUnk25 = 0;
    gWmUnk26 = 0;
    gWmUnk27 = 0;
    gWmUnk28 = 0;
    gWmUnk29 = 0;
    gWmUnk30 = 0;
    gWmUnk31 = 0;
    gWmGoToLocation = 0;
    gWmAreaId = -1;
    gWmWorldPosX = 173;
    gWmWorldPosY = 122;
    gWmUnk11 = -1;
    gWmUnk12 = -1;
    gWmRandMapId = -1;
    gWmEncounterType = -1;
    gWmUnk34 = -1;
    gWmUnk35 = -1;
    gWmCurrentArea = -1;
    gWmGasoline = 80000;
    gWmUnk39 = ART_NULL;
    gWmUnk58 = ART_NULL;
    gWmUnk60 = ART_NULL;
    gWmUnk61 = ART_NULL;
    gWmUnk62 = ART_NULL;
    gWmUnk63 = ART_NULL;
    gWmUnk66 = ART_NULL;
    gWmUnk67 = ART_NULL;
    gWmUnk53 = 0;
    gWmUnk36 = 0;
    gWmUnk37 = 0;
    gWmTravelByCar = 0;
    gWmCarWidth = 0;
    gWmCarHeight = 0;
    gWmCarFrameNo = 0;
    gWmLocListImg = 0;
    gWmLocListHeight = 0;
    gWmLocListStartLine = 0;
    gWmImgDat03 = 0;
    gWmDialImg = 0;
    gWmDialWidth = 0;
    gWmDialHeight = 0;
    gWmDialFrame = 0;
    gWmCarFrameImg = 0;
    gWmCarFrameWidth = 0;
    gWmCarFrameHeight = 0;
    gWmVtLogoImg = 0;
    gWmVtLogoWidth = 0;
    gWmVtLogoHeight = 0;
    gWmUnk64 = 0;
    gWmUnk65 = 0;
    gWmUnk68 = 0;
    gWmUnk69 = 0;
    gWmLocListWidth = 0;
    gWmCarImg = 0;
    for( i = 0; i < 2; i++ ){
        gWmUnk83[ i ] = NULL;
        gWmUnk99[ i ] = ART_NULL;
        gWmUnk85[ i ] = NULL;
        gWmUnk84[ i ] = ART_NULL;
    }
    gWmUnk74 = ART_NULL;
    gWmUnk76 = ART_NULL;
    gWmUnk78 = 0;
    gWmUnk72 = 0;
    gWmUnk73 = 0;
    gWmUnk70 = 0;
    gWmMonthImg = 0;
    gWmDigitImg = 0;
    WmUnk69( gWmWorldPosX, gWmWorldPosY );
    return 0;
}

int WmClose()
{
    if( gWmTerrainTypes ){ Free( gWmTerrainTypes ); gWmTerrainTypes = NULL; }
    if( gWmTiles ){ Free( gWmTiles ); gWmTiles = NULL; }
    gWmHorizTileCnt = 0;
    gWmTilesCnt = 0;
    if( gWmEncounterTable ){ Free( gWmEncounterTable ); gWmEncounterTable = NULL; }
    gWmEncounterTableCnt = 0;
    if( gWmPlayers ){ Free( gWmPlayers ); gWmPlayers = NULL; }
    gWmPlayersCnt = 0;
    if( gWmAreas ){ Free( gWmAreas ); gWmAreas = NULL; }
    gWmAreasCount = 0;
    if( gWmMaps ){ Free( gWmMaps ); gWmMaps = NULL; }
    gWmMapCount = 0;
    if( gWmColor01 ){ 
	PalExtClose( gPalColorCubeRGB[0][31][0] );
        gWmColor01 = 0;
    }
    return ( MessageClose( &gWmMsg ) == 1 ) - 1;
}

int WmUnk108()
{
    gWmMapOffsetX = 0;
    gWmMapOffsetY = 0;
    WmLoadWmapDat();
    WmClearTerrain( 0 );
    return (WmInitVars2() != -1) - 1;
}

int WmSaveWmapDatFile( xFile_t *fh )
{
    int i, j, k, n;

    if( dbputBei( fh, gWmUnk23 ) == -1 ) return -1;
    if( dbputBei( fh, gWmAreaId ) == -1 ) return -1;
    if( dbputBei( fh, gWmWorldPosX ) == -1 ) return -1;
    if( dbputBei( fh, gWmWorldPosY ) == -1 ) return -1;    
    if( dbputBei( fh, gWmUnk31 ) == -1 ) return -1;
    if( dbputBei( fh, gWmRandMapId ) == -1 ) return -1;
    if( dbputBei( fh, gWmEncounterType ) == -1 ) return -1;
    if( dbputBei( fh, gWmUnk34 ) == -1 ) return -1;
    if( dbputBei( fh, gWmTravelByCar ) == -1 ) return -1;
    if( dbputBei( fh, gWmCurrentArea ) == -1 ) return -1;
    if( dbputBei( fh, gWmGasoline ) == -1 ) return -1;
    if( dbputBei( fh, gWmAreasCount ) == -1 ) return -1;
    if( gWmAreasCount > 0 ){
        for( i = 0; i < gWmAreasCount; i++ ){
            if( dbputBei( fh, gWmAreas[ i ].WorldPosX ) == -1 )  return -1;
            if( dbputBei( fh, gWmAreas[ i ].WorldPosY) == -1 )  return -1;
            if( dbputBei( fh, gWmAreas[ i ].StartState) == -1 )  return -1;
            if( dbputBei( fh, gWmAreas[ i ].LockState[1]) == -1 )  return -1;
            if( dbputBei( fh, gWmAreas[ i ].EntranceCnt) == -1 )  return -1;
            for( j = 0; j < gWmAreas[ i ].EntranceCnt; j++ ){
                if( dbputBei( fh, gWmAreas[ i ].Entrances[ j ].i01 ) == -1 ) return -1;
            }
        }
    }        
    if( dbputBei( fh, gWmTilesCnt ) == -1 ) return -1;
    if( dbputBei(fh, gWmHorizTileCnt) == -1 ) return -1;
    for( i = 0; i < gWmTilesCnt; i++ ){
	for( j = 0; j < 6; j++ ){
	    for( k = 0; k < 7; k++ ){
        	if( dbputBei( fh, gWmTiles[ i ].Dsc[j][k].Fog ) == -1 ) return -1;
            }
        }
    }
    n = 0;
    for( j = 0 ;j < gWmEncounterTableCnt; j++ ){
        for( k = 0; k < gWmEncounterTable[ j ].TabIdxCnt; k++ ){
            if( gWmEncounterTable[ j ].TabIdx[ k ].Counter != -1 ) n++;
        }
    }                        
    if( dbputBei( fh, n ) == -1 ) return -1;
    for( i = 0; i < gWmEncounterTableCnt; i++ ){
        for( j = 0; j < gWmEncounterTable[ i ].TabIdxCnt; j++ ){
            if( (n = gWmEncounterTable[ i ].TabIdx[ j ].Counter) != -1 ){
                if( dbputBei( fh, i ) == -1 ) return -1;
                if( dbputBei( fh, j ) == -1 ) return -1;
                if( dbputBei( fh, n ) == -1 ) return -1;
            }
        }
    }
    return 0;
}

int WmLoadWmapDatFile( xFile_t *fh )
{
    int i, j, k, a, b, c, pint;
    
    if( dbgetBei( fh, &gWmUnk23  ) == -1 ) return -1; 	 // Unk
    if( dbgetBei( fh, &gWmAreaId ) == -1 ) return -1;    // Unk
    if( dbgetBei( fh, &gWmWorldPosX ) == -1 ) return -1; // pos X
    if( dbgetBei( fh, &gWmWorldPosY ) == -1 ) return -1; // pos Y
    if( dbgetBei( fh, &gWmUnk31 ) == -1 ) return -1;     // Unk
    if( dbgetBei( fh, &gWmRandMapId ) == -1 ) return -1; // random map
    if( dbgetBei( fh, &gWmEncounterType ) == -1 ) return -1;	 // Unk
    if( dbgetBei( fh, &gWmUnk34 ) == -1 ) return -1;     // Unk
    if( dbgetBei( fh, &gWmTravelByCar ) == -1 ) return -1; // Unk
    if( dbgetBei( fh, &gWmCurrentArea ) == -1 ) return -1; // Number of the map the car is on
    if( dbgetBei( fh, &gWmGasoline ) == -1 ) return -1;    // Amount of fuel
    if( dbgetBei( fh, &a ) == -1 ) return -1;    	   // City information, number of records
    for( i = 0; i < a; i++ ){
        if( dbgetBei( fh, &gWmAreas[ i ].WorldPosX ) == -1 ) return -1;  // City coordinate X
        if( dbgetBei( fh, &gWmAreas[ i ].WorldPosY ) == -1 ) return -1;  // City coordinate Y
        if( dbgetBei( fh, &gWmAreas[ i ].StartState ) == -1 ) return -1; // State of the city on world map ( accesible or not)
        if( dbgetBei( fh, &pint ) == -1 ) return -1;			 // City button on world map ( accesible or not)
        gWmAreas[ i ].LockState[1] = pint;
        if( dbgetBei( fh, &b ) == -1 ) return -1;			 // number of entrances
        for( j = 0; j < b; j++ ){
            if( dbgetBei( fh, &gWmAreas[ i ].Entrances[ j ].i01 ) == -1 ) return -1; // entrance on/off
        }            
    }    
    if( dbgetBei( fh, &a ) == -1 ) return -1; // number of tiles on the world map
    if( dbgetBei( fh, &b ) == -1 ) return -1; // value of num_horizontal_tiles
    for( i = 0; i < a; i++ ){
	for( j = 0; j < 6; j++ ){
    	    for( k = 0; k < 7; k++){
        	if( dbgetBei( fh, &pint ) == -1 ) return -1; // world map 'fog'
    		gWmTiles[ i ].Dsc[ j ][ k ].Fog = pint;
    	    }
    	}        	
    }
    if( dbgetBei( fh, &a ) == -1 ) return -1; // number of special encounters (373)
    for( i = 0; i < a; i++ ){
        if( dbgetBei( fh, &b ) == -1 ) return -1; // Encounter table number
        if( dbgetBei( fh, &c ) == -1 ) return -1; // enc_X (number in the encounter table )
        if( dbgetBei( fh, &pint ) == -1 ) return -1; // already encountered (or not )
        gWmEncounterTable[ b ].TabIdx[ c ].Counter = pint;
    }
    WmUnk48(); 
    return 0;
}

int WmSaveWmapDat()
{
    xFile_t *fh;
    int err = 0;
    
    if( !( fh = dbOpen( "worldmap.dat", "wb" ) ) ) return -1;
    if( WmSaveWmapDatFile( fh ) == -1 ) err = -1;
    dbClose( fh ); 
    return err;
}

int WmLoadWmapDat()
{
    xFile_t *fh;
    int err = 0;

    if( !( fh = dbOpen( "worldmap.dat", "rb" ) ) ) return -1;
    if( WmLoadWmapDatFile( fh ) == -1 ) err = -1;
    dbClose( fh );

    return err;
}

int WmLoadWmapTxt()
{
    int i, k, idx, j, tmp;
    WmTile_t *tiles;    
    Config_t WmCfg;
    char stmp1[40], stmp2[20], *s;    

    s = NULL;
    if( WmAreaInit() == -1 || !CfgInit( &WmCfg ) ) return -1;

    if( CfgLoadFromFile( &WmCfg, "data/worldmap.txt", 1 ) == 1 ){
        for( i = 0; i < 6; i++ ){
            if( !CfgGetInteger( &WmCfg, "data", gWmFreqList[ i ], &gWmFreqValues[ i ] ) ) break;
        }
        CfgGetString( &WmCfg, "data", "terrain_types", &s );
        WmSetTerrainTypes( &WmCfg, s );            
        for( j = 0; ;j++ ){
            sprintf( stmp1, "Encounter Table %d", j );
            if( !CfgGetString( &WmCfg, stmp1, "lookup_name", &s ) ) break;
            if( WmLoadEncounterTable( &WmCfg, s ) == -1 ) return -1;
        }

        if( !CfgGetInteger( &WmCfg, "Tile Data", "num_horizontal_tiles", &gWmHorizTileCnt ) ){ WinMsgError( "\nwmConfigInit::Error loading tile data!" ); exit( 1 ); }
        for( k = 0; k < 9999; k++ ){
            sprintf( stmp1, "Tile %d", k );
            if( !CfgGetInteger( &WmCfg, stmp1, "art_idx", &tmp ) ) break;
            gWmTilesCnt++;
            tiles = Realloc( gWmTiles, gWmTilesCnt * sizeof( WmTile_t ) );
            if( !tiles ){ WinMsgError( "\nwmConfigInit::Error loading tiles!" ); exit( 1 ); }
            idx = gWmTilesCnt - 1;
            tiles[ idx ].ArtId = -1;
            tiles[ idx ].Pix = NULL;
            tiles[ idx ].WorldMask = NULL;
            tiles[ idx ].WalkMaskName[0] = '\0';
            gWmTiles = tiles;
            tiles[ idx ].Img = ART_NULL;
            tiles[ idx ].Difficulty = 0;
            gWmTiles[ idx ].ArtId = ArtMakeId( 6, tmp, 0, 0, 0 );
            if( CfgGetInteger( &WmCfg, stmp1, "encounter_difficulty", &tmp ) == 1 ) gWmTiles[ idx ].Difficulty = tmp;
            if( CfgGetString( &WmCfg, stmp1, "walk_mask_name", &s ) == 1 ) strncpy( gWmTiles[ idx ].WalkMaskName, s, 40 );
            for( i = 0; i < 6; i++ ){
                for( j = 0; j < 7; j++ ){
                    sprintf( stmp2, "%d_%d", j, i );
                    if( !CfgGetString( &WmCfg, stmp1, stmp2, &s ) ){ WinMsgError( "\nwmConfigInit::Error loading tiles!" ); exit( 1 ); }
                    if( WmSetTileDsc( &gWmTiles[ gWmTilesCnt - 1 ], j, i, s ) == -1 ){ WinMsgError( "\nwmConfigInit::Error loading tiles!" ); exit( 1 ); }
                }
            }
        }
    }
    CfgFree( &WmCfg );   
    return 0; 
}

int WmLoadEncounterTable( Config_t *cfg, char *SectionName )
{
    WmEnctr_t *enct, *enc;
    int cnt, i;
    unsigned int tmp;
    char stmp[40], *s;

    s = NULL;
    gWmEncounterTableCnt++;
    enct = Realloc( gWmEncounterTable, gWmEncounterTableCnt * sizeof( WmEnctr_t ) );
    if( !enct ){ WinMsgError( "\nwmConfigInit::Error loading Encounter Table!" ); exit( 1 ); }
    cnt = gWmEncounterTableCnt - 1;
    gWmEncounterTable = enct;

    enc = &enct[ cnt ];
    enc->Name[ 0 ] = '\0';
    enc->IdxCnt = 0;
    enc->i01 = 0;
    enc->TabIdxCnt = 0;
    enc->EncIdx = cnt;
    strncpy( enc->Name, SectionName, 40 );

    if( CfgGetString( cfg, SectionName, "maps", &s ) == 1 ){
        do{
            if( enc->IdxCnt >= 6 ) break;
            tmp = strlen( s );
            if( StrParseFromFunc( &s, &enc->MapIdxPool[ enc->IdxCnt ], (void *)WmLookupMapByName ) == -1 ) break;
            enc->IdxCnt++;
            if( !*s ) break;
        }while( (tmp + s) > s );
    }

    i = 0;
    while( 1 ){
        sprintf( stmp, "enc_%02d", i );
        if( !CfgGetString( cfg, SectionName, stmp, &s ) ) break;
        if( i >= 40 ){ WinMsgError( "\nwmConfigInit::Error: Encounter Table: Too many table indexes!!" ); exit( 1 ); }
        gWmCfg = cfg;
        if( WmLoadEncounters( &enc->TabIdx[ enc->TabIdxCnt ], s ) == -1 ) return -1;
        i++; enc->TabIdxCnt++;
    }
    return 0;
}

int WmLoadEncounters( WmIdx_t *EncIdx, char *line )
{
    char *s;

    EncIdx->Flags = 0;
    EncIdx->MapId = -1;
    EncIdx->SceneryType = 2;
    EncIdx->Chance = 0;
    EncIdx->Counter = -1;
    EncIdx->EncCnt = 0;
    if( WmParseExprZero( &EncIdx->Expr ) == -1 ) return -1;    
    while( line && *line ){
        StrParseGetPrefixedInt( &line, "chance", &EncIdx->Chance, ":" );
        StrParseGetPrefixedInt( &line, "counter", &EncIdx->Counter, ":" );
        if( strstr( line, "special" ) ){
            EncIdx->Flags |= 0x01;
            line += 8;
        }
        if( line ){            
            if( ( s = strstr( line, "map:" ) ) ){
                line = s + 4;
                StrParseFromFunc( &line, &EncIdx->MapId, (void *)WmLookupMapByName );
            }
        }
        if( WmParseEnc( EncIdx, &line ) == -1 ) break;
        if( line ){            
            if( (s = strstr( line, "scenery:" )) ){
                line = s + 8;
                StrParseFromList( &line, &EncIdx->SceneryType, gWmWeightList, 4 );
            }
        }
        WmParseExprCond( &line, "if", &EncIdx->Expr );
    }
    return 0;
}

int WmParseEnc( WmIdx_t *xx, char **pline )
{
    WmEnc_t *e;
    char *s;
    char *line;
    char *ps;

    line = *pline;
    if( strncasecmp( line, "Enc:", 4 ) ) return -1;
    line += 4;
    ps = strstr( line, "," );
    if( ps ){
        *pline = ps + 1;
        *ps = '\0';
    } else {
        *pline = NULL;
    }

    while( line ){
        e = &xx->Enc[ xx->EncCnt ];
        e->RandMin = 1;
        e->RandMax = 1;
        e->i03 = -1;
        e->Type = 0;
        
        if( *line == '(' ){ // get bracket (num1-num2)
            e->RandMin = strtol( ++line, NULL, 10 );
            for( ; *line != '-'; line++ ){
                if( !*line ) break;
            }
            if( *line == '-' ) line++;
            e->RandMax = strtol( line, NULL, 10 );
            for( ; *line != ')'; line++ ){
                if( !*line ) break;
            }
            if( *line == ')' ) line++;
        }
        while( *line == ' ' ) line++;

        s = strchr( line, ' ');
        if( s ) *s = '\0'; // cut string
        if( StrParseFromFunc( &line, &e->i03, (void *)WmParseEncPlayer ) == -1 ) return -1;
        if( s ){ // restore string
    	    line = s + 1;
            *s = ' ';
        }

        s = strchr( line, ' ' );
        if( s ) *s = '\0';
        if( *line ) StrParseFromList( &line, &e->Type, gWmEncTypeList, 4 );
        if( s ) *s = ' ';

        xx->EncCnt++;
        for( ; *line == ' '; line++ );
        if( *line == '\0' ) line = NULL;
    }
    if( ps ) *ps = ',';
    return 0;
}

int WmParseEncPlayer( char *line, int *PlayerId )
{
    int err;
    
    *PlayerId = 0;
    if( (err = strcasecmp( line, "player" ) ) ){
        if( WmGetPlayerId( line, PlayerId ) != -1 ) return 0;
        if( (err = WmParseAddEncounter( line, PlayerId )) != -1 ) return 0;
    } else {
        *PlayerId = -1;
    }
    return err;
}

int WmGetPlayerId( char *PlayerName, int *PlayerId )
{
    int i;

    for( i = 0; i < gWmPlayersCnt; i++ ){
        if( !strcasecmp( gWmPlayers[ i ].Name, PlayerName ) ) break;
    }
    if( i >= gWmPlayersCnt ){
        *PlayerId = -1;
        return -1;
    }
    *PlayerId = i;
    return 0;
}

int WmParseAddEncounter( char *line, int *PlayerId )
{
    char stmp1[80], stmp2[80], *s, *EncType;
    WmPlayer_t *pl, *p;
    int i, tmp;

    sprintf( stmp1, "Encounter: %s", line );
    sprintf( stmp2, "type_00" );
    if( !CfgGetString( gWmCfg, stmp1, stmp2, &EncType ) ) return -1;
    gWmPlayersCnt++;
    if( !(pl = Realloc( gWmPlayers, gWmPlayersCnt * sizeof( WmPlayer_t )) ) ){ WinMsgError( "\nwmConfigInit::Error Reading EncBaseType!" ); exit( 1 ); }
    gWmPlayers = pl;
    p = &pl[ gWmPlayersCnt - 1 ];
    p->Name[ 0 ] = '\0';
    p->Position = 0;
    p->Spacing = 1;
    p->Distance = -1;
    p->TeamCnt = 0;
    strncpy( p->Name, line, 40 );
    do{
        if( WmParsePlayerState( &p->Team[ p->TeamCnt ], &s ) == -1 ) return -1;
        p->TeamCnt++;
        sprintf( stmp2, "type_%02d", p->TeamCnt );
    }while( CfgGetString( gWmCfg, stmp1, stmp2, &s ) );
    CfgGetInteger( gWmCfg, stmp1, "team_num", &tmp );
    for( i = 0; i < p->TeamCnt; i++ ){
        if( (p->Team[ i ].Pid >> 24) == 1 ) p->Team[ i ].TeamNum = tmp;
    }
    if( CfgGetString( gWmCfg, stmp1, "position", &s ) == 1 ){
        StrParseFromList( &s, &p->Position, gWmPositionsList, 6 );
        StrParseGetPrefixedInt( &s, "spacing", &p->Spacing, ":" );
        StrParseGetPrefixedInt( &s, "distance", &p->Distance, ":" );
    }
    *PlayerId = gWmPlayersCnt - 1;
    return 0;
}

int WmParsePlayerState( WmTeam_t *p, char **pLine )
{
    WmItem_t *Items;
    char *s, *line;
    int i;

    line = *pLine;
    p->i05 = -1;
    p->RatioFlg = 1;
    p->Ratio = 100;
    p->Pid = -1;
    p->Flags = 0;
    p->Distance = 0;
    p->TileNum = -1;
    p->ItemsCnt = 0;
    p->ScriptId = -1;
    p->TeamNum = -1;
    if( WmParseExprZero( &p->Expr ) == -1 ) return -1;
    if( !StrParseGetPrefixedInt( &line, "ratio", &p->Ratio, ":") ) p->RatioFlg = 0;
    s = strstr( line, "dead," );
    if( s == line ){
        p->Flags |= 0x01;
        line += 5;
    }
    StrParseGetPrefixedInt( &line, "pid", &p->Pid, ":" );
    if( !p->Pid ) p->Pid = -1;
    StrParseGetPrefixedInt( &line, "distance", &p->Distance, ":" );
    StrParseGetPrefixedInt( &line, "tilenum", &p->TileNum, ":" );
    Items = p->Items;
    for( i = 0; i < 10; i++ ){
        if( !strstr( line, "item:" ) ) break;
        WmParseItems( &line, &Items[ p->ItemsCnt ], &p->ItemsCnt, ":" );
    }
    StrParseGetPrefixedInt( &line, "script", &p->ScriptId, ":" );
    WmParseExprCond( &line, "if", &p->Expr );
    return 0;
}

int WmParseExprZero( WmExpr_t *cond )
{
    int i;
    
    cond->Args = 0;
    for( i = 0; i < 3; i++ ){
        cond->Cond[ i ].Operator = 0;
        cond->Cond[ i ].Arg1 = 0;
        cond->Cond[ i ].Arg2 = 0;
        cond->Cond[ i ].ArgType = 0;
    }
    cond->i02[0] = 0;
    cond->i02[1] = 0;
    return 0;
}

int WmSetTerrainTypes( Config_t *cfg, char *Line )
{
    char *s, *p1, *p2, c1, c2;
    int a, i;

    i = 1;
    if( !*Line ) return -1;
    s = Line;
    if( *Line ){
        do{
            if( *s == ',' ) i++;
        }while ( *++s );
    }
    gWmTerrainTypesCnt = i;
    if( !(gWmTerrainTypes = Malloc( i * sizeof( WmTerrain_t ) ) ) ) return -1;
    for( i = 0; i < gWmTerrainTypesCnt; i++ ){
        gWmTerrainTypes[ i ].Name[ 0 ] = '\0';
        gWmTerrainTypes[ i ].i01 = 0;
        gWmTerrainTypes[ i ].MapId = 0;
    }
    StrLwr( Line );
    s = Line;        
    for( i = 0; *s; i++ ){
	s += strspn( s, " " );
        a = strcspn( s, "," );
        p1 = s + a;
        c1 = *p1;
        s[ a ] = '\0';
        a = strcspn( s, ":" );
        p2 = &s[ a ];
        c2 = s[ a ];
        s[a] = '\0';
        strncpy( gWmTerrainTypes[ i ].Name, s, 40 );
        gWmTerrainTypes[ i ].i01 = strtol( &s[ a + 1 ], NULL, 10 );
        *p2 = c2;
        *p1 = c1;
	s = p1;
        if( c1 == ',' ) s++;
    }
    if( gWmTerrainTypesCnt <= 0 ) return 0;
    for( i = 0; i < gWmTerrainTypesCnt; i++ ){
        if( WmSetRandTerrMap( cfg, &gWmTerrainTypes[ i ] ) == -1 ) return -1;
    }
    return 0;
}

int WmSetRandTerrMap( Config_t *cfg, WmTerrain_t *terr )
{
    char stmp1[40], stmp2[80], *s;

    sprintf( stmp2, "Random Maps: %s", terr->Name );
    do{
        sprintf( stmp1, "map_%02d", terr->MapId );
        if( !CfgGetString( cfg, stmp2, stmp1, &s ) ) return 0;
        if( StrParseFromFunc( &s, &terr->MapName[ terr->MapId ], (void *)WmLookupMapByName ) == -1 ) return -1;
        terr->MapId++;
    }while( terr->MapId < 20 );        
    return -1;
}

int WmSetTileDsc( WmTile_t *tile, int a2, int a3, char *text )
{
    WmTileDsc_t *TileDsc;
    int i;

    if( !*text ) return -1;
    TileDsc = &tile->Dsc[ a3 ][ a2 ];
    TileDsc->Fog = 0;
    if( StrParseFromFunc( &text, &TileDsc->TerrainType, WmFindMatchForTerrainType ) == -1 ) return -1;
    if( StrParseFromList( &text, &TileDsc->FillDir, gWmFillDirList, 9 ) == -1 ) return -1;    
    for( i = 0; i < 3; i++ ){
        if( StrParseFromList( &text, &TileDsc->Freq[ i ], gWmFreqList, 6 ) == -1 ) return -1;
    }        
    if( StrParseFromFunc( &text, &TileDsc->EncounterType, WmFindMatchForEncounterType ) == -1 ) return -1;
    return 0;
}

int WmFindMatchForEncounterType( int *pIdx, char *EncTypeName )
{
    int i;

    for( i = 0; i < gWmEncounterTableCnt; i++ ){
        if( !strcasecmp( EncTypeName, gWmEncounterTable[ i ].Name ) ) break;
    }
    if( i >= gWmEncounterTableCnt ){
        eprintf( "\nWorldMap Error: Couldn't find match for Encounter Type!" );
        *pIdx = -1;
        return -1;
    }
    *pIdx = i;
    return 0;
}

int WmFindMatchForTerrainType( int *pIdx, char *TerrainTypeName ) 
{
    int i;

    for( i = 0; i < gWmTerrainTypesCnt; i++ ){
        if( !strcasecmp( TerrainTypeName, gWmTerrainTypes[ i ].Name ) ) break;
    }    
    if( i >= gWmTerrainTypesCnt ){
        eprintf("\nWorldMap Error: Couldn't find match for Terrain Type!");
        *pIdx = -1;
        return -1;
    }
    *pIdx = i;
    return 0;
}

int WmParseItems( char **pLine, WmItem_t *item, int *idx, const char *separator )
{
    char *line, *s, tmp1, tmp2;
    int pos2, pos1, err;

    line = *pLine;
    err = 0;
    if( !**pLine ) return -1;
    StrLwr(line);
    if( *line == ',' ){
        ++line;
        ++*pLine;
    }
    s = &line[ strspn( line, " " ) ];
    pos1 = strcspn( s, "," );
    tmp1 = s[ pos1 ];
    s[ pos1 ] = '\0';
    pos2 = strcspn( s, separator );
    tmp2 = s[ pos2 ];
    s[ pos2 ] = '\0';
    if( !strcmp( s, "item" ) ){
        *pLine += pos1 + 1;
        WmParseItemState( &s[ pos2 + 1 ], item );
        err = 1;
        (*idx)++;
    }
    s[ pos2 ] = tmp2;
    s[ pos1 ] = tmp1;
    return ( err ) ? 0 : -1;
}

int WmParseItemState( char *line, WmItem_t *item )
{
    char *s;

    while( *line == ' ' ) line++;
    item->i03 = 1;
    item->Used = 0;
    item->i02 = 1;
    if( *line == '(' ){ // get '(num-num)'
	item->i02 = strtol( ++line, NULL, 10 );
	while( 1 ){
    	    s = line + 1;
    	    if( !(IsTable[ *line + 1 ] & 0x20) ) break;
    	    line++;
	}
	if( *line == '-' ){
    	    line++;
    	    item->i03 = strtol( s, NULL, 10 );
    	    while( IsTable[ *line + 1 ] & 0x20 ) line++;
	} else {
    	    item->i03 = item->i02;
	}
	if( *line == ')' ) line++;
    }
    while( *line == ' ' ) line++;
    item->i01 = strtol( line, NULL, 10 );
    while( IsTable[ *line + 1 ] & 0x20 ) line++;
    while( *line == ' ' ) line++;
    if( strstr(line, "{wielded}") || strstr(line, "(wielded)") || strstr(line, "{worn}") || strstr(line, "(worn)") ) item->Used = 1;
    return 0;
}

int WmParseExprCond( char **pLine, char *Expr, WmExpr_t *Cond )
{
    int err;
    char *s;

    while( Cond->Args < 3 ){
        err = WmParseGetExprArgs(pLine, Expr, &Cond->Cond[Cond->Args].ArgType, &Cond->Cond[Cond->Args].Operator, &Cond->Cond[Cond->Args].Arg1, &Cond->Cond[Cond->Args].Arg2 );
        if( err == -1 ) break;
        Cond->Args++;
        if( !*pLine ) break;
        s = strstr( *pLine, "and" );
        if( s ){
            *pLine = s + 3;
            Cond->Cond[ Cond->Args + 2].Arg2 = 1;
        } else {
            s = strstr( *pLine, "or" );
            if( !s ) return 0;
            *pLine = s + 2;
            Cond->Cond[ Cond->Args + 2].Arg2 = 2;
        }
    }
    return err;
}

int WmParseGetExprArgs( char **pLine, char *a2, int *pType, int *pOperator, int *pVal, int *ArgRt )
{
    char *p, *s, *str, c1, c2;
    int n1, n2, flg;    

    flg = 0;
    if( *pLine ) return 0;
    s = *pLine;
    if( !*pLine || !*s ) return -1;
    StrLwr( *pLine );
    if( *s == ',' ){ s++; (*pLine)++; }

    p = s + strspn( s, " " );
    n1 = strcspn( p, "," );
    c1 = p[ n1 ];
    p[ n1 ] = '\0';
    n2 = strcspn( p, "(" );
    c2 = p[ n2 ];
    p[ n2 ] = '\0';
    str = &p[ n2 ] + 1;
    if( strstr( p, a2 ) == p ) flg = 1;
    p[ n2 ] = c2;
    p[ n1 ] = c1;
    if( !flg ) return -1;

    if( strstr( str, "rand(") == str ){
        str += 5;
        *pType = 3;
        *pOperator = 0;
        *pVal = strtol( str, NULL, 10 );
        if( ( s = strstr( str, ")" ) ) ) str = s + 1;
        if( ( s = strstr( str, ")" ) ) ) str = s + 1;
        *pLine = ( s = strstr(str, ",") ) ? (s + 1) : str; 
        return 0;
    }
    if( strstr( str, "global(") == str ){
        str += 7;
        *pType = 1;
        *pVal = strtol( str, NULL, 10 );
        if( (s = strstr( str, ")" )) ) str = s + 1;
        while( *str == ' ' ) str++;
        if( WmParseExprGetOperator( &str, pOperator ) == -1 ) return -1;
        *ArgRt = strtol( str, NULL, 10 );
        if( ( s = strstr( str, ")" ) ) ) str = s + 1;
        *pLine = ( s = strstr(str, ",") ) ? (s + 1) : str; 
        return 0;
    } else if( strstr( str, "player(level)" ) == str ){
        str += 13;
        *pType = 4;
        for( ; *str == ' '; str++ );
        if( WmParseExprGetOperator( &str, pOperator ) == -1 ) return -1;
        *ArgRt = strtol( str, NULL, 10 );
        if( ( s = strstr( str, ")" ) ) ) str = s + 1;
        *pLine = ( s = strstr(str, ",") ) ? (s + 1) : str; 
        return 0;
    } else if( strstr(str, "days_played") == str ){
        str += 11;
        *pType = 5;
        for( ; *str == ' '; str++ );
        if( WmParseExprGetOperator( &str, pOperator ) == -1 ) return -1;
        *ArgRt = strtol( str, NULL, 10 );
        if( ( s = strstr( str, ")" ) ) ) str = s + 1;
        *pLine = ( s = strstr(str, ",") ) ? (s + 1) : str; 
        return 0;                
    } else if( strstr( str, "time_of_day" ) == str ){
        str += 11;
        *pType = 6;
        for( ; *str == ' '; str++ );
        if( WmParseExprGetOperator( &str, pOperator ) == -1 ) return -1;
        *ArgRt = strtol( str, NULL, 10 );
        if( ( s = strstr( str, ")" ) ) ) str = s + 1;
        *pLine = ( s = strstr(str, ",") ) ? (s + 1) : str; 
        return 0;                
    } else {
        if( strstr( str, "enctr(num_critters)" ) != str ){ *pLine = str; return 0; }
        str += 19;
        *pType = 2;
        for( ; *str == ' '; str++ );
        if( WmParseExprGetOperator( &str, pOperator ) == -1 ) return -1;
        *ArgRt = strtol( str, NULL, 10 );
        if( (s = strstr(str, ")")) ) str = s + 1;
        *pLine = ( (s = strstr(str, ",")) ) ? (s + 1) : str;
        return 0;                
    }
}

int WmParseExprGetOperator( char **pLine, int *OpId )
{
    int i;

    *OpId = 0;
    for( i = 0; i < 5; i++ ){
	if( strstr( *pLine, gWmOperatorList[ i ] ) == *pLine ){
	    *OpId = i;
	    for( *pLine += strlen( gWmOperatorList[ i ] ); **pLine == ' '; (*pLine)++ );
	    return 0;	
	}
    }
    return -1;
}

int WmAreaInit()
{
    WmArea_t *pAreas, *Area;
    WmEntrance_t *Entrance;
    char stmp1[40], stmp2[40];
    Config_t Cfg;
    int ImgId, i;
    char *tmp;

    tmp = 0;
    if( WmMapInit() == -1 ) return -1;
    if( !CfgInit( &Cfg ) ) return -1;
    if( CfgLoadFromFile( &Cfg, "data/city.txt", 1 ) == 1 ){
        for( i = 0; i < 5000; i++ ){
            sprintf( stmp2, "Area %02d", i );
            if( !CfgGetInteger( &Cfg, stmp2, "townmap_art_idx", &ImgId ) ) break;
            gWmAreasCount++;
            pAreas = Realloc( gWmAreas, gWmAreasCount * sizeof( WmArea_t ) );
            if( !pAreas ){ WinMsgError( "\nwmConfigInit::Error loading areas!" ); exit( 1 ); }
            gWmAreas = pAreas;
            Area = &pAreas[ gWmAreasCount - 1 ];
            Area->Name[ 0 ] = '\0';
            Area->Id = -1;
            Area->WorldPosX = 0;
            Area->WorldPosY = 0;
            Area->Size = 2;
            Area->StartState = 0;
            Area->LockState[ 0 ] = 0;
            Area->LockState[ 1 ] = 0;
            Area->TnmapArtIdx = -1;
            Area->TnmapLblArtIdx = -1;
            Area->EntranceCnt = 0;
            Area->Id = i;
            if( ImgId != -1 ) ImgId = ArtMakeId(6, ImgId, 0, 0, 0);
            Area->TnmapArtIdx = ImgId;
            if( CfgGetInteger(&Cfg, stmp2, "townmap_label_art_idx", &ImgId) == 1 ){
                Area->TnmapLblArtIdx = ( ImgId == -1 ) ? -1 : ArtMakeId( 6, ImgId, 0, 0, 0 );
            }
            if( !CfgGetString( &Cfg, stmp2, "area_name", &tmp ) ){ WinMsgError( "\nwmConfigInit::Error loading areas!" ); exit( 1 ); }
            strcpy( Area->Name, tmp );
            if( !CfgGetString( &Cfg, stmp2, "world_pos", &tmp ) ){ WinMsgError( "\nwmConfigInit::Error loading areas!" ); exit( 1 ); }
            if( StrParseGetInt( &tmp, &Area->WorldPosX ) == -1 ) return -1;
            if( StrParseGetInt( &tmp, &Area->WorldPosY ) == -1 ) return -1;
            if( !CfgGetString( &Cfg, stmp2, "start_state", &tmp) ){ WinMsgError( "\nwmConfigInit::Error loading areas!" ); exit( 1 ); }
            if( StrParseFromList( &tmp, &Area->StartState, gWmOffOnList, 2 ) == -1 ) return -1; 
            if( CfgGetString( &Cfg, stmp2, "lock_state", &tmp ) == 1 ){
                if( StrParseFromList( &tmp, Area->LockState, gWmOffOnList, 2 ) == -1 ) return -1;
            }
            if( !CfgGetString( &Cfg, stmp2, "size", &tmp ) ){ WinMsgError( "\nwmConfigInit::Error loading areas!" ); exit( 1 ); }
            if( StrParseFromList( &tmp, &Area->Size, gWmSizeList, 3 ) == -1 ) return -1;
            while( Area->EntranceCnt < 10 ){
                sprintf( stmp1, "entrance_%d", Area->EntranceCnt );
                if( !CfgGetString( &Cfg, stmp2, stmp1, &tmp ) ) break;
                Entrance = &Area->Entrances[ Area->EntranceCnt ];
                Entrance->i01 = 0;
                Entrance->i02 = 0;
                Entrance->i03 = 0;
                Entrance->Id = -1;
                Entrance->i05 = 0;
                Entrance->i06 = 0;
                Entrance->i07 = 0;
                if( StrParseFromList( &tmp, &Entrance->i01, gWmOffOnList, 2 ) == -1 ) return -1;
                if( StrParseGetInt( &tmp, &Entrance->i02 ) == -1 ) return -1;
                if( StrParseGetInt( &tmp, &Entrance->i03 ) == -1 ) return -1;
                if( StrParseFromFunc( &tmp, &Entrance->Id, (void *)WmLookupMapByName ) == -1 ) return -1;
                if( StrParseGetInt( &tmp, &Entrance->i05 ) == -1 ) return -1;
                if( StrParseGetInt( &tmp, &Entrance->i06 ) == -1 ) return -1;
                if( StrParseGetInt( &tmp, &Entrance->i07 ) == -1 ) return -1;
                Area->EntranceCnt++;
            }
        }
    }
    CfgFree( &Cfg );
    if( gWmAreasCount != 49 ){ WinMsgError( "\nwmAreaInit::Error loading Cities!" ); exit( 1 ); }
    return 0;                
}

int WmLookupMapByName( int *Idx, char *Name )
{
    int i;

    for( i = 0; i < gWmMapCount; i++ ){
        if( !strcasecmp( Name, gWmMaps[ i ].LookupName ) ) break;
    }
    if( i < gWmMapCount ){
        *Idx = i;
        return 0;
    }
    eprintf( "\nWorldMap Error: Couldn't find match for Map Index!" );
    *Idx = -1;
    return -1;
}

int WmMapZero( Wmap_t *map )
{
    map->i11 = -1;
    map->i13 = -1;
    map->MapFileName[ 0 ] = '\0';
    map->BgMusicFname[ 0 ] = '\0';
    map->Flags = 0x3F;
    map->AmbientSfxCnt = 0;
    map->StartPointsCnt = '\0';
    map->LookupName[ 0 ] = '\0';
    map->MapFileName[ 0 ] = '\0';
    return 0;
}

int WmMapInit()
{
    int err, StartPointsCnt, a2, i, j;
    Wmap_t *map, *m;
    WmStartPoint_t *stp;
    Config_t cfg;
    char KeyName[40], stmp[40], *pValue;

    pValue = NULL;
    if( !CfgInit( &cfg ) ) return -1;
    if( CfgLoadFromFile( &cfg, "data/maps.txt", 1 ) == 1 ){
        for( i = 0; ; i++ ){
            sprintf( stmp, "Map %03d", i );
            if( !CfgGetString( &cfg, stmp, "lookup_name", &pValue ) ) break;
            gWmMapCount++;
            if( !(map = Realloc( gWmMaps, gWmMapCount * sizeof( Wmap_t ) )) ){ WinMsgError( "\nwmConfigInit::Error loading maps!" ); exit( 1 ); }
            gWmMaps = map;
            m = &map[ gWmMapCount - 1 ];
            WmMapZero( m );
            strncpy( m->LookupName, pValue, 40 );
            if( !CfgGetString( &cfg, stmp, "map_name", &pValue ) ){ WinMsgError( "\nwmConfigInit::Error loading maps!" ); return -1; }
            StrLwr( pValue );
            strncpy( m->MapFileName, pValue, 40 );
            if( CfgGetString( &cfg, stmp, "music", &pValue ) ) strncpy( m->BgMusicFname, pValue, 40 );
            if( CfgGetString( &cfg, stmp, "ambient_sfx", &pValue ) ){
                while( pValue ){
                    err = StrParseGetPrefixedStrInt( &pValue, m->AmbientSfx[ m->AmbientSfxCnt ].Name, &m->AmbientSfx[ m->AmbientSfxCnt ].Id, ":" );
                    if( err == -1 ) return -1;
                    m->AmbientSfxCnt++;
                    if( !*pValue ) pValue = NULL;
                    if( m->AmbientSfxCnt >= 6 ){
                        if( pValue ){
                            eprintf( "\n\nwmMapInit::Error reading ambient sfx.  Too many!  Str: %s, MapIdx: %d", pValue, i );
                            pValue = NULL;
                        }
                    }
                }
            }
            if( CfgGetString( &cfg, stmp, "saved", &pValue ) ){
                if( StrParseFromList( &pValue, &a2, gWmNoYesList, 2 ) == -1 ) return -1;
                if( a2 ) m->Flags |= 0x01; else m->Flags &= ~0x01;
            }
            if( CfgGetString( &cfg, stmp, "dead_bodies_age", &pValue ) ){
                if( StrParseFromList( &pValue, &a2, gWmNoYesList, 2 ) == -1 ) return -1;
                if( a2 ) m->Flags |= 0x02; else m->Flags &= ~0x02;
            }
            if( CfgGetString( &cfg, stmp, "can_rest_here", &pValue ) ){
                if( StrParseFromList( &pValue, &a2, gWmNoYesList, 2 ) == -1 ) return -1;
                if( a2 ) m->Flags |= 0x08; else m->Flags &= ~0x08;
                if( StrParseFromList( &pValue, &a2, gWmNoYesList, 2 ) == -1 ) return -1;
                if( a2 ) m->Flags |= 0x10; else m->Flags &= ~0x10;
                if( StrParseFromList( &pValue, &a2, gWmNoYesList, 2 ) == -1 ) return -1;
                if( a2 ) m->Flags |= 0x20; else m->Flags &= ~0x20;
            }
            if( CfgGetString( &cfg, stmp, "pipboy_active", &pValue ) ){
                if( StrParseFromList( &pValue, &a2, gWmNoYesList, 2 ) == -1 ) return -1;
                if( a2 ) m->Flags |= 0x04; else m->Flags &= ~0x04;
            }
            if( CfgGetString( &cfg, stmp, "random_start_point_0", &pValue ) ){
                j = 0;
                while( pValue ){
                    while( *pValue ){
                        StartPointsCnt = m->StartPointsCnt;
                        if( StartPointsCnt >= 15 ) break;
                        stp = &m->RandomStartPoints[ StartPointsCnt ];
                        stp->Elevation = 0;
                        stp->TileNum = -1;
                        stp->i03 = -1;
                        StrParseGetPrefixedInt( &pValue, "elev", &stp->Elevation, ":" );
                        StrParseGetPrefixedInt( &pValue, "tile_num", &stp->TileNum, ":" );
                        m->StartPointsCnt++;
                    }
                    sprintf( KeyName, "random_start_point_%1d", j++ );
                    if( !CfgGetString( &cfg, stmp, KeyName, &pValue ) ) pValue = NULL;
                }
            }
        }
    }
    CfgFree( &cfg );
    return 0;
}

int WmGetMapCount()
{
    return gWmMapCount;
}

int WmGetMapFileName( int Idx, char *FileName )
{
    if( Idx == -1 || Idx > gWmMapCount ) {
        *FileName = '\0';
        return -1;
    }
    sprintf( FileName, "%s.MAP", gWmMaps[ Idx ].MapFileName );
    return 0;
}

int WmGetMapIdxByFileName( char *FileName )
{
    char *s;
    int i;

    s = FileName;
    StrLwr( FileName );
    s = strchr( FileName, '.' );
    if( s ) *s = '\0';
    for( i = 0; i < gWmMapCount; i++ ){
        if( !strcmp( gWmMaps[ i ].MapFileName, FileName ) ) break;
    }    
    if( s ) *s = '.';
    if( i >= gWmMapCount ) return -1;
    return i;    
}

int WmIsMapSaved( int Idx )
{
    return gWmMaps[ Idx ].Flags & 0x01;
}

int WmIsCurrentMapMapSaved()
{
    return gWmMaps[ gMapCurrentMapId ].Flags & 0x01;
}

int WmIsCurrentMapDeadBodyAge()
{
    return gWmMaps[ gMapCurrentMapId ].Flags & 0x02;
}

int WmCanRestHere( int Place )
{
    const int mask[ 3 ] = { 0x08, 0x10, 0x20 };

    return mask[ Place ] & gWmMaps[ gMapCurrentMapId ].Flags;
}

int WmPipBoyEquipped()
{
    return GMovieEnabled( 3 );
}

void WmUnk41( int MapIdx )
{
    int AreaId;

    if( MapIdx >= 0 && MapIdx < gWmMapCount && ( gWmMaps[ MapIdx ].Flags & 0x01 ) != 0 ){
        if( WmFindAreaByEntranceId( MapIdx, &AreaId ) != -1 ) WmUnk04( AreaId, 2 );
    }
}

int WmGetEntranceIdx( int AreaId, int EntranceId, int *pEntranceIdx )
{
    int i;

    for( i = 0; i < gWmAreas[ AreaId ].EntranceCnt && (EntranceId != gWmAreas[ AreaId ].Entrances[ i ].Id); i++ );

    if( i >= gWmAreas[ AreaId ].EntranceCnt ){
        *pEntranceIdx = -1;
	return -1;
    }
    *pEntranceIdx = i;
    return 0;
}

int WmUnk43( int AreaId, int EntranceId, int Unk, int *pIdx )
{
    int n, i;

    for( i = 0; i < gWmAreas[ AreaId ].EntranceCnt; i++ ){
        if( EntranceId == gWmAreas[ AreaId ].Entrances[ i ].Id ){
            if( Unk == -1 ) break;
            n = gWmAreas[ AreaId ].Entrances[ i ].i05;
            if( n == -1 || Unk == n ) break;
        }
    }
    if( i >= gWmAreas[ AreaId ].EntranceCnt ){
        *pIdx = -1;
        return -1;
    }
    *pIdx = i;
    return 0;
}

int WmFindAreaByEntrance( int EntranceId, int *pIdx )
{
    int j, i;
    
    for( i = 0; i < gWmAreasCount; i++ ){
        for( j = 0; (j < gWmAreas[ i ].EntranceCnt) && (EntranceId != gWmAreas[ i ].Entrances[ j ].Id); j++ );
        if( EntranceId == gWmAreas[ i ].Entrances[ j ].Id ) break;        
    }
    if( i >= gWmAreasCount || j >= gWmAreas[ i ].EntranceCnt ){
        *pIdx = -1;
	return -1;
    }
    *pIdx = i;
    return 0;
}

int WmUnk45( int MapId, int a2, int Val )
{
    int v5;
    int AreaId = 0;

    if( MapId < 0 || MapId >= gWmMapCount ) return -1;
    if( !( gWmMaps[ MapId ].Flags & 0x01 ) ) return 0;
    if( WmFindAreaByEntranceId( MapId, &AreaId ) == -1 ) return -1;
    if( WmUnk43( AreaId, MapId, a2, &v5 ) == -1 ) return -1;
    if( ( gWmMaps[ MapId ].Flags & 0x01) ) gWmAreas[ AreaId ].Entrances[ v5 ].i01 = Val;
    return 0;
}


int WmMenu()
{
    return WmMenu1( 0 );
}

int WmMenu1( int UnusedArg )
{
    WmArea_t *v18,*area;
    unsigned int SysTime, Sel;
    int AreaMseX,AreaMseY,MseBt,v19,v20,EntranceId,MseY,MseX,v28, Time,sz;

    MseX = MseY = 0;
    v28 = 0;
    EntranceId = -1;
    Time = 0;
    if( WmMenuCreate() == -1 ){ WmMenuDestroy(); return -1; }
    WmGetArea( gWmWorldPosX, gWmWorldPosY, &gWmAreaId );
    while( EntranceId == -1 && v28 != -1 ){
        Sel = InpUpdate();
        SysTime = TimerGetSysTime();
        MseGetCursorPosition( &MseX, &MseY );
        AreaMseX = gWmMapOffsetX + MseX - 22;
        AreaMseY = MseY + gWmMapOffsetY - 21;
        if( Sel == 17 || Sel == 24 || Sel == 324 ) SysQuitDlg();
        ScptPlayMovieEv( NULL, gWmMenuWin );
        if( gMenuEscape ) break;
        MseBt = MseGetButtons();
        if( gWmUnk13 == 1 ){  
    	    WmMoveStep(); 
    	    if( gWmUnk13 == gWmTravelByCar ){ // car travel
                WmMoveStep(); WmMoveStep(); WmMoveStep();
                if( GlobVarGet( 439 ) ) WmMoveStep();
                if( GlobVarGet( 455 ) ) WmMoveStep();
                if( GlobVarGet( 456 ) ){ WmMoveStep(); WmMoveStep(); WmMoveStep(); }
                gWmCarFrameNo++;
                if( ArtGetFpd( gWmCarImg ) <= gWmCarFrameNo ) gWmCarFrameNo = 0;
                WmUnk86( 100 );
                if( gWmGasoline <= 0 ){
                    gWmUnk11 = gWmUnk12 = gWmUnk13 = 0;
                    WmGetArea( AreaMseX, AreaMseY, &gWmAreaId );
                    gWmTravelByCar = 0;
                    if( gWmAreaId == -1 ){
                        gWmCurrentArea = 21;
                        sz = 5 * gWmAreas[ 21 ].Size;
                        WmSetPos( 21, gWmWorldPosX + gWmUnk44 / 2 + gWmUnk94[ sz ] / 2, gWmWorldPosY + gWmUnk94[ sz + 1 ] / 2 + gWmUnk45 / 2 );
                        gWmAreas[ 21 ].StartState = 1;
                        gWmAreas[ 21 ].LockState[ 1 ] = 1;
                        gWmAreaId = 21;
                    } else {
                        gWmCurrentArea = gWmAreaId;
                    }
                    eprintf( "\nRan outta gas!" );
                }
    	    }  
            WmDrawMapScreen();
            if( TimerDiff( SysTime, Time ) > 1000 && PartyHeal( 3 ) ){ IfaceRenderHP( 0 ); Time = SysTime; }
            WmUnk69( gWmWorldPosX, gWmWorldPosY );
            if( gWmSteps <= 0 ){ // stop
        	gWmUnk13 = 0; 
        	WmGetArea( gWmWorldPosX, gWmWorldPosY, &gWmAreaId ); 
    	    }
            WmDrawMapScreen();
            if( WmWait( 18000 ) && gMenuEscape ) break;
            if( gWmUnk13 == 1 ){
                if( WmUnk49() ){
                    if( gWmRandMapId != -1 ){
                        if( gWmUnk13 == gWmTravelByCar ) WmFindAreaByEntranceId( gWmRandMapId, &gWmCurrentArea );
                        MapOpenById( gWmRandMapId );
                    }
                    break;
                }
            }
        }
        if( (MseBt & 0x01) && !(MseBt & 0x04) ){
    	    if( !MseCursorCenterInArea( 22, 21, 472, 465 ) ) continue;
    	    // click on map area
    	    if( !gWmUnk13 && !gWmGoToLocation && abs32( gWmWorldPosX - AreaMseX ) < 5 && abs32( gWmWorldPosY - AreaMseY ) < 5 ){
            	gWmGoToLocation = 1;
            	WmDrawMapScreen();
    	    }
        }
        if( MseBt & 0x10 ){
    	    if( gWmGoToLocation == 1 ){ // go to location
                gWmGoToLocation = 0;
                WmDrawMapScreen();
                if( abs32( gWmWorldPosX - AreaMseX ) >= 5 || abs32( gWmWorldPosY - AreaMseY ) >= 5 ) continue;
                if( gWmAreaId == -1 ){
                    EntranceId = 0; // terrain location
                } else {
                    v18 = &gWmAreas[ gWmAreaId ];
                    if( ( v18->LockState[1] == 2 ) && ( v18->TnmapArtIdx != -1 ) ){
                    	if( WmTownMap( &EntranceId ) == -1 ){ v28 = -1; break; }
                    } else {
                    	if( WmGetAssignId( &EntranceId ) == -1 ){ v28 = -1; break; }
                    	gWmAreas[ gWmAreaId ].LockState[ 1 ] = 2;
                    }
                }
                if( EntranceId != -1 ){
                    if( gWmTravelByCar == 1 ){
                    	gWmTravelByCar = 0;
                        if( gWmAreaId == -1 )
                            WmFindAreaByEntranceId( EntranceId, &gWmCurrentArea );
                        else
                            gWmCurrentArea = gWmAreaId;
                    }
                    MapOpenById( EntranceId );
                    break;
                }                
	        continue;
    	    }
            if( MseCursorCenterInArea(22, 21, 472, 465) ) WmMapTravel( AreaMseX, AreaMseY );
            gWmGoToLocation = 0;
	}
        if( gWmUnk65 ){
            gWmLocListStartLine += gWmUnk65;
            WmDrawIface( 1 );
            if( gWmUnk65 > -1 ){
                if( gWmUnk64 <= gWmLocListStartLine )
                WmRedrawWidgets();
            } else {
        	if( gWmUnk64 >= gWmLocListStartLine ) WmRedrawWidgets();
            }
        }
    	switch( Sel ){
    	    case -1: break;
    	    case 350 ... 356: // buttons on location list        	
                v19 = gWmLocListStartLine / 27 + Sel - 350;
                if( v19 < gWmKnownLocationsCount ){
                    v20 = gWmKnownLocations[ v19 ];
                    area = &gWmAreas[ v20 ];
                    if( WmIsAreaKnown( area->Id ) ){
                        if( v20 != gWmAreaId ){
                            WmMapTravel( area->WorldPosX, area->WorldPosY );
                            gWmGoToLocation = 0;
                        }
                    }
                }
		break;
        case 331: WmMoveMap( 20, 20, 0, -1, 0, 1 ); break;
        case 333: WmMoveMap( 20, 20, 0, 1, 0, 1 ); break;
        case 336: WmMoveMap( 20, 20, 1, 0, 0, 1 ); break;
             // list of known locations arrows buttons
        case 397: WmListScrollUpDn( -27 ); break;
        case 401: WmListScrollUpDn( 27 ); break;
        case 327: WmUnk48(); break;
        case 328: WmMoveMap( 20, 20, -1, 0, 0, 1 ); break;
        case 'T': case 't': // Town/World
    	    if( gWmUnk13 || gWmAreaId == -1 ) break;
            area = &gWmAreas[ gWmAreaId ];
            if( area->LockState[ 1 ] != 2 || area->TnmapArtIdx == -1 ) break;
            if( WmTownMap( &EntranceId ) == -1 ) v28 = -1;
            if( EntranceId != -1 ){
                if( gWmTravelByCar == 1 ) WmFindAreaByEntranceId( EntranceId, &gWmCurrentArea );
                MapOpenById( EntranceId );
            }            
	    break;        
        }
    }
    if( WmMenuDestroy() == -1 ) return -1;
    return v28;    
}

int WmUnk47()
{
    if( gWmAreaId == 45 ){
        if( gWmAreaId < gWmAreasCount ) gWmAreas[45].StartState = 0;
        if( gWmAreasCount > 46 ) gWmAreas[46].StartState = 1;
        WmUnk04(46, 2);
    }
    return 0;
}

int WmUnk48()
{
    int x, y;

    x = gWmWorldPosX - 203;
    if( x < 0 ) x = 0;    
    if( x > gWmUnk16 ) x = gWmUnk16;
    
    y = gWmWorldPosY - 200;
    if( y < 0 ) y = 0;    
    if( y > gWmUnk17 ) y = gWmUnk17;
    
    gWmMapOffsetX = x;
    gWmMapOffsetY = y;
    WmDrawMapScreen();
    return 0;
}

int WmUnk49()
{
    MsgLine_t v17, Line;
    Obj_t *ob;
    char *Message, v16[120], *eax0, *Str2;
    int SysTime,v2,GameTime,v5,v6,v7,v8,i,v12,v13,v14,pAreaId,EntryValue,v24,v25,v26,zz;

    v25 = 0;
    EntryValue = 1;
    SysTime = TimerGetSysTime();
    if( TimerDiff( SysTime, gWmTimeStamp ) < 1500 ) return 0;
    gWmTimeStamp = SysTime;
    if( abs32( gWmUnk36 - gWmWorldPosX ) < 3 ) return 0;
    if( abs32( gWmUnk37 - gWmWorldPosY ) < 3 ) return 0;
    WmGetArea( gWmWorldPosX, gWmWorldPosY, &pAreaId );
    if( pAreaId != -1 ) return 0;
    if( !gWmUnk23 ){
        v2 = ScptGetGameDekaSeconds();
        if( (v2 / 864000) > 35 ){
            gWmRandMapId = -1;
            gWmUnk23 = 1;
            if( gWmTravelByCar == 1 ) WmFindAreaByEntranceId( 149, &gWmCurrentArea );
            MapOpenById( 149 );
            return 1;
        }
    }
    WmGetTile( gWmWorldPosX, gWmWorldPosY, &gWmTile );
    GameTime = ScptGetGameTime();
    if( GameTime >= 1800 ){
        v26 = 2;
    } else {
	if( GameTime < 1200 ){
    	    if( GameTime > 600 ){
        	v26 = 0;
    	    } else {
    		v26 = 2;
	    }
	} else {
	    v26 = 1;
	}
    }
    v5 = gWmFreqValues[ gWmTile->Freq[ v26 ] ];
    if( v5 > 0 && v5 < 100 && CfgGetInteger( &gConfiguration, "preferences", "game_difficulty", &EntryValue ) == 1 ){
        v6 = v5 / 15;
        if( EntryValue ){
             if( EntryValue > 1 && EntryValue == 2 ) v5 += v6;
        } else {
            v5 -= v6;
        }
    }
    v24 = RandMinMax( 0, 100 );
    if( v24 >= v5 ) return 0;
    WmDrawEncounter();
    v7 = 1;
    gWmUnk31 = 1;
    gWmUnk35 = 0;
    if( gWmEncounterTable[ gWmEncounterType ].TabIdx[ gWmUnk34 ].Flags & 0x01 ){
        gWmUnk35 = 2;
        WmFindAreaByEntranceId( gWmRandMapId, &pAreaId );
        v8 = 5 * gWmAreas[ pAreaId ].Size;
        WmSetPos( pAreaId, gWmWorldPosX + gWmUnk44 / 2 + gWmUnk94[ v8 ] / 2, gWmWorldPosY + gWmUnk45 / 2 + gWmUnk94[ v8 + 1] / 2 );
        v7 = 3;
        if( pAreaId >= 0 && pAreaId < gWmAreasCount ){
            if ( gWmAreas[ pAreaId ].LockState[0] != 1 ) gWmAreas[ pAreaId ].StartState = 1;
        }
    }    
    for( i = 0; i < 7; i++ ){
        gWmUnk35 = v7 - gWmUnk35;
        if( WmDrawMapScreen() == -1 ) return -1;
        TimerWait( 200 );
    }
    if( gWmTravelByCar == 1 ){
	const int tt[ 3 ] = { 40, 30, 0 };
        v5 -= tt[ v26 ];
    }
    if( v5 > v24 ){
        v13 = PartyGetBestSkillLvl( SKILL_OUTDOORSMAN );
        if( ( ob = InvGetItem( gObjDude, PID_MOTIONSENSOR ) ) ){
    	    if( gObjDude == ob->Owner ) v13 += 20;
        }
	if( v13 > 95 ) v13 = 95;
        zz = gWmWorldPosY / 300 * gWmHorizTileCnt + gWmWorldPosX / 350 % gWmHorizTileCnt;
        eprintf( "\nEncounter Difficulty Mod: %d", gWmTiles[ zz ].Difficulty );
        v12 = RandMinMax( 1, 100 );
        v13 = gWmTiles[ zz ].Difficulty + v13;
        if( v12 < v13 ){
            v25 = 1;
            if( v13 < 100 ){
                v14 = 100 - v13;
                if( 100 - v13 > 0 ){
                    Message = MessageGetMessage( &gMessage, &Line, 8500 );
                    if( strlen( Message ) >= 110 ){
                        eprintf( "WorldMap: Error: Rnd Encounter string too long!" );
                    } else {
                        sprintf( v16, Message, v14 );
                        IfcMsgOut( v16 );
                    }
                    eprintf( "WorldMap: Giving Player [%d] Experience For Catching Rnd Encounter!", v14 );
                    if( v14 < 100 ) FeatLvlUp( v14 );
                }
            }
        }
    } else {
        v25 = 1;
    }
    gWmUnk36 = gWmWorldPosX;
    gWmUnk37 = gWmWorldPosY;
    if( v25 ){
        eax0 = "You detect something up ahead.";
        Str2 = "Do you wish to encounter it?";
        eax0 = MessageGetMessage( &gWmMsg, &v17, 2999 );
        Str2 = MessageGetMessage( &gWmMsg, &v17, 50 * gWmEncounterType + 3000 + gWmUnk34 );
        if( !DlgBox( eax0, &Str2, 1, 169, 116, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 17 ) ){
            gWmUnk31 = 0;
            gWmRandMapId = -1;
            gWmEncounterType = -1;
            gWmUnk34 = -1;
            return 0;
        }
    }
    return 1;
}

int WmGetCurTile()
{
    return WmGetTile( gWmWorldPosX, gWmWorldPosY, &gWmTile );
}

int WmGetTile( int x, int y, WmTileDsc_t **pTile )
{
    *pTile = &gWmTiles[ y / 300 * gWmHorizTileCnt + x / 350 % gWmHorizTileCnt ].Dsc[ y % 300 / 50 ][ x % 350 / 50 ];
    return 0;
}

int WmDrawEncounter()
{
    WmEnctr_t *Enc;
    int tmp, chance, i, EncList[41], difficulty, MaxChance, EncListNum;

    difficulty = 1;
    if( !gWmTile ) WmGetTile( gWmWorldPosX, gWmWorldPosY, &gWmTile );

    gWmEncounterType = gWmTile->EncounterType;
    Enc = &gWmEncounterTable[ gWmEncounterType ];

    EncListNum = MaxChance = 0;
    for( i = 0; i < Enc->TabIdxCnt; i++ ){
        tmp = WmCompare( &Enc->TabIdx[ i ].Expr, 0 ) != 0;
        if( Enc->TabIdx[ i ].Counter == 0 ) continue;
        if( tmp ){
            EncList[ EncListNum++ ] = i;
            MaxChance += Enc->TabIdx[ i ].Chance;
        }            
    }

    chance = RandMinMax( 0, MaxChance ) + FeatGetVal( gObjDude, FEAT_LUCK ) - 5;
    if( PerkLvl( gObjDude, PERK_EXPLORER ) > 0 ) chance += 2;
    if( PerkLvl( gObjDude, PERK_RANGER ) > 0 ) chance++;
    if( PerkLvl( gObjDude, PERK_SCOUT ) > 0 ) chance++;
    if( CfgGetInteger( &gConfiguration, "preferences", "game_difficulty", &difficulty ) == 1 ){
        if( difficulty ){
            if( difficulty > 1 && difficulty == 2 ){
                chance -= 5;
                if( chance < 0 ) chance = 0;
            }
        } else {
            chance += 5;
            if( chance > MaxChance ) chance = MaxChance;
        }
    }

    for( i = 0; i < EncListNum; i++ ){
        tmp = Enc->TabIdx[ EncList[ i ] ].Chance;
        if( chance < tmp ) break;
        chance -= tmp;
    }
    if( i == EncListNum ) i = EncListNum - 1;
    gWmUnk34 = EncList[ i ];

    if( Enc->TabIdx[ gWmUnk34 ].Counter > 0 ) Enc->TabIdx[ gWmUnk34 ].Counter--;
    if( Enc->TabIdx[ gWmUnk34 ].MapId == -1 ){
        if( Enc->IdxCnt <= 0 ){
            gWmRandMapId = gWmTerrainTypes[ gWmTile->TerrainType ].MapName[ RandMinMax( 0, gWmTerrainTypes[ gWmTile->TerrainType ].MapId - 1 ) ];
        } else {
            gWmRandMapId = Enc->MapIdxPool[ RandMinMax( 0, Enc->IdxCnt - 1 ) ];
        }
    } else {
        gWmRandMapId = Enc->TabIdx[ gWmUnk34 ].MapId;
    }
    return 0;
}

int WmSetupRandomEncounter()
{
    Obj_t *obj2, *a1, *obj1;
    WmIdx_t *enc;
    int i, RandMin, difficulty;    
    Scpt01_t scr2, scr1;
    MsgLine_t Line;

    obj2 = 0;
    difficulty = 1;
    a1 = NULL;
    if( gWmRandMapId == -1 ) return 0;    
    enc = &gWmEncounterTable[ gWmEncounterType ].TabIdx[ gWmUnk34 ];
    IfcMsgOut( MessageGetMessage( &gWmMsg, &Line, 2998 ) );
    IfcMsgOut( MessageGetMessage( &gWmMsg, &Line, 50 * gWmEncounterType + 3000 + gWmUnk34 ) );
    switch( enc->SceneryType ){
	case 0 ... 3:
            eprintf( "\nwmSetupRandomEncounter: Scenery Type: %s", gWmWeightList[ enc->SceneryType ] );
            CfgGetInteger( &gConfiguration, "preferences", "game_difficulty", &difficulty );
            if( enc->EncCnt <= 0 ) return 0;
            break;
        default:
            eprintf( "\nERROR: wmSetupRandomEncounter: invalid Scenery Type!" );
            return -1;
    }
    for( i = 0; i < enc->EncCnt; i++/*, enc += 16*/ ){
            RandMin = RandMinMax( enc->Enc[ i ].RandMin, enc->Enc[ i ].RandMax);
            if( difficulty ){
                if( difficulty > 1 && difficulty == 2 ) RandMin += 2;
            } else {
                RandMin -= 2;
                if( RandMin - 2 < enc->Enc[ i ].RandMin ) RandMin = enc->Enc[ i ].RandMin;
            }
            if( PartyUnk12() > 2 ) RandMin += 2;
            if( !RandMin ) continue;
            if( WmSetupCritter( enc->Enc[ i ].i03, &a1, RandMin ) == -1 ){ ScptWorldMap(); return -1; }
            if( i > 0 ){
                if( obj2 ){
                    if( obj2 != a1 ){
                        if( enc->EncCnt != 1 ){
                            if( enc->EncCnt == 2 && !IN_COMBAT ){
                        	obj2->Critter.State.WhoHitMeObj = a1;
                        	a1->Critter.State.WhoHitMeObj = obj2;
                        	scr1.obj = obj2;
                        	scr1.i07 = 500;
                        	memset( &scr1.i03, 0, 16 );
                        	scr1.i08 = 0;
                        	scr1.crit = a1;
                        	AiUnk22( a1, obj2 );
                        	ScptUnk120( &scr1 );
                            }
                        } else {                        
                    	    if( !IN_COMBAT ){
                        	obj1 = gObjDude;
                        	obj2->Critter.State.WhoHitMeObj = gObjDude;
                        	scr2.obj = obj2;
                        	memset( &scr2.i03, 0, 16 );
                        	scr2.i07 = 500;
                        	scr2.i08 = 0;
                        	scr2.crit = obj1;
                        	AiUnk22( obj1, obj2 );
                        	ScptUnk120( &scr2 );
                    	    }
                        }
                    }
                }
        }
        obj2 = a1;
    }
    return 0;
}

int WmSetupCritter( int eax0, Obj_t **edx0, int a3 )
{
    int GridPos;
    int k,v12,i,TileNum,j;
    WmTeam_t *v5;
    WmPlayer_t *player;
    Obj_t *a1, *pObj;

    if ( eax0 == -1 ) return 0;
    *edx0 = 0;
    player = &gWmPlayers[ eax0 ];
    eprintf( "\nwmSetupCritterObjs: typeIdx: %d, Formation: %s", eax0, gWmPositionsList[ player->Position ] );
    if( WmSetupCritterObjs(player) == -1 ) return -1;
    for( i = 0; i < player->TeamCnt; i++ ){
        v5 = &player->Team[ i ];
        if( player->Team[ i ].Pid != -1 && WmCompare( &player->Team[ i ].Expr, &a3 ) ){
            if( v5->RatioFlg ){
                if( v5->RatioFlg == 1 ) TileNum = 1;
            } else {
                TileNum = v5->Ratio * a3 / 100;
            }
            if( TileNum < 1 ) TileNum = 1;
            for( j = 0; j < TileNum; j++ ){
                if( WmSetupRndNextTileNum( player, v5, &GridPos, TileNum ) == -1 ){
                    eprintf( "\nERROR: wmSetupCritterObjs: wmSetupRndNextTileNum:" );
                } else if( v5->Pid != -1 ){
                    if( ObjCopy( &pObj, v5->Pid ) == -1 ) return -1;
                    if( !*edx0 && OBJTYPE( v5->Pid ) == TYPE_CRIT ) *edx0 = pObj;
                    if( v5->TeamNum != -1 && OBJTYPE( pObj->Pid ) == TYPE_CRIT ) pObj->Critter.State.GroupId = v5->TeamNum;
                    if( v5->ScriptId != -1 ){
                        if( pObj->ScrId != -1 ){ ScptRemove( pObj->ScrId ); pObj->ScrId = -1; }
                        UseUnk03( pObj, 4, v5->ScriptId - 1 );
                    }
                    if( player->Position )
                        ObjMoveToTile( pObj, GridPos, gCurrentMapLvl, 0 );
                    else
                        UseUnk46( pObj, GridPos, 0, 0 );
                    ObjSetRotation( pObj, TileTurnAt( GridPos, gObjDude->GridId ), 0 );
                    for( k = 0; k < v5->ItemsCnt; k++ ){
                        v12 = ( v5->Items[ k ].i03 == v5->Items[ k ].i02 ) ? v5->Items[ k ].i03 : RandMinMax( v5->Items[ k ].i02, v5->Items[ k ].i03 );
                        if( !v12 ) continue;
                        if( ObjCopy( &a1, v5->Items[ k ].i01 ) == -1 ) return -1;
                        if( v5->Items[ k ].i01 == PID_MONEY && PerkLvl( gObjDude, PERK_FORTUNE_FINDER ) > 0 ) v12 *= 2;
                        if( ItemAdd( pObj, a1, v12 ) == -1 ) return -1;
                        ObjLightItem( a1, 0 );
                        if( v5->Items[ k ].Used && InvUnk28( pObj, a1, 1 ) == -1 ){
                            eprintf( "\nERROR: wmSetupCritterObjs: Inven Wield Failed: %d on %s: Critter Fid: %d", a1->Pid, CritterGetName( pObj ), pObj->ImgId );
                        }
                    }
                    if( (v5->Flags & 1) != 0 && OBJTYPE( pObj->Pid ) == TYPE_CRIT ) CritterKill( pObj, -1, 0 );
                }                        
            }
        }                
    }
    return 0;
}

int WmSetupCritterObjs( WmPlayer_t *player )
{
    int i, StartPointsCnt;

    for( i = 0; i < 2; i++ ){
        gWmUnk105[ i ] = 0;
        gWmUnk109[ i ] = 0;
        gWmMapDst[ i ] = -1;
        gWmUnk103[ i ] = (i & 1) ? 5 : 1;
    }
    gWmUnk129 = 0;
    switch( player->Position ){
        case 0:
            gWmMapDst[ 0 ] = gObjDude->GridId;
            gWmUnk109[ 0 ] = RandMinMax( 0, 5 );
            gWmMapStart = gWmMapDst[ 0 ];
            return 0;
        case 1 ... 5:
            StartPointsCnt = gWmMaps[ gMap.MapId ].StartPointsCnt;
            if( StartPointsCnt ){
                i = RandMinMax( 0, StartPointsCnt - 1 );
                gWmMapDst[ 0 ] = gWmMaps[ gMap.MapId ].RandomStartPoints[ i ].TileNum;
                gWmMapDst[ 1 ] = gWmMapDst[ 0 ];
                gWmUnk105[ 0 ] = gWmMaps[ gMap.MapId ].RandomStartPoints[ i ].i03;
                gWmUnk105[ 1 ] = gWmUnk105[ 0 ];
            } else {
                gWmUnk105[ 0 ] = 0;
                gWmMapDst[ 0 ] = gObjDude->GridId;
                gWmMapDst[ 1 ] = gObjDude->GridId;
                gWmUnk105[ 1 ] = 0;
            }
            gWmUnk109[ 0 ] = TileTurnAt( gWmMapDst[ 0 ], gObjDude->GridId );
            gWmUnk109[ 1 ] = TileTurnAt( gWmMapDst[ 1 ], gObjDude->GridId );
            gWmMapStart = gWmMapDst[ 0 ];
            return 0;
    }
    eprintf( "\nERROR: wmSetupCritterObjs: invalid Formation Type!" );
    return -1;
}

int WmSetupRndNextTileNum( WmPlayer_t *player, WmTeam_t *team, int *pTileNum, int TileNum )
{
    int Distance,v11,v12,i;
    
    for( i = 0; i <= 25; i++ ){
        switch( player->Position ){
            case 0:
                if( team->Distance ){
                    Distance = team->Distance;
                } else {
                    Distance = RandMinMax( -2, 2 ) + FeatGetVal( gObjDude, FEAT_PERCEPTION );
                    if( PerkLvl( gObjDude, PERK_CAUTIOUS_NATURE ) ) Distance += 3;
                }
                if( Distance < 0 ) Distance = 0;
                if( team->TileNum == -1 ) TileGetTileNumInDir( gObjDude->GridId, gWmUnk109[0], Distance );
                if( ++gWmUnk109[0] >= 6 ) gWmUnk109[0] = 0;
                TileNum = TileGetTileNumInDir( gObjDude->GridId, (RandMinMax( 0, 5 ) + gWmUnk109[0]) % 6, RandMinMax( 0, Distance / 2 ) );
                break;
            case 1: case 2:
                TileNum = gWmMapDst[ gWmUnk128 ];
                if( gWmUnk129 ){
                    v11 = (gWmUnk103[ gWmUnk128 ] + gWmUnk109[ gWmUnk128 ] ) % 6;
                    v12 = TileGetTileNumInDir( gWmMapDst[ gWmUnk128 ], v11, player->Spacing );
                    TileNum = gWmMapDst[ gWmUnk128 ] = TileGetTileNumInDir( v12, (v11 + gWmUnk103[gWmUnk128]) % 6, player->Spacing );
                    gWmUnk128 = 1 - gWmUnk128;
                }
                break;
            case 3:
                TileNum = gWmMapDst[ gWmUnk128 ];
                if( gWmUnk129 ){
                    TileNum = TileGetTileNumInDir(gWmMapDst[gWmUnk128], (gWmUnk103[gWmUnk128] + gWmUnk109[gWmUnk128]) % 6, player->Spacing);
                    gWmMapDst[ gWmUnk128 ] = TileNum;
                    gWmUnk128 = 1 - gWmUnk128;
                }
                break;
            case 4:
                TileNum = gWmMapDst[ gWmUnk128 ];
                if( gWmUnk129 ){
                    TileNum = TileGetTileNumInDir( gWmMapDst[ gWmUnk128 ], (gWmUnk109[ gWmUnk128 ] + 3 + gWmUnk103[ gWmUnk128 ] ) % 6, player->Spacing );
                    gWmMapDst[ gWmUnk128 ] = TileNum;
                    gWmUnk128 = 1 - gWmUnk128;
                }
                break;
            case 5:
                TileNum = gWmMapDst[ 0 ];
                if( gWmUnk129 ){
                    gWmUnk109[ 0 ] = (gWmUnk109[ 0 ] + 1) % 6;
                    TileNum = gWmMapDst[ 0 ] = TileGetTileNumInDir( gWmMapDst[ 0 ], gWmUnk109[ 0 ], player->Spacing );
                }
                break;
            default:
                break;
        }        
        gWmUnk129++;
        if( WmUnk53( TileNum ) ){
	    eprintf( "\nwmSetupRndNextTileNum:TileNum: %d", TileNum );
	    *pTileNum = TileNum;
	    return 0;
        }
        eprintf( "\nWARNING: EVAL-TILE-NUM FAILED!" );
        if( TileGetDistance( gWmMapStart, gWmMapDst[ gWmUnk128 ] ) > 25 ) return -1;        
    }
    return -1;
}

int WmUnk53( int TileNum )
{
    return !ObjReach( gObjDude, TileNum, gCurrentMapLvl ) && AnimMakeTrace( gObjDude, gObjDude->GridId, TileNum, 0, 0, (void *)ObjUnk55 ) != 0;
}

int WmCompare( WmExpr_t *expr, int *var )
{
    int ret, i;

    ret = 1;    
    for( i = 0; i < expr->Args; i++ ){
        ret = 1;
        switch( expr[ i ].Cond->ArgType ){
            case 1:
                if( !WmScrCmp( GlobVarGet( expr[ i ].Cond->Arg1 ), expr[ i ].Cond->Operator, expr[ i ].Cond->Arg2 ) ) ret = 0;
                break;
            case 2:
                if( !WmScrCmp( *var, expr[ i ].Cond->Operator, expr[ i ].Cond->Arg2 ) ) ret = 0;
                break;
            case 3:
                if( RandMinMax( 0, 100 ) > expr[ i ].Cond->Arg1 ) ret = 0;
                break;
            case 4:
                if( !WmScrCmp( FeatGetPoints( 1 ), expr[ i ].Cond->Operator, expr[ i ].Cond->Arg2 ) ) ret = 0;
                break;
            case 5:
                if( !WmScrCmp( ScptGetGameDekaSeconds() / 864000, expr[ i ].Cond->Operator, expr[ i ].Cond->Arg2 ) ) ret = 0;
                break;
            case 6:
                if( !WmScrCmp( ScptGetGameTime() / 100, expr[ i ].Cond->Operator, expr[ i ].Cond->Arg2 ) ) ret = 0;
                break;
        }
        if( !ret && expr[ i ].i02[0] == 1 ) break;
    }        
    return ret;
}

int WmScrCmp( int Arg1, int Operator, int Arg2 )
{
    switch( Operator ){
        case 1: return ( Arg1 == Arg2 ) ? 1 : 0;
        case 2: return ( Arg1 != Arg2 ) ? 1 : 0;
        case 3: return ( Arg1 < Arg2 ) ? 1 : 0;
        case 4: return ( Arg1 > Arg2 ) ? 1 : 0;
    }
    return 0;
}

int WmWait( int a1 )
{
    unsigned int Time, ds, t;

    while( a1 ){
        t = ScptGetGameDekaSeconds();
        Time = EvQeGetTime();
        ds = Time >= (t + a1) ? a1 : (Time - t);
        a1 -= ds;
        ScptTimeAdvance( ds );
        t = ((ScptGetGameTime() / 100) + 12) % ArtGetFpd( gWmDialImg );
        if( t != gWmDialFrame ){
            gWmDialFrame = t;
            WmDrawDial( 1 );
        }
        WmCalendar( 1 );
        if( EvQeTrig() ) return 1;
    }
    return 0;    
}

int WmLoadWorldMask( int Idx )
{
    int err;
    xFile_t *fh;
    char stmp[ 268 ];

    stmp[ 264 ] = '\0';
    err = 0;
    if( gWmTiles[ Idx ].WorldMask ) return 0;
    if( !gWmTiles[ Idx ].WalkMaskName[ 0 ] ) return 0;    
    if( !(gWmTiles[ Idx ].WorldMask = Malloc( 13200 )) ) return -1;
    sprintf( stmp, "data/%s.msk", gWmTiles[ Idx ].WalkMaskName );    
    if( !(fh = dbOpen(stmp, "rb")) ) return -1;
    if( dbreadByteBlk( fh, gWmTiles[ Idx ].WorldMask, 13200 ) == -1 ) err = -1;
    dbClose( fh );
    return err;
}

int WmIsAreaAccessible( int wx ,int wy )
{
    int v4;

    v4 = wy / 300 * gWmHorizTileCnt + wx / 350 % gWmHorizTileCnt;
    if( WmLoadWorldMask( v4 ) == -1 ) return 0;
    if( !gWmTiles[ v4 ].WorldMask ) return 0;
    return ((1 << (((wx % 350) >> 3) & 3)) & gWmTiles[ v4 ].WorldMask[ 44 * (wy % 300) + ((wx % 350) / 8 ) ] ) != 0;
}

int WmMapTravel( int a1, int a2 )
{
    int v2, v3, result;

    gWmUnk11 = a1;
    gWmUnk12 = a2;
    gWmAreaId = -1;
    gWmUnk13 = 1;
    v2 = abs32(a1 - gWmWorldPosX);
    v3 = abs32(a2 - gWmWorldPosY);
    if( v2 < v3 ){
        gWmSteps = v3;
        gWmUnk26 = 2 * v2;
        gWmUnk28 = 0;
        gWmUnk25 = 2 * v2 - v3;
        gWmUnk27 = 2 * (v2 - v3);
        gWmUnk53 = 1;
        gWmUnk29 = 1;
    } else {
        gWmSteps = v2;
        gWmUnk26 = 2 * v3;
        gWmUnk29 = 0;
        gWmUnk25 = 2 * v3 - v2;
        gWmUnk27 = 2 * (v3 - v2);
        gWmUnk28 = 1;
        gWmUnk53 = 1;
    }
    gWmUnk30 = 1;
    if( gWmUnk11 < gWmWorldPosX ){
        gWmUnk53 = -gWmUnk53;
        gWmUnk28 = -gWmUnk28;
    }
    if( gWmUnk12 < gWmWorldPosY ){
        gWmUnk30 = -gWmUnk30;
        gWmUnk29 = -gWmUnk29;
    }
    result = WmUnk80();
    if( !result ) return WmUnk48();
    return result;
}

void WmMoveStep()
{
    int TerrainDifficulty, v3;

    if( gWmSteps <= 0 ) return;
    if( ++gWmUnk130 > 4 ) gWmUnk130 = 1;
    WmGetTile( gWmWorldPosX, gWmWorldPosY, &gWmTile );
    TerrainDifficulty = gWmTerrainTypes[ gWmTile->TerrainType ].i01 - PerkLvl( gObjDude, PERK_PATHFINDER ); // travel time and battery drain is reduced by 25% with perk
    if( TerrainDifficulty < 1 ) TerrainDifficulty = 1;
    if( gWmUnk130 / TerrainDifficulty < 1 ) return;
    if( gWmUnk25 >= 0 ){
        if( WmIsAreaAccessible( gWmUnk53 + gWmWorldPosX, gWmUnk30 + gWmWorldPosY ) ){
            gWmUnk11 = gWmUnk12 = gWmUnk13 = 0;
            WmGetArea( gWmWorldPosX, gWmWorldPosX, &gWmAreaId );
            gWmSteps = 0;
            return;
        }
        gWmUnk25 += gWmUnk27;
        gWmWorldPosX += gWmUnk53;
        gWmWorldPosY += gWmUnk30;
        WmMoveMap( 1, 1, gWmUnk30, gWmUnk53, 0, 0 );
    } else {
        if( WmIsAreaAccessible( gWmUnk28 + gWmWorldPosX, gWmUnk29 + gWmWorldPosY ) ){
            gWmUnk11 = gWmUnk12 = gWmUnk13 = 0;
            WmGetArea( gWmWorldPosX, gWmWorldPosX, &gWmAreaId );
            gWmSteps = 0;
            return;
        }
        gWmUnk25 += gWmUnk26;
        gWmWorldPosY += gWmUnk29;
        gWmWorldPosX += gWmUnk28;
        WmMoveMap( 1, 1, gWmUnk29, gWmUnk28, 0, 0 );
    }
    v3 = --gWmSteps;
    if( !gWmSteps ){
        gWmUnk12 = gWmUnk13 = gWmUnk11 = v3;
    }        
}

void WmListScrollUpDn( int Scroll )
{
    int i, k;

    for( i = 0; i != 7; i++ ) WinDisableWidget( gWmWidgets[ i ] );

    gWmUnk64 = gWmLocListStartLine;
    k = gWmLocListStartLine + 7 * Scroll;
    if( Scroll >= 0 ){
        if( gWmLocListHeight - 230 > gWmUnk64 ){
    	    gWmUnk64 = gWmLocListStartLine + 7 * Scroll;
    	    if( k > gWmLocListHeight - 230 ) gWmUnk64 = gWmLocListHeight - 230;
	    gWmUnk65 = Scroll;
	}
    } else {
        if( gWmLocListStartLine > 0 ){
    	    gWmUnk64 = gWmLocListStartLine + 7 * Scroll;
    	    if( k < 0 ) gWmUnk64 = 0;
	    gWmUnk65 = Scroll;
	}
    }
    if( !gWmUnk65 ) return;
    gWmLocListStartLine += gWmUnk65;
    WmDrawIface( 1 );
    if( gWmUnk65 > -1 ){
        if( gWmUnk64 > gWmLocListStartLine ) return;
    } else if( gWmUnk64 < gWmLocListStartLine ){
        return;
    }
    WmRedrawWidgets();
}

void WmRedrawWidgets()
{
    int i;

    gWmUnk65 = 0;
    for( i = 0; i < 7; i++ ) WinEnableWidget( gWmWidgets[ i ] );
}

int WmMenuCreate()
{
    CachePool_t *v105;
    ArtFrmHdr_t *Img, *v15,*v21,*v28,*v33,*v41;
    int v20,v27,v32,v50,v59,v64,v69,ObjWidth,ObjHeight,Id,i;

    gWmTimeStamp = TimerGetSysTime();
    gWmFontSave = FontGetCurrent();
    FontSet( 0 );
    GSoundSetBg( gWmTravelByCar ? "20car" : "23world", 12 );
    CycleColorStop();
    GmouseLoadCursor( 1 );
    
    if( (gWmMenuWin = WinCreateWindow(0, 0, 640, 480, gPalColorCubeRGB[0][0][0], 4)) == -1 ) return -1;    

    if( !( Img = ArtLoadImg( ArtMakeId( 6, 136, 0, 0, 0 ), &gWmWallPaper ) ) ) return -1;
    gWmWpapW = ArtGetObjWidth( Img, 0, 0 );
    gWmWpapH = ArtGetObjHeight( Img, 0, 0 );
    ArtClose( gWmWallPaper );
    gWmWallPaper = ART_NULL;
    
    if( !(gWmWpapImg = ArtGetBitmap( ArtMakeId( 6, 136, 0, 0, 0 ), 0, 0, &gWmWallPaper )) ) return -1;
    
    if( !(gWmSurface = WinGetSurface( gWmMenuWin )) )return -1;
    ScrCopy( gWmWpapImg, gWmWpapW, gWmWpapH, gWmWpapW, gWmSurface, 640 );

    for( i = 0; i < 3; i++ ){
	WM_GET_IMG( 336 + i, gWmMenuImg[ i ].ArtId, gWmMenuImg[ i ].w, gWmMenuImg[ i ].h, gWmMenuImg[ i ].ImgObj, gWmMenuImg[ i ].ImgData, v15 );
    }

    WM_GET_IMG( 168, v20, gWmUnk44, gWmUnk45, gWmUnk40, gWmUnk41, v21);

    if( !(gWmUnk43 = ArtGetBitmap( ArtMakeId(6, 223, 0, 0, 0), 0, 0, &gWmUnk42 )) ) return -1;

    WM_GET_IMG( 139, v27, gWmUnk100, gWmUnk48, gWmUnk46, gWmUnk47, v28 );
    WM_GET_IMG( 138, v32, gWmUnk51, gWmUnk54, gWmUnk49, gWmUnk50, v33 );

    for( i = 0; i < 4; i++ ){
	WM_GET_IMG( gWmUnk125[ i ], Id, gWmUnk81[ i ], gWmUnk82[ i ], gWmUnk79[ i ], gWmUnk80[ i ], v41 );
    }

    for( i = 0; i < gWmTilesCnt; i++ ) gWmTiles[ i ].Img = ART_NULL;    

    v50 = ArtMakeId( 6, 364, 0, 0, 0 );    
    if( !(Img = ArtLoadImg( v50, &v105 )) ) return -1;
    gWmLocListWidth = ArtGetObjWidth( Img, 0, 0 );
    gWmLocListHeight = ArtGetObjHeight( Img, 0, 0 );
    ArtClose( v105 );

    gWmLocListImg = ArtGetBitmap( v50, 0, 0, &gWmUnk58 ) + 27 * gWmLocListWidth;
    if( !gWmLocListImg ) return -1;

    if( !(gWmImgDat03 = ArtGetBitmap( ArtMakeId(6, 367, 0, 0, 0), 0, 0, &gWmUnk60)) ) return -1;
    if( !(gWmDialImg = ArtLoadImg( ArtMakeId(6, 365, 0, 0, 0), &gWmUnk61)) ) return -1;
    gWmDialWidth = ArtGetObjWidth( gWmDialImg, 0, 0 );
    gWmDialHeight = ArtGetObjHeight( gWmDialImg, 0, 0 );

    v59 = ArtMakeId( 6, 363, 0, 0, 0 );
    if( !(Img = ArtLoadImg( v59, &v105 ) ) ) return -1;
    gWmCarFrameWidth = ArtGetObjWidth( Img, 0, 0 );
    gWmCarFrameHeight = ArtGetObjHeight( Img, 0, 0 );
    ArtClose( v105 );
    if( !(gWmCarFrameImg = ArtGetBitmap( v59, 0, 0, &gWmUnk62 ) ) ) return -1;

    v64 = ArtMakeId( 6, 366, 0, 0, 0 );
    if( !( Img = ArtLoadImg( v64, &v105 ) ) ) return -1;
    gWmVtLogoWidth = ArtGetObjWidth( Img, 0, 0 );
    gWmVtLogoHeight = ArtGetObjHeight( Img, 0, 0 );
    ArtClose( v105 );    
    if( !(gWmVtLogoImg = ArtGetBitmap( v64, 0, 0, &gWmUnk63 ) ) ) return -1;

    v69 = ArtMakeId( 6, 8, 0, 0, 0 );
    if( !(Img = ArtLoadImg( v69, &v105 ) ) ) return -1;
    ObjWidth = ArtGetObjWidth( Img, 0, 0 );
    ObjHeight = ArtGetObjHeight( Img, 0, 0 );
    ArtClose( v105 );
    gWmUnk68 = ArtGetBitmap( v69, 0, 0, &gWmUnk66 );
    gWmUnk69 = ArtGetBitmap( ArtMakeId( 6, 9, 0, 0, 0 ), 0, 0, &gWmUnk67 );
    if( !( gWmMonthImg = ArtLoadImg( ArtMakeId( 6, 129, 0, 0, 0 ), &gWmUnk74 ) ) ) return -1;
    if( !( gWmDigitImg = ArtLoadImg( ArtMakeId( 6, 82, 0, 0, 0 ), &gWmUnk76 ) ) ) return -1;

    WinCreateButton( gWmMenuWin, 519, 439, ObjWidth, ObjHeight, -1, -1, -1, 'T', gWmUnk68, gWmUnk69, 0, 32 ); // Town World

    for( i = 0; i < 7; i++ ){ // buttons on list
        gWmWidgets[ i ] = WinCreateButton( gWmMenuWin, 508, 138 + i * 27, ObjWidth, ObjHeight, -1, -1, -1, 350 + i, gWmUnk68, gWmUnk69, 0, 32 );
    }

    for( i = 0; i < 2; i++ ){
        if( !(Img = ArtLoadImg( ArtMakeId( 6, 200 - i, 0, 0, 0 ), &gWmUnk84[ i ] )) ) return -1;
        gWmUnk78 = ArtGetObjWidth( Img, 0, 0 );
        gWmUnk72 = ArtGetObjHeight( Img, 0, 0 );
        gWmUnk83[ i ] = ArtGetObjData( Img, 0, 0 );
    }

    for( i = 0; i < 2; i++ ){
        if( !(Img = ArtLoadImg( ArtMakeId( 6, 182 - i , 0, 0, 0 ), &gWmUnk99[ i ] ) ) ) return -1;
        gWmUnk73 = ArtGetObjWidth( Img, 0, 0 );
        gWmUnk70 = ArtGetObjHeight( Img, 0, 0 );
        gWmUnk85[ i ] = ArtGetObjData( Img, 0, 0 );
    }

    WinCreateButton( gWmMenuWin, 480, 137, gWmUnk78, gWmUnk72, -1, -1, -1, 397, gWmUnk83[ 1 ], gWmUnk83[ 0 ], 0, 32 ); // arrow UP
    WinCreateButton( gWmMenuWin, 480, 152, gWmUnk73, gWmUnk70, -1, -1, -1, 401, gWmUnk85[ 1 ], gWmUnk85[ 0 ], 0, 32 ); // arrow DN

    if( gWmTravelByCar == 1 ){        
        if( !(gWmCarImg = ArtLoadImg( ArtMakeId( 6, 433, 0, 0, 0 ), &gWmUnk39 ) ) ) return -1;
        gWmCarWidth = ArtGetObjWidth( gWmCarImg, 0, 0 );
        gWmCarHeight = ArtGetObjHeight( gWmCarImg, 0, 0 );
    }
    InpTaskStart( WmMouseHandle );
    if( WmMakeKnownLocationList( &gWmKnownLocations, &gWmKnownLocationsCount ) == -1 ) return -1;
    gWmUnk121 = 1;
    if( WmDrawMapScreen() == -1 ) return -1;        
    WinUpdate( gWmMenuWin );
    ScptDisable();
    ScptFlush(); // remove all scripts
    return 0;
}

int WmMenuDestroy()
{
    int i;

    InpTaskStop( WmMouseHandle );
    if( gWmWpapImg ){ ArtClose( gWmWallPaper ); gWmWpapImg = 0; }
    gWmWallPaper = ART_NULL;
    if( gWmMenuWin != -1 ){ WinClose( gWmMenuWin ); gWmMenuWin = -1; }
    if( gWmUnk40 != ART_NULL ) ArtClose( gWmUnk40 );  
    gWmUnk41 = 0;
    if( gWmUnk42 != ART_NULL ) ArtClose( gWmUnk42 );  
    gWmUnk43 = 0;
    if ( gWmUnk46 != ART_NULL ) ArtClose( gWmUnk46 ); 
    gWmUnk47 = 0;
    if( gWmUnk49 != ART_NULL ) ArtClose( gWmUnk49 );
    *gWmUnk50 = 0;
    for( i = 0; i != 4; i++ ){
        if( gWmUnk79[ i ] != ART_NULL ) ArtClose( gWmUnk79[ i ] );
        gWmUnk80[ i ] = NULL;
    }
    for( i = 0; i < 3; i++ ){
        ArtClose( gWmMenuImg[ i ].ImgObj );
        gWmMenuImg[ i ].ImgObj = ART_NULL;
        gWmMenuImg[ i ].ImgData = NULL;
    }    
    for( i = 0; i < gWmTilesCnt; i++ ){
        if( gWmTiles[ i ].Img == ART_NULL ) continue;
        ArtClose( gWmTiles[ i ].Img );
        gWmTiles[ i ].Img = ART_NULL;
        gWmTiles[ i ].Pix = NULL;
        if( !gWmTiles[ i ].WorldMask ) continue;
        Free( gWmTiles[ i ].WorldMask );
        gWmTiles[ i ].WorldMask = NULL;
    }
    
    if( gWmLocListImg ){ ArtClose( gWmUnk58 ); gWmUnk58 = ART_NULL; gWmLocListImg = NULL; }
    if( gWmImgDat03 ){ ArtClose( gWmUnk60 ); gWmUnk60 = ART_NULL; gWmImgDat03 = NULL; }
    if( gWmDialImg ){ ArtClose( gWmUnk61 ); gWmUnk61 = ART_NULL; gWmDialImg = NULL; }
    if( gWmCarFrameImg ){ ArtClose( gWmUnk62 ); gWmUnk62 = ART_NULL; gWmCarFrameImg = NULL; }
    if( gWmVtLogoImg ){ ArtClose( gWmUnk63 ); gWmUnk63 = ART_NULL; gWmVtLogoImg = NULL; }
    if( gWmUnk68 ){ ArtClose( gWmUnk66 ); gWmUnk66 = ART_NULL; gWmUnk68 = NULL; }
    if( gWmUnk69 ){ ArtClose( gWmUnk67 ); gWmUnk67 = ART_NULL; gWmUnk69 = NULL; }

    for( i = 0; i < 2; i++ ){
        ArtClose( gWmUnk84[ i ] );
        gWmUnk84[ i ] = ART_NULL;
        gWmUnk83[ i ] = NULL;
        ArtClose( gWmUnk99[ i ] );
        gWmUnk99[ i ] = ART_NULL;
        gWmUnk85[ i ] = NULL;
    }
    gWmUnk72 = 0;
    gWmUnk73 = 0;
    gWmUnk70 = 0;
    gWmUnk78 = 0;
    if( gWmMonthImg ){ ArtClose( gWmUnk74 ); gWmUnk74 = ART_NULL; gWmMonthImg = NULL; }
    if( gWmDigitImg ){ ArtClose( gWmUnk76 ); gWmUnk76 = ART_NULL; gWmDigitImg = NULL; }
    if( gWmCarImg ){
        ArtClose( gWmUnk39 );
        gWmUnk39 = ART_NULL;
        gWmCarImg = NULL;
        gWmCarWidth = 0;
        gWmCarHeight = 0;
    }
    gWmUnk31 = 0;
    gWmRandMapId = -1;
    gWmEncounterType = -1;
    gWmUnk34 = -1;
    IfaceIndicatorBoxShow();
    MapUnk34();
    CycleColorStart();
    FontSet(gWmFontSave);
    if( gWmKnownLocations ){
        Free( gWmKnownLocations );
        gWmKnownLocations = NULL;
    }
    gWmKnownLocationsCount = 0;
    gWmUnk121 = 0;
    ScptEnable();
    return 0;
}

int WmMoveMap( int dy, int dx, int hor, int vert, int *pAllow, int a6 )
{
    int x, y;

    x = gWmMapOffsetY;
    y = gWmMapOffsetX;
    if( pAllow ) *pAllow = 1;
    if( vert < 0 ){
        if( x > 0 ){
            x -= dx;
            if( x < 0 ) x = 0;
        } else {
    	    if( pAllow ) *pAllow = 0;
        }
    } else {
	if( vert > 0 ){
	    if( x >= gWmUnk17 ){
    		if( pAllow ) *pAllow = 0;
	    } else {
	        x += dx;
		if( x > gWmUnk17 ) x = gWmUnk17;
	    }
	}
    }
    if( hor < 0 ){
        if( y > 0 ){
            y -= dy;
            if( y < 0 ) y = 0;
        } else {
    	    if( pAllow ) *pAllow = 0;
        }
    } else {
	if( hor > 0 ){
	    if( y >= gWmUnk16 ){
    		if( pAllow ) *pAllow = 0;
	    } else {
		y += dy;
		if( y > gWmUnk16 ) y = gWmUnk16;
	    }
	}
    }
    gWmMapOffsetY = x;
    gWmMapOffsetX = y;
    if( a6 == 1 ){
	if( WmDrawMapScreen() == -1 ){
	    gWmMapOffsetX = gWmMapOffsetX;
	    gWmMapOffsetY = gWmMapOffsetY;
	    return -1;
	}
    }
    gWmMapOffsetX = gWmMapOffsetX;
    gWmMapOffsetY = gWmMapOffsetY;
    return 0;
}

void WmMouseHandle( int a1 )
{
    int Hor, CursorId, Vert, MseY, MseX;
    unsigned int CursorShape;

    Hor = Vert = 0;
    MseX = MseY = 0;
    MseGetCursorPosition( &MseX, &MseY );
    if( MseX == 0 ) Hor = -1;
    if( MseX == 639 ) Hor = 1;
    if( MseY == 0 ) Vert = -1;
    if( MseY == 479 ) Vert = 1;
    CursorId = GmouseGetCursorId();
    if( !Hor && !Vert ){
        if( CursorId != 1 ) GmouseLoadCursor( 1 );
        return;
    }
    if( Hor >= 0 ){
        if( Hor <= 0 ) {
            if( Vert >= 0 ) {
                if( Vert > 0 ) CursorShape = 9;
            } else {
                CursorShape = 5;
            }
        } else if( Vert >= 0 ){
            CursorShape = ( Vert <= 0 ) ? 7 : 8;
        } else {
            CursorShape = 6;
        }
    } else if( Vert >= 0 ){
        CursorShape = ( Vert <= 0 ) ? 11 : 10;
    } else {
        CursorShape = 4;
    }

    if( Hor || Vert ){
        if( TimerDiff( TimerGetTime(), gWmMseTimer ) > 50 ){
            gWmMseTimer = TimerGetTime();
            WmMoveMap( 20, 20, Hor, Vert, &gWmMoveMapAllow, 1 );
        }
    }
    if( !gWmMoveMapAllow ) CursorShape += 8;
    if( CursorId != CursorShape ) GmouseLoadCursor( CursorShape );
}

int WmUnk68( int a1, int a2, int a3, int a4, int a5, int a6 )
{
    int v6,v7,v8,v9,*p;

    v6 = gWmHorizTileCnt;
    v7 = a2 + a4;
    v8 = a1;
    v9 = a5 + a3;
    if( v7 >= 0 ){
        if( v7 >= 7 ){
            if( a1 % gWmHorizTileCnt == gWmHorizTileCnt - 1 ) return -1;
            v8 = a1 + 1;
            v7 %= 7;
        }
    } else {
        if( !(a1 % gWmHorizTileCnt) ){
	    gWmHorizTileCnt = v6;
	    return -1;
        }
        v7 += 7;
        v8 = a1 - 1;
    }
    if( v9 >= 0 ){
        if( v9 >= 6 ){
            if( v8 > gWmTilesCnt - gWmHorizTileCnt - 1 ) return -1;
            v8 += gWmHorizTileCnt;
            v9 %= 6;
        }
    } else {
        if( v8 < gWmHorizTileCnt ) return -1;
        v9 += 6;
        v8 -= gWmHorizTileCnt;
    }
    p = &gWmTiles[ v8 ].Dsc[ v9 ][ v7 ].Fog;
    if( a6 != 1 || !*p ) *p = a6;
    gWmHorizTileCnt = v6;
    return 0;
}

int WmUnk69( int wX, int wY )
{
    int tmp;

    tmp = 1;
    if( PerkLvl( gObjDude, PERK_SCOUT ) ) tmp = 2;
    return WmUnk70( wX, wY, tmp );
}

int WmUnk70( int wx, int wy, int scout )
{
    WmTileDsc_t *v8;
    int v3,v4,v5,j,v7,FillDir,i;

    v3 = wx / 350 % gWmHorizTileCnt + wy / 300 * gWmHorizTileCnt;
    v4 = wx % 350 / 50;
    v5 = wy % 300 / 50;
    for( i = -scout; i <= scout; i++ ){
        for( j = -scout; j <= scout; j++ ){
            v7 = j;
            WmUnk68( v3, v4, v5, v7, i, 1 );
        }
    }
    v8 = gWmTiles[ v3 ].Dsc[ v5 ];
    FillDir = v8[ v4 ].FillDir;
    v8[ v4 ].Fog = 2;

    switch( FillDir ){
        case 2:
            while( v5 > 0 ){
                if( --v5 > 0 ) WmUnk68( v3, v4, v5, 0, 0, 2 );
            }
            break;
        case 4:
            while( v4 > 0 ){
                if( --v4 > -1 ) WmUnk68( v3, v4, v5, 0, 0, 2 );
            }
            if( v3 % gWmHorizTileCnt > 0 ){
                for( i = 0; i < 7; i++ ) WmUnk68( v3 - 1, i, v5, 0, 0, 2 );
            }
            break;
    }
    return 0;
}

int WmUnk71( int a1, int a2, int *a3 )
{
    *a3 = gWmTiles[ a1 / 350 % gWmHorizTileCnt + a2 / 300 * gWmHorizTileCnt ].Dsc[ a2 % 300 / 50 ][ a1 % 350 / 50 ].Fog;
    return 0;
}

int WmLoadTileImg( int TileNo )
{
    WmTile_t *tile;

    tile = &gWmTiles[ TileNo ];
    if( tile->Pix ) return 0;
    if( (tile->Pix = ArtGetBitmap( tile->ArtId, 0, 0, &tile->Img )) ) return 0;
    WmMenuDestroy();
    return -1;
}

int WmDrawMapScreen()
{
    WmImg_t *wimg;
    int v0, i, v3, v4, v5, v6, xpos, v17, v18, v19, v20, v21, v22, v23; 
    int TileOfst, j, TileIdx, v28, dy, y, dx, x, ypos;

    if( gWmUnk121 != 1 ) return 0;
    v17 = gWmMapOffsetX % 350;
    v18 = gWmMapOffsetY % 300;
    v20 = 300 - gWmMapOffsetY % 300;
    v22 = gWmMapOffsetX % 350;
    v0 = gWmMapOffsetY / 300 * gWmHorizTileCnt + gWmMapOffsetX / 350 % gWmHorizTileCnt;
    v21 = 350 * (gWmMapOffsetY % 300);
    j = 0;
    v19 = 350 - gWmMapOffsetX % 350;
    do{
	v23 = 0;
	v28 = v0;
	for( i = 0; i < 450; i += v3 ){
    	    if ( WmLoadTileImg( v0 ) == -1 ) return -1;
    	    v3 = 350;
    	    v5 = 300;
    	    v4 = 0;
    	    if( i == 0 ){  v4 = v22; v3 = v19; }
    	    if( v3 + i > 450 ) v3 = 450 - i;
    	    if( !j ){ v5 = v20; v4 += v21; }
    	    if( v5 + j > 443 ) v5 = 443 - j;
    	    v0++;
    	    ScrCopy( gWmTiles[ v28 ].Pix + v4, v3, v5, 350, gWmSurface + 640 * (j + 21) + i + 22 , 640 );
    	    v6 = v23 + 1;
    	    v28++; 
    	    v23++;
	}
	j += v5;
	v0 += gWmHorizTileCnt - v6;
    }while( j < 443 );

    // draw captions
    for( i = 0; i < gWmAreasCount; i++ ){
        if( !gWmAreas[ i ].StartState ) continue;
        wimg = &gWmMenuImg[ gWmAreas[ i ].Size ];
        x = gWmAreas[ i ].WorldPosX - gWmMapOffsetX;
        y = gWmAreas[ i ].WorldPosY - gWmMapOffsetY;
        if( x < 0 || x > ( 472 - wimg->w ) ) continue;
        if( y < 0 || y > ( 465 - wimg->h ) ) continue;
        WmLocationCaption( &gWmAreas[ i ], wimg, gWmSurface, x, y );
    }

    // fog of war
    TileOfst = (gWmMapOffsetX / 350) % gWmHorizTileCnt + (gWmMapOffsetY / 300) * gWmHorizTileCnt;
    for( y = 0; y < 443; y += dy, TileOfst += gWmHorizTileCnt ){
	TileIdx = TileOfst;
        for( x = 0; x < 450; x += dx, TileIdx++ ){
            dx = 350 - ( (x == 0) ? v17 : 0 );
            if( x + dx > 450 ) dx = 450 - x;
            dy = 300 - ( (y == 0) ? v18 : 0 );
            if( y + dy > 443 ) dy = 443 - y;
            ypos = y + 21 - ( y ? 0 : v18 );
            for( j = 0; j < 6; j++, ypos += 50 ){
                xpos = x + 22 - ( x ? 0 : v17 );
                for( i = 0; i < 7; i++, xpos += 50 ) WmWarFog( &gWmTiles[ TileIdx ], i, j, xpos, ypos, 1 );
            }            
        }        
    }    
    WmPositionIndicator();
    WmDrawIface( 1 );
    return 0;    
}

void WmCalendar( int Update )
{
    int w0, w1, Num, gtime, i, GYear, Day, Month, h;
    char *surf, *pDst, *Digit;
    VidRect_t Rect;

    ScptGetGameDate( &Month, &Day, &GYear );
    Month--;

    surf = gWmSurface;

    // day 
    surf += 12 * 640 + 487;
    w0 = ArtGetObjWidth( gWmDigitImg, 0, 0 );
    h = ArtGetObjHeight( gWmDigitImg, 0, 0 );
    Digit = (char *)ArtGetObjData( gWmDigitImg, 0, 0 );
    ScrCopy( Digit + 9 * ( Day / 10 ), 9, h, w0, surf, 640 );
    ScrCopy( Digit + 9 * ( Day % 10 ), 9, h, w0, surf + 9, 640 );

    // month name
    w1 = ArtGetObjWidth( gWmMonthImg, 0, 0 );
    ScrCopy( (char *)ArtGetObjData( gWmMonthImg, 0, 0 ) + w1 * 15 * Month, w1, 14, w1, surf + 1 * 640 + 26, 640);

    // year
    pDst = surf + 98;
    for( i = 0; i < 4; i++ ){
        Num = GYear % 10;
        pDst -= 9;
        ScrCopy( Digit + 9 * Num, 9, h, w0, pDst, 640 );
        GYear /= 10;
    }

    // time
    gtime = ScptGetGameTime();
    pDst += 72;
    for( i = 0; i < 4; i++ ){
	Num = gtime % 10;
        ScrCopy( Digit + 9 * Num, 9, h, w0, pDst, 640 );
        pDst -= 9;
        gtime /= 10;
    }

    if( Update ){
        Rect.lt = 487;
        Rect.tp = 12;
        Rect.bm = h + 12;
        Rect.rt = 630;
        WinAreaUpdate( gWmMenuWin, &Rect );
    }
}

int WmGetArea( int PosX, int PosY, int *AreaNum )
{
    int i;
    WmArea_t *Area;
    WmImg_t *img;

    PosY += 21;
    PosX += 22;
    for( i = 0; i < gWmAreasCount; i++ ){
        Area = &gWmAreas[ i ];
        if( gWmAreas[ i ].StartState ){
            if( PosX >= Area->WorldPosX && PosY >= Area->WorldPosY ){
                img = &gWmMenuImg[ Area->Size ];
                if( PosX <= img->w + Area->WorldPosX && PosY <= img->h + Area->WorldPosY ) break;
            }
        }
    }
    *AreaNum = ( i >= gWmAreasCount ) ? -1 : i;
    return 0;
}

void WmLocationCaption( WmArea_t *Area, WmImg_t *img, char *surf, int Xpos, int Ypos )
{
    MsgLine_t MsgLine;
    char stmp[40];    
    int TxtId, w;
    Msg_t *msg;

    ObjRenderSemiTr( img->ImgData, img->w, img->h, img->w, surf, Xpos, Ypos, 640, 0x10000, gWmColor01, gObjPalBY );
    Ypos += img->h + 1;
    if( Ypos >= (464 - gFont.ChrHeight()) ) return;
    if( WmIsAreaKnown( Area->Id ) ){
        msg = &gMapMsg;
        TxtId = Area->Id + 1500;
    } else { // unknown city
        TxtId = 1004;
        msg = &gWmMsg;
    }
    strncpy( stmp, MessageGetMessage( msg, &MsgLine, TxtId ), 40 );
    w = gFont.LineWidth( stmp );
    gFont.Print( &surf[ 640 * Ypos + img->w / 2 + Xpos - w / 2], stmp, w, 640, gPalColorCubeRGB[0][31][0] );
}

void WmDrawGrayed( char *surf, int w, int h, int pitch )
{
    int i, j;

    for( i = pitch - w; --h != -1; surf += i ){
        for( j = w; j > 0; j--, surf++ ){
            *surf = gPalShades[ *(unsigned char *)surf ][ 75 ];
        }
    }
}

void WmWarFog( WmTile_t *Tiles, int WmX, int WmY, int ScrX, int ScrY, int width )
{
    char *surf;
    int x, h, w, tmp, y;

    y = ScrY;
    x = ScrX;
    h = 50;
    w = 50 * width;
    if( ScrY < 21 ){
        h = ( ScrY < 0 ) ? (ScrY + 29) : ( 50 - (21 - ScrY) );
        y = 21;
    }
    if( h + ScrY > 464 ) h = 464 - ScrY;
    if( ScrX < 22 ){
        x = 22;
        w -= 22 - ScrX;
    }
    tmp = w + ScrX;
    if( tmp > 472 ) w -= tmp - 472;
    if( w > 0 && h > 0 ){
        surf = &gWmSurface[ 640 * y + x ];
        switch( Tiles->Dsc[ WmY ][ WmX ].Fog ){
    	    case 0: ScrFillSolid( surf, w, h, 640, gPalColorCubeRGB[0][0][0] ); break; // black
    	    case 1: WmDrawGrayed( surf, w, h, 640 ); break; // grayed
    	    default: break; // uncovered
        }
    }
}

int WmPositionIndicator()
{
    int v0, v1, v4, v5;
    char *pDst, *pSrc;

    if( gWmUnk11 >= 1 || gWmUnk12 >= 1 ){
        if( gWmUnk31 == 1 ){
            v5 = gWmUnk81[ gWmUnk35 ];
            v4 = gWmUnk82[ gWmUnk35 ];
            pDst = gWmUnk80[ gWmUnk35 ];
        } else {
            v5 = gWmUnk51;
            v4 = gWmUnk54;
            pDst = gWmUnk50;
        }
        if( gWmWorldPosX >= gWmMapOffsetX && gWmWorldPosY >= gWmMapOffsetY && gWmWorldPosX < gWmMapOffsetX + 450 && gWmWorldPosY < gWmMapOffsetY + 443 )
            ScrCopyAlpha(pDst, v5, v4, v5, (22 - gWmMapOffsetX + gWmWorldPosX - v5 / 2 + 640 * (21 - gWmMapOffsetY + gWmWorldPosY - v4 / 2) + gWmSurface), 640);
        if( gWmUnk11 >= gWmMapOffsetX && gWmUnk12 >= gWmMapOffsetY && gWmUnk11 < gWmMapOffsetX + 450 && gWmUnk12 < gWmMapOffsetY + 443 )
            ScrCopyAlpha(gWmUnk47, gWmUnk100, gWmUnk48, gWmUnk100, (22 - gWmMapOffsetX + gWmUnk11 - gWmUnk100 / 2 + 640 * (21 - gWmMapOffsetY + gWmUnk12 - gWmUnk48 / 2) + gWmSurface), 640);
    } else {
        if( gWmUnk31 == 1 ){
            pSrc = gWmUnk80[ gWmUnk35 ];
            v0 = gWmUnk82[ gWmUnk35 ];
            v1 = gWmUnk81[ gWmUnk35 ];
        } else {
            pSrc = ( gWmGoToLocation ) ? gWmUnk43 : gWmUnk41;
            v0 = gWmUnk45;
            v1 = gWmUnk44;
        }
        if( gWmWorldPosX >= gWmMapOffsetX && gWmWorldPosY >= gWmMapOffsetY && gWmWorldPosX < gWmMapOffsetX + 450 && gWmWorldPosY < gWmMapOffsetY + 443 ){
            ScrCopyAlpha( pSrc, v1, v0, v1, (gWmSurface + 22 - gWmMapOffsetX + gWmWorldPosX - v1 / 2 + 640 * (21 - gWmMapOffsetY + gWmWorldPosY - v0 / 2)), 640 );
            return 0;
        }
    }
    return 0;
}

int WmUnk80()
{
    return gWmWorldPosX >= gWmMapOffsetX && gWmWorldPosY >= gWmMapOffsetY && gWmWorldPosX < gWmMapOffsetX + 450 && gWmWorldPosY < gWmMapOffsetY + 443;
}

int WmGetMiscMsg( WmArea_t *a1, char *a2 )
{
    MsgLine_t msg;

    strncpy( a2, MessageGetMessage( &gMapMsg, &msg, 1500 + a1->Id ), 40 ); // Misc messages
    return 0;
}

void WmGetLocationName( int CityId, char *CityName )
{
    char *s;
    MsgLine_t MsgLine;

    s = MessageGetMessage( &gMapMsg, &MsgLine, 1500 + CityId );
    strncpy( CityName, s, 40 );
}

int WmIsAreaKnown( int AreaId )
{
    if( ( AreaId < 0 ) || ( AreaId >= gWmAreasCount ) ) return 0;
    if( gWmAreas[ AreaId ].LockState[1] == 0 ) return 0;
    return gWmAreas[ AreaId ].StartState == 1;
}

int WmLocKarma( int AreaId )
{
    if( ( AreaId < 0 ) || ( AreaId >= gWmAreasCount ) ) return 0;
    if( gWmAreas[ AreaId ].LockState[ 1 ] == 0 ) return 0;
    if( gWmAreas[ AreaId ].StartState == 1 ) return 1;
    return 0;
}

int WmUnk03( int EntranceId )
{
    int Location, EnIdx;

    if( WmFindAreaByEntrance( EntranceId, &Location ) ) return 0;
    if( WmGetEntranceIdx( Location, EntranceId, &EnIdx ) ) return 0;
    return  gWmAreas[ Location ].Entrances[ EnIdx ].i01 == 1;
}

int WmUnk04( int LocationId, int a2 )
{
    WmArea_t *location;
    WmTileDsc_t *tile;
    int WorldPosY, WorldPosX;

    if( LocationId < 0 || LocationId >= gWmAreasCount ) return 0;
    location = &gWmAreas[ LocationId ];
    if( location->StartState == 1 && a2 ) WmUnk69( location->WorldPosX, location->WorldPosY );
    WorldPosY = location->WorldPosY;
    WorldPosX = location->WorldPosX;
    location->LockState[ 1 ] = a2;
    if( WmGetTile( WorldPosX, WorldPosY, &tile ) == -1 ) return 0;
    if( a2 == 1 ){
        tile->Fog = 1; // grayed
    } else if( a2 == 2 && !location->LockState[ 1 ] ){
        location->LockState[ 1 ] = 1;
    }
    return 1;
}

int WmUnk05( int LocationId, int a2, int a3 )
{
    WmArea_t *v4;

    if( LocationId < 0 || LocationId >= gWmAreasCount ) return 0;
    v4 = &gWmAreas[ LocationId ];
    if( v4->LockState[ 0 ] == 1 && !a3 ) return 0;
    v4->StartState = a2;
    return 1;
}

int WmSetPos( int Location, int WmX, int WmY )
{
    if( Location == 0 ) return 0;
    if( Location < 0 || Location >= gWmAreasCount ) return -1;
    if( WmX < 0 || WmY >= 350 * gWmHorizTileCnt ) return -1;
    if( WmY < 0 || WmY >= 300 * ( gWmTilesCnt / gWmHorizTileCnt ) ) return -1;
    gWmAreas[ Location ].WorldPosX = WmX;
    gWmAreas[ Location ].WorldPosY = WmY;
    return 0;
}

int WmGetWorldPos( int *pWmX, int *pWmY )
{
    if( pWmX ) *pWmX = gWmWorldPosX;
    if( pWmY ) *pWmY = gWmWorldPosY;
    return 0;
}

int WmGetCurrentLocation( int *pLocId )
{
    if( !pLocId ) return -1;
    *pLocId = gWmAreaId;
    return 0;
}

void WmClearTerrain( int Terrain )
{
    int i, j, k;

    for( i = 0; i < gWmTilesCnt; i++ ){
        for( j = 0; j < 6; j++){            
            for( k = 0; k < 7; k++ ){
        	gWmTiles[ i ].Dsc[ j ][ k ].TerrainType = Terrain;
            }
        }        
    }        
}

int WmUnk10( int a1 )
{
    return WmMenu1( a1 );
}

int WmTownMap( int *a1 )
{
    WmEntrance_t *Entrances;
    int sel, id, v9, v10;

    *a1 = -1;
    if( WmCreateTownMenu() == -1 ){
        WmCloseTownMenu();
        return -1;
    }
    if( gWmAreaId == -1 ) return -1;
    Entrances = gWmAreas[ gWmAreaId ].Entrances;
    while( 1 ){
        sel = InpUpdate();
        if( sel == 17 || sel == 24 || sel == 324 ) SysQuitDlg();
        if( gMenuEscape ) break;
        if( gWmUnk65 ){
            gWmLocListStartLine += gWmUnk65;
            WmDrawIface( 1 );
            if( gWmUnk65 > -1 ){
                if( gWmUnk64 <= gWmLocListStartLine ) WmRedrawWidgets();
            } else {
        	if( gWmUnk64 >= gWmLocListStartLine ) WmRedrawWidgets();
            }
        }
        if( sel != -1 ){
            if( sel == KEY_ESC ) break;
            if( sel >= '1' && sel < (gWmAreas[ gWmAreaId ].EntranceCnt + '1') ){
                id = sel - '1';
                *a1 = Entrances[ id ].Id;
                MapUnk10( Entrances[ id ].i05, Entrances[ id ].i06, Entrances[ id ].i07 );
                break;
            }
            if( sel < 350 || sel >= 357 ){
                if( sel >= 'w' ){
                    if( sel == 'w' ){
                	sel = KEY_ESC;
                    } else {
                        if( sel < 401 ){
                            if( sel == 397 ) WmListScrollUpDn( -27 );
                        } else if( sel <= 401 ){
                            WmListScrollUpDn( KEY_ESC );
                        } else if ( sel == 2069 ) {
                            if( WmGetCityEntryName() == -1 ) return -1;
                        }
                    }
                } else {
            	    if( sel == 'W' || sel == 't' ) sel = KEY_ESC;
            	    if( sel == 'T' ) sel = KEY_ESC;
                }
                if( sel == KEY_ESC ) break;
            } else {
                v9 = gWmLocListStartLine / 27 + sel - 350;
                if( v9 < gWmKnownLocationsCount ){
                    v10 = gWmKnownLocations[ v9 ];
                    if( !WmIsAreaKnown( gWmAreas[ v10 ].Id ) ) break;
                    if( v10 != gWmAreaId ){
                        WmMapTravel( gWmAreas[ v10 ].WorldPosX, gWmAreas[ v10 ].WorldPosY );
                        gWmGoToLocation = 0;
                        break;
                    }
                }
            }
        }
    }
    if( WmCloseTownMenu() == -1 ) return -1;
    return 0;
}

int WmCreateTownMenu()
{
    ArtFrmHdr_t *Img;
    WmEntrance_t *p;
    int TnmapArtIdx, i;

    gWmCityId = gWmAreaId;
    TnmapArtIdx = gWmAreas[ gWmAreaId ].TnmapArtIdx;
    Img = ArtLoadImg( TnmapArtIdx, &gWmUnk134 );
    if( !Img ) return -1;
    gWmUnk135 = ArtGetObjWidth( Img, 0, 0 );
    gWmUnk136 = ArtGetObjHeight( Img, 0, 0 );
    ArtClose( gWmUnk134 );
    gWmUnk134 = ART_NULL;
    gWmUnk133 = ArtGetBitmap( TnmapArtIdx, 0, 0, &gWmUnk134 );
    if( !gWmUnk133 ) return -1;
    for( i = 0; i < gWmAreas[ gWmAreaId ].EntranceCnt; i++ ) gWmUnk101[ i ] = -1;
    if( gWmAreas[ gWmAreaId ].EntranceCnt > 0 ){
	p = gWmAreas[ gWmAreaId ].Entrances;
	for( i = 0; i < gWmAreas[ gWmAreaId ].EntranceCnt; p++, i++ ){
    	    if( !p->i01 || p->i02 == -1 || p->i03 == -1 ) continue;
    	    gWmUnk101[ i ] = WinCreateButton( gWmMenuWin, p->i02, p->i03, gWmUnk44, gWmUnk45, -1, 2069, -1,'1' + i, gWmUnk41, gWmUnk43, 0, 32 );
    	    if( gWmUnk101[ i ] == -1 ) return -1;
	}
    }
    InpTaskStop( WmMouseHandle );
    if( WmGetCityEntryName() == -1 ) return -1;
    return 0;
}

int WmGetCityEntryName()
{
    int i, w;
    WmEntrance_t *p;
    MsgLine_t msg;

    ScrCopy( gWmUnk133, gWmUnk135, gWmUnk136, gWmUnk135, (gWmSurface + 13462), 640 );
    WmDrawIface( 0 );
    p = gWmAreas[ gWmAreaId ].Entrances;
    for( i = 0; i < gWmAreas[ gWmAreaId ].EntranceCnt; i++, p++){
        if( !p->i01 ) continue;
        if( p->i02 == -1 ) continue;
        if( p->i03 == -1 ) continue;
        msg.Id = 200 + 10 * gWmCityId + i; // location names
        if( MessageGetMsg( &gWmMsg, &msg ) != 1 ) continue;
        if( !msg.Text ) continue;
        w = gFont.LineWidth( msg.Text );
        WinDrawText( gWmMenuWin, msg.Text, w, gWmUnk44 / 2 + p->i02 - w / 2, gWmUnk45 + p->i03 + 2, gPalColorCubeRGB[0][31][0] | 0x2010000 );
    }
    WinUpdate( gWmMenuWin );
    return 0;
}

int WmCloseTownMenu()
{
    int i, id;

    if( gWmUnk134 != ART_NULL ){
        ArtClose( gWmUnk134 );
        gWmUnk133 = 0;
        gWmUnk134 = ART_NULL;
        gWmUnk135 = 0;
        gWmUnk136 = 0;
    }
    if( gWmCityId != -1 ){
        for( i = 0; i < gWmAreas[ gWmCityId ].EntranceCnt; i++ ){
            if( ( id = gWmUnk101[ i ] ) == -1 ) continue;
            WinImageRemove( id );
            gWmUnk101[ i ] = -1;
        }
    }
    if( WmDrawMapScreen() == -1 ) return -1;
    InpTaskStart( WmMouseHandle );
    return 0;    
}

int WmUnk86( int n )
{
    GlobVarGet( 456 );
    GlobVarGet( 455 );
    if( GlobVarGet( 453 ) ) n /= 2;
    gWmGasoline -= n;
    if( gWmGasoline < 0 ) gWmGasoline = 0;
    return 0;
}

int WmCarRefuel( int fuel )
{
    int overfuel;

    if( fuel + gWmGasoline <= 80000 ){
        gWmGasoline += fuel;
        return 0;
    }
    overfuel = 80000 - gWmGasoline;
    gWmGasoline = 80000;
    return fuel - overfuel;
}

int WmCarFuelGauge()
{
    return gWmGasoline;
}

int WmCarNoFuel()
{
    return gWmGasoline <= 0;
}

int WmUnk90()
{
    return gWmCurrentArea;
}

int WmUnk91()
{
    Map01_t map;
    MsgLine_t msg;

    memset( &msg, 0, sizeof( MsgLine_t ) );
    if( gWmGasoline <= 0 ){
        IfcMsgOut( MessageGetMessage( &gWmMsg, &msg, 1502 ) ); // 'The car is out of power.'
        return -1;
    }
    gWmTravelByCar = 1;
    memset( &map, 0, sizeof( Map01_t ) );
    map.MapId = -1;
    MapSetPos( &map );
    gWmAreas[ 21 ].StartState = 0;
    gWmAreas[ 21 ].LockState[ 1 ] = 0;
    return 0;
}

int WmUnk92()
{
    int CurrentMapId;

    CurrentMapId = MapGetCurrentMapId();
    if( CurrentMapId >= 0 && CurrentMapId < gWmMapCount ) return gWmMaps[ CurrentMapId ].AmbientSfxCnt;
    return -1;
}

int WmUnk93()
{
    Wmap_t *map;
    int MapId, cnt, i;

    MapId = MapGetCurrentMapId();
    cnt = 0;
    if( MapId < 0 || MapId >= gWmMapCount ) return -1;
    map = &gWmMaps[ MapId ];
    for( i = 0; i < map->AmbientSfxCnt; i++ ){
        cnt += map->AmbientSfx[ i ].Id;
    }
    cnt = RandMinMax( 0, cnt );    
    for( i = 0; i < map->AmbientSfxCnt; i++ ){
    	if( cnt < map->AmbientSfx[ i ].Id ) return i;
        cnt -= map->AmbientSfx[ i ].Id;
    }
    return -1;
}

int WmGetSfxFileName( int NameId, char **pName )
{
    int CurrentMapId, flg, n;
    Wmap_t *map;
    char *s;

    if( !pName ) return -1;
    *pName = NULL;
    CurrentMapId = MapGetCurrentMapId();
    if( CurrentMapId < 0 || CurrentMapId >= gWmMapCount ) return -1;
    map = &gWmMaps[ CurrentMapId ];
    if( ( NameId < 0 ) || ( NameId >= map->AmbientSfxCnt ) ) return -1;
    s = map->AmbientSfx[ NameId ].Name;
    *pName = s;
    flg = 0;
    if( !strcmp( s, "brdchir1" ) ){
        flg = 1;
    } else if( !strcmp( *pName, "brdchirp" ) ){
        flg = 2;
    }
    if( flg ){
        n = ScptGetGameTime();
        if( n >= 1800 ){
    	    *pName = gWmSfxFname[ flg - 1 ];
        } else {
    	    if( n >= 1200 ) return 0;
    	    if( n <= 600 ) *pName = gWmSfxFname[ flg - 1 ];
        }
    }
    return 0;
}

int WmDrawIface( int Update )
{
    int gtime;
    char *dat;

    ScrCopyAlpha( gWmWpapImg, gWmWpapW, gWmWpapH, gWmWpapW, gWmSurface, 640 );
    WmRenderLocationNameList();
    gtime = ScptGetGameTime() / 100;
    gtime = (gtime + 12) % ArtGetFpd( gWmDialImg );
    if( gtime != gWmDialFrame ){
        gWmDialFrame = gtime;
        WmDrawDial( 0 );
    }
    WmDrawDial( 0 );

    if( gWmTravelByCar ){        
        if( !(dat = ArtGetObjData( gWmCarImg, gWmCarFrameNo, 0 ) ) ) return -1;
        ScrCopy( dat, gWmCarWidth, gWmCarHeight, gWmCarWidth, WIN_XY( 514, 336, 640, gWmSurface ), 640);
        ScrCopyAlpha( gWmCarFrameImg, gWmCarFrameWidth, gWmCarFrameHeight, gWmCarFrameWidth, gWmSurface + 330 * 640 + 499, 640 );
        WmDrawGasGauge();
    } else {
        ScrCopyAlpha( gWmVtLogoImg, gWmVtLogoWidth, gWmVtLogoHeight, gWmVtLogoWidth, gWmSurface + 330 * 640 + 495, 640 ); // logo Vault-tech Maps
    }
    WmCalendar( 0 );
    if( Update == 1 ) WinUpdate( gWmMenuWin );
    return 0;
}

void WmDrawGasGauge()
{
    int i, j;
    char *p;

    j = 70 * gWmGasoline / 80000;
    if( (j & 1) != 0 ) j--;

    p = WIN_XY( 500, 339, 640, gWmSurface );
    for( i = 70; i > j; p += 640, i-- ) *p = 14;
    for( ;j > 0; j -= 2 ){
        *p = 196; p += 640;
        *p = 14;  p += 640;
    }
}

int WmRenderLocationNameList()
{
    int i, Idx, w, h, v30;
    char *v0, *v12, *v29;
    CachePool_t *ImgDat;
    ArtFrmHdr_t *img;
    char *pix;
    char *v11;

    ScrCopyAlpha( (gWmLocListWidth * gWmLocListStartLine + gWmLocListImg + 9), 119, 178, gWmLocListWidth, WIN_XY( 501, 135, 640, gWmSurface ), 640 );

    v29 = gWmSurface + 138 * 640 + 530;
    v0 = v29 - 640 * (gWmLocListStartLine % 27);
    v30 = gWmLocListStartLine / 27;

    if( v30 < gWmKnownLocationsCount ){        
        if( ( Idx = gWmAreas[ gWmKnownLocations[ gWmLocListStartLine / 27 ] ].TnmapLblArtIdx) != -1 ){
            if( !(img = ArtLoadImg( Idx, &ImgDat ) ) ) return -1;
            w = ArtGetObjWidth( img, 0, 0 );
            h = ArtGetObjHeight( img, 0, 0 );
            if( !(pix = ArtGetObjData( img, 0, 0 ) ) ) return -1;
            h -= gWmLocListStartLine % 27;
	    v11 = v0;
            if( v11 < v29 - 640 ) v11 = v29 - 640;
            ScrCopy( pix + w * (gWmLocListStartLine % 27), w, h, w, v11, 640 );
            ArtClose( ImgDat );
            ImgDat = ART_NULL;
        }
    }
    v12 = v0 + 27 * 640;
    i = v30 + 1;
    do{
        if( i < gWmKnownLocationsCount ){
            if( ( Idx = gWmAreas[ gWmKnownLocations[ i ] ].TnmapLblArtIdx ) != -1 ){
                if( !(img = ArtLoadImg( Idx, &ImgDat ) ) ) return -1;
                w = ArtGetObjWidth( img, 0, 0 );
                h = ArtGetObjHeight( img, 0, 0 );
                if( !(pix = ArtGetObjData( img, 0, 0 ) ) ) return -1;
                ScrCopy( pix, w, h, w, v12, 640 );
                ArtClose( ImgDat );
                ImgDat = ART_NULL;
            }
        }
        ++i;
        v12 += 27 * 640;
    }while ( i < (v30 + 6) );
    i = v30 + 6;
    if( i < gWmKnownLocationsCount ){
        if( ( Idx = gWmAreas[ gWmKnownLocations[ i ] ].TnmapLblArtIdx ) != -1 ){
            if( !(img = ArtLoadImg( Idx, &ImgDat ) ) ) return -1;
            w = ArtGetObjWidth( img, 0, 0 );
            h = ArtGetObjHeight( img, 0, 0 ) - 5;            
            if( !( pix = ArtGetObjData( img, 0, 0 ) ) ) return -1;
            ScrCopy( pix, w, h, w, v12, 640 );
            ArtClose( ImgDat );
            ImgDat = ART_NULL;
        }
    }
    ScrCopyAlpha( gWmImgDat03, 119, 178, 119, gWmSurface + 640*135 + 501, 640 );
    return 0;
}

int WmMakeKnownLocationList( int **pNameIds, int *pCount )
{
    int slots, Allocated, *p, i;

    slots = 10;
    if( *pNameIds ){  Free( *pNameIds ); *pNameIds = NULL; }
    Allocated = sizeof( int *) * 10;
    *pCount = 0;
    if( !(*pNameIds = Malloc( Allocated ) ) ) return -1;    
    for( i = 0; i < gWmAreasCount; i++ ){
        if( !WmIsAreaKnown( i ) ) continue;
        if( gWmAreas[ i ].TnmapLblArtIdx == -1 ) continue;
        (*pCount)++;
        if( slots <= *pCount ){
            Allocated += sizeof( int *) * 10;
            slots += 10;
            if( !(p = Realloc( *pNameIds, Allocated ) ) ) return -1;
            *pNameIds = p;
        }
        (*pNameIds)[*pCount - 1] = i;
    }
    qsort( *pNameIds, *pCount, sizeof( int ), (void *)WmSortNamesCb );
    return 0;
}

int WmSortNamesCb( int *idx1, int *idx2 )
{
    return strcasecmp( gWmAreas[ *idx1 ].Name, gWmAreas[ *idx2 ].Name );
}

void WmDrawDial( int UpdateFlag )
{
    VidRect_t Rect;

    ScrCopyAlpha( ArtGetObjData( gWmDialImg, gWmDialFrame, 0 ), gWmDialWidth, gWmDialHeight, gWmDialWidth, gWmSurface + 48*640 + 532, 640 );
    if( !UpdateFlag ) return;
    Rect.rt = gWmDialWidth + 532;
    Rect.lt = 532;
    Rect.bm = gWmDialHeight + 47;
    Rect.tp = 47;
    WinAreaUpdate( gWmMenuWin, &Rect );
}

int WmGetAssignId( int *pIdx )
{
    WmArea_t *area;
    int i;

    *pIdx = -1;
    if( gWmAreaId == -1 ) return -1;
    area = &gWmAreas[ gWmAreaId ];
    if( !area->EntranceCnt ) return -1;    
    for( i = 0; i < area->EntranceCnt; i++ ){
    	if( area->Entrances->i01 != 1 ){
    	    *pIdx = area->Entrances[ i ].Id;
    	     break;
    	}            
    }    
    if( i == area->EntranceCnt ){
        area->Entrances[ 0 ].i01 = 1;
        *pIdx = area->Entrances[ 0 ].Id;
    }
    return 0;
}

int WmStartMapMusic()
{
    int MapIdx;

    MapIdx = MapGetCurrentMapId();
    if( MapIdx != -1 && MapIdx < gWmMapCount ){
	if( gWmMaps[ MapIdx ].BgMusicFname[ 0 ] ){
	    if( GSoundSetBg( gWmMaps[ MapIdx ].BgMusicFname, 12 ) != -1 ) return 0; // Success
	}
    }
    eprintf( "\nWorldMap Error: Couldn't start map Music!\n" ); 
    return -1; // Fail
}
int WmSetMapMusic( int MapId, const char *MusicFname )
{
    if( MapId == -1 || MapId >= gWmMapCount ) return -1;
    if( !MusicFname ) return -1;
    eprintf( "\nwmSetMapMusic: %d, %s", MapId, MusicFname );
    strncpy( gWmMaps[ MapId ].BgMusicFname, MusicFname, 40 );
    gWmMaps[ MapId ].BgMusicFname[ 39 ] = '\0';
    if( MapGetCurrentMapId() == MapId ){
        GSoundBgClose();
        WmStartMapMusic();
    }
    return 0;
}

int WmFindAreaByEntranceId( int Id, int *pIdx )
{
    int i, j;
    WmEntrance_t *p;

    *pIdx = 0;
    for( i = 0; i < gWmAreasCount; i++ ){
	for( j = 0; j < gWmAreas[ i ].EntranceCnt; j++, p++ ){
	    if( Id == gWmAreas[ i ].Entrances[ j ].Id ){
		*pIdx = i;
		return 0;
	    }    		
    	}	
    }
    return 0;
}

int WmSetArea( int AreaId )
{
    if( AreaId < 0 || AreaId >= gWmKnownLocationsCount ) return -1;
    gWmAreaId = AreaId;
    gWmUnk11 = 0;
    gWmUnk12 = 0;
    gWmUnk13 = 0;
    gWmWorldPosX = gWmAreas[ AreaId ].WorldPosX;
    gWmWorldPosY = gWmAreas[ AreaId ].WorldPosY;
    return 0;
}

