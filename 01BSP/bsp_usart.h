#ifndef _BSP_USART_H
#define _BSP_USART_H
#include "stm32f10x_conf.h"

//串口接收队列缓存长度
#define USART_BUFFER_LEN    4000



//定义配置使能
#define USART1_CONFIG_ENABLED                (1)
#define USART2_CONFIG_ENABLED                (1)
#define USART3_CONFIG_ENABLED                (0)



//定义串口波特率
#define USART1_BAUDRATE                       115200
#define USART2_BAUDRATE                       115200
#define USART3_BAUDRATE                       115200



//函数接口
void USART_SendByte(USART_TypeDef* USARTx, uint8_t byte);
void USART_SendBytes(USART_TypeDef* USARTx, uint8_t *str, uint8_t len);
void USART_DEBUG(char *str);
void Send_CMD(USART_TypeDef* USARTx, u8 HCMD, u8 LCMD);
void Send_CMD_DAT(USART_TypeDef* USARTx, u8 HCMD, u8 LCMD, char *dat, u16 dat_len);
u8 USART_BufferRead(u8 *data);
void USART_BufferWrite(u8 ntemp);
void SoftwareRESET(void);

void HandleDatCmd(u16 cmd, char* dat, u16 datLen);



#endif  //_BSP_USART_H


