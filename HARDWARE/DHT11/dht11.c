#include "dht11.h"
#include "delay.h"  // 需要微秒级延时函数（下面有说明）

// 引脚输出模式配置
static void DHT11_Mode_Output(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_DATA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    // 输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 不上下拉（DHT11 内部有上拉）
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

// 引脚输入模式配置
static void DHT11_Mode_Input(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_DATA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;     // 输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 不上下拉（依赖外部上拉或内部上拉）
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

// 输出高低电平（宏定义简化代码）
#define DHT11_DATA_HIGH GPIO_SetBits(DHT11_GPIO_PORT, DHT11_DATA_PIN)
#define DHT11_DATA_LOW  GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_DATA_PIN)
#define DHT11_DATA_READ GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_DATA_PIN)


void DHT_Start(void)
{
	DHT11_Mode_Output();
	DHT11_DATA_HIGH;
	delay_ms(100);
	DHT11_DATA_LOW;
	delay_ms(20);
	DHT11_DATA_HIGH;
	delay_us(30);
	DHT11_Mode_Input();
}

uint16_t DHT_Scan(void)
{
	return DHT11_DATA_READ; // 返回读取数据
}


uint16_t DHT_ReadBit(void)
{
	while(DHT11_DATA_READ == RESET);
	delay_us(40);
	if(DHT11_DATA_READ == SET)
	{
		while(DHT11_DATA_READ == SET);
		return 1;
	}
	else
	{
		return 0;
	}
}

uint16_t DHT_ReadByte(void)
{
	uint16_t i, data = 0;
	for(i = 0; i < 8; i++)
	{
		data <<= 1; // 向左进一位
		data |= DHT_ReadBit();
	}
	return data;
}

uint16_t DHT_ReadData(uint8_t buffer[5])
{
	uint16_t i =0;
	
	DHT_Start();
	if(DHT_Scan() == RESET)
	{
		while(DHT_Scan() == RESET);
		while(DHT_Scan() == SET);
		for(i = 0; i < 5; i++)
		{
			buffer[i] = DHT_ReadByte();
		}
		// DHT11输出的40位数据
		while(DHT_Scan() == RESET);
		DHT11_Mode_Output();
		DHT11_DATA_HIGH;
		
		uint8_t check = buffer[0] + buffer[1] + buffer[2] + buffer[3];
		if(check != buffer[4])
		{
			return 1; // 数据出错
		}
	}
	return  0;
}
///**
// * @brief  初始化 DHT11（配置 GPIO 并发送起始信号）
// * @retval 无
// */
//void DHT11_Init(void) {
//    // 初始化为输出模式，拉高总线（空闲状态）
//    DHT11_Mode_Output();
//    DHT11_DATA_HIGH;
//    delay_ms(100);  // 等待 DHT11 稳定
//}

///**
// * @brief  读取 DHT11 温湿度数据
// * @param  data: 存储温湿度的结构体指针
// * @retval 0: 读取成功；1: 读取失败（超时或校验错误）
// */
//uint8_t DHT11_ReadData(DHT11_DataTypedef *data) {
//    uint8_t buf[5] = {0};  // 存储 40 位数据（8位湿度整数 + 8位湿度小数 + 8位温度整数 + 8位温度小数 + 8位校验和）
//    uint8_t i, j;

//    // 1. 主机发送起始信号
//    DHT11_Mode_Output();
//    DHT11_DATA_LOW;        // 拉低总线至少 18ms
//    delay_ms(20);
//    DHT11_DATA_HIGH;       // 拉高总线 20-40us
//    delay_us(30);

//    // 2. 切换为输入模式，等待 DHT11 响应
//    DHT11_Mode_Input();
//    delay_us(40);

//    // 3. 检查 DHT11 响应（低电平 80us）
//    if (DHT11_DATA_READ == 0) {
//        // 等待响应低电平结束（80us）
//        while (DHT11_DATA_READ == 0);
//        // 等待响应高电平结束（80us）
//        while (DHT11_DATA_READ == 1);

//        // 4. 读取 40 位数据（5 字节）
//        for (i = 0; i < 5; i++) {
//            for (j = 0; j < 8; j++) {
//                // 每 bit 前有 50us 低电平
//                while (DHT11_DATA_READ == 0);
//                // 延时 26-28us 后判断电平：高电平 >40us 为 1，否则为 0
//                delay_us(30);
//                buf[i] <<= 1;  // 左移一位
//                if (DHT11_DATA_READ == 1) {
//                    buf[i] |= 0x01;  // 写 1
//                }
//                // 等待高电平结束
//                while (DHT11_DATA_READ == 1);
//            }
//        }

//        // 5. 校验数据（前 4 字节和 == 第 5 字节）
//        if (buf[0] + buf[1] + buf[2] + buf[3] == buf[4]) {
//            data->humidity = buf[0];    // 湿度整数
//            data->humi_dec = buf[1];    // 湿度小数（DHT11 通常为 0）
//            data->temp = buf[2];        // 温度整数
//            data->temp_dec = buf[3];    // 温度小数（DHT11 通常为 0）
//            return 0;  // 读取成功
//        }
//    }

//    // 读取失败（超时或校验错误）
//    return 1;

//}
