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
* ��������:     GetTaskTopPrio
* ����˵��:     ��ȡ������ȼ�����
* ��ڲ���:     ��
* ���ز���:     
* ��ע��Ϣ:   
********************************************************************/

unsigned char GetTaskTopPrio(void)
{
	unsigned char prio,y,x;

	y = OSUnMapTbl[OSRdyGrp];     //��ȡ��Ӧ���λΪ1���к�
	x = OSUnMapTbl[OSRdyTbl[y]];    // ��ȡ��Ӧ���λΪ1���к�
	prio = (y << 3) + x;
	return 	prio;
}

/********************************************************************
* ��������:     SetTaskPrio
* ����˵��:     �����л�������̬
* ��ڲ���:     ��
* ���ز���:     
* ��ע��Ϣ:   
********************************************************************/

void SetTaskPrio(unsigned char prio)
{

	OSRdyGrp  |= OSMapTbl[prio>>3];     //��Ӧ�к�����1  

	OSRdyTbl[prio>>3] |= OSMapTbl[prio&0x07]; //��Ӧ�кŵ��к�����1 

}

/********************************************************************
* ��������:     ClearTaskPrio
* ����˵��:     ����̬�л�������
* ��ڲ���:     ��
* ���ز���:     
* ��ע��Ϣ:   
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

	if ((OSRdyTbl[prio >> 3] &= ~OSMapTbl[prio & 0x07]) == 0)  //��Ӧ������
	{
		OSRdyGrp &= ~OSMapTbl[prio >> 3];                  //�����ȫΪ�㣬˵������û�ж�Ӧ�������ˣ���ʱ�Ը�������
	}
}

/********************************************************************
* ��������:     RegisterSystemTask
* ����˵��:     ע��������
* ��ڲ���:     ��
* ���ز���:     
* ��ע��Ϣ:   
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
* ��������:     UnRegisterSystemTask
* ����˵��:     ɾ��������
* ��ڲ���:     ��
* ���ز���:     
* ��ע��Ϣ:   
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
* ��������:     InitOsTaskList
* ����˵��:     ��ʼ�������б�
* ��ڲ���:     ��
* ���ز���:     
* ��ע��Ϣ:   
********************************************************************/

void InitOsTaskList(void)
{
//	pOsTaskList = OsTaskList;
}

/********************************************************************
* ��������:     OsTaskListProcess
* ����˵��:     �������Ⱥ���
* ��ڲ���:     ��
* ���ز���:     
* ��ע��Ϣ:   
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
* ��������:     OsTaskRun
* ����˵��:     ������ú���
* ��ڲ���:     ��
* ���ز���:     
* ��ע��Ϣ:   
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
