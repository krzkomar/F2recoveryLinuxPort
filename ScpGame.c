/*
    Game engine related commands handlers.
*/
#include "FrameWork.h"

#define PTR2INT( ptr )	0
#define RETINT( scr, ret_val ) IntpPushiA( scr, ret_val ); IntpPushwA( scr, SCR_INT )
#define RETPTR( scr, ret_val ) IntpPushPtrA( scr, ret_val ); IntpPushwA( scr, SCR_PTR )
#define RETSTR( scr, ret_val ) IntpPushiA( scr, ret_val ); IntpPushwA( scr, SCR_STRING )
#define RETFTR( scr, ret_val ) IntpPushiA( scr, ret_val ); IntpPushwA( scr, SCR_FSTRING )

#define GETARGI( scr, type, arg, arg_num, name )	\
        type = IntpPopwA( scr );\
        arg = IntpPopiA( scr );		\
        if( type == SCR_FSTRING ) IntpStringDeRef( scr, type, arg );	\
        if( (type & 0xF7FF) != SCR_INT ) IntpError( "script error: %s: invalid arg %d[%i] to "#name, scr->FileName, arg_num, type );

#define GETARGP( scr, type, arg, arg_num, name )	\
        type = IntpPopwA( scr );\
        arg = IntpPopPtrA( scr );\
        if( type == SCR_FSTRING ) IntpStringDeRef( scr, type, PTR2INT( arg ) );\
        if( (type & 0xF7FF) != SCR_PTR ) IntpError( "script error: %s: invalid arg %d[0x%x]{0x%x} to "#name, scr->FileName, arg_num, type, arg );

#define GETARGIP( scr, type, argi, argp, arg_num, name )	\
        type = IntpPopwA( scr );\
        if( type == SCR_PTR ) \
    	    argp = IntpPopPtrA( scr );\
        else \
    	    argi = IntpPopiA( scr );\
        if( type == SCR_FSTRING ) IntpStringDeRef( scr, type, PTR2INT( argi ) );\
        if( (type & 0xF7FF) != SCR_PTR && (type & 0xF7FF) != SCR_INT) IntpError( "script error: %s: invalid arg %d to "#name, scr->FileName, arg_num );

#define GETARGS( scr, type, arg, arg_num, name )	\
        type = IntpPopwA( scr );\
        arg = IntpPopiA( scr );		\
        if( type == SCR_FSTRING ) IntpStringDeRef( scr, type, arg );	\
        if( (type & 0xF7FF) != SCR_STRING ) IntpError( "script error: %s: invalid arg %d to "#name, scr->FileName, arg_num );

#define GETARGF( scr, type, arg, arg_num, name )	\
        type = IntpPopwA( scr );\
        arg = IntpPopiA( scr );		\
        if( type == SCR_FSTRING ) IntpStringDeRef( scr, type, arg );	\
        if( (type & 0xF7FF) != SCR_FSTRING ) IntpError( "script error: %s: invalid arg %d to "#name, scr->FileName, arg_num );



uint32_t gScrGameLightLvl[ 3 ] = { 0x4000, 0xA000, 0x10000 };
uint16_t gScrGameMovieFlags[ 17 ] = { 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,  0x9, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B };

VidRect_t gScrGameUnk001 = { 0, 0, 0x280, 0x1E0 };

int gScrGameMood;
char *gScrGamePlayerName;

const char *ScriptErrClass[ 4 ] = { "unimped", "obj is NULL", "can't match program to sid", "follows" };


void ScrGameErrorMsg( Intp_t *strerr, const char *Operand, int ErrClass )
{
    char stmp[ 268 ];

    sprintf( stmp, "Script Error: %s: op_%s: %s", strerr->FileName, Operand, ScriptErrClass[ ErrClass ] );
    eprintf( "%s", stmp );
}

void ScrGameEprintf( const char *fmt, ... )
{
    char Tmp[256];
    va_list vargs;
    va_list valist;

    va_start( valist, fmt );
    va_copy( vargs, valist );
    vsprintf( Tmp, fmt, vargs );
    eprintf( "%s", Tmp );
}

int ScrGameIsTileVisible( int pos )
{
    return ( abs32( gTileCentIdx - pos ) % 200 < 5 ) || ( abs32( gTileCentIdx - pos ) / 200 < 5 );
}

int ScrGameSetShape( Obj_t *critter, Obj_t *a2, int Flags )
{
    int ImageId, arg;
    VidRect_t Area;

    ImageId = -1;
    if( critter == gObjDude ) IfaceHandSlotUpdate( ( GameIfaceStat() == 0 ), -1, -1 );
    arg = (critter->ImgId & 0xF000) >> 12;
    if( Flags & 0x3000000 ){
        if( critter == gObjDude ){
            if( IfaceGetSelectedHand() ){
                if( Flags & 0x2000000 ) arg = 0;
            } else {
        	if( Flags & 0x1000000 ) arg = 0;
            }     
        } else { 
    	    if( Flags & 0x2000000 ) arg = 0;
        }        
        if( !arg ) ImageId = ArtMakeId( (critter->ImgId & 0xF000000) >> 24, critter->ImgId & 0xFFF, (critter->ImgId & 0xFF0000) >> 16, 0, (critter->ImgId & 0x70000000) >> 28);
    } else {
	if( critter == gObjDude ) ImageId = ArtMakeId( (critter->ImgId & 0xF000000) >> 24, gArtPrep[0], (critter->ImgId & 0xFF0000) >> 16, arg, (critter->ImgId & 0x70000000) >> 28 );
	InvUpdateStatistics( critter, a2, 0 );
    }
    if( ImageId != -1 ){
        ObjSetShape( critter, ImageId, &Area );
        TileUpdateArea( &Area, gMapCurrentLvl );
    }
    return 0;
}

/*
    void give_exp_points (int points) - add the player experience points
*/
void ScrGame_GiveExpPoints( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;

    GETARGI( scr, type, val, 0, "give_exp_points" );
    SCP_DBGA( "give_exp_points( [%x]%i )", type, val );
    if( FeatLvlUp( val ) ) ScrGameEprintf( "\nScript Error: %s: op_give_exp_points: stat_pc_set failed", scr->FileName );
}

/*
    void scr_return( ? ) - Sets the return value for a scripts C-engine node, to be used by C code.
*/
void ScrGame_Return( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;
    Scpt_t *spt;

    GETARGI( scr, type, val, 0, "scr_return" );
    SCP_DBGA( "scr_return( [%x]%i )", type, val );
    if( ScptPtr( ScptGetActionSource( scr ), &spt ) != -1 ) spt->i11 = val;
}

/*
    void play_sfx( string sfxFileName ) - Plays the specified sound effect
*/
void ScrGame_PlaySfx( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;

    GETARGS( scr, type, val, 0, "play_sfx" );
    SCP_DBGA( "play_sfx( [%x]%i )", type, val );
    GSoundPlay( IntpGetString( scr, type >> 8, val ) );
}

/*
    void set_map_start(int x, int y, 0-2 elev, 0-5 rot) - Sets the start location & rotation for the next time this map is entered (loaded & run).
*/
void ScrGame_SetMapStart( Intp_t *scr )
{
    SCP_DBG_VAR;
    int MapIdx, Val[ 4 ];
    uint16_t type[ 4 ];

    GETARGI( scr, type[ 0 ], Val[ 0 ], 0, "set_map_start" );
    GETARGI( scr, type[ 1 ], Val[ 1 ], 1, "set_map_start" );
    GETARGI( scr, type[ 2 ], Val[ 2 ], 2, "set_map_start" );
    GETARGI( scr, type[ 3 ], Val[ 3 ], 3, "set_map_start" );
    SCP_DBGA( "set_map_start( [%x]%i, [%x]%i, [%x]%i, [%x]%i )", type[3], Val[3], type[2], Val[2], type[1], Val[1], type[0], Val[0] );
    if( MapSetLvl( Val[ 1 ] ) ){
        ScrGameEprintf("\nScript Error: %s: op_set_map_start: map_set_elevation failed", scr->FileName );
    } else {
        MapIdx = 200 * Val[ 2 ] + Val[ 3 ];
        if( TileSetCenter( MapIdx, 3 ) )
            ScrGameEprintf( "\nScript Error: %s: op_set_map_start: tile_set_center failed", scr->FileName );
        else
            MapSetStart( MapIdx, Val[ 1 ], Val[ 0 ] );
    }
}

/*
    void override_map_start(int x, int y, 0-2 elev, 0-5 rot) 
    - Used when loading a new map, this forces the player (obj_dude) to start at a particular location and rotation when first coming up.
*/
void ScrGame_OverrideMapStart( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned int GridPos, x, y, lvl, rotation;
    char stmp[ 60 ];
    uint16_t type[ 4 ];

    scr->Flags |= SCR_FPROC_RUN;
    GETARGI( scr, type[ 0 ], rotation, 0, "override_map_start" );
    GETARGI( scr, type[ 1 ], lvl, 1, "override_map_start" );
    GETARGI( scr, type[ 2 ], y, 2, "override_map_start" );
    GETARGI( scr, type[ 3 ], x, 3, "override_map_start" );
    SCP_DBGA( "override_map_start( [%x]%i, [%x]%i, [%x]%i, [%x]%i )", type[3], x, type[2], y, type[1], lvl, type[0], rotation );
    sprintf( stmp, "OVERRIDE_MAP_START: x: %d, y: %d", x, y );
    eprintf( "%s\n", stmp );
    
    GridPos = 200 * y + x;
    if( GridPos == -1 ) return;
    if( ObjSetRotation( gObjDude, rotation, 0 ) ) ScrGameEprintf( "\nError: %s: obj_set_rotation failed in override_map_start!", scr->FileName );
    if( ObjMoveToTile( gObjDude, GridPos, lvl, NULL ) ){
        ScrGameEprintf( "\nError: %s: ObjMoveToTile failed in override_map_start!", scr->FileName );
        if( ObjMoveToTile( gObjDude, gTileCentIdx, lvl, NULL ) ){
            ScrGameEprintf( "\nError: %s: ObjMoveToTile RECOVERY Also failed!", scr->FileName );
            exit( 1 );
        }
    }
    TileSetCenter( GridPos, 1 );
    TileUpdate();
    scr->Flags &= ~SCR_FPROC_RUN;
}

/*
    int has_skill(ObjectPtr who, int skill [0 to 17]) - This returns the level of the skill of the target critter. (0 to 200 for Fallout 1), (0 to 300 for Fallout 2)
*/
void ScrGame_HasSkill( Intp_t *scr )
{
    SCP_DBG_VAR;
    int Val, Total;
    Obj_t *critter;
    uint16_t type[2];

    GETARGI( scr, type[0], Val, 0, "has_skill" );
    GETARGP( scr, type[1], critter, 1, "has_skill" );
    SCP_DBGA( "has_skill( [%x]%p, [%x]%i )", type[1], critter, type[0], Val );
    Total = 0;
    if( critter ){
        if( OBJTYPE( critter->Pid ) == TYPE_CRIT )
            Total = SkillGetTotal( critter, Val );
    } else {
        ScrGameErrorMsg( scr, "has_skill", 1 );
    }
    RETINT( scr, Total );
}

/*
    boolean using_skill(ObjectPtr who, int skill) - check on the use of "active skills"
*/
void ScrGame_UsingSkill( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *critter;
    int Val;
    uint16_t type[2];

    GETARGI( scr, type[0], Val, 0, "using_skill" );
    GETARGP( scr, type[1], critter, 1, "using_skill" );
    SCP_DBGA( "using_skill( [%x]%p, [%x]%i )", type[1], critter, type[0], Val );
    if( Val == 8 && critter == gObjDude ) Val = CritterUsingSkill( 0 );
    RETINT( scr, Val );
}

/*
    int roll_vs_skill( ObjectPtr who, int skill, int modifier ) - performs a random check ("die roll") on the ability (skill)
*/
void ScrGame_RollVsSkill( Intp_t *scr )
{
    SCP_DBG_VAR;
    Scpt_t *script;
    Obj_t *critter;
    int skill, modifier, sk;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], modifier, 0, "roll_vs_skill" );
    GETARGI( scr, type[ 1 ], skill, 1, "roll_vs_skill" );
    GETARGP( scr, type[ 2 ], critter, 2, "roll_vs_skill" );
    SCP_DBGA( "roll_vs_skill( [%x]%p, [%x]%i, [%x]%i )", type[2], critter, type[1], skill, type[0], modifier );
    sk = 0;
    if( critter ){
        if( OBJTYPE( critter->Pid ) == TYPE_CRIT ){
            if( ScptPtr( ScptGetActionSource( scr ), &script ) != -1 ) sk = SkillUse( critter, skill, &script->i20, modifier );
        }
    } else {
        ScrGameErrorMsg( scr, "roll_vs_skill", 1 );
    }
    RETINT( scr, sk );
}

/*
    ** not fully implemented in game, return is always 0 **    
    int skill_contest() - Returns the value of a completed skill vs skill contest (to run through is_success & is_critical).
*/
void ScrGame_SkillContest( Intp_t *scr )
{
    SCP_DBG_VAR;
    int Val[ 3 ];
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], Val[ 0 ], 0, "skill_contest" );
    GETARGI( scr, type[ 1 ], Val[ 1 ], 1, "skill_contest" );
    GETARGI( scr, type[ 2 ], Val[ 2 ], 2, "skill_contest" );
    SCP_DBGA( "skill_contest( [%x]%i, [%x]%i, [%x]%i )", type[2], Val[2], type[1], Val[1], type[0], Val[0] );
    ScrGameErrorMsg( scr, "skill_contest", 0 );
    RETINT( scr, 0 );
}

/*
    int do_check(ObjectPtr who, int check, int modifier) - performs a random check ("die roll") on the basic characteristics critter
*/
void ScrGame_DoCheck( Intp_t *scr )
{
    SCP_DBG_VAR;
    Scpt_t *script;
    Obj_t *obj;
    int a3[2], val;
    uint16_t type[3];
    
    val = 0;
    GETARGI( scr, type[ 0 ], a3[ 0 ], 0, "do_check" );
    GETARGI( scr, type[ 1 ], a3[ 1 ], 1, "do_check" );
    GETARGP( scr, type[ 2 ], obj, 2, "do_check" );
    SCP_DBGA( "do_check( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], a3[1], type[0], a3[0] );
    if( !obj ){
	ScrGameErrorMsg( scr, "do_check", 1 );
    } else {
	if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ){
    	    ScrGameErrorMsg( scr, "do_check", 1 );
	} else {
    	    switch( a3[1] ){
        	case 0 ... 6:
            	    val = FeatDice( obj, a3[1], a3[0], &script->i20 );
            	    break;
        	default:
            	    ScrGameEprintf( "\nScript Error: %s: op_do_check: Stat out of range", scr->FileName );
            	    break;
    	    }
	}
    }
    RETINT( scr, val );
}

/* 
    int is_success(int val) 
    - Returns True if a given contest roll result value is a success, otherwise False. 
    Can generate Critical Success or Critical Failure as well.
*/
void ScrGame_Success( Intp_t *scr )
{
    SCP_DBG_VAR;
    int k, val;
    uint16_t type;

    GETARGI( scr, type, val, 0, "success" );
    SCP_DBGA( "success( [%x]%i )", type, val );
    switch( val ){
        case 0 ... 1: k = 0; break;
        case 2 ... 3: k = 1; break;
        default: k = -1; break;
    }
    RETINT( scr, k );
}

/* 
    int is_critical( int va l) - Returns True if a given contest roll result is a critical result, otherwise False.
*/
void ScrGame_Critical( Intp_t *scr )
{
    SCP_DBG_VAR;
    int stat, val;
    uint16_t type;

    GETARGI( scr, type, val, 0, "critical" );
    SCP_DBGA( "critical( [%x]%i )", type, val );
    switch( val ){
        case 0:
        case 3:
            stat = 1;
            break;
        case 1:
        case 2:
            stat = 0;
            break;
        default: stat = -1; break;
    }
    RETINT( scr, stat );
}

/*
    int how_much(int val) 
    - Returns the value of a completed skill vs. skill contest (how much the rolls differed by). 
    This requires that you first call one of the contest roll commands, such as roll_vs_skill, skill_contest, etc..
*/
void ScrGame_HowMuch( Intp_t *scr )
{
    SCP_DBG_VAR;
    Scpt_t *script;
    uint16_t type;
    int hm, Val;

    hm = 0;
    GETARGI( scr, type, Val, 0, "how_much" );
    SCP_DBGA( "how_much( [%x]%i )", type, Val );
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "how_much", 2 );
    else
        hm = script->i20;
    RETINT( scr, hm );
}

/*
    void mark_area_known (int mark_type, int area, int mark_state) - establish the status of the location
*/
void ScrGame_MarkAreaKnown( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val[ 3 ];
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "mark_area_known" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "mark_area_known" );
    GETARGI( scr, type[ 2 ], val[ 2 ], 2, "mark_area_known" );
    SCP_DBGA( "mark_area_known( [%x]%i, [%x]%i, [%x]%i )", type[2], val[2], type[1], val[1], type[0], val[0] );
    if( val[ 2 ] ){ // mark area to be known
        if( val[ 2 ] == 1 ) WmUnk41( val[ 1 ] );
        return;
    }
    // unmark area
    if( val[0] == -66 ) {
        WmUnk05( val[1], 0, 1 );
        return;
    }
    WmUnk05( val[1], 1, 1 );
    WmUnk04( val[1], val[0] );    
}

/*
    ** not fully implemented in game, return is always 0 **
    int reaction_influence( int ?, int ?, int ? )
*/
void ScrGame_ReactionInfluence( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val[ 3 ];
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "reaction_influence" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "reaction_influence" );
    GETARGI( scr, type[ 2 ], val[ 2 ], 2, "reaction_influence" );
    SCP_DBGA( "reaction_influence( [%x]%i, [%x]%i, [%x]%i )", type[2], val[2], type[1], val[1], type[0], val[0] );
    RETINT( scr, EvQeGetReactionInfluence( val[2], val[1], val[0] ) );
}
    
/*
    int random( int min, int max ) - returns a random number between min ... max inclusive
*/
void ScrGame_Random( Intp_t *scr )
{
    SCP_DBG_VAR;
    int Val[ 2 ];
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], Val[ 0 ], 0, "random" );
    GETARGI( scr, type[ 1 ], Val[ 1 ], 1, "random" );
    SCP_DBGA( "random( [%x]%i, [%x]%i )", type[1], Val[1], type[0], Val[0] );    
    RETINT( scr, ( RecGetState() == 2 ) ?  RandMinMax( Val[1], Val[0] ) : ((Val[0] - Val[1]) / 2) );
}

/*
    Critter roll_dice(obj Critter, int Skill) - Returns the value of the completed dice roll. NOTE: Unimplemented, return always 0
*/
void ScrGame_RollDice( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val[ 2 ];
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "roll_dice" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "roll_dice" );
    SCP_DBGA( "roll_dice( [%x]%i, [%x]%i )", type[1], val[1], type[0], val[0] );    
    ScrGameErrorMsg( scr, "roll_dice", 0 );
    RETINT( scr, 0 );
}

/*
    int move_to (ObjectPtr obj, int tile_num, int elev) - move the object
*/
void ScrGame_MoveTo( Intp_t *scr )
{
    SCP_DBG_VAR;
    int a,b;
    int lvl, tile_num, k = 0;
    uint16_t type[ 3 ];
    Obj_t *obj;
    VidRect_t Rect, Area, Area2;

    GETARGI( scr, type[ 0 ], lvl, 0, "move_to" );
    GETARGI( scr, type[ 1 ], tile_num, 1, "move_to" );
    GETARGP( scr, type[ 2 ], obj, 2, "move_to" );
    SCP_DBGA( "move_to( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], tile_num, type[0], lvl );    
    if( obj ){
        if( obj == gObjDude ){
            b = TileUnk25();
            a = TileUnk22();
            if( b ) TileUnk24();
            if( a ) TileUnk21();            
            if( ( k = ObjMoveToTile( obj, tile_num, lvl, &Area ) ) != -1 ) TileSetCenter( obj->GridId, 1 );
            if( b ) TileUnk23();
            if( a ) TileUnk20();
        } else {
            ObjGetRefreshArea( obj, &Rect );
            if( ( lvl != obj->Elevation) && (OBJTYPE( obj->Pid ) == 1 ) ) CombatUnk79( obj );            
            if( ( k = ObjMoveToTile( obj, tile_num, lvl, &Area2 ) ) != -1 ){
                RegionExpand( &Rect, &Area2, &Area2 );
                TileUpdateArea( &Area2, gMapCurrentLvl );
            }
        }
    } else {
        ScrGameErrorMsg( scr, "move_to", 1 );
    }
    RETINT( scr, k );
}

/*
    ObjectPtr create_object( int pid, int tile_num, 0-2 elev, int sid )
*/
void ScrGame_CreateObject( Intp_t *scr )
{
    SCP_DBG_VAR;
    VidRect_t Area;
    Scpt_t *script;
    Proto_t *proto;
    unsigned int Pid_high;
    int pid, tile_num, lvl, sid, n;
    uint16_t type[4];
    Obj_t *Obj;

    n = 0;
    Obj = NULL;
    GETARGI( scr, type[ 0 ], sid, 0, "create_object" );
    GETARGI( scr, type[ 1 ], lvl, 1, "create_object" );
    GETARGI( scr, type[ 2 ], tile_num, 2, "create_object" );
    GETARGI( scr, type[ 3 ], pid, 3, "create_object" );
    SCP_DBGA( "create_object( [%x]%i, [%x]%i, [%x]%i, [%x]%i )", type[3], pid, type[2], tile_num, type[1], lvl, type[0], sid );    
    if( LsgPending() ){
        eprintf( "\nError: attempt to Create critter in load/save-game: %s!", scr->FileName );
    } else if( pid ){
        if( ProtoGetObj( pid, &proto ) != -1 ){
    	    if( ObjCreate( &Obj, proto->ImgId, pid ) != -1 ){
        	if( tile_num == -1 ) tile_num = 0;
        	if( ObjMoveToTile( Obj, tile_num, lvl, &Area ) != -1 ) TileUpdateArea( &Area, Obj->Elevation );
    	    }
        }
        if( sid != -1 ){
            Pid_high = OBJTYPE( Obj->Pid );
            if( Pid_high == 1 ){
                n = 4;
            } else if( !Pid_high || Pid_high == 2 ){
                n = 3;
            }
            if( Obj->ScrId != -1 ){
                ScptRemove( Obj->ScrId );
                Obj->ScrId = -1;
            }
            if( ScptNewScript( &Obj->ScrId, n ) != -1 && ScptPtr( Obj->ScrId, &script ) != -1 ){
                script->LocVarId = sid - 1;
                if( n == 1 ){
                    script->HexOrTimer = (Obj->Elevation << 29) | Obj->GridId;
                    script->Radius = 3;
                }
                Obj->TimeEv = ScptNewObjId();
                script->i08 = Obj->TimeEv;
                script->TimeEv = Obj;
                ScptSetupLocalVars( sid - 1, Obj->ScrId );
            }
        }
    } else {
        eprintf( "\nError: attempt to Create critter With PID of 0: %s!", scr->FileName );
    }
    RETPTR( scr, Obj );
}

/*
    int destroy_object( ObjectPtr obj ) 
    - Destroys an object (obj), which will cause it’s script to be 
     called in the destroy_proc section if the object is *NOT* the calling object.
*/
void ScrGame_DestroyObject( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj, *Owner;
    VidRect_t Area, v14;
    uint16_t type;
    int v3;

    scr->Flags |= SCR_FPROC_RUN;
    v3 = 0;
    GETARGP( scr, type, obj, 0, "destroy_object" );
    SCP_DBGA( "destroy_object( [%x]%p )", type, obj );
    if( !obj ){
        ScrGameErrorMsg( scr, "destroy_object", 1 );
        scr->Flags &= ~SCR_FPROC_RUN;
        return;
    }
    if( OBJTYPE( obj->Pid ) == 1 && LsgPending() ){
        eprintf( "\nError: attempt to destroy critter in load/save-game: %s!", scr->FileName );
        scr->Flags &= ~SCR_FPROC_RUN;
        return;
    }
    if( obj == ScptGetSelfObj( scr ) ) v3 = 1;
    if( OBJTYPE( obj->Pid ) == 1 ) CombatUnk79( obj );
    Owner = ObjGetOwner( obj );
    if( Owner ){
        ItemUseItem( Owner, obj, Item33( Owner, obj ) );
        if( Owner == gObjDude ) IfaceHandSlotUpdate( GameIfaceStat() == 0, -1, -1 );
        ObjUnk14( obj, 1u, 0, 0 );
        if( v3 ){
            obj->ScrId = -1;
            obj->Flags |= 0x05;
        } else {
            AnimRegClear( obj );
            ObjDestroy( obj, 0 );
        }
    } else {
        AnimRegClear( obj );
        ObjDestroy( obj, &Area );
        TileUpdateArea( &v14, gMapCurrentLvl );
    }
    scr->Flags &= ~SCR_FPROC_RUN;
    if( v3 ) scr->Flags |= 0x1000000;    
}

/*
    void display_msg( string message ) - displays a message on the information display
*/
void ScrGame_DisplayMsg( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int dbg, val;

    GETARGS( scr, type, val, 0, "display_msg" );
    SCP_DBGA( "display_msg( [%x]%i )", type, val );
    IfcMsgOut( IntpGetString( scr, type >> 8, val ) );
    CfgGetInteger( &gConfiguration, "debug", "show_script_messages", &dbg );
}

/*
    void script_overrides - C-engine reports that the script overrides the default behavior of the object
*/
void ScrGame_ScriptOverrides( Intp_t *scr )
{
    SCP_DBG_VAR;
    Scpt_t *script;

    SCP_DBGA( "script_overrides" );
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "script_overrides", 2 );
    else
        script->OverrideFlag = 1;
}

