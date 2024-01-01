#include "FrameWork.h"

Feat_t gFeats[ 43 ] = {
    // skills
    { 0, 0, 0x00, 0x01, 0x0A, 5 }, //0 stamina
    { 0, 0, 0x01, 0x01, 0x0A, 5 }, //1 perception
    { 0, 0, 0x02, 0x01, 0x0A, 5 }, //2 endurance
    { 0, 0, 0x03, 0x01, 0x0A, 5 }, //3 charisma
    { 0, 0, 0x04, 0x01, 0x0A, 5 }, //4 inteligence
    { 0, 0, 0x05, 0x01, 0x0A, 5 }, //5 agility
    { 0, 0, 0x06, 0x01, 0x0A, 5 }, //6 luck
    //
    { 0, 0, 0x0A, 0x00, 999, 0 }, //7 HP
    { 0, 0, 0x4B, 0x01, 99,  0 }, //8 AP
    // + 9
    { 0, 0, 0x12, 0x00, 999, 0 }, //9 AC
    { 0, 0, 0x1F, 0x00, 0x7FFFFFFF, 0 }, // 10 unused
    { 0, 0, 0x20, 0x00, 500, 0 }, // 11 melee damage
    { 0, 0, 0x14, 0x00, 999, 0 }, // 12 carry weight
    { 0, 0, 0x18, 0x00, 0x3C, 0 }, 
    { 0, 0, 0x19, 0x00, 0x1E, 0 }, 
    { 0, 0, 0x1A, 0x00, 0x64, 0 }, 
    { 0, 0, 0x5E, -0x3c,0x64, 0 }, 
    { 0, 0, 0x00, 0x00, 0x64, 0 }, 
    { 0, 0, 0x00, 0x00, 0x64, 0 }, 
    { 0, 0, 0x00, 0x00, 0x64, 0 }, 
    { 0, 0, 0x00, 0x00, 0x64, 0 }, 
    { 0, 0, 0x00, 0x00, 0x64, 0 }, 
    { 0, 0, 0x00, 0x00, 0x64, 0 }, 
    { 0, 0, 0x00, 0x00, 0x64, 0 }, 
    { 0, 0, 0x16, 0x00, 0x5A, 0 }, 
    { 0, 0, 0x00, 0x00, 0x5A, 0 }, 
    { 0, 0, 0x00, 0x00, 0x5A, 0 }, 
    { 0, 0, 0x00, 0x00, 0x5A, 0 }, 
    { 0, 0, 0x00, 0x00, 0x5A, 0 }, 
    { 0, 0, 0x00, 0x00, 0x64, 0 }, 
    { 0, 0, 0x00, 0x00, 0x5A, 0 }, 
    { 0, 0, 0x53, 0x00, 0x5F, 0 }, 
    { 0, 0, 0x17, 0x00, 0x5F, 0 }, 
    { 0, 0, 0x00, 0x10, 0x65, 0x19 }, 
    { 0, 0, 0x00, 0x00, 0x01, 0 }, 
    { 0, 0, 0x0A, 0x00, 0x7D0, 0 }, 
    { 0, 0, 0x0B, 0x00, 0x7D0, 0 }, 
    { 0, 0, 0x0C, 0x00, 0x7D0, 0 }, 
    // 38 feats titles
    { 0, 0, 0x00, 0x00, 0x7FFFFFFF, 0 }, 
    { 0, 0, 0x00, 0x01, 0x63, 1 }, 
    { 0, 0, 0x00, 0x00, 0x7FFFFFFF, 0 }, 
    { 0, 0, 0x00, -0x14,0x14, 0 }, 
    { 0, 0, 0x00, 0x00, 0x7FFFFFFF, 0 }
};

Msg_t gFeatMsgStat;
char *gFeatComment[ 10 ];
int gFeatPoints[ 10 ];

int FeatStatInit()
{
    int i, IdA, IdB;
    char stmp[260];
    MsgLine_t MsgLines;

    for( i = 0; i < 5; i++ ){
        gFeatPoints[ i ] = gFeats[ 38 + i ].Value;
    }

    if( MessageInit( &gFeatMsgStat ) != 1 ) return -1;
    sprintf( stmp, "%s%s", gGamePath, "stat.msg");
    if( MessageLoad( &gFeatMsgStat, stmp ) != 1 ) return -1;

    IdA = 100;
    IdB = 200;
    for( i = 0; i < 38; i++ ){
        gFeats[ i ].Name = MessageGetMessage( &gFeatMsgStat, &MsgLines, IdA++ );
        gFeats[ i ].Description = MessageGetMessage( &gFeatMsgStat, &MsgLines, IdB++ );
    }

    IdA = 400;
    IdB = 500;
    for( i = 0; i < 5; i++ ){ // unspent skill points, level, experience, reputation, karma
        gFeats[ 38 + i ].Name = MessageGetMessage( &gFeatMsgStat, &MsgLines, IdA++ );
        gFeats[ 38 + i ].Description = MessageGetMessage( &gFeatMsgStat, &MsgLines, IdB++ );
    }

    IdA = 301;
    for( i = 0; i < 10; i++ ){    
        gFeatComment[ i ] = MessageGetMessage( &gFeatMsgStat, &MsgLines, IdA++ );
    }
    return 0;
}

