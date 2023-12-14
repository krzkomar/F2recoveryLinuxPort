// no xref 
/*
Mov02_t *MovUnk20( int (*Cb)(xFile_t *, char *, int ), xFile_t *a2, xFile_t *fh )
{
    Mov02_t *result, *p;
    int v5, v6;
    char v7[4];
    int (*retaddr)(xFile_t *, char *, size_t);

    MovProtectMemArea();
    if( !gMovVideoDrv || !gMovAlloc ) return NULL;
    if( !(p = (Mov02_t *)gMovAlloc( sizeof( Mov02_t ) )) ) return NULL;
    memset( p, 0, sizeof( Mov02_t ) );
    MovUnk22( &v6 );
    MovUnk21( p );
    MovSetInput( retaddr );
    p->i01 = fh;
    if( !(v5 = MovLoadHdr( a2 ) == 0) ){
        p->data = MovLoadChunk();
        p->Idx = 0;
    }
    MovUnk22( p );
    MovUnk21( v7 );
    if( v5 ){ MovUnk24( p ); return NULL; }
    return p;
}

void MovUnk21( Mov02_t *p )
{
    gMovReadFileCb = p->ReadFile;
    gMovInputData = *(MveUnk01_t *)&p->Input;
    gMovFileHandler = p->file2;
//    gMovChunkHdr = p->ChunkHdr;
    gMovSurfaceA = p->SurfA;
    gMovSurfaceB = p->SurfB;
    gMovUnk101 = p->i12;
    gMovScrLockA = p->ScrLockA;
    gMovScrLockB = p->ScrLockB;
    gMveScrWidth = p->ScrW;
    gMovScrHeight = p->ScrH;
    gMovScrSize = p->ScrSize;
    gMov16bppMode = p->Color;
    gMovSurfWidth = p->SurfH;
    gMovSurfHeight = p->SurfW;
    gMovScrBufSize = p->BufSizeA;
    gMovScrBufSizeA = p->BufSizeB;
    gMovScrBufSizeB = p->BufSizeC;
}

void MovUnk22( Mov02_t *p )
{
    p->ReadFile = gMovReadFileCb;
//    p->Input = gMovInputData;
    p->file2 = gMovFileHandler;
//    p->ChunkHdr = gMovChunkHdr;
    p->SurfA = gMovSurfaceA;
    p->SurfB = gMovSurfaceB;
    p->i12 = gMovUnk101;
    p->ScrLockA = gMovScrLockA;
    p->ScrLockB = gMovScrLockB;
    p->ScrW = gMveScrWidth;
    p->ScrH = gMovScrHeight;
    p->ScrSize = gMovScrSize;
    p->Color = gMov16bppMode;
    p->SurfW = gMovSurfWidth;
    p->SurfH = gMovSurfHeight;
    p->BufSizeA = gMovScrBufSize;
    p->BufSizeB = gMovScrBufSizeA;
    p->BufSizeC = gMovScrBufSizeB;
}

int MovStepA( Mov02_t *mov, void *a2, int *a3, int *a4 )
{
    char *pData; // esi
    int pos; // edi
    char *v6; // ebp
    char *v7; // esi
    int v8; // eax
    int v9; // ecx
    int v10; // ebx
    int v11; // eax
    int v12; // ecx
    int err; // [esp+10h] [ebp-5Ch]
    Mov02_t p; // [esp+18h] [ebp-54h] BYREF

    err = 0;
    MovUnk22( &p );
    MovUnk21( mov );
    pData = mov->data;
    pos = mov->Idx;
    v6 = 0;
    while( pData ){
        v8 = *(int *)&pData[ pos ];
        pData = &pData[ pos ] + 4;
        pos = (unsigned short)v8;
        switch( TYPE( char, v8, 2 ) ){
            case 0:
                err = -1;
                break;
            case 1:
                pos = 0;
                pData = MovLoadChunk();
                v6 = 0;
                continue;
            case 5:
                v9 = 0;
                if( TYPE( char, v8 , 3 ) >= 2 ) v9 = TYPE( short, pData, 3 );
                v10 = gMovType;
                gMovType = 0;
                if( !v9 ){
            	    if( TYPE( char, v8, 3 ) ){
                	if( TYPE( short, pData, 2 ) != 1 ){
            		    err = -5;
            		    gMovType = v10;
            		    break;
                	}
                	v11 = TYPE( short, pData, 2 );
        	    } else {
                	v11 = 1;
            	    }
            	    if( MovVideoBufferInit(*(_WORD *)pData, *((_WORD *)pData + 1), v11, 0) ){
            		gMovType = v10;
			err = -2;
			break;
            	    }
                }
                err = -5;
                gMovType = v10;
                break;
            case 7:
                v12 = 0;
                if( TYPE( char, v8, 3 ) ) LOWORD(v12) = *((_WORD *)pData + 2);
                if( v12 ){
                    err = -8;
                } else {
                    *a2 = gMovSurfaceA;
                    *a3 = gMovSurfWidth;
                    *a4 = gMovSurfHeight;
                }
                break;
            case 0xF:
                v6 = pData;
                continue;
            case 0x11:
                if( HIBYTE(v8) < 3 ){ err = -8; break; }
                if( (pData[ 12 ] & 1) != 0 ) MovSwapSurfaces();
                if( gMov16bppMode ){ err = -8; break; }
                MovProtUnk07();
                if( !MovFlipScreen() ){ err = -12; break; }
                MovDecodeFrameD( v6, pData + 14, *((_WORD *)pData + 2), *((_WORD *)pData + 3), *((_WORD *)pData + 4), *((_WORD *)pData + 5) );
                MovUnlockSurfaces();
                continue;
            default:
                if( !mov->i01 ) continue;
                if( !(err = mov->i01(BYTE2(v8), HIBYTE(v8), pData)) ) continue;
                break;
        }
        break;
    }
    MovUnk22( mov );
    mov->data = pData;
    mov->Idx = pos;
    MovUnk21( &p );
    MovProtUnk07();
    return err;
}

void MovUnk24( Mov02_t *a1 )
{
    Mov02_t p;

    MovUnk22( &p );
    MovUnk21( a1 );
    MovUnk52();
    MovVideoClose();
    MovUnk21( &p );
    if( gMovFree ) gMovFree( a1 );
}

*/
