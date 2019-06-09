#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>



int s0 = 2 ;
int s1 = 0 ;
int s2 = 4 ;
int s3 = 5 ;
int stepCount = 0 ;

int indicatorRelay = 8 ;
int jioRelay = 9 ;

float L1 , L2 , L3 , L4 , L5 , L6 ;


const char* ssid = "ANB Cabin";
const char* password = "Sysfem1987";
String temp = "" ;
int count = 0 ; //  variable for implementing state machine

void connectToWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void sendDataToThingsSpeak(String link) {
  HTTPClient http;    //Declare object of class HTTPClient
  http.begin(link);     //Specify request destination
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload
  if (httpCode == HTTP_CODE_OK) {
    Serial.println(payload);    //Print request response payload
  } else {
    Serial.println(httpCode);   //Print HTTP return code
  }
  http.end();  //Close connection
  delay(10000);  // 1min to send only 1 data in 1 hour
}

void setup() {
  Serial.begin(19200);
  delay(100);
}

void writeLogic( int m3 , int m2 , int m1 , int m0 ) {
  digitalWrite(s3, m3);
  digitalWrite(s2, m3);
  digitalWrite(s1, m3);
  digitalWrite(s0, m3);
  delay(50);
}


void loop() {

  switch (stepCount) {
    case 0 : {
        connectToWifi();
        delay(500);
        stepCount = 1 ;
        break ;
      }
    case 1 : {
        writeLogic(0, 0, 0, 0);
        L1 = analogRead(A0) * 50.00 / 1024;
        writeLogic(0, 0, 0, 1);
        L2 = analogRead(A0) * 50.00 / 1024;
        writeLogic(0, 0, 1, 0);
        L3 = analogRead(A0) * 50.00 / 1024;
        writeLogic(0, 0, 1, 1);
        L4 = analogRead(A0) * 50.00 / 1024;
        writeLogic(0, 1, 0, 0);
        L5 = analogRead(A0) * 50.00 / 1024;
        writeLogic(0, 1, 0, 1);
        L6 = analogRead(A0) * 50.00 / 1024;
        stepCount = 2 ;
        break ;
      }
    case 2 : {

        String Link = "http://api.thingspeak.com/update?api_key=OBRCM986KNTXI6AX&field1=" + String(L1) + "&field2=" + String(L2) + "&field3=" + String(L3) + "&field4=" + String(L4) + "&field5=" + String(L5) + "&field6=" + String(L6);
        sendDataToThingsSpeak(Link);
        delay(3000);
        stepCount = 1 ;
        break ;
      }
  }
}
