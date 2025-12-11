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
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

class WiFiManager {
   public:
    static WiFiManager& instance();

    void begin();
    void loop();
    void clearCredentials();
    bool isInAPMode() const { return _isAPMode; }
    bool isConnected() const { return WiFi.status() == WL_CONNECTED; }

    WiFiManager(const WiFiManager&)            = delete;
    WiFiManager& operator=(const WiFiManager&) = delete;

   private:
    WiFiManager();
    ~WiFiManager() = default;

    bool connect();
    void startAccessPoint();
    void setupCaptivePortal();
    void setupWebServer();
    void loadCredentials();
    void saveCredential(const char* path, const String& value);
    void redirectToCaptivePortal(AsyncWebServerRequest* request);

    String _ssid;
    String _password;
    String _ip;
    String _gateway;

    IPAddress _localIP;
    IPAddress _localGateway;
    IPAddress _localSubnet;

    AsyncWebServer _server;
    DNSServer      _dnsServer;

    bool _isAPMode;

    static constexpr const char* SSID_PATH     = "/ssid.txt";
    static constexpr const char* PASSWORD_PATH = "/password.txt";
    static constexpr const char* IP_PATH       = "/ip.txt";
    static constexpr const char* GATEWAY_PATH  = "/gateway.txt";

    static constexpr const char* PARAM_SSID     = "ssid";
    static constexpr const char* PARAM_PASSWORD = "password";
    static constexpr const char* PARAM_IP       = "ip";
    static constexpr const char* PARAM_GATEWAY  = "gateway";

    static constexpr uint8_t       DNS_PORT           = 53;
    static constexpr unsigned long CONNECTION_TIMEOUT = 10000;
};

extern WiFiManager& wifiManager;

#endif  // WIFI_MANAGER_H
