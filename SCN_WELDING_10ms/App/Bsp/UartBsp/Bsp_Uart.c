#include "Bsp_Uart.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"

#define _PORT_SUPPURT    (2)
#define _USARTx_         (USART2)
#define _USARTx_IRQn     (USART2_IRQn)
#define _USARTx_TIMEOUT     (10)  //单位毫秒

static BSP_SerialCfg_STR     gSerialCfg;

static uint32_t SerialBsp_GetBaud(eSerBaud eBaud)
{
	uint32_t dwBaud = SERIAL_BAUDRATE_9600;
	
	switch ( eBaud )
	{
		case _eB1200:
			dwBaud = SERIAL_BAUDRATE_1200;
		break;
		case _eB2400:
			dwBaud = SERIAL_BAUDRATE_2400;
		break;
		case _eB4800:
			dwBaud = SERIAL_BAUDRATE_4800;
		break;
		case _eB9600:
			dwBaud = SERIAL_BAUDRATE_9600;
		break;		
		case _eB19200:
			dwBaud = SERIAL_BAUDRATE_19200;
		break;
		case _eB38400:
			dwBaud = SERIAL_BAUDRATE_38400;
		break;
		case _eB57600:
			dwBaud = SERIAL_BAUDRATE_57600;
		break;
		case _eB115200:
			dwBaud = SERIAL_BAUDRATE_115200;
		break;
		case _eB230400:
			dwBaud = SERIAL_BAUDRATE_230400;
		break;
		case _eB460800:
			dwBaud = SERIAL_BAUDRATE_460800;
		break;
		default:
			break;
	}

	return dwBaud;
}

static uint8_t SerialBsp_GetDataBits(eSerDataBits eDataBits)
{
	uint8_t byDataBits = USART_WordLength_8b;
	
	switch ( eDataBits )
	{
		case _eBITS_8:
			byDataBits = USART_WordLength_8b;
		break;		
		default:
			break;
	}

	return byDataBits;
}

static uint32_t SerialBsp_GetParity(eSerParity	 eParity)
{
	uint32_t byParity = USART_Parity_No;
	
	switch ( eParity )
	{
		case _eNO_PARITY:
			byParity = USART_Parity_No;
		break;
		case _eODD_PARITY:
			byParity = USART_Parity_Odd;
		break;
		case _eEVEN_PARITY:
			byParity = USART_Parity_Even;
		break;
		default:
			break;
	}

	return byParity;
}

static uint32_t SerialBsp_GetStopBits(eSerStopBits eStopBits)
{
	uint32_t byStopBits = USART_StopBits_1;
	
	switch ( eStopBits )
	{
		case _eSTOP_1:
			byStopBits = USART_StopBits_1;
		break;
		case _eSTOP_1_5:
			byStopBits = USART_StopBits_1_5;
		break;
		case _eSTOP_2:
			byStopBits = USART_StopBits_2;
		break;
		default:
			break;
	}

	return byStopBits;
}

void SerialBsp_IRQnEnable(uint16_t byPort)
{
  NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = _USARTx_IRQn;			     	//设置串口2中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	     	//抢占优先级 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//子优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能
  NVIC_Init(&NVIC_InitStructure);
}

void SerialBsp_IRQnDisable(uint16_t byPort)
{
  NVIC_InitTypeDef NVIC_InitStructure;
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = _USARTx_IRQn;			     	//设置串口2中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	     	//抢占优先级 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//子优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;					//使能
  NVIC_Init(&NVIC_InitStructure);
}

void USART_RxMode(void)
{
  gSerialCfg.dwFifoRxCnt = 0;

	USART_ITConfig(_USARTx_, USART_IT_RXNE, ENABLE);
	
//	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void USART_TxMode(void)
{
	//GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void USART_GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;	         
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);		    

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;	       
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);		   

		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;	         
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);		    

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;	       
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);		  
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;	       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
//		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;	       
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	
//	
//		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;	       
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	
//	
//		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;	       
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;	       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;	       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	//上电设置
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;	       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void USART_Open(USART_TypeDef* USARTx)
{
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);               
	USART_ITConfig(USARTx, USART_IT_ERR, ENABLE);  
	USART_ITConfig(USARTx, USART_IT_ORE, ENABLE); 			
}

