// Arduino RBD Light Library v2.1.8 Example - Blink the Arduino LED without delay.
// https://github.com/alextaujenis/RBD_Light
// Copyright (c) 2015 Alex Taujenis - MIT License

#include <RBD_Timer.h> // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Light.h> // https://github.com/alextaujenis/RBD_Light

RBD::Light light(3);

void setup() {
  light.blink(250,250);
}

void loop() {
  light.update();
}