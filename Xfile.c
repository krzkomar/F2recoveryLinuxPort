#include "FrameWork.h"
#include <glob.h>


static xList_t *gxListDb = NULL;
static int xEnumCb( xFileSpec_t *filespec );
static void (*xGuardList[ 32 ])();
static int xListCount;

const char *gxFileErrors[16] = {
    "Error 0",				// 0
    "No such file or directory",	// 1
    "Argument list too big",		// 2
    "Exec format error",		// 3
    "Bad file number",			// 4
    "Not enough memory",		// 5
    "Permission denied",		// 6
    "File exists",			// 7
    "Cross-device link",		// 8
    "Invalid argument",			// 9
    "File table overflow",		// 10
    "Too many open files",		// 11
    "No space left on device",		// 12
    "Argument too large",		// 13
    "Result too large",			// 14
    "Resource deadlock would occur"	// 15
};

void xNull(){ /* Null function */ }
void (*xCleanList)() = xNull;

const char *xstrerror( int ErrNo )
{
    if ( ErrNo < 16 ) return gxFileErrors[ErrNo];
    return "unknown error";
}

void xclose_int( xFile_t *stream )
{
    assert( stream ); 		// assert_(0, "stream", 112, "xfile.c");
    switch( stream->SrcType ){
	case FILE_DB:  dclose( stream->dfile );  break;
	case FILE_ZIP: zclose( stream->zfile );  break;
	default: fclose( stream->rfile ); break;	
    }
    memset( stream, 0, sizeof( xFile_t ) );
    free( stream );
}

int xflush( xFile_t *stream )
{
    assert( stream );		//_assert_(0, "stream", 137, "xfile.c");
    switch( stream->SrcType ){
	case 0: return fflush( stream->rfile );
	case 1: return dflush( stream->dfile );
	case 2: return zflush( stream->zfile, 0 );
    }
    return fflush( stream->rfile );
}

xFile_t *xopen( const char *filename, const char *mode )
{
    xFile_t *stream;
    FILE *fh;
    xList_t *dblist;
    dFile_t *dbh;
    int m2, m1;
    char fname[ 256 ], dir[ 256 ];

    assert( filename );	//_assert_(0, "filename", 162, "xfile.c");
    assert( mode ); 		// _assert_(0, "mode", 163, "xfile.c");
    if( !( stream = malloc( sizeof( xFile_t ) ) ) ) return NULL;
    memset( stream, 0x00, sizeof( xFile_t ) );
    xSplitPath( filename, dir, NULL, NULL );
    if( dir[0] == '/' || dir[0] == '.' ){
        if( !( fh = fopen( filename, mode ) ) ){ free( stream ); return NULL; }
        stream->file = fh;
        stream->SrcType = FILE_RAW;
        sprintf( fname, "%s", filename );
    } else {
	for( dblist = gxListDb; dblist; dblist = dblist->next ){
	    if( (dblist->Flags & 1) == 0 ){ // true file
		sprintf( fname, "%s/%s", dblist->Path, filename );
		if( !(fh = fopen( fname, mode ) ) ) continue;
		stream->SrcType = FILE_RAW;
		stream->file = fh;
		break;
	    }
	    if( (dbh = dopen( dblist->Dbase, filename, mode ) ) ){
        	stream->file = dbh;
    		stream->SrcType = FILE_DB;
        	sprintf( fname, "%s", filename );
        	break;
	    }
	}
	if( !stream->file ){        
    	    if( !(fh = fopen( filename, mode ) ) ){ free( stream ); return NULL; }
	    stream->file = fh;
	    stream->SrcType = FILE_RAW;
	    sprintf( fname, "%s", filename );
        }
    }
    if( !stream->SrcType ){
        m1 = fgetc( stream->file );
        m2 = fgetc( stream->file );
        if( m1 == 0x1F && m2 == 0x8B ){ // gzip file signature
            fclose( stream->file );
            stream->SrcType = FILE_ZIP;
            stream->file = zOpenByFileName( fname, mode );
        } else {
            rewind( stream->file );
        }
    }
    return stream;
}

