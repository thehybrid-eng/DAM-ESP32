#ifndef HX711_DRIVER_H
#define HX711_DRIVER_H

#include <Arduino.h>
#include <HX711.h>

class HX711Driver {

public:

    bool begin(uint8_t dout,
               uint8_t sck);

    float readWeight();

private:

    HX711 scale;

    float calibrationFactor = 5053.0f;
};

#endif