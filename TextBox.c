/* Not done */
#include "FrameWork.h"

int TextBoxEditNumber( int WinId, int *pValue, int Digits, int Clear, char a5, int posX, int posY );
int TextBoxUnk09( int min, int max );

int gTextBoxWinId = -1;
WinDlg_t *gTextBoxGuard = NULL;
int gTextBoxXpos;
int gTextBoxYpos;


int TextBoxFileSelect( char *Title, char **FileList, int Win, void (*Cb)(), int w, int h, int flags )
{
    return TextBoxSelect( Title, FileList, Win, Cb, w, h, flags, 0 );
}

int TextBoxSelect( char *msg, char **FileList, int n, void (*Cb)(), int a0, int a1, int a7, int a8 )
{
    VidRect_t Area;
    Window_t *Window;
    int i,v8,v10,v14,v11,v19,v20,v46,v21,v22,sel,ecc;
    int bottom,right,a3,v97,v102,Ypos,win,arg4,posY,Pitch,zgz;
    unsigned int Width, v111, v112, a2;
    char *p;

    v8 = a8;
    Pitch = n;
    v111 = -1;
    v102 = -1;
    if( !gWinSys.Init ) return -1;
    a2 = TextBoxListWidth( FileList, n );
    v10 = a2 + 16;
    v11 = gFont.LineWidth( msg );
    if( v11 > (a2 + 16) ){
        a2 += v11 - v10;
        v10 = v11;
    }
    v14 = v10 + 20;
    zgz = 10;
    for( i = 13; i > 8; i--, zgz-- ){
        v97 = i * gFont.ChrHeight() + 22;
        if( (win = WinCreateWindow( a0, a1, v14, v97, 256, 20 ) ) != -1 ) break;
    }
    if( win == -1 ) return -1;
    Window = WinGetWindow( win );
    ScrRectangle( Window->Surface, v14, 0, 0, v14 - 1, v97 - 1, COLOR( 0, 0, 0 ) );
    ScrShadowRectangle( Window->Surface, v14, 1, 1, v14 - 2, v97 - 2, COLOR( 0, 0, gWinSys.Colors[ 1 ] ), COLOR( 0, 0, gWinSys.Colors[ 2 ] ) );
    ScrFillSolid( &Window->Surface[ 5 * v14 + 5 ], v14 - 11, gFont.ChrHeight() + 3, v14, COLOR( 0, 0, gWinSys.Colors[ 0 ] ) );
    gFont.Print( &Window->Surface[ v14 / 2 + 8 * v14 - gFont.LineWidth( msg ) / 2 ], msg, v14, v14, COLOR( 0, 0, gWinSys.Colors[ 3 ] ) | 0x10000 );
    ScrShadowRectangle( Window->Surface, v14, 5, 5, v14 - 6, gFont.ChrHeight() + 8, COLOR( 0, 0, gWinSys.Colors[ 2 ] ), COLOR( 0, 0, gWinSys.Colors[ 1 ] ) );
    arg4 = gFont.ChrHeight() + 16;
    v19 = gFont.ChrHeight();
    a3 = zgz * v19 + arg4;
    p = &Window->Surface[ v14 * arg4 + 8 ];
    Ypos = 8;
    ScrFillSolid( p - 2 * v14 - 3, a2 + 6, zgz * v19 + 2, v14, COLOR( 0, 0, gWinSys.Colors[ 0 ] ));
    if( a8 < 0 || a8 >= Pitch ) v8 = 0;
    if( Pitch - v8 >= zgz ){
        v22 = 0;
    } else {
        v20 = Pitch - zgz;
        if( Pitch - zgz < 0 ) v20 = 0;
        v21 = v8;
        v8 = v20;
        v22 = v21 - v20;
    }
    WinDrawTextList( win, &FileList[ v8 ], ( Pitch >= zgz ) ? zgz : Pitch, a2, Ypos, arg4, a7 | 0x2000000 );
    ScrEnlight( &p[ v14 * v22 * gFont.ChrHeight() ], a2, gFont.ChrHeight(), v14 );
    ScrShadowRectangle(Window->Surface, v14, 5, arg4 - 3, a2 + 10, a3, COLOR( 0, 0, gWinSys.Colors[ 2 ] ), COLOR( 0, 0, gWinSys.Colors[ 1 ] ) );
    WinAddButton( win, v14 - 25, arg4 - 3, -1, -1, 328, -1, "\x18\x18\x18", 0 );
    WinAddButton( win, v14, a3 - gFont.ChrHeight() - 5, -1, -1, 336, -1, "\x19\x19\x19", 0 );
    WinAddButton( win, v14 / 2 - 32, v14 - gFont.ChrHeight() - 6, -1, -1, -1, 27, "Done", 0 );
    posY = arg4 + gFont.ChrHeight() + 7;
    Width = 14;
    ScrFillSolid( &Window->Surface[ v14 - 21 + v14 * posY ], 15, a3 - posY - gFont.ChrHeight() - 8, v14, COLOR( 0, 0, gWinSys.Colors[ 0 ] ) );
    WinCreateButton( win, v14 - 21, posY, 15, v14 - gFont.ChrHeight() - 8, -1, -1, 2048, -1, 0, 0, 0, 0 );
    ScrShadowRectangle( Window->Surface, v14, v14 - 22, posY - 1, v14 - 21 + 15, v14 - gFont.ChrHeight() - 9, COLOR( 0, 0, gWinSys.Colors[ 2 ] ), COLOR( 0, 0, gWinSys.Colors[ 1 ] ) );
    ScrShadowRectangle( Window->Surface, v14, v14 - 21, posY, v14 - 21 + 14, posY + 14, COLOR( 0, 0, gWinSys.Colors[ 1 ] ), COLOR( 0, 0, gWinSys.Colors[ 2 ] ));
    ScrEnlight( &Window->Surface[ v14 - 21 + v14 * posY ], 14, 14, v14 );
    for( i = 0; i < zgz; i++ ){
        WinCreateButton( win, Ypos, v14 * gFont.ChrHeight() + arg4, a2, gFont.ChrHeight(), 512 + i, -1, 1024 + i, -1, 0, 0, 0, 0 );
    }
    WinCreateButton( win, 0, 0, v14, gFont.ChrHeight() + 8, -1, -1, -1, -1, 0, 0, 0, 16 );
    WinUpdate( win );
    v112 = Pitch - zgz;
    while( 1 ){
        sel = InpUpdate();
        MseGetCursorPosition( &right, &bottom );
        if( sel != 13 && (sel < 1024 || sel >= (zgz + 1024) ) ){
    	    if( sel == 2048 ){
    		if( Window->Geometry.tp + posY > bottom ){
            	    sel = 329;
    		} else {
    		    if( (Window->Geometry.tp + Width + posY) >= bottom ) continue;
        	    sel = 337;
        	}
            }
        } else {
    	    if( v22 != -1 ){
                v102 = v8 + v22;
                if( v8 + v22 < Pitch ){
            	    if( !Cb ){
                    	WinClose( win );
			return v102;
            	    }
            	    Cb();
                }
                v102 = -1;
    	    }
    	    continue;
        }        
        if( sel == 27 ) break;
        if( ( sel >= 512) && ( sel < zgz + 512 ) ){
    	    ecc = sel - 512;
	    if( !((ecc != v22) && (ecc < Pitch)) ) continue;
	    v111 = v22;
	    v22 = ecc;
    	    sel = -3;
        }
        switch( sel ){
            case 327:
                    if( v8 > 0 ){
                        sel = -4;
                        v8 = 0;
                    }
                    break;
            case 328:
                    if( v22 <= 0 ){
                        if( v8 > 0 ){
                            sel = -4;
                            v8--;
                        }
                    } else {
                        sel = -3;
                        v111 = v22--;
                    }
                    break;
            case 329:
                    if( v8 > 0 ){
                        v8 -= zgz;
                        if( v8 < 0 ) v8 = 0;
                        sel = -4;
                    }
                    break;
            case 335:
                    if( v8 < v112 ){
                        sel = -4;
                        v8 = v112;
                    }
                    break;
            case 336:
                    if( v22 >= (zgz - 1) || v22 >= Pitch - 1 ){
                        if( v8 + zgz < Pitch ){
                            sel = -4;
                    	    v8++;
                        }
                    } else {
                        sel = -3;
                        v111 = v22++;
                    }
                    break;
            case 337:
                    if( v8 < v112 ){
                        v8 += zgz;
                        if( v8 > v112 ) v8 = v112;
                        sel = -4;
                    }
                    break;
            default:
                    if( (Pitch <= zgz) || ( !(sel >= 'a' && sel <= 'z') && !(sel >= 'A' && sel <= 'Z') ) ) break;
                    v46 = TextBoxSearchLine( sel, FileList, Pitch );
                    sel = v46;
                    if( sel == -1 ) break;
                    v8 = ( sel > v112 ) ? v112 : sel;
                    v22 = v46 - v8;
                    sel = -4;
                    break;
        }
	switch( sel ){
            case -4:
            	    ScrFillSolid( p, a2, a3 - arg4, v14, COLOR( 0, 0, gWinSys.Colors[ 0 ] ) );
            	    WinDrawTextList( win, &FileList[ v8 ],( Pitch >= zgz ) ? zgz : Pitch, a2, Ypos, arg4, a7 | 0x2000000 );
            	    ScrEnlight( &p[ v14 * v22 * gFont.ChrHeight() ], a2, gFont.ChrHeight(), v14 );
            	    if( Pitch > zgz ){
                	ScrFillSolid( &(&Window->Surface[ v14 - 21 ])[ v14 * posY ], Width + 1, Width + 1, v14, COLOR( 0, 0, gWinSys.Colors[ 0 ] ) );
                	gFont.ChrHeight();
                	posY = v14 + ecc + gFont.ChrHeight() + 7;
                	ScrShadowRectangle( Window->Surface, v14, v14 - 21, posY, Width + v14 - 21, Width + posY, COLOR( 0, 0, gWinSys.Colors[ 1 ] ), COLOR( 0, 0, gWinSys.Colors[ 2 ] ) );
                	ScrEnlight( &(&Window->Surface[ v14 - 21 ])[ v14 * posY ], Width, Width, v14 );
            	    }
            	    WinRedraw( Window, &Window->Geometry, 0 );
		    break;
	    case -3:
        	    Area.lt = Ypos + (&Window->Geometry)->lt;
        	    Area.rt = Area.lt + a2;
        	    if( v111 != -1 ){
            		Area.tp = v111 * gFont.ChrHeight() + arg4 + v14;
            		Area.bm = Area.tp + gFont.ChrHeight();
            		ScrFillSolid( &p[ v14 * ecc * gFont.ChrHeight() ], a2, gFont.ChrHeight(), v14, COLOR( 0, 0, gWinSys.Colors[ 0 ] ) );            		
            		gFont.Print( &p[ v14 * v111 * gFont.ChrHeight() ], FileList[ v8 + v111 ], v14, v14, 
            		    ( a7 & 0xff00 ) ? ( (a7 & 0xffff0000) | COLOR( 0, 0, gPalBase[ a7 + 43 ].b ) ) : a7 
            		);
            		WinRedraw( Window, &Area, 0 );
        	    }
        	    if( v22 != -1 ){
            		Area.tp = v22 * gFont.ChrHeight() + v14;
            		Area.bm = Area.tp + gFont.ChrHeight();
            		ScrEnlight( &p[ v14 * v22 * gFont.ChrHeight() ], a2, gFont.ChrHeight(), v14 );
            		WinRedraw( Window, &Area, 0 );
        	    }
        	    break;
	}
    }
    WinClose( win );
    return v102;
}


