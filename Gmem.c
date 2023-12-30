#include "FrameWork.h"

int GmemSetup()
{
    CfgSetMemMng( Malloc, Realloc, Free );
//    SetMemMng2(MallocCb1, ReallocCb1, FreeCb1);
    return 0;
}

