#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include "universal-mqtt.h"
#include <IRremote.h>

#define PUBLISH_DELAY 1000

unsigned long lastMillis = 0;
int PIN = 25;
IRsend irsend(PIN);

void setup() {
  Serial.begin(115200);

  setupCloudIoT();
}

void loop() {
  mqttClient->loop();
  if (!mqttClient->connected()) {
    connect();
  }
}

void clasificarAccion(String &payload){
    Serial.println("Antes: " );
  int accion =  payload.toInt();
  Serial.println(accion);
  switch(accion){
    case 10:
      //Encender
      Serial.println("Encender");
      irsend.sendNEC(0x2FD48B7, 32);
      delay(10);
      payload="on";
      break;
    case 11:
      //Apagar
      Serial.println("Apagar");
      irsend.sendNEC(0x2FD48B7, 32);
      delay(10);
      payload="off";
      break;
  }
  Serial.println("Después: ");
}

// The MQTT callback function for commands and configuration updates
// Place your message handler code here.
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  clasificarAccion(payload);
  // publish a message roughly every PUBLISH_DELAY ms.
  if (millis() - lastMillis > PUBLISH_DELAY) {
    lastMillis = millis();
    delay(50);  // <- fixes some issues with WiFi stability
     Serial.println("enviando: " + payload);
    publishTelemetry(payload);
  }
}
