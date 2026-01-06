/**
 * @file        osc_manager.cpp
 * @brief       OSC manager implementation for the Wiicon Remote project
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

#include "osc_manager.h"

OSCManager& oscManager = OSCManager::instance();

OSCManager& OSCManager::instance() {
    static OSCManager instance;
    return instance;
}

OSCManager::OSCManager() : _initialized(false), _bufferIndex(0) {}

bool OSCManager::begin() {
    if (_initialized) {
        return true;
    }

    if (!wifiManager.isConnected()) {
        Log::warning("OSC: WiFi not connected, cannot initialize");
        return false;
    }

    _initialized = true;
    Log::info("OSC initialized -> %s:%d", OSC_TARGET_IP, OSC_TARGET_PORT);
    return true;
}

bool OSCManager::isReady() const { return _initialized && wifiManager.isConnected() && !wifiManager.isInAPMode(); }

void OSCManager::sendEulerAngles(float roll, float pitch, float yaw) {
    if (!isReady()) {
        if (!_initialized && wifiManager.isConnected()) {
            begin();
        }
        if (!isReady()) return;
    }

    sendFloat3(OSC_ADDRESS_EULER, roll, pitch, yaw);
}

void OSCManager::sendFloat(const char* address, float value) {
    if (!isReady()) return;

    _bufferIndex = 0;

    writeOSCString(address);
    writeOSCString(",f");
    writeOSCFloat(value);

    IPAddress targetIP;
    targetIP.fromString(OSC_TARGET_IP);

    _udp.beginPacket(targetIP, OSC_TARGET_PORT);
    _udp.write(_buffer, _bufferIndex);
    _udp.endPacket();
}

void OSCManager::sendFloat3(const char* address, float v1, float v2, float v3) {
    if (!isReady()) return;

    _bufferIndex = 0;

    writeOSCString(address);
    writeOSCString(",fff");
    writeOSCFloat(v1);
    writeOSCFloat(v2);
    writeOSCFloat(v3);

    IPAddress targetIP;
    String    configIP = wifiManager.getOscIP();

    if (configIP.length() > 0) {
        targetIP.fromString(configIP.c_str());
    } else {
        IPAddress localIP = WiFi.localIP();
        IPAddress subnet  = WiFi.subnetMask();

        targetIP = IPAddress(localIP[0] | ~subnet[0], localIP[1] | ~subnet[1], localIP[2] | ~subnet[2],
                             localIP[3] | ~subnet[3]);
    }

    _udp.beginPacket(targetIP, OSC_TARGET_PORT);
    _udp.write(_buffer, _bufferIndex);
    _udp.endPacket();
}

void OSCManager::writeOSCString(const char* str) {
    size_t len = strlen(str);

    memcpy(_buffer + _bufferIndex, str, len);
    _bufferIndex += len;

    _buffer[_bufferIndex++] = '\0';

    padToFourBytes();
}

void OSCManager::writeOSCFloat(float value) {
    union {
        float    f;
        uint32_t i;
    } u;
    u.f = value;

    // OSC uses big-endian (network byte order)
    _buffer[_bufferIndex++] = (u.i >> 24) & 0xFF;
    _buffer[_bufferIndex++] = (u.i >> 16) & 0xFF;
    _buffer[_bufferIndex++] = (u.i >> 8) & 0xFF;
    _buffer[_bufferIndex++] = u.i & 0xFF;
}

void OSCManager::padToFourBytes() {
    while (_bufferIndex % 4 != 0) {
        _buffer[_bufferIndex++] = '\0';
    }
}
