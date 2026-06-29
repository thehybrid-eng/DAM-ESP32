#include "RelayController.h"
#include "MCP23017Driver.h"
#include "SystemState.h"

RelayController relayController;

void RelayController::begin() {

    bool ok = mcpDriver.begin(0x20);

    if (ok) {
        Serial.println("MCP23017 Ready");
    } else {
        Serial.println("MCP23017 Failed");
    }
}

void RelayController::setRelay(uint8_t relay, bool state) {

    if (relay >= 16) return;

    deviceState.relays[relay].relayState = state;

    mcpDriver.writePin(relay, state);
}

void RelayController::update() {

}