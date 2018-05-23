#include "os_cpu.h"
#include "ucos_ii.h"
#include "bsp.h"
#include "app.h"
#include "DataSource.h"
#include "Driver.h"

// task stack define
__align(8) static  OS_STK         TaskStartStk[TASK_START_STK_SIZE];

// task define
void TaskStart(void *p_arg);

/******************************************************begin**************************************************************/
int main(void)
{
    INT8U os_err;
    
    BSP_IntDisAll();                                                      /* 关总中断 */
    
    OSInit();                                                             /* Initialize "uC/OS-II, The Real-Time Kernel". */

	os_err = OSTaskCreateExt((void (*)(void *)) TaskStart,                /* Create the start task. */
                             (void          * ) 0,
                             (OS_STK        * )&TaskStartStk[TASK_START_STK_SIZE - 1],
                             (INT8U           ) TASK_START_PRIO,
                             (INT16U          ) TASK_START_PRIO,
                             (OS_STK        * )&TaskStartStk[0],
                             (INT32U          ) TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
	

    os_err = os_err;
	OSStart();                                                            /* Start multitasking (i.e. give control to uC/OS-II). */

    return 0;
}


void TaskStart (void *p_arg)
{   
	BSP_Init();                                                            /* 时钟 GPIO 初始化 */
	BSP_CPU_TickInit();  
	
    CreatDriverTask();
    
	//get_boma();
	
    CreatAPPTask();
    CreatDataSourceTask();

	OSTaskSuspend(TASK_START_PRIO);	                                      /* suspend but not delete */
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************************************************end**************************************************************/

