#include "TimeManager.h"

#include <WiFi.h>

#include "config.h"

time_t nowTime;

tm timeInfo;

TimeManager timeKeeper;

void TimeManager::begin() {

    rtcAvailable = rtc.begin();

    if (rtcAvailable) {

        Serial.println("RTC READY");

    } else {

        Serial.println(
            "RTC FAILED → USING NTP"
        );
    }

    configTime(
        19800,
        0,
        "pool.ntp.org",
        "time.nist.gov"
    );
}

void TimeManager::update() {

}

uint8_t TimeManager::getHour() {

    if (rtcAvailable) {

        DateTime now = rtc.now();

        return now.hour();
    }

    if (getLocalTime(&timeInfo)) {

        return timeInfo.tm_hour;
    }

    return 0;
}

uint8_t TimeManager::getMinute() {

    if (rtcAvailable) {

        DateTime now = rtc.now();

        return now.minute();
    }

    if (getLocalTime(&timeInfo)) {

        return timeInfo.tm_min;
    }

    return 0;
}

uint8_t TimeManager::getSecond() {

    if (rtcAvailable) {

        DateTime now = rtc.now();

        return now.second();
    }

    if (getLocalTime(&timeInfo)) {

        return timeInfo.tm_sec;
    }

    return 0;
}