#include "stm32f4xx.h"                  // Device header
#include "tcp_client.h"

void Tcp_Init(void)
{	
//	printf("AT+RESTORE\r\n");									//重置模块为出厂设置	
//	delay_ms(7000);
	
	printf("AT\r\n");
	delay_ms(3000);
	printf("AT+CWMODE=1\r\n");    								//配置为Station模式
	delay_ms(3000);
	printf("AT+CWJAP_DEF=\"Misaki\",\"Yc20231122\"\r\n");     		//连接到AP
	delay_ms(10000);
	printf("AT+CIFSR\r\n");   									//查询主机IP地址
	delay_ms(3000);
	printf("AT+CIPMUX=0\r\n");									//开启单链接
	delay_ms(3000);
	printf("AT+CIPSTART=\"TCP\",\"tcp.tlink.io\",8647\r\n");	//连接到TCP服务器
	delay_ms(3000);
	printf("AT+CIPSEND=16\r\n");								//发送16个字节到TCP服务器
	delay_ms(3000);
	printf("3L0C87619Q9JA1A1\r\n");								//发送设备序列号，连接设备
	delay_ms(3000);
	
}
//连接TLINK物联网云平台
void TCP_Connect(void)
{
 
	printf("AT+CIPSTART=\"TCP\",\"tcp.tlink.io\",8647\r\n");	//连接到TCP服务器
	delay_ms(3000);
	printf("AT+CIPSEND=16\r\n");								//发送16个字节到TCP服务器

	delay_ms(3000);
	printf("3L0C87619Q9JA1A1\r\n");								//发送设备序列号，连接设备

	delay_ms(3000);
}

//TCP协议向TLINK物联网云平台发送数据
void Tcp_Data_Send(float Temp,float Humi)
{	
	printf("AT+CIPSEND=13\r\n");
	delay_ms(3000);
	printf("#%.2f,%.2f#\r\n", Temp, Humi);
	delay_ms(2000);
}
