#define DiKey_Task 1
#include  "MotionTask.h"
#include  "DouDirBlower.h"
#include  "SglDirBlower.h"
#include  "Windpusher.h"
#include  "Bsp_OsTask.h"
#include  "DiKeyTask.h"
#include  "Bsp_Di.h"
#include  "Bsp_Do.h"
#include  "Bsp_Key.h"
uint8_t   KeyOld;
uint8_t   KeyTime;        //
uint8_t   KeyCode;        //
uint8_t   KeyReadReady;   //
uint8_t   KeyUpdate;   //


void clrKeyTemp(Bsp_KeyMgEvent_STR *pKey);
int8_t setDikEvent(uint8_t chg, uint8_t event);
int8_t clrDikEvent(uint8_t chg);
uint8_t keyFilter2(uint8_t key);
uint8_t keyFilter(uint8_t key)
{
	
	KeyUpdate = key >>4;
	if(KeyUpdate==KeyOld)    	//
	{
		if(KeyUpdate!=0)				// 
		{
			KeyTime ++;
			if(KeyTime>=200)	//防止按键不放被重复执行
			{
				KeyTime = 200;
			}
			if(KeyTime==10 )   //
			{
				KeyCode = KeyOld;
				KeyOld  = KeyUpdate;
				KeyReadReady = 1;
			}
			
		}
	}
	else	//
	{		
		KeyOld  = KeyUpdate;
		KeyTime = 0;			
	}
	if(KeyReadReady == 1)
	{
		KeyReadReady = 0;
		return KeyCode;
	}
	else
	{
		KeyReadReady = 0;
		KeyCode = 0;
		return 0;
	}
}


uint8_t keyFilter2(uint8_t key)
{
	static uint8_t KeyUpdate[4]={0};
	static uint8_t KeyOld[4]={0};

	static uint8_t KeyTime[4]={0};
	static uint8_t KeyReadReady[4]={0};
	uint8_t value = 0;
	uint8_t i = 0;
	key = key>>4;
	KeyUpdate[0] = key &0x01;
	KeyUpdate[1] = key &0x02;
	KeyUpdate[2] = key &0x04;
	KeyUpdate[3] = key &0x08;
	for(i = 0; i < 4; i++)
	{
		if(KeyUpdate[i]==KeyOld[i])    	//
		{
			if(KeyUpdate[i]!=0)				// 
			{
				KeyTime[i] ++;
				if(KeyTime[i] >= 200)	//防止按键不放被重复执行
				{
					KeyTime[i] = 200;
				}
				if(KeyTime[i]==10 )   //
				{

					KeyOld[i]  = KeyUpdate[i];
					KeyReadReady[i] = 1;
				}
				
			}
		}
		else	//
		{		
			KeyOld[i]  = KeyUpdate[i];
			KeyTime[i] = 0;			
		}
	}
	
	
	for(i = 0; i < 4; i++)
	{
		if(KeyReadReady[i] == 1)
		{
			value |= (1<<i); 
			KeyReadReady[i] = 0;
		}
		else
		{
			KeyReadReady[i] = 0;
			value &= ~(1<<i); 

		}
	}
  return value;
}

