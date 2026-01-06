/**
 * @file        helpers.h
 * @brief       Helper functions for the WiiCon Remote project
 *
 * @author      See AUTHORS file for full list of contributors
 * @date        2025
 * @version     1.0.0
 *
 * ========================================================================================
 *
 * MIT License
 * Copyright (c) 2025 WiiCon Remote Contributors
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

#ifndef HELPERS_H
#define HELPERS_H

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>

#include "actions.h"
#include "bmi160.h"
#include "config.h"
#include "led_manager.h"
#include "logger.h"
#include "madgwick.h"
#include "osc_manager.h"

/**
 * Sends Euler angles via Serial
 * Output format: roll,pitch,yaw (in degrees)
 */
void sendEulerAngles();

/**
 * Initialize LittleFS filesystem
 */
void initLittleFS();

/**
 * Read a file from the filesystem
 * @param fs Filesystem reference (e.g., LittleFS)
 * @param path Path to the file
 * @return File content as String
 */
String readFile(fs::FS& fs, const char* path);

/**
 * Write content to a file in the filesystem
 * @param fs Filesystem reference (e.g., LittleFS)
 * @param path Path to the file
 * @param message Content to write
 */
void writeFile(fs::FS& fs, const char* path, const char* message);

#endif  // HELPERS_H
