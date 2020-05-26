#include "MbSlaveFunctionOp.h"

uint8_t byCoil[_coilLen];
uint8_t byReg[_RegLen];


#define MB_PDU_FUNC_READ_COILCNT_MAX            ( 0x07D0 )
#define MB_PDU_FUNC_READ_REGCNT_MAX             ( 0x007D )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX        ( 0x0078 )
#define MB_PDU_FUNC_WRITE_MUL_COILCNT_MAX       ( 0x07B0 )

#define _RemoteRespMaxNum                       (100)

static int8_t MbDataBigToLittle(uint8_t *DstbyBuf, uint16_t wLen, uint8_t *SrcbyBuf)
{
	uint16_t i = 0;
	
	for (i = 0; i < (wLen ); i++)
	{
		DstbyBuf[2 * i] = SrcbyBuf[2 * i + 1];
		DstbyBuf[2 * i + 1] = SrcbyBuf[2 * i];
	}

	return 0;
}


eMBErrorCode MbFuncWriteHoldingRegister(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
    eMBErrorCode    eStatus = MB_ENOERR;

    return eStatus;
}

eMBErrorCode MbFuncWriteMultipleHoldingRegister(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
    eMBErrorCode    eStatus = MB_ENOERR;

  return eStatus;
}

eMBErrorCode MbFuncWriteCoil(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
    eMBErrorCode    eStatus = MB_ENOERR;

    return eStatus;
}

eMBErrorCode MbFuncWriteMultipleCoils(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
    eMBErrorCode    eStatus = MB_ENOERR;

    return eStatus;
}

eMBErrorCode MbFuncReadDiscreteInputs(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
	eMBErrorCode    eStatus = MB_ENOERR;

	return eStatus;
}

eMBErrorCode MbFuncReadInputRegister(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
	eMBErrorCode    eStatus = MB_ENOERR;
		
	return eStatus;
}

eMBErrorCode MbFuncReadHoldingRegister(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
	eMBErrorCode    eStatus = MB_ENOERR;

		
	return eStatus;
}

eMBErrorCode MbFuncReadCoils(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
	eMBErrorCode    eStatus = MB_ENOERR;


	return eStatus;
}

eMBErrorCode MbFuncuUserDefined(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
	eMBErrorCode    eStatus = MB_ENOERR;
	return eStatus;
}

int8_t MbBitToByte(uint16_t wStartNo, uint16_t wCount, uint8_t *pbyDst)
{
	uint8_t* bySrc;
	uint16_t i;
	if((wStartNo + wCount)>=sizeof(byCoil))
	{
		return 0;
	}

	bySrc = byCoil + wStartNo;

	for(i=0;i<wCount;i++)
	{
		if( *(bySrc++)!=0 )
		{
			*(pbyDst+i/8) |= 1<<(i%8);	
		}
		else
		{
			*(pbyDst+i/8) &= ~(1<<(i%8));
		}
	}
	return 1;
}

int8_t MbByteToBit(uint16_t wStartNo, uint16_t wCount, uint8_t *pbySrc)
{
	uint8_t* pbyDst;
	uint16_t i;
	
	if((wStartNo + wCount)>=sizeof(byCoil))
	{
		return 0;
	}

	pbyDst = byCoil + wStartNo;

	for(i=0;i<wCount;i++)
	{
		if( *(pbySrc+i/8) & (1<<(i%8)) )
		{
			*(pbyDst++)= 0xFF;
		}
		else
		{
			*(pbyDst++)= 0x00;
		}
	}

	return 1;
}

eMBErrorCode MbFuncRespReadCoils(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
	eMBErrorCode    eStatus = MB_ENOERR;

 	if((pFrameInf->wStart+pFrameInf->wNum) >= sizeof(byCoil))
	{
		byBuf[1]  = 0x80+pFrameInf->byFuncCode;
		byBuf[2]  = MB_EX_ILLEGAL_DATA_ADDRESS;
		*pwLen = 3;
		return MB_ENOREG;
	}	


	byBuf[0]  = pFrameInf->bySlaveID;
	byBuf[1]  = pFrameInf->byFuncCode;
	byBuf[2]  = (pFrameInf->wNum+7)/8;

	MbBitToByte(pFrameInf->wStart,pFrameInf->wNum,byBuf+3);
	
	//memcpy(byBuf+3,byCoil+pFrameInf->wStart,pFrameInf->wNum);
	
	*pwLen = 3+(pFrameInf->wNum+7)/8;
	
	return eStatus;
}