void xclose( const char *filename, const char *mode, xFile_t *stream )
{
    assert( filename );	// _assert_(0, "filename", 246, "xfile.c");
    assert( mode );		// _assert_(0, "mode", 247, "xfile.c");
    assert( stream );		// _assert_(0, "stream", 248, "xfile.c");
    xclose_int( stream );
}

void xsetbuf0( xFile_t *stream, void *buf )
{
    assert( stream );		// _assert_(0, "stream", 263, "xfile.c");
    assert( buf );		// _assert_(0, "buf", 264, "xfile.c");

    switch( stream->SrcType ){
	case 0: setbuf( stream->rfile, buf ); break;
	case 1: dsetbuf( stream->dfile, buf ); break;
    }    
}

void xsetbuf1( xFile_t *stream, char *bufffer, size_t size, int mode )
{
    assert( stream );		// _assert_(0, "stream", 284, "xfile.c");
    switch( stream->SrcType ){
	case 0: setvbuf( stream->rfile, bufffer, mode, size ); break;
	case 1: dunlock( stream->dfile ); break;
    }    
    setvbuf( stream->file, bufffer, mode, size );
}

int xprintf( xFile_t *stream, char *format, ... )
{
    va_list vl;
    va_start( vl, format );

    assert( format );		// _assert_(0, "format", 305, "xfile.c");
    return xvprintf( stream, format, vl );
    va_end( vl );
}

int xscanf( xFile_t *stream, char *format, ... )
{
    va_list vl;
    va_start( vl, format );

    assert( stream );		// _assert_(0, "stream", 317, "xfile.c");
    assert( format );		// _assert_(0, "format", 318, "xfile.c");
    return -1;
    va_end( vl );
}

int xvprintf( xFile_t *stream, const char *fmt, va_list vl )
{
    assert( stream );		//assert_(0, "stream", 332, "xfile.c");
    assert( fmt );		// _assert_(0, "format", 333, "xfile.c");
    switch( stream->SrcType){
	case 0: return vfprintf( stream->rfile, fmt, vl );
	case 1: return dvprintf( stream->dfile->Dbase, fmt, vl );
	case 2: return zvprintf( stream->zfile, fmt, vl );
    }
    return vfprintf( stream->file, fmt, vl );
}

int xgetc_int( xFile_t *stream )
{
    assert( stream );		// _assert_(0, "stream", 354, "xfile.c");

    switch( stream->SrcType ){
	case 0:return fgetc( stream->rfile );
	case 1:return dgetc( stream->dfile );
	case 2:return zgetc( stream->zfile );
    }
    return fgetc( stream->rfile );
}

char *xgets_int( char *str, int num, xFile_t *stream )
{
    assert( str );		// _assert_(0, "s", 375, "xfile.c");
    assert( num );		// _assert_(0, "n", 376, "xfile.c");
    assert( stream );		// _assert_(0, "stream", 377, "xfile.c");

    switch( stream->SrcType ){
	case 0: return fgets( str, num, stream->rfile );
	case 1: return dgets( str, num, stream->dfile );
	case 2: return zgets( str, num, stream->zfile );
    }
    return fgets(str, num, stream->file);
}

int xputc_int( int c, xFile_t *stream )
{
    assert( stream );		// _assert_(0, "stream", 399, "xfile.c");
    switch( stream->SrcType ){
	case 0: return fputc( c, stream->rfile );
	case 1: return dputc( c, stream->dfile );
	case 2: return zputc( stream->zfile, c );
    }    
    return fputc(c, stream->file);
}

int xputs_int( char *s, xFile_t *stream )
{
    assert( s );		//_assert_(0, "s", 421, "xfile.c");
    assert( stream );		//_assert_(0, "stream", 422, "xfile.c");
    switch( stream->SrcType ){
	case 0: return fputs( s, stream->rfile );
	case 1: return dputs( s, stream->dfile );
	case 2: return zputs( stream->zfile, s );
    }
    return fputs( s, stream->file );
}

