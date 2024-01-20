#pragma once

/*
    Game engine related commands handlers.
*/

/*
short gScrGameMovieFlags[ 17 ];
VidRect_t gScrGameUnk001;
int gScrGameMood;
char *gScrGamePlayerName;
*/

void ScrGameErrorMsg( Intp_t *strerr, const char *Operand, int ErrClass );
void ScrGameEprintf( const char *fmt, ... );
int ScrGameIsTileVisible( int pos );
int ScrGameSetShape( Obj_t *critter, Obj_t *a2, int Flags );

/*
    void give_exp_points( int points );
*/
void ScrGame_GiveExpPoints( Intp_t *scr );
/*
    void scr_return
*/
void ScrGame_Return( Intp_t *scr );
/*
    void play_sfx( string sfxFileName );
*/
void ScrGame_PlaySfx( Intp_t *scr );
/*
    void set_map_start( int x, int y, int elev, int rot );
*/
void ScrGame_SetMapStart( Intp_t *scr );
/*
    void override_map_start( int x, int y, int elev, int rot );
*/
void ScrGame_OverrideMapStart( Intp_t *scr );
/*
    int has_skill( ObjectPtr who, int skill );
*/
void ScrGame_HasSkill( Intp_t *scr );
/*
    boolean using_skill( ObjectPtr who, int skill );
*/
void ScrGame_UsingSkill( Intp_t *scr );
/*
    int roll_vs_skill( ObjectPtr who, int skill, int modifier );
*/
void ScrGame_RollVsSkill( Intp_t *scr );
/*
    ** not fully implemented in game, return is always 0 **    
    int skill_contest( int, int, int );
*/
void ScrGame_SkillContest( Intp_t *scr );
/*
    int do_check( ObjectPtr who, int check, int modifier );
*/
void ScrGame_DoCheck( Intp_t *scr );
/* 
    int success( int );
*/
void ScrGame_Success( Intp_t *scr );
/* 
    int critical( int ); 
*/
void ScrGame_Critical( Intp_t *scr );
/*
    int how_much( int val );
*/
void ScrGame_HowMuch( Intp_t *scr );
/*
    mark_area_known( int mark_type, int area, int mark_state );
    mark_type: town_map, world_map
*/
void ScrGame_MarkAreaKnown( Intp_t *scr );
/*
    ** not fully implemented in game, return is always 0 **
    int reaction_influence( int ?, int ?, int ? );
*/
void ScrGame_ReactionInfluence( Intp_t *scr );
/*
    int random( int min, int max );
*/
void ScrGame_Random( Intp_t *scr );
/*
    ** not fully implemented in game, return is always 0 **
    Critter roll_dice( obj Critter, int Skill );
*/
void ScrGame_RollDice( Intp_t *scr );
/*
    int move_to( ObjectPtr obj, int tile_num, 0-2 elev );
*/
void ScrGame_MoveTo( Intp_t *scr );
/*
    ObjectPtr create_object( int pid, int tile_num, 0-2 elev, int sid );
*/
void ScrGame_CreateObject( Intp_t *scr );
/*
    int destroy_object( ObjectPtr obj );
*/
void ScrGame_DestroyObject( Intp_t *scr );
/*
    void display_msg( string message );
*/
void ScrGame_DisplayMsg( Intp_t *scr );
/*
    void script_overrides
*/
void ScrGame_ScriptOverrides( Intp_t *scr );
/*
    int obj_is_carrying_obj( ObjectPtr invenObj, int item_pid );
*/
void ScrGame_ObjIsCarryingObj( Intp_t *scr );
/*
    boolean tile_contains_obj_pid( int tile, int elev, int pid );
*/
void ScrGame_TileContainsObjPid( Intp_t *scr );
/*
    ObjectPtr ?();
*/
void ScrGameUnk02( Intp_t *scr );
/*
    ObjPtr source_obj(); - returns a pointer to the source object (activator); for this action
*/
void ScrGame_SourceObj( Intp_t *scr );
/*
    ObjectPtr target_obj();
*/
void ScrGame_TargetObj( Intp_t *scr );
/*
    ObjectPtr dude_obj
*/
void ScrGame_DudeObj( Intp_t *scr );
/*
    ObjectPtr obj_being_used_with
*/
void ScrGame_ObjBeingUsedWith( Intp_t *scr );
/*
    int op_local_var
*/
void ScrGame_OpLocalvar( Intp_t *scr );
/*
    void set_local_var( int var_index, int value );
*/
void ScrGame_SetLocalvar( Intp_t *scr );
/*
    int op_map_var( int var_index );
*/
void ScrGame_OpMapVar( Intp_t *scr );
/*
    void set_map_var( int var_index, int value );
*/
void ScrGame_SetMapVar( Intp_t *scr );
/*
    int op_global_var( int var_index );
*/
void ScrGame_OpGlobalVar( Intp_t *scr );
/*
    void set_global_var( int var_index, int value );
*/
void ScrGame_OpSetGlobalVar( Intp_t *scr );
/*
    int script_action
*/
void ScrGame_ScriptAction( Intp_t *scr );
/*
    int op_obj_type( ObjectPtr );
*/
void ScrGame_OpObjType( Intp_t *scr );
/*
    int op_item_subtype( ObjectPtr item );
*/
void ScrGame_OpItemSubtype( Intp_t *scr );
/*
    int get_critter_stat( ObjectPtr who, int stat );
*/
void ScrGame_GetCritterStat( Intp_t *scr );
/*
    int set_critter_stat( ObjectPtr who, int stat, int amount );
*/
void ScrGame_SetCritterStat( Intp_t *scr );
/*
    void animate_stand_obj( ObjectPtr obj );
*/
void ScrGame_AnimateStandObj( Intp_t *scr );
/*
    void animate_stand_reverse_obj( ObjectPtr obj );
*/
void ScrGame_AnimateStandReverseObj( Intp_t *scr );
/*
    void animate_move_obj_to_tile( ObjectPtr who, int tile, int speed );
*/
void ScrGame_AnimateMoveObjToTile( Intp_t *scr );
/*
    boolean tile_in_tile_rect( int tile1, int tile2, int tile3, int tile4, int tile );
*/
void ScrGame_TileInTileRect( Intp_t *scr );
void ScrGame_Dummy( Intp_t *scr );
/*
    int tile_distance( int tile1, int tile2 );
*/
void ScrGame_TileDistance( Intp_t *scr );
/*
    int tile_distance_objs( ObjectPtr obj1, ObjectPtr obj2 );
*/
void ScrGame_TileDistanceObjs( Intp_t *scr );
/*
    int tile_num( ObjectPtr obj );
*/
void ScrGame_TileNuM( Intp_t *scr );
/*
    int tile_num_in_direction( int start_tile, int dir, int distance );
*/
void ScrGame_TileNumInDirection( Intp_t *scr );
/*
    void pickup_obj( ObjectPtr obj );
*/
void ScrGame_PickupObj( Intp_t *scr );
/*
    void drop_obj( ObjectPtr obj );
*/
void ScrGame_DropObj( Intp_t *scr );
/*
    void add_obj_to_inven( ObjectPtr who, ObjectPtr item );
*/
void ScrGame_AddObjToInv( Intp_t *scr );
/*
    void rm_obj_from_inven( ObjectPtr who, ObjectPtr obj );
*/
void ScrGame_RmObjFromInv( Intp_t *scr );
/*
    void wield_obj_critter( ObjectPtr who, ObjectPtr obj );
*/
void ScrGame_WieldObjCritter( Intp_t *scr );
/*
    void use_obj( ObjectPtr obj );
*/
void ScrGame_UseObj( Intp_t *scr );
/*
    boolean obj_can_see_obj( ObjectPtr src, ObjectPtr dst );
*/
void ScrGame_ObjCanSeeObj( Intp_t *scr );
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
    ); 
