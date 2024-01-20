#include "FrameWork.h"
#include "GVars.h"

int gScptUnk02;
Scpt01_t gScptUnk15;
Combat02_t gScptUnk114;

int gScptUnk18;

Msg_t gScptMsgBook[ 1450 ];
Msg_t gScptMsg;
char gScptTimeStr[ 7 ];

char gScptFileName[ 20 ];

int gScptUnk01 = 1;
int gScptUnk13 = 0;
int gScptUnk17 = 0;
char gScptUnk22[6] = {0};
int gScptUnk24 = 0;
int gScptUnk23 = 0;


char *gScptTypeName[ SCR_TYPES ] = { "s_system", "s_spatial", "s_time", "s_item", "s_critter" };

int gScptCount = 0;

int gScptUnk10 = 0;
ScptCache_t *gScptUnk50 = NULL;
int gScptUnk51 = 0;
int gScptUnk52 = 1;

ScptBook_t gScrScripts[ SCR_TYPES ] = { NULL };
char *gScptPath = "scripts/";

int gScptEnable = 0;
int gScptUnk07 = 0;
int gScptUnk03 = 0;
unsigned int gScptInGameDekaSeconds = 302400;


ScptVars_t *gScptLocVarTable = NULL;
int gScptFiles = 0;
int gScptObjIds = 4;

int gScptUnk14 = -1;
int *gScptLocalVars = NULL;

int gScptLocalVarsCnt = 0;

Scpt_t *ScptUnk21();

//char *ScptGetDialog( int MsgPage, int MsgId, int a3 );

int gScptUnk109 = 0;
int gScptUnk110 = 0;
int gScptUnk111 = 0;
int gScptUnk113 = 0;
int gScptUnk112 = 0;
int gScptCalendarDays[ 13 ] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };

int gScptUnk02;
int gScptUnk115;
int gScptUnk118;
int gScptUnk121;
int gScptUnk122;
int gScptUnk116;
int gScptUnk117;
int gScptUnk117;
Obj_t *gScptUnk102;
Obj_t *gScptUnk100;
Obj_t *gScptUnk101;
Obj_t *gScptUnk119;
Obj_t *gScptUnk120;
Msg_t gScptMsgBook[1450];
Msg_t gScptMsg;
char gScptTimeStr[7];
char gScptUnk104[21];
int gScptUnk107;
int gScptUnk108;
char gScptFileName[20];


#define P_PROC_COUNT	28
const char *gScptP_proc[ P_PROC_COUNT ] = {
    "no_p_proc",
    "start",
    "spatial_p_proc",
    "description_p_proc",
    "pickup_p_proc",
    "drop_p_proc",
    "use_p_proc",
    "use_obj_on_p_proc",
    "use_skill_on_p_proc",
    "none_x_bad",
    "none_x_bad",
    "talk_p_proc",
    "critter_p_proc",
    "combat_p_proc",
    "damage_p_proc",
    "map_enter_p_proc",
    "map_exit_p_proc",
    "create_p_proc",
    "destroy_p_proc",
    "none_x_bad",
    "none_x_bad",
    "look_at_p_proc",
    "timed_event_p_proc",
    "map_update_p_proc",
    "push_p_proc",
    "is_dropping_p_proc",
    "combat_is_starting_p_proc",
    "combat_is_over_p_proc"
};

/*******************************************************************************************/

int ScptGetGameDekaSeconds()
{
    return gScptInGameDekaSeconds;
}

void ScptGetGameDate( int *pMonth, int *pDay, int *pGYear)
{
    int y,m,d,days;

    y = (gScptInGameDekaSeconds / 864000 + 24) / 365 + 2241;    
    d = (gScptInGameDekaSeconds / 864000 + 24) % 365;
    for( m = 6; d >= (days = gScptCalendarDays[ m ]); ){
        m++;
        d -= days;
        if( m >= 12 ){
            y++;
            m = 0;
        }
    }
    if( pDay ) *pDay = d + 1;
    if( pMonth ) *pMonth = m + 1;
    if( pGYear ) *pGYear = y;
}

int ScptGetGameTime()
{
  return 100 * ((gScptInGameDekaSeconds / 600) / 60 % 24) + (gScptInGameDekaSeconds / 600) % 60; // h * 100 + min
}

char *ScptGetGameTimeStr()
{
    sprintf( gScptTimeStr, "%d:%02d", (gScptInGameDekaSeconds / 600) / 60 % 24, ( gScptInGameDekaSeconds / 600 ) % 60 );
    return gScptTimeStr;
}

void ScptSetGameTime( int time )
{
    if( !time ) time = 1;
    gScptInGameDekaSeconds = time;
}

void ScptSetSeconds( int sec )
{
    gScptInGameDekaSeconds = 10 * sec;
    if( !gScptInGameDekaSeconds ) gScptInGameDekaSeconds = 1;
}

void ScptTimeAdvance( int dsec )
{
    gScptInGameDekaSeconds += dsec;
    if( (gScptInGameDekaSeconds / GYEAR) >= 13 ){
        EndGameSetupDeathEnding( 2 );
        gMenuEscape = 2;
    }
}

void ScptTimeCap2( int dsec )
{
    gScptInGameDekaSeconds += 10 * dsec;
    if( ScptGetGameDekaSeconds() / GYEAR >= 13 ){
        EndGameSetupDeathEnding( 2 );
        gMenuEscape = 2;
    }
}

int ScptClockInit()
{
    if( EvQeSchedule( 10 * (60 * (60 - (gScptInGameDekaSeconds / 600) % 60 - 1) + 3600 * (24 - (gScptInGameDekaSeconds / 600) / 60 % 24 - 1) + 60), 0, 0, 4 ) == -1 ) return -1;
    if( !gMap.Name[ 0 ] ) return 0;
    if( EvQeSchedule( 600, 0, 0, 12 ) == -1 ) return -1;
    return 0;
}

int ScptUnk146()
{
    int v4;

    v4 = -1;
    eprintf( "\nQUEUE PROCESS: Midnight!" );
    if( GMovieGetError() ) return 0;
    UseUnjamAll();
    if( !GdialogUnk01() ) ScptPlayMovieEv( &v4, -1 );
    CritterRadSetDose( gObjDude );
    EvQeRun( 4, 0 );
    ScptClockInit();
    if( v4 != -1 ) return 1;
    return 0;
}

int ScptPlayMovieEv( int *pMovieId, int WinId )
{
    int Arroyo, MovieId, MovieFlags, GameOver;

    MovieFlags = 11; GameOver = 0; Arroyo = 0; MovieId = -1;
    if( GlobVarGet( GVAR_ENEMY_ARROYO ) ){
        MovieId = MOVIE_Arroyo_Failure; 
        MovieFlags = 5; GameOver = 1;
    } else if( gScptInGameDekaSeconds < 360 && GlobVarGet( GVAR_FALLOUT_2 ) < 3 ){
        if( gScptInGameDekaSeconds < 270 || GlobVarGet( GVAR_FALLOUT_2 ) == 3 ){
            if( gScptInGameDekaSeconds >= 180 && GlobVarGet( GVAR_FALLOUT_2 ) != 3 ){
                Arroyo = 1; MovieId = MOVIE_Arroyo_Dream_2;
            } else {
                if( gScptInGameDekaSeconds >= 90 && GlobVarGet( GVAR_FALLOUT_2 ) != 3 ){ Arroyo = 1; MovieId = MOVIE_Arroyo_Dream_1; }
            }
        } else {
            Arroyo = 1; MovieId = MOVIE_Arroyo_Dream_3;
        }
    } else if( !GMovieEnabled( MOVIE_Arroyo_Dream_4 ) ){
        Arroyo = 1;
        WmUnk05( 0, 0, 1 );
        WmUnk05( 22, 1, 1 );
        WmUnk04( 22, 2 );
    }
    if( MovieId == -1 || GMovieEnabled( MovieId ) ){
        MovieId = -1;
    } else {
        if( WinId != -1 ) WinUpdateDirty( WinId );
        GMoviePlay( MovieId, MovieFlags );
        if( WinId != -1 ) WinMoveTop( WinId );
        if( Arroyo == 1 ) GlobVarSet( GVAR_TOWN_REP_ARROYO, GlobVarGet( GVAR_TOWN_REP_ARROYO ) - 15 );
    }
    if( GameOver == 1 ) 
	gMenuEscape = 2;
    else
        TileUpdate();
    if( pMovieId ) * pMovieId = MovieId;
    return 0;
}

int ScptUnk144()
{
    ScptExecMapUpdateScripts( 23 );
    EvQeRun( 12, 0 );
    if( !gMap.Name[0] ) return 0;
    if( EvQeSchedule( 600, 0, 0, 12 ) == -1 ) return -1;
    return 0;    
}

int ScptNewObjId()
{
    Obj_t *p;

    do{
        gScptObjIds++;
        for( p = ObjGetObjectFirst(); p; p = ObjGetObjectNext() ){
            if( gScptObjIds == p->TimeEv ) break;
        }
    } while ( p );
    if( gScptObjIds >= 18000 ) eprintf( "\n    ERROR: new_obj_id() !!!! Picked PLAYER ID!!!!" );
    return gScptObjIds++;
}

