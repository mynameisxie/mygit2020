#ifndef _Bsp595_H
#define _Bsp595_H

#include  "types.h"

#include "stm32f10x.h"

#define TRI_WAVE 	0  		//������ǲ�
#define SIN_WAVE 	1		//������Ҳ�
#define SQU_WAVE 	2		//�������


void AD9833_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase );
void AD9833_WaveSeting2(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase );
void AD9833_SetFreq(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase );
void AD9833_Init_GPIO(void);
void AD9833_AmpSet(unsigned char amp);




#endif

