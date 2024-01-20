#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include "FrameWork.h"


int FileCopy( const char* source, const char* destination )
{    
    off_t bytesCopied = 0;
    struct stat fileinfo = {0};
    int input, output, result;    

    if( (input = open( source, O_RDONLY ) ) == -1) return -1;        
    if( (output = creat(destination, 0660) ) == -1 ){ close( input ); return -1; }

    fstat( input, &fileinfo );
    result = sendfile( output, input, &bytesCopied, fileinfo.st_size );

    close( input );
    close( output );
    return result;
}

int FileInflateA( char *fname1, char *fname2 ) // inflate
{
    FILE *fh;
    int a,b;
    int c;
    zFile_t *fh1;
    FILE *fh2;

    if(!(fh = fopen( fname1, "rb" ))) return -1;
    a = fgetc( fh );
    b = fgetc( fh );
    fclose( fh );

    if( a == 0x1F && b == 0x8B ){ // source compressed
        fh1 = zOpenByFileName( fname1, "rb" );	
        fh2 = fopen( fname2, "wb" );
        if( fh1 && fh2 ){
            while ( (c = zgetc( fh1 )) != -1 ) fputc( c, fh2 );            
            zclose( fh1 );
            fclose( fh2 );
            return 0;
        } else {
            if( fh1 ) zclose( fh1 );
            if( fh2 ) fclose( fh2 );
            return -1;
        }
    } else {    
        FileCopy( fname1, fname2 );
        return 0;
    }
}

int FileDeflate( char *SrcFileName, char *NewFileName ) // deflate
{
    FILE *fh;
    zFile_t *zh;
    int a, b;

    if( !(fh = fopen( SrcFileName, "rb" )) ) return -1;
    a = fgetc( fh );
    b = fgetc( fh );
    rewind( fh );
    if( a == 31 && b == 139 ){
        fclose( fh );
        FileCopy( SrcFileName, NewFileName );
    } else {
        if( !(zh = zOpenByFileName( NewFileName, "wb" ) ) ){ fclose( fh ); return -1; }
        while( (a = fgetc( fh )) != -1 ) zputc( zh, a );
        fclose( fh );
        zclose( zh );
    }
    return 0;
}

int FileInflateB( char *SrcFileName, char *NewFileName )
{
    zFile_t *zh;
    FILE *fh;
    int a, b;


    if(!(fh = fopen( SrcFileName, "rb" ))) return -1;
    a = fgetc( fh );
    b = fgetc( fh );
    fclose( fh );
    if( a == 31 && b == 139 ){
        FileCopy( SrcFileName, NewFileName );
    } else {        
        if( !(zh = zOpenByFileName(SrcFileName, "rb")) ) return -1;
        if( !(fh = fopen( NewFileName, "wb" ) ) ){ zclose( zh ); return -1; }
        while ( (a = zgetc(zh)) != -1 ) fputc( a, fh );
        zclose( zh );
        fclose( fh );
    }
    return 0;
}

