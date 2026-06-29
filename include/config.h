#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID "Sumato Globaltech"
#define WIFI_PASSWORD "sumatoglobal@2026"

#define DEVICE_ID "ESP32_Irrigation_01"

#define MQTT_BROKER "mqtt.sumato.tech"
#define MQTT_PORT 1883

#define MQTT_TOPIC_EVENTS "drip/events"
#define MQTT_TOPIC_PUB "drip/status"
#define MQTT_TOPIC_SUB "drip/control"

#define MQTT_USERNAME "user1"
#define MQTT_PASSWORD "secret"

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

#define DHT22_PIN 4

#define MIN_TANK_WEIGHT 5.0

#define MAX_RUNTIME_MS 3600000

#define WIFI_FAIL_TIMEOUT 600000

#define FIRMWARE_VERSION "1.0.0"

#define TEST_MODE true

#endif