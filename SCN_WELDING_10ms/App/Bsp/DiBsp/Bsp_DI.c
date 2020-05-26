#include  "Bsp_DI.h"
#include  "stm32f10x.h"
#include  "core_cm3.h"

Bsp_DiMgInfo_STR gDiMgInfo;

#define _Di_CP_H  (GPIO_SetBits(GPIOB,GPIO_Pin_11))
#define _Di_CP_L  (GPIO_ResetBits(GPIOB,GPIO_Pin_11))

#define _Di_PL_H   (GPIO_SetBits(GPIOB,GPIO_Pin_10))
#define _Di_PL_L   (GPIO_ResetBits(GPIOB,GPIO_Pin_10))

#define _Di_GetData   (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1))

static int8_t Bsp_DiHardwareInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	  

	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;				    
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);


	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;				    
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;				    
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;	
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	return 0;
}

static int8_t Bsp_DiSoftwareInit(void)
{
	uint32_t i = 0;
	
	gDiMgInfo.byCfgUpdataStatus = 0x02;
	
	for(i = 0; i < _DI_NUM_MAX; i++)
	{
		gDiMgInfo.byChannlQualityCode[i] = _eDiErr;
		gDiMgInfo.byDiCode[i] = 0;
	}
	
	return 0;
}

static int8_t Bsp_DiInit(void)
{
	Bsp_DiHardwareInit();
	
	Bsp_DiSoftwareInit();
	
	return 0;
}


static int8_t Bsp_DiOpen(void)
{
	return 0;
}

static int8_t Bsp_DiClose(void)
{
	
	return 0;
}

static int8_t Bsp_DiIoctl(void)
{
	return 0;
}

static int8_t Bsp_DiWirte(uint8_t *byBuf, uint16_t wlen)
{	
	return 0;
}
static int8_t Bsp_DiDataCollection(uint8_t byChannel, uint8_t *pbyDICode)
{
	uint8_t i = 0;

	_Di_PL_L;
	_Di_CP_L;
	
	__NOP();
	__NOP();
	_Di_PL_H;

	__NOP();
	
	for(i = 0;i < _DI_NUM_MAX;i ++)
	{
		_Di_CP_L;
		__NOP();
		
		if(0x00 == _Di_GetData)
		{
			pbyDICode[_DI_NUM_MAX-i-1] = 0xFF;
		}
		else
		{
			pbyDICode[_DI_NUM_MAX-i-1] = 0x00;
		}
		
		_Di_CP_H;
		__NOP();
	}

	
	return 0;
}

static int8_t Bsp_DiRead(uint8_t *byBuf)
{	
	uint8_t byChnStatus = 0;
	uint8_t i = 0;
	
	for(i = 0;i < _DI_NUM_MAX;i ++)
	{
		if(0x00 != gDiMgInfo.byDiCode[i])
		{
			byChnStatus |= (0x01 << (i));
		}
		else
		{
			byChnStatus &= ~(0x01 << (i));
		}
	}
	byBuf[0] = byChnStatus;

	return 0;
}

static int8_t Bsp_DiUpdataCfg(void)
{
	memcpy((uint8_t *)gDiMgInfo.CfgInf,(uint8_t *)gDiMgInfo.CfgInfBack,_DI_NUM_MAX*sizeof(Bsp_DiCfg_STR));
	return 0;
}

static int8_t Bsp_DiChannelDiag(uint8_t byChannel,uint8_t wDiCode)
{
	gDiMgInfo.byChannlQualityCode[byChannel] = _eDiOk;

	return 0;
}

static int8_t Bsp_DiDataOpHandler(void)
{
	uint32_t i = 0;
	
	Bsp_DiDataCollection(i,&gDiMgInfo.byDiCode[i]);
	
	for(i = 0; i < _DI_NUM_MAX; i++)
	{
		Bsp_DiChannelDiag(i,gDiMgInfo.byDiCode[i]);
	}

	return 0;
}

static int8_t Bsp_DiDemerHandler(void)
{
	uint32_t i = 0;

	for(i = 0; i < _DI_NUM_MAX; i++)
	{
		Bsp_DiDataCollection(i,&gDiMgInfo.byDiCode[i]);
	}

	return 0;
}

static int8_t Bsp_DiTick(uint8_t byTick)
{
	if(0x01 == gDiMgInfo.byRunMode)
	{
		Bsp_DiDemerHandler();
	}
	else if(0x00 == gDiMgInfo.byCfgUpdataStatus)
	{
		return 0;
	}
	else if(0x01 == gDiMgInfo.byCfgUpdataStatus)
	{
		Bsp_DiUpdataCfg();
		
		gDiMgInfo.byCfgUpdataStatus = 0x02;
	}
	else if(0x02 == gDiMgInfo.byCfgUpdataStatus)
	{
		Bsp_DiDataOpHandler();
	}

	return 0;
}

static int8_t Bsp_DicfgAnalysis(uint8_t *buf)
{
	memcpy((uint8_t *)gDiMgInfo.CfgInfBack,buf,_DI_NUM_MAX*sizeof(Bsp_DiCfg_STR));

	gDiMgInfo.byCfgUpdataStatus = 0x01;
	
	return 0;
}

static int8_t Bsp_DigetChnStatus(uint8_t *byBuf)
{
	uint32_t i = 0;
	
	for(i = 0; i < _DI_NUM_MAX;i++)
	{
		if(_eDiOk != gDiMgInfo.byChannlQualityCode[i])
		{
			byBuf[0] |= (0x01 << i);
		}
		else
		{
			byBuf[0] &= ~(0x01 << i);
		}
	}
	return 0;
}


Bsp_DiMgEvent_STR gDiMgEvent = 
{
	Bsp_DiInit,
	Bsp_DiOpen,
	Bsp_DiClose,
	Bsp_DiIoctl,
	Bsp_DiWirte,
	Bsp_DiRead,
	Bsp_DiTick,
	Bsp_DicfgAnalysis,
	Bsp_DigetChnStatus
};

