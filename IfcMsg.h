#pragma once
/*
int gIfcMsgEnable = 0;
VidRect_t gIfcMsgArea = { 0x17, 0x18, 0x0BD, 0x53 };
int gIfcMsgButtDn = -1;
int gIfcMsgButtUp = -1;
char gIfcMsgBufLines[79];
char gIfcMsgUnk12[7921];
char *gIfcMsgSurf;
int gIfcMsgDisplLines;
int gIfcMsgBtEnable;
int gIfcMsgUnk06;
int gIfcMsgWidth;
int gIfcMsgUnk03;
int gIfcMsgUnk05;
int gIfcMsgTime;
*/

int IfcMsgInit();
void IfcMsgFlush();
void IfcMsgClose();
void IfcMsgOut( char *str );
void IfcMsgUpdate();
void IfcMsgButtUp();
void IfcMsgButtDn();
void IfcMsgSetCursorA();
void IfcMsgSetCursorB();
void IfcMsgSetCursorC();
void IfcMsgButtDisable();
void IfcMsgButtEnable();