int TextBoxDialogEdit1( char *text, int MaxChars, char *Label, int Xpos, int Ypos )
{
    int win, w, a, h;
    char *Surface;

    if( !gWinSys.Init ) return -1;
    w = gFont.LineWidth( Label ) + 12;
    a = MaxChars + 1;
    if( w < a * gFont.MaxWidth() ) w = gFont.MaxWidth() * a;

    w += 16;
    if( w < 160 ) w = 160;

    h = 5 * gFont.ChrHeight() + 16;
    if( ( win = WinCreateWindow( Xpos, Ypos, w, h, 256, 20 ) ) == -1 ) return -1;

    WinDrawFrame( win );
    Surface = WinGetWindow( win )->Surface;
    ScrFillSolid( WIN_XY( 14, gFont.ChrHeight() + 14, w, Surface ), w - 2*14, gFont.ChrHeight() + 2, w, WIN_PALCOLOR_ALPHA );
    gFont.Print( WIN_XY( 8, 8, w, Surface ), Label, w, w, WIN_FRCOL_D );
    ScrShadowRectangle( Surface, w, 14, gFont.ChrHeight() + 14, w - 14, 2 * gFont.ChrHeight() + 16, WIN_FRCOL_B, WIN_FRCOL_A );
    WinAddButton( win, (w / 2) - 72, h - 8 - gFont.ChrHeight() - 6, -1, -1, -1, 13, "Done",  0 );
    WinAddButton( win, (w / 2) + 8,  h - 8 - gFont.ChrHeight() - 6, -1, -1, -1, 27, "Cancel", 0 );
    WinUpdate( win );
    TextBoxDialogRun( win, text, MaxChars, 16, gFont.ChrHeight() + 16, WIN_FRCOL_C, WIN_PALCOLOR_ALPHA );
    WinClose( win );
    return 0;
}

