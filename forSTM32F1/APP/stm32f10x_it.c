/**
  ******************************************************************************
  * @file    RCC/RCC_ClockConfig/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "os_cpu.h"
#include "ucos_ii.h"
#include "Driver.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/




/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  if (CoreDebug->DHCSR & 1) {  //check C_DEBUGEN == 1 -> Debugger Connected  
      __breakpoint(0);  // halt program execution here         
  }  
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

void TIM4_IRQHandler(void) 
{
  
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */

    OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}

extern Rx_t rx_port[];
void USART1_IRQHandler(void)
{
    uint8_t dat = 0;
    OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
    OSIntNesting++;
    OS_EXIT_CRITICAL();          //恢复全局中断标志
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        dat = USART_ReceiveData(USART1);  //上行通讯
        
        if(rx_port[0].count == 0x00)
        {
            if(dat == 0x7E)
            {
                rx_port[0].buff[0] = dat;
                rx_port[0].start   = 0x01;
                rx_port[0].timeout = 0x10;
                rx_port[0].count++;
            }
        }
        else
        {
            rx_port[0].buff[rx_port[0].count++] = dat;
            rx_port[0].timeout = 0x10;
            
            if(rx_port[0].count >= PORT_RX_BUFF_LEN)
            {
                rx_port[0].count = 0;
            }
        }
    }
    
    
    OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换
    
}


void USART2_IRQHandler(void)
{
    uint8_t dat = 0;
    OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
    OSIntNesting++;
    OS_EXIT_CRITICAL();          //恢复全局中断标志

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        dat = USART_ReceiveData(USART2);  //上行通讯
        
        if(rx_port[1].count == 0x00)
        {
            rx_port[1].buff[0] = dat;
            rx_port[1].start   = 0x01;
            rx_port[1].timeout = 0x10;
            rx_port[1].count++;
        }
        else
        {
            rx_port[1].buff[rx_port[1].count++] = dat;
            rx_port[1].timeout = 0x10;
            
            if(rx_port[1].count >= PORT_RX_BUFF_LEN)
            {
                rx_port[1].count = 0;
            }
        }
    }
    
    OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换
}

void USART3_IRQHandler(void)
{
    uint8_t dat = 0;
    OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
    OSIntNesting++;
    OS_EXIT_CRITICAL();          //恢复全局中断标志

    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        dat = USART_ReceiveData(USART3);  //上行通讯
        
        if(rx_port[2].count == 0x00)
        {
            rx_port[2].buff[0] = dat;
            rx_port[2].start   = 0x01;
            rx_port[2].timeout = 0x10;
            rx_port[2].count++;
        }
        else
        {
            rx_port[2].buff[rx_port[2].count++] = dat;
            rx_port[2].timeout = 0x10;
            
            if(rx_port[2].count >= PORT_RX_BUFF_LEN)
            {
                rx_port[2].count = 0;
            }
        }
    }
    
    OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换
}




void UART4_IRQHandler(void)
{
	uint8_t dat = 0;
    OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
    OSIntNesting++;
    OS_EXIT_CRITICAL();          //恢复全局中断标志

    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        dat = USART_ReceiveData(UART4);  //上行通讯
        
        if(rx_port[3].count == 0x00)
        {
            if(dat == 0x7E)
            {
                rx_port[3].buff[0] = dat;
                rx_port[3].start   = 0x01;
                rx_port[3].timeout = 0x10;
                rx_port[3].count++;
            }
        }
        else
        {
            rx_port[3].buff[rx_port[3].count++] = dat;
            rx_port[3].timeout = 0x10;
            
            if(rx_port[3].count >= PORT_RX_BUFF_LEN)
            {
                rx_port[3].count = 0;
            }
        }
    }
    
    OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换
}

void UART5_IRQHandler(void)
{
    uint8_t dat = 0;
    OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
    OSIntNesting++;
    OS_EXIT_CRITICAL();          //恢复全局中断标志

    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        dat = USART_ReceiveData(UART5);  //上行通讯
        
        if(rx_port[4].count == 0x00)
        {
            if(dat == 0x7E)
            {
                rx_port[4].buff[0] = dat;
                rx_port[4].start   = 0x01;
                rx_port[4].timeout = 0x10;
                rx_port[4].count++;
            }
        }
        else
        {
            rx_port[4].buff[rx_port[4].count++] = dat;
            rx_port[4].timeout = 0x10;
            
            if(rx_port[4].count >= PORT_RX_BUFF_LEN)
            {
                rx_port[4].count = 0;
            }
        }
    }
    
    OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
