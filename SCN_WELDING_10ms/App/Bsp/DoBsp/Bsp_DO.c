#include  "Bsp_DO.h"
#include  "stm32f10x.h"
Bsp_DoMgInfo_STR gDoMgInfo;

#define _Do_Stcp_H  (GPIO_SetBits(GPIOA,GPIO_Pin_5))
#define _Do_Stcp_L  (GPIO_ResetBits(GPIOA,GPIO_Pin_5))

#define _Do_Shcp_H   (GPIO_SetBits(GPIOA,GPIO_Pin_6))
#define _Do_Shcp_L   (GPIO_ResetBits(GPIOA,GPIO_Pin_6))

#define _Do_Ds_H   (GPIO_SetBits(GPIOA,GPIO_Pin_4))
#define _Do_Ds_L   (GPIO_ResetBits(GPIOA,GPIO_Pin_4))

#define _Do_OE_H   (GPIO_SetBits(GPIOA,GPIO_Pin_7))
#define _Do_OE_L   (GPIO_ResetBits(GPIOA,GPIO_Pin_7))
static int8_t Bsp_DoGpioInit(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	  
    
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;				    
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	  _Do_Stcp_L;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);


	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;				    
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	  _Do_Shcp_L;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;				    
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;	
	  _Do_Ds_L;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;				    
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;	
	  _Do_OE_H;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	return 0;
}

static int8_t Bsp_DoOpen(void)
{
	return 0;
}

static int8_t Bsp_DoClose(void)
{
	
	return 0;
}

static int8_t Bsp_DoIoctl(void)
{
	return 0;
}

static int8_t Bsp_DoDelayUs(uint32_t i)
{
	i = i*10;

	while(i--);

	return 0;
}


static int8_t Bsp_DoDataOutput(uint8_t byChannel, uint8_t *byDoCode)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		  if(0x00 == byDoCode[i])
		  {
			_Do_Ds_L;
		  }
		  else
		  {
			_Do_Ds_H;
		  }
		  
		  _Do_Shcp_L;
		  Bsp_DoDelayUs(10);
	   
		  _Do_Shcp_H;
		  Bsp_DoDelayUs(10);	  
	}

	_Do_Stcp_L;
	
	Bsp_DoDelayUs(10);
	
	_Do_Stcp_H;
	Bsp_DoDelayUs(5);
	_Do_OE_L;
	
	return 0;
}

static int8_t Bsp_DoWirte(uint8_t *byBuf, uint16_t wlen)
{	
	uint8_t i = 0;
	uint8_t j = 0;
	for(i =0; i<_DO_NUM_MAX ; i++)
	{
		
		if((0x1 << i) == (byBuf[0] & (0x1 << i)))
		{
			if(i==0)j=6;
			else if(i==1)j=5;
			else if(i==2)j=4;
			else if(i==3)j=3;
			else j = 0;
			gDoMgInfo.byDoCode[j] = 0xFF;
		}
		else
		{
			if(i==0)j=6;
			else if(i==1)j=5;
			else if(i==2)j=4;
			else if(i==3)j=3;
			else j = 0;
			gDoMgInfo.byDoCode[j] = 0x00;
		}
	}

	//memcpy(gDoMgInfo.pBackArea->byDoCode,gDoMgInfo.byDoCode,_DO_NUM_MAX);

	return 0;
}

static int8_t Bsp_DoRead(uint8_t *byBuf)
{	
	uint8_t i = 0,j = 0;
	uint8_t byChnValue = 0;
	
	for(i = 0;i < _DO_NUM_MAX;i ++)
	{
		if(i==0)j=6;
		else if(i==1)j=5;
		else if(i==2)j=4;
		else if(i==3)j=3;
		else j = 0;
		if(0x00 != gDoMgInfo.byDoCode[j])
		{

			byChnValue |= (0x01 << (i));
		}
		else
		{
			byChnValue &= ~(0x01 << (i));
		}
	}
	byBuf[0] = byChnValue;
	
	return 0;
}

