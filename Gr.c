#include "FrameWork.h"
// ** Graphic library **

#define GRAY_SCALE        240.0 * 0.0019607844

char gGrGrayMap[ 256 ];
int *gGrUnk03;
unsigned int gGrUnk06;
int dword_596E98;
int *gGrUnk04;
int *gGrUnk05;
char *gGrUnk01;
int gGrUnk07;
int gGrUnk08;



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

//void GrUnk08( unsigned int a1, int a2, int a3, int a4, int a5, int a6, char *a7, int a8, unsigned int a9, int a10, char a11 ) // nu ?
//{
/*
    int v11; // ecx
    char *v12; // ebp
    char *v13; // edx
    char *v14; // edx
    unsigned int v15; // eax
    char *v16; // ebx
    unsigned int v17; // ecx
    unsigned __int8 i; // al
    unsigned __int8 v19; // al
    unsigned int v21; // [esp+0h] [ebp-28h]
    unsigned int v22; // [esp+8h] [ebp-20h]
    unsigned int v23; // [esp+Ch] [ebp-1Ch]
    int v24; // [esp+10h] [ebp-18h]
    unsigned __int8 v25; // [esp+14h] [ebp-14h]
    unsigned __int8 v26; // [esp+18h] [ebp-10h]

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
    while ( --v24 != -1 )
    {
        v17 = v23;
        for ( i = v26;
              v17;
              --v17 )
        {
            if ( (v25 & i) != 0 )
                *v16 = a11;
            i >>= 1;
            if ( !i )
            {
                v19 = *v14++;
                v25 = v19;
                i = 0x80;
            }
            ++v16;
        }
        v12 += a9 >> 3;
        v16 += v22;
        LOBYTE(v15) = v12[v21];
        v14 = &v12[v21 + 1];
        v25 = v15;
    }
*/
//}

//void GrUnk07(char *a1, unsigned int a2, int a3, int a4,unsigned char *a5, int a6, int a7 ) // nu ?
//{
/*
    unsigned char *result; // eax
    unsigned int j; // edx
    int v11; // ecx
    int v12; // ebx
    unsigned int i; // [esp+4h] [ebp-Ch]
    int v15; // [esp+18h] [ebp+8h]

    result = a5;
    v15 = a6 - a2;
    for ( i = a4 - a2;
          --a3 != -1;
          a1 += i )
    {
        for ( j = 0;
              j < a2;
              *(result - 1) = *(_BYTE *)(v11 + v12) )
        {
            v11 = ((*a1 & 0xF) << 8) + a7;
            v12 = *result;
            ++a1;
            ++result;
            ++j;
        }
        result += v15;
    }
*/
//}

//void GrUnk06( char *result, unsigned int a2, int a3, int a4, char *a5, int a6, char a7 ) // nu ?
//{
/*
    unsigned int i;

    while ( --a3 != -1 )
    {
        for ( i = 0;
              i < a2;
              ++a5 )
        {
            if ( *result )
                *a5 = a7;
            ++result;
            ++i;
        }
        a5 += a6 - a2;
        result += a4 - a2;
    }
*/
//}

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

