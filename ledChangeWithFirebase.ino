//Formally known as FirebaseNeopixelTest2

#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
 
#define PIN D1
#define NUM_LEDS 8
 
const char* ssid = "MySpectrumWiFid0-2G";
const char* password = "bravebread422";
 
FirebaseData firebaseData;
 
Adafruit_NeoPixel leds(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
 
// Current color values
int redValue = 0;
int greenValue = 0;
int blueValue = 0;
 
void setup() {
  Serial.begin(9600);
  connectWifi();
  leds.begin();
   
  Firebase.begin("https://pulse-55391.firebaseio.com/", "3s0NdJBSlc33NNRQcEFwBB3NLnrHoxVyIk43tI4V");
}
 
void loop() {
 
  if (Firebase.getInt(firebaseData, "/red")) {
    if  (firebaseData.dataType() == "int") {
      int val = firebaseData.intData();
      if (val != redValue) {
        redValue = val;
         setLedColor();
      }
    }
  }
 
  if (Firebase.getInt(firebaseData, "/green")) {
    if  (firebaseData.dataType() == "int") {
      int val = firebaseData.intData();
      if (val != greenValue) {
        greenValue = val;
        setLedColor();
      }
    }
  }
 
  if (Firebase.getInt(firebaseData, "/blue")) {
    if  (firebaseData.dataType() == "int") {
      int val = firebaseData.intData();
      if (val != blueValue) {
        blueValue = val;
        setLedColor();
      }
    }
  }
}
 
void connectWifi() {
  // Let us connect to WiFi
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println(".......");
  Serial.println("WiFi Connected....IP Address:");
  Serial.println(WiFi.localIP());
 
}
 
void setLedColor() {
  for (int i=0; i < NUM_LEDS; i++) 
    leds.setPixelColor(i, leds.Color(redValue, greenValue, blueValue));
   leds.show();
}
