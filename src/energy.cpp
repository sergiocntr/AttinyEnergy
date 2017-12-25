#include "energy.h"
#include "config.h"
void setup() {
  Serial.begin(9600);
  Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
  sensor.calibrate();
  setup_wifi();                   // Connect to wifi
  setupOTA();
  client.setServer(mqtt_server, mqtt_port);
  //client.setCallback(callback);
  Serial.println("Done!");
}

void setup_wifi() {
  //qui solo inizializzazione del wifi
  smartDelay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet); // Set static IP (2,7s) or 8.6s with DHCP  + 2s on battery
}
void loop() {
  // We use 230V because it is the common standard in European countries
  // Change to your local, if necessary
  float U = 230;
  for (char i = 0; i < 10; i++) if (WiFi.status() != WL_CONNECTED) delay(1000);
  //se scollegato ... dormi
  if (WiFi.status() != WL_CONNECTED){
      WiFi.disconnect();
      WiFi.mode( WIFI_OFF );
      WiFi.forceSleepBegin();
      delay( 1 );
      for (char z = 0; z < 60; z++) {
        //scaldaacqua();
        delay(5000);
      }
      WiFi.forceSleepWake();
      delay(1);
      //riparti d'accapo
      setup();
    }
  // To measure current we need to know the frequency of current
  // By default 50Hz is used, but you can specify own, if necessary
  float I = sensor.getCurrentAC();

  // To calculate the power we need voltage multiplied by current
  float P = U * I;

  Serial.println(String("I = ") + I + " A");
  Serial.println(String("P = ") + P + " Watts");
  smartDelay(100);
  reconnect();
  smartDelay(100);
  sendThing(P,ftopic,"power");
  smartDelay(5000);
}
void smartDelay(unsigned long ms){
  unsigned long start = millis();
  do
  {
    ArduinoOTA.handle();
    client.loop();
  } while (millis() - start < ms);
}
void reconnect() {
  //bool conn = False;
  for (char i = 0; i < 10; i++) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqttNode,mqttUser,mqttPass))
    {
      client.subscribe(ftopic);
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
