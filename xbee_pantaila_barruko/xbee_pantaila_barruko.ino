#include <SoftwareSerial.h>
#include "U8glib.h"
#include "SparkFunHTU21D.h"
#include <Process.h>

SoftwareSerial xbee(8, 9); //Rx, Tx
U8GLIB_PCD8544 u8g(6, 5, 4); // SPI HW com: CS = 6, A0 D/C = 5, Reset = 4
HTU21D sentsorea;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Arduino Yun abiarazi
  //while(!Serial) {}

  Bridge.begin();
  
  xbee.begin(9600); // Xbee abiarazi
  sentsorea.begin(); // HTU21D abiarazi
  u8g.setRot180(); // Pantailari buelta emateko

}

void loop() {   
  // put your main code here, to run repeatedly:
  if(xbee.available()>0) {
    float hezeK = xbee.parseFloat(); // Xbee-tik zenbakizko datuak badatoz humb-en gorde
    float tenpK = xbee.parseFloat(); // Xbee-tik zenbakizko datuak badatoz temp-en gorde
    Serial.println("JASOTAKO DATUAK"); // Monitor Seriean ikusteko
    Serial.print("  Hezetasuna: "); Serial.println(hezeK); // Monitor Seriean ikusteko
    Serial.print("  Tenperatura: "); Serial.println(tenpK); // Monitor Seriean ikusteko

    float hezeB = sentsorea.readHumidity();
    float tenpB = sentsorea.readTemperature();
    Serial.println("BARRUKO DATUAK"); // Monitor Seriean ikusteko
    Serial.print("  Hezetasuna: "); Serial.println(hezeB); // Monitor Seriean ikusteko
    Serial.print("  Tenperatura: "); Serial.println(tenpB);
    
    // picture loop
    u8g.firstPage();  // Pantailako lehengo orrian idazteko
    do {
      draw(hezeK, tenpK, hezeB, tenpB);  // Panatailan idatzi beharreko aldagaiak
    } while( u8g.nextPage() ); // Hurrengo orrira salto egiteko

    // datuen bidalketa hodeira
    uploadData2Cloud(hezeK, tenpK, hezeB, tenpB);
  }
}

void draw(float hezeK, float tenpK, float hezeB, float tenpB) {
  u8g.setFont(u8g_font_courB08); // Pantailako letra mota
  u8g.drawStr(11, 13, "INT."); // Pantailan idatzi beharrekoa eta bere posizioa
  u8g.drawStr(55, 13, "EXT."); // Pantailan idatzi beharrekoa eta bere posizioa
  u8g.drawLine(44, 0, 44, 48); // Pantailan marrak marrazteko
  u8g.drawLine(0, 14, 84, 14); // Pantailan marrak marrazteko
  u8g.setPrintPos(4, 30); // Datorren datuaren posizioa
  u8g.print(tenpB, 1); // Ze barruko datu idatzi eta bere dezimalak
  u8g.setPrintPos(4, 45); // Datorren datuaren posizioa
  u8g.print(hezeB, 1); // Ze barruko datu idatzi eta bere dezimalak
  u8g.setPrintPos(48, 30); // Datorren datuaren posizioa
  u8g.print(tenpK, 1); // Ze kanpoko datu idatzi eta bere dezimalak
  u8g.setPrintPos(48, 45); // Datorren datuaren posizioa
  u8g.print(hezeK, 1); // Ze kanpoko datu idatzi eta bere dezimalak
}

void uploadData2Cloud(float hezeK, float tenpK, float hezeB, float tenpB) {
  Process prozesua;
  prozesua.begin("python");
  prozesua.addParameter("/root/thingspeak3.py");
  prozesua.addParameter(String(hezeK));
  prozesua.addParameter(String(tenpK));
  prozesua.addParameter(String(hezeB));
  prozesua.addParameter(String(tenpB));
  prozesua.runAsynchronously();
  unsigned long start = millis();
  while(prozesua.running()) {
    unsigned long now = millis();
    if((now - start) > 3000)
      prozesua.close();
    else
      delay(100);
  }
}

