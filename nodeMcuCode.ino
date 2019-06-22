#include <ESP8266WiFi.h>


int count = 1 ; //  variable for implementing state machine
String ssid = "ANB Cabin" ; // wifi ssid
String password = "Sysfem1987"; // wifi password
String ip = "192.168.0.102" ; // ip address of server
int port = 8090 ; // port of server

WiFiClient client1; //client object

/*function string2char
  This function converts string to char array */
char* string2char(String command) {
  if (command.length() != 0)
  {
    char*p = const_cast<char*>(command.c_str());
    return p;
  }
}

/*function clientMode
  This function does 2 steps
  STEP 1 - In step 1 when count is 0 it tries to connect with wifi and then to server and after connection
  count is equal to 1 i.e count = 1 .
  STEP 2 - In step 2 , count is 1 if it is connected to server then it sends data to port or if data
  is available from server it prints data serially .
  and if connection with server is lost we again jump to first step by making count = 0 .
*/
void clientMode() {
  if (count == 0 ) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(string2char(ssid), string2char(password) );

    // Tries to connect with WiFi
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(250);
    }
    delay(100);

    //Tries to connect with server
    while (!client1.connect(string2char(ip), port))
    {
      Serial.println("Connecting to server");
      delay(500);
    }
    count = 1 ; // go to count = 1
  } else if (count == 1 ) { // connection to server is successful
    if (client1.connected()) {
      if (Serial.available()) {
        String data = Serial.readStringUntil('\n');
        Serial.flush();
        Serial.println(data);
        client1.println(data); // send data to server
      }

    } else { // if connection is lost then go to count = 0
      count = 0 ;
    }
  }
}


void setup() {
  Serial.begin(19200);
}

void loop() {
  clientMode();
}