void FeatResetPoints()
{
    int i;

    for( i = 0; i < 5; i++ ){
         gFeatPoints[ i ] = gFeats[ 38 + i ].Value;
    }
}

int FeatStatClose()
{
    MessageClose( &gFeatMsgStat );
    return 0;
}

int FeatLoadPoints( xFile_t *fh )
{
    int i;

    for( i = 0; i < 5; i++ ){
        if( dbgetBei( fh, &gFeatPoints[ i ] ) == -1 ) return -1;
    }
    return 0;
}

int FeatSavePoints( xFile_t *fh )
{
    int i;

    for( i = 0; i < 5; i++ ){
        if( dbputBei( fh, gFeatPoints[ i ] ) == -1 ) return -1;
    }
    return 0;
}

int FeatGetVal( Obj_t *dude, int FeatId )
{
    Obj_t *p;
    int tmp, Val, v9, v30;

    if( FeatId >= 35 ){
	switch( FeatId ){
	    case 35: Val = CritterGetHp( dude ); break;
	    case 36: Val = CritterPoisoned( dude ); break;
	    case 37: Val = CritterRadiated( dude ); break;
	    default: Val = 0;
	}
	return Val;
    }
    Val = FeatGetBase( dude, FeatId );
    if( dude == gObjDude ) Val += TraitSpecBonus( FeatId );
    Val += FeatGetBoost( dude, FeatId );
    if( IN_COMBAT && (FeatId == FEAT_AC) &&  (CombatUnk05() != dude) ){
        v9 = 0;
        v30 = 1;
        tmp = 0;
        if( dude == gObjDude && PerkLvl( gObjDude, PERK_HTH_EVADE ) ){
            if( (p = InvGetRHandObj( gObjDude )) && ItemGetObjType( p ) == PR_ITEM_WEAPON && Item58( p ) ) v9 = 1;
            if( !v9 ){
                if( (p = InvGetLHandObj( gObjDude )) && ItemGetObjType( p ) == PR_ITEM_WEAPON && Item58( p ) ) v9 = 1;
                if( !v9 ){
                    v30 = 2;
                    tmp = SkillGetTotal( gObjDude, SKILL_UNARMED ) / 12;
                }
            }
        }
        Val += tmp + dude->Critter.State.CurrentAP * v30;
    }
    if( FeatId == FEAT_PERCEPTION && (dude->Critter.State.CombatResult & 0x40) ) Val -= 5;
    if( FeatId == FEAT_33 ){ // starting age
        Val += ScptGetGameDekaSeconds() / 315360000;
    }
    if( FeatId == FEAT_AP ){
        tmp = FeatGetVal( dude, FEAT_CARRY ) - ItemGetBackPackWeight( dude );
        if( tmp < 0 ) Val -= -tmp / 40 + 1;
    }
    if( dude == gObjDude ){
        switch( FeatId ){
    	    case FEAT_PERCEPTION:
        	    if( PerkLvl( gObjDude, PERK_GAIN_PERCEPTION ) ) Val++;
        	    break;
    	    case FEAT_ENDURANCE:
        	    if( PerkLvl( dude, PERK_GAIN_ENDURANCE ) ) Val++;
        	    break;
    	    case FEAT_CHARISMA: 
        	    if( !PerkLvl( dude, PERK_GAIN_CHARISMA ) ) break;
        	    tmp = 0;
        	    if( ( p = InvGetRHandObj( dude ) ) && p->Pid == PID_MIRROREDSHADES ) tmp = 1;
        	    if( ( p = InvGetLHandObj( dude ) ) && p->Pid == PID_MIRROREDSHADES ) tmp = 1;
        	    if( tmp ) Val++;
        	    break;
    	    case FEAT_INTELLIGENCE: 
        	    if( PerkLvl( dude, PERK_GAIN_INTELLIGENCE ) ) Val++;
        	    break;
    	    case FEAT_AGILITY:
        	    if( PerkLvl( dude, PERK_GAIN_AGILITY ) ) Val++;
        	    break;
    	    case FEAT_LUCK:
        	    if( PerkLvl( dude, PERK_GAIN_LUCK ) ) Val++;
        	    break;
    	    case FEAT_PSNRES:
        	    if( PerkLvl( dude, PERK_VAULT_CITY_INOCULATIONS ) ) Val += 10;
        	    break;
    	    case FEAT_RADRES: 
        	    if( PerkLvl( dude, PERK_VAULT_CITY_INOCULATIONS ) ) Val += 10; 
        	    break;
            case 24: // damage resistance normal
            case 30: // damage resistance explosive
            	    if( PerkLvl(dude, PERK_DERMAL_IMPACT_ARMOR ) ){
                	Val += 5;
            	    } else if( PerkLvl( dude, PERK_DERMAL_IMPACT_ASSLT_ENCH ) ){
                	Val += 10;
            	    }
            	    break;
            case 25: // dam resistance laser
            case 26: // dam resistance fire
            case 27: // dam resistance plasma
            	    if( PerkLvl( dude, PERK_PHOENIX_ARMOR_IMPLANTS ) ){
                	Val += 5;
            	    } else if( PerkLvl( dude, PERK_PHOENIX_ASSAULT_ENCH ) ){
                	Val += 10;
            	    }
            	    break;
            case FEAT_HP:
            	    if( PerkLvl( dude, PERK_ALCOHOL_RAISED_HP1  ) ) Val += 2;
            	    if( PerkLvl( dude, PERK_ALCOHOL_RAISED_HP2  ) ) Val += 4;
            	    if( PerkLvl( dude, PERK_ALCOHOL_LOWERED_HP1 ) ) Val -= 2;
            	    if( PerkLvl( dude, PERK_ALCOHOL_LOWERED_HP2 ) ) Val -= 4;
            	    if( PerkLvl( dude, PERK_AUTODOC_RAISED_HP1  ) ) Val += 2;
            	    if( PerkLvl( dude, PERK_AUTODOC_RAISED_HP2  ) ) Val += 4;
            	    if( PerkLvl( dude, PERK_AUTODOC_LOWERED_HP1 ) ) Val -= 2;
            	    if( PerkLvl( dude, PERK_AUTODOC_LOWERED_HP2 ) ) Val -= 4;
            	    break;
            case FEAT_STAMINA:
        	    if( PerkLvl( gObjDude, PERK_GAIN_STRENGHT ) ) Val++;
        	    if( PerkLvl( dude, PERK_ADRENALINE_RUSH ) ){
        		if( FeatGetVal( gObjDude, 35 ) < (FeatGetVal( gObjDude, FEAT_HP ) / 2) ) Val++;
        	    }
        	    break;
        }
    }
    if( Val < gFeats[ FeatId ].Min ) return gFeats[ FeatId ].Min;
    if( Val > gFeats[ FeatId ].Max ) return gFeats[ FeatId ].Max;
    return Val;
}

