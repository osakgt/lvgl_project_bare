#include "gui_app.h"
#include <string.h>
#include <stdlib.h>


// 系统全局变量
system_status_t g_system_status = {
    .battery_level = 85,
    .heart_rate = 72,
    .steps = 8543,
    .distance = 6543,
    .calories = 312,
    .sleep_hours = 7,
    .weather_temp = 22,
    .weather_humidity = 65,
    .notification_count = 3,
    .current_sport = SPORT_RUNNING,
    .sport_duration = 45,
};

// LVGL屏幕对象
static lv_obj_t *screens[GUI_SCREEN_ACTIVITY + 1];
static lv_obj_t *current_screen = NULL;

// 滑动提示标签
static lv_obj_t *swipe_hint_label;

// 自定义样式
static lv_style_t style_btn;
static lv_style_t style_btn_pressed;
static lv_style_t style_card;
static lv_style_t style_title;
static lv_style_t style_text;

// 主界面元素
static lv_obj_t *main_time_label;
static lv_obj_t *main_date_label;
static lv_obj_t *main_battery_label;
static lv_obj_t *main_steps_label;
static lv_obj_t *main_heart_label;
static lv_obj_t *main_distance_label;
static lv_obj_t *main_calories_label;
static lv_obj_t *main_sleep_label;
static lv_obj_t *main_notification_label;
static lv_obj_t *main_sport_label;

// 心率历史数据（模拟）
static uint8_t heart_history[20] = {72, 75, 73, 76, 74, 77, 75, 76, 74, 78, 76, 75, 77, 74, 76, 75, 78, 76, 74, 75};

// 运动数据（模拟）
static uint16_t steps_history[7] = {8234, 9456, 7654, 10234, 8765, 9543, 8543};

// 模拟通知数据
static notification_t notifications[5] = {
    {1, 0, "新消息", "您有一条新消息", 1712345678},
    {2, 1, "电话", "来自张三的未接来电", 1712345600},
    {3, 3, "闹钟", "起床时间到了", 1712344500},
};

// 按钮事件回调
static void btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED) {
        uint32_t btn_id = (uint32_t)(uintptr_t)lv_obj_get_user_data(btn);

        switch(btn_id) {
            case LV_BTN_MENU_ID:
                lv_scr_load(screens[GUI_SCREEN_MENU]);
                break;
            case LV_BTN_SETTINGS_ID:
                lv_scr_load(screens[GUI_SCREEN_SETTINGS]);
                break;
            case LV_BTN_SPORTS_ID:
                lv_scr_load(screens[GUI_SCREEN_SPORTS]);
                break;
            case LV_BTN_HEART_RATE_ID:
                lv_scr_load(screens[GUI_SCREEN_HEART_RATE]);
                break;
            case LV_BTN_SLEEP_ID:
                lv_scr_load(screens[GUI_SCREEN_SLEEP]);
                break;
            case LV_BTN_NOTIFICATION_ID:
                lv_scr_load(screens[GUI_SCREEN_NOTIFICATION]);
                break;
            case LV_BTN_CLOCK_ID:
                lv_scr_load(screens[GUI_SCREEN_CLOCK]);
                break;
            case LV_BTN_WEATHER_ID:
                lv_scr_load(screens[GUI_SCREEN_WEATHER]);
                break;
            case LV_BTN_BATTERY_ID:
                lv_scr_load(screens[GUI_SCREEN_BATTERY]);
                break;
            case LV_BTN_ACTIVITY_ID:
                lv_scr_load(screens[GUI_SCREEN_ACTIVITY]);
                break;
            case LV_BTN_BACK_ID:
                lv_scr_load(screens[GUI_SCREEN_MAIN]);
                break;
        }
    }
}

// 创建按钮
static lv_obj_t *create_btn(lv_obj_t *parent, int32_t x, int32_t y, uint32_t btn_id, const char *text)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 60, 60);
    lv_obj_set_pos(btn, x, y);
    lv_obj_add_style(btn, &style_btn, 0);
    lv_obj_add_style(btn, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_user_data(btn, (void*)btn_id);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    return btn;
}

