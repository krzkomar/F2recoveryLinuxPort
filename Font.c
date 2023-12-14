#include "FrameWork.h"

static FontHdr_t gFontData[ 10 ];
static FontObj_t gFontDb[ 10 ];
static FontHdr_t *gFontHdrData;

Font_t gFont = { -1, 0,0,0,0,0,0,0,0,0 };

int FontGetLineWidth( char *str );
void FontDataSelect( int FontId );
void FontPrint( char *Dst, char *Str, int DstPitch, int FieldPixSize, int Color );
int FontGetHeight();

int FontCharGetWidth( char chr);
int FontGetLineWidthMono( char *str );
int FontGetDistance();
int FontLinePixels( char *str );
int FontGetMaxWidth();

int FontInit()
{
    static FontObj_t FontTemplate = { 0, 9, 
	FontDataSelect, FontPrint, FontGetHeight, 
	FontGetLineWidth, FontCharGetWidth, FontGetLineWidthMono, 
	FontGetDistance, FontLinePixels, FontGetMaxWidth 
    };
    int i,n;
    FontObj_t tmp;

    n = -1;
    memcpy( &tmp, &FontTemplate, sizeof( FontObj_t ) );
    for( i = 0; i < 10; i++ ){
        if( FontLoadFromFile( i ) == -1 ){
            gFontData[ i ].count = 0;
        } else if( n == -1 ){
            n = i;
        }
    }
    if( n == -1 ) return -1;
    if( FontSetup( &tmp ) == -1 ) return -1;
    FontSet( n );
    return 0;
}

void FontFree()
{
    int i;

    for( i = 0; i != 10; i++ ){
        if( gFontData[ i ].count ) Free( gFontData[i].fontinfo );
    }
}

int FontLoadFromFile( int FontFileIdx )
{
    xFile_t *DbFile;
    FontInfo_t *pFontInfo, *p;
    int r, count, DataSize;
    void *buffer;
    char Path[16];

    sprintf( Path, "font%d.fon", FontFileIdx );
    DbFile = dbOpen(Path, "rb");
    if( !DbFile ) return -1;
    if( dbread( &gFontData[ FontFileIdx ], 20, 1, DbFile) != 1 ){ // 20 - size of FontHdr_t in 32bit, there are 2 pointers !
	dbClose( DbFile );
	return -1;    
    }
    if( ( pFontInfo = Malloc( sizeof( FontInfo_t ) * gFontData[ FontFileIdx ].count ) ) == NULL ){
	dbClose( DbFile );
	return -1;    
    }
    gFontData[ FontFileIdx ].fontinfo = pFontInfo;
    r = dbread( pFontInfo, sizeof( FontInfo_t ), gFontData[ FontFileIdx ].count, DbFile );
    count = gFontData[ FontFileIdx ].count;
    if( r != count ) Free( gFontData[ FontFileIdx ].fontinfo );
    p = &gFontData[ FontFileIdx ].fontinfo[ count - 1 ];
    DataSize = gFontData[ FontFileIdx ].height * ((p->width + 7) / 8) + p->offset;
    buffer = Malloc( DataSize );
    gFontData[ FontFileIdx ].data = buffer;
    if( !buffer ){
	dbClose( DbFile );
	Free( gFontData[ FontFileIdx ].fontinfo );
    }
    if( dbread( buffer, 1, DataSize, DbFile) != DataSize ){
	dbClose( DbFile );
	Free( gFontData[ FontFileIdx ].fontinfo );
    }
    dbClose( DbFile );
    return 0;
}

int FontSetup( FontObj_t *p )
{
    int i;
    FontObj_t *tmp;

    if( !p ) return -1;    
    if(( gFont.Fonts >= 10 ) || ( p->dbMax <= p->dbMin )) return -1;    
    for( i = p->dbMin; i <= p->dbMax; i++ ){
	if( FontLookup( i, &tmp ) ) return -1;	
    }
    memcpy( &gFontDb[ gFont.Fonts++ ], p, sizeof( FontObj_t ) ) ;
    return 0;    
}

int FontUnk01( int Id )
{
    int v1;
    FontObj_t *p, *font;

    if( !FontLookup( Id, &font ) ) return -1;
    v1 = 0;
    if( gFont.Fonts <= 0 )  return -1;
    for( p = gFontDb; (p != font) || ((gFont.FontId >= font->dbMin) && (gFont.FontId <= font->dbMax)); p++ ){
        if( ++v1 >= gFont.Fonts ) return -1;
    }
    memmove( &gFontDb[v1], &gFontDb[v1 + 1], 4 * (gFont.Fonts - v1 - 1) );
    gFont.Fonts--;
    return 0;
}

