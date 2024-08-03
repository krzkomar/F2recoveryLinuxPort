#include "FrameWork.h"
// ** Graphic library **

#define GRAY_SCALE        240.0 * 0.0019607844

char 	gGrGrayMap[ 256 ];
int 	*gGrBuff3;
unsigned int gGrUnk06;
int 	gGrUnk02;
int 	*gGrBuff2;
int 	*gGrBuff1;
char	*gGrBuff4;
int 	gGrUnk08;


unsigned int 	gGrMatchLength;
unsigned int 	*gGrLZBranchL;
unsigned int 	*gGrLZBranchR;
unsigned int 	*gGrLZNode;
unsigned char	*gGrBuffData;
unsigned int 	gGrMatchOffset;

static void GrLZTreeSort( int idx );
static void GrLZLookForMatch( int Idx );
static void GrLZDictionaryInit();

#define LZROOT	0x1001
#define LZNULL	0x1000

#define d_out( dat )	\
    { *OutBuffer++ = dat;	\
    if( ++OutSize > InSize ){ err = -1; goto Error; } }


unsigned char GrMaxRGB( unsigned char Color8 )
{
    unsigned short col16;
    unsigned int r, b, g;

    col16 = PalConvColor8to16( Color8 );
    r = col16 & 0x001f;
    b = (col16 & 0x7C00) >> 10;
    g = (col16 & 0x03E0) >> 5;
    if( b > g ) g = b;
    if( (g & 0xff) <= r ) g = r;
    return g;
}

