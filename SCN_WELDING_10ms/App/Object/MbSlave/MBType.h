/*
	2015/07/16 MB公用文件
*/
#ifndef _MB_Type_H
#define _MB_Type_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "types.h"
//#include "MbCfg.h"

#define _BUFLEN         (256)

#define _MaxPortNum     (8)
#define _MaxDevNum      (_MaxPortNum * 16)
#define _MaxCmdNum      (_MaxDevNum * 4)

#define _MaxTcpPortNum     (16)
#define _MaxTcpLinkNum     (_MaxTcpPortNum * 16)
#define _MaxTcpCmdNum      (_MaxDevNum * 4)


#define _MaxTableNum    (1024)

#define _MaxIzone       (4096)
#define _MaxQzone       (4096)

#define _EilencePeriodTime   (1000)
#define _OverflowTime        (4000000000u)

typedef struct
{	       
	uint8_t  bySlaveID;        
	uint8_t  byFuncCode;  
	uint8_t  byErrCode;
	uint8_t  byRsv;
	uint16_t wStart;          
	uint16_t wNum; 
	uint16_t wID; 
	uint8_t *pbyInputAddr;
	uint8_t *pbyOutnputAddr;
	uint16_t wDataCnt;
	uint16_t wInputLen;
	uint16_t wOutnputLen;
	uint16_t wData;
	uint8_t  bybuf[8];
}FrameInf_STR;

typedef struct
{	      
	uint8_t  bySlaveID;	
	uint8_t  byFuncCode;  
	uint8_t  byErrCode;
	uint8_t  byRsv;
	uint16_t wStart;          
	uint16_t wNum; 
	uint16_t wID; 
	uint16_t wEventId;
	uint16_t wData;
	uint8_t  bybuf[8];
}SlaveRemoteRespInf_STR;

typedef struct
{	     
	uint8_t  bySlaveID;        
	uint8_t  byFuncCode;  
	uint16_t wStart;          
	uint16_t wNum; 
}MbCmdFrameInf_STR;

#define MB_FUNC_NONE                          (  0 )

#define MB_FUNC_READ_COILS                    (  1 )
#define MB_FUNC_READ_DISCRETE_INPUTS          (  2 )
#define MB_FUNC_READ_HOLDING_REGISTER         (  3 )
#define MB_FUNC_READ_INPUT_REGISTER           (  4 )
#define MB_FUNC_WRITE_REGISTER                (  6 )
#define MB_FUNC_WRITE_SINGLE_COIL             (  5 )
#define MB_FUNC_WRITE_MULTIPLE_COILS          ( 15 )
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      ( 16 )

#define MB_FUNC_USER_DEFINE     			  ( 32 )

#define MB_FUNC_ERROR                         ( 128 )

#define _MAXFUNCTONCODE       				  ( 9 )


typedef enum
{
    MB_EX_NONE = 0x00,
    MB_EX_ILLEGAL_FUNCTION = 0x01,
    MB_EX_ILLEGAL_DATA_ADDRESS = 0x02,
    MB_EX_ILLEGAL_DATA_VALUE = 0x03,
    MB_EX_SLAVE_DEVICE_FAILURE = 0x04,
    MB_EX_ACKNOWLEDGE = 0x05,
    MB_EX_SLAVE_BUSY = 0x06,
    MB_EX_MEMORY_PARITY_ERROR = 0x08,
    MB_EX_GATEWAY_PATH_FAILED = 0x0A,
    MB_EX_GATEWAY_TGT_FAILED = 0x0B
} eMBException;

typedef enum
{
    MB_ENOERR,                  /*!< no error. */
    MB_ENOREG,                  /*!< illegal register address. */
    MB_EINVAL,                  /*!< illegal argument. */
    MB_EPORTERR,                /*!< porting layer error. */
    MB_ENORES,                  /*!< insufficient resources. */
    MB_EIO,                     /*!< I/O error. */
    MB_EILLSTATE,               /*!< protocol stack in illegal state. */
    MB_ETIMEDOUT,                /*!< timeout error occurred. */
    MB_CRCERR,
    MB_FUNCERR,
    MB_NONEFUNC,
    MB_FRAMEERR,
    MB_SLAVEIDERR
} eMBErrorCode;



typedef enum 
{ 
	_eRs232, 
	_eRs485 
} eSerMode;


typedef enum 
{ 
	_ePoll, 
	_eSlave,
	_eXnet 
} ePortType;

typedef enum 
{ 
	_ePortStop,
	_ePortStart
} ePortStartType;

typedef enum 
{ 
	_eClient,
	_eServer
} eLinkType;

typedef enum 
{ 
	_eNoneCfg, 
	_eReFreshing, 
	_eReFresh,
	_eReFreshed
} eCfgFresh;

typedef enum 
{ 
	_eInit, 
	_eRun, 
	_eStop 
} eMbWorkMode;


typedef enum
{
	_QUERY_VALIDCMD,
    _FRAME_SENDING,
	_FRAME_WAITRECE, 
    _FRAME_RECEOK,  
    _WAIT_IntervalTime
} eMBEventStatus;

typedef enum 
{ 
	_eMbTcpClient, 
	_eMbTcpServer, 
	_eMbRtu 
} eMbCmdType;

typedef enum 
{ 
	_eWork, 
	_eClose,
	_eDebuging,
	_eListening
} eBspMode;

typedef enum 
{ 
	_eCmdNon, 
	_eCmdErr, 
	_eCmdOk
} eMbCmdStatus;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

