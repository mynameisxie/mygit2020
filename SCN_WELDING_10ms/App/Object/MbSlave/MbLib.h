#ifndef _MBLIB_H
#define _MBLIB_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "MbType.h"

typedef enum 
{ 
	_eCOILS,
	_eDISCRETE_INPUTS,
	_eHOLDING_REGISTER,
	_eINPUT_REGISTER
} eMbTypeCode;

extern int8_t Mb_MemInit(void);
extern int8_t Mb_MemWirte(eMbTypeCode eType,uint16_t wOffset,uint8_t *byBuf, uint16_t wlen);
extern int8_t Mb_MemRead(eMbTypeCode eType,uint16_t wOffset,uint8_t *byBuf, uint16_t wlen);
extern int8_t Mb_ConfigSlaveAddr(uint8_t byAddr);
extern int8_t Mb_ProtocolOp(eMbCmdType eType,uint8_t byID,uint8_t * byBuf,uint16_t *pwLen);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

