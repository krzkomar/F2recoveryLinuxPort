#include "FrameWork.h"

Skill_t gSkills[ 18 ] = {
    { NULL, NULL, NULL, 28, 5,  4, 5, -1, 1, 0,  0 }, // 0 Small guns
    { NULL, NULL, NULL, 29, 0,  2, 5, -1, 1, 0,  0 }, // 1 big guns
    { NULL, NULL, NULL, 30, 0,  2, 5, -1, 1, 0,  0 }, // 2 energy weapons
    { NULL, NULL, NULL, 31, 30, 2, 5,  0, 1, 0,  0 }, // 3 unarmed
    { NULL, NULL, NULL, 32, 20, 2, 5,  0, 1, 0,  0 }, // 4 melee weapons
    { NULL, NULL, NULL, 33, 0,  4, 5, -1, 1, 0,  0 }, // 5 throwing
    { NULL, NULL, NULL, 34, 0,  2, 1,  4, 1, 25, 0 }, // 6 first aid
    { NULL, NULL, NULL, 35, 5,  1, 1,  4, 1, 50, 0 }, // 7 doctor
    { NULL, NULL, NULL, 36, 5,  3, 5, -1, 1, 0,  0 }, // 8 sneak
    { NULL, NULL, NULL, 37, 10, 1, 1,  5, 1, 25, 1 }, // 9 lockpick
    { NULL, NULL, NULL, 38, 0,  3, 5, -1, 1, 25, 1 }, // 10 steal
    { NULL, NULL, NULL, 39, 10, 1, 1,  5, 1, 25, 1 }, // 11 traps
    { NULL, NULL, NULL, 40, 0,  4, 4, -1, 1, 0,  0 }, // 12 science
    { NULL, NULL, NULL, 41, 0,  3, 4, -1, 1, 0,  0 }, // 13 repair
    { NULL, NULL, NULL, 42, 0,  5, 3, -1, 1, 0,  0 }, // 14 speech
    { NULL, NULL, NULL, 43, 0,  4, 3, -1, 1, 0,  0 }, // 15 barter
    { NULL, NULL, NULL, 44, 0,  5, 6, -1, 1, 0,  0 }, // 16 gambling
    { NULL, NULL, NULL, 45, 0,  2, 2,  4, 1, 100, 0 } // 17 outdoorsman
};

int gSkillUnk80 = 0;
int gSkillUnk60 = 0;
int gSkillUnk61 = 0;
int gSkillMenuInited = 0;
int gSkillMenuIds[ 6 ] = { 0x79, 0x77, 0x78, 0x08, 0x09, 0xAA };
int gSkillMenuIdx[ 8 ] = { 0x08, 0x09, 0x0A, 0x0B, 0x06, 0x07, 0x0C, 0x0D };

const int gSkillUnk15[ 5 ] = { 64, 16, 32, 8, 4 };
const int gSkillDoctorInjuresMask[5] = { 0x40, 0x10, 0x20, 0x08, 0x04 }; // dmaged eye, crippled la, crippled ra, crippled ll, crippled rl



int gSkillUsage[ 18 ][3]; // 12*18
int gSkillSpecials[ 4 ];
Msg_t gSkillMsg;
Geo_t gSkillMenuGeo[12];
char *gSkillMenuSurfaces[16];
Msg_t gSkillMenuMsg;
MsgLine_t gSkillMenuMsgLine;
char *gSkillMenuPix[6];
CachePool_t *gSkillMenuImgs[6];
int gSkillMenuWin;
char *gSkillMenuSurf;
int gSkillMenuFontSave;


int SkillInit()
{
    int i;
    char path[ 260 ];
    MsgLine_t MsgList;
    
    if( MessageInit( &gSkillMsg ) != 1 ) return -1;
    sprintf( path, "%s%s", gGamePath, SKILL_MSG_FILE );
    if( MessageLoad( &gSkillMsg, path ) != 1 ) return -1;

    for( i = 0; i < 18; i++ ) {
        MsgList.Id = i + 100;
        if( MessageGetMsg( &gSkillMsg, &MsgList ) == 1 ) gSkills[ i ].Name = MsgList.Text;
        MsgList.Id = i + 200;
        if( MessageGetMsg( &gSkillMsg, &MsgList ) == 1 ) gSkills[ i ].Dsc = MsgList.Text;
        MsgList.Id = i + 300;
        if( MessageGetMsg( &gSkillMsg, &MsgList ) == 1 ) gSkills[ i ].Attr = MsgList.Text;
    }
    memset( gSkillSpecials, 0xff, 4 * sizeof( int ) );
    memset( gSkillUsage, 0, 216 );
    return 0;
}

int SkillClear()
{
    memset( gSkillSpecials, 0xff, 4 * sizeof( int ) );
    memset( gSkillUsage, 0, 216 );
    return 0;
}

int SkillMsgClose()
{
    MessageClose( &gSkillMsg );
    return 0;
}

int SkillLoadSpecials( xFile_t *fname )
{
    return ( dbreadBeiBlk( fname, gSkillSpecials, 4 ) != -1) - 1;
}

int SkillSaveSpecials( xFile_t *fh )
{
    return ( dbputBeiBlk( fh, gSkillSpecials, 4 ) != -1) - 1;
}

