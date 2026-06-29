#include "CloudSDK.h"

#include <PubSubClient.h>

#include <ArduinoJson.h>

#include "config.h"

#include "MQTTManager.h"

#include "SystemState.h"

#include "EventBus.h"

#include "StorageSDK.h"

extern PubSubClient mqttClient;

// ======================================================
// EVENT COOLDOWN
// ======================================================

static unsigned long
lastEventPublish = 0;

// ======================================================
// SYSTEM STATE PUBLISH
// ======================================================

void CloudSDK::publishSystemState()
{
    StaticJsonDocument<2048> doc;

    // =========================================
    // DEVICE INFO
    // =========================================

    doc["firmware"] =
        FIRMWARE_VERSION;

    doc["free_heap"] =
        ESP.getFreeHeap();

    doc["chip_revision"] =
        ESP.getChipRevision();

    // =========================================
    // SENSOR DATA
    // =========================================

    doc["wifi"] =
        deviceState.sensors
            .wifiConnected;

    doc["temperature"] =
        deviceState.sensors
            .temperature;

    doc["humidity"] =
        deviceState.sensors
            .humidity;

    doc["uptime"] =
        deviceState.sensors
            .uptimeMinutes;

    doc["tank1"] =
        deviceState.sensors
            .tankWeights[0];

    doc["tank2"] =
        deviceState.sensors
            .tankWeights[1];

    // =========================================
    // RULES
    // =========================================

    JsonObject rules =
        doc.createNestedObject(
            "rules");

    rules["autoHumidityMode"] =
        deviceState.rules
            .autoHumidityMode;

    rules["minHumidity"] =
        deviceState.rules
            .minHumidity;

    rules["maxTemperature"] =
        deviceState.rules
            .maxTemperature;

    rules["disableOnLowTank"] =
        deviceState.rules
            .disableOnLowTank;

    // =========================================
    // RELAYS
    // =========================================

    JsonArray relays =
        doc.createNestedArray(
            "relays");

    for (int i = 0; i < 16; i++)
    {
        JsonObject r =
            relays
                .createNestedObject();

        r["zone"] = i;

        r["state"] =
            deviceState
                .relays[i]
                .relayState;

        r["enabled"] =
            deviceState
                .relays[i]
                .schedule
                .enabled;

        r["hour"] =
            deviceState
                .relays[i]
                .schedule
                .hour;

        r["minute"] =
            deviceState
                .relays[i]
                .schedule
                .minute;

        r["duration"] =
            deviceState
                .relays[i]
                .schedule
                .durationSec;

        r["offTime"] =
            deviceState
                .relays[i]
                .offTime;

        r["manualOverride"] =
            deviceState
                .relays[i]
                .manualOverrideActive;
    }

    // =========================================
    // SERIALIZE
    // =========================================

    char buffer[2048];

    serializeJson(doc, buffer);

    // =========================================
    // MQTT SEND
    // =========================================

    if (mqttClient.connected())
    {
        mqttClient.publish(
            MQTT_TOPIC_PUB,
            buffer);

        Serial.println(
            "Cloud State Published");
    }
    else
    {
        StorageSDK
            ::enqueueTelemetry(
                String(buffer));

        Serial.println(
            "Telemetry Buffered");
    }
}

// ======================================================
// EVENT PUBLISH
// ======================================================

void CloudSDK::publishEvents()
{
    // =========================================
    // EVENT INTERVAL
    // =========================================

    if (millis() - lastEventPublish <
        60000)
    {
        return;
    }

    // =========================================
    // EVENT AVAILABLE
    // =========================================

    if (!EventBus::available())
    {
        return;
    }

    lastEventPublish = millis();

    // =========================================
    // GET EVENT
    // =========================================

    SystemEvent e =
        EventBus::get();

    StaticJsonDocument<256> doc;

    doc["zone"] =
        e.zone;

    doc["message"] =
        e.message;

    doc["type"] =
        e.type;

    char buffer[256];

    serializeJson(doc, buffer);

    // =========================================
    // MQTT SEND
    // =========================================

    if (mqttClient.connected())
    {
        mqttClient.publish(
            MQTT_TOPIC_EVENTS,
            buffer);

        Serial.println(
            "Event Published");
    }
    else
    {
        StorageSDK
            ::enqueueTelemetry(
                String(buffer));

        Serial.println(
            "Event Buffered");
    }
}