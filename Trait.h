#pragma once

typedef struct // size of 12
{
    char 	*Name;	// name of trait
    char 	*Dsc;	// short description
    int 	Id;	// identifier
} Trait_t;

enum{
    TRAIT_FAST_METABOLISM = 0,	// 0
    TRAIT_BRUISER,		// 1
    TRAIT_SMALL_FRAME,		// 2
    TRAIT_ONE_HANDER,		// 3
    TRAIT_FINESSE,		// 4
    TRAIT_KAMIKAZE,		// 5
    TRAIT_HEAVY_HANDED,		// 6
    TRAIT_FAST_SHOT,		// 7
    TRAIT_BLOODY_MESS,		// 8
    TRAIT_JINXED,		// 9 critical failures increased
    TRAIT_GOOD_NATURED,		// 10
    TRAIT_CHEM_RELIANT,		// 11
    TRAIT_CHEM_RESISTANT,	// 12
    TRAIT_SEX_APPEAL,		// 13
    TRAIT_SKILLED,		// 14
    TRAIT_GIFTED		// 15
};


//Trait_t gTraits[ 16 ];
//Msg_t gTraitMsg;
//int gTraitSpec[ 2 ];

int  TraitInit();
void TraitSpecReset();
void TraitClose();
int  TraitLoad( xFile_t *fh );
int  TraitSave( xFile_t *fh );
void TraitSetSpec( int s1, int s2 );
void TraitGetSpec( int *Spec1, int *Spec2 );
char *TraitGetName( unsigned int Id );
char *TraitGetDsc( unsigned int Id );
int  TraitGetId( unsigned int Id );
int  TraitSpecActive(  int id );
int  TraitSpecBonus( int Id );
int  TraitSpecBoost( unsigned int Id );
