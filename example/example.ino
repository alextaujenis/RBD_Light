// include files
#include <Light.h>

// initialize variables
Light LED(13);

void setup() {
  // setup code that runs once
}

void loop() {
  // main code that runs repeatedly
  LED.on();
  delay(1000);
  LED.off();
  delay(1000);
}