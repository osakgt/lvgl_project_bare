#include "timer.h"

/**
 * @brief  初始化定时器2
 * @param  arr：自动重装载值（计数器最大值，范围 0~65535）
 * @param  psc：预分频值（分频系数 = psc + 1，范围 0~65535）
 * @note   定时器6时钟频率 = 84MHz（APB1时钟为42MHz，基本定时器时钟为APB1×2）
 *         定时周期 = (arr + 1) * (psc + 1) / 84000000 (单位：秒)
 */
void TIM2_Init(uint16_t arr, uint16_t psc) {
    // 1. 使能定时器2时钟（挂载在APB1总线）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 2. 配置定时器基本参数
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_Period = arr;               // 自动重装载值
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;            // 预分频值
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      // 时钟不分频（基本定时器无滤波功能，此参数无效）
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;      // 重复计数器（基本定时器无此功能，设为0）
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    // 3. 使能定时器更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // 4. 配置NVIC中断优先级
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;  
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  // 抢占优先级（0~15，值越小优先级越高）
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;     // 子优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;        // 使能中断通道
    NVIC_Init(&NVIC_InitStruct);
}

/**
 * @brief  启动定时器2
 */
void TIM2_Start(void) {
    TIM_Cmd(TIM2, ENABLE);  // 使能定时器6计数器
}

/**
 * @brief  停止定时器2
 */
void TIM2_Stop(void) {
    TIM_Cmd(TIM2, DISABLE);  // 关闭定时器6计数器
}

//void TIM2_IRQHandler(void) {
//    // 检查是否为更新中断
//    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
////        lv_tick_inc(1);
//        static uint16_t ledtime=0;
//		ledtime++;
//		if(ledtime>=500)
//		{
//			GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
//			ledtime=0;
//		}
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  // 清除中断标志位
//    }
//}
