#include "FrameWork.h"

//#define DEBUG

char  gAcmUnk06[11][11];
char  gAcmUnk05[3][3][3];
short gAcmUnk04[5][5][5];
int *gAcmPcmTable;
int *gAcmPcmAlloc;
int gAcmOpenedCnt = 0;
int gAcmMxInited = 0;

int AcmFiller1( Acm_t *sfx, int Column, int a );
int AcmFiller2( Acm_t *sfx, int Column, int a );
int AcmFiller3( Acm_t *sfx, int Column, int a );
int AcmFiller4( Acm_t *sfx, int Column, int a );
int AcmFiller5( Acm_t *sfx, int Column, int a );
int AcmFiller6( Acm_t *sfx, int Column, int a );
int AcmFiller7( Acm_t *sfx, int Column, int a );
int AcmFiller8( Acm_t *sfx, int Column, int a );
int AcmFiller9( Acm_t *sfx, int Column, int a );
int AcmFiller10( Acm_t *sfx, int Column, int a );
int AcmFiller11( Acm_t *sfx, int Column, int a );
int AcmFiller12( Acm_t *sfx, int Column, int a );
int AcmFiller13( Acm_t *sfx, int Column, int a );
int AcmFiller14( Acm_t *sfx, int Column, int a );

int (*gAcmFiller[32])( Acm_t *, int, int ) = {
    AcmFiller1, AcmFiller2, AcmFiller2, AcmFiller3,	// 0..3
    AcmFiller3, AcmFiller3, AcmFiller3, AcmFiller3,	// 4..7
    AcmFiller3, AcmFiller3, AcmFiller3, AcmFiller3,	// 8..11
    AcmFiller3, AcmFiller3, AcmFiller3, AcmFiller3,	// 12..15
    AcmFiller3, AcmFiller4, AcmFiller5, AcmFiller6,	// 16..19
    AcmFiller7, AcmFiller8, AcmFiller9, AcmFiller10,	// 20..23
    AcmFiller11,AcmFiller2, AcmFiller12,AcmFiller13,	// 24..27
    AcmFiller2, AcmFiller14,AcmFiller2, AcmFiller2,	// 28..31
};


#define GET_BITBLOK( dat, bits )	\
    for( ;acm->Bits < bits; acm->Bits += 8 ){\
        if( --acm->BitBlkAvail < 0 ){\
            c = AcmGetBitBlk( acm );\
        } else {\
            c = *acm->pBitBlk++;\
        }\
        acm->BitBlock |= ( unsigned char )c << (acm->Bits & 0xff);\
    }\
    dat = acm->BitBlock & ((1 << bits) - 1);

#define SHIFT_BITBLOCK( bits )	acm->BitBlock >>= bits; acm->Bits -= bits;

/*********************************************/

int AcmInit( Acm_t *acm, int (*Cb)(void *file, void *data, int size), void *FileHandler )
{
    acm->Cb = (void *)Cb;
    acm->File = FileHandler;
    if( !( acm->Buf1 = (char *)malloc( 512 ) ) ) return 0;
    acm->Buf1Size = 512;
    acm->BitBlkAvail = 0;    
    return 1;
}

char AcmGetBitBlk( Acm_t *acm )
{
    int e, a;
    char *b;

    e = acm->Cb( acm->File, acm->Buf1, acm->Buf1Size );
    acm->BitBlkAvail = e;
    if( !e ){
        memset(acm->Buf1, 0, acm->Buf1Size);
        acm->BitBlkAvail = acm->Buf1Size;
    }
    a = acm->BitBlkAvail;
    acm->pBitBlk = acm->Buf1;
    b = acm->pBitBlk;
    acm->BitBlkAvail = a - 1;
    acm->pBitBlk = b + 1;
    return *b;
}

