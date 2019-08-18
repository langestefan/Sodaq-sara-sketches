#include <Wire.h>
#include <Sodaq_wdt.h>
#include <Sodaq_LSM303AGR.h>

Sodaq_LSM303AGR accelerometer;

void setup() {

  // Bootup delay to programm the board.
  delay(5000);

  Wire.begin();// I2C for the accelerometer

  // Disable the LSM303AGR
  accelerometer.disableAccelerometer();
  accelerometer.disableMagnetometer();

  pinMode(MAG_INT, OUTPUT);
  pinMode(GPS_ENABLE, OUTPUT);
  pinMode(SARA_ENABLE, OUTPUT);
  pinMode(SARA_TX_ENABLE, OUTPUT);

  digitalWrite(MAG_INT, LOW); // we need to make this low otherwise this pin on the LSM303AGR starts leaking current
  digitalWrite(GPS_ENABLE, LOW); // low=poweredoff, high=poweredon
  digitalWrite(SARA_ENABLE, LOW); // low=poweredoff, high=poweredon
  digitalWrite(SARA_TX_ENABLE, LOW); // low=poweredoff, high=poweredon

  //this code is needed to setup watchdogtimer and to make MCU sleep
  sodaq_wdt_enable(WDT_PERIOD_8X); // watchdog expires in ~8 seconds
  sodaq_wdt_reset(); // restting the watchdog

  initSleep();

  SerialUSB.flush();
  SerialUSB.end();
  USBDevice.detach();
  USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE; // Disable USB
}

void loop() {
  sodaq_wdt_reset(); // restting the watchdog
  systemSleep();
}

/**
  Initializes the CPU sleep mode.
*/
void initSleep()
{
  // Set the sleep mode
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
}

/**
  Powers down all devices and puts the system to deep sleep.
*/
void systemSleep()
{

  __WFI(); // SAMD sleep

}

/** Note: When your board is in sleep, or does not react anymore. You can get it back to life to double press the reset button.
*/
