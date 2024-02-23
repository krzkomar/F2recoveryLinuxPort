#include "FrameWork.h"

void (*gMapRedrawIsoCb)( VidRect_t * ) = MapRedrawIso; 
int gMapGridFlags[ 3 ] = { 2, 4, 8 };
int gMapSysTime = 0;
int gMapAmbientEnable = 0;
int gMapIsoPlayerElev =  0;
int gMapIsoUnk04 = -1;
int gMapIsoUnk05 = 0;
int gMapScriptId = -1;
int *gMapLocalVars = NULL;
int *gMapVars = NULL;
int gMapLocalVarsCnt = 0;
int gMapVarsCnt = 0;
char *gMapName = "";
int gMapUnk43 = 0;
int gMapUnk03 = -1;
char gMapCurrentFName[ 16 ];
int gMapUnk33;
int gMapUnk35;
int gMapUnk34;
int gMapIsoGridData[ 30000 ]; // 3 * 100 * 100
Map01_t gMapCurrentPos;
VidRect_t gMapIsoGeo;
Msg_t gMapMsg;
char *gMapIsoSurf;
Map_t gMap;
int *gMapIsoGrid[ 3 ];
int  gMapIsoWin;
char gMapCityName[ 40 ];
char gMapFileName[ 260 ];

/**********************************************************/

int MapIsoInit()
{
    int i;

    TileUnk24();
    TileUnk21();
    gMapCurrentFName[0] = '\0';
    for( i = 0; i != 3; i++ ) gMapIsoGrid[ i ] = &gMapIsoGridData[ i * 100 * 100 ];
    if( (gMapIsoWin = WinCreateWindow( 0, 0, gVidMainGeo.rt - gVidMainGeo.lt + 1, gVidMainGeo.bm - gVidMainGeo.tp - 99, 256, 10 )) == -1 ){ eprintf( "win_add failed in iso_init\n" ); return -1; }
    gMapIsoSurf = WinGetSurface( gMapIsoWin );
    if( !gMapIsoSurf ){ eprintf( "win_get_buf failed in iso_init\n" ); return -1; }
    if( WinGetRect( gMapIsoWin, &gMapIsoGeo ) ){ eprintf( "win_get_rect failed in iso_init\n" ); return -1; }
    if( ArtInit() ){ eprintf( "art_init failed in iso_init\n" ); return -1; }
    eprintf( ">art_init\t\t" );
    if( TileInit( gMapIsoGrid, 100, 100, 200, 200, gMapIsoSurf, gVidMainGeo.rt - gVidMainGeo.lt + 1, gVidMainGeo.bm - gVidMainGeo.tp - 99, gVidMainGeo.rt - gVidMainGeo.lt + 1, MapUpdateArea ) ){
        eprintf( "tile_init failed in iso_init\n" );
        return -1;
    }
    eprintf( ">tile_init\t\t" );

    if( ObjInit( gMapIsoSurf, gVidMainGeo.rt - gVidMainGeo.lt + 1, gVidMainGeo.bm - gVidMainGeo.tp - 99, gVidMainGeo.rt - gVidMainGeo.lt + 1 ) ){ 
	eprintf( "obj_init failed in iso_init\n" ); 
	return -1; 
    }

    eprintf( ">obj_init\t\t" );
    CycleColorsInit();
    eprintf( ">cycle_init\t\t" );
    TileUnk20();
    TileUnk23();
    if( IfaceInit() ){ eprintf( "intface_init failed int iso_init\n" ); return -1; }
    eprintf( ">intface_init\t" );
    MapUnk08( -1 );
    gMapIsoPlayerElev = -1;
    gMapIsoUnk04 = -1;
    gMapIsoUnk05 = -1;
    return 0;
}

void MapIsoReset()
{
    if( gMapVars ){
        Free( gMapVars );
        gMapVars = NULL;
        gMapVarsCnt = 0;
    }
    if( gMapLocalVars ){
        Free( gMapLocalVars );
        gMapLocalVars = NULL;
        gMapLocalVarsCnt = 0;
    }
    ObjReset();
    CycleColorRestart();
    IfaceReset();
    gMapIsoPlayerElev = -1;
    gMapIsoUnk04 = -1;
    gMapIsoUnk05 = -1;
}

void MapIsoClose()
{
    IfaceClose();
    CycleColorDisable();
    ObjClose();
    ArtCacheFree();
    WinClose( gMapIsoWin );
    if( gMapVars ){
        Free( gMapVars );
        gMapVars = NULL;
        gMapVarsCnt = 0;
    }
    if( gMapLocalVars ){
        Free(gMapLocalVars);
        gMapLocalVars = NULL;
        gMapLocalVarsCnt = 0;
    }
}

void MapMsgInit()
{
    char stmp[60];
    char *Exec;
    CfgGetString( &gConfiguration, "system", "executable", &Exec );
    if( !strcasecmp( Exec, "mapper" ) ) gMapRedrawIsoCb = MapMapperCb;
    if( MessageInit( &gMapMsg ) == 1 ){
        sprintf( stmp, "%smap.msg", gGamePath );
        if( MessageLoad( &gMapMsg, stmp ) != 1 ) eprintf( "\nError loading map_msg_file!" );
    } else {
        eprintf( "\nError initing map_msg_file!" );
    }
    MapUnk09();

    InpTaskStart( GmouseProcess );
    GmouseSetIfaceMode( 0 );
    WinMoveTop( gMapIsoWin );
}

void MapStartA() // no x-ref
{
    MapUnk09();
    InpTaskStart( GmouseProcess );
    GmouseSetIfaceMode( 0 );
    WinMoveTop( gMapIsoWin );
}

int MapExitMsg()
{
    WinUpdateDirty( gMapIsoWin );
    GmouseLoadCursor( 1 );
    InpTaskStop( GmouseProcess );
    if( MessageClose( &gMapMsg ) != 1 ){ eprintf( "\nError exiting map_msg_file!" ); return -1; }
    return 1;
}

