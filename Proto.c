#include "FrameWork.h"

ProtoLib_t gProtoObjLib[ 11 ] = { 
    { NULL, 0, 0, 1 }, // items
    { NULL, 0, 0, 1 }, // critters
    { NULL, 0, 0, 1 }, // scenery
    { NULL, 0, 0, 1 }, // walls
    { NULL, 0, 0, 1 }, // tiles
    { NULL, 0, 0, 1 }, // misc
    { NULL, 0, 0, 1 }, // intrface
    { NULL, 0, 0, 0 }, // inven
    { NULL, 0, 0, 0 }, // heads
    { NULL, 0, 0, 0 }, // backgnd
    { NULL, 0, 0, 0 }, // skilldex
};

int gProtoAllocSize[ 11 ] = { 132, 416, 56, 36, 28, 28, 0, 0, 0, 0, 0 };
int gProtoUnk02 = 0;

Proto_t gProtoDude = 
{
    .Pid = 0x1000000,
    .TextID = -1,
    .ImgId = 0x1000001,
    .LtRad = 0,
    .LtInt = 0,
    .Flags = 0x20000000,
    .FlgExt = 0,
    .AttackMode = -1,
    .Critt.Type = 0,
    .Critt.BaseStat = { 5,5,5,5, 5,5,5,5, 5,5,5,5, 5,5,5,5, 18, 0,0,0,0,0,0,0,0, 100, 0,0,0, 23, 0 },
    .Critt.BoostStat = {0},
    .Critt.Skills = {0},
    .Critt.ProtoID = 0,
    .Critt.KillExp = 0,
    .Critt.Gender = 0,
    .Critt.TypeNameID = 0,
    .i09 = -1,
    .AiPacketNum = 0,
    .i11 = 0
};

int gProtoImgId = 0x1000001;
Proto_t gProtos = { 0 }; // tymczasowo 0
char *gProtoFilePath = "proto/";
int gProtoVirgin1 = 0;
int gProtoError = 0;

char *gProtoPerkNames[120];
char *gProtoStatNames[40];
Msg_t gProtoMsg[ 6 ];
char *gProtoUnk105[2];
char *gProtoUnk102[6];
Msg_t gProtoMessages;
char *gProtoUnk106[8];
char *gProtoUnk100[4];
char *gProtoUnk101[7];
char *gProtoUnk103[7];
char *gProtoUnk104[19];

int ProtoUnk08( unsigned int Pid, char *a2, int *a3 );

/**************************************************************************/

void ProtoUnk10( char *path, unsigned int Pid )
{
    strcpy( path, gProtoDataFilePath );
    strcpy( path + strlen( path ), gProtoFilePath );
    if( Pid == -1 ) return;
    strcpy( path + strlen( path ), ArtGetCatDirName( ARTT( Pid ) ) );
}

int ProtoUnk09( unsigned int Pid, char *fname )
{
    int c, tmp;
    xFile_t *fh;
    char stmp[ 260 ];

    if( ProtoUnk08( Pid, fname, &tmp ) ) sprintf( fname, "%08d", 0 );

    strcpy( stmp, gProtoDataFilePath );
    strcpy( &stmp[ strlen( stmp ) ], gProtoFilePath );
    if( Pid != -1 ) strcpy( &stmp[strlen(stmp)], ArtGetCatDirName( ARTT( Pid ) ) );    
    strcpy( &stmp[ strlen( stmp ) ], "/" );
    strcpy( &stmp[ strlen( stmp ) ], ArtGetCatDirName( ARTT( Pid ) ) );
    strcpy( &stmp[ strlen( stmp ) ], ".lst" );
    if( !(fh = dbOpen( stmp, "a+t") ) ) return -1;
    dbseek( fh, -1, 2 );
    c = dbgetc( fh );
    dbseek( fh, 0, 2 );
    if( c != '\n' ) dbputc( c, fh );
    dbPrintf( fh, "%s.pro\n", fname );
    dbClose( fh );
    return 0;
}

int ProtoUnk08( unsigned int Pid, char *a2, int *a3 )
{
    xFile_t *fh;
    int i;
    char c;
    char stmp3[260];
    char stmp1[260];
    char stmp2[260];

    if( Pid == -1 ){ *a2 = 0; *a3 = -1; return -1; }
    strcpy( stmp2, gProtoDataFilePath );
    strcpy( stmp2 + strlen( stmp2 ), gProtoFilePath );
    if( Pid != -1 ) strcpy( stmp2 + strlen( stmp2 ), ArtGetCatDirName( Pid >> 24 ) );
    strcpy( stmp2 + strlen( stmp2 ), "/" );
    strcpy( stmp2 + strlen( stmp2 ), ArtGetCatDirName( Pid >> 24 ) );
    strcpy( stmp2 + strlen( stmp2 ), ".lst" );
    if( !(fh = dbOpen( stmp2, "rt" ) ) ) return -2;
    strcpy( stmp3, a2 );
    StrUpr( stmp3 );
    strcpy( stmp3 + strlen( stmp3 ), ".PRO" );
    *a3 = 0;
    while( dbgets( stmp1, 260, fh ) ){
        if( stmp1[0] != '\n' || stmp1[1] != '\0' ) ++*a3;
        for( i = -1; (c = stmp1[ ++i ]); ){
            if( c == '\n' ) stmp1[i--] = '\0';
        }
        StrUpr( stmp1 );
        if( !strcmp( stmp1, stmp3 ) ) { dbClose(fh); return -1; }
    }
    *a3 = -1;
    dbClose( fh );
    return 0;    
}

int ProtoGetFName( unsigned int Idx, char *fname )
{
    unsigned int i;
    char *p, tmp1[260], tmp2[260];
    xFile_t *fh;

    if( Idx == -1 || !fname ) return -1;    
    strcpy( tmp2, gProtoDataFilePath );
    strcpy( tmp2 + strlen( tmp2 ), gProtoFilePath );
    if( Idx != -1 ) strcpy( tmp2 + strlen( tmp2 ), ArtGetCatDirName( Idx >> 24 ) );
    strcpy( tmp2 + strlen( tmp2 ), "/" );
    strcpy( tmp2 + strlen( tmp2 ), ArtGetCatDirName( Idx >> 24 ) );
    strcpy( tmp2 + strlen( tmp2 ), ".lst" );
    if( !(fh = dbOpen(tmp2, "rt")) ) return -1;    
    Idx &= 0xFFFFFF;    
    for( i = 1; i <= Idx; i++ ){
        if( !dbgets( tmp1, 260, fh ) ) break;
    }
    if( i - 1 == Idx ){
        p = strchr( tmp1, ' ' );
        if( p ) *p = '\0';
        p = strchr( tmp1, '\n' );
        if( p ) *p = '\0';
        strcpy( fname, tmp1 );
    }
    dbClose( fh );
    return 0;
}

int ProtoGetTypeSize( ProtoType_e Type )
{
    if( Type < TYPE_PROTO_ALL ) return gProtoAllocSize[ Type ];
    return 0;
}

int ProtoOpenable( unsigned int Pid )
{
    Proto_t *proto;

    if( ProtoGetObj(Pid, &proto) == -1 ) return 0;
    if( !(proto->FlgExt & 0x800) && !(Pid >> 24) && proto->Critt.Type == PR_ITEM_CONTAINER ) return 1;
    return proto->FlgExt & 0x800;
}

int ProtoDrugUsable( unsigned int Pid )
{
    Proto_t *proto;

    if( ProtoGetObj(Pid, &proto) == -1 ) return 0;
    if( !(proto->FlgExt & 0x1000) && !(Pid >> 24) && (proto->Critt.Type == PR_ITEM_DRUG ) ) return 1;
    return proto->FlgExt & 0x1000;
}

int ProtoUnk05( unsigned int Pid )
{
    Proto_t *proto;

    if( ProtoGetObj( Pid, &proto ) == -1 ) return 0;
    if( OBJTYPE( Pid ) == TYPE_CRIT ) return 1;
    return (proto->FlgExt & 0x4000) != 0;
}

int ProtoItemAccessible( unsigned int Pid )
{
    Proto_t *proto;

    if( OBJTYPE( Pid ) != TYPE_ITEM ) return 0;
    if( ProtoGetObj( Pid, &proto ) == -1 ) return 0;
    if( proto->Critt.Type == PR_ITEM_CONTAINER ) return proto->FlgExt & PR_CONTAINER_OPEN; // locked ?
    return 1;    
}

char *ProtoGetMsg( int Pid, int MsgId )
{
    MsgLine_t MsgLine;
    Proto_t *proto;

    if( ProtoGetObj( Pid, &proto ) == -1 ) return NULL;
    if( proto->TextID == -1 ) return NULL;        
    MsgLine.Id = proto->TextID + MsgId;
    if( MessageGetMsg( &gProtoMsg[ OBJTYPE( Pid ) ], &MsgLine ) != 1 ) return NULL;
    return MsgLine.Text;        
}

char *ProtoGetObjName( unsigned int Pid )
{
    if( Pid == 0x1000000 ) return CritterGetName( gObjDude ); // player name
    return ProtoGetMsg( Pid, 0 );
}

char *ProtoGetDudeDsc( int Pid )
{
    return ProtoGetMsg( Pid, 1 );
}