// 创建信息卡片
static lv_obj_t *create_card(lv_obj_t *parent, int32_t x, int32_t y, const char *title, const char *value)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_size(card, 120, 80);
    lv_obj_set_pos(card, x, y);
    lv_obj_set_style_bg_color(card, lv_color_hex(0xf0f0f0), 0);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, lv_color_hex(0xe0e0e0), 0);
    lv_obj_set_style_radius(card, 10, 0);

    lv_obj_t *title_label = lv_label_create(card);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0x666666), 0);
    lv_label_set_text(title_label, title);
    lv_obj_align(title_label, LV_ALIGN_TOP_LEFT, 10, 10);

    lv_obj_t *value_label = lv_label_create(card);
    lv_obj_set_style_text_font(value_label, &lv_font_montserrat_20, 0);
    lv_label_set_text(value_label, value);
    lv_obj_align(value_label, LV_ALIGN_CENTER, 0, 10);

    // 返回卡片对象，而不是内部标签
    return card;
}

// 创建主界面
static void create_main_screen(void)
{
    screens[GUI_SCREEN_MAIN] = lv_obj_create(NULL);
    lv_obj_set_size(screens[GUI_SCREEN_MAIN], 320, 240);

    // 背景
    lv_obj_set_style_bg_color(screens[GUI_SCREEN_MAIN], lv_color_hex(0x2c3e50), 0);
    lv_obj_set_style_bg_grad_color(screens[GUI_SCREEN_MAIN], lv_color_hex(0x34495e), LV_GRAD_DIR_VER);

    // 标题栏
    lv_obj_t *header = lv_obj_create(screens[GUI_SCREEN_MAIN]);
    lv_obj_set_size(header, 320, 40);
    lv_obj_set_pos(header, 0, 0);
    lv_obj_set_style_bg_color(header, lv_color_hex(0x34495e), 0);
    lv_obj_set_style_radius(header, 0, 0);

    lv_obj_t *time_label = lv_label_create(header);
    lv_obj_set_style_text_color(time_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_18, 0);
    lv_label_set_text(time_label, "10:30");
    lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 10, 0);
    main_time_label = time_label;

    lv_obj_t *date_label = lv_label_create(header);
    lv_obj_set_style_text_color(date_label, lv_color_hex(0xbdc3c7), 0);
    lv_label_set_text(date_label, "2024-01-15");
    lv_obj_align(date_label, LV_ALIGN_RIGHT_MID, -10, 0);
    main_date_label = date_label;

    // 状态栏
    lv_obj_t *status_bar = lv_obj_create(screens[GUI_SCREEN_MAIN]);
    lv_obj_set_size(status_bar, 320, 40);
    lv_obj_set_pos(status_bar, 0, 40);
    lv_obj_set_style_bg_color(status_bar, lv_color_hex(0x34495e), 0);
    lv_obj_set_style_radius(status_bar, 0, 0);

    lv_obj_t *battery_card = create_card(status_bar, 10, 5, "电量", "85%");
    main_battery_label = lv_obj_get_child(battery_card, 1);

    lv_obj_t *steps_card = create_card(status_bar, 80, 5, "步数", "8543");
    main_steps_label = lv_obj_get_child(steps_card, 1);

    lv_obj_t *heart_card = create_card(status_bar, 150, 5, "心率", "72");
    main_heart_label = lv_obj_get_child(heart_card, 1);

    lv_obj_t *notification_card = create_card(status_bar, 220, 5, "通知", "3");
    main_notification_label = lv_obj_get_child(notification_card, 1);

    // 主内容区域
    lv_obj_t *main_content = lv_obj_create(screens[GUI_SCREEN_MAIN]);
    lv_obj_set_size(main_content, 320, 160);
    lv_obj_set_pos(main_content, 0, 80);

    lv_obj_t *distance_card = create_card(main_content, 10, 20, "距离", "6.5km");
    main_distance_label = lv_obj_get_child(distance_card, 1);

    lv_obj_t *calories_card = create_card(main_content, 110, 20, "卡路里", "312");
    main_calories_label = lv_obj_get_child(calories_card, 1);

    lv_obj_t *sleep_card = create_card(main_content, 210, 20, "睡眠", "7h");
    main_sleep_label = lv_obj_get_child(sleep_card, 1);

    lv_obj_t *sport_card = create_card(main_content, 10, 100, "运动", "45min");
    main_sport_label = lv_obj_get_child(sport_card, 1);

    // 底部导航
    lv_obj_t *nav_bar = lv_obj_create(screens[GUI_SCREEN_MAIN]);
    lv_obj_set_size(nav_bar, 320, 40);
    lv_obj_set_pos(nav_bar, 0, 200);
    lv_obj_set_style_bg_color(nav_bar, lv_color_hex(0x34495e), 0);
    lv_obj_set_style_radius(nav_bar, 0, 0);

    create_btn(nav_bar, 10, 0, LV_BTN_MENU_ID, LV_SYMBOL_MENU);
    create_btn(nav_bar, 80, 0, LV_BTN_SETTINGS_ID, LV_SYMBOL_SETTINGS);
    create_btn(nav_bar, 150, 0, LV_BTN_SPORTS_ID, LV_SYMBOL_PLAY);
    create_btn(nav_bar, 220, 0, LV_BTN_NOTIFICATION_ID, LV_SYMBOL_BELL);
}

