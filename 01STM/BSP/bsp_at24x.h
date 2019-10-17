#ifndef __24CXX_H
#define __24CXX_H
#include "bsp_common.h"   

//宏定义
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C16

//========================定时器使用====================================
#define WARNNING_MAX_VALUE		0x6DDD00		//警告值
#define BYTE_FIRST_CNT			(EE_END + 1)

//AT存储的地址  -->const定义
enum
{
	EE_BYTE_3rd = 0,	//第三个字节
	EE_BYTE_2nd,		//第二个字节
	EE_BYTE_1st_LOCAL,	//存储第一个字节的地址/位置
	EE_INIT,			//EEPROM 是否初次上电

	EE_END				//结束标志	
};

//是否初次上电值
typedef enum 
{
	INIT_YES = 0,
	INIT_NO,
} 
eeprom_init_val;

//bool WarningAndWriteOutAndSave(bool flag);
void WarningAndWriteOutAndSave(u8* flag);
//void EEPROMFirstPowerCol(void);
void EEPROM_check(void);
void EEPROM_clear(void);
void EEPROM_init(void);

//------------IIC引脚配置---------------------
//IO方向设置	
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 		EEPROM_PIN     
#define IIC_SCL    PBout(6)	//SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

////IO方向设置	
//#define SDA_IN()  {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=(u32)8<<12;}
//#define SDA_OUT() {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=(u32)3<<12;}

////IO操作函数	 		EEPROM_PIN     
//#define IIC_SCL    PBout(10)	//SCL
//#define IIC_SDA    PBout(11) //SDA	 
//#define READ_SDA   PBin(11)  //输入SDA 



//=============================AT24函数==================================
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC

#endif
















