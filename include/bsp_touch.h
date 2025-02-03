#ifndef BSP_TOUCH_H
#define BSP_TOUCH_H

#include "hal_touch.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// typedef enum{
//     NO_GESTURE = 0x00,
//     SLIDE_UP = 0x01,
//     SLIDE_DWN = 0x02,
//     SLIDE_LFT = 0x03,
//     SLIDE_RGT = 0x04,
//     SNGL_CLCK = 0x05,
//     DBL_CLCK = 0x0B,
//     LONG_PRS = 0x0C
    
// } gesture_t;

// Interrupt task handle for the touch controller
extern TaskHandle_t bsp_interrupt_task_handle;

extern QueueHandle_t bsp_touch_queue;

typedef struct{
    uint16_t x;
    uint16_t y;
    uint8_t gesture;
} bsp_touch_data_t;

extern bsp_touch_data_t bsp_touch_data;


void bsp_initialize_touchscreen(void);

void bsp_initialize_touchscreen_task();

void IRAM_ATTR bsp_touch_interrupt_handler(void* args);

void bsp_initialize_interrupt_service();

void bsp_touch_interrupt_task(void *args);

hal_err_t bsp_start_touchscreen_interrupt_service(void);

bool bsp_get_latest_touch_data(bsp_touch_data_t *touch_data);

#endif

