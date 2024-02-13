#include "FrameWork.h"

int gObjUnk52[ 6 ][ 36 ];
char gObjLight[ 2*6*8*18 ];

VidRect_t gObjUnk16;
Obj_t *gObjUnk27[100];

VidRect_t gObjViewPortArea;

ObjList_t *gObjGridObjects[ 200*200 ];
char gObjPalRY[256];
char gObjPalBY[256];
int gObjUnk17;
char *gObjIsoSurface;
int gObjUnk14;
Obj_t *gObjRadius; // light player object 
int gObjIsoPitch;
int gObjUnk13;
Obj_t *gObjDude;
char gObjUnk79[ 5001 ];
char gObjUnk01[5003];

int gObjUnk20 = 0;
int gObjGridWinWidth = 0;
int gObjGridWinHeight = 0;
unsigned int gObjGridWinSize = 0;
int *gObjUnk11[ 2 ] = {NULL,NULL};
int *gObjViewPort[ 2 ] = {NULL,NULL};
int *gObjViewPortAxisY = NULL;
int *gObjViewPortAxisX = NULL;
ObjList_t **gObjRenderList = NULL;
int gObjUnk28 = 0;
ObjList_t *gObjOffGridObjs = NULL;
int gObjUnk12 = 0;
int gObjMapObjLvl = 0;
int gObjMapObjIdx = 0;
ObjList_t *gObjMapObj = NULL;
int *gObjArtTable = NULL;
unsigned int gObjCnt = 0;
int gObjUnk41 = 0;
int gObjUnk40 = -1;
VidRect_t gObjUnk39[ 9 ] = { 
    { 0, 0, 0x60, 0x2A },
    { 0, 0, 0xA0, 0x4A },
    { 0, 0, 0xE0, 0x6A },
    { 0, 0, 0x20, 0x8A },
    { 0, 0, 0x60, 0xAA },
    { 0, 0, 0xA0, 0xCA },
    { 0, 0, 0xE0, 0xEA },
    { 0, 0, 0x20, 0x0A },
    { 0, 0, 0x60, 0x2A }
};

int gObjUnk38[ 36 ] = {
	1, 2, 3, 4, 5, 6, 7, 8, 2, 3, 4, 5, 6, 7, 8, 3, 4, 5,
	6, 7, 8, 4, 5, 6, 7, 8, 5, 6, 7, 8, 6, 7, 8, 7, 8, 8
};
int gObjViolLvl = -1;
int gObjPlayerMapX = -1;
int gObjPlayerMapY = -1;
int gObjPlayerMapLvl = -1;
int gObjUnk36 = 1;
char *gObjShadeWall = NULL;
char *gObjShadeGlass = NULL;
char *gObjShadeSteam = NULL;
char *gObjShadeEnergy = NULL;
char *gObjShadeRed = NULL;
Obj_t *gObjUnk42 = NULL;
int gObjUnk43 = 0;
int gObjUnk37[4] = { 1, 0, 3, 5 };

int gObjUnk21;


int ObjInit( char *a1, int Width, int Height, int Pitch )
{
    memset( gObjUnk01, 0, 5001 );
    gObjViewPortArea.rt = Width + 320;
    gObjViewPortArea.lt = -320;
    gObjViewPortArea.bm = Height + 240;
    gObjViewPortArea.tp = -240;
    gObjGridWinWidth = ( Width  + 641 ) / 32 + 1;
    gObjGridWinHeight = ( Height + 481 ) / 12 + 1;
    gObjGridWinSize = gObjGridWinWidth * gObjGridWinHeight;
    memset( gObjGridObjects, 0, sizeof( gObjGridObjects ) );
    
    if( ObjViewPortInit() == -1 ) return -1;
    if( ObjUnk84() == -1 ){ ObjFree(); return -1; }
    if( ObjAllocRenderList() == -1 || ItemGridLightInit() == -1 || TextInit( a1, Width, Height ) == -1 ){
        if( gObjUnk11[ 1 ] ){ Free( gObjUnk11[ 1 ] ); gObjUnk11[ 1 ] = NULL; }
        if( gObjUnk11[ 0 ] ){ Free( gObjUnk11[ 0 ] ); gObjUnk11[ 0 ] = NULL; }
        ObjFree();
        return -1;
    }
    ObjUnk90();
    ObjColorInit();
    gObjUnk12 = TileGetPointed( gObjViewPortArea.lt, gObjViewPortArea.tp ) - gObjUnk21;
    gObjUnk13 = Width;
    gObjUnk14 = Height;
    gObjIsoSurface = a1;
    gObjUnk16.lt = 0;
    gObjUnk16.tp = 0;
    gObjUnk16.rt = Width - 1;
    gObjUnk17 = Height * Width;
    gObjIsoPitch = Pitch;
    gObjUnk16.bm = Height - 1;
    ObjCreate( &gObjDude, ArtMakeId(1, gArtPrep[0], 0, 0, 0), 0x1000000 ); // main character create
    gObjDude->Flags |= 0x20000405;
    ObjSetLight( gObjDude, 4, 0x10000, 0 );
    if( PartyAddMember( gObjDude ) == -1 ){ eprintf( "\n  Error: Can't add Player into party!" ); exit(1); }
    ObjCreate( &gObjRadius, ArtMakeId( 6, 2, 0, 0, 0), -1 ); // light spot on player
    gObjRadius->Flags |= 0x20000405;
    gObjUnk20 = 1;
    return 0;
}

void ObjReset()
{
    if( !gObjUnk20 ) return;
    TextFlush();
    ObjClear();
    memset( gObjUnk01, 0, 5001 );
    ItemGridLightReset();
}

void ObjClose()
{
    if( !gObjUnk20 ) return;
    gObjDude->Flags &= ~0x4000000;
    gObjRadius->Flags &= ~0x4000000;
    ObjClear();
    TextClose();
    PalExtClose( gPalColorCubeRGB[24][26][31] );
    PalExtClose( gPalColorCubeRGB[9][31][31] );
    PalExtClose( gPalColorCubeRGB[31][31][31] );
    PalExtClose( gPalColorCubeRGB[29][31][1] );
    PalExtClose( gPalColorCubeRGB[31][0][0] );
    ItemGridLightReset();
    if( gObjRenderList ){ Free( gObjRenderList ); gObjRenderList = NULL; }
    if( gObjUnk11[1] ){ Free( gObjUnk11[1] ); gObjUnk11[1] = NULL; }
    if( gObjUnk11[0] ){ Free( gObjUnk11[0] ); gObjUnk11[0] = NULL; }
    ObjFree();
}

int ObjLoadObj( Obj_t *obj, xFile_t *fh )
{
    if( dbgetBei( fh, &obj->TimeEv ) == -1 ) return -1;		// 00 Id for new created objects ? In game Time for event ?
    if( dbgetBei( fh, &obj->GridId ) == -1 ) return -1;		// 04 hex grid id 
    if( dbgetBei( fh, &obj->PosX ) == -1 ) return -1; 		// 08 x ?
    if( dbgetBei( fh, &obj->PosY ) == -1 ) return -1; 		// 0C y ?
    if( dbgetBei( fh, &obj->Sx ) == -1 ) return -1; 		// 10 sx ?
    if( dbgetBei( fh, &obj->Sy ) == -1 ) return -1; 		// 14 sy ?
    if( dbgetBei( fh, &obj->FrameNo ) == -1 ) return -1; 	// 18 frame number
    if( dbgetBei( fh, &obj->Orientation ) == -1 ) return -1;	// 1C orientation
    if( dbgetBei( fh, &obj->ImgId ) == -1 ) return -1; 		// 20 Art PID
    if( dbgetBei( fh, &obj->Flags ) == -1 ) return -1; 		// 24 flags PRFLG_*
    if( dbgetBei( fh, &obj->Elevation ) == -1 ) return -1; 	// 28 Map Lvl
    if( dbgetBei( fh, &obj->Pid ) == -1 ) return -1; 		// 2C object Pid
    if( dbgetBei( fh, &obj->CritterIdx ) == -1 ) return -1; 	// 30 Critter index number
    if( dbgetBei( fh, &obj->LightRadius ) == -1 ) return -1; 	// 34 Light radius
    if( dbgetBei( fh, &obj->LightIntensity ) == -1 ) return -1; // 38 Light intensity
    if( dbgetBei( fh, &obj->OutlineColor ) == -1 ) return -1; 	// 3C Outline color PR_OL_*
    if( dbgetBei( fh, &obj->ScrId ) == -1 ) return -1;		// 40 Map script PID
    if( dbgetBei( fh, &obj->ScrFNameId ) == -1 ) return -1;	// 44 Script filename Id

    obj->OutlineColor = PR_OL_NONE;
    obj->Owner = NULL;

    if( ProtoLoadObj( obj, fh ) ) return -1; 

    if( obj->Pid < 0x5000010 || obj->Pid > 0x5000017 ){
        if( (OBJTYPE( obj->Pid ) == TYPE_ITEM) && !(gMap.MapFlags & 0x01) ) ObjLoad( obj );
    } else if( obj->Container.Ammo <= 0 ){
        if( (obj->ImgId & 0xFFF) < 33 ) obj->ImgId = ArtMakeId(5, (obj->ImgId & 0xFFF) + 16, (obj->Pid & 0xFF0000) >> 16, 0, 0);
    }
    return 0;
}

int ObjLoadMapObjs( xFile_t *fh )
{
    int err;

    err = ObjLoadFile( fh );
    gObjViolLvl = -1;
    return err;
}

int ObjLoadFile( xFile_t *fh )
{
    Scpt_t *scr;
    ObjList_t *Obj;
    ObjCritter_t *p;
    int CntTotal, FixMapInv,pint,i, j, k;

    if( !fh ) return -1;
    FixMapInv = 0;
    if( CfgGetInteger( &gConfiguration, "mapper", "fix_map_inventory", &FixMapInv ) != 1 ) FixMapInv = 0;
    if( CfgGetInteger( &gConfiguration, "preferences", "violence_level", &gObjViolLvl ) != 1 ) gObjViolLvl = 3;
    // create table for all map object images
    if( dbgetBei( fh, &CntTotal ) ) return -1; // total number of ojects on all levels
    if( gObjArtTable ) Free( gObjArtTable );
    if( CntTotal ){
        gObjArtTable = Malloc( CntTotal * sizeof( int ) );
        memset( gObjArtTable, 0, CntTotal * sizeof( int ) );
        if( !gObjArtTable ) return -1;
        gObjCnt = 0;
    }

    for( j = 0; j < 3; j++ ){
	if( dbgetBei( fh, &pint ) ) return -1; // number of objects in this level    
        for( i = 0; i < pint; i++ ){
            if( CntTotal == 0 ) return -1;
            if( !(Obj = Malloc( sizeof( ObjList_t ) ) ) ) return -1;
            Obj->Next = NULL;
            Obj->object = NULL;
            // create obj
            if( ObjNewObj( &Obj->object ) == -1 ){ if( p == 0 && Obj ){ Free( Obj ); Obj = NULL; } return -1; }
	    // load obj
            if( ObjLoadObj( Obj->object, fh ) ){ if( Obj && Obj->object ){ Free( Obj->object ); Obj->object = NULL; } if( p == 0 && Obj ){ Free( Obj ); Obj = 0; } return -1; }
            Obj->object->OutlineColor = 0;
            gObjArtTable[ gObjCnt++ ] = Obj->object->ImgId;

    	    p = &Obj->object->Critter;
    	    // connect script
            if( Obj->object->ScrId != -1 ){
                if( ScptPtr( Obj->object->ScrId, &scr ) == -1 ){
                    eprintf( "\nError connecting object to script Id:0x%x ObjName:'%s'!", Obj->object->ScrId, CritterGetName( Obj->object ) );
                    Obj->object->ScrId = -1;
                } else {
                    scr->TimeEv = Obj->object; // ???
                    Obj->object->ScrFNameId = scr->LocVarId;
                }
            }
            ObjGetArtFileId( &Obj->object->ImgId );
            Obj->object->Elevation = j;
            ObjAddObject( Obj );
            if( ( ( Obj->object->Flags & 0x400 ) != 0 ) && ( OBJTYPE( Obj->object->Pid ) == 1 ) && ( Obj->object->TimeEv != 18000 ) ) Obj->object->Flags &= ~0x4000000;
            if( p->Box.Cnt <= 0 ){
                p->Box.Capacity = 0;
                p->Box.Box = NULL;
            } else {
                if( !(p->Box.Box = Malloc( p->Box.Capacity * sizeof( ObjStack_t ) ) ) ) return -1;
                for( k = 0; k < p->Box.Cnt; k++ ){
                    if( dbgetBei( fh, &p->Box.Box[ k ].Quantity ) ){ eprintf( "Error loading inventory\n" ); return -1; }
                    if( FixMapInv ){
                        p->Box.Box[ k ].obj = Malloc( sizeof( Obj_t ) );
                        if( !p->Box.Box[ k ].obj || ObjLoadObj( p->Box.Box[ k ].obj, fh ) ){ eprintf( "Error loading inventory\n" ); return -1; }
                    } else {
                        if( ObjLoadItems( fh, &p->Box.Box[ k ].obj, j ) == -1 ) return -1;
                    }                        
                }            
            }
        }        
    }
    ObjLightGrid(); 
    return 0;
}

void ObjLookForAgressor()
{
    int ObjCnt, hit, cmb;
    ObjCritterCond_t *inj;
    Obj_t **ObjTable;

    ObjCnt = ObjGetObjList( -1, gCurrentMapLvl, TYPE_CRIT, &ObjTable );
    hit = gObjDude->Critter.State.WhoHitMe;
    inj = &gObjDude->Critter.State;
    if( hit == -1 ){
        gObjDude->Critter.State.WhoHitMe = 0;
    } else {
        cmb = CombatLookForAgressor( 0, hit, ObjTable, ObjCnt );
        inj->WhoHitMe = ( cmb == ObjCnt ) ? 0: ObjTable[ cmb ]->TimeEv; // ??
    }
    if( ObjCnt && ObjTable ) Free( ObjTable );
}

void ObjLoad( Obj_t *dude )
{
    ObjContainer_t *ef;
    Proto_t *proto;

    ef = &dude->Container;
    if( OBJTYPE( dude->Pid ) != TYPE_ITEM ) return;
    proto = NULL;
    if( ProtoGetObj( dude->Pid, &proto ) == -1 ){ eprintf( "\nError: obj_load: proto_ptr failed on pid" ); exit( 1 ); }
    if( ItemGetObjType( dude ) == PR_ITEM_WEAPON ){
        if( ef->AmmoId == 0xCCCCCCCC || ef->AmmoId == -1 ) ef->AmmoId = proto->Critt.BaseStat[14];
        if( ef->Charges == 0xCCCCCCCC || ef->Charges == -1 || ef->Charges != proto->Critt.BaseStat[15] ) ef->Charges = proto->Critt.BaseStat[15];
    } else if( OBJTYPE( dude->Pid ) == TYPE_MISC ){
        if( ef->Charges == 0xCCCCCCCC ){
            ef->Charges = proto->Critt.BaseStat[2];
            if( proto->Critt.BaseStat[2] == 0xCCCCCCCC ){
                eprintf( "\nError: Misc Item Prototype %s: charges incorrect!", ProtoGetObjName( dude->Pid ) );
                ef->Charges = 0;
            }
        } else {
            if( ef->Charges != proto->Critt.BaseStat[2] ) ef->Charges = proto->Critt.BaseStat[2];
        }
    }
}

int ObjSave( Obj_t *Obj, xFile_t *fh )
{
    if( dbputBei( fh, Obj->TimeEv ) == -1 ) return -1;
    if( dbputBei( fh, Obj->GridId ) == -1 ) return -1;
    if( dbputBei( fh, Obj->PosX ) == -1 ) return -1;
    if( dbputBei( fh, Obj->PosY ) == -1 ) return -1;
    if( dbputBei( fh, Obj->Sx ) == -1 ) return -1;
    if( dbputBei( fh, Obj->Sy ) == -1 ) return -1;
    if( dbputBei( fh, Obj->FrameNo ) == -1 ) return -1;
    if( dbputBei( fh, Obj->Orientation ) == -1 ) return -1;
    if( dbputBei( fh, Obj->ImgId ) == -1 ) return -1;
    if( dbputBei( fh, Obj->Flags ) == -1 ) return -1;
    if( dbputBei( fh, Obj->Elevation ) == -1 ) return -1;
    if( dbputBei( fh, Obj->Pid) == -1 ) return -1;
    if( dbputBei( fh, Obj->CritterIdx ) == -1 ) return -1;
    if( dbputBei( fh, Obj->LightRadius ) == -1 ) return -1;
    if( dbputBei( fh, Obj->LightIntensity ) == -1 ) return -1;
    if( dbputBei( fh, Obj->OutlineColor ) == -1 ) return -1;
    if( dbputBei( fh, Obj->ScrId ) == -1 ) return -1;
    if( dbputBei( fh, Obj->ScrFNameId ) == -1 ) return -1;
    if( ProtoSaveDude( Obj, fh ) ) return -1;
    return 0;
}

