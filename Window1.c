#include "FrameWork.h"

extern void SysQuitDlg(); // temp

typedef struct
{
  int Width;
  int Height;
} Resolutions_t;

int gWinUnk203[ 16 ];
int gWinUnk209[ 64 * 256 ];
Win02_t gWinList[ 16 ];
int (**gWinUnk100)( int );
int (*gWinUnk109)( int, char *, int * );
int (*gWinUnk202)( int, char * );
int gWinResortWidth;
int (*gWinUnk110)( int, char *, char *, int, int );
int (*gWinUnk108)( int id, char *);
int gWinResortHeight;
int gWinColorR_HL;
int gWinTextFont;
int (*gWinUnk115)( int, char *, int * );
int (*gWinUnk116)( int, char *, int * );
int (*gWinUnk117)( int, char *, int * );
int gWinColorG;
int gWinColorB;
int gWinTextFlags;
int gWinColorR;
int gWinColorG_HL;
int gWinColorB_HL;
int gWinUnk200 = 250;
int gWinUnk104 = 1;
int gWinUnk201 = -1;
int gWinCurrent = -1;
static int ( *gWinInitVidModeSel[12])() = {
    VidInit320x200x8bpp, VidInit640x480x8bpp, VidInitDummy, VidInit320x400x8bpp,
    VidInitDummy, VidInit640x400x8bpp, VidInitDummy, VidInit800x600x8bpp,
    VidInitDummy, VidInit1024x768x8bpp, VidInitDummy, VidInit1280x1024x8bpp
};
static Resolutions_t gWinResolutionTable[12] = {
    { 320,  200  }, { 640,  480  }, { 640,  240  }, { 320,  400  }, 
    { 640,  200  }, { 640,  400  }, { 800,  300  }, { 800,  600  }, 
    { 1024, 384  }, { 1024, 768  }, { 1280, 512  }, { 1280, 1024 }
};
int gWinUnk101 = 0;
int gWinDlgCnt = -1;
int gWinUnk102 = 1;

/*************************************************************************/

int WinGetFont()
{
    return gWinTextFont;
}

int WinSetFont( int FontId )
{
    gWinTextFont = FontId;
    FontSet( FontId );
    return 1;
}

void WinSetTextColorDefault()
{
    gWinColorG = 31;
    gWinColorB = 31;
    gWinTextFlags = 0x2010000;
    gWinColorR = 31;
}

int WinGetTextFlags()
{
    return gWinTextFlags;
}

int WinSetTextFlags( int Flags )
{
    gWinTextFlags = Flags;
    return 1;
}

char WinGetTextColor()
{
    return gPalColorCubeRGB[ gWinColorR ][ gWinColorG ][ gWinColorB ];
}

char WinGetColorHL()
{
    return gPalColorCubeRGB[ gWinColorR_HL ][ gWinColorG_HL ][ gWinColorB_HL ];
}

int WinSetTextColor( float r, float g, float b )
{
    gWinColorG = (int)lround( 31.0 * g );
    gWinColorB = (int)lround( 31.0 * b );
    gWinColorR = (int)lround( 31.0 * r );
    return 1;
}

int WinSetTextColorHL( float r, float g, float b )
{
    gWinColorG_HL = (int)lround( 31.0 * g );
    gWinColorB_HL = (int)lround( 31.0 * b );
    gWinColorR_HL = (int)lround( 31.0 * r );
    return 1;
}

int WinUnk97( char *Str, int a2, int a3 )
{
    char *s, *p;
    int t, k, w;

    t = a3;
    s = Str;
    k = 1;
    while( *Str ){
        w = gFont.ChrWidth( *Str ) + t;
        if( *Str == '\n' || w > a2 ){
            while( w > a2 ) w -= gFont.ChrWidth( *Str );
            t = a3;
            k++;
            p = Str + 1;
            if( *Str == '\n' ){
        	Str = p;
            } else {
        	while ( Str != s && *Str != ' ' ) Str--;
        	if( Str == s ) Str = p;
            }
            while( *Str == ' ' ) Str++;
            s = Str;
        } else {
            Str++;
            t += gFont.Distance();;
        }
    }
    return k;
}

int WinUnk47( int Idx, int xpos, int ypos, int Flags )
{
    VidRect_t Area;
    Win02_t *win;
    int err,RegionCnt,i,i27,Region;

    win = &gWinList[ Idx ];
    err = 0;
    RegionCnt = win->RegionCnt;
    Region = win->Region;
    if( !RegionCnt ) return 0;
    if( Flags & 0x04 )  Flags |= 0x01;
    if( Flags & 0x08 )  Flags |= 0x02;
    for( i = 0; i < win->RegionCnt; i++ ){
        if( !win->Regions[ i ] ) continue;
        WinGetRect( win->WinId, &Area );
        if( !RegionUnk02( win->Regions[ i ], xpos - Area.lt, ypos - Area.tp ) ){
            if( win->Regions[ i ]->i26 == 1 ){
                if( win->Regions[ i ]->i31 ){
                    win->Regions[ i ]->i31( win->Regions[ i ], win->Regions[ i ]->i33, 3 );
                    err = 1;
                    if( !win->Regions || Region != win->Region ) return 1;
                }
                if( win->Regions[ i ]->i32 ){
                    win->Regions[ i ]->i32( win->Regions[ i ], win->Regions[ i ]->i34, 3 );
                    err = 1;
                    if( !win->Regions || Region != win->Region ) return 1;
                }
                if( win->Regions[ i ]->Script ){
                    if( win->Regions[ i ]->i22 ){
                        SciUnk11( win->Regions[ i ]->Script, win->Regions[ i ]->i22 );
                        err = 1;
                        if( !win->Regions || Region != win->Region ) return 1;
                    }
                }
            }
            if( (win->Regions[ i ]->i27 & 0x05) && !(Flags & 0x05) ) win->Regions[ i ]->i27 &= ~0x05;
            if( (win->Regions[ i ]->i27 & 0x0A) && !(Flags & 0x0A) ) win->Regions[ i ]->i27 &= ~0x0A;
            win->Regions[ i ]->i26 = 0;
            continue;
        }
        i27 = win->Regions[ i ]->i27;
        if( win->Regions[ i ]->i26 ){
            if( !(Flags & 0x0F) ) win->Regions[ i ]->i29 = 0;
        } else {
            win->Regions[ i ]->i29 = ( (i27 & 0x0F) || !(Flags & 0x0F) ) ? 0:1;
            win->Regions[ i ]->i28 = 1;
            win->Regions[ i ]->i26 = 1;
    	    err = 1;
        }
        if( win->Regions[ i ]->i29 ) continue;
        win->Regions[ i ]->i27 = Flags;
        if( (Flags & 0x01) && !(i27 & 0x01) ){
            if( win->Regions[ i ]->i31 ){
                win->Regions[ i ]->i31( win->Regions[ i ], win->Regions[ i ]->i33, 0 ); 
                if( !win->Regions || Region != win->Region ) return err; 
                err = 1;
            }
            if( win->Regions[ i ]->Script && win->Regions[ i ]->i19 ){
                SciUnk11( win->Regions[ i ]->Script, win->Regions[ i ]->i19 ); 
                if( !win->Regions || Region != win->Region ) return err;  
                err = 1;
            }
            win->Regions[ i ]->i25 = gWinUnk200 + TimerGetSysTime();
            continue;
	}
        if( (Flags & 0x04) && !(i27 & 0x04) ){
            if( TimerGetSysTime() < win->Regions[i]->i25 ){
                win->Regions[ i ]->i27 &= ~0x04;
            } else {
                if( win->Regions[ i ]->i31 ){
                    win->Regions[ i ]->i31( win->Regions[ i ], win->Regions[ i ]->i33, 4 ); 
                    err = 1;
                }
            }
            if( !win->Regions || Region != win->Region ) return err;
            continue;
	}
        if( !(Flags & 0x01) && (i27 & 0x01) ){
            if( win->Regions[ i ]->i31 ){
                win->Regions[ i ]->i31( win->Regions[ i ], win->Regions[ i ]->i33, 1 );                 
                if( !win->Regions || Region != win->Region ) return 1;
                err = 1; 
            }
            if( win->Regions[ i ]->Script && win->Regions[ i ]->i20 ){
                SciUnk11( win->Regions[ i ]->Script, win->Regions[ i ]->i20 ); 
                if( !win->Regions || Region != win->Region ) return 1;
                err = 1; 
            }
            continue;
	}
        if( (Flags & 0x02) && !(i27 & 0x02) ){
            if( win->Regions[ i ]->i32 ){
                win->Regions[ i ]->i32( win->Regions[ i ], win->Regions[ i ]->i34, 0 ); 
                if( !win->Regions || Region != win->Region ) return 1;
                err = 1;
            }
            if( win->Regions[ i ]->Script && win->Regions[ i ]->i23 ){
                SciUnk11( win->Regions[ i ]->Script, win->Regions[ i ]->i23 ); 
                if( !win->Regions || Region != win->Region ) return 1;
                err = 1; 
            }
            continue;
        }
        if( (Flags & 0x08) && !(i27 & 0x08) ){
            if( TimerGetSysTime() < win->Regions[i]->i25 ){
                win->Regions[ i ]->i27 &= ~0x08;
            } else {
                if( win->Regions[ i ]->i32 ){
                    win->Regions[ i ]->i32( win->Regions[ i ], win->Regions[ i ]->i34, 4 ); 
                    if( !win->Regions || Region != win->Region ) return 1;
                    err = 1; 
                }
            }
            continue;
        }
        if( !(Flags & 0x02) && (i27 & 0x02) ){
            if( win->Regions[ i ]->i32 ){
                win->Regions[ i ]->i32( win->Regions[ i ], win->Regions[ i ]->i34, 1 ); 
                if( !win->Regions || Region != win->Region ) return 1;
                err = 1; 
            }
            if( win->Regions[ i ]->Script && win->Regions[ i ]->i24 ){
                SciUnk11( win->Regions[ i ]->Script, win->Regions[ i ]->i24 ); 
                if( !win->Regions || Region != win->Region ) return 1;
                err = 1; 
            }
    	}
    }
    return err;
}

int WinMouseUpdate( int a1, int a2, int Butt, int a4 )
{
    Region_t **Regions, *p, *v9;
    int err, i, n;

    err = WinUnk47( a1, a2, Butt, a4 );    
    n = gWinList[ a1 ].Region;
    for( i = 0; i < gWinList[ a1 ].RegionCnt; i++ ){
        Regions = gWinList[ a1 + i ].Regions;
        if( Regions ){
            v9 = Regions[ i ];
            if( v9 ){
                if( v9->i28 ){
                    v9->i28 = 0;
                    p = gWinList[ a1 + i ].Regions[ i ];
                    err = 1;
                    if( p->i31 ){
                        p->i31( p, p->i33, 2 );
                        if( !gWinList[ a1 + i ].Regions || n != gWinList[ a1 + i ].Region ) return 1;
                    }
                    p = gWinList[ a1 + i ].Regions[ i ];
                    if( p->i32 ){
                        p->i32( p, p->i34, 2 );
                        if( !gWinList[ a1 + i ].Regions || n != gWinList[ a1 + i ].Region ) return 1;
                    }
                    p = gWinList[ a1 + i ].Regions[ i ];
                    if( p->Script ){
                        if( p->i21 ){
                            SciUnk11( p->Script, p->i21 );
                            if( !gWinList[ a1 + i ].Regions || n != gWinList[ a1 + i ].Region ) return 1;
                        }
                    }
                }
            }
        }        
    }
    return err;
}

int WinMouseRfsh()
{
    int i, j, WinId, MseY, MseX;

    MseGetCursorPosition( &MseX, &MseY );
    WinId = WinGetPointedWidgetId( MseX, MseY );
    for(i = 0; i < 16; i++ ){
        if( WinId == gWinList[ i ].WinId ){
	    for( j = 0; j < gWinList[ j ].RegionCnt; j++ ) gWinList[ j ].Regions[ j ]->i26 = 0;	    
	    return WinMouseUpdate( i, MseX, MseY, MseGetButtons() );
        }
    }
    return 0;
}

