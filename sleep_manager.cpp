/**
 * @file        sleep_manager.cpp
 * @brief       Implementation of the deep sleep manager for the Wiicon Remote project
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

#include "sleep_manager.h"

void initSleepManager() {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << BUTTON_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    Log::info("Sleep manager initialized on LP GPIO %d", BUTTON_PIN);

    checkWakeupCause();
}

void goToSleep() {
    Log::info("Preparing for Deep Sleep...");

    if (digitalRead(BUTTON_PIN) == LOW) {
        Log::info("Release the button to sleep...");
        while (digitalRead(BUTTON_PIN) == LOW) {
            delay(100);
        }
        delay(200);
    }

    Serial.flush();

    esp_deep_sleep_enable_gpio_wakeup(1ULL << BUTTON_PIN, ESP_GPIO_WAKEUP_GPIO_LOW);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

    Log::info("Good night! :)");
    esp_deep_sleep_start();
}

bool checkWakeupCause() {
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();

    switch (cause) {
        case ESP_SLEEP_WAKEUP_GPIO:
        case ESP_SLEEP_WAKEUP_EXT1:
            Log::info("Wakeup cause: Button pressed");
            return true;

        case ESP_SLEEP_WAKEUP_TIMER:
            Log::info("Wakeup cause: Timer");
            return false;

        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            Log::info("Wakeup cause: Normal boot / Reset");
            return false;
    }
}

bool isWakeButtonPressed() { return digitalRead(BUTTON_PIN) == LOW; }