int ObjSaveUnk07( xFile_t *fh )
{
    ObjList_t *v6;
    Obj_t *WhoHitMe;
    int v11,v15,v20,v21,v25,v26,v27,v28;

    if( !fh ) return -1;
    ObjUnk79();
    v21 = dbtell( fh );
    v25 = 0;
    if( dbputBei(fh, 0) ) return -1;    
    for( v27 = 0; v27 < 3; v25 += v28, v27++ ){
	dbtell( fh );
	v28 = 0;
	if( dbputBei( fh, 0 ) ) return -1;    
	for( v26 = 0; v26 < 40000; v26++ ){
    	    if( (v6 = gObjGridObjects[v26]) ){
		for( ; v6; v6 = v6->Next ){
    		    if( v27 != v6->object->Elevation || (v6->object->Flags & 0x04) ) continue;
    		    if( OBJTYPE( v6->object->Pid ) == 1 ){
        		WhoHitMe = v6->object->Critter.State.WhoHitMeObj;
        		if( v6->object->Critter.State.WhoHitMe ){
            		    if( v6->object->Critter.State.WhoHitMe != -1 ) v6->object->Critter.State.WhoHitMe = v6->object->Critter.State.WhoHitMeObj->CritterIdx;
        		} else {
            		    v6->object->Critter.State.WhoHitMe = -1;
        		}
    		    }
    		    if( ObjSave( v6->object, fh ) ) return -1;
    		    if( OBJTYPE( v6->object->Pid ) == 1 ) v6->object->Critter.State.WhoHitMeObj = WhoHitMe;
		    for( v11 = 0; v11 < v6->object->Critter.Box.Cnt; v11++ ){
			if( dbputBei( fh, v6->object->Critter.Box.Box[ v11 ].Quantity ) ) return -1;
    			if( ObjSaveDude( fh, v6->object->Critter.Box.Box[ v11 ].obj ) == -1 ) return -1;
		    }
    	    	    v28++;
		}    
    	    }        
	}
	v15 = dbtell( fh );
	dbseek( fh, v21, 0 );
	dbputBei( fh, v28 );
	dbseek( fh, v15, 0 );	
    }
    v20 = dbtell( fh );
    dbseek( fh, v21, 0 );
    dbputBei( fh, v25 );
    dbseek( fh, v20, 0 );
    return 0;    
}

void ObjRenderObjects( VidRect_t *Area, int MapLvl )
{
    ObjList_t *list;
    VidRect_t Region;
    int light,v4,v5,LightA,v24,v25,v26,v27;
    int v30,n,*p,i, e,g,AmbLight;

    if( !gObjUnk20 ) return;
    if( RegionShrink( Area, &gObjUnk16, &Region ) ) return;    
    AmbLight = ItemMapGetLight();
    v24 = Region.rt + 320;
    v4 = Region.tp - 240;
    v25 = Region.bm + 240;
    v5 = Region.lt - 320;
    v26 = TileGetPointed( v5, v4 );
    v27 = (v24 - v5 + 1) / 32;
    v30 = (v25 - v4 + 1) / 12;
    n = gObjUnk28 = 0;

    for( i = 0, p = gObjUnk11[ gTileCentIdx & 1 ]; i < gObjGridWinSize; i++ ){
	e = p[ i ];
        if( v30 <= gObjViewPortAxisY[ e ] || v27 <= gObjViewPortAxisX[ e ] ) continue;
        g = gObjViewPort[ gTileCentIdx & 1 ][ e ];
        if( (list = gObjGridObjects[ v26 + g ] ) ){
            LightA = ItemGridGetLightA(MapLvl, list->object->GridId);
            if( LightA >= AmbLight )
                light = ItemGridGetLightA(MapLvl, list->object->GridId);
            else
                light = AmbLight;
        }
        for( ;list; list = list->Next ){ // render flat objects first
            if( MapLvl < list->object->Elevation ) break; // object is above
            if( MapLvl != list->object->Elevation ) continue;
            if( ( list->object->Flags & PRFLG_FLAT ) == 0 ) break;
            if( list->object->Flags & PRFLG_INVISIBLE ) continue;
            ObjRender( list->object, &Region, light );
            if( !(list->object->OutlineColor & 0xFFFFFF) ) continue;
            if( list->object->OutlineColor < 0 ) continue;
            if( gObjUnk28 < 100 ) gObjUnk27[ gObjUnk28++ ] = list->object;
        }
        if( list ) gObjRenderList[ n++ ] = list;
    }
    for( i = 0; i < n; i++ ){
        if( (list = gObjRenderList[ i ] ) ){
            light = AmbLight;
            if( ItemGridGetLightA( MapLvl, list->object->GridId ) >= light ) light = ItemGridGetLightA( MapLvl, list->object->GridId );
        }
        for( ;list; list = list->Next ){
            if( MapLvl < list->object->Elevation ) break;
            if( MapLvl != list->object->Elevation ) continue;
            if( list->object->Flags & PRFLG_INVISIBLE ) continue;
            ObjRender( list->object, &Region, light );
            if( !( list->object->OutlineColor & 0xFFFFFF) ) continue;
            if( list->object->OutlineColor < 0 ) continue;
            if( gObjUnk28 < 100 ) gObjUnk27[ gObjUnk28++ ] = list->object;
        }
    }
}

void ObjRenderCursor( VidRect_t *Area )
{
    int i;
    ObjList_t *p;
    VidRect_t tmp;

    if( !gObjUnk20 ) return;
    if( RegionShrink( Area, &gObjUnk16, &tmp ) ) return;    
    for( i = 0; i < gObjUnk28; i++ ) ObjRenderHexCursor( gObjUnk27[ i ], &tmp );
    TextRender( &tmp );
    for( p = gObjOffGridObjs; p; p = p->Next ){
        if( (p->object->Flags & 1) == 0 ) ObjRender( p->object, &tmp, 0x10000 );
    }    
}

void ObjUnk10( int result, int a2, int a3, unsigned int a4, int a7, int *a6, int a9 )
{
/*
// -- Not Used function --
    int v7,v8,v9,v10,v11,v12,v13,v15,v26,v28,v29,w,v18,Spitch,FrameNo;
    char *ObjData,Alpha[4],*Data;
    CachePool_t *v20;
    unsigned int Orientation;

    FrameNo = a2;
    Orientation = a3;
    if( a4 >= gObjUnk13 ) return a4;
    if( a4 > a6[2] ) return a4;
    if( a7 >= gObjUnk14 ) return a7;
    if( a7 > a6[3] ) return a7;
    result = ArtIsHidden( OBJTYPE( result ) );
    if( result ) return result;    
    if( !(Data = ArtLoadImg(result, &v20)) ) return Data;
    v8 = a6[0];
    v9 = a6[1];
    v29 = a6[2] - a6[0] + 1;
    v28 = a6[3] - v9 + 1;
    if( v8 < 0 ) v8 = 0;
    if( v9 < 0 ) v9 = 0;
    if( v8 + v29 > gObjUnk13 ) v29 = gObjUnk13 - v8;
    if( v9 + v28 > gObjUnk14 ) v28 = gObjUnk14 - v9;
    ArtGetObjSize(Data, FrameNo, Orientation, &Spitch, Alpha);
    if( v7 > v8 ){
        v11 = v29 + v8;
        w = ( v7 + Spitch <= v11 ) ? Spitch : (v11 - v7);
    } else {
        v10 = Spitch - (v8 - v7);
        v7 = v8;
        w = ( v10 > v29 ) ? v29 : v10;
    }
    v12 = a7;
    if( v9 < a7 ){
        v13 = *Alpha;
        v26 = 0;
        if( *Alpha + a7 > v9 + v28 ){
            v15 = v9 + v28 - a7;
        } else {
    	    v15 = v13;
        }
    } else {
        a7 = v9;
        v15 = *Alpha - (v9 - v12);
        v26 = v9 - a7;
        if( v15 > v28 ) v15 = v28;
    }    
    if( w > 0 && v15 > 0 ){
        ObjData = ArtGetObjData(Data, FrameNo, Orientation);
        if( OBJTYPE( result ) == 6 )
            ScrCopyAlpha( &ObjData[ v26 * Spitch + v18 ], w, v15, Spitch, &gObjIsoSurface[ a7 * gObjIsoPitch + v7 ], gObjIsoPitch );
        else
            ObjRenderNormal( &ObjData[ v26 * Spitch + v18 ], w, v15, Spitch, gObjIsoSurface, v7, a7, gObjIsoPitch, a9 );
    }
    return ArtClose(v20);
*/
}

int ObjCreate( Obj_t **obj, int ArtId, int Pid )
{
    Proto_t *proto;
    ObjList_t *p;

    if( !obj ) return -1;
    
    p = Malloc( sizeof( ObjList_t ) );
    if( !p ) return -1;
    p->Next = NULL;
    p->object = NULL;

    if( ObjNewObj( &p->object ) == -1 ){
        if( p ){ Free( p ); p = NULL; }
        return -1;
    }
    
    p->object->ImgId = ArtId;
    ObjAddObject( p );
    if( obj ) *obj = p->object;
    p->object->Pid = Pid;
    p->object->TimeEv = ScptNewObjId();
    if( Pid == -1 || OBJTYPE( Pid ) == TYPE_TILE ){
        p->object->Container.Box.Box = NULL;        
        p->object->Container.Box.Cnt = 0;
        return 0;
    }
    proto = NULL;

    ProtoEffectInit( p->object );
    if( ProtoGetObj( Pid, &proto ) == -1 ) return 0;
    ObjSetLight( p->object, proto->LtRad, proto->LtInt, 0 );
    if( (proto->Flags & PRFLG_FLAT) ) ObjSetPlayer( p->object, 0 );
    if( proto->Flags & 0x0010 ) p->object->Flags |= 0x10;
    if( proto->Flags & 0x0800 ) p->object->Flags |= 0x800;
    if( proto->Flags & 0xff00 ){
        if( proto->Flags & 0x1000000 ){
            p->object->Flags |= 0x01000000;
        } else if( proto->Flags & 0x2000000 ){
            p->object->Flags |= 0x02000000;
        } else if( proto->Flags & 0x4000000 ){
            p->object->Flags |= 0x04000000;
        } else if( proto->Flags & 0x8000000 ){
            p->object->Flags |= 0x08000000;
        } else if( proto->Flags & 0x40000000 ){
            p->object->Flags |= 0x40000000;
        }
    } else {
        p->object->Flags |= 0x80000000;
    }
    if( proto->Flags & 0x20000000 ) p->object->Flags |= 0x20000000;
    if( proto->Flags & 0x80000000 ) p->object->Flags |= 0x80000000;
    if( proto->Flags & 0x10000000 ) p->object->Flags |= 0x10000000;
    if( proto->Flags & 0x00001000 ) p->object->Flags |= 0x00001000;
    UseCreateScript( p->object, &p->object->ScrId );
    return 0;
}

int ObjCopy( Obj_t **pObj, int Pid )
{
    Proto_t *proto;

    *pObj = NULL;
    if( ProtoGetObj( Pid, &proto ) != -1 ) return -1;
    return ObjCreate( pObj, proto->ImgId, Pid );    
}

int ObjAddToStack( ObjList_t *stk, Obj_t *item )
{
    ObjList_t *list;
    ObjBox_t *box;
    int i;
    int j;
    ObjList_t *Ptr;

    if( !item ) return -1;
    list = Malloc( sizeof( ObjList_t ) );
    if( !list ) return -1;
    list->Next = NULL;
    list->object = NULL;
    if( ObjNewObj( &list->object ) == -1 ){
        if( list ){ Free( list ); list = NULL; }
	return -1;
    }
    ProtoDudeEffectReset( list->object );
    memcpy( list->object, item, sizeof( Obj_t ) );
    if( stk ) stk->object = list->object;
    ObjAddObject( list );
    stk->object->TimeEv = ScptNewObjId();
    if( stk->object->ScrId != -1 ){
        stk->object->ScrId = -1;
        UseCreateScript( stk->object, &stk->object->ScrId );
    }
    if( ObjSetRotation( stk->object, item->Orientation, 0 ) == -1 ){
        if( list ){ Free( list ); list = NULL; }
        return -1;
    }
    stk->object->Flags &= ~0x2000;
    box = &stk->object->Container.Box;
    box->Capacity = 0;
    box->Box = 0;
    box->Cnt = 0;    
    for( i = j = 0; i < item->Critter.Box.Cnt; j++, i++ ){
	if( !ObjAddToStack( stk, item->Critter.Box.Box[ j ].obj ) ) return -1;
        if( item->Critter.Box.Box[ j ].obj == -1 ){ Free( list ); list = NULL; return -1; }
        if( ItemAdd( stk->object, Ptr, item->Container.Box.Box[ i ].Quantity ) == -1 ){
            if( list ){ Free( list ); list = NULL; }
    	    return -1;
        }
    }    
    return 0;
}

int ObjUnk14( Obj_t *obj, unsigned int GridId, int MapLvl, VidRect_t *Area )
{
    ObjList_t *list;

    if( !obj || GridId > 39999 || MapLvl > 2 ) return -1;
    if( !(list = Malloc( sizeof( ObjList_t ) )) ) return -1;
    list->Next = NULL;
    list->object = NULL;
    list->object = obj;
    return ObjAddObjToList( list, GridId, MapLvl, Area );
}

int ObjLightItem( Obj_t *Obj, VidRect_t *Area )
{
    int GridId;
    ObjList_t *tmp;
    ObjList_t *list;

    if( !Obj || ObjFindInList( Obj, &list, &tmp ) ) return -1;
    if ( ObjLight( Obj, 1, Area ) == -1 && Area ) ObjGetRadiusArea( Obj, Area );
    if ( tmp ){
        tmp->Next = list->Next;
    } else {
        GridId = list->object->GridId;
        if( GridId == -1 )
            gObjOffGridObjs = gObjOffGridObjs->Next;
        else
            gObjGridObjects[ GridId ] = gObjGridObjects[ GridId ]->Next;
    }
    if( &tmp != (void *)-4 ){
        if( list ){
            Free( list );
            list = NULL;
        }
    }
    Obj->GridId = -1;
    return 0;
}

int ObjMove( Obj_t *Obj, int Xpos, int Ypos, VidRect_t *Area )
{
    VidRect_t rect, Rect;
    ObjList_t *q, *p;

    if( !Obj ) return -1;
    if( ObjFindInList( Obj, &p, &q ) == -1 ) return -1;
    if( Obj == gObjDude ){
        if( Area ){
            ObjGetRadiusArea( gObjRadius, &Rect );
            memcpy( Area, &Rect, sizeof( VidRect_t ) );            
            if( q ){
                q->Next = p->Next;
            } else {
                if( p->object->GridId == -1 )
                    gObjOffGridObjs = gObjOffGridObjs->Next;
                else
                    gObjGridObjects[p->object->GridId] = gObjGridObjects[ p->object->GridId ]->Next;
            }
            Obj->PosX += Xpos; Obj->Sx += Xpos;
            Obj->PosY += Ypos; Obj->Sy += Ypos;
            ObjAddObject( p );
            Rect.tp += Ypos; Rect.rt += Xpos;
            Rect.bm += Ypos; Rect.lt += Xpos;
            ObjMove( gObjRadius, Xpos, Ypos, 0 );
            RegionExpand( Area, &Rect, Area );
            return 0;
        } else {
            if( q ){
                q->Next = p->Next;
            } else {
                if( p->object->GridId == -1 )
                    gObjOffGridObjs = gObjOffGridObjs->Next;
                else
                    gObjGridObjects[ p->object->GridId ] = gObjGridObjects[ p->object->GridId ]->Next;
            }
            Obj->PosX += Xpos; Obj->Sx += Xpos;
            Obj->PosY += Ypos; Obj->Sy += Ypos;
            ObjAddObject( p );
            ObjMove( gObjRadius, Xpos, Ypos, 0 );
            return 0;
        }
    } else if ( Area ){
        ObjGetRadiusArea( Obj, Area );
        if( q ){
            q->Next = p->Next;
        } else {
            if( p->object->GridId == -1 )
                gObjOffGridObjs = gObjOffGridObjs->Next;
            else
                gObjGridObjects[ p->object->GridId ] = gObjGridObjects[ p->object->GridId ]->Next;
        }
        Obj->PosX += Xpos; Obj->Sx += Xpos;
        Obj->PosY += Ypos; Obj->Sy += Ypos;
        ObjAddObject( p );
	memcpy( &rect, Area, sizeof( VidRect_t ) );
        rect.lt += Xpos; rect.rt += Xpos;
        rect.bm += Ypos; rect.tp += Ypos;
        RegionExpand( Area, &rect, Area );
        return 0;
    } else {
        if( q ){
            q->Next = p->Next;
        } else {
            if( p->object->GridId == -1 )
                gObjOffGridObjs = gObjOffGridObjs->Next;
            else
                gObjGridObjects[ p->object->GridId ] = gObjGridObjects[ p->object->GridId ]->Next;
        }
        Obj->PosX += Xpos; Obj->Sx += Xpos;
        Obj->PosY += Ypos; Obj->Sy += Ypos;
        ObjAddObject( p );
        return 0;
    }
}