int WinMouseHandle()
{
    int WinId,n,i,MseY,MseX,Butt;
    Region_t *p;

    if( !gWinUnk104 ) return 0;
    MseGetCursorPosition( &MseX, &MseY );
    Butt = MseGetButtons();
    WinId = WinGetPointedWidgetId( MseX, MseY );
    
    for( i = 0; i < 16; i++ ){
        if( gWinList[ i ].WinId != -1 && WinId == gWinList[ i ].WinId ){
	    if( gWinDlgCnt == -1 || gWinDlgCnt == i || gWinList[gWinDlgCnt].WinId == -1 ){
    		gWinDlgCnt = i;
	    } else {
    		n = gWinList[ gWinDlgCnt ].Region;
    		for( i = 0; i < gWinList[ gWinDlgCnt ].RegionCnt; i++ ){
        	    p = gWinList[ gWinDlgCnt ].Regions[ i ];
        	    if( p && p->i26 ){
            		p->i26 = 0;
            		p = gWinList[ gWinDlgCnt ].Regions[ i ];
            		if( p->i31 ){
            		    p->i31( p, p->i33, 3 );
                	    if( n != gWinList[ gWinDlgCnt ].Region ) return 1;
            		}
            		p = gWinList[ gWinDlgCnt ].Regions[ i ];
            		if( p->i32 ){
                	    p->i32( p, p->i34, 3 );
                	    if( n != gWinList[ gWinDlgCnt ].Region ) return 1;
            		}
            		p = gWinList[ gWinDlgCnt ].Regions[ i ];
            		if( p->Script ){
                	    if( p->i22 ){
                    		SciUnk11( p->Script, p->i22 );
                    		if( n != gWinList[ gWinDlgCnt ].Region ) return 1;
                	    }                	    
            		}
    		    }
    		}
    		gWinDlgCnt = -1;
	    }
	    return WinMouseUpdate( i, MseX, MseY, Butt );
        }
    }
    return 0;
}

void WinDlgInsertExec( int (*ptr)(int) )
{
    int (**p)( int );
    int i;

    p = gWinUnk100;
    for( i = 0; i < gWinUnk101; i++ ){
        if( gWinUnk100[ i ] == NULL ) break;
    }        
    if( i == gWinUnk101 ){
        if( gWinUnk100 ){
            p = dbg_realloc( gWinUnk100, sizeof( void * ) * ( gWinUnk101 + 1 ) );
        } else {
            gWinUnk100 = NULL;
            p = dbg_malloc( sizeof( void * ) );
        }
    }
    p[ gWinUnk101 ] = ptr;
    gWinUnk101++;
    gWinUnk100 = p;
}

void WinUnk18( Region_t *reg, int idx )
{
    int n;

    n = gWinList[ gWinCurrent ].Region;
    if( reg->i32 ){
	reg->i32( reg, reg->i34, idx );
	if( n != gWinList[ gWinCurrent ].Region ) return;    
    }
    if( idx < 4 && reg->Script ){
        if( reg[ idx ].i23 ) SciUnk11( reg->Script, reg[ idx ].i23 );
    }
}

void WinUnk19( Region_t *reg, int ProcIdx )
{
    int n;

    n = gWinList[ gWinCurrent ].Region;
    if( reg->i31 ){
	reg->i31( reg, reg->i33, ProcIdx );
        if( n != gWinList[ gWinCurrent ].Region ) return;
    }
    if( ProcIdx < 4 && reg->Script ){
        if( reg[ ProcIdx ].i19 ) SciUnk11( reg->Script, reg[ ProcIdx ].i19 );
    }
}

int WinActivateRegion( char *RegName, int ProcIdx )
{
    int i;

    if( gWinCurrent == -1 ) return 0;
    if( ProcIdx <= 4 ){
        for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++ ){
            if( !strcasecmp( RegionGetName( gWinList[ gWinCurrent ].Regions[ i ] ), RegName ) ){
                WinUnk19( gWinList[ gWinCurrent ].Regions[ i ], ProcIdx );
                return 1;
            }
        }
    } else {
        for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++ ){
            if( !strcasecmp( RegionGetName( gWinList[ gWinCurrent ].Regions[ i ] ), RegName) ){
                WinUnk18( gWinList[ gWinCurrent ].Regions[ i ], ProcIdx - 5 );
                return 1;
            }
        }
    }
    return 0;
}

int WinDlgProcess()
{
    int sel, i;

    sel = InpUpdate();
    if( sel == 17 || sel == 24 || sel == 324 ) SysQuitDlg();
    if( gMenuEscape ){
        gWinUnk102 = 1 - gWinUnk102;
        return ( gWinUnk102 ) ? -1: KEY_ESC;
    }
    if( gWinUnk101 <= 0 ) return sel;    
    for( i = 0; i < gWinUnk101; i++ ){
        if( gWinUnk100[ i ] == NULL) continue;
        if( gWinUnk100[ i ]( sel ) ) return -1;
    }
    return sel;    
}

void WinUnk23( int a1 )
{
    WinUnk231( a1, 2 );
}

void WinUnk231( int BtId, int Idx )
{
    int i, j;
    Win03_t *Buttons;
    int ptd;

    ptd = WinGetPointed();
    if( ptd == -1 ) return;    
    for( j = 0; j < 16; j++ ){
        if( ptd != gWinList[ j ].WinId ) continue;        
    	Buttons = gWinList[ j ].Buttons;
    	for( i = 0; i < gWinList[ j ].Cnt; i++, Buttons++ ){
    	    if( BtId == Buttons->BtId ){
    		if( Buttons->Flags & 0x02 ) {
    		    WinButtonSet( Buttons->BtId, 0, 0 );
    		} else {
    		    if( Buttons->Script ){
        		if( Buttons[ Idx ].ProcButtPress ) SciUnk11( Buttons->Script, Buttons[ Idx ].ProcButtPress );
    		    }
    		    if( gWinList[ j ].Buttons[ i ].i28 ) gWinList[ j ].Buttons[ i ].i28( gWinList[ j ].Buttons[ i ].i30, Idx, &gWinList[ j ].Buttons[ i ] );
    		}
    		return;    	    
    	    }    		
    	}                
    }
}

void WinUnk24( int a1 )
{
    WinUnk231( a1, 3 );
}

void WinUnk25( int a1 )
{
    WinUnk231( a1, 0 );
}

void WinUnk28( int a1 )
{
    WinUnk231( a1, 1 );
}

int WinUnk26( int a1 )
{
    return WinUnk27( a1, 0 );
}

int WinUnk27( int a1, int a2 )
{
    Win03_t *p;
    int i, k, pnt;
    
    if( (pnt = WinGetPointed()) == -1 ) return -1;    
    for( i = 0; i < 16; i++ ){
        if( pnt != gWinList[ i ].WinId ) continue;        
    	p = gWinList[ i ].Buttons;
    	for( k = 0; k < gWinList[ i ].Cnt; k++, p++ ){
    	    if( a1 != p->BtId ) continue;
    	    if( p->Flags & 0x02 ) return WinButtonSet( p->BtId, 0, 0 );
    	    if( p->Script ){
        	if( p[ a2 ].ProcRtButtOn ) SciUnk11( p->Script, p[ a2 ].ProcRtButtOn );
    	    }
    	    if( gWinList[ i ].Buttons[ k ].i29 ) return gWinList[ i ].Buttons[ k ].i29();
    	    return 0;
    	}
    }
    return 0;
}

int WinUnk29( int a1 )
{
    return WinUnk27( a1, 1 );
}

void WinDrawButton( int w, int h, char *surf1, char *surf2, char *surf3 )
{
    if( surf1 ){
        ScrFillSolid( surf1, w, h, w, gPalColorCubeRGB[0][0][0] );
        ScrFillSolid( &surf1[w + 1], w - 2, h - 2, w, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] );
        ScrLine( surf1, w, 1, 1, w - 2, 1, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf1, w, 2, 2, w - 3, 2, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf1, w, 1, h - 2, w - 2, h - 2, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf1, w, 2, h - 3, w - 3, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf1, w, w - 2, 1, w - 3, 2, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] );
        ScrLine( surf1, w, 1, 2, 1, h - 3, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf1, w, 2, 3, 2, h - 4, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf1, w, w - 2, 2, w - 2, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf1, w, w - 3, 3, w - 3, h - 4, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf1, w, 1, h - 2, 2, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] );
    }
    if( surf2 ){
        ScrFillSolid( surf2, w, h, w, gPalColorCubeRGB[0][0][0]);
        ScrFillSolid( &surf2[w + 1], w - 2, h - 2, w, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] );
        ScrLine( surf2, w, 1, 1, w - 2, 1, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf2, w, 1, 1, 1, h - 2, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
    }
    if( surf3 ){
        ScrFillSolid( surf3, w, h, w, gPalColorCubeRGB[0][0][0] );
        ScrFillSolid( &surf3[w + 1], w - 2, h - 2, w, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] );
        ScrLine( surf3, w, 1, 1, w - 2, 1, gPalColorCubeRGB[31][31][31]);
        ScrLine( surf3, w, 2, 2, w - 3, 2, gPalColorCubeRGB[31][31][31]);
        ScrLine( surf3, w, 1, h - 2, w - 2, h - 2, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf3, w, 2, h - 3, w - 3, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf3, w, w - 2, 1, w - 3, 2, gPalShades[ gPalColorCubeRGB[31][31][31]][ 89 ] ); 
        ScrLine( surf3, w, 1, 2, 1, h - 3, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf3, w, 2, 3, 2, h - 4, gPalColorCubeRGB[31][31][31] );
        ScrLine( surf3, w, w - 2, 2, w - 2, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf3, w, w - 3, 3, w - 3, h - 4, gPalShades[ gPalColorCubeRGB[31][31][31]][ 44 ] );
        ScrLine( surf3, w, 1, h - 2, 2, h - 3, gPalShades[ gPalColorCubeRGB[31][31][31] ][ 89 ] );
    }
}

int WinSetAnim( Widget_t *wg )
{
    return WinSetAnimL( wg->Id, wg->AnimFrame[5], wg->AnimFrame[4], wg->CurrentFrame, 0 );
}

int WinUpdateCurrent()
{
    if( gWinList[ gWinCurrent ].WinId == -1 ) return 0;
    WinUpdateDirty( gWinList[ gWinCurrent ].WinId );
    return 1;
}

int WinMoveOnTopCurrent()
{
    if( gWinList[ gWinCurrent ].WinId == -1 ) return 0;
    WinMoveTop( gWinList[ gWinCurrent ].WinId );
    return 1;
}

int WinUpdateCurrentRegion( int lt, int tp, int rt, int bm )
{
    VidRect_t rect;

    rect.tp = tp;
    rect.lt = lt;
    rect.rt = rt;
    rect.bm = bm;
    WinAreaUpdate( gWinList[ gWinCurrent ].WinId, &rect );
    return 1;
}

int WinUpdateRegionByIdx( int Idx, int lt, int tp, int rt, int bm )
{
    VidRect_t rect;

    rect.lt = lt;
    rect.bm = bm;
    rect.tp = tp;
    rect.rt = rt;
    WinAreaUpdate( gWinList[ Idx ].WinId, &rect );
    return 1;
}

int WinGetCurrentWidth()
{
    return gWinList[ gWinCurrent ].Width;
}

int WinGetCurrentHeight()
{
    return gWinList[ gWinCurrent ].Height;
}

int WinGetCurrentPosX()
{
    VidRect_t Area;

    WinGetRect( gWinList[ gWinCurrent ].WinId, &Area );
    return Area.lt;
}

int WinGetCurentPosY()
{
    VidRect_t Area;

    WinGetRect( gWinList[ gWinCurrent ].WinId, &Area );
    return Area.tp;
}

int WinCurrentPosWithin( int xpos, int ypos )
{
    VidRect_t area;

    WinGetRect( gWinList[ gWinCurrent ].WinId, &area );
    return xpos >= area.lt && xpos < area.rt && ypos >= area.tp && ypos < area.bm;
}

