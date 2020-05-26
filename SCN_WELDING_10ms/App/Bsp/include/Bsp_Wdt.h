#ifndef _BSP_WDT_H
#define _BSP_WDT_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include  "types.h"


typedef struct
{
	int8_t 	  (*init)(void);
	int8_t    (*free)(void);
}Bsp_WdtMgEvent_STR;

extern Bsp_WdtMgEvent_STR gWdtMgEvent;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