void MapAmbientEnable()
{
    if( gMapAmbientEnable ) return;    
    TextEvEnable();
    if( !GameIfaceStat() ) GmouseSetIsoMode();
    InpTaskStart( AnimProcess );
    InpTaskStart( AnimAmbient );
    ScptAmbientEnable();
    gMapAmbientEnable = 1;    
}

int MapAmbientDisable()
{
    if( !gMapAmbientEnable ) return 0;
    ScptAmbientDisable();
    InpTaskStop( AnimAmbient );
    InpTaskStop( AnimProcess );
    GmouseSetIfaceMode( 0 );
    TextEvDisable();
    gMapAmbientEnable = 0;
    return 1;
}

int MapAmbientEnabled()
{
    return gMapAmbientEnable == 0;
}

int MapSetLvl( unsigned int a1 )
{
    int v1;

    v1 = 0;
    if( a1 > 2 ) return -1;
    if( GmouseGetCursorId() != 25 ){
        v1 = GmouseUnk58();
        GmouseUnk03();
        GmouseLoadCursor( 0 );
    }
    if( a1 != gCurrentMapLvl ) WmUnk45( gCurrentMapId, a1, 1 );
    gCurrentMapLvl = a1;
    AnimClear( gObjDude );
    AnimUnk24( gObjDude, gObjDude->Orientation, gObjDude->ImgId );
    PartyUnk07();
    if( gMapScriptId != -1 ) ScptMapUpdate();
    if( v1 ) GmouseIsoEnter();
    return 0;
}

int MapUnk23( unsigned int a1 )
{
    return a1 > 2 || (gMap.MapFlags & gMapGridFlags[ a1 ] ) != 0;
}

int MapSetVar( int VarId , int Val )
{
    if( VarId < 0 || VarId >= gMapVarsCnt ){
        eprintf( "ERROR: attempt to reference map var out of range: %d", VarId );
        return -1;
    }
    gMapVars[ VarId ] = Val;
    return 0;
}

int MapGetVar( int VarId )
{
    if( VarId < 0 || VarId >= gMapVarsCnt ){
	eprintf( "ERROR: attempt to reference map var out of range: %d", VarId );
	return 0;
    }
    return gMapVars[ VarId ];
}

int MapSetLocalVar( int VarId, int Val )
{
    if( VarId < 0 || VarId >= gMapLocalVarsCnt ){
        eprintf( "ERROR: attempt to reference local var out of range: %d", VarId );
        return -1;
    }
    gMapLocalVars[ VarId ] = Val;
    return 0;    
}

int MapGetLocalVar( int VarId )
{
    if( VarId >= 0 && VarId < gMapLocalVarsCnt ) return gMapLocalVars[ VarId ];
    eprintf( "ERROR: attempt to reference local var out of range: %d", VarId );
    return 0;
}

int MapVarsAdd( int VarNum )
{
    int *p, id;

    id = gMapLocalVarsCnt;
    gMapLocalVarsCnt += VarNum;    
    if( !(p = (int *)Realloc( gMapLocalVars, gMapLocalVarsCnt * sizeof( int ) )) ){ eprintf( "\nError: Ran out of memory!" ); return -1; }
    gMapLocalVars = p;
    memset( p + gMapLocalVarsCnt - VarNum, 0, VarNum * sizeof( int ) );
    return id;
}

void MapSetStart( int GridPos, int MapLvl, int Rotation )
{
    gMapUnk33 = MapLvl;
    gMapUnk35 = Rotation;
    gMapUnk34 = GridPos;
}

void MapNewScript( int ScriptId ) // not used
{
    Obj_t *obj;
    Scpt_t *scr;

    if( ScriptId < 0 ) ScriptId = 0;
    gMap.ScriptId = ( ScriptId == -1 ) ? 0 : ScriptId;
    if( gMapScriptId != -1 ){
        ScptRemove( gMapScriptId );
        gMapScriptId = -1;
    }
    if( ScriptId <= 0 ) return;
    if( ScptNewScript( &gMapScriptId, 0 ) == -1 ) return;
    obj = NULL;
    ObjCreate( &obj, ArtMakeId( 5, 12, 0, 0, 0 ), -1 );
    obj->Flags |= 0x20000005;
    ObjMoveToTile( obj, 1, 0, 0 );
    obj->ScrId = gMapScriptId;
    ScptSetArg( gMapScriptId, !(gMap.MapFlags & 0x01) );
    ScptPtr( gMapScriptId, &scr );
    scr->LocVarId = gMap.ScriptId - 1;
    scr->Flags |= 0x08;
    obj->ProtoPid = ScptNewObjId();
    scr->i08 = obj->ProtoPid;
    scr->TimeEv = obj;
}

void MapCurSetFileName( char *fname )
{
    strcpy( gMapCurrentFName, fname );
}

void MapCurGetFileName( char *fname )
{
    strcpy( fname, gMapCurrentFName );
}

char *MapCityLvlName( int MapId, unsigned int lvl )
{
    MsgLine_t msg;

    if( MapId < 0 || MapId >= WmGetMapCount() || lvl >= 3 ) return NULL;
    return MessageGetMessage( &gMapMsg, &msg, 200 + lvl + 3 * MapId );
}

int MapAreaCmpByMapId( int MapId1, int MapId2 )
{
    int Area2, Area1;

    if( MapId1 < 0 || MapId1 >= WmGetMapCount() ) return 0;
    if( MapId2 < 0 || MapId2 >= WmGetMapCount() ) return 0;
    if( !WmIsMapSaved( MapId1 ) ) return 0;    
    if( !WmIsMapSaved( MapId2 ) ) return 0;    
    if( WmFindAreaByEntranceId(MapId1, &Area1) == -1 ) return 0;
    if( WmFindAreaByEntranceId(MapId2, &Area2) == -1 ) return 0;
    return Area1 == Area2;
}