int ProtoInitItem( Proto_t *proto, int Pid )
{
    Pid = Pid & 0xFFFFFF;
    proto->Pid = -1;
    proto->TextID = 100 * Pid;
    proto->ImgId = ArtMakeId( 0, Pid - 1, 0, 0, 0 );
    if( !ArtFileExist( proto->ImgId ) ) proto->ImgId = ArtMakeId( 0, 0, 0, 0, 0 );
    proto->LtRad = 0;
    proto->LtInt = 0;
    proto->Flags = 0xA0000008;
    proto->FlgExt = 0xA000;
    proto->AttackMode = -1;
    proto->Critt.Type = TYPE_MISC;
    ProtoUnk02( proto, TYPE_MISC );
    proto->Critt.BaseStat[ 18 ] = 1;
    proto->Critt.BaseStat[ 19 ] = 1;
    proto->Critt.BaseStat[ 20 ] = 10;
    proto->Critt.BaseStat[ 21 ] = 0;
    proto->Critt.BaseStat[ 22 ] = -1;
    proto->Critt.BaseStat[ 23 ] = 48;
    return 0;
}

int ProtoUnk02( Proto_t *proto, ProtoType_e Type )
{
    int i;

    switch( Type ){
        case TYPE_ITEM:
            proto->Critt.BaseStat[ 0 ] = 0;
            for( i = 0; i < 8; i++ ){
                proto->Critt.BaseStat[ 8 + i ] = 0;
                proto->Critt.BaseStat[ 1 + i ] = 0;
            }
            proto->Critt.BaseStat[16] = -1;
            proto->Critt.BaseStat[17] = -1;
            proto->Critt.BaseStat[15] = -1;
            return 0;
        case TYPE_CRIT:
            proto->Critt.BaseStat[1] = 0;
            proto->Critt.BaseStat[0] = 250;
            proto->FlgExt |= 0x800;
            return 0;
        case TYPE_SCEN:
            proto->Critt.BaseStat[1] = -1;
            proto->Critt.BaseStat[2] = -1;
            proto->Critt.BaseStat[3] = 0;
            proto->Critt.BaseStat[4] = 0;
            proto->Critt.BaseStat[5] = 0;
            proto->Critt.BaseStat[6] = 0;
            proto->Critt.BaseStat[7] = 0;
            proto->Critt.BaseStat[8] = 0;
            proto->Critt.BaseStat[9] = 0;
            proto->Critt.BaseStat[10] = 0;
            proto->Critt.BaseStat[11] = 0;
            proto->Critt.BaseStat[12] = 0;
            proto->Critt.BaseStat[13] = 0;
            proto->Critt.BaseStat[14] = 0;
            proto->Critt.BaseStat[15] = 0;
            proto->Critt.BaseStat[16] = 0;
            proto->Critt.BaseStat[0] = 0;
            proto->FlgExt |=0x1000;
            return 0;
        case TYPE_WALL:
            proto->Critt.BaseStat[1] = 0;
            proto->Critt.BaseStat[2] = 0;
            proto->Critt.BaseStat[3] = 0;
            proto->Critt.BaseStat[4] = 0;
            proto->Critt.BaseStat[5] = 0;
            proto->Critt.BaseStat[6] = -1;
            proto->Critt.BaseStat[7] = 0;
            proto->Critt.BaseStat[8] = 0;
            proto->Critt.BaseStat[9] = 0;
            proto->Critt.BaseStat[10] = 0;
            proto->Critt.BaseStat[11] = -1;
            proto->Critt.BaseStat[12] = 0;
            proto->Critt.BaseStat[13] = 0;
            proto->Critt.BaseStat[14] = -1;
            proto->Critt.BaseStat[15] = 0;
            proto->Critt.BaseStat[16] = 0;
            proto->Critt.BaseStat[0] = 0;
            return 0;
        case TYPE_TILE:
            proto->Critt.BaseStat[1] = 20;
            proto->Critt.BaseStat[2] = 0;
            proto->Critt.BaseStat[3] = 0;
            proto->Critt.BaseStat[4] = 1;
            proto->Critt.BaseStat[5] = 1;
            proto->Critt.BaseStat[0] = 0;
            return 0;
        case TYPE_MISC:
            proto->Critt.BaseStat[1] = 20;
            proto->Critt.BaseStat[0] = -1;
            return 0;
        case TYPE_IFCE:
            proto->Critt.BaseStat[0] = -1;
            proto->FlgExt |= 0x1000;
            return 0;
        default: return 0;
    }
}

int ProtoInitCritter( Proto_t *proto, int ImgPid )
{
    ImgPid &= 0x00FFFFFF;
    if( gProtoUnk02 == 0 ) return -1;
    proto->Pid = -1;
    proto->LtRad = 0;
    proto->LtInt = 0;
    proto->Flags = 0x20000000;
    proto->FlgExt = 0x6000;
    proto->AttackMode = -1;
    proto->Critt.Type = 0;
    proto->Critt.ProtoID = 0;
    proto->i09 = -1;
    proto->AiPacketNum = 1;
    proto->TextID = 100 * ImgPid;
    proto->i11 = 1;
    proto->ImgId = ArtMakeId( 1, ImgPid - 1, 0, 0, 0 );
    if( !ArtFileExist( proto->ImgId ) ) proto->ImgId = ArtMakeId( 1, 0, 0, 0, 0 );
    proto->Critt.KillExp = 60;
    proto->Critt.Gender = 0;
    proto->Critt.TypeNameID = 0;
    FeatReset( &proto->Critt );
    SkillReset( &proto->Critt );
    return 0;
}

void ProtoDudeEffectReset( Obj_t *dude )
{
    memset( &dude->Critter, 0, sizeof(ObjCritter_t) );
}

int ProtoLoadCritterCondition( ObjCritterCond_t *Critter, xFile_t *fh)
{
    int tmp;
    
    if( dbgetBei( fh, &Critter->DmgLastTurn ) == -1 ) return -1; // 58 00 Reaction to player (?)
    if( dbgetBei( fh, &Critter->Reaction ) == -1 ) return -1; // 5C 04 Current mp (?)
    if( dbgetBei( fh, &Critter->CurrentAP ) == -1 ) return -1; // 60 08 Combat result
    if( dbgetBei( fh, &Critter->CombatResult ) == -1 ) return -1; // 64 0C Damage last turn
    if( dbgetBei( fh, &Critter->AIpackNb ) == -1 ) return -1; // 68 10 AI packed number
    if( dbgetBei( fh, &Critter->GroupId ) == -1 ) return -1; // 6C 14 Group ID
    if( dbgetBei( fh, &tmp ) == -1 ) return -1; // 70 18 Who hit me
    Critter->WhoHitMe = tmp;
    return 0;
}

int ProtoSaveCritterCondition( ObjCritterCond_t *dude, xFile_t *fh )
{
    if( dbputBei( fh, dude->DmgLastTurn ) == -1 ) return -1;    
    if( dbputBei( fh, dude->Reaction ) == -1 ) return -1;     
    if( dbputBei( fh, dude->CurrentAP ) == -1 ) return -1;    
    if( dbputBei( fh, dude->CombatResult ) == -1 ) return -1;    
    if( dbputBei( fh, dude->AIpackNb ) == -1 ) return -1;    
    if( dbputBei( fh, dude->GroupId ) == -1 ) return -1;    
    if( dbputBei( fh, dude->WhoHitMe ) == -1 ) return -1;
    return 0;
}