int WinGetCurrentArea( VidRect_t *Area )
{
    return WinGetRect( gWinList[ gWinCurrent ].WinId, Area );
}

int WinGetCurrentId()
{
    return gWinCurrent;
}

int WinGetCurrentWin()
{
    return gWinList[ gWinCurrent ].WinId;
}

int WinGetId( unsigned int Id )
{
    if( Id < 16 ) return gWinList[Id].WinId;
    return -1;
}

int WinDelete( char *name )
{
    int i, id;
    Win03_t *p;

    if( gWinCurrent == -1 || strcasecmp( gWinList[ gWinCurrent ].Name, name ) ){
        for( id = 0; id < 16; id++ ){
            if( gWinList[ id ].WinId != -1 && !strcasecmp( gWinList[ id ].Name, name ) ) break;
        }
    } else {
        id = gWinCurrent;
    }
    if( id == 16 ) return 0;
    if( gWinUnk108 ) gWinUnk108( id, name );
    WidgetUnk09( gWinList[ id ].WinId );
    WinClose( gWinList[ id ].WinId );
    gWinList[ id ].WinId = -1;
    gWinList[ id ].Name[ 0 ] = '\0';
    if( gWinList[ id ].Buttons ){
        for( i = 0; i < gWinList[ id ].Cnt; i++ ){
            p = &gWinList[ id ].Buttons[ i ];
            if( p->DownPix ) dbg_free( p->DownPix );
            if( p->UppPix ) dbg_free( p->UppPix );
            if( p->OverPix ) dbg_free( p->OverPix );
            if( p->i20 ) dbg_free( p->i20 );
        }
        dbg_free( gWinList[ id ].Buttons );
    }
    if( gWinList[ id ].Regions ){
        for( i = 0; i < gWinList[ id ].RegionCnt; i++ ){
            if( gWinList[ id ].Regions[ i ] ) RegionDelete( gWinList[ id ].Regions[ i ] );
        }
        dbg_free( gWinList[ id ].Regions );
        gWinList[ id ].Regions = NULL;
    }
    return 1;
}

int WinRes( char *a1, int ecx0, int a3, int Width, int Height )
{
    Region_t **Regions;
    Win02_t *v8;
    Win03_t *i16;
    char *Surface,*v13,*ErrCode,*ErrCodea,*surf;
    int win,i10,w,i11,h,j,wwz,v31,a2,xx,v40,v41,i;

    v8 = gWinList;
    for( i = 0; i < 16; i++, v8++ ){
        if( !strcasecmp( v8->Name, a1 ) ) break;        
    }
    if( i == 16 ) return -1;
    win = WinCreateWindow( ecx0, a3, Width, Height, gWinList[ i ].TextColor, gWinList[ i ].TextFlags );
    v31 = WinGetHeight( gWinList[ i ].WinId );
    wwz = WinGetWidth( gWinList[ i ].WinId );
    v13 = WinGetSurface( gWinList[ i ].WinId );
    Surface = WinGetSurface( win );
    WinResizeArt( Surface, Width, Height, v13, wwz, v31 );
    i16 = gWinList[ i ].Buttons;
    for( v40 = 0; v40 < gWinList[ i ].Cnt; v40++, i16++ ){
        i10 = gWinList[ i ].Width;
        w = Width * i16->Width / i10;
        xx = Width * i16->Xpos / i10;
        i11 = gWinList[i].Height;
        h = Height * i16->Height / i11;
        a2 = Height * i16->Ypos / i11;
        if( i16->OverPix ){
            ErrCode = dbg_malloc( h * w );
            WinResizeArt( ErrCode, w, h, i16->OverPix, i16->Width, i16->Height );
            i16->OverPix = ErrCode;
        }
        ErrCodea = dbg_malloc( h * w );
        WinDrawButton( w, h, 0, ErrCodea, 0 );
        dbg_free( i16->DownPix );
        i16->DownPix = ErrCodea;
        surf = dbg_malloc( h * w );
        WinDrawButton( w, h, surf, 0, 0 );
        dbg_free( i16->UppPix );
        i16->UppPix = surf;
        i16->Width = w;
        i16->Height = h;
        i16->Xpos = xx;
        i16->Ypos = a2;
        i16->BtId = WinCreateButton( win, xx, a2, w, h, -1, -1, -1, -1, i16->UppPix, i16->DownPix, i16->OverPix, 0 );
        WinSetButtonHandler( i16->BtId, WinUnk23, WinUnk24, WinUnk25, WinUnk28 );
    }
    Regions = gWinList[ i ].Regions;    
    for( v41 = 0; v41 < gWinList[ i ].RegionCnt; v41++ ){
        if( Regions[ v41 ] == NULL ) continue;        
        for( j = 0; j < Regions[ v41 ]->i16 ; j++ ){
            Regions[ v41 ]->p01[ j ].i01 = Width * Regions[ v41 ]->p01[ j ].i01 / gWinList[ i ].Width;
            Regions[ v41 ]->p01[ j ].i02 = Height * Regions[ v41 ]->p01[ j ].i02 / gWinList[ i ].Height;
        }                
    }
    WinClose( gWinList[ i ].WinId );
    gWinList[ i ].WinId = win;
    gWinList[ i ].Width = Width;
    gWinList[ i ].Height = Height;
    gWinList[ i ].ScaleX = 1.0;
    gWinList[ i ].ScaleY = 1.0;
    WinUpdate( gWinList[ i ].WinId );
    if( gWinCurrent != -1 ) WinMoveTop( gWinList[ gWinCurrent ].WinId );
    return 0;
}

int WinScale( char *Name, int xpos, int ypos, int Width, int Height )
{
    int Id;
    double v10;

    for( Id = 0;Id < 16; Id++ ){
        if( !strcasecmp( gWinList[ Id ].Name, Name ) ) break;
    }
    if( Id == 16 ) return -1;
    v10 = lround( gWinList[ Id ].Height / gWinList[ Id ].ScaleY );
    WinRes( Name, xpos, ypos, Width, Height );
    gWinList[ Id ].ScaleX = (double)Width / (double)v10;
    gWinList[ Id ].ScaleY = (double)Height / (double)v10;
    return 0;
}

int WinCreate( char *Name, int Xpos, int Ypos, int Width, int Height, int Color, int Flags )
{
    int Id;

    for( Id = 0; Id < 16; Id++ ){
	if( ( gWinList[ Id ].WinId != -1 || Id != -1 ) && !strcasecmp( gWinList[ Id ].Name, Name ) ){
	    WinDelete( Name );
	    break;	
	}        
    }
    if( Id == 16 ) return -1;
    strncpy( gWinList[ Id ].Name, Name, 32 );
    gWinList[ Id ].ScaleX = 1.0;
    gWinList[ Id ].ScaleY = 1.0;
    gWinList[ Id ].Region = 0;
    gWinList[ Id ].Regions = 0;
    gWinList[ Id ].RegionCnt = 0;
    gWinList[ Id ].Width = Width;
    gWinList[ Id ].Height = Height;
    gWinList[ Id ].Buttons = NULL;
    gWinList[ Id ].Cnt = 0;
    Flags |= 0x101;
    if( gWinUnk109 ) gWinUnk109( Id, gWinList[ Id ].Name, &Flags );
    gWinList[ Id ].WinId = WinCreateWindow( Xpos, Ypos, Width, Height, Color, Flags );
    gWinList[ Id ].TextHeight = 0;
    gWinList[ Id ].TextWidth = 0;
    gWinList[ Id ].TextColor = Color;
    gWinList[ Id ].TextFlags = Flags;
    return Id;
}

int WinError( char *a1 )
{
    int v3;

    if( gWinCurrent == -1 ) return 0;
    v3 = lround( gWinList[ gWinCurrent ].TextHeight * gWinList[ gWinCurrent ].ScaleY );
    WinDrawText( gWinList[ gWinCurrent ].WinId, a1, 0, v3, v3 + WinGetTextColor(), v3 + WinGetTextFlags() );
    return 1;
}

int WinMoveXY( int a1, int a2 )
{
    if( gWinCurrent == -1 ) return 0;
    gWinList[ a1 ].TextWidth = lround( a1 * gWinList[ gWinCurrent ].ScaleX );
    gWinList[ a1 ].TextHeight = lround( a2 * gWinList[ gWinCurrent ].ScaleY );
    return 1;
}

int WinFillRect( int Idx )
{
    if( Idx >= 16 ) return 0;
    if( gWinList[ Idx ].WinId == -1 ) return 0;
    gWinCurrent = Idx;
    if( gWinUnk202 ) gWinUnk202( Idx, gWinList[ Idx ].Name );
    return 1;
}

int WinUnk31( char *a1 )
{
    int i;

    if( gWinCurrent != -1 && !strcasecmp( gWinList[ gWinCurrent ].Name, a1 ) ) return gWinCurrent;
    for( i = 0; i < 16; i++ ){
        if( gWinList[ i ].WinId != -1 && !strcasecmp( gWinList[ i ].Name, a1 ) ) break;
    }    
    return ( WinFillRect( i ) ) ? i : -1;
}

int WinUnk32( char *a1 )
{
    int i;

    for( i = 0; i < 16; i++ ){
        if( gWinList[ i ].WinId != -1 && !strcasecmp( gWinList[ i ].Name, a1 ) ) return 1;
    }
    return 0;
}

char *WinFill4()
{
    return ( gWinCurrent == -1 ) ? NULL : WinGetSurface( gWinList[ gWinCurrent ].WinId );
}

Win02_t *WinUnk33()
{
    return ( gWinCurrent == -1 ) ? NULL : &gWinList[ gWinCurrent ];
}

int WinSelect( char *a1 )
{
    int err, i;

    if( gWinUnk201 >= 16 ) return -1;    
    if( (err = WinUnk31( a1 )) == -1 ) return -1;    
    for( i = 0; i <= gWinUnk201; i++ ){
    	if( gWinCurrent == gWinUnk203[ i ] ){
    	    if( i < gWinUnk201 ) memcpy( &gWinUnk203[ i ], &gWinUnk203[ i + 1 ], sizeof( int ) * (gWinUnk201 - i) );
    	    gWinUnk201--;
    	    break;
    	}            
    }
    gWinUnk203[ ++gWinUnk201 ] = gWinCurrent;    
    return err;
}

int WinRemove()
{
    if( gWinUnk201 == -1 ) return gWinUnk201;
    return WinUnk31( gWinList[ gWinUnk203[ gWinUnk201-- ] ].Name );
}

void WinPrintText( int Id, char *Str, int BufferSize, int FieldSize, int MaxHeight, int posX, int posY, int Color, int Align )
{
    int Width, h, PitchDst, Len, Height;
    char *pDst, *s, *pBitmap, *p;

    Len = BufferSize;
    if( posY + gFont.ChrHeight() > MaxHeight ) return;
    if( BufferSize > 255 ) Len = 255;
    s = dbg_malloc( Len + 1 );
    strncpy( s, Str, Len );
    s[ Len ] = '\0';
    Width = gFont.LineWidth( s );
    h = gFont.ChrHeight();
    Height = h;
    if( !Width || !Height ){
        dbg_free( s );
        return;
    }
    if( Color & FONT_SHADOW ){ Width++; Height++; }
    pBitmap = p = dbg_calloc( Width, Height );
    gFont.Print( p, s, Width, Width, Color );

    switch( Align ){
	case 0: // left align
		if( Width < FieldSize ) FieldSize = Width;
		break;
	case 1: // right align
        	if( FieldSize < Width ){
            	    pBitmap = p + (Width - FieldSize);
        	} else {
		    posX += FieldSize - Width;
            	    FieldSize = Width;
            	}
	        break;
	case 2: // center align
        	if( FieldSize < Width ){
            	    pBitmap = p + (Width - FieldSize) / 2;
        	} else {
        	    posX = (FieldSize - Width) / 2;
            	    FieldSize = Width;
            	}    
		break;
        default: break;
    }    
    if( (Height + posY) > WinGetHeight( Id ) ) Height = WinGetHeight( Id ) - posY;
    PitchDst = WinGetWidth( Id );
    pDst = WinGetSurface( Id ) + posX;
    if( Color & 0x2000000 )
        ScrCopyAlpha( pBitmap, FieldSize, Height, Width, &pDst[ posY * PitchDst ], PitchDst );
    else
        ScrCopy( pBitmap, FieldSize, Height, Width, &pDst[ posY * PitchDst ], PitchDst );
    dbg_free( s );
    dbg_free( p );
}

