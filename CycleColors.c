#include "FrameWork.h"

static int  gCycleColorSpeed = 1;

static Pal8_t gCycPalWastes[ 4 ] = { 
    { 0x00, 0x6C, 0x00 },
    { 0x0B, 0x73, 0x07 },
    { 0x1B, 0x7B, 0x0F },
    { 0x2B, 0x83, 0x1B } 
};

static Pal8_t gCycPalScreens[ 6 ] = { 
    { 0x53, 0x3F, 0x2B },
    { 0x4B, 0x3B, 0x2B },
    { 0x43, 0x37, 0x27 },
    { 0x3F, 0x33, 0x27 },
    { 0x37, 0x2F, 0x23 },
    { 0x33, 0x2B, 0x23 } 
};

// red, orange
static Pal8_t gCycPalFire[ 5 ] = { 
    { 0xFF, 0x00, 0x00 },
    { 0xD7, 0x00, 0x00 },
    { 0x93, 0x2B, 0x0B },
    { 0xFF, 0x77, 0x00 },
    { 0xFF, 0x3B, 0x00 } 
};

// red
static Pal8_t gCycPalBurn[ 5 ] = { 
    { 0x47, 0x00, 0x00 },
    { 0x7B, 0x00, 0x00 },
    { 0xB3, 0x00, 0x00 },
    { 0x7B, 0x00, 0x00 },
    { 0x47, 0x00, 0x00 } 
};

static Pal8_t gCycPalEE[ 5 ] = { 
    { 0x6B, 0x6B, 0x6F },
    { 0x63, 0x67, 0x7F },
    { 0x57, 0x6B, 0x8F },
    { 0x00, 0x93, 0xA3 },
    { 0x6B, 0xBB, 0xFF } 
};

static int  gCycleColorInited = 0;
static int  gCycleColorEnable = 0;
static int  gCycleColorCountWastes = 0;
static int  gCycleColorCountScreens = 0;
static int  gCycleColorCountFire = 0;
static int  gCycleColorCountBurn = 0;
static int  gCycleColorCountA = 0;
static int  gCycleColorRed = 0;
static char gCycleColorRedDelta = -4;

static int gCycleColorTimeB;
static int gCycleColorTimeE;
static int gCycleColorTimeC;
static int gCycleColorTimeA;

static void CycleColorsTask();

void CycleColorsInit()
{
    int i, speed, ColCycling;

    if( gCycleColorInited ) return;
    if( !CfgGetInteger( &gConfiguration, "system", "color_cycling", &ColCycling ) ) ColCycling = 1;
    if( !ColCycling ) return;
    
    for( i = 0; i < 12; i++ ) ((char*)gCycPalWastes)[i] >>= 2;
    for( i = 0; i < 18; i++ ) ((char*)gCycPalScreens)[i] >>= 2;
    for( i = 0; i < 15; i++ ) ((char*)gCycPalFire)[i] >>= 2;
    for( i = 0; i < 15; i++ ) ((char*)gCycPalBurn)[i] >>= 2;
    for( i = 0; i < 15; i++ ) ((char*)gCycPalEE)[i] >>= 2;
    InpTaskStart( CycleColorsTask );
    gCycleColorInited = 1;
    gCycleColorEnable = 1;
    if( !CfgGetInteger( &gConfiguration, "system", "cycle_speed_factor", &speed) ) speed = 1;
    CycleColorsSetSpeed( speed );
}

void CycleColorRestart()
{
    if( !gCycleColorInited ) return;
    
    gCycleColorTimeE = 0;
    gCycleColorTimeC = 0;
    gCycleColorTimeB = 0;
    gCycleColorTimeA = 0;
    InpTaskStart( CycleColorsTask );
    gCycleColorEnable = 1;
}

void CycleColorDisable()
{
    if( !gCycleColorInited ) return;    
    InpTaskStop( CycleColorsTask );
    gCycleColorInited = 0;
    gCycleColorEnable = 0;    
}

