#pragma once
#define ART_FNAME_SIZE	13
#define ART_DEFAULT_CACHE_SIZE_MB	8
#define ART_NULL	((void *)-1)

#define ART_IDX( obj )	((obj->ImgId) & 0xfff)
#define ART_TYP( obj )  (((obj->ImgId) & 0xf000) >> 12)
#define ART_GRP( obj )  (((obj->ImgId) & 0x70000000) >> 28)
#define ARTT( pid )	(((pid) >> 24) & 0xff)

// prototype categories
enum{
    PROTO_ITEMS,	// 0
    PROTO_CRITTERS,	// 1
    PROTO_SCENERY,	// 2
    PROTO_WALLS,	// 3
    PROTO_TILES,	// 4
    PROTO_MISC,		// 5
    PROTO_INTRFACE,	// 6
    PROTO_INVEN,	// 7
    PROTO_HEADS,	// 8
    PROTO_BACKGRND,	// 9
    PROTO_SKILLDEX,	// 10
    PROTOs
};

typedef struct
{
    short x;
    short y;
} PosXY_t;

typedef struct
{
    int x;
    int y;
} PosXYi_t;

typedef struct // size of 32
{
  int Flags;		// 0,
  char SubDirName[16];	//
  char *DirLstFname;	//
  char *p02;		//
  int NamesCount;	//
} ProtoCat_t;

// 
typedef struct  // size of 12
{
  int Arg0;	// first argument
  int Arg1;	// second argument
  int Arg2;	// third argument
} ArtHeadsInfo_t;

// *.Frm file subheader of frame area
typedef struct // size of 12
{
  unsigned short	Width;		// 0 2 frame width
  unsigned short	Height;		// 2 2 frame height
  unsigned int		Pixels;		// 6 4 total number of pixels in frame ( and hence data bytes in frame )
  union{
    short PixShiftX;			// 8 2 pixel shift in X direction of frame - the pixel offset of this frame from the previous one.
    short Frames;
  };
  unsigned short PixShiftY;		// 10 2 pixel shift in Y direction of frame - the pixel offset of this frame from the previous one.
  char Data[];
} __attribute__((packed)) ArtFrm_t;

// *.Frm file header
typedef struct	// size of 62
{
  unsigned int		Version;	// 4
  unsigned short 	Fps;		// 2
  unsigned short 	ActionFrame;	// 2
  unsigned short 	Fpd;		// 2  -> frames per direction
  short 		PixShiftX[6];	// 12
  short 		PixShiftY[6];	// 12
  unsigned int 		FirstFrame[6];	// 24 -> offsets of the first frame for each direction from beginning of the frame area
  unsigned int 		AllDataSize;	// 4  -> size of frame area
  ArtFrm_t 		FrameArea[];	// Frame Area
} __attribute__((packed))  ArtFrmHdr_t;

extern char gProtoDataFilePath[ 260 ];
extern ProtoCat_t gArtCatalog[ PROTOs ];
extern int gArtLangSubdir;
extern int gArtPrep[6];
extern char gArtSubDir[ 32 ];
extern Cache_t gArtCacheDb;
extern ArtHeadsInfo_t *gArtHeadsInfo;
extern int *gArtAnonAlias;
extern int *gArtCritterFidShouldRunData;

int ArtInit();
void ArtCacheFree();
char *ArtGetCatDirName( unsigned int Category );
int ArtIsHidden( unsigned int ObjType );
void ArtGetCatToggleFlag0( unsigned int Category );
int ArtGetFilesCount( unsigned int Category );

/*
    Id32:[ii aa .nnn]: ii - category[8], aa[8] - args, . - unknown, nnn - file number 
    0..11 - FileNo    
*/
int ArtGetHeadsInfo( int Id );
void ArtLoadImgScaled( int Id, char *pDst, int DstWidth, int DstHeight, int DstPitch );
ArtFrmHdr_t *ArtLoadImg( int Id, CachePool_t **Obj );
char *ArtGetBitmap( int Id, int FrNo, int Orientation, CachePool_t **Obj );
char *ArtLoadBmp( int IdCode, CachePool_t **Img, int *pWidth, int *pHeight );
int ArtClose( CachePool_t *Block );
int ArtCacheCleaner();
int ArtUnk01( int a1, int a2, int a3 );
int ArtGetFilenameFromList( int Category, int FileIdx, char *fname );
int ArtMakeFnameMark( int id, int Category, char *Mark0, char *Mark1 );
char *ArtMakeFileNameFromId( int FileId );
int ArtReadLstFile( char *fname, char **Entries, int *Count );
int ArtGetObjSpeed( ArtFrmHdr_t *Art );
int ArtGetActionFrame( ArtFrmHdr_t *Art );
int ArtGetFpd( ArtFrmHdr_t *Art ); // returns frames per direction
int ArtGetObjWidth( ArtFrmHdr_t *Data, int FrameNo, int Orientation );
int ArtGetObjHeight( ArtFrmHdr_t *Data, int FrameNo, int Orientation );
int ArtGetObjSize( ArtFrmHdr_t *Data, int FrameNo, int Orientation, int *pWidth, int *pHeight );
int ArtGetFrameShift( ArtFrmHdr_t *Art, int FrameNo, int Orientation, int *pShiftX, int *pShiftY );
int ArtGetCenter( ArtFrmHdr_t *Art, int Direction, int *pCenterX, int *pCenterY );
char *ArtGetObjData( ArtFrmHdr_t *Art, int FrameNo, int Orientation );
ArtFrm_t *ArtGetFrameData( ArtFrmHdr_t *data, unsigned int FrameNo, unsigned int Orientation );
int ArtFileExist( int FileId );
int ArtFileExist1( int FileId );
int ArtGetAnonAlias( int Idx );
int ArtCritterFidShouldRunData( int Id );
int ArtCacheGetCpx( int Id );
int ArtGetFileLen( int FileId, int *pLen);
int ArtLoadImage( int FileIdCode, int *Size, ArtFrmHdr_t *Data );
void ArtFree( void *n );
int ArtMakeId( int Category, short fidx, short num, short Args, int a5 );
int ArtLoadFrame( ArtFrm_t *Data, xFile_t *fh, int Frame );
int ArtLoadFrmHdr( ArtFrmHdr_t *hdr, xFile_t *fh );
int ArtLoadFile( char *fname, ArtFrmHdr_t **data );
int ArtLoadFrmFile( char *fpath, ArtFrmHdr_t *data );
int ArtSaveFrame( ArtFrm_t *Data, xFile_t *fh, int Frames );
int ArtSaveHdr( ArtFrmHdr_t *Data, xFile_t *fh );
int ArtSave( char *fname, ArtFrmHdr_t *ArtData );
int ArtExpandCache();