/*
    int obj_is_carrying_obj( ObjectPtr invenObj, ObjectPtr itemObj ) 
    - returns number of itemObj inside invenObj's inventory, note that both arguments are object pointers    
*/
void ScrGame_ObjIsCarryingObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj;
    int item_pid, QuantityTot;
    uint16_t type[ 2 ];
    
    QuantityTot = 0;	
    GETARGI( scr, type[ 0 ], item_pid, 0, "obj_is_carrying_obj" );
    GETARGP( scr, type[ 1 ], obj, 1, "obj_is_carrying_obj" );
    SCP_DBGA( "obj_is_carrying_obj( [%x]%p, [%x]%i )", type[1], obj, type[0], item_pid );
    if( obj )
        QuantityTot = InvGetQuantityTot( obj, item_pid );
    else
        ScrGameErrorMsg( scr, "obj_is_carrying_obj_pid", 1 );
    RETINT( scr, QuantityTot );
}

/*
    boolean tile_contains_obj_pid (int tile, int elev, int pid) - check whether a given hex object with the specified PID    
*/
void ScrGame_TileContainsObjPid( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *p;
    int reply, val[ 3 ];
    uint16_t type[ 3 ];

    reply = 0;
    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "tile_contains_obj_pid" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "tile_contains_obj_pid" );
    GETARGI( scr, type[ 2 ], val[ 2 ], 2, "tile_contains_obj_pid" );
    SCP_DBGA( "tile_contains_obj_pid( [%x]%i, [%x]%i, [%x]%i )", type[2], val[2], type[1], val[1], type[0], val[0] );
    for( p = ObjGetFirst( val[ 1 ], val[ 2 ] ); p; p = ObjGetNext() ){
        if( val[ 0 ] == p->Pid ){ reply = 1; break; }
    }
    RETINT( scr, reply );
}

/*
    ObjectPtr self_obj - returns a pointer to an object in a script which is executed
*/
void ScrGame_SelfObj( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "self_obj" );
    RETPTR( scr, ScptGetSelfObj( scr ) );
}

/*
    ObjPtr source_obj - returns a pointer to the source object (activator) for this action
*/
void ScrGame_SourceObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *crit;
    Scpt_t *script;

    SCP_DBGA( "source_obj" );
    crit = 0;
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "source_obj", 2 );
    else
        crit = script->SourceObj;
    RETPTR( scr, crit );
}

/*
    ObjectPtr target_obj - returns a pointer to the object on which the action takes place
*/
void ScrGame_TargetObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    int ActionSource;
    Scpt_t *script;
    Obj_t *obj;

    SCP_DBGA( "target_obj" );
    ActionSource = ScptGetActionSource(scr);
    obj = NULL;
    if( ScptPtr( ActionSource, &script ) == -1 )
        ScrGameErrorMsg( scr, "target_obj", 2 );
    else
        obj = script->TargetObj;
    RETPTR( scr, obj );
}

/*
    ObjectPtr dude_obj - returns a pointer to Player
*/
void ScrGame_DudeObj( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "dude_obj" );
    RETPTR( scr, gObjDude );
}

/*
    ObjectPtr obj_being_used_with() - Returns a pointer to the object being used on another object.
*/
void ScrGame_ObjBeingUsedWith( Intp_t *scr )
{
    SCP_DBG_VAR;
    Scpt_t *script;
    Obj_t *obj;

    SCP_DBGA( "obj_being_used_with" );
    obj = NULL;
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "obj_being_used_with", 2 );
    else
        obj = script->TargetObj;
    RETPTR( scr, obj );
}

/*
    int local_var( unsigned int var_index ) - returns the value of the specified local variable
*/
void ScrGame_OpLocalvar( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int var, tmp;

    tmp = -1;
    GETARGI( scr, type, var, 0, "op_local_var" );
    SCP_DBGA( "op_local_var( [%x]%i )", type, var );
    ScptGetLocVar( ScptGetActionSource( scr ), var, &tmp );
    RETINT( scr, tmp );
}

/*
    void set_local_var( unsigned int var_index, int value ) - Sets the value of a local variable (var_index) to a given (value).
*/
void ScrGame_SetLocalvar( Intp_t *scr )
{
    SCP_DBG_VAR;
    int var[ 2 ];
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], var[ 0 ], 0, "set_local_var" );
    GETARGI( scr, type[ 1 ], var[ 1 ], 1, "set_local_var" );
    SCP_DBGA( "set_local_var( [%x]%i, [%x]%i )", type[1], var[1], type[0], var[0] );
    ScptSetLocVar( ScptGetActionSource( scr ), var[ 1 ], var[ 0 ] );
}

/*
    int map_var( unsigned int var_index ) - returns the value of a given variable maps
*/
void ScrGame_OpMapVar( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int var;

    GETARGI( scr, type, var, 0, "op_map_var" );
    SCP_DBGA( "op_map_var( [%x]%i )", type, var );
    RETINT( scr, MapGetGlobalVar( var ) );
}

/*
    void set_map_var( unsigned int var_index, int value ) - assign a value to a variable card
*/
void ScrGame_SetMapVar( Intp_t *scr )
{
    SCP_DBG_VAR;
    int var[ 2 ];
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], var[ 0 ], 0, "set_map_var" );
    GETARGI( scr, type[ 1 ], var[ 1 ], 1, "set_map_var" );
    SCP_DBGA( "set_map_var( [%x]%i, [%x]%i )", type[1], var[1], type[0], var[0] );
    MapSetGlobalVar( var[ 1 ], var[ 0 ] );
}

/*
    int global_var( unsigned int var_index ) - returns the value of the specified global variable
*/
void ScrGame_OpGlobalVar( Intp_t *scr )
{
    SCP_DBG_VAR;
    int v2, var;
    uint16_t type;

    GETARGI( scr, type, var, 0, "op_global_var" );
    SCP_DBGA( "op_global_var( [%x]%i )", type, var );
    v2 = -1;
    if( gGValCount )
        v2 = GlobVarGet( var );
    else
        ScrGameEprintf( "\nScript Error: %s: op_global_var: no global vars found!", scr->FileName );
    RETINT( scr, v2 );
}

/*
    void set_global_var (unsigned int var_index, int value) - assign a value to a global variable
*/
void ScrGame_OpSetGlobalVar( Intp_t *scr )
{
    SCP_DBG_VAR;
    int var[ 2 ];
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], var[ 0 ], 0, "set_global_var" );
    GETARGI( scr, type[ 1 ], var[ 1 ], 1, "set_global_var" );
    SCP_DBGA( "set_global_var( [%x]%i, [%x]%i )", type[1], var[1], type[0], var[0] );
    if( gGValCount )
        GlobVarSet( var[1], var[0] );
    else
        ScrGameEprintf( "\nScript Error: %s: op_set_global_var: no global vars found!", scr->FileName );
}

/*
    int script_action - returns the number of the event handler for the action which triggered the script
*/
void ScrGame_ScriptAction( Intp_t *scr )
{
    SCP_DBG_VAR;
    int v4;
    Scpt_t *script;

    v4 = 0;
    SCP_DBGA( "script_action" );
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "script_action", 2 );
    else
        v4 = script->ActionEventId;
    RETINT( scr, v4 );
}

/*
    int obj_type( ObjectPtr obj ) - Returns the type of an object (obj). This would be ‘Item’, ‘Wall’, ‘Scenery’, etc.
*/
void ScrGame_OpObjType( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj;
    int ObjType;
    uint16_t type;

    ObjType = -1;
    GETARGP( scr, type, obj, 0, "op_obj_type" );
    SCP_DBGA( "op_obj_type( [%x]%p )", type, obj );
    if( obj ) ObjType = OBJTYPE( obj->ImgId );
    RETINT( scr, ObjType );
}

/*
    int obj_item_subtype( ObjectPtr obj ) - Returns the subtype of an object of type 'item'. Examples would be food, armor, weapons, etc.
*/
void ScrGame_OpItemSubtype( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj;
    Proto_t *proto;
    uint16_t type;
    int subtype;

    GETARGP( scr, type, obj, 0, "op_item_subtype" );
    SCP_DBGA( "op_item_subtype( [%x]%p )", type, obj );
    if( obj ){
        if( OBJTYPE( obj->Pid ) ){
            subtype = -1;
        } else {
            subtype = ProtoGetObj( obj->Pid, &proto );
            if( subtype != -1 ) subtype = ItemGetObjType( obj );
        }
    } else {
        subtype = -1;
    }
    RETINT( scr, subtype );
}

/*
    int get_critter_stat( ObjectPtr who, int stat ) - return the value of the characteristic
*/
void ScrGame_GetCritterStat( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj;
    int r, val;
    uint16_t type[2];

    GETARGI( scr, type[ 0 ], val, 0, "get_critter_stat" );
    GETARGP( scr, type[ 1 ], obj, 1, "get_critter_stat" );
    SCP_DBGA( "get_critter_stat( [%x]%p, [%x]%i )", type[1], obj,  type[0], val );
    if( obj ){
        r = FeatGetVal( obj, val );
    } else {
        ScrGameErrorMsg( scr, "get_critter_stat", 1 );
        r = -1;
    }
    RETINT( scr, r );
}

/*
    int set_critter_stat( ObjectPtr who, int stat, int val ) - Sets the value of a desired attribute/stat in a critter (who) to a given value (val).
*/
void ScrGame_SetCritterStat( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type[ 3 ];
    Obj_t *who;
    int stat, amount, err;

    GETARGI( scr, type[ 0 ], amount, 0, "set_critter_stat" );
    GETARGI( scr, type[ 1 ], stat, 1, "set_critter_stat" );
    GETARGP( scr, type[ 2 ], who, 2, "set_critter_stat" );
    SCP_DBGA( "set_critter_stat( [%x]%p, [%x]%i, [%x]%i )", type[2], who, type[1], stat, type[0], amount );
    err = 0;
    if( who ){
        if( who == gObjDude ){
            FeatSetBase( who, stat, FeatGetTotal( who, stat ) + amount );
        } else {
            ScrGameErrorMsg( scr, "set_critter_stat", 3 );
            eprintf( " Can't modify anyone except obj_dude!" );
            err = -1;
        }
    } else {
        ScrGameErrorMsg( scr, "set_critter_stat", 1 );
        err = -1;
    }
    RETINT( scr, err );
}

/*
    void animate_stand_obj( ObjectPtr obj ) - plays the animation with the number 0 for a given object
*/
void ScrGame_AnimateStandObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Scpt_t *script;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "animate_stand_obj" );
    SCP_DBGA( "animate_stand_obj( [%x]%p )", type, obj );
    if( !obj ){
        if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ){
            ScrGameErrorMsg( scr, "animate_stand_obj", 2 );
            return;
        }
        ScptGetSelfObj( scr );
    }
    if( !IN_COMBAT ){
        AnimRegStart( 1 );
        AnimRegAnimation( obj, 0, 0 );
        AnimRegEnd();
    }
}

/*
    void animate_stand_reverse_obj (ObjectPtr obj) - plays the animation with the number 0 for a given object backwards
*/
void ScrGame_AnimateStandReverseObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;
    int ActionSource;
    Scpt_t *script;

    GETARGP( scr, type, obj, 0, "animate_stand_reverse_obj" );
    SCP_DBGA( "animate_stand_reverse_obj( [%x]%p )", type, obj );
    if( !obj ){
        ActionSource = ScptGetActionSource( scr );
        if( ScptPtr( ActionSource, &script ) == -1 ){
            ScrGameErrorMsg( scr, "animate_stand_reverse_obj", 2 );
            return;
        }
        ScptGetSelfObj( scr );
    }
    if( !IN_COMBAT ){
        AnimRegStart( 1 );
        AnimRegAnimReverse( obj, 0, 0 );
        AnimRegEnd();
    }
}

/*
    void animate_move_obj_to_tile( ObjectPtr who, int tile, int speed ) - animation move the object to the specified hex
*/
void ScrGame_AnimateMoveObjToTile( Intp_t *scr )
{
    SCP_DBG_VAR;
    Scpt_t *script;
    Obj_t *who;
    uint16_t type[ 3 ];
    int TileNo, Speed;

    GETARGI( scr, type[ 0 ], Speed, 0, "animate_move_obj_to_tile" );
    GETARGI( scr, type[ 1 ], TileNo, 0, "animate_move_obj_to_tile" );
    GETARGP( scr, type[ 2 ], who, 0, "animate_move_obj_to_tile" );
    SCP_DBGA( "animate_move_obj_to_tile( [%x]%p, [%x]%i, [%x]%i )", type[2], who, type[1], TileNo, type[0], Speed );
    if( who ){
        if( TileNo > -1 ){
            if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ){
                ScrGameErrorMsg( scr, "animate_move_obj_to_tile", 2 );
            } else if( CritterCanTalk( who ) && !IN_COMBAT ){
                if( Speed & 0x10 ){
                    AnimRegClear( who );
                    Speed &= ~0x10u;
                }
                AnimRegStart( 1 );
                if( Speed )
                    AnimObjRunToTile( who, TileNo, who->Elevation, -1, 0 );
                else
                    AnimObjMoveToTile( who, TileNo, who->Elevation, -1, 0 );
                AnimRegEnd();
            }
        }
    } else {
        ScrGameErrorMsg( scr, "animate_move_obj_to_tile", 1 );
    }
}

/*
    boolean tile_in_tile_rect( int tile1, int tile2, int tile3, int tile4, int tile ) - determines if the point (hex) rectangle
*/
void ScrGame_TileInTileRect( Intp_t *scr )
{
    SCP_DBG_VAR;
    int tmp[ 10 ], val[ 5 ], InTile;
    uint16_t type[ 5 ];

    InTile = 0;
    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "tile_in_tile_rect" ); tmp[ 0*2 + 1 ] = val[ 0 ] % 200; tmp[ 0*2 + 0 ] = val[ 0 ] / 200;
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "tile_in_tile_rect" ); tmp[ 1*2 + 1 ] = val[ 1 ] % 200; tmp[ 1*2 + 0 ] = val[ 1 ] / 200;
    GETARGI( scr, type[ 2 ], val[ 2 ], 2, "tile_in_tile_rect" ); tmp[ 2*2 + 1 ] = val[ 2 ] % 200; tmp[ 2*2 + 0 ] = val[ 2 ] / 200;
    GETARGI( scr, type[ 3 ], val[ 3 ], 3, "tile_in_tile_rect" ); tmp[ 3*2 + 1 ] = val[ 3 ] % 200; tmp[ 3*2 + 0 ] = val[ 3 ] / 200;
    GETARGI( scr, type[ 4 ], val[ 4 ], 4, "tile_in_tile_rect" ); tmp[ 4*2 + 1 ] = val[ 4 ] % 200; tmp[ 4*2 + 0 ] = val[ 4 ] / 200;
    SCP_DBGA( "tile_in_tile_rect( [%x]%i, [%x]%i, [%x]%i, [%x]%i, [%x]%i )", type[4], val[4], type[3], val[3], type[2], val[2], type[1], val[1], type[0], val[0] );
    if( tmp[0] <= tmp[8] && tmp[0] >= tmp[2] && tmp[1] >= tmp[9] && tmp[1] <= tmp[3] ) InTile = 1;
    RETINT( scr, InTile );
}

/*
    make_day_time
    - dummy function
*/
void ScrGame_MakeDayTime( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "make_day_time" );
    /* Empty */
}

/*
    int tile_distance( int tile1, int tile2 ) - returns the distance between two hexes
*/
void ScrGame_TileDistance( Intp_t *scr )
{
    SCP_DBG_VAR;
    int Distance, val[ 2 ];
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "tile_distance" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "tile_distance" );
    SCP_DBGA( "tile_distance( [%x]%i, [%x]%i )", type[1], val[1], type[0], val[0] );
    if( val[0] == -1 || val[1] == -1 )
        Distance = 9999;
    else
        Distance = TileGetDistance( val[ 1 ], val[ 0 ] );
    RETINT( scr, Distance );
}

/*
    int tile_distance_objs( ObjectPtr obj1, ObjectPtr obj2 ) - returns the distance between two objects
*/
void ScrGame_TileDistanceObjs( Intp_t *scr )
{
    SCP_DBG_VAR;
    int Distance;
    Obj_t *val[2];
    uint16_t type[2];

    GETARGP( scr, type[ 0 ], val[ 0 ], 0, "tile_distance_objs" );
    GETARGP( scr, type[ 1 ], val[ 1 ], 1, "tile_distance_objs" );
    SCP_DBGA( "tile_distance_objs( [%x]%p, [%x]%p )", type[1], val[1], type[0], val[0]);
    if( !val[0] || !val[1] ){
        Distance = 9999;
    } else {
	if( val[0] >= (Obj_t *)40000 && val[1] >= (Obj_t *)40000 ){
    	    if( val[0]->Elevation == val[1]->Elevation ){
        	if( val[1]->GridId != -1 && val[0]->GridId != -1 ){
            	    Distance = TileGetDistance( val[1]->GridId, val[0]->GridId );
        	} else {
        	    Distance = 9999;
        	}
    	    } else {
    		Distance = 9999;
    	    }
	} else {
	    ScrGameErrorMsg( scr, "tile_distance_objs", 3 );
	    eprintf( " Passed a tile # instead of an object!!!BADBADBAD!" );
	    Distance = 9999;
	}
    }
    RETINT( scr, Distance );
}

/*
    int tile_num( ObjectPtr obj ) - Returns the tile number of object (obj).
*/
void ScrGame_TileNuM( Intp_t *scr )
{
    SCP_DBG_VAR;
    int idx;
    Obj_t *obj;
    uint16_t type;

    idx = -1;
    GETARGP( scr, type, obj, 0, "tile_num" );
    SCP_DBGA( "tile_num( [%x]%p )", type, obj );
    if( obj )
        idx = obj->GridId;
    else
        ScrGameErrorMsg( scr, "tile_num", 1 );
    RETINT( scr, idx );
}

/*
    int tile_num_in_direction( int start_tile, int dir, int distance ) - hexa number, located at a distance predetermined in a predetermined direction from said hexa
*/
void ScrGame_TileNumInDirection( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val[3], num;
    uint16_t type[3];

    num = -1;
    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "tile_num_in_direction" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "tile_num_in_direction" );
    GETARGI( scr, type[ 2 ], val[ 2 ], 2, "tile_num_in_direction" );
    SCP_DBGA( "tile_num_in_direction( [%x]%i, [%x]%i, [%x]%i )", type[2], val[2], type[1], val[1], type[0], val[0] );
    if( val[ 2 ] == -1 ){
        ScrGameErrorMsg( scr, "tile_num_in_direction", 3 );
        eprintf( " tileNum is -1!" );
    } else if( val[ 1 ] >= 6 ){
        ScrGameErrorMsg( scr, "tile_num_in_direction", 3 );
        num = val[ 2 ];
        eprintf( " rotation out of Range!" );
    } else if( val[0] ){
        num = TileGetTileNumInDir( val[2], val[1], val[0] );
        if( num < -1 ){
            eprintf( "\nError: %s: op_tile_num_in_direction got #: %d", scr->FileName, num );
            num = -1;
        }
    }
    RETINT( scr, num );
}

/*
    void pickup_obj( ObjectPtr obj ) - Critter, the script is called, this function raises the specified object from the ground
*/
void ScrGame_PickupObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;
    Scpt_t *script;

    GETARGP( scr, type, obj, 0, "pickup_obj" );
    SCP_DBGA( "pickup_obj( [%x]%p )", type, obj );
    if( obj ){
        if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ){
            ScrGameErrorMsg( scr, "pickup_obj", 2 );
        } else if( script->TargetObj ){
            ActionUseOnItem( script->TargetObj, obj );
        } else {
            ScrGameErrorMsg( scr, "pickup_obj", 1 );
        }
    }
}

/*
    void drop_obj( ObjectPtr obj ) - Critter orders, in a script that is performing this function to reset the object to the ground
*/
void ScrGame_DropObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;
    Scpt_t *script;

    GETARGP( scr, type, obj, 0, "drop_obj" );
    SCP_DBGA( "drop_obj( [%x]%p )", type, obj );
    if( obj ){
        if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ){
            ScrGameErrorMsg( scr, "drop_obj", 1 );
        } else if( script->TargetObj ){
            UseDropObj( script->TargetObj, obj );
        } else {
            ScrGameErrorMsg( scr, "drop_obj", 2 );
        }
    }
}

/*
    void add_obj_to_inven( ObjectPtr who, ObjectPtr item ) - add the item to your inventory object
*/
void ScrGame_AddObjToInv( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type[ 2 ];
    Obj_t *item[ 2 ];
    VidRect_t Area;

    GETARGP( scr, type[ 0 ], item[ 0 ], 0, "add_obj_to_inven" );
    GETARGP( scr, type[ 1 ], item[ 1 ], 1, "add_obj_to_inven" );
    SCP_DBGA( "add_obj_to_inven( [%x]%p, [%x]%p )", type[1], item[1], type[0], item[0] );
    if( item[ 1 ] && item[ 0 ] ){
        if( item[0]->Owner ){
            ScrGameErrorMsg( scr, "add_obj_to_inven", 3 );
            eprintf( " Item was already attached to something else!" );
        } else if( !ItemAdd( item[ 1 ], item[ 0 ], 1 ) ){
            ObjLightItem( item[ 0 ], &Area );
            TileUpdateArea( &Area, item[ 0 ]->Elevation );
        }
    }
}

