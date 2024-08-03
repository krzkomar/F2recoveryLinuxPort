#include "FrameWork.h"

static void (*gdbRWfuncCb)() = NULL;
static int gXgetsLastPos = 0;
static int gdbRWfuncThres;
static xPath_t *gDbList;

static int dbSortCb( const void *Str1, const void *Str2);

int dbLoadData( char *patchfilename, char *datafilename )
{
    if( datafilename && !xOpenList( datafilename ) ) return -1;
    if( patchfilename ) xOpenList( patchfilename );
    return 0;
}

int dbNull()
{
    /* Empty */
    return 0;
}

void dbFree()
{
    xLoadFileList( 0 );
}

int dbCheckFileLength( const char *filename, int *pLen )
{
    xFile_t *xf;

    assert( filename );		// _assert_(0, "filename", 108, "db.c");
    assert( pLen );		// _assert_(0, "de", 109, "db.c");
    if( !(xf = xopen( filename, "rb") ) ) return -1;
    *pLen = xfilelength_int(xf);
    xclose_int( xf );
    return 0;
}

int dbReadFile( const char *filename, void *buf )
{
    void *p;
    xFile_t *xf;
    int flen, max, i, chunksize;
    unsigned int readout;

    p = buf;
    assert( filename );	// _assert_(0, "filename", 141, "db.c");
    assert( buf );	// _assert_(0, "buf", 142, "db.c");
    if( !(xf = xopen(filename, "rb")) ) return -1;
    flen = xfilelength_int(xf);
    max = flen;
    if( gdbRWfuncCb ){
        i = flen;
        chunksize = gdbRWfuncThres - gXgetsLastPos;
        if( max >= gdbRWfuncThres - gXgetsLastPos ){
            do{
                readout = xread_int( p, 1, chunksize, xf );
                p += readout;
                i -= readout;
                gXgetsLastPos = 0;
                gdbRWfuncCb();
                chunksize = gdbRWfuncThres;
            } while ( i >= gdbRWfuncThres );
        }
        if( i ) gXgetsLastPos += xread_int( p, 1, i, xf );
    } else {
        xread_int( p, 1, flen, xf );
    }
    xclose_int(xf);
    return 0;
}

int dbClose( xFile_t *stream )
{
    xclose_int( stream );
    return (stream == NULL) - 1;
}

xFile_t *dbOpen( const char *fname, const char *mode )
{
    return xopen( fname, mode );
}

int dbPrintf( xFile_t *stream, const char *fmt, ... )
{
    va_list vlist;
    va_list va;

    va_start( va, fmt );
    assert( fmt );	// _assert_(0, "format", 224, "db.c");
    va_copy( vlist, va );
    return xvprintf( stream, fmt, vlist );
}

int dbPuts( xFile_t *stream, char *str )
{
    return xprintf( stream, str );
}

int dbgetc( xFile_t *stream )
{
    int data;

    if( !gdbRWfuncCb ) return xgetc_int( stream );

    data = xgetc_int( stream );
    if( ++gXgetsLastPos >= gdbRWfuncThres ){
        gdbRWfuncCb();
        gXgetsLastPos = 0;
    }
    return data;
}

char *dbgets( char *str, int num, xFile_t *xstream )
{
    char *s;
    unsigned int lastpos;

    if( !gdbRWfuncCb ) return xgets_int(str, num, xstream);

    s = xgets_int(str, num, xstream);
    if( s ){
        lastpos = strlen(str) + gXgetsLastPos;
        if( lastpos >= gdbRWfuncThres ){
            do{
                gXgetsLastPos = lastpos;
                gdbRWfuncCb();
                lastpos = gXgetsLastPos - gdbRWfuncThres;
            }while( gXgetsLastPos - gdbRWfuncThres >= gdbRWfuncThres );
        }
        s = str;
        gXgetsLastPos = lastpos;
    }
    return s;
}

int dbputc( int chr, xFile_t *stream )
{
    return xputc_int( chr, stream );
}

int dbputs( char *str, xFile_t *stream )
{
    return xputs_int( str, stream );
}

