#include "FrameWork.h"

#define ENDING_SEQ7C	327

int EndGameUnk19 = 0;
int EndGameUnk20 = 0;
int EndGameUnk21 = 0;
int EndGameUnk22 = 0;
EndGame_t *gEndGameInfo = NULL;
int gEndGameInfoCnt = 0;

char gEndGameNarratorFile[ 16 ];
char gEndGameUnk02[ 31 ];
int gEndGameUnk03;
char gEndGameCutsPath[ 260 ];
int gEndGameUnk05;
EndGame02_t *gEndGameEndings;
char **gEndGameUnk07;
int gEndGameUnk08;
int gEndGameUnk09;
int gEndGameUnk10;
int gEndGameUnk11;
int gEndGameCount;
int gEndGameUnk13;
int gEndGameUnk14;
char *gEndGameUnk15;
int gEndGameUnk16;
char *gEndGameSurface;
int gEndGameWindow;

/***********************************************/
void EndGameRun()
{
    int i;

    if( EndGameUnk03() == -1 ) return;    
    for( i = 0; i < gEndGameCount; i++ ){
        if( GlobVarGet( gEndGameEndings[ i ].GVarId ) != gEndGameEndings[ i ].GVarVal ) continue;
        if( gEndGameEndings[ i ].ArtNum == ENDING_SEQ7C ){
            EndGameUnk05( gEndGameEndings[ i ].PanDir, gEndGameEndings[ i ].Narrator );
        } else {
            EndGameUnk06( ArtMakeId( 6, gEndGameEndings[ i ].ArtNum, 0, 0, 0 ), gEndGameEndings[ i ].Narrator );
        }
    }        
    EndGameUnk04();
}

void EndGameMoviePlay()
{
    GSoundBgClose();
    MapAmbientDisable();
    FadeStep( gFadePaletteC );
    EndGameUnk22 = 0;
    InpTaskStart( (void *)EndGameUnk16 );
    GSoundBgSetCb( (void *)EndGameUnk15 );
    GSoundLoadBg( "akiss", 12, 14, 15 );
    TimerWaitProc( 3000 );
    FeatGetVal( gObjDude, 34 );
    Credits( "credits.txt", -1, 0 );
    GSoundBgClose();
    GSoundBgSetCb( NULL );
    InpTaskStop(EndGameUnk16);
    GSoundBgClose();
    PalLoadFromFile( "color.pal" );
    FadeStep( gPalBase );
    MapAmbientEnable();
    EndGameUnk02();
}

void EndGameUnk02()
{
    int flg1, flg2, CursorId, IsCursorClear;
    MsgLine_t msg;

    flg1 = 0;
    if( (flg2 = MapAmbientDisable()) ) flg1 = GmouseUnk58();
    if( flg1 ) GmouseUnk03();
    IsCursorClear = MseCursorHidden();
    if( IsCursorClear ) MseCursorShow();
    CursorId = GmouseGetCursorId();
    GmouseLoadCursor(1);
    msg.Id = 30; // 'Do you want to continue playing ?'
    if( MessageGetMsg( &gMessage, &msg ) == 1 && !DlgBox(msg.Text, 0, 0, 169, 117, gPalColorCubeRGB[31][18][8], 0, gPalColorCubeRGB[31][18][8], 16) ) gMenuEscape = 2;
    GmouseLoadCursor( CursorId );
    if( IsCursorClear ) MseCursorHide();
    if( flg1 ) GmouseIsoEnter();
    if( flg2 ) MapAmbientEnable();
}

