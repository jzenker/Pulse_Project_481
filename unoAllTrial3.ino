//Josephine Zenker
//Code modified from Sparkfun MAX3010x Pulse and Proximity Sensor Library
//Last modified: March 19, 2020
//Code for serial communication Arduino Uno (sender)

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <math.h>
#include <Adafruit_NeoPixel.h>

#include<SoftwareSerial.h>

//Character array to store the incoming data
//Large enough to hold the longest string sent


SoftwareSerial ArduinoUno ( 12, 11); //RX: 2, TX: 3

MAX30105 particleSensor; //heart rate monitor object

const byte RATE_SIZE = 4; //Increase this for more averaging samples
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat;  //Time at which the last beat occurred

float beatsPerMinute;
float beatAvg; //raw value of HR
float oldValue; //old value of HR

const int PIXEL_COUNT = 80;
const int NUM_STRIPS = 2;
const int NUM_BUTTONS = 2;

const int Pins[NUM_STRIPS] = {7, 5};
const int Buttons[NUM_BUTTONS] = {6, 4};

char beatAvgTemp [100]; //temp chary array for hr values

Adafruit_NeoPixel Strips[NUM_STRIPS] = {
  Adafruit_NeoPixel(PIXEL_COUNT, Pins[0], NEO_RGB),
  Adafruit_NeoPixel(PIXEL_COUNT, Pins[1], NEO_GRB),
};

uint32_t red = Adafruit_NeoPixel::Color(255, 0, 0);
uint32_t green = Adafruit_NeoPixel::Color(0, 255, 0);
uint32_t blue = Adafruit_NeoPixel::Color(0, 0, 255);
uint32_t yellow = Adafruit_NeoPixel::Color(127, 127, 0);
uint32_t white = Adafruit_NeoPixel::Color(127, 127, 127);
uint32_t purple = Adafruit_NeoPixel::Color(255, 0, 175);
uint32_t orange = Adafruit_NeoPixel::Color(255, 40, 0);
uint32_t cyan = Adafruit_NeoPixel::Color(0, 175, 100);
uint32_t pink = Adafruit_NeoPixel::Color(255, 50, 50);
uint32_t black = Adafruit_NeoPixel::Color(0, 0, 0);

#define arraysize 11
const char* COLORS[11] = {"red", "blue", "green", "white", "purple", "pink", "cyan", 
"orange", "yellow", "black"}; 


bool oldState = HIGH;
bool oldState2 = HIGH;
int showType = 0;


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


  pinMode(Buttons[0], INPUT_PULLUP);
  pinMode(Buttons[1], INPUT_PULLUP);


  for (int strip = 0; strip < NUM_STRIPS; strip++) {
    Strips[strip].begin();
  }


}

void loop() {
  readBPM();
  lightShow();

}

void readBPM() {
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

    //Serial.println(beatAvg); 
    send_float(beatAvg); 


    /*

    dtostrf(beatAvg, 3, 2, beatAvgTemp);
    Serial.println(beatAvgTemp);
    ArduinoUno.println(beatAvgTemp);
    //ArduinoUno.println(beatAvg or beatAvgTemp);
    */

  }
 // return beatAvg; 
}


void lightShow() {

  // Get current button state.
  bool newState = digitalRead(Buttons[0]);
  bool newState2 = digitalRead(Buttons[1]);

  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(Buttons[0]);
    if (newState == LOW) {
      showType++;
      if (showType > 9)
        showType = 0;
      startShow(showType);

    }
  }

  if (newState2 == LOW && oldState2 == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState2 = digitalRead(Buttons[1]);
    if (newState2 == LOW) {
      showType++;
      if (showType > 9)
        showType = 0;
      startShow(showType);

    }
  }

  // Set the last button state to the old state.
  oldState = newState;
  oldState2 = newState2;

}

void startShow(int i) {
  // String currentColor = Allcolor(red);

  switch (i) {
    case 0: Allcolor(red);
      Serial.println("red"); 
     // Serial.println(COLORS[0]); 
      break;

    case 1: Allcolor(blue);
      //Serial.println("blue"); 
      whichColor(1); 
      break;

    case 2: Allcolor(green);
      Serial.println("green");
      break;

    case 3: Allcolor(white);
      Serial.println("white");
      break;

    case 4: Allcolor(purple);
      Serial.println("purple");
      break;

    case 5: Allcolor(pink);
      Serial.println("pink");
      break;

    case 6: Allcolor(cyan);
      Serial.println("cyan");
      break;

    case 7: Allcolor(orange);
      Serial.println("orange");
      break;

    case 8: Allcolor(yellow);
      Serial.println("yellow");
      break;

    case 9: Allcolor(black);
      Serial.println("black");

      break;
  }
  
}


void whichColor(int index) {

  Serial.println(COLORS[index]); 
  
}


//edited from mild lee
void Allcolor(uint32_t c) {


  int fadeControl = 150;
  int fadeDir = -1;
  int fadeStep = 10;


  for (int i = 0; i < PIXEL_COUNT; i++)
  {
    Strips[1].setPixelColor(i, c);
    Strips[1].setBrightness(fadeControl);
    Strips[1].show();

    Strips[0].setPixelColor(i, c);
    Strips[0].setBrightness(fadeControl);
    Strips[0].show();

    fadeControl = fadeControl - fadeDir;
    if (fadeControl < 0 || fadeControl > 255) {
      fadeDir = fadeDir * -1;
      fadeControl = fadeControl + fadeDir;
    }
    delay(fadeStep);
  }
}




void send_float(float arg) {
  byte * data = (byte *) &arg;
  ArduinoUno.write(data, sizeof(arg));
  Serial.println(beatAvg); //this is able to print here bc of the return from the hr 
}