int ProtoLoadObj( Obj_t *dude, xFile_t *fh )
{
    int tmp;
    Proto_t *proto;

    /*
        Number of map objects in this object's inventory. If this is non zero, then after reading this map object, 
	the objects in the inventory must be read. The objects in the inventory are map objects as well, and they follow this object, 
	being preceeded by a 4 byte integer being the count of this map object in the inventory. 
    */
    if( dbgetBei( fh, &dude->Container.Box.Cnt ) == -1 ) return -1; // 48
    /* Maximum number of slots in critter inventory. */
    if( dbgetBei( fh, &dude->Container.Box.Capacity ) == -1 ) return -1; // 4C
    // 50 ?? 'Unknown 10' pointer ???!!
    if( dbgetBei( fh, &tmp ) == -1 ) return -1;
    dude->Container.Box.Box = NULL; // tmp
    
    if( OBJTYPE( dude->Pid ) == TYPE_CRIT ){
        if( dbgetBei( fh, &dude->Critter.i04 ) == -1 ) return -1;	 // 54 Unk11
    	if( ProtoLoadCritterCondition( &dude->Critter.State, fh ) == -1 ) return -1;		 // 58 .. 70
    	if( dbgetBei( fh, &dude->Critter.HitPts ) == -1 ) return -1; 	// 74
    	if( dbgetBei( fh, &dude->Critter.Radiated ) == -1 ) return -1;	// 
    	if( dbgetBei( fh, &dude->Critter.Poisoned ) == -1 ) return -1;	// pointer!
    } else {
	// ?? 'Unknown 11'
        if( dbgetBei( fh, &dude->Container.i04 ) == -1 ) return -1; // 54 Unk11
        if( dude->PosY == 0xCCCCCCCC ){
            eprintf( "Note: Reading pud: updated_flags was un-Set!" );
            dude->PosY = 0;
        }
        switch( OBJTYPE( dude->Pid ) ){
            case TYPE_ITEM:
                if( ProtoGetObj( dude->Pid, &proto ) == -1 ) return -1;
                switch ( proto->Critt.Type ){
                    case PR_ITEM_WEAPON: // Extra fields for weapon
                        if( dbgetBei( fh, &dude->Container.Ammo ) == -1 ) return -1;   // Ammo count. Amount of ammunition loaded in this weapon
                        if( dbgetBei( fh, &dude->Container.AmmoId ) == -1 ) return -1; // Id of Ammo prototype
                        return 0;
                    case PR_ITEM_AMMO: // Extra fields for ammo -> Amount of ammo in magazine. Number of bullets or charges in this magazine.
                         if( dbgetBei( fh, &dude->Container.Ammo ) == -1 ) return -1;
                        return 0;
                    case PR_ITEM_MISC: // Extra fields for misc item -> Charges
                        if( dbgetBei( fh, &dude->Container.Charges ) == -1 ) return -1;
                        return 0;
                    case PR_ITEM_KEY: // Extra fields for keys -> KeyCode
                        if( dbgetBei( fh, &dude->Container.KeyCode ) == -1 ) return -1;
                        return 0;
                }
                return 0;
            case TYPE_SCEN:
                if( ProtoGetObj( dude->Pid, &proto ) == -1 ) return -1;
                switch( proto->Critt.Type ){
                    case PR_SCN_DOOR: // Extra fields for WalkThrough portals/doors
                        if( dbgetBei( fh, &dude->Scenery.i05 ) == -1 ) return -1;
                        return 0;
                    case PR_SCN_STAIRS: // Extra fields for stairs
                        if( dbgetBei( fh, &dude->Scenery.i06 ) == -1 ) return -1;
                        if( dbgetBei( fh, &dude->Scenery.i05 ) == -1 ) return -1;
                        return 0;                        
                    case PR_SCN_ELEVATOR: // Extra fields for elevators
                        if( dbgetBei( fh, &dude->Scenery.i05 ) == -1 ) return -1;
                        if( dbgetBei( fh, &dude->Scenery.i06 ) == -1 ) return -1;
                        return 0;                        
                    case PR_SCN_LADDER_BOT:// Extra fields for ladder bottom
                        if( gMap.Version == 19 ){ if( dbgetBei( fh, &dude->Scenery.i06 ) == -1 ) return -1; return 0; }
                        if( dbgetBei( fh, &dude->Scenery.i05 ) == -1 ) return -1;
                        if( dbgetBei( fh, &dude->Scenery.i06 ) == -1 ) return -1;
                        return 0;
                    case PR_SCN_LADDER_TOP: // Extra fields for ladder top
                        if( gMap.Version == 19 ){ if( dbgetBei( fh, &dude->Scenery.i06 ) == -1 ) return -1; return 0; }
                        if( dbgetBei( fh, &dude->Scenery.i05 ) == -1 ) return -1;
                        if( dbgetBei( fh, &dude->Scenery.i06 ) == -1 ) return -1;
                        break;
                    default:
                        return 0;
                }
                break;
            case TYPE_MISC:
                if( dude->Pid < 0x5000010 ) return 0;
                if( dude->Pid > 0x5000017 ) return 0;
                /* Exit grid */
                if( dbgetBei( fh, &dude->Grid.DestMapId ) == -1 ) return -1;
                if( dbgetBei( fh, &dude->Grid.DestStartPos ) == -1 ) return -1;
                if( dbgetBei( fh, &dude->Grid.DestMapElev ) == -1 ) return -1;
                if( dbgetBei( fh, &dude->Grid.DestOrientation ) == -1 ) return -1;
                return 0;
            default:
        	return 0;
        }
    }
    return 0;
}

int ProtoLoadA( xFile_t *fh, Obj_t *dude, int a3 )
{
    if( a3 != 16 ) return -1;
    if( dbgetBei(fh, &dude->TimeEv) ) return -1;
    if( dbgetBei(fh, &dude->GridId) ) return -1;
    if( dbgetBei(fh, &dude->PosX) ) return -1;
    if( dbgetBei(fh, &dude->PosY) ) return -1;
    return 0;
}

int ProtoSaveA( xFile_t *fh, Obj_t *dude, int a3 )
{
    if( a3 != 16 ) return -1;
    if( dbputBei(fh, dude->TimeEv) ) return -1;
    if( dbputBei(fh, dude->GridId) ) return -1;
    if( dbputBei(fh, dude->PosX) ) return -1;
    if( dbputBei(fh, dude->PosY) ) return -1;
    return 0;
}

int ProtoSaveDude( Obj_t *dude, xFile_t *fh )
{
    Proto_t *proto;

    if( dbputBei( fh, dude->Container.Box.Cnt ) == -1 ) return -1;
    if( dbputBei( fh, dude->Container.Box.Capacity ) == -1 ) return -1;
    if( dbputBei( fh, dude->Container.Box.Box ? -1:0 ) == -1 ) return -1;

    if( OBJTYPE( dude->Pid ) == TYPE_CRIT ){
        if( dbputBei( fh, dude->Critter.i04 ) == -1 || 
    	    ProtoSaveCritterCondition( &dude->Critter.State, fh) || 
    	    dbputBei( fh, dude->Critter.HitPts ) == -1 || 
    	    dbputBei( fh, dude->Critter.Radiated ) == -1 || 
    	    dbputBei( fh, dude->Critter.Poisoned ) == -1 ) return -1;
        return 0;
    }
    if( dbputBei( fh, dude->Container.i04 ) == -1 ) return -1;    // Flags ?
    switch( OBJTYPE( dude->Pid ) ){
        case TYPE_ITEM:
            if( ProtoGetObj( dude->Pid, &proto ) == -1 ) return -1;            
            switch( proto->Critt.Type ){
                case PR_ITEM_WEAPON: if( dbputBei( fh, dude->Container.Ammo ) == -1 || dbputBei( fh, dude->Container.AmmoId ) == -1 ) return -1; return 0;                    
                case PR_ITEM_AMMO: if( dbputBei( fh, dude->Container.Ammo ) == -1 ) return -1; return 0;
                case PR_ITEM_MISC: if( dbputBei( fh, dude->Container.Charges ) == -1 ) return -1; return 0;
                case PR_ITEM_KEY: if( dbputBei( fh, dude->Container.KeyCode ) == -1 ) return -1; return 0;
            }            
            break;
        case TYPE_SCEN:
            if( ProtoGetObj( dude->Pid, &proto ) == -1 ) return -1;
            switch( proto->Critt.Type ){
                case PR_SCN_DOOR: if( dbputBei( fh, dude->Scenery.i05 ) == -1 ) return -1; return 0;
                case PR_SCN_STAIRS: if( dbputBei( fh, dude->Scenery.i06 ) == -1 || dbputBei( fh, dude->Scenery.i05 ) == -1 ) return -1; return 0;
                case PR_SCN_ELEVATOR: if( dbputBei( fh, dude->Scenery.i05 ) == -1 || dbputBei( fh, dude->Scenery.i06 ) == -1 ) return -1; return 0;
                case PR_SCN_LADDER_BOT: if( dbputBei( fh, dude->Scenery.i05 ) == -1 || dbputBei( fh, dude->Scenery.i06 ) == -1 ) return -1; return 0;
                case PR_SCN_LADDER_TOP: if( dbputBei( fh, dude->Scenery.i05 ) == -1 || dbputBei( fh, dude->Scenery.i06 ) == -1 ) return -1; return 0;
            }
            break;
        case TYPE_MISC:
            if( dude->Pid < 0x5000010 ) return 0;
            if( dude->Pid > 0x5000017 ) return 0;
            if( dbputBei( fh, dude->Grid.DestMapId ) == -1 ) return -1;
            if( dbputBei( fh, dude->Grid.DestStartPos ) == -1 ) return -1;
            if( dbputBei( fh, dude->Grid.DestMapElev ) == -1 ) return -1;
            if( dbputBei( fh, dude->Grid.DestOrientation ) == -1 ) return -1;
            break;
    }        
    return 0;
}

int ProtoEffectApply( Obj_t *dude )
{
    unsigned int Pid;
    Proto_t *proto;

    if( !gProtoUnk02 ) return -1;
    dude->Critter.Box.Cnt = 0;
    dude->Critter.Box.Capacity = 0;
    dude->Critter.Box.Box = NULL;
    if( ProtoGetObj( dude->Pid, &proto ) == -1 ) return -1;
    switch( OBJTYPE( dude->Pid ) ){
        case TYPE_ITEM:
            switch( proto->Critt.Type ){
                case PR_ITEM_CONTAINER:
                    dude->Container.i04 = 0;
                    break;
                case PR_ITEM_WEAPON:
                    dude->Container.Ammo = proto->Critt.BaseStat[ 15 ];
                    dude->Container.AmmoId = proto->Critt.BaseStat[ 14 ];
                    break;
                case PR_ITEM_AMMO:
                    dude->Container.Ammo = proto->Critt.BaseStat[ 1 ];
                    break;
                case PR_ITEM_MISC:
                    dude->Container.Charges = proto->Critt.BaseStat[ 2 ];
                    break;
                case PR_ITEM_KEY:
                    dude->Container.KeyCode = proto->Critt.BaseStat[ 0 ];
                    break;
                default:
            	break;
            }
            break;
        case TYPE_SCEN:
            switch( proto->Critt.Type ){
                case 0:
                    dude->Scenery.i05 = proto->Critt.BaseStat[ 0 ];
                    break;
                case 1:
                    dude->Scenery.i06 = proto->Critt.BaseStat[ 0 ];
                    dude->Scenery.i05 = proto->Critt.BaseStat[ 1 ];
                    break;
                case 2:
                    dude->Scenery.i05 = proto->Critt.BaseStat[ 0 ];
                    dude->Scenery.i06 = proto->Critt.BaseStat[ 1 ];
                    break;
                case 3:
                case 4:
                    dude->Scenery.i06 = proto->Critt.BaseStat[ 0 ];
                    break;
            }
            break;
        case TYPE_MISC:
            Pid = dude->Pid;
            if( Pid >= 0x5000010 && Pid <= 0x5000017 ){
                dude->Grid.DestStartPos = -1;
                dude->Grid.DestMapElev = 0;
                dude->Grid.DestOrientation = 0;
                dude->Grid.DestMapId = -1;
            }
            break;
    }    
    return 0;
}