void SkillReset( Critter_t *cr )
{
    int i;

    for( i = 0; i < 18; i++ ) cr->Skills[ i ] = 0;
}

void SkillSetSpecials( int *set, int n )
{
    int i;    
    for( i = 0; i < n; i++ ) gSkillSpecials[ i ] = set[ i ];
}

void SkillGetSpecials( int *res, int num )
{
    int i;

    for( i = 0; i < num; i++ ){
        res[ i ] = gSkillSpecials[ i ];
    }
}

int SkillIsSpecial( int SkilIdx )
{
    return SkilIdx == gSkillSpecials[0] || SkilIdx == gSkillSpecials[1] || SkilIdx == gSkillSpecials[2] || SkilIdx == gSkillSpecials[3];
}

signed int SkillGetTotal( Obj_t *dude, unsigned int SkillIdx )
{
    int base, Val, pts;
    Skill_t *p;    

    if( SkillIdx >= 18 ) return -5;
    base = SkillGetVal( dude, SkillIdx );
    if( base < 0 ) return base;
    p = &gSkills[ SkillIdx ];
    Val = FeatGetVal( dude, p->i07 );
    if( p->i08 != -1 ) Val += FeatGetVal( dude, p->i08 );    
    pts = p->i05 + p->i06 * Val + base * p->i09;
    if( dude == gObjDude ){
        if( SkillIdx == gSkillSpecials[0] || SkillIdx == gSkillSpecials[1] || SkillIdx == gSkillSpecials[2] || SkillIdx == gSkillSpecials[3] ){
            pts += p->i09 * base;
            if( !PerkLvl( dude, PERK_TAG ) || SkillIdx != gSkillSpecials[3] ) pts += 20;
        }
        pts += TraitSpecBoost( SkillIdx ) + PerkGrantPts( dude, SkillIdx ) + SkillGetDifficulty( SkillIdx );
    }
    if( pts > 300 ) return 300;
    return pts;
}

int SkillGetLvl( unsigned int SkillIdx )
{
    if( SkillIdx < 18 ) return gSkills[ SkillIdx ].i05;    
    return -5;
}

int SkillGetVal( Obj_t *dude, unsigned int SkillIdx )
{
    Proto_t *proto;

    if( SkillIdx >= 18 ) return -5;
    ProtoGetObj( dude->Pid, &proto );
    return proto->Critt.Skills[ SkillIdx ];
}

int SkillLvlUp( Obj_t *dude, unsigned int SkillIdx )
{
    signed int Total;
    int pts;
    int cost;
    Proto_t *proto;

    if( dude != gObjDude || ( SkillIdx >= 18 ) ) return 0;
    ProtoGetObj( dude->Pid, &proto );
    if( ( pts = FeatGetPoints( FEAT_HP ) ) <= 0 ) return 0;
    Total = SkillGetTotal( dude, SkillIdx );
    cost = 1;
    if( Total >= 300 ) return cost;
    if( Total >= 201 ){ cost = 6; goto SetUp; }
    if( Total >= 176 ){ cost = 5; if( pts < 5 ) return cost; goto SetUp; }
    if( Total >= 151 ){ cost = 4; if( pts < 4 ) return cost; goto SetUp; }
    if( Total >= 126 ){ cost = 3; if( pts < 3 ) return cost; goto SetUp; }
    if( Total >= 101 ){ cost = 2; if( pts < 2 ) return cost; goto SetUp; }
    if( pts < cost ) return cost;
SetUp:
    if( !FeatSetPoints(0, pts - cost) ) proto->Critt.Skills[ SkillIdx ]++;
    return cost;
}

int SkillInc( Obj_t *dude, unsigned int SkillIdx )
{
    Proto_t *proto;

    if( dude != gObjDude ) return -5;
    if( SkillIdx < 18 ? 0 : 1 ) return -5;
    ProtoGetObj( dude->Pid, &proto );
    if( SkillGetTotal( dude, SkillIdx ) >= 300 ) return -3;
    proto->Critt.Skills[ SkillIdx ]++;
    return 0;
}

int SkillPtsCost( int pts )
{
    if( pts >= 201 ) return 6;
    if( pts >= 176 ) return 5;
    if( pts >= 151 ) return 4;
    if( pts >= 126 ) return 3;
    if( pts >= 101 ) return 2;
    return 1;    
}

int SkillLvlDn( Obj_t *dude, unsigned int SkillIdx )
{
    int result; int pts; int tot; int cost; int Total;
    Proto_t *proto;

    if( dude != gObjDude ) return -5;
    if( SkillIdx >= 18 ) return -5;
    ProtoGetObj( dude->Pid, &proto );
    if( proto->Critt.Skills[ SkillIdx ] <= 0 ) return -2;
    pts = FeatGetPoints( 0 );
    tot = SkillGetTotal( dude, SkillIdx ) - 1;
    cost = 1;
    if( tot >= 201 ) cost = 6; else
    if( tot >= 176 ) cost = 5; else
    if( tot >= 151 ) cost = 4; else
    if( tot >= 126 ) cost = 3; else
    if( tot >= 101 ) cost = 2;
    pts += cost;    
    if( ( result = FeatSetPoints( 0, pts ) ) ) return result;
    proto->Critt.Skills[ SkillIdx ]--;
    if( SkillIdx != gSkillSpecials[0] && SkillIdx != gSkillSpecials[1] && SkillIdx != gSkillSpecials[2] && SkillIdx != gSkillSpecials[3] ) goto err;
    Total = SkillGetTotal( dude, SkillIdx );
    if( SkillPtsCost( pts ) == SkillPtsCost( Total ) ) goto err;
    if( (result = FeatSetPoints( 0, pts - 1 )) ) return result;
err:
    if( proto->Critt.Skills[ SkillIdx ] < 0 ) proto->Critt.Skills[ SkillIdx ] = 0;
    return 0;
}

