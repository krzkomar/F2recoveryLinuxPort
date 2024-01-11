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
        if( (type & 0xF7FF) != SCR_INT ) IntpError( "script error: %s: invalid arg %d to "#name, scr->FileName, arg_num );

#define GETARGP( scr, type, arg, arg_num, name )	\
        type = IntpPopwA( scr );\
        arg = IntpPopPtrA( scr );\
        if( type == SCR_FSTRING ) IntpStringDeRef( scr, type, PTR2INT( arg ) );\
        if( (type & 0xF7FF) != SCR_PTR ) IntpError( "script error: %s: invalid arg %d to "#name, scr->FileName, arg_num );

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




short gScrGameMovieFlags[ 17 ] = { 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,  0x9, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B };

VidRect_t gScrGameUnk001 = { 0, 0, 0x280, 0x1E0 };

int gScrGameMood;
char *gScrGamePlayerName;

void ScrGameErrorMsg( Intp_t *strerr, const char *Operand, int ErrClass )
{
//    char stmp[268]; // [esp+0h] [ebp-10Ch] BYREF

//    sprintf(stmp, "Script Error: %s: op_%s: %s", *(const char **)strerr, Operand, ScriptErrClass[ErrClass]);
//    return eprintf(stmp);
}

void ScrGameEprintf( const char *fmt, ... )
{
/*
    char Tmp[256];
    va_list vargs;
    va_list valist;

    va_start( valist, fmt );
    va_copy( vargs, valist );
    vsprintf( Tmp, fmt, (va_list)&vargs );
    eprintf( Tmp );
*/
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
        TileUpdateArea( &Area, gCurrentMapLvl );
    }
    return 0;
}

/*
    void give_exp_points( int points )
*/
void ScrGame_GiveExpPoints( Intp_t *scr )
{
    short type;
    int val;

    GETARGI( scr, type, val, 0, "give_exp_points" );
    if( FeatLvlUp( val ) ) ScrGameEprintf( "\nScript Error: %s: op_give_exp_points: stat_pc_set failed", scr->FileName );
}

/*
    void scr_return
*/
void ScrGame_Return( Intp_t *scr )
{
    short type;
    int val;
    Scpt_t *spt;

    GETARGI( scr, type, val, 0, "scr_return" );
    if( ScptPtr( ScptGetActionSource( scr ), &spt ) != -1 ) spt->i11 = val;
}

/*
    void play_sfx( string sfxFileName )
*/
void ScrGame_PlaySfx( Intp_t *scr )
{
    short type;
    int val;

    GETARGS( scr, type, val, 0, "play_sfx" );
    GSoundPlay( IntpGetArg( scr, type >> 8, val ) );
}

/*
    void set_map_start( int x, int y, int elev, int rot )
*/
void ScrGame_SetMapStart( Intp_t *scr )
{
    int i, MapIdx, Val[ 4 ];
    short Type[ 4 ];

    for( i = 0; i < 4; i++ ){
	GETARGI( scr, Type[ i ], Val[ i ], i, "set_map_start" );
    }
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
    void override_map_start( int x, int y, int elev, int rot )
*/
void ScrGame_OverrideMapStart( Intp_t *scr )
{
    unsigned int Val[ 4 ], GridPos, i;
    char stmp[ 60 ];
    short Type[ 4 ];

    scr->Flags |= 0x20;
    for( i = 0; i < 4; i++ ){
	GETARGI( scr, Type[ i ], Val[ i ], i, "override_map_start" );
    }
    sprintf( stmp, "OVERRIDE_MAP_START: x: %d, y: %d", Val[ 3 ], Val[ 2 ] );
    eprintf( "%s", stmp );
    
    GridPos = 200 * Val[ 2 ] + Val[ 3 ];
    if( GridPos != -1 ){
        if( ObjSetRotation( gObjDude, Val[ 0 ], 0 ) ) ScrGameEprintf("\nError: %s: obj_set_rotation failed in override_map_start!", scr->FileName);
        if( ObjMoveToTile( gObjDude, GridPos, Val[ 1 ], 0 ) ){
            ScrGameEprintf( "\nError: %s: obj_move_to_tile failed in override_map_start!", scr->FileName );
            if( ObjMoveToTile( gObjDude, gTileCentIdx, Val[ 1 ], 0 ) ){
                ScrGameEprintf( "\nError: %s: obj_move_to_tile RECOVERY Also failed!", scr->FileName );
                exit( 1 );
            }
        }
        TileSetCenter( GridPos, 1 );
        TileUpdate();
    }
    scr->Flags &= ~0x20;
}

/*
    int has_skill( ObjectPtr who, int skill )
*/
void ScrGame_HasSkill( Intp_t *scr )
{
    int Val, Total;
    Obj_t *critter;
    short Type1, Type2;

    GETARGI( scr, Type1, Val, 0, "has_skill" );
    GETARGP( scr, Type2, critter, 1, "has_skill" );

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
    boolean using_skill( ObjectPtr who, int skill )
*/
void ScrGame_UsingSkill( Intp_t *scr )
{
    Obj_t *critter;
    int Val;
    short Type1, Type2;

    GETARGI( scr, Type1, Val, 0, "using_skill" );
    GETARGP( scr, Type2, critter, 1, "using_skill" );

    if( Val == 8 && critter == gObjDude ) Val = CritterUsingSkill( 0 );
    RETINT( scr, Val );
}

/*
    int roll_vs_skill( ObjectPtr who, int skill, int modifier )
*/
void ScrGame_RollVsSkill( Intp_t *scr )
{
//    Scpt_t *script;
    Obj_t *critter;
    int skill, modifier, sk;
    short Type[ 3 ];

    GETARGI( scr, Type[ 0 ], modifier, 0, "roll_vs_skill" );
    GETARGI( scr, Type[ 1 ], skill, 1, "roll_vs_skill" );
    GETARGP( scr, Type[ 2 ], critter, 2, "roll_vs_skill" );

    sk = 0;
    if( critter ){
        if( OBJTYPE( critter->Pid ) == TYPE_CRIT ){
//            if( ScptPtr( ScptGetActionSource( scr ), &script ) != -1 ) sk = SkillUse( critter, skill, script->i20, modifier );
        }
    } else {
        ScrGameErrorMsg( scr, "roll_vs_skill", 1 );
    }
    RETINT( scr, sk );
}

/*
    ** not fully implemented in game, return is always 0 **    
    int skill_contest( int, int, int )
*/
void ScrGame_SkillContest( Intp_t *scr )
{
    int i;
    int Val[ 3 ];
    short Type[ 3 ];

    for( i = 0; i < 3; i++ ){
	GETARGI( scr, Type[ i ], Val[ i ], i, "skill_contest" );
    }
    ScrGameErrorMsg( scr, "skill_contest", 0 );
    RETINT( scr, 0 );
}

/*
    int do_check( ObjectPtr who, int check, int modifier )
*/
void ScrGame_DoCheck( Intp_t *scr )
{
    Scpt_t *script;
    Obj_t *obj;
    int a3[2], val;
    short a2[3];
    
    val = 0;
    GETARGI( scr, a2[ 0 ], a3[ 0 ], 0, "do_check" );
    GETARGI( scr, a2[ 1 ], a3[ 1 ], 1, "do_check" );
    GETARGP( scr, a2[ 2 ], obj, 2, "do_check" );
    
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
    int success( int )
*/
void ScrGame_Success( Intp_t *scr )
{
    int k, val;
    short type;

    GETARGI( scr, type, val, 0, "success" );
    switch( val ){
        case 0 ... 1: k = 0; break;
        case 2 ... 3: k = 1; break;
        default: k = -1; break;
    }
    RETINT( scr, k );
}

/* 
    int critical( int ) 
*/
void ScrGame_Critical( Intp_t *scr )
{
    int stat, val;
    short type;

    GETARGI( scr, type, val, 0, "critical" );
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
    int how_much( int val )
*/
void ScrGame_HowMuch( Intp_t *scr )
{
    Scpt_t *script;
    short type;
    int hm, Val;

    hm = 0;
    GETARGI( scr, type, Val, 0, "how_much" );
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "how_much", 2 );
    else
        hm = script->i20;
    RETINT( scr, hm );
}

/*
    mark_area_known( int mark_type, int area, int mark_state )
    mark_type: town_map, world_map
*/
void ScrGame_MarkAreaKnown( Intp_t *scr )
{
    int i, val[ 3 ];
    short type[ 3 ];

    for( i = 0; i < 3; i++ ){
	GETARGI( scr, type[ i ], val[ i ], i, "mark_area_known" );
    }
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
    int val[ 3 ], i;
    short type[ 3 ];

    for( i = 0; i < 3; i++ ){
	GETARGI( scr, type[ i ], val[ i ], i, "reaction_influence" );
    }
    RETINT( scr, EvQeGetReactionInfluence( val[2], val[1], val[0] ) );
}
    
/*
    int random( int min, int max )
*/
void ScrGame_Random( Intp_t *scr )
{
    int i, Val[ 2 ];
    short type[ 2 ];

    for( i = 0; i < 2; i++ ){
	GETARGI( scr, type[ i ], Val[ i ], i, "random" );
    }
    RETINT( scr, ( RecGetState() == 2 ) ?  RandMinMax( Val[1], Val[0] ) : ((Val[0] - Val[1]) / 2) );
}

/*
    ** not fully implemented in game, return is always 0 **
    Critter roll_dice( obj Critter, int Skill )
*/
void ScrGame_RollDice( Intp_t *scr )
{
    int i, val[ 2 ];
    short type[ 2 ];

    for( i = 0; i < 2; i++ ){
	GETARGI( scr, type[ i ], val[ i ], i, "roll_dice" );
    }
    ScrGameErrorMsg( scr, "roll_dice", 0 );
    RETINT( scr, 0 );
}

/*
    int move_to( ObjectPtr obj, int tile_num, 0-2 elev )
*/
void ScrGame_MoveTo( Intp_t *scr )
{
    int a,b;
    int lvl, tile_num, k = 0;
    short a2[ 3 ];
    Obj_t *obj;
    VidRect_t Rect, Area, Area2;

    GETARGI( scr, a2[ 0 ], lvl, 0, "move_to" );
    GETARGI( scr, a2[ 1 ], tile_num, 1, "move_to" );
    GETARGP( scr, a2[ 2 ], obj, 2, "move_to" );

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
            ObjGetRadiusArea( obj, &Rect );
            if( ( lvl != obj->Elevation) && (OBJTYPE( obj->Pid ) == 1 ) ) CombatUnk79( obj );            
            if( ( k = ObjMoveToTile( obj, tile_num, lvl, &Area2 ) ) != -1 ){
                RegionExpand( &Rect, &Area2, &Area2 );
                TileUpdateArea( &Area2, gCurrentMapLvl );
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
    VidRect_t Area;
    Scpt_t *script;
    Proto_t *proto;
    unsigned int Pid_high;
    int pid, tile_num, lvl, sid, n;
    short type[4];
    Obj_t *Obj;

    n = 0;
    Obj = NULL;
    GETARGI( scr, type[ 0 ], sid, 0, "create_object" );
    GETARGI( scr, type[ 1 ], lvl, 1, "create_object" );
    GETARGI( scr, type[ 2 ], tile_num, 2, "create_object" );
    GETARGI( scr, type[ 3 ], pid, 3, "create_object" );
    
    if( LsgGetUnk02() ){
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
                ScptUnk102( sid - 1, Obj->ScrId );
            }
        }
    } else {
        eprintf( "\nError: attempt to Create critter With PID of 0: %s!", scr->FileName );
    }
    RETPTR( scr, Obj );
}

/*
    int destroy_object( ObjectPtr obj )
*/
void ScrGame_DestroyObject( Intp_t *scr )
{
    Obj_t *obj, *Owner;
    VidRect_t Area, v14;
    short type;
    int v3;

    scr->Flags |= 0x20;
    v3 = 0;
    GETARGP( scr, type, obj, 0, "destroy_object" );
    if( !obj ){
        ScrGameErrorMsg( scr, "destroy_object", 1 );
        scr->Flags &= ~0x20;
        return;
    }
    if( OBJTYPE( obj->Pid ) == 1 && LsgGetUnk02() ){
        eprintf( "\nError: attempt to destroy critter in load/save-game: %s!", scr->FileName );
        scr->Flags &= ~0x20;
        return;
    }
    if( obj == ScptUnk140( scr ) ) v3 = 1;
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
            AnimClear( obj );
            ObjDestroy( obj, 0 );
        }
    } else {
        AnimClear( obj );
        ObjDestroy( obj, &Area );
        TileUpdateArea( &v14, gCurrentMapLvl );
    }
    scr->Flags &= ~0x20;
    if( v3 ) scr->Flags |= 0x1000000;    
}

/*
    void display_msg( string message )
*/
void ScrGame_DisplayMsg( Intp_t *scr )
{
    short type;
    int dbg, val;

    GETARGS( scr, type, val, 0, "display_msg" );

    IfcMsgOut( IntpGetArg( scr, type >> 8, val ) );
    CfgGetInteger( &gConfiguration, "debug", "show_script_messages", &dbg );
}

/*
    void script_overrides
*/
void ScrGame_ScriptOverrides( Intp_t *scr )
{
    Scpt_t *script;

    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "script_overrides", 2 );
    else
        script->i18 = 1;
}

/*
    int obj_is_carrying_obj( ObjectPtr invenObj, int item_pid )
*/
void ScrGame_ObjIsCarryingObj( Intp_t *scr )
{
    Obj_t *obj;
    int item_pid, QuantityTot;
    short a2[ 2 ];
    
    QuantityTot = 0;	
    GETARGI( scr, a2[ 0 ], item_pid, 0, "obj_is_carrying_obj" );
    GETARGP( scr, a2[ 1 ], obj, 1, "obj_is_carrying_obj" );
    if( obj )
        QuantityTot = InvGetQuantityTot( obj, item_pid );
    else
        ScrGameErrorMsg( scr, "obj_is_carrying_obj_pid", 1 );
    RETINT( scr, QuantityTot );
}

