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
#include "helpers.h"
#include "logger.h"
#include <LittleFS.h>

const char* PARAM_SSID     = "ssid";
const char* PARAM_PASSWORD = "password";
const char* PARAM_IP       = "ip";
const char* PARAM_GATEWAY  = "gateway";

const char* ssidPath     = "/ssid.txt";
const char* passwordPath = "/password.txt";
const char* ipPath       = "/ip.txt";
const char* gatewayPath  = "/gateway.txt";

String ssid;
String password;
String ip;
String gateway;

IPAddress localIP;
IPAddress localGateway;
IPAddress localSubnet(255, 255, 255, 0);

AsyncWebServer server(80);

void setupWiFiManager()
{
    ssid     = readFile(LittleFS, ssidPath);
    password = readFile(LittleFS, passwordPath);
    ip       = readFile(LittleFS, ipPath);
    gateway  = readFile(LittleFS, gatewayPath);

    if (connectToWiFi())
    {
        Log::info("WiFi connected successfully");
        return;
    }

    Log::info("Starting WiFi Manager AP...");
    
    WiFi.softAP("Wiicon-Setup", NULL, 1, false, 1);

    IPAddress apIP = WiFi.softAPIP();
    Log::info("WiFi Manager AP started. IP: %s", apIP.toString().c_str());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(LittleFS, "/wifi_manager.html", "text/html");
    });

    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
    {
        int params = request->params();
        for (int i = 0; i < params; i++)
        {
            const AsyncWebParameter* p = request->getParam(i);
            if (p->isPost())
            {
                if (p->name() == PARAM_SSID)
                {
                    ssid = p->value();
                    writeFile(LittleFS, ssidPath, ssid.c_str());
                    Log::info("SSID saved: %s", ssid.c_str());
                }
                if (p->name() == PARAM_PASSWORD)
                {
                    password = p->value();
                    writeFile(LittleFS, passwordPath, password.c_str());
                    Log::info("Password saved");
                }
                if (p->name() == PARAM_IP)
                {
                    ip = p->value();
                    writeFile(LittleFS, ipPath, ip.c_str());
                    Log::info("IP saved: %s", ip.c_str());
                }
                if (p->name() == PARAM_GATEWAY)
                {
                    gateway = p->value();
                    writeFile(LittleFS, gatewayPath, gateway.c_str());
                    Log::info("Gateway saved: %s", gateway.c_str());
                }
            }
        }
        request->send(200, "text/plain", "Credentials saved. Restarting...");
        delay(2000);
        ESP.restart();
    });

    server.serveStatic("/", LittleFS, "/");
    server.begin();
    Log::info("WiFi Manager server started");
}

bool connectToWiFi()
{
    if (ssid.isEmpty() || password.isEmpty())
    {
        Log::warning("SSID or password is empty");
        return false;
    }

    WiFi.mode(WIFI_STA);

    if (!ip.isEmpty() && !gateway.isEmpty())
    {
        localIP.fromString(ip.c_str());
        localGateway.fromString(gateway.c_str());

        if (!WiFi.config(localIP, localGateway, localSubnet))
        {
            Log::error("Failed to configure static IP");
            return false;
        }
    }

    WiFi.begin(ssid.c_str(), password.c_str());
    Log::info("Connecting to WiFi: %s", ssid.c_str());

    unsigned long startTime = millis();
    const unsigned long timeout = 10000;

    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - startTime >= timeout)
        {
            Log::error("WiFi connection timeout");
            return false;
        }
        delay(100);
    }

    Log::info("Connected to WiFi!");
    Log::info("IP address: %s", WiFi.localIP().toString().c_str());

    return true;
}

