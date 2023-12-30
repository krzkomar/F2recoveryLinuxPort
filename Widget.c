#include "FrameWork.h"

int gWidgetEditorRun = 0;
WidgetMsg_t gWidgetEditor;
int gWidgetUnk08;
int gWidgetEditValidFlg;
int gWidgetUnk07;
WidgetProc_t *gWidgetUnk10[ 32 ];
Widget02_t gWidgetUnk13;
char *gWidgetEditorString;
int gWidgetUnk09;
WidgetMsg_t *gWidgetEvQe;
int gWidgetEvQeSize;
Window01_t *gWidgets;
int gWidgetUnk12;
int gWidgetTableSize;
int gWidgetUnk02;
int gWidgetUnk01;
int gWidgetUnk03;

/*******************************************************/

void WidgetEditDrawCursor( WidgetMsg_t *msg )
{
    VidRect_t rect;
    Window01_t *wg;
    char *Surface;
    unsigned int SysTime;
    int posX,posY,space,spc,wrap,s,chrW,p,Color,pitch,FontId,widx;

    SysTime = TimerGetSysTime();
    if( SysTime >= gWidgetUnk01 + 500 ){
        gWidgetUnk01 = SysTime;
        gWidgetUnk02 = gWidgetUnk02 == 0;
    }
    FontId = FontGetCurrent();
    widx = msg->id - 1;
    posX = gWidgets[ widx ].Xpos;
    posY = gWidgets[ widx ].Ypos;
    FontSet( gWidgets[ widx ].Font );
    space = gFont.Distance();
    wg = &gWidgets[ widx ];
    spc = space;
    wrap = gWidgets[ widx ].Wrap;
    if( wrap ){
        if( wrap == 1 ){
            posX += wg->StrField - 1;
            for( p = msg->Str01 - msg->EditedString + msg->StrLen01; p != msg->CursorPos; posX -= spc + gFont.ChrWidth(msg->EditedString[p - 1]) );
        }
    } else {
        s = msg->Str01 - msg->EditedString;
        if( s < msg->CursorPos ) posX -= space;
        while( s < msg->CursorPos ){
            chrW = gFont.ChrWidth( msg->EditedString[ s++ ] );
            posX += spc + chrW;
        }
    }
    Color = ( gWidgetUnk02 ) ? gWidgets[ widx ].ColorBg : gWidgets[ widx ].ColorFg;
    rect.rt = posX;
    rect.tp = posY;
    rect.lt = posX;
    rect.bm = posY + gFont.ChrHeight() - 1;
    pitch = WinGetWidth( gWidgets[ widx ].Id );
    Surface = WinGetSurface( gWidgets[ widx ].Id );
    ScrLine( Surface, pitch, posX, posY, posX, rect.bm, Color );// cursor
    WinAreaUpdate( gWidgets[ widx ].Id, &rect );
    FontSet( FontId );
}

