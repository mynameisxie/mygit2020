#include  "Windpusher.h"
#include  "MotionTask.h"
#include  "Bsp_OsTask.h"
#include  "Bsp_Di.h"
#include  "Bsp_Do.h"
#include  "Bsp_Key.h"
#include  "MbTask.h"
#include  "DiKeyTask.h"
#include  "OlcdTask.h"
uint8_t OpenWindDo[2]={0x01,0x04}; 
uint8_t ClosWindDo[2]={0x02,0x08}; 
uint8_t OpenWindDi[2]={0x01,0x04}; 
uint8_t ClosWindDi[2]={0x02,0x08}; 
//uint8_t OpenDo[4]={0x01,0x02,0x04,0x08};
//uint8_t OpenDo[4]={0x01,0x02,0x04,0x08};
static void DealPusherInAuto(CtrCfg_STR *pContr, uint8_t wnum);
static int8_t OpenWind(uint8_t wnum);
static int8_t StopWind(uint8_t wnum);
static int8_t CloseWind(uint8_t wnum);
uint8_t getPusherState(CtrCfg_STR *pContr,uint8_t wnum)
{
	return pContr->state[wnum];
}
	
void DealWindPusher(CtrCfg_STR *pContr)
{
	DealPusherInAuto(pContr,0);
	
	DealPusherInAuto(pContr,1);
}

void DealPusherInAuto(CtrCfg_STR *pContr, uint8_t wnum)
{
	uint8_t Di = 0;
  if(wnum > 1) return;
	switch(pContr->state[wnum])
	{
		case WIND_CLOSED : //当前状态关到位
			//开
			pContr->Mcnt[wnum] = 0;
      if(pContr->msg->evt[wnum] == 1)
			{
				OpenWind(wnum);
			  pContr->state[wnum] = WIND_OPENING;
			}
			
		  break;
		case WIND_OPENED://当前状态开到位
			//关
		  pContr->Mcnt[wnum] = 0;
      if(pContr->msg->evt[wnum] == 2)
			{
			  CloseWind(wnum);
			  pContr->state[wnum] = WIND_CLOSING;
				pContr->Mcnt[wnum] = SysCfg.tmout*1000/MOTION_TASK_CYCLE;
			}
      break;
		case WIND_CLOSING://当前状态关闭中
			if(SysCfg.rback == 1)
			{
				gDiMgEvent.read(&Di);
			}
			Di &= 0x0F;
		  Di &= ClosWindDi[wnum]; 
		  pContr->Mcnt[wnum]--;
		  //30秒超时/开到位检测
		  if(Di || pContr->Mcnt[wnum] ==0)
			{
				StopWind(wnum);
				if(Di == 0)
				{
					if(pContr->Sys->rback == 1)
					{
						CtrlErr.pusher[wnum] = 5;
					}
					else
					{
						CtrlErr.pusher[wnum] = 2;
					}
				}
				else
				{
					CtrlErr.pusher[wnum] = 2;
				}
			  pContr->state[wnum] = WIND_CLOSED;
				pContr->Mcnt[wnum] = 0;
			}
			else
			{
				CtrlErr.pusher[wnum] = 7;
			}
			//开
			if(pContr->msg->evt[wnum] == 1)
			{
				StopWind(wnum);
			  pContr->state[wnum] = WIND_PAUSE3SEC;
			  pContr->pre_st[wnum] = WIND_CLOSING;
				pContr->Pcnt[wnum] = 0;
				pContr->Mcnt[wnum] = 0;
			}
			//停
			if(pContr->msg->evt[wnum] == 3)
			{
			  StopWind(wnum);
				pContr->state[wnum] = WIND_PAUSE;
				CtrlErr.pusher[wnum] = 3;
			}
      break;
		case WIND_OPENING://当前状态开启中
			if(SysCfg.rback == 1)
			{
				gDiMgEvent.read(&Di);
			}
		  Di &= 0x0F;
			Di &= OpenWindDi[wnum]; 
		  pContr->Mcnt[wnum]++;
		  
			//30秒超时/开到位检测
			if(Di || pContr->Mcnt[wnum] >= SysCfg.tmout*1000/MOTION_TASK_CYCLE)
			{
				StopWind(wnum);
			  pContr->state[wnum] = WIND_OPENED;
				if(Di == 0)
				{
					if(pContr->Sys->rback == 1)
					{
						CtrlErr.pusher[wnum] = 4;
					}
					else
					{
						CtrlErr.pusher[wnum] = 1;
					}
				}
				else
				{
					CtrlErr.pusher[wnum] = 1;
				}
				pContr->Mcnt[wnum] = 0;
			}
			else
			{
				CtrlErr.pusher[wnum] = 6;
			}
			//关
			if(pContr->msg->evt[wnum] == 2)
			{
				StopWind(wnum);
			  pContr->state[wnum] = WIND_PAUSE3SEC;
				pContr->pre_st[wnum] = WIND_OPENING;
				pContr->Pcnt[wnum] = 0;
				pContr->Mcnt[wnum] = SysCfg.tmout*1000/MOTION_TASK_CYCLE;
				
			}
			//停
			if(pContr->msg->evt[wnum] == 3)
			{
			  StopWind(wnum);
				pContr->state[wnum] = WIND_PAUSE;
			}
			break;
		case WIND_PAUSE://当前状态暂停
			//开
		  pContr->Mcnt[wnum] = 0;
			if(pContr->msg->evt[wnum] == 1)
			{
				OpenWind(wnum);
			  pContr->state[wnum]= WIND_OPENING;		
				pContr->Mcnt[wnum] = 0;
			}
			//关
			if(pContr->msg->evt[wnum] == 2)
			{
			  CloseWind(wnum);
			  pContr->state[wnum] = WIND_CLOSING;
				pContr->Mcnt[wnum] = SysCfg.tmout*1000/MOTION_TASK_CYCLE;
			}
			CtrlErr.pusher[wnum] = 3;
			break;
		case WIND_PAUSE3SEC://当前状态延时3秒
			pContr->Pcnt[wnum]++;
			if(pContr->Pcnt[wnum] > MOTION_TIME_PAUSE)
			{
				pContr->Pcnt[wnum] = 0;
				if(pContr->pre_st[wnum] == WIND_OPENING)//打开
				{
					CloseWind(wnum);
			    pContr->state[wnum] = WIND_CLOSING;
				}
				else if(pContr->pre_st[wnum] == WIND_CLOSING)//关闭
				{
					OpenWind(wnum);
			    pContr->state[wnum] = WIND_OPENING;

				}
				else
				{
					;
				}
			}
			break;
    default:break;
	}
	

}

