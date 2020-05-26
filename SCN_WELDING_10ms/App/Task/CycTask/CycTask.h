#ifndef _CycTask_H
#define _CycTask_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include  "types.h"

	
typedef enum{
	S_SELF_TEST,//自检
	S_RUNNING,//运行
	S_ALRM,//报警
	S_TRIG_DELAY,//等待触发时间
	S_SEND_NOISE,//发射超声波
	S_ADJ_BLESS,//调整振幅
	S_ADJ_TIME,//调整时间
	S_SET_PARA,//设置参数
}TaskState;

extern uint8_t PWRFlag;//系统启动参数标志
extern uint8_t AlarmCode;
extern uint8_t Trackstep;
extern void  CycTask_Creat(void);
extern void SwitchIOInit();
extern uint32_t FrequencySweep();
extern uint32_t FrequencyTracking2(void);
extern void SetCycTaskState(uint8_t state);
extern uint8_t search_flag;
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
