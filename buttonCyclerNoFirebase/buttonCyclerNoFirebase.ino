//Formally known as ButtonPressTestBeforeFirebase2

#include <Adafruit_NeoPixel.h>

#define PIN D1 //data pin
#define NUM_LEDS 8 //8 leds beig used in the strip 
#define BUTTON_PIN D3 //pin that changes button 

Adafruit_NeoPixel strip = Adafruit_NeoPixel (NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

boolean oldState = HIGH; 
int mode = 0;  
int numModes = 3; //number of color modes 

void setup() {
  Serial.begin(9600); 
  pinMode(BUTTON_PIN, INPUT); 
  strip.begin(); //Initialize neopixel strip object 
  strip.show(); 
 
}

void loop() {

    boolean buttonVal = digitalRead(BUTTON_PIN); //get the current button state

    if((buttonVal == LOW) && (oldState == HIGH)) {
      delay(buttonVal);
      buttonVal = digitalRead(BUTTON_PIN); 
      if(buttonVal == LOW) {
        if(++mode > numModes) mode = 0; //go to next color, wrap around after last 
        switch(mode) {
          case 0: //red 
            for (int i=0; i<NUM_LEDS; i++) {
               strip.setPixelColor(i, 255,0,0); 
               strip.show();  
               Serial.println("red"); 
                }
            break; 

          case 1: //green
            for (int i=0; i<NUM_LEDS; i++) {
                 strip.setPixelColor(i, 0,255,0); 
                 strip.show();  
                 Serial.println("green"); 
                  }
              break; 

          case 2: //blue 
            for (int i=0; i<NUM_LEDS; i++) {
                 strip.setPixelColor(i, 0,0,255); 
                 strip.show();  
                 Serial.println("blue"); 
                  }
              break;   
          }
        }
      }
    }
