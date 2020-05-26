#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include  "types.h"
 
#define DTECTOR   1
typedef struct
{
	int8_t    (*Init)(void);
	uint16_t    (*read)(uint8_t ch,uint8_t n);
}Bsp_ADCEvent_STR;

uint16_t getRealAdcValue(uint8_t channel);
extern Bsp_ADCEvent_STR gAdcMgEvent;
int AdcCheck(void);
void Init_ADC1(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
