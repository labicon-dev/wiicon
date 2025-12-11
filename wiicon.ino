/**
 * @file        wiicon.ino
 * @brief       Main file for the Wiicon Remote project
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

#include "wiicon.h"

int accelMap[3]  = {0, 1, 2};
int accelSign[3] = {1, 1, 1};
int gyroMap[3]   = {0, 1, 2};
int gyroSign[3]  = {1, 1, 1};

unsigned long lastTime = 0;

void setup() {
    Serial.begin(SERIAL_BAUD);

    Log::init(LOG_LEVEL_DEBUG);
    Log::info("Wiicon Remote Project - Starting setup...");

    initLittleFS();

    if (CLEAR_NETWORK_INFO) {
        wifiManager.clearCredentials();
    }

    wifiManager.begin();

    if (!DISABLE_BMI160_SENSOR) 
    {
        Wire.begin(SDA_PIN, SCL_PIN);

        Log::info("Starting BMI160 raw reader (Wire-only). Initializing sensor...");

        if (SCL_PIN == 1 || SDA_PIN == 1) {
            Log::warning(
                "Warning: using GPIO1 for I2C may conflict with Serial TX (GPIO1). Consider using other pins like 21 "
                "(SDA) "
                "and 22 (SCL).");
        }

        i2cScanner();

        if (!initBMI160()) {
            Log::warning("Warning: failed to init BMI160 (I2C read/write). Check wiring and I2C address.");
        } else {
            Log::info("BMI160 init attempted (check chip id above).\n");
            autoCalibrateAccelerometer();
        }

        Log::info("Starting gyroscope auto-calibration. Keep the device stationary...");

        if (!calibrateGyro(CALIB_SAMPLES, CALIB_DELAY_MS)) {
            Log::error("Gyroscope calibration failed (I2C reads). Continuing without bias correction.");
        } else {
            float mappedBias[3];
            for (int i = 0; i < 3; ++i) mappedBias[i] = gyroBiasRaw[gyroMap[i]] * (float)gyroSign[i];

            Log::info("Gyro raw biases (deg/s): %.4f, %.4f, %.4f", gyroBiasRaw[0], gyroBiasRaw[1], gyroBiasRaw[2]);
            Log::info("Gyro mapped biases (deg/s): %.4f, %.4f, %.4f", mappedBias[0], mappedBias[1], mappedBias[2]);
        }
    }

    lastTime = micros();
}

void loop() {
    wifiManager.loop();

#ifndef DISABLE_BMI160_SENSOR
    unsigned long now = micros();
    float         dt  = (now - lastTime) / 1000000.0f;
    if (dt <= 0) return;

    float measuredHz = 1.0f / dt;
    sampleFreq       = 0.95f * sampleFreq + 0.05f * measuredHz;
    lastTime         = now;

    sendEulerAngles();

    delay(10);
#endif
}
