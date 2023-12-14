#include "FrameWork.h"
#include <fnmatch.h>

int filelength(FILE *);

int dCmp( char *path, char *entry )
{
    return !fnmatch( path, entry, FNM_PATHNAME );
}

int dPathValidate( char *p )
{
    int i, tmp;
    char c;

    if( !p ) return 0;
    for( i = 0; (c = p[i]); i++ ){
        if( c < '[' ){
            if( c == '!' ){
        	i++;
        	if( !p[ i ] ) return 0;
            }
        } else if( c == '[' ){
    	    i++;
            if( p[ i ] == '!' ) i++;
	    do{
                if( p[ i ] == ']' ) return 1;
                if( p[ i ] == '`' ) i++;
                if( p[ i ] == 0 ) break;
                i++;
                if( p[ i ] == '-' ){
                    c = p[i + 1];
                    tmp = i + 1;
                    if( c == '`' ) tmp++;
                    if( !p[ tmp ] ) break;
                    i = tmp + 1;
                }
            }while( p[ i ] );
            return 0;
        }
        if( c == '`' && !p[ ++i ] ) return 0;
    }
    return 1;    
}

int dReadDirEntry( char *path, char *entry )
{
    if( !entry || !path ) return 0;
    if( !dPathValidate( path ) ) return 0;
    if( *entry ) return dCmp( path, entry );
    return (*path == '\0');
}

dFileBase_t *dOpenArchive( const char *filename )
{
    FILE *fh;
    dFileBase_t *dbase;
    int i, flen, nmlen;
    unsigned int size, ArchSize;
    char *p;
    
    assert( filename );	// _assert_(0, "filename", 74, "dfile.c");
    if( !(fh = fopen( filename, "rb" ) ) ) return NULL;
    if( !(dbase = malloc( sizeof( dFileBase_t ) ) ) ) goto err;
    memset( dbase, 0x00, sizeof( dFileBase_t ) );
    flen = filelength( fh );
    if( fseek( fh, flen - 8, SEEK_SET )) goto err;
    if( fread( &size, 4, 1, fh ) != 1 ) goto err; 		// TreeSize
    if( fread( &ArchSize, 4, 1, fh) != 1 ) goto err;		// DataSize
    if( fseek( fh, flen - size - 8, SEEK_SET ) ) goto err;	// jump to beginning of DirTree
    if( fread( &dbase->FilesTot, 4, 1, fh ) != 1 ) goto err; 	// FilesTotal
    if( !(dbase->DirTree = malloc( dbase->FilesTot * sizeof( dVfile_t ) )) ) goto err;
    memset( dbase->DirTree, 0x00,  dbase->FilesTot * sizeof( dVfile_t ) );
    for( i = 0; i < dbase->FilesTot; i++ ){
	// nazwa pliku
        if( fread( &nmlen, 4, 1, fh ) != 1 ) break; 		// FileNameSize         	
        if( !(dbase->DirTree[ i ].FileName = malloc( nmlen + 1 )) ) break; // dlugosc nazwy pliku + '\0'        
        if( fread( dbase->DirTree[ i ].FileName, nmlen, 1, fh) != 1 ) break; // Filename
        dbase->DirTree[ i ].FileName[ nmlen ] = '\0';
        // change '\\' to '/' - added
        for( p = dbase->DirTree[ i ].FileName; (p = strchr( p, '\\')); p++ ) *p = '/';
        if( fread( &dbase->DirTree[ i ].Compressed, 1, 1, fh ) != 1 ) break; // Type 1 - compressed / 0 - decompressed
        if( fread( &dbase->DirTree[ i ].RealSize,   4, 1, fh ) != 1 ) break; // Real file Size
        if( fread( &dbase->DirTree[ i ].PackedSize, 4, 1, fh ) != 1 ) break; // Packed file Size
        if( fread( &dbase->DirTree[ i ].Location,   4, 1, fh ) != 1 ) break; // Offset
    }
    if( i < dbase->FilesTot ) goto err;
    dbase->ArchName = StrDup( (char *)filename );
    dbase->TreeSize = flen - ArchSize;
    fclose( fh );
    return dbase;        
err:
    dCloseArchive( dbase );
    fclose( fh );
    return NULL;
}

