#include "DHT22Driver.h"

DHT22Driver dhtDriver;

void DHT22Driver::begin(uint8_t pin) {

    dht = new DHT(pin, DHT22);

    dht->begin();

    Serial.println("DHT22 Initialized");
}

void DHT22Driver::update() {

    static unsigned long lastRead = 0;

    if (millis() - lastRead < 2000) {
        return;
    }

    lastRead = millis();

    float t = dht->readTemperature();

    float h = dht->readHumidity();

    if (!isnan(t)) {

        temperature = t;
    }

    if (!isnan(h)) {

        humidity = h;
    }
}

float DHT22Driver::getTemperature() {

    return temperature;
}

float DHT22Driver::getHumidity() {

    return humidity;
}