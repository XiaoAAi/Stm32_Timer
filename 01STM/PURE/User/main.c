#include "bsp_common.h"
extern bool flag_save_eeprom;

int main(void)
{
    delay_init();            //延时函数初始化
    GPIO_Configure();
    USART_Configure();
    NVIC_Configure();
	TIM3_Int_Init(4999, 7199);	//500ms
	EEPROMFirstPowerCol();
	
//	OLED_Init();
//	OLED_Clear();
//		OLED_ShowCHinese(0,0,0);//中
//		OLED_ShowCHinese(18,0,1);//景
//		OLED_ShowCHinese(36,0,2);//园
//		OLED_ShowCHinese(54,0,3);//电
//		OLED_ShowCHinese(72,0,4);//子
//		OLED_ShowCHinese(90,0,5);//科
//		OLED_ShowCHinese(108,0,6);//技

	while(1)
	{					

		WarningAndWriteOutAndSave((u8*)&flag_save_eeprom);	
	}
}




