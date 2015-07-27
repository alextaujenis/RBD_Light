// include files
#include <Light.h>

// initialize variables
Light LED(13);
unsigned long on_timer;
unsigned long off_timer;
int timeout = 1000;

void turnLightOn() {
  LED.on();
  off_timer = millis();
}

void turnLightOff() {
  LED.off();
  on_timer = millis();
}

void setup() {
  turnLightOn();
}

void loop() {
  if(LED.isOn() && millis() - off_timer > timeout) {
    turnLightOff();
  }
  if(LED.isOff() && millis() - on_timer > timeout) {
    turnLightOn();
  }
}