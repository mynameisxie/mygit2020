#ifndef _NormalIO_H
#define _NormalIO_H
#include  "types.h"
#include  "MotionTask.h"
#define IO_OPEN       1
#define IO_CLOS       2
uint8_t getNormalState(CtrCfg_STR *pContr,uint8_t bnum);
void DealNormalIO(CtrCfg_STR *pContr);
void checkIOStatus(CtrCfg_STR *pContr);
#endif