int AcmAlloc( Acm_t *acm, int (*cb)(void *arg, int nsize, char *buf), void *file )
{
    acm->Cb = (void *)cb;
    acm->File = file;
    if( !(acm->Buf1 = malloc( 512 )) ) return 0;
    acm->Buf1Size = 512;
    acm->BitBlkAvail = 0;
    acm->BitBlock = 0;
    acm->Bits = 0;    
    return 1;
}

void AcmMxBuild()
{
    int j, i, k;

    if( gAcmMxInited ) return;
        for( k = 0; k < 3; k++ ){
            for( j = 0; j < 3; j++ ){
                for(i = 0; i < 3; i++ ){
                    gAcmUnk05[i][j][k] = k + j * 4 + i * 16;
                }
            }
        }

        for( k = 0; k < 5; k++ ){
            for( j = 0; j < 5; j++ ){
                for( i = 0; i < 5; i++ ){
                    gAcmUnk04[i][j][k] = k + j * 8 + i * 64;
                }
            }
        }

        for( i = 0; i < 11; i++ ){
            for( j = 0; j < 11; j++ ){
                gAcmUnk06[j][i] = i + j*16;
            }
        }
        gAcmMxInited = 1;
}

void AcmSetPos( Acm_t *acm, int i, int col, int idx )
{
    acm->block[ (i * acm->Cols) + col ] = gAcmPcmTable[ idx ];
}

void AcmSetZero( Acm_t *acm, int i, int col )
{
    acm->block[ (i * acm->Cols) + col ] = 0;
}

int AcmFiller1( Acm_t *acm, int Column, int a ) // fill the Column with zero
{
    int i;

    for( i = 0; i < acm->Rows; i++ ){
        acm->block[ Column + acm->Cols * i ] = 0;
    }
    return 1;
}

int AcmFiller2( Acm_t *sfx, int Column, int a ) // start a new bitblock
{
    return 0;
}

int AcmFiller3( Acm_t *acm, int Column, int Bits ) // linear
{
    int i, c, dd;
    int middle;

    middle = 1 << (Bits - 1);
    for( i = 0; i < acm->Rows; i++ ){
	GET_BITBLOK( dd, Bits )
	SHIFT_BITBLOCK( Bits )
        AcmSetPos( acm, i, Column, dd - middle );
    }
    return 1;
}

int AcmFiller4( Acm_t *acm, int col, int a )
{
    int i, dd, c;

    for( i = 0; i < acm->Rows; i++ ){
    	GET_BITBLOK( dd, 3 )
        if( (dd & 1) == 0 ){
    	    SHIFT_BITBLOCK( 1 )
	    AcmSetZero( acm, i, col );
    	    if( ++i >= acm->Rows ) break;
    	    AcmSetZero( acm, i, col );
    	    continue;
        }     
        if( (dd & 2) == 0 ){
    	    SHIFT_BITBLOCK( 2 )
    	    AcmSetZero( acm, i, col );
    	    continue;
    	}
    	SHIFT_BITBLOCK( 3 )
    	AcmSetPos( acm, i, col, (dd & 0x04) ? 1 : -1 );
    }
    return 1;
}

int AcmFiller5( Acm_t *acm, int col, int a )
{
    int i, dd, c;

    for( i = 0; i < acm->Rows; i++ ){
    	GET_BITBLOK( dd, 2 )
        if( (dd & 1) == 0 ){
    	    SHIFT_BITBLOCK( 1 )
    	    AcmSetZero( acm, i, col );
        } else {
    	    SHIFT_BITBLOCK( 2 )
    	    AcmSetPos( acm, i, col, (dd & 0x02) ? 1 : -1 );
        }
    }
    return 1;
}

int AcmFiller6( Acm_t *acm, int col, int a )
{
    unsigned int i, dd, c;
    int n;

    for( i = 0; i < acm->Rows; i++ ){
	GET_BITBLOK( dd, 5 )
        SHIFT_BITBLOCK( 5 )
        n = gAcmUnk05[0][0][dd];
        AcmSetPos( acm, i, col, ( n & 3 ) - 1 );
        if( ++i >= acm->Rows ) break;
        AcmSetPos( acm, i, col, ( ( n >> 2 ) & 3) - 1 );
        if( ++i >= acm->Rows ) break;
        AcmSetPos( acm, i, col, ( n >> 4 ) - 1 );
    }
    return 1;
}

