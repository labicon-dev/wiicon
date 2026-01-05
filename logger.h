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
#include <stdarg.h>

/**
 * Log levels
 */
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

    /**
     * Log a debug message
     * @param format Format string
     * @param ... Arguments
     */
    static void debug(const char* format, ...);

    /**
     * Log an info message
     * @param format Format string
     * @param ... Arguments
     */
    static void info(const char* format, ...);

    /**
     * Log a warning message
     * @param format Format string
     * @param ... Arguments
     */
    static void warning(const char* format, ...);

    /**
     * Log an error message
     * @param format Format string
     * @param ... Arguments
     */
    static void error(const char* format, ...);

    /**
     * Log a critical message
     * @param format Format string
     * @param ... Arguments
     */
    static void critical(const char* format, ...);

    /**
     * Log a message with a specific level
     * @param level Log level
     * @param format Format string
     * @param ... Arguments
     */
    static void log(LogLevel level, const char* format, ...);

    /**
     * Log a message with a specific level and location
     * @param level Log level
     * @param file File name
     * @param line Line number
     * @param format Format string
     * @param ... Arguments
     */
    static void logWithLocation(LogLevel level, const char* file, int line, const char* format, ...);

   private:
    /**
     * Current log level
     */
    static LogLevel _level;

    /**
     * Whether timestamp is enabled
     */
    static bool _timestampEnabled;

    /**
     * Buffer for the log message
     */
    static char _buffer[256];

    /**
     * Print a log message
     * @param level Log level
     * @param file File name
     * @param line Line number
     * @param message Message to print
     */
    static void printLog(LogLevel level, const char* file, int line, const char* message);

    /**
     * Convert a log level to a string
     * @param level Log level
     * @return String representation of the log level
     */
    static const char* levelToString(LogLevel level);
};

/**
 * Log macros
 */
#define LOG_DEBUG(fmt, ...) Log::logWithLocation(LOG_LEVEL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) Log::logWithLocation(LOG_LEVEL_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) Log::logWithLocation(LOG_LEVEL_WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Log::logWithLocation(LOG_LEVEL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) Log::logWithLocation(LOG_LEVEL_CRITICAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

/**
 * Disable debug messages
 * @param fmt Format string
 * @param ... Arguments
 */
#ifdef LOG_DISABLE_DEBUG
#define LOG_D(fmt, ...) ((void)0)
#else
#define LOG_D(fmt, ...) LOG_DEBUG(fmt, ##__VA_ARGS__)
#endif

#endif  // LOGGER_H