int MapAreaCmpByEntId( int EntId1, int EntId2 )
{
    int Area2, Area1;

    Area1 = -1;
    Area2 = -2;
    if( WmFindAreaByEntranceId(EntId1, &Area1) == -1 ) return -1;
    if( WmFindAreaByEntranceId(EntId2, &Area2) == -1 ) return -1;    
    if( Area1 != Area2 ) return -1;    
    return Area1;    
}

char *MapGetCityName( int MapId )
{
    MsgLine_t msg;
    int AreaId;

    if( WmFindAreaByEntranceId( MapId, &AreaId ) == -1 ) return "ERROR! F2"; 
    return MessageGetMessage( &gMapMsg, &msg, AreaId + 1500 );
}

char *MapGetCurrentCityName( int EntranceId )
{
    int AreaId;

    if( !WmFindAreaByEntranceId( EntranceId, &AreaId ) ){
        WmGetLocationName( AreaId, gMapCityName );
    } else {
	strcpy( gMapCityName, gMapName );
    }
    return gMapCityName;
}

int MapGetCurrentMapId()
{
    return gMap.MapId;
}

int MapMove( int dx, int dy)
{
    char *v9, *v10;
    int yg,tmp,v8,ScrWidth,i,pX,pY,xg,ScrHeight,v21;
    VidRect_t AreaH, AreaV;

    if( TimerCurrDiff( gMapSysTime ) < 33 ) return -2; // scroll speed
    gMapSysTime = TimerGetSysTime();
    xg = 32 * dx; 
    yg = 24 * dy;
    if( !xg && !yg ) return -1;
    GmouseUnk03();

    // move origin
    TileGetScrCoordinates( gTileCentIdx, &pX, &pY ); // get origin
    pX += xg + 16;
    pY += yg + 8;    
    if( ( tmp = TileGetPointed( pX, pY ) ) == -1 ) return -1;
    if( TileSetCenter( tmp, 0 ) == -1 ) return -1; // set origin

    // scroll
    AreaV = gMapIsoGeo;
    AreaH  = gMapIsoGeo;
    ScrWidth  = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    ScrHeight = gVidMainGeo.bm - gVidMainGeo.tp - 99;

    if( xg ) ScrWidth = gVidMainGeo.rt - gVidMainGeo.lt - 31;
    if( yg ) ScrHeight -= 24;
    if( xg < 0 )
        AreaH.rt = AreaH.lt - xg;
    else
        AreaH.lt = AreaH.rt - xg;

    v8 = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    if( yg < 0 ) {
        AreaV.bm = AreaV.tp - yg;
        v10 = &gMapIsoSurf[ v8 * (ScrHeight - 1) ];
        v9 = &gMapIsoSurf[ v8 * (gVidMainGeo.bm - gVidMainGeo.tp - 100) ];
        if( xg < 0 )
            v9 -= xg;
        else
            v10 += xg;
        v21 = -(gVidMainGeo.rt - gVidMainGeo.lt + 1);
    } else {
        AreaV.tp = AreaV.bm - yg;
        v9 = gMapIsoSurf;
        v10 = &gMapIsoSurf[v8 * yg];
        if( xg < 0 )
            v9 = &gMapIsoSurf[-xg];
        else
            v10 += xg;
        v21 = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    }
    for( i = 0; i < ScrHeight; v9 += v21, i++, v10 += v21 ) memmove( v9, v10, ScrWidth );    
    if( xg ) gMapRedrawIsoCb( &AreaH );
    if( yg ) gMapRedrawIsoCb( &AreaV );

    WinUpdate( gMapIsoWin );
    return 0;
}

char *MapGetFilePath( char *fname )
{
    if( *fname == '/' ) return fname;
    sprintf( gMapFileName, "maps/%s", fname );
    return gMapFileName;    
}

int MapUnk10( int a1, int a2, int a3 )
{
    gMapIsoPlayerElev = a1;
    gMapIsoUnk04 = a2;
    gMapIsoUnk05 = a3;
    return 0;
}

void MapUnk09()
{
    MapSetLvl(0);
    TileSetCenter( 20100, 2 );
    memset( &gMapCurrentPos, 0, sizeof( gMapCurrentPos ) );
    gMap.MapLvl = 0;
    gMap.PlayerOrientation = 0;
    gMap.LocVarsCnt = 0;
    gMap.Version = 20;
    gMap.Name[0] = 0;
    gMap.StartHexGrid = 20100;
    ObjClear(); // remove gObjDude
    AnimReset();
    if( gMapVars ){
        Free( gMapVars );
        gMapVars = NULL;
        gMapVarsCnt = 0;
    }
    if( gMapLocalVars ){
        Free( gMapLocalVars );
        gMapLocalVars = NULL;
        gMapLocalVarsCnt = 0;
    }
    MapResetGrid();
    MapUnk01();
    TileUpdate();
}

void MapLoadMapDialog()
{
    char *FilePath, **FileList;
    int n;

    FilePath = MapGetFilePath( "*.map" );
    if( (n = dbGetFileList( FilePath, &FileList ) ) ){
        if( (gMapUnk43 = TextBoxSelect( "Select a map to load:", FileList, n, 0, 80, 80, gPalColorCubeRGB[15][15][15], gMapUnk43 )) != -1 ){
            MapLoadMAP( FileList[ gMapUnk43 ] );
            WmStartMapMusic();
        }
        dbDelFileList( FileList );
    }
}

int MapLoadMAP( char *fname )
{
    int err = -1;
    char *s;
    xFile_t *fh;

    StrUpr( fname );
    if( (s = strstr( fname, ".MAP" ) ) ){ 
	// check if the file *.SAV exist
        strcpy( s, ".SAV" ); // *.MAP -> *.SAV
        fh = dbOpen( MapGetFilePath( fname ), "rb" );
        strcpy( s, ".MAP" ); // restore *.MAP
         if( fh ){
            dbClose( fh );
            // if file *.SAV exist, load file *.MAP
            err = MapLoadSAV( fname );
            WmStartMapMusic();
        }
    }
    if( err == -1 ){
        if( (fh = dbOpen( MapGetFilePath( fname ), "rb") ) ){
            err = MapLoadMapFile( fh );
            dbClose( fh );
        }
        if( !err ){
            strcpy( gMap.Name, fname );
            gObjDude->Critter.State.WhoHitMe = 0;
        }
    }
    return err;
}

