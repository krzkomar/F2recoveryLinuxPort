#include "FrameWork.h"

char *(*gMouseMgrHandlerCb)( char * ) = MouseMgrHandlerDefaultCb;
int  (*gMouseMgrGetTimeResCb)( void ) = MouseMgrTimeResDefaultCb;
int  (*gMouseMgrGetTimeCb)( void ) = MouseMgrGetTime;
int gMouseMgrCnt = 1;

MseMgr_t gMouseMgrCache[ 32 ];
int gMouseMgrAnimating;
Pal8_t *gMouseMgrCurPal;
MseMgrCursor_t *gMouseMgrCurAnim;
MseMgrCursor_t *gMouseMgrCurStat;
int gMouseMgrCurrentIdx;

/*****************************************************************/

char *MouseMgrHandlerDefaultCb( char *fname )
{
    /* dummy callback function */
    return fname;
}

int MouseMgrTimeResDefaultCb()
{
    return 1000;
}

int MouseMgrGetTime()
{
    return TimerGetSysTime();
}

int MouseMgrSetCursor( char *Data, int w, int h, int pitch, int x0, int y0, char alpha )
{
    return MseSetStaticCursor( Data, w, h, pitch, x0, y0, alpha );
}

void MouseMgrSetHandler( char *(*HandlerCb)( char *) )
{
    gMouseMgrHandlerCb = HandlerCb;
}

void MouseMgrSetupTime( int (*TimeResCb)(void), int (*TimeGetCb)(void) )
{
    gMouseMgrGetTimeResCb = ( TimeResCb ) ? TimeResCb : MouseMgrTimeResDefaultCb;
    gMouseMgrGetTimeCb = TimeGetCb ? TimeGetCb : MouseMgrGetTime;
}

void MouseMgrDelCache( MseMgr_t *mse )
{
    int i;

    if( mse->Type == M_STATIC ){
        if( mse->Cursor ){
            if( mse->Cursor->stat.Data ){
                dbg_free( mse->Cursor->stat.Data );
                mse->Cursor->stat.Data = NULL;
            }
            dbg_free( mse->Cursor );
            mse->Cursor = NULL;
        }
    } else if( mse->Type == M_ANIMATED && mse->Cursor ){
        if( mse->Cursor->anim.Data ){
            for( i = 0; i < mse->Cursor->anim.cnt2; i++ ){
                dbg_free( &mse->Cursor->anim.Data[ i ] );
                dbg_free( &mse->Cursor->anim.bmp[ i ] );
            }
            dbg_free( mse->Cursor->anim.Data );
            dbg_free( mse->Cursor->anim.bmp );
            dbg_free( mse->Cursor->anim.CentX );
            dbg_free( mse->Cursor->anim.CentY );
        }
        dbg_free( mse->Cursor );
        mse->Cursor = NULL;
    }
    mse->Type = M_UNSPEC;
    mse->Name1[ 0 ] = '\0';
}

void MouseMgrFlushCache()
{
    int i;

    for( i = 0; i < 32; i++ ) MouseMgrDelCache( &gMouseMgrCache[ i ] );
}

int MouseMgrInsertCache( MseMgrCursor_t **Cursor, int Type, Pal8_t *Palette, char *Name )
{
    int i, n, k;

    n = -1;
    for( i = 0; i < 32; i++ ){
	if( !strcasecmp( Name, gMouseMgrCache[ i ].Name1 ) ){
	    MouseMgrDelCache( &gMouseMgrCache[ i ] );
	    n = i;
	    break;
	}
    }
    if( n != -1 ) i = n;
    if( i == 32 ){
        n = -1;
        k = gMouseMgrCnt;
        for( i = 0; i < 32; i++ ){
            if( k > gMouseMgrCache[ i ].i01 ){
                k = gMouseMgrCache[ i ].i01;
                n = i;
            }
        }
        if( n == -1 ){ eprintf( "Mouse cache overflow!!!!\n" ); exit( 1 ); }
        i = n;
        MouseMgrDelCache( &gMouseMgrCache[ i ] );
    }
    gMouseMgrCache[ i ].Type = Type;
    memcpy( gMouseMgrCache[ i ].Pal, Palette, sizeof( Pal8_t ) );
    gMouseMgrCache[ i ].i01 = gMouseMgrCnt;
    gMouseMgrCnt++;
    strncpy( gMouseMgrCache[ i ].Name1, Name, 31 );
    gMouseMgrCache[ i ].Name2[ 0 ] = '\0';
    gMouseMgrCache[ i ].Cursor = *Cursor;
    return i;
}

