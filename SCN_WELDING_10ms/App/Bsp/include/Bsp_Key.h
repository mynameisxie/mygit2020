#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include  "types.h"

typedef struct
{
	int8_t 	  (*init)(void);
	int8_t    (*tick)(uint8_t byTick);
	uint8_t   (*read)(void);
	uint8_t   Value;
	uint8_t   KeyOld;
	uint8_t   flag;
}Bsp_KeyMgEvent_STR;

extern Bsp_KeyMgEvent_STR gKeyMgEvent;

void delaynop(uint8_t n);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
