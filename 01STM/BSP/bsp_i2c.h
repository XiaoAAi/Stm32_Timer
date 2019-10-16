#ifndef __MYIIC_H
#define __MYIIC_H
#include "bsp_common.h"

//切换EEPROM和OELD写入通道
//static bool switch_eeprom_oled;

//enum sw_ee_ol
//{
//	EEPROM_PIN = 1,
//	OLED_PIN = !EEPROM_PIN
//};

//IO方向设置	
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 		EEPROM_PIN     
#define IIC_SCL    PBout(6)	//SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 


//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
void PB7int(void);
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  

//void IIC_Switch_EEPROM_OLED(bool flag);		//切换通道函数
#endif
















