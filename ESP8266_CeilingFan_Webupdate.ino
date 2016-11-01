#define CAYENNE_DEBUG         // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#define roomLight 4
#define fanOff 5
#define fanLow 12
#define fanMed 14
#define fanHigh 13

int lastDimLevel = 0;

#include <CayenneDefines.h>
#include <BlynkSimpleEsp8266.h>
#include <CayenneWiFiClient.h>
#include <CayenneClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

// Cayenne authentication token. This is obtained from the Cayenne Dashboard.
const char* token = "w59t22fwxx";
const char* ssid = "Wu-Tang LAN";
const char* password = "allstream11";
const char* host = "esp8266-webupdate";

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting Sketch...");
  pinMode(A0, INPUT);
  pinMode(roomLight, OUTPUT);
  pinMode(fanOff, OUTPUT);
  pinMode(fanLow, OUTPUT);
  pinMode(fanMed, OUTPUT);
  pinMode(fanHigh, OUTPUT);

  digitalWrite(roomLight, LOW);
  digitalWrite(fanOff, LOW);
  digitalWrite(fanLow, LOW);
  digitalWrite(fanMed, LOW);
  digitalWrite(fanHigh, LOW);
  
  Cayenne.begin(token, ssid, password);

  MDNS.begin(host);
  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
}

void loop()
{
  httpServer.handleClient();
  Cayenne.run();
}

CAYENNE_IN(V0)
{
 push(roomLight); 
}

CAYENNE_IN(V1)
{
  int sliderState = round(getValue.asInt()/1000.0);

  switch(sliderState) {
    case 0:
    {
      push(fanOff);
      break;
    }
    case 1:
    {
      push(fanLow);
      break;
    }
    case 2:
    {
      push(fanMed);
      break;
    }
    case 3:
    {
      push(fanHigh);
      break;
    }
  }
  
}

CAYENNE_IN(V2)
{
  int dimLevel = round(getValue.asInt()/1000.0);
  int dimDelay = dimLevel - lastDimLevel;
  
  if(dimDelay < 0){
    dimDelay = 10 - dimDelay;
  }
  lastDimLevel = dimLevel;

  digitalWrite(roomLight, HIGH);
  delay(1000*0.605*dimDelay);
  digitalWrite(roomLight, LOW);
}

void push(int button){
  digitalWrite(button, HIGH);
  delay(100);
  digitalWrite(button, LOW);
}