eMBErrorCode MbFuncRespReadDiscreteInputs(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
	eMBErrorCode    eStatus = MB_ENOERR;

 	if((pFrameInf->wStart+pFrameInf->wNum) >= sizeof(byCoil))
	{
		byBuf[1]  = 0x80+pFrameInf->byFuncCode;
		byBuf[2]  = MB_EX_ILLEGAL_DATA_ADDRESS;
		*pwLen = 3;
		return MB_ENOREG;
	}	


	byBuf[0]  = pFrameInf->bySlaveID;
	byBuf[1]  = pFrameInf->byFuncCode;
	byBuf[2]  = (pFrameInf->wNum+7)/8;

//	memcpy(byBuf+3,byCoil+pFrameInf->wStart,pFrameInf->wInputLen);
	MbBitToByte(pFrameInf->wStart,pFrameInf->wNum,byBuf+3);

	*pwLen = 3+(pFrameInf->wNum+7)/8;
	
	return eStatus;
}



eMBErrorCode MbFuncRespReadHoldingRegister(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
	eMBErrorCode    eStatus = MB_ENOERR;

 	if((pFrameInf->wStart+pFrameInf->wNum) >= sizeof(byReg)/2)
	{
		byBuf[1]  = 0x80+pFrameInf->byFuncCode;
		byBuf[2]  = MB_EX_ILLEGAL_DATA_ADDRESS;
		*pwLen = 3;
		return MB_ENOREG;
	}	

	byBuf[0]  = pFrameInf->bySlaveID;
	byBuf[1]  = pFrameInf->byFuncCode;
	byBuf[2]  = pFrameInf->wNum*2;

	MbDataBigToLittle(byBuf+3,pFrameInf->wNum,byReg+pFrameInf->wStart*2); 
	
	*pwLen = 3+pFrameInf->wNum*2;
	
	return eStatus;
}

eMBErrorCode MbFuncRespReadInputRegister(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
	eMBErrorCode    eStatus = MB_ENOERR;
	
 	if((pFrameInf->wStart+pFrameInf->wNum) >= sizeof(byReg)/2)
	{
		byBuf[1]  = 0x80+pFrameInf->byFuncCode;
		byBuf[2]  = MB_EX_ILLEGAL_DATA_ADDRESS;
		*pwLen = 3;
		return MB_ENOREG;
	}	

	byBuf[0]  = pFrameInf->bySlaveID;
	byBuf[1]  = pFrameInf->byFuncCode;
	byBuf[2]  = pFrameInf->wNum*2;

	MbDataBigToLittle(byBuf+3,pFrameInf->wNum,byReg+pFrameInf->wStart*2); 
	
	*pwLen = 3+pFrameInf->wInputLen;
	
	return eStatus;
}

eMBErrorCode MbFuncRespWriteCoil( FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
    eMBErrorCode    eStatus = MB_ENOERR;
	
 	if((pFrameInf->wStart+pFrameInf->wNum) >= sizeof(byCoil))
	{
		byBuf[1]  = 0x80+pFrameInf->byFuncCode;
		byBuf[2]  = MB_EX_ILLEGAL_DATA_ADDRESS;
		*pwLen = 3;
		return MB_ENOREG;
	}	

	if(0x00 != byBuf[4])
	{
		byCoil[pFrameInf->wStart] =  0xFF;
	}
	else
	{
		byCoil[pFrameInf->wStart] =  0x00;
	}

	*pwLen = 6;

	
  return eStatus;
}

