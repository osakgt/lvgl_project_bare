#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

// 串口引脚定义（USART1 默认引脚：PA9-TX, PA10-RX）
#define USARTx_BAUDRATE 115200  // 波特率

// GPIO 配置


// 接收缓冲区大小
#define USART1_RECV_LEN 256  // 接收缓冲区长度
#define USART6_RECV_LEN 256  // 接收缓冲区长度

typedef struct {
    uint8_t buffer[USART1_RECV_LEN];
    uint16_t write_index;
    uint16_t read_index;
    uint8_t recv_flag;  // 接收到新数据标志
} USART_RX_Buffer_t;

extern USART_RX_Buffer_t USART1_RxBuffer;
extern USART_RX_Buffer_t USART6_RxBuffer;

// 函数声明
void USART1_Init(void);                      
void USART_SendByte(USART_TypeDef* USARTx, uint8_t data);
uint16_t USART1_ReceiveData(uint8_t *buffer, uint16_t max_len);
uint8_t USART1_GetRecvFlag(void);
void USART_SendString(USART_TypeDef* USARTx, char* str) ;
uint16_t USART6_ReceiveData(uint8_t *buffer, uint16_t max_len);
uint8_t USART6_GetRecvFlag(void);

#endif
