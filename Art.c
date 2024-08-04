#include "FrameWork.h"
//#define DBG_SHOW_FNAMES

char gProtoDataFilePath[ 260 ] = { 0 };

//
ProtoCat_t gArtCatalog[ PROTOs ] = {
    { 0, "items",    NULL, NULL, 0 },
    { 0, "critters", NULL, NULL, 0 },
    { 0, "scenery",  NULL, NULL, 0 },
    { 0, "walls",    NULL, NULL, 0 },
    { 0, "tiles",    NULL, NULL, 0 },
    { 0, "misc",     NULL, NULL, 0 },
    { 0, "intrface", NULL, NULL, 0 },
    { 0, "inven",    NULL, NULL, 0 },
    { 0, "heads",    NULL, NULL, 0 },
    { 0, "backgrnd", NULL, NULL, 0 },
    { 0, "skilldex", NULL, NULL, 0 }
};

int gArtLangSubdir = 0;
int gArtPrep[ 6 ] = {0,0,0,0,0,1};

// .bss
char gArtSubDir[ 32 ];
Cache_t gArtCacheDb;
//char gArtFilePath[ 300 ];
ArtHeadsInfo_t *gArtHeadsInfo;
int *gArtAnonAlias;
int *gArtCritterFidShouldRunData;

