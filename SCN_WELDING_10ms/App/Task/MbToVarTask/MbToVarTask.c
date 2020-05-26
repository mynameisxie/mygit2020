#include  "MbToVarTask.h"
#include  "Bsp_OsTask.h"
#include  "MbSlave.h"
#include  "MotionTask.h"
#include  "MbSlave.h"
#include  "Bsp_DI.h"
#include  "Bsp_DO.h"

uint16_t MbRegCmd[4] = {0};
uint16_t Dio16 = 0;
uint8_t  MbDioCmd[4] = {0};
extern Bsp_DiMgInfo_STR gDiMgInfo;
extern Bsp_DoMgInfo_STR gDoMgInfo;
void getDoState(uint16_t *pbuf);
void getDiState(uint16_t *pbuf);
void MbToVarTask(void) 
{
	uint8_t equip = Controller.Sys->type;
	switch(equip)
	{
		case WIND_PUSHER:
			gMbSlaveMgEvent.read(_eHOLDING_REGISTER,0,(uint8_t *)&MbRegCmd,2);
			if(MbRegCmd[0]>0 && MbRegCmd[0]<4)
			{
				setMdsEvent(DRIVER0, MbRegCmd[0]);
			}
			if(MbRegCmd[1]>0 && MbRegCmd[1]<4)
			{
				
				setMdsEvent(DRIVER1, MbRegCmd[1]);
			}
			MbRegCmd[1] = 0;
			MbRegCmd[0] = 0;
			//将指令清零
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,0,(uint8_t *)MbRegCmd,2);
			//上传控制器状态
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,6,(uint8_t *)CtrlErr.pusher,2);
			//上传控制器DIDO状态
			getDoState(MbRegCmd);
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,10,(uint8_t *)MbRegCmd,4);
			getDiState(MbRegCmd);
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,18,(uint8_t *)MbRegCmd,4);
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,4,(uint8_t *)&Dio16,1);
			break;
		case DOU_DIRBLOWER:
			gMbSlaveMgEvent.read(_eHOLDING_REGISTER,40,(uint8_t *)&MbRegCmd,2);
			if(MbRegCmd[0]>0 && MbRegCmd[0]<4)
			{
				setMdsEvent(DRIVER0, MbRegCmd[0]);
			}
			if(MbRegCmd[1]>0 && MbRegCmd[1]<4)
			{
				
				setMdsEvent(DRIVER1, MbRegCmd[1]);
			}
			MbRegCmd[1] = 0;
			MbRegCmd[0] = 0;
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,40,(uint8_t *)&MbRegCmd,2);
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,46,(uint8_t *)CtrlErr.dblower,2);
			//上传控制器DIDO状态
			getDoState(MbRegCmd);
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,50,(uint8_t *)&MbRegCmd,4);
			getDiState(MbRegCmd);
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,58,(uint8_t *)&MbRegCmd,4);
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,44,(uint8_t *)&Dio16,1);
			break;
		case SGL_DIRBLOWER:
			gMbSlaveMgEvent.read(_eHOLDING_REGISTER,80,(uint8_t *)&MbRegCmd,4);
			if(MbRegCmd[0]>0 && MbRegCmd[0]<4)
			{
				setMdsEvent(DRIVER0, MbRegCmd[0]);
			}
			if(MbRegCmd[1]>0 && MbRegCmd[1]<4)
			{
				
				setMdsEvent(DRIVER1, MbRegCmd[1]);
			}
			if(MbRegCmd[2]>0 && MbRegCmd[2]<4)
			{
				setMdsEvent(DRIVER2, MbRegCmd[2]);
			}
			if(MbRegCmd[3]>0 && MbRegCmd[3]<4)
			{
				
				setMdsEvent(DRIVER3, MbRegCmd[3]);
			}
			MbRegCmd[0] = 0;
			MbRegCmd[1] = 0;
			MbRegCmd[2] = 0;
			MbRegCmd[3] = 0;
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,80,(uint8_t *)&MbRegCmd,4);	
			
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,90,(uint8_t *)CtrlErr.sblower,4);
			//上传控制器DIDO状态
			getDoState(MbRegCmd);
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,98,(uint8_t *)&MbRegCmd,4);
			getDiState(MbRegCmd);
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,106,(uint8_t *)&MbRegCmd,4);
			gMbSlaveMgEvent.write(_eHOLDING_REGISTER,88,(uint8_t *)&Dio16,1);
			break;
		case NORMAL_IO:
			gMbSlaveMgEvent.write(_eDISCRETE_INPUTS,0,gDiMgInfo.byDiCode,8);//DI
		  gMbSlaveMgEvent.read(_eCOILS,8,MbDioCmd,4);//DO
		  if(MbDioCmd[0] == 0xff)
				setMdsEvent(DRIVER0, 1);
			else
				setMdsEvent(DRIVER0, 3);
			if(MbDioCmd[1] == 0xff)
				setMdsEvent(DRIVER1, 1);
			else
				setMdsEvent(DRIVER1, 3);
			if(MbDioCmd[2] == 0xff)
				setMdsEvent(DRIVER2, 1);
			else
				setMdsEvent(DRIVER2, 3);
			if(MbDioCmd[3] == 0xff)
				setMdsEvent(DRIVER3, 1);
			else
				setMdsEvent(DRIVER3, 3);
			break;
		default:break;
			
	}
	MbRegCmd[0] = AutoHandFlag;//手自动方式
	MbRegCmd[1] = equip;
	if(equip == 4 && Controller.Sys->rback == 0)
	{
		MbRegCmd[1] = 4;
	}
	else if(equip == 4 && Controller.Sys->rback == 1)
	{
		MbRegCmd[1] = 5;
	}
	gMbSlaveMgEvent.write(_eHOLDING_REGISTER,140,(uint8_t *)MbRegCmd,2);
	
}

