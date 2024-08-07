#include "FrameWork.h"

Msg_t gCrMsg;
char gCrName[ 32 ];
int  gCrUnk02;
int  gCrKillStats[ 19 ];

char *gCrName_ = "corpse";
int gCrUnk09;

Obj_t *gCrUnk08 = NULL;

static int gCrCondFeats[ 8 ] =  { 0, 1, 2, 3, 4, 5, 35, 14 };
static int gCrUnk13[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, -1, 0, -3, -2, 0, -1, 0, 0, -2, -5, -5,
    -4, -3, -3, -3, -1, -5, -15, -10, -6, -5, -5, -5,
    -3, -6, -20, -10
};


/*****************************/

int CritterInit()
{
    char path[ 260 ];

    CritterNameInit();
    memset( gCrKillStats, 0, sizeof( gCrKillStats ) );
    if( MessageInit( &gCrMsg ) != 1 ){ eprintf("Error: Initing critter name message file!"); return -1; }    
    sprintf( path, "%sscrname.msg", gGamePath );
    if( MessageLoad( &gCrMsg, path ) != 1 ) { eprintf( "Error: Loading critter name message file!" ); return -1;}
    return 0;    
}

void CritterReset()
{
    CritterNameInit();
    memset( gCrKillStats, 0, sizeof( gCrKillStats ) );
}

void CritterClose()
{
    MessageClose( &gCrMsg );
}

int CritterLoad( xFile_t *fh )
{
    Proto_t *proto;

    if( dbgetBei( fh, &gCrUnk02 ) == -1 ) return -1;
    ProtoGetObj( gObjDude->Pid, &proto );
    return CritterLoadFile( fh, &proto->Critt );    
}

int CritterSave( xFile_t *fh )
{
    Proto_t *proto;

    if( dbputBei(fh, gCrUnk02) == -1 ) return -1;
    ProtoGetObj( gObjDude->Pid, &proto );
    return CritterSaveFile( fh, &proto->Critt );
}

void CritterDuplicate( Critter_t *p1, Critter_t *p2 )
{
    memcpy( p1, p2, sizeof( Critter_t ) );//372
}

char *CritterGetName( Obj_t *dude )
{
    MsgLine_t MsgLine;
    Scpt_t *scr;
    char *s;

    if( dude == gObjDude ) return gCrName;
    if( dude->ScrFNameId == -1 && dude->ScrId != -1 ){
        if( ScptPtr( dude->ScrId, &scr ) != dude->ScrFNameId ) dude->ScrFNameId = scr->LocVarId;
    }
    if( dude->ScrFNameId == -1 ){
	s = ProtoGetObjName( dude->Pid );
    } else {
	MsgLine.Id = 101 + dude->ScrFNameId;
	if( MessageGetMsg( &gCrMsg, &MsgLine ) != 1 ){
	    s = ProtoGetObjName( dude->Pid );
	} else {
	    s = MsgLine.Text;
	    if( !*s ) s = ProtoGetObjName( dude->Pid );	
	}
    }
    gCrName_ = s;
    return gCrName_;
}

int CritterSetName( char *name )
{
    if( strlen( name ) > 32 ) return -1;
    strncpy( gCrName, name, 32 );
    return 0;
}

void CritterNameInit()
{
    strncpy( gCrName, "None", 32 );
}

int CritterGetHp( Obj_t *dude )
{
    if( (dude->Pid >> 24) == 1 ) return dude->Critter.HitPts;
    return 0;
}

int CritterHeal( Obj_t *dude, int dmg )
{
    int hp;

    if( OBJTYPE( dude->Pid ) != TYPE_CRIT ) return 0;
    hp = FeatGetVal( dude, FEAT_HP );
    dude->Critter.HitPts += dmg;
    if( hp >= dude->Critter.HitPts ){
        if( dude->Critter.HitPts <= 0 && dude->Critter.State.CombatResult >= 0 ) CritterKill( dude, -1, 1 );
    } else {
        dude->Critter.HitPts = hp;
    }    
    return 0;
}