int SkillDec( Obj_t *dude, unsigned int SkillIdx )
{
    Proto_t *proto;

    if( dude != gObjDude || SkillIdx >= 18 ) return -5;
    ProtoGetObj( dude->Pid, &proto );
    if( proto->Critt.Skills[ SkillIdx ] <= 0 )  return -2;
    proto->Critt.Skills[ SkillIdx ]--;
    return 0;
}

int SkillUse( Obj_t *dude, unsigned int SkillIdx, int *a3, int a4 )
{
    Obj_t *best;
    int Total;

    if( SkillIdx >= 18 ) return 1;
    if( dude == gObjDude && SkillIdx != 10 ){        
        if( (best = PartyGetBestSkilled( SkillIdx )) ){
            if( PartyBestSkill( best ) == SkillIdx ) dude = best;
        }
    }
    Total = SkillGetTotal( dude, SkillIdx );
    if( dude == gObjDude && SkillIdx == SKILL_STEAL ){
        if( CritterUsingSkill( 0 ) ){
          if( CritterUnk42() ) Total += 30;
        }
    }
    return RandUnk05( a4 + Total, FeatGetVal( dude, FEAT_CRIT ), a3 );
}

int SkillUseOnDude( Obj_t *dude1, Obj_t *dude2, int a3, unsigned int SkillIdx, int a5, int *a6 )
{
    int v8; int v10; int v11;

    if( (v8 = SkillUse( dude1, SkillIdx, &v11, a3 ) ) >= 2 ){
        if( SkillUse( dude2, SkillIdx, &v10, a5 ) >= 2 ) v11 -= v10;
        v8 = RandUnk04( v11, 0 );
    }
    if( a6 ) *a6 = v11;
    return v8;
}

char *SkillGetName( unsigned int SkillNo )
{
    if( SkillNo < 18 ) return gSkills[ SkillNo ].Name;    
    return 0;
}

char *SkillGetDsc( unsigned int SkillNo )
{
    if( SkillNo < 18 ) return gSkills[SkillNo].Dsc;
    return 0;
}

char *SkillGetAttr( unsigned int SkillNo )
{
    if( SkillNo < 18 ) return gSkills[SkillNo].Attr;
    return 0;
}

int SkillGetImgId( unsigned int SkillNo )
{
    if( SkillNo < 18 ) return gSkills[SkillNo].i04;
    return 0;
}

void SkillHoningAward( Obj_t *dude,int SkillIdx, int a3, int a4 )
{
    int i10,PtsPrev;
    char stmp[60];
    MsgLine_t MsgLine;

    if( dude != gObjDude || a3 <= 0 ) return;    
    i10 = gSkills[ SkillIdx ].i10;
    if( i10 == 0 ) return;
    if( gSkills[ SkillIdx ].i11 && a4 < 0 ) i10 += abs( a4 );
    PtsPrev = FeatGetPoints( FEAT_ENDURANCE );
    if( FeatLvlUp( a3 * i10 ) || i10 <= 0 ) return;
    MsgLine.Id = 505; // 'You earn %d XP for honing your skills.'
    if( MessageGetMsg( &gSkillMsg, &MsgLine ) != 1 ) return;
    sprintf( stmp, MsgLine.Text, FeatGetPoints( FEAT_ENDURANCE  ) - PtsPrev );
    IfcMsgOut( stmp );
}

