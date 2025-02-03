# CST816S Touch Screen Component for Smartwatch

This repository showcases a CST816S touch screen component. It is made in a modular layered embedded architecture for easier feature addition and changing the logic or swapping MCU's.

## **Layered Architecture**

## **Features**

- **Layered Architecture:** Implements a clean separation between HAL (Hardware Abstraction Layer), BSP (Board Support Layer), and the Application Layer.
- **Read Touch Data:** Read the touch data from the CST816S device 
- **Interrupt Mechanism:** Use an interrrupt mechanism (ISR) to reduce the overhead by continuously polling and use the interrupt to handle the touch gesture
- **Noise Filtering:** Filtering the ghost/noise touches on the screen that can occur from light pressure or leftover pressure
- **Modular Design:** Demonstrates integration potential with other components like BLE GATT drivers and LVGL for UI.

## **Project Structure**

```
|-- src/
|   |-- hal_touch.c           # Hardware Abstraction Layer (HAL) implementation
|   |-- bsp_touch.c           # Board Support Layer (BSL) implementation
|   |-- app_touch.c           # Application-specific logic
|-- include/
|   |-- hal_touch.h           # HAL public header
|   |-- hal_data_types.h           # HAL public header
|   |-- bsp_touch.h           # BSP public header
|   |-- app_touch.h           # Application interface
|-- README.md                 # Documentation
|-- sdkconfig                 # ESP-IDF configuration file
```

## **Architecture**

### **HAL (Hardware Abstraction Layer)**
The HAL encapsulates all direct interactions with ESP SoC, ensuring hardware-specific operations are abstracted.

#### Responsibilities:
- Initialze I2C protocol
- Read from registers 
- Write to registers

#### Key Functions:
- `hal_initialize_i2c_controller(void);`
- `hal_i2c_read_register(uint8_t register_address, uint8_t *data)`
- `hal_i2c_write_data(uint8_t register_address, uint8_t *data)`
- `hal_read_touch_gesture(uint8_t *touch_data)`
- `hal_read_touch_coordinates(uint16_t *x, uint16_t *y)`

### **BSP (Board Support Layer)**
The BSP manages the communication between the Application Layer (APP) and the underlying hardware (HAL). This is designed so that it can be swapped easily to another SoC

#### Responsibilities:
- CST816S initialization
- Interrupt handling (ISR)
- Reading Data From HAL

#### Key Functions:
- `bsp_initialize_touchscreen(void)`
- `bsp_initialize_touchscreen_task()`
- `IRAM_ATTR bsp_touch_interrupt_handler(void* args)`
- `bsp_get_latest_touch_data(bsp_touch_data_t *touch_data)`

### **Application Layer**
The Application Layer provides smartwatch-specific logic, integrating I2C with other components such as BLE drivers and LVGL.

#### Responsibilities:
- Application-specific tasks like starting up the touch screen driver
- Integration of touch data to UI

#### Key Functions:
- `app_initialize_touchscreen()`
- `app_initialize_touchscreen_task()`
- `app_start_touchscreen()`
- `app_get_latest_touch_data(bsp_touch_data_t *touch_data)`

## **Integration with Other Components**
This CST816S I2C driver is designed to integrate seamlessly with other smartwatch components:
- **BLE Server Driver:** Reads, updates and notifies the Connected device of the server state.
- **LVGL UI Library:** Displays Information based on the BLE profile on a graphical interface.

## **License**
This project is licensed under the GPL-3.0 License. See the LICENSE file for details.

## **Acknowledgments**
Special thanks to the ESP-IDF community and contributors for providing robust BLE support.

