#include <SoftwareSerial.h>

SoftwareSerial xbee(9, 10);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);  
  
  xbee.begin(9600);
  delay(1000);

  xbee.print("+++");
  Serial.println("+++");
  delay(10000);
  while(xbee.available()>0) {
    Serial.print(char(xbee.read()));
  }
  Serial.println();
 
  xbee.print("ATSM1"); xbee.write(0x0D);
  Serial.println("ATSM1"); 
  delay(5000);
  while(xbee.available()>0) {
    Serial.print(char(xbee.read()));
  }
  Serial.println();
  /*
  xbee.print("ATWR"); xbee.write(0x0D);
  Serial.println("ATWR"); 
  delay(5000);
  while(xbee.available()>0) {
    Serial.print(char(xbee.read()));
  }
  Serial.println();
  */
  xbee.print("ATCN"); xbee.write(0x0D);
  Serial.println("ATCN");
  delay(5000);
  while(xbee.available()>0) {
    Serial.print(char(xbee.read()));
  }
  Serial.println();


}

void loop() {
  // put your main code here, to run repeatedly:
  delay(5000);
  digitalWrite(6, HIGH);
  delay(5000);
  digitalWrite(6, LOW);
}
