// This file allows the DM8BA10 library to be used with ESP-IDF without including the Arduino.h dependency
#pragma once

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp32/rom/ets_sys.h"
#include <cstring>
#include <cmath>

#define HIGH true
#define LOW  false
#define OUTPUT GPIO_MODE_OUTPUT

inline void delayMicroseconds(uint32_t us) {
    esp_rom_delay_us(us);
}

inline void pinMode(int8_t pin, gpio_mode_t mode) {
    esp_rom_gpio_pad_select_gpio(pin);
    gpio_set_direction((gpio_num_t)pin, mode);
}

inline void digitalWrite(int8_t csPin, bool value) {
    gpio_set_level((gpio_num_t)csPin, value ? 1 : 0);
}