int ObjPutCursor( Obj_t *Obj, int Xpos, int Ypos, int MapLvl, VidRect_t *Area )
{
    ArtFrmHdr_t *art;
    VidRect_t Area1, Area2;
    CachePool_t *ImgObj;
    ObjList_t *p, *q;
    unsigned int GridIdx;
    int h,w,flg;

    if( !Obj ) return -1;
    GridIdx = Obj->GridId;
    flg = 0;
    if( GridIdx <= 39999 ){
        if( ObjFindInList( Obj, &p, &q ) == -1 ) return -1;
        if( ObjLight(Obj, 1, Area) == -1 && Area ) ObjGetRadiusArea( Obj, Area );
        if( q ){
            q->Next = p->Next;
        } else {
            if( p->object->GridId == -1 )
                gObjOffGridObjs = gObjOffGridObjs->Next;
            else
                gObjGridObjects[ p->object->GridId ] = gObjGridObjects[ p->object->GridId ]->Next;
        }
        Obj->GridId = -1;
        Obj->Elevation = MapLvl;
        flg = 1;
    } else {
	if( MapLvl == Obj->Elevation ){
    	    if( Area ) ObjGetRadiusArea( Obj, Area );
	} else {
	    if( ObjFindInList( Obj, &p, &q ) == -1 ) return -1;
	    if( Area ) ObjGetRadiusArea( Obj, Area );
	    if( q ){
    		q->Next = p->Next;
	    } else {
		if( p->object->GridId != -1 ){
    		    gObjGridObjects[ p->object->GridId ] = gObjGridObjects[ p->object->GridId  ]->Next;
		} else {
		    gObjOffGridObjs = gObjOffGridObjs->Next;
		}
	    }
	    Obj->Elevation = MapLvl;
	    flg = 1;
	}
    }
    if( (art = ArtLoadImg( Obj->ImgId, &ImgObj ) ) ){
        ArtGetObjSize( art, Obj->FrameNo, Obj->Orientation, &w, &h ); // set cursor position
        Obj->Sx = Xpos - w / 2;
        Obj->Sy = Ypos - ( h - 1 );
        ArtClose( ImgObj );
    }
    if( flg ) ObjAddObject( p );
    if( Area ){
        ObjGetRadiusArea( Obj, &Area2 );
        RegionExpand( Area, &Area2, Area );
    }
    if( Obj == gObjDude ){
        if( Area ){
            ObjPutCursor( gObjRadius, Xpos, Ypos, MapLvl, &Area1 );
            RegionExpand( Area, &Area1, Area );
        } else {
            ObjPutCursor( gObjRadius, Xpos, Ypos, MapLvl, 0 );
        }
    }
    return 0;
}

int ObjMoveToTile( Obj_t *obj, unsigned int GridPos, int MapLvl, VidRect_t *pLightArea )
{
    int v19,xg,yg,v16,v31,k;
    ObjList_t *p, *ListPrev, *ListCur;
    Obj_t *object;
    VidRect_t v22, Area2;
    Map01_t ptr;

    if( !obj || GridPos > 39999 || MapLvl > 2 ) return -1;
    if( ObjFindInList( obj, &ListCur, &ListPrev ) == -1 ) return -1;
    k = ObjLight( obj, 1, pLightArea );
    if( pLightArea ){
        if( k == -1 ) ObjGetRadiusArea( obj, pLightArea );
        memcpy( &Area2, pLightArea, sizeof( VidRect_t ) );
    }
    if( ListPrev ){ // found -> unlink
        ListPrev->Next = ListCur->Next;
    } else { // not found
        if( ListCur->object->GridId == -1 )
            gObjOffGridObjs = gObjOffGridObjs->Next; // remove from list
        else
            gObjGridObjects[ ListCur->object->GridId ] = gObjGridObjects[ ListCur->object->GridId ]->Next; // remove from grid
    }
    if( ObjAddObjToList( ListCur, GridPos, MapLvl, pLightArea ) == -1 ) return -1;
    if( IN_COMBAT && OBJTYPE( obj->ImgId ) == TYPE_CRIT ) CombatUnk01( obj, obj->OutlineColor && obj->OutlineColor >= 0 );
    if( pLightArea ) RegionExpand( pLightArea, &Area2, pLightArea );

    if( obj == gObjDude ){
        for( p = gObjGridObjects[ GridPos ]; p; p = p->Next ){
            object = p->object;
            if( MapLvl < p->object->Elevation ) break;
            if( MapLvl != p->object->Elevation || OBJTYPE( object->ImgId ) != TYPE_MISC ) continue;
            if( object->Pid >= 0x5000010 && object->Pid <= 0x5000017 ){ // Exit Grid
                    memset( &ptr, 0, sizeof( ptr ) );
                    ptr.MapId = object->Grid.DestMapId;
                    ptr.PosY = object->Grid.DestStartPos;
                    ptr.Frame = object->Grid.DestMapElev;
                    ptr.Orientation = object->Grid.DestOrientation;
                    MapSetPos( &ptr );
                    WmUnk45( ptr.MapId, ptr.Frame, 1 );
            }            
        }
        gObjUnk01[ GridPos / 8 ] |= 1 << ( GridPos & 7 );
        xg = (GridPos % 200) / 2;
        yg = (GridPos / 200) / 2;
        if( xg != gObjPlayerMapX || yg != gObjPlayerMapY || MapLvl != gObjPlayerMapLvl ){
            v16 = gMapIsoGrid[ MapLvl ][ 100 * yg + xg ];
            v19 = gMapIsoGrid[ MapLvl ][ 100 * gObjPlayerMapY + gObjPlayerMapX ];
            v31 = ( ArtMakeId( 4, 1, 0, 0, 0 ) == ArtMakeId( 4, (v16 >> 16) & 0xFFF, 0, 0, 0 ) );
            if( ( v31 != gObjUnk36 ) || ((((v16 & 0xffff0000) >> 16) & 0xF000) >> 12 != (((v19 & 0xffff0000) >> 16) & 0xF000) >> 12 )){
                if( !gObjUnk36 ) TileRoofVisible(gObjPlayerMapX, gObjPlayerMapY, MapLvl, 1);
                if( !v31 ) TileRoofVisible(xg, yg, MapLvl, 0 );
                if( pLightArea ) RegionExpand( pLightArea, &gVidMainGeo, pLightArea );
            }
            gObjPlayerMapX = xg;
            gObjPlayerMapY = yg;
            gObjPlayerMapLvl = MapLvl;
            gObjUnk36 = v31;
        }
        if( pLightArea ){
            ObjMoveToTile( gObjRadius, GridPos, MapLvl, &v22 );
            RegionExpand( pLightArea, &v22, pLightArea );
        } else {
            ObjMoveToTile( gObjRadius, GridPos, MapLvl, 0 );
        }
        if( MapLvl != obj->Elevation ){
            MapSetLvl( MapLvl );
            TileSetCenter( GridPos, 3 );
            if( (gCombatStatus & 1) != 0 ) gMenuEscape = 1;
        }
    } else if( MapLvl != gObjPlayerMapLvl && OBJTYPE( obj->Pid ) == TYPE_CRIT ){
        CombatUnk79( obj );
    }
    return 0;
}

int ObjUnk19()
{
    int Id;
    Id = ArtMakeId( 4, ((gMapIsoGrid[ gObjDude->Elevation ][ 100 * gObjPlayerMapY + gObjPlayerMapX ] ) >> 24) & 0xFFF, 0, 0, 0 );
    if( Id != ArtMakeId( 4, 1, 0, 0, 0 ) ) TileRoofVisible( gObjPlayerMapX, gObjPlayerMapY, gObjDude->Elevation, 1 );
    return 0;
}

int ObjSetShape( Obj_t *obj, int ImgId, VidRect_t *Area )
{
    VidRect_t TmpArea;

    if( !obj ) return -1;
    if( Area ){
        ObjGetRadiusArea( obj, Area );
        obj->ImgId = ImgId;
        ObjGetRadiusArea( obj, &TmpArea );
        RegionExpand( Area, &TmpArea, Area );
    } else {
        obj->ImgId = ImgId;
    }
    return 0;
}

int ObjSetFrame( Obj_t *Crit, int Frame, VidRect_t *Area )
{
    ArtFrmHdr_t *Img; 
    VidRect_t Rect;
    CachePool_t *ImgObj;

    if( !Crit ) return -1;
    if( !(Img = ArtLoadImg( Crit->ImgId, &ImgObj ) ) ) return -1;
    ArtClose( ImgObj );
    if( Frame >= Img->Fpd ) return -1;
    if( Area ){
        ObjGetRadiusArea( Crit, Area );
        Crit->FrameNo = Frame;
        ObjGetRadiusArea( Crit, &Rect );
        RegionExpand( Area, &Rect, Area );
    } else {
        Crit->FrameNo = Frame;
    }
    return 0;
}

int ObjIncFrame( Obj_t *obj, VidRect_t *Area )
{
    ArtFrmHdr_t *Img;
    int Frame;
    VidRect_t Rect;
    CachePool_t *ImgObj;

    if( !obj ) return -1;    
    if( !(Img = ArtLoadImg( obj->ImgId, &ImgObj ) ) ) return -1;
    ArtClose( ImgObj );
    Frame = obj->FrameNo + 1;
    if( Frame >= Img->Fpd ) Frame = 0;
    if( Area ){
        ObjGetRadiusArea( obj, Area );
        obj->FrameNo = Frame;
        ObjGetRadiusArea( obj, &Rect );
        RegionExpand( Area, &Rect, Area );
    } else {
        obj->FrameNo = Frame;
    }
    return 0;
}

int ObjDecFrame( Obj_t *obj, VidRect_t *Area )
{
    int Frame;
    ArtFrmHdr_t *Img;
    VidRect_t Rect;
    CachePool_t *ImgObj;

    if( !obj || !(Img = ArtLoadImg(obj->ImgId, &ImgObj) ) ) return -1;
    ArtClose( ImgObj );
    Frame = obj->FrameNo - 1;
    if( Frame < 0 ) Frame = Img->Fpd - 1;
    if( Area ){
        ObjGetRadiusArea( obj, Area );
        obj->FrameNo = Frame;
        ObjGetRadiusArea( obj, &Rect );
        RegionExpand( Area, &Rect, Area );
    } else {
        obj->FrameNo = Frame;
    }
    return 0;
}
int ObjSetRotation( Obj_t *obj, unsigned int Orientation, VidRect_t *Area )
{
    VidRect_t tmp;

    if( !obj || Orientation >= 6 ) return -1;
    if( Area ){
        ObjGetRadiusArea( obj, Area );
        obj->Orientation = Orientation;
        ObjGetRadiusArea( obj, &tmp );
        RegionExpand( Area, &tmp, Area );
    } else {
        obj->Orientation = Orientation;
    }
    return 0;
}

int ObjTurnCW( Obj_t *obj, VidRect_t *Area )
{
    unsigned int ori;

    ori = obj->Orientation + 1;
    if( ori >= 6 ) ori = 0;
    return ObjSetRotation( obj, ori, Area );
}

int ObjTurnCCW( Obj_t *obj, VidRect_t *Area )
{
    unsigned int ori;

    ori = ( obj->Orientation ) ? (obj->Orientation - 1) : 5;
    return ObjSetRotation( obj, ori, Area );
}

void ObjLightGrid()
{
    int i;
    ObjList_t *p;

    ItemGridLightReset();
    for( i = 0; i != 40000; i++ ){
        for( p = gObjGridObjects[ i ]; p; p = p->Next ){
            ObjLight( p->object, 0, 0 );
        }
    }
}

int ObjSetLight( Obj_t *obj, int LightRadius, int LightIntensity, VidRect_t *Area )
{
    int ret;
    VidRect_t rect;

    if( !obj ) return -1;
    ret = ObjLightedOff( obj, Area );
    if( LightIntensity > 0 ){
        if( LightRadius >= 8 ) LightRadius = 8;
        obj->LightIntensity = LightIntensity;
        obj->LightRadius = LightRadius;
        if( Area ){
            ret = ObjLightedOn( obj, &rect );
            RegionExpand( Area, &rect, Area );
        } else {
            return ObjLightedOn( obj, 0 );
        }
    } else {
        obj->LightIntensity = 0;
        obj->LightRadius = 0;
    }
    return ret;
}

int ObjGetLightIntensity( Obj_t *obj )
{
    int inten, l;

    l = ItemMapGetLight();
    inten = ItemGridGetLight( obj->Elevation, obj->GridId );
    if( obj == gObjDude ) inten -= gObjDude->LightIntensity;
    if( inten < l ) return l;
    if( inten > 0x10000 ) return 0x10000;
    return inten;
}

int ObjLightedOn( Obj_t *obj, VidRect_t *Area )
{
    if( !obj ) return -1;
    if( obj->LightIntensity <= 0 ){
        obj->Flags &= ~PRFLG_LIGHTED;
        return -1;
    }
    if( !( obj->Flags & PRFLG_LIGHTED ) ){
        obj->Flags |= PRFLG_LIGHTED;
        if( ObjLight( obj, 0, Area ) == -1 ){
            if( Area ) ObjGetRadiusArea( obj, Area );
        }
    }
    return 0;
}

int ObjLightedOff( Obj_t *obj, VidRect_t *rect )
{
    if( !obj ) return -1;
    if( obj->LightIntensity <= 0 ){
	obj->Flags &= ~PRFLG_LIGHTED;
        return -1;
    }
    if( obj->Flags & PRFLG_LIGHTED ){
        if( ObjLight( obj, 1, rect ) == -1 ){
            if( rect ) ObjGetRadiusArea( obj, rect );
        }
        obj->Flags &= ~PRFLG_LIGHTED;
    }
    return 0;
}

int ObjUnk32( Obj_t *obj, VidRect_t *Area )
{
    VidRect_t tmp;

    if( !obj || (obj->Flags & 1) == 0 )  return -1;
    obj->Flags &= ~0x01;
    obj->OutlineColor &= ~0x8000;
    if( ObjLight(obj, 0, Area) == -1 && Area ) ObjGetRadiusArea( obj, Area );
    if( obj != gObjDude ) return 0;
    if( !Area ) return 0;
    ObjGetRadiusArea( gObjRadius, &tmp );
    RegionExpand( Area, &tmp, Area );
    return 0;
}

int ObjUnk33( Obj_t *obj, VidRect_t *Area )
{
    VidRect_t rect;

    if( !obj || ( obj->Flags & 1 ) ) return -1;
    if( ObjLight( obj, 1, Area ) == -1 && Area ) ObjGetRadiusArea( obj, Area );
    obj->Flags |= 0x01;
    if( obj->OutlineColor & 0xFFFFFF ) obj->OutlineColor |= 0x8000;
    if( obj != gObjDude ) return 0;
    if( !Area ) return 0;
    ObjGetRadiusArea( gObjRadius, &rect );
    RegionExpand( Area, &rect, Area );
    return 0;
}

int ObjUnk34( Obj_t *obj, VidRect_t *Area )
{
    if( !obj ) return -1;
    obj->OutlineColor &= ~0x80000000;
    if( Area ) ObjGetRadiusArea( obj, Area );
    return 0;
}

int ObjUnk35( Obj_t *obj, VidRect_t *area )
{
    if( !obj ) return -1;
    if( (obj->OutlineColor & 0xFFFFFF) != 0 ) obj->OutlineColor |= 0x80000000;
    if( area ) ObjGetRadiusArea( obj, area );
    return 0;
}

int ObjSetPlayer( Obj_t *obj, VidRect_t *RadArea ) // zla nazwa ?
{
    VidRect_t Rect, Area;
    ObjList_t *ObjList, *list;

    if( !obj ) return -1;
    if( ObjFindInList( obj, &ObjList, &list ) == -1 ) return -1;
    if( RadArea ){
        ObjGetRadiusArea( obj, RadArea );
        if( list ){
            list->Next = ObjList->Next;
        } else {
            if( ObjList->object->GridId == -1 )
                gObjOffGridObjs = gObjOffGridObjs->Next;
            else
                gObjGridObjects[ ObjList->object->GridId ] = gObjGridObjects[ ObjList->object->GridId ]->Next;
        }
        obj->Flags ^= 0x08;
        ObjAddObject( ObjList );
        ObjGetRadiusArea( obj, &Rect );
        RegionExpand( RadArea, &Area, RadArea );
        return 0;
    }
    if( list ){
        list->Next = ObjList->Next;
    } else {
        if( ObjList->object->GridId == -1 )
            gObjOffGridObjs = gObjOffGridObjs->Next;
        else
            gObjGridObjects[ ObjList->object->GridId ] = gObjGridObjects[ ObjList->object->GridId ]->Next;
    }
    obj->Flags ^= 0x08;
    ObjAddObject( ObjList );
    return 0;
}

void ObjUnk37( int Update )
{
    int i, t;
    ObjList_t *p;

    gObjUnk41 = 1 - gObjUnk41;
    if( gObjUnk41 ){
        for( i = 0; i != 40000; ++i ){
            for( p = gObjGridObjects[ i ]; p; p = p->Next ){
                if( gCurrentMapLvl < p->object->Elevation ) break;
                if( gCurrentMapLvl != p->object->Elevation ) continue;
                t = OBJTYPE( p->object->ImgId );
                if( t == 3 || t == 2 ) p->object->Flags |= 0x010000;
            }
        }
    } else {
        for( i = 0; i != 40000; i++ ){
            for( p = gObjGridObjects[ i ]; p; p = p->Next ){
                if( gCurrentMapLvl < p->object->Elevation ) break;
                if( gCurrentMapLvl != p->object->Elevation ) continue;
                t = OBJTYPE( p->object->ImgId );
                if( t == 3 || t == 2 ) p->object->Flags &= ~0x010000;
            }
        }
    }
    if( Update ) TileUpdate();
}

int ObjDestroy( Obj_t *obj, VidRect_t *radius )
{
    ObjList_t *list, *a1, *a2a;

    if( !obj ) return -1;
    GmouseUnk66( obj );
    if( !ObjFindInList( obj, &a1, &a2a ) ){
        if( ObjLight( obj, 1, radius ) == -1 ){
            if( radius ) ObjGetRadiusArea( obj, radius );
        }
        return ( !ObjDelete( a1, a2a ) ) ? 0 : -1;
    }
    if( &a2a == (void *)-8 ) return -1;
    if( !(list = Malloc( sizeof( ObjList_t ) ) ) ) return -1;    
    list->Next = NULL;
    list->object = NULL;
    list->object = obj;
    if( ObjDelete( list, list ) ) return -1;
    return 0;
}

