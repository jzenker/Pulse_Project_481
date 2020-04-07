//Josephine Zenker
//Code modified from Sparkfun MAX3010x Pulse and Proximity Sensor Library
//Last modified: March 19, 2020
//Code for serial communication Arduino Uno (sender)

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <math.h>

#include<SoftwareSerial.h>
SoftwareSerial ArduinoUno ( 12,11); //RX: 2, TX: 3

MAX30105 particleSensor; //heart rate monitor object

const byte RATE_SIZE = 4; //Increase this for more averaging samples
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat;  //Time at which the last beat occurred

float beatsPerMinute;
float beatAvg; //raw value of HR
float oldValue; //old value of HR


void setup() {
  Serial.begin(115200); //enable Serial Monitor
  ArduinoUno.begin(115200); //enable SUART Port

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0x0A); //Turn Green LED to low

}

void loop() {
  readBPM();

}

void readBPM() {
  String stringValue; //Will hold average BPM to convert to a string
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  //Prints out avg BPM only if between these resting heart rate values (50-150)
  //And if the value isn't an immediate repeat of the last one
  //print anything thats changed except for null values

  if (beatAvg >= 50 && beatAvg <= 150 && beatAvg != oldValue) {
    oldValue = beatAvg;

    String stringValue;

    stringValue = convertFloatToString(beatAvg); //originally, I wanted to convert from a float to a string

    delay(5000);
    Serial.println(stringValue);
    ArduinoUno.println(stringValue); //prints this to the Arduino's serial
  }
}


//Function modified from bacharakis on codebender
//This function works and has been tested
//Value array size should be 3 and dtostrf should be (convertValue, 3, 0, value) but only this work
String convertFloatToString(float convertValue) {
  char value[4];
  String valueAsString;
  valueAsString = dtostrf(convertValue, 1, 0, value);
  //Serial.println(valueAsString);
  // valueAsString = String(value);
  return valueAsString;
}
