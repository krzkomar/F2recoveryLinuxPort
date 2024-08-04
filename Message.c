#include "FrameWork.h"

#define MESSAGE_BAD_WORDS_FILTER_FILE	"data/badwords.txt"

Msg_t gMessage; //misc.msg

char *gMessageBadWordsSubst = "!@#$%&*@#*!&$%#&%#*%!$&%@*$@&";
char **gMessageBadWordsList = NULL;
int gMessageBadWordsCount = 0;
int *gMessageBadWordsLength = NULL;

int MessageBadWordsInit()
{
    xFile_t *fh;
    int i,len;
    char stmp[ 80 ], *p;

    if( !(fh = dbOpen( MESSAGE_BAD_WORDS_FILTER_FILE, "rt" ) ) ) return -1;
    gMessageBadWordsCount = 0;
    while( dbgets( stmp, 79, fh ) ) gMessageBadWordsCount++;
    gMessageBadWordsList = (char **)Malloc( gMessageBadWordsCount * sizeof( char **) );
    if( !gMessageBadWordsList ){
	dbClose(fh);
	return -1;
    }
    gMessageBadWordsLength = (int *)Malloc( gMessageBadWordsCount * sizeof( int *) );
    if( !gMessageBadWordsLength ){
        Free( gMessageBadWordsList );
        dbClose( fh );
        return -1;
    }
    dbseek( fh, 0, SEEK_SET );
    for( i = 0; i < gMessageBadWordsCount; i++ ){
        if( !dbgets(stmp, 79, fh) ) break;
        len = strlen( stmp ) + 1;
        if( stmp[ len - 2 ] == '\n' ) stmp[ len - 2 ] = '\0';
        p = strdup( stmp );
        gMessageBadWordsList[ i ] = p;
        if( !p ) break;
        StrUpr( p );
        gMessageBadWordsLength[ i ] = len;
    }    
    len = i;
    dbClose( fh );
    if( i != gMessageBadWordsCount ){
	for( i = len - 1; i > 0; i-- ){
    	    Free( gMessageBadWordsList[ i ] );
	}
	Free( gMessageBadWordsList );
	Free( gMessageBadWordsLength );    
	return -1;
    }
    return 0;
}

void MessageBadWordsClear()
{
    int i;

    for( i = 0; i < gMessageBadWordsCount; i++ ) Free( gMessageBadWordsList[ i ] );
    if( gMessageBadWordsCount ){
        Free( gMessageBadWordsList );
        Free( gMessageBadWordsLength );
    }
    gMessageBadWordsCount = 0;
}

int MessageInit( Msg_t *Msg )
{
    if( !Msg ) return 0;
    Msg->Lines = 0;
    Msg->Count = 0;
    return 1;
}

int MessageClose( Msg_t *msg )
{
    int i;
    MsgLine_t *p;

    if( !msg ) return 0;
    for( i = 0; i < msg->Count; i++ ){
	p = &msg->Lines[ i ];
        if( p->Audio ) Free( p->Audio );
        if( p->Text ) Free( p->Text );
    }
    if( msg->Lines ){
        Free( msg->Lines );
        msg->Lines = NULL;
    }
    msg->Count = 0;
    return 1;
}

int MessageSave( Msg_t *msg, const char *fname )
{
    xFile_t *fh;
    int i;
    char stmp[260], *pStr;

    if( !msg ) return 0;
    if( !fname ) return 0;
    if( CfgGetString( &gConfiguration, "system", "language", &pStr ) != 1 ) return 0;
    sprintf( stmp, "%s/%s/%s", "text", pStr, fname );
    if( !(fh = dbOpen( stmp, "wt" ) ) ) return 0;
    for( i = 0; i < msg->Count; i++ ){
        dbPrintf( fh, "%c%d%c%c%s%c%c%s%c\n", 
    	    '{', msg->Lines[ i ].Id, 
    	    '}', '{', msg->Lines[ i ].Audio, 
    	    '}', '{', msg->Lines[ i ].Text, '}');
    }
    dbClose( fh );
    return 1;
}

