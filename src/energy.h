#ifndef energy_h
#define energy_h
#include <config.h>
//#include <topic.h>
#include <ota.h>
#include <pin.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "ACS712.h"
WiFiClient espClient;
PubSubClient client(espClient);
ACS712 sensor(ACS712_20A, A0);
//void myOTAUpdate();
void reconnect();
void sendThing(uint16_t P,const char* topic,char* argomento);
void smartDelay(unsigned long ms);
void setup_wifi();
void calibrate();
float getCurrentAC(uint16_t frequency);
#endif
