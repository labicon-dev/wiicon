/**
 * @file        helpers.cpp
 * @brief       Implementation of the helper functions for the Wiicon Remote project
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

#include "helpers.h"

void sendEulerAngles() {
    int16_t ax_raw, ay_raw, az_raw;
    int16_t gx_raw, gy_raw, gz_raw;

    // Read accelerometer data
    if (!readAccelRaw(&ax_raw, &ay_raw, &az_raw)) {
        Log::error("Failed to read ACC data");
        return;
    }

    // Read gyroscope data
    if (!readGyroRaw(&gx_raw, &gy_raw, &gz_raw)) {
        Log::error("Failed to read GYR data");
        return;
    }

    // Debug: print raw values if all are zero
    if (ax_raw == 0 && ay_raw == 0 && az_raw == 0 && gx_raw == 0 && gy_raw == 0 && gz_raw == 0) {
        Log::error("Raw sensor values are all zero — check wiring, address, or that sensor is powered.");
    }

    // Apply axis remapping and sign inversion before converting to physical units
    int   rawA[3] = {ax_raw, ay_raw, az_raw};
    int   rawG[3] = {gx_raw, gy_raw, gz_raw};
    float a_mapped[3];
    float g_mapped[3];
    float bias_mapped[3];

    for (int i = 0; i < 3; ++i) {
        a_mapped[i] = (float)rawA[accelMap[i]] * (float)accelSign[i];
        // Convert accel LSB -> g
        a_mapped[i] = a_mapped[i] / ACC_LSB_PER_G;
        g_mapped[i] = (float)rawG[gyroMap[i]] * (float)gyroSign[i];
        // Bias in deg/s for mapped axis: get raw bias from source axis and apply sign
        bias_mapped[i] = gyroBiasRaw[gyroMap[i]] * (float)gyroSign[i];
        // Convert gyro LSB -> deg/s and remove bias
        g_mapped[i] = g_mapped[i] / GYR_LSB_PER_DPS - bias_mapped[i];
    }

    // Feed the Madgwick with mapped axes (order: X=0, Y=1, Z=2)
    MadgwickAHRSupdate(g_mapped[0], g_mapped[1], g_mapped[2], a_mapped[0], a_mapped[1], a_mapped[2]);

    // Convert quaternion to Euler angles (degrees)
    float roll, pitch, yaw;
    getEulerAngles(&roll, &pitch, &yaw);

    // Optionally swap roll and yaw before sending
#if SWAP_ROLL_YAW
    float outRoll = yaw;
    float outYaw  = roll;
#else
    float outRoll = roll;
    float outYaw  = yaw;
#endif

    // Send via Serial (CSV: roll,pitch,yaw)
    Serial.print(outRoll, 2);
    Serial.print(',');
    Serial.print(pitch, 2);
    Serial.print(',');
    Serial.println(outYaw, 2);

    // Send via OSC
    oscManager.sendEulerAngles(outRoll, pitch, outYaw);
    LedManager::signalOscReady();
}

void initLittleFS() {
    if (!LittleFS.begin(true)) {
        Log::error("Failed to mount LittleFS");
        return;
    }
    Log::info("LittleFS mounted");
}

String readFile(fs::FS& fs, const char* path) {
    Log::debug("Reading file: %s", path);
    File file = fs.open(path);

    if (!file || file.isDirectory()) {
        Log::error("Failed to open file: %s", path);
        return "";
    }

    String fileContent;
    while (file.available()) {
        fileContent = file.readStringUntil('\n');
        break;
    }

    file.close();
    return fileContent;
}

void writeFile(fs::FS& fs, const char* path, const char* message) {
    Log::debug("Writing file: %s", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        Log::error("Failed to open file for writing: %s", path);
        return;
    }

    if (file.print(message)) {
        Log::debug("File written successfully: %s", path);
    } else {
        Log::error("Failed to write file: %s", path);
    }

    file.close();
}