int MapOpenById( int MapIdx )
{
    int err;
    char stmp[ 24 ];

    ScptSetArg( gMapScriptId, MapIdx );
    if( WmGetMapFileName( MapIdx, stmp ) == -1 ) return -1;    

    gMapUnk03 = MapIdx;
    err = MapLoadMAP( stmp );
    WmStartMapMusic();
    return err;
}
    
int MapLoadMapFile( xFile_t *fh )
{
    Obj_t *p;
    Scpt_t *scr;
    int err, MseCursor;
    char *errmsg, *v14, stmp1[80], stmp2[80];

    MapSavingRandomEncounter( 1 );
    GSoundLoadBg( "wind2", 12, 13, 16 );
    MapAmbientDisable();
    PartyLoad();//-->
    GmouseScrollDisable();
    MseCursor = GmouseGetCursorId();
    GmouseLoadCursor( 25 );
    dbSetRWFunc( GmouseProcess, 0x8000 );
    TileUpdateDisable();
    err = 0;
    // clear screen
DD
    WinDrawFilledRect( gMapIsoWin, 0, 0, gVidMainGeo.rt - gVidMainGeo.lt + 1, gVidMainGeo.bm - gVidMainGeo.tp - 99, gPalColorCubeRGB[0][0][0] );
    WinUpdate( gMapIsoWin );
    AnimReset();
    ScptDisable();
    gMapScriptId = -1;
    errmsg = "Invalid file handle";
    if( !fh ) goto Error;
    // load header
    if( MapLoadHdr( &gMap, fh ) ) goto Error;
    errmsg = "Invalid map version";
    if( gMap.Version != 20 && gMap.Version != 19 ) goto Error;
    if( gMapIsoPlayerElev == -1 ){
        gMapIsoPlayerElev = gMap.MapLvl;
        gMapIsoUnk04 = gMap.StartHexGrid;
        gMapIsoUnk05 = gMap.PlayerOrientation;
    }
    ObjClear();
    if( gMap.VarsCnt < 0 ) gMap.VarsCnt = 0;
    if( gMap.LocVarsCnt < 0 ) gMap.LocVarsCnt = 0;
DD
// load variables
    MapFreeVars();
    if( gMap.VarsCnt == 0 ){
        gMapVarsCnt = 0;
    } else {
        if( !(gMapVars = Malloc( gMap.VarsCnt * sizeof( int ) )) ) goto Error;
        gMapVarsCnt = gMap.VarsCnt;
    }
    if( dbreadBeiBlk( fh, gMapVars, gMapVarsCnt ) != 0 ) goto Error;
    // load local variables
    MapFreeLocalVars();
    if( gMap.LocVarsCnt == 0 ){
        gMapLocalVarsCnt = 0;
    } else {
        if( !(gMapLocalVars = Malloc( gMap.LocVarsCnt * sizeof( int ) )) ) goto Error;
        gMapLocalVarsCnt = gMap.LocVarsCnt;
    }
    if( dbreadBeiBlk( fh, gMapLocalVars, gMapLocalVarsCnt ) != 0 ) goto Error;
// load tiles into grid
    if( MapLoadGrid( fh, gMap.MapFlags ) ) goto Error;
// load script for map
    if( ScptLoadScript( fh ) ) goto Error;
    if( ObjLoadMapObjs( fh ) ) goto Error;
    if( !(gMap.MapFlags & MAPFLG_SAV) ) MapUnk07(); // not savegame map
    if( MapSetLvl( gMapIsoPlayerElev) ) goto Error;
    if( TileSetCenter(gMapIsoUnk04, 2) ) goto Error;
    ItemMapSetLight( 0x10000, 0 );
    ObjMoveToTile( gObjDude, gTileCentIdx, gCurrentMapLvl, NULL );
    ObjSetRotation(gObjDude, gMapIsoUnk05, 0);
    gMap.MapId = WmGetMapIdxByFileName( gMap.Name );
DD
    if( !(gMap.MapFlags & MAPFLG_SAV) ){
        sprintf( stmp2, "maps/%s", gMap.Name );            
        if( (v14 = strstr( stmp2, ".MAP" )) ){
	*v14 = '\0';
        } else {        	
    	if( (v14 = strstr(stmp2, ".map")) ) *v14 = '\0';
        }
        strcpy( &stmp2[strlen(stmp2)], ".GAM" );
        GlobVarLoadFile(stmp2, "MAP_GLOBAL_VARS:", &gMapVarsCnt, &gMapVars);
        gMap.VarsCnt = gMapVarsCnt;
    }
    ScptEnable();
    errmsg = NULL;
    if( gMap.ScriptId > 0 && ScptNewScript( &gMapScriptId, 0 ) == -1 ) goto Error;
printf("MapScriptId:%x\n", gMapScriptId);
    p = NULL;
    ObjCreate( &p, ArtMakeId( 5, 12, 0, 0, 0 ), -1 ); // scrblk.frm
    p->Flags |= 0x20000005; // PRFLG_LIGHTTHROU
    ObjMoveToTile( p, 1, 0, NULL );
    p->ScrId = gMapScriptId;
    ScptSetArg( gMapScriptId, (gMap.MapFlags & 0x01) == 0 );
    ScptPtr( gMapScriptId, &scr );
    scr->LocVarId = gMap.ScriptId - 1;
    scr->Flags |= 0x08;
    p->TimeEv = ScptNewObjId();
    scr->i08 = p->TimeEv;
    scr->TimeEv = p;
SCP_DBG_EN;
DD
    ScptUnk23();
    ScptRun( gMapScriptId, SCPT_AEV_MAP_ENTER_P_PROC );
    ScptUnk22();
//SCP_DBG_DIS;
    if( WmSetupRandomEncounter() == -1 ) errmsg = "Error Setting up random encounter";
Error:
    if( errmsg ){
        sprintf( stmp1, "%s while loading map.", errmsg );
        eprintf( "%s", stmp1 );
        err = -1;
        MapUnk09();
    } else {
        ObjUnk80( gMap.MapFlags );
    }
DD
    PartyRecoverLoad();
    IfacePanelDisable();
    ProtoDudeImgInit();
    MapUnk01();
    dbSetRWFunc( NULL, 0 );
    MapAmbientEnable();
    GmouseScrollDisable();
    GmouseLoadCursor( 25 );
    if( ScptLoadAllScripts() == -1 ) eprintf( "\n   Error: scr_load_all_scripts failed!" );
    ScptMapEnter();
    ScptMapUpdate();
    TileUpdateEnable();
DD
    if( gMapCurrentPos.MapId > 0 ){

        if( gMapCurrentPos.Orientation >= 0 ) ObjSetRotation( gObjDude, gMapCurrentPos.Orientation, 0 );
    } else {

        TileUpdate();
    }
    ScptClockInit();
    if( GSoundMapInit() == -1 ) err = -1;
    WmUnk41( gMap.MapId );
    WmUnk45( gMap.MapId, gCurrentMapLvl, 1 );
    if( WmUnk47() ) err = -1;
    dbSetRWFunc( NULL, 0 );
    if( !GameIfaceStat() ) GmouseScrollEnable();
    GmouseLoadCursor( MseCursor );
DD
    gMapIsoPlayerElev = -1;
    gMapIsoUnk04 = -1;
    gMapIsoUnk05 = -1;
    GMovieFade();
    gMap.Version = 20;
DD
    return err;
}