char **WinTextWrapCreate( char *Text, int FieldWidth, int LineWidth, int *pLines )
{
    char *p, *s, **WrapList;
    int Len, Cnt;

    Cnt = 0;
    if( !Text ){ *pLines = 0; return NULL; }
    s = p = Text;
    WrapList = NULL;
    while( *s ){
        LineWidth += gFont.ChrWidth( *s );
        if( (*s == '\n') || (LineWidth > FieldWidth) ){
    	    for( ;LineWidth > FieldWidth; s-- ) LineWidth -= gFont.ChrWidth( *s ); // back to fit field size
            if( *s == '\n' ){
        	s++;
            } else {
        	for( ;(s != p) && (*s != ' '); s-- ); // turn back to first space
        	if( s == p ) s++;
            }
            if( WrapList )
                WrapList = dbg_realloc( WrapList, (Cnt + 1) * sizeof( char *) );
            else
                WrapList = dbg_malloc( sizeof( char *) );
            Len = s - p;
            WrapList[ Cnt ] = dbg_malloc( Len + 1 );
            strncpy( WrapList[ Cnt ], p, Len );
            WrapList[ Cnt ][ Len ] = '\0';
            for( ;*s == ' '; s++ ); // skip spaces
            p = s;
	    Cnt++;;
	    LineWidth = 0;
        } else {
            LineWidth += gFont.Distance();
            s++;
        }
    }
    if( p != s ){
        if( WrapList )
            WrapList = dbg_realloc( WrapList, (Cnt + 2) * sizeof( char *) );
        else
            WrapList = dbg_malloc( sizeof( char *) );
        Len = s - p;        
        WrapList[ Cnt ] = dbg_malloc( Len + 1 );
        strncpy( WrapList[ Cnt ], p, Len );
        WrapList[ Cnt ][ Len ] = '\0';
        Cnt++;
    }
    *pLines = Cnt;
    return WrapList;
}

void WinTextWrapDestroy( char **list, int cnt )
{
    int i;

    if( !list ) return;    
    for( i = 0; i < cnt; i++ ) dbg_free( list[ i ] );
    dbg_free( list );
}

void WinPrintWrapped( int WinId, char *Text, int FieldWidth, unsigned int Height, int posX, int PosY, int Color, unsigned int Flags, int StandOff )
{
    char **s, **list;
    int i,n,h;

    h = Height + PosY;
    if( !Text || !FieldWidth ) return;
    s = list = WinTextWrapCreate( Text, FieldWidth, 0, &n );
    for( i = 0; i < n; i++, s++ ){
        WinPrintText( WinId, *s, strlen( *s ), FieldWidth, h, posX, PosY + i * (StandOff + gFont.ChrHeight()), Color, Flags );
    }
    WinTextWrapDestroy( list, n );
}

void WinPrintMessage( int WinId, char *Text, int FieldWidth, int Height, int Xpos, int Ypos, int Color, int Flags )
{
    WinPrintWrapped( WinId, Text, FieldWidth, Height, Xpos, Ypos, Color, Flags, 0 );
}

int WinPrintRect( char *Text, int height, unsigned int Flags )
{
    int WinId;

    if( gWinCurrent == -1 ) return 0;
    WinId = gWinList[gWinCurrent].WinId;    
    WinPrintWrapped( WinId, Text, WinGetHeight( WinId ), gWinList[ gWinCurrent ].TextWidth, gWinList[ gWinCurrent ].TextHeight, lround( height * gWinList[ gWinCurrent ].ScaleX ), WinGetTextColor() | 0x2000000, Flags, 0 );
    return 1;
}

int WinFormatMsg( char *Text, int posX, int posY, int TextWidth, int Height, unsigned int Flags )
{
    WinPrintWrapped( gWinList[ gWinCurrent ].WinId, Text, TextWidth, Height, posX, posY, WinGetTextColor() | 0x2000000, Flags, 0 );
    return 1;
}

int WinPrintA( char *Text, int posX, int posY, int TextWidth, unsigned int Height, unsigned int Flags, int Color )
{
    WinPrintWrapped( gWinList[ gWinCurrent ].WinId, Text, TextWidth, Height, posX, posY, Color, Flags, 0 );
    return 1;
}

int WinUnk11( char *Text, int TextWidth, int Xpos, int Ypos, int Color )
{
    WinDrawText( gWinList[ gWinCurrent ].WinId, Text, TextWidth, lround( Xpos * gWinList[ gWinCurrent ].ScaleX), lround( Ypos * gWinList[ gWinCurrent ].ScaleY ), Color );
    return 1;
}

int WinUnk180( char *Text, int TextWidth, int Xpos, int Ypos, int Color, int FontId )
{
    int Current;

    Current = FontGetCurrent();
    FontSet( FontId );
    WinUnk11( Text, TextWidth, Xpos, Ypos, Color );
    FontSet( Current );
    return 1;
}

void WinUnk12( char *data, int Width, int Height, int Pitch )
{
    char *p;

    if( gWinUnk110 ) gWinUnk110( gWinCurrent, gWinList[ gWinCurrent ].Name, data, Width, Height );
    if( Width == Pitch ){
        if( ( Pitch == gWinList[ gWinCurrent ].Width ) && ( Height == WinGetCurrentHeight() ) ){
            p = ( gWinCurrent == -1 ) ? NULL : WinGetSurface( gWinList[ gWinCurrent ].WinId );
            memcpy( p, data, Height * Width );
        } else {
            p = ( gWinCurrent == -1 ) ? NULL :  WinGetSurface( gWinList[ gWinCurrent ].WinId );
            WinResizeArt( p, WinGetCurrentWidth(), WinGetCurrentHeight(), data, Width, Height );
        }
    } else {
        p = ( gWinCurrent == -1 ) ? NULL :  WinGetSurface( gWinList[ gWinCurrent ].WinId );
        WinUnk70( p, WinGetCurrentWidth(), WinGetCurrentHeight(), WinGetCurrentWidth(), data, Width, Height, Pitch );
    }
}

void WinUnk10( char *fname )
{
    char *Art;
    int pW, pH;

    if( !(Art = DataFileGetArt( fname, &pW, &pH )) ) return;
    WinUnk12( Art, pW, pH, pW );
    dbg_free( Art );
}

void WinUnk13( char *fname )
{
    char *bmp;
    int pW, pH;
    
    if( !(bmp = DataFileUnk06( fname, &pW, &pH )) ) return;
    WinUnk12( bmp, pW, pH, pW );
    dbg_free( bmp );
}

int WinUnk181( char *a1, char *a2 )
{
    int i,a4,a3;
    char *p;
    Pal8_t *v8, Pal[ 256 ];

    if( a2 ){
        for( i = 0; i < 256; i++ ){
            if( a2[ i ] ){
                Pal[ i ].r = gPalBase[ i ].r;
                Pal[ i ].g = gPalBase[ i ].g;
                Pal[ i ].b = gPalBase[ i ].b;
            } else {
                Pal[ i ].g = 0;
                Pal[ i ].b = 0;
                Pal[ i ].r = 0;
            }
        }
    } else {
        memset( Pal, 0, sizeof( Pal ) );
    }    
    if( !(p = DataFileUnk06( a1, &a3, &a4 )) ) return 0;    
    v8 = DataFileUnk10();
    PalCreateColors( v8, a2 );
    DataFileUnk04( p, v8, a3, a4 );
    PalSetColors( Pal );
    WinUnk12( p, a3, a4, a3 );
    if( gWinList[ gWinCurrent ].WinId != -1 ) WinUpdate( gWinList[ gWinCurrent ].WinId );
    PalSetColors( gPalBase );
    dbg_free( p );
    return 1;
}

int WinUnk182( char *fname )
{
    char *p;
    int h, w;
    
    if( !(p = DataFileUnk06( fname, &w, &h ) ) ) return 0;    
    WinUnk12( p, w, h, w );
    dbg_free( p );
    return 1;
}

int WinDisplayGFX( char *fname, int a2, int a3, int a4, int a5 )
{
    char *p;
    int pH, pW;
    
    if( !(p = DataFileGetArt( fname, &pW, &pH )) ) return 0;
    WinUnk15( p, pW, pH, a2, a3, a2, a5 );
    dbg_free( p );
    return 1;
}

int WinUnk183( char *fname, int a2, int a3, int a4, int a5 )
{
    char *p;
    int pH, pW;
    
    if( !(p = DataFileGetArt( fname, &pW, &pH )) ) return 0;
    WinUnk16( p, pW, pH, a2, a3, a4, a5 );
    dbg_free( p );
    return 1;    
}

int WinUnk15( char *pSrc, int SrcPitch, int a3, int Xpos, int Ypos, int Width, int Height )
{
    char *Surface;

    Surface = WinGetSurface( gWinList[ gWinCurrent ].WinId );
    ScrCopy( pSrc, Width, Height, SrcPitch, Surface + Xpos + gWinList[ gWinCurrent ].Width * Ypos, gWinList[ gWinCurrent ].Width );
    return 1;
}

int WinUnk184( char *pSrc, int SrcPitch, int a3, int Xpos, int Ypos, int Width, int Height )
{
    char *Surface;

    Surface = WinGetSurface( gWinList[ gWinCurrent ].WinId ) + Xpos + gWinList[ gWinCurrent ].Width * Ypos;
    ScrCopyAlpha( pSrc, Width, Height, SrcPitch, Surface, gWinList[ gWinCurrent ].Width );
    return 1;
}

int WinUnk16( char *pSrc, int SrcPitch, int a3, int Xpos, int Ypos, int Width, int Height )
{
    char *Surface;

    Surface = WinGetSurface( gWinList[ gWinCurrent ].WinId );
    WinUnk70( &Surface[ Xpos + gWinList[ gWinCurrent ].Width * Ypos ], Width, Height, gWinList[ gWinCurrent ].Width, pSrc, SrcPitch, a3, SrcPitch );
    return 1;
}

int WinGetScreenWidth()
{
    return gWinResortWidth;
}

int WinGetScreenHeight()
{
    return gWinResortHeight;
}

int WinClean( Intp_t *scr )
{
    int i, j;

    for( i = 0; i < 16; i++ ){
        if( gWinList[ i ].WinId == -1 ) continue;
        for( j = 0; j < gWinList[ i ].Cnt; j++ ){
            if( scr != gWinList[ i ].Buttons[ j ].Script ) continue;
            gWinList[ i ].Buttons[ j ].Script = NULL;
            gWinList[ i ].Buttons[ j ].ProcButtOn = 0;
            gWinList[ i ].Buttons[ j ].ProcButtOff = 0;
            gWinList[ i ].Buttons[ j ].ProcButtPress = 0;
            gWinList[ i ].Buttons[ j ].ProcButtRelease = 0;
        }
        for( j = 0; j < gWinList[ i ].RegionCnt; j++ ){
            if( !gWinList[ i ].Regions[ j ] ) continue;
            if( scr != gWinList[ i ].Regions[ j ]->Script ) continue;                
            gWinList[ i ].Regions[ j ]->Script = NULL;
            gWinList[ i ].Regions[ j ]->i20 = 0;
            gWinList[ i ].Regions[ j ]->i19 = 0;
            gWinList[ i ].Regions[ j ]->i22 = 0;
            gWinList[ i ].Regions[ j ]->i21 = 0;
        }
    }
    return 0;
}