int MessageLoad( Msg_t *Msg, const char *fname )
{
    xFile_t *fh;
    MsgLine_t ml;
    int exit;
    char TextField[1028];
    char AudioField[1028];
    char tmp[1028];
    char stmp[260];
    char *Lang;

    if( !Msg || !fname ) return 0;
    if( CfgGetString( &gConfiguration, "system", "language", &Lang ) != 1 ) return 0;
    sprintf( stmp, "%s/%s/%s", "text", Lang, fname );
    if( !(fh = dbOpen( stmp, "rt" ) ) ) return 0;
    ml.Unk = 0;
    ml.Audio = AudioField;
    ml.Text  = TextField;
    while( !(exit = MessageGetParameter( fh, tmp )) ){
        if( MessageGetParameter( fh, AudioField ) ){ ErrorPrintf( "\nError loading audio field.\n", stmp ); break; }
        if( MessageGetParameter( fh, TextField ) ){ ErrorPrintf( "\nError loading text field.\n", stmp ); break; }
        if( MessageNumberParse( &ml.Id, tmp ) != 1 ){ ErrorPrintf( "\nError parsing number.\n", stmp ); break; }
        if( MessageAddRemoveLine( Msg, &ml ) != 1 ){ ErrorPrintf( "\nError adding message.\n", stmp ); break; }
    }
    if( exit == 1 ){
	ErrorPrintf( "\nError loading message file %s at offset %x.\n.", stmp, dbtell( fh ) );
    }
    dbClose( fh );    
    return exit;
}

int MessageGetMsg( Msg_t *Msg, MsgLine_t *Line )
{
    int LineNo;

    if( !Msg || !Line || Msg->Count <= 0 || MessageFindLineInsertPos( Msg, Line->Id, &LineNo ) != 1 ) return 0;
    Line->Unk   = Msg->Lines[ LineNo ].Unk & 1;
    Line->Audio = Msg->Lines[ LineNo ].Audio;
    Line->Text  = Msg->Lines[ LineNo ].Text;
    return 1;
}

int MessageAddRemoveLine_p( Msg_t *Msg, MsgLine_t *NewLine )
{
    if( Msg && NewLine ) return MessageAddRemoveLine( Msg, NewLine );
    return 0;
}

int MessageDelete_p( Msg_t *msg, int Id )
{
    if( msg && (msg->Count > 0) ) return MessageDelete( msg, Id );
    return 0;
}

int MessageMakeFilePath( char *fpath, const char *fname )
{
    char *lang;

    if( !fpath || !fname || CfgGetString(&gConfiguration, "system", "language", &lang ) != 1 ) return 0;
    sprintf( fpath, "%s/%s/%s", "text", lang, fname );
    return 1;
}

int MessageFindLineInsertPos( Msg_t *msg, int Id, int *LineNo )
{
    int a, b, n;

    *LineNo = 0;
    if( msg->Count <= 0 ) return 0;
    a = msg->Count - 1;
    b = 0;
    while( a >= b ){    
        *LineNo = (b + a) / 2;
        n = msg->Lines[ *LineNo ].Id;
        if( Id == n ) return 1;
        if( Id >= n ) 
    	    b = *LineNo + 1;
        else
            a = *LineNo - 1;
    }
    if( Id > n ) (*LineNo)++;
    return 0;
}

int MessageAddRemoveLine( Msg_t *Msg, MsgLine_t *NewLine )
{
    MsgLine_t *p;
    int LineNo;

    if( MessageFindLineInsertPos( Msg, NewLine->Id, &LineNo ) == 1 ){ // if found then remove
        p = &Msg->Lines[ LineNo ];
        if( p->Audio ) Free( p->Audio );
        p = &Msg->Lines[ LineNo ];
        if( p->Text ) Free( p->Text );
    } else { // add a new one
        if( Msg->Lines ){ // if occupied than expand
            p = (MsgLine_t *)Realloc( Msg->Lines, (Msg->Count + 1) * sizeof( MsgLine_t ) );
            if( !p ) return 0;
            Msg->Lines = p;
            // move message one position
            if( LineNo != Msg->Count ) memmove( &Msg->Lines[ LineNo + 1 ], &Msg->Lines[ LineNo ], (Msg->Count - LineNo) * sizeof( MsgLine_t ) );
        } else { // create a new
            Msg->Lines = (MsgLine_t *)Malloc( sizeof( MsgLine_t ) );
            if( !Msg->Lines ) return 0;
            Msg->Count = 0;
            LineNo = 0;
        }
        Msg->Lines[ LineNo ].Unk   = 0;
        Msg->Lines[ LineNo ].Audio = NULL;
        Msg->Lines[ LineNo ].Text  = NULL;
        Msg->Count++;
    }
    Msg->Lines[ LineNo ].Audio = strdup( NewLine->Audio );
    if( !Msg->Lines[ LineNo ].Audio ) return 0;
    Msg->Lines[ LineNo ].Text = strdup( NewLine->Text );
    if( !Msg->Lines[ LineNo ].Text ) return 0;
    Msg->Lines[ LineNo ].Id = NewLine->Id;
    return 1;
}

