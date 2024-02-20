#include "FrameWork.h"

typedef struct 
{
  int i01; // mapid ?
  int i02; // lvl ?
  int i03; // grid pos ?
} Elevator_t;

// 24 elevators
char *gElevSndFile[ 12 ] = { "ELV1_1", "ELV1_1", "ERROR", "ERROR", "ELV1_2", "ELV1_2", "ELV1_1", "ERROR", "ELV1_3", "ELV1_3", "ELV2_3", "ELV1_1" };

int gElevButtImgId[ 3 ] = { 141, 142, 149 };
int gElevBgImgId[ 2*24 ] = { 
    143, -1,  143, 150, 144, -1,  144, 145, 146, -1,  146, 147, 
    146, -1,  146, 151, 148, -1,  146, -1,  146, -1,  146, 147, 
    388, -1,  143, 150, 148, -1,  148, -1,  148, -1,  143, 150, 
    143, 150, 143, 150, 143, 150, 143, 150, 143, 150, 143, 150
};
int gElevButtNum[ 24 ] = { 4, 2, 3, 2, 3, 2, 3, 3, 3, 3, 3, 2, 4, 2, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2 };
Elevator_t gElevLevels[ 24 ][ 4 ] = {
    { { 0x0E, 0, 0x49FC },{ 0x0E, 1, 0x49F8 },{ 0x0F, 0, 0x535C },{ 0x0F, 1, 0x535C } },
    { { 0x0D, 0, 0x5016 },{ 0x0E, 0, 0x3A40 },{ 0x00, 0, -1     },{ 0x00, 0, -1 } },
    { { 0x21, 0, 0x30D2 },{ 0x21, 1, 0x4E7E },{ 0x22, 0, 0x43A0 },{ 0x00, 0, -1 } },
    { { 0x22, 0, 0x3F0C },{ 0x22, 1, 0x3F0C },{ 0x00, 0, -1     },{ 0x00, 0, -1 } },
    { { 0x31, 0, 0x3A48 },{ 0x31, 1, 0x3B10 },{ 0x32, 0, 0x3290 },{ 0x00, 0, -1 } },
    { { 0x32, 0, 0x5FC8 },{ 0x32, 1, 0x62E4 },{ 0x00, 0, -1     },{ 0x00, 0, -1 } },
    { { 0x2A, 0, 0x57FE },{ 0x2A, 1, 0x57FE },{ 0x2A, 2, 0x57FE },{ 0x00, 0, -1 } },
    { { 0x2A, 2, 0x3706 },{ 0x2B, 0, 0x3706 },{ 0x2B, 2, 0x3706 },{ 0x00, 0, -1 } },
    { { 0x28, 0, 0x3718 },{ 0x28, 1, 0x57E8 },{ 0x28, 2, 0x43A0 },{ 0x00, 0, -1 } },
    { { 0x09, 0, 0x3588 },{ 0x09, 1, 0x5B06 },{ 0x09, 2, 0x439C },{ 0x00, 0, -1 } },
    { { 0x1C, 0, 0x4B64 },{ 0x1C, 1, 0x4B64 },{ 0x1C, 2, 0x4E8E },{ 0x00, 0, -1 } },
    { { 0x1C, 2, 0x4E96 },{ 0x1D, 0, 0x54CE },{ 0x00, 0, -1     },{ 0x00, 0, -1 } },
    { { 0x1C, 0, 0x4E9A },{ 0x1C, 1, 0x4E9C },{ 0x1C, 2, 0x51CC },{ 0x1D, 0, 0x580C } },
    { { 0x0C, 1, 0x3EB4 },{ 0x0C, 2, 0x3890 },{ 0x00, 0, -1     },{ 0x00, 0, -1 } },
    { { 0x06, 0, 0x3718 },{ 0x06, 1, 0x57E8 },{ 0x06, 2, 0x43A0 },{ 0x00, 0, -1 } },
    { { 0x1E, 0, 0x3718 },{ 0x1E, 1, 0x57E8 },{ 0x1E, 2, 0x43A0 },{ 0x00, 0, -1 } },
    { { 0x24, 0, 0x3588 },{ 0x24, 1, 0x5B06 },{ 0x24, 2, 0x439C },{ 0x00, 0, -1 } },
    { { 0x27, 0, 0x4385 },{ 0x24, 0, 0x4C10 },{ 0x00, 0, -1     },{ 0x00, 0, -1 } },
    { { 0x6D, 2, 0x29CD },{ 0x6D, 1, 0x29D1 },{ 0x00, 0, -1     },{ 0x00, 0, -1 } },
    { { 0x6D, 2, 0x3969 },{ 0x6D, 1, 0x3AFB },{ 0x00, 0, -1     },{ 0x00, 0, -1 } },
    { { 0x6D, 2, 0x5D45 },{ 0x6D, 1, 0x6389 },{ 0x00, 0, -1     },{ 0x00, 0, -1 } },
    { { 0x6D, 2, 0x6612 },{ 0x6D, 1, 0x6091 },{ 0x00, 0, -1     },{ 0x00, 0, -1 } },
    { { 0x89, 0, 0x5D91 },{ 0x94, 1, 0x408E },{ 0x00, 0, -1     },{ 0x00, 0, -1 } },
    { { 0x3E, 0, 0x364D },{ 0x3F, 1, 0x4603 },{ 0x00, 0, -1     },{ 0x00, 0, -1 } }
};