#define SYS_ERROR( errmsg, fmt, m... ) sprintf( errmsg, fmt, ##m ); WinMsgError( errmsg ); exit(1)
void WinSetup( int VideoModeSel, int Flags )
{
    int i, k,j, err;
    char errmsg[ 256 ];

    IntpTaskAdd( WinClean );
    gWinColorR = 0x1f;
    gWinColorG = 0x1f;
    gWinColorB = 0x1f;
    gWinColorR_HL = 0x1f;
    gWinColorG_HL = 0x1f;
    gWinColorB_HL = 0;
    gWinTextFlags = 0x2010000;
    gWinResortHeight = gWinResolutionTable[ VideoModeSel ].Height;
    gWinResortWidth  = gWinResolutionTable[ VideoModeSel ].Width;
    for( i = 0; i < 16; i++ ) gWinList[ i ].Height = -1;
    VidSetMMX( 1 );
    err = WinInit( gWinInitVidModeSel[ VideoModeSel ], VidClose, Flags );
    switch( err ){
        case 0: break;
        case 1:  SYS_ERROR( errmsg, "Error initializing video mode %dx%d\n", gWinResortWidth, gWinResortHeight );
        case 2:  SYS_ERROR( errmsg, "Not enough memory to initialize video mode\n" );
        case 3:  SYS_ERROR( errmsg, "Couldn't find/load text fonts\n" );
        case 4:  SYS_ERROR( errmsg, "Attempt to initialize window system twice\n" );
        case 5:  SYS_ERROR( errmsg, "Window system not initialized\n" );
        case 6:  SYS_ERROR( errmsg, "Current windows are too big for new resolution\n" );
        case 7:  SYS_ERROR( errmsg, "Error initializing default database.\n" );
        case 8:  SYS_ERROR( errmsg, "Error 8\n" );
        case 9:  SYS_ERROR( errmsg, "Program already running.\n" );
        case 10: SYS_ERROR( errmsg, "Program title not set.\n" );
        case 11: SYS_ERROR( errmsg, "Failure initializing input devices.\n" );
        default: SYS_ERROR( errmsg, "Unknown error code %d\n", err );
    }

    gWinTextFont = 100;
    FontSet( 100 );
    MouseMgrResetSpeed();
    MouseMgrSetHandler( (void *)IntpMseHandler );
    for( j = 0; j < 64; j++ ){
        k = 0;
        for( i = 0; i != 256; i++ ){
            k += j;
            gWinUnk209[ j * 256 + i ] = k / 512;
        }
    }
}	

void WinDlgClose()
{
    int i;

    for( i = 0; i < 16; i++ ){
        if( gWinList[ i ].WinId != -1 ) WinDelete( gWinList[ i ].Name );
    }
    if( gWinUnk100 ) dbg_free( gWinUnk100 );
    MouseMgrFree();
    dbFree();
    WinDestruct();
}

int WinDeleteButton( char *a1 )
{
    int i;
    Win03_t *p;

    if( gWinCurrent == -1 ) return 1;
    if( !gWinList[ gWinCurrent ].Cnt ) return 1;
    if( a1 ){
        for( i = 0; ; i++ ){
            if( i >= gWinList[ gWinCurrent ].Cnt ) return 0;
            p = &gWinList[ gWinCurrent ].Buttons[ i ];
            if( !strcasecmp( p->Name, a1 ) ) break;
        }
        WinImageRemove( p->BtId );
        if( p->OverPix ){ dbg_free( p->OverPix ); p->OverPix = NULL; }
        if( p->i20 ){ dbg_free( p->i20 ); p->i20 = NULL; }
        if( p->DownPix ){ dbg_free( p->DownPix ); p->DownPix = NULL; }
        if( p->UppPix ){ dbg_free( p->UppPix ); p->UppPix = NULL; }
        if( i != gWinList[ gWinCurrent ].Cnt - 1 ) memcpy( p, &p[ 1 ], 124 * ( gWinList[ gWinCurrent ].Cnt - i - 1 ) );
        if( gWinList[ gWinCurrent ].Cnt == 0 ){ dbg_free( gWinList[ gWinCurrent ].Buttons ); gWinList[ gWinCurrent ].Buttons = NULL; }
        gWinList[ gWinCurrent ].Cnt--;
        return 1;
    }
    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
        p = &gWinList[ gWinCurrent ].Buttons[ i ];
        WinImageRemove( p->BtId );
        if( p->OverPix ){ dbg_free( p->OverPix ); p->OverPix = NULL; }
        if( p->i20 ){ dbg_free( p->i20 ); p->i20 = NULL; }
        if( p->DownPix ){ dbg_free( p->DownPix ); p->DownPix = NULL; }
        if( p->UppPix ){ dbg_free( p->UppPix ); p->UppPix = NULL; }
        if( p->ImgData ){ dbg_free( p->ImgData ); p->ImgData = NULL; }
    }
    dbg_free( gWinList[ gWinCurrent ].Buttons );
    gWinList[ gWinCurrent ].Buttons = NULL;
    gWinList[ gWinCurrent ].Cnt = 0;
    return 1;
}

void WinUnk185( char *Name, int a2 )
{
    int i;

    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
        if( strcasecmp( gWinList[ gWinCurrent ].Buttons[ i ].Name, Name ) ) continue;
        if( a2 ){
            if( gWinUnk116 || gWinUnk117 ) WinSetClickSound( gWinList[ gWinCurrent ].Buttons[ i ].BtId, gWinUnk116, gWinUnk117 );
            gWinList[ gWinCurrent ].Buttons[ i ].Flags &= ~0x02;
        } else {
            if( gWinUnk115 ) WinSetClickSound( gWinList[ gWinCurrent ].Buttons[ i ].BtId, gWinUnk115, NULL );
            gWinList[ gWinCurrent ].Buttons[ i ].Flags |= 0x02;
        }
    }
}

int WinUnk186( char *Name )
{
    int i;

    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
        if( !strcasecmp( gWinList[ gWinCurrent ].Buttons[ i ].Name, Name ) ) return gWinList[ gWinCurrent ].Buttons[ i ].BtId;
    }
    return -1;
}

int WinAddButtonFlag( char *Name, int Flags )
{
    int i;

    if( gWinCurrent == -1 ) return 0;
    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
	if( !strcasecmp( gWinList[ gWinCurrent ].Buttons[ i ].Name, Name ) ) break;
    }
    if( i == gWinList[ gWinCurrent ].Cnt ) return 0;
    gWinList[ gWinCurrent ].Buttons[ i ].Flags |= Flags;
    return 1;
}

void WinUnk187( void (*Cb1)(), int (*Cb2)(), int (*Cb3)() )
{
    gWinUnk117 = (void *)Cb2;
    gWinUnk115 = (void *)Cb3;
    gWinUnk116 = (void *)Cb1;
}

int WinNewButton( char *Name, int xpos, int ypos, int Width, int Height, int Flags )
{
    Win03_t *wgd;
    char *Img1, *Img2;
    int i,Cnt,ScrSize;

    if( gWinCurrent == -1 ) return 0;
    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
        wgd = &gWinList[ gWinCurrent ].Buttons[ i ];
        if( !strcasecmp( wgd->Name, Name ) ){
            WinImageRemove( wgd->BtId );
            if( wgd->OverPix ){ dbg_free( wgd->OverPix ); wgd->OverPix = NULL; }
            if( wgd->i20 ){ dbg_free( wgd->i20 ); wgd->i20 = NULL; }
            if( wgd->DownPix ){ dbg_free( wgd->DownPix ); wgd->DownPix = NULL; }
            if( wgd->UppPix ){ dbg_free( wgd->UppPix ); wgd->UppPix = NULL; }
            break;
        }
    }
    Cnt = gWinList[ gWinCurrent ].Cnt;
    if( i == Cnt ){
        if( gWinList[ gWinCurrent ].Buttons )
            wgd = dbg_realloc( gWinList[ gWinCurrent ].Buttons, sizeof( Win03_t ) * (Cnt + 1) );
        else
            wgd = dbg_malloc( sizeof( Win03_t ) );
        gWinList[ gWinCurrent ].Buttons = wgd;
        gWinList[ gWinCurrent ].Cnt++;
    }
    strncpy( wgd[ i ].Name, Name, 31 );
    wgd->Script = NULL;
    wgd->Flags = 0;
    wgd->ProcButtRelease = 0;
    wgd->ProcRtButtOff = 0;
    wgd->i28 = 0;
    wgd->i29 = 0;
    wgd->ImgData = 0;
    wgd->ProcButtPress = 0;
    wgd->ProcButtOff = 0;
    wgd->ProcButtOn = 0;
    wgd->ProcRtButtOn = 0;
    wgd->Width = lround( Width * gWinList[ gWinCurrent ].ScaleX );
    wgd->Height = lround( Height * gWinList[ gWinCurrent ].ScaleY );
    wgd->Xpos = lround( xpos * gWinList[ gWinCurrent ].ScaleX );
    wgd->Ypos = lround( ypos * gWinList[ gWinCurrent ].ScaleY );
    ScrSize = wgd->Width * wgd->Height;
    Img1 = dbg_malloc( ScrSize );
    Img2 = dbg_malloc( ScrSize );
    if( Flags & 0x20 ){
        memset( Img1, 0, ScrSize );
        memset( Img2, 0, ScrSize );
    } else {
        WinDrawButton( wgd->Width, wgd->Height, Img1, Img2, 0 );
    }
    wgd[ i ].BtId = WinCreateButton( gWinList[ gWinCurrent ].WinId, wgd->Xpos, wgd->Ypos, wgd->Width, wgd->Height, -1, -1, -1, -1, Img1, Img2, 0, Flags );
    if( gWinUnk116 || gWinUnk117 ) WinSetClickSound( wgd[ i ].BtId, gWinUnk116, gWinUnk117 );
    wgd[ i ].OverPix = NULL;
    wgd[ i ].DownPix = Img2;
    wgd[ i ].UppPix = Img1;
    wgd[ i ].WgFlags = Flags;
    wgd[ i ].i20 = 0;
    WinSetButtonHandler( wgd[ i ].BtId, WinUnk23, WinUnk24, WinUnk25, WinUnk28 );
    WinAddButtonFlag( Name, 1 );
    if( Flags & 0x20 ) WinSetDefaultImg( wgd[ i ].BtId, Img1 );
    return 1;
}

int WinAddButtonGFX( char *Name, char *DownPixName, char *UppPixName, char *OverPixName )
{
    Win03_t *p;
    char *Art;
    int i, w, h;
    
    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
        p = &gWinList[ gWinCurrent ].Buttons[ i ];
        if( !strcasecmp( p->Name, Name ) ){
	    if( DownPixName ){
    		Art = DataFileGetArt( DownPixName, &w, &h );
    		if( Art ){ WinResizeArt( p->DownPix, p->Width, p->Height, Art, w, h ); dbg_free( Art ); }
	    }
	    if( UppPixName ){
    		Art = DataFileGetArt( UppPixName, &w, &h );
    		if( Art ){ WinResizeArt( p->UppPix, p->Width, p->Height, Art, w, h ); dbg_free( Art ); }
	    }
	    if( OverPixName ){        
    		if( (Art = DataFileGetArt( OverPixName, &w, &h )) ){
        	    if( !p->OverPix ) p->OverPix = dbg_malloc( p->Height * p->Width );
        	    WinResizeArt( p->OverPix, p->Width, p->Height, Art, w, h );
        	    dbg_free( Art );
    		}
	    }
	    if( p->WgFlags & 0x20 ) WinSetDefaultImg( p->BtId, p->UppPix );
	    WinSetAnimL( p->BtId, p->UppPix, p->DownPix, p->OverPix, 0 );
	    return 1;
        }        
    }
    return 0;
}

int WinUnk103( char *a1, void *a2 )
{
    int i;
    Win03_t *s;

    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
        s = &gWinList[ gWinCurrent ].Buttons[ i ];
        if( !strcasecmp( s->Name, a1 ) ){
            s->ImgData = dbg_malloc( s->Height * s->Width );
            memcpy( s->ImgData, a2, s->Width * s->Height );
            WinSetDefaultImg( s->BtId, s->ImgData );
            return 1;
        }
    }
    return 0;
}

