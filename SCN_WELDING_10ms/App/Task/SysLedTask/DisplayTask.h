#ifndef _SysLedTask_H
#define _SysLedTask_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
#include  "types.h"
	
#define FREQUENCY      0
#define UP_LIMIT       1
#define DOWN_LIMIT     2
#define ALARM          3
#define LOGO           4
#define AMPLIT         5//振幅
#define TRIGGE_TIME    6
#define SCAN_INTV      7//强制扫描参数
#define SOFT_START_TIM 8
#define MAX_P          9
#define END_SET        10
	



extern uint8_t FlushCursor;
extern 	uint8_t LedBuff[4];
extern void FlushCursorIncreas();
	//开机启动设置专用
extern void S_FlushCursorIncreas();
extern void SysLedRun(void);
extern void DisplayTask_Creat(void);
extern void DigTubeDisplay();
extern void SetDisplayType(uint8_t type);
	
	#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
