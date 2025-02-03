#include "include/app_touch.h"

void app_initialize_touchscreen(){
    bsp_initialize_touchscreen();
}

void app_initialize_touchscreen_task(){
    bsp_initialize_touchscreen_task();
}

void app_start_touchscreen(){
    app_initialize_touchscreen();
    app_initialize_touchscreen_task();
}

void app_get_latest_touch_data(bsp_touch_data_t *touch_data){
    bsp_touch_data_t data;

    if(bsp_get_latest_touch_data(&data)){
        *touch_data = data;
    }

}