/**
 * @file        wifi_manager.cpp
 * @brief       WiFi Manager implementation for the Wiicon Remote project
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

#include "wifi_manager.h"

WiFiManager& wifiManager = WiFiManager::instance();

WiFiManager& WiFiManager::instance() {
    static WiFiManager instance;
    return instance;
}

WiFiManager::WiFiManager() : _localSubnet(255, 255, 255, 0), _server(80), _isAPMode(false) {}

void WiFiManager::begin() {
    loadCredentials();

    if (connect()) {
        Log::info("WiFi connected successfully");
        return;
    }

    startAccessPoint();
}

void WiFiManager::loop() {
    if (_isAPMode) {
        _dnsServer.processNextRequest();
        LedManager::signalAPMode();
    }
}

void WiFiManager::clearCredentials() {
    writeFile(LittleFS, SSID_PATH, "");
    writeFile(LittleFS, PASSWORD_PATH, "");
    writeFile(LittleFS, IP_PATH, "");
    writeFile(LittleFS, GATEWAY_PATH, "");
    writeFile(LittleFS, OSC_IP_PATH, "");
    Log::info("WiFi credentials cleared");
}

void WiFiManager::loadCredentials() {
    _ssid     = readFile(LittleFS, SSID_PATH);
    _password = readFile(LittleFS, PASSWORD_PATH);
    _ip       = readFile(LittleFS, IP_PATH);
    _gateway  = readFile(LittleFS, GATEWAY_PATH);
    _oscIP    = readFile(LittleFS, OSC_IP_PATH);
}

void WiFiManager::saveCredential(const char* path, const String& value) { writeFile(LittleFS, path, value.c_str()); }

bool WiFiManager::connect() {
    if (_ssid.isEmpty() || _password.isEmpty()) {
        Log::warning("SSID or password is empty");
        return false;
    }

    WiFi.mode(WIFI_STA);

    if (!_ip.isEmpty() && !_gateway.isEmpty()) {
        _localIP.fromString(_ip.c_str());
        _localGateway.fromString(_gateway.c_str());

        if (!WiFi.config(_localIP, _localGateway, _localSubnet)) {
            Log::error("Failed to configure static IP");
            return false;
        }
        Log::info("Using static IP: %s", _ip.c_str());
    } else {
        WiFi.config(IPAddress(0, 0, 0, 0), IPAddress(0, 0, 0, 0), IPAddress(0, 0, 0, 0));
        Log::info("Using DHCP for automatic IP assignment");
    }

    WiFi.begin(_ssid.c_str(), _password.c_str());
    Log::info("Connecting to WiFi: %s", _ssid.c_str());

    unsigned long startTime = millis();

    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startTime >= CONNECTION_TIMEOUT) {
            Log::error("WiFi connection timeout");
            LedManager::signalErrorGeneral();
            return false;
        }
        LedManager::signalWifiConnecting();
        delay(10);
    }

    LedManager::signalSuccess();
    delay(1000);
    LedManager::off();

    Log::info("Connected to WiFi!");
    Log::info("IP address: %s", WiFi.localIP().toString().c_str());

    return true;
}

void WiFiManager::startAccessPoint() {
    Log::info("Starting WiFi Manager AP...");

    _isAPMode = true;
    WiFi.softAP("WiiCon Setup", NULL, 1, false, 1);

    IPAddress apIP = WiFi.softAPIP();
    Log::info("WiFi Manager AP started. IP: %s", apIP.toString().c_str());

    setupCaptivePortal();
    setupWebServer();

    _server.begin();
    Log::info("WiFi Manager server started");
    Log::info("AP Mode active");
}

void WiFiManager::setupCaptivePortal() {
    IPAddress apIP = WiFi.softAPIP();

    _dnsServer.start(DNS_PORT, "*", apIP);
    Log::info("DNS server started for captive portal");

    // Android
    _server.on("/generate_204", HTTP_GET, [this](AsyncWebServerRequest* request) { redirectToCaptivePortal(request); });
    _server.on("/gen_204", HTTP_GET, [this](AsyncWebServerRequest* request) { redirectToCaptivePortal(request); });

    // iOS / macOS
    _server.on("/hotspot-detect.html", HTTP_GET,
               [this](AsyncWebServerRequest* request) { redirectToCaptivePortal(request); });
    _server.on("/library/test/success.html", HTTP_GET,
               [this](AsyncWebServerRequest* request) { redirectToCaptivePortal(request); });

    // Windows
    _server.on("/connecttest.txt", HTTP_GET,
               [this](AsyncWebServerRequest* request) { redirectToCaptivePortal(request); });
    _server.on("/ncsi.txt", HTTP_GET, [this](AsyncWebServerRequest* request) { redirectToCaptivePortal(request); });

    // Firefox
    _server.on("/success.txt", HTTP_GET, [this](AsyncWebServerRequest* request) { redirectToCaptivePortal(request); });

    // Not found
    _server.onNotFound([this](AsyncWebServerRequest* request) { redirectToCaptivePortal(request); });
}

void WiFiManager::setupWebServer() {
    // Main page
    _server.on("/", HTTP_GET,
               [](AsyncWebServerRequest* request) { request->send(LittleFS, "/wifi_manager.html", "text/html"); });

    // WiFi network scan
    _server.on("/scan", HTTP_GET, [](AsyncWebServerRequest* request) {
        int    n    = WiFi.scanNetworks();
        String json = "[";

        for (int i = 0; i < n; i++) {
            if (i > 0) json += ",";
            json += "{\"ssid\":\"" + WiFi.SSID(i) + "\",";
            json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
            json += "\"secure\":" + String(WiFi.encryptionType(i) != WIFI_AUTH_OPEN) + "}";
        }

        json += "]";
        WiFi.scanDelete();
        request->send(200, "application/json", json);
    });

    // Form submission
    _server.on("/", HTTP_POST, [this](AsyncWebServerRequest* request) {
        int params = request->params();

        for (int i = 0; i < params; i++) {
            const AsyncWebParameter* p = request->getParam(i);

            if (!p->isPost()) continue;

            if (p->name() == PARAM_SSID) {
                _ssid = p->value();
                saveCredential(SSID_PATH, _ssid);
                Log::info("SSID saved: %s", _ssid.c_str());
            } else if (p->name() == PARAM_PASSWORD) {
                _password = p->value();
                saveCredential(PASSWORD_PATH, _password);
                Log::info("Password saved");
            } else if (p->name() == PARAM_IP) {
                _ip = p->value();
                saveCredential(IP_PATH, _ip);
                Log::info("IP saved: %s", _ip.c_str());
            } else if (p->name() == PARAM_GATEWAY) {
                _gateway = p->value();
                saveCredential(GATEWAY_PATH, _gateway);
                Log::info("Gateway saved: %s", _gateway.c_str());
            } else if (p->name() == PARAM_OSC_IP) {
                _oscIP = p->value();
                saveCredential(OSC_IP_PATH, _oscIP);
                Log::info("OSC IP saved: %s", _oscIP.c_str());
            }
        }

        request->send(200, "text/plain", "Credentials saved. Restarting...");
        delay(2000);
        ESP.restart();
    });

    // Static files
    _server.serveStatic("/", LittleFS, "/");
}

void WiFiManager::redirectToCaptivePortal(AsyncWebServerRequest* request) {
    request->redirect("http://" + WiFi.softAPIP().toString() + "/");
}
