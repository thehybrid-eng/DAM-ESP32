#include "EventBus.h"

static SystemEvent lastEvent;

static bool hasEvent = false;

void EventBus::publish(
    EventType type,
    uint8_t zone,
    String message)
{
    lastEvent.type = type;

    lastEvent.zone = zone;

    lastEvent.message = message;

    hasEvent = true;
}

bool EventBus::available()
{
    return hasEvent;
}

SystemEvent EventBus::get()
{
    hasEvent = false;

    return lastEvent;
}