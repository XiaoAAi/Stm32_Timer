#include "bsp_at24x.h" 
#include "bsp_delay.h"

//------------------------IIC-----------------------------
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);	
	IIC_SCL=1; 
	delay_us(4);
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);		
	delay_us(4);	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
//	PB7int();
//	IIC_SDA=1;
	delay_us(4);	   
	IIC_SCL=1;
	delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	delay_us(4);
	IIC_SCL=0;//时钟输出0 	
	delay_us(4);	
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	IIC_SDA=0;
	delay_us(4);
	SDA_OUT();
	IIC_SCL=1;
	delay_us(4);
//	IIC_SDA=0;
//	delay_us(4);
//	IIC_SCL=1;
//	delay_us(4);
	IIC_SCL=0;
	delay_us(4);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(4);
	IIC_SCL=1;
	delay_us(4);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(4);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(4); 
		IIC_SCL=0;	
		delay_us(4);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(4);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(3); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


//-------------------------AT24CXXX-----------------------------


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
//u8 addr_1st = BYTE_FIRST_CNT;			//存储第一个字节的地址/位置
//u8 timer_value[6] = {0};	//存储计数器的值  高位在前低位在后
volatile u32 threshold_value = 0;	//警告阈值判断	高32bit判断
bool flag_save_eeprom = FALSE;	//存储EEPROM标志
#define WRITE_OUT_1W	40	//40*255 = 10200次

////功能：初次上电判断函数  （init配置）  -->暂不需要
//void EEPROMFirstPowerCol(void)
//{
//	if(AT24CXX_ReadOneByte(EE_INIT) == INIT_YES)
//	{
//		//清除所有字节的值
//		//....

//		//设置存储第一字节的地址
//		AT24CXX_WriteOneByte(EE_BYTE_1st_LOCAL, BYTE_FIRST_CNT);
//		AT24CXX_WriteOneByte(EE_INIT, INIT_NO);	
//	}
//	else	//获取EEPROM中的计数值
//	{
//		u8 addr_1st = AT24CXX_ReadOneByte(EE_BYTE_1st_LOCAL);
//		timer_value[EE_BYTE_3rd] = AT24CXX_ReadOneByte(EE_BYTE_3rd);
//		timer_value[EE_BYTE_2nd] = AT24CXX_ReadOneByte(EE_BYTE_2nd);
//		timer_value[EE_BYTE_1st_LOCAL] = AT24CXX_ReadOneByte(addr_1st);	
//	}
	
//}

//功能：第一个字节的地址
u16 Return1stAddr(void)
{
	return (u16)(AT24CXX_ReadOneByte(EE_BYTE_1st_HIGH) << 8 | AT24CXX_ReadOneByte(EE_BYTE_1st_LOW));
}

//功能：第一位字节的返回
u8 Return1stValue(void)
{
//	u16 temp = AT24CXX_ReadOneByte(EE_BYTE_1st_HIGH) << 8 | AT24CXX_ReadOneByte(EE_BYTE_1st_LOW);

	return AT24CXX_ReadOneByte(Return1stAddr());
}

//功能：写第一字节的值
void Write1stValue(u8 date)
{
	AT24CXX_WriteOneByte(Return1stAddr(), date);
}


//功能：EEPROM存储
void EEPROMReadSaveCol(void)
{
//	u8 addr_1st = AT24CXX_ReadOneByte(EE_BYTE_1st_LOCAL);
//	u8 temp = AT24CXX_ReadOneByte(addr_1st);
	
	u8 temp = Return1stValue();
	Write1stValue(((temp+1)&0xFF));
//	Write1stValue((0xFF));
	
	
//	AT24CXX_WriteOneByte(addr_1st, (temp + 1));
//	AT24CXX_WriteOneByte(addr_1st, 0xFF);
	if(temp == 0xFF){	
		temp = AT24CXX_ReadOneByte(EE_BYTE_2nd);
		AT24CXX_WriteOneByte(EE_BYTE_2nd, ((temp+1)&0xFF));
		if(temp == 0xFF){
	
			temp = AT24CXX_ReadOneByte(EE_BYTE_3rd);
			AT24CXX_WriteOneByte(EE_BYTE_3rd, ((temp+1)&0xFF));
		}
			
	}
}

