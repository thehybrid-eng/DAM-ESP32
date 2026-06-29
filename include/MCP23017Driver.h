#ifndef MCP23017_DRIVER_H
#define MCP23017_DRIVER_H

#include <Arduino.h>
#include <Wire.h>

class MCP23017Driver {
public:
    bool begin(uint8_t address);

    void writePin(uint8_t pin, bool state);

    bool readPin(uint8_t pin);

private:
    uint8_t _address;
};

extern MCP23017Driver mcpDriver;

#endif