/*
    void rm_obj_from_inven(ObjectPtr who, ObjectPtr obj) 
    - Removes an object (obj) from another object’s (who’s) inventory. 
    Note: this leaves the removed object in at location (0,1) on the map! You must call move_to(…) to place it back on the map.
*/
void ScrGame_RmObjFromInv( Intp_t *scr )
{
    SCP_DBG_VAR;
    int k, flags;
    VidRect_t Area;
    Obj_t *obj[2];
    uint16_t type[2];

    flags = 0;
    k = 0;
    GETARGP( scr, type[ 0 ], obj[ 0 ], 0, "rm_obj_from_inven" );
    GETARGP( scr, type[ 1 ], obj[ 1 ], 1, "rm_obj_from_inven" );
    SCP_DBGA( "rm_obj_from_inven( [%x]%p, [%x]%p )", type[1], obj[1], type[0], obj[0] );
    if( obj[ 1 ] && obj[ 0 ] ){
        if( obj[ 0 ]->Flags & 0x07000000 ){
            if( obj[ 0 ]->Flags & 0x01000000 ) flags |= 0x01000000;
            if( obj[ 0 ]->Flags & 0x02000000 ) flags |= 0x02000000;
            if( obj[ 0 ]->Flags & 0x04000000 ) flags |= 0x04000000;
            k = 1;
        }
        if( !ItemUseItem( obj[ 1 ], obj[ 0 ], 1 ) ){
            ObjUnk14( obj[ 0 ], 1, 0, &Area );
            TileUpdateArea( &Area, obj[ 0 ]->Elevation );
            if( k ) ScrGameSetShape( obj[ 1 ], obj[ 0 ], flags );
        }
    }
}

/*
    void wield_obj_critter (ObjectPtr who, ObjectPtr obj) - equips critter (who) object (obj)
*/
void ScrGame_WieldObjCritter( Intp_t *scr )
{
    SCP_DBG_VAR;
    int v10, v14, v16;
    uint16_t type[2];
    Obj_t *obj[2], *v13, *ArmorObj;

    v16 = 0;
    ArmorObj = 0;
    v13 = 0;
    v14 = 1;
    GETARGP( scr, type[ 0 ], obj[ 0 ], 0, "wield_obj_critter" );
    GETARGP( scr, type[ 1 ], obj[ 1 ], 1, "wield_obj_critter" );
    SCP_DBGA( "wield_obj_critter( [%x]%p, [%x]%p )", type[1], obj[1], type[0], obj[0] );
    if( !obj[0] || !obj[1] ){
        ScrGameErrorMsg( scr, "wield_obj_critter", 1 );
        return;
    }
    if( OBJTYPE( obj[1]->Pid ) != 1 ){
        ScrGameErrorMsg( scr, "wield_obj_critter", 3 );
        eprintf( " Only works for critters!  ERROR ERROR ERROR!" );
        return;
    }
    if( obj[1] == gObjDude ){
        if ( !IfaceGetSelectedHand() ) v14 = 0;
        if ( !ItemGetObjType(obj[0]) ) ArmorObj = InvGetArmorObj(gObjDude);
        v16 = 1;
        v13 = obj[0];
    }
    if( InvUnk28( obj[1], obj[0], v14 ) == -1 ){
        ScrGameErrorMsg( scr, "wield_obj_critter", 3 );
        eprintf( " inven_wield failed!  ERROR ERROR ERROR!");
    } else if( obj[1] == gObjDude ){
        if( v16 ) InvUpdateStatistics( obj[1], ArmorObj, v13 );
        if( GameIfaceStat() ) v10 = 0;
        IfaceHandSlotUpdate( v10, -1, -1 );
    }
}

/*
    void use_obj( ObjectPtr obj ) - use the object
*/
void ScrGame_UseObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    Scpt_t *script;
    Obj_t *obj, *item;
    uint16_t type;

    GETARGP( scr, type, obj, 0, "use_obj" );
    SCP_DBGA( "use_obj( [%x]%p )", type, obj );
    if( !obj ) goto err;
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ) goto err;
    if( script->TargetObj ){
        item = ScptGetSelfObj( scr );
        if( OBJTYPE( item->Pid ) == 1 )
            ActionUseOnScenery( script->TargetObj, obj );
        else
            UseObject( item, obj, obj );
    } else {
err:
        ScrGameErrorMsg( scr, "use_obj", 1 );
    }
}

/*
    boolean obj_can_see_obj( ObjectPtr src_obj, ObjectPtr dst_obj )
     - Returns True if the source object (src_obj) has line-of-sight (LOS) with the destination object (dst_obj). 
     This also takes into account perception & stealth rolls of the objects are critters.
*/
void ScrGame_ObjCanSeeObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    int n;
    uint16_t type[ 2 ];
    Obj_t *DstObj, *pObj, *SrcObj;    

    pObj = NULL;
    n = 0;
    GETARGP( scr, type[ 0 ], DstObj, 0, "obj_can_see_obj" );
    GETARGP( scr, type[ 1 ], SrcObj, 1, "obj_can_see_obj" );
    SCP_DBGA( "obj_can_see_obj( [%x]%p, [%x]%p )", type[1], SrcObj, type[0], DstObj );
    if( DstObj && SrcObj ){
        if ( DstObj->GridId != -1 ){
            if( DstObj == gObjDude ) CritterUsingSkill( 0 );
//            FeatGetVal( SrcObj, FEAT_PERCEPTION ); // return ignored !
            if( AiObjCanHearObj( SrcObj, DstObj ) ){
                AnimUnk06( SrcObj, SrcObj->GridId, DstObj->GridId, 0, &pObj, 16 );
                if( DstObj == pObj ) n = 1;
            }
        }
    } else {
        ScrGameErrorMsg( scr, "obj_can_see_obj", 1 );
    }
    RETINT( scr, n );
}

/*
 => makes the critter, the script is called, to attack the specified object with different modifiers
void attack_complex( 
7	ObjectPtr who, 
6	int called_shot, 
5	int num_attacks, 
4	int bonus, 
3	int min_damage, 
2	int max_damage, 
1	int attacker_results, 
0	int target_results
    ) 
*/
void ScrGame_Attack( Intp_t *scr )
{
    SCP_DBG_VAR;
    Scpt01_t tmp;
    Obj_t *who, *p;
    int target_results, attacker_results, max_damage, min_damage, bonus, num_attacks, called_shot;
    uint16_t type[ 8 ];

    GETARGI( scr, type[ 0 ], target_results, 0, "attack" );
    GETARGI( scr, type[ 1 ], attacker_results, 1, "attack" );
    GETARGI( scr, type[ 2 ], max_damage, 2, "attack" );
    GETARGI( scr, type[ 3 ], min_damage, 3, "attack" );
    GETARGI( scr, type[ 4 ], bonus, 4, "attack" );
    GETARGI( scr, type[ 5 ], num_attacks, 5, "attack" );
    GETARGI( scr, type[ 6 ], called_shot, 6, "attack" );
    GETARGP( scr, type[ 7 ], who, 7, "attack" );
    SCP_DBGA( "attack( [%x]%p, [%x]%i, [%x]%i, [%x]%i, [%x]%i, [%x]%i, [%x]%i, [%x]%i )", 
	type[7], who, type[6], called_shot, type[5], num_attacks, type[4], bonus, 
	    type[3], min_damage, type[2], max_damage, type[1], attacker_results, type[0], target_results );
    if( !who ){
        ScrGameErrorMsg( scr, "attack", 1 );
        return;
    }
    scr->Flags |= SCR_FPROC_RUN;
    if( !(p = ScptGetSelfObj( scr )) ){
        scr->Flags &= ~SCR_FPROC_RUN;
        return;
    }
    if( !CritterCanTalk( p ) || ( p->Flags & 0x01 ) ){
        eprintf( "\n   But is already Inactive (Dead/Stunned/Invisible)" );
        scr->Flags &= ~SCR_FPROC_RUN;
        return;
    }
    if( !CritterCanTalk( who ) || ( who->Flags & 0x01) ){
        eprintf( "\n   But target is already dead or invisible" );
        scr->Flags &= ~SCR_FPROC_RUN;
        return;
    }
    if( who->Critter.State.Reaction & 0x04 ){
        eprintf( "\n   But target is AFRAID" );
        scr->Flags &= ~SCR_FPROC_RUN;
        return;
    }
    if( IN_DIALOG ) return;
    if( IN_COMBAT ){
        if( !(p->Critter.State.Reaction & 0x01) ){
            p->Critter.State.Reaction |= 0x01;
            p->Critter.State.WhoHitMeObj = who;
        }
    } else {
        tmp.i03[ 0 ] = 0;
        tmp.i03[ 1 ] = bonus;
        tmp.i03[ 2 ] = 0;
        tmp.i03[ 3 ] = min_damage;
        tmp.obj = p;
        tmp.i07 = max_damage;
        tmp.crit = who;
        if( attacker_results == target_results ){
            tmp.i08 = 1;
            tmp.i10 = target_results;
            tmp.i09 = attacker_results;
        } else {
            tmp.i08 = 0;
        }
        ScptUnk121( &tmp );
    }
    scr->Flags &= ~SCR_FPROC_RUN;
}

/*
    void start_gdialog( int msgFileNum, ObjectPtr who, int mood, int headNum, int backgroundIdx ) - to start a dialogue
*/
void ScrGame_StartGdialog( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *who;
    Proto_t *proto;
    int n, BackGroundIdx, HeadNum, Mood, MsgFileNum;
    uint16_t type[ 5 ];
    
    GETARGI( scr, type[ 0 ], BackGroundIdx, 0, "start_gdialog" );
    GETARGI( scr, type[ 1 ], HeadNum, 1, "start_gdialog" );
    GETARGI( scr, type[ 2 ], Mood, 2, "start_gdialog" );
    GETARGP( scr, type[ 3 ], who, 3, "start_gdialog" );
    GETARGI( scr, type[ 4 ], MsgFileNum, 4, "start_gdialog" );
    SCP_DBGA( "start_gdialog( [%x]%i, [%x]%p, [%x]%i, [%x]%i, [%x]%i)", type[4], MsgFileNum, type[3], who, type[2], Mood, type[1], HeadNum, type[0], BackGroundIdx );

    if( IN_COMBAT ) return;
    if( !who ){
        ScrGameErrorMsg( scr, "start_gdialog", 1 );
        return;
    }
    gDlgHeadId = -1;
    if( OBJTYPE( who->Pid ) == TYPE_CRIT ){
        if( ProtoGetObj( who->Pid, &proto ) == -1 ) return;
        if( HeadNum != -1 ) gDlgHeadId = ArtMakeId( 8, HeadNum, 0, 0, 0 );
    } else {
	if( HeadNum != -1 ) gDlgHeadId = ArtMakeId( 8, HeadNum, 0, 0, 0 );
    }
    GdialogSetBg( BackGroundIdx );
    gScrGameMood = Mood;
    if( gDlgHeadId != -1 ){
        if( (n = EvQeUnk20( EvQeUnk23( gDlgPartyMemberObj ), 0, 0 )) ){
            if( n <= 1 ){
                gScrGameMood = 4;
            } else if( n == 2 ){
                gScrGameMood = 1;
            }
        } else {
            gScrGameMood = 7;
        }
    }
    gDlgActionSrc = ScptGetActionSource( scr );
    gDlgPartyMemberObj = ScptGetSelfObj( scr );
    GdialogStartConversation( gDlgHeadId, gScrGameMood );
}

/*
    void end_dialogue - complete dialogue
*/
void ScrGame_EndDialogue( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "end_dialogue" );
    if( GdialogUnk06() == -1 ) return;
    gDlgPartyMemberObj = 0;
    gDlgActionSrc = -1;
}

/*
    void dialogue_reaction (int mood) - animates an emotional reaction in the dialogue
*/
void ScrGame_DialogueReaction( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;

    GETARGI( scr, type, val, 0, "dialogue_reaction" );
    SCP_DBGA( "dialogue_reaction( [%x]%i )", type, val );
    gScrGameMood = val;
    GdialogReaction( val );
}

void ScrGameUnk04( int x0, int y0, int x1, int y1, int a5, int a6 )
{
    ScrGameUnk05( x0, y0, x1, y1, a5, a6 );
}

void ScrGameUnk05( int x0, int y0, int x1, int y1, int a5, int v9 )
{
    VidRect_t v15, v16, Area2;
    Obj_t *i;
    int v5, v6, rt, v8, a2;

    Area2.rt = x0;
    Area2.bm = y0;
    v5 = x1;
    a2 = y1;
    memset( &v15, 0, sizeof( VidRect_t ) );
    if( x0 > y0 ){
        Area2.bm = x0;
        Area2.rt = y0;
    }
    v6 = a2;
    if( x1 > a2 ){
        a2 = x1;
        v5 = v6;
    }
    rt = Area2.rt;
    v8 = a2 / 200 - v5 / 200;
    for( ;rt <= Area2.bm; ){
        for( i = ObjGetVisibleObjectFirst( rt ); i; i = ObjGetVisibleObjectNext() ){
            if( (i->Flags & 1) != v9 ) continue;
            if( v5 > i->GridId || i->GridId > a2 ) continue;
            if( (i->GridId - v5) / 200 <= v8 ){
                ObjGetRefreshArea( i, &v16 );
                if( v9 )
            	    i->Flags &= ~0x01;
                else
                    i->Flags |= 0x01;
                RegionExpand( &v15, &v16, &v15 );
            }
        }
        TileUpdateArea( &v15, ++rt );
    }
}

/*
    => another feature of working with a variety of options for the game, is different from the number of arguments metarule
    int metarule3(
3	    int meta3_switch, 
2	    int meta3_par1, 
1	    int meta3_par2, 
0	    int meta3_par3
	) 
    100 -> METARULE3_CLR_FIXED_TIMED_EVENTS
    101 -> METARULE3_MARK_SUBTILE
    102 -> METARULE3_SET_WM_MUSIC // not implemented
    103 -> METARULE3_GET_KILL_COUNT
    104 -> METARULE3_MARK_MAP_ENTRANCE
    105 -> METARULE3_WM_SUBTILE_STATE
    106 -> METARULE3_TILE_GET_NEXT_CRITTER
    107 -> METARULE3_ART_SET_BASE_FID_NUM
    108 -> METARULE3_TILE_SET_CENTER
    109 -> METARULE3_CHEM_USE_LEVEL
    110 -> METARULE3_CAR_OUT_OF_FUEL
    111 -> METARULE3_MAP_GET_LOAD_AREA
*/
void ScrGame_Metarule3( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *p;
    int obj;
    int v22, v23;
    VidRect_t Area;
    int Id;
    int arg0;
    int arg1;
    int v24;
    int result;
    int meta3_switch;
    Obj_t *p2, *p0;
    uint16_t type[4];

    v23 = 0;
    GETARGIP( scr, type[ 0 ], arg0, p0,0, "metarule3" );
    GETARGI( scr, type[ 1 ], arg1, 1, "metarule3" );
    GETARGIP( scr, type[ 2 ], obj, p2,2, "metarule3" );
    GETARGI( scr, type[ 3 ], meta3_switch, 3, "metarule3" );
    SCP_DBGA( "metarule3( [%x]%i, [%x]%i,  [%x]%i,  [%x]%i)", type[3], meta3_switch, type[2], obj, type[1], arg1, type[0], arg0 );
    switch( meta3_switch ){
	case 100:
	    ScptUnk131( obj, arg1 );
	    EvQeRun( 3, (void *)ScptUnk130 );
	    result = 0;
	    break;
        case 101:
            result = WmUnk70( obj, arg1, arg0 );
            break;
        case 103:
            result = CritterGetKillStat( obj );
            break;
        case 104:
            result = WmUnk45( obj, arg0, arg1 );
            break;
        case 105:
            if( !WmUnk71( obj, arg1, &v22 ) ) v23 = v22;
            result = v23;
            break;
        case 106:
            if( !arg0 ) v24 = 1;
            for( p = ObjGetFirst( arg1, obj ); p; p = ObjGetNext() ){
                if( OBJTYPE( p->Pid ) == 1 && v24 ) break;
                if( p == p0 ) v24 = 1;
            }
            RETPTR( scr, p );
            return;
        case 107:
            Id = ArtMakeId((p2->ImgId & 0xF000000) >> 24, arg1, (p2->ImgId & 0xFF0000u) >> 16, (p2->ImgId & 0xF000) >> 12, (p2->ImgId & 0x70000000) >> 28);
            ObjSetShape(p2, Id, &Area);
            TileUpdateArea( &Area, gMapCurrentLvl );
            result = v23;
            return;
        case 108:
    	    result = TileSetCenter( obj, 1 );
            break;
        case 109:
    	    result = AiGetChemUse( p2 );
            break;
        case 110:
            result = ( WmCarNoFuel() == 1 );
    	    break;
        case 111:
    	    result = MapGetAreaByEntrance();
    	    break;
        default: return;                
    }
    RETINT( scr, result );
}

/*
    void set_map_music( int MapId, string FileName )
*/
void ScrGame_SetMapMusic( Intp_t *scr )
{
    SCP_DBG_VAR;
    char *s;
    int val[ 2 ];
    uint16_t type[ 2 ];

    GETARGS( scr, type[ 0 ], val[ 0 ], 0, "set_map_music" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "set_map_music" );
    SCP_DBGA( "set_map_music( [%x]%i, [%x]%i )", type[1], val[1], type[0], val[0] );
    s = IntpGetString( scr, type[0] >> 8, val[ 0 ] );
    eprintf( "\nset_map_music: %d, %s", val[ 1 ], s );
    WmSetMapMusic( val[ 1 ], s );
}

/*
    void set_obj_visibility ( ObjectPtr obj, boolean visibility ) - sets the visibility flag of the object
*/
void ScrGame_SetObjVisibility( Intp_t *scr )
{
    SCP_DBG_VAR;
    VidRect_t Area;
    Obj_t *obj;
    int val;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], val, 0, "set_obj_visibility" );
    GETARGP( scr, type[ 1 ], obj, 1, "set_obj_visibility" );
    SCP_DBGA( "set_obj_visibility( [%x]%p, [%x]%i )", type[1], obj, type[0], val );

    if( !obj ){
        ScrGameErrorMsg( scr, "set_obj_visibility", 1 );
        return;
    }
    if( LsgPending() ){
        eprintf( "\nError: attempt to set_obj_visibility in load/save-game: %s!", scr->FileName );
        return;
    }
    if( val ){ // set visibility
        if( !(obj->Flags & OBJ_FLG_UNK01 ) ){
            if( IN_COMBAT ){
                ObjUnk35( obj, 0 );
                ObjClrOutline( obj, NULL );
            }
            if( ObjVisibilityEnable( obj, &Area ) != -1 ){
                if( OBJTYPE( obj->Pid ) == TYPE_CRIT ) obj->Flags |= OBJ_FLG_VISIBLE;
                TileUpdateArea( &Area, obj->Elevation );
            }
        }
    } else { // unset visibility
        if( obj->Flags & OBJ_FLG_UNK01 ){
            if( OBJTYPE( obj->Pid ) == TYPE_CRIT ) obj->Flags &= ~OBJ_FLG_VISIBLE;
            if( ObjVisibilityDisable( obj, &Area ) != -1 ) TileUpdateArea( &Area, obj->Elevation );            
        }
    }
}

/*
    => Loads a new map (map_name), removing all scripts currently running and passing on 
	the entrance location (start_location) to the new map’s map_init script. 
	Can also use the number representing the map in the map list (for example, between 0 to 64 in Fallout 1)
    void load_map( string or int map_name, int start_location ) 
*/
void ScrGame_LoadMap( Intp_t *scr )
{
    SCP_DBG_VAR;
    int MapIdxByFileName, val1, val2;
    uint16_t type1, type2;
    char *Arg;
    MapPosition_t tmp;

    GETARGI( scr, type1, val1, 0, "load_map" );
    type2 = IntpPopwA(scr);
    val2 = IntpPopiA( scr );
    if( type2 == SCR_FSTRING ) IntpStringDeRef( scr, SCR_FSTRING, val2 );
    SCP_DBGA( "load_map( [%x]%i, [%x]%i )", type2, val2, type1, val1 );
    MapIdxByFileName = -1;
    Arg = 0;
    if( type2 != SCR_INT ){
        if( (type2 & 0xF7FF) == SCR_STRING )
            Arg = IntpGetString( scr, type2 >> 8, val2 );
        else
            IntpError( "script error: %s: invalid arg 1 to load_map", scr->FileName );
    }
    if( Arg ){
        gGVals[ 27 ] = val1;
        MapIdxByFileName = WmGetMapIdxByFileName( Arg );
    } else if( val2 >= 0 ){
        MapIdxByFileName = val2;
        gGVals[ 27 ] = val1;
    }
    if( MapIdxByFileName != -1 ){
        tmp.MapId = MapIdxByFileName;
        tmp.GridPos = -1;
        tmp.Lvl = -1;
        tmp.Orientation = -1;
        MapSetPos( &tmp );
    }
}

/*
    void wm_area_set_pos (int areaIdx, int xPos, int yPos) - set the world coordinates for the specified city
*/
void ScrGame_WmAreaSetPos( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val[ 3 ];
    uint16_t type[ 16 ];

    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "wm_area_set_pos" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "wm_area_set_pos" );
    GETARGI( scr, type[ 2 ], val[ 2 ], 2, "wm_area_set_pos" );
    SCP_DBGA( "wm_area_set_pos( [%x]%i, [%x]%i, [%x]%i )", type[2], val[2], type[1], val[1], type[0], val[0] );
    if( WmSetPos( val[ 2 ], val[ 1 ], val[ 0 ] ) != -1 ){
        ScrGameErrorMsg( scr, "wm_area_set_pos", 3 );
        eprintf( "Invalid Parameter!" );
    }
}

/*
    void set_exit_grids( elevation markElev, int mapID, int elevation, int tileNum, int rotation) 
    - Sets all exit grids on a given elevation (markElev) to point to a destination mapID (may be -1 which means stay on this map), 
    elevation, tileNum, and rotation.
*/
void ScrGame_SetExitGrids( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *p;
    int val[5];
    uint16_t type[5];

    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "set_exit_grids" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "set_exit_grids" );
    GETARGI( scr, type[ 2 ], val[ 2 ], 2, "set_exit_grids" );
    GETARGI( scr, type[ 3 ], val[ 3 ], 3, "set_exit_grids" );
    GETARGI( scr, type[ 4 ], val[ 4 ], 4, "set_exit_grids" );
    SCP_DBGA( "set_exit_grids( [%x]%i, [%x]%i, [%x]%i, [%x]%i, [%x]%i )", type[4], val[4], type[3], val[3], type[2], val[2], type[1], val[1], type[0], val[0] );
    for( p = ObjGetVisibleObjectFirst( val[4] ); p; p = ObjGetVisibleObjectNext() ){
        if( p->Pid >= 0x5000010 && p->Pid <= 0x5000017 ){
            p->Grid.DestMapId = val[3];
            p->Grid.DestStartPos = val[1];
            p->Grid.DestMapElev = val[2];
        }
    }
}