void USART_Config(USART_TypeDef* USARTx,void *pcfg)
{
	Ser_cfg_STR *pSerial_cfg = (Ser_cfg_STR *)pcfg;
	
  USART_InitTypeDef USART_InitStructure;  

	USART_InitStructure.USART_BaudRate = SerialBsp_GetBaud(pSerial_cfg->eBaud);					
	USART_InitStructure.USART_WordLength = SerialBsp_GetDataBits(pSerial_cfg->eDataBits);		
	USART_InitStructure.USART_StopBits = SerialBsp_GetStopBits(pSerial_cfg->eStopBits);			
	USART_InitStructure.USART_Parity = SerialBsp_GetParity(pSerial_cfg->eParity);				
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				
		
	USART_Init(USARTx, &USART_InitStructure);	
	
	USART_Open(USARTx);
	
	USART_Cmd(USARTx, ENABLE);
}

static int8_t SerialBsp_Init(uint16_t byPort)
{
    Ser_cfg_STR Serial_cfg = {_eB57600,
							  _eBITS_8,
							  _eNO_PARITY,					  
								_eSTOP_1
							};
	
	if(_PORT_SUPPURT != byPort)
	{
		return -1;
	}

	SerialBsp_IRQnDisable(byPort);
		
	USART_GpioInit();
	
	USART_Config(_USARTx_,&Serial_cfg);

	USART_RxMode();
	
	SerialBsp_IRQnEnable(byPort);
	
	return 0;
}

int8_t SerialBsp_Open(uint16_t byPort)
{
	if(_PORT_SUPPURT != byPort)
	{
		return -1;
	}
	
	return 0;
}

int8_t SerialBsp_close(uint16_t byPort)
{
	if(_PORT_SUPPURT != byPort)
	{
		return -1;
	}
	
	return 0;
}

int8_t SerialBsp_cfg(uint16_t byPort,void *pcfg)
{
	if(_PORT_SUPPURT != byPort)
	{
		return -1;
	}

	SerialBsp_IRQnDisable(byPort);
	
	USART_Config(_USARTx_,pcfg);

	USART_RxMode();

	SerialBsp_IRQnEnable(byPort);
	
	return 0;
}

int8_t SerialBsp_wirte(uint16_t byPort,uint8_t *byBuf, uint16_t wlen)
{
	if(_PORT_SUPPURT != byPort)
	{
		return -1;
	}

	 gSerialCfg.dwFifoTxCnt = 0;
	
	 memcpy(gSerialCfg.byTxBuf,byBuf,wlen);

	 gSerialCfg.dwTxCnt = wlen;

	 USART_TxMode();
   
	 USART_ITConfig(_USARTx_, USART_IT_TXE, DISABLE);
	 
	 USART_SendData(_USARTx_,gSerialCfg.byTxBuf[gSerialCfg.dwFifoTxCnt]);
		
	 gSerialCfg.dwFifoTxCnt++;
	 
	 USART_ITConfig(_USARTx_, USART_IT_TXE, ENABLE);
	 
	return 0;
}
int8_t SerialBsp_read(uint16_t byPort,uint8_t *byBuf,uint16_t *pwlen)
{
	if(_PORT_SUPPURT != byPort)
	{
		return -1;
	}

	if(_eCompleteRece == gSerialCfg.eReceMode)
	{
		memcpy(byBuf,gSerialCfg.byRxBuf,gSerialCfg.dwRxCnt);
		*pwlen = gSerialCfg.dwRxCnt;
		gSerialCfg.eReceMode = _eIdle;
		return 0;
	}
		
	return -1;
}


