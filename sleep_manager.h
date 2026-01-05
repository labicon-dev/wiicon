/**
 * @file        sleep_manager.h
 * @brief       Deep sleep management for the Wiicon Remote project
 *
 * @details     Provides functions for managing ESP32-C6 deep sleep with
 *              external wakeup via GPIO. Supports LP (Low Power) IO pins
 *              for waking the chip from deep sleep.
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

#ifndef SLEEP_MANAGER_H
#define SLEEP_MANAGER_H

#include <Arduino.h>

#include "config.h"
#include "esp_sleep.h"
#include "logger.h"

#define WAKE_BITMASK (1ULL << WAKE_PIN) /**< Wakeup bitmask for the wake pin */

/**
 * Initialize the sleep manager
 * Configures the wake pin and logs the wakeup cause
 */
void initSleepManager();

/**
 * Enter deep sleep mode
 * Configures EXT1 wakeup and powers down to deep sleep
 */
void goToSleep();

/**
 * Check the wakeup cause and log it
 * @return true if woken by external button, false otherwise
 */
bool checkWakeupCause();

/**
 * Check if the wake button is currently pressed
 * @return true if button is pressed (HIGH), false otherwise
 */
bool isWakeButtonPressed();

#endif  // SLEEP_MANAGER_H