int CritterPoisoned( Obj_t *dude )
{
    if( (dude->Pid >> 24) == 1 ) return dude->Critter.Poisoned;
    return 0;
}

int CritterPoisonInc( Obj_t *dude, int val ) // poison
{
    MsgLine_t MsgLine;

    if( dude != gObjDude ) return 0;
    if( val <= 0 ){
        if( gObjDude->Critter.Poisoned <= 0 ) return 0;
    } else {
        val -= FeatGetVal( dude, FEAT_PSNRES ) * val / 100;
    }
    gObjDude->Critter.Poisoned += val;
    if( gObjDude->Critter.Poisoned > 0 ){
        EvQeRun( 5, 0 );
        EvQeSchedule( 10 * (505 - 5 * gObjDude->Critter.Poisoned), gObjDude, 0, EV_POISON_TIMER );
        MsgLine.Id = 3000; // 'you have been poisoned'
        if( val < 0 ) MsgLine.Id = 3002; // 'you fill a little better'
        if( MessageGetMsg( &gMessage, &MsgLine ) == 1 ) IfcMsgOut( MsgLine.Text );
    } else {
	gObjDude->Critter.Poisoned = 0;
	MsgLine.Id = 3003; // 'you fill better'
	if( MessageGetMsg( &gMessage, &MsgLine ) == 1 ) IfcMsgOut( MsgLine.Text );
    }
    if( dude == gObjDude ) IfaceIndicatorBoxUpdate();
    return 0;
}

int CritterPoisonEv( Obj_t *dude, MsgLine_t *MsgLine )
{
    if( dude != gObjDude ) return 0;
    CritterPoisonInc( dude, -2 );
    CritterHeal( dude, -1 );
    IfaceRenderHP( 0 );
    if( MessageGetMsg( &gMessage, MsgLine ) == 1 ) IfcMsgOut( MsgLine->Text );
    return (OBJTYPE( dude->Pid ) == 1 ? dude->Critter.HitPts : 0 ) <= 5;
}

int CritterRadiated( Obj_t *dude )
{
    if( OBJTYPE( dude->Pid ) == 1 ) return dude->Critter.Radiated;
    return 0;
}

int CritterRadInc( Obj_t *dude, int Dose )
{
    MsgLine_t MsgLine;
    Proto_t *proto;
    Obj_t *p;

    if( dude != gObjDude ) return 0;
    ProtoGetObj( gObjDude->Pid, &proto );
    if( Dose > 0 ) Dose -= FeatGetVal( dude, FEAT_RADRES ) * Dose / 100;
    if( Dose > 0 ){
	proto->Critt.Type |= 0x02;
	if( (p = InvGetLHandObj( gObjDude )) ){
	    if( p->Pid == PID_GEIGER || p->Pid == PID_GEIGERCOUNTER ) goto jj2;
	}
        if( (p = InvGetRHandObj( gObjDude )) ){
    	    if( p->Pid == PID_GEIGER || p->Pid == PID_GEIGERCOUNTER ) goto jj2;
        }
    	p = NULL;    	    
jj2:
	if( p ){
	    if( Item90( p ) ){
		if( Dose > 5 ){
		    MsgLine.Id = 1009; // 'The geiger counter is clicking wildly.'
		    if( MessageGetMsg( &gMessage, &MsgLine ) == 1 ) IfcMsgOut( MsgLine.Text );
		} else {
		    MsgLine.Id = 1008; // 'The geiger counter is clicking.'
		    if( MessageGetMsg( &gMessage, &MsgLine ) == 1 ) IfcMsgOut( MsgLine.Text );
		}
	    }
	}
    }
    if( Dose >= 10 ){
        MsgLine.Id = 1007; // 'you have received large dose of radiation'
        if( MessageGetMsg( &gMessage, &MsgLine ) == 1 ) IfcMsgOut( MsgLine.Text );
    }
    dude->Critter.Radiated += Dose;
    if( dude->Critter.Radiated <= 0 ) dude->Critter.Radiated = 0;
    if( dude == gObjDude ) IfaceIndicatorBoxUpdate( dude, Dose );
    return 0;
}

