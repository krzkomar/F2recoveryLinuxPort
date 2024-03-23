#include "FrameWork.h"

int gSdf = 0;

int SdfOpen( char ***FileList, int *Cnt )
{
    if( !FileList || !Cnt ) return -1;
    *Cnt = dbGetFileList( "selfrun/*.sdf", FileList );
    return 0;
}

int SdfClose( char **List )
{
    if( !List ) return -1;
    dbDelFileList( List );
    return 0;
}

int SdfUnk01( char *fname, Sdf_t *sdf )
{
    int err;
    char stmp[ 260 ];

    if( !fname || !sdf ) return -1;
    if( RecGetState() != 2 ) return -1;
    if( gSdf ) return -1;
    sprintf( stmp, "%s%s", "selfrun/", fname );    
    if( (err = SdfUnk06( stmp, sdf )) ) return -1;
    gSdf = 1;
    return err;
}

void SdfUnk02( Sdf_t *sdf )
{
    int clr, sel;
    char stmp[ 260 ];

    if( gSdf != 1 ) return;
    sprintf( stmp, "%s%s", "selfrun/", sdf->vcrname );
    if( RecPlayInit( stmp, 5, (void *)SdfUnk05 ) != 1 ) return;
    clr = MseCursorHidden();
    if( clr ) MseCursorShow();
    while( gSdf == 1 ){
        sel = InpUpdate();
        if( sel != sdf->i28 ) GameProcess( sel, 0 );
    }
    while( MseGetButtons() ) InpUpdate();
    if( !(clr == 0) ) MseCursorHide();
}

int SdfUnk03( char *fname, char *vcrname, Sdf_t *sdf )
{
    int err;
    char stmp[ 260 ];

    if( !fname || !vcrname || RecGetState() != 2 || gSdf ) return -1;
    sprintf( sdf->vcrname, "%s%s", fname, ".vcr" );
    strcpy( sdf->sdfname, vcrname );
    sdf->i28 = 18;
    sprintf( stmp, "%s%s%s", "selfrun/", fname, ".sdf" );
    if( (err = SdfUnk07( stmp, sdf )) ) return -1;
    gSdf = 2;
    return err;
}

void SdfUnk04( Sdf_t *sdf )
{
    int brk, sel;
    char stmp[ 260 ];

    if( gSdf != 2 ) return;
    sprintf( stmp, "%s%s", "selfrun/", sdf->vcrname );
    if( RecRecInit( stmp ) == 1 ){        
        if( MseCursorHidden() ) MseCursorShow();
        for( brk = 0; !brk; ){
            sel = InpUpdate();
            if( sel == sdf->i28 ){
                RecClr();
                brk = 1;
                gMenuEscape = 2;
            } else {
                GameProcess( sel, 0 );
            }
        }
    }
    gSdf = 0;
}

void SdfUnk05()
{
    gMenuEscape = 2;
    gSdf = 0;
}

int SdfUnk06( char *fname, Sdf_t *sdf )
{
    int err;
    xFile_t *fh;

    err = -1;
    if( !fname || !sdf ) return -1;    
    if( !( fh = dbOpen( fname, "rb" ) ) ) return -1;
    if( !dbreadByteBlk( fh, sdf->vcrname, 13 ) && !dbreadByteBlk( fh, sdf->sdfname, 13 ) && !dbgetBei( fh, &sdf->i28 ) ) err = 0;
    dbClose( fh );
    return err;
}

int SdfUnk07( const char *fname, Sdf_t *sdf )
{
    xFile_t *fh;
    char stmp[ 260 ], *patch;
    int err;

    if( !fname || !sdf ) return -1;
    CfgGetString( &gConfiguration, "system", "master_patches", &patch );
    sprintf( stmp, "%s/%s", patch, "selfrun/" );
    xDirCreate( stmp );
    if( !(fh = dbOpen( fname, "wb" )) ) return -1;
    err = -1;
    if( !dbputbBlk( fh, sdf->vcrname, 13 ) && !dbputbBlk( fh, sdf->sdfname, 13 ) && !dbputBei( fh, sdf->i28 ) ) err = 0;
    dbClose( fh );
    return err;
}