int ProtoEffectInit( Obj_t *dude )
{
    Proto_t *proto;

    if( !gProtoUnk02 ) return -1;
    if( !dude ) return -1;
    if( dude->Pid == -1 ) return -1;

    memset( &dude->Critter, 0, sizeof( ObjCritter_t ) );
    if( (dude->Pid >> 24) != 1 ) return ProtoEffectApply( dude );
    dude->Critter.Box.Cnt = 0;
    dude->Critter.Box.Capacity = 0;
    dude->Critter.Box.Box = NULL;
    CombatUnk06( dude );
    dude->Critter.HitPts = FeatGetVal( dude, FEAT_HP );
    dude->Critter.State.CurrentAP = FeatGetVal( dude, FEAT_AP );
    FeatStatsRecalculate( dude );
    dude->Critter.State.CombatResult = 0;
    if( ProtoGetObj( dude->Pid, &proto ) != -1 ){
        dude->Critter.State.Reaction = proto->AiPacketNum;
        dude->Critter.State.CurrentAP = proto->i11;
    }
    return 0;
}

void ProtoDudeImgInit()
{
    int n;
    Proto_t *proto;

    if( ProtoGetObj( PROTO_1, &proto ) == -1 ) return;    
    n = GMovieEnabled( 3 ) ? 2 : 0;
    gArtPrep[ 0 ] = ( FeatGetVal( gObjDude, FEAT_GENDER ) ) ? gArtPrep[ 2 + n ] : gArtPrep[ 1 + n ];
    if( !InvGetArmorObj( gObjDude ) ){
        if( InvGetRHandObj( gObjDude) || InvGetLHandObj( gObjDude ) ) n = ( gObjDude->ImgId & 0xF000 ) >> 12;
        ObjSetShape( gObjDude, ArtMakeId( 1, gArtPrep[ 0 ], 0, n, 0 ), 0 );
    }
    proto->ImgId = ArtMakeId( 1, gArtPrep[ 0 ], 0, 0, 0 );
}

int ProtoDudeInit( char *fname )
{
    Proto_t *proto = NULL, *proto_cur;

    gProtoDude.ImgId = ArtMakeId( 1, gArtPrep[0], 0, 0, 0 );
    if( gProtoVirgin1 ) ObjBoxDelete( &gObjDude->Container );
    gProtoVirgin1 = 1;
    if( ProtoGetObj( PROTO_1, &proto_cur ) == -1 ) return -1;    
    ProtoGetObj( gObjDude->Pid, &proto );
    ProtoEffectInit( gObjDude );
    gObjDude->Critter.State.AIpackNb = 0;
    gObjDude->Critter.State.GroupId = 0;
    ProtoReset();
    if( CritterLoadAllStats( fname ) == -1 ) gProtoError = -1;
    proto->Critt.BaseStat[29] = 100;
    proto->Critt.ProtoID = 0;
    proto->Critt.KillExp = 0;
    proto->Critt.Gender = 0;
    proto->Critt.TypeNameID = 0;
    ProtoDudeImgInit();
    InvSelectMain();
    if( gObjDude->Flags & 0x08 ) ObjSetPlayer( gObjDude, NULL );
    if( gObjDude->Flags & 0x10 ) gObjDude->Flags &= ~0x11;
    FeatStatsRecalculate( gObjDude );
    CritterHeal( gObjDude, 10000 );
    if( gProtoError ) eprintf( " ** Error in proto_dude_init()! **" );    
    return 0;
}

int ProtoInitScenery( Proto_t *proto, int Pid )
{
    int type;

    type = Pid & 0xFFFFFF;
    proto->Pid = -1;
    proto->TextID = 100 * type;
    proto->ImgId = ArtMakeId( 2, type - 1, 0, 0, 0 );
    if( !ArtFileExist( proto->ImgId ) ) proto->ImgId = ArtMakeId( 2, 0, 0, 0, 0 );
    proto->LtRad = 0;
    proto->LtInt = 0;
    proto->Flags = 0;
    proto->FlgExt = 0x2000;
    proto->AttackMode = -1;
    proto->Critt.Type = 5;
    ProtoUnk01( proto, TYPE_MISC );
    proto->Critt.BaseStat[2] = 1;
    proto->Critt.BaseStat[4] = 48;
    return 0;
}

int ProtoUnk01( Proto_t *proto, ProtoType_e Type )
{
    switch( Type ){
        case TYPE_ITEM:
            proto->Critt.BaseStat[0] = 0;
            proto->FlgExt |= 0x0800;
            break;
        case TYPE_CRIT:
            proto->Critt.BaseStat[1] = -1;
            proto->Critt.BaseStat[0] = -1;
            proto->FlgExt |= 0x0800;
            break;
        case TYPE_SCEN:
            proto->Critt.BaseStat[1] = -1;
            proto->Critt.BaseStat[0] = -1;
            proto->FlgExt |= 0x0800;
            break;
        case TYPE_WALL:
            proto->Critt.BaseStat[0] = -1;
            proto->FlgExt |= 0x0800;
            break;
        case TYPE_TILE:
            proto->Critt.BaseStat[0] = -1;
            proto->FlgExt |= 0x0800;
            break;
        default: break;
    }
    return 0;
}

int ProtoInitWall( Proto_t *proto, int Pid )
{
    int type;

    type = Pid & 0xFFFFFF;
    proto->Pid = -1;
    proto->TextID = 100 * type;
    proto->ImgId = ArtMakeId( 3, type - 1, 0, 0, 0 );
    if( !ArtFileExist( proto->ImgId ) ) proto->ImgId = ArtMakeId(3, 0, 0, 0, 0);
    proto->LtInt = 0;
    proto->Flags = 0;
    proto->FlgExt= 0x2000;
    proto->AttackMode = -1;
    proto->Critt.Type = 1;
    proto->LtRad = 0;
    return 0;
}

int ProtoInitTile( Proto_t *proto, int Pid )
{
    int type;

    type = Pid & 0xFFFFFF;
    proto->Pid = -1;
    proto->TextID = 100 * type;
    proto->ImgId = ArtMakeId(4, type - 1, 0, 0, 0);
    if( !ArtFileExist( proto->ImgId ) ) proto->ImgId = ArtMakeId(4, 0, 0, 0, 0);
    proto->LtInt = 0;
    proto->Flags = -1;
    proto->FlgExt = 1;
    proto->LtRad = 0;
    return 0;
}

int ProtoInitMisc( Proto_t *proto, int Pid )
{
    int type;

    type = Pid & 0xFFFFFF;
    proto->Pid = -1;
    proto->TextID = 100 * type;
    proto->ImgId = ArtMakeId(5, type - 1, 0, 0, 0);
    if( !ArtFileExist( proto->ImgId ) ) proto->ImgId = ArtMakeId( 5, 0, 0, 0, 0 );
    proto->LtInt = 0;
    proto->Flags = 0;
    proto->FlgExt = 0;
    proto->LtRad = 0;
    return 0;
}

int ProtoDuplicate( unsigned int Pid1, unsigned int Pid2 )
{
    Proto_t *proto2, *proto1;

    if( (Pid1 >> 24) != (Pid2 >> 24) ) return -1;
    if( ProtoGetObj( Pid1, &proto1 ) == -1 ) return -1;
    if( ProtoGetObj( Pid2, &proto2 ) == -1 ) return -1;
    memcpy(proto2, proto1, gProtoAllocSize[ Pid2 >> 24 ]);
    proto2->Pid = Pid2;
    return 0;        
}

int ProtoTestFlg( Proto_t *proto, int flg )
{
    if( flg == -1 ) return 1;
    if( OBJTYPE( proto->Pid ) ){
        if( OBJTYPE( proto->Pid ) != TYPE_SCEN ) return 0;
        if( flg == proto->Critt.Type ) return 1;
    } else if( flg == proto->Critt.Type ) {
        return 1;
    }
    return 0;
}

