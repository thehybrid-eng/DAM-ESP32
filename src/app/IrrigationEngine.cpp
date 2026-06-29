#include "IrrigationEngine.h"

#include "SystemState.h"

#include "RelaySDK.h"

#include "EventBus.h"

#include "config.h"

IrrigationEngine irrigationEngine;

void IrrigationEngine::begin()
{

}

void IrrigationEngine::update()
{
    auto &rules =
        deviceState.rules;

    // Humidity automation

    if (rules.autoHumidityMode)
    {
        if (deviceState.sensors.humidity <
            rules.minHumidity)
        {
            static bool triggered = false;

            if (!triggered)
            {
                RelaySDK::openValveForDuration(
                    0,
                    300000
                );

                EventBus::publish(
                    EVENT_SCHEDULE_TRIGGERED,
                    0,
                    "Humidity Irrigation"
                );

                triggered = true;
            }
        }
    }

    // Temperature protection

    if (deviceState.sensors.temperature >
        rules.maxTemperature)
    {
        EventBus::publish(
            EVENT_INVALID_COMMAND,
            255,
            "High Temperature Warning"
        );
    }

    // Low tank protection

    if (rules.disableOnLowTank)
    {
        for (int i = 0; i < 4; i++)
        {
            if (deviceState.sensors
                    .tankWeights[i]
                < 5.0)
            {
                RelaySDK::closeValve(i);

                EventBus::publish(
                    EVENT_INVALID_COMMAND,
                    i,
                    "Tank Protection Triggered"
                );
            }
        }
    }
}