// 创建菜单界面
static void create_menu_screen(void)
{
    screens[GUI_SCREEN_MENU] = lv_obj_create(NULL);
    lv_obj_set_size(screens[GUI_SCREEN_MENU], 320, 240);

    // 背景
    lv_obj_set_style_bg_color(screens[GUI_SCREEN_MENU], lv_color_hex(0xecf0f1), 0);

    // 返回按钮
    create_btn(screens[GUI_SCREEN_MENU], 10, 10, LV_BTN_BACK_ID, LV_SYMBOL_LEFT);

    // 标题
    lv_obj_t *title = lv_label_create(screens[GUI_SCREEN_MENU]);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_label_set_text(title, "功能菜单");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 50);

    // 菜单项
    lv_obj_t *btn1 = create_btn(screens[GUI_SCREEN_MENU], 110, 100, LV_BTN_SETTINGS_ID, LV_SYMBOL_SETTINGS);
    lv_obj_t *btn1_label = lv_obj_get_child(btn1, 0);
    lv_label_set_text(btn1_label, "设置");

    lv_obj_t *btn2 = create_btn(screens[GUI_SCREEN_MENU], 210, 100, LV_BTN_SPORTS_ID, LV_SYMBOL_PLAY);
    lv_obj_t *btn2_label = lv_obj_get_child(btn2, 0);
    lv_label_set_text(btn2_label, "运动");

    lv_obj_t *btn3 = create_btn(screens[GUI_SCREEN_MENU], 110, 170, LV_BTN_HEART_RATE_ID, LV_SYMBOL_HEART);
    lv_obj_t *btn3_label = lv_obj_get_child(btn3, 0);
    lv_label_set_text(btn3_label, "心率");

    lv_obj_t *btn4 = create_btn(screens[GUI_SCREEN_MENU], 210, 170, LV_BTN_SLEEP_ID, LV_SYMBOL_SLEEP);
    lv_obj_t *btn4_label = lv_obj_get_child(btn4, 0);
    lv_label_set_text(btn4_label, "睡眠");
}

// 创建设置界面
static void create_settings_screen(void)
{
    screens[GUI_SCREEN_SETTINGS] = lv_obj_create(NULL);
    lv_obj_set_size(screens[GUI_SCREEN_SETTINGS], 320, 240);

    // 背景
    lv_obj_set_style_bg_color(screens[GUI_SCREEN_SETTINGS], lv_color_hex(0xecf0f1), 0);

    // 返回按钮
    create_btn(screens[GUI_SCREEN_SETTINGS], 10, 10, LV_BTN_BACK_ID, LV_SYMBOL_LEFT);

    // 标题
    lv_obj_t *title = lv_label_create(screens[GUI_SCREEN_SETTINGS]);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_label_set_text(title, "设置");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 50);

    // 设置项
    lv_obj_t *btn1 = create_btn(screens[GUI_SCREEN_SETTINGS], 110, 100, LV_BTN_BATTERY_ID, LV_SYMBOL_BATTERY_FULL);
    lv_obj_t *btn1_label = lv_obj_get_child(btn1, 0);
    lv_label_set_text(btn1_label, "电量");

    lv_obj_t *btn2 = create_btn(screens[GUI_SCREEN_SETTINGS], 210, 100, LV_BTN_WEATHER_ID, LV_SYMBOL_WEATHER);
    lv_obj_t *btn2_label = lv_obj_get_child(btn2, 0);
    lv_label_set_text(btn2_label, "天气");

    lv_obj_t *btn3 = create_btn(screens[GUI_SCREEN_SETTINGS], 110, 170, LV_BTN_CLOCK_ID, LV_SYMBOL_CLOCK);
    lv_obj_t *btn3_label = lv_obj_get_child(btn3, 0);
    lv_label_set_text(btn3_label, "时钟");

    lv_obj_t *btn4 = create_btn(screens[GUI_SCREEN_SETTINGS], 210, 170, LV_BTN_NOTIFICATION_ID, LV_SYMBOL_BELL);
    lv_obj_t *btn4_label = lv_obj_get_child(btn4, 0);
    lv_label_set_text(btn4_label, "通知");
}

