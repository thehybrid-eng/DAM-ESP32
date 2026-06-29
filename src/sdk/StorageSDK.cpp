#include "StorageSDK.h"

#include <Arduino.h>

#include <Preferences.h>

#include "SystemState.h"

#include <vector>

static std::vector<String>
telemetryQueue;

Preferences prefs;

void StorageSDK::saveSchedules()
{

    prefs.begin("scheduler", false);

    for (int i = 0; i < 16; i++)
    {

        auto &sched =
            deviceState.relays[i]
            .schedule;

        String base =
            "z" + String(i);

        prefs.putBool(
            (base + "e").c_str(),
            sched.enabled
        );

        prefs.putUChar(
            (base + "h").c_str(),
            sched.hour
        );

        prefs.putUChar(
            (base + "m").c_str(),
            sched.minute
        );

        prefs.putUInt(
            (base + "d").c_str(),
            sched.durationSec
        );
    }

    prefs.end();

    Serial.println(
        "Schedules Saved"
    );
}

void StorageSDK::loadSchedules()
{

    prefs.begin("scheduler", true);

    for (int i = 0; i < 16; i++)
    {

        auto &sched =
            deviceState.relays[i]
            .schedule;

        String base =
            "z" + String(i);

        sched.enabled =
            prefs.getBool(
                (base + "e").c_str(),
                false
            );

        sched.hour =
            prefs.getUChar(
                (base + "h").c_str(),
                0
            );

        sched.minute =
            prefs.getUChar(
                (base + "m").c_str(),
                0
            );

        sched.durationSec =
            prefs.getUInt(
                (base + "d").c_str(),
                0
            );
    }

    prefs.end();

    Serial.println(
        "Schedules Loaded"
    );
}

void StorageSDK::enqueueTelemetry(
    String payload)
{
    telemetryQueue.push_back(
        payload
    );

    Serial.println(
        "Telemetry Queued"
    );
}

bool StorageSDK::hasTelemetry()
{
    return !telemetryQueue.empty();
}

String StorageSDK::dequeueTelemetry()
{
    if (telemetryQueue.empty())
        return "";

    String payload =
        telemetryQueue.front();

    telemetryQueue.erase(
        telemetryQueue.begin()
    );

    return payload;
}