#pragma once

#define GAME_NAME	"FALLOUT II"

/*
int gMainUnk03;
int gMainUnk05 = 0;
char **gMainFileList = NULL;
int gMainUnk06 = 0;
int gMainUnk07 = 0;
int gMainUnk08 = 0;
int gMainUnk01 = 0;
extern void *gCharEditKarma;
*/

void MainGameVersion( char *str );
void MainGame( int argc, char **argv );
int MainInit( int argc, char **argv );
int MainReset();
void MainKill();
int MainGameCreate( char *MapName );
int MainUnk01();
int MainUnk02();
void MainGameLoop();
int MainUnk03();
void MainFilesClose();
void MainSelectRecordMap();
void MainUnk04();
void MainUnk05();
int MainUnk08( char *a1, char *path );
int MainUnk06( char *str, int LineWidth, short *pOffs, short *pLines );


