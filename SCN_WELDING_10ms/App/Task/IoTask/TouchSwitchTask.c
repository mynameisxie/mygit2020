#include  "DisplayTask.h"
#include  "MotionTask.h"
#include  "Bsp_OsTask.h"
#include  "BSP_595.h"
#include  "Bsp_Flash.h"
#include "Bsp_Uart.h"
#include  "stm32f10x_gpio.h"
#include  "TouchSwitchTask.h"
#include  "CycTask.h"
#define _IoTaskCyc   2
uint8_t test_flag=0;
static uint8_t step = 0;
uint8_t pre_pos = 0;
uint16_t duty_test = 700;
uint16_t BuzzerFlag=0;
uint8_t SystemState = 0;
//��̤���IO��ʼ��
void TouchIOInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
   //��̤����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_4; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIO
	//npn�������PA6���Ƚ����������PA5��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIO
	
	CLR_NPN_HIGH;//  NPN  ����
	CLR_CMP_HIGH;//�Ƚ��� ����
	displayDuty=0;
}
//��ťλ��IO��ʼ��
void SwitchIOInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PA0/1/2/3 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
uint8_t isTouchPushed()
{
	if(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))return 1;
	if(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8))return 1;
	return 0;
}
uint8_t getSwitchPos(void)
{
	static uint8_t flag = 0;
	static uint16_t counter = 0;
	static uint16_t counter2 = 0;
	static uint16_t counter14 = 0;
	static uint16_t counter15 = 0;
	static uint8_t  pos13 = 0;
	static uint8_t  pos14 = 0;
	static uint8_t  pos15 = 0;
	static uint8_t  prepos13 = 0;
	static uint8_t  prepos14 = 0;
	static uint8_t  prepos15 = 0;
	prepos13 = pos13;
	pos13 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
	if(pos13==0)
	{
		counter2++;
		if(counter2 == 200)
		{
			counter2 = 200;
			return 4;//������
		}
	}
	if(prepos13==0 && pos13 ==1)
	{
//		if(counter2 > 200)
//		{
//			counter2 = 0;
//			return 4;//������
//		}
		if(counter2 > 2 && counter2<200)  
		{
			counter2 = 0;
			return 1;//�̰���
		}
		counter2 =0;
	}

	prepos14 = pos14;
	prepos15 = pos15;
	pos14 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
	pos15 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
	if((pos14 == 1) && (pos15 == 1))
	{
		counter=0;
		flag=0;
		return 0;//�޶���
	}
	counter++;
	if(pos14==0 &&prepos14==1) counter14 = counter;
	if(pos15==0 &&prepos15==1) counter15 = counter;
	if((pos14==0) &&(pos15==0))
	{
		if(0==flag)
		{
			flag = 1;
			if(counter14>counter15) return 2;//˳ʱ��
			if(counter14<counter15) return 3;//��ʱ��
		}
		else
		{
			return 0;//�޶���
		}
	}
	return 0;//�޶���
}
//��ť���	
void SwitchCheck()
{
	uint8_t keyValue = 0;
	static uint16_t counter = 0;
	static uint8_t preState = 0;
	if(AlarmCode != 0) return;
	keyValue = getSwitchPos();
	if(keyValue==1 || keyValue == 4)SetBuzzerSoundTimes(1);
	switch(SystemState)
	{
		case 0://����״̬
				if(keyValue == 1)//�̰�
				{
					SetDisplayType(AMPLIT);
					FlushCursor =1;
					SystemState = 1;
				}
				if(keyValue == 4)//����
				{
					SetDisplayType(TRIGGE_TIME);
					FlushCursor =1;
					SystemState = 2;
				}
			break;
			
		case 1://�����
				if(keyValue == 1)
				{
					FlushCursorIncreas();
				}
				if(keyValue == 2)
				{
					
					if(FlushCursor==1)DispPara.MaxPwmDyty += 10;
					if(FlushCursor==2)DispPara.MaxPwmDyty += 100;
					if(FlushCursor==3)DispPara.MaxPwmDyty += 1000;
					if(DispPara.MaxPwmDyty >= 1000) DispPara.MaxPwmDyty = 1000;
				}
				if(keyValue == 3)
				{
					if(FlushCursor==1)DispPara.MaxPwmDyty -= 10;
					if(FlushCursor==2)DispPara.MaxPwmDyty -= 100;
					if(FlushCursor==3)DispPara.MaxPwmDyty -=1000;
					if(DispPara.MaxPwmDyty >= 1000) DispPara.MaxPwmDyty = 0;
				}
				if(keyValue == 4)
				{
					SystemState = 0;
					SetDisplayType(FREQUENCY);
					FlushCursor =0;
					SysConfig.MaxPwmDyty = DispPara.MaxPwmDyty;
					memcpy(&SysConfig,&DispPara,sizeof(SysConfig));
					flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)(&SysConfig),sizeof(SysConfig)/4);
				}
			break;
		
		case 2://������ʱ��
				if(keyValue == 1)
				{
					FlushCursorIncreas();
				}
				if(keyValue == 2)
				{
					
					if(FlushCursor==1)DispPara.triggeTime += 1;
					if(FlushCursor==2)DispPara.triggeTime += 10;
					if(FlushCursor==3)DispPara.triggeTime += 100;
					if(DispPara.triggeTime >= 999) DispPara.triggeTime = 999;
				}
				if(keyValue == 3)
				{
					if(FlushCursor==1)DispPara.triggeTime -= 1;
					if(FlushCursor==2)DispPara.triggeTime -= 10;
					if(FlushCursor==3)DispPara.triggeTime -=100;
					
					if(DispPara.triggeTime >= 999) DispPara.triggeTime = 0;
				}
				if(keyValue == 4)
				{
					SystemState = 0;
					SetDisplayType(FREQUENCY);
					FlushCursor =0;
					SysConfig.triggeTime = DispPara.triggeTime;
					memcpy(&SysConfig,&DispPara,sizeof(SysConfig));
					flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)(&SysConfig),sizeof(SysConfig)/4);
				}
			break;
		case 3://��Ƶ������

			  if(keyValue == 1)
				{
					S_FlushCursorIncreas();
				}
				if(keyValue == 2)
				{
					if(FlushCursor==1)DispPara.Hfreq += 100;
					if(FlushCursor==2)DispPara.Hfreq += 1000;
					if(FlushCursor==3)DispPara.Hfreq += 10000;
					if(FlushCursor==4)
					{
						SystemState=4;//��һ����������
						SetDisplayType(DOWN_LIMIT);
					}
					if(DispPara.Hfreq >= 30000) DispPara.Hfreq = 30000;
				}
				if(keyValue == 3)
				{
					if(FlushCursor==1)DispPara.Hfreq -= 100;
					if(FlushCursor==2)DispPara.Hfreq -= 1000;
					if(FlushCursor==3)DispPara.Hfreq -=10000;
					if(FlushCursor==4)
					{
						SystemState=7;//��һ����߹��ʱ�
						SetDisplayType(MAX_P);
					}
					if(DispPara.Hfreq >= 30000) DispPara.Hfreq = 0;
				}
				if(keyValue == 4)//�˳�
				{
					SystemState = 8;
					SetDisplayType(END_SET);
					FlushCursor =0;
					memcpy(&SysConfig,&DispPara,sizeof(SysConfig));
					flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)(&SysConfig),sizeof(SysConfig)/4);
				}
			break;
		case 4://����Ƶ������
			  if(keyValue == 1)
				{
					S_FlushCursorIncreas();
				}
				if(keyValue == 2)
				{
					if(FlushCursor==1)DispPara.Lfreq += 100;
					if(FlushCursor==2)DispPara.Lfreq += 1000;
					if(FlushCursor==3)DispPara.Lfreq += 10000;
					if(FlushCursor==4)
					{
						SetDisplayType(SCAN_INTV);
						SystemState=5;//��һ��ǿ��ɨ�����
					}
					if(DispPara.Lfreq >= 30000) DispPara.Lfreq = 30000;
				}
				if(keyValue == 3)
				{
					if(FlushCursor==1)DispPara.Lfreq -= 100;
					if(FlushCursor==2)DispPara.Lfreq -= 1000;
					if(FlushCursor==3)DispPara.Lfreq -=10000;
					if(FlushCursor==4)
					{
						SetDisplayType(UP_LIMIT);
						SystemState=3;//��һ����������
					}
					if(DispPara.Lfreq >= 30000) DispPara.Lfreq = 0;
				}
				if(keyValue == 4)//�˳�
				{
					SystemState = 8;
					SetDisplayType(END_SET);
					FlushCursor =0;
					memcpy(&SysConfig,&DispPara,sizeof(SysConfig));
					flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)(&SysConfig),sizeof(SysConfig)/4);
				}			
			break;
		case 5://����ǿ��ɨ�����
			if(keyValue == 1)
				{
					S_FlushCursorIncreas();
				}
				if(keyValue == 2)
				{
					if(FlushCursor==1)DispPara.scanIntv += 1;
					if(FlushCursor==2)DispPara.scanIntv += 10;
					if(FlushCursor==3)DispPara.scanIntv += 100;
					if(FlushCursor==4)
					{
						SetDisplayType(SOFT_START_TIM);
						SystemState=6;//��һ��������ʱ��
					}
					if(DispPara.scanIntv >= 999) DispPara.scanIntv = 999;
				}
				if(keyValue == 3)
				{
					if(FlushCursor==1)DispPara.scanIntv -= 1;
					if(FlushCursor==2)DispPara.scanIntv -= 10;
					if(FlushCursor==3)DispPara.scanIntv -=100;
					if(FlushCursor==4)
					{
						SystemState=4;//��һ����������
						SetDisplayType(DOWN_LIMIT);
					}
					if(DispPara.scanIntv >= 999) DispPara.scanIntv = 0;
				}
				if(keyValue == 4)//�˳�
				{
					SystemState = 8;
					SetDisplayType(END_SET);
					FlushCursor =0;
					memcpy(&SysConfig,&DispPara,sizeof(SysConfig));
					flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)(&SysConfig),sizeof(SysConfig)/4);
				}			
			break;
		case 6://����������ʱ��
				if(keyValue == 1)
				{
					S_FlushCursorIncreas();
				}
				if(keyValue == 2)
				{
					if(FlushCursor==1)DispPara.softStartTim += 1;
					if(FlushCursor==2)DispPara.softStartTim += 10;
					if(FlushCursor==3)DispPara.softStartTim += 100;
					if(FlushCursor==4)
					{
						SetDisplayType(MAX_P);
						SystemState=7;//��һ����߹��ʱ�
					}
					if(DispPara.softStartTim >= 999) DispPara.softStartTim = 999;
				}
				if(keyValue == 3)
				{
					if(FlushCursor==1)DispPara.softStartTim -= 1;
					if(FlushCursor==2)DispPara.softStartTim -= 10;
					if(FlushCursor==3)DispPara.softStartTim -=100;
					if(FlushCursor==4)
					{
						SetDisplayType(SCAN_INTV);
						SystemState=5;//��һ��ǿ��ɨ�����
					}
					if(DispPara.softStartTim >= 999) DispPara.softStartTim = 0;
				}
				if(keyValue == 4)//�˳�
				{
					SystemState = 8;
					SetDisplayType(END_SET);
					FlushCursor =0;
					memcpy(&SysConfig,&DispPara,sizeof(SysConfig));
					flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)(&SysConfig),sizeof(SysConfig)/4);
				}
			break;
		case 7://������߹��ʱȣ������ã�
				if(keyValue == 1)
				{
					S_FlushCursorIncreas();
				}
				if(keyValue == 2)
				{
					if(FlushCursor==1)DispPara.MaxP += 1;
					if(FlushCursor==2)DispPara.MaxP += 10;
					if(FlushCursor==3)DispPara.MaxP += 100;
					if(FlushCursor==4)
					{
						SetDisplayType(UP_LIMIT);
						SystemState=3;//��һ��
					}
					if(DispPara.MaxP >= 1000) DispPara.MaxP = 1000;
				}
				if(keyValue == 3)
				{
					if(FlushCursor==1)DispPara.MaxP -= 1;
					if(FlushCursor==2)DispPara.MaxP -= 10;
					if(FlushCursor==3)DispPara.MaxP -=100;
					if(FlushCursor==4)
					{
						SystemState=6;//��һ��������ʱ��
						SetDisplayType(SOFT_START_TIM);
					}
					if(DispPara.MaxP >= 1000) DispPara.MaxP = 0;
				}
				if(keyValue == 4)//�˳�
				{
					SystemState = 8;
					SetDisplayType(END_SET);
					FlushCursor =0;
					memcpy(&SysConfig,&DispPara,sizeof(SysConfig));
					flash_write(SYSTEM_CONFIG_ADDR,(uint32_t *)(&SysConfig),sizeof(SysConfig)/4);
				}			
			break;
		case 8://������������
			
			
			break;
		default:
			break;
	}
}

void SetBuzzerSoundTimes(uint8_t times)
{
	if(BuzzerFlag != 0xFF)
	{
		BuzzerFlag = times;
	}
}

void BuzzerSound()
{
	static uint16_t counter=0;
	if(BuzzerFlag >0)
	{

		if(counter == 0)
		{
			SET_ALARM;
		}
		if(counter ==1)
		{
			CLR_ALARM;
		}
		if(counter ==2)
		{
			counter =0;
			if(BuzzerFlag != 0xFF)//������ͣ
			{
				if(--BuzzerFlag == 0)CLR_ALARM;
			}
		}
		counter++;	
	}
	else
	{
		counter =0;
	}
}

void TouchTask(void)
{
	
	SwitchCheck();
	BuzzerSound();
}


void  TouchTask_Creat(void)
{
	gTaskCfgMgEvent.CreatTask(TouchTask,3,10,Normal_Type);
}