MseMgr_t *MouseMgrGetCache( char *fname, Pal8_t **pPal, int *pCentX, int *pCentY, int *pWidth, int *pHeight, int *Type )
{
    int i;

    for( i = 0; i < 32; i++ ){
        if( strncasecmp( gMouseMgrCache[ i ].Name1, fname, 31 ) && strncasecmp( gMouseMgrCache[ i ].Name2, fname, 31 ) ) continue;
	*pPal = gMouseMgrCache[ i ].Pal;
	gMouseMgrCurrentIdx = i;
	*Type = gMouseMgrCache[ i ].Type;
	if( *Type == M_STATIC ){
    	    *pWidth = gMouseMgrCache[ i ].Cursor->stat.Width;
    	    *pHeight = gMouseMgrCache[ i ].Cursor->stat.Height;
    	    *pCentX = gMouseMgrCache[ i ].Cursor->stat.CentX;
    	    *pCentY = gMouseMgrCache[ i ].Cursor->stat.CentY;
	} else if( *Type == M_ANIMATED ){
    	    *pWidth = gMouseMgrCache[ i ].Cursor->anim.Width;
    	    *pHeight = gMouseMgrCache[ i ].Cursor->anim.Height;
    	    *pCentX = gMouseMgrCache[ i ].Cursor->anim.CentX[ gMouseMgrCache[ i ].Cursor->anim.cnt3 ];
    	    *pCentY = gMouseMgrCache[ i ].Cursor->anim.CentY[ gMouseMgrCache[ i ].Cursor->anim.cnt3 ];
	}
	return &gMouseMgrCache[ i ];
    }
    return NULL;
}

void MouseMgrResetSpeed()
{
    MseSetSpeed( 1.0 );
}

void MouseMgrFree()
{
    int i;

    MseSetStaticCursor( 0, 0, 0, 0, 0, 0, 0 );
    if( gMouseMgrCurStat ){
        dbg_free( gMouseMgrCurStat );
        gMouseMgrCurStat = NULL;
    }
    for( i = 0; i < 32; i++ ) MouseMgrDelCache( &gMouseMgrCache[ i ] );
    gMouseMgrCurPal = 0;
    gMouseMgrCurAnim = NULL;
}

void MouseMgrAnimate()
{
    int frame;

    if( !gMouseMgrAnimating ) return;    
    if( !gMouseMgrCurAnim ){
        eprintf( "Animating == 1 but curAnim == 0\n" );
        gMouseMgrAnimating = 0;
        return;    
    }
    if( gMouseMgrGetTimeCb() < gMouseMgrCurAnim->anim.Time ) return;
    gMouseMgrCurAnim->anim.Time = lround( gMouseMgrGetTimeCb() + gMouseMgrGetTimeResCb() * gMouseMgrCurAnim->anim.Fps / gMouseMgrCurAnim->anim.cnt2 );
    if( gMouseMgrCurAnim->anim.cnt1 == gMouseMgrCurAnim->anim.cnt3 ) return;    
    frame = gMouseMgrCurAnim->anim.cnt3 + gMouseMgrCurAnim->anim.Align;
    if( frame >= 0 ){
        if( gMouseMgrCurAnim->anim.cnt2 <= frame ) frame = 0;
    } else {
        frame = gMouseMgrCurAnim->anim.cnt2 - 1;
    }
    gMouseMgrCurAnim->anim.cnt3 = frame;
    memcpy( gMouseMgrCurAnim->anim.Data[ frame ], gMouseMgrCurAnim->anim.bmp[ frame ], gMouseMgrCurAnim->anim.Height * gMouseMgrCurAnim->anim.Width );
    DataFileUnk04( gMouseMgrCurAnim->anim.Data[ frame ], gMouseMgrCurPal, gMouseMgrCurAnim->anim.Width, gMouseMgrCurAnim->anim.Height );
    MseSetStaticCursor( 
	gMouseMgrCurAnim->anim.Data[ frame ], 
	gMouseMgrCurAnim->anim.Width, gMouseMgrCurAnim->anim.Height, 
	gMouseMgrCurAnim->anim.Width, 
	gMouseMgrCurAnim->anim.CentX[ frame ], gMouseMgrCurAnim->anim.CentY[ frame ], 0
    );
}