int8_t StopWind(uint8_t wnum)
{
	
	uint8_t Do = 0;
	if(wnum > 1) return -1;
	gDoMgEvent.read(&Do);
	Do &= ~OpenWindDo[wnum];
	Do &= ~ClosWindDo[wnum];
	gDoMgEvent.wirte(&Do,4);
	return 1;
}

int8_t CloseWind(uint8_t wnum)
{
	uint8_t Do = 0;
	if(wnum > 1) return -1;
	gDoMgEvent.read(&Do);
	Do |= ClosWindDo[wnum]; 
  gDoMgEvent.wirte(&Do,4);
	return 1;
}

int8_t OpenWind(uint8_t wnum)
{
	uint8_t Do = 0;
	if(wnum > 1) return -1;
	gDoMgEvent.read(&Do);
	Do |= OpenWindDo[wnum];
	gDoMgEvent.wirte(&Do,4);
	return 1;
}
void checkPusherStatus(CtrCfg_STR *pContr)
{
	uint8_t Di = 0, i = 0;
	if(pContr->Sys->rback == 1)
	{
		gDiMgEvent.Tick(1);
	}
	gDiMgEvent.read(&Di);
	
	for(i=0;i<2;i++)
	{
		//开到位
		if((Di&OpenWindDi[i]) && ((Di&ClosWindDi[i])==0))
		{
			pContr->state[i] = WIND_OPENED;
			CtrlErr.pusher[i] = 1;
		}
		//关到位
		else if((Di&ClosWindDi[i]) && ((Di&OpenWindDi[i])==0))
		{
			pContr->state[i] = WIND_CLOSED;
			CtrlErr.pusher[i] = 2;
		}
		//停止
		else if((Di&ClosWindDi[i])==0 && ((Di&OpenWindDi[i])==0))
		{
			pContr->state[i] = WIND_PAUSE;
			CtrlErr.pusher[i] = 3;
		}
	}
}
