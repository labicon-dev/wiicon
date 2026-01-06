/**
 * @file        wiicon.ino
 * @brief       Main file for the WiiCon Remote project
 *
 * @details     Includes functions for reading sensor data, applying Madgwick filter
 *              and sending Euler angles via Serial in CSV format: R,P,Y
 *
 * @author      See AUTHORS file for full list of contributors
 * @date        2025
 * @version     1.0.0
 *
 * ========================================================================================
 *
 * MIT License
 * Copyright (c) 2025 WiiCon Contributors
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

#include "wiicon.h"

unsigned long lastTime = 0;

int accelMap[3]  = {0, 1, 2};
int accelSign[3] = {1, 1, 1};
int gyroMap[3]   = {0, 1, 2};
int gyroSign[3]  = {1, 1, 1};

void setup() {
    Serial.begin(SERIAL_BAUD);

    LedManager::begin();
    LedManager::signalStartup();

    ButtonManager::begin();
    ButtonManager::onSingleClick = actionToggleDataMode;
    ButtonManager::onDoubleClick = actionResetCalibration;
    ButtonManager::onTripleClick = actionResetWifiConfig;
    ButtonManager::onLongPress   = actionSleep;

    delay(DELAY_STARTUP_SAFETY_MS);

    Log::init(LOG_LEVEL_DEBUG);
    Log::info("WiiCon Remote Project - Starting setup...");

    initSleepManager();
    initLittleFS();

    wifiManager.begin();

    Log::info("Waiting for WiFi connection...");
    while (!wifiManager.isConnected() && !wifiManager.isInAPMode()) {
        wifiManager.loop();
        ButtonManager::loop();
        yield();
    }

    if (wifiManager.isConnected()) {
        Log::info("WiFi connected. Starting sensor initialization...");

        Wire.begin(SDA_PIN, SCL_PIN);

        Log::info("Starting BMI160 reader. Initializing sensor...");

        I2CScanner();

        if (!initBMI160Sensor()) {
            Log::error("Failed to init BMI160 (I2C read/write). Check wiring and I2C address.");
            LedManager::signalErrorSensor();
        } else {
            Log::info("BMI160 initialized successfully (chip id: 0x%02X).", BMI160_CHIP_ID);
            autoCalibrateAccelerometer();
            LedManager::signalSuccess();
        }

        Log::info("Starting gyroscope calibration. Keep the device stationary...");

        if (!calibrateGyro(CALIB_SAMPLES, CALIB_DELAY_MS)) {
            Log::error("Gyroscope calibration failed. Continuing without bias correction.");
            LedManager::signalErrorGeneral();
        } else {
            float mappedBias[3];
            for (int i = 0; i < 3; ++i) mappedBias[i] = gyroBiasRaw[gyroMap[i]] * (float)gyroSign[i];

            Log::info("Gyroscope calibration successful. Raw biases (deg/s): %.4f, %.4f, %.4f", gyroBiasRaw[0],
                      gyroBiasRaw[1], gyroBiasRaw[2]);
            Log::info("Gyroscope calibration successful. Mapped biases (deg/s): %.4f, %.4f, %.4f", mappedBias[0],
                      mappedBias[1], mappedBias[2]);
            LedManager::signalSuccess();
        }
    } else {
        Log::info("Device in AP mode. Calibration will start after WiFi connection.");
    }

    lastTime = micros();
}

void loop() {
    wifiManager.loop();
    ButtonManager::loop();

    if (wifiManager.isConnected()) {
        unsigned long now = micros();
        float         dt  = (now - lastTime) / 1000000.0f;
        if (dt > 0) {
            float measuredHz = 1.0f / dt;
            sampleFreq       = 0.95f * sampleFreq + 0.05f * measuredHz;
            lastTime         = now;
            sendEulerAngles();
        }
    }

    yield();
}
