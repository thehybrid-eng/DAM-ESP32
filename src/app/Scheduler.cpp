#include "Scheduler.h"

#include "SystemState.h"

#include "TimeManager.h"

#include "RelaySDK.h"

Scheduler scheduler;

void Scheduler::begin()
{

    Serial.println("Scheduler Ready");
}

void Scheduler::update()
{

    uint8_t currentHour =
        timeKeeper.getHour();

    uint8_t currentMinute =
        timeKeeper.getMinute();

    static bool triggered[16] = {false};

    for (int i = 0; i < 16; i++)
    {

        auto &sched =
            deviceState.relays[i]
            .schedule;

        if (!sched.enabled)
            continue;

        if (currentHour == sched.hour &&
            currentMinute == sched.minute &&
            !triggered[i])
        {

            RelaySDK::openValveForDuration(
                i,
                sched.durationSec * 1000UL
            );

            Serial.println(
                "Scheduled Relay Triggered"
            );

            Serial.print("Zone: ");

            Serial.println(i);

            triggered[i] = true;
        }

        // reset trigger
        // after minute changes

        if (currentMinute != sched.minute)
        {

            triggered[i] = false;
        }
    }
}