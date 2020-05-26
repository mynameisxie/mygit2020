#include  "MotionTask.h"
#include  "DouDirBlower.h"
#include  "Bsp_OsTask.h"
#include  "Bsp_Di.h"
#include  "Bsp_Do.h"
#include  "Bsp_Key.h"
#include "stm32f10x.h"
//双向风机控制器
uint8_t PstBlDo[2]={0x01,0x04}; 
uint8_t NgtBlDo[2]={0x02,0x08}; 
uint8_t PStBlDi[2]={0x01,0x04}; 
uint8_t NgtBlDi[2]={0x02,0x08}; 
uint8_t defbuf[2]={0x03,0x0c};
static void DealDouBlInAuto(CtrCfg_STR *pContr, uint8_t bnum);

static int8_t BlPositiveRun(uint8_t bnum);
static int8_t BlNegativeRun(uint8_t bnum);
static int8_t BlStop(uint8_t bnum);

uint8_t getDouBlowerState(CtrCfg_STR *pContr,uint8_t bnum)
{
	return pContr->state[bnum];
}
void DealDoudirBlower(CtrCfg_STR *pContr)
{
	DealDouBlInAuto(pContr,0);
	DealDouBlInAuto(pContr,1);
}
void DealDouBlInAuto(CtrCfg_STR *pContr, uint8_t bnum)
{
	static uint8_t pre_di[2] = {0};
	uint8_t Di=0;
  if(bnum > 1) return;
	
	gDiMgEvent.read(&Di);
	Di = Di&defbuf[bnum];
  switch(pContr->state[bnum])
	{
		//当前状态反转中
    case DBL_NGTV_RUNING:
			if(pContr->msg->evt[bnum] == 1)
			{
			  BlStop(bnum);
			  pContr->state[bnum] = DBL_PAUSE5SEC;
        Controller.Pcnt[bnum]=0;				
				pContr->pre_st[bnum] = DBL_NGTV_RUNING;
			}
			if(pContr->msg->evt[bnum] == 3)
			{
			  BlStop(bnum);
			  pContr->state[bnum] = DBL_STOP;
				pContr->Mcnt[bnum] = 0;

			}
			if(Di != pre_di[bnum])
			{
				pContr->Mcnt[bnum] = 0;
			}
			if(pContr->Mcnt[bnum]++ == CHECK_BACK)
			{
//				Di &= NgtBlDi[bnum];
				if((Di&NgtBlDi[bnum]) == 0)
				{
					CtrlErr.dblower[bnum] = 5;//反转异常		
				}
				else
				{
					CtrlErr.dblower[bnum] = 2;
				}
				pContr->Mcnt[bnum] = 0;

			}
	    break;
			//当前状态正转中
	  case DBL_PSTV_RUNING:
			if(pContr->msg->evt[bnum] == 2)
			{
			  BlStop(bnum);
			  Controller.state[bnum] = DBL_PAUSE5SEC;		
				Controller.Pcnt[bnum]=0;	
				pContr->pre_st[bnum] = DBL_PSTV_RUNING;
				pContr->Mcnt[bnum] = 0;

			}
			if(pContr->msg->evt[bnum] == 3)
			{
			  BlStop(bnum);
			  pContr->state[bnum] = DBL_STOP;
				pContr->Mcnt[bnum] = 0;

			}
			if(Di != pre_di[bnum])
			{
				pContr->Mcnt[bnum] = 0;

			}
			if(pContr->Mcnt[bnum]++ == CHECK_BACK)
			{
//				Di &= PStBlDi[bnum];
				if((Di&PStBlDi[bnum]) == 0)
				{
					CtrlErr.dblower[bnum] = 4;//正转异常		
				}
				else
				{
					CtrlErr.dblower[bnum] = 1;
				}
				pContr->Mcnt[bnum] = 0;
			}
			
	    break;
			//当前状态停止
	  case DBL_STOP:
			if(pContr->msg->evt[bnum] == 1)
			{
			  BlPositiveRun(bnum);
				pContr->state[bnum] = DBL_PSTV_RUNING;
//				CtrlErr.dblower[bnum] = 1;//正转中
				pContr->Mcnt[bnum] = 0;

			}
			else if(pContr->msg->evt[bnum] == 2)
			{
			  BlNegativeRun(bnum);
				pContr->state[bnum] = DBL_NGTV_RUNING;	
//				CtrlErr.dblower[bnum] = 2;//反转中
				pContr->Mcnt[bnum] = 0;				

			}
			else
			{
//				CtrlErr.dblower[bnum] = 3;//停止
			}
			if(Di != pre_di[bnum])
			{
				pContr->Mcnt[bnum] = 0;

			}
			if(pContr->Mcnt[bnum]++ >= CHECK_BACK)
			{
				if(Di & PStBlDi[bnum])
				{
					CtrlErr.dblower[bnum] = 4;//正转异常		
				}
				
				if(Di & NgtBlDi[bnum])
				{
					CtrlErr.dblower[bnum] = 5;//反转异常		
				}
				
				if((Di & (PStBlDi[bnum] | NgtBlDi[bnum])) == 0)
				{
					CtrlErr.dblower[bnum] = 3;//停止
				}
				pContr->Mcnt[bnum] = 0;
			}
			
			
	    break;
			//当前状态暂停5S
	  case DBL_PAUSE5SEC:
			CtrlErr.dblower[bnum] = 6;//切换
			Controller.Pcnt[bnum]++;
			if(Controller.Pcnt[bnum] > BLOWER_TIME_PAUSE)
			{
				pContr->Pcnt[bnum] = 0;
				pContr->Mcnt[bnum] = 0;
				
			  if(pContr->pre_st[bnum] ==  DBL_NGTV_RUNING)
				{
			    BlPositiveRun(bnum);
				  pContr->state[bnum] = DBL_PSTV_RUNING;						
					CtrlErr.dblower[bnum] = 1;//正转
					pContr->Mcnt[bnum] = 0;
				}
			  if(pContr->pre_st[bnum] == DBL_PSTV_RUNING)
				{
			    BlNegativeRun(bnum);
				  pContr->state[bnum] = DBL_NGTV_RUNING;				
					CtrlErr.dblower[bnum] = 2;//正转
					pContr->Mcnt[bnum] = 0;
				}

			}
	    break;
	  default:break;
  }
	pre_di[bnum] = Di;
}
int8_t BlPositiveRun(uint8_t bnum)
{
	uint8_t Do = 0;
	if(bnum > 1) return -1;
	gDoMgEvent.read(&Do);
	Do |= PstBlDo[bnum];
	gDoMgEvent.wirte(&Do,4); 
	return 1;
}
int8_t BlNegativeRun(uint8_t bnum)
{
	uint8_t Do = 0;
	if(bnum > 2) return -1;
	gDoMgEvent.read(&Do);
	Do |= NgtBlDo[bnum];
	gDoMgEvent.wirte(&Do,4); 
	return 1;
}
int8_t BlStop(uint8_t bnum)
{
	uint8_t Do = 0;
	if(bnum > 1) return -1;
	gDoMgEvent.read(&Do);
	Do &= ~NgtBlDo[bnum];
	Do &= ~PstBlDo[bnum];
	gDoMgEvent.wirte(&Do,4); 
	return 1;
}
void checkDblStatus(CtrCfg_STR *pContr)
{
	uint8_t Di = 0,Do = 0,i = 0;
	gDoMgEvent.read(&Do);
	gDiMgEvent.read(&Di);
	for(i=0;i<2;i++)
	{
		//正转
		if((Di&PStBlDi[i]) && (Di&NgtBlDi[i])==0)
		{
			pContr->state[i] = DBL_PSTV_RUNING;
			CtrlErr.dblower[i] = 1;
		}
		//反转
		else if((Di&NgtBlDi[i]) && (Di&PStBlDi[i])==0)
		{
			pContr->state[i] = DBL_NGTV_RUNING;
			CtrlErr.dblower[i] = 2;
		}
		//停止
		else if((Di&NgtBlDi[i])==0 && (Di&PStBlDi[i])==0)
		{
			pContr->state[i] = DBL_STOP;
			CtrlErr.dblower[i] = 3;
		}
		//出错
		else
		{
			
		}
	}
}
