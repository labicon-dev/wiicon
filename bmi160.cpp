/**
 * @file        bmi160.cpp
 * @brief       BMI160 IMU driver implementation for the WiiCon project
 * 
 * @details     I2C communication, sensor initialization, calibration routines,
 *              and raw data acquisition for the Bosch BMI160 IMU.
 * 
 * @author      See AUTHORS file for full list of contributors
 * @date        2025
 * @version     1.0.0
 * 
 * @see         bmi160.h for full license and attribution information
 * 
 * ========================================================================================
 * DERIVED WORK - See bmi160.h for complete attribution chain
 * ========================================================================================
 * 
 * Copyright (c) 2012 Jeff Rowberg (I2Cdev library)
 * Copyright (c) 2015 Intel Corporation (BMI160-Arduino library)
 * Copyright (c) 2025 WiiCon Contributors (this implementation)
 * 
 * SPDX-License-Identifier: MIT
 * 
 * ========================================================================================
 */

#include "bmi160.h"

float gyroBiasRaw[3] = {0.0f, 0.0f, 0.0f};

void writeReg(uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(BMI160_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

bool readBytes(uint8_t reg, uint8_t *buf, uint8_t len)
{
    Wire.beginTransmission(BMI160_ADDR);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0)
        return false;
    Wire.requestFrom((int)BMI160_ADDR, (int)len);
    for (uint8_t i = 0; i < len; ++i)
    {
        if (Wire.available())
            buf[i] = Wire.read();
        else
            return false;
    }
    return true;
}

int16_t toInt16(uint8_t lsb, uint8_t msb)
{
    return (int16_t)((msb << 8) | lsb);
}

bool initBMI160()
{
    // Soft reset
    writeReg(REG_CMD, 0xB6);
    delay(100);

    // Check chip ID
    uint8_t id = 0;
    if (!readBytes(REG_CHIP_ID, &id, 1))
        return false;
    if (id != BMI160_CHIP_ID)
    {
        Log::error("BMI160 chip id mismatch: 0x%02X", id);
    }

    writeReg(REG_CMD, REG_ACC_NORMAL_MODE);
    delay(50);
    writeReg(REG_CMD, REG_GYR_NORMAL_MODE);
    delay(50);

    // Configure accelerometer: ODR and range
    // ACC_CONF: ODR=100Hz, normal BW
    writeReg(REG_ACC_CONF, 0x28);
    // ACC_RANGE: 0x03 => ±2g (see datasheet)
    writeReg(REG_ACC_RANGE, 0x03);

    // Configure gyroscope: ODR=100Hz, range ±2000 dps
    writeReg(REG_GYR_CONF, 0x28);
    writeReg(REG_GYR_RANGE, 0x00); // 0x00 => ±2000 dps

    delay(50);
    return true;
}

void autoCalibrateAccelerometer()
{
    Log::info("Starting accelerometer auto-calibration command (0x37)...");
    writeReg(REG_CMD, 0x37);
    delay(100);
    // Wait extra time to complete the calibration
    delay(1000);
    Log::info("Accelerometer auto-calibration command sent.");
}

bool calibrateGyro(int samples, int delayMs)
{
    if (samples <= 0)
        return false;
    
    long sumX = 0, sumY = 0, sumZ = 0;
    uint8_t buf[6];
    
    for (int i = 0; i < samples; ++i)
    {
        if (!readBytes(REG_GYR_DATA, buf, 6))
            return false;
        int16_t gx_raw = toInt16(buf[0], buf[1]);
        int16_t gy_raw = toInt16(buf[2], buf[3]);
        int16_t gz_raw = toInt16(buf[4], buf[5]);
        sumX += gx_raw;
        sumY += gy_raw;
        sumZ += gz_raw;
        delay(delayMs);
    }
    
    float avgX = (float)sumX / (float)samples;
    float avgY = (float)sumY / (float)samples;
    float avgZ = (float)sumZ / (float)samples;
    
    // Convert to deg/s using scale factor and store in raw order
    gyroBiasRaw[0] = avgX / GYR_LSB_PER_DPS;
    gyroBiasRaw[1] = avgY / GYR_LSB_PER_DPS;
    gyroBiasRaw[2] = avgZ / GYR_LSB_PER_DPS;
    
    return true;
}

void i2cScanner()
{
    Log::info("I2C scan:");
    bool found = false;
    for (uint8_t address = 1; address < 127; ++address)
    {
        Wire.beginTransmission(address);
        uint8_t error = Wire.endTransmission();
        if (error == 0)
        {
            Log::info("Found device at 0x%02X", address);
            found = true;
        }
    }
    if (!found)
        Log::info("No I2C devices found. Check wiring and pull-ups.");
}

bool readAccelRaw(int16_t *ax_raw, int16_t *ay_raw, int16_t *az_raw)
{
    uint8_t buf[6];
    if (!readBytes(REG_ACC_DATA, buf, 6))
        return false;
    
    *ax_raw = toInt16(buf[0], buf[1]);
    *ay_raw = toInt16(buf[2], buf[3]);
    *az_raw = toInt16(buf[4], buf[5]);
    
    return true;
}

bool readGyroRaw(int16_t *gx_raw, int16_t *gy_raw, int16_t *gz_raw)
{
    uint8_t buf[6];
    if (!readBytes(REG_GYR_DATA, buf, 6))
        return false;
    
    *gx_raw = toInt16(buf[0], buf[1]);
    *gy_raw = toInt16(buf[2], buf[3]);
    *gz_raw = toInt16(buf[4], buf[5]);
    
    return true;
}