/*
void GrUnk08( unsigned int a1, int a2, int a3, int a4, int a5, int a6, char *a7, int a8, unsigned int a9, int a10, char a11 ) // nu ?
{
    unsigned char i,v19,v25,v26;
    unsigned int v15, v17, v21, v22, v23;
    int v11,v24;
    char *v12,*v13,*v14,*v16;

    v11 = a4 - a2;
    v12 = &a7[(a9 >> 3) * a2];
    v21 = a1 >> 3;
    v13 = &v12[a1 >> 3];
    v25 = *v13;
    v24 = v11 + 1;
    v14 = v13 + 1;
    v26 = 128 >> (a1 & 7);
    v15 = a3 - a1 + 1;
    v23 = v15;
    v22 = a10 - v15;
    v16 = (char *)(a8 + a5 + a10 * a6);
    while( --v24 != -1 ){        
        i = v26;
        for( v17 = v23; v17; v17--, v16++ ){
            if( (v25 & i) != 0 ) *v16 = a11;
            i >>= 1;
            if( !i ){
                v19 = *v14++;
                v25 = v19;
                i = 0x80;
            }            
        }
        v12 += a9 >> 3;
        v16 += v22;
        v15 = v12[ v21 ] & 0xff;
        v14 = &v12[v21 + 1];
        v25 = v15;
    }
}

void GrUnk07(char *Src, unsigned int Width, int Height, int SrcPitch, unsigned char *img, int DstPitch, char *Shader ) // nu ?
{
    unsigned int j, i, diff;

    diff = DstPitch - Width;
    for( i = SrcPitch - Width; Height != -1; Height--, Src += i ) {
        for( j = 0; j < Width; Src++, img++, j++ ){
            *img = Shader[ (*Src & 0xf ) * 256 + *img ];
        }
        img += diff;
    }
}

void GrUnk06( char *MaskData, unsigned int Width, int Height, int MaskPitch, char *ImgData, int DstPitch, char FillColor ) // nu ?
{
    unsigned int i;

    while( --Height != -1 ){
        for( i = 0; i < Width; ImgData++, MaskData++, i++ ){
            if( *MaskData ) *ImgData = FillColor;
        }
        ImgData += DstPitch - Width;
        MaskData += MaskPitch - Width;
    }
}
*/
//int GrUnk05( const char *Fname, char *a2, int a3, int a4, int a5, int a6 ) // nu
//{
/*
    int v7; // ebx
    struct xFile_t *fh; // edi MAPDST
    int err; // ebp
    int i; // ebp MAPDST
    int v11; // eax
    int v12; // edx
    int v14; // eax
    int j; // ebp
    int v16; // eax
    int v17; // edx
    int v18; // eax
    int k; // ebp
    int v21; // eax
    int v22; // edx
    int m; // ebp
    int v24; // eax
    int v25; // eax
    int n; // ebp
    int v27; // eax
    int v28; // edx
    int v29; // eax
    int v31; // eax
    int v32; // ecx
    int v33; // eax
    int v34; // ecx
    int v35; // eax
    int ii; // ebp
    char v37[257]; // [esp+1h] [ebp-131h]
    int tmp[4]; // [esp+102h] [ebp-30h]
    int v39; // [esp+112h] [ebp-20h]
    int cnt; // [esp+116h] [ebp-1Ch]
    int v42; // [esp+11Eh] [ebp-14h]
    char v43; // [esp+122h] [ebp-10h]
    int v44; // [esp+13Eh] [ebp+Ch]

    v7 = 0;
    fh = dbOpen(Fname, "rb");
    if( !fh ) return -1;
    for( i = 0; i < 16; i += 4 ){
        v11 = dbgetc(fh);
        *(int *)((char *)tmp + i) = v11;
        if( v11 == -1 ) break;
        v12++;
    }
    if( v12 >= 4 ){
        do{
            tmp[0] = tmp[1];
            tmp[1] = tmp[2];
            tmp[2] = tmp[3];
            v14 = dbgetc(fh);
            tmp[3] = v14;
        }while( v14 != -1 && (tmp[0] != 'B' || tmp[1] != 'M' || tmp[2] != 'H' || v14 != 'D') );
        if( tmp[3] == -1 )
            goto LABEL_26;
        for( j = 0; j < 4; j++ ){
            v16 = dbgetc(fh);
            tmp[j] = v16;
            if( v16 == -1 ) break;
            v17++;
        }
        if( v17 < 4 ) goto LABEL_26;
        tmp[0] = dbgetc(fh);
        tmp[1] = dbgetc(fh);
        v42 = tmp[1] + (tmp[0] << 8);
        do{
            tmp[0] = tmp[1];
            tmp[1] = tmp[2];
            tmp[2] = tmp[3];
            v18 = dbgetc(fh);
            tmp[3] = v18;
        } while ( v18 != -1 && (tmp[0] != 'C' || tmp[1] != 'M' || tmp[2] != 'A' || v18 != 'P') );
        if( tmp[3] == -1 ) goto LABEL_26;
        for( k = 0; k < 4; k++ ){
            v21 = dbgetc( fh );
            tmp[k] = v21;
            if( v21 == -1 ) break;
            v22++;
        }
        if( v22 < 4 ){
LABEL_26:
            dbClose(fh);
            return -1;
        }
        for( m = 0; m < 256; v37[m] = gPalColorCubeRGB[0][0][v39 | (tmp[0] >> 3 << 10) | (v24 >> 3)] ){
            tmp[0] = dbgetc(fh);
            if ( tmp[0] == -1 )
                break;
            tmp[1] = dbgetc(fh);
            if ( tmp[1] == -1 )
                break;
            v24 = dbgetc(fh);
            tmp[2] = v24;
            if ( v24 == -1 )
                break;
            v39 = 32 * (tmp[1] >> 3);
            ++m;
        }
        if( m < 256 ) goto LABEL_44;
        do{
            tmp[0] = tmp[1];
            tmp[1] = tmp[2];
            tmp[2] = tmp[3];
            v25 = dbgetc(fh);
            tmp[3] = v25;
        } while ( v25 != -1 && (tmp[0] != 'B' || tmp[1] != 'O' || tmp[2] != 'D' || v25 != 'Y') );
        if( tmp[3] == -1 ) goto LABEL_44;
        for( n = 0; n < 4; n++ ){
            v27 = dbgetc(fh);
            tmp[n] = v27;
            if( v27 == -1 ) break;
            v28++;
        }
        if( v28 < 4 ){
LABEL_44:
            dbClose(fh);
            return -1;
        }
        while( 1 ){
            v29 = dbgetc(fh);
            cnt = v29;
            if ( v29 == -1 )
                break;
            if( v29 >= 128 ){
                v43 = 0;
                v33 = dbgetc(fh);
                tmp[0] = v33;
                if( v33 == -1 ) break;
                if( v33 ) v43 = v37[ v33 + 1 ];
                v35 = 256 - cnt;
                for( ii = 0; ii <= v35; ii++ ){
                    if( v7 >= a4 && v7 <= a6 && v34 >= a5 && v34 <= v44 ) *a2++ = v43;
                    if( ++v7 == v42 ){ v34++; v7 = 0; }
                }
                if( ii <= v35 ) break;
            } else {
                i = 0;
                if( v29 >= 0 ){
                    do{
                        v31 = dbgetc(fh);
                        tmp[0] = v31;
                        if( v31 == -1 ) break;
                        if( v7 >= a4 && v7 <= a6 && v32 >= a5 && v32 <= v44 ){
                            *a2 = ( v31 ) ? v37[v31 + 1] : 0;
                            a2++;
                        }
                        if( ++v7 == v42 ) v7 = 0;
                        i++;
                    } while ( i <= cnt );
                }
                if( i <= cnt ) break;
            }
        }
        err = v42;
    } else {
        err = -1;
    }
    dbClose(fh);
    return err;
*/
//return -1;
//}

