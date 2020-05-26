#ifndef _BSP_595_H
#define _BSP_595_H

#include  "types.h"

#include "stm32f10x.h"

#define T_LED                 0
#define T_DTBUE               1
//ÊýÂë¹Ü
#define SHCP_DTUBE_PORT       GPIOB			              
#define SHCP_DTUBE_CLK 	      RCC_APB2Periph_GPIOB		
#define SHCP_DTUBE_PIN	      GPIO_Pin_11			       
 
#define STCP_DTUBE_PORT       GPIOB			              
#define STCP_DTUBE_CLK 	      RCC_APB2Periph_GPIOB		
#define STCP_DTUBE_PIN	      GPIO_Pin_10			        
 
#define DS_DTUBE_PORT         GPIOB			              
#define DS_DTUBE_CLK 	      RCC_APB2Periph_GPIOB		
#define DS_DTUBE_PIN	      GPIO_Pin_9
 
#define DTUBE_SHCP_Low()      GPIO_ResetBits( SHCP_DTUBE_PORT, SHCP_DTUBE_PIN )
#define DTUBE_SHCP_High()     GPIO_SetBits( SHCP_DTUBE_PORT, SHCP_DTUBE_PIN )
#define DTUBE_STCP_Low()      GPIO_ResetBits( STCP_DTUBE_PORT, STCP_DTUBE_PIN )
#define DTUBE_STCP_High()     GPIO_SetBits( STCP_DTUBE_PORT, STCP_DTUBE_PIN )
#define DTUBE_Data_Low()      GPIO_ResetBits( DS_DTUBE_PORT, DS_DTUBE_PIN )
#define DTUBE_Data_High()     GPIO_SetBits( DS_DTUBE_PORT, DS_DTUBE_PIN )
//LEDµÆ
#define SHCP_LED_PORT         GPIOB			              
#define SHCP_LED_CLK 	      RCC_APB2Periph_GPIOB		
#define SHCP_LED_PIN	      GPIO_Pin_8			       
 
#define STCP_LED_PORT         GPIOB			              
#define STCP_LED_CLK 	      RCC_APB2Periph_GPIOB		
#define STCP_LED_PIN	      GPIO_Pin_7			        
 
#define DS_LED_PORT           GPIOB			              
#define DS_LED_CLK 	          RCC_APB2Periph_GPIOB		
#define DS_LED_PIN	          GPIO_Pin_6	
 
#define LED_SHCP_Low()        GPIO_ResetBits( SHCP_LED_PORT, SHCP_LED_PIN )
#define LED_SHCP_High()       GPIO_SetBits( SHCP_LED_PORT, SHCP_LED_PIN )
#define LED_STCP_Low()        GPIO_ResetBits( STCP_LED_PORT, STCP_LED_PIN )
#define LED_STCP_High()       GPIO_SetBits( STCP_LED_PORT, STCP_LED_PIN )
#define LED_Data_Low()        GPIO_ResetBits( DS_LED_PORT, DS_LED_PIN )
#define LED_Data_High()       GPIO_SetBits( DS_LED_PORT, DS_LED_PIN )
 
 
#define SET_ALARM GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define CLR_ALARM GPIO_ResetBits(GPIOB, GPIO_Pin_12)

void HC595_GPIO_Config(void);
void HC595_Send_Byte(uint8_t byte,uint8_t type);
void HC595_CS(uint8_t type);
void HC595_Send_Multi_Byte(uint8_t *data, uint8_t len,uint8_t type);

#endif