int MouseMgrLoadAnim( char *fname, int StartFrame )
{
    MseMgrCursor_t **p_Cursor, *Cursor, *v5;
    MseMgrAnimated_t *p;
    MseMgr_t *Cache;
    xFile_t *fh;
    Pal8_t *Palette;
    float fps;
    char *Data,*v47,*s,ErrCode[80];
    int cnt3,Height,*CentX,type,v38,v39,v40,v41,frames,tmp;

    s = fname;
    v47 = gMouseMgrHandlerCb( fname );
    Cache = MouseMgrGetCache( s, &Palette, &tmp, &tmp, &tmp, &tmp, &type );
    p_Cursor = &Cache->Cursor;
    if( Cache ){
        if( type != M_ANIMATED ){ MouseMgrLoadCursor( s ); return 1; }
        Cache->Cursor->anim.cnt1 = StartFrame;
        Cursor = Cache->Cursor;
        if( Cursor->anim.cnt1 >= Cursor->anim.cnt3 ){
            if( Cursor->anim.cnt1 - Cursor->anim.cnt3 >= Cursor->anim.cnt2 + Cursor->anim.cnt3 - Cursor->anim.cnt1 )
                Cursor->anim.Align = -1;
            else 
    		Cursor->anim.Align = 1;
        } else if( Cursor->anim.cnt3 - Cursor->anim.cnt1 < Cursor->anim.cnt2 + Cursor->anim.cnt1 - Cursor->anim.cnt3 ){
            Cursor->anim.Align = -1;
    	    Cursor->anim.Align = 1;
        }
        if( !gMouseMgrAnimating || gMouseMgrCurAnim != *p_Cursor ){
            memcpy((*p_Cursor)->anim.Data[(*p_Cursor)->anim.cnt3], (*p_Cursor)->anim.bmp[(*p_Cursor)->anim.cnt3], (*p_Cursor)->anim.Height * (*p_Cursor)->anim.Width);
            DataFileUnk04((*p_Cursor)->anim.Data[(*p_Cursor)->anim.cnt3], Palette, (*p_Cursor)->anim.Width, (*p_Cursor)->anim.Height);
            v5 = *p_Cursor;
            cnt3 = (*p_Cursor)->anim.cnt3;
            Height = (*p_Cursor)->anim.Height;
            CentX = (*p_Cursor)->anim.CentX;
            Data = (*p_Cursor)->anim.Data[ cnt3 ];
            MseSetStaticCursor( Data, v5->anim.Width, Height, v5->anim.Width, CentX[ cnt3 ], v5->anim.CentY[ cnt3 ], 0 );
            gMouseMgrAnimating = 1;
        }
        gMouseMgrCurAnim = *p_Cursor;
        gMouseMgrCurPal = Palette;
        gMouseMgrCurAnim->anim.Time = gMouseMgrGetTimeCb();
        return 1;
    }
    if( gMouseMgrAnimating ){
        gMouseMgrCurPal = NULL;
        gMouseMgrAnimating = 0;
        gMouseMgrCurAnim = NULL;
    } else if( gMouseMgrCurStat ) {
        dbg_free( gMouseMgrCurStat );
        gMouseMgrCurStat = NULL;
    }
    if( !(fh = dbOpen(v47, "r")) ){ eprintf( "mouseSetFrame: couldn't find %s\n", v47 ); return 0; }
    dbgets( ErrCode, 80, fh );
    if( strncasecmp( ErrCode, "anim", 4 ) ){ dbClose( fh ); MouseMgrLoadCursor( s ); return 1; }    
    if( !(s = strchr( ErrCode, ' ' )) ) return 0;
    sscanf( s + 1, "%d %f", &frames, &fps );
    p = (MseMgrAnimated_t *)dbg_malloc( 40 );
    p->Data = (char **)dbg_malloc( sizeof( char * ) * frames );
    p->bmp = (char **)dbg_malloc( sizeof( char * ) * frames );
    p->CentX = (int *)dbg_malloc( sizeof( int ) * frames );
    p->CentY = (int *)dbg_malloc( sizeof( int ) * frames );
    p->Fps = fps;
    p->Time = gMouseMgrGetTimeCb();
    p->cnt3 = 0;
    p->cnt1 = StartFrame;
    p->cnt2 = frames;
    p->Align = ( (int)p->cnt2 >> 1 <= StartFrame ) ? -1 : 1;    
    for( tmp = 0; tmp < frames; tmp++ ){
        ErrCode[0] = 0;
        dbgets( ErrCode, 80, fh );
        if( !ErrCode[ 0 ] ){
            eprintf( "Not enough frames in %s, got %d, needed %d\n", v47, tmp, frames );
            break;
        }
        if( !(s = strchr( ErrCode, ' ' )) ){ eprintf( "Bad line %s in %s\n", ErrCode, s ); return 0; }
        *s = '\0';
        sscanf( s + 1, "%d %d", &v41, &v40 );
        p->bmp[ tmp ] = DataFileUnk06( gMouseMgrHandlerCb( ErrCode ), &v39, &v38 );
        p->Data[ tmp ] = (char *)dbg_malloc( v38 * v39 );
        memcpy( p->Data[tmp], p->bmp[tmp], v38 * v39 );
        DataFileUnk04( p->Data[tmp], DataFileUnk10(), v39, v38 );
        p->CentX[ tmp ] = v41;
        p->CentY[ tmp ] = v40;
    }
    dbClose( fh );
    p->Width = v39;
    p->Height = v38;
    gMouseMgrCurrentIdx = MouseMgrInsertCache( (MseMgrCursor_t **)&p, M_ANIMATED, DataFileUnk10(), fname );
    strncpy( gMouseMgrCache[ gMouseMgrCurrentIdx ].Name2, s, 31 );
    gMouseMgrCurAnim = (MseMgrCursor_t *)p;
    gMouseMgrCurPal = gMouseMgrCache[ gMouseMgrCurrentIdx ].Pal;
    gMouseMgrAnimating = 1;
    MseSetStaticCursor( p->Data[ 0 ], p->Width, p->Height, p->Width, p->CentX[ 0 ], p->CentY[ 0 ], 0 );
    return 1;
}

