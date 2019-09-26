#include "bsp_common.h"

int main(void)
{
    delay_init();            //延时函数初始化
    GPIO_Configure();
    USART_Configure();
    NVIC_Configure();
	TIM3_Int_Init(4999, 7199);	//500ms

	while(1)
	{		
		
	}
}




