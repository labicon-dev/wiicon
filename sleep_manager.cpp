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
#include "config.h"
#include "logger.h"
#include "esp_sleep.h"

#define WAKE_BITMASK (1ULL << WAKE_PIN)

void initSleepManager()
{
    pinMode(WAKE_PIN, INPUT);
    
    Log::info("Sleep manager initialized (wake pin: GPIO%d)", WAKE_PIN);
    
    checkWakeupCause();
}

void goToSleep()
{
    Log::info("Entering Deep Sleep...");
    Serial.flush();

    esp_sleep_enable_ext1_wakeup(WAKE_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

    esp_deep_sleep_start();
}

bool checkWakeupCause()
{
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    
    switch (cause)
    {
        case ESP_SLEEP_WAKEUP_EXT1:
            Log::info("Wakeup cause: Button pressed (EXT1)");
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

bool isWakeButtonPressed()
{
    return digitalRead(WAKE_PIN) == HIGH;
}

