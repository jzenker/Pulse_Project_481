//Senior Project code: Josephine Zenker
//NodeMCU: Receiver & WiFi Connection 
//Serial input basics modified from Robin2: https://forum.arduino.cc/index.php?topic=288234.0 
//This code formats the received information, along with a user id, to be inserted into the 
//proper columns inside of the PHP insert script. When inserted correctly, it displays 
//on a webpage. 

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include<SoftwareSerial.h>
#include <string.h>

//How to communicate from NodeMCU to Arduino 
SoftwareSerial NodeMCU (D2, D1); //RX: D2, TX: D1

const char* ssid = "MySpectrumWiFid0-2G";
const char* password = "bravebread422";

const char* host = "http://arden.cs.unca.edu/~jzenker/jzenker_481B/scripts/insert.php?";
char result[63];

const byte numChars = 63;
char receivedChars[numChars];

char *finalString;
char *hr;
char *color;


WiFiClient client;
HTTPClient http;


boolean newData = false;


void setup() {
  
  Serial.begin(115200); //enable Serial Monitor
  NodeMCU.begin(9600); //enable SUART Port

  WiFi.mode(WIFI_OFF); //Prevents issue of reconnecting
  delay(1000);
  WiFi.mode(WIFI_STA); //Hides ESP as hotspot
  WiFi.begin(ssid, password);

}

void loop() {

  if (millis() % 2000 == 0) { //Millis library gets rid of timing issues between boards 
    recvWithEndMarker();
    showNewData();


  }
}

//Funtion modified from Robin2 
void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (NodeMCU.available() > 0 && newData == false) {
    rc = NodeMCU.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}



void showNewData() {
  if (newData == true) {
    finalString = receivedChars; //or could just print out received Chars


    //Separates tokens into variables
    color = strtok(finalString, ":"); 
    hr = strtok (NULL, "/");

    printf("%s\n", color);
    printf("%d\n", hr);
 


    sprintf(result, "&color=%s&hr=%d", color, hr);
    //Serial.println(result);


    String data;

    //Insert information into data string.
    data = String("user_id=") + "lJohnson" + String("&hr_value=") + hr +  String("&color=") + result; //multiple variables


    //Concatenate strings to insert data to the right location on the website.
    bool httpResult = http.begin(host + data);
    if (!httpResult) {
      Serial.println("Invalid HTTP request");
    }

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println(httpCode);
      String payload = http.getString();
      Serial.println(payload);
    }

    http.end(); //close connection 

    newData = false;


  }
}
