#ifndef _SglDirBlower_H
#define _SglDirBlower_H
#include  "types.h"
#include  "MotionTask.h"
#define SBL_RUNING       1
#define SBL_STOP         2
#define SBL_PAUSE5SEC    3
uint8_t getSglBlowerState(CtrCfg_STR *pContr,uint8_t bnum);
void DealSgldirBlower(CtrCfg_STR *pContr);
void checkSblStatus(CtrCfg_STR *pContr);
#endif
