#include "Bsp_Uart.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
uint16_t gfrequency = 2000;//20KHZ
uint16_t gRate = 3600;
uint16_t displayDuty = 0;
void TIM3_PWM_ToGPIO_Init()
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//?????3??
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //??GPIO???AFIO????????
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3?????  TIM3_CH2->PB5    
 
   //????????????,??TIM3 CH2?PWM????	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//???GPIO
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

void TIM3_PWM_Init(uint16_t arr,uint16_t psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//?????3??
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //??GPIO???AFIO????????
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3?????  TIM3_CH2->PB5    
 
   //????????????,??TIM3 CH2?PWM????	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//???GPIO
 
   //???TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //???????????????????????????
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //??????TIMx??????????? 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????
	//???TIM3 Channel2 PWM??	 
	TIM_OCInitStructure.TIM_Pulse = 732;//arr/2;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//TIM_OCPolarity_High; //????:TIM???????
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //??T??????????TIM3 OC2
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //??TIM3?CCR2????????
	
}

int8_t TIM3_PWM_Open()
{
	TIM_Cmd(TIM3, ENABLE);  //??TIM3
}

int8_t TIM3_PWM_Close()
{
	TIM_Cmd(TIM3, DISABLE);  //??TIM3
}

void SetPwmDuty(uint16_t duty)//150 == 15%
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_Pulse = duty*65535/10000;//arr/2;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//TIM_OCPolarity_High; //????:TIM???????
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //??T??????????TIM3 OC2
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //??TIM3?CCR2????????
	displayDuty = duty;
}