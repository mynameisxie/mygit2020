#ifndef _CRC16_H
#define _CRC16_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "types.h"

extern uint16_t CRC16(uint8_t * pchMsg, uint16_t wDataLen);
extern uint16_t CRC16_Sum(uint8_t *pchMsg, uint16_t wDataLen, uint16_t wCRC16);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