int AcmFiller7( Acm_t *acm, int col, int a)
{
    int i, n, dd, c;

    for( i = 0; i < acm->Rows; i++ ){
    	GET_BITBLOK( dd, 4 )
        if( (dd & 1) == 0 ){
    	    SHIFT_BITBLOCK( 1 )
            AcmSetZero( acm, i, col );
            if( ++i >= acm->Rows )  return 1;
            AcmSetZero( acm, i, col );
            continue;
        }
        if( (dd & 2) == 0 ){
    	    SHIFT_BITBLOCK( 2 )
            AcmSetZero( acm, i, col );
            continue;
        }
        SHIFT_BITBLOCK( 4 )
        if( dd & 0x08 ){
            n = ( dd & 0x04 ) ? 2 : 1;
        } else {
    	    n = ( dd & 0x04 ) ? -1 : -2;
        }
        AcmSetPos( acm, i, col, n );
    }
    return 1;
}

int AcmFiller8( Acm_t *acm, int col, int a )
{
    int i, n, dd, c;

    for( i = 0; i < acm->Rows; i++ ){
    	GET_BITBLOK( dd, 3 )
        if( (dd & 1) == 0 ){
    	    SHIFT_BITBLOCK( 1 )
            AcmSetZero( acm, i, col );
            continue;
        }
    	SHIFT_BITBLOCK( 3 )
        if( dd & 0x04 ){
            n = ( dd & 0x02 ) ? 2 : 1;
        } else {
    	    n = ( dd & 0x02 ) ? -1 : -2;
        }
        AcmSetPos( acm, i, col, n );
    }
    return 1;
}

int AcmFiller9( Acm_t *acm, int Column, int a )
{
    return AcmUnk01( acm, Column );
}

int AcmUnk01( Acm_t *acm, int col )
{
    int i, c, dd, n;

    for( i = 0; i < acm->Rows; i++ ){
	GET_BITBLOK( dd, 7 )
        SHIFT_BITBLOCK( 7 )
        n = gAcmUnk04[0][0][dd];
        AcmSetPos( acm, i, col, (n & 7) - 2 );        
        if( ++i >= acm->Rows ) break;
	AcmSetPos( acm, i, col, ( ( n >> 3 ) & 7) - 2 );
	if( ++i >= acm->Rows ) break;
	AcmSetPos( acm, i, col, ( n >> 6 ) - 2 );
    }
    return 1;
}

int AcmFiller10( Acm_t *acm, int col, int a )
{
    int i, n, c, dd;

    for( i = 0 ; i < acm->Rows; i++ ){
        GET_BITBLOK( dd, 5 )
        if( (dd & 1) == 0 ){
    	    SHIFT_BITBLOCK( 1 )
            AcmSetZero( acm, i, col );
            if( ++i >= acm->Rows ) return 1;
            AcmSetZero( acm, i, col );
            continue;
        }
        if( (dd & 2) == 0 ){
    	    SHIFT_BITBLOCK( 2 )
            AcmSetZero( acm, i, col );
            continue;
        }
        if( (dd & 4) == 0 ){
    	    SHIFT_BITBLOCK( 4 )
            AcmSetPos( acm, i, col, ( dd & 8 ) ? 1 : -1 );
    	    continue;
        }
        SHIFT_BITBLOCK( 5 )
        n = (dd >> 3) & 3;
        if( n >= 2 ) n += 3;
        AcmSetPos( acm, i, col, n - 3 );
    }
    return 1;
}

