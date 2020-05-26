#include  "Bsp_Wdt.h"
#include  "stm32f10x.h"

#define _WDT_H  (GPIO_SetBits(GPIOA,GPIO_Pin_0))
#define _WDT_L  (GPIO_ResetBits(GPIOA,GPIO_Pin_0))

static int8_t WdtBsp_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	  

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;				    
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	return 0;
}


int8_t WdtBsp_free(void)
{
		uint32_t ii = 1000;
	
		_WDT_L;
	
	   while(0 != (ii--));
	
		_WDT_H;
	
		return 0;
}


Bsp_WdtMgEvent_STR gWdtMgEvent = 
{
	WdtBsp_Init,
	WdtBsp_free
};