void WidgetUnk02( int a1, int a2 )
{
    WidgetMsg_t *p;
    char *EditedString, *errcall, *v34;
    int Key,v3,Wrap,v31,p0,ErrCode,CursorPos,FontId;

    if( gWidgetUnk03 ) return;
    if( a2 < 0 ) return;
    if( (a2 & 0x400) == 0 ) return;
    a2 &= ~0x0400;
    v3 = a2 - 1;
    if( v3 < 0 ) return;
    if( v3 >= gWidgetEvQeSize ) return;
    p = &gWidgetEvQe[ v3 ];
    if( !p->i01 ) return;
    gWidgetUnk03 = 1;
    EditedString = p->EditedString;
    p->Str01 = EditedString;
    p->StrLen01 = strlen( EditedString );
    Wrap = gWidgets[ p->id - 1 ].Wrap;
    gWidgets[ p->id - 1 ].Wrap = gWidgetUnk03;
    ErrCode = Wrap;
    if( strlen( p->EditedString ) ){
        errcall = dbg_malloc(p->InputLength + 1 );
        memcpy( errcall, p->EditedString, p->InputLength);
        CursorPos = p->CursorPos;
    } else {
        errcall = NULL;
    }
Next:
    FontId = FontGetCurrent();
    FontSet( gWidgets[ p->id - 1 ].Font );
    if( &p->EditedString[ p->CursorPos ] >= p->Str01 ){
        v31 = 0;
        v34 = p->Str01;
        if( p->Str01 < &p->EditedString[ p->CursorPos ] ){
            do{            
                v31 += gFont.ChrWidth( *p->Str01 ) + gFont.Distance();
                v34++;
            }while( v34 < &p->EditedString[ p->CursorPos ] );
        }
        if( v31 > gWidgets[p->id - 1].StrField ){
            gWidgets[p->id - 1].Wrap = 1;
            if( v31 - gWidgets[p->id - 1].StrField > 0 ){
                do{
                    p0 = gFont.ChrWidth( *p->Str01 );
                    p->Str01++;
                } while( v31 - (p0 + gFont.Distance()) > 0 );
            }
            p->StrLen01 = &p->EditedString[p->CursorPos] - p->Str01;
            WidgetDrawText0( p->id, p->Str01, p->StrLen01 );
        } else {
	    p->StrLen01 = strlen( p->Str01 );
	    WidgetDrawText1( p->id, p->Str01 );
	}
    } else {
        p->Str01 = &p->EditedString[ p->CursorPos ];
        gWidgets[ p->id - 1 ].Wrap = 0;
	p->StrLen01 = strlen( p->Str01 );
	WidgetDrawText1( p->id, p->Str01 );
    }
    WidgetLabelMouseDefine( p->id );
    FontSet( FontId );
    WidgetEditDrawCursor( p );
    while( 1 ){
        if( p->Validator ) p->Validator();
        Key = InpUpdate();
        if( Key == KEY_ESC ){
	    if( errcall )
    		memcpy( p->EditedString, errcall, p->InputLength );
	    else
    		memset( p->EditedString, 0, p->InputLength );
	    p->CursorPos = CursorPos;
    	    break;
        }
        if( Key == 13 ) break;
        switch( Key ){
            case 32 ... 127:
            	    if( p->InputLength > p->StrLen ){
                	if( p->StrLen >= p->CursorPos ){
                    	    if( p->StrLen > p->CursorPos ) memmove( &p->EditedString[ p->CursorPos + 1], &p->EditedString[ p->CursorPos ], p->StrLen - p->CursorPos );
                    	    p->EditedString[ p->CursorPos ] = Key;
                	}
            		p->StrLen++;
                	p->CursorPos++;
            	    }
            	    break;
            case 8:
            	    if( p->StrLen ){
                	if( p->CursorPos ){
                    	    p->CursorPos--;
                    	    if( p->StrLen > p->CursorPos ) memcpy( &p->EditedString[ p->CursorPos ], &p->EditedString[ p->CursorPos + 1 ], p->StrLen - p->CursorPos );
                    	    p->StrLen--;
                	}
            	    }
            	    break;
            case 327: p->CursorPos = 0; break;
            case 0x14B: if( --p->CursorPos < 0 ) p->CursorPos = 0; break;
            case 333: if( ++p->CursorPos > p->StrLen ) p->CursorPos = p->StrLen; break;
            case 0x14F: p->CursorPos = p->StrLen; break;
            case 339:
            	    if( p->CursorPos < p->StrLen ){
                	if( p->StrLen > p->CursorPos ) memcpy( &p->EditedString[ p->CursorPos ], &p->EditedString[ p->CursorPos + 1], p->StrLen - p->CursorPos );
                	p->StrLen--;
            	    }
            	    break;
            default: WidgetEditDrawCursor( p ); continue;
        }
        goto Next;
    }
    if( errcall ) dbg_free( errcall );
    if( strlen( p->EditedString ) < p->CursorPos ) p->CursorPos = strlen( p->EditedString );
    gWidgets[ p->id - 1 ].Wrap = ErrCode;
    p->StrLen = p->StrLen01 = strlen( p->EditedString );
    WidgetDrawText1( p->id, p->EditedString );
    WidgetLabelMouseDefine( p->id );
    gWidgetUnk03 = 0;
}

