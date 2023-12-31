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
    int tmp;
    Obj_t *dd;
    int Val; // ecx MAPDST
//    int Min; // esi

    if( FeatId < 35 || FeatId >= 38 ){
        if( FeatId < 35 ){
            Val = FeatGetBase( dude, FeatId );

            if( dude == gObjDude ) Val += TraitSpecBonus( FeatId );
            Val += FeatGetBoost( dude, FeatId );

            if( (FeatId == FEAT_AC) && (gCombatStatus & 1) != 0 && CombatUnk05() != dude ){
/*
                v9 = 0;
                v30 = 1;
                tmp = 0;
                if( dude == gObjDude && PerkLvl( gObjDude, PERK_HTH_EVADE ) ){
                    v11 = InvGetRHandObj( gObjDude );
                    if( v11 && ItemGetObjType( v11 ) == 3 && Item58( v11 ) ) v9 = 1;
                    if( !v9 ){
                        v13 = InvGetLHandObj( gObjDude );
                        if( v13 && ItemGetObjType( v13 ) == 3 && Item58( v13 ) )
                            v9 = 1;
                        if( !v9 ){
                            v30 = 2;
                            tmp = SkillGetTotal(gObjDude, 3u) / 12;
                        }
                    }
                }
                Val += tmp + dude->Critter.State.CurrentMP * v30;
*/
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
                if( FeatId ){
                    if( (FeatId == FEAT_PERCEPTION) && PerkLvl( gObjDude, 85 ) ){
                        Val++;
                    } else if( ( FeatId == FEAT_ENDURANCE ) && PerkLvl( dude, 86 ) ){
                        Val++;
                    } else {
                        if( FeatId == FEAT_CHARISMA ){
//printf("==>%p\n", dude);
//printf("=o=>%x\n", dude->Pid);
//// ??                           PerkLvl( dude, 87 );
//                            tmp = 0;
//                            if( ( dd = InvGetRHandObj( dude ) ) && dd->Pid == 433 ) tmp = 1;
//DD
//                            if( ( dd = InvGetLHandObj( dude ) ) && dd->Pid == 433 ) tmp = 1;
//DD
//                            if( tmp ) Val++;
//DD
                        } else if( (FeatId == FEAT_INTELLIGENCE) && PerkLvl(dude, 88) ){
                            Val++;
                        } else if( (FeatId == FEAT_AGILITY) && PerkLvl(dude, 89) ){
                            Val++;
                        } else if( (FeatId == FEAT_LUCK) && PerkLvl(dude, 90) ){
                            Val++;
                        } else if( (FeatId == FEAT_PSNRES) && PerkLvl(dude, 78) ){
                            Val += 10;
                        } else if( (FeatId == FEAT_RADRES) && PerkLvl(dude, 78) ){
                            Val += 10;
                        } else {
                            switch( FeatId ){
                                case 24: case 30: // dam resistance normal, explosive
                                    if( PerkLvl(dude, 74) ){
                                        Val += 5;
                                    } else if( PerkLvl(dude, 75u) ){
                                        Val += 10;
                                    }
                                    break;
                                case 25: case 26: case 27: // dam resistance laser,fire,plasma
                                    if( PerkLvl(dude, 76) ){
                                        Val += 5;
                                    } else if( PerkLvl(dude, 77) ){
                                        Val += 10;
                                    }
                                    break;
                                case FEAT_HP:
                                    if( PerkLvl( dude, 108 ) ) Val += 2;
                                    if( PerkLvl( dude, 109 ) ) Val += 4;
                                    if( PerkLvl( dude, 110 ) ) Val -= 2;
                                    if( PerkLvl( dude, 111 ) ) Val -= 4;
                                    if( PerkLvl( dude, 112 ) ) Val += 2;
                                    if( PerkLvl( dude, 113 ) ) Val += 4;
                                    if( PerkLvl( dude, 114 ) ) Val -= 2;
                                    if( PerkLvl( dude, 115 ) ) Val -= 4;
                                    break;
                            }
                        }
                    }
                } else {
                    PerkLvl( gObjDude, 84 );
                    if( PerkLvl( dude, 79 ) ){
                        if( FeatGetVal( gObjDude, 35 ) < (FeatGetVal( gObjDude, FEAT_HP ) / 2) ) Val++;
                    }
                }
            }
            if( Val < gFeats[ FeatId ].Min ) return gFeats[ FeatId ].Min;
            if( Val > gFeats[ FeatId ].Max ) return gFeats[ FeatId ].Max;
            return Val;
        } else {
            return 0;
        }
    } else if( FeatId == 35 ){
        return CritterGetHp( dude );
    } else {        
        return ( FeatId == 36 ) ? CritterPoisoned( dude ) : CritterRadiated( dude );
    }
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
	case 7 ... 32: return -1;
	case 0 ... 6: case 33 ... 34:
    	    if( dude == gObjDude ) NewVal -= TraitSpecBonus( Id );
    	    if( NewVal < gFeats[ Id ].Min ) return -2;
    	    if( NewVal > gFeats[ Id ].Max ) return -3;
    	    ProtoGetObj( dude->Pid, &proto );
    	    proto->Critt.BaseStat[ Id ] = NewVal;
    	    if( Id <= 6 ) FeatStatsRecalculate( dude );
	    break;
