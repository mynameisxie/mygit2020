#ifndef _BSP_OSTASK_H
#define _BSP_OSTASK_H

#include "types.h"

#define OSTASKCOUNT  9

typedef enum
{
	 Normal_Type   = 0x01, 
	 Signal_Type  = 0x02      /*任务运行触发周期必须大于等于设置工作周期,如AI的采样任务必须大于10ms*/
}OsTaskType;

typedef struct 
{
	void (*TaskFun)(void);
	unsigned char TaskPrio;
	unsigned char TaskType;
	unsigned int TaskCycle;
	unsigned int RealTime;
}OsTask,*pOsTask;


typedef struct
{
		void      (*Init)(void);
		void      (*CreatTask)(void (*TaskFun)(void),unsigned char TaskPrio,unsigned int TaskCycle,OsTaskType TaskType); 
		void      (*ChangeTaskPrio)(unsigned char prio);
		void      (*TaskTick)(uint8_t byTick);
	  void      (*TaskRun)(void);
}BSP_TaskMgEvent_STR;

extern BSP_TaskMgEvent_STR gTaskCfgMgEvent;
#endif

