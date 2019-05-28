#include <ESP8266WiFi.h>;
#include <ESP8266HTTPClient.h>;
char valChar[15];
int stepCount = 0 ;

const char* ssid = "hot";
const char* password = "123123123";
String ip = "192.168.43.100" ; // ip address of server
int port = 8090 ; // port of server
String temp = "" ;
int count = 0 ; //  variable for implementing state machine

WiFiClient client1; //client object
char* string2char(String command) {
  if (command.length() != 0)
  {
    char*p = const_cast<char*>(command.c_str());
    return p;
  }
}

void connectToWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  while (!client1.connect(string2char(ip), port))
  {
    Serial.println("*");
    delay(500);
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
  pinMode(15, OUTPUT);
  digitalWrite(15, LOW);
  delay(1000);
  digitalWrite(15, HIGH);
  delay(1000);

}

void loop() {

  switch (stepCount) {
    case 0 : {
        connectToWifi();
        delay(500);
        stepCount = 1 ;
      }
    case 1 : {

        Serial.flush();
        Serial.print("D");
        stepCount = 2 ;
        break ;
      }
    case 2 : {
        if (Serial.available()) {

          String val = Serial.readString();
          Serial.flush();
          delay(100);
          val.toCharArray(valChar, val.length());
          if (val.length() > 8) {
            val.remove(0, 2);
          }
          for (int i = 0 ; i < val.length() ; i++ ) {
            if (valChar[i] == 'm' ) {
              val.remove(i - 2, val.length());
              Serial.println();
              Serial.println( String(val));
              Serial.flush();
              if (client1.connected()) {
                client1.println("1,laser," + String(val));
              } else { 
                // if connection is lo st then go to count = 0
                stepCount = 0 ;
              }
              String Link = "http://api.thingspeak.com/update?api_key=OBRCM986KNTXI6AX&field1=" + String(val);
              sendDataToThingsSpeak(Link);
              delay(5000);
            }
          }
          stepCount = 1 ;
        }
        break ;
      }
  }
}

