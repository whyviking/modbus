#ifndef __MYLIB_H__
#define __MYLIB_H__
#include "stm32f10x.h"

extern uint32_t my_ftoh_Lfirst(float f);
extern uint32_t my_ftoh_Hfirst(float f);
extern uint16_t getsum(uint8_t *src, uint16_t length);
extern uint16_t verifylength(uint16_t dat);
extern uint32_t my_atoi(const uint8_t *src, uint8_t size);
extern uint16_t my_crccheck( uint8_t * pucFrame, uint16_t usLen );
extern uint16_t my_ltob(uint16_t dat);
#endif



