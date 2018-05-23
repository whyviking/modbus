#include "Driver.h"



Rx_t      rx_port[PORT_NUM]                          = {0x00};  // ���ڽ��սṹ��
OS_EVENT *rx_mbox_port[PORT_NUM]                     = {0x00};  // ���ڽ�������

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
*                                        ���ڽ�����ɼ��
*
* Description: �����������Ƿ������һ֡���ݵĽ��գ�û���ʲô��������������Ͷ�Ӧ���ڵ����䣬
*              ����Я���˸ô��ڵĽ��ջ���ָ�롣�ж��Ƿ���ɵ������ǣ����ڽ����ֽ������ֽڼ� ���
*              ʱ�䳬���趨�ĳ�ʱʱ�䣬����Ϊ���һ֡�Ľ���
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
				if(rx_port[i].timeout <= 0x00) // timeout ����봮�ڵ�Ƶ����أ��ڴ�Ӧ���в�Ҫ��ܾ�׼������δ����
				{
					rx_port[i].start = 0x00;
					rx_port[i].count = 0x00;
					OSMboxPost(rx_mbox_port[i], (void *)&rx_port[i]);  //ȷ�Ͻ�����ɺ� ������Ϣ ͬ�����ݴ�������
				}
			}
			
		}
    }
}

uint32_t USART_Send(USART_TypeDef* USARTx, const uint8_t *pBuf, uint32_t len)  // ���ڷ��ͺ��� �������� ����
{
	uint32_t sendlen=0;
	uint32_t i=0;
    
    USART_ClearFlag(USARTx,USART_FLAG_TC);            // �����ط���״̬
	for(i=0;i<len;i++)
	{
		USART_SendData(USARTx , pBuf[i]);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET)
        {
            OSTimeDlyHMSM(0, 0, 0, 1);
        }
	}
	sendlen = i;	  
	return sendlen;   // ���ر����ѷ��͵��ֽ���
}