eMBErrorCode MbFuncRespWriteHoldingRegister( FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
    eMBErrorCode    eStatus = MB_ENOERR;

 	if((pFrameInf->wStart+pFrameInf->wNum) >= sizeof(byReg)/2)
	{
		byBuf[1]  = 0x80+pFrameInf->byFuncCode;
		byBuf[2]  = MB_EX_ILLEGAL_DATA_ADDRESS;
		*pwLen = 3;
		return MB_ENOREG;
	}	
	
	MbDataBigToLittle(byReg+pFrameInf->wStart*2,pFrameInf->wNum,byBuf+4);

	*pwLen = 6;
	
    return eStatus;
}

eMBErrorCode MbFuncRespWriteMultipleCoils(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
    eMBErrorCode    eStatus = MB_ENOERR;
	uint16_t i;
	uint8_t  *pbySrc = byBuf+7;
	uint8_t * pbyDst;

 	if((pFrameInf->wStart+pFrameInf->wNum) >= sizeof(byCoil))
	{
		byBuf[1]  = 0x80+pFrameInf->byFuncCode;
		byBuf[2]  = MB_EX_ILLEGAL_DATA_ADDRESS;
		*pwLen = 3;
		return MB_ENOREG;
	}

	pbyDst = byCoil + pFrameInf->wStart;

	for(i=0;i<pFrameInf->wNum;i++)
	{
		if( *(pbySrc+i/8) & (1<<(i%8)) )
		{
			*(pbyDst++)= 0xFF;
		}
		else
		{
			*(pbyDst++)= 0x00;
		}
	}

	*pwLen = 6;
    return eStatus;
}

eMBErrorCode MbFuncRespWriteMultipleHoldingRegister(FrameInf_STR *pFrameInf,uint8_t * byBuf,uint16_t *pwLen)
{
    eMBErrorCode    eStatus = MB_ENOERR;
	
 	if((pFrameInf->wStart+pFrameInf->wNum) >= sizeof(byReg)/2)
	{
		byBuf[1]  = 0x80+pFrameInf->byFuncCode;
		byBuf[2]  = MB_EX_ILLEGAL_DATA_ADDRESS;
		*pwLen = 3;
		return MB_ENOREG;
	}	
  
	MbDataBigToLittle(byReg+pFrameInf->wStart*2,pFrameInf->wNum,byBuf+7);
	
	*pwLen = 6;
	
    return eStatus;
}

xMbSlaveFuncHandler MbSlaveFuncHandler[_MAXFUNCTONCODE] = {
	{MB_FUNC_READ_COILS, MbFuncReadCoils},
	{MB_FUNC_READ_DISCRETE_INPUTS, MbFuncReadDiscreteInputs},
	{MB_FUNC_READ_HOLDING_REGISTER, MbFuncReadHoldingRegister},
	{MB_FUNC_READ_INPUT_REGISTER, MbFuncReadInputRegister},
	{MB_FUNC_WRITE_SINGLE_COIL, MbFuncWriteCoil},
	{MB_FUNC_WRITE_REGISTER, MbFuncWriteHoldingRegister},
	{MB_FUNC_WRITE_MULTIPLE_COILS, MbFuncWriteMultipleCoils},
	{MB_FUNC_WRITE_MULTIPLE_REGISTERS, MbFuncWriteMultipleHoldingRegister},
	{MB_FUNC_USER_DEFINE, MbFuncuUserDefined},
};

xMbSlaveRespFuncHandler MbSlaveRespFuncHandler[_MAXFUNCTONCODE] = {
	{MB_FUNC_READ_COILS, MbFuncRespReadCoils},
	{MB_FUNC_READ_DISCRETE_INPUTS, MbFuncRespReadDiscreteInputs},
	{MB_FUNC_READ_HOLDING_REGISTER, MbFuncRespReadHoldingRegister},
	{MB_FUNC_READ_INPUT_REGISTER, MbFuncRespReadInputRegister},
	{MB_FUNC_WRITE_SINGLE_COIL, MbFuncRespWriteCoil},
	{MB_FUNC_WRITE_REGISTER, MbFuncRespWriteHoldingRegister},
	{MB_FUNC_WRITE_MULTIPLE_COILS, MbFuncRespWriteMultipleCoils},
	{MB_FUNC_WRITE_MULTIPLE_REGISTERS, MbFuncRespWriteMultipleHoldingRegister},
	/*{MB_FUNC_USER_DEFINE, MbFuncuUserDefined},*/
};

