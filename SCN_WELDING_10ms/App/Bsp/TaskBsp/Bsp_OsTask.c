#include "Bsp_OsTask.h"
#include "string.h"

static OsTask  OsTaskList[OSTASKCOUNT];
static unsigned char OsUsedList[OSTASKCOUNT] = {0,0,0,0,0,0,0,0};
static unsigned char OSRdyGrp;
static unsigned char OSRdyTbl[8];
const unsigned char OSMapTbl[8] ={0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};

pOsTask pOsTaskList = OsTaskList;;

const unsigned char OSUnMapTbl[] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0x00 to 0x0F                             
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0x10 to 0x1F                             
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0x20 to 0x2F                             
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0x30 to 0x3F                             
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0x40 to 0x4F                           
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0x50 to 0x5F                             
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0x60 to 0x6F                             
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0x70 to 0x7F                             
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0x80 to 0x8F                             
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0x90 to 0x9F                             
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0xA0 to 0xAF                             
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0xB0 to 0xBF                             
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0xC0 to 0xCF                             
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0xD0 to 0xDF                             
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       //* 0xE0 to 0xEF                             
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0        //* 0xF0 to 0xFF                             
}; 

/********************************************************************
* 函数名称:     GetTaskTopPrio
* 函数说明:     获取最高优先级任务
* 入口参数:     无
* 返回参数:     
* 备注信息:   
********************************************************************/

unsigned char GetTaskTopPrio(void)
{
	unsigned char prio,y,x;

	y = OSUnMapTbl[OSRdyGrp];     //获取对应最低位为1的行号
	x = OSUnMapTbl[OSRdyTbl[y]];    // 获取对应最低位为1的列号
	prio = (y << 3) + x;
	return 	prio;
}

/********************************************************************
* 函数名称:     SetTaskPrio
* 函数说明:     挂起切换到就绪态
* 入口参数:     无
* 返回参数:     
* 备注信息:   
********************************************************************/

void SetTaskPrio(unsigned char prio)
{

	OSRdyGrp  |= OSMapTbl[prio>>3];     //对应行号设置1  

	OSRdyTbl[prio>>3] |= OSMapTbl[prio&0x07]; //对应行号的列号设置1 

}

/********************************************************************
* 函数名称:     ClearTaskPrio
* 函数说明:     运行态切换到挂起
* 入口参数:     无
* 返回参数:     
* 备注信息:   
********************************************************************/

void ClearTaskPrio(unsigned char prio)
{
	if(Normal_Type == (pOsTaskList+prio)->TaskType)
	{
		(pOsTaskList+prio)->RealTime = (pOsTaskList+prio)->RealTime%(pOsTaskList+prio)->TaskCycle; 
	}
	else if(Signal_Type == (pOsTaskList+prio)->TaskType)
	{
		(pOsTaskList+prio)->RealTime = 0;	
	}

	if ((OSRdyTbl[prio >> 3] &= ~OSMapTbl[prio & 0x07]) == 0)  //对应列清零
	{
		OSRdyGrp &= ~OSMapTbl[prio >> 3];                  //如果列全为零，说明该列没有对应的任务了，此时对该行清零
	}
}

/********************************************************************
* 函数名称:     RegisterSystemTask
* 函数说明:     注册任务函数
* 入口参数:     无
* 返回参数:     
* 备注信息:   
********************************************************************/

void RegisterSystemTask(void (*TaskFun)(void),unsigned char TaskPrio,unsigned int TaskCycle,OsTaskType TaskType)
{	
	(pOsTaskList+TaskPrio)->TaskFun = TaskFun;
	(pOsTaskList+TaskPrio)->TaskPrio = TaskPrio;
	(pOsTaskList+TaskPrio)->TaskType = TaskType;
	(pOsTaskList+TaskPrio)->TaskCycle = TaskCycle;
	(pOsTaskList+TaskPrio)->RealTime = 0;
	
	OsUsedList[TaskPrio] = TaskPrio;
}

/********************************************************************
* 函数名称:     UnRegisterSystemTask
* 函数说明:     删除任务函数
* 入口参数:     无
* 返回参数:     
* 备注信息:   
********************************************************************/

void UnRegisterSystemTask(void (*TaskFun)(void))
{
	unsigned char Count;

	for(Count = 0 ; Count < OSTASKCOUNT; Count++)
	{
		if((pOsTaskList+Count)->TaskFun == TaskFun)
		{
			ClearTaskPrio((pOsTaskList+Count)->TaskPrio);
			
			(pOsTaskList+Count)->TaskFun = NULL;

			memcpy(&OsUsedList[Count],&OsUsedList[Count+1],OSTASKCOUNT-Count-1);
		}
	}

}
/********************************************************************
* 函数名称:     InitOsTaskList
* 函数说明:     初始化任务列表
* 入口参数:     无
* 返回参数:     
* 备注信息:   
********************************************************************/

void InitOsTaskList(void)
{
//	pOsTaskList = OsTaskList;
}

/********************************************************************
* 函数名称:     OsTaskListProcess
* 函数说明:     任务表调度函数
* 入口参数:     无
* 返回参数:     
* 备注信息:   
********************************************************************/

void OsTaskListProcess(uint8_t byTick)
{
	unsigned char Count = 0;

	for(Count = 0; Count < OSTASKCOUNT; Count++)
	{
		if(OsUsedList[Count] != 0)
		{
			(pOsTaskList+Count)->RealTime ++;
			if((pOsTaskList+Count)->RealTime > (pOsTaskList+Count)->TaskCycle)
			{
				SetTaskPrio(Count);
			}
		}
	}
}

/********************************************************************
* 函数名称:     OsTaskRun
* 函数说明:     任务调用函数
* 入口参数:     无
* 返回参数:     
* 备注信息:   
********************************************************************/

void OsTaskRun(void)
{
	
	unsigned char prio;
	
		
	prio = GetTaskTopPrio();

	if(prio)
	{
		ClearTaskPrio(prio);
		((pOsTaskList+prio)->TaskFun)();
		
	}
}

BSP_TaskMgEvent_STR gTaskCfgMgEvent = 
{
	InitOsTaskList,
	RegisterSystemTask,
	SetTaskPrio,
	OsTaskListProcess,
	OsTaskRun
};
