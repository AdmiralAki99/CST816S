#include "include/bsp_touch.h"

TaskHandle_t bsp_interrupt_task_handle = NULL;
bsp_touch_data_t bsp_touch_data;
QueueHandle_t bsp_touch_queue;

void bsp_initialize_touchscreen(void){
    // Initialize the i2c controller
    int ret = hal_initialize_i2c_controller();
    if(ret != ESP_OK){
        ESP_LOGE(LOG_TAG, "I2C controller initialization failed");
        return;
    }

    // Initialize the interrupt mechanism that is described in the datasheet
    bsp_initialize_interrupt_service();

    // Initialize the queue for the touch data
    bsp_touch_queue = xQueueCreate(5, sizeof(bsp_touch_data_t));
}

void bsp_initialize_touchscreen_task(){
    bsp_start_touchscreen_interrupt_service();
}

void IRAM_ATTR bsp_touch_interrupt_handler(void* args){

    // Notify the main thread about the interrupt
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(bsp_interrupt_task_handle, 0, eNoAction, xHigherPriorityTaskWoken);

    if(xHigherPriorityTaskWoken){
        portYIELD_FROM_ISR();
    }
    
}

void bsp_initialize_interrupt_service(){
    // Initialize the interrupt handler
    hal_gpio_config_t interrupt_config = {
        .pin = INT_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .interrupt_type = GPIO_INTR_NEGEDGE // Interrupt on the negative edge since it is active low
    };

    hal_err_t err = hal_initialize_gpio_interrupt_config(&interrupt_config, bsp_touch_interrupt_handler);
    if(err != ESP_OK){
        ESP_LOGE(LOG_TAG, "Error initializing interrupt service");
    }
}

void bsp_touch_interrupt_task(void *args){
    while(1){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Read the gesture from the touch controller
        uint8_t data;
        int err = hal_read_touch_gesture(&data);
        if (err != 0){
            printf("Error reading gesture\n");
        }
        // If there is no gesture, do not read the coordinates
       if(data != 0x0){
            // Read the touch coordinates
            uint16_t x,y;
            err = hal_read_touch_coordinates(&x, &y);
            if(err != 0){
                printf("Error reading touch coordinates\n");
            }

            printf("BSP X: %d, Y: %d, Gesture: %d\n", x, y, data);

            bsp_touch_data.x = x;
            bsp_touch_data.y = y;
            bsp_touch_data.gesture = data;

            xQueueSend(bsp_touch_queue, &bsp_touch_data, 0); // Send the touch data to the queue
       }

    }
}

hal_err_t bsp_start_touchscreen_interrupt_service(void){

    xTaskCreate(bsp_touch_interrupt_task, "Touch Interrupt Task", 2048, NULL, 10, &bsp_interrupt_task_handle);

    return ESP_OK;
}

bool bsp_get_latest_touch_data(bsp_touch_data_t *touch_data){
    return xQueueReceive(bsp_touch_queue, touch_data, 0);
}