void FontDataSelect( int FontId )
{
    if( FontId >= 10 ) return;
    if( gFontData[ FontId ].count == 0 ) return;
    gFontHdrData = &gFontData[ FontId ];    
}

int FontGetCurrent()
{
    return gFont.FontId;
}

void FontSet( int FontId )
{
    FontObj_t *p;
    if( FontLookup( FontId, &p) ){
        gFont.Print = p->Print;
        gFont.ChrHeight = p->Height;
        gFont.LineWidth = p->LineWidth;
        gFont.ChrWidth = p->Width;
        gFont.LineWidthMono = p->WidthMono;
        gFont.Distance = p->Distance;
        gFont.Pixels = p->Pixels;
        gFont.MaxWidth = p->MaxWidth;
        gFont.FontId = FontId;
        p->Select( FontId );
    }
}

int FontLookup( int n, FontObj_t **p )
{
    int i;

    if( gFont.Fonts <= 0 ) return 0;    
    for( i = 0; i < gFont.Fonts; i++ ){
	if( n >= gFontDb[ i ].dbMin && n <= gFontDb[ i ].dbMax ){
	    *p = &gFontDb[ i ];
	    return 1;
	}
    }
    return 0;            
}

void FontPrint( char *Dst, char *Str, int FieldPixSize, int DstPitch, int Color )
{
    char *d, *p, *NextPos, *s, *src;
    int width, j, i, Spacer, mask, c;

    d = Dst;
    if( Color & FONT_SHADOW ){
        c = Color & (~(COLOR_MASK | FONT_SHADOW));
        Color &= ~FONT_SHADOW;
        gFont.Print( &Dst[ DstPitch + 1 ], Str, FieldPixSize, DstPitch, (c | COLOR_ALPHA) & ~FONT_SHADOW );
    }
    if( Color & FONT_MONOSPACE ) Spacer = gFont.MaxWidth();
    for( s = Str; *s; s++ ){ // kolejne znaki lancucha
        if( *s >= gFontHdrData->count ) continue; // znak spoza zakresu
        width = gFontHdrData->fontinfo[ (int)*s ].width;
        if( Color & FONT_MONOSPACE ){
            NextPos = d + Spacer;
            d += (Spacer - gFontHdrData->distance - width) / 2;
        } else {
            NextPos = d + width + gFontHdrData->distance;
        }
        if( NextPos - Dst > FieldPixSize ) break;

        src = (char *)&gFontHdrData->data[ gFontHdrData->fontinfo[ (int)*s ].offset ];
        for( i = 0; i < gFontHdrData->height; i++, src++ ){ // kolejne linie znaku
            for( j = 0, mask = 0x80; j < width; d++, j++, mask >>= 1 ){ // narysowanie linii znaku
                if( mask == 0  ){ mask = 0x80; src++; } // roluj maske, pobierz kolejny bajt z wejscia
                if( mask & *src ) *d = (char)Color; 	// jesli '1' na bicie to postaw punkt                       
            }                    
            d += DstPitch - width; // kolejna linia znaku
        }
        d = NextPos;        
    }
    
    if( Color & FONT_UNDERLINE ){ // podkreslenie
        p = &Dst[ DstPitch * (gFontHdrData->height - 1) ];
        for( i = 0; i < (d - Dst); p++, i++ ) *(p - 1) = Color;
    }
}

int FontGetHeight()
{
    return gFontHdrData->height;
}

int FontGetLineWidth( char *str )
{
    int width;
    
    if( *str == '\0') return 0;
    for( width = 0; *str; str++ ){
        if( *str < gFontHdrData->count ) width += gFontHdrData->distance + gFontHdrData->fontinfo[ (int)*str ].width;
    }
    return width;
}

int FontCharGetWidth( char chr)
{
    return gFontHdrData->fontinfo[ (unsigned int)chr ].width;
}

int FontGetLineWidthMono( char *str )
{
    return strlen( str ) * gFont.MaxWidth();
}

int FontGetDistance()
{
    return gFontHdrData->distance;
}

int FontLinePixels( char *str )
{
    return gFont.LineWidth( str ) * gFont.ChrHeight();
}

int FontGetMaxWidth()
{
    FontInfo_t *fontinfo;
    int i, width;

    fontinfo = gFontHdrData->fontinfo;
    for( width = i = 0; i < gFontHdrData->count; i++, fontinfo++){
        if( width < fontinfo->width ) width = fontinfo->width;
    }
    return width + gFontHdrData->distance;
}

