﻿#include "bsp_at24x.h" 
#include "bsp_delay.h"


//初始化IIC接口
void AT24CXX_Init(void)
{
	IIC_Init();
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		if(IIC_Wait_Ack())
			printf("NO ack \r\n");
		else
			printf("ack \r\n");
		IIC_Send_Byte(ReadAddr>>8);//发送高地址
//		IIC_Wait_Ack();		 
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	 

	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址
 	}else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 
	}	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(25);//避免每次开机都写AT24CXX			   
	if(temp==0X05)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(25,0X05);
	    temp=AT24CXX_ReadOneByte(25);	  
		if(temp==0X05)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 
//============================执行程序========================
u8 addr_1st = BYTE_FIRST_CNT;			//存储第一个字节的地址/位置
u8 timer_value[6] = {0};	//存储计数器的值  高位在前低位在后
u32 Threshold_value = 0;	//警告阈值判断	高32bit判断
u16 threshold_low = 0;		//警告低16bit值判断

//功能：EEPROM存储->先读取判断再写入
void EEPROMReadSaveCol(void)
{
	u8 temp = AT24CXX_ReadOneByte(addr_1st);
	AT24CXX_WriteOneByte(addr_1st, (temp+1));
	if(temp == 0xFF){	
		temp = AT24CXX_ReadOneByte(EE_BYTE_2nd);
		AT24CXX_WriteOneByte(EE_BYTE_2nd, (temp + 1));
		if(temp == 0xFF){
			temp = AT24CXX_ReadOneByte(EE_BYTE_3rd);
			AT24CXX_WriteOneByte(EE_BYTE_3rd, (temp + 1));
			if(temp == 0xFF){
				temp = AT24CXX_ReadOneByte(EE_BYTE_4th);
				AT24CXX_WriteOneByte(EE_BYTE_4th, (temp + 1));
				if(temp == 0xFF){	
					temp = AT24CXX_ReadOneByte(EE_BYTE_5th);
					AT24CXX_WriteOneByte(EE_BYTE_5th, (temp + 1));
					if(temp == 0xFF){	
						temp = AT24CXX_ReadOneByte(EE_BYTE_6th);
						AT24CXX_WriteOneByte(EE_BYTE_6th, (temp + 1));
					}

				}
			}
		}
			
	}
}

//功能：初次上电判断函数
void EEPROMFirstPowerCol(void)
{
	if(AT24CXX_ReadOneByte(EE_INIT) == INIT_YES)
	{
		//清除所有字节的值
		//....

		//设置存储第一字节的地址
		AT24CXX_WriteOneByte(EE_BYTE_1st_LOCAL, BYTE_FIRST_CNT);
		AT24CXX_WriteOneByte(EE_INIT, INIT_NO);	
	}
	else	//获取EEPROM中的计数值
	{
		addr_1st = AT24CXX_ReadOneByte(EE_BYTE_1st_LOCAL);
		timer_value[EE_BYTE_6th] = AT24CXX_ReadOneByte(EE_BYTE_6th);
		timer_value[EE_BYTE_5th] = AT24CXX_ReadOneByte(EE_BYTE_5th);
		timer_value[EE_BYTE_4th] = AT24CXX_ReadOneByte(EE_BYTE_4th);
		timer_value[EE_BYTE_3rd] = AT24CXX_ReadOneByte(EE_BYTE_3rd);
		timer_value[EE_BYTE_2nd] = AT24CXX_ReadOneByte(EE_BYTE_2nd);
		timer_value[EE_BYTE_1st_LOCAL] = AT24CXX_ReadOneByte(addr_1st);	
		Threshold_value = timer_value[EE_BYTE_3rd] + timer_value[EE_BYTE_4th] >> 8 + timer_value[EE_BYTE_5th] >> 8*2 \
						 + timer_value[EE_BYTE_6th] >> 8*3;
	}
	
}
//功能：警告处理
void EEPROMWarnningCol(void)
{
	if(Threshold_value > WARNNING_VALUE)
	{
		//直接警告即可
	}
}