void CritterRadSetDose( Obj_t *dude )
{
    static int gCrRadDice[6] = { 2, 0, -2, -4, -6, -8 };
    int RadLvl, RadCat;
    Proto_t *proto;
    Critter_t *cr;
    int *p;

    if( dude != gObjDude ) return;    
    ProtoGetObj( gObjDude->Pid, &proto );
    cr = &proto->Critt;
    if( !(cr->Type & 2) ) return;
    gCrUnk09 = 0;
    EvQeRun( 6, (void *)CritterRadUnk01 );

    RadLvl = ( OBJTYPE( dude->Pid ) == 1 ) ? dude->Critter.Radiated : 0;
    if( RadLvl <= 99  ) RadCat = 0;
	else if( RadLvl <= 199 ) RadCat = 1;
	else if( RadLvl <= 399 ) RadCat = 2;
	else if( RadLvl <= 599 ) RadCat = 3;
	else if( RadLvl <= 999 ) RadCat = 4;
	else RadCat = 5;

    if( FeatDice( dude, 2, gCrRadDice[ RadCat ], 0) <= 1 ) RadCat++;
    if( RadCat > gCrUnk09 ){
        if( !(p = Malloc( 8 )) ) return;
        p[1] = 0;
        p[0] = RadCat;
        EvQeSchedule( 36000 * RandMinMax( 4, 18 ), dude, p, EV_RAD_TIMER );
    }
    cr->Type &= ~0x02;        
}

void CritterRadUnk01( int *a1 )
{
    gCrUnk09 = *a1;
}

void CritterUnk11( Obj_t *dude, int *a2 )
{
    if( a2[1] == 1 ) CritterRadApply( dude, *a2, 1 );
}

void CritterRadApply( Obj_t *dude, int a2, int a3)
{
    int n, i;
    MsgLine_t MsgLine;

    if( a2 == 0 ) return;
    a2--;
    n = ( a3 ) ? -1 : 1;
    if( dude == gObjDude ){
        MsgLine.Id = a2 + 1000; // radiated messages
        if( MessageGetMsg( &gMessage, &MsgLine ) == 1 ) IfcMsgOut( MsgLine.Text );
    }
    for( i = 0; i < 8; i++ ){
        FeatSetBoost( dude, gCrCondFeats[ i ], FeatGetBoost( dude, gCrCondFeats[ i ] ) + n * gCrUnk13[ a2 + i ] );
    }
    if( !(dude->Critter.State.CombatResult & 0x80) ){
        for( i = 0; i < 6; i++ ){
            if( FeatGetTotal( dude, gCrCondFeats[ i ] ) + FeatGetBoost( dude, gCrCondFeats[ i ] ) <= 0 ){
        	CritterKill( dude, -1, 1 );
        	break;
            }
        }        
    }
    if( dude->Critter.State.CombatResult & 0x80 && dude == gObjDude ){
        MsgLine.Id = 1006; // you have died from radiation sickness
        if( MessageGetMsg( &gMessage, &MsgLine ) == 1 ) IfcMsgOut(  MsgLine.Text );
    }    
}

void CritterRadEv( Obj_t *dude, int *a2 )
{
    int *p;

    if( a2[1] == 0 ){
	if( !(p = Malloc( sizeof( int[ 2 ] ) ) ) ) return;    
	EvQeRun( 6, (void *)CritterUnk11 );
	p[0] = a2[0];
	p[1] = 1;
DD
//	EvQeSchedule( gCrUnk07, dude, p, EV_RAD_TIMER );
    }
    CritterRadApply( dude, a2[0], a2[1] );
}