Obj_t *ScptUnk142( int a1 )
{
    Scpt_t *scr;
    Obj_t *p;
    int v3;

    if( ScptPtr( a1, &scr ) == -1 ) return NULL;    
    for( p = ObjGetObjectFirst(); p; p = ObjGetObjectNext() ){
        v3 = ( scr->TimeEv ) ? (p == scr->TimeEv) : (p->TimeEv == scr->i08);
        if( v3 ) return p;
    }    
    return NULL;
}

int ScptGetActionSource( Intp_t *a1 )
{
    ScptCache_t *j;
    int i,a2;

    for( i = 0; i < 5; i++ ){
        for( j = gScrScripts[ i ].First; j; j = j->Next ){
            a2 = 0;
            for(; a2 < j->ScptUsed; a2++ ){
                if( a1 == j->Script[ a2 ].i07 ) break;
            }
            if( a2 < j->ScptUsed && a1 == j->Script[ a2 ].i07 ) break;
        }
        if( j && a1 == j->Script[ a2 ].i07 ) break;
    }
    if( !j ) return -1;
    return ( a1 == j->Script[ a2 ].i07 ) ? j->Script[ a2 ].Id : -1;
}

Obj_t *ScptGetSelfObj( Intp_t *a1 )
{
    unsigned int v3;
    int v6;
    Scpt_t *v7;
    Obj_t *obj;
    Scpt_t *a2, *scr;

    obj = 0;
    v3 = ScptGetActionSource( a1 );
    if( ScptPtr( v3, &a2 ) == -1 ) return NULL;
    if( a2->TimeEv ) return a2->TimeEv;
    if( OBJTYPE( v3 ) != TYPE_CRIT ) return NULL;
    ObjCreate( &obj, ArtMakeId( 6, 3, 0, 0, 0 ), -1 );
    ObjUnk33( obj, 0 );
    ObjSetPlayer( obj, 0 );
    obj->ScrId = v3;
    if( ScptPtr( v3, &scr ) == -1 ) return ART_NULL;
    obj->TimeEv = ScptNewObjId();
    scr->i08 = obj->TimeEv;    
    scr->TimeEv = obj;
    for( v6 = 0; v6 >= 3; v6++ ){        
	for( v7 = ScptUnk20( v6 ); v7; v7 = ScptUnk21() ){
	    if( v7 != a2 ) continue;
	    ObjMoveToTile( obj, a2->HexOrTimer & 0x3FFFFFF, v6, 0 );
	    return obj;
	}        	
    }
    return obj;
}

char *ScptUnk139( int a1 )
{
    Scpt_t *v4;

    if( ScptPtr( a1, &v4 ) == -1 ) return "<INVALID>";
    if( v4->i07 ) return v4->i07->FileName;
    gScptUnk104[ 0 ] = '\0';
    return ( ScptGetScriptFname( v4->LocVarId & 0xFFFFFF, gScptUnk104 ) == -1 ) ? "<INVALID>" : gScptUnk104;
}

int ScptUseObject( int Pids, Obj_t *critter, Obj_t *item )
{
    Scpt_t *scr;

    if( ScptPtr( Pids, &scr ) == -1 ) return -1;
    scr->crit = critter;
    scr->item = item;
    return 0;
}

int ScptSetArg( int ScriptId, int ArgVal )
{
    Scpt_t *scr;

    if( ScptPtr( ScriptId, &scr ) == -1 ) return -1;
    scr->ArgVal = ArgVal;
    return 0;    
}

int ScptUnk136( int a1, int a2 )
{
    Scpt_t *scr;

    if( ScptPtr( a1, &scr ) == -1 ) return -1;
    scr->i17 = a2;
    return 0;        
}

Intp_t *ScptLoad( char *fname )
{
    char stmp[ 256 ];

    strcpy( stmp, gProtoDataFilePath );
    strcpy( &stmp[ strlen( stmp ) ], gScptPath );
    strcpy( &stmp[ strlen( stmp ) ], fname );
    strcpy( &stmp[ strlen( stmp ) ], ".int" );
    return IntpLoad( stmp );
}

void ScptTaskCb( int arg )
{
    int Time, i;

    if( !gScptUnk108 ){
        gScptUnk107 = TimerGetTime();
        gScptUnk108 = 1;
    }
    Time = TimerGetTime();
    i = 1;
    if( gScptEnable == 1 ){
        gScptUnk107 = Time;
        while( --i != -1 ) SciUnk19();
    }
    WinRun();
    if( gScptEnable == 1 && gScptEnable == gScptUnk07 && !GdialogUnk01() ){
        ScptUnk133();
        ScptUnk132();
    }
}

int ScptUnk133()
{
    ScptCache_t *p;
    int result, v1, ScrCnt, v5;

    v1 = 12;
    if( GdialogUnk01() || IN_COMBAT ) return 1;
    ScrCnt = 0;
    for( p = gScrScripts[ SCR_TYPE_CRT ].First; p; p = p->Next ) ScrCnt += p->ScptUsed;
    v5 = gScptUnk109 + 1;
    if( v5 >= ScrCnt ) v5 = 0;
    gScptUnk109 = v5;

    if( v5 >= ScrCnt ) return ScrCnt;    
    if( IN_COMBAT ) v1 = 13;
    result = v5 / 16;
    for( p = gScrScripts[ SCR_TYPE_CRT ].First; p; p = p->Next ){
        if( --result <= 0 ) break;
    }
    if( p ){
        gScptUnk109 = v5;
        result = ScptExecScriptProc( p->Script[ v5 % 16 ].Id, v1 );
        v5 = gScptUnk109;
    }
    gScptUnk109 = v5;        
    return result;
}

void ScptUnk132()
{
    int Time, v1;

    Time = TimerGetTime();
    v1 = 0;
    if( GlobUnk01() != 4 && TimerDiff( Time, gScptUnk111 ) >= 30000 ){
        gScptUnk111 = Time;
        ScptExecMapUpdateScripts( 23 );
    }
    if( TimerDiff( Time, gScptUnk110 ) >= 100 ){
        gScptUnk110 = Time;
        if( !IN_COMBAT ) gScptInGameDekaSeconds++;
        v1 = 1;
    }
    if( !v1 ) return;
    while( !EvQeIsEmpty() ){
        if( ScptGetGameDekaSeconds() < EvQeGetTime() ) break;
        EvQeTrig();
    }
}

void ScptUnk131( int a1, int a2 )
{
    gScptUnk112 = a2;
    gScptUnk113 = a1;
}

int ScptUnk130( int a1, Scpt_t *a2 )
{
    return a1 == gScptUnk113 && a2->i01 == gScptUnk112;
}

int ScptAddTimerEvent( int ScrId, int a1, int a2 )
{
DD
/*
    int *p;
    Scpt_t *scr;

    p = Malloc(8u);
    if( !p ) return -1;
    *p = a1;
    p[1] = a2;
    if( ScptPtr(a1, &scr) != -1 && EvQeSchedule(a1, scr->TimeEv, p, 3) != -1 ) return 0;
    Free(p);
*/
    return -1;
}

int ScptUnk128( xFile_t *a1, int *a2 )
{
    if( dbputBei( a1, a2[ 0 ] ) == -1 ) return -1;
    if( dbputBei( a1, a2[ 1 ] ) == -1 ) return -1;
    return 0;
}

int ScptLoadUnk17( xFile_t *fh, int **ptr )
{
    int *p;

    if( !(p = Malloc( sizeof( int[ 2 ] ) )) ) return -1;
    if( dbgetBei( fh, &p[ 0 ] ) == -1 ) return -1;
    if( dbgetBei( fh, &p[ 1 ] ) == -1 ) return -1;
    *ptr = p;
    return 0;
}

int ScptUnk126( int Unused, Obj_t *obj )
{
    Scpt_t *scr;

    if( ScptPtr( obj->TimeEv, &scr ) != -1 ){
        scr->ArgVal = obj->GridId;
        ScptExecScriptProc( obj->TimeEv, 22 );
    }
    return 0;
}

void ScptUnk125()
{
    gScptUnk02 = 0;
}

void ScptUnk124( Scpt_t *a1 )
{
    if( (gScptUnk02 & 0x01) && (gScptUnk15.obj == a1->TimeEv) )  gScptUnk02 &= ~0x401;
}

