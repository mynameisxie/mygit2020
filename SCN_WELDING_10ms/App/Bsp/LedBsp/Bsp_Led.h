#ifndef _BspLED_H
#define _BspLED_H

#include  "types.h"

#define RUN   0x01
#define STATE 0x02
#define ERR   0x03 

typedef struct
{
	void    (*init)(void);
	void    (*set)(uint8_t led);
	void    (*clr)(uint8_t led);

}Bsp_Led_STR;

extern Bsp_Led_STR gLedEvet;
#endif