/*
    boolean anim_busy( ObjectPtr who ) - Checking the status of object animation
*/
void ScrGame_AnimBusy( Intp_t *scr )
{
    SCP_DBG_VAR;
    int busy = 0;
    Obj_t *obj;
    uint16_t type;

    GETARGP( scr, type, obj, 0, "anim_busy" );
    SCP_DBGA( "anim_busy( [%x]%p )", type, obj );
    if( obj )
        busy = AnimUnk39( obj );
    else
        ScrGameErrorMsg( scr, "anim_busy", 1 );
    RETINT( scr, busy );
}

/*
    void critter_heal( ObjectPtr who, int amount )
     - Heals a critter for a given amount (if given a value above their MaxHP will go up to their maximum HP). 
     [ Note: can also input negative amounts, causing the critter to be hurt for that amount. 
     This is useful because it bypasses all resistances and thresholds. Sduibek (talk) 18:03, August 4, 2012 (UTC) ]
*/
void ScrGame_CritterHeal( Intp_t *scr )
{
    SCP_DBG_VAR;
    int result, val;
    Obj_t *obj;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], val, 0, "critter_heal" );
    GETARGP( scr, type[ 1 ], obj, 1, "critter_heal" );
    SCP_DBGA( "critter_heal( [%x]%p, [%x]%i )", type[1], obj, type[1], val );
    result = CritterHeal( obj, val );
    if( gObjDude == obj ) IfaceRenderHP( 1 );
    RETINT( scr, result );
}

/*
    void set_light_level(int level) - set the level of illumination map 0 - 100%
*/
void ScrGame_SetLightLevel( Intp_t *scr)
{
    SCP_DBG_VAR;
    uint16_t type;
    int val, light;

    GETARGI( scr, type, val, 0, "set_light_level" );
    SCP_DBGA( "set_light_level( [%x]%i )", type, val );
    if( val >= 50 ){
        if( val == 50 ){
            LightMapSetLt( gScrGameLightLvl[1], 1 );
            return;
        }
        light = gScrGameLightLvl[1] + val * (gScrGameLightLvl[2] - gScrGameLightLvl[1]) / 100;
    } else {
        light = val * (gScrGameLightLvl[1] - 16384) / 100 + 16384;
    }
    LightMapSetLt( light, 1 );
}

/*
    int game_time() - Returns the current game time in ticks.
*/
void ScrGame_GameTime( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint32_t time;

    SCP_DBGA( "game_time" );
    time = ScptGetGameDekaSeconds();
    RETINT( scr, time );
}

/*
    int game_time_in_seconds - returns the current game time (the time elapsed from the start of the game) in seconds
*/
void ScrGame_GameTimeInSeconds( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "game_time_in_seconds" );
    RETINT( scr, ScptGetGameDekaSeconds() / 10 );
}

/*
    int elevation( ObjectPtr obj ) - Returns the current elevation being displayed.
*/
void ScrGame_Elevation( Intp_t *scr )
{
    SCP_DBG_VAR;
    int Elevation = 0;
    Obj_t *obj;
    uint16_t type;

    GETARGP( scr, type, obj, 1, "elevation" );
    SCP_DBGA( "elevation( [%x]%p )", type, obj );
    if( obj )
        Elevation = obj->Elevation;
    else
        ScrGameErrorMsg( scr, "elevation", 1 );
    RETINT( scr, Elevation );
}

/*
    void kill_critter( ObjectPtr obj, int death_frame )
     - Kills a critter (obj) outright, placing it in the chosen death frame. 
     Note: this does NOT animate the critter, and does NOT refresh the screen! 
     It is meant to be used in scripts run when entering/exiting a map (map_init/map_exit).
*/
void ScrGame_KillCritter( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val, flg = 0;
    Obj_t *obj;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], val, 0, "kill_critter" );
    GETARGP( scr, type[ 1 ], obj, 1, "kill_critter" );
    SCP_DBGA( "kill_critter( [%x]%p, [%x]%i )", type[1], obj, type[0], val );
    if( obj ){
        if( LsgPending() ){
            eprintf( "\nError: attempt to destroy critter in load/save-game: %s!", scr->FileName );
        } else {
            scr->Flags |= SCR_FPROC_RUN;
            if( ScptGetSelfObj( scr ) == obj ) flg = 1;
            AnimRegClear( obj );
            CombatUnk79( obj );
            CritterKill( obj, val, 1 );
            scr->Flags &= ~SCR_FPROC_RUN;
            if( flg ) scr->Flags |= 0x1000000;
        }
    } else {
        ScrGameErrorMsg( scr, "kill_critter", 1 );
    }
}

int ScrGamUnk06( Obj_t *obj, int a2, int a3 )
{
    int vl;

    vl = 3;
    if( a2 >= 51 && a2 <= 63 ){
        CfgGetInteger( &gConfiguration, "preferences", "violence_level", &vl );
        if( vl >= 3 ){
    	    if( ArtFileExist( ArtMakeId(1, obj->ImgId & 0xFFF, a2, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 ) ) ) return a2;
        }
        if( a3 ) return 20;
        if( ArtFileExist( ArtMakeId(1, obj->ImgId & 0xFFF, 21, (obj->ImgId & 0xF000) >> 12, obj->Orientation + 1 ) ) ) return 20;
        return 21;
    }
    return a2;
}

/*
    void kill_critter_type( int pid, int death_frame ) - kills all the critters with the given PID and defines the image remains
*/
void ScrGame_KillCritterType( Intp_t *scr )
{
//    SCP_DBG_VAR;
DD
/*
    int v2; // edi
    int v3; // ebp
    __int16 v4; // ax
    int v5; // ecx
    int v6; // eax
    int v7; // ecx
    int v8; // edx
    unsigned __int16 v9; // ax
    Obj_t *p; // ecx
    Obj_t *v11; // ecx
    Obj_t *v12; // ecx
    Obj_t *v13; // ecx MAPDST
    int v14; // eax
    Obj_t *v16; // ecx
    VidRect_t v17; // edx
    Obj_t *v18; // ecx
    VidRect_t v19; // [esp+0h] [ebp-40h] BYREF
    int val[2]; // [esp+10h] [ebp-30h]
    __int16 type[2]; // [esp+18h] [ebp-28h]
    Obj_t *v22; // [esp+1Ch] [ebp-24h]
    int v23; // [esp+20h] [ebp-20h]
    unsigned int v24; // [esp+24h] [ebp-1Ch]

    v2 = 0;
    v3 = 0;
    v23 = 0;
    v24 = 0;
    v22 = 0;

    GETARGI( scr, type[ 0 ], val[0], 0, "kill_critter_type" );
    GETARGI( scr, type[ 1 ], val[1], 1, "kill_critter_type" );
    if( LsgPending() ){
        eprintf( "\nError: attempt to destroy critter in load/save-game: %s!", scr->FileName );
        return;
    }
    scr->Flags |= SCR_FPROC_RUN;
    for( p = ObjGetObjectFirst(); p; p = ObjGetObjectNext() ){
        while( (p->ImgId & 0xFF0000) >> 16 >= 48 ){
            p = ObjGetObjectNext();
            if( !p ) goto LABEL_27;
        }
        if( (p->Flags & 1) == 0 && val[1] == p->Pid && !CritterIsDead( p ) ){
            if( v11 == v22 || v23 > 200 ){
                ScrGameErrorMsg( scr, "kill_critter_type", 3 );
                eprintf( " Infinite loop destroying critters!" );
                scr->Flags &= ~SCR_FPROC_RUN;
                return;
            }
            AnimRegClear( v11 );
            if( val[0] ){
                CombatUnk79( v12 );
                if( val[0] == 1 ){
                    CritterKill( v13, ScrGamUnk06(v13, dword_518ED0[ v2++ ], 1), 1 );
                    if( v2 >= 11 ) v2 = 0;
                } else {
                    CritterKill( v13, 48, 1 );
                }
            } else {
                AnimRegClear( v12 );
                ObjDestroy( v18, &v17 );
                TileUpdateArea( &v19, gMapCurrentLvl );
            }
            v22 = v16;
            v23++;
            ObjGetObjectFirst();
            gMap.Time = ScptGetGameDekaSeconds();
        }
    }
LABEL_27:
    scr->Flags &= ~SCR_FPROC_RUN;
*/
}

/*
    void critter_damage( ObjectPtr who, int dmg_amount ) - Critter given cause damage
*/
void ScrGame_CritterDamage( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *v10, *v16;
    int val, v15;
    uint16_t type[3];

    GETARGI( scr, type[ 0 ], val, 0, "critter_damage" );
    GETARGI( scr, type[ 1 ], v15, 1, "critter_damage" );
    GETARGP( scr, type[ 2 ], v16, 2, "critter_damage" );
    SCP_DBGA( "critter_damage( [%x]%p, [%x]%i, [%x]%i )", type[2], v16, type[1], v15, type[0], val );
    if( v16 ){
        if( OBJTYPE( v16->Pid ) == TYPE_CRIT ){
            v10 = ScptGetSelfObj( scr );
            if( v16->Critter.State.WhoHitMe == -1 ) v16->Critter.State.WhoHitMe = 0;
            ActionUnk05( v16->GridId, v16->Elevation, v15, v15, val & 0xFCFF, (val & 0x200) == 0, val & 0x100 );
            scr->Flags &= ~SCR_FPROC_RUN;
            if( v10 == v16 ) scr->Flags |= 0x1000000;
        } else {
            ScrGameErrorMsg( scr, "critter_damage", 3 );
            eprintf( " Can't call on non-critters!" );
        }
    } else {
        ScrGameErrorMsg( scr, "critter_damage", 1 );
    }
}

/*
    void add_timer_event(ObjectPtr obj, int time, int info) - Adds a timed event call to the queue, at a given time offset, to call an object’s (obj) script. Info is used to let scripts differentiate between timed event calls so that they can be hooked in multiple times. Info is read back by the script using the fixed_param operator. Note that time is in ticks (you can use game_ticks(seconds_num) to get the time in ticks from time in seconds).
*/
void ScrGame_OpAddTimerEvent( Intp_t *scr )
{
    SCP_DBG_VAR;
    int Time, Info;
    Obj_t *obj;
    uint16_t type[3];

    GETARGI( scr, type[ 0 ], Info, 0, "add_timer_event" );
    GETARGI( scr, type[ 1 ], Time, 1, "add_timer_event" );
    GETARGP( scr, type[ 2 ], obj, 2, "add_timer_event" );
    SCP_DBGA( "add_timer_event( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], Time, type[0], Info );
    if( obj )
        ScptAddTimerEvent( obj->ScrId, Time, Info );
    else
        ScrGameEprintf( "\nScript Error: %s: op_add_timer_event: pobj is NULL!", scr->FileName );
}

/*
    void rm_timer_event(ObjectPtr obj) - Removes (clears) all timer events hooked to a given object’s (obj) script.
*/
void ScrGame_RmTimeEvent( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "rm_timer_event" );
    SCP_DBGA( "rm_timer_event( [%x]%p )", type, obj );
    if( obj )
        EvQeRmEvent( obj );
    else
        ScrGameEprintf( "\nScript Error: %s: op_rm_timer_event: pobj is NULL!", scr->FileName );
}

/*
    int game_ticks( int seconds ) - the transformation of a second -> tick
*/
void ScrGame_GameTicks( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val;
    uint16_t type;

    GETARGI( scr, type, val, 0, "game_ticks" );
    SCP_DBGA( "game_ticks( [%x]%i )", type, val );
    if( val < 0 ) val = 0;
    RETINT( scr, 10 * val );
}

/*
    int has_trait (int trait_type, ObjectPtr who, int trait) - return the value of the specified property
*/
void ScrGame_HasTrait( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj;
    int AIpackNb, perk, v14;
    uint16_t type[3];

    AIpackNb = 0;
    GETARGI( scr, type[ 0 ], perk, 0, "has_trait" );
    GETARGP( scr, type[ 1 ], obj, 1, "has_trait" );
    GETARGI( scr, type[ 2 ], v14, 2, "has_trait" );
    SCP_DBGA( "has_trait( [%x]%i, [%x]%p, [%x]%i )", type[2], v14, type[1], obj, type[0], perk );
    if( !obj ){
        ScrGameErrorMsg( scr, "has_trait", 1 );
    } else {
	if( !v14 ){
    	    if( perk >= 119 )
        	ScrGameEprintf("\nScript Error: %s: op_has_trait: Perk out of range", scr->FileName);
    	    else
        	AIpackNb = PerkLvl(obj, perk);
	} else {
	    if( v14 <= 1 ){
    		switch( perk ){
        	    case 5: if( OBJTYPE( obj->Pid ) == TYPE_CRIT ) AIpackNb = obj->Critter.State.AIpackNb; break;
    		    case 6: if( OBJTYPE( obj->Pid ) == TYPE_CRIT ) AIpackNb = obj->Critter.State.GroupId; break;
    	    	    case 10: AIpackNb = obj->Orientation; break;
    	    	    case 666: AIpackNb = ((obj->Flags & 0x01) == 0); break;
        	    case 669: AIpackNb = ItemGetBackPackWeight( obj ); break;
    		}
	    } else {
		if( v14 == 2 ){
    		    if( perk >= 16 )
        		ScrGameEprintf( "\nScript Error: %s: op_has_trait: Trait out of range", scr->FileName );
    		    else
        		AIpackNb = TraitSpecActive( perk );
		} else {
		    ScrGameEprintf( "\nScript Error: %s: op_has_trait: Trait out of range", scr->FileName );
		    AIpackNb = 0;
		}
	    }
	}
    }
    RETINT( scr, AIpackNb );
}

/*
    boolean obj_can_hear_obj( ObjectPtr src_obj, ObjectPtr dst_obj )
    - Returns True if the source object (src_obj) is capable of hearing the destination object (dst_obj). 
    This includes distance factors, current activity (standing/walking/running), and skill use (stealth/etc.).
*/
void ScrGame_ObjCanHearObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj[ 2 ];
    uint16_t type[ 2 ];
    int CanHear;

    CanHear = 0;
    GETARGP( scr, type[ 0 ], obj[ 0 ], 0, "obj_can_hear_obj" );
    GETARGP( scr, type[ 1 ], obj[ 1 ], 1, "obj_can_hear_obj" );
    SCP_DBGA( "obj_can_hear_obj( [%x]%p, [%x]%p )", type[1], obj[1], type[0], obj[0] );
    if( !obj[0] || !obj[ 1 ] ) {
	if( ( obj[0]->GridId != -1 ) && AiObjCanHearObj( obj[1], obj[0] ) ) CanHear = 1;
    }
    RETINT( scr, CanHear );
}

/*
    int game_time_hour - returns the current time in the format HHMM
*/
void ScrGame_GameTimeHour( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "game_time_hour" );
    RETINT( scr, ScptGetGameTime() );
}

/*
    int fixed_param - returns the value of the fixed parameter passed to a script
*/
void ScrGame_FixedParam( Intp_t *scr )
{
    SCP_DBG_VAR;
    Scpt_t *script;
    int param = 0;

    SCP_DBGA( "fixed_param" );
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "fixed_param", 2 );
    else
        param = script->ArgVal;
    RETINT( scr, param );
}

/*
    boolean tile_is_visible( int tile ) 
    - Returns True if a given hex (tile) is currently visible, i.e. an object on it could conceivably be displayed on-screen. 
    This includes hexes that may technically have bases that are off-screen, but on whom objects could exist that would bound into the actual display area.
*/
void ScrGame_TileIsVisible( Intp_t *scr )
{
    SCP_DBG_VAR;
    int Visibility, val;
    uint16_t type;

    Visibility = 0;
    GETARGI( scr, type, val, 0, "tile_is_visible" );
    SCP_DBGA( "tile_is_visible( [%x]%i )", type, val );
    if( ScrGameIsTileVisible( val ) ) Visibility = 1;
    RETINT( scr, Visibility );        
}

/*
    void dialogue_system_enter - automatic invocation of the dialogue system
*/
void ScrGame_DialogueSystemEnter( Intp_t *scr )
{
    SCP_DBG_VAR;
    Scpt_t *script;
    Obj_t *obj;

    SCP_DBGA( "dialogue_system_enter" );
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ) return;    
    obj = ScptGetSelfObj( scr );
    if( (OBJTYPE( obj->Pid ) != TYPE_CRIT || CritterCanTalk( obj )) && !IN_COMBAT && GlobVarFloatMsgInc( 4 ) != -1 ) gDlgPartyMemberObj = ScptGetSelfObj( scr );    
}

/*
    int action_being_used - returns the skill ID used on the object, which is called in the script command
*/
void ScrGame_ActionBeingUsed( Intp_t *scr )
{
    SCP_DBG_VAR;
    int used;
    Scpt_t *script;

    SCP_DBGA( "action_being_used" );
    used = -1;
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "action_being_used", 2 );
    else
        used = script->i17;
    RETINT( scr, used );
}

/*
    int critter_state( ObjectPtr who )
    - Returns the state of a given critter object (from combat data), used to determine if a critter is dead, unconscious, etc.
*/
void ScrGame_CritterState( Intp_t *scr )
{
    SCP_DBG_VAR;
    int state;
    Obj_t *obj;
    int v10, v9;
    uint16_t type;

    state = 1;
    GETARGP( scr, type, obj, 0, "critter_state" );
    SCP_DBGA( "critter_state( [%x]%p )", type, obj );
    if( obj && OBJTYPE( obj->Pid ) == TYPE_CRIT ){
        if( CritterCanTalk(obj) ){
            v9 = (obj->ImgId & 0xFF0000) >> 16;
            v10 = 0;
            if( v9 >= 48 && v9 <= 49 ) v10 = 2;
            state = (obj->Critter.State.CombatResult & 0x7C) | v10;
        } else if( !CritterIsDead( obj ) ){
            state = 2;
        }
    } else {
        ScrGameErrorMsg( scr, "critter_state", 1 );
    }
    RETINT( scr, state );
}

/*
    void game_time_advance( int amount ) - increases the game time (the time elapsed from the start of the game) for a specified number of ticks
*/
void ScrGame_GameTimeAdvance( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val, time, i;

    GETARGI( scr, type, val, 0, "game_time_advance" );
    SCP_DBGA( "game_time_advance( [%x]%i )", type, val );
    time = val % 864000;
    for( i = 0; i < (val / 864000); i++ ){
        ScptTimeAdvance( 864000 );
        EvQeTrig();
    }
    ScptTimeAdvance( time );
    EvQeTrig();
}

/*
    void radiation_inc( ObjectPtr who, int amount )
     - Increments a critter’s radiation counter by a given amount. 
     NOTE: This should only be done to the player (obj_dude) in Fallout due to design restrictions!
*/
void ScrGame_RadiationInc( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val;
    Obj_t *dude;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], val, 0, "radiation_inc" );
    GETARGP( scr, type[ 1 ], dude, 1, "radiation_inc" );
    SCP_DBGA( "radiation_inc( [%x]%p, [%x]%i )", type[1], dude, type[0], val );
    if( dude )
        CritterRadInc( dude, val );
    else
        ScrGameErrorMsg( scr, "radiation_inc", 1 );
}

/*
    void radiation_dec( ObjectPtr who, int amount )
     - Decrements a critter’s radiation counter by a given amount. 
     NOTE: This should only be done to the player (obj_dude) in Fallout due to design restrictions!
*/
void ScrGame_RadiationDec( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val;
    Obj_t *dude;
    uint16_t type[ 2 ];
    
    GETARGI( scr, type[ 0 ], val, 0, "radiation_dec" );
    GETARGP( scr, type[ 1 ], dude, 1, "radiation_dec" );
    SCP_DBGA( "radiation_dec( [%x]%p, [%x]%i )", type[1], dude, type[0], val );
    if( dude ){
        CritterRadInc( dude, ( CritterRadiated( dude ) >= 0 ) ? -val : 0 );
    } else {
        ScrGameErrorMsg( scr, "radiation_dec", 1 );
    }
}

/*
    int critter_attempt_placement( ObjectPtr who, int hex, int elev ) - place the critter at a given position
*/
void ScrGame_CritterAttemptPlacement( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val, GridIdx;
    Obj_t *obj;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], val, 0, "critter_attempt_placement" );
    GETARGI( scr, type[ 1 ], GridIdx, 1, "critter_attempt_placement" );
    GETARGP( scr, type[ 2 ], obj, 2, "critter_attempt_placement" );
    SCP_DBGA( "critter_attempt_placement( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], GridIdx, type[0], val );
    if( !obj ){
        ScrGameErrorMsg( scr, "critter_attempt_placement", 1 );
        return;
    }
    if( val != obj->Elevation && ( OBJTYPE( obj->Pid ) == TYPE_CRIT ) ) CombatUnk79( obj );
    ObjMoveToTile( obj, 0, val, 0 );
    RETINT( scr, UseUnk46( obj, GridIdx, val, 1 ) );
}

/*
    int obj_pid( ObjectPtr obj ) - Returns the prototype id # (pid) of an object (obj).
*/
void ScrGame_ObjPid( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj;
    uint16_t type;
    int pid = -1;

    GETARGP( scr, type, obj, 0, "obj_pid" );
    SCP_DBGA( "obj_pid( [%x]%p )", type, obj );
    if( obj )
        pid = obj->Pid;
    else
        ScrGameErrorMsg( scr, "obj_pid", 1 );
    RETINT( scr, pid );
}

/*
    int cur_map_index - the index of the current map
*/
void ScrGame_CurMapIndex( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "cur_map_index");
    RETINT( scr, MapGetCurrentMapId() );
}

/*
    int critter_add_trait( ObjectPtr who, int trait_type, int trait, int amount ) - Critter adds the specified property
*/
void ScrGame_OpCritterAddTrait( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val, prk, v14, err = -1;
    Obj_t *obj;
    uint16_t type[ 4 ];

    GETARGI( scr, type[ 0 ], val, 0, "critter_add_trait" );
    GETARGI( scr, type[ 1 ], prk, 1, "critter_add_trait" );
    GETARGI( scr, type[ 2 ], v14, 2, "critter_add_trait" );
    GETARGP( scr, type[ 3 ], obj, 3, "critter_add_trait" );
    SCP_DBGA( "critter_add_trait( [%x]%p, [%x]%i, [%x]%i, [%x]%i )", type[3], obj, type[2], v14, type[1], prk, type[0], val  );
    if( obj ){
        if( OBJTYPE( obj->Pid ) == TYPE_CRIT ){
            if( v14 ){
                if ( v14 == 1 ){
                    if( prk == 5 ){
                        AiUnk54( obj, val );
                    } else if( prk == 6 && !PartyMembRdy( obj ) && val != obj->Critter.State.GroupId && !LsgPending() ){
                        AiUnk53( obj, val );
                    }
                } else {
                    ScrGameEprintf( "\nScript Error: %s: op_critter_add_trait: Trait out of range", scr->FileName );
                }
            } else {
                eprintf( "\nintextra::critter_add_trait: Adding Perk %s to %s", PerkGetName( prk ), CritterGetName( obj ) );
                if( val ){
                    if( PerkLvlUp( obj, prk ) ){
                        ScrGameEprintf( "\nScript Error: %s: op_critter_add_trait: perk_add_force failed", scr->FileName );
                        eprintf( "Perk: %d", prk );
                    }
                } else if( PerkLvlDec( obj, prk ) ){
                    ScrGameEprintf( "\nScript Error: %s: op_critter_add_trait: per_sub failed", scr->FileName );
                    eprintf( "Perk: %d", prk );
                }
                if( obj == gObjDude ) IfaceRenderHP( 1 );
            }
        }
    } else {
        ScrGameErrorMsg( scr, "critter_add_trait", 1 );
    }
    RETINT( scr, err );
}

