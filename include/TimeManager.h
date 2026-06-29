#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <Arduino.h>
#include <RTClib.h>

class TimeManager {

public:

    void begin();

    void update();

    uint8_t getHour();

    uint8_t getMinute();

    uint8_t getSecond();

private:

    RTC_DS3231 rtc;

    bool rtcAvailable = false;
};

extern TimeManager timeKeeper;

#endif