int MessageDelete( Msg_t *Msg, int Id )
{
    MsgLine_t *p;
    int Pos;

    if( MessageFindLineInsertPos(Msg, Id, &Pos) != 1 ) return 0;    
    p = &Msg->Lines[ Pos ];
    if( p->Audio ) Free( p->Audio );
    if( p->Text ) Free( p->Text );
    memmove( &Msg->Lines[ Pos ], &Msg->Lines[ Pos + 1 ], (Msg->Count - Pos) * sizeof( MsgLine_t ) );
    Msg->Count--;
    if( Msg->Count <= 0 ){
        Free( Msg->Lines );
        Msg->Lines = NULL;
        return 1;
    }
    p = (MsgLine_t *)Realloc( Msg->Lines, Msg->Count * sizeof( MsgLine_t ) );
    if( !p ) return 0;
    Msg->Lines = p;
    return 1;
}

int MessageNumberParse( int *Val, char *Str )
{
    char *s;
    int err;

    if( *Str == '\0' ) return 0;
    s = Str;
    err = 1;
    if( (*Str == '-') || (*Str == '+') ) s++;
    for( ; *s; s++ ){
        if( (IsTable[(int)(*s + 1)] & 0x20) == 0 ){
            err = 0;
            break;
        }
    }
    *Val = strtol( Str, NULL, 10 );
    return err;
}

int MessageGetParameter( xFile_t *fh, char *Text )
{
    int ex, c, i;

    ex = 1;
    while( ( c = dbgetc( fh ) ) != -1  ){
        if( c == '}' ){
	    ex = 2;
	    ErrorPrintf( "\nError reading message file - mismatched delimiters.\n" );
	    break;
        }
        if( c == '{' ){
	    ex = 0;
    	    break;
        }
    }
    if( c != '{' ) return ex;    
    i = 0;
    while( 1 ){
        if( (c = dbgetc( fh )) == -1 ){
            ErrorPrintf( "\nError reading message file - EOF reached.\n" );
            return 3;
        }
        if( c == '}' ) break;
        if( c == '\n' ) continue;
        Text[ i++ ] = c;
        if( i > 1024 ){
	    ErrorPrintf( "\nError reading message file - text exceeds limit.\n" );
	    return 4;
        }
    }
    Text[ i ] = '\0';
    return ex;
}

char *MessageGetMessage( Msg_t *Msg, MsgLine_t *Line, int StrId )
{
    Line->Id = StrId;
    if( MessageGetMsg( Msg, Line ) != 1 ){
        Line->Text = "Error";
        eprintf( " ** String not found @ getmsg(), MESSAGE.C **" );
    }
    return Line->Text;
}

int MessageLangFilter( Msg_t *msg )
{
/*
    static char tmp[ 1024 ];
    int LangFilter = 0, SubstPos, k, SubstLen, n, j;
    char *str, *t, *p;
    
    if( !msg ) return 0;    
    if( msg->Count == 0 ) return 1;
    if( gMessageBadWordsCount == 0 ) return 1;
    CfgGetInteger( &gConfiguration, "preferences", "language_filter", &LangFilter );
    if( LangFilter != 1 ) return 1;
    SubstLen = strlen( gMessageBadWordsSubst );
    SubstPos = RandMinMax( 1, SubstLen ) - 1;
    for( k = 0; k < msg->Count; k++ ){
        str = msg->Lines[ k ].Text;
        strcpy( tmp, str );
        StrUpr( tmp ); 
        for( n = 0; n < gMessageBadWordsCount; n++ ){
            for( t = tmp; (t = strstr( t, gMessageBadWordsList[ n ] )); t++ ){
                if( 
            	    ( t == tmp || (IsTable[ t[-1] + 1 ] & 0xC0 ) == 0) && 
            	    ( IsTable[ t[ gMessageBadWordsLength[ n ] ] + 1 ] & 0xC0 ) == 0 
            	){
                    msg->Lines[ LineNo ].Unk |= 1;
                    p = &str[ t - tmp ];
                    for( j = 0; j < gMessageBadWordsLength[ n ]; j++, p++ ){
                        *p = gMessageBadWordsSubst[ SubstPos++ ];
                        if( SubstPos == SubstLen ) SubstPos = 0;
                    }
                }
            }                
        }
    }
*/
    return 1;
}