int ArtInit()
{
    char *p, *w, str[512], *language;
    int i, CacheSizeMB;
    xFile_t *fd;

    if( !CfgGetInteger( &gConfiguration, "system", "art_cache_size", &CacheSizeMB ) ) CacheSizeMB = ART_DEFAULT_CACHE_SIZE_MB;
    if( CacheInit( &gArtCacheDb, ArtGetFileLen, (void *)ArtLoadImage, ArtFree, SIZE_MB( CacheSizeMB ) ) != 1 ){
	eprintf("cache_init failed in art_init");
	return -1;
    }

    if( CfgGetString( &gConfiguration, "system", "language", &language) == 1 && strcasecmp( "english", language ) ){
        strcpy( gArtSubDir, language );
        gArtLangSubdir = 1;
    }
    // load lst file
    for(i = 0; i < PROTOs; i++ ){
        gArtCatalog[ i ].Flags = 0;
        sprintf( str, "%s%s%s/%s.lst", gProtoDataFilePath, "art/", gArtCatalog[ i ].SubDirName, gArtCatalog[ i ].SubDirName );

        if( ArtReadLstFile( str, &gArtCatalog[ i ].DirLstFname, &gArtCatalog[ i ].NamesCount ) ){
            eprintf( "art_read_lst failed in art_init" );
            CacheClose( &gArtCacheDb );
            return -1;
        }
    }

    gArtAnonAlias = Malloc( gArtCatalog[ PROTO_CRITTERS ].NamesCount * sizeof( void * ) );
    if( !gArtAnonAlias ){
        gArtCatalog[ PROTO_CRITTERS ].NamesCount = 0;
        eprintf( "Out of memory for anon_alias in art_init" );
        CacheClose( &gArtCacheDb );
        return -1;
    }
    gArtCritterFidShouldRunData = Malloc( gArtCatalog[ PROTO_CRITTERS ].NamesCount * sizeof( void * ) );
    if( !gArtCritterFidShouldRunData ){
        gArtCatalog[ PROTO_CRITTERS ].NamesCount = 0;
        eprintf( "Out of memory for artCritterFidShouldRunData in art_init" );
        CacheClose( &gArtCacheDb );
        return -1;
    }
    
    for( i = 0; i < gArtCatalog[ PROTO_CRITTERS ].NamesCount; i++ ){
        gArtCritterFidShouldRunData[ i ] = 0;
    }

    sprintf( str, "%s%s%s/%s.lst", gProtoDataFilePath, "art/", gArtCatalog[ PROTO_CRITTERS ].SubDirName, gArtCatalog[ PROTO_CRITTERS ].SubDirName );
    // open list
    if( !( fd = dbOpen( str, "rt" ) ) ){
        eprintf("Unable to open %s in art_init", str);
        CacheClose( &gArtCacheDb );
        return -1;
    }

    for( i = 0; i < gArtCatalog[ PROTO_CRITTERS ].NamesCount; i++ ){
        if( !strcasecmp( &gArtCatalog[ PROTO_CRITTERS ].DirLstFname[ i*ART_FNAME_SIZE ], "hmjmps") ){
            gArtPrep[3] = i;
        } else if( !strcasecmp( &gArtCatalog[ PROTO_CRITTERS ].DirLstFname[ i*ART_FNAME_SIZE ], "hfjmps") ){
            gArtPrep[4] = i;
        }
        if( !strcasecmp( &gArtCatalog[ PROTO_CRITTERS ].DirLstFname[ i*ART_FNAME_SIZE ], "hmwarr" ) ){
            gArtPrep[1] = i;
            gArtPrep[0] = i;
        } else if( !strcasecmp( &gArtCatalog[ PROTO_CRITTERS ].DirLstFname[ i*ART_FNAME_SIZE ], "hfprim" ) ){
            gArtPrep[2] = i;
        }
    }

    for( i = 0; i < gArtCatalog[ PROTO_CRITTERS ].NamesCount; i++ ){
        if( !dbgets(str, 200, fd) ) break;            
        if( (p = strchr( str, ',')) ){
    	p++;
            gArtAnonAlias[ i ] = strtol( p, NULL, 10 );
            if( (p = strchr( p, ',')) ){
                gArtCritterFidShouldRunData[ i ] = strtol( p + 1, NULL, 10 );
            } else {
                gArtCritterFidShouldRunData[ i ] = 0;
            }
        } else {
            gArtAnonAlias[ i ] = gArtPrep[0];
            gArtCritterFidShouldRunData[ i ] = 1;
        }
    }
    dbClose( fd );

    for( i = 0; i < gArtCatalog[ PROTO_TILES ].NamesCount; i++ ){
        if( !strcasecmp( &gArtCatalog[ PROTO_TILES ].DirLstFname[ i * ART_FNAME_SIZE ], "grid001.frm" ) ){
    	    gArtPrep[5] = i;
    	    break;
        }
    }
        
    gArtHeadsInfo = Malloc( gArtCatalog[ PROTO_HEADS ].NamesCount * sizeof( ArtHeadsInfo_t ));
    if( !gArtHeadsInfo ){
        gArtCatalog[ PROTO_HEADS ].NamesCount = 0;
        eprintf( "Out of memory for head_info in art_init" );
        CacheClose( &gArtCacheDb );
        return -1;
    }

    // open lst file
    sprintf( str, "%s%s%s/%s.lst", gProtoDataFilePath, "art/", gArtCatalog[ PROTO_HEADS ].SubDirName, gArtCatalog[ PROTO_HEADS ].SubDirName );    
    if( !(fd = dbOpen( str, "rt") ) ){
        eprintf( "Unable to open %s in art_init", str );
        CacheClose( &gArtCacheDb );
        return -1;
    }

    // load lst file arguments
    for( i = 0; i < gArtCatalog[ PROTO_HEADS ].NamesCount; i++ ){
        if( !dbgets( str, 200, fd ) ) break; // read line
	w = str;	
	// name
	p = w;
        p = strchr( w, ',');            
        if( p ) *p++ = '\0'; else p = w;
	// arg 0
	w = p;	
	p = strchr( p, ',' );
        if( p ) *p++ = 0; else p = w;
        gArtHeadsInfo[ i ].Arg0 = strtol( w, NULL, 10 );
	// arg 1
        w = p;
	p = strchr( p, ',' );
        if( p ) *p++ = '\0'; else p = w;
        gArtHeadsInfo[ i ].Arg1 = strtol( w, NULL, 10 );
	// arg 2
        p = strpbrk( w, " ,;\t\n" );
        if( p ) *p = '\0';
        gArtHeadsInfo[ i ].Arg2 = strtol( w, NULL, 10 );
    }
    dbClose( fd );
    return 0;
}

void ArtCacheFree()
{
    int i;

    CacheClose( &gArtCacheDb );
    Free( gArtAnonAlias );
    Free( gArtCritterFidShouldRunData );
    for( i = 0; i < PROTOs; i++ ){
        Free( gArtCatalog[ i ].DirLstFname );
        gArtCatalog[ i ].DirLstFname = NULL;
	gArtCatalog[ i ].p02 = NULL;
    }
    Free( gArtHeadsInfo );
}