int xread_int( void *buffer, unsigned int size, unsigned int count, xFile_t *stream )
{
    assert( buffer );		//_assert_(0, "ptr", 482, "xfile.c");
    assert( stream );		// _assert_(0, "stream", 483, "xfile.c");

    switch( stream->SrcType ){
	case 0: return fread( buffer, size, count, stream->rfile );
	case 1: return dread( buffer, size, count, stream->dfile );
	case 2: return zread( stream->zfile, buffer, count * size );
    }
    return fread( buffer, size, count, stream->rfile );
}

int xwrite_int( void *buffer, size_t size, size_t count, xFile_t *stream )
{
    assert( buffer );		// _assert_(0, "ptr", 504, "xfile.c");
    assert( stream );		// _assert_(0, "stream", 505, "xfile.c");
    switch( stream->SrcType ){
	case 0: return fwrite( buffer, size, count, stream->rfile );
	case 1: return dwrite( buffer, size, count, stream->dfile );
	case 2: return zputblock( stream->zfile, buffer, count * size );
    }
    return fwrite( buffer, size, count, stream->rfile );
}

int xseek_int( xFile_t *stream, int offset, int origin )
{
    assert( stream );		// _assert_(0, "stream", 547, "xfile.c");
    switch( stream->SrcType ){
	case 0: return fseek( stream->rfile, offset, origin );
	case 1: return dseek( stream->dfile, offset, origin );
	case 2: return zseek( stream->zfile, offset, origin );
    }
    return fseek( stream->rfile, offset, origin );
}

int xtell_int( xFile_t *stream )
{
    assert( stream );		//_assert_(0, "stream", 588, "xfile.c");
    switch( stream->SrcType ){
	case 0: return ftell( stream->rfile );
	case 1: return dtell( stream->dfile );
	case 2: return ztell( stream->zfile );
    }
    return ftell( stream->rfile );
}

void xrewind_int( xFile_t *stream )
{
    assert( stream );		// _assert_(0, "stream", 608, "xfile.c");
    switch( stream->SrcType ){
	case 0: rewind( stream->rfile ); return;
	case 1: drewind( stream->dfile );return;
	case 2: zrewind( stream->zfile );return;
    }
    rewind( stream->rfile );
}

void xDunk01( xFile_t *stream )
{
    assert( stream );	//_assert_(0, "stream", 628, "xfile.c");
    switch( stream->SrcType ){
//	case 0: stream->file->_flag &= ~0x30; return;
	case 1: d_unk01(stream->file); return;
	case 2: return;
    }
}

int xfeof_int( xFile_t *stream )
{
    assert( stream );		//_assert_(0, "stream", 648, "xfile.c");
    switch( stream->SrcType ){
	case 0: return feof( stream->rfile );
	case 1: return dfeof( stream->dfile );
	case 2: return zfeof( stream->zfile );
    }
    return feof( stream->rfile );
}

int filelength( FILE *f )
{
    int pos, size;

    pos = ftell( f );
    fseek( f, 0, SEEK_END );
    size = ftell( f );
    fseek( f, pos, SEEK_SET );
    return size;
}

int xerror( xFile_t *stream )
{
    assert( stream );		//assert_(0, "stream", 669, "xfile.c");
    switch( stream->SrcType ){
	case 0: return ferror( stream->file );
	case 1: return derror( stream->file );
	case 2: return *zerrorp() != NULL;
    }
    return ferror( stream->file );
}

int xfilelength_int( xFile_t *stream )
{
    assert( stream );		//_assert_(0, "stream", 690, "xfile.c");
    switch( stream->SrcType ){
	case 0: return filelength( stream->rfile );
	case 1: return dFilesTotal( stream->dfile );
	case 2: return 0;
    }
    return filelength( stream->rfile );
}

void xLoadFileList( char *PathList )
{
    xList_t *list, *tmp;
    char *fpath;

    list = gxListDb;
    gxListDb = NULL;
    while( list ){
        tmp = list->next;
        if( list->Flags & 0x01 ) dCloseArchive( list->Dbase );
        free( list->Path );
        free( list );
        list = tmp;
    }
    if( PathList ){
        for( fpath = strtok( PathList, ";" ); fpath; fpath = strtok( NULL, ";" ) ){
            if( !xOpenList( fpath ) ) break;
        }
    }
}