/*
    void critter_rm_trait (ObjectPtr who, int trait_type, int trait, int amount) - remove trait / Perk
*/
void ScrGame_CritterRmTrait( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val0, val1, val2, err;
    Obj_t *obj;
    uint16_t type[4];

    err = -1;
    GETARGI( scr, type[ 0 ], val0, 0, "critter_rm_trait" );
    GETARGI( scr, type[ 1 ], val1, 1, "critter_rm_trait" );
    GETARGI( scr, type[ 2 ], val2, 2, "critter_rm_trait" );
    GETARGP( scr, type[ 3 ], obj, 3, "critter_rm_trait" );
    SCP_DBGA( "critter_rm_trait( [%x]%p, [%x]%i, [%x]%i, [%x]%i )", type[3], obj, type[2], val2, type[1], val1, type[0], val0  );
    if( obj ){
        ScrGameErrorMsg( scr, "critter_rm_trait", 1 );
        return;
    }
    if( OBJTYPE( obj->Pid ) == TYPE_CRIT ){
        if( val2 ){
            ScrGameEprintf( "\nScript Error: %s: op_critter_rm_trait: Trait out of range", scr->FileName );
        } else {
            while( PerkLvl( obj, val1 ) > 0 ){
                if( PerkLvlDec( obj, val1 ) ) ScrGameEprintf( "\nScript Error: op_critter_rm_trait: perk_sub failed" );
            }
        }
    }
    RETINT( scr, err );
}

/*
    int|string proto_data(int pid, int data_member) - Returns the value of a data-member of a given prototype (pid).
*/
void ScrGame_ProtoData( Intp_t *scr )
{
    SCP_DBG_VAR;
    char *p;
    int pid, data_member;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], data_member, 0, "proto_data" );
    GETARGI( scr, type[ 1 ], pid, 1, "proto_data" );
    SCP_DBGA( "proto_data( [%x]%i, [%x]%i )", type[1], pid, type[0], data_member );
    switch( ProtoDataMember( pid, data_member, (void **)&p ) ){
	case 1: RETPTR( scr, p );return;
	case 2: RETFTR( scr, IntpAddString( scr, p ) ); return;
    }
    RETINT( scr, 0 ); return;
}

/*
    string message_str( int msg_list, int msg_num ) - returns a string message to the number of the file msg
*/
void ScrGame_MessageStr( Intp_t *scr )
{
    SCP_DBG_VAR;
    unsigned int msg_num, msg_list;
    char *msg;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], msg_num, 0, "message_str" );
    GETARGI( scr, type[ 1 ], msg_list, 1, "message_str" );
    SCP_DBGA( "message_str( [%x]%i, [%x]%i )", type[1], msg_list, type[0], msg_num );
    if( msg_list ){
        msg = ScptGetDialog( msg_list, msg_num, 1 );
        if( !msg ){
            eprintf( "\nError: No message file EXISTS!: index %d, line %d", msg_list, msg_num );
            msg = "Error";
        }
    } else {
        msg = "Error";
    }
    RETFTR( scr, IntpAddString( scr, msg ) );
}

/*
    ObjectPtr (int) critter_inven_obj (ObjectPtr who, int where) - returns a pointer to the object in the slot inventory or the total number of objects in the inventory
*/
void ScrGame_CritterInvenObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val;
    Obj_t *obj;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], val, 0, "critter_inven_obj" );
    GETARGP( scr, type[ 1 ], obj, 1, "critter_inven_obj" );
    SCP_DBGA( "critter_inven_obj( [%x]%p, [%x]%i )", type[1], obj, type[0], val );
    if( !obj ){
        ScrGameErrorMsg( scr, "critter_inven_obj", 1 );
    } else {
	if( OBJTYPE( obj->Pid ) == TYPE_CRIT ){
    	    switch( val ){
        	case -2: RETINT( scr, obj->Critter.Box.Cnt ); return;
        	case 0:  RETPTR( scr, InvGetArmorObj( obj ) ); return;
        	case 1:  if( obj != gObjDude || IfaceGetSelectedHand() ){ RETPTR( scr, InvGetRHandObj( obj ) ); } return;
        	case 2:  if( obj != gObjDude || !IfaceGetSelectedHand() ){ RETPTR( scr, InvGetLHandObj( obj ) ); } return;
        	default: ScrGameEprintf( "\nscript error: %s: Error in critter_inven_obj -- wrong type!", scr->FileName ); break;        	
    	    }
	} else {
	    ScrGameErrorMsg( scr, "critter_inven_obj", 3 );
	    eprintf( "  Not a critter!" );
	}
    }
    RETINT( scr, 0 );
}

/*
    void obj_set_light_level (ObjectPtr obj, int intensity, int distance) - makes the object light source
*/
void ScrGame_ObjSetLightLevel( Intp_t *scr )
{
    SCP_DBG_VAR;
    VidRect_t Area;
    Obj_t *obj;
    int rad, inten;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], rad, 0, "obj_set_light_level" );
    GETARGI( scr, type[ 1 ], inten, 1, "obj_set_light_level" );
    GETARGP( scr, type[ 2 ], obj, 2, "obj_set_light_level" );
    SCP_DBGA( "obj_set_light_level( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], inten, type[0], rad );
    if( !obj ){
        ScrGameErrorMsg(scr, "obj_set_light_level", 1);
        return;
    }
    if( inten ){
        if( ObjSetLight( obj, rad, (inten << 16) / 100, &Area ) == -1 ) return;
    } else if( ObjSetLight( obj, rad, 0, &Area ) == -1 ){
        return;
    }
    TileUpdateArea( &Area, obj->Elevation );
}

/*
    void world_map() - Sends a request for the game engine to bring up the World Map screen, for the player to move around to different locations.
*/
void ScrGame_WorldMap( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "world_map() ");
    ScptWorldMap();
}

/*
    ObjectPtr inven_cmds (ObjectPtr who, int inven_cmd, int slotNum) - returns a pointer to the object in the inventory
*/
void ScrGame_InvenCmds( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val0, val1;
    Obj_t *obj, *item = NULL;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], val0, 0, "inven_cmds" );
    GETARGI( scr, type[ 1 ], val1, 1, "inven_cmds" );
    GETARGP( scr, type[ 2 ], obj, 2, "inven_cmds" );
    SCP_DBGA( "inven_cmds( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], val1, type[0], val0 );
    if( obj ){
        if( val1 == 13 ) item = InvUnk27( obj, val0 );
    } else {
        ScrGameErrorMsg( scr, "anim", 1 );
    }
    RETPTR( scr, item );
}

/*
    void float_msg( ObjectPtr who, str msg, int type )
     - Attempts to create a floating-text message (str) attached to an object (who) using colors dictated by type. 
     There are two special types, WARNING and SEQUENTIAL. 
     WARNING is used to print a message centered on the screen (such as for end-of-quest notifications), 
     and SEQUENTIAL will cycle through the colors, in an attempt to give critters different-colored messages to differentiate them.
*/
void ScrGame_FloatMsg( Intp_t *scr )
{
    SCP_DBG_VAR;
    VidRect_t Area;
    Obj_t *obj;
    int colorFg,Idx[2],Font,colorBg, i;
    static int stat_zm1 = 1;
    uint16_t type[3];
    char *Text;

    colorFg = gPalColorCubeRGB[31][31][11];
    Text = 0;
    Font = 101;
    colorBg = gPalColorCubeRGB[0][0][0];
    for( i = 0; i < 2; i++ ){
        type[ i ] = IntpPopwA( scr );
        Idx[ i ] = IntpPopiA( scr );
        if( type[ i ] == SCR_FSTRING ) IntpStringDeRef( scr, type[ i ], Idx[ i ] );
        if( i == 1 ){
            if( (type[ i ] & 0xF7FF) == SCR_STRING ) Text = IntpGetString( scr, type[ i ] >> 8, Idx[ i ]);
        } else {
            if( (type[ i ] & 0xF7FF) != SCR_INT ) IntpError("script error: %s: invalid arg %d to float_msg", scr->FileName, i );
        }
    }
    GETARGP( scr, type[ 2 ], obj, 2, "float_msg" );
    SCP_DBGA( "float_msg( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], Idx[1], type[0], Idx[0] );
    if( !obj ){
        ScrGameErrorMsg( scr, "float_msg", 1 );
        return;
    }
    if( !Text || *Text == '\0' ){
        TextUnk10( obj );
        TileUpdate();
    }
    if( gMapCurrentLvl != obj->Elevation ) return;
    if( Idx[0] == -1 ){
        Idx[0] = stat_zm1 + 1;
        if( Idx[0] > 12 ) Idx[0] = 1;
        stat_zm1 = Idx[0];
    }
    switch( Idx[0] ){
        case -2:
            colorFg = gPalColorCubeRGB[31][0][0];
            colorBg = gPalColorCubeRGB[0][0][0];
            Font = 103;
            obj = 0;
            TileSetCenter( gObjDude->GridId, 1 );
            break;
        case 0:
        case 8:  colorFg = gPalColorCubeRGB[31][31][11]; break;
        case 1:
        case 5:
        case 10: colorFg = gPalColorCubeRGB[10][10][10]; break;
        case 2:  colorFg = gPalColorCubeRGB[31][0][0];   break;
        case 3:  colorFg = gPalColorCubeRGB[0][31][0];   break;
        case 4:  colorFg = gPalColorCubeRGB[0][0][31];   break;
        case 6:  colorFg = gPalColorCubeRGB[20][20][20]; break;
        case 7:  colorFg = gPalColorCubeRGB[31][10][10]; break;
        case 9:  colorFg = gPalColorCubeRGB[31][31][31]; break;
        case 11: colorFg = gPalColorCubeRGB[8][8][8];    break;
        case 12: colorFg = gPalColorCubeRGB[15][15][15]; break;
        default: break;
    }
    if( TextFloatMessage( obj, Text, Font, colorFg, colorBg, &Area) != -1 ) TileUpdateArea( &Area, obj->Elevation );
}

/*
    int metarule( int meta_switch, int meta_par ) - function works with a variety of gaming options, has a huge number of variations
    
    13 -> METARULE_SIGNAL_END_GAME
    14 -> METARULE_TEST_FIRSTRUN
    15 -> METARULE_ELEVATOR
    16 -> METARULE_PARTYCOUNT
    17 -> METARULE_AREA_KNOWN
    18 -> METARULE_WHO_ON_DRUGS
    19 -> METARULE_MAP_KNOWN        
    22 -> METARULE_IS_LOADGAME
    30 -> METARULE_CAR_CURRENT_TOWN
    31 -> METARULE_GIVE_CAR_TO_PARTY
    32 -> METARULE_GIVE_CAR_GAS
    40 -> METARULE_SKILL_CHECK_TAG
    42 -> METARULE_DROP_ALL_INVEN
    43 -> METARULE_INVEN_UWIELD_WHO
    44 -> METARULE_GET_WORLDMAP_XPOS
    45 -> METARULE_GET_WORLDMAP_YPOS
    46 -> METARULE_CURRENT_TOWN
    47 -> METARULE_LANGUAGE_FILTER
    48 -> METARULE_VIOLENCE_FILTER
    49 -> METARULE_W_DAMAGE_TYPE // weapon
    50 -> METARULE_CRITTER_BARTERS
    51 -> METARULE_CRITTER_KILL_TYPE
    52 -> METARULE_SET_CAR_CARRY_AMOUNT
    53 -> METARULE_GET_CAR_CARRY_AMOUNT
*/
void ScrGame_MetaRule( Intp_t *scr ) 
{
    SCP_DBG_VAR;
    Obj_t *obj;
    uint16_t type[ 2 ];
    Proto_t *proto, *pObj;
    int v14, v15, v16, meta_par, sel, EntryValue, WeaponBase;
    
    WeaponBase = 0;
    type[ 0 ] = IntpPopwA( scr );    
    if( type[ 0 ] == SCR_PTR ){
        obj = IntpPopPtrA( scr );
	GETARGI( scr, type[ 1 ], sel, 1, "metarule" );
	SCP_DBGA( "metarule( [%x]%i, [%x]%p )", type[1], sel, type[0], obj );
    } else {
        meta_par = IntpPopiA( scr );
	if( type[ 0 ] == SCR_FSTRING ) IntpStringDeRef( scr, type[ 0 ], meta_par );
	if( (type[ 0 ] & ~0x800) != SCR_INT ) IntpError( "script error: %s: invalid arg %d to metarule", scr->FileName, 0 );
	GETARGI( scr, type[ 1 ], sel, 1, "metarule" );
	SCP_DBGA( "metarule( [%x]%i, [%x]%i )", type[1], sel, type[0], meta_par );
    }
    switch( sel ){
	case 13: gMenuEscape = 2; RETINT( scr, 0 ); break;	    
        case 14: RETINT( scr, (gMap.MapFlags & 0x01) == 0 ); break;
        case 15: ScptRequestElevator( ScptGetSelfObj( scr ), meta_par ); RETINT( scr, 0 ); break;
        case 16: RETINT( scr, PartyUnk12() ); break;
        case 17: RETINT( scr, WmLocKarma( meta_par ) ); break;
        case 18: RETINT( scr, EvQeEnqueued( obj, 0 ) ); break;
        case 19: RETINT( scr, WmUnk03( meta_par ) ); break;
        case 22: RETINT( scr, LsgPending() ); break;
        case 30: RETINT( scr, WmUnk90() ); break;
        case 31: RETINT( scr, WmUnk91() ); break;
        case 32: RETINT( scr, WmCarRefuel( meta_par ) ); break;
        case 40: RETINT( scr, SkillIsSpecial( meta_par ) ); break;
        case 42:
            v14 = Item18( obj, obj->GridId );
            if( gObjDude == obj ){
                IfaceHandSlotUpdate( 0, -1, -1 );
                IfaceRenderAC( 0 );
            }
	    RETINT( scr, v14 );
	    break;
        case 43:
            v15 = 1;
            if( obj == gObjDude && !IfaceGetSelectedHand() ) v15 = 0;
            v16 = InvUnk30( obj, v15, 0 );
            if( obj == gObjDude ){
                IfaceHandSlotUpdate( GameIfaceStat() == 0, -1, -1 );
            } else {
                if( ItemGetObjType( InvGetLHandObj( obj ) ) == 3 ) obj->Flags &= ~0x1000000;
            }
	    RETINT( scr, v16 );
	    break;
        case 44: 
    	    WmGetWorldPos( &EntryValue, NULL ); RETINT( scr, EntryValue ); 
    	    break;
        case 45: 
    	    WmGetWorldPos( NULL, &EntryValue ); RETINT( scr, EntryValue ); 
    	    break;
        case 46:
            if( WmGetCurrentLocation( &EntryValue ) == -1 ) eprintf( "\nIntextra: Error: metarule: current_town" );
	    RETINT( scr, EntryValue );
	    break;
        case 47:
            CfgGetInteger( &gConfiguration, "preferences", "language_filter", &EntryValue );
	    RETINT( scr, EntryValue );
	    break;
        case 48:
            CfgGetInteger( &gConfiguration, "preferences", "violence_level", &EntryValue );
	    RETINT( scr, EntryValue );
	    break;
        case 49:
            if( OBJTYPE( obj->Pid ) == TYPE_ITEM ){
        	if( ItemGetObjType( obj ) == PR_ITEM_WEAPON ){ RETINT( scr, ItemGetWeaponBase( NULL, obj ) ); break; }
            } else { 
                if( ArtMakeId( 5, 10, 0, 0, 0 ) == obj->ImgId ){ RETINT( scr, 6 ); break; }
            }
            ScrGameErrorMsg( scr, "metarule:w_damage_type", PR_ITEM_WEAPON );
            eprintf( "Not a weapon!" );
	    RETINT( scr, WeaponBase );
	    break;
        case 50:
            if( OBJTYPE( obj->Pid ) == TYPE_CRIT ){
                ProtoGetObj( obj->Pid, &proto );
                if( proto->Critt.Type & CRITT_RADIATED ) WeaponBase = 1;
            }
	    RETINT( scr, WeaponBase );
	    break;
        case 51:
	    RETINT( scr, CritterGetGender( obj ) );
	    break;
        case 52:
            if( ProtoGetObj( PID_CARTRUCK, &pObj ) != -1 ){
                WeaponBase = 1;
                pObj->Critt.BaseStat[0] = meta_par;
            }                
	    RETINT( scr, WeaponBase );
	    break;
        case 53:
            if( ProtoGetObj( PID_CARTRUCK, &proto ) != -1 ) WeaponBase = proto->Critt.BaseStat[ 0 ];
	    RETINT( scr, WeaponBase );
	    break;
    }    
//SCP_DBG_EN;
}

/*
    void anim( ObjectPtr who, int anim, int direction )
    Sets up a single-frame animation (anim) for the object (who) that runs in the given direction.
*/
void ScrGame_OpAnim( Intp_t *scr )
{
    SCP_DBG_VAR;
    ObjCritterCond_t *p_State;
    VidRect_t Area2, Area1;
    Obj_t *obj;
    int v9, Id, v12, Orientation, v21;
    uint16_t type[ 3 ];

    p_State = NULL;
    v12 = 0;
    GETARGI( scr, type[ 0 ], Orientation, 0, "anim" );
    GETARGI( scr, type[ 1 ], v21, 1, "anim" );
    GETARGP( scr, type[ 2 ], obj, 2, "anim" );
    SCP_DBGA( "anim( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], v21, type[0], Orientation );
    if( !obj ){
        ScrGameErrorMsg( scr, "anim", 1 );
        return;
    }
    if( v21 < 0x41 ){
        if( OBJTYPE( obj->Pid ) == TYPE_CRIT ) p_State = &obj->Critter.State;
        v9 = ScrGamUnk06( obj, v21, 1 );
        AnimRegStart( 1 );
        if( !Orientation ){
            AnimRegAnimation( obj, v9, 0 );
            if ( v9 >= 20 && v9 <= 35 ){
                AnimUnk62( obj, ArtMakeId( 1, obj->ImgId & 0xFFF, v9 + 28, (obj->ImgId & 0xF000) >> 12, (obj->ImgId & 0x70000000) >> 28 ), -1 );
            }
            if( p_State ) p_State->CombatResult &= ~0x02;            
        } else {
    	    Id = ArtMakeId( (obj->ImgId & 0xF000000) >> 24, obj->ImgId & 0xFFF, v9, (obj->ImgId & 0xF000) >> 12, (obj->ImgId & 0x70000000) >> 28 );
    	    AnimRegAnimReverse( obj, v9, 0 );
    	    if( v9 == 36 ){
        	v12 = ArtMakeId( (Id & 0xF000000) >> 24, Id & 0xFFF, 49, (Id & 0xF000) >> 12, (Id & 0x70000000) >> 28 );
    	    } else {
        	if( v9 == 37 ) v12 = ArtMakeId( (Id & 0xF000000) >> 24, Id & 0xFFF, 48, (Id & 0xF000) >> 12, (Id & 0x70000000) >> 28 );
    	    }
    	    if( p_State ) p_State->CombatResult |= 0x02;
    	    AnimUnk62( obj, v12, -1 );
        }
        AnimRegEnd();
        return;
    }
    if( v21 == 1000 ){
        if( Orientation < 6 ){
            ObjSetRotation( obj, Orientation, &Area2 );
            TileUpdateArea( &Area2, gMapCurrentLvl );
        }
    } else if( v21 == 1010 ){
        ObjSetFrame( obj, Orientation, &Area1 );
        TileUpdateArea( &Area1, gMapCurrentLvl );
    } else {
        ScrGameEprintf( "\nScript Error: %s: op_anim: anim out of range", scr->FileName );
    }
}

/*
    ObjectPtr obj_carrying_pid_obj( ObjectPtr who, int pid )
     - Returns an Object pointer to an instance of an object of type pid if an object (who) is carrying an object of that type.
*/
void ScrGame_ObjCarryingPidObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val;
    Obj_t *obj, *Item;
    uint16_t type[ 2 ];

    Item = NULL;
    GETARGI( scr, type[ 0 ], val, 0, "obj_carrying_pid_obj" );
    GETARGP( scr, type[ 1 ], obj, 1, "obj_carrying_pid_obj" );
    SCP_DBGA( "obj_carrying_pid_obj( [%x]%p, [%x]%i )", type[1], obj, type[0], val );
    if( obj )
        Item = InvGetItem( obj, val );
    else
        ScrGameErrorMsg( scr, "obj_carrying_pid_obj", 1 );
    RETPTR( scr, Item );
}

/*
    void reg_anim_func (int par1, int par2) - work with a list of animations
*/
void ScrGame_RegAnimFunc( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj;
    int par[ 2 ];
    uint16_t type[ 2 ];
    
    type[0] = IntpPopwA( scr );
    obj = NULL;
    if( type[0] != SCR_PTR ){
	par[0] = IntpPopiA( scr );
	if( type[0] == SCR_FSTRING ) IntpStringDeRef( scr, type[0], par[0] );
	if( (type[0] & 0xF7FF) != SCR_INT ) IntpError( "script error: %s: invalid arg %d to reg_anim_func", scr->FileName, 0 );	
    } else {
	obj = IntpPopPtrA( scr );
        if( type[0] == SCR_FSTRING ) IntpStringDeRef( scr, type[0], PTR2INT( obj ) );\
        if( (type[0] & 0xF7FF) != SCR_PTR ) IntpError( "script error: %s: invalid arg %d to reg_anim_func", scr->FileName, 0 );
    }

    GETARGI( scr, type[ 1 ], par[1], 1, "reg_anim_func" );
    SCP_DBGA( "reg_anim_func( [%x]%i, [%x]%i )", type[1], par[1], type[0], par[0] );
    if( IN_COMBAT ) return;
    switch( par[1] ){
	case 1: AnimRegStart( par[0] ); break;
	case 2: AnimRegClear( obj ); break;
	case 3: AnimRegEnd( ); break;
    }
}

/*
    void reg_anim_animate( ObjectPtr what, int anim, int delay )
     Adds a single, in-place animation on an object (what) to an animation sequence-list, 
     at a given delay from the previous animation (delay should always be -1)!
*/
void ScrGame_RegAnimAnimate( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val0, val1, vlvl;
    Obj_t *obj;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], val0, 0, "reg_anim_animate" );
    GETARGI( scr, type[ 1 ], val1, 1, "reg_anim_animate" );
    GETARGP( scr, type[ 2 ], obj, 2, "reg_anim_animate" );
    SCP_DBGA( "reg_anim_animate( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], val1, type[0], val0 );
    if( IN_COMBAT ) return;
    if( val1 == 20 && obj && obj->Pid == 0x100002F ){
	vlvl = 0;
	CfgGetInteger( &gConfiguration, "preferences", "violence_level", &vlvl );
	if( vlvl < 2 ) return;
    }
    if( obj )
        AnimRegAnimation( obj, val1, val0 );
    else
        ScrGameErrorMsg( scr, "reg_anim_animate", 1 );
}

/*
    void reg_anim_animate_reverse( ObjectPtr what, int anim, int delay ) - adds a single turn animation, which will be played backwards
*/
void ScrGame_RegAnimAnimateReverse( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val[ 2 ];
    Obj_t *obj;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "reg_anim_animate_reverse" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "reg_anim_animate_reverse" );
    GETARGP( scr, type[ 2 ], obj, 2, "reg_anim_animate_reverse" );
    SCP_DBGA( "reg_anim_animate_reverse( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], val[1], type[0], val[0] );
    if( IN_COMBAT ) return;
    if( obj )
        AnimRegAnimReverse( obj, val[ 1 ], val[ 0 ] );
    else
        ScrGameErrorMsg( scr, "reg_anim_animate_reverse", 1 );
}