void CycleColorStop()
{
    gCycleColorEnable = 0;
}

void CycleColorStart()
{
    gCycleColorEnable = 1;
}

int CycleColorIsEnabled()
{
    return gCycleColorEnable;
}

int CycleColorsSetSpeed( int Speed )
{
    gCycleColorSpeed = Speed;
    return CfgSetInteger( &gConfiguration, "system", "cycle_speed_factor", Speed );
}

int CycleColorGetSpeed()
{
    return gCycleColorSpeed;
}

static void CycleColorsTask()
{
    Pal8_t *Current;
    int Update, i, j, SysTime, thr;

    if( !gCycleColorEnable ) return;

    Update = 0;
    Current = PalGetCurrent();
    SysTime = TimerGetSysTime();
    thr = 200 * gCycleColorSpeed;

    if( TimerDiff(SysTime, gCycleColorTimeE) >= thr ){
        Update = 1;        
        gCycleColorTimeE = SysTime;

        // radioactive wastes animation, green 229 ... 232 
        j = 229;
        for( i = gCycleColorCountWastes; i < 4; i++, j++ ) Current[ j ] = gCycPalWastes[ i ];
        for( i = 0; i < gCycleColorCountWastes; i++, j++ ) Current[ j ] = gCycPalWastes[ i ];
        if( --gCycleColorCountWastes < 0 ) gCycleColorCountWastes = 3;

	// computer screen animation, light blue 248 ... 253
        j = 248;
        for( i = gCycleColorCountScreens; i < 6; i++, j++ ) Current[ j ] = gCycPalScreens[ i ];
        for( i = 0; i < gCycleColorCountScreens; i++, j++ ) Current[ j ] = gCycPalScreens[ i ];
        if( --gCycleColorCountScreens < 0 ) gCycleColorCountScreens = 5;        

	// fire animation, orange/red/yellow 238 ... 242
        j = 238;
        for( i = gCycleColorCountFire; i < 5; i++, j++ ) Current[ j ] = gCycPalFire[ i ];
        for( i = 0; i < gCycleColorCountFire; i++, j++ ) Current[ j ] = gCycPalFire[ i ];
        if( --gCycleColorCountFire < 0 ) gCycleColorCountFire = 4;        


    }

    if( TimerDiff( SysTime, gCycleColorTimeC ) >= thr ){
        gCycleColorTimeC = SysTime;
        Update = 1;
        // burn animation, red  243 .. 247
        j = 243;
        for( i = gCycleColorCountBurn; i < 5; i++, j++ ) Current[ j ] = gCycPalBurn[ i ];        
        for( i = 0; i < gCycleColorCountBurn; i++, j++ ) Current[ j ] = gCycPalBurn[ i ];
        if( --gCycleColorCountBurn < 0 ) gCycleColorCountBurn = 4;
    }

    if( TimerDiff( SysTime, gCycleColorTimeB ) >= thr ){
        Update = 1;
        gCycleColorTimeB = SysTime;
        j = 233;
        for( i = gCycleColorCountA; i < 5; i++, j++ ) Current[ j ] = gCycPalEE[ i ];
        for( i = 0; i < gCycleColorCountA; i++, j++ ) Current[ j ] = gCycPalEE[ i ];
        if( --gCycleColorCountA < 0 ) gCycleColorCountA = 4;
    }

    if( TimerDiff( SysTime, gCycleColorTimeA ) >= thr ){
        gCycleColorTimeA = SysTime;
        Update = 1;
        if( (gCycleColorRed == 0) || gCycleColorRed == 60 ) gCycleColorRedDelta = -gCycleColorRedDelta;
        gCycleColorRed += gCycleColorRedDelta;
        Current[ 254 ].g = 0;
        Current[ 254 ].b = 0;
        Current[ 254 ].r = gCycleColorRed;
    }
    if( Update ) FadePalMod( Current, 229, 255 );
}

