#include "usart.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"

//uint8_t g_uart1_dma_rxbuf[UART1_DMA_RX_BUF_LEN] = {0};
//uint8_t g_uart1_dma_txbuf[UART1_DMA_TX_BUF_LEN] = {0};
//uint16_t g_uart1_rx_read_idx = 0;
//uint16_t g_uart1_rx_write_idx = 0;
//uint8_t  g_uart1_tx_done_flag = 0;
USART_RX_Buffer_t USART1_RxBuffer;
USART_RX_Buffer_t USART6_RxBuffer;

/**
 * @brief  串口 GPIO 及 USART 初始化
 * @param  无
 * @retval 无
 */
void USART1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 使能时钟（GPIOA 和 USART1）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    // 2. 配置 GPIO 为复用功能
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10,GPIO_AF_USART1);
/*初始化串口1*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // 复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 速度 100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽输出（TX）
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // 上拉（RX）
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // 3. 配置 USART 参数
    USART_InitStructure.USART_BaudRate = USARTx_BAUDRATE;  // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  // 8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      // 1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         // 无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  // 收发模式
    USART_Init(USART1, &USART_InitStructure);	
    // 5. 使能 USART
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
/*初始化串口6*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7,GPIO_AF_USART6);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // 复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 速度 100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽输出（TX）
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // 上拉（RX）
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = USARTx_BAUDRATE;  // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  // 8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      // 1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         // 无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  // 收发模式
    USART_Init(USART6, &USART_InitStructure);
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART6, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	

}

/**
 * @brief  发送单个字节
 * @param  USARTx: 串口外设（如 USART1）
 * @param  data: 要发送的字节
 * @retval 无
 */
void USART_SendByte(USART_TypeDef* USARTx, uint8_t data) {
    // 等待发送缓冲区为空
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    // 发送数据
    USART_SendData(USARTx, data);
}

/**
 * @brief  发送字符串
 * @param  USARTx: 串口外设
 * @param  str: 要发送的字符串（以 '\0' 结尾）
 * @retval 无
 */
void USART_SendString(USART_TypeDef* USARTx, char* str) {
    while (*str != '\0') {
        USART_SendByte(USARTx, *str);
        str++;
    }
}

uint16_t USART1_ReceiveData(uint8_t *buffer, uint16_t max_len)
{
    uint16_t data_len = 0;
    
    // 检查是否有新数据
    if(USART1_RxBuffer.read_index != USART1_RxBuffer.write_index)
    {
        while(USART1_RxBuffer.read_index != USART1_RxBuffer.write_index && data_len < max_len)
        {
            buffer[data_len++] = USART1_RxBuffer.buffer[USART1_RxBuffer.read_index];
            USART1_RxBuffer.read_index = (USART1_RxBuffer.read_index + 1) % USART1_RECV_LEN;
        }
    }
    
    // 清除接收标志
    USART1_RxBuffer.recv_flag = 0;
    
    return data_len;
}

uint8_t USART1_GetRecvFlag(void)
{
    return USART1_RxBuffer.recv_flag;
}

void USART1_ClearRecvFlag(void)
{
    USART1_RxBuffer.recv_flag = 0;
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART1);
        
        // 将数据存入环形缓冲区
        USART1_RxBuffer.buffer[USART1_RxBuffer.write_index] = data;
        USART1_RxBuffer.write_index = (USART1_RxBuffer.write_index + 1) % USART1_RECV_LEN;
        
        // 设置接收标志
        USART1_RxBuffer.recv_flag = 1;
        
        // 如果缓冲区满了，覆盖最旧的数据
        if(USART1_RxBuffer.write_index == USART1_RxBuffer.read_index)
        {
            USART1_RxBuffer.read_index = (USART1_RxBuffer.read_index + 1) % USART1_RECV_LEN;
        }
        USART_SendByte(USART6,data);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

uint16_t USART6_ReceiveData(uint8_t *buffer, uint16_t max_len)
{
    uint16_t data_len = 0;
    
    // 检查是否有新数据
    if(USART6_RxBuffer.read_index != USART6_RxBuffer.write_index)
    {
        while(USART6_RxBuffer.read_index != USART6_RxBuffer.write_index && data_len < max_len)
        {
            buffer[data_len++] = USART6_RxBuffer.buffer[USART6_RxBuffer.read_index];
            USART6_RxBuffer.read_index = (USART6_RxBuffer.read_index + 1) % USART6_RECV_LEN;
        }
    }
	// 清除接收标志
    USART6_RxBuffer.recv_flag = 0;
    
    return data_len;

}

uint8_t USART6_GetRecvFlag(void)
{
    return USART6_RxBuffer.recv_flag;
}

void USART6_ClearRecvFlag(void)
{
    USART6_RxBuffer.recv_flag = 0;
}

void USART6_IRQHandler(void)
{
    if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART6);
        
        // 将数据存入环形缓冲区
        USART6_RxBuffer.buffer[USART6_RxBuffer.write_index] = data;
        USART6_RxBuffer.write_index = (USART6_RxBuffer.write_index + 1) % USART6_RECV_LEN;
        
        // 设置接收标志
        USART6_RxBuffer.recv_flag = 1;
        
        // 如果缓冲区满了，覆盖最旧的数据
        if(USART6_RxBuffer.write_index == USART6_RxBuffer.read_index)
        {
            USART6_RxBuffer.read_index = (USART6_RxBuffer.read_index + 1) % USART6_RECV_LEN;
        }
//        USART_SendByte(USART6,data);
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
    }
}

int fputc(int ch, FILE *f)
{
    // 等待串口发送数据寄存器为空（TXE位为1）
    while ((USART6->SR & USART_FLAG_TXE) == RESET);
    
    // 将字符写入串口数据寄存器，触发发送
    USART6->DR = (uint8_t)ch;
    
    return ch;
}
