// Arduino RBD Light Library Example - Blink the Arduino LED in real-time with no delay
// Copyright 2015 Alex Taujenis
// MIT License

#include <RBD_Timer.h>
#include <RBD_Light.h>

RBD::Light light(13);

void setup() {
  light.blink(100,100);
}

void loop() {
  light.update();
}