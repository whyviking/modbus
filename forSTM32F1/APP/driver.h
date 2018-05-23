#ifndef __DRIVER_H__
#define __DRIVER_H__
#include "config.h"
#include "ucos_ii.h"

#define PORT_NUM                              0x05
#define PORT_RX_BUFF_LEN                      0x80

#define PORT_0_RX_MBOX                        (rx_mbox_port[0])
#define PORT_1_RX_MBOX                        (rx_mbox_port[1])
#define PORT_2_RX_MBOX                        (rx_mbox_port[2])
#define PORT_3_RX_MBOX                        (rx_mbox_port[3])
#define PORT_4_RX_MBOX                        (rx_mbox_port[4])

#define PORT_0                                (USART1)
#define PORT_1                                (USART2)
#define PORT_2                                (USART3)
#define PORT_3                                (UART4)
#define PORT_4                                (UART5)

typedef struct{
    uint8_t  comid;
    uint8_t  start;
    int8_t   timeout;
    uint16_t count;
    uint8_t  buff[PORT_RX_BUFF_LEN];
}Rx_t;

extern OS_EVENT *rx_mbox_port[];


extern void     CreatDriverTask(void);
extern uint32_t USART_Send(USART_TypeDef* USARTx, const uint8_t *pBuf, uint32_t len);  // 串口发送函数 各个串口 共用












#endif





















