#define Motion_Task 1
#include  "Bsp_Di.h"
#include  "Bsp_Do.h"
#include  "Bsp_Key.h"
#include  "MotionTask.h"
#include  "DouDirBlower.h"
#include  "SglDirBlower.h"
#include  "Windpusher.h"
#include  "Bsp_OsTask.h"
#include  "DiKeyTask.h"
#include  "NormalIO.h"
#include  "MbTask.h"
#include  "OlcdTask.h"
#include "stm32f10x.h"
void MotionTask()
{
  uint8_t equip = Controller.Sys->type;
	switch(equip)
	{
		//通用IO控制器
	  case NORMAL_IO:
			getValidMsg(&SysMsg);
			DealNormalIO(&Controller);
		  clrSysMsg(&SysMsg);			
			break;
		//门窗推杆控制器
		case WIND_PUSHER:
			getValidMsg(&SysMsg);
			DealWindPusher(&Controller);
		  clrSysMsg(&SysMsg);
		  
			break;
		//双向风机控制器
		case DOU_DIRBLOWER:
		  getValidMsg(&SysMsg);
			DealDoudirBlower(&Controller);
		  clrSysMsg(&SysMsg);
			break;
		//单向风机控制器
		case SGL_DIRBLOWER:
		  getValidMsg(&SysMsg);
			DealSgldirBlower(&Controller);
		  clrSysMsg(&SysMsg);
			break;
		default:
			break;
	}
}

void  MotionTask_Creat(void)
{
	gTaskCfgMgEvent.CreatTask(MotionTask,1,MOTION_TASK_CYCLE,Normal_Type);

}

void getValidMsg(SYSMSG_STR *pmsg)
{
	uint8_t i =0;

	for(i=0;i<4;i++)
	{
		pmsg->evt[i] = 0;
		if(pmsg->mdsevt[i].flag)
			pmsg->evt[i] = pmsg->mdsevt[i].cmd;		
		if(pmsg->dikevt[i].flag)
			pmsg->evt[i] = pmsg->dikevt[i].cmd;
		if(pmsg->lcdevt[i].flag)
			pmsg->evt[i] = pmsg->lcdevt[i].cmd;
	}
}
void clrSysMsg(SYSMSG_STR *pmsg)
{
	uint8_t i = 0;
	for(i=0;i<4;i++)
	{
		pmsg->evt[i] = 0;
		pmsg->dikevt[i].flag=0;
		pmsg->dikevt[i].cmd =0;
		pmsg->lcdevt[i].flag=0;
		pmsg->lcdevt[i].cmd=0;
		pmsg->mdsevt[i].flag=0;
		pmsg->mdsevt[i].cmd=0;
	}
}
uint8_t getContrState(uint8_t nchan)
{
	return Controller.state[nchan];
}
void checkSysStatus(CtrCfg_STR *pCtrl)
{
	switch(pCtrl->Sys->type)
	{
	  case NORMAL_IO:
			checkIOStatus(pCtrl);
			break;
	  case WIND_PUSHER:
      checkPusherStatus(pCtrl);
			break;
		case DOU_DIRBLOWER:
			checkDblStatus(pCtrl);
			break;
		case SGL_DIRBLOWER:
			checkSblStatus(pCtrl);
			break;			
		default:
			break;
		
	}
}