// 创建运动界面
static void create_sports_screen(void)
{
    screens[GUI_SCREEN_SPORTS] = lv_obj_create(NULL);
    lv_obj_set_size(screens[GUI_SCREEN_SPORTS], 320, 240);

    // 背景
    lv_obj_set_style_bg_color(screens[GUI_SCREEN_SPORTS], lv_color_hex(0xecf0f1), 0);

    // 返回按钮
    create_btn(screens[GUI_SCREEN_SPORTS], 10, 10, LV_BTN_BACK_ID, LV_SYMBOL_LEFT);

    // 标题
    lv_obj_t *title = lv_label_create(screens[GUI_SCREEN_SPORTS]);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_label_set_text(title, "运动模式");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 50);

    // 运动模式按钮
    const char *sports_icon[] = {LV_SYMBOL_PLAY, LV_SYMBOL_WALK, LV_SYMBOL_BIKE, LV_SYMBOL_WATER};
    const char *sports_name[] = {"跑步", "步行", "骑行", "游泳"};

    for (int i = 0; i < 4; i++) {
        lv_obj_t *btn = create_btn(screens[GUI_SCREEN_SPORTS], 40 + (i % 2) * 120, 100 + (i / 2) * 70, LV_BTN_SPORTS_ID + i, sports_icon[i]);
        lv_obj_t *label = lv_obj_get_child(btn, 0);
        lv_label_set_text(label, sports_name[i]);
    }
}