int TextBoxUnk10( char *Label, char **TextList, int Lines, int Ypos, int Yposa, int Color, char *Title )
{
    char *Surface;
    int WinId,h,w,a,s,err,Height;

    err = 0;
    if( !gWinSys.Init ) return -1;
    h = gFont.ChrHeight();
    Height = ( Lines + 3 ) * h + 28;
    w = TextBoxListWidth( TextList, Lines ) + 16;
    a = gFont.LineWidth( Label ) + 16;
    if( a > w ) w = a;
    w += 16;
    a = gVidMainGeo.rt - gVidMainGeo.lt + 1;
    if( w + Ypos > a ) w = a - Ypos;    
    if( (WinId = WinCreateWindow( Ypos, Yposa, w, Height, 256, 20 ) ) == -1 ) return -1;
    WinDrawFrame( WinId );
    Surface = WinGetWindow( WinId )->Surface;
    gFont.Print( &Surface[8 * w + 8], Label, w, w, gPalColorCubeRGB[0][0][gWinSys.Colors[3]] | 0x10000 );
    s = w * (gFont.ChrHeight() + 8) + 8 + 8 * w + 8;
    ScrFillSolid( &Surface[ s - 4 + -3 * w ], w - 24, Lines * gFont.ChrHeight() + 7, w, gPalColorCubeRGB[0][0][ gWinSys.Colors[0] ] );
    WinDrawTextList( WinId, TextList, Lines, 0, 16, gFont.ChrHeight() + 16, Color );
    ScrShadowRectangle( &Surface[ s - 4 + -3 * w ], w, 0, 0, w - 24, Lines * gFont.ChrHeight() + 7, gPalColorCubeRGB[0][0][gWinSys.Colors[2]], gPalColorCubeRGB[0][0][gWinSys.Colors[1]] );
    h = Height - 8;
    w /= 2;
    if( Title ){
        WinAddButton( WinId, w - gFont.LineWidth( Title ) - 32, h - gFont.ChrHeight() - 6, -1, -1, -1, 1024, Title, 0 );
	WinAddButton( WinId, w + 16, h - gFont.ChrHeight() - 6, -1, -1, -1, 27, "Done", 0 );
    } else {
	WinAddButton( WinId, w - 32, h - gFont.ChrHeight() - 6, -1, -1, -1, 27, "Done", 0 );
    }
    WinUpdate( WinId );
    while( 1 ){
        a = InpUpdate();
        if( a == KEY_ESC ) break;
        if( a == 1024 ){
            err = 1;
            break;
        }
    }
    WinClose( WinId );
    return err;
}

