#include  "SglDirBlower.h"
#include  "MotionTask.h"
#include  "Bsp_OsTask.h"
#include  "Bsp_Di.h"
#include  "Bsp_Do.h"
#include  "Bsp_Key.h"


//单向风机控制器

uint8_t BlRunDo[4]={0x01,0x02,0x04,0x08}; 
uint8_t BlRunDi[4]={0x01,0x02,0x04,0x08}; 
int8_t oBlowerStrategyAnaly(void);
static void DealSglBlInAuto(CtrCfg_STR *pContr, uint8_t bnum);
static int8_t sBlRun(uint8_t bnum);
static int8_t sBlStop(uint8_t bnum);
uint8_t getSglBlowerState(CtrCfg_STR *pContr,uint8_t bnum)
{
	return pContr->state[bnum];
}
void DealSgldirBlower(CtrCfg_STR *pContr)
{
	DealSglBlInAuto(pContr,0);
	DealSglBlInAuto(pContr,1);
	DealSglBlInAuto(pContr,2);
	DealSglBlInAuto(pContr,3);
}
void DealSglBlInAuto(CtrCfg_STR *pContr, uint8_t bnum)
{
	uint8_t Di = 0;
  if(bnum > 3) return;
	switch(pContr->state[bnum])
	{
	  case SBL_RUNING:
		  if(pContr->msg->evt[bnum] == 3)
			{
				sBlStop(bnum);
			  pContr->state[bnum] = SBL_STOP;		
				CtrlErr.sblower[bnum] = 2;
				pContr->Mcnt[bnum] = 0;
			}
			if(pContr->Mcnt[bnum]++ == CHECK_BACK)
			{
				pContr->Mcnt[bnum] = 0;
				gDiMgEvent.read(&Di);
				Di &= BlRunDi[bnum];
				if(Di == 0)
				{
					CtrlErr.sblower[bnum] = 3;//异常		
				}
				else
				{
					CtrlErr.sblower[bnum] = 1;//运行
				}
			}
		break;
	  case SBL_STOP:
		  if(pContr->msg->evt[bnum] == 1)
			{
				sBlRun(bnum);
			  pContr->state[bnum] = SBL_RUNING;		
				CtrlErr.sblower[bnum] = 1;
				pContr->Mcnt[bnum] = 0;
			}
			if(pContr->Mcnt[bnum]++ == CHECK_BACK)
			{
				gDiMgEvent.read(&Di);
				Di &= BlRunDi[bnum];
				pContr->Mcnt[bnum] = 0;
				if(Di)
				{
					CtrlErr.sblower[bnum] = 3;//异常		
				}
				else
				{
					CtrlErr.sblower[bnum] = 2;//运行
				}
			}
		break;
	  default:break;
	}
}

int8_t sBlRun(uint8_t bnum)
{
	uint8_t Do = 0;
	if(bnum > 3) return -1;
	gDoMgEvent.read(&Do);
	Do |= BlRunDo[bnum];
	gDoMgEvent.wirte(&Do,4);
	return 1;
}


int8_t sBlStop(uint8_t bnum)
{
	uint8_t Do = 0;
	if(bnum > 3) return -1;
	gDoMgEvent.read(&Do);
	Do &= ~BlRunDo[bnum];
	gDoMgEvent.wirte(&Do,4); 
	return 1;
}

void checkSblStatus(CtrCfg_STR *pContr)
{
	uint8_t Di = 0,Do = 0, i = 0;;
	gDoMgEvent.read(&Do);
	gDiMgEvent.read(&Di);
	for(i = 0;i < 4; i++)
	{
		if(Di & (0x01<<i))
		{
			pContr->state[i] = SBL_RUNING;
			CtrlErr.sblower[i] = 1;
			pContr->Mcnt[i] = 0;				
		}
		else
		{
			pContr->state[i] = SBL_STOP;
			CtrlErr.sblower[i] = 2;
			pContr->Mcnt[i] = 0;				
		}
	}
}