/*
    boolean tile_contains_obj_pid( int tile, int elev, int pid )
*/
void ScrGame_TileContainsObjPid( Intp_t *scr )
{
    Obj_t *p;
    int i,reply, val[ 3 ];
    short type[ 3 ];

    reply = 0;
    for( i = 0; i < 3; i++ ){
        GETARGI( scr, type[ i ], val[ i ], i, "tile_contains_obj_pid" );
    }    
    for( p = ObjGetFirst( val[ 1 ], val[ 2 ] ); p; p = ObjGetNext() ){
        if( val[ 0 ] == p->Pid ){ reply = 1; break; }
    }
    RETINT( scr, reply );
}

/*
    ObjectPtr ?()
*/
void ScrGameUnk02( Intp_t *scr )
{
    RETPTR( scr, ScptUnk140( scr ) );
}

/*
    ObjPtr source_obj() - returns a pointer to the source object (activator) for this action
*/
void ScrGame_SourceObj( Intp_t *scr )
{
    Obj_t *crit;
    Scpt_t *script;

    crit = 0;
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "source_obj", 2 );
    else
        crit = script->crit;
    RETPTR( scr, crit );
}

/*
    ObjectPtr target_obj()
*/
void ScrGame_TargetObj( Intp_t *scr )
{
    int ActionSource;
    Scpt_t *script;
    Obj_t *obj;

    ActionSource = ScptGetActionSource(scr);
    obj = NULL;
    if( ScptPtr( ActionSource, &script ) == -1 )
        ScrGameErrorMsg( scr, "target_obj", 2 );
    else
        obj = script->item;
    RETPTR( scr, obj );
}

/*
    ObjectPtr dude_obj
*/
void ScrGame_DudeObj( Intp_t *scr )
{
    RETPTR( scr, gObjDude );
}

/*
    ObjectPtr obj_being_used_with
*/
void ScrGame_ObjBeingUsedWith( Intp_t *scr )
{
    Scpt_t *script;
    Obj_t *obj;

    obj = NULL;
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "obj_being_used_with", 2 );
    else
        obj = script->item;
    RETPTR( scr, obj );
}

/*
    int op_local_var
*/
void ScrGame_OpLocalvar( Intp_t *scr )
{
    short type;
    int var, tmp;

    tmp = -1;
    GETARGI( scr, type, var, 0, "op_local_var" );
    ScptGetLocVar( ScptGetActionSource( scr ), var, &tmp );
    RETINT( scr, tmp );
}

/*
    void set_local_var( int var_index, int value )
*/
void ScrGame_SetLocalvar( Intp_t *scr )
{
    int i, var[ 2 ];
    short type[ 2 ];

    for( i = 0; i < 2; i++ ){
	GETARGI( scr, type[ i ], var[ i ], i, "set_local_var" );
    }
    ScptSetLocVar( ScptGetActionSource( scr ), var[ 1 ], var[ 0 ] );
}

/*
    int op_map_var( int var_index )
*/
void ScrGame_OpMapVar( Intp_t *scr )
{
    short type;
    int var;

    GETARGI( scr, type, var, 0, "op_map_var" );
    RETINT( scr, MapGetVar( var ) );
}

/*
    void set_map_var( int var_index, int value )
*/
void ScrGame_SetMapVar( Intp_t *scr )
{
    int i, var[ 2 ];
    short type[ 2 ];

    for( i = 0; i < 2; i++ ){
	GETARGI( scr, type[ i ], var[ i ], i, "set_map_var" );
    }
    MapSetVar( var[ 1 ], var[ 0 ] );
}

/*
    int op_global_var( int var_index )
*/
void ScrGame_OpGlobalVar( Intp_t *scr )
{
    int v2, var;
    short type;

    GETARGI( scr, type, var, 0, "op_global_var" );
    v2 = -1;
    if( gGValCount )
        v2 = GlobVarGet( var );
    else
        ScrGameEprintf( "\nScript Error: %s: op_global_var: no global vars found!", scr->FileName );
    RETINT( scr, v2 );
}

/*
    void set_global_var( int var_index, int value )
*/
void ScrGame_OpSetGlobalVar( Intp_t *scr )
{
    int i, var[ 2 ];
    short type[ 2 ];

    for( i = 0; i < 2; i++ ){
	GETARGI( scr, type[ i ], var[ i ], i, "set_global_var" );
    }
    if( gGValCount )
        GlobVarSet( var[1], var[0] );
    else
        ScrGameEprintf( "\nScript Error: %s: op_set_global_var: no global vars found!", scr->FileName );
}

/*
    int script_action
*/
void ScrGame_ScriptAction( Intp_t *scr )
{
    int v4;
    Scpt_t *script;

    v4 = 0;
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "script_action", 2 );
    else
        v4 = script->i12;
    RETINT( scr, v4 );
}

/*
    int op_obj_type( ObjectPtr )
*/
void ScrGame_OpObjType( Intp_t *scr )
{
    Obj_t *obj;
    int ObjType;
    short type;

    ObjType = -1;
    GETARGP( scr, type, obj, 0, "op_obj_type" );
    if( obj ) ObjType = OBJTYPE( obj->ImgId );
    RETINT( scr, ObjType );
}

/*
    int op_item_subtype( ObjectPtr item )
*/
void ScrGame_OpItemSubtype( Intp_t *scr )
{
    Obj_t *obj;
    Proto_t *proto;
    short type;
    int subtype;

    GETARGP( scr, type, obj, 0, "op_item_subtype" );
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
    int get_critter_stat( ObjectPtr who, int stat )
*/
void ScrGame_GetCritterStat( Intp_t *scr )
{
    Obj_t *obj;
    int r, val;
    short type[2];

    GETARGI( scr, type[ 0 ], val, 0, "get_critter_stat" );
    GETARGP( scr, type[ 1 ], obj, 1, "get_critter_stat" );
    if( obj ){
        r = FeatGetVal( obj, val );
    } else {
        ScrGameErrorMsg( scr, "get_critter_stat", 1 );
        r = -1;
    }
    RETINT( scr, r );
}

/*
    int set_critter_stat( ObjectPtr who, int stat, int amount )
*/
void ScrGame_SetCritterStat( Intp_t *scr )
{
    short type[ 3 ];
    Obj_t *who;
    int stat, amount, err;

    GETARGI( scr, type[ 0 ], amount, 0, "set_critter_stat" );
    GETARGI( scr, type[ 1 ], stat, 1, "set_critter_stat" );
    GETARGP( scr, type[ 2 ], who, 2, "set_critter_stat" );

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
    void animate_stand_obj( ObjectPtr obj )
*/
void ScrGame_AnimateStandObj( Intp_t *scr )
{
    short type;
    Scpt_t *script;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "animate_stand_obj" );
    if( !obj ){
        if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ){
            ScrGameErrorMsg( scr, "animate_stand_obj", 2 );
            return;
        }
        ScptUnk140( scr );
    }
    if( !IN_COMBAT ){
        AnimStart( 1 );
        AnimUnk48( obj, 0, 0 );
        AnimBegin();
    }
}

/*
    void animate_stand_reverse_obj( ObjectPtr obj )
*/
void ScrGame_AnimateStandReverseObj( Intp_t *scr )
{
    short type;
    Obj_t *obj;
    int ActionSource;
    Scpt_t *script;

    GETARGP( scr, type, obj, 0, "animate_stand_reverse_obj" );
    if( !obj ){
        ActionSource = ScptGetActionSource( scr );
        if( ScptPtr( ActionSource, &script ) == -1 ){
            ScrGameErrorMsg( scr, "animate_stand_reverse_obj", 2 );
            return;
        }
        ScptUnk140( scr );
    }
    if( !IN_COMBAT ){
        AnimStart( 1 );
        AnimUnk49( obj, 0, 0 );
        AnimBegin();
    }
}

/*
    void animate_move_obj_to_tile( ObjectPtr who, int tile, int speed )
*/
void ScrGame_AnimateMoveObjToTile( Intp_t *scr )
{
    Scpt_t *script;
    Obj_t *who;
    short type[ 3 ];
    int TileNo, Speed;

    GETARGI( scr, type[ 0 ], Speed, 0, "animate_move_obj_to_tile" );
    GETARGI( scr, type[ 1 ], TileNo, 0, "animate_move_obj_to_tile" );
    GETARGP( scr, type[ 2 ], who, 0, "animate_move_obj_to_tile" );
    if( who ){
        if( TileNo > -1 ){
            if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ){
                ScrGameErrorMsg( scr, "animate_move_obj_to_tile", 2 );
            } else if( CritterCanTalk( who ) && !IN_COMBAT ){
                if( Speed & 0x10 ){
                    AnimClear( who );
                    Speed &= ~0x10u;
                }
                AnimStart( 1 );
                if( Speed )
                    AnimObjRunToTile( who, TileNo, who->Elevation, -1, 0 );
                else
                    AnimObjMoveToTile( who, TileNo, who->Elevation, -1, 0 );
                AnimBegin();
            }
        }
    } else {
        ScrGameErrorMsg( scr, "animate_move_obj_to_tile", 1 );
    }
}

/*
    boolean tile_in_tile_rect( int tile1, int tile2, int tile3, int tile4, int tile )
*/
void ScrGame_TileInTileRect( Intp_t *scr )
{
    int i, tmp[ 10 ], val[ 5 ], InTile;
    short type[ 5 ];

    InTile = 0;
    for( i = 0; i < 5; i++){
	GETARGI( scr, type[ i ], val[ i ], i, "tile_in_tile_rect" );
        tmp[ i*2 + 1 ] = val[ i ] % 200;
        tmp[ i*2 + 0 ] = val[ i ] / 200;
    }
    if( tmp[0] <= tmp[8] && tmp[0] >= tmp[2] && tmp[1] >= tmp[9] && tmp[1] <= tmp[3] ) InTile = 1;
    RETINT( scr, InTile );
}

void ScrGame_Dummy( Intp_t *scr )
{
    /* Empty */
}

/*
    int tile_distance( int tile1, int tile2 )
*/
void ScrGame_TileDistance( Intp_t *scr )
{
    int i, Distance, val[ 2 ];
    short type[ 2 ];

    for( i = 0; i < 2; i++ ){
	GETARGI( scr, type[ i ], val[ i ], i, "tile_distance" );
    }
    if( val[0] == -1 || val[1] == -1 )
        Distance = 9999;
    else
        Distance = TileGetDistance( val[ 1 ], val[ 0 ] );
    RETINT( scr, Distance );
}

/*
    int tile_distance_objs( ObjectPtr obj1, ObjectPtr obj2 )
*/
void ScrGame_TileDistanceObjs( Intp_t *scr )
{
    int i, Distance;
    Obj_t *val[2];
    short type[2];

    for( i = 0; i < 2; i++ ){
	GETARGP( scr, type[ i ], val[ i ], i, "tile_distance_objs" );
    }
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
    int tile_num( ObjectPtr obj )
*/
void ScrGame_TileNuM( Intp_t *scr )
{
    int idx;
    Obj_t *obj;
    short type;

    idx = -1;
    GETARGP( scr, type, obj, 0, "tile_num" );
    if( obj )
        idx = obj->GridId;
    else
        ScrGameErrorMsg( scr, "tile_num", 1 );
    RETINT( scr, idx );
}

/*
    int tile_num_in_direction( int start_tile, int dir, int distance )
*/
void ScrGame_TileNumInDirection( Intp_t *scr )
{
    int i, val[3], num;
    short type[3];

    num = -1;
    for( i = 0; i < 3; i++ ){
	GETARGI( scr, type[ i ], val[ i ], i, "tile_num_in_direction" );
    }
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
    void pickup_obj( ObjectPtr obj )
*/
void ScrGame_PickupObj( Intp_t *scr )
{
    short type;
    Obj_t *obj;
    Scpt_t *script;

    GETARGP( scr, type, obj, 0, "pickup_obj" );
    if( obj ){
        if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ){
            ScrGameErrorMsg( scr, "pickup_obj", 2 );
        } else if( script->item ){
            ActionPickupItem( script->item, obj );
        } else {
            ScrGameErrorMsg( scr, "pickup_obj", 1 );
        }
    }
}

/*
    void drop_obj( ObjectPtr obj )
*/
void ScrGame_DropObj( Intp_t *scr )
{
    short type;
    Obj_t *obj;
    Scpt_t *script;

    GETARGP( scr, type, obj, 0, "drop_obj" );
    if( obj ){
        if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ){
            ScrGameErrorMsg( scr, "drop_obj", 1 );
        } else if( script->item ){
            UseDropObj( script->item, obj );
        } else {
            ScrGameErrorMsg( scr, "drop_obj", 2 );
        }
    }
}

