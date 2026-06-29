#include "RelaySDK.h"
#include "RelayController.h"
#include "SystemState.h"
#include "EventBus.h"

void RelaySDK::openValve(uint8_t zone) {

    relayController.setRelay(zone, true);
    EventBus::publish(
    EVENT_RELAY_ON,
    zone,
    "Relay ON"
);
}

void RelaySDK::closeValve(uint8_t zone) {

    relayController.setRelay(zone, false);
    EventBus::publish(
    EVENT_RELAY_OFF,
    zone,
    "Relay OFF"
);
}

void RelaySDK::openValveForDuration(
    uint8_t zone,
    uint32_t durationMs
) {

    relayController.setRelay(
        zone,
        true
    );

    deviceState.relays[zone].relayState =
        true;

    deviceState.relays[zone].offTime =
        millis() + durationMs;

    Serial.print("Valve ON Duration: ");

    Serial.println(zone);
}