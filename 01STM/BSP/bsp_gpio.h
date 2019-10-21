#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H
#include "stm32f10x_conf.h"





//引脚输入输出宏定义
//小灯开关
#define  LED_OPEN    		GPIO_SetBits(GPIOA, GPIO_Pin_15)
#define  LED_CLOSE   		GPIO_ResetBits(GPIOA, GPIO_Pin_15)
#define  LED_SWITCH()	 	(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) ? LED_CLOSE : LED_OPEN)
//beep
#define	BEEP_ON				GPIO_ResetBits(GPIOB, GPIO_Pin_14);
#define	BEEP_OFF			GPIO_SetBits(GPIOB, GPIO_Pin_14);

//Key input
#define 	KEY_READ								\
if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) != 1)	\
{													\
	delay_ms(500);									\
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) != 1)\
	{									\
		BEEP_OFF						\
		delay_ms(200);					\
		BEEP_ON							\
		delay_ms(100);					\
		BEEP_OFF						
						
		
		

#define 	KEY_READ_END				\
		BEEP_OFF						\
		delay_ms(200);					\
		BEEP_ON							\
		delay_ms(100);					\
		BEEP_OFF						\
		delay_ms(200);					\
		BEEP_ON							\
		delay_ms(100);					\
		BEEP_OFF						\
	}									\
}

//函数定义
void USART_Configure(void);
void GPIO_Configure(void);
void NVIC_Configure(void);


#endif  //_BSP_GPIO_H


