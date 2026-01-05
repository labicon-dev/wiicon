/**
 * @file        osc_manager.h
 * @brief       OSC (Open Sound Control) manager for the Wiicon Remote project
 *
 * @details     Provides functions for sending sensor data via OSC over UDP.
 *              Supports configurable target IP, port, and OSC address patterns.
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

#ifndef OSC_MANAGER_H
#define OSC_MANAGER_H

#include <Arduino.h>
#include <WiFiUdp.h>

class OSCManager {
   public:
    static OSCManager& instance();

    /**
     * Initialize the OSC manager
     * @return true if initialization was successful
     */
    bool begin();

    /**
     * Send Euler angles via OSC
     * @param roll Roll angle in degrees
     * @param pitch Pitch angle in degrees
     * @param yaw Yaw angle in degrees
     */
    void sendEulerAngles(float roll, float pitch, float yaw);

    /**
     * Send a single float value via OSC
     * @param address OSC address pattern (e.g., "/wiicon/roll")
     * @param value Float value to send
     */
    void sendFloat(const char* address, float value);

    /**
     * Send three float values via OSC
     * @param address OSC address pattern (e.g., "/wiicon/euler")
     * @param v1 First float value
     * @param v2 Second float value
     * @param v3 Third float value
     */
    void sendFloat3(const char* address, float v1, float v2, float v3);

    /**
     * Check if OSC is ready to send (WiFi connected, not in AP mode)
     * @return true if ready
     */
    bool isReady() const;

    OSCManager(const OSCManager&)            = delete;
    OSCManager& operator=(const OSCManager&) = delete;

   private:
    OSCManager();
    ~OSCManager() = default;

    void writeOSCString(const char* str);
    void writeOSCFloat(float value);
    void padToFourBytes();

    WiFiUDP  _udp;
    bool     _initialized;
    uint8_t  _buffer[256];
    size_t   _bufferIndex;
};

extern OSCManager& oscManager;

#endif  // OSC_MANAGER_H

