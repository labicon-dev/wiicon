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
 
 #include "config.h"
 
 class LedManager {
 public:
     static void begin();
     
     static void signalStartup();      // Fast fixed on
     static void signalWifiSearch();   // State change
     static void signalSuccess();      // 3 slow blinks
     static void signalErrorSensor();  // Fast infinite blink
     static void signalErrorGeneral(); // SOS pattern
     static void off();                // Turn off all LEDs
 
 private:
     static void setLed(bool on);
     static void blink(int times, int delayMs);
 };
 
 #endif