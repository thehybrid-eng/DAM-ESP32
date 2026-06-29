#include "NetworkManager.h"

#include <WiFi.h>

#include "config.h"
#include "SystemState.h"

NetworkManager network;

void NetworkManager::begin()
{
    WiFi.mode(WIFI_STA);

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );

    Serial.println(
        "Connecting WiFi..."
    );

    int retry = 0;

    while (
        WiFi.status() != WL_CONNECTED &&
        retry < 30)
    {
        delay(500);

        Serial.print(".");

        retry++;
    }

    Serial.println();

    if (WiFi.status() ==
        WL_CONNECTED)
    {
        Serial.println(
            "WiFi Connected"
        );

        Serial.print(
            "IP Address: "
        );

        Serial.println(
            WiFi.localIP()
        );

        deviceState
            .sensors
            .wifiConnected = true;
    }
    else
    {
        Serial.println(
            "WiFi Failed"
        );

        deviceState
            .sensors
            .wifiConnected = false;
    }
}

void NetworkManager::update()
{
    deviceState
        .sensors
        .wifiConnected =
        (WiFi.status() ==
         WL_CONNECTED);
}

bool NetworkManager::isConnected()
{
    return (
        WiFi.status() ==
        WL_CONNECTED
    );
}