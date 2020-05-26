#include "stm32f10x.h"
#include <stdio.h>
#include "Bsp_OsTask.h"
#include "Bsp_Uart.h"
#include "CycTask.h"
#include "Bsp_Flash.h"

#include "BSP_595.h"
#include  "TouchSwitchTask.h"
#include "DisplayTask.h"
#include "BSP_AD9833.h"
#include  "Bsp_ADC.h"
/*
************************************************************************************************************************
*  ������ɨ��ģ��                                                   
*  ������ͨ�Ų�����   ��9600 ��Bsp_Uart                                         
*	 ������ͨ�Ų�����   ��31300��Bsp_Uart
*  �ֵ�ԭװͨ�Ų����� ��57600��Bsp_Uart              
*  ֧�ֵ�342��ذ汾  ��3.3.14                         
*  ֧�ֵ�326��ذ汾  ��
*
************************************************************************************************************************
*/
extern void OlcdTaskInit(void);
extern void TIM3_PWM_Init(uint16_t arr,uint16_t psc);
extern uint16_t gfrequency;

void delay_ms()
{
	uint16_t i=0;
	for(i=0;i<5000;i++)
	;
}
static  void  App_ObjCreate (void)
{
//	uint16_t  arr = 720000/gfrequency;
//	TIM3_PWM_Init(arr,0);//PWM��ʼ��
//	TIM3_PWM_ToGPIO_Init();
	Init_ADC1();//ADC��ʼ��
	AD9833_Init_GPIO();
	HC595_GPIO_Config();
	TouchIOInit();
	SwitchIOInit();
	
	TIM3_PWM_Init(7200,0);//10.17ռ�ձ�
	SetPwmDuty(10);
	TIM3_PWM_Open();
//	TIM1_Int_Init(50,72);//1MƵ�ʣ�����50us
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

static  void  App_TaskCreate (void)
{
  CycTask_Creat();
	DisplayTask_Creat();
	TouchTask_Creat();
}

void SetVector(void)
{
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);   
}

void RCC_Configuration(void)
{   
  SystemInit();
} 

static  void  BSP_SysInit (void)
{
	SetVector();
	RCC_Configuration();
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOC,ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
}


int main(void)
{
	__disable_irq();
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	BSP_SysInit();
	App_ObjCreate();
	App_TaskCreate();
	falshCheck();
	if(0==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))
	{
		delay_ms();
		if(0==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))
		{
			SystemState = 3;
			SetDisplayType(UP_LIMIT);
			PWRFlag = 1;
		}
	}
	HC595_Send_Multi_Byte(LedBuff,3,T_LED);
	__enable_irq();
	if (SysTick_Config(SystemCoreClock/1000))
	{ 
		while (1);
	}
	
	while(1)
	{
		gTaskCfgMgEvent.TaskRun();
	}
}