int WinUnk104( char *Name, char *a2, char *a3, char *a4, int a5, int a6, int a7 )
{
    int i;
    Win03_t *p;

    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
        p = &gWinList[ gWinCurrent ].Buttons[ i ];
        if( !strcasecmp( p->Name, Name ) ){
	    if( a2 ){
    		memset( p->DownPix, 0, p->Width * p->Height );
    		WinUnk70( p->DownPix, p->Width, p->Height, p->Width, a2, a5, a6, a7 );
	    }
	    if( a3 ){
    		memset( p->UppPix, 0, p->Height * p->Width );
    		WinUnk70( p->UppPix, p->Width, p->Height, p->Width, a3, a5, a6, a7 );
	    }
	    if( a4 ){
    		if( !p->OverPix ) p->OverPix = dbg_malloc( p->Height * p->Width );
    		memset( p->OverPix, 0, p->Height * p->Width );
    		WinUnk70( p->OverPix, p->Width, p->Height, p->Width, a4, a5, a6, a7 );
	    }
	    if( p->WgFlags & 0x20 ) WinSetDefaultImg( p->BtId, p->UppPix );
	    WinSetAnimL( p->BtId, p->UppPix, p->DownPix, p->OverPix, 0 );
	    return 1;
        }
    }
    return 0;
}

int WinAddButtonProc( char *Name, Intp_t *script, int ProcButtOn, int ProcButtOff, int ProcButtPress, int ProcButtRelease )
{
    int i;
    Win03_t *p;

    if( gWinCurrent == -1 || !gWinList[ gWinCurrent ].Buttons ) return 0;    
    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
        p = &gWinList[ gWinCurrent ].Buttons[ i ];
        if( !strcasecmp( p->Name, Name ) ){
            p->ProcButtOn = ProcButtOn;
            p->ProcButtOff = ProcButtOff;
            p->ProcButtPress = ProcButtPress;
            p->ProcButtRelease = ProcButtRelease;
            p->Script = script;
            return 1;
        }
    }    
    return 0;
}

int WinAddButtonRightProc( char *Name, Intp_t *Scr, int ProcRtButtOn, int ProcRtButtOff )
{
    int i;
    Win03_t *p;

    if( gWinCurrent == -1 || !gWinList[ gWinCurrent ].Buttons ) return 0;    
    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
        p = &gWinList[ gWinCurrent ].Buttons[ i ];
        if( !strcasecmp( p->Name, Name ) ){
            p->ProcRtButtOff = ProcRtButtOff;
            p->ProcRtButtOn = ProcRtButtOn;
            p->Script = Scr;
            return 1;
        }
    }
    return 0;
}

int WinUnk190( char *Name, int (*a2)(int, int, int), int a3 )
{
    int i;
    Win03_t *p;

    if( gWinCurrent == -1 || !gWinList[ gWinCurrent ].Buttons ) return 0;    
    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
        p = &gWinList[ gWinCurrent ].Buttons[ i ];
        if( !strcasecmp( p->Name, Name ) ){
            p->i30 = a3;
            p->i28 = (void *)a2;
            return 1;
        }
    }    
    return 0;
}

int WinUnk191( char *Name, int (*a2)(), int a3 )
{
    int i;
    Win03_t *p;

    if( gWinCurrent == -1 || !gWinList[ gWinCurrent ].Buttons ) return 0;    
    for( i = 0; i < gWinList[ gWinCurrent ].Cnt; i++ ){
        p = &gWinList[ gWinCurrent ].Buttons[ i ];
        if( !strcasecmp( p->Name, Name ) ){
            p->i29 = a2;
            p->i31 = a3;
            WinSetHandler2( p[ i ].BtId, -1, -1, WinUnk26, WinUnk29 );
            return 1;
        }
    }
    return 0;
}

int WinAddButtonText( char *Name, char *Text )
{
    return WinUnk105( Name, Text, 2, 2, 0, 0 );
}

int WinUnk105( char *Name, char *Text, int bx, int by, int ax, int ay )
{
    Win03_t *p;
    char *ptr;
    int i, h, w, pitch, x, y;

    if( gWinCurrent == -1 || !gWinList[gWinCurrent].Buttons ) return 0;    
    for( i = 0; i < gWinList[gWinCurrent].Cnt; i++ ){
        p = &gWinList[ gWinCurrent ].Buttons[ i ];
        if( !strcasecmp( p->Name, Name ) ){
            h = gFont.ChrHeight() + 1;
            w = gFont.LineWidth( Text ) + 1;
            ptr = dbg_malloc( h * w );
            x = (p->Width - w) / 2 + ax;
            y = (p->Height - h) / 2 + ay;
            pitch = w;
            if( x < 0 ){ w -= x; x = 0; }
            if( w + x >= p->Width ) w = p->Width - x;
            if( y < 0 ){ h -= y; y = 0; }
            if( h + y >= p->Height ) h = p->Height - y;
            if( p->UppPix )
                ScrCopy( &(p->UppPix)[y * p->Width + x], w, h, p->Width, ptr, w );
            else
                memset( ptr, 0, h * w );            
            gFont.Print( ptr, Text, w, w, WinGetTextColor() + WinGetTextFlags() );
            ScrCopyAlpha( ptr, w, h, pitch, p->UppPix + y * p->Width + x, p->Width );

            w = gFont.LineWidth( Text ) + 1;
            h = gFont.ChrHeight() + 1;
            x = bx + (p->Width - w) / 2;
            y = by + (p->Height - h) / 2;
            pitch = w;
            if( x < 0 ){ w -= x; x = 0; }
            if( w + x >= p->Width ) w = p->Width - x;
            if( y < 0 ){ h -= y; y = 0; }
            if( h + y >= p->Height ) h = p->Height - y;
            if( p->DownPix )
                ScrCopy( &p->DownPix[y * p->Width + x], w, h, p->Width, ptr, w );
            else
                memset( ptr, 0, h * w );
            gFont.Print( ptr, Text, w, w, WinGetTextColor() + WinGetTextFlags() );
            ScrCopyAlpha( ptr, w, h, pitch, p->DownPix + y * p->Width + x, p->Width );
            dbg_free( ptr );
            if( p->WgFlags & 0x20 ) WinSetDefaultImg( p->BtId, p->UppPix );
            WinSetAnimL( p->BtId, p->UppPix, p->DownPix, p->OverPix, 0 );
            return 1;
        }
    }    
    return 0;
}

int WinFill( float fr, float fg, float fb )
{    
    WinDrawFilledRect( 
	gWinList[ gWinCurrent ].WinId, 
	0, 0, WinGetCurrentWidth(), WinGetCurrentHeight(), 
	gPalColorCubeRGB[ lround( 31.0 * fr ) ][ lround( 31.0 * fg ) ][ lround( 31.0 * fb ) ] 
    );
    return 1;
}

int WinFillArea( int xpos, int ypos, int width, int height, float r, float g, float b )
{
    WinDrawFilledRect( 
	gWinList[ gWinCurrent ].WinId, 
	lround( xpos * gWinList[ gWinCurrent ].ScaleX), lround( ypos * gWinList[ gWinCurrent ].ScaleY), 
	lround( width * gWinList[ gWinCurrent ].ScaleX), lround( height * gWinList[ gWinCurrent ].ScaleY), 
	gPalColorCubeRGB[ lround( 31.0 * r ) ][ lround( 31.0 * g ) ][ lround( 31.0 * b ) ]
    );
    return 1;
}

void WinSetRegion()
{
    Region01_t *p01;

    p01 = gWinList[ gWinCurrent ].Regions[ gWinList[ gWinCurrent ].RegionId ]->p01;
    WinUnk193( p01->i01, p01->i02, 0 );
    RegionUnk01( gWinList[ gWinCurrent ].Regions[ gWinList[ gWinCurrent ].RegionId ] );
}

void *WinUnk195( char *Name )
{
    int i;

    if( gWinCurrent == -1 ) return 0;
    for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++ ){
        if( !strcasecmp( RegionGetName( gWinList[ gWinCurrent ].Regions[ i ] ), Name ) ) return RegionGetUserData( gWinList[ gWinCurrent ].Regions[ i ]);
    }
    return 0;
}

void WinUnk196( char *Name, void *pUser )
{
    int i;

    if( gWinCurrent == -1 ) return;
    for( i = 0; i < gWinList[gWinCurrent].RegionCnt; i++ ){
        if( !strcasecmp( RegionGetName( gWinList[ gWinCurrent ].Regions[ i ] ), Name) ){
            RegionSetUserData( gWinList[ gWinCurrent ].Regions[ i ], pUser );
            return;
        }
    }    
}

int WinCheckRegion( char *Name )
{
    int i;

    if( gWinCurrent == -1 ) return 0;
    if( gWinList[ gWinCurrent ].WinId == -1 ) return 0;    
    for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++ ){
        if( gWinList[ gWinCurrent ].Regions[ i ] ){
            if( !strcasecmp( RegionGetName( gWinList[ gWinCurrent ].Regions[ i ] ), Name ) ) return 1;
        }
    }    
    return 0;
}

int WinUnk106( int RegIdx )
{
    int i, RegionCnt;
    Region_t **Regions;

    if( gWinCurrent == -1 ) return 0;
    if( gWinList[ gWinCurrent ].Regions ){        
        Regions = gWinList[ gWinCurrent ].Regions;
        for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++, Regions++ ){
            if( !*Regions ) break;            
        }
        RegionCnt = gWinList[ gWinCurrent ].RegionCnt;
        if( i == RegionCnt ){
            gWinList[ gWinCurrent ].Regions = dbg_realloc( gWinList[ gWinCurrent ].Regions, sizeof( Region_t ** ) * (i + 1) );
            gWinList[ gWinCurrent ].RegionCnt++;
        }
    } else {
        gWinList[ gWinCurrent ].Regions = dbg_malloc( sizeof(Region_t **) );
        gWinList[ gWinCurrent ].RegionCnt = 1;
        i = 0;
    }
    if( RegIdx )
	gWinList[ gWinCurrent ].Regions[ i ] = RegionInit( RegIdx + 1 );
    else 
	gWinList[ gWinCurrent ].Regions[ i ] = NULL;
    gWinList[ gWinCurrent ].RegionId = i;
    return 1;
}

int WinUnk193( int a1, int a2, int a3 )
{
    int n;

    if( gWinCurrent == -1 ) return 0;
    n = gWinList[ gWinCurrent ].RegionId;
    if( !gWinList[ gWinCurrent ].Regions[ n ] ) gWinList[ gWinCurrent ].Regions[ n ] = RegionInit( 1 );
    if( a3 ){
        a1 = lround( a1 * gWinList[ gWinCurrent ].ScaleX );
        a2 = lround( a2 * gWinList[ gWinCurrent ].ScaleY );
    }
    RegionAddPoint( gWinList[ gWinCurrent ].Regions[ n ], a1, a2 );
    return 1;
}

int WinUnk197( int a1, int a2, int a3, int a4, int a5 )
{

    WinUnk193( a1, a2, a5 );
    WinUnk193( a3, a2, a5 );
    WinUnk193( a3, a4, a5 );
    WinUnk193( a1, a4, a5 );
    return 0;
}

int WinUnk198( char *Name, int (*Cb)(void *, int, int), int CbArg )
{
    int i;

    if( gWinCurrent == -1 ) return 0;    
    for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++ ){
        if( !gWinList[ gWinCurrent ].Regions[ i ] ) continue;
    	if( !strcasecmp( gWinList[ gWinCurrent ].Regions[ i ]->Name, Name ) ){
    	    gWinList[ gWinCurrent ].Regions[ i ]->i31 = Cb;
    	    gWinList[ gWinCurrent ].Regions[ i ]->i33 = CbArg;
    	    return 1;
    	}
    }
    return 0;
}

