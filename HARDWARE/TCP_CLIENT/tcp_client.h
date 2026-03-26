#ifndef __TCP_CLINET
#define __TCP_CLINET

#include "usart.h"
#include "delay.h"
void Tcp_Init(void);
void TCP_Connect(void);
void Tcp_Data_Send(float Temp,float Humi);

#endif