void ScptProcess()
{
    int v0;
    Obj_t *p;
    Obj_t *i;
    Map01_t v13;
    short a4;
    int a3;
    int MapId;

    if( gScptUnk02 ){
        if( gScptUnk02 & 0x01 ){
            v0 = ActionUnk13();
            if( !v0 ){
                gScptUnk02 &=  ~0x401;
                memcpy( &gScptUnk114, &gScptUnk15, sizeof( gScptUnk114 ) );
                if( gScptUnk02 & 0x40 ){
                    gScptUnk02 &= ~0x40;
                    CombatStart( 0 );
                } else {
                    CombatStart( &gScptUnk114 );
                    memset( &gScptUnk114, 0, sizeof( gScptUnk114 ) );
                }
            }
        }
        if( gScptUnk02 & 0x02 ){ gScptUnk02 &= ~0x02; WmUnk10( v0 ); }
        if( gScptUnk02 & 0x04 ){ gScptUnk02 &= ~0x04; WmMenu(); }
        if( gScptUnk02 & 0x08 ){
            MapId = gMap.MapId;
            a3 = gScptUnk118;
            a4 = -1;
            gScptUnk02 &= ~0x08;
            if( ElevatorMenu( gScptUnk115, &MapId, &a3, &a4 ) != -1 ){
                AutomapSave();
                if( MapId == gMap.MapId ){
                    if( a3 == gCurrentMapLvl ){
                        AnimClear( gObjDude );
                        ObjSetRotation( gObjDude, 2, NULL );
                        UseUnk46( gObjDude, a4, a3, 0 );
                    } else {
                        for( i = ObjGetVisibleObjectFirst( gObjDude->Elevation ); i; i = ObjGetVisibleObjectNext() ){
                            if( (i->Pid >> 24) == 2 && (i->Pid == 0x2000099 || i->Pid == 0x20001A5 || i->Pid == 0x20001D6 ) && TileGetDistance( i->GridId, gObjDude->GridId ) <= 4 ) break;
                        }
                        AnimClear( gObjDude );
                        ObjSetRotation( gObjDude, 2, 0 );
                        UseUnk46( gObjDude, a4, a3, 0 );
                        if( i ){
                            ObjSetFrame( i, 0, 0 );
                            ObjMoveToTile( i, i->GridId, i->Elevation, NULL );
                            i->Flags &= ~0xA0000010;
                            i->Critter.State.Reaction &= ~0x01;
                            ObjLightGrid();
                        } else {
                            eprintf( "\nWarning: Elevator: Couldn't find old elevator doors!" );
                        }
                    }
                } else {
                    for( p = ObjGetVisibleObjectFirst( gObjDude->Elevation ); p; p = ObjGetVisibleObjectNext() ){
                        if( (p->Pid >> 24) == 2 && (p->Pid == 0x2000099 || p->Pid == 0x20001A5 || p->Pid == 0x20001D6) && TileGetDistance( p->GridId, gObjDude->GridId ) <= 4 ) break;
                    }
                    if( p ){
                        ObjSetFrame( p, 0, 0 );
                        ObjMoveToTile( p, p->GridId, p->Elevation, NULL );
                        p->Flags &= ~0xA0000010;
                        p->Critter.State.Reaction &= ~0x01;
                        ObjLightGrid();
                    } else {
                        eprintf( "\nWarning: Elevator: Couldn't find old elevator doors!" );
                    }
                    v13.MapId = MapId;
                    v13.Frame = a3;
                    v13.PosY = a4;
                    v13.Orientation = 2;
                    MapSetPos( &v13 );
                }
            }
        }
        if( gScptUnk02 & 0x10 ){ gScptUnk02 &= ~0x10; ActionExplode( gScptUnk121, gScptUnk122, gScptUnk116, gScptUnk117, 0, 1 ); }
        if( gScptUnk02 & 0x20 ){ gScptUnk02 &= ~0x20; GdialogEnter( gScptUnk102, 0 ); }
        if( gScptUnk02 & 0x80 ){ gScptUnk02 &= ~0x80; EndGameRun(); EndGameMoviePlay(); }
        if( gScptUnk02 & 0x100 ){ gScptUnk02 &= ~0x100; InvMenuSteal( gScptUnk100, gScptUnk101 ); }
        if( gScptUnk02 & 0x200 ){ gScptUnk02 &= ~0x200; InvStealAttempt( gScptUnk119, gScptUnk120 ); }
    }
}

void ScptUnk122()
{
    Obj_t *i;
    Map01_t v9;
    short PosY;
    int Lvl, pMapId;

    if( gScptUnk02 & 8 ){
        pMapId = gMap.MapId;
        PosY = -1;
        Lvl = gScptUnk118;
        if( ElevatorMenu( gScptUnk115, &pMapId, &Lvl, &PosY ) != -1 ){
            AutomapSave();
            if( pMapId == gMap.MapId ){
                if( Lvl == gCurrentMapLvl ){
                    AnimClear( gObjDude );
                    ObjSetRotation( gObjDude, 2, NULL );
                    UseUnk46( gObjDude, PosY, Lvl, 0 );
                } else {
                    for( i = ObjGetVisibleObjectFirst( gObjDude->Elevation ); i; i = ObjGetVisibleObjectNext() ){
                        if( (i->Pid >> 24) == 2 && (i->Pid == 0x2000099 || i->Pid == 0x20001A5 || i->Pid == 0x20001D6 ) && TileGetDistance( i->GridId, gObjDude->GridId ) <= 4 ) break;
                    }
                    AnimClear( gObjDude );
                    ObjSetRotation( gObjDude, 2, NULL );
                    UseUnk46( gObjDude, PosY, Lvl, 0 );
                    if( i ){
                        ObjSetFrame( i, 0, 0 );
                        ObjMoveToTile( i, i->GridId, i->Elevation, NULL );
                        i->Flags &= ~0xA0000010;
                        i->Critter.State.Reaction &= ~0x01;
                        ObjLightGrid();
                    } else {
                        eprintf( "\nWarning: Elevator: Couldn't find old elevator doors!" );
                    }
                }
            } else {
                v9.MapId = pMapId;
                v9.Frame = Lvl;
                v9.PosY = PosY;
                v9.Orientation = 2;
                MapSetPos( &v9 );
            }
        }
    }
    if( gScptUnk02 & 0x100 ) InvMenuSteal( gScptUnk100, gScptUnk101 );
    gScptUnk02 = 0;
}

int ScptUnk121( Scpt01_t *a1 )
{
    if( gScptUnk02 & 0x400 ) return -1;
    if( a1 )
        memcpy( &gScptUnk15, a1, sizeof( Scpt01_t ) );
    else
        gScptUnk02 |= 0x40;
    gScptUnk02 |= 0x01;
    return 0;
}

int ScptUnk120( Scpt01_t *scr )
{
    if( scr )
        memcpy( &gScptUnk15, scr, sizeof( Scpt01_t ) );
    else
        gScptUnk02 |= gScptUnk02;
    gScptUnk02 |= 0x401;
    return 0;
}

int ScptUnk119()
{
    if( IN_COMBAT ) gMenuEscape = 1;
    gScptUnk02 |= 0x02;
    return 0;
}

int ScptWorldMap()
{
    if( IN_COMBAT ) gMenuEscape = 1;
    gScptUnk02 |= 0x04;
    return 0;
}

int ScptRequestElevator( Scpt_t *a1, int Reaction )
{
    Obj_t *p;
    int i, j, n;
    
    n = a1->i01;
    if( n == -1 ){
	eprintf( "\nError: scripts_request_elevator! Bad tile num" );
	return -1;
    }
    n -= 1005;
    for( i = -5; i < 5; i++, n += 190 ){	
	for( j = -5; j < 5; j++, n++ ){
	    for( p = ObjGetVisibleObjectFirst( a1->i11 ); p; p = ObjGetVisibleObjectNext() ){
		if( n == p->GridId && p->Pid == 0x200050d ) break;
	    }
	    if( p ) break;
	}
	if( p != 0 ) break;
    }
    if( Reaction == -1 ) return -1;
    gScptUnk02 |= 0x08;
    gScptUnk115 = Reaction;
    gScptUnk118 = gCurrentMapLvl;
    return 0;	
}

void ScptExplosion( unsigned int tilenum, int a2, int a3, int a4 )
{
    gScptUnk02 |= 0x10;
    gScptUnk121 = tilenum;
    gScptUnk122 = a2;
    gScptUnk116 = a3;
    gScptUnk117 = a4;
}

void ScptUnk115( Obj_t *a1 )
{
    gScptUnk102 = a1;
    gScptUnk02 |= 0x20;
}

void ScptSlideShow()
{
    gScptUnk02 |= 0x80;
}

int ScptUnk113( void *a1, void *a2 )
{
    gScptUnk100 = a1;
    gScptUnk101 = a2;
    gScptUnk02 |= 0x100;
    return 0;
}

void ScptUnk112( Obj_t *a1, Obj_t *a2 )
{
    gScptUnk119 = a1;
    gScptUnk120 = a2;
    gScptUnk02 |= 0x200;
}

void ScptUnk111( char *a1 )
{
    strcpy( a1, gProtoDataFilePath );
    strcpy( &a1[ strlen( a1 ) ], gScptPath );
}

