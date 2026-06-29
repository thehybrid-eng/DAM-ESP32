#ifndef DHT22_DRIVER_H
#define DHT22_DRIVER_H

#include <Arduino.h>
#include <DHT.h>

class DHT22Driver {

public:

    void begin(uint8_t pin);

    void update();

    float getTemperature();

    float getHumidity();

private:

    DHT* dht = nullptr;

    float temperature = 0;

    float humidity = 0;
};

extern DHT22Driver dhtDriver;

#endif