int SkillAttempt( Obj_t *dude1, Obj_t *dude2, unsigned int SkillIdx, int a4 )
{
    ObjCritterCond_t *p_State;
    MsgLine_t MsgLineA;
    MsgLine_t MsgLineB;
    Obj_t *db;
    Obj_t *dd;
    Obj_t *crit;
    char v22;
    char stmp[60];
    char v26;
    int v7;
    int Hp;
    int v11;
    int v17;
    int v19;
    int v21;
    int v23;
    int v24;
    int v29;
    int v31;
    int v33;
    int v34;
    int Total;
    int InjuresMask[5];
    int v47;
    int v48;
    int Val;
    int Max;
    int Min;
    int v52;
    int v53;
    int v55;
    int v56;
    int v57;

DD
    crit = dude1;
    v57 = 0;
    v47 = 0;
    Min = 0;
    Max = 0;
    v52 = 0;
    p_State = &dude2->Critter.State;
    Val = FeatGetVal(dude2, 35);
    v55 = 1;
    v48 = FeatGetVal(dude2, 7);
    v56 = 1;
    if( crit == gObjDude && (SkillIdx == 6 || SkillIdx == 7) ){
        v7 = PerkLvl(crit, 19u);
        Min = 4 * v7;
        Max = 10 * v7;
    }
    v53 = FeatGetVal(crit, 15) + a4;
    switch( SkillIdx ){
        case SKILL_FIRSTAID:
            if( SkillUsingTired(6) == -1 ){
                MsgLineA.Id = RandMinMax(0, 2) + 590; // 'You've taxed your ability with that skill. Wait a while.'/'You're too tired.'/'The strain might kill you.'
                if( MessageGetMsg(&gSkillMsg, &MsgLineA) == 1 ) IfcMsgOut(MsgLineA.Text);
                return -1;
            }
            if( !CritterIsDead(dude2) ){
                Hp = CritterGetHp(dude2);
                if( Hp >= FeatGetVal(dude2, 7) ){
                    if( crit == gObjDude ){
                        MsgLineA.Id = (dude2 != gObjDude) + 501; // 'You lookhealthy already.'
                        if( MessageGetMsg(&gSkillMsg, &MsgLineA) != 1 ) return -1;
                        if( dude2 == gObjDude ){
                            strcpy( stmp, MsgLineA.Text );
                        } else {
                            sprintf(stmp, MsgLineA.Text, ObjGetName(dude2));
                        }
                        IfcMsgOut(stmp);
                        v55 = 0;
                    }
                } else {
                    FadeStep(gFadePaletteC);
                    if( CritterGetBodyType(dude2) == 2 )
                        v11 = 1;
                    else
                        v11 = SkillUse(crit, SkillIdx, &v47, v53);
                    if( v11 == 2 || v11 == 3 ){
                        v47 = RandMinMax(Min + 1, Max + 5);
                        CritterHeal(dude2, v47);
                        if( crit == gObjDude ){
                            MsgLineA.Id = 500; // 'You heal %d hot points.'
                            if( MessageGetMsg(&gSkillMsg, &MsgLineA) != 1 ) return -1;
                            if( v48 - Val < v47 ) v47 = v48 - Val;
                            sprintf(stmp, MsgLineA.Text, v47);
                            IfcMsgOut(stmp);
                        }
                        dude2->Grid.DestMapId &= ~0x04;
                        SkillUsing(6);
                        v57 = 1;
                        if( dude2 == gObjDude ) IfaceRenderHP(1);
                    } else {
                        MsgLineA.Id = 503; // 'Ypu fail to do any healing.'
                        if( MessageGetMsg(&gSkillMsg, &MsgLineA) != 1 ) return -1;
                        sprintf(stmp, MsgLineA.Text, v47);
                        IfcMsgOut(stmp);
                    }
                    ScptMapUpdate();
                    FadeStep(gPalBase);
                }
                if( crit == gObjDude ) ScptTimeCap2(1800);
                goto LABEL_43;
            }
            MsgLineA.Id = RandMinMax(0, 2) + 512; // 'You can't heal dead.'
            if( MessageGetMsg(&gSkillMsg, &MsgLineA) != 1 ) goto LABEL_43;
            goto LABEL_103;
        case SKILL_DOCTOR:
            if( SkillUsingTired(7) == -1 ){
                MsgLineA.Id = RandMinMax(0, 2) + 590; // 
                if( MessageGetMsg(&gSkillMsg, &MsgLineA) != 1 ) IfcMsgOut( MsgLineA.Text );
                return -1;
            }
            if( CritterIsDead(dude2) ){
                MsgLineA.Id = RandMinMax(0, 2) + 512; //
                if( MessageGetMsg(&gSkillMsg, &MsgLineA) != 1 ) goto LABEL_43;
                goto LABEL_103;
            }
            v29 = CritterGetHp(dude2);
            if( v29 < FeatGetVal(dude2, 7) || CritterUnk30(dude2) ){
                FadeStep(gFadePaletteC);
                if( CritterGetBodyType(dude2) != 2 && CritterUnk30(dude2) ){
                    InjuresMask[0] = gSkillDoctorInjuresMask[0];
                    InjuresMask[1] = gSkillDoctorInjuresMask[1];
                    InjuresMask[2] = gSkillDoctorInjuresMask[2];
                    InjuresMask[3] = gSkillDoctorInjuresMask[3];
                    InjuresMask[4] = gSkillDoctorInjuresMask[4];
                    v31 = 0;
                    do{
                        if( (InjuresMask[ v31 ] & p_State->CombatResult) != 0 ){
                            v56++;
                            v33 = SkillUse(crit, SkillIdx, &v47, v53);
                            MsgLineA.Id = v31 + 530; // 'damaged eye'
                            if( MessageGetMsg(&gSkillMsg, &MsgLineA) != 1 ) return -1;
                            if( v33 == 2 || v33 == 3 ){
                                p_State->CombatResult &= ~InjuresMask[ v31 ];
                                v57 = 1;
                                MsgLineB.Id = (dude2 != gObjDude) + 520; // 'You heal your %s.'
                                v52 = 1;
                                SkillUsing(7);
                                dude2->Grid.DestMapId &= ~0x04;
                            } else {
                                MsgLineB.Id = (dude2 != gObjDude) + 525; // 'You fail to heal your %s.'
                            }
                            if( MessageGetMsg(&gSkillMsg, &MsgLineB) != 1 ) return -1;
                            sprintf(stmp, MsgLineB.Text, MsgLineA.Text);
                            IfcMsgOut(stmp);
                            SkillHoningAward(crit, SkillIdx, v57, a4);
                            v55 = 0;
                        }
                        v31++;
                    }while( v31 < 5 );
                }
                if( CritterGetBodyType(dude2) == 2 ){
                    v34 = 1;
                } else {
                    Total = SkillGetTotal(crit, SkillIdx);
                    v34 = RandUnk05(Total, v53, &v47);
                }
                if( v34 == 2 || v34 == 3 ){
                    v47 = RandMinMax(Min + 4, Max + 10);
                    CritterHeal(dude2, v47);
                    if( crit == gObjDude ){
                        MsgLineA.Id = 500; // 
                        if( MessageGetMsg(&gSkillMsg, &MsgLineA) != 1 ) return -1;
                        if( v48 - Val < v47 ) v47 = v48 - Val;
                        sprintf(stmp, MsgLineA.Text, v47);
                        IfcMsgOut(stmp);
                    }
                    if( !v52 ) SkillUsing(7);
                    v57 = 1;
                    dd = gObjDude;
                    dude2->Grid.DestMapId &= ~0x04;
                    if( dude2 == dd ) IfaceRenderHP(1);
                    SkillHoningAward(crit, SkillIdx, v57, a4);
                    v55 = 0;
                    ScptMapUpdate();
                    FadeStep(gPalBase);
                } else {
                    MsgLineA.Id = 503; // 'You fail to do any healing.'
                    if( MessageGetMsg(&gSkillMsg, &MsgLineA) != 1 ) return -1;
                    sprintf(stmp, MsgLineA.Text, v47);
                    IfcMsgOut(stmp);
                    ScptMapUpdate();
                    FadeStep(gPalBase);
                }
            } else if ( crit == gObjDude ){
                MsgLineA.Id = (dude2 != gObjDude) + 501; //
                if( MessageGetMsg(&gSkillMsg, &MsgLineA) != 1 ) return -1;
                if( dude2 == gObjDude ){
                    strcpy( stmp, MsgLineA.Text );
                } else {
                    sprintf(stmp, MsgLineA.Text, ObjGetName( dude2 ) );
                }
                IfcMsgOut(stmp);
                v55 = 0;
            }
            if( crit == gObjDude ){
                ScptTimeCap2( v56 * (( SkillIdx == 7 ) ? 3600 : 1800 ) );
                goto LABEL_94;
            }
            goto LABEL_43;
        case SKILL_SNEAK:
        case SKILL_LOCKPICK:
            goto LABEL_43;
        case SKILL_STEAL:
            ScptStealAttempt( crit, dude2 );
            goto LABEL_43;
        case SKILL_TRAPS:
            MsgLineA.Id = 551; // 'You fail to find any traps'
            if( MessageGetMsg( &gSkillMsg, &MsgLineA ) == 1 ) IfcMsgOut( MsgLineA.Text );
            return -1;
        case SKILL_SCIENCE:
            MsgLineA.Id = 552; // 'You fail to learn anything.'
            if( MessageGetMsg( &gSkillMsg, &MsgLineA ) == 1 ) IfcMsgOut( MsgLineA.Text );
            return -1;
        case SKILL_REPAIR:
            if( CritterGetBodyType( dude2 ) == 2 ){
                if( SkillUsingTired( SKILL_REPAIR ) == -1 ){
                    MsgLineA.Id = RandMinMax( 0, 2 ) + 590; // 
                    if( MessageGetMsg( &gSkillMsg, &MsgLineA ) == 1 ) IfcMsgOut( MsgLineA.Text );
                    return -1;
                }
                if( CritterIsDead(dude2) ){
                    MsgLineA.Id = 1101; // 'You got it?'
                    if( MessageGetMsg( &gSkillMsg, &MsgLineA ) == 1 )
LABEL_103:
                        IfcMsgOut( MsgLineA.Text );
                } else {
                    v17 = CritterGetHp( dude2 );
                    if( v17 < FeatGetVal(dude2, 7) || CritterUnk30(dude2) ){
                        InjuresMask[0] = gSkillUnk15[0];
                        InjuresMask[1] = gSkillUnk15[1];
                        InjuresMask[2] = gSkillUnk15[2];
                        InjuresMask[3] = gSkillUnk15[3];
                        InjuresMask[4] = gSkillUnk15[4];
                        FadeStep(gFadePaletteC);
                        v19 = 0;
                        do{
                            if( (InjuresMask[v19] & p_State->CombatResult) != 0 ){
                                v56++;
                                v21 = SkillUse(crit, SkillIdx, &v47, v53);
                                MsgLineA.Id = v19 + 530; // 'damaged eye'
                                if( MessageGetMsg(&gSkillMsg, &MsgLineA) != 1 ) return -1;
                                if( v21 == 2 || v21 == 3 ){
                                    p_State->CombatResult &= ~InjuresMask[v19];
                                    v57 = 1;
                                    MsgLineB.Id = (dude2 != gObjDude) + 520; // 
                                    SkillUsing(13);
                                    v22 = dude2->Grid.DestMapId & ~0x04;
                                    v52 = 1;
                                    dude2->Grid.DestMapId = v22;
                                } else {
                                    MsgLineB.Id = (dude2 != gObjDude) + 525;
                                }
                                if( MessageGetMsg(&gSkillMsg, &MsgLineB) != 1 ) return -1;
                                sprintf(stmp, MsgLineB.Text, MsgLineA.Text);
                                IfcMsgOut(stmp);
                                SkillHoningAward(crit, SkillIdx, v57, a4);
                                v55 = 0;
                            }
                            v19++;
                        }while( v19 < 5 );
                        v23 = SkillGetTotal( crit, SkillIdx );
                        v24 = RandUnk05( v23, v53, &v47 );
                        if( v24 == 2 || v24 == 3 ){
                            v47 = RandMinMax( Min + 4, Max + 10 );
                            CritterHeal( dude2, v47 );
                            if( crit == gObjDude ){
                                MsgLineA.Id = 500;
                                if( MessageGetMsg( &gSkillMsg, &MsgLineA ) != 1 ) return -1;
                                if( v48 - Val < v47 ) v47 = v48 - Val;
                                sprintf( stmp, MsgLineA.Text, v47 );
                                IfcMsgOut( stmp );
                            }
                            if( !v52 ) SkillUsing( 13 );
                            db = gObjDude;
                            v26 = dude2->Grid.DestMapId & 0xFB;
                            v57 = 1;
                            dude2->Grid.DestMapId = v26;
                            if( dude2 == db ) IfaceRenderHP(1);
                            SkillHoningAward( crit, SkillIdx, v57, a4 );
                            v55 = 0;
                            ScptMapUpdate();
                            FadeStep(gPalBase);
                        } else {
                            MsgLineA.Id = 503;
                            if( MessageGetMsg( &gSkillMsg, &MsgLineA ) != 1 ) return -1;
                            sprintf( stmp, MsgLineA.Text, v47 );
                            IfcMsgOut( stmp );
                            ScptMapUpdate();
                            FadeStep( gPalBase );
                        }
                    } else if( crit == gObjDude ){
                        MsgLineA.Id = (dude2 != gObjDude) + 501;
                        if( MessageGetMsg( &gSkillMsg, &MsgLineA ) != 1 ) return -1;
                        sprintf( stmp, MsgLineA.Text, ObjGetName( dude2 ) );
                        IfcMsgOut( stmp );
                        v55 = 0;
                    }
                    if( crit == gObjDude ){
                        ScptTimeCap2( ( SkillIdx == 7 ) ? 3600 * v56 : 1800 * v56 );
LABEL_94:
                    }
                }
LABEL_43:
                if( v55 ) SkillHoningAward( crit, SkillIdx, v57, a4 );
                if( SkillIdx == SKILL_FIRSTAID || SkillIdx == SKILL_DOCTOR ) ScptMapUpdate();
                return 0;
            } else {
                MsgLineA.Id = 553; // ' You cannot repair that.'
                if( MessageGetMsg( &gSkillMsg, &MsgLineA ) == 1 ) IfcMsgOut( MsgLineA.Text );
                return -1;
            }
        default:
            MsgLineA.Id = 510; // 'skill use: invalid skill used.'
            if( MessageGetMsg( &gSkillMsg, &MsgLineA ) == 1 ) eprintf( "%s", MsgLineA.Text );
            return -1;
    }
}

