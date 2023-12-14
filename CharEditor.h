#pragma once

#define CHREDT_COL_SELECT	gPalColorCubeRGB[18][17][3]
#define CHREDT_COL_DIMMED 	gPalColorCubeRGB[14][12][3]
#define CHREDT_COL_LVL_A	gPalColorCubeRGB[31][31][11]
#define CHREDT_COL_LVL_B	gPalColorCubeRGB[0][31][0]
#define CHREDT_COL_LVL_C	gPalColorCubeRGB[15][15][5]
#define CHREDT_COL_LVL_D	gPalColorCubeRGB[1][9][1]
#define CHREDT_COL_LVL_E	gPalColorCubeRGB[31][18][8]
#define CHREDT_COL_LVL_F 	gPalColorCubeRGB[31][0][0]
#define CHREDT_COL_LVL_G	gPalColorCubeRGB[31][31][31]
#define CHREDT_COL_LVL_H	gPalColorCubeRGB[20][20][20]
#define CHREDT_COL_LVL_I	gPalColorCubeRGB[0][0][0]

typedef struct 
{
  int Val;
  int ImgId;
  int TitleId;
  int DscId;
} KarmaMsg_t;

typedef struct 
{
  int i01;
  int i02;
} GenreMsg_t;

/**********************************************************/
extern int gChrEditUnspentChrPts;
extern int gChrEditKarmaIds2[8];

int CharEditMenu( int mode );
int CharEditMenuCreate();
void CharEditClose();
void CharEditClean();
void CharEditInit();
int CharEditLineEditor( int WinId, int KeyDone, char *Text, int MaxLen, int PosX, int PosY, int FgColor, int BgColor, char a9 );
int CharEditStrChrMark( int chr );
char *CharEditFnameChgExt( char *NewFileName, char *OldFileName, char *NewExt );
void CharEditShowFolder();
void CharEditShowPerks();
int CharEditStrCmp( char **pStr1, char **pStr2 );
unsigned int CharEditShowKills();
void CharEditCounterDisplay( int Xpos, int Ypos, int flg, int a4, int a5, int WinId );
void CharEditShowLevelInfo();
void CharEditShowStats( int Id, int a1, int a2 );
void CharEditShowHeroGender();
void CharEditShowHeroAge();
void CharEditShowHeroName();
void CharEditShowParams();
void CharEditShowSkills( int SkillLock );
void CharEditDscUpdate();
int CharEditDialogName();
void CharEditPrintName( char *surf, int Width );
int CharEditDialogAge();
int CharEditDialogGender();
void CharEditStatModify( int Sel );
int CharEditPrint();
int CharEditFileExist( const char *fname );
int CharEditPrintToFile( char *fname );
void CharEditUnk57();
void CharEditStrClear( char *str, int len );
void CharEditStrFillDot( char *str, int len );
int CharEditAreasDefine();
int CharEditCharReset();
void CharEditStoreObj();
void CharEditRestoreObj();
char *CharEditValToStr( int Val, char *str );
int CharEditCaption( int ImgId, char *Title, char *Param, char *Dsc );
int CharEditFindPerk( int PerkId );
void CharEditFolderSelect();
void CharEditAreaSelect( int sel );
void CharEditSkillUpDn( unsigned int sel );
int CharEditTagSkillsAvail();
void CharEditTagSkillChoose( int Sel );
void CharEditShowTraits();
int CharEditOptTraitsAvail();
void CharEditSpecTraitChoose( int Sel );
char *CharEditShowKarma();
int CharEditSaveLvl( xFile_t *fh );
int CharEditLoadLvl( xFile_t *fh );
void CharEditStartChar();
int CharEditLvlUp();
void CharEditPerkListCb();
int CharEditPerkDialog();
int CharEditListMenu( int Avail, void (*Cb)() );
int CharEditPerkDlgList();
void CharEditLoseTraitCb();
int CharEditLoseTrait();
void CharEditPickPerkCb();
int CharEditPickPerk();
void CharEditPickPerkUpdate();
int CharEditLoseTraitUpdate( int a1 );
int CharEditDscPanel( int ImgId, char *Name, char *Param, char *Dsc );
void CharEditPerksSave();
void CharEditPerksApply();
int CharEditPerkLimit();
int CharEditStatOverFlow();
void CharEditFolderArrows();
void CharEditFoldArrDelete();
void CharEditFolderUpDn( int dir );
void CharEditFoldArrowsHandle();
int CharEditPerksFoldTitle( char *str );
int CharEditFoldLine( char *Str );
int CharEditFoldKillsLine( char *Name, int Val );
int CharEditLoadKarma();
void CharEditFreeKarma();
int CharEditLoadGenre();
void CharEditFreeGenre();