int dbread( void *buffer, size_t size, size_t count, xFile_t *stream )
{
    void *pdata;
    int nbytes, remain;
    unsigned int bytes, tmp, nwr;

    pdata = buffer;
    if( !gdbRWfuncCb ) return xread_int(buffer, size, count, stream);
    nwr = 0;
    nbytes = count * size;
    remain = gdbRWfuncThres - gXgetsLastPos;
    if( nbytes >= remain ){
        do{
            bytes = xread_int(pdata, 1u, remain, stream);
            pdata = pdata + bytes;
            nbytes -= bytes;
            gXgetsLastPos = 0;
            tmp = bytes;
            gdbRWfuncCb();
            remain = gdbRWfuncThres;
            nwr = tmp;
        }while ( nbytes >= gdbRWfuncThres );
    }
    if( nbytes ){
        tmp = xread_int(pdata, 1u, nbytes, stream);
        gXgetsLastPos += tmp;
        nwr += tmp;
    }
    return nwr / size;
}

int dbwrite( void *buffer, size_t size, size_t count, xFile_t *stream )
{
    return xwrite_int( buffer, size, count, stream );
}

int dbseek( xFile_t *stream, int offset, int origin )
{
    return xseek_int( stream, offset, origin );
}

int dbtell( xFile_t *stream )
{
    return xtell_int( stream );
}

void dbrewind( xFile_t *stream )
{
    xrewind_int( stream );
}

int dbfeof( xFile_t *stream )
{
    return xfeof_int( stream );
}

int dbgetb( xFile_t *stream, char *byte )
{
    int chr;

    chr = dbgetc(stream);
    if( chr == -1 ) return -1;
    *byte = chr;
    return 0;
}

int dbgetBew( xFile_t *stream, short *pword )
{
    int err;
    unsigned char tmp;
    short a;

    err = dbgetc(stream);
    if( err != -1 ){
        tmp = err;
        err = 0;
    }
    if( err != -1 ){
        *pword = tmp;
        err = dbgetc(stream);
        if( err != -1 ){
            tmp = err;
            err = 0;
        }
        if( err != -1 ){
            a = *pword << 8;
            *pword = a;
            err = 0;
            *pword = tmp | a;
        }
    }
    return err;
}

int dbgetBei( xFile_t *stream, int *pint)
{
    int tmp;

    if( xread_int( &tmp, 4, 1, stream ) == -1 ) return -1;
//    tmp = (( tmp & 0x0000ffff) << 16) | (( tmp & 0xffff0000) >> 16 );
    *pint = BSWAP32(tmp); //(( tmp & 0x00ff00ff) << 8) | (( tmp & 0xff00ff00) >> 8 );
    return 0;
}

int dbputb( xFile_t *stream, int bdata )
{
    return ( xputc_int( bdata, stream ) != -1) - 1;
}

int dbputBew( xFile_t *stream, short wdata )
{
    if( ((xputc_int(wdata >> 8, stream) != -1) - 1) == -1 ) return -1;
    return (xputc_int(wdata, stream) != -1) - 1;
}

int dbputBei( xFile_t *stream, int idata )
{
    if( dbputBew( stream, idata >> 16 ) == -1 ) return -1;
    return dbputBew( stream, idata );    
}

int dbputLei( xFile_t *stream, int idata)
{
    if( dbputb( stream, idata ) == -1 ) return -1;
    if( dbputb( stream, idata >> 8 ) == -1 ) return -1;    
    if( dbputb( stream, idata >> 16 ) == -1 ) return -1;    
    return dbputb( stream, idata >> 24 );
}

int dbputBed( xFile_t *stream, unsigned int wdata )
{ 
    if( dbputBew( stream, wdata >> 16) != -1 ) return -1;
    return dbputBew( stream, wdata );
}

int dbreadByteBlk( xFile_t *stream, char *buff, int size )
{
    int i, a;

    for( i = 0; i < size; i++, buff++ ){
        if( (a = dbgetc( stream )) == -1 ) return -1;
        *buff = a;
    }
    return 0;
}

int dbreadBewBlk( xFile_t *stream, short *pwdata, int size )
{
    int i, tmp;

    for( i = 0; i < size; i++, pwdata++ ){
        if( (tmp = dbgetc( stream ) ) == -1 ) return -1;
        *pwdata = (tmp << 8) & 0xff00;
        if( (tmp = dbgetc( stream ) ) == -1 ) return -1;
        *pwdata |= tmp;
    }
    return 0;
}

int dbreadBeiBlk( xFile_t *stream, int *pidata, int size )
{
    unsigned int i, *p;

    if( size == 0 ) return 0;
    if( !dbread( pidata, 4 * size, 1, stream ) ) return -1;
    p = (unsigned int *)pidata;
    for( i = 0; i< size; i++, p++ ){
	*p = BSWAP32( *p );
    }
    return 0;
}

