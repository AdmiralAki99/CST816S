#ifndef HAL_DATA_TYPES_H
#define HAL_DATA_TYPES_H

#include <inttypes.h>

typedef struct{
    uint32_t pin;
    uint8_t mode;
    uint8_t pull_up_en;
    uint8_t pull_down_en;
    uint8_t interrupt_type;
}hal_gpio_config_t;

typedef uint8_t hal_err_t;


#endif