int SkillUseSteal( Obj_t *dude1, Obj_t *dude2, Obj_t *Company, int a3 )
{
    int tmp, v24, Awareness, v16, v11;
    int CatchChance;
    char stmp[60];
    MsgLine_t MsgLine;
DD
    tmp = 0;
    Awareness = 1 - gSkillUnk60;
    if( dude1 != gObjDude || !PerkLvl( dude1, PERK_PICKPOCKET ) ){
        Awareness -= 4 * Item24( Company );
        if( OBJTYPE( dude2->ImgId ) == TYPE_CRIT ) ActionUnk16( dude1, dude2 );
    }
    if( dude2->Critter.State.CombatResult & 0x03 ) Awareness += 20;
    v24 = Awareness + SkillGetTotal( dude1, SKILL_STEAL );
    if( v24 > 95 ) v24 = 95;
    if( dude1 == gObjDude && PartyMembRdy( dude2 ) ){
        v11 = 3;
    } else {
        v11 = RandUnk05( v24, FeatGetVal( dude1, FEAT_CRIT ), &tmp );
    }
    if( v11 == 3 ){
        CatchChance = 0;
    } else if( v11 ){
        if( OBJTYPE( dude2->Pid ) == TYPE_CRIT ){
            v16 = SkillGetTotal( dude2, SKILL_STEAL ) - Awareness;
        } else {
            v16 = 30 - Awareness;
        }
        CatchChance = RandUnk05( v16, 0, &tmp );
    } else {
        CatchChance = 2;
    }
    if( CatchChance == 2 || CatchChance == 3 ){
        MsgLine.Id = 570 + 2 * a3; // you're caught stealing the %s
        if( MessageGetMsg( &gSkillMsg, &MsgLine ) != 1 ) return -1;
        sprintf( stmp, MsgLine.Text, ObjGetName( Company ) );
        IfcMsgOut( stmp );
        return 0;
    } else {
        MsgLine.Id = 570 + 2 * a3 + 1; // you steal %s
        if( MessageGetMsg( &gSkillMsg, &MsgLine ) != 1 ) return -1;
        sprintf( stmp, MsgLine.Text, ObjGetName( Company ) );
        IfcMsgOut( stmp );
        return 1;
    }
}