int GrLZDeflate( unsigned char *InBuffer, char *OutBuffer, int InSize )
{
    unsigned int DataSrcIdx;
    char BitMask, Seq[ 32 ];
    int BlkStop, OutSize, dat, err, i, DataInCnt, tail, nSeq,len;

    gGrLZNode = NULL;
    gGrLZBranchR = NULL;
    gGrLZBranchL = NULL;
    gGrBuffData = NULL;    
    if( (gGrLZBranchL = Malloc( (4096 + 8) * 4 )) ){        
        if( (gGrLZBranchR = Malloc( (4096 + 280) * 4 )) ){            
            if( (gGrLZNode = Malloc( (4096 + 8) * 4 )) ){
                if( (gGrBuffData = Malloc( 4096 + 18 + 8 ) ) ) err = 1;
            }
        }
    }
    if( !err ){
        eprintf( "\nGRAPHLIB: Error allocating compression buffers!\n" );
        if( gGrLZBranchL ) Free( gGrLZBranchL );
        if( gGrLZBranchR ) Free( gGrLZBranchR );
        if( gGrLZNode ) Free( gGrLZNode );
        if( gGrBuffData ) Free( gGrBuffData );
        return -1;
    }

    GrLZDictionaryInit();
    memset( gGrBuffData, 32, 4078 );    
    Seq[ 0 ] = 0;    
    BlkStop = 4096 - 18;
    for( i = 0; ( i < 18 ) && ( i < InSize ); i++ ) gGrBuffData[ BlkStop + i ] = *InBuffer++;
    for( i = 0; i < 18; i++ ) GrLZLookForMatch( 4077 - i ); // !! diff
    GrLZLookForMatch( BlkStop );
    err = 0;
    DataSrcIdx = 0;
    OutSize = 0;
    nSeq = 1;
    DataInCnt = 18;
    tail = 18;
    BitMask = 1;
    do{
        if( gGrMatchLength > tail ) gGrMatchLength = tail;
        if( gGrMatchLength > 2 ){
    	    // copy of sequence length
            Seq[ nSeq++ ] = gGrMatchOffset; //!! diff
            Seq[ nSeq++ ] = ( (gGrMatchOffset >> 4) & 0xf0 ) | (gGrMatchLength - 3);
        } else {
    	    // indirect copying
            gGrMatchLength = 1;
            Seq[ 0     ] |= BitMask;
            Seq[ nSeq++ ] = gGrBuffData[ BlkStop ];
        }
        if( (BitMask <<= 1) == 0 ){    	    
	    for( i = 0; i < nSeq; i++ ) d_out( Seq[ i ] );
    	    Seq[ 0 ] = 0; // map reset
    	    nSeq = 1;	 // buffer counter reset
    	    BitMask = 1; // mask reset
        }
        len = gGrMatchLength;
        for( i = 0; i < len; i++ ){
            dat = *InBuffer++;
            if( ++DataInCnt >= InSize ) break;
            GrLZTreeSort( DataSrcIdx );
            gGrBuffData[ DataSrcIdx ] = dat;
            if( DataSrcIdx < 17 ) gGrBuffData[ DataSrcIdx + 4096 ] = dat;
            DataSrcIdx = (DataSrcIdx + 1) & 0xFFF;
            BlkStop = (BlkStop + 1) & 0xFFF;
            GrLZLookForMatch( BlkStop );
        }
        for( ;i < len; i++ ){ // finish, when indata buffer exhausted
            GrLZTreeSort( DataSrcIdx );
            DataSrcIdx = (DataSrcIdx + 1) & 0xFFF;
            BlkStop = (BlkStop + 1) & 0xFFF;
            if( --tail ) GrLZLookForMatch( BlkStop );
        }
    } while( tail > 0 );
    if( nSeq > 1 ){ // flush out rest of the buffer
	for( i = 0; i < nSeq; i++ ) d_out( Seq[ i ] );
    }
Error:
    Free( gGrLZBranchL );
    Free( gGrLZBranchR );
    Free( gGrLZNode );
    Free( gGrBuffData );
    if( err == -1 ) return -1;
    return OutSize;
}