char *ArtGetCatDirName( unsigned int Category )
{
    if( Category >= PROTOs ) return NULL;
    return gArtCatalog[ Category ].SubDirName;
}

int ArtIsHidden( unsigned int Category )
{
    if( Category >= PROTOs ) return 0;
    return gArtCatalog[ Category ].Flags & PRFLG_INVISIBLE;
}

void ArtGetCatToggleFlag0( unsigned int Category )
{
    if( Category < PROTOs ) gArtCatalog[ Category ].Flags ^= PRFLG_INVISIBLE;
}

int ArtGetFilesCount( unsigned int Category )
{
    if( Category >= PROTOs ) return 0; 
    return gArtCatalog[ Category ].NamesCount;
}


/*
    Id32:[ii aa .nnn]: ii - category[8], aa[8] - args, . - unknown, nnn - file number 
    0..11 - FileNo    
*/
int ArtGetHeadsInfo( int Id )
{
    int idx;
    if( (Id & 0xF000000) >> 24 != PROTO_HEADS ) return 0;
    idx = Id & 0xFFF;
    if( idx > gArtCatalog[ PROTO_HEADS ].NamesCount ) return 0;
    switch( (Id & 0xFF0000 ) >> 16){
	case 1: return gArtHeadsInfo[ idx ].Arg0;
	case 4: return gArtHeadsInfo[ idx ].Arg1;
	case 7: return gArtHeadsInfo[ idx ].Arg2;
    }
    return 0;
}

void ArtLoadImgScaled( int Id, char *pDst, int PosX, int PosY, int DstPitch )
{
    ArtFrm_t *FrameData, *frm;
    ArtFrmHdr_t *data;
    CachePool_t *Obj;
    int Width, Height, y, x;
    char *pSrc;

    if( Id == -1 ) return;    
    data = 0;
    CacheLoad( &gArtCacheDb, Id, (void **)&data, &Obj );
    if( !data ) return;    
    FrameData = ArtGetFrameData( data, 0, 0 );
    pSrc = FrameData->Data;
    frm    = ArtGetFrameData( data, 0, 0);
    Width  = ( frm ) ? frm->Width  : -1;
    Height = ( frm ) ? frm->Height : -1;
    x = PosX - Width;
    y = PosY - Height;
    if( x < 0 || y < 0 ){
        if( PosY * Width >= PosX * Height )
            ScrScaleImgAlpha( pSrc, Width, Height, Width, &pDst[ DstPitch * ((PosY - PosX * Height / Width) / 2) ], PosX, PosX * Height / Width, DstPitch );
        else
            ScrScaleImgAlpha( pSrc, Width, Height, Width, &pDst[ (PosX - PosY * Width / Height) / 2 ], PosY * Width / Height, PosY, DstPitch );
    } else {
        ScrCopyAlpha( pSrc, Width, Height, Width, &pDst[ DstPitch * (y / 2) + x / 2 ], DstPitch );
    }
    CacheUnlockBlock( &gArtCacheDb, Obj );    
}

ArtFrmHdr_t *ArtLoadImg( int Id, CachePool_t **Obj )
{
    ArtFrmHdr_t *data;

    data = NULL;
    if( Obj ) CacheLoad( &gArtCacheDb, Id, (void **)&data, Obj );
    return data;
}

char *ArtGetBitmap( int Id, int FrNo, int Orientation, CachePool_t **Obj )
{
    ArtFrm_t *img;
    ArtFrmHdr_t *data = NULL;

    if( Obj ) CacheLoad( &gArtCacheDb, Id, (void **)&data, Obj );
    if( data == NULL ) return NULL;
    img = ArtGetFrameData( data, FrNo, Orientation );
    if( !img ) return NULL;
    return img->Data;
}

char *ArtLoadBmp( int IdCode, CachePool_t **Img, int *pWidth, int *pHeight )
{
    ArtFrmHdr_t *data = NULL;
    ArtFrm_t *n;

    *Img = NULL;
    CacheLoad( &gArtCacheDb, IdCode, (void **)&data, Img );
    if( !data ) return 0;
    n = ArtGetFrameData( data, 0, 0 );
    *pWidth = n ? n->Width : -1;
    if( *pWidth == -1 ) return 0;    
    *pHeight = n ? n->Height : -1;
    if( *pHeight == -1 ) return 0;
    if( !n ) return 0;
    return n->Data;
}