int EndGameUnk03()
{
    int IsCursorClear, i;
    char *s;

    if( EndGameUnk17() ) return -1;
    GSoundBgClose();
    gEndGameUnk10 = MapAmbientDisable();
    CycleColorStop();
    GmouseLoadCursor( 0 );
    IsCursorClear = MseCursorHidden();
    gEndGameUnk11 = (IsCursorClear == 0);
    if( IsCursorClear ) MseCursorShow();
    gEndGameUnk16 = FontGetCurrent();
    FontSet( 101 );
    FadeStep( gFadePaletteC );
    if( (gEndGameWindow = WinCreateWindow( 0, 0, 640, 480, gPalColorCubeRGB[0][0][0], 4 ) ) == -1 ) return -1;
    if( !(gEndGameSurface = WinGetSurface( gEndGameWindow )) ) return -1;
    CycleColorStop();
    GSoundUnk33( (void *)EndGameUnk11 );
    gEndGameUnk08 = 0;
    CfgGetInteger( &gConfiguration, "preferences", "subtitles", &gEndGameUnk08 );
    if( !gEndGameUnk08 ) return 0;
    if( CfgGetString( &gConfiguration, "system", "language", &s ) != 1 ){ gEndGameUnk08 = 0; return 0; }
    sprintf( gEndGameCutsPath, "text/%s/cuts/", s );    
    if( !(gEndGameUnk07 = Malloc( 200 ) ) ){ gEndGameUnk08 = 0; return 0; }
    for( i = 0; i != 50; i++ ) gEndGameUnk07[ i ] = NULL;    
    if( !(gEndGameUnk15 = Malloc( 200 ) ) ){ Free( gEndGameUnk07 ); gEndGameUnk08 = 0; }
    return 0;
}

void EndGameUnk04()
{
    if( gEndGameUnk08 ){
        EndGameUnk14();
        Free( gEndGameUnk15 );
        Free( gEndGameUnk07 );
        gEndGameUnk07 = NULL;
        gEndGameUnk08 = 0;
    }
    if( gEndGameEndings ){
        Free( gEndGameEndings );
        gEndGameEndings = NULL;
    }
    gEndGameCount = 0;
    FontSet( gEndGameUnk16 );
    GSoundUnk33( 0 );
    WinClose( gEndGameWindow );
    if( !gEndGameUnk11 ) MseCursorHide();
    GmouseLoadCursor( 1 );
    PalLoadFromFile( "color.pal" );
    FadeStep( gPalBase );
    CycleColorStart();
    if( gEndGameUnk10 ) MapAmbientEnable();
}

