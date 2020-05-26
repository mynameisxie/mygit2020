#ifndef _MbToVarTask_h
#define _MbToVarTask_h

#ifdef __cplusplus
extern "c" {
#endif  /* __cplusplus */

#include  "types.h"


extern void  MbToVarTask_Creat(void);
int8_t clrMdsEvent(uint8_t chg);
int8_t setMdsEvent(uint8_t chg, uint8_t event);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
