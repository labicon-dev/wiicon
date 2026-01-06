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

void LedManager::begin() {
    pinMode(LED_PIN_R, OUTPUT);
    pinMode(LED_PIN_G, OUTPUT);
    pinMode(LED_PIN_B, OUTPUT);
    off();
    Log::info("RGB LED Manager initialized on pins R:%d G:%d B:%d", LED_PIN_R, LED_PIN_G, LED_PIN_B);
}

void LedManager::setColor(bool r, bool g, bool b) {
    bool stateR = LED_RGB_COMMON_ANODE ? !r : r;
    bool stateG = LED_RGB_COMMON_ANODE ? !g : g;
    bool stateB = LED_RGB_COMMON_ANODE ? !b : b;

    digitalWrite(LED_PIN_R, stateR);
    digitalWrite(LED_PIN_G, stateG);
    digitalWrite(LED_PIN_B, stateB);
}

void LedManager::off() { setColor(false, false, false); }

void LedManager::blink(bool r, bool g, bool b, int times, int delayMs) {
    for (int i = 0; i < times; i++) {
        setColor(r, g, b);
        delay(delayMs);
        off();
        delay(delayMs);
    }
}

void LedManager::signalStartup() {
    setColor(true, true, true);
    delay(1000);
    off();
}

void LedManager::signalWifiSearch() {
    static unsigned long lastToggle = 0;
    const int            interval   = 200;

    if (millis() - lastToggle > interval) {
        lastToggle = millis();
        bool state = digitalRead(LED_PIN_R);

        bool nextState = !state;

        if (LED_RGB_COMMON_ANODE) nextState = !nextState;

        setColor(nextState, nextState, false);
    }
}

void LedManager::signalWifiConnecting() {
    static unsigned long lastToggle = 0;
    const int            interval   = 500;

    if (millis() - lastToggle > interval) {
        lastToggle = millis();
        static bool state = false;
        state = !state;
        setColor(state, state, false);
    }
}

void LedManager::signalAPMode() {
    static unsigned long lastToggle = 0;
    const int            interval   = 500;

    if (millis() - lastToggle > interval) {
        lastToggle = millis();
        static bool state = false;
        state = !state;
        setColor(state, false, state);
    }
}

void LedManager::signalSuccess() {
    setColor(false, true, false);
    delay(2000);
    off();
}

void LedManager::signalErrorSensor() {
    while (true) {
        blink(true, false, false, 1, 100);
    }
}

void LedManager::signalErrorGeneral() { blink(true, false, true, 3, 300); }

void LedManager::signalOscReady() {
    static unsigned long lastBlinkTime = 0;
    static bool          isLedOn       = false;

    const int interval = 2000;
    const int duration = 100;

    unsigned long now = millis();

    if (!isLedOn) {
        if (now - lastBlinkTime > interval) {
            setColor(false, false, true);
            isLedOn       = true;
            lastBlinkTime = now;
        }
    } else {
        if (now - lastBlinkTime > duration) {
            off();
            isLedOn = false;
        }
    }
}