void xEnumerate( int (*enumfunc)( char *path ))
{
    xList_t *p;

    assert( enumfunc );	// _assert_(0, "enumfunc", 729, "xfile.c");
    for( p = gxListDb; p; p = p->next ){
        if( !(enumfunc( p->Path )) ) break;
    }
}

int xOpenList( const char *path )
{
    xList_t *p, *cur, *q, *new_el, *tmp;
    dFileBase_t *dbase;
    char cwd[ 280 ];
    static int Guard = 0;

    assert( path );		//_assert_(0, "path", 747, "xfile.c");
    if( !Guard ){
        xCleanAdd( xListDestroy );
        Guard = 1;
    }
    // przeszukaj baze aktualnie otwartych plikow
    for( p = gxListDb; p; p = p->next ){
    	if( !strcasecmp( p->Path, path ) ){ // plik znaleziono
	    cur = gxListDb;
	    if( p != gxListDb ){ // ustaw jako biezacy
		q = p->next;
    		gxListDb = p;
		cur->next = q;
		p->next = cur;      
	    }  
	    return 1;
    	}                                       //  znaleziono    	    
    }    
    // pliku nie znaleziono, otworz plik i dodaj do listy    
    if( !(new_el = malloc( sizeof( xList_t ) )) ) return 0;
    memset( new_el, 0x00, sizeof( xList_t ) );    
    if( !(new_el->Path = StrDup( (char *)path ) ) ){ free( new_el ); return 0; }
    if( (dbase = dOpenArchive( path )) ){ // otwórz plik bazy danych
        new_el->Flags = 1;
        new_el->Dbase = dbase;
        tmp = gxListDb;
        gxListDb = new_el;
        new_el->next = tmp;
        return 1;
    }
    // ** plik rzeczywisty **
    if( !getcwd( cwd, 260 ) ) return 0; // zachowaj aktualna sciezke katalogu
    // jesli sciezka nie istnije to ją stwórz
    if( xSetCurrentDir( path ) ){
	if( xCreatePath( path ) ) return 0;
    }
    xSetCurrentDir( cwd ); // odtworz sciezke
    tmp = gxListDb;
    gxListDb = new_el;
    new_el->next = tmp;
    return 1;    
}

int xRemFromList( const char *path )
{
    xList_t *p, *prev;

    assert( path );		//_assert_(0, "path", 813, "xfile.c");    
    for( prev = p = gxListDb; p ; prev = p, p = p->next ){
	if( !strcasecmp(p->Path, path) ){
	    if( prev == gxListDb )
    		gxListDb = p->next;
	    else
    		prev->next = p->next;
	    if( (p->Flags & 1) != 0 ) dCloseArchive( p->Dbase );
	    free( p->Path );
	    free( p );
	    return 1;
	}
    }
    return 0;    
}

//static int xReadDirInit( const char *name, xDir_t *pdir )
//{
//    pdir->dir = opendir( name );
//    if( !pdir->dir ) return 0;    
//    pdir->entry = readdir( pdir->dir );
//    if( !pdir->entry ){ closedir( pdir->dir ); return 0; }
//    return 1;        
//}

//static int xReadDir( xDir_t *d )
//{
//    d->entry = readdir( d->dir );
//    if( d->entry ) return 1;
//    closedir( d->dir );
//    return 0;
//}

//static int xDirClose( xDir_t *d )
//{
//    return closedir( d->dir ) == 0;
//}

//void xMakePath( char *path, char *dir, char *fname, char *extension )
//{
//    char *x = "", *sep, *ex;
//    
//    if( !dir ){ dir = x; sep = x; } else sep = "/";
//    if( !fname ) fname = x;
//    if( !extension ){ extension = x; ex = x; } else ex = ".";
//    sprintf( path, "%s%s%s%s%s", dir, sep, fname, ex, extension );
//}

