#ifndef APP_TOUCH_H
#define APP_TOUCH_H

#include "bsp_touch.h"

void app_initialize_touchscreen();

void app_initialize_touchscreen_task();

void app_start_touchscreen();

void app_get_latest_touch_data(bsp_touch_data_t *touch_data);

#endif