int AcmFiller11( Acm_t *acm, int col, int a )
{
    int i, n, c, dd;

    for( i = 0 ; i < acm->Rows; i++ ){
    	GET_BITBLOK( dd, 4 )
        if( (dd & 1) == 0 ){
    	    SHIFT_BITBLOCK( 1 )
	    AcmSetZero( acm, i, col );
            continue;
        }
        if( (dd & 2) == 0 ){
    	    SHIFT_BITBLOCK( 3 )
            AcmSetPos( acm, i, col, ( dd & 0x04 ) ? 1 : -1 );
            continue;
        }
        SHIFT_BITBLOCK( 4 )
        n = (dd >> 2) & 3;
        if( n >= 2 ) n += 3;
        AcmSetPos( acm, i, col, n - 3 );
    }
    return 1;
}

int AcmFiller12( Acm_t *acm, int col, int a )
{
    int i, c, dd, n;

    for( i = 0; i < acm->Rows; i++ ){
    	GET_BITBLOK( dd, 5 )
        if( (dd & 1) == 0 ){
    	    SHIFT_BITBLOCK( 1 )
            AcmSetZero( acm, i, col );
            if( ++i >= acm->Rows ) return 1;
            AcmSetZero( acm, i, col );
            continue;
        }
        if( (dd & 2) == 0 ){
    	    SHIFT_BITBLOCK( 2 )
            AcmSetZero( acm, i, col );
            continue;
        }
        SHIFT_BITBLOCK( 5 )
        n = (dd >> 2) & 7;
        if( n >= 4 ) n++;
        AcmSetPos( acm, i, col, n - 4 );
    }
    return 1;
}

int AcmFiller13( Acm_t *acm, int col, int a )
{
    int i, c, dd, n;    

    for( i = 0; i < acm->Rows; i++ ){
    	GET_BITBLOK( dd, 4 )
        if( (dd & 1) == 0 ){
    	    SHIFT_BITBLOCK( 1 )
            AcmSetZero( acm, i, col );
            continue;
        }
        SHIFT_BITBLOCK( 4 )
        n = (dd >> 1) & 7;
        if( n >= 4 ) n++;
        AcmSetPos( acm, i, col, n - 4 );
    }
    return 1;
}

int AcmFiller14( Acm_t *acm, int col, int a )
{
    int i, c, dd, n;

    for( i = 0; i < acm->Rows; i++ ){
	GET_BITBLOK( dd, 7 )
        SHIFT_BITBLOCK( 7 )

        n = gAcmUnk06[ 0 ][ dd ];
        AcmSetPos( acm, i, col, (n & 0x0f) - 5 );
        if( ++i >= acm->Rows ) break;
        AcmSetPos( acm, i, col, (n >> 4) - 5 );
    }
    return 1;
}

/*********************************************/

int AcmProc( Acm_t *acm )
{
    unsigned int ef;
    unsigned char c;
    int i,n,k;
    char pwr;
    short val;

    // 4 bit get 'pwr'
    GET_BITBLOK( pwr, 4 )
    SHIFT_BITBLOCK( 4 )

    // 16 bit val
    GET_BITBLOK( val, 16 )
    SHIFT_BITBLOCK( 16 )
    n = (1 << pwr);
    for( i = 0, k = 0; i < n; i++, k += val ) gAcmPcmTable[ i ] = k;
    for( i = 1, k = -val; i <= n; i++, k -= val ) gAcmPcmTable[ -i ] = k;

    AcmMxBuild();
    if( acm->Cols <= 0 )return 1;
    for( i = 0; i < acm->Cols; i++ ){
	GET_BITBLOK( ef, 5 )
	SHIFT_BITBLOCK( 5 )
        if( !gAcmFiller[ ef ]( acm, i, ef ) ) return 0;
    }
    return 1;
}

