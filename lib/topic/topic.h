#ifndef topic_h
#define topic_h
#include <energy.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
class topic
{
  public:
    topic(char* mqttNode,char* mqttTopic);
    void reconnect();
    void sendThing(uint16_t myvalue,const char* topic,char* argomento);
  private:

  };
  #endif




/*
//TOPIC COMANDO IN INGRESSO -> subscrive

//const char* acquaTopic ="/casa/esterno/caldaia/acqua";  //0 o 1 per attivare il preriscaldamento acqua
//const char* resetTopic ="/casa/esterno/caldaia/reset"; //0 o 1 per premere pulsante reset
//TOPIC DI RISPOSTA/ALLARME -> publish
//const char* scaldaTopic ="/casa/esterno/caldaia/scalda/set"; // ON o OFF sensore attivita' caldaia
//const char* scaldaAcquaTopic ="/casa/esterno/caldaia/acqua/set"; // ON o OFF sensore attivita' caldaia
//const char* scaldaAvailableTopic ="/casa/esterno/caldaia/scalda"; // ON o OFF sensore attivita' caldaia

//const char* logTopic ="/casa/esterno/caldaia/log"; // temp H20 caldaia
void reconnect() {
  //bool conn = False;
  for (char i = 0; i < 10; i++) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("NodeEnergy",mqttUser,mqttPass))
    {
      Serial.println("connected");
      //conn = True;
      //client.publish(logTopic, "NodeMCU Caldaia connesso");
      client.subscribe(powerTopic);
      client.loop();

    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      smartDelay(5000);
    }
  }
  //if(!conn) loop();
}
void sendThing(uint16_t P,const char* topic,char* argomento) {
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder[argomento] = P;
  char JSONmessageBuffer[50];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  client.publish(topic, JSONmessageBuffer);
}

*/