int GrCompress( unsigned char *a1, char *a2, int a3 )
{
    int v3; // esi
    int v4; // edi
    unsigned int v5; // eax
    unsigned int v6; // ebx
    char *v7; // edx
    int v8; // edx
    int v9; // edx
    int v10; // ecx
    unsigned int v11; // eax
    char v12; // dl
    int v13; // ebp
    int v14; // edx
    char *v15; // ebp
    unsigned int v16; // edx
    int v17; // ebp
    unsigned int v18; // ecx
    char *v19; // ebp
    char v20; // al
    unsigned int v21; // ebp
    short v22; // cx
    unsigned int v23; // eax
    unsigned int v24; // ebp
    int v25; // esi
    char *v26; // ecx
    int v27; // ebx
    char v30[32]; // [esp+2h] [ebp-60h]
    int v31; // [esp+22h] [ebp-40h]
    unsigned char *v32; // [esp+26h] [ebp-3Ch]
    char *v33; // [esp+2Ah] [ebp-38h]
    int v34; // [esp+2Eh] [ebp-34h]
    int v35; // [esp+32h] [ebp-30h]
    unsigned int v36; // [esp+36h] [ebp-2Ch]
    unsigned int v37; // [esp+3Ah] [ebp-28h]
    unsigned int v38; // [esp+3Eh] [ebp-24h]
    int v39; // [esp+42h] [ebp-20h]
    char *v40; // [esp+46h] [ebp-1Ch]
    unsigned char *v41; // [esp+4Ah] [ebp-18h]
    char v42; // [esp+4Eh] [ebp-14h]

    v41 = a1;
    v40 = a2;
    v39 = a3;
    v3 = 4078;
    v4 = 0;
    v35 = 0;
    v31 = 0;
    v37 = 1;
    v42 = 1;
    gGrUnk03 = gGrUnk04 = gGrUnk05 = gGrUnk01 = NULL;    
    if( ( gGrUnk05 = Malloc( 16416 ) ) ){
        if( ( gGrUnk04 = Malloc( 17504 ) ) ){            
            if( ( gGrUnk03 = Malloc( 16416 ) ) ){                
                if( ( gGrUnk01 = Malloc( 4122 ) ) ) v35 = 1;
            }
        }
    }
    if( !v35 ){
        eprintf("\nGRAPHLIB: Error allocating compression buffers!\n");
        if( gGrUnk05 ) Free( gGrUnk05 );
        if( gGrUnk04 ) Free( gGrUnk04 );
        if( gGrUnk03 ) Free( gGrUnk03 );
        if( gGrUnk01 ) Free( gGrUnk01 );
        return -1;
    }
    GrUnk03();
    v30[0] = 0;
    memset( gGrUnk01, 32, 4078 );
    v5 = 4078;
    v6 = 0;
    do{
        v32 = v41;
        v33 = &gGrUnk01[ v5 ];
        gGrUnk01[ v5 ] = *v41++;
        if( ++v31 >= v39 ) break;
        ++v6;
    }while ( ++v5 < 4096 );
    

    v9 = 4077;
    dword_596E98 = v6;
    do
        GrUnk02( v9 );
    while ( v9 != 4059 );
    GrUnk02( 4078 );
    while( 1 ){
        if( v6 < gGrUnk06 ) gGrUnk06 = v6;
        v11 = v37 + 1;
        if( (unsigned int)gGrUnk06 > 2 ){
            v30[v37] = gGrUnk08;
            v33 = (char *)((gGrUnk06 - 3) | ((unsigned int)gGrUnk08 >> 4) & 0xF0);
            v37 = v11 + 1;
            v30[v11] = (gGrUnk06 - 3) | ((unsigned int)gGrUnk08 >> 4) & 0xF0;
        } else {
            gGrUnk06 = 1;
            v30[0] |= v42;
            v12 = gGrUnk01[v3];
            v13 = v37++;
            v30[v13] = v12;
        }
        v42 *= 2;
        if ( v42 ) goto LABEL_33;
        v14 = 0;
        if( v37 ) break;
LABEL_32:
        gGrUnk07 += v37;
        v30[0] = 0;
        v37 = 1;
        v42 = 1;
LABEL_33:
        v16 = 0;
        v38 = gGrUnk06;
        if( gGrUnk06 ){
            do{
                v34 = *v41;
                v17 = v31;
                ++v41;
                ++v31;
                if( v17 >= v39 ) break;
                GrUnk01(v10);
                v19 = &gGrUnk01[v18];
                v20 = v34;
                gGrUnk01[v18] = v34;
                if( v18 < 0x11 ) v19[4096] = v20;
                v21 = v38;
                v3 = ((short)v3 + 1) & 0xFFF;
                GrUnk02(v3);
            } while ( v16 < v21 );
        }
        while ( v16 < v38 ){
            GrUnk01(v10);
            v3 = ((short)v3 + 1) & 0xFFF;
            v10 = (v22 + 1) & 0xFFF;
            if( --v6 ) GrUnk02(v3);
        }
        if( !v6 ){
            if( v37 > 1 ){
                v23 = 0;
                v24 = v37;
                v25 = v39;
                do{
                    ++v4;
                    v26 = v40 + 1;
                    *v40 = v30[v23];
                    v40 = v26;
                    if( v4 > v25 ){
                        v35 = -1;
                        goto LABEL_49;
                    }
                    ++v23;
                }while ( v23 < v24 );
                gGrUnk07 += v37;
            }
            goto LABEL_49;
        }
    }
    while( 1 ){
        v15 = v40;
        ++v4;
        *v40 = v30[v14];
        v40 = v15 + 1;
        if( v4 > v39 ) break;
        if( ++v14 >= v37 ) goto LABEL_32;
    }
    v35 = -1;
LABEL_49:
    Free( gGrUnk05 );
    Free( gGrUnk04 );
    Free( gGrUnk03 );
    v27 = v35;
    Free(gGrUnk01);
    if( v27 == -1 ) return -1;
    return v4;
}

