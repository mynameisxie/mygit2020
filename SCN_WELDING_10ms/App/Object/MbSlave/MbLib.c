#include  "MbLib.h"
#include  "crc16.h"
#include  "MbSlaveFunctionOp.h"
#include  "Bsp_Uart.h"
#include  "Bsp_Led.h"
#include  "MotionTask.h"
uint8_t g_SlaveAddr = 1;

int8_t Mb_MemInit(void)
{
	memset(byCoil,0x00,_coilLen);
	memset(byReg,0x00,_RegLen);	
	return 0;	
}

int8_t Mb_MemWirte(eMbTypeCode eType,uint16_t wOffset,uint8_t *byBuf, uint16_t wlen)
{
	if((_eCOILS == eType) || (_eDISCRETE_INPUTS == eType))
	{
		if((wOffset+wlen) > _coilLen)
		{
			return -1;
		}

		memcpy(byCoil+wOffset,byBuf,wlen);
		
		return 0;
	}
	else if((_eHOLDING_REGISTER == eType) || (_eINPUT_REGISTER == eType))
	{
		if((wOffset+wlen*2) > _RegLen)
		{
			return -1;
		}

		memcpy(byReg+wOffset,byBuf,wlen*2);
		
		return 0;

	}
	else
	{
		return -1;
	}
}

int8_t Mb_MemRead(eMbTypeCode eType,uint16_t wOffset,uint8_t *byBuf, uint16_t wlen)
{
	if((_eCOILS == eType) || (_eDISCRETE_INPUTS == eType))
	{
		if((wOffset+wlen) > _coilLen)
		{
			return -1;
		}

		memcpy(byBuf,byCoil+wOffset,wlen);
		
		return 0;
	}
	else if((_eHOLDING_REGISTER == eType) || (_eINPUT_REGISTER == eType))
	{
		if((wOffset+wlen*2) > _RegLen)
		{
			return -1;
		}

		memcpy(byBuf,byReg+wOffset,wlen*2);
		
		return 0;

	}
	else
	{
		return -1;
	}

}

int8_t Mb_ConfigSlaveAddr(uint8_t byAddr)
{
	g_SlaveAddr = byAddr;
	return 0;
}


static int8_t Mb_IsSendToMyself(uint8_t byID,uint8_t * byBuf,uint16_t wLen)
{
	if(byBuf[0] == g_SlaveAddr)
	{
		return 0;
	}

	return -1;
}

int8_t Mb_SendingFrameOp(uint8_t *byBuf, uint16_t wLen)
{
	gSerialMgEvent.wirte(2,byBuf,wLen);
	
	return 0;
}

static int8_t Mb_IsCrcCheckOk(uint8_t byID,uint8_t * byBuf,uint16_t wLen)
{
	uint16_t wCalculatedCRCValue = 0;
	uint8_t CRCValueHigh = 0;
	uint8_t CRCValueLow  = 0;
	
	wCalculatedCRCValue = CRC16((uint8_t*)byBuf, (uint16_t)(wLen - 2));//生成校验码
	
	CRCValueHigh = *(byBuf + wLen - 1);
	CRCValueLow = *(byBuf + wLen-2);
	
	if (wCalculatedCRCValue != ((CRCValueHigh<<8)+CRCValueLow))
	{
		return -1;
	}

	return 0;
}

static int8_t Mb_IsFuncVaildCheckOk(uint8_t * byBuf,uint16_t wLen)
{
 	if((MB_FUNC_READ_COILS  ==  byBuf[1])
		|| (MB_FUNC_READ_DISCRETE_INPUTS  ==  byBuf[1])
		|| (MB_FUNC_READ_HOLDING_REGISTER  ==  byBuf[1])
//		|| (MB_FUNC_READ_INPUT_REGISTER  ==  byBuf[1])
		|| (MB_FUNC_WRITE_SINGLE_COIL  ==  byBuf[1])
		|| (MB_FUNC_WRITE_REGISTER  ==  byBuf[1])
		|| (MB_FUNC_WRITE_MULTIPLE_COILS  ==  byBuf[1])
		|| (MB_FUNC_WRITE_MULTIPLE_REGISTERS  ==  byBuf[1])
		)
 	{
		return 0;
 	}
	return -1;
}