void WidgetDrawMsg( int idx, WidgetMsg_t *msg )
{
    char *sb, *s;
    int sd,StrWidth,ToWide, FontId;

    FontId = FontGetCurrent();
    FontSet( gWidgets[ idx-1 ].Font );
    sd = msg->CursorPos;
    sb = msg->EditedString + sd;
    if( sb < msg->Str01 ){
        msg->Str01 = sb;
        gWidgets[ idx ].Wrap = 0;
        msg->StrLen01 = strlen( msg->Str01 );
        WidgetDrawText1( idx, msg->Str01 );
    } else {
	s = msg->Str01;
	StrWidth = 0;
    	while ( s < (msg->EditedString + sd) ){
    	    StrWidth += gFont.ChrWidth(*s) + gFont.Distance();
    	    s++;
    	}
	if( StrWidth <= gWidgets[idx - 1].StrField ){
	    msg->StrLen01 = strlen( msg->Str01 );
	    WidgetDrawText1( idx, msg->Str01 );
	} else {
	    ToWide = StrWidth - gWidgets[idx - 1].StrField;
	    gWidgets[ idx - 1 ].Wrap = 1;
	    while( ToWide > 0 ){
    		ToWide -= gFont.ChrWidth( *msg->Str01 ) + gFont.Distance();
    		msg->Str01++;
	    }
	    msg->StrLen01 = &msg->EditedString[ msg->CursorPos ] - msg->Str01;
	    WidgetDrawText0( idx, msg->Str01, &msg->EditedString[ msg->CursorPos ] - msg->Str01 );
	}
    }
    WidgetLabelMouseDefine( idx );
    FontSet( FontId );
}

void WidgetEditorStart( int WgId, char *Str, int ValidatorFlg, int BufSize, int AutoUpdate )
{
    int tmp,Current,idx;

    idx = WgId;
    tmp = gWidgetEditorRun;
    if( gWidgetEditorRun ) return;
    gWidgetEditValidFlg = BufSize;
    gWidgetEditor.InputLength = ValidatorFlg;
    gWidgetEditor.EditedString = Str;
    gWidgetUnk09 = AutoUpdate;
    gWidgetEditorRun = 1;
    gWidgetEditor.i01 = 1;
    gWidgetEditor.id = WgId;
    gWidgetEditor.StrLen = strlen( Str );
    gWidgetEditor.CursorPos = gWidgetEditor.StrLen;
    gWidgetUnk08 = tmp;
    gWidgetUnk07 = tmp;
    gWidgetEditor.Str01 = Str;
    gWidgetEditor.StrLen01 = strlen( Str );
    if( strlen( Str ) ){
        gWidgetEditorString = (char *)dbg_malloc( ValidatorFlg + 1 );
        memcpy( gWidgetEditorString, Str, ValidatorFlg );
    } else {
        gWidgetEditorString = NULL;
    }
    gWidgets[ idx - 1 ].Wrap = 0;
    Current = FontGetCurrent();
    FontSet( gWidgets[ idx-1 ].Font ); 
    WidgetDrawText1( idx, Str );
    FontSet( Current );
    if( AutoUpdate ) InpTaskStart( WidgetEditorUpdate );
    WidgetDrawMsg( idx, &gWidgetEditor );
}

