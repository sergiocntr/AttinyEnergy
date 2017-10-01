#include <math.h>
#include <ota.h>
#include <caldaia.h>
#include <config.h>
#include <topic.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <nodeRelay.h>
#include <pin.h>

WiFiClient espClient;
//MQTT stuff
PubSubClient client(espClient);
