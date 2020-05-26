#include  "CycTask.h"
#include  "Bsp_OsTask.h"
#include  "stm32f10x_gpio.h"
#include  "Bsp_Uart.h"
#include  "Bsp_ADC.h"
#include  "TouchSwitchTask.h"
#include  "BSP_AD9833.h"
#include  "Bsp_Flash.h"
#include  "DisplayTask.h"
#define _CycTaskCyc   10
#define TRACK_INTERV 5 //??????
#define TRACK_EXTENT 50 //??????
uint8_t AlarmCode = 0;
static uint8_t TskState = 0;
extern uint8_t LedBuff[4];
uint8_t PWRFlag = 0;//系统启动参数标志
uint16_t VFB;//反馈电流
uint16_t maxVFB;//反馈电流2
uint16_t SecmaxVFB;
uint32_t  Vresult[4] = {0};
uint8_t soft_flag = 0;//软启动标志
uint8_t TrackingFlag = 0;
uint8_t search_flag = 0;
uint8_t Trackstep = 0;

static uint32_t FreqTrcking = 0;
static uint32_t MaxcurrentFreq = 0;
void delayn(uint16_t cnt)
{
	for(;cnt>0;cnt--);
}

uint32_t FrequencyTracking2(void)
{
	static uint8_t step = 0;
	static uint16_t counter = 0;
	static uint32_t currentFreq = 0;
	if(TrackingFlag == 1)
	{
		switch(Trackstep)
		{
			case 0://
				SetPwmDuty(100);
				currentFreq = FreqTrcking + TRACK_EXTENT;
				AD9833_WaveSeting2((currentFreq)*4,0,SQU_WAVE,0);//
				Trackstep++;
				maxVFB =0;
				break;
			case 1://??30ms
				if(counter++ >= 10)
				{
					counter = 0;
					Trackstep++;
				}
				break;
			case 2://??AD?
				VFB = getRealAdcValue(DTECTOR);//??????
				if(VFB > maxVFB )//????? 
				{
					maxVFB = VFB;
					MaxcurrentFreq = currentFreq;
				}
				Trackstep++;
				break;
			case 3://???????
				if(currentFreq >= FreqTrcking - TRACK_EXTENT)
				{
					currentFreq = currentFreq - TRACK_INTERV;
				}
				else
				{
					AD9833_WaveSeting2((MaxcurrentFreq)*4,0,SQU_WAVE,0);
					gfrequency = (MaxcurrentFreq)/10;
					FreqTrcking = MaxcurrentFreq;
					TrackingFlag = 2;//????
					return 0;
				}
				__disable_irq();        /*!< global Interrupt enable */
				AD9833_WaveSeting2(currentFreq*4,0,SQU_WAVE,0);
				__enable_irq();
				Trackstep ++;//????
				break;
			case 4://????
				if(++counter >= 4)
				{
					counter = 0;
					Trackstep=2;
				}
				break;
			default:
				break;
		}
	}
	else
	{
		
	}
}
//频率扫描，获取最佳频率
uint32_t FrequencySweep()
{
	static uint8_t step = 0;
	static uint16_t i = 0;
	static uint16_t counter = 0;
	static uint32_t currentFreq = 0;

	static uint8_t flag = 0;
	switch(step)
	{
		case 0://第一个测试频率
			AD9833_WaveSeting2(SysConfig.Hfreq*4,0,SQU_WAVE,0);//从频率下限开始检测
			currentFreq = SysConfig.Hfreq;
			MaxcurrentFreq = 0;
		  flag = 1;
			step++;
			break;
		case 1://等待35ms
			if(counter++ >= 35)
			{
				counter = 0;
				step++;
			}
			break;
		case 2://获取AD值
			VFB = getRealAdcValue(DTECTOR);//探头电流反馈
			if(VFB > maxVFB)//电压在上升 
			{
				maxVFB = VFB;
				MaxcurrentFreq = currentFreq;
			}

			step++;
		case 3://测试下一个频率
			if(currentFreq >= SysConfig.Lfreq)
			{
				currentFreq = currentFreq - SysConfig.scanIntv;
			}
			else
			{
				MaxcurrentFreq += 150;
				AD9833_WaveSeting2((MaxcurrentFreq)*4,0,SQU_WAVE,0);
				gfrequency = (MaxcurrentFreq)/10;
				CLR_CMP_HIGH;		
        displayDuty	= 0;			
				if((MaxcurrentFreq < SysConfig.Lfreq) || (maxVFB <= 20))
				{
					SetCycTaskState(S_ALRM);	
					SetBuzzerSoundTimes(0xFF);
					AlarmCode = 3;
				}
				return 1;//未发现合适频率
			}
			__disable_irq();        /*!< global Interrupt enable */
			AD9833_WaveSeting2(currentFreq*4,0,SQU_WAVE,0);
			__enable_irq();
			step ++;//继续采样
			break;
		case 4://等待采样
			if(++counter >= 5)
			{
				counter = 0;
				step=2;
			}
			break;
		default:
		  break;			
	}
	return 2;
}
uint8_t SelfTest(void)
{
	static uint16_t tmcnt = 0;
	tmcnt++;
	if(tmcnt == 200)//2S  波形发生器启动
	{
		AD9833_WaveSeting(SysConfig.Hfreq*4,0,SQU_WAVE,0);//从频率下限开始检测
		return 2;
	}
	if(tmcnt == 300)//3秒  NPN拉高 
	{
		SET_NPN_HIGH;
		SetPwmDuty(100);//10.17占空比
		TIM3_PWM_Open();
		return 2 ;
	}
	else if(tmcnt == 310)//3.1s之后  比较器拉高
	{
		SET_CMP_HIGH;
		search_flag = 1;
		return 2;
	}
	if(search_flag == 2)
	{
		return 0;
	}
	if(tmcnt>=10000)tmcnt=10000;
	return 2;
}
void AlrmValueCheck(void)
{
	static uint16_t V_Pout = 0;
	static uint8_t counter =0;
	if(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))
	{
		SetCycTaskState(S_ALRM);
		SetDisplayType(ALARM);
		SetBuzzerSoundTimes(0xFF);
		AlarmCode = 4;
	}
}

