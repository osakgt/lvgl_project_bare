 
#include "lvgl_demo.h"



#if 1
static TaskHandle_t           start_handler;
//static SemaphoreHandle_t 	xMutex;
 
void lvgl_task(void *pvParameter)
{

	gui_init();
	
	while(1)
	{
			lv_task_handler();
			vTaskDelay(pdMS_TO_TICKS(5));

	}
}
 
//void led_task(void *pvParameter)
//{
//	
//	while(1)
//	{
//			GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
//		vTaskDelay(pdMS_TO_TICKS(500));
//	}
//}

void start_task(void *pvParameter) 
{
//		xMutex = xSemaphoreCreateMutex();

    taskENTER_CRITICAL();		
		printf("start123\r\n");
		
    
		xTaskCreate(lvgl_task, "lvgl_task", 4096, NULL, 3, NULL);
//		xTaskCreate(led_task, "led_task", 128, NULL, 2, NULL);
		
    vTaskDelete(start_handler);
 
    taskEXIT_CRITICAL();
}
 
void lvgl_demo(void)
{
	xTaskCreate(start_task, "start_task", 512, NULL, 4, &start_handler);
    vTaskStartScheduler();

}
#endif
