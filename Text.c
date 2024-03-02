#include "FrameWork.h"

int gTextCount = 0;
int gTextBaseDelay = 3500;
int gTextLineDelay = 1399;

Text_t *gTextList[ 20 ];
int gTextWidth;
int gTextHeight; 
char *gTextSurface;
int gTextEvEnable;
int gTextEnable;

static void TextEvent();

int TextInit( char *Surface, int Width, int Height )
{
    double BaseDelay, LineDelay;

    if( gTextEnable ) return -1;
    gTextSurface = Surface;
    gTextWidth = Width;
    gTextHeight = Height;
    gTextCount = 0;
    InpTaskStart( TextEvent );
    if( CfgGetFloat( &gConfiguration, "preferences", "text_base_delay", &BaseDelay ) != 1 ) BaseDelay = 3.500;
    if( CfgGetFloat( &gConfiguration, "preferences", "text_line_delay", &LineDelay ) != 1 ) LineDelay = 1.399;
    gTextEvEnable = 1;
    gTextBaseDelay = lround( 1000.0 * BaseDelay );
    gTextEnable = 1;
    gTextLineDelay = lround( 1000.0 * LineDelay );
    return 0;
}

int TextFlush()
{
    int i;

    if( !gTextEnable ) return -1;
    for( i = 0; i < gTextCount; i++ ){
        Free( gTextList[ i ]->bmp );
        Free( gTextList[ i ] );
    }
    gTextCount = 0;
    InpTaskStart( TextEvent );
    return 0;
}

void TextClose()
{
    if( !gTextEnable ) return;    
    TextFlush();
    InpTaskStop( TextEvent );
    gTextEnable = 0;    
}

void TextEvDisable()
{
    gTextEvEnable = 0;
}

void TextEvEnable()
{
    gTextEvEnable = 1;
}

int TextEvStatus()
{
    return gTextEvEnable;
}

void TextSetBaseDelay( double val )
{
    if( val < 1.0 ) val = 1.0;
    gTextBaseDelay = lround( val * 1000.0 );
}

void TextSetLineDelay( double var )
{
    if( var < 0.0 ) var = 0.0;
    gTextLineDelay = lround( var * 1000.0 );
}

int TextFloatMessage( Obj_t *obj, char *Text, int Font, int ColorA, int ColorB, VidRect_t *Area )
{
    Text_t *TextObj;
    short Lines[ 64 ], cnt;
    int i,size,FontId,dp;
    char *s1,*s2,*p,c;

    if( !gTextEnable || gTextCount >= 19 || !Text ) return -1;
    if( *Text == '\0' ) return -1;    
    if( !( TextObj = Malloc( sizeof( Text_t ) ) ) ) return -1;
    memset( TextObj, 0, sizeof( Text_t ) );
    FontId = FontGetCurrent();
    FontSet( Font );

    if( WinTextWrap( Text, 200, Lines, &cnt ) ){ FontSet( FontId ); return -1; }
    TextObj->LinesCnt = cnt - 1;
    if( TextObj->LinesCnt < 1 ) eprintf( "**Error in TextObjectCreate()\n" );
    TextObj->w = 0;
    for( i = 0; i < TextObj->LinesCnt; i++ ){
        s1 = &Text[ Lines[ i + 1] ];
        s2 = &Text[ Lines[ i ] ];
        if( *(s1 - 1) == ' ' ) s1--;
        c = *s1;
        *s1 = '\0';
        if( gFont.LineWidth( s2 ) >= TextObj->w ) TextObj->w = gFont.LineWidth( s2 );
        *s1 = c;
    }
    TextObj->h = (gFont.ChrHeight() + 1) * TextObj->LinesCnt;
    if( ColorB != -1 ){ TextObj->h += 2; TextObj->w += 2; }
    size = TextObj->h * TextObj->w;
    TextObj->bmp = Malloc( size );
    if( !TextObj->bmp ){ FontSet( FontId ); return -1; }
    memset( TextObj->bmp, 0, size );
    p = TextObj->bmp;
    dp = (gFont.ChrHeight() + 1) * TextObj->w;
    if( ColorB != -1 ) p += TextObj->w;
    for( i = 0; i < TextObj->LinesCnt; i++, p += dp ){
        s2 = &Text[ Lines[ i ] ];
        s1 = &Text[ Lines[ i + 1 ] ];
        if( *(s1 - 1) == ' ' ) s1--;
        c = *s1;
        *s1 = '\0';
        gFont.Print( p + ( TextObj->w - gFont.LineWidth( s2 ) ) / 2, s2, TextObj->w, TextObj->w, ColorA );
        *s1 = c;
    }
    if( ColorB != -1 ) ScrEdgeHighlight(TextObj->bmp, TextObj->w, TextObj->h, TextObj->w, ColorB);
    if( obj ){
        TextObj->GridPos = obj->GridId;
    } else {
        TextObj->GridPos = gTileCentIdx;
        TextObj->Flags |= 0x02;
    }
    TextUnk09( TextObj );
    if( Area ){
        Area->lt = TextObj->x;
        Area->tp = TextObj->y;
        Area->rt = TextObj->w + TextObj->x - 1;
        Area->bm = TextObj->h + TextObj->y - 1;
    }
    TextUnk10( obj );
    TextObj->obj = obj;
    TextObj->Time = TimerGetTime();
    gTextList[ gTextCount++ ] = TextObj;
    FontSet( FontId );
    return 0;
}