void GrLZDictionaryInit()
{
    int i;

    for( i = 4097; i < 4353; i++ ) gGrLZBranchR[ i ] = LZNULL;    
    for( i = 0; i < 4096; i++ ) gGrLZNode[ i ] = LZNULL;
}

void GrLZLookForMatch( int StartIdx )
{
    unsigned int length, DictPos, tmp;    
    int Match;

    Match = 1;
    DictPos = LZROOT + gGrBuffData[ StartIdx ];
    gGrLZBranchL[ StartIdx ] = LZNULL;
    gGrLZBranchR[ StartIdx ] = LZNULL;
    gGrMatchLength = 0;
    do{
	do{
    	    if( Match < 0 ){
    		if( gGrLZBranchL[ DictPos ] == LZNULL ){
            	    gGrLZBranchL[ DictPos ] = StartIdx;
            	    gGrLZNode[ StartIdx ] = DictPos;
            	    return;
        	}
        	DictPos = gGrLZBranchL[ DictPos ];
    	    } else {
        	if( gGrLZBranchR[ DictPos ] == LZNULL ){
            	    gGrLZBranchR[ DictPos ] = StartIdx;
            	    gGrLZNode[ StartIdx ] = DictPos;
		    return;
        	}
        	DictPos = gGrLZBranchR[ DictPos ];
    	    }        
    	    for( length = 1; length < 18; length++ ){
        	Match = (unsigned int)gGrBuffData[ StartIdx + length ] - (unsigned int)gGrBuffData[ DictPos + length ];
        	if( Match != 0 ) break;
    	    }        
        }while( length <= gGrMatchLength );
        gGrMatchOffset = DictPos;
        gGrMatchLength = length;
    }while( length < 18 );

    gGrLZNode[ StartIdx ] = gGrLZNode[ DictPos ];
    gGrLZBranchL[ StartIdx ] = gGrLZBranchL[ DictPos ];
    gGrLZBranchR[ StartIdx ] = gGrLZBranchR[ DictPos ];
    gGrLZNode[ gGrLZBranchL[ DictPos ] ] = StartIdx;
    gGrLZNode[ gGrLZBranchR[ DictPos ] ] = StartIdx;
    
    tmp = gGrLZNode[ DictPos ];
    if( gGrLZBranchR[ tmp ] == DictPos )
        gGrLZBranchR[ tmp ] = StartIdx;
    else
        gGrLZBranchL[ tmp ] = StartIdx;
    gGrLZNode[ DictPos ] = LZNULL;
}