int CritterLoadUnk01( xFile_t *fh, int **dat )
{
    int *p;

    if( !(p = Malloc( 8 )) ) return -1;
    if( dbgetBei( fh, &p[0] ) == -1 || dbgetBei(fh, &p[1] ) == -1 ){ Free( p ); return -1; }
    *dat = p;
    return 0;
}

int CritterSaveUnk01( xFile_t *fh, int *dat )
{
    if( dbputBei( fh, dat[0] ) == -1 || dbputBei( fh, dat[1] ) == -1 ) return -1;
    return 0;
}

int CritterUnk03( Obj_t *dude )
{
    Proto_t *proto;

    if( OBJTYPE( dude->Pid ) == 1 && ProtoGetObj( dude->Pid, &proto ) == -1 ) return 0;
    return proto->Critt.TypeNameID;    
}

int CritterKillStatReset()
{
    memset( gCrKillStats, 0, sizeof(gCrKillStats) );
    return 0;
}

void CritterKillStatInc( int idx )
{
    if( idx != -1 && idx < 19 ) gCrKillStats[ idx ]++;
}

int CritterGetKillStat( int idx )
{
    if( idx == -1 || idx >= 19 ) return 0;
    return gCrKillStats[ idx ];
}

int CritterLoadKillStats( xFile_t *fh )
{
    if( dbreadBeiBlk( fh, gCrKillStats, 19 ) != -1 ) return 0;
    dbClose( fh );
    return -1;
}

int CritterSaveKillStats( xFile_t *fh )
{
    if( dbputBeiBlk( fh, (unsigned int *)gCrKillStats, 19 ) != -1 ) return 0;
    dbClose( fh );
    return -1;
}

int CritterGetGender( Obj_t *dude )
{
    Proto_t *proto;

    if( dude == gObjDude ){
        if( (FeatGetVal( dude, FEAT_GENDER )) != 1 ) return 0;
	return 1;
    } else if( (dude->Pid >> 24) == 1 ){
        ProtoGetObj( dude->Pid, &proto );
        return proto->Critt.Gender;
    }
    return -1;
}

char *CritterKillTypeName( int TxtId )
{
    MsgLine_t MsgLine;

    if( TxtId == -1 || TxtId >= 19 ) return "";
    if( TxtId < 0 ) return NULL;
    return MessageGetMessage( &gProtoMessages, &MsgLine, TxtId + 1450 );    
}

char *CritterGetKillTypeDsc( int TxtId )
{
    MsgLine_t MsgLine;

    if( TxtId == -1 || TxtId >= 19 ) return "";
    if( TxtId < 0 ) return 0;
    return MessageGetMessage( &gProtoMessages, &MsgLine, TxtId + 1469 );    
}

int CritterHit1( Obj_t *dude, int a2 )
{
    if( ( dude->Pid >> 24 ) != 1 ) return -1;
    if( dude->Critter.HitPts < FeatGetVal( dude, FEAT_HP ) ) CritterHeal( dude, 14 * (a2 / 3) );
    dude->Critter.State.Reaction &= ~0x04;
    return 0;
}

int CritterUnk26( Obj_t *n )
{
    return n == gCrUnk08;
}

