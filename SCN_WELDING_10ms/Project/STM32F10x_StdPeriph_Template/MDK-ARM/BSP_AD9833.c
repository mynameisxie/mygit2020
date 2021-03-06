#include "BSP_AD9833.h"


//***************************
//		Pin assign	   	
//			STM32			AD9833
//		GPIOB_Pin_15 		---> FSYNC
//		GPIOB_Pin_14 		---> SCK
//		GPIOB_Pin_13 		---> DAT
//		GPIOB_Pin_12		---> CS
//***************************	

	/*???? */ 
	#define PORT_FSYNC	GPIOA
	#define PIN_FSYNC	GPIO_Pin_10

	#define PORT_SCK	GPIOA
	#define PIN_SCK		GPIO_Pin_11

	#define PORT_DAT	GPIOA
	#define PIN_DAT		GPIO_Pin_9

//	#define PORT_CS		GPIOA
//	#define PIN_CS		GPIO_Pin_12  //???????

//****************************************************************

	#define FSYNC_0()		GPIO_ResetBits(PORT_FSYNC, PIN_FSYNC)
	#define FSYNC_1()		GPIO_SetBits(PORT_FSYNC, PIN_FSYNC)

	#define SCK_0()		GPIO_ResetBits(PORT_SCK, PIN_SCK)
	#define SCK_1()		GPIO_SetBits(PORT_SCK, PIN_SCK)

	#define DAT_0()		GPIO_ResetBits(PORT_DAT, PIN_DAT)
	#define DAT_1()		GPIO_SetBits(PORT_DAT, PIN_DAT)	

	#define CS_0()		//GPIO_ResetBits(PORT_DAT, PIN_CS)	
	#define CS_1()		//GPIO_SetBits(PORT_DAT, PIN_CS)	

//???AD9833 GPIO

void AD9833_Init_GPIO()
{
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PIN_FSYNC|PIN_SCK|PIN_DAT; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(PORT_SCK, &GPIO_InitStructure);

//	SCK_1();

}



/*
*********************************************************************************************************
*	? ? ?: AD9833_Delay
*	????: ????
*	?    ?: ?
*	? ? ?: ?
*********************************************************************************************************
*/
static void AD9833_Delay(void)
{
	uint16_t i;
	for (i = 0; i < 1; i++);
}

/*
*********************************************************************************************************
*	? ? ?: AD9833_Write
*	????: ?SPI????16?bit??
*	?    ?: TxData : ??
*	? ? ?: ?
*********************************************************************************************************
*/
void AD9833_Write(unsigned int TxData)
{
	unsigned char i;

	SCK_1();
	//AD9833_Delay();
	FSYNC_1();
	//AD9833_Delay();
	FSYNC_0();
	//AD9833_Delay();
	for(i = 0; i < 16; i++)
	{
		if (TxData & 0x8000)
			DAT_1();
		else
			DAT_0();
		
		AD9833_Delay();
		SCK_0();
		AD9833_Delay();		
		SCK_1();
		
		TxData <<= 1;
	}
	FSYNC_1();
	
} 

/*
*********************************************************************************************************
*	? ? ?: AD9833_AmpSet
*	????: ?????????
*	?    ?: 1.amp :???  0- 255
*	? ? ?: ?
*********************************************************************************************************
*/ 


void AD9833_AmpSet(unsigned char amp)
{
	unsigned char i;
	unsigned int temp;
   	
	CS_0();
	temp =0x1100|amp;
	for(i=0;i<16;i++)
	{
	    SCK_0();	
	   if(temp&0x8000)
	   	DAT_1();
	   else
		DAT_0();
		temp<<=1;
	    SCK_1();
	    AD9833_Delay();
	}
	
   	CS_1();
}


/*
*********************************************************************************************************
*	? ? ?: AD9833_WaveSeting
*	????: ?SPI????16?bit??
*	?    ?: 1.Freq: ???, 0.1 hz - 12Mhz
			  2.Freq_SFR: 0 ? 1
			  3.WaveMode: TRI_WAVE(???),SIN_WAVE(???),SQU_WAVE(??)
			  4.Phase : ??????
*	? ? ?: ?
*********************************************************************************************************
*/ 
void AD9833_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************?????16???***********************************/
		frequence_mid=268435456/25;//??25M??
		//????????25MHZ,????????,??MHz ,AD9833????25MHz
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  //??frequence_hex???32?????????,???????14?????;
		frequence_LSB=frequence_hex; //frequence_hex?16???frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;//??????,16???????????14?
		frequence_MSB=frequence_hex>>14; //frequence_hex?16???frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;//??????,16???????????14?

		Phs_data=Phase|0xC000;	//???
		AD9833_Write(0x0100); //??AD9833,?RESET??1
		AD9833_Write(0x2100); //????????,B28??RESET??1

		if(Freq_SFR==0)				  //?????????????0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //???????0????
			AD9833_Write(frequence_LSB); //L14,???????0??14?????
			AD9833_Write(frequence_MSB); //H14 ???????14?????
			AD9833_Write(Phs_data);	//????
			//AD9833_Write(0x2000); /**??FSELECT??0,????????,?????0????**/
	    }
		if(Freq_SFR==1)				//?????????????1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//???????1????
			AD9833_Write(frequence_LSB); //L14,???????1??14???
			AD9833_Write(frequence_MSB); //H14 ?????1?
			AD9833_Write(Phs_data);	//????
			//AD9833_Write(0x2800); /**??FSELECT??0,??FSELECT??1,????????1??,????????,?????1????**/
		}

		if(WaveMode==TRI_WAVE) //???????
		 	AD9833_Write(0x2002); 
		if(WaveMode==SQU_WAVE)	//??????
			AD9833_Write(0x2028); 
		if(WaveMode==SIN_WAVE)	//??????
			AD9833_Write(0x2000); 

}

void AD9833_WaveSeting2(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************?????16???***********************************/
		frequence_mid=268435456/25;//??25M??
		//????????25MHZ,????????,??MHz ,AD9833????25MHz
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  //??frequence_hex???32?????????,???????14?????;
		frequence_LSB=frequence_hex; //frequence_hex?16???frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;//??????,16???????????14?
		frequence_MSB=frequence_hex>>14; //frequence_hex?16???frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;//??????,16???????????14?

		Phs_data=Phase|0xC000;	//???
//		AD9833_Write(0x0100); //??AD9833,?RESET??1
//		AD9833_Write(0x2100); //????????,B28??RESET??1

		if(Freq_SFR==0)				  //?????????????0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //???????0????
			AD9833_Write(frequence_LSB); //L14,???????0??14?????
			AD9833_Write(frequence_MSB); //H14 ???????14?????
//			AD9833_Write(Phs_data);	//????
			//AD9833_Write(0x2000); /**??FSELECT??0,????????,?????0????**/
	  }
		if(Freq_SFR==1)				//?????????????1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//???????1????
			AD9833_Write(frequence_LSB); //L14,???????1??14???
			AD9833_Write(frequence_MSB); //H14 ?????1?
			AD9833_Write(Phs_data);	//????
			//AD9833_Write(0x2800); /**??FSELECT??0,??FSELECT??1,????????1??,????????,?????1????**/
		}

		if(WaveMode==TRI_WAVE) //???????
		 	AD9833_Write(0x2002); 
		if(WaveMode==SQU_WAVE)	//??????
			AD9833_Write(0x2028); 
		if(WaveMode==SIN_WAVE)	//??????
			AD9833_Write(0x2000); 


}


