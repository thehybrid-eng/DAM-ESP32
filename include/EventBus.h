#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <Arduino.h>

enum EventType
{
    EVENT_RELAY_ON,
    EVENT_RELAY_OFF,
    EVENT_SCHEDULE_TRIGGERED,
    EVENT_INVALID_COMMAND
};

struct SystemEvent
{
    EventType type;

    uint8_t zone;

    String message;
};

class EventBus
{
public:

    static void publish(
        EventType type,
        uint8_t zone,
        String message
    );

    static bool available();

    static SystemEvent get();
};

#endif