#include  "Bsp_ADC.h"
#include  "stm32f10x.h"

#define N 10 //
#define M 4 //
#define CHANEL0_LIMIT  300
#define CHANEL1_LIMIT  300
#define CHANEL2_LIMIT  300
#define CHANEL3_LIMIT  300
uint16_t value[M];
uint16_t valueMax[M];
uint16_t AD_Value[N][M]; //
uint16_t After_filter[M]; //

extern uint8_t soft_flag;
int i;


void ADCIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PA0/1/2/3 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void ADC1_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	ADC_DeInit(ADC1); //

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB

| RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO |RCC_APB2Periph_USART1, ENABLE ); //??ADC1????,??????

RCC_ADCCLKConfig(RCC_PCLK2_Div6); //最大频率为  72M/6=12M,ADC

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //
	ADC_InitStructure.ADC_ScanConvMode =ENABLE; //
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC
	ADC_InitStructure.ADC_NbrOfChannel = M; //
	ADC_Init(ADC1, &ADC_InitStructure); //


	//
	//
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_71Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_71Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_71Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_71Cycles5 );


	// 
	ADC_DMACmd(ADC1, ENABLE);


	ADC_Cmd(ADC1, ENABLE); //

	ADC_ResetCalibration(ADC1); //
	while(ADC_GetResetCalibrationStatus(ADC1)); //


	ADC_StartCalibration(ADC1); //

	while(ADC_GetCalibrationStatus(ADC1)); //

}


void DMA_Configuration(void)
{

	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1); //?DMA???1?????????
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA??ADC???
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; //DMA?????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //????????????
	DMA_InitStructure.DMA_BufferSize = N*M; //DMA???DMA?????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //?????????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //?????????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //?????16?
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //?????16?
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //?????????
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA?? x??????
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA??x????????????
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //??DMA_InitStruct?????????DMA???
	DMA_Cmd(DMA1_Channel1, DISABLE); //??DMA??	
}

void Init_ADC1(void)
{  
	DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;	
	ADCIO_Configuration();
	ADC1_Configuration();
	DMA_Configuration();
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //??DMA??	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	

}


u16 GetVolt(u16 advalue)
{
	
	return (u16)(advalue * 330 / 4096); //???????100?,????????

}

void filter(void)
{
	uint32_t sum = 0;
	u8 count;
	uint8_t i=0;
	for(i=0;i<4;i++)
	{
		for ( count=0;count<N;count++)
		{
			sum += AD_Value[count][i];
		}
		After_filter[i]=sum/N;
		sum=0;
	}
}



uint16_t getRealAdcValue(uint8_t channel)
{
		filter();
	return GetVolt(After_filter[channel]);
}
int AdcCheck(void)
{
	static uint16_t src = 0;
	uint8_t i=0;
	filter();
	for(i=0;i<4;i++)
	{
		value[i] = GetVolt(After_filter[i]);
	}
}