void EndGameUnk05( int a1, char *a2 )
{
    ArtFrmHdr_t *v3;
    Pal8_t v20[256];
    CachePool_t *Obj;
    double v31, v16;
    int v8,v9,v10,v11,v12,v15,time_ms,SrcPitch,v26,ObjWidth,v30,v32,v33;
    char v14,v19[768],*ObjData;

    if( (v3 = ArtLoadImg( ArtMakeId( 6, 327, 0, 0, 0 ), &Obj )) ){
        time_ms = 0;
        ObjWidth = ArtGetObjWidth(v3, 0, 0);
        SrcPitch = ObjWidth;
        ArtGetObjHeight( v3, 0, 0 );
        ObjData = ArtGetObjData( v3, 0, 0 );
        ScrFillSolid( gEndGameSurface, 640, 480, 640, gPalColorCubeRGB[0][0][0] );
        EndGameUnk10( 6, 327 );
        memcpy( v19, &gPalBase, 768 );
        FadeSetPalette( gFadePaletteC );
        v8 = ObjWidth - 640;
        EndGameUnk07( a2 );
        v32 = v8 * 4;
        v9 = 16 * v8 / v8;
        if( gEndGameUnk03 ){
            v10 = 1000 * GSoundSpkGetSamples();
            if( v10 > (16 * v8) >> 1 ) v9 = (v10 + v9 * (v8 / 2)) / v8;
        }
        if( a1 == -1 ){
            v26 = 0;
            v33 = SrcPitch - 640;
        } else{
            v26 = SrcPitch - 640;
            v33 = 0;
        }
        InpTaskUnHalt();
        v11 = 0;
        v12 = 640 - v32;
        v30 = (a1 == 1);
        while( v33 != v26 ){
            if( TimerCurrDiff( time_ms ) >= v9 ){
            	ScrCopy( &ObjData[ v33 ], 640, 480, SrcPitch, gEndGameSurface, 640 );
            	if( v11 ) EndGameUnk13();
            	WinUpdate( gEndGameWindow );
            	time_ms = TimerGetSysTime();
            	if( v33 > v32 ){
            	    if( v12 > v33 ){
                	v14 = 0;
            	    } else {
                    	ObjWidth = v32 - (v33 - v12);
                    	v14 = 1;
                    	v31 = ObjWidth / v32;
            	    }
            	} else {
            	    v14 = 1;
            	    v31 = v33 / v32;
            	}
            	if( v14 ){            	    
            	    v16 = v31;
            	    for( v15 = 0; v15 < 768; v15++ ){
DD
//                v29 = v19[v15];
                        ObjWidth = lround(v16);
                        v19[ v15 + 767 ] = ObjWidth;
            	    }
            	    FadeSetPalette( v20 );
            	}
            	v33 += a1;
            	if( v30 && v33 == v32 ){
        	    gEndGameUnk09 = 0;
        	    gEndGameUnk05 = 0;
        	    if( gEndGameUnk03 ) GSoundSpkDelete();
        	    if( gEndGameUnk13 ) gEndGameUnk14 = TimerGetSysTime();
        	    v11 = 1;
            	} else {
            	    if( v12 == v33 && a1 == -1 ){
                	gEndGameUnk09 = 0;
                	gEndGameUnk05 = 0;
                	if( gEndGameUnk03 ) GSoundSpkDelete();
                	if( gEndGameUnk13 ) gEndGameUnk14 = TimerGetSysTime();
                	v11 = 1;
            	    }
		}
	    }
            SoundUpdateAll();
            if( InpUpdate() != -1 ){
                GSoundSpkCancel();
                EndGameUnk14();
                gEndGameUnk03 = 0;
                gEndGameUnk13 = 0;
                break;
            }                
    	}
        InpTaskHalt();
        ArtClose( Obj );
        FadeStep( gFadePaletteC );
        ScrFillSolid( gEndGameSurface, 640, 480, 640, gPalColorCubeRGB[0][0][0] );
        WinUpdate( gEndGameWindow );
    }
    while( MseGetButtons() ) InpUpdate();    
}

void EndGameUnk06( int a1, char *a2 )
{
    ArtFrmHdr_t *result;
    char *ObjData;
    int v7, SysTime, v9;
    CachePool_t *ImgObj;

    result = ArtLoadImg( a1, &ImgObj );
    if( !result ) return;
    ObjData = ArtGetObjData( result, 0, 0 );
    if( ObjData ){
        ScrCopy(ObjData, 640, 480, 640, gEndGameSurface, 640 );
        WinUpdate(gEndGameWindow);
        EndGameUnk10( OBJTYPE( a1 ), a1 & 0xFFF );
        EndGameUnk07(a2);
        v7 = ( gEndGameUnk13 || gEndGameUnk03 ) ? -1 : 3000;
        FadeStep( gPalBase );
        TimerWaitProc( 0x1F4 );
        gEndGameUnk05 = 0;
        gEndGameUnk09 = 0;
        if( gEndGameUnk03 ) GSoundSpkDelete();
        if( gEndGameUnk13 ) gEndGameUnk14 = TimerGetSysTime();
        SysTime = TimerGetSysTime();
        InpTaskUnHalt();
        while( (v9 = InpUpdate()) == -1 && !gEndGameUnk05 && !gEndGameUnk09 && TimerCurrDiff( SysTime ) < v7 ){
            ScrCopy( ObjData, 640, 480, 640, gEndGameSurface, 640 );
            EndGameUnk13();
            WinUpdate(gEndGameWindow);
            SoundUpdateAll();
        }
        InpTaskHalt();
        GSoundSpkCancel();
        EndGameUnk14();
        gEndGameUnk03 = 0;
        gEndGameUnk13 = 0;
        if( v9 == -1 ) TimerWaitProc( 0x1F4 );
        FadeStep(gFadePaletteC);
        while( MseGetButtons() ) InpUpdate();
    }
    ArtClose( ImgObj );
}