/*
    void reg_anim_obj_move_to_obj( ObjectPtr who, ObjectPtr dest_obj, int delay )
    Adds an animation to cause a critter object (who) to attempt to walk to another object (dest_obj) 
     at a given delay from the previous animation (delay should always be -1)!
*/
void ScrGame_RegAnimObjMoveToObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val;
    Obj_t *DstObj, *Object;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], val, 0, "reg_anim_obj_move_to_obj" );
    GETARGP( scr, type[ 1 ], DstObj, 1, "reg_anim_obj_move_to_obj" );
    GETARGP( scr, type[ 2 ], Object, 2, "reg_anim_obj_move_to_obj" );
    SCP_DBGA( "reg_anim_obj_move_to_obj( [%x]%p, [%x]%p, [%x]%i )", type[2], Object, type[1], DstObj, type[0], val );
    if( IN_COMBAT ) return;
    if( Object )
        AnimObjMoveToObj( Object, DstObj, -1, val );
    else
        ScrGameErrorMsg( scr, "reg_anim_obj_move_to_obj", 1 );
}

/*
    void reg_anim_obj_run_to_obj( ObjectPtr who, ObjectPtr dest_obj, int delay ) - adds to the queue animation podbeganiya one object to another
*/
void ScrGame_RegAnimObjRunToObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val;
    Obj_t *obj2, *obj1;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], val, 0, "reg_anim_obj_run_to_obj" );
    GETARGP( scr, type[ 1 ], obj2, 1, "reg_anim_obj_run_to_obj" );
    GETARGP( scr, type[ 2 ], obj1, 2, "reg_anim_obj_run_to_obj" );
    SCP_DBGA( "reg_anim_obj_run_to_obj( [%x]%p, [%x]%p, [%x]%i )", type[2], obj1, type[1], obj2, type[0], val );
    if( IN_COMBAT ) return;
    if( obj1 )
        AnimObjRunToObj( obj1, obj2, -1, val );
    else
        ScrGameErrorMsg( scr, "reg_anim_obj_run_to_obj", 1 );
}

/*
    void reg_anim_obj_move_to_tile( ObjectPtr who, int dest_tile, int delay ) 
    Adds an animation to cause a critter object (who) to attempt to walk to a given destination tile number (dest_tile) 
     at a given delay from the previous animation (delay should always be -1)!
*/
void ScrGame_RegAnimObjMoveToTile( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val, tile;
    Obj_t *obj;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], val, 0, "reg_anim_obj_move_to_tile" );
    GETARGI( scr, type[ 1 ], tile, 1, "reg_anim_obj_move_to_tile" );
    GETARGP( scr, type[ 2 ], obj, 2, "reg_anim_obj_move_to_tile" );
    SCP_DBGA( "reg_anim_obj_move_to_tile( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], tile, type[0], val );
    if( IN_COMBAT ) return;
    if( obj )
        AnimObjMoveToTile( obj, tile, obj->Elevation, -1, val );
    else
        ScrGameErrorMsg( scr, "reg_anim_obj_move_to_tile", 1 );
}

/*
    void reg_anim_obj_run_to_tile( ObjectPtr who, int dest_tile, int delay ) - adds to the queue object animation, running to the target hex
*/
void ScrGame_RegAnimObjRunToTile( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val, tile;
    Obj_t *obj;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], val, 0, "reg_anim_obj_run_to_tile" );
    GETARGI( scr, type[ 1 ], tile, 1, "reg_anim_obj_run_to_tile" );
    GETARGP( scr, type[ 2 ], obj, 2, "reg_anim_obj_run_to_tile" );
    SCP_DBGA( "reg_anim_obj_run_to_tile( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], tile, type[0], val );
    if( IN_COMBAT ) return;
    if( obj )
        AnimObjRunToTile( obj, tile, obj->Elevation, -1, val );
    else
        ScrGameErrorMsg( scr, "reg_anim_obj_run_to_tile", 1 );
}

/*
    void play_gmovie( int movieNum ) - play video
*/
void ScrGame_PlayGmovie( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type, tmp[ 34 ];
    int val;

    memcpy( tmp, gScrGameMovieFlags, sizeof( gScrGameMovieFlags ) );
    scr->Flags |= SCR_FPROC_RUN;
    GETARGI( scr, type, val, 0, "play_gmovie" );
    SCP_DBGA( "play_gmovie( [%x]%i )", type, val );
    GdialogUnk04();
    if( GMoviePlay( val, tmp[ val ] ) == -1 ) eprintf( "\nError playing movie %d!", val );
    GdialogTaskCb();
    scr->Flags &= ~SCR_FPROC_RUN;
}

/*
    void add_mult_objs_to_inven( ObjectPtr who, ObjectPtr item, int count ) - add a few units of a given item in the inventory    
*/
void ScrGame_AddMultObjsToInven( Intp_t *scr )
{
    SCP_DBG_VAR;
    VidRect_t Area;
    int Quantity;
    Obj_t *item, *dude;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], Quantity, 0, "add_mult_objs_to_inven" );
    GETARGP( scr, type[ 1 ], item, 1, "add_mult_objs_to_inven" );
    GETARGP( scr, type[ 2 ], dude, 2, "add_mult_objs_to_inven" );
    SCP_DBGA( "add_mult_objs_to_inven( [%x]%p, [%x]%p, [%x]%i )", type[2], dude, type[1], item, type[0], Quantity );
    if( !dude || !item ) return;
    if( Quantity >= 0 ){
        if( Quantity > 99999 ) Quantity = 500;
    } else {
        Quantity = 1;
    }
    if( !ItemAdd( dude, item, Quantity ) ){
        ObjLightItem( item, &Area );
        TileUpdateArea( &Area, item->Elevation );
    }
}

/*
    int rm_mult_objs_from_inven(ObjectPtr who, ObjectPtr obj, int count)
    Removes (count) instances of an object (obj) from another object’s (who’s) inventory. 
    Note: this leaves the removed object in at location (0,1) on the map! 
	You must call move_to(…) to place it back on the map. 
    NOTE: This function returns the actual count that was removed (if you attempted to remove more instances than existed). 
	You *MUST* store this value in a variable (though you don’t have to actually do anything with it).
*/
void ScrGame_RmMultObjsFromInven( Intp_t *scr )
{
    SCP_DBG_VAR;
    VidRect_t Area;
    int Quantity, v19, v11;
    Obj_t *item, *dude;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], Quantity, 0, "rm_mult_objs_from_inven" );
    GETARGP( scr, type[ 1 ], item, 1, "rm_mult_objs_from_inven" );
    GETARGP( scr, type[ 2 ], dude, 2, "rm_mult_objs_from_inven" );
    SCP_DBGA( "rm_mult_objs_from_inven( [%x]%p, [%x]%p, [%x]%i )", type[2], dude, type[1], item, type[0], Quantity );
    if( !dude || !item ) return;    
    v19 = ( item->Flags & 0x7000000 ) ? 1 : 0;
    v11 = Item33( dude, item );
    if( v11 > Quantity ) v11 = Quantity;
    if( v11 ){
        if( !ItemUseItem( dude, item, v11 ) ){
            ObjUnk14( item, 1u, 0, &Area );
            TileUpdateArea( &Area, item->Elevation );
            if( v19 ){
                if( dude == gObjDude ) IfaceHandSlotUpdate( GameIfaceStat() == 0, -1, -1 );
            }
        }
    }
    RETINT( scr, v11 );
}

/*
    int get_month - returns the current month
*/
void ScrGame_GetMonth( Intp_t *scr )
{
    SCP_DBG_VAR;
    int month;

    SCP_DBGA( "get_month");
    ScptGetGameDate( &month, NULL, NULL );
    RETINT( scr, month );
}

/*
    int get_day - returns the current number
*/
void ScrGame_GetDay( Intp_t *scr )
{
    SCP_DBG_VAR;
    int day;

    SCP_DBGA( "get_day");
    ScptGetGameDate( NULL, &day, NULL );
    RETINT( scr, day );
}

/*
    void explosion( int where, int elevation, int damage ) - produce an explosion
*/
void ScrGame_Explosion( Intp_t *scr )
{
    SCP_DBG_VAR;
    int dmg, lvl, tilenum, buum;
    uint16_t type[ 3 ];

    GETARGI( scr, type[ 0 ], dmg, 0, "explosion" );
    GETARGI( scr, type[ 1 ], lvl, 1, "explosion" );
    GETARGI( scr, type[ 2 ], tilenum, 2, "explosion" );
    SCP_DBGA( "explosion( [%x]%i, [%x]%i, [%x]%i )", type[2], tilenum, type[1], lvl, type[0], dmg );
    buum = 1;
    if( tilenum == -1 ){
        eprintf( "\nError: explosion: bad tile_num!" );
    } else {
        if( !dmg ) buum = 0;
        ScptExplosion( tilenum, lvl, buum, dmg );
    }
}

/*
    int days_since_visited - the number of days that have passed since last player card
*/
void ScrGame_DaysSinceVisited( Intp_t *scr )
{
    SCP_DBG_VAR;
    int time;

    SCP_DBGA( "days_since_visited");
    if( gMap.Time )
        time = (ScptGetGameDekaSeconds() - gMap.Time) / 864000;
    else
        time = -1;
    RETINT( scr, time );
}

/*
    void gsay_start - ctavitsya before calling start a dialogue
*/
void ScrGame_StartDialog( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "gsay_start");
    scr->Flags |= SCR_FPROC_RUN;
    if( GdialogStart() ){
        scr->Flags &= ~SCR_FPROC_RUN;
        IntpError( "Error starting dialog." );
    }
    scr->Flags &= ~SCR_FPROC_RUN;
}


/*
    void gsay_end - ctavitsya after the call to start a dialogue
*/
void ScrGame_GsayEnd( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "gsay_end");
    scr->Flags |= SCR_FPROC_RUN;
    GdialogEnd();
    scr->Flags &= ~SCR_FPROC_RUN;
}

/*
    void gSay_Reply( int msg_list, int msg_num ) - Sets up a reply block (what the *CRITTER* says). gSay_Start void Dialog Starts a new dialog sequence.
*/
void ScrGame_GsayReply( Intp_t *scr )
{
    SCP_DBG_VAR;
    int i, val[2];
    uint16_t type[2];
    char *s;

    scr->Flags |= SCR_FPROC_RUN;
    s = NULL;
    for( i = 0; i < 2; i++ ){
        type[ i ] = IntpPopwA( scr );
        val[ i ] = IntpPopiA( scr );
        if( type[ i ] == SCR_FSTRING ) IntpStringDeRef( scr, type[ i ], val[ i ] );

	if( (type[ i ] & 0xF7FF) != SCR_INT ){
            if( i ){
                IntpError("script error: %s: invalid arg %d to gsay_reply", scr->FileName, i );
            } else if( type[ i ] == SCR_STRING || (type[ i ] == SCR_FSTRING)){
                s = IntpGetString( scr, type[0] >> 8, val[0] );
            } else {
                IntpError( "script error: %s: invalid arg %d to gsay_reply (type:%x)", scr->FileName, i, type[ i ] );
            }
        }
    }
    SCP_DBGA( "gsay_reply( [%x]%i, [%x]%i )", type[1], val[1], type[0], val[0] );
    if( s )
        GdialogSayReply( scr, s );
    else
        GdialogSayReplyConst( scr, val[ 1 ], val[ 0 ] );
    scr->Flags &= ~SCR_FPROC_RUN;
}

/*
    void gsay_option( int msg_list, int msg_num, procedure target, int reaction ) - display options in the dialog box without checking intelligence
*/
void ScrGame_GsayOption( Intp_t *scr )
{
    SCP_DBG_VAR;
    int i;
    int Idx[ 4 ];
    uint16_t type[ 4 ];
    char *s;

    scr->Flags |= SCR_FPROC_RUN;
    s = NULL;

    type[ 0 ] = IntpPopwA( scr );
    Idx[ 0 ] = IntpPopiA( scr );
    if( type[ 0 ] == SCR_FSTRING ) IntpStringDeRef( scr, SCR_FSTRING, Idx[ 0 ] );

    type[ 1 ] = IntpPopwA( scr );
    Idx[ 1 ] = IntpPopiA( scr );
    if( type[ 1 ] == SCR_FSTRING ) IntpStringDeRef( scr, SCR_FSTRING, Idx[ 1 ] );

    for( i = 2; i < 4; i++ ){
        type[ i ] = IntpPopwA( scr );
        Idx[ i ] = IntpPopiA( scr );
        if( type[ i ] == SCR_FSTRING ) IntpStringDeRef( scr, SCR_FSTRING, Idx[ i ] );
        if( (type[ i ] & 0xF7FF) != SCR_INT ){
            if( i == 2 ){
                if( (type[ i ] & 0xF7FF) == SCR_STRING )
                    s = IntpGetString( scr, type[ i ] >> 8, Idx[ i + 2 ] );
                else
                    IntpError( "script error: %s: invalid arg %d to gsay_option", scr->FileName, 2 );
            } else {
                IntpError( "script error: %s: invalid arg %d to gsay_option", scr->FileName, i );
            }
        }
    }
    SCP_DBGA( "gsay_option( [%x]%i, [%x]%i, [%x]%i, [%x]%i )", type[3], Idx[3], type[2], Idx[2], type[1], Idx[1], type[0], Idx[0] );
    if( (type[ 1 ] & 0xF7FF) == SCR_STRING ){
        IntpGetString( scr, type[ 1 ] >> 8, Idx[ 1 ] );
        if( s )
            GdialogSayOptionStrNoProc( Idx[ 3 ], s, Idx[ 0 ] );
        else
            GdialogSayOptionConstNoProc( Idx[ 3 ], Idx[ 2 ], Idx[ 0 ] );
        scr->Flags &= ~SCR_FPROC_RUN;
        return;
    }
    if( (type[ 1 ] & 0xF7FF) != SCR_INT ){
        IntpError( "Invalid arg 3 to sayOption" );
        scr->Flags &= ~SCR_FPROC_RUN;
        return;
    }
    if( s )
        GdialogSayOptionStr( Idx[ 3 ], s, Idx[ 1 ], Idx[ 0 ] );
    else
        GdialogSayOptionConst( Idx[ 3 ], Idx[ 2 ], Idx[ 1 ], Idx[ 0 ] );
    scr->Flags &= ~SCR_FPROC_RUN;
}

/*
    void gsay_message(int msg_list, int msg_num, int reaction) - displays a replica of the NPC, do not imply any response from the player
*/
void ScrGame_GsayMessage( Intp_t *scr )
{
    SCP_DBG_VAR;
    int i, val[3];
    uint16_t type[3];
    char *Arg;

    Arg = NULL;
    scr->Flags |= SCR_FPROC_RUN;
    for( i = 0; i < 3; i++ ){
        type[ i ] = IntpPopwA( scr );
        val[ i ] = IntpPopiA( scr );
        if( type[ i ] == SCR_FSTRING ) IntpStringDeRef( scr, type[ i ], val[ i ] );
        if( ( type[ i ] & 0xF7FF ) != SCR_INT ){
            if( i == 1 ){
                if( (type[ i ] & 0xF7FF) == SCR_STRING )
                    Arg = IntpGetString( scr, type[ i ] >> 8, val[ i ] );
                else
                    IntpError( "script error: %s: invalid arg %d to gsay_message", scr->FileName, 1 );
            } else {
                IntpError( "script error: %s: invalid arg %d to gsay_message", scr->FileName, i );
            }
        }
    }    
    SCP_DBGA( "gsay_message( %s, [%x]%i, [%x]%i )", Arg, type[2], val[2], type[1], val[1] );
    if( Arg )
        GdialogSayReply( scr, Arg );
    else
        GdialogSayReplyConst( scr, val[2], val[1] );
    GdialogSayOptionConstNoProc( -2, -2, 50 );
    GdialogFinish();
    scr->Flags &= ~SCR_FPROC_RUN;
}

/*
    void giq_option( int iq_test, int msg_list, int msg_num, procedure target, int reaction ) - display options in the dialog
*/
void ScrGame_GigOption( Intp_t *scr )
{
    SCP_DBG_VAR;
    int i, v12, v13, v14, val[5];
    uint16_t type[5];
    char *a3;
//char *Arg;

    scr->Flags |= SCR_FPROC_RUN;
    for( i = 0; i < 5; i++ ){
        type[ i ] = IntpPopwA( scr );
        val[ i ] = IntpPopiA( scr );
        if( type[ i ] == SCR_FSTRING ) IntpStringDeRef( scr, type[ i ], val[ i ] );
        if( (type[ i ] & 0xF7FF) == SCR_INT ) continue;
        if( i == 2 ){
            if( (type[ i ] & 0xF7FF) == SCR_STRING )
                a3 = IntpGetString( scr, type[ i ] >> 8, val[ i ] );
            else
                IntpError( "script error: %s: invalid arg %d[%x] to giq_option", scr->FileName, 2, type[ i ] );
        } else {
            IntpError( "script error: %s: invalid arg %d to giq_option", scr->FileName, i );
        }
    }
    SCP_DBGA( "giq_option( [%x]%i, [%x]%i, [%x]%i, [%x]%i, [%x]%i )", type[4], val[4], type[3], val[3], type[2], val[2], type[1], val[1], type[0], val[0] );
    a3 = NULL;
    v14 = v12 = FeatGetVal( gObjDude, FEAT_INTELLIGENCE );
    v13 = PerkLvl( gObjDude, PERK_SMOOTH_TALKER );
    if( v13 ) v14 = v12 + v13;
    if( val[ 4 ] < 0 ){
        if( -v14 < val[ 4 ] ){
            scr->Flags &= ~SCR_FPROC_RUN;
            return;
        }
    } else if( v14 < val[ 4 ] ){
        scr->Flags &= ~SCR_FPROC_RUN;
        return;
    }
    if( (type[ 1 ] & 0xF7FF) == SCR_STRING ){
//	Arg = IntpGetString( scr, type[ 1 ] >> 8, val[ 1 ] );
        if( a3 )
            GdialogSayOptionStrNoProc( val[3], a3, val[0] );
        else
            GdialogSayOptionConstNoProc( val[3], val[2], val[0] );
        scr->Flags &= ~SCR_FPROC_RUN;
        return;
    }
    if( type[ 1 ] != SCR_INT ){
        IntpError( "Invalid arg 4 to sayOption" );
        scr->Flags &= ~SCR_FPROC_RUN;
        return;
    }
    if( a3 )
        GdialogSayOptionStr( val[3], a3, val[1], val[0] );
    else
        GdialogSayOptionConst( val[3], val[2], val[1], val[0] );
    scr->Flags &= ~SCR_FPROC_RUN;
}

/*
    void poison( ObjectPtr who, int amount ) - change the level of poisoning critter
*/
void ScrGame_Poison( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val;
    Obj_t *dude;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], val, 0, "poison" );
    GETARGP( scr, type[ 1 ], dude, 1, "poison" );
    SCP_DBGA( "poison( [%x]%p, [%x],%x )", type[1], dude, type[0], val );
    if( dude ){
        if( CritterPoisonInc( dude, val ) ) eprintf( "\nScript Error: poison: adjust failed!" );
    } else {
        ScrGameErrorMsg( scr, "poison", 1 );
    }
}

/*
    int get_poison( ObjectPtr who ) - return the level of poisoning critter
*/
void ScrGame_GetPoison( Intp_t *scr )
{
    SCP_DBG_VAR;
    int state;
    Obj_t *obj;
    uint16_t type;

    state = 0;
    GETARGP( scr, type, obj, 0, "get_poison" );
    SCP_DBGA( "get_poison( [%x]%p )", type, obj );
    if( obj ){
        if( OBJTYPE( obj->Pid ) == TYPE_CRIT )
            state = CritterPoisoned( obj );
        else
            eprintf( "\nScript Error: get_poison: who is not a critter!" );
    } else {
        ScrGameErrorMsg( scr, "get_poison", 1 );
    }
    RETINT( scr, state );
}

/*
    void party_add( ObjectPtr who )
    Adds a given critter (who) into the list of party members. 
    This will also setup those objects so that they will not be saved in maps, and certain other things.
*/
void ScrGame_PartyAdd( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "party_add" );
    SCP_DBGA( "party_add( [%x]%p )", type, obj );
    if( obj )
        PartyAddMember( obj );
    else
        ScrGameErrorMsg( scr, "party_add", 1 );
}

/*
    void party_remove( ObjectPtr who ) - disconnect the player from the party member
*/
void ScrGame_PartyRemove( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "party_remove" );
    SCP_DBGA( "party_remove( [%x]%p )", type, obj );
    if( obj )
        PartyRemoveMember( obj );
    else
        ScrGameErrorMsg( scr, "party_remove", 1 );
}

/*
    void reg_anim_animate_forever( ObjectPtr what, int anim )
    Adds a single, in-place animation on an object (what) to an animation sequence-list, at a given delay from the previous animation (delay should always be -1)! 
    This animation will animate continuously until something in the system interrupts it. 
    To be used *very* sparingly, for instance Gizmo’s sign and the ‘pray’ signs in the children of the cathedral (which will have to be toned down).
*/
void ScrGame_RegAnimAnimateForever( Intp_t *scr )
{
    SCP_DBG_VAR;
    int anim;
    Obj_t *obj;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], anim, 0, "reg_anim_animate_forever" );
    GETARGP( scr, type[ 1 ], obj, 1, "reg_anim_animate_forever" );
    SCP_DBGA( "reg_anim_animate_forever( [%x]%p, [%x],%x )", type[1], obj, type[0], anim );
    if( IN_COMBAT ) return;    
    if( obj )
        AnimRegAnimateForever( obj, anim, ANIM_FOREVER );
    else
        ScrGameErrorMsg( scr, "reg_anim_animate_forever", 1 );
}

/*
    int critter_injure( ObjectPtr who, int how )
    Injures a given critter (who) by crippling given limbs/body parts (defined by DAM_CRIP_ARM_LEFT, DAM_BLIND )
*/
void ScrGame_CritterInjure( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val0,v13,v14, inj;
    Obj_t *obj;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], val0, 0, "critter_injure" );
    GETARGP( scr, type[ 1 ], obj, 1, "critter_injure" );
    SCP_DBGA( "critter_injure( [%x]%p, [%x]%i )", type[1], obj, type[0], val0 );
    if( !obj ){
        ScrGameErrorMsg( scr, "critter_injure", 1 );
        return;
    }
    inj = val0 & 0x7C;
    if( val0 & 0x800000 )
        obj->Critter.State.CombatResult &= ~inj;
    else
        obj->Critter.State.CombatResult |= inj;
    if( obj == gObjDude && ( inj & 0x30 ) ){
        IfaceUnk16( &v14, &v13 );
        IfaceHandSlotUpdate( 1, v14, v13 );
    }
}

/*
    Boolean combat_is_initialized - checks if combat is in the process
*/
void ScrGame_InCombat( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "combat_is_initialized" );
    RETINT( scr, IN_COMBAT );
}

/*
    int gdialog_barter( int bartermod ) - Tells the dialog system to switch to the barter screen.
*/
void ScrGame_GdialogBarter( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int bartermod;

    GETARGI( scr, type, bartermod, 0, "gdialog_barter" );
    SCP_DBGA( "gdialog_barter( [%x]%i )", type, bartermod );
    if( GdialogBarterMenu( bartermod ) == -1 ) eprintf( "\nScript Error: gdialog_barter: failed" );
}

