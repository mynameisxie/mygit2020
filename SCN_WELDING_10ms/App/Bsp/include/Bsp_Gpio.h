#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include  "types.h"

unsigned char DS2431_ReadRom( unsigned char *id );
void Delay_Nus(uint32_t Nus);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