int ObjBoxDelete( ObjContainer_t *box )
{
    int i;
    ObjList_t *p;

    for( i = 0; i < box->Box.Cnt; i++ ){
        p = Malloc( 8 );
        if( p ){
            p->Next = NULL;
            p->object = NULL;
        }
        p->object = box->Box.Box[ i ].obj;
        p->object->Flags &= ~0x400;
        ObjDelete( p, p );
        box->Box.Box[ i ].obj = NULL;
    }
    if( box->Box.Box ){
        Free(box->Box.Box);
        box->Box.Box = NULL;
        box->Box.Cnt = 0;
        box->Box.Capacity = 0;
    }
    return 0;
}

int ObjOpenable( Obj_t *obj )
{
    if( obj->Pid == PID_LIGHTEDFLARE || obj->Pid == PID_DYNAMITE || obj->Pid == PID_PLASTICEXPLOSIVES ) return 0;
    return ProtoOpenable( obj->Pid );
}

int ObjTalkative( Obj_t *obj )
{
    return ProtoUnk05( obj->Pid ) && OBJTYPE( obj->Pid ) == TYPE_CRIT && CritterCanTalk( obj );
}

int ObjUnk42( Obj_t *ScenObj )
{
    Proto_t *proto;

    if( OBJTYPE( ScenObj->Pid ) != TYPE_SCEN ) return 0;
    if( ProtoGetObj( ScenObj->Pid, &proto ) == -1 ) return 0;
    return ( proto->Critt.BaseStat[0] & 4 ) != 0;
}

Obj_t *ObjUnk43( int Id )
{
    Obj_t *p;

    for( p = ObjGetObjectFirst(); p; p = ObjGetObjectNext() ){
        if( Id == p->TimeEv ) break;
    }
    return p;
}

Obj_t *ObjGetOwner( Obj_t *obj )
{
    Obj_t *Owner;

    Owner = obj->Owner;
    if( !Owner ) return NULL;
    while( Owner->Owner ) Owner = Owner->Owner;
    return Owner;
}

void ObjClear()
{
    int i;
    ObjList_t *p, *q, *tmp;
    
    ScptFlush();
    for( i = 0; i != 200*200; i++ ){
        q = NULL;
        for( p = gObjGridObjects[ i ]; p; ){
    	    tmp = p->Next;
            if( ObjDelete( p, q ) == -1 ) q = p;
            p = tmp;
        }
    }
    q = NULL;
    for( p = gObjOffGridObjs; p; ){
	tmp = p->Next;
	if( ObjDelete( p, q ) == -1 ) q = p;
	p = tmp;
    }
    gObjPlayerMapY = -1;
    gObjPlayerMapLvl = -1;
    gObjUnk36 = 1;
    gObjPlayerMapX = -1;
}

Obj_t *ObjGetObjectFirst()
{
    for( gObjMapObjIdx = gObjMapObjLvl = 0; gObjMapObjIdx < 40000; gObjMapObjIdx++ ){
	for( gObjMapObj = gObjGridObjects[ gObjMapObjIdx ]; gObjMapObj; gObjMapObj = gObjMapObj->Next ){
    	    if( !ArtIsHidden( OBJTYPE( gObjMapObj->object->ImgId ) ) ) return gObjMapObj->object;    	    
    	}
    }
    return NULL;
}

Obj_t *ObjGetObjectNext()
{
    if( gObjMapObj ){
        gObjMapObj = gObjMapObj->Next;
        while( gObjMapObjIdx < 40000 ){
            while( gObjMapObj ){
                if( !ArtIsHidden( OBJTYPE( gObjMapObj->object->ImgId ) ) ) return gObjMapObj->object;
                gObjMapObj = gObjMapObj->Next;
            }
            gObjMapObj = gObjGridObjects[ ++gObjMapObjIdx ];
        }
    }
    gObjMapObj = NULL;
    return NULL;
}

Obj_t *ObjGetVisibleObjectFirst( int MapLvl )
{
    ObjList_t *Next;

    gObjMapObjLvl = MapLvl;    
    for( gObjMapObjIdx = 0; gObjMapObjIdx < 40000; gObjMapObjIdx++ ){
	for( Next = gObjGridObjects[ gObjMapObjIdx ]; Next; Next = gObjMapObj->Next ){
    	    gObjMapObj = Next;
    	    if( MapLvl != Next->object->Elevation ) continue;
    	    if( !ArtIsHidden( OBJTYPE( Next->object->ImgId ) ) ) return gObjMapObj->object;    	    
	}	
    }
    gObjMapObj = Next;
    return NULL;    
}

Obj_t *ObjGetVisibleObjectNext()
{
    if( !gObjMapObj ) return NULL;    
    for( gObjMapObj = gObjMapObj->Next ;gObjMapObjIdx < 40000; gObjMapObj = gObjGridObjects[ ++gObjMapObjIdx ] ){
        for( ; gObjMapObj; gObjMapObj = gObjMapObj->Next ){
            if( gObjMapObj->object->Elevation != gObjMapObjLvl ) continue;
            if( !ArtIsHidden( OBJTYPE( gObjMapObj->object->ImgId ) ) ) return gObjMapObj->object;
        }        
    }
    gObjMapObj = NULL;
    return NULL;
}

Obj_t *ObjGetFirst( int MapLvl, int GridIdx )
{
    gObjMapObjIdx = GridIdx;
    gObjMapObjLvl = MapLvl;    
    for( gObjMapObj = gObjGridObjects[ GridIdx ]; gObjMapObj; gObjMapObj = gObjMapObj->Next ){
        if( MapLvl != gObjMapObj->object->Elevation ) continue; 
        if( !ArtIsHidden( OBJTYPE( gObjMapObj->object->ImgId ) ) ) return gObjMapObj->object;
    }         
    return NULL;
}

Obj_t *ObjGetNext()
{
    if( !gObjMapObj ) return NULL;    
    for( gObjMapObj = gObjMapObj->Next; gObjMapObj; gObjMapObj = gObjMapObj->Next ){
        if( gObjMapObj->object->Elevation != gObjMapObjLvl ) continue;
        if( !ArtIsHidden( OBJTYPE( gObjMapObj->object->ImgId )) ) return gObjMapObj->object;
    }
    return NULL;
}

void ObjGetRadiusArea( Obj_t *obj, VidRect_t *Rect )
{
    CachePool_t *ImgObj;
    ArtFrmHdr_t *Img;
    int ObjHeight, ObjWidth, TileHeight, TileWidth, OlColor;

    if( !obj || !Rect ) return;
    OlColor = ( obj->OutlineColor & 0xFFFFFF ) != 0;
    if( !(Img = ArtLoadImg( obj->ImgId, &ImgObj ) ) ){
        Rect->tp = 0;
        Rect->rt = 0;
        Rect->bm = 0;
        Rect->lt = 0;
        return;
    }    
    ArtGetObjSize( Img, obj->FrameNo, obj->Orientation, &ObjWidth, &ObjHeight );
    if( obj->GridId == -1 ){
        Rect->lt = obj->Sx;
        Rect->tp = obj->Sy;
        Rect->rt = ObjWidth + obj->Sx - 1;
        Rect->bm = ObjHeight + obj->Sy - 1;
    } else if( TileGetScrCoordinates( obj->GridId, &TileWidth, &TileHeight) ){
        Rect->tp = 0;
        Rect->rt = 0;
        Rect->bm = 0;
        Rect->lt = 0;
        OlColor = 0;
    } else {
        TileWidth  = TileWidth  + 16 + Img->PixShiftX[ obj->Orientation ] + obj->PosX;
        TileHeight = TileHeight + 8 + Img->PixShiftY[ obj->Orientation ] + obj->PosY;
        Rect->lt = TileWidth - ObjWidth / 2;
        Rect->tp = TileHeight - ObjHeight + 1;
        Rect->rt = ObjWidth + Rect->lt - 1;
        Rect->bm = TileHeight;
    }
    ArtClose( ImgObj );
    if( OlColor ){
        Rect->tp--;
        Rect->rt++;
        Rect->bm++;
        Rect->lt--;
    }
}

int ObjUnk53( int GridIdx, int MapLvl)
{
    ObjList_t *p;

    for( p = gObjGridObjects[ GridIdx ]; p; p = p->Next ){
        if( MapLvl != p->object->Elevation ) continue;
        if( ( p->object != gGmouseObjA ) && ( p->object != gGmouseObjB ) ) return 1;
    }
    return 0;    
}

Obj_t *ObjReach( Obj_t *obj, int GridIdx, int MapLvl )
{
    ObjList_t *list, *p;
    Obj_t *p1, *p2;
    unsigned int idx;
    int i, type;

    if( GridIdx > 39999 ) return 0;    
    for( list = gObjGridObjects[ GridIdx ] ;list; list = list->Next ){
        p1 = list->object;
        if( MapLvl == list->object->Elevation ){
            if( (p1->Flags & 1) == 0 && (p1->Flags & 0x10) == 0 && p1 != obj ){
                type = OBJTYPE( p1->ImgId );
                if( type == TYPE_SCEN || type == TYPE_CRIT || type == TYPE_WALL ) return list->object;
            }
        }            
    }
    for( i = 0; i < 6; i++ ){
        idx = TileGetTileNumInDir( GridIdx, i, 1 );
        if( idx > 39999 ) continue;
        for( p = gObjGridObjects[ idx ]; p; p = p->Next ){
    	    p2 = p->object;
    	    if( (p->object->Flags & 0x800) != 0 && MapLvl == p2->Elevation ){
        	if( !(p2->Flags & 1) && !(p2->Flags & 0x10) && p2 != obj ){
            	    type = OBJTYPE( p2->ImgId );
            	    if( type == TYPE_SCEN || type == TYPE_CRIT || type == TYPE_WALL ) return p->object;
        	}
    	    }        	    
        }
    }
    return NULL;    
}

Obj_t *ObjUnk55( unsigned int GridIdx, Obj_t *obj, int MapLvl )
{
    ObjList_t *p;
    int type, i, Pos;

    if ( GridIdx > 39999 ) return NULL;    
    for( p = gObjGridObjects[ GridIdx ]; p; p = p->Next ){
        if( MapLvl != p->object->Elevation ) continue;
        if( (p->object->Flags & 0x01) || ( (p->object->Flags & 0x10) && (p->object->Flags & 0x80 ) ) || (obj == p->object) ) continue;        
        type = OBJTYPE( p->object->ImgId );
        if( type == TYPE_SCEN || type == TYPE_CRIT || type == TYPE_WALL ) return p->object;
    }
    for( i = 0; i < 6; i++ ){
        Pos = TileGetTileNumInDir( GridIdx, i, 1 );
        if( Pos > 39999 ) continue;                        
    	for( p = gObjGridObjects[ Pos ]; p; p = p->Next ){
    	    if( !(p->object->Flags & 0x800) || MapLvl != p->object->Elevation ) continue;
    	    if( (p->object->Flags & 0x01) || (p->object->Flags & 0x10) || (p->object == obj ) ) continue;    	    
            type = OBJTYPE( p->object->ImgId );
            if( type == TYPE_SCEN || type == TYPE_CRIT || type == TYPE_WALL ) return p->object;    	        	            		
    	}    
    }
    return NULL;
}

Obj_t *ObjUnk56( unsigned int GridIdx, Obj_t *obj, int MapLvl )
{
    ObjList_t *p;
    int type, i, Pos;

    if( GridIdx > 39999 ) return NULL;    
    for( p = gObjGridObjects[ GridIdx ]; p; p = p->Next ){
        if( MapLvl != p->object->Elevation ) continue;        
        if( (p->object->Flags & 0x01) || (p->object->Flags & 0x10) || (p->object == obj) ) continue;        
        type = OBJTYPE( p->object->ImgId );
        if( (type != TYPE_SCEN) && (type != TYPE_CRIT) && (type != TYPE_WALL) ) continue;
        if( gObjUnk42 || type != TYPE_CRIT) return p->object;
        gObjUnk42 = p->object;                        
    }
    for( i = 0; i < 6; i++ ){
        Pos = TileGetTileNumInDir( GridIdx, i, 1 );
        if( Pos > 39999 ) continue;        
        for( p = gObjGridObjects[ Pos ]; p; p = p->Next ){
            if( !( p->object->Flags & 0x800 ) || MapLvl != p->object->Elevation ) continue;            
            if( (p->object->Flags & 0x01) || (p->object->Flags & 0x10) || (p->object == obj) ) continue;            
            type = OBJTYPE( p->object->ImgId );
            if( (type == TYPE_SCEN) || (type == TYPE_CRIT) || (type == TYPE_WALL) ){
                if( gObjUnk42 || (type != TYPE_CRIT) ) return p->object;
                gObjUnk42 = p->object;
            }                        
        }        
    }    
    return NULL;
}

int ObjUnk57( int MapIdx, int MapLvl )
{
    ObjList_t *p;

    if( MapIdx <= 0 || MapIdx >= 40000 ) return -1;    
    for( p = gObjGridObjects[ MapIdx ]; p; p = p->Next ){
        if( MapLvl < p->object->Elevation ) break;
        if( MapLvl == p->object->Elevation && p->object->Pid == 0x500000C ) return 0;
    }    
    return -1;
}

Obj_t *ObjUnk58( Obj_t *obj, int GridIdx, int MapLvl )
{
    ObjList_t *MapObj;
    Obj_t *object;
    int type;

    MapObj = gObjGridObjects[ GridIdx ];
    if( !MapObj ) return 0;
    while( 1 ){
        object = MapObj->object;
        if( MapLvl == MapObj->object->Elevation && (object->Flags & 1) == 0 && (object->Flags & 0x20000000) == 0 && object != obj ){
            type = OBJTYPE( object->ImgId );
            if( type == 2 || type == 3 ) break;
        }
        MapObj = MapObj->Next;
        if( !MapObj ) return 0;
    }
    return MapObj->object;
}

int ObjGetDistance( Obj_t *crit, Obj_t *obj )
{
    int Distance;

    if( !crit || !obj ) return 0;
    Distance = TileGetDistance( crit->GridId, obj->GridId );
    if( crit->Flags & 0x800 ) Distance--;
    if( obj->Flags & 0x800 ) Distance--;
    if( Distance <= 0 ) return 0;
    return Distance;
}

int ObjUnk60( Obj_t *crit, int MapIdx1, Obj_t *obj, int MapIdx2 )
{
    int Distance;

    if( !crit || !obj ) return 0;
    Distance = TileGetDistance( MapIdx1, MapIdx2 );
    if( crit->Flags & 0x800 ) Distance--;
    if( obj->Flags & 0x800 ) Distance--;
    if( Distance <= 0 ) return 0;
    return Distance;
}

int ObjGetObjList( int GridPos, int MapLvl, int ObjType , Obj_t ***ObjTable )
{
    int n, i, k;
    ObjList_t *p;

    if( !ObjTable ) return -1;
    n = 0;
    if( GridPos == -1 ){
        for( i = 0; i != 40000; i++ ){
            for( p = gObjGridObjects[ i ]; p; p = p->Next ){
                if( (p->object->Flags & 1) == 0 && p->object->Elevation == MapLvl && (p->object->ImgId & 0xF000000) >> 24 == ObjType ) n++;
            }
        }
    } else {
        for( p = gObjGridObjects[ GridPos ]; p; p = p->Next ){
            if( (p->object->Flags & 1) == 0 && MapLvl == p->object->Elevation && (p->object->ImgId & 0xF000000) >> 24 == ObjType ) n++;
        }
    }
    if( !n ) return 0;
    if( !(*ObjTable = Malloc( n * sizeof( Obj_t *) )) ) return -1;
    k = 0;
    if( GridPos == -1 ){
        for( i = 0; i != 40000; i++ ){
            for(p = gObjGridObjects[ i ]; p; p = p->Next ){
                if( (p->object->Flags & 1) == 0 && p->object->Elevation == MapLvl && (p->object->ImgId & 0xF000000) >> 24 == ObjType && k < n ){
                    (*ObjTable)[ k++ ] = p->object;
                }                    
            }
        }
    } else {
        for( p = gObjGridObjects[ GridPos ]; p; p = p->Next){
            if( (p->object->Flags & 1) == 0 && p->object->Elevation == MapLvl && (p->object->ImgId & 0xF000000) >> 24 == ObjType && k < n ){
                (*ObjTable)[ k++ ] = p->object;
            }                
        }
    }
    return n;
}

void ObjCritterListDestroy(void *n )
{
    if ( !n ) return;
    Free(n);
}

void ObjUnk63( char *pSrc, int Width, int Height, int SrcPitch, char *pDst, int Xpos, int Ypos, int DstPitch, char *Color, char *ColorMap )
{
    unsigned char *pd, *pix;
    int j, i, DstDiff;

    pd = (unsigned char *)pDst + Xpos + DstPitch * Ypos;
    DstDiff = DstPitch - Width;
    for( i = SrcPitch - Width; --Height != -1; pSrc += i, pd += DstDiff ){
        for( j = 0; j < Width; pSrc++, j++, pd++ ){
	    pix = (unsigned char *)Color + 256 * ColorMap[ (unsigned int)*pSrc ];
            *pd = pix[ *pd ];
        }        
    }
}