void WidgetEditorUpdate()
{
    int Key, WgIdx;

    Key = KeyGet();
    WgIdx = gWidgetEditor.id;
    WidgetEditDrawCursor( &gWidgetEditor );
    if( gWidgetEditValidFlg ) gWidgetEditor.Validator();
    if( Key == -1 ){ WidgetEditDrawCursor( &gWidgetEditor ); return; }
    if( Key == KEY_ESC ){
        if( gWidgetEditorString )
            memcpy( gWidgetEditor.EditedString, gWidgetEditorString, gWidgetEditor.InputLength );
        else
            memset( gWidgetEditor.EditedString, 0, gWidgetEditor.InputLength );
        gWidgetEditor.CursorPos = 0;
        gWidgetEditor.StrLen = strlen( gWidgetEditor.EditedString );
	WidgetDrawMsg( WgIdx, &gWidgetEditor ); 
	return;
    }
    if( Key == '\r' ){
	if( gWidgetEditorString ) dbg_free( gWidgetEditorString );
	if( strlen( gWidgetEditor.EditedString ) ){
    	    gWidgetEditorString = dbg_malloc( gWidgetEditor.InputLength + 1 );
    	    memcpy( gWidgetEditorString, gWidgetEditor.EditedString, gWidgetEditor.InputLength );
	} else {
    	    gWidgetEditorString = NULL;
	}
	gWidgetEditor.CursorPos = 0;
	WidgetDrawMsg( WgIdx, &gWidgetEditor );
	return;
    }
    if( Key == -2 ){ WidgetEditDrawCursor( &gWidgetEditor ); return; }
    if( Key > 31 && Key <= 127 ){
        if( gWidgetEditor.InputLength > gWidgetEditor.StrLen ){
            if( gWidgetEditor.StrLen >= gWidgetEditor.CursorPos ){
                if( gWidgetEditor.StrLen > gWidgetEditor.CursorPos ) memmove( &gWidgetEditor.EditedString[ gWidgetEditor.CursorPos + 1 ], &gWidgetEditor.EditedString[ gWidgetEditor.CursorPos ], gWidgetEditor.StrLen - gWidgetEditor.CursorPos );
                gWidgetEditor.EditedString[ gWidgetEditor.CursorPos ] = Key;
            }
            gWidgetEditor.StrLen++;
            gWidgetEditor.CursorPos++;
        }
        WidgetDrawMsg( WgIdx, &gWidgetEditor );
        return;
    }
    if( Key < 331 ){
        if( Key < 8 ){ WidgetEditDrawCursor( &gWidgetEditor ); return; }
        if( Key <= 8 ){
            if( gWidgetEditor.StrLen ){
                if( gWidgetEditor.CursorPos ){
                    gWidgetEditor.CursorPos--;
                    if( gWidgetEditor.StrLen > gWidgetEditor.CursorPos ) memcpy( &gWidgetEditor.EditedString[ gWidgetEditor.CursorPos ], &gWidgetEditor.EditedString[gWidgetEditor.CursorPos + 1], gWidgetEditor.StrLen - gWidgetEditor.CursorPos );
                    gWidgetEditor.StrLen--;
                }
            }
        } else {
    	    if( Key != 327 ){ WidgetEditDrawCursor( &gWidgetEditor ); return; }
	    gWidgetEditor.CursorPos = 0;
	}
    } else {
        if( Key > 331 ){
            if( Key < 335 ){
                if( Key == 333 ){ 
            	gWidgetEditor.CursorPos++; 
            	if( gWidgetEditor.CursorPos > gWidgetEditor.StrLen ) gWidgetEditor.CursorPos = gWidgetEditor.StrLen; 
        	    } else {
            	WidgetEditDrawCursor( &gWidgetEditor ); 
            	return;
                }
            }
            if( Key > 335 ){
                if( Key == 339){
                    if( gWidgetEditor.StrLen > gWidgetEditor.CursorPos ){
                        if( gWidgetEditor.StrLen > gWidgetEditor.CursorPos ) memcpy(&gWidgetEditor.EditedString[ gWidgetEditor.CursorPos ], &gWidgetEditor.EditedString[ gWidgetEditor.CursorPos + 1 ], gWidgetEditor.StrLen - gWidgetEditor.CursorPos );
                        gWidgetEditor.StrLen--;
                    }
                } else {
            	    WidgetEditDrawCursor( &gWidgetEditor ); 
            	    return;
        	}
            } else {
        	gWidgetEditor.CursorPos = gWidgetEditor.StrLen;
            }
        } else {
    	    gWidgetEditor.CursorPos--;
    	    if( gWidgetEditor.CursorPos < 0 ) gWidgetEditor.CursorPos = 0;	    
        }
    }
    WidgetDrawMsg( WgIdx, &gWidgetEditor ); 
}

void WindgetEditorEnd()
{
    int id;

    if( !gWidgetEditorRun ) return;
    WidgetDrawMsg( gWidgetEditor.id, &gWidgetEditor );
    id = gWidgetEditor.id - 1;
    if( gWidgetEditor.id - 1 >= 0 && id <= gWidgetTableSize && gWidgets[ id ].Pending ) gWidgets[ id ].Pending = 0;
    InpTaskStop( WidgetEditorUpdate );
    if( gWidgetEditorString ) dbg_free( gWidgetEditorString );
    gWidgetEditorRun = 0;
}