void GrUnk03()
{
    int *v0; // ebx
    int *v1; // ecx
    int v2; // eax
    int *v3; // edx
    int i; // eax

    v0 = gGrUnk04;
    v1 = gGrUnk03;
    v2 = 4097;
    v3 = gGrUnk04;
    do
        v3[ v2++ ] = 4096;
    while( v2 != 4353 );
    for( i = 0; i != 4096; i++ ) v1[ i ] = 4096;
    gGrUnk03 = v1;
    gGrUnk04 = v0;
}

int GrUnk02( int a1 )
{
    unsigned char *v1; // edi
    unsigned char *v2; // ebp
    int *v3; // edx
    int v4; // eax
    int v5; // ecx
    int v6; // eax
    int *v7; // eax
    int *result; // eax
    int *v9; // eax
    unsigned int v10; // eax
    unsigned char *v11; // ebx
    int v12; // edx
    int *v13; // eax
    int *v14; // ebx
    int *v15; // ecx
    int v16; // eax
    int *v17; // ecx
    int *v18; // [esp+0h] [ebp-2Ch]
    int *v19; // [esp+4h] [ebp-28h]
    int v20; // [esp+8h] [ebp-24h]
    int v22; // [esp+10h] [ebp-1Ch]

    v1 = (unsigned char *)gGrUnk01;
    v2 = (unsigned char *)&gGrUnk01[a1];
    v22 = (unsigned char)gGrUnk01[a1] + 4097;
    v3 = gGrUnk05;
    v4 = a1;
    gGrUnk05[v4] = 4096;
    v5 = 1;
    gGrUnk04[v4] = v3[v4];
    v20 = v4 * 4;
    gGrUnk06 = 0;
    while ( 1 )
    {
        v6 = v22;
        if ( v5 < 0 )
        {
            v9 = &gGrUnk05[v6];
            if ( *v9 == 4096 )
            {
                *v9 = a1;
                result = (int *)((char *)gGrUnk03 + v20);
                *(int *)((char *)gGrUnk03 + v20) = v22;
                gGrUnk01 = (char *)v1;
                return result;
            }
            v22 = *v9;
        }
        else
        {
            v7 = &gGrUnk04[v6];
            if ( *v7 == 4096 )
            {
                *v7 = a1;
                result = (int *)((char *)gGrUnk03 + v20);
                *(int *)((char *)gGrUnk03 + v20) = v22;
                goto LABEL_18;
            }
            v22 = *v7;
        }
        v10 = 1;
        v11 = v2 + 1;
        v12 = v22 + 1;
        do
        {
            v5 = *v11 - v1[v12];
            if ( v5 )
                break;
            ++v12;
            ++v10;
            ++v11;
        }
        while ( (int)v10 < 18 );
        if ( v10 > gGrUnk06 )
        {
            gGrUnk06 = v10;
            gGrUnk08 = v22;
            if ( v10 >= 18 )
                break;
        }
    }
    v13 = gGrUnk03;
    v18 = &gGrUnk03[v22];
    gGrUnk03[a1] = *v18;
    v14 = gGrUnk05;
    v19 = &gGrUnk05[v22];
    gGrUnk05[a1] = *v19;
    v15 = gGrUnk04;
    gGrUnk04[a1] = gGrUnk04[v22];
    v13[*v19] = a1;
    v13[v15[v22]] = a1;
    v16 = *v18;
    v17 = &v15[v16];
    if ( v22 == *v17 )
        *v17 = a1;
    else
        v14[v16] = a1;
    result = (int *)v22;
    gGrUnk03[v22] = 4096;
LABEL_18:
    gGrUnk01 = (char *)v1;
    return result;
}