int dCloseArchive( dFileBase_t *dbase )
{
    dFile_t *p;
    unsigned int i;
    char *str;

    assert( dbase );	// _assert_(0, "dbase", 173, "dfile.c");
    for( p = dbase->OpenedList; p; p = p->next ) dclose( p );
    if( dbase->DirTree ){
        for(i = 0; i < dbase->FilesTot; i++  ){
            str = dbase->DirTree[ i ].FileName;
            if( str ) free( str );
        }
        free( dbase->DirTree );
    }
    if( dbase->ArchName ) free( dbase->ArchName );
    memset( dbase, 0x00, sizeof( dFileBase_t ) );
    free( dbase );
    return 1;
}

int dGetFile( dFileBase_t *dbase, dDirEnt_t *obj, char *path )
{
    int i;

    if( dbase->FilesTot == 0 ) return 0;
    for( i = 0; i < dbase->FilesTot; i++ ){
	if( dReadDirEntry( path, dbase->DirTree[ i ].FileName ) ){
	    strcpy( obj->Entry, dbase->DirTree[ i ].FileName );
	    strcpy( obj->Path, path );
	    obj->i520 = i;
	    return 1;
	}
    }
    return 0;
}

int dReadDir( dFileBase_t *db, dDirEnt_t *de)
{
    int i;

    for( i = de->i520 + 1; i < db->FilesTot; i++ ){
	if( !dReadDirEntry( de->Path, db->DirTree[ i ].FileName ) ){
	    strcpy( de->Entry, db->DirTree[ i ].FileName );
	    de->i520 = i;
	    return 1;
	}
    }
    return 0;
}

unsigned int dFilesTotal( dFile_t *stream )
{
    return stream->Vfile->RealSize; 
}

int dclose( dFile_t *stream )
{
    dFile_t *t, *prev;
    int err = 0;

    assert( stream );	// _assert_(0, "stream", 253, "dfile.c");
    if( (stream->Vfile->Compressed & 0x01) && inflateEnd( stream->pData ) ) err = -1;
    if( stream->pData ) free( stream->pData );
    if( stream->buff1024 ) free( stream->buff1024 );
    if( stream->fhandler ) fclose( stream->fhandler );
    
    for( prev = t = stream->Dbase->OpenedList; t; t = t->next ){
        if( t == stream ){ //  odszukanie dowiazania do odcinanej galezi
    	    if( prev == stream->Dbase->OpenedList )
        	stream->Dbase->OpenedList = stream->next; // zmiana dowiazania pomijajaca odcinany fragment
    	    else
        	prev->next = stream->next;
    	    break;
        }
        prev = t;
    }
    memset( stream, 0x00, sizeof( dFile_t ) );  // zwolnienie obszaru
    free( stream );
    return err;
}

int dflush( dFile_t *stream )
{
    assert( stream );	//_assert_(0, "stream", 286, "dfile.c");
    return 0;
}

dFile_t *dopen( dFileBase_t *dbase, const char *fname, const char *mode )
{
    assert( dbase );		//_assert_(0, "dbase", 295, "dfile.c");
    assert( fname );		// _assert_(0, "filename", 296, "dfile.c");
    assert( mode );		// _assert_(0, "mode", 297, "dfile.c");
    return dopen_int( dbase, fname, mode, NULL );
}

dFile_t *dopenstrm( const char *filename, const char *mode, dFile_t *stream )
{
    assert( filename );	// _assert_(0, "filename", 306, "dfile.c");
    assert( mode ); 		//_assert_(0, "mode", 307, "dfile.c");
    assert( stream ); 		//_assert_(0, "stream", 308, "dfile.c");
    return dopen_int( stream->Dbase, filename, mode, stream );
}

void dsetbuf( dFile_t *stream, void *buf)
{
    assert( stream );		// _assert_( 0, "stream", 318, "dfile.c" );
    assert( buf );		//_assert_( 0, "buf", 319, "dfile.c" );
}

int dunlock( dFile_t *stream )
{
    assert( stream );		// _assert_(0, "stream", 329, "dfile.c");
    return 1;			//CSyncObject::Unlock();
}

