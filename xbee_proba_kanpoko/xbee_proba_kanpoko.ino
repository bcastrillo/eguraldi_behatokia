#include <SoftwareSerial.h>
//#include "SparkFunHTU21D.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <DHT.h>

#define LED_PIN 13
#define DHTPIN 5
#define DHTTYPE DHT22  //DHT22 (AM2302)

SoftwareSerial xbee(9, 10); //Rx, Tx
//HTU21D sentsorea;
DHT dht(DHTPIN, DHTTYPE);

volatile int wdt_count = 1;
volatile int f_wakeup = 0;

void setup() {
  // Clear the WDT flag in the MCU Status Register
  MCUSR &= ~(_BV(WDRF));

  // put your setup code here, to run once:
  Serial.begin(9600);

  xbee.begin(9600); // Xbee abiarazi
  setupXbee();

//  sentsorea.begin(); // HTU21D abiarazi
  dht.begin(); // DHT abiarazi

  Serial.println("setup() ProMini ondo"); // Monitor Serie panatailan inprimatzeko
  setupWDT();
}

void setupXbee() {
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
  delay(3000);
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
  delay(3000);
  while(xbee.available()>0) {
    Serial.print(char(xbee.read()));
  }
  Serial.println();
}


void setupWDT() {
  // ATmega328p manual
  // Setup the WatchDog Timer (WDT) as indicated in section 11.8
  // The WDT Control Register (WDTCSR) is described in section 11.9.2
  // Alterations to the Watchdog set-up must follow timed sequences:
  // 1.- In the same operation, write a logic one to the Watchdog change enable bit (WDCE) and WDE.
  WDTCSR |= (_BV(WDCE) | _BV(WDE));

  // 2.- Within the next four clock cycles, write the WDE and Watchdog prescaler bits (WDP) as desired,
  //     but with the WDCE bit cleared. This must be done in one operation.
  WDTCSR = (1 << WDP2 | 1 << WDP1); // Table 11-2. Watchdog Timer Prescale Select
  // WDP3 WDP2 WDP1 WDP0 Number of WDT Oscillator Cycles Typical Time-out at VCC = 5.0V
  // 0    1    1    0    128K (131072) cycles            1.0s

  // Table 11-1. Watchdog Timer Configuration
  // WDTON WDE WDIE Mode           Action on Time-out
  // 0     0   1    Interrupt Mode Interrupt
  //                               The "WDT_vect" ISR (Interrupt Service Routine) will be called when the WDT times out.
  WDTCSR |= _BV(WDIE); //

  // Table 10-1. Active Clock Domains and Wake-up Sources in the Different Sleep Modes.
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // maximum power save
}

void loop() {
  // put your main code here, to run repeatedly:

  float hezeK = dht.readHumidity(); //HTU21D-tik hezetasuna neurtu eta humd aldagaian gorde
  float tenpK = dht.readTemperature(); //HTU21D-tik tenperatura neurtu eta temp aldagaian gorde

  if (isnan(hezeK) || isnan(tenpK)) {
    Serial.print("Error");
    return;
  }

  //float hezeK = 2.0; //Sentsore barik aproba egiteko
  //float tenpK = 1.0; //Sentsore barik aproba egiteko
  digitalWrite(6, LOW);
  delay(50);
  Serial.print(hezeK); Serial.print(" "); Serial.println(tenpK);
  xbee.print(hezeK); // Xbee-tik bidali beharreko aldagaia
  xbee.print(" "); // Xbee-tik bidali beharreko datua
  xbee.print(tenpK); // Xbee-tik bidali beharreko aldagaia
  xbee.flush();
  delay(1000);
  digitalWrite(6, HIGH);

  enterSleep();

  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  //delay(5000); // 5segunduro bidali datuak
}

// Watchdog Interrupt Service.
// This function is executed when watchdog times out.
ISR(WDT_vect) {
  // keep arduino sleeping for 60s
  if (wdt_count < 20)
    wdt_count++;
  else {
    f_wakeup = 1;
    wdt_count = 1;
  }
}

void enterSleep(void) {
  /* Disable the peripherals. */
  power_all_disable();

  while (f_wakeup == 0) {
    sleep_enable();
    sleep_cpu(); // Enter sleep mode
    sleep_disable(); // The program will continue from here after WDT timeout
  }
  f_wakeup = 0;

  /* Re-enable the peripherals. */
  power_all_enable();
}