int MouseMgrLoadMou( char *fname, int CentX, int CentY )
{
    MseMgrCursor_t *p;
    Pal8_t *Palette;
    MseMgr_t *v7;
    char *s;
    int type,cent,h,w;

    v7 = MouseMgrGetCache( fname, &Palette, &cent, &cent, &w, &h, &type );
    s = gMouseMgrHandlerCb( fname );
    if( !v7 ){
        p = (MseMgrCursor_t *)dbg_malloc( sizeof( MseMgrCursor_t ) );
        p->stat.Data = DataFileUnk06( s, &w, &h );
        p->stat.CentX = CentX;
        p->stat.CentY = CentY;
        p->stat.Width = w;
        p->stat.Height = h;
        gMouseMgrCurrentIdx = MouseMgrInsertCache( &p, M_STATIC, DataFileUnk10(), fname );
        type = M_STATIC;
        Palette = gMouseMgrCache[ gMouseMgrCurrentIdx ].Pal;
    }
    if( type == M_UNSPEC ) return 1;
    if( type > M_STATIC ){
        if( type == M_ANIMATED ){
            gMouseMgrCurAnim = p;
            gMouseMgrAnimating = 1;
            gMouseMgrCurPal = Palette;
        }
        return 1;
    }
    if( gMouseMgrCurStat->stat.Data ) dbg_free( gMouseMgrCurStat->stat.Data );
    gMouseMgrCurStat->stat.Data = (char *)dbg_malloc( h * w );
    memcpy( gMouseMgrCurStat->stat.Data, p->stat.Data, h * w );
    DataFileUnk04( gMouseMgrCurStat->stat.Data, Palette, w, h );
    MseSetStaticCursor( gMouseMgrCurStat->stat.Data, w, h, w, CentX, CentY, 0 );
    gMouseMgrAnimating = 0;
    return 1;
}

