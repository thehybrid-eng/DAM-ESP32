#include "SystemState.h"

SystemState deviceState;

void initSystemState()
{
    deviceState.emergencyActive = false;

    for (int i = 0; i < TOTAL_CHANNELS; i++)
    {
        auto &r = deviceState.relays[i];

        r.manualOverrideActive = false;

        r.overrideTimeout = 0;

        r.schedule.enabled = false;

        r.schedule.hour = 6;

        r.schedule.minute = 0;

        r.schedule.durationSec = 1800;

        r.relayState = false;
    }
}