int CritterKill( Obj_t *dude, int DeathFrame, int a2 )
{
    int result, v5, Id;
    char DestMapElev;
    VidRect_t region, Area1, v12;

    result = OBJTYPE( dude->Pid );
    if( result != 1 ) return result;
    v12.lt = dude->Elevation;
    PartyRemoveMember( dude );
    if( CritterUnk31( dude ) ){
        v5 = (dude->ImgId & 0xFF0000) >> 16;
        if( v5 != 20 && v5 != 21 ) goto LABEL_15;
        if( v5 == 20 ) goto LABEL_13;
        Id = ArtMakeId(1, dude->ImgId & 0xFFF, 49, (dude->ImgId & 0xF000) >> 12, dude->Orientation + 1);
        if( !ArtFileExist(Id) )
LABEL_13:
            Id = ArtMakeId(1, dude->ImgId & 0xFFF, 48, (dude->ImgId & 0xF000) >> 12, dude->Orientation + 1);
    } else {
        if( DeathFrame < 0 ) DeathFrame = 63;
        if( DeathFrame > 63 ) eprintf( "Error: Critter Kill: death_frame out of range!" );
        Id = ArtMakeId(1, dude->ImgId & 0xFFF, DeathFrame, (dude->ImgId & 0xF000) >> 12, dude->Orientation + 1);
        ObjGetArtFileId(&Id);
        if( !ArtFileExist(Id) ){
            eprintf( "Error: Critter Kill: Can't match fid!" );
            Id = ArtMakeId(1, dude->ImgId & 0xFFF, 62, (dude->ImgId & 0xF000) >> 12, dude->Orientation + 1);
            ObjGetArtFileId( &Id );
        }
    }
    ObjSetFrame( dude, 0, &Area1 );
    ObjSetShape( dude, Id, &region );
    RegionExpand( &Area1, &region, &Area1 );
LABEL_15:
    if( !CritterGetInjure( dude->Pid, 2048 ) ){
        dude->Flags |= 0x10;
        if( !(dude->Flags & 0x08) ) ObjSetPlayer( dude, &region );
    }
    RegionExpand( &Area1, &region, &Area1 );
    ObjLightedOff( dude, &region );
    RegionExpand( &Area1, &region, &Area1 );
    DestMapElev = dude->Critter.State.CombatResult;
    dude->Critter.HitPts = 0;
    dude->Critter.State.CombatResult = DestMapElev | 0x80;
    if( dude->ScrId != -1 ){
        ScptRemove( dude->ScrId );
        dude->ScrId = -1;
    }
    gCrUnk08 = dude;
    EvQeRun( 0, (void *)CritterUnk26 );
    result = Item17( dude );
    if( a2 ) TileUpdateArea( &Area1, v12.lt );
    if( dude == gObjDude ){
        EndGameSetupDeathEnding( 0 );
        gMenuEscape = 2;
    }
    return result;
}

int CritterUnk27( Obj_t *dude )
{
    Proto_t *proto;

    ProtoGetObj( dude->Pid, &proto );
    return proto->Critt.i91;
}

int CritterCanTalk( Obj_t *dude )
{
    if( !dude ) return 0;
    if( OBJTYPE( dude->Pid ) != TYPE_CRIT ) return 0;
    if( dude->Critter.State.CombatResult & 0x81 ) return 0;
    return (dude->Critter.State.CombatResult & 0x8081) == 0 && !(dude->Critter.State.CombatResult & 0x80);
}

int CritterIsDead( Obj_t *dude )
{
    if( !dude ) return 0;
    if( OBJTYPE( dude->Pid ) != TYPE_CRIT ) return 0;
    if( FeatGetVal(dude, 35) <= 0 ) return 1;
    return (dude->Critter.State.CombatResult & 0xff) & 0x80;
}

int CritterUnk30( Obj_t *dude )
{
    if( !dude ) return 0;
    if( ( dude->Pid >> 24) != 1 ) return 0;
    return (dude->Critter.State.CombatResult & 0x7C) != 0;
}

int CritterUnk31( Obj_t *dude )
{
    int n;

    if( !dude ) return 0;
    if( (dude->Pid >> 24) != 1 ) return 0;
    if( (dude->Critter.State.CombatResult & 3) != 0 ) return 1;
    n = (dude->ImgId & 0xFF0000) >> 16;
    if( n >= 20 && n <= 35 ) return 1;
    return (n >= 48) && (n <= 63);
}