int FeatGetTotal( Obj_t *dude, unsigned int Id )
{
    int val;

    val = FeatGetBase( dude, Id );
    if( dude == gObjDude ) return val + TraitSpecBonus( Id );
    return val;
}

int FeatGetBase( Obj_t *dude, int Id )
{
    Proto_t *proto;

    if( Id < 35 || Id >= 38 ){
        if( Id < 35 ){
            ProtoGetObj( dude->Pid, &proto );
            return proto->Critt.BaseStat[ Id ];
        } else {
            return 0;
        }
    } else if( Id == 35 ){
        return CritterGetHp( dude );
    } else {
        if( Id == 36 ) return CritterPoisoned( dude );
        return CritterRadiated( dude );
    }
}

int FeatGetBoost( Obj_t *dude, int Id )
{
    Proto_t *proto = NULL;

    if( Id >= 35 ) return 0;
    ProtoGetObj( dude->Pid, &proto );
    return proto->Critt.BoostStat[ Id ];
}

int FeatSetBase( Obj_t *dude, int Id, int NewVal )
{
    Proto_t *proto;

    switch( Id ){
	case 0 ... 6:
            if( dude == gObjDude ) NewVal -= TraitSpecBonus( Id );
            if( NewVal < gFeats[ Id ].Min ) return -2;
            if( NewVal > gFeats[ Id ].Max ) return -3;
            ProtoGetObj( dude->Pid, &proto );
            proto->Critt.BaseStat[ Id ] = NewVal;
            if( Id <= 6 ) FeatStatsRecalculate( dude );
            return 0;		
	case 7 ... 32: return -1;
	case 35: return CritterHeal( dude, NewVal - CritterGetHp( dude ) );
	case 36: return CritterPoisonInc( dude, NewVal - CritterPoisoned( dude ) );
	case 37: return CritterRadInc( dude, NewVal - CritterRadiated( dude ) );
    }        
    return -5;
}

