/**
 * @file        button_manager.cpp
 * @brief       Implementation of the button manager for the Wiicon Remote project
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

#include "button_manager.h"

void (*ButtonManager::onSingleClick)() = nullptr;
void (*ButtonManager::onDoubleClick)() = nullptr;
void (*ButtonManager::onTripleClick)() = nullptr;
void (*ButtonManager::onLongPress)()   = nullptr;

unsigned long ButtonManager::_lastStateChange  = 0;
unsigned long ButtonManager::_lastClickTime    = 0;
bool          ButtonManager::_lastState        = HIGH;
int           ButtonManager::_clickCount       = 0;
bool          ButtonManager::_longPressHandled = false;

void ButtonManager::begin() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    _lastState = digitalRead(BUTTON_PIN);
    Log::info("Button manager initialized on pin %d", BUTTON_PIN);
}

void ButtonManager::loop() {
    bool          currentState = digitalRead(BUTTON_PIN);
    unsigned long now          = millis();

    if (currentState != _lastState) {
        if (now - _lastStateChange > DEBOUNCE_TIME) {
            _lastState       = currentState;
            _lastStateChange = now;

            if (currentState == LOW) {
                _longPressHandled = false;
            } else {
                if (!_longPressHandled) {
                    _clickCount++;
                    _lastClickTime = now;
                }
            }
        }
    }

    if (currentState == LOW && !_longPressHandled) {
        if (now - _lastStateChange > LONG_PRESS_TIME) {
            _longPressHandled = true;
            _clickCount       = 0;
            Log::info("Button: Long Press Detected");
            if (onLongPress) onLongPress();
        }
    }

    if (_clickCount > 0 && (now - _lastClickTime > CLICK_TIMEOUT) && currentState == HIGH) {
        if (_clickCount == 1) {
            Log::info("Button: Single Click");
            if (onSingleClick) onSingleClick();
        } else if (_clickCount == 2) {
            Log::info("Button: Double Click");
            if (onDoubleClick) onDoubleClick();
        } else if (_clickCount >= 3) {
            Log::info("Button: Triple Click");
            if (onTripleClick) onTripleClick();
        }
        _clickCount = 0;
    }
}