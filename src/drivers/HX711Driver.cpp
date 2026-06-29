#include "HX711Driver.h"

bool HX711Driver::begin(uint8_t dout,
                        uint8_t sck) {

    scale.begin(dout, sck);

    scale.set_scale(calibrationFactor);

    scale.tare();

    return true;
}

float HX711Driver::readWeight() {

    return scale.get_units(5);
}