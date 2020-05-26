#include "stm32f10x.h"
#include"Bsp_Led.h"
static void LedIoInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	  

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;				    
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;				    
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;				    
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
static void SetLed(uint8_t led)
{
	switch(led)
	{
		case RUN:
			GPIO_SetBits(GPIOC,GPIO_Pin_15);
		break;
		case STATE:
			GPIO_SetBits(GPIOC,GPIO_Pin_14);
		break;
		case ERR:
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
		break;
		default:
		break;
	}
}
static void ClrLed(uint8_t led)
{
	switch(led)
	{
		case RUN:
			GPIO_ResetBits(GPIOC,GPIO_Pin_15);
		break;
		case STATE:
			GPIO_ResetBits(GPIOC,GPIO_Pin_14);
		break;
		case ERR:
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		break;
		default:
		break;
	}
}
Bsp_Led_STR gLedEvet = 
{
	LedIoInit,
	SetLed,
	ClrLed,
	
};
