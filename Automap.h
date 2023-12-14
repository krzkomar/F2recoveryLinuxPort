#pragma once

typedef struct 
{
  char c01;
  int i01;
  int tab[480];
} Automap_t;

int AutomapSave();
int AutomapFSave( xFile_t * );
int AutomapFLoad( xFile_t * );