// 创建心率界面
static void create_heart_rate_screen(void)
{
    screens[GUI_SCREEN_HEART_RATE] = lv_obj_create(NULL);
    lv_obj_set_size(screens[GUI_SCREEN_HEART_RATE], 320, 240);

    // 背景
    lv_obj_set_style_bg_color(screens[GUI_SCREEN_HEART_RATE], lv_color_hex(0xe74c3c), 0);

    // 返回按钮
    create_btn(screens[GUI_SCREEN_HEART_RATE], 10, 10, LV_BTN_BACK_ID, LV_SYMBOL_LEFT);

    // 标题
    lv_obj_t *title = lv_label_create(screens[GUI_SCREEN_HEART_RATE]);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_label_set_text(title, "心率监测");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // 当前心率显示
    lv_obj_t *heart_display = lv_obj_create(screens[GUI_SCREEN_HEART_RATE]);
    lv_obj_set_size(heart_display, 200, 60);
    lv_obj_set_pos(heart_display, 60, 70);
    lv_obj_set_style_bg_color(heart_display, lv_color_hex(0xc0392b), 0);
    lv_obj_set_style_radius(heart_display, 30, 0);

    lv_obj_t *heart_label = lv_label_create(heart_display);
    lv_obj_set_style_text_font(heart_label, &lv_font_montserrat_36, 0);
    lv_obj_set_style_text_color(heart_label, lv_color_white(), 0);
    lv_label_set_text(heart_label, "72");
    lv_obj_align(heart_label, LV_ALIGN_CENTER, 0, -10);

    lv_obj_t *unit_label = lv_label_create(heart_display);
    lv_obj_set_style_text_color(unit_label, lv_color_hex(0xecf0f1), 0);
    lv_label_set_text(unit_label, "bpm");
    lv_obj_align(unit_label, LV_ALIGN_BOTTOM_MID, 0, -10);

    // 心率图表
    lv_obj_t *chart = lv_chart_create(screens[GUI_SCREEN_HEART_RATE]);
    lv_obj_set_size(chart, 280, 80);
    lv_obj_set_pos(chart, 20, 150);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
//    lv_chart_set_series_width(chart, 2, 0);
//    lv_chart_set_series_bg_color(chart, lv_color_hex(0xecf0f1), 0);
	lv_chart_series_t *series = lv_chart_add_series(chart,lv_color_hex3(0xFF0000),LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_series_color(chart,series ,lv_color_white());

    // 添加历史数据点
    for (int i = 0; i < 20; i++) {
        lv_chart_set_next_value(chart,series, heart_history[i]);
    }
}

// 创建睡眠界面
static void create_sleep_screen(void)
{
    screens[GUI_SCREEN_SLEEP] = lv_obj_create(NULL);
    lv_obj_set_size(screens[GUI_SCREEN_SLEEP], 320, 240);

    // 背景
    lv_obj_set_style_bg_color(screens[GUI_SCREEN_SLEEP], lv_color_hex(0x9b59b6), 0);

    // 返回按钮
    create_btn(screens[GUI_SCREEN_SLEEP], 10, 10, LV_BTN_BACK_ID, LV_SYMBOL_LEFT);

    // 标题
    lv_obj_t *title = lv_label_create(screens[GUI_SCREEN_SLEEP]);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_label_set_text(title, "睡眠监测");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // 睡眠时间显示
    lv_obj_t *sleep_card = lv_obj_create(screens[GUI_SCREEN_SLEEP]);
    lv_obj_set_size(sleep_card, 150, 60);
    lv_obj_set_pos(sleep_card, 85, 70);
    lv_obj_set_style_bg_color(sleep_card, lv_color_hex(0x8e44ad), 0);
    lv_obj_set_style_radius(sleep_card, 30, 0);

    lv_obj_t *sleep_label = lv_label_create(sleep_card);
    lv_obj_set_style_text_font(sleep_label, &lv_font_montserrat_36, 0);
    lv_obj_set_style_text_color(sleep_label, lv_color_white(), 0);
    lv_label_set_text(sleep_label, "7");
    lv_obj_align(sleep_label, LV_ALIGN_CENTER, 0, -10);

    lv_obj_t *unit_label = lv_label_create(sleep_card);
    lv_obj_set_style_text_color(unit_label, lv_color_hex(0xecf0f1), 0);
    lv_label_set_text(unit_label, "小时");
    lv_obj_align(unit_label, LV_ALIGN_BOTTOM_MID, 0, -10);

    // 睡眠质量信息
    lv_obj_t *quality_label = lv_label_create(screens[GUI_SCREEN_SLEEP]);
    lv_obj_set_style_text_color(quality_label, lv_color_hex(0xecf0f1), 0);
    lv_label_set_text(quality_label, "睡眠质量: 良好");
    lv_obj_align(quality_label, LV_ALIGN_TOP_MID, 0, 140);

    lv_obj_t *deep_label = lv_label_create(screens[GUI_SCREEN_SLEEP]);
    lv_obj_set_style_text_color(deep_label, lv_color_hex(0xecf0f1), 0);
    lv_label_set_text(deep_label, "深度睡眠: 2.5小时");
    lv_obj_align(deep_label, LV_ALIGN_TOP_MID, 0, 165);

    lv_obj_t *light_label = lv_label_create(screens[GUI_SCREEN_SLEEP]);
    lv_obj_set_style_text_color(light_label, lv_color_hex(0xecf0f1), 0);
    lv_label_set_text(light_label, "浅度睡眠: 4.5小时");
    lv_obj_align(light_label, LV_ALIGN_TOP_MID, 0, 190);
}

// 创建通知界面
static void create_notification_screen(void)
{
    screens[GUI_SCREEN_NOTIFICATION] = lv_obj_create(NULL);
    lv_obj_set_size(screens[GUI_SCREEN_NOTIFICATION], 320, 240);

    // 背景
    lv_obj_set_style_bg_color(screens[GUI_SCREEN_NOTIFICATION], lv_color_hex(0xecf0f1), 0);

    // 返回按钮
    create_btn(screens[GUI_SCREEN_NOTIFICATION], 10, 10, LV_BTN_BACK_ID, LV_SYMBOL_LEFT);

    // 标题
    lv_obj_t *title = lv_label_create(screens[GUI_SCREEN_NOTIFICATION]);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_label_set_text(title, "通知");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // 通知列表
    for (int i = 0; i < 3; i++) {
        lv_obj_t *noti_card = lv_obj_create(screens[GUI_SCREEN_NOTIFICATION]);
        lv_obj_set_size(noti_card, 280, 50);
        lv_obj_set_pos(noti_card, 20, 60 + i * 55);
        lv_obj_set_style_bg_color(noti_card, lv_color_white(), 0);
        lv_obj_set_style_border_width(noti_card, 1, 0);
        lv_obj_set_style_border_color(noti_card, lv_color_hex(0xbdc3c7), 0);
        lv_obj_set_style_radius(noti_card, 5, 0);

        lv_obj_t *icon = lv_label_create(noti_card);
        lv_obj_set_style_text_font(icon, &lv_font_montserrat_20, 0);
        if (notifications[i].type == 0) {
            lv_label_set_text(icon, LV_SYMBOL_EMAIL);
        } else if (notifications[i].type == 1) {
            lv_label_set_text(icon, LV_SYMBOL_CALL);
        } else if (notifications[i].type == 3) {
            lv_label_set_text(icon, LV_SYMBOL_ALARM);
        }
        lv_obj_align(icon, LV_ALIGN_LEFT_MID, 10, 0);

        lv_obj_t *content = lv_label_create(noti_card);
        lv_obj_set_style_text_font(content, &lv_font_montserrat_14, 0);
        lv_label_set_text(content, notifications[i].title);
        lv_obj_align(content, LV_ALIGN_LEFT_MID, 35, 0);
    }
}

// 创建时钟界面
static void create_clock_screen(void)
{
    screens[GUI_SCREEN_CLOCK] = lv_obj_create(NULL);
    lv_obj_set_size(screens[GUI_SCREEN_CLOCK], 320, 240);

    // 背景
    lv_obj_set_style_bg_color(screens[GUI_SCREEN_CLOCK], lv_color_hex(0x2c3e50), 0);
    lv_obj_set_style_bg_grad_color(screens[GUI_SCREEN_CLOCK], lv_color_hex(0x34495e), LV_GRAD_DIR_VER);

    // 返回按钮
    create_btn(screens[GUI_SCREEN_CLOCK], 10, 10, LV_BTN_BACK_ID, LV_SYMBOL_LEFT);

    // 时间显示
    lv_obj_t *time_display = lv_obj_create(screens[GUI_SCREEN_CLOCK]);
    lv_obj_set_size(time_display, 200, 80);
    lv_obj_set_pos(time_display, 60, 60);
    lv_obj_set_style_bg_opa(time_display, LV_OPA_TRANSP, 0);

    lv_obj_t *time_label = lv_label_create(time_display);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(time_label, lv_color_white(), 0);
    lv_label_set_text(time_label, "10:30");
    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, -10);

    lv_obj_t *date_label = lv_label_create(time_display);
    lv_obj_set_style_text_font(date_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(date_label, lv_color_hex(0xbdc3c7), 0);
    lv_label_set_text(date_label, "2024年01月15日 周一");
    lv_obj_align(date_label, LV_ALIGN_BOTTOM_MID, 0, 10);

    // 时区显示
    lv_obj_t *timezone_label = lv_label_create(screens[GUI_SCREEN_CLOCK]);
    lv_obj_set_style_text_color(timezone_label, lv_color_hex(0x95a5a6), 0);
    lv_label_set_text(timezone_label, "GMT+8 北京");
    lv_obj_align(timezone_label, LV_ALIGN_BOTTOM_MID, 0, 10);
}

// 创建天气界面
static void create_weather_screen(void)
{
    screens[GUI_SCREEN_WEATHER] = lv_obj_create(NULL);
    lv_obj_set_size(screens[GUI_SCREEN_WEATHER], 320, 240);

    // 背景
    lv_obj_set_style_bg_color(screens[GUI_SCREEN_WEATHER], lv_color_hex(0x3498db), 0);
    lv_obj_set_style_bg_grad_color(screens[GUI_SCREEN_WEATHER], lv_color_hex(0x2980b9), LV_GRAD_DIR_VER);

    // 返回按钮
    create_btn(screens[GUI_SCREEN_WEATHER], 10, 10, LV_BTN_BACK_ID, LV_SYMBOL_LEFT);

    // 天气图标
    lv_obj_t *weather_icon = lv_label_create(screens[GUI_SCREEN_WEATHER]);
    lv_obj_set_style_text_font(weather_icon, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(weather_icon, lv_color_white(), 0);
    lv_label_set_text(weather_icon, LV_SYMBOL_SUN);
    lv_obj_align(weather_icon, LV_ALIGN_TOP_MID, 0, 60);

    // 温度显示
    lv_obj_t *temp_label = lv_label_create(screens[GUI_SCREEN_WEATHER]);
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_36, 0);
    lv_obj_set_style_text_color(temp_label, lv_color_white(), 0);
    lv_label_set_text(temp_label, "22°C");
    lv_obj_align(temp_label, LV_ALIGN_TOP_MID, 0, 110);

    // 天气描述
    lv_obj_t *desc_label = lv_label_create(screens[GUI_SCREEN_WEATHER]);
    lv_obj_set_style_text_color(desc_label, lv_color_hex(0xecf0f1), 0);
    lv_label_set_text(desc_label, "晴朗");
    lv_obj_align(desc_label, LV_ALIGN_TOP_MID, 0, 145);

    // 湿度信息
    lv_obj_t *humidity_label = lv_label_create(screens[GUI_SCREEN_WEATHER]);
    lv_obj_set_style_text_color(humidity_label, lv_color_hex(0xecf0f1), 0);
    lv_label_set_text(humidity_label, "湿度: 65%");
    lv_obj_align(humidity_label, LV_ALIGN_BOTTOM_MID, 0, 20);
}

// 创建电量界面
static void create_battery_screen(void)
{
    screens[GUI_SCREEN_BATTERY] = lv_obj_create(NULL);
    lv_obj_set_size(screens[GUI_SCREEN_BATTERY], 320, 240);

    // 背景
    lv_obj_set_style_bg_color(screens[GUI_SCREEN_BATTERY], lv_color_hex(0x27ae60), 0);

    // 返回按钮
    create_btn(screens[GUI_SCREEN_BATTERY], 10, 10, LV_BTN_BACK_ID, LV_SYMBOL_LEFT);

    // 电量显示
    lv_obj_t *battery_container = lv_obj_create(screens[GUI_SCREEN_BATTERY]);
    lv_obj_set_size(battery_container, 250, 120);
    lv_obj_set_pos(battery_container, 35, 40);
    lv_obj_set_style_bg_color(battery_container, lv_color_hex(0x2ecc71), 0);
    lv_obj_set_style_radius(battery_container, 20, 0);

    // 电量条背景
    lv_obj_t *battery_bg = lv_obj_create(battery_container);
    lv_obj_set_size(battery_bg, 210, 60);
    lv_obj_set_pos(battery_bg, 20, 30);
    lv_obj_set_style_bg_color(battery_bg, lv_color_hex(0x27ae60), 0);
    lv_obj_set_style_radius(battery_bg, 10, 0);

    // 电量条前景
    lv_obj_t *battery_fg = lv_obj_create(battery_container);
    lv_obj_set_size(battery_fg, 178, 60);  // 85% of 210
    lv_obj_set_pos(battery_fg, 20, 30);
    lv_obj_set_style_bg_color(battery_fg, lv_color_hex(0xf1c40f), 0);
    lv_obj_set_style_radius(battery_fg, 10, 0);

    // 电量百分比
    lv_obj_t *battery_percent = lv_label_create(battery_container);
    lv_obj_set_style_text_font(battery_percent, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(battery_percent, lv_color_white(), 0);
    lv_label_set_text(battery_percent, "85%");
    lv_obj_align(battery_percent, LV_ALIGN_CENTER, 0, 0);

    // 续航时间
    lv_obj_t *续航_label = lv_label_create(screens[GUI_SCREEN_BATTERY]);
    lv_obj_set_style_text_color(续航_label, lv_color_white(), 0);
    lv_label_set_text(续航_label, "预计续航: 18小时");
    lv_obj_align(续航_label, LV_ALIGN_BOTTOM_MID, 0, 20);
}

// 创建活动数据界面
static void create_activity_screen(void)
{
    screens[GUI_SCREEN_ACTIVITY] = lv_obj_create(NULL);
    lv_obj_set_size(screens[GUI_SCREEN_ACTIVITY], 320, 240);

    // 背景
    lv_obj_set_style_bg_color(screens[GUI_SCREEN_ACTIVITY], lv_color_hex(0xecf0f1), 0);

    // 返回按钮
    create_btn(screens[GUI_SCREEN_ACTIVITY], 10, 10, LV_BTN_BACK_ID, LV_SYMBOL_LEFT);

    // 标题
    lv_obj_t *title = lv_label_create(screens[GUI_SCREEN_ACTIVITY]);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_label_set_text(title, "活动数据");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // 步数图表
    lv_obj_t *steps_chart = lv_chart_create(screens[GUI_SCREEN_ACTIVITY]);
    lv_obj_set_size(steps_chart, 280, 150);
    lv_obj_set_pos(steps_chart, 20, 50);
    lv_chart_set_type(steps_chart, LV_CHART_TYPE_BAR);
    lv_chart_set_range(steps_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 12000);
    lv_chart_set_div_line_count(steps_chart, 6, 5);
	
	lv_chart_series_t *series1 = lv_chart_add_series(steps_chart,lv_color_hex3(0x3333FF),LV_CHART_AXIS_PRIMARY_Y);

    // 添加步数数据
    for (int i = 0; i < 7; i++) {
        lv_chart_set_next_value(steps_chart, series1, steps_history[i]);
    }

    // 图表标签
    lv_obj_t *chart_label = lv_label_create(screens[GUI_SCREEN_ACTIVITY]);
    lv_obj_set_style_text_color(chart_label, lv_color_hex(0x7f8c8d), 0);
    lv_label_set_text(chart_label, "过去7天步数统计");
    lv_obj_align(chart_label, LV_ALIGN_BOTTOM_MID, 0, 10);
}

// 更新数据函数
static void update_data(void)
{
    // 更新主界面数据
    char buffer[32];

    // 电量
    sprintf(buffer, "%d%%", g_system_status.battery_level);
    lv_label_set_text(main_battery_label, buffer);

    // 步数
    sprintf(buffer, "%d", g_system_status.steps);
    lv_label_set_text(main_steps_label, buffer);

    // 心率
    sprintf(buffer, "%d", g_system_status.heart_rate);
    lv_label_set_text(main_heart_label, buffer);

    // 通知数量
    sprintf(buffer, "%d", g_system_status.notification_count);
    lv_label_set_text(main_notification_label, buffer);

    // 距离
    sprintf(buffer, "%.1fkm", g_system_status.distance / 1000.0);
    lv_label_set_text(main_distance_label, buffer);

    // 卡路里
    sprintf(buffer, "%d", g_system_status.calories);
    lv_label_set_text(main_calories_label, buffer);

    // 睡眠
    sprintf(buffer, "%dh", g_system_status.sleep_hours);
    lv_label_set_text(main_sleep_label, buffer);

    // 运动
    sprintf(buffer, "%dmin", g_system_status.sport_duration);
    lv_label_set_text(main_sport_label, buffer);

    // 模拟数据更新
    g_system_status.heart_rate += (rand() % 5) - 2;  // 心率小幅度波动
    if (g_system_status.heart_rate < 60) g_system_status.heart_rate = 60;
    if (g_system_status.heart_rate > 100) g_system_status.heart_rate = 100;

    g_system_status.steps += rand() % 10;  // 步数增加

    g_system_status.battery_level -= 1;  // 电量缓慢下降
    if (g_system_status.battery_level < 0) g_system_status.battery_level = 0;
}

// 初始化GUI
void gui_init(void)
{
    // 初始化LVGL
    lv_init();

    // 初始化自定义样式
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 15);
    lv_style_set_bg_color(&style_btn, lv_color_hex(0x3498db));
    lv_style_set_bg_grad_color(&style_btn, lv_color_hex(0x2980b9));
    lv_style_set_bg_grad_dir(&style_btn, LV_GRAD_DIR_VER);
    lv_style_set_border_width(&style_btn, 0);
    lv_style_set_text_color(&style_btn, lv_color_white());
    lv_style_set_shadow_width(&style_btn, 8);
    lv_style_set_shadow_color(&style_btn, lv_color_hex(0x2980b9));
    lv_style_set_shadow_opa(&style_btn, LV_OPA_50);

    lv_style_init(&style_btn_pressed);
    lv_style_set_bg_color(&style_btn_pressed, lv_color_hex(0x2980b9));
    lv_style_set_bg_grad_color(&style_btn_pressed, lv_color_hex(0x1f618d));

    lv_style_init(&style_card);
    lv_style_set_bg_color(&style_card, lv_color_hex(0xf8f9fa));
    lv_style_set_border_width(&style_card, 1);
    lv_style_set_border_color(&style_card, lv_color_hex(0xdee2e6));
    lv_style_set_radius(&style_card, 10);
    lv_style_set_shadow_width(&style_card, 3);
    lv_style_set_shadow_color(&style_card, lv_color_hex(0x000000));
    lv_style_set_shadow_opa(&style_card, LV_OPA_20);

    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_title, lv_color_hex(0x2c3e50));

    lv_style_init(&style_text);
    lv_style_set_text_font(&style_text, &lv_font_montserrat_16);
    lv_style_set_text_color(&style_text, lv_color_hex(0x34495e));

    // 创建所有屏幕
    create_main_screen();
    create_menu_screen();
    create_settings_screen();
    create_sports_screen();
    create_heart_rate_screen();
    create_sleep_screen();
    create_notification_screen();
    create_clock_screen();
    create_weather_screen();
    create_battery_screen();
    create_activity_screen();

    // 加载主界面
    lv_scr_load(screens[GUI_SCREEN_MAIN]);
}

// GUI任务处理函数
void gui_task_handler(void)
{
    // 更新LVGL
    lv_timer_handler();

    // 定期更新数据
    static uint32_t last_update = 0;
    if (lv_tick_get() - last_update > 1000) {  // 每秒更新一次
        update_data();
        last_update = lv_tick_get();
    }
}