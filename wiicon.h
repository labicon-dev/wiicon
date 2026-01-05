#ifndef WIICON_H
#define WIICON_H

#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "bmi160.h"
#include "madgwick.h"
#include "helpers.h"
#include "logger.h"
#include "wifi_manager.h"
#include "sleep_manager.h"
#include "osc_manager.h"

void setup();
void loop();

#endif // WIICON_H
