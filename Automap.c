#include "FrameWork.h"

int gAutomap_03[ 480 ] = { 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int gAutomapUnk10[ 160 ] = {
    -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0, -1, -1,  0,  0,  0,  0,  0, -1, -1,  0,  0,  0,
     0,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0, -1,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1, -1,
    -1, -1, -1,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 
};

int gAutomapImgIds[ 5 ] = { 171, 8, 9, 172, 173 };

int gAutomapScannerStatus = 0;

Automap_t gAutomap_05;
int gAutomapRawSize;
char gAutomapUnk102;
char *gAutomapRawData = NULL;
char *gAutomapData = NULL; // 200x200 map compressed to 50x200 2-bit map + 1kB

static int AutomapRename( char *dir, char *NewFileName );
static int AutomapTmp2Db( char *subdirname );
static int AutomapUnk11( char *sub );

int AutomapInit()
{
    gAutomapScannerStatus = 0;
    AutomapCreateDatabase( );
    return 0;
}

void AutomapReset()
{
    gAutomapScannerStatus = 0;
    AutomapCreateDatabase();
}

void AutomapClose()
{
    char stmp[ 512 ];
    char *pValue;

    if( CfgGetString( &gConfiguration, "system", "master_patches", &pValue ) != 1 ) return;
    
    sprintf( stmp, "%s/%s/%s", pValue, "maps", "automap.db" );
    xFileRemove( stmp );    
}

int AutomapFLoad( xFile_t *fh )
{

    return (dbgetBei( fh, &gAutomapScannerStatus ) != -1) - 1;
}

int AutomapFSave( xFile_t *fh )
{

    return (dbputBei( fh, gAutomapScannerStatus ) != -1) - 1;
}

int AutomapUnk04( int a1 )
{

    return gAutomapUnk10[ a1 ];
}

void AutomapScanner( int a1, int MotionFlg )
{
    MsgLine_t msg;
    CachePool_t *ImgObj[ 5 ];
    Obj_t *HandObj;
    unsigned int sel;
    char *Imgs[ 5 ];
    int i, win, bt, lvl, flg, ImgIds[ 5 ], k, FontSave, ExitFlg, SwRes;

    ImgIds[ 0 ] = gAutomapImgIds[ 0 ];
    ImgIds[ 1 ] = gAutomapImgIds[ 1 ];
    ImgIds[ 2 ] = gAutomapImgIds[ 2 ];
    ImgIds[ 3 ] = gAutomapImgIds[ 3 ];
    ImgIds[ 4 ] = gAutomapImgIds[ 4 ];
    for( i = 0; i < 5; i++ ){
        if( !(Imgs[ i ] = ArtGetBitmap( ArtMakeId( 6, ImgIds[ i ], 0, 0, 0 ), 0, 0, &ImgObj[ i ] )) ){
            for( ; --i != -1; ArtClose( ImgObj[ i ] ) );
            return;
        }
    }
    if( a1 ) ObjUnk79();
    FontSave = FontGetCurrent();
    FontSet( 101 );
    win = WinCreateWindow( 75, 0, 519, 480, gPalColorCubeRGB[21][16][9], 20 );
    bt = WinCreateButton( win, 111, 454, 15, 16, -1, -1, -1, 's', Imgs[ 1 ], Imgs[ 2 ], 0, 0x20 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    bt = WinCreateButton( win, 277, 454, 15, 16, -1, -1, -1, KEY_ESC, Imgs[ 1 ], Imgs[ 2 ], 0, 0x20 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    SwRes = WinCreateButton( win, 457, 340, 42, 74, -1, -1, 'l', 'h', Imgs[ 3 ], Imgs[ 4 ], 0, 0x21 );
    if( SwRes != -1 ) WinSetClickSound( SwRes, (void *)GSoundPlayTg, (void *)GSoundPlayTg );
    if( !( gAutomapScannerStatus & 0x02 ) ) WinButtonSet( SwRes, 1, 0 );
    lvl = gMapCurrentLvl;
    gAutomapScannerStatus &= 0x02; // clear all except resolution mode
    if( a1 ) gAutomapScannerStatus |= 0x01;
    if( MotionFlg ) gAutomapScannerStatus |= 0x04; // motion scanner enabled
    AutomapRender( win, gMapCurrentLvl, Imgs[ 0 ], gAutomapScannerStatus );
    flg = 0;
    k = MapAmbientDisable();
    GmouseLoadCursor( 1 );    
    for( ExitFlg = 0; !ExitFlg; ){
        sel = InpUpdate();
	switch( sel ){
    	    case 9:  case KEY_ESC: ExitFlg = 1; break;
    	    case 17: case 324: case 301: SysQuitDlg(); break;
    	    case 'A': case 'a': ExitFlg = 1; break;
    	    case 'H': case 'h': // Hi res
    		    if( gAutomapScannerStatus & 0x02 ) break;
    		    flg = 1; 
    		    gAutomapScannerStatus |= 0x02;
    		    WinButtonSet( SwRes, 0, 0 );
    		    break;
    	    case 'L': case 'l': // Lo res
    		    if( !(gAutomapScannerStatus & 0x02) ) break;    		     
    		    flg = 1; 
    		    gAutomapScannerStatus &= ~0x02;
    		    WinButtonSet( SwRes, 1, 0 );
    		    break;
    	    case 'S': case 's':{
    		    if( lvl != gMapCurrentLvl ){ 
    			flg = 1; 
    			lvl = gMapCurrentLvl; 
    		    }
    		    if( gAutomapScannerStatus & 0x04 ) break;
        	    HandObj = InvGetLHandObj( gObjDude );
        	    if( !HandObj || HandObj->Pid != PID_MOTIONSENSOR1 ){
            		HandObj = InvGetRHandObj( gObjDude );
            		if( !HandObj || HandObj->Pid != PID_MOTIONSENSOR1 ) HandObj = NULL;
        	    }
        	    if( HandObj ){
            		if( ItemGetCharges( HandObj ) > 0 ){
            		    flg = 1; 
            		    gAutomapScannerStatus |= 0x04; 
            		    ItemMotSensCharge( HandObj );
            		} else {
            		    GSoundPlay( "iisxxxx1" ); 
            		    // 'the motion sensor has no charges remaining'
            		    DlgBox( MessageGetMessage( &gMessage, &msg, 18 ), 0, 0, 165, 140, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 0 );
            		}
        	    } else {
                	GSoundPlay( "iisxxxx1" ); 
                	// 'the motion sensor option is not installed'
                	DlgBox( MessageGetMessage( &gMessage, &msg, 17 ), 0, 0, 165, 140, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 0 );
        	    }                        		
        	    break;
    	    }
    	    case 390: InpScrShotTake(); break;

        }
        if( gMenuEscape ) break;
        if( flg ){
            AutomapRender( win, lvl, Imgs[ 0 ], gAutomapScannerStatus );
            flg = 0;
        }
    }        
    if( k ) MapAmbientEnable();
    WinClose( win );
    FontSet( FontSave );
    for( i = 0; i < 5; i++ ) ArtClose( ImgObj[ i ] );
}

void AutomapRender( int win, int MapLvl, char *a3, char Flags )
{
    Obj_t *i;
    char *surf;
    int color, ObjType, Pos;

    WinDrawFilledRect( win, 0, 0, 519, 480, ( (Flags & 1) != 0 ) ? gPalColorCubeRGB[8][8][8] : gPalColorCubeRGB[21][16][9] );
    WinDrawFrame( win );
    surf = WinGetSurface( win );
    ScrCopy( a3, 519, 480, 519, surf, 519 );

    for( i = ObjGetVisibleObjectFirst( MapLvl ); i; i = ObjGetVisibleObjectNext() ){
        if( i->GridId == -1 ) continue;
        ObjType = OBJTYPE( i->ImgId );
        if( Flags & 0x01 ){
    	    if( ObjType == 1 && (i->Flags & 1) == 0 && (Flags & 4) != 0 && i->Grid.DestMapElev >= 0 ){
        	color = gPalColorCubeRGB[31][0][0];
    	    } else {
    		if( !( i->Flags & 0x40000000 ) ) continue;
    		if( i->Pid == 0x2000031 ){
            	    color = gPalColorCubeRGB[31][18][8];
    		} else if( ObjType == 3 ){
            	    color = gPalColorCubeRGB[0][31][0];
    		} else if( ObjType == 2 && (Flags & 0x02) && i->Pid != 0x2000158 ){
            	    color = gPalColorCubeRGB[0][15][0];
    	        } else if( i == gObjDude ){
            	    color = gPalColorCubeRGB[31][0][0];
    		} else {
            	    color = gPalColorCubeRGB[0][0][0];
    		}
    	    }
        }
        Pos = -2 * (i->GridId % 200) - 10 + 519 * (2 * (i->GridId / 200) + 9) - 60;
        if( !( Flags & 0x01 ) ){
            switch( ObjType ){
                case 0: color = gPalColorCubeRGB[6][11][17]; break;
                case 1: color = gPalColorCubeRGB[28][0][0]; break;
                case 2: color = gPalColorCubeRGB[0][14][0]; break;
                case 3: color = gPalColorCubeRGB[12][8][2]; break;
                case 5: color = gPalColorCubeRGB[30][29][2]; break;
                default: color = gPalColorCubeRGB[0][0][0]; break;
            }
        }
        if( color == gPalColorCubeRGB[0][0][0] ) continue;
        if( (Flags & 1) != 0 ){
            if( surf[ Pos ] != gPalColorCubeRGB[0][31][0] || color != gPalColorCubeRGB[0][15][0] ) surf[ Pos ] = color;
            if( gPalColorCubeRGB[0][31][0] != surf[ Pos ] || color != gPalColorCubeRGB[0][15][0] ) surf[ Pos + 1 ] = color;
            if( i == gObjDude ){
                surf[ Pos - 1 ] = color; surf[ Pos - 519 ] = color; surf[ Pos + 519 ] = color;
            }
        } else {
            surf[ Pos + 1 ] = color; surf[ Pos + 519 ] = color; surf[ Pos + 520 ] = color; surf[ Pos + 518 ] = color; 
            surf[ Pos + 521 ] = color; surf[ Pos + 1038 ] = color; surf[ Pos + 1039 ] = color; surf[ Pos + 0 ] = color;
        }
    }
    color = ( Flags & 0x01 ) ? gPalColorCubeRGB[0][31][0] : gPalColorCubeRGB[12][8][2];
    if( MapGetCurrentMapId() != -1 ){
        color |= 0x2000000;
        WinDrawText( win, MapGetCityName( MapGetCurrentMapId() ), 240, 150, 380, color );
        WinDrawText( win, MapCityLvlName( MapGetCurrentMapId(), MapLvl ), 240, 150, 396, color );
    }
    WinUpdate( win );
}

int AutomapRenderMinimap( int WinId, int MapId, int MapLvl )
{
    char *surf, *p, c, color1, color2,*n;
    int nible, j, e, i;

    surf = WIN_XY( 238, 105 - 25 , 640, WinGetSurface( WinId ) );
    color1 = gPalColorCubeRGB[0][31][0];
    color2 = gPalColorCubeRGB[0][15][0];
    if( !( gAutomapData = Malloc( 11024 ) ) ){ eprintf( "\nAUTOMAP: Error allocating data buffer!\n" ); return -1; }
    memset( gAutomapData, 0, 11024 );
    if( AutomapCreateMinimap( MapId, MapLvl ) == -1 ){ Free( gAutomapData ); return -1; }
    nible = 0;
    p = gAutomapData;
    for( i = 0; i < 200; i++, surf += 640*2 ){
	n = surf;
        for( j = 0; j < 200; j++, n += 2, c <<= 2 ){
            if( --nible <= 0 ){
                nible = 4;
                c = *p++;
            }
            e = (c >> 6 ) & 0x03;
            if( e == 0 ) continue;
            if( e == 1 ){
                n[ 0 ] = color1;
                n[ 1 ] = color1;
                continue;
            } 
	    if( e == 2 ){
                n[ 0 ] = color2;
                n[ 1 ] = color2;
            }
        }
    }
    Free( gAutomapData );
    return 0;
}

int AutomapSave()
{
    xFile_t *fh2, *fh1;
    char a1[1024], stmp[256];
    int j, MapId, v7, v8, v9, v13, i, pint;

    MapId = MapGetCurrentMapId();
    v7 = 0;
    v8 = gAutomap_05.tab[ 3 * MapId + gMapCurrentLvl ];
    if( v8 < 0 ) return 0;
    eprintf( "\nAUTOMAP: Saving AutoMap DB index %d, level %d\n", MapId, gMapCurrentLvl );
    gAutomapData = (char *)Malloc( 11024 );
    if( gAutomapData ){
        gAutomapRawData = (char *)Malloc( 11024 );
        if( gAutomapRawData ) v7 = 1;
    }
    if( !v7 ){ eprintf( "\nAUTOMAP: Error allocating data buffers!\n" ); return -1; }
    sprintf( stmp, "%s/%s", "maps", "automap.db" );    
    if( !( fh2 = dbOpen( stmp, "r+b" ) ) )
	{ eprintf( "\nAUTOMAP: Error opening automap database file!\n" ); eprintf( "Error continued: automap_pip_save: path: %s", stmp ); Free( gAutomapData ); Free( gAutomapRawData ); return -1; }
    if( AutomapReadHdr( fh2 ) == -1 )
	{ eprintf( "\nAUTOMAP: Error reading automap database file header!\n" ); Free( gAutomapData ); Free( gAutomapRawData ); dbClose( fh2 ); return -1; }
    AutomapMakeMinimap( gMapCurrentLvl );    
    if( ( v7 = GrCompress( (unsigned char *)gAutomapData, gAutomapRawData, 10000 ) ) == -1 ) {
        gAutomapUnk102 = 0;
        gAutomapRawSize = 10000;
    } else {
        gAutomapUnk102 = 1;
        gAutomapRawSize = v7;
    }
    if( !v8 ){
        if( dbseek( fh2, 0, 2 ) == -1 || dbtell( fh2 ) != gAutomap_05.i01 ) v9 = 0;
        if( !v9 ){
            eprintf( "\nAUTOMAP: Error reading automap database file header!\n" );
            Free( gAutomapData );
            Free( gAutomapRawData );
            dbClose( fh2 );
            return -1;
        }
        if( AutomapSaveEntryData( fh2 ) == -1 ){ Free( gAutomapData ); Free( gAutomapRawData ); return -1; }
        gAutomap_05.tab[3 * MapId + gMapCurrentLvl] = gAutomap_05.i01;
        gAutomap_05.i01 += gAutomapRawSize + 5;
        if( AutomapWriteHdr( fh2 ) == -1 ){ Free( gAutomapData ); Free( gAutomapRawData ); return -1; }
        dbseek( fh2, 0, 2 );
        dbClose( fh2 );
        Free( gAutomapData );
        Free( gAutomapRawData );
        return 1;
    }
    sprintf( a1, "%s/%s", "maps", "automap.tmp" );        
    if( !( fh1 = dbOpen( a1, "wb" ) ) )
        { eprintf( "\nAUTOMAP: Error creating temp file!\n" ); Free( gAutomapData ); Free( gAutomapRawData ); dbClose( fh2 ); return -1; }
    dbrewind( fh2 );
    if( AutomapFileCopy( fh2, fh1, v8 ) == -1 )
        { eprintf( "\nAUTOMAP: Error copying file data!\n" ); dbClose( fh2 ); dbClose( fh1 ); Free( gAutomapData ); Free( gAutomapRawData ); return -1; }
    if( AutomapSaveEntryData( fh1 ) == -1 ){ dbClose( fh2 ); Free( gAutomapData ); Free( gAutomapRawData ); return -1; }
    if( dbgetBei( fh2, &pint ) == -1 )
        { eprintf( "\nAUTOMAP: Error reading database #1!\n" ); dbClose( fh2 ); dbClose( fh1 ); Free( gAutomapData ); Free( gAutomapRawData ); return -1; }
    v7 = pint + 5;
    if( ( v13 = dbFileLength( fh2 ) ) == -1 )
        { eprintf( "\nAUTOMAP: Error reading database #2!\n" ); dbClose( fh2 ); dbClose( fh1 ); Free( gAutomapData ); Free( gAutomapRawData ); return -1; }
    if( v13 != v7 ){
        if( dbseek( fh2, v7, 0 ) == -1 )
    	{ eprintf( "\nAUTOMAP: Error writing temp data!\n" ); dbClose( fh2 ); dbClose( fh1 ); Free( gAutomapData ); Free( gAutomapRawData ); return -1; }
        if( AutomapFileCopy( fh2, fh1, v13 - v7 ) == -1 )
    	{ eprintf( "\nAUTOMAP: Error copying file data!\n" ); dbClose( fh2 ); dbClose( fh1 ); Free( gAutomapData ); Free( gAutomapRawData ); return -1; }
    }
    v7 = gAutomapRawSize - pint;
    for( i = 0; i < 160; i++ ){
        for( j = i; j != i + 3; j++ ){
            if( v8 < gAutomap_05.tab[ j ] ) gAutomap_05.tab[ j ] += v7;
        }                
    }
    gAutomap_05.i01 += v7;
    if( AutomapWriteHdr( fh1 ) == -1 ){ dbClose( fh2 ); Free( gAutomapData ); Free( gAutomapRawData ); return -1; }
    dbseek( fh1, 0, 2 );
    dbClose( fh1 );
    dbClose( fh2 );
    Free( gAutomapData );
    return AutomapUnk11( "master_patches" );
}

static int AutomapUnk11( char *sub )
{
    char *p;

    Free( gAutomapRawData );
    if( CfgGetString( &gConfiguration, "system", sub, &p ) != 1 ){
	eprintf( "\nAUTOMAP: Error reading config info!\n" );
	return -1;
    }
    return AutomapTmp2Db( p );
}

static int AutomapTmp2Db( char *subdirname )
{
    char stmp[ 768 ];

    sprintf( stmp, "%s/%s/%s", subdirname, "maps", "automap.db" );
    if( xFileRemove( stmp ) ){
	eprintf( "\nAUTOMAP: Error removing database!\n" );
	return -1;
    }    
    return AutomapRename( subdirname, stmp );
}

static int AutomapRename( char *dir, char *NewFileName )
{
    char stmp[ 512 ];

    sprintf( stmp, "%s/%s/%s", dir, "maps", "automap.tmp" );
    if( rename( stmp, NewFileName ) ){
	eprintf( "\nAUTOMAP: Error renaming database!\n" );
	return -1;
    }
    return 1;
}

int AutomapSaveEntryData( xFile_t *fh )
{
    int err;
    char *p;

    err = 1;
    p = ( gAutomapUnk102 == 1 ) ? gAutomapRawData : gAutomapData;
    if( dbputLei( fh, gAutomapRawSize ) == -1 || dbputb( fh, gAutomapUnk102 ) == -1 || dbputbBlk( fh, p, gAutomapRawSize ) == -1 ) err = 0;
    if( !err ){
	eprintf( "\nAUTOMAP: Error writing automap database entry data!\n" );
	dbClose( fh );
	return -1;
    }
    return 0;
}

int AutomapCreateMinimap( int MapId, int MapLvl )
{
    xFile_t *fh;
    char stmp[ 532 ];

    gAutomapRawData = NULL;
    sprintf( stmp, "%s/%s", "maps", "automap.db" );

    if( !( fh = dbOpen( stmp, "r+b" ) ) )
	{ eprintf( "\nAUTOMAP: Error opening automap database file!\n" ); eprintf( "Error continued: AM_ReadEntry: path: %s", stmp ); return -1; }
    if( AutomapReadHdr( fh ) == -1 )
	{ eprintf( "\nAUTOMAP: Error reading automap database header!\n" ); dbClose( fh ); return -1; }
    if( gAutomap_05.tab[ 3 * MapId + MapLvl ] <= 0 )
	{ eprintf( "\nAUTOMAP: Error reading automap database entry data!\n" ); dbClose( fh ); return -1; }
    if( dbseek( fh, gAutomap_05.tab[ 3 * MapId + MapLvl ], SEEK_SET ) == -1  )
	{ eprintf( "\nAUTOMAP: Error reading automap database entry data!\n" ); dbClose( fh ); return -1; }
    if( dbgetBei( fh, &gAutomapRawSize ) == -1  )
	{ eprintf( "\nAUTOMAP: Error reading automap database entry data!\n" ); dbClose( fh ); return -1; }
    if( dbgetb( fh, &gAutomapUnk102 ) == -1 )
	{ eprintf( "\nAUTOMAP: Error reading automap database entry data!\n" ); dbClose( fh ); return -1; }
    if( gAutomapUnk102 != 1 ){
        if( dbreadByteBlk( fh, gAutomapData, gAutomapRawSize ) == -1 )
    	    { eprintf( "\nAUTOMAP: Error reading automap database entry data!\n" ); dbClose( fh ); return -1; }
    } else {
	if( !( gAutomapRawData = Malloc( 11024 ) ) )
	    { eprintf( "\nAUTOMAP: Error allocating decompression buffer!\n" ); dbClose( fh ); return -1; }
	if( dbreadByteBlk( fh, gAutomapRawData, gAutomapRawSize ) == -1 )
	    { eprintf( "\nAUTOMAP: Error reading automap database entry data!\n" ); dbClose( fh ); return -1; }
	if( GrDecompress( (unsigned char *)gAutomapRawData, gAutomapData, 100 * 100 ) == -1 )
	    { eprintf( "\nAUTOMAP: Error decompressing DB entry!\n" ); dbClose( fh ); Free( gAutomapRawData ); return -1; }
    }
    dbClose( fh );
    if( gAutomapRawData ) Free( gAutomapRawData );
    return 0;
}

int AutomapWriteHdr( xFile_t *fh )
{
    dbrewind( fh );
    if( dbputb( fh, gAutomap_05.c01 ) == -1 ){ eprintf( "AUTOMAP: Error writing automap database header! 1" ); dbClose( fh ); return -1; }
    if( dbputLei( fh, gAutomap_05.i01 ) == -1 ){ eprintf( "AUTOMAP: Error writing automap database header! 2" ); dbClose( fh ); return -1; }
    if( dbputLeiBlk( fh, gAutomap_05.tab, 480 ) == -1 ){ eprintf( "AUTOMAP: Error writing automap database header! 3" ); dbClose( fh ); return -1; }
    return 0;
}

int AutomapReadHdr( xFile_t *fh )
{
    if( dbgetb( fh, &gAutomap_05.c01 ) == -1 ) return -1; 
    if( dbgetBei( fh, &gAutomap_05.i01 ) == -1 ) return -1;
    if( dbreadBeiBlk( fh, gAutomap_05.tab, 480 ) == -1 ) return -1;
    if( gAutomap_05.c01 != 1 ) return -1;
    return 0;
}

void AutomapMakeMinimap( int MapLvl )
{
    Obj_t *obj;
    int type, xg, bytepos;
    char item_type, bitpos;

    memset( gAutomapData, 0, 10000 );
    ObjUnk79();
    for( obj = ObjGetVisibleObjectFirst( MapLvl ); obj; obj = ObjGetVisibleObjectNext() ){
        if( obj->GridId == -1 || (obj->Flags & 0x40000000) == 0 ) continue;
        type = OBJTYPE( obj->ImgId );
        item_type = 0;
        if( type == 2 && obj->Pid != 0x2000158 ) item_type = 2; // PID_GRAVESITE1?
        if( type == 3 ) item_type = 1;
        if( item_type ){
            xg = 200 - ( obj->GridId % 200 );
            bytepos = xg / 4 + 50 * ( obj->GridId / 200 );
            bitpos = 2 * ( 3 - xg % 4 );
            gAutomapData[ bytepos ] &= ~( 3 << bitpos );
            gAutomapData[ bytepos ] |= item_type << bitpos;
        }
    }
}

int AutomapCreateDatabase()
{
    xFile_t *fh;
    char fpath[ 256 ];

    gAutomap_05.c01 = 1;
    gAutomap_05.i01 = 1925;
    memcpy( gAutomap_05.tab, gAutomap_03, sizeof( gAutomap_05.tab ) );
    sprintf( fpath, "%s/%s", "maps", "automap.db" );
    if( !(fh = dbOpen( fpath, "wb" )) ){
        eprintf( "\nAUTOMAP: Error creating automap database file!\n" );
        return -1;
    }
    if( AutomapWriteHdr( fh ) == -1 ) return -1;    
    dbClose( fh );
    return 0;    
}

int AutomapUnk17( int a1, int a2 )
{

    if( a2 < 0 || a2 > 2 ) return 0;
    if( a1 < 0 || a1 > 159 ) return 0;
    return gAutomap_03[ 3 * a1 + a2 ] >= 0;
}

int AutomapFileCopy( xFile_t *fsrc, xFile_t *fdst , unsigned int Size )
{
    void *buffer;
    unsigned int i, blks;

    if( !(buffer = Malloc( 65535 )) ) return -1;
    blks = Size / 0xFFFF;
    if( blks ){        
        for( i = 0; i < blks; i++ ){
            if( dbread( buffer, 0xFFFF, 1, fsrc ) != 1 ) break;
            if( dbwrite( buffer, 0xFFFF, 1, fdst ) != 1 ) break;                
        }
        if( i < blks ){ Free( buffer ); return -1; }
    }
    if( !(Size % 0xFFFF) || (dbread( buffer, Size % 0xFFFF, 1, fsrc ) == 1 && dbwrite( buffer, Size % 0xFFFF, 1, fdst ) == 1 ) ){
        Free( buffer );
        return 0;
    } else {
        Free( buffer ); return -1;
    }
}

int AutomapLoadDB( Automap_t **pAutomap )
{
    xFile_t *fh;
    char stmp[ 520 ];

    sprintf( stmp, "%s/%s", "maps", "automap.db" );
    if( !(fh = dbOpen( stmp, "rb" ) ) ){
	eprintf( "\nAUTOMAP: Error opening database file for reading!\n" );
	eprintf( "Error continued: ReadAMList: path: %s\n", stmp );
	return -1;
    }
    if( AutomapReadHdr( fh ) == -1 ){ eprintf( "\nAUTOMAP: Error reading automap '%s' database header pt2!\n", stmp ); dbClose( fh ); return -1; }
    dbClose( fh );
    *pAutomap = &gAutomap_05;
    return 0;
}