void AcmJuggle16( short *pWrap, int *pBlk, int sub_cnt, int sub_len )
{
    int i, j, r0,r1,r2,r3, *p;

    for( j = 0; j < sub_len; j++, pWrap += 2, pBlk++ ){
	p = pBlk;
        r0 = pWrap[0];
        r1 = pWrap[1];
        for( i = 0; i < (sub_cnt / 2); i++ ){
            r2 = *p; *p = 2 * r1 + r0 + *p; p += sub_len;
            r3 = *p; *p = 2 * r2 - r1 - *p; p += sub_len;
            r0 = r2; r1 = r3;
        }
        pWrap[0] = r0;
        pWrap[1] = r1;
    }
}

void AcmJuggle32( int *pWrap, int *pBlk, int sub_cnt, int sub_len )
{
    int j, r0, r1, r2, r3, i, *p;

    for( j = 0; j < sub_len; j++, pWrap += 2, pBlk++ ){
        p = pBlk;
        r0 = pWrap[0];
        r1 = pWrap[1];
        for( i = 0; i < (sub_cnt / 2); i++ ){
            r2 = *p; *p = 2 * r1 + r0 + *p;  p += sub_len;
            r3 = *p; *p = 2 * r2 - r1 - *p;  p += sub_len;
            r0 = r2; r1 = r3;
        }
        pWrap[0] = r0;
        pWrap[1] = r1;            
    }
}

void AcmJuggleBlock( Acm_t *acm )
{
    int sub_len, sub_cnt, *p,i, *pWrap, j, *pBlk;

    if( acm->RecursLvl == 0 ) return;        
    pBlk = acm->block;
    for( j = acm->Rows; j > 0; j -= acm->StepSubCnt, pBlk += acm->StepBytes ){
        sub_cnt = acm->StepSubCnt;
        sub_len = acm->Cols / 2;
        if( sub_cnt > j ) sub_cnt = j;
        sub_cnt *= 2;
        AcmJuggle16( (short *)acm->WrapBuf, pBlk, sub_cnt, sub_len );        
        pWrap = &acm->WrapBuf[ sub_len ];
        for( i = 0, p = pBlk; i < sub_cnt; i++, p += sub_len ) p[0]++;
        while( sub_len > 1 ){
            sub_len /= 2;
            sub_cnt *= 2;
            AcmJuggle32( pWrap, pBlk, sub_cnt, sub_len );
            pWrap += sub_len * 2;
        }            
    }    
}

/*
int AcmDecodeBlock( Acm_t *acm )
{
    if( !AcmProc( acm ) ) return 0;
    AcmJuggleBlock( acm );
    acm->ReadPtr = acm->block;
    acm->ReadCnt = acm->OutAvail;
    acm->ReadAvail -= acm->OutAvail;
    if( acm->ReadAvail >= 0 ) return 1;
    acm->ReadAvail = 0;
    acm->ReadCnt += acm->ReadAvail;
    return 1;
}
*/

int AcmInflate( Acm_t *acm, short *data, int nbytes )
{
    int *rdat;
    int nin;
    unsigned int i;

    rdat = acm->ReadPtr;
    nin  = acm->ReadCnt;
    for( i = 0; i < nbytes; i += 2, nin-- ){

        if( nin == 0 ){
            if( !acm->ReadAvail ) break;
            if( !AcmProc( acm ) ) break;
            AcmJuggleBlock( acm );
            acm->ReadPtr = acm->block;
            acm->ReadCnt = acm->OutAvail;
            acm->ReadAvail -= acm->OutAvail;
            if( acm->ReadAvail < 0 ){
                acm->ReadCnt += acm->ReadAvail;
                acm->ReadAvail = 0;
            }
            rdat = acm->ReadPtr;
            nin = acm->ReadCnt;
        }

        *data++ = (*rdat++) >> acm->RecursLvl;

    }
    acm->ReadPtr = rdat;
    acm->ReadCnt = nin;
    return i;
}

void AcmClose( Acm_t *acm )
{
    if( acm->Buf1 ) free( acm->Buf1 );
    if( acm->WrapBuf ) free( acm->WrapBuf );
    if( acm->block ) free( acm->block );
    free( acm );
    if( --gAcmOpenedCnt ) return;    
    if( !gAcmPcmAlloc ) return;        
    free( gAcmPcmAlloc );
    gAcmPcmAlloc = NULL;    
}

