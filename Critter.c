#include "FrameWork.h"

Msg_t gCrMsg;
char gCrName[ 32 ];
int  gCrUnk02;
int  gCrKillStats[ 19 ];

char *gCrName_ = "corpse";
int gCrUnk09;

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
    if( MessageInit( &gCrMsg ) != 1 ){ eprintf("\nError: Initing critter name message file!\n"); return -1; }    
    sprintf( path, "%sscrname.msg", gGamePath );
    if( MessageLoad( &gCrMsg, path ) != 1 ) { eprintf( "\nError: Loading critter name message file!\n" ); return -1;}
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
//    return CritterSaveFile( fh, proto->Critt );
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

void CritterHeal( Obj_t *dude, int dmg )
{
    int hp;

    if( OBJTYPE( dude->Pid ) != TYPE_CRIT ) return;
    hp = FeatGetVal( dude, FEAT_HP );
    dude->Critter.HitPts += dmg;
    if( hp >= dude->Critter.HitPts ){
        if( dude->Critter.HitPts <= 0 && dude->Critter.State.CombatResult >= 0 ) CritterKill( dude, 1 );
    } else {
        dude->Critter.HitPts = hp;
    }    
}

int CritterPoisoned( Obj_t *dude )
{
    if( (dude->Pid >> 24) == 1 ) return dude->Critter.Poisoned;
    return 0;
}

void CritterPoisonInc( Obj_t *dude, int val ) // poison
{
/*
    int v2;int v9;
    MsgLine_t MsgLine;

    if( dude != gObjDude ) return -1;
    if( val <= 0 ){
        if( gObjDude->Critter.Poisoned <= 0 ) return 0;
    } else {
        v2 = 100;
        val -= FeatGetVal( dude, FEAT_PSNRES ) * val / 100;
    }
    gObjDude->Critter.Poisoned += val;
    if( gObjDude->Critter.Poisoned > 0 ){
        Unk9014( 5, 0 );
        v2 = 0;
        Unk20003( 10 * (505 - 5 * *(_DWORD *)(v9 + 52)), gObjDude, 0 );
        MsgLine.Id = 3000; // you have been poisoned
        if( val < 0 ) MsgLine.Id = 3002; // you fill a little better
        if( MessageGetMsg( &gMessage, MsgLine ) == 1 ) IfcMsgOut();
    } else {
	gObjDude->Critter.Poisoned = 0;
	MsgLine.Id = 3003; // you fill better
	if( MessageGetMsg( &gMessage, MsgLine ) == 1 ) IfcMsgOut();
    }
    if( dude == gObjDude ) Unk20004(v8, v2);
*/
}

int CritterPoison( Obj_t *dude, MsgLine_t *MsgLine )
{
    if( dude != gObjDude ) return 0;
    CritterPoisonInc( dude, -2 );
    CritterHeal( dude, -1 );
//    Unk8004();
//    if( MessageGetMsg( &gMessage, MsgLine ) == 1 ) IfcMsgOut();
    return ((dude->Pid >> 24) == 1 ? dude->Critter.HitPts : 0 ) <= 5;
}

int CritterRadiated( Obj_t *dude )
{
    if( (dude->Pid >> 24) == 1 ) return dude->Critter.Radiated;
    return 0;
}

void CritterRadInc( Obj_t *dude, int Val )
{
/*
    MsgLine_t MsgLine;
    Proto_t *proto;

    if( dude != gObjDude ) return -1;
    ProtoGetObj( gObjDude->Pid, &proto );
    if( Val > 0 ) Val -= FeatGetVal( dude, FEAT_RADRES ) * Val / 100;
    if( Val > 0 ) proto->Critt.Type |= 0x02;
    if( Val > 0 ){
	Unk4011( gObjDude );
        Unk4010( gObjDude );
    }
    if( Val >= 10 ){
        MsgLine.Id = 1007; // 'you have received large dose of radiation'
        if( MessageGetMsg( &gMessage, &MsgLine ) == 1 ) IfcMsgOut();
    }
    dude->Critter.Radiated += Val;
    if( dude->Critter.Radiated <= 0 ) dude->Critter.Radiated = 0;
    if( dude == gObjDude ) Unk20004( dude, Val );
*/
}

void CritterRadSetDose( Obj_t *dude )
{
/*
    static int gCrRadDice[6] = { 2, 0, -2, -4, -6, -8 };
    int RadLvl, RadCat;
    Proto_t *proto;
    Critter_t *cr;

    if( dude != gObjDude ) return;    
    ProtoGetObj( gObjDude->Pid, &proto );
    cr = &proto->Critt;
    if( !(cr->Flags & 2) ) return;
    gCrUnk09 = 0;
    ScrMsg_01( 6, CritterRadUnk01 );

    RadLvl = ( HIBYTE(dude->Pid >> 24) == 1 ) ? dude->Critter.Radiated : 0;
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
        p[0] = v5;
        Unk20003( 36000 * RandMinMax( 4, 18 ), dude, p );
    }
    cr->Flags &= ~0x02;        
*/
}