int xMakeDirList( char *filespec, int (*enumfunc)(xFileSpec_t *), xPath_t *pList )
{
    glob_t GlobBuf = {0};
    char **pName, stmp[260],fname[256],fext[256],dir[256];
    int n;
    xList_t *list;
    dDirEnt_t Entry;
    xFileSpec_t FileSpec;

    assert( filespec );
    assert( enumfunc );

    FileSpec.List = pList;
    xSplitPath( filespec, dir, fname, fext );
    if( dir[0] == '/' || dir[0] == '.' ){
	if( glob( filespec, GLOB_MARK, NULL, &GlobBuf ) ) return 1;
	for( pName = GlobBuf.gl_pathv; *pName; pName++ ){
	    FileSpec.flags = XDB_DIRECTORY;
	    n = strlen( *pName );
	    if( n > 0 ){ 
		    if( (*pName)[n-1] == '/' ){ // directory
			(*pName)[n-1] = '\0';
			if( !strcmp( *pName, "." ) || !strcmp( *pName, ".." ) ) continue;
			FileSpec.flags = XDB_FILE;
		    } 
	    } 
	    strncpy( FileSpec.fpath, *pName, 259 );
    	    if( !enumfunc( &FileSpec ) ){ globfree( &GlobBuf ); return 1; }
	}
	globfree( &GlobBuf );
	return 0;
    }    
    for( list = gxListDb; list; list = list->next ){
        if( (list->Flags & 1) == 0 ){ // looking in real directories
            sprintf( stmp, "%s/%s", list->Path, filespec );
	    if( glob( stmp, GLOB_MARK, NULL, &GlobBuf ) ) continue;
	    for( pName = GlobBuf.gl_pathv; *pName; pName++ ){
		FileSpec.flags = XDB_DIRECTORY;
		n = strlen( *pName );
		if( n > 0 ){ 
		    if( (*pName)[n-1] == '/' ){ // directory
			(*pName)[n-1] = '\0';
			if( !strcmp( *pName, "." ) || !strcmp( *pName, ".." ) ) continue;
			FileSpec.flags = XDB_FILE;			
		    }		    
		} 
		strncpy( FileSpec.fpath, *pName, 259 );
        	if( !enumfunc( &FileSpec ) ){ globfree( &GlobBuf ); return 1; }
	    }
	    globfree( &GlobBuf );
        } else { // looking in database
    	    if( dGetFile( (dFileBase_t *)list->Dbase, &Entry, filespec ) ){
		FileSpec.flags = XDB_DBASE;
		do{
    		    strcpy( FileSpec.fpath, Entry.Entry );
    		    if( !enumfunc( &FileSpec ) ) return 1;
		}while( dReadDir( (dFileBase_t *)list->Dbase, &Entry ) );
    	    }
        }        
    }
    xSplitPath(filespec, dir, fname, fext);
    if( glob( filespec, GLOB_MARK, NULL, &GlobBuf ) ) return 1;
    for( pName = GlobBuf.gl_pathv; *pName; pName++ ){
	FileSpec.flags = XDB_DIRECTORY;
	n = strlen( *pName );
	if( n > 0 ){ 
	    if( (*pName)[n-1] == '/' ){ // directory
		(*pName)[n-1] = '\0';
		if( !strcmp( *pName, "." ) || !strcmp( *pName, ".." ) ) continue;
		FileSpec.flags = XDB_FILE;
	    }
	} 
	strncpy( FileSpec.fpath, *pName, 259 );
    	if( !enumfunc( &FileSpec ) ){ globfree( &GlobBuf ); return 1; }
    }
    globfree( &GlobBuf );
    return 0;
}

int xLoadPathList( char *filespec, xPath_t *list )
{
    xMakeDirList(filespec, xEnumCb, list);
    return list->nsize != -1;
}

void xUnloadFileList( xPath_t *list )
{
    unsigned int i;
    int idx;
    char **p;

    assert( list );		// _assert_(0, "list", 949, "xfile.c");
    i = 0;
    if( list->nsize ){
        idx = 0;
        do{
            p = &list->FullPathList[idx];
            if( *p ) free(*p);
            i++;
            idx++;
        }while( i < list->nsize );
    }
    free(list->FullPathList);
    memset(list, 0, 8u);
}

