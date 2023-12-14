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
char *gAutomapData = NULL;

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
    
    sprintf( stmp, "%s/%s/%s", pValue, "MAPS", "AUTOMAP.DB" );
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
    if( SwRes != -1 ) WinSetClickSound( SwRes, GSoundPlayTg, GSoundPlayTg );
    if( !( gAutomapScannerStatus & 0x02 ) ) WinButtonSet( SwRes, 1, 0 );
    lvl = gCurrentMapLvl;
    gAutomapScannerStatus &= 0x02; // clear all except resolution mode
    if( a1 ) gAutomapScannerStatus |= 0x01;
    if( MotionFlg ) gAutomapScannerStatus |= 0x04; // motion scanner enabled
    AutomapRender( win, gCurrentMapLvl, Imgs[ 0 ], gAutomapScannerStatus );
    flg = 0;
    k = MapUnk35();
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
    		    if( lvl != gCurrentMapLvl ){ 
    			flg = 1; 
    			lvl = gCurrentMapLvl; 
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
    if( k ) MapUnk34();
    WinClose( win );
    FontSet( FontSave );
    for( i = 0; i < 5; i++ ) ArtClose( ImgObj[ i ] );
}

void AutomapRender( int win, int MapLvl, char *a3, char Flags )
{
    char *surf;
    int color;
    Obj_t *i;
    int ObjType, Pid, Pos;

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
    char *surf, *p, c, color1, color2;
    int nible, j, e, i;

    surf = WinGetSurface( WinId ) + 640*105 + 238;
    color1 = gPalColorCubeRGB[0][31][0];
    color2 = gPalColorCubeRGB[0][15][0];
    if( !( gAutomapData = Malloc( 11024 ) ) ){ eprintf( "\nAUTOMAP: Error allocating data buffer!\n" ); return -1; }
    if( AutomapCreateMinimap( MapId, MapLvl ) == -1 ){ Free( gAutomapData ); return -1; }
    nible = 0;
    p = gAutomapData;
    for( i = 0; i < 200; i++, surf += 2*440 ){
        for( j = 0; j < 200; j++, surf += 2, c <<= 2 ){
            if( --nible <= 0 ){
                nible = 4;
                c = *p++;
            }
            e = (c >> 6 ) & 0x03;
            if( e == 0 ) continue;
            if( e == 1 ){
                surf[ 0 ] = color1;
                surf[ 1 ] = color1;
                continue;
            } 
	    if( e == 2 ){
                surf[ 0 ] = color2;
                surf[ 1 ] = color2;
            }
        }
    }
    Free( gAutomapData );
    return 0;
}