void ObjRenderHatch( char *pSrc, int Width, int Height, int SrcPitch, char *pDst, int Xpos, int Ypos, int DstPitch )
{
    char *p;
    int pos,toggle,i,dSrc,dDst;

    pos = Xpos + DstPitch * Ypos;
    dDst = DstPitch - Width;
    p = pDst + pos;
    dSrc = SrcPitch - Width;
    toggle = ( pDst + pos - gObjIsoSurface ) / gObjUnk13;
    toggle = toggle & 1;
    while( --Height != -1 ){
        for( i = 0; i < Width; i++, pSrc++, p++ ){
            if( *pSrc == 0 ) continue;            
            if( toggle == ((long int)p & 1) ) *p = *pSrc;            
        }
        toggle = 1 - toggle;
        p += dDst;
        pSrc += dSrc;
    }
}

void ObjRenderNormal( char *pSrc, int SrcW, int SrcH, int SrcPitch, char *pDst, int x, int y, int DstPitch, int Shade )
{
    unsigned char *p, a;
    int i, DiffDst, DiffSrc;

    p = (unsigned char *)pDst + x + DstPitch * y;
    DiffDst = DstPitch - SrcW;
    DiffSrc = SrcPitch - SrcW;
    for( ;--SrcH != -1; p += DiffDst, pSrc += DiffSrc ){
        for( i = 0; i < SrcW; pSrc++, p++, i++ ){            
            if( !(a = *pSrc) ) continue;
            if( a < 229 ) a = gPalShades[ a ][ (Shade >> 9) & 0xff ];
            *p = a;
        }        
    }
}

// render glass, steam, energy, cities on word map
void ObjRenderSemiTr( char *ImgDat, int Width, int Height, int SrcPitch, char *surf, int Xpos, int Ypos, int DstPitch, int Flags, char *Color, char *ColorMap )
{
    unsigned char *pDst, col, mask;
    unsigned int i, n, pal;

    pDst = (unsigned char *)&surf[ Xpos + DstPitch * Ypos ];
    DstPitch -= Width;
    for( ;--Height != -1; pDst += DstPitch, ImgDat += SrcPitch - Width ){
        for( i = 0; i < Width; i++, ImgDat++, pDst++ ){
    	    mask = *ImgDat;
            if( mask ){
        	n = ColorMap[ mask ];
        	pal = (unsigned int)*pDst;
        	col = Color[ 256 * n + pal ];
        	*pDst = gPalShades[ (unsigned int)col ][ (Flags >> 9) & 0xff ];
            }
        }        
    }
}

// render semi transparent walls
void ObjRenderMix( char *ImgWall, int Width, int Height, int SrcPitch, char *surf, int DstPitch, char *ImgShade, int ImgShadePitch, int inten )
{
    unsigned char c;
    int i,srcd,dstd,yd;
    unsigned int brt;

    dstd = DstPitch - Width;
    yd = ImgShadePitch - Width;
    srcd = SrcPitch - Width;
    brt = inten >> 9;
    for( ;--Height != -1; surf += dstd, ImgShade += yd, ImgWall += srcd ){
        for( i = 0; i < Width; ImgWall++, ImgShade++, surf++, i++ ){
            if( *ImgWall ){
                c = gPalShades[ *((unsigned char *)ImgWall)][ brt ];
                if( *ImgShade ) c = gPalSemiTransparent[ (unsigned)gPalShades[ c ][ (unsigned char)*ImgShade ] ][ gPalShades[ (unsigned char)*surf ][ 128 - (unsigned char)*ImgShade ] ];
                *surf = c;
            }            
        }
    }
}

void ObjUnk68( int MapLvl, int HlColor, VidRect_t *Area )
{
    Obj_t *obj;

    if( gObjUnk43 ) return;
    gObjUnk43 = 1;
    for( obj = ObjGetVisibleObjectFirst(MapLvl); obj; obj = ObjGetVisibleObjectNext() ){
        if( ( OBJTYPE( obj->ImgId ) == 0 ) && !obj->Owner ){
            ObjSetOutline( obj, HlColor, 0 );
            if( obj ) obj->OutlineColor &= ~0x80000000;
        }
    }
    if( Area ) {
        Area->rt = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        Area->tp = 0;
        Area->lt = 0;
        Area->bm = gVidMainGeo.bm - gVidMainGeo.tp - 99;
    } else {
        TileUpdate();
    }
}

void ObjUnk69( int MapLvl, VidRect_t *Area )
{
    Obj_t *obj;

    if( gObjUnk43 != 1 ) return;
    gObjUnk43 = 0;
    for( obj = ObjGetVisibleObjectFirst(MapLvl); obj; obj = ObjGetVisibleObjectNext() ){
        if( ( OBJTYPE( obj->ImgId ) == 0 ) && !obj->Owner ){
            if( obj && (obj->OutlineColor & 0xFFFFFF) ) obj->OutlineColor |= 0x80000000;
            if( obj ) obj->OutlineColor = 0;
        }
    }
    if( Area ){
        Area->rt = gVidMainGeo.rt - gVidMainGeo.lt + 1;
        Area->tp = 0;
        Area->lt = 0;
        Area->bm = gVidMainGeo.bm - gVidMainGeo.tp - 99;
    } else {
        TileUpdate();
    }
}

void ObjUnk70( int a1, VidRect_t *a2 )
{
    if( gObjUnk43 )
        ObjUnk69( a1, a2 );
    else
        ObjUnk68( a1, 16, a2 );
}

int ObjSetOutline( Obj_t *obj, int HlColor, VidRect_t *RadiusArea )
{
    if( !obj || (obj->OutlineColor & 0xFFFFFF) || (obj->Flags & 0x1000) ) return -1;
    obj->OutlineColor = HlColor;
    if( obj->Flags & 0x01 ) obj->OutlineColor |= 0x8000;
    if( RadiusArea ) ObjGetRadiusArea( obj, RadiusArea );
    return 0;
}

int ObjGetRadius( Obj_t *obj, VidRect_t *Area )
{
    if( !obj ) return -1;
    if( Area ) ObjGetRadiusArea( obj, Area );
    obj->OutlineColor = 0;
    return 0;
}

void ObjUnk73()
{
    ObjList_t *p;
    int i;

    for( i = 0; i != 40000; i++ ){
        for( p = gObjGridObjects[ i ]; p; p = p->Next ) p->object->OutlineColor = 0;
    }
}

int ObjInteraction( Obj_t *obj, int Xpos, int Ypos )
{
    ArtFrmHdr_t *v5, *v6, *Data;
    CachePool_t *Block;
    Proto_t *proto;
    char *ObjData, v13;
    int x0,y0,x1,y1,Flags,v15,FlgExt,v17,v18,Height,Width,pY,pX,ObjFlg;

    ObjFlg = 0;
    if( obj == gObjRadius || (obj->Flags & 1) == 0 ){
        v5 = ArtLoadImg(obj->ImgId, &Block);
        v6 = v5;
        Data = v5;
        if( v5 ){
            ArtGetObjSize(v5, obj->FrameNo, obj->Orientation, &Width, &Height);
            if( obj->GridId == -1 ){
                x0 = obj->Sx;
                y0 = obj->Sy;
                x1 = x0 + Width - 1;
                y1 = y0 + Height - 1;
            } else {
                TileGetScrCoordinates(obj->GridId, &pX, &pY);
                pX += 16;
                pY += 8;
                pX += v6->PixShiftX[ obj->Orientation ];
                pY += v6->PixShiftY[ obj->Orientation ];
                pX += obj->PosX;
                pY += obj->PosY;
                x0 = pX - Width / 2;
                y0 = pY - Height + 1;
                y1 = pY;
                x1 = x0 + Width - 1;
            }
            if( x0 <= Xpos && x1 >= Xpos && Ypos >= y0 && Ypos <= y1 ){
                ObjData = ArtGetObjData( Data, obj->FrameNo, obj->Orientation );
                if( ObjData ){
                    if( ObjData[Width * (Ypos - y0) + Xpos - x0] ){
                        v13 = ObjFlg | 1;
                        Flags = obj->Flags;
                        ObjFlg = ObjFlg | 1;
                        if( (Flags & 0xFC000) != 0 ){
                            if( (obj->Flags & 0x8000) == 0 ) ObjFlg = (v13 & ~0x03) | 2;
                        } else {
                            v15 = (obj->ImgId & 0xF000000) >> 24;
                            if( v15 == 3 || v15 == 2 ){
                                ProtoGetObj(obj->Pid, &proto);
                                FlgExt = proto->FlgExt;
                                if( (FlgExt & 0x8000000) != 0 || FlgExt < 0 ){
                                    v17 = TileUnk14(obj->GridId, gObjDude->GridId);
                                } else if( (FlgExt & 0x10000000) != 0 ){
                                    v18 = TileUnk14(obj->GridId, gObjDude->GridId);
                                    v17 = v18 | TileUnk15(gObjDude->GridId, obj->GridId);
                                } else if ( (FlgExt & 0x20000000) != 0 ){
                                    v17 = TileUnk14(obj->GridId, gObjDude->GridId) && TileUnk15(gObjDude->GridId, obj->GridId);
                                } else {
                                    v17 = TileUnk15(gObjDude->GridId, obj->GridId);
                                }
                                if( v17 && ObjInteraction(gObjRadius, Xpos, Ypos) ) ObjFlg = ObjFlg | 4;
                            }
                        }
                    }
                }
            }
            ArtClose(Block);
        }
    }
    return ObjFlg;
}

// creates interaction objects list
int ObjTabCreate( int Xpos, int Ypos, int MapLvl, int Type, ObjTable_t **pTable )
{
    ObjList_t *list;
    int flg;
    ObjTable_t *t;
    int pt,e,tg,i,cnt;

    cnt = 0;
    pt = TileGetPointed( Xpos - 320, Ypos - 240 );
    *pTable = NULL;
    tg = gTileCentIdx & 1;
    for( i = 0; i < gObjGridWinSize; i++ ){
        e = gObjUnk11[ tg ][ i ];
        if( gObjViewPortAxisY[ e ] >= 30 || gObjViewPortAxisX[ e ] >= 20 ) continue;
        if( !( list = gObjGridObjects[ pt + gObjViewPort[ tg ][ e ] ] ) ) continue;
        for( ;list; list = list->Next){
            if( list->object->Elevation > MapLvl ) break;
            if( list->object->Elevation != MapLvl || !(Type == -1 || OBJTYPE( list->object->ImgId ) == Type) || list->object == gObjRadius ) continue;
            if( !(flg = ObjInteraction( list->object, Xpos, Ypos ) ) ) continue;
            if( !(t = Realloc( *pTable, (cnt + 1) * sizeof( ObjTable_t ) ) ) ) continue;
            *pTable = t;
            t[ cnt ].obj = list->object;
            t[ cnt ].Flags = flg;
            cnt++;
        }
    }        
    return cnt;
}

void ObjTabDestroy( ObjTable_t **tab )
{
    if( !tab ) return;    
    if( !*tab ) return;
    Free( *tab );
    *tab = NULL;
}

void ObjUnk77( int a1 )
{
    gObjUnk01[ a1 >> 3 ] |= 1 << (a1 & 7);
}

void ObjUnk78()
{
    memset( gObjUnk01, 0, 5001 );
}

void ObjUnk79()
{
    ObjList_t *p;
    unsigned char mask;
    int i, j;

    memset( gObjUnk79, 0, sizeof( gObjUnk79 ) );
    for( i = 0; i < 5001; i++){
        if( !gObjUnk01[ i ] ) continue;
        for( j = i - 400; j != 400 + i; j += 25 ){
            if( j > 5000 || j < 0 ) continue;
            gObjUnk79[ j ] = -1;
            if( j > 0 ) gObjUnk79[ j - 1 ] = -1;
            if( j < 5000 ) gObjUnk79[ j + 1 ] = -1;
            if( j > 1 ) gObjUnk79[ j - 2 ] = -1;
            if( j < 4999 ) gObjUnk79[ j + 2 ] = -1;
        }            
    }    
    for( i = 0; i < 5001; i++ ){
        if( !gObjUnk79[ i ] ) continue;
        mask = 1;
        for( j = 0; j < 8; j++, mask <<= 1 ){
            if( (mask & gObjUnk79[ i ] ) && j < 40000 ){
                for( p = gObjGridObjects[ i * 8 + i ]; p; p = p->Next ){
                    if( p->object->Elevation == gObjDude->Elevation ) p->object->Flags |= 0x40000000;
                }
            }                
        }
    }
    memset( gObjUnk01, 0, sizeof( gObjUnk01 ) );
}

char *ObjGetName( Obj_t *obj )
{
    ProtoType_e type;

    type = OBJTYPE( obj->ImgId );
    if( type == TYPE_ITEM ) return ItemGetName( obj );
    if( type == TYPE_CRIT ) return CritterGetName( obj );
    return ProtoGetObjName( obj->Pid );
}

char *ObjGetDsc( Obj_t *obj )
{
    if( OBJTYPE( obj->ImgId ) == TYPE_ITEM ) return ItemGetDsc( obj );
    return ProtoGetDudeDsc( obj->Pid );
}

void ObjUnk80( int flg )
{
    int i;
    unsigned int tmp;
    char atmp[4096];
    CachePool_t *ImgObj;

    if( !gObjArtTable ) return;    
    memset( atmp, 0x00, sizeof( atmp ) );
    if( !(flg & 2) ){
        for( i = 0; i != 100*100; i++ ){
            atmp[ gMapIsoGrid[ 0 ][ i ] & 0xFFF ] = 1;
            atmp[ gMapIsoGrid[ 0 ][ i ] & 0xFFF ] = 1;
        }
    }
    if( (flg & 4) == 0 ){
        for( i = 0; i != 100 * 100; i++ ){
            atmp[ gMapIsoGrid[ 1 ][ i ] & 0xFFF ] = 1;
            atmp[ gMapIsoGrid[ 1 ][ i ] & 0xFFF ] = 1;
        }
    }
    if( (flg & 8) == 0 ){
        for( i = 0; i != 100 * 100; i++ ){
            atmp[ gMapIsoGrid[ 2 ][ i ] & 0xFFF ] = 1;
            atmp[ gMapIsoGrid[ 2 ][ i ] & 0xFFF ] = 1;
        }
    }
    qsort( gObjArtTable, gObjCnt, sizeof( int ), (void *)ObjArtSort );
    for( tmp = gObjCnt; OBJTYPE( gObjArtTable[ --tmp ] ) == TYPE_WALL; );
    tmp++;
    if( ArtLoadImg( gObjArtTable[0], &ImgObj ) ) ArtClose( ImgObj );
    
    for( i = 1; i < tmp; i++ ){
        if( gObjArtTable[ i - 1 ] != gObjArtTable[ i ] && ArtLoadImg( gObjArtTable[ i ], &ImgObj ) ) ArtClose( ImgObj );
    }

    for( i = 0; i < 0x1000; i++ ){
        if( atmp[ i ] ){
            if( ArtLoadImg( ArtMakeId( 4, i, 0, 0, 0 ), &ImgObj ) ) ArtClose( ImgObj );
        }
    }
    for( i = tmp; i < gObjCnt; i++ ){
        if( gObjArtTable[ i - 1 ] != gObjArtTable[ i ] ){
            if( ArtLoadImg( gObjArtTable[ i ], &ImgObj ) ) ArtClose( ImgObj );
        }
    }
    Free( gObjArtTable );
    gObjArtTable = NULL;
    gObjCnt = 0;    
}

int ObjUnk81()
{
    memset( gObjGridObjects, 0x00, 40000 * sizeof( int ));
    return 0;
}

int ObjViewPortInit()
{
    int Pointed,v3,v4,*v5,xd,v8,i,y0,x0,j,v22,toggle;

    if( !gObjViewPort[0] && !gObjViewPort[1] ){
        gObjViewPort[0] = Malloc(4 * gObjGridWinSize);
        if( gObjViewPort[0] ){
            gObjViewPort[1] = Malloc(4 * gObjGridWinSize);
            if( gObjViewPort[1] ){
                j = 0;
                do{
                    Pointed = TileGetPointed(gObjViewPortArea.lt, gObjViewPortArea.tp);
                    v3 = gTileCentIdx & 1;
                    v4 = Pointed;
                    if( Pointed != -1 ){
                        v5 = gObjViewPort[v3];
                        TileGetScrCoordinates(Pointed, &x0, &y0);
                        toggle = 16;
                        x0 += 16;
                        y0 += 8;
                        if( x0 > gObjViewPortArea.lt ) toggle = -16;
                        xd = x0;
                        v22 = 0;
                        if( gObjGridWinHeight > 0 ){
                            while( 1 ){
                                i = 0;
                                if( gObjGridWinWidth > 0 ) break;
LABEL_13:
                                xd = toggle + x0;
                                y0 += 12;
                                ++v22;
                                toggle = -toggle;
                                if( v22 >= gObjGridWinHeight ) goto LABEL_14;
                            }
                            while( 1 ){
                                v8 = TileGetPointed(xd, y0);
                                if( v8 == -1 ) goto LABEL_25;
                                *v5 = v8 - v4;
                                ++v5;
                                ++i;
                                xd += 32;
                                if( i >= gObjGridWinWidth ) goto LABEL_13;
                            }
                        }
                    }
LABEL_14:
                    if( TileSetCenter(gTileCentIdx + 1, 2) == -1 ) goto LABEL_25;
                    ++j;
                }while ( j < 2 );
                gObjViewPortAxisY = Malloc( gObjGridWinSize * sizeof( int ) );
                if( gObjViewPortAxisY ){
                    for( i = 0; i < gObjGridWinSize; i++ ){
                        gObjViewPortAxisY[ i ] = i / gObjGridWinWidth;
                    }
                    gObjViewPortAxisX = Malloc( gObjGridWinSize * sizeof( int ) );
                    if( gObjViewPortAxisX ){
                        for( i = 0; i < gObjGridWinSize; i++ ){
                            gObjViewPortAxisX[ i ] = i % gObjGridWinWidth;
                        }
                        return 0;
                    }
                }
            }
        }
LABEL_25:
        ObjFree();
    }
    return -1;
}