void EndGameUnk07( char *NarratorFile )
{
    int i;
    char stmp[ 280 ];
    double g;

    gEndGameUnk03 = 0;
    gEndGameUnk13 = 0;
    GSoundSpkCancel();
    EndGameUnk14();
    gEndGameUnk03 = 0;
    gEndGameUnk13 = 0;
    sprintf( stmp, "%s%s", "narrator/", NarratorFile );
    if( GSoundLoadSpk( stmp, 10, 15, 14 ) != -1 ) gEndGameUnk03 = 1;
    if( gEndGameUnk08 ){
        sprintf( stmp, "%s%s.txt", gEndGameCutsPath, NarratorFile );
        if( !EndGameUnk12( stmp ) ){
            g = ( gEndGameUnk03 ) ? ( GSoundSpkGetSamples() / EndGameUnk20 ) : 0.08;
            for( i = 0; i < EndGameUnk19; i++ ){
                gEndGameUnk15[ i ] = lround( strlen( gEndGameUnk07[ i ] ) * g * 1000.0 +i );
            }                
            gEndGameUnk13 = 1;
        }
    }
}

void EndGameUnk08()
{
    gEndGameUnk09 = 0;
    gEndGameUnk05 = 0;
    if( gEndGameUnk03 ) GSoundSpkDelete();
    if( gEndGameUnk13 ) gEndGameUnk14 = TimerGetSysTime();
}

void EndGameUnk09()
{
    GSoundSpkCancel();
    EndGameUnk14();
    gEndGameUnk03 = 0;
    gEndGameUnk13 = 0;
}

void EndGameUnk10( int FileId, int a2 )
{
    char *s, stmp[ 260 ], fname[ 44 ];

    if( ArtGetFilenameFromList( FileId, a2, fname ) ) return;
    if( (s = strrchr( fname, '.' )) ) *s = '\0';
    if( strlen( fname ) > 8 ) return;
    sprintf( stmp, "%s/%s.pal", "art/intrface", fname );
    PalLoadFromFile( stmp );
}

void EndGameUnk11()
{
    gEndGameUnk05 = 1;
}

int EndGameUnk12( char *fname )
{
    xFile_t *fh;
    char *s, stmp[ 280 ];

    EndGameUnk14();    
    if( !(fh = dbOpen( fname, "rt" ) ) ) return -1;
    while( dbgets( stmp, 256, fh ) ){        
        if( (s = strchr( stmp, '\n' )) ) *s = '\0';
        if( !(s = strchr( stmp, ':' )) ) continue;
        if( EndGameUnk19 >= 50 ) continue;
        gEndGameUnk07[ EndGameUnk19++ ] = strdup( s + 1 );
        EndGameUnk20 += strlen( s + 1 );
    }
    dbClose( fh );
    return 0;
}

void EndGameUnk13()
{
    char *str, *s, *p, c;
    int w,ypos,h,i;
    short Wrap[ 64 ], pOffsets;

    if( EndGameUnk19 <= EndGameUnk21 ){ if( gEndGameUnk13 ) gEndGameUnk09 = 1; return; } 
    if( TimerCurrDiff( gEndGameUnk14 ) > *(gEndGameUnk15 + EndGameUnk21) ){ EndGameUnk21++; return; }    
    if( !(str = gEndGameUnk07[ EndGameUnk21 ]) ) return;
    if( WinTextWrap( str, 540, Wrap, &pOffsets ) ) return;
    h = gFont.ChrHeight();
    ypos = 480 - pOffsets * gFont.ChrHeight();
    for( i = 0; i < (pOffsets - 1); i++, ypos += h ){
        s = &str[ Wrap[ i + 1 ] ];
        p = &str[ Wrap[ i ] ];
        if( s[ -1 ] == ' ' ) s--;
        c = s[ 0 ];
        s[ 0 ] = '\0';
        w = gFont.LineWidth( p );
        ScrFillSolid( &gEndGameSurface[ 160 * ypos ] + (640 - w) / 2, w, h, 640, gPalColorCubeRGB[0][0][0] );
        gFont.Print( &gEndGameSurface[ 640 * ypos ] + (640 - w) / 2, p, w, 640, gPalColorCubeRGB[31][31][31] );
        s[ 0 ] = c;
    }                    
}