int TextBoxUnk11( char *a1, int a2, int a3, int a4 )
{
    int v8, win, Height;
    char *Surface;

    if( !gWinSys.Init ) return -1;
    Height = 3 * gFont.ChrHeight() + 16;
    v8 = gFont.LineWidth( a1 ) + 16;
    if( v8 < 144 ) v8 = 144;
    v8 += 16;
    if( (win = WinCreateWindow( a2, a3, v8, Height, 256, 20 ) ) == -1 ) return -1;
    WinDrawFrame( win );
    Surface = WinGetWindow( win )->Surface;
    gFont.Print( &Surface[ 8 * v8 + 16 ], a1, v8, v8, ( a4 & 0xff00 ) ? (gPalColorCubeRGB[0][0][ gPalBase[ a4 + 43 ].r ] | (a4 & 0xffff0000)) : (a4 >> 8) );
    v8 = v8 / 2;
    WinAddButton( win, v8 - 64, Height - 8 - gFont.ChrHeight() - 6, -1, -1, -1, 121, "Yes", 0 );
    WinAddButton( win, v8 + 16, Height - 8 - gFont.ChrHeight() - 6, -1, -1, -1, 110, "No",  0 );
    WinUpdate( win );
    do
        v8 = InpUpdate();
    while( v8 != KEY_ESC && v8 != 'n' && v8 != 'N' && v8 != '\n' && v8 != ' ' && v8 != 'y' && v8 != 'Y' );
    WinClose( win );
    return 1;    
}

int TextBoxUnk12( char *str, int a2, int a3, int Color )
{
    int w,pitch,win,Height;
    char *Surface;

    if( !gWinSys.Init ) return -1;
    Height = 3 * gFont.ChrHeight() + 16;
    w = gFont.LineWidth( str ) + 16;
    if( w < 80 ) w = 80;
    pitch = w + 16;
    if( (win = WinCreateWindow( a2, a3, pitch, Height, 256, 20 )) == -1 ) return -1;
    WinDrawFrame( win );
    Surface = WinGetWindow( win )->Surface;
    if( Color & 0xff00 ){
	gFont.Print( &Surface[8 * pitch + 16], str, pitch, pitch, (Color & 0xffff0000) | gPalColorCubeRGB[0][0][ gPalBase[ Color + 43 ].r ] );
    } else {
	gFont.Print( &Surface[8 * pitch + 16], str, pitch, pitch, Color );
    }
    WinAddButton( win, pitch / 2 - 32, Height - gFont.ChrHeight() - 8, -1, -1, -1, 27, "Done", 0 );
    WinUpdate( win );
    while( InpUpdate() != KEY_ESC );
    WinClose( win );
    return 0;
}

int TextBoxUnk13( char **pText, int a2, int a3, int a4, int FrameColor )
{
    VidRect_t Area;
    int WinId;

    if( !gWinSys.Init ) return -1;
    if( (WinId = TextBoxCreate( pText, a2, a3, a4, FrameColor, COLOR( 0, 0, gWinSys.Colors[0] ), &Area ) ) == -1 ) return -1;
    return TextBoxUnk01( WinId, &Area, pText, a2, FrameColor, COLOR( 0, 0, gWinSys.Colors[0] ), 0, -1 );
    
}

int TextBoxCreate( char **TextList, int Lines, int Xpos, int Ypos, int FrameColor, int BgColor, VidRect_t *Area )
{
    int WinId, w, h;

    h = Lines * gFont.ChrHeight() + 2*TEXTBOX_MARGIN_V;
    w = TextBoxListWidth( TextList, Lines ) + 2*TEXTBOX_MARGIN_H;
    if( h < 2 || w < 2 ) return -1;
    
    if( (WinId = WinCreateWindow( Xpos, Ypos, w, h, BgColor, 0x14 )) == -1 ) return -1;
    WinDrawTextList( WinId, TextList, Lines, w - 4, TEXTBOX_MARGIN_H, TEXTBOX_MARGIN_V, FrameColor );
    WinDrawRectangle( WinId, 0, 0, w - 1, h - 1, WIN_FRCOL_ALPHA );
    WinDrawRectangle( WinId, 1, 1, w - 2, h - 2, FrameColor );
    WinUpdate( WinId );
    WinGetRect( WinId, Area );
    
    return WinId;
}

