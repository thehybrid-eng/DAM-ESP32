#include "TankManager.h"
#include "HX711Driver.h"
#include "SystemState.h"

TankManager tanks;

HX711Driver tank1;
HX711Driver tank2;
// HX711Driver tank3;
// HX711Driver tank4;

void TankManager::begin() {

    tank1.begin(32,25);
    tank2.begin(33,25);
    // tank3.begin(34,25);
    // tank4.begin(35,25);

    Serial.println("HX711 Ready");
}

void TankManager::update() {

    deviceState.sensors.tankWeights[0] =
        tank1.readWeight();

    deviceState.sensors.tankWeights[1] =
        tank2.readWeight();

    // deviceState.sensors.tankWeights[2] =
    //     tank3.readWeight();

    // deviceState.sensors.tankWeights[3] =
    //     tank4.readWeight();
}