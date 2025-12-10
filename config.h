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

const int      SDA_PIN     = 2;
const int      SCL_PIN     = 1;
const uint8_t  BMI160_ADDR = 0x68;
const uint32_t SERIAL_BAUD = 115200;

// If true, swap roll and yaw in the serial output (useful if the sensor axes are rotated)
#define SWAP_ROLL_YAW 0

extern int accelMap[3];
extern int accelSign[3];
extern int gyroMap[3];
extern int gyroSign[3];

const float ACC_LSB_PER_G   = 16384.0f;  // ±2g => LSB/g ≈ 16384
const float GYR_LSB_PER_DPS = 16.4f;     // ±2000 dps => LSB/(deg/s) ≈ 16.4
const int   CALIB_SAMPLES   = 200;
const int   CALIB_DELAY_MS  = 5;

// DEBUG ONLY
#define DISABLE_BMI160_SENSOR 1
#define CLEAR_NETWORK_INFO 0

#endif  // CONFIG_H