int TextBoxUnk01( int WinId, VidRect_t *Area, char **lines, int text, int FrameColor, int CanvasColor, WinDlg_t *dlg, int Idx )
{
    unsigned int Height,Pitch,Field;
    int v8,h,w,Width,v22,v29,v35,tp,lt,ColorBg,ColorFg,v50,sel;
    char *v18,*v26,*v42,*p,*Surface, *v24;
    VidRect_t *v56;

    v56 = Area;
    v8 = -1;
    v50 = -1;
    if( dlg ){
        Surface = WinGetWindow( dlg->WinId )->Surface;
        lt = dlg->box[ Idx ].Rect.lt;
        tp = dlg->box[ Idx ].Rect.tp;
        h = dlg->box[ Idx ].Rect.bm - tp;
        w = dlg->box[ Idx ].Rect.rt - lt;
        Width = WinGetWidth( dlg->WinId );
        w++;
        h++;
        ColorFg = dlg->ColorA;
        if(dlg->ColorA & 0xff00 ) ColorFg = (dlg->ColorA & 0xffff0000) | gPalColorCubeRGB[0][0][ gPalBase[ dlg->ColorA + 43 ].r ];        
        ColorBg = dlg->ColorB;
        if( dlg->ColorB & 0xff00 ) ColorBg = (dlg->ColorB & 0xffff0000) | gPalColorCubeRGB[0][0][ gPalBase[ dlg->ColorB + 43 ].r ];
        ScrColorXchg( &Surface[ Width * tp + lt ], w, h, Width, ColorFg, ColorBg );
        WinAreaUpdate( dlg->WinId, &dlg->box[ Idx ].Rect );
    }

    v18 = WinGetWindow( WinId )->Surface;
    Pitch = v56->rt - Area->lt + 1;
    MseGetCursorPosition( &lt, &tp );
    while( ( sel = InpUpdate() ) == -1 ){
        MseGetCursorPosition( &ColorFg, &ColorBg );
        if( lt - 4 > ColorFg ) break;
        if( !((lt + 4) >= ColorFg && (tp - 4) <= ColorBg && (tp + 4) >= ColorBg)) break;
    }

    Field = Pitch - 4;
    Height = Area->bm - Area->tp + 1 - 16;
    v42 = v18 + 2;
    while( 1 ){
        MseGetCursorPosition( &ColorFg, &ColorBg );
        if( sel == -2 && dlg && MseCursorCenterInArea( dlg->Area.lt, dlg->Area.tp, dlg->Area.rt, dlg->Area.bm ) ){            
            for( v22 = 0; v22 < dlg->Cnt; v22++ ){
                if( MseCursorCenterInArea( dlg->box[ v22 ].Rect.lt, dlg->box[ v22 ].Rect.tp, dlg->box[ v22 ].Rect.rt, dlg->box[ v22 ].Rect.bm ) ) break;
            }
            if( (v22 < dlg->Cnt) && (v22 != Idx) ){ v8 = -2 - v22; break; }
        }
        if( ((MseGetButtons() & 0x10) || (MseGetButtons() & 0x01)) && !(MseGetButtons() & 0x04) ){
            if( MseCursorCenterInArea( v56->lt, v56->tp + 8, v56->rt, v56->bm - 9 ) ) break;
            v8 = -1; 
            break;
        }
        if( sel == KEY_ESC ){ v8 = -1; break; }
        if( sel == 13 ) break;
        if( sel == 331 ){
    	    if( dlg && Idx > 0 ){ v8 = -2 - (Idx - 1); break; }
        } else {
	    if( sel == 333 ){
		if( dlg && ( dlg->Cnt - 1 > Idx ) ){ v8 = -2 - (Idx + 1); break;}
	    } else {
    		v24 = lines[ v8 ];
    		if( sel == 328 ){
    		    while( v8 > 0 ){
            		v8--;
            		if( *(--v24) ) break;
    		    }
    		    sel = -3;
    		} else {
    		    if( sel == 336 ){
    			while( v8 < text - 1 ){
        		    v8++;
            		    v26 = v24;
            		    v24++;
            		    if( *v26 ) break;            	
    			}
    			sel = -3;
    		    } else {
    			if( ( ColorFg != lt || ColorBg != tp ) && MseCursorCenterInArea( v56->lt, v56->tp + 8, v56->rt, v56->bm - 9 ) ) {
    			    v29 = (ColorBg - v56->tp - 8) / gFont.ChrHeight();
    			    sel = v29;
    			    if( v29 != v50 ){
            			v8 = ( lines[ v29 ] ) ? sel : -1;
            			sel = -3;
    			    }
    			    lt = ColorFg;
    			    tp = ColorBg;
    			}
    		    }
    		}
    	    }
    	}
        if( sel == -3 ){
            WinDrawFilledRect( WinId, 2, 8, Field, Height, CanvasColor );
            WinDrawTextList( WinId, lines, text, Field, 2, 8, FrameColor );
            if( v8 != -1 ) ScrEnlight( &v42[ Pitch * (v8 * gFont.ChrHeight() + 8) ], Field, gFont.ChrHeight(), Pitch );
            WinUpdate( WinId );
        }
        sel = InpUpdate();
    }
    WinClose( WinId );
    if( !dlg ) return v8;
    p = WinGetWindow( dlg->WinId )->Surface;
    lt = dlg->box[ Idx ].Rect.lt;
    tp = dlg->box[ Idx ].Rect.tp;
    v35 = WinGetWidth( dlg->WinId );
    ColorFg = ( dlg->ColorA & 0xff00 ) ? (dlg->ColorA & 0xffff0000 ) | gPalColorCubeRGB[0][0][ gPalBase[ dlg->ColorA + 43 ].r ]: dlg->ColorA;
    ColorBg = ( dlg->ColorB & 0xff00 ) ? (dlg->ColorB & 0xffff0000 ) | COLOR( 0, 0, gPalBase[ dlg->ColorB + 43 ].r ) : dlg->ColorB;
    ScrColorXchg( &p[ v35 * tp + lt ], dlg->box[ Idx ].Rect.rt - lt + 1, dlg->box[ Idx ].Rect.bm - tp + 1, v35, ColorFg, ColorBg );
    WinAreaUpdate( dlg->WinId, &dlg->box[ Idx ].Rect );
    return v8;
}