int ProtoDataMember( unsigned int Pid, unsigned int MembId, void **DataMember )
{
    int err;
    Proto_t *proto;

    err = 1;
    if( ProtoGetObj(Pid, &proto) == -1 ) return -1;
    switch( Pid >> 24 ){
        case TYPE_ITEM:
    	    switch( MembId ){        	
        	case 0: *DataMember = proto; return 1;
        	case 1: *DataMember = ( proto->Pid == 0x1000000 ) ? CritterGetName( gObjDude ) : ProtoGetMsg( proto->Pid, 0 ); return 2;
        	case 2: *DataMember = ProtoGetMsg( proto->Pid, 1 ); return 2;
        	case 3: *DataMember = &proto->ImgId; return 1;
        	case 4: *DataMember = &proto->LtRad; return 1; 
        	case 5: *DataMember = &proto->LtInt; return 1; 
        	case 6: *DataMember = &proto->Flags; return 1;
                case 7: *DataMember = &proto->FlgExt; return 1;
                case 8: *DataMember = &proto->AttackMode; return 1;
                case 9: *DataMember = &proto->Critt; return 1;
                case 11: *DataMember = &proto->Critt.BaseStat[18]; return 1;
                case 12: *DataMember = &proto->Critt.BaseStat[19]; return 1;
                case 13: *DataMember = &proto->Critt.BaseStat[20]; return 1;
                case 14: *DataMember = &proto->Critt.BaseStat[21]; return 1;
                case 15: *DataMember = &proto->Critt.BaseStat[22]; return 1;
                case 555:
            	    if( proto->Critt.Type == 3 ) *DataMember = &proto->Critt.BaseStat[4];
            	    return 1;
                default: eprintf( "\tError: Unimp'd data member in proto_data_member! " ); 
                return 1;
            }            
            break;
        case TYPE_CRIT:
            switch( MembId ){
                case 0: *DataMember = proto; return 1;
                case 1: *DataMember = ( proto->Pid == 0x1000000 ) ? CritterGetName( gObjDude ) : ProtoGetMsg( proto->Pid, 0 ); return 2;
                case 2: *DataMember = ProtoGetMsg( proto->Pid, 1 ); return 2;
                case 3: *DataMember = &proto->ImgId; return 1;
                case 4: *DataMember = &proto->LtRad; return 1;
                case 5: *DataMember = &proto->LtInt; return 1;
                case 6: *DataMember = &proto->Flags; return 1;
                case 7: *DataMember = &proto->FlgExt; return 1;
                case 8: *DataMember = &proto->AttackMode; return 1;
                case 10: *DataMember = &proto->i09; return 1;
                case 11: *DataMember = &proto->Critt.ProtoID; return 1;
                default: eprintf("\tError: Unimp'd data member in proto_data_member!"); return 1;
            }
        case TYPE_SCEN:
            switch( MembId ){
                case 0: *DataMember = proto; return 1;
                case 1: *DataMember = ( proto->Pid == 0x1000000 ) ? CritterGetName( gObjDude ) : ProtoGetMsg( proto->Pid, 0 ); return 2;
                case 2: *DataMember = ProtoGetMsg(proto->Pid, 1); return 2;
                case 3: *DataMember = &proto->ImgId; return 1;
                case 4: *DataMember = &proto->LtRad; return 1;
                case 5: *DataMember = &proto->LtInt; return 1;
                case 6: *DataMember = &proto->Flags; return 1;
                case 7: *DataMember = &proto->FlgExt; return 1;
                case 8: *DataMember = &proto->AttackMode; return 1;
                case 9: *DataMember = &proto->Critt; return 1;
                case 11:*DataMember = &proto->Critt.BaseStat[2]; return 1;
                default: eprintf("\tError: Unimp'd data member in proto_data_member!"); return 1;
            }
        case TYPE_WALL:
            switch( MembId ){
                case 0: *DataMember = proto; return 1;
                case 1: *DataMember = ( proto->Pid == 0x1000000 ) ? CritterGetName( gObjDude ) : ProtoGetMsg( proto->Pid, 0 ); return 2;
                case 2: *DataMember = ProtoGetMsg( proto->Pid, 1 ); return 2;
                case 3: *DataMember = &proto->ImgId; return 1;
                case 4: *DataMember = &proto->LtRad; return 1;
                case 5: *DataMember = &proto->LtInt; return 1;
                case 6: *DataMember = &proto->Flags; return 1;
                case 7: *DataMember = &proto->FlgExt; return 1;
                case 8: *DataMember = &proto->AttackMode; return 1;
                case 9: *DataMember = &proto->Critt; return 1;
                default: eprintf("\tError: Unimp'd data member in proto_data_member!"); return 1;
            }
        case TYPE_TILE: eprintf("\tError: Unimp'd data member in proto_data_member!"); return 1;
        case TYPE_MISC:
            switch( MembId ){
                case 0: *DataMember = proto; return 1;
                case 1: *DataMember = ( proto->Pid == 0x1000000 ) ? CritterGetName(gObjDude) : ProtoGetMsg( proto->Pid, 0 ); return 2;
                case 2: *DataMember = ProtoGetMsg( proto->Pid, 1 ); return 1;
                case 3: *DataMember = &proto->ImgId; return 1;
                case 4: *DataMember = &proto->LtRad; return 1;
                case 5: *DataMember = &proto->LtInt; return 1;
                case 6: *DataMember = &proto->Flags; return 1;
                case 7: *DataMember = &proto->FlgExt; return 1;
                default: eprintf( "\tError: Unimp'd data member in proto_data_member!" ); return 1;
            }
        default: return 1;
    }
    return err;
}

int ProtoInit()
{
    int i;
    char stmp[260];
    char tmp[4];
    char txt[16];
    char *PatchesPath;
    MsgLine_t MsgLine;

    memset( tmp, 0, sizeof(tmp) );
    if( !CfgGetString( &gConfiguration, "system", "master_patches", &PatchesPath ) ) return -1;
    sprintf( stmp, "%s/proto", PatchesPath );
    xDirCreate( stmp );
    strcpy( stmp + strlen( stmp ), "/critters" ); 
    xDirCreate( stmp );
    strcpy( stmp + strlen( stmp ), "/items" ); 
    xDirCreate( stmp );

    ProtoInitCritter( &gProtoDude, PROTO_1 );
    gProtoDude.Pid = PROTO_1;

    gProtoImgId = ArtMakeId( 1, 1, 0, 0, 0 );
    gObjDude->Pid = PROTO_1;
    gObjDude->ScrId = -1;
    for( i = 0; i < 6; i++ ) ProtoFreeCategory( i );
    ProtoObjCount();
    gProtoUnk02 = 1;
    ProtoDudeInit( "premade/player.gcd" );

    for( i = 0; i < 6; i++ ){
        if( MessageInit( &gProtoMsg[ i ] ) != 1 ){ eprintf( "Error: Initing proto message files!" ); return -1; }
    }

    for( i = 0; i < 6; i++ ){
        strcpy( txt, ArtGetCatDirName( i ) );
        txt[ 4 ] = '\0';
        sprintf( stmp, "%spro_%4s%s", gGamePath, txt, ".msg" );
        if( MessageLoad( &gProtoMsg[ i ], stmp ) != 1 ){ eprintf( "Error: Loading proto message files!" ); return -1; }
    }

    gProtoStatNames[ 0 ] = "Drug Stat (Special)";
    gProtoStatNames[ 1 ] = "None";
    for( i = 0; i < 38; i++ ){
        if( !( gProtoStatNames[ 2 + i ] = FeatGetName( i ) ) ){ eprintf("Error: Finding stat names!"); return -1; }
    }

    gProtoPerkNames[0] = "None";        
    for( i = 0; i < 119; i++ ){        
        if( !(gProtoPerkNames[ 1 + i ] = PerkGetName(i)) ){ eprintf("Error: Finding perk names!"); return -1; }
    }
    if( MessageInit( &gProtoMessages ) != 1 ){ eprintf( "Error: Initing main proto message file!" ); return -1; }
    sprintf( stmp, "%sproto.msg", gGamePath );

    if( MessageLoad( &gProtoMessages, stmp ) != 1 ){ eprintf("Error: Loading main proto message file!"); return -1; }
    gProtoUnk100[0] = MessageGetMessage( &gProtoMessages, &MsgLine, 10 );
    for( i = 0; i < 8; i++ ){
        gProtoUnk106[ i ] = MessageGetMessage( &gProtoMessages, &MsgLine, 100 + i );
    }
    for( i = 0; i < 7; i++ ){
        gProtoUnk101[ i ] = MessageGetMessage( &gProtoMessages, &MsgLine, 150 + i );
    }
    for( i = 0; i != 6; i++ ){
        gProtoUnk102[ i ] = MessageGetMessage( &gProtoMessages, &MsgLine, 200 + i );
    }
    for( i =0; i < 7; i++ ){
        gProtoUnk103[ i ] = MessageGetMessage( &gProtoMessages, &MsgLine, 250 + i );
    }
    for( i = 0; i < 19; i++ ){
        gProtoUnk104[ i ] = MessageGetMessage( &gProtoMessages, &MsgLine, 300 + i );
    }
    for( i = 0; i < 2; i++ ){
        gProtoUnk105[ i ] = MessageGetMessage( &gProtoMessages, &MsgLine, 350 + i );
    }
    for( i = 0; i < 3; i++ ){
        gProtoUnk100[ i + 1 ] = MessageGetMessage( &gProtoMessages, &MsgLine, 400 + i );
    }
    return 0;
}

int ProtoFreePremade()
{
    int i;

    ProtoInitCritter( &gProtoDude, PROTO_1 );
    gProtoDude.Pid = PROTO_1;
    gProtoDude.ImgId = ArtMakeId( 1, 1, 0, 0, 0 );
    gObjDude->Pid = PROTO_1;
    gObjDude->ScrId = -1;
    gObjDude->Flags &= ~0xFC000;
    for( i = 0; i < 6; i++ ) ProtoFreeCategory( i );
    ProtoObjCount();
    gProtoUnk02 = 1;
    ProtoDudeInit( "premade/player.gcd" );
    return 0;
}

void ProtoClose()
{
    int i;

    for( i = 0; i < 6; i++ ) ProtoFreeCategory( i );
    gProtoUnk02 = 0;
    for( i = 0; i < 6; i++ ) MessageClose( &gProtoMsg[ i ] );
    MessageClose( &gProtoMessages );
}

