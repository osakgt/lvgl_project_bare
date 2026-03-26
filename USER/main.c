//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F407ZGT6,正点原子Explorer STM32F4开发板,主频168MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 FSMC
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/09
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//此模块可以直接插入正点原子Explorer STM32F4开发板TFTLCD插槽，无需手动接线
//STM32连接引脚是指TFTLCD插槽引脚内部连接的STM32引脚
//=========================================电源接线================================================//
//     LCD模块             TFTLCD插槽引脚        STM32连接引脚
//      VDD       --->         5V/3.3              DC5V/3.3V          //电源
//      GND       --->          GND                  GND              //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为16位并口总线
//     LCD模块             TFTLCD插槽引脚        STM32连接引脚
//      DB0       --->          D0                   PD14        -|   
//      DB1       --->          D1                   PD15         |  
//      DB2       --->          D2                   PD0          | 
//      DB3       --->          D3                   PD1          | 
//      DB4       --->          D4                   PE7          |
//      DB5       --->          D5                   PE8          |
//      DB6       --->          D6                   PE9          |
//      DB7       --->          D7                   PE10         |
//如果使用8位模式，请使用下面高8位并口数据引脚                    |===>液晶屏16位并口数据信号
//      DB8       --->          D8                   PE11         |
//      DB9       --->          D9                   PE12         |
//      DB10      --->          D10                  PE13         |
//      DB11      --->          D11                  PE14         |
//      DB12      --->          D12                  PE15         |
//      DB13      --->          D13                  PD8          |
//      DB14      --->          D14                  PD9          |
//      DB15      --->          D15                  PD10        -|
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 				     TFTLCD插槽引脚        STM32连接引脚 
//       WR       --->          WR                   PD5             //液晶屏写数据控制信号
//       RD       --->          RD                   PD4             //液晶屏读数据控制信号
//       RS       --->          RS                   PF12            //液晶屏数据/命令控制信号
//       RST      --->          RST                复位引脚          //液晶屏复位控制信号
//       CS       --->          CS                   PG12            //液晶屏片选控制信号
//       BL       --->          BL                   PB15            //液晶屏背光控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块             TFTLCD插槽引脚        STM32连接引脚 
//      PEN       --->          PEN                  PB1             //触摸屏触摸中断信号
//      MISO      --->          MISO                 PB2             //触摸屏SPI总线读信号
//      MOSI      --->          MOSI                 PF11            //触摸屏SPI总线写信号
//      T_CS      --->          TCS                  PC13            //触摸屏片选控制信号
//      CLK       --->          CLK                  PB0             //触摸屏SPI总线时钟信号
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/		
#include "main.h"
/*FreeRTOS配置*/

#if 0
/*主程序*/
//uint8_t buffer[5];
//float t,h;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数

	LCD_Init();
	TP_Init();
	USART1_Init();
	LED_Init();
//	TIM2_Init(999,83);
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
//	TIM2_Start();

	lvgl_demo();
	while(1)
	{
		delay_ms(1);
		static uint8_t msLVGL=0;
		if(msLVGL++>=5)
		{
			lv_task_handler();
			msLVGL=0;
		}

	}
}

//void TIM2_IRQHandler(void) {
//    // 检查是否为更新中断
//    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
//        lv_tick_inc(1);
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

void vApplicationTickHook( void ){
	lv_tick_inc(1);
}

#endif

#if 0
/*测试程序*/	
int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	LCD_Init();	   //液晶屏初始化
	LED_Init();
	USART1_Init();
	TIM2_Init(999,83);
	TIM2_Start();
  //循环测试
	while(1)
	{
//		delay_ms(500);
//		main_test(); 		//测试主界面
//		Test_Read();     //读ID和颜色值测试
//		Test_Color();  		//简单刷屏填充测试
//		Test_FillRec();		//GUI矩形绘图测试
//		Test_Circle(); 		//GUI画圆测试
//		Test_Triangle();    //GUI三角形绘图测试
//		English_Font_test();//英文字体示例测试
//		Chinese_Font_test();//中文字体示例测试
//		Pic_test();			//图片显示示例测试
//		Rotate_Test();   //旋转显示测试
		//如果不带触摸，或者不需要触摸功能，请注释掉下面触摸屏测试项
		Touch_Test();		//触摸屏手写测试  
	}
}

#endif