int TextBoxUnk03( char *a1 )
{
    Window_t *Window;
    int v9, v2;
    char *Surface;

    if( !gWinSys.Init ) return -1;
    v2 = gFont.ChrHeight();
    if( gTextBoxWinId == -1 ){
        gTextBoxWinId = WinCreateWindow( 80, 80, 300, 192, 256, 4 );
        if( gTextBoxWinId == -1 ) return -1;
        WinDrawFrame( gTextBoxWinId );
        Surface = WinGetWindow( gTextBoxWinId )->Surface;
        WinDrawFilledRect( gTextBoxWinId, 8, 8, 284, v2, 257 );
        WinDrawText( gTextBoxWinId, a1, 0, (300 - gFont.LineWidth( a1 )) / 2, 8, 0x2000104 );
        ScrShadowRectangle( Surface, 300, 8, 8, 291, v2 + 8, COLOR( 0, 0, gWinSys.Colors[2] ), COLOR( 0, 0, gWinSys.Colors[1] ) );
        WinDrawFilledRect( gTextBoxWinId, 9, 26, 282, 135, 257 );
        ScrShadowRectangle( Surface, 300, 8, 25, 291, v2 + 145, COLOR( 0, 0, gWinSys.Colors[2] ), COLOR( 0, 0, gWinSys.Colors[1] ) );
        gTextBoxXpos = 9;
        gTextBoxYpos = 26;
        WinSetButtonHandler( WinAddButton( gTextBoxWinId, (300 - gFont.LineWidth( "Close" )) / 2, 184 - v2 - 6, -1, -1, -1, -1, "Close", 0 ), 0, 0, 0, TextBoxUnk04 );
        WinCreateButton( gTextBoxWinId, 8, 8, 284, v2, -1, -1, -1, -1, 0, 0, 0, 16 );
    }
    a1[ 1 ] = '\0';
    if( a1[ 0 ] ){
        do{
            v9 = gFont.ChrWidth( *a1 );
            if( *a1 == '\n' || gTextBoxXpos + v9 > 291 ){
                gTextBoxXpos = 9;
                gTextBoxYpos += v2;
            }
            while( 160 - gTextBoxYpos < v2 ){
                Window = WinGetWindow( gTextBoxWinId );
                ScrCopy( &Window->Surface[300 * v2 + 7809], 282, 134 - v2 - 1, 300, Window->Surface + 7809, 300 );
                gTextBoxYpos -= v2;
                WinDrawFilledRect( gTextBoxWinId, 9, gTextBoxYpos, 282, v2, 257 );
            }
            if( a1[ 0 ] != '\n' ){
//                tmp = *a1;
                WinDrawText( gTextBoxWinId, a1 + 1, 0, gTextBoxXpos, gTextBoxYpos, 0x2000104 );
                gTextBoxXpos += v9 + gFont.Distance();
            }
        }while( *++a1 );
    }
    WinUpdate( gTextBoxWinId );
    return 0;
}

void TextBoxUnk04()
{
    WinClose( gTextBoxWinId );
    gTextBoxWinId = -1;
}

int TextBoxNew( int WinId, int x0, int y0, int w, int h, int ColorFg, int ColorBg )
{
    Window_t *win;
    int x1, y1;

    win = WinGetWindow( WinId );
    if( !gWinSys.Init ) return -1;
    if( !win ) return -1;
    if( win->Dialog ) return -1;
    x1 = x0 + w;
    y1 = y0 + h;
    if( x1 > win->Width || y1 > win->Height ) return -1;
    if( !( win->Dialog = Malloc( sizeof( WinDlg_t ) ) ) ) return -1;
    win->Dialog->WinId = WinId;
    win->Dialog->Area.lt = x0;
    win->Dialog->Area.tp = y0;
    win->Dialog->Area.rt = x1 - 1;
    win->Dialog->Area.bm = y1 - 1;
    win->Dialog->Cnt = 0;
    win->Dialog->ColorA = ColorFg;
    win->Dialog->ColorB = ColorBg;
    WinDrawFilledRect( WinId, x0, y0, w, h, ColorBg );
    WinDrawRectangle( WinId, x0, y0, x1 - 1, y1 - 1, ColorFg );
    return 0;
}

int TextBoxAddMsg( int WinId, int x, char *msg, int key, int Lines, char **TextList, int FgColor, int BgColor )
{
    Window_t *win;
    WinDlg_t *Dialog;
    int Cnt, Ypos, Xpos;
    
    if( !gWinSys.Init ) return -1;
    if( !(win = WinGetWindow( WinId )) ) return -1;
    Dialog = win->Dialog;
    if( !Dialog || Dialog->Cnt == 15 ) return -1;
    Cnt = Dialog->Cnt;
    Xpos = Dialog->Area.lt + x;
    Ypos = (win->Dialog->Area.tp + win->Dialog->Area.bm - gFont.ChrHeight()) / 2;
    if( (WinCreateButton( WinId, Xpos, Ypos, gFont.LineWidth( msg ), gFont.ChrHeight(), -1, -1, key, -1, 0, 0, 0, 0)) == -1 ) return -1;
    WinDrawText( WinId, msg, 0, Xpos, Ypos, win->Dialog->ColorA | 0x2000000 );
    win->Dialog->box[ Cnt ].Rect.lt = Xpos;
    win->Dialog->box[ Cnt ].Rect.tp = Ypos;
    win->Dialog->box[ Cnt ].Rect.rt = gFont.LineWidth( msg ) + Xpos - 1;
    win->Dialog->box[ Cnt ].Rect.bm = gFont.ChrHeight() + Ypos - 1;
    win->Dialog->box[ Cnt ].key = key;
    win->Dialog->box[ Cnt ].Lines = Lines;
    win->Dialog->box[ Cnt ].TextList = TextList;
    win->Dialog->box[ Cnt ].FgColor = FgColor;
    win->Dialog->box[ Cnt ].BgColor = BgColor;
    win->Dialog->Cnt++;
    return 0;
}

void TextBoxDelete( int WinId )
{
    Window_t *Window;
    WinDlg_t *dlg;

    Window = WinGetWindow( WinId );
    if( !gWinSys.Init || !Window ) return;
    dlg = Window->Dialog;
    if( !Window->Dialog ) return;
    WinDrawFilledRect( WinId, dlg->Area.lt, dlg->Area.tp, dlg->Area.rt - dlg->Area.lt + 1, dlg->Area.bm - dlg->Area.tp + 1, Window->FontColor );
    Free( Window->Dialog );
    Window->Dialog = NULL;
}

