#include "OTASDK.h"

#include <WiFi.h>

#include <ArduinoOTA.h>

void OTASDK::begin()
{
    ArduinoOTA
        .onStart([]()
    {
        Serial.println(
            "OTA Start"
        );
    });

    ArduinoOTA
        .onEnd([]()
    {
        Serial.println(
            "OTA Complete"
        );
    });

    ArduinoOTA
        .onProgress([](
            unsigned int progress,
            unsigned int total)
    {
        Serial.printf(
            "OTA Progress: %u%%\n",
            (progress * 100) / total
        );
    });

    ArduinoOTA
        .onError([](
            ota_error_t error)
    {
        Serial.printf(
            "OTA Error[%u]\n",
            error
        );
    });

    ArduinoOTA.begin();

    Serial.println(
        "OTA Ready"
    );
}

void OTASDK::update()
{
    ArduinoOTA.handle();
}