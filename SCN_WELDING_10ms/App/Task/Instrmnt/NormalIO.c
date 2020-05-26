#include  "NormalIO.h"
#include  "SglDirBlower.h"
#include  "MotionTask.h"
#include  "Bsp_OsTask.h"
#include  "Bsp_Di.h"
#include  "Bsp_Do.h"
#include  "Bsp_Key.h"
#include  "MbSlave.h"
extern Bsp_DiMgInfo_STR gDiMgInfo;
extern Bsp_DoMgInfo_STR gDoMgInfo;
//ÆÕÍ¨IO
uint8_t OpenDo[4]={0x01,0x02,0x04,0x08}; 
int8_t oBlowerStrategyAnaly(void);
static void DealIOInAuto(CtrCfg_STR *pContr, uint8_t bnum);
static int8_t normalOpen(uint8_t bnum);
static int8_t normalClos(uint8_t bnum);
uint8_t getNormalState(CtrCfg_STR *pContr,uint8_t bnum)
{
	return pContr->state[bnum];
}
void DealNormalIO(CtrCfg_STR *pContr)
{
	DealIOInAuto(pContr,0);
	DealIOInAuto(pContr,1);
	DealIOInAuto(pContr,2);
	DealIOInAuto(pContr,3);
}
void DealIOInAuto(CtrCfg_STR *pContr, uint8_t bnum)
{
  if(bnum > 3) return;
	switch(pContr->state[bnum])
	{
	  case IO_OPEN:
		  if(pContr->msg->evt[bnum] == 3)
			{
				normalClos(bnum);
			  pContr->state[bnum] = IO_CLOS;		
			}
		break;
	  case IO_CLOS:
		  if(pContr->msg->evt[bnum] == 1)
			{
				normalOpen(bnum);
			  pContr->state[bnum] = IO_OPEN;		
			}
		break;
	  default:break;
	}
}

int8_t normalOpen(uint8_t bnum)
{
	uint8_t Do = 0,i=0;
	uint8_t temp[4]={0};
	if(bnum > 3) return -1;
	
	gDoMgEvent.read(&Do);
	Do |= OpenDo[bnum];
	gDoMgEvent.wirte(&Do,4);
	for(i=0;i<4;i++)
	{
		if(Do&(0x01<<i))
			temp[i] = 0xff;
		else
			temp[i] = 0;
	}
	gMbSlaveMgEvent.write(_eCOILS,8,temp,4);//DO
	return 1;
}


int8_t normalClos(uint8_t bnum)
{
	uint8_t Do = 0,i=0;
	uint8_t temp[4]={0};
	if(bnum > 3) return -1;
	gDoMgEvent.read(&Do);
	Do &= ~OpenDo[bnum];
	gDoMgEvent.wirte(&Do,4); 
		for(i=0;i<4;i++)
	{
		if(Do&(0x01<<i))
			temp[i] = 0xff;
		else
			temp[i] = 0;
	}
	gMbSlaveMgEvent.write(_eCOILS,8,temp,4);//DO
	return 1;
}

void checkIOStatus(CtrCfg_STR *pContr)
{
	uint8_t Do = 0, i = 0;
	gDoMgEvent.read(&Do);
	for(i = 0;i < 4; i++)
	{
		if(Do & (0x01<<i))
		{
			pContr->state[i] = IO_OPEN;
		}
		else
		{
			pContr->state[i] = IO_CLOS;		
		}
	}
}