void ObjFree()
{
    if( gObjViewPortAxisX ){ Free( gObjViewPortAxisX ); gObjViewPortAxisX = NULL; }
    if( gObjViewPortAxisY ){ Free( gObjViewPortAxisY ); gObjViewPortAxisY = NULL; }
    if( gObjViewPort[ 1 ] ){ Free( gObjViewPort[ 1 ] ); gObjViewPort[ 1 ] = NULL; }
    if( gObjViewPort[ 0 ] ){ Free( gObjViewPort[ 0 ] ); gObjViewPort[ 0 ] = NULL; }
}

int ObjUnk84()
{
    int i;
    if( gObjUnk11[ 0 ] || gObjUnk11[ 1 ] ) return -1;
    if( !(gObjUnk11[ 0 ] = (int *)Malloc( gObjGridWinSize * sizeof( int ) )) ) goto err;
    if( !(gObjUnk11[ 1 ] = (int *)Malloc( gObjGridWinSize * sizeof( int ) )) ){
err:
        if( gObjUnk11[ 1 ] ){ Free( gObjUnk11[ 1 ] ); gObjUnk11[ 1 ] = NULL; }
        if( gObjUnk11[ 0 ] ){ Free( gObjUnk11[ 0 ] ); gObjUnk11[ 0 ] = NULL; }
        return -1;
    }
    for( i = 0; i < gObjGridWinSize; i++ ){
        gObjUnk11[ 0 ][ i ] = i;
        gObjUnk11[ 1 ][ i ] = i;
    }
    qsort( gObjUnk11[ 0 ], gObjGridWinSize, sizeof( int ), (void *)ObjSortA );
    qsort( gObjUnk11[ 1 ], gObjGridWinSize, sizeof( int ), (void *)ObjSortB );
    return 0;
}

int ObjSortA( int *a1, int *a2 )
{
    return gObjViewPort[ 0 ][ *a1 ] - gObjViewPort[ 0 ][ *a2 ];
}

int ObjSortB( int *a1, int *a2 )
{
    return gObjViewPort[ 1 ][ *a1 ] - gObjViewPort[ 1 ][ *a2 ];
}

void ObjUnk87()
{
    if( gObjUnk11[1] ){
        Free( gObjUnk11[ 1 ] );
        gObjUnk11[ 1 ] = NULL;
    }
    if( gObjUnk11[ 0 ] ){
        Free( gObjUnk11[ 0 ] );
        gObjUnk11[ 0 ] = NULL;
    }
}

int ObjAllocRenderList()
{
    ObjList_t **p;
    int i;

    if( gObjRenderList ) return -1;
    if( !(p = (ObjList_t **)Malloc( gObjGridWinSize * sizeof( ObjList_t ** ) )) ) return -1;
    for( i = 0; i < gObjGridWinSize; i++ ) p[ i++ ] = NULL;    
    gObjRenderList = p;
    return 0;
}

void ObjUnk89()
{
    if( !gObjRenderList ) return;
    Free( gObjRenderList );
    gObjRenderList = NULL;
}

void ObjUnk90()
{
    int idx,p,e,k,j,n,i,jm;

    e = 864 * (gTileCentIdx & 1);             // 2x6x8x18
    for( i = 0; i < 6; i++, e += 144 ){
        n = 0;         
        for( j = 0, jm = 8; j < 8; j++, jm-- ){
            p = TileGetTileNumInDir( gTileCentIdx, (i + 1) % 6, j );
            for( k = 0; k < jm; k++, n++){
                gObjLight[ e + 4*n + k ] = TileGetTileNumInDir( p, i, k + 1 ) - gTileCentIdx;
            }            
        }
    }

    e = 864 * ((gTileCentIdx + 1) & 1);
    for( i = 0; i < 6; i++, e += 144 ){
        n = 0;
        for( j = 0, jm = 8; j < 8; j++, jm-- ){
            p = TileGetTileNumInDir( gTileCentIdx + 1, (i + 1) % 6, j );
            idx = e + 4 * n;
            for( k = 0; k < jm; n++, k++, idx++ ){
                gObjLight[ idx ] = TileGetTileNumInDir( p, i, k + 1 ) - (gTileCentIdx + 1);
            }            
        }
    }
}

int ObjColorInit()
{
    unsigned int r, g, b;
    int i;

    for( i = 0; i < 256; i++ ){
        r = RGB16_GET_R( PalConvColor8to16( i ) );        
        b = RGB16_GET_B( PalConvColor8to16( i ) );
        g = RGB16_GET_G( PalConvColor8to16( i ) );
        gObjPalRY[ i ] = ((r + 5 * g + 4 * b) / 10) >> 2;
        gObjPalBY[ i ] = ((b + 3 * r + 6 * g) / 10) >> 2;
    }
    gObjPalRY[ 0 ] = 0;
    gObjPalBY[ 0 ] = 0;
    gObjShadeWall = PalExtOpen( gPalColorCubeRGB[24][26][31] );
    gObjShadeGlass = PalExtOpen( gPalColorCubeRGB[9][31][31] );
    gObjShadeSteam = PalExtOpen( gPalColorCubeRGB[31][31][31] );
    gObjShadeEnergy = PalExtOpen( gPalColorCubeRGB[29][31][1] );
    gObjShadeRed = PalExtOpen( gPalColorCubeRGB[31][0][0] );
    return 0;
}

void ObjUnk91()
{
    PalExtClose( gPalColorCubeRGB[24][26][31] );
    PalExtClose( gPalColorCubeRGB[9][31][31] );
    PalExtClose( gPalColorCubeRGB[31][31][31] );
    PalExtClose( gPalColorCubeRGB[29][31][1] );
    PalExtClose( gPalColorCubeRGB[31][0][0] );
}

void ObjUnk92()
{
    gObjUnk12 = TileGetPointed( gObjViewPortArea.lt, gObjViewPortArea.tp ) - gTileCentIdx;
}

int ObjSaveDude( xFile_t *fh, Obj_t *dude )
{
    Obj_t *WhoHitMe;
    int i;

    if( dude->Flags & 0x04 ) return 0;
    if( OBJTYPE( dude->Pid ) == 1 ){
        WhoHitMe = dude->Critter.State.WhoHitMeObj;
        if( dude->Critter.State.WhoHitMeObj ){
            if( dude->Critter.State.WhoHitMe != -1 ) dude->Critter.State.WhoHitMe = WhoHitMe->CritterIdx;
        } else {
            dude->Critter.State.WhoHitMe = -1;
        }
    }
    if( ObjSave( dude, fh ) ) return -1;
    if( OBJTYPE( dude->Pid ) == 1 ) dude->Critter.State.WhoHitMeObj = WhoHitMe;
    for( i = 0; i < dude->Critter.Box.Cnt; i++ ){
        if( dbputBei( fh, dude->Critter.Box.Box[ i ].Quantity ) ) return -1;
        if( ObjSaveDude( fh, dude->Critter.Box.Box[ i ].obj ) == -1 || (dude->Critter.Box.Box[ i ].obj->Flags & 0x04) ) return -1;
    }
    return 0;
}

int ObjLoadItems( xFile_t *fh, Obj_t **stack, int Lvl )
{
    Scpt_t *scr;
    ObjStack_t tmp;
    int i;

    if( ObjNewObj( &tmp.obj ) == -1 ){ *stack = tmp.obj; return 0; }
    if( ObjLoadObj( tmp.obj, fh ) ){ *stack = tmp.obj; return 0; }
    tmp.obj->OutlineColor = 0;
    if( tmp.obj->ScrId != -1 ){
        if( ScptPtr( tmp.obj->ScrId, &scr ) == -1 )
            tmp.obj->ScrId = -1;
        else
            scr->TimeEv = tmp.obj;
    }
    ObjGetArtFileId( &tmp.obj->ImgId );
    if( !ArtFileExist1( tmp.obj->ImgId ) ){
        eprintf( "\nError: invalid object art fid: %u", tmp.obj->ImgId );
        if( tmp.obj ){ Free( tmp.obj ); tmp.obj = NULL; }
        return -2;
    }
    if( Lvl == -1 )
        Lvl = tmp.obj->Elevation;
    else
        tmp.obj->Elevation = Lvl;

    tmp.obj->Owner = NULL;
    // no items in container
    if( tmp.obj->Container.Box.Cnt <= 0 ){
        tmp.obj->Container.Box.Capacity = 0;
        tmp.obj->Container.Box.Box = NULL;
        *stack = tmp.obj; 
        return 0;
    }
    if( !(tmp.obj->Container.Box.Box = Malloc( tmp.obj->Container.Box.Capacity * sizeof( ObjStack_t ) )) ) return -1;
    for( i = 0; i < tmp.obj->Container.Box.Cnt; i++ ){
	if( dbgetBei( fh, &tmp.obj->Container.Box.Box[ i ].Quantity ) ) return -1;
        if( ObjLoadItems( fh, &tmp.obj->Container.Box.Box[ i ].obj, Lvl ) == -1 ) return -1;
    }
    *stack = tmp.obj; 
    return 0;
}

int ObjFSave( xFile_t *fh )
{
    int Id;

    Id = gObjDude->ScrId;
    gObjDude->Flags &= ~0x04;
    gObjDude->ScrId = -1;
    ObjSaveDude( fh, gObjDude );
    gObjDude->ScrId = Id;
    gObjDude->Flags |= 0x04;
    if( dbputBei( fh, gObjUnk21 ) != -1 ) return 0;
    dbClose( fh );
    return -1;
}

int ObjLoadDude( xFile_t *fh )
{
    int i, ObjId, pint, Orientation, Elevation, GridId, Items;
    Obj_t *tmp;
    int NewPos, NewLvl, NewOri;

    pint = -1;
    GridId = gObjDude->GridId;
    Elevation = gObjDude->Elevation;
    Orientation = gObjDude->Orientation;
    ObjId = gObjDude->TimeEv;
    
    ScptClearDudeScript();
    Items = ObjLoadItems( fh, &tmp, -1 );
    memcpy( gObjDude, tmp, sizeof( Obj_t ) );
    gObjDude->Flags |= 0x04;
    ScptClearDudeScript();
    gObjDude->TimeEv = ObjId;
    ScptSetDudeScript();
    NewPos = gObjDude->GridId;
    gObjDude->GridId = GridId;
    NewLvl = gObjDude->Elevation;
    gObjDude->Elevation = Elevation;
    NewOri = gObjDude->Orientation;
    gObjDude->Orientation = Orientation;

    if( ScptSetDudeScript() != -1 ){
        ObjMoveToTile( gObjDude, NewPos, NewLvl, NULL );
        ObjSetRotation( gObjDude, NewOri, 0 );
    }

    for( i = 0; i < gObjDude->Critter.Box.Cnt; i++ ){
        gObjDude->Critter.Box.Box[ i ].obj->Owner = gObjDude;
    }
    ObjLookForAgressor();
    tmp->Critter.Box.Capacity = 0;
    tmp->Critter.Box.Box = NULL;
    tmp->Critter.Box.Cnt = 0;
    tmp->Flags &= ~OBJ_FLG_NOTREMOVE;
    if( ObjDestroy( tmp, 0 ) == -1 ) eprintf( "\nError: obj_load_dude: Can't destroy temp object!\n" );
    InvSelectMain();
    if( dbgetBei( fh, &pint ) == -1 ){ 
	dbClose( fh ); return -1; 
    } else {
        TileSetCenter( pint, 3 );
    }
    return Items;
}

int ObjNewObj( Obj_t **stack )
{
    if( !stack ) return -1;    
    if( !( *stack = Malloc( sizeof( Obj_t ) ) ) ) return -1;

    memset( *stack, 0, sizeof( Obj_t ) );
    (*stack)->TimeEv = -1;
    (*stack)->GridId = -1;
    (*stack)->CritterIdx = -1;
    (*stack)->OutlineColor = 0;
    (*stack)->Pid = -1;
    (*stack)->ScrId = -1;
    (*stack)->Owner = NULL;
    (*stack)->ScrFNameId = -1;
    return 0;
}

void ObjUnk95( void **a1 )
{
    if( !a1 ) return;
    if( !*a1 ) return;
    Free( *a1 );
    *a1 = NULL;    
}

int ObjAllocList( ObjList_t **list )
{
    ObjList_t *p;

    if( !list ) return -1;
    p = (ObjList_t *)Malloc( sizeof( ObjList_t ) );
    *list = p;
    if( !p ) return -1;
    p->Next = NULL;
    (*list)->object = NULL;
    return 0;
}

void ObjFreeList( ObjList_t **list )
{
    if( !list ) return;
    if( !*list ) return;
    Free( *list );
    *list = NULL;
}

int ObjFindInList( Obj_t *obj, ObjList_t **list_cur, ObjList_t **list_prev )
{
    if( !obj || !list_cur ) return -1;

    *list_cur = (obj->GridId == -1 ) ? gObjOffGridObjs : gObjGridObjects[ obj->GridId ];
    if( list_prev ){
        *list_prev = NULL;
        for( ; *list_cur && (obj != (*list_cur)->object); *list_cur = (*list_cur)->Next ){
            *list_prev = *list_cur;
        }
    } else {
        for( ; *list_cur && ( obj != ( *list_cur)->object ); *list_cur = (*list_cur)->Next );
    }
    return ( *list_cur ) ? 0 : -1;
}

void ObjAddObject( ObjList_t *NewObj )
{
    int GridId, a, b;
    ObjList_t **p;
    Obj_t *object;
    ArtFrmHdr_t *Art1, *Art2;
    CachePool_t *ImgObj1, *ImgObj2;

    if( !NewObj ) return;        
    if( (GridId = NewObj->object->GridId) == -1 ){
    	NewObj->Next = gObjOffGridObjs;
    	gObjOffGridObjs = NewObj;
    	return;
    }    
    Art2 = NULL;
    for( p = &gObjGridObjects[ GridId ]; *p; p = &(*p)->Next ){ // add object on the same grid position
        object = (*p)->object;
        if( object->Elevation > NewObj->object->Elevation ) break;
        if( object->Elevation < NewObj->object->Elevation ) continue;
        if( (object->Flags & PRFLG_FLAT) == 0 && (NewObj->object->Flags & PRFLG_FLAT) != 0 ) break;
        if( (object->Flags & PRFLG_FLAT) != (NewObj->object->Flags & PRFLG_FLAT) ) continue;
        if( !( Art1 = ArtLoadImg( object->ImgId, &ImgObj1 ) ) ) continue;
        if( !Art2 ) Art2 = ArtLoadImg( NewObj->object->ImgId, &ImgObj2 );
if( !Art2 ) break; // added
        a = Art1->PixShiftY[ object->Orientation ] + object->PosY;
        b = Art2->PixShiftY[ NewObj->object->Orientation ] + NewObj->object->PosY;
        if( b < a ){ ArtClose( ImgObj1 ); break; }
        if( b == a ){
            if( Art1->PixShiftX[ object->Orientation ] + object->PosX > NewObj->object->PosX + Art2->PixShiftX[ NewObj->object->Orientation ] ){ ArtClose( ImgObj1 ); break; }
        }
        ArtClose( ImgObj1 );
    } 
    if( Art2 ) ArtClose( ImgObj2 );
    NewObj->Next = *p;
    *p = NewObj;
}

int ObjDelete( ObjList_t *obl1, ObjList_t *obl2 )
{
    Obj_t *obj;
    int gid;

    obj = obl1->object;
    if( !obj || (obj->Flags & 0x400) ) return -1;
    ObjBoxDelete( &obj->Container ); // remove object container
    if( obl1->object->ScrId != -1 ){ // remove scripts
        ScptRun( obl1->object->ScrId, SCPT_AEV_DESTROY_P_PROC );
        ScptRemove( obl1->object->ScrId );
    }
    if( obl2 != obl1 ){
        if( obl2 ){
            obl2->Next = obl1->Next;
        } else {
            gid = obl1->object->GridId;
            if( gid == -1 )
                gObjOffGridObjs = gObjOffGridObjs->Next;
            else
                gObjGridObjects[ gid ] = gObjGridObjects[ gid ]->Next;
        }
    }
    if( obl1 && obl1->object ){ Free( obl1->object ); obl1->object = NULL; } // delete object
    if( obl1 ){ Free( obl1 ); obl1 = NULL; } // remove link list
    return 0;
}

int ObjAddObjToList( ObjList_t *pList, int GridPos, int MapLvl, VidRect_t *Area )
{
    if( !pList || GridPos > 39999 || MapLvl > 2 ) return -1;
    pList->object->GridId = GridPos;
    pList->object->Elevation = MapLvl;
    pList->object->PosX = 0;
    pList->object->PosY = 0;
    pList->object->Owner = 0;
    ObjAddObject( pList );
    if( ObjLight( pList->object, GridPos, Area ) == -1 ){
        if( Area ) ObjGetRadiusArea( pList->object, Area );
    }
    return 0;
}