int SkillGetDifficulty( int SkillIdx )
{
    int diff;
    int tmp;

    diff = 0;
    switch( SkillIdx ){
        case 6 ... 17:
            tmp = 1;
            CfgGetInteger( &gConfiguration, "preferences", "game_difficulty", &tmp );
            if( tmp ){
                if( tmp == 2 ) diff = -10;
            } else {
                diff = 20;
            }
            break;
        default:
            return 0;
    }
    return diff;
}

int SkillUsingTired( int SkillIdx )
{
    int i;

    for( i = 0; (i < 3) && gSkillUsage[ SkillIdx ][ i ]; i++ );    
    if( i < 3 ) return i;
    return ( (( RandUnk03() - gSkillUsage[ SkillIdx ][ 0 ] ) / 36000) <= 24 ) ? -1 : 2;
}

int SkillUsing( int SkillIdx )
{
    int n, i;

    if( ( n = SkillUsingTired( SkillIdx ) ) == -1 ) return -1;
    if( gSkillUsage[ SkillIdx ][ n ] ){
        for( i = 0; i < n; i++ ){
            gSkillUsage[ SkillIdx ][ i ] = gSkillUsage[ SkillIdx ][ i + 1 ];
        }
        gSkillUsage[ SkillIdx ][ i ] = RandUnk03();
    } else {
        gSkillUsage[ SkillIdx ][ 0 ] = RandUnk03();
    }
    return 0;
}