int AutomapSave()
{
/*
    int MapId; // edx MAPDST
    int v2; // esi
    int v3; // edi
    xFile_t *fh2; // eax MAPDST
    xFile_t *v6; // edx
    char *v7; // eax
    int v8; // ecx
    int v9; // ecx
    unsigned int v11; // ecx
    int v12; // ebx
    int v13; // eax
    int v14; // ecx
    int v16; // edx MAPDST
    unsigned int v17; // edi
    int v18; // edi
    char *v19; // ebx
    int i; // esi
    unsigned int v21; // eax
    char a1[8]; // [esp+0h] [ebp-528h] BYREF
    char stmp[256]; // [esp+400h] [ebp-128h] BYREF
    int pint; // [esp+504h] [ebp-24h] BYREF
    xFile_t *fh1; // [esp+508h] [ebp-20h] MAPDST

    MapId = MapGetCurrentMapId();
    v2 = gCurrentMapLvl;
    v3 = 0;
    if ( gAutomap_05.tab[3 * MapId + gCurrentMapLvl] < 0 )
        return 0;
    eprintf("\nAUTOMAP: Saving AutoMap DB index %d, level %d\n", MapId, gCurrentMapLvl);
    buff = (char *)Malloc(11024u);
    if ( buff )
    {
        gAutomapRawData = (char *)Malloc(11024u);
        if ( gAutomapRawData )
            v3 = 1;
    }
    if ( !v3 )
    {
        eprintf("\nAUTOMAP: Error allocating data buffers!\n");
        return -1;
    }
    sprintf(stmp, "%s\\%s", "MAPS", "AUTOMAP.DB");
    fh2 = dbOpen(stmp, "r+b");
    if ( !fh2 )
    {
        eprintf("\nAUTOMAP: Error opening automap database file!\n");
        eprintf("Error continued: automap_pip_save: path: %s", stmp);
        Free(buff);
        Free(gAutomapRawData);
        return -1;
    }
    if ( AutomapReadHdr(fh2) == -1 )
    {
        eprintf(aAutomapErrorRe);
        Free(buff);
        Free(gAutomapRawData);
        dbClose(v6);
        return -1;
    }
    AutomapUnk16(v2);
    v7 = (char *)GrUnk04((unsigned __int8 *)buff, gAutomapRawData, 10000);
    if ( v7 == (char *)-1 )
    {
        byte = 0;
        pdata = (char *)10000;
    }
    else
    {
        pdata = v7;
        byte = 1;
    }
    if ( v8 )
    {
        sprintf(a1, "%s\\%s", "MAPS", "AUTOMAP.TMP");
        fh1 = dbOpen(a1, "wb");
        if ( !fh1 )
        {
            eprintf("\nAUTOMAP: Error creating temp file!\n");
            Free(buff);
            Free(gAutomapRawData);
            dbClose(fh2);
            return -1;
        }
        dbrewind(fh2);
        if ( AutomapUnk18(fh2, fh1, v11) == -1 )
        {
            eprintf("\nAUTOMAP: Error copying file data!\n");
            dbClose(fh2);
            dbClose(fh1);
            Free(buff);
            Free(gAutomapRawData);
            return -1;
        }
        if ( AutomapUnk14(fh1) == -1 )
            goto LABEL_30;
        if ( dbgetBei(fh2, &pint) == -1 )
        {
            eprintf("\nAUTOMAP: Error reading database #1!\n");
            dbClose(fh2);
            dbClose(fh1);
            Free(buff);
            Free(gAutomapRawData);
            return -1;
        }
        v12 = pint;
        v13 = dbFileLength(fh2);
        v16 += v12;
        if ( v13 == -1 )
        {
            eprintf("\nAUTOMAP: Error reading database #2!\n");
            dbClose(fh2);
            dbClose(fh1);
            Free(buff);
            Free(gAutomapRawData);
            return -1;
        }
        v17 = v13 - v16;
        if ( v13 != v16 )
        {
            if ( dbseek(fh2, v16, 0) == -1 )
            {
                eprintf("\nAUTOMAP: Error writing temp data!\n");
                dbClose(fh2);
                dbClose(fh1);
                Free(buff);
                Free(gAutomapRawData);
                return -1;
            }
            if ( AutomapUnk18(fh2, fh1, v17) == -1 )
            {
                eprintf("\nAUTOMAP: Error copying file data!\n");
                dbClose(fh2);
                dbClose(fh1);
                Free(buff);
                Free(gAutomapRawData);
                return -1;
            }
        }
        v18 = 12;
        v19 = &pdata[-pint];
        for ( i = 0;
              i < 160;
              ++i )
        {
            v21 = 12 * i;
            do
            {
                if ( v14 < gAutomap_05.tab[v21 / 4] )
                    gAutomap_05.tab[v21 / 4] += (int)v19;
                v21 += 4;
            }
            while ( v21 != v18 );
            v18 += 12;
        }
        gAutomap_05.i01 += (int)v19;
        if ( AutomapWriteHdr(fh1) == -1 )
        {
LABEL_30:
            dbClose(fh2);
            Free(buff);
            Free(gAutomapRawData);
            return -1;
        }
        dbseek(fh1, 0, 2);
        dbClose(fh1);
        dbClose(fh2);
        Free(buff);
        return AutomapUnk11("master_patches");
    }
    else
    {
        if ( dbseek(fh2, 0, 2) == -1 || dbtell(fh2) != gAutomap_05.i01 )
            v9 = 0;
        if ( v9 )
        {
            if ( AutomapUnk14(fh2) == -1 )
                goto LABEL_22;
            gAutomap_05.tab[3 * MapId + v2] = gAutomap_05.i01;
            gAutomap_05.i01 += (int)(pdata + 5);
            if ( AutomapWriteHdr(fh2) == -1 )
            {
LABEL_22:
                Free(buff);
                Free(gAutomapRawData);
                return -1;
            }
            else
            {
                dbseek(fh2, 0, 2);
                dbClose(fh2);
                Free(buff);
                Free(gAutomapRawData);
                return 1;
            }
        }
        else
        {
            eprintf("\nAUTOMAP: Error reading automap database file header!\n");
            Free(buff);
            Free(gAutomapRawData);
            dbClose(fh2);
            return -1;
        }
    }
*/
}

int AutomapUnk11( char *a1 )
{
/*
    char *v1, *v2, char *v4;

    Free( gAutomapRawData );
    if( CfgGetString( &gConfiguration, v1, a1, &v2 ) == 1 ) return AutomapUnk12( v4 );
    eprintf( "\nAUTOMAP: Error reading config info!\n" );
*/
    return -1;
}

int AutomapUnk12( char *a1 )
{
/*
    char stmp[ 768 ], *v3;

    sprintf( stmp, "%s/%s/%s", v3, "MAPS", "AUTOMAP.DB" );
    if( !xFileRemove( stmp ) ) return AutomapUnk13( a1 );
    eprintf( "\nAUTOMAP: Error removing database!\n" );
*/
    return -1;
}

int AutomapUnk13( char *dir )
{
    char stmp[ 512 ], v3, *v4;

    sprintf( stmp, "%s/%s/%s", dir, "maps", "AUTOMAP.TMP" );
//    if( !FileRename( stmp, &v3 ) ) return 1;
    eprintf( "\nAUTOMAP: Error renaming database!\n" );
    return -1;
}