int dbputbBlk( xFile_t *stream, char *pbdata, int size )
{
    int i;

    for( i= 0; i < size; i++, pbdata++ ){
        if( xputc_int( *pbdata, stream ) == -1 ) return -1;
    }
    return 0;
}

int dbputBewBlk( xFile_t *stream, short *pwdata, int size )
{
    int i;

    for( i= 0; i < size; i++, pwdata++ ){
        if( ((xputc_int( *pwdata >> 16, stream ) != -1) - 1) == -1 ) return -1;
        if( ((xputc_int( *pwdata, stream ) != -1) - 1) == -1 ) return -1;
    }
    return 0;
}

int dbputBeiBlk( xFile_t *stream, int *pidata, int size )
{
    int i;

    for( i= 0; i < size; i++, pidata++ ){    
        if ( dbputBew(stream, *pidata >> 16 ) == -1 ) return -1;
        if ( dbputBew(stream, *pidata) == -1 ) return -1;
    }
    return 0;
}

int dbputLeiBlk( xFile_t *stream, int *pidata, int size )
{
    int i;

    for( i= 0; i < size; i++, pidata++ ){    
        if( dbputb( stream, *pidata ) == -1 ) return -1;
        if( dbputb( stream, *pidata >> 8 ) == -1 ) return -1;
        if( dbputb( stream, *pidata >> 16 ) == -1 ) return -1;
        if( dbputb( stream, *pidata >> 24 ) == -1 ) return -1;
    }
    return 0;
}

int dbGetFileList( char *filepath, char ***FileList )
{
    xPath_t *p, *ndb1, *ndb2;
    unsigned int i, nsize;
    int Count;
    char **PathList;
    char fname[260], path[260], ext[260], *fpath;

    if( !(p = malloc( sizeof( xPath_t ) )) ) return 0;
    memset( p, 0x00, sizeof( xPath_t ) );
    fpath = filepath;
    ndb1 = ndb2 = p;
    Count = 0;

    if( !xLoadPathList( fpath, ndb1 ) ){ free( ndb1 ); return 0; }
    if( ndb1->nsize ){        
        qsort(ndb1->FullPathList, ndb1->nsize, sizeof(char *), dbSortCb);
        nsize = ndb1->nsize;
        for( i = 0; i < nsize - 1; ){ // usuwa duplikaty
            if( strcasecmp(ndb2->FullPathList[ i ], ndb2->FullPathList[ i + 1 ] ) ){
                i++;
            } else { // duplikaty
                memmove(ndb2->FullPathList[ i + 1 ], &ndb2->FullPathList[ i + 2 ], ((ndb2->nsize - i) - 2) * sizeof(char *) ); //  przesun elementy o 1 pozycje w prawo
                ndb2->FullPathList[ ndb2->nsize - 1 ] = ndb2->FullPathList[ i + 1 ];
                nsize--;
            }
        }                
        for( Count = i = 0; i < nsize; i++ ){
            xSplitPath( ndb2->FullPathList[ i ], path, fname, ext );
            if( (*fpath == '*') || (*path == '\0') ){
        	if( strchr( path, '/') != NULL ) continue;
            }
            sprintf( ndb2->FullPathList[ i ], "%s%s", fname, ext );
            Count++;                                	    
	}
    }
    ndb2->next = gDbList;
    PathList = ndb2->FullPathList;
    gDbList = ndb2;
    *FileList = PathList;
    return Count;
}

void dbDelFileList( char **pList )
{
    xPath_t *list, *prev;

    for( prev = list = gDbList; list; list = list->next ){
        if( pList == list->FullPathList ){ // found list in catalog
    	    // unlink from catalog
    	    if( prev == gDbList )
        	gDbList = list->next;
    	    else
        	prev->next = list->next;
    	    // delete list
    	    xUnloadFileList( list );
    	    free( list );
    	    return;
        }
        prev = list;
    }
    dbNull1();
}

void dbNull1()
{
    /* Empty */
}

int dbFileLength( xFile_t *stream )
{
    return xfilelength_int( stream );
}

void dbSetRWFunc( void (*cb)(), unsigned int threshold )
{
    if( cb && threshold ){
        gdbRWfuncCb = cb;
    } else {
        threshold = 0;
        gdbRWfuncCb = NULL;
    }
    gdbRWfuncThres = threshold;
    dbNull2();
}

void dbNull2()
{
    /* Empty */
}

static int dbSortCb( const void *Str1, const void *Str2)
{
    return strcasecmp( *(char **)Str1, *(char **)Str2 );
}
