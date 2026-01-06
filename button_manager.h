/**
 * @file        button_manager.h
 * @brief       Button Manager for the Wiicon Remote project
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

#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include <Arduino.h>

#include "config.h"
#include "logger.h"

class ButtonManager {
   public:
    /**
     * Initialize the button manager
     */
    static void begin();

    /**
     * Loop the button manager
     */
    static void loop();

    /**
     * Callback for single click
     */
    static void (*onSingleClick)();

    /**
     * Callback for double click
     */
    static void (*onDoubleClick)();

    /**
     * Callback for triple click
     */
    static void (*onTripleClick)();

    /**
     * Callback for long press
     */
    static void (*onLongPress)();

   private:
    static unsigned long _lastStateChange;  /**< Last state change time */
    static unsigned long _lastClickTime;    /**< Last click time */
    static bool          _lastState;        /**< Last state */
    static int           _clickCount;       /**< Click count */
    static bool          _longPressHandled; /**< Long press handled */

    static const int DEBOUNCE_TIME   = 50;   /**< Debounce time */
    static const int CLICK_TIMEOUT   = 400;  /**< Click timeout */
    static const int LONG_PRESS_TIME = 3000; /**< Long press time */
};

#endif