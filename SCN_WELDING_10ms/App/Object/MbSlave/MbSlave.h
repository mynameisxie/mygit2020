#ifndef _MB_SLAVE_H
#define _MB_SLAVE_H
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include  "types.h"
#include  "MbLib.h"	
//#include "MbType.h"

#define MB_FUNC_READ_COILS                    (  1 )
#define MB_FUNC_READ_DISCRETE_INPUTS          (  2 )
#define MB_FUNC_READ_HOLDING_REGISTER         (  3 )
#define MB_FUNC_READ_INPUT_REGISTER           (  4 )


typedef struct
{
	int8_t    (*Init)(void);    
	int8_t    (*write)(eMbTypeCode eType,uint16_t wOffset,uint8_t *byBuf, uint16_t wlen);
	int8_t    (*read)(eMbTypeCode eType,uint16_t wOffset,uint8_t *byBuf, uint16_t wlen);
	int8_t    (*Tick)(void);
	int8_t    (*setAddr)(uint8_t byAddr);
}Bsp_MbSlaveMgEvent_STR;


extern Bsp_MbSlaveMgEvent_STR gMbSlaveMgEvent;

extern int8_t Mb_SlaveTask(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