int MouseMgrLoadCursor( char *fname )
{
    MseMgr_t *Cache;
    xFile_t *fh;
    Pal8_t *Pal;
    char *s, stmp[80];
    int err,Type,h,w,CentY,CentX;

    if( ( Cache = MouseMgrGetCache( fname, &Pal, &CentX, &CentY, &w, &h, &Type ) ) ){
        if( gMouseMgrCurStat ){
            dbg_free( gMouseMgrCurStat );
            gMouseMgrCurStat = NULL;
        }
        gMouseMgrCurPal = NULL;
        gMouseMgrAnimating = 0;
        gMouseMgrCurAnim = NULL;
        if( Type == M_STATIC ){
            gMouseMgrCurStat->stat.Data = (char *)dbg_malloc( h * w );
            memcpy( gMouseMgrCurStat->stat.Data, Cache->Cursor->stat.Data, h * w );
            DataFileUnk04( gMouseMgrCurStat->stat.Data, Pal, w, h );
            MseSetStaticCursor( gMouseMgrCurStat->stat.Data, w, h, w, CentX, CentY, 0 );
            gMouseMgrAnimating = 0;
        } else {
            if( Type == M_ANIMATED ){
                gMouseMgrCurAnim->anim = Cache->Cursor->anim;
                gMouseMgrCurPal = Pal;
                gMouseMgrCurAnim->anim.cnt3 = 0;
                gMouseMgrCurAnim->anim.cnt1 = 0;
                MseSetStaticCursor( 
            	    gMouseMgrCurAnim->anim.Data[ 0 ], 
            	    gMouseMgrCurAnim->anim.Width, gMouseMgrCurAnim->anim.Height, gMouseMgrCurAnim->anim.Width, 
            	    gMouseMgrCurAnim->anim.CentX[ 0 ], gMouseMgrCurAnim->anim.CentY[ 0 ], 0 
            	);
                gMouseMgrAnimating = 1;
            }
        }
        return 1;
    } else {
        s = strchr( fname, '.' );
        if( s && !strcasecmp( s + 1, "mou" ) ) return MouseMgrLoadMou( fname, 0, 0 );
        s = gMouseMgrHandlerCb( fname );        
        if( !( fh = dbOpen( s, "r" ) ) ){ eprintf( "Can't find %s\n", s ); return 0; }
        stmp[ 0 ] = '\0';
        dbgets( stmp, 79, fh );
        if( !stmp[ 0 ] == '\0' ) return 0;
        if( !strncasecmp( stmp, "anim", sizeof( "anim" ) ) ){
            dbClose( fh );
            err = MouseMgrLoadAnim( fname, 0 );
        } else {
            if( !(s = strchr( stmp, ' ' )) ) return 0;
            s[ 0 ] = '\0';
            sscanf( s + 1, "%d %d", &CentX, &CentY );
            dbClose( fh );
            err = MouseMgrLoadMou( stmp, CentX, CentY );
        }
        strncpy( gMouseMgrCache[ gMouseMgrCurrentIdx ].Name2, fname, 31 );
        return err;
    }
}

