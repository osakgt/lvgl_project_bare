#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f4xx.h"
// DHT11 引脚定义（可根据实际接线修改）
#define DHT11_GPIO_PORT    GPIOB
#define DHT11_DATA_PIN     GPIO_Pin_12  // 示例：PB12 连接 DHT11 的 DATA 引脚

// 数据结构体（存储温湿度）
//typedef struct {
//    uint8_t humidity;    // 湿度整数部分（0-99 %RH）
//    uint8_t humi_dec;        // 温度整数部分（0-50 ℃）
//    uint8_t temp;    // 湿度小数部分（DHT11 通常为 0）
//    uint8_t temp_dec;    // 温度小数部分（DHT11 通常为 0）
//} DHT11_DataTypedef;

// 函数声明
void DHT_Start(void);
uint16_t DHT_ReadData(uint8_t buffer[5]);

#endif
