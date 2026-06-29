#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include <Arduino.h>

class RelayController {
public:
    void begin();

    void setRelay(uint8_t relay, bool state);

    void update();
};

extern RelayController relayController;

#endif