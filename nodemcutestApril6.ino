

//Josephine Zenker
//Last modified: March 19, 2020
//Code for serial communication NodeMCU (receiver)

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include<SoftwareSerial.h>

SoftwareSerial NodeMCU (D2, D1); //SRX  = DPin-D2; STX = DPin-D1

/*My network credentials*/
const char* ssid = "MySpectrumWiFid0-2G";
const char* password = "bravebread422";

/*Webserver address*/
const char* host = "http://arden.cs.unca.edu/~jzenker/jzenker_481B/scripts/insert.php?user_id=jzenker";

char beatAvgTemp [100];
String content = "";

void setup() {
  delay(1000);

  Serial.begin(115200); //enable Serial Monitor
  NodeMCU.begin(115200); //enable SUART Port

  WiFi.mode(WIFI_OFF); //Prevents issue of reconnecting
  delay(1000);
  WiFi.mode(WIFI_STA); //Hides ESP as hotspot
  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting");
  /*Print "..." until connected */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  /*If connection is successful, show IP address in serial*/
  Serial.println("");
  Serial.print("Connected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //IP address assigned to ESP


}

void loop() {
 /*

  String value;
  value = NodeMCU.readString();
  Serial.println(value); 



 

    if (c.length() > 0) {


    }
  */




  // String data = ("&hr_value=") + beatAvg;


  delay(3000); //liked 3000

  //read and write as same speed
  //real time
  //sync script
  //real time kernel times off crystal
  //rudementray real term kernel
  //rtkernel
  //nodemcu arduino clear buffer
  //activate hardware flow control

  /*
    HTTPClient http;

    bool httpResult = http.begin(host);
    if (!httpResult) {
      Serial.println("Invalid HTTP request");
    }

    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //content type header


    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println(httpCode);
      String payload = http.getString();
      Serial.println(payload);
    }

    http.end();
    // Serial.println("closing connection");*/

}