void EndGameUnk14()
{
    int i;

    for( i = 0; i < EndGameUnk19; i++ ){
        if( !gEndGameUnk07[ i ] ) continue;
        Free( gEndGameUnk07[ i ] );
        gEndGameUnk07[ i ] = NULL;
    }
    EndGameUnk21 = 0;
    EndGameUnk20 = 0;
    EndGameUnk19 = 0;
}

void EndGameUnk15()
{
    EndGameUnk22 = 1;
}

void EndGameUnk16()
{
    if( !EndGameUnk22 ) return;
    GSoundLoadBg( "10labone", 11, 14, 16 );
    GSoundBgSetCb( NULL );
    InpTaskStop( EndGameUnk16 );
}

int EndGameUnk17()
{
    xFile_t *fh;
    EndGame02_t *p, Eg;
    int len;
    char stmp[ 256 ], delim[ 4 ], *str, *s;

    strcpy( delim, " \t," );
    if( gEndGameEndings ){ Free( gEndGameEndings ); gEndGameEndings = NULL; }
    gEndGameCount = 0;    
    if( !(fh = dbOpen( "data/endgame.txt", "rt" ) ) ) return -1;
    while( dbgets( stmp, 256, fh ) ){
        for( str = stmp; IsTable[(*str + 1)] & 0x02; str++ );
        if( *str == '#' ) continue;
        if( !(s = strtok( str, delim )) ) continue;
        Eg.GVarId = strtol( s, NULL, 10 );        
        if( !(s = strtok( NULL, delim )) ) continue;        
        Eg.GVarVal = strtol( s, NULL, 10 );        
        if( !(s = strtok( NULL, delim )) ) continue;
        Eg.ArtNum = strtol( s, NULL, 10 );        
        if( !(s = strtok( NULL, delim )) ) continue;
        strcpy( Eg.Narrator, s );
        len = strlen( Eg.Narrator ) + 1;
        if( IsTable[ Eg.Narrator[ len - 2 ] + 1 ] & 0x02 ) Eg.Narrator[ len - 2 ] = '\0';
        s = strtok( NULL, delim );
        Eg.PanDir = ( s ) ? strtol( s, NULL, 10 ) : 1;        
        if( !(p = Realloc( gEndGameEndings, sizeof( EndGame02_t ) * (gEndGameCount + 1) )) ){ dbClose( fh ); return -1; }
        gEndGameEndings = p;
        gEndGameCount++;
        memcpy( &p[ gEndGameCount ], &Eg, sizeof( EndGame02_t ) );
    }
    dbClose( fh );
    return 0;
}

void EndGameUnk18()
{
    if( gEndGameEndings ){
        Free( gEndGameEndings );
        gEndGameEndings = NULL;
    }
    gEndGameCount = 0;
}

