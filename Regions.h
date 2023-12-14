#pragma once

typedef struct Regions_t
{
    VidRect_t Area;
    struct Regions_t *Prev;
} Regions_t;

void RegionsPurge();
void RegionsUpdate( Regions_t **RegionList, VidRect_t *ObjArea );
Regions_t *RegionsUnk01( VidRect_t *a1, VidRect_t *a2 );
Regions_t *RegionsPop();
void RegionsPush( Regions_t *newOne );
void RegionExpand( VidRect_t *Area1, VidRect_t *Area2, VidRect_t *AreaOut );
int RegionShrink( VidRect_t *Area1, VidRect_t *Area2, VidRect_t *AreaOut );

