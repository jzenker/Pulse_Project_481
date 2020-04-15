#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include<SoftwareSerial.h>
#include <string.h>

SoftwareSerial NodeMCU (D2, D1); //RX: D2, TX: D1

const char* ssid = "MySpectrumWiFid0-2G";
const char* password = "bravebread422";

//char host[] = "http://arden.cs.unca.edu/~jzenker/jzenker_481B/scripts/insert.php?user_id=jzenker&hr_value=100";
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

  if (millis() % 2000 == 0) {
    recvWithEndMarker();
    showNewData();


  }
}

//----code from robin2
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


    //---WORKING
    //Separates tokens into variables


    color = strtok(finalString, ":"); 
    hr = strtok (NULL, "/");

    //while (p != NULL) {}
    printf("%s\n", color);
    printf("%d\n", hr);
 


    sprintf(result, "&color=%s&hr=%d", color, hr);
    //Serial.println(result);


    ////---------Working but with strings, which is not ideal DO NOT MESS WITH IT

    String data;


    //100 is a test string 
    data = String("user_id=") + "lJohnson" + String("&hr_value=") + hr +  String("&color=") + result; //multiple variables


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

    http.end();

    newData = false;



  }
}