int WidgetUnk01( int a1, char *a2, int a3, void( *a4)() )
{
    int i, Current, Button;

    if( a1 < 1 || a1 > gWidgetTableSize ) return 0;
    if( !gWidgets[a1 - 1].Pending ) return 0;
    for( i = 0; i < gWidgetEvQeSize; i++ ){
        if( !gWidgetEvQe[ i ].i01 ) break;
    }
    if( i == gWidgetEvQeSize ){
        if( gWidgetEvQe )
            gWidgetEvQe = dbg_realloc( gWidgetEvQe, sizeof( WidgetMsg_t ) * ( gWidgetEvQeSize + 1 ) );
        else
            gWidgetEvQe = dbg_malloc( sizeof( WidgetMsg_t ) );
        gWidgetEvQeSize++;
    }
    gWidgetEvQe[ i ].i01 = 1;
    gWidgetEvQe[ i ].CursorPos = 0;
    gWidgetEvQe[ i ].Validator = a4;
    gWidgetEvQe[ i ].InputLength = a3;
    gWidgetEvQe[ i ].id = a1;
    gWidgetEvQe[ i ].EditedString = a2;
    gWidgetEvQe[ i ].i09 = NULL;
    gWidgetEvQe[ i ].i05 = NULL;
    gWidgetEvQe[ i ].StrLen = strlen( a2 );
    Current = FontGetCurrent();
    FontSet( gWidgets[ a1 - 1 ].Font );
    Button = WinCreateButton( 
	gWidgets[ i ].Id, gWidgets[ i ].Xpos, gWidgets[ i ].Ypos, 
	gWidgets[ i ].StrField, gFont.ChrHeight(), -1, -1, -1, (i + 1) | 0x400, 0, 0, 0, 0
    );
    WinSetButtonHandler( Button, 0, 0, 0, WidgetUnk02 );
    WidgetDrawText1( a1, a2 );
    gWidgetEvQe[ i ].i08 = Button;
    FontSet( Current );
    return i + 1;
}

void WidgetUnk03( int a1 )
{
    WidgetUnk02( gWidgetEvQe[ a1 - 1 ].i08, a1 | 0x400 );
}

int WidgetEvProcess( int Id )
{
    int i;

    for( i = 0; i < gWidgetEvQeSize; i++ ){
        if( Id == gWidgets[ gWidgetEvQe[ i ].id - 1 ].Id ) WidgetEvExe( i + 1 );
    }
    return 1;
}

int WidgetEvExe( int Idx )
{
    WidgetMsg_t *p;
    int widx;
    
    Idx--;
    if( Idx - 1 < 0 && Idx >= gWidgetEvQeSize ) return 0;    
    p = &gWidgetEvQe[ Idx ];
    if( !p->i01 ) return 0;
    if( p->i09 ) gWidgetEvQe[ Idx ].i09( p->EditedString, p->i05 );
    widx = gWidgetEvQe[ Idx ].id - 1;
    if( widx >= 0 && widx <= gWidgetTableSize ){
        if( gWidgets[ widx ].Pending ) gWidgets[ widx ].Pending = 0;
    }
    gWidgetEvQe[ Idx ].i01 = 0;
    return 1;
}

int WidgetUnk04( int MsgId, void (*cb)(char *, void * ), void *ptr )
{
    MsgId--;
    if( MsgId < 0 || MsgId >= gWidgetEvQeSize ) return 0;
    if( !gWidgetEvQe[ MsgId ].i01 ) return 0;
    gWidgetEvQe[ MsgId ].i05 = ptr;
    gWidgetEvQe[ MsgId ].i09 = cb;
    return 1;
}

int WidgetMsgNew( int MsgId, int posX, int Field, int posY, int Font, int Wrap, int Flags, char Color )
{
    Window01_t *p;
    int i, h, SaveFontId, FontHeight;

    for( i = 0; i < gWidgetTableSize; i++ ){
        if( !gWidgets[ i ].Pending ) break;        
    }
    if( i == gWidgetTableSize ){
        if( gWidgets )
            p = dbg_realloc( gWidgets, sizeof( Window01_t ) * (gWidgetTableSize + 1) );
        else
            p = dbg_malloc( sizeof( Window01_t ) );
        gWidgets = p;
        gWidgetTableSize++;
    }
    SaveFontId = FontGetCurrent();
    FontSet( Font );
    h = gFont.ChrHeight();
    FontSet( SaveFontId );
    if( Flags & 0x10000 ){
        FontHeight = h + 1;
        posY++;
    }
    gWidgets[ i ].Pending = 1;
    gWidgets[ i ].Id = MsgId;
    gWidgets[ i ].StrField = posY;
    gWidgets[ i ].Xpos = posX;
    gWidgets[ i ].Ypos = Field;
    gWidgets[ i ].Font = Font;
    gWidgets[ i ].Wrap = Wrap;
    gWidgets[ i ].ColorBg = Flags;
    gWidgets[ i ].FontHeight = FontHeight;
    gWidgets[ i ].ColorFg = Color;
    return i + 1;
}

