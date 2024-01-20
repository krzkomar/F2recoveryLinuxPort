#pragma once

#define TYPEH( n )	((n) >> 8)
#define ARG_TYPE( Type, TypeEnum, ErrStr )	if( ( Type & 0xF7FF) != TypeEnum ) IntpError( ErrStr );

extern Sound_t *gScpIfcSounds[32];

//int ScpIfcNevsUpdate();
//int ScpIfcSoundDelete( int );#include "FrameWork.h"
//Sound_t *gScpIfcSounds[ 32 ];
//Pal8_t gScpIfcUnk02[ 256 ];
//int gScpIfcUnk01;
//int gScpIfcUnk07;
//Intp_t *gScpIfcUnk08;
//int gScpIfcUnk05;
//char gScpIfcUnk03[ 100 ];
//char gScpIfcUnk04[ 100 ];

/*************************************************/

void ScpIfc_FillWin3x3( Intp_t *scr );
void ScpIfc_MessageFormat( Intp_t *scr ); // 8073
void ScpIfc_Print( Intp_t *scr ); // 8072
void ScpIfc_SelectFileList( Intp_t *scr ); // 809F
void ScpIfc_TokenizeString( Intp_t *scr ); // 80A0
void ScpIfc_PrintRect( Intp_t *scr ); // 8074
void ScpIfc_Select( Intp_t *scr );
void ScpIfc_Display( Intp_t *scr );
void ScpIfc_DisplayRaw( Intp_t *scr );
void ScpIfcPal( Pal8_t *Pal, Pal8_t *PalStop, float fsteps , int Update );
void ScpIfcPalA( Pal8_t *a1, Pal8_t *a2, float a3 );
int ScpIfcUnk01();
void ScpIfcUnk02( float a1 );
void ScpIfcUnk03( float a1 );
void ScpIfcUnk04( float a1 );
void ScpIfcUnk05( float a1 );
void ScpIfc_FadeIn( Intp_t *scr );
void ScpIfc_FadeOut( Intp_t *scr );
int ScpIfcUnk06();
void ScpIfc_SettingMovieFlag( Intp_t *scr );
void ScpIfc_PlayMovie1( Intp_t *scr );
void ScpIfc_PlayMovie( Intp_t *scr );
void ScpIfc_MovieStat( Intp_t *scr );
void ScpIfc_DeleteRegion( Intp_t *scr );
void ScpIfc_ActivateRegion( Intp_t *scr );
void ScpIfc_CheckRegion( Intp_t *scr );
void ScpIfc_SetRegion( Intp_t *scr );
void ScpIfc_SetRegionProc( Intp_t *scr );
void ScpIfc_SetRightButtonProc( Intp_t *scr );
void ScpIfc_CreateWindow( Intp_t *scr );
void ScpIfc_ResizeWindow( Intp_t *scr );
void ScpIfc_ScaleWindow( Intp_t *scr );
void ScpIfc_DeleteWindow( Intp_t *scr );
void ScpIfc_SayStart( Intp_t *scr );
void ScpIfc_StartDialog( Intp_t *scr );
void ScpIfc_SetTitle( Intp_t *scr );
void ScpIfc_SayGotoReply( Intp_t *scr );
void ScpIfc_SayOption( Intp_t *scr );
void ScpIfc_SayReply( Intp_t *scr );
int ScpIfcUnk08( Intp_t *scr );
void ScpIfc_SayEnd( Intp_t *scr );
void ScpIfc_unk09( Intp_t *scr );
void ScpIfc_SayQuit( Intp_t *scr );
int ScpIfcGetTimeOut();
int ScpIfcSetTimeOut( int time );
void ScpIfc_SayMsgTimeout( Intp_t *scr );
void ScpIfc_SayMessage( Intp_t *scr );
void ScpIfc_GotoXY( Intp_t *scr );
void ScpIfc_AddButtonFlag( Intp_t *scr );
void ScpIfc_AddRegionFlag( Intp_t *scr );
void ScpIfc_AddButton( Intp_t *scr );
void ScpIfc_AddButtonText( Intp_t *scr );
void ScpIfc_AddButtonGFX( Intp_t *scr );
void ScpIfc_AddButtonProc( Intp_t *scr );
void ScpIfc_AddButtonRightProc( Intp_t *scr );
void ScpIfc_ShowWin( Intp_t *scr );
void ScpIfc_DeleteButton( Intp_t *scr );
void ScpIfc_FillWin( Intp_t *scr );
void ScpIfc_FillRect( Intp_t *scr );
void ScpIfc_MouseRedraw( Intp_t *scr );
void ScpIfc_MouseDrawCursor();
void ScpIfc_MouseShape( Intp_t *scr );
void ScpIfc_SetGlobalMouseFunc( Intp_t *scr );
void ScpIfc_DisplayGFX( Intp_t *scr );
void ScpIfc_LoadPaletteTable( Intp_t *scr );
void ScpIfc_AddNamedEvent( Intp_t *scr );
void ScpIfc_AddNamedHandler( Intp_t *scr );
void ScpIfc_ClearNamed( Intp_t *scr );
void ScpIfc_SignalNamed( Intp_t *scr );
void ScpIfc_AddKey( Intp_t *scr );
void ScpIfc_DeleteKey( Intp_t *scr );
void ScpIfc_RefreshMouse( Intp_t *scr );
void ScpIfc_SetFont( Intp_t *scr );
void ScpIfc_SetTextFlags( Intp_t *scr );
void ScpIfc_SetTextColor( Intp_t *scr );
void ScpIfc_SayOptionColor( Intp_t *scr );
void ScpIfc_SayReplytColor( Intp_t *scr );
void ScpIfc_SetTextHighlightColor( Intp_t *scr );
void ScpIfc_SayReplyWindow( Intp_t *scr );
void ScpIfc_SayReplyFlags( Intp_t *scr );
void ScpIfc_SayOptionFlags( Intp_t *scr );
void ScpIfc_SayOptionWindow( Intp_t *scr );
void ScpIfc_SayBorder( Intp_t *scr );
void ScpIfc_SayScrollUp( Intp_t *scr );
void ScpIfc_SayScrollDn( Intp_t *scr );
void ScpIfc_SaySetSpacing( Intp_t *scr );
void ScpIfc_Restart( Intp_t *scr );
void ScpIfcSoundCb( int *p, int a2 );
int ScpIfcSoundDelete( int Idx );
void ScpIfcDeleteSounds();
unsigned int ScpIfcSounPlay( short Flags, char *fname );
int ScpIfcSoundPause( int Idx );
int ScpIfcSoundRewind( int Idx );
int ScpIfcSounResume( int Idx );
void ScpIfc_SoundPlay( Intp_t *scr );
void ScpIfc_SoundPause( Intp_t *scr );
void ScpIfc_SoundResume( Intp_t *scr );
void ScpIfc_SoundStop( Intp_t *scr );
void ScpIfc_SoundRewind( Intp_t *scr );
void ScpIfc_SoundDelete( Intp_t *scr );
void ScpIfc_SetOneOptPause( Intp_t *scr );
int ScpIfcNevsUpdate();
int ScpUnk02( unsigned int a1 );
void ScpIfcInit();