int dvprintf( dFileBase_t *stream, const char *format, va_list vl)
{
    assert( stream );		//_assert_(0, "stream", 368, "dfile.c");
    assert( format );		// _assert_(0, "format", 369, "dfile.c");
    return -1;
}

int dgetc( dFile_t *stream )
{
    int data;

    assert( stream );		// _assert_(0, "stream", 384, "dfile.c");
    if( (stream->Flags & 0x02) || (stream->Flags & 0x04 ) ) return -1;
    if( stream->Flags & 0x01 ){
        data = stream->FirstChr;
        stream->Flags &= ~0x01; // unget flag
    } else {
        data = dReadChar( stream );
        if( data == -1 ) stream->Flags |= 0x02;// eof ? 
    }
    return data;
}

char *dgets( char *s, int n, dFile_t *stream )
{
    int Char, i;
    char *p;

    p = s;
    assert( s );		// _assert_(0, "s", 407, "dfile.c");
    assert( n );		// _assert_(0, "n", 408, "dfile.c");
    assert( stream );		// _assert_(0, "stream", 409, "dfile.c");
    if( (stream->Flags & 0x02) || (stream->Flags & 0x04) ) return NULL;
    if( stream->Flags & 0x01 ){
        stream->Flags &= ~0x01;
        *p++ = stream->FirstChr;
        n--;
    }
    if( n - 1 > 0 ){
        for( i = 0; i < n; i++ ){
            Char = dReadChar( stream );
            if( Char == -1 ) break;
            *p++ = Char;
            if( Char == '\n' ) break;
        }
    }
    if( p == s ) return NULL;
    *p = '\0';
    return s;
}

int dputc( int c, dFile_t *stream )
{
    assert( stream );	// _assert_( 0, "stream", 437, "dfile.c" );
    return -1;
}

int dputs( char *s, dFile_t *stream )
{
    assert( s );	// _assert_(0, "s", 448, "dfile.c");
    assert( stream );	// _assert_(0, "stream", 449, "dfile.c");
    return -1;
}

int dungetc( int chr, dFile_t *stream )
{
    assert( stream );	// _assert_(0, "stream", 480, "dfile.c");
    if( chr == -1 ) return -1;
    if( stream->Flags & 0x01 ) return -1;
    stream->Flags |= 0x01;
    stream->FirstChr = chr;
    stream->Flags &= ~0x02;
    return stream->FirstChr;
}

int dread( void *ptr, unsigned int blksize, int blknum, dFile_t *stream )
{
    unsigned int avail, n, count, UngetByte;
    char *p = ptr;

    UngetByte = 0;
    assert( ptr );		//_assert_(0, "ptr", 499, "dfile.c");
    assert( stream );		//_assert_(0, "stream", 500, "dfile.c");
    if( (stream->Flags & 0x02) || (stream->Flags & 0x04) ) return 0;
    avail = stream->Vfile->RealSize - stream->CurPos;
    if( stream->Flags & 0x01 ) avail++;
    n = blknum * blksize;
    if( avail < blknum * blksize ){
        n = avail;
        stream->Flags |= 0x02;
    }
    if( stream->Flags & 0x01 ){
        *p++ = stream->FirstChr;
        stream->Flags &= ~0x01;
        n--;
        UngetByte = 1;
    }
    if( stream->Vfile->Compressed & 0x01 ){
        if( !dReadCompressed( stream, ptr, n ) ){
            stream->Flags |= 0x04;
            return 0;
        }
        count = n;
    } else {
        count = fread( p, 1, n, stream->fhandler ) + UngetByte;
        stream->CurPos += count;
    }
    return count / blksize;
}

int dwrite( void *buffer, size_t size, size_t count, dFile_t *stream )
{
    assert( buffer );	// _assert_(0, "ptr", 538, "dfile.c");
    assert( stream );	// _assert_(0, "stream", 539, "dfile.c");
    return count - 1;
}

int dtellp( dFile_t *stream, int *pos )
{
    assert( pos );	// _assert_(0, "pos", 555, "dfile.c");
    *pos = dtell( stream );
    return (*pos == -1);
}

