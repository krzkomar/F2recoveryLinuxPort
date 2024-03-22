#include "FrameWork.h"

static int gFontMgrInited = 0;
static int gFontsMgr = 0;
FontObj_t gFontInit = {
    0x64, 0x6E, 
    (void *)FontMgrSelect, 
    (void *)FontMgrPrint, 
    (void *)FontMgrHeight, 
    (void *)FontMgrLineWidth, 
    (void *)FontMgrWidth, 
    (void *)FontMgrWidthMono, 
    (void *)FontMgrDistance, 
    (void *)FontMgrPixels, 
    (void *)FontMgrMaxWidth
};

FontMgr_t  gFontMgrDataBase[ FONT_MGR_DATABASE_SIZE ];
int 	   gFontMgrCurId;
FontMgr_t *gFontMgrCurData;

int FontMgrInit()
{
    int Id, first;

    first = -1;
    for( Id = 0; Id < 16; Id++ ){
        if( FontMgrLoad( Id ) == -1 ){
            gFontMgrDataBase[ Id ].Data = NULL;
            gFontMgrDataBase[ Id ].MaxH = 0;
        } else {
            gFontsMgr++;
            if( first == -1 ) first = Id;
        }
    }
    if( first == -1 )  return -1;
    gFontMgrInited = 1;
    FontMgrSelect( first + 100 );
    return 0;
}

void FontMgrClose()
{
    int i;

    for( i = 0; i != 16; i++ ){
        if( gFontMgrDataBase[ i ].Data ) dbg_free( gFontMgrDataBase[ i ].Data );
    }
}

int FontMgrLoad( int FontId )
{
    FontMgr_t *db;
    FontMgrDsc_t *dsc;
    xFile_t *fh;
    int err = -1, i, magic, flen;
    char fname[ 16 ];

    db = &gFontMgrDataBase[ FontId ];
    sprintf( fname, "font%d.aaf", FontId );
    if( !(fh = dbOpen(fname, "rb")) ) return -1;
    flen = dbFileLength( fh );
    if( dbgetBei( fh, &magic ) ) goto err;
    
    if( magic != FONT_AFF_MAGIC ) goto err; 
    if( dbread( &db->MaxH, 2, 1, fh ) != 1 ) goto err; // max glyph height
    WBE_TO_LE( db->MaxH );
    if( dbread( &db->gapH, 2, 1, fh) != 1 ) goto err; // horizontal gap
    WBE_TO_LE( db->gapH );
    if( dbread( &db->SpaceW, 2, 1, fh ) != 1 ) goto err; // width of space character
    WBE_TO_LE( db->SpaceW );
    if( dbread( &db->gapW, 2, 1, fh) != 1 ) goto err; // vertical gapsize    
    WBE_TO_LE( db->gapW );

    dsc = db->dsc;
    db->MaxW = 0;
    for( i = 0; i < 256; i++, dsc++ ){
        if( dbread( &dsc->Width, 2, 1, fh ) != 1 ) goto err; // glyph width
        WBE_TO_LE( dsc->Width);
        if( dbread( &dsc->Height, 2, 1, fh) != 1 ) goto err; // glyph height
        WBE_TO_LE( dsc->Height );
        if( dsc->Width > db->MaxW ) db->MaxW = dsc->Width;
        if( dbgetBei( fh, &dsc->Offset ) ) goto err; // offset
    }
    flen -= FONT_AFF_FILE_OFFSET;
    if( !(db->Data = (char *)dbg_malloc( flen )) ) goto err;
    if( dbread( db->Data, flen, 1, fh ) != 1 ){ // read data
	dbg_free( db->Data );
	dbClose( fh );
	return -1;
    }
    err = 0;
err:
    dbClose( fh );
    return err;
}

void FontMgrSelect( int FontId )
{
    unsigned int id;

    if( !gFontMgrInited ) return;
    
    id = FontId - 100;
    if( (FontId - 100) > gFontsMgr ) return;    
    if( !gFontMgrDataBase[ id ].Data ) return;    
    gFontMgrCurId = FontId - 100;
    gFontMgrCurData = &gFontMgrDataBase[ id ];
}