int EndGameDeathEndingInit()
{
    xFile_t *fh;
    EndGame_t *q, tmp;
    char *p, buff[ 256 ], delim[ 4 ];
    int len, err;
    
    err = 0;
    strcpy( delim, " \t," );
    strcpy( gEndGameNarratorFile, "narrator/nar_5" );
    if( !( fh = dbOpen( "data/enddeath.txt", "rt" ) ) ) return -1;
    while( dbgets( buff, 256, fh ) ){
        for( p = buff; IsTable[ *p + 1 ] & 0x02; p++ );
        if( *p == '#' ) continue;
        if( !(p = strtok( p, delim )) ) continue;
        tmp.GVarId = strtol( p, NULL, 10 );
        if( !(p = strtok( 0, delim )) ) continue;
        tmp.GVarVal = strtol( p, NULL, 10 );
        if( !(p = strtok( 0, delim )) ) continue;
        tmp.AreaKnown = strtol( p, NULL, 10 );
        if( !(p = strtok( 0, delim )) ) continue;
        tmp.AreaNotKnown = strtol( p, NULL, 10 );
        if( !(p = strtok( 0, delim )) ) continue;
        tmp.MinLvl = strtol( p, NULL, 10 );
        if( !(p = strtok( 0, delim )) ) continue;
        tmp.OccurChance = strtol( p, NULL, 10 );
        if( !(p = strtok( 0, delim )) ) continue;
        strcpy( tmp.FileName, p );
        tmp.Dir = '\0';
        len = strlen( tmp.FileName ) + 1;
        if( IsTable[ (tmp.FileName[ len - 2 ] + 1 ) ] & 0x02 ) tmp.FileName[ len - 2 ] = '\0';
        if( !(q = Realloc( gEndGameInfo, sizeof( EndGame_t ) * (gEndGameInfoCnt + 1) )) ){ err = -1; break; }
        memcpy( &q[ gEndGameInfoCnt ], &tmp, sizeof( EndGame_t ) );
        gEndGameInfo = q;
        gEndGameInfoCnt++;
    }
    dbClose( fh );
    return err;
}

void EndGameClose()
{
    if( !gEndGameInfo ) return;
    Free( gEndGameInfo );
    gEndGameInfo = NULL;
    gEndGameInfoCnt = 0;
}

void EndGameSetupDeathEnding( int a1 )
{
    int n, flg, r, i, RandMax;

    flg = 0;
    RandMax = 0;
    if( !gEndGameInfoCnt ){ eprintf( "Error: endgameSetupDeathEnding: No endgame death info!" ); return; }
    strcpy( gEndGameNarratorFile, "narrator/" );
    EndGameChance( &RandMax );
    i = 0;
    if( a1 ){
        if( a1 == 2 ) GMoviePlay( 8, 11 );
    } else if( GlobVarGet( 491 ) ){
        i = 12;
        flg = 1;
    }
    if( !flg ){	
        r = RandMinMax( 0, RandMax );
        for( n = 0; i < gEndGameInfoCnt; i++ ){
            if( gEndGameInfo[ i ].Dir == 1 ){
                n += gEndGameInfo[ i ].OccurChance;
                if( n >= r ) break;
            }                
        }
    }
    strcpy( gEndGameUnk02, gEndGameInfo[ i ].FileName );
    eprintf( "endgameSetupDeathEnding: Death Filename Picked: %s", gEndGameNarratorFile );
}

int EndGameChance( int *a1 )
{
    int i;

    *a1 = 0;
    for( i = 0; i < gEndGameInfoCnt; i++ ){
        gEndGameInfo[ i ].Dir = 0;
        if( gEndGameInfo[ i ].GVarId != -1 ){
    	    if( GlobVarGet( gEndGameInfo[ i ].GVarId ) >= gEndGameInfo[ i ].GVarVal ) continue;
        }
        if( gEndGameInfo[ i ].AreaKnown != -1 && !WmIsAreaKnown( gEndGameInfo[ i ].AreaKnown ) ) continue;
        if( gEndGameInfo[ i ].AreaNotKnown != -1 && WmIsAreaKnown( gEndGameInfo[ i ].AreaKnown ) ) continue;            
        if( FeatGetPoints( FEAT_PT_LV ) < gEndGameInfo[ i ].MinLvl ) continue;
        gEndGameInfo[ i ].Dir = 1;
        *a1 += gEndGameInfo[ i ].OccurChance;
    }
    return 0;
}

char *EndGameSetNarratorFile()
{
    if( !gEndGameInfoCnt ){
        eprintf( "Error: endgameSetupDeathEnding: No endgame death info!" );
        strcpy( gEndGameNarratorFile, "narrator/nar_4" );
    }
    eprintf( "endgameSetupDeathEnding: Death Filename: %s", gEndGameNarratorFile );
    return gEndGameNarratorFile;
}