int ArtClose( CachePool_t *Block )
{
    return (CacheUnlockBlock( &gArtCacheDb, Block ) != 0) - 1;
}

int ArtExpandCache()
{
    return (CacheExpand(&gArtCacheDb) != 0) - 1;
}

int ArtRemove( int FileId )
{
    return (CacheRemove( &gArtCacheDb, FileId ) != 0) - 1;
}

int ArtGetFilenameFromList( int Category, int FileIdx, char *fname )
{
    if( Category >= PROTOs ) return -1;
    if( FileIdx >= gArtCatalog[ Category ].NamesCount ) return -1;
    strcpy( fname, &gArtCatalog[ Category ].DirLstFname[ ART_FNAME_SIZE * FileIdx] );
    return 0;
}

int ArtMakeFnameMark( int id, int Category, char *Mark0, char *Mark1 )
{
    if( Category >= PROTOs ) return -1;
    if( id >= 38 && id <= 47 ){ // 38..47
        *Mark0 = id + 61;	// 99 .. 108 -> {c,d,e,f,g,h,i,j,k} {*}
        *Mark1 = Category + 'c';// {c,d,e,f,g,h,i,j} {*}
        if( Category == 0 ) return -1;
        return 0;
    }
    switch( id ){
        case 36: *Mark0 = 'h'; *Mark1 = 'c'; return 0; // h c
        case 37: *Mark0 = 'j'; *Mark1 = 'c'; return 0; // j c
        case 64: *Mark0 = 'a'; *Mark1 = 'n'; return 0; // a n
    }
    if( id >= 48 ){ *Mark0 = id + 49; *Mark1 = 'r'; return 0; } // r
    if( id >= 20 ){ *Mark0 = id + 77; *Mark1 = 'b'; return 0; } // b
    if( id == 18 ){
        *Mark1 = 'a'; *Mark0 = 's'; 				  // a s
	if( Category == PROTO_CRITTERS ){ *Mark1 = 'd'; *Mark0 = 'm'; return 0; }   // d m
        if( Category == PROTO_TILES ){ *Mark1 = 'g'; *Mark0 = 'm'; return 0; }   // g m
        return 0;        
    }
    if( id != 13 ){
        *Mark0 = id + 'a'; *Mark1 = 'a';			         // a a
        if( id <= 1 && Category > 0 ){ *Mark1 = Category + 'c'; } // {c,d,e,f,g,h,i,j} a
        return 0;
    }
    *Mark1 = 'a'; *Mark0 = 'n';					 // a n
    if( Category > 0 ){ *Mark1 = Category + 'c'; *Mark0 = 'e'; }  // {c,d,e,f,g,h,i,j} e
    return 0;    
}

char *ArtMakeFileNameFromId( int FileId )
{
    static char gArtFilePath[ 512 ];
    static const char *gArtMark1 = "gggnnnbbbgnb";
    static const char *gArtMark2 = "vfngfbnfvppp";

    int b, nb, idx, num, ExtIdx, Category, ofs;
    char Id1, Id2;

    ExtIdx = (FileId & 0x70000000) >> 28;
    b = ArtCacheGetCpx( FileId );
    if( b != -1 ) FileId = b;
    gArtFilePath[0] = 0;
    nb = FileId & 0xFFF;
    idx = (FileId & 0xFF0000) >> 16;
    num = (FileId & 0xF000) >> 12;
    Category = (FileId & 0xF000000) >> 24;


    if( nb >= gArtCatalog[ Category ].NamesCount ) return 0;
    if( Category >= PROTOs ) return 0;

    ofs = ART_FNAME_SIZE * nb;
    if( Category == PROTO_CRITTERS ){
        if( ArtMakeFnameMark(idx, num, &Id2, &Id1) == -1 ) return 0;
        if( ExtIdx ){
            sprintf( gArtFilePath, "%s%s%s/%s%c%c.fr%c", gProtoDataFilePath, "art/", 
        	     gArtCatalog[ PROTO_CRITTERS ].SubDirName, &gArtCatalog[ PROTO_CRITTERS ].DirLstFname[ ofs ], Id1, Id2, ExtIdx + ('0' - 1)
        	   );
            return gArtFilePath;
        }
        sprintf( gArtFilePath, "%s%s%s/%s%c%c.frm", gProtoDataFilePath, "art/", 
    		 gArtCatalog[ PROTO_CRITTERS ].SubDirName, &gArtCatalog[ PROTO_CRITTERS ].DirLstFname[ ofs ], Id1, Id2
    		);
        return gArtFilePath;
    }
    if( Category == PROTO_HEADS ){
	Id2 = gArtMark2[ idx ];
	if( Id2 == 'f' )
    	    sprintf( gArtFilePath, "%s%s%s/%s%c%c%d.frm", gProtoDataFilePath, "art/", 
    		     gArtCatalog[ PROTO_HEADS ].SubDirName, &gArtCatalog[ PROTO_HEADS ].DirLstFname[ ofs ], gArtMark1[ idx ], 'f', num
    		   );
	else
    	    sprintf( gArtFilePath, "%s%s%s/%s%c%c.frm", gProtoDataFilePath, "art/", 
    		     gArtCatalog[ PROTO_HEADS ].SubDirName, &gArtCatalog[ PROTO_HEADS ].DirLstFname[ ofs ], gArtMark1[ idx ], Id2
    		   );
        return gArtFilePath;
    }

    sprintf( gArtFilePath, "%s%s%s/%s", gProtoDataFilePath, "art/", gArtCatalog[ Category ].SubDirName, &gArtCatalog[ Category ].DirLstFname[ ofs ] );
    return gArtFilePath;
}