int MapLoadSAV( char *fname )
{
    int err;
    char stmp2[ 16 ];
    char stmp1[ 24 ];

    eprintf( "\nMAP: Loading SAVED map." );
    CharEditFnameChgExt( stmp2, fname, "sav" );
    err = MapLoadMAP( stmp2 );
    if( ScptGetGameDekaSeconds() >= gMap.Time ){
	if( ((ScptGetGameDekaSeconds() - gMap.Time) / 3600 ) >= 24 ) UseUnjamAll();
	if( MapClearKilled() == -1 ){ eprintf( "\nError: Critter aging failed on map load!" ); return -1; }
    }
    if( !WmIsCurrentMapMapSaved() ){
        eprintf( "\nDestroying RANDOM encounter map." );
        strcpy( stmp1, gMapCurrentFName );
        CharEditFnameChgExt( gMapCurrentFName, stmp1, "SAV" );
        LsgDeleteFile( "maps/", gMapCurrentFName );
        strcpy( gMapCurrentFName, stmp1 );
    }
    return err;    
}

int MapClearKilled()
{
    Proto_t *proto;
    Obj_t *dude, **olist, *p, *obj, *a1;
    int Pid_high,tmp,ObjAllocated,cnt,v15,err,v20,i;

    if( !WmIsCurrentMapDeadBodyAge() ) return 0;
    tmp = ( ScptGetGameDekaSeconds() - gMap.Time ) / 3600;
    if( !tmp ) return 0;
    for( dude = ObjGetObjectFirst(); dude; dude = ObjGetObjectNext() ){
        if( OBJTYPE( dude->Pid ) != TYPE_CRIT ) continue;
        if( dude == gObjDude ) continue;
        if( PartyMembRdy( dude ) ) continue;
        if( CritterIsDead( dude ) ) continue;
        dude->Grid.DestMapId &= ~0x04;
        if( CritterGetGender( dude ) != 10 && !CritterGetInjure( dude->Pid, 512 ) ) CritterHit1( dude, tmp );
    }
    v20 = 0;
    if( tmp > 144 ) v20 = 1;    
    if( tmp > 336 ) v20 = 2;    
    if( !v20 ) return 0;
    
    ObjAllocated = 100;    
    cnt = 0;
    olist = Malloc( sizeof(Obj_t *) * 100 );
    for( p = ObjGetObjectFirst(); p; p = ObjGetObjectNext() ){
        Pid_high = OBJTYPE( p->Pid );
        if( Pid_high == 1 ){
            if( p != gObjDude ){
                if( CritterIsDead( p ) ){
                    if( CritterGetGender( p ) != 10 && !CritterGetInjure( p->Pid, 256 ) ){
                        olist[ cnt++ ] = p;
                        if( cnt >= ObjAllocated ){
                            ObjAllocated *= 2;
                            if( !(olist = Realloc( olist, ObjAllocated * sizeof( Obj_t * ) )) ){ eprintf( "\nError: Out of Memory!" ); return -1; }
                        }
                    }
                }
            }
        } else if( v20 == 2 && Pid_high == 5 && p->ImgId == 0x500000B ){
            olist[ cnt++ ] = p;
            if( cnt >= ObjAllocated ){
                ObjAllocated *= 2;                
                if( !(olist = Realloc( olist, ObjAllocated * sizeof( Obj_t * ) )) ){ eprintf( "\nError: Out of Memory!" ); return -1; }
            }
        }
    }    
    err = 0;
    for( i = 0; i < cnt; i++ ){
        obj = olist[ i ];
        if( OBJTYPE( obj->Pid ) == TYPE_CRIT ){
	    if( !CritterGetInjure( obj->Pid, 64 ) ) Item18( obj, obj->GridId );
	    if( ObjCopy( &a1, 0x5000004 ) == -1 ){ err = -1; break; }	    
    	    ObjMoveToTile( a1, obj->GridId, obj->Elevation, 0 );
    	    v15 = RandMinMax( 0, 3 );
    	    ProtoGetObj( obj->Pid, &proto );
    	    if( obj->Flags & 0x800 ){
    	        v15 += 6;
    	    }else if( CritterGetGender( obj ) != 7 && CritterGetGender( obj ) != 12 ){
        	v15 += 3;
    	    }
    	    ObjSetFrame( a1, v15, 0 );
        }
        AnimClear( obj );
        ObjDestroy( obj, 0 );
    }
    Free( olist );
    return err;
}