void MouseMgrCursorUpdate()
{
    int Type,i,h,w;

    Type = gMouseMgrCache[ gMouseMgrCurrentIdx ].Type;
    if( Type ){
        if( Type == M_STATIC ){
            w = gMouseMgrCache[ gMouseMgrCurrentIdx ].Cursor->stat.Width;
            h = gMouseMgrCache[ gMouseMgrCurrentIdx ].Cursor->stat.Height;
        } else if( Type == M_ANIMATED ){
            w = gMouseMgrCache[ gMouseMgrCurrentIdx ].Cursor->anim.Width;
            h = gMouseMgrCache[ gMouseMgrCurrentIdx ].Cursor->anim.Height;            
        }
    }

    if( Type == M_STATIC ){
        if( gMouseMgrCurStat ){
            dbg_free( gMouseMgrCurStat );
            gMouseMgrCurStat = dbg_malloc( h * w );
            memcpy( gMouseMgrCurStat->stat.Data, gMouseMgrCache[ gMouseMgrCurrentIdx ].Cursor->stat.Data, h * w );
            DataFileUnk04( gMouseMgrCurStat->stat.Data, gMouseMgrCache[ gMouseMgrCurrentIdx ].Pal, w, h );
            MseSetStaticCursor( 
        	gMouseMgrCurStat->stat.Data, 
        	w, h, w, 
        	gMouseMgrCache[ gMouseMgrCurrentIdx ].Cursor->stat.CentX, gMouseMgrCache[ gMouseMgrCurrentIdx ].Cursor->stat.CentY, 0 
    	    );
        } else {
            eprintf( "Hm, current mouse type is M_STATIC, but no current mouse pointer\n" );
        }
    } else if( Type == M_ANIMATED ){
        if( gMouseMgrCurAnim ){            
            for( i = 0; gMouseMgrCurAnim->anim.cnt2 > i; i++ ){
                memcpy( gMouseMgrCurAnim->anim.Data[ i ], gMouseMgrCurAnim->anim.bmp[ i ], h * w );
                DataFileUnk04( gMouseMgrCurAnim->anim.Data[ i ], gMouseMgrCache[ gMouseMgrCurrentIdx ].Pal, w, h );
            }
            MseSetStaticCursor( 
        	gMouseMgrCurAnim->anim.Data[ gMouseMgrCurAnim->anim.cnt3 ], 
        	w, h, w, 
        	gMouseMgrCurAnim->anim.CentX[ gMouseMgrCurAnim->anim.cnt3 ], gMouseMgrCurAnim->anim.CentY[ gMouseMgrCurAnim->anim.cnt3 ], 0
    	    );
        } else {
            eprintf( "Hm, current mouse type is M_ANIMATED, but no current mouse pointer\n" );
        }
    }
}

void MouseMgrUnk10()
{
    MseMgrCursor_t *Cursor, *p;
    int i,j,w,h;

    for( j = 0; j != 32; j++ ){
        if( gMouseMgrCache[j].Type != M_ANIMATED ) continue;
        Cursor = gMouseMgrCache[ gMouseMgrCurrentIdx ].Cursor;
        h = Cursor->anim.Height;
        w = Cursor->anim.Width;
        p = gMouseMgrCache[ j ].Cursor;
        for( i = 0; i < p->anim.cnt2; i++ ){
            memcpy( p->anim.Data[ i ], p->anim.bmp[ i ], h * w );
            DataFileUnk04( p->anim.Data[ i ], gMouseMgrCache[ j ].Pal, w, h );
        }
    }
    if( gMouseMgrCache[ gMouseMgrCurrentIdx ].Type == M_STATIC ) MouseMgrCursorUpdate();
}

void MouseMgrRedraw()
{
    MseCursorRedraw();
}

void MouseMgrDrawCursor()
{
    MseDrawCursor();
}

