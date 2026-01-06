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
#include <LittleFS.h>
#include <WiFi.h>

#include "helpers.h"
#include "led_manager.h"
#include "logger.h"

class WiFiManager {
   public:
    /**
     * Get the singleton instance of the WiFi manager
     * @return Reference to the WiFi manager instance
     */
    static WiFiManager& instance();

    /**
     * Initialize the WiFi manager
     */
    void begin();

    /**
     * Loop the WiFi manager
     */
    void loop();

    /**
     * Clear the WiFi credentials
     */
    void clearCredentials();

    /**
     * Check if the WiFi manager is in AP mode
     * @return true if in AP mode, false otherwise
     */
    bool isInAPMode() const { return _isAPMode; }

    /**
     * Check if the WiFi manager is connected
     * @return true if connected, false otherwise
     */
    bool isConnected() const { return WiFi.status() == WL_CONNECTED; }

    /**
     * Get the OSC IP address
     * @return OSC IP address
     */
    String getOscIP() const { return _oscIP; } /**< OSC IP address */

    WiFiManager(const WiFiManager&)            = delete; /**< Delete copy constructor */
    WiFiManager& operator=(const WiFiManager&) = delete; /**< Delete assignment operator */

   private:
    /**
     * Constructor
     */
    WiFiManager();

    /**
     * Destructor
     */
    ~WiFiManager() = default;

    /**
     * Connect to the WiFi network
     * @return true if connected, false otherwise
     */
    bool connect();

    /**
     * Start the access point
     */
    void startAccessPoint();

    /**
     * Setup the captive portal
     */
    void setupCaptivePortal();

    /**
     * Setup the web server
     */
    void setupWebServer();

    /**
     * Load the WiFi credentials
     */
    void loadCredentials();

    /**
     * Save the WiFi credential
     */
    void saveCredential(const char* path, const String& value);

    /**
     * Redirect to the captive portal
     * @param request Request to redirect
     */
    void redirectToCaptivePortal(AsyncWebServerRequest* request);

    String _ssid;     /**< SSID of the WiFi network */
    String _password; /**< Password of the WiFi network */
    String _ip;       /**< IP address of the WiFi network */
    String _gateway;  /**< Gateway of the WiFi network */
    String _oscIP;    /**< OSC IP address */

    IPAddress _localIP;      /**< Local IP address */
    IPAddress _localGateway; /**< Local gateway */
    IPAddress _localSubnet;  /**< Local subnet */

    AsyncWebServer _server;    /**< Web server */
    DNSServer      _dnsServer; /**< DNS server */

    bool _isAPMode; /**< Whether the WiFi manager is in AP mode */

    /**
     * Path to the SSID file
     */
    static constexpr const char* SSID_PATH = "/ssid.txt";

    /**
     * Path to the password file
     */
    static constexpr const char* PASSWORD_PATH = "/password.txt";

    /**
     * Path to the IP file
     */
    static constexpr const char* IP_PATH = "/ip.txt";

    /**
     * Path to the gateway file
     */
    static constexpr const char* GATEWAY_PATH = "/gateway.txt";

    /**
     * Path to the OSC IP file
     */
    static constexpr const char* OSC_IP_PATH = "/osc_ip.txt";

    /**
     * Parameter for the SSID
     */
    static constexpr const char* PARAM_SSID = "ssid";

    /**
     * Parameter for the password
     */
    static constexpr const char* PARAM_PASSWORD = "password";

    /**
     * Parameter for the IP
     */
    static constexpr const char* PARAM_IP = "ip";

    /**
     * Parameter for the gateway
     */
    static constexpr const char* PARAM_GATEWAY = "gateway";

    /**
     * Parameter for the OSC IP
     */
    static constexpr const char* PARAM_OSC_IP = "osc_ip";

    /**
     * Port for the DNS server
     */
    static constexpr uint8_t DNS_PORT = 53;

    /**
     * Connection timeout
     */
    static constexpr unsigned long CONNECTION_TIMEOUT = 10000;
};

/**
 * Global instance of the WiFi manager
 */
extern WiFiManager& wifiManager;

#endif  // WIFI_MANAGER_H
