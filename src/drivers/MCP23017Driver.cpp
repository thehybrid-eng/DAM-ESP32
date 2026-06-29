#include "MCP23017Driver.h"
#define IODIRA 0x00
#define IODIRB 0x01
#define GPIOA  0x12
#define GPIOB  0x13

MCP23017Driver mcpDriver;


bool MCP23017Driver::begin(uint8_t address) {

    _address = address;

    Wire.beginTransmission(_address);

    if (Wire.endTransmission() != 0) {
        return false;
    }

    Wire.beginTransmission(_address);
    Wire.write(IODIRA);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(_address);
    Wire.write(IODIRB);
    Wire.write(0x00);
    Wire.endTransmission();

    return true;
}

void MCP23017Driver::writePin(uint8_t pin, bool state) {

    uint8_t reg = (pin < 8) ? GPIOA : GPIOB;

    uint8_t bit = (pin < 8) ? pin : pin - 8;

    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(_address, (uint8_t)1);

    uint8_t value = Wire.read();

    if (state)
        value |= (1 << bit);
    else
        value &= ~(1 << bit);

    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

bool MCP23017Driver::readPin(uint8_t pin) {

    uint8_t reg = (pin < 8) ? GPIOA : GPIOB;

    uint8_t bit = (pin < 8) ? pin : pin - 8;

    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(_address, (uint8_t)1);

    uint8_t value = Wire.read();

    return (value & (1 << bit));
}