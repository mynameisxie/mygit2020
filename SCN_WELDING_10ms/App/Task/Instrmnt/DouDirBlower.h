#ifndef _DouDirBlower_H
#define _DouDirBlower_H
#include  "types.h"
#include  "MotionTask.h"
#define DBL_NGTV_RUNING   1
#define DBL_PSTV_RUNING   2
#define DBL_STOP          3
#define DBL_PAUSE5SEC     4
uint8_t getDouBlowerState(CtrCfg_STR *pContr,uint8_t bnum);
void DealDoudirBlower(CtrCfg_STR *pContr);
void checkDblStatus(CtrCfg_STR *pContr);
#endif