char gElevKeys[ 24 ][ 4 ] = {
    { '1', '2', '3', '4'}, { 'G', '1',  0,  0 }, { '1', '2', '3', 0 }, { '3', '4',  0,  0 }, 
    { '1', '2', '3', 0  }, { '3', '4',  0,  0 }, { '1', '2', '3', 0 }, { '3', '4', '6', 0 },
    { '1', '2', '3', 0  }, { '1', '2', '3', 0 }, { '1', '2', '3', 0 }, { '3', '4',  0,  0 },
    { '1', '2', '3','4' }, { '1', '2',  0,  0 }, { '1', '2', '3', 0 }, { '1', '2', '3', 0 },
    { '1', '2', '3', 0  }, { '1', '2',  0,  0 }, { '1', '2',  0,  0 }, { '1', '2',  0,  0 },
    { '1', '2',  0,  0  }, { '1', '2',  0,  0 }, { '1', '2',  0,  0 }, { '1', '2',  0,  0 }
};

PosXYi_t gElevButtSize[ 3 ];
//int gElevButtSize[2].x;
//int gElevButtSize[2].y;
int gElevBGImgW;
int gElevBGImgH;
int gElevImgW1;
int gElevImgH1;
int gElevWin;
CachePool_t *gElevButtImgObj[ 3 ];
CachePool_t *gElevBgImgObj;
CachePool_t *gElevImgObj1;
char *gElevSurface;
int gElevMap;
char *gElevButtImg[ 3 ];
char *gElevBgImg;
char *gElevImg1;

int ElevatorMenu( int Style, int *pMapId, int *pStartLvl, short *a4 )
{
    double v12,v18,v19;
    int v6,i,v21,v23,SysTime,sel,v45;
    float v41,v42,v43;

    if( Style >= 24 ) return -1;
    if( ElevatorMenuCreate( Style ) == -1 ) return -1;
    for( i = 0; i < 4 && ( gElevLevels[ Style ][ i ].i01 != *pMapId ); i++ );
    if( i < 4 ){ // found
        i += *pStartLvl;
        if( gElevLevels[ Style ][ i ].i03 != -1 ) *pStartLvl = i;
    }
    switch( Style ){
	case 11: if( *pStartLvl <= 2 ) *pStartLvl -= 2; else *pStartLvl -= 3; break;
	case 5: if( *pStartLvl >= 2 ) *pStartLvl -= 2; break;
	case 4: if( *pStartLvl == 4 ) *pStartLvl -= 2; break;
    }
    if( *pStartLvl > 3 ) *pStartLvl -= 3;
    eprintf( "\n the start elev level %d\n", *pStartLvl );

    v41 = 12.0 / (gElevButtNum[Style] - 1);
    v12 = *pStartLvl * v41;
    v43 = v12;
    v45 = lround( v12 );
    ScrCopy( (gElevButtImg[2] + v45 * gElevButtSize[2].y * gElevButtSize[2].x / 13), gElevButtSize[2].x, gElevButtSize[2].y / 13, gElevButtSize[2].x, (gElevSurface + 41 * *&gElevBGImgW + 121), gElevBGImgW );
    WinUpdate( gElevWin );
    v6 = 0;
    do{
        sel = InpUpdate();
        if( sel == KEY_ESC ) v6 = 1;
        if( sel >= 500 && sel < 504 ) v6 = 1;
        if( sel > 0 && sel < 500 ){
            if( (v6 = ElevatorKeySel( Style, sel )) ){
                sel = 499 + v6;
                v6 = 1;
            }
        }
    }while( !v6 );

    if( sel != KEY_ESC ){
        sel -= 500;
        if( *pStartLvl != sel ){
            v18 = sel * v41;
            v19 = (gElevButtNum[ Style ] - 1) * 0.083333336;
	    v42 = v19;
            v21 = lround( v19 * 276.92307 );
            if( sel < *pStartLvl ) v42 = -v42;
            v23 = sel - *pStartLvl;
            if( v23 < 0 ) v23 = *pStartLvl - sel;
            GSoundPlay( gElevSndFile[ 4 * gElevButtNum[ Style ] + v23 - 8 ] );
            do{ // lvl gauge animation
                SysTime = TimerGetSysTime();
                v43 += v42;
                ScrCopy( (gElevButtImg[ 2 ] + lround( v43 ) * gElevButtSize[2].y * gElevButtSize[2].x / 13), gElevButtSize[2].x, gElevButtSize[2].y / 13, gElevButtSize[2].x, (gElevSurface + 41 * gElevBGImgW + 121), gElevBGImgW );
                WinUpdate( gElevWin );
                while( TimerCurrDiff( SysTime ) < v21 );
            }while( (v42 <= 0.0 || v43 < v18) && (v42 >= 0.0 || v43 > v18) );
        }
        TimerWaitProc( 200 );
    }
    ElevatorMenuClose();
    if( sel != KEY_ESC ){
        *pMapId = gElevLevels[ Style ][ sel ].i01;
        *pStartLvl = gElevLevels[ Style ][ sel ].i02;
        *a4 = gElevLevels[ Style ][ sel ].i03;
    }
    return 0;
}

