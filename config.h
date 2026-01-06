/**
 * @file        config.h
 * @brief       Configuration file for the Wiicon Remote project
 *
 * @author      See AUTHORS file for full list of contributors
 * @date        2025
 * @version     1.0.0
 *
 * ========================================================================================
 *
 * MIT License
 * Copyright (c) 2025 Wiicon Remote Contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ========================================================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#include "driver/gpio.h"

const uint32_t SERIAL_BAUD = 115200;

// DATA MODE
#define DATA_SERIAL_LOG 0

// BUTTON MANAGER
const gpio_num_t BUTTON_PIN = GPIO_NUM_3;

// BMI160 SENSOR
const int     SDA_PIN     = 4;
const int     SCL_PIN     = 2;
const uint8_t BMI160_ADDR = 0x68;
enum class DataMode { RAW, FILTERED };

// SLEEP MANAGER
const int SLEEP_DEBOUNCE_MS = 1000;

// TIMING CONSTANTS
const int DELAY_LED_FEEDBACK_MS   = 200;  /**< LED feedback duration after mode toggle */
const int DELAY_BEFORE_RESTART_MS = 1000; /**< Delay before device restart */
const int DELAY_STARTUP_SAFETY_MS = 3000; /**< Safety delay at startup to prevent immediate sleep */

// LED STATUS INDICATOR
const int  LED_PIN_R            = 18;
const int  LED_PIN_G            = 19;
const int  LED_PIN_B            = 20;
const bool LED_RGB_COMMON_ANODE = false;

// OSC MANAGER
constexpr const char* OSC_TARGET_IP     = "192.168.1.255";
constexpr int         OSC_TARGET_PORT   = 9000;
constexpr const char* OSC_ADDRESS_EULER = "/wiicon/euler";

// DATA MAPPING
#define SWAP_ROLL_YAW 0

extern int accelMap[3];
extern int accelSign[3];
extern int gyroMap[3];
extern int gyroSign[3];

const float ACC_LSB_PER_G   = 16384.0f; /**< ±2g => LSB/g ≈ 16384 */
const float GYR_LSB_PER_DPS = 16.4f;    /**< ±2000 dps => LSB/(deg/s) ≈ 16.4 */
const int   CALIB_SAMPLES   = 200;
const int   CALIB_DELAY_MS  = 5;

#endif  // CONFIG_H