int WidgetDrawText1( int Idx, char *str )
{
    int FontId;
    
    Idx--;
    if( Idx < 0 || Idx > gWidgetTableSize ) return 0;    
    if( !gWidgets[ Idx ].Pending ) return 0;
    FontId = FontGetCurrent();
    FontSet( gWidgets[ Idx ].Font );
    WinDrawFilledRect( 
	gWidgets[ Idx ].Id, gWidgets[ Idx ].Xpos, gWidgets[ Idx ].Ypos, 
	gWidgets[ Idx ].StrField, gWidgets[ Idx ].FontHeight, gWidgets[ Idx ].ColorFg 
    );
//    WinPrintText( 
//	gWidgets[ Idx ].Id, str, gWidgets[ Idx ].StrField, strlen( str ), 
//	WinGetHeight( gWidgets[ Idx ].Id ), gWidgets[ Idx ].Xpos, gWidgets[ Idx ].Ypos, gWidgets[ Idx ].ColorBg | 0x2000000, gWidgets[ Idx ].Wrap );
    FontSet( FontId );
    return 1;
}

int WidgetDrawText0( int MsgId, char *Str, int StrBufLen )
{
    int Current;

    MsgId--;
    if( MsgId < 0 || MsgId > gWidgetTableSize ) return 0;
    if( !gWidgets[ MsgId ].Pending ) return 0;
    Current = FontGetCurrent();
    FontSet( gWidgets[ MsgId ].Font);
    WinDrawFilledRect( 
	gWidgets[ MsgId ].Id, gWidgets[ MsgId ].Xpos, gWidgets[ MsgId ].Ypos, 
	gWidgets[ MsgId ].StrField, gWidgets[ MsgId ].FontHeight, gWidgets[ MsgId ].ColorFg
    );
//    WinPrintText( 
//	gWidgets[ MsgId ].Id, Str, gWidgets[ MsgId ].StrField, StrBufLen, 
//	WinGetHeight( gWidgets[ MsgId ].Id ), gWidgets[ MsgId ].Xpos, gWidgets[ MsgId ].Ypos, 
//	gWidgets[ MsgId ].ColorBg | 0x2000000, gWidgets[ MsgId ].Wrap 
//    );
    FontSet( Current );
    return 1;
}

int WidgetLabelMouseDefine( int MsgId )
{
    Window01_t *msg;
    VidRect_t rect;

    MsgId--;
    if( MsgId < 0 || MsgId > gWidgetTableSize ) return 0;
    msg = &gWidgets[ MsgId ];
    if( !msg->Pending ) return 0;
    rect.lt = msg->Xpos;
    rect.tp = msg->Ypos;
    rect.rt = msg->StrField + msg->Xpos;
    rect.bm = gFont.ChrHeight() + msg->Ypos;
    WinAreaUpdate( gWidgets[ MsgId ].Id, &rect );
    return 1;
}

int WidgetUnk06( int a1 )
{
    a1--;
    if( a1 < 0 || a1 > gWidgetTableSize ) return 0;
    if( !gWidgets[ a1 ].Pending ) return 0;
    gWidgets[ a1 ].Pending = 0;
    return 1;
}

int WidgetUnk07( int Idx ) 
{
    int i;
    WidgetProc_t *p;

    for( i = 0; i != 32; ++i ){
        p = gWidgetUnk10[ i ];
        if( p && Idx == p->i01 ){
            dbg_free( gWidgetUnk10[ i ] );
            gWidgetUnk10[ i ] = NULL;
        }
    }
    return 1;
}

int WidgetUnk08( int Idx, int Font, int Wrap, int ColorBg, int ColorFg )
{
    int Current, h;

    Idx--;
    if( Idx < 0 || Idx > gWidgetTableSize ) return 0;
    if( !gWidgets[ Idx ].Pending ) return 0;
    gWidgets[ Idx ].Font = Font;
    gWidgets[ Idx ].Wrap = Wrap;
    Current = FontGetCurrent();
    FontSet( Font );
    h = gFont.ChrHeight();
    FontSet( Current );
    if( (gWidgets[ Idx ].ColorBg & 0x10000) == 0 && (ColorBg & 0x10000) != 0 ){
        h++;
        gWidgets[ Idx ].StrField++;
    }
    gWidgets[ Idx ].FontHeight = h;
    gWidgets[ Idx ].ColorBg = ColorBg;
    gWidgets[ Idx ].ColorFg = ColorFg;
    return 1;
}

int WidgetUnk09( int Id )
{
    int i;

    WidgetEvProcess( Id );    
    for( i = 0; i < gWidgetTableSize; i++ ){
        if( Id == gWidgets[ i ].Id && i >= 0 && i <= gWidgetTableSize ){
            if( gWidgets[ i ].Pending ) gWidgets[ i ].Pending = 0;
        }        
    }        
    return WidgetUnk07( Id );
}