int CritterGetBodyType( Obj_t *pObj )
{
    Proto_t *proto;

    if( !pObj ){
        eprintf( "Error: critter_body_type: pobj was NULL!" );
        return 0;
    }

    if( (pObj->Pid >> 24) != 1 ) return 0;
    ProtoGetObj( pObj->Pid, &proto );
    return proto->Critt.ProtoID;
}

int CritterLoadA( Critter_t *cr, char *fname )
{
    xFile_t *fh;

    if( !(fh = dbOpen( fname, "rb" ) ) ) return -1;
    if( CritterLoadFile(fh, cr) == -1 ){
        dbClose(fh);
        return -1;
    }
    dbClose( fh );
    return 0;
}

int CritterLoadAllStats( char *fname )
{
    xFile_t *fh;
    Critter_t *cr;
    Proto_t *proto;

    if( !(fh = dbOpen( fname, "rb" )) ) return -1;
    ProtoGetObj( gObjDude->Pid, &proto );
    cr = &proto->Critt;
    if( CritterLoadFile( fh, cr ) == -1 ){ dbClose( fh ); return -1; }
    dbread( gCrName, 32, 1, fh );
    if( SkillLoadSpecials( fh ) == -1 || TraitLoad( fh ) == -1 || dbgetBei( fh, &gChrEditUnspentChrPts ) == -1 ){ dbClose( fh ); return -1; }
    cr->BaseStat[29] = 100;
    cr->ProtoID = 0;
    cr->i91 = 0;
    cr->Gender = 0;
    dbClose( fh );
    return 0;    
}

int CritterLoadFile( xFile_t *fh, Critter_t *Data )
{
    if( dbgetBei( fh, &Data->Type ) == -1 ) return -1;
    if( dbreadBeiBlk( fh, Data->BaseStat, 35 ) == -1 ) return -1;
    if( dbreadBeiBlk( fh, Data->BoostStat, 35 ) == -1 ) return -1;
    if( dbreadBeiBlk( fh, Data->Skills, 18 ) == -1 ) return -1;
    if( dbgetBei( fh, &Data->ProtoID ) == -1 ) return -1; // proto ID ( -0x400 )
    if( dbgetBei( fh, &Data->i91 ) == -1 ) return -1; // Unk 1
    if( dbgetBei( fh, &Data->Gender ) == -1 ) return -1; // Unk 2
    if( dbgetBei( fh, &Data->TypeNameID ) == -1 ) Data->TypeNameID = 0; // msg 1450 + ID in Proto.msg
    return 0;
}

int CritterSaveStats( Critter_t *cr, char *fname )
{
    xFile_t *fh;

    if( !(fh = dbOpen(fname, "wb") ) ) return -1;
    if( CritterSaveFile(fh, cr) == -1 ){
        dbClose(fh);
        return -1;
    }
    dbClose(fh);
    return 0;
}

int CritterSaveAllStats( char *fname )
{
    xFile_t *fh;
    Critter_t *cr;
    Proto_t *proto;

    if( !( fh = dbOpen( fname, "wb" ) ) ) return -1;
    ProtoGetObj( gObjDude->Pid, &proto );
    cr = &proto->Critt;
    if( CritterSaveFile( fh, cr ) == -1 ){ dbClose( fh ); return -1; }
    dbwrite( gCrName, 32, 1, fh );
    if( SkillSaveSpecials( fh ) == -1 || TraitSave( fh ) == -1 || dbputBei( fh, gChrEditUnspentChrPts ) == -1 ){ dbClose( fh ); return -1; }
    dbClose( fh );
    return 0;
}