int WinUnk199( char *Name, int (*Cb)(void *, int, int), int CbArg )
{
    int i;

    if( gWinCurrent == -1 ) return 0;
    for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++ ){
        if( gWinList[ gWinCurrent ].Regions[ i ] ){
    	    if( !strcasecmp( gWinList[ gWinCurrent ].Regions[ i ]->Name, Name ) ){
        	gWinList[ gWinCurrent ].Regions[ i ]->i32 = Cb;
        	gWinList[ gWinCurrent ].Regions[ i ]->i34 = CbArg;
        	return 1;
    	    }
        }
    }
    return 0;
}

int WinRegionSetProcedure( char *Name, Intp_t *Script, int ProcOn, int ProcOff, int ProcPress, int ProcRelease )
{
    int i;

    if( gWinCurrent == -1 ) return 0;
    for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++ ){
        if( gWinList[ gWinCurrent ].Regions[ i ] ){
    	    if( !strcasecmp( gWinList[ gWinCurrent ].Regions[ i ]->Name, Name ) ){
        	gWinList[ gWinCurrent ].Regions[ i ]->i21 = ProcOn;
        	gWinList[ gWinCurrent ].Regions[ i ]->i22 = ProcOff;
        	gWinList[ gWinCurrent ].Regions[ i ]->i19 = ProcPress;
        	gWinList[ gWinCurrent ].Regions[ i ]->i20 = ProcRelease;
        	gWinList[ gWinCurrent ].Regions[ i ]->Script = Script;
        	return 1;
    	    }
        }
    }
    return 0;
}

int WinRightButtonSetProcedure( char *Name, Intp_t *Script, int a3, int a4 )
{
    int i;

    if( gWinCurrent == -1 ) return 0;
    for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++ ){
        if( gWinList[ gWinCurrent ].Regions[ i ] ){
    	    if( !strcasecmp( gWinList[ gWinCurrent ].Regions[ i ]->Name, Name ) ){
    		gWinList[ gWinCurrent ].Regions[ i ]->i23 = a3;
        	gWinList[ gWinCurrent ].Regions[ i ]->i24 = a4;
        	gWinList[ gWinCurrent ].Regions[ i ]->Script = Script;
    		return 1;
    	    }
        }
    }
    return 0;
}

int WinAddRegionFlag( char *Name, int Flags )
{
    int i;

    if( gWinCurrent == -1 ) return 0;
    for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++ ){
        if( gWinList[ gWinCurrent ].Regions[ i ] ){
    	    if( !strcasecmp( gWinList[ gWinCurrent ].Regions[ i ]->Name, Name ) ){
        	RegionSetFlags( gWinList[ gWinCurrent ].Regions[ i ], Flags );
        	return 1;
    	    }
        }
    }
    return 0;
}

int WinSetRegionA( char *a1 )
{
    int k, i;

    if( gWinCurrent == -1 ) return 0;
    k = gWinList[ gWinCurrent ].RegionId;
    if( !gWinList[ gWinCurrent ].Regions[ k ] ) return 0;
    for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++ ){
        if( i != k ){
            if( gWinList[ gWinCurrent ].Regions[ i ] ){
                if( !strcasecmp( RegionGetName( gWinList[ gWinCurrent ].Regions[ i ] ), a1 ) ){
                    RegionDelete( gWinList[ gWinCurrent ].Regions[ i ] );
                    gWinList[ gWinCurrent ].Regions[ i ] = 0;
                    break;
                }
            }
        }
    }
    RegionAddName( gWinList[ gWinCurrent ].Regions[ k ], a1 );
    return 1;
}

int WinDeleteRegion( char *Name )
{
    int i;

    if( gWinCurrent == -1 || gWinList[gWinCurrent].WinId == -1 ) return 0;
    if( Name ){
        for( i = 0; i < gWinList[gWinCurrent].RegionCnt; i++ ){
            if( gWinList[ gWinCurrent ].Regions[ i ] ){
                if( !strcasecmp( RegionGetName( gWinList[ gWinCurrent ].Regions[ i ] ), Name ) ){
                    RegionDelete( gWinList[ gWinCurrent ].Regions[ i ] );
                    gWinList[ gWinCurrent ].Regions[ i ] = 0;
                    gWinList[ gWinCurrent ].Region++;
                    return i != gWinList[ gWinCurrent ].RegionCnt;
                }
            }
        }
        return i != gWinList[ gWinCurrent ].RegionCnt;
    } else {
        if( gWinList[ gWinCurrent ].Regions++ ){
            for( i = 0; i < gWinList[ gWinCurrent ].RegionCnt; i++ ){
                if( gWinList[ gWinCurrent ].Regions[ i ] ) RegionDelete( gWinList[ gWinCurrent ].Regions[ i ] );
            }
            dbg_free( gWinList[ gWinCurrent ].Regions );
            gWinList[ gWinCurrent ].Regions = 0;
            gWinList[ gWinCurrent ].RegionCnt = 0;
        }
        return 1;
    }
}

void WinRun()
{
    MvePlay();
    MouseMgrAnimate();
    WinMouseHandle();
    WidgetUnk13();
}

int WinPlay()
{
    return MveIsPlaying();
}

int WinSetMovieFlags( char a1 )
{
    return MveSetFlags( a1 ) == 0;
}

int WinPlayStart( char *a1 )
{
    return MveStart( gWinList[ gWinCurrent ].WinId, a1 ) == 0;
}

int WinPlayAt( char *eax0, int a1, int a2, int a3, int a5 )
{
    return MveStartAt( gWinList[ gWinCurrent ].WinId, eax0, a1, a2, a3, a5 ) == 0;
}

void WinPlayAbort()
{
    MveAbort();
}

void WinUnk200( char *pDst, int DstPitch, int a3, char *pSrc, int SrcPitch, int a6 )
{
    char *src;
    int v7,v8,v9,v10,v11,v13,Width,Height;

    src = pSrc;
    v7 = DstPitch / 2;
    v13 = DstPitch / 2;
    Height = a6;
    Width = SrcPitch;
    if( DstPitch / 2 - SrcPitch / 2 < 0 ){
        v8 = SrcPitch / 2 - v7;
        src = &pSrc[ v8 ];
        Width = SrcPitch - v8;
    }
    v9 = v7 + SrcPitch / 2;
    if( v9 >= DstPitch ) Width += DstPitch - 4 - v9;
    if( v13 - a6 / 2 < 0 ){
        v10 = a6 / 2 - v13;
        src += SrcPitch * v10;
        Height = a6 - v10;
    }
    v11 = v13 + a6 / 2;
    if( v11 >= a3 ) Height += a3 - v11;
    ScrCopy( src, Width, Height, SrcPitch, pDst, DstPitch );
}

void WinUnk70( char *a1, int a2, int a3, int a4, char *pSrc, int Width, int a7, int SrcPitch )
{
    char *v8, *v14, *v18, *v23;
    int v10,v11,v12,j,v15,v16,k,v19,v20,v21,i,v24,v25,v27,v28,v29,v30;

    v8 = a1;
    if( a2 == Width && a7 == a3 ){
        ScrCopy( pSrc, Width, a3, SrcPitch, a1, a4 );
    } else {
        v25 = a4 - a2;
        v28 = ((Width << 16) / a2) >> 16;
        v10 = ((a7 << 16) / a3) >> 16;
        v27 = (unsigned short)((Width << 16) / a2);
        v24 = (unsigned short)((a7 << 16) / a3);
        v20 = v10 * SrcPitch;
        if( v20 ){
            v21 = 0;
            for( i = 0; v21 < a3; v21++ ){
                v11 = 0;
                v12 = 0;
                for( j = 0; v12 < a2; v12++ ){
                    *v8++ = pSrc[j];
                    v11 += v27;
                    j += v28;
                    if( v11 >= 0x10000 ){
                        ++j;
                        v11 = (unsigned short)v11;
                    }
                }
                v14 = &pSrc[v20];
                v8 += v25;
                pSrc += v20;
                i += v24;
                if( i >= 0x10000 ){
                    i &= 0xffff;
                    pSrc = &v14[SrcPitch];
                }
            }
        } else {
            v29 = v10 * SrcPitch;
            v30 = v10 * SrcPitch;
            if( a3 > 0 ){
                do{
                    v23 = v8;
                    v15 = 0;
                    v16 = 0;
                    for( k = 0; k < a2; k++ ){
                        *v8++ = pSrc[v15];
                        v16 += v27;
                        v15 += v28;
                        if( v16 >= 0x10000 ){
                            ++v15;
                            v16 = (unsigned short)v16;
                        }
                    }
                    if( ++v29 < a3 ){
                        v8 += v25;
                        v30 += v24;
                        if( v30 < 0x10000 ){
                            do{
                                v18 = v23;
                                v19 = a2 & 3;
                                if( (a2 & 0xFFFFFFFC) != 0 ){
                                    memcpy(v8, v23, 4 * ((unsigned int)a2 >> 2));
                                    v18 = &v23[4 * ((unsigned int)a2 >> 2)];
                                    v8 += 4 * ((unsigned int)a2 >> 2);
                                }
                                if( (a2 & 3) != 0 ){
                                    memcpy(v8, v18, v19);
                                    v8 += v19;
                                }
                                v8 += v25;
                                v30 += v24;
                                ++v29;
                            }
                            while ( v29 < a3 && v30 < 0x10000 );
                        }
                        v30 &= 0xffff;
                        pSrc += SrcPitch;
                    }
                } while ( v29 < a3 );
            }
        }
    }
}

int WinResizeArt( char *a1, int a2, int a3, char *a4, int a5, int a6 )
{
    char *v18,*v22,*v27,*v14;
    int result, v9,v10,v11,v12,i,v15,v16,j,v19,v20,v21,v23,v25,v26,v28,v29;

    v27 = a4;
    if( a2 == a5 && a6 == a3 ){
        result = a5 * a6;
        memcpy( a1, a4, a5 * a6 );
    } else {
        v26 = ((a5 << 16) / a2) >> 16;
        v9 = ((a6 << 16) / a3) >> 16;
        result = (unsigned short)((a6 << 16) / a3);
        v25 = (unsigned short)((a5 << 16) / a2);
        v23 = result;
        v20 = v9 * a5;
        if( v20 ){
            v10 = 0;
            result = a3;
            v21 = 0;
            if( a3 > 0 ){
                do{
                    v11 = 0;
                    v12 = 0;
                    for( i = 0; v12 < a2; v12++ ){
                        *a1 = v27[i];
                        i += v26;
                        v11 += v25;
                        ++a1;
                        if( v11 >= 0x10000 ){
                            ++i;
                            v11 = (unsigned short)v11;
                        }
                    }
                    v14 = &v27[v20];
                    v10 += v23;
                    v27 += v20;
                    if( v10 >= 0x10000 ){
                        v10 = (unsigned short)v10;
                        v27 = &v14[a5];
                    }
                    result = v21 + 1;
                    v21 = result;
                }while ( result < a3 );
            }
        } else {
            v28 = v9 * a5;
            v29 = v9 * a5;
            if( a3 > 0 ){
                do{
                    v22 = a1;
                    v15 = 0;
                    v16 = 0;
                    for( j = 0; j < a2; j++ ){
                        *a1++ = v27[v15];
                        v16 += v25;
                        v15 += v26;
                        if( v16 >= 0x10000 ){
                            ++v15;
                            v16 = (unsigned short)v16;
                        }
                    }
                    if( ++v28 < a3 ){
                        v29 += v23;
                        if( v29 < 0x10000 ){
                            do{
                                v18 = v22;
                                v19 = a2 & 3;
                                if( (a2 & 0xFFFFFFFC) != 0 ){
                                    memcpy(a1, v22, 4 * ((unsigned int)a2 >> 2));
                                    v18 = &v22[4 * ((unsigned int)a2 >> 2)];
                                    a1 += 4 * ((unsigned int)a2 >> 2);
                                }
                                if( (a2 & 3) != 0 ){
                                    memcpy(a1, v18, v19);
                                    a1 += v19;
                                }
                                v29 += v23;
                                v28++;
                            }while( v28 < a3 && v29 < 0x10000 );
                        }
                        v29 &= 0xffff;
                        v27 += a5;
                    }
                    result = v28;
                }while ( v28 < a3 );
            }
        }
    }
    return result;
}