int WidgetUnk10()
{
    int i;

    for( i = 0; i != 32; i++ ){
        if( gWidgetUnk10[ i ] ) WidgetWinRegisterUpdate( gWidgetUnk10[ i ] );
    }
    return 0;
}

int WidgetUnk11( int WinId, char *Text, int Ypos, int Color )
{
    WinDrawText( WinId, Text, 0, ( WinGetWidth( WinId ) - gFont.LineWidth( Text ) ) / 2, Ypos, Color );
    return 1;
}

void WidgetWinRegisterUpdate( WidgetProc_t *proc )
{
    char stmp[ 80 ];
    float val;

    switch( proc->i04 & 0xff ){    
	case 1:  case 2: case 16: val = proc->itg; break;
	case 4:  val = proc->flt * 0.000015258789; break;
	case 8:  WinDrawText( proc->i01, proc->str, 0, proc->i02, proc->i03, proc->i05 ); return;
	default: goto err;	
    }

    switch( proc->i04 & 0xFF00 ){
	case 0x100: sprintf( stmp, " %d ", (int)lround( val ) ); WinDrawText( proc->i01, stmp, 0, proc->i02, proc->i03, proc->i05 | 0x1000000 ); return;
	case 0x200: sprintf( stmp, " %f ", val ); WinDrawText( proc->i01, stmp, 0, proc->i02, proc->i03, proc->i05 | 0x1000000 ); return;
	case 0x400: sprintf( stmp, " %6.2f%% ", val * 100.0 ); WinDrawText( proc->i01, stmp, 0, proc->i02, proc->i03, proc->i05 | 0x1000000 ); return;
	case 0x800: if( proc->i07 ) proc->i07( proc->itg ); return;
    }
err:
    eprintf( "Invalid input type given to win_register_update\n" );
    return;
}

int WidgetUnk12()
{
    int i;
    WidgetProc_t *p;

    for( i = 0; i != 32; i++ ){
        p = gWidgetUnk10[ i ];
        if( p && (p->i04 & 0xFF00) == 0x800 ) gWidgetUnk10[ i ]->i08( gWidgetUnk10[ i ]->itg );
    }
    return 1;
}

int WidgetUnk13()
{
    int i;

    for( i = 0; i < 32; i++ ){
        if( gWidgetUnk10[ i ] ) WidgetWinRegisterUpdate( gWidgetUnk10[ i ] );
    }
    return 1;
}

int WidgetUnk05( int a1, int a2, int a3, void (*a4)(int), void (*a5)(int), int a6, int a7, int a8 )
{
    int i;
    
    for( i = 0; i < 32; i++ ){
    	if( gWidgetUnk10[ i ] == NULL ) break;
    }
    if( i == 32 ) return -1;
    gWidgetUnk10[ i ] = dbg_malloc( 8 * sizeof( int ) );
    gWidgetUnk10[ i ]->i01 = a1;
    gWidgetUnk10[ i ]->i02 = a2;
    gWidgetUnk10[ i ]->i03 = a3;
    gWidgetUnk10[ i ]->i04 = a7;
    gWidgetUnk10[ i ]->i05 = a8;
    gWidgetUnk10[ i ]->itg = a6;
    gWidgetUnk10[ i ]->i07 = a4;
    gWidgetUnk10[ i ]->i08 = a5;
    return i;
}

int WidgetUnk14( int idx )
{
    if( idx > 31 || !gWidgetUnk10[ idx ] ) return 0;
    dbg_free( gWidgetUnk10[ idx ] );
    gWidgetUnk10[ idx ] = NULL;
    return 1;
}

void WidgetUnk15()
{
    int i;

    for( i = 0; i != 32; i++ ){
        if( gWidgetUnk10[ i ] ) WidgetWinRegisterUpdate( gWidgetUnk10[ i ] );
    }
}

void WidgetUnk16()
{
    if( gWidgetUnk13.data ){
        dbg_free( gWidgetUnk13.data );
        gWidgetUnk13.data = NULL;
    }
    if( gWidgetUnk13.i02 ){
        dbg_free( gWidgetUnk13.i02 );
        gWidgetUnk13.i02 = NULL;
    }
    memset( &gWidgetUnk13, 0, sizeof( Widget02_t ) );
    gWidgetUnk12 = 0;
}