/*
    void add_obj_to_inven( ObjectPtr who, ObjectPtr item )
*/
void ScrGame_AddObjToInv( Intp_t *scr )
{
    int i;
    short type[ 2 ];
    Obj_t *item[ 2 ];
    VidRect_t Area;

    for( i = 0; i < 2; i++ ){
	GETARGP( scr, type[ i ], item[ i ], i, "add_obj_to_inven" );
    }
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
    void rm_obj_from_inven( ObjectPtr who, ObjectPtr obj )
*/
void ScrGame_RmObjFromInv( Intp_t *scr )
{
    int i, k, flags;
    VidRect_t Area;
    Obj_t *obj[2];
    short type[2];

    flags = 0;
    k = 0;
    for( i = 0; i < 2; i++ ){    
	GETARGP( scr, type[ i ], obj[ i ], i, "rm_obj_from_inven" );
    }
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
    void wield_obj_critter( ObjectPtr who, ObjectPtr obj )
*/
void ScrGame_WieldObjCritter( Intp_t *scr )
{
    int i, v10, v14, v16;
    short type[2];
    Obj_t *obj[2], *v13, *ArmorObj;

    v16 = 0;
    ArmorObj = 0;
    v13 = 0;
    v14 = 1;
    for( i = 0; i < 2; i++ ){
	GETARGP( scr, type[ i ], obj[ i ], i, "wield_obj_critter" );
    }
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
    void use_obj( ObjectPtr obj )
*/
void ScrGame_UseObj( Intp_t *scr )
{
    Scpt_t *script;
    Obj_t *obj, *item;
    short type;

    GETARGP( scr, type, obj, 0, "use_obj" );
    if( !obj ) goto err;
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 ) goto err;
    if( script->item ){
        item = ScptUnk140( scr );
        if( OBJTYPE( item->Pid ) == 1 )
            ActionUseOnScenery( script->item, obj );
        else
            UseObject( item, obj, obj );
    } else {
err:
        ScrGameErrorMsg( scr, "use_obj", 1 );
    }
}

/*
    boolean obj_can_see_obj( ObjectPtr src, ObjectPtr dst )
*/
void ScrGame_ObjCanSeeObj( Intp_t *scr )
{
    int i,n;
    short type[ 2 ];
    Obj_t *obj[ 2 ], *pObj;    

    pObj = NULL;
    n = 0;
    for( i = 0; i < 2; i++ ){
	GETARGP( scr, type[ i ], obj[ i ], i, "obj_can_see_obj" );
    }
    if( obj[0] && obj[1] ){
        if ( obj[0]->GridId != -1 ){
            if( obj[0] == gObjDude ) CritterUsingSkill( 0 );
//            FeatGetVal( obj[1], 1 );
            if( AiObjCanHearObj( obj[1], obj[0] ) ){
                AnimUnk06( obj[1], obj[1]->GridId, obj[0]->GridId, 0, &pObj, 16 );
                if( obj[0] == pObj ) n = 1;
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
    Scpt01_t tmp;
    Obj_t *who, *p;
    int target_results, attacker_results, max_damage, min_damage, bonus, num_attacks, called_shot;
    short type[ 8 ];

    GETARGI( scr, type[ 0 ], target_results, 0, "attack" );
    GETARGI( scr, type[ 1 ], attacker_results, 1, "attack" );
    GETARGI( scr, type[ 2 ], max_damage, 2, "attack" );
    GETARGI( scr, type[ 3 ], min_damage, 3, "attack" );
    GETARGI( scr, type[ 4 ], bonus, 4, "attack" );
    GETARGI( scr, type[ 5 ], num_attacks, 5, "attack" );
    GETARGI( scr, type[ 6 ], called_shot, 6, "attack" );
    GETARGP( scr, type[ 7 ], who, 7, "attack" );

    if( !who ){
        ScrGameErrorMsg( scr, "attack", 1 );
        return;
    }
    scr->Flags |= 0x20;
    if( !(p = ScptUnk140( scr )) ){
        scr->Flags &= ~0x20;
        return;
    }
    if( !CritterCanTalk( p ) || ( p->Flags & 0x01 ) ){
        eprintf( "\n   But is already Inactive (Dead/Stunned/Invisible)" );
        scr->Flags &= ~0x20;
        return;
    }
    if( !CritterCanTalk( who ) || ( who->Flags & 0x01) ){
        eprintf( "\n   But target is already dead or invisible" );
        scr->Flags &= ~0x20;
        return;
    }
    if( who->Critter.State.Reaction & 0x04 ){
        eprintf( "\n   But target is AFRAID" );
        scr->Flags &= ~0x20;
        return;
    }
    if( !GdialogUnk01() ){
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
        scr->Flags &= ~0x20;
    }
}

/*
    void start_gdialog( int msgFileNum, ObjectPtr who, int mood, int headNum, int backgroundIdx ) - to start a dialogue
*/
void ScrGame_StartGdialog( Intp_t *scr )
{
    Obj_t *who;
    Proto_t *proto;
    int n, BackGroundIdx, HeadNum, Mood, MsgFileNum;
    short type[ 5 ];
    
    GETARGI( scr, type[ 0 ], BackGroundIdx, 0, "start_gdialog" );
    GETARGI( scr, type[ 1 ], HeadNum, 1, "start_gdialog" );
    GETARGI( scr, type[ 2 ], Mood, 2, "start_gdialog" );
    GETARGP( scr, type[ 3 ], who, 3, "start_gdialog" );
    GETARGI( scr, type[ 4 ], MsgFileNum, 4, "start_gdialog" );

    if( IN_COMBAT ) return;
    if( !who ){
        ScrGameErrorMsg( scr, "start_gdialog", 1 );
        return;
    }
    gDlgUnk46 = -1;
    if( OBJTYPE( who->Pid ) == 1 ){
        if( ProtoGetObj( who->Pid, &proto ) == -1 ) return;
        if( HeadNum != -1 ) gDlgUnk46 = ArtMakeId( 8, HeadNum, 0, 0, 0 );
    } else {
	if( HeadNum != -1 ) gDlgUnk46 = ArtMakeId( 8, HeadNum, 0, 0, 0 );
    }
    GdialogUnk07( BackGroundIdx );
    gScrGameMood = Mood;
    if( gDlgUnk46 != -1 ){
        if( (n = EvQeUnk20( EvQeUnk23( gDlgUnk44 ) )) ){
            if( n <= 1 ){
                gScrGameMood = 4;
            } else if( n == 2 ){
                gScrGameMood = 1;
            }
        } else {
            gScrGameMood = 7;
        }
    }
    gDlgUnk47 = ScptGetActionSource( scr );
    gDlgUnk44 = ScptUnk140( scr );
    GdialogUnk05( gDlgUnk46, gScrGameMood );
}

/*
    ?
*/
void ScrGame_Unk01( Intp_t *scr )
{
    if( GdialogUnk06() == -1 ) return;
    gDlgUnk44 = 0;
    gDlgUnk47 = -1;
}

/*
    void dialogue_reaction (int mood) - animates an emotional reaction in the dialogue
*/
void ScrGame_DialogueReaction( Intp_t *scr )
{
    short type;
    int val;

    GETARGI( scr, type, val, 0, "dialogue_reaction" );
    gScrGameMood = val;
    GdialogReaction( val );
}

void ScrGameUnk04( int x0, int y0, int x1, int y1, int a5 )
{
    ScrGameUnk05( x0, y0, x1, y1, a5 );
}

void ScrGameUnk05( int x0, int y0, int x1, int y1, int a5 )
{
/*
    int v5; // ebp
    int v6; // eax
    int rt; // edi
    int v8; // esi
    int v9; // ecx
    Obj_t *i; // ebx
    int GridId; // eax
    int v12; // ecx
    int bm; // ebx
    int v14; // edx
    VidRect_t v15; // [esp-8h] [ebp-3Ch] BYREF
    char v16[8]; // [esp+8h] [ebp-2Ch] BYREF
    VidRect_t Area2; // [esp+10h] [ebp-24h]
    int a2; // [esp+20h] [ebp-14h]
    int v19; // [esp+24h] [ebp-10h]

    Area2.rt = x0;
    Area2.bm = y0;
    v5 = x1;
    a2 = y1;
    memset( &v15, 0, sizeof(v15) );
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
                ObjGetRadiusArea( i, &v16 );
                if( v12 )
            	    i->Flags &= ~0x01;
                else
                    i->Flags |= 0x01;
                RegionExpand( &v15, &v16, &v15 );
            }
        }
        TileUpdateArea( &v15, ++rt );
    }
*/
}

/*
    => another feature of working with a variety of options for the game, is different from the number of arguments metarule
    int metarule3(
3	    int meta3_switch, 
2	    int meta3_par1, 
1	    int meta3_par2, 
0	    int meta3_par3
	) 
*/
void ScrGame_Metarule3( Intp_t *scr )
{
/*
    Obj_t *i;
    Obj_t *obj;
    Obj_t *v22;
    Obj_t *v23;
    VidRect_t Area;
    int KillStat;
    int v13;
    int Id;
    int arg0;
    int arg1;
    int v24;
    int result;
    int meat3_switch;
    short type[4];

    v23 = 0;
    GETARGI( scr, type[ 0 ], arg0, 0, "metarule3" );
    GETARGI( scr, type[ 1 ], arg1, 1, "metarule3" );
    GETARGI( scr, type[ 2 ], obj, 2, "metarule3" );
    GETARGI( scr, type[ 3 ], meta3_switch, 3, "metarule3" );
    switch( meta3_switch ){
	case 'd':
	    ScptUnk131( (int)obj, arg1 );
	    EvQeRun( 3, (void *)ScptUnk130 );
	    result = 0;
	    break;
        case 'e':
            result = WmUnk70( (int)obj, arg1, arg0 );
            break;
        case 'g':
            result = CritterGetKillStat( (int)obj );
            break;
        case 'h':
            result = WmUnk45( (int)obj, arg0, arg1 );
            break;
        case 'i':
            if( !WmUnk71( (int)obj, arg1, (int *)&v22 ) ) v23 = v22;
            result = v23;
            break;
        case 'j':
            if( !arg0 ) v24 = 1;
            for( i = ObjGetFirst( arg1, (int)obj ); i; i = ObjGetNext() ){
                if( OBJTYPE( i->Pid ) == 1 && v24 ) break;
                if( i == (Obj_t *)arg0 ) v24 = 1;
            }
            RETPTR( scr, i );
            return;
        case 'k':
            Id = ArtMakeId((obj->ImgId & 0xF000000) >> 24, arg1, (obj->ImgId & 0xFF0000u) >> 16, (obj->ImgId & 0xF000) >> 12, (obj->ImgId & 0x70000000) >> 28);
            ObjSetShape(obj, Id, &Area);
            TileUpdateArea( &Area, gCurrentMapLvl );
            result = (int)v23;
            break;
        case 'l':
    	    result = TileSetCenter( (int)obj, 1 );
            break;
        case 'm':
    	    result = AiGetChemUse( obj );
            break;
        case 'n':
            result = ( WmCarNoFuel() == 1 );
    	    break;
        case 'o':
    	    result = MapGetAreaByEntrance();
    	    break;
        default: return;                
    }
    RETINT( scr, result );
*/
}

/*
    void set_map_music( int MapId, string FileName )
*/
void ScrGame_SetMapMusic( Intp_t *scr )
{
    char *s;
    int val[ 2 ];
    short type[ 2 ];

    GETARGS( scr, type[ 0 ], val[ 0 ], 0, "set_map_music" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "set_map_music" );

    s = IntpGetArg( scr, type[0] >> 8, val[ 0 ] );
    eprintf( "\nset_map_music: %d, %s", val[ 1 ], s );
    WmSetMapMusic( val[ 1 ], s );
}

/*
    void set_obj_visibility ( ObjectPtr obj, boolean visibility ) - sets the visibility flag of the object
*/
void ScrGame_SetObjVisibility( Intp_t *scr )
{
    VidRect_t Area;
    Obj_t *obj;
    int val;
    short type[ 2 ];

    GETARGI( scr, type[ 1 ], val, 0, "set_obj_visibility" );
    GETARGP( scr, type[ 1 ], obj, 1, "set_obj_visibility" );
    if( !obj ){
        ScrGameErrorMsg( scr, "set_obj_visibility", 1 );
        return;
    }
    if( LsgGetUnk02() ){
        eprintf( "\nError: attempt to set_obj_visibility in load/save-game: %s!", scr->FileName );
        return;
    }
    if( val ){
        if( !(obj->Flags & 0x01) ){
            if( IN_COMBAT ){
                ObjUnk35( obj, 0 );
                ObjGetRadius( obj, 0 );
            }
            if( ObjUnk33( obj, &Area ) != -1 ){
                if( OBJTYPE( obj->Pid ) == TYPE_CRIT ) obj->Flags |= 0x10;
                TileUpdateArea( &Area, obj->Elevation );
            }
        }
    } else {
        if( obj->Flags & 0x01 ){
            if( OBJTYPE( obj->Pid ) == TYPE_CRIT ) obj->Flags &= ~0x10;
            if( ObjUnk32( obj, &Area ) != -1 ) TileUpdateArea( &Area, obj->Elevation );            
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
    int MapIdxByFileName, val1, val2;
    short type1, type2;
    char *Arg;
    Map01_t tmp;

    GETARGI( scr, type1, val1, 0, "load_map" );
    type2 = IntpPopwA(scr);
    val2 = IntpPopiA( scr );
    if( type2 == 0x9801 ) IntpStringDeRef( scr, 0x9801, val2 );

    MapIdxByFileName = -1;
    Arg = 0;
    if( type2 != SCR_INT ){
        if( (type2 & 0xF7FF) == SCR_STRING )
            Arg = IntpGetArg( scr, type2 >> 8, val2 );
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
        tmp.PosY = -1;
        tmp.Frame = -1;
        tmp.Orientation = -1;
        MapSetPos( &tmp );
    }
}

/*
    void wm_area_set_pos (int areaIdx, int xPos, int yPos) - set the world coordinates for the specified city
*/
void ScrGame_WmAreaSetPos( Intp_t *scr )
{
    int i, val[ 3 ];
    short type[ 16 ];

    for( i = 0; i < 3; i++ ){
	GETARGI( scr, type[ i ], val[ i ], i, "wm_area_set_pos" );
    }
    if( WmSetPos( val[ 2 ], val[ 1 ], val[ 0 ] ) != -1 ){
        ScrGameErrorMsg( scr, "wm_area_set_pos", 3 );
        eprintf( "Invalid Parameter!" );
    }
}

void ScrGame_SetExitGrids( Intp_t *scr )
{
    int i;
    Obj_t *p;
    int val[5];
    short type[5];

    for( i = 0; i < 5; i++ ){
	GETARGI( scr, type[ i ], val[ i ], i, "set_exit_grids" );
    }
    for( p = ObjGetVisibleObjectFirst( val[4] ); p; p = ObjGetVisibleObjectNext() ){
        if( p->Pid >= 0x5000010 && p->Pid <= 0x5000017 ){
            p->Grid.DestMapId = val[3];
            p->Grid.DestStartPos = val[1];
            p->Grid.DestMapElev = val[2];
        }
    }
}

void ScrGame_AnimBusy( Intp_t *scr )
{
    int busy = 0;
    Obj_t *obj;
    short type;

    GETARGP( scr, type, obj, 0, "anim_busy" );
    if( obj )
        busy = AnimUnk39( obj );
    else
        ScrGameErrorMsg( scr, "anim_busy", 1 );
    RETINT( scr, busy );
}

void ScrGame_CritterHeal( Intp_t *scr )
{
    int result, val;
    Obj_t *obj;
    short type[ 2 ];

    GETARGI( scr, type[ 0 ], val, 0, "critter_heal" );
    GETARGP( scr, type[ 1 ], obj, 1, "critter_heal" );
    result = CritterHeal( obj, val );
    if( gObjDude == obj ) IfaceRenderHP( 1 );
    RETINT( scr, result );
}

void ScrGame_SetLightLevel( Intp_t *scr)
{
/*
    short type;
    int val, light,lt1, lt2;

    lt1 = gScrGameLightLvl[1];
    lt2 = gScrGameLightLvl[2];

    GETARGI( scr, type, val, 0, "set_light_level" );
    if( val >= 50 ){
        if( val == 50 ){
            ItemMapSetLight( lt1, 1 );
            return;
        }
        light = lt1 + val * (lt2 - lt1) / 100;
    } else {
        light = val * (lt1 - 16384) / 100u + 16384;
    }
    ItemMapSetLight( light, 1 );
*/
}

void ScrGame_Unk05( Intp_t *scr )
{
    int GameDekaSeconds;

    GameDekaSeconds = ScptGetGameDekaSeconds();
    RETINT( scr, GameDekaSeconds );
}

void ScrGame_GetGTime( Intp_t *scr )
{
    RETINT( scr, ScptGetGameDekaSeconds() / 10 );
}

void ScrGame_Elevation( Intp_t *scr )
{
    int Elevation = 0;
    Obj_t *obj;
    short type;

    GETARGP( scr, type, obj, 1, "elevation" );
    if ( obj )
        Elevation = obj->Elevation;
    else
        ScrGameErrorMsg( scr, "elevation", 1 );
    RETINT( scr, Elevation );
}

void ScrGame_KillCritter( Intp_t *scr )
{
    int val, flg = 0;
    Obj_t *obj;
    short type[ 2 ];

    GETARGI( scr, type[ 0 ], val, 0, "kill_critter" );
    GETARGP( scr, type[ 1 ], obj, 1, "kill_critter" );
    if( obj ){
        if( LsgGetUnk02() ){
            eprintf( "\nError: attempt to destroy critter in load/save-game: %s!", scr->FileName );
        } else {
            scr->Flags |= 0x20;
            if( ScptUnk140( scr ) == obj ) flg = 1;
            AnimClear( obj );
            CombatUnk79( obj );
            CritterKill( obj, val, 1 );
            scr->Flags &= ~0x20u;
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

void ScrGame_KillCritterType( Intp_t *scr )
{
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
    if( LsgGetUnk02() ){
        eprintf( "\nError: attempt to destroy critter in load/save-game: %s!", scr->FileName );
        return;
    }
    scr->Flags |= 0x20;
    for( p = ObjGetObjectFirst(); p; p = ObjGetObjectNext() ){
        while( (p->ImgId & 0xFF0000) >> 16 >= 48 ){
            p = ObjGetObjectNext();
            if( !p ) goto LABEL_27;
        }
        if( (p->Flags & 1) == 0 && val[1] == p->Pid && !CritterIsDead( p ) ){
            if( v11 == v22 || v23 > 200 ){
                ScrGameErrorMsg( scr, "kill_critter_type", 3 );
                eprintf( " Infinite loop destroying critters!" );
                scr->Flags &= ~0x20;
                return;
            }
            AnimClear( v11 );
            if( val[0] ){
                CombatUnk79( v12 );
                if( val[0] == 1 ){
                    CritterKill( v13, ScrGamUnk06(v13, dword_518ED0[ v2++ ], 1), 1 );
                    if( v2 >= 11 ) v2 = 0;
                } else {
                    CritterKill( v13, 48, 1 );
                }
            } else {
                AnimClear( v12 );
                ObjDestroy( v18, &v17 );
                TileUpdateArea( &v19, gCurrentMapLvl );
            }
            v22 = v16;
            v23++;
            ObjGetObjectFirst();
            gMap.Time = ScptGetGameDekaSeconds();
        }
    }
LABEL_27:
    scr->Flags &= ~0x20u;
*/
}

void ScrGame_CritterDamage( Intp_t *scr )
{
    Obj_t *v10, *v16;
    int val, v15;
    short type[3];

    GETARGI( scr, type[ 0 ], val, 0, "critter_damage" );
    GETARGI( scr, type[ 1 ], v15, 0, "critter_damage" );
    GETARGP( scr, type[ 2 ], v16, 1, "critter_damage" );

    if( v16 ){
        if( OBJTYPE( v16->Pid ) == TYPE_CRIT ){
            v10 = ScptUnk140( scr );
            if( v16->Critter.State.WhoHitMe == -1 ) v16->Critter.State.WhoHitMe = 0;
            ActionUnk05( v16->GridId, v16->Elevation, v15, v15, val & 0xFCFF, (val & 0x200) == 0, val & 0x100 );
            scr->Flags &= ~0x20;
            if( v10 == v16 ) scr->Flags |= 0x1000000;
        } else {
            ScrGameErrorMsg( scr, "critter_damage", 3 );
            eprintf( " Can't call on non-critters!" );
        }
    } else {
        ScrGameErrorMsg( scr, "critter_damage", 1 );
    }
}

void ScrGame_OpAddTimerEvent( Intp_t *scr )
{
    int val[ 2 ];
    Obj_t *obj;
    short type[3];

    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "add_timer_event" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "add_timer_event" );
    GETARGP( scr, type[ 2 ], obj, 2, "add_timer_event" );

    if( obj )
        ScptAddTimerEvent( obj->ScrId, val[ 1 ], val[ 0 ] );
    else
        ScrGameEprintf( "\nScript Error: %s: op_add_timer_event: pobj is NULL!", scr->FileName );
}

void ScrGame_RmTimeEvent( Intp_t *scr )
{
    short type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "rm_timer_event" );
    if( obj )
        EvQeDelA( obj );
    else
        ScrGameEprintf( "\nScript Error: %s: op_add_timer_event: pobj is NULL!", scr->FileName );
}

void ScrGame_GameTicks( Intp_t *scr )
{
    int val;
    short type;

    GETARGI( scr, type, val, 0, "game_ticks" );
    if( val < 0 ) val = 0;
    RETINT( scr, 10 * val );
}

void ScrGame_HasTrait( Intp_t *scr )
{
    Obj_t *obj;
    int AIpackNb, perk, v14;
    short type[3];

    AIpackNb = 0;
    GETARGI( scr, type[ 0 ], perk, 0, "has_trait" );
    GETARGP( scr, type[ 1 ], obj, 1, "has_trait" );
    GETARGI( scr, type[ 2 ], v14, 2, "has_trait" );
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

void ScrGame_ObjCanHearObj( Intp_t *scr )
{
    Obj_t *obj[ 2 ];
    short type[ 2 ];
    int CanHear;

    CanHear = 0;
    GETARGP( scr, type[ 0 ], obj[ 0 ], 0, "obj_can_hear_obj" );
    GETARGP( scr, type[ 1 ], obj[ 1 ], 1, "obj_can_hear_obj" );

    if( !obj[0] || !obj[ 1 ] ) {
	if( ( obj[0]->GridId != -1 ) && AiObjCanHearObj( obj[1], obj[0] ) ) CanHear = 1;
    }
    RETINT( scr, CanHear );
}

void ScrGame_GetGameTime( Intp_t *scr )
{
    RETINT( scr, ScptGetGameTime() );
}

void ScrGame_FixedParam( Intp_t *scr )
{
    Scpt_t *script;
    int param = 0;

    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "fixed_param", 2 );
    else
        param = script->ArgVal;
    RETINT( scr, param );
}

void ScrGame_TileIsVisible( Intp_t *scr )
{
    int Visibility, val;
    short type;

    Visibility = 0;
    GETARGI( scr, type, val, 0, "tile_is_visible" );
    if( ScrGameIsTileVisible( val ) ) Visibility = 1;
    RETINT( scr, Visibility );        
}

void ScrGame_Unk06( Intp_t *scr )
{
    Scpt_t *script;
    Obj_t *obj;

    if( ScptPtr( ScptGetActionSource( scr ), &script ) != -1 ){
        obj = ScptUnk140( scr );
        if( (OBJTYPE( obj->Pid ) != TYPE_CRIT || CritterCanTalk( obj )) && !IN_COMBAT && GlobVarUnk02( 4 ) != -1 ) gDlgUnk44 = ScptUnk140( scr );
    }
}

void ScrGame_ActionBeingUsed( Intp_t *scr )
{
    int used;
    Scpt_t *script;

    used = -1;
    if( ScptPtr( ScptGetActionSource( scr ), &script ) == -1 )
        ScrGameErrorMsg( scr, "action_being_used", 2 );
    else
        used = script->i17;
    RETINT( scr, used );
}

void ScrGame_CritterState( Intp_t *scr )
{
    int state;
    Obj_t *obj;
    int v10, v9;
    short type;

    state = 1;
    GETARGP( scr, type, obj, 0, "critter_state" );
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

void ScrGame_GameTimeAdvance( Intp_t *scr )
{
    short type;
    int val, time, i;

    GETARGI( scr, type, val, 0, "game_time_advance" );
    time = val % 864000;
    for( i = 0; i < (val / 864000); i++ ){
        ScptTimeAdvance( 864000 );
        EvQeTrig();
    }
    ScptTimeAdvance( time );
    EvQeTrig();
}

void ScrGame_RadiationInc( Intp_t *scr )
{
    int val;
    Obj_t *dude;
    short type[ 2 ];

    GETARGI( scr, type[ 0 ], val, 0, "radiation_inc" );
    GETARGP( scr, type[ 1 ], dude, 1, "radiation_inc" );
    if( dude )
        CritterRadInc( dude, val );
    else
        ScrGameErrorMsg( scr, "radiation_inc", 1 );
}

void ScrGame_RadiationDec( Intp_t *scr )
{
    int val;
    Obj_t *dude;
    short type[ 2 ];
    
    GETARGI( scr, type[ 0 ], val, 0, "radiation_dec" );
    GETARGP( scr, type[ 1 ], dude, 1, "radiation_dec" );
    if( dude ){
        CritterRadInc( dude, ( CritterRadiated( dude ) >= 0 ) ? -val : 0 );
    } else {
        ScrGameErrorMsg( scr, "radiation_dec", 1 );
    }
}

void ScrGame_CritterAttemptPlacement( Intp_t *scr )
{
    int val, GridIdx;
    Obj_t *obj;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], val, 0, "critter_attempt_placement" );
    GETARGI( scr, type[ 1 ], GridIdx, 1, "critter_attempt_placement" );
    GETARGP( scr, type[ 2 ], obj, 2, "critter_attempt_placement" );
    if( !obj ){
        ScrGameErrorMsg( scr, "critter_attempt_placement", 1 );
        return;
    }
    if( val != obj->Elevation && ( OBJTYPE( obj->Pid ) == TYPE_CRIT ) ) CombatUnk79( obj );
    ObjMoveToTile( obj, 0, val, 0 );
    RETINT( scr, UseUnk46( obj, GridIdx, val, 1 ) );
}

void ScrGame_ObjPid( Intp_t *scr )
{
    Obj_t *obj;
    short type;
    int pid = -1;

    GETARGP( scr, type, obj, 0, "obj_pid" );
    if( obj )
        pid = obj->Pid;
    else
        ScrGameErrorMsg( scr, "obj_pid", 1 );
    RETINT( scr, pid );
}

void ScrGame_GetMapId( Intp_t *scr )
{
    IntpPushiA( scr, MapGetCurrentMapId() );
    IntpPushwA( scr, 0xC001 );
}

void ScrGame_OpCritterAddTrait( Intp_t *scr )
{
    int val, prk, v14, err = -1;
    Obj_t *obj;
    short type[ 4 ];

    GETARGI( scr, type[ 0 ], val, 0, "critter_add_trait" );
    GETARGI( scr, type[ 1 ], prk, 1, "critter_add_trait" );
    GETARGI( scr, type[ 2 ], v14, 2, "critter_add_trait" );
    GETARGP( scr, type[ 3 ], obj, 3, "critter_add_trait" );
    if( obj ){
        if( OBJTYPE( obj->Pid ) == TYPE_CRIT ){
            if( v14 ){
                if ( v14 == 1 ){
                    if( prk == 5 ){
                        AiUnk54( obj, val );
                    } else if( prk == 6 && !PartyMembRdy( obj ) && val != obj->Critter.State.GroupId && !LsgGetUnk02() ){
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

void ScrGame_CritterRmTrait( Intp_t *scr )
{
    int val0, val1, val2, err;
    Obj_t *obj;
    short type[4];

    err = -1;
    GETARGI( scr, type[ 0 ], val0, 0, "critter_rm_trait" );
    GETARGI( scr, type[ 1 ], val1, 1, "critter_rm_trait" );
    GETARGI( scr, type[ 2 ], val2, 2, "critter_rm_trait" );
    GETARGP( scr, type[ 3 ], obj, 3, "critter_rm_trait" );
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

void ScrGame_ProtoData( Intp_t *scr )
{
    int *p;
    int MembId[ 2 ];
    short type[ 2 ];

    GETARGI( scr, type[ 0 ], MembId[0], 0, "proto_data" );
    GETARGI( scr, type[ 1 ], MembId[1], 1, "proto_data" );
    switch( ProtoDataMember( MembId[1], MembId[0], (void **)&p ) ){
	case 1: RETINT( scr, *p );return;
//	case 2: RETFTR( scr, IntpDbgStr( scr, p, p ) ); return;
	default: RETINT( scr, 0 ); return;
    }
}

void ScrGame_MessageStr( Intp_t *scr )
{
    unsigned int line, Idx;
    char *msg;
    short type[ 2 ];

    GETARGI( scr, type[ 0 ], line, 0, "message_str" );
    GETARGI( scr, type[ 1 ], Idx, 1, "message_str" );
    if( Idx ){
        msg = ScptGetDialog( Idx, line, 1 );
        if( !msg ){
            eprintf( "\nError: No message file EXISTS!: index %d, line %d", Idx, line );
            msg = "Error";
        }
    } else {
        msg = "Error";
    }
    RETFTR( scr, IntpDbgStr( scr, msg, line ) );
}

void ScrGame_CritterInvenObj( Intp_t *scr )
{
    int val;
    Obj_t *obj;
    short type[ 2 ];

    GETARGI( scr, type[ 0 ], val, 0, "critter_inven_obj" );
    GETARGP( scr, type[ 1 ], obj, 1, "critter_inven_obj" );
    if( !obj ){
        ScrGameErrorMsg(scr, "critter_inven_obj", 1);
    } else {
	if( OBJTYPE( obj->Pid ) == TYPE_CRIT ){
    	    switch( val ){
        	case -2: RETINT( scr, obj->Critter.Box.Cnt ); break;
        	case 0:  RETPTR( scr, InvGetArmorObj( obj ) ); break;
        	case 1:  if( obj != gObjDude || IfaceGetSelectedHand() ){ RETPTR( scr, InvGetRHandObj( obj ) ); } break;
        	case 2:  if( obj != gObjDude || !IfaceGetSelectedHand() ){ RETPTR( scr, InvGetLHandObj( obj ) ); } break;
        	default: ScrGameEprintf( "\nscript error: %s: Error in critter_inven_obj -- wrong type!", scr->FileName ); break;
    	    }
	} else {
	    ScrGameErrorMsg( scr, "critter_inven_obj", 3 );
	    eprintf( "  Not a critter!" );
	}
    }
    RETINT( scr, 0 );
}

void ScrGame_ObjSetLightLevel( Intp_t *scr )
{
    VidRect_t Area;
    Obj_t *obj;
    int rad, inten;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], rad, 0, "obj_set_light_level" );
    GETARGI( scr, type[ 1 ], inten, 1, "obj_set_light_level" );
    GETARGP( scr, type[ 2 ], obj, 2, "obj_set_light_level" );
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

void ScrGame_Unk07( Intp_t *scr )
{
    ScptUnk118();
}

void ScrGame_InvenCmds( Intp_t *scr )
{
    int val0, val1;
    Obj_t *obj, *item = NULL;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], val0, 0, "inven_cmds" );
    GETARGI( scr, type[ 1 ], val1, 1, "inven_cmds" );
    GETARGP( scr, type[ 2 ], obj, 2, "inven_cmds" );
    if( obj ){
        if( val1 == 13 ) item = InvUnk27( obj, val0 );
    } else {
        ScrGameErrorMsg( scr, "anim", 1 );
    }
    RETPTR( scr, item );
}

void ScrGame_FloatMsg( Intp_t *scr )
{
    VidRect_t Area;
    Obj_t *obj;
    int colorFg,Idx[2],Font,colorBg, i;
    static int stat_zm1 = 1;
    short type[3];
    char *Text;

    colorFg = gPalColorCubeRGB[31][31][11];
    Text = 0;
    Font = 101;
    colorBg = gPalColorCubeRGB[0][0][0];
    for( i = 0; i < 2; i++ ){
        type[ i ] = IntpPopwA( scr );
        Idx[ i ] = IntpPopiA( scr );
        if( type[ i ] == 0x9801 ) IntpStringDeRef( scr, type[ i ], Idx[ i ] );
        if( i == 1 ){
            if( (type[ i ] & 0xF7FF) == 0x9001 ) Text = IntpGetArg( scr, type[ i ] >> 8, Idx[ i ]);
        } else {
            if( (type[ i ] & 0xF7FF) != 0xC001 ) IntpError("script error: %s: invalid arg %d to float_msg", scr->FileName, i );
        }
    }
    GETARGP( scr, type[ 2 ], obj, 2, "float_msg" );
    if( !obj ){
        ScrGameErrorMsg( scr, "float_msg", 1 );
        return;
    }
    if( !Text || *Text == '\0' ){
        TextUnk10( obj );
        TileUpdate();
    }
    if( gCurrentMapLvl != obj->Elevation ) return;
    if( Idx[0] == -1 ){
        Idx[0] = stat_zm1 + 1;
        if( (unsigned int)(stat_zm1 + 1) > 12 ) Idx[0] = 1;
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
    if( TextObjCreate( obj, Text, Font, colorFg, colorBg, &Area) != -1 ) TileUpdateArea( &Area, obj->Elevation );
}

void ScrGame_MetaRule( Intp_t *scr ) 
{
/*
    int v2; // ebp
    int v3; // edi
    __int16 v4; // ax
    int v5; // ecx
    Obj_t *v6; // eax
    int v7; // ecx
    __int16 v8; // dx
    unsigned __int16 v9; // ax
    Intp_t *v10; // eax
    int Unk02; // edx
    Obj_t *v12; // eax
    unsigned int v13; // edx
    int v14; // ecx
    int v15; // edx
    int v16; // edi
    int v17; // ecx
    Obj_t *LHandObj; // eax
    Obj_t *v19; // edx
    Obj_t *v20; // edi
    Obj_t *obj; // [esp+0h] [ebp-38h]
    int sel; // [esp+4h] [ebp-34h]
    int EntryValue; // [esp+8h] [ebp-30h] BYREF
    __int16 type[2]; // [esp+Ch] [ebp-2Ch]
    Proto_t *proto; // [esp+10h] [ebp-28h] BYREF
    Proto_t *pObj; // [esp+14h] [ebp-24h] BYREF
    Proto_t *v27; // [esp+18h] [ebp-20h] BYREF
    int WeaponBase; // [esp+1Ch] [ebp-1Ch]

    v2 = 0;
    v3 = 0;
    WeaponBase = 0;
    do
    {
        v4 = IntpPopwA(scr);
        *(__int16 *)((char *)type + v5) = v4;
        v6 = (Obj_t *)IntpPopiA(scr);
        v8 = *(__int16 *)((char *)type + v7);
        *(Obj_t **)((char *)&obj + v3) = v6;
        if ( v8 == (__int16)0x9801 )
            IntpStringDeRef(scr, *(__int16 *)((char *)type + v7), (int)v6);
        v9 = *(__int16 *)((char *)type + v7);
        HIBYTE(v9) &= ~8u;
        if ( v9 != 0xC001 )
            IntpError("script error: %s: invalid arg %d to metarule", scr->FileName, v2);
        ++v2;
        v3 += 4;
    }
    while ( v2 < 2 );
    if ( sel != 13 )
    {
        switch ( sel )
        {
            case 14:
                Unk02 = (gMap.MapFlags & 1) == 0;
LABEL_76:
                v10 = scr;
                goto LABEL_77;
            case 15:
                v12 = ScptUnk140(scr);
                ScptRequestElevator((Scpt_t *)v12, v13);
                v10 = scr;
                Unk02 = 0;
                goto LABEL_77;
            case 16:
                Unk02 = PartyUnk12();
                goto LABEL_76;
            case 17:
                Unk02 = WmLocKarma((int)obj);
                goto LABEL_76;
            case 18:
                Unk02 = EvQeEnqueued(&obj->TimeEv, 0);
                goto LABEL_76;
            case 19:
                Unk02 = WmUnk03((int)obj);
                goto LABEL_76;
            case 22:
                Unk02 = LsgGetUnk02();
                goto LABEL_76;
            case 30:
                Unk02 = WmUnk90();
                goto LABEL_76;
            case 31:
                Unk02 = WmUnk91();
                goto LABEL_76;
            case 32:
                Unk02 = WmCarRefuel((int)obj);
                goto LABEL_76;
            case 40:
                Unk02 = SkillIsSpecial((int)obj);
                goto LABEL_76;
            case 42:
                v14 = Item18(obj, obj->GridId);
                if ( gObjDude == obj )
                {
                    IfaceHandSlotUpdate(0, -1, -1);
                    IfaceResetAC(0);
                }
                Unk02 = v14;
                goto LABEL_76;
            case 43:
                v15 = 1;
                if ( obj == gObjDude && !IfaceGetSelectedHand() )
                    v15 = 0;
                v16 = InvUnk30(obj, v15, 0);
                if ( obj == gObjDude )
                {
                    v17 = GameIfaceStat() == 0;
                    IfaceHandSlotUpdate(v17, -1, -1);
                }
                else
                {
                    LHandObj = InvGetLHandObj(obj);
                    if ( ItemGetObjType(LHandObj) == 3 )
                        HIBYTE(v19->Flags) &= ~1u;
                }
                Unk02 = v16;
                goto LABEL_76;
            case 44:
                WmGetWorldPos(&EntryValue, 0);
                Unk02 = EntryValue;
                goto LABEL_76;
            case 45:
                WmGetWorldPos(0, &EntryValue);
                Unk02 = EntryValue;
                goto LABEL_76;
            case 46:
                if ( WmGetCurrentLocation(&EntryValue) == -1 )
                    eprintf("\nIntextra: Error: metarule: current_town");
                Unk02 = EntryValue;
                goto LABEL_76;
            case 47:
                CfgGetInteger(&gConfiguration, "preferences", "language_filter", &EntryValue);
                Unk02 = EntryValue;
                goto LABEL_76;
            case 48:
                CfgGetInteger(&gConfiguration, "preferences", "violence_level", &EntryValue);
                Unk02 = EntryValue;
                goto LABEL_76;
            case 49:
                v20 = obj;
                if ( HIBYTE(obj->Pid) )
                {
                    if ( ArtMakeId(5, 10, 0, 0, 0) == v20->ImgId )
                    {
                        WeaponBase = 6;
                        break;
                    }
                }
                else if ( ItemGetObjType(obj) == 3 )
                {
                    WeaponBase = (int)ItemGetWeaponBase(0, v20);
                    break;
                }
                ScrGameErrorMsg(scr, "metarule:w_damage_type", 3);
                eprintf("Not a weapon!");
                break;
            case 50:
                if ( OBJTYPE( obj->Pid ) == TYPE_CRIT ){
                    ProtoGetObj(obj->Pid, &proto);
                    proto = (Proto_t *)((char *)proto + 32);
                    if( (proto->Pid & 2) != 0 ) WeaponBase = 1;
                }
                break;
            case 51:
                Unk02 = CritterGetGender(obj);
                goto LABEL_76;
            case 52:
                if( ProtoGetObj( 455, &pObj ) != -1 ){
                    WeaponBase = 1;
                    pObj->Critt.BaseStat[0] = (int)obj;
                }
                break;
            case 53:
                if( ProtoGetObj(455, &v27) != -1 ) WeaponBase = v27->Critt.BaseStat[0];
                break;
            default:
                return;
        }
        Unk02 = WeaponBase;
        goto LABEL_76;
    }
    v10 = scr;
    Unk02 = 0;
    gMenuEscape = 2;
LABEL_77:
    RETINT( scr, Unk02 );
*/
}

void ScrGame_OpAnim( Intp_t *scr )
{
    ObjCritterCond_t *p_State;
    VidRect_t Area2, Area1;
    Obj_t *obj;
    int v9, Id, v12, Orientation, v21;
    short type[ 3 ];

    p_State = NULL;
    v12 = 0;
    GETARGI( scr, type[ 0 ], Orientation, 0, "anim" );
    GETARGI( scr, type[ 1 ], v21, 1, "anim" );
    GETARGP( scr, type[ 2 ], obj, 2, "anim" );

    if( !obj ){
        ScrGameErrorMsg( scr, "anim", 1 );
        return;
    }
    if( v21 < 0x41 ){
        if( OBJTYPE( obj->Pid ) == TYPE_CRIT ) p_State = &obj->Critter.State;
        v9 = ScrGamUnk06( obj, v21, 1 );
        AnimStart( 1 );
        if( !Orientation ){
            AnimUnk48( obj, v9, 0 );
            if ( v9 >= 20 && v9 <= 35 ){
                AnimUnk62( obj, ArtMakeId( 1, obj->ImgId & 0xFFF, v9 + 28, (obj->ImgId & 0xF000) >> 12, (obj->ImgId & 0x70000000) >> 28 ), -1 );
            }
            if( p_State ) p_State->CombatResult &= ~0x02;            
        } else {
    	    Id = ArtMakeId( (obj->ImgId & 0xF000000) >> 24, obj->ImgId & 0xFFF, v9, (obj->ImgId & 0xF000) >> 12, (obj->ImgId & 0x70000000) >> 28 );
    	    AnimUnk49( obj, v9, 0 );
    	    if( v9 == 36 ){
        	v12 = ArtMakeId( (Id & 0xF000000) >> 24, Id & 0xFFF, 49, (Id & 0xF000) >> 12, (Id & 0x70000000) >> 28 );
    	    } else {
        	if( v9 == 37 ) v12 = ArtMakeId( (Id & 0xF000000) >> 24, Id & 0xFFF, 48, (Id & 0xF000) >> 12, (Id & 0x70000000) >> 28 );
    	    }
    	    if( p_State ) p_State->CombatResult |= 0x02;
    	    AnimUnk62( obj, v12, -1 );
        }
        AnimBegin();
        return;
    }
    if( v21 == 1000 ){
        if( Orientation < 6 ){
            ObjSetRotation( obj, Orientation, &Area2 );
            TileUpdateArea( &Area2, gCurrentMapLvl );
        }
    } else if( v21 == 1010 ){
        ObjSetFrame( obj, Orientation, &Area1 );
        TileUpdateArea( &Area1, gCurrentMapLvl );
    } else {
        ScrGameEprintf( "\nScript Error: %s: op_anim: anim out of range", scr->FileName );
    }
}

void ScrGame_ObjCarryingPidObj( Intp_t *scr )
{
    int val;
    Obj_t *obj, *Item;
    short type[ 2 ];

    Item = NULL;
    GETARGI( scr, type[ 0 ], val, 0, "obj_carrying_pid_obj" );
    GETARGP( scr, type[ 1 ], obj, 1, "obj_carrying_pid_obj" );
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
    Obj_t *obj;
    int par[ 2 ];
    short type[ 2 ];
    
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
    if( IN_COMBAT ) return;
    switch( par[1] ){
	case 1: AnimStart( par[0] );
	case 2: AnimClear( obj );
	case 3: AnimBegin( );    
    }
}

void ScrGame_RegAnimAnimate( Intp_t *scr )
{
    int val0, val1, vlvl;
    Obj_t *obj;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], val0, 0, "reg_anim_animate" );
    GETARGI( scr, type[ 1 ], val1, 1, "reg_anim_animate" );
    GETARGP( scr, type[ 2 ], obj, 2, "reg_anim_animate" );
    if( IN_COMBAT ) return;
    if( val1 == 20 && obj && obj->Pid == 0x100002F ){
	vlvl = 0;
	CfgGetInteger( &gConfiguration, "preferences", "violence_level", &vlvl );
	if( vlvl < 2 ) return;
    }
    if( obj )
        AnimUnk48( obj, val1, val0 );
    else
        ScrGameErrorMsg( scr, "reg_anim_animate", 1 );
}

void ScrGame_RegAnimAnimateReverse( Intp_t *scr )
{
    int val[ 2 ];
    Obj_t *obj;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "reg_anim_animate_reverse" );
    GETARGI( scr, type[ 1 ], val[ 1 ], 1, "reg_anim_animate_reverse" );
    GETARGP( scr, type[ 2 ], obj, 2, "reg_anim_animate_reverse" );
    if( IN_COMBAT ) return;
    if( obj )
        AnimUnk49( obj, val[ 1 ], val[ 0 ] );
    else
        ScrGameErrorMsg( scr, "reg_anim_animate_reverse", 1 );
}

void ScrGame_RegAnimObjMoveToObj( Intp_t *scr )
{
    int val;
    Obj_t *DstObj, *Object;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], val, 0, "reg_anim_obj_move_to_obj" );
    GETARGP( scr, type[ 1 ], DstObj, 1, "reg_anim_obj_move_to_obj" );
    GETARGP( scr, type[ 2 ], Object, 2, "reg_anim_obj_move_to_obj" );
    if( IN_COMBAT ) return;
    if( Object )
        AnimObjMoveToObj( Object, DstObj, -1, val );
    else
        ScrGameErrorMsg( scr, "reg_anim_obj_move_to_obj", 1 );
}

void ScrGame_RegAnimObjRunToObj( Intp_t *scr )
{
    int val;
    Obj_t *obj2, *obj1;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], val, 0, "reg_anim_obj_run_to_obj" );
    GETARGP( scr, type[ 1 ], obj2, 1, "reg_anim_obj_run_to_obj" );
    GETARGP( scr, type[ 2 ], obj1, 2, "reg_anim_obj_run_to_obj" );

    if( IN_COMBAT ) return;
    if( obj1 )
        AnimObjRunToObj( obj1, obj2, -1, val );
    else
        ScrGameErrorMsg( scr, "reg_anim_obj_run_to_obj", 1 );
}

void ScrGame_RegAnimObjMoveToTile( Intp_t *scr )
{
    int val, tile;
    Obj_t *obj;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], val, 0, "reg_anim_obj_move_to_tile" );
    GETARGI( scr, type[ 1 ], tile, 1, "reg_anim_obj_move_to_tile" );
    GETARGP( scr, type[ 2 ], obj, 2, "reg_anim_obj_move_to_tile" );
    if( IN_COMBAT ) return;
    if( obj )
        AnimObjMoveToTile( obj, tile, obj->Elevation, -1, val );
    else
        ScrGameErrorMsg( scr, "reg_anim_obj_move_to_tile", 1 );
}

void ScrGame_RegAnimObjRunToTile( Intp_t *scr )
{
    int val, tile;
    Obj_t *obj;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], val, 0, "reg_anim_obj_run_to_tile" );
    GETARGI( scr, type[ 1 ], tile, 1, "reg_anim_obj_run_to_tile" );
    GETARGP( scr, type[ 2 ], obj, 2, "reg_anim_obj_run_to_tile" );
    if( IN_COMBAT ) return;
    if( obj )
        AnimObjRunToTile( obj, tile, obj->Elevation, -1, val );
    else
        ScrGameErrorMsg( scr, "reg_anim_obj_run_to_tile", 1 );
}

void ScrGame_PlayGmovie( Intp_t *scr )
{
    short type, tmp[ 34 ];
    int val;

    memcpy( tmp, gScrGameMovieFlags, sizeof( gScrGameMovieFlags ) );
    scr->Flags |= 0x20;
    GETARGI( scr, type, val, 0, "play_gmovie" );
    GdialogUnk04();
    if( GMoviePlay( val, tmp[ val ] ) == -1 ) eprintf( "\nError playing movie %d!", val );
    GdialogUnk03();
    scr->Flags &= ~0x20;
}

void ScrGame_AddMultObjsToInven( Intp_t *scr )
{
    VidRect_t Area;
    int Quantity;
    Obj_t *item, *dude;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], Quantity, 0, "add_mult_objs_to_inven" );
    GETARGP( scr, type[ 1 ], item, 1, "add_mult_objs_to_inven" );
    GETARGP( scr, type[ 2 ], dude, 2, "add_mult_objs_to_inven" );
    if( !dude || !item ) return;
    if( Quantity >= 0 ){
        if( Quantity > 99999 ) Quantity = 500;
    } else {
        Quantity = 1;
    }
    if( !ItemAdd( dude, item, Quantity ) ){
        ObjLightItem( dude, &Area );
        TileUpdateArea( &Area, dude->Elevation );
    }
}

