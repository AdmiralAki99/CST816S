#include "include/hal_touch.h"

hal_err_t hal_initialize_i2c_controller(void){
    i2c_config_t communication_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_DEVICE_FREQUENCY
    };

    // Now checking if the new config is applied and if it is successful
    esp_err_t ret = i2c_param_config(I2C_DEVICE_CONTROLLER, &communication_config);
    if(ret != ESP_OK){
        ESP_LOGE(LOG_TAG, "I2C parameter configuration failed");
        return ESP_FAIL;
    }

    ret = i2c_driver_install(I2C_DEVICE_CONTROLLER, I2C_MODE_MASTER, 0, 0, 0);
    if(ret != ESP_OK){
        ESP_LOGE(LOG_TAG, "I2C driver installation failed");
        return ESP_FAIL;
    }

    return ESP_OK; // Everything is fine

}

hal_err_t hal_i2c_read_register(uint8_t register_address, uint8_t *data){

    i2c_cmd_handle_t command_link = i2c_cmd_link_create(); // Create a new command link
    i2c_master_start(command_link); // Start the command link

    // Target the device with the ID to communicate a incoming read signal
    i2c_master_write_byte(command_link, CHIP_ID << 1 | 0, true);

    // Write the register address to read from
    i2c_master_write_byte(command_link,register_address, true);

    // Start the command link again to read from it
    i2c_master_start(command_link);

    // Write the read request to the device with the register
    i2c_master_write_byte(command_link, CHIP_ID << 1 | 1, true);

    // Read the register address to read from
    i2c_master_read_byte(command_link, data, I2C_MASTER_NACK);

    // Stop the command link
    i2c_master_stop(command_link);

    // Execute the command link and check if it was successful
    esp_err_t ret = i2c_master_cmd_begin(I2C_DEVICE_CONTROLLER, command_link, 1000 / portTICK_PERIOD_MS);
    if(ret != ESP_OK){
        ESP_LOGE(LOG_TAG, "I2C read failed");
        return ret;
    }

    // Delete the command link
    i2c_cmd_link_delete(command_link);

    return ESP_OK;
}

hal_err_t hal_i2c_write_data(uint8_t register_address, uint8_t *data){
    
    // Creating the command link for the I2C communication
    i2c_cmd_handle_t command_link = i2c_cmd_link_create();

    // Start the command link
    i2c_master_start(command_link);

    // Target the device with the ID to communicate a incoming write signal
    i2c_master_write_byte(command_link, CHIP_ID << 1 | 0, true);

    // Write the register address to write to
    i2c_master_write_byte(command_link, register_address, true);

    // Send the data to the register address
    i2c_master_write_byte(command_link, *data, true);

    // Stop the command link
    i2c_master_stop(command_link);

    // Execute the command link and check if it was successful
    esp_err_t ret = i2c_master_cmd_begin(I2C_DEVICE_CONTROLLER, command_link, 1000 / portTICK_PERIOD_MS);
    if(ret != ESP_OK){
        ESP_LOGE(LOG_TAG, "I2C write failed");
        return ret;
    }

    // Delete the command link
    i2c_cmd_link_delete(command_link);

    return ESP_OK;
}

hal_err_t hal_read_touch_gesture(uint8_t *touch_data){
    uint8_t data;
    esp_err_t err = hal_i2c_read_register(GESTURE_ID_REG, &data);

    if(err != ESP_OK){
        ESP_LOGE(LOG_TAG, "Error reading gesture ID");
        return err;
    }

    *touch_data = data;

    return ESP_OK; // Everything worked out fine

}

hal_err_t hal_read_touch_coordinates(uint16_t *x, uint16_t *y){
    uint8_t high_data, low_data;

    // Read the X coordinate
    esp_err_t err = hal_i2c_read_register(0x03, &high_data);
    if(err != ESP_OK){
        ESP_LOGE(LOG_TAG, "Error reading X coordinate");
        return err;
    }

    err = hal_i2c_read_register(0x04, &low_data);
    if(err != ESP_OK){
        ESP_LOGE(LOG_TAG, "Error reading X coordinate");
        return err;
    }

    // x = (high_data << 8) | low_data; // Shifting the high data by 8 bits and ORing it with the low data (Dont know why this is 128 constantly)

    // Converting the low data from uint8_t to uint16_t
    *x = (uint16_t)low_data;

    // Read the Y coordinate
    err = hal_i2c_read_register(0x05, &high_data);
    if(err != ESP_OK){
        ESP_LOGE(LOG_TAG, "Error reading Y coordinate");
        return err;
    }

    err = hal_i2c_read_register(0x06, &low_data);
    if(err != ESP_OK){
        ESP_LOGE(LOG_TAG, "Error reading Y coordinate");
        return err;
    }

    *y = (high_data << 8) | low_data; // Shifting the high data by 8 bits and ORing it with the low data

    return ESP_OK;

}

hal_err_t hal_check_touch_status(uint8_t *status){
    uint8_t data;
    esp_err_t err = hal_i2c_read_register(0x02, &data);

    if(err != ESP_OK){
        ESP_LOGE(LOG_TAG, "Error reading status");
        return err;
    }

    *status = data;

    return ESP_OK;
}


hal_err_t hal_touchscreen_get_firmware(uint8_t *data){
    esp_err_t err = hal_i2c_read_register(FIRMWARE_VERSION_REG, data);

    if(err != ESP_OK){
        ESP_LOGE(LOG_TAG, "Error reading firmware version");
        return err;
    }

    return ESP_OK;

}

hal_err_t hal_read_auto_sleep_time(uint8_t *data){
    esp_err_t err = hal_i2c_read_register(AUTO_SLEEP_TIME_REG, data);

    printf("Auto sleep time: %d\n", *data);

    if(err != ESP_OK){
        ESP_LOGE(LOG_TAG, "Error reading auto sleep time");
        return err;
    }

    return ESP_OK;
}

hal_err_t hal_initialize_gpio_interrupt_config(hal_gpio_config_t *config, void (*interrupt_handler)(void *)){
    gpio_config_t interrupt_config = {
        .pin_bit_mask = (1ULL << config->pin),
        .mode = config->mode,
        .pull_up_en = config->pull_up_en,
        .pull_down_en = config->pull_down_en,
        .intr_type = config->interrupt_type
    };

    esp_err_t err = gpio_config(&interrupt_config);
    if(err != ESP_OK){
        return ESP_FAIL;
    }

    // Installing an Interrupt Service Routing (ISR) to the GPIO pin
    err = gpio_install_isr_service(0);
    if(err != ESP_OK){
        return ESP_FAIL;
    }

    // Adding the interrupt handler to the GPIO pin
    err = gpio_isr_handler_add(config->pin, interrupt_handler, NULL);
    if(err != ESP_OK){
        return ESP_FAIL;
    }

    return ESP_OK;
}