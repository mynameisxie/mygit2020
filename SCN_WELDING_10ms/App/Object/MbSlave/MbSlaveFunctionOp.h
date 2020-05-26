/*
	2015/07/16 MB公用文件
*/
#ifndef _MbSlaveFunctionOp_H
#define _MbSlaveFunctionOp_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "MbType.h"


#define _coilLen 128
#define _RegLen 160

typedef struct
{
	uint8_t       byFuncCode;
	eMBErrorCode  (*pxHandler) (FrameInf_STR *pFrameInf,uint8_t *byBuf,uint16_t *pwLen);
} xMbSlaveFuncHandler;

typedef struct
{
	uint8_t       byFuncCode;
	eMBErrorCode  (*pxHandler) (FrameInf_STR *pFrameInf,uint8_t *byBuf,uint16_t *pwLen);
} xMbSlaveRespFuncHandler;

extern uint8_t byCoil[_coilLen];
extern uint8_t byReg[_RegLen];

extern int8_t MbXnetReadFrameResp(uint16_t wEventID, int8_t byStatus, uint8_t *byBuf,uint16_t wlen);
extern int8_t MbXnetWirteFrameResp(uint16_t wEventID, int8_t byStatus);


extern xMbSlaveFuncHandler MbSlaveFuncHandler[_MAXFUNCTONCODE];
extern xMbSlaveRespFuncHandler MbSlaveRespFuncHandler[_MAXFUNCTONCODE];


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

