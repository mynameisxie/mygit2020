#ifndef _BSP_DO_H
#define _BSP_DO_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#include  "types.h"

#define _DO_NUM_MAX    (8)

typedef enum 
{ 
	_eDoOk,
	_eDoErr,
	_eDoDoubt
} eDoQualityCode;

typedef struct
{
	uint8_t byFaultsafetyFlag;
	uint8_t	byFaultsafetyData;
	uint8_t byRsv[6];
}Bsp_DoCfg_STR;

typedef struct
{
	uint8_t               byStatus;
	uint8_t 			  byRsv0[7];
	uint8_t 			  byDoCode [_DO_NUM_MAX];
}Bsp_DoBackArea_STR;

typedef struct
{
	uint8_t 	          byCfgUpdataStatus;   //0x00: 未更新过组态 0x01: 有新组态需要更新 0x02:已更新 
	uint8_t               byRunMode;           //0x01:标定模式 0x00 : 正常模式
	uint8_t 	          byRsv[2]; 
	
	uint8_t				  byChannlStatus [_DO_NUM_MAX];
	uint8_t				  byChannlQualityCode [_DO_NUM_MAX];
	uint8_t				  byDoCode [_DO_NUM_MAX];
	
	Bsp_DoBackArea_STR    *pBackArea;

	Bsp_DoCfg_STR         CfgInf[_DO_NUM_MAX];
	Bsp_DoCfg_STR         CfgInfBack[_DO_NUM_MAX];
}Bsp_DoMgInfo_STR;

typedef struct
{
	int8_t    (*Init)(void);
	int8_t    (*open)(void);
	int8_t    (*close)(void);
	int8_t    (*ioctl)(void);
	int8_t    (*wirte)(uint8_t *byBuf, uint16_t wlen);
	int8_t    (*read)(uint8_t *byBuf);
	int8_t    (*Tick)(uint8_t byTick);
	int8_t    (*cfgAnalysis)(uint8_t *buf);
	int8_t    (*getChnStatus)(uint8_t *byBuf);
}Bsp_DoMgEvent_STR;


extern Bsp_DoMgEvent_STR gDoMgEvent;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
