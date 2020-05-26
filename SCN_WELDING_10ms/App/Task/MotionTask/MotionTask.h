#ifndef _MotionTask_H
#define _MotionTask_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
#include  "types.h"
//系统组态存储地址
#define SYSTEM_CONFIG_ADDR  0x800E000
#define SYSTEM_CONFIG_ADDR2  0x800E400
//控制器类型

#define NORMAL_IO      1 
#define DOU_DIRBLOWER  2
#define SGL_DIRBLOWER  3
#define WIND_PUSHER    4
//设备号
#define DRIVER0   0    
#define DRIVER1   1  
#define DRIVER2   2  
#define DRIVER3   3 
//动作任务运行周期
#define	MOTION_TASK_CYCLE  10
	
//门窗开关超时时间
#define MOTION_TIME_OUT 	 30000/MOTION_TASK_CYCLE
//门窗动作切换暂停时间
#define MOTION_TIME_PAUSE  3000/MOTION_TASK_CYCLE
//门窗动作切换暂停时间
#define BLOWER_TIME_PAUSE  5000/MOTION_TASK_CYCLE
//DO输出回检时间
#define CHECK_BACK  2000/MOTION_TASK_CYCLE
//通信标识

typedef struct
{
	uint32_t H;
	uint32_t baud;
	uint32_t type;
	uint32_t rback;
	uint32_t tmout;
	uint32_t crc;
}SysCfg_STR;

typedef struct 
{
	uint8_t from;
	uint8_t event;
}Command_STR;

typedef struct
{
	uint8_t flag;
	uint8_t cmd;
	
}LcdKeyE_STR;

typedef struct
{
	uint8_t flag;
	uint8_t cmd;
	
}DiKeyE_STR;

typedef struct
{
	uint8_t flag;
	uint8_t cmd;
	
}ModbusE_STR;
typedef struct
{
	uint8_t evt[4];
	LcdKeyE_STR *lcdevt;
	DiKeyE_STR  *dikevt;
	ModbusE_STR *mdsevt;
}SYSMSG_STR;

typedef struct
{
	SysCfg_STR *Sys;//控制器类型
	uint8_t pre_st[4];//上一个状态
	uint8_t state[4];//当前状态
	uint32_t Mcnt[4];//超时计时
	uint32_t Pcnt[4];//超时计时
	SYSMSG_STR *msg;
}CtrCfg_STR;

typedef struct
{
	uint16_t pusher[2];
	uint16_t dblower[2];
	uint16_t sblower[4];
	uint16_t IOctrl[4];
}CtrlErr_STR;


#ifdef Motion_Task 
DiKeyE_STR  DiKeyE[4];
LcdKeyE_STR LcdKeyE[4];
ModbusE_STR ModbusE[4];
CtrlErr_STR CtrlErr;
uint8_t	com_flag;
uint8_t com_cnt;
uint8_t	flashErr;
uint8_t AutoHandFlag = 0;
SysCfg_STR  SysCfg ={
	1,   //设备地址1
	4,//波特率0=115200,1=57600,2=38400,3=19200,4=9600
	0x04,//门窗推杆控制器
	0x00,//无反馈
	30,//超时时间0-200S
	39,//crc = 1+4+4+0+30
};
SYSMSG_STR  SysMsg ={
{0},
LcdKeyE,
DiKeyE,
ModbusE,
};
CtrCfg_STR Controller={
&SysCfg,
{1,1,1,1},
{1,1,1,1},
{0,0},
{0,0},
&SysMsg
};
char *version = "V01.00.00.00  ";//软件程序版本
#else
extern SysCfg_STR  SysCfg;
extern CtrCfg_STR Controller;
extern DiKeyE_STR  DiKeyE[4];
extern LcdKeyE_STR LcdKeyE[4];
extern ModbusE_STR ModbusE[4];
extern SYSMSG_STR  SysMsg;
extern CtrlErr_STR CtrlErr;
extern uint8_t	com_flag;
extern uint8_t com_cnt;
extern uint8_t	flashErr;
extern char *version;//软件程序版本
extern uint8_t AutoHandFlag;
#endif

void  MotionTask_Creat(void);
void GetEvtFromMds(ModbusE_STR *event);
void GetEvtFromHMI(LcdKeyE_STR *event);
void GetEvtFromDik(DiKeyE_STR *event);
void ModeSwitch(void);
void getValidMsg(SYSMSG_STR *pmsg);
void clrSysMsg(SYSMSG_STR *pmsg);
uint8_t getContrState(uint8_t nchan);
void checkSysStatus(CtrCfg_STR *pCtrl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