//	case 35: CritterHeal( dude, CritterGetHp( 35, NewVal ) ); break;
//	case 36: CritterPoisonInc( dude, CritterPoisoned( 36, NewVal ) ); break;
//	case 37: CritterRadInc( dude, CritterRadiated( Id, NewVal ) ); break;
	default: return -5;
    }
    return 0;            	
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
DD
//    int i;
//    for( i = 0; i < 5; i++ ) gFeatComment[ 9 + i ] = gFeats[ FEAT_PTS + i ].Value;    
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
/*
    int Val, HpBoost;
    MsgLine_t MsgLine;

    pts = gFeatPoints[ 2 ] + CharEditUnk53( gObjDude, 50 ) * 5 * pts / 100 + pts;
    if( pts < gFeats[40].Min ) pts = gFeats[40].Min;
    if( pts > gFeats[40].Max ) pts = gFeats[40].Max;
    gFeatPoints[ 2 ] = pts;
    while( gFeatPoints[1] < 99 && pts >= FeatNextLvlPts() ){
        if( FeatSetPoints(1u, gFeatPoints[1] + 1) ) continue;
        Val = FeatGetVal(gObjDude, FEAT_HP);
        MsgLine.Id = 600;
        if( MessageGetMsg( &gFeatMsgStat, &MsgLine ) == 1 ) IfcMsgOut();
        CritterUnk37( 3, 600 );
        GSoundPlay( "levelup" );
        HpBoost = 4 * CharEditUnk53( gObjDude, 28 ) + (FeatGetBase(gObjDude, 2) + TraitSpecBonus( 2 )) / 2 + 2;
        FeatSetBoost( gObjDude, FEAT_HP, FeatGetBoost( gObjDude, FEAT_HP ) + HpBoost );
        CritterHeal( gObjDude, FeatGetVal( gObjDude, FEAT_HP ) - Val );
        CharEditUnk26();
        if( LvlUpAllow ) PambLvlUp();
    }
*/
}

void FeatLvlDn( int pts )
{
/*
    v2 = gFeatPoints[1];
    gFeatPoints[2] = pts;
    v1 = 1;
    while( pts >= v3 = FeatPtsPerLvl( ++v1 ) && v1 < 99 );
    v5 = v1 - 1;
    FeatSetPoints(1u, v5);
    CharEditUnk25(3);
    FeatGetBase(gObjDude, 2);
    if( v6 == gObjDude ) TraitSpecBonus( 2 );
    CritterHeal( gObjDude, -((v2 - v5) * (4 * CharEditUnk53( gObjDude, 28 ) + v8)) );
    FeatSetBoost( gObjDude, FEAT_HP, FeatGetBoost( gObjDude, FEAT_HP ) - v10 );
    CharEditUnk26();
*/
}

