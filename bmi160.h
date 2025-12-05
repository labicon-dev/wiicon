/**
 * @file        bmi160.h
 * @brief       BMI160 IMU driver for the WiiCon project
 * 
 * @details     Lightweight I2C driver for the Bosch BMI160 6-axis IMU.
 *              Provides initialization, configuration, calibration, and
 *              raw data reading for accelerometer and gyroscope.
 * 
 * @author      See AUTHORS file for full list of contributors
 * @date        2025
 * @version     1.0.0
 * 
 * @see         Bosch BMI160 Datasheet:
 *              https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmi160-ds000.pdf
 * 
 * ========================================================================================
 * DERIVED WORK NOTICE
 * ========================================================================================
 * 
 * This code is based on the BMI160-Arduino library for Intel(R) Curie(TM) devices:
 * Copyright (c) 2015 Intel Corporation. All rights reserved.
 * 
 * Which in turn is based on the I2Cdev device library by Jeff Rowberg:
 * https://github.com/jrowberg/i2cdevlib
 * Copyright (c) 2012 Jeff Rowberg
 * 
 * Register definitions and configuration values are derived from the
 * Bosch BMI160 datasheet (Document revision 1.0, November 2020).
 * 
 * ========================================================================================
 * LICENSE (MIT)
 * ========================================================================================
 * 
 * Copyright (c) 2012 Jeff Rowberg (I2Cdev library)
 * Copyright (c) 2015 Intel Corporation (BMI160-Arduino library)
 * Copyright (c) 2025 WiiCon Contributors (this implementation)
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
 * SPDX-License-Identifier: MIT
 * 
 * ========================================================================================
 */

#ifndef BMI160_H
#define BMI160_H

#include <Arduino.h>
#include <Wire.h>

#include "config.h"

const uint8_t REG_CHIP_ID         = 0x00;
const uint8_t REG_GYR_DATA        = 0x0C;
const uint8_t REG_ACC_DATA        = 0x12;
const uint8_t REG_CMD             = 0x7E;
const uint8_t REG_ACC_CONF        = 0x40;
const uint8_t REG_ACC_NORMAL_MODE = 0x11;
const uint8_t REG_GYR_NORMAL_MODE = 0x15;
const uint8_t REG_ACC_RANGE       = 0x41;
const uint8_t REG_GYR_CONF        = 0x42;
const uint8_t REG_GYR_RANGE       = 0x43;

const uint8_t BMI160_CHIP_ID = 0xD1;

extern float gyroBiasRaw[3];

/**
 * Write a value to a register of the BMI160
 * @param reg Register address
 * @param val Value to be written
 */
void writeReg(uint8_t reg, uint8_t val);

/**
 * Read multiple bytes from a register of the BMI160
 * @param reg Register address
 * @param buf Buffer to store the read bytes
 * @param len Number of bytes to read
 * @return true if the read was successful
 */
bool readBytes(uint8_t reg, uint8_t* buf, uint8_t len);

/**
 * Convert two bytes (LSB, MSB) to a 16-bit signed integer
 * @param lsb Least significant byte (LSB)
 * @param msb Most significant byte (MSB)
 * @return 16-bit signed integer result
 */
int16_t toInt16(uint8_t lsb, uint8_t msb);

/**
 * Basic initialization of the BMI160
 * Configures operation modes, ODR and ranges
 * @return true if the initialization was successful
 */
bool initBMI160();

/**
 * Trigger automatic calibration of the accelerometer
 * Send auto-calibration command and wait for completion
 */
void autoCalibrateAccelerometer();

/**
 * Average gyroscope calibration
 * Collect N samples and calculate the average bias for each axis
 * @param samples Number of samples to collect
 * @param delayMs Delay between samples in milliseconds
 * @return true if the calibration was successful
 */
bool calibrateGyro(int samples, int delayMs);

/**
 * I2C scanner for debugging
 * List all devices found on the I2C bus
 */
void i2cScanner();

/**
 * Read raw accelerometer data
 * @param ax_raw Pointer to store X value
 * @param ay_raw Pointer to store Y value
 * @param az_raw Pointer to store Z value
 * @return true if the read was successful
 */
bool readAccelRaw(int16_t* ax_raw, int16_t* ay_raw, int16_t* az_raw);

/**
 * Read raw gyroscope data
 * @param gx_raw Pointer to store X value
 * @param gy_raw Pointer to store Y value
 * @param gz_raw Pointer to store Z value
 * @return true if the read was successful
 */
bool readGyroRaw(int16_t* gx_raw, int16_t* gy_raw, int16_t* gz_raw);

#endif  // BMI160_H
