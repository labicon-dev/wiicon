#ifndef WIICON_H
#define WIICON_H

#include <Arduino.h>
#include <Wire.h>

#include "actions.h"
#include "bmi160.h"
#include "button_manager.h"
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

extern int accelMap[3];  /**< Accelerometer map */
extern int accelSign[3]; /**< Accelerometer sign */
extern int gyroMap[3];   /**< Gyroscope map */
extern int gyroSign[3]; /**< Gyroscope sign */

#endif  // WIICON_H