void ScrGame_RmMultObjsToInven( Intp_t *scr )
{
    VidRect_t Area;
    int Quantity, v19, v11;
    Obj_t *item, *dude;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], Quantity, 0, "rm_mult_objs_from_inven" );
    GETARGP( scr, type[ 1 ], item, 1, "rm_mult_objs_from_inven" );
    GETARGP( scr, type[ 2 ], dude, 2, "rm_mult_objs_from_inven" );
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

void ScrGame_GetMonth( Intp_t *scr )
{
    int month;

    ScptGetGameDate( &month, 0, 0 );
    RETINT( scr, month );
}

void ScrGame_GetDay( Intp_t *scr )
{
    int day;

    ScptGetGameDate( NULL, &day, NULL );
    RETINT( scr, day );
}

void ScrGame_Explosion( Intp_t *scr )
{
    int val0, val1, tilenum, a3;
    short type[ 3 ];

    GETARGI( scr, type[ 0 ], val0, 0, "explosion" );
    GETARGI( scr, type[ 1 ], val1, 1, "explosion" );
    GETARGI( scr, type[ 2 ], tilenum, 2, "explosion" );
    a3 = 1;
    if( tilenum == -1 ){
        eprintf( "\nError: explosion: bad tile_num!" );
    } else {
        if( !val0 ) a3 = 0;
        ScptExplosion( tilenum, val1, a3, val0 );
    }
}

