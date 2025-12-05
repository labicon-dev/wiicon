/**
 * @file        logger.h
 * @brief       Global logger for the Wiicon Remote project
 *
 * @details     Provides a global logging facility that can be used from any module
 *              without creating instances. Supports multiple log levels and timestamps.
 *
 * @usage       Log::init(LOG_DEBUG);           // Initialize with desired level
 *              Log::info("System started");    // Log info message
 *              Log::debug("Value: %d", val);   // Log with formatting
 *              LOG_ERROR("Something failed");  // Macro with file/line info
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
 * SPDX-License-Identifier: MIT
 *
 * ========================================================================================
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

typedef enum {
    LOG_LEVEL_DEBUG    = 0,
    LOG_LEVEL_INFO     = 1,
    LOG_LEVEL_WARNING  = 2,
    LOG_LEVEL_ERROR    = 3,
    LOG_LEVEL_CRITICAL = 4,
    LOG_LEVEL_NONE     = 5
} LogLevel;

class Log {
public:
    /**
     * Initialize the logger with the minimum log level
     * @param level Minimum log level to display (messages below this are ignored)
     */
    static void init(LogLevel level = LOG_LEVEL_INFO);

    /**
     * Set the minimum log level at runtime
     * @param level New minimum log level
     */
    static void setLevel(LogLevel level);

    /**
     * Get the current log level
     */
    static LogLevel getLevel();

    /**
     * Enable/disable timestamp in logs
     */
    static void enableTimestamp(bool enable);

    static void debug(const char* format, ...);
    static void info(const char* format, ...);
    static void warning(const char* format, ...);
    static void error(const char* format, ...);
    static void critical(const char* format, ...);

    static void log(LogLevel level, const char* format, ...);

    static void logWithLocation(LogLevel level, const char* file, int line, const char* format, ...);

private:
    static LogLevel _level;
    static bool _timestampEnabled;
    static char _buffer[256];

    static void printLog(LogLevel level, const char* file, int line, const char* message);
    static const char* levelToString(LogLevel level);
};

#define LOG_DEBUG(fmt, ...)    Log::logWithLocation(LOG_LEVEL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)     Log::logWithLocation(LOG_LEVEL_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...)  Log::logWithLocation(LOG_LEVEL_WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)    Log::logWithLocation(LOG_LEVEL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) Log::logWithLocation(LOG_LEVEL_CRITICAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#ifdef LOG_DISABLE_DEBUG
    #define LOG_D(fmt, ...) ((void)0)
#else
    #define LOG_D(fmt, ...) LOG_DEBUG(fmt, ##__VA_ARGS__)
#endif

#endif // LOGGER_H