int ElevatorMenuCreate( int style )
{
    CachePool_t **pImgObj;
    PosXY_t *pSize;
    int i, win, bt, flg;

    gElevMap = MapAmbientDisable();
    CycleColorStop();
    flg = 1;
    GmouseLoadCursor( 1 );
    GmouseUnk03();
    GmouseLoadCursor( 1 );
    ScptDisable();
    for( i = 0; i < 3; i++, pSize += 2, pImgObj++ ){        
        if( !( gElevButtImg[ i ] = ArtLoadBmp( ArtMakeId( 6, gElevButtImgId[ i ], 0, 0, 0 ), &gElevButtImgObj[ i ], &gElevButtSize[ i ].x, &gElevButtSize[ i ].y ) ) ) break;
    }
    if( i < 3 ){
        for( ; !i; i-- ) ArtClose( gElevButtImgObj[ i ] );
        if( gElevMap ) MapAmbientEnable();
        CycleColorStart();
        GmouseLoadCursor(1);
        return -1;
    }
    gElevImg1 = ART_NULL;
    gElevBgImg = ART_NULL;
    gElevBgImg = ArtLoadBmp( ArtMakeId( 6, gElevBgImgId[ 2 * style ], 0, 0, 0 ), &gElevBgImgObj, &gElevBGImgW, &gElevBGImgH );
    if( gElevBgImg ){        
        if( (i = gElevBgImgId[ 2 * style + 1 ] ) != -1 ){
            gElevImg1 = ArtLoadBmp( ArtMakeId( 6, i, 0, 0, 0 ), &gElevImgObj1, &gElevImgW1, &gElevImgH1 );
            if( !gElevImg1 ){
                flg = 0;
                gElevImg1 = ART_NULL;
            }
        }
    } else {
        flg = 0;
        gElevBgImg = ART_NULL;
    }
    if( !flg ){
        if( gElevBgImg != ART_NULL ) ArtClose( gElevBgImgObj );
        if( gElevImg1 != ART_NULL ) ArtClose( gElevImgObj1 );
        for( i = 0; i != 3; i++ ) ArtClose( gElevButtImgObj[ i ] );
        if( gElevMap ) MapAmbientEnable();
        CycleColorStart();
        GmouseLoadCursor( 1 );
        return -1;
    }    
    if( (win = gElevWin = WinCreateWindow( ( 640 - gElevBGImgW ) / 2, ( 379 - gElevBGImgH ) / 2, gElevBGImgW, gElevBGImgH, 256, 18 )) == -1 ){
        if( gElevBgImg != ART_NULL ) ArtClose( gElevBgImgObj );
        if( gElevImg1 != ART_NULL ) ArtClose( gElevImgObj1 );
        for( i = 0; i != 3; i++ ) ArtClose( gElevButtImgObj[ i ] );
        if( gElevMap ) MapAmbientEnable();
        CycleColorStart();
        GmouseLoadCursor( 1 );
        return -1;
    }
    gElevSurface = WinGetSurface( win );
    memcpy( gElevSurface, gElevBgImg, gElevBGImgH * gElevBGImgW );
    if( gElevImg1 != ART_NULL ) ScrCopy( gElevImg1, gElevImgW1, gElevImgH1, gElevImgW1, (gElevSurface + gElevBGImgW * (gElevBGImgH - gElevImgH1)), gElevBGImgW );
    for( i = 0; i < gElevButtNum[ style ]; i++ ){
        bt = WinCreateButton( gElevWin, 13, 40 + i*60, gElevButtSize[ 0 ].x, gElevButtSize[ 0 ].y, -1, -1, -1, 500 + i, gElevButtImg[ 1 ], gElevButtImg[ 0 ], 0, 32 );
        if( bt != -1 ) WinSetClickSound( bt, GSoundPlayPushBt, 0 ); 
    }
    return 0;
}

void ElevatorMenuClose()
{
    int i;

    WinClose( gElevWin );
    if( gElevBgImg != ART_NULL ) ArtClose( gElevBgImgObj );
    if( gElevImg1 != ART_NULL ) ArtClose( gElevImgObj1 );
    for( i = 0; i != 3; i++ ) ArtClose( gElevButtImgObj[ i ] );
    ScptEnable();
    if( gElevMap ) MapAmbientEnable();
    CycleColorStart();
    GmouseLoadCursor( 1 );
}

int ElevatorKeySel( int Style, int Key )
{
    int i;
    char c;

    i = 0;
    do{
        if( !(c = gElevKeys[ Style ][ i ]) ) break;
        i++;
        if( Key == c ) return i;
    }while( i < 4 );
    return 0;
}


