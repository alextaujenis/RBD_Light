// include files
#include <Light.h>

// static values
int BAUD = 11500;

// initialize variables
Light LED(13);
unsigned long on_timer = 0;
unsigned long off_timer = 0;
int timeout = 1000;

void setup() {
  Serial.begin(BAUD);
}

void loop() {
  if(LED.isOn() && millis() - off_timer > timeout) {
    turnLightOff();
  }
  if(LED.isOff() && millis() - on_timer > timeout) {
    turnLightOn();
  }
}

// local functions

void turnLightOn() {
  LED.on();
  off_timer = millis();
}

void turnLightOff() {
  LED.off();
  on_timer = millis();
}