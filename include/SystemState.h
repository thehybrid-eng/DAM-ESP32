#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <Arduino.h>

#define TOTAL_CHANNELS 16
#define TOTAL_INPUTS 4

struct IrrigationSchedule
{
    bool enabled = false;

    uint8_t hour = 0;

    uint8_t minute = 0;

    uint32_t durationSec = 0;
};

struct IrrigationRules
{
    bool autoHumidityMode = false;

    float minHumidity = 40.0;

    float maxTemperature = 40.0;

    bool disableOnLowTank = true;
};


struct RelayState
{
    bool manualOverrideActive;

    unsigned long overrideTimeout;

    bool relayState = false;

    unsigned long offTime = 0;

    IrrigationSchedule schedule;
};

struct SensorData
{
    float tankWeights[4];

    float temperature;

    float humidity;

    bool wifiConnected;

    uint32_t uptimeMinutes;
};


struct SystemState
{
    bool emergencyActive;

    RelayState relays[TOTAL_CHANNELS];

    bool digitalInputs[TOTAL_INPUTS];

    SensorData sensors;

    IrrigationRules rules;
};


extern SystemState deviceState;

void initSystemState();

#endif