void WinUnk201( char *a1, int a2, int a3, int a4, unsigned char *a5, unsigned char *a6, char *a7, int a8 )
{
    int i, j;
    unsigned short v24;

    for( ;--a3 != -1; a1 += a4 - a2, a7 += a8 - a2 ){
        for( i = 0; i < a2; i += v24, a1 += v24, a7 += v24, a5 += v24, a6 += 2 ){
            v24 = a6[1] + (a6[0] << 8);
            if( !(v24 & 0x8000) ){
                if( v24 & 0x4000 ){
                    v24 &= ~0x4000;
                    memcpy( a7, a1, v24 );
                } else {
                    for( j = 0; j < v24; j++ ){
                        a7[ j ] = gPalColorCubeRGB
			    [ gWinUnk209[ ( gPalBase[ (int)a5[ j ] ].r << 8 ) | ~a6[ j + 2 ] ] + gWinUnk209[ ( gPalBase[ (int)a1[ j ] ].r << 8 ) | a6[ j + 2  ] ] ]
			    [ gWinUnk209[ ( gPalBase[ (int)a5[ j ] ].g << 8 ) | ~a6[ j + 2 ] ] + gWinUnk209[ ( gPalBase[ (int)a1[ j ] ].g << 8 ) | a6[ j + 2  ] ] ]
			    [ gWinUnk209[ ( gPalBase[ (int)a5[ j ] ].b << 8 ) | ~a6[ j + 2 ] ] + gWinUnk209[ ( gPalBase[ (int)a1[ j ] ].b << 8 ) | a6[ j + 2  ] ] ];
                    }
                    a6 += v24;
                    if( v24 & 0x01 ) a6++;
                }
            } else {
                v24 &= ~0x8000;
            }            
        }        
    }
}

int WinUnk202( char *a1, int a2, char *a3, char *a4, char *a5, int a6, int *a7 )
{
    char *v38,*v41,*v43,*i,*v9;
    unsigned short v10,v12,v29,v60,v61;
    int v7,j,result,v13,v14,v15,v16,v21,v39,v40,k,n,v56,v58;

    v40 = a7[2] - *a7 + 1;
    v7 = a2 * a7[1];    
    v43 = v7 + a1;
    v41 = v7 + a3;
    i = a6 * a7[1] + a5;
    for( v56 = 0; v56 < a7[1]; v56++ ){
        for( j = 0; j < a2; j += v10 ){
            v9 = a4 + 2;
            v10 = a4[1] + (a4[0] << 8);
            a4 += 2;
            if( v10 & 0xC000 ){
                v10 &= 0x3F00;
            } else {
                a4 = v9 + v10;
                if( v10 & 0x01 ) a4++;
            }
        }
    }
    for( ; ; v56++ ){
        result = v56;
        if( v56 >= a7[3] ) break;
        for( k = 0; k < a7[0]; i += v61, k += v61, v43 += v61, v41 += v61 ){
            v12 = a4[1] + (a4[0] << 8);
            v61 = v12;
            a4 += 2;
            if( !(v12 & 0x8000) ){
                if( v12 & 0x4000 ){
                    v61 &= 0xBF00;
                    v13 = a7[0];
                    v14 = k + v61;
                    if( v14 > a7[0] ){
                        v15 = v14 - v13;
                        v16 = v13 - k;
                        if( v15 > v40 ) v15 = v40;
                        memcpy( &i[v16], &v43[v16], v15 );
                    }
                } else {
                    if( (k + v12) > a7[0] ){
                        v21 = k + v12 - a7[0];
                        if( v21 > v40 ) v21 = v40;
                        for( n = 0; n < v21; n++ ){
                            i[ n ] = gPalColorCubeRGB
                    	    [ gWinUnk209[ ( gPalBase[ (int)v41[ n ] ].r << 8 ) | ( ~a4[ n ] & 0xff ) ] + gWinUnk209[ ( gPalBase[ (int)v43[ n ] ].r << 8 ) | ( a4[ n ] & 0xff ) ] ]
                    	    [ gWinUnk209[ ( gPalBase[ (int)v41[ n ] ].g << 8 ) | ( ~a4[ n ] & 0xff ) ] + gWinUnk209[ ( gPalBase[ (int)v43[ n ] ].g << 8 ) | ( a4[ n ] & 0xff ) ] ]
                    	    [ gWinUnk209[ ( gPalBase[ (int)v41[ n ] ].b << 8 ) | ( ~a4[ n ] & 0xff ) ] + gWinUnk209[ ( gPalBase[ (int)v43[ n ] ].b << 8 ) | ( a4[ n ] & 0xff ) ] ];
                        }
                    }
                    a4 += v61;
                    if( v61 & 0x01 ) a4++;
                }
            } else {
                v61 &= 0x7F00;
            }            
        }
        for( ;k < a7[2]; v43 += v60, i += v60, v41 += v60, k += v60 ){
            v29 = a4[1] + (a4[0] << 8);
            v60 = v29;
            a4 += 2;
            if( !(v29 & 0x8000) ){
                if( v29 & 0x4000 ){
                    v60 &= 0xBF00;
                    if( k + v60 <= a7[2] )
                        memcpy( i, v43, v60 );
                    else
                        memcpy( i, v43, a7[2] - k );
                } else {
                    v58 = ( v29 + k > a7[2] ) ? (a7[2] - k) : v29;
                    for( n = 0; n < v58; n++ ){
                        i[ n ] = gPalColorCubeRGB
                        [ gWinUnk209[ (gPalBase[ (int)v41[ n ] ].r << 8) | ~(a4[ n ] & 0xff) ] + gWinUnk209[ (gPalBase[ (int)v43[ n ] ].r << 8) | (a4[ n ] & 0xff) ] ]
                        [ gWinUnk209[ (gPalBase[ (int)v41[ n ] ].g << 8) | ~(a4[ n ] & 0xff) ] + gWinUnk209[ (gPalBase[ (int)v43[ n ] ].g << 8) | (a4[ n ] & 0xff) ] ]
                        [ gWinUnk209[ (gPalBase[ (int)v41[ n ] ].b << 8) | ~(a4[ n ] & 0xff) ] + gWinUnk209[ (gPalBase[ (int)v43[ n ] ].b << 8) | (a4[ n ] & 0xff) ] ];
                    }
                    a4 += v60;
                    if( v60 & 0x01 ) a4++;
                }
            } else {
                v60 &= 0x7F00;
            }            
        }
        v43 += a2 - k;
        v41 += a2 - k;
        for( i += a6 - k; k < a2; k += v39 ){
            v38 = a4 + 2;
            v39 = (a4[0] << 8) + a4[1];
            a4 += 2;
            if( !(v39 & 0xC000) ){
                a4 = v38 + v39;
                if( v39 & 0x01 ) a4++;
            }
            v39 &= 0x3F00;
        }
        
    }
    return result;
}

void WinBlit( char *data, int SrcW, int SrcH, char *surf, int DstW, int DstH )
{
    int i, tmp, j, w, h;
    char *pSrc;

    w = SrcW / 3;
    h = SrcH / 3;    
    pSrc = &data[ SrcW * (SrcH / 3) + SrcW / 3 ];
    for( j = 0; j < DstW; j += w ){
        for( i = 0; i < DstH; i += h ){
            tmp = i + h;
            if( tmp >= DstH ) tmp = DstH;
            ScrCopy( pSrc, ( w + j < DstW ) ? w : (DstW - j), tmp - i, SrcW, &surf[ DstW * i + j ], DstW );
        }
    }
    for( i = 0; i < DstW; i += w ){
        tmp = w + i;
        if( tmp >= DstW ) tmp = DstW;
        ScrCopy( &data[ w ], tmp - i, ( h >= DstH ) ? DstH : h, SrcW, &surf[ i ], DstW );
        ScrCopy( &data[ SrcW * 2 * h + w ], tmp - i, h, SrcW, &surf[ DstW * (DstH - h) + i ], DstW );
    }
    for( i = 0; DstH > i; i += h ){
        tmp = h + i;
        if( tmp >= DstH ) tmp = DstH;
        ScrCopy( &data[ SrcW * h ], ( w < DstW ) ? w : DstW, tmp - i, SrcW, &surf[ DstW * i ], DstW );
        ScrCopy( &data[ 2 * w + SrcW * h ], w, tmp - i, SrcW, &surf[ DstW * i + DstW - w ], DstW );
    }
    tmp = ( tmp >= DstH ) ? DstH : h;
    ScrCopy( data, ( w < DstW ) ? w : DstW, tmp, SrcW, surf, DstW );
    ScrCopy( &data[ 2 * w ], w, tmp, SrcW, &surf[ DstW - w ], DstW );
    tmp = ( tmp >= DstW ) ? DstW : w;
    ScrCopy( &data[ SrcW * 2 * h ], tmp, h, SrcW, &surf[ DstW * (DstH - h) ], DstW );
    ScrCopy( &data[ 2 * w + SrcW * 2 * h ], w, h, SrcW, &surf[ DstW * (DstH - h) + DstW - w ], DstW );
}

void WinUnk203()
{
    gWinUnk104 = 1;
}

void WinUnk204()
{
    gWinUnk104 = 0;
}

void WinUnk205( int a1 )
{
    gWinUnk200 = a1;
}

int WinUnk206( int a1, int a2, int a3, int a4, int Wrap, int Flags, char a7 )
{
    if( gWinCurrent == -1 || gWinList[ gWinCurrent ].WinId == -1 ) return -1;
    return WidgetMsgNew( gWinList[ gWinCurrent ].WinId, a1, a2, a3, a4, Wrap, Flags, a7 );
}

int WinDrawTextA( int Idx, char *str )
{
    return WidgetDrawText1( Idx, str );
}

int WinLabelMouseDefine( int MsgId )
{
    return WidgetLabelMouseDefine( MsgId );
}

int WinUnk207( int a1 )
{
    return WidgetUnk06( a1 );
}

int WinUnk208( int a1, int a2, int a3, int a4, unsigned char a5 )
{
    return WidgetUnk08( a1, a2, a3, a4, a5 );
}

int WinUnk209( int a1, char *a2, int a3, void *a4 )
{
    return WidgetUnk01( a1, a2, a3, a4 );
}

int WinUnk210( int Idx )
{
    if( Idx != -1 ) return WidgetEvExe( Idx );
    if( gWinCurrent == -1 || gWinList[ gWinCurrent ].WinId == -1 ) return 0;
    return WidgetEvProcess( gWinList[ gWinCurrent ].WinId );
}

int WinTextWrap( char *Str, int LineWidth, short *pOffs, short *pLines )
{
    unsigned int LinePix, dist, i;
    char *n, *s;

    *pLines = 1;
    pOffs[ 0 ] = 0;
    for( i = 1; i < 64; i++ ) pOffs[ i ] = -1;
    
    if( gFont.MaxWidth() > LineWidth ) return -1;
    if( gFont.LineWidth( Str ) < LineWidth ){ pOffs[ (*pLines)++ ] = strlen( Str ); return 0; }    
    dist = gFont.Distance();
    LinePix = 0;
    n = NULL;
    for( s = Str; *s; s++ ){
        LinePix += dist + gFont.ChrWidth( *s );
        if( LinePix <= LineWidth ){
            if( ( IsTable[ *s + 1 ] & 0x02 ) || *s == '-' ) n = s; 
        } else {
            if( *pLines >= 64 ) return -1;
            if( n ){
                pOffs[ *pLines ] = n - Str + 1;
                s = n;
            } else {
        	s--;
                pOffs[ *pLines ] = s - Str + 1;
            }
            (*pLines)++;
            LinePix = 0;
            n = NULL;
        }
    }
    if( *pLines >= 64 ) return -1;
    pOffs[ *pLines ] = s - Str + 1;
    (*pLines)++;
    return 0;
}

