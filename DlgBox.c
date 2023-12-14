#include "FrameWork.h"

void DlgBoxList( char *Surf, char **LineList, int Lines, int LineOfs, int Marked, int Width );

int gDBoxUnk01[2] = { 0xDA, 0xD9 };
int gDBoxUnk03[2] = { 0x17, 0x1B };
int gDBoxUnk02[2] = { 0x1d, 0x1d };
int gDBoxUnk04[2] = { 0x51, 0x62 };
int gDBoxUnk08[2] = { 0x33, 0x25 };
int gDBoxUnk05[4] = { 0x05, 0x06, 0xa8, 0x100 };
int gDBoxUnk06[7] = { 0xE0, 0x08, 0x09, 0xB5, 0xB6, 0xC7, 0xC8 };
int gDBoxUnk07[7] = { 0xE1, 0x08, 0x09, 0xB5, 0xB6, 0xC7, 0xC8 };


int DlgBox( char *Str1, char **List, int Lines, int Xpos, int Ypos, int Color1, char *Str2, int Color2, int Flags )
{
    CachePool_t *ImgC, *ImgB, *ImgA, *Img;
    int i,n, MaxLen, y, v26, bt, len,Img1H,Img1W,Img0H,Img0W,WinH,WinW,ExCode,w,a5,v86,v87,type,win,FontId,Xalg;
    char *Img0,**ps,*Img2,*Img1,*h;
    char stmp[260], Path[260];
    short pLines[64], Splitted;
    MsgLine_t Line;
    Msg_t Msg;

    FontId = FontGetCurrent();
    MaxLen = 0;
    a5 = v87 = v86 = 0;
    if( Str2 ) v87 = 1;
    if( Str1 ) v86 = 1;
    if( Flags & 0x10 ){
        v87 = 1;
        Flags = (Flags & ~0x21) | 0x01;
    }
    if( v86 ) MaxLen = gFont.LineWidth( Str1 );
    i = 0;
    if( Lines ){ 
	for( i = 0; i < Lines; i++ ){ 
	    MaxLen = ( gFont.LineWidth( List[ i ] ) <= MaxLen ) ? MaxLen : gFont.LineWidth( List[ i ] ); 
	} 
    }
    if( (Flags & 0x01) || v87 ){ type = 1;  } 
    else if( Flags & 0x02 ){ type = v87; }
    else {
        if( v86 ) i++;
        type = MaxLen > 168 || i > 5;
    }
    if( !(Img0 = ArtLoadBmp( ArtMakeId( 6, gDBoxUnk01[type], 0, 0, 0 ), &Img, &WinW, &WinH ) ) ){ FontSet( FontId ); return -1; }    
    if( ( win = WinCreateWindow(Xpos, Ypos, WinW, WinH, 256, 20) ) == -1 ){ ArtClose( Img ); FontSet( FontId ); return -1; }
    h = WinGetSurface( win );
    memcpy( h, Img0, WinW * WinH );
    Xalg = gDBoxUnk02[ type ];
    y = gDBoxUnk03[ type ];
    if( !(Flags & 0x20) ){
        if( !(Img0 = ArtLoadBmp( ArtMakeId( 6, 209, 0, 0, 0 ), &ImgA, &Img0W, &Img0H )) ){ ArtClose( Img ); FontSet( FontId ); WinClose( win ); return -1; }        
        if( !(Img1 = ArtLoadBmp( ArtMakeId( 6, 9, 0, 0, 0 ), &ImgB, &Img1W, &Img1H)) ){ ArtClose( Img ); ArtClose( ImgA ); FontSet( FontId ); WinClose( win ); return -1; }
        if( !(Img2 = ArtGetBitmap( ArtMakeId( 6, 8, 0, 0, 0 ), 0, 0, &ImgC)) ){ ArtClose( Img ); ArtClose( ImgA ); ArtClose( ImgB ); FontSet( FontId ); WinClose( win ); return -1; }
        v26 = v87 ? gDBoxUnk08[ type ] : ( ( WinW - Img0W ) / 2 );
        ScrCopy( Img0, Img0W, Img0H, Img0W, (h + v26 + WinW * gDBoxUnk04[ type ] ), WinW );
        if( MessageInit(&Msg) != 1 ){ ArtClose( Img ); ArtClose( ImgA ); ArtClose( ImgB ); ArtClose( ImgC ); FontSet( FontId ); WinClose( win ); return -1; }
        sprintf( Path, "%s%s", gGamePath, "DBOX.MSG" );
        if( MessageLoad( &Msg, Path ) != 1 ){ ArtClose( Img ); ArtClose( ImgA ); ArtClose( ImgB ); ArtClose( ImgC ); FontSet( FontId ); return -1; }
        FontSet( 103 );
        Line.Id = ((Flags & 0x10) != 0) + 100;
        if( MessageGetMsg( &Msg, &Line ) == 1 ) gFont.Print( h + v26 + WinW * (gDBoxUnk04[ type ] + 3) + 35, Line.Text, WinW, WinW, gPalColorCubeRGB[18][17][3] );
        bt = WinCreateButton( win, v26 + 13, gDBoxUnk04[type] + 4, Img1W, Img1H, -1, -1, -1, 500, Img2, Img1, 0, 32 );
        if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
        a5 = 1;
    }
    if( v87 && type == 1 ){
	if( a5 ){
    	    if( (Flags & 0x10) != 0 ) Str2 = MessageGetMessage( &Msg, &Line, 102 );
    	    FontSet( 103 );
    	    ScrCopyAlpha( Img0, Img0W, Img0H, Img0W, (h + Img0W + gDBoxUnk08[type] + WinW * gDBoxUnk04[type] + 24), WinW );
    	    gFont.Print((h + Img0W + gDBoxUnk08[type] + WinW * (gDBoxUnk04[type] + 3) + 59), Str2, WinW, WinW, gPalColorCubeRGB[18][17][3] );
    	    bt = WinCreateButton(win, Img0W + gDBoxUnk08[type] + 37, gDBoxUnk04[type] + 4, Img1W, Img1H, -1, -1, -1, 501, Img2, Img1, 0, 32 );
    	    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
	    a5 = 1;
	} else {
    	    if( !(Img0 = ArtLoadBmp( ArtMakeId( 6, 209, 0, 0, 0 ), &ImgA, &Img0W, &Img0H )) ){ ArtClose( Img ); FontSet( FontId ); WinClose( win ); return -1; }    	    
    	    if( !(Img1 = ArtLoadBmp( ArtMakeId( 6, 9, 0, 0, 0 ), &ImgB, &Img1W, &Img1H )) ){ ArtClose( Img ); ArtClose( ImgA ); FontSet( FontId ); WinClose( win ); return -1; }    	    
    	    if( !(Img2 = ArtGetBitmap( ArtMakeId(6, 8, 0, 0, 0), 0, 0, &ImgC)) ){ ArtClose( Img ); ArtClose( ImgA ); ArtClose( ImgB ); FontSet( FontId ); WinClose( win ); return -1; }
    	    if( MessageInit( &Msg ) != 1 ){ ArtClose(Img); ArtClose(ImgA); ArtClose(ImgB); ArtClose(ImgC); FontSet( FontId ); WinClose( win ); return -1; }
    	    sprintf( Path, "%s%s", gGamePath, "DBOX.MSG" );
    	    if( MessageLoad( &Msg, Path ) != 1 ){ ArtClose(Img); ArtClose(ImgA); ArtClose(ImgB); ArtClose(ImgC); FontSet( FontId ); WinClose( win ); return -1; }
    	    w = gDBoxUnk08[type];
    	    ScrCopyAlpha( Img0, Img0W, Img0H, Img0W, (h + w + WinW * gDBoxUnk04[ type ]), WinW );
    	    FontSet( 103 );
    	    gFont.Print( h + w + WinW * (gDBoxUnk04[ type ] + 3) + 35, Str2, WinW, WinW, gPalColorCubeRGB[18][17][3] );
    	    bt = WinCreateButton( win, w + 13, gDBoxUnk04[ type ] + 4, Img1W, Img1H, -1, -1, -1, 501, Img2, Img1, 0, 32 );
    	    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
	    a5 = 1;
        }
    }
    FontSet( 101 );
    if( !( Flags & 0x0800 ) ){	
	n = Lines;
	if( v86 ) n++;
	y += ((gDBoxUnk05[ v87 ] * gFont.ChrHeight()) >> 1) - ((n * gFont.ChrHeight()) >> 1);    
    }

    if( v86 ){
        if( (Flags & 4) != 0 ){
            gFont.Print( h + Xalg + y * WinW, Str1, WinW, WinW, Color1 );
        } else {
            gFont.Print( h + y * WinW + (WinW - gFont.LineWidth( Str1 )) / 2, Str1, WinW, WinW, Color1 );
        }
        y += gFont.ChrHeight();
    }
    if( Lines ){    
        for(i = 0; i < Lines; i++ ){
            Splitted = 1;
            if( gFont.LineWidth( List[ i ] ) <= WinW - 26 ){
                if( Flags & 0x04 ){
                    gFont.Print( h + Xalg + y * WinW, List[ i ], WinW, WinW, Color2 );
                } else {
                    gFont.Print( h + y * WinW + (WinW - gFont.LineWidth( List[ i ] )) / 2, List[ i ], WinW, WinW, Color2 );
                }
                y += gFont.ChrHeight();
            } else {
                if( TextWrap( List[ i ], WinW - 26, pLines, &Splitted) ) eprintf( "\nError: dialog_out" );                
                for( n = 1,ps = &List[ i ]; n < Splitted; n++, y += gFont.ChrHeight() ){
                    len = pLines[n] - pLines[n - 1];
                    if( len >= 260 ) len = 259;
                    strncpy( stmp, &(*ps)[pLines[n - 1]], len );
                    stmp[ len ] = 0;
                    if( Flags & 4 ){
                        gFont.Print( h + Xalg + y * WinW, stmp, WinW, WinW, Color2 );
                    } else {
                        gFont.Print( h + y * WinW + (WinW - gFont.LineWidth(stmp)) / 2, stmp, WinW, WinW, Color2 );
                    }                    
                }
            }                
        }
    }
    WinUpdate( win );

    for( ExCode = -1; ExCode == -1; ){
        switch( InpUpdate() ){
            case 500: ExCode = 1; break;
            case '\r': GSoundPlay( "ib1p1xx1" ); ExCode = 1; break;
            case 501: case KEY_ESC: ExCode = 0; break;
            case 'Y': case 'y': if( Flags & 0x10 ) ExCode = 1; break;
            case 'N': case 'n': if( Flags & 0x10 ) ExCode = 0; break;
        }
        if( gMenuEscape ) ExCode = 1;
    }
    WinClose( win );
    ArtClose( Img );
    FontSet( FontId );
    if( a5 ){
        ArtClose( ImgA );
        ArtClose( ImgB );
        ArtClose( ImgC );
        MessageClose( &Msg );
    }
    return ExCode;
}

