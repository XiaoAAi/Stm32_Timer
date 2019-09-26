#include "bsp_common.h"

bool flagEnableDebug = TRUE;		//DEBUG打印

u8 UsartBuffer[USART_BUFFER_LEN] = {0}; //数据缓冲区
u16 UsartWptr = 0;
u16 UsartRptr = 0;



//功能：串口1接PC机
#if USART1_CONFIG_ENABLED > 0
void USART1_IRQHandler(void)
{
    uint8_t nTemp;

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); //clear flag
        USART_BufferWrite(nTemp);
    }

    if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(USART1);    // delete data
        USART_ClearFlag(USART1, USART_FLAG_ORE);
    }
}
#endif

//功能：串口2接电机板
#if USART2_CONFIG_ENABLED > 0
void USART2_IRQHandler(void)
{
    uint8_t nTemp;

    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(USART2);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); //clear flag
        /**********************************************/
        USART_BufferWrite(nTemp);
    }

    if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(USART2);    // delete data
        USART_ClearFlag(USART2, USART_FLAG_ORE);
    }

}
#endif

#if USART3_CONFIG_ENABLED > 0
void USART3_IRQHandler(void)
{
    uint8_t nTemp;

    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(USART3);
        USART_ClearITPendingBit(USART3, USART_IT_RXNE); //clear flag
        /**********************************************/
    }

    if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(USART3);    // delete data
        USART_ClearFlag(USART3, USART_FLAG_ORE);
    }

}
#endif


//USART发送单字节
void USART_SendByte(USART_TypeDef* USARTx, uint8_t byte)
{
    /* 发送一个字节数据到USART */
    USART_SendData(USARTx, byte);

    /* 等待发送数据寄存器为空 */
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

//USART发送字符串
void USART_SendBytes(USART_TypeDef* USARTx, uint8_t *str, uint8_t len)
{
    uint8_t i;

    for(i = 0; i < len; i++)
    {
        USART_SendByte(USARTx, str[i]);
    }
}

//USART发送字符串
void USART_SendBytess(USART_TypeDef* USARTx, char *str)
{
    while(*str != '\0')
    {
        USART_SendByte(USARTx, *str);
        str++;
    }
}
void USART_DEBUG(char *str)
{
    if(flagEnableDebug)
    {
        while(*str != '\0')
        {
            USART_SendByte(USART1, *str);
            str++;
        }
    }
}


u8 USART_BufferRead(u8 *data)
{
	u8 ret = 0;
    if(UsartRptr != UsartWptr) // empty
    {
        *data = UsartBuffer[UsartRptr];
		UsartRptr = (UsartRptr + 1) % USART_BUFFER_LEN;
		ret = 1; 		
    }

    return ret;
}

//写入数据 + 接收中断处理
void USART_BufferWrite(u8 ntemp)
{
    if((UsartWptr + 1) % USART_BUFFER_LEN == UsartRptr) // full
    {
        return;
    }

    UsartBuffer[UsartWptr] = ntemp;

    if(UsartBuffer[UsartWptr] == 0x0A && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 1) % USART_BUFFER_LEN] == 0x0D
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 2) % USART_BUFFER_LEN] == 0xC1 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 3) % USART_BUFFER_LEN] == 0x81
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 4) % USART_BUFFER_LEN] == 0x02 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 5) % USART_BUFFER_LEN] == 0x00
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 6) % USART_BUFFER_LEN] == 0x14 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 7) % USART_BUFFER_LEN] == 0x03)
    {
        //复位对射
        __disable_irq();
        NVIC_SystemReset();
    }

    UsartWptr = (UsartWptr + 1) % USART_BUFFER_LEN;
}