// tworzy fizyczna sciezke na dysku
int xCreatePath( const char *path )
{
    xList_t *p;
    char *s, c, cwd[ 260 ], stmp[ 512 ], dir[ 256 ];
    
    if( !getcwd( cwd, 260 ) ) return -1;
    xSplitPath( path, dir, NULL, NULL );
    if( dir[ 0 ] == '\\' || dir[ 0 ] == '/' || dir[ 0 ] == '.' ){
        strcpy( stmp, path );
    } else { // pobierz sciezke z listy
	for( p = gxListDb; p; p = p->next ){
            if( !(p->Flags & 1) ){
        	sprintf( stmp, "%s/%s", p->Path, path );
        	break;
            }            
        }
        if( !p ) sprintf( stmp, "%s/%s", cwd, path );
    }    
    if( stmp[ 0 ] == '\\' || stmp[ 0 ] == '/' ) s = &stmp[ 1 ]; // pomin znak podkatalogu
    for( s = stmp; *s; s++ ){
	if( *s == '\\' || *s == '/' ){
    	    c = *s;
    	    *s = '\0' ;
    	    if( xSetCurrentDir( stmp ) ){
        	if( xDirCreate( stmp ) ){ xSetCurrentDir( cwd ); return -1; }
    	    } else {
        	xSetCurrentDir( cwd );
    	    }
    	    *s = c;
	}	
    }
    xDirCreate( stmp );
    xSetCurrentDir( cwd );
    return 0;
}

int xRemovePath( const char *path )
{
    xList_t *list;
    char stmp[512], cwd[260], dir[256];

    if( !getcwd( cwd, 260 ) ) return -1;
    xSplitPath( path, dir, NULL, NULL );
    if( dir[ 0 ] == '\\' || dir[ 0 ] == '/' || dir[ 0 ] == '.' ){
        strcpy( stmp, path );
    } else {
	for( list = gxListDb; list; list = list->next ){
            if( !(gxListDb->Flags & 0x01) ){
        	sprintf( stmp, "%s/%s", gxListDb->Path, path );
        	break;
            }
        }
        if( !list ) sprintf( stmp, "%s/%s", cwd, path );
    }
    return rmdir( stmp ) != 0 ? errno : 0;
}

void xListDestroy()
{
    xList_t *p, *tmp;

    p = gxListDb;
    gxListDb = NULL;
    while( p ){
        tmp = p->next;
        if( p->Flags & 0x01 ) dCloseArchive( p->Dbase );
        free( p->Path );
        free( p );
        p = tmp;
    }
}

int xEnumCb( xFileSpec_t *filespec )
{
    xPath_t *list;
    char **newtab;

    list = filespec->List;
    if( filespec->flags & 0x01 ) return 1;
    newtab = realloc( list->FullPathList, (list->nsize + 1) * sizeof( char *));//  powiekszenie tablicy sciezek o 1
    if( newtab ){
	list->FullPathList = newtab;
	list->FullPathList[ list->nsize ] = StrDup( filespec->fpath );  // dopisanie na koncu tablicy nowego lancucha
	if( list->FullPathList[ list->nsize ] != NULL ){
	    list->nsize++;
	    return 1;
	}
    }
    xUnloadFileList( list );
    list->nsize = -1;
    return 0;
}

void xCleanListProcess()
{
    int i;

    i = xListCount - 1;
    if( xListCount != 33 ){
        xListCount = 33;
        for( ; i; i-- ) xGuardList[ i ]();
    }
}

int xCleanAdd( void ( *CleanFunc)( void ) )
{
    int x;

    x = xListCount;
    xCleanList = xCleanListProcess;
    if( xListCount >= 32 ) return -1;
    xGuardList[ xListCount ] = CleanFunc;
    xListCount = x + 1;
    return 0;
}

// *** extra func ***

void xSplitPath( const char *path, char *dir, char *fname, char *ext )
{
    const char *p, *ex;
    int n;
    
    if( path == NULL ) return;
    if( *path == '\0' ) return;
    if( dir ) dir[0] = '\0';
    if( fname ) fname[0] = '\0';
    if( ext ) ext[0] = '\0';
    n = strlen( path );
    if( n == 0 ) return;
    ex = path + n;
    for( p = ex; (p != path) && ( *p != '.') && ( *p != '/'); p-- );
    if( *p == '.' ){
	if( ext ) strcpy( ext, p );
	ex = p;
    }
    for( p = ex; (p != path) && ( *p != '/'); p-- );
    if( *p == '/' ){
	if( fname ){ strncpy( fname, p + 1, ex - p - 1 ); fname[ ex - p - 1 ] = '\0'; }
	if( dir ){ strncpy( dir, path, p - path ); dir[ p - path ] = '\0'; }
    } else {
	if( fname ){ strncpy( fname, path, ex - path ); fname[ ex - path ] = '\0'; }
    }    
}