void CritterRadUnk01( int *a1 )
{
    gCrUnk09 = *a1;
}

void CritterUnk11( Obj_t *dude, int *a2 )
{
//    if( a2[1] == 1 ) CritterRadiating( dude, *a2, 1 );
}

void CritterRadApply( Obj_t *dude, int a2, int a3)
{
/*
    int id, n, i;
    MsgLine_t MsgLine;

    if( a2 == 0 ) return    
    id = a2 - 1;
    n = ( a3 ) ? -1 : 1;
    if( dude == gObjDude ){
        MsgLine.Id = id + 1000; // radiated messages
        if( MessageGetMsg( &gMessage, &MsgLine ) == 1 ) IfcMsgOut();
    }
    for( i = 0; i < 8; i++ ){
        FeatSetBoost( dude, gCrCondFeats[ i ], FeatGetBoost( dude, gCrCondFeats[ i ] ) + n * gCrUnk13[ id + i ] );
    }
    if( !(dude->Critter.State.CombatResult & 0x80) ){
        for( i = 0; i < 6; i++ ){
            if( FeatGetTotal( dude, gCrCondFeats[ i ] ) + FeatGetBoost( dude, gCrCondFeats[ i ] ) <= 0 ){
        	CritterKill( dude, 1 );
        	break;
            }
        }        
    }
    if( dude->Critter.State.CombatResult & 0x80 && dude == gObjDude ){
        MsgLine.Id = 1006; // you have died from radiation sickness
        if( MessageGetMsg( &gMessage, &MsgLine ) == 1 ) IfcMsgOut();
    }    
*/
}

void CritterRadUnk02( Obj_t *dude, int *a2 )
{
/*
    int *p;

    if( a2[1] == 0 ){
    if( !(p = Malloc( 8 ) ) ) return;    
	Unk9014( 6, CritterUnk11 );
	p[0] = a2[0];
	p[1] = 1;
	Unk20003( gCrUnk07, dude, p );
    }
    CritterRadApply( dude, *a2, a2[1] );    
*/
}

int CritterLoadUnk01( xFile_t *fh, int **dat )
{
    int *p;

    if( !(p = Malloc( 8 )) ) return -1;
    if( dbgetBei( fh, &p[0] ) == -1 || dbgetBei(fh, &p[1] ) == -1 ){ Free( p ); return -1; }
printf("\tCritter { %i %i }\n", p[0], p[1] );
    *dat = p;
    return 0;
}

int CritterSaveUnk01( xFile_t *fh, int *dat )
{
    if( dbputBei( fh, &dat[0] ) == -1 || dbputBei( fh, &dat[1] ) == -1 ) return -1;
    return 0;
}