void ScrGame_GetTime( Intp_t *scr )
{
    int time;

    if( gMap.Time )
        time = (ScptGetGameDekaSeconds() - gMap.Time) / 864000;
    else
        time = -1;
    RETINT( scr, time );
}

void ScrGame_StartDialog( Intp_t *scr )
{
    scr->Flags |= 0x20;
    GdialogUnk08();
    if( scr ){
        scr->Flags &= ~0x20;
        IntpError( "Error starting dialog." );
    }
    scr->Flags &= ~0x20;
}

void ScrGame_Unk09( Intp_t *scr )
{
    scr->Flags |= 0x20;
    GdialogUnk16();
    scr->Flags &= ~0x20;
}

void ScrGame_GsayReply( Intp_t *scr )
{
    int i, val[2];
    char type[2], *s;

    scr->Flags |= 0x20;
    s = NULL;
    for( i = 0; i < 2; i++ ){
        type[ i ] = IntpPopwA( scr );
        val[ i ] = IntpPopiA( scr );
        if( type[ i ] == 0x9801 ) IntpStringDeRef( scr, type[ i ], val[ i ] );
        if( (type[ i ] & 0xF7FF) != 0xC001 ){
            if( i ){
                IntpError("script error: %s: invalid arg %d to gsay_reply", scr->FileName, i );
            } else if( type[ i ] == 0x9001 ){
                s = IntpGetArg( scr, type[1], val[0] );
            } else {
                IntpError( "script error: %s: invalid arg %d to gsay_reply", scr->FileName, 0 );
            }
        }
    }
    if( s )
        GdialogUnk15( scr, s );
    else
        GdialogUnk14( scr, val[ 1 ], val[ 0 ] );
    scr->Flags &= ~0x20;
}

