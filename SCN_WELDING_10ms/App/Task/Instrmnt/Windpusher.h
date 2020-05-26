#ifndef _Windpusher_H
#define _Windpusher_H
#include  "types.h"
#include  "MotionTask.h"
//ÃÅ´°ÍÆ¸Ë¿ØÖÆÆ÷×´Ì¬
#define WIND_CLOSED    1
#define WIND_CLOSING   2
#define WIND_OPENED    3
#define WIND_OPENING   4
#define WIND_PAUSE     5
#define WIND_PAUSE3SEC 6
uint8_t getPusherState(CtrCfg_STR *pContr,uint8_t wnum);
void DealWindPusher(CtrCfg_STR *pContr);
void checkPusherStatus(CtrCfg_STR *pContr);
#endif
