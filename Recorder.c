#include "FrameWork.h"


Rec_t *gRec = NULL;
int gRecFrame = 0;
char gRecState = 2;
char gRecUnk01 = 0;
int gRecPlayUnk05 = 0;
int gRecUnk04 = 0;
int gRecPlayUnk04 = 0;
int gRecPlayUnk03 = 0;
unsigned int gRecTimer = 0;
int gRecUnk02 = 0;
xFile_t *gRecFile = NULL;
int gRecUnk06 = 0;
int (*gRecPlayUnk02)(void) = NULL;
int gRecPlayUnk01 = 0;
int gRecUnk07 = 0;

Rec_t gRecUnk08;

int RecRecInit( const char *fname )
{
    if( gRecState != 2 || !fname ) return 0;
    if( !gRec ) return 0;
    if( !( gRec = Malloc( RECORD_FRAMES_BUFFER_SIZE * sizeof( Rec_t )) ) ) return 0;

    if( !( gRecFile = dbOpen( fname, "wb" ) ) ){
        if( gRec ){
            RecReset();
            Free( gRec );
            gRec = NULL;
        }
        return 0;
    }
    if( !gRecUnk02 ) gRecUnk02 = xCleanAdd( RecClr );    
    gRec[ gRecFrame ].i00 = 1;
    gRec[ gRecFrame ].i01 = 0;
    gRec[ gRecFrame ].i02 = 0;
    gRec[ gRecFrame ].mbutt = KeyGetLangId();
    while( MseGetButtons() ) MseUpdate();
    MseGetCursorPosition( &gRec[ gRecFrame ].mx, &gRec[ gRecFrame ].my );
    gRecUnk04 = 1;
    gRecFrame++;
    gRecTimer = TimerGetSysTime();
    KeyFlush();
    gRecState = 0;
    return 1;
}

int RecPlayInit( const char *fname, int a2, int (*a3)(void) )
{
    int v5;

    if( gRecState != 2 || !fname ) return 0;
    if( gRec || (gRec = Malloc( RECORD_FRAMES_BUFFER_SIZE * sizeof( Rec_t ) )) == 0 ){
        v5 = 0;
    } else {
        RecReset();
        v5 = 1;
    }
    if( v5 != 1 ) return 0;    

    if( (gRecFile = dbOpen(fname, "rb")) ){
	if( RecPlay() == 1 ){
	    while( MseGetButtons() ) MseUpdate();
	    KeyFlush();
	    gRecPlayUnk01 = a2;
	    gRecPlayUnk02 = a3;
	    gRecPlayUnk03 = 1;
	    gRecPlayUnk04 = 0;
	    gRecUnk04 = 0;
	    gRecPlayUnk05 = 0;
	    gRecTimer = TimerGetSysTime();
	    gRecState = 1;
	    gRecUnk08.i01 = 0;
	    gRecUnk08.i02 = 0;
	    return 1;
	}
	dbClose( gRecFile );
    }
    if( gRec ){
        RecReset();
        Free( gRec );
        gRec = NULL;
    }
    return 0;
}

void RecClr()
{
    if( gRecState <= 1 ) gRecUnk01 |= 0x80;
    KeyFlush();
}

int RecGetState()
{
    return gRecState;
}

int RecRecord()
{
    int err;
    Rec_t *v2, *v8;
    unsigned int v4 = 0, v5, v7; // v4 not identified !!!!

    err = 0;
    if( gRecUnk01 < 0 ){
        gRecUnk01 &= ~0x80;
        if( gRecState ){
            if ( gRecState == 1 )
            {
                dbClose(gRecFile);
                gRecFile = 0;
                if( gRec ){
                    RecReset();
                    Free( gRec );
                    gRec = 0;
                }
                KeySetLang( gRecUnk07 );
                if( gRecPlayUnk02 ) gRecPlayUnk02();
            }
        } else {
            RecFlushOut();
            dbClose( gRecFile );
            gRecFile = 0;
            if( gRec ){
                RecReset();
                Free(gRec);
                gRec = 0;
            }
        }
        gRecState = 2;
    }
    if( !gRecState ){
        gRecUnk04++;
        gRecPlayUnk05 = TimerCurrDiff(gRecTimer);
        if( gRecFrame == (RECORD_FRAMES_BUFFER_SIZE - 1) ) RecFlushOut();
        return err;
    }
    if( gRecState > 1 ) return err;
    if( gRecUnk06 > gRecFrame || RecPlay() ){
        v2 = &gRec[ gRecFrame ];
        if( gRecUnk08.i02 < v2->i02 && v2->i01 > gRecUnk08.i01 ){        
            while( TimerCurrDiff( gRecTimer ) < v4 ); // v4 ??
        }
        gRecUnk04++;
        while( gRecUnk04 >= gRec[ gRecFrame ].i02 ){
            v5 = TimerCurrDiff( gRecTimer );
            v7 = gRec[ gRecFrame ].i01 + 5;
            gRecPlayUnk05 = v5;
            if( v5 > v7 || v5 < gRec[ gRecFrame ].i01 - 5 ) gRecTimer += gRecPlayUnk05 - gRec[ gRecFrame ].i01;
            v8 = &gRec[ gRecFrame ];
            switch( v8->i00 ){
                case 1: // synchro
                    gRecState = 2;
                    gRecUnk07 = KeyGetLangId();
                    KeySetLang( gRec[ gRecFrame ].mbutt );
                    while( MseGetButtons() ) MseUpdate();
                    gRecState = 1;
                    MseCursorHide();
                    MseSetCursorPosition( gRec[ gRecFrame ].mx, gRec[ gRecFrame ].my );
                    MseCursorShow();
                    KeyFlush();
                    gRecPlayUnk04 = gRecPlayUnk01;
                    gRecTimer = TimerGetSysTime();
                    gRecUnk04 = 0;
                    break;
                case 2: // rekord klawiatury
                    KeyCodeInsert( v8->mx );
                    break;
                case 3: // rekord myszy
                    err = 3;
                    MseProcess( v8->mx, v8->my, v8->mbutt );
                    break;
                default:
                    break;
            }
            memcpy( &gRecUnk08, &gRec[ gRecFrame++ ], sizeof( Rec_t ) ); // nigdzie nie wykorzystane? ....
        }
        return err;
    }
    if( gRecState <= 1 ) gRecUnk01 |= 0x80u;
    KeyFlush();
    return 0;
}