int FontMgrHeight()
{
    if( !gFontMgrInited ) return 0;
    return gFontMgrCurData->gapW + gFontMgrCurData->MaxH;
}

int FontMgrLineWidth( char *str )
{
    int w = 0;

    if( !gFontMgrInited ) return 0;
    for( ;*str; str++ ){
        if( *str == ' ' ){
            w += gFontMgrCurData->gapH;
            w += gFontMgrCurData->SpaceW;
        } else {
            w += gFontMgrCurData->dsc[ (int)*str ].Width;
            w += gFontMgrCurData->gapH;
        }
    }
    return w;
}

int FontMgrWidth( char Chr )
{
    if( !gFontMgrInited ) return 0;
    return ( Chr == ' ' ) ? gFontMgrCurData->SpaceW : gFontMgrCurData->dsc[ (( unsigned int)Chr) & 0xff ].Width;
}

int FontMgrWidthMono( char *str )
{
    if( !gFontMgrInited ) return 0;
    return FontMgrMaxWidth() * strlen( str );
}

int FontMgrDistance()
{
    if( !gFontMgrInited ) return 0;
    return gFontMgrCurData->gapH;
}

int FontMgrPixels( char *str )
{
    if( !gFontMgrInited ) return 0;
    return FontMgrLineWidth( str ) * FontMgrHeight();
}

int FontMgrMaxWidth()
{
    int a;

    if( !gFontMgrInited ) return 0;
    a = ( gFontMgrCurData->SpaceW <= gFontMgrCurData->MaxW ) ? gFontMgrCurData->MaxW : gFontMgrCurData->SpaceW;
    return a + gFontMgrCurData->gapH;
}

int FontMgrGetCurId()
{
    return gFontMgrCurId;
}

void FontMgrPrint( unsigned char *Dst, char *Str, int FieldPixSize, int Pitch, int Color )
{
    FontMgr_t *data;
    int w, j, MonoGap, i;
    uint32_t chr;
    unsigned char *p, *s, *scr, *ofs, *Pal;

    while( 1 ){
        data = gFontMgrCurData;
	s = Dst;
        if( !gFontMgrInited ) return;
        if( !(Color & FONT_SHADOW) ) break;        
        Color &= ~FONT_SHADOW;
        FontMgrPrint( WIN_XY( 1, 1, Pitch, Dst ), Str, FieldPixSize, Pitch, (COLOR_ALPHA | Color) & ~0x100FF );
    }

    if( Color & FONT_MONOSPACE ){
        MonoGap = ( gFontMgrCurData->MaxW >= gFontMgrCurData->SpaceW ) ? gFontMgrCurData->MaxW : gFontMgrCurData->SpaceW;
        MonoGap += gFontMgrCurData->gapH;
    }

    Pal = (unsigned char *)PalExtOpen( Color );

    for( ; (chr = *Str & 0xff); Str++ ){
        w = ( chr == ' ' ) ? data->SpaceW : data->dsc[ chr ].Width;
        if( Color & FONT_MONOSPACE ){
            scr = Dst + MonoGap;
            Dst += (MonoGap - data->gapH - w) / 2;
        } else {
            scr = Dst + data->gapH + w;
        }
        if( scr - s > FieldPixSize ) break;
        p = &Dst[ Pitch * ( data->MaxH - data->dsc[ chr ].Height ) ];
        ofs = (unsigned char *)&data->Data[ data->dsc[ chr ].Offset ];
        for( i = 0; i < data->dsc[ chr ].Height; i++, p += Pitch - w ){
            for( j = 0; j < w; j++, p++, ofs++ ){
		*p = Pal[ 256 * (*ofs & 0x0f) + *p ];
            }                
        }
        Dst = scr;
    }

    if( Color & FONT_UNDERLINE ){
        j = Dst - s;
        p = s + Pitch * (data->MaxH - 1);
	for( i = 0; i < j; i++, p++ ) *p = Color;
    }
    PalExtClose( Color );
}

int FontMgrGetGapW()
{
    if( !gFontMgrInited ) return 0;
    return gFontMgrCurData->gapW;
}

int FontMgrGetSpaceW()
{
    if( !gFontMgrInited ) return 0;
    return gFontMgrCurData->SpaceW;
}