int TextBoxRun( WinDlg_t *tb, int TextId )
{    
    VidRect_t Area;
    WinDlgBox_t *txp, *tx;
    int r, ws, key;

    if( gTextBoxGuard ) return -1;
    gTextBoxGuard = tb;
    tx = tb->box;
    do{
        txp = &tx[ TextId ];
        r = TextBoxCreate( txp->TextList, txp->Lines, txp->Rect.lt, tb->Area.bm + 1, txp->FgColor, txp->BgColor, &Area );
        if( r == -1 ){ gTextBoxGuard = 0; return -1; }
        ws = TextBoxUnk01( r, &Area, txp->TextList, txp->Lines, txp->FgColor, txp->BgColor, tb, TextId );
        key = ws;
        if( ws < -1 ) TextId = -2 - ws;
    }while( ws < -1 );
    if( ws != -1 ){
        InpFlushBuffer();
        InpPushBuffer( ws );
        key = tb->box[ TextId ].key;
    }
    gTextBoxGuard = NULL;
    return key;
}

int TextBoxSearchLine( int chr, char **TextList, int Lines )
{
    int i, s;

    if( chr >= 'A' && chr <= 'Z' ) chr +=' ';
    i = 0;
    for( i = 0; i< Lines; i++, TextList++ ){
        s = **TextList;
        if( s == chr || s == chr - ' ' ) return i;
    }
    return -1;        
}

int TextBoxListWidth( char **TextList, int Lines )
{
    int max, w, i;

    max = 0;
    for( i = 0; i < Lines; i++, TextList++ ){
        w = gFont.LineWidth( *TextList );
        if( w > max ) max = w;        
    }
    return max;
}

int TextBoxDialogRun( int WinId, char *s, int MaxLen, int PosX, int PosY, int ColorFg, int ColorBg )
{
    VidRect_t Area;
    Window_t *Window;
    int StrPos,Width,sel,flg;
    unsigned int w, Height;
    char *surf;

    flg = 1;
    Window = WinGetWindow( WinId );
    surf = &Window->Surface[ Window->Width * PosY + PosX ];
    Height = gFont.ChrHeight();
    StrPos = strlen( s );
    s[ StrPos + 0 ] = '_';
    s[ StrPos + 1 ] = '\0';
    Width = gFont.LineWidth( s );
    ScrFillSolid( surf, Width, Height, Window->Width, ColorBg );
    gFont.Print( surf, s, Width, Window->Width, ColorFg );
    Area.lt = PosX + Window->Geometry.lt;
    Area.tp = PosY + Window->Geometry.tp;
    Area.rt = Area.lt + Width;
    Area.bm = Area.tp + Height;
    WinRedraw( Window, &Area, 0 );
    for( ;StrPos <= MaxLen; StrPos++ ){
        sel = InpUpdate();
    	    if( sel == -1 ){
    	StrPos--;
        } else {
    	    if( sel == KEY_ESC ){ s[ StrPos + 0 ] = '\0'; return -1; }
    	    if( sel == KEY_BS ){
    		if( StrPos > 0 ){
    		    w = gFont.LineWidth( s );
    		    if( flg ){
        		ScrFillSolid( surf, w, Height, Window->Width, ColorBg );
        		Area.lt = PosX + Window->Geometry.lt;
        		Area.tp = PosY + Window->Geometry.tp;
        		Area.rt = Area.lt + w;
        		Area.bm = Area.tp + Height;
        		WinRedraw( Window, &Area, 0 );
        		s[ 0 ] = '_';
        		s[ 1 ] = '\0';
        		StrPos = 1;
    		    } else {
        		s[ StrPos + 0 ] = ' ';
        		s[ StrPos - 1 ] = '_';
    		    }
    		    ScrFillSolid( surf, w, Height, Window->Width, ColorBg );
    		    gFont.Print( surf, s, w, Window->Width, ColorFg );
    		    Area.lt = PosX + Window->Geometry.lt;
    		    Area.tp = PosY + Window->Geometry.tp;
    		    Area.rt = Area.lt + w;
    		    Area.bm = Area.tp + Height;
    		    WinRedraw( Window, &Area, 0 );
    		    s[ StrPos ] = '\0';
    		    StrPos -= 2;
    		    flg = 0;
    		} else {
    		    StrPos--;
    		}
    	    } else {
    		if( sel == KEY_ENTER ) break;
    		if( StrPos == MaxLen ){
        	    StrPos = MaxLen - 1;
    		} else {
        	    if( sel <= 0 || sel >= 256 ){
    			StrPos--;
        	    } else {
        		s[ StrPos + 0 ] = sel;
        		s[ StrPos + 1 ] = '_';
        		s[ StrPos + 2 ] = '\0';
        		w = gFont.LineWidth( s );
        		ScrFillSolid( surf, w, Height, Window->Width, ColorBg );
        		gFont.Print( surf, s, w, Window->Width, ColorFg );
        		Area.lt = PosX + Window->Geometry.lt;
        		Area.tp = PosY + Window->Geometry.tp;
        		Area.rt = Area.lt + w;
        		Area.bm = Area.tp + Height;
        		WinRedraw( Window, &Area, 0 );
        		flg = 0;
        	    }
    		}
    	    }
        }        
    }
    s[ StrPos ] = '\0';
    return 0;
}