int CritterSaveFile( xFile_t *fh, Critter_t *cr )
{
    if( dbputBei( fh, cr->Type ) == -1 ) return -1;
    if( dbputBeiBlk( fh, (unsigned int *)cr->BaseStat, 35 ) == -1 ) return -1;
    if( dbputBeiBlk( fh, (unsigned int *)cr->BoostStat, 35 ) == -1 ) return -1;
    if( dbputBeiBlk( fh, (unsigned int *)cr->Skills, 18 ) == -1 ) return -1;
    if( dbputBei( fh, cr->ProtoID) == -1 ) return -1;
    if( dbputBei( fh, cr->i91) == -1 ) return -1;
    if( dbputBei( fh, cr->Gender) == -1 ) return -1;
    if( dbputBei( fh, cr->TypeNameID ) == -1 ) return -1;
    return 0;
}

void CritterUnk36( char a1 )
{
    Proto_t *proto;

    ProtoGetObj( gObjDude->Pid, &proto );
    proto->Critt.Type &= ~(1 << a1);
    if( !a1 ) EvQeRmEventType( gObjDude, EV_SNEAK_TIMER );
    IfaceIndicatorBoxUpdate();
}

void CritterUnk37( char a1 )
{
    Proto_t *proto;

    ProtoGetObj( gObjDude->Pid, &proto );
    proto->Critt.Type |= 1 << a1;
    if( !a1 ) CritterSneakEv();
    IfaceIndicatorBoxUpdate();
}

void CritterUnk38( char a1 )
{
    Proto_t *proto;

    ProtoGetObj( gObjDude->Pid, &proto );
    if( ( (1 << a1) & proto->Critt.Type) )
        CritterUnk36( a1 );
    else
        CritterUnk37( a1 );
}

int CritterUsingSkill( char Effect )
{
    Proto_t *proto;

    ProtoGetObj( gObjDude->Pid, &proto );
    return (1 << Effect) & proto->Critt.Type;
}

int CritterSneakEv()
{
    int lvl, time;

    lvl = SkillGetTotal( gObjDude, SKILL_SNEAK );
    time = 600;
    gCrUnk02 = 0;
    if( SkillUse( gObjDude, SKILL_SNEAK, 0, 0 ) < 2 ){
        if( lvl > 250 ){
            time = 100;
        } else if( lvl > 200 ){
            time = 120;
        } else if( lvl > 170 ){
            time = 150;
        } else if( lvl > 135 ){
            time = 200;
        } else if( lvl > 100 ){
            time = 300;
        } else if( lvl > 80 ){
    	    time = 400;
        }
    } else {
        time = 600;
        gCrUnk02 = 1;
    }
    EvQeSchedule( time, gObjDude, NULL, EV_SNEAK_TIMER );
    return 0;
}

void CritterUnk41()
{
    CritterUnk36( 0 );
}

int CritterUnk42()
{
    Proto_t *proto;

    ProtoGetObj( gObjDude->Pid, &proto );
    if( !(proto->Critt.Type & 0x01) ) return 0;
    return gCrUnk02;    
}

int CritterKnockDownEv( Obj_t *Obj, void *Ptr )
{
    if( Obj->Critter.State.CombatResult & CMBT_DAM_DEAD ) return 0;
    Obj->Critter.State.CombatResult &= ~( CMBT_DAM_KNOCKED_OUT | CMBT_DAM_KNOCKED_DOWN );
    Obj->Critter.State.CombatResult |= CMBT_DAM_KNOCKED_DOWN;
    if( IN_COMBAT ){
        Obj->Critter.State.Reaction |= 0x01;
        return 0;
    }
    AnimUnk25( Obj );
    return 0;
}

void CritterUnk44( Obj_t *dude )
{
    if( OBJTYPE( dude->Pid ) != 1 ) return;
    if( dude->Critter.State.CombatResult & CMBT_DAM_DEAD  ) return;
    dude->Critter.State.CombatResult &= ~( CMBT_DAM_KNOCKED_OUT | CMBT_DAM_KNOCKED_DOWN );
    ObjSetShape( dude, ArtMakeId( OBJTYPE( dude->ImgId ), dude->ImgId & 0xFFF, 0, (dude->ImgId & 0xF000) >> 12, dude->Orientation + 1 ), 0 );
}

