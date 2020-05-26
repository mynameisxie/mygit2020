#ifndef _BspFlash_H
#define _BspFlash_H

#include  "types.h"
typedef struct
{
	uint32_t Hfreq;//频率上限
	uint32_t Lfreq;//频率下限
	uint32_t MaxPwmDyty;//最大振幅 20~100
	uint32_t softStartTim;//软启动参数 0-999
	uint32_t scanIntv;//扫频间隔频率 1-999
	uint32_t triggeTime;//强制触发时间，0表示接触触发，0~99.9 单位S
	uint32_t MaxP;
}SysConfig_t;

extern SysConfig_t SysConfig;//系统参数
extern SysConfig_t DispPara;//显示参数
uint8_t flash_write(uint32_t StartAddr,uint32_t *p_data,uint32_t size);
int flash_read(uint32_t StartAddr,uint32_t *p_data,uint32_t size);
uint8_t falshCheck();
#endif
