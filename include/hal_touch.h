#ifndef HAL_TOUCH_H
#define HAL_TOUCH_H

#include <stdio.h>
#include "sdkconfig.h"

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_system.h"
#include "esp_mac.h"

#include "hal_data_types.h"

#define SDA_PIN 8
#define SCL_PIN 9
#define RST_PIN 5
#define INT_PIN 21

#define I2C_DEVICE_FREQUENCY 100000 // 100kHz I2C frequency used for testing since internal pullups are used
#define I2C_DEVICE_CONTROLLER I2C_NUM_0

#define CHIP_ID 0x15
#define WRITE_ADDR 0x2A
#define READ_ADDR 0x2B

// Register addresses
#define CHIP_ID_REG 0xA7
#define GESTURE_ID_REG 0x01
#define AUTO_SLEEP_TIME_REG 0xF9
#define FIRMWARE_VERSION_REG 0xA9

#define LOG_TAG "CST816S"

#define DEBUG

// typedef struct {
//     uint16_t x;
//     uint16_t y;
//     uint8_t gesture;
// } touch_data_t;

hal_err_t hal_initialize_i2c_controller(void);

hal_err_t hal_i2c_read_register(uint8_t register_address, uint8_t *data);

// Function to write data to the I2C device

hal_err_t hal_i2c_write_data(uint8_t register_address, uint8_t *data);

hal_err_t hal_read_touch_gesture(uint8_t *touch_data);

hal_err_t hal_read_touch_coordinates(uint16_t *x, uint16_t *y);

hal_err_t hal_check_touch_status(uint8_t *status);

hal_err_t hal_read_auto_sleep_time(uint8_t *data);

hal_err_t hal_touchscreen_get_firmware(uint8_t *data);

hal_err_t hal_initialize_gpio_interrupt_config(hal_gpio_config_t *config, void (*interrupt_handler)(void *));
#endif