int ObjLight( Obj_t *obj, int Dark, VidRect_t *a3 )
{
    VidRect_t *v32,*v33,*v35,Area2,RectRadius,*r;
    Proto_t *proto;
    ObjList_t *v23;
    Obj_t *v34, *object, *v27;
    unsigned int j,idx, i_, v47, v48;
    char *v44, *v49, flg;
    int v444,i,v77,v21,v24;
    int Elevation,v28,v29,FlgExt,v31,Tab[36],pY,pX,v50,Pos,v53;
    void (*Cb)(int, int, int);

    r = a3;
    if( !obj ) return -1;
    if( obj->LightIntensity <= 0 ) return -1;
    flg = obj->Flags;
    if( (flg & 1) != 0 || (flg & 0x20) == 0 || obj->GridId > 39999u ) return -1;
    if( Dark ){
        Cb = (void (*)(int, int, int))ItemGridLightDec;
        ItemGridLightDec(obj->Elevation, obj->GridId, obj->LightIntensity);
    } else {
        Cb = (void (*)(int, int, int))ItemGridLightInc;
        ItemGridLightInc(obj->Elevation, obj->GridId, obj->LightIntensity);
    }
    ObjGetRadiusArea(obj, &RectRadius);
    if( obj->LightRadius > 8 ) obj->LightRadius = 8;
    if( obj->LightIntensity > 0x10000 ) obj->LightIntensity = 0x10000;
    v44 = &gObjLight[864 * (obj->GridId & 1)];
    v444 = (obj->LightIntensity - 655) / (obj->LightRadius + 1);
    Tab[0] = obj->LightIntensity - v444;
    Tab[1] = Tab[0] - v444;
    Tab[8] = Tab[0] - v444;
    Tab[2] = Tab[0] - v444 - v444;
    Tab[9] = Tab[2];
    Tab[15] = Tab[2];
    Tab[3] = Tab[2] - v444;
    Tab[10] = Tab[2] - v444;
    Tab[16] = Tab[2] - v444;
    Tab[21] = Tab[2] - v444;
    Tab[4] = Tab[2] - v444 - v444;
    Tab[11] = Tab[4];
    Tab[17] = Tab[4];
    Tab[22] = Tab[4];
    Tab[26] = Tab[4];
    Tab[5] = Tab[4] - v444;
    Tab[12] = Tab[4] - v444;
    Tab[18] = Tab[4] - v444;
    Tab[23] = Tab[4] - v444;
    Tab[27] = Tab[4] - v444;
    Tab[30] = Tab[4] - v444;
    i = 0;
    Tab[6] = Tab[4] - v444 - v444;
    Tab[13] = Tab[6];
    Tab[19] = Tab[6];
    Tab[24] = Tab[6];
    Tab[28] = Tab[6];
    Tab[31] = Tab[6];
    Tab[33] = Tab[6];
    i_ = 0;
    Tab[7] = Tab[6] - v444;
    Tab[14] = Tab[6] - v444;
    Tab[20] = Tab[6] - v444;
    Tab[25] = Tab[6] - v444;
    Tab[29] = Tab[6] - v444;
    Tab[32] = Tab[6] - v444;
    Tab[34] = Tab[6] - v444;
    Tab[35] = Tab[6] - v444;
    do{
        if( obj->LightRadius >= gObjUnk38[i_ / 4] ){
            v48 = i_;
            v47 = i_;
            v49 = &v44[i_];
            j = 0;
            v50 = i_;
            do{
                v77 = (int)(j + 1) % 6;
                if( (unsigned int)i <= 35 ){
                    switch( v47 ){
                        case 0u:
                            v53 = 0;
                            break;
                        case 1u:
                            v53 = gObjUnk52[j][0];
                            break;
                        case 2u:
                            v53 = gObjUnk52[j][1];
                            break;
                        case 3u:
                            v53 = gObjUnk52[j][2];
                            break;
                        case 4u:
                            v53 = gObjUnk52[j][3];
                            break;
                        case 5u:
                            v53 = gObjUnk52[j][4];
                            break;
                        case 6u:
                            v53 = gObjUnk52[j][5];
                            break;
                        case 7u:
                            v53 = gObjUnk52[j][6];
                            break;
                        case 8u:
                            v53 = gObjUnk52[v77][0] & gObjUnk52[j][0];
                            break;
                        case 9u:
                            v53 = gObjUnk52[j][1] & gObjUnk52[j][8];
                            break;
                        case 10u:
                            v53 = gObjUnk52[j][2] & gObjUnk52[j][9];
                            break;
                        case 11u:
                            v53 = gObjUnk52[j][3] & gObjUnk52[j][10];
                            break;
                        case 12u:
                            v53 = gObjUnk52[j][4] & gObjUnk52[j][11];
                            break;
                        case 13u:
                            v53 = gObjUnk52[j][5] & gObjUnk52[j][12];
                            break;
                        case 14u:
                            v53 = gObjUnk52[j][6] & gObjUnk52[j][13];
                            break;
                        case 15u:
                            v53 = gObjUnk52[v77][1] & gObjUnk52[j][8];
                            break;
                        case 16u:
                            v53 = gObjUnk52[j][15] & (gObjUnk52[j][9] | gObjUnk52[j][8]);
                            break;
                        case 17u:
                            v53 = 
                        	((gObjUnk52[j][15] | gObjUnk52[j][10]) & gObjUnk52[j][9]) | 
                        	(gObjUnk52[j][16] & (gObjUnk52[j][10] | gObjUnk52[j][9])) | 
                        	((gObjUnk52[j][10] | gObjUnk52[j][9]) & gObjUnk52[j][8]);
                            break;
                        case 18u:
                            v53 = (gObjUnk52[j][11] | gObjUnk52[j][10] | gObjUnk52[j][9] | gObjUnk52[j][0]) & (gObjUnk52[j][17] | gObjUnk52[j][9] | gObjUnk52[j][16]) & gObjUnk52[j][10];
                            break;
                        case 19u:
                            v53 = 
                        	(gObjUnk52[j][18] & (gObjUnk52[j][12] | gObjUnk52[j][10] | gObjUnk52[j][9])) | 
                        	((gObjUnk52[j][18] | gObjUnk52[j][17]) & gObjUnk52[j][11]);
                            break;
                        case 20u:
                            v53 = 
                        	((gObjUnk52[j][19] | gObjUnk52[j][18] | gObjUnk52[j][17] | gObjUnk52[j][16]) & gObjUnk52[j][11]) | 
                        	((gObjUnk52[j][10] | gObjUnk52[j][9]) & (gObjUnk52[j][12] | gObjUnk52[j][11] | gObjUnk52[j][2]))| 
                        	((gObjUnk52[j][12] | gObjUnk52[j][11] | gObjUnk52[j][2]) & gObjUnk52[j][8]);
                            break;
                        case 21u:
                            v53 = (gObjUnk52[v77][2] & gObjUnk52[j][15]) | (gObjUnk52[v77][1] & gObjUnk52[j][8]);
                            break;
                        case 22u:
                            v53 = 
                        	(gObjUnk52[j][16] & (gObjUnk52[j][21] | gObjUnk52[j][15])) | 
                        	(gObjUnk52[j][15] & (gObjUnk52[j][21] | gObjUnk52[j][9])) | 
                        	((gObjUnk52[j][21] | gObjUnk52[j][15] | gObjUnk52[v77][1]) & gObjUnk52[j][8]);
                            break;
                        case 23u:
                            v53 = gObjUnk52[j][22] & ( gObjUnk52[j][17] | gObjUnk52[j][15] ) & ( gObjUnk52[j][9] | gObjUnk52[j][3] | gObjUnk52[j][16] );
                            break;
                        case 24u:
                            v53 = 
                        	( gObjUnk52[j][23] & gObjUnk52[j][18] ) | 
                        	( gObjUnk52[j][17] & (gObjUnk52[j][23] | gObjUnk52[j][22] | gObjUnk52[j][15]) ) | 
                        	(( gObjUnk52[j][8] | gObjUnk52[j][9] ) & (gObjUnk52[j][23] | gObjUnk52[j][16] | gObjUnk52[j][15] ) ) | 
                        	( ( gObjUnk52[j][18] | gObjUnk52[j][17] | gObjUnk52[j][10] | gObjUnk52[j][9] | gObjUnk52[j][0]) & gObjUnk52[j][16] );
                            break;
                        case 25u:
                            v53 = 
                        	( (gObjUnk52[j][19] | gObjUnk52[j][0]) & gObjUnk52[j][24] ) | 
                        	( gObjUnk52[j][18] & (gObjUnk52[j][24] | gObjUnk52[j][23] | (gObjUnk52[j][16] | gObjUnk52[j][8])) ) | 
                        	( (gObjUnk52[j][17] | gObjUnk52[j][10]) & (gObjUnk52[j][24] | (gObjUnk52[j][16] | gObjUnk52[j][8]) | gObjUnk52[j][17])) | 
                        	( gObjUnk52[j][1] & (gObjUnk52[j][8] | (gObjUnk52[j][24] | gObjUnk52[j][23] | gObjUnk52[j][16] | gObjUnk52[j][15] | gObjUnk52[j][8])) & gObjUnk52[j][9]);
                            break;
                        case 26u:
                            v53 = 
                        	( gObjUnk52[v77][3] & gObjUnk52[j][21]) |
                        	( gObjUnk52[j][8] & gObjUnk52[v77][1] ) | 
                        	( gObjUnk52[v77][2] & gObjUnk52[j][15] );
                            break;
                        case 27u:
                            v53 = 
                        	( gObjUnk52[j][16] | gObjUnk52[j][8] ) & 
                        	( gObjUnk52[j][21] | gObjUnk52[j][15] | gObjUnk52[v77][1] ) & 
                        	( (gObjUnk52[j][8] | ( gObjUnk52[j][26] | gObjUnk52[j][21] | gObjUnk52[j][15] | gObjUnk52[v77][0] )) & gObjUnk52[j][22] );
                            break;
                        case 28u:
                            v53 = 
                        	( gObjUnk52[j][27] & gObjUnk52[j][23] ) | 
                        	( gObjUnk52[j][22] & (gObjUnk52[j][23] | gObjUnk52[j][17] | gObjUnk52[j][9]) ) | 
                        	( gObjUnk52[j][16] & (gObjUnk52[j][27] | gObjUnk52[j][22] | gObjUnk52[j][21] | gObjUnk52[v77][0]) ) | 
                        	( ( gObjUnk52[j][8] | gObjUnk52[j][15]) & (gObjUnk52[j][23] | gObjUnk52[j][16] | gObjUnk52[j][9] ) );
                            break;
                        case 29u:
                            v53 = 
                        	( gObjUnk52[j][28] & gObjUnk52[j][24] ) | 
                        	( gObjUnk52[j][22] & gObjUnk52[j][17] ) | 
                        	( gObjUnk52[j][15] & ( gObjUnk52[j][9] | gObjUnk52[j][16] | gObjUnk52[j][8] | gObjUnk52[j][23] ) );
                            break;
                        case 30u:
                            v53 = 
                        	( gObjUnk52[v77][4] & gObjUnk52[j][26] ) | 
                        	( gObjUnk52[v77][2] & gObjUnk52[j][15] ) | 
                        	( gObjUnk52[ j ][8] & gObjUnk52[v77][1] ) | 
                        	( gObjUnk52[v77][3] & gObjUnk52[j][21] );
                            break;
                        case 31u:
                            v53 = 
                        	( gObjUnk52[j][30] & gObjUnk52[j][27] ) | 
                        	( gObjUnk52[j][26] & (gObjUnk52[j][27] | gObjUnk52[j][22] | gObjUnk52[j][8]) ) | 
                        	(( gObjUnk52[j][15] | gObjUnk52[v77][1] ) & ( gObjUnk52[j][8] | gObjUnk52[j][21] ));
                            break;
                        case 32u:
                            v53 = 
                        	( gObjUnk52[j][28] & (gObjUnk52[j][31] | gObjUnk52[j][0]) ) | 
                        	( gObjUnk52[j][27] & (gObjUnk52[j][28] | gObjUnk52[j][23] | gObjUnk52[j][16] | gObjUnk52[j][8]) ) | gObjUnk52[j][22] | 
                        	( gObjUnk52[v77][1] & gObjUnk52[j][8] ) | 
                        	( ( gObjUnk52[j][28] | gObjUnk52[j][23] | gObjUnk52[j][16] | gObjUnk52[j][9] | gObjUnk52[j][8]) & gObjUnk52[j][15] ) |
                        	( gObjUnk52[j][21] & ( gObjUnk52[j][16] | gObjUnk52[j][8] | gObjUnk52[j][28] ) );
                            break;
                        case 33u:
                            v53 = 
                        	( gObjUnk52[v77][5] & gObjUnk52[j][30] ) |                         	
                        	( gObjUnk52[v77][3] & gObjUnk52[j][21] ) | 
                        	( gObjUnk52[v77][2] & gObjUnk52[j][15] ) | 
                        	( gObjUnk52[v77][1] & gObjUnk52[j][ 8] ) | 
                        	( gObjUnk52[v77][4] & gObjUnk52[j][26] );
                            break;
                        case 34u:
                            v53 = 
                        	( ( gObjUnk52[j][31] | gObjUnk52[j][27] | gObjUnk52[j][22] | gObjUnk52[j][16]) & gObjUnk52[j][26] ) | 
                        	( ( gObjUnk52[j][21] | gObjUnk52[j][15] ) & ( gObjUnk52[j][30] | gObjUnk52[j][26] | gObjUnk52[v77][2] ) ) | 
                        	( ( gObjUnk52[j][30] | gObjUnk52[j][26] | gObjUnk52[v77][2] ) & gObjUnk52[j][8] );
                            break;
                        case 35u:
                            v21 = v77;
                            v53 = 
                        	( gObjUnk52[v21][4] & gObjUnk52[ j ][26] ) | 
                        	( gObjUnk52[v21][3] & gObjUnk52[ j ][21] ) | 
                        	( gObjUnk52[v21][2] & gObjUnk52[ j ][15] ) | 
                        	( gObjUnk52[ j ][8] & gObjUnk52[v21][ 1] ) | 
                        	( gObjUnk52[v21][5] & gObjUnk52[ j ][30] ) |
                        	( gObjUnk52[v21][6] & gObjUnk52[ j ][33] );
                            break;
                    }
                }
                if( !v53 ){
                    idx = *(int *)v49 + obj->GridId;
                    Pos = idx;
                    if( idx <= 39999 ){
                        v23 = gObjGridObjects[idx];
                        v24 = 1;
                        if( v23 ){
                            while( 1 ){
                                object = v23->object;
                                if( (v23->object->Flags & 1) == 0 ){
                                    Elevation = object->Elevation;
                                    if( Elevation > obj->Elevation ) goto LABEL_94;
                                    if( Elevation == obj->Elevation ) break;
                                }
LABEL_93:
                                v23 = v23->Next;
                                if( !v23 ) goto LABEL_94;
                            }
                            ObjGetRadiusArea(object, &Area2);
                            RegionExpand(&RectRadius, &Area2, &RectRadius);
                            v27 = v23->object;
                            v28 = (v23->object->Flags & 0x20000000) == 0;
                            v29 = (v23->object->ImgId & 0xF000000) >> 24;
                            v53 = v28;
                            if( v29 == 3 ){
                                if( (v27->Flags & 8) == 0 ){
                                    ProtoGetObj(v27->Pid, &proto);
                                    FlgExt = proto->FlgExt;
                                    if( (FlgExt & 0x8000000) != 0 || (FlgExt & 0x40000000) != 0 ){
                                        if ( j != 4 && j != 5 && (j || i >= 8) && (j != 3 || i <= 15) )
LABEL_91:
                                            v24 = 0;
                                    }else if( (FlgExt & 0x10000000) != 0 ){
                                        if( j && j != 5 ) goto LABEL_91;
                                    } else if ( (FlgExt & 0x20000000) != 0 ){
                                        if ( j >= 2 && j != 4 && j != 5 && (j != 3 || i <= 15) ) goto LABEL_91;
                                    } else if ( j >= 2 && (j != 5 || i <= 7) ){
                                        goto LABEL_91;
                                    }
                                }
                            } else if ( v28 && j && j <= 3 ){
                                goto LABEL_91;
                            }
                            if( v53 ) goto LABEL_94;
                            goto LABEL_93;
                        }
LABEL_94:
                        if( v24 ) Cb(obj->Elevation, Pos, *(int *)((char *)Tab + v48));
                    }
                }
                v31 = v50;
                j++;
                *(int *)((char *)gObjUnk52[0] + v50) = v53;
                v50 = v31 + 144;
                v49 += 144;
            }while( j < 6 );
        }
        i++;
        i_ += 4;
    }while( i < 36 );
    if( r ){
        v32 = r;
        v33 = &gObjUnk39[obj->LightRadius];
        v34 = obj;
        r->lt = v33->lt;
        v33 = (VidRect_t *)((char *)v33 + 4);
        v32 = (VidRect_t *)((char *)v32 + 4);
        v32->lt = v33->lt;
        v33 = (VidRect_t *)((char *)v33 + 4);
        v32 = (VidRect_t *)((char *)v32 + 4);
        v32->lt = v33->lt;
        v32->tp = v33->tp;
        TileGetScrCoordinates(v34->GridId, &pX, &pY);
        pX += 16;
        pY += 8;
        pX -= r->rt >> 1;
        v35 = r;
        pY -= r->bm >> 1;
        r->lt += pX;
        v35->tp += pY;
        v35->rt += pX;
        v35->bm += pY;
        RegionExpand(v35, &RectRadius, v35);
    }
    return 0;
}