int xSetCurrentDir( const char *dir ) // ret 0 ->ok, else errno
{
    return chdir( dir ) != 0 ? errno : 0;
}

int xDirCreate( const char *dir )
{
    return mkdir( dir, 0777 ) != 0 ? errno : 0;
}

int xFileRemove( const char *fname )
{
//    eprintf( "remove file: '%s'\n", fname );
    return unlink( fname );
}


/*
int xMakeDirList( char *filespec, int (*enumfunc)(xFileSpec_t *), xPath_t *pList )
{
    struct xList_t *list;
    short *psrc;
    xFileSpec_t *pdst;
    char c;
    dDirEnt_t SyncObj;
    xFileSpec_t FileSpec;
    char stmp[260];
    char fname[256];
    char fext[4];
    char dir[4];
    xDir_t sdir;
    char drive[4];

    assert( filespec );
    assert( enumfunc )

    FileSpec.List = pList;
    xSplitPath(filespec, drive, dir, fname, fext);
    if( drive[0] || dir[0] == '\\' || dir[0] == '/' || dir[0] == '.' ){
        if( !xReadDirInit(filespec, &sdir) ) return xDirClose( &sdir );
        do{
            FileSpec.flags = 0;
            if( ( sdir.entry->d_attr & 0x10 ) != 0 ){
            	if( !strcmp( sdir.entry->d_name, "." ) || !strcmp( sdir.entry->d_name, ".." ) ) continue;
        	FileSpec.flags = 1;
    	    }
    	    xMakePath( FileSpec.fpath, drive, dir, sdir.entry->d_name, 0 );
    	    if( !enumfunc(&FileSpec) ) return xDirClose( &sdir );
        }while( xReadDir( &sdir ) );
        return xDirClose( &sdir );                            
    }
    
    for( list = gxListDb; list; list = list->next ){
        if( (list->Flags & 1) == 0 ){
            sprintf( stmp, "%s/%s", list->Path, filespec );
            if( xReadDirInit( stmp, &sdir ) ){
        	do{
            	    FileSpec.flags = 0;
            	    if( (sdir.entry->d_attr & 0x10) != 0 ){
            		if( !strcmp( sdir.entry->d_name, ".")  || !strcmp( sdir.entry->d_name, ".." ) ) continue;
        		FileSpec.flags = 1;
        	    }
        	    xMakePath(FileSpec.fpath, drive, dir, sdir.entry->d_name, 0);
        	    if( !enumfunc(&FileSpec) ) return xDirClose( &sdir );
        	}while( xReadDir( &sdir ) );
            }
            xDirClose( &sdir );
        } else {
    	    if( dGetFile((dFileBase_t *)list->Dbase, &SyncObj, filespec) ){
		FileSpec.flags = 2;
		do{
    		    strcpy( FileSpec.fpath, SyncObj );
    		    if( !enumfunc( &FileSpec ) ) return 1;
		}while( dReadDir( (dFileBase_t *)list->Dbase, &SyncObj ) );
    	    }
        }        
    }
    xSplitPath(filespec, drive, dir, fname, fext);
    if( !xReadDirInit(filespec, &sdir) ) return xDirClose( &sdir );
    do{
        FileSpec.flags = 0;
        if( (sdir.entry->d_attr & 0x10) != 0 ){
            if( !strcmp( sdir.entry->d_name, "." ) || !strcmp( sdir.entry->d_name, ".." ) ) continue;
            FileSpec.flags = 1;
        }
        xMakePath( FileSpec.fpath, drive, dir, sdir.entry->d_name, 0 );
        if( !enumfunc( &FileSpec ) ) return xDirClose( &sdir );
    } while( xReadDir( &sdir ) );
    return xDirClose( &sdir );
}

*/