*/
void ScrGame_Attack( Intp_t *scr );
/*
    void start_gdialog( int msgFileNum, ObjectPtr who, int mood, int headNum, int backgroundIdx ); - to start a dialogue
*/
void ScrGame_StartGdialog( Intp_t *scr );
/*
    ?
*/
void ScrGame_Unk01( Intp_t *scr );
/*
    void dialogue_reaction (int mood); - animates an emotional reaction in the dialogue
*/
void ScrGame_DialogueReaction( Intp_t *scr );
void ScrGameUnk04( int x0, int y0, int x1, int y1, int a5 );
void ScrGameUnk05( int x0, int y0, int x1, int y1, int a5 );
/*
    => another feature of working with a variety of options for the game, is different from the number of arguments metarule
    int metarule3(
3	    int meta3_switch, 
2	    int meta3_par1, 
1	    int meta3_par2, 
0	    int meta3_par3
	); 
*/
void ScrGame_Metarule3( Intp_t *scr );
/*
    void set_map_music( int MapId, string FileName );
*/
void ScrGame_SetMapMusic( Intp_t *scr );
/*
    void set_obj_visibility ( ObjectPtr obj, boolean visibility ); - sets the visibility flag of the object
*/
void ScrGame_SetObjVisibility( Intp_t *scr );
/*
    => Loads a new map (map_name);, removing all scripts currently running and passing on 
	the entrance location (start_location); to the new map’s map_init script. 
	Can also use the number representing the map in the map list (for example, between 0 to 64 in Fallout 1);
    void load_map( string or int map_name, int start_location ); 
*/
void ScrGame_LoadMap( Intp_t *scr );
/*
    void wm_area_set_pos (int areaIdx, int xPos, int yPos); - set the world coordinates for the specified city
*/
void ScrGame_WmAreaSetPos( Intp_t *scr );
void ScrGame_SetExitGrids( Intp_t *scr );
void ScrGame_AnimBusy( Intp_t *scr );
void ScrGame_CritterHeal( Intp_t *scr );
void ScrGame_SetLightLevel( Intp_t *scr);
void ScrGame_Unk05( Intp_t *scr );
void ScrGame_GetGTime( Intp_t *scr );
void ScrGame_Elevation( Intp_t *scr );
void ScrGame_KillCritter( Intp_t *scr );
int ScrGamUnk06( Obj_t *obj, int a2, int a3 );
void ScrGame_KillCritterType( Intp_t *scr );
void ScrGame_CritterDamage( Intp_t *scr );
void ScrGame_OpAddTimerEvent( Intp_t *scr );
void ScrGame_RmTimeEvent( Intp_t *scr );
void ScrGame_GameTicks( Intp_t *scr );
void ScrGame_HasTrait( Intp_t *scr );
void ScrGame_ObjCanHearObj( Intp_t *scr );
void ScrGame_GetGameTime( Intp_t *scr );
void ScrGame_FixedParam( Intp_t *scr );
void ScrGame_TileIsVisible( Intp_t *scr );
void ScrGame_Unk06( Intp_t *scr );
void ScrGame_ActionBeingUsed( Intp_t *scr );
void ScrGame_CritterState( Intp_t *scr );
void ScrGame_GameTimeAdvance( Intp_t *scr );
void ScrGame_RadiationInc( Intp_t *scr );
void ScrGame_RadiationDec( Intp_t *scr );
void ScrGame_CritterAttemptPlacement( Intp_t *scr );
void ScrGame_ObjPid( Intp_t *scr );
void ScrGame_GetMapId( Intp_t *scr );
void ScrGame_OpCritterAddTrait( Intp_t *scr );
void ScrGame_CritterRmTrait( Intp_t *scr );
void ScrGame_ProtoData( Intp_t *scr );
void ScrGame_MessageStr( Intp_t *scr );
void ScrGame_CritterInvenObj( Intp_t *scr );
void ScrGame_ObjSetLightLevel( Intp_t *scr );
void ScrGame_Unk07( Intp_t *scr );
void ScrGame_InvenCmds( Intp_t *scr );
void ScrGame_FloatMsg( Intp_t *scr );
void ScrGame_MetaRule( Intp_t *scr ); 
void ScrGame_OpAnim( Intp_t *scr );
void ScrGame_ObjCarryingPidObj( Intp_t *scr );
/*
    void reg_anim_func (int par1, int par2); - work with a list of animations
*/
void ScrGame_RegAnimFunc( Intp_t *scr );
void ScrGame_RegAnimAnimate( Intp_t *scr );
void ScrGame_RegAnimAnimateReverse( Intp_t *scr );
void ScrGame_RegAnimObjMoveToObj( Intp_t *scr );
void ScrGame_RegAnimObjRunToObj( Intp_t *scr );
void ScrGame_RegAnimObjMoveToTile( Intp_t *scr );
void ScrGame_RegAnimObjRunToTile( Intp_t *scr );
void ScrGame_PlayGmovie( Intp_t *scr );
void ScrGame_AddMultObjsToInven( Intp_t *scr );
void ScrGame_RmMultObjsToInven( Intp_t *scr );
void ScrGame_GetMonth( Intp_t *scr );
void ScrGame_GetDay( Intp_t *scr );
void ScrGame_Explosion( Intp_t *scr );
void ScrGame_GetTime( Intp_t *scr );
void ScrGame_StartDialog( Intp_t *scr );
void ScrGame_Unk09( Intp_t *scr );
void ScrGame_GsayReply( Intp_t *scr );
void ScrGame_GsayOption( Intp_t *scr );
/*
    void gsay_message(int msg_list, int msg_num, int reaction); - displays a replica of the NPC, do not imply any response from the player
*/
void ScrGame_GsayMessage( Intp_t *scr );
void ScrGame_GigOption( Intp_t *scr );
void ScrGame_Poison( Intp_t *scr );
void ScrGame_GetPoison( Intp_t *scr );
void ScrGame_PartyAdd( Intp_t *scr );
void ScrGame_PartyRemove( Intp_t *scr );
void ScrGame_RegAnimAnimateForever( Intp_t *scr );
void ScrGame_CritterInjure( Intp_t *scr );
void ScrGame_InCombat( Intp_t *scr );
void ScrGame_GdialogBarter( Intp_t *scr );
void ScrGame_GameDifficulty( Intp_t *scr );
void ScrGame_RunningBurningGuy( Intp_t *scr );
void ScrGame_Unk10( Intp_t *scr );
void ScrGame_ObjIsLocked( Intp_t *scr );
void ScrGame_ObjLock( Intp_t *scr );
void ScrGame_ObjUnlock( Intp_t *scr );
void ScrGame_ObjIsOpen( Intp_t *scr );
void ScrGame_ObjOpen( Intp_t *scr );
void ScrGame_ObjClose( Intp_t *scr );
void ScrGame_IfaceDisable( Intp_t *scr );
void ScrGame_IfaceEnable( Intp_t *scr );
void ScrGame_IfaceState( Intp_t *scr );
void ScrGame_GfadeOut( Intp_t *scr );
void ScrGame_GfadeIn( Intp_t *scr );
void ScrGame_ItemCapsTotal( Intp_t *scr );
void ScrGame_ItemCapsAdjust( Intp_t *scr );
void ScrGame_AnimActionFrame( Intp_t *scr );
void ScrGame_RegAnimPlaySfx( Intp_t *scr );
void ScrGame_CritterModSkill( Intp_t *scr );
void ScrGame_SfxBuildCharName( Intp_t *scr );
void ScrGame_SfxBuildAmbientName( Intp_t *scr );
void ScrGame_SfxBuildInterfaceName( Intp_t *scr );
void ScrGame_SfxBuildItemName( Intp_t *scr );
void ScrGame_SfxBuildWeaponName( Intp_t *scr );
void ScrGame_SfxBuildSceneryName( Intp_t *scr );
void ScrGame_SfxBuildOpenName( Intp_t *scr );
/*
    void attack_setup( ObjectPtr who, ObjectPtr victim ); - makes a critter who attack victim
*/
void ScrGame_AttackSetup( Intp_t *scr );
/*
    int destroy_mult_objs( ObjectPtr item, int count ); - destroys the specified number of copies of the object
*/
void ScrGame_DestroyMultObjs( Intp_t *scr );
/*
    ObjectPtr use_obj_on_obj( ObjectPtr item, ObjectPtr targetObj ); 
	Attempt to use an item object on a target object (targetObj);. 
	This could be used to have a critter use a Stimpack on the player, for instance, or to use a key on a door.
*/
void ScrGame_UseObjOnObj( Intp_t *scr );
void ScrGame_EndGameSlideShow( Intp_t *scr );
/*
    void move_obj_inven_to_obj( ObjectPtr srcObj, ObjectPtr destObj ); - moves the inventory object in the inventory object srcObj destObj
*/
void ScrGame_MoveObjInvenToObj( Intp_t *scr );
void ScrGame_EndGameMovie( Intp_t *scr );
/*
    ObjectPtr obj_art_fid( ObjectPtr obj ); - Returns the fid # (used to index art); of a given object (obj);. 
*/
void ScrGame_ObjArtFid( Intp_t *scr );
/*
    void art_anim (int fid); - returns the type of animation for a given FID
*/
void ScrGame_ArtAnim( Intp_t *scr );
/*
    ObjectPtr party_member_obj( int pid ); - returns a pointer to a party member for PID
*/
void ScrGame_PartyMemberObj( Intp_t *scr );
/*
    int rotation_to_tile( int srcTile, int destTile ); - Returns the rotation (0…5); to face a particular tile (destTile); from a particular tile (srcTile);.
*/
void ScrGame_RotationToTile( Intp_t *scr );
/*
    int jam_lock(ObjectPtr lockableObj);
	Jams a lock, which prevents the player from picking the lock for approximately 24 hours. 
	Meant to be used when a player critically fails to pick a lock.
*/
void ScrGame_JamLock( Intp_t *scr );
/*
    void gdialog_set_barter_mod( int mod );
	Sets the current modifier for barter to a given percentage (mod);. 
	Used to make barter easier/harder, even if the player initiates barter (as opposed to the script starting it.);
*/
void ScrGame_GdialogSetBarterMod( Intp_t *scr );
/*
    int combat_difficulty - returns the complexity of the fight, set in the game settings
*/
void ScrGame_CombatDifficulty( Intp_t *scr );
/*
    boolean obj_on_screen( ObjectPtr what ); - checks to see whether the object on the game screen
*/
void ScrGame_ObjOnScreen( Intp_t *scr );
/*
    boolean critter_is_fleeing( ObjectPtr who ); - checks whether the critter escapes from the battlefield
*/
void ScrGame_CritterIsFleeing( Intp_t *scr );
/*
    int critter_set_flee_state( ObjectPtr who, Boolean flee_on ); - Sets the FLEE flag on or off. This controls whether the critter flees during combat.
*/
void ScrGame_CritterSetFleeState( Intp_t *scr );
/*
    terminate_combat void Combat 
    Tells the combat system to terminate prematurely. 
    USE WITH CAUTION. This doesn’t prevent another (or even the SAME) script from re-starting combat, so make sure you turn off any hostile flags, etc.
*/
void ScrGame_TerminateCombat( Intp_t *scr );
/*
    void debug_msg( string text ); - displays a message box debug message
*/
void ScrGame_DebugMsg( Intp_t *scr );
/*
    void critter_stop_attacking( ObjectPtr who ); - set a flag indicating that the critter does not want to continue the fight
*/
void ScrGame_CritterStopAttacking( Intp_t *scr );
/*
    ObjectPtr tile_contains_pid_obj( int tile, int elev, int pid ); - returns a pointer to the first object at a given hex
*/
void ScrGame_TileContainsPidObj( Intp_t *scr );
/*
    string obj_name( ObjectPtr what ); - returns the name of the object
*/
void ScrGame_ObjName( Intp_t *scr );
/*
    int get_pc_stat( int pcStat ); - Returns the value of a desired pc-only stat of the obj_dude. These are found in define.h all starting with “PCSTAT_”.
*/
void ScrGame_GetPcStat( Intp_t *scr );
void ScpGameSetup();
