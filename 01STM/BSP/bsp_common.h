#ifndef _BSP_COMMON_H
#define _BSP_COMMON_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include <core_cm3.h>

typedef enum
{
    FALSE = 0, TRUE = 1
} bool;


#include "bsp_usart.h"
#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "bsp_delay.h"
#include "bsp_sys.h"







//函数定义
unsigned short ModBusCRC(unsigned char *pData, unsigned int siLen);



#endif  //_BSP_COMMON_H



