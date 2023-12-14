#include "FrameWork.h"

static Regions_t *gRegions = NULL;

void RegionsPurge()
{
    Regions_t *p;

    while( gRegions ){
    	p = p->Prev;
        Free( gRegions );
        gRegions = p;
    }
}

#define AREA_COPY	\
            pNew->Area.lt = regArea.lt;\
            pNew->Area.rt = regArea.rt;\
            pNew->Area.tp = regArea.tp;\
            pNew->Area.bm = regArea.bm;

#define AREA_LINK	\
            pNew->Prev = *RegionList;\
            *RegionList = pNew;\
            RegionList = &pNew->Prev;

void RegionsUpdate( Regions_t **RegionList, VidRect_t *ObjArea )
{
    Regions_t *region, *tmp, *pNew;
    VidRect_t mse, regArea;

    mse = *ObjArea;
    while( (region = *RegionList) ){
        if( mse.rt < region->Area.lt  || // on the left
    	    mse.bm < region->Area.tp  || // over
    	    mse.lt > region->Area.rt  || // on the right
    	    mse.tp > region->Area.bm     // below
    	){ // object position is outside region
            RegionList = &region->Prev;
            continue;
        }

        // object is inside region
        regArea = region->Area;
        *RegionList = region->Prev; // point to Prev region
        tmp = gRegions;
        gRegions = region;
        region->Prev = tmp;

        if( regArea.tp < mse.tp ){ // move cursor down, update upper area
            if( !(pNew = RegionsPop()) ){ printf("XXX\n"); break; }
            AREA_COPY
            pNew->Area.bm = mse.tp - 1;
	    AREA_LINK
            regArea.tp = mse.tp;
        }
        if( regArea.bm > mse.bm ){ // move cursor up, update area below
            if( !(pNew = RegionsPop()) ){ printf("XXX\n");break; }
	    AREA_COPY
            pNew->Area.tp = mse.bm + 1;
	    AREA_LINK
            regArea.bm = mse.bm;
        }
        if( regArea.lt < mse.lt ){ // move cursor right, update left area
            if( !(pNew = RegionsPop()) ) break;
	    AREA_COPY
            pNew->Area.rt = ObjArea->lt - 1;
	    AREA_LINK    
        }
        if( regArea.rt > mse.rt ){ // move cursor left, update right area
            if( !(pNew = RegionsPop()) ) break;
	    AREA_COPY
            pNew->Area.lt = mse.rt + 1;
	    AREA_LINK    
        }
    }
}

Regions_t *RegionsUnk01( VidRect_t *a1, VidRect_t *a2 )
{
    Regions_t **p_pRet;
    int i;
    Regions_t *r;
    Regions_t *pNew;
    VidRect_t *p_tp;
    VidRect_t tmp[4];
    VidRect_t tighten;
    Regions_t *pRet;

    pRet = 0;
    if( RegionShrink(a2, a1, &tighten) ){
        pNew = RegionsPop();
        pRet = pNew;
        if( pNew ){
            pNew->Area.lt = a1->lt;
            p_tp = a1;
            pNew->Area.tp = p_tp->lt;
            p_tp = (p_tp + 4);
            pNew->Area.rt = p_tp->lt;
            pNew->Area.bm = p_tp->tp;
            pRet->Prev = 0;
        }
    } else {
        tmp[0].lt = a1->lt;
        tmp[0].tp = a1->tp;
        tmp[0].rt = a1->rt;
        tmp[0].bm = tighten.tp - 1;
        tmp[1].lt = a1->lt;
        tmp[1].tp = tighten.tp;
        tmp[1].rt = tighten.lt - 1;
        tmp[1].bm = tighten.bm;
        tmp[2].lt = tighten.rt + 1;
        tmp[2].tp = tighten.tp;
        tmp[2].rt = a1->rt;
        tmp[2].bm = tighten.bm;
        tmp[3].lt = a1->lt;
        tmp[3].tp = tighten.bm + 1;
        tmp[3].rt = a1->rt;
        tmp[3].bm = a1->bm;
        p_pRet = &pRet;
        for( i = 0; i < 4; i++ ){
            if( tmp[i].rt >= tmp[i].lt && tmp[i].bm >= tmp[i].tp ){
                r = RegionsPop();
                *p_pRet = r;
                if( !r ) return r;
                r->Area.lt = tmp[i].lt;
                r->Area.tp = tmp[i].tp;
                r->Area.rt = tmp[i].rt;
                r->Area.bm = tmp[i].bm;
                (*p_pRet)->Prev = 0;
                p_pRet = &(*p_pRet)->Prev;
            }
        }
    }
    return pRet;
}

Regions_t *RegionsPop()
{
    Regions_t *pNew, *tmp;
    int i;

    if( gRegions == NULL ){ // create 10 linked regions
        for( i = 0; i < 10; i++ ){
            if( !(pNew = Malloc( sizeof( Regions_t ) )) ) break;
            tmp = gRegions;
            gRegions = pNew;
            pNew->Prev =tmp;
        }
    }
    if( gRegions == NULL ) return NULL;
    tmp = gRegions;
    gRegions = gRegions->Prev;
    return tmp;
}

void RegionsPush( Regions_t *newOne )
{
    Regions_t *tmp;
    
    tmp = gRegions;
    gRegions = newOne;
    newOne->Prev = tmp;
}

void RegionExpand( VidRect_t *Area1, VidRect_t *Area2, VidRect_t *AreaOut )
{
    AreaOut->lt = ( Area1->lt < Area2->lt ) ? Area1->lt : Area2->lt;
    AreaOut->tp = ( Area1->tp < Area2->tp ) ? Area1->tp : Area2->tp;
    AreaOut->rt = ( Area1->rt > Area2->rt ) ? Area1->rt : Area2->rt;
    AreaOut->bm = ( Area1->bm > Area2->bm ) ? Area1->bm : Area2->bm;
}

int RegionShrink( VidRect_t *Area1, VidRect_t *Area2, VidRect_t *AreaOut )
{
    *AreaOut = *Area1;
    if( Area1->lt > Area2->rt || Area2->lt > Area1->rt || Area2->bm < Area1->tp || Area2->tp > Area1->bm ) return -1;
    if( Area2->lt > Area1->lt ) AreaOut->lt = Area2->lt;
    if( Area2->tp > Area1->tp ) AreaOut->tp = Area2->tp;
    if( Area2->rt < Area1->rt ) AreaOut->rt = Area2->rt;
    if( Area2->bm < Area1->bm ) AreaOut->bm = Area2->bm;
    return 0;
}