int ScptExecScriptProc( int ScriptId, int arg )
{
    int flg, tmp;
    char *s, stmp[ 16 ];
    Scpt_t *scr;

    flg = 0;
    if( !gScptEnable ) return -1;
    if( ScptPtr( ScriptId, &scr ) == -1 ) return -1;
    scr->i18 = 0;
    if( !(scr->Flags & SCR_01) ){
	TimerThrGetTime();
        stmp[ 0 ] = '\0';
        if( ScptGetScriptFname( scr->LocVarId & 0xFFFFFF, stmp ) == -1 ) return -1;
        if( ( s = strchr( stmp, '.' ) ) ) *s = '\0';
        scr->i07 = ScptLoad( stmp );
        if( !scr->i07 ){ eprintf( "\nError: ScptExecScriptProc: script load failed '%s'!", stmp ); return -1; }
        flg = 1;
        scr->Flags |= SCR_01;
    }

    if( !scr->i07 ) return -1;    
    if( scr->i07->Flags & 0x124  ) return 0;    
    tmp = scr->PprocIdx[ arg ];
    if( !tmp ) tmp = 1;
    if( tmp == -1 ) return -1;        
    if( !scr->item ) scr->item = scr->TimeEv;
    scr->Flags |= SCR_04;
    if( flg == 1 ){
        ScptIndexPproc( scr );
        tmp = scr->PprocIdx[ arg ];
        if( !tmp ) tmp = 1;
        scr->i12 = 0;
        SciItpEqAdd( scr->i07 );
        SciOpcodeExec( scr->i07, -1 );
    }
    scr->i12 = arg;
    SciUnk13( scr->i07, tmp );
    scr->crit = NULL;
    return 0;
}

void ScptIndexPproc( Scpt_t *scr )
{
    int i, idx;

    for( i = 0; i < P_PROC_COUNT; i++ ){
        idx = SciGetProcedureIdx( scr->i07, gScptP_proc[ i ] );
        if( idx == -1 ) idx = 0;
        scr->PprocIdx[ i ] = idx;
    }
}

int ScptUnk108( int Pid, int idx )
{
    Scpt_t *scr;

    return ScptPtr( Pid, &scr ) != -1 && scr->PprocIdx[ idx ] != 0;
}

int ScptAppendFileToList( char *fname )
{
    xFile_t *fh;
    char stmp[284];

    strcpy( stmp, gProtoDataFilePath );
    strcpy( &stmp[strlen(stmp)], gScptPath );
    strcpy( &stmp[strlen(stmp)], "scripts.lst" );    
    if( !(fh = dbOpen( stmp, "at" ) ) ) return -1;
    dbseek( fh, 0, 2 );
    if( ScptIsFileOnList( fname, 0 ) ) sprintf( fname, "%08d", gScptCount );
    dbPrintf( fh, "%s.int\n", fname );
    dbClose( fh );
    gScptCount++;
    return 0;
}

int ScptIsFileOnListA( char *fname )
{
    return ScptIsFileOnList( fname, NULL );
}

int ScptIsFileOnList( char *fname, int *pLine)
{
    int i;
    char *s, stmp1[260], stmp2[260], stmp3[80];
    xFile_t *fh;

    if( *fname == '\0' ) return -1;
    strcpy( stmp1, gProtoDataFilePath );
    strcpy( &stmp1[ strlen( stmp1 ) ], gScptPath );
    strcpy( &stmp1[ strlen( stmp1 ) ], "scripts.lst" );    
    if( !( fh = dbOpen( stmp1, "rt" ) ) ) return -1;
    strcpy( stmp3, fname );
    strcpy( &stmp3[ strlen( stmp3 ) ], ".int" );
    StrUpr( stmp3 );
    if( pLine ) *pLine = -1;
    i = 0;
    if( gScptCount > 0 ){
	for(;;){
            if( dbgets( stmp2, 260, fh ) ){
        	if( (s = strchr( stmp2, ' ' )) ) *s = '\0';
        	if( (s = strchr( stmp2, ';' )) ) *s = '\0';
        	StrUpr(stmp2);
        	if( pLine ) ++*pLine;
        	if( !strcmp( stmp2, stmp3 ) ) return -1;
            } else {
    		i++;
        	if( i >= gScptCount ) break;
            }
        }
    }
    dbClose( fh );
    return 0;
}

int ScptLoadFileList()
{
    char *s, stmp1[260], stmp2[260];
    ScptVars_t *p;
    xFile_t *fh;
    int tmp;
    
    strcpy( stmp1, gProtoDataFilePath );
    strcpy( &stmp1[ strlen( stmp1 ) ], gScptPath );
    strcpy( &stmp1[ strlen( stmp1 ) ], "scripts.lst" );    
    if( !(fh = dbOpen( stmp1, "rt" )) ) return -1;
    while( dbgets( stmp2, 260, fh ) ){
        gScptFiles++;
        if( !(p = Realloc( gScptLocVarTable, sizeof( ScptVars_t ) * gScptFiles ) ) ) return -1;
        gScptLocVarTable = p;
        p[ gScptFiles - 1 ].LocalVars = 0;
        StrLwr( stmp2 );        
        if( !(s = strstr( stmp2, ".int" )) ){
            gScptFiles--;
        } else {
            tmp = s - stmp2; // filename length
            if( tmp > 13 ) return -1;
            strncpy( p[ gScptFiles - 1 ].fname, stmp2, tmp );
            p[ gScptFiles - 1 ].fname[ tmp ] = '\0';
        }        
        if( ( s = strstr( stmp2, "#" ) ) ){
            if( (s = strstr( s, "local_vars=" )) ) p[ gScptFiles - 1 ].LocalVars = strtol( s + sizeof( "local_vars=" ) - 1, NULL, 10 );
        }
    }
    dbClose( fh );
    return 0;
}

void ScpVarNamesFree()
{
    if( gScptLocVarTable ){
        Free(gScptLocVarTable);
        gScptLocVarTable = NULL;
    }
    gScptFiles = 0;
}

int ScptUnk102( int a1, int Pid )
{
    Scpt_t *scr;

    if( ScptPtr( Pid, &scr ) == -1 ) return -1;    
    scr->LocVarsCnt = gScptLocVarTable[ a1 ].LocalVars;
    return 0;
}

int ScptGetScriptFname( int ScriptFileIdx, char *stmp )
{
    sprintf( stmp, "%s.int", gScptLocVarTable[ ScriptFileIdx ].fname );
    return 0;
}

int ScptSetDudeScript()
{
    Scpt_t *scr;
    Proto_t *proto;

    proto = NULL;
    if( ScptClearDudeScript() == -1 ) return -1;    
    if( !gObjDude ){ eprintf( "\nError in scr_set_dude_script: obj_dude uninitialized!" ); return -1; }    
    if( ProtoGetObj( 0x1000000, &proto ) == -1 ){ eprintf( "\nError in scr_set_dude_script: can't find obj_dude proto!" ); return -1; }
    proto->AttackMode = 0x4000000;
    UseCreateScript( gObjDude, &gObjDude->ScrId );
    if( ScptPtr( gObjDude->ScrId, &scr ) == -1 ){ eprintf( "\nError in scr_set_dude_script: can't find obj_dude script!" ); return -1; }
    scr->Flags |= SCR_NOTREMOVE | SCR_08;
    return 0;
}

int ScptClearDudeScript()
{
    Scpt_t *scr;

    if( !gObjDude ){ eprintf( "\nError in scr_clear_dude_script: obj_dude uninitialized!" ); return -1; }
    if( gObjDude->ScrId != -1 ){
        if( ScptPtr( gObjDude->ScrId, &scr ) != -1 ) scr->Flags &= ~(SCR_NOTREMOVE | SCR_08);
        ScptRemove( gObjDude->ScrId );
        gObjDude->ScrId = -1;
    }
    return 0;
}

int ScptInit()
{
    int i;

    if( MessageInit(&gScptMsg) != 1 ) return -1;
    for( i = 0; i < 1450; i++ ){
        if( MessageInit( &gScptMsgBook[ i ]) != 1 ) return -1;
    }
    ScptFlush();
    IntpSetErrHandler( TextBoxUnk03 );
    ScpAInitCommands();
    ScptBookReset();
    gScptUnk02 = 0;
    PartyUnk06();
    if( ScptLoadFileList() == -1 ) return -1;
    return 0;    
}

int ScptUnk01()
{
    ScptFlush();
    gScptUnk02 = 0;
    PartyUnk06( 0 );
    return 0;
}

int ScptGameInit()
{
    char stmp[ 260 ];
    int i;

    if( MessageInit( &gScptMsg ) != 1 ){ eprintf( "\nError initing script message file!" ); return -1; }
    for( i = 0; i < 1450; i++ ){
        if( MessageInit( &gScptMsgBook[ i ] ) != 1 ){ eprintf( "\nERROR IN SCRIPT_DIALOG_MSGS!" ); return -1; }            
    }
    sprintf( stmp, "%s%s", gGamePath, "script.msg" );
    if( MessageLoad( &gScptMsg, stmp ) != 1 ){ eprintf("\nError loading script message file!"); return -1; }
    gScptEnable = 1;
    gScptUnk03 = 1;
    gScptInGameDekaSeconds = 1;
    ScptSetGameTime( 302400 );
    InpTaskStart( ScptTaskCb );
    if( ScptSetDudeScript() == -1 ) return -1;
    gScptUnk52 = 1;
    gScptUnk02 = 0;
    return 0;
}