int MapGetAreaByEntrance()
{
    int Area = -1;

    if( WmFindAreaByEntranceId(gCurrentMapId, &Area) == -1 ) return -1;
    return Area;
}

int MapSetPos( Map01_t *p )
{
    if( !p ) return -1;
    gMapCurrentPos = *p;
    if( gMapCurrentPos.MapId == 0 ) gMapCurrentPos.MapId = -2;
    if( IN_COMBAT ) gMenuEscape = 1;
    return 0;
}

int MapJump()
{
    int v1;

    if( gMapCurrentPos.MapId == 0 ) return 0;
    GmouseUnk03();
    GmouseLoadCursor( 0 );
    if( gMapCurrentPos.MapId == -1 ){
        if( !IN_COMBAT ){
            AnimReset();
            WmUnk10( 0 );
            memset( &gMapCurrentPos, 0, sizeof( gMapCurrentPos ) );
        }
    } else if( gMapCurrentPos.MapId == -2 ){
        if( !IN_COMBAT ){
            AnimReset();
            WmMenu();
            memset( &gMapCurrentPos, 0, sizeof( gMapCurrentPos ) );
            return 0;
        }
    } else {
        v1 = -1;
        if( !IN_COMBAT ){
            if( gMapCurrentPos.MapId != gMap.MapId || gCurrentMapLvl == gMapCurrentPos.Frame ) MapOpenById( gMapCurrentPos.MapId );
            if( gMapCurrentPos.PosY != -1 && gMapCurrentPos.PosY && gMap.MapId != 19 && gMap.MapId != 37 && gMapCurrentPos.Frame <= 2 ){
                ObjMoveToTile( gObjDude, gMapCurrentPos.PosY, gMapCurrentPos.Frame, 0 );
                MapSetLvl( gMapCurrentPos.Frame );
                ObjSetRotation( gObjDude, gMapCurrentPos.Orientation, 0 );
            }
            if( TileSetCenter( gObjDude->GridId, 1 ) == -1 ) eprintf( "\nError: map: attempt to center out-of-bounds!" );
            memset( &gMapCurrentPos, 0, sizeof( gMapCurrentPos ) );
            WmFindAreaByEntranceId( gMap.MapId, &v1 );
            if( WmSetArea( v1 ) == -1 ) eprintf( "\nError: couldn't make jump on worldmap for map jump!" );
        }
    }
    return 0;
}

void MapUnk07()
{
    Obj_t *obj;

    for( obj = ObjGetObjectFirst(); obj; obj = ObjGetObjectNext() ){
        if( obj->Pid != -1 && OBJTYPE( obj->Pid ) == TYPE_CRIT ){
            if( obj->Critter.State.WhoHitMe == -1 ) obj->Critter.State.WhoHitMe = 0;
        }        
    }
}

int MapSetMapFname()
{
    char stmp[ 28 ];

    stmp[ 0 ] = '\0';
    if( TextBoxDialogEdit1( stmp, 8, "Save file (no extension):", 80, 80 ) ) return -1;
    strcpy( stmp + strlen( stmp ), ".map" );
    strcpy( gMapCurrentFName, stmp );
    MapMapSave();
    return 0;
}

int MapMapSave()
{
    int err;
    char *FilePath, stmp[80], *patches;
    xFile_t *fh;

    stmp[ 0 ] = '\0';
    err = -1;
    if( CfgGetString( &gConfiguration, "system", "master_patches", &patches ) == 1 ){
        strcpy( stmp + strlen( stmp ), patches );
        xDirCreate( stmp );
        strcpy( stmp + strlen( stmp ), "/MAPS" );
        xDirCreate(stmp);
    }
    if( !gMapCurrentFName[0] ){ eprintf( "\nError: map_save: map header corrupt!" ); return -1; }
    FilePath = MapGetFilePath(gMapCurrentFName);    
    if( (fh = dbOpen(FilePath, "wb")) ){
	err = MapSaving( fh );
        dbClose( fh );
    } else {
        eprintf( "Unable to open %s to write!", gMapCurrentFName );
    }
    if( !err ){
        eprintf( "%s saved.", gMapCurrentFName );
    }
    return err;
}

