#include <avr/sleep.h>
#include <avr/power.h>

#define LED_PIN 13

volatile int wdt_count = 1;
volatile int f_wakeup = 0;

void setup() {
  // Clear the WDT flag in the MCU Status Register
  MCUSR &= ~(_BV(WDRF));
  
  Serial.begin(9600);
  Serial.println("Initialising...");
  delay(100); //Allow for serial print to complete.

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("Initialisation complete.");
  delay(100); //Allow for serial print to complete.

  setupWDT();
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
  WDTCSR = (1<<WDP2 | 1<<WDP1); // Table 11-2. Watchdog Timer Prescale Select
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
  enterSleep();
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
}

// Watchdog Interrupt Service. 
// This function is executed when watchdog times out.
ISR(WDT_vect) {
    // keep arduino sleeping for 60s
  if(wdt_count < 60)
    wdt_count++;
  else {
    f_wakeup = 1;
    wdt_count = 1;
  }
}

void enterSleep(void) {
  /* Disable the peripherals. */
  power_all_disable();

  while(f_wakeup == 0) {
    sleep_enable();
    sleep_cpu(); // Enter sleep mode
    sleep_disable(); // The program will continue from here after WDT timeout
  }
  f_wakeup = 0;

  /* Re-enable the peripherals. */
  power_all_enable();
}