int ArtReadLstFile( char *fname, char **Entries, int *Count )
{
    xFile_t *fh;
    int i;
    char *p, *q, stmp[ 200 ];
    
    if( !(fh = dbOpen( fname, "rt") ) ) return -1;
    for( i = 0; dbgets( stmp, 200, fh ); i++ ); // count lines
    dbseek( fh, 0, SEEK_SET ); // rewind
    *Count = i;
    if( !(p = (char *)Malloc( ART_FNAME_SIZE * i )) ){ dbClose( fh ); return -1; }    
    *Entries = p;
    for( i = 0; i < *Count; i++ ){
        dbgets( stmp, 200, fh );        
        if( (q = strpbrk( stmp, " ,;\r\t\n" )) ) *q = '\0'; // cut on white character
        strncpy( &p[ i * ART_FNAME_SIZE ], stmp, 12 );
        p[ i * ART_FNAME_SIZE + 12 ] = '\0';
    }
    dbClose( fh );
    return 0;
}

int ArtGetObjSpeed( ArtFrmHdr_t *Art )
{
    int spd;

    if( !Art ) return 10;
    if( !(spd = Art->Fps) ) return 10;
    return spd;
}

int ArtGetActionFrame( ArtFrmHdr_t *Art )
{
    if( Art ) return Art->ActionFrame;
    return -1;
}

int ArtGetFpd( ArtFrmHdr_t *Art )
{
    if ( Art ) return Art->Fpd;
    return -1;
}

int ArtGetObjWidth( ArtFrmHdr_t *Data, int FrameNo, int Orientation )
{
    ArtFrm_t *frm;
    
    if( !( frm = ArtGetFrameData(Data, FrameNo, Orientation ) ) ) return -1;
    return frm->Width;    
}

int ArtGetObjHeight( ArtFrmHdr_t *Data, int FrameNo, int Orientation )
{
    ArtFrm_t *frm;

    if( !(frm = ArtGetFrameData( Data, FrameNo, Orientation )) ) return -1;
    return frm->Height;
}

int ArtGetObjSize( ArtFrmHdr_t *Data, int FrameNo, int Orientation, int *pWidth, int *pHeight )
{
    ArtFrm_t *frm;
    
    if( !(frm = ArtGetFrameData( Data, FrameNo, Orientation)) ){
        if( pWidth  ) *pWidth = 0;
        if( pHeight ) *pHeight = 0;
        return -1;
    }
    if( pWidth  ) *pWidth  = frm->Width;
    if( pHeight ) *pHeight = frm->Height;
    return 0;
}

int ArtGetFrameShift( ArtFrmHdr_t *Data, int FrameNo, int Orientation, int *pShiftX, int *pShiftY )
{
    ArtFrm_t *frm;

    if( !(frm = ArtGetFrameData( Data, FrameNo, Orientation ) ) ) return -1;
    *pShiftX = (short)frm->PixShiftX;
    *pShiftY = (short)frm->PixShiftY;
    return 0;
}