int ProtoObjCount()
{
    xFile_t *fh;
    int d;
    unsigned int i;
    char stmp[ 260 ];

    for( i = 0; i < 6; i++ ){
        gProtoObjLib[ i ].ObjRoot = 0;
        gProtoObjLib[ i ].Current = 0;
        gProtoObjLib[ i ].Cnt = 0;
        gProtoObjLib[ i ].Active = 1;
        strcpy( stmp, gProtoDataFilePath );
        strcpy( stmp + strlen( stmp ), gProtoFilePath );
        strcpy( stmp + strlen( stmp ), ArtGetCatDirName( i ) );
        strcpy( stmp + strlen( stmp ), "/" );
        strcpy( stmp + strlen( stmp ), ArtGetCatDirName( i ) );
        strcpy( stmp + strlen( stmp ), ".lst" );

        if( !(fh = dbOpen( stmp, "rt") ) ) return -1;
        while( (d = dbgetc( fh ) ) != -1 ){
    	    if( d == '\n' ) gProtoObjLib[ i ].Active++;
        }
        if( d != '\n' ) gProtoObjLib[ i ].Active++;
        dbClose( fh );        
    }
    return 0;
}

int ProtoLoadItems( ProtoItemsData_t *Data, int Type, xFile_t *fh )
{
    unsigned int tmp1, tmp2, tmp3;

    switch( Type ){
        case 0:
            if( dbgetBei(fh, (int *)&Data->Type0.i01) == -1 || 
        	dbreadBeiBlk(fh, Data->Type0.i02, 7) == -1 || 
        	dbreadBeiBlk(fh, Data->Type0.i03, 7) == -1 || 
        	dbgetBei(fh, (int *)&tmp3) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type0.i05) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type0.i06) == -1 )
            { return -1; }
            Data->Type0.i04 = tmp3;
            return 0;
        case 1:
            if( dbgetBei(fh, (int *)&Data->Type1.i01) == -1 || dbgetBei(fh, (int *)&Data->Type1.i02) == -1 ) return -1;
            return 0;          
        case 2:
            if( dbgetBei(fh, (int *)&tmp3) == -1 || 
        	dbgetBei(fh, (int *)&tmp2) == -1 || 
        	dbgetBei(fh, (int *)&tmp1) == -1 || 
        	dbreadBeiBlk(fh, Data->Type2.i04, 3) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type2.i05) == -1 || 
        	dbreadBeiBlk(fh, Data->Type2.i06, 3) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type2.i07) == -1 || 
        	dbreadBeiBlk(fh, Data->Type2.i08, 3) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type0.i03[6]) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type0.i04) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type2.i11) == -1 )
            { return -1; }
            Data->Type2.i01 = tmp3;
            Data->Type2.i02 = tmp2;
            Data->Type2.i03 = tmp1;
            return 0;
        case 3:
            if( dbgetBei(fh, (int *)&Data->Type3.i01) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type3.i02) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type3.i03) == -1 || 
        	dbgetBei(fh, (int *)&tmp3) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type0.i02[3]) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type0.i02[4]) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type3.i07) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type0.i02[6]) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type0.i03) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type0.i03[1]) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type3.i11) == -1 || 
        	dbgetBei(fh, (int *)&tmp2) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type3.i13) == -1 || 
        	dbgetBei(fh, (int *)&tmp1) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type3.i15) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type0.i04) == -1 || 
        	dbgetb(fh, (char *)&Data->Type3.b17) == -1 )
            { return -1; }
            Data->Type3.i04 = tmp3;
            Data->Type3.i12 = tmp2;
            Data->Type3.i14 = tmp1;
            return 0;
        case 4:
            if( dbgetBei(fh, (int *)&tmp3) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type4.i02) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type4.i03) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type4.i04) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type4.i05) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type4.i06) == -1 )
            { return -1; }
            Data->Type4.i01 = tmp3;
            return 0;
        case 5:
            if( dbgetBei(fh, (int *)&Data->Type5.i01) == -1 ||
        	dbgetBei(fh, (int *)&Data->Type5.i02) == -1 || 
        	dbgetBei(fh, (int *)&Data->Type5.i03) == -1 )
            { return -1; }
    	    return 0;
        case 6:
            if( dbgetBei(fh, (int *)&Data->Type6.i01 ) == -1 ) return -1;
            return 0;            
    }
    return 0;
}

int ProtoLoadScenery( int *Data, int Type, xFile_t *fh )
{
    switch( Type ){
        case PR_SCN_DOOR:
            if( dbgetBei( fh, &Data[0] ) == -1 ) return -1; // 29 Walk thru flags 0x0f -> yes
            if( dbgetBei( fh, &Data[1] ) == -1 ) return -1; // 2D unk
            return 0;            
        case PR_SCN_STAIRS:
            if( dbgetBei( fh, &Data[0] ) == -1 ) return -1; // 29 dest tile
            if( dbgetBei( fh, &Data[1] ) == -1 ) return -1; // 2d dest map
            return 0;
        case PR_SCN_ELEVATOR:
            if( dbgetBei( fh, &Data[0] ) == -1 ) return -1; // 29 elev type
            if( dbgetBei( fh, &Data[1] ) == -1 ) return -1; // 2d elev level
            return 0;
        case PR_SCN_LADDER_BOT:
            if( dbgetBei( fh, &Data[0] ) == -1 ) return -1; // 29 dest tile
            return 0;            
        case PR_SCN_LADDER_TOP:
            if( dbgetBei( fh, &Data[0] ) == -1 ) return -1; // 29 dest tile
            return 0;            
        case 5:
            if( dbgetBei( fh, &Data[0] ) == -1 ) return -1; // 29 unk
            return 0;            
    }
    return 0;
}

int ProtoLoadPrototype( Proto_t *Pt, xFile_t *fh)
{
    char c;
    int tmp1;

    /* *.PRO header */
    if( dbgetBei( fh, &Pt->Pid ) == -1 ) return -1;	// 00
    if( dbgetBei( fh, &Pt->TextID ) == -1 ) return -1;	// 04
    if( dbgetBei( fh, &Pt->ImgId ) == -1 ) return -1;	// 08
    switch( OBJTYPE( Pt->Pid ) ){
        case TYPE_ITEM:
            if( dbgetBei( fh, &Pt->LtRad ) == -1 ) return -1;  		// 0C Light radius
    	    if( dbgetBei( fh, &Pt->LtInt ) == -1 ) return -1;  		// 10 Light intensity
    	    if( dbgetBei( fh, &Pt->Flags ) == -1 ) return -1;  		// 14 Flags PRFLG_*
    	    if( dbgetBei( fh, &Pt->FlgExt ) == -1 ) return -1; 		// 18 PR_ITEM_FLG
    	    if( dbgetBei( fh, &Pt->AttackMode ) == -1 ) return -1;	// 1B attack modes
    	    if( dbgetBei( fh, &Pt->Critt.ScrId ) == -1 ) return -1;	// 1C Script ID
    	    if( dbgetBei( fh, &Pt->Critt.BaseStat[18] ) == -1 ) return -1; // 20 Obj sub type: PR_ITEM_*
    	    if( dbgetBei( fh, &Pt->Critt.BaseStat[19]) == -1 ) return -1;  // 24 Material ID
    	    if( dbgetBei( fh, &Pt->Critt.BaseStat[20]) == -1 ) return -1;  // 28 Size
    	    if( dbgetBei( fh, &Pt->Critt.BaseStat[21]) == -1 ) return -1;  // 2C Weight
    	    if( dbgetBei( fh, &Pt->Critt.BaseStat[22]) == -1 ) return -1;  // 30 Cost
    	    // ??Art ID ?
    	    if( dbgetb( fh, (char *)&Pt->Critt.BaseStat[23]) == -1 ) return -1; // sound ?
            if( ProtoLoadItems( (void *)Pt->Critt.BaseStat, Pt->Critt.ScrId, fh) ) return -1; 
            return 0;    	    
        case TYPE_CRIT:
            if ( dbgetBei(fh, &Pt->LtRad) == -1 || dbgetBei(fh, &Pt->LtInt) == -1 || dbgetBei(fh, &Pt->Flags) == -1 || dbgetBei(fh, &Pt->FlgExt) == -1 || dbgetBei(fh, &Pt->AttackMode) == -1 || dbgetBei(fh, &Pt->i09) == -1 || dbgetBei(fh, &Pt->AiPacketNum) == -1 || dbgetBei(fh, &Pt->i11) == -1 )
            { return -1; }
            if( CritterLoadFile(fh, &Pt->Critt) == -1 ) return -1;
            return 0;
        case TYPE_SCEN:
            if( dbgetBei( fh, &Pt->LtRad ) == -1 ) return -1; // 0c
    	    if( dbgetBei( fh, &Pt->LtInt ) == -1 ) return -1; // 10 
    	    if( dbgetBei( fh, &Pt->Flags ) == -1 ) return -1; // 14 [ 18 ] wall light type flags
    	    if( dbgetBei( fh, &Pt->FlgExt ) == -1 ) return -1; // 18 wall type flags [ 1a ] action flags
    	    if( dbgetBei( fh, &Pt->AttackMode ) == -1 ) return -1; // 1a action flags [1c] script
    	    if( dbgetBei( fh, &Pt->Critt.Type ) == -1 ) return -1; // 1c script [20] scenery sub type
    	    if( dbgetBei( fh, &Pt->Critt.BaseStat[2] ) == -1 ) return -1; // 20 [24] Material Id
    	    if( dbgetb( fh, &c ) == -1 ) return -1; // 21 [ 28 ] Sound
    	    Pt->Critt.BaseStat[4] = c;
            if( ProtoLoadScenery((int *)Pt->Critt.BaseStat, Pt->Critt.Type, fh) == -1 ) return -1; // 29/2d
            return 0;            
        case TYPE_WALL:
            if( 
        	dbgetBei(fh, &Pt->LtRad) != -1 && 
        	dbgetBei(fh, &Pt->LtInt) != -1 && 
        	dbgetBei(fh, &Pt->Flags) != -1 && 
        	dbgetBei(fh, &Pt->FlgExt) != -1 && 
        	dbgetBei(fh, &Pt->AttackMode) != -1 && 
        	dbgetBei(fh, (int *)&tmp1) != -1 )
            {
                Pt->Critt.Type = tmp1;
                return 0;
            }
            return -1;
        case TYPE_TILE:
            if ( dbgetBei(fh, &Pt->LtRad) == -1 || dbgetBei(fh, &Pt->LtInt) == -1 || dbgetBei(fh, &Pt->Flags) == -1 || dbgetBei(fh, (int *)&tmp1) == -1 ) return -1;            
            Pt->FlgExt = tmp1;
            return 0;
        case TYPE_MISC:
            if( dbgetBei(fh, &Pt->LtRad) != -1 && dbgetBei(fh, &Pt->LtInt) != -1 && dbgetBei(fh, &Pt->Flags) != -1 && dbgetBei(fh, &Pt->FlgExt) != -1 ) return 0;
            return -1;
    }
    return 0;
}