int MapSaving( xFile_t *fh )
{
    Obj_t *VisibleObjectFirst, *VisibleObjectNext, *obj;
    int idx, lvl;
    char stmp1[80], stmp2[80];

    if( !fh ) return -1;
    ScptDisable();    
    for( lvl = 0; lvl < 3; lvl++ ){
        for( idx = 0; idx < 10000; idx++ ){
            if( ArtMakeId( 4, gMapIsoGrid[ lvl ][ idx ] & 0xFFF, 0, 0, 0 ) != ArtMakeId( 4, 1, 0, 0, 0 ) ) break;
            if( ArtMakeId( 4, OBJTYPE( gMapIsoGrid[ lvl ][ idx ] ) & 0xFFF, 0, 0, 0 ) != ArtMakeId( 4, 1, 0, 0, 0 ) ) break;
        }        
        if( idx >= 10000 ){
            VisibleObjectFirst = ObjGetVisibleObjectFirst( lvl );            
            if( VisibleObjectFirst ){
                for( obj = VisibleObjectFirst; obj && VisibleObjectNext->Flags & 0x04; obj = VisibleObjectNext ){
                    VisibleObjectNext = ObjGetVisibleObjectNext();                    
                }
                if( obj ){
                    if( !(obj->Flags & 0x04) ) gMap.MapFlags &= ~gMapGridFlags[ lvl ];
                } else {
                    gMap.MapFlags |= gMapGridFlags[ lvl ];
                }
            } else {
                gMap.MapFlags |= gMapGridFlags[ lvl ];
            }
        } else {
            gMap.MapFlags &= ~gMapGridFlags[ lvl ];
        }        
    }
    gMap.LocVarsCnt = gMapLocalVarsCnt;
    gMap.VarsCnt = gMapVarsCnt;
    gMap.Darkness = 1;
    MapSaveHdr( &gMap, fh );
    if( gMap.VarsCnt ) dbputBeiBlk( fh, gMapVars, gMap.VarsCnt );
    if( gMap.LocVarsCnt ) dbputBeiBlk( fh, gMapLocalVars, gMap.LocVarsCnt );
    for( lvl = 0; lvl < 3; lvl++ ){
        if( (gMap.MapFlags & gMapGridFlags[ lvl ] ) == 0 ) dbputLeiBlk( fh, gMapIsoGrid[ lvl ], 10000 );
    }
    if( ScptSaveScript( fh ) == -1 ){
        sprintf( stmp2, "Error saving scripts in %s", gMap.Name );
        TextBoxUnk12( stmp2, 80, 80, gPalColorCubeRGB[31][0][0] );
    }
    if( ObjSaveUnk07( fh ) == -1 ){
        sprintf( stmp1, "Error saving objects in %s", gMap.Name );
        TextBoxUnk12( stmp1, 80, 80, gPalColorCubeRGB[31][0][0] );
    }
    ScptEnable();
    return 0;
}

int MapSaveMap( char *fName )
{
    strcpy( gMapCurrentFName, fName );
    return MapMapSave();
}

int MapSavingRandomEncounter( int a1 )
{
    char stmp[ 16 ];
    Scpt_t *res;

    if( !gMap.Name[0] ) return 0;
    AnimReset();
    PartySave();
    if( a1 & 0x01 ){
        EvQeRunAll();
DD
        PartyLoad();
DD
        PartySaveBox();
DD
        ScptMapExit();
        if( gMapScriptId != -1 ) ScptPtr( gMapScriptId, &res );
        ScptClockInit();
        ObjUnk19();
    }
    gMap.MapFlags |= 0x01;
    gMap.Time = ScptGetGameDekaSeconds();
    if ( (a1 & 0x01) && !WmIsCurrentMapMapSaved() ){
        eprintf( "\nNot saving RANDOM encounter map." );
        strcpy( stmp, gMap.Name );
        CharEditFnameChgExt( gMap.Name, stmp, "SAV" );
        LsgDeleteFile( "MAPS/", gMap.Name );
        strcpy( gMap.Name, stmp );
    } else {
	eprintf( "\n Saving \".SAV\" map." );
	strcpy( stmp, gMap.Name );
        CharEditFnameChgExt( gMap.Name, stmp, "SAV" );
	if( MapMapSave() == -1 ) return -1;
	strcpy( gMap.Name, stmp );
	AutomapSave();
    }
    if( a1 & 1 ){
        gMap.Name[ 0 ] = '\0';
        ObjClear();
        ProtoResetTypes();
        MapResetGrid();
        ScptClockInit();
    }
    return 0;
}

int MapUnk08()
{
    char stmp[ 260 ], *tmp;

    tmp = NULL;
    if( CfgGetString( &gConfiguration, "system", "master_patches", &tmp ) == 1 ){
	strcpy( stmp, tmp );
    } else {
        strcpy( stmp, "DATA" );
    }
    xDirCreate( stmp );
    strcpy( stmp + strlen( stmp ), "/maps" );
    xDirCreate( stmp );
    return 0;
}

void MapUpdateArea( VidRect_t *Area )
{
    WinAreaUpdate( gMapIsoWin, Area );
}

void MapRedrawIso( VidRect_t *Area )
{
    VidRect_t rect;
    
    if( RegionShrink( Area, &gMapIsoGeo, &rect ) == -1 ) return;
    TileRenderFloor( &rect, gCurrentMapLvl );
    TileRenderSketch( &rect, gCurrentMapLvl );
    ObjRenderObjects( &rect, gCurrentMapLvl );
    TileRenderRoof( &rect, gCurrentMapLvl );
    ObjRenderCursor( &rect );    
}

void MapMapperCb( VidRect_t *Area )
{
    VidRect_t rect;

    if( RegionShrink( Area, &gMapIsoGeo, &rect) != -1 ){
        ScrFillSolid( 
    	    (char *)(rect.lt + gMapIsoSurf + (gVidMainGeo.rt - gVidMainGeo.lt + 1) * rect.tp), 
    	    rect.rt - rect.lt + 1, rect.bm - rect.tp + 1, gVidMainGeo.rt - gVidMainGeo.lt + 1, 0
    	);
        TileRenderFloor( &rect, gCurrentMapLvl );
        TileRenderSketch( &rect, gCurrentMapLvl);
        ObjRenderObjects( &rect, gCurrentMapLvl );
        TileRenderRoof( &rect, gCurrentMapLvl );
        ObjRenderCursor( &rect );
    }
}

int MapAllocVars( int num )
{
    if( gMapVars ){
        Free( gMapVars );
        gMapVars = NULL;
        gMapVarsCnt = 0;
    }
    if( num ){
        gMapVars = Malloc( num * sizeof( int ) );
        if( !gMapVars ) return -1;
    }
    gMapVarsCnt = num;
    return 0;
}

void MapFreeVars()
{
    if( !gMapVars ) return;    
    Free(gMapVars);
    gMapVars = NULL;
    gMapVarsCnt = 0;
}

int MapLoadVars( xFile_t *fh )
{
    return ( dbreadBeiBlk( fh, gMapVars, gMapVarsCnt ) == 0 ) - 1;
}

