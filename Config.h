#pragma once

#define CFG_ROOT_MARKER		((void *)0xFEBAFEBA)

typedef struct 
{
    char *Name;
    union{
	struct _Config_t *SubSect;
	 char	*sValue;
    };
} ConfigTree_t;

typedef struct 
{
  int (*Read)(FILE *fh, void *data, int size, int k );
  int (*Write)(FILE *fh, void *data, int size, int k );
  int Args[3];
} ConfigMtd_t;

typedef struct _Config_t
{
    char 		*Value;
    int 		nSect;
    int 		RootCount;
    int 		RootAllocated;
    ConfigMtd_t		Methods;
    union{
	ConfigTree_t 	*Section;
	int		iDat;
    };
} Config_t;

/* plik 1 */
void *CfgMallocMem( unsigned int nsize );
void *CfgReallocMem( void *ptr, size_t size );
void CfgFreeMem( void *ptr );
int  CfgInitSection( Config_t *cfg, int SectCount, ConfigMtd_t *Methods, int sectsize );
int  CfgSectionExpand( Config_t *cfg, int size );
int  CfgDelete( Config_t *cfg );
int  CfgLookupSection( Config_t *Cfg, const char *Name, int *Position );
int  CfgFindSection( Config_t *Cmd, const char *Name );
int  CfgDeleteSection( Config_t *cfg, char *SectionName );
int  CfgAttachBranch( Config_t *cfg, Config_t *branch );
int  CfgUnk01( Config_t *cfg, const char *SecName, int Position );
int  CfgGetFileInt( FILE *fh, int *Val );
int  CfgRootLoad( FILE *fh, Config_t *cfg );
int  CfgLoadData( FILE *fh, Config_t *cfg, int args );
int  CfgPutFileInt( FILE *fh, int n );
int  CfgSaveRoot( FILE *fh, Config_t *cfg );
int  CfgSaveData( FILE *fh, Config_t *cfg, int arg );
void CfgSetMemMng( void *(*malloc_)(unsigned int nsize), void *(*realloc_)(void *ptr, size_t size ), void (*free_)(void *) );

/* plik 2 */
int  CfgInit( Config_t *cfg );
void CfgFree( Config_t *Cfg );
int  CfgCmdLineParser( Config_t *Cfg, int Argc, char **Argv );
int  CfgGetString( Config_t *Cfg, const char *SectionName, const char *KeyName, char **pStr );
int  CfgSetString( Config_t *Cfg, char *SectionName, char *KeyName, char *sValue );
int  CfgGetInteger( Config_t *Cfg, const char *SectionName, const char *EntryName, int *iValue );
int  CfgGetIntParamList( Config_t *Cfg, const char *SectionName, char *EntryName, int *ValueList, int ParametersNo );
int  CfgSetIntParamList( Config_t *Cfg, char *SectionName, char *EntryName, int *ValueList, int ParametersNo );
int  CfgSetInteger( Config_t *Cfg, char *SectionName, char *KeyName, int iValue );
int  CfgLoadFromFile( Config_t *Cfg, const char *CfgPath, int DatabaseFlag );
int  CfgSaveFile( Config_t *Cfg, const char *CfgPath, int Mode );
int  CfgParser( Config_t *Cfg, char *input );
int  CfgFetchKeyValuePair( char *input, char *key_name, char *value );
int  CfgSetEntry( Config_t *cfg, char *SectName );
int  CfgValidateSectionName( char *str );
int  CfgGetFloat( Config_t *cfg, char *SectName, char *KeyName, double *Val );
int  CfgSetDouble( Config_t *Cfg, char *SectionName, char *KeyName, double dValue );