int ScptGameReset()
{
    eprintf( "\nScripts: [Game Reset]" );
DD
return 0;
    ScptReset();
DD
    ScptGameInit();
DD
    PartyUnk06();
DD
    ScptDeleteAll();
DD
    return (ScptSetDudeScript() != -1) - 1;
}

int ScptClose()
{
    gScptEnable = 0;
    gScptUnk07 = 0;
    if( MessageClose( &gScptMsg ) != 1 ){
        eprintf( "\nError exiting script message file!" );
        return -1;
    }
    ScptFlush();
    ScptDeleteAll();
    SciUnk01();
    SciUnk20();
    InpTaskStop( ScptTaskCb );
    gScptUnk02 = 0;
    if( gScptLocVarTable ){
        Free( gScptLocVarTable );
        gScptLocVarTable = NULL;
    }
    gScptFiles = 0;
    return 0;
}

int ScptMsgFree()
{
    int i;

    for( i = 0; i < 1450; i++ ){
        if( gScptMsgBook[i].Count ){
            if( MessageClose( &gScptMsgBook[i] ) != 1 || MessageInit( &gScptMsgBook[i] ) != 1 ){
                eprintf( "\nERROR in scr_message_free!" );
                return -1;
            }
        }
    }
    return 0;
}

int ScptReset()
{
    gScptUnk03 = 0;
    gScptEnable = 0;
    gScptUnk07 = 0;
    ScptMsgFree();
    ScptFlush();
    SciUnk20();
    InpTaskStop( ScptTaskCb );
    MessageClose( &gScptMsg );
    if( ScptClearDudeScript() == -1 ) return -1;
    gScptUnk02 = 0;
    return 0;    
}

int ScptEnable()
{
    if( !gScptUnk03 ) return -1;
    gScptUnk07 = 1;
    gScptEnable = 1;
    return 0;
}

int ScptDisable()
{
    gScptEnable = 0;
    return 0;
}

void ScptEnableUnk07()
{
    gScptUnk07 = 1;
}

void ScptDisableUnk07()
{
    gScptUnk07 = 0;
}

int ScptSaveVariables( xFile_t *fh )
{
    return ( dbputBeiBlk( fh, gGVals, gGValCount ) != -1 ) - 1;
}

int ScptLoadVariables( xFile_t *fh )
{
    return ( dbreadBeiBlk( fh, gGVals, gGValCount ) != -1 ) - 1;
}

int ScptLoadVarTest( xFile_t *fh )
{
    int *p;

    if( !(p = Malloc( gGValCount * sizeof( int ) )) ) return -1;
    if( dbreadBeiBlk( fh, p, gGValCount ) == -1 ) return -1;
    Free( p );
    return 0;
}

int ScptBookReset()
{
    xFile_t *fh;
    int c, i;
    char fname[ 268 ];

    gScptCount = 0;
    strcpy( fname, gProtoDataFilePath );
    strcpy( &fname[ strlen( fname ) ], gScptPath );
    strcpy( &fname[ strlen( fname ) ], "scripts.lst" );
    if( !( fh = dbOpen( fname, "rt" ) ) ) return -1;
    while( (c = dbgetc( fh )) != -1 ){
        if( c == '\n') gScptCount++;
    }
    gScptCount++;
    dbClose( fh );
    for( i = 0; i < 5; i++ ){
        gScrScripts[ i ].First = NULL;
        gScrScripts[ i ].Current = NULL;
        gScrScripts[ i ].Blocks = 0;
        gScrScripts[ i ].NextId = 0;
    }
    return 0;
}

int ScptSaveScpt( Scpt_t *scr, xFile_t *fh )
{
    if( dbputBei( fh, scr->Id ) == -1) return -1;
    if( dbputBei( fh, scr->i01 ) == -1 ) return -1;
    switch( scr->Id >> 24 ){
        case 1:
            if( dbputBei( fh, scr->HexOrTimer ) == -1 ) return -1;
            if( dbputBei( fh, scr->Radius ) == -1 ) return -1;
            break;
        case 2:
            if( dbputBei( fh, scr->HexOrTimer ) == -1 ) return -1;
            break;
    }
    if( dbputBei( fh, scr->Flags ) == -1 ) return -1; 
    if( dbputBei( fh, scr->LocVarId ) == -1 ) return -1; 
    if( dbputBei( fh, 0 ) == -1 ) return -1;  // rubbish, originally -> pointer!
    if( dbputBei( fh, scr->i08 ) == -1 ) return -1; 
    if( dbputBei( fh, scr->LocVarsIdx ) == -1 ) return -1; 
    if( dbputBei( fh, scr->LocVarsCnt ) == -1 ) return -1; 
    if( dbputBei( fh, scr->i11 ) == -1 ) return -1; 
    if( dbputBei( fh, scr->i12 ) == -1 ) return -1; 
    if( dbputBei( fh, scr->ArgVal ) == -1 ) return -1; 
    if( dbputBei( fh, scr->i17 ) == -1 ) return -1; 
    if( dbputBei( fh, scr->i18 ) == -1 ) return -1; 
    if( dbputBei( fh, scr->i19 ) == -1 ) return -1; 
    if( dbputBei( fh, scr->i20 ) == -1 ) return -1;
    if( dbputBei( fh, scr->i21 ) == -1 ) return -1;
    return 0;
}

int ScptSavePage( ScptCache_t *scr, xFile_t *fh )
{
    int i;

    for( i = 0; i < 16; i++ ){
        if( ScptSaveScpt( &scr->Script[ i ], fh ) ) return -1;
    }

    if( dbputBei( fh, scr->ScptUsed ) == -1 ) return -1;
    if( dbputBei( fh, 0 ) == -1 ) return -1; //rubbish, originally -> pointer!
    return 0;
}

int ScptSaveScript( xFile_t *fh )
{
    ScptCache_t *Current, *p, *r, *q;
    Scpt_t tmp, *scp;
    ScptBook_t *dir;
    int i, j, n, k;

    dir = gScrScripts;    
    for( j = 0; j < 5; j++, dir++ ){
        n = 16 * dir->Blocks;
        Current = dir->Current;
        p = NULL;
        if( Current ) n += Current->ScptUsed - 16;
        for( q = dir->First; q; q = q->Next ){
            scp = q->Script;
            for( i = 0; i < q->ScptUsed; i++, scp++ ){
                p = dir->Current;
                if( !(scp->Flags & 0x08) ) continue;
                k = p->ScptUsed - 1;
                n--;
                if( p == q && k <= i ) break;
                while( p != q || k > i ){
                    if( (p->Script[ k ].Flags & 8) == 0 ) break;
                    if( --k >= 0 ) continue;
                    r = dir->First;
                    if( dir->First == p ) continue;
                    while( p != r->Next ) r = r->Next;
                    p = r;
                    k = r->ScptUsed - 1;                                    
                }
                if( p != q || k > i ){
                    memcpy( &tmp, scp, sizeof( tmp ) );
                    memcpy( scp, &p->Script[ k ], sizeof( Scpt_t ) );
                    memcpy( &p->Script[ k ], &tmp, sizeof( Scpt_t ) );
                    n++;
                }                                    
            }
        }
        if( dbputBei(fh, n) == -1 ) return -1;        
        if( n <= 0 ) continue;
        for( r = dir->First; r != p; r = r->Next ){
            if( ScptSavePage( r, fh ) ) return -1;                
        }
        if( !p ) continue;
        k = p->ScptUsed;                
        for( i = 0; i < r->ScptUsed; i++ ){
            if( (r->Script[ i ].Flags & 8) != 0 ) break;
        }
        if( i <= 0 ) continue;
        r->ScptUsed = i;
        if( ScptSavePage( r, fh ) ) return -1;
        r->ScptUsed = k;
    }
    return 0;
}

int ScptLoadScpt( Scpt_t *scp, xFile_t *fh )
{
    int tmp;

    if( dbgetBei( fh, &scp->Id ) == -1 ) return -1; // PID
    if( dbgetBei( fh, &scp->i01 ) == -1 ) return -1; // next script, not used
    scp->i01 = -1;
    switch( SCRT( scp->Id ) ){
	case SCR_TYPE_SPT: // spatial or timer
    	    if( dbgetBei( fh, &scp->HexOrTimer ) == -1 ) return -1; // spatial script hex
    	    if( dbgetBei( fh, &scp->Radius ) == -1 ) return -1; // spatial script radius
    	    break;
	case SCR_TYPE_TIM: 
    	    if( dbgetBei( fh, &scp->HexOrTimer ) == -1 ) return -1; // timer script time    
    	    break;
    }
    if( dbgetBei( fh, &scp->Flags ) == -1 ) return -1; // script flags
    if( dbgetBei( fh, &scp->LocVarId ) == -1 ) return -1; // script id
    if( dbgetBei( fh, &tmp ) == -1 ) return -1; // unk 5
    if( dbgetBei( fh, &scp->i08 ) == -1 ) return -1; // script oid
    if( dbgetBei( fh, &scp->LocVarsIdx ) == -1 ) return -1; // local var offset
    if( dbgetBei( fh, &scp->LocVarsCnt ) == -1 ) return -1; // local vars num
    if( dbgetBei( fh, &scp->i11 ) == -1 ) return -1; // unk 9
    if( dbgetBei( fh, &scp->i12 ) == -1 ) return -1; // unk 10
    if( dbgetBei( fh, &scp->ArgVal ) == -1 ) return -1; // unk 11
    if( dbgetBei( fh, &scp->i17 ) == -1 ) return -1; // unk 12
    if( dbgetBei( fh, &scp->i18 ) == -1 ) return -1; // unk 13
    if( dbgetBei( fh, &scp->i19 ) == -1 ) return -1; // unk 14
    if( dbgetBei( fh, &scp->i20 ) == -1 ) return -1; // unk 15
    if( dbgetBei( fh, &scp->i21 ) == -1 ) return -1; // unk 16
    scp->i07 = 0;
    scp->TimeEv = 0;
    scp->crit = 0;
    scp->item = 0;
    memset( scp->PprocIdx, 0, 28 * 4 );
    if( !(gScptUnk18 & 1) ) scp->LocVarsCnt = 0;
    return 0;
}