int ProtoSaveItems( ProtoItemsData_t *it, int Type, xFile_t *fh )
{
    switch( Type ){
        case 0:
            if( dbputBei(fh, it->Type0.i01) != -1 && 
        	dbputBeiBlk(fh, (unsigned int *)it->Type0.i02, 7) != -1 && 
        	dbputBeiBlk(fh, (unsigned int *)it->Type0.i03, 7) != -1 && 
        	dbputBei(fh, it->Type0.i04) != -1 && 
        	dbputBei(fh, it->Type0.i05) != -1 && 
        	dbputBei(fh, it->Type0.i06) != -1 
    	      ) return 0;
            return -1;
        case 1:
            if( dbputBei(fh, it->Type1.i01) != -1 && dbputBei(fh, it->Type1.i02) != -1 ) return 0;
            return -1;
        case 2:
            if( dbputBeiBlk(fh, (unsigned int *)it, 3) != -1 && 
        	dbputBeiBlk(fh, (unsigned int *)it->Type2.i04, 3) != -1 && 
        	dbputBei(fh, it->Type2.i05) != -1 && 
        	dbputBeiBlk(fh, (unsigned int *)it->Type2.i06, 3) != -1 && 
        	dbputBei(fh, it->Type2.i07) != -1 && 
        	dbputBeiBlk(fh, (unsigned int *)it->Type2.i08, 3) != -1 && 
        	dbputBei(fh, it->Type2.i09) != -1 && 
        	dbputBei(fh, it->Type2.i10) != -1 && 
        	dbputBei(fh, it->Type2.i11) != -1 
              ) return 0;
              return -1;
        case 3:
            if( dbputBei(fh, it->Type3.i01) != -1 && 
        	dbputBei(fh, it->Type3.i02) != -1 && 
        	dbputBei(fh, it->Type3.i03) != -1 && 
        	dbputBei(fh, it->Type3.i04) != -1 && 
        	dbputBei(fh, it->Type3.i05) != -1 && 
        	dbputBei(fh, it->Type3.i06) != -1 && 
        	dbputBei(fh, it->Type3.i07) != -1 && 
        	dbputBei(fh, it->Type3.i08) != -1 && 
        	dbputBei(fh, it->Type3.i09) != -1 && 
        	dbputBei(fh, it->Type3.i10) != -1 && 
        	dbputBei(fh, it->Type3.i11) != -1 && 
        	dbputBei(fh, it->Type3.i12) != -1 && 
        	dbputBei(fh, it->Type3.i13) != -1 && 
        	dbputBei(fh, it->Type3.i14) != -1 && 
        	dbputBei(fh, it->Type3.i15) != -1 && 
        	dbputBei(fh, it->Type3.i16) != -1 && 
        	dbputb(fh, it->Type3.b17) != -1 
    	       ) return 0;
            return -1;
        case 4:
            if( dbputBei(fh, it->Type4.i01) != -1 && 
        	dbputBei(fh, it->Type4.i02) != -1 && 
        	dbputBei(fh, it->Type4.i03) != -1 && 
        	dbputBei(fh, it->Type4.i04) != -1 && 
        	dbputBei(fh, it->Type4.i05) != -1 && 
        	dbputBei(fh, it->Type4.i06) != -1 
               ) return 0;
             return -1;
        case 5:
            if( dbputBei(fh, it->Type5.i01) != -1 && 
        	dbputBei(fh, it->Type5.i02) != -1 && 
        	dbputBei(fh, it->Type5.i03) != -1 
              ) return 0;                 
            return -1;
        case 6:
            if( dbputBei(fh, it->Type6.i01) != -1 ) return 0;
            return -1;
    }
    return 0;
}

int ProtoSaveScenery( ProtoSceneryData_t *sc, int Type, xFile_t *fh )
{
    switch( Type ){
        case 0: return ( dbputBei(fh, sc->i01) != -1 && dbputBei(fh, sc->i02) != -1 ) ? 0 : -1;
        case 1: return ( dbputBei(fh, sc->i01) != -1 && dbputBei(fh, sc->i02) != -1 ) ? 0 : -1;
        case 2: return ( dbputBei(fh, sc->i01) != -1 && dbputBei(fh, sc->i02) != -1 ) ? 0 : -1;
        case 3: return ( dbputBei(fh, sc->i01) != -1 ) ? 0 : -1;
        case 4: return ( dbputBei(fh, sc->i01) != -1 ) ? 0 : -1;
        case 5: return ( dbputBei(fh, sc->i01) != -1 ) ? 0 : -1;
    }
    return 0;
}

int ProtoSaveCritter( Proto_t *pt, xFile_t *fh )
{
    if( dbputBei(fh, pt->Pid) == -1 || dbputBei(fh, pt->TextID) == -1 || dbputBei(fh, pt->ImgId) == -1 ) return -1;
    switch( (pt->Pid >> 24) ){
        case 0:
            if( dbputBei(fh, pt->LtRad) != -1 && 
        	dbputLei(fh, pt->LtInt) != -1 && 
        	dbputBei(fh, pt->Flags) != -1 && 
        	dbputBei(fh, pt->FlgExt) != -1 && 
        	dbputBei(fh, pt->AttackMode) != -1 && 
        	dbputBei(fh, pt->Critt.Type) != -1 && 
        	dbputBei(fh, pt->Critt.BaseStat[18]) != -1 && 
        	dbputBei(fh, pt->Critt.BaseStat[19]) != -1 && 
        	dbputLei(fh, pt->Critt.BaseStat[20]) != -1 && 
        	dbputBei(fh, pt->Critt.BaseStat[21]) != -1 && 
        	dbputBei(fh, pt->Critt.BaseStat[22]) != -1 && 
        	dbputb(fh, pt->Critt.BaseStat[23]) != -1 && !ProtoSaveItems((ProtoItemsData_t *)pt->Critt.BaseStat, pt->Critt.Type, fh) )
                return 0;
    	    return -1;
        case 1:
            if( dbputBei(fh, pt->LtRad) != -1 && 
        	dbputLei(fh, pt->LtInt) != -1 && 
        	dbputBei(fh, pt->Flags) != -1 && 
        	dbputBei(fh, pt->FlgExt) != -1 && 
        	dbputBei(fh, pt->AttackMode) != -1 && 
        	dbputBei(fh, pt->i09) != -1 && 
        	dbputBei(fh, pt->AiPacketNum) != -1 && 
        	dbputBei(fh, pt->i11) != -1 && CritterSaveFile(fh, &pt->Critt) != -1 )
                return 0;
    	    return -1;
        case 2:
            if( dbputBei(fh, pt->LtRad) != -1 && 
        	dbputLei(fh, pt->LtInt) != -1 && 
        	dbputBei(fh, pt->Flags) != -1 && 
        	dbputBei(fh, pt->FlgExt) != -1 && 
        	dbputBei(fh, pt->AttackMode) != -1 && 
        	dbputBei(fh, pt->Critt.Type) != -1 && 
        	dbputBei(fh, pt->Critt.BaseStat[2]) != -1 && 
        	dbputb(fh, pt->Critt.BaseStat[4]) != -1 && ProtoSaveScenery((ProtoSceneryData_t *)pt->Critt.BaseStat, pt->Critt.Type, fh) != -1 )
                return 0;
    	    return -1;
        case 3:
            if( dbputBei(fh, pt->LtRad) != -1 && 
        	dbputLei(fh, pt->LtInt) != -1 && 
        	dbputBei(fh, pt->Flags) != -1 && 
        	dbputBei(fh, pt->FlgExt) != -1 && 
        	dbputBei(fh, pt->AttackMode) != -1 && 
        	dbputBei(fh, pt->Critt.Type) != -1 ) return 0;
    	    return -1;
        case 4:
            if( dbputBei(fh, pt->LtRad) != -1 && 
        	dbputBei(fh, pt->LtInt) != -1 && 
        	dbputBei(fh, pt->Flags) != -1 && 
        	dbputBei(fh, pt->FlgExt) != -1 ) return 0;
    	    return -1;
        case 5:
            if( dbputBei(fh, pt->LtRad) != -1 && 
        	dbputLei(fh, pt->LtInt) != -1 &&
        	dbputBei(fh, pt->Flags) != -1 && 
        	dbputBei(fh, pt->FlgExt) != -1 ) return 0;
    	    return -1;
    }
    return 0;
}

