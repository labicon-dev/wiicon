/**
 * @file        wifi_manager.h
 * @brief       WiFi Manager for the Wiicon Remote project
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

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

extern const char* PARAM_SSID;
extern const char* PARAM_PASSWORD;
extern const char* PARAM_IP;
extern const char* PARAM_GATEWAY;

extern const char* ssidPath;
extern const char* passwordPath;
extern const char* ipPath;
extern const char* gatewayPath;

extern String ssid;
extern String password;
extern String ip;
extern String gateway;

extern IPAddress localIP;
extern IPAddress localGateway;
extern IPAddress localSubnet;

extern AsyncWebServer server;

/**
 * Clear all WiFi config entries
 */
void clearNetwork();

/**
 * Setup WiFi Manager
 * Tries to connect to saved WiFi, or starts AP mode for configuration
 */
void setupWiFiManager();

/**
 * Connect to WiFi using saved credentials
 * @return true if connected successfully
 */
bool connectToWiFi();

#endif  // WIFI_MANAGER_H