/*
    int difficulty_level - returns the complexity of the game, set in the settings
*/
void ScrGame_GameDifficulty( Intp_t *scr )
{
    SCP_DBG_VAR;
    int gd;

    SCP_DBGA( "difficulty_level" );
    if( CfgGetInteger( &gConfiguration, "preferences", "game_difficulty", &gd ) != 1 ) gd = 1;
    RETINT( scr, gd );
}

/*
    int running_burning_guy - returns the running-burning-guy, set in the game settings
*/
void ScrGame_RunningBurningGuy( Intp_t *scr )
{
    SCP_DBG_VAR;
    int rbg;

    SCP_DBGA( "running_burning_guy" );
    if( CfgGetInteger( &gConfiguration, "preferences", "running_burning_guy", &rbg ) != 1 ) rbg = 1;
    RETINT( scr, rbg );
}

/*
    void inven_unwield( ObjectPtr who ) - Critter command to remove any weapons / items from both hands
*/
void ScrGame_InvenUnwield( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj;
    int n;

    obj = ScptGetSelfObj( scr );
    SCP_DBGA( "inven_unwield( %p )", obj );
    n = 1;
    if( ( obj == gObjDude ) && !IfaceGetSelectedHand() ) n = 0;
    InvUnwield( obj, n );
}

/*
    int obj_is_locked( ObjectPtr what ) - Returns True if a given object (what) is a locked object, False if it is unlocked or not a lockable object.
*/
void ScrGame_ObjIsLocked( Intp_t *scr )
{
    SCP_DBG_VAR;
    int n;
    Obj_t *obj;
    uint16_t type;

    n = 0;
    GETARGP( scr, type, obj, 0, "obj_is_locked" );
    SCP_DBGA( "obj_is_locked( [%x]%p )", type, obj );
    if( obj )
        n = UseObjLocked( obj );
    else
        ScrGameErrorMsg( scr, "obj_is_locked", 1 );
    RETINT( scr, n );
}

/*
    void obj_lock (ObjectPtr what) - trying to lock object
*/
void ScrGame_ObjLock( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_lock" );
    SCP_DBGA( "obj_lock( [%x]%p )", type, obj );
    if( obj )
        UseObjLock( obj );
    else
        ScrGameErrorMsg( scr, "obj_lock", 1 );
}

/*
    void obj_unlock( ObjectPtr what ) - Attempts to unlock a given object (what) if it is of a lockable type.
*/
void ScrGame_ObjUnlock( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_unlock" );
    SCP_DBGA( "obj_unlock( [%x]%p )", type, obj );
    if( obj )
        UseObjUnlock( obj );
    else
        ScrGameErrorMsg( scr, "obj_unlock", 1 );
}

/*
    int obj_is_open( ObjectPtr what ) - Returns True if a given object (what) is an open object, False if it is closed or not an openable object.
*/
void ScrGame_ObjIsOpen( Intp_t *scr )
{
    SCP_DBG_VAR;
    int n;
    Obj_t *obj;
    uint16_t type;

    n = 0;
    GETARGP( scr, type, obj, 0, "obj_is_open" );
    SCP_DBGA( "obj_is_open( [%x]%p )", type, obj );
    if ( obj )
        n = UseObjInAnimation( obj );
    else
        ScrGameErrorMsg(scr, "obj_is_open", 1);
    RETINT( scr, n );
}

/*
    void obj_open( ObjectPtr what ) - Attempts to open a given object (what) if it is of an openable type.
*/
void ScrGame_ObjOpen( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_open" );
    SCP_DBGA( "obj_open( [%x]%p )", type, obj );
    if( obj )
        UseObjOpen( obj );
    else
        ScrGameErrorMsg( scr, "obj_open", 1 );
}

/*
    void obj_close(ObjectPtr what) - Attempts to close a given object (what) if it is of an openable type.
*/
void ScrGame_ObjClose( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_close" );
    SCP_DBGA( "obj_close( [%x]%p )", type, obj );
    if( obj )
        UseObjClose( obj );
    else
        ScrGameErrorMsg( scr, "obj_close", 1 );
}

/*
    void game_ui_disable - block the user interface (control)
*/
void ScrGame_IfaceDisable( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "game_ui_disable" );
    GameIfaceDisable( 0 );
}

/*
    void game_ui_enable - unlock user interface (control)
*/
void ScrGame_IfaceEnable( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "game_ui_enable" );
    GameIfaceEnable();
}

/*
    boolean game_ui_is_disabled - check the status of the user interface
*/
void ScrGame_IfaceState( Intp_t *scr )
{    
    SCP_DBG_VAR;

    SCP_DBGA( "game_ui_is_disabled" );
    RETINT( scr, GameIfaceStat() );
}

/*
    void gfade_out( int time ) - screen blanking
*/
void ScrGame_GfadeOut( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;

    GETARGI( scr, type, val, 0, "gfade_out" );
    SCP_DBGA( "gfade_out( [%x]%i )", type, val );
    if( val )
        FadeStep( gFadePaletteC );
    else
        ScrGameErrorMsg( scr, "gfade_out", 1 );
}

/*
    void gfade_in( int time ) - the restoration of the screen after quenching
*/
void ScrGame_GfadeIn( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;

    GETARGI( scr, type, val, 0, "gfade_in" );
    SCP_DBGA( "gfade_in( [%x]%i )", type, val );
    if( val )
        FadeStep( gPalBase );
    else
        ScrGameErrorMsg( scr, "gfade_in", 1 );
}

/*
    int item_caps_total( ObjectPtr obj ) - Returns the current caps total in a given object’s (obj) inventory.
*/
void ScrGame_ItemCapsTotal( Intp_t *scr )
{
    SCP_DBG_VAR;
    int MoneyAll;
    Obj_t *obj;
    uint16_t type;

    MoneyAll = 0;
    GETARGP( scr, type, obj, 0, "item_caps_total" );
    SCP_DBGA( "item_caps_total( [%x]%p )", type, obj );
    if( obj )
        MoneyAll = ItemGetMoneyAll( obj );
    else
        ScrGameErrorMsg( scr, "item_caps_total", 1 );
    RETINT( scr, MoneyAll );
}

/*
    int item_caps_adjust( ObjectPtr obj, int amount ) - Modifies the current caps count in an object (obj) by a given amount (amount). Can be positive or negative.
*/
void ScrGame_ItemCapsAdjust( Intp_t *scr )
{
    SCP_DBG_VAR;
    int adj, n;
    Obj_t *obj;
    uint16_t type[ 2 ];

    n = -1;
    GETARGI( scr, type[ 0 ], adj, 0, "item_caps_adjust" );
    GETARGP( scr, type[ 1 ], obj, 1, "item_caps_adjust" );
    SCP_DBGA( "item_caps_adjust( [%x]%p, [%x]%i )", type[1], obj, type[0], adj );
    if( obj )
        n = ItemBarter( obj, adj );
    else
        ScrGameErrorMsg( scr, "item_caps_adjust", 1 );
    RETINT( scr, n );
}

/*
    int anim_action_frame( ObjectPtr who, int frame ) - returns the animation action of a given object animation
*/
void ScrGame_AnimActionFrame( Intp_t *scr )
{
    SCP_DBG_VAR;
    ArtFrmHdr_t *data;
    int val0;
    Obj_t *obj;
    CachePool_t *img;
    uint16_t type[ 2 ];
    int ActionFrame;

    ActionFrame = 0;
    GETARGI( scr, type[ 0 ], val0, 0, "anim_action_frame" );
    GETARGP( scr, type[ 1 ], obj, 1, "anim_action_frame" );
    SCP_DBGA( "anim_action_frame( [%x]%p, [%x]%i )", type[1], obj, type[0], val0 );
    if( obj ){
        if( (data = ArtLoadImg( ArtMakeId( (obj->ImgId & 0xF000000) >> 24, obj->ImgId & 0xFFF, val0, 0, obj->Orientation ), &img )) ){
            ActionFrame = ArtGetActionFrame( data );
            ArtClose( img );
        }
    } else {
        ScrGameErrorMsg( scr, "anim_action_frame", 1 );
    }
    RETINT( scr, ActionFrame );
}

/*
    void reg_anim_play_sfx( ObjectPtr who, string sfx_name, int delay )
     Adds an animation to cause an object (who) to attempt to play a given sound effect (sfx_name) at a given delay from the previous animation!
*/
void ScrGame_RegAnimPlaySfx( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *who;
    int delay, sfx_name;
    uint16_t type[ 3 ];
    char *s;

    GETARGI( scr, type[ 0 ], delay, 0, "reg_anim_play_sfx" );
    GETARGS( scr, type[ 1 ], sfx_name, 1, "reg_anim_play_sfx" );
    GETARGP( scr, type[ 2 ], who, 2, "reg_anim_play_sfx" );    
    SCP_DBGA( "reg_anim_play_sfx( [%x]%p, [%x]%i, [%x]%i )", type[2], who, type[1], sfx_name, type[0], delay );
    if( !(s = IntpGetString( scr, type[1], sfx_name ) ) ){
        ScrGameErrorMsg( scr, "reg_anim_play_sfx", 3 );
        eprintf( " Can't match string!" );
    }
    if( who )
        AnimRegPlaySfx( who, s, delay );
    else
        ScrGameErrorMsg( scr, "reg_anim_play_sfx", 1 );
}

/*
    critter_mod_skill( obj, val0, val1 )
*/
void ScrGame_CritterModSkill( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj;
    uint16_t type[ 3 ];
    int v9, i, j, v18, v19, val0, val1;
    
    GETARGI( scr, type[ 0 ], val0, 0, "critter_mod_skill" );
    GETARGI( scr, type[ 1 ], val1, 0, "critter_mod_skill" );
    GETARGP( scr, type[ 2 ], obj, 0, "critter_mod_skill" );
    SCP_DBGA( "critter_mod_skill( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], val1, type[0], val0 );
    if( obj && val0 ){
        if( OBJTYPE( obj->Pid ) == TYPE_CRIT ){
            if ( obj == gObjDude ){
                v9 = abs32( val0 );
                if( SkillIsSpecial(val1) ) v9 /= 2;
                if( val0 ){
                    for( i = 0; i < v9; i++ ) SkillInc( gObjDude, val1 );                    
                } else {
                    for( j = 0; j < v9; j++ ) SkillDec( gObjDude, val1 );
                }
                if( obj == gObjDude ){
                    IfaceUnk16( &v19, &v18 );
                    IfaceHandSlotUpdate( 0, v19, v18 );
                }
            } else {
                ScrGameErrorMsg( scr, "critter_mod_skill", 3 );
                eprintf( " Can't modify anyone except obj_dude!" );
            }
        }
    } else {
        ScrGameErrorMsg(scr, "critter_mod_skill", 1);
    }
    RETINT( scr, 0 );
}

/*
    sfx_build_char_name( ObjectPtr who, int anim_num, int gsound_character_extra )
*/
void ScrGame_SfxBuildCharName( Intp_t *scr )
{
    SCP_DBG_VAR;
    int err;
    unsigned int val0;
    int val1;
    Obj_t *obj;
    char stmp[ 16 ];
    uint16_t type[ 3 ];

    err = 0;
    GETARGI( scr, type[ 0 ], val0, 0, "sfx_build_char_name" );
    GETARGI( scr, type[ 1 ], val1, 0, "sfx_build_char_name" );
    GETARGP( scr, type[ 2 ], obj, 0, "sfx_build_char_name" );
    SCP_DBGA( "sfx_build_char_name( [%x]%p, [%x]%i, [%x]%i )", type[2], obj, type[1], val1, type[0], val0 );
    if( obj ){
        strcpy( stmp, GSoundCharacterFileName( obj, val1, val0 ) );
        err = IntpAddString( scr, stmp );
    } else {
        ScrGameErrorMsg( scr, "sfx_build_char_name", 1 );
    }
    RETFTR( scr, err );
}

/*
    sfx_build_ambient_name( string )
*/
void ScrGame_SfxBuildAmbientName( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;
    char stmp[24];

    GETARGI( scr, type, val, 0, "sfx_build_ambient_name" );
    SCP_DBGA( "sfx_build_ambient_name( [%x]%i )", type, val );
    strcpy( stmp, GSoundAmbientFileName( IntpGetString( scr, type >> 8, val ) ) );
    RETFTR( scr, IntpAddString( scr, stmp ) );
}

/*
    sfx_build_interface_name( string )
*/
void ScrGame_SfxBuildInterfaceName( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;
    char stmp[24];

    GETARGI( scr, type, val, 0, "sfx_build_interface_name" );
    SCP_DBGA( "sfx_build_interface_name( [%x]%i )", type, val );
    strcpy( stmp, GSoundItemFileName( IntpGetString( scr, type >> 8, val ) ) );
    RETFTR( scr, IntpAddString( scr, stmp ) );
}

/*
    sfx_build_item_name( string )
*/
void ScrGame_SfxBuildItemName( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;
    char stmp[24];

    GETARGI( scr, type, val, 0, "sfx_build_item_name" );
    SCP_DBGA( "sfx_build_item_name( [%x]%i )", type, val );
    strcpy( stmp, GSoundItemFileName( IntpGetString( scr, type >> 8, val ) ) );
    RETFTR( scr, IntpAddString( scr, stmp ) );
}

/*
    sfx_build_weapon_name( int gsound_sfx_weapon_action_type, ObjectPtr what, int hit_mode, ObjectPtr who )
*/
void ScrGame_SfxBuildWeaponName( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *who, *what;
    int hit_mode, action_type;
    char stmp[16];
    uint16_t type[ 4 ];

    GETARGP( scr, type[ 0 ], who, 0, "sfx_build_weapon_name" );
    GETARGI( scr, type[ 1 ], hit_mode, 1, "sfx_build_weapon_name" );
    GETARGP( scr, type[ 2 ], what, 2, "sfx_build_weapon_name" );
    GETARGI( scr, type[ 3 ], action_type, 3, "sfx_build_weapon_name" );
    SCP_DBGA( "sfx_build_weapon_name( [%x]%i, [%x]%p, [%x]%i, [%x]%p )", type[3], action_type, type[2], what, type[1], hit_mode, type[0], who );
    strcpy( stmp, GSoundWeaponFileName( action_type, what, hit_mode, who ) );
    RETFTR( scr, IntpAddString( scr, stmp ) );
}

/*
    sfx_build_scenery_name( int, int, string )
*/
void ScrGame_SfxBuildSceneryName( Intp_t *scr )
{
    SCP_DBG_VAR;
    char stmp[16];
    int val0, val1, val2;
    uint16_t type[3];

    GETARGI( scr, type[ 0 ], val0, 0, "sfx_build_scenery_name" );
    GETARGI( scr, type[ 1 ], val1, 1, "sfx_build_scenery_name" );
    GETARGI( scr, type[ 2 ], val2, 2, "sfx_build_scenery_name" );
    SCP_DBGA( "sfx_build_scenery_name( [%x]%i, [%x]%i, [%x]%i )", type[2], val2, type[1], val1, type[0], val0 );
    strcpy( stmp, GSoundSceneryFileName( val2, val1, IntpGetString( scr, type[ 1 ], val0 ) ) );
    RETFTR( scr, IntpAddString( scr, stmp ) );
}

/*
    sfx_build_open_name( ObjectPtr obj, string fname )
*/
void ScrGame_SfxBuildOpenName( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val,n;
    Obj_t *obj;
    uint16_t type[2];
    char stmp[16];

    n = 0;
    GETARGI( scr, type[ 0 ], val, 1, "sfx_build_open_name" );
    GETARGP( scr, type[ 1 ], obj, 2, "sfx_build_open_name" );
    SCP_DBGA( "sfx_build_open_name( [%x]%p, [%x]%i )", type[1], obj, type[0], val );
    if( obj ){
        strcpy( stmp, GSoundOpenFileName( obj, val ) );
        n = IntpAddString( scr, stmp );
    } else {
        ScrGameErrorMsg( scr, "sfx_build_open_name", 1 );
    }
    RETFTR( scr, n );
}

/*
    void attack_setup( ObjectPtr who, ObjectPtr victim ) - makes a critter who attack victim
*/
void ScrGame_AttackSetup( Intp_t *scr )
{
    SCP_DBG_VAR;
    Scpt01_t tmp;
    Obj_t *VictimObj, *WhoObj;
    uint16_t type[ 2 ];

    GETARGP( scr, type[ 0 ], VictimObj, 0, "attack_setup" );
    GETARGP( scr, type[ 1 ], WhoObj, 1, "attack_setup" );
    SCP_DBGA( "attack_setup( [%x]%p, [%x]%p )", type[1], WhoObj, type[0], VictimObj );
    scr->Flags |= SCR_FPROC_RUN;
    if( WhoObj ){
        if( !CritterCanTalk( WhoObj ) || ( WhoObj->Flags & 0x01 ) ){
            eprintf( "\n   But is already dead or invisible" );
            scr->Flags &= ~SCR_FPROC_RUN;
            return;
        }
        if( !CritterCanTalk( VictimObj ) || (VictimObj->Flags & 0x01) ){
            eprintf( "\n   But target is already dead or invisible" );
            scr->Flags &= ~SCR_FPROC_RUN;
            return;
        }
        if( VictimObj->Critter.State.Reaction & OBJ_STAT_FLEE ){
            eprintf( "\n   But target is AFRAID" );
            scr->Flags &= ~SCR_FPROC_RUN;
            return;
        }
        if( IN_COMBAT ){
            if( !(WhoObj->Critter.State.Reaction & 0x01) ){
                WhoObj->Critter.State.Reaction |= 0x01;
                WhoObj->Critter.State.WhoHitMeObj = VictimObj;
            }
        } else {
            tmp.crit = VictimObj;
            tmp.obj = WhoObj;
            memset( tmp.i03, 0, sizeof( tmp.i03 ) );
            tmp.i07 = 0x7FFFFFFF;
            if( WhoObj == VictimObj ) {
DD
//                tmp.i10 = VictimObj;
//                tmp.i09 = WhoObj;
                tmp.i08 = 1;
            } else {
                tmp.i08 = 0;
            }
            ScptUnk121( &tmp );
        }
    }
    scr->Flags &= ~SCR_FPROC_RUN;
}

/*
    int destroy_mult_objs( ObjectPtr item, int count ) - destroys the specified number of copies of the object
*/
void ScrGame_DestroyMultObjs( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj, *Owner;
    VidRect_t Area;
    int val[ 2 ];
    uint16_t type[ 2 ];
    int v26, v27, v16;

    v26 = 0;
    v27 = 0;
    scr->Flags |= SCR_FPROC_RUN;
    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "destroy_mult_objs" );
    GETARGP( scr, type[ 1 ], obj, 1, "destroy_mult_objs" );    
    SCP_DBGA( "destroy_mult_objs( [%x]%p, [%x]%i )", type[1], obj, type[0], val[ 0 ] );
    if( obj == ScptGetSelfObj( scr ) ) v26 = 1;
    if( OBJTYPE( obj->Pid ) == TYPE_CRIT ) CombatUnk79(obj);
    Owner = ObjGetOwner( obj );
    if( Owner ){
        v16 = Item33( Owner, obj );
        if( v16 > val[0] ) v16 = val[0];
        ItemUseItem( Owner, obj, v16 );
        if( Owner == gObjDude ) IfaceHandSlotUpdate( (GameIfaceStat() == 0), -1, -1 );
        ObjUnk14( obj, 1, 0, 0 );
        if( v26 ){
            obj->ScrId = -1;
    	    obj->Flags |= 0x05;
        } else {
            AnimRegClear( obj );
            ObjDestroy( obj, &Area );
        }
        v27 = v16;
    } else {
        AnimRegClear( obj );
        ObjDestroy( obj, &Area );
        TileUpdateArea( &Area, gMapCurrentLvl );
    }
    RETINT( scr, v27 );
    scr->Flags &= ~SCR_FPROC_RUN;
    if( v26 ) scr->Flags |= 0x1000000;
}

/*
    ObjectPtr use_obj_on_obj( ObjectPtr item, ObjectPtr targetObj ) 
	Attempt to use an item object on a target object (targetObj). 
	This could be used to have a critter use a Stimpack on the player, for instance, or to use a key on a door.
*/
void ScrGame_UseObjOnObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj1, *obj2, *p;
    uint16_t type[2];
    Scpt_t *script;

    GETARGP( scr, type[ 0 ], obj1, 0, "use_obj_on_obj" );
    GETARGP( scr, type[ 1 ], obj2, 0, "use_obj_on_obj" );
    SCP_DBGA( "use_obj_on_obj( [%x]%p, [%x]%p )", type[1], obj2, type[0], obj1 );
    if( obj2 && obj1 ){
	if( ScptPtr( ScptGetActionSource( scr ), &script ) != -1 ){
    	    p = ScptGetSelfObj( scr );
    	    if( OBJTYPE( p->Pid ) == TYPE_CRIT )
        	ActionUseSceneryObject( p, obj1, obj2 );
    	    else
        	UseObjOnObj( p, obj1, obj2 );
    	    return;
	}
    }
    ScrGameErrorMsg( scr, "use_obj_on_obj", 1 );    
}

/*
    void endgame_slideshow - lose the final slideshow
*/
void ScrGame_EndGameSlideShow( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "endgame_slideshow()" );
    scr->Flags |= SCR_FPROC_RUN;
    ScptSlideShow();
    scr->Flags &= ~SCR_FPROC_RUN;
}

/*
    void move_obj_inven_to_obj( ObjectPtr srcObj, ObjectPtr destObj ) - moves the inventory object in the inventory object srcObj destObj
*/
void ScrGame_MoveObjInvenToObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *v10, *obj, *RHandObj, *ArmorObj;
    uint16_t type[ 2 ];
    int v15, v9;

    RHandObj = 0;
    v15 = 0;
    ArmorObj = 0;
    GETARGP( scr, type[ 0 ], v10, 0, "move_obj_inven_to_obj" );
    GETARGP( scr, type[ 1 ], obj, 0, "move_obj_inven_to_obj" );
    SCP_DBGA( "move_obj_inven_to_obj( [%x]%p, [%x]%p )", type[1], obj, type[0], v10 );
    if( obj && v10 ){
        if( obj == gObjDude )
            ArmorObj = InvGetArmorObj( gObjDude );
        else
            RHandObj = InvGetRHandObj( obj );
        if( obj != gObjDude && RHandObj ){
            if( RHandObj->Flags & 0x1000000 ) v15 |= 0x1000000;
            if( RHandObj->Flags & 0x2000000 ) v15 |= 0x2000000;
            ScrGameSetShape( obj, RHandObj, v15 );
        }
        ItemMoveObjInvToObj( obj, v10 );
        if( obj == gObjDude ){
            if( ArmorObj ) InvUpdateStatistics( gObjDude, ArmorObj, 0 );
            ProtoDudeImgInit();
            if( GameIfaceStat() ) v9 = 0;
            IfaceHandSlotUpdate( v9, -1, -1 );
        }
    } else {
        ScrGameErrorMsg( scr, "move_obj_inven_to_obj", 1 );
    }
}

/*
    void endgame_movie - lose the final movie
*/
void ScrGame_EndGameMovie( Intp_t *scr )
{
    SCP_DBG_VAR;

    SCP_DBGA( "EndGameMovie()" );
    scr->Flags |= SCR_FPROC_RUN;
    EndGameMoviePlay();
    scr->Flags &= ~SCR_FPROC_RUN;
}

