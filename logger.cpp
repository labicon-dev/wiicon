/**
 * @file        logger.cpp
 * @brief       Implementation of the logger functions for the Wiicon Remote project
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

#include "logger.h"

LogLevel Log::_level            = LOG_LEVEL_INFO; /**< Current log level */
bool     Log::_timestampEnabled = true;           /**< Whether timestamp is enabled */
char     Log::_buffer[256]      = {0};            /**< Buffer for the log message */

void Log::init(LogLevel level) {
    _level            = level;
    _timestampEnabled = true;
}

void Log::setLevel(LogLevel level) { _level = level; }

LogLevel Log::getLevel() { return _level; }

void Log::enableTimestamp(bool enable) { _timestampEnabled = enable; }

const char* Log::levelToString(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
        case LOG_LEVEL_INFO:
            return "INFO";
        case LOG_LEVEL_WARNING:
            return "WARN";
        case LOG_LEVEL_ERROR:
            return "ERROR";
        case LOG_LEVEL_CRITICAL:
            return "CRIT";
        default:
            return "???";
    }
}

void Log::printLog(LogLevel level, const char* file, int line, const char* message) {
    if (level < _level) {
        return;
    }

    if (_timestampEnabled) {
        Serial.print("[");
        Serial.print(millis());
        Serial.print("ms] ");
    }

    Serial.print("[");
    Serial.print(levelToString(level));
    Serial.print("] ");

    if (file != nullptr && level >= LOG_LEVEL_WARNING) {
        const char* filename = file;
        const char* p        = file;
        while (*p) {
            if (*p == '/' || *p == '\\') {
                filename = p + 1;
            }
            p++;
        }
        Serial.print("(");
        Serial.print(filename);
        Serial.print(":");
        Serial.print(line);
        Serial.print(") ");
    }

    Serial.println(message);
}

void Log::debug(const char* format, ...) {
    if (LOG_LEVEL_DEBUG < _level) return;

    va_list args;
    va_start(args, format);
    vsnprintf(_buffer, sizeof(_buffer), format, args);
    va_end(args);

    printLog(LOG_LEVEL_DEBUG, nullptr, 0, _buffer);
}

void Log::info(const char* format, ...) {
    if (LOG_LEVEL_INFO < _level) return;

    va_list args;
    va_start(args, format);
    vsnprintf(_buffer, sizeof(_buffer), format, args);
    va_end(args);

    printLog(LOG_LEVEL_INFO, nullptr, 0, _buffer);
}

void Log::warning(const char* format, ...) {
    if (LOG_LEVEL_WARNING < _level) return;

    va_list args;
    va_start(args, format);
    vsnprintf(_buffer, sizeof(_buffer), format, args);
    va_end(args);

    printLog(LOG_LEVEL_WARNING, nullptr, 0, _buffer);
}

void Log::error(const char* format, ...) {
    if (LOG_LEVEL_ERROR < _level) return;

    va_list args;
    va_start(args, format);
    vsnprintf(_buffer, sizeof(_buffer), format, args);
    va_end(args);

    printLog(LOG_LEVEL_ERROR, nullptr, 0, _buffer);
}

void Log::critical(const char* format, ...) {
    if (LOG_LEVEL_CRITICAL < _level) return;

    va_list args;
    va_start(args, format);
    vsnprintf(_buffer, sizeof(_buffer), format, args);
    va_end(args);

    printLog(LOG_LEVEL_CRITICAL, nullptr, 0, _buffer);
}

void Log::log(LogLevel level, const char* format, ...) {
    if (level < _level) return;

    va_list args;
    va_start(args, format);
    vsnprintf(_buffer, sizeof(_buffer), format, args);
    va_end(args);

    printLog(level, nullptr, 0, _buffer);
}

void Log::logWithLocation(LogLevel level, const char* file, int line, const char* format, ...) {
    if (level < _level) return;

    va_list args;
    va_start(args, format);
    vsnprintf(_buffer, sizeof(_buffer), format, args);
    va_end(args);

    printLog(level, file, line, _buffer);
}