int GrUnk01( int a1 )
{
    int *v1; // esi
    int idx; // eax
    int *v4; // ebx
    int v5; // eax
    int *p; // eax
    int v7; // ebp
    int *v8; // ecx
    int v9; // edx
    int v10; // edx
    int *v11; // ebx
    int v12; // ecx
    int *v13; // edx
    int *v14; // [esp+0h] [ebp-20h]
    int *v15; // [esp+4h] [ebp-1Ch]

    v1 = gGrUnk04;
    idx = 4 * a1;
    if ( *(int *)((char *)gGrUnk03 + idx) != 4096 )
    {
        v4 = (int *)((char *)gGrUnk04 + idx);
        if ( *(int *)((char *)gGrUnk04 + idx) == 4096 )
        {
            v5 = *(int *)((char *)gGrUnk05 + idx);
        }
        else
        {
            p = (int *)((char *)gGrUnk05 + idx);
            v7 = *p;
            if ( *p == 4096 )
            {
                v5 = *v4;
            }
            else
            {
                v5 = *p;
                if ( gGrUnk04[v7] != 4096 )
                {
                    do
                        v5 = gGrUnk04[v5];
                    while ( gGrUnk04[v5] != 4096 );
                    v14 = gGrUnk05;
                    v15 = &gGrUnk05[v5];
                    v8 = gGrUnk03;
                    gGrUnk04[gGrUnk03[v5]] = *v15;
                    v8[*v15] = v8[v5];
                    v9 = v14[a1];
                    *v15 = v9;
                    v8[v9] = v5;
                }
                v10 = v1[a1];
                v11 = gGrUnk03;
                v1[v5] = v10;
                v11[v10] = v5;
            }
        }
        v12 = gGrUnk03[a1];
        gGrUnk03[v5] = v12;
        v13 = &v1[v12];
        if ( a1 == *v13 )
            *v13 = v5;
        else
            gGrUnk05[v12] = v5;
        idx = (int)gGrUnk03;
        gGrUnk03[a1] = 4096;
    }
    gGrUnk04 = v1;
    return idx;
}

int GrDecompress( unsigned char *InBuff, char *OutBuff, int MaxSize )
{
    int Size, FrmIdx;
    unsigned short mask,len;
    short i, patt;

    FrmIdx = 0xfee;
    Size = 0;
    mask = 0;
    if( !( gGrUnk01 = Malloc( 0x101a ) ) ){ eprintf( "\nGRAPHLIB: Error allocating decompression buffer!\n" ); return -1; }
    memset( gGrUnk01, 0x20, 0xfee );
    do{
        mask >>= 1;
        if( (mask & 0x100) == 0 ){
            mask = ( unsigned short)*InBuff++ | 0xff00;
        }
        if( mask & 0x01 ){
    	    gGrUnk01[ FrmIdx++ ] = *InBuff;
    	    *OutBuff++ = *InBuff++;
    	    FrmIdx &= 0xFFF;
    	} else {
	    patt = InBuff[ 0 ] | ((InBuff[ 1 ] & 0xF0) << 4);
	    len = (InBuff[ 1 ] & 0x0F) + 2;    
	    InBuff += 2;
	    for( i = 0; i <= len; i++ ){
    		*OutBuff = gGrUnk01[ FrmIdx++ ] = gGrUnk01[ (patt + i) & 0xFFF ];
    		FrmIdx &= 0xFFF;
    		OutBuff++;
    		if( ++Size >= MaxSize ){ Free( gGrUnk01 ); return 0; }
	    }
        }
    }while( Size++ < MaxSize );
    Free( gGrUnk01 );
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