int CritterUnk45( Obj_t *dude1, Obj_t *dude2 )
{
    if( !dude1 ||( dude2 && OBJTYPE( dude2->ImgId ) != 1 ) ) return -1;
    if( OBJTYPE( dude1->Pid ) != 1 ) return 0;
    if( !dude2 || dude1->Critter.State.GroupId != dude2->Critter.State.GroupId || (FeatDice(dude1, 4, -1, 0) < 2 && (!PartyMembRdy(dude1) || !PartyMembRdy(dude2))) ){
        dude1->Critter.State.WhoHitMeObj = dude2;
        if( dude2 == gObjDude ) EvQeUnk19( dude1, -3 );
    }
    return 0;
}

int CritterUnk46()
{
    int CanRestHere,ObjList,i,err,GroupId,RestFlag;
    Obj_t **p;

    CanRestHere = WmCanRestHere( gMapCurrentLvl );
    RestFlag = 0;
    if( !CanRestHere ) RestFlag = 1;
    err = 1;
    GroupId = gObjDude->Critter.State.GroupId;
    ObjList = ObjGetObjList( -1, gMapCurrentLvl, 1, &p );
    for( i = 0; i < ObjList; i++ ){
        if( ( p[ i ]->Grid.DestMapElev & 0xff ) < 0 ) continue;
	if( p[ i ] == gObjDude ) continue;
    	if( p[ i ]->Critter.State.WhoHitMe != gObjDude->TimeEv ) continue;    	    
    	if( !RestFlag ) continue;
    	if( GroupId == p[ i ]->Critter.State.GroupId ){
    	    err = 0;
    	    break;
        }            
    }        
    if( ObjList ) ObjCritterListDestroy( p );
    return err;
}

int CritterUnk47( Obj_t *dude, int a2)
{
    int Condition;

    if( (dude->Pid >> 24) != 1 ) return 0;
    Condition = dude->Critter.State.CombatResult;
    if( (Condition & 4) != 0 && (Condition & 8) != 0 ) return 8 * a2;
    if( (Condition & 0xC) != 0 ) return 4 * a2;
    return a2;
}

int CritterOverload( Obj_t *dude )
{
    return FeatGetVal( dude, FEAT_CARRY ) < ItemGetBackPackWeight( dude );
}

int CritterUnk49( Obj_t *dude )
{
    if( !dude ) return 0;
    return (dude->Critter.State.Reaction & 4) != 0;
}

int CritterGetInjure( int Pid, int mask )
{
    Proto_t *proto;

    if( Pid == -1 || OBJTYPE( Pid ) != TYPE_CRIT ) return 0;
    if( ProtoGetObj( Pid, &proto ) == -1 ) return 0;
    return proto->ImgId & mask;
}

int CritterSetInjure( int dudeId, int mask )
{
    Proto_t *proto;

    if( dudeId == -1 || OBJTYPE( dudeId ) != 1 ) return -1;    
    if( ProtoGetObj( dudeId, &proto ) != -1 ) return -1;
    proto->ImgId |= mask;
    return 0;    
}

int CritterClrInjure( int dudeId, int mask )
{
    Proto_t *proto;

    if( dudeId == -1 || OBJTYPE( dudeId ) != 1 ) return -1;    
    if( ProtoGetObj( dudeId, &proto ) == -1 ) return -1;
    proto->ImgId &= ~mask;
    return 0;
}

int CritterToggleInjure( int dudeId, int mask)
{
    Proto_t *proto;

    if( dudeId == -1 || OBJTYPE( dudeId ) != 1 ) return -1;    
    if( ProtoGetObj( dudeId, &proto ) == -1 ) return -1;
    proto->ImgId ^= ~mask;
    return 0;

}


