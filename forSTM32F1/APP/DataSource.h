#ifndef __DATASOURCE_H__
#define __DATASOURCE_H__
#include "config.h"
#include "app.h"
#define DEV_NUM                                     0x02

#define COMMD_ANALONG                               0x04    // 获取模拟量命令
#define COMMD_DIGITAL                               0x02    // 获取数字量命令

//#define DEV1_ADDR                                   29
//#define DEV2_ADDR                                   0xFE

#define PORT_1_ADDR                                 0x01
#define PORT_2_ADDR                                 0x02
#define PORT_3_ADDR                                 0x03
#define PORT_4_ADDR                                 0x04

#define ANALOG_NUM                                  64u
#define DIGITAL_NUM                                 10u

#define _SET_BIT(x,y)                               ((x)|=(0x0001<<(y)))   
#define _CLR_BIT(x,y)                               ((x)&=~(0x0001<<(y)))  
#define _CPL_BIT(x,y)                               ((x)^=(0x0001<<(y)))   
#define _GET_BIT(x,y)                               ((x)&(0x0001<<(y)))  


typedef struct {
    uint8_t   addr;
    uint8_t   cmmd;
    uint16_t  startaddr;
    uint16_t  num;
    uint16_t  crc;
}Inquire_t;

extern void    CreatDataSourceTask(void);
extern uint8_t GetAnalogData(uint16_t *dst);
extern uint8_t GetDigitalData(uint8_t *dst);
extern uint8_t GetUpsSta(UP_Data_t *dst);
extern void    ComError(uint8_t id);
extern void    GetCom(uint8_t *dat);
extern void    SetCom(uint8_t id);
#endif



