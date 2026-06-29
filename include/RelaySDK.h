#ifndef RELAY_SDK_H
#define RELAY_SDK_H

#include <Arduino.h>

class RelaySDK
{

public:
    static void openValve(uint8_t zone);

    static void closeValve(uint8_t zone);

    static void openValveForDuration(
        uint8_t zone,
        uint32_t durationMs);
};

#endif