int FeatIncVal( Obj_t *dude, int Id )
{
    int val;

    val = FeatGetBase( dude, Id );
    if( dude == gObjDude ) val += TraitSpecBonus( Id );    
    return FeatSetBase( dude, Id, val + 1 );
}

int FeatDecVal( Obj_t *dude, int Id )
{
    int val;

    val = FeatGetBase( dude, Id );
    if( dude == gObjDude ) val += TraitSpecBonus( Id );
    return FeatSetBase( dude, Id, val - 1 );
}

int FeatSetBoost( Obj_t *dude, int Id, int NewVal )
{
    Proto_t *proto;

    switch( Id ){
	case 0 ... 34:
    	    ProtoGetObj( dude->Pid, &proto );
    	    proto->Critt.BoostStat[ Id ] = NewVal;
    	    if( Id <= 6 ) FeatStatsRecalculate( dude );
    	    break;
	case 35: CritterHeal( dude, NewVal ); break;
	case 36: CritterPoisonInc( dude, NewVal ); break;
	case 37: CritterRadInc( dude, NewVal ); break;
	default: return -5;
    }
    return 0;
}

void FeatReset( Critter_t *p )
{
    int i;

    for( i = 0; i < 34; i++ ){
        p->BoostStat[ i ] = 0;
        p->BaseStat[ i ] = gFeats[ i ].Value;
    }
}

void FeatStatsRecalculate( Obj_t *dude )
{

    int stm, ag, en_tot, st_tot, en1, lk, stm1, pe;
    Proto_t *proto;

    stm = FeatGetVal(dude, FEAT_STAMINA );
    stm1 = stm;
    pe = FeatGetVal( dude, FEAT_PERCEPTION );
    en1 = FeatGetVal( dude, FEAT_ENDURANCE );
    // in = FeatGetVal( dude, FEAT_INTELLIGENCE );
    ag = FeatGetVal( dude, FEAT_AGILITY );
    lk = FeatGetVal( dude, FEAT_LUCK );
    en_tot = FeatGetTotal( dude, FEAT_ENDURANCE ) * 2;
    st_tot = FeatGetTotal( dude, FEAT_STAMINA );

    ProtoGetObj( dude->Pid, &proto );
    proto->Critt.BaseStat[ FEAT_HP      ] = 15 + st_tot + en_tot;
    proto->Critt.BaseStat[ FEAT_AP      ] = 5 + ag / 2; // action points
    proto->Critt.BaseStat[ FEAT_AC      ] = ag;  // armor class
    proto->Critt.BaseStat[ FEAT_MELEE   ] = ( ( stm - 5 ) >= 1 ) ? (stm - 5) : 1; // melee damage
    proto->Critt.BaseStat[ FEAT_CARRY   ] = 25 * stm1 + 25; // carry weight
    proto->Critt.BaseStat[ FEAT_SEQ     ] = 2 * pe;  // sequence
    proto->Critt.BaseStat[ FEAT_HEALING ] = ( en1 / 3 >= 1 ) ? (en1 / 3) : 1; // healing rate
    proto->Critt.BaseStat[ FEAT_CRIT    ] = lk;      // critical chance
    proto->Critt.BaseStat[ FEAT_16      ] = 0;
    proto->Critt.BaseStat[ FEAT_RADRES  ] = 2 * en1; // radiation resistance
    proto->Critt.BaseStat[ FEAT_PSNRES  ] = 5 * en1; // poison resistance

}

char *FeatGetName( unsigned int feat )
{
    if( feat < 35 ) return gFeats[ feat ].Name;
    if( feat < 35 || feat >= 38 ) return 0;
    return gFeats[ feat ].Name;
}

char *FeatGetDsc( unsigned int feat )
{
    if( feat < 35 ) return gFeats[ feat ].Description;
    if( (int)feat < 35 || feat >= 38 ) return 0;
    return gFeats[ feat ].Description;
}

char *FeatGetComments( unsigned int FeatNo )
{
    if( FeatNo < 1 )  FeatNo = 1;
    if( FeatNo > 10 ) FeatNo = 10;
    return gFeatComment[ FeatNo - 1 ];
}

int FeatGetPoints( unsigned int Feat )
{
    if( Feat < 5 ) return gFeatPoints[ Feat ];
    return 0;
}

