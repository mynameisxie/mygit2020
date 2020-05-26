#ifndef _IOTask_H
#define _IOTask_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include  "types.h"

#define SET_NPN_HIGH GPIO_SetBits(GPIOA, GPIO_Pin_6);
#define CLR_NPN_HIGH GPIO_ResetBits(GPIOA, GPIO_Pin_6);
#define SET_CMP_HIGH GPIO_SetBits(GPIOA, GPIO_Pin_5);
#define CLR_CMP_HIGH GPIO_ResetBits(GPIOA, GPIO_Pin_5);
extern uint8_t test_flag;
extern uint8_t SystemState;
extern uint8_t isTouchPushed();
extern void  TouchTask_Creat(void);
extern void SetBuzzerSoundTimes(uint8_t times);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
