#include "bsp_common.h"
extern bool flag_save_eeprom;
extern bool flag_checkout;

#define OPEN_TIP	BEEP_ON			\
					delay_ms(300);	\
					BEEP_OFF

int main(void)
{
    delay_init();            //延时函数初始化
    GPIO_Configure();
    USART_Configure();
    NVIC_Configure();
	TIM3_Int_Init(9999, 7199);	//500ms
//	TIM3_Int_Init(499, 7199);	//50ms
	//开机提示音
//	OPEN_TIP
	
//	//EEPROM判断是否第一次烧写？ 不需要
//	EEPROMFirstPowerCol();	
	//初始化OLED
	OLED_Init();
	OLED_Clear();
	DISPLAY_SHI_LIAN_TIMER
	
	EEPROM_init();

	while(1)
	{							

//清空 长按1S     蜂鸣器2声
KEY_READ
		EEPROM_clear();
KEY_READ_END

		
		//写入		
		WarningAndWriteOutAndSave((u8*)&flag_save_eeprom);	
		

		//检测+报警+显示
		if(flag_checkout)
		{
			flag_checkout = FALSE;
			EEPROM_check();
		}
	}
}