void ObjRenderHexCursor( Obj_t *obj, VidRect_t *area )
{
    unsigned char *img, *surf, *v28, *Shade, *Palette, ColorCycle, Color, *surface, *ObjData, *m;
    ArtFrmHdr_t *v3, *v4, *Data;
    VidRect_t Area2, Area1, *v37;
    CachePool_t *Obj;
    int Orientation,FrameNo,ObjHeight,GridId,i,j,toggle,tmp,Height,Width,pY,pX,pitch,v6,v42,v43,v48,v49,v50,v51,v52,v10,v11,v12,v15;

    v37 = area;
    v3 = ArtLoadImg( obj->ImgId, &Obj );
    v4 = v3;
    Data = v3;
    if( !v3 ) return;
    Orientation = obj->Orientation;
    v48 = 0;
    v49 = 0;
    ArtGetObjSize( v3, obj->FrameNo, Orientation, &Width, &Height );
    v6 = obj->Orientation;
    FrameNo = obj->FrameNo;
    v50 = Width - 1;
    ObjHeight = ArtGetObjHeight( v4, FrameNo, v6 );
    GridId = obj->GridId;
    v51 = ObjHeight - 1;
    if( GridId == -1 ){
        Area1.lt = obj->Sx;
        Area1.tp = obj->Sy;
        Area1.rt = v50 + obj->Sx;
        Area1.bm = v51 + obj->Sy;
    } else {
        TileGetScrCoordinates(obj->GridId, &pX, &pY);
        pX += 16;
        pY += 8;
        pX += *(int *)(&v4->Fpd + obj->Orientation) >> 16;
        pY += *(int *)&v4->PixShiftX[obj->Orientation + 5] >> 16;
        pX += obj->PosX;
        v10 = pY + obj->PosY;
        Area1.lt = pX - (Width >> 1);
        Area1.tp = v10 - v51;
        pY = v10;
        Area1.bm = v10;
        Area1.rt = Width + Area1.lt - 1;
        obj->Sx = Area1.lt;
        obj->Sy = Area1.tp;
    }
    Area2 = *v37;
    Area2.lt--;
    Area2.tp--;
    Area2.bm++;
    Area2.rt++;
    RegionShrink(&Area2, &gObjUnk16, &Area2);
    if( RegionShrink(&Area1, &Area2, &Area1) ){ ArtClose( Obj ); return; }
    v11 = Area1.tp - obj->Sy + v49;
    v48 += Area1.lt - obj->Sx;
    v50 = v48 + Area1.rt - Area1.lt;
    v49 = v11;
    v12 = obj->Orientation;
    v51 = v11 + Area1.bm - Area1.tp;
    ObjData = (unsigned char *)ArtGetObjData(Data, obj->FrameNo, v12);
    img = ObjData;
    surface = (unsigned char *)&gObjIsoSurface[ gObjIsoPitch * obj->Sy + obj->Sx ];
    pitch = gObjIsoPitch - Width;
    v52 = obj->OutlineColor & 0x40000000;
    v15 = obj->OutlineColor & 0xFFFFFF;
    switch( v15 ){
        case 1: Color = -13; v42 = 5; v52 = 0; v43 = Height / 5; break;
        case 2: Color = gPalColorCubeRGB[31][0][0]; v43 = 0; if( v52 ){ Shade = (unsigned char *)gObjPalBY; Palette = (unsigned char *)gObjShadeRed; } break;
        case 4: Color = gPalColorCubeRGB[15][15][15]; v43 = 0;  if( v52 ){ Shade = (unsigned char *)gObjPalBY; Palette = (unsigned char *)gObjShadeWall; } break;
        case 8: Color = -27; v42 = 4; v52 = 0; v43 = Height / 4;  break;
        case 16: Color = gPalColorCubeRGB[29][29][8]; v43 = 0; if( v52 ){ Shade = (unsigned char *)gObjPalBY; Palette = (unsigned char *)gObjShadeRed;} break;
        case 32: Color = 61; v42 = 1; v52 = 0; v43 = Height; break;
        default: Color = gPalColorCubeRGB[31][0][31]; v43 = 0; v52 = 0; break;
    }
    ColorCycle = Color;
    surf = surface;
    for( i = 0; i < Height; surf += pitch, i++ ){
        toggle = 1;
        if( v43 ){
            if( !(i % v43) ) ColorCycle++;
            if( ColorCycle > v42 + Color - 1 ) ColorCycle = Color;
        }
        for( j = 0; j < Width; j++, img++, surf++ ){
	    tmp = surf - (unsigned char *)gObjIsoSurface;
            if( img[ 0 ] && toggle ){
                if( j >= v48 && j <= v50 && i >= v49 && i <= v51 && tmp > 0 && tmp % gObjIsoPitch ){
                    surf[ -1 ] = ( v52 ) ? Palette[ surf[ -1 ] + 256 * Shade[ ColorCycle ] ] : ColorCycle;
                }
                toggle = 0;
            } else if( !img[ 0 ] && !toggle ){
                if( j >= v48 && j <= v50 && i >= v49 && i <= v51 ){
                    surf[ 0 ] = ( v52 ) ? Palette[ surf[ 0 ] + 256 * Shade[ ColorCycle ] ] : ColorCycle;
                }
                toggle = 1;
            }
        }
        if( img[ -1 ] == 0 || tmp >= gObjUnk17 ) continue;
        tmp = j - 1;
        if( tmp >= v48 && tmp <= v50 && i >= v49 && i <= v51 ){
            surf[ 0 ] = ( v52 ) ? Palette[ surf[ 0 ] + 256 * Shade[ ColorCycle ] ] : ColorCycle;
        }
    }

    for( i = 0; i < Width; ++i ){        
        toggle = 1;
        surf = (unsigned char *)&surface[i];
        ColorCycle = Color;
        m = &ObjData[i];
        for( j = 0; j < Height; m += Width, surf += gObjIsoPitch, j++ ){
            if( v43 ){
                if( !(j % v43) ) ColorCycle++;
                if( ColorCycle > Color + v42 - 1 ) ColorCycle = Color;
            }
            if( m[ 0 ] && toggle ){
                if( i >= v48 && i <= v50 && j >= v49 && j <= v51 ){
                    v28 = &surf[ -gObjIsoPitch ];
                    if( &surf[-gObjIsoPitch] >= (unsigned char *)gObjIsoSurface ){
                        v28[ 0 ] = ( v52 ) ? Palette[ v28[ 0 ] + 256 * Shade[ ColorCycle ] ] : ColorCycle;
                    }
                }
                toggle = 0;
            } else if( !m[ 0 ] && !toggle ){
                if( i >= v48 && i <= v50 && j >= v49 && j <= v51 ){
                    surf[ 0 ] = ( v52 ) ? Palette[ surf[ 0 ] + 256 * Shade[ ColorCycle ] ] : ColorCycle;
                }
                toggle = 1;
            }                    
        }
        if( m[ -Width ] ){
            if( surf - (unsigned char *)gObjIsoSurface < gObjUnk17 ){
                tmp = j - 1;
                if( i >= v48 && i <= v50 && tmp >= v49 && tmp <= v51 ){
                    surf[ 0 ] = ( v52 ) ? Palette[ surf[ 0 ] + 256 * Shade[ ColorCycle ] ] : ColorCycle;
                }
            }
        }
    }    
    ArtClose( Obj );    
}

void ObjRender( Obj_t *obj, VidRect_t *Area, int Shade )
{
    ArtFrmHdr_t *Img, *Data;
    VidRect_t v28[ 4 ], AreaOut, rect, Area1;
    CachePool_t *Obj, *ImgObj;
    Proto_t *proto;
    char *ObjData;
    void *pSrc;
    int Orientation, ObjWidth, FlgExt, v16, pW, Id, h, w, ObjHight, tp, Height, Width, i;

    if( ArtIsHidden( OBJTYPE( obj->ImgId ) ) ) return;    
    if( !(Img = ArtLoadImg( obj->ImgId, &Obj )) ) return;
    Orientation = obj->Orientation;
    ObjWidth = ArtGetObjWidth( Img, obj->FrameNo, Orientation );
    ObjHight = ArtGetObjHeight( Img, obj->FrameNo, obj->Orientation ) - 1;
    if( obj->GridId == -1 ){ // off grid
        rect.lt = obj->Sx;
        rect.tp = obj->Sy;
        rect.rt = obj->Sx + ObjWidth - 1;
        rect.bm = obj->Sy + ObjHight;
    }else{
        TileGetScrCoordinates( obj->GridId, &pW, &Id );
        pW = pW + 16 + Img->PixShiftX[ obj->Orientation ] + obj->PosX;
        Id = Id + 8 + Img->PixShiftY[ obj->Orientation ] + obj->PosY;
        rect.lt = pW - ObjWidth / 2;
        rect.bm = Id;
        rect.tp = rect.bm - ObjHight;
        rect.rt = ObjWidth + rect.lt - 1;
        obj->Sx = rect.lt;
        obj->Sy = rect.tp;
    }
    if( RegionShrink( &rect, Area, &rect ) ){
        ArtClose( Obj );
        return;
    }
    ObjData = ArtGetObjData( Img, obj->FrameNo, obj->Orientation );
    pSrc = ObjData + rect.lt - obj->Sx + ObjWidth * (rect.tp - obj->Sy);
    Height = rect.bm - rect.tp + 1;
    Width = rect.rt - rect.lt + 1;
    tp = OBJTYPE( obj->ImgId );
    if( tp >= 2 ){
	if( tp > 3 ){
    	    if( tp == TYPE_IFCE ){ // render: iso cursor
        	ScrCopyAlpha( pSrc, Width, Height, ObjWidth, &gObjIsoSurface[ rect.tp * gObjIsoPitch + rect.lt ], gObjIsoPitch );
        	ArtClose( Obj );
        	return;
    	    }
	} else {
	    if( !(gObjDude->Flags & 0x01) && !(obj->Flags & 0xFC000) ){
		ProtoGetObj( obj->Pid, &proto );
		FlgExt = proto->FlgExt;
		if( (FlgExt & 0x8000000) || FlgExt < 0 ){
    		    v16 = TileUnk14( obj->GridId, gObjDude->GridId );
    		    if( v16 && TileUnk15( obj->GridId, gObjDude->GridId ) && (obj->Flags & 0x10000000) ) v16 = 0;
		} else if( (FlgExt & 0x10000000) ){
    		    v16 = TileUnk15( gObjDude->GridId, obj->GridId ) | TileUnk14( obj->GridId, gObjDude->GridId );
		} else if( (FlgExt & 0x20000000) ){
		    v16 = TileUnk14(obj->GridId, gObjDude->GridId) && TileUnk15(gObjDude->GridId, obj->GridId);
		} else {
		    v16 = TileUnk15( gObjDude->GridId, obj->GridId );
		    if( v16 && TileUnk14( gObjDude->GridId, obj->GridId ) && (obj->Flags & 0x10000000) ) v16 = 0;    
		}    
		if( v16 ){
		    if( (Data = ArtLoadImg( gObjRadius->ImgId, &ImgObj )) ){
			ArtGetObjSize( Data, 0, 0, &w, &h );
			TileGetScrCoordinates( gObjRadius->GridId, &Area1.lt, &Area1.tp );
			Area1.lt += 16;
			Area1.tp += 8;
			Area1.lt += Data->PixShiftX[ 0 ];
			Area1.tp += Data->PixShiftY[ 0 ];
			Area1.lt += gObjRadius->PosX;
			Area1.bm = gObjRadius->PosY + Area1.tp;
			Area1.lt -= w / 2;
			Area1.tp = Area1.bm - h + 1;
			Area1.rt = w + Area1.lt - 1;
			gObjRadius->Sx = Area1.lt;
			gObjRadius->Sy = Area1.tp;
			if( RegionShrink( &Area1, &rect, &AreaOut ) ){
			    ArtClose( ImgObj );
			} else {
			    v28[ 0 ].lt = rect.lt;
			    v28[ 0 ].tp = rect.tp;
			    v28[ 0 ].rt = rect.rt;
			    v28[ 0 ].bm = AreaOut.tp - 1;
			    
			    v28[ 1 ].lt = rect.lt;
			    v28[ 1 ].tp = AreaOut.tp;
			    v28[ 1 ].rt = AreaOut.lt - 1;
			    v28[ 1 ].bm = AreaOut.bm;
			    
			    v28[ 2 ].lt = AreaOut.rt + 1;
			    v28[ 2 ].tp = AreaOut.tp;
			    v28[ 2 ].rt = rect.rt;
			    v28[ 2 ].bm = AreaOut.bm;
			    
			    v28[ 3 ].lt = rect.lt;
			    v28[ 3 ].tp = AreaOut.bm + 1;
			    v28[ 3 ].rt = rect.rt;
			    v28[ 3 ].bm = rect.bm;
			    for( i = 0; i < 4; i++ ){
				if( ( v28[ i ].lt > v28[ i ].rt) || ( v28[ i ].tp > v28[ i ].bm ) ) continue;
  				ObjRenderNormal(
 				    pSrc + ObjWidth * (v28[ i ].tp - rect.tp) + v28[ i ].lt - rect.lt, 
  				    v28[ i ].rt - v28[ i ].lt + 1, v28[ i ].bm - v28[ i ].tp + 1, ObjWidth, gObjIsoSurface, v28[ i ].lt, v28[ i ].tp, gObjIsoPitch, Shade
  				);
			    }
			    ObjRenderMix(
				pSrc + ObjWidth * (AreaOut.tp - rect.tp) + AreaOut.lt - rect.lt, 
				AreaOut.rt - AreaOut.lt + 1, AreaOut.bm - AreaOut.tp + 1, 
				ObjWidth, 
				gObjIsoSurface + gObjIsoPitch * AreaOut.tp + AreaOut.lt, 
				gObjIsoPitch, 
				ArtGetObjData( Data, 0, 0 ) + (AreaOut.tp - Area1.tp) * w + AreaOut.lt - Area1.lt, 
				w, Shade
			    );
			    ArtClose( ImgObj );
			    ArtClose( Obj );
			    return;
			}
		    }
		}
	    }
	}
    }
    switch( obj->Flags & 0xFC000 ){
        case PRFLG_TR_RED:   ObjRenderSemiTr( pSrc, Width, Height, ObjWidth, gObjIsoSurface, rect.lt, rect.tp, gObjIsoPitch, Shade, gObjShadeRed, gObjPalBY ); break;
        case PRFLG_TR_WALL:  ObjRenderSemiTr( pSrc, Width, Height, ObjWidth, gObjIsoSurface, rect.lt, rect.tp, gObjIsoPitch, Shade, gObjShadeWall, gObjPalBY ); break;
        case PRFLG_TR_GLASS: ObjRenderSemiTr( pSrc, Width, Height, ObjWidth, gObjIsoSurface, rect.lt, rect.tp, gObjIsoPitch, Shade, gObjShadeGlass, gObjPalRY ); break;
        case PRFLG_TR_STEAM: ObjRenderSemiTr( pSrc, Width, Height, ObjWidth, gObjIsoSurface, rect.lt, rect.tp, gObjIsoPitch, Shade, gObjShadeSteam, gObjPalBY ); break;
        case PRFLG_TR_ENERGY:ObjRenderSemiTr( pSrc, Width, Height, ObjWidth, gObjIsoSurface, rect.lt, rect.tp, gObjIsoPitch, Shade, gObjShadeEnergy, gObjPalBY ); break;
        default: ObjRenderNormal( pSrc, Width, Height, ObjWidth, gObjIsoSurface, rect.lt, rect.tp, gObjIsoPitch, Shade ); break;
    }
    ArtClose( Obj );
}

void ObjGetArtFileId( int *Id )
{
    int v2, v3, v4, v5;
    short v6;

    if( (*Id >> 24) == 1 ){
        v2 = 0;
        if( gObjViolLvl == -1 ){
            if( CfgGetInteger( &gConfiguration, "preferences", "violence_level", &gObjViolLvl ) != 1 ) gObjViolLvl = 3;
            v2 = 1;
        }
        switch( gObjViolLvl ){
            case 0:  v3 = 63; v4 = 51; break;
            case 1:  v3 = 61; v4 = 51; break;
            case 2:  v3 = 56; v4 = 51; break;
            default: v3 = 66; v4 = 66; break;
        }
        v5 = (*Id & 0xFF0000) >> 16;
        if( (v5 >= v4) && (v5 <= v3) ){
            v6 = ( v5 == 62 ) ? 48 : 49;
            *Id = ArtMakeId(1, *Id & 0xFFF, v6, (*Id & 0xF000) >> 12, (*Id & 0x70000000) >> 28);
        }
        if( v2 == 1 ) gObjViolLvl = -1;
    }
}

int ObjArtSort( int *Art1, int *Art2 )
{
    int a,b,c,d;

    c = *Art1;
    d = *Art2;

    a = gObjUnk37[(c & 0xF000000) >> 24];
    b = gObjUnk37[(d & 0xF000000) >> 24];
    if( a > b ) return 1;
    if( a < b ) return -1;

    a = d & 0xFFF;
    b = c & 0xFFF;
    if( b > a ) return 1;
    if( b < a ) return -1;

    a = (c & 0xF000) >> 12;
    b = (d & 0xF000) >> 12;
    if( a > b ) return 1;
    if( a < b ) return -1;

    a = (c & 0xFF0000) >> 16;
    b = (d & 0xFF0000) >> 16;
    if( a > b ) return 1;
    if( a >= b ) return 0;
    return -1;
}