int CritterUnk03( Obj_t *dude )
{
/*
    Proto_t *proto;

    if( (dude->Pid >> 24) == 1 && ProtoGetObj( dude->Pid, &proto ) == -1 ) return 0;
    return proto->i11[ 90 ];    
*/
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
    if( dbputBeiBlk( fh, gCrKillStats, 19 ) != -1 ) return 0;
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

int CritterUnk26( int n )
{
//    return n == gCrUnk08;
}

int CritterKill( Obj_t *dude, int a2 )
{
/*
    int result; int DeathFrame; int v5; char v6; char Condition; int v8; int v9; int region[4]; int Id;
    VidRect_t v10; VidRect_t i11;

    if( (dude->Pid >> 24) != 1 ) return 0;
    i11 = dude->i11;
    PambRemoveMember( dude );
    if( CritterUnk31( dude ) ){
        v5 = (dude->ImgId & 0xFF0000) >> 16;
        if( v5 != 20 && v5 != 21 ) goto LABEL_15;
        if( v5 == 20 ) goto LABEL_13;
        Id = ArtMakeId( 1, dude->ImgId & 0xFFF, 49, (dude->ImgId & 0xF000) >> 12, dude->i08 + 1 );
        if( !ArtFileExist( Id ) ){
LABEL_13:
            Id = ArtMakeId( 1, dude->ImgId & 0xFFF, 48, (dude->ImgId & 0xF000) >> 12, dude->i08 + 1 );
        }
    } else {
        if( DeathFrame < 0 )  DeathFrame = 63;
        if( DeathFrame > 63 ) eprintf("\nError: Critter Kill: death_frame out of range!");
        Id = ArtMakeId(1, dude->ImgId & 0xFFF, DeathFrame, (dude->ImgId & 0xF000) >> 12, dude->i08 + 1);
        Unk3001( &Id );
        if( !ArtFileExist( Id ) ){
            eprintf( "\nError: Critter Kill: Can't match fid!" );
            Id = ArtMakeId( 1, dude->ImgId & 0xFFF, 62, (dude->ImgId & 0xF000) >> 12, dude->i08 + 1 );
            Unk3001( &Id );
        }
    }
    Unk3002( dude, 0, region );
    ProtoUnk44( dude, Id, &v10 );
    RegionsMakeGreater( (VidRect_t *)region, &v10, (VidRect_t *)region );
LABEL_15:
    if( !CritterUnk50(dude->Pid, 2048) ){
        v6 = dude->Flags | 0x10;
        dude->Flags = v6;
        if( (v6 & 8) == 0 ) ProtoUnk104( dude, &v10.lt );
    }
    RegionsMakeGreater( region, &v10, region );
    Unk3003( dude, v10 );
    RegionsMakeGreater( region, &v10, region );

    dude->Critter.HitPts = 0;
    dude->Critter.State.CombatResult = dude->Critter.State.CombatResult | 0x80;
    if( dude->id != -1 ){
        ScptRemove( dude->id, region );
        dude->id = -1;
    }
    gCrUnk08 = (int)dude;
    Unk9014( 0, CritterUnk26 );
    result = Unk3004( dude, region );
    if( a2 ) result = Unk3005( v8, i11 );
    if( dude == gObjDude ){
        result = EndGameSetupDeathEnding();
        gMenuEscape = v9;
    }
    return result;
*/
}

int CritterUnk27( Obj_t *dude )
{
    Proto_t *proto;

    ProtoGetObj( dude->Pid, &proto );
//    return proto->Critt.i91;
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
    if( !dude ) return;    
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
        eprintf( "\nError: critter_body_type: pobj was NULL!" );
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
//printf( "=-=>%i\n", Data->ProtoID);
    if( dbgetBei( fh, &Data->i91 ) == -1 ) return -1; // Unk 1
    if( dbgetBei( fh, &Data->Gender ) == -1 ) return -1; // Unk 2
    if( dbgetBei( fh, &Data->TypeNameID ) == -1 ) Data->TypeNameID = 0; // msg 1450 + ID in Proto.msg
//printf( "=--=>%i\n", Data->TypeNameID);
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
//    if( SkillSaveA( fh ) == -1 || TraitSave( fh ) == -1 || dbputBei( fh, gChrEditUnspentChrPts ) == -1 ){ dbClose( fh ); return -1; }
    dbClose( fh );
    return 0;
}


int CritterSaveFile( xFile_t *fh, Critter_t *cr )
{
    xFile_t *fh_1;

    if( dbputBei( fh, cr->Type ) == -1 ) return -1;
    if( dbputBeiBlk( fh_1, cr->BaseStat, 35 ) == -1 ) return -1;
    if( dbputBeiBlk( fh, cr->BoostStat, 35 ) == -1 ) return -1;
    if( dbputBeiBlk( fh, cr->Skills, 18 ) == -1 ) return -1;
    if( dbputBei( fh, cr->ProtoID) == -1 ) return -1;
    if( dbputBei( fh, cr->i91) == -1 ) return -1;
    if( dbputBei( fh, cr->Gender) == -1 ) return -1;
    if( dbputBei( fh, cr->TypeNameID ) == -1 ) return -1;
    return 0;
}

void CritterUnk36( char a1 )
{
/*
    Proto_t *proto;

    ProtoGetObj(gObjDude->Pid, &proto);
    proto->Critt.Type &= ~(1 << a1);
    if( !a1 ) ScpRemoveTimeEvents( gObjDude, 10 );
    Unk20004();
*/
}

void CritterUnk37( char a1 )
{
/*
    Proto_t *proto;
    Critter_t *cr;

    ProtoGetObj( gObjDude->Pid, &proto );
    proto->Critt.Type |= 1 << a1;
    if( !a1 ) CritterUnk40();
    Unk20004();
*/
}

void CritterUnk38( char a1 )
{
    Proto_t *proto;
    Critter_t *cr;

    ProtoGetObj(gObjDude->Pid, &proto);
    if( ( (1 << a1) & proto->Critt.Type) )
        CritterUnk36( a1 );
    else
        CritterUnk37( a1 );
}

int CritterUnk39( char Effect )
{
    Proto_t *proto;

    ProtoGetObj( gObjDude->Pid, &proto );
    return (1 << Effect) & proto->Critt.Type;
}

int CritterUnk40()
{
/*
    signed int ValPercent;
    int v1;

    ValPercent = SkillGetValPercent(gObjDude, 8u);
    if( SkillUnk13(gObjDude, 8u, 0, 0) < 2 )
    {
        v1 = 600;
        gCrUnk02 = 0;
        if( ValPercent <= 250 ){
            if( ValPercent <= 200 ){
                if( ValPercent <= 170 ){
                    if( ValPercent <= 135 ){
                        if( ValPercent <= 100 ){
                            if( ValPercent > 80 ) v1 = 400;
                        } else {
                            v1 = 300;
                        }
                    } else {
                        v1 = 200;
                    }
                } else {
                    v1 = 150;
                }
            } else {
                v1 = 120;
            }
        } else {
            v1 = 100;
        }
    }else {
        v1 = 600;
        gCrUnk02 = 1;
    }
    Unk20003(v1, gObjDude, 0);
*/
    return 0;
}

void CritterUnk41()
{
    CritterUnk36( 0 );
}

int CritterUnk42()
{
    Proto_t *proto;
return 0;
    ProtoGetObj( gObjDude->Pid, &proto );
    if( !(proto->Critt.Type & 1) ) return 0;
//    return gCrUnk02;    
}

int CritterUnk43( Obj_t *dude)
{
/*
    char Condition;
    char *unk;

    Condition = dude->Critter.State.CombatResult;
    unk = (char *)&dude->Critter.State.Reaction;
    if( Condition >= 0 ){
        unk[8] = Condition & 0xFC;
        unk[8] = Condition & 0xFC | 2;
        if( (gCombatStatus & 1) != 0 ){
            *unk |= 1u;
            return 0;
        }
        Unk3009();
    }
*/
    return 0;
}

void CritterUnk44( Obj_t *dude )
{
/*
    char Condition;
    int Id;

    if( (dude->Pid >> 24) != 1 ) return;
    Condition = dude->Critter.State.CombatResult;
    if( Condition < 0 ) return;        
    dude->Critter.State.CombatResult = Condition & 0xFC;
    Id = ArtMakeId( (dude->ImgId & 0xF000000) >> 24, dude->ImgId & 0xFFF, 0, (dude->ImgId & 0xF000) >> 12, dude->ImgArg + 1 );
    ProtoUnk44(dude, Id, 0);        
*/
}

int CritterUnk45( Obj_t *dude1, Obj_t *dude2 )
{
/*
    if( !dude1 ) return -1;
    if( dude2 && (dude2->ImgId & 0xF000000) >> 24 != 1 ) return -1;
    if( (dude1->Pid >> 24) == 1 && (!dude2 || dude1->i21 != dude2->i21 || FeatDice(dude1, 4, -1, 0) < 2 && (!PambUnk11(dude1) || !PambUnk11(dude2))) ){
        dude1->i22 = dude2;
        if( dude2 == gObjDude ) Unk3010( dude1, -3 );
    }
*/
    return 0;
}

int CritterUnk46()
{
    int CanRestHere,ObjList,i,err,GroupId,RestFlag;
    Obj_t **p;

    CanRestHere = WmCanRestHere(gCurrentMapLvl);
    RestFlag = 0;
    if( !CanRestHere ) RestFlag = 1;
    err = 1;
    GroupId = gObjDude->Critter.State.GroupId;
    ObjList = ObjGetObjList( -1, gCurrentMapLvl, 1, &p );
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
return 0;
//    return FeatGetVal( dude, FEAT_CARRY ) < CharEditUnk59( dude );
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

int CritterSetInjure( Obj_t *dude, int mask )
{
    Proto_t *proto;

    if( dude->Pid == -1 || (dude->Pid) >> 24 != 1 ) return -1;    
    if( ProtoGetObj( dude, &proto ) != -1 ) return -1;
    proto->ImgId |= mask;
    return 0;    
}

int CritterClrInjure( Obj_t *dude, int mask )
{
    Obj_t *proto;

    if( dude->Pid == -1 || (dude->Pid) >> 24 != 1 ) return -1;    
    if( ProtoGetObj( dude, &proto ) == -1 ) return -1;
    proto->ImgId &= ~mask;
    return 0;
}

int CritterToggleInjure( Obj_t *dude, int mask)
{
    Obj_t *proto;

    if( dude->Pid == -1 || (dude->Pid) >> 24 != 1 ) return -1;    
    if( ProtoGetObj( dude, &proto ) == -1 ) return -1;
    proto->ImgId ^= ~mask;
    return 0;

}


