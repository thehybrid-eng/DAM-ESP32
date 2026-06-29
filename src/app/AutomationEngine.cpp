#include "AutomationEngine.h"

#include "SystemState.h"

#include "config.h"

#include "RelaySDK.h"

#include "EventBus.h"

static unsigned long lastEventMillis = 0;

void AutomationEngine::update()
{
    bool canPublishEvent =
        (millis() - lastEventMillis >
         60000);

    // Emergency stop

    if (deviceState.emergencyActive)
    {
        for (int i = 0; i < 16; i++)
        {
            RelaySDK::closeValve(i);
        }

        if (canPublishEvent)
        {
            EventBus::publish(
                EVENT_INVALID_COMMAND,
                255,
                "Emergency Stop Activated");

            lastEventMillis = millis();
        }

        return;
    }

    // Tank dry protection

    for (int i = 0; i < 2; i++)
    {
        if (!TEST_MODE &&
            deviceState.sensors
                    .tankWeights[i] < MIN_TANK_WEIGHT)
        {
            RelaySDK::closeValve(i);

            if (canPublishEvent)
            {
                EventBus::publish(
                    EVENT_INVALID_COMMAND,
                    i,
                    "Tank Empty Shutdown");

                lastEventMillis =
                    millis();
            }
        }
    }

    // Runtime protection

    for (int i = 0; i < 16; i++)
    {
        auto &relay =
            deviceState.relays[i];

        if (relay.relayState &&
            relay.offTime > 0 &&
            millis() >
                relay.offTime +
                    MAX_RUNTIME_MS)
        {
            RelaySDK::closeValve(i);

            if (canPublishEvent)
            {
                EventBus::publish(
                    EVENT_INVALID_COMMAND,
                    i,
                    "Relay Runtime Exceeded");

                lastEventMillis =
                    millis();
            }
        }
    }
}