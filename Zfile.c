#include "FrameWork.h"

#define ALGO_DEFLATE	0x08
#define FTEXT		0x01
#define FHCRC		0x02
#define FEXTRA		0x04
#define FNAME		0x08
#define FCOMMENT	0x10

static unsigned char zMagic[ 2 ] = { 0x1f, 0x8b }; // GZIP magic
static char *Zerror = NULL;

unsigned int zSumTable[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3, 
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5, 
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59, 
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

static unsigned int zSum( int in, unsigned char *pdata, unsigned int nsize )
{
    unsigned char i;
    unsigned char a;
    char tmp_2;
    unsigned char *p;
    int h;
    int tmp_1;

    if( !pdata ) return 0;
    for( i = ~in; nsize >= 8; i = h ^ (a >> 8) ){
        a = zSumTable[(i ^ *pdata)] ^ (i >> 8);
        a = zSumTable[(a ^ pdata[1])] ^ (a >> 8);
        a = zSumTable[(a ^ pdata[2])] ^ (a >> 8);
        a = zSumTable[(a ^ pdata[3])] ^ (a >> 8);
        tmp_2 = pdata[4];
        p = pdata + 1;
        nsize -= 8;
        a = zSumTable[(a ^ tmp_2)] ^ (a >> 8);
        tmp_2 = p[4];
        p += 2;
        a = zSumTable[(a ^ tmp_2)] ^ (a >> 8);
        tmp_2 = p[3];
        p += 2;
        a = zSumTable[(a ^ tmp_2)] ^ (a >> 8);
        h = zSumTable[(a ^ p[2])];
        pdata = p + 3;
    }
    for( ; nsize; nsize-- ){
        tmp_1 = zSumTable[ (i ^ *pdata++) ];
        i = tmp_1 ^ (i >> 8);
    }
    return ~i;
}

zFile_t *zOpenInt( const char *fname, const char *mode, int dbaseId )
{
    unsigned int CompLvl;
    unsigned int CompStrategy;
    char smode[80];
    zFile_t *fh;
    char *s;

    CompStrategy = Z_DEFAULT_STRATEGY;
    CompLvl = Z_DEFAULT_COMPRESSION;
    if( !fname || !mode ) return NULL;
    if( !(fh = malloc( sizeof( zFile_t ) )) ) return NULL;
    fh->zip.zalloc = Z_NULL;
    fh->zip.zfree = Z_NULL;
    fh->zip.opaque = Z_NULL;
    fh->pRdBuffRoot = Z_NULL;
    fh->zip.next_in = Z_NULL;
    fh->pWrBuffRoot = NULL;
    fh->zip.next_out = Z_NULL;
    fh->zip.avail_out = 0;
    fh->zip.avail_in = 0;
    fh->file = NULL;
    fh->Error = 0;
    fh->eof = 0;
    fh->crc = zSum( 0, 0, 0 );
    fh->p20 = NULL;
    fh->RealFileFlag = 0;
    if( !(fh->name = malloc( strlen( fname ) + 1 )) ){ zFileClose( fh ); return NULL; }
    strcpy( fh->name, fname );
    // parse mode line
    s = smode;
    fh->mode = 0;
    do{
        switch( *mode ){
    	    case 'r': fh->mode = 'r'; break;
    	    case 'w': case 'a': fh->mode = 'w'; break;
    	    case '0' ... '9': CompLvl = *mode - '0'; continue;
    	    case 'f': CompStrategy = Z_FILTERED; continue;
    	    case 'h': CompStrategy = Z_HUFFMAN_ONLY; continue;
        }                
        *s++ = *mode;
    }while( *mode++ && s != &s[ 79 ] );
    // initialize compressor/decompessor
    if( fh->mode == 0 ){ zFileClose( fh ); return NULL; }
    if( fh->mode == 'w' ){
        fh->zip.next_out = fh->pWrBuffRoot = malloc( 0x4000 );
        if( deflateInit2( &fh->zip, CompLvl, Z_DEFLATED, 15, 8, CompStrategy ) || !fh->pWrBuffRoot ){ zFileClose( fh ); return NULL; }
    } else {
        fh->zip.next_in = fh->pRdBuffRoot = malloc( 0x4000 );
        if( inflateInit2( &fh->zip, -MAX_WBITS ) || !fh->pRdBuffRoot ){ zFileClose( fh ); return NULL; }
    }
    // open file
    fh->zip.avail_out = 0x4000;
    *zerrorp() = NULL;
    if( dbaseId < 0 ) // by filename in real fs
        fh->file = fopen( fname, smode );
    else // by file descriptor number
        fh->file = fdopen( dbaseId, smode );
    if( !fh->file ){ zFileClose( fh ); return NULL; }
    if( fh->mode == 'w' ) {
	// write a header
        fprintf( fh->file, "%c%c%c%c%c%c%c%c%c%c", zMagic[0], zMagic[1], Z_DEFLATED, GZ_FILE_FLAGS, GZ_TIMESTUMP, GZ_COMP_FLAGS, GZ_SYSID );
        fh->pos = 10;
    } else {
	// read a header
        zReadHdr( fh );
        fh->pos = ftell( fh->file ) - fh->zip.avail_in;
    }    
    return fh;
}

zFile_t *zOpenByFileName( const char *fname, const char *mode )
{

    return zOpenInt( fname, mode, -1 );
}

zFile_t *zOpenByDbaseId( int dbaseId, const char *mode )
{
    char fname[ 24 ];

    if( dbaseId < 0 ) return 0;
    sprintf( fname, "<fd:%d>", dbaseId );
    return zOpenInt( fname, mode, dbaseId );
}

int zFlush_( zFile_t *stream, int CompLvl, int CompStrategy )
{
    if( !stream || stream->mode != 'w' ) return -2;
    if( !stream->zip.avail_out ){
        stream->zip.next_out = stream->pWrBuffRoot;
        if( fwrite( stream->zip.next_out, 1, 0x4000, stream->file ) != 0x4000 ) stream->Error = -1;
        stream->zip.avail_out = 0x4000;
    }
    return deflateParams( &stream->zip, CompLvl, CompStrategy );
}

int zGetInt8( zFile_t *stream )
{
    Bytef *k;

    if( stream->eof ) return -1;
    if( !stream->zip.avail_in ){
	*zerrorp() = NULL;
	stream->zip.avail_in = fread(stream->pRdBuffRoot, 1, 0x4000, stream->file);
	if( !stream->zip.avail_in ){
	    stream->eof = 1;
	    if( ferror( stream->file ) ) stream->Error = -1;
	    return -1;
	}
	stream->zip.next_in = stream->pRdBuffRoot;
    }
    k = stream->zip.next_in;
    stream->zip.next_in++;
    stream->zip.avail_in--;
    return *k;
}

void zReadHdr( zFile_t *stream )
{
    unsigned int i,j,k;
    int n;
    unsigned char Flags, chr;

    for( i = 0; i < 2; i++ ){
        chr = zGetInt8( stream );
        if( chr != zMagic[ i ] ){
            if( i ){
                stream->zip.next_in--;
                stream->zip.avail_in++;
            }
            if( chr != -1 ){
                stream->RealFileFlag = 1;
                stream->zip.next_in--;
                stream->zip.avail_in++;
            }
            stream->Error = stream->zip.avail_in == 0;
            return;
        }
    }
    chr = zGetInt8( stream );
    Flags = zGetInt8( stream );
    if( (chr != ALGO_DEFLATE) || ((Flags & 0xE0) != 0 ) ){ stream->Error = -3; return; }
    for( j = 0; j < 6; j++ ) zGetInt8( stream );
    if( Flags & FEXTRA ){
        n = zGetInt8( stream ) + 256 * zGetInt8( stream );
        while( (--n != -1) && (zGetInt8( stream ) != -1) );
    }
    if( Flags & FNAME ){
        do chr = zGetInt8( stream );  while ( chr && chr != -1 );
    }
    if( Flags & FCOMMENT ){
        do chr = zGetInt8( stream ); while ( chr && chr != -1 );
    }
    if( Flags & FHCRC ){
        for ( k = 0; k < 2; k++ ) zGetInt8( stream ); 
    }
    if( stream->eof ){ stream->Error = -3; return; }
    stream->Error = 0;
}

int zFileClose( zFile_t *zfile )
{
    int err;

    err = 0;
    if( !zfile ) return -2;
    if( zfile->p20 ) free( zfile->p20 );
    if( zfile->zip.state ){
        if( zfile->mode == 'w' ){
            err = deflateEnd( &zfile->zip );
        } else {
            if( zfile->mode == 'r' ) err = inflateEnd( &zfile->zip );
        }
    }
    if( zfile->file && fclose( zfile->file ) && **zerrorp() != '\x1f' ) err = -1;
    if( zfile->Error < 0 ) err = zfile->Error;
    if( zfile->pRdBuffRoot ) free( zfile->pRdBuffRoot );
    if( zfile->pWrBuffRoot ) free( zfile->pWrBuffRoot );
    if( zfile->name ) free( zfile->name );
    free( zfile );
    return err;
}

int zread( zFile_t *stream, char *buff, int nsize )
{
    int n, k;
    unsigned char *next_out;
  
    next_out = (unsigned char *)buff;
    if( !stream || stream->mode != 'r' ) return -2;
    if( stream->Error == -3 || stream->Error == -1 ) return -1;
    if( stream->Error == 1 ) return 0;
    stream->zip.next_out = (Bytef*)buff;
    stream->zip.avail_out = nsize;
    if( nsize == 0 ){
	stream->crc = zSum(stream->crc, next_out, (void *)stream->zip.next_out - (void*)next_out);
	return nsize - stream->zip.avail_out;
    }
    while( !stream->RealFileFlag ){
        if( (stream->zip.avail_in == 0) && !stream->eof ){ // get next portion of data
            *zerrorp() = NULL;
            stream->zip.avail_in = fread( stream->pRdBuffRoot, 1u, 0x4000u, stream->file );
            if( !stream->zip.avail_in ){
                stream->eof = 1;
                if( ferror( stream->file ) ) { 
                    stream->Error = Z_ERRNO;
		    stream->crc = zSum(stream->crc, next_out, (void *)stream->zip.next_out - (void*)next_out);
		    return nsize - stream->zip.avail_out;
                }
            }
            stream->zip.next_in = stream->pRdBuffRoot;
        }

        stream->Error = inflate( &stream->zip, 0 );
        if( stream->Error == Z_STREAM_END ){
            stream->crc = zSum( stream->crc, next_out, (void *)stream->zip.next_out - (void *)next_out );
            next_out = (unsigned char *)stream->zip.next_out;
            if( zGetInt32( stream ) == stream->crc && zGetInt32( stream ) == stream->zip.total_out ){
                zReadHdr( stream );
                if( !stream->Error ){
                    k = stream->zip.total_out;
                    n = stream->zip.total_in;
                    inflateReset( &stream->zip );
                    stream->zip.total_out = k;
                    stream->zip.total_in = n;
                    stream->crc = zSum(0, 0, 0);
                }
            } else {
                stream->Error = Z_DATA_ERROR;
            }
        }
        if( stream->Error || stream->eof || !stream->zip.avail_out ){
	    stream->crc = zSum(stream->crc, next_out, (void *)stream->zip.next_out - (void*)next_out);
	    return nsize - stream->zip.avail_out;
        }
    }
    n = stream->zip.avail_in;
    if( n > stream->zip.avail_out ) n = stream->zip.avail_out;
    if( n ){
        memcpy(stream->zip.next_out, stream->zip.next_in, n);
        stream->zip.next_out = (Bytef *)&buff[ n ];
        stream->zip.next_in = &stream->zip.next_in[ n ];
        stream->zip.avail_out = stream->zip.avail_out - n;
        buff += stream->zip.avail_in;
        stream->zip.avail_in -= n;
    }
    if( stream->zip.avail_out ) stream->zip.avail_out -= fread( buff, 1, stream->zip.avail_out, stream->file );
    n = nsize - stream->zip.avail_out;
    stream->zip.total_in += n;
    stream->zip.total_out += n;
    return n;
}

int zgetc( zFile_t *stream )
{
    int tmp = 0;
    
    return ( zread( stream, (char *)&tmp, 1 ) == 1 ) ? tmp : -1;
}

char *zgets( char *str, int nsize, zFile_t *stream )
{
    char *s;
    int i;

    s = str;
    if( !str || nsize <= 0 ) return NULL;
    for( i = 0; i < nsize; i++ ){
        if( --i <= 0 ) break;
        if( zread( stream, s, 1 ) != 1 ) break;
        if( *s++ == '\n' ) break;
    }
    *s = '\0';
    return ( str == s && i > 0 ) ? NULL : str;
}

int zputblock( zFile_t *str, void *buffer, int nsize )
{
    if( !str || str->mode != 'w' ) return -2;
    str->zip.avail_in = nsize;
    str->zip.next_in = buffer;
    if( nsize ){
	do{
    	    if( !str->zip.avail_out ){
    		str->zip.next_out = str->pWrBuffRoot;
    		if( fwrite( str->pWrBuffRoot, 1, 0x4000, str->file ) != 0x4000 ){
		    str->Error = -1;
		    break;
		}
		str->zip.avail_out = 0x4000;
    	    }
    	    str->Error = deflate( &str->zip, 0 );
	}while( !str->Error && str->zip.avail_in );
    }
    str->crc = zSum(str->crc, buffer, nsize);
    return nsize - str->zip.avail_in;
}

int zvprintf( zFile_t *fh, const char *fmt, va_list vargs )
{
    int len;
    char buf[ 4096 ];

    vsprintf( buf, fmt, vargs );
    len = strlen( buf );
    if( len <= 0 ) return 0;
    return zputblock( fh, buf, len );
}

int zputc( zFile_t *stream, int chr )
{
    char tmp[ 8 ];

    tmp[ 0 ] = chr;
    return ( zputblock( stream, tmp, 1 ) == 1 ) ? tmp[ 0 ] : -1;
}

int zputs( zFile_t *stream, char *str )
{
    return zputblock( stream, str, strlen( str ) );
}

int zFlushInt( zFile_t *file, int arg )
{
    unsigned int n, avail_out, brk;
    Bytef *pWrBuffRoot;

    brk = 0;
    if( !file || file->mode != 'w' ) return -2;
    file->zip.avail_in = 0;
    do{
        n = 0x4000 - file->zip.avail_out;
        if( file->zip.avail_out != 0x4000 ){
            if( fwrite(file->pWrBuffRoot, 1u, 0x4000 - file->zip.avail_out, file->file) != n ){
                file->Error = -1;
                return -1;
            }
            pWrBuffRoot = file->pWrBuffRoot;
            file->zip.avail_out = 0x4000;
            file->zip.next_out = pWrBuffRoot;
        }
        if( brk ) break;
        file->Error = deflate(&file->zip, arg);
        if( !n && file->Error == -5 ) file->Error = brk;
        avail_out = file->zip.avail_out;
        if( avail_out || file->Error == 1 ) avail_out = 1;
        brk = avail_out;
    }while( file->Error < 2 );
    return ( file->Error == 1 ) ? 0 : file->Error;
}

int zflush( zFile_t *stream, int arg  )
{    
    if( !zFlushInt( stream, arg ) ){
	fflush( stream->file );
	return ( stream->Error == 1 ) ? 0 : stream->Error;
    }    
    return 0;
}

int zseek( zFile_t *stream, int offset, int whence )
{
    int k;
    
    if( !stream ) return -1;
    if( whence == 2 ) return -1;
    if( stream->Error == -1 || stream->Error == -3 ) return -1;
    if( stream->mode == 'w' ){
        if( !whence ) offset -= stream->zip.total_out;
        if( offset < 0 ) return -1;
        if( !stream->pRdBuffRoot ){
            stream->pRdBuffRoot = malloc( 0x4000 );
            memset( stream->pRdBuffRoot, 0x00, 0x4000 );
        }
        while( offset > 0 ){
            k = zputblock( stream, stream->pRdBuffRoot, ( offset < 0x4000 ) ? offset : 0x4000 );
            if( !k ) return -1;
            offset -= k;
        }
        return stream->zip.total_in;
    }
    if( whence == 1 ) offset += stream->zip.total_out;
    if( offset < 0 ) return -1;
    if( stream->RealFileFlag ){
        stream->zip.next_in = stream->pRdBuffRoot;
        stream->zip.avail_in = 0;
        if( fseek( stream->file, offset, SEEK_SET ) < 0 ) return -1;            
        stream->zip.total_in = offset;
        stream->zip.total_out = offset;
        return offset;
    }
    if( offset < stream->zip.total_out ){
	if( zrewind( stream ) < 0 ) return -1;
    } else {
        offset -= stream->zip.total_out;
    }
    if( offset && !stream->pWrBuffRoot ) stream->pWrBuffRoot = malloc( 0x4000 );
    while( offset > 0 ){
        k = zread( stream, stream->pWrBuffRoot, ( offset < 0x4000 ) ? k = offset : 0x4000 );
	if( k <= 0 ) return -1;
        offset -= k;
    }
    return stream->zip.total_out;
}

int zrewind( zFile_t *stream )
{
    if( !stream || stream->mode != 'r' ) return -1;
    stream->eof = 0;
    stream->zip.avail_in = 0;
    stream->Error = 0;
    stream->zip.next_in = stream->pRdBuffRoot;
    if( stream->pos ){
        inflateReset( &stream->zip );
        return fseek( stream->file, stream->pos, SEEK_SET );
    } else {
        rewind( stream->file );
        return 0;
    }
}

int ztell( zFile_t *stream )
{
    return zseek( stream, 0, SEEK_CUR );
}

int zfeof( zFile_t *stream )
{
    return ( stream && stream->mode == 'r' ) ? stream->eof : 0;
}

int zGetInt32( zFile_t *stream )
{
    int k;
    int n;

    k = zGetInt8( stream );
    k += zGetInt8( stream ) << 8;
    k += zGetInt8( stream ) << 16;
    n = zGetInt8( stream );
    if( n == -1 ) stream->Error = -3;
    return k + (n << 24);
}

int zclose( zFile_t *fh )
{
    unsigned int tmp, i;
    
    if( !fh ) return -2;
    if( fh->mode == 'w'){
	if( zFlushInt( fh, 4 ) ){ // save a 8-bit trailer
    	    tmp = fh->crc;
    	    for( i = 0; i < 4; i++, tmp >>= 8 ){ // save CRC32
        	fputc( tmp, fh->file );        	
    	    }
    	    tmp = fh->zip.total_in;
    	    for( i = 0; i < 4; i++, tmp >>= 8 ){ // save length of uncompressed
        	fputc( tmp, fh->file );
    	    }
        }
    }
    return zFileClose( fh );
}

char **zerrorp()
{
    return &Zerror;
}