static int8_t Mb_CreatErrRespFrame(eMBErrorCode eMBError,uint8_t * byBuf,uint16_t *pwLen)
{
	uint16_t wCalculatedCRCValue = 0;

	*(byBuf + 0) = byBuf[0];
    *(byBuf + 1) = byBuf[1] + 0x80;
    *(byBuf + 2) = eMBError;
    *pwLen = 3;
	
	wCalculatedCRCValue   = CRC16(byBuf, *pwLen); 	//生成校验码
	*(byBuf + *pwLen)     = (uint8_t)(wCalculatedCRCValue);			//CRC检验码低位
	*(byBuf + *pwLen + 1) = (uint8_t)(wCalculatedCRCValue>>8);	//CRC检验码高位
	*pwLen += 2;
		
	return 0;
}

static int8_t Mb_GetFrameInf(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
	pFrameInf->bySlaveID  = (*(byBuf + 0));
	pFrameInf->byFuncCode = (*(byBuf + 1));
	pFrameInf->wStart     = ((*(byBuf + 2))<<8) + (*(byBuf + 3));
	
	if ((MB_FUNC_WRITE_SINGLE_COIL == pFrameInf->byFuncCode) || (MB_FUNC_WRITE_REGISTER == pFrameInf->byFuncCode))
	{
		pFrameInf->wNum = 1;
	}
	else
	{
		pFrameInf->wNum = ((*(byBuf + 4))<<8) + (*(byBuf + 5));              //个数
	}

	return 0;
}

static int8_t Mb_InserCrcCode(uint8_t * byBuf,uint16_t *pwLen)
{
	uint16_t wReapLen = *pwLen;
	uint16_t wCalculatedCRCValue = 0;
	
	wCalculatedCRCValue   = CRC16(byBuf, wReapLen);			//生成校验码
	*(byBuf + wReapLen + 0)   = (uint8_t)(wCalculatedCRCValue); //CRC检验码低位
	*(byBuf + wReapLen + 1)   = (uint8_t)(wCalculatedCRCValue>>8);	//CRC检验码高位

	*pwLen += 2;
	
	return 0;
}

static int8_t Mb_ProtocolStackOp(uint8_t byID,uint8_t * byBuf,uint16_t *pwLen)
{
	uint8_t i = 0;
	FrameInf_STR sFrameInf;
	
	sFrameInf.wID = byID;

	Mb_GetFrameInf(&sFrameInf,byBuf,pwLen);

	memcpy(sFrameInf.bybuf,byBuf,6);  //作为tcp客户端使用时，保留通信头信息
	
	for(i = 0; i < _MAXFUNCTONCODE; i++)
	{
		if(MbSlaveRespFuncHandler[i].byFuncCode == sFrameInf.byFuncCode)
		{
			MbSlaveRespFuncHandler[i].pxHandler(&sFrameInf,byBuf,pwLen);

			Mb_InserCrcCode(byBuf,pwLen);

			return 0;
		}
	}
		
	return -1;
}

int8_t Mb_ProtocolOp(eMbCmdType eType,uint8_t byID,uint8_t * byBuf,uint16_t *pwLen)
{
	if(0 != Mb_IsSendToMyself(byID,byBuf,*pwLen))  //判断是否发给本机
	{
		return -1;
	}

	if(0 != Mb_IsCrcCheckOk(byID,byBuf,*pwLen))   //数据有效性检查
	{
		return -2;
	}

	if(0 != Mb_IsFuncVaildCheckOk(byBuf,*pwLen))  //是否支持该命令
	{
		Mb_CreatErrRespFrame(MB_FUNCERR,byBuf,pwLen);
		
		Mb_SendingFrameOp(byBuf,*pwLen);
		
		return -3;
	}
	
	if(0 != Mb_ProtocolStackOp(byID,byBuf,pwLen))  //协议栈处理
	{
		return -4;
	}
	if(byID > 0)
	{
		Mb_SendingFrameOp(byBuf,*pwLen);
	}

	return 0;
}