void DiKeyTask(void)
{
	uint8_t key;
	uint8_t equip;
  uint8_t state = 0;
	gDiMgEvent.read(&key);
	key = keyFilter2(key);
	if(key==0)return;
	equip = Controller.Sys->type;
	switch(key)
	{
		
		case 0x01:
			
			switch(equip)
			{
				case WIND_PUSHER:
					state = getPusherState(&Controller,DRIVER0);
					if(state == WIND_CLOSED || state == WIND_CLOSING || state == WIND_PAUSE )
					{
						setDikEvent(DRIVER0,1);
					}
					else if(state == WIND_OPENING)
					{
						setDikEvent(DRIVER0,3);
					}
					break;
				case DOU_DIRBLOWER:
					state = getPusherState(&Controller,DRIVER0);
					if(state == DBL_NGTV_RUNING || state == DBL_STOP)
					{
						setDikEvent(DRIVER0,1);
					}
					else if(state == DBL_PSTV_RUNING)
					{
						setDikEvent(DRIVER0,3);
					}
					break;
				case SGL_DIRBLOWER:
					state = getPusherState(&Controller,DRIVER0);
					if(state == SBL_STOP)
					{
						setDikEvent(DRIVER0,1);
					}
					else
					{
						setDikEvent(DRIVER0,3);
					}	
					break;
				default:
					break;
			}
			break;
		case 0x02:
			
			switch(equip)
			{
				case WIND_PUSHER:
					state = getPusherState(&Controller,DRIVER0);
				  if(state == WIND_OPENED || state == WIND_OPENING || state == WIND_PAUSE )
					{
						setDikEvent(DRIVER0,2);
					}
					else if(state == WIND_CLOSING)
					{
						setDikEvent(DRIVER0,3);
					}
					break;
				case DOU_DIRBLOWER:
					state = getPusherState(&Controller,DRIVER0);
					if(state == DBL_PSTV_RUNING || state == DBL_STOP)
					{
						setDikEvent(DRIVER0,2);
					}
					else if(state == DBL_NGTV_RUNING)
					{
						setDikEvent(DRIVER0,3);
					}
					break;
				case SGL_DIRBLOWER:
					state = getPusherState(&Controller,DRIVER1);
					if(state == SBL_STOP)
					{
						setDikEvent(DRIVER1,1);
					}
					else
					{
						setDikEvent(DRIVER1,3);
					}
					break;
				default:
					break;
			}
			break;
		case 0x04:
			switch(equip)
			{
				case WIND_PUSHER:
					state = getPusherState(&Controller,DRIVER1);
					if(state == WIND_CLOSED || state == WIND_CLOSING || state == WIND_PAUSE )
					{
						setDikEvent(DRIVER1,1);
					}
					else if(state == WIND_OPENING)
					{
						setDikEvent(DRIVER1,3);
					}
					break;
				case DOU_DIRBLOWER:
					state = getPusherState(&Controller,DRIVER1);
					if(state == DBL_NGTV_RUNING || state == DBL_STOP)
					{
						setDikEvent(DRIVER1,1);
					}
					else if(state == DBL_PSTV_RUNING)
					{
						setDikEvent(DRIVER1,3);
					}
					break;
				case SGL_DIRBLOWER:
					state = getPusherState(&Controller,DRIVER2);
					if(state == SBL_STOP)
					{
						setDikEvent(DRIVER2,1);
					}
					else
					{
						setDikEvent(DRIVER2,3);
					}	
					break;
				default:
					break;
			}		
			break;
		case 0x08:
			switch(equip)
			{
				case WIND_PUSHER:
					state = getPusherState(&Controller,DRIVER1);
				  if(state == WIND_OPENED || state == WIND_OPENING || state == WIND_PAUSE )
					{
						setDikEvent(DRIVER1,2);
					}
					else if(state == WIND_CLOSING)
					{
						setDikEvent(DRIVER1,3);
					}
					break;
				case DOU_DIRBLOWER:
					state = getPusherState(&Controller,DRIVER1);
					if(state == DBL_PSTV_RUNING || state == DBL_STOP)
					{
						setDikEvent(DRIVER1,2);
					}
					else if(state == DBL_NGTV_RUNING)
					{
						setDikEvent(DRIVER1,3);
					}
					break;
				case SGL_DIRBLOWER:
					state = getPusherState(&Controller,DRIVER3);
					if(state == SBL_STOP)
					{
						setDikEvent(DRIVER3,1);
					}
					else
					{
						setDikEvent(DRIVER3,3);
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void  DiKeyTask_Creat(void)
{
	gTaskCfgMgEvent.CreatTask(DiKeyTask,7,3,Normal_Type);
}

int8_t setDikEvent(uint8_t chg, uint8_t event)
{
//	if(AutoHandFlag) return 0;
	if(chg>4 || event>4) return -1;
	DiKeyE[chg].cmd = event;
	DiKeyE[chg].flag = 1;
	return 1;
}

int8_t clrDikEvent(uint8_t chg)
{
	if(chg>4) return -1;
	DiKeyE[chg].cmd  = 0;
	DiKeyE[chg].flag = 0;
	return 1;
}

void clrKeyTemp(Bsp_KeyMgEvent_STR *pKey)
{
	pKey->flag = 0;
	pKey->Value = 0;
}