#if 0
/*串口测试*/
lv_ui  guider_ui; 

int main(void)
{
	delay_init(168);     //初始化延时函数
	USART1_Init();
	uint16_t t =12;
	while(1)
	{
		USART_SendString(USART6,"Hello World\r\n");
		delay_ms(1000);
		printf("%d\r\n",t);
		delay_ms(1000);
		
	}

}
#endif

#if 0
/*温湿度读取测试*/

uint8_t buffer[5];
float t,h;
int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	DHT_Start();
	USART1_Init();
  //循环测试
	while(1)
	{	
		if(DHT_ReadData(buffer) == 0)
		{
			h = buffer[0] + buffer[1] / 10.0;
			t = buffer[2] + buffer[3] / 10.0;
			printf("Temp: %05.2f\r\n",t);
			printf("Humdity: %05.2f%%\r\n",h);
			delay_ms(1000);

		}
	}
}

#endif

#if 1
/*lvgl测试*/

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数

	LCD_Init();
	TP_Init();
	LED_Init();
	USART1_Init();
	TIM2_Init(999,83);

	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	
	TIM2_Start();
	
	gui_init();
	
//	    // 按钮
//    lv_obj_t *myBtn = lv_btn_create(lv_scr_act());                               // 创建按钮; 父对象：当前活动屏幕
//    lv_obj_set_pos(myBtn, 20, 10);                                               // 设置坐标
//    lv_obj_set_size(myBtn, 120, 50);                                             // 设置大小
////	lv_obj_add_event_cb(myBtn,myBtn_event,LV_EVENT_CLICKED,NULL);
//	
//    // 按钮上的文本
//    lv_obj_t *label_btn = lv_label_create(myBtn);                                // 创建文本标签，父对象：上面的btn按钮
//    lv_obj_align(label_btn, LV_ALIGN_CENTER, 0, 0);                              // 对齐于：父对象
//    lv_label_set_text(label_btn, "Test");                                        // 设置标签的文本
// 
//    // 独立的标签
//    lv_obj_t *myLabel = lv_label_create(lv_scr_act());                           // 创建文本标签; 父对象：当前活动屏幕
//    lv_label_set_text(myLabel, "Hello world!");                                  // 设置标签的文本
//    lv_obj_align(myLabel, LV_ALIGN_CENTER, 0, 0);                                // 对齐于：父对象
//    lv_obj_align_to(myBtn, myLabel, LV_ALIGN_OUT_TOP_MID, 0, -20);               // 对齐于：某对象
	
	while(1)
	{
		gui_task_handler();
	}
	

}
void TIM2_IRQHandler(void) {
    // 检查是否为更新中断
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        lv_tick_inc(1);
        static uint16_t ledtime=0;
		ledtime++;
		if(ledtime>=500)
		{
			GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
			ledtime=0;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  // 清除中断标志位
    }
}

#endif

#if 0
/*WIFI模块测试*/
lv_ui  guider_ui; 

uint8_t buffer[5];
float t,h;
int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	DHT_Start();
	USART1_Init();
	USART_SendString(USART6,"串口6初始化完成，等待ESP返回数据...\r\n");
	Tcp_Init();
//	TCP_Connect();
	while(1)
	{	

		if(DHT_ReadData(buffer) == 0)
		{
			h = buffer[0] + buffer[1] / 10.0;
			t = buffer[2] + buffer[3] / 10.0;
			delay_ms(1000);
			Tcp_Data_Send(t,h);

		}
	}
}
#endif

#if 0
/*FreeRtos测试*/
void vTask1( void *pvParameters )
{
  for( ;; ) {    
    LED0=0;
    delay_ms(5000);
    LED0=1;
    delay_ms(5000);
  }
}
void vTask2( void *pvParameters )
{
  for( ;; ) {
    LCD_Clear(BLACK);  
    delay_ms(5000);
    LCD_Clear(WHITE);
    delay_ms(5000);
  }
}

static void prvSetupHardware( void )
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  delay_init(168);     
  USART1_Init();   
  LED_Init(); 
  LCD_Init();   
}

int main(void)
{ 
  prvSetupHardware();
  
  xTaskCreate(vTask1, "Task 1", 1000, NULL, 1, NULL);
  xTaskCreate(vTask2, "Task 2", 1000, NULL, 1, NULL);
  
  vTaskStartScheduler();
}

#endif
