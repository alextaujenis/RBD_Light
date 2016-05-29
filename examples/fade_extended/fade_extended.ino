// Arduino RBD Light Library v2.1.7 Example - Fade the Arduino LED without delay.
// https://github.com/alextaujenis/RBD_Light
// Copyright 2016 Alex Taujenis
// MIT License

#include <RBD_Timer.h> // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Light.h> // https://github.com/alextaujenis/RBD_Light

RBD::Light light(5, true);                 // use brightness correction
int x = 0;  

void setup() {
  Serial.begin(38400);                     // open the serial port at 38400 bps:  
  light.fade(1000,1000,1000,1000,50,150);  // fade from 50 to 150 brightness 
  //light.blink(1000,1000,50,150);         // blink from 50 to 150 brightness 
}

void loop() {

  // Benchmarking light.update()
  unsigned long time1 = millis();
  for(x=0; x< 1000; x++){
     light.update();     
  }
  unsigned long timeDelta = millis() - time1;
  Serial.print("microsecs per light.update(): "); 
  Serial.println(timeDelta);
}
