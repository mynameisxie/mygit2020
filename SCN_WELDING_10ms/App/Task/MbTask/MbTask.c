#include  "MbTask.h"
#include  "Bsp_OsTask.h"
#include  "MbSlave.h"
#include  "MotionTask.h"
#include "Bsp_Uart.h"
#include "stm32f10x_usart.h"
#define _MbTaskCyc   5
uint16_t data[4] = {0};
void MbTask(void)
{
		gMbSlaveMgEvent.Tick();
}

void  MbTask_Creat(void)
{
	gMbSlaveMgEvent.Init();
		//����ͨѶ��ַ
	gMbSlaveMgEvent.setAddr((uint8_t)SysCfg.addr);
	//����ͨѶ������
//	gSerialMgEvent.setbaud(2,SysCfg.baud);	
	gTaskCfgMgEvent.CreatTask(MbTask,4,_MbTaskCyc,Normal_Type);
}