int ScptLoadScriptPage( ScptCache_t *dat, xFile_t *fh )
{
    int i, tmp;

    for( i = 0; i < 16; i++ ){
        if( ScptLoadScpt( &dat->Script[ i ], fh ) ) return -1;
    }
    //check block
    if( dbgetBei( fh, &dat->ScptUsed ) == -1 ) return -1;
    if( dbgetBei( fh, &tmp ) == -1 ) return -1;
    return 0;
}

int ScptLoadScript( xFile_t *fh )
{
    int count = 0;
    int j,i, Cat;
    ScptCache_t *NewPage, *p;
    ScptBook_t *dir;
    dir = gScrScripts;
    for( Cat = 0; Cat < 5; Cat++, dir++ ){
        if( dbgetBei( fh, &count ) == -1 ) return -1;
        if( count == 0 ){ // skip if zero
            dir->Current = NULL;
            dir->Blocks = 0;
            dir->First = NULL;
            continue;
        }        
	// loop counter must be modulo 16 (round up)
        dir->Blocks = count / 16;
        if( count % 16 ) dir->Blocks++;
	// create new script page of 16 scripts
	p = NULL;
	dir->First = dir->Current = NULL;
        for( i = 0; i < dir->Blocks; i++ ){
            if( !(NewPage = Malloc( sizeof( ScptCache_t ) )) ) return -1;
            if( ScptLoadScriptPage( NewPage, fh ) ) return -1;
            for( j = 0; j < 16; j++ ){
                NewPage->Script[ j ].TimeEv = NULL;
                NewPage->Script[ j ].crit = NULL;
                NewPage->Script[ j ].item = NULL;
                NewPage->Script[ j ].i07 = 0;
                NewPage->Script[ j ].Flags &= ~0x01;
            }
            NewPage->Next = NULL;
	    if( !p ){
		p = NewPage;
		dir->First = p;
	    } else {
        	p->Next = NewPage;
        	p = NewPage;
    	    }
        }
        dir->Current = NewPage;
    }
    return 0;
}

int ScptPtr( int Pid, Scpt_t **pScript )
{
    int i;
    ScptCache_t *spt;

    *pScript = NULL;
    if( Pid == -1 ) return -1;
    if( Pid == SCPT_UNSETID ){
        eprintf( "\n  ERROR: scr_ptr called with UN-SET id #!!!!" );
        return -1;
    }
    for( spt = gScrScripts[ SCRT( Pid ) ].First; spt; spt = spt->Next ){
        for( i = 0; i < spt->ScptUsed; i++ ){
    	    if( Pid == spt->Script[ i ].Id ){ // found script !
		*pScript = &spt->Script[ i ];
		return 0;
    	    }
	}    
    }
    return -1;
}

int ScptCreateId( int Category )
{
    int Id;
    Scpt_t *pScript;

    Id = gScrScripts[ Category ].NextId;
    gScrScripts[ Category ].NextId = Id + 1;
    Category <<= 24;
    do{
        if( ScptPtr( Id | Category, &pScript ) == -1 ) break;
    }while( ++Id <= MAX_SCPTID );
    return Id;
}

int ScptGetFreeId( int Category )
{
    return gScrScripts[ Category ].NextId;
}

int ScptNewScript( int *pNewId, int Category )
{
    ScptBook_t *dir;
    ScptCache_t *cache, *NewPage;
    int NewId;
    Scpt_t *scp;

    dir = &gScrScripts[Category];
    cache = dir->Current;
    if( dir->First ){
        if( cache->ScptUsed == 16 ){
            NewPage = Malloc( sizeof( ScptCache_t ) );
            cache->Next = NewPage;
            if( !NewPage ) return -1;
            NewPage->ScptUsed = 0;
            NewPage->Next = NULL;
            dir->Current = NewPage;
            cache = NewPage;
            dir->Blocks++;
        }
    } else {
        NewPage = Malloc( sizeof( ScptCache_t ) );
        cache = NewPage;
        if( !NewPage ) return -1;
        NewPage->Next = NULL;
        NewPage->ScptUsed = 0;
        dir->Blocks = 1;
        dir->Current = NewPage;
        dir->First = NewPage;
    }
    NewId = ScptCreateId( Category ) | ( Category << 24 );
    scp = &cache->Script[ cache->ScptUsed++ ];
    *pNewId = scp->Id = NewId;
    scp->i01 = -1;
    scp->HexOrTimer = -1;
    scp->Flags = 0;
    scp->LocVarId = -1;
    scp->i07 = 0;
    scp->LocVarsIdx = -1;
    scp->LocVarsCnt = 0;
    scp->i11 = 0;
    scp->i12 = 0;
    scp->ArgVal = 0;
    scp->TimeEv = 0;
    scp->crit = NULL;
    scp->item = NULL;
    scp->i17 = -1;
    scp->i18 = 0;
    scp->i19 = 0;
    scp->i20 = 0;
    scp->i21 = 0;
    memset( scp->PprocIdx, 0, 28 * 4 );
    return 0;
}

int ScptUnk17( int *pPid, int a2, int a3 )
{
    Scpt_t *scr;

    if( ScptNewScript( pPid, 1 ) == -1 ) return -1;
    if( ScptPtr( *pPid, &scr ) == -1 ) return -1;    
    scr->HexOrTimer = (a3 << 29) | a2;
    scr->Radius = 0;
    return 0;
}

int ScptRemoveLocalVars( Scpt_t *Scr )
{
    int size, i, j, *p;
    ScptCache_t *q;

    if( !Scr ) return -1;
    if( !Scr->LocVarsCnt ) return 0;
    size = gScptLocalVarsCnt;
    if( gScptLocalVarsCnt <= 0 || Scr->LocVarsIdx < 0 ) return 0;
    gScptLocalVarsCnt -= Scr->LocVarsCnt;
    if( (gScptLocalVarsCnt - Scr->LocVarsCnt) == Scr->LocVarsIdx ) return 0;
    memmove( &gScptLocalVars[ Scr->LocVarsIdx ], &gScptLocalVars[ Scr->LocVarsIdx + Scr->LocVarsCnt ], (size - Scr->LocVarsCnt - Scr->LocVarsIdx) * sizeof( int ) );

    p = Realloc( gScptLocalVars, gScptLocalVarsCnt * sizeof( int ) );
    if( !p ) eprintf( "\nError in mem_realloc in scr_remove_local_vars!\n" );    
    gScptLocalVars = p;

    for( j = 0; j < 5; j++ ){
        for( q = gScrScripts[ j ].First; q; q = q->Next ){
            for( i = 0; i < q->ScptUsed; i++ ){
                if( Scr->LocVarsIdx < q->Script[ i ].LocVarsIdx ) q->Script[ i ].LocVarsIdx -= Scr->LocVarsCnt;
            }
            if( i < q->ScptUsed ) break;
        }
    }        
    return 0;
}

