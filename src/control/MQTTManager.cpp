#include "MQTTManager.h"

#include <WiFi.h>

#include <PubSubClient.h>

#include <ArduinoJson.h>

#include "config.h"

#include "SystemState.h"

#include "RelaySDK.h"

#include "StorageSDK.h"

WiFiClient espClient;

PubSubClient mqttClient(espClient);

static unsigned long
lastMQTTMessage = 0;

void mqttCallback(
    char *topic,
    byte *payload,
    unsigned int length)
{
    // =====================================
    // MESSAGE INTERVAL
    // =====================================

    if (millis() - lastMQTTMessage <
        5000)
    {
        return;
    }

    lastMQTTMessage = millis();

    Serial.println(
        "\n========== MQTT MESSAGE ==========");

    Serial.print("Topic: ");

    Serial.println(topic);

    Serial.print("Raw Payload: ");

    for (unsigned int i = 0;
         i < length;
         i++)
    {
        Serial.print(
            (char)payload[i]);
    }

    Serial.println();

    // =====================================
    // JSON
    // =====================================

    StaticJsonDocument<1024> doc;

    DeserializationError error =
        deserializeJson(
            doc,
            payload,
            length);

    if (error)
    {
        Serial.print(
            "JSON Parse Failed: ");

        Serial.println(
            error.c_str());

        return;
    }

    Serial.println(
        "JSON Parsed Successfully");

    // =====================================
    // RELAY COMMAND
    // =====================================

    if (doc.containsKey("relay"))
    {
        int relay =
            doc["relay"] | -1;

        bool state =
            doc["state"] | false;

        Serial.print(
            "Relay Number: ");

        Serial.println(relay);

        Serial.print(
            "Relay State: ");

        Serial.println(
            state ? "ON" : "OFF");

        if (relay >= 0 &&
            relay < 16)
        {
            if (state)
            {
                RelaySDK
                    ::openValve(
                        relay);
            }
            else
            {
                RelaySDK
                    ::closeValve(
                        relay);
            }

            Serial.println(
                "Relay Command Executed");
        }
        else
        {
            Serial.println(
                "Invalid Relay Number");
        }
    }

    // =====================================
    // SCHEDULE
    // =====================================

    if (doc.containsKey(
            "schedule"))
    {
        int zone =
            doc["zone"] | 0;

        if (zone >= 0 &&
            zone < 16)
        {
            auto &sched =
                deviceState
                    .relays[zone]
                    .schedule;

            sched.enabled =
                doc["schedule"]
                   ["enabled"] |
                false;

            sched.hour =
                doc["schedule"]
                   ["hour"] |
                0;
                

            sched.minute =
                doc["schedule"]
                   ["minute"] |
                0;

            sched.durationSec =
                doc["schedule"]
                   ["duration_sec"] |
                0;

            StorageSDK
                ::saveSchedules();

            Serial.println(
                "Schedule Updated");

            Serial.print(
                "Zone: ");

            Serial.println(zone);

            Serial.print(
                "Hour: ");

            Serial.println(
                sched.hour);

            Serial.print(
                "Minute: ");

            Serial.println(
                sched.minute);

            Serial.print(
                "Duration: ");

            Serial.println(
                sched.durationSec);
        }
    }

    // =====================================
    // RULES
    // =====================================

    if (doc.containsKey(
            "rules"))
    {
        auto &rules =
            deviceState.rules;

        rules.autoHumidityMode =
            doc["rules"]
               ["autoHumidityMode"] |
            false;

        rules.minHumidity =
            doc["rules"]
               ["minHumidity"] |
            40.0;

        rules.maxTemperature =
            doc["rules"]
               ["maxTemperature"] |
            40.0;

        rules.disableOnLowTank =
            doc["rules"]
               ["disableOnLowTank"] |
            true;

        Serial.println(
            "Rules Updated");
    }

    Serial.println(
        "==================================");
}

void mqttInit()
{
    mqttClient.setServer(
        MQTT_BROKER,
        MQTT_PORT);

    mqttClient.setCallback(
        mqttCallback);
}

void mqttConnect()
{
    if (WiFi.status() !=
        WL_CONNECTED)
    {
        return;
    }

    if (mqttClient.connected())
    {
        return;
    }

    Serial.println(
        "Connecting MQTT...");

    if (mqttClient.connect(
            DEVICE_ID,
            MQTT_USERNAME,
            MQTT_PASSWORD))
    {
        Serial.println(
            "MQTT Connected");

        if (mqttClient.subscribe(
                MQTT_TOPIC_SUB))
        {
            Serial.println(
                "MQTT Subscribe OK");
        }
        else
        {
            Serial.println(
                "MQTT Subscribe Failed");
        }
    }
}

void mqttLoop()
{
    mqttClient.loop();
}