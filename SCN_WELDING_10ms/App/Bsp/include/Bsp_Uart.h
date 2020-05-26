#ifndef _BSP_UART_H
#define _BSP_UART_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#include  "types.h"

#define _BUFs  ( 50 )

#define  SERIAL_BAUDRATE_110                             110
#define  SERIAL_BAUDRATE_300                             300
#define  SERIAL_BAUDRATE_1200                           1200
#define  SERIAL_BAUDRATE_2400                           2400
#define  SERIAL_BAUDRATE_4800                           4800
#define  SERIAL_BAUDRATE_9600                          9600//6040ÊÇ9600¡¢342ÊÇ31300
#define  SERIAL_BAUDRATE_19200                         19200
#define  SERIAL_BAUDRATE_38400                         38400
#define  SERIAL_BAUDRATE_57600                         31300
#define  SERIAL_BAUDRATE_115200                       115200
#define  SERIAL_BAUDRATE_230400                       230400
#define  SERIAL_BAUDRATE_460800                       460800
#define  SERIAL_BAUDRATE_921600                       921600

typedef enum 
{ 
	_eIdle,
	_eWaiteRece,
	_eSending,
	_eReceing,
	_eCompleteRece
} eReceModeCode;

typedef enum 
{ 
	_eTxIdle,
	_eTxWaiteRece,
	_eTxCompleteRece
} eTxModeCode;

typedef enum 
{ 
	_eNO_PARITY, 
	_eODD_PARITY, 
	_eEVEN_PARITY, 
	_eMARK_PARITY, 
	_eSPACE_PARITY 
} eSerParity;

typedef enum 
{ 
	_eSTOP_1, 
	_eSTOP_1_5, 
	_eSTOP_2 
} eSerStopBits;

typedef enum 
{ 
	_eBITS_5, 
	_eBITS_6, 
	_eBITS_7, 
	_eBITS_8 
} eSerDataBits;

typedef enum 
{ 
	_eB50,		
	_eB75,		
	_eB110,		
	_eB134,		
	_eB150,    
	_eB200,
	_eB300,		
	_eB600,		
	_eB1200,	
	_eB1800,	
	_eB2400,   
	_eB4800,
	_eB9600,		
	_eB19200,	
	_eB38400,	
	_eB57600,	
	_eB115200,
	_eB230400,
	_eB460800
} eSerBaud;


typedef struct
{
	eSerBaud     eBaud;
	eSerDataBits eDataBits;
	eSerParity   eParity;
	eSerStopBits eStopBits;
	uint8_t      byRsv[3];
}Ser_cfg_STR;


typedef struct
{
	uint8_t  byID;
	eReceModeCode eReceMode;
	eTxModeCode   eTxMode;
	uint8_t  byTxDelayCnt;
	uint32_t dwFifoRxCnt;
	uint32_t dwFifoTxCnt;
	uint32_t dwRxCnt;
	uint32_t dwTxCnt;
	uint32_t dwRxTo;
	uint8_t  byTxBuf[_BUFs];
	uint8_t  byRxBuf[_BUFs];
}BSP_SerialCfg_STR;

typedef struct
{
	int8_t 	  (*init)(uint16_t byPort);
	int8_t    (*open)(uint16_t byPort);
	int8_t    (*close)(uint16_t byPort);
	int8_t    (*cfg)(uint16_t byPort,void *pcfg);
	int8_t    (*wirte)(uint16_t byPort,uint8_t *byBuf, uint16_t wlen);
	int8_t    (*read)(uint16_t byPort,uint8_t *byBuf,uint16_t *wlen);
	int8_t    (*intHandler)(uint16_t byPort);
	int8_t    (*tick)(uint8_t byTick);
	int8_t    (*setbaud)(uint16_t byPort,uint8_t ebaud);
}Bsp_SerialMgEvent_STR;
 
extern void TIM3_PWM_Init(uint16_t arr,uint16_t psc);
extern void TIM3_PWM_ToGPIO_Init();
extern Bsp_SerialMgEvent_STR gSerialMgEvent;
extern uint16_t gRate;
extern uint16_t gfrequency;
extern int8_t TIM3_PWM_Open();
extern int8_t TIM3_PWM_Close();
extern void SetPwmDuty(uint16_t duty);
extern uint16_t displayDuty; 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
