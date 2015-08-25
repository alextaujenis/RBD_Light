// Arduino Light Library Example - Slowly pulse the Arduino LED in real-time
// Copyright 2015 Alex Taujenis
// MIT License

#include <Timer.h>
#include <Light.h>

Light light(13);

void setup() {
  light.pulse(2500,250,2000,750,1000);
}

void loop() {
  light.update();
}