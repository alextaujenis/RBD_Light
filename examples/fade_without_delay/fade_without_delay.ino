// Arduino RBD Light Library v2.1.7 Example - Fade the Arduino LED without delay.
// https://github.com/alextaujenis/RBD_Light
// Copyright 2016 Alex Taujenis
// MIT License

#include <RBD_Timer.h> // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Light.h> // https://github.com/alextaujenis/RBD_Light

RBD::Light light(3);

void setup() {
  light.fade(500,250,500,250);
}

void loop() {
  light.update();
}