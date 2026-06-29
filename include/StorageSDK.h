#ifndef STORAGE_SDK_H
#define STORAGE_SDK_H
#include <Arduino.h>

class StorageSDK
{

public:
    static void saveSchedules();

    static void loadSchedules();

    static void enqueueTelemetry(
        String payload);

    static bool hasTelemetry();

    static String dequeueTelemetry();
};

#endif