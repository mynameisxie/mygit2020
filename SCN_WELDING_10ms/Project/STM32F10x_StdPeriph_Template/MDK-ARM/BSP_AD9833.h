#ifndef _Bsp595_H
#define _Bsp595_H

#include  "types.h"

#include "stm32f10x.h"

#define TRI_WAVE 	0  		//输出三角波
#define SIN_WAVE 	1		//输出正弦波
#define SQU_WAVE 	2		//输出方波


void AD9833_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase );
void AD9833_WaveSeting2(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase );
void AD9833_SetFreq(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase );
void AD9833_Init_GPIO(void);
void AD9833_AmpSet(unsigned char amp);




#endif

