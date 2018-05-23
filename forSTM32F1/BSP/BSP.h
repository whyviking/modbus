#ifndef __BSP_H__
#define __BSP_H__

#include "stm32f10x_gpio.h"

#define TEXT                  0


#define LED1_PIN              GPIO_Pin_15
#define LED1_PORT             GPIOA

#define LED2_PIN              GPIO_Pin_8
#define LED2_PORT             GPIOB

#define LED1_ON               GPIO_ResetBits(LED1_PORT, LED1_PIN)
#define LED1_OFF              GPIO_SetBits(LED1_PORT, LED1_PIN)

#define LED2_ON               GPIO_ResetBits(LED2_PORT, LED2_PIN)
#define LED2_OFF              GPIO_SetBits(LED2_PORT, LED2_PIN)

#define USART1_RX_PIN         GPIO_Pin_10
#define USART1_TX_PIN         GPIO_Pin_9
#define USART1_PORT           GPIOA

#define USART2_RX_PIN         GPIO_Pin_3
#define USART2_TX_PIN         GPIO_Pin_2
#define USART2_PORT           GPIOA

#define USART3_RX_PIN         GPIO_Pin_11
#define USART3_TX_PIN         GPIO_Pin_10
#define USART3_PORT           GPIOB

#define USART4_RX_PIN         GPIO_Pin_11
#define USART4_TX_PIN         GPIO_Pin_10
#define USART4_PORT           GPIOC

#define USART5_RX_PIN         GPIO_Pin_2
#define USART5_TX_PIN         GPIO_Pin_12
#define USART5_RX_PORT        GPIOD
#define USART5_TX_PORT        GPIOC


#define _SET_BIT(x,y)          ((x)|=(0x0001<<(y)))   
#define _CLR_BIT(x,y)          ((x)&=~(0x0001<<(y)))  
#define _CPL_BIT(x,y)          ((x)^=(0x0001<<(y)))   
#define _GET_BIT(x,y)          ((x)&(0x0001<<(y)))    


void BSP_IntDisAll(void); //¹Ø×ÜÖÐ¶Ï
void BSP_CPU_TickInit(void);

void BSP_Init(void);


#endif


