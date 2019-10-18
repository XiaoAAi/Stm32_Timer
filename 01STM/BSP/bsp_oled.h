//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : Evk123
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : 0.69寸OLED 接口演示例程(STM32F103ZE系列IIC)
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）            
//              ----------------------------------------------------------------
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __BSP_OLED_H
#define __BSP_OLED_H			  	 
#include "bsp_common.h"
#include "stdlib.h"	

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    

//功能：显示实联医疗定时器
//#define  DISPLAY_SHI_LIAN_TIMER			\
//	{									\
//		OLED_Clear();					\
//		OLED_ShowCHinese(0,0,0);		\
//		OLED_ShowCHinese(18,0,1);		\
//		OLED_ShowCHinese(36,0,2);		\
//		OLED_ShowCHinese(54,0,3);		\
//		OLED_ShowCHinese(72,0,4);		\
//		OLED_ShowCHinese(90,0,5);		\
//		OLED_ShowCHinese(108,0,6);		\
//		OLED_ShowChar(120, 4, 'H', 0);	\
//	}

#define  DISPLAY_SHI_LIAN_TIMER			\
	{									\
		OLED_Clear();					\
		OLED_ShowCHinese(0,0,0);		\
		OLED_ShowCHinese(18,0,1);		\
		OLED_ShowCHinese(36,0,2);		\
		OLED_ShowCHinese(54,0,3);		\
		OLED_ShowCHinese(72,0,4);		\
		OLED_ShowCHinese(90,0,5);		\
		OLED_ShowCHinese(108,0,6);		\
		OLED_ShowCHinese_8X16(120, 3, 0);	\
		OLED_ShowCHinese_8X16(120, 5, 1);	\
	}

//		//固定位置  万 千 百 十 个 H
//		OLED_ShowCHinese_32X32(10, 3, 0);
//		OLED_ShowCHinese_32X32(30, 3, 1);
//		OLED_ShowCHinese_32X32(50, 3, 2);
//		OLED_ShowCHinese_32X32(70, 3, 3);
//		OLED_ShowCHinese_32X32(90, 3, 8);
//		OLED_ShowChar(120, 4, 'H', 0);
	

//-----------------OLED IIC端口定义----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_10)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_11)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_11)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
//void Picture();
//void IIC2_Start();
//void IIC2_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);


//显示汉字  32X32
void OLED_ShowCHinese_32X32(u8 x,u8 y,u8 no);
//显示汉字8X16
void OLED_ShowCHinese_8X16(u8 x,u8 y,u8 no);


//void IIC2_Wait_Ack();
#endif  
	 