int ArtGetCenter( ArtFrmHdr_t *ArtHdr, int Dir, int *pCenterX, int *pCenterY )
{
    if( !ArtHdr ) return -1;
    *pCenterX = ArtHdr->PixShiftX[ Dir ];
    *pCenterY = ArtHdr->PixShiftY[ Dir ];
    return 0;
}

char *ArtGetObjData( ArtFrmHdr_t *Art, int FrameNo, int Orientation )
{
    ArtFrm_t *frm;

    if( !( frm = ArtGetFrameData( Art, FrameNo, Orientation ) ) ) return NULL;
    return frm->Data;
}

ArtFrm_t *ArtGetFrameData( ArtFrmHdr_t *data, unsigned int FrameNo, unsigned int Orientation )
{
    int i;
    ArtFrm_t *p;

    if( Orientation >= 6 || !data ) return NULL;
    if( FrameNo < 0 || FrameNo >= data->Fpd ) return NULL;
    
    p = (ArtFrm_t *)((char *)data->FrameArea + (unsigned int)data->FirstFrame[ Orientation ]);
    for( i = 0; i < FrameNo; i++ ){
	p = (ArtFrm_t *)((char*)p + p->Pixels + sizeof( ArtFrm_t ));
    }
    return p;
}

int ArtFileExist( int FileId )
{
    char *fname;
    int len;
    
    if( !(fname = ArtMakeFileNameFromId( FileId ) ) ) return 0;
    return ( dbCheckFileLength( fname, &len ) == -1 ) ? 0 : 1;
}

int ArtFileExist1( int FileId )
{
    char *fname;
    int len;
    
    if( !(fname = ArtMakeFileNameFromId( FileId ) ) ) return 0;
    return ( dbCheckFileLength( fname, &len ) == -1 ) ? 0 : 1;
}

int ArtGetAnonAlias( int Idx)
{
    return gArtAnonAlias[ Idx ];
}

int ArtCritterFidShouldRunData( int Id )
{
    if ( (Id & 0xF000000) >> 24 != PROTO_CRITTERS ) return 0;
    return gArtCritterFidShouldRunData[ Id & 0xFFF ];
}

int ArtCacheGetCpx( int Id )
{
    int ct, nm;

    ct = (Id & 0x0F000000) >> 24;
    nm = (Id & 0x00FF0000) >> 16;
    if( ct != PROTO_CRITTERS ) return -1;
    if( (nm != 27 && nm != 29 && nm != 30 && nm != 55 && nm != 57 && nm != 58 && nm != 33 && nm != 64 ) ) return -1;
    
    return  ((((Id & 0x70000000) >> 28) << 28) & 0x70000000) | 
	    ((nm << 16) & 0xFF0000 ) | 
	    0x1000000 | 
	    (unsigned short)(((Id & 0xF000) >> 12) << 12) | 
	    ( gArtAnonAlias[Id & 0xFFF] & 0xFFF );
}

int ArtGetFileLen( int FileId, int *pLen )
{
    int error;
    char *path;
    int r;
    const char *fname;
    char strpath[260];
    int flen;

    error = -1;
    path = ArtMakeFileNameFromId( FileId );
    if( !path ) return -1;
    r = 0;
    if( gArtLangSubdir ){
        if( !(fname = strchr( path, '/' )) ) fname = path;
        sprintf( strpath, "art/%s/%s", gArtSubDir, fname );
        if( !dbCheckFileLength( strpath, &flen ) ) r = 1;
    }
    if( !r ) r = dbCheckFileLength( path, &flen ) == 0;
    if( r ){
        error = 0;
        *pLen = flen;
    }
    return error;
}

int ArtLoadImage( int FileIdCode, int *Size, ArtFrmHdr_t *Data )
{
    int err;
    char *fname, *p, str[ 260 ];

    if( !( fname = ArtMakeFileNameFromId( FileIdCode ) ) ) return -1;    
    err = 0;
    if( gArtLangSubdir ){
        if( !(p = strchr( fname , '/' )) ) p = fname;
        sprintf(str, "art/%s/%s", gArtSubDir, p);
        if( !ArtLoadFrmFile( str, Data ) ) err = 1;
    }

    if( !err ) err = ArtLoadFrmFile( fname, Data ) == 0;
    if( !err ) return -1;
    *Size = Data->AllDataSize + sizeof( ArtFrmHdr_t ) + sizeof( ArtFrm_t ); //74
    return 0;        
}