void ScrGame_GsayOption( Intp_t *scr )
{
/*
    char *v2; // edx
    int v3; // eax
    int v4; // eax
    int v5; // edi
    int v6; // ebp
    int v7; // ecx
    unsigned __int16 v8; // ax
    int v9; // edx
    int v10; // edx
    char *Arg; // eax
    int Idx; // [esp+0h] [ebp-34h]
    int a2; // [esp+4h] [ebp-30h]
    int v14; // [esp+8h] [ebp-2Ch]
    int a1; // [esp+Ch] [ebp-28h]
    __int16 type[4]; // [esp+10h] [ebp-24h]
    char *a3; // [esp+18h] [ebp-1Ch]

    LOBYTE(scr->Flags) |= 0x20u;
    type[0] = IntpPopwA(scr);
    a3 = v2;
    v3 = IntpPopiA(scr);
    Idx = v3;
    if ( type[0] == (__int16)0x9801 )
        IntpStringDeRef(scr, 0x9801, v3);
    type[1] = IntpPopwA(scr);
    v4 = IntpPopiA(scr);
    a2 = v4;
    if ( type[1] == (__int16)0x9801 )
        IntpStringDeRef(scr, 0x9801, v4);
    v5 = 2;
    v6 = 8;
    do
    {
        type[v5] = IntpPopwA(scr);
        *(int *)((char *)&Idx + v6) = IntpPopiA(scr);
        if ( type[v5] == (__int16)0x9801 )
            IntpStringDeRef(scr, 0x9801, *(int *)((char *)&Idx + v6));
        v8 = type[v5];
        v9 = v8;
        BYTE1(v9) = HIBYTE(v8) & 0xF7;
        if ( v9 != 0xC001 )
        {
            if ( v7 == 2 )
            {
                if ( v9 == 0x9001 )
                    a3 = IntpGetArg(scr, SHIBYTE(v8), *(int *)((char *)&Idx + v6));
                else
                    IntpError("script error: %s: invalid arg %d to gsay_option", scr->FileName, 2);
            }
            else
            {
                IntpError("script error: %s: invalid arg %d to gsay_option", scr->FileName, v7);
            }
        }
        v6 += 4;
        ++v5;
    }
    while ( v7 + 1 < 4 );
    v10 = (unsigned __int16)type[1];
    BYTE1(v10) = HIBYTE(type[1]) & 0xF7;
    if ( v10 == 0x9001 )
    {
        Arg = IntpGetArg(scr, SHIBYTE(type[1]), a2);
        if ( a3 )
            GdialogUnk11(a1, (int)a3, Idx);
        else
            GdialogUnk10(a1, v14, Idx);
        goto LABEL_25;
    }
    if ( v10 != 0xC001 )
    {
        IntpError("Invalid arg 3 to sayOption");
LABEL_25:
        LOBYTE(scr->Flags) &= ~0x20u;
        return;
    }
    if ( a3 )
        GdialogUnk13(a1, (int)a3, (const char *)a2, Idx);
    else
        GdialogUnk12(a1, v14, a2, Idx);
    LOBYTE(scr->Flags) &= ~0x20u;
*/
}

/*
    void gsay_message(int msg_list, int msg_num, int reaction) - displays a replica of the NPC, do not imply any response from the player
*/
void ScrGame_GsayMessage( Intp_t *scr )
{
/*
    int v2; // edi
    int v3; // ebp
    char v4; // ah
    __int16 v5; // ax
    int v6; // ecx
    int v7; // eax
    int v8; // ecx
    __int16 v9; // dx
    unsigned __int16 v10; // ax
    int v11; // edx
    int val[3]; // [esp+0h] [ebp-30h]
    __int16 type[3]; // [esp+Ch] [ebp-24h] OVERLAPPED
    char *Arg; // [esp+14h] [ebp-1Ch]

    v2 = 0;
    v3 = 0;
    Arg = 0;
    scr->Flags |= 0x20;
    do {
        v5 = IntpPopwA(scr);
        *(__int16 *)((char *)type + v6) = v5;
        v7 = IntpPopiA(scr);
        v9 = *(__int16 *)((char *)type + v8);
        val[v3] = v7;
        if ( v9 == (__int16)0x9801 ) IntpStringDeRef(scr, *(__int16 *)((char *)type + v8), v7);
        v10 = *(__int16 *)((char *)type + v8);
        v11 = v10;
        BYTE1(v11) = HIBYTE(v10) & 0xF7;
        if( v11 != 0xC001 ){
            if( v2 == 1 ){
                if( v11 == 0x9001 )
                    Arg = IntpGetArg(scr, SHIBYTE(v10), val[v3]);
                else
                    IntpError("script error: %s: invalid arg %d to gsay_message", scr->FileName, 1);
            } else {
                IntpError("script error: %s: invalid arg %d to gsay_message", scr->FileName, v2);
            }
        }
        ++v3;
        ++v2;
    }
    while ( v2 < 3 );

    if( Arg )
        GdialogUnk15( scr, Arg );
    else
        GdialogUnk14( scr, val[2], val[1] );
    GdialogUnk10(-2, -2, 50);
    GdialogUnk09();
    scr->Flags &= ~0x20;
*/
}