int ScptRemove( int Pid )
{
    int i;
    ScptBook_t *bk;
    ScptCache_t *qq;
    ScptCache_t *Current;
    int Occupied;

    if( Pid == -1 ) return -1;
    bk = &gScrScripts[ SCRT( Pid ) ];
    for( qq = bk->First; qq; qq = qq->Next ){            
        for( i = 0; i < qq->ScptUsed; i++ ){
            if( Pid == qq->Script[ i ].Id ) break;
        }
        if( i < qq->ScptUsed ) break;
    }
    if( !qq ) return -1;    
    if( ( qq->Script[ i ].Flags & SCR_02 ) && qq->Script[ i ].i07 ) qq->Script[ i ].i07 = 0;
    if( qq->Script[ i ].Flags & SCR_NOTREMOVE ) return 0;
    if( (gScptUnk02 & 0x01) && (gScptUnk15.obj == qq->Script[ i ].TimeEv) ) gScptUnk02 &= ~0x401;
    if( ScptRemoveLocalVars( &qq->Script[ i ] ) == -1 ) eprintf( "\nERROR Removing local vars on scr_remove!!\n" );
    if( EvQeDelB( qq->Script[ i ].TimeEv, 3 ) == -1 ) eprintf( "\nERROR Removing Timed Events on scr_remove!!\n" );
    Current = bk->Current;
    if(( qq != bk->Current ) || ( i + 1 != Current->ScptUsed )){
        memcpy( &qq->Script[ i ], &bk->Current->Script[ bk->Current->ScptUsed - 1 ], sizeof( Scpt_t ) );
        bk->Current->ScptUsed--;
        if( !bk->Current->ScptUsed ){
            bk->Blocks--;
            for( qq = bk->First; bk->Current != qq->Next; qq = qq->Next );
            qq->Next = NULL;
            Free( bk->Current );
            bk->Current = qq;
        }
    } else {                 
	Occupied = Current->ScptUsed;
    	Current->ScptUsed--;
    	if( Occupied == 1 ){
    	    bk->Blocks--;
    	    Free( Current );
    	    if( bk->Blocks ){
    		for( qq = bk->First; bk->Current != qq->Next; qq = qq->Next );
    		qq->Next = NULL;
    		bk->Current = qq;
    	    } else {
        	bk->First = NULL;
        	bk->Current = NULL;
    	    }
    	}
    }
    return 0;
}

int ScptFlush()
{
    ScptBook_t *scp;
    ScptCache_t *p;
    int i, k;
    
    EvQeRun( 3, 0 );
    ScptMsgFree();
    for( k = 0; k < SCR_TYPES; k++ ){
	scp = &gScrScripts[ k ];        
        for( p = scp->First; p; p = p->Next ){    	    
            for( i = 0; i < p->ScptUsed; i++ ){
        	if( p->Script[ i ].Flags & SCR_NOTREMOVE ) continue;
        	if( i || ( p->ScptUsed != 1 ) ){
            	    ScptRemove( p->Script[ i ].Id );
        	} else {
            	    ScptRemove( p->Script[ 0 ].Id );
        	}
            }
        }
    }
    gScptUnk10 = 0;
    gScptUnk50 = 0;
    gScptUnk51 = 0;
    gMapScriptId = -1;
    SciUnk20();
    ExportUndefVars();
    return 0;
}

int ScptDeleteAll()
{
    ScptBook_t *p1, *p2, *v3;
    ScptCache_t *Script, *v4;

    p1 = gScrScripts;
    EvQeRun(3, 0);
    ScptMsgFree();
    p2 = &gScrScripts[5];
    do
    {
        v3 = p1;
        Script = p1->First;
        if( p1->First ){
            do
            {
        	v4 = Script->Next;
                Free( Script );
                Script = v4;
            }
            while ( v4 );
        }
        v3->Current = NULL;
        v3->Blocks = 0;
        p1++;
        v3->First = NULL;
    }
    while ( p1 != p2 );
    gScptUnk10 = 0;
    gScptUnk50 = 0;
    gScptUnk51 = 0;
    gMapScriptId = -1;
    SciUnk20();
    ExportUndefVars();
    return 0;
}

Scpt_t *ScptUnk20( int a1 )
{
    Scpt_t *result;

    gScptUnk51 = a1;
    result = gScrScripts[ SCR_TYPE_SPT ].First->Script;
    gScptUnk10 = 0;
    gScptUnk50 = gScrScripts[ SCR_TYPE_SPT ].First;
    if( gScrScripts[SCR_TYPE_SPT].First && ((gScrScripts[SCR_TYPE_SPT].First->Script[0].Flags & 2) != 0 || (gScrScripts[SCR_TYPE_SPT].First->Script[0].HexOrTimer & 0xE0000000) >> 29 != a1) ) return ScptUnk21();
    return result;
}

Scpt_t *ScptUnk21()
{
    Scpt_t *r;
    ScptCache_t *p;
    int i;

    p = gScptUnk50;
    i = gScptUnk10;
    if( gScptUnk50 ){
        do{
            if( ++i == 16 ){
                p = p->Next;
                i = 0;
            } else if( i >= p->ScptUsed ){
                p = NULL;
            }
            if( !p ) break;
        }while( (p->Script[ i ].Flags & 0x02) || (p->Script[ i ].HexOrTimer & 0xE0000000) >> 29 != gScptUnk51 );
        r = ( p ) ? &p->Script[ i ] : NULL;
    } else {
        r = NULL;
    }
    gScptUnk10 = i;
    gScptUnk50 = p;
    return r;
}

void ScptUnk22()
{
    gScptUnk52 = 1;
}

void ScptUnk23()
{
    gScptUnk52 = 0;
}

int ScptUnk24( Obj_t *a1, int a2, int a3 )
{
    int v6, i,HexOrTimer;
    Scpt_t *p, *Script, *res;

    if( a1 == gGmouseObjA ) return 0;
    if( a1 == gGmouseObjB ) return 0;
    if( (a1->Flags & 0x01) || (a1->Flags & 0x08) ) return 0;
    if( a2 < 10 ) return 0;
    if( !gScptUnk52 ) return 0;
    gScptUnk52 = 0;
    v6 = a3 << 29;    
    i = a2 | (v6 & 0xE0000000);
    for( p = ScptUnk20( a3 ); p; p = ScptUnk21() ){
        HexOrTimer = p->HexOrTimer;
        if( i == HexOrTimer ){
            if( ScptPtr( p->Id, &Script ) != -1 ){
                Script->crit = a1;
                Script->item = 0;
            }
        } else {
            if( !p->Radius || TileGetDistance( HexOrTimer & 0x3FFFFFF, a2 ) > p->Radius ) continue;
            if( ScptPtr( p->Id, &res ) != -1 ){
                res->crit = a1;
                res->item = 0;
            }
        }
        ScptExecScriptProc( p->Id, 2 );
    }
    gScptUnk52 = 1;
    return 1;
}

int ScptUnk25( int a1, int a2, int a3 )
{
    return TileGetDistance( a1, a3 ) <= a2;
}

int ScptGetUsage( int Type )
{
    int use;

    use = 16 * gScrScripts[ Type ].Blocks;
    if( gScrScripts[ Type ].Current ) use += gScrScripts[ Type ].Current->ScptUsed - 16;
    return use;
}

int ScptCountUsage( int Type )
{
    ScptCache_t *p;
    int Usage;
    
    Usage = 0;
    for( p = gScrScripts[ Type ].First; p; p = p->Next ) Usage += p->ScptUsed;
    return Usage;
}

int ScptLoadAllScripts()
{
    int i, j;
    ScptCache_t *scr;

    for( i = 1; i < 5; i++ ){
        for( scr = gScrScripts[ i ].First; scr; scr = scr->Next ){
            for( j = 0; j < scr->ScptUsed; j++ ){
                ScptExecScriptProc( scr->Script[ j ].Id, 1 );
            }
        }
    }
    return 0;
}

void ScptUnk29()
{
    ScptExecMapUpdateScripts(15);
}

void ScptUnk30()
{
    ScptExecMapUpdateScripts( 23 );
}

void ScptExecMapUpdateScripts( int a1 )
{
    ScptCache_t *p;
    Scpt_t *scr;
    int v1,v5,j,*v9,i,*v11,v15,v16,*n;

    v16 = a1;
    v1 = 0;
    v15 = 0;
    gScptUnk52 = 0;
    if( a1 == 15 )
        v15 = (gMap.MapFlags & 1) == 0;
    else
        ScptExecScriptProc( gMapScriptId, 0 );
    for( i = 0; i < 5; i++ ){
        for( p = gScrScripts[ i ].First; p; p = p->Next ) v1 += p->ScptUsed;
    }
    if( v1 == 0 ) return;    
    n = Malloc( v1 * sizeof( int ) );
    if( !n ){ eprintf( "\nError: scr_exec_map_update_scripts: Out of memory for sidList!"); return; }            
    v5 = 0;
    for( i = 0; i < 5; i++ ){
        for( p = gScrScripts[ i ].First; p; p = p->Next ){
            for( j = 0; j < p->ScptUsed; j++ ){
                if( p->Script[ j ].Id != gScptUnk14 && p->Script[ j ].PprocIdx[ v16 ] > 0 ) n[ v5++ ] = p->Script[ j ].Id;
            }            
        }        
    }
    
    if( v16 == 15 ){
        v11 = n;
        for( i = 0; i < v5; i++, v11++ ){
            if( ScptPtr( *v11, &scr ) != -1 ) scr->ArgVal = v15;
            ScptExecScriptProc( *v11, a1 );
        }
    } else {
    	v9 = n;
    	for( i = 0; i < v5; i++, v9++ ) ScptExecScriptProc( *v9, a1 );
    }
    Free( n );
    gScptUnk52 = 1;
}

void ScptUnk32()
{
    ScptExecMapUpdateScripts( 16 );
}

