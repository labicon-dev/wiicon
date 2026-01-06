/**
 * @file        actions.cpp
 * @brief       Implementation of the actions for the WiiCon Remote project
 *
 * @author      See AUTHORS file for full list of contributors
 * @date        2025
 * @version     1.0.0
 *
 * ========================================================================================
 *
 * MIT License
 * Copyright (c) 2025 WiiCon Remote Contributors
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

#include "actions.h"

DataMode dataMode = DataMode::FILTERED;

void actionToggleDataMode() {
    dataMode = dataMode == DataMode::RAW ? DataMode::FILTERED : DataMode::RAW;
    Log::info("Data mode toggled to %s", dataMode == DataMode::RAW ? "RAW" : "FILTERED");
    LedManager::setColor(0, 0, 1);
    delay(DELAY_LED_FEEDBACK_MS);
    LedManager::off();
}

void actionResetCalibration() {
    Log::info("Resetting calibration...");
    LedManager::setColor(1, 1, 0);
    if (calibrateGyro(CALIB_SAMPLES, CALIB_DELAY_MS)) {
        LedManager::signalSuccess();
    } else {
        LedManager::signalErrorGeneral();
    }
}

void actionResetWifiConfig() {
    Log::warning("Resetting WiFi configuration... This will reboot the device.");
    WiFiManager::instance().clearCredentials();
    LedManager::signalErrorGeneral();
    delay(DELAY_BEFORE_RESTART_MS);
    ESP.restart();
}

void actionSleep() {
    Log::info("Entering deep sleep...");
    LedManager::off();
    goToSleep();
}