int RecAllocBuffer()
{
    if( gRec ) return 0;
    if( !(gRec = Malloc( RECORD_FRAMES_BUFFER_SIZE * sizeof( Rec_t ) ) ) ) return 0;
    gRecFrame = 0;
    return 1;
}

int RecFreeBuffer()
{
    if( !gRec ) return 0;
    gRecFrame = 0;
    Free( gRec );
    gRec = NULL;
    return 1;
}

int RecReset()
{
    if( !gRec ) return 0;
    gRecFrame = 0;
    return 1;
}

int RecFlushOut()
{
    int i;

    if( !gRec || !gRecFile ) return 0;
    
    for( i = 0; i < gRecFrame; i++ ){
        if( !RecSaveFrame( &gRec[ i ], gRecFile ) ) break;
    }
    if( gRec ) gRecFrame = 0;
    if( i == gRecFrame && gRec ) return 1;
    return 0;
}

int RecPlay()
{
    int err, i;

    i = gRecUnk06;
    if( gRecFile ){
	if( gRec ){
	    gRecFrame = 0;
	    i = 0;
	    do {
    		gRecUnk06 = i;
    		err = RecLoadFrame( &gRec[ i ], gRecFile );
    		i = gRecUnk06;
    		if( !err ) break;
    		i = gRecUnk06 + 1;
	    }while ( i < 4096 );
	    if( i ){
		gRecUnk06 = i;
		return 1;
	    }
	}
    }
    gRecUnk06 = i;
    return 0;
}

int RecSaveFrame( Rec_t *Frame, xFile_t *fh )
{
    int err;

    err = 0;
    if( dbputLei(fh, Frame->i00) != -1 && dbputLei(fh, Frame->i01) != -1 && dbputLei(fh, Frame->i02) != -1 ){
        switch( Frame->i00 ){
            case 1:
                if( dbputLei(fh, Frame->mx) != -1 && dbputLei(fh, Frame->my) != -1 && dbputLei(fh, Frame->mbutt) != -1 ) err = 1;
                break;
            case 2:
        	if( dbputBew(fh, Frame->mx) != -1 )  err = 1;
                break;
            case 3:
                if( dbputLei(fh, Frame->mx) != -1 && dbputLei(fh, Frame->my) != -1 && dbputLei(fh, Frame->mbutt) != -1 ) err = 1;
                break;
            default:
                return err;
        }
    }
    return err;
}

int RecLoadFrame( Rec_t *Frame, xFile_t *fh )
{
    int err = 0;

    if( dbgetBei(fh, &Frame->i00) != -1 && dbgetBei(fh, &Frame->i01) != -1 && dbgetBei(fh, &Frame->i02) != -1 ){
        switch( Frame->i00 ){
            case 1:
                if( dbgetBei(fh, &Frame->mx) != -1 && dbgetBei(fh, &Frame->my) != -1 && dbgetBei(fh, &Frame->mbutt) != -1 ) err = 1;
                break;
            case 2:
                if( dbgetBew(fh, (short *)&Frame->mx) != -1 ) err = 1;
                break;
            case 3:
                if( dbgetBei(fh, &Frame->mx) != -1 && dbgetBei(fh, &Frame->my) != -1 && dbgetBei(fh, &Frame->mbutt) != -1 ) err = 1;
                break;
            default:
                return err;
        }
    }
    return err;
}