void TextRender( VidRect_t *area )
{
    VidRect_t rec;
    Text_t *text;
    int i;

    if( !gTextEnable ) return;

    for( i = 0; i < gTextCount; i++ ){
        text = gTextList[ i ];
        TileGetScrCoordinates( text->GridPos, &text->x, &text->y );
        text->x += text->offsx;
        text->y += text->offsy;
        rec.lt = text->x;
        rec.tp = text->y;
        rec.rt = text->w + text->x - 1;
        rec.bm = text->h + text->y - 1;
        if( !RegionShrink( &rec, area, &rec ) )
            ScrCopyAlpha(
        	&text->bmp[ ( rec.tp - text->y ) * text->w + rec.lt - text->x ], 
        	rec.rt - rec.lt + 1, rec.bm - rec.tp + 1, 
        	text->w, 
        	&gTextSurface[ gTextWidth * rec.tp + rec.lt ], 
        	gTextWidth
    	    );
    }            
}

int TextCount()
{
    return gTextCount;
}

static void TextEvent()
{
    int Update, i;
    Text_t *text;
    VidRect_t Area, Rect;
    Text_t **t;

    if( !gTextEvEnable ) return;
    Update = i = 0;
    t = gTextList;
    while( i < gTextCount ){
        text = gTextList[ i ];
        if( (text->Flags & 1) == 0 ){
    	    if( TimerDiff( TimerGetTime(), text->Time ) < ( gTextLineDelay * text->LinesCnt + gTextBaseDelay ) ){
    		i++;
    		t++;
    		continue;
    	    }
        }
        TileGetScrCoordinates( text->GridPos, &text->x, &text->y );
        text->x += text->offsx;
        text->y += text->offsy;
        Area.lt = text->x;
        Area.tp = text->y;
        Area.rt = text->w + text->x - 1;
        Area.bm = text->h + text->y - 1;
        if( Update ) {
            RegionExpand( &Rect, &Area, &Rect );
        } else {
            Rect = Area;
            Update = 1;
        }
        Free( gTextList[ i ]->bmp );
        Free( gTextList[ i ] );
        memmove( t, t + 1, (gTextCount - i - 1) * sizeof( Text_t *) );
        gTextCount--;
    }         
    if( Update ) TileUpdateArea( &Rect, gMapCurrentLvl );
}

void TextUnk09( Text_t *text )
{
    int x,y,v6,v9,v11,v12,v13,v21,v24,yg,xg;

    TileGetScrCoordinates(text->GridPos, &xg, &yg);
    text->x = xg + 16 - text->w / 2;
    text->y = yg;
    if( (text->Flags & 2) == 0 ) text->y -= text->h + 60;
    x = text->x;
    y = text->y;
    v6 = y + text->h - 1;
    if( x < 0 || y < 0 || x + text->w - 1 >= gTextWidth || v6 >= gTextHeight ){
        text->x = text->x - text->w / 2;
        v9 = text->x;
        v24 = text->x + text->w - 1;
        if( text->x < 0 || y < 0 || v24 >= gTextWidth || v6 >= gTextHeight ){
            v11 = text->w + text->x;
            text->x = v11;
            v12 = v11;
            v13 = v11 + text->w - 1;
            if( v12 < 0 || y < 0 || v13 >= gTextWidth || v6 >= gTextHeight ){
                text->x = xg - 16 - text->w;
                text->y = yg - 16 - text->h;
                v21 = text->y + text->h - 1;
                if( text->x >= 0 && text->y >= 0 && text->x + text->w - 1 < gTextWidth && v21 < gTextHeight ){
                    text->offsx = text->x - xg;
		    text->offsy = text->y - yg;
		    return;
                }
                text->x = text->w + 64 + text->x;
                v24 = text->x + text->w - 1;
                if( text->x >= 0 && text->y >= 0 && v24 < gTextWidth && v21 < gTextHeight ){
                    text->offsx = text->x - xg;
		    text->offsy = text->y - yg;
		    return;
                }
                text->x = xg + 16 - text->w / 2;
                text->y = yg;
                v12 = text->x;
                v21 = text->y + text->h - 1;
                if( v12 < 0 || text->y < 0 || v12 + text->w - 1 >= gTextWidth || v21 >= gTextHeight ){
                    text->x = text->x - text->w / 2;
                    v24 = text->x + text->w - 1;
                    if( text->x >= 0 && text->y >= 0 && v24 < gTextWidth && v21 < gTextHeight ){
                        text->offsx = text->x - xg;
			text->offsy = text->y - yg;
			return;
                    }
                    text->x = text->w + text->x;
                    v24 = text->x + text->w - 1;
                    if( text->x >= 0 && text->y >= 0 && v24 < gTextWidth && v21 < gTextHeight ){
                        text->offsx = text->x - xg;
			text->offsy = text->y - yg;
			return;
                    }
                    text->x = xg + 16 - text->w / 2;
                    text->y = yg - (text->h + 60);
                    v12 = text->x;
                }
            }
            text->offsx = v12 - xg;
	    text->offsy = text->y - yg;
	    return;
        }
        text->offsx = v9 - xg;
        text->offsy = text->y - yg;
    } else {
        text->offsx = x - xg;
        text->offsy = text->y - yg;
    }
}

void TextUnk10( Obj_t *obj )
{
    int i;

    for( i = 0; i < gTextCount; i++ ){
        if( obj == gTextList[ i ]->obj ) gTextList[ i ]->Flags |= 0x01;
    }
}

