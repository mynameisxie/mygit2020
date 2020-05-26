#ifndef _Bsplcd_H
#define _Bsplcd_H
#include  "types.h"

void OLED_Init_I(void);
void gpiotest(void);
void lcd_init(void);
void PrintfCustom(char *Data_Pointer,uint8_t StartXAddress, uint8_t StartYAddress, uint8_t flag);
void Fill_RAM(unsigned char Data);
#endif