int TextBoxInput( int *Num, int min, int max, unsigned char a4, char *Text, int Xpos, int Ypos )
{
    char *Surface, *stmp;
    int w,win,err,Height,Width,Field,ebx0,OldValue,posY,posX;

    if( !gWinSys.Init || max < min ) return -1;
    OldValue = *Num;
    if( OldValue < min || OldValue > max ) *Num = ( max >= *Num ) ? min : max;    
    ebx0 = TextBoxUnk09( min, max );
    if( ebx0 == -1 ) return -1;
    w = gFont.MaxWidth();
    Field = w * ebx0;
    w = gFont.LineWidth( Text );
    if( w < ebx0 ) w = ebx0;
    w += 16;
    if( w < 160 ) w = 160;
    Height = 5 * gFont.ChrHeight() + 16;
    posX = (w - Field) / 2;
    posY = gFont.ChrHeight() + 14;
    Width = gFont.ChrHeight() + 2;    
    if( (win = WinCreateWindow( Xpos, Ypos, w, Height, 256, 20 ) ) == -1 ) return -1;
    WinDrawFrame( win );
    WinDrawFilledRect( win, posX, posY, Field, Width, 257 );
    WinDrawText( win, Text, w - 16, 8, 8, 261 );
    Surface = WinGetSurface( win );
    ScrShadowRectangle( Surface, w, posX - 2, posY - 2, Field + posX + 1, Width + posY - 1, COLOR( 0, 0, gWinSys.Colors[2] ),COLOR( 0, 0, gWinSys.Colors[1] ) );
    WinAddButton( win, w / 2 - 72, Height - 8 - gFont.ChrHeight() - 6, -1, -1, -1, 13, "Done", 0 );
    WinAddButton( win, w / 2 + 16, Height - 8 - gFont.ChrHeight() - 6, -1, -1, -1, 27, "Cancel", 0 );    
    if( !( stmp = Malloc( 80 ) ) ) return -1;
    sprintf( stmp, "Please enter a number between %d and %d.", min, max );
    WinUpdate( win );
    while( 1 ){
        err = TextBoxEditNumber( win, Num, ebx0, a4, min < 0, posX, posY );
        if( *Num >= min && *Num <= max ) break;
        if( err == -1 ) break;
        TextBoxUnk12( stmp, Xpos - 70, Ypos + 100, 262 );
        *Num = OldValue;
    }
    Free( stmp );
    WinClose( win );
    return err;
}

int TextBoxUnk09( int min, int max )
{
    char *s;
    int l1, l2;

    if( !( s = Malloc( 17 ) ) ) return -1;
    sprintf( s, "%d", min );
    l1 = strlen( s );
    sprintf( s , "%d", max );
    l2 = strlen( s );
    Free( s );
    return (( l2 > l1 ) ? l2 : l1) + 1;
}

int TextBoxEditNumber( int WinId, int *pValue, int Digits, int Clear, char a5, int posX, int posY )
{
    Window_t *Window;
    VidRect_t Area;
    unsigned int Height;
    int pos,enter,sel,tmp,OldVal,full,Width,TextWidth;
    char *n;
    
    TextWidth = 1;
    if( !( Window = WinGetWindow( WinId ) ) ) return -1;
    OldVal = *pValue;    
    Height = gFont.ChrHeight();
    WinGetSurface( WinId );
    WinGetWidth( WinId );
    Width = gFont.MaxWidth() * Digits;
    if( !(n = Malloc( Digits + 1 )) )return -1;
    if( Clear )
        n[ 0 ] = '\0';
    else
        sprintf( n, "%d", *pValue );
    pos = strlen( n );
    n[ pos + 0 ] = '_';
    n[ pos + 1 ] = '\0';

    WinDrawText( WinId, n, Width, posX, posY, 260 );
    Area.lt = posX;
    Area.tp = posY;
    Area.rt = Width + posX;
    Area.bm = posY + Height;
    WinAreaUpdate( WinId, &Area );
    enter = 0;
    if( pos <= Digits ){
        full = Digits - 1;
        do{
            sel = InpUpdate();
            switch( sel ){
        	case KEY_ESC: *pValue = OldVal; Free( n ); return -1;
        	case 13: enter = 1; continue;
        	case 331: 
            	    if( pos > 0 ){
            		n[ pos - 1 ] = '_';
            		n[ pos + 0 ] = '\0';
            		WinDrawFilledRect( WinId, posX, posY, gFont.LineWidth( n ), Height, 257 );
            		pos--;
            		WinDrawText( WinId, n, Width, posX, posY, 260 );
            	        WinAreaUpdate( WinId, &Area );
        		TextWidth = 0;
            	    }
            	    pos--;   
            	    continue;
                case KEY_BS:
        	    if( pos > 0 ){
            		tmp = gFont.LineWidth( n );
            		if( TextWidth ){
                	    n[ 1 ] = '\0';
                	    pos = 1;
            		    n[ 0 ] = '_';
            		} else {
                	    n[ pos + 0 ] = '\0';
                	    n[ pos - 1 ] = '_';
            		}
            		WinDrawFilledRect( WinId, posX, posY, tmp, Height, 257 );
            		WinDrawText( WinId, n, Width, posX, posY, 260 );
            		WinAreaUpdate( WinId, &Area );
            	        pos--;
            		TextWidth = 0;
        	    }
        	    pos--;
        	    continue;
                case '0' ... '9':
    		    n[ pos + 0 ] = sel;
    		    n[ pos + 1 ] = '_';
    		    n[ pos + 2 ] = '\0';
    		    WinDrawFilledRect( WinId, posX, posY, gFont.LineWidth( n ), Height, 257 );
    		    WinDrawText( WinId, n, Width, posX, posY, 260 );
    		    WinAreaUpdate( WinId, &Area );
    		    TextWidth = 0;
    		    continue;
            }
    	    if( sel <= -1 ){
        	if( pos > Digits ) break;
    	    } else {
		if( (pos != full) && ( sel == '-' ) && ( pos || !a5 ) ) pos--;
    		pos--; 
    	    }
        }while( ++pos <= Digits && !enter );
    }
    n[ pos ] = '\0';
    *pValue = strtol( n, NULL, 10 );
    Free( n );
    return 0;
}