int DlgBoxLoad( char *Title, char **pList, char *Selected, int Lines, int Xpos, int Ypos )
{
    char stmp[260], *Imgs[7], *surf;
    Geo_t Geo[7];
    CachePool_t *ImgH[7];
    MsgLine_t Line;
    Msg_t Msg;
    int j,i,bt,time,sel,v38,v41,v42,SysTime,v44,v46,v49,bottom;
    int right,v66,FontId,win,v72,ExCode,v74,Action,Width,v81,LineOffs;
    int Marked,v84,v85;

    FontId = FontGetCurrent();
    Marked = 0;
    v74 = -2;
    ExCode = -1;
    LineOffs = 0;
    v84 = Lines > 12;
    v81 = Lines - 13;
    if( v81 < 0 ){
        v81 = Lines - 1;
        if( v81 < 0 ) v81 = 0;
    }
    for( i = 0; i < 7; i++ ){
        if( !(Imgs[ i ] = ArtLoadBmp( ArtMakeId( 6, gDBoxUnk06[ i ], 0, 0, 0), &ImgH[ i ], &Geo[ i ].Width, &Geo[ i ].Height)) ) break;        
    }
    if( i < 7 ){    
	if( --i >= 0 ){
    	    j = i;
    	    do {
        	ArtClose( ImgH[ j-- ] );
    		--i;
    	    }while( j >= 0 );
	}
	return -1;
    }
    
    Width = Geo[0].Width;
    win = WinCreateWindow( Xpos, Ypos, Geo[0].Width, Geo[0].Height, 256, 20 );
    if( win == -1 ){ for( i = 0; i < 7; i++ ) ArtClose( ImgH[ i ] ); return -1; }
    surf = WinGetSurface( win );
    memcpy( surf, Imgs[0], Geo[0].Width * Geo[0].Height );
    if( MessageInit( &Msg ) != 1 ){ WinClose( win ); for( i = 0; i < 7; i++ ) ArtClose( ImgH[ i ] ); return -1; }
    sprintf( stmp, "%s%s", gGamePath, "DBOX.MSG" );
    if( MessageLoad(&Msg, stmp) != 1 ){ WinClose( win ); for( i = 0; i < 7; i++ ) ArtClose( ImgH[ i ] ); return -1; }
    FontSet( 103 );
    gFont.Print( &surf[ 187 * Width + 79  ], MessageGetMessage( &Msg, &Line, 100 ), Width, Width, gPalColorCubeRGB[18][17][3] );
    gFont.Print( &surf[ 187 * Width + 182 ], MessageGetMessage( &Msg, &Line, 103 ), Width, Width, gPalColorCubeRGB[18][17][3] );
    bt = WinCreateButton( win, 58,  187, Geo[ 2 ].Width, Geo[ 2 ].Height, -1, -1, -1, 500, Imgs[ 1 ], Imgs[ 2 ], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    bt = WinCreateButton( win, 163, 187, Geo[ 2 ].Width, Geo[ 2 ].Height, -1, -1, -1, 501, Imgs[ 1 ], Imgs[ 2 ], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    bt = WinCreateButton( win, 36, 44, Geo[ 6 ].Width, Geo[ 6 ].Height, -1, 505, 506, 505, Imgs[ 5 ], Imgs[ 6 ], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    bt = WinCreateButton( win, 36, Geo[ 6 ].Height + 44, Geo[ 4 ].Width, Geo[ 4 ].Height, -1, 503, 504, 503, Imgs[ 3 ], Imgs[ 4 ], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    WinCreateButton( win, 55, 49, 190, 124, -1, -1, -1, 502, 0, 0, 0, 0 );
    if( Title ) gFont.Print( &surf[ 16 * Width + 49 ], Title, Width, Width, gPalColorCubeRGB[18][17][3] );
    FontSet( 101 );
    DlgBoxList( surf, pList, Lines, LineOffs, Marked, Width );

    WinUpdate( win );
    v72 = ' ';

    while( ExCode == -1 ){
        time = TimerGetSysTime();
        sel = InpUpdate();
        if( sel == KEY_ESC ){ ExCode = 1; continue; }
        Action = v85 = v66 = 0;
        switch( sel ){
    	    case 500:
        	    if( Lines ){
            		strncpy( Selected, (&pList[Marked])[ LineOffs ], 16 );
            		ExCode = 0;
        	    } else {
            		ExCode = 1;
        	    }
        	    break;
    	    case 501: ExCode = 1; break;
    	    case 502: 
            	    if( Lines == 0 ) break;
            	    MseGetCursorPosition( &right, &bottom ); 
            	    v38 = (bottom - Ypos - 49) / gFont.ChrHeight();
            	    if( v38 - 1 < 0 ) v38 = 0;
            	    if( v84 || v38 < Lines ){
                    	if( v38 > 11 ) v38 = 11;
            	    } else {
                    	v38 = Lines - 1;
            	    }
            	    Marked = v38;
            	    if( v38 == v74 ){
                    	GSoundPlay( "ib1p1xx1" );
                    	strncpy( Selected, pList[ LineOffs + v74 ], 16 );
                    	ExCode = 0;
            	    }
            	    v74 = Marked;
            	    DlgBoxList( surf, pList, Lines, LineOffs, Marked, Width );
            	    break;
            case 506: Action = 1; break;
            case 504: Action = 2; break;
            case 328:
                    if( --LineOffs < 0 ){
                        if( --Marked < 0 ) Marked = 0;
                        LineOffs = 0;
                    }
                    DlgBoxList( surf, pList, Lines, LineOffs, Marked, Width );
                    v74 = -2;
                    break;
            case 336:
                    v41 = Marked + 1;
                    if( v84 ){
                        if( ++LineOffs > v81 ){
                            Marked++;
                            if( v41 > 12 ) Marked = 11;
                            LineOffs = v81;
                        }
                    } else {
                        Marked++;
                        if( v41 > v81 ) Marked = v81;
                    }
                    DlgBoxList( surf, pList, Lines, LineOffs, Marked, Width );
                    v74 = -2;
                    break;
            case 327:
                    Marked = 0;
                    LineOffs = 0;
                    v74 = -2;
                    DlgBoxList( surf, pList, Lines, 0, 0, Width );
                    break;
            case 335:
                    if( v84 ){
                        Marked = 11;
                        LineOffs = v81;
                    } else {
                        Marked = v81;
                        LineOffs = 0;
                    }
                    DlgBoxList( surf, pList, Lines, LineOffs, Marked, Width );
                    v74 = -2;
                    break;
    	}                                    
    	if( !Action ){
    	    WinUpdate( win );
    	    if( !--v72 ){ v72 = ' '; v74 = -2; }
//    	    while( TimerCurrDiff( time ) < 41 );
    	} else {
    	    v42 = 4;
    	    v74 = -2;
    	    do{
                SysTime = TimerGetSysTime();
            	v44 = ++v85;
            	if(( !v66 && v44 == 1 ) || ( v66 == 1 && v85 > 14.4 )){
        	    v66 = 1;
        	    if( v85 > 14.4 && ++v42 > 24 ) v42 = 24;
        	    if( Action == 1 ){
            		if( --LineOffs < 0 ){
                	    LineOffs = 0;
                	    Marked -= Action;
                	    if( Marked < 0 ) Marked = 0;
            		}
        	    } else {
            		v46 = Marked + 1;
            		if( v84 ){
                	    if( ++LineOffs > v81 ){
                    		Marked++;
                    		LineOffs = v81;
                    		if( v46 > 11 ) Marked = 11;
                	    }
            		} else {
                	    Marked++;
                	    if( v46 > v81 ) Marked = v81;
            		}
        	    }
        	    DlgBoxList( surf, pList, Lines, LineOffs, Marked, Width );
        	    WinUpdate( win );
            	}
            	if( v85 > 14.4 ){
//            	    while( TimerCurrDiff( SysTime ) < 1000 / v42 );
            	} else {
//            	    while( TimerCurrDiff( SysTime ) < 41 );
            	}
            	if( gMenuEscape ){ ExCode = 1; break; }
            	v49 = InpUpdate();                     
    	    }while( v49 != 505 && v49 != 503 );
    	}
    	if( gMenuEscape ) ExCode = 1;            
    }
    WinClose( win );
    for( i = 0; i < 7; i++ ) ArtClose( ImgH[ i ] );
    MessageClose( &Msg );
    FontSet( FontId );
    return ExCode;
}

#define FNAME_CUT( Name )	for( i = 0; (i < 12) && Name[ i ] && (Name[ i ] != '.'); i++ );  Name[ i ] = '\0';

int DlgBoxSave( char *Title, char **pList, char *Selected, int Lines, int Xpos, int Ypos )
{
    Geo_t Geo[7];
    CachePool_t *ImgH[7];
    MsgLine_t Line;
    Msg_t Msg;
    CachePool_t **Img;
    int *p_Height;
    unsigned int v17;
    unsigned int v35;
    unsigned int v37;
    unsigned int v43;
    unsigned int v44;
    unsigned int v48;
    unsigned int v49;
    unsigned int v58;
    unsigned int v59;
    unsigned int v61;
    unsigned int v63;
    unsigned int v73;
    unsigned int v77;
    unsigned int v78;
    unsigned int v79;
    unsigned int v82;
    unsigned int v104;
    unsigned int w;
    unsigned int h;
//    char *Bmp;
    char *v16;
//    char *s;
//    char *_v2;
    char **v30;
    char *v32;
    char *EditSurf;
    char **v54;
    char *v55;
    char *pd;
    char *ps;
//    char c;
    char EditField[32];
    char *Imgs[7];
    char *surf;
//    char *pal;
//    char **pDst;
//    char **v137;
    int Id;
    int v10;
    int v11;
    int i;
    int j;
//    int _v1;
    int v24;
    int bt;
    int v29;
    int v31;
    int v33;
    int v36;
    int v38;
    int SysTime;
    int v41;
    int v42;
//    int v45;
//    int v46;
//    int v47;
    int v50;
//    int _h;
    int v52;
    int v56;
    int v57;
    int v60;
    int aa;
    int v64;
    int v65;
    int v66;
    int v67;
    int v68;
    int v69;
    int v71;
    int v72;
    int v75;
    int v76;
    int v80;
    int v83;
    int v84;
    int v85;
    int v86;
//    int k;
//    int col;
    int stmp[65];
    int v105;
    int v112;
    int BlinkCnt;
    int Toggle;
    int CurPos;
    int bottom;
    int right;
    int FontId;
    int ExCode;
    int v123;
    int PrevSel;
    int v125;
    int v126;
    int LineOffs;
    int Offset;
    int chr;
    int Width;
//    int *pWidth;
    int win;
    int v139;
    int v140;
    int v141;
    int v142;
    int v143;
    int v144;
DD
    FontId = FontGetCurrent();
    LineOffs = 0;
    Toggle = 0;
    Offset = 0;
    PrevSel = -2;
    ExCode = -1;
    BlinkCnt = 3;
    v143 = Lines > 12;
    v126 = Lines - 13;

    if( v126 < 0 ){
        v126 = Lines - 1;
        if( Lines - 1 < 0 ) v126 = 0;
    }
    for( i = 0; i < 7; i++ ){
        if( !(Imgs[ i ] = ArtLoadBmp( ArtMakeId( 6, gDBoxUnk07[ i ], 0, 0, 0 ), &ImgH[ i ], &Geo[i].Width, &Geo[i].Height )) ) break;
    }
    if( i < 7 ){ for( i--; i >= 0; i-- ) ArtClose( ImgH[ i ] ); return -1; }
    Width = Geo[0].Width;
    win = WinCreateWindow( Xpos, Ypos, Geo[0].Width, Geo[0].Height, 256, 20 );
    if ( win == -1 ){ for( i = 0; i < 7; i++ ) ArtClose( ImgH[ i ] ); return -1; }
    surf = WinGetSurface( win );
    memcpy( surf, Imgs[ 0 ], Geo[0].Width * Geo[0].Height );
    if( MessageInit( &Msg ) != 1 ){ WinClose( win ); for( i = 0; i < 7; i++ ) ArtClose( ImgH[ i ] ); return -1; }
    sprintf( stmp, "%s%s", gGamePath, "DBOX.MSG" );
    if( MessageLoad( &Msg, stmp ) != 1 ){ WinClose( win ); for( i = 0; i < 7; i++ ) ArtClose( ImgH[ i ] ); return -1; }
    FontSet( 103 );
    gFont.Print( &surf[ 213 * Width + 79  ], MessageGetMessage( &Msg, &Line, 100 ), Width, Width, gPalColorCubeRGB[18][17][3] );
    gFont.Print( &surf[ 213 * Width + 182 ], MessageGetMessage( &Msg, &Line, 103 ), Width, Width, gPalColorCubeRGB[18][17][3] );
    bt = WinCreateButton( win, 58, 214, Geo[2].Width, Geo[2].Height, -1, -1, -1, 500, Imgs[1], Imgs[2], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    bt = WinCreateButton( win, 163, 214, Geo[2].Width, Geo[2].Height, -1, -1, -1, 501, Imgs[1], Imgs[2], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    bt = WinCreateButton( win, 36, 44, Geo[6].Width, Geo[6].Height, -1, 505, 506, 505, Imgs[5], Imgs[6], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    bt = WinCreateButton( win, 36, Geo[6].Height + 44, Geo[4].Width, Geo[4].Height, -1, 503, 504, 503, Imgs[3], Imgs[4], 0, 32 );
    if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, GSoundPlayReleaseBt );
    WinCreateButton( win, 55, 49, 190, 124, -1, -1, -1, 502, 0, 0, 0, 0 );
    if( Title ) gFont.Print( &surf[16 * Width + 49], Title, Width, Width, gPalColorCubeRGB[18][17][3] );
    FontSet( 101 );
    h = gFont.ChrHeight();
    w = gFont.LineWidth("_") - 4;
    DlgBoxList( surf, pList, Lines, Offset, LineOffs, Width );
    FNAME_CUT( Selected );
    strncpy( EditField, Selected, 32 );
    aa = strlen( EditField );
    EditField[ aa + 1 ] = '\0';
    EditField[ aa ] = ' ';
    CurPos = v36;
    v38 = 190 * Width + 57;
    EditSurf = &surf[ v38 ];
    ScrFillSolid( EditSurf, gFont.LineWidth( EditField ), h, Width, 100 );
    gFont.Print( EditSurf, EditField, Width, Width, gPalColorCubeRGB[0][31][0] );

    WinUpdate( win );
    v112 = ' ';

    if( ExCode == -1 ){
        v105 = v139 = v140 = v38;
        v104 = h - 2;
        v141 = Lines - 1;
        do{
            SysTime = TimerGetSysTime();
            v41 = InpUpdate();
            v123 = v144 = v125 = 0;
            chr = v41;
            switch( v41 ){
                case 500: ExCode = 0; break;
                case 13: GSoundPlay("ib1p1xx1"); ExCode = 0; break;
                case 501: case 27: ExCode = 1; break;
                case 339: case 8:
        	    if( CurPos < 1 ) break;
            	    ScrFillSolid( EditSurf, gFont.LineWidth(EditField), h, Width, 100 );
            	    EditField[ CurPos-- ] = '\0';
            	    EditField[ CurPos ] = ' ';
            	    gFont.Print( EditSurf, EditField, Width, Width, gPalColorCubeRGB[0][31][0] );
            	    WinUpdate( win );
            	    break;
            	default:
        	    if( chr < ' ' || chr > 'z' || CurPos >= 8 ){
            		if( chr != 502 || !Lines ){
                	    switch( chr ){
                    		case 506: v123 = 1; break;
                    		case 504: v123 = 2; break;
                    		case 328:
                        	    if( --Offset < 0 ){ if( --LineOffs < 0 ) LineOffs = 0; Offset = 0; }
                        	    DlgBoxList(surf, pList, Lines, Offset, LineOffs, Width);
                		    PrevSel = -2;
                        	    break;
                    		case 336:
                        	    aa = LineOffs + 1;
                        	    if( v143 ){ 
                        		if( ++Offset >= v126 ){ LineOffs++; if( aa >= 12 ) LineOffs = 11; Offset = v126; }
                        	    } else {
                            		LineOffs++; if( aa > v126 ) LineOffs = v126;
                        	    }
                        	    DlgBoxList(surf, pList, Lines, Offset, LineOffs, Width);
                		    PrevSel = -2;
                        	    break;
                    		case 327:
                        	    LineOffs = Offset = 0;
                        	    DlgBoxList( surf, pList, Lines, 0, 0, Width );
                		    PrevSel = -2;
                        	    break;
                    		case 335:
                        	    if( v143 ){
                            		LineOffs = 11; Offset = v126;
                        	    } else {
                            		LineOffs = v126; Offset = 0;
                        	    }
                        	    DlgBoxList( surf, pList, Lines, Offset, LineOffs, Width );
                		    PrevSel = -2;
                        	    break;
                	    }
            		} else {
            		    // 502
            		    MseGetCursorPosition( &right, &bottom );                
            		    LineOffs = (bottom - Ypos - 49) / gFont.ChrHeight();
            		    if( LineOffs - 1 < 0 ) LineOffs = 0;
            		    if( v143 || LineOffs < Lines ){
                		if( LineOffs > 11 ) LineOffs = 11;
            		    } else {
                		LineOffs = v141;
            	    	    }
            		    if( LineOffs == PrevSel ){ // double click
                		GSoundPlay( "ib1p1xx1" );
                		strncpy( Selected, pList[ LineOffs + Offset ], 16 );
                		FNAME_CUT( Selected );
                		ExCode = 2;
            		    } else { // select
                		PrevSel = LineOffs;
                		ScrFillSolid( EditSurf, gFont.LineWidth( EditField ), h, Width, 100 );
                		strncpy( EditField, pList[ LineOffs + Offset ], 16 );
                		FNAME_CUT( EditField );
                		CurPos = strlen( EditField );
                		EditField[ CurPos ] = ' ';
                		EditField[ CurPos + 1 ] = '\0';
                		gFont.Print( EditSurf, EditField, Width, Width, gPalColorCubeRGB[0][31][0] );
                		DlgBoxList( surf, pList, Lines, Offset, LineOffs, Width );
            		    }
            		}
        	    } else if( CharEditStrChrMark( chr ) ){
            		ScrFillSolid( EditSurf, gFont.LineWidth( EditField ), h, Width, 100 );
            		EditField[ CurPos++ ] = chr;
            		EditField[ CurPos ] = ' ';
            		EditField[ CurPos + 1 ] = '\0';
            		gFont.Print( EditSurf, EditField, Width, Width, gPalColorCubeRGB[0][31][0] );
            		WinUpdate( win );
        	    }
        	    break;
    	    }            
	    if( !v123 ){
    		if( --BlinkCnt <= 0 ){ // blinking cursor
        	    BlinkCnt = 3;
        	    ScrFillSolid( surf + v38 + gFont.LineWidth( EditField ) - w, w, v104, Width, Toggle ? 100 : gPalColorCubeRGB[0][31][0] );
    		    Toggle = !Toggle;
    		}
    		WinUpdate( win );
		aa = v112;
    		v112--;
    		if( aa == 1 ){ v112 = 32; PrevSel = -2; }
//    		while( TimerCurrDiff( SysTime ) < 41 );
	    } else {
    		v63 = 4;
    		PrevSel = -2;
    		while( 1 ){ // edycja
//        	    v64 = TimerGetSysTime();
        	    v142 = 0;
        	    v65 = v144 + 1;
        	    Toggle = 1;
        	    v144++;
        	    if( ( !v125 && v65 == 1 ) || ( v125 == 1 && v144 > 14.4 ) ){
        		v125 = 1;
        		if( v144 > 14.4 && ++v63 > 24 ) v63 = 24;
        		if( v123 == 1 ){
            		    if( --Offset < 0 ){ v66 = 0; Offset = 0; LineOffs -= v123; if( LineOffs < 0 ) LineOffs = v66; }
        		} else {
            		    v67 = LineOffs + 1;
            		    if( v143 ){
            			if( ++Offset > v126 ){ LineOffs++; Offset = v126; if( v67 > 11 ) LineOffs = 11; }
            		    } else {
            			v66 = v126; LineOffs++; if( v67 > v126 ) LineOffs = v66;
            		    }
        		}
        	        DlgBoxList( surf, pList, Lines, Offset, LineOffs, Width );
        		WinUpdate( win );
    		    }
        	    v68 = v142 - 1;
        	    if( v144 > 14.4 ){
            		BlinkCnt = v142 - 1;
            		if( v68 <= 0 ){
                	    BlinkCnt = 3;
                	    ScrFillSolid( &surf[ v38 + gFont.LineWidth( EditField ) - w ], w, v104, Width, Toggle ? 100 : gPalColorCubeRGB[0][31][0] );
                	    Toggle = !Toggle;
            		}
            		while( TimerCurrDiff( v64 ) < 1000 / v63 );
    		    } else {
            		BlinkCnt = v142 - 1;
            		if( v68 <= 0 ){
                	    BlinkCnt = 3;
                	    ScrFillSolid( &surf[ v38 + gFont.LineWidth( EditField ) - w ], w, v104, Width, Toggle ? 100 : gPalColorCubeRGB[0][31][0] );
                	    Toggle = !Toggle;
            		}
//            		while( TimerCurrDiff( v64 ) < 41 );
        	    }
        	    if( gMenuEscape ){ ExCode = 1; break; }
        	    v80 = InpUpdate();
        	    if( v80 == 505 || v80 == 503 ) break;
    		}       
	    }     
	    if( gMenuEscape ) ExCode = 1;
	}while( ExCode == -1 );
    }
    if( ExCode ){
        if( ExCode == 2 ) ExCode = 0;
    } else if( CurPos ){
        EditField[ CurPos ] = '\0';
        strcpy( EditField, Selected );
    } else {
        ExCode = 1;
    }
    WinClose( win );
    for( i = 0; i < 7; i++ ) ArtClose( ImgH[ i ] );
    MessageClose( &Msg );
    FontSet( FontId );
    return ExCode;
}

void DlgBoxList( char *Surf, char **LineList, int Lines, int LineOfs, int Marked, int Width )
{
    int i, h, y;
    char **s;

    h = gFont.ChrHeight();
    y = 49;
    ScrFillSolid( Surf + 49 * Width + 55, 190, 124, Width, 100 );
    if( Lines == 0 ) return;    
    if( Lines - LineOfs > 12 ) Lines = 12;
    s = &LineList[ LineOfs ];
    for( i = 0; i < Lines; i++, y +=h ){
        gFont.Print( Surf + Width * y + 55, *s++, Width, Width, ( i == Marked ) ? gPalColorCubeRGB[31][31][11] : gPalColorCubeRGB[0][31][0] );
    }    
}