int8_t SerialBsp_intHandler(uint16_t byPort)
{
	if(_PORT_SUPPURT != byPort)
	{
		return -1;
	}

	if(USART_GetITStatus(_USARTx_, USART_IT_RXNE) != RESET)	 //判断读寄存器是否非空
	{
		USART_ClearITPendingBit(_USARTx_,	USART_IT_RXNE); 
	  if(gSerialCfg.dwFifoRxCnt < _BUFs)
	  {
		  gSerialCfg.byRxBuf[gSerialCfg.dwFifoRxCnt] = USART_ReceiveData(_USARTx_);   //将读寄存器的数据缓存到接收缓冲区里
	  }
	  gSerialCfg.dwFifoRxCnt++;
	  gSerialCfg.eReceMode = _eReceing;
	  gSerialCfg.dwRxTo    = _USARTx_TIMEOUT;
	}
	else if(USART_GetITStatus(_USARTx_, USART_IT_ORE) != RESET)	 //判断读寄存器是否非空
	{
		USART_ReceiveData(_USARTx_);
	  USART_ClearITPendingBit(_USARTx_,  USART_IT_ORE);  
	} 
	 else if(USART_GetITStatus(_USARTx_, USART_FLAG_ORE) != RESET)	 
	{
		USART_ReceiveData(_USARTx_);
	  USART_ClearITPendingBit(_USARTx_,  USART_FLAG_ORE);  
	}	
	else if(USART_GetITStatus(_USARTx_, USART_IT_TXE) != RESET)			//这段是为了避免STM32 USART 第一个字节发不出去的BUG 
	{ 				  
	   USART_ClearITPendingBit(_USARTx_,	USART_IT_TXE);	

	  if(gSerialCfg.dwFifoTxCnt < gSerialCfg.dwTxCnt)
	  {
	  	USART_SendData(_USARTx_,gSerialCfg.byTxBuf[gSerialCfg.dwFifoTxCnt]);
	  }
	  else
	  {
		USART_ITConfig(_USARTx_, USART_IT_TXE, DISABLE);
		gSerialCfg.eTxMode = _eTxWaiteRece;
		gSerialCfg.byTxDelayCnt = 5;
	  }
	  gSerialCfg.dwFifoTxCnt++;
	} 
	else if(USART_GetITStatus(_USARTx_, USART_IT_LBD) != RESET)  
	{
	  USART_ClearITPendingBit(_USARTx_,  USART_IT_LBD);  
	}
	else if(USART_GetITStatus(_USARTx_, USART_IT_CTS) != RESET)  
	{
	  USART_ClearITPendingBit(_USARTx_,  USART_IT_CTS);  
	}
	 else if(USART_GetITStatus(_USARTx_, USART_IT_ERR) != RESET)	   
	{
	  USART_ClearITPendingBit(_USARTx_,  USART_IT_ERR);  
	}
	 else if(USART_GetITStatus(_USARTx_, USART_IT_NE) != RESET)  
	{
	  USART_ClearITPendingBit(_USARTx_,  USART_IT_NE);  
	}
	 else if(USART_GetITStatus(_USARTx_, USART_IT_FE) != RESET)  
	{
	  USART_ClearITPendingBit(_USARTx_,  USART_IT_FE);  
	}
	else if(USART_GetITStatus(_USARTx_, USART_IT_PE) != RESET)   
	{
	  USART_ClearITPendingBit(_USARTx_,  USART_IT_PE);  
	}
	 else if(USART_GetITStatus(_USARTx_, USART_IT_TC) != RESET)  
	{
	  USART_ClearITPendingBit(_USARTx_,  USART_IT_TC);  
	} 
	 else if(USART_GetITStatus(_USARTx_, USART_IT_IDLE) != RESET)    
	{
	  USART_ClearITPendingBit(_USARTx_,  USART_IT_IDLE);	
	}
	else
	{
	}
	
	NVIC_ClearPendingIRQ(_USARTx_IRQn);//清除中断标志

	return 0;
}

int8_t SerialBsp_tick(uint8_t byTick)
{
	if(_eReceing == gSerialCfg.eReceMode)
	{
		if(gSerialCfg.dwRxTo > 0)
		{
			gSerialCfg.dwRxTo --;
		}
		else
		{
			gSerialCfg.eReceMode = _eCompleteRece;
			gSerialCfg.dwRxCnt   = gSerialCfg.dwFifoRxCnt;
			USART_RxMode();
		}
	}

	if(_eTxWaiteRece == gSerialCfg.eTxMode)
	{
		if(0x00 == gSerialCfg.byTxDelayCnt)
		{
			gSerialCfg.eTxMode = _eTxCompleteRece;
			USART_RxMode();
		}
		gSerialCfg.byTxDelayCnt--;
	}
	return 0;
}
int8_t SerialBsp_setbaud(uint16_t byPort,uint8_t ebaud)
{
  Ser_cfg_STR Serial_cfg = {_eB9600,
							  _eBITS_8,
							  _eNO_PARITY,					  
								_eSTOP_1
							};
	switch(ebaud)
	{
		case 0:
			Serial_cfg.eBaud = _eB115200;
		break;
		case 1:
			Serial_cfg.eBaud = _eB57600;
		break;
		case 2:
			Serial_cfg.eBaud = _eB38400;
		break;
		case 3:
			Serial_cfg.eBaud = _eB19200;
		break;
		case 4:
			Serial_cfg.eBaud = _eB9600;
		break;
		default:
			Serial_cfg.eBaud = _eB9600;
		break;
	}
	if(_PORT_SUPPURT != byPort)
	{
		return -1;
	}
	
	SerialBsp_cfg(byPort,&Serial_cfg);
	return 0;
}
Bsp_SerialMgEvent_STR gSerialMgEvent = 
{
	SerialBsp_Init,
	SerialBsp_Open,
	SerialBsp_close,
	SerialBsp_cfg,
	SerialBsp_wirte,
	SerialBsp_read,
	SerialBsp_intHandler,
	SerialBsp_tick,
	SerialBsp_setbaud,
};