int dseek( dFile_t *stream, int offset, int whence )
{
    dVfile_t *dbase;
    char Flags;
    int err;
    unsigned int new_pos, tmp, curpos;

    assert( stream );		// _assert_(0, "stream", 569, "dfile.c");
    Flags = stream->Flags;
    if( Flags & 0x04 ) return 1;
    if( (Flags & 0x08) && offset && whence ) return 1;
    if( whence != 0){
        if( whence == 1 ){
            tmp = stream->CurPos;
        } else {
            if( whence != 2 ) return 1;
            tmp = stream->Vfile->RealSize;
        }
        new_pos = offset + tmp;
    } else {
        new_pos = offset;
    }
    dbase = stream->Vfile;
    if( new_pos >= dbase->RealSize ) return 1;
    curpos = stream->CurPos;
    if( new_pos == curpos ){ stream->Flags &= ~0x03; return 0; }
    if( new_pos ){
        if( dbase->Compressed & 0x01 ){
            if( new_pos < curpos ) drewind( stream );
            while( new_pos > stream->CurPos ){
                if( dReadChar( stream ) == -1 ) return 1;
            }
        } else {
            if( fseek( stream->fhandler, new_pos - curpos, 1 ) ){
                stream->Flags |= 0x04;
                return 1;
            }
            stream->DataPos = new_pos;
        }
        stream->Flags &= ~0x03;
        return 0;
    }
    if( fseek( stream->fhandler, stream->Dbase->TreeSize + dbase->Location, 0 ) ){
        err = 1;
        stream->Flags |= 0x04;
    } else if( inflateEnd( stream->pData ) ){
        err = 1;
        stream->Flags |= 0x04;
    } else {
        stream->pData->zalloc = Z_NULL;
        stream->pData->zfree = Z_NULL;
        stream->pData->opaque = Z_NULL;
        stream->pData->next_in = (Bytef *)stream->buff1024;
        stream->pData->avail_in = 0;
        if( inflateInit( stream->pData ) ){
            err = 1;
            stream->Flags |= 0x04;
        } else {
            stream->CurPos = 0;
            stream->DataPos = 0;
            stream->Flags &= ~0x03;
            return 0;
        }
    }
    return err;
}

int dseekset( dFile_t *stream, unsigned int *pos )
{
    assert( pos );	// _assert_(0, "pos", 642, "dfile.c");
    return (dseek( stream, *pos, 0 ) != 0);
}

int dtell( dFile_t *stream )
{
    assert( stream );	// _assert_(0, "stream", 654, "dfile.c");
    return stream->CurPos;
}

int drewind( dFile_t *stream )
{
    int err;

    assert( stream ); 	// _assert_(0, "stream", 664, "dfile.c");
    err = dseek( stream, 0, 0 );
    stream->Flags &= ~0x04;
    return err;
}

void d_unk01( dFile_t *stream ) //!!
{
    assert( stream );	//_assert_(0, "stream", 675, "dfile.c");
    stream->Flags &= ~0x06;
}

int dfeof( dFile_t *stream )
{
    assert( stream );	// _assert_(0, "stream", 685, "dfile.c");
    return (stream->Flags & 0x02);
}

int derror( dFile_t *stream )
{
    assert( stream );	// _assert_(0, "stream", 695, "dfile.c");
    return (stream->Flags & 0x04);
}

static int dbCmpCb( const char *str, dVfile_t *entry )
{
    return strcasecmp( str, entry->FileName );
}

void *dLookingUpFile( dFileBase_t *dbase, const char *path )
{
    return bsearch(path, dbase->DirTree, dbase->FilesTot, sizeof( dVfile_t ), (void *)dbCmpCb );
}

