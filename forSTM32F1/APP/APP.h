#ifndef __APP_H__
#define __APP_H__
#include "stm32f10x.h"

#pragma pack (1)
typedef struct{
    uint8_t  head;            //7e
    uint8_t  addr;            //00
    uint8_t  save;            //00
    uint16_t analog[64];      //模拟量
    uint8_t  none[8];        //00
	  uint8_t  ups_sta[2];
    uint8_t  none1[10];        //00
    uint8_t  switc[10];        //开关量
    uint8_t  none2;        //00
    uint8_t  crc;             //cheaksum
    uint8_t  end;             //0d
}UP_Data_t;
#pragma pack()

extern void CreatAPPTask(void);


#endif