void ArtFree( void *n )
{
    Free(n);
}

int ArtMakeId( int Category,short fidx, short num,short Args, int a5 )
{
    int FrSplit = fidx;
    int cc;

    if( Category != PROTO_CRITTERS ) goto LABEL_8;
    if( num == 33 ) goto LABEL_8;
    if( num < 20 ) goto LABEL_8;
    if( num > 35 ) goto LABEL_8;
    cc = (Args << 12) | ((num << 16) & 0xFF0000) | 0x1000000;
    if( ArtFileExist( (fidx & 0xFFF) | ((a5 << 28) & 0x70000000) | cc) ) goto xx;
    if( FrSplit == 1 ) goto LABEL_8;
    if( ArtFileExist( (fidx & 0xFFF) | cc | 0x10000000) ) goto xx;
    goto LABEL_8;
        
LABEL_8:
    FrSplit = 0;
xx:
    return ((FrSplit << 28) & 0x70000000) | (Category << 24) | ((num << 16) & 0xFF0000) | (Args << 12) | (fidx & 0xFFF);
}

int ArtLoadFrame( ArtFrm_t *Data, xFile_t *fh, int Frames )
{
    ArtFrm_t *p;
    int i, tmpi;
    short tmpw;
    p = Data;
    for( i = 0; i < Frames; i++ ){
        if( dbgetBew( fh, &tmpw ) == -1 ) return -1;
	p->Width = tmpw;
        if( dbgetBew( fh, &tmpw ) == -1 ) return -1;
        p->Height = tmpw;
        if( dbgetBei( fh, &tmpi ) == -1 ) return -1;
        p->Pixels = tmpi;
        if( dbgetBew( fh, &tmpw ) == -1 ) return -1;
        p->PixShiftX = tmpw;
        if( dbgetBew( fh, &tmpw ) == -1 ) return -1;
	p->PixShiftY = tmpw;
        if( dbread( p->Data, p->Pixels, 1, fh ) != 1 ) return -1;
        p = (ArtFrm_t *)( (char *)&p->Data + p->Pixels );
    }
    return 0;        
}

int ArtLoadFrmHdr( ArtFrmHdr_t *hdr, xFile_t *fh )
{
    int i, tmp2[ 6 ];
    short tmp1[ 6 ];
    
    if( dbgetBei( fh, &tmp2[0] ) == -1 ) return -1;
    hdr->Version = tmp2[0];
    if( dbgetBew( fh, &tmp1[0] ) == -1 ) return -1;
    hdr->Fps = tmp1[0];
    if( dbgetBew( fh, &tmp1[0] ) == -1) return -1;
    hdr->ActionFrame = tmp1[0];
    if( dbgetBew( fh, &tmp1[0] ) == -1 ) return -1;
    hdr->Fpd = tmp1[0];
    if( dbreadBewBlk( fh, tmp1, 6 ) == -1) return -1;
    for( i = 0; i < 6; i++ ) hdr->PixShiftX[ i ] = tmp1[ i ];
    if( dbreadBewBlk( fh, tmp1, 6 ) == -1) return -1;
    for( i = 0; i < 6; i++ ) hdr->PixShiftY[ i ] = tmp1[ i ];
    if( dbreadBeiBlk( fh, tmp2, 6 ) == -1) return -1;
    for( i = 0; i < 6; i++ ) hdr->FirstFrame[ i ] = tmp2[ i ];
    if( dbgetBei( fh, &tmp2[0] ) == -1 ) return -1;    
    hdr->AllDataSize = tmp2[0];
    return 0;
}

int ArtLoadFile( char *fname, ArtFrmHdr_t **data )
{
    ArtFrmHdr_t *p;
    xFile_t *fh;
    int i, len;

    if( dbCheckFileLength( fname, &len ) == -1 ) return -2;
    if( !(p = Malloc( len )) ) return -1;    
    *data = p;
    if( !(fh = dbOpen( fname, "rb" ) ) ) return -2;
    if( ArtLoadFrmHdr( *data, fh ) ){ dbClose( fh ); Free( *data ); return -3; }
    for( i = 0; i < 6; i++ ){
        if( (i == 0) || ( p->FirstFrame[ i ] != p->FirstFrame[ 0 ] ) ){
    	    if( ArtLoadFrame( (ArtFrm_t *)((char *)&p->FrameArea + p->FirstFrame[ i ] ), fh, p->Fpd ) ) break;
    	}
    }
    dbClose( fh );
    if( i < 6 ){ Free( *data ); return -5; }
    return 0;
}

