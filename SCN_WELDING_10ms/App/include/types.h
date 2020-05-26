#ifndef _TYPES_H
#define _TYPES_H

#include <string.h>

#ifndef _BOOL_
#define _BOOL_
 typedef enum {FALSE = 0, TRUE = 1} Bool;		 
#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif

typedef   signed char			int8_t;
typedef   signed short      int int16_t;
typedef   signed            int int32_t;
typedef   signed long long	int int64_t;


typedef unsigned char			uint8_t;
typedef unsigned short      int uint16_t;
typedef unsigned            int uint32_t;
typedef unsigned long long	int uint64_t;


//typedef enum {RESET = 0, SET = !RESET} FlagStatus, IntStatus, SetState;
//#define PARAM_SETSTATE(State) ((State==RESET) || (State==SET))


//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
//#define PARAM_FUNCTIONALSTATE(State) ((State==DISABLE) || (State==ENABLE))

//#ifndef Status
// typedef enum {ERROR = 0, SUCCESS = !ERROR} Status;
//#endif

typedef   signed char BOOL;


#if defined   (__CC_ARM)      /* ARM Compiler */
	#define __ALIGN_BEGIN(n)    __align(n)
#elif defined (__ICCARM__)    /* IAR Compiler */
	#define __ALIGN_BEGIN(n)
#else
	#define __ALIGN_BEGIN(n)
#endif

#endif
