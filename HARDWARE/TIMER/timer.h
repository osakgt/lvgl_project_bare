#ifndef TIMER_H_
#define TIMER_H_

#include "stm32f4xx.h"

// 函数声明
void TIM2_Init(uint16_t arr, uint16_t psc);  // 初始化定时器6（arr：自动重装载值，psc：预分频值）
void TIM2_Start(void);                       // 启动定时器6
void TIM2_Stop(void);                        // 停止定时器6

// 外部声明中断回调函数（用户可在其他文件中实现具体逻辑）

#endif /* TIM6_H_ */

