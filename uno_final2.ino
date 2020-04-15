#include <Adafruit_NeoPixel.h>
#include<SoftwareSerial.h>
#include "MAX30105.h"
#include "heartRate.h"
MAX30105 particleSensor;
#include <Wire.h>

const int PIXEL_COUNT = 100;
const int NUM_STRIPS = 3;
const int NUM_BUTTONS = 2;

const int Pins[NUM_STRIPS] = {7, 5, 3};
const byte Buttons[NUM_BUTTONS] = {6, 4};

//----For combining two strips into one
Adafruit_NeoPixel Strips[NUM_STRIPS] = {
  Adafruit_NeoPixel(PIXEL_COUNT, Pins[0], NEO_RGB),
  Adafruit_NeoPixel(PIXEL_COUNT, Pins[1], NEO_GRB),
  Adafruit_NeoPixel(PIXEL_COUNT, Pins[2], NEO_GRB)
};

SoftwareSerial ArduinoUno ( 11, 12); //RX: 2, TX: 3


///--------COLORS--------
uint32_t red = Adafruit_NeoPixel::Color(255, 0, 0);
uint32_t cyan = Adafruit_NeoPixel::Color(0, 175, 100);
uint32_t yellow = Adafruit_NeoPixel::Color(255, 200, 0);
uint32_t black = Adafruit_NeoPixel::Color(0, 0, 0);


char* myTemplate = "";
const char* currentColor;
//char data[200];
char result[63];
char hrStr[10];

const char* COLORS[] = {"Black", "Cyan", "Yellow", "Red", "Party Mode"}; //for string literals


///-----END OF COLORS-----

unsigned long patternInterval = 20 ; // time between steps in the pattern
unsigned long sendInterval = 2000;


///----Used for creating a finite state machine
unsigned long currentMillis = 0;
unsigned long previousButtonMillis = 0 ; // for currentMillis when last update occoured
unsigned long previousButtonMillis_2 = 0;
unsigned long previousButtonMillis_3 = 0; //when last update occured for hr monitor

unsigned long intervals [] = { 1, 1, 1, 1 } ; // speed for each pattern


void setup() {

  Serial.begin(115200);
  ArduinoUno.begin(9600);

  wipe(); // wipes the LED buffers
  pinMode(Buttons[0], INPUT_PULLUP); // change pattern for both strips with this button

  for (int strip = 0; strip < NUM_STRIPS; strip++) { //combines two strips into one
    Strips[strip].begin();
  }
}



void loop() {

  int hrValue = hrNumberGenerator();

  currentMillis = millis(); //for buttons

  static int pattern = 0, lastReading;

  int reading = digitalRead(Buttons[0]);

  if (lastReading == HIGH && reading == LOW) {
    pattern++ ; // Go to next color in the COLORS array

    if (pattern > 4) {
      pattern = 0; // If too big, wraps around

      //patternInterval = intervals[pattern]; // sets the speed for this pattern
      wipe(); // clear out the buffer
      delay(50); // debounce delay
    }
  }

  lastReading = reading; // save for next time

  if (currentMillis - previousButtonMillis > patternInterval) {
    updatePattern(pattern);

    //---this can be in this if or the next if
    currentColor = COLORS[pattern]; //stores next color into the led

    previousButtonMillis = currentMillis;
  }

  if (currentMillis - previousButtonMillis_2 > sendInterval) {
    //use ":" as delimiter to split
    myTemplate = "%s:%d/";
    sprintf(result, myTemplate, currentColor, hrValue);
    ArduinoUno.println(result);
    Serial.println(result);


    previousButtonMillis_2 = currentMillis;
  }

}

int hrNumberGenerator() {
  int randNumber = random(70, 110);
  return randNumber;

}


///---HR monitor that does not work with the timing limitations of the Arduino
/*
  void hrVal() {
  long irValue = particleSensor.getIR();

  hrStartMillis = millis();

  if (checkForBeat(irValue) == true) {

    long delta = hrStartMillis - previousButtonMillis_3;

    beatsPerMinute = 60 / (delta / 1000.0);
    Serial.println(beatsPerMinute);
    previousButtonMillis_3 = hrStartMillis;
  }
  }
*/


void  updatePattern(int pat) { // call the pattern currently being created
  switch (pat) {
    case 0:
      allColor(black);
      break;
    case 1:
      allColor(cyan);
      break;
    case 2:
      allColor(yellow);
      break;
    case 3:
      allColor(red);
      break;
    case 4:
      rainbowCycle();
      break;
  }
}


void colorWipe(uint32_t c) { // modified from Adafruit example to make it a state machine
  static int i = 0;
  for (int strip = 0; strip < NUM_STRIPS; strip++) {
    Strips[strip].setPixelColor(i, c);
    Strips[strip].show();
  }
  i++;
  for (int strip = 0; strip < NUM_STRIPS; strip++) {
    if (i >= Strips[strip].numPixels()) {
      i = 0;
      wipe(); // blank out strip
    }
  }
  previousButtonMillis = currentMillis; // time for next change to the display
}

void allColor(uint32_t c) {
  for (int strip = 0; strip < NUM_STRIPS; strip++) {
    for (int i = 0; i < PIXEL_COUNT ; i++)
    {
      Strips[strip].setPixelColor(i, c);
      Strips[strip].show();


    }
    previousButtonMillis = currentMillis; // time for next change to the display
  }
}



void wipe() { // clear all LEDs
  for (int strip = 0; strip < NUM_STRIPS; strip++) {
    for (int i = 0; i < Strips[strip].numPixels(); i++) {
      Strips[strip].setPixelColor(i, Strips[strip].Color(0, 0, 0));
    }
  }
}

void rainbowCycle() { // modified from Adafruit example to make it a state machine
  static uint16_t j = 0;
  for (int strip = 0; strip < NUM_STRIPS; strip++) {
    for (int i = 0; i < Strips[strip].numPixels(); i++) {
      Strips[strip].setPixelColor(i, Wheel(((i * 256 / Strips[strip].numPixels()) + j) & 255));
    }
  }
  for (int strip = 0; strip < NUM_STRIPS; strip++) {
    Strips[strip].show();
    j++;
  }
  if (j >= 256 * 5) j = 0;
  previousButtonMillis = currentMillis; // time for next change to the display
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    for (int strip = 0; strip < NUM_STRIPS; strip++) {
      return Strips[strip].Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    for (int strip = 0; strip < NUM_STRIPS; strip++) {
      return Strips[strip].Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
  }
  WheelPos -= 170;
  for (int strip = 0; strip < NUM_STRIPS; strip++) {
    return Strips[strip].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