Acm_t *AcmOpen(int (*ReadCb)(void *Fh, void *data, int size), void *file, int *pnChannels,int *pFreq,int *pnSamples )
{
    Acm_t *acm;
    int dat, b;
    char c;

    if( !(acm = (Acm_t *)malloc( sizeof( Acm_t ) ) ) ) return NULL;
    memset( acm, 0x00, sizeof( Acm_t ) );
    gAcmOpenedCnt++;

    if( !AcmInit( acm, ReadCb, file ) ){ AcmClose( acm ); *pnChannels = *pFreq = *pnSamples = 0; return NULL; }

    acm->BitBlock = 0;
    acm->Bits = 0;
    // 24 bit of ACM signature 1
    GET_BITBLOK( dat, 24 )
    SHIFT_BITBLOCK( 24 )
    if( (dat & 0xFFFFFF) != ACM_SIGNATURE ) { AcmClose( acm ); *pnChannels = *pFreq = *pnSamples = 0; return 0; }

    // 8 bit version
    GET_BITBLOK( dat, 8 )
    SHIFT_BITBLOCK( 8 )

    if( (unsigned char)dat != 1 ) { AcmClose( acm ); *pnChannels = *pFreq = *pnSamples = 0; return 0; }

    // 2x16 bit -> 32bit samples count
    GET_BITBLOK( acm->ReadAvail, 16 )
    SHIFT_BITBLOCK( 16 )
    GET_BITBLOK( dat, 16 )
    acm->ReadAvail |= (dat << 16);
    SHIFT_BITBLOCK( 16 )

    // 16 bit channels
    GET_BITBLOK( acm->nChannels, 16 )
    SHIFT_BITBLOCK( 16 )

    // 16 bit of frequency
    GET_BITBLOK( acm->Frequency, 16 )
    SHIFT_BITBLOCK( 16 )

    // 4 bit attributes 1 recursion level
    GET_BITBLOK( acm->RecursLvl, 4 )
    SHIFT_BITBLOCK( 4 )

    // 12 bit - number of sub blocks in a packed block
    GET_BITBLOK( acm->Rows, 12 )
    SHIFT_BITBLOCK( 12 )

    acm->Cols = 1 << acm->RecursLvl; // size of one sub block
    acm->OutAvail = acm->Rows * acm->Cols; // size of one packed block

    b = ( acm->RecursLvl ) ? ( 3 * acm->Cols / 2 - 2 ) : 0;

    acm->StepSubCnt = 2048 / acm->Cols - 2;
    if( acm->StepSubCnt < 1 ) acm->StepSubCnt = 1;
    acm->StepBytes = acm->StepSubCnt * acm->Cols;
    if( b ){
        if( !(acm->WrapBuf = (int *)malloc( b * sizeof(int) )) ) { AcmClose( acm ); *pnChannels = *pFreq = *pnSamples = 0; return 0; }
        memset( acm->WrapBuf, 0x00, b * sizeof(int) );
    }    
    if( !( acm->block = ( int * )malloc( acm->OutAvail * sizeof( int ) ) ) ) { AcmClose( acm ); *pnChannels = *pFreq = *pnSamples = 0; return 0; }
    acm->ReadCnt = 0;
    if( gAcmOpenedCnt == 1 ){
        gAcmPcmAlloc = malloc( 0x10000 * sizeof(int) );
	gAcmPcmTable = &gAcmPcmAlloc[ 0x8000 ];
    }
    *pnChannels = 1;//acm->nChannels; // commented because of dialogs
    *pFreq = 22050;//acm->Frequency; // commented because of dialogs
    *pnSamples = acm->ReadAvail;
#ifdef DEBUG
    eprintf( "Decode ACM file: samples=%i, freq=%i, channels=%i\n", acm->ReadAvail, acm->Frequency, acm->nChannels );
#endif
    return acm;
}