void  MbToVarTask_Creat(void)
{
	gTaskCfgMgEvent.CreatTask(MbToVarTask,2,500,Normal_Type);

}
void getDoState(uint16_t *pbuf)
{
	if(gDoMgInfo.byDoCode[6] == 0xff){
		pbuf[0] = 1;
		Dio16 |= 0x10;
	}
	else{
		pbuf[0] = 0;
		Dio16 &= ~0x10;
	}
	if(gDoMgInfo.byDoCode[5] == 0xff){
		pbuf[1] = 1;
		Dio16 |= 0x20;
	}
	else{
		pbuf[1] = 0;
		Dio16 &= ~0x20;
	}
	if(gDoMgInfo.byDoCode[4] == 0xff){
		pbuf[2] = 1;
		Dio16 |= 0x40;
	}
	
	
	else{
		pbuf[2] = 0;
		Dio16 &= ~0x40;
	}
	if(gDoMgInfo.byDoCode[3] == 0xff){
		pbuf[3] = 1;
		Dio16 |= 0x80;
	}
	else{
		pbuf[3] = 0;
		Dio16 &= ~0x80;
	}
	
}
void getDiState(uint16_t *pbuf)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		if(gDiMgInfo.byDiCode[i] == 0xff){
			pbuf[i] = 1;
			Dio16 |= (1<<i);
		}
		else{
			pbuf[i] = 0;
			Dio16 &= ~(1<<i);
		}
	}
}
int8_t setMdsEvent(uint8_t chg, uint8_t event)
{
//	if(AutoHandFlag) return 0;
	if(chg>4 || event>4) return -1;
	ModbusE[chg].cmd = event;
	ModbusE[chg].flag = 1;
	return 1;
}

int8_t clrMdsEvent(uint8_t chg)
{
	if(chg>4) return -1;
	ModbusE[chg].cmd  = 0;
	ModbusE[chg].flag = 0;
	return 1;
} 
