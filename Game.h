#pragma once

#define CURSOR_CLOCK	26

extern int gMapperEnable;
extern int gSysMapperEnable;
extern int gGameIface;

int GameSysInit( char *ProgName, int flag1, int Font, int Flags, int argc, char **argv );
void GameReset();
void GameExit();
int GameProcess( int sel, int a2 );
void GameIfaceDisable( int IfaceMode );
void GameIfaceEnable();
int GameIfaceStat();

