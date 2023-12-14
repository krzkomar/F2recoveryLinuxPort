#include "FrameWork.h"

typedef struct // size of 16
{
    int		Id;	// text identifier
    char	Unk;	// ?
    char	*Audio;	// audio path ?
    char	*Text;	// text message
} MsgLine_t;

typedef struct // size of 8
{
    int		Count;	 // lines count
    MsgLine_t	*Lines;  // lines data
} Msg_t;


extern Msg_t gMessage;
extern char **gMessageBadWordsList;
extern int gMessageBadWordsCount;
extern int *gMessageBadWordsLength;

int MessageBadWordsInit();
void MessageBadWordsClear();
int MessageInit( Msg_t *Msg );
int MessageClose( Msg_t *msg );
int MessageSave( Msg_t *msg, const char *fname );
int MessageLoad( Msg_t *Msg, const char *fname );
int MessageGetMsg( Msg_t *Msg, MsgLine_t *Line );
int MessageAddRemoveLine_p( Msg_t *Msg, MsgLine_t *NewLine );
int MessageDelete_p( Msg_t *msg, int Id );
int MessageMakeFilePath( char *fpath, const char *fname );
int MessageFindLineInsertPos( Msg_t *msg, int Id, int *LineNo );
int MessageAddRemoveLine( Msg_t *Msg, MsgLine_t *NewLine );
int MessageDelete( Msg_t *Msg, int Id );
int MessageNumberParse( int *Val, char *Str );
int MessageGetParameter( xFile_t *fh, char *Text );
char *MessageGetMessage( Msg_t *Msg, MsgLine_t *Line, int StrId );
int MessageLangFilter( Msg_t *msg );
