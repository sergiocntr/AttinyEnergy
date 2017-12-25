#ifndef config_h
#define config_h
#include <Arduino.h>
//WIFI stuff
#include <ESP8266WiFi.h>
const char* ssid     = "TIM-23836387";
const char* password = "51vEBuMvmALxNQHVIHQKkn52";
const char* webpass ="admin";
IPAddress ip(192, 168, 1, 213); //Node static EnergyMonitor
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
const char* mqtt_server = "192.168.1.100";
const uint16_t mqtt_port = 8883;
const char* mqttUser ="sergio";
const char* mqttPass ="donatella";
const char* mqttNode = "EnergyNode";
const char* ftopic = "/casa/interno/energia"; // 0 o 1 per attivare il riscaldamento
#endif
