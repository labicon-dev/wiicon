/**
 * @file        led_manager.cpp
 * @brief       Implementation of the LED Manager for the Wiicon Remote project
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

 #include "led_manager.h"
 #include "logger.h"
 
 void LedManager::begin() {
     pinMode(INTERNAL_LED_PIN, OUTPUT);
     off();
     Log::info("LED Manager initialized on GPIO %d", INTERNAL_LED_PIN);
 }
 
 void LedManager::setLed(bool on) {
     // If LED_INVERTED is true: on(true) -> write LOW
     digitalWrite(INTERNAL_LED_PIN, (LED_INVERTED ? !on : on));
 }
 
 void LedManager::off() {
     setLed(false);
 }
 
 void LedManager::blink(int times, int delayMs) {
     for (int i = 0; i < times; i++) {
         setLed(true);
         delay(delayMs);
         setLed(false);
         delay(delayMs);
     }
 }
 
 void LedManager::signalStartup() {
     setLed(true);
     delay(1000);
     off();
 }
 
 void LedManager::signalWifiSearch() {
     static unsigned long lastToggle = 0;
     const int interval = 200;
 
     if (millis() - lastToggle > interval) {
         lastToggle = millis();
         int state = digitalRead(INTERNAL_LED_PIN);
         digitalWrite(INTERNAL_LED_PIN, !state);
     }
 }
 
 void LedManager::signalSuccess() {
     Log::info("Status: Success");
     blink(3, 600);
     off();
 }
 
 void LedManager::signalErrorSensor() {
     Log::error("Status: Sensor Error Loop");
     while (true) {
         blink(2, 50);
         delay(200);
     }
 }
 
 void LedManager::signalErrorGeneral() {
     Log::error("Status: General Error");
     blink(3, 100);
     delay(200);
     blink(3, 300);
     delay(200);
     blink(3, 100);
     delay(1000);
 }