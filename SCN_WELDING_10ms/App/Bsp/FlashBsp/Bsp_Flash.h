#ifndef _BspFlash_H
#define _BspFlash_H

#include  "types.h"
typedef struct
{
	uint32_t Hfreq;//Ƶ������
	uint32_t Lfreq;//Ƶ������
	uint32_t MaxPwmDyty;//������ 20~100
	uint32_t softStartTim;//���������� 0-999
	uint32_t scanIntv;//ɨƵ���Ƶ�� 1-999
	uint32_t triggeTime;//ǿ�ƴ���ʱ�䣬0��ʾ�Ӵ�������0~99.9 ��λS
	uint32_t MaxP;
}SysConfig_t;

extern SysConfig_t SysConfig;//ϵͳ����
extern SysConfig_t DispPara;//��ʾ����
uint8_t flash_write(uint32_t StartAddr,uint32_t *p_data,uint32_t size);
int flash_read(uint32_t StartAddr,uint32_t *p_data,uint32_t size);
uint8_t falshCheck();
#endif
