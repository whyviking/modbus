#include "Driver.h"



Rx_t      rx_port[PORT_NUM]                          = {0x00};  // 串口接收结构体
OS_EVENT *rx_mbox_port[PORT_NUM]                     = {0x00};  // 串口接收邮箱

__align(8) static  OS_STK         TaskRxPollStk[TASK_RXPOLL_STK_SIZE];



void TaskRxPoll(void *p_arg);

void CreatDriverTask(void)
{   
    uint8_t i = 0x00;
    
    for(i=0; i<PORT_NUM; i++)
    {
        rx_mbox_port[i]  = OSMboxCreate(0);
        rx_port[i].comid = i;
    }
    
    OSTaskCreate (TaskRxPoll,        
              (void *)0,
              &TaskRxPollStk[TASK_RXPOLL_STK_SIZE - 1],
              TASK_RXPOLL_PRIO);
}
/*
*********************************************************************************************************
*                                        串口接收完成检测
*
* Description: 检测各个串口是否完成了一帧数据的接收，没完成什么都不做，完成则发送对应串口的邮箱，
*              邮箱携带了该串口的接收缓存指针。判断是否完成的依据是：串口接收字节流，字节间 间隔
*              时间超过设定的超时时间，则认为完成一帧的接收
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/
void TaskRxPoll(void *p_arg)
{
	uint8_t i = 0x00;
	  
    while(1)
    {
        OSTimeDlyHMSM(0, 0, 0, 1);
		
		for(i=0; i<PORT_NUM; i++)
		{
			if(rx_port[i].start != 0x00)
			{
				rx_port[i].timeout--;
				if(rx_port[i].timeout <= 0x00) // timeout 最好与串口的频率相关，在此应用中不要求很精准，所以未关联
				{
					rx_port[i].start = 0x00;
					rx_port[i].count = 0x00;
					OSMboxPost(rx_mbox_port[i], (void *)&rx_port[i]);  //确认接收完成后 发送消息 同步数据处理任务
				}
			}
			
		}
    }
}

uint32_t USART_Send(USART_TypeDef* USARTx, const uint8_t *pBuf, uint32_t len)  // 串口发送函数 各个串口 共用
{
	uint32_t sendlen=0;
	uint32_t i=0;
    
    USART_ClearFlag(USARTx,USART_FLAG_TC);            // 清楚相关发送状态
	for(i=0;i<len;i++)
	{
		USART_SendData(USARTx , pBuf[i]);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET)
        {
            OSTimeDlyHMSM(0, 0, 0, 1);
        }
	}
	sendlen = i;	  
	return sendlen;   // 返回本次已发送的字节数
}