int SkillUsingClear()
{
    memset(gSkillUsage, 0, sizeof(gSkillUsage));
    return 0;
}

int SkillUsingSave( xFile_t *fh )
{
    return ( dbputBeiBlk( fh, (int *)gSkillUsage, sizeof( gSkillUsage ) / sizeof(int) ) != -1 ) - 1;
}

int SkillUsingLoad( xFile_t *fh )
{    
    return (dbreadBeiBlk( fh, (int *)gSkillUsage, sizeof( gSkillUsage ) / sizeof(int) ) != -1) - 1;
}

char *SkillUseTryWantMsg( int Want )
{
    MsgLine_t MsgLine;
    int MsgId, pool;

    MsgId = 999999;
    pool = 0;
    if( Want ){
        if( Want == 1 ){
            pool = 4;
            MsgId = 1100;
        }
    } else {
        pool = 5;
        MsgId = 1000;
    }
    return MessageGetMessage( &gSkillMsg, &MsgLine, RandMinMax( 0, pool ) + MsgId );
}

int SkillDexMenu()
{
    int ability = -1, sel;

    if( SkillMenuOpen() == -1 ){
	eprintf( "\n ** Error loading skilldex dialog data! **\n" );
	return -1;
    }    
    while( ability == -1 ){
        sel = InpUpdate();
        if( sel == 27 || sel == 500 || gMenuEscape ){
            ability = 0;
        } else if ( sel == 13 ) {
            GSoundPlay( "ib1p1xx1" );
            ability = 0;
        } else if( sel >= 501 && sel <= 509 ) {
            ability = sel - 500;
        }
    }
    if( ability ) TimerWait( 111 );
    SkillMenuClose();
    return ability;
}

