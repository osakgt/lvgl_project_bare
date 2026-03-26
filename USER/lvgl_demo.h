#ifndef __LVGL_DEMO_H
#define __LVGL_DEMO_H

#include "lcd.h"
#include "led.h"
#include "touch.h"
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "dht11.h"
#include "semphr.h"
#include "queue.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "gui_app.h"

void lvgl_demo(void);

#endif
