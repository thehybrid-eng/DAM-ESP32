#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <PubSubClient.h>

#include "config.h"
#include "SystemState.h"

#include "NetworkManager.h"
#include "MQTTManager.h"

#include "RelayController.h"

#include "IrrigationEngine.h"
#include "AutomationEngine.h"

#include "TankManager.h"
#include "DHT22Driver.h"

#include "MCP23017Driver.h"
#include "HX711Driver.h"

#include "TimeManager.h"
#include "Scheduler.h"

#include "StorageSDK.h"
#include "CloudSDK.h"

#include "OTASDK.h"

extern PubSubClient mqttClient;

// TASK HANDLES

TaskHandle_t mqttTaskHandle;

TaskHandle_t cloudTaskHandle;

TaskHandle_t automationTaskHandle;

TaskHandle_t sensorTaskHandle;

// MQTT TASK

void mqttTask(void *pv)
{
    while (true)
    {
        network.update();

        mqttConnect();

        mqttLoop();

        OTASDK::update();

        vTaskDelay(
            50 / portTICK_PERIOD_MS
        );
    }
}

// AUTOMATION TASK

void automationTask(void *pv)
{
    while (true)
    {
        //scheduler.update();

        //irrigationEngine.update();

        //AutomationEngine::update();

        vTaskDelay(
            100 / portTICK_PERIOD_MS
        );
    }
}

// SENSOR TASK

void sensorTask(void *pv)
{
    while (true)
    {
        tanks.update();

        dhtDriver.update();

        timeKeeper.update();

        deviceState.sensors.temperature =
            dhtDriver.getTemperature();

        deviceState.sensors.humidity =
            dhtDriver.getHumidity();

        deviceState.sensors.uptimeMinutes =
            millis() / 60000;

        vTaskDelay(
            2000 / portTICK_PERIOD_MS
        );
    }
}

// CLOUD TASK

void cloudTask(void *pv)
{
    while (true)
    {
        //CloudSDK::publishEvents();

        // static unsigned long lastCloud = 0;

        // if (millis() - lastCloud > 300000)
        // {
        //     lastCloud = millis();

        //     CloudSDK::publishSystemState();
        // }

        // resend buffered telemetry

        if (mqttClient.connected())
        {
            while (
                StorageSDK::hasTelemetry())
            {
                String payload =
                    StorageSDK
                    ::dequeueTelemetry();

                mqttClient.publish(
                    MQTT_TOPIC_PUB,
                    payload.c_str());

                Serial.println(
                    "Buffered Telemetry Sent");
            }
        }

        vTaskDelay(
            1000 / portTICK_PERIOD_MS
        );
    }
}

// SETUP

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println(
        "Industrial Controller Boot");

    Wire.begin(
        I2C_SDA_PIN,
        I2C_SCL_PIN
    );

    // SYSTEM INIT

    initSystemState();

    StorageSDK::loadSchedules();

    network.begin();

    mqttInit();

    relayController.begin();

    irrigationEngine.begin();

    tanks.begin();

    dhtDriver.begin(
        DHT22_PIN
    );

    timeKeeper.begin();

    scheduler.begin();

    // OTA START
    // AFTER WIFI INIT

    OTASDK::begin();

    // TASKS

    xTaskCreatePinnedToCore(
        mqttTask,
        "MQTTTask",
        8192,
        NULL,
        1,
        &mqttTaskHandle,
        0
    );

    xTaskCreatePinnedToCore(
        automationTask,
        "AutomationTask",
        8192,
        NULL,
        1,
        &automationTaskHandle,
        1
    );

    xTaskCreatePinnedToCore(
        sensorTask,
        "SensorTask",
        8192,
        NULL,
        1,
        &sensorTaskHandle,
        1
    );

    xTaskCreatePinnedToCore(
        cloudTask,
        "CloudTask",
        8192,
        NULL,
        1,
        &cloudTaskHandle,
        0
    );

    Serial.println(
        "RTOS Tasks Started");
}

// MAIN LOOP

void loop()
{
    vTaskDelay(
        1000 / portTICK_PERIOD_MS
    );
}