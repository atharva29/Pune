#include<SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  mySerial.begin(19200);
  Serial.begin(19200);
  delay(500);
}

char valChar[15];
void loop() {
  readLaser();
}

void readLaser() {

  mySerial.print("D");
  delay(10);
  while (!mySerial.available());
  String val = mySerial.readString();
  //Serial.println(val);
  val.toCharArray(valChar, val.length());

  if (val.length() > 8) {
    val.remove(0,1);
  }

  for (int i = 0 ; i < val.length() ; i++ ) {
    if (valChar[i] == 'm' ) {
      val.remove(i - 1, val.length());
      if (val.charAt(0) == ':' || val.charAt(0) == ' '  ) {
        val.remove(0, 1);
      }
      Serial.println("1,laser," + String(val));
      break ;
    }
  }
}