void ScrGame_GigOption( Intp_t *scr )
{
    int i;
    int v12;
    int v13;
    int v14;
//char *Arg;
    int val[5];
    short type[5];
    char *a3;

    scr->Flags |= 0x20;
    for( i = 0; i < 5; i++ ){
        type[ i ] = IntpPopwA( scr );
        val[ i ] = IntpPopiA( scr );
        if( type[ i ] == 0x9801 ) IntpStringDeRef( scr, type[ i ], val[ i ] );
        if( (type[ i ] & 0xF7FF) != 0xC001 ){
            if( i == 2 ){
                if( type[ i ] == 0x9001 )
                    a3 = IntpGetArg( scr, type[ i ] >> 8, val[ i ] );
                else
                    IntpError( "script error: %s: invalid arg %d to giq_option", scr->FileName, 2 );
            } else {
                IntpError( "script error: %s: invalid arg %d to giq_option", scr->FileName, i );
            }
        }
    }
    a3 = NULL;
    v14 = v12 = FeatGetVal( gObjDude, FEAT_INTELLIGENCE );
    v13 = PerkLvl( gObjDude, PERK_SMOOTH_TALKER );
    if( v13 ) v14 = v12 + v13;
    if( val[ 4 ] < 0 ){
        if( -v14 < val[ 4 ] ){
            scr->Flags &= ~0x20;
            return;
        }
    } else if( v14 < val[ 4 ] ){
        scr->Flags &= ~0x20;
        return;
    }
    if( (type[ 1 ] & 0xF7FF) == 0x9001 ){
//	Arg = IntpGetArg( scr, type[ 1 ] >> 8, val[ 1 ] );
        if( a3 )
            GdialogUnk11( val[3], a3, val[0] );
        else
            GdialogUnk10( val[3], val[2], val[0] );
        scr->Flags &= ~0x20u;
        return;
    }
    if( type[ 1 ] != 0xC001 ){
        IntpError( "Invalid arg 4 to sayOption" );
        scr->Flags &= ~0x20u;
        return;
    }
    if( a3 )
        GdialogUnk13( val[3], a3, val[1], val[0] );
    else
        GdialogUnk12( val[3], val[2], val[1], val[0] );
    scr->Flags &= ~0x20;
}

void ScrGame_Poison( Intp_t *scr )
{
    int val;
    Obj_t *dude;
    short type[ 2 ];

    GETARGI( scr, type[ 0 ], val, 0, "poison" );
    GETARGP( scr, type[ 1 ], dude, 1, "poison" );
    if( dude ){
        if( CritterPoisonInc( dude, val ) ) eprintf( "\nScript Error: poison: adjust failed!" );
    } else {
        ScrGameErrorMsg( scr, "poison", 1 );
    }
}

void ScrGame_GetPoison( Intp_t *scr )
{
    int state;
    Obj_t *obj;
    short type;

    state = 0;
    GETARGP( scr, type, obj, 0, "get_poison" );
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

void ScrGame_PartyAdd( Intp_t *scr )
{
    short type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "party_add" );
    if( obj )
        PartyAddMember( obj );
    else
        ScrGameErrorMsg( scr, "party_add", 1 );
}

void ScrGame_PartyRemove( Intp_t *scr )
{
    short type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "party_remove" );
    if( obj )
        PartyRemoveMember( obj );
    else
        ScrGameErrorMsg( scr, "party_remove", 1 );
}

void ScrGame_RegAnimAnimateForever( Intp_t *scr )
{
    int val0;
    Obj_t *obj;
    short type[ 2 ];

    GETARGI( scr, type[ 0 ], val0, 0, "reg_anim_animate_forever" );
    GETARGP( scr, type[ 1 ], obj, 1, "reg_anim_animate_forever" );
    if( IN_COMBAT ) return;    
    if( obj )
        AnimUnk67( obj, val0, -1 );
    else
        ScrGameErrorMsg( scr, "reg_anim_animate_forever", 1 );
}

void ScrGame_CritterInjure( Intp_t *scr )
{
    int val0,v13,v14, v9;
    Obj_t *obj;
    short type[ 2 ];

    GETARGI( scr, type[ 0 ], val0, 0, "critter_injure" );
    GETARGP( scr, type[ 1 ], obj, 1, "critter_injure" );
    if( !obj ){
        ScrGameErrorMsg( scr, "critter_injure", 1 );
        return;
    }
    v9 = val0 & 0x7C;
    if( val0 & 0x800000 )
        obj->Critter.State.CombatResult &= ~v9;
    else
        obj->Critter.State.CombatResult |= v9;
    if( obj == gObjDude && ( v9 & 0x30 ) ){
        IfaceUnk16( &v14, &v13 );
        IfaceHandSlotUpdate( 1, v14, v13 );
    }
}

void ScrGame_InCombat( Intp_t *scr )
{
    RETINT( scr, gCombatStatus & 0x01 );
}

void ScrGame_GdialogBarter( Intp_t *scr )
{
    short type;
    int val;

    GETARGI( scr, type, val, 0, "gdialog_barter" );
    if( GdialogUnk58( val ) == -1 ) eprintf( "\nScript Error: gdialog_barter: failed" );
}

void ScrGame_GameDifficulty( Intp_t *scr )
{
    int gd;

    if( CfgGetInteger( &gConfiguration, "preferences", "game_difficulty", &gd ) != 1 ) gd = 1;
    RETINT( scr, gd );
}

void ScrGame_RunningBurningGuy( Intp_t *scr )
{
    int rbg;

    if( CfgGetInteger( &gConfiguration, "preferences", "running_burning_guy", &rbg ) != 1 ) rbg = 1;
    RETINT( scr, rbg );
}

void ScrGame_Unk10( Intp_t *scr )
{
    Obj_t *obj;
    int n;

    obj = ScptUnk140( scr );
    n = 1;
    if( obj == gObjDude && !IfaceGetSelectedHand() ) n = 0;
    InvUnk31( obj, n );
}

void ScrGame_ObjIsLocked( Intp_t *scr )
{
    int n;
    Obj_t *obj;
    short type;

    n = 0;
    GETARGP( scr, type, obj, 0, "obj_is_locked" );
    if( obj )
        n = UseObjLocked( obj );
    else
        ScrGameErrorMsg( scr, "obj_is_locked", 1 );
    RETINT( scr, n );
}

void ScrGame_ObjLock( Intp_t *scr )
{
    short type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_lock" );
    if( obj )
        UseObjLock( obj );
    else
        ScrGameErrorMsg( scr, "obj_lock", 1 );
}

void ScrGame_ObjUnlock( Intp_t *scr )
{
    short type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_unlock" );
    if( obj )
        UseObjUnlock( obj );
    else
        ScrGameErrorMsg( scr, "obj_unlock", 1 );
}

void ScrGame_ObjIsOpen( Intp_t *scr )
{
    int n;
    Obj_t *obj;
    short type;

    n = 0;
    GETARGP( scr, type, obj, 0, "obj_is_open" );
    if ( obj )
        n = UseObjInAnimation( obj );
    else
        ScrGameErrorMsg(scr, "obj_is_open", 1);
    RETINT( scr, n );
}

void ScrGame_ObjOpen( Intp_t *scr )
{
    short type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_open" );
    if( obj )
        UseRunAnimation1( obj );
    else
        ScrGameErrorMsg( scr, "obj_open", 1 );
}

void ScrGame_ObjClose( Intp_t *scr )
{
    short type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_close" );
    if( obj )
        UseRunAnimation2( obj );
    else
        ScrGameErrorMsg( scr, "obj_close", 1 );
}

void ScrGame_IfaceDisable( Intp_t *scr )
{
    GameIfaceDisable( 0 );
}

void ScrGame_IfaceEnable( Intp_t *scr )
{
    GameIfaceEnable();
}

void ScrGame_IfaceState( Intp_t *scr )
{    
    RETINT( scr, GameIfaceStat() );
}

void ScrGame_GfadeOut( Intp_t *scr )
{
    short type;
    int val;

    GETARGI( scr, type, val, 0, "gfade_out" );
    if( val )
        FadeStep( gFadePaletteC );
    else
        ScrGameErrorMsg( scr, "gfade_out", 1 );
}

void ScrGame_GfadeIn( Intp_t *scr )
{
    short type;
    int val;

    GETARGI( scr, type, val, 0, "gfade_in" );
    if( val )
        FadeStep( gPalBase );
    else
        ScrGameErrorMsg( scr, "gfade_in", 1 );
}

void ScrGame_ItemCapsTotal( Intp_t *scr )
{
    int MoneyAll;
    Obj_t *obj;
    short type;

    MoneyAll = 0;
    GETARGP( scr, type, obj, 0, "item_caps_total" );
    if( obj )
        MoneyAll = ItemGetMoneyAll( obj );
    else
        ScrGameErrorMsg( scr, "item_caps_total", 1 );
    RETINT( scr, MoneyAll );
}

void ScrGame_ItemCapsAdjust( Intp_t *scr )
{
    int adj, n;
    Obj_t *obj;
    short type[ 2 ];

    n = -1;
    GETARGI( scr, type[ 0 ], adj, 0, "item_caps_adjust" );
    GETARGP( scr, type[ 1 ], obj, 1, "item_caps_adjust" );
    if( obj )
        n = ItemBarter( obj, adj );
    else
        ScrGameErrorMsg( scr, "item_caps_adjust", 1 );
    RETINT( scr, n );
}