int MapAllocLocalVars( int num )
{
    if( gMapLocalVars ){
        Free( gMapLocalVars );
        gMapLocalVars = NULL;
        gMapLocalVarsCnt = 0;
    }
    if( num ){
        gMapLocalVars = Malloc( num * sizeof( int ) );
        if( !gMapLocalVars ) return -1;
    }
    gMapLocalVarsCnt = num;
    return 0;
}

void MapFreeLocalVars()
{
    if ( !gMapLocalVars ) return;    
    Free(gMapLocalVars);
    gMapLocalVars = NULL;
    gMapLocalVarsCnt = 0;    
}

int MapLoadLocalVars( xFile_t *fh )
{
    return ( dbreadBeiBlk( fh, gMapLocalVars, gMapLocalVarsCnt ) == 0 ) - 1;
}

void MapUnk01()
{
    ObjUnk78();
    if( gObjDude ){
        if( (gObjDude->ImgId & 0xFF0000) >> 16 ){
            ObjSetFrame( gObjDude, 0, 0 );
            gObjDude->ImgId = ArtMakeId(1, gObjDude->ImgId & 0xFFF, 0, (gObjDude->ImgId & 0xF000) >> 12, gObjDude->Orientation + 1);
        }
        if( gObjDude->GridId == -1 ){
            ObjMoveToTile( gObjDude, gTileCentIdx, gCurrentMapLvl, 0 );
            ObjSetRotation( gObjDude, gMap.PlayerOrientation, 0 );
        }
        ObjSetLight( gObjDude, 4, 0x10000, NULL );
        gObjDude->Flags |= 0x04;
        AnimUnk24( gObjDude, gObjDude->Orientation, gObjDude->ImgId );
        PartyUnk07();
    }
    GmouseSetHexCursorBlank();
    GmouseIsoEnter();
}

void MapIsoGridInit()
{
    int i;
    for( i = 0; i != 3; i++ ) gMapIsoGrid[ i ] = &gMapIsoGridData[ i * 100 * 100 ];
}

void MapResetGrid()
{
    int x,idx,lvl,y;

    for( lvl = 0; lvl != 3; lvl++ ){
        idx = 0;
        for( y = 0; y < 100; y++ ){
            for( x = 0; x < 100; x++, idx++ ){
                gMapIsoGrid[ lvl ][ idx ] |= (((ArtMakeId( 4, 1, 0, 0, 0 ) & 0xFFF) | (((gMapIsoGrid[ lvl ][ idx ] >> 16) & 0xF000) >> 12)) << 16);
                gMapIsoGrid[ lvl ][ idx ] = (ArtMakeId( 4, 1, 0, 0, 0 ) & 0xFFF) | ((gMapIsoGrid[ lvl ][ idx ] & 0xF000u) >> 12) | (((gMapIsoGrid[ lvl ][ idx ] & 0xffff0000) >> 24) << 16);
            }
        }
    }
}

int MapLoadGrid( xFile_t *fh, int Flags )
{
    int lvl, i;
    unsigned int v6;

    MapResetGrid();
    for( lvl = 0; lvl < 3; lvl++ ){
        if( ( Flags & gMapGridFlags[ lvl ] ) ) continue;
        if( dbreadBeiBlk( fh, gMapIsoGrid[ lvl ], 100*100 ) ) return -1;
        for( i = 0; i != 10000; i++ ){
            v6 = (gMapIsoGrid[ lvl ][ i ] >> 16);
            gMapIsoGrid[ lvl ][ i ] |= (((v6 & 0xFFF) | ((((v6 & 0xF000) >> 12) & 0xFE) << 12)) << 16);
        }
    }
    return 0;
}

int MapSaveHdr( Map_t *map, xFile_t *fh )
{
    if( dbputBei( fh, map->Version ) == -1 ) return -1;
    if( dbputbBlk( fh, map->Name, 16 ) == -1 ) return -1;
    if( dbputBei( fh, map->StartHexGrid ) == -1 ) return -1;
    if( dbputBei( fh, map->MapLvl ) == -1 ) return -1; 
    if( dbputBei( fh, map->PlayerOrientation ) == -1 ) return -1; 
    if( dbputBei( fh, map->LocVarsCnt ) == -1 ) return -1; 
    if( dbputBei( fh, map->ScriptId ) == -1 ) return -1; 
    if( dbputBei( fh, map->MapFlags ) == -1 ) return -1; 
    if( dbputBei( fh, map->Darkness ) == -1 ) return -1; 
    if( dbputBei( fh, map->VarsCnt ) == -1 ) return -1; 
    if( dbputBei( fh, map->MapId ) == -1 ) return -1; 
    if( dbputBei( fh, map->Time ) == -1 ) return -1; 
    if( dbputBeiBlk( fh, map->Filler, 44 ) == -1 ) return -1;
    return 0;
}

int MapLoadHdr( Map_t *map, xFile_t *fh )
{
    if( dbgetBei( fh, &map->Version ) == -1 ) return -1;
    if( dbreadByteBlk( fh, map->Name, 16 ) == -1 ) return -1;
    if( dbgetBei( fh, &map->StartHexGrid ) == -1 ) return -1;
    if( dbgetBei( fh, &map->MapLvl ) == -1 ) return -1; // map lvl
    if( dbgetBei( fh, &map->PlayerOrientation ) == -1 ) return -1; // player orientation
    if( dbgetBei( fh, &map->LocVarsCnt ) == -1 ) return -1;
    if( dbgetBei( fh, &map->ScriptId ) == -1 ) return -1; // script id
    if( dbgetBei( fh, &map->MapFlags ) == -1 ) return -1; // map flags
    if( dbgetBei( fh, &map->Darkness ) == -1 ) return -1; // darkness
    if( dbgetBei( fh, &map->VarsCnt ) == -1 ) return -1; 
    if( dbgetBei( fh, &map->MapId ) == -1 ) return -1; // map id
    if( dbgetBei( fh, &map->Time ) == -1 ) return -1;  // time

    if( dbreadBeiBlk( fh, map->Filler, 44 ) == -1 ) return -1; // ??
    return 0;
}