//功能：EEPROM低8bit写入超过10200次移至下个存储区
void EEPROMWriteOut1W(void)
{
	u16 cnt_16B_value = (AT24CXX_ReadOneByte(EE_BYTE_3rd) << 8) | (AT24CXX_ReadOneByte(EE_BYTE_2nd));
	//读取低8bit存储位置值 减去 结束值 （5 - 4 = 1）表示使用1个低8bit存储位置
	u16 temp1 = Return1stAddr();
	u16 temp = (Return1stAddr() - EE_END) * WRITE_OUT_1W;
	if(cnt_16B_value >= (Return1stAddr() - EE_END) * WRITE_OUT_1W)
	{
//		u8 temp = AT24CXX_ReadOneByte(EE_BYTE_1st_LOCAL);
//		//修改写入低8bit的地址 +1
//		AT24CXX_WriteOneByte(EE_BYTE_1st_LOCAL, (AT24CXX_ReadOneByte(EE_BYTE_1st_LOCAL) + 1));
		
		u16 temp = Return1stAddr() + 1;
		//修改地址高8bit    	低8bit
		AT24CXX_WriteOneByte(EE_BYTE_1st_HIGH, (u8)(temp>>8&0xFF));
		AT24CXX_WriteOneByte(EE_BYTE_1st_LOW, (u8)(temp&0xFF));

	}	
}


//功能：警告 + 写入超出处理 + 写入保存
//参数：bool flag:存储EEPROM标志
//返回值：bool 用于修改flag_save_eeprom的值
void WarningAndWriteOutAndSave(u8* flag)
{

	if(*flag)	//判断是否需要存储
	{
//		EEPROMWarnningCol();	//警告提醒    -->太过频繁
		
		EEPROMWriteOut1W();		//低8bit写入超出

		EEPROMReadSaveCol();	//写入保存

		
		*flag = FALSE;
	}

//	return FALSE;
}

//功能：返回以10s 为单位的值
u32 ReturnEEprom10sValue(void)
{
	//读取 10s为单位的值
	threshold_value = (AT24CXX_ReadOneByte(EE_BYTE_3rd) << 16) | (AT24CXX_ReadOneByte(EE_BYTE_2nd) << 8) \
		| Return1stValue();
	
	return threshold_value;
}


//功能：警告处理
void EEPROMWarnningCol(void)
{
	if(ReturnEEprom10sValue() > WARNNING_MAX_VALUE)
	{
		//直接警告即可
		BEEP_ON
	}
}

//功能：读取EEPROM数据
//参数：无
//返回值： > 0 表示有效的小时数   = 0表示无效值 没有累计一小时
u16 ReadEEpromHowHour(void)
{
	static u16 cnt_fact_hour_value = 0;			//对比
	//获取到小时数
	u16 cnt_hour_value = (ReturnEEprom10sValue() / 360) & 0xFFFF;
	
	if(cnt_hour_value != cnt_fact_hour_value)
	{
		cnt_fact_hour_value = cnt_hour_value;
	}
	else
	{
		cnt_hour_value = 0;
	}

	return cnt_hour_value;
}


//功能:处理小时数显示OLED
void ByHourDisplay(u16 hour_val)
{
	int cnt = 0;

	while(hour_val)
	{
		OLED_ShowCHinese_32X32((90 - (cnt * 20)), 3, (hour_val % 10));
		hour_val /= 10;
		cnt++;
	}
}



//EEPROM  检测
void EEPROM_check(void)
{
	EEPROMWarnningCol();		//EEPROM报警
	ByHourDisplay(ReadEEpromHowHour());		//显示时长
}

//EEPROM init 
void EEPROM_init(void)
{
	OLED_ShowCHinese_32X32(90, 3, 0);
	EEPROM_check();
}

//EEPROM clear
void EEPROM_clear(void)
{
	for(int i=0; i<EE_TYPE; i++)
	{
		AT24CXX_WriteOneByte(i, 0x00);
	}
	//设置存储第一字节的地址
	AT24CXX_WriteOneByte(EE_BYTE_1st_HIGH, 0x00);
	AT24CXX_WriteOneByte(EE_BYTE_1st_LOW, BYTE_FIRST_CNT);
//	AT24CXX_WriteOneByte(EE_INIT, INIT_NO);			//初始化，--->不需要
	
	OLED_Clear();
	DISPLAY_SHI_LIAN_TIMER
	EEPROM_init();		//初始化
}


