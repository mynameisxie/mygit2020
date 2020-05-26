#include  "MbSlave.h"
#include  "Bsp_Uart.h"


uint8_t byReceBuf[50];

int8_t Mb_SlaveInit(void)
{
	Mb_MemInit();
	return 0;
}

int8_t Mb_SlaveWirte(eMbTypeCode eType,uint16_t wOffset,uint8_t *byBuf, uint16_t wlen)
{
	return Mb_MemWirte(eType, wOffset,byBuf,wlen);
}

int8_t Mb_SlaveRead(eMbTypeCode eType,uint16_t wOffset,uint8_t *byBuf, uint16_t wlen)
{
	return  Mb_MemRead(eType, wOffset,byBuf,wlen);;
}

int8_t Mb_SlaveTask(void)
{

	return 0;
}

int8_t Mb_SlavesetAddr(uint8_t byAddr)
{
	return Mb_ConfigSlaveAddr(byAddr);
}

Bsp_MbSlaveMgEvent_STR gMbSlaveMgEvent = 
{
	Mb_SlaveInit,
	Mb_SlaveWirte,
	Mb_SlaveRead,
	Mb_SlaveTask,
	Mb_SlavesetAddr
};

