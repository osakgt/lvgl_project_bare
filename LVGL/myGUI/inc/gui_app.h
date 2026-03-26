#ifndef __GUI_APP_H
#define __GUI_APP_H

#include "lvgl.h"
#include <stdint.h>

// GUI初始化函数
void gui_init(void);

// GUI任务处理函数（在main中调用）
void gui_task_handler(void);

// 界面枚举
typedef enum {
    GUI_SCREEN_MAIN,          // 主界面
    GUI_SCREEN_MENU,          // 菜单界面
    GUI_SCREEN_SETTINGS,      // 设置界面
    GUI_SCREEN_SPORTS,       // 运动界面
    GUI_SCREEN_HEART_RATE,   // 心率界面
    GUI_SCREEN_SLEEP,         // 睡眠界面
    GUI_SCREEN_NOTIFICATION,  // 通知界面
    GUI_SCREEN_CLOCK,         // 时钟界面
    GUI_SCREEN_WEATHER,      // 天气界面
    GUI_SCREEN_BATTERY,      // 电量界面
    GUI_SCREEN_ACTIVITY,      // 活动数据界面
} gui_screen_t;

// 运动类型枚举
typedef enum {
    SPORT_RUNNING,
    SPORT_WALKING,
    SPORT_CYCLING,
    SWIMMING,
    SPORT_COUNT,
} sport_type_t;

// 通知消息结构
typedef struct {
    uint8_t id;
    uint8_t type;          // 0: 消息 1: 电话 2: 邮件 3: 闹钟
    char title[32];
    char content[64];
    uint32_t timestamp;
} notification_t;

// 系统状态结构
typedef struct {
    uint8_t battery_level;     // 电量百分比
    uint8_t heart_rate;        // 心率
    uint8_t steps;             // 步数
    uint32_t distance;         // 距离(米)
    uint32_t calories;         // 卡路里
    uint8_t sleep_hours;       // 睡眠小时数
    uint8_t weather_temp;      // 温度
    uint8_t weather_humidity;  // 湿度
    uint8_t notification_count; // 通知数量
    uint8_t current_sport;     // 当前运动类型
    uint8_t sport_duration;    // 运动时长(分钟)
} system_status_t;

// 获取系统状态
extern system_status_t g_system_status;

// LVGL对象宏定义
#define LV_SCREEN_MAIN_ID         0
#define LV_SCREEN_MENU_ID          1
#define LV_SCREEN_SETTINGS_ID      2
#define LV_SCREEN_SPORTS_ID        3
#define LV_SCREEN_HEART_RATE_ID    4
#define LV_SCREEN_SLEEP_ID         5
#define LV_SCREEN_NOTIFICATION_ID   6
#define LV_SCREEN_CLOCK_ID         7
#define LV_SCREEN_WEATHER_ID       8
#define LV_SCREEN_BATTERY_ID       9
#define LV_SCREEN_ACTIVITY_ID      10

// 按件对象ID
#define LV_BTN_MENU_ID          100
#define LV_BTN_SETTINGS_ID      101
#define LV_BTN_SPORTS_ID        102
#define LV_BTN_HEART_RATE_ID    103
#define LV_BTN_SLEEP_ID         104
#define LV_BTN_NOTIFICATION_ID  105
#define LV_BTN_CLOCK_ID         106
#define LV_BTN_WEATHER_ID       107
#define LV_BTN_BATTERY_ID       108
#define LV_BTN_ACTIVITY_ID      109
#define LV_BTN_BACK_ID          110

// 标签对象ID
#define LV_LABEL_TIME_ID         200
#define LV_LABEL_DATE_ID         201
#define LV_LABEL_BATTERY_ID      202
#define LV_LABEL_STEPS_ID        203
#define LV_LABEL_HEART_RATE_ID   204
#define LV_LABEL_DISTANCE_ID     205
#define LV_LABEL_CALORIES_ID     206
#define LV_LABEL_SLEEP_ID        207
#define LV_LABEL_NOTIFICATION_ID 208
#define LV_LABEL_WEATHER_ID      209
#define LV_LABEL_SPORT_ID        210
#define LV_LABEL_DURATION_ID     211

// 图表对象ID
#define LV_CHART_HEART_ID        300
#define LV_CHART_STEPS_ID       301

// 滑动指示器ID
#define LV_INDICATOR_ID          400

#endif /* __GUI_H */