int AutomapUnk14( xFile_t *fh )
{
/*
    int err;
    char *p;

    err = 1;
    p = ( gAutomapUnk102 == 1 ) ? gAutomapRawData : buff;
    if( dbputLei( fh, gAutomapRawSize ) == -1 || dbputb( fh, gAutomapUnk102 ) == -1 || dbputbBlk( fh, p, gAutomapRawSize ) == -1 ) err = 0;
    if( err ) return 0;
    eprintf( "\nAUTOMAP: Error writing automap database entry data!\n" );
    dbClose( fh );
*/
    return -1;
}

int AutomapCreateMinimap( int MapId, int MapLvl )
{
    xFile_t *fh;
    char stmp[ 532 ];

    gAutomapRawData = NULL;
    sprintf( stmp, "%s/%s", "maps", "AUTOMAP.DB" );

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
	if( GrDecompress( gAutomapRawData, gAutomapData, 100*100 ) == -1 )
	    { eprintf( "\nAUTOMAP: Error decompressing DB entry!\n" ); dbClose( fh ); Free( gAutomapRawData ); return -1; }
    }
    dbClose( fh );
    if( gAutomapRawData ) Free( gAutomapRawData );
    return 0;
}

int AutomapWriteHdr( xFile_t *fh )
{
    dbrewind( fh );
    if( dbputb( fh, gAutomap_05.c01 ) == -1 || dbputLei( fh, gAutomap_05.i01 ) == -1 || dbputLeiBlk( fh, gAutomap_05.tab, 480 ) == -1 ){
	eprintf( "\nAUTOMAP: Error writing automap database header!\n" );
	dbClose( fh );
	return -1;
    }
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

void AutomapUnk16( int a1 )
{
    Obj_t *obj;
    int v3, v5, v6;
    char v4, v7, *v8, v9;

    memset( gAutomapData, 0, 10000 );
    ObjUnk79();
    for( obj = ObjGetVisibleObjectFirst( a1 ); obj; obj = ObjGetVisibleObjectNext() ){
        if( obj->GridId == -1 || (obj->Flags & 0x40000000) == 0 ) continue;
        v3 = (obj->ImgId & 0xF000000) >> 24;
        v4 = 0;
        if( v3 >= 2 ){
            if( v3 <= 2 && obj->Pid != 0x2000158 ) v4 = 2;
            if( v3 == 3 ) v4 = 1;
        }
        if( v4 ){
            v5 = 200 - obj->GridId % 200;
            v6 = v5 / 4 + 50 * ( obj->GridId / 200 );
            v7 = 2 * ( 3 - v5 % 4 );
            v9 = ~( 3 << v7 ) & gAutomapData[ v6 ];
            gAutomapData[ v6 ] = v9;
            gAutomapData[ v6 ] = (v4 << v7) | v9;
        }
    }
}

int AutomapCreateDatabase()
{
    xFile_t *fh;
    int r;
    char fpath[ 256 ];

    gAutomap_05.c01 = 1;
    gAutomap_05.i01 = 1925;
    memcpy( gAutomap_05.tab, gAutomap_03, sizeof( gAutomap_05.tab ) );
    sprintf( fpath, "%s/%s", "MAPS", "AUTOMAP.DB" );
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
    if( a2 >= 3 || a2 < 0 ) return 0;
    if( a1 >= 160 || a1 < 0 ) return 0;
    return gAutomap_03[ 3 * a1 + a2 ] >= 0;
}

int AutomapUnk18( xFile_t *fh1, xFile_t *fh2 , unsigned int a3 )
{
    void *p;
    unsigned int v6, v8;

    if( !(p = Malloc( 65535 )) ) return -1;
    v8 = a3 / 0xFFFF;
    if( a3 / 0xFFFF ){        
        if( a3 / 0xFFFF ){
            for( v6 = 0; v6 < v8; v6++ ){
                if( dbread( p, 0xFFFF, 1, fh1 ) != 1 ) break;
                if( dbwrite( p, 0xFFFF, 1, fh2 ) != 1 ) break;                
            }
        }        
        if( v6 < v8 ){ Free( p ); return -1; }
    }
    if( !(a3 % 0xFFFF) || dbread( p, a3 % 0xFFFF, 1, fh1 ) == 1 && dbwrite( p, a3 % 0xFFFF, 1, fh2 ) == 1 ){
        Free( p );
        return 0;
    } else {
        Free( p ); return -1;
    }
}

int AutomapLoadDB( Automap_t **pAutomap )
{
    xFile_t *fh;
    char stmp[ 520 ];

    sprintf( stmp, "%s/%s", "maps", "AUTOMAP.DB" );
    if( !(fh = dbOpen( stmp, "rb" ) ) ){
	eprintf( "\nAUTOMAP: Error opening database file for reading!\n" );
	eprintf( "Error continued: ReadAMList: path: %s\n", stmp );
	return -1;
    }
    if( AutomapReadHdr( fh ) == -1 ){ eprintf( "\nAUTOMAP: Error reading automap database header pt2!\n" ); dbClose( fh ); return -1; }
    dbClose( fh );
    *pAutomap = &gAutomap_05;
    return 0;
}

