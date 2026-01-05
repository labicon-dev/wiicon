#ifndef WIICON_H
#define WIICON_H

#include <Arduino.h>
#include <Wire.h>

#include "bmi160.h"
#include "config.h"
#include "helpers.h"
#include "led_manager.h"
#include "logger.h"
#include "madgwick.h"
#include "osc_manager.h"
#include "sleep_manager.h"
#include "wifi_manager.h"

/**
 * Setup function
 */
void setup();

/**
 * Loop function
 */
void loop();

extern unsigned long lastTime;

int accelMap[3]  = {0, 1, 2}; /**< Accelerometer map */
int accelSign[3] = {1, 1, 1}; /**< Accelerometer sign */
int gyroMap[3]   = {0, 1, 2}; /**< Gyroscope map */
int gyroSign[3]  = {1, 1, 1}; /**< Gyroscope sign */

#endif  // WIICON_H