void softStartF(uint16_t Dsty,uint16_t interv)
{
	uint16_t mDuty = (Dsty-100)/interv;
	uint16_t duty = 100;//从10%开始软启动
	uint16_t i = 0;
	//逐渐增大功率-
	for(i=0; i < interv; i++)
	{
		duty += mDuty;
		if(duty > SysConfig.MaxPwmDyty)return;
		SetPwmDuty(duty);	
		delayn(5000);
	}
}


void SetCycTaskState(uint8_t state)
{
	TskState = state;
}
//主任务
void CycTask()
{
	uint16_t i =0;
	uint8_t result = 0;
	static uint8_t PreTskState = S_SELF_TEST;
	static uint8_t test_flag = 0;
	static uint8_t cnt = 0;
	static uint8_t trigg_flag = 0;
	static uint16_t counter = 0;//任务计数器
	if(PWRFlag == 1) return;//不运行任务
	AlrmValueCheck();
	switch(TskState)
	{
		case S_SELF_TEST://上电自检
			result = SelfTest();
			if(result == 0) //自检正常
			{
				SetDisplayType(FREQUENCY);//显示频率
				TskState = S_RUNNING;
				PreTskState = S_SELF_TEST;
				soft_flag = 0;
			}
		  else if(result == 1) TskState = S_SET_PARA;//设置参数
			else if(result == 3) TskState = S_ALRM;//报警
			break;
		case  S_RUNNING:
			test_flag = isTouchPushed();
			if(test_flag)//启动按钮/脚踏 被触发
			{
				if((SysConfig.triggeTime != 0) && (trigg_flag==0))//点动触发，有触发时间
				{
					TskState = S_TRIG_DELAY;
				}
				if((soft_flag == 0)&&(trigg_flag==0))//软启动
				{
					TrackingFlag = 1;
					SET_CMP_HIGH;
					soft_flag = 1;
					softStartF(SysConfig.MaxPwmDyty, SysConfig.softStartTim);
				}
			}
			else
			{
				trigg_flag =0;
				soft_flag = 0;
				CLR_CMP_HIGH;
				displayDuty	= 0;			
			}
			break;
		case  S_TRIG_DELAY:
			if(counter++ > (SysConfig.triggeTime))
			{
				//结束触发，返回正常运行
				TskState = S_RUNNING;
				displayDuty	= 0;			
				soft_flag = 0;
				CLR_CMP_HIGH;
				counter = 0;
				trigg_flag = 1;
			}
			break;
		case 	S_ALRM://报警
			SetDisplayType(ALARM);
			CLR_CMP_HIGH;
			CLR_NPN_HIGH;
			displayDuty	= 0;			
			break;

		default:
			break;
	}
}

void  CycTask_Creat(void)
{
	TskState = S_SELF_TEST;//上电先自检
	gTaskCfgMgEvent.CreatTask(CycTask,5,_CycTaskCyc,Normal_Type);
}