int ArtLoadFrmFile( char *fpath, ArtFrmHdr_t *data )
{
    xFile_t *fh;
    int i;
#ifdef DBG_SHOW_FNAMES
    printf("Fname:%s\n", fpath);
#endif

    if( !(fh = dbOpen(fpath, "rb")) ) return -2;

    if( ArtLoadFrmHdr( data, fh ) ){ dbClose( fh ); return -3; }    

    for( i = 0; i < 6; i++ ){
        if( ((i == 0) || data->FirstFrame[0] != data->FirstFrame[i])){ 
    	    if( ArtLoadFrame( (ArtFrm_t *)( (char *)&data->FrameArea + data->FirstFrame[i] ), fh, data->Fpd ) ){
		dbClose( fh ); 
		return -5;
    	    }
        }
    }
    dbClose( fh ); 
    return 0;
}

int ArtSaveFrame( ArtFrm_t *Data, xFile_t *fh, int Frames )
{
    ArtFrm_t *p;
    char *pdat;
    int i;

    p = Data;
    for( i = 0; i < Frames; i++ ){
        if( dbputBew(fh, p->Width ) == -1 ) return -1;
        if( dbputBew(fh, p->Height ) == -1 ) return -1;
        if( dbputBei(fh, p->Pixels ) == -1 ) return -1;
        if( dbputBew(fh, p->PixShiftX ) == -1 ) return -1;
        if( dbputBew(fh, p->PixShiftY ) == -1 ) return -1;
        pdat = p->Data;
        if( dbwrite( pdat, p->Pixels, 1, fh ) != 1 ) return -1;
        p = (ArtFrm_t *)&pdat[ p->Pixels ];
    }
    return 0;
}

int ArtSaveHdr( ArtFrmHdr_t *Data, xFile_t *fh )
{
    int i, tmp2[6];
    short tmp1[6];
    
    if( dbputBei( fh, Data->Version ) == -1 ) return -1;
    if( dbputBew( fh, Data->Fps ) == -1 ) return -1;
    if( dbputBew( fh, Data->ActionFrame ) == -1 ) return -1;
    if( dbputBew( fh, Data->Fpd ) == -1 ) return -1;

    for( i = 0; i < 6; i++ ) tmp1[i] = Data->PixShiftX[ i ]; 
    if( dbputBewBlk( fh, tmp1, 6 ) == -1 ) return -1;
    for( i = 0; i < 6; i++ ) tmp1[i] = Data->PixShiftY[ i ]; 
    if( dbputBewBlk( fh, tmp1, 6 ) == -1 ) return -1;
    for( i = 0; i < 6; i++ ) tmp2[i] = Data->FirstFrame[ i ]; 
    if( dbputBeiBlk( fh, tmp2, 6 ) == -1 ) return -1;
    if( dbputBei( fh, (int)Data->AllDataSize ) == -1 ) return -1;
    return 0;
}

int ArtSave( char *fname, ArtFrmHdr_t *ArtData )
{
    xFile_t *fh;
    ArtFrmHdr_t *p;
    int i;

    if( !ArtData ) return -1;    
    if( !(fh = dbOpen( fname, "wb" )) ) return -1;
    if( ArtSaveHdr( ArtData, fh ) ){ dbClose( fh ); return -1; }        
    p = ArtData;
    for( i = 0; i < 6; i++ ){
        if( (i == 0) || (ArtData->FirstFrame[0] != p->FirstFrame[0] ) ){
    	    if( ArtSaveFrame( (ArtFrm_t *)((char *)&ArtData->FrameArea + p->FirstFrame[0]), fh, ArtData->Fpd ) ) break;
        }
        p = (ArtFrmHdr_t *)((char *)p + 4);
    }
    dbClose( fh ); 
    if( i < 6 ) return -1; 
    return 0;     
}


