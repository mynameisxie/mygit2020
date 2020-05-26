#ifndef _BSP_DI_H
#define _BSP_DI_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include  "types.h"

#define _DI_NUM_MAX    (8)

typedef enum 
{ 
	_eDiOk,
	_eDiErr,
	_eDiDoubt
} eDiQualityCode;

#define _DI_ProductionInfo_Addr           (0x00138800)

typedef struct
{
	uint32_t	dwCalibrationPara[_DI_NUM_MAX*2];
}Bsp_DiProductionInfo_STR;

typedef struct
{
	uint8_t byRsv[8];
}Bsp_DiCfg_STR;

typedef struct
{
	uint8_t               byStatus;
	uint8_t 			  byRsv0[7];
	uint8_t 			  byChannlStatus [_DI_NUM_MAX];
}Bsp_DiBackArea_STR;

typedef struct
{
	uint32_t              Para0;
	uint32_t 			  Para1;
}Bsp_DiCalPara_STR;

typedef struct
{
	uint8_t 	          byCfgUpdataStatus;   //0x00: 未更新过组态 0x01: 有新组态需要更新 0x02:已更新 
	uint8_t               byRunMode;           //0x01:标定模式 0x00 : 正常模式
	uint8_t 	          byRsv[2]; 

	uint8_t				  byChannlStatus [_DI_NUM_MAX];
	uint8_t				  byChannlQualityCode [_DI_NUM_MAX];
	uint8_t				  byDiCode [_DI_NUM_MAX];
	Bsp_DiBackArea_STR    *pBackArea;

	Bsp_DiCfg_STR         CfgInf[_DI_NUM_MAX];
    Bsp_DiCfg_STR         CfgInfBack[_DI_NUM_MAX];
	Bsp_DiCalPara_STR     Calibration[_DI_NUM_MAX];
}Bsp_DiMgInfo_STR;


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
}Bsp_DiMgEvent_STR;


extern Bsp_DiMgEvent_STR gDiMgEvent;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