dFile_t * dopen_int( dFileBase_t *dbase, const char *fname, const char *mode, dFile_t *NewNode )
{
    dVfile_t *dtree;
    dFile_t *new_node;
    z_streamp r;

    new_node = NewNode;
    dtree = bsearch( fname, dbase->DirTree, dbase->FilesTot, sizeof( dVfile_t ), (void *)dbCmpCb );
    if( !dtree ){ // No file in database
        if( NewNode ) dclose( NewNode );
        return NULL;
    }
    if( *mode != 'r' ){ // Write not supported
        if( NewNode ) dclose( NewNode );
        return NULL;
    }
    if( NewNode ){
        if( dbase != NewNode->Dbase ){
            dclose( NewNode );
            return NULL;
        }
        if( NewNode->fhandler ){
            fclose( NewNode->fhandler );
            NewNode->fhandler = NULL;
        }
        new_node->DataPos = 0;
        new_node->CurPos = 0;
        new_node->Flags = 0;
    } else { // make new node
        if( !(new_node = malloc( sizeof( dFile_t ) )) ) return NULL;
        memset( new_node, 0x00, sizeof( dFile_t ) );//  zeruj nowa strukture
        new_node->Dbase = dbase;
        new_node->next = dbase->OpenedList;
        dbase->OpenedList = new_node;
    }
    new_node->Vfile = dtree;
    new_node->fhandler = fopen( dbase->ArchName, "rb" );
    if( !new_node->fhandler ) goto err;
    if( fseek( new_node->fhandler, dtree->Location + dbase->TreeSize, SEEK_SET ) ) goto err; // ustawia skaznik pliku na plik wirtualny
    if( dtree->Compressed & 0x01 ){ // Compressed file
        if( !new_node->pData ) {
            if( !( new_node->pData = malloc( sizeof( z_stream ) ) ) ) goto err;
            if( !( new_node->buff1024 = malloc( 1024 )) ) goto err;
        }
        new_node->pData->zalloc = Z_NULL;
        new_node->pData->zfree = Z_NULL;
        new_node->pData->opaque = Z_NULL;
        new_node->pData->next_in = (Bytef *)new_node->buff1024;
        new_node->pData->avail_in = 0;
        if( inflateInit( new_node->pData ) ) goto err;
    } else { // decompressed file
        if( (r = new_node->pData) ) {
            free( r );
            new_node->pData = NULL;
        }
        if( new_node->buff1024 ){
            free( new_node->buff1024 );
            new_node->buff1024 = NULL;
        }
    }
    if( mode[ 1 ] == 't' ) new_node->Flags |= 0x08;
    return new_node;
err:
    dclose( new_node );
    return 0;
}

int dReadChar( dFile_t *stream )
{
    unsigned char chr, c;
    dVfile_t *vfile;
    
    vfile = stream->Vfile;
    if( vfile->Compressed & 0x01 ){
        if( !dReadCompressed( stream, (char *)&chr, 1 ) ) return -1;
        if( (stream->Flags & 0x08) && (chr == '\r') && dReadCompressed( stream, (char *)&c, 1 ) ){
            if( c == '\n' ){
                chr = '\n';
            } else {
                stream->UngetChr = c;
                stream->Flags |= 0x10;
                stream->CurPos--;
            }
        }
        return chr;
    }
    if( stream->CurPos >= vfile->RealSize ) return -1;
    if( (chr = fgetc( stream->fhandler ) ) == -1 ){ DD return -1;}
    if( (stream->Flags & 0x08 ) && (chr == '\r') && ( (stream->CurPos + 1) < stream->Vfile->RealSize ) ){
	c = fgetc( stream->fhandler );
        if( c == '\n' ){
            chr = '\n';
            stream->CurPos++;
        } else {
            ungetc( c, stream->fhandler );
        }
    }
    stream->CurPos++;
    return chr;
}

int dReadCompressed( dFile_t *stream, char *buffer, size_t size )
{
    unsigned int BlkSize;

    if( stream->Flags & 0x10 ){
        *buffer++ = stream->UngetChr;
        stream->Flags &= ~0x10;
        stream->CurPos++;
        size--;
        if( !size ) return 1;
    }

    stream->pData->next_out = (Bytef *)buffer;
    stream->pData->avail_out = size;
    do{
        if( stream->pData->avail_out == 0 ) break;
        if( stream->pData->avail_in == 0 ){ // bufor oprozniony, rozkopresuj kolejna partie danych
            BlkSize = stream->Vfile->PackedSize - stream->DataPos;
            if( BlkSize > 1024 ) BlkSize = 1024;
            if( fread( stream->buff1024, BlkSize, 1, stream->fhandler ) != 1 ) break;
            stream->pData->avail_in = BlkSize;
            stream->pData->next_in = (Bytef *)stream->buff1024;
            stream->DataPos += BlkSize;
        }
    }while( !inflate( stream->pData, 0 ) );
    if( stream->pData->avail_out ) return 0; // EOF
    stream->CurPos += size;
    return 1;
}