int SkillMenuOpen()
{
    int size, i, Width, tot, h, w, butt, y;
    char toggle,*Message, *ps, *str, stmp[ 1024 ];

    gSkillMenuFontSave = FontGetCurrent();
    gSkillMenuInited = 0;
    GmouseUnk03();
    GmouseLoadCursor( 1 );
    if( MessageInit( &gSkillMenuMsg ) != 1 ) return -1;
    sprintf( stmp, "%s%s", gGamePath, "skilldex.msg" );
    if( MessageLoad( &gSkillMenuMsg, stmp ) != 1 ) return -1;
    for( i = 0; i < 6; i++ ){
        if( !( gSkillMenuPix[ i ] = ArtLoadBmp( ArtMakeId( 6, gSkillMenuIds[ i ], 0, 0, 0), &gSkillMenuImgs[ i ], &gSkillMenuGeo[ i ].Width, &gSkillMenuGeo[ i ].Height ) ) ) break;
    }

    if( i < 6 ){
        for( i--; i > -1; i-- ) ArtClose( gSkillMenuImgs[ i ] );
        MessageClose( &gSkillMenuMsg ); 
        return -1;
    }

    toggle = 0;
    for( i = 0; i < 16; i++ ){        
        if( !(gSkillMenuSurfaces[ i ] = (char *)Malloc( gSkillMenuGeo[1].Height * gSkillMenuGeo[1].Width + 512 )) ) break;
        toggle = (toggle ^ 1) & 1;
        if( toggle ){
            size = gSkillMenuGeo[2].Width * gSkillMenuGeo[2].Height;
            ps = gSkillMenuPix[2];
        } else {
            size = gSkillMenuGeo[1].Width * gSkillMenuGeo[1].Height;
            ps = gSkillMenuPix[1];
        }
        memcpy( gSkillMenuSurfaces[ i ], ps, size );
    }

    if( i < 16 ){
        for( i--; i > -1; i-- ) Free( gSkillMenuSurfaces[ i ] );        
        for( i = 0; i < 6; i++ ) ArtClose( gSkillMenuImgs[ i ] );
        MessageClose( &gSkillMenuMsg ); 
        return -1;
    }

    gSkillMenuWin = WinCreateWindow( 640 - gSkillMenuGeo[0].Width - 4, 379 - gSkillMenuGeo[0].Height - 6, gSkillMenuGeo[0].Width, gSkillMenuGeo[0].Height, 256, 18 );

    if( gSkillMenuWin == -1 ){
        for( i = 0; i < 16; i++ ) Free( gSkillMenuSurfaces[ i ] );
        for( i = 0; i < 6; i++ ) ArtClose( gSkillMenuImgs[ i ] );
        MessageClose( &gSkillMenuMsg ); 
        return -1; 
    }
    gSkillMenuInited = MapAmbientDisable();
    CycleColorStop();
    GmouseLoadCursor(1);

    gSkillMenuSurf = WinGetSurface( gSkillMenuWin );
    memcpy( gSkillMenuSurf, gSkillMenuPix[ 0 ], gSkillMenuGeo[ 0 ].Width * gSkillMenuGeo[ 0 ].Height );
    FontSet( 103 );
    Width = gSkillMenuGeo[ 0 ].Width;
    Message = MessageGetMessage( &gSkillMenuMsg, &gSkillMenuMsgLine, 100 );
    gFont.Print( &gSkillMenuSurf[ 14 * gSkillMenuGeo[0].Width + 55 ], Message, Width, Width, gPalColorCubeRGB[18][17][3] );
    y = 48;
    for( i = 0; i < 8; i++, y += 36 ){
        tot = SkillGetTotal( gObjDude, gSkillMenuIdx[ i ] );
        if( tot == -1 ) tot = 0;
        ScrCopy( &gSkillMenuPix[ 5 ][ 14 * (tot / 100)], 14, 24, 336, &gSkillMenuSurf[ y * gSkillMenuGeo[ 0 ].Width + 110 ], gSkillMenuGeo[ 0 ].Width );
        tot %= 100;
        ScrCopy( &gSkillMenuPix[ 5 ][ 14 * (tot / 10)], 14, 24, 336, &gSkillMenuSurf[ y * gSkillMenuGeo[ 0 ].Width + 124 ], gSkillMenuGeo[ 0 ].Width );
        ScrCopy( &gSkillMenuPix[ 5 ][ 14 * (tot % 10)], 14, 24, 336, &gSkillMenuSurf[ y * gSkillMenuGeo[ 0 ].Width + 138 ], gSkillMenuGeo[ 0 ].Width );
    }
    y = 45;
    h = ( (gSkillMenuGeo[2].Height - gFont.ChrHeight()) / 2 ) + 1;
    for( i = 0; i < 8; i++, y += 36 ){
        strcpy( stmp, MessageGetMessage( &gSkillMenuMsg, &gSkillMenuMsgLine, 102 + i ) );
        w = ( (gSkillMenuGeo[2].Width - gFont.LineWidth( stmp ) ) / 2 ) + 1;
        if( w < 0 ) w = 0;
        gFont.Print( &gSkillMenuSurfaces[i*2 + 0][w + gSkillMenuGeo[1].Width * h], stmp, gSkillMenuGeo[1].Width, gSkillMenuGeo[1].Width, gPalColorCubeRGB[18][17][3] );
        gFont.Print( &gSkillMenuSurfaces[i*2 + 1][w + gSkillMenuGeo[2].Width * h], stmp, gSkillMenuGeo[2].Width, gSkillMenuGeo[2].Width, gPalColorCubeRGB[14][12][3] );
        butt = WinCreateButton( gSkillMenuWin, 15, y, gSkillMenuGeo[2].Width, gSkillMenuGeo[2].Height, -1, -1, -1, i + 501, gSkillMenuSurfaces[i*2 + 0], gSkillMenuSurfaces[i*2 + 1], 0, 32 );
        if( butt != -1 ) WinSetClickSound( butt, (void *)GSoundPlayKnobLt, (void *)GSoundPlayKnobRt );
    }
    w = gSkillMenuGeo[0].Width;
    str = MessageGetMessage( &gSkillMenuMsg, &gSkillMenuMsgLine, 101 );
    gFont.Print( &gSkillMenuSurf[337 * gSkillMenuGeo[0].Width + 72], str, w, w, gPalColorCubeRGB[18][17][3] );
    butt = WinCreateButton( gSkillMenuWin, 48, 338, gSkillMenuGeo[3].Width, gSkillMenuGeo[3].Height, -1, -1, -1, 500, gSkillMenuPix[3], gSkillMenuPix[4], 0, 32 );
    if( butt != -1 ) WinSetClickSound(butt, GSoundPlayPushBt, GSoundPlayReleaseBt);
    WinUpdate( gSkillMenuWin );
    return 0;
}

void SkillMenuClose()
{
    int i;

    WinClose( gSkillMenuWin );
    for( i = 0; i != 16;  i++ ) Free( gSkillMenuSurfaces[ i ] ); 
    for( i = 0; i != 6;  i++ ) ArtClose( gSkillMenuImgs[ i ] );
    MessageClose(&gSkillMenuMsg);
    FontSet( gSkillMenuFontSave );
    if( gSkillMenuInited ) MapAmbientEnable();
    CycleColorStart();
    GmouseLoadCursor( 1 );
}