static int8_t Bsp_DoUpdataCfg(void)
{
	memcpy((uint8_t *)gDoMgInfo.CfgInf,(uint8_t *)gDoMgInfo.CfgInfBack,_DO_NUM_MAX*sizeof(Bsp_DoCfg_STR));
	return 0;
}

static int8_t Bsp_DoChannelDiag(uint8_t byChannel,uint8_t wDoCode)
{
	gDoMgInfo.byChannlQualityCode[byChannel] = _eDoOk;

	return 0;
}

static int8_t Bsp_DoDataOpHandler(void)
{
	uint32_t i = 0;

	Bsp_DoDataOutput(0,gDoMgInfo.byDoCode);
	
	for(i = 0; i < _DO_NUM_MAX; i++)
	{
		Bsp_DoChannelDiag(i,gDoMgInfo.byDoCode[i]);
	}
	
	return 0;
}

static int8_t Bsp_DoDemerHandler(void)
{
	uint32_t i = 0;

	for(i = 0; i < _DO_NUM_MAX; i++)
	{
//		Bsp_DoDataOutput(i,gDoMgInfo.byDoCode[i]);
	}

	return 0;
}

static int8_t Bsp_DoTick(uint8_t byTick)
{
	if(0x01 == gDoMgInfo.byRunMode)
	{
		Bsp_DoDemerHandler();
	}
	else if(0x00 == gDoMgInfo.byCfgUpdataStatus)
	{
		return 0;
	}
	else if(0x01 == gDoMgInfo.byCfgUpdataStatus)
	{
		Bsp_DoUpdataCfg();
		
		gDoMgInfo.byCfgUpdataStatus = 0x02;
	}
	else if(0x02 == gDoMgInfo.byCfgUpdataStatus)
	{
		Bsp_DoDataOpHandler();
	}

	return 0;
}

static int8_t Bsp_DoHardwareInit(void)
{
	uint32_t i = 0;
	
	for(i = 0; i < _DO_NUM_MAX; i++)
	{
		//Bsp_DoDataOutput(i,gDoMgInfo.byDoCode[i]);
		gDoMgInfo.byDoCode[i] = 0x00;
	}
	
	Bsp_DoGpioInit();
	
	Bsp_DoDataOutput(0,gDoMgInfo.byDoCode);

	return (0);
}

static int8_t Bsp_DoSoftwareInit(void)
{	
	uint32_t i = 0;
	
	gDoMgInfo.byCfgUpdataStatus = 0x02;

	for(i = 0; i < _DO_NUM_MAX; i++)
	{
		gDoMgInfo.byChannlQualityCode[i] = _eDoErr;
	}
	
	return (0);
}

static int8_t Bsp_DoInit(void)
{
	Bsp_DoHardwareInit();

	Bsp_DoSoftwareInit();

	return 0;
}

static int8_t Bsp_DocfgAnalysis(uint8_t *buf)
{
	memcpy((uint8_t *)gDoMgInfo.CfgInfBack,buf,_DO_NUM_MAX*sizeof(Bsp_DoCfg_STR));

	gDoMgInfo.byCfgUpdataStatus = 0x01;
		
	return 0;
}

static int8_t Bsp_DogetChnStatus(uint8_t *byBuf)
{
	uint32_t i = 0;
	
	for(i = 0; i < _DO_NUM_MAX;i++)
	{
		if(_eDoOk != gDoMgInfo.byChannlQualityCode[i])
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

Bsp_DoMgEvent_STR gDoMgEvent = 
{
	Bsp_DoInit,
	Bsp_DoOpen,
	Bsp_DoClose,
	Bsp_DoIoctl,
	Bsp_DoWirte,
	Bsp_DoRead,
	Bsp_DoTick,
	Bsp_DocfgAnalysis,
	Bsp_DogetChnStatus
};