void ScrGame_AnimActionFrame( Intp_t *scr )
{
    ArtFrmHdr_t *data;
    int val0;
    Obj_t *obj;
    CachePool_t *img;
    short type[ 2 ];
    int ActionFrame;

    ActionFrame = 0;
    GETARGI( scr, type[ 0 ], val0, 0, "anim_action_frame" );
    GETARGP( scr, type[ 1 ], obj, 1, "anim_action_frame" );
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

void ScrGame_RegAnimPlaySfx( Intp_t *scr )
{
    Obj_t *obj;
    int val0, val1;
    short type[ 3 ];
    char *s;

    GETARGI( scr, type[ 0 ], val0, 0, "reg_anim_play_sfx" );
    GETARGS( scr, type[ 1 ], val1, 0, "reg_anim_play_sfx" );
    GETARGP( scr, type[ 2 ], obj, 0, "reg_anim_play_sfx" );
    s = IntpGetArg( scr, type[1], val1 );
    if( !s ){
        ScrGameErrorMsg( scr, "reg_anim_play_sfx", 3 );
        eprintf( " Can't match string!" );
    }
    if( obj )
        AnimUnk66( obj, s, val0 );
    else
        ScrGameErrorMsg( scr, "reg_anim_play_sfx", 1 );
}

void ScrGame_CritterModSkill( Intp_t *scr )
{
    Obj_t *obj;
    short type[ 3 ];
    int v9, i, j, v18, v19, val0, val1;
    
    GETARGI( scr, type[ 0 ], val0, 0, "critter_mod_skill" );
    GETARGI( scr, type[ 1 ], val1, 0, "critter_mod_skill" );
    GETARGP( scr, type[ 2 ], obj, 0, "critter_mod_skill" );
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

void ScrGame_SfxBuildCharName( Intp_t *scr )
{
    int err;
    unsigned int val0;
    int val1;
    Obj_t *obj;
    char stmp[ 16 ];
    short type[ 3 ];

    err = 0;
    GETARGI( scr, type[ 0 ], val0, 0, "sfx_build_char_name" );
    GETARGI( scr, type[ 1 ], val1, 0, "sfx_build_char_name" );
    GETARGP( scr, type[ 2 ], obj, 0, "sfx_build_char_name" );
    if( obj ){
        strcpy( stmp, GSoundProtoFname6( obj, val1, val0 ) );
        err = IntpDbgStr( scr, stmp, val0 );
    } else {
        ScrGameErrorMsg( scr, "sfx_build_char_name", 1 );
    }
    RETFTR( scr, err );
}

void ScrGame_SfxBuildAmbientName( Intp_t *scr )
{
    short type;
    int val;
    char stmp[24];

    GETARGI( scr, type, val, 0, "sfx_build_ambient_name" );
    strcpy( stmp, GSoundProtoFname5( IntpGetArg( scr, type >> 8, val ) ) );
    RETFTR( scr, IntpDbgStr( scr, stmp, val ) );
}

void ScrGame_SfxBuildInterfaceName( Intp_t *scr )
{
    short type;
    int val;
    char stmp[24];

    GETARGI( scr, type, val, 0, "sfx_build_interface_name" );
    strcpy( stmp, GSoundProtoFname4( IntpGetArg( scr, type >> 8, val ) ) );
    RETFTR( scr, IntpDbgStr( scr, stmp, val ) );
}

void ScrGame_SfxBuildItemName( Intp_t *scr )
{
    short type;
    int val;
    char stmp[24];

    GETARGI( scr, type, val, 0, "sfx_build_item_name" );
    strcpy( stmp, GSoundProtoFname4( IntpGetArg( scr, type >> 8, val ) ) );
    RETFTR( scr, IntpDbgStr( scr, stmp, val ) );
}

void ScrGame_SfxBuildWeaponName( Intp_t *scr )
{
    Obj_t *val0, *obj;
    int val1, val2;
    char stmp[16];
    short type[ 4 ];

    GETARGP( scr, type[ 0 ], val0, 0, "sfx_build_weapon_name" );
    GETARGI( scr, type[ 1 ], val1, 1, "sfx_build_weapon_name" );
    GETARGP( scr, type[ 2 ], obj, 2, "sfx_build_weapon_name" );
    GETARGI( scr, type[ 3 ], val2, 3, "sfx_build_weapon_name" );
    strcpy( stmp, GSoundProtoFname3( val2, obj, val1, val0 ) );
    RETFTR( scr, IntpDbgStr( scr, stmp, val1 ) );
}

void ScrGame_SfxBuildSceneryName( Intp_t *scr )
{
    char stmp[16];
    int val0, val1, val2;
    char type[2], *s;

    GETARGI( scr, type[ 0 ], val0, 0, "sfx_build_scenery_name" );
    GETARGI( scr, type[ 1 ], val1, 1, "sfx_build_scenery_name" );
    GETARGI( scr, type[ 2 ], val2, 2, "sfx_build_scenery_name" );
    s = IntpGetArg( scr, type[ 1 ], val0 );
    strcpy( stmp, GSoundProtoFname1( val2, val1, s ) );
    RETFTR( scr, IntpDbgStr( scr, stmp, val1 ) );
}

void ScrGame_SfxBuildOpenName( Intp_t *scr )
{
    int val,n;
    Obj_t *obj;
    short type[2];
    char stmp[16];

    n = 0;
    GETARGI( scr, type[ 0 ], val, 1, "sfx_build_open_name" );
    GETARGP( scr, type[ 1 ], obj, 2, "sfx_build_open_name" );
    if( obj ){
        strcpy( stmp, GSoundProtoFname2( obj, val ) );
        n = IntpDbgStr( scr, stmp, val );
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
    Scpt01_t tmp;
    Obj_t *VictimObj, *WhoObj;
    short type[ 2 ];

    GETARGP( scr, type[ 0 ], VictimObj, 0, "attack_setup" );
    GETARGP( scr, type[ 1 ], WhoObj, 1, "attack_setup" );
    scr->Flags |= 0x20;
    if( WhoObj ){
        if( !CritterCanTalk( WhoObj ) || ( WhoObj->Flags & 0x01 ) ){
            eprintf( "\n   But is already dead or invisible" );
            scr->Flags &= ~0x20;
            return;
        }
        if( !CritterCanTalk( VictimObj ) || (VictimObj->Flags & 0x01) ){
            eprintf( "\n   But target is already dead or invisible" );
            scr->Flags &= ~0x20;
            return;
        }
        if( VictimObj->Critter.State.Reaction & OBJ_STAT_FLEE ){
            eprintf( "\n   But target is AFRAID" );
            scr->Flags &= ~0x20;
            return;
        }
        if( gCombatStatus & 0x01 ){
            if( !(WhoObj->Grid.DestMapId & 0x01) ){
                WhoObj->Critter.State.Reaction |= 0x01;
                WhoObj->Critter.State.WhoHitMeObj = VictimObj;
            }
        } else {
            tmp.crit = VictimObj;
            tmp.obj = WhoObj;
            memset( tmp.i03, 0, sizeof( tmp.i03 ) );
            tmp.i07 = 0x7FFFFFFF;
            if( WhoObj == VictimObj ) {
//                tmp.i10 = VictimObj;
//                tmp.i09 = WhoObj;
                tmp.i08 = 1;
            } else {
                tmp.i08 = 0;
            }
            ScptUnk121( &tmp );
        }
    }
    scr->Flags &= ~0x20;
}

/*
    int destroy_mult_objs( ObjectPtr item, int count ) - destroys the specified number of copies of the object
*/
void ScrGame_DestroyMultObjs( Intp_t *scr )
{
    Obj_t *obj, *Owner;
    VidRect_t Area;
    int val[ 2 ];
    short type[ 2 ];
    int v26, v27, v16;

    v26 = 0;
    v27 = 0;
    scr->Flags |= 0x20;
    GETARGI( scr, type[ 0 ], val[ 0 ], 0, "destroy_mult_objs" );
    GETARGP( scr, type[ 1 ], obj, 1, "destroy_mult_objs" );    
    if( obj == ScptUnk140( scr ) ) v26 = 1;
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
            AnimClear( obj );
            ObjDestroy( obj, &Area );
        }
        v27 = v16;
    } else {
        AnimClear( obj );
        ObjDestroy( obj, &Area );
        TileUpdateArea( &Area, gCurrentMapLvl );
    }
    RETINT( scr, v27 );
    scr->Flags &= ~0x20;
    if( v26 ) scr->Flags |= 0x1000000;
}

/*
    ObjectPtr use_obj_on_obj( ObjectPtr item, ObjectPtr targetObj ) 
	Attempt to use an item object on a target object (targetObj). 
	This could be used to have a critter use a Stimpack on the player, for instance, or to use a key on a door.
*/
void ScrGame_UseObjOnObj( Intp_t *scr )
{
    Obj_t *obj1, *obj2, *p;
    short type[2];
    Scpt_t *script;

    GETARGP( scr, type[ 0 ], obj1, 0, "use_obj_on_obj" );
    GETARGP( scr, type[ 1 ], obj2, 0, "use_obj_on_obj" );
    if( obj2 && obj1 ){
	if( ScptPtr( ScptGetActionSource( scr ), &script ) != -1 ){
    	    p = ScptUnk140( scr );
    	    if( OBJTYPE( p->Pid ) == TYPE_CRIT )
        	ActionUseSceneryObject( p, obj1, obj2 );
    	    else
        	UseUnk18( p, obj1, obj2 );
    	    return;
	}
    }
    ScrGameErrorMsg( scr, "use_obj_on_obj", 1 );    
}

void ScrGame_Unk11( Intp_t *scr )
{
    scr->Flags |= 0x20;
    ScptUnk114();
    scr->Flags &= ~0x20;
}

/*
    void move_obj_inven_to_obj( ObjectPtr srcObj, ObjectPtr destObj ) - moves the inventory object in the inventory object srcObj destObj
*/
void ScrGame_oveObjInvenToObj( Intp_t *scr )
{
    Obj_t *v10, *obj, *RHandObj, *ArmorObj;
    short type[ 2 ];
    int v15, v9;

    RHandObj = 0;
    v15 = 0;
    ArmorObj = 0;
    GETARGP( scr, type[ 0 ], v10, 0, "move_obj_inven_to_obj" );
    GETARGP( scr, type[ 1 ], obj, 0, "move_obj_inven_to_obj" );
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
        Item15( obj, v10 );
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

void ScrGame_Unk12( Intp_t *scr )
{
    scr->Flags |= 0x20;
    EndGameUnk01();
    scr->Flags &= ~0x20;
}

/*
    ObjectPtr obj_art_fid( ObjectPtr obj ) - Returns the fid # (used to index art) of a given object (obj). 
*/
void ScrGame_ObjArtFid( Intp_t *scr )
{
    int id = 0;
    short type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_art_fid" );
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
    short type;
    int val;

    GETARGI( scr, type, val, 0, "art_anim" );
    RETINT( scr, (val & 0xFF0000) >> 16 );
}

/*
    ObjectPtr party_member_obj( int pid ) - returns a pointer to a party member for PID
*/
void ScrGame_PartyMemberObj( Intp_t *scr )
{
    short type;
    int pid;

    GETARGI( scr, type, pid, 0, "party_member_obj" );
    RETPTR( scr, PartyUnk09( pid ) );
}

/*
    int rotation_to_tile( int srcTile, int destTile ) - Returns the rotation (0…5) to face a particular tile (destTile) from a particular tile (srcTile).
*/
void ScrGame_RotationToTile( Intp_t *scr )
{
    int srcTile, destTile;
    short type[ 2 ];

    GETARGI( scr, type[ 0 ], destTile, 0, "rotation_to_tile" );
    GETARGI( scr, type[ 1 ], srcTile, 0, "rotation_to_tile" );
    RETINT( scr, TileTurnAt( srcTile, destTile ) );
}

/*
    int jam_lock(ObjectPtr lockableObj)
	Jams a lock, which prevents the player from picking the lock for approximately 24 hours. 
	Meant to be used when a player critically fails to pick a lock.
*/
void ScrGame_JamLock( Intp_t *scr )
{
    short type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "jam_lock" );    
    UseObjJam( obj );
}

/*
    void gdialog_set_barter_mod( int mod )
	Sets the current modifier for barter to a given percentage (mod). 
	Used to make barter easier/harder, even if the player initiates barter (as opposed to the script starting it.)
*/
void ScrGame_GdialogSetBarterMod( Intp_t *scr )
{
    short type;
    int val;

    GETARGI( scr, type, val, 0, "gdialog_set_barter_mod" );
    GdialogUnk57( val );
}

/*
    int combat_difficulty - returns the complexity of the fight, set in the game settings
*/
void ScrGame_CombatDifficulty( Intp_t *scr )
{
    int n;

    if( CfgGetInteger( &gConfiguration, "preferences", "combat_difficulty", &n ) != 1 ) n = 0;
    RETINT( scr, n );
}

/*
    boolean obj_on_screen( ObjectPtr what ) - checks to see whether the object on the game screen
*/
void ScrGame_ObjOnScreen( Intp_t *scr )
{
    short type;
    int n;
    Obj_t *obj;
    VidRect_t Area2, Area1;

    Area2 = (VidRect_t)gScrGameUnk001;
    n = 0;
    GETARGP( scr, type, obj, 1, "obj_on_screen" );    
    if( obj ){
        if( gCurrentMapLvl == obj->Elevation ){
            ObjGetRadiusArea( obj, &Area1 );
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
    int n = 0;    
    Obj_t *obj;
    short type;

    GETARGP( scr, type, obj, 1, "critter_is_fleeing" );    
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
    int state;
    Obj_t *obj;
    short type[ 2 ];

    GETARGI( scr, type[ 0 ], state, 0, "critter_set_flee_state" );
    GETARGP( scr, type[ 1 ], obj, 1, "critter_set_flee_state" );    
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
    ?
*/
void ScrGame_Unk13( Intp_t *scr )
{
    Obj_t *obj;

    if( !IN_COMBAT ) return;
    gMenuEscape = 1;    
    if( !(obj = ScptUnk140( scr ) ) ) return;
    if( OBJTYPE( obj->Pid ) != TYPE_CRIT ) return;
    obj->Critter.State.WhoHitMe = 0;
    obj->Critter.State.Reaction |= 0x02;
    CombatUnk12( obj, 0 );
}

/*
    void debug_msg( string text ) - displays a message box debug message
*/
void ScrGame_DebugMsg( Intp_t *scr )
{
    int val, debug_en;
    short type;
    char *s;

    debug_en = 0;
    GETARGS( scr, type, val, 0, "debug_msg" );    
    if( (s = IntpGetArg( scr, type >> 8, val )) ){
        CfgGetInteger( &gConfiguration, "const", "show_script_messages", &debug_en );
        if( debug_en ){
            eprintf( "\n%s", s );
        }
    }
}

/*
    void critter_stop_attacking( ObjectPtr who ) - set a flag indicating that the critter does not want to continue the fight
*/
void ScrGame_CritterStopAttacking( Intp_t *scr )
{
    short type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "critter_stop_attacking" );
    if( obj ){
        obj->Critter.State.WhoHitMe = 0;
        obj->Critter.State.Reaction |= 0x02;
        CombatUnk12( obj, 0 );
    } else {
        ScrGameErrorMsg( scr, "critter_stop_attacking", 1 );
    }
}

/*
    ObjectPtr tile_contains_pid_obj( int tile, int elev, int pid ) - returns a pointer to the first object at a given hex
*/
void ScrGame_TileContainsPidObj( Intp_t *scr )
{
    int tile, elev, pid;
    Obj_t *q, *p;
    short type [3 ];

    q = NULL;
    GETARGI( scr, type[ 0 ], pid, 0, "tile_contains_pid_obj" );
    GETARGI( scr, type[ 1 ], elev, 1, "tile_contains_pid_obj" );
    GETARGI( scr, type[ 2 ], tile, 2, "tile_contains_pid_obj" );
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
    int n = 0;
    short type;
    Obj_t *obj;

    GETARGP( scr, type, obj, 0, "obj_name" );
    if( obj ){
        gScrGamePlayerName = ObjGetName( obj );
    } else {
        n = 1;
        ScrGameErrorMsg( scr, "obj_name", 1 );
    }
    RETFTR( scr, IntpDbgStr( scr, gScrGamePlayerName, n ) );
}

/*
    int get_pc_stat( int pcStat ) - Returns the value of a desired pc-only stat of the obj_dude. These are found in define.h all starting with “PCSTAT_”.
*/
void ScrGame_GetPcStat( Intp_t *scr )
{
    short type;
    int val;

    GETARGI( scr, type, val, 0, "get_pc_stat" );
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
    SciAddOpcode( 0x80BC, ScrGameUnk02 );
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
    SciAddOpcode( 0x80D1, ScrGame_Dummy );
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
    SciAddOpcode( 0x80DF, ScrGame_Unk01 );
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
    SciAddOpcode( 0x80EA, ScrGame_Unk05 );
    SciAddOpcode( 0x80EB, ScrGame_GetGTime );
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
    SciAddOpcode( 0x80F6, ScrGame_GetGameTime );
    SciAddOpcode( 0x80F7, ScrGame_FixedParam );
    SciAddOpcode( 0x80F8, ScrGame_TileIsVisible );
    SciAddOpcode( 0x80F9, ScrGame_Unk06 );
    SciAddOpcode( 0x80FA, ScrGame_ActionBeingUsed );
    SciAddOpcode( 0x80FB, ScrGame_CritterState );
    SciAddOpcode( 0x80FC, ScrGame_GameTimeAdvance );
    SciAddOpcode( 0x80FD, ScrGame_RadiationInc );
    SciAddOpcode( 0x80FE, ScrGame_RadiationDec );
    SciAddOpcode( 0x80FF, ScrGame_CritterAttemptPlacement );
    SciAddOpcode( 0x8100, ScrGame_ObjPid );
    SciAddOpcode( 0x8101, ScrGame_GetMapId );
    SciAddOpcode( 0x8102, ScrGame_OpCritterAddTrait );
    SciAddOpcode( 0x8103, ScrGame_CritterRmTrait );
    SciAddOpcode( 0x8104, ScrGame_ProtoData );
    SciAddOpcode( 0x8105, ScrGame_MessageStr );
    SciAddOpcode( 0x8106, ScrGame_CritterInvenObj );
    SciAddOpcode( 0x8107, ScrGame_ObjSetLightLevel );
    SciAddOpcode( 0x8108, ScrGame_Unk07 );
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
    SciAddOpcode( 0x8117, ScrGame_RmMultObjsToInven );
    SciAddOpcode( 0x8118, ScrGame_GetMonth );
    SciAddOpcode( 0x8119, ScrGame_GetDay );
    SciAddOpcode( 0x811A, ScrGame_Explosion );
    SciAddOpcode( 0x811B, ScrGame_GetTime );
    SciAddOpcode( 0x811C, ScrGame_StartDialog );
    SciAddOpcode( 0x811D, ScrGame_Unk09 );
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
    SciAddOpcode( 0x812C, ScrGame_Unk10 );
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
    SciAddOpcode( 0x8146, ScrGame_Unk11 );
    SciAddOpcode( 0x8147, ScrGame_oveObjInvenToObj );
    SciAddOpcode( 0x8148, ScrGame_Unk12 );
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
    SciAddOpcode( 0x8153, ScrGame_Unk13 );
    SciAddOpcode( 0x8154, ScrGame_DebugMsg );
    SciAddOpcode( 0x8155, ScrGame_CritterStopAttacking );
}


