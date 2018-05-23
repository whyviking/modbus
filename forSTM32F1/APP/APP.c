#include "app.h"
#include "os_cpu.h"
#include "app_cfg.h"
#include "ucos_ii.h"
#include "bsp.h"
#include "config.h"
#include "mylib.h"
#include "Driver.h"
#include "DataSource.h"

// 任务堆栈
__align(8) static  OS_STK   TaskLedStk[TASK_LED_STK_SIZE];  
__align(8) static  OS_STK   TaskUpdataStk[TASK_UPDATA_STK_SIZE];
__align(8) static  OS_STK   TaskPollStk[TASK_POLL_STK_SIZE]; 

UP_Data_t updata = {0};                      // 数据 上传 结构体

    uint8_t *src  = 0, crc_i;
//任务声明
void TaskLed(void *p_arg);                            
void TaskUpdata(void *p_arg);
void TaskPoll(void *p_arg);


void CreatAPPTask(void)
{  
                                                   // 创建任务   
    OSTaskCreate (TaskLed,        
                  (void *)0,
                  &TaskLedStk[TASK_LED_STK_SIZE - 1],
                  TASK_LED_PRIO);
                  
    OSTaskCreate (TaskUpdata,        
                  (void *)0,
                  &TaskUpdataStk[TASK_UPDATA_STK_SIZE - 1],
                  TASK_UPDATA_PRIO); 
    
    OSTaskCreate (TaskPoll,        
                  (void *)0,
                  &TaskPollStk[TASK_POLL_STK_SIZE - 1],
                  TASK_POLL_PRIO);  
                  
}

// led1 闪烁任务 用于表示 系统运行状态
void TaskLed(void *p_arg)
{
    while(1)
    {
        LED1_ON;
        OSTimeDlyHMSM(0, 0, 0, 250);
        LED1_OFF;
        OSTimeDlyHMSM(0, 0, 0, 250);
    }
}


__inline void PackData(void)
{
    	
    updata.head = 0x7E;
    GetAnalogData(updata.analog);
    GetDigitalData(updata.switc);
	  //GetUpsSta(&updata);

	   src = (uint8_t *)&updata;
	  updata.crc=0;
   	for(crc_i=3; crc_i<162; crc_i++)       //计算累加和
    { 
		   updata.crc+= src[crc_i];         
    }
    updata.end  = 0x0D;
}

__inline void SendData(void)
{
    uint8_t *src = (uint8_t *)&updata;
    
    USART_Send(USART1, src, sizeof(updata));
}

//数据上传
void TaskUpdata(void *p_arg)
{
    uint8_t i   = 0x00;
    uint8_t sum = 0x00;
    uint8_t err = 0x00;
    Rx_t    *p  = 0x00;

    while(1)
    {
        p = OSMboxPend(rx_mbox_port[0], 0, &err);   // 等待	
        if(p == 0x00)
        {
            continue;
        }
        
        if(p->buff[0] != 0x7e)
        { 
            continue ;
        }
        
        sum = 0;
        
        for(i = 2; i<18; i++)         // 计算校验和
        {
            sum += p->buff[i];
        }
        
        if(sum != p->buff[i])
        {
           continue ;
        }
        
        PackData();
		LED2_ON;
        SendData();
		LED2_OFF;
    }
}




void TaskPoll(void *p_arg)
{

    uint8_t tem[2] = {0x00};
    
    while(1)
    {
        OSTimeDlyHMSM(0, 0, 5, 0);

            GetCom(tem);
            
            if(tem[0] == 0x00)
            {
                ComError(0x00);
            }else
						SetCom(0x00);

            if(tem[1] == 0x00)
            {
                ComError(0x01);
            }else
            SetCom(0x01);
		
       
    }

}