void ScptPrintScriptUsage()
{
    char stmp[20];
    int tmp,j,i, *alc;
    ScptCache_t *p;

    alc = Malloc( gScptCount * sizeof( int ) );
    if( !alc ){ eprintf( "\nERROR: scr_print_script_usage failed!\n" ); return; }
    for( i = 0; i < gScptCount; i++ ) alc[ i ] = 0;    
    eprintf( "\n\n\t<<< SCRIPT DEBUG DUMP >>>\n" );
    for( i = 0; i < 5; i++ ){
        strcpy( stmp, gScptTypeName[ i ] );
        StrUpr( stmp );
	tmp = gScrScripts->Blocks * 16;
	if( gScrScripts->Current ) tmp += gScrScripts->Current->ScptUsed - 16;
        eprintf( "\n\t[%s]: %d", stmp, tmp );
        for( p = gScrScripts->First; p; p = p->Next ){                
            for( j = 0; j < p->ScptUsed; j++ ){
                alc[ p->Script[ j ].LocVarId & 0xFFFFFF ] = 1;
            }
        }            
    }
    eprintf( "\n[MAP_USAGE]scripts/obj_dude.int   [DEST]scripts/" );
    for( i = 0; i < gScptCount; i++ ){
        if( alc[ i ] == 0 ) continue;
        eprintf( "\n[MAP_USAGE]scripts/" );
        stmp[0] = '\0';
        if( ScptGetScriptFname( i & 0xFFFFFF, stmp ) == -1 )
            eprintf( "ERROR /" );
        else
            eprintf( "%s   [DEST]scripts/", stmp );
    }
    eprintf( "\n" );
    Free( alc );
}

int ScptGetMsgStr( int MsgIdx, Msg_t **Msg )
{
    char *p, stmp[260], tt[28];
    int k;

    *Msg = NULL;
    if ( MsgIdx == -1 ) return -1;
    k = MsgIdx - 1;
    if( !gScptMsgBook[ MsgIdx - 1 ].Count == 0 ){
	tt[0] = '\0';
	ScptGetScriptFname( k & 0xFFFFFF, tt );
	p = strchr(tt, '.');
	if( p ) *p = '\0';
	sprintf(stmp, "dialog/%s.msg", tt);
	if( MessageLoad( gScptMsgBook, stmp ) != 1 ){ eprintf( "\nError loading script dialog message file!" ); return -1; }
	if( MessageLangFilter( gScptMsgBook ) != 1 ){ eprintf( "\nError filtering script dialog message file!" ); return -1; }
    }
    *Msg = &gScptMsgBook[ k ];
    return 0;
}

char *ScptGetDialogA( int a1, int a2 )
{
    return ScptGetDialog( a1, a2, 0 );
}

char *ScptGetDialog( int MsgPage, int MsgId, int SpkFlg )
{
    MsgLine_t MsgList;
    Msg_t *Msg;

    if( !MsgPage && !MsgId ) return NULL;
    if( MsgPage == -1 && MsgId == -1 ) return NULL;
    if( MsgPage == -2 && MsgId == -2 ) return MessageGetMessage( &gProtoMessages, &MsgList, 650 );
    if( ScptGetMsgStr( MsgPage, &Msg ) == -1 ){ eprintf( "\nERROR: message_str: can't find message file: List: %d!", MsgPage ); return 0; }
    if( ( gDlgUnk46 & 0xF000000 ) >> 24 != 8 ) SpkFlg = 0;
    MsgList.Id = MsgId;
    if( MessageGetMsg( Msg, &MsgList ) != 1 ){ eprintf( "\nError: can't find message: List: %d, Num: %d!", MsgPage, MsgId ); return "Error"; }
    if( !SpkFlg || !GdialogUnk01() ) return MsgList.Text;
    if( !( MsgList.Audio && *MsgList.Audio ) ){ eprintf( "Missing speech name: %d\n", MsgList.Id ); return MsgList.Text; }
    GdialogLipsyncStart( ( MsgList.Unk & 0x01 ) ? 0 : MsgList.Audio );
    return MsgList.Text;    
}

int ScptGetLocVar( int ScrId, int VarIdx, int *pValue )
{
    Scpt_t *scr, *res;

    scr = 0;
    if( (ScrId >> 24) == 0 ){
        eprintf("\nError! System scripts/Map scripts not allowed local_vars! ");
        gScptFileName[ 0 ] = 0;
        ScptGetScriptFname( ScrId & 0xFFFFFF, gScptFileName);
        eprintf( ":%s\n", gScptFileName );
        *pValue = -1;
	return -1;
    }    
    if( ScptPtr( ScrId, &scr ) == -1 ){ *pValue = -1; return -1; }
    if( !scr->LocVarsCnt ){
        if( ScptPtr( ScrId, &res ) != -1 ) res->LocVarsCnt = gScptLocVarTable[ scr->LocVarId ].LocalVars;
    }
    if( scr->LocVarsCnt > 0 ){
        if( scr->LocVarsIdx == -1 ){
            scr->LocVarsIdx = MapVarsAdd(scr->LocVarsCnt);
            *pValue = MapGetLocalVar( scr->LocVarsIdx + VarIdx );
        } else {
            *pValue = MapGetLocalVar( scr->LocVarsIdx + VarIdx );
        }
    }
    return 0;
}

int ScptSetLocVar( int ScrId, int VarIdx, int Value )
{
    Scpt_t *scr;

    scr = NULL;
    if( ScptPtr( ScrId, &scr ) == -1 ) return -1;
    if( !scr->LocVarsCnt ){
        if( ScptPtr( ScrId, &scr ) != -1 ) scr->LocVarsCnt = gScptLocVarTable[ scr->LocVarId ].LocalVars;
    }
    if( scr->LocVarsCnt <= 0 ) return -1;
    if( scr->LocVarsIdx == -1 ){
        scr->LocVarsIdx = MapVarsAdd( scr->LocVarsCnt );
        MapSetLocalVar( scr->LocVarsIdx + VarIdx, Value );
    } else {
        MapSetLocalVar( scr->LocVarsIdx + VarIdx, Value );
    }
    return 0;
}

int ScptUnk39()
{
    Scpt_t *scr;
    int gid;

    if( !gMapScriptId || gMapScriptId == -1 ) return 0;    
    if( ( gid = CombatGetGroupId() ) == -1 ) return 0;
    if( ScptPtr( gMapScriptId, &scr ) != -1 ) scr->ArgVal = gid;
    ScptExecScriptProc( gMapScriptId, 13 );
    return ScptPtr( gMapScriptId, &scr ) != -1 && scr->i18;
}

int ScptUnk40( Obj_t *a1, int edx0,int a3, int a4 )
{
    Scpt_t *Scr, *a2;
    ScptCache_t *i, *j;
    int err,v6,v7,v14,*v19,v21,*n,v30,v31;

    err = 16 * (gScrScripts[1].Blocks + gScrScripts[3].Blocks);
    v6 = 0;
    if( !err ) return 0;    
    n = Malloc((gScrScripts[1].Blocks + gScrScripts[3].Blocks) << 6);
    if( !n ) return -1;    
    gScptUnk52 = 0;
    for( i = gScrScripts[3].First; i; i = i->Next ){                
    	v31 = v6;
    	for( v7 = 0; v7 < i->ScptUsed; v7++, v31++, v6++ ){
    	    if( i->Script[ v7 ].PprocIdx[ 14 ] <= 0 && !i->Script[ v7 ].i07 ) ScptExecScriptProc( i->Script[ v7 ].Id, 1 );
    	    if( i->Script[ v7 ].PprocIdx[ 14 ] <= 0 ) continue;    
    	    if( !i->Script[ v7 ].TimeEv ) continue;            	    
            if( i->Script[ v7 ].TimeEv->Elevation != a4 ) continue;
            if( TileGetDistance(i->Script[ v7 ].TimeEv->GridId, edx0) > a3 ) continue;            
            n[ v31 + 1 ] = i->Script[ v7 ].Id;
    	}
    }
    for( j = gScrScripts[ SCR_TYPE_SPT ].First; j; j = j->Next ){                
        v14 = v6;
        for( v30 = 0; v30 < j->ScptUsed; v30++ ){
            if( j->Script[ v30 ].PprocIdx[ 14 ] <= 0 && !j->Script[ v30 ].i07 ) ScptExecScriptProc( j->Script[ v30 ].Id, 1 );
            if( j->Script[ v30 ].PprocIdx[ 14 ] <= 0 ) continue;
            if( (j->Script[ v30 ].HexOrTimer & 0xE0000000) >> 29 != a4 ) continue;
            if( TileGetDistance( j->Script[ v30 ].HexOrTimer & 0x3FFFFFF, edx0 ) > a3 ) continue;
            v6++, v14++;
            n[ v14 ] = j->Script[ v30 ].Id;
        }
    }
    v19 = n;
    for( v21 = 0; v21 < v6; v19++, v21++ ){
        if( ScptPtr( *v19, &Scr ) != -1 ) Scr->ArgVal = 20;
        if( ScptPtr( *v19, &a2 ) != -1 ){
            a2->crit = NULL;
            a2->item = a1;
        }
        ScptExecScriptProc( *v19, 14 );
    }
    if( n ) Free( n );
    gScptUnk52 = 1;
    return 0;
}
