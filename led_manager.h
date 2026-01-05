/**
 * @file        led_manager.h
 * @brief       LED Manager for the Wiicon Remote project
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

#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>

#include "config.h"
#include "logger.h"

class LedManager {
   public:
    /**
     * Initialize the LED Manager
     */
    static void begin();

    /**
     * Set the color of the RGB LED
     * @param r Red
     * @param g Green
     * @param b Blue
     */
    static void setColor(bool r, bool g, bool b);

    /**
     * Signal the startup of the device
     */
    static void signalStartup();

    /**
     * Signal the search for WiFi
     */
    static void signalWifiSearch();

    /**
     * Signal the success of the operation
     */
    static void signalSuccess();

    /**
     * Signal the error of the sensor
     */
    static void signalErrorSensor();

    /**
     * Signal the error of the general
     */
    static void signalErrorGeneral();

    /**
     * Signal the OSC is ready to send data
     */
    static void signalOscReady();

    /**
     * Turn off the RGB LED
     */
    static void off();

   private:
    /**
     * Blink the RGB LED
     * @param r Red
     * @param g Green
     * @param b Blue
     * @param times Number of times to blink
     * @param delayMs Delay between blinks
     */
    static void blink(bool r, bool g, bool b, int times, int delayMs);
};

#endif