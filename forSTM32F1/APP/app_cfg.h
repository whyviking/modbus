/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : EHS
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  TASK_START_PRIO                               1u
#define  TASK_RXPOLL_PRIO                              3u

#define  TASK_LED_PRIO                                 11u
#define  TASK_UPDATA_PRIO                              2u
#define  TASK_POLL_PRIO                                8u

#define  TASK_PORT1DH_PRIO                             4u
#define  TASK_PORT2DH_PRIO                             5u
#define  TASK_PORT1TX_PRIO                             6u
#define  TASK_PORT2TX_PRIO                             7u


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  TASK_START_STK_SIZE                          64u
#define  TASK_RXPOLL_STK_SIZE                         128u

#define  TASK_LED_STK_SIZE                            64u
#define  TASK_UPDATA_STK_SIZE                         128u
#define  TASK_POLL_STK_SIZE                           128u

#define  TASK_PORT1DH_STK_SIZE                        128u
#define  TASK_PORT2DH_STK_SIZE                        128u
#define  TASK_PORT1TX_STK_SIZE                        64u
#define  TASK_PORT2TX_STK_SIZE                        64u



/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/


#endif