void WidgetUnk17()
{
    memset( gWidgetUnk10, 0, 32 * 4 );
    gWidgets = 0;
    gWidgetTableSize = 0;
    gWidgetEvQe = 0;
    gWidgetEvQeSize = 0;
    WidgetUnk16();
}

void WidgetUnk18()
{
    if( gWidgets ) dbg_free( gWidgets );
    gWidgetTableSize = 0;
    gWidgets = 0;
    if( gWidgetEvQe ) dbg_free( gWidgetEvQe );
    gWidgetEvQe = 0;
    gWidgetEvQeSize = 0;
    WidgetUnk16();
}

void WidgetUnk19()
{
    VidRect_t Area;
    char *Surface, *surf;
    int Width;

    if( !gWidgetUnk12 ) return;
    WinGetWidth( gWidgetUnk13.WinId );
    Surface = WinGetSurface( gWidgetUnk13.WinId );
    surf = &Surface[ gWidgetUnk13.Area.tp + gWidgetUnk13.Area.lt ];
    Width = WinGetWidth( gWidgetUnk13.WinId );
    ScrCopy( gWidgetUnk13.data, gWidgetUnk13.Area.rt, gWidgetUnk13.Area.bm, gWidgetUnk13.Area.rt, surf, Width );
    ScrCopy( gWidgetUnk13.i02, gWidgetUnk13.i04, gWidgetUnk13.Area.bm, gWidgetUnk13.Area.rt, surf, WinGetWidth( gWidgetUnk13.WinId ) );
    Area.lt = gWidgetUnk13.Area.lt;
    Area.tp = gWidgetUnk13.Area.tp;
    Area.rt = gWidgetUnk13.Area.rt + gWidgetUnk13.Area.lt;
    Area.bm = gWidgetUnk13.Area.bm + gWidgetUnk13.Area.tp;
    WinAreaUpdate( gWidgetUnk13.WinId, &Area );
}

void WidgetUnk20( int a1, int a2, int a3 )
{
    if( !gWidgetUnk12 ) return;
    gWidgetUnk13.i04 = a1;
    gWidgetUnk13.i05 = a1;
    gWidgetUnk13.i06 = a2;
    WidgetUnk19();
}

void WidgetUnk21( float a1, float a2, int a3, int a4 )
{
    if( !gWidgetUnk12 ) return;    
    gWidgetUnk13.i04 = gWidgetUnk13.i05 = lround( gWidgetUnk13.Area.rt * a2 );
    gWidgetUnk13.i06 = lround( gWidgetUnk13.i05 );
    WidgetUnk19();
    SoundUpdateAll();    
}

void WidgetUnk22( float a1, int a2, int a3 )
{
    if( !gWidgetUnk12 ) return;    
    gWidgetUnk13.i04 = gWidgetUnk13.i05 + lround( (gWidgetUnk13.i06 - gWidgetUnk13.i05) * a1 );
    WidgetUnk19();
    SoundUpdateAll();    
}

void WidgetUnk23( int WinId, char *fname1, char *fname2, int a4, int lt, int tp )
{
    int h2,w2,h1,w1;

    WidgetUnk16();
    gWidgetUnk13.data = DataFileUnk06( fname1, &w1, &h1 );
    gWidgetUnk13.i02  = DataFileUnk06( fname2, &w2, &h2 );
    if( w1 == w2 && h2 == h1 ){
        gWidgetUnk13.Area.rt = w1;
        gWidgetUnk13.Area.bm = h1;
        gWidgetUnk13.Area.lt = lt;
        gWidgetUnk13.Area.tp = tp;
        gWidgetUnk13.WinId = WinId;
        WidgetUnk20( 0, 0, 1004 );
        gWidgetUnk12 = 1;
    } else {
        WidgetUnk16();
        eprintf( "status bar dimensions not the same\n" );
    }
}

void WidgetUnk24( int *a1, int *a2, int *a3, int *a4, int a5 )
{
    if( gWidgetUnk12 ){
        *a2 = gWidgetUnk13.i04;
        *a3 = gWidgetUnk13.i05;
        *a4 = gWidgetUnk13.i06;
    } else {
        *a4 = -1;
        *a3 = -1;
        *a2 = -1;
    }
}

void WidgetUnk25( int result, int a2, int a3, int a4, int a5 )
{
    if( !gWidgetUnk12 ) return;
    gWidgetUnk13.i05 = a3;
    gWidgetUnk13.i06 = a4;
    gWidgetUnk13.i04 = a2;
}