int FeatSetPoints( unsigned int Id, int NewVal )
{
    if( Id >= 5 ) return -5;
    if( NewVal < gFeats[ FEAT_PTS + Id ].Min ) return -2;
    if( NewVal > gFeats[ FEAT_PTS + Id ].Max ) return -3;
    if( Id == 2 && NewVal < gFeatPoints[ 2 ] ){
	FeatLvlDn( NewVal );
    } else {
	gFeatPoints[ Id ] = NewVal;
	if( Id == 2 ) FeatLvlUpStats( 0, 1 );
    }
    return 0;
}

void FeatPtsReset()
{
    int i;
    for( i = 0; i < 5; i++ ) gFeatPoints[ i ] = gFeats[ FEAT_PTS + i ].Value;
}

int FeatNextLvlPts()
{
    return FeatPtsPerLvl( gFeatPoints[ 1 ] + 1 );
}

int FeatPtsPerLvl( int lvl )
{
    if( lvl >= FEAT_PTS_LEVELCAP ) return -1;
    return FEAT_PTS_PER_LEVEL( lvl );
}

char *FeatGetPtsName( unsigned int Id )
{
    if ( Id < 5 ) return gFeats[ FEAT_PTS + Id ].Name;
    return NULL;
}

char *FeatGetPtsDsc( unsigned int Id )
{
    if( Id < 5 ) return gFeats[Id + 38].Description;
    return NULL;
}

int FeatGetId( int idx )
{
    if( idx < 35 ) return gFeats[ idx ].Id;
    if( idx < 35 || idx >= 38 ) return 0;
    return gFeats[ idx ].Id;
}

int FeatDice( Obj_t *dude, int FeatId, int Boost, int *pDraw )
{
    int a;
    int r;

    a = FeatGetVal( dude, FeatId ) + Boost;
    r = RandMinMax( 1, 10 );
    if( pDraw ) *pDraw = a - r;
    return (r <= a) + 1; // 2 - Success, 1 - Fail
}

int FeatLvlUp( int pts )
{
    return FeatLvlUpStats( pts, 1 );
}

int FeatLvlUpStats( int pts, int LvlUpAllow )
{
    int Min, Val, Base;
    MsgLine_t msg;

    Min = gFeatPoints[ 2 ] + PerkLvl( gObjDude, 50 ) * 5 * pts / 100 + pts;
    if( Min < gFeats[ 40 ].Min ) Min = gFeats[ 40 ].Min;
    if( Min > gFeats[ 40 ].Max ) Min = gFeats[ 40 ].Max;
    gFeatPoints[ 2 ] = Min;
    while( gFeatPoints[ 1 ] < 99 && Min >= FeatNextLvlPts() ){
        if( FeatSetPoints( 1, gFeatPoints[ 1 ] + 1 ) ) continue;
        Val = FeatGetVal( gObjDude, FEAT_HP );
        msg.Id = 600; // 'You have gone up a level.'
        if( MessageGetMsg( &gFeatMsgStat, &msg ) == 1 ) IfcMsgOut( msg.Text );
        CritterUnk37( 3 );
        GSoundPlay( "levelup" );
        Base = FeatGetBase( gObjDude, 2 ) + TraitSpecBonus( 2 );
        FeatSetBoost( gObjDude, FEAT_HP, FeatGetBoost( gObjDude, FEAT_HP ) + (4 * PerkLvl( gObjDude, PERK_LIFEGIVER ) + Base / 2 + 2) );
        CritterHeal( gObjDude, FeatGetVal( gObjDude, FEAT_HP ) - Val );
        IfaceRenderHP( 0 );
        if( LvlUpAllow ) PartyLvlUp();
    }
    return 0;
}

int FeatLvlDn( int pts )
{
    int n, v1, v2;
    
    v1 = 1;
    v2 = gFeatPoints[ 1 ];
    gFeatPoints[ 2 ] = pts;
    while( pts >= FeatPtsPerLvl(++v1) && v1 < 99 );
    v1--;
    FeatSetPoints( 1, v1 );
    CritterUnk36( 3 );
    n = -((v2 - v1) * (4 * PerkLvl( gObjDude, PERK_LIFEGIVER ) + (FeatGetBase( gObjDude, 2 ) + TraitSpecBonus( 2 )) / 2 + 2));
    CritterHeal( gObjDude, n );
    FeatSetBoost( gObjDude, FEAT_HP, FeatGetBoost( gObjDude, FEAT_HP ) - n );
    IfaceRenderHP( 0 );
    return 0;
}