void ProtoSaveCritterFile( unsigned int Pid )
{
    unsigned int n;
    xFile_t *fh;
    char stmp[260];
    Proto_t *proto;

    if( ProtoGetObj( Pid, &proto ) == -1 ) return;
    strcpy( stmp, gProtoDataFilePath );
    strcpy( &stmp[strlen(stmp)], gProtoFilePath );
    if( Pid != -1 ) strcpy( &stmp[strlen(stmp)], ArtGetCatDirName( Pid >> 24  ) );
    n = strlen(stmp) + 1;
    stmp[n - 1] = '/';
    ProtoGetFName( Pid, &stmp[n] );        
    if( !(fh = dbOpen(stmp, "wb")) ) return;
    ProtoSaveCritter( proto, fh );
    dbClose( fh );
}

int ProtoLoadCritterFile( unsigned int Pid, Proto_t **Pt )
{
    unsigned int len;
    int err;
    xFile_t *fh;
    char fname[512];

    strcpy( fname, gProtoDataFilePath );
    strcpy( fname + strlen( fname ), gProtoFilePath );
    err = 0;
    if( Pid != -1 ) strcpy( fname + strlen( fname ), ArtGetCatDirName( OBJTYPE( Pid ) ) );
    len = strlen( fname ) + 1;
    fname[ len - 1 ] = '/';
    if( ProtoGetFName( Pid, &fname[ len ] ) == -1 ) return -1;
    if( !(fh = dbOpen(fname, "rb")) ){ eprintf("Error: Can't fopen proto! '%s'\n", fname); *Pt = 0; return -1; }
    if( ProtoAlloc( OBJTYPE( Pid ), (void **)Pt ) == -1 ){ dbClose(fh); return -1; }
    err = 0;
    if( ProtoLoadPrototype( *Pt, fh ) ) err =-1;
    dbClose( fh );
    return err;
}

int ProtoAlloc( unsigned int Type, void **pObj )
{
    ProtoLib_t *pt;
    ProtoObj_t *obj, *pnew;

    if( !(*pObj = (ProtoObj_t *)Malloc( ( Type < 11 ) ? gProtoAllocSize[ Type ] : 0 ) ) ) return -1;
    pt = &gProtoObjLib[ Type ];
    obj = pt->Current;
    if( pt->ObjRoot ){
        if( obj->Count == 16 ){
            pnew = (ProtoObj_t *)Malloc( sizeof( ProtoObj_t ) );
            obj->next = pnew;
            if( !pnew ){ Free( *pObj ); *pObj = NULL; return -1; }
            pnew->Count = 0;
            pnew->next = NULL;
            pt->Current = pnew;
            pt->Cnt++;
            obj = pnew;
        }
    } else {
        obj = (ProtoObj_t *)Malloc( sizeof( ProtoObj_t ) );
        if( !obj ){ Free( *pObj ); *pObj = NULL; return -1; }
        obj->next = NULL;
        obj->Count = 0;
        pt->Cnt = 1;
        pt->Current = obj;
        pt->ObjRoot = obj;
    }
    obj->Objs[ obj->Count ] = *pObj;
    obj->Count++;
    return 0;
}

int ProtoCreate( int *pPid, ProtoType_e Type )
{
    Proto_t *proto;

    if( ProtoAlloc( Type, (void **)&proto ) != -1 ) return -1;
    *pPid = ProtoInc( Type ) | Type;
    switch( Type ){
        case TYPE_ITEM: ProtoInitItem( proto, *pPid ); break;
        case TYPE_CRIT: ProtoInitCritter( proto, *pPid ); break;
        case TYPE_SCEN: ProtoInitScenery( proto, *pPid ); break;
        case TYPE_WALL: ProtoInitWall( proto, *pPid ); break;
        case TYPE_TILE: ProtoInitTile( proto, *pPid ); break;
        case TYPE_MISC: ProtoInitMisc( proto, *pPid ); break;
        default: return -1;
    }
    proto->Pid = *pPid;
    return 0;
}

unsigned int ProtoRemove( unsigned int Pid )
{
    int i;
    ProtoLib_t *obj;
    ProtoObj_t *p, *Current;

    if( Pid == -1 ) return -1;
    obj = &gProtoObjLib[ Pid >> 24 ];
    for( p = obj->ObjRoot; p; p = p->next ){
        for( i = 0; i < p->Count; i++ ){
            if( Pid == p->Objs[ i ]->Pid ) break;
        }
        if( i < p->Count ) break;
    }
    if( !p ) return -1;
    Free( p->Objs[ i ] );
    p->Objs[ i ] = NULL;
    
    Current = obj->Current;
    if( p == Current ){
        if( (i + 1) == Current->Count ){
            Current->Count--;
            if( !Current->Count ){
                obj->Cnt--;
                Free( Current );
                if( obj->Cnt ){
                    for( p = obj->ObjRoot; obj->Current != p->next; p = p->next );
                    p->next = NULL;
                    obj->Current = p;
                } else {
                    obj->ObjRoot = NULL;
                    obj->Current = NULL;
                }
            }
        }
    } else {
	p = obj->Current;
	p->Count--;
	p->Objs[ i ] = obj->Current->Objs[ p->Count ];
	p = obj->Current;
	if( p->Count ){
    	    p->Objs[ p->Count ] = NULL;
	} else {
    	    obj->Cnt--;
    	    Free( obj->Current );
    	    for( p = obj->ObjRoot; obj->Current != p->next; p = p->next );
    	    p->next = NULL;
    	    obj->Current = p;
	}
    }
    return 0;
}

void ProtoFreeBlock( int Type )
{
    ProtoObj_t *p;
    int i;

    if( !(p = gProtoObjLib[ Type ].ObjRoot) ) return;    
    gProtoObjLib[ Type ].Cnt--;
    gProtoObjLib[ Type ].ObjRoot = gProtoObjLib[ Type ].ObjRoot->next;
    for( i = 0; i < p->Count; i++ ) Free( p->Objs[ i ] );
    Free( p );
}

int ProtoFreeCategory( int Cat )
{
    ProtoObj_t *p, *tmp;
    int i;

    for( p = gProtoObjLib[ Cat ].ObjRoot; p; ){
        tmp = p->next;
        for( i = 0; i < p->Count; i++ ) Free( p->Objs[ i ] );
        Free( p );
        p = tmp;
    }
    gProtoObjLib[ Cat ].Current = NULL;
    gProtoObjLib[ Cat ].Cnt = 0;
    gProtoObjLib[ Cat ].ObjRoot = NULL;
    return 0;
}

void ProtoResetTypes()
{
    int i;
    for( i = 0; i < 6; i++ )  ProtoFreeCategory(i);
}

int ProtoGetObj( int Pid, Proto_t **pObj )
{
    int i;
    ProtoObj_t *p;
    int id;

    *pObj = NULL;
    if( Pid == -1 ) return -1;
    if( Pid == PROTO_1 ) {
        *pObj = &gProtoDude;
        return 0;
    }        
    for( p = gProtoObjLib[ OBJTYPE( Pid ) ].ObjRoot; p; p = p->next ){        
        for( i = 0; i < p->Count; i++ ){
            if( p->Objs[ i ]->Pid == Pid ) break;
        }
        if( i < p->Count ) break;
    }
    if( !p ){ // no objs found in library -> load and add to library
	id = OBJTYPE( Pid );
	if( gProtoObjLib[ id ].ObjRoot ){
    	    if( (p = gProtoObjLib[ id ].Current ) ){
        	if( gProtoObjLib[ id ].Cnt + p->Count - 1 > 32 ){
            	    eprintf( "^" );
            	    ProtoFreeBlock( id );
        	}
    	    }
	}
	return ProtoLoadCritterFile( Pid, pObj );    
    } 
    *pObj = p->Objs[ i ];
    return 0;
}

int ProtoInc( int Type )
{
    int x;

    x = gProtoObjLib[ Type ].Active;
    gProtoObjLib[ Type ].Active = x + 1;
    return x;
}

int ProtoDec( int Type )
{
    int x;

    x = gProtoObjLib[ Type ].Active;
    gProtoObjLib[ Type ].Active = x - 1;
    return x;
}

int ProtoActive( int Type )
{
    return gProtoObjLib[ Type ].Active;
}

int ProtoFindArtMatch( int Pid )
{
    int err,flg,Type,i,Active;
    Proto_t *proto2;

    err = -1;
    flg = 0;
    if( Pid == -1 ) return -1;    
    Type = (Pid & 0xF000000) >> 24;
    Active = gProtoObjLib[Type].Active;
    for( i = 1; i < Active; i++ ){
	if( ProtoGetObj( i | (Type << 24), &proto2 ) == -1 ){    
	    eprintf( "Error: proto_find_art_match: proto_ptr failed!" );
	    return -1;
	}
	if( Pid == proto2->ImgId ){
            if( flg ){
                eprintf("Error: Found duplicate fid in second prototype!");
            } else {
                flg = 1;
                err = Pid;
            }
        }
    }
    return err;
}

int ProtoReset()
{
    Critter_t *Crit;
    Proto_t *proto;

    ProtoGetObj( gObjDude->Pid, &proto );
    Crit = &proto->Critt;
    FeatPtsReset();
    FeatReset( Crit );
    CritterReset();
    CharEditStartChar();
    SkillReset( Crit );
    SkillClear();
    PerkZero();
    TraitSpecReset();
    FeatStatsRecalculate( gObjDude );
    return 0;
}