void GrLZTreeSort( int idx )
{
    unsigned int k, m;

    if( gGrLZNode[ idx ] == LZNULL ) return;
    if( gGrLZBranchR[ idx ] != LZNULL ){
        if( gGrLZBranchL[ idx ] != LZNULL ){
            k = gGrLZBranchL[ idx ];
            if( gGrLZBranchR[ k ] != LZNULL ){
                while( gGrLZBranchR[ k = gGrLZBranchR[ k ] ] != LZNULL );
                gGrLZBranchR[ gGrLZNode[ k ] ] = gGrLZBranchL[ k ];
                gGrLZNode[ gGrLZBranchL[ k ] ] = gGrLZNode[ k ];
                gGrLZBranchL[ k ] = gGrLZBranchL[ idx ];
                gGrLZNode[ gGrLZBranchL[ idx ] ] = k;
            }
            gGrLZBranchR[ k ] = gGrLZBranchR[ idx ];
            gGrLZNode[ gGrLZBranchR[ idx ] ] = k;
        } else
            k = gGrLZBranchR[ idx ];
    } else
        k = gGrLZBranchL[ idx ];
    gGrLZNode[ k ] = gGrLZNode[ idx ];
    m = gGrLZNode[ idx ];
    if( gGrLZBranchR[ m ] == idx )
        gGrLZBranchR[ m ] = k;
    else
        gGrLZBranchL[ m ] = k;
    gGrLZNode[ idx ] = LZNULL;
}

int GrLZInflate( unsigned char *InBuff, char *OutBuff, int MaxSize )
{
    int Size, FrmIdx;
    unsigned short mask,len;
    short i, patt;

    FrmIdx = 0xfee;
    Size = 0;
    mask = 0;
    if( !( gGrBuff4 = Malloc( 0x101a ) ) ){ eprintf( "\nGRAPHLIB: Error allocating decompression buffer!\n" ); return -1; }
    memset( gGrBuff4, 0x20, 0xfee );
    do{
        mask >>= 1;
        if( (mask & 0x100) == 0 ){
            mask = ( unsigned short)*InBuff++ | 0xff00;
        }
        if( mask & 0x01 ){
    	    gGrBuff4[ FrmIdx++ ] = *InBuff;
    	    *OutBuff++ = *InBuff++;
    	    FrmIdx &= 0xFFF;
    	} else {
	    patt = InBuff[ 0 ] | ((InBuff[ 1 ] & 0xF0) << 4);
	    len = (InBuff[ 1 ] & 0x0F) + 2;    
	    InBuff += 2;
	    for( i = 0; i <= len; i++ ){
    		*OutBuff = gGrBuff4[ FrmIdx++ ] = gGrBuff4[ (patt + i) & 0xFFF ];
    		FrmIdx &= 0xFFF;
    		OutBuff++;
    		if( ++Size >= MaxSize ){ Free( gGrBuff4 ); return 0; }
	    }
        }
    }while( Size++ < MaxSize );
    Free( gGrBuff4 );
    return 0;
}

void GrMakeGrayMap( int FirstColor, int LastColor ) // create gray scale
{
    unsigned int i, gs, tmp, b, g, r;

    if( FirstColor < 0 || LastColor > 255 || FirstColor > LastColor ) return;
    
    for( i = FirstColor; i <= LastColor; i++ ){
	tmp = PalConvColor8to16( i );
	r = RGB16_GET_R( tmp );
	g = RGB16_GET_G( tmp );
	b = RGB16_GET_B( tmp );

	// select max value from RGB
        tmp = ( g > b ) ? g : b;
        if( r > tmp ) tmp = r;
	gs = tmp;

	// select min value from RGB
        tmp = ( g < b ) ? g : b;
        if( r < tmp ) tmp = r;
        gs += tmp;

        tmp = (unsigned char)lround( gs * GRAY_SCALE );
        gGrGrayMap[ i ] = gPalColorCubeRGB[ tmp ][ tmp ][ tmp ];
    }
}

// turns image to gray scale
void GrGrayMapApply( unsigned char *img, int w, int h, int pitch )
{
    int nl, i, j;

    nl = pitch - w;
    for( j = 0; j < h; j++ ){
        for( i = 0; i < w; i++, img++ ){
            *img = gGrGrayMap[ *img ];
        }
        img += nl;
    }
}



