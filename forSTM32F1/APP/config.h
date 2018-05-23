#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "stm32f10x.h"


typedef enum{
	ENOERR = 0,
	ELENGTHERR,
	EADDRERR,
	ECHECKERR,
    EBUFFNULL,
}ERR_NUM;


#endif