/*
    ObjectPtr obj_art_fid( ObjectPtr obj ) - Returns the fid # (used to index art) of a given object (obj). 
*/
void ScrGame_ObjArtFid( Intp_t *scr )
{
    SCP_DBG_VAR;
    int id = 0;
    uint16_t type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_art_fid" );
    SCP_DBGA( "obj_art_fid( [%x]%p )", type, obj );
    if( obj )
        id = obj->ImgId;
    else
        ScrGameErrorMsg( scr, "obj_art_fid", 1 );
    RETINT( scr, id );
}

/*
    void art_anim (int fid) - returns the type of animation for a given FID
*/
void ScrGame_ArtAnim( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;

    GETARGI( scr, type, val, 0, "art_anim" );
    SCP_DBGA( "art_anim( obj:[%x]%i )", type, val );
    RETINT( scr, (val & 0xFF0000) >> 16 );
}

/*
    ObjectPtr party_member_obj( int pid ) - returns a pointer to a party member for PID
*/
void ScrGame_PartyMemberObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int pid;

    GETARGI( scr, type, pid, 0, "party_member_obj" );
    SCP_DBGA( "party_member_obj( obj:[%x]%i )", type, pid );
    RETPTR( scr, PartyMemberObj( pid ) );
}

/*
    int rotation_to_tile( int srcTile, int destTile ) - Returns the rotation (0…5) to face a particular tile (destTile) from a particular tile (srcTile).
*/
void ScrGame_RotationToTile( Intp_t *scr )
{
    SCP_DBG_VAR;
    int srcTile, destTile;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], destTile, 0, "rotation_to_tile" );
    GETARGI( scr, type[ 1 ], srcTile, 0, "rotation_to_tile" );
    SCP_DBGA( "rotation_to_tile( [%x]%i, [%x]%i )", type[1], srcTile, type[0], destTile );
    RETINT( scr, TileTurnAt( srcTile, destTile ) );
}

/*
    int jam_lock(ObjectPtr lockableObj)
	Jams a lock, which prevents the player from picking the lock for approximately 24 hours. 
	Meant to be used when a player critically fails to pick a lock.
*/
void ScrGame_JamLock( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "jam_lock" );    
    SCP_DBGA( "jam_lock( obj:[%x]%p )", type, obj );
    UseObjJam( obj );
}

/*
    void gdialog_set_barter_mod( int mod )
	Sets the current modifier for barter to a given percentage (mod). 
	Used to make barter easier/harder, even if the player initiates barter (as opposed to the script starting it.)
*/
void ScrGame_GdialogSetBarterMod( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;

    GETARGI( scr, type, val, 0, "gdialog_set_barter_mod" );
    SCP_DBGA( "gdialog_set_barter_mod( mod:[%x]%i )", type, val );
    GdialogSetBarterModifier( val );
}

/*
    int combat_difficulty - returns the complexity of the fight, set in the game settings
*/
void ScrGame_CombatDifficulty( Intp_t *scr )
{
    SCP_DBG_VAR;
    int n;

    SCP_DBGA( "combat_difficulty" );
    if( CfgGetInteger( &gConfiguration, "preferences", "combat_difficulty", &n ) != 1 ) n = 0;
    RETINT( scr, n );
}

/*
    boolean obj_on_screen( ObjectPtr what ) - checks to see whether the object on the game screen
*/
void ScrGame_ObjOnScreen( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int n;
    Obj_t *obj;
    VidRect_t Area2, Area1;

    Area2 = (VidRect_t)gScrGameUnk001;
    n = 0;
    GETARGP( scr, type, obj, 1, "obj_on_screen" );    
    SCP_DBGA( "obj_on_screen( what:[%x]%p )", type, obj );
    if( obj ){
        if( gMapCurrentLvl == obj->Elevation ){
            ObjGetRefreshArea( obj, &Area1 );
            if( !RegionShrink( &Area1, &Area2, &Area1 ) ) n = 1;
        }
    } else {
        ScrGameErrorMsg( scr, "obj_on_screen", 1 );
    }
    RETINT( scr, n );
}

/*
    boolean critter_is_fleeing( ObjectPtr who ) - checks whether the critter escapes from the battlefield
*/
void ScrGame_CritterIsFleeing( Intp_t *scr )
{
    SCP_DBG_VAR;
    int n = 0;    
    Obj_t *obj;
    uint16_t type;

    GETARGP( scr, type, obj, 1, "critter_is_fleeing" );
    SCP_DBGA( "critter_is_fleeing( who:[%x]%p )", type, obj );
    if( obj )
        n = ( obj->Critter.State.Reaction & OBJ_STAT_FLEE );
    else
        ScrGameErrorMsg( scr, "critter_is_fleeing", 1 );
    RETINT( scr, n );
}

/*
    int critter_set_flee_state( ObjectPtr who, Boolean flee_on ) - Sets the FLEE flag on or off. This controls whether the critter flees during combat.
*/
void ScrGame_CritterSetFleeState( Intp_t *scr )
{
    SCP_DBG_VAR;
    int state;
    Obj_t *obj;
    uint16_t type[ 2 ];

    GETARGI( scr, type[ 0 ], state, 0, "critter_set_flee_state" );
    GETARGP( scr, type[ 1 ], obj, 1, "critter_set_flee_state" );    
    SCP_DBGA( "critter_set_flee_state( who:[%x]%p FleeOn:[%x]%i )", type[ 1 ], obj, type[ 0 ], state );
    if( obj ){
        if( state )
            obj->Critter.State.Reaction |= OBJ_STAT_FLEE;
        else
            obj->Critter.State.Reaction &= ~OBJ_STAT_FLEE;
    } else {
        ScrGameErrorMsg( scr, "critter_set_flee_state", 1 );
    }
}

/*
 terminate_combat
 void Combat Tells the combat system to terminate prematurely. USE WITH CAUTION. This doesn’t prevent another (or even the SAME) script from re-starting combat, so make sure you turn off any hostile flags, etc.
*/
void ScrGame_TerminateCombat( Intp_t *scr )
{
    SCP_DBG_VAR;
    Obj_t *obj;

    if( !IN_COMBAT ) return;
    SCP_DBGA( "terminate_combat()" );
    gMenuEscape = 1;    
    if( !(obj = ScptGetSelfObj( scr ) ) ) return;
    if( OBJTYPE( obj->Pid ) != TYPE_CRIT ) return;
    obj->Critter.State.WhoHitMeObj = NULL;
    obj->Critter.State.Reaction |= 0x02;
    CombatStopAttack( obj, NULL );
}

/*
    void debug_msg( string text ) - displays a message box debug message
*/
void ScrGame_DebugMsg( Intp_t *scr )
{
    SCP_DBG_VAR;
    int val, debug_en;
    uint16_t type;
    char *s;

    debug_en = 0;
    GETARGS( scr, type, val, 0, "debug_msg" );    
    SCP_DBGA( "debug_msg( text:[%x]%i )", type, val );
    if( (s = IntpGetString( scr, type >> 8, val )) ){
        CfgGetInteger( &gConfiguration, "const", "show_script_messages", &debug_en );
        if( debug_en ) eprintf( "\n%s", s );
    }
}

/*
    void critter_stop_attacking( ObjectPtr who ) - set a flag indicating that the critter does not want to continue the fight
*/
void ScrGame_CritterStopAttacking( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "critter_stop_attacking" );
    SCP_DBGA( "critter_stop_attacking( obj:[%x]%p )", type, obj );
    if( obj ){
        obj->Critter.State.WhoHitMeObj = NULL;
        obj->Critter.State.Reaction |= 0x02;
        CombatStopAttack( obj, NULL );
    } else {
        ScrGameErrorMsg( scr, "critter_stop_attacking", 1 );
    }
}

/*
    ObjectPtr tile_contains_pid_obj( int tile, int elev, int pid ) - returns a pointer to the first object at a given hex
*/
void ScrGame_TileContainsPidObj( Intp_t *scr )
{
    SCP_DBG_VAR;
    int tile, elev, pid;
    Obj_t *q, *p;
    uint16_t type [3 ];

    q = NULL;
    GETARGI( scr, type[ 0 ], pid, 0, "tile_contains_pid_obj" );
    GETARGI( scr, type[ 1 ], elev, 1, "tile_contains_pid_obj" );
    GETARGI( scr, type[ 2 ], tile, 2, "tile_contains_pid_obj" );
    SCP_DBGA( "tile_contains_pid_obj( tile:[%x]%i, elev:[%x]%i, pid:[%x]%i )", type[ 2 ], tile, type[ 1 ], elev, type[ 0 ], pid  );
    if( tile != -1 ){
        for( p = ObjGetFirst( elev, tile ); p; p = ObjGetNext() ){
            if( pid == p->Pid ) break;
        }
        if( p ) q = p;
    }
    RETPTR( scr, q );
}

/*
    string obj_name( ObjectPtr what ) - returns the name of the object
*/
void ScrGame_ObjName( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_name" );
    SCP_DBGA( "obj_name( obj:[%x] %p )", type, obj );
    if( obj ){
        gScrGamePlayerName = ObjGetName( obj );
    } else {
        ScrGameErrorMsg( scr, "obj_name", 1 );
    }
    RETFTR( scr, IntpAddString( scr, gScrGamePlayerName ) );
}

/*
    int get_pc_stat( int pcStat ) - Returns the value of a desired pc-only stat of the obj_dude. These are found in define.h all starting with “PCSTAT_”.
*/
void ScrGame_GetPcStat( Intp_t *scr )
{
    SCP_DBG_VAR;
    uint16_t type;
    int val;

    GETARGI( scr, type, val, 0, "get_pc_stat" );
    SCP_DBGA( "get_pc_stat( pcStat:[%x] %x )", type, val );
    RETINT( scr, FeatGetPoints( val ) );
}

void ScpGameSetup()
{
    SciAddOpcode( 0x80A1, ScrGame_GiveExpPoints );
    SciAddOpcode( 0x80A2, ScrGame_Return );
    SciAddOpcode( 0x80A3, ScrGame_PlaySfx );
    SciAddOpcode( 0x80A4, ScrGame_ObjName );
    SciAddOpcode( 0x80A5, ScrGame_SfxBuildOpenName );
    SciAddOpcode( 0x80A6, ScrGame_GetPcStat );
    SciAddOpcode( 0x80A7, ScrGame_TileContainsPidObj );
    SciAddOpcode( 0x80A8, ScrGame_SetMapStart );
    SciAddOpcode( 0x80A9, ScrGame_OverrideMapStart );
    SciAddOpcode( 0x80AA, ScrGame_HasSkill );
    SciAddOpcode( 0x80AB, ScrGame_UsingSkill );
    SciAddOpcode( 0x80AC, ScrGame_RollVsSkill );
    SciAddOpcode( 0x80AD, ScrGame_SkillContest );
    SciAddOpcode( 0x80AE, ScrGame_DoCheck );
    SciAddOpcode( 0x80AF, ScrGame_Success );
    SciAddOpcode( 0x80B0, ScrGame_Critical );
    SciAddOpcode( 0x80B1, ScrGame_HowMuch );
    SciAddOpcode( 0x80B2, ScrGame_MarkAreaKnown );
    SciAddOpcode( 0x80B3, ScrGame_ReactionInfluence );
    SciAddOpcode( 0x80B4, ScrGame_Random );
    SciAddOpcode( 0x80B5, ScrGame_RollDice );
    SciAddOpcode( 0x80B6, ScrGame_MoveTo );
    SciAddOpcode( 0x80B7, ScrGame_CreateObject );
    SciAddOpcode( 0x80B8, ScrGame_DisplayMsg );
    SciAddOpcode( 0x80B9, ScrGame_ScriptOverrides );
    SciAddOpcode( 0x80BA, ScrGame_ObjIsCarryingObj );
    SciAddOpcode( 0x80BB, ScrGame_TileContainsObjPid );
    SciAddOpcode( 0x80BC, ScrGame_SelfObj );
    SciAddOpcode( 0x80BD, ScrGame_SourceObj );
    SciAddOpcode( 0x80BE, ScrGame_TargetObj );
    SciAddOpcode( 0x80BF, ScrGame_DudeObj );
    SciAddOpcode( 0x80C0, ScrGame_ObjBeingUsedWith );
    SciAddOpcode( 0x80C1, ScrGame_OpLocalvar );
    SciAddOpcode( 0x80C2, ScrGame_SetLocalvar );
    SciAddOpcode( 0x80C3, ScrGame_OpMapVar );
    SciAddOpcode( 0x80C4, ScrGame_SetMapVar );
    SciAddOpcode( 0x80C5, ScrGame_OpGlobalVar );
    SciAddOpcode( 0x80C6, ScrGame_OpSetGlobalVar );
    SciAddOpcode( 0x80C7, ScrGame_ScriptAction );
    SciAddOpcode( 0x80C8, ScrGame_OpObjType );
    SciAddOpcode( 0x80C9, ScrGame_OpItemSubtype );
    SciAddOpcode( 0x80CA, ScrGame_GetCritterStat );
    SciAddOpcode( 0x80CB, ScrGame_SetCritterStat );
    SciAddOpcode( 0x80CC, ScrGame_AnimateStandObj );
    SciAddOpcode( 0x80CD, ScrGame_AnimateStandReverseObj );
    SciAddOpcode( 0x80CE, ScrGame_AnimateMoveObjToTile );
    SciAddOpcode( 0x80CF, ScrGame_TileInTileRect );
    SciAddOpcode( 0x80D0, ScrGame_Attack );
    SciAddOpcode( 0x80D1, ScrGame_MakeDayTime );
    SciAddOpcode( 0x80D2, ScrGame_TileDistance );
    SciAddOpcode( 0x80D3, ScrGame_TileDistanceObjs );
    SciAddOpcode( 0x80D4, ScrGame_TileNuM );
    SciAddOpcode( 0x80D5, ScrGame_TileNumInDirection );
    SciAddOpcode( 0x80D6, ScrGame_PickupObj );
    SciAddOpcode( 0x80D7, ScrGame_DropObj );
    SciAddOpcode( 0x80D8, ScrGame_AddObjToInv );
    SciAddOpcode( 0x80D9, ScrGame_RmObjFromInv );
    SciAddOpcode( 0x80DA, ScrGame_WieldObjCritter );
    SciAddOpcode( 0x80DB, ScrGame_UseObj );
    SciAddOpcode( 0x80DC, ScrGame_ObjCanSeeObj );
    SciAddOpcode( 0x80DD, ScrGame_Attack );
    SciAddOpcode( 0x80DE, ScrGame_StartGdialog );
    SciAddOpcode( 0x80DF, ScrGame_EndDialogue );
    SciAddOpcode( 0x80E0, ScrGame_DialogueReaction );
    SciAddOpcode( 0x80E1, ScrGame_Metarule3 );
    SciAddOpcode( 0x80E2, ScrGame_SetMapMusic );
    SciAddOpcode( 0x80E3, ScrGame_SetObjVisibility );
    SciAddOpcode( 0x80E4, ScrGame_LoadMap );
    SciAddOpcode( 0x80E5, ScrGame_WmAreaSetPos );
    SciAddOpcode( 0x80E6, ScrGame_SetExitGrids );
    SciAddOpcode( 0x80E7, ScrGame_AnimBusy );
    SciAddOpcode( 0x80E8, ScrGame_CritterHeal );
    SciAddOpcode( 0x80E9, ScrGame_SetLightLevel );
    SciAddOpcode( 0x80EA, ScrGame_GameTime );
    SciAddOpcode( 0x80EB, ScrGame_GameTimeInSeconds );
    SciAddOpcode( 0x80EC, ScrGame_Elevation );
    SciAddOpcode( 0x80ED, ScrGame_KillCritter );
    SciAddOpcode( 0x80EE, ScrGame_KillCritterType );
    SciAddOpcode( 0x80EF, ScrGame_CritterDamage );
    SciAddOpcode( 0x80F0, ScrGame_OpAddTimerEvent );
    SciAddOpcode( 0x80F1, ScrGame_RmTimeEvent );
    SciAddOpcode( 0x80F2, ScrGame_GameTicks );
    SciAddOpcode( 0x80F3, ScrGame_HasTrait );
    SciAddOpcode( 0x80F4, ScrGame_DestroyObject );
    SciAddOpcode( 0x80F5, ScrGame_ObjCanHearObj );
    SciAddOpcode( 0x80F6, ScrGame_GameTimeHour );
    SciAddOpcode( 0x80F7, ScrGame_FixedParam );
    SciAddOpcode( 0x80F8, ScrGame_TileIsVisible );
    SciAddOpcode( 0x80F9, ScrGame_DialogueSystemEnter );
    SciAddOpcode( 0x80FA, ScrGame_ActionBeingUsed );
    SciAddOpcode( 0x80FB, ScrGame_CritterState );
    SciAddOpcode( 0x80FC, ScrGame_GameTimeAdvance );
    SciAddOpcode( 0x80FD, ScrGame_RadiationInc );
    SciAddOpcode( 0x80FE, ScrGame_RadiationDec );
    SciAddOpcode( 0x80FF, ScrGame_CritterAttemptPlacement );
    SciAddOpcode( 0x8100, ScrGame_ObjPid );
    SciAddOpcode( 0x8101, ScrGame_CurMapIndex );
    SciAddOpcode( 0x8102, ScrGame_OpCritterAddTrait );
    SciAddOpcode( 0x8103, ScrGame_CritterRmTrait );
    SciAddOpcode( 0x8104, ScrGame_ProtoData );
    SciAddOpcode( 0x8105, ScrGame_MessageStr );
    SciAddOpcode( 0x8106, ScrGame_CritterInvenObj );
    SciAddOpcode( 0x8107, ScrGame_ObjSetLightLevel );
    SciAddOpcode( 0x8108, ScrGame_WorldMap );
    SciAddOpcode( 0x8109, ScrGame_InvenCmds );
    SciAddOpcode( 0x810A, ScrGame_FloatMsg );
    SciAddOpcode( 0x810B, ScrGame_MetaRule );
    SciAddOpcode( 0x810C, ScrGame_OpAnim );
    SciAddOpcode( 0x810D, ScrGame_ObjCarryingPidObj );
    SciAddOpcode( 0x810E, ScrGame_RegAnimFunc );
    SciAddOpcode( 0x810F, ScrGame_RegAnimAnimate );
    SciAddOpcode( 0x8110, ScrGame_RegAnimAnimateReverse );
    SciAddOpcode( 0x8111, ScrGame_RegAnimObjMoveToObj );
    SciAddOpcode( 0x8112, ScrGame_RegAnimObjRunToObj );
    SciAddOpcode( 0x8113, ScrGame_RegAnimObjMoveToTile );
    SciAddOpcode( 0x8114, ScrGame_RegAnimObjRunToTile );
    SciAddOpcode( 0x8115, ScrGame_PlayGmovie );
    SciAddOpcode( 0x8116, ScrGame_AddMultObjsToInven );
    SciAddOpcode( 0x8117, ScrGame_RmMultObjsFromInven );
    SciAddOpcode( 0x8118, ScrGame_GetMonth );
    SciAddOpcode( 0x8119, ScrGame_GetDay );
    SciAddOpcode( 0x811A, ScrGame_Explosion );
    SciAddOpcode( 0x811B, ScrGame_DaysSinceVisited );
    SciAddOpcode( 0x811C, ScrGame_StartDialog );
    SciAddOpcode( 0x811D, ScrGame_GsayEnd );
    SciAddOpcode( 0x811E, ScrGame_GsayReply );
    SciAddOpcode( 0x811F, ScrGame_GsayOption );
    SciAddOpcode( 0x8120, ScrGame_GsayMessage );
    SciAddOpcode( 0x8121, ScrGame_GigOption );
    SciAddOpcode( 0x8122, ScrGame_Poison );
    SciAddOpcode( 0x8123, ScrGame_GetPoison );
    SciAddOpcode( 0x8124, ScrGame_PartyAdd );
    SciAddOpcode( 0x8125, ScrGame_PartyRemove );
    SciAddOpcode( 0x8126, ScrGame_RegAnimAnimateForever );
    SciAddOpcode( 0x8127, ScrGame_CritterInjure );
    SciAddOpcode( 0x8128, ScrGame_InCombat );
    SciAddOpcode( 0x8129, ScrGame_GdialogBarter );
    SciAddOpcode( 0x812A, ScrGame_GameDifficulty );
    SciAddOpcode( 0x812B, ScrGame_RunningBurningGuy );
    SciAddOpcode( 0x812C, ScrGame_InvenUnwield );
    SciAddOpcode( 0x812D, ScrGame_ObjIsLocked );
    SciAddOpcode( 0x812E, ScrGame_ObjLock );
    SciAddOpcode( 0x812F, ScrGame_ObjUnlock );
    SciAddOpcode( 0x8131, ScrGame_ObjOpen );
    SciAddOpcode( 0x8130, ScrGame_ObjIsOpen );
    SciAddOpcode( 0x8132, ScrGame_ObjClose );
    SciAddOpcode( 0x8133, ScrGame_IfaceDisable );
    SciAddOpcode( 0x8134, ScrGame_IfaceEnable );
    SciAddOpcode( 0x8135, ScrGame_IfaceState );
    SciAddOpcode( 0x8136, ScrGame_GfadeOut );
    SciAddOpcode( 0x8137, ScrGame_GfadeIn );
    SciAddOpcode( 0x8138, ScrGame_ItemCapsTotal );
    SciAddOpcode( 0x8139, ScrGame_ItemCapsAdjust );
    SciAddOpcode( 0x813A, ScrGame_AnimActionFrame );
    SciAddOpcode( 0x813B, ScrGame_RegAnimPlaySfx );
    SciAddOpcode( 0x813C, ScrGame_CritterModSkill );
    SciAddOpcode( 0x813D, ScrGame_SfxBuildCharName );
    SciAddOpcode( 0x813E, ScrGame_SfxBuildAmbientName );
    SciAddOpcode( 0x813F, ScrGame_SfxBuildInterfaceName );
    SciAddOpcode( 0x8140, ScrGame_SfxBuildItemName );
    SciAddOpcode( 0x8141, ScrGame_SfxBuildWeaponName );
    SciAddOpcode( 0x8142, ScrGame_SfxBuildSceneryName );
    SciAddOpcode( 0x8143, ScrGame_AttackSetup );
    SciAddOpcode( 0x8144, ScrGame_DestroyMultObjs );
    SciAddOpcode( 0x8145, ScrGame_UseObjOnObj );
    SciAddOpcode( 0x8146, ScrGame_EndGameSlideShow );
    SciAddOpcode( 0x8147, ScrGame_MoveObjInvenToObj );
    SciAddOpcode( 0x8148, ScrGame_EndGameMovie );
    SciAddOpcode( 0x8149, ScrGame_ObjArtFid );
    SciAddOpcode( 0x814A, ScrGame_ArtAnim );
    SciAddOpcode( 0x814B, ScrGame_PartyMemberObj );
    SciAddOpcode( 0x814C, ScrGame_RotationToTile );
    SciAddOpcode( 0x814D, ScrGame_JamLock );
    SciAddOpcode( 0x814E, ScrGame_GdialogSetBarterMod );
    SciAddOpcode( 0x814F, ScrGame_CombatDifficulty );
    SciAddOpcode( 0x8150, ScrGame_ObjOnScreen );
    SciAddOpcode( 0x8151, ScrGame_CritterIsFleeing );
    SciAddOpcode( 0x8152, ScrGame_CritterSetFleeState );
    SciAddOpcode( 0x8153, ScrGame_TerminateCombat );
    SciAddOpcode( 0x8154, ScrGame_DebugMsg );
    SciAddOpcode( 0x8155, ScrGame